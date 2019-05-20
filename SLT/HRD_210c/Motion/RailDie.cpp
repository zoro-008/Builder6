//---------------------------------------------------------------------------
#include <vcl.h>
#include <Math.h>
#pragma hdrstop

#include "RailDie.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "LotUnit.h"
#include "SpcSubUnit.h"
//---------------------------------------------------------------------------
#include "Epoxy.h"
#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CRailDie RAD;

CRailDie::CRailDie()
{


}
CRailDie::~CRailDie (void)
{
    //메인폼으로 이동.
    //Close();
}
void CRailDie::Init()
{
    InitCycleName();
    
    m_sPartName   = "RailDie " ;
    m_dLastIdxPos = 0.0 ;
    m_bInsYReady  = false ;
    m_bEpxFailIgnr  = false ;

    m_iHomeTO  = (int)eiWRD_HomeTO    ;
    m_iStartTO = (int)eiWRD_ToStartTO ;
    m_iStopTO  = (int)eiWRD_ToStopTO  ;
    m_iCycleTO = (int)eiWRD_CycleTO   ;

//    double dTemp = PM.GetValue (miWRD_XIdx , pvWRD_XIdxWorkStt ) + (OM.DevInfo.dColPitch * (OM.DevInfo.iColCnt - 1)) ;
//    PM.SetPos(miWRD_XIdx,pvWRD_XIdxPull,dTemp);

    Reset();
    Load(true);

    AT_MoveCyl(aiWRD_Clamp   , ccBwd );
}
void CRailDie::Close()
{
    Load(false);
}
void CRailDie::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CRailDie::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();

}

bool CRailDie::FindChip(EN_ARAY_ID _riWhre , int &r , int &c )
{
    int r1,c1;

    EN_CHIP_STAT iFindStat ;

    if(_riWhre == riRDF) { //X Idx
             if(DM.ARAY[riRDF].GetCntStat(csWait )) DM.ARAY[riRDF].FindLastColRow(csWait  , r , c) ;
        else if(DM.ARAY[riRDF].GetCntStat(csMask )) DM.ARAY[riRDF].FindLastColRow(csMask  , r , c) ;
        else if(DM.ARAY[riRDF].GetCntStat(csWait2)) DM.ARAY[riRDF].FindLastColRow(csWait2 , r , c) ;
        else if(DM.ARAY[riRDF].GetCntStat(csUnkwn)){
            c = DM.ARAY[riRDF].FindLastCol(csUnkwn);
            if(c % 2)DM.ARAY[riRDF].FindLastColRow    (csUnkwn , r , c) ;
            else     DM.ARAY[riRDF].FindLastColFrstRow(csUnkwn , r , c) ;
        }
        else return false;

        return (r > -1 && c > -1);
    }

//    else if(_riWhre == riRDV) { //Y Ins
//             if(DM.ARAY[riRDV].GetCntStat(csWait )) DM.ARAY[riRDV].FindLastColRow(csWait,r,c);
//        else if(DM.ARAY[riRDV].GetCntStat(csMask )) DM.ARAY[riRDV].FindLastColRow(csMask,r,c);
//        else if(DM.ARAY[riRDV].GetCntStat(csUnkwn)) {
//            c = DM.ARAY[riRDV].FindLastCol(csUnkwn);
//            if(c % 2)DM.ARAY[riRDV].FindLastColRow    (csUnkwn , r , c) ;
//            else     DM.ARAY[riRDV].FindLastColFrstRow(csUnkwn , r , c) ;
//        }
//        else {
//            r= 0 ; c = 0 ;
//        }
//
//        return (r > -1 && c > -1);
//    }

    else return false ;
}

double CRailDie::GetWorkPos(EN_MOTR_ID _iMotr)
{
    double dWorkPos = MT_GetCmdPos(_iMotr) ;
    double dReGrip  = 0.0 ;
    int r  , c  ;
    if(_iMotr == miWRD_XIdx){
        if(FindChip(riRDF,r,c)){
            c = OM.DevInfo.iColCnt - c - 1 ;
            dWorkPos = PM.GetValue (_iMotr , pvWRD_XIdxWorkStt ) + ( OM.DevInfo.dColPitch * c ) ;

            if(DM.ARAY[riRDF].GetStep() >1 ) {
                for(int i =0 ; i < DM.ARAY[riRDF].GetStep() - 1 ; i++) dReGrip += OM.DevOptn.iWRDRgDis[i] * OM.DevInfo.dColPitch;
                dWorkPos -= dReGrip ;
            }
        }
    }
    else if(_iMotr == miWRD_YIns){
        if( FindChip(riRDF,r,c)) {
//            r = OM.DevInfo.iRowCnt - r - 1 ;
            dWorkPos = PM.GetValue (_iMotr , pvWRD_YInsWorkStt ) + ( OM.DevInfo.dRowPitch * r ) ;
        }
    }
    return dWorkPos ;
}

double CRailDie::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;
    int r = -1 , c = -1 ;

    if(_iMotr == miWRD_XIdx){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piWRD_XIdxWait    : dPos = PM.GetValue (_iMotr , pvWRD_XIdxWait    ); break ;
            case piWRD_XIdxWorkStt : dPos = PM.GetValue (_iMotr , pvWRD_XIdxWorkStt ); break ;
//            case piWRD_XIdxPull    : dPos = PM.GetValue (_iMotr , pvWRD_XIdxPull    ); break ;
//            case piWRD_XIdxBack    : dPos = PM.GetValue (_iMotr , pvWRD_XIdxBack    ); break ;
            case piWRD_XIdxOut     : dPos = PM.GetValue (_iMotr , pvWRD_XIdxOut     ); break ;
            case piWRD_XIdxWork    : dPos = GetWorkPos  (_iMotr                     ); break ;
        }
    }
    else if(_iMotr == miWRD_YIns){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piWRD_YInsWait    : dPos = PM.GetValue (_iMotr , pvWRD_YInsWait    ); break ;
            case piWRD_YInsWorkStt : dPos = PM.GetValue (_iMotr , pvWRD_YInsWorkStt ); break ;
            case piWRD_YInsWork    : dPos = GetWorkPos  (_iMotr                     ); break ;
        }
    }


    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CRailDie::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    double dCmdPos = MT_GetCmdPos(miWRD_ZHed) ;
    double dTrgPos = MT_GetTrgPos(miWRD_ZHed) ;

    double dCanMovePos = PM.GetValue(miWRD_ZHed,pvWRD_ZHedMove)+0.1 ;

    bool isDieZWaitPs = (dCmdPos < dCanMovePos) &&
                        (dTrgPos < dCanMovePos) ;
    bool isWrdClampFw = !AT_Complete(aiWRD_Clamp  ,ccBwd);

    bool isHeadRailZone = MT_GetCmdPos(miWRD_YHed) < PM.GetValue(miWRD_YHed , pvWRD_YHedStripWait);



    bool bPsbClampFw  = !AT_Complete(aiPSB_Clamp  ,ccBwd);

    //const double dIdxCrashArea = 310.0 ; //나중에 마스터 옵션으로 빼기

    if(_iMotr == miWRD_XIdx){
        if((!isDieZWaitPs && isHeadRailZone) && isWrdClampFw){ sMsg = "RAIL HED Z is not wait Pos" ; bRet = false ;}
        if(MT_GetTrgPos(miPSB_XIdx) + GetMotrPos(_iMotr , _iPstnId) > OM.MstOptn.d3stDis) {sMsg = "Index Crash!" ; bRet = false ;}

        switch(_iPstnId) {
            default                : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRD_XIdxWait    : //if(PM.GetValue(_iMotr,pvWRD_XIdxWait) + MT_GetCmdPos(miPSB_XIdx) >= OM.MstOptn.d3stDis ) {
                                     //    sMsg = "Not Able to Move Because of a Motr Crash(WRD_X Wait Position)" ;
                                     //    bRet = false ;
                                     //}
                                     break ;
            case piWRD_XIdxWorkStt : //if(PM.GetValue(_iMotr,pvWRD_XIdxWorkStt) + MT_GetCmdPos(miPSB_XIdx) >= OM.MstOptn.d3stDis ) {
                                     //    sMsg = "Not Able to Move Because of a Motr Crash(WRD_X WorkStt Position)" ;
                                     //    bRet = false ;
                                     //}
                                     break ;
            case piWRD_XIdxOut     : if(bPsbClampFw){
                                         sMsg = "Post Buffer Clamp is Fwd" ;
                                         bRet = false ;
                                     }
                                     //if(PM.GetValue(_iMotr,pvWRD_XIdxOut) + MT_GetCmdPos(miPSB_XIdx) >= OM.MstOptn.d3stDis ) {
                                     //    sMsg = "Not Able to Move Because of a Motr Crash(WRD_X Out Position)" ;
                                     //    bRet = false ;
                                     //}
                                     break ;

            case piWRD_XIdxWork    : //if(GetWorkPos(_iMotr) + MT_GetCmdPos(miPSB_XIdx) >= OM.MstOptn.d3stDis ) {
                                     //    sMsg = "Not Able to Move Because of a Motr Crash(WRD_X Work Position)" ;
                                     //    bRet = false ;
                                     //}
                                     break ;
        }
    }
    else if(_iMotr == miWRD_YIns){
        switch(_iPstnId) {
            default                : break ;
            case piWRD_YInsWait    : break ;
            case piWRD_YInsWorkStt : break ;
            case piWRD_YInsWork    : break ;
        }
    }

    else {
        sMsg = AnsiString(MT_GetName(_iMotr)) + " is Not this part Motr!" ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle && !Step.iToStart )FM_MsgOk(MT_GetName(_iMotr),sMsg.c_str());
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CRailDie::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool isXMoving = !MT_GetStopInpos(miWRD_XIdx);

    if(_iActr == aiWRD_Clamp ){//|| _iActr == aiWRD_Align){
        if(isXMoving) {sMsg = "Rail Die X Index is Moving" ; bRet = false ;}
    }
    //else if(_iActr == aiWRD_CAlign){
    //    if(_bFwd && isXMoving){sMsg = "Rail Die X Index is Moving" ; bRet = false ;}
    //}
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

bool CRailDie::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlowSpeed) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) {
        if(_bSlowSpeed)return MT_GoAbsSlow(_iMotr , dPosition);
        else           return MT_GoAbsRun (_iMotr , dPosition);
    }
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CRailDie::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CRailDie::CycleHome()     //sun DLL direct access.
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
    static bool bWRDNone = true ;

    switch (Step.iHome) {

        default: if(Step.iHome) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iHome = 0 ;
                 return true ;

        case 10: MT_SetHomeEnd(miWRD_XIdx,false);
                 MT_SetHomeEnd(miWRD_YIns,false);
                 AT_MoveCyl(aiWRD_Clamp  , ccBwd);
                 //AT_MoveCyl(aiWRD_CAlign , ccBwd);
                 bWRDNone = DM.ARAY[riRDF].CheckAllStat(csNone) ;
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiWRD_Clamp  , ccBwd))return false ;
                 //if(!AT_MoveCyl(aiWRD_CAlign , ccBwd))return false ;
                 MT_DoHome(miWRD_XIdx);
                 MT_DoHome(miWRD_YIns);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miWRD_XIdx))return false ;
                 if(!MT_GetHomeEnd(miWRD_YIns))return false ;
                 if(bWRDNone) {MT_GoAbsMan(miWRD_XIdx , PM.GetValue(miWRD_XIdx , pvWRD_XIdxWait  )); }
                 else         {MT_GoAbsMan(miWRD_XIdx , m_dLastIdxPos);                              }
                 MT_GoAbsMan(miWRD_YIns , PM.GetValue(miWRD_YIns , pvWRD_YInsWait));

                 Step.iHome++;
                 return false ;

        case 13: if(bWRDNone) {if(!MT_GoAbsMan(miWRD_XIdx , PM.GetValue(miWRD_XIdx , pvWRD_XIdxWait  ))) return false ; }
                 else         {if(!MT_GoAbsMan(miWRD_XIdx , m_dLastIdxPos                             )) return false ; }
                 if(!MT_GoAbsMan(miWRD_YIns , PM.GetValue(miWRD_YIns , pvWRD_YInsWait))) return false ;
//                 if(!bWRDNone) AT_MoveCyl(aiWRD_Clamp,ccFwd);
//
//                 Step.iHome++;
//                 return false ;
//
//        case 14: if(!bWRDNone) if(!AT_MoveCyl(aiWRD_Clamp,ccFwd)) return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CRailDie::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CRailDie::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CRailDie::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)m_iStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    static bool bNone = false ;

    //Move Home.
    switch (Step.iToStart) {
        default: if(Step.iToStart) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStart = 0 ;
                 return true ;

        case 10: bNone = DM.ARAY[riRDF].CheckAllStat(csNone);
                 if(!bNone) {
                     Step.iToStart = 15 ;
                     return false ;
                 }
                 //None -> 11
                 //Not None -> 15
                 //MoveActr(aiWRD_Clamp  ,ccBwd);
                 //MoveActr(aiWRD_CAlign ,ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: //if(!MoveActr(aiWRD_Clamp  ,ccBwd)) return false ;
                 //if(!MoveActr(aiWRD_CAlign ,ccBwd)) return false ;
                 //IO_SetY(yWRD_Vaccum , false);
                 MoveActr(aiWRD_Clamp  ,ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 12: if(!MoveActr(aiWRD_Clamp  ,ccBwd))    return false ;
                 MoveMotr(miWRD_XIdx,piWRD_XIdxWait);
                 Step.iToStart ++ ;
                 return false ;

        case 13: if(!MoveMotr(miWRD_XIdx,piWRD_XIdxWait)) return false ;
                 DM.ARAY[riRDF].ChangeStat(csMask , csWait);

                 Step.iToStart = 0 ;
                 return true ;

        case 15: if(!MoveActr(aiWRD_Clamp  ,ccFwd)) return false ;
                 //MoveActr(aiWRD_CAlign  ,ccFwd);

                 IO_SetY(yWRD_Vaccum , true);
                 Step.iToStart ++ ;
                 return false ;

        case 16:
                 //if(!MoveActr(aiWRD_CAlign  ,ccFwd)) return false ;
                 DM.ARAY[riRDF].ChangeStat(csMask , csWait);

                 Step.iToStart = 0 ;
                 return true ;



    }
}

bool CRailDie::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10: //DM.ARAY[riRDF].ChangeStat(csWait,csUnkwn);
                 if(!MT_GetStopInpos(miWRD_XIdx)) return false ;
                 m_dLastIdxPos = GetLastCmd(miWRD_XIdx);

                 //IO_SetY(yWRD_Vaccum , false); 껏다키면 밀리는듯
                 Step.iToStop ++;
                 return false ;

        case 11: 
                 MoveActr(aiWRD_Clamp ,ccBwd);
                 //MoveActr(aiWRD_CAlign,ccBwd);

                 Step.iToStop ++;
                 return false ;

        case 12: if(!MoveActr(aiWRD_Clamp,ccBwd)) return false ;

                 Step.iToStop = 0   ;
                 return true ;
    }
}

/*
bool CRailDie::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;


    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        riRDF == 레일 플로우...
        riRDV == 레일 결과값들.

        csUnknwn = 작업전것들./.
        csWait   = BeforeVision 찍어주기 기다림.
        csMask   = 자제 내려놓기 기다림.
        csWait2  = 자제 내려놓고 After비전 찍어주기 기다림.

        csWork   = 작업 후


        riRDV //비전 결과값들.
        csNone   = 자제 없음.
        csUnkwn  = 작업전.
        csRslt0  = Ori Fail
        csRslt1  = Epx Fail
        csRslt2  = Die Fail


    DM.ARAY[riRDF].SetDispItem(csNone   ,"자제없음"   ) ; DM.ARAY[riWRD].SetDispItem(csNone   ,"자제없음"   ) ;
    DM.ARAY[riRDF].SetDispItem(csUnkwn  ,"작업전"     ) ; DM.ARAY[riWRD].SetDispItem(csUnkwn  ,"작업전"     ) ;
    DM.ARAY[riRDF].SetDispItem(csWait   ,"BF비전대기" ) ; DM.ARAY[riWRD].SetDispItem(csRslt0  ,"방향불량"   ) ;
    DM.ARAY[riRDF].SetDispItem(csMask   ,"칩놓기대기" ) ; DM.ARAY[riWRD].SetDispItem(csRslt1  ,"에폭시불량" ) ;
    DM.ARAY[riRDF].SetDispItem(csWait2  ,"AT비전대기" ) ; DM.ARAY[riWRD].SetDispItem(csRslt2  ,"칩위치불량" ) ;
    DM.ARAY[riRDF].SetDispItem(csWork   ,"작업끝"     ) ; DM.ARAY[riWRD].SetDispItem(csRslt3  ,"기준점불량" ) ;
                                                          DM.ARAY[riWRD].SetDispItem(csWork   ,"작업끝"     ) ;







        //Check
        int  iCNow     = DM.ARAY[riRDF].FindLastCol(csUnkwn) ;
        int  iCRvs     = OM.DevInfo.iColCnt - iCNow ;

        bool isVisnZone   = PM.GetValue(miWRD_YHed , pvWRD_YHedStripWait) < MT_GetEncPos(miWRD_YHed) ;

        int  iReGribStep  = DM.ARAY[riRDF].GetStep() ;
        bool bAllNone     = DM.ARAY[riRDF].CheckAllStat(csNone ) ;
        bool bPSBNone     = DM.ARAY[riRAP].CheckAllStat(csNone ) ;



        bool isPSBWorkSttCon = DM.ARAY[riRAP].GetStep() >= 1 || bPSBNone ;
        bool isWorkPsbCon    = bPSBNone || !(iCRvs >= OM.DevOptn.iWRD_XWkCon && DM.ARAY[riRAP].GetStep() > 2 && IO_GetX(xPSB_OutDetect)) ; //TODO :: 포스트와의 조건 체크.
        bool bReGrip         = iReGribStep > 0 &&  iCRvs >= OM.DevOptn.iWRDRgCol[iReGribStep-1] && iReGribStep < OM.DevOptn.iWRD_XRgCnt + 1 ;

        bool bCheckXIdxPos   =  MT_CmprPos(miWRD_XIdx,GetWorkPos(miWRD_XIdx)) ;
        bool bCheckYIdxPos   =  MT_CmprPos(miWRD_YIns,GetWorkPos(miWRD_YIns)) ;

        bool isWorkChip1     = ( DM.ARAY[riRDF].GetCntStat(csUnkwn) ||  DM.ARAY[riRDF].GetCntStat(csMask) ||  DM.ARAY[riRDF].GetCntStat(csWait)) ; //졸려서 대충 때운거.
        bool isWorkChip2     = ( DM.ARAY[riRDF].GetCntStat(csUnkwn) && !DM.ARAY[riRDF].GetCntStat(csMask) && !DM.ARAY[riRDF].GetCntStat(csWait)) ;

        bool isVisnAtGrib   = DM.ARAY[riRDF].GetCntStat(csMask) && PM.GetValue(miWRD_YHed,pvWRD_YHedStripWait) <= MT_GetCmdPos(miWRD_YHed) ;





        //Judgment
        bool isCycleWorkStt =  DM.ARAY[riRDF].CheckAllStat(csUnkwn) && iReGribStep == 0 && isPSBWorkSttCon ; // Unkwn -> Wait
        bool isCycleReGrip  = !bAllNone &&  iReGribStep > 0 && bReGrip ;//거의 안씀.                  //
        bool isCycleAtVisn  =  DM.ARAY[riRDF].GetCntStat(csWait) && isVisnZone ;
        bool isCycleBfVisn  =  DM.ARAY[riRDF].GetCntStat(csWait) && isVisnZone ;
        bool isCycleWorkEnd = !bAllNone && !isWorkChip1 && DM.ARAY[riRAP].CheckAllStat(csNone);






        bool isCycleWork    = !bAllNone &&  iReGribStep > 0 && (isWorkChip1 ) &&
                            ((!bCheckXIdxPos||!bCheckYIdxPos) || isWorkChip2) &&
                               isWorkPsbCon ; //isWorkChip &&  ;
        bool isCycleWorkY   =  false ;//!bAllNone &&  iReGribStep > 0 && !bCheckYIdxPos ;//&& isWorkChip    ;//&& isWorkCon ;
        bool isCycleWorkEnd = !bAllNone && !isWorkChip1 && DM.ARAY[riRAP].CheckAllStat(csNone);

        bool isConEnd       =  bAllNone ;

//        if( DM.ARAY[riWRD].CheckAllStat(csNone) &&  )  EM_SetErr(eiWRD_Unknwn) ;
//        if(!DM.ARAY[riWRD].CheckAllStat(csNone) &&  )  EM_SetErr(eiWRD_Disp  ) ;

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleWorkStt ) {Trace(m_sPartName.c_str(),"CycleWorkStt      Stt"); Step.iSeq = scWorkStt     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleReGrip  ) {Trace(m_sPartName.c_str(),"CycleReGrip       Stt"); Step.iSeq = scReGrip      ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork    ) {Trace(m_sPartName.c_str(),"CycleWork         Stt"); Step.iSeq = scWork        ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWorkY   ) {Trace(m_sPartName.c_str(),"CycleWorkY        Stt"); Step.iSeq = scWorkY       ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isVisnAtGrib   ) {Trace(m_sPartName.c_str(),"CycleVisnAtGrib   Stt"); Step.iSeq = scVisnAtGrib  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWorkEnd ) {Trace(m_sPartName.c_str(),"CycleWorkEnd      Stt"); Step.iSeq = scWorkEnd     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd      ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default           :                        Trace(m_sPartName.c_str(),"default          End");Step.iSeq = scIdle ;  return false ;
        case scIdle       :                                                                                              return false ;
        case scWorkStt    : if(CycleWorkStt   ()){ Trace(m_sPartName.c_str(),"CycleWorkStt     End");Step.iSeq = scIdle ;} return false ;
        case scReGrip     : if(CycleReGrip    ()){ Trace(m_sPartName.c_str(),"CycleReGrip      End");Step.iSeq = scIdle ;} return false ;
        case scWork       : if(CycleWork      ()){ Trace(m_sPartName.c_str(),"CycleWork        End");Step.iSeq = scIdle ;} return false ;
        case scWorkY      : if(CycleWorkY     ()){ Trace(m_sPartName.c_str(),"CycleWorkY       End");Step.iSeq = scIdle ;} return false ;
        case scVisnAtGrib : if(CycleVisnAtGrib()){ Trace(m_sPartName.c_str(),"CycleVisnAtGrib  End");Step.iSeq = scIdle ;} return false ;
        case scWorkEnd    : if(CycleWorkEnd   ()){ Trace(m_sPartName.c_str(),"CycleWorkEnd     End");Step.iSeq = scIdle ;} return false ;
    }

}  */

bool CRailDie::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;


    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;
       //DM.ARAY[riRDF].SetDispItem(csNone   ,"자제없음"   ) ; DM.ARAY[riWRD].SetDispItem(csNone   ,"자제없음"   ) ;
       //DM.ARAY[riRDF].SetDispItem(csUnkwn  ,"작업전"     ) ; DM.ARAY[riWRD].SetDispItem(csUnkwn  ,"작업전"     ) ;
       //DM.ARAY[riRDF].SetDispItem(csWait   ,"BF비전대기" ) ; DM.ARAY[riWRD].SetDispItem(csRslt0  ,"방향불량"   ) ;
       //DM.ARAY[riRDF].SetDispItem(csMask   ,"칩놓기대기" ) ; DM.ARAY[riWRD].SetDispItem(csRslt1  ,"에폭시불량" ) ;
       //DM.ARAY[riRDF].SetDispItem(csWait2  ,"AT비전대기" ) ; DM.ARAY[riWRD].SetDispItem(csRslt2  ,"칩위치불량" ) ;
       //DM.ARAY[riRDF].SetDispItem(csWork   ,"작업끝"     ) ; DM.ARAY[riWRD].SetDispItem(csRslt3  ,"기준점불량" ) ;
       //                                                      DM.ARAY[riWRD].SetDispItem(csWork   ,"작업끝"     ) ;

        //Check
        int  iCNow     = DM.ARAY[riRDF].FindLastCol(csUnkwn) ;
        int  iCRvs     = OM.DevInfo.iColCnt - iCNow ;

//        bool isVisnZone      = PM.GetValue(miWRD_YHed , pvWRD_YHedStripWait)-0.2 < MT_GetEncPos(miWRD_YHed) ;
        bool isVisnZone      = PM.GetValue(miWRD_YHed , pvWRD_YHedStripWait) <= MT_GetCmdPos(miWRD_YHed) ;
        bool bCheckXIdxPos   = MT_CmprPos(miWRD_XIdx,GetWorkPos(miWRD_XIdx)) ;
        bool bCheckYIdxPos   = MT_CmprPos(miWRD_YIns,GetWorkPos(miWRD_YIns)) ;
        bool bNeedWork       = DM.ARAY[riRDF].GetCntStat(csUnkwn) && //자제 넣고 스타트 하면 csWait로 되지 않아 홀드 되는 것 때문에 추가.
                              !DM.ARAY[riRDF].GetCntStat(csWait ) &&
                              !DM.ARAY[riRDF].GetCntStat(csWait2) &&
                              !DM.ARAY[riRDF].GetCntStat(csMask ) ;

        int  iReGribStep     = DM.ARAY[riRDF].GetStep() ;
        bool bExstUnknwn     = DM.ARAY[riRDF].GetCntStat(csUnkwn ) ;
        bool bAllNone        = DM.ARAY[riRDF].CheckAllStat(csNone ) ;
        bool bPSBNone        = DM.ARAY[riPSF].CheckAllStat(csNone ) ;
        bool isPSBWorkSttCon = DM.ARAY[riPSF].GetStep() >= 1 || bPSBNone ;
        bool isWorkPsbCon    = bPSBNone ||

        !(iCRvs >= OM.DevOptn.iWRD_XWkCon && DM.ARAY[riPSF].GetStep() >= 1 ) ;
//        !(iCRvs >= OM.DevOptn.iWRD_XWkCon && DM.ARAY[riPSF].GetStep() == 2 ) ||
        bool bReGrip         = iReGribStep > 0 &&  iCRvs >= OM.DevOptn.iWRDRgCol[iReGribStep-1] && iReGribStep < OM.DevOptn.iWRD_XRgCnt + 1 ;

        //Judgment
        //첫위치 찾아가는놈.
        bool isCycleWorkStt =  DM.ARAY[riRDF].CheckAllStat(csUnkwn) && iReGribStep == 0 && isPSBWorkSttCon ;
        //리그립 하는놈.
        bool isCycleReGrip  = !bAllNone &&  iReGribStep > 0 && bReGrip ;//거의 안씀.
        //bf비전,Place,At비전 끝나고서 다음자제로 이동.
        bool isCycleWork    = !bAllNone && bExstUnknwn &&  iReGribStep > 0 && isWorkPsbCon && (!bCheckYIdxPos || !bCheckXIdxPos || bNeedWork) ;
        //AT 비전 검사.
        bool isCycleAtVisn  =  DM.ARAY[riRDF].GetCntStat(csWait2) && isVisnZone ;
        //BF 비전 검사.
        bool isCycleBfVisn  =  DM.ARAY[riRDF].GetCntStat(csWait ) && isVisnZone ;
        //배출.
        bool isCycleWorkEnd = !bAllNone && DM.ARAY[riRDF].CheckAllStat(csWork) && DM.ARAY[riPSF].CheckAllStat(csNone);
        //종료.
        bool isConEnd       =  bAllNone ;//&&DM.ARAY[riREV].GetCntStat(csWork) ;

//        if( DM.ARAY[riWRD].CheckAllStat(csNone) &&  )  EM_SetErr(eiWRD_Unknwn) ;
//        if(!DM.ARAY[riWRD].CheckAllStat(csNone) &&  )  EM_SetErr(eiWRD_Disp  ) ;

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleWorkStt ) {Trace(m_sPartName.c_str(),"CycleWorkStt Stt"); Step.iSeq = scWorkStt  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleReGrip  ) {Trace(m_sPartName.c_str(),"CycleReGrip  Stt"); Step.iSeq = scReGrip   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork    ) {Trace(m_sPartName.c_str(),"CycleWork    Stt"); Step.iSeq = scWork     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleAtVisn  ) {Trace(m_sPartName.c_str(),"CycleAtVisn  Stt"); Step.iSeq = scAtVisn   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleBfVisn  ) {Trace(m_sPartName.c_str(),"CycleBfVisn  Stt"); Step.iSeq = scBfVisn   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWorkEnd ) {Trace(m_sPartName.c_str(),"CycleWorkEnd Stt"); Step.iSeq = scWorkEnd  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd       ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default        :                     Trace(m_sPartName.c_str(),"default      End");Step.iSeq = scIdle ;  return false ;
        case scIdle    :                                                                                         return false ;
        case scWorkStt : if(CycleWorkStt()){ Trace(m_sPartName.c_str(),"CycleWorkStt End");Step.iSeq = scIdle ;} return false ;
        case scReGrip  : if(CycleReGrip ()){ Trace(m_sPartName.c_str(),"CycleReGrip  End");Step.iSeq = scIdle ;} return false ;
        case scWork    : if(CycleWork   ()){ Trace(m_sPartName.c_str(),"CycleWork    End");Step.iSeq = scIdle ;} return false ;
        case scAtVisn  : if(CycleAtVisn ()){ Trace(m_sPartName.c_str(),"CycleAtVisn  End");Step.iSeq = scIdle ;} return false ;
        case scBfVisn  : if(CycleBfVisn ()){ Trace(m_sPartName.c_str(),"CycleBfVisn  End");Step.iSeq = scIdle ;} return false ;
        case scWorkEnd : if(CycleWorkEnd()){ Trace(m_sPartName.c_str(),"CycleWorkEnd End");Step.iSeq = scIdle ;} return false ;
    }

}

bool CRailDie::CycleVisn(EN_VISN_ID _Id, bool _bInit, bool _bDryRun)
{
    if(_bInit) Step.iVisn = 10 ;

    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmVisn.OnDelay(Step.iVisn && Step.iVisn == PreStep.iVisn && CheckStop() && !OM.MstOptn.bDebugMode, 4000 )) {
        EM_SetErr(eiHED_VisnTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iVisn=%02d" , __FUNC__ , Step.iVisn );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iVisn = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iVisn=%02d" , __FUNC__ , Step.iVisn );
    if(Step.iVisn != PreStep.iVisn) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iVisn = Step.iVisn ;

    //Cycle.
    switch (Step.iVisn) {

        default : if(Step.iVisn) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iVisn=%02d" , __FUNC__ , Step.iVisn );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iVisn = 0 ;
                  return true ;

        case  10: SV_SetLight(_Id); //조명 세팅. rs-232라 느림. 미리 보내 놓아야함.
                  m_tmVisnTemp.Clear();
                  Step.iVisn++ ;
                  return false ;

        case  11: if(_Id == viRbf && !m_tmVisnTemp.OnDelay(true , OM.DevOptn.iRalBfVsBfDly)) return false ;
                  if(_Id == viRat && !m_tmVisnTemp.OnDelay(true , OM.DevOptn.iRalAtVsBfDly)) return false ;

                  SV_Grab(_Id);
                  m_tmVisnTemp.Clear() ;
                  Step.iVisn++;
                  return false;

        case  12: if(_Id == viRbf) {
                      if(m_tmVisnTemp.OnDelay(!OM.MstOptn.bDebugMode, OM.DevOptn.iRalBfVsGrabTO)) {
                          EM_SetErr(eiHED_RbfVsGrabFail);
                          Step.iVisn = 0 ;
                          return true ;
                      }
                  }
                  if(_Id == viRat) {
                      if(m_tmVisnTemp.OnDelay(!OM.MstOptn.bDebugMode, OM.DevOptn.iRalAtVsGrabTO)) {
                          EM_SetErr(eiHED_RatVsGrabFail);
                          Step.iVisn = 0 ;
                          return true ;
                      }
                  }

                  if(!SV_GetGrabEnd(_Id)) return false ;

                  if(!_bDryRun) SV_Inspect(_Id,false);
                  m_tmVisnTemp.Clear() ;
                  Step.iVisn++;
                  return false ;

        case  13: if(_Id == viRbf && m_tmVisnTemp.OnDelay(!OM.MstOptn.bDebugMode,OM.DevOptn.iRalBfVsInspTO )) {
                      EM_SetErr(eiHED_RbfVsInspFail);
                      Step.iVisn = 0 ;
                      return true ;
                  }
                  if(_Id == viRat && m_tmVisnTemp.OnDelay(!OM.MstOptn.bDebugMode,OM.DevOptn.iRalAtVsInspTO )) {
                      EM_SetErr(eiHED_RatVsInspFail);
                      Step.iVisn = 0 ;
                      return true ;
                  }
                  if(!_bDryRun && !SV_GetInspEnd(_Id)) return false ;

                  Step.iVisn=0;
                  return true ;

    }
}

//One Cycle.
bool CRailDie::CycleWorkStt(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
//        DM.ARAY[riRDF].ChangeStat(csWait,csUnkwn);
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

    int r1,r2 ;
    int r,c ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY (yWRD_Vaccum , false);
                  MoveActr(aiWRD_Clamp , ccBwd);
                  //MoveActr(aiWRD_CAlign, ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: 
                  if(!MoveActr(aiWRD_Clamp , ccBwd))    return false ;
//                  if(!MoveActr(aiWRD_Align, ccBwd)) return false ;
                  MoveMotr(miWRD_XIdx , piWRD_XIdxWait );
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRD_XIdx , piWRD_XIdxWait )) return false ;
                  MoveActr(aiWRD_Clamp , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiWRD_Clamp , ccFwd))   return false ;
                  //if(!MoveActr(aiWRD_CAlign, ccBwd)) return false ;
                  MoveMotr(miWRD_XIdx , piWRD_XIdxWorkStt );
                  MoveMotr(miWRD_YIns , piWRD_YInsWorkStt );
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miWRD_XIdx , piWRD_XIdxWorkStt )) return false ;
                  if(!MoveMotr(miWRD_YIns , piWRD_YInsWorkStt )) return false ;
                  IO_SetY(yWRD_Vaccum , true);
                  Step.iCycle++;
                  return false ;

        case  15:
                  //MoveActr(aiWRD_CAlign, ccFwd);
                  DM.ARAY[riRDF].SetStep(1);
                  Step.iCycle++;
                  return false ;

        case  16: //if(!MoveActr(aiWRD_CAlign, ccFwd)) return false ;

                  FindChip(riRDF,r,c);
                  if(OM.MstOptn.bIgnrDieAttach) {
                      for (int i= 0; i<OM.DevInfo.iRowCnt; i++){
                          DM.ARAY[riRDF].SetStat(i,c,csWork);
                          Step.iCycle = 0 ;
                          return true ;
                      }
                  }

                  for (int i= 0; i<OM.DevInfo.iRowCnt; i++){
                      if(DM.ARAY[riRDV].GetStat(i,c) == csRslt0) DM.ARAY[riRDF].SetStat(i,c,csWork);
                  }
//                  FindChip(riWRD,r1,c1);
                  if(DM.ARAY[riRDV].GetStat(r,c) == csUnkwn) { //Start Position Up
                      DM.ARAY[riRDF].SetStat(r,c,csWait);
                  }

                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CRailDie::CycleReGrip(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO);
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
    static double dReGrip  = 0.0 ;
    double dTemp = 0.0 ;    
    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: dReGrip  = 0.0 ;
                  dReGrip  = OM.DevOptn.iWRDRgDis[DM.ARAY[riRDF].GetStep()-1] * OM.DevInfo.dColPitch;
                  dTemp    = MT_GetCmdPos(miWRE_XIdx) - dReGrip;
                  if( dTemp <= MT_GetMinPos(miWRD_XIdx) ) {
                      EM_SetErr(eiWRD_RegripPosMinOver);
                      Step.iCycle = 0 ;
                      return true;
                  }

                  MoveActr(aiWRD_Clamp  , ccBwd);
                  //MoveActr(aiWRD_CAlign , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiWRD_Clamp  , ccBwd)) return false ;

                  MT_GoIncRun(miWRD_XIdx,-dReGrip  + OM.MstOptn.dRegripOfs); //충돌 체크 안됨.
                  Step.iCycle++;
                  return false;

        case  12: if(!MT_GetStopInpos(miWRD_XIdx)) return false ; //abs 로 바꿀까 말까 고민중...
                  //if(!MoveActr(aiWRD_CAlign , ccBwd)) return false ;

                  MoveActr(aiWRD_Clamp, ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiWRD_Clamp, ccFwd)) return false ;
                  //MoveActr(aiWRD_CAlign, ccFwd);
                  Step.iCycle++;
                  return false ;

        case  14: //if(!MoveActr(aiWRD_CAlign, ccFwd)) return false ;
                  DM.ARAY[riRDF].SetStep(DM.ARAY[riRDF].GetStep()+1);
//                  if(DM.ARAY[riREV].GetCntStat(csUnkwn)) {
//                      MoveMotr(miPRB_XIdx , piPRB_XIdxWork );
//                      MoveMotr(miPRB_YIns , piPRB_YInsWork );
//                  }
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CRailDie::CycleWork(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
//        DM.ARAY[riRDF].ChangeStat(csWait,csUnkwn);
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
    int r1,c1 ;
    static bool bVsGood = false ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //m_bInsYReady = false ;
                  IO_SetY (yWRD_Vaccum , false);
                  if(GetMotrPos(miWRD_XIdx,piWRD_XIdxWork) >= MT_GetMaxPos(miWRD_XIdx) ){
                      EM_SetErr(eiWRD_WorkPosMaxOver);
                      Step.iCycle = 0;
                      return true;
                  }

                  //if(!MT_CmprPos(miWRD_XIdx,GetWorkPos(miWRD_XIdx))) MoveActr(aiWRD_CAlign   , ccBwd);

                  MoveActr(aiWRD_Clamp   , ccFwd);
                  //MoveActr(aiWRD_CAlign  , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiWRD_Clamp   , ccFwd)) return false ;
                  //if(!MoveActr(aiWRD_CAlign  , ccBwd)) return false ;
                  //if(!MT_CmprPos(miWRD_XIdx,GetWorkPos(miWRD_XIdx))&&!MoveActr(aiWRD_CAlign  , ccBwd)) return false ;

                  MoveMotr(miWRD_XIdx , piWRD_XIdxWork );
                  MoveMotr(miWRD_YIns , piWRD_YInsWork );
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRD_XIdx , piWRD_XIdxWork )) return false ;
                  if(!MoveMotr(miWRD_YIns , piWRD_YInsWork )) return false ;
                  IO_SetY (yWRD_Vaccum , true);
                  //MoveActr(aiWRD_CAlign   , ccFwd);
                  //m_tmTemp.Clear() ;
                  Step.iCycle++;
                  return false;

        case  13: //if(!m_tmTemp.OnDelay(true , OM.DevOptn.iRalBfVsBfDly)) return false ;
                  //if(!MoveActr(aiWRD_CAlign   , ccFwd)) return false ;
                  FindChip(riRDF,r,c);
                  if(OM.MstOptn.bIgnrDieAttach) {
                      for (int i= 0; i<OM.DevInfo.iRowCnt; i++){
                          DM.ARAY[riRDF].SetStat(i,c,csWork);
                          Step.iCycle = 0 ;
                          return true ;
                      }
                  }

                  //방향페일 마스킹.
                  for (int i= 0; i<OM.DevInfo.iRowCnt; i++){
                      if(DM.ARAY[riRDV].GetStat(i,c) == csRslt0) DM.ARAY[riRDF].SetStat(i,c,csWork);
                  }

                  if(DM.ARAY[riRDV].GetStat(r,c) == csUnkwn) { //Start Position Up
                      DM.ARAY[riRDF].SetStat(r,c,csWait);
                  }

                                                                     /*에폭시*/
                  //if(OM.DevOptn.iRalBfVsFailCnt && DM.ARAY[riWRD].GetCntStat(csRslt1) >= OM.DevOptn.iRalBfVsFailCnt ) {
                  //    EM_SetErr(eiWRD_RalBfVsFailCnt);
                  //    Step.iCycle = 0 ;
                  //    return true ;
                  //}
                  //                                                  /*다이페일*/
                  //if(OM.DevOptn.iRalAtVsFailCnt && DM.ARAY[riWRD].GetCntStat(csRslt2) >= OM.DevOptn.iRalAtVsFailCnt ) {
                  //    EM_SetErr(eiWRD_RalAtVsFailCnt);
                  //    Step.iCycle = 0 ;
                  //    return true ;
                  //}



//                  m_bInsYReady = true ;

                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CRailDie::CycleAtVisn(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
//        DM.ARAY[riRDF].ChangeStat(csWait,csUnkwn);
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

    static int iVisnFailCnt = 0 ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: m_bInsYReady = false ;
                  MoveActr(aiWRD_Clamp   , ccFwd) ;
                  //MoveActr(aiWRD_CAlign  , ccFwd) ;
                  //MoveActr(aiWRD_CAlign  , ccBwd) ;
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiWRD_Clamp   , ccFwd)) return false ;
                  //if(!MoveActr(aiWRD_CAlign  , ccFwd)) return false ;
                  MoveMotr(miWRD_YIns , piWRD_YInsWork );
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRD_YIns , piWRD_YInsWork )) return false ;

                  CycleVisn(viRat , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case  13: if(!CycleVisn(viRat , false , OM.MstOptn.bDryRun)) return false ;

                  if(!OM.MstOptn.bDryRun)SV_GetRslt(viRat,&RsltRat);
                  else {
                      //RsltRat.bRslt  = true ;
                      RsltRat.iRet   = 0 ;
                      RsltRat.fRsltX = 0.0 ;
                      RsltRat.fRsltY = 0.0 ;
                      RsltRat.fRsltT = 0.0 ;
                  }



                    //EM_SetErr(eiHED_RalAtVsThchFail);
                  RsltRat.iRet == veRatOk ? sTemp = "GOOD" : sTemp = "NG" ;
                  sTemp += AnsiString(" X=")+ RsltRat.fRsltX + AnsiString(" Y=") + RsltRat.fRsltY + AnsiString(" T=") + RsltRat.fRsltT ;
                  Trace("AfterVisn",sTemp.c_str());

                  FindChip(riRDF,r,c);

                  if(RsltRat.iRet == veRatOk ) { //Good
                      DM.ARAY[riRDV].SetStat(r,c,csWork);
                      DM.ARAY[riRDF].SetStat(r,c,csWork);
                      iVisnFailCnt = 0 ;
                  }
                  else if(RsltRat.iRet == veRatLcpFail ) { //기준점 관련 불량.
                      DM.ARAY[riRDV].SetStat(r,c,csRslt2);
                      DM.ARAY[riRDF].SetStat(r,c,csWork);
                      iVisnFailCnt++;
                  }
                  else if(RsltRat.iRet == veRatRctFail ||
                          RsltRat.iRet == veRatLen     ||
                          RsltRat.iRet == veRatRctAng ) { //칩관련 불량.
                      DM.ARAY[riRDV].SetStat(r,c,csRslt2);
                      DM.ARAY[riRDF].SetStat(r,c,csWork);
                      iVisnFailCnt++;
                  }
                  else if(RsltRat.iRet == veRatRctOfs ) { //자제 안착 오프셑 불량.
                      DM.ARAY[riRDV].SetStat(r,c,csRslt2);
                      DM.ARAY[riRDF].SetStat(r,c,csWork);
                      iVisnFailCnt++;
                  }

                  Trace("AT VISION",("X=" + AnsiString(RsltRat.fRsltX)).c_str() );
                  Trace("AT VISION",("Y=" + AnsiString(RsltRat.fRsltY)).c_str() );
                  Trace("AT VISION",("T=" + AnsiString(RsltRat.fRsltT)).c_str() );


                  SPCS.WriteAtVsDataCsv(&DM.ARAY[riRDF],&RsltRat);

                  if(OM.DevOptn.iRalAtVsFailCnt && iVisnFailCnt >= OM.DevOptn.iRalAtVsFailCnt ) {
                      EM_SetErr(eiHED_RatVsFailCnt);
                      iVisnFailCnt = 0 ;
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  Step.iCycle = 0 ;
                  return true ;

    }
}

bool CRailDie::CycleBfVisn(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
//        DM.ARAY[riRDF].ChangeStat(csWait,csUnkwn);
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

//    static int iVisnFailCnt = 0 ; //제거 에폭시 불량은 그냥 에러 띄우고 버튼으로 눌러서 패스 할지 한다.

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10:


                  MoveActr(aiWRD_Clamp   , ccFwd) ;
                  //MoveActr(aiWRD_CAlign  , ccFwd) ;
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiWRD_Clamp   , ccFwd)) return false ;
                  //if(!MoveActr(aiWRD_CAlign  , ccFwd)) return false ;
                  MoveMotr(miWRD_YIns , piWRD_YInsWork );
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRD_YIns , piWRD_YInsWork )) return false ;

                  CycleVisn(viRbf , true , OM.MstOptn.bDryRun) ;
                  Step.iCycle++;
                  return false;

        case  13: if(!CycleVisn(viRbf , false , OM.MstOptn.bDryRun))return false ;

                  FindChip(riRDF,r,c);

                  if(!OM.MstOptn.bDryRun)SV_GetRslt(viRbf,&RsltRbf);
                  else {
                      //<sun>  RsltRbf.bRslt  = true ;
                      RsltRbf.iRet   = 0 ;
                      RsltRbf.fRsltX = 0.0 ;
                      RsltRbf.fRsltY = 0.0 ;
                      RsltRbf.fRsltT = 0.0 ;
                  }





                  Trace("BF VISION",("X=" + AnsiString(RsltRbf.fRsltX)).c_str() );
                  Trace("BF VISION",("Y=" + AnsiString(RsltRbf.fRsltY)).c_str() );
                  Trace("BF VISION",("T=" + AnsiString(RsltRbf.fRsltT)).c_str() );

                  if(OM.MstOptn.bIgnrBfVision) {
                      DM.ARAY[riRDF].SetStat(r,c,csMask);
                      RsltRbf.fRsltX = 0.0 ;
                      RsltRbf.fRsltY = 0.0 ;
                      RsltRbf.fRsltT = 0.0 ;
                      //m_bEpxFailIgnr = false ;
                      Step.iCycle = 0 ;
                      return true ;
                  }



                  if(RsltRbf.iRet == veRbfOk ) {    //EM_SetErr(eiHED_RalBfVsThchFail);
                      //범위 오버.
                      if( fabs(RsltRbf.fRsltX) > (OM.DevInfo.dWf_PitchX)/2 ||
                          fabs(RsltRbf.fRsltY) > (OM.DevInfo.dWf_PitchY)/2 ||
                          fabs(RsltRbf.fRsltT) > OM.CmnOptn.dHedTLmtErr ) {
                          RsltRbf.fRsltX = 0.0 ;
                          RsltRbf.fRsltY = 0.0 ;
                          RsltRbf.fRsltT = 0.0 ;
                          EM_SetErr(eiRAD_VisnRlstOver);
                          m_bEpxFailIgnr = false ;
                          Step.iCycle = 0 ;
                          return true ;
                      }
                      DM.ARAY[riRDF].SetStat(r,c,csMask);
                  }
                  else if(RsltRbf.iRet == veRbfTsdFail) {
                      //귀찮응게 일단...eiRAD_VisnRlstOver이에러로.
                      RsltRbf.fRsltX = 0.0 ;
                      RsltRbf.fRsltY = 0.0 ;
                      RsltRbf.fRsltT = 0.0 ;
                      EM_SetErr(eiHED_RbfVsFailCnt);
                      m_bEpxFailIgnr = false ;
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  else if(RsltRbf.iRet == veRbfNoPkg) { //어차피 자제 없음은 오리 에서 걸르지만 혹시나 빠진경우.
                      //if(OM.CmnOptn.bIgnrRalBfVsEpoxyFail || m_bEpxFailIgnr) {
                      //    DM.ARAY[riRDF].SetStat(r,c,csMask);
                      //    m_bEpxFailIgnr = false ;
                      //}
                      //else {
                          DM.ARAY[riRDV].SetStat(r,c,csRslt1);
                          EM_SetErr(eiHED_RbfVsFailCnt); //에폭시는 페일 카운트 없고 한번 나면 스탑 해서 오퍼확인.
                          Step.iCycle = 0 ;
                          return true ;
                      //}
                  }
                  else if(RsltRbf.iRet == veRbfLCPFail) {
                       //귀찮응게 일단...eiRAD_VisnRlstOver이에러로.
                      RsltRbf.fRsltX = 0.0 ;
                      RsltRbf.fRsltY = 0.0 ;
                      RsltRbf.fRsltT = 0.0 ;
                      EM_SetErr(eiHED_RbfVsFailCnt);
                      m_bEpxFailIgnr = false ;
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  else if(RsltRbf.iRet == veRbfTsdFail2) {
                       //귀찮응게 일단...eiRAD_VisnRlstOver이에러로.
                      RsltRbf.fRsltX = 0.0 ;
                      RsltRbf.fRsltY = 0.0 ;
                      RsltRbf.fRsltT = 0.0 ;
                      EM_SetErr(eiHED_RbfVsFailCnt);
                      m_bEpxFailIgnr = false ;
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  else if(RsltRbf.iRet == veRbfEpoxy) { //에폭시 불량.
                      if(OM.CmnOptn.bIgnrRalBfVsEpoxyFail || m_bEpxFailIgnr) {
                          DM.ARAY[riRDF].SetStat(r,c,csMask);
                          m_bEpxFailIgnr = false ;
                      }
                      else {
                          DM.ARAY[riRDV].SetStat(r,c,csRslt1);
                          EM_SetErr(eiHED_RbfVsFailCnt); //에폭시는 페일 카운트 없고 한번 나면 스탑 해서 오퍼확인.
                          Step.iCycle = 0 ;
                          return true ;
                      }
                  }
                  m_bEpxFailIgnr = false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CRailDie::CycleWorkEnd(void)
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

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiWRD_Clamp   , ccFwd); //
                  IO_SetY (yWRD_Vaccum , false);
                  //MoveActr(aiWRD_CAlign  , ccBwd); //

                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiWRD_Clamp   , ccFwd)) return false ;
                  //if(!MoveActr(aiWRD_CAlign  , ccBwd)) return false ;
//                  MoveMotr(miWRD_XIdx,piWRD_XIdxPull);
//                  Step.iCycle++;
//                  return false;
//
//        case  12: if(!MoveMotr(miWRD_XIdx,piWRD_XIdxPull)) return false ;
//                  MoveActr(aiWRD_Clamp  , ccBwd); //Pull 포지션을 셋업창에 하나 두어주자.
//                  Step.iCycle++;
//                  return false;
//
//        case  13: if(!MoveActr(aiWRD_Clamp  , ccBwd)) return false ;
//                  MoveMotr(miWRD_XIdx,piWRD_XIdxBack);
//                  Step.iCycle++;
//                  return false;
//
//        case  14: if(!MoveMotr(miWRD_XIdx,piWRD_XIdxBack)) return false ;
//                  MoveActr(aiPRB_Clamp, ccFwd);
//                  Step.iCycle++;
//                  return false ;
//
//        case  15: if(!MoveActr(aiPRB_Clamp, ccFwd)) return false ;
                  MoveMotr(miWRD_XIdx,piWRD_XIdxOut);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRD_XIdx,piWRD_XIdxOut)) return false ;
                  MoveActr(aiWRD_Clamp  , ccBwd);
//                  MoveActr(aiWRD_Align  , ccBwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiWRD_Clamp  , ccBwd)) return false ;
                  MoveMotr(miWRD_XIdx,piWRD_XIdxWait);

                  DM.ShiftArrayData(riRDV,riPSV);
                  DM.ARAY[riPSF].SetLotNo(DM.ARAY[riRDF].GetLotNo());
                  DM.ARAY[riPSF].SetID   (DM.ARAY[riRDF].GetID   ());
                  DM.ARAY[riRDF].SetStat(csNone );
                  DM.ARAY[riPSF].SetStat(csUnkwn);

                  Step.iCycle++;

        case  14: if(!MoveMotr(miWRD_XIdx,piWRD_XIdxWait)) return false ;





                  Step.iCycle = 0 ;
                  return true ;
    }
}




double CRailDie::GetLastCmd(EN_MOTR_ID _iMotr)
{
    double _dLastIdxPos = 0.0 ;
    if(!MT_GetAlarm(_iMotr) && !MT_GetNLimSnsr(_iMotr) && !MT_GetPLimSnsr(_iMotr) ) _dLastIdxPos = MT_GetCmdPos(_iMotr) ;
    else                                                                            _dLastIdxPos = GetMotrPos  (_iMotr , (EN_PSTN_ID)0) ;

    return _dLastIdxPos ;
}

bool CRailDie::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CRailDie::CheckStop()
{
    if(!MT_GetStop(miWRD_XIdx)) return false ;
    if(!MT_GetStop(miWRD_YIns)) return false ;
//    if(!AT_Done(aiWRD_Align )) return false ;
    if(!AT_Done(aiWRD_Clamp )) return false ;

    return true ;
}

void CRailDie::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() + ".INI";

    if ( IsLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos " , m_dLastIdxPos );
        UserINI.Load(sPath, "Member" , "m_bInsYReady  " , m_bInsYReady  );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos " , m_dLastIdxPos );
        UserINI.Save(sPath, "Member" , "m_bInsYReady  " , m_bInsYReady  );
    }
}
//---------------------------------------------------------------------------
