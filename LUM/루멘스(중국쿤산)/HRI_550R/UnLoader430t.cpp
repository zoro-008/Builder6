//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UnLoader430t.h"
#include "PostBuff.h"
#include "Loader.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "EqpComUnit.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CUnLoader ULD;

CUnLoader::CUnLoader(void)
{
    m_sPartName = "UnLoader " ;
    Reset();
}

void CUnLoader::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CUnLoader::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CUnLoader::~CUnLoader (void)
{
//    Trace("","");
}

bool CUnLoader::FindChip(int &r , int &c )
{
    r = DM.ARAY[riULD].FindFrstRow(csEmpty) ;
    return (r > -1 && r > -1);
}

double CUnLoader::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int  r  , c  ;
    bool bOddRow ;

    FindChip(r , c);

    //X , Y Pos Setting.
    double dZ1Pos = PM.GetValue(miULD_ZCmp , pvULD_ZWorkStartPs) - r * OM.DevInfo.dCsSlPitch ;
    double dPos  = 0.0 ;

    if(_iMotr == miULD_ZCmp){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                         ); break ;
            case piULD_ZWait         : dPos = PM.GetValue (_iMotr , pvULD_ZWaitPs         ); break ;
            case piULD_ZPickFwd      : dPos = PM.GetValue (_iMotr , pvULD_ZPickFwdPs      ); break ;
            case piULD_ZClampOn      : dPos = PM.GetValue (_iMotr , pvULD_ZClampOnPs      ); break ;
            case piULD_ZPickBwd      : dPos = PM.GetValue (_iMotr , pvULD_ZPickBwdPs      ); break ;
            case piULD_ZWorkStart    : dPos = PM.GetValue (_iMotr , pvULD_ZWorkStartPs    ); break ;
            case piULD_ZPlaceFwd     : dPos = PM.GetValue (_iMotr , pvULD_ZPlaceFwdPs     ); break ;
            case piULD_ZClampOff     : dPos = PM.GetValue (_iMotr , pvULD_ZClampOffPs     ); break ;
            case piULD_ZPlaceBwd     : dPos = PM.GetValue (_iMotr , pvULD_ZPlaceBwdPs     ); break ;
            case piULD_ZWork         : dPos = dZ1Pos                                       ; break ;
        }
    }

    else if(_iMotr == miULD_YCmp){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos  (_iMotr                       ); break ;
            case piULD_YWait         : dPos = PM.GetValue   (_iMotr , pvULD_YWaitPs       ); break ;
            case piULD_YPick         : dPos = PM.GetValue   (_iMotr , pvULD_YPickPs       ); break ;
            case piULD_YWork         : dPos = PM.GetValue   (_iMotr , pvULD_YWorkPs       ); break ;
            case piULD_YPlace        : dPos = PM.GetValue   (_iMotr , pvULD_YPlacePs      ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CUnLoader::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isBwPusher        = AT_Complete(aiPSB_Psh , ccBwd) ;
    bool isRailGateCnsr    = IO_GetX(xPSB_PkgOut2) ; //IO_GetX(xPSB_PkgOut1) || IO_GetX(xPSB_PkgOut2) ;
    bool isClampYFwd       = MT_CmprPos(miULD_YCmp , GetMotrPos(miULD_YCmp , piULD_YPick)) || MT_CmprPos(miULD_YCmp , GetMotrPos(miULD_YCmp , piULD_YPlace    ));
    bool isClampZDnstair   = MT_CmprPos(miULD_ZCmp , piULD_ZPickBwd)  || MT_CmprPos(miULD_ZCmp , piULD_ZPickFwd  ) || MT_CmprPos(miULD_ZCmp , piULD_ZClampOn   );
    bool isClampZCenter    = MT_CmprPos(miULD_ZCmp , piULD_ZWait   )  || MT_CmprPos(miULD_ZCmp , piULD_ZWork     ) || MT_CmprPos(miULD_ZCmp , piULD_ZWorkStart );
    bool isClampZUpstair   = MT_CmprPos(miULD_ZCmp , piULD_ZPlaceBwd) || MT_CmprPos(miULD_ZCmp , piULD_ZPlaceFwd ) || MT_CmprPos(miULD_ZCmp , piULD_ZClampOff  );

    if(_iMotr == miULD_ZCmp){
             if(!isBwPusher    ) { sMsg = "Pusher Ac Motor is not Bwd" ; bRet = false ;}
        else if(isRailGateCnsr) { sMsg = "Strip is feeding now      " ; bRet = false ;}
            switch(_iPstnId) {
                default               :                                                                                                                   break ;
                //UpStair
                case piULD_ZPickBwd   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = MT_GetName(miULD_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piULD_ZClampOn   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = MT_GetName(miULD_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piULD_ZPickFwd   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = MT_GetName(miULD_YCmp)+" is Fwd" ; bRet = false ;} break ;
                //Middle
                case piULD_ZWait      : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = MT_GetName(miULD_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piULD_ZWorkStart : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = MT_GetName(miULD_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piULD_ZWork      : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = MT_GetName(miULD_YCmp)+" is Fwd" ; bRet = false ;} break ;
                //DnStatir
                case piULD_ZPlaceFwd  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = MT_GetName(miULD_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piULD_ZClampOff  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = MT_GetName(miULD_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piULD_ZPlaceBwd  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = MT_GetName(miULD_YCmp)+" is Fwd" ; bRet = false ;} break ;

        }
    }

    else if(_iMotr == miULD_YCmp){
        if(!isBwPusher) return false ;
        else {
            switch(_iPstnId) {
                default               :                                                                                           break ;
                case piULD_YWait      :                                                                                           break ;
                case piULD_YWork      :                                                                                           break ;
                case piULD_YPick      : if(isClampZCenter) {sMsg = MT_GetName(miULD_ZCmp)+" is Middle Position" ; bRet = false ;} break ;
                case piULD_YPlace     : if(isClampZCenter) {sMsg = MT_GetName(miULD_ZCmp)+" is Middle Position" ; bRet = false ;} break ;
            }
        }
    }
    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool  CUnLoader::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    if(_iActr == aiULD_Cmp) {
    }
    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}

bool CUnLoader::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CUnLoader::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CUnLoader::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiULD_HomeTO);
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
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: //MT_Reset(miULD_ZCmp);
                 //MT_Reset(miULD_YCmp);

                 //MT_SetServo(miULD_ZCmp,true);
                 //MT_SetServo(miULD_YCmp,true);
                 //MoveActr(aiULD_Cmp,ccBwd);
                 PSB.MoveActr(aiPSB_Psh,ccBwd);
                 IO_SetY(yULD_MgzInAC , false);
                 IO_SetY(yULD_MgzOutAC, false);
                 Step.iHome++;
                 return false ;

        case 11: //if(!MoveActr(aiULD_Cmp,ccBwd)) return false ;
                 if(!PSB.MoveActr(aiPSB_Psh,ccBwd)) return false ;
                 MT_SetHomeEnd(miULD_ZCmp , true) ;
                 MT_ClearPos(miULD_ZCmp);
                 MT_GoIncMan(miULD_ZCmp,-10); //12mm
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetStop(miULD_ZCmp))return false;
                 MT_DoHome(miULD_YCmp);
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miULD_YCmp))return false ;
                 MT_DoHome(miULD_ZCmp);
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GetHomeEnd(miULD_ZCmp))return false ;
                 MT_GoAbsMan(miULD_YCmp, GetMotrPos(miULD_YCmp,piULD_YWait));
                 MT_GoAbsMan(miULD_ZCmp, GetMotrPos(miULD_ZCmp,piULD_ZWait));
                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GoAbsMan(miULD_YCmp, GetMotrPos(miULD_YCmp,piULD_YWait)))return false ;
                 if(!MT_GoAbsMan(miULD_ZCmp, GetMotrPos(miULD_ZCmp,piULD_ZWait)))return false ;
                 Step.iHome = 0;
                 return true ;

    }
}

bool CUnLoader::Autorun(void) //오토런닝시에 계속 타는 함수.
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

        bool bUseHRT430S, bUseHRI580I;

        if(OM.EqpOptn.sModelName == "HRT-430S") bUseHRT430S = true;
        else if(OM.EqpOptn.sModelName == "HRI-580T") bUseHRI580I = true;

        bool isExstStrip   = !DM.ARAY[riPR1].CheckAllStat(csNone) || !DM.ARAY[riPR2].CheckAllStat(csNone) ||
                             !DM.ARAY[riWRP].CheckAllStat(csNone) || !DM.ARAY[riWRK].CheckAllStat(csNone) ||
                             !DM.ARAY[riPS1].CheckAllStat(csNone) || !DM.ARAY[riPS2].CheckAllStat(csNone) ||
                             !DM.ARAY[riLD1].CheckAllStat(csNone) ;

        bool isLotChanged = (DM.ARAY[riULD].GetLotNo()!="" && DM.ARAY[riULD].GetLotNo() != DM.ARAY[riPS2].GetLotNo() && !DM.ARAY[riPS2].CheckAllStat(csNone)) ||
                            (DM.ARAY[riULD].GetLotNo()!="" && DM.ARAY[riULD].GetLotNo() != DM.ARAY[riPS1].GetLotNo() && !DM.ARAY[riPS1].CheckAllStat(csNone) && DM.ARAY[riPS2].CheckAllStat(csNone));

                             //(DM.ARAY[riULD].GetLotNo() != DM.ARAY[riPS1].GetLotNo() && !DM.ARAY[riPS1].CheckAllStat(csNone)) ;
                             //DM.ARAY[riULD].GetCntStat  (csWork) &&
                             //( !DM.ARAY[riPS2].CheckAllStat(csNone) && (DM.ARAY[riPS2].GetLotNo() != DM.ARAY[riULD].GetLotNo()) ) ;
                             //


        bool isZWorkPos    = MT_CmprPos(miULD_ZCmp ,GetMotrPos(miULD_ZCmp , piULD_ZWork)) ;

        bool isCycleSupply = DM.ARAY[riULD].CheckAllStat(csNone ) && !IO_GetX(xULD_MgzIn) && !Stat.bChckSplyEmpty ;
        bool isCyclePick   = DM.ARAY[riULD].CheckAllStat(csNone ) && (IO_GetX(xULD_MgzIn) ||  OM.CmnOptn.bDryRun ) &&
                             (isExstStrip);// ||!isPreEqpEnd) ;
        bool isCycleWork   = DM.ARAY[riULD].GetCntStat  (csEmpty) && !isZWorkPos ;//&& PSB.GetSeqStep() != scOut ;

        //bool isCyclePlace  = !DM.ARAY[riULD].CheckAllStat(csNone ) && !DM.ARAY[riULD].CheckAllStat(csEmpty) &&
        //                     (DM.ARAY[riULD].CheckAllStat(csWork ) || isLotFinished || (!isExstStrip && LD1.GetStat().bWorkEnd));
        bool isCyclePlace  = !DM.ARAY[riULD].CheckAllStat(csNone ) &&
                             (DM.ARAY[riULD].CheckAllStat(csWork ) || isLotChanged || (!isExstStrip && LD1.GetStat().bWorkEnd));



        bool isCycleEnd    = DM.ARAY[riULD].CheckAllStat(csNone ) && !isExstStrip && LD1.GetStat().bWorkEnd;//  isPreEqpEnd  ;

        Stat.bReadyMgz = isZWorkPos ;

        //모르는 스트립에러.
        if(  DM.ARAY[riULD].CheckAllStat(csNone) && (  IO_GetX(xULD_MgzDetect1 ) ||  IO_GetX(xULD_MgzDetect2 ))                       ) EM_SetErr(eiULD_MgzUnknown) ;
        //카세트 사라짐.
        if( !DM.ARAY[riULD].CheckAllStat(csNone) && ( !IO_GetX(xULD_MgzDetect1 ) && !IO_GetX(xULD_MgzDetect2 ))&& !OM.CmnOptn.bDryRun ) EM_SetErr(eiULD_MgzDispr  ) ; //sun bDryRun 옵션 추가.
        //카세트 OUT 매거진 풀

        if(  DM.ARAY[riULD].CheckAllStat(csNone) && !IO_GetX(xULD_MgzIn) && Stat.bChckSplyEmpty && DM.ARAY[riPS2].GetCntExist() && !OM.CmnOptn.bDryRun ) EM_SetErr(eiULD_NeedMgz  ) ;


        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleSupply) {Trace(m_sPartName.c_str(),"CycleSupply Stt"); Step.iSeq = scSupply ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePick  ) {Trace(m_sPartName.c_str(),"CyclePick   Stt"); Step.iSeq = scPick   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork  ) {Trace(m_sPartName.c_str(),"CycleWork   Stt"); Step.iSeq = scWork   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePlace ) {Trace(m_sPartName.c_str(),"CyclePlace  Stt"); Step.iSeq = scPlace  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default    :                      Trace(m_sPartName.c_str(),"default     End");Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                       return false ;
        case scSupply: if(CycleSupply()){ Trace(m_sPartName.c_str(),"CycleSupply End");Step.iSeq = scIdle ;} return false ;
        case scPick  : if(CyclePick  ()){ Trace(m_sPartName.c_str(),"CyclePick   End");Step.iSeq = scIdle ;} return false ;
        case scWork  : if(CycleWork  ()){ Trace(m_sPartName.c_str(),"CycleWork   End");Step.iSeq = scIdle ;} return false ;
        case scPlace : if(CyclePlace ()){ Trace(m_sPartName.c_str(),"CyclePlace  End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}

bool CUnLoader::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10 ;

    //Ok.
    return true;

}

bool CUnLoader::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CUnLoader::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr(eiULD_ToStartTO);

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

        case 10: PSB.MoveActr(aiPSB_Psh,ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!PSB.MoveActr(aiPSB_Psh,ccBwd)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CUnLoader::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 5000)) EM_SetErr(eiULD_ToStopTO);

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

        case 10: IO_SetY(yULD_MgzInAC ,false);
                 IO_SetY(yULD_MgzOutAC,false);
                 PSB.MoveActr(aiPSB_Psh,ccBwd);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!PSB.MoveActr(aiPSB_Psh,ccBwd)) return false ;
                 Step.iToStop = 0   ;
                 return true ;
    }
}

//One Cycle.
bool CUnLoader::CycleSupply(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 7000 )) {
        EM_SetErr(eiULD_CycleTO);
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

        case  10: IO_SetY(yULD_MgzInAC,true);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  11: r1 = m_tmTemp.OnDelay(true , 6000) ;
                  r2 = IO_GetX(xULD_MgzIn)           ;
                  if(!r1&&!r2) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  12: if(!m_tmTemp.OnDelay(true ,  200)) return false ;
                  IO_SetY(yULD_MgzInAC,false);
                  if(!IO_GetX(xULD_MgzIn))Stat.bChckSplyEmpty = true ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CUnLoader::CyclePick()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiULD_CycleTO);
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
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiULD_Cmp    , ccFwd );
                  MoveMotr(miULD_YCmp   , piULD_YWork) ;
                  IO_SetY(yULD_MgzInAC,true);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiULD_Cmp    , ccFwd  )) return false ;
                  if(!MoveMotr(miULD_YCmp   , piULD_YWork)) return false ;
                  MoveMotr(miULD_ZCmp , piULD_ZPickFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miULD_ZCmp , piULD_ZPickFwd))return false ;
                  MoveMotr(miULD_YCmp , piULD_YPick);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miULD_YCmp , piULD_YPick))return false ;
                  if(!IO_GetX(xULD_MgzIn) && !OM.CmnOptn.bDryRun)return false ;
                  IO_SetY(yULD_MgzInAC,false);
                  MoveMotr(miULD_ZCmp , piULD_ZClampOn) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miULD_ZCmp , piULD_ZClampOn)) return false ;
                  MoveActr(aiULD_Cmp , ccBwd);
                  DM.ARAY[riULD].SetStat(csEmpty) ;
                  DM.ARAY[riULD].SetLotNo("");
                  Stat.bChckSplyEmpty = false ;
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiULD_Cmp , ccBwd)) return false ;
                  //IO_SetY(yULD_MgzInAC,true);
                  MoveMotr(miULD_ZCmp , piULD_ZPickBwd) ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miULD_ZCmp , piULD_ZPickBwd)) return false ;
                  MoveMotr(miULD_YCmp , piULD_YWork) ;
                  IO_SetY(yULD_MgzInAC,true);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miULD_YCmp , piULD_YWork)) return false ;
                  //IO_SetY(yULD_MgzInAC,false);
                  MoveMotr(miULD_ZCmp , piULD_ZWorkStart);
                  Step.iCycle++;
                  return false ;

        case  18: if(!MoveMotr(miULD_ZCmp , piULD_ZWorkStart)) return false ;
                  IO_SetY(yULD_MgzInAC,false);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CUnLoader::CycleWork ()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiULD_CycleTO);
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
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miULD_YCmp , piULD_YWork) ;
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miULD_YCmp , piULD_YWork)) return false ;
                  MoveMotr(miULD_ZCmp , piULD_ZWork);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miULD_ZCmp , piULD_ZWork)) return false ;
                  Step.iCycle = 0;
                  return true ;
    }

}

bool CUnLoader::CyclePlace ()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiULD_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        IO_SetY(yULD_MgzOutAC,false);
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
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miULD_YCmp , piULD_YWork) ;
                  IO_SetY(yULD_MgzOutAC,true);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miULD_YCmp , piULD_YWork)) return false ;
                  MoveMotr(miULD_ZCmp , piULD_ZPlaceFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miULD_ZCmp , piULD_ZPlaceFwd))return false ;

                  IO_SetY(yULD_MgzOutAC,false);

                  MoveMotr(miULD_YCmp , piULD_YPlace) ;
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miULD_YCmp , piULD_YPlace))return false ;
                  MoveMotr(miULD_ZCmp , piULD_ZClampOff) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miULD_ZCmp , piULD_ZClampOff)) return false ;
                  MoveActr(aiULD_Cmp , ccFwd);
                  DM.ARAY[riULD].SetStat(csNone) ;
                  DM.ARAY[riULD].SetLotNo("");
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiULD_Cmp , ccFwd)) return false ;
                  IO_SetY(yULD_MgzOutAC,true);
                  MoveMotr(miULD_ZCmp , piULD_ZPlaceBwd) ;

                  //DM.ARAY[riULD].SetLotNo("");
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miULD_ZCmp , piULD_ZPlaceBwd)) return false ;
                  MoveMotr(miULD_YCmp , piULD_YWork) ;
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miULD_YCmp , piULD_YWork)) return false ;
                  MoveMotr(miULD_ZCmp , piULD_ZPickFwd);
                  Step.iCycle++;
                  return false ;

        case  18: if(MoveMotr(miULD_ZCmp , piULD_ZPickFwd)) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;




                  /////////////////
        case  19: if(m_tmTemp.OnDelay(true , 2000)){
                      if(IO_GetX(xULD_MgzOutFull)) {
                          EM_SetErr(eiULD_MgzFull);
                          Step.iCycle = 0 ;
                          return true ;
                      }
                  }

                  if(IO_GetX(xULD_MgzOutFull)) return false ;

                  IO_SetY(yULD_MgzOutAC,false);


                  Step.iCycle = 0;
                  return true ;
    }
}

void CUnLoader::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CUnLoader::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CUnLoader::CheckStop()
{
    if(!MT_GetStop(miULD_YCmp)) return false ;
    if(!MT_GetStop(miULD_ZCmp)) return false ;

    if(!AT_Done(aiULD_Cmp   )) return false ;

    return true ;
}

void CUnLoader::Load(bool IsLoad , FILE *fp)
{

}
//---------------------------------------------------------------------------
