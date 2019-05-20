//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "WorkStage.h"
#include "Index.h"
//#include "RunThread.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
//#include "LotUnit.h"
#include "UserIni.h"
#include "UserFile.h"




//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CWorkStage WKS;

CWorkStage::CWorkStage(void)
{

}

CWorkStage::~CWorkStage (void)
{

}

void CWorkStage::Init(bool _bFront )
{

    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();

}

void CWorkStage::Close()
{
    Load(false);
}

void CWorkStage::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}


bool CWorkStage::FindChip(int &c , EN_ARAY_ID &id)
{
    id = riIDX_R ;
         if(DM.ARAY[riIDX_R].GetCntStat(csVisn   )) {c = DM.ARAY[riIDX_R].FindLastCol(csVisn   ); return true  ;}
    else if(DM.ARAY[riIDX_R].GetCntStat(csAttach )) {c = DM.ARAY[riIDX_R].FindLastCol(csAttach ); return true  ;}
    else if(DM.ARAY[riIDX_R].GetCntStat(csDisp   )) {c = DM.ARAY[riIDX_R].FindLastCol(csDisp   ); return true  ;}
    else if(DM.ARAY[riIDX_R].GetCntStat(csPush   )) {c = DM.ARAY[riIDX_R].FindLastCol(csPush   ); return true  ;}
    else if(DM.ARAY[riIDX_R].GetCntStat(csEndVisn)) {c = DM.ARAY[riIDX_R].FindLastCol(csEndVisn); return true  ;}
    else if(DM.ARAY[riIDX_R].GetCntStat(csWorkEnd)) {c = DM.ARAY[riIDX_R].FindLastCol(csWorkEnd); return true  ;}
    else                                            {c = -1 ;                                     return false ;}

}

double CWorkStage::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    double dWorkStt ; //작업해야할 메거진의 맨 아래 슬롯 위치.

    if(_iMotr == miSTG_XStage){
             if(_iPstnId ==  piSTG_XStagWait   ) dPos = PM.GetValue (_iMotr , pvSTG_XStagWait     );
        else if(_iPstnId ==  piSTG_XStagCenter ) dPos = PM.GetValue (_iMotr , pvSTG_XStagCenter   );
        else                                     dPos = MT_GetCmdPos(_iMotr                       );
    }
    else if(_iMotr == miSTG_YStage){
             if(_iPstnId ==  piSTG_YStagWait   ) dPos = PM.GetValue (_iMotr , pvSTG_YStagWait     );
        else if(_iPstnId ==  piSTG_YStagCenter ) dPos = PM.GetValue (_iMotr , pvSTG_YStagCenter   );
        else                                     dPos = MT_GetCmdPos(_iMotr                       );
    }
    else if(_iMotr == miSTG_TStage){
             if(_iPstnId ==  piSTG_TStagWait   ) dPos = PM.GetValue (_iMotr , pvSTG_TStagWait     );
        else if(_iPstnId ==  piSTG_TStagWork   ) dPos = PM.GetValue (_iMotr , pvSTG_TStagWork     );
        else                                     dPos = MT_GetCmdPos(_iMotr                       );
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CWorkStage::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

bool CWorkStage::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CWorkStage::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CWorkStage::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStartTO);

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

        case 10: Step.iToStart ++ ;
                 return false ;

        case 11: 
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CWorkStage::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStopTO);

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

        case 10:


                 Step.iToStop++;
                 return false ;

        case 11:

                 Step.iToStop ++ ;
                 return false ;

        case 12: 
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CWorkStage::Autorun(void) //오토런닝시에 계속 타는 함수.
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

        bool isIdxWorkPos = IDX_R.IsWorkPos() ;
        int  iWorkingCol ;
        EN_ARAY_ID riWork ;
        FindChip(iWorkingCol ,riWork);
        bool isWorking = iWorkingCol >= 0 ;

        //FOS 순서    
        //csVisn    얼라인검사
        //csAttach  픽커 칩 붙인다.
        //csDisp    디스펜싱 하고.
        //csPush    일정 시간 꾹 누르고 픽커 빠진다.
        //csEndVisn 작업 완료 비젼 확인.
        //csWorkEnd 작업 종료 하고 스테이지 내린다.

        //DIE순서
        //csDisp    파란판에 패턴 디스펜싱 한다.
        //csVisn    얼라인 검사 해본다.
        //csAttach  칩을 붙인다.
        //csEndVisn 작업 완료 비젼 확인.
        //csWorkEnd 작업 종료 하고 스테이지 내린다.

        bool isCycleUp   =  DM.ARAY[riIDX_R].GetCntStat(csUnkwn) &&
                            isIdxWorkPos                         &&
                           !isWorking                            &&
                            IDX_R.GetSeqStep() == CIndex::scIdle ; //MT_GetCmdPos(miSTG_ZStagR) == PM.GetValue(miSTG_ZStagR , pvSTG_ZStagWait);
        bool isCycleDown =  isWorking && DM.ARAY[riIDX_R].GetStat(0,iWorkingCol) == csWorkEnd ;

        bool isCycleEnd  =  IDX_R.GetStat().bWorkEnd && DM.ARAY[riIDX_R].CheckAllStat(csNone) ;

        if(EM_IsErr()) return false ;


         //Normal Decide Step.
             if (isCycleUp    ) {Trace(m_sPartName.c_str(),"CycleUp   Stt"); Step.iSeq = scUp   ; InitCycleStep(); m_iCycleSttTime=GetTickCount();} //
        else if (isCycleDown  ) {Trace(m_sPartName.c_str(),"CycleDown Stt"); Step.iSeq = scDown ; InitCycleStep(); m_iCycleSttTime=GetTickCount();} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default      :                   Trace(m_sPartName.c_str(),"default     End");                                                        Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                              return false ;
        case scUp    : if(CycleUp   ()){ Trace(m_sPartName.c_str(),"CycleUp     End");m_iCycleTime[Step.iSeq]=GetTickCount()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scDown  : if(CycleDown ()){ Trace(m_sPartName.c_str(),"CycleDown   End");m_iCycleTime[Step.iSeq]=GetTickCount()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}

bool CWorkStage::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_HomeTo,m_sPartName.c_str());
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

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: IO_SetY(ySTG_Vccm,true);
                 m_tmCycle.Clear();
                 Step.iHome++;
                 return false ;

        case 11: if(!m_tmCycle.OnDelay(true , 500)) return false ;
                 //작업 하다가 홈잡을때...
                 //자제배큠빨아보니 있고 장비껐다가 켠상태가 아닐때.
                 //이거 디스펜싱 하고 그럴때 정지 하고 홈잡으면 대책 없음...
                 //오퍼레이션에서 일단 막자..
                 if(IO_GetX(xSTG_Vccm)) {
                     EM_SetErrMsg(eiSTG_RemovePkg , "작업 Stage에 자제를 제거하여 주십시오");
                     Step.iHome = 0 ;
                     return true ;
                 }

                 if(IO_GetX(xSTG_Vccm) && MT_GetHomeEnd(miSTG_XStage)&&
                                          MT_GetHomeEnd(miSTG_YStage)&&
                                          MT_GetHomeEnd(miSTG_TStage)){
                     MT_GoAbsMan(miSTG_XStage , PM.GetValue(miSTG_XStage , pvSTG_XStagCenter));
                     MT_GoAbsMan(miSTG_YStage , PM.GetValue(miSTG_YStage , pvSTG_YStagCenter));
                     MT_GoAbsMan(miSTG_TStage , PM.GetValue(miSTG_TStage , pvSTG_TStagWork  ));

                     Step.iHome++ ;
                     return false ;
                 }

                 Step.iHome=13 ;
                 return false ;

        case 12: if(!MT_GoAbsMan(miSTG_XStage , PM.GetValue(miSTG_XStage , pvSTG_XStagCenter))) return false ;
                 if(!MT_GoAbsMan(miSTG_YStage , PM.GetValue(miSTG_YStage , pvSTG_YStagCenter))) return false ;
                 if(!MT_GoAbsMan(miSTG_TStage , PM.GetValue(miSTG_TStage , pvSTG_TStagWork  ))) return false ;


                 Step.iHome++;
                 return false ;

        case 13: IO_SetY(ySTG_Vccm,false);
                 MT_DoHome(miSTG_ZStage);
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GetHomeEnd(miSTG_ZStage)) return false ;
                 MT_DoHome(miSTG_XStage);
                 MT_DoHome(miSTG_YStage);
                 MT_DoHome(miSTG_TStage);
                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GetHomeEnd(miSTG_XStage)) return false ;
                 if(!MT_GetHomeEnd(miSTG_YStage)) return false ;
                 if(!MT_GetHomeEnd(miSTG_TStage)) return false ;

                 MT_GoAbsMan(miSTG_XStage , PM.GetValue(miSTG_XStage , pvSTG_XStagWait));
                 MT_GoAbsMan(miSTG_YStage , PM.GetValue(miSTG_YStage , pvSTG_YStagWait));
                 MT_GoAbsMan(miSTG_TStage , PM.GetValue(miSTG_TStage , pvSTG_TStagWait));
                 MT_GoAbsMan(miSTG_ZStage , PM.GetValue(miSTG_ZStage , pvSTG_ZStagWait));
                 Step.iHome = 0;
                 return true ;

    }
}

//One Cycle.
bool CWorkStage::CycleUp()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS :%s Step.iCycle=%02d",m_sPartName.c_str() , __FUNC__ , Step.iCycle );
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
        //Step.iCycle = 0;
        //return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: MoveMotr(miSTG_XStage , piSTG_XStagCenter);
                 MoveMotr(miSTG_YStage , piSTG_YStagCenter);
                 MoveMotr(miSTG_TStage , piSTG_TStagWork  );
                 IO_SetY(ySTG_Ejct , false);
                 IO_SetY(ySTG_Vccm , false);
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveMotr(miSTG_XStage , piSTG_XStagCenter)) return false ;
                 if(!MoveMotr(miSTG_YStage , piSTG_YStagCenter)) return false ;
                 if(!MoveMotr(miSTG_TStage , piSTG_TStagWork  )) return false ;

                 MoveMotr(miSTG_ZStage , piSTG_ZStagPick);
                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(miSTG_ZStage , piSTG_ZStagPick)) return false ;
                 IO_SetY(ySTG_Vccm , true);
                 m_tmCycle.Clear();
                 Step.iCycle++;
                 return false ;

        case 13: if(!m_tmCycle.OnDelay(true , 200)) return false ;
                 MoveMotr(miSTG_ZStage , piSTG_ZStagWork);
                 Step.iCycle++;
                 return false ;

        case 14: if(!MoveMotr(miSTG_ZStage , piSTG_ZStagWork)) return false ;
                 int  iWorkingCol ;
                 EN_ARAY_ID riWork ;
                 FindChip(iWorkingCol ,riWork);
                 if(OM.DevInfo.iWorkMode == wmFos)DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csVisn);
                 else                             DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csDisp);

                 Step.iCycle = 0;
                 return true ;
    }




}



bool CWorkStage::CycleDown(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS :%s Step.iCycle=%02d",m_sPartName.c_str() , __FUNC__ , Step.iCycle );
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
        //Step.iCycle = 0;
        //return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: MoveMotr(miSTG_XStage , piSTG_XStagCenter);
                 MoveMotr(miSTG_YStage , piSTG_YStagCenter);
                 MoveMotr(miSTG_TStage , piSTG_TStagWork  );
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveMotr(miSTG_XStage , piSTG_XStagCenter)) return false ;
                 if(!MoveMotr(miSTG_YStage , piSTG_YStagCenter)) return false ;
                 if(!MoveMotr(miSTG_TStage , piSTG_TStagWork  )) return false ;

                 MoveMotr(miSTG_ZStage , piSTG_ZStagPick);
                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(miSTG_ZStage , piSTG_ZStagPick)) return false ;
                 IO_SetY(ySTG_Vccm , false);
                 IO_SetY(ySTG_Ejct , true );
                 m_tmCycle.Clear();
                 Step.iCycle++;
                 return false ;

        case 13: if(!m_tmCycle.OnDelay(true , 200)) return false ;
                 IO_SetY(ySTG_Ejct , false);
                 MoveMotr(miSTG_ZStage , piSTG_ZStagWait);
                 Step.iCycle++;
                 return false ;

        case 14: if(!MoveMotr(miSTG_ZStage , piSTG_ZStagWait)) return false ;
                 int  iWorkingCol ;
                 EN_ARAY_ID riWork ;
                 FindChip(iWorkingCol ,riWork);
                 DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csWork);

                 Step.iCycle = 0;
                 return true ;
    }

}


//---------------------------------------------------------------------------
bool CWorkStage::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(!MT_GetStop(miIDX_XIndxR)){
        sMsg = "Motor " + AnsiString(MT_GetName(miIDX_XIndxR)) + " is Moving." ;
        bRet = false ;
    }

    if(_iMotr == miSTG_XStage){

    }
    else if(_iMotr == miSTG_YStage){

    }
    else if(_iMotr == miSTG_TStage){

    }
    else if(_iMotr == miSTG_ZStage){

    }

    else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr),sMsg.c_str());
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CWorkStage::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    //if(_iActr == aiLDR_IndxUpUp){
    //    if(_bFwd == ccFwd) {
    //        //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
    //    }
    //}
    //else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    //}


    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr),sMsg.c_str());
    }

    return bRet ;
}

bool CWorkStage::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow )
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(_bSlow) {
        MT_GoAbsSlow(_iMotr , dPosition);
    }
    else {
        if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
        else            return MT_GoAbsMan(_iMotr , dPosition);
    }

    return false ;
}







void CWorkStage::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CWorkStage::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CWorkStage::SetLastCmd()
{
    m_dLastIdxPos = MT_GetCmdPos(miSTG_ZStage);
    return ;

}

double CWorkStage::GetLastCmd(EN_MOTR_ID _iMotr)
{
    double dLastIdxPos = 0.0 ;
    if(!MT_GetAlarm(_iMotr) && !MT_GetNLimSnsr(_iMotr) && !MT_GetPLimSnsr(_iMotr) ) dLastIdxPos = MT_GetCmdPos(_iMotr) ;
    else                                                                            dLastIdxPos = GetMotrPos  (_iMotr , (EN_PSTN_ID)0) ;

    return dLastIdxPos ;
}

bool CWorkStage::CheckMoved()
{
    if(m_dLastIdxPos != MT_GetCmdPos(miSTG_ZStage)) return true  ; //이파트는 필요 없다.
    else                                            return false ;

}

bool CWorkStage::CheckStop()
{
    if(!MT_GetStop(miSTG_XStage)) return false ;
    if(!MT_GetStop(miSTG_YStage)) return false ;
    if(!MT_GetStop(miSTG_TStage)) return false ;
    if(!MT_GetStop(miSTG_ZStage)) return false ;


    //if(!AT_Done(aiIDX_IndxUpDn)) return false ;

    return true ;
}

void CWorkStage::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";
    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
}




                                                     
