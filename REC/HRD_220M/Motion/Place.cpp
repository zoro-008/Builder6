//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Place.h"
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
CPlace PLC ;

CPlace::CPlace(void)
{

}

void CPlace::Init()
{
    m_sPartName = "Place " ;
    Reset();
    Load(true);

    InitCycleName();
}

void CPlace::Close()
{
    Load(false);
}

void CPlace::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CPlace::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}

CPlace::~CPlace (void)
{

}

bool CPlace::FindChip(int &r , int &c )
{
//    r = DM.ARAY[riLD1].FindFrstRow(csUnkwn) ;
    return false;
}

double CPlace::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
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
bool CPlace::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
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
bool  CPlace::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
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

bool CPlace::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CPlace::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPlace::CycleHome()     //sun DLL direct access.
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

bool CPlace::Autorun(void) //오토런닝시에 계속 타는 함수.
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

        bool isCyclePlace         =  DM.ARAY[riDSP].CheckAllStat(csWork) && !DM.ARAY[riPIK].CheckAllStat(csNone) && IO_GetX(xETC_PickVacuum) && SEQ.Pt_Step.bPlace ;
        bool isCycleEnd           =  true;


        if(EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCyclePlace      ) {Trace(m_sPartName.c_str(),"CyclePlace     Stt"); Step.iSeq = scPlace      ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd        ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default          :                         Trace(m_sPartName.c_str(),"default          End");Step.iSeq = scIdle ;  return false ;
        case scIdle      :                                                                                                 return false ;
        case scPlace     : if(CyclePlace      ()){ Trace(m_sPartName.c_str(),"CyclePlace       End");Step.iSeq = scIdle ;} return false ;

    }

  return false ;

}

bool CPlace::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{

    if (Step.iSeq) return false;

    Stat.bReqStop = true ;
    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CPlace::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CPlace::ToStart(void) //스타트를 하기 위한 함수.
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

bool CPlace::ToStop(void) //스탑을 하기 위한 함수.
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

// 싸이클 지그 플레이스
bool CPlace::CyclePlace()
{
    //Check Cycle Time Out. // m_tmCycle.OnDelay(bool, int)
    AnsiString sTemp ;  //현재 싸이클 && (현재 싸이클 == 프리스텝 싸이클) &&   true     && 마스터 옵션에서 디버그 모드가 아닐때랑
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 180000 )) { // hhh
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

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MT_GoAbsSlow(miSTG_ZPick , PM.GetValue(miSTG_ZPick , pvSTG_ZPickPlace) + OM.DevOptn.dJigPCLOfs);
                  Step.iCycle++;
                  return false;

        case  11: if(!MT_GoAbsSlow(miSTG_ZPick , PM.GetValue(miSTG_ZPick , pvSTG_ZPickPlace) + OM.DevOptn.dJigPCLOfs)) return false;
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  12: if(!m_tmDelay.OnDelay(true, OM.DevOptn.iPlaceDelay * 1000)) return false ;
                  IO_SetY(yETC_Vacuum     , false);
                  IO_SetY(yETC_Eject      , true );
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  13: if( IO_GetY(yETC_Vacuum     )) return false ;
                  //if( IO_GetY(yETC_VCMAirOnOff)) return false ;
                  if(!IO_GetY(yETC_Eject      )) return false ;
                  if(!m_tmDelay.OnDelay(true , OM.DevOptn.iEjectdelay)) return false ;
                  DM.ARAY[riPIK].SetStat(csNone);
                  MoveMotr(miSTG_ZPick, piSTG_ZPickWait);
                  IO_SetY(yETC_Eject, false);
                  Step.iCycle++;
                  return false;

        case  14: if(IO_GetY(yETC_Eject)) return false ;
                  if(!MoveMotr(miSTG_ZPick, piSTG_ZPickWait)) return false ;
                  MoveActr(aiSTG_PshFwBw, ccBwd);
                  SPC.LOT.AddWorkCnt();
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiSTG_PshFwBw, ccBwd)) return false ;
                  DM.ARAY[riDSP].SetStat(csNone);
                  SEQ.Pt_Step.bIdle      = false;
                  SEQ.Pt_Step.bJigPick   = false;
                  SEQ.Pt_Step.bDispenser = false;
                  SEQ.Pt_Step.bPlace     = false;
                  SEQ.bSEQEnd = false;
                  SEQ.End_Time = Now();
                  Step.iCycle = 0;
                  return true;

    }
}

void CPlace::SetLastCmd() //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
{
    return ; //이파트는 필요 없다.

}

bool CPlace::CheckMoved() //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
{
    
    return true ; //이파트는 필요 없다.
}

bool CPlace::CheckStop() // 장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.
{

    if(!AT_Done(aiDSP_IdxUpDn)) return false ;
    if(!AT_Done(aiSTG_PshFwBw)) return false ;

    if(!MT_GetStop(miDSP_XDsps)) return false ;
    if(!MT_GetStop(miSTG_ZPick)) return false ;

    return true ; // 모든 장비가 정지가 되었으면 return true
}

void CPlace::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\Place.INI";

    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_sLotNo" , m_sLotNo );
    }

    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_sLotNo" , m_sLotNo );
    }


}
//---------------------------------------------------------------------------
