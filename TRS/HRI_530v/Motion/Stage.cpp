//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Stage.h"
#include "StageVision.h"
#include "Sort.h"

#include "StageVT.h"
#include "StageOST.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "LotUnit.h"
#include "UserIni.h"

#include "SortCmn.h"

//#include "Formoperation.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CStage STG;

CStage::CStage(void)
{

}

CStage::~CStage (void)
{

}

void CStage::Init()
{
    m_sPartName = "Stage " ;
    Reset();


    InitCycleName();
    InitCycleTime();

    m_dLastIdxPos = 0 ;

    Load(true);


    //m_iStgMoveCnt = 0;
}

void CStage::Close()
{
    Load(false);
}

void CStage::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}


bool CStage::FindChip(int &r , int &c )
{
    //r = DM.ARAY[riLDR].FindLastRow(csUnkwn) ;
    if(DM.ARAY[riLDR].FindFrstColRow(csUnkwn , r,c)){
        return true ;
    }
    else {
        r=0 ;
        c=0 ;
        return false ;
    }

}

double CStage::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    //int    iR,iC ;
    //FindChip(iR ,iC);

    double dWorkPos ;
    dWorkPos = PM.GetValue (_iMotr , pvSTG_TTbleWork ) + OM.CmnOptn.dStgWorkOffset ; //OST 수율이 안나와서 화면에서 옵셑 선택하게 야매수정.

    double dWaitPos ;
    dWaitPos = PM.GetValue (_iMotr , pvSTG_TTbleWait ) + OM.CmnOptn.dStgWorkOffset ; //OST 수율이 안나와서 화면에서 옵셑 선택하게 야매수정.


    if(_iMotr == miSTG_TTble){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piSTG_TTbleWait     : dPos = dWaitPos                                    ; break ;
            case piSTG_TTbleWork     : dPos = dWorkPos                                    ; break ;

        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CStage::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

bool CStage::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CStage::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CStage::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    EN_ARAY_ID riTarget ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: OST.MoveActr(aiOST_PshrDnUp , ccBwd);
                 OST.MoveActr(aiOST_PTolUpDn , ccBwd);
                 VTI.MoveActr(aiVTI_PshrDnUp , ccBwd);
                 VTI.MoveActr(aiVTI_PTolUpDn , ccBwd);

                 Step.iToStart++ ;
                 return false ;

        case 11: if(!OST.MoveActr(aiOST_PshrDnUp , ccBwd))return false;
                 if(!OST.MoveActr(aiOST_PTolUpDn , ccBwd))return false;
                 if(!VTI.MoveActr(aiVTI_PshrDnUp , ccBwd))return false;
                 if(!VTI.MoveActr(aiVTI_PTolUpDn , ccBwd))return false;

                 if(DM.ARAY[riSTF].CheckAllStat(csEmpty)&& //다비었을 경우는 위치 초기화 하고
                    DM.ARAY[riSTL].CheckAllStat(csEmpty)&&
                    DM.ARAY[riSTB].CheckAllStat(csEmpty)&&
                    DM.ARAY[riSTR].CheckAllStat(csEmpty)){
                     //MT_GoAbsMan(miSTG_TTble , PM.GetValue(miSTG_TTble , pvSTG_TTbleWork));
                     MoveMotr(miSTG_TTble , piSTG_TTbleWork); //오프셑 추가되어서


                 }
                 else { //작업 중이였을경우. 라스트 인덱스 포지션으로 이동 한다.
                     MT_GoAbsMan(miSTG_TTble , GetMotrPos(miSTG_TTble , piSTG_TTbleWork) + m_dLastIdxPos);
                 }






                 Step.iToStart++;
                 return false ;

        case 12: if(!MT_GetStop(miSTG_TTble)) return false ;
                 //ChangeSTGArayByAngle(ScmGetStgAngle());
                 //Scm
                 // 20160302 OST 로그 카운트가 안맞아 분석해보니 에러 뜰때 OST작업중이면 로그를 남기고.
                 // 에러클리어 하고 스타트 하면 여기서 다시 리셑을 해서 그런데....
                 riTarget = riSTL ;
                 for(int c = 0 ; c< DM.ARAY[riTarget].GetMaxCol() ; c++){
                     for(int r = 0 ; r< DM.ARAY[riTarget].GetMaxRow() ; r++){
                         if(DM.ARAY[riTarget].GetStat(r,c)!=csEmpty){
                             DM.ARAY[riTarget].SetStat(r,c,csUnkwn);
                         }
                     }
                 }
                 riTarget = riSTB ;
                 for(int c = 0 ; c< DM.ARAY[riTarget].GetMaxCol() ; c++){
                     for(int r = 0 ; r< DM.ARAY[riTarget].GetMaxRow() ; r++){
                         if(DM.ARAY[riTarget].GetStat(r,c)!=csEmpty){
                             DM.ARAY[riTarget].SetStat(r,c,csUnkwn);
                         }
                     }
                 }
                 riTarget = riSTR ;
                 for(int c = 0 ; c< DM.ARAY[riTarget].GetMaxCol() ; c++){
                     for(int r = 0 ; r< DM.ARAY[riTarget].GetMaxRow() ; r++){
                         if(DM.ARAY[riTarget].GetStat(r,c)!=csEmpty){
                             DM.ARAY[riTarget].SetStat(r,c,csUnkwn);
                         }
                     }
                 }

                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CStage::ToStop(void) //스탑을 하기 위한 함수.
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


        case 10: OST.MoveActr(aiOST_PshrDnUp , ccBwd); //안전빵으로 그냥 여기서도 백워드 시킨다.
                 OST.MoveActr(aiOST_PTolUpDn , ccBwd);
                 VTI.MoveActr(aiVTI_PshrDnUp , ccBwd);
                 VTI.MoveActr(aiVTI_PTolUpDn , ccBwd);
                 Step.iToStop++ ;
                 return false ;

        case 11: if(!OST.MoveActr(aiOST_PshrDnUp , ccBwd))return false;
                 if(!OST.MoveActr(aiOST_PTolUpDn , ccBwd))return false;
                 if(!VTI.MoveActr(aiVTI_PshrDnUp , ccBwd))return false;
                 if(!VTI.MoveActr(aiVTI_PTolUpDn , ccBwd))return false;

                 SetLastCmd();
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CStage::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iSeq=%02d" , __FUNC__ , Step.iSeq );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    static int iCycleSttTime = 0;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool bOSTInspEnd =  DM.ARAY[riSTL].GetCntStat(csUnkwn) == 0 ;
        bool bVTIInspEnd =  DM.ARAY[riSTB].GetCntStat(csUnkwn) == 0 ;
        bool bVSNInspEnd =  DM.ARAY[riSTR].GetCntStat(csUnkwn) == 0 ;


        bool bExistWork  =  ScmGetCntStat  (riSRT , csWork ) ||
                           !ScmCheckAllStat(riZIG , csEmpty) ||
                            DM.ARAY[riLDR].GetCntStat  (csUnkwn) ||
                            DM.ARAY[riPRL].GetCntStat  (csUnkwn) ||
                           (IO_GetX(xLSP_Tray5Sply) && !LOT.GetRqstLotEnd()) ;

        bool bStgAllEmpty = DM.ARAY[riSTF].CheckAllStat(csEmpty) &&
                            DM.ARAY[riSTL].CheckAllStat(csEmpty) &&
                            DM.ARAY[riSTB].CheckAllStat(csEmpty) &&
                            DM.ARAY[riSTR].CheckAllStat(csEmpty) ;

        bool bFrntReady  =  DM.ARAY[riSTF].GetCntStat(csWork) || //정상 플로우.
                           (LOT.GetRqstLotEnd() && !bExistWork && DM.ARAY[riSTF].CheckAllStat(csEmpty) && !bStgAllEmpty);            //랏엔드시 플로우.

        bool bOSTIdle    =  OST.GetSeqStep() == CStageOST    ::scIdle ;
        bool bVTIIdle    =  VTI.GetSeqStep() == CStageVT     ::scIdle ;
        bool bVSNIdle    =  VSN.GetSeqStep() == CStageVision ::scIdle ;
        bool bSRTNotWorkOnSTG = SRT.GetSeqStep() != CSort::scPickSTG &&
                                SRT.GetSeqStep() != CSort::scPlceSTG ;


        bool bAllActrBwd  = AT_Complete(aiVTI_PshrDnUp , ccBwd) && AT_Complete(aiVTI_PTolUpDn , ccBwd) &&
                            AT_Complete(aiOST_PshrDnUp , ccBwd) && AT_Complete(aiOST_PTolUpDn , ccBwd) ;

        bool bPckrZDown    = MT_GetTrgPos(miSRT_ZPckr) > PM.GetValue(miSRT_ZPckr , pvSRT_ZPckrMove) + 1.0 ;
        bool bPckrOnStgPos = MT_GetCmdPos(miSRT_XPckr) <= PM.GetValue(miSRT_XPckr , pvSRT_XPckrPocket) + 0.1 &&
                             MT_GetCmdPos(miSRT_XPckr) >= PM.GetValue(miSRT_XPckr , pvSRT_XPckrPocket) - 0.1 &&
                             MT_GetCmdPos(miSRT_YPckr) <= PM.GetValue(miSRT_YPckr , pvSRT_YPckrPocket) + 0.1 &&
                             MT_GetCmdPos(miSRT_YPckr) >= PM.GetValue(miSRT_YPckr , pvSRT_YPckrPocket) - 0.1 ;

        //일단 이것은 안쓴다.
        //
        bool bPckrWorkingSTG= bPckrZDown && bPckrOnStgPos ;


        bool isCycleWork =  bOSTInspEnd && bOSTIdle  &&  //true true
                            bVTIInspEnd && bVTIIdle  &&  //true true
                            bVSNInspEnd && bVSNIdle  &&  //true true
                            bFrntReady  &&               //true
                            bAllActrBwd &&
                            bSRTNotWorkOnSTG ;
        bool isCycleEnd  =  bStgAllEmpty ;

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleWork  ) {Trace(m_sPartName.c_str(),"CycleWork   Stt"); Step.iSeq = scWork   ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default     End");                                                      Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                             return false ;
        case scWork  : if(CycleWork  ()){ Trace(m_sPartName.c_str(),"CycleWork   End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}

bool CStage::CycleHome()     //sun DLL direct access.
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

        case 10: OST.MoveActr(aiOST_PshrDnUp , ccBwd); //안전빵으로 그냥 여기서도 백워드 시킨다.
                 OST.MoveActr(aiOST_PTolUpDn , ccBwd);
                 VTI.MoveActr(aiVTI_PshrDnUp , ccBwd);
                 VTI.MoveActr(aiVTI_PTolUpDn , ccBwd);

                 //기본이 엠티임.
                 if(DM.ARAY[riSTF].CheckAllStat(csNone))DM.ARAY[riSTF].SetStat(csEmpty);
                 if(DM.ARAY[riSTL].CheckAllStat(csNone))DM.ARAY[riSTL].SetStat(csEmpty);
                 if(DM.ARAY[riSTB].CheckAllStat(csNone))DM.ARAY[riSTB].SetStat(csEmpty);
                 if(DM.ARAY[riSTR].CheckAllStat(csNone))DM.ARAY[riSTR].SetStat(csEmpty);



                 Step.iHome++ ;
                 return false ;

        case 11: if(!OST.MoveActr(aiOST_PshrDnUp , ccBwd))return false;
                 if(!OST.MoveActr(aiOST_PTolUpDn , ccBwd))return false;
                 if(!VTI.MoveActr(aiVTI_PshrDnUp , ccBwd))return false;
                 if(!VTI.MoveActr(aiVTI_PTolUpDn , ccBwd))return false;

                 Step.iHome++;
                 return false ;

        case 12: MT_DoHome(miSTG_TTble);
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miSTG_TTble)) return false ;
                 if(DM.ARAY[riSTF].CheckAllStat(csEmpty)&& //다비었을 경우는 위치 초기화 하고
                    DM.ARAY[riSTL].CheckAllStat(csEmpty)&&
                    DM.ARAY[riSTB].CheckAllStat(csEmpty)&&
                    DM.ARAY[riSTR].CheckAllStat(csEmpty)){
                     //MT_GoAbsMan(miSTG_TTble , PM.GetValue(miSTG_TTble , pvSTG_TTbleWork));
                     MoveMotr(miSTG_TTble , piSTG_TTbleWork); //오프셑 추가되어서
                 }
                 else { //작업 중이였을경우. 라스트 인덱스 포지션으로 이동 한다.
                     //MT_GoAbsMan(miSTG_TTble , PM.GetValue(miSTG_TTble , pvSTG_TTbleWork)+ m_dLastIdxPos);
                     MT_GoAbsMan(miSTG_TTble , GetMotrPos(miSTG_TTble , piSTG_TTbleWork) + m_dLastIdxPos);
                 }


                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GetStop(miSTG_TTble)) return false ;
                 //ChangeSTGArayByAngle(ScmGetStgAngle());
                 Step.iHome = 0;
                 return true ;

    }
}
/*
bool CStage::TestWork()
{

    const int iOSTRslt = 0 ;
    const int iVTRslt  = 1 ;
    const int iVSNRslt = 2 ;
    EN_ARAY_ID riTarget ;
    int        iTargetIdx ;

    EN_CHIP_STAT iTempStat ;

    //Cycle.
    //right stage.
    DM.ARAY[riSG1].SetDispLock(true);
    DM.ARAY[riSG2].SetDispLock(true);
    DM.ARAY[riSG3].SetDispLock(true);
    DM.ARAY[riSG4].SetDispLock(true);

    riTarget = ScmGetRghtSTGAray();
    for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
        iTempStat = DM.ARAY[riTarget].GetStat(0,0);
        if(ScmGetSckStat(riTarget ,0, i)!=csEmpty) { //일단 비젼 결과값 해당 레이어에 세팅 하고.
            iTempStat = ScmGetSckStat(riTarget ,0, i);
            ScmSetSckSubStat(riTarget ,0,i,iVSNRslt ,iTempStat);        //(EN_ARAY_ID _iAray , int r , int c , int idx , EN_CHIP_STAT _iStat)
            //iTempStat = DM.ARAY[riTarget].GetStat(0,0);
            ScmSetSckStat   (riTarget ,0,i,csGood);
            //iTempStat = DM.ARAY[riTarget].GetStat(0,0);
            if(ScmGetSckSubFail(riTarget ,0,i,iVSNRslt)){
                iTempStat = ScmGetSckSubStat(riTarget,0,i,iVSNRslt);
                ScmSetSckStat(riTarget ,0,i,iTempStat);
            }
            if(ScmGetSckSubFail(riTarget ,0,i,iVTRslt )){
                iTempStat = ScmGetSckSubStat(riTarget,0,i,iVTRslt );
                ScmSetSckStat(riTarget ,0,i,iTempStat);
            }
            if(ScmGetSckSubFail(riTarget ,0,i,iOSTRslt)){
                iTempStat = ScmGetSckSubStat(riTarget,0,i,iOSTRslt);
                ScmSetSckStat(riTarget ,0,i,iTempStat);
            }
        }
    }


    //뒤에꺼.
    //현재 검사의 결과값 해당 SubStat레이어로 옮긴후 UnKwn으로 세팅
    riTarget = ScmGetRearSTGAray();
    for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
        if(ScmGetSckStat(riTarget ,0, i)!=csEmpty) { //일단 비젼 결과값 해당 레이어에 세팅 하고.
            iTempStat = ScmGetSckStat(riTarget ,0, i);
            ScmSetSckSubStat(riTarget ,0,i,iVTRslt  ,iTempStat);        //(EN_ARAY_ID _iAray , int r , int c , int idx , EN_CHIP_STAT _iStat)
            ScmSetSckStat   (riTarget ,0,i,csUnkwn);
        }
    }

    //윈쪽꺼
    //현재 검사의 결과값 해당 SubStat레이어로 옮긴후 UnKwn으로 세팅
    riTarget = ScmGetLeftSTGAray();
    for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
        if(ScmGetSckStat(riTarget ,0, i)!=csEmpty) { //일단 비젼 결과값 해당 레이어에 세팅 하고.
            iTempStat = ScmGetSckStat(riTarget ,0, i);
            ScmSetSckSubStat(riTarget ,0,i,iOSTRslt  ,iTempStat);        //(EN_ARAY_ID _iAray , int r , int c , int idx , EN_CHIP_STAT _iStat)
            ScmSetSckStat   (riTarget ,0,i,csUnkwn);
        }
    }

    //앞에 껏은 회전이 안되므로 그냥쓰면 된다.
    riTarget = ScmGetFrntSTGAray();
    for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
        iTargetIdx = i ;
        if(DM.ARAY[riTarget].GetStat(0,iTargetIdx)!=csEmpty) {
            DM.ARAY[riTarget].SetSubStat(0,iTargetIdx,iVSNRslt ,csGood);
            DM.ARAY[riTarget].SetSubStat(0,iTargetIdx,iVTRslt  ,csGood);
            DM.ARAY[riTarget].SetSubStat(0,iTargetIdx,iOSTRslt ,csGood);
            DM.ARAY[riTarget].SetStat   (0,iTargetIdx,csUnkwn);
        }
    }

    DM.ARAY[riSG1].SetDispLock(false);
    DM.ARAY[riSG2].SetDispLock(false);
    DM.ARAY[riSG3].SetDispLock(false);
    DM.ARAY[riSG4].SetDispLock(false);
}
*/

//One Cycle.
bool CStage::CycleWork(void)
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

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    bool r1,r2 ;

    const int iOSTRslt = 0 ;
    const int iVTRslt  = 1 ;
    const int iVSNRslt = 2 ;
    EN_ARAY_ID riTarget ;
    int        iTargetIdx ;

    EN_CHIP_STAT iTempStat ;





    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //right stage.
                  riTarget = riSTR;
                  for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
                      //iTempStat = DM.ARAY[riTarget].GetStat(0,0);
                      if(DM.ARAY[riTarget].GetStat(i,0)!=csEmpty) { //일단 비젼 결과값 해당 레이어에 세팅 하고.
                          iTempStat = DM.ARAY[riTarget].GetStat(i,0);
                          DM.ARAY[riTarget].SetSubStat(i,0,iVSNRslt ,iTempStat);        //(EN_ARAY_ID _iAray , int r , int c , int idx , EN_CHIP_STAT _iStat)
                          DM.ARAY[riTarget].SetStat   (i,0,csGood);

                          //일단 ost->vt->vision 순서로 중요도 설정됌.
                          if(DM.ARAY[riTarget].GetSubStat(i,0,iVSNRslt) == csRslt1 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iVSNRslt) == csRslt2 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iVSNRslt) == csRslt3 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iVSNRslt) == csRslt4 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iVSNRslt) == csRslt5 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iVSNRslt) == csRslt6 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iVSNRslt) == csRslt7 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iVSNRslt) == csRslt8 ){
                              iTempStat = DM.ARAY[riTarget].GetSubStat(i,0,iVSNRslt);
                              DM.ARAY[riTarget].SetStat(i,0,iTempStat);
                          }
                          if(DM.ARAY[riTarget].GetSubStat(i,0,iVTRslt) == csRslt1 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iVTRslt) == csRslt2 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iVTRslt) == csRslt3 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iVTRslt) == csRslt4 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iVTRslt) == csRslt5 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iVTRslt) == csRslt6 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iVTRslt) == csRslt7 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iVTRslt) == csRslt8 ){
                              iTempStat = DM.ARAY[riTarget].GetSubStat(i,0,iVTRslt);
                              DM.ARAY[riTarget].SetStat(i,0,iTempStat);
                          }
                          if(DM.ARAY[riTarget].GetSubStat(i,0,iOSTRslt) == csRslt1 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iOSTRslt) == csRslt2 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iOSTRslt) == csRslt3 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iOSTRslt) == csRslt4 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iOSTRslt) == csRslt5 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iOSTRslt) == csRslt6 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iOSTRslt) == csRslt7 ||
                             DM.ARAY[riTarget].GetSubStat(i,0,iOSTRslt) == csRslt8 ){
                              iTempStat = DM.ARAY[riTarget].GetSubStat(i,0,iOSTRslt);
                              DM.ARAY[riTarget].SetStat(i,0,iTempStat);
                          }
                      }
                  }

                  riTarget = riSTB;
                  for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
                      if(DM.ARAY[riTarget].GetStat(0,i)!=csEmpty) { //일단 비젼 결과값 해당 레이어에 세팅 하고.
                          iTempStat = DM.ARAY[riTarget].GetStat(0,i);
                          DM.ARAY[riTarget].SetSubStat(0,i,iVTRslt  ,iTempStat);        //(EN_ARAY_ID _iAray , int r , int c , int idx , EN_CHIP_STAT _iStat)
                          DM.ARAY[riTarget].SetStat   (0,i,csUnkwn);
                      }
                  }

                  riTarget = riSTL;
                  for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
                      if(DM.ARAY[riTarget].GetStat(i,0)!=csEmpty) { //일단 비젼 결과값 해당 레이어에 세팅 하고.
                          iTempStat = DM.ARAY[riTarget].GetStat(i,0);
                          DM.ARAY[riTarget].SetSubStat(i,0,iOSTRslt  ,iTempStat);        //(EN_ARAY_ID _iAray , int r , int c , int idx , EN_CHIP_STAT _iStat)
                          DM.ARAY[riTarget].SetStat   (i,0,csUnkwn);
                      }
                  }

                  riTarget = riSTF;
                  for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
                      if(DM.ARAY[riTarget].GetStat(0,i)!=csEmpty) { //일단 비젼 결과값 해당 레이어에 세팅 하고.
                          DM.ARAY[riTarget].SetStat   (0,i,csUnkwn);
                      }
                  }


                   /*

                  //뒤에꺼.
                  //현재 검사의 결과값 해당 SubStat레이어로 옮긴후 UnKwn으로 세팅
                  riTarget = riSTB;
                  for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
                      if(ScmGetSckStat(riTarget ,0, i)!=csEmpty) { //일단 비젼 결과값 해당 레이어에 세팅 하고.
                          iTempStat = ScmGetSckStat(riTarget ,0, i);
                          ScmSetSckSubStat(riTarget ,0,i,iVTRslt  ,iTempStat);        //(EN_ARAY_ID _iAray , int r , int c , int idx , EN_CHIP_STAT _iStat)
                          ScmSetSckStat   (riTarget ,0,i,csUnkwn);
                      }
                  }

                  //윈쪽꺼
                  //현재 검사의 결과값 해당 SubStat레이어로 옮긴후 UnKwn으로 세팅
                  riTarget = ScmGetLeftSTGAray();
                  for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
                      if(ScmGetSckStat(riTarget ,0, i)!=csEmpty) { //일단 비젼 결과값 해당 레이어에 세팅 하고.
                          iTempStat = ScmGetSckStat(riTarget ,0, i);
                          ScmSetSckSubStat(riTarget ,0,i,iOSTRslt  ,iTempStat);        //(EN_ARAY_ID _iAray , int r , int c , int idx , EN_CHIP_STAT _iStat)
                          ScmSetSckStat   (riTarget ,0,i,csUnkwn);
                      }
                  }

                  //앞에 껏은 회전이 안되므로 그냥쓰면 된다.
                  riTarget = ScmGetFrntSTGAray();
                  for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
                      iTargetIdx = i ;
                      if(DM.ARAY[riTarget].GetStat(0,iTargetIdx)!=csEmpty) {
                          DM.ARAY[riTarget].SetSubStat(0,iTargetIdx,iVSNRslt ,csGood);
                          DM.ARAY[riTarget].SetSubStat(0,iTargetIdx,iVTRslt  ,csGood);
                          DM.ARAY[riTarget].SetSubStat(0,iTargetIdx,iOSTRslt ,csGood);
                          DM.ARAY[riTarget].SetStat   (0,iTargetIdx,csUnkwn);
                      }
                  }
                  DM.ARAY[riSG1].SetDispLock(false);
                  DM.ARAY[riSG2].SetDispLock(false);
                  DM.ARAY[riSG3].SetDispLock(false);
                  DM.ARAY[riSG4].SetDispLock(false);

                  */

                  Step.iCycle++ ;
                  return false ;

        case  11: MT_GoIncRun(miSTG_TTble , 90.0);
                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MT_GetStop(miSTG_TTble)) return false ;
                  ChangeSTGAray90();

                  SetLastCmd();
                  m_tmDelay.Clear();

                  Step.iCycle++;
                  return false ;

        case  13: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iStgAftStopDelay)) return false ;
                  for(int i = 0 ; i < MAX_SUBSTAT ; i++){
                      DM.ARAY[riSTF].SetSubStat(i,csEmpty);
                  }
                  riTarget = riSTR;
                  //DM.ARAY[riSG1].SetDispLock(true);
                  //DM.ARAY[riSG2].SetDispLock(true);
                  //DM.ARAY[riSG3].SetDispLock(true);
                  //DM.ARAY[riSG4].SetDispLock(true);
                  //ChangeSTGArayByAngle(ScmGetStgAngle());
                  //DM.ARAY[riSG1].SetDispLock(false);
                  //DM.ARAY[riSG2].SetDispLock(false);
                  //DM.ARAY[riSG3].SetDispLock(false);
                  //DM.ARAY[riSG4].SetDispLock(false);


                  Step.iCycle = 0 ;
                  return true ;
    }
}


//---------------------------------------------------------------------------
bool CStage::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool bVTIPshNotBwd = !AT_Complete(aiVTI_PshrDnUp , ccBwd);
    bool bVTITolNotBwd = !AT_Complete(aiVTI_PTolUpDn , ccBwd);
    bool bOSTPshNotBwd = !AT_Complete(aiOST_PshrDnUp , ccBwd);
    bool bOSTTolNotBwd = !AT_Complete(aiOST_PTolUpDn , ccBwd);

    bool bPckrZDown    = MT_GetTrgPos(miSRT_ZPckr) > PM.GetValue(miSRT_ZPckr , pvSRT_ZPckrMove) + 1.0 ;
    bool bPckrOnStgPos = MT_GetCmdPos(miSRT_XPckr) <= PM.GetValue(miSRT_XPckr , pvSRT_XPckrPocket) + 0.1 &&
                         MT_GetCmdPos(miSRT_XPckr) >= PM.GetValue(miSRT_XPckr , pvSRT_XPckrPocket) - 0.1 &&
                         MT_GetCmdPos(miSRT_YPckr) <= PM.GetValue(miSRT_YPckr , pvSRT_YPckrPocket) + 0.1 &&
                         MT_GetCmdPos(miSRT_YPckr) >= PM.GetValue(miSRT_YPckr , pvSRT_YPckrPocket) - 0.1 ;




    bool bPckrFwdOnStgPos = bPckrZDown && bPckrOnStgPos ;

    bool bActrNorBwd   = bVTIPshNotBwd || bVTITolNotBwd || bOSTPshNotBwd || bOSTTolNotBwd ;


    if(_iMotr == miSTG_TTble){
        if(bPckrFwdOnStgPos){ sMsg = "Picker is down on stage"                     ; bRet = false ;}
        switch(_iPstnId) {
            default                   :                 { sMsg = "Not " +AnsiString(MT_GetName(_iMotr))+"'s Postion" ; bRet = false ;} break ;
            case piSTG_TTbleWait      : if(bActrNorBwd) { sMsg = "OST or VT Cylinder is Not Bwd"                     ; bRet = false ;} break ;
            case piSTG_TTbleWork      : if(bActrNorBwd) { sMsg = "OST or VT Cylinder is Not Bwd"                     ; bRet = false ;} break ;
        }
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
bool CStage::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
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

bool CStage::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

void CStage::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CStage::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CStage::SetLastCmd()
{
    m_dLastIdxPos = MT_GetCmdPos(miSTG_TTble) - GetMotrPos(miSTG_TTble , piSTG_TTbleWork);//오프셑 추가 되어서...PM.GetValue(miSTG_TTble , pvSTG_TTbleWork);
    return ; //이파트는 필요 없다.

}

bool CStage::CheckMoved()
{
    if(m_dLastIdxPos != MT_GetCmdPos(miSTG_TTble)) return true  ; //이파트는 필요 없다.
    else                                           return false ;

}

bool CStage::CheckStop()
{
    if(!MT_GetStop(miSTG_TTble)) return false ;

    //if(!AT_Done(aiLDR_IndxUpUp)) return false ;

    return true ;
}

void CStage::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";
    if(m_dLastIdxPos > MT_GetMaxPos(miSTG_TTble)) m_dLastIdxPos = MT_GetMaxPos(miSTG_TTble) ;
    int iTemp ;
    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
}

void CStage::ChangeSTGAray90()
{
    //지역변수로 했을때에 메뉴 관련 메모리 누수 있음...
    //CArray FrntBuff ;
    //riSTB로 그냥 고정할당함.

    EN_CHIP_STAT iTempStat    ;
    EN_CHIP_STAT iTempSubStat ;

    int sr,sc , dr,dc;
    EN_ARAY_ID iSrc ;
    EN_ARAY_ID iDst ;

    //Stat SubStat 까지 한방임.
    DM.ARAY[riSTU].CopyData(DM.ARAY[riSTF]) ;





    //SubStat Index
    const int iOSTRslt = 0 ;
    const int iVTRslt  = 1 ;
    const int iVSNRslt = 2 ;


    //오른쪽것은 앞으로 온다.
    for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
        iSrc = riSTR ;
        iDst = riSTF ;

        sr = i ;
        sc = 0 ;

        dr = 0 ;
        dc = MAX_TOOL_SOCK_COL - i - 1 ;

        iTempStat    = DM.ARAY[iSrc].GetStat   (sr,sc);
        DM.ARAY[iDst].SetStat(dr,dc ,iTempStat) ;

        //앞짝은 이거 해도 그만 안해도 그만.
        iTempSubStat = DM.ARAY[iSrc].GetSubStat(sr,sc,iVSNRslt);
        DM.ARAY[iDst].SetSubStat(dr,dc ,iVSNRslt, iTempSubStat);

        iTempSubStat = DM.ARAY[iSrc].GetSubStat(sr,sc,iVTRslt );
        DM.ARAY[iDst].SetSubStat(dr,dc ,iVTRslt , iTempSubStat);

        iTempSubStat = DM.ARAY[iSrc].GetSubStat(sr,sc,iOSTRslt);
        DM.ARAY[iDst].SetSubStat(dr,dc ,iOSTRslt, iTempSubStat);
    }

    //뒤쪽것은 오른쪽으로 온다.
    for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
        iSrc = riSTB ;
        iDst = riSTR ;

        sr = 0 ;
        sc = i ;

        dr = i ;
        dc = 0 ;

        iTempStat    = DM.ARAY[iSrc].GetStat   (sr,sc);
        DM.ARAY[iDst].SetStat(dr,dc ,iTempStat) ;

        iTempSubStat = DM.ARAY[iSrc].GetSubStat(sr,sc,iVSNRslt);
        DM.ARAY[iDst].SetSubStat(dr,dc ,iVSNRslt, iTempSubStat);

        iTempSubStat = DM.ARAY[iSrc].GetSubStat(sr,sc,iVTRslt );
        DM.ARAY[iDst].SetSubStat(dr,dc ,iVTRslt , iTempSubStat);

        iTempSubStat = DM.ARAY[iSrc].GetSubStat(sr,sc,iOSTRslt);
        DM.ARAY[iDst].SetSubStat(dr,dc ,iOSTRslt, iTempSubStat);
    }

    //왼쪽것은 뒤쪽으로 간다.
    for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
        iSrc = riSTL ;
        iDst = riSTB ;

        sr = i ;
        sc = 0 ;

        dr = 0 ;
        dc = MAX_TOOL_SOCK_COL - i - 1 ;

        iTempStat    = DM.ARAY[iSrc].GetStat   (sr,sc);
        DM.ARAY[iDst].SetStat(dr,dc ,iTempStat) ;

        iTempSubStat = DM.ARAY[iSrc].GetSubStat(sr,sc,iVSNRslt);
        DM.ARAY[iDst].SetSubStat(dr,dc ,iVSNRslt, iTempSubStat);

        iTempSubStat = DM.ARAY[iSrc].GetSubStat(sr,sc,iVTRslt );
        DM.ARAY[iDst].SetSubStat(dr,dc ,iVTRslt , iTempSubStat);

        iTempSubStat = DM.ARAY[iSrc].GetSubStat(sr,sc,iOSTRslt);
        DM.ARAY[iDst].SetSubStat(dr,dc ,iOSTRslt, iTempSubStat);
    }

    //버퍼에 있던것은 왼쪽으로 간다.
    for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
        //iSrc = riSTL ;
        iDst = riSTL ;

        sr = 0 ;
        sc = i ;

        dr = i ;
        dc = 0 ;

        iTempStat    = DM.ARAY[riSTU].GetStat   (sr,sc);
        DM.ARAY[iDst].SetStat(dr,dc ,iTempStat) ;

        iTempSubStat = DM.ARAY[riSTU].GetSubStat(sr,sc,iVSNRslt);
        DM.ARAY[iDst].SetSubStat(dr,dc ,iVSNRslt, iTempSubStat);

        iTempSubStat = DM.ARAY[riSTU].GetSubStat(sr,sc,iVTRslt );
        DM.ARAY[iDst].SetSubStat(dr,dc ,iVTRslt , iTempSubStat);

        iTempSubStat = DM.ARAY[riSTU].GetSubStat(sr,sc,iOSTRslt);
        DM.ARAY[iDst].SetSubStat(dr,dc ,iOSTRslt, iTempSubStat);
    }
}





