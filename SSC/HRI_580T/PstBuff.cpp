//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "PstBuff.h"
#include "UnLoader.h"
#include "Rail.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "VisnComUnit.h"
#include "SPCUnit.h"
#include "UserIni.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CPstBuff  PSB;

CPstBuff::CPstBuff(void)
{
    m_sPartName = "Post Buff " ;
    m_iTrimErr  = 0 ;
    Reset();
    Load(true);
}

void CPstBuff::Reset()
{
    ResetTimer();

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
    Load(false);
//    Trace("","");
}

bool CPstBuff::FindChip(EN_CHIP_STAT _iStat ,  int &r , int &c ) //안씀.
{
    c = DM.ARAY[riPSB].FindFrstCol(_iStat);

    if(c % 2)DM.ARAY[riPSB].FindFrstColLastRow(_iStat , r , c) ;
    else     DM.ARAY[riPSB].FindFrstColRow    (_iStat , r , c) ;

    return (r > -1 && c > -1);
}

double CPstBuff::GetMotrPosTable(EN_MOTR_ID _iMotr , int _iRow , int _iCol)
{
    int    r , c ;
    double dStartYPos ;
    double dStartXPos ;
    double dYPos      ;
    double dXPos      ;

    r = _iRow ;//OM.DevOptn.iRowInspCnt ? _iRow/OM.DevOptn.iRowInspCnt * OM.DevOptn.iRowInspCnt : _iRow ;
    c = _iCol ;//OM.DevOptn.iColInspCnt ? _iCol/OM.DevOptn.iColInspCnt * OM.DevOptn.iColInspCnt : _iCol ;

    dStartYPos = PM.GetValue(_iMotr , pvPSB_YTrmWorkSttPs ) ;
    dStartXPos = PM.GetValue(_iMotr , pvPSB_XTrmWorkSttPs ) ;

    if(_iMotr != miPSB_YTrm && _iMotr != miPSB_XTrm) return MT_GetCmdPos(_iMotr) ;

    dXPos = dStartXPos + c * OM.DevInfo.dColPitch ;
    dYPos = dStartYPos + r * OM.DevInfo.dRowPitch ;

    if(OM.DevInfo.dColGrGap && OM.DevInfo.iColGrCnt) {
        dXPos = dXPos + (c / OM.DevInfo.iColGrCnt) * (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) ;
    }


    if(OM.DevInfo.dRowGrGap && OM.DevInfo.iRowGrCnt) {
        dYPos = dYPos + (r / OM.DevInfo.iRowGrCnt) * (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) ;
    }

    if     (_iMotr == miPSB_YTrm) return dYPos ;
    else if(_iMotr == miPSB_XTrm) return dXPos ;
}

double CPstBuff::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int    r  , c  ;
    int    gr , gc ;
    double dPos ;

    if(_iMotr == miPSB_XTrm){
        switch(_iPstnId) {
            case piPSB_XTrmRslt0   : FindChip(csRslt0   , r , c); break ;
            case piPSB_XTrmRslt1   : FindChip(csRslt1   , r , c); break ;
            case piPSB_XTrmRslt2   : FindChip(csRslt2   , r , c); break ;
            case piPSB_XTrmRslt3   : FindChip(csRslt3   , r , c); break ;
            case piPSB_XTrmRslt4   : FindChip(csRslt4   , r , c); break ;
            case piPSB_XTrmRslt5   : FindChip(csRslt5   , r , c); break ;
            case piPSB_XTrmRslt6   : FindChip(csRslt6   , r , c); break ;
            case piPSB_XTrmRslt7   : FindChip(csRslt7   , r , c); break ;
            case piPSB_XTrmRslt8   : FindChip(csRslt8   , r , c); break ;
            case piPSB_XTrmRslt9   : FindChip(csRslt9   , r , c); break ;
            case piPSB_XTrmRslt10  : FindChip(csRslt10  , r , c); break ;
            case piPSB_XTrmRslt11  : FindChip(csRslt11  , r , c); break ;
            case piPSB_XTrmRslt12  : FindChip(csRslt12  , r , c); break ;
            case piPSB_XTrmRslt13  : FindChip(csRslt13  , r , c); break ;
            case piPSB_XTrmRslt14  : FindChip(csRslt14  , r , c); break ;
            case piPSB_XTrmRslt15  : FindChip(csRslt15  , r , c); break ;
            case piPSB_XTrmRslt16  : FindChip(csRslt16  , r , c); break ;
            case piPSB_XTrmFail    : FindChip(csFail    , r , c); break ;
        }
    }
    else if(_iMotr == miPSB_YTrm){
        switch(_iPstnId) {
            case piPSB_YTrmRslt0   : FindChip(csRslt0   , r , c); break ;
            case piPSB_YTrmRslt1   : FindChip(csRslt1   , r , c); break ;
            case piPSB_YTrmRslt2   : FindChip(csRslt2   , r , c); break ;
            case piPSB_YTrmRslt3   : FindChip(csRslt3   , r , c); break ;
            case piPSB_YTrmRslt4   : FindChip(csRslt4   , r , c); break ;
            case piPSB_YTrmRslt5   : FindChip(csRslt5   , r , c); break ;
            case piPSB_YTrmRslt6   : FindChip(csRslt6   , r , c); break ;
            case piPSB_YTrmRslt7   : FindChip(csRslt7   , r , c); break ;
            case piPSB_YTrmRslt8   : FindChip(csRslt8   , r , c); break ;
            case piPSB_YTrmRslt9   : FindChip(csRslt9   , r , c); break ;
            case piPSB_YTrmRslt10  : FindChip(csRslt10  , r , c); break ;
            case piPSB_YTrmRslt11  : FindChip(csRslt11  , r , c); break ;
            case piPSB_YTrmRslt12  : FindChip(csRslt12  , r , c); break ;
            case piPSB_YTrmRslt13  : FindChip(csRslt13  , r , c); break ;
            case piPSB_YTrmRslt14  : FindChip(csRslt14  , r , c); break ;
            case piPSB_YTrmRslt15  : FindChip(csRslt15  , r , c); break ;
            case piPSB_YTrmRslt16  : FindChip(csRslt16  , r , c); break ;
            case piPSB_YTrmFail    : FindChip(csFail    , r , c); break ;
        }
    }
    //X , Y Pos Setting.
    double dXPos =  PM.GetValue(miPSB_XTrm , pvPSB_XTrmWorkSttPs) +
                    c * OM.DevInfo.dColPitch ;

    double dYPos =  PM.GetValue(miPSB_YTrm , pvPSB_YTrmWorkSttPs) +
                    r * OM.DevInfo.dRowPitch ;

    if( OM.DevInfo.iColGrCnt) {
        gc = c / OM.DevInfo.iColGrCnt ;
//        dXPos = dXPos + (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * c / OM.DevInfo.iColGrCnt ;
        dXPos = dXPos + (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * gc ;

    }

    if( OM.DevInfo.iRowGrCnt) {
        gr = r / OM.DevInfo.iRowGrCnt ;
//        dYPos = dYPos + (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * r / OM.DevInfo.iRowGrCnt;
        dYPos = dYPos + (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * gr;

    }










    if(_iMotr == miPSB_TSrt){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piPSB_TSrtWait    : dPos = PM.GetValue (_iMotr , pvPSB_TSrtWaitPs   ); break ;
            case piPSB_TSrtRjc0    : dPos = PM.GetValue (_iMotr , pvPSB_TSrtRjc0Ps   ); break ;
            case piPSB_TSrtRjc1    : dPos = PM.GetValue (_iMotr , pvPSB_TSrtRjc1Ps   ); break ;
            case piPSB_TSrtRjc2    : dPos = PM.GetValue (_iMotr , pvPSB_TSrtRjc2Ps   ); break ;
        }
    }

    else if(_iMotr == miPSB_XIns){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piPSB_XInsWait    : dPos = PM.GetValue (_iMotr , pvPSB_XInsWaitPs   ); break ;
            case piPSB_XInsInsrt   : dPos = PM.GetValue (_iMotr , pvPSB_XInsInsrtPs  ); break ;
            case piPSB_XInsOut     : dPos = PM.GetValue (_iMotr , pvPSB_XInsOutPs    ); break ;
        }
    }

    else if(_iMotr == miPSB_XTrm){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piPSB_XTrmWait    : dPos = PM.GetValue (_iMotr , pvPSB_XTrmWaitPs   ); break ;
            case piPSB_XTrmWorkStt : dPos = PM.GetValue (_iMotr , pvPSB_XTrmWorkSttPs); break ;
            case piPSB_XTrmCheck   : dPos = dXPos + PM.GetValue (_iMotr , pvPSB_XTrmCheckPs);
            case piPSB_XTrmRslt0   : dPos = dXPos ;                                     break ;
            case piPSB_XTrmRslt1   : dPos = dXPos ;                                     break ;
            case piPSB_XTrmRslt2   : dPos = dXPos ;                                     break ;
            case piPSB_XTrmRslt3   : dPos = dXPos ;                                     break ;
            case piPSB_XTrmRslt4   : dPos = dXPos ;                                     break ;
            case piPSB_XTrmRslt5   : dPos = dXPos ;                                     break ;
            case piPSB_XTrmRslt6   : dPos = dXPos ;                                     break ;
            case piPSB_XTrmRslt7   : dPos = dXPos ;                                     break ;
            case piPSB_XTrmRslt8   : dPos = dXPos ;                                     break ;
            case piPSB_XTrmRslt9   : dPos = dXPos ;                                     break ;
            case piPSB_XTrmRslt10  : dPos = dXPos ;                                     break ;
            case piPSB_XTrmRslt11  : dPos = dXPos ;                                     break ;
            case piPSB_XTrmRslt12  : dPos = dXPos ;                                     break ;
            case piPSB_XTrmRslt13  : dPos = dXPos ;                                     break ;
            case piPSB_XTrmRslt14  : dPos = dXPos ;                                     break ;
            case piPSB_XTrmRslt15  : dPos = dXPos ;                                     break ;
            case piPSB_XTrmRslt16  : dPos = dXPos ;                                     break ;
            case piPSB_XTrmFail    : dPos = dXPos ;                                     break ;
        }
    }

    else if(_iMotr == miPSB_YTrm){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piPSB_YTrmWait    : dPos = PM.GetValue (_iMotr , pvPSB_YTrmWaitPs   ); break ;
            case piPSB_YTrmWorkStt : dPos = PM.GetValue (_iMotr , pvPSB_YTrmWorkSttPs); break ;
//            case piPSB_YTrmCheck   : dPos = dYPos ;                                     break ;
            case piPSB_YTrmRslt0   : dPos = dYPos ;                                     break ;
            case piPSB_YTrmRslt1   : dPos = dYPos ;                                     break ;
            case piPSB_YTrmRslt2   : dPos = dYPos ;                                     break ;
            case piPSB_YTrmRslt3   : dPos = dYPos ;                                     break ;
            case piPSB_YTrmRslt4   : dPos = dYPos ;                                     break ;
            case piPSB_YTrmRslt5   : dPos = dYPos ;                                     break ;
            case piPSB_YTrmRslt6   : dPos = dYPos ;                                     break ;
            case piPSB_YTrmRslt7   : dPos = dYPos ;                                     break ;
            case piPSB_YTrmRslt8   : dPos = dYPos ;                                     break ;
            case piPSB_YTrmRslt9   : dPos = dYPos ;                                     break ;
            case piPSB_YTrmRslt10  : dPos = dYPos ;                                     break ;
            case piPSB_YTrmRslt11  : dPos = dYPos ;                                     break ;
            case piPSB_YTrmRslt12  : dPos = dYPos ;                                     break ;
            case piPSB_YTrmRslt13  : dPos = dYPos ;                                     break ;
            case piPSB_YTrmRslt14  : dPos = dYPos ;                                     break ;
            case piPSB_YTrmRslt15  : dPos = dYPos ;                                     break ;
            case piPSB_YTrmRslt16  : dPos = dYPos ;                                     break ;
            case piPSB_YTrmFail    : dPos = dYPos ;                                     break ;
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
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    if(_iMotr == miPSB_XIns){
        if(!AT_Complete(aiPSB_FlprCmp , ccBwd) && GetMotrPos(_iMotr , _iPstnId) > MT_GetCmdPos(_iMotr) ){ sMsg = AT_GetName(aiPSB_FlprCmp )+" is not Bwd" ; bRet = false ;}
    }
    else if(_iMotr == miPSB_YTrm){
        //if(!AT_Complete(aiPSB_Flpr    , ccFwd)){ sMsg = AT_GetName(aiPSB_FlprCmp )+" is not Fwd" ; bRet = false ;}
        if(!AT_Complete(aiPSB_Trimmer , ccBwd)){ sMsg = AT_GetName(aiPSB_Trimmer )+" is not Bwd" ; bRet = false ;}
    }
    else if(_iMotr == miPSB_XTrm){
        //if(!AT_Complete(aiPSB_Flpr    , ccFwd)){ sMsg = AT_GetName(aiPSB_FlprCmp )+" is not Fwd" ; bRet = false ;}
        if(!AT_Complete(aiPSB_Trimmer , ccBwd)){ sMsg = AT_GetName(aiPSB_Trimmer )+" is not Bwd" ; bRet = false ;}
    }
    else if(_iMotr == miPSB_TSrt){
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

    if(_iActr == aiPSB_Align){
    }
    else if(_iActr == aiPSB_Finger){
    }
    else if(_iActr == aiPSB_Flpr){
        double ddd = GetMotrPos(miPSB_XTrm,piPSB_XTrmWait) ;
        double sss = MT_GetEncPos(miPSB_XTrm);
        double xxx = MT_GetCmdPos(miPSB_XTrm);
        if(sss > ddd+0.1) { sMsg = MT_GetName(miPSB_XTrm)+" is not at Wait Position" ; bRet = false ;}
        if(sss > xxx+0.1) { sMsg = MT_GetName(miPSB_XTrm)+" is not at Wait Position" ; bRet = false ;}



        if(!MT_CmprPos(miPSB_XTrm,GetMotrPos(miPSB_XTrm,piPSB_XTrmWait))) { sMsg = MT_GetName(miPSB_XTrm)+" is not at Wait Position" ; bRet = false ;}
        //if(!MT_CmprPos(miPSB_XTrm,GetMotrPos(miPSB_XTrm,piPSB_XTrmWait))) { sMsg = MT_GetName(miPSB_XTrm)+" is not at Wait Position" ; bRet = false ;}
        if(!AT_Complete(aiPSB_FlprCmp , ccFwd)){ sMsg = AT_GetName(aiPSB_FlprCmp )+" is not Fwd" ; bRet = false ;}
    }
    else if(_iActr == aiPSB_FlprCmp){
    }
    else if(_iActr == aiPSB_PusherFB){
        //if(!AT_Complete(aiPSB_PusherUD , ccFwd)){ sMsg = AT_GetName(aiPSB_PusherUD )+" is not Fwd" ; bRet = false ;}
    }
    else if(_iActr == aiPSB_PusherUD){
        //if(!AT_Complete(aiPSB_PusherFB , ccBwd)){ sMsg = AT_GetName(aiPSB_PusherFB )+" is not Bwd" ; bRet = false ;}
    }
    else if(_iActr == aiPSB_Trimmer){
        if(!MT_GetStop(miPSB_XTrm)) { sMsg = MT_GetName(miPSB_XTrm)+" is Moving" ; bRet = false ;}
        if(!MT_GetStop(miPSB_YTrm)) { sMsg = MT_GetName(miPSB_YTrm)+" is Moving" ; bRet = false ;}
    }
    else {
        return false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}

bool CPstBuff::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(_iMotr == miPSB_XIns && _iPstnId == piPSB_XInsInsrt && MT_CmprPos(_iMotr , GetMotrPos(_iMotr , piPSB_XInsWait)) )
        return MT_GoAbs(_iMotr , dPosition , OM.DevOptn.dPSB_XInsVel);

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
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
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

        case 10: MT_Reset(miPSB_XTrm);
                 MT_Reset(miPSB_YTrm);
                 MT_Reset(miPSB_XIns);
                 MT_Reset(miPSB_TSrt);

                 MT_SetServo(miPSB_XTrm,true);
                 MT_SetServo(miPSB_YTrm,true);
                 MT_SetServo(miPSB_XIns,true);
                 MT_SetServo(miPSB_TSrt,true);

                 MoveActr(aiPSB_Align    , ccBwd);
                 MoveActr(aiPSB_Finger   , ccBwd);
                 MoveActr(aiPSB_FlprCmp  , ccFwd);
                 MoveActr(aiPSB_PusherFB , ccBwd);
                 MoveActr(aiPSB_Trimmer  , ccBwd);
                 Step.iHome++ ;
                 return false ;

        case 11: if(!MoveActr(aiPSB_Align    , ccBwd)) return false ;
                 if(!MoveActr(aiPSB_Finger   , ccBwd)) return false ;
                 if(!MoveActr(aiPSB_FlprCmp  , ccFwd)) return false ;
                 if(!MoveActr(aiPSB_PusherFB , ccBwd)) return false ;
                 if(!MoveActr(aiPSB_Trimmer  , ccBwd)) return false ;

                 MoveActr(aiPSB_PusherUD , ccBwd);
                 Step.iHome++ ;
                 return false ;


        case 12: if(!MoveActr(aiPSB_PusherUD , ccBwd)) return false ;

                 MT_DoHome(miPSB_XTrm);
                 MT_DoHome(miPSB_YTrm);
                 MT_DoHome(miPSB_XIns);
                 MT_DoHome(miPSB_TSrt);

                 Step.iHome++ ;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miPSB_XTrm)) return false ;
                 if(!MT_GetHomeEnd(miPSB_YTrm)) return false ;
                 if(!MT_GetHomeEnd(miPSB_XIns)) return false ;
                 if(!MT_GetHomeEnd(miPSB_TSrt)) return false ;

                 MT_GoAbsMan(miPSB_XTrm , GetMotrPos(miPSB_XTrm,piPSB_XTrmWait)) ;
                 MT_GoAbsMan(miPSB_YTrm , GetMotrPos(miPSB_YTrm,piPSB_YTrmWait)) ;
                 MT_GoAbsMan(miPSB_XIns , GetMotrPos(miPSB_XIns,piPSB_XInsWait)) ;
                 MT_GoAbsMan(miPSB_TSrt , GetMotrPos(miPSB_TSrt,piPSB_TSrtWait)) ;

                 Step.iHome++ ;
                 return false ;

        case 14: if(!MT_GoAbsMan(miPSB_XTrm , GetMotrPos(miPSB_XTrm,piPSB_XTrmWait)))return false ;
                 if(!MT_GoAbsMan(miPSB_YTrm , GetMotrPos(miPSB_YTrm,piPSB_YTrmWait)))return false ;
                 if(!MT_GoAbsMan(miPSB_XIns , GetMotrPos(miPSB_XIns,piPSB_XInsWait)))return false ;
                 if(!MT_GoAbsMan(miPSB_TSrt , GetMotrPos(miPSB_TSrt,piPSB_TSrtWait)))return false ;

                 MoveActr(aiPSB_Flpr , ccBwd);

                 Step.iHome++;
                 return false ;

        case 15: if(!MoveActr(aiPSB_Flpr , ccBwd)) return false ;
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

        case 10: MoveActr(aiPSB_Finger , ccBwd);
                 MoveActr(aiPSB_PusherFB , ccBwd);
                 MoveActr(aiPSB_PusherUD , ccBwd);

                 IO_SetY(yPSB_SortAir , true ) ; //장비 스타트 할때 에어 켜줌.
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiPSB_Finger , ccBwd)) return false ;
                 if(!MoveActr(aiPSB_PusherFB , ccBwd)) return false ;
                 if(!MoveActr(aiPSB_PusherUD , ccBwd)) return false ;

                 MoveMotr(miPSB_XIns , piPSB_XInsWait);
                 Step.iToStart ++ ;
                 return false ;

        case 12: if(!MoveMotr(miPSB_XIns , piPSB_XInsWait)) return false ;

                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CPstBuff::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiPSB_ToStopTO);

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

        case 10: MoveActr(aiPSB_Align , ccBwd);
                 IO_SetY(yPSB_SortAir , false ) ; //장비 멈출때 에어 꺼줌.
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MoveActr(aiPSB_Align , ccBwd)) return false ;
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
                          ( DM.ARAY[riPSB].GetCntStat(csRslt1 ) && !OM.MstOptn.bEmptyNoTrim ) ||
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

        bool isCycleReady = !DM.ARAY[riPSB].GetCntExist() &&(!AT_Complete(aiPSB_Flpr    , ccBwd)||
                                                             !AT_Complete(aiPSB_FlprCmp , ccBwd)||
                                                             !AT_Complete(aiPSB_Finger  , ccBwd)||
                                                             !AT_Complete(aiPSB_Align   , ccBwd)) &&
                                                              RAL.GetSeqStep() != CRail::scMvAuto;
        bool isCycleIn    = false ; //IO_GetX(xWK2_Out);  // DM.ARAY[riVS2].GetCntExist() &&!DM.ARAY[riVS2].GetCntStat(csUnkwn) &&
                            //!DM.ARAY[riPSB].GetCntExist() && RAL.GetSeqStep() != CRail::scMvAuto  ;

        bool isCycleFlip  = !DM.ARAY[riPSB].CheckAllStat(csNone) &&  isExstFail        && !AT_Complete(aiPSB_Flpr , ccFwd)   && RAL.GetSeqStep() != CRail::scMvAuto ;
        bool isCycleWork  = !DM.ARAY[riPSB].CheckAllStat(csNone) &&  isExstFail        &&  AT_Complete(aiPSB_Flpr , ccFwd)   ;
        bool isCycleOut   = !DM.ARAY[riPSB].CheckAllStat(csNone) && !isExstFail        &&  DM.ARAY[riULD].GetCntStat(csEmpty)&& RAL.GetSeqStep() != CRail::scMvAuto && ULD.GetSeqStep() == CUnLoader::scIdle ;
        bool isConEnd     =  DM.ARAY[riPSB].CheckAllStat(csNone) && !IO_GetX(xWK2_Out) ;

        if(isCycleFlip/*따진자제 에러 나서 못씀.AT_Complete(aiPSB_Flpr,ccBwd)*/) {
            if(!IO_GetX(xPSB_Pkg)&&DM.ARAY[riPSB].GetCntExist (      ))EM_SetErr(eiPSB_PkgDispr  );
            if( IO_GetX(xPSB_Pkg)&&DM.ARAY[riPSB].CheckAllStat(csNone))EM_SetErr(eiPSB_PkgUnknown);
        }


        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleReady) {Trace(m_sPartName.c_str(),"CycleReady Stt"); Step.iSeq = scReady; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCycleIn   ) {Trace(m_sPartName.c_str(),"CycleIn    Stt"); Step.iSeq = scIn   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleFlip ) {Trace(m_sPartName.c_str(),"CycleFlip  Stt"); Step.iSeq = scFlip ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork ) {Trace(m_sPartName.c_str(),"CycleWork  Stt"); Step.iSeq = scWork ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleOut  ) {Trace(m_sPartName.c_str(),"CycleOut   Stt"); Step.iSeq = scOut  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd    ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default      :              /*Trace(m_sPartName.c_str(),"default    End");Step.iSeq = scIdle ;*/  return false ;
        case  scReady: if(CycleReady()){Trace(m_sPartName.c_str(),"CycleReady End");Step.iSeq = scIdle ;} return false ;
        case  scIn   : if(CycleIn   ()){Trace(m_sPartName.c_str(),"CycleIn    End");Step.iSeq = scIdle ;} return false ;
        case  scFlip : if(CycleFlip ()){Trace(m_sPartName.c_str(),"CycleFlip  End");Step.iSeq = scIdle ;} return false ;
        case  scWork : if(CycleWork ()){Trace(m_sPartName.c_str(),"CycleWork  End");Step.iSeq = scIdle ;} return false ;
        case  scOut  : if(CycleOut  ()){Trace(m_sPartName.c_str(),"CycleOut   End");Step.iSeq = scIdle ;} return false ;
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

        case 10 : MoveMotr(miPSB_XTrm , piPSB_XTrmWait) ;
                  MoveMotr(miPSB_YTrm , piPSB_YTrmWait) ;
                  MoveActr(aiPSB_Align , ccBwd        );
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveMotr(miPSB_XTrm , piPSB_XTrmWait)) return false ;
                  if(!MoveMotr(miPSB_YTrm , piPSB_YTrmWait)) return false ;
                  if(!MoveActr(aiPSB_Align , ccBwd        )) return false ;
                  MoveActr(aiPSB_FlprCmp ,ccFwd);
                  MoveActr(aiPSB_Finger  ,ccBwd);
                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveActr(aiPSB_FlprCmp ,ccFwd)) return false ;
                  if(!MoveActr(aiPSB_Finger  ,ccBwd)) return false ;

                  MoveActr(aiPSB_Flpr , ccBwd) ;
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiPSB_Flpr , ccBwd)) return false ;
                  MoveActr(aiPSB_FlprCmp , ccBwd) ;
                  Step.iCycle++;
                  return false ;

        case 14 : if(!MoveActr(aiPSB_FlprCmp , ccBwd)) return false ;
                  Step.iCycle=0;
                  return true ;

    }
}
bool CPstBuff::CycleIn(void) //스트립이 들어옴.
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








        case 11 : if(IO_GetX(xWK2_Out) && !OM.CmnOptn.bDryRun) return false ;
                  MoveActr(aiPSB_FlprCmp ,ccFwd);
                  //IO_SetY(yWRK_Belt,false);
                  if(!MoveMotr(miPSB_XIns , piPSB_XInsWait)) return false ;
                  //DM.ARAY[riPSB].FlipY();   //플립할때 뒤집지 않고 들어 왔을때 뒤집어야 예외상황이 적다. 플립에 뒤집으면 트림하다 말고 장비 껐다 키면 데이터 뒤집힐수 있음.
                  if(OM.CmnOptn.bNoTrimFail)DM.ARAY[riPSB].SetStat(csWork) ;

                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveActr(aiPSB_FlprCmp  , ccFwd)) return false ;
                  MoveActr(aiPSB_FlprCmp ,ccBwd);
                  MoveActr(aiPSB_Finger  ,ccFwd);
                  //MoveActr(aiPSB_Align   ,ccFwd);
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiPSB_FlprCmp , ccBwd)) return false ;
                  if(!MoveActr(aiPSB_Finger  , ccFwd)) return false ;
                  //if(!MoveActr(aiPSB_Align   , ccFwd)) return false ;
                  MoveMotr(miPSB_XIns , piPSB_XInsInsrt);
                  Step.iCycle++;
                  return false ;

        case 14 : if(!MoveMotr(miPSB_XIns , piPSB_XInsInsrt)) return false ;
                  MoveMotr(miPSB_XIns , piPSB_XInsWait);
                  Step.iCycle++;
                  return false ;

        case 15 : if(!MoveMotr(miPSB_XIns , piPSB_XInsWait)) return false ;

                  Step.iCycle = 0;
                  return true ;
    }
}

//One Cycle.
bool CPstBuff::CycleFlip(void) //뒤집기.
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
        //Step.iCycle = 0;
        //return true ;
    }

    bool r1 , r2 , r3 , r4 ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiPSB_Finger , ccBwd) ;
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiPSB_Finger , ccBwd)) return false ;
                  MoveMotr(miPSB_XIns , piPSB_XInsWait) ;
                  Step.iCycle++;
                  return false ;



        case 12 : if(!MoveMotr(miPSB_XIns , piPSB_XInsWait)) return false ;
                  MoveActr(aiPSB_FlprCmp ,ccBwd);
                  MoveActr(aiPSB_Finger  ,ccFwd);
                  //MoveActr(aiPSB_Align   ,ccFwd);
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiPSB_FlprCmp , ccBwd)) return false ;
                  if(!MoveActr(aiPSB_Finger  , ccFwd)) return false ;
                  //if(!MoveActr(aiPSB_Align   , ccFwd)) return false ;
                  MoveMotr(miPSB_XIns , piPSB_XInsInsrt);
                  Step.iCycle++;
                  return false ;

        case 14 : if(!MoveMotr(miPSB_XIns , piPSB_XInsInsrt)) return false ;
                  MoveMotr(miPSB_XIns , piPSB_XInsWait);
                  MoveActr(aiPSB_Align , ccFwd);
                  Step.iCycle++;
                  return false ;

        case 15 : if(!MoveActr(aiPSB_Align ,ccFwd)) return false ;
                  MoveActr(aiPSB_FlprCmp ,ccFwd);
                  Step.iCycle++;
                  return false ;

        case 16 : if(!MoveActr(aiPSB_FlprCmp ,ccFwd)) return false ;
                  MoveActr(aiPSB_Align ,ccBwd);
                  Step.iCycle++;
                  return false ;

        case 17 : if(!MoveActr(aiPSB_Align ,ccBwd)) return false ;
                  MoveActr(aiPSB_Flpr ,ccFwd);
                  Step.iCycle++;
                  return false ;

        case 18 : if(!MoveActr(aiPSB_Flpr ,ccFwd)) return false ;
                  if(!MoveMotr(miPSB_XIns , piPSB_XInsWait)) return false ;
                  if(!IO_GetX(xPSB_Pkg)&&!OM.CmnOptn.bDryRun) {
                      //EM_SetErr(eiPSB_PkgDispr) ;
                      //MoveActr (aiPSB_Flpr    , ccBwd) ;
                      //MoveActr (aiPSB_FlprCmp , ccBwd) ;
                      //Step.iCycle = 0;
                      //return true ;
                  }
                  if(OM.MstOptn.bPSB_XYTrmHome) {
                      m_bIsHome = true;
                      MT_DoHome(miPSB_XTrm);
                      MT_DoHome(miPSB_YTrm);
                  }
                  Step.iCycle++;
                  return false ;

        case 19 : if(!MT_GetHomeEnd(miPSB_XTrm) && OM.MstOptn.bPSB_XYTrmHome ) return false ;
                  if(!MT_GetHomeEnd(miPSB_YTrm) && OM.MstOptn.bPSB_XYTrmHome ) return false ;
                  m_bIsHome = false ;

                  MoveActr(aiPSB_Finger , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 20 : if(!MoveActr(aiPSB_Finger , ccBwd)) return false ;
                  SPC.SaveTrimLog(riPSB,true);
                  Step.iCycle = 0;
                  return true ;
    }
}

//One Cycle.
bool CPstBuff::CycleWork(void) //트림.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 15000 )) {
        if(Step.iCycle == 13 && !IO_GetX(xPSB_Bin1)){EM_SetErr(eiPSB_NoRjBox1); Step.iCycle = 0 ; return true ;}
        if(Step.iCycle == 13 && !IO_GetX(xPSB_Bin2)){EM_SetErr(eiPSB_NoRjBox2); Step.iCycle = 0 ; return true ;}
        if(Step.iCycle == 13 && !IO_GetX(xPSB_Bin3)){EM_SetErr(eiPSB_NoRjBox3); Step.iCycle = 0 ; return true ;}

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
        //Step.iCycle = 0;
        //return true ;
    }

    bool r1 , r2 , r3 , r4 ;


    EN_PSTN_ID   piFindPosX , piFindPosY , piRjctSrtT;
    EN_CHIP_STAT csFindChip ;
    int          iCol , iRow ;                                                                                                  //1,2,3 bin
         if(DM.ARAY[riPSB].GetCntStat(csRslt0 )) {piFindPosX = piPSB_XTrmRslt0  ; piFindPosY = piPSB_YTrmRslt0  ; piRjctSrtT = piPSB_TSrtRjc0 - 1 + (EN_PSTN_ID)1                       ; csFindChip = csRslt0  ;}
    else if(DM.ARAY[riPSB].GetCntStat(csRslt1 ) && !OM.MstOptn.bEmptyNoTrim ) {piFindPosX = piPSB_XTrmRslt1  ; piFindPosY = piPSB_YTrmRslt1  ; piRjctSrtT = piPSB_TSrtRjc0 - 1 + (EN_PSTN_ID)OM.CmnOptn.iSortItRslt1 ; csFindChip = csRslt1  ;}
    else if(DM.ARAY[riPSB].GetCntStat(csRslt2 )) {piFindPosX = piPSB_XTrmRslt2  ; piFindPosY = piPSB_YTrmRslt2  ; piRjctSrtT = piPSB_TSrtRjc0 - 1 + (EN_PSTN_ID)OM.CmnOptn.iSortItRslt2 ; csFindChip = csRslt2  ;}
    else if(DM.ARAY[riPSB].GetCntStat(csRslt3 )) {piFindPosX = piPSB_XTrmRslt3  ; piFindPosY = piPSB_YTrmRslt3  ; piRjctSrtT = piPSB_TSrtRjc0 - 1 + (EN_PSTN_ID)OM.CmnOptn.iSortItRslt3 ; csFindChip = csRslt3  ;}
    else if(DM.ARAY[riPSB].GetCntStat(csRslt4 )) {piFindPosX = piPSB_XTrmRslt4  ; piFindPosY = piPSB_YTrmRslt4  ; piRjctSrtT = piPSB_TSrtRjc0 - 1 + (EN_PSTN_ID)OM.CmnOptn.iSortItRslt4 ; csFindChip = csRslt4  ;}
    else if(DM.ARAY[riPSB].GetCntStat(csRslt5 )) {piFindPosX = piPSB_XTrmRslt5  ; piFindPosY = piPSB_YTrmRslt5  ; piRjctSrtT = piPSB_TSrtRjc0 - 1 + (EN_PSTN_ID)OM.CmnOptn.iSortItRslt5 ; csFindChip = csRslt5  ;}
    else if(DM.ARAY[riPSB].GetCntStat(csRslt6 )) {piFindPosX = piPSB_XTrmRslt6  ; piFindPosY = piPSB_YTrmRslt6  ; piRjctSrtT = piPSB_TSrtRjc0 - 1 + (EN_PSTN_ID)OM.CmnOptn.iSortItRslt6 ; csFindChip = csRslt6  ;}
    else if(DM.ARAY[riPSB].GetCntStat(csRslt7 )) {piFindPosX = piPSB_XTrmRslt7  ; piFindPosY = piPSB_YTrmRslt7  ; piRjctSrtT = piPSB_TSrtRjc0 - 1 + (EN_PSTN_ID)OM.CmnOptn.iSortItRslt7 ; csFindChip = csRslt7  ;}
    else if(DM.ARAY[riPSB].GetCntStat(csRslt8 )) {piFindPosX = piPSB_XTrmRslt8  ; piFindPosY = piPSB_YTrmRslt8  ; piRjctSrtT = piPSB_TSrtRjc0 - 1 + (EN_PSTN_ID)OM.CmnOptn.iSortItRslt8 ; csFindChip = csRslt8  ;}
    else if(DM.ARAY[riPSB].GetCntStat(csRslt9 )) {piFindPosX = piPSB_XTrmRslt9  ; piFindPosY = piPSB_YTrmRslt9  ; piRjctSrtT = piPSB_TSrtRjc0 - 1 + (EN_PSTN_ID)OM.CmnOptn.iSortItRslt9 ; csFindChip = csRslt9  ;}
    else if(DM.ARAY[riPSB].GetCntStat(csRslt10)) {piFindPosX = piPSB_XTrmRslt10 ; piFindPosY = piPSB_YTrmRslt10 ; piRjctSrtT = piPSB_TSrtRjc0 - 1 + (EN_PSTN_ID)OM.CmnOptn.iSortItRslt10; csFindChip = csRslt10 ;}
    else if(DM.ARAY[riPSB].GetCntStat(csRslt11)) {piFindPosX = piPSB_XTrmRslt11 ; piFindPosY = piPSB_YTrmRslt11 ; piRjctSrtT = piPSB_TSrtRjc0 - 1 + (EN_PSTN_ID)OM.CmnOptn.iSortItRslt11; csFindChip = csRslt11 ;}
    else if(DM.ARAY[riPSB].GetCntStat(csRslt12)) {piFindPosX = piPSB_XTrmRslt12 ; piFindPosY = piPSB_YTrmRslt12 ; piRjctSrtT = piPSB_TSrtRjc0 - 1 + (EN_PSTN_ID)OM.CmnOptn.iSortItRslt12; csFindChip = csRslt12 ;}
    else if(DM.ARAY[riPSB].GetCntStat(csRslt13)) {piFindPosX = piPSB_XTrmRslt13 ; piFindPosY = piPSB_YTrmRslt13 ; piRjctSrtT = piPSB_TSrtRjc0 - 1 + (EN_PSTN_ID)OM.CmnOptn.iSortItRslt13; csFindChip = csRslt13 ;}
    else if(DM.ARAY[riPSB].GetCntStat(csRslt14)) {piFindPosX = piPSB_XTrmRslt14 ; piFindPosY = piPSB_YTrmRslt14 ; piRjctSrtT = piPSB_TSrtRjc0 - 1 + (EN_PSTN_ID)OM.CmnOptn.iSortItRslt14; csFindChip = csRslt14 ;}
    else if(DM.ARAY[riPSB].GetCntStat(csRslt15)) {piFindPosX = piPSB_XTrmRslt15 ; piFindPosY = piPSB_YTrmRslt15 ; piRjctSrtT = piPSB_TSrtRjc0 - 1 + (EN_PSTN_ID)1                       ; csFindChip = csRslt15 ;}
    else if(DM.ARAY[riPSB].GetCntStat(csRslt16)) {piFindPosX = piPSB_XTrmRslt16 ; piFindPosY = piPSB_YTrmRslt16 ; piRjctSrtT = piPSB_TSrtRjc0 - 1 + (EN_PSTN_ID)1                       ; csFindChip = csRslt16 ;}
    else if(DM.ARAY[riPSB].GetCntStat(csFail  )) {piFindPosX = piPSB_XTrmFail   ; piFindPosY = piPSB_YTrmFail   ; piRjctSrtT = piPSB_TSrtRjc0 - 1 + (EN_PSTN_ID)OM.CmnOptn.iSortItFail  ; csFindChip = csFail   ;}
    else                                         {Step.iCycle = 0; return true  ;                                                                                                                }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : IO_SetY(yPSB_SortAir , true ) ; //전에꺼 혹시 남은 자제 있을수도 있어서 미리 불어줌.

                  if(AT_Complete(aiPSB_Flpr,ccBwd) && !IO_GetX(xPSB_Pkg)&&!OM.CmnOptn.bDryRun) {
                      EM_SetErr(eiPSB_PkgDispr) ;
                      Step.iCycle = 0;
                      return true ;
                  }

                  MoveActr(aiPSB_Trimmer ,ccBwd);
                  Step.iCycle++;
                  return false ;


        case 11 : if(!MoveActr(aiPSB_Trimmer ,ccBwd)) return false ;
                  MoveMotr(miPSB_XTrm , piFindPosX);
                  MoveMotr(miPSB_YTrm , piFindPosY);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case 12 : if(!MT_CmprPos      (miPSB_TSrt,GetMotrPos(miPSB_TSrt , piRjctSrtT))&&
                     !m_tmTemp.OnDelay(true , OM.CmnOptn.iSrtMotrDelay               )) return false ;

                  IO_SetY(yPSB_SortAir , false ) ; //미리 불어준거 꺼줌.

                  MoveMotr(miPSB_TSrt , piRjctSrtT);
                  Step.iCycle++;
                  return false ;

        //스텝 바꿀때 주의 위에서 씀.
        case 13 : if(!MoveMotr(miPSB_XTrm , piFindPosX)) return false ;
                  if(!MoveMotr(miPSB_YTrm , piFindPosY)) return false ;
                  if(!MoveMotr(miPSB_TSrt , piRjctSrtT)) return false ;
                  if(!IO_GetX(xPSB_Bin1)) {return false ; }
                  if(!IO_GetX(xPSB_Bin2)) {return false ; }
                  if(!IO_GetX(xPSB_Bin3)) {return false ; }
                  MoveActr(aiPSB_Trimmer ,ccFwd);

                  Step.iCycle++;
                  return false ;

        case 14 : if(!MoveActr(aiPSB_Trimmer ,ccFwd)) return false ;
                  MoveActr(aiPSB_Trimmer ,ccBwd);
                  Step.iCycle++;
                  return false ;

        case 15 : if(!MoveActr(aiPSB_Trimmer ,ccBwd)) return false ;
                  IO_SetY(yPSB_SortAir , true ) ; //자제 호스 까지 떨어지고  에어 불어줌.
                  if(OM.CmnOptn.bTrimCheck) MT_GoIncRun(miPSB_XTrm,PM.GetValue(miPSB_XTrm,pvPSB_XTrmCheckPs));
                  Step.iCycle++;
                  return false ;

        case 16 : if(OM.CmnOptn.bTrimCheck) { if(!MT_GetStopInpos(miPSB_XTrm) ) return false ; }
                  if(OM.CmnOptn.bTrimCheck) { if( IO_GetX(xPSB_TrimCheck,true) ) { EM_SetErr(eiPSB_TrimCheck); m_iTrimErr++; Step.iCycle = 0; return true ; } }
                  FindChip(csFindChip , iRow , iCol) ;
                  DM.ARAY[riPSB].SetStat(iRow,iCol,csEmpty) ;
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

        case 10 : MoveActr(aiPSB_FlprCmp , ccFwd          );
                  MoveMotr(miPSB_XIns    , piPSB_XInsWait );
                  MoveActr(aiPSB_PusherFB, ccBwd          );
                  MoveMotr(miPSB_XTrm , piPSB_XTrmWait);
                  MoveMotr(miPSB_YTrm , piPSB_YTrmWait);
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiPSB_FlprCmp , ccFwd          )) return false ;
                  if(!MoveMotr(miPSB_XIns    , piPSB_XInsWait )) return false ;
                  if(!MoveActr(aiPSB_PusherFB, ccBwd          )) return false ;
                  if(!MoveMotr(miPSB_XTrm , piPSB_XTrmWait)    ) return false ;
                  if(!MoveMotr(miPSB_YTrm , piPSB_YTrmWait)    ) return false ;

                  MoveActr(aiPSB_Align    ,ccBwd);

                  MoveActr(aiPSB_PusherUD ,ccBwd);
                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveActr(aiPSB_Align    ,ccBwd)) return false ;
                  if(!MoveActr(aiPSB_PusherUD ,ccBwd)) return false ;
                  MoveActr(aiPSB_Finger   ,ccFwd);
                  MoveActr(aiPSB_Flpr ,ccBwd);
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiPSB_Finger   ,ccFwd)) return false ;
                  if(!MoveActr(aiPSB_Flpr ,ccBwd)) return false ;
                  if(!IO_GetX(xPSB_Pkg)&&DM.ARAY[riPSB].GetCntExist ( )){
                      EM_SetErr(eiPSB_PkgDispr  );
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  MoveActr(aiPSB_FlprCmp ,ccBwd);
                  Step.iCycle++;
                  return false ;

        case 14 : if(!MoveActr(aiPSB_FlprCmp ,ccBwd)) return false ;
                  if(OM.EqpOptn.bExistUnLoader) {if(!ULD.GetStat().bReadyMgz) return false ;}
                  else                          {/*인라인 장비용 다음장비 레디 시그널*/     }

                  MoveMotr(miPSB_XIns , piPSB_XInsOut);

                  Step.iCycle++;
                  return false ;

        case 15 : if(!MoveMotr(miPSB_XIns , piPSB_XInsOut)) return false ;





                  MoveActr(aiPSB_PusherUD,ccFwd);
                  MoveActr(aiPSB_Finger  ,ccBwd);
                  Step.iCycle++;
                  return false ;

        case 16 : if(!MoveActr(aiPSB_PusherUD,ccFwd)) return false ;
                  if(!MoveActr(aiPSB_Finger  ,ccBwd)) return false ;
                  MoveMotr(miPSB_XIns    , piPSB_XInsWait) ;
                  MoveActr(aiPSB_PusherFB, ccFwd         ) ;
                  Step.iCycle++;
                  return false ;

        case 17 :
                  if(!MoveActr(aiPSB_PusherFB, ccFwd         )) return false ;

                  if( IO_GetX(xPSB_PkgOut1) || IO_GetX(xPSB_PkgOut2)) {
                      EM_SetErr(eiPSB_PusherMiss);
                      MoveActr(aiPSB_PusherUD,ccBwd);
                      MoveActr(aiPSB_PusherFB,ccBwd);
                      return false ;
                  }

                  if(DM.ARAY[riULD].CheckAllStat(csEmpty)) DM.ARAY[riULD].SetLotNo(DM.ARAY[riPSB].GetLotNo());
                  DM.ARAY[riPSB].FlipY();
                  VC.WriteLogMap(riPSB);
                  LT.WriteArayData(riPSB);
                  SPC.SaveTrimLog(riPSB,false,m_iTrimErr);
                  m_iTrimErr = 0 ;
                  DM.ARAY[riPSB].SetStat(csNone);
                  DM.ARAY[riULD].SetStat(DM.ARAY[riULD].FindFrstRow(csEmpty),0 , csWork);
//
//                  LT.AddDayInfoWorkStrp(1);
//                  LT.AddDayInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);

//                  LT.AddLotInfoWorkStrp(1);
//                  LT.AddLotInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);
//
                  MoveActr(aiPSB_PusherFB, ccBwd );
                  Step.iCycle++;
                  return false ;

        case 18 : if(!MoveActr(aiPSB_PusherFB,ccBwd)) return false ;
                  MoveActr(aiPSB_PusherUD, ccBwd );

                  Step.iCycle++;
                  return false ;

        case 19 : if(!MoveActr(aiPSB_PusherFB, ccBwd )) return false ;
                  if(!MoveActr(aiPSB_PusherUD, ccBwd )) return false ;
                  if(!MoveMotr(miPSB_XIns    , piPSB_XInsWait)) return false ;


                  Step.iCycle = 0;
                  return true ;
    }
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
    if(!AT_Done(aiPSB_Align   )) return false ;
    if(!AT_Done(aiPSB_Finger  )) return false ;
    if(!AT_Done(aiPSB_Flpr    )) return false ;
    if(!AT_Done(aiPSB_FlprCmp )) return false ;
    if(!AT_Done(aiPSB_PusherFB)) return false ;
    if(!AT_Done(aiPSB_PusherUD)) return false ;
    if(!AT_Done(aiPSB_Trimmer )) return false ;

    if(!MT_GetStop(miPSB_TSrt)) return false ;
    if(!MT_GetStop(miPSB_XIns)) return false ;
    if(!MT_GetStop(miPSB_XTrm)) return false ;
    if(!MT_GetStop(miPSB_YTrm)) return false ;

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
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_iTrimErr" , m_iTrimErr  );
    }
    else {
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_iTrimErr" , m_iTrimErr  );
    }
}

















