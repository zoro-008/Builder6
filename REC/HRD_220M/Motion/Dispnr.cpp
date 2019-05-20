//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Dispnr.h"
//---------------------------------------------------------------------------
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "LotUnit.h"
#include "SPCUnit.h"
#include "Rs232Man.h"
#include "Sequence.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CDispnr DIP ;

CDispnr::CDispnr(void)
{

}

void CDispnr::Init()
{
    m_sPartName = "Dispnr " ;
    Reset();

    Load(true);

    InitCycleName();
}

void CDispnr::Close()
{
    Load(false);
}

void CDispnr::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CDispnr::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}

CDispnr::~CDispnr (void)
{

}

bool CDispnr::FindChip(int &r , int &c )
{
//    r = DM.ARAY[riLD1].FindFrstRow(csUnkwn) ;
    return false;
}

double CDispnr::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos     = 0.0 ;

    if(_iMotr == miDSP_XDsps){
        switch(_iPstnId) {
            default                 : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piDSP_XIndxWait    : dPos = PM.GetValue (_iMotr , pvDSP_XIndxWait    ); break ;
            case piDSP_XIndxWorkStt : dPos = PM.GetValue (_iMotr , pvDSP_XIndxWorkStt ); break ;
            case piDSP_XIndxEnd     : dPos = PM.GetValue (_iMotr , pvDSP_XIndxEnd     ); break ;
        }
    }
    else if(_iMotr == miSTG_ZPick){
        switch(_iPstnId) {
            default                 : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piSTG_ZPickWait    : dPos = PM.GetValue (_iMotr , pvSTG_ZPickWait    ); break ;
            case piSTG_ZPickJIG     : dPos = PM.GetValue (_iMotr , pvSTG_ZPickJIG     ); break ;
            case piSTG_ZPickWork    : dPos = PM.GetValue (_iMotr , pvSTG_ZPickWork    ); break ;
            case piSTG_ZPickPlace   : dPos = PM.GetValue (_iMotr , pvSTG_ZPickPlace   ); break ;
        }
    }



    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CDispnr::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isPshFw = AT_Complete(ySTG_PickFw, ccFwd);
    bool isPshBw = AT_Complete(ySTG_PickBw, ccBwd);

    if(_iMotr == miDSP_XDsps){
        switch(_iPstnId) {
            default                 :                                                                                break ;
            case piDSP_XIndxWait    :                                                                                break ;
            case piDSP_XIndxWorkStt :                                                                                break ;
            case piDSP_XIndxEnd     :                                                                                break ;
        }
    }
    else if(_iMotr == miSTG_ZPick){
        switch(_iPstnId) {
            default                 :                                                                                break ;
            case piSTG_ZPickWait    :                                                                                break ;
            case piSTG_ZPickJIG     : if(isPshFw) {sMsg = "Now is Pusher Cylinder Fw"; bRet = false;}                break ;
            case piSTG_ZPickWork    :                                                                                break ;
            case piSTG_ZPickPlace   :                                                                                break ;
        }
    }
    else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool  CDispnr::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    bool bRet = true ;
    AnsiString sMsg ;

    bool isZStop = MT_GetStopInpos(miSTG_ZPick);

    if(aiDSP_IdxUpDn){
        //if(bLDR_YClmpMoving) {sMsg = AnsiString(MT_GetName(miLDR_YClmp)) + "is Moving!"; bRet = false ;}
        //if(bLDR_ZClmpMoving) {sMsg = AnsiString(AT_GetName(miLDR_ZClmp)) + "is Moving!"; bRet = false ;}
    }
    else if(aiSTG_PshFwBw){
         if(!_bFwd) {
             if(!isZStop) { sMsg = AnsiString(MT_GetName(miSTG_ZPick)) + " is Moveing!" ; bRet = false ; }
         }          
    }
    else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

bool CDispnr::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CDispnr::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CDispnr::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_HomeTo , m_sPartName.c_str());//EM_SetErr(eiPRB_HomeTo);
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
                 //Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: MT_SetHomeEnd(miDSP_XDsps, false);
                 MT_SetHomeEnd(miSTG_ZPick, false);

                 MT_DoHome(miDSP_XDsps);


                 Step.iHome++;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miDSP_XDsps)) return false ;
                 MT_DoHome(miSTG_ZPick);
                 Step.iHome++;
                 return false;

        case 12: if(!MT_GetHomeEnd(miSTG_ZPick)) return false ;

                 MT_GoAbsMan(miDSP_XDsps, PM.GetValue(miDSP_XDsps , pvDSP_XIndxWait));
                 MT_GoAbsMan(miSTG_ZPick, PM.GetValue(miSTG_ZPick , pvSTG_ZPickWait));


                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miDSP_XDsps, PM.GetValue(miDSP_XDsps , pvDSP_XIndxWait))) return false ;
                 if(!MT_GoAbsMan(miSTG_ZPick, PM.GetValue(miSTG_ZPick , pvSTG_ZPickWait))) return false ;

                 AT_MoveCyl(aiDSP_IdxUpDn, ccFwd);
                 AT_MoveCyl(aiSTG_PshFwBw, ccBwd);

                 Step.iHome++ ;
                 return false ;

        case 14: if(!AT_MoveCyl(aiDSP_IdxUpDn, ccFwd)) return false ;
                 if(!AT_MoveCyl(aiSTG_PshFwBw, ccBwd)) return false ;

                 Step.iHome++;
                 return true ;

    }
}

bool CDispnr::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

//프리버퍼 오리검사 레이져조사 비젼검사
//Unkwn -> Mask     Work       Good
//                             Fail

    if (Step.iSeq == 0) {
        if(Stat.bReqStop)return false ;

        bool isCycleDispenser     =  DM.ARAY[riDSP].CheckAllStat(csUnkwn) && !DM.ARAY[riPIK].CheckAllStat(csNone) && SEQ.Pt_Step.bDispenser;
        bool isCycleEnd           =  true;

        if(!DM.ARAY[riDSP].CheckAllStat(csWork) && !DM.ARAY[riPIK].CheckAllStat(csNone) && !IO_GetX(xETC_PickVacuum)) EM_SetErr(eiPKG_Dispr);

        if(EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCycleDispenser  ) {Trace(m_sPartName.c_str(),"CycleDispenser Stt"); Step.iSeq = scDispenser  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd        ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default          :                         Trace(m_sPartName.c_str(),"default          End");Step.iSeq = scIdle ;  return false ;
        case scIdle      :                                                                                                 return false ;
        case scDispenser : if(CycleDispenser  ()){ Trace(m_sPartName.c_str(),"CycleDispenser   End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;

}

bool CDispnr::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{

    if (Step.iSeq) return false;

    Stat.bReqStop = true ;
    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CDispnr::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CDispnr::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() , 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str()); //EM_SetErr(eiPRB_ToStartTO);

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

        case 10: Step.iToStart++;
                 return false ;

        case 11: IO_SetY(yETC_VCMAirOnOff, true);
                 Step.iToStart++;
                 return false ;

        case 12: if(!IO_GetY(yETC_VCMAirOnOff)) return false;
                 Step.iToStart = 0;
                 return true ;
    }
}

bool CDispnr::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() , 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str());//EM_SetErr(eiPRB_ToStopTO);

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

        case 10:
                 Step.iToStop++ ;
                 return false ;

        case 11:
                 Step.iToStop = 0;
                 return true ;

    }
}

// 싸이클 디스펜서
bool CDispnr::CycleDispenser()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 180000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiPRB_CycleTO);
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

    //double dDSP_Center = (PM.GetValue(miDSP_XDsps , pvDSP_XIndxWorkStt) + PM.GetValue(miDSP_XDsps , pvDSP_XIndxEnd) / 2 ) ;
    double dDSP_Center    = (PM.GetValue(miDSP_XDsps , pvDSP_XIndxEnd) - PM.GetValue(miDSP_XDsps , pvDSP_XIndxWorkStt)) / 2  ;
    double dDSP_Work      = ((dDSP_Center / (OM.DevOptn.iDisprIncPosCnt)) * iDisprIncCnt) ;
    double dDSP_CenterPos = (PM.GetValue(miDSP_XDsps , pvDSP_XIndxWorkStt) + dDSP_Center) ;



    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiDSP_IdxUpDn , ccBwd);
                  iDisprIncCnt  = 0;
                  iDisprShotCnt = 0;
                  iDisprRptCnt  = 0;
                  Step.iCycle++;
                  return false;

        case 11 : if(!MoveActr(aiDSP_IdxUpDn , ccBwd))return false;
                  MT_GoAbsRun(miDSP_XDsps , dDSP_CenterPos);
                  //MoveMotr(miDSP_XDsps, ddspiDSP_XIndxWorkStt); // hhh

                  Step.iCycle++;
                  return false;

        case 12 : if(!MT_GoAbsRun(miDSP_XDsps , dDSP_CenterPos)) return false ;
                  Step.iCycle++;
                  return false;

        //여기서 부터 다시 돌림.
        case 13 : ////// Dispenser On
                  if(iDisprShotCnt <  OM.DevOptn.iDisprShotCnt && !OM.CmnOptn.bDisprSkip) RSM.Shot();
                  if(iDisprShotCnt == OM.DevOptn.iDisprShotCnt) RSM.ShotEnd();
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case 14 : //if(!OM.CmnOptn.bDisprSkip && RSM.CheckShot()) return false ;  쓰레기값 날라옴..
                  if(!m_tmDelay.OnDelay(true, OM.DevOptn.iDisprdelay)) return false;
                  Step.iCycle++;
                  return false ;

        case 15:  MT_GoAbsAcDc(miDSP_XDsps , dDSP_CenterPos - dDSP_Work, OM.DevOptn.iDisprSpd, 1000, 1000);
                  Step.iCycle++;
                  return false;

        case 16 : if(!MT_GoAbsAcDc(miDSP_XDsps , dDSP_CenterPos - dDSP_Work, OM.DevOptn.iDisprSpd, 1000, 1000) ) return false ;
                  MT_GoAbsAcDc(miDSP_XDsps , dDSP_CenterPos + dDSP_Work, OM.DevOptn.iDisprSpd, 1000, 1000);
                  Step.iCycle++;
                  return false;

        case 17 : if(!MT_GoAbsAcDc(miDSP_XDsps , dDSP_CenterPos + dDSP_Work, OM.DevOptn.iDisprSpd, 1000, 1000)) return false;
                  iDisprIncCnt++;
                  if(iDisprIncCnt < OM.DevOptn.iDisprIncPosCnt) {
                      iDisprShotCnt++;
                      Step.iCycle = 13 ;
                      return false;
                  }
                  RSM.ShotEnd();
                  iDisprShotCnt = 0;
                  iDisprIncCnt  = 0;
                  Step.iCycle++;
                  return false;

        case 18 : MT_GoAbsAcDc(miDSP_XDsps , PM.GetValue(miDSP_XDsps, pvDSP_XIndxWorkStt), OM.DevOptn.iDisprSpd,  1000, 1000);
                  Step.iCycle++;
                  return false;

        case 19 : if(!MT_GoAbsAcDc(miDSP_XDsps , PM.GetValue(miDSP_XDsps, pvDSP_XIndxWorkStt) , OM.DevOptn.iDisprSpd, 1000, 1000)) return false;
                  MT_GoAbsAcDc(miDSP_XDsps , PM.GetValue(miDSP_XDsps, pvDSP_XIndxEnd) , OM.DevOptn.iDisprSpd, 1000, 1000);
                  Step.iCycle++;
                  return false;

        case 20 : if(!MT_GoAbsAcDc(miDSP_XDsps , PM.GetValue(miDSP_XDsps, pvDSP_XIndxEnd) , OM.DevOptn.iDisprSpd, 1000, 1000)) return false;
                  iDisprRptCnt++;
                  if(iDisprRptCnt <= OM.DevOptn.iDisprRptCnt)
                  {
                      Step.iCycle = 18 ;
                      return false;
                  }
                  iDisprRptCnt = 0;
                  Step.iCycle++;
                  return false;

        case 21 : MoveActr(aiDSP_IdxUpDn, ccFwd);
                  Step.iCycle++;
                  return false;

        case 22 : if(!MoveActr(aiDSP_IdxUpDn, ccFwd)) return false ;
                  MoveMotr(miDSP_XDsps, piDSP_XIndxWait);
                  Step.iCycle++;
                  return false;

        case 23 : if(!MoveMotr(miDSP_XDsps , piDSP_XIndxWait)) return false ;
                  MoveMotr(miSTG_ZPick, piSTG_ZPickPlace);
                  Step.iCycle++;
                  return false;

        case 24 : if(!MoveMotr(miSTG_ZPick, piSTG_ZPickPlace)) return false ;
                  DM.ARAY[riDSP].SetStat(csWork);
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case 25 : if(!m_tmDelay.OnDelay(true , OM.DevOptn.iEndDelay * 1000))return false;
                  Step.iCycle = 0 ;
                  return true;

    }
}


void CDispnr::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CDispnr::CheckMoved()
{
    
    return true ; //이파트는 필요 없다.
}

bool CDispnr::CheckStop()
{

    if(!AT_Done(aiDSP_IdxUpDn)) return false ;
    if(!AT_Done(aiSTG_PshFwBw)) return false ;

    if(!MT_GetStop(miDSP_XDsps)) return false ;
    if(!MT_GetStop(miSTG_ZPick)) return false ;

    return true ;
}

void CDispnr::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\Dispnr.INI";

    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_sLotNo " , m_sLotNo  );

    }

    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_sLotNo " , m_sLotNo  );


    }


}
//---------------------------------------------------------------------------
