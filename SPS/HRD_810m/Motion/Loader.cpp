
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Loader.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "LotUnit.h"
#include "Epoxy.h"
//---------------------------------------------------------------------------
#include "PreBuff.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone�����.
//---------------------------------------------------------------------------
CLoader LDR;

CLoader::CLoader()
{
    Init();

//    AT_MoveCyl(aiLDR_Stopper,ccBwd);
    AT_MoveCyl(aiLDR_MgzGrip,ccBwd);
    AT_MoveCyl(aiLDR_Out    ,ccBwd);
    AT_MoveCyl(aiLDR_Pusher ,ccBwd);

}
CLoader::~CLoader (void)
{
    Close();
}
void CLoader::Init()
{
    m_sPartName = "Loader " ;
    m_iMgzCnt   = 0 ;

    m_iHomeTO  = (int)eiLDR_HomeTO    ;
    m_iStartTO = (int)eiLDR_ToStartTO ;
    m_iStopTO  = (int)eiLDR_ToStopTO  ;
    m_iCycleTO = (int)eiLDR_CycleTO   ;

    Reset();
//    AT_MoveCyl(aiLDR_Sptr , ccFwd);
    Load(true);
}
void CLoader::Close()
{
    Load(false);
}
void CLoader::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CLoader::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();

}

bool CLoader::FindChip(EN_ARAY_ID _riWhre , int &r , int &c )
{
    if(_riWhre == riLDR) {
        if(OM.DevOptn.iLdrOrder) r = DM.ARAY[riLDR].FindFrstRow(csUnkwn) ;
        else                     r = DM.ARAY[riLDR].FindLastRow(csUnkwn) ;
        return (r > -1 && r > -1);
    }

    return false ;
}

double CLoader::GetWorkPos(EN_MOTR_ID _iMotr)
{
    double dWorkPos = MT_GetCmdPos(_iMotr);
    int r  , c  ;

    if(_iMotr == miLDR_ZCmp){
        if(FindChip(riLDR,r,c)) {
//            dWorkPos = PM.GetValue(_iMotr , pvLDR_ZCmpWorkStt) - (OM.DevInfo.iMgzSlotCnt-r-1) * OM.DevInfo.dMgzSlotPitch ; //Ȩ���� ��ġ�� ���� �ٲ��־ ���� �ٲ�.
            if(OM.DevOptn.iLdrOrder) dWorkPos = PM.GetValue(_iMotr , pvLDR_ZCmpWorkStt) - (r) * OM.DevInfo.dMgzSlotPitch ;
            else                     dWorkPos = PM.GetValue(_iMotr , pvLDR_ZCmpWorkStt) + (OM.DevInfo.iMgzSlotCnt-r-1) * OM.DevInfo.dMgzSlotPitch ;
        }
    }

    return dWorkPos ;
}

double CLoader::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    int r = -1 ;
    int c = -1 ;

    if(_iMotr == miLDR_YCmp){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piLDR_YCmpWait    : dPos = PM.GetValue (_iMotr , pvLDR_YCmpWait    ); break ;
//            case piLDR_YCmpIn      : dPos = PM.GetValue (_iMotr , pvLDR_YCmpIn      ); break ;
            case piLDR_YCmpWorkStt : dPos = PM.GetValue (_iMotr , pvLDR_YCmpWorkStt ); break ;
        }
    }
    else if(_iMotr == miLDR_ZCmp){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piLDR_ZCmpWait    : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpWait    ); break ;
            case piLDR_ZCmpIn      : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpIn      ); break ;
            case piLDR_ZCmpWorkStt : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpWorkStt ); break ;
            case piLDR_ZCmpOut     : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpOut     ); break ;
            case piLDR_ZCmpWork    : dPos = GetWorkPos  (_iMotr                     ); break ;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CLoader::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg = "";

    bool isFwStopper   = !AT_Complete(aiLDR_Stopper ,ccBwd);
    bool isFwPusher    = !AT_Complete(aiLDR_Pusher  ,ccBwd);
    bool isFwMgzGrip   = !AT_Complete(aiLDR_MgzGrip ,ccBwd);
    bool isOutCylFw    = !AT_Complete(aiLDR_Out     ,ccBwd);
    bool isMgzDetected =  IO_GetX(xLDR_InDetect); //|| IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2);
    bool isPRB_InSsr   =  IO_GetX(xPRB_InDetect);


    if(_iMotr == miLDR_YCmp){
        switch(_iPstnId) {
            default                    : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
                case piLDR_YCmpWait    : break ;
//                case piLDR_YCmpIn         : if(isFwStopper   ) { sMsg = "Out Cylinder is Fwd" ; bRet = false ;} break ;
                case piLDR_YCmpWorkStt : if(isFwStopper   ) { sMsg = "LDR Stopper Cylinder is Fwd" ; bRet = false ;} break ;
//                                         if(isMgzDetected ) { sMsg = "Mgz is Detected"             ; bRet = false ;} break ;
        }
    }
    else if(_iMotr == miLDR_ZCmp){
             if(isFwPusher  ) { sMsg = "Pusher Cylinder is not Bwd"           ; bRet = false ;}
        else if(isPRB_InSsr ) { sMsg = "Strip is feeding now(InSsr Detected)" ; bRet = false ;}
        else if(isFwMgzGrip ) { sMsg = "Mgz Grip Cylinder is not Bwd"         ; bRet = false ;}
        switch(_iPstnId) {
            default                    : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
                case piLDR_ZCmpWait    : break ;
                case piLDR_ZCmpIn      : break ;
                case piLDR_ZCmpWorkStt : break ;
                case piLDR_ZCmpOut     : if(isOutCylFw )  { sMsg = "Out Cylinder is Fwd" ; bRet = false ;} break ;
                case piLDR_ZCmpWork    : if(isOutCylFw )  { sMsg = "Out Cylinder is Fwd" ; bRet = false ;} break ;
        }
    }
    else {
        sMsg = MT_GetName(_iMotr) + " is Not this part Motr!" ;
        bRet = false ;
    }

    static AnsiString sPreMsg = sMsg ;
    if(!bRet){
        if(sPreMsg != sMsg) {
            Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
            if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
        }
        sPreMsg = sMsg ;
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CLoader::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

//    bool isMgzDetected =  //IO_GetX(xLDR_InDetect) ||
//                          IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2);
    bool isZMoving     = !MT_GetStopInpos(miLDR_ZCmp);
    bool isYMoving     = !MT_GetStopInpos(miLDR_YCmp);

    if(_iActr == aiLDR_Stopper){
        if(_bFwd && isYMoving) {sMsg = "LDR_Y_Axis is Moving" ; bRet = false ;}
    }
    else if(_iActr == aiLDR_MgzGrip){
        if(_bFwd && isZMoving) {sMsg = "LDR_Z_Axis is Moving" ; bRet = false ;}
    }
    else if(_iActr == aiLDR_Out    ){
    //    if(_bFwd && IO_GetX(xLDR_FullDetect)) {sMsg = "LDR Mgz is Full" ; bRet = false ;} //���� ��ġ�� ����.
    }
    else if(_iActr == aiLDR_Pusher ){
        if(isZMoving) {sMsg = "LDR_Z is Moving" ; bRet = false ;}
    }

    else {
        sMsg = AT_GetName(_iActr) + " is Not this part Cylinder!" ;
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}

bool CLoader::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlowSpeed) // ���͸� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) {
        if(_bSlowSpeed)return MT_GoAbsSlow(_iMotr , dPosition);
        else           return MT_GoAbsRun (_iMotr , dPosition);
    }
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CLoader::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CLoader::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
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

        case 10: MT_SetServo(miLDR_ZCmp,true);
                 MT_SetServo(miLDR_YCmp,true);

                 MT_SetHomeEnd(miLDR_YCmp,false);
                 MT_SetHomeEnd(miLDR_ZCmp,false);
                 AT_MoveCyl(aiLDR_MgzGrip, ccBwd);
                 AT_MoveCyl(aiLDR_Out    , ccBwd);
                 AT_MoveCyl(aiLDR_Pusher , ccBwd);
//                 AT_MoveCyl(aiLDR_Stopper, ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiLDR_MgzGrip, ccBwd))return false ;
                 if(!AT_MoveCyl(aiLDR_Out    , ccBwd))return false ;
                 if(!AT_MoveCyl(aiLDR_Pusher , ccBwd))return false ;
                 //if(!AT_MoveCyl(aiLDR_Stopper, ccBwd))return false ;

                 MT_DoHome(miLDR_ZCmp);
                 MT_DoHome(miLDR_YCmp);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miLDR_ZCmp))return false ;
                 if(!MT_GetHomeEnd(miLDR_YCmp))return false ;
                 MT_GoAbsMan(miLDR_ZCmp , PM.GetValue(miLDR_ZCmp , pvLDR_ZCmpWait  ));
                 MT_GoAbsMan(miLDR_YCmp , PM.GetValue(miLDR_YCmp , pvLDR_YCmpWait  ));
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miLDR_ZCmp , PM.GetValue(miLDR_ZCmp , pvLDR_ZCmpWait  )))return false ;
                 if(!MT_GoAbsMan(miLDR_YCmp , PM.GetValue(miLDR_YCmp , pvLDR_YCmpWait  )))return false ;
//                 AT_MoveCyl(aiLDR_Stopper, ccBwd);
//                 Step.iHome++;
//                 return false ;
//
//        case 14: if(!AT_MoveCyl(aiLDR_Stopper, ccBwd)) return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CLoader::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CLoader::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CLoader::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)m_iStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: if(Step.iToStart) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStart = 0 ;
                 return true ;

        case 10: MoveActr(aiLDR_Out,ccBwd);
                 MoveActr(aiLDR_Pusher , ccBwd         );
                 MoveMotr(miLDR_YCmp   , piLDR_YCmpWait);
                 MoveActr(aiLDR_Stopper, ccFwd         );
                 if(IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2)) MoveActr(aiLDR_MgzGrip,ccFwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiLDR_Out,ccBwd)) return false ;
                 if(!MoveActr(aiLDR_Pusher , ccBwd         )) return false;
                 if(!MoveMotr(miLDR_YCmp   , piLDR_YCmpWait)) return false;
                 if(!MoveActr(aiLDR_Stopper, ccFwd         )) return false;
                 if(IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2)) if(!MoveActr(aiLDR_MgzGrip,ccFwd)) return false ;
//                 if(IO_GetX(xPRB_InDetect) && DM.ARAY[riPRB].CheckAllStat(csNone) ) EM_SetErr(eiPRB_Unknwn);

                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CLoader::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
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

    //Move Home.
    switch (Step.iToStop) {
        default: if(Step.iToStop) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStop=%02d" , __FUNC__ , Step.iToStop );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStop = 0;
                 return true ;

        case 10: //m_tmTemp.Clear();
                 MoveActr(aiLDR_Out,ccBwd);
                 MoveActr(aiLDR_Stopper, ccBwd         );
                 Step.iToStop++;
                 return false ;

        case 11: //if(!m_tmTemp.OnDelay(!MT_GetHomeEnd(miLDR_YCmp) , 300)) return false ; //�׳��� �̷��� ��ٷ� �ش�.
                 if(!MoveActr(aiLDR_Out,ccBwd)) return false ;
                 if(!MoveActr(aiLDR_Stopper, ccBwd         )) return false ;
                 if(MT_GetHomeEnd(miLDR_YCmp)) MoveMotr(miLDR_YCmp,piLDR_YCmpWait);

                 //MoveActr(aiLDR_Stopper,ccBwd);
                 MoveActr(aiLDR_Pusher ,ccBwd);
                 Step.iToStop ++;
                 return false ;

        case 12: if(MT_GetHomeEnd(miLDR_YCmp)) {
                     if(!MoveMotr(miLDR_YCmp,piLDR_YCmpWait)) return false ;
                 }
                 //if(!MoveActr(aiLDR_Stopper,ccBwd)) return false ;
                 if(!MoveActr(aiLDR_Pusher ,ccBwd)) return false ;
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CLoader::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool isMgzDetected  =  IO_GetX(xLDR_InDetect) || IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2);
        bool isZWorkPos     =  MT_CmprPos(miLDR_ZCmp ,GetMotrPos(miLDR_ZCmp , piLDR_ZCmpWork)) ;
//        bool isLotEndMgzCnt =  OM.DevOptn.iLotEndMgzCnt == 1 && OM.DevOptn.iLotEndMgzCnt == m_iMgzCnt ;

        bool isCycleSupply  =  false ;
        bool isCyclePick    =  DM.ARAY[riLDR].CheckAllStat(csNone ) && !isMgzDetected &&
                             ((OM.CmnOptn.iLotEndChipCnt && EPX.GetEpxLotCnt() < OM.CmnOptn.iLotEndChipCnt) || !OM.CmnOptn.iLotEndChipCnt)&&// || GetLotNo()!="") &&
                              !OM.CmnOptn.bLoadingStop ;

        bool isCycleWork    =  DM.ARAY[riLDR].GetCntStat  (csUnkwn) && !isZWorkPos &&
                               DM.ARAY[riPRB].CheckAllStat(csNone ) &&
                              !IO_GetX(xPRB_InDetect) && //�� �̳� Prb ...
                              !OM.CmnOptn.bLoadingStop ;

        bool isCyclePush    =  DM.ARAY[riLDR].GetCntStat  (csUnkwn) &&  isZWorkPos &&
                               DM.ARAY[riPRB].CheckAllStat(csNone ) && //!IO_GetX(xPRB_InDetect) && //�� �̳� Prb ...
                              !OM.CmnOptn.bLoadingStop ;


        bool isCyclePlace   =  DM.ARAY[riLDR].CheckAllStat(csEmpty) && DM.ARAY[riPRB].CheckAllStat(csNone ) ;
        bool isConEnd       =  DM.ARAY[riLDR].CheckAllStat(csNone ) || (OM.CmnOptn.iLotEndChipCnt && EPX.GetEpxLotCnt() >= OM.CmnOptn.iLotEndChipCnt) ;

        if(  DM.ARAY[riLDR].CheckAllStat(csNone) &&  isMgzDetected )  EM_SetErr(eiLDR_Unknwn) ;
        if( !DM.ARAY[riLDR].CheckAllStat(csNone) && !isMgzDetected && AT_Complete(aiLDR_MgzGrip,ccFwd) && !OM.MstOptn.bDryRun)  EM_SetErr(eiLDR_Dispr  ) ;

//      if(IO_GetX(xLDR_FullDetect)) EM_SetErr(eiLDR_FullMgz);

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleSupply ) {Trace(m_sPartName.c_str(),"CycleSupply  Stt"); Step.iSeq = scSupply ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePick   ) {Trace(m_sPartName.c_str(),"CyclePick    Stt"); Step.iSeq = scPick   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork   ) {Trace(m_sPartName.c_str(),"CycleWork    Stt"); Step.iSeq = scWork   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePush   ) {Trace(m_sPartName.c_str(),"CyclePush    Stt"); Step.iSeq = scPush   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePlace  ) {Trace(m_sPartName.c_str(),"CyclePlace   Stt"); Step.iSeq = scPlace  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd      ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default        :                     Trace(m_sPartName.c_str(),"default     End");Step.iSeq = scIdle ;  return false ;
        case scIdle    :                                                                                        return false ;
        case scSupply  : if(CycleSupply ()){ Trace(m_sPartName.c_str(),"CycleSupply End");Step.iSeq = scIdle ;} return false ;
        case scPick    : if(CyclePick   ()){ Trace(m_sPartName.c_str(),"CyclePick   End");Step.iSeq = scIdle ;} return false ;
        case scWork    : if(CycleWork   ()){ Trace(m_sPartName.c_str(),"CycleWork   End");Step.iSeq = scIdle ;} return false ;
        case scPush    : if(CyclePush   ()){ Trace(m_sPartName.c_str(),"CyclePush   End");Step.iSeq = scIdle ;} return false ;
        case scPlace   : if(CyclePlace  ()){ Trace(m_sPartName.c_str(),"CyclePlace  End");Step.iSeq = scIdle ;} return false ;
    }

}

//One Cycle.
bool CLoader::CycleSupply(void)
{
    return false ;
}
bool CLoader::CyclePick(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //�̷��� ������ �ٲܼ� �ְ� �ؼ� ����Ŭ �̸����� �־��ָ� ���ٳ�...
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
        Step.iCycle = 0;
        return true;
    }






    int       r1,r2 ;
    String    sLotNo , sDate;
    TDateTime tTemp ; //����.

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiLDR_MgzGrip, ccBwd);
                  MoveActr(aiLDR_Pusher , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiLDR_MgzGrip, ccBwd)) return false ;
                  if(!MoveActr(aiLDR_Pusher , ccBwd)) return false ;
                  MoveMotr(miLDR_ZCmp , piLDR_ZCmpIn  );
                  MoveMotr(miLDR_YCmp , piLDR_YCmpWait);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLDR_ZCmp  ,piLDR_ZCmpIn  )) return false ;
                  if(!MoveMotr(miLDR_YCmp , piLDR_YCmpWait)) return false ;
                  MoveActr(aiLDR_Stopper, ccBwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiLDR_Stopper, ccBwd)) return false ;
                  MoveMotr(miLDR_YCmp , piLDR_YCmpWorkStt);
                  Step.iCycle++;
                  return false ;

        case  14:      if(IO_GetX(xLDR_InDetect)                   ) MT_EmgStop(miLDR_YCmp); //IO_On Delay �� �ϵ��� �ϼ���...�ɼ��� ��� �ϰٽ��ϴ�.
                  else if(!MoveMotr(miLDR_YCmp , piLDR_YCmpWorkStt)) return false ;

                  if(!IO_GetX(xLDR_InDetect) && !OM.MstOptn.bDryRun) {
                      MoveMotr(miLDR_YCmp , piLDR_YCmpWait);
                      EM_SetErr(eiLDR_NeedMgz);
                      Step.iCycle = 0;
                      return true ;
                  }

                  DM.ARAY[riLDR].SetStat(csUnkwn); //Data Masking

//                  m_iMgzCnt++;
/*
                  if(m_iMgzCnt >= OM.DevOptn.iLotEndMgzCnt || m_iMgzCnt==0 ) {
                      sLotNo = PopLotNo() ;
                      LT.LotOpen(sLotNo , OM.m_sCrntDev );

                      tTemp.Val = LT.LotInfo.dStartTime ;
                      sDate = tTemp.FormatString("hh_nn_ss") ;

                      LT.AddDayInfoLotCnt(1);
                      m_iMgzCnt = 1;

                      DM.ARAY[riLDR].SetLotNo(sLotNo) ;
                      DM.ARAY[riLDR].SetID(m_iMgzCnt*100) ;
                  }
                  else {
                      tTemp.Val = LT.LotInfo.dStartTime ;
                      sDate = tTemp.FormatString("hh_nn_ss") ;
                      m_iMgzCnt++;
//                      sLotNo = "(" + sDate + ")" + LT.LotInfo.sLotNo ;
                      sLotNo = GetLotNo() ;
                      DM.ARAY[riLDR].SetLotNo(sLotNo) ;
                      DM.ARAY[riLDR].SetID(m_iMgzCnt*100) ;
                  }
*/
                  MoveActr(aiLDR_Stopper, ccFwd);
                  MoveMotr(miLDR_YCmp , piLDR_YCmpWait   ); //Ȯ�� 17
                  Step.iCycle++;
                  return false ;

        case  15: if(MT_GetHomeSnsr(miLDR_YCmp)) MT_EmgStop(miLDR_YCmp);
                  if(!MoveActr(aiLDR_Stopper, ccFwd)) return false ;
//                  MoveMotr(miLDR_YCmp , piLDR_YCmpWait   ); //Ȯ�� 17
//                  MT_SetHomeEnd(miLDR_YCmp,false);

//                  MT_DoHome(miLDR_YCmp); //Ȩ���ǵ��� ������ �ϴ���.
                  MoveMotr(miLDR_ZCmp , piLDR_ZCmpWorkStt);
                  Step.iCycle++;
                  return false ;

        case  16: if(MT_GetHomeSnsr(miLDR_YCmp)) MT_EmgStop(miLDR_YCmp);
                  if(!MoveMotr(miLDR_ZCmp , piLDR_ZCmpWorkStt)) return false ;
                  MoveActr(aiLDR_MgzGrip,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(MT_GetHomeSnsr(miLDR_YCmp)) MT_EmgStop(miLDR_YCmp);
                  if(!MoveActr(aiLDR_MgzGrip, ccFwd         )) return false ;
//                  if(!MoveMotr(miLDR_YCmp   , piLDR_YCmpWait)) return false ;
                  if(!MT_GetStopInpos(miLDR_YCmp)) return false ;
                  MT_DoHome(miLDR_YCmp); //Ȩ���ǵ��� ������ �ϴ���.
                  Step.iCycle++;
                  return false ;

        case  18: //if(!MT_GetHomeEnd(miLDR_YCmp)) return false ;
                  if(!OM.MstOptn.bDryRun) {
                      if(!IO_GetX(xLDR_MgzDetect1) || !IO_GetX(xLDR_MgzDetect2)) EM_SetErr(eiLDR_PickFail);
                  }

                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CLoader::CycleWork(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //�̷��� ������ �ٲܼ� �ְ� �ؼ� ����Ŭ �̸����� �־��ָ� ���ٳ�...
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

    int  r,c ;
    int  iSltCnt ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //MT_SetServo(miLDR_ZCmp,true);
                  //MT_SetServo(miLDR_YCmp,true);

                  MoveActr(aiLDR_Pusher  , ccBwd);
                  MoveActr(aiLDR_MgzGrip , ccBwd);
//                  PRB.MoveActr(aiPRB_Stopper , ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiLDR_Pusher  , ccBwd)) return false ;
                  if(!MoveActr(aiLDR_MgzGrip , ccBwd)) return false ;
//                  if(!PRB.MoveActr(aiPRB_Stopper , ccFwd)) return false ;                  
                  MoveMotr(miLDR_ZCmp , piLDR_ZCmpWork );
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLDR_ZCmp  ,piLDR_ZCmpWork )) return false ;
                  MoveActr(aiLDR_MgzGrip , ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiLDR_MgzGrip , ccFwd)) return false ;

                  Step.iCycle = 0 ;
                  return true ;
        }
}

bool CLoader::CyclePush(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //�̷��� ������ �ٲܼ� �ְ� �ؼ� ����Ŭ �̸����� �־��ָ� ���ٳ�...
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

    int  r,c ;
    int  iSltCnt ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiLDR_Pusher  , ccBwd);
                  MoveActr(aiLDR_MgzGrip , ccFwd);
                  PRB.MoveActr(aiPRB_Stopper , ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiLDR_Pusher  , ccBwd)) return false ;
                  if(!MoveActr(aiLDR_MgzGrip , ccFwd)) return false ;
                  if(!PRB.MoveActr(aiPRB_Stopper , ccFwd)) return false ;
//                  MoveActr(aiPRB_Stopper,ccFwd);
                  Step.iCycle++;
                  return false;

        case  12: //if(!MoveActr(aiPRB_Stopper,ccFwd)) return false ;
                  MoveActr(aiLDR_Pusher, ccFwd);
                  //Lot No �� �� ���� �ִ´�.
                  iSltCnt = OM.DevInfo.iMgzSlotCnt - DM.ARAY[riLDR].FindLastRow(csUnkwn) ;
                  DM.ARAY[riPRB].SetID("");//DM.ARAY[riLDR].GetID().ToIntDef(9900) + iSltCnt);
                  DM.ARAY[riPRB].SetLotNo("");//DM.ARAY[riLDR].GetLotNo());

                  Step.iCycle++;
                  return false ;

        case  13: if(IO_GetX(xLDR_PshOverload)) {
                      MoveActr(aiLDR_Pusher, ccBwd);
                      EM_SetErr(eiLDR_PshOverload);
                      Step.iCycle = 0;
                      return true ;
                  }
                  if(!MoveActr(aiLDR_Pusher, ccFwd)) return false ; //Fwd �����̸� �־�� �ɼ��� �ְٳ׿�.
                  if(!IO_GetX(xPRB_InDetect) && !OM.MstOptn.bDryRun) {
//                      EM_SetErr(eiLDR_PshFeedingFail); //�� �������� �����Ѱ� ���⵵ �ϴ�.
                      FindChip(riLDR,r,c);
                      DM.ARAY[riLDR].SetStat(r, 0, csEmpty);
                      MoveActr(aiLDR_Pusher, ccBwd);
                      Step.iCycle ++;
                      return false ;
                  }
                  DM.ARAY[riPRB].SetStat(csUnkwn);

                  FindChip(riLDR,r,c);
                  DM.ARAY[riLDR].SetStat(r, 0, csEmpty);

                  MoveActr(aiLDR_Pusher, ccBwd);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveActr(aiLDR_Pusher, ccBwd)) return false ;
//                  if(!IO_GetX(xPRB_InDetect)) return false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CLoader::CyclePlace(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //�̷��� ������ �ٲܼ� �ְ� �ؼ� ����Ŭ �̸����� �־��ָ� ���ٳ�...
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

    int  r,c ;
    int  iSltCnt ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiLDR_Pusher  , ccBwd);
                  MoveActr(aiLDR_MgzGrip , ccBwd);
                  MoveActr(aiLDR_Out     , ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiLDR_Pusher  , ccBwd)) return false ;
                  if(!MoveActr(aiLDR_MgzGrip , ccBwd)) return false ;
                  if(!MoveActr(aiLDR_Out     , ccFwd)) return false ;
                  MoveActr(aiLDR_Out     , ccBwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveActr(aiLDR_Out     , ccBwd)) return false ;

                  if(IO_GetX(xLDR_FullDetect)) {
                      EM_SetErr(eiLDR_FullMgz);
                      Step.iCycle=0;
                      return true;
                  }

                  MoveMotr(miLDR_ZCmp , piLDR_ZCmpOut );
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miLDR_ZCmp  ,piLDR_ZCmpOut )) return false ;
//                  MoveActr(aiLDR_MgzGrip , ccBwd);
                  MoveActr(aiLDR_Out     , ccFwd);

                  DM.ARAY[riLDR].SetStat(csNone) ;
                  Step.iCycle++;
                  return false ;

        case  14: //if(!MoveActr(aiLDR_MgzGrip , ccBwd)) return false ;
                  if(!MoveActr(aiLDR_Out     , ccFwd)) return false ;
//                  DM.ARAY[riLDR].SetStat(csNone) ;
                  MoveActr(aiLDR_Out, ccBwd);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiLDR_Out, ccBwd)) return false ;
                  MoveMotr(miLDR_ZCmp,piLDR_ZCmpWait); //�ڲ� ���� ����.
//                  DM.ARAY[riLDR].SetStat(csNone) ;
//                  if(m_iMgzCnt == OM.DevOptn.iLotEndMgzCnt) DM.ARAY[riLDR].SetLotNo(""); //�־�� �ϳ�?

                  Step.iCycle = 0 ;
                  return true ;
    }
}

void CLoader::SetLastCmd()
{
    return ; //����Ʈ�� �ʿ� ����.

}

bool CLoader::CheckMoved()
{
    return true ; //����Ʈ�� �ʿ� ����.
}

bool CLoader::CheckStop()
{
    if(!MT_GetStop(miLDR_YCmp)) return false ;
    if(!MT_GetStop(miLDR_ZCmp)) return false ;

    if(!AT_Done(aiLDR_Stopper )) return false ;
    if(!AT_Done(aiLDR_MgzGrip )) return false ;
    if(!AT_Done(aiLDR_Out     )) return false ;
    if(!AT_Done(aiLDR_Pusher  )) return false ;

    return true ;
}

void CLoader::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() + ".INI";

    if ( IsLoad ) {
        UserINI.Load(sPath, "Member" , "m_iMgzCnt " , m_iMgzCnt );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_iMgzCnt " , m_iMgzCnt );
    }
}
//---------------------------------------------------------------------------