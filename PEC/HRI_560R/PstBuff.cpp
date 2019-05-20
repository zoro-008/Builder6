//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "PstBuff.h"
#include "UnLoader.h"
#include "Rail.h"
#include "Loader.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "VisnComUnit.h"
#include "SPCUnit.h"
#include "UserIni.h"
#include "LotUnit.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CPstBuff  PSB;

CPstBuff::CPstBuff(void)
{
    Init();
}

void CPstBuff::Init()
{
    m_sPartName = "Post Buff " ;
    m_iTrimErr  = 0 ;
    Reset();
    Load(true);
}

void CPstBuff::Reset()
{
    ResetTimer();
    isRunWarn = false;

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CPstBuff::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CPstBuff::~CPstBuff (void)
{
    Close();
}
void CPstBuff::Close()
{
    Load(false);
}

bool CPstBuff::FindChipMrk( int &r , int &c ) //안씀.
{
    int cSel  = OM.DevInfo.iColCnt ;
    int rTemp = 0 ;


//    c = DM.ARAY[riPSB].FindFrstCol(csRslt0 ); if(c != -1){if(cSel > c) cSel = c ; }
    c = DM.ARAY[riPSB].FindFrstCol(csRslt1  ); if(c != -1){if(cSel > c) cSel = c ; }
    c = DM.ARAY[riPSB].FindFrstCol(csRslt2  ); if(c != -1){if(cSel > c) cSel = c ; }
    c = DM.ARAY[riPSB].FindFrstCol(csRslt3  ); if(c != -1){if(cSel > c) cSel = c ; }
    c = DM.ARAY[riPSB].FindFrstCol(csRslt4  ); if(c != -1){if(cSel > c) cSel = c ; }
    c = DM.ARAY[riPSB].FindFrstCol(csRslt5  ); if(c != -1){if(cSel > c) cSel = c ; }
    c = DM.ARAY[riPSB].FindFrstCol(csRslt6  ); if(c != -1){if(cSel > c) cSel = c ; }
    c = DM.ARAY[riPSB].FindFrstCol(csRslt7  ); if(c != -1){if(cSel > c) cSel = c ; }
    c = DM.ARAY[riPSB].FindFrstCol(csRslt8  ); if(c != -1){if(cSel > c) cSel = c ; }
    c = DM.ARAY[riPSB].FindFrstCol(csRslt9  ); if(c != -1){if(cSel > c) cSel = c ; }
    c = DM.ARAY[riPSB].FindFrstCol(csRslt10 ); if(c != -1){if(cSel > c) cSel = c ; }
    c = DM.ARAY[riPSB].FindFrstCol(csRslt11 ); if(c != -1){if(cSel > c) cSel = c ; }
    c = DM.ARAY[riPSB].FindFrstCol(csRslt12 ); if(c != -1){if(cSel > c) cSel = c ; }
    c = DM.ARAY[riPSB].FindFrstCol(csRslt13 ); if(c != -1){if(cSel > c) cSel = c ; }
    c = DM.ARAY[riPSB].FindFrstCol(csRslt14 ); if(c != -1){if(cSel > c) cSel = c ; }
    c = DM.ARAY[riPSB].FindFrstCol(csFail   ); if(c != -1){if(cSel > c) cSel = c ; }

    if(cSel % 2){

        rTemp = -1 ;
        DM.ARAY[riPSB].FindFrstColLastRow(csRslt1  , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColLastRow(csRslt2  , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColLastRow(csRslt3  , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColLastRow(csRslt4  , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColLastRow(csRslt5  , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColLastRow(csRslt6  , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColLastRow(csRslt7  , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColLastRow(csRslt8  , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColLastRow(csRslt9  , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColLastRow(csRslt10 , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColLastRow(csRslt11 , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColLastRow(csRslt12 , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColLastRow(csRslt13 , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColLastRow(csRslt14 , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColLastRow(csFail   , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;

    }
    else { //삐구다.
        rTemp = OM.DevInfo.iRowCnt ;
        DM.ARAY[riPSB].FindFrstColRow    (csRslt1  , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColRow    (csRslt2  , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColRow    (csRslt3  , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColRow    (csRslt4  , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColRow    (csRslt5  , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColRow    (csRslt6  , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColRow    (csRslt7  , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColRow    (csRslt8  , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColRow    (csRslt9  , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColRow    (csRslt10 , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColRow    (csRslt11 , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColRow    (csRslt12 , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColRow    (csRslt13 , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColRow    (csRslt14 , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColRow    (csFail   , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
    }
    r = rTemp ; c = cSel ;
    return (r > -1 && c > -1 );

}

bool CPstBuff::FindChipVsn(EN_CHIP_STAT _iStat , int &r , int &c )
{
    int rTemp = -1 , cTemp = -1 ;
    int rRst  = -1 , cRst  = OM.DevInfo.iColCnt ;

    //Col Find
    cTemp = DM.ARAY[riPSB].FindFrstCol(_iStat);
    if(cTemp != -1 && cRst > cTemp) cRst = cTemp ;

    //Row Find
    if(cRst % 2){
        DM.ARAY[riPSB].FindFrstColLastRow(_iStat , rTemp , cTemp) ;
        if(rTemp != -1 && cTemp == cRst && rRst<rTemp) rRst = rTemp ;
    }

    else{
        rRst = OM.DevInfo.iRowCnt ;
        DM.ARAY[riPSB].FindFrstColRow(_iStat , rTemp , cTemp) ;
        if(rTemp != -1 && cTemp == cRst && rRst>rTemp) rRst = rTemp ;
    }

    r= rRst ; c= cRst ;
    return (r > -1 && c > -1 );

}
double CPstBuff::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int     r ,  c ,  r1 ,  c1 ,  r2 ,  c2;
    int    gr , gc , gr1 , gc1 , gr2 , gc2;
    double dPos = 0.0 ;


    FindChipMrk(r  , c );
    FindChipVsn(csMarking , r1 , c1);
    FindChipVsn(csReMark  , r2 , c2);

    //X , Y Pos Setting.
    double dVsnXPos   =  PM.GetValue(miPSB_XMrk , pvPSB_XMrkVisnStart) - c1 * OM.DevInfo.dColPitch ;
    double dVsnYPos   =  PM.GetValue(miPSB_YMrk , pvPSB_YMrkVisnStart) + r1 * OM.DevInfo.dRowPitch ;
    double dXPos      =  PM.GetValue(miPSB_XMrk , pvPSB_XMrkWorkStart) - c  * OM.DevInfo.dColPitch ;
    double dYPos      =  PM.GetValue(miPSB_YMrk , pvPSB_YMrkWorkStart) + r  * OM.DevInfo.dRowPitch ;
    double dReMrkXPos =  PM.GetValue(miPSB_XMrk , pvPSB_XMrkWorkStart) - c2 * OM.DevInfo.dColPitch ;
    double dReMrkYPos =  PM.GetValue(miPSB_YMrk , pvPSB_YMrkWorkStart) + r2 * OM.DevInfo.dRowPitch ;

    if(OM.DevInfo.iColGrCnt) {
        gc         = c  / OM.DevInfo.iColGrCnt ;
        gc1        = c1 / OM.DevInfo.iColGrCnt ;
        gc2        = c2 / OM.DevInfo.iColGrCnt ;
        dXPos      = dXPos      - (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * gc ;
        dVsnXPos   = dVsnXPos   - (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * gc1;
        dReMrkYPos = dReMrkXPos - (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * gc2;

    }

    if( OM.DevInfo.iRowGrCnt) {
        gr         = r  / OM.DevInfo.iRowGrCnt ;
        gr1        = r1 / OM.DevInfo.iRowGrCnt ;
        gr2        = r2 / OM.DevInfo.iRowGrCnt ;
        dYPos      = dYPos      + (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * gr ;
        dVsnYPos   = dVsnYPos   + (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * gr1;
        dReMrkYPos = dReMrkYPos + (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * gr2;
    }

    if(_iMotr ==  miPSB_XMrk)
    {
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos (_iMotr                       ); break ;
            case piPSB_XMrkWait      : dPos = PM.GetValue  (_iMotr , pvPSB_XMrkWait      ); break ;
            case piPSB_XMrkWorkStart : dPos = PM.GetValue  (_iMotr , pvPSB_XMrkWorkStart ); break ;
            case piPSB_XMrkVisnStart : dPos = PM.GetValue  (_iMotr , pvPSB_XMrkVisnStart ); break ;
            case piPSB_XMrkVisn      : dPos = dVsnXPos                                    ; break ;
            case piPSB_XMrkWork      : dPos = dXPos                                       ; break ;
            case piPSB_XMrkReWork    : dPos = dReMrkXPos                                  ; break ;
        }
    }
    else if(_iMotr ==  miPSB_YMrk)
    {
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos (_iMotr                       ); break ;
            case piPSB_YMrkWait      : dPos = PM.GetValue  (_iMotr , pvPSB_YMrkWait      ); break ;
            case piPSB_YMrkWorkStart : dPos = PM.GetValue  (_iMotr , pvPSB_YMrkWorkStart ); break ;
            case piPSB_YMrkVisnStart : dPos = PM.GetValue  (_iMotr , pvPSB_YMrkVisnStart ); break ;
            case piPSB_YMrkVisn      : dPos = dVsnYPos                                    ; break ;
            case piPSB_YMrkWork      : dPos = dYPos                                       ; break ;
            case piPSB_YMrkReWork    : dPos = dReMrkYPos                                  ; break ;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}
//---------------------------------------------------------------------------
bool CPstBuff::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId) )) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool isMarkingDn = !AT_MoveCyl(aiPSB_Marking , ccBwd ) ;

    if(_iMotr == miPSB_XMrk){
        if(isMarkingDn) { sMsg = "isMarkingDn" ; bRet = false ;}
        switch(_iPstnId) {
            default                   : break ;
            case piPSB_XMrkWait       : break ;
            case piPSB_XMrkWorkStart  : break ;
            case piPSB_XMrkVisnStart  : break ;
            case piPSB_XMrkWork       : break ;
            case piPSB_XMrkVisn       : break ;
        }
    }
    else if(_iMotr == miPSB_YMrk){
        if(isMarkingDn) { sMsg = "isMarkingDn" ; bRet = false ;}
        switch(_iPstnId) {
            default                   : break ;
            case piPSB_YMrkWait       : break ;
            case piPSB_YMrkWorkStart  : break ;
            case piPSB_YMrkVisnStart  : break ;
            case piPSB_YMrkWork       : break ;
            case piPSB_YMrkVisn       : break ;
        }
    }
    else {
       bRet = false;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CPstBuff::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

//    bool isMarkingDn = AT_Complete(aiPSB_Marking , ccFwd ) ;
    bool isXMoving   = MT_GetStopInpos(miPSB_XMrk);
    bool isYMoving   = MT_GetStopInpos(miPSB_YMrk);

    if(_iActr == aiPSB_Marking){
        if(!isXMoving && !isYMoving) { sMsg = "isXMrkMoving || isYMrkMoving" ; bRet = false ;}
    }
    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}
void CPstBuff::SetTrgPos()
{
    AnsiString sTemp ;

    sTemp = "Trigger OneShot";

    Trace("PsbTrigger" , sTemp.c_str());

    MT_OneShotAxtTrg(miPSB_XMrk , true , 1000);

}

void CPstBuff::ResetTrgPos()
{
    MT_ResetAxtTrgPos(miPSB_XMrk) ;
}

bool CPstBuff::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CPstBuff::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPstBuff::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
        EM_SetErr(eiPSB_HomeTO);
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

        case 10: MT_Reset(miPSB_XMrk);
                 MT_Reset(miPSB_YMrk);

                 MT_SetServo(miPSB_XMrk,true);
                 MT_SetServo(miPSB_YMrk,true);

                 AT_MoveCyl(aiPSB_Align    , ccBwd);
                 AT_MoveCyl(aiPSB_Marking  , ccBwd);
                 AT_MoveCyl(aiPSB_Stopper  , ccFwd);
                 AT_MoveCyl(aiPSB_RjctOpen , ccFwd);
                 AT_MoveCyl(aiPSB_RjctUpDn , ccBwd);


                 IO_SetY(yPSB_FeedingAC , false ) ;

                 Step.iHome++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPSB_Align    , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_Marking  , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_Stopper  , ccFwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_RjctOpen , ccFwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_RjctUpDn , ccBwd)) return false ;

                 SetTrgPos  ();
                 ResetTrgPos();
                 
                 MT_DoHome(miPSB_XMrk);
                 MT_DoHome(miPSB_YMrk);

                 Step.iHome++ ;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miPSB_XMrk)) return false ;
                 if(!MT_GetHomeEnd(miPSB_YMrk)) return false ;

                 MT_GoAbsMan(miPSB_XMrk , PM.GetValue(miPSB_XMrk,pvPSB_XMrkWait)) ;
                 MT_GoAbsMan(miPSB_YMrk , PM.GetValue(miPSB_YMrk,pvPSB_YMrkWait)) ;

                 Step.iHome++ ;
                 return false ;

        case 13: if(!MT_GoAbsMan(miPSB_XMrk , PM.GetValue(miPSB_XMrk,pvPSB_XMrkWait)))return false ;
                 if(!MT_GoAbsMan(miPSB_YMrk , PM.GetValue(miPSB_YMrk,pvPSB_YMrkWait)))return false ;
                 AT_MoveCyl(aiPSB_Marking  , ccFwd);  // 0에 있어도 부딪힐건 없을꺼 같네요.
                 Step.iHome++ ;
                 return false ;

        case 14: if(!AT_MoveCyl(aiPSB_Marking    , ccFwd)) return false ;
                 if(!OM.CmnOptn.bVsMrkSkip) VC.SendReset(vsVisn4);
                 Step.iHome = 0;
                 return true ;
    }
}

bool CPstBuff::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //Clear Timer.
//    m_ToStopTimer.Clear();

    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10 ;

    //Ok.
    return true;

}

bool CPstBuff::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CPstBuff::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiPSB_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart);
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: IO_SetY(yPSB_FeedingAC  , false) ;
                 MoveActr(aiPSB_RjctOpen , ccFwd) ;
                 MoveActr(aiPSB_RjctUpDn , ccBwd) ;
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiPSB_RjctOpen , ccFwd)) return false;
                 if(!MoveActr(aiPSB_RjctUpDn , ccBwd)) return false;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CPstBuff::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() && !OM.MstOptn.bDebugMode , 8000)) EM_SetErr(eiPSB_ToStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    Stat.bReqStop = false ;

    bool bWaitPos = MT_CmprPos(miPSB_XMrk,GetMotrPos(miPSB_XMrk,piPSB_XMrkWait)) && MT_CmprPos(miPSB_YMrk,GetMotrPos(miPSB_YMrk,piPSB_YMrkWait)) ;

    //Move Home.
    switch (Step.iToStop) {
        default: Step.iToStop = 0;
                 return true ;

        case 10: if(RAL.GetSeqStep() != CRail::scMvAuto) IO_SetY(yPSB_FeedingAC , false ) ;
                 MoveActr(aiPSB_Align,ccBwd);
                 Step.iToStop++ ;
                 return false ;

        case 11: if(!MoveActr(aiPSB_Align,ccBwd)) return false ;
                 if(!bWaitPos) { MoveActr(aiPSB_Marking  , ccBwd); }
                 Step.iToStop++ ;
                 return false ;

        case 12: if(!bWaitPos) { if(!MoveActr(aiPSB_Marking    , ccBwd)) return false ; }
                 if(!bWaitPos) { MT_GoAbsMan(miPSB_XMrk , GetMotrPos(miPSB_XMrk,piPSB_XMrkWait)) ; }
                 if(!bWaitPos) { MT_GoAbsMan(miPSB_YMrk , GetMotrPos(miPSB_YMrk,piPSB_YMrkWait)) ; }

                 Step.iToStop++ ;
                 return false ;

        case 13: if(!bWaitPos) { if(!MT_GoAbsMan(miPSB_XMrk , GetMotrPos(miPSB_XMrk,piPSB_XMrkWait)))return false ; }
                 if(!bWaitPos) { if(!MT_GoAbsMan(miPSB_YMrk , GetMotrPos(miPSB_YMrk,piPSB_YMrkWait)))return false ; }
                 MoveActr(aiPSB_Marking  , ccFwd);  // 0에 있어도 부딪힐건 없을꺼 같네요.
                 IO_SetY(yPSB_RejectMotor , false);
                 Step.iToStop++ ;
                 return false ;

        case 14: if(!MoveActr(aiPSB_Marking    , ccFwd)) return false ;
                 Step.iToStop = 0   ;
                 return true ;
    }
}


bool CPstBuff::Autorun(void) //오토런닝시에 계속 타는 함수.
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

        bool isExstFail = DM.ARAY[riPSB].GetCntStat(csFail  ) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt0 ) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt1 ) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt2 ) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt3 ) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt4 ) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt5 ) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt6 ) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt7 ) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt8 ) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt9 ) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt10) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt11) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt12) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt13) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt14) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt15) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt16) ;

        bool isNeedVsn    = !DM.ARAY[riPSB].CheckAllStat(csNone) &&  DM.ARAY[riPSB].GetCntStat(csMarking)&& !OM.CmnOptn.bVsMrkSkip ;
        bool isNeedMrk    = !DM.ARAY[riPSB].CheckAllStat(csNone) && !DM.ARAY[riPSB].CheckAllStat(csWork) && !DM.ARAY[riPSB].GetCntStat(csMarking) &&  isExstFail && !OM.CmnOptn.bMrkAllSkip ;

        bool isCycleReady = !DM.ARAY[riPSB].GetCntExist() && (!AT_Complete(aiPSB_Align    , ccBwd)||
                                                              !AT_Complete(aiPSB_Marking  , ccFwd)||
                                                              !AT_Complete(aiPSB_Stopper  , ccFwd)) ;

        bool isCycleBind   = (isNeedMrk || isNeedVsn) && IO_GetX(xPSB_Pkg) && !RAL.bMrkPkgRemove && AT_Complete(aiPSB_Align , ccBwd);
        bool isCycleRemove =  RAL.bMrkPkgRemove && !DM.ARAY[riPSB].CheckAllStat(csNone) && !OM.CmnOptn.bAfterMrkRemove && IO_GetX(xPSB_Pkg);
        bool isCycleWork   =  isNeedMrk && !isNeedVsn  && !RAL.bMrkPkgRemove && !OM.CmnOptn.bMrkAllSkip && AT_Complete(aiPSB_Align , ccFwd);
        bool isCycleAfter  =  RAL.bMrkPkgRemove && !DM.ARAY[riPSB].CheckAllStat(csNone) && OM.CmnOptn.bAfterMrkRemove;
        bool isCycleVisn   =  isNeedVsn && !RAL.bMrkPkgRemove && !OM.CmnOptn.bVsMrkSkip && AT_Complete(aiPSB_Align , ccFwd);

        bool isCycleOut   =  !DM.ARAY[riPSB].CheckAllStat(csNone) && //DM.ARAY[riPSB].CheckAllStat(csWork) &&
                           (!isExstFail || OM.CmnOptn.bMrkAllSkip) && DM.ARAY[riULD].GetCntStat(csNone) &&
                            (IO_GetX(xPSB_Pkg) || IO_GetX(xPSB_PkgOut)) &&
                             RAL.GetSeqStep() != CRail::scMvAuto &&
                             ULD.GetSeqStep() == CUnLoader::scIdle ;
        bool isConEnd     =  DM.ARAY[riPSB].CheckAllStat(csNone) && !IO_GetX(xPSB_PkgOut);

        //모르는 스트립에러.
        if( DM.ARAY[riPSB].CheckAllStat(csNone) &&  IO_GetX(xPSB_Pkg) ) EM_SetErr(eiPSB_PkgUnknown ) ;

        //스트립 사라짐 에러.
        if(!DM.ARAY[riPSB].CheckAllStat(csNone) && RAL.GetSeqStep() != CRail::scMvAuto && (!IO_GetX(xPSB_Pkg) && !IO_GetX(xPSB_PkgOut)) && !OM.CmnOptn.bDryRun) EM_SetErr(eiPSB_PkgDispr ) ;

        //리젝 풀 에러.
        if(IO_GetX(xPSB_RejectDetect)) EM_SetErr(eiPSB_RejectFull);

        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleReady ) {Trace(m_sPartName.c_str(),"CycleReady  Stt"); Step.iSeq = scReady  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleBind  ) {Trace(m_sPartName.c_str(),"CycleBind   Stt"); Step.iSeq = scBind   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleRemove) {Trace(m_sPartName.c_str(),"CycleRemove Stt"); Step.iSeq = scRemove ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork  ) {Trace(m_sPartName.c_str(),"CycleWork   Stt"); Step.iSeq = scWork   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleAfter ) {Trace(m_sPartName.c_str(),"CycleAfter  Stt"); Step.iSeq = scAfter  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleVisn  ) {Trace(m_sPartName.c_str(),"CycleVisn   Stt"); Step.iSeq = scVisn   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleOut   ) {Trace(m_sPartName.c_str(),"CycleOut    Stt"); Step.iSeq = scOut    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd     ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default      :              /*Trace(m_sPartName.c_str(),"default    End");Step.iSeq = scIdle ;*/  return false ;
        case  scReady  : if(CycleReady  ()){Trace(m_sPartName.c_str(),"CycleReady  End");Step.iSeq = scIdle ;} return false ;
        case  scBind   : if(CycleBind   ()){Trace(m_sPartName.c_str(),"CycleBind   End");Step.iSeq = scIdle ;} return false ;
        case  scRemove : if(CycleRemove ()){Trace(m_sPartName.c_str(),"CycleRemove End");Step.iSeq = scIdle ;} return false ;
        case  scWork   : if(CycleWork   ()){Trace(m_sPartName.c_str(),"CycleWork   End");Step.iSeq = scIdle ;} return false ;
        case  scAfter  : if(CycleRemove ()){Trace(m_sPartName.c_str(),"CycleAfter  End");Step.iSeq = scIdle ;} return false ;
        case  scVisn   : if(CycleVisn   ()){Trace(m_sPartName.c_str(),"CycleVisn   End");Step.iSeq = scIdle ;} return false ;
        case  scOut    : if(CycleOut    ()){Trace(m_sPartName.c_str(),"CycleOut    End");Step.iSeq = scIdle ;} return false ;
    }
}
bool CPstBuff::CycleReady(void) //스트립 Ready.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
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

    if(Stat.bReqStop) {
        //Step.iCycle = 0;
        //return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiPSB_Align    , ccBwd );
                  MoveActr(aiPSB_Marking  , ccBwd );
                  MoveActr(aiPSB_Stopper  , ccFwd );
                  MoveActr(aiPSB_RjctOpen , ccFwd );
                  MoveActr(aiPSB_RjctUpDn , ccBwd );
                  if(OM.CmnOptn.bMrkAllSkip){
                      DM.ARAY[riPSB].SetStat(csWork);
                  }
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiPSB_Align    , ccBwd )) return false ;
                  if(!MoveActr(aiPSB_Marking  , ccBwd )) return false ;
                  if(!MoveActr(aiPSB_Stopper  , ccFwd )) return false ;
                  if(!MoveActr(aiPSB_RjctOpen , ccFwd )) return false ;
                  if(!MoveActr(aiPSB_RjctUpDn , ccBwd )) return false;
                  MoveMotr(miPSB_XMrk,piPSB_XMrkWait);
                  MoveMotr(miPSB_YMrk,piPSB_YMrkWait);
                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveMotr(miPSB_XMrk,piPSB_XMrkWait)) return false;
                  if(!MoveMotr(miPSB_YMrk,piPSB_YMrkWait)) return false;
                  MoveActr(aiPSB_Marking  , ccFwd);  // 0에 있어도 부딪힐건 없을꺼 같네요.
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiPSB_Marking    , ccFwd)) return false ;
                  Step.iCycle = 0;
                  return true ;


    }
}

bool CPstBuff::CycleBind(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiPSB_CycleTO);
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
     static bool bPSBSkip = false ;
    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPSB_Stopper , ccFwd);
                  MoveActr(aiPSB_Marking , ccBwd);
                  MoveActr(aiPSB_Align   , ccBwd);
                  bPSBSkip = OM.CmnOptn.bVsMrkSkip && OM.CmnOptn.bMrkAllSkip;
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPSB_Stopper , ccFwd)) return false ;
                  if(!MoveActr(aiPSB_Marking , ccBwd)) return false ;
                  if(!MoveActr(aiPSB_Align   , ccBwd)) return false ;
                  if(!bPSBSkip) MoveMotr(miPSB_XMrk , piPSB_XMrkWork); //비전 아니면 마킹 위치로 가있기.
                  if(!bPSBSkip) MoveMotr(miPSB_YMrk , piPSB_YMrkWork);
                  IO_SetY(yPSB_FeedingAC , true);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  12: if(!m_tmTemp.OnDelay(true , 300)) return false ;
                  MoveActr(aiPSB_Align , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiPSB_Align , ccFwd)) return false ;
                  Step.iCycle++ ;
                  return false ;

        case  14: IO_SetY(yPSB_FeedingAC , false);
                  if(!bPSBSkip&&!MoveMotr(miPSB_XMrk , piPSB_XMrkWork)) return false ;
                  if(!bPSBSkip&&!MoveMotr(miPSB_YMrk , piPSB_YMrkWork)) return false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}


//One Cycle.
bool CPstBuff::CycleWork(void) //Marking
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 15000 )) {
        EM_SetErr(eiPSB_CycleTO);
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

    if(Stat.bReqStop) {
    }
    bool bExstFail  = DM.ARAY[riPSB].GetCntStat(csFail  ) || DM.ARAY[riPSB].GetCntStat(csRslt0 ) ||
                      DM.ARAY[riPSB].GetCntStat(csRslt1 ) || DM.ARAY[riPSB].GetCntStat(csRslt2 ) ||
                      DM.ARAY[riPSB].GetCntStat(csRslt3 ) || DM.ARAY[riPSB].GetCntStat(csRslt4 ) ||
                      DM.ARAY[riPSB].GetCntStat(csRslt5 ) || DM.ARAY[riPSB].GetCntStat(csRslt6 ) ||
                      DM.ARAY[riPSB].GetCntStat(csRslt7 ) || DM.ARAY[riPSB].GetCntStat(csRslt8 ) ||
                      DM.ARAY[riPSB].GetCntStat(csRslt9 ) || DM.ARAY[riPSB].GetCntStat(csRslt10) ||
                      DM.ARAY[riPSB].GetCntStat(csRslt11) || DM.ARAY[riPSB].GetCntStat(csRslt12) ||
                      DM.ARAY[riPSB].GetCntStat(csRslt13) || DM.ARAY[riPSB].GetCntStat(csRslt14) ||
                      DM.ARAY[riPSB].GetCntStat(csRslt15) || DM.ARAY[riPSB].GetCntStat(csRslt16) ;

    bool r1 , r2 , r3 , r4 ;
    int  iCol , iRow ;
    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiPSB_Marking,ccBwd);
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiPSB_Marking ,ccBwd)) return false ;
                  MoveMotr(miPSB_XMrk , piPSB_XMrkWork);
                  MoveMotr(miPSB_YMrk , piPSB_YMrkWork);
                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveMotr(miPSB_XMrk , piPSB_XMrkWork)) return false ;
                  if(!MoveMotr(miPSB_YMrk , piPSB_YMrkWork)) return false ;
                  MoveActr(aiPSB_Marking ,ccFwd);
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiPSB_Marking , ccFwd)) return false ;
                  if(OM.CmnOptn.bUsePenMrkMvX) MT_GoIncMan(miPSB_XMrk , OM.CmnOptn.dPenMrkMvX) ;
                  Step.iCycle++;
                  return false ;

        case 14 : if(OM.CmnOptn.bUsePenMrkMvX) if(!MT_GetStopInpos(miPSB_XMrk)) return false ;
                  if(OM.CmnOptn.bUsePenMrkMvY) MT_GoIncMan(miPSB_YMrk , OM.CmnOptn.dPenMrkMvY) ;
                  Step.iCycle++;
                  return false ;

        case 15 : if(OM.CmnOptn.bUsePenMrkMvY) if(!MT_GetStopInpos(miPSB_YMrk)) return false ;
                  MoveActr(aiPSB_Marking ,ccBwd);
                  Step.iCycle++;
                  return false ;

        case 16 : if(!MoveActr(aiPSB_Marking ,ccBwd)) return false ;
                  FindChipMrk(iRow , iCol) ;
                  if(OM.CmnOptn.bVsMrkSkip){
                      DM.ARAY[riPSB].SetStat(iRow, iCol , csWork);
                  }
                  else {
                      DM.ARAY[riPSB].SetStat(iRow,iCol,csMarking);
                  }
                  Step.iCycle=0;
                  return true ;
    }
}

//One Cycle.
bool CPstBuff::CycleVisn(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiPSB_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        if(!OM.CmnOptn.bVsMrkSkip ) VC.SendReset(vsVisn4 ) ;

        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    int r,c ;

    int  iFailCnt ;
    bool r1 , r2 , r3 , r4 ;

    //메뉴얼 동작중에 에러 뜨면 메뉴얼이 중간에 멈춘다.
    bool bManualInsp = !Step.iSeq ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPSB_Marking , ccBwd);
                  Step.iCycle++;
                  return false ;


        case  11: if(!MoveActr(aiPSB_Marking , ccBwd)) return false ;
                  MoveMotr(miPSB_XMrk , piPSB_XMrkVisn);
                  MoveMotr(miPSB_YMrk , piPSB_YMrkVisn);
                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miPSB_XMrk , piPSB_XMrkVisn)) return false ;
                  if(!MoveMotr(miPSB_YMrk , piPSB_YMrkVisn)) return false ;
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!IO_GetX(xHED_4Ready)&&!OM.CmnOptn.bVsMrkSkip){
                      EM_SetErr(eiHED_Visn4NotReady) ;
                      Step.iCycle = 0 ;
                      return true;
                  }
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  14: IO_SetY(yHED_4Trigger , true);
                  if(!m_tmTemp.OnDelay(true , 100))return false;
                  IO_SetY(yHED_4Trigger , false);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  15: FindChipVsn(csMarking , r , c);
                  if(!m_tmTemp.OnDelay(true , OM.CmnOptn.dPSB_VisionTime))return false;
                  if( IO_GetX(xHED_4Result)) { DM.ARAY[riPSB].SetStat(r, c, csWork); }
                  if(!IO_GetX(xHED_4Result)) { DM.ARAY[riPSB].SetStat(r, c, csFail); }
                  if(DM.ARAY[riPSB].GetStat(r, c) == csMarking) return false;
                  Step.iCycle++;
                  return false;

        case  16: if(DM.ARAY[riPSB].GetCntStat(csFail)){
                      EM_SetErr(eiPSB_MarkingFail);
                  }
                  Step.iCycle = 0;
                  return true;
        }
}

bool CPstBuff::CycleRemove(void) //스트립 Ready.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
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

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(yPSB_FeedingAC , false);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  11: if(!m_tmTemp.OnDelay(true, 100))return false;
                  MoveActr(aiPSB_Stopper , ccBwd );
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveActr(aiPSB_Stopper , ccBwd))return false;
                  MoveActr(aiPSB_RjctUpDn , ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiPSB_RjctUpDn , ccFwd))return false;
                  MoveActr(aiPSB_RjctOpen , ccBwd);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveActr(aiPSB_RjctOpen , ccBwd)) return false;
                  MoveActr(aiPSB_RjctUpDn , ccBwd);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiPSB_RjctUpDn , ccBwd)) return false;
                  IO_SetY(yPSB_RejectMotor , true); //느리군.
                  MoveActr(aiPSB_RjctOpen , ccFwd);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveActr(aiPSB_RjctOpen , ccFwd)) return false;
                  if(!IO_GetX(xPSB_RejectDetect)) return false; //자재 이미 하나 빠져 있으면 감지되고 있는데...충분히 못빠지지 않나?
                  m_tmTemp.Clear();
                  MoveActr(aiPSB_Stopper , ccFwd);

                  LT.PSBInfo.iPSBStripCnt += 1;

                  iPSBGoodCavityCnt = DM.ARAY[riPSB].GetCntStat(csWork);
                  LT.PSBInfo.iPSBGoodCavity += iPSBGoodCavityCnt;

                  iPSBBadCavityCnt = (OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt) - iPSBGoodCavityCnt;
                  LT.PSBInfo.iPSBBadCavity += iPSBBadCavityCnt;

                  Step.iCycle++;
                  return false;

        case  17: if(!MoveActr(aiPSB_Stopper , ccFwd))return false;
                  if(!m_tmTemp.OnDelay(true , OM.CmnOptn.dPSBRejectDelay)) return false; //음 아닌가 생긴걸 잘 몰라서...그냥 무조건 에러 띠우는건데 이건...
                  if(IO_GetX(xPSB_RejectDetect)){
                      EM_SetErr(eiPSB_RejectFull);
                  }
                  IO_SetY(yPSB_RejectMotor , false);
                  LT.WritePSBData(riPSB);
                  DM.ARAY[riPSB].SetStat(csNone);
                  isRunWarn = true;
                  Step.iCycle++;
                  return false;

        case  18: if(ULD.GetLastStrip()){
                      DM.ARAY[riTRY].SetStep(1);
                  }

                  Step.iCycle = 0 ;
                  return true;
    }
}


bool CPstBuff::CycleVisnMM(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
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

    if(Stat.bReqStop) {
        //Step.iCycle = 0;
        //return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiPSB_Marking,ccBwd); // 혹여나 해서 여기 넣고 To Stop 에도 넣겟음. dd
                  FindChipVsn(csMarking , m_iReviewR , m_iReviewC);
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiPSB_Marking,ccBwd)) return false ;
                  Step.iCycle++;
                  return false ;

        case 12 : FindChipVsn(csMarking , m_iReviewR , m_iReviewC) ;
                  DM.ARAY[riVS4].SetStat(m_iReviewR,m_iReviewC,csWork);
                  MoveMotr(miPSB_XMrk , piPSB_XMrkVisn) ;
                  MoveMotr(miPSB_YMrk , piPSB_YMrkVisn) ;
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveMotr(miPSB_XMrk , piPSB_XMrkVisn)) return false ;
                  if(!MoveMotr(miPSB_YMrk , piPSB_YMrkVisn)) return false ;
                  Step.iCycle=0;
                  return true ;
    }
}

bool CPstBuff::CycleOut(void) //자제 아웃.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiPSB_CycleTO);
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

    if(Stat.bReqStop) {
        //Step.iCycle = 0 ;
        //return true ;
    }

    bool r1 , r2 , r3 , r4 ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiPSB_Align   , ccBwd);
                  MoveActr(aiPSB_Stopper , ccFwd);

                  if(AT_Complete(aiPSB_Marking , ccBwd)) MoveMotr(miPSB_XMrk,piPSB_XMrkWait);
                  if(AT_Complete(aiPSB_Marking , ccBwd)) MoveMotr(miPSB_YMrk,piPSB_YMrkWait);


                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiPSB_Align   , ccBwd)) return false ;
                  if(!MoveActr(aiPSB_Stopper , ccFwd)) return false ; //TODO :: ?? 오타 인가?
                  IO_SetY(yPSB_FeedingAC,true);
                  Step.iCycle++;
                  return false ;

        case 12 : MoveActr(aiPSB_Stopper   ,ccBwd);
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiPSB_Stopper   ,ccBwd)) return false ;

                  //스토퍼 다운되면 ...데이터 넘김.
                  //DM.ARAY[riPSB].SetStat(csNone) ; //TODO :: 시점이 안맞아서 옮길수 밖에 없엇음...틀리면 수정 요망.
                  //if(DM.ARAY[riULD].CheckAllStat(csNone)) {
                  //    DM.ARAY[riULD].SetLotNo(DM.ARAY[riPSB].GetLotNo());
                  //    DM.ARAY[riULD].SetID(DM.ARAY[riPSB].GetID());
                  //}
                  //DM.ARAY[riULD].SetStat(DM.ARAY[riULD].FindFrstRow(csEmpty),0 , csWork);
                  //DM.ARAY[riULD].SetStat(csWork);

                  Step.iCycle++;
                  return false ;


        case 14 : if(!IO_GetX(xPSB_Pkg) || OM.CmnOptn.bDryRun) MoveActr(aiPSB_Stopper , ccFwd) ;
                  if(!AT_GetCmd(aiPSB_Stopper)) return false ;


                  Step.iCycle++;
                  return false ;

        case 15 : if(!MoveMotr(miPSB_XMrk,piPSB_XMrkWait)) return false ;
                  if(!MoveMotr(miPSB_YMrk,piPSB_YMrkWait)) return false ;
                  if(!IO_GetX(xPSB_PkgOut)) return false ;
                  MoveActr(aiPSB_Marking , ccFwd);
//                  DM.ShiftArrayData(riLOT , riULD); //TODO :: ?? 시점에 문제가 있나본데 여기다 하다 못넘어가면 마스킹이 안되는데...
                  DM.ARAY[riPSB].SetStat(csNone) ;  //TODO :: 일루 옮김.
                  DM.ShiftArrayData(riLT1 , riULD ); //TODO :: ?? 시점에 문제가 있나본데 여기다 하다 못넘어가면 마스킹이 안되는데...
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case 16 : if(m_tmTemp.OnDelay(IO_GetX(xPSB_PkgOut) , OM.CmnOptn.iPsbToOutDelay)) {
                      EM_SetErr(eiPSB_OutStripExist);
                      //스토퍼 올라가기 시작 하면 레일에서 ::CycleMvAuto를 타고 있어서 데이터가 들어온다.
                      //무브 오토시에 레일을 끄면 끝까지 동작이 안됨.
                      if(!DM.ARAY[riPSB].GetCntExist() && RAL.GetSeqStep() != CRail::scMvAuto)IO_SetY(yPSB_FeedingAC,false);
                      Step.iCycle = 0;
                      return true ;
                  }
                  if(IO_GetX(xPSB_PkgOut)) return false ;

 /*
        case 15 : if(!MoveActr(aiPSB_Stopper ,ccFwd)) return false ;
                  if(m_tmTemp.OnDelay(true , OM.CmnOptn.iPsbToOutDelay)) {
                      EM_SetErr(eiPSB_OutStripExist);
                      IO_SetY(yWR2_FeedingAC,false);
                      return false ;
                  }

                  if(IO_GetX(xPSB_PkgOut4)||IO_GetX(xPSB_Pkg) )return false ;//Doking ...X
 */
                  //스토퍼 올라가기 시작 하면 레일에서 ::CycleMvAuto를 타고 있어서 데이터가 들어온다.
                  //무브 오토시에 레일을 끄면 끝까지 동작이 안됨.
                  if(!DM.ARAY[riPSB].GetCntExist() && RAL.GetSeqStep() != CRail::scMvAuto)IO_SetY(yPSB_FeedingAC,false);
                  Step.iCycle++;
                  return false ;

        case 17 :
                  Step.iCycle = 0;
                  return true ;
    }
}
bool CPstBuff::LotInfoAdd  ()
{
//    DM.ARAY[riLOT].ChangeStat(csUnkwn,csWork);
    LT.AddDayInfoWorkStrp(1);
    LT.AddDayInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);

    LT.AddLotInfoWorkStrp(1);
    LT.AddLotInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);

    LT.AddDayInfoFailChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - DM.ARAY[riULD].GetCntStat(csWork));
    LT.AddLotInfoFailChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - DM.ARAY[riULD].GetCntStat(csWork));
//    iVsFailCnt         = OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - DM.ARAY[riLOT].GetCntStat(csWork);
    OM.iTotalChip     += OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt ;
    OM.iTotalFailChip += OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - DM.ARAY[riULD].GetCntStat(csWork) ;

    if(OM.iTotalChip < 0 || OM.iTotalFailChip < 0) {//OverFlow
        OM.iTotalChip     = 0 ;
        OM.iTotalFailChip = 0 ;
    }
    LT.AddDayInfoChips(DM.ARAY[riULD].GetCntStat(csRslt1 ) , //"V Empty"
                       DM.ARAY[riULD].GetCntStat(csRslt2 ) , //"V Chip"
                       DM.ARAY[riULD].GetCntStat(csRslt3 ) , //"V Dust"
                       DM.ARAY[riULD].GetCntStat(csRslt4 ) , //"V LFlow"
                       DM.ARAY[riULD].GetCntStat(csRslt5 ) , //"V Broken"
                       DM.ARAY[riULD].GetCntStat(csRslt6 ) , //"V Crack"
                       DM.ARAY[riULD].GetCntStat(csRslt7 ) , //"V Burr"
                       DM.ARAY[riULD].GetCntStat(csRslt8 ) , //"V Zener"
                       DM.ARAY[riULD].GetCntStat(csRslt9 ) , //"V Wire"
                       DM.ARAY[riULD].GetCntStat(csRslt10) , //"V Orient"
                       DM.ARAY[riULD].GetCntStat(csRslt11) , //"V Pmeasure"
                       DM.ARAY[riULD].GetCntStat(csRslt12) , //"V Pdist"
                       DM.ARAY[riULD].GetCntStat(csRslt13) , //"V Match"
                       DM.ARAY[riULD].GetCntStat(csRslt14) , //"V Flow"
                       DM.ARAY[riULD].GetCntStat(csFail  ) ); //"Heat Fail");

    LT.AddLotInfoChips(DM.ARAY[riULD].GetCntStat(csRslt1 ) , //"V Empty"
                       DM.ARAY[riULD].GetCntStat(csRslt2 ) , //"V Chip"
                       DM.ARAY[riULD].GetCntStat(csRslt3 ) , //"V Dust"
                       DM.ARAY[riULD].GetCntStat(csRslt4 ) , //"V LFlow"
                       DM.ARAY[riULD].GetCntStat(csRslt5 ) , //"V Broken"
                       DM.ARAY[riULD].GetCntStat(csRslt6 ) , //"V Crack"
                       DM.ARAY[riULD].GetCntStat(csRslt7 ) , //"V Burr"
                       DM.ARAY[riULD].GetCntStat(csRslt8 ) , //"V Zener"
                       DM.ARAY[riULD].GetCntStat(csRslt9 ) , //"V Wire"
                       DM.ARAY[riULD].GetCntStat(csRslt10) , //"V Orient"
                       DM.ARAY[riULD].GetCntStat(csRslt11) , //"V Pmeasure"
                       DM.ARAY[riULD].GetCntStat(csRslt12) , //"V Pdist"
                       DM.ARAY[riULD].GetCntStat(csRslt13) , //"V Match"
                       DM.ARAY[riULD].GetCntStat(csRslt14) ,
                       DM.ARAY[riULD].GetCntStat(csFail  ) ); //"Heat Fail");//"V Flow"
                  //!DM.ARAY[riWK2].CheckAllStat(csNone) &&

    if(GetLastStrip()) {
        m_bLotEnded = true ;
        memcpy(&EndedLot , &LT.LotInfo , sizeof(CLot::SLotInfo));

        Trace("LotEnd",DM.ARAY[riULD].GetLotNo().c_str());
        LT.LotInfo.dEndTime = Now();
        LT.WriteLotLog   ();
        LT.WriteLotDayLog();
        Trace("WriteLotLog",DM.ARAY[riULD].GetLotNo().c_str());
        //LT.LotEnd();
    }
    DM.ARAY[riULD].SetStat(csNone) ;
}
bool CPstBuff::GetLastStrip()
{
    bool bLDRExist = !DM.ARAY[riLDR].CheckAllStat(csNone) ;
    bool bPRBExist = !DM.ARAY[riPRB].CheckAllStat(csNone) ;
    bool bWR1Exist = !DM.ARAY[riWR1].CheckAllStat(csNone) ;
    bool bWR2Exist = !DM.ARAY[riWR2].CheckAllStat(csNone) ;
    bool bWR3Exist = !DM.ARAY[riWR3].CheckAllStat(csNone) ;

    bool bLDRDiff  = DM.ARAY[riPSB].GetLotNo() != DM.ARAY[riLDR].GetLotNo() ;
    bool bPRBDiff  = DM.ARAY[riPSB].GetLotNo() != DM.ARAY[riPRB].GetLotNo() ;
    bool bWR1Diff  = DM.ARAY[riPSB].GetLotNo() != DM.ARAY[riWR1].GetLotNo() ;
    bool bWR2Diff  = DM.ARAY[riPSB].GetLotNo() != DM.ARAY[riWR2].GetLotNo() ;
    bool bWR3Diff  = DM.ARAY[riPSB].GetLotNo() != DM.ARAY[riWR3].GetLotNo() ;

    bool bLastMgz  = LDR._iMgzCnt >= OM.DevOptn.iLotEndMgzCnt ;

         if(bWR3Exist) return bWR3Diff ;
    else if(bWR2Exist) return bWR2Diff ;
    else if(bWR1Exist) return bWR1Diff ;
    else if(bPRBExist) return bPRBDiff ;
    else if(bLDRExist) return bLDRDiff ;
    else if(!bLastMgz) return false    ;
    else               return true     ;
}

void CPstBuff::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CPstBuff::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CPstBuff::CheckStop()
{
    if(!AT_Done(aiPSB_Align  )) return false ;
    if(!AT_Done(aiPSB_Marking)) return false ;
    if(!AT_Done(aiPSB_Stopper)) return false ;

    if(!MT_GetStop(miPSB_XMrk)) return false ;
    if(!MT_GetStop(miPSB_YMrk)) return false ;

    return true ;
}

void CPstBuff::Load(bool IsLoad )
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() +".INI";

    //Load Device.
    if(IsLoad) {
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_bLotEnded " , &m_bLotEnded  );
    }
    else {
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_bLotEnded " ,  m_bLotEnded  );
    }
}

















