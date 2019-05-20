//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "UnLoader.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
//#include "LotUnit.h"
#include "LoaderFront.h"
#include "UserIni.h"
#include "UserFile.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone�����.
//---------------------------------------------------------------------------
CUnLoader ULD;

//CGridData g_LotData ;

CUnLoader::CUnLoader(void)
{

}

CUnLoader::~CUnLoader (void)
{

}

void CUnLoader::Init()
{
    m_sPartName = "UnLoader" ;

    g_LotData.LoadFromCsv(EXE_FOLDER + "SeqData\\LotData.csv");

    m_sCheckSafeMsg = "" ;

    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();
}

void CUnLoader::Close()
{
    Load(false);
    g_LotData.SaveToCsv(EXE_FOLDER + "SeqData\\LotData.csv");
}

void CUnLoader::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}

bool CUnLoader::FindChip(int &r ,int &c , EN_CHIP_STAT _eStat)
{
    //if(_eStat == csEmpty) { //���� ����.
        if(-1 != DM.ARAY[riULD].FindFrstRowLastCol(_eStat , r , c)){
            return true ;
        }
    //}

    return false ;
}

double CUnLoader::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    double dPickStt ; //�۾��ؾ��� �ް����� �� �Ʒ� ���� ��ġ.
//    double dPlceStt ; //�۾��ؾ��� �ް����� �� �Ʒ� ���� ��ġ.
    double dPick    ; //���� �۾��ؾ��� ������.
//    double dPlce    ; //���� �۾��ؾ��� ������.

    int r , c;
    EN_ARAY_ID riWork ;

    if(FindChip(r, c, csEmpty)){
        dPickStt = PM.GetValue(miULD_ZElevt , pvULD_ZElevtPickFrst) ;
        dPick    = dPickStt + r * OM.DevInfo.dULDMgzSlotPitch ;
    }

//    if(FindChip(r, c, csWorkEnd)){
//        dPlceStt = PM.GetValue(miULD_ZElevt , pvULD_ZElevFPlceFrst) ;
//        dPlce    = dPlceStt + r * OM.DevInfo.dULDMgzSlotPitch ;
//    }

    if(_iMotr == miULD_ZElevt){
             if(_iPstnId ==  piULD_ZElevtWait    ) dPos = PM.GetValue (_iMotr , pvULD_ZElevtWait    );
        else if(_iPstnId ==  piULD_ZElevtPickFrst) dPos = PM.GetValue (_iMotr , pvULD_ZElevtPickFrst);
//        else if(_iPstnId ==  piULD_ZElevFPlceFrst) dPos = PM.GetValue (_iMotr , pvULD_ZElevFPlceFrst);
        else if(_iPstnId ==  piULD_ZElevtPick    ) dPos = dPick                                      ;
        else if(_iPstnId ==  piULD_ZElevFPlce    ) dPos = PM.GetValue (_iMotr , pvULD_ZElevtPlce    );
        else                                       dPos = MT_GetCmdPos(_iMotr                       );


        
        
        
    }   
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CUnLoader::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

bool CUnLoader::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;
}

bool CUnLoader::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CUnLoader::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
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

        case 10:
                 Step.iToStart ++ ;
                 return false ;

        case 11:
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CUnLoader::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
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

        case 10: //if(IO_GetX(xPRI_Detect1)){
                 //    Step.iCycle = 0;
                 //    return true;
                 //}
                 //
                 if(IO_GetX(xULD_Detect7)) { // juhyeon
                      EM_SetErrMsg(eiPRT_Crash , (m_sPartName + "���� ����� ���� ���� ���·� ���� ���� ���¸� Ȯ���ϼ���").c_str());
                      Step.iToStop = 0 ;
                      return true ;
                 }
                 MoveMotr(miULD_ZElevt , piULD_ZElevtWait);
                 Step.iToStop++;
                 return false;

        case 11: if(!MoveMotr(miULD_ZElevt , piULD_ZElevtWait))return false;
                 SetLastCmd();
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CUnLoader::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
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

        //if( DM.ARAY[riULD].CheckAllStat(csNone) &&  IO_GetX(xULD_Detect7)) {EM_SetErrMsg(eiPKG_Unknwn , "UnLoader Unknwn PKG Error."   ); return false;}
        //if(!DM.ARAY[riULD].CheckAllStat(csNone) && !IO_GetX(xULD_Detect7)) {EM_SetErrMsg(eiPKG_Dispr  , "UnLoader Disappear PKG Error."); return false;}

        //sunsun             iWorkMode
        //const int  itemp = 0 ; //0:�븻, 1:����.

        // Loader Rear
//        bool isZPickPos = MT_CmprPos(miLDR_ZElevF , GetMotrPos(miLDR_ZElevF , piLDR_ZElevFPick   )) ;
//        bool isSafeIdx  = MT_GetCmdPos(miIDX_XIndxR) <= GetMotrPos(miIDX_XIndxR , piIDX_XIndxRGetWait) ;
//        bool isCyclePick = DM.ARAY[riLDR_F].GetCntStat(csUnkwn) && !isZPickPos && isSafeIdx && !DM.ARAY[riLDR_F].GetCntStat(csMask);
//        bool isCycleEnd  = DM.ARAY[riLDR_F].GetCntStat(csUnkwn) == 0 && DM.ARAY[riLDR_F].GetCntStat(csMask) == 0 ;

        bool isZPickPos  = MT_CmprPos(miULD_ZElevt , GetMotrPos(miULD_ZElevt , piULD_ZElevtPick   )) && MT_GetStopInpos(miULD_ZElevt) ;
        bool isZPlcePos  = MT_CmprPos(miULD_ZElevt , GetMotrPos(miULD_ZElevt , piULD_ZElevFPlce   )) && MT_GetStopInpos(miULD_ZElevt) ;

        bool isCyclePick = DM.ARAY[riULD].GetCntStat(csEmpty) && !isZPickPos && !IO_GetX(xRTL_OutDetect) ;
        bool isCyclePlce = DM.ARAY[riULD].CheckAllStat(csWorkEnd) && !DM.ARAY[riULD].GetCntStat(csEmpty) && !isZPlcePos && !IO_GetX(xRTL_OutDetect) ;
        bool isCycleEnd  = !isCyclePick && !isCyclePlce ;

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCyclePick  ) {Trace(m_sPartName.c_str(),"CyclePick   Stt"); Step.iSeq = scPick   ; InitCycleStep(); m_iCycleSttTime=GetTime();}
        else if (isCyclePlce  ) {Trace(m_sPartName.c_str(),"CyclePlce   Stt"); Step.iSeq = scPlce   ; InitCycleStep(); m_iCycleSttTime=GetTime();}
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default     End");                                                   Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                          return false ;
        case scPick  : if(CyclePick  ()){ Trace(m_sPartName.c_str(),"CyclePick   End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scPlce  : if(CyclePlce  ()){ Trace(m_sPartName.c_str(),"CyclePlce   End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}
//---------------------------------------------------------------------------
bool CUnLoader::CycleHome()     //sun DLL direct access.
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

        case 10: if(IO_GetX(xULD_Detect7)) { 
                      EM_SetErrMsg(eiPRT_Crash , (m_sPartName + "���� ����� ���� ���� ���·� ���� ���� ���¸� Ȯ���ϼ���").c_str());
                      Step.iHome = 0 ;
                      return true ;
                 }
                 MT_DoHome(miULD_ZElevt);
                 Step.iHome++;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miULD_ZElevt)) return false ;
                 MT_GoAbsMan(miULD_ZElevt , PM.GetValue(miULD_ZElevt , pvULD_ZElevtWait ));
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GoAbsMan(miULD_ZElevt , PM.GetValue(miULD_ZElevt , pvULD_ZElevtWait ))) return false ;
                 Step.iHome = 0;
                 return true ;
    }
}
//---------------------------------------------------------------------------
bool CUnLoader::CyclePick(void)
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

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case  10: if(IO_GetX(xULD_Detect7)) {
                      EM_SetErrMsg(eiPRT_Crash , (m_sPartName + "���� ����� ���� ���� ���·� ���� ���� ���¸� Ȯ���ϼ���").c_str());
                      Step.iHome = 0 ;
                      return true ;
                  }

                  MoveMotr(miULD_ZElevt , piULD_ZElevtPick);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miULD_ZElevt , piULD_ZElevtPick)) return false ;
                  Step.iCycle=0;
                  return true;
    }
}
//---------------------------------------------------------------------------
bool CUnLoader::CyclePlce(void)
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

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case  10: if(IO_GetX(xULD_Detect7)) {
                      EM_SetErrMsg(eiPRT_Crash , (m_sPartName + "���� ����� ���� ���� ���·� ���� ���� ���¸� Ȯ���ϼ���").c_str());
                      Step.iHome = 0 ;
                      return true ;
                  }

                  MoveMotr(miULD_ZElevt , piULD_ZElevFPlce);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miULD_ZElevt , piULD_ZElevFPlce)) return false ;
                  Step.iCycle=0;
                  return true;
    }
}
//---------------------------------------------------------------------------
bool CUnLoader::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iMotr == miULD_ZElevt){
        if(IO_GetX(xRTL_OutDetect)) {
            sMsg = m_sPartName + "���� ����� ���� ���� ���·� ���� ���� ���¸� Ȯ���ϼ���." ;
            bRet = false;
        }

//        if(MT_GetCmdPos(miLTL_XGenRr) < PM.GetValue(miLTL_XGenRr , pvLTL_XWait)) {
//            sMsg = m_sPartName + "���� �ε����� ��ġ�� Wait��ġ ���� �����ϴ�.";
//            bRet = false;
//        }
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
bool CUnLoader::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
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
    //    sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
    //    bRet = false ;
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

bool CUnLoader::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow )
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

void CUnLoader::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // ���͸� �����϶� ���� �Լ�.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CUnLoader::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CUnLoader::SetLastCmd()
{
    m_dLastIdxPos = MT_GetCmdPos( miULD_ZElevt);
    return ; //����Ʈ�� �ʿ� ����.
}

bool CUnLoader::CheckMoved()
{
    if(m_dLastIdxPos != MT_GetCmdPos(miULD_ZElevt)) return true  ; //����Ʈ�� �ʿ� ����.
    else                                            return false ;
}

bool CUnLoader::CheckStop()
{
    if(!MT_GetStop(miULD_ZElevt)) return false ;

    return true ;
}

void CUnLoader::Load(bool _bLoad)
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




