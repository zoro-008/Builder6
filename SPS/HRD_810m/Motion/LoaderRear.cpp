//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "LoaderRear.h"
//#include "RunThread.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
//#include "PstnMan.h"
#include "OptionMan.h"
//#include "LotUnit.h"
#include "UserIni.h"
#include "UserFile.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone�����.
//---------------------------------------------------------------------------
CLoaderRear LDR_R;

CGridData g_LotData ;

CLoaderRear::CLoaderRear(void)
{

}

CLoaderRear::~CLoaderRear (void)
{

}

void CLoaderRear::Init()
{
    m_sPartName = "Front Loader" ;

    g_LotData.LoadFromCsv(EXE_FOLDER + "SeqData\\LotData.csv");

    m_sCheckSafeMsg = "" ;

    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();

}

void CLoaderRear::Close()
{
    Load(false);
    g_LotData.SaveToCsv(EXE_FOLDER + "SeqData\\LotData.csv");
}

void CLoaderRear::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}


bool CLoaderRear::FindChip(int &r ,int &c , EN_CHIP_STAT _eStat)
{
    if(_eStat == csUnkwn) { //���� ����.
        if(-1 != DM.ARAY[riLDR_R].FindFrstRowLastCol(csUnkwn , r , c)){
            id = Add.riLDR ;
            return true ;
        }
    }

    return false ;
}

double CLoaderRear::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    double dPickStt ; //�۾��ؾ��� �ް����� �� �Ʒ� ���� ��ġ.
    double dPlceStt ; //�۾��ؾ��� �ް����� �� �Ʒ� ���� ��ġ.
    double dPick    ; //���� �۾��ؾ��� ������.
    double dPlce    ; //���� �۾��ؾ��� ������.
    int r , c;
    EN_ARAY_ID riWork ;

    if(FindChip(r,c,riIDX )){ //
        dPickStt = PM.GetValue(Add.miLDR_ZElev , Add.pvLDR_ZElevPickFrst) ;
        dPlceStt = PM.GetValue(Add.miLDR_ZElev , Add.pvLDR_ZElevPlceFrst) ;
        dPick    = dPickStt + r * OM.DevInfo.dMgzSlotPitch ;
        dPlce    = dPickStt + r * OM.DevInfo.dMgzSlotPitch ;
    }

    if(_iMotr == Add.miLDR_ZElev){
             if(_iPstnId ==  Add.piLDR_ZElevWait    ) dPos = PM.GetValue (_iMotr , Add.pvLDR_ZElevWait    );
        else if(_iPstnId ==  Add.piLDR_ZElevPickFrst) dPos = PM.GetValue (_iMotr , Add.pvLDR_ZElevPickFrst);
        else if(_iPstnId ==  Add.piLDR_ZElevPlceFrst) dPos = PM.GetValue (_iMotr , Add.pvLDR_ZElevPlceFrst);
        else if(_iPstnId ==  Add.piLDR_ZElevPick    ) dPos = dPick                                         ;
        else if(_iPstnId ==  Add.piLDR_ZElevPlce    ) dPos = dPlce                                         ;
        else                                          dPos = MT_GetCmdPos(_iMotr                          );
    }
    else if(_iMotr == Add.mi
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CLoaderRear::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

bool CLoaderRear::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CLoaderRear::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CLoaderRear::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: if(IO_GetX(Add.xPRI_Detect1)) {
                     EM_SetErrMsg(eiPRT_Crash , (m_sPartName + "��Ʈ �浹 ���� ���� ������").c_str());
                     Step.iToStart = 0 ;
                     return true ;
                 }

                 Step.iToStart ++ ;
                 return false ;

        case 11: //if(!MT_GetStopInpos(Add.miLDR_ZElev)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CLoaderRear::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    Stat.bReqStop = false ;

    //Move Home.
    switch (Step.iToStop) {
        default: Step.iToStop = 0;
                 return true ;

        case 10: if(IO_GetX(Add.xPRI_Detect1)){
                     Step.iCycle = 0;
                     return true;
                 }
                 MoveMotr(Add.miLDR_ZElev , Add.piLDR_ZElevWait);
                 Step.iToStop++;
                 return false;

        case 11: if(!MoveMotr(Add.miLDR_ZElev , Add.piLDR_ZElevWait))return false;
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CLoaderRear::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;


    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool isZWorkPos = IsWorkPos();

        //�������� �����ε��� �����̳�....                                                                  ��
        bool isCycleWork = (DM.ARAY[Add.riLDRTop].GetCntStat (csUnkwn) || DM.ARAY[Add.riLDRBtm].GetCntStat (csUnkwn)) &&
                           !isZWorkPos && !IO_GetX(Add.xPRI_Detect1);

                           // DM.ARAY[Add.riPRI   ].CheckAllStat(csNone) &&
                           //(DM.ARAY[Add.riIDX   ].FindFrstCol(csWork  ) <= 1 || DM.ARAY[Add.riIDX   ].CheckAllStat(csEmpty));
        bool isCycleEnd  = DM.ARAY[Add.riLDRTop].GetCntStat(csUnkwn)==0&& DM.ARAY[Add.riLDRBtm].GetCntStat(csUnkwn)==0 ;

        if(EM_IsErr()) return false ;


         //Normal Decide Step.
             if (isCycleWork  ) {Trace(m_sPartName.c_str(),"CycleWork   Stt"); Step.iSeq = scWork   ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default     End");                                                        Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                               return false ;
        case scWork  : if(CycleWork  ()){ Trace(m_sPartName.c_str(),"CycleWork   End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}
bool CLoaderRear::IsWorkPos()
{
    return MT_CmprPos(Add.miLDR_ZElev ,GetMotrPos(Add.miLDR_ZElev , Add.piLDR_ZElevWork)) ;

}

bool CLoaderRear::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        sTemp  = sTemp.sprintf(" %s TIMEOUT Step.iHome=%02d" , __FUNC__ , Step.iHome );
        sTemp  = m_sPartName + sTemp ;
        EM_SetErrMsg(eiPRT_HomeTo,sTemp.c_str());
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iHome != PreStep.iHome) {
        sTemp = sTemp.sprintf("%s Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iHome = Step.iHome ;

    if(Stat.bReqStop) {
        //Step.iHome = 0;
        //return true ;
    }

    switch (Step.iHome) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: if(IO_GetX(Add.xPRI_Detect1)) {
                      EM_SetErrMsg(eiPRT_Crash , (m_sPartName + " ���� ���Ժ� ���� ���� ���·� ��Ʈ ���� Ȯ���ϼ���").c_str());
                      Step.iHome = 0 ;
                      return true ;
                 }
                 MT_DoHome(Add.miLDR_ZElev);
                 Step.iHome++;
                 return false ;

        case 11: if(!MT_GetHomeEnd(Add.miLDR_ZElev)) return false ;
                 MT_GoAbsMan(Add.miLDR_ZElev , PM.GetValue(Add.miLDR_ZElev , Add.pvLDR_ZElevWait ));
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GoAbsMan(Add.miLDR_ZElev , PM.GetValue(Add.miLDR_ZElev , Add.pvLDR_ZElevWait ))) return false ;
                 SetLastCmd();
                 Step.iHome = 0;
                 return true ;

    }
}

//One Cycle.
bool CLoaderRear::CycleWork(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //��ž�ÿ� �׳� ����.
        //Step.iCycle = 0;
        //return true ;
    }

    int r ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case  10: if(IO_GetX(Add.xPRI_Detect1)) {
                      EM_SetErrMsg(eiPRT_Crash , (m_sPartName + " ���� ���Ժ� ���� ���� ���·� ��Ʈ ���� Ȯ���ϼ���").c_str());
                      Step.iHome = 0 ;
                      return true ;
                  }
                  MoveMotr(Add.miLDR_ZElev , Add.piLDR_ZElevWork);


                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(Add.miLDR_ZElev , Add.piLDR_ZElevWork))return false ;
                  Step.iCycle=0;
                  return true;
    }

}

//---------------------------------------------------------------------------
bool CLoaderRear::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iMotr == Add.miLDR_ZElev){
        if(IO_GetX(Add.xPRI_Detect1)) {
            sMsg = m_sPartName + " ���� ���Ժ� ���� ���� ���·� ��Ʈ ���� Ȯ���ϼ���" ;
            bRet = false;
        }
        if(MT_GetCmdPos(Add.miPRI_XIndx) > PM.GetValue(Add.miPRI_XIndx , Add.pvPRI_XWait)) {
            sMsg = m_sPartName + "���� �ε����� ��ġ�� Wait��ġ ���� �����ϴ�.";
            bRet = false;
        }
    }
    else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        m_sCheckSafeMsg = sMsg ;
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iSeq)FM_MsgOk(MT_GetName(_iMotr),sMsg.c_str());
    }
    else {
        m_sCheckSafeMsg = "" ;
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CLoaderRear::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    //if(_iActr == aiLDR_IndxUpUp){
    //    if(_bFwd == ccFwd) {
    //        //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray ���� ������!"); bRet = false ;}
    //    }
    //}
    //else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    //}


    if(!bRet){
        m_sCheckSafeMsg = sMsg ;
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr),sMsg.c_str());
    }
    else {
        m_sCheckSafeMsg = "" ;
    }

    return bRet ;
}

bool CLoaderRear::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow )
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(_bSlow) {
        return MT_GoAbsSlow(_iMotr , dPosition);
    }
    else {
        if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
        else            return MT_GoAbsMan(_iMotr , dPosition);
    }

    return false ;
}







void CLoaderRear::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // ���͸� �����϶� ���� �Լ�.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CLoaderRear::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CLoaderRear::SetLastCmd()
{
    m_dLastIdxPos = MT_GetCmdPos(Add. miLDR_ZElev);
    return ; //����Ʈ�� �ʿ� ����.

}

bool CLoaderRear::CheckMoved()
{
    if(m_dLastIdxPos != MT_GetCmdPos(Add.miLDR_ZElev)) return true  ; //����Ʈ�� �ʿ� ����.
    else                                               return false ;

}

bool CLoaderRear::CheckStop()
{
    if(!MT_GetStop(Add.miLDR_ZElev)) return false ;

    return true ;
}

void CLoaderRear::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";
    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
}





