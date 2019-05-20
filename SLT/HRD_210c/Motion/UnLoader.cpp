//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UnLoader.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "LotUnit.h"
//---------------------------------------------------------------------------
#include "Loader.h"
#include "Epoxy.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CUnLoader ULD;

CUnLoader::CUnLoader()
{

}
CUnLoader::~CUnLoader (void)
{
    //메인폼으로 이동.
    //Close();
}
void CUnLoader::Init()
{
    InitCycleName();
    
    m_sPartName    = "UnLoader " ;
    m_sWorkedLotNo = "";
    m_sWorkedId    = "";

    m_iHomeTO  = (int)eiULD_HomeTO    ;
    m_iStartTO = (int)eiULD_ToStartTO ;
    m_iStopTO  = (int)eiULD_ToStopTO  ;
    m_iCycleTO = (int)eiULD_CycleTO   ;

    Reset();
//    AT_MoveCyl(aiLDR_Sptr , ccFwd);
    Load(true);

    //    AT_MoveCyl(aiULD_Stopper,ccBwd);
    AT_MoveCyl(aiULD_MgzGrip,ccBwd);
    AT_MoveCyl(aiULD_Out    ,ccBwd);

    m_bRqstUldYHome = false ;

}
void CUnLoader::Close()
{
    Load(false);
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

bool CUnLoader::FindChip(EN_ARAY_ID _riWhre , int &r , int &c )
{
    if(_riWhre == riULD) {
        if(OM.DevOptn.iUdrOrder) r = DM.ARAY[riULD].FindFrstRow(csEmpty) ;
        else                     r = DM.ARAY[riULD].FindLastRow(csEmpty) ;
        return (r > -1 && r > -1);
    }

    return false ;
}

double CUnLoader::GetWorkPos(EN_MOTR_ID _iMotr)
{
    double dWorkPos = MT_GetCmdPos(_iMotr);
    int r  , c  ;
    if(_iMotr == miULD_ZCmp){
        if(FindChip(riULD,r,c)) {
            if(OM.DevOptn.iUdrOrder) dWorkPos = PM.GetValue(miULD_ZCmp , pvULD_ZCmpWorkStt) - (r) * OM.DevInfo.dMgzSlotPitch ;
            else                     dWorkPos = PM.GetValue(miULD_ZCmp , pvULD_ZCmpWorkStt) + (OM.DevInfo.iMgzSlotCnt-r-1) * OM.DevInfo.dMgzSlotPitch ;

            return dWorkPos ;
        }
    }
    return dWorkPos ;
}

double CUnLoader::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    if(_iMotr == miULD_YCmp){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piULD_YCmpWait    : dPos = PM.GetValue (_iMotr , pvULD_YCmpWait    ); break ;
//            case piULD_YCmpIn      : dPos = PM.GetValue (_iMotr , pvULD_YCmpIn      ); break ;
            case piULD_YCmpWorkStt : dPos = PM.GetValue (_iMotr , pvULD_YCmpWorkStt ); break ;
        }
    }
    else if(_iMotr == miULD_ZCmp){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piULD_ZCmpWait    : dPos = PM.GetValue (_iMotr , pvULD_ZCmpWait    ); break ;
            case piULD_ZCmpIn      : dPos = PM.GetValue (_iMotr , pvULD_ZCmpIn      ); break ;
            case piULD_ZCmpWorkStt : dPos = PM.GetValue (_iMotr , pvULD_ZCmpWorkStt ); break ;
            case piULD_ZCmpOut     : dPos = PM.GetValue (_iMotr , pvULD_ZCmpOut     ); break ;
            case piULD_ZCmpWork    : dPos = GetWorkPos  (_iMotr                     ); break ;
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

    bool isFwStopper   = !AT_Complete(aiULD_Stopper,ccBwd);
    bool isFwMgzGrip   = !AT_Complete(aiULD_MgzGrip,ccBwd);
    bool isOutCylFw    = !AT_Complete(aiULD_Out   ,ccBwd);
    bool isMgzDetected =  IO_GetX(xULD_InDetect); //|| IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2);
    bool isPSB_OutSsr  =  IO_GetX(xPSB_OutDetect);

    if(_iMotr == miULD_YCmp){
        switch(_iPstnId) {
            default                    : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
                case piULD_YCmpWait    : break ;
//                case piULD_YCmpIn         : if(isFwStopper   ) { sMsg = "(Out Cylinder is Fwd)" ; bRet = false ;} break ;
                case piULD_YCmpWorkStt : if(isFwStopper   ) { sMsg = "Out Cylinder is Fwd" ; bRet = false ;} break ;
//                                         if(isMgzDetected ) { sMsg = "(Mgz is Detected)"     ; bRet = false ;} break ;
        }
    }
    else if(_iMotr == miULD_ZCmp){
        if(isFwMgzGrip  ) { sMsg = "Mgz Grip Cylinder is not Bwd"          ; bRet = false ;}
        if(isPSB_OutSsr ) { sMsg = "Strip is feeding now(OutSsr Detected)" ; bRet = false ;}
        switch(_iPstnId) {
            default                    : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
                case piULD_ZCmpIn      : break ;
                case piULD_ZCmpWorkStt : break ;
                case piULD_ZCmpOut     : if(isOutCylFw )  { sMsg = "Out Cylinder is Fwd" ; bRet = false ;} break ;
                case piULD_ZCmpWork    : if(isOutCylFw )  { sMsg = "Out Cylinder is Fwd" ; bRet = false ;} break ;
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
bool CUnLoader::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

//    bool isMgzDetected =  //IO_GetX(xULD_InDetect) ||
//                          IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2);
    bool isZMoving     = !MT_GetStopInpos(miULD_ZCmp);

    if(_iActr == aiULD_Stopper){
    }
    else if(_iActr == aiULD_MgzGrip){
        if(_bFwd) if(isZMoving) {sMsg = "ULD_Z_Axis is Moving" ; bRet = false ;}
    }
    else if(_iActr == aiULD_Out    ){
       // if(_bFwd) if(IO_GetX(xULD_FullDetect)) {sMsg = "ULD Mgz is Full" ; bRet = false ;}
    }
    else {
        sMsg = AnsiString(AT_GetName(_iActr)) + " is Not this part Cylinder!" ;
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr),sMsg.c_str());
    }

    return bRet ;
}

bool CUnLoader::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlowSpeed) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) {
        if(_bSlowSpeed)return MT_GoAbsSlow(_iMotr , dPosition);
        else           return MT_GoAbsRun (_iMotr , dPosition);
    }
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CUnLoader::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CUnLoader::CycleHome()     //sun DLL direct access.
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

        case 10: AT_MoveCyl(aiULD_MgzGrip, ccBwd);
                 AT_MoveCyl(aiULD_Out    , ccBwd);
                 //AT_MoveCyl(aiULD_Stopper, ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiULD_MgzGrip, ccBwd))return false ;
                 if(!AT_MoveCyl(aiULD_Out    , ccBwd))return false ;
                 //if(!AT_MoveCyl(aiULD_Stopper, ccBwd))return false ;

                 MT_DoHome(miULD_ZCmp);
                 MT_DoHome(miULD_YCmp);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miULD_ZCmp))return false ;
                 if(!MT_GetHomeEnd(miULD_YCmp))return false ;
                 MT_GoAbsMan(miULD_ZCmp , PM.GetValue(miULD_ZCmp , pvULD_ZCmpWait  ));
                 MT_GoAbsMan(miULD_YCmp , PM.GetValue(miULD_YCmp , pvULD_YCmpWait  ));
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miULD_ZCmp , PM.GetValue(miULD_ZCmp , pvULD_ZCmpWait  )))return false ;
                 if(!MT_GoAbsMan(miULD_YCmp , PM.GetValue(miULD_YCmp , pvULD_YCmpWait  )))return false ;
//                 AT_MoveCyl(aiULD_Stopper, ccBwd);
//                 Step.iHome++;
//                 return false ;
//
//        case 14: if(!AT_MoveCyl(aiULD_Stopper, ccBwd)) return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CUnLoader::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

    Step.iToStop = 10;
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

        case 10: MoveActr(aiULD_Out,ccBwd);
                 MoveActr(aiULD_Stopper, ccFwd         );
                 if(MT_GetHomeEnd(miULD_YCmp)) MoveMotr(miULD_YCmp  , piULD_YCmpWait);
                 if(IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2)) MoveActr(aiULD_MgzGrip,ccFwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiULD_Out,ccBwd)) return false ;
                 if(!MoveActr(aiULD_Stopper, ccFwd         )) return false ;
                 if(MT_GetHomeEnd(miULD_YCmp)) {
                     if(!MoveMotr(miULD_YCmp  , piULD_YCmpWait)) return false;
                 }
                 if(IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2)) if(!MoveActr(aiULD_MgzGrip,ccFwd)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CUnLoader::ToStop(void) //스탑을 하기 위한 함수.
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
                 MoveActr(aiULD_Out,ccBwd);
                 MoveActr(aiULD_Stopper, ccBwd         );
                 Step.iToStop++;
                 return false ;

        case 11: //if(!m_tmTemp.OnDelay(!MT_GetHomeEnd(miULD_YCmp) , 300)) return false ; //그나마 이렇게 기다려 준다.
                 if(!MoveActr(aiULD_Out,ccBwd)) return false ;
                 if(!MoveActr(aiULD_Stopper, ccBwd         )) return false ;
                 if(MT_GetHomeEnd(miULD_YCmp)) MoveMotr(miULD_YCmp,piULD_YCmpWait);
                 Step.iToStop ++;
                 return false ;

        case 12: //if(!MoveMotr(miULD_YCmp,piULD_YCmpWait)) return false ;
                 if(MT_GetHomeEnd(miULD_YCmp)) {
                     if(!MoveMotr(miULD_YCmp,piULD_YCmpWait)) return false ;
                 }


                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CUnLoader::IsMgzLastStrip() //1개의 메거진 에서 1개의 메거진으로 담을때.
{
    AnsiString sLastLotNo ;


    if(DM.ARAY[riULD].GetCntStat(csWork)) sLastLotNo = DM.ARAY[riULD].GetLotNo() ;
    else                                  return false ; //sLastLotNo = m_sWorkedLotNo ; 한장도 안꼽혔을때는 라스트 스트립은 무조건 아님.

    bool bLDRExist = !DM.ARAY[riLDR].CheckAllStat(csNone) ;
    bool bPRBExist = !DM.ARAY[riPRB].CheckAllStat(csNone) ;
    bool bWREExist = !DM.ARAY[riREF].CheckAllStat(csNone) ;
    bool bWRDExist = !DM.ARAY[riRDF].CheckAllStat(csNone) ;
    bool bPSBExist = !DM.ARAY[riPSF].CheckAllStat(csNone) ;

    bool bLDRDiff  = sLastLotNo != DM.ARAY[riLDR].GetLotNo() ;
    bool bPRBDiff  = sLastLotNo != DM.ARAY[riPRB].GetLotNo() ;
    bool bWREDiff  = sLastLotNo != DM.ARAY[riREF].GetLotNo() ;
    bool bWRDDiff  = sLastLotNo != DM.ARAY[riRDF].GetLotNo() ;
    bool bPSBDiff  = sLastLotNo != DM.ARAY[riPSF].GetLotNo() ;


         if( bPSBExist) return bPSBDiff ;
    else if( bWRDExist) return bWRDDiff ;
    else if( bWREExist) return bWREDiff ;
    else if( bPRBExist) return bPRBDiff ;
    else if( bLDRExist) return bLDRDiff ;
    else                return true     ; //레일에 자제가 없고 라스트 메거진이면 들고있으면 플레이스 없으면 픽을 하지 말아야 한다.
}

bool CUnLoader::IsWorkEnd() //그냥 노냥 담을때.
{

    bool bLDRExist = !DM.ARAY[riLDR].CheckAllStat(csNone) ;
    bool bPRBExist = !DM.ARAY[riPRB].CheckAllStat(csNone) ;
    bool bWREExist = !DM.ARAY[riREF].CheckAllStat(csNone) ;
    bool bWRDExist = !DM.ARAY[riRDF].CheckAllStat(csNone) ;
    bool bPSBExist = !DM.ARAY[riPSF].CheckAllStat(csNone) ;

    bool bExistMgz  = OM.DevOptn.iLotEndMgzCnt==0 || LDR._iMgzCnt < OM.DevOptn.iLotEndMgzCnt ;

         if( bPSBExist) return false ;
    else if( bWRDExist) return false ;
    else if( bWREExist) return false ;
    else if( bPRBExist) return false ;
    else if( bLDRExist) return false ;
    else if( bExistMgz) return false ;
    else                return true  ; //레일에 다 없고 로더 메거진 카운트 끝났을때.


}

bool CUnLoader::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool isMgzDetected  =  IO_GetX(xULD_InDetect) || IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2);

        bool isZWorkPos     =  MT_CmprPos(miULD_ZCmp ,GetMotrPos(miULD_ZCmp , piULD_ZCmpWork)) ;

        bool isCycleSupply  =  false ;
        bool isCyclePick    =  DM.ARAY[riULD].CheckAllStat(csNone ) && !isMgzDetected && !IsWorkEnd() ;

        bool isCycleWork    =  DM.ARAY[riULD].GetCntStat  (csEmpty) && !isZWorkPos ;
        bool isCyclePlace   =  DM.ARAY[riULD].CheckAllStat(csWork ) || (IsWorkEnd() && !DM.ARAY[riULD].CheckAllStat(csNone )) ;

        bool isCycleUldYHome= m_bRqstUldYHome ;
        bool isConEnd       = DM.ARAY[riULD].CheckAllStat(csNone ) &&  IsWorkEnd() ;

        if(  DM.ARAY[riULD].CheckAllStat(csNone) &&  isMgzDetected )  EM_SetErr(eiULD_Unknwn) ;
        if( !DM.ARAY[riULD].CheckAllStat(csNone) && !isMgzDetected && AT_Complete(aiULD_MgzGrip,ccFwd) && !OM.MstOptn.bDryRun)  EM_SetErr(eiULD_Dispr  ) ;

        //if(  DM.ARAY[riULD].CheckAllStat(csNone) && !isMgzDetected && DM.ARAY[riPSF].GetCntExist() ) EM_SetErr(eiULD_NeedMgz) ;
        if(IO_GetX(xULD_FullDetect)) EM_SetErr(eiULD_FullMgz);

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleSupply   ) {Trace(m_sPartName.c_str(),"CycleSupply   Stt"); Step.iSeq = scSupply   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePick     ) {Trace(m_sPartName.c_str(),"CyclePick     Stt"); Step.iSeq = scPick     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork     ) {Trace(m_sPartName.c_str(),"CycleWork     Stt"); Step.iSeq = scWork     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePlace    ) {Trace(m_sPartName.c_str(),"CyclePlace    Stt"); Step.iSeq = scPlace    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleUldYHome ) {Trace(m_sPartName.c_str(),"CycleUldYHome Stt"); Step.iSeq = scUldYHome ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd        ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default        :                      Trace(m_sPartName.c_str(),"default       End");Step.iSeq = scIdle ;  return false ;
        case scIdle    :                                                                                           return false ;
        case scSupply  : if(CycleSupply  ()){ Trace(m_sPartName.c_str(),"CycleSupply   End");Step.iSeq = scIdle ;} return false ;
        case scPick    : if(CyclePick    ()){ Trace(m_sPartName.c_str(),"CyclePick     End");Step.iSeq = scIdle ;} return false ;
        case scWork    : if(CycleWork    ()){ Trace(m_sPartName.c_str(),"CycleWork     End");Step.iSeq = scIdle ;} return false ;
        case scPlace   : if(CyclePlace   ()){ Trace(m_sPartName.c_str(),"CyclePlace    End");Step.iSeq = scIdle ;} return false ;
        case scUldYHome: if(CycleUldYHome()){ Trace(m_sPartName.c_str(),"CycleUldYHome End");Step.iSeq = scIdle ;} return false ;
    }
}

//One Cycle.
bool CUnLoader::CycleSupply(void)
{
    return false ;
}
bool CUnLoader::CyclePick(void)
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

    int      r1,r2 ;
    String    sLotNo , sDate;
    int       m_iMgzCnt ;
    TDateTime tTemp ; //예비.

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiULD_MgzGrip, ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiULD_MgzGrip, ccBwd)) return false ;
                  MoveMotr(miULD_ZCmp , piULD_ZCmpIn  );
                  MoveMotr(miULD_YCmp , piULD_YCmpWait);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miULD_ZCmp  ,piULD_ZCmpIn  )) return false ;
                  if(!MoveMotr(miULD_YCmp , piULD_YCmpWait)) return false ;
                  MoveActr(aiULD_Stopper, ccBwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiULD_Stopper, ccBwd)) return false ;
                  MoveMotr(miULD_YCmp , piULD_YCmpWorkStt);
                  Step.iCycle++;
                  return false ;

        case  14: if(IO_GetX(xULD_InDetect)) MT_EmgStop(miULD_YCmp); //IO_On Delay 로 하도록 하세요...옵션은 사양 하겟습니다.
                  else {if(!MoveMotr(miULD_YCmp , piULD_YCmpWorkStt)) return false ;}

                  if(!IO_GetX(xULD_InDetect) && !OM.MstOptn.bDryRun) {
                      MoveMotr(miULD_YCmp , piULD_YCmpWait);
                      EM_SetErr(eiULD_NeedMgz);
                      Step.iCycle = 0;
                      return true ;
                  }

                  DM.ARAY[riULD].SetStat(csEmpty); //Data Masking

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  15: if(!m_tmTemp.OnDelay(true,300)) return false ;
                  MoveActr(aiULD_Stopper, ccFwd);
                  MoveMotr(miULD_YCmp , piULD_YCmpWait   ); //
                  Step.iCycle++;
                  return false ;

        case  16:
                  if(MT_GetHomeSnsr(miULD_YCmp)) MT_EmgStop(miULD_YCmp);
                  if(!MoveActr(aiULD_Stopper, ccFwd)) return false ;
//                  MoveMotr(miULD_YCmp , piULD_YCmpWait   ); //확인 17
                  //MT_DoHome(miULD_YCmp); //홈스피드좀 빠르게 하던가.
                  MoveMotr(miULD_ZCmp , piULD_ZCmpWorkStt);
                  Step.iCycle++;
                  return false ;

        case  17: if(MT_GetHomeSnsr(miULD_YCmp)) MT_EmgStop(miULD_YCmp);
                  if(!MoveMotr(miULD_ZCmp , piULD_ZCmpWorkStt)) return false ;
                  MoveActr(aiULD_MgzGrip,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  18: if(MT_GetHomeSnsr(miULD_YCmp)) MT_EmgStop(miULD_YCmp);
                  if(!MoveActr(aiULD_MgzGrip, ccFwd         )) return false ;
                  //if(!MT_GetStopInpos(miULD_YCmp)) return false ; 오래걸려서 확인 안함.
                  //MT_DoHome(miULD_YCmp);

                  Step.iCycle++;
                  return false ;

        case  19: //if(!MT_GetHomeEnd(miULD_YCmp)) return false ;
//                  if(!MoveMotr(miULD_YCmp   , piULD_YCmpWait)) return false ;
                  if(!OM.MstOptn.bDryRun) {
                      if(!IO_GetX(xULD_MgzDetect1) && !IO_GetX(xULD_MgzDetect2)) EM_SetErr(eiULD_PickFail);
                  }

                  m_bRqstUldYHome = true ;

                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CUnLoader::CycleWork(void)
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

        case  10: MoveActr(aiULD_Out,ccBwd);
                  MoveActr(aiULD_MgzGrip , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiULD_Out,ccBwd)) return false ;
                  if(!MoveActr(aiULD_MgzGrip , ccBwd)) return false ;
                  MoveMotr(miULD_ZCmp , piULD_ZCmpWork );
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miULD_ZCmp  ,piULD_ZCmpWork )) return false ;
                  MoveActr(aiULD_MgzGrip , ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiULD_MgzGrip , ccFwd)) return false ;
                  if(DM.ARAY[riULD].GetCntStat(csWork)) {
                      m_sWorkedId    = DM.ARAY[riULD].GetID   ();
                      m_sWorkedLotNo = DM.ARAY[riULD].GetLotNo();
                  }
                  Step.iCycle = 0 ;
                  return true ;
        }
}

bool CUnLoader::CycleUldYHome()
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

        case  10: if(!MT_GetStopInpos(miULD_YCmp)) return false ; //아직 오고 있을수도 있어서.

                  MoveMotr(miULD_YCmp , piULD_YCmpWait);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miULD_YCmp , piULD_YCmpWait)) return false ;
                  MT_DoHome(miULD_YCmp);
                  //MT_JogN(miULD_YCmp);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MT_GetHomeEnd(miULD_YCmp))return false ;
                  m_bRqstUldYHome = false ;
                  Step.iCycle = 0 ;
                  return true ;
                  /*
                  MT_JogN(miULD_YCmp);
                  Step.iCycle++;
                  return false ;

        case  12: if(MT_GetCmdPos(miULD_YCmp) < 0.0) {
                      MT_Stop(miULD_YCmp); //많이 갔는데 못찾았다.
                      m_bRqstUldYHome = false ;
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  if(MT_GetHomeSnsr(miULD_YCmp)) MT_Stop(miULD_YCmp);
                  if(!MT_GetStop(miULD_YCmp))return false ;
                  MT_JogP(miULD_YCmp);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MT_GetHomeSnsr(miULD_YCmp)) MT_Stop(miULD_YCmp);
                  if(!MT_GetStop(miULD_YCmp))return false ;
                  m_bRqstUldYHome = false ;
                  Step.iCycle = 0 ;
                  return true ;      */
    }
}

bool CUnLoader::CyclePlace(void)
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

        case  10:
                  MoveActr(aiULD_MgzGrip , ccBwd);
                  MoveActr(aiULD_Out     , ccFwd);
                  Step.iCycle++;
                  return false;

        case  11:
                  if(!MoveActr(aiULD_MgzGrip , ccBwd)) return false ;
                  if(!MoveActr(aiULD_Out     , ccFwd)) return false ;
                  MoveActr(aiULD_Out     , ccBwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveActr(aiULD_Out     , ccBwd)) return false ;

                  if(IO_GetX(xULD_FullDetect)) {
                      EM_SetErr(eiULD_FullMgz);
                      Step.iCycle=0;
                      return true;
                  }

                  MoveMotr(miULD_ZCmp , piULD_ZCmpOut );
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miULD_ZCmp  ,piULD_ZCmpOut )) return false ;
//                  MoveActr(aiULD_MgzGrip , ccBwd);
                  MoveActr(aiULD_Out     , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  14: //if(!MoveActr(aiULD_MgzGrip , ccBwd)) return false ;
                  if(!MoveActr(aiULD_Out     , ccFwd)) return false ;
                  DM.ARAY[riULD].SetStat(csNone) ;
                  MoveActr(aiULD_Out, ccBwd); //미스테리 #1 왕복운동을 하였음.dd
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiULD_Out, ccBwd)) return false ;
                  MoveMotr(miULD_ZCmp , piULD_ZCmpWait  );
//                  DM.ARAY[riULD].SetStat(csNone) ;
//                  if(m_iMgzCnt == OM.DevOptn.iLotEndMgzCnt) DM.ARAY[riULD].SetLotNo(""); //있어야 하나?

                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CUnLoader::CyclePitchUp()
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

        case  10:
                  MoveActr(aiULD_MgzGrip , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11:
                  if(!MoveActr(aiULD_MgzGrip , ccBwd)) return false ;
                  MT_GoIncMan(miULD_ZCmp , -OM.DevInfo.dMgzSlotPitch);
                  Step.iCycle++;
                  return false;

        case  12: if(!MT_GetStop(miULD_ZCmp)) return false ;
                  MoveActr(aiULD_MgzGrip , ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiULD_MgzGrip , ccFwd)) return false ;

                  Step.iCycle = 0 ;
                  return true ;
    }

}

bool CUnLoader::CyclePitchDn()
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

        case  10:
                  MoveActr(aiULD_MgzGrip , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11:
                  if(!MoveActr(aiULD_MgzGrip , ccBwd)) return false ;
                  MT_GoIncMan(miULD_ZCmp , OM.DevInfo.dMgzSlotPitch);
                  Step.iCycle++;
                  return false;

        case  12: if(!MT_GetStop(miULD_ZCmp)) return false ;
                  MoveActr(aiULD_MgzGrip , ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiULD_MgzGrip , ccFwd)) return false ;

                  Step.iCycle = 0 ;
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

    if(!AT_Done(aiULD_Stopper )) return false ;
    if(!AT_Done(aiULD_MgzGrip )) return false ;
    if(!AT_Done(aiULD_Out     )) return false ;

    return true ;
}

void CUnLoader::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() + ".INI";

    if ( IsLoad ) {
        UserINI.Load(sPath, "Member" , "m_sWorkedId    " , m_sWorkedId    );
        UserINI.Load(sPath, "Member" , "m_sWorkedLotNo " , m_sWorkedLotNo );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_sWorkedId    " , m_sWorkedId    );
        UserINI.Save(sPath, "Member" , "m_sWorkedLotNo " , m_sWorkedLotNo );
    }
}
//---------------------------------------------------------------------------
