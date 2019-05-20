//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PreBuff.h"
#include "Loader.h"

#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UnLoader430t.h"
#include "Sequence.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone�����.
//---------------------------------------------------------------------------
CPreBuff PRB;
//bool bDragWorkEnd ;      //�Ƴ� �� ���.....mornig ;;;

CPreBuff::CPreBuff(void)
{
}

void CPreBuff::Init()
{
    m_sPartName = "PreBuff " ;
    Reset();
    Load(true);
}

void CPreBuff::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CPreBuff::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CPreBuff::~CPreBuff (void)
{

}

void CPreBuff::Close()
{
    Load(false);
}

bool CPreBuff::FindChip(int &r , int &c )
{
//    r = DM.ARAY[riLD1].FindFrstRow(csUnkwn) ;
    return false;
}

double CPreBuff::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos     = 0.0 ;
    double dStepPos = 0.0 ;

    dStepPos = PM.GetValue(miPRB_XCmp,pvPRB_XGripPs) +
               OM.DevInfo.iColCnt * OM.DevInfo.dColPitch / 2.0 +
               DM.ARAY[riPRB].GetStep() * OM.DevInfo.dColPitch ;


    if(_iMotr == miPRB_XCmp){
        switch(_iPstnId) {
            default             : dPos = MT_GetCmdPos(_iMotr                   ); break ;
            case piPRB_XWait    : dPos = PM.GetValue (_iMotr , pvPRB_XWaitPs   ); break ;
            case piPRB_XGrip    : dPos = PM.GetValue (_iMotr , pvPRB_XGripPs   ); break ;
            case piPRB_XReGrip  : dPos = PM.GetValue (_iMotr , pvPRB_XReGripPs ); break ;
            case piPRB_XPull    : dPos = PM.GetValue (_iMotr , pvPRB_XPullPs   ); break ;
            case piPRB_XBack    : dPos = PM.GetValue (_iMotr , pvPRB_XBackPs   ); break ;
            case piPRB_XHitWait : dPos = PM.GetValue (_iMotr , pvPRB_XHitWait  ); break ;
            case piPRB_XOut1    : dPos = PM.GetValue (_iMotr , pvPRB_XOut1Ps   ); break ;
            case piPRB_XOut2    : dPos = PM.GetValue (_iMotr , pvPRB_XOut2Ps   ); break ;
            case piPRB_XStep1   : dPos = dStepPos;                                break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CPreBuff::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isClampClose = AT_Complete(aiPRB_Cmp  , ccFwd ) ;
    //bool isIdClampDn  = AT_Complete(aiPRB_IdChk, ccBwd ) ;

    if(_iMotr == miPRB_XCmp){
        switch(_iPstnId) {
            default             :                                                                               break ;
            case piPRB_XWait    : if(isClampClose) { sMsg = AT_GetName(aiPRB_Cmp  )+" is Fwd" ; bRet = false ;} break ;
            case piPRB_XGrip    :                                                                               break ;
            case piPRB_XReGrip  :                                                                               break ;
            case piPRB_XPull    :                                                                               break ;
            case piPRB_XBack    :                                                                               break ;
            case piPRB_XHitWait :                                                                               break ;
            case piPRB_XOut1    :                                                                               break ;
            case piPRB_XOut2    :                                                                               break ;
        }
    }




            /*
            default             :                                                                               break ;
            case piPRB_XWait    : if(isClampClose) { sMsg = AT_GetName(aiPRB_Cmp  )+" is Fwd" ; bRet = false ;} break ;
                                  //if(isIdClampDn ) { sMsg = AT_GetName(aiPRB_IdChk)+" is Bwd" ; bRet = false ;} break ;
            case piPRB_XGrip    : if(isIdClampDn ) { sMsg = AT_GetName(aiPRB_IdChk)+" is Bwd" ; bRet = false ;} break ;
            case piPRB_XReGrip  : if(isIdClampDn ) { sMsg = AT_GetName(aiPRB_IdChk)+" is Bwd" ; bRet = false ;} break ;
            case piPRB_XPull    : if(isIdClampDn ) { sMsg = AT_GetName(aiPRB_IdChk)+" is Bwd" ; bRet = false ;} break ;
            case piPRB_XBack    : if(isIdClampDn ) { sMsg = AT_GetName(aiPRB_IdChk)+" is Bwd" ; bRet = false ;} break ;
            case piPRB_XIdCheck : if(isIdClampDn ) { sMsg = AT_GetName(aiPRB_IdChk)+" is Bwd" ; bRet = false ;} break ;
            case piPRB_XOut1    : if(isIdClampDn ) { sMsg = AT_GetName(aiPRB_IdChk)+" is Bwd" ; bRet = false ;} break ;
            case piPRB_XOut2    : if(isIdClampDn ) { sMsg = AT_GetName(aiPRB_IdChk)+" is Bwd" ; bRet = false ;} break ;

        }
    }
    */

    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }

    return true ;
}

//---------------------------------------------------------------------------
bool  CPreBuff::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    bool isXMoving = !MT_GetStopInpos(miPRB_XCmp);

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iActr == aiPRB_Cmp) {
        if( !_bFwd) {
            if(isXMoving) { sMsg = MT_GetName(miPRB_XCmp)+" is Moving" ; bRet = false ;}
        }
    }
    else if(_iActr == aiPRB_HtBlk) {
        //if( !_bFwd) {
        //    if(isXMoving) { sMsg = MT_GetName(miPRB_XCmp)+" is Moving" ; bRet = false ;}
        //}
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}

bool CPreBuff::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // ���͸� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CPreBuff::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPreBuff::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPRB_HomeTo);
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

        case 10: IO_SetY(yPRB_FeedingAC ,false) ;
                 IO_SetY(yPRB_AirBlower ,false);
                 AT_MoveCyl(aiPRB_Cmp  ,ccBwd);
                 //AT_MoveCyl(aiPRB_IdChk,ccFwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPRB_Cmp  ,ccBwd)) return false ;
                 //if(!AT_MoveCyl(aiPRB_IdChk,ccFwd)) return false ;
                 MT_DoHome(miPRB_XCmp);
                 MT_SetHomeEnd(miPRB_XCmp , false);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miPRB_XCmp)) return false;
                 //MT_GoAbsMan(miPRB_XCmp, GetMotrPos(miPRB_XCmp,piPRB_XWait));
                 MT_GoAbsMan(miPRB_XCmp, m_dLastIndPos);
                 Step.iHome++;
                 return false ;

        case 13: //if(!MT_GoAbsMan(miPRB_XCmp, GetMotrPos(miPRB_XCmp,piPRB_XWait))) return false;
                 if(!MT_GoAbsMan(miPRB_XCmp, m_dLastIndPos)) return false ;

                 Step.iHome = 0;
                 return true ;

    }
}

bool CPreBuff::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    //static CDelayTimer tmWait ; ��ŸƮ�Ҷ� ���� ���ð� �ʱ�ȭ������ؼ� ��������� �Ҵ�. ����
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    if ( IO_GetX(xPRB_Overload )                                ) EM_SetErr(eiPRB_Overload );
    if ( IO_GetX(xPRB_StripOver) && !OM.CmnOptn.bIgnrStrOverAlm ) EM_SetErr(eiPRB_StripOver);

//    if (!DM.ARAY[riPR2].GetCntExist())tmWait.Clear() ;
    if (DM.ARAY[riPRB].GetSubStep() != 1 )m_tmWait.Clear() ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop && (!IO_GetX(xPRB_1Pkg) /*|| !IO_GetX(xPRB_2Pkg)*/))return false ;

        bool isCycleIn       =  DM.ARAY[riPRB].CheckAllStat(csNone) &&
                              ((IO_GetX(xPRB_1Pkg) /*&& IO_GetX(xPRB_2Pkg)*/) || ( OM.CmnOptn.bDryRun && IO_GetX(xLDR_PusherFw) ));

        //�巹�� ���� ����ϸ鼭 �ٲ�����.
        //ITW �۾� �ϴµ� ���簡 ������ �����ϋ��� ��� �����ϵ���...
        //Option �߰� ����...
        bool isCycleDrag     =  DM.ARAY[riPRB].GetCntExist (      ) &&  DM.ARAY[riPRB].GetSubStep() == 0 && (DM.ARAY[riWRK].CheckAllStat(csNone) || DM.ARAY[riWRK].GetSubStep() == 1) &&
                                DM.ARAY[riPRB].GetStep() > DM.ARAY[riPRB].GetMaxCol()/2  ;


        //����....�ҽ�. JS 20150408
        //bool isCycleDragStep =  DM.ARAY[riPRB].GetCntExist (      ) &&  DM.ARAY[riPRB].GetSubStep() == 0 && (DM.ARAY[riWRK].CheckAllStat(csNone) || DM.ARAY[riWRK].GetSubStep() == 1) &&
        //                       (DM.ARAY[riWRK].GetMaxCol()-DM.ARAY[riWRK].FindLastCol(csUnkwn)-1) > DM.ARAY[riPRB].GetStep();
        //WorkZone�� ���ܰ� ���� �ؼ� ���ư��� �Ѵ�.
        bool isCycleDragStep;
        if(!OM.MstOptn.bUsedSmallStrip){
            isCycleDragStep =  DM.ARAY[riPRB].GetCntExist (      ) &&  DM.ARAY[riPRB].GetSubStep() == 0 && (DM.ARAY[riWRK].CheckAllStat(csNone) || DM.ARAY[riWRK].GetSubStep() == 1) &&
                              (DM.ARAY[riWRK].GetMaxCol()-DM.ARAY[riWRK].FindLastCol(csUnkwn)-1) > DM.ARAY[riPRB].GetStep();
        }
        else {
            isCycleDragStep =  DM.ARAY[riPRB].GetCntExist (      ) &&  DM.ARAY[riPRB].GetSubStep() == 0 && (DM.ARAY[riWRK].CheckAllStat(csNone) || DM.ARAY[riWRK].GetSubStep() == 1) ;
        }


        bool isCycleOut      =  DM.ARAY[riPRB].GetCntExist (      ) && DM.ARAY[riPRB].GetSubStep() == 1 &&
                                DM.ARAY[riWRK].CheckAllStat(csNone) &&
                                m_tmWait.OnDelay(true ,OM.DevOptn.iWaitDelay) ;// || (DM.ARAY[riWRK].GetCntExist())) ;//&& !DM.ARAY[riWRK].GetCntColStat(2,csUnkwn)))  ;
        bool isCycleEnd      =  DM.ARAY[riPRB].CheckAllStat(csNone) ;

        //Unknown Strip
//        if ( IO_GetX(xPRB_3Pkg) && !OM.CmnOptn.bDryRun && DM.ARAY[riPR1].CheckAllStat(csNone) && DM.ARAY[riPR2].CheckAllStat(csNone) )
//            EM_SetErr(eiPRB_Unknown) ;
        if ( IO_GetX(xPRB_3Pkg) && !OM.CmnOptn.bDryRun && DM.ARAY[riPRB].CheckAllStat(csNone ) ) EM_SetErr(eiPRB_Unknown) ;

        //Dissapear Strip
        //if (isCycleOut && !OM.CmnOptn.bDryRun) {
        //    if ((!DM.ARAY[riPRB].CheckAllStat(csNone) && DM.ARAY[riWRK].GetSubStep() == 1) && !IO_GetX(xPRB_3Pkg) ) EM_SetErr(eiPRB_Dispr)   ;
        //}

        if(!isCycleIn && EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCycleOut     ) {Trace(m_sPartName.c_str(),"CycleOut      Stt"); Step.iSeq = scOut      ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleDrag    ) {Trace(m_sPartName.c_str(),"CycleDrag     Stt"); Step.iSeq = scDrag     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleDragStep) {Trace(m_sPartName.c_str(),"CycleDragStep Stt"); Step.iSeq = scDragStep ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleIn      ) {Trace(m_sPartName.c_str(),"CycleIn       Stt"); Step.iSeq = scIn       ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd     ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default         :                      Trace(m_sPartName.c_str(),"default       End");Step.iSeq = scIdle ;  return false ;
        case scIdle     :                                                                                           return false ;
        case scIn       : if(CycleIn      ()){ Trace(m_sPartName.c_str(),"CycleIn       End");Step.iSeq = scIdle ;} return false ;
        case scDrag     : if(CycleDrag    ()){ Trace(m_sPartName.c_str(),"CycleDrag     End");Step.iSeq = scIdle ;} return false ;
        case scDragStep : if(CycleDragStep()){ Trace(m_sPartName.c_str(),"CycleDragStep End");Step.iSeq = scIdle ;} return false ;
        case scOut      : if(CycleOut     ()){ Trace(m_sPartName.c_str(),"CycleOut      End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;

}

bool CPreBuff::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (LDR.GetSeqStep() == CLoader::scWork) return false ; //�δ� ��ŷ �ϸ鼭 Ǫ�� �ϰ� ������ ��ž�� �Ǿ� ��Ʈ�� �ɷ� �ִ� �� ����.
    if (Step.iSeq) return false;

    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CPreBuff::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CPreBuff::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() , 5000)) EM_SetErr(eiPRB_ToStartTO);

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

        case 10: if(DM.ARAY[riPRB].CheckAllStat(csUnkwn)){
                     m_tmWait.Clear();
                 }
                 IO_SetY(yETC_BlowerAC,false);
                 if(DM.ARAY[riPRB].CheckAllStat(csNone))  MoveActr(aiPRB_Cmp , ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(DM.ARAY[riPRB].CheckAllStat(csNone) && !MoveActr(aiPRB_Cmp , ccBwd)) return false ;

                 if(DM.ARAY[riPRB].CheckAllStat(csNone))  MoveMotr(miPRB_XCmp , piPRB_XWait);
                 Step.iToStart++;
                 return false ;

        case 12: if(DM.ARAY[riPRB].CheckAllStat(csNone) && !MoveMotr(miPRB_XCmp , piPRB_XWait)) return false ;

                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CPreBuff::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() , 10000)) EM_SetErr(eiPRB_ToStopTO);

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

        case 10: IO_SetY(yPRB_FeedingAC,false);

                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MT_GetAlarm   (miPRB_XCmp) &&
                    !MT_GetNLimSnsr(miPRB_XCmp) &&
                    !MT_GetPLimSnsr(miPRB_XCmp) ) m_dLastIndPos = MT_GetCmdPos(miPRB_XCmp) ;
                 else                             m_dLastIndPos = GetMotrPos  (miPRB_XCmp , piPRB_XWait) ;

                 Step.iToStop = 0   ;
                 return true ;
    }
}

//One Cycle.
bool CPreBuff::CycleIn(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPRB_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        IO_SetY(yPRB_AirBlower,false);
        IO_SetY(yPRB_FeedingAC,false);
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

        case  10: IO_SetY(yPRB_AirBlower,true);
                  MoveActr(aiPRB_Cmp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPRB_Cmp , ccBwd)) return false ;
                  IO_SetY(yPRB_FeedingAC,true);
                  MoveMotr(miPRB_XCmp , piPRB_XWait);
                  Step.iCycle++;
                  return false;

        case  12: if(/*IO_GetX(xPRB_2Pkg) ||*/ IO_GetX(xPRB_1Pkg) ) return false;//|| ( !OM.CmnOptn.bDryRun && !IO_GetX(xLDR_PusherBw)) ) return false;
                  if(!MoveMotr(miPRB_XCmp , piPRB_XWait)) return false ;
                  IO_SetY(yPRB_FeedingAC,false);
                  DM.ARAY[riPRB].SetStat(csUnkwn) ;
                  DM.ARAY[riPRB].SetSubStep(0) ;
                  //if(OM.EqpOptn.bExistUnLoader && !DM.ARAY[riULD].GetCntExist()) ULD.Reset();
                  IO_SetY(yPRB_AirBlower,false);
                  Step.iCycle++;
                  return false;
                  //Step.iCycle = 0 ;
                  //return true ;


        case  13: if(PM.GetValue(miPRB_XCmp , pvPRB_XReGripPs) != PM.GetValue(miPRB_XCmp , pvPRB_XGripPs)){
                      MoveActr(aiPRB_Cmp , ccFwd);
                      Step.iCycle++;
                      return false;
                  }
                  else {
                      Step.iCycle++;
                      return true;
                  }

        case  14: if(!MoveActr(aiPRB_Cmp , ccFwd)) return false;
                  MoveMotr(miPRB_XCmp , piPRB_XReGrip);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miPRB_XCmp , piPRB_XReGrip)) return false;
                  MoveActr(aiPRB_Cmp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveActr(aiPRB_Cmp , ccBwd)) return false;
                  MoveMotr(miPRB_XCmp , piPRB_XGrip);
                  Step.iCycle++;
                  return false;

        case  17: if(!MoveMotr(miPRB_XCmp , piPRB_XGrip)) return false;
                  Step.iCycle++;
                  return false;
    }
}

bool CPreBuff::CycleDrag()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPRB_CycleTO);
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

        case  10: MoveActr(aiPRB_Cmp,ccFwd  );
                  //MoveActr(aiPRB_IdChk,ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPRB_Cmp,ccFwd  )) return false ;
                  //if(!MoveActr(aiPRB_IdChk,ccFwd)) return false ;
                  MoveMotr(miPRB_XCmp,piPRB_XHitWait);
                  Step.iCycle= 15 ;
                  return false ;

/*        case  10: MoveActr(aiPRB_Cmp,ccBwd  );    ���� ���� ���� ��...�� �ҽ� ���� ����;;;
                  MoveActr(aiPRB_IdChk,ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPRB_Cmp,ccBwd  )) return false ;
                  if(!MoveActr(aiPRB_IdChk,ccFwd)) return false ;
                  Step.iCycle= 15 ;
                  return false ;
*/
        /*  �̾� �����ǵ�������..
                  MoveMotr(miPRB_XCmp,piPRB_XGrip);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPRB_XCmp,piPRB_XGrip))return false ;
                  MoveActr(aiPRB_Cmp,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiPRB_Cmp,ccFwd))return false ;
                  MoveMotr(miPRB_XCmp , piPRB_XPull) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miPRB_XCmp , piPRB_XPull)) return false ;
                  MoveActr(aiPRB_Cmp,ccBwd);
                  Step.iCycle++;
                  return false ;
        */

        case  15: //if(!MoveActr(aiPRB_Cmp,ccBwd)) return false ;
                  if(!MoveMotr(miPRB_XCmp,piPRB_XHitWait)) return false ;
                  MoveActr(aiPRB_Cmp,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveActr(aiPRB_Cmp,ccBwd)) return false ;
                  MoveMotr(miPRB_XCmp , piPRB_XBack) ;
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miPRB_XCmp , piPRB_XBack)) return false ;
                  MoveActr(aiPRB_Cmp,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  18: if(!MoveActr(aiPRB_Cmp,ccFwd)) return false ;
                  //MT_GoAbs(miPRB_XCmp,GetMotrPos(miPRB_XCmp,piPRB_XIdCheck),OM.DevOptn.dPreRailOutVel);
                  Step.iCycle++;
                  return false ;

        case  19: //if(!MT_GoAbs(miPRB_XCmp,GetMotrPos(miPRB_XCmp,piPRB_XIdCheck),OM.DevOptn.dPreRailOutVel)) return false ;
                  //DM.ShiftArrayData(riPR1,riPR2);
                  DM.ARAY[riPRB].SetSubStep(1);
                  //if(!OM.CmnOptn.bIgnrIdCehck) MoveActr(aiPRB_IdChk,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  20: //if(!OM.CmnOptn.bIgnrIdCehck && !MoveActr(aiPRB_IdChk,ccBwd)) return false ;
                  //if(IO_GetX(xPRB_IdCheck)) {
                  //    //MoveActr(aiPRB_IdChk,ccFwd);
                  //    EM_SetErr(eiPRB_IdCheck);
                  //    Step.iCycle = 0;
                  //    return true;
                  //}

                  //MoveActr(aiPRB_IdChk,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  21: //if(!MoveActr(aiPRB_IdChk,ccFwd)) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  22: //autorun ���� �� if(!m_tmTemp.OnDelay(true , OM.DevOptn.iWaitDelay)) return false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CPreBuff::CycleDragStep()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPRB_CycleTO);
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

    double dPos ;

    bool bFollowWRK = !DM.ARAY[riWRK].CheckAllStat(csNone) ;

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

        case  10: //dPos = GetMotrPos(miPRB_XCmp,piPRB_XGrip) +
                  //       OM.DevInfo.iColCnt * OM.DevInfo.dColPitch / 2.0 +
                  //       DM.ARAY[riPRB].GetStep() * OM.DevInfo.dColPitch ;

                  //MoveActr(aiPRB_Cmp,ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: //if(!MoveActr(aiPRB_Cmp,ccFwd  )) return false ;
                  //MoveMotr(miPRB_XCmp,piPRB_XStep1);
                  Step.iCycle++;
                  return false;

        case  12: //if(!MoveMotr(miPRB_XCmp,piPRB_XStep1))return false ;
                  DM.ARAY[riPRB].SetStep(OM.DevInfo.iColCnt) ;
                  //
                  //if(!OM.MstOptn.bPRBCmp) { //�������� ���� ������ �����ε� �ɸ��� ���.
                  //    Step.iCycle = 0 ;
                  //    return true ;
                  //}
                  //
                  //MoveActr(aiPRB_Cmp , ccBwd);
                  Step.iCycle ++;
                  return false ;

        case  13: //if(!MoveActr(aiPRB_Cmp , ccBwd)) return false;
                  //MoveActr(aiPRB_Cmp , ccFwd);
                  Step.iCycle ++;
                  return false ;

        case  14: //if(!MoveActr(aiPRB_Cmp , ccFwd)) return false;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CPreBuff::CycleOut()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPRB_CycleTO);
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

        case  10: MoveActr(aiPRB_Cmp,ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPRB_Cmp,ccFwd)) return false ;
                  MoveMotr(miPRB_XCmp , piPRB_XOut1);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miPRB_XCmp , piPRB_XOut1)) return false ;
                  MT_GoAbs(miPRB_XCmp,GetMotrPos(miPRB_XCmp,piPRB_XOut2),OM.DevOptn.dPreRailOutVel);
                  Step.iCycle++;
                  return false ;
//����ݵ�ü ���� ������ ��� ����..JS 2013. 5. 22.
/*
        case  13: if(!MT_GoAbs(miPRB_XCmp,GetMotrPos(miPRB_XCmp,piPRB_XOut2),OM.DevOptn.dPreRailOutVel)) return false ;
                  m_sWorkedLotNo = DM.ARAY[riPRB].GetLotNo() ;
                  DM.ShiftArrayData(riPRB,riWRK);
                  MoveMotr(miPRB_XCmp,piPRB_XBack);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miPRB_XCmp,piPRB_XBack))return false ;
                  MoveActr(aiPRB_Cmp,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiPRB_Cmp,ccBwd)) return false ;
                  MoveMotr(miPRB_XCmp , piPRB_XGrip) ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miPRB_XCmp , piPRB_XGrip)) return false ;
                  //DM.ARAY[riPR1].SetStat(csNone);
                  DM.ARAY[riPRB].SetStat(csNone);
                  Step.iCycle = 0 ;
                  return true ;
*/

        case  13: if(!MT_GoAbs(miPRB_XCmp,GetMotrPos(miPRB_XCmp,piPRB_XOut2),OM.DevOptn.dPreRailOutVel)) return false ;
                  m_sWorkedLotNo = DM.ARAY[riPRB].GetLotNo() ;
                  DM.ShiftArrayData(riPRB,riWRK);
                  MoveActr(aiPRB_Cmp,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveActr(aiPRB_Cmp,ccBwd)) return false ;
                  MoveMotr(miPRB_XCmp,piPRB_XBack);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveMotr(miPRB_XCmp,piPRB_XBack))return false ;
                  MoveMotr(miPRB_XCmp , piPRB_XGrip) ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miPRB_XCmp , piPRB_XGrip)) return false ;
                  //DM.ARAY[riPR1].SetStat(csNone); ��
                  DM.ARAY[riPRB].SetStat(csNone);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

void CPreBuff::SetLastCmd()
{
    return ; //����Ʈ�� �ʿ� ����.

}

bool CPreBuff::CheckMoved()
{
    return true ; //����Ʈ�� �ʿ� ����.
}

bool CPreBuff::CheckStop()
{
    if(!MT_GetStop(miPRB_XCmp)) return false ;
    if(!AT_Done(aiPRB_Cmp    )) return false ;
    //if(!AT_Done(aiPRB_IdChk  )) return false ;

    return true ;
}

void CPreBuff::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\PreBuff.INI";

    if ( IsLoad ) UserINI.Load(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
    }

}
//---------------------------------------------------------------------------