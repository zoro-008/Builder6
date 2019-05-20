//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Rail.h"
#include "Head.h"
#include "Loader.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "VisnComUnit.h"
#include "LotUnit.h"
#include "UserIni.h"
#include "Loader.h"
#include "PstBuff.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CRail  RAL;

CRail::CRail(void)
{
    m_sPartName = "Rail " ;
    iPsbiRowCnt = 0 ;
    iVsFailCnt  = 0 ;
    bMrkOneSkip = false ;
    m_lStrpCycleTime = 0 ;
    m_lPreCycleTime  = 0 ;

    Reset();
    Load(true);

    m_bLotEnded = false ;
}

void CRail::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CRail::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CRail::~CRail (void)
{
    //FormMain으로  Load(false);
}

bool CRail::FindChip(int &r , int &c )
{
    return false;
}

double CRail::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos     = 0.0 ;

    if(_iMotr == miWK1_YFlp){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piWK1_YFlpNormal  : dPos = PM.GetValue (_iMotr , pvWK1_YFlpNormal  ); break ;
            case piWK1_YFlpInverse : dPos = PM.GetValue (_iMotr , pvWK1_YFlpInverse ); break ;
        }
    }
    else if(_iMotr == miWK1_XIns){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piWK1_XInsWait    : dPos = PM.GetValue (_iMotr , pvWK1_XInsWait    ); break ;
            case piWK1_XInsStpr    : dPos = PM.GetValue (_iMotr , pvWK1_XInsStpr    ); break ;
            case piWK1_XInsOut     : dPos = PM.GetValue (_iMotr , pvWK1_XInsOut     ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CRail::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isClampClose = AT_Complete(aiWK1_FlpCmp , ccFwd ) ;
    bool isClampOpen  = AT_Complete(aiWK1_FlpCmp , ccBwd ) ;
    bool isWK1Exist   = DM.ARAY[riWR1].GetCntExist();
    bool isFlpDone    = MT_GetStopInpos(miWK1_YFlp) && (MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpNormal)) || MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpInverse))) ;
    bool isInsUp      = AT_Complete(aiWK1_Ins    , ccBwd ) ;
    //bool isInsWait    = MT_CmprPos (miWK1_XIns , PM.GetValue(miWK1_XIns,pvWK1_XInsWait)) ; //
    bool isWRK_YWait   = MT_CmprPos (miWRK_YVsn , PM.GetValue(miWRK_YVsn,pvWRK_YVsnWait)) ;
    bool isPrbStripOutExist = IO_GetX(xPRB_PkgOut);

    if(_iMotr == miWK1_YFlp){
        switch(_iPstnId) {
            default                :                                                   break ;
            case piWK1_YFlpNormal  : if((!isClampClose && isWK1Exist)|| !isInsUp || !isWRK_YWait || isPrbStripOutExist) { sMsg = "Flipper Zone Check" ;
                                                                                                                          MoveActr(aiWK1_FlpCmp, ccFwd); bRet = false ;} break ;
            case piWK1_YFlpInverse : if((!isClampClose && isWK1Exist)|| !isInsUp || !isWRK_YWait || isPrbStripOutExist) { sMsg = "Flipper Zone Check" ;
                                                                                                                          MoveActr(aiWK1_FlpCmp, ccFwd); bRet = false ;} break ;

        }
    }
    else if(_iMotr == miWK1_XIns){
        switch(_iPstnId) {
            default                :                                                   break ;
            case piWK1_XInsWait    : if(!isFlpDone                ) { sMsg = "!isFlpDone                " ; bRet = false ;} break ;
            case piWK1_XInsStpr    : if(!isFlpDone || !isClampOpen) { sMsg = "!isFlpDone || !isClampOpen" ; bRet = false ;} break ;
            case piWK1_XInsOut     : if(!isFlpDone || !isClampOpen) { sMsg = "!isFlpDone || !isClampOpen" ; bRet = false ;} break ;
        }
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }


    return bRet ;
}

//---------------------------------------------------------------------------
bool  CRail::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    //bool b1 = MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpNormal)) ;
    //bool b2 = MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpInverse)) ;

    bool isFlpDone     = MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpNormal)) || MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpInverse)) ;
    //bool isInsWait     = MT_CmprPos (miWK1_XIns , PM.GetValue(miWK1_XIns,pvWK1_XInsWait  )) ;

    if(_iActr == aiWK1_Ins) {
        if(!_bFwd) {
            if(!isFlpDone ) { sMsg = "!isFlpDone || !isInsWait" ; bRet = false ;}
        }
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}

bool CRail::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CRail::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CRail::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() && !OM.MstOptn.bDebugMode , 15000 )) {
        EM_SetErr(eiRAL_HomeTO);
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

        case 10: MT_Reset(miWK1_XIns);
                 MT_Reset(miWK1_YFlp);
                 MT_SetServo(miWK1_XIns,true);
                 MT_SetServo(miWK1_YFlp,true);

                 AT_MoveCyl(aiPRB_Stopper , ccFwd);
                 AT_MoveCyl(aiWK1_FlpCmp  , ccFwd);
                 AT_MoveCyl(aiWK1_Ins     , ccBwd);
                 AT_MoveCyl(aiWK1_Stopper , ccFwd);
                 AT_MoveCyl(aiWK1_Align   , ccBwd);
                 AT_MoveCyl(aiWK2_Align   , ccBwd);
                 AT_MoveCyl(aiWK2_Stopper , ccFwd);
                 AT_MoveCyl(aiWK3_Align   , ccBwd);
                 AT_MoveCyl(aiWK3_Stopper , ccFwd);

                 IO_SetY(yPRB_FeedingAC , false) ;
                 IO_SetY(yWK1_FeedingAC , false) ;
                 IO_SetY(yPSB_FeedingAC , false) ;

                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPRB_Stopper , ccFwd)) return false ;
                 if(!AT_MoveCyl(aiWK1_FlpCmp  , ccFwd)) return false ;
                 if(!AT_MoveCyl(aiWK1_Ins     , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWK1_Stopper , ccFwd)) return false ;
                 if(!AT_MoveCyl(aiWK1_Align   , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWK2_Align   , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWK2_Stopper , ccFwd)) return false ;
                 if(!AT_MoveCyl(aiWK3_Align   , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWK3_Stopper , ccFwd)) return false ;
                 MT_DoHome(miWK1_XIns) ;
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miWK1_XIns))return false ;
                 MT_GoAbsMan(miWK1_XIns , PM.GetValue(miWK1_XIns , pvWK1_XInsWait));
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miWK1_XIns , PM.GetValue(miWK1_XIns , pvWK1_XInsWait))) return false ;
                 MT_DoHome(miWK1_YFlp) ;
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GetHomeEnd(miWK1_YFlp))return false ;
                 MT_GoAbsMan(miWK1_YFlp , PM.GetValue(miWK1_YFlp , pvWK1_YFlpNormal));
                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GoAbsMan(miWK1_YFlp , PM.GetValue(miWK1_YFlp , pvWK1_YFlpNormal))) return false ;
                 AT_MoveCyl(aiWK1_FlpCmp,ccBwd);
                 Step.iHome++;
                 return false ;

        case 16: if(!AT_MoveCyl(aiWK1_FlpCmp,ccBwd)) return false ;

                 Step.iHome = 0;
                 return true ;
    }
}

bool CRail::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //Clear Timer.
    Stat.bReqStop = true ;
//    m_ToStopTimer.Clear();

    //During the auto run, do not stop.
//    EC.SetPreEqpReady(false);
//    if (EC.GetPreEqpSending()) return false ;
    if (Step.iSeq ) return false;


    Step.iToStop = 10 ;

    //Ok.
    return true;

}

bool CRail::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    if(IO_GetX(xPRB_PkgOut)) {
        EM_SetErr(eiPRB_OutStripExist);
        return false ;
    }
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CRail::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiRAL_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart);
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    static bool bWK1_FlpCmpBw = false ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: if(IO_GetX(xPRB_Pkg) && DM.ARAY[riPRB].CheckAllStat(csNone)){
                     DM.ARAY[riPRB].SetStat(csUnkwn);
                     DM.ARAY[riPRB].SetLotNo(m_sWorkedLotNo) ;

                     DM.ARAY[riPRB].SetID   (AnsiString(m_sWorkedId.ToIntDef(0)/1000 * 900 +99))  ;
                     sTemp = DM.ARAY[riPRB].GetLotNo();
                     Trace("PRB Strip Add Lot" , sTemp.c_str());
                     sTemp = DM.ARAY[riPRB].GetID()   ;
                     Trace("PRB Strip Add ID" , sTemp.c_str());
                 }

                 IO_SetY(yPRB_FeedingAC , false) ;
                 IO_SetY(yWK1_FeedingAC , false) ;
                 if(AT_Complete(aiWK1_FlpCmp,ccBwd)) bWK1_FlpCmpBw = true ;

                 MoveMotr(miWRK_XVsn , piWRK_XVsnWait);
                 MoveMotr(miWRK_YVsn , piWRK_YVsnWait);

                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveMotr(miWRK_XVsn , piWRK_XVsnWait)) return false ;
                 if(!MoveMotr(miWRK_YVsn , piWRK_YVsnWait)) return false ;

                 MoveActr(aiPRB_Stopper,ccFwd);
                 MoveActr(aiWK1_Stopper,ccFwd);
                 MoveActr(aiWK2_Stopper,ccFwd);
                 MoveActr(aiWK3_Stopper,ccFwd);

                 MoveActr(aiWK1_Ins , ccBwd);
                 Step.iToStart++;
                 return false ;

        case 12: if(!MoveActr(aiPRB_Stopper,ccFwd)) return false ;
                 if(!MoveActr(aiWK1_Stopper,ccFwd)) return false ;
                 if(!MoveActr(aiWK2_Stopper,ccFwd)) return false ;
                 if(!MoveActr(aiWK3_Stopper,ccFwd)) return false ;
                 if(!MoveActr(aiWK1_Ins , ccBwd)) return false ;
                 MoveMotr(miWK1_XIns , piWK1_XInsWait);
                 MoveActr(aiWK1_Align , ccFwd);
                 IO_SetY(yPRB_FeedingAC , true);

                 Step.iToStart++;
                 return false ;

        case 13: if(!MoveMotr(miWK1_XIns , piWK1_XInsWait)) return false ;
                 if(!MoveActr(aiWK1_Align , ccFwd)) return false ;
                 MoveActr(aiWK1_FlpCmp , ccFwd );

                 Step.iToStart++;
                 return false ;

        case 14: if(!MoveActr(aiWK1_FlpCmp , ccFwd )) return false ;
                 MoveActr(aiWK1_Align , ccBwd);
                 Step.iToStart++;
                 return false ;

        case 15: if(!MoveActr(aiWK1_Align , ccBwd)) return false ;
                 MoveMotr(miWK1_YFlp,piWK1_YFlpNormal);
                 MoveActr(aiWK1_Stopper,ccFwd);
                 Step.iToStart++;
                 return false ;

        case 16: if(!MoveMotr(miWK1_YFlp,piWK1_YFlpNormal)) return false ;
                 if(!MoveActr(aiWK1_Stopper,ccFwd        )) return false ;
                 //자제는 있는데 센서가 감지 안되고 있어서 포지션 정렬.
                 if((DM.ARAY[riWR1].GetCntExist() && !IO_GetX(xWR1_Pkg)) ||
                    (DM.ARAY[riWR1].GetCntExist() &&  bWK1_FlpCmpBw    ) ) {
                     MoveActr(aiWK1_Ins,ccFwd);
                     MoveActr(aiWK1_Align , ccBwd);
                     MoveActr(aiWK1_FlpCmp, ccBwd);
                     Step.iToStart = 20 ;
                     return false ;
                 }
                 Step.iToStart = 0 ;
                 return true ;


        //자제는 있는데 센서가 감지 안되고 있어서 포지션 정렬.
        case 20: if(!MoveActr(aiWK1_Ins,ccFwd   )) return false ;
                 if(!MoveActr(aiWK1_Align,ccBwd )) return false ;
                 if(!MoveActr(aiWK1_FlpCmp,ccBwd)) return false ;
                 MoveMotr(miWK1_XIns , piWK1_XInsStpr);
                 Step.iToStart++;
                 return false ;

        case 21: if(!MoveMotr(miWK1_XIns , piWK1_XInsStpr)) return false ;
                 MoveActr(aiWK1_Align , ccFwd);
                 Step.iToStart++;
                 return false ;

        case 22: if(!MoveActr(aiWK1_Align , ccFwd)) return false ;
                 MoveActr(aiWK1_FlpCmp , ccFwd);
                 Step.iToStart++;
                 return false ;

        case 23: if(!MoveActr(aiWK1_FlpCmp , ccFwd)) return false ;
                 MoveActr(aiWK1_Align,ccBwd);
                 Step.iToStart++;
                 return false ;

        case 24: if(!MoveActr(aiWK1_Align , ccBwd)) return false ;
                 MoveMotr(miWK1_XIns , piWK1_XInsWait);
                 Step.iToStart++;
                 return false ;

        case 25: if(!MoveMotr(miWK1_XIns , piWK1_XInsWait)) return false ;
                 MoveActr(aiWK1_Ins , ccBwd);
                 IO_SetY(yPRB_FeedingAC , false);
                 Step.iToStart++;
                 return false ;

        case 26 : if(!MoveActr(aiWK1_Ins , ccBwd)) return false ;
                 Step.iToStart = 0;
                 return true ;
    }
}

bool CRail::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() && !OM.MstOptn.bDebugMode , 8000)) EM_SetErr(eiRAL_ToStopTO);

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

        case 10: IO_SetY(yPRB_AirBlower , false) ;
                 IO_SetY(yPRB_FeedingAC , false) ;
                 IO_SetY(yWK1_FeedingAC , false) ;
                 MoveActr(aiWK1_Align,ccBwd);
                 MoveActr(aiWK2_Align,ccBwd);
                 MoveActr(aiWK3_Align,ccBwd);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MoveActr(aiWK1_Align,ccBwd)) return false ;
                 if(!MoveActr(aiWK2_Align,ccBwd)) return false ;
                 if(!MoveActr(aiWK3_Align,ccBwd)) return false ;
                 MoveActr(aiWK1_FlpCmp,ccBwd);
                 MoveActr(aiPRB_Stopper,ccBwd);
                 MoveActr(aiWK1_Stopper,ccBwd);
                 MoveActr(aiWK2_Stopper,ccBwd);
                 MoveActr(aiWK3_Stopper,ccBwd);

                 Step.iToStop ++ ;
                 return false ;

        case 12: if(!MoveActr(aiWK1_FlpCmp,ccBwd)) return false ;
                 if(!MoveActr(aiPRB_Stopper,ccBwd)) return false ;
                 if(!MoveActr(aiWK1_Stopper,ccBwd)) return false ;
                 if(!MoveActr(aiWK2_Stopper,ccBwd)) return false ;
                 if(!MoveActr(aiWK3_Stopper,ccBwd)) return false ;

                 Step.iToStop = 0   ;
                 return true ;
    }
}


bool CRail::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    //bool bPRBRet = AutorunPRB();//프리버퍼 인싸이클 분리.

    //Special....
    //if( Step.iCycle != scMvPrb && !DM.ARAY[riPRB].GetCntExist()) {
    //         if(!IO_GetX(xPRB_PkgIn1) &&  IO_GetY(yPRB_FeedingAC)){}
    //    else if( IO_GetX(xPRB_PkgIn1) && !IO_GetY(yPRB_FeedingAC)){IO_SetY(yPRB_FeedingAC , true); }
    //    else if( IO_GetX(xPRB_PkgIn1) &&  IO_GetY(yPRB_FeedingAC)){}
    //    else if(!IO_GetX(xPRB_PkgIn1) &&  IO_GetY(yPRB_FeedingAC)){}
    //    else if( IO_GetX(xPRB_Pkg   ) &&  IO_GetY(yPRB_FeedingAC)){DM.ARAY[riPRB].SetStat(csUnkwn); IO_SetY(yPRB_FeedingAC , false);}
    //    else if( IO_GetX(xPRB_Pkg   ) && !IO_GetY(yPRB_FeedingAC)){}
    //}



    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop) return false ;

        bool isVS1End = DM.ARAY[riWR1].GetCntExist() && (!DM.ARAY[riVS1].GetCntStat(csUnkwn) && !DM.ARAY[riWR1].GetCntStat (csUnkwn));
        bool isVS2End = DM.ARAY[riWR2].GetCntExist() && (!DM.ARAY[riVS2].GetCntStat(csUnkwn) && !DM.ARAY[riWR2].GetCntStat (csUnkwn));
        bool isVS3End = DM.ARAY[riWR3].GetCntExist() && (!DM.ARAY[riVS3].GetCntStat(csUnkwn) && !DM.ARAY[riWR3].GetCntStat (csUnkwn));

        bool isWK1End =!DM.ARAY[riWR1].GetCntExist() || isVS1End ;
        bool isWK2End =!DM.ARAY[riWR2].GetCntExist() || isVS2End ;
        bool isWK3End =!DM.ARAY[riWR3].GetCntExist() || isVS3End ;

        bool isWK2NeedFeed = DM.ARAY[riWR2].GetCntExist() && DM.ARAY[riVS2].CheckAllStat(csWork) ;
        bool isWK3NeedFeed = DM.ARAY[riWR3].GetCntExist() && DM.ARAY[riVS3].CheckAllStat(csWork) ;

        bool isCycleInNeed = DM.ARAY[riLDR].GetCntExist() && DM.ARAY[riPRB].CheckAllStat(csNone) ;


        bool isDoingInsp  =  HED.GetSeqStep() == CHead::scInsp ;

        bool isCycleBind  = //(DM.ARAY[riVS1].GetCntStat(csUnkwn)&& IO_GetX(xWR1_Pkg)) || //&& !AT_Complete(aiWK1_Lift,ccFwd)) ||
                            ( DM.ARAY[riVS2].GetCntStat(csUnkwn)&& IO_GetX(xWR2_Pkg) && DM.ARAY[riVS3].GetCntStat(csUnkwn)&& IO_GetX(xWR3_Pkg)) &&
                            (!AT_Complete(aiWK2_Align , ccFwd) && !AT_Complete(aiWK3_Align , ccFwd));

        bool isCycleUnBind= ( !DM.ARAY[riVS2].GetCntStat(csUnkwn)&& !IO_GetX(xWR2_Pkg) &&//&& !isDoingInsp) ||
                              !DM.ARAY[riVS3].GetCntStat(csUnkwn)&& !IO_GetX(xWR3_Pkg) ) ;//&& !isDoingInsp) ) ;

        bool isExistPSBX  =  IO_GetX(xPSB_Pkg);// || IO_GetX(xPSB_PkgOut3) || IO_GetX(xPSB_PkgOut4) ;

        bool isPSBReady   = AT_Complete(aiPSB_Align  , ccBwd) && AT_Complete(aiPSB_Stopper , ccFwd) && !isExistPSBX;

        //bool isWK1Ready   = false ;//MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpNormalPs)) &&
                            //AT_Complete(aiWK1_Align,ccBwd) && AT_Complete(aiWK1_FlpCmp,ccFwd) && AT_Complete(aiWK1_Ins,ccBwd) && AT_Complete(aiWK1_Stopper,ccFwd) ;

//        m_bWaitLotChange  =  DM.ARAY[riPRB].GetCntExist() && DM.ARAY[riWK1].GetCntExist() && //TODO : 이거 기다리면 안될거 같다.
//                             DM.ARAY[riPRB].GetLotNo   () != DM.ARAY[riWK1].GetLotNo   () ;

        bool isCycleWK1Rdy = !IO_GetX(xWR1_Pkg) && !AT_Complete(aiWK1_FlpCmp,ccFwd) && DM.ARAY[riWR1].GetCntExist();

//        bool isCycleMvAll  =  DM.ARAY[riPRB].GetCntExist() &&  isVS1End &&  isVS2End &&  isVS3End &&
//                              DM.ARAY[riPSB].CheckAllStat(csNone) && isPSBReady;

        //bool isCycleMvAll  =  isVS1End &&  isVS2End &&  isVS3End && isPSBReady;

//        bool isCycleMvR1   =  DM.ARAY[riPRB].GetCntExist() && isWK1End ;
        bool isExistRail   = DM.ARAY[riWR1].GetCntExist() ||
                             DM.ARAY[riWR2].GetCntExist() ||
                             DM.ARAY[riWR3].GetCntExist() ;

        bool isReadyMv1    = isWK1End && isWK2End ;
        bool isReadyMv2    = isWK2End && isWK3End ;
        bool isReadyMv3    = isWK3End && isPSBReady && DM.ARAY[riPSB].CheckAllStat(csNone) ;

        bool isCycleMvAuto = !isDoingInsp && !isCycleInNeed && isExistRail &&
                              isReadyMv1 && isReadyMv2 && isReadyMv3 ;

        bool isCycleMvPrb  =  DM.ARAY[riPRB].GetCntExist() && !DM.ARAY[riWR1].GetCntExist() && !isDoingInsp && !isWK2NeedFeed  && !isWK3NeedFeed ;
                             // ( !DM.ARAY[riWR2].CheckAllStat(csWork) && !DM.ARAY[riWR3].CheckAllStat(csWork) )  ;

//        bool isCycleMvAuto = (isWK1End && isWK2End && isWK3End) && !isDoingInsp &&
//                             (isCycleMvAll || isCycleMvR1 || isCycleMv1 || isCycleMv2 || isCycleMv3 ) ;

        bool isCycleIn    = (IO_GetX(xPRB_PkgIn)|| /*LDR.GetSeqStep() == CLoader::scWork ||*/ OM.CmnOptn.bDryRun) &&
                            !DM.ARAY[riPRB].GetCntExist()  ;//&& !isCycleMvAuto;


        bool isConEnd      = !DM.ARAY[riPRB].GetCntExist() && !DM.ARAY[riWR1].GetCntExist() &&
                             !DM.ARAY[riWR2].GetCntExist() && !DM.ARAY[riWR3].GetCntExist() &&
                             !DM.ARAY[riPSB].GetCntExist() ;

        if(isConEnd) { isCycleUnBind = false;}

        if(isCycleMvAuto) {
            unsigned long lCrntTime = GetTickCount() ;
            m_lStrpCycleTime = lCrntTime - m_lPreCycleTime ;
            m_lPreCycleTime  = lCrntTime ;

        }

        //모르는 스트립에러.
        if(!DM.ARAY[riPRB ].CheckExist(0,0) &&  IO_GetX(xPRB_Pkg) ) EM_SetErr(eiPRB_PkgUnknown ) ;
        if(!DM.ARAY[riWR1 ].CheckExist(0,0) &&  IO_GetX(xWR1_Pkg) ) EM_SetErr(eiWK1_PkgUnknown ) ;
        if(!DM.ARAY[riWR2 ].CheckExist(0,0) &&  IO_GetX(xWR2_Pkg) ) EM_SetErr(eiWK2_PkgUnknown ) ;
        if(!DM.ARAY[riWR3 ].CheckExist(0,0) &&  IO_GetX(xWR3_Pkg) ) EM_SetErr(eiWK3_PkgUnknown ) ;

        //스트립 사라짐 에러.
        if( DM.ARAY[riPRB ].CheckExist(0,0) && (!IO_GetX(xPRB_Pkg) && !IO_GetX(xPRB_PkgIn)) && !OM.CmnOptn.bDryRun ) EM_SetErr(eiPRB_PkgDispr ) ;

        //스테이지 올리면 센서가 감지 않되어 뺀다.
        if(DM.ARAY[riWR1].CheckExist(0,0) && !IO_GetX(xWR1_Pkg) && MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpNormal)) && !isCycleWK1Rdy && !OM.CmnOptn.bDryRun) EM_SetErr(eiWK1_PkgDispr ) ;
        if(DM.ARAY[riWR2].CheckExist(0,0) && !IO_GetX(xWR2_Pkg) && AT_Complete(aiWK2_Align , ccBwd) && !OM.CmnOptn.bDryRun) EM_SetErr(eiWK2_PkgDispr ) ;
        if(DM.ARAY[riWR3].CheckExist(0,0) && !IO_GetX(xWR3_Pkg) && AT_Complete(aiWK3_Align , ccBwd) && !OM.CmnOptn.bDryRun) EM_SetErr(eiWK3_PkgDispr ) ;

        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleIn    ) {Trace(m_sPartName.c_str(),"CycleIn     Stt"); Step.iSeq = scIn     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //스트립을 PreBuff로 이송.
        else if (isCycleWK1Rdy) {Trace(m_sPartName.c_str(),"CycleWK1Rdy Stt"); Step.iSeq = scWK1Rdy ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleMvAuto) {Trace(m_sPartName.c_str(),"CycleMvAuto Stt"); Step.iSeq = scMvAuto ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleMvPrb ) {Trace(m_sPartName.c_str(),"CycleMvPrb  Stt"); Step.iSeq = scMvPrb  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleBind  ) {Trace(m_sPartName.c_str(),"CycleBind   Stt"); Step.iSeq = scBind   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleUnBind) {Trace(m_sPartName.c_str(),"CycleUnBind Stt"); Step.iSeq = scUnBind ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd     ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default        :                 /*Trace(m_sPartName.c_str(),"default     End");*/Step.iSeq = scIdle ;  return false ;
        case  scIn     : if(CycleIn     ()){ Trace(m_sPartName.c_str(),"CycleIn     End");Step.iSeq = scIdle ;} return false ;
        case  scWK1Rdy : if(CycleWK1Rdy ()){ Trace(m_sPartName.c_str(),"CycleWK1Rdy End");Step.iSeq = scIdle ;} return false ;
        case  scMvAuto : if(CycleMvAuto ()){ Trace(m_sPartName.c_str(),"CycleMvAuto End");Step.iSeq = scIdle ;} return false ;
        case  scMvPrb  : if(CycleMvPrb  ()){ Trace(m_sPartName.c_str(),"CycleMvPrb  End");Step.iSeq = scIdle ;} return false ;
        case  scBind   : if(CycleBind   ()){ Trace(m_sPartName.c_str(),"CycleBind   End");Step.iSeq = scIdle ;} return false ;
        case  scUnBind : if(CycleUnBind ()){ Trace(m_sPartName.c_str(),"CycleUnBind End");Step.iSeq = scIdle ;} return false ;

    }
}

bool CRail::AutorunPRB(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;

//    static int iStep    = 0 ;
//    static int iPreStep = 0 ;
//    static int iWorkEnd = false ;

    if (PRBInfo.iStep == 0) {
        if (Stat.bReqStop && !IO_GetX(xPRB_PkgIn) /*&& ccFwd == AT_GetCmd(aiLDR_Pusher) */) return false ;
        bool isCycleIn = (IO_GetX(xPRB_PkgIn) || OM.CmnOptn.bDryRun) && !DM.ARAY[riPRB].GetCntExist() ;//&& !isCycleMvAuto;
        bool isConEnd  = !isCycleIn;

        if(EM_IsErr()) return false ;

        //Normal Decide Step.
        if (isCycleIn ) {Trace(m_sPartName.c_str(),"CycleIn Stt"); PRBInfo.iStep = 10 ; PRBInfo.iPreStep = 0;} //스트립을 PreBuff로 이송.
        if (isConEnd  ) return true ;
    }

    if (m_tmCycle.OnDelay(PRBInfo.iStep == PRBInfo.iPreStep && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
        EM_SetErr(eiRAL_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : PRBInfo.iStep=%02d" , __FUNC__ , PRBInfo.iStep );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        PRBInfo.iStep = 0 ;
        return false;
    }

    PRBInfo.iPreStep = PRBInfo.iStep ;

    switch (PRBInfo.iStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  //if(iPreStep == iStep)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiPRB_Stopper  , ccFwd);
                  IO_SetY(yPRB_AirBlower,true); //열을 식혀 보장...
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiPRB_Stopper  , ccFwd)) return false ;
                  IO_SetY(yPRB_FeedingAC , true) ;  //ana
                  Step.iCycle++;
                  return false ;

        case 12 : if(!IO_GetX(xPRB_Pkg)&&!OM.CmnOptn.bDryRun) return false ;
                  IO_SetY(yPRB_AirBlower,false); //열을 식혀 보장...
                  Step.iCycle = 0;
                  return true ;
    }

}

bool CRail::CycleIn(void) //스트립을 PreBuff로 이송.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
        EM_SetErr(eiRAL_CycleTO);
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
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiPRB_Stopper  , ccFwd);
                  IO_SetY(yPRB_AirBlower,true); //열을 식혀 보장...
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiPRB_Stopper  , ccFwd)) return false ;
                  IO_SetY(yPRB_FeedingAC , true) ;  //ana
                  Step.iCycle++;
                  return false ;

        case 12 : if(!IO_GetX(xPRB_Pkg)&&!OM.CmnOptn.bDryRun) return false ;
                  DM.ARAY[riPRB].SetStat(csUnkwn) ;
                  IO_SetY(yPRB_AirBlower,false); //열을 식혀 보장...
                  Step.iCycle = 0;
                  return true ;
    }
}

bool CRail::CycleWK1Rdy(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
        EM_SetErr(eiRAL_CycleTO);
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
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : if(IO_GetX(xPRB_PkgOut)) {
                      EM_SetErr(eiPRB_OutStripExist);
                      return false ;
                  }
                  MoveActr(aiWK1_FlpCmp   , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiWK1_FlpCmp   , ccBwd)) return false ;
                  MoveMotr(miWK1_XIns,piWK1_XInsWait);
                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveMotr(miWK1_XIns,piWK1_XInsWait)) return false ;
                  MoveActr(aiWK1_Ins   , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiWK1_Ins   , ccBwd)) return false ;
                  MoveMotr(miWK1_YFlp,piWK1_YFlpNormal);
                  Step.iCycle++;
                  return false ;

        case 14 : if(!MoveMotr(miWK1_YFlp,piWK1_YFlpNormal)) return false ;
                  if(!IO_GetX(xWR1_TurnDetect))
                  {
                      EM_SetErr(eiWK1_FlipperMoveFail);
                      return false ;
                  }
                  MoveActr(aiWK1_Ins   , ccFwd);
                  Step.iCycle++;
                  return false ;

        case 15 : if(!MoveActr(aiWK1_Ins   , ccFwd)) return false ;
                  MoveMotr(miWK1_XIns,piWK1_XInsStpr);
                  Step.iCycle++;
                  return false ;

        case 16 : if(!MoveMotr(miWK1_XIns   , piWK1_XInsStpr)) return false ;
                  MoveActr(aiWK1_Align   , ccFwd);
                  Step.iCycle++;
                  return false ;

        case 17 : if(!MoveActr(aiWK1_Align   , ccFwd)) return false ;
                  MoveActr(aiWK1_FlpCmp  , ccFwd);
                  Step.iCycle++;
                  return false ;

        case 18 : if(!MoveActr(aiWK1_FlpCmp   , ccFwd)) return false ;
                  MoveMotr(miWK1_XIns,piWK1_XInsWait);
                  MoveActr(aiWK1_Align  , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 19 : if(!MoveMotr(miWK1_XIns,piWK1_XInsWait)) return false ;
                  if(!MoveActr(aiWK1_Align   , ccBwd)) return false ;
                  MoveActr(aiWK1_Ins  , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 20 : if(!MoveActr(aiWK1_Ins,ccBwd)) return false ;
                  Step.iCycle = 0;
                  return true ;
    }
}

bool CRail::CycleMvAuto(void) //자동 계산  이동.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
        EM_SetErr(eiRAL_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

//    static bool bCanMvPRB = false ;
    static bool bCanMvWK1 = false ;
    static bool bCanMvWK2 = false ;
    static bool bCanMvWK3 = false ;

    int r1 , r2 , r3 , r4 ;

    int  r , c , iCnt;

    static bool bInPkg  = false ;
    static bool bPsbPkg = false ;

    if(IO_GetX(xPRB_PkgIn )) bInPkg  = true ;
    if(IO_GetX(xPSB_Pkg   )) bPsbPkg = true ;

    int iRslt1 ;
    int iRslt2 ; 
    int iRslt3 ;
    int iRslt4 ;
    int iRslt5 ; 
    int iRslt6 ;
    int iRslt7 ; 
    int iRslt8 ; 
    int iRslt9 ; 
    int iRslt10; 
    int iRslt11;
    int iRslt12; 
    int iRslt13; 
    int iRslt14; 

    int iTotal ;

    bool bCheckSFailCnt ;

//    if( bCanMvWK2&& IO_GetXDn(xWK2_Out)) {MoveActr(aiPSB_FlprCmp , ccFwd); bCatch = true ; Trace("Catch","PSB_PKG");} 다운엣지로 이렇게 음냥.

    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: bInPkg = false ;
                  bPsbPkg= false ;
                  bMrkPkgRemove = false;

                  bCanMvWK3 = DM.ARAY[riWR3].GetCntExist()&& DM.ARAY[riVS3].GetCntExist() &&  !DM.ARAY[riPSB].GetCntExist() ;
                  bCanMvWK2 = DM.ARAY[riWR2].GetCntExist()&& DM.ARAY[riVS2].GetCntExist() && (!DM.ARAY[riWR3].GetCntExist()||bCanMvWK3) ;
                  bCanMvWK1 = DM.ARAY[riWR1].GetCntExist()&& DM.ARAY[riVS1].GetCntExist() && (!DM.ARAY[riWR2].GetCntExist()||bCanMvWK2) ;

                  MoveActr(aiWK1_Align   , ccBwd);
                  MoveActr(aiWK2_Align   , ccBwd);
                  MoveActr(aiWK3_Align   , ccBwd);

                  MoveActr(aiWK1_FlpCmp,ccFwd);
                  MoveActr(aiWK1_Ins   ,ccBwd);

                  MoveActr(aiPRB_Stopper  , ccFwd);
                  MoveActr(aiWK1_Stopper  , ccFwd);
                  MoveActr(aiWK2_Stopper  , ccFwd);
                  MoveActr(aiWK3_Stopper  , ccFwd);

                  if(bCanMvWK1) MoveMotr(miWK1_YFlp,piWK1_YFlpInverse) ;

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiWK1_Align   , ccBwd)) return false ;
                  if(!MoveActr(aiWK2_Align   , ccBwd)) return false ;
                  if(!MoveActr(aiWK3_Align   , ccBwd)) return false ;
                  if(!MoveActr(aiWK1_FlpCmp  , ccFwd)) return false ;
                  if(!MoveActr(aiWK1_Ins     , ccBwd)) return false ;
                  if(!MoveActr(aiPRB_Stopper , ccFwd)) return false ;
                  if(!MoveActr(aiWK1_Stopper , ccFwd)) return false ;
                  if(!MoveActr(aiWK2_Stopper , ccFwd)) return false ;
                  if(!MoveActr(aiWK3_Stopper , ccFwd)) return false ;

                  IO_SetY(yPRB_FeedingAC , true);   //ana
                  IO_SetY(yWK1_FeedingAC , true);


                  if(bCanMvWK1) MoveActr(aiWK1_Stopper,ccBwd);
                  if(bCanMvWK3) {
                      IO_SetY (yPSB_FeedingAC , true);
                      MoveActr(aiWK3_Stopper , ccBwd);
                  }

                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  12: if(bCanMvWK2 && !m_tmTemp.OnDelay(true , OM.CmnOptn.iWK2StprDnDelay))return false ;
                  if(bCanMvWK2) {MoveActr(aiWK2_Stopper , ccBwd); m_tmTemp.Clear();}
                  Step.iCycle++ ;
                  return false ;


        case  13: if(bCanMvWK3&&!MoveActr(aiWK3_Stopper , ccBwd)) return false ;
                  if(bCanMvWK2&&!MoveActr(aiWK2_Stopper , ccBwd)) return false ;

                  //속도 업을 위해 미리 인서트 내려놓는다.
                  if(MT_CmprPos(miWK1_YFlp,GetMotrPos(miWK1_YFlp , piWK1_YFlpInverse))) {
                      MoveActr(aiWK1_Ins    , ccFwd);
                      MoveActr(aiWK1_FlpCmp , ccBwd);
                  }
                  if(bCanMvWK3){
                      //DM.ARAY[riLOT] = DM.ARAY[riWR3] ; //애는 아직 Unkwn 이 Work 임.
                      DM.ARAY[riLT1] = DM.ARAY[riWR3] ; //TODO :: dd add
                      DM.ARAY[riLT1].ChangeStat(csUnkwn,csWork);
                      DM.ARAY[riLT1].SetStep(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - DM.ARAY[riLT1].GetCntStat(csWork));
                      sTemp = "DM.ARAY[riLT1].GetStep() : " + (String)DM.ARAY[riLT1].GetStep();
                      Trace(m_sPartName.c_str(),sTemp.c_str());
                      //DM.ARAY[riLOT].SetStep(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - DM.ARAY[riLOT].GetCntStat(csWork));
                      if(OM.CmnOptn.bMrk1Skip) {//마킹 하는 자재를 스킵 하거나 없으면 지금 자재를 스킵.
                          DM.ARAY[riWR3].SetStat(csWork);
                          OM.CmnOptn.bMrk1Skip = false ;
                          bMrkOneSkip          = true  ; //체크박스 풀기용.Operation 폼
                      }
                      DM.ShiftArrayData(riWR3 , riPSB) ;
                      //DM.ShiftArrayData(riWR3 , riLOT) ;



                      //DM.ARAY[riLT1].SetLotNo(DM.ARAY[riPSB].GetLotNo());
                      //DM.ARAY[riLT1].SetID   (DM.ARAY[riPSB].GetID())

                      //DM.ShiftArrayData(riPSB , riLT1) ;

                      DM.ARAY[riVS4].SetStat(csNone)   ;

                      //DM.ARAY[riPSB].ChangeStat(csWork , csUnkwn) ;

                      iRslt1  = DM.ARAY[riPSB].GetCntStat(csRslt1 ) ;
                      iRslt2  = DM.ARAY[riPSB].GetCntStat(csRslt2 ) ;
                      iRslt3  = DM.ARAY[riPSB].GetCntStat(csRslt3 ) ;
                      iRslt4  = DM.ARAY[riPSB].GetCntStat(csRslt4 ) ;
                      iRslt5  = DM.ARAY[riPSB].GetCntStat(csRslt5 ) ;
                      iRslt6  = DM.ARAY[riPSB].GetCntStat(csRslt6 ) ;
                      iRslt7  = DM.ARAY[riPSB].GetCntStat(csRslt7 ) ;
                      iRslt8  = DM.ARAY[riPSB].GetCntStat(csRslt8 ) ;
                      iRslt9  = DM.ARAY[riPSB].GetCntStat(csRslt9 ) ;
                      iRslt10 = DM.ARAY[riPSB].GetCntStat(csRslt10) ;
                      iRslt11 = DM.ARAY[riPSB].GetCntStat(csRslt11) ;
                      iRslt12 = DM.ARAY[riPSB].GetCntStat(csRslt12) ;
                      iRslt13 = DM.ARAY[riPSB].GetCntStat(csRslt13) ;
                      iRslt14 = DM.ARAY[riPSB].GetCntStat(csRslt14) ;

                      iTotal = iRslt1 + iRslt2 + iRslt3 + iRslt4 + iRslt5 + iRslt6 + iRslt7 + iRslt8 + iRslt9 + iRslt10 + iRslt11 + iRslt12 + iRslt13 + iRslt14 ;

                      sTemp = "DM.ARAY[riPSB] 토탈 페일 갯수 : " + (String)iTotal;
                      Trace(m_sPartName.c_str(),sTemp.c_str());
                      //토탈 페일 갯수.

                      if( OM.DevOptn.iCheckFailCnt && !DM.ARAY[riPSB].CheckAllStat(csNone) ) {
                          if(iTotal>= OM.DevOptn.iCheckFailCnt && OM.CmnOptn.bCheckFailCnt) {
                              bMrkPkgRemove = true;
                          }
                      }

                      //같은 에러 갯수.
                      bCheckSFailCnt = OM.DevOptn.iCheckR1FailCnt || OM.DevOptn.iCheckR2FailCnt || OM.DevOptn.iCheckR3FailCnt || OM.DevOptn.iCheckR4FailCnt ||
                                       OM.DevOptn.iCheckR5FailCnt || OM.DevOptn.iCheckR6FailCnt || OM.DevOptn.iCheckR7FailCnt || OM.DevOptn.iCheckR8FailCnt || OM.DevOptn.iCheckR9FailCnt ||
                                       OM.DevOptn.iCheckR10FailCnt|| OM.DevOptn.iCheckR11FailCnt|| OM.DevOptn.iCheckR12FailCnt|| OM.DevOptn.iCheckR13FailCnt|| OM.DevOptn.iCheckR14FailCnt ;

                      if( bCheckSFailCnt && !DM.ARAY[riPSB].CheckAllStat(csNone) && OM.CmnOptn.bCheckFailCnt) {
//                          if(VC.GetLastErrCnt(1,csRslt0 )>= OM.CmnOptn.iCheckR0FailCnt ) EM_SetErr(eiWK1_VsnSFailCnt);
                          if(OM.DevOptn.iCheckR1FailCnt &&iRslt1 >= OM.DevOptn.iCheckR1FailCnt ) {bMrkPkgRemove = true; }
                          if(OM.DevOptn.iCheckR2FailCnt &&iRslt2 >= OM.DevOptn.iCheckR2FailCnt ) {bMrkPkgRemove = true; }
                          if(OM.DevOptn.iCheckR3FailCnt &&iRslt3 >= OM.DevOptn.iCheckR3FailCnt ) {bMrkPkgRemove = true; }
                          if(OM.DevOptn.iCheckR4FailCnt &&iRslt4 >= OM.DevOptn.iCheckR4FailCnt ) {bMrkPkgRemove = true; }
                          if(OM.DevOptn.iCheckR5FailCnt &&iRslt5 >= OM.DevOptn.iCheckR5FailCnt ) {bMrkPkgRemove = true; }
                          if(OM.DevOptn.iCheckR6FailCnt &&iRslt6 >= OM.DevOptn.iCheckR6FailCnt ) {bMrkPkgRemove = true; }
                          if(OM.DevOptn.iCheckR7FailCnt &&iRslt7 >= OM.DevOptn.iCheckR7FailCnt ) {bMrkPkgRemove = true; }
                          if(OM.DevOptn.iCheckR8FailCnt &&iRslt8 >= OM.DevOptn.iCheckR8FailCnt ) {bMrkPkgRemove = true; }
                          if(OM.DevOptn.iCheckR9FailCnt &&iRslt9 >= OM.DevOptn.iCheckR9FailCnt ) {bMrkPkgRemove = true; }
                          if(OM.DevOptn.iCheckR10FailCnt&&iRslt10>= OM.DevOptn.iCheckR10FailCnt) {bMrkPkgRemove = true; }
                          if(OM.DevOptn.iCheckR11FailCnt&&iRslt11>= OM.DevOptn.iCheckR11FailCnt) {bMrkPkgRemove = true; }
                          if(OM.DevOptn.iCheckR12FailCnt&&iRslt12>= OM.DevOptn.iCheckR12FailCnt) {bMrkPkgRemove = true; }
                          if(OM.DevOptn.iCheckR13FailCnt&&iRslt13>= OM.DevOptn.iCheckR13FailCnt) {bMrkPkgRemove = true; }
                          if(OM.DevOptn.iCheckR14FailCnt&&iRslt14>= OM.DevOptn.iCheckR14FailCnt) {bMrkPkgRemove = true; }
                      }
                  }


                  if(bCanMvWK2){
                      DM.ShiftArrayData(riWR2 , riWR3) ;
                      DM.ARAY[riVS2].SetStat(csNone)   ;
                      DM.ARAY[riVS3].SetStat(csUnkwn)  ;
                      DM.ARAY[riWR3].ChangeStat(csWork , csUnkwn) ;
                  }
                  Step.iCycle++ ;
                  return false ;

        case  14: if(bCanMvWK3){if(!IO_GetX(xWR3_Pkg) || OM.CmnOptn.bDryRun) MoveActr(aiWK3_Stopper , ccFwd) ;}
                  if(bCanMvWK2){if(!IO_GetX(xWR2_Pkg) || OM.CmnOptn.bDryRun) MoveActr(aiWK2_Stopper , ccFwd) ;}
                  //if(bCanMvWK3) if(!AT_GetCmd(aiWK3_Stopper)) return false ;
                  //if(bCanMvWK2) if(!AT_GetCmd(aiWK2_Stopper)) return false ;
                  Step.iCycle++ ;
                  return false ;

        case  15: if(bCanMvWK3&&!MoveActr(aiWK3_Stopper , ccFwd)) return false ;
                  if(bCanMvWK2&&!MoveActr(aiWK2_Stopper , ccFwd)) return false ;
                  Step.iCycle++ ;
                  return false ;

        case  16: if(bCanMvWK1 && !MoveMotr(miWK1_YFlp,piWK1_YFlpInverse)) return false ;
                  if(bCanMvWK1 && !IO_GetX(xWR1_TurnDetect)){
                      EM_SetErr(eiWK1_FlipperMoveFail);
                      return false ;
                  }

                  if(bCanMvWK1) MoveActr(aiWK1_FlpCmp ,ccBwd);
                  if(bCanMvWK1) MoveActr(aiWK1_Ins    ,ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  17: if(bCanMvWK1) if(!MoveActr(aiWK1_FlpCmp ,ccBwd )) return false ;
                  if(bCanMvWK1) if(!MoveActr(aiWK1_Ins    ,ccFwd )) return false ;
                  if(bCanMvWK1) if(!MoveActr(aiWK1_Stopper,ccBwd )) return false ;
                  if(bCanMvWK1) MoveMotr(miWK1_XIns,piWK1_XInsOut);
                  if(bCanMvWK1){
                      DM.ShiftArrayData(riWR1 , riWR2) ;
                      DM.ARAY[riWR2].FlipX();
                      DM.ARAY[riVS1].SetStat(csNone) ;
                      DM.ARAY[riVS2].SetStat(csUnkwn);
                      DM.ARAY[riWR2].ChangeStat(csWork , csUnkwn) ;
                  }

                  if(bCanMvWK1) IO_SetY(yWK1_FeedingAC , true);    //xWK2_Pkg ?
                  Step.iCycle++ ;
                  return false ;

        case  18: if(bCanMvWK1 && !MoveMotr(miWK1_XIns,piWK1_XInsOut)) return false ;
                  if(bCanMvWK1 && !IO_GetX (xWK1_IndxOverload)){
                      EM_SetErr(eiWK1_IndexOverload);
                      return true ;
                  }
                  //if(bCanMvWK1) MoveActr(aiWK1_Ins , ccBwd);  //MoveMotr(miWK1_XIns,piWK1_XInsWait);
                  if(bCanMvWK1) MoveMotr(miWK1_XIns,piWK1_XInsWait);

                  Step.iCycle++ ;
                  return false ;

                  //IO_GetX(xWR2_Pkg) 이조건은 자제가 끼었을때 플리퍼가 돌아버려서 그럼 확실히 확인용.
        case  19: //if(bCanMvWK1&&!MoveActr(aiWK1_Ins , ccBwd))return false ;
                  if(bCanMvWK1&&!MoveMotr(miWK1_XIns,piWK1_XInsWait)) return false ;
                  if(bCanMvWK1&&!IO_GetX(xWR2_Pkg)) return false ;
                  //if(bCanMvWK1) MoveMotr(miWK1_XIns,piWK1_XInsWait);
                  if(bCanMvWK1) MoveActr(aiWK1_Ins , ccBwd);  //MoveMotr(miWK1_XIns,piWK1_XInsWait);
                  if(bCanMvWK1) MoveActr(aiWK1_Stopper,ccFwd);

                  Step.iCycle++ ;
                  return false ;

        case  20: if(bCanMvWK1&&!MoveActr(aiWK1_Ins , ccBwd))return false ;//if(!MoveMotr(miWK1_XIns,piWK1_XInsWait)) return false ;
                  if(bCanMvWK1) MoveMotr(miWK1_YFlp,piWK1_YFlpNormal);
                  Step.iCycle++ ;
                  return false ;

        case  21:
                  Step.iCycle++ ;
                  return false ;

        case  22: //if( bInPkg                        &&!IO_GetX(xPRB_Pkg)) return false ;
                  //if( bCanMvPRB&&!OM.CmnOptn.bDryRun&&!IO_GetX(xWR1_Pkg)) return false ;
                  if(bCanMvWK1 && !MoveMotr(miWK1_YFlp,piWK1_YFlpNormal)) return false ;
                  if(bCanMvWK1 && !IO_GetX(xWR1_TurnDetect)){
                      EM_SetErr(eiWK1_FlipperMoveFail);
                      return false ;
                  }

                  if(bCanMvWK1&&!MoveActr(aiWK1_Ins    ,ccBwd)) return false ;
                  if(bCanMvWK1&&!MoveActr(aiWK1_Stopper,ccFwd)) return false ;
                  //if(bCanMvWK1&&!MoveMotr(miWK1_XIns,piWK1_XInsWait)) return false ;   UPH 위해.
                  if(bCanMvWK1&&!OM.CmnOptn.bDryRun&&!IO_GetX(xWR2_Pkg)) return false ;
                  if(bCanMvWK2&&!OM.CmnOptn.bDryRun&&!IO_GetX(xWR3_Pkg)) return false ;
                  MoveActr(aiWK2_Align , ccFwd);
                  MoveActr(aiWK3_Align , ccFwd);

                  Step.iCycle++;
                  return false ;

        case  23: if(!MoveActr(aiWK1_Align , ccFwd))return false ;
                  if(!MoveActr(aiWK2_Align , ccFwd))return false ;

                  Step.iCycle++;
                  return false ;

        case  24:
                  Step.iCycle++;
                  return false ;

        case  25:
                  IO_SetY(yWK1_FeedingAC , false);
                  Step.iCycle++;
                  return false ;

        case  26: //if(bCanMvWK3&&!OM.CmnOptn.bDryRun&& (!IO_GetX(xPSB_Pkg)&&!AT_Complete(aiPSB_Lift,ccFwd)) ) return false ; // 마지막 자재일경우를 대비 해서 하긴 하는데 상황 아직 잘 모르겟음.
                  if(bCanMvWK3&&!OM.CmnOptn.bDryRun&& !bPsbPkg ) return false ; // 마지막 자재일경우를 대비 해서 하긴 하는데 상황 아직 잘 모르겟음.
                  if(PSB.GetSeqStep() != CPstBuff::scOut && PSB.GetSeqStep() != CPstBuff::scBind) IO_SetY(yPSB_FeedingAC , false); //이런 더러운 경우가.dd;

                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CRail::CycleMvPrb(void) //자동 계산  이동.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
        EM_SetErr(eiRAL_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        IO_SetY(yPRB_AirBlower , false);
        IO_SetY(yPRB_FeedingAC , false);

        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    static bool bInPkg = false ;

    if(IO_GetX(xPRB_PkgIn)) {
        bInPkg = true ;
        IO_SetY(yPRB_FeedingAC, true);
        IO_SetY(yPRB_AirBlower , true);
        Trace("CycleMvPrb","bInPkg");
    }
    if(bInPkg && !IO_GetX(xPRB_PkgIn)) {
        DM.ARAY[riPRB].SetStat(csUnkwn) ;
        IO_SetY(yPRB_AirBlower , false);
        Trace("CycleMvPrb","Data Set");
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: bInPkg = false ;
                  MoveActr(aiWK1_FlpCmp , ccBwd);
                  MoveActr(aiWK1_Align  , ccBwd);
                  MoveActr(aiWK1_Ins    , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  11: if(!MoveActr(aiWK1_FlpCmp , ccBwd)) return false ;
                  if(!MoveActr(aiWK1_Align  , ccBwd)) return false ;
                  if(!MoveActr(aiWK1_Ins    , ccBwd)) return false ;
                  IO_SetY(yPRB_FeedingAC,true);
                  MoveActr(aiPRB_Stopper , ccBwd) ;
                  MoveActr(aiWK1_Stopper , ccFwd) ;
                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveActr(aiPRB_Stopper , ccBwd)) return false ;
                  if(!MoveActr(aiWK1_Stopper , ccFwd)) return false ;
                  m_sWorkedLotNo = DM.ARAY[riPRB].GetLotNo() ;
                  m_sWorkedId    = DM.ARAY[riPRB].GetID() ;
                  DM.ShiftArrayData(riPRB , riWR1) ;
                  DM.ARAY[riVS1].SetStat(csUnkwn);

                  Step.iCycle++ ;
                  return false ;

        case  13: if(!IO_GetX(xPRB_Pkg) || OM.CmnOptn.bDryRun) MoveActr(aiPRB_Stopper , ccFwd) ;
                  if(!AT_GetCmd(aiPRB_Stopper)) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  14: if(!MoveActr(aiPRB_Stopper , ccFwd)) return false ;
                  if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPrbToWk1Delay))
                  {
                      EM_SetErr(eiPRB_StripFeedingFail);
                      IO_SetY(yPRB_FeedingAC,false);
                      return false ;
                  }
                  if(IO_GetX(xPRB_PkgOut) || IO_GetX(xPRB_Pkg) )return false ;//Doking ...X

                  IO_SetY(yPRB_FeedingAC,false); 
                  MoveMotr(miWK1_XIns,piWK1_XInsWait);
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!MoveMotr(miWK1_XIns,piWK1_XInsWait)) return false ;
                  MoveActr(aiWK1_Ins    ,ccFwd);
                  MoveActr(aiWK1_FlpCmp ,ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  16: if(!MoveActr(aiWK1_Ins    ,ccFwd)) return false ;
                  if(!MoveActr(aiWK1_FlpCmp ,ccBwd)) return false ;
//                  MT_GoAbs(miWK1_XIns,GetMotrPos(miWK1_XIns,piWK1_XInsStpr),OM.CmnOptn.dWK1InsStprPsSpeed ) ;
                  MoveMotr(miWK1_XIns,piWK1_XInsStpr);
//                  DM.ShiftArrayData(riPRB , riWR1) ;
//                  DM.ARAY[riVS1].SetStat(csUnkwn);
                  Step.iCycle++ ;
                  return false ;

        case  17: if(!MoveMotr(miWK1_XIns,piWK1_XInsStpr)) return false ;
//                  if(!IO_GetX(xWR1_IndxOverload))
//                  {
//                      EM_SetErr(eiWK1_IndexOverload);
//                      return false ;
//                  }
                  if(!IO_GetX(xWR1_Pkg)) {
                      EM_SetErr(eiWK1_PkgDispr);
                      return false ;
                  }


                  MoveActr(aiWK1_Align,ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  18: if(!MoveActr(aiWK1_Align,ccFwd)) return false ;
                  MoveActr(aiWK1_FlpCmp,ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  19: if(!MoveActr(aiWK1_FlpCmp,ccFwd)) return false ;
                  MoveActr(aiWK1_Ins,ccBwd);
                  MoveActr(aiWK1_Align,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  20: if(!MoveActr(aiWK1_Ins  ,ccBwd)) return false ;
                  if(!MoveActr(aiWK1_Align,ccBwd)) return false ;
                  MoveMotr(miWK1_XIns,piWK1_XInsWait);
                  Step.iCycle++ ;
                  return false ;

        case  21: if( bInPkg && !IO_GetX(xPRB_Pkg)) return false ;


                  IO_SetY(yPRB_FeedingAC, false);
                  IO_SetY(yPRB_AirBlower , false);

                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CRail::CycleBind(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiRAL_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

//    static bool bExistWK1 = false ;
    static bool bExistWK2 = false ;
    static bool bExistWK3 = false ;    

    bool r1 , r2 , r3 , r4 ;

    //static bool bInPkg = false ;

    //if(IO_GetX(xPRB_PkgIn1) || IO_GetX(xPRB_PkgIn2)) bInPkg = true ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: bExistWK3 = DM.ARAY[riWR3].GetCntExist();
                  bExistWK2 = DM.ARAY[riWR2].GetCntExist();
//                  bExistWK1 = DM.ARAY[riWR1].GetCntExist();

//                  MoveActr(aiWK1_Stopper , ccFwd);
                  MoveActr(aiWK2_Stopper , ccFwd);
                  MoveActr(aiWK3_Stopper , ccFwd);

                  Step.iCycle++;
                  return false ;

        case  11: //if(!MoveActr(aiWK1_Stopper , ccFwd)) return false ;
                  if(!MoveActr(aiWK2_Stopper , ccFwd)) return false ;
                  if(!MoveActr(aiWK3_Stopper , ccFwd)) return false ;
                  IO_SetY(yWK1_FeedingAC , true);
                  m_tmTemp.Clear();

                  Step.iCycle++;
                  return false ;

        case  12: if(!m_tmTemp.OnDelay(true , 300)) return false ;
                  MoveActr(aiWK2_Align , ccFwd);
                  MoveActr(aiWK3_Align , ccFwd);

                  Step.iCycle++ ;
                  return false ;

        case  13: //if(!MoveActr(aiWK1_Align , ccFwd)) return false ;
                  if(!MoveActr(aiWK2_Align , ccFwd)) return false ;
                  if(!MoveActr(aiWK3_Align , ccFwd)) return false ;

                  Step.iCycle++ ;
                  return false ;

        case  14: //MoveActr(aiWK2_Align , ccBwd);
                  //MoveActr(aiWK3_Align , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  15: //if(!MoveActr(aiWK1_Align , ccBwd)) return false ;
                  //if(!MoveActr(aiWK2_Align , ccBwd)) return false ;
                  //if(!MoveActr(aiWK3_Align , ccBwd)) return false ;
                  IO_SetY(yWK1_FeedingAC , false);
                  //스테이지 올리고 바꿔야 바인딩 되고 HEAD에서 검사를 제때 시작 한다.                  DM.ARAY[riWK1].GetCntExist()
//                  if( bExistWK1 && DM.ARAY[riVS1].CheckAllStat(csNone))DM.ARAY[riVS1].SetStat(csUnkwn);
                  if( bExistWK2 && DM.ARAY[riVS2].CheckAllStat(csNone))DM.ARAY[riVS2].SetStat(csUnkwn);
                  if( bExistWK3 && DM.ARAY[riVS3].CheckAllStat(csNone))DM.ARAY[riVS3].SetStat(csUnkwn);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CRail::CycleUnBind(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiRAL_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    bool r1 , r2 , r3 , r4 ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //bExistWK3 = DM.ARAY[riWR3].GetCntExist();
                  //bExistWK2 = DM.ARAY[riWR2].GetCntExist();
//                  bExistWK1 = DM.ARAY[riWR1].GetCntExist();

//                  MoveActr(aiWK1_Stopper , ccFwd);
                  MoveActr(aiWK2_Stopper , ccFwd);
                  MoveActr(aiWK3_Stopper , ccFwd);

                  MoveActr(aiWK2_Align , ccBwd);
                  MoveActr(aiWK3_Align , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  11: //if(!MoveActr(aiWK1_Stopper , ccFwd)) return false ;
                  if(!MoveActr(aiWK2_Stopper , ccFwd)) return false ;
                  if(!MoveActr(aiWK3_Stopper , ccFwd)) return false ;
                  if(!MoveActr(aiWK2_Align   , ccBwd)) return false ;
                  if(!MoveActr(aiWK3_Align   , ccBwd)) return false ;

                  Step.iCycle++ ;
                  return false ;

        case  12: Step.iCycle = 0 ;
                  return true ;
    }
}

void CRail::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CRail::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CRail::CheckStop()
{
    if(!AT_Done(aiPRB_Stopper )) return false ;
    if(!AT_Done(aiWK1_Align   )) return false ;
    if(!AT_Done(aiWK1_FlpCmp  )) return false ;
    if(!AT_Done(aiWK1_Ins     )) return false ;
    if(!AT_Done(aiWK1_Stopper )) return false ;
    if(!AT_Done(aiWK2_Align   )) return false ;
    if(!AT_Done(aiWK2_Stopper )) return false ;
    if(!AT_Done(aiWK3_Align   )) return false ;
    if(!AT_Done(aiWK3_Stopper )) return false ;

    if(!MT_GetStop(miWK1_XIns)) return false ;
    if(!MT_GetStop(miWK1_YFlp)) return false ;

    return true ;
}

void CRail::Load(bool IsLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() +".INI";

    //Load Device.
    Trace( "CRail","Load Start") ;

    if(IsLoad) {
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "iPsbiRowCnt   " , iPsbiRowCnt   );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "iVsFailCnt    " , iVsFailCnt    );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "bMrkOneSkip   " , bMrkOneSkip   );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_bLotEnded   " , m_bLotEnded   );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedLotNo" , m_sWorkedLotNo);
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedId   " , m_sWorkedId   );
    }

    else {
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "iPsbiRowCnt   " , iPsbiRowCnt   );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "iVsFailCnt    " , iVsFailCnt    );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "bMrkOneSkip   " , bMrkOneSkip   );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_bLotEnded   " , m_bLotEnded   );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedLotNo" , m_sWorkedLotNo);
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedId   " , m_sWorkedId   );
    }
    Trace( "CRail","Load End") ;
}

bool CRail::GetLastStrip()
{
    bool bLDRExist = !DM.ARAY[riLDR].CheckAllStat(csNone) ;
    bool bPRBExist = !DM.ARAY[riPRB].CheckAllStat(csNone) ;
    bool bWR1Exist = !DM.ARAY[riWR1].CheckAllStat(csNone) ;
    bool bWR2Exist = !DM.ARAY[riWR2].CheckAllStat(csNone) ;

    bool bLDRDiff  = DM.ARAY[riWR3].GetLotNo() != DM.ARAY[riLDR].GetLotNo() ;
    bool bPRBDiff  = DM.ARAY[riWR3].GetLotNo() != DM.ARAY[riPRB].GetLotNo() ;
    bool bWR1Diff  = DM.ARAY[riWR3].GetLotNo() != DM.ARAY[riWR1].GetLotNo() ;
    bool bWR2Diff  = DM.ARAY[riWR3].GetLotNo() != DM.ARAY[riWR2].GetLotNo() ;

    bool bLastMgz  = LDR._iMgzCnt >= OM.DevOptn.iLotEndMgzCnt ;

         if(bWR2Exist) return bWR2Diff ;
    else if(bWR1Exist) return bWR1Diff ;
    else if(bPRBExist) return bPRBDiff ;
    else if(bLDRExist) return bLDRDiff ;
    else if(!bLastMgz) return false    ;
    else               return true     ;
}

