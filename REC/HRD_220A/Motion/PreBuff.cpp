//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff.h"
#include "Rail.h"
//---------------------------------------------------------------------------
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UnLoader.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CPreBuff PRB;

CPreBuff::CPreBuff(void)
{

}

void CPreBuff::Init()
{
    m_sPartName = "PreBuff " ;
    Reset();
    Load(true);

    InitCycleName();
}

void CPreBuff::Close()
{
    Load(false);
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
    m_tmDelay  .Clear();
}

CPreBuff::~CPreBuff (void)
{

}

bool CPreBuff::FindChip(int &r , int &c )
{
//    r = DM.ARAY[riLD1].FindFrstRow(csUnkwn) ;
    return false;
}

double CPreBuff::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = MT_GetCmdPos(_iMotr);

    return dPos ;
}

//---------------------------------------------------------------------------
bool CPreBuff::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    //if(_iMotr == miPRB_XCmp){
    //    switch(_iPstnId) {
    //        default             :                                                                                              break ;
    //        case piPRB_XCmpWait : if(isClampClose) { sMsg = AnsiString(AT_GetName(aiPRB_CmpDnUp))+" is Down" ; bRet = false ;} break ;
    //        case piPRB_XCmpGrip :                                                                                              break ;
    //        case piPRB_XCmpPull :                                                                                              break ;
    //        case piPRB_XCmpBack :                                                                                              break ;
    //        case piPRB_XCmpOut  :                                                                                              break ;
    //    }
    //}
    //
    //else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    //}



    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return true ;
}

//---------------------------------------------------------------------------
bool  CPreBuff::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd )) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iActr == aiPRB_StprUpDn) {
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

bool CPreBuff::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CPreBuff::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPreBuff::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_HomeTo , m_sPartName.c_str());//EM_SetErr(eiPRB_HomeTo);
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

        case 10: IO_SetY(yPRB_FeedAc  ,false) ;
                 IO_SetY(yPRB_AirBlow ,false) ;
                 AT_MoveCyl(aiPRB_StprUpDn  ,ccFwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPRB_StprUpDn  ,ccFwd)) return false ;
                 Step.iHome = 0;
                 return true ;

    }
}

bool CPreBuff::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

//프리버퍼 오리검사 레이져조사 비젼검사
//Unkwn -> Mask     Work       Good
//                             Fail

    if (Step.iSeq == 0) {
        if(Stat.bReqStop)return false ;

        bool isCycleIn      =  DM.ARAY[riPRB].CheckAllStat(csNone ) && AT_GetCmd(aiLDR_PshrFwBw)==ccFwd; //센서보다 이게 나은듯.
        bool isCycleOriVisn =  DM.ARAY[riPRB].CheckAllStat(csUnkwn) ;
        bool isCycleOut     =  DM.ARAY[riPRB].CheckAllStat(csMask ) && RAL._bNeedStrip;

        bool isCycleEnd     =  DM.ARAY[riPRB].CheckAllStat(csNone) && LDR.GetWorkEnd();

        //Unknown Strip
        if ( IO_GetX(xPRB_Pkg2) && DM.ARAY[riPRB].CheckAllStat(csNone))
            EM_SetErrMsg(eiPKG_Unknwn , DM.ARAY[riPRB].GetName().c_str()) ;

        //Dissapear Strip
        if(!OM.MstOptn.bDryRun) {
            if (!IO_GetX(xPRB_Pkg2) &&!DM.ARAY[riPRB].CheckAllStat(csNone)) {
                EM_SetErrMsg(eiPKG_Dispr , DM.ARAY[riPRB].GetName().c_str()) ;
            }
        }

        if(EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCycleOut    ) {Trace(m_sPartName.c_str(),"CycleOut     Stt"); Step.iSeq = scOut     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleOriVisn) {Trace(m_sPartName.c_str(),"CycleOriVisn Stt"); Step.iSeq = scOriVisn ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleIn     ) {Trace(m_sPartName.c_str(),"CycleIn      Stt"); Step.iSeq = scIn      ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd    ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default         :                     Trace(m_sPartName.c_str(),"default      End");Step.iSeq = scIdle ;  return false ;
        case scIdle     :                                                                                         return false ;
        case scOut      : if(CycleOut    ()){ Trace(m_sPartName.c_str(),"CycleOut     End");Step.iSeq = scIdle ;} return false ;
        case scOriVisn  : if(CycleOriVisn()){ Trace(m_sPartName.c_str(),"CycleOriVisn End");Step.iSeq = scIdle ;} return false ;
        case scIn       : if(CycleIn     ()){ Trace(m_sPartName.c_str(),"CycleIn      End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;

}

bool CPreBuff::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{

    //During the auto run, do not stop.
    if (LDR.GetSeqStep() == CLoader::scWork) return false ; //로더 워킹 하면서 푸쉬 하고 있을때 스탑을 되어 스트립 걸려 있는 것 방지.
    if (RAL.GetSeqStep() == CRail  ::scFeed) return false ; //로더 워킹 하면서 푸쉬 하고 있을때 스탑을 되어 스트립 걸려 있는 것 방지.
    if (Step.iSeq) return false;

    Stat.bReqStop = true ;
    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CPreBuff::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CPreBuff::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() , 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str()); //EM_SetErr(eiPRB_ToStartTO);

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

        case 10: IO_SetY(yPRB_AirBlow,true );
                 IO_SetY(yPRB_FeedAc ,false);

                 //Vision Initial
                 IO_SetY(yVSC_OriStart  , false);
                 IO_SetY(yVSC_Spear1    , false);



                 MoveActr(aiPRB_StprUpDn , ccFwd);
                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveActr(aiPRB_StprUpDn , ccFwd)) return false ;

                 if(!OM.CmnOptn.bSkipOriVisn){
                     if(!IO_GetX(xVSC_OriReady)) EM_SetErrMsg(eiVSN_NotReady , "Orientation Vision");
                 }

                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CPreBuff::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() , 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str());//EM_SetErr(eiPRB_ToStopTO);

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

        case 10: IO_SetY(yPRB_AirBlow,false);
                 IO_SetY(yPRB_FeedAc ,false);

                 //Vision Initial
                 IO_SetY(yVSC_OriStart  , false);
                 IO_SetY(yVSC_JobChange , false);

                 Step.iToStop ++ ;
                 return false ;

        case 11: Step.iToStop = 0   ;
                 return true ;
    }
}

//롤러로 스트립 밀어 넣고 비젼존까지.
bool CPreBuff::CycleIn(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiPRB_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        IO_SetY(yPRB_FeedAc,false);
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

        case  10: IO_SetY(yPRB_FeedAc,true);
                  Step.iCycle++;
                  return false;

        case  11: if(IO_GetX(xPRB_PkgIn1)) { //자제 들어왔다.
                      Step.iCycle++;
                      return false ;
                  }
                  if(AT_GetCmd(aiLDR_PshrFwBw)==ccBwd) { //푸셔 뒤로 갈때까지 자제 없으면 자제없는 슬롯.
                      IO_SetY(yPRB_FeedAc,false);
                      Step.iCycle=0;
                      return true ;
                  }
                  return false ;

        case  12: if(!IO_GetX(xPRB_PkgIn1)) return false ;
                  DM.ARAY[riPRB].SetStat(csUnkwn) ;
                  Step.iCycle++;
                  return false;

        case  13: if(!IO_GetX(xPRB_Pkg2)) return false ;
                  IO_SetY(yPRB_FeedAc,false);
                  Step.iCycle = 0;
                  return true ;
    }
}

//비젼오리엔테이션 검사.
bool CPreBuff::CycleOriVisn(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiPRB_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        IO_SetY(yPRB_FeedAc,false);
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

        case  10: //if(IO_GetX(yVSC_AlnReady,true);
                  if(OM.CmnOptn.bSkipOriVisn) {
                      DM.ARAY[riPRB].SetStat(csMask);
                      Step.iCycle=0;
                      return true ;
                  }
                  
                  if(IO_GetX(xVSC_OriBusy)) {
                      EM_SetErrMsg(eiVSN_NotReady , "Orientation Vision");
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  IO_SetY(yVSC_OriStart , true);

                  Step.iCycle++;
                  return false;

        case  11: if(!IO_GetX(xVSC_OriBusy)) return false ;
                  IO_SetY(yVSC_OriStart , false);
                  Step.iCycle++;
                  return false ;

        case  12: if(IO_GetX(xVSC_OriBusy)) return false ; //Inspection End

                  if(IO_GetX(xVSC_OriRslt)){
                      DM.ARAY[riPRB].SetStat(csMask);
                  }
                  else {
                      EM_SetErrMsg(eiVSN_InspNG , "Orientation Vision");
                  }

                  Step.iCycle=0;
                  return true ;
    }
}

bool CPreBuff::CycleOut()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiPRB_CycleTO);
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

        case  10: MoveActr(aiPRB_StprUpDn , ccBwd);
                  IO_SetY(yPRB_FeedAc , true);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPRB_StprUpDn , ccBwd)) return false ;
                  Step.iCycle++;
                  return false ;

        case  12: if(IO_GetX(xPRB_Pkg2)) return false ;
                  MoveActr(aiPRB_StprUpDn , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiPRB_StprUpDn , ccFwd)) return false;
                  IO_SetY(yPRB_FeedAc , false);
                  DM.ShiftArrayData(riPRB , riLSR);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

void CPreBuff::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CPreBuff::CheckMoved()
{
    
    return true ; //이파트는 필요 없다.
}

bool CPreBuff::CheckStop()
{

    if(!AT_Done(aiPRB_StprUpDn)) return false ;

    return true ;
}

void CPreBuff::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\PreBuff.INI";

    //if ( _bLoad ) UserINI.Load(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );

    //else {
    //    UserINI.ClearFile(sPath) ;
    //    UserINI.Save(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
    //}

}
//---------------------------------------------------------------------------
