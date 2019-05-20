//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Head.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "LotUnit.h"
//---------------------------------------------------------------------------
#include "RailDie.h"
#include "Stage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
void __stdcall HedZStripWorkWait(long _lAxis, DWORD _dwFlag)
{
//Tmfpemfmf gksk ehffueh ?

    MT_GoAbsRun(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedStWorkWait));

}
void __stdcall HedZWaferWorkWait(long _lAxis, DWORD _dwFlag)
{
//Tmfpemfmf gksk ehffueh ?

    MT_GoAbsRun(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWfWorkWait));

}
//void __stdcall HedZWaferWorkWait(long _lAxis, DWORD _dwFlag)
//{
////    MT_GoAbsRun(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWait));
//    MT_GoAbsRun(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWfWorkWait));
//}

CHead HED;

CHead::CHead()
{
    Init();
}
CHead::~CHead (void)
{
    Close();
}
void CHead::Init()
{
    m_sPartName   = "Head " ;
    m_dLastIdxPos = 0.0 ;
    iStgCheck     = hsNotWork ;

    iHomeTO  = (int)eiHED_HomeTO    ;
    iStartTO = (int)eiHED_ToStartTO ;
    iStopTO  = (int)eiHED_ToStopTO  ;
    iCycleTO = (int)eiHED_CycleTO   ;

    Reset();
    Load(true);
}
void CHead::Close()
{
    Load(false);
}
void CHead::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CHead::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();

}
/*
void __stdcall CHead::HedZStripWorkWait(long _lAxis, DWORD _dwFlag)
{
//Tmfpemfmf gksk ehffueh ?

    MT_GoAbsRun(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedStWorkWait));

}
void __stdcall CHead::HedZWaferWorkWait(long _lAxis, DWORD _dwFlag)
{
//Tmfpemfmf gksk ehffueh ?

    MT_GoAbsRun(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWfWorkWait));

}
*/

bool CHead::FindChip(EN_ARAY_ID _riWhre , int &r , int &c , EN_CHIP_STAT _stats )
{
//축에서 가까운쪽이 스타트 포지션으로 하기로 햇음.
    int r1,c1;
    if(_riWhre == riWRD) {
        if(_stats == csMask) { //Data Making dyd
            DM.ARAY[riWRD].FindLastColRow(csMask,r,c);
            return (r > -1 && c > -1);
        }
        else {
            c = DM.ARAY[riWRD].FindLastCol(csUnkwn);
            if(c % 2)DM.ARAY[riWRD].FindLastColRow    (csUnkwn , r1 , c1) ;
            else     DM.ARAY[riWRD].FindLastColFrstRow(csUnkwn , r1 , c1) ;
            r = r1 ;
            c = c1 ;

            return (r > -1 && c > -1);
        }
    }
    return false ;
}

double CHead::GetWorkPos(EN_MOTR_ID _iMotr)
{
    double dWorkPos = 0.0 ;
    int r  , c  ;

    if(_iMotr == miWRD_YHed){
        if(FindChip(riWRD,r,c)) {
            dWorkPos = PM.GetValue (_iMotr , pvWRD_YHedStripStt ) + ( OM.DevInfo.dRowPitch * r ) ;
        }
    }
    else {
        dWorkPos = MT_GetCmdPos(_iMotr);
    }

    return dWorkPos ;
}


double CHead::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;
    int r = -1 , c = -1 ;

    if(_iMotr == miWRD_XHed){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piWRD_XHedWait      : dPos = PM.GetValue (_iMotr , pvWRD_XHedWait      ); break ;
            case piWRD_XHedWafer     : dPos = PM.GetValue (_iMotr , pvWRD_XHedWafer     ); break ;
            case piWRD_XHedStrip     : dPos = PM.GetValue (_iMotr , pvWRD_XHedStrip     ); break ;
//            case piWRD_XHedWork      : dPos = PM.GetValue (_iMotr , pvWRD_XHedWork      ); break ;
        }
    }
    else if(_iMotr == miWRD_YHed){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piWRD_YHedWait      : dPos = PM.GetValue (_iMotr , pvWRD_YHedWait      ); break ;
            case piWRD_YHedWafer     : dPos = PM.GetValue (_iMotr , pvWRD_YHedWafer     ); break ;
            case piWRD_YHedWaferWait : dPos = PM.GetValue (_iMotr , pvWRD_YHedWaferWait ); break ;
            case piWRD_YHedWaferZMove: dPos = PM.GetValue (_iMotr , pvWRD_YHedWaferZMove); break ;
            case piWRD_YHedStripStt  : dPos = PM.GetValue (_iMotr , pvWRD_YHedStripStt  ); break ;
            case piWRD_YHedStripWait : dPos = PM.GetValue (_iMotr , pvWRD_YHedStripWait ); break ;
            case piWRD_YHedStripZMove: dPos = PM.GetValue (_iMotr , pvWRD_YHedStripZMove); break ;
            case piWRD_YHedWork      : dPos = GetWorkPos  (_iMotr                       ); break ;
        }
    }
    else if(_iMotr == miWRD_ZHed){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piWRD_ZHedWait      : dPos = PM.GetValue (_iMotr , pvWRD_ZHedWait      ); break ;
            case piWRD_ZHedWfWork    : dPos = PM.GetValue (_iMotr , pvWRD_ZHedWfWork    ); break ;
            case piWRD_ZHedStWork    : dPos = PM.GetValue (_iMotr , pvWRD_ZHedStWork    ); break ;
        }
    }
    else if(_iMotr == miWRD_SHed){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piWRD_SHedWait      : dPos = PM.GetValue (_iMotr , pvWRD_SHedWait      ); break ;
            case piWRD_SHedWorkStt   : dPos = PM.GetValue (_iMotr , pvWRD_SHedWorkStt   ); break ;

//            case piWRD_SHedStrip     : dPos = PM.GetValue (_iMotr , pvWRD_SHedStrip     ); break ;
//            case piWRD_SHedWork      : dPos = PM.GetValue (_iMotr , pvWRD_SHedWork      ); break ;
        }
    }
    else if(_iMotr == miWRD_ZEjt){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piWRD_ZEjtWait      : dPos = PM.GetValue (_iMotr , pvWRD_ZEjtWait      ); break ;
            case piWRD_ZEjtWorkStt   : dPos = PM.GetValue (_iMotr , pvWRD_ZEjtWorkStt   ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CHead::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool bHedZWaitPs = MT_GetStopInpos(miWRD_ZHed) && MT_CmprPos(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWait)) ;
    bool bStageStop  = MT_GetStopInpos(miWRD_XStg) && MT_GetStopInpos(miWRD_YStg) && MT_GetStopInpos(miWRD_SStg) && MT_GetStopInpos(miWRD_ZExd) ;
    bool bIdxStop    = MT_GetStopInpos(miWRD_XIdx) ;

    if(_iMotr == miWRD_XHed){
        if(!bHedZWaitPs  && !Step.iCycle) { sMsg = "HED_Z Axis is Not Wait Position" ; bRet = false ;}
        switch(_iPstnId) {
            default                : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRD_XHedWait    : if(!bHedZWaitPs) { sMsg = "HED_Z Axis is Not Wait Position" ; bRet = false ;} break ;
            case piWRD_XHedWafer   : break ;
            case piWRD_XHedStrip   : break ;
        }
    }
    else if(_iMotr == miWRD_YHed){
        if(!bHedZWaitPs && !Step.iCycle) { sMsg = "HED_Z Axis is Not Wait Position" ; bRet = false ;}
        switch(_iPstnId) {
            default                  : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRD_YHedWait      : if(!bHedZWaitPs) { sMsg = "HED_Z Axis is Not Wait Position" ; bRet = false ;} break ;
            case piWRD_YHedWafer     : break ;
            case piWRD_YHedWaferWait : break ;
            case piWRD_YHedWaferZMove: break ;
            case piWRD_YHedStripStt  : break ;
            case piWRD_YHedStripWait : break ;
            case piWRD_YHedStripZMove: break ;
            case piWRD_YHedWork      : break ;

        }
    }
    else if(_iMotr == miWRD_ZHed){
        switch(_iPstnId) {
            default                : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRD_ZHedWait    : break ;
            case piWRD_ZHedWfWork  : if(!bStageStop) { sMsg = "Stage Axis is Moving"       ; bRet = false ;} break ;
            case piWRD_ZHedStWork  : if(!bIdxStop  ) { sMsg = "WRD_X_Index Axis is Moving" ; bRet = false ;} break ;
        }
    }
    else if(_iMotr == miWRD_SHed){
        if(!bHedZWaitPs) { sMsg = "HED_Z Axis is Not Wait Position" ; bRet = false ;}
        switch(_iPstnId) {
            default                : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRD_SHedWait    : break ;
            case piWRD_SHedWorkStt : break ;
//            case piWRD_SHedWafer   : break ;
//            case piWRD_SHedStrip   : break ;
//            case piWRD_SHedWork      : break ;
        }
    }
    else if(_iMotr == miWRD_ZEjt){
        switch(_iPstnId) {
            default                : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRD_ZEjtWait    : break ;
            case piWRD_ZEjtWorkStt : break ;
        }
    }

    else {
        sMsg = MT_GetName(_iMotr) + " is Not this part Motr!" ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CHead::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

//

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}

bool CHead::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CHead::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CHead::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)iHomeTO);
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

        default: if(Step.iHome) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iHome = 0 ;
                 return true ;

        case 10: AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilMove + OM.MstOptn.dVoiceCoilOffset ) ;
                 MT_ResetIntrpt(miWRD_YHed);

                 MT_SetHomeEnd(miWRD_XHed,false);
                 MT_SetHomeEnd(miWRD_YHed,false);
                 MT_SetHomeEnd(miWRD_ZHed,false);
                 MT_SetHomeEnd(miWRD_SHed,false);
                 MT_SetHomeEnd(miWRD_ZEjt,false);

                 MT_Home(miWRD_ZHed);
                 MT_Home(miWRD_ZEjt);
                 Step.iHome++;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miWRD_ZHed)) return false ;
                 if(!MT_GetHomeEnd(miWRD_ZEjt)) return false ;
                 MT_DoHome(miWRD_XHed);
                 MT_DoHome(miWRD_YHed);
                 MT_DoHome(miWRD_SHed);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miWRD_XHed))return false ;
                 if(!MT_GetHomeEnd(miWRD_YHed))return false ;
                 if(!MT_GetHomeEnd(miWRD_SHed))return false ;
                 MT_GoAbsMan(miWRD_ZEjt , PM.GetValue(miWRD_ZEjt , pvWRD_ZEjtWait  ));
                 MT_GoAbsMan(miWRD_XHed , PM.GetValue(miWRD_XHed , pvWRD_XHedWait  ));
                 MT_GoAbsMan(miWRD_YHed , PM.GetValue(miWRD_YHed , pvWRD_YHedWait  ));
                 MT_GoAbsMan(miWRD_ZHed , PM.GetValue(miWRD_ZHed , pvWRD_ZHedWait  ));
                 MT_GoAbsMan(miWRD_SHed , PM.GetValue(miWRD_SHed , pvWRD_SHedWait  ));

                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miWRD_ZEjt , PM.GetValue(miWRD_ZEjt , pvWRD_ZEjtWait  ))) return false ;
                 if(!MT_GoAbsMan(miWRD_XHed , PM.GetValue(miWRD_XHed , pvWRD_XHedWait  ))) return false ;
                 if(!MT_GoAbsMan(miWRD_YHed , PM.GetValue(miWRD_YHed , pvWRD_YHedWait  ))) return false ;
                 if(!MT_GoAbsMan(miWRD_ZHed , PM.GetValue(miWRD_ZHed , pvWRD_ZHedWait  ))) return false ;
                 if(!MT_GoAbsMan(miWRD_SHed , PM.GetValue(miWRD_SHed , pvWRD_SHedWait  ))) return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CHead::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CHead::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CHead::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)iStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    static bool bChip = false ;
    //Move Home.
    switch (Step.iToStart) {
        default: if(Step.iToStart) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStart = 0 ;
                 return true ;

        case 10: bChip = DM.ARAY[riDIE].CheckStat(0,0,csUnkwn);
                 MT_ResetIntrpt(miWRD_YHed);
                 MoveMotr(miWRD_ZHed,piWRD_ZHedWait);
                 MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveMotr(miWRD_ZHed,piWRD_ZHedWait)) return false ;
                 if(!MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait)) return false ;
                 if(bChip) {
                     MoveMotr(miWRD_XHed,piWRD_XHedStrip    );
                     MoveMotr(miWRD_YHed,piWRD_YHedStripWait);
                 }
                 else {
                     MoveMotr(miWRD_XHed,piWRD_XHedWafer    );
                     MoveMotr(miWRD_YHed,piWRD_YHedWaferWait);
                 }
                 Step.iToStop ++;
                 return false ;

        case 12: if(bChip) {
                     if(!MoveMotr(miWRD_XHed,piWRD_XHedStrip    )) return false ;
                     if(!MoveMotr(miWRD_YHed,piWRD_YHedStripWait)) return false ;
                 }
                 else {
                     if(!MoveMotr(miWRD_XHed,piWRD_XHedWafer    )) return false ;
                     if(!MoveMotr(miWRD_YHed,piWRD_YHedStripWait)) return false ;
                 }
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CHead::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)iStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    Stat.bReqStop = false ;
    static bool bChip = false ;
    //Move Home.
    switch (Step.iToStop) {
        default: if(Step.iToStop) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStop=%02d" , __FUNC__ , Step.iToStop );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStop = 0;
                 return true ;

        case 10: bChip = DM.ARAY[riDIE].CheckStat(0,0,csUnkwn);
                 MT_ResetIntrpt(miWRD_YHed);
                 MoveMotr(miWRD_ZHed,piWRD_ZHedWait);
                 MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait);
                 Step.iToStop ++;
                 return false ;

        case 11: if(!MoveMotr(miWRD_ZHed,piWRD_ZHedWait)) return false ;
                 if(!MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait)) return false ;
                 if(bChip) {
                     MoveMotr(miWRD_XHed,piWRD_XHedStrip    );
                     MoveMotr(miWRD_YHed,piWRD_YHedStripWait);
                 }
                 else {
                     MoveMotr(miWRD_XHed,piWRD_XHedWafer    );
                     MoveMotr(miWRD_YHed,piWRD_YHedWaferWait);
                 }
                 Step.iToStop ++;
                 return false ;

        case 12: if(bChip) {
                     if(!MoveMotr(miWRD_XHed,piWRD_XHedStrip    )) return false ;
                     if(!MoveMotr(miWRD_YHed,piWRD_YHedStripWait)) return false ;
                 }
                 else {
                     if(!MoveMotr(miWRD_XHed,piWRD_XHedWafer    )) return false ;
                     if(!MoveMotr(miWRD_YHed,piWRD_YHedStripWait)) return false ;
                 }
                 Step.iToStop = 0   ;
                 return true ;
    }
}

bool CHead::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;
        //TODO :: 보이스 코일 위치.

//        bool isEJT_UpDn   =  AT_Complete(aiEJT_UpDn,ccFwd) ;

        bool bCheckIdxPos =  MT_CmprPos(miWRD_XIdx,RAD.GetWorkPos(miWRD_XIdx)) && DM.ARAY[riWRD].GetCntStat(csUnkwn) ;//&& isEJT_UpDn;
        bool bCheckStgPos =  MT_CmprPos(miWRD_XStg,RAD.GetWorkPos(miWRD_XStg)) &&
                             MT_CmprPos(miWRD_YStg,RAD.GetWorkPos(miWRD_YStg)) &&
                             MT_CmprPos(miWRD_SStg,RAD.GetWorkPos(miWRD_SStg)) && DM.ARAY[riSTG].GetCntStat(csUnkwn) ;//&& isEJT_UpDn;

        //TODO :: Pick sdfsjklfjafskldf
        bool isCyclePick  = !OM.MstOptn.bIgnrDieAttach && bCheckStgPos && DM.ARAY[riDIE].CheckStat(0,0,csNone ) ;
        bool isCyclePlace = !OM.MstOptn.bIgnrDieAttach && bCheckIdxPos && DM.ARAY[riDIE].CheckStat(0,0,csUnkwn) ;

        bool isConEnd     =  DM.ARAY[riWRD].CheckAllStat(csNone) && DM.ARAY[riDIE].CheckAllStat(csNone);


        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCyclePick  ) {Trace(m_sPartName.c_str(),"CyclePick   Stt"); Step.iSeq = scPick  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePlace ) {Trace(m_sPartName.c_str(),"CyclePlace  Stt"); Step.iSeq = scPlace ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd     ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default    End");Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                      return false ;
        case scPick  : if(CyclePick  ()){ Trace(m_sPartName.c_str(),"CyclePick  End");Step.iSeq = scIdle ;} return false ;
        case scPlace : if(CyclePlace ()){ Trace(m_sPartName.c_str(),"CyclePlace End");Step.iSeq = scIdle ;} return false ;
    }

}

//One Cycle.

bool CHead::CyclePick(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        MT_ResetIntrpt(miWRD_YHed);
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
    //    return true;
    //}

    bool r,c ;

    static double dPickWait = 0.0 ;
    static bool   bWrdMask  = false ;
    static double ZEjtWkBf  = 0.0 ;


    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;
                                         //TODO :: piWRD_ZHedWait 포지션 나중에 워크 웨이트로 바꺼랑.
        case   8: if(!MoveMotr(miWRD_ZHed,piWRD_ZHedWait )) return false ;
                  if(!MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait )) return false ;
                  MoveMotr(miWRD_XHed,piWRD_XHedWafer    );
                  MoveMotr(miWRD_YHed,piWRD_YHedWaferWait);
                  //보이스 코일
                  AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilMove + OM.MstOptn.dVoiceCoilOffset ) ;

                  Step.iCycle++;
                  return false;

        case   9: if(!MoveMotr(miWRD_XHed,piWRD_XHedWafer    )) return false ;
                  if(!MoveMotr(miWRD_YHed,piWRD_YHedWaferWait)) return false ;
                  Step.iCycle++;
                  return false;

        case  10: iStgCheck = hsNotWork ;
                  //CallBack Reset
                  MT_ResetIntrpt(miWRD_YHed);

                  MoveMotr(miWRD_ZHed,piWRD_ZHedWait );
                  MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait );
                  MoveMotr(miWRD_SHed,piWRD_SHedWorkStt);

                  dPickWait = PM.GetValue(miWRD_YHed,pvWRD_YHedStripWait) ;
                  bWrdMask  = DM.ARAY[riWRD].GetCntStat(csMask) ;
                  ZEjtWkBf  = PM.GetValue(miWRD_ZEjt,pvWRD_ZEjtWorkBefore) ;

                  IO_SetY(yHED_Eject ,false);
                  IO_SetY(yHED_Vaccum,false);


                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miWRD_ZHed,piWRD_ZHedWait )) return false ;
                  if(!MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait )) return false ;
                  if(!MoveMotr(miWRD_SHed,piWRD_SHedWorkStt)) return false ;

                  MoveMotr(miWRD_XHed,piWRD_XHedWafer);
                  MoveMotr(miWRD_YHed,piWRD_YHedWafer);

                  //CallBakc 지정.
                  MT_SetIntrptPos(miWRD_YHed , PM.GetValue(miWRD_YHed,pvWRD_YHedStripZMove) , HedZWaferWorkWait );

                  iStgCheck = hsGrabStt ; // -> STG
                  VSM.Grab(viWfr); // stg move .

                  if(!bWrdMask ) {
                      Step.iCycle = 15;
                      return false;
                  }

                  if(OM.CmnOptn.bIgnrRalAtVs ) {
                      if(bWrdMask) {
                          FindChip(riWRD,r,c,csMask) ;
                          DM.ARAY[riWRD].SetStat(r,c,csWork);
                          DM.ARAY[riRAD].SetStat(r,c,csWork);
                      }
                      Step.iCycle = 15;
                      return false;
                  }


//                  if(dPickWait <= MT_GetCmdPos(miWRD_YHed)) {
//                      if(MT_CmprPos(miWRD_YIns,RAD.GetWorkPos(miWRD_YIns)) && RAD._bInsYReady) VSM.Grab(viRalBf);  //이거 그립 딜레이 _bInsYReady
//                  }

                  Step.iCycle++;
                  return false;

        case  12: if(dPickWait <= MT_GetCmdPos(miWRD_YHed)) {
                      if(!MT_CmprPos(miWRD_YIns,RAD.GetWorkPos(miWRD_YIns)) && !RAD._bInsYReady) return false ;
                      VSM.Grab(viRalAt);  //이거 그립 딜레이 _bInsYReady
                      m_tmTemp.Clear() ;
                      Step.iCycle++;
                      return false ;
                  }
                  else return false ;

        case  13: if(m_tmTemp.OnDelay(!VSM.GetGrabEnd(viRalAt),OM.DevOptn.iRalAtVsGrabDly)) {
                      EM_SetErr(eiHED_RalAtVsGrabFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  if(!VSM.GetGrabEnd(viRalAt)) return false ;
                  VSM.Inspect(viRalAt); //쓰레드로 빼던가 나중에 ...
                  //비전에 따라 딜레이 밑으로 빼던가 다른데로 빼던가.
                  m_tmTemp.Clear() ;
                  Step.iCycle++;
                  return false ;

        case  14: if(m_tmTemp.OnDelay(!VSM.GetInspEnd(viRalAt),OM.DevOptn.iRalAtVsInspDly)) {
                      EM_SetErr(eiHED_RalAtVsInspFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  if(!VSM.GetInspEnd(viRalAt)) return false ;
                  VSM.GetRslt(viRalAt,&RsltAt);

                  if(!RsltAt.bRslt){
                      EM_SetErr(eiHED_RalAtVsInspFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  else{
                      if(DM.ARAY[riWRD].GetCntStat(csMask)) {
                          if(RsltAt.iRet = 0 ) {
                              RAD.FindChip(riWRD,r,c) ;
                              DM.ARAY[riWRD].SetStat(r,c,csWork);
                              DM.ARAY[riRAD].SetStat(r,c,csWork);
                          }
                          else if(RsltAt.iRet = 1 ) {
                              EM_SetErr(eiHED_RalAtVsThchFail);
                              Step.iCycle = 0 ;
                              return true ;
                          }
                          else if(RsltAt.iRet = 2 ) {
                              RAD.FindChip(riWRD,r,c) ;
                              DM.ARAY[riWRD].SetStat(r,c,csRslt2);
                              DM.ARAY[riRAD].SetStat(r,c,csWork);
                          }
                      }
                  }

                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveMotr(miWRD_XHed,piWRD_XHedWafer)) return false ;
                  if(!MoveMotr(miWRD_YHed,piWRD_YHedWafer)) return false ;

//                  if(!MoveMotr(miWRD_XHed,piWRD_XHedWafer)) return false ;
//                  if(!MoveMotr(miWRD_YHed,piWRD_YHedWafer)) return false ;

                  //스테이지와 연계 플레이 nn
                  if(iStgCheck == hsEmpty   ) {
                      Step.iCycle = 9;
                      return false   ;
                  }
                  else if( iStgCheck == hsWorkFail ){ //STG Fail
                      Step.iCycle = 0;
                      return true    ;
                  }
                  if(iStgCheck != hsWorkEnd ) return false ;
                  //Stg ghkrdlsdhkghkrdls

                  //TODO :: 보이스 코일. 이거 위치 조정 해야댐요.
                  AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilWafer + OM.MstOptn.dVoiceCoilOffset ) ;

                  MoveMotr(miWRD_ZHed,piWRD_ZHedWfWork);

                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miWRD_ZHed,piWRD_ZHedWfWork)) return false ;
                  IO_SetY(yHED_Vaccum,true);

                  //밑에서 치면서 보이스 코일 힘두 바꾸고 ㅇㄴㅇㄹ

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  17: if(!m_tmTemp.OnDelay(true,OM.DevOptn.iVacuumOnDly)) return false ;
                  if(!IO_GetX(xHED_Vaccum) && !OM.CmnOptn.bIgnrVcSsr) {
                      EM_SetErr(eiHED_VacuumSsr);
                      MoveMotr(miWRD_ZHed,piWRD_ZHedWait);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  //CallBakc 지정.
                  MT_SetIntrptPos(miWRD_ZEjt , PM.GetValue(miWRD_ZEjt,pvWRD_ZEjtWorkBefore) , HedZWaferWorkWait );
                  MoveMotr(miWRD_ZEjt,piWRD_ZEjtWorkStt);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

                  //Delay ///////////////넣기에는 너무 짧아. ? vel ??? time? dis ? scan time asdfjksdjfklshut
                  //Callback 있는데 한번더 해둠...안전빵임요
                  //TODO :: piWRD_ZHedWait ->  HedZWaferWorkWait
        case  18: if(ZEjtWkBf <= MT_GetEncPos(miWRD_ZEjt)) MoveMotr(miWRD_ZHed,piWRD_ZHedWait);
                  else return false ;

                  Step.iCycle++;
                  return false;

        case  19: if(!MoveMotr(miWRD_ZEjt,piWRD_ZEjtWorkStt)) return false ;
                  MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait);

                  Step.iCycle++;
                  return false;

        case  20: if(!MoveMotr(miWRD_ZHed,piWRD_ZHedWait   )) return false ;
                  if(!MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait   )) return false ;

                  DM.ARAY[riDIE].SetStat(0,0,csUnkwn);
                  STG.FindChip(riSTG,r,c);
                  DM.ARAY[riSTG].SetStat(r,c,csWork);

                  //TODO :: 보이스 코일. 이거 위치 조정 해야댐요.
                  AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilMove + OM.MstOptn.dVoiceCoilOffset ) ;

                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CHead::CyclePlace(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
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
    //    return true;
    //}

    bool r,c ;
    bool bCheckStgPos = false ;

    static double dPlaceWait = 0.0 ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case   9: if(!MoveMotr(miWRD_XHed,piWRD_XHedStrip    )) return false ;
                  if(!MoveMotr(miWRD_YHed,piWRD_YHedStripWait)) return false ;

                  Step.iCycle++;
                  return false;


        case  10: dPlaceWait = PM.GetValue(miWRD_YHed,pvWRD_YHedWaferWait) ;
                  MoveMotr(miWRD_ZHed,piWRD_ZHedWait );
                  MoveMotr(miWRD_SHed,piWRD_SHedWorkStt);
                  IO_SetY(yHED_Eject ,false);
                  IO_SetY(yHED_Vaccum,true );
                  //보이스 코일.
                  //TODO :: 보이스 코일. 이거 위치 조정 해야댐요.
                  AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilMove + OM.MstOptn.dVoiceCoilOffset ) ;

                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miWRD_ZHed,piWRD_ZHedWait )) return false ;
                  if(!MoveMotr(miWRD_SHed,piWRD_SHedWorkStt)) return false ;
                  if(!(MT_CmprPos(miWRD_YIns,RAD.GetWorkPos(miWRD_YIns)) && RAD._bInsYReady)) return false ; // 이거 위로 올려야 ... 할까?

                  VSM.Grab(viRalBf);  //이거 그립 딜레이 _bInsYReady
                  MoveMotr(miWRD_XHed,piWRD_XHedStrip);
                  MoveMotr(miWRD_YHed,piWRD_YHedWork );

                  //CallBakc 지정.
                  MT_SetIntrptPos(miWRD_YHed , PM.GetValue(miWRD_YHed,pvWRD_YHedStripZMove) , HedZStripWorkWait );

//                  if(!(MT_CmprPos(miWRD_YIns,RAD.GetWorkPos(miWRD_YIns)) && RAD._bInsYReady)) return false ; // 이거 위로 올려야 ... 할까?

                  m_tmTemp.Clear() ;
                  Step.iCycle++;
                  return false ;

        case  12: if(m_tmTemp.OnDelay(!VSM.GetGrabEnd(viRalBf),OM.DevOptn.iRalBfVsGrabDly)) {
                      EM_SetErr(eiHED_RalBfVsGrabFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  if(!VSM.GetGrabEnd(viRalBf)) return false ;
                  VSM.Inspect(viRalBf); //쓰레드로 빼던가 나중에 ...
                  m_tmTemp.Clear() ;
                  Step.iCycle++;
                  return false ;

        case  13: if(m_tmTemp.OnDelay(!VSM.GetInspEnd(viRalBf),OM.DevOptn.iRalBfVsInspDly)) {
                      EM_SetErr(eiHED_RalBfVsInspFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  if(!VSM.GetInspEnd(viRalBf)) return false ;
                  VSM.GetRslt(viRalBf,&RsltBf);

                  if(!RsltBf.bRslt){
                      EM_SetErr(eiHED_RalBfVsInspFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  //viRalBf  = 0:Pass , 1:LCP Fail , 2: Epoxy Fail
                  else {
                      if(RsltAt.iRet = 1 ) {
                          EM_SetErr(eiHED_RalBfVsThchFail);
                          Step.iCycle = 0 ;
                          return true ;
                      }
                      else if(RsltAt.iRet = 2 ) {
                          if(!OM.CmnOptn.bIgnrRalBfVsEpoxyFail ) {
                              EM_SetErr(eiHED_RalBfVsThchFail);
                              Step.iCycle = 0 ;
                              return true ;
                          }
                          else {
                              FindChip(riWRD,r,c) ;
                              DM.ARAY[riWRD].SetStat(r,c,csRslt2);
                              DM.ARAY[riRAD].SetStat(r,c,csWork);

                              MoveMotr(miWRD_XHed,piWRD_XHedStrip);
                              MoveMotr(miWRD_YHed,piWRD_YHedStripWait);
                              Step.iCycle = 9 ;
                              return true ;
                          }
                      }
                  }

                  if(!MT_GoAbsRun(miWRD_XHed,GetMotrPos(miWRD_XHed,piWRD_XHedWafer) - RsltBf.fRsltX )) return false ;
                  if(!MT_GoAbsRun(miWRD_XHed,GetMotrPos(miWRD_YHed,piWRD_YHedWafer) - RsltBf.fRsltY )) return false ;
                  if(!MT_GoAbsRun(miWRD_XHed,GetMotrPos(miWRD_SHed,piWRD_SHedWorkStt) + RsltBf.fRsltT )) return false ;

//                  if(!MoveMotr(miWRD_XHed,piWRD_XHedWafer)) return false ;
//                  if(!MoveMotr(miWRD_YHed,piWRD_YHedWafer)) return false ;

                  //보이스 코일.
                  //TODO :: 보이스 코일. 이거 위치 조정 해야댐요.
                  AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilStrip + OM.MstOptn.dVoiceCoilOffset ) ;

                  MoveMotr(miWRD_ZHed,piWRD_ZHedStWork);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miWRD_ZHed,piWRD_ZHedStWork)) return false ;
                  //밑에서 치면서 보이스 코일 힘두 바꾸고 ㅇㄴㅇㄹ
                  //TODO ::

                  IO_SetY(yHED_Vaccum,false);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  15: if(!m_tmTemp.OnDelay(true,OM.DevOptn.iVacuumOffDly)) return false ;

                  MoveMotr(miWRD_ZHed,piWRD_ZHedWait);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveMotr(miWRD_ZHed,piWRD_ZHedWait)) return false ;

                  DM.ARAY[riDIE].SetStat(0,0,csNone);
                  FindChip(riWRD,r,c);
                  DM.ARAY[riWRD].SetStat(r,c,csMask);
                  DM.ARAY[riRAD].SetStat(r,c,csMask);

                  //TODO :: 보이스 코일. 이거 위치 조정 해야댐요.
                  AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilMove + OM.MstOptn.dVoiceCoilOffset ) ;

                  Step.iCycle = 0 ;
                  return true ;
    }
}

void CHead::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CHead::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CHead::CheckStop()
{
    if(!MT_GetStop(miWRD_XHed)) return false ;
    if(!MT_GetStop(miWRD_YHed)) return false ;
    if(!MT_GetStop(miWRD_ZHed)) return false ;
    if(!MT_GetStop(miWRD_SHed)) return false ;
    if(!MT_GetStop(miWRD_ZEjt)) return false ;

    return true ;
}

void CHead::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() + ".INI";

    if ( IsLoad ) {
//        UserINI.Load(sPath, "Member" , "m_iHedCnt " , m_iHedCnt );
    }
    else {
        UserINI.ClearFile(sPath) ;
//        UserINI.Save(sPath, "Member" , "m_iHedCnt " , m_iHedCnt );
    }
}
//---------------------------------------------------------------------------
