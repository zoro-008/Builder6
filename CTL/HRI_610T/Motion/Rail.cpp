//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Rail.h"
#include "PreBuff.h"
#include "LaserEO.h"
//---------------------------------------------------------------------------
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CRail RAL;

CRail::CRail(void)
{

}

CRail::~CRail (void)
{

}

void CRail::Init()
{                               
    m_sPartName = "Rail " ;
    Reset();
    Load(true);

    InitCycleName();
}
void CRail::Close()
{
    Load(false);
}

void CRail::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));

    m_bNeedStrip = false ;
}

void CRail::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}



bool CRail::FindChip(int &r , int &c )
{
    //    r = DM.ARAY[riLD1].FindFrstRow(csUnkwn) ;
    return false;


    return false ;
}

double CRail::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = MT_GetCmdPos(_iMotr) ;


    if(_iMotr == miRAL_XLStp){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piRAL_XLStpWait  : dPos = PM.GetValue (_iMotr , pvRAL_XLStpWait      ); break ;
            case piRAL_XLStpWork  : dPos = PM.GetValue (_iMotr , pvRAL_XLStpWork      ); break ;
        }
    }

    else if(_iMotr == miRAL_XVStp){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piRAL_XVStpWait  : dPos = PM.GetValue (_iMotr , pvRAL_XVStpWait      ); break ;
            case piRAL_XVStpWork  : dPos = PM.GetValue (_iMotr , pvRAL_XVStpWork      ); break ;
        }
    }
    else if(_iMotr == miRAL_YCvsn){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piRAL_YCvsnWait  : dPos = PM.GetValue (_iMotr , pvRAL_YCvsnWait      ); break ;
            case piRAL_YCvsnWork  : dPos = PM.GetValue (_iMotr , pvRAL_YCvsnWork      ); break ;
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

    /*
    if(_iMotr == miRAL_XIdx){
        if(isMotrNeg && isIdxUp) {sMsg = "Index Pinger is Up"; bRet = false ;}
        switch(_iPstnId) {
            default                :  break ;
            case piRAL_XIdxWait    :  break ;
            case piRAL_XIdxWorkStt :  break ;
            case piRAL_XIdxOut     :  break ;
            case piRAL_XIdxWork    :  break ;
        }
    }*/

    if(_iMotr == miRAL_XLStp){
        switch(_iPstnId) {
            default               :  { sMsg = "Not " +AnsiString(MT_GetName(miLDR_ZClmp))+"'s Postion" ; bRet = false ;} break ;
            case piRAL_XLStpWait  :  break ;
            case piRAL_XLStpWork  :  break ;
        }
    }

    else if(_iMotr == miRAL_XVStp){
        switch(_iPstnId) {
            default               :  { sMsg = "Not " +AnsiString(MT_GetName(miLDR_ZClmp))+"'s Postion" ; bRet = false ;} break ;
            case piRAL_XVStpWait  :  break ;
            case piRAL_XVStpWork  :  break ;
        }
    }
    else if(_iMotr == miRAL_YCvsn){
        if(IO_GetX(xRAL_LMask) || IO_GetX(xRAL_VMask)) { sMsg = "Remove Rail Mask!" ; bRet = false ; }
        switch(_iPstnId) {
            default               : { sMsg = "Not " +AnsiString(MT_GetName(miLDR_ZClmp))+"'s Postion" ; bRet = false ;} break ;
            case piRAL_YCvsnWait  : break ;
            case piRAL_YCvsnWork  : break ;
        }
    }
    else {
        bRet = false ;
    }
    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool  CRail::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd )) return true ;
    
    bool bRet = true ;
    AnsiString sMsg ;

    if(_iActr == aiRAL_LStpUpDn) {
        //if(_bFwd) {
        //    if(isXMoving) { sMsg = AnsiString(MT_GetName(miRAL_XIdx))+" is Moving" ; bRet = false ;}
        //}
    }
    else if(_iActr == aiRAL_LStnUpDn) {
    }
    else if(_iActr == aiRAL_LAlnFwBw) {
    }
    else if(_iActr == aiRAL_VStpUpDn) {
    }
    else if(_iActr == aiRAL_VStnUpDn) {
    }
    else if(_iActr == aiRAL_VAlnFwBw) {

    }
    else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
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

bool CRail::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 15000 )) {
        EM_SetErrMsg(eiPRT_HomeTo , m_sPartName.c_str());//EM_SetErr(eiRAL_HomeTo);
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

        case 10: AT_MoveCyl(aiRAL_LAlnFwBw  ,ccBwd);
                 AT_MoveCyl(aiRAL_VAlnFwBw  ,ccBwd);
                 AT_MoveCyl(aiRAL_LStpUpDn  ,ccFwd);
                 AT_MoveCyl(aiRAL_VStpUpDn  ,ccFwd);
                 IO_SetY(yRAL_FeedAc , false);

                 Step.iHome++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiRAL_LAlnFwBw ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiRAL_VAlnFwBw ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiRAL_LStpUpDn ,ccFwd)) return false ;
                 if(!AT_MoveCyl(aiRAL_VStpUpDn ,ccFwd)) return false ;

                 AT_MoveCyl(aiRAL_LStnUpDn  ,ccBwd);
                 AT_MoveCyl(aiRAL_VStnUpDn  ,ccBwd);

                 Step.iHome++;
                 return false ;

        case 12: if(!AT_MoveCyl(aiRAL_LStnUpDn  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiRAL_VStnUpDn  ,ccBwd)) return false ;


                 MT_SetHomeEnd(miRAL_XLStp , false);
                 MT_SetHomeEnd(miRAL_XVStp , false);


                 MT_DoHome(miRAL_XLStp);
                 MT_DoHome(miRAL_XVStp);

                 Step.iHome++ ;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miRAL_XLStp)) return false;
                 if(!MT_GetHomeEnd(miRAL_XVStp)) return false;

                 MT_GoAbsMan(miRAL_XLStp, PM.GetValue(miRAL_XLStp , pvRAL_XLStpWork));
                 MT_GoAbsMan(miRAL_XVStp, PM.GetValue(miRAL_XVStp , pvRAL_XVStpWork));
                 Step.iHome++ ;
                 return false ;

        case 14: if(!MT_GoAbsMan(miRAL_XLStp, PM.GetValue(miRAL_XLStp , pvRAL_XLStpWork)))return false ;
                 if(!MT_GoAbsMan(miRAL_XVStp, PM.GetValue(miRAL_XVStp , pvRAL_XVStpWork)))return false ;

                 if(IO_GetX(xRAL_LMask) || IO_GetX(xRAL_VMask)){ //||
                   //!DM.ARAY[riPRB].CheckAllStat(csNone) || !DM.ARAY[riLSR].CheckAllStat(csNone) ||
                   //!DM.ARAY[riVSN].CheckAllStat(csNone) || !DM.ARAY[riPSB].CheckAllStat(csNone) ||
                   // IO_GetX(xPRB_PkgIn1) || IO_GetX(xPRB_Pkg2  ) || IO_GetX(xRAL_LPkg3) ||
                   // IO_GetX(xRAL_VPkg4 ) || IO_GetX(xPSB_PkgIn5) || IO_GetX(xPSB_Pkg5 ) || IO_GetX(xPSB_PkgOut6 )){ //마스크 있으면 홈 않잡음. 자제 있음은 신경 안쓴다.

                     MT_SetHomeEnd(miRAL_YCvsn , true);
                     MT_SetPos(miRAL_YCvsn , PM.GetValue(miRAL_YCvsn , pvRAL_YCvsnWork));
                     Step.iHome = 0;
                     return true ;
                 }


                 

                 MT_SetHomeEnd(miRAL_YCvsn , false);
                 MT_DoHome(miRAL_YCvsn);

                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GetHomeEnd(miRAL_YCvsn)) return false;
                 MT_GoAbsMan(miRAL_YCvsn, PM.GetValue(miRAL_YCvsn , pvRAL_YCvsnWork));
                 Step.iHome++;
                 return false ;

        case 16: if(!MT_GoAbsMan(miRAL_YCvsn, PM.GetValue(miRAL_YCvsn , pvRAL_YCvsnWork)))return false ;
                 Step.iHome = 0;
                 return true ;



    }
}

bool CRail::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    //static CDelayTimer tmWait ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

//프리버퍼 오리검사 레이져조사  마킹검사필요  비젼검사
//Unkwn -> Mask     Work        Wait          Good
//                                            Fail

    if (Step.iSeq == 0) {
        if(Stat.bReqStop )return false ;

        //bool bLSRNeedToWork  = DM.ARAY[riLSR].CheckAllStat(csMask);
        bool bRailExstStrip  =!DM.ARAY[riPRB].CheckAllStat(csNone)||
                              !DM.ARAY[riLSR].CheckAllStat(csNone)||
                              !DM.ARAY[riVSN].CheckAllStat(csNone);
        bool bVSNNeedToWork  = DM.ARAY[riVSN].GetCntStat  (csWait);
        bool bLSRNeedToWork  = DM.ARAY[riLSR].CheckAllStat(csMask);

        bool bExstStripToWork = !PRB.GetWorkEnd() ;


        //ReadyToFeed는 Feeding 동작 하면서 자제가 빠져나갈 예정이거나 아예없는 조건을 고려 한다.
        //다음 위치에 자제가 있는지 있으면 피딩 동작으로 빠져나갈수 있는지를 고려 하여 조건을 잡는다.
        //즉 피딩하려 할때 PSB존에 작업이 안끝났으면 VSN존에 있는 자제는 FEED해 봤자 나갈 수가 없고 차례대로 뒤에 놈들은 작업이 안된다.
                              //비젼작업이 다끝나고 포스트 버퍼에 자제가 없거나                                비젼존에 자제가 없을때
        bool bLSRSkipToFeed  =( DM.ARAY[riLSR].CheckAllStat(csMask) && OM.CmnOptn.bSkipLaser);
        bool bVSNReadyToFeed =(!DM.ARAY[riVSN].GetCntStat  (csWait) && DM.ARAY[riPSB].CheckAllStat(csNone)) /*&& !DM.ARAY[riVSN].CheckAllStat(csNone)*/|| DM.ARAY[riVSN].CheckAllStat(csNone); //
        bool bLSRReadyToFeed =((DM.ARAY[riLSR].CheckAllStat(csWork) && bVSNReadyToFeed) || bLSRSkipToFeed ) || DM.ARAY[riLSR].CheckAllStat(csNone);
        bool bPRBReadyToFeed =( DM.ARAY[riPRB].CheckAllStat(csMask) ||!bExstStripToWork                   );//|| DM.ARAY[riPRB].CheckAllStat(csNone)PRB을 안기다릴경우); //스트립이 들어왔을때는 비젼검사 금방끝나니깐 기다리기 위해.


                              //굿과 페일만 있어야 준비됌.

        //에어스위치들 딜레이 먹게 OnDle

        bool isCycleFeed  =  bRailExstStrip && bPRBReadyToFeed && bLSRReadyToFeed && bVSNReadyToFeed ;  //스트립 굴리기.
        bool isCycleBind  = (bLSRNeedToWork && AT_Complete(aiRAL_LStnUpDn , ccBwd)) || //스트립 스테이션 올리기.
                            (bVSNNeedToWork && AT_Complete(aiRAL_VStnUpDn , ccBwd)) ;

        bool isCycleEnd   = DM.ARAY[riLSR].CheckAllStat(csNone) &&
                            DM.ARAY[riVSN].CheckAllStat(csNone) && PRB.GetWorkEnd();


        //Unknown Strip
        if (IO_GetX(xRAL_LPkg3) && DM.ARAY[riLSR].CheckAllStat(csNone)) EM_SetErrMsg(eiPKG_Unknwn , DM.ARAY[riLSR].GetName().c_str()) ;
        if (IO_GetX(xRAL_VPkg4) && DM.ARAY[riVSN].CheckAllStat(csNone)){
            EM_SetErrMsg(eiPKG_Unknwn , DM.ARAY[riVSN].GetName().c_str()) ;
        }
        bool aa,dd;

        //Dissapear Strip
        if (!OM.MstOptn.bDryRun) {
            if (!IO_GetX(xRAL_LPkg3) && !DM.ARAY[riLSR].CheckAllStat(csNone)&& LSR.GetSeqStep()==CLaser::scIdle){ //마지막 아이들 보는 조건 테스트 해야 함...JH
                 aa= IO_GetX(xRAL_LPkg3);
                 dd= DM.ARAY[riLSR].CheckAllStat(csNone);
                 EM_SetErrMsg(eiPKG_Dispr , DM.ARAY[riLSR].GetName().c_str()) ;
            }
            bool bTemp = !IO_GetX(xRAL_VPkg4) && !DM.ARAY[riVSN].CheckAllStat(csNone) ;
            if (!IO_GetX(xRAL_VPkg4) && !DM.ARAY[riVSN].CheckAllStat(csNone)) EM_SetErrMsg(eiPKG_Dispr , DM.ARAY[riVSN].GetName().c_str()) ;
        }

        if(EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCycleFeed) {Trace(m_sPartName.c_str(),"CycleFeed Stt"); Step.iSeq = scFeed ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCycleBind) {Trace(m_sPartName.c_str(),"CycleBind Stt"); Step.iSeq = scBind ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default     :                  Trace(m_sPartName.c_str(),"default   End");Step.iSeq = scIdle ;  return false ;
        case scIdle :                                                                                   return false ;
        case scFeed : if(CycleFeed()){ Trace(m_sPartName.c_str(),"CycleFeed End");Step.iSeq = scIdle ;} return false ;
        case scBind : if(CycleBind()){ Trace(m_sPartName.c_str(),"CycleBind End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;

}

bool CRail::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;

    if (Step.iSeq) return false;

    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CRail::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CRail::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() , 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str());

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    bool bExstUnKnown = false ;

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: MoveActr(aiRAL_LStpUpDn , ccFwd);
                 MoveActr(aiRAL_VStpUpDn , ccFwd);
                 MoveActr(aiRAL_LAlnFwBw , ccBwd);
                 MoveActr(aiRAL_VAlnFwBw , ccBwd);
                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveActr(aiRAL_LStpUpDn , ccFwd)) return false ;
                 if(!MoveActr(aiRAL_VStpUpDn , ccFwd)) return false ;
                 if(!MoveActr(aiRAL_LAlnFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiRAL_VAlnFwBw , ccBwd)) return false ;

                 if((MT_GetCmdPos(miRAL_YCvsn) < GetMotrPos(miRAL_YCvsn , piRAL_YCvsnWork)-0.1) ||
                    (MT_GetCmdPos(miRAL_YCvsn) > GetMotrPos(miRAL_YCvsn , piRAL_YCvsnWork)+0.1)) { //레일 홈이 안잡혔음.
                     EM_SetErr(eiRAL_NotWorkPos); //레일이 홈안잡혔으니 마스크 걷고 홈잡아라.~

                 }
                 IO_SetY(yRAL_FeedAc , true);
                 m_tmDelay.Clear();
                 Step.iToStart++;
                 return false ;



        //모든 종료 확인. 위에서 숫자씀 조심.
        case 12: //if(!m_tmDelay.OnDelay(true , 500)) return false ; 바인드 싸이클로 뺌.
                 //if(!DM.ARAY[riVSN].CheckAllStat(csNone)) MoveActr(aiRAL_VAlnFwBw , ccFwd);
                 //if(!DM.ARAY[riLSR].CheckAllStat(csNone)) MoveActr(aiRAL_LAlnFwBw , ccFwd);

                 Step.iToStart++ ;
                 return false ;

        case 13: //if(!AT_Done(aiRAL_VAlnFwBw)) return false ;
                 //if(!AT_Done(aiRAL_LAlnFwBw)) return false ;

                 IO_SetY(yRAL_FeedAc , false);

                 Step.iToStart = 0 ;



                 return true ;
    }
}

bool CRail::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() , 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str());//EM_SetErr(eiRAL_ToStopTO);

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

        case 10: IO_SetY(yRAL_FeedAc,false);
                 m_tmDelay.Clear();
                 Step.iToStop ++;
                 return false ;

        case 11: if(!m_tmDelay.OnDelay(true , 500)) return false ; //kye

                 //MoveActr(aiRAL_LStpUpDn , ccBwd);
                 //MoveActr(aiRAL_VStpUpDn , ccBwd);
                 MoveActr(aiRAL_LAlnFwBw , ccBwd);
                 MoveActr(aiRAL_VAlnFwBw , ccBwd);

                 MoveActr(aiRAL_LStnUpDn , ccBwd);
                 MoveActr(aiRAL_VStnUpDn , ccBwd);

                 Step.iToStop++;
                 return false ;

        case 12: //if(!MoveActr(aiRAL_LStpUpDn , ccBwd)) return false ;
                 //if(!MoveActr(aiRAL_VStpUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiRAL_LAlnFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiRAL_VAlnFwBw , ccBwd)) return false ;

                 if(!MoveActr(aiRAL_LStnUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiRAL_VStnUpDn , ccBwd)) return false ;


                 Step.iToStop = 0 ;
                 return true ;
    }
}

//자 ~~~ 자제 빼자.
bool CRail::CycleFeed()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiRAL_CycleTO);
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



    static bool bVSNCanOut = false ; //다음으로 보낼수 있는 조건.
    static bool bLSRCanOut = false ;
    static bool bPRBCanOut = false ;

    static bool bVSNCanEnt = false ; //이전에서 들어와도 되는 조건.
    static bool bLSRCanEnt = false ;
    static bool bPRBCanEnt = false ;


    static int  iInspRowNo = 0 ; //마킹비젼 1줄검사일때 사용.


    //Visn Zone
    if(AT_Complete(aiRAL_VStpUpDn , ccBwd)){ //스토퍼 내렸을때. 자제 배출 감지하여 스토퍼 올림.
        if(!IO_GetX(xRAL_VPkg4)){
            MoveActr(aiRAL_VStpUpDn , ccFwd);
        }
    }

    //Laser Zone
    if(AT_Complete(aiRAL_LStpUpDn , ccBwd)){ //스토퍼 내렸을때.
        if(!IO_GetX(xRAL_LPkg3)){
            MoveActr(aiRAL_LStpUpDn , ccFwd);
        }
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //bVSNNeedOut = (!DM.ARAY[riVSN].GetCntStat  (csWork) && DM.ARAY[riPSB].CheckAllStat(csNone)) || DM.ARAY[riVSN].CheckAllStat(csNone); //
                  if(OM.CmnOptn.bSkipLaser && DM.ARAY[riLSR].CheckAllStat(csMask)) DM.ARAY[riLSR].SetStat(csWork);
                  bVSNCanEnt = (!DM.ARAY[riVSN].GetCntStat  (csWait) && DM.ARAY[riPSB].CheckAllStat(csNone)) || DM.ARAY[riVSN].CheckAllStat(csNone);
                  bLSRCanEnt = ( DM.ARAY[riLSR].CheckAllStat(csWork) && bVSNCanEnt                         ) || DM.ARAY[riLSR].CheckAllStat(csNone);
                  //bPRBCanEnt = ( DM.ARAY[riPRB].CheckAllStat(csMask) || PRB.GetWorkEnd()                   );//|| DM.ARAY[riPRB].CheckAllStat(csNone)


                  bVSNCanOut =  !DM.ARAY[riVSN].GetCntStat  (csWait) &&!DM.ARAY[riVSN].GetCntStat(csNone) && DM.ARAY[riPSB].CheckAllStat(csNone) ;
                  bLSRCanOut =   DM.ARAY[riLSR].CheckAllStat(csWork) && bVSNCanEnt;
                  bPRBCanOut =   DM.ARAY[riPRB].CheckAllStat(csMask) && bLSRCanEnt;//|| DM.ARAY[riPRB].CheckAllStat(csNone)PRB을 안기다릴경우); //스트립이 들어왔을때는 비젼검사 금방끝나니깐 기다리기 위해.

                  MoveActr(aiRAL_VAlnFwBw , ccBwd);
                  MoveActr(aiRAL_LAlnFwBw , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiRAL_VAlnFwBw , ccBwd))return false ;
                  if(!MoveActr(aiRAL_LAlnFwBw , ccBwd))return false ;

                  //레일 신나게 돌려라~~~
                  IO_SetY(yRAL_FeedAc , true);

                  if(bVSNCanOut) { //비젼존 부터 작업 할 경우.
                      Step.iCycle = 20 ;
                      return false ;
                  }
                  else if(bLSRCanOut) { //비젼에 작업 할께 없고 LSR 부터 할 경우.
                      Step.iCycle = 30 ;
                      return false ;
                  }
                  else if(bPRBCanOut) {
                      Step.iCycle = 40 ;
                      return false ;
                  }
                  else { //타면 안되는 이상한 오류 조건.
                      IO_SetY(yRAL_FeedAc , false);
                      Step.iCycle = 0 ;
                      return true ;
                  }



        //비젼존 부터 작업 할때.
        case  20: MoveActr(aiRAL_VStnUpDn , ccBwd);
                  MoveActr(aiRAL_VStpUpDn , ccBwd);



                  Step.iCycle++;
                  return false;

        case  21: //if(!MoveActr(aiRAL_VStnUpDn , ccBwd)) return false ;
                  //if(!MoveActr(aiRAL_VStpUpDn , ccBwd)) return false ;

                  DM.ShiftArrayData(riVSN , riPSB);

                  m_tmDelay.Clear();

                  Step.iCycle++;
                  return false ;

        case  22: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iRailStprDnDelay)) return false ;

                  if(bLSRCanOut) { //레이져 쪽 작업 할 차례
                      Step.iCycle = 30 ;
                      return false ;
                  }
                  else if(bPRBCanOut) {
                      Step.iCycle = 40 ;
                      return false ;
                  }
                  else { //정상 종료.

                      Step.iCycle=50;
                      return false ;
                  }




        //레이져존 작업.
        case  30: MoveActr(aiRAL_LStnUpDn , ccBwd);
                  MoveActr(aiRAL_LStpUpDn , ccBwd);

                  Step.iCycle++;
                  return false;

        case  31: if(!MoveActr(aiRAL_LStnUpDn , ccBwd)) return false ; //프리버퍼는 스테이션이 없어서
                  if(!MoveActr(aiRAL_LStpUpDn , ccBwd)) return false ; //확인을 하지 않으면 너무 붙어서 들어온다.

                  DM.ShiftArrayData(riLSR , riVSN);
                  if(OM.CmnOptn.iMarkInsp == moOneRow) { //한줄 검사일때만 올리고.
                      if(++iInspRowNo>=OM.DevInfo.iRowCnt){
                          iInspRowNo = 0 ;
                      }
                      for(int c = 0 ; c < DM.ARAY[riVSN].GetMaxCol() ; c++) DM.ARAY[riVSN].SetStat(iInspRowNo,c,csWait); //한줄만 남긴다.
                  }
                  else if(OM.CmnOptn.iMarkInsp == moAllRow)  { //아니면 0 데이터는 다남긴다.
                      DM.ARAY[riVSN].SetStat(csWait);
                      iInspRowNo = 0 ;

                  }
                  else { //moNone
                  }

                  m_tmDelay.Clear();

                  Step.iCycle++;
                  return false ;

        case  32: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iRailStprDnDelay)) return false ;

                  if(bPRBCanOut) {
                      Step.iCycle = 40 ;
                      return false ;
                  }
                  else { //정상 종료.
                      //IO_SetY(yRAL_FeedAc , false);
                      Step.iCycle=50;
                      return false ;
                  }




        //프리버퍼 작업.
        case  40: m_bNeedStrip = true ;
                  m_tmDelay.Clear();

                  Step.iCycle++;
                  return false;

        case  41: //if(m_tmDelay.OnDelay(true , 1000)) {
                  //    Step.iCycle=50;
                  //    return false ;
                  //}

                  if(!IO_GetX(xRAL_LPkg3)) return false ; //자제 들어왔음

                  Step.iCycle=50;
                  return false ;



        //모든 종료 확인. 위에서 숫자씀 조심.
        case  50: if(!AT_Complete(aiRAL_VStpUpDn , ccFwd))return false ;
                  if(!AT_Complete(aiRAL_LStpUpDn , ccFwd))return false ;
                  //if(!AT_Complete(aiPRB_StprUpDn , ccFwd))return false ;
                  if(!AT_Complete(aiPRB_StprUpDn , ccFwd))return false ; //요거는 프리버퍼 싸이클에서 조작 한다.

                  //자제 도착 확인.
                  if(bVSNCanOut&& IO_GetX(xPSB_PkgIn5))return false ; //
                  if(bLSRCanOut&&!IO_GetX(xRAL_VPkg4 ))return false ; //
                  if(bPRBCanOut&&!IO_GetX(xRAL_LPkg3 ))return false ; //



                  IO_SetY(yRAL_FeedAc , false);
                  m_bNeedStrip = false ;
                  Step.iCycle = 0 ;
                  return true ;


    }
}



//작업 위치로 이동.
bool CRail::CycleBind()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiRAL_CycleTO);
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

    static bool bLSRNeedToBind = false ;
    static bool bVSNNeedToBind = false ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiRAL_LStpUpDn , ccFwd);
                  MoveActr(aiRAL_VStpUpDn , ccFwd);
                  MoveActr(aiRAL_LAlnFwBw , ccBwd);
                  MoveActr(aiRAL_VAlnFwBw , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiRAL_LStpUpDn , ccFwd)) return false ;
                  if(!MoveActr(aiRAL_VStpUpDn , ccFwd)) return false ;
                  if(!MoveActr(aiRAL_LAlnFwBw , ccBwd)) return false ;
                  if(!MoveActr(aiRAL_VAlnFwBw , ccBwd)) return false ;

                  IO_SetY(yRAL_FeedAc , true);
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false ;



        //모든   종료 확인. 위에서 숫자씀 조심.
        case  12: if(!m_tmDelay.OnDelay(true , 300)) return false ;
                  bLSRNeedToBind = (!DM.ARAY[riLSR].CheckAllStat(csNone) || IO_GetX(xRAL_LPkg3)) ;
                  bVSNNeedToBind = (!DM.ARAY[riVSN].CheckAllStat(csNone) || IO_GetX(xRAL_VPkg4)) ;


                  if(bLSRNeedToBind) MoveActr(aiRAL_LAlnFwBw , ccFwd);
                  if(bVSNNeedToBind) MoveActr(aiRAL_VAlnFwBw , ccFwd);


                  Step.iCycle++ ;
                  return false ;

        case  13: if(!AT_Done(aiRAL_LAlnFwBw)) return false ;
                  if(!AT_Done(aiRAL_VAlnFwBw)) return false ;




                  if(bLSRNeedToBind)MoveActr(aiRAL_LStnUpDn , ccFwd);
                  if(bVSNNeedToBind)MoveActr(aiRAL_VStnUpDn , ccFwd);

                  Step.iCycle++;
                  return false ;

        case  14: if(bLSRNeedToBind && !MoveActr(aiRAL_LStnUpDn , ccFwd)) return false ;
                  if(bVSNNeedToBind && !MoveActr(aiRAL_VStnUpDn , ccFwd)) return false ;

                  if(bLSRNeedToBind)MoveActr(aiRAL_LAlnFwBw , ccBwd);
                  if(bVSNNeedToBind)MoveActr(aiRAL_VAlnFwBw , ccBwd);

                  IO_SetY(yRAL_FeedAc , false);

                  Step.iCycle++;
                  return false ;

        case  15: if(!bLSRNeedToBind && !MoveActr(aiRAL_LAlnFwBw , ccBwd)) return false ;
                  if(!bVSNNeedToBind && !MoveActr(aiRAL_VAlnFwBw , ccBwd)) return false ;
                  Step.iCycle=0;
                  return true;
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
    if(!MT_GetStop(miRAL_XLStp)) return false ;
    if(!MT_GetStop(miRAL_XVStp)) return false ;

    if(!AT_Done(aiRAL_LAlnFwBw)) return false ;
    if(!AT_Done(aiRAL_LStnUpDn)) return false ;
    if(!AT_Done(aiRAL_LStpUpDn)) return false ;
    if(!AT_Done(aiRAL_VAlnFwBw)) return false ;
    if(!AT_Done(aiRAL_VStnUpDn)) return false ;
    if(!AT_Done(aiRAL_VStpUpDn)) return false ;

    return true ;
}

void CRail::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\Rail.INI";

    //if ( _bLoad ) UserINI.Load(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );

    //else {
    //    UserINI.ClearFile(sPath) ;
    //    UserINI.Save(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
    //}

}
//---------------------------------------------------------------------------
