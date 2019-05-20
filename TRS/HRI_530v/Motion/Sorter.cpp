//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Sorter.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "LotUnit.h"
#include "UserIni.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone�����.
//---------------------------------------------------------------------------
CLoader LSP;

CLoader::CLoader(void)
{

}

CLoader::~CLoader (void)
{

}

void CLoader::Init()
{
    m_sPartName = "Loader " ;
    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();


}

void CLoader::Close()
{
    Load(false);
}

void CLoader::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}


bool CLoader::FindChip(int &r , int &c )
{
    //r = DM.ARAY[riLDR].FindLastRow(csUnkwn) ;
    if(DM.ARAY[riLDR].FindFrstColRow(csUnkwn , r,c)){
        return true ;
    }
    else {
        r=0 ;
        c=0 ;
        return false ;
    }

}

double CLoader::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    int    iR,iC ;
    FindChip(iR ,iC);

    double dWorkPos ;
    dWorkPos = PM.GetValue(miLDR_YIndx , pvLDR_YIndxWorkStt) ;
    dWorkPos = dWorkPos - (OM.DevInfo.dRowPitch * iR) ;

    if(_iMotr == miLSP_ZElev){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piLDR_YIndxWait     : dPos = PM.GetValue (_iMotr , pvLDR_YIndxWait      ); break ;
            case piLDR_YIndxGet      : dPos = PM.GetValue (_iMotr , pvLDR_YIndxGet       ); break ;
            case piLDR_YIndxWorkStt  : dPos = PM.GetValue (_iMotr , pvLDR_YIndxWorkStt   ); break ;
            case piLDR_YIndxOut      : dPos = PM.GetValue (_iMotr , pvLDR_YIndxOut       ); break ;
            case piLDR_YIndxWork     : dPos = dWorkPos                                    ; break ;

        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CLoader::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

bool CLoader::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CLoader::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CLoader::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
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

        case 10: if(!DM.ARAY[riLDR].CheckAllStat(csNone)){ //Ư���� �� ������.'
                     Step.iToStart=20 ;
                 }
                 else {
                     Step.iToStart=30 ;
                 }
                 return false ;


        //Work���� ���� ������.
        case 20: if (!CheckMoved()){ //�ȿ���������.
                     Step.iToStart=22 ;
                     return false ;
                 }

                 MoveActr(aiLDR_IndxUpUp , ccBwd);
                 Step.iToStart++;
                 return false ;

        case 21: if(!MoveActr(aiLDR_IndxUpUp , ccBwd)) return false ;
                 MoveMotr(miLDR_YIndx , piLDR_YIndxWork );
                 Step.iToStart++;
                 return false ;

        case 22: if(!MoveMotr(miLDR_YIndx , piLDR_YIndxWork )) return false ;
                 MoveActr(aiLDR_IndxUpUp , ccFwd);
                 Step.iToStart++;
                 return false ;

        case 23: if(!MoveActr(aiLDR_IndxUpUp , ccFwd)) return false ;
                 Step.iToStart=0;
                 return true ;


        //��ũ ���� ���� ����.
        case 30: MoveActr(aiLDR_IndxUpUp , ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 31: if(!MoveActr(aiLDR_IndxUpUp , ccBwd)) return false;
                 MoveMotr(miLDR_YIndx , piLDR_YIndxWait);
                 Step.iToStart++;
                 return false ;

        case 32: if(!MoveMotr(miLDR_YIndx , piLDR_YIndxWait)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CLoader::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
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

        case 10: SetLastCmd();
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CLoader::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    static int iCycleSttTime = 0;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool isCycleIn   = !DM.ARAY[riPRL].CheckAllStat(csNone ) && DM.ARAY[riLDR].CheckAllStat(csNone );
        bool isCycleWork = !DM.ARAY[riLDR].CheckAllStat(csNone ) && DM.ARAY[riLDR].GetCntStat  (csUnkwn);
        bool isCycleOut  = !DM.ARAY[riLDR].CheckAllStat(csNone ) &&!DM.ARAY[riLDR].GetCntStat  (csUnkwn);
        bool isCycleEnd  =  DM.ARAY[riPRL].CheckAllStat(csNone ) && DM.ARAY[riLDR].CheckAllStat(csNone );


        //�𸣴� ��Ʈ������.
        if(  DM.ARAY[riPRL].CheckAllStat(csNone) && IO_GetX(xLDR_Tray3Work)) EM_SetErrMsg(eiPKG_Unknwn , DM.ARAY[riPRL].GetName().c_str());
        //ī��Ʈ �����.
        if( !DM.ARAY[riPRL].CheckAllStat(csNone) &&!IO_GetX(xLDR_Tray3Work)) EM_SetErrMsg(eiPKG_Dispr  , DM.ARAY[riPRL].GetName().c_str());
        //ī��Ʈ OUT �Ű��� Ǯ

        if(EM_IsErr()) return false ;


         //Normal Decide Step.
             if (isCycleIn    ) {Trace(m_sPartName.c_str(),"CycleIn     Stt"); Step.iSeq = scIn     ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCycleWork  ) {Trace(m_sPartName.c_str(),"CycleWork   Stt"); Step.iSeq = scWork   ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCycleOut   ) {Trace(m_sPartName.c_str(),"CycleOut    Stt"); Step.iSeq = scOut    ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default     End");                                                      Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                             return false ;
        case scIn    : if(CycleIn    ()){ Trace(m_sPartName.c_str(),"CycleIn     End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scWork  : if(CycleWork  ()){ Trace(m_sPartName.c_str(),"CycleWork   End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scOut   : if(CycleOut   ()){ Trace(m_sPartName.c_str(),"CycleOut    End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}

bool CLoader::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_HomeTo,m_sPartName.c_str());
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , Step.iHome );
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

        case 10: MoveActr(aiLDR_IndxUpUp, ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!MoveActr(aiLDR_IndxUpUp, ccBwd)) return false ;
                 MT_DoHome(miLDR_YIndx);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miLDR_YIndx)) return false ;
                 MT_GoAbsMan(miLDR_YIndx , PM.GetValue(miLDR_YIndx , pvLDR_YIndxWait ));
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miLDR_YIndx , PM.GetValue(miLDR_YIndx , pvLDR_YIndxWait ))) return false ;
                 SetLastCmd();
                 Step.iHome = 0;
                 return true ;

    }
}




//One Cycle.
bool CLoader::CycleIn(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiLDR_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    bool r1,r2 ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiLDR_IndxUpUp , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiLDR_IndxUpUp , ccBwd)) return false ;
                  MoveMotr(miLDR_YIndx , piLDR_YIndxGet);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miLDR_YIndx , piLDR_YIndxGet)) return false ;
                  MoveActr(aiLDR_IndxUpUp , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiLDR_IndxUpUp , ccFwd)) return false ;
                  MoveMotr(miLDR_YIndx,piLDR_YIndxWorkStt);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miLDR_YIndx,piLDR_YIndxWorkStt)) return false ;
                  DM.ShiftArrayData(riPRL , riLDR);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

//One Cycle.
bool CLoader::CycleWork(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiLDR_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    bool r1,r2 ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiLDR_IndxUpUp , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiLDR_IndxUpUp , ccFwd)) return false ;
                  MoveMotr(miLDR_YIndx , piLDR_YIndxWork);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miLDR_YIndx , piLDR_YIndxWork)) return false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CLoader::CycleOut(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiLDR_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    bool r1,r2 ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiLDR_IndxUpUp , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiLDR_IndxUpUp , ccFwd)) return false ;
                  MoveMotr(miLDR_YIndx , piLDR_YIndxOut);
                  DM.ShiftArrayData(riLDR , riPSL);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

//---------------------------------------------------------------------------
bool CLoader::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool bExistPRL   = DM.ARAY[riPRL].GetCntExist() ;
    bool bExistPSL   = DM.ARAY[riPSL].GetCntExist() ;
    bool bExistLDR   = DM.ARAY[riLDR].GetCntExist() ;
    bool bIndxFwd    = !AT_Complete(aiLDR_IndxUpUp , ccBwd);


    bool bUnderWorkPos = PM.GetValue(miLDR_YIndx , pvLDR_YIndxWorkStt)+10.0 >= MT_GetCmdPos(miLDR_YIndx) ;

    if(_iMotr == miLDR_YIndx){
        if(bIndxFwd) {
            switch(_iPstnId) {
                default                   :                                  { sMsg = "Not " +AnsiString(MT_GetName(_iMotr))+"'s Postion" ; bRet = false ;} break ;
                case piLDR_YIndxWait      : if(bUnderWorkPos && bExistPRL )  { sMsg = AnsiString(DM.ARAY[riPRL].GetName())+" is Exist"    ; bRet = false ;} break ;
                case piLDR_YIndxGet       : if(bUnderWorkPos && bExistPRL )  { sMsg = AnsiString(DM.ARAY[riPRL].GetName())+" is Exist"    ; bRet = false ;} break ;
                case piLDR_YIndxWorkStt   :                                                                                                                 break ;
                case piLDR_YIndxOut       : if(bExistPSL     && bExistLDR )  { sMsg = AnsiString(DM.ARAY[riPSL].GetName())+" is Exist"    ; bRet = false ;} break ;
                case piLDR_YIndxWork      :                                                                                                                 break ;

            }
        }
    }

    else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr),sMsg.c_str());
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CLoader::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    if(_iActr == aiLDR_IndxUpUp){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bLSP_ZTop*/) {sMsg = AnsiString("Tray ���� ������!"); bRet = false ;}
        }
    }
    else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    }


    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr),sMsg.c_str());
    }

    return bRet ;
}

bool CLoader::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // ���͸� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

void CLoader::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // ���͸� �����϶� ���� �Լ�.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CLoader::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CLoader::SetLastCmd()
{
    m_dLastIdxPos = MT_GetCmdPos(miLDR_YIndx);
    return ; //����Ʈ�� �ʿ� ����.

}

bool CLoader::CheckMoved()
{
    if(m_dLastIdxPos != MT_GetCmdPos(miLDR_YIndx)) return true  ; //����Ʈ�� �ʿ� ����.
    else                                           return false ;

}

bool CLoader::CheckStop()
{
    if(!MT_GetStop(miLDR_YIndx)) return false ;

    if(!AT_Done(aiLDR_IndxUpUp)) return false ;

    return true ;
}

void CLoader::Load(bool _bLoad)
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





