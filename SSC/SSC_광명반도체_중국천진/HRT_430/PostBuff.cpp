//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PostBuff.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "EqpComUnit.h"
#include "UserIni.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CPostBuff PSB;

CPostBuff::CPostBuff(void)
{
    m_sPartName = "PostBuff " ;
    Reset();
    Load(true);
}

void CPostBuff::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CPostBuff::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CPostBuff::~CPostBuff (void)
{
    Load(false);
}

bool CPostBuff::FindChip(int &r , int &c )
{
//    r = DM.ARAY[riLD1].FindFrstRow(csUnkwn) ;
//    return (r > -1 && r > -1);
    return false;
}

double CPostBuff::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{

    bool bOddRow ;
    double dPos  = 0.0 ;

    if(_iMotr == miPSB_XCmp){
        switch(_iPstnId) {
            default          : dPos = MT_GetCmdPos(_iMotr                  ); break ;
            case piPSB_XWait : dPos = PM.GetValue (_iMotr , pvPSB_XWaitPs  ); break ;
            case piPSB_XGrip : dPos = PM.GetValue (_iMotr , pvPSB_XGripPs  ); break ;
            case piPSB_XPull : dPos = PM.GetValue (_iMotr , pvPSB_XPullPs  ); break ;
            case piPSB_XBack : dPos = PM.GetValue (_iMotr , pvPSB_XBackPs  ); break ;
            case piPSB_XOut  : dPos = PM.GetValue (_iMotr , pvPSB_XOutPs   ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CPostBuff::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iMotr == miPSB_XCmp){
        switch(_iPstnId) {
            default          :                                          break ;
            case piPSB_XWait :                                          break ;
            case piPSB_XGrip :                                          break ;
            case piPSB_XPull :                                          break ;
            case piPSB_XBack :                                          break ;
            case piPSB_XOut  :                                          break ;
        }
    }

    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool  CPostBuff::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool isXMoving = !MT_GetStopInpos(miPSB_XCmp);

    if(_iActr == aiPSB_Cmp) {
        if(!_bFwd) {
            if(isXMoving) { sMsg = MT_GetName(miPRB_XCmp)+" is Moving" ; bRet = false ; }
        }
    }
    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}

bool CPostBuff::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;


    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CPostBuff::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPostBuff::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErr(eiPSB_HomeTo);
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

    //if(Stat.bReqStop) {
    //    Step.iHome = 0;
    //    return true ;
    //}

    switch (Step.iHome) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
//                 Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: AT_MoveCyl(aiPSB_Cmp,ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPSB_Cmp,ccBwd))return false ;
                 MT_DoHome(miPSB_XCmp);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miPSB_XCmp))return false;
                 //MoveMotr(miPSB_XCmp,piPSB_XWait);
                 MT_GoAbsMan(miPSB_XCmp, m_dLastIndPos);
                 Step.iHome++;
                 return false ;

        case 13: //if(!MoveMotr(miPSB_XCmp,piPSB_XWait))return false;
                 if(!MT_GoAbsMan(miPSB_XCmp, m_dLastIndPos))return false;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CPostBuff::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    if ( IO_GetX(xPSB_Overload) ) EM_SetErr(eiPSB_Overload);

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

//        bool PostEqipReady =  IO_GetX(xETC_PostEqpReady) ;
        bool PostEqipReady = true ;

        bool isPstEqpReady = EC.GetPstEqpReady();

        bool isCycleDrag   = !DM.ARAY[riPS1].CheckAllStat(csUnkwn) && !DM.ARAY[riPS1].CheckAllStat(csNone) && !IO_GetX(xPSB_2Pkg);
        bool isCycleOut    = !DM.ARAY[riPS2].CheckAllStat(csUnkwn) && !DM.ARAY[riPS2].CheckAllStat(csNone) && (isPstEqpReady || OM.MstOptn.bNoLdUl) && !IO_GetX(xPSB_2Pkg);
        bool isCycleEnd    =  DM.ARAY[riPS1].CheckAllStat(csNone ) &&  DM.ARAY[riPS2].CheckAllStat(csNone) ;

        //모르는 스트립에러.
        if( DM.ARAY[riPS1].CheckAllStat(csNone) && DM.ARAY[riPS2].CheckAllStat(csNone) ) {
            if(IO_GetX(xPSB_1Pkg)) EM_SetErr(eiPSB_Unknown) ;
        }

        //strip 사라짐.
        if( isCycleOut && DM.ARAY[riPS2].GetCntExist() && !OM.CmnOptn.bDryRun) {
            if(!IO_GetX(xPSB_1Pkg))EM_SetErr(eiPSB_Dispr) ;
        }

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleDrag ) {Trace(m_sPartName.c_str(),"CycleDrag Stt"); Step.iSeq = scDrag; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleOut  ) {Trace(m_sPartName.c_str(),"CycleOut  Stt"); Step.iSeq = scOut ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd  ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default    :                      Trace(m_sPartName.c_str(),"default     End");Step.iSeq = scIdle ;  return false ;
        case scDrag  : if(CycleDrag  ()){ Trace(m_sPartName.c_str(),"CycleDrag   End");Step.iSeq = scIdle ;} return false ;
        case scOut   : if(CycleOut   ()){ Trace(m_sPartName.c_str(),"CycleOut    End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}

bool CPostBuff::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;
}

bool CPostBuff::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CPostBuff::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr(eiPSB_ToStartTO);

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

        case 10: if(DM.ARAY[riPS1].CheckAllStat(csNone)&&
                    DM.ARAY[riPS2].CheckAllStat(csNone))  MoveActr(aiPSB_Cmp , ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(DM.ARAY[riPS1].CheckAllStat(csNone)&&
                    DM.ARAY[riPS2].CheckAllStat(csNone)&& !MoveActr(aiPSB_Cmp , ccBwd)) return false ;

                 if(DM.ARAY[riPS1].CheckAllStat(csNone)&&
                    DM.ARAY[riPS2].CheckAllStat(csNone))  MoveMotr(miPSB_XCmp , piPSB_XWait);

                 Step.iToStart++;
                 return false ;

        case 12: if(DM.ARAY[riPS1].CheckAllStat(csNone)&&
                    DM.ARAY[riPS2].CheckAllStat(csNone)&& !MoveMotr(miPSB_XCmp , piPSB_XWait)) return false ;


                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CPostBuff::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErr(eiPSB_ToStopTO);

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

        case 10: Step.iToStop ++ ;
                 return false ;

        case 11: //m_dLastIndPos = MT_GetCmdPos(miPSB_XCmp) ;
                 if(!MT_GetAlarm   (miPSB_XCmp) &&
                    !MT_GetNLimSnsr(miPSB_XCmp) &&
                    !MT_GetPLimSnsr(miPSB_XCmp) ) m_dLastIndPos = MT_GetCmdPos(miPSB_XCmp) ;
                 else                             m_dLastIndPos = GetMotrPos  (miPSB_XCmp , piPSB_XWait) ;

                 Step.iToStop = 0   ;
                 return true ;
    }
}

//One Cycle.
bool CPostBuff::CycleDrag()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPSB_CycleTO);
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

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPSB_Cmp,ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPSB_Cmp,ccBwd)) return false ;
                  MoveMotr(miPSB_XCmp,piPSB_XGrip);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPSB_XCmp,piPRB_XGrip))return false ;
                  MoveActr(aiPSB_Cmp,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiPSB_Cmp,ccFwd))return false ;
                  MoveMotr(miPSB_XCmp , piPSB_XPull) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miPSB_XCmp , piPSB_XPull)) return false ;
                  DM.ShiftArrayData(riPS1,riPS2);
                  MoveActr(aiPSB_Cmp,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiPSB_Cmp,ccBwd)) return false ;
                  MoveMotr(miPSB_XCmp , piPSB_XBack) ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miPSB_XCmp , piPSB_XBack)) return false ;
                  MoveActr(aiPSB_Cmp,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiPSB_Cmp,ccFwd)) return false ;

                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CPostBuff::CycleOut()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPSB_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        EC.SetPstEqpSending(false) ;
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

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  EC.SetPstEqpSending(false) ;
                  Step.iCycle = 0 ;
                  return true ;

        case  10: EC.SetPstEqpSending(true) ;

                  MoveActr(aiPSB_Cmp,ccFwd);
                  EC.WriteArayData(riPS2);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPSB_Cmp,ccFwd)) return false ;
                  MT_GoAbs(miPSB_XCmp,GetMotrPos(miPSB_XCmp,piPSB_XOut),OM.DevOptn.dPostRailOutVel);
                  DM.ARAY[riPS2].SetStat(csNone);
                  Step.iCycle++;
                  return false ;

        case  12: //if(IO_GetXDn(xPSB_2Pkg))EC.SendPstEqp(psData);
                  if(!MT_GetStop(miPSB_XCmp)) return false ;
                  MoveActr(aiPSB_Cmp,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiPSB_Cmp,ccBwd)) return false ;
                  if( IO_GetX(xPSB_2Pkg)) return false;
                  EC.SetPstEqpSending(false) ;


                  //DM.ARAY[riPS1].SetStat(csNone);

                  MoveMotr(miPSB_XCmp , piPSB_XWait) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miPSB_XCmp , piPSB_XWait) ) return false ;

                  Step.iCycle = 0 ;
                  return true ;
    }
}

void CPostBuff::SetLastCmd()
{
    return ; //이파트는 필요 없다.
}

bool CPostBuff::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CPostBuff::CheckStop()
{
    if(!MT_GetStop(miPSB_XCmp)) return false ;
    if(!AT_Done(aiPSB_Cmp    )) return false ;

    return true ;
}

void CPostBuff::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\PstBuff.INI";

    if ( IsLoad ) UserINI.Load(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
    }
}
//---------------------------------------------------------------------------
