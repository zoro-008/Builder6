//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Head.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"

#include "SAInterface.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "LotUnit.h"
//---------------------------------------------------------------------------
#include "RailDie.h"
#include "Stage.h"
#include "ManualMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define VOICECOIL_PS_OFFSET_Y 2.0
#define VOICECOIL_PS_OFFSET_Z 1.0

const int bUseBackRash = false ;

//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
void __stdcall CHead::CallAxisZWaferPs(long _lAxis, DWORD _dwFlag) {
    MT_GoAbsRun(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWfWorkWait));
}
void __stdcall CHead::CallAxisZWaferVc(long _lAxis, DWORD _dwFlag) {
    AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilWafer) ; //보이스 코일
}
void __stdcall CHead::CallVisnAtGrib(long _lAxis, DWORD _dwFlag) {
    SV_Grab(viRat);
}


void __stdcall CHead::CallAxisZStripPs(long _lAxis, DWORD _dwFlag) {
    MT_GoAbsRun(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedMove ));
}
void __stdcall CHead::CallAxisYWaferPs(long _lAxis, DWORD _dwFlag) {
    MT_GoAbsRun(miWRD_YHed,PM.GetValue(miWRD_YHed,pvWRD_YHedWaferZMove));
}

double CHead::GetVoiceCoilVolt(double _dGram)
{
    double dRet = OM.MstOptn.dVoiceCoilA * _dGram + OM.MstOptn.dVoiceCoilB ;

    if(dRet >  5.0 ) dRet = 0.0 ;
    if(dRet < -5.0 ) dRet = 0.0 ;

    return dRet ;
}









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
void __stdcall CHead::HedZWaitPick(long _lAxis, DWORD _dwFlag)
{
    if(!MT_CmprPos(miWRD_ZHed , PM.GetValue(miWRD_ZHed,pvWRD_ZHedWait))) {
        MT_EmgStop(miWRD_YHed);
        EM_SetErr(eiHED_MotorYZCrash);
        HED.Step.iCycle = 0 ; //푸하하하...;;;;;
    }
    else {
        MT_SetIntrptPos(miWRD_YHed , PM.GetValue(miWRD_YHed,pvWRD_YHedWaferZMove) , HedZWaferWorkWait );
    }
}
void __stdcall CHead::HedZWaitPlace(long _lAxis, DWORD _dwFlag)
{
    if(!MT_CmprPos(miWRD_ZHed , PM.GetValue(miWRD_ZHed,pvWRD_ZHedWait))) {
        MT_EmgStop(miWRD_YHed);
        EM_SetErr(eiHED_MotorYZCrash);
        HED.Step.iCycle = 0 ; //푸하하하...;;;;; 이게 아마 안된다...쉬발.......답이 안보임.
    }
    else {
        MT_SetIntrptPos(miWRD_YHed , PM.GetValue(miWRD_YHed,pvWRD_YHedStripZMove) , HedZStripWorkWait );
    }
}

//void __stdcall HedZWaferWorkWait(long _lAxis, DWORD _dwFlag)
//{
////    MT_GoAbsRun(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWait));
//    MT_GoAbsRun(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWfWorkWait));
//}

CHead HED;

CHead::CHead()
{

}
CHead::~CHead (void)
{
    //메인폼으로 이동.
    //Close();
}
void CHead::SetIntrptPos1()
{
//    MT_ResetIntrpt(miWRD_YHed);
    MT_SetIntrptPos(10 , 0 , HedZStripWorkWait );
}
void CHead::SetIntrptPos2()
{
//    MT_ResetIntrpt(miWRD_YHed);
    MT_SetIntrptPos(miWRD_YHed , PM.GetValue(miWRD_YHed,pvWRD_YHedWaferZMove) , HedZWaferWorkWait );
}
void CHead::SetIntrptPos3()
{
//    MT_ResetIntrpt(miWRD_YHed);
    MT_SetIntrptPos(miWRD_YHed , PM.GetValue(miWRD_YHed,pvWRD_YHedWaferZMove) , HedZWaitPlace ); //2dan callback 안되면 즞망.
}
void CHead::SetIntrptPos4()
{
//    MT_ResetIntrpt(miWRD_YHed);
    MT_SetIntrptPos(miWRD_YHed , PM.GetValue(miWRD_YHed,pvWRD_YHedStripZMove) , HedZStripWorkWait );
}

void CHead::Init()
{
    InitCycleName();
    
    m_sPartName    = "Head " ;
    m_dLastIdxPos  = 0.0 ;
    m_iHomeSubStep = 0 ;
    iStgCheck     = hsNotWork ;

    m_dVoiceCoilWafer   = 0.0 ;
    m_dVoiceCoilWafer1  = 0.0 ;
    m_dVoiceCoilStrip   = 0.0 ;
    m_dVoiceCoilStrip1  = 0.0 ;

    m_iHomeTO  = (int)eiHED_HomeTO    ;
    m_iStartTO = (int)eiHED_ToStartTO ;
    m_iStopTO  = (int)eiHED_ToStopTO  ;
    m_iCycleTO = (int)eiHED_CycleTO   ;


    m_bDispPickCal  = false ;
    m_bDispPlaceCal = false ;
    m_dPickCalPos   = 0.0 ;
    m_dPlaceCalPos  = 0.0 ;

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

    AI_SetY(VOICE_COIL_ID , 0.0);
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
    bool bRet = false;
    if(_riWhre == riRDF) {
             if(DM.ARAY[riRDF].GetCntStat(csWait )) bRet = DM.ARAY[riRDF].FindLastColRow(csWait  , r , c) ;
        else if(DM.ARAY[riRDF].GetCntStat(csMask )) bRet = DM.ARAY[riRDF].FindLastColRow(csMask  , r , c) ;
        else if(DM.ARAY[riRDF].GetCntStat(csWait2)) bRet = DM.ARAY[riRDF].FindLastColRow(csWait2 , r , c) ;
        else if(DM.ARAY[riRDF].GetCntStat(csUnkwn)){
            c = DM.ARAY[riRDF].FindLastCol(csUnkwn);
            if(c % 2)bRet = DM.ARAY[riRDF].FindLastColRow    (csUnkwn , r , c) ;
            else     bRet = DM.ARAY[riRDF].FindLastColFrstRow(csUnkwn , r , c) ;
        }
        else bRet = false;
    }
    return bRet ;
}

double CHead::GetWorkPos(EN_MOTR_ID _iMotr)
{
    double dWorkPos = MT_GetCmdPos(_iMotr);
    int r  , c  ;

    if(_iMotr == miWRD_YHed){
        if(!OM.MstOptn.bPickUpTestMode) {
            if(FindChip(riRDF,r,c)) {
                dWorkPos = PM.GetValue (_iMotr , pvWRD_YHedStripStt ) + ( OM.DevInfo.dRowPitch * r ) ;
            }
        }
        else {
            dWorkPos = PM.GetValue (_iMotr , pvWRD_YHedStripStt ) ;

        }

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
            case piWRD_ZHedMove      : dPos = PM.GetValue (_iMotr , pvWRD_ZHedMove      ); break ;            
            case piWRD_ZHedWfWorkWait: dPos = PM.GetValue (_iMotr , pvWRD_ZHedWfWorkWait); break ;
            case piWRD_ZHedWfWork    : dPos = PM.GetValue (_iMotr , pvWRD_ZHedWfWork    ); break ;
            case piWRD_ZHedStWorkWait: dPos = PM.GetValue (_iMotr , pvWRD_ZHedStWorkWait); break ;
            case piWRD_ZHedStWork    : dPos = PM.GetValue (_iMotr , pvWRD_ZHedStWork    ); break ;
        }
    }
    else if(_iMotr == miWRD_THed){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piWRD_THedWait      : dPos = PM.GetValue (_iMotr , pvWRD_THedWait      ); break ;
//            case piWRD_THedWorkStt   : dPos = PM.GetValue (_iMotr , pvWRD_THedWorkStt   ); break ;
            case piWRD_THedWafer     : dPos = PM.GetValue (_iMotr , pvWRD_THedWafer     ); break ;
            case piWRD_THedStrip     : dPos = PM.GetValue (_iMotr , pvWRD_THedStrip     ); break ;
//            case piWRD_THedWork      : dPos = PM.GetValue (_iMotr , pvWRD_THedWork      ); break ;
        }
    }
    else if(_iMotr == miWRD_ZEjt){
        switch(_iPstnId) {
            default                    : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piWRD_ZEjtWait        : dPos = PM.GetValue (_iMotr , pvWRD_ZEjtWait      ); break ;
            case piWRD_ZEjtWorkBefore  : dPos = PM.GetValue (_iMotr , pvWRD_ZEjtWorkBefore); break ;
            case piWRD_ZEjtWorkStt     : dPos = PM.GetValue (_iMotr , pvWRD_ZEjtWork      ); break ;
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

    bool bHedZWaitPs = MT_GetStopInpos(miWRD_ZHed) && MT_GetCmdPos(miWRD_ZHed) <= PM.GetValue(miWRD_ZHed,pvWRD_ZHedMove) ;
    bool bStageStop  = MT_GetStopInpos(miWRD_XStg) && MT_GetStopInpos(miWRD_YStg) && MT_GetStopInpos(miWRD_TStg) && MT_GetStopInpos(miWRD_ZExd) ;
    bool bIdxStop    = MT_GetStopInpos(miWRD_XIdx) ;
    //Z축이 웨이퍼 높이까지 내려 갈수 있는지.
    bool bHedZWfrCanDn  = MT_GetCmdPos(miWRD_YHed) >= PM.GetValue(miWRD_YHed,pvWRD_YHedWaferZMove) ;
    bool bHedZWfrDnPos  = MT_GetCmdPos(miWRD_ZHed) >= PM.GetValue(miWRD_ZHed,pvWRD_ZHedMove      ) ;

    //Manual 용.
    bool bHedZCanDn     = MT_CmprPos(miWRD_YHed , PM.GetValue(miWRD_YHed,pvWRD_YHedStripStt ))  || MT_CmprPos(miWRD_YHed , PM.GetValue(miWRD_YHed,pvWRD_YHedWafer ));


    if(_iMotr == miWRD_XHed){
        if(!bHedZWaitPs  && !Step.iCycle) { sMsg = "HED_Z Axis is Over Move Position" ; bRet = false ;}
        switch(_iPstnId) {
            default                : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRD_XHedWait    : if(!bHedZWaitPs) { sMsg = "HED_Z Axis Over Move Position" ; bRet = false ;} break ;
            case piWRD_XHedWafer   : break ;
            case piWRD_XHedStrip   : break ;
        }
    }
    else if(_iMotr == miWRD_YHed){
        if(!bHedZWaitPs && !Step.iCycle) { sMsg = "HED_Z Axis is Over Move Position" ; bRet = false ;}
        switch(_iPstnId) {
            default                  : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRD_YHedWait      : if(!bHedZWaitPs) { sMsg = "HED_Z Axis Over Move Position" ; bRet = false ;} break ;
            case piWRD_YHedWafer     : break ;
            case piWRD_YHedWaferWait : if(!bHedZWfrDnPos) { sMsg = "HED_Z Axis is Not Move Position"; bRet = false ;} break ;
            case piWRD_YHedWaferZMove: break ;
            case piWRD_YHedStripStt  : if(!bHedZWfrDnPos) { sMsg = "HED_Z Axis is Not Move Position"; bRet = false ;} break ;
            case piWRD_YHedStripWait : if(!bHedZWfrDnPos) { sMsg = "HED_Z Axis is Not Move Position"; bRet = false ;} break ;
            case piWRD_YHedStripZMove: if(!bHedZWfrDnPos) { sMsg = "HED_Z Axis is Not Move Position"; bRet = false ;} break ;
            case piWRD_YHedWork      : if(!bHedZWfrDnPos) { sMsg = "HED_Z Axis is Not Move Position"; bRet = false ;} break ;

        }
    }
    else if(_iMotr == miWRD_ZHed){
        if(!bHedZCanDn && !Step.iCycle && _iPstnId != piWRD_ZHedWait && _iPstnId != piWRD_ZHedMove) { sMsg = "HED_Y Axis is Not Wait Position" ; bRet = false ;}
        switch(_iPstnId) {
            default                   : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRD_ZHedWait       : break ;
            case piWRD_ZHedMove       : break ;
            case piWRD_ZHedWfWorkWait : if(!bHedZWfrCanDn){sMsg = "Not Wafer Position" ; bRet = false ;}break ;//if(!bStageStop) { sMsg = "Stage Axis is Moving"       ; bRet = false ;} break ;
            case piWRD_ZHedWfWork     : if(!bStageStop) { sMsg = "Stage Axis is Moving"; bRet = false ;}
                                        if(!bHedZWfrCanDn){sMsg = "Not Wafer Position" ; bRet = false ;}break ;//if(!bStageStop) { sMsg = "Stage Axis is Moving"       ; bRet = false ;} break ;
            case piWRD_ZHedStWorkWait : break ;//if(!bIdxStop  ) { sMsg = "WRD_X_Index Axis is Moving" ; bRet = false ;} break ;
            case piWRD_ZHedStWork     : if(!bIdxStop  ) { sMsg = "WRD_X_Index Axis is Moving" ; bRet = false ;} break ;

        }
    }
    else if(_iMotr == miWRD_THed){
        if(!bHedZWaitPs) { sMsg = "HED_Z Axis is Not Wait Position" ; bRet = false ;}
        switch(_iPstnId) {
            default                : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRD_THedWait    : break ;
//            case piWRD_THedWorkStt : break ;
            case piWRD_THedWafer   : break ;
            case piWRD_THedStrip   : break ;
//            case piWRD_THedWork      : break ;
        }
    }
    else if(_iMotr == miWRD_ZEjt){
        switch(_iPstnId) {
            default                    : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRD_ZEjtWait        : break ;
            case piWRD_ZEjtWorkBefore  : break ;
            case piWRD_ZEjtWorkStt     : break ;
        }
    }

    else {
        sMsg = AnsiString(MT_GetName(_iMotr)) + " is Not this part Motr!" ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr),sMsg.c_str());
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
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr),sMsg.c_str());
    }

    return bRet ;
}

bool CHead::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlowSpeed) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) {
        if(_bSlowSpeed)return MT_GoAbsSlow(_iMotr , dPosition);
        else           return MT_GoAbsRun (_iMotr , dPosition);
    }
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CHead::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}
bool CHead::CycleHomeSub()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    static int m_iHomeSubPreStep = m_iHomeSubStep ;

    bool bTest = CheckStop() ;
    if (m_tmHome.OnDelay(m_iHomeSubStep && m_iHomeSubStep == m_iHomeSubPreStep && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iHomeTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : m_iHomeSubStep=%02d" , __FUNC__ , m_iHomeSubStep );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(m_iHomeSubStep != m_iHomeSubPreStep) {
        sTemp = sTemp.sprintf("%s m_iHomeSubStep=%02d" , __FUNC__ , m_iHomeSubStep );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    m_iHomeSubPreStep = m_iHomeSubStep ;

    if(Stat.bReqStop) {
        //m_iHomeSubStep = 0;
        //return true ;
    }

    switch (m_iHomeSubStep) {

        default: if(m_iHomeSubStep) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s m_iHomeSubStep=%02d" , __FUNC__ , m_iHomeSubStep );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 m_iHomeSubStep = 0 ;
                 return true ;

        case 10: MT_SetHomeEnd(miWRD_THed,false);
                 MT_SetSubHome(miWRD_THed,true);
                 MT_DoHome(miWRD_THed);
                 m_iHomeSubStep++;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miWRD_THed)) return false ;
                 MT_GoAbsMan(miWRD_THed , PM.GetValue(miWRD_THed , pvWRD_THedWait  ));

                 m_iHomeSubStep++;
                 return false ;

        case 14: if(!MT_GoAbsMan(miWRD_THed , PM.GetValue(miWRD_THed , pvWRD_THedWait  ))) return false ;
                 m_iHomeSubStep = 0;
                 return true ;
    }
}

bool CHead::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    bool bTest = CheckStop() ;
    if (m_tmHome.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iHomeTO);
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

        case 10:
                 IO_SetY(yHED_VoiceCoilServo,false);
                 AI_SetY(VOICE_COIL_ID , 0.0);
//                 MT_ResetIntrpt(miWRD_YHed);
//                 MT_ResetIntrpt(miWRD_ZHed);
//                 MT_ResetIntrpt(miWRD_ZEjt);

                 MT_SetHomeEnd(miWRD_XHed,false);
                 MT_SetHomeEnd(miWRD_YHed,false);
                 MT_SetHomeEnd(miWRD_ZHed,false);
                 MT_SetHomeEnd(miWRD_THed,false);
                 MT_SetHomeEnd(miWRD_ZEjt,false);

                 MT_DoHome(miWRD_ZHed);
                 MT_DoHome(miWRD_ZEjt);
                 Step.iHome++;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miWRD_ZHed)) return false ;
                 if(!MT_GetHomeEnd(miWRD_ZEjt)) return false ;
                 MT_DoHome(miWRD_XHed);
                 MT_DoHome(miWRD_YHed);
                 //MT_SetSubHome(miWRD_THed,true);
                 MT_DoHome(miWRD_THed);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miWRD_XHed))return false ;
                 if(!MT_GetHomeEnd(miWRD_YHed))return false ;
                 if(!MT_GetHomeEnd(miWRD_THed))return false ;
//                 CycleHomeSub();
//                 MT_DoHome(miWRD_THed);
//                 m_iHomeSubStep = 0 ; //TODO :: TEST
                 Step.iHome++;
                 return false ;

        case 13: //if(!CycleHomeSub()) return false ;
                 if(!MT_GetHomeEnd(miWRD_THed))return false ;
                 MT_GoAbsMan(miWRD_ZEjt , PM.GetValue(miWRD_ZEjt , pvWRD_ZEjtWait  ));
                 MT_GoAbsMan(miWRD_XHed , PM.GetValue(miWRD_XHed , pvWRD_XHedWait  ));
                 MT_GoAbsMan(miWRD_YHed , PM.GetValue(miWRD_YHed , pvWRD_YHedWait  ));

                 MT_GoAbsMan(miWRD_THed , PM.GetValue(miWRD_THed , pvWRD_THedWait  ));

                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GoAbsMan(miWRD_ZEjt , PM.GetValue(miWRD_ZEjt , pvWRD_ZEjtWait  ))) return false ;
                 if(!MT_GoAbsMan(miWRD_XHed , PM.GetValue(miWRD_XHed , pvWRD_XHedWait  ))) return false ;
                 if(!MT_GoAbsMan(miWRD_YHed , PM.GetValue(miWRD_YHed , pvWRD_YHedWait  ))) return false ;

                 if(!MT_GoAbsMan(miWRD_THed , PM.GetValue(miWRD_THed , pvWRD_THedWait  ))) return false ;
                 MT_GoAbsMan(miWRD_ZHed , PM.GetValue(miWRD_ZHed , pvWRD_ZHedWait  ));

                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GoAbsMan(miWRD_ZHed , PM.GetValue(miWRD_ZHed , pvWRD_ZHedWait  ))) return false ;

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
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)m_iStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    static bool bExistChip = false ;
    //Move Home.
    switch (Step.iToStart) {
        default: if(Step.iToStart) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStart = 0 ;
                 return true ;

        case 10:

                  //m_dVoiceCoilWafer  = OM.DevOptn.dVoiceCoilWafer  * OM.MstOptn.dVoiceCoilA + OM.MstOptn.dVoiceCoilB ;
                 //m_dVoiceCoilWafer1 = OM.DevOptn.dVoiceCoilWafer1 * OM.MstOptn.dVoiceCoilA + OM.MstOptn.dVoiceCoilB ;
                 //m_dVoiceCoilStrip  = OM.DevOptn.dVoiceCoilStrip  * OM.MstOptn.dVoiceCoilA + OM.MstOptn.dVoiceCoilB ;
                 //m_dVoiceCoilStrip1 = OM.DevOptn.dVoiceCoilStrip1 * OM.MstOptn.dVoiceCoilA + OM.MstOptn.dVoiceCoilB ;

                 //if(m_dVoiceCoilWafer  > 5.0 || m_dVoiceCoilWafer   < -5.0 ) m_dVoiceCoilWafer  = 0.0 ; //안써도 될거 같으니깐 요렇게 넘어가자.
                 //if(m_dVoiceCoilWafer1 > 5.0 || m_dVoiceCoilWafer1  < -5.0 ) m_dVoiceCoilWafer1 = 0.0 ;
                 //if(m_dVoiceCoilStrip  > 5.0 || m_dVoiceCoilStrip   < -5.0 ) m_dVoiceCoilStrip  = 0.0 ;
                 //if(m_dVoiceCoilStrip1 > 5.0 || m_dVoiceCoilStrip1  < -5.0 ) m_dVoiceCoilStrip1 = 0.0 ;

                 AI_SetY(VOICE_COIL_ID , 0.0 ) ; //OM.DevOptn.dVoiceCoilMove) ; //보이스 코일

                 MT_ResetIntrpt(miWRD_YHed);
                 MT_ResetIntrpt(miWRD_ZEjt);
                 MoveMotr(miWRD_ZHed,piWRD_ZHedMove);
                 MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveMotr(miWRD_ZHed,piWRD_ZHedMove)) return false ;
                 if(!MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait)) return false ;

                 bExistChip = !DM.ARAY[riHED].CheckAllStat(csNone);

                 if(bExistChip) {
                     MoveMotr(miWRD_XHed,piWRD_XHedWafer    );
                     MoveMotr(miWRD_YHed,piWRD_YHedWaferWait); //Vision Grab Enable
                 }
                 else {
                     MoveMotr(miWRD_XHed,piWRD_XHedStrip    );
                     MoveMotr(miWRD_YHed,piWRD_YHedStripWait);
                 }
                 Step.iToStart ++;
                 return false ;

        case 12: if(bExistChip) {
                     if(!MoveMotr(miWRD_XHed,piWRD_XHedWafer    )) return false ;
                     if(!MoveMotr(miWRD_YHed,piWRD_YHedWaferWait)) return false ;
                 }
                 else {
                     if(!MoveMotr(miWRD_XHed,piWRD_XHedStrip    )) return false ;
                     if(!MoveMotr(miWRD_YHed,piWRD_YHedStripWait)) return false ;
                 }
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CHead::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)m_iStopTO);

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

        case 10: AI_SetY(VOICE_COIL_ID , 0.0 ) ; //OM.DevOptn.dVoiceCoilMove) ; //보이스 코일
                 IO_SetY(yHED_VoiceCoilServo , false);
                 MT_ResetIntrpt(miWRD_YHed);
                 MT_ResetIntrpt(miWRD_ZEjt);
                 MoveMotr(miWRD_ZHed,piWRD_ZHedMove);
                 MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait);
                 Step.iToStop ++;
                 return false ;

        case 11: if(!MoveMotr(miWRD_ZHed,piWRD_ZHedMove)) return false ;
                 if(!MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait)) return false ;
                 if(bChip) {
                     MoveMotr(miWRD_XHed,piWRD_XHedStrip    );
                     MoveMotr(miWRD_YHed,piWRD_YHedStripWait);
                 }
                 else {
                     MoveMotr(miWRD_XHed,piWRD_XHedWafer    );
                     MoveMotr(miWRD_YHed,piWRD_YHedWaferWait);
                 }
                 //MoveMotr(miWRD_THed , piWRD_THedWait); //백레쉬 때문에

                 Step.iToStop ++;
                 return false ;

        case 12: if(bChip) {
                     if(!MoveMotr(miWRD_XHed,piWRD_XHedStrip    )) return false ;
                     if(!MoveMotr(miWRD_YHed,piWRD_YHedStripWait)) return false ;
                 }
                 else {
                     if(!MoveMotr(miWRD_XHed,piWRD_XHedWafer    )) return false ;
                     if(!MoveMotr(miWRD_YHed,piWRD_YHedWaferWait)) return false ;
                 }
                 //if(!MoveMotr(miWRD_THed , piWRD_THedWait)) return false ;
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
                             /*riREV , riRDV 검사 결과.*/

        bool isEpx         = DM.ARAY[riREV].GetCntStat(csWork) || //EPX.GetEpxLotCnt() <= OM.CmnOptn.iLotEndChipCnt ;
                            (DM.ARAY[riRDV].GetCntStat(csUnkwn) - DM.ARAY[riRDF].GetCntStat(csWait2));//플레이스 하고선 검사하기 전에 아직 언노운이다.

        bool isCyclePick  = !OM.MstOptn.bIgnrDieAttach && DM.ARAY[riSTG].GetCntStat(csGood) && DM.ARAY[riHED].CheckStat(0,0,csNone ) && !OM.CmnOptn.bStageStop && (isEpx || OM.MstOptn.bPickUpTestMode) ;// 픽 테스트 할때 홀드 남...불편해서 일단 주석.
        bool isCyclePlace = !OM.MstOptn.bIgnrDieAttach && DM.ARAY[riHED].CheckStat(0,0,csGood ) &&
                                                                                                  //테스트 모드설정시에 레일은 그냥 동작 하기 때문에 처박을 수 있어서.
                           ((DM.ARAY[riRDF].GetCntStat(csMask) && !OM.MstOptn.bPickUpTestMode) || (DM.ARAY[riRDF].CheckAllStat(csNone) && OM.MstOptn.bPickUpTestMode)) ;

        bool isConEnd     =  DM.ARAY[riRDF].CheckAllStat(csNone) ;//&& //DM.ARAY[riHED].CheckAllStat(csNone);

        if(!OM.CmnOptn.bIgnrVcSsr && !OM.MstOptn.bDryRun){
            if( DM.ARAY[riHED].CheckStat(0,0,csNone ) &&  IO_GetX(xHED_Vaccum) ){
                EM_SetErr(eiHED_Unknwn) ;
            }
        }
        if(!OM.CmnOptn.bIgnrVcSsr && !OM.MstOptn.bDryRun){
            if(!DM.ARAY[riHED].CheckStat(0,0,csNone ) && !IO_GetX(xHED_Vaccum) ){
                DM.ARAY[riHED].SetStat(csNone);
                IO_SetY(yHED_Vaccum , false);
                EM_SetErr(eiHED_Dispr ) ;

            }
        }


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
bool CHead::InspectToolSafe(void)
{
    double dCmdPosY = MT_GetCmdPos(miWRD_YHed) ;
    double dCmdPosZ = MT_GetCmdPos(miWRD_ZHed) ;

    double dEncPosY = MT_GetEncPos(miWRD_YHed) ;
    double dEncPosZ = MT_GetEncPos(miWRD_ZHed) ;


    bool   dCrashZonePosY = false ;
    bool   dCrashZonePosZ = false ;

    if(dCmdPosY < OM.MstOptn.dToolCrashWfrY && dCmdPosY > OM.MstOptn.dToolCrashRalY) dCrashZonePosY = true ;
    if(dEncPosY < OM.MstOptn.dToolCrashWfrY && dEncPosY > OM.MstOptn.dToolCrashRalY) dCrashZonePosY = true ;

    if(dCmdPosZ > OM.MstOptn.dToolCrashZ                                           ) dCrashZonePosZ = true ;
    if(dEncPosZ > OM.MstOptn.dToolCrashZ                                           ) dCrashZonePosZ = true ;

    if(dCrashZonePosY && dCrashZonePosZ) { //충돌 위치.
        MT_EmgStop(miWRD_YHed);
        MT_EmgStop(miWRD_ZHed);

        Sleep(100);
        MT_SetServo(miWRD_YHed , false);
        MT_SetServo(miWRD_ZHed , false);

        return false ;
    }

    return true ;
}

bool CHead::CyclePick() //메뉴얼일때 강제로 T축 안돌리기)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        MT_ResetIntrpt(miWRD_YHed);
        MT_ResetIntrpt(miWRD_ZEjt);
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(!InspectToolSafe()) {
        Step.iCycle = 0;
        return true;
    }



    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true;
    //}

    if(IO_GetX(xHED_Overload) && !OM.CmnOptn.bIgnrHedOverload) {  //오버로드 확인 하고.
        EM_SetErr(eiHED_VoiceCoilOverload);
        Step.iCycle = 0;
        return true ;
    }




    int r,c ;

//    static double dPickWait = 0.0 ;
//    static bool   bWrdMask  = false ;
//    static double dZEjtWkBf = 0.0 ;

    double dTemp1 , dTemp2 , dEjtHeight;


    bool bTemp ;

    double dCrntTime ;
    static double dPreTime = GetTickTime();

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: dCrntTime = GetTickTime();
                  m_dPickCycleTime = (dCrntTime - dPreTime)/1000.0 ;
                  dPreTime = dCrntTime ;

                  //플레이스 페일 확인용.
                  IO_SetY(yHED_Eject ,false);
                  IO_SetY(yHED_Vaccum,true );

                  //AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilStrip);

                  MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait );

                  //메뉴얼 동작시 동작을 안해서 넣음
                  MoveActr(aiEJT_UpDn , ccFwd);

                  MoveMotr(miWRD_ZHed,piWRD_ZHedMove ); //여기부터 세타 돌리는것 하다 말았음.

                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait )) return false ;

                  if(!MoveMotr(miWRD_ZHed,piWRD_ZHedMove )) return false ;
                  if(!MoveActr(aiEJT_UpDn , ccFwd)        ) return false ;

                  MoveMotr(miWRD_XHed,piWRD_XHedWafer);
                  MoveMotr(miWRD_YHed,piWRD_YHedWafer);
                  if(MM.GetManNo() == mcHED_CyclePickNoT) MT_GoAbsRun(miWRD_THed,GetMotrPos(miWRD_THed,piWRD_THedWafer) );
                  else                                    MT_GoAbsRun(miWRD_THed,GetMotrPos(miWRD_THed,piWRD_THedWafer) - (OM.CmnOptn.bUseHeadTWfr ? STG.GetTOffset() : 0.0 ));



                  

                  //웨이퍼스테이지에서 관장하나 혹시나 해서.
                  IO_SetY(yEJT_Suction , true);

                  Step.iCycle++;
                  return false;

        case  12: if(PM.GetValue(miWRD_YHed,pvWRD_YHedWaferZMove) < MT_GetCmdPos(miWRD_YHed) && MT_GetCmdPos(miWRD_ZHed) != PM.GetValue(miWRD_ZHed , pvWRD_ZHedWfWorkWait)) { //

                      //플레이스 페일 확인용.  자제 아직 붙어 있는지.
                      if(!OM.CmnOptn.bIgnrVcSsr && !OM.MstOptn.bDryRun && IO_GetX(xHED_Vaccum)) {
                          EM_SetErr(eiHED_Unknwn) ;
                          MoveMotr(miWRD_ZHed,piWRD_ZHedMove );
                          Step.iCycle = 0;
                          return true ;
                      }




                      MoveMotr(miWRD_ZHed,piWRD_ZHedWfWorkWait,true);
                      //AI_SetY(VOICE_COIL_ID , m_dVoiceCoilWafer) ;

                      if(OM.CmnOptn.bUseVcCoil) {
                          IO_SetY(yHED_VoiceCoilServo , true);
                      }
                      AI_SetY(VOICE_COIL_ID , GetVoiceCoilVolt(OM.DevOptn.dVoiceCoilWafer) ) ;


                      //에메하다 확인해야함.
                          /*메뉴얼 픽 할때 한번 떨어졌던 자제는 자꾸 엄한데서 에러남.
                          //플레이스 페일 확인용.  자제 아직 붙어 있는지.
                          if(!OM.CmnOptn.bIgnrVcSsr && !OM.MstOptn.bDryRun && IO_GetX(xHED_Vaccum)) {
                              EM_SetErr(eiHED_Unknwn) ;
                              MoveMotr(miWRD_ZHed,piWRD_ZHedMove );
                              Step.iCycle = 0;
                              return true ;
                          }
                          */


                  }

//                  if(PM.GetValue(miWRD_YHed,pvWRD_YHedWaferZMove) - VOICECOIL_PS_OFFSET_Y < MT_GetCmdPos(miWRD_YHed)) { //
//                      //보이스 코일 웨이퍼 Z축 이동으로 세팅.
//                      AI_SetY(VOICE_COIL_ID , m_dVoiceCoilWafer) ;
//                  }
                  if(!MT_GetStopInpos(miWRD_THed)              ) return false ;
                  if(!MoveMotr(miWRD_XHed,piWRD_XHedWafer     )) return false ;
                  if(!MoveMotr(miWRD_YHed,piWRD_YHedWafer     )) return false ;
                  if(!MoveMotr(miWRD_ZHed,piWRD_ZHedWfWorkWait)) return false ;



                  //보이스 코일 최종 픽으로 세팅.
                  AI_SetY(VOICE_COIL_ID , GetVoiceCoilVolt(OM.DevOptn.dVoiceCoilWafer1)) ;

                  //최종 픽위치로 Z축 내림.
                  MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWfWork) ,OM.DevOptn.dPickWaitVel,OM.DevOptn.dPickWaitAc,OM.DevOptn.dPickWaitDc);

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

                  //웨이퍼 컨텍.
        case  13: if(!MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWfWork) ,OM.DevOptn.dPickWaitVel,OM.DevOptn.dPickWaitAc,OM.DevOptn.dPickWaitDc)) return false ;
                  if(m_tmTemp.OnDelay(true,2000)) {
                      EM_SetErr(eiHED_EjtVacuumFail); //TODO :: Error Id
                      MoveMotr(miWRD_ZHed,piWRD_ZHedMove );
                      Step.iCycle = 0;
                      return true ;
                  }
                  if(!OM.MstOptn.bDryRun && !IO_GetX(xEJT_Vaccum)) return false ;

                  //툴 , 이젝터 속도 통일 위해.
                  //MoveMotr(miWRD_ZEjt,piWRD_ZEjtWorkBefore);
                  MT_GoAbsAcDc(miWRD_ZEjt,PM.GetValue(miWRD_ZEjt,pvWRD_ZEjtWorkBefore) , OM.DevOptn.dPickWaitVel,OM.DevOptn.dPickWaitAc,OM.DevOptn.dPickWaitDc);


                  Step.iCycle++;
                  return false;

                  //툴 , 이젝터 속도 통일 위해.
        case  14: //if(!MoveMotr(miWRD_ZEjt,piWRD_ZEjtWorkBefore)) return false ; MT_GoAbsAcDc(miWRD_ZEjt,PM.GetValue(miWRD_ZHed,pvWRD_ZEjtWorkBefore) , OM.DevOptn.dPickWaitVel,OM.DevOptn.dPickWaitAc,OM.DevOptn.dPickWaitDc)
                  if(!MT_GoAbsAcDc(miWRD_ZEjt,PM.GetValue(miWRD_ZEjt,pvWRD_ZEjtWorkBefore) , OM.DevOptn.dPickWaitVel,OM.DevOptn.dPickWaitAc,OM.DevOptn.dPickWaitDc)) return false ;
                  //MoveMotr(miWRD_ZEjt,piWRD_ZEjtWorkStt);

                  //툴 , 이젝터 height 통일 위해.
                  MT_GoAbsAcDc(miWRD_ZEjt,PM.GetValue(miWRD_ZEjt,pvWRD_ZEjtWork                ),OM.DevOptn.dPickWaitVel,OM.DevOptn.dPickWaitAc,OM.DevOptn.dPickWaitDc);
                  dEjtHeight = PM.GetValue(miWRD_ZEjt,pvWRD_ZEjtWork ) - PM.GetValue(miWRD_ZEjt,pvWRD_ZEjtWorkBefore ) ;
                  MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWfWork) - dEjtHeight ,OM.DevOptn.dPickWaitVel,OM.DevOptn.dPickWaitAc,OM.DevOptn.dPickWaitDc);




                  Step.iCycle++;
                  return false ;

        case  15: if(!MT_GoAbsAcDc(miWRD_ZEjt,PM.GetValue(miWRD_ZEjt,pvWRD_ZEjtWork                ),OM.DevOptn.dPickWaitVel,OM.DevOptn.dPickWaitAc,OM.DevOptn.dPickWaitDc)) return false ;
                  IO_SetY(yEJT_Suction , false);
                  dEjtHeight = PM.GetValue(miWRD_ZEjt,pvWRD_ZEjtWork ) - PM.GetValue(miWRD_ZEjt,pvWRD_ZEjtWorkBefore ) ;
                  if(!MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWfWork) - dEjtHeight ,OM.DevOptn.dPickWaitVel,OM.DevOptn.dPickWaitAc,OM.DevOptn.dPickWaitDc)) return false ;

                  //if(!m_tmTemp.OnDelay(true,OM.CmnOptn.iEjctrUpAfterDelay))return false ;

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  16: if(!m_tmTemp.OnDelay(true,OM.DevOptn.iVacuumOnDly)){
                      if(IO_GetX(xHED_Vaccum) || OM.MstOptn.bDryRun) {
                          //픽이 정상 완료. 되고 이젝터 올라가면서
                          m_tmTemp.Clear();
                          Step.iCycle++;
                          return false ;
                      }
                      //이물 날림 때문에 테스트 용임.... 위의 자제가 뚜껑 역활을 하면서 이젝터 핀이 내려가면서 이물이 옆으로 못튀게 하려고 해봄.

                      return false ;
                  }

                  if(OM.CmnOptn.bIgnrVcSsr) {//배큠에러 무시일경우.
                      m_tmTemp.Clear();
                      Step.iCycle++;
                      return false;
                  }

                  //배큠에러.
                  EM_SetErr(eiHED_VacuumSsr);
                  IO_SetY(yHED_Eject ,false);
                  IO_SetY(yHED_Vaccum,false);
                  MoveMotr(miWRD_ZHed,piWRD_ZHedWait);
                  IO_SetY(yEJT_Suction , false);
                  MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait);
                  Step.iCycle = 0 ;
                  return true ;


        case  17: if(!m_tmTemp.OnDelay(true , OM.DevOptn.iEjectorBfDnDly)) return false ;

                  //이젝터 먼저 내려 모았으나 포지션 틀어짐.
                  if(OM.MstOptn.bEjectorDnFirst)MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait);
                  Step.iCycle++;
                  return false;




                  //조금 있다가 Z축도 올라가기 시작.
        case  18: //MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWfWorkWait), OM.DevOptn.dPickWaitVel,OM.DevOptn.dPickWaitAc,OM.DevOptn.dPickWaitDc); //픽대기위치로 이동.
                  if(OM.MstOptn.bEjectorDnFirst && !MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait)) return false ;


                  //보이스 코일  Z축 이동 으로 세팅.
                  AI_SetY(VOICE_COIL_ID , GetVoiceCoilVolt(OM.DevOptn.dVoiceCoilWafer)) ;
                  MoveMotr(miWRD_ZHed , piWRD_ZHedMove);
                  //배큠을 끈다. 안끄면 이젝터가 낑겨서 안내려옴.
                  IO_SetY(yEJT_Suction , false);
                  Step.iCycle++;
                  return false;

                  //이젝터 이동.
        case  19: dTemp1 = MT_GetCmdPos(miWRD_ZHed) ;
                  dTemp2 = GetMotrPos(miWRD_ZHed , piWRD_ZHedWfWorkWait)- 2.0 ;
                  if(dTemp1 > dTemp2) return false ;


                  MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait);

                  //백레쉬 때문에
                  //if(bUseBackRash)MoveMotr(miWRD_THed , piWRD_THedWait);



                  //MoveMotr(miWRD_YHed,piWRD_YHedWaferZMove);
                  //위험 ... Z축 속도 줄이면 작살남. 1400이상일때만 동작 하게 함.
                  if(DM.ARAY[riRDF].GetCntStat(csMask) && MT_GetRunVel(miWRD_ZHed) > 1400){
                      if(!OM.MstOptn.bIgnrBfVision) {
                          MT_GoAbsRun(miWRD_XHed,GetMotrPos(miWRD_XHed,piWRD_XHedStrip  ) + RAD.GetXOffset()  );
                          MT_GoAbsRun(miWRD_YHed,GetMotrPos(miWRD_YHed,piWRD_YHedWork   ) + RAD.GetYOffset()  );
                          MT_GoAbsRun(miWRD_THed,GetMotrPos(miWRD_THed,piWRD_THedStrip  ) + (OM.CmnOptn.bUseHeadTRal ? RAD.GetTOffset() : 0.0 ) );
                          Trace("WfrX",AnsiString(RAD.GetXOffset()).c_str());
                          Trace("WfrY",AnsiString(RAD.GetYOffset()).c_str());
                          Trace("WfrT",AnsiString(RAD.GetTOffset()).c_str());

                      }
                      else {
                          MT_GoAbsRun(miWRD_XHed,GetMotrPos(miWRD_XHed,piWRD_XHedStrip));
                          MT_GoAbsRun(miWRD_YHed,GetMotrPos(miWRD_YHed,piWRD_YHedWork ));
                          MT_GoAbsRun(miWRD_THed,GetMotrPos(miWRD_THed,piWRD_THedStrip));
                      }
                  }



                  Step.iCycle++;
                  return false ;

                  //이젝터 내려간것. 확인 하고.
        case  20: if(!MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait)) return false;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

                  //이젝터 딜레이 안주면 웨이퍼 긁힐수도 있음.
        case  21: if(!m_tmTemp.OnDelay(true,OM.CmnOptn.iEjctrDnAfterDelay)) return false ;



                  AI_SetY(VOICE_COIL_ID , 0.0);//OM.DevOptn.dVoiceCoilMove) ;
                  IO_SetY(yHED_VoiceCoilServo , false);


                  if(!OM.MstOptn.bDryRun && !IO_GetX(xHED_Vaccum)) {
                      //배큠에러.
                      EM_SetErr(eiHED_VacuumSsr);
                      IO_SetY(yHED_Eject ,false);
                      IO_SetY(yHED_Vaccum,true);
                      MoveMotr(miWRD_ZHed,piWRD_ZHedWait);
                      Step.iCycle = 0 ;
                      return true ;
                  }















                  DM.ARAY[riHED].SetStat(csGood);

                  if(OM.CmnOptn.bUseMapFile) DM.ARAY[riSTG].ChangeStat(csGood,csEmpty);
                  else                       DM.ARAY[riSTG].SetStat   (csUnkwn);
                  Step.iCycle++;
                  return false ;
                                                              /*나중에 포지션으로 빼기.*/
        case  22: //if(PM.GetValue(miWRD_ZHed,pvWRD_ZHedMove) + 10.0 > MT_GetCmdPos(miWRD_ZHed)) { //해드가 무부 가능 포지션 5미리 전에 Y축 움직이기 시작.
                      //MoveMotr(miWRD_YHed,piWRD_YHedWaferZMove);
                      //AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilMove) ;
                  //}
                  if(!MoveMotr(miWRD_ZHed,piWRD_ZHedMove))return false ;
                  //if(bUseBackRash && !MoveMotr(miWRD_THed , piWRD_THedWait)) return false ;

                  //미리보내 본다.
                  if(DM.ARAY[riRDF].GetCntStat(csMask)){
                      if(!OM.MstOptn.bIgnrBfVision) {
                          MT_GoAbsRun(miWRD_XHed,GetMotrPos(miWRD_XHed,piWRD_XHedStrip  ) + RAD.GetXOffset()  );
                          MT_GoAbsRun(miWRD_YHed,GetMotrPos(miWRD_YHed,piWRD_YHedWork   ) + RAD.GetYOffset()  );
                          MT_GoAbsRun(miWRD_THed,GetMotrPos(miWRD_THed,piWRD_THedStrip  ) + (OM.CmnOptn.bUseHeadTRal ? RAD.GetTOffset() : 0.0 ) );
                          Trace("WfrX",AnsiString(RAD.GetXOffset()).c_str());
                          Trace("WfrY",AnsiString(RAD.GetYOffset()).c_str());
                          Trace("WfrT",AnsiString(RAD.GetTOffset()).c_str());

                      }
                      else {
                          MT_GoAbsRun(miWRD_XHed,GetMotrPos(miWRD_XHed,piWRD_XHedStrip  )  );
                          MT_GoAbsRun(miWRD_YHed,GetMotrPos(miWRD_YHed,piWRD_YHedWork   )  );
                          MT_GoAbsRun(miWRD_THed,GetMotrPos(miWRD_THed,piWRD_THedStrip  )  );
                      }
                  }



                  Step.iCycle = 0 ;
                  return true ;
    }
}

/*
bool CHead::CyclePick(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        MT_ResetIntrpt(miWRD_YHed);
        MT_ResetIntrpt(miWRD_ZEjt);
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

    int r,c ;

//    static double dPickWait = 0.0 ;
//    static bool   bWrdMask  = false ;
//    static double dZEjtWkBf = 0.0 ;


    bool bTemp ;
    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(yHED_Eject ,false);
                  IO_SetY(yHED_Vaccum,true );

                  if(!MT_CmprPos(miWRD_ZHed,PM.GetValue(miWRD_ZHed,piWRD_ZHedMove))) AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilStrip);//OM.DevOptn.dVoiceCoilMove) ; //보이스 코일
                  else AI_SetY(VOICE_COIL_ID , 0 );//OM.DevOptn.dVoiceCoilMove) ; //보이스 코일

                  MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait );
                  MT_GoAbsRun(miWRD_THed,PM.GetValue(miWRD_THed,pvWRD_THedWafer) + STG.GetTOffset());
                  //MoveMotr(miWRD_THed,piWRD_THedWafer);
                  MoveMotr(miWRD_ZHed,piWRD_ZHedMove );

                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait )) return false ;
                  if(!MT_GoAbsRun(miWRD_THed,PM.GetValue(miWRD_THed,pvWRD_THedWafer) + STG.GetTOffset())) return false ;
                  if(!MoveMotr(miWRD_ZHed,piWRD_ZHedMove )) return false ;

                  AI_SetY(VOICE_COIL_ID , 0 );

                  MoveMotr(miWRD_XHed,piWRD_XHedWafer);
                  MoveMotr(miWRD_YHed,piWRD_YHedWafer);

                  Step.iCycle++;
                  return false;

        case  12: if(PM.GetValue(miWRD_YHed,pvWRD_YHedWaferZMove) - VOICECOIL_PS_OFFSET_Y < MT_GetCmdPos(miWRD_YHed)) { //
                      AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilWafer) ;
//                      MoveMotr(miWRD_ZHed,piWRD_ZHedWfWorkWait);
                  }

                  if(PM.GetValue(miWRD_YHed,pvWRD_YHedWaferZMove) < MT_GetCmdPos(miWRD_YHed)) { //
//                      AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilWafer) ;
                      MoveMotr(miWRD_ZHed,piWRD_ZHedWfWorkWait);
                  }


                  if(!MoveMotr(miWRD_XHed,piWRD_XHedWafer)) return false ;
                  if(!MoveMotr(miWRD_YHed,piWRD_YHedWafer)) return false ;
                  if(!MoveMotr(miWRD_ZHed,piWRD_ZHedWfWorkWait)) return false ;

                  AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilWafer1) ;
                  IO_SetY(yHED_Vaccum,true);
                  MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWfWork) ,OM.DevOptn.dPickWaitVel,OM.DevOptn.dPickWaitAc,OM.DevOptn.dPickWaitDc);

                  Step.iCycle++;
                  return false ;

        case  13: if(IO_GetX(xHED_Overload) && !OM.CmnOptn.bIgnrHedOverload) {
                      EM_SetErr(eiHED_VoiceCoilOverload);
                      Step.iCycle = 0;
                      return true ;
                  }
//                  if(!MT_GetStopInpos(miWRD_ZHed)) return false ;
                  if(!MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWfWork) ,OM.DevOptn.dPickWaitVel,OM.DevOptn.dPickWaitAc,OM.DevOptn.dPickWaitDc)) return false ;
//                  IO_SetY(yHED_Vaccum,true);
                  MoveMotr(miWRD_ZEjt,piWRD_ZEjtWorkBefore);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  14: if(!m_tmTemp.OnDelay(true,OM.DevOptn.iVacuumOnDly)) return false ;
                  if(!IO_GetX(xHED_Vaccum) && !OM.CmnOptn.bIgnrVcSsr) {
                      EM_SetErr(eiHED_VacuumSsr);
                      IO_SetY(yHED_Eject ,false);
                      IO_SetY(yHED_Vaccum,false);
                      MoveMotr(miWRD_ZHed,piWRD_ZHedWait);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miWRD_ZEjt,piWRD_ZEjtWorkBefore)) return false ;
                  MoveMotr(miWRD_ZEjt,piWRD_ZEjtWorkStt);
                  MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWfWorkWait), OM.DevOptn.dPickWaitVel,OM.DevOptn.dPickWaitAc,OM.DevOptn.dPickWaitDc);
                  Step.iCycle++;
                  return false;

        case  16: if(!IO_GetX(xHED_Vaccum) && !OM.CmnOptn.bIgnrVcSsr) {
                      EM_SetErr(eiHED_VacuumSsr);
                      IO_SetY(yHED_Eject ,false);
                      IO_SetY(yHED_Vaccum,false);
                      MoveMotr(miWRD_ZHed,piWRD_ZHedWait);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  if(!MoveMotr(miWRD_ZEjt,piWRD_ZEjtWorkStt)) return false ;
                  if(!MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWfWorkWait), OM.DevOptn.dPickWaitVel,OM.DevOptn.dPickWaitAc,OM.DevOptn.dPickWaitDc)) return false ;
                  //if(!MT_GetStopInpos(miWRD_ZHed)) return false ;
                  MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait);
                  AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilWafer) ;
                  Step.iCycle++;
                  return false;

        case  17: if(!MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait)) return false;

//                  AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilWafer) ;
                  MoveMotr(miWRD_YHed,piWRD_YHedWaferZMove);
                  MoveMotr(miWRD_ZHed,piWRD_ZHedMove);
//                  MoveMotr(miWRD_XHed,piWRD_XHedStrip);
                  DM.ARAY[riHED].SetStat(0,0,csUnkwn);

//                  STG.SetLastPickPos() ;
                  if(OM.CmnOptn.bUseMapFile) DM.ARAY[riSTG].ChangeStat(csWork,csEmpty);
                  else                       DM.ARAY[riSTG].SetStat   (csUnkwn);

                  Step.iCycle = 0 ;
                  return true ;
    }
}
*/


bool CHead::CyclePickCurve(void)
{
/*
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        MT_ResetIntrpt(miWRD_YHed);
        MT_ResetIntrpt(miWRD_ZEjt);
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

    int r,c ;

//    static double dPickWait = 0.0 ;
//    static bool   bWrdMask  = false ;
//    static double dZEjtWkBf = 0.0 ;


    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }

                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(yHED_Eject ,false);
                  IO_SetY(yHED_Vaccum,false);

                  if(MT_CmprPos(miWRD_ZHed , PM.GetValue(miWRD_ZHed,piWRD_ZHedMove))) AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilWafer );//OM.DevOptn.dVoiceCoilMove) ; //보이스 코일
                  else AI_SetY(VOICE_COIL_ID , 0 );//OM.DevOptn.dVoiceCoilMove) ; //보이스 코일

                  MT_ResetIntrpt(miWRD_YHed);
                  MT_ResetIntrpt(miWRD_ZEjt);

                  Step.iCycle++;
                  return false;

        case  11: MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait );
                  MoveMotr(miWRD_THed,piWRD_THedWafer);
                  MoveMotr(miWRD_ZHed,piWRD_ZHedMove );

                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait )) return false ;
                  if(!MoveMotr(miWRD_THed,piWRD_THedWafer)) return false ;
                  if(!MoveMotr(miWRD_ZHed,piWRD_ZHedMove )) return false ;

                  MoveMotr(miWRD_XHed,piWRD_XHedWafer);
                  MoveMotr(miWRD_YHed,piWRD_YHedWafer);
                  //CallBack
                  MT_SetIntrptPos(miWRD_YHed , PM.GetValue(miWRD_YHed,pvWRD_YHedWaferZMove) , CallAxisZWaferPs ); //Position Z
                  MT_SetIntrptPos(miWRD_YHed , PM.GetValue(miWRD_YHed,pvWRD_YHedWaferZMove) - VOICECOIL_PS_OFFSET_Y , CallAxisZWaferVc ); //Voice Coil

                  if(!OM.MstOptn.bIgnrWfVision) {
                      STG.SetReqVision();
                  }

                  if(!DM.ARAY[riRDF].GetCntStat(csMask)) { //레일 에프터 비전 패스.
                      Step.iCycle = 16;
                      return false;
                  }

                  if(OM.CmnOptn.bIgnrRalAtVs ) {
                      if(DM.ARAY[riRDF].GetCntStat(csMask)) {
                          FindChip(riRDF,r,c,csMask) ;
                          DM.ARAY[riWRD].SetStat(r,c,csWork);
                          DM.ARAY[riRDF].SetStat(r,c,csWork);
                      }
                      Step.iCycle = 16;
                      return false;
                  }
                  //CallBack At Grib
                  MT_SetIntrptPos(miWRD_YHed , PM.GetValue(miWRD_YHed,pvWRD_YHedStripWait ) , CallVisnAtGrib   ); //Position Z
                  m_tmTemp.Clear() ;
                  Step.iCycle++;
                  return false;

        case  13: //검사 되겟지 머.
                  if(m_tmTemp.OnDelay(!VS_GetGrabEnd(viRat),OM.DevOptn.iRalAtVsGrabTO )) {
                      EM_SetErr(eiHED_RalAtVsGrabFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  if(!VS_GetGrabEnd(viRat)) return false ;
                  VS_Inspect(viRat,false); //쓰레드로 빼던가 나중에 ...
                  //비전에 따라 딜레이 밑으로 빼던가 다른데로 빼던가.
                  m_tmTemp.Clear() ;
                  Step.iCycle++;
                  return false ;

        case  14: if(m_tmTemp.OnDelay(!VS_GetInspEnd(viRat),OM.DevOptn.iRalAtVsInspTO )) {
                      EM_SetErr(eiHED_RalAtVsInspFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  if(!VS_GetInspEnd(viRat)) return false ;
//                  VS_GetRslt(viRat,&RsltRat);

                  if(!RsltRat.bRslt){
                      EM_SetErr(eiHED_RalAtVsInspFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  else{
                      if(DM.ARAY[riRDF].GetCntStat(csMask)) {
                          if(RsltRat.iRet == 0 ) {
                              RAD.FindChip(riRDF,r,c) ;
                              DM.ARAY[riWRD].SetStat(r,c,csWork);
                              DM.ARAY[riRDF].SetStat(r,c,csWork);
                          }
                          else if(RsltRat.iRet == 1 ) {
                              EM_SetErr(eiHED_RalAtVsThchFail);
                              Step.iCycle = 0 ;
                              return true ;
                          }
                          else if(RsltRat.iRet == 2 ) {
                              RAD.FindChip(riRDF,r,c) ;
                              DM.ARAY[riWRD].SetStat(r,c,csRslt2);
                              DM.ARAY[riRDF].SetStat(r,c,csWork);
                          }
                      }
                  }

                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveMotr(miWRD_XHed,piWRD_XHedWafer)) return false ;
                  if(!MoveMotr(miWRD_YHed,piWRD_YHedWafer)) return false ;
                  if(!MoveMotr(miWRD_ZHed,piWRD_ZHedWfWorkWait)) return false ;
                  AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilWafer1) ;
                  MoveMotr(miWRD_ZHed,piWRD_ZHedWfWork);

                  Step.iCycle++;
                  return false ;

        case  16: //if(IO_GetX(xHED_Overload)) {
                  //    EM_SetErr(eiHED_VoiceCoilOverload);
                  //    Step.iCycle = 0;
                  //    return true ;
                  //}
                  if(!MoveMotr(miWRD_ZHed,piWRD_ZHedWfWork)) return false ;
                  IO_SetY(yHED_Vaccum,true);


                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  17: if(!m_tmTemp.OnDelay(true,OM.DevOptn.iVacuumOnDly)) return false ;
                  if(!IO_GetX(xHED_Vaccum) && !OM.CmnOptn.bIgnrVcSsr) {
                      EM_SetErr(eiHED_VacuumSsr);
                      IO_SetY(yHED_Eject ,false);
                      IO_SetY(yHED_Vaccum,false);
                      MoveMotr(miWRD_ZHed,piWRD_ZHedWait);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  //CallBakc 지정.
                  MT_SetIntrptPos(miWRD_ZEjt , PM.GetValue(miWRD_ZEjt,pvWRD_ZEjtWorkBefore) , CallAxisZStripPs );
                  MT_SetIntrptPos(miWRD_ZHed , PM.GetValue(miWRD_ZHed,pvWRD_ZHedWfWorkWait) , CallAxisYWaferPs );
                  MoveMotr(miWRD_ZEjt,piWRD_ZEjtWorkStt);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  18: //if(IO_GetX(xHED_Overload)) {
                  //    EM_SetErr(eiHED_VoiceCoilOverload);
                  //    Step.iCycle = 0;
                  //    return true ;
                  //}

                  if(!MoveMotr(miWRD_ZEjt,piWRD_ZEjtWorkStt)) return false ;
//                  MT_ResetIntrpt(miWRD_ZEjt);
                  MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait);

                  Step.iCycle++;
                  return false;

        case  19: if(!MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait       )) return false ;
//                  if(!MoveMotr(miWRD_ZHed,piWRD_ZHedStWorkWait )) return false ;

                  DM.ARAY[riHED].SetStat(0,0,csUnkwn);
//                  STG.FindChip(riSTG,r,c);
//                  DM.ARAY[riSTG].SetStat(r,c,csWork);
                  STG.SetLastPickPos() ;
                  if(OM.CmnOptn.bUseMapFile) DM.ARAY[riSTG].ChangeStat(csWork,csEmpty);
                  else                       DM.ARAY[riSTG].SetStat   (csUnkwn);


                  //TODO :: 보이스 코일. 이거 위치 조정 해야댐요.
//                  AI_SetY(VOICE_COIL_ID , 0);//OM.DevOptn.dVoiceCoilMove) ;

                  Step.iCycle = 0 ;
                  return true ;
    }
*/
}

bool CHead::CyclePlace(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        MT_ResetIntrpt(miWRD_YHed);
        MT_ResetIntrpt(miWRD_ZEjt);
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

    if(!InspectToolSafe()) {
        Step.iCycle = 0;
        return true;
    }

    int r,c ;
    bool bCheckStgPos = false ;

    static double dPlaceWait = 0.0 ;
    static int    iRalBfVisnFailCnt = 0 ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10:
                  IO_SetY(yHED_Eject ,false);
                  IO_SetY(yHED_Vaccum,true );

                  AI_SetY(VOICE_COIL_ID , 0.0);//OM.DevOptn.dVoiceCoilMove );
                  MoveMotr(miWRD_ZHed,piWRD_ZHedMove );

                  Step.iCycle++;
                  return false;

                  //아직 Y축은 가고 있는중.
        case  11: if(!MoveMotr(miWRD_ZHed,piWRD_ZHedMove )) return false ;
                  if(!MoveMotr(miWRD_ZHed,piWRD_ZHedMove))return false ;
                  //if(!MoveMotr(miWRD_THed,piWRD_THedStrip)) return false ;


                  if(!OM.MstOptn.bIgnrBfVision || !OM.MstOptn.bDryRun) {
                      MT_GoAbsRun(miWRD_XHed,GetMotrPos(miWRD_XHed,piWRD_XHedStrip  ) + RAD.GetXOffset()  );
                      MT_GoAbsRun(miWRD_YHed,GetMotrPos(miWRD_YHed,piWRD_YHedWork   ) + RAD.GetYOffset()  );
                      MT_GoAbsRun(miWRD_THed,GetMotrPos(miWRD_THed,piWRD_THedStrip  ) + (OM.CmnOptn.bUseHeadTRal ? RAD.GetTOffset() : 0.0 ) );
                      Trace("RADX",AnsiString(RAD.GetXOffset()).c_str());
                      Trace("RADY",AnsiString(RAD.GetYOffset()).c_str());
                      Trace("RADT",AnsiString(RAD.GetTOffset()).c_str());

                  }
                  else {
                      MT_GoAbsRun(miWRD_XHed,GetMotrPos(miWRD_XHed,piWRD_XHedStrip  )  );
                      MT_GoAbsRun(miWRD_YHed,GetMotrPos(miWRD_YHed,piWRD_YHedWork   )  );
                      MT_GoAbsRun(miWRD_THed,GetMotrPos(miWRD_THed,piWRD_THedStrip  )  );
                  }
                  Step.iCycle++;
                  return false ;


        case  12: //if(!MT_GetStopInpos(miWRD_XHed)) return false ;
                  //if(!MT_GetStopInpos(miWRD_YHed)) return false ;
                  //if(!MT_GetStopInpos(miWRD_THed)) return false ;

                  if(!OM.MstOptn.bIgnrBfVision || !OM.MstOptn.bDryRun) {
                      if(!MT_GoAbsRun(miWRD_XHed,GetMotrPos(miWRD_XHed,piWRD_XHedStrip  ) + RAD.GetXOffset()  ))return false ;
                      if(!MT_GoAbsRun(miWRD_YHed,GetMotrPos(miWRD_YHed,piWRD_YHedWork   ) + RAD.GetYOffset()  ))return false ;
                      if(!MT_GoAbsRun(miWRD_THed,GetMotrPos(miWRD_THed,piWRD_THedStrip  ) + (OM.CmnOptn.bUseHeadTRal ? RAD.GetTOffset() : 0.0 ) ))return false ;
                  }
                  else {
                      if(!MT_GoAbsRun(miWRD_XHed,GetMotrPos(miWRD_XHed,piWRD_XHedStrip  )  ))return false ;
                      if(!MT_GoAbsRun(miWRD_YHed,GetMotrPos(miWRD_YHed,piWRD_YHedWork   )  ))return false ;
                      if(!MT_GoAbsRun(miWRD_THed,GetMotrPos(miWRD_THed,piWRD_THedStrip  )  ))return false ;
                  }










                  if(OM.CmnOptn.bUseVcCoil) {
                      IO_SetY(yHED_VoiceCoilServo , true);
                  }
                  AI_SetY(VOICE_COIL_ID , GetVoiceCoilVolt(OM.DevOptn.dVoiceCoilStrip)) ;

                  MoveMotr(miWRD_ZHed,piWRD_ZHedStWorkWait);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miWRD_ZHed,piWRD_ZHedStWorkWait)) return false ;
                  AI_SetY(VOICE_COIL_ID , GetVoiceCoilVolt(OM.DevOptn.dVoiceCoilStrip1)) ;
                  MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedStWork), OM.DevOptn.dPlaceWaitVel,OM.DevOptn.dPlaceWaitAc,OM.DevOptn.dPlaceWaitDc);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedStWork), OM.DevOptn.dPlaceWaitVel,OM.DevOptn.dPlaceWaitAc,OM.DevOptn.dPlaceWaitDc))return false ;
                  //if(!MT_GetStopInpos(miWRD_ZHed)) return false ;
                  IO_SetY(yHED_Vaccum,false);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  15: if(!m_tmTemp.OnDelay(true,OM.DevOptn.iVacuumOffDly)) return false ;
                  IO_SetY(yHED_Eject,true);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  16: if(!m_tmTemp.OnDelay(true,OM.DevOptn.iEjectOnDly)) return false ;
                  IO_SetY(yHED_Eject,false);
                  MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedStWorkWait), OM.DevOptn.dPlaceWaitVel,OM.DevOptn.dPlaceWaitAc,OM.DevOptn.dPlaceWaitDc);
                  Step.iCycle++;
                  return false;

        case  17: //if(!MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedStWorkWait), OM.DevOptn.dPlaceWaitVel,OM.DevOptn.dPlaceWaitAc,OM.DevOptn.dPlaceWaitDc)) return false ;
                  if(!MT_GetStopInpos(miWRD_ZHed))return false ;
                  DM.ARAY[riHED].SetStat(csNone);
                  AI_SetY(VOICE_COIL_ID , GetVoiceCoilVolt(OM.DevOptn.dVoiceCoilStrip)) ;
                  MoveMotr(miWRD_ZHed,piWRD_ZHedMove);
                  Step.iCycle++;
                  return false;

        case  18: if(!MoveMotr(miWRD_ZHed,piWRD_ZHedMove)) return false ;
                  AI_SetY(VOICE_COIL_ID , 0.0);//OM.DevOptn.dVoiceCoilMove) ;
                  IO_SetY(yHED_VoiceCoilServo , false);

                  DM.ARAY[riHED].SetStat(csNone);  //여기서 뻑났을 확율 높음.
                  FindChip(riRDF,r,c);

                  if(!OM.MstOptn.bPickUpTestMode) {
                      if(OM.CmnOptn.bIgnrRalAtVs) {
                          DM.ARAY[riRDV].SetStat(r,c,csWork);
                          DM.ARAY[riRDF].SetStat(r,c,csWork);
                      }
                      else {
                          DM.ARAY[riRDF].SetStat(r,c,csWait2);
                      }
                  }
/*
                  if(OM.CmnOptn.bIgnrRalAtVs) {
                      DM.ARAY[riWRD].SetStat(r,c,csWork);
                      DM.ARAY[riRDF].SetStat(r,c,csWork);
                  }
                  else {
                      DM.ARAY[riRDF].SetStat(r,c,csWait2);
                  }
*/
                  MoveMotr(miWRD_YHed,piWRD_YHedWaferWait);

                  //이건 있으나 마나네....
                  if(!OM.CmnOptn.bIgnrVcSsr && !OM.MstOptn.bDryRun){
                      if( DM.ARAY[riHED].CheckStat(0,0,csNone ) &&  IO_GetX(xHED_Vaccum) ){
                          EM_SetErr(eiHED_Unknwn) ;
                      }
                  }





                  Step.iCycle = 0 ;
                  return true ;

    }
}

/*
bool CHead::CyclePlace(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        MT_ResetIntrpt(miWRD_YHed);
        MT_ResetIntrpt(miWRD_ZEjt);
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

    int r,c ;
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

        case  10:
                  IO_SetY(yHED_Eject ,false);
                  IO_SetY(yHED_Vaccum,true );

                  if(MT_CmprPos(miWRD_ZHed,PM.GetValue(miWRD_ZHed,piWRD_ZHedMove)) )AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilWafer );//OM.DevOptn.dVoiceCoilMove) ; //보이스 코일
                  else AI_SetY(VOICE_COIL_ID , 0 );//OM.DevOptn.dVoiceCoilMove) ; //보이스 코일

//                  MT_ResetIntrpt(miWRD_YHed);
//                  MT_ResetIntrpt(miWRD_ZEjt);

                  MoveMotr(miWRD_ZHed,piWRD_ZHedMove );
                  MoveMotr(miWRD_THed,piWRD_THedStrip);
                  //TODO ::
                  VS_Grab(viRbf);  //이거 그립 딜레이 _bInsYReady

                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miWRD_ZHed,piWRD_ZHedMove )) return false ;
                  if(!MoveMotr(miWRD_THed,piWRD_THedStrip)) return false ;

                  //보이스 코일.
                  AI_SetY(VOICE_COIL_ID , 0);//OM.DevOptn.dVoiceCoilMove) ;
                  if(!OM.MstOptn.bIgnrBfVision) {
                      VS_Grab(viRbf);  //이거 그립 딜레이 _bInsYReady
                      MoveMotr(miWRD_XHed,piWRD_XHedStrip);
                      MoveMotr(miWRD_YHed,piWRD_YHedStripWait);
//                      MoveMotr(miWRD_YHed,piWRD_YHedWork);


                      m_tmTemp.Clear() ;
                      Step.iCycle++;
                      return false;
                  }
                  else {
                      MoveMotr(miWRD_XHed,piWRD_XHedStrip);
                      MoveMotr(miWRD_YHed,piWRD_YHedStripWait);
//                      MoveMotr(miWRD_YHed,piWRD_YHedWork);
                      Step.iCycle = 14;
                      return false;
                  }

                  Step.iCycle++;
                  return false ;

        case  12: //if(PM.GetValue(miWRD_YHed,pvWRD_YHedStripWait) <= MT_GetCmdPos(miWRD_YHed)) { //pvWRD_YHedStripWait 이게 비전 찍을수 있는 위치 인가봅니다.


                  if(m_tmTemp.OnDelay(!VS_GetGrabEnd(viRbf) ,OM.DevOptn.iRalBfVsGrabTO )) {
                      EM_SetErr(eiHED_RalBfVsGrabFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  if(!VS_GetGrabEnd(viRbf) ) return false ;
                  VS_Inspect(viRbf,false); //쓰레드로 빼던가 나중에 ...
                  m_tmTemp.Clear() ;
                  Step.iCycle++;
                  return false ;

        case  13: if(m_tmTemp.OnDelay(!VS_GetInspEnd(viRbf) ,OM.DevOptn.iRalBfVsInspTO )) {
                      EM_SetErr(eiHED_RalBfVsInspFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  if(!VS_GetInspEnd(viRbf) ) return false ;
                  VS_GetRslt(viRbf,&RsltRbf);

                  if(!RsltRbf.bRslt){
                      EM_SetErr(eiHED_RalBfVsInspFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  //viRbf  = 0:Pass , 1:LCP Fail , 2: Epoxy Fail
                  else {
                      if(RsltRbf.iRet == 1 ) {
                          EM_SetErr(eiHED_RalBfVsThchFail);
                          Step.iCycle = 0 ;
                          return true ;
                      }
                      else if(RsltRbf.iRet == 2 ) {
                          if(!OM.CmnOptn.bIgnrRalBfVsEpoxyFail ) {
                              EM_SetErr(eiHED_RalBfVsThchFail);
                              Step.iCycle = 0 ;
                              return true ;
                          }
                          else {
                              FindChip(riRDF,r,c) ;
                              DM.ARAY[riWRD].SetStat(r,c,csRslt2);
                              DM.ARAY[riRDF].SetStat(r,c,csWork);

//                              MoveMotr(miWRD_XHed,piWRD_XHedStrip);
//                              MoveMotr(miWRD_YHed,piWRD_YHedStripWait);
                              Step.iCycle = 0 ;
                              return true ;
                          }
                      }
                  }
                  Step.iCycle++;
                  return false ;

        case  14:


                  if(!OM.MstOptn.bIgnrBfVision) {
                      MT_GoAbsRun(miWRD_XHed,GetMotrPos(miWRD_XHed,piWRD_XHedStrip  ) + RsltRbf.fRsltX  );
                      MT_GoAbsRun(miWRD_YHed,GetMotrPos(miWRD_YHed,piWRD_YHedWork   ) + RsltRbf.fRsltY  );
                      MT_GoAbsRun(miWRD_THed,GetMotrPos(miWRD_THed,piWRD_THedStrip  ) + RsltRbf.fRsltT  );
                  }
                  else {
                      MT_GoAbsRun(miWRD_XHed,GetMotrPos(miWRD_XHed,piWRD_XHedStrip  )  );
                      MT_GoAbsRun(miWRD_YHed,GetMotrPos(miWRD_YHed,piWRD_YHedWork   )  );
                      MT_GoAbsRun(miWRD_THed,GetMotrPos(miWRD_THed,piWRD_THedStrip  )  );
                  }
                  Step.iCycle++;
                  return false ;

        case  15: if(!MT_GetStopInpos(miWRD_XHed)) return false ;
                  if(!MT_GetStopInpos(miWRD_YHed)) return false ;
                  if(!MT_GetStopInpos(miWRD_THed)) return false ;

                  AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilStrip) ;

                  MoveMotr(miWRD_ZHed,piWRD_ZHedStWorkWait);
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miWRD_ZHed,piWRD_ZHedStWorkWait)) return false ;
                  AI_SetY(VOICE_COIL_ID , OM.DevOptn.dVoiceCoilStrip1) ;
                  MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedStWork), OM.DevOptn.dPlaceWaitVel,OM.DevOptn.dPlaceWaitAc,OM.DevOptn.dPlaceWaitDc);
                  Step.iCycle++;
                  return false ;

        case  17: //if(!MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedStWork), OM.DevOptn.dPlaceWaitVel,OM.DevOptn.dPlaceWaitAc,OM.DevOptn.dPlaceWaitDc)) return false ;
                  if(!MT_GetStopInpos(miWRD_ZHed)) return false ;
                  IO_SetY(yHED_Vaccum,false);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  18: if(!m_tmTemp.OnDelay(true,OM.DevOptn.iVacuumOffDly)) return false ;
                  if(OM.DevOptn.iEjectOnDly) {
                      IO_SetY(yHED_Eject,true);
                      m_tmTemp.Clear();
                  }
                  else {
                      Step.iCycle=19;
                      return false;
                  }
                  Step.iCycle++;
                  return false;

        case  19: if(!m_tmTemp.OnDelay(true,OM.DevOptn.iEjectOnDly)) return false ;
                  IO_SetY(yHED_Eject,false);
                  MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedStWorkWait), OM.DevOptn.dPlaceWaitVel,OM.DevOptn.dPlaceWaitAc,OM.DevOptn.dPlaceWaitDc);
                  Step.iCycle++;
                  return false;

        case  20: if(!MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedStWorkWait), OM.DevOptn.dPlaceWaitVel,OM.DevOptn.dPlaceWaitAc,OM.DevOptn.dPlaceWaitDc)) return false ;
//                  if(!MT_GetStopInpos(miWRD_ZHed)) return false ;
//                  MoveMotr(miWRD_XHed,piWRD_XHedStrip);
                  MoveMotr(miWRD_YHed,piWRD_YHedStripWait);
                  MoveMotr(miWRD_ZHed,piWRD_ZHedMove);

                  DM.ARAY[riHED].SetStat(0,0,csNone);
                  FindChip(riRDF,r,c);
                  DM.ARAY[riWRD].SetStat(r,c,csMask);
                  DM.ARAY[riRDF].SetStat(r,c,csMask);

                  //TODO :: 보이스 코일. 이거 위치 조정 해야댐요.
//                  AI_SetY(VOICE_COIL_ID , 0);//OM.DevOptn.dVoiceCoilMove) ;

                  Step.iCycle = 0 ;
                  return true ;
    }
}

*/

bool CHead::CyclePickCal(void)
{
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
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

    if(IO_GetX(xETC_ResetSw) || IO_GetX(xETC_StopSw)) {
        MT_EmgStop(miWRD_ZHed);

        MoveMotr(miWRD_ZHed , piWRD_ZHedMove);

        Step.iCycle = 0 ;
        return true ;

    }

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true;
    //}


    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: m_dPickCalPos = 0.0 ;

                  IO_SetY(yEJT_Suction , true);
                  STG.MoveMotr(miWRD_ZExd , piWRD_ZExdExpend);
                  MoveMotr(miWRD_ZEjt , piWRD_ZEjtWait  );
                  MoveMotr(miWRD_ZHed , piWRD_ZHedMove  );




                  Step.iCycle++;
                  return false ;

        case  11: if(!STG.MoveMotr(miWRD_ZExd , piWRD_ZExdExpend)) return false ;
                  if(!MoveMotr(miWRD_ZEjt , piWRD_ZEjtWait  )) return false ;
                  if(!MoveMotr(miWRD_ZHed , piWRD_ZHedMove  )) return false ;


                  MoveMotr(miWRD_XHed , piWRD_XHedWafer );
                  MoveMotr(miWRD_YHed , piWRD_YHedWafer );
                  MoveMotr(miWRD_THed , piWRD_THedWafer );

                  MoveActr(aiEJT_UpDn,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveActr(aiEJT_UpDn,ccFwd)) return false ;
                  if(!MoveMotr(miWRD_XHed , piWRD_XHedWafer )) return false ;
                  if(!MoveMotr(miWRD_YHed , piWRD_YHedWafer )) return false ;
                  if(!MoveMotr(miWRD_THed , piWRD_THedWafer )) return false ;

                  MoveMotr(miWRD_ZHed,piWRD_ZHedWfWorkWait);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miWRD_ZHed,piWRD_ZHedWfWorkWait)) return false ;

                  //이미 센서에 불이 들어왔다.
                  if(IO_GetX(xHED_Overload)) {
                      EM_SetErr(eiHED_VoiceCoilOverload);
                      MoveMotr(miWRD_ZHed , piWRD_ZHedMove);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //보이스 코일 쓰면 위치 정밀도가 안나옴.
                  //AI_SetY(VOICE_COIL_ID , GetVoiceCoilVolt(OM.DevOptn.dVoiceCoilWafer1)) ;
                  //IO_SetY(yHED_VoiceCoilServo , OM.CmnOptn.bUseVcCoil);

                  //일단 2.0만 더내리면 센서 미감지시 위험...
                  MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWfWork)+2.0, 0.2 ,2.0,2.0);
                  Step.iCycle++;
                  return false ;

        case  14: if(IO_GetX(xHED_Overload) && m_dPickCalPos == 0.0) {m_dPickCalPos = MT_GetCmdPos(miWRD_ZHed); MT_Stop(miWRD_ZHed);}
                  if(!MT_GetStop(miWRD_ZHed))return false ;
                  MoveMotr(miWRD_ZHed , piWRD_ZHedMove);
                  //IO_SetY(yHED_VoiceCoilServo , false);
                  IO_SetY(yEJT_Suction , false);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveMotr(miWRD_ZHed , piWRD_ZHedMove)) return false ;

                  MoveMotr(miWRD_YHed , piWRD_YHedWaferWait);
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miWRD_YHed , piWRD_YHedWaferWait)) return false ;
                  if(m_dPickCalPos != 0.0) m_bDispPickCal = true ;

                  Step.iCycle = 0 ;
                  return true ;

        }


}

bool CHead::CyclePlaceCal(void)
{
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
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

    if(IO_GetX(xETC_ResetSw) || IO_GetX(xETC_StopSw)) {
        MT_EmgStop(miWRD_ZHed);

        MoveMotr(miWRD_ZHed , piWRD_ZHedMove);

        Step.iCycle = 0 ;
        return true ;

    }

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: m_dPlaceCalPos = 0.0 ;
                  if(!IO_GetX(xHED_Vaccum)) {
                      EM_SetErr(eiHED_VacuumSsr);
                      Step.iCycle = 0 ;
                      return true ;

                  }

                  RAD.MoveActr(aiWRD_Clamp , ccFwd) ;

                  MoveMotr(miWRD_XHed , piWRD_XHedStrip    );
                  MoveMotr(miWRD_YHed , piWRD_YHedStripStt );
                  MoveMotr(miWRD_THed , piWRD_THedStrip    );

                  Step.iCycle++;
                  return false ;

        case  11: if(!RAD.MoveActr(aiWRD_Clamp , ccFwd)) return false ;
                  if(!MoveMotr(miWRD_XHed , piWRD_XHedStrip    )) return false ;
                  if(!MoveMotr(miWRD_YHed , piWRD_YHedStripStt )) return false ;
                  if(!MoveMotr(miWRD_THed , piWRD_THedStrip    )) return false ;


                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;









        case  12: if(!m_tmTemp.OnDelay(true , 500)) return false ;

                  //RAD.MoveActr(aiWRD_CAlign , ccFwd);

                  Step.iCycle++;
                  return false ;

        case  13: //if(!RAD.MoveActr(aiWRD_CAlign , ccFwd)) return false ;

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  14: if(!m_tmTemp.OnDelay(true , 500)) return false ;
                  //RAD.MoveActr(aiWRD_CAlign , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  15: //if(!RAD.MoveActr(aiWRD_CAlign , ccBwd)) return false ;



                  //이미 센서에 불이 들어왔다.
                  if(IO_GetX(xHED_Overload)) {
                      EM_SetErr(eiHED_VoiceCoilOverload);
                      MoveMotr(miWRD_ZHed , piWRD_ZHedMove);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  //보이스 코일 쓰면 위치 정밀도가 안나옴.
                  //AI_SetY(VOICE_COIL_ID , GetVoiceCoilVolt(OM.DevOptn.dVoiceCoilStrip1)) ;
                  //IO_SetY(yHED_VoiceCoilServo , OM.CmnOptn.bUseVcCoil);

                  //일단 2.0만 더내리면 센서 미감지시 위험...
                  MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedStWork)+2.0, 0.2 ,2.0,2.0);
                  Step.iCycle++;
                  return false ;

        case  16: if(IO_GetX(xHED_Overload) && m_dPlaceCalPos == 0.0) {m_dPlaceCalPos = MT_GetCmdPos(miWRD_ZHed); MT_Stop(miWRD_ZHed);}
                  if(!MT_GetStop(miWRD_ZHed))return false ;
                  MoveMotr(miWRD_ZHed , piWRD_ZHedMove);
                  //IO_SetY(yHED_VoiceCoilServo , false);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miWRD_ZHed , piWRD_ZHedMove)) return false ;

                  MoveMotr(miWRD_YHed , piWRD_YHedStripWait);
                  RAD.MoveActr(aiWRD_Clamp  , ccBwd) ;
                  //RAD.MoveActr(aiWRD_CAlign , ccBwd) ;



                  Step.iCycle++;
                  return false ;

        case  18: if(!MoveMotr(miWRD_YHed , piWRD_YHedStripWait)) return false ;
                  if(!RAD.MoveActr(aiWRD_Clamp  , ccBwd)) return false ;
                  //if(!RAD.MoveActr(aiWRD_CAlign , ccBwd)) return false ;

                  if(m_dPlaceCalPos != 0.0) m_bDispPlaceCal = true ;

                  Step.iCycle = 0 ;
                  return true ;

        }


}

bool CHead::CycleRalPlace(void)
{
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
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

    static double dSensorOnPos = PM.GetValue(miWRD_ZHed,pvWRD_ZHedStWorkWait) ;

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: m_dPlaceCalPos = 0.0 ;
                  if(!IO_GetX(xHED_Vaccum)) {
                      EM_SetErr(eiHED_VacuumSsr);
                      Step.iCycle = 0 ;
                      return true ;

                  }


                  MoveMotr(miWRD_XHed , piWRD_XHedStrip    );
                  MoveMotr(miWRD_YHed , piWRD_YHedStripStt );
                  MoveMotr(miWRD_THed , piWRD_THedStrip    );

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveMotr(miWRD_XHed , piWRD_XHedStrip    )) return false ;
                  if(!MoveMotr(miWRD_YHed , piWRD_YHedStripStt )) return false ;
                  if(!MoveMotr(miWRD_THed , piWRD_THedStrip    )) return false ;


                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  12: Step.iCycle++;
                  return false ;

        case  13: m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  14: //if(!m_tmTemp.OnDelay(true , 10)) return false ;
                  //RAD.MoveActr(aiWRD_CAlign , ccBwd);
                  //이미 센서에 불이 들어왔다.
                  if(IO_GetX(xHED_Overload)) {
                      EM_SetErr(eiHED_VoiceCoilOverload);
                      MoveMotr(miWRD_ZHed , piWRD_ZHedMove);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  MT_GoAbsMan(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedStWorkWait)-2.0);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MT_GetStop(miWRD_ZHed))return false ;


                  //보이스 코일 쓰면 위치 정밀도가 안나옴.
                  //AI_SetY(VOICE_COIL_ID , GetVoiceCoilVolt(OM.DevOptn.dVoiceCoilStrip1)) ;
                  //IO_SetY(yHED_VoiceCoilServo , OM.CmnOptn.bUseVcCoil);

                  //일단 2.0만 더내리면 센서 미감지시 위험...
                  MT_GoIncAcDc(miWRD_ZHed, 6.0, 2.0 ,20.0,20.0);
                  Step.iCycle++;
                  return false ;

        case  16: if(IO_GetX(xHED_Overload)) {dSensorOnPos = MT_GetCmdPos(miWRD_ZHed);  MT_Stop(miWRD_ZHed);}
                  if(!MT_GetStop(miWRD_ZHed))return false ;

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  17: if(!m_tmTemp.OnDelay(true , 300)) return false ;
                  IO_SetY(yWRD_Vaccum , true);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  18: if(!m_tmTemp.OnDelay(true , 300)) return false ;
                  IO_SetY(yHED_Vaccum , false);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  19: if(!m_tmTemp.OnDelay(true , 300)) return false ;
                  MT_GoAbsMan(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWait));

                  Step.iCycle++;
                  return false ;

        case  20: if(!MT_GetStop(miWRD_ZHed))return false ;
                  MoveMotr(miWRD_YHed , piWRD_YHedStripWait , true);
                  Step.iCycle++;
                  return false ;

        case  21: if(!MoveMotr(miWRD_YHed , piWRD_YHedStripWait , true)) return false ;
                  SV_Grab(viRat);
                  m_tmTemp.Clear() ;
                  Step.iCycle++;
                  return false ;

        case  22: if(m_tmTemp.OnDelay(!SV_GetGrabEnd(viRat),OM.DevOptn.iRalAtVsGrabTO )) {
                      EM_SetErr(eiHED_RatVsGrabFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  if(!SV_GetGrabEnd(viRat)) return false ;
                  Step.iCycle++;
                  return false ;

                  MoveMotr(miWRD_YHed , piWRD_YHedStripStt );

                  Step.iCycle++;
                  return false ;

        case  23: if(!MoveMotr(miWRD_YHed , piWRD_YHedStripStt )) return false ;
                  MT_GoAbsMan(miWRD_ZHed,dSensorOnPos-2.0);
                  Step.iCycle++;
                  return false ;

        case  24: if(!MT_GetStop(miWRD_ZHed))return false ;


                  //보이스 코일 쓰면 위치 정밀도가 안나옴.
                  //AI_SetY(VOICE_COIL_ID , GetVoiceCoilVolt(OM.DevOptn.dVoiceCoilStrip1)) ;
                  //IO_SetY(yHED_VoiceCoilServo , OM.CmnOptn.bUseVcCoil);

                  //일단 2.0만 더내리면 센서 미감지시 위험...
                  MT_GoIncAcDc(miWRD_ZHed,6.0, 2.0 ,20.0,20.0);
                  Step.iCycle++;
                  return false ;

        case  25: if(IO_GetX(xHED_Overload)) {MT_Stop(miWRD_ZHed);}
                  if(!MT_GetStop(miWRD_ZHed))return false ;

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  26: if(!m_tmTemp.OnDelay(true , 300)) return false ;
                  IO_SetY(yHED_Vaccum , true);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  27: if(!m_tmTemp.OnDelay(true , 300)) return false ;
                  IO_SetY(yWRD_Vaccum , false);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  28: if(!m_tmTemp.OnDelay(true , 300)) return false ;
                  MT_GoAbsMan(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWait));

                  Step.iCycle++;
                  return false ;

        case  29: if(!MT_GetStop(miWRD_ZHed))return false ;
                  MoveMotr(miWRD_YHed , piWRD_YHedStripWait , true);
                  Step.iCycle++;
                  return false ;

        case  30: if(!MoveMotr(miWRD_YHed , piWRD_YHedStripWait , true)) return false ;
                  Step.iCycle = 0 ;
                  return true ;

        }


}




bool CHead::CycleWfrPlace(void) //Manual
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
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

    int r,c ;

    static int    iInspStep = 0 ;
    const  double dInspGap  = 2 ;
    const  int    iInspCnt  = 9 ; //3X3 = 9      ''
    double dMoveX , dMoveY , dMoveT ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(yEJT_Suction , false);
                  STG.MoveMotr(miWRD_ZExd , piWRD_ZExdExpend);
                  MoveMotr(miWRD_ZEjt , piWRD_ZEjtWait  );
                  MoveMotr(miWRD_ZHed , piWRD_ZHedMove  );




                  Step.iCycle++;
                  return false ;

        case  11: if(!STG.MoveMotr(miWRD_ZExd , piWRD_ZExdExpend)) return false ;
                  if(!MoveMotr(miWRD_ZEjt , piWRD_ZEjtWait  )) return false ;
                  if(!MoveMotr(miWRD_ZHed , piWRD_ZHedMove  )) return false ;


                  MoveMotr(miWRD_XHed , piWRD_XHedWafer );
                  MoveMotr(miWRD_YHed , piWRD_YHedWafer );
                  MoveMotr(miWRD_THed , piWRD_THedWafer );

                  MoveActr(aiEJT_UpDn,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveActr(aiEJT_UpDn,ccFwd)) return false ;
                  if(!MoveMotr(miWRD_XHed , piWRD_XHedWafer )) return false ;
                  if(!MoveMotr(miWRD_YHed , piWRD_YHedWafer )) return false ;
                  if(!MoveMotr(miWRD_THed , piWRD_THedWafer )) return false ;




         //  원래꺼.
                  MoveMotr(miWRD_ZHed,piWRD_ZHedWfWorkWait);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miWRD_ZHed,piWRD_ZHedWfWorkWait)) return false ;

                  AI_SetY(VOICE_COIL_ID , GetVoiceCoilVolt(OM.DevOptn.dVoiceCoilStrip1)) ;
                  IO_SetY(yHED_VoiceCoilServo , OM.CmnOptn.bUseVcCoil);

                  MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWfWork), OM.DevOptn.dPlaceWaitVel,OM.DevOptn.dPlaceWaitAc,OM.DevOptn.dPlaceWaitDc);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MT_GoAbsAcDc(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWfWork), OM.DevOptn.dPlaceWaitVel,OM.DevOptn.dPlaceWaitAc,OM.DevOptn.dPlaceWaitDc))return false ;
                  MoveMotr(miWRD_ZHed , piWRD_ZHedWfWork);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveMotr(miWRD_ZHed , piWRD_ZHedWfWork)) return false ;
                  IO_SetY(yHED_Vaccum , false);
                  IO_SetY(yEJT_Suction , true);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  16: if(!m_tmTemp.OnDelay(true , 100 )) return false ;

                  IO_SetY(yHED_Eject , true);

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  17: if(!m_tmTemp.OnDelay(true , 300 )) return false ;



                  //IO_SetY(yEJT_Suction , true);

                  m_tmTemp.Clear();



                  Step.iCycle++;
                  return false ;

        case  18: //if(!m_tmTemp.OnDelay(true , 500 )) return false ;
                  //칩에 찍힘 나는 것 확인 위해.... 이렇게 하면 찍은 범인은 혹시 칩위에 있을까봐
                  MT_GoAbsAcDc(miWRD_ZHed , PM.GetValue(miWRD_ZHed,pvWRD_ZHedWfWorkWait)-1.5 ,0.5 , 2.0 , 2.0);

                  Step.iCycle++;
                  return false ;

        case  19: if(!MT_GetStopInpos(miWRD_ZHed)) return false ;
                  IO_SetY(yHED_Eject , false);


                  MoveMotr(miWRD_ZHed , piWRD_ZHedMove);


                  Step.iCycle++;
                  return false ;

        case  20: if(!MoveMotr(miWRD_ZHed , piWRD_ZHedMove)) return false ;
                  IO_SetY(yEJT_Suction , false); 
                  IO_SetY(yHED_VoiceCoilServo , false);
                  MoveMotr(miWRD_YHed , piWRD_YHedWaferWait);
                  Step.iCycle++;
                  return false ;

        case  21: if(!MoveMotr(miWRD_YHed , piWRD_YHedWaferWait)) return false ;
                  SV_Grab(viWfr);
                  m_tmTemp.Clear() ;
                  Step.iCycle++;
                  return false ;

        case  22: if(m_tmTemp.OnDelay(!SV_GetGrabEnd(viWfr),OM.DevOptn.iWfrVsGrabTO )) {
                      EM_SetErr(eiSTG_WfrVsGrabFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  if(!SV_GetGrabEnd(viWfr)) return false ;

                  DM.ARAY[riHED].SetStat(csNone);

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
    if(!MT_GetStop(miWRD_THed)) return false ;
    if(!MT_GetStop(miWRD_ZEjt)) return false ;

    bool bTest1 = MT_GetStop(miWRD_XHed) ;
    bool bTest2 = MT_GetStop(miWRD_YHed) ;
    bool bTest3 = MT_GetStop(miWRD_ZHed) ;
    bool bTest4 = MT_GetStop(miWRD_THed) ;
    bool bTest5 = MT_GetStop(miWRD_ZEjt) ;


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
