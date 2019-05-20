//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Index.h"
#include "LoaderRear.h"
//#include "RunThread.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "Stage.h"
//#include "LotUnit.h"
#include "UserIni.h"
#include "UserFile.h"
//#include "Stage.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CIndex IDX;

#define GET_OFFSET_Z 6.0

CIndex::CIndex(void)
{

}

CIndex::~CIndex (void)
{

}

void CIndex::Init()
{
    m_sPartName = "Index" ;
    m_sCheckSafeMsg = "" ;

    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();

}

void CIndex::Close()
{
    Load(false);
}

void CIndex::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}



bool CIndex::FindChip(int &r ,int &c , EN_CHIP_STAT _eStat)
{    //FindChip(r,c,csUnkwn,riLDR_R);
    return LDR_R.FindChip(r,c,_eStat);
}

//bool CIndex::FindChip(int &c , EN_ARAY_ID &id)
//{
    //int iWorkC = DM.ARAY[Add.riIDX].FindFrstCol(Add.csAfterWork) ;
    //int iWaitC = DM.ARAY[Add.riIDX].FindFrstCol(Add.csWait     ) ;
    //int iC     = iWorkC > iWaitC ? iWorkC

//    int iWaitC   = DM.ARAY[riIDX].FindLastCol(csWait ) ;
//    int iUnknwnC = DM.ARAY[riIDX].FindLastCol(csUnkwn) ;
//
//    int iC ;
//    if(iWaitC != -1 && iUnknwnC != -1 ){
//        iC = iWaitC > iUnknwnC ? iWaitC : iUnknwnC ;
//    }
//    else if( iWaitC != -1){
//        iC = iWaitC ;
//    }
//    else if( iUnknwnC != -1){
//        iC = iUnknwnC ;
//    }
//    else {
//        iC = -1 ;
//    }
//
//    id = riIDX ;
//    c  = iC ;
//    return true ;
//}


double CIndex::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;
    int c ;
    EN_ARAY_ID _iAray;
//    FindChip(c , _iAray)    ; //현재 작업해야할 포지션.

//    double dWork = PM.GetValue (_iMotr , pvIDX_XIndxWorkStart) + (OM.DevInfo.iBoatColCnt-c-1) * OM.DevInfo.dBoatColPitch ;
    //double dGetPos = PM.GetValue(_iMotr, pvIDX_XIndxRGetFrst) + OM.DevInfo.dCmsWidth ;


    if(_iMotr == miIDX_XIndxR){
             if(_iPstnId ==  piIDX_XIndxRWait   ) dPos = PM.GetValue (_iMotr , pvIDX_XIndxRWait   );
        else if(_iPstnId ==  piIDX_XIndxRGetWait) dPos = PM.GetValue (_iMotr , pvIDX_XIndxRGetWait);
        else if(_iPstnId ==  piIDX_XIndxRCheck1 ) dPos = PM.GetValue (_iMotr , pvIDX_XIndxRCheck1 ); // CMOS 로더에서 CMOS Index 공급 첫 번째 위치
        else if(_iPstnId ==  piIDX_XIndxRCheck2 ) dPos = PM.GetValue (_iMotr , pvIDX_XIndxRCheck2 );
        else if(_iPstnId ==  piIDX_XIndxRRotator) dPos = PM.GetValue (_iMotr , pvIDX_XIndxRRotator);
        else                                      dPos = MT_GetCmdPos(_iMotr                      );
    }

    else if(_iMotr == miIDX_TRotat){
             if(_iPstnId ==  piIDX_TRotatWait  ) dPos = PM.GetValue (_iMotr , pvIDX_TRotatWait   );
        else if(_iPstnId ==  piIDX_TRotatRotate) dPos = PM.GetValue (_iMotr , pvIDX_TRotatRotate );
        else if(_iPstnId ==  piIDX_TRotatNormal) dPos = PM.GetValue (_iMotr , pvIDX_TRotatNormal );
        else                                     dPos = MT_GetCmdPos(_iMotr                      );
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CIndex::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}
void CIndex::Update()
{
//    if(IO_GetXDn(Add.xIDX_IndxNotOvLd)) {
//        EM_SetErrMsg(eiPRT_OverLoad , (m_sPartName + " Index에서 오버로드가 발생되었습니다.").c_str());
//        MT_EmgStop(Add.miIDX_XIndx);
//        MoveActr(Add.aiIDX_IndxUpDn , ccFwd);
//    }
//    if(DM.ARAY[Add.riPRI].CheckAllStat(csNone)){
//        m_tmHeat.Clear();
//    }
}

bool CIndex::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CIndex::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CIndex::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: // 로테이트 사이클이 아닌데, 로테이트 실린더가 올라가 있으면 에러 띄우기
                 if(!DM.ARAY[riRTT].CheckAllStat(csNone)){
                     Step.iToStart = 0 ;
                     return false ;
                 }
                 MoveMotr(miIDX_TRotat , piIDX_TRotatWait);

                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveMotr(miIDX_TRotat , piIDX_TRotatWait)) return false ;
                 MoveActr(aiIDX_RoatorUpDn , ccBwd);
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CIndex::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10: //MoveActr(aiIDX_RoatorUpDn, ccBwd);

                 Step.iToStop++;
                 return false ;

        case 11: //if(!MoveActr(aiIDX_RoatorUpDn, ccBwd)) return false ;
                 SetLastCmd();
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CIndex::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;                   // piIDX_XIndxRGetFrst        , // CMOS 로더에서 CMOS Index 공급 첫 번째 위치

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;


            int r,c ;

/*   20181029 오성철과장 인덱스에서 충돌감지로 에러 몇번 떳을때 인덱스 Get조건이 안되는 문제로 업데이트.
            bool isWorking = STG.GetWorkingRC(r,c);

            int  iMaxModuleCnt    = OM.DevInfo.iMidColCnt * OM.DevInfo.iMidRowCnt ;
            int  iMidHeightCnt    = DM.ARAY[riSTG].GetCntStat(csMidREndHeight ); //1장작업 종료 상태.
            int  iCmsREndAlignCnt = DM.ARAY[riSTG].GetCntStat(csCmsREndAlign  );

            //Epoxy First 체크박스에서 콤보박스로 옵션 변경 진섭
            int  iWorkWaitCnt     = OM.DevOptn.bUseFstDisp ? DM.ARAY[riSTG].GetCntStat(csCmsLDisp1 )+DM.ARAY[riSTG].GetCntStat(csCmsRNeed) : DM.ARAY[riSTG].GetCntStat(csCmsRNeed) ;

            //2번째장 작업 부터는 그전 작업하던 것이 탑유부이 작업 할때쯤 꺼내오면 렉타임 없음.
            bool bNeedNext        =  DM.ARAY[riSTG].GetCntStat(csCmsRNeed) &&
                                   ((iMidHeightCnt + iWorkWaitCnt + iCmsREndAlignCnt)==iMaxModuleCnt);//나중에 스킵 켜고 진행 할 경우.
                                                                                                      //Fix 나 TopUV일경우로바꿔야 한다

            bool isCycleGet     =  !OM.CmnOptn.bRLoadingStop &&
                                   MT_CmprPos(miLDR_ZElevR , LDR_R.GetMotrPos(miLDR_ZElevR , piLDR_ZElevRPick )) &&
                                  (DM.ARAY[riSTG  ].CheckAllStat(csNone) || bNeedNext  ) &&
                                   DM.ARAY[riTRF  ].CheckAllStat(csNone)      &&
                                   DM.ARAY[riIDX  ].CheckAllStat(csNone)      &&
                                   DM.ARAY[riRTT  ].CheckAllStat(csNone)      &&
                                   DM.ARAY[riLDR_R].GetCntStat  (csUnkwn)     &&
                                   DM.ARAY[riLDR_R].GetCntStat  (csDetect) ==0&&
                                   LDR_R.GetSeqStep() == CLoaderRear::scIdle ;//  !IO_GetX(xIDX_Detect)    ;
            bool isCyclePlace   = MT_CmprPos(miLDR_ZElevR , LDR_R.GetMotrPos(miLDR_ZElevR , piLDR_ZElevRPlce )) &&
                                  DM.ARAY[riRTT  ].CheckAllStat(csWorkEnd) &&
                                  LDR_R.GetSeqStep() == CLoaderRear::scIdle ;
*/
//20181029 새버전... 홀드 보안.
            bool isWorking = STG.GetWorkingRC(r,c);

            int  iMidREndHeightCnt = DM.ARAY[riSTG].GetCntStat(csMidREndHeight ); //1장작업 종료 상태.
            int  iCmsREndAlignCnt  = DM.ARAY[riSTG].GetCntStat(csCmsREndAlign  ); //거의 막바지작업때(얼라인확인 Or 높이화인)에 인덱스로 로더리어에서 꺼내오게함.
            int  iWorkWaitCnt      = DM.ARAY[riSTG].GetCntStat(csCmsLDisp1 )+DM.ARAY[riSTG].GetCntStat(csCmsRNeed);


            bool bStgNeedModule   =  DM.ARAY[riSTG].GetCntStat(csCmsRNeed) && //RNeed는 꼭있어야 함. 이거 없으면 마지막미들블럭 작업하고 찝으러감.
                                   ((iMidREndHeightCnt + iWorkWaitCnt + iCmsREndAlignCnt)==OM.DevInfo.iMidColCnt * OM.DevInfo.iMidRowCnt);//다른상태는 무시하고 마지막(얼라인확인 Or 높이화인)만걸러냄.

            //미들블럭이 로더에 남아있거나 프리레일에 작업할게 있는경우.
            bool bExistMidBfStg =  DM.ARAY[riLDR_F].GetCntStat(csUnkwn   ) ||
                                   DM.ARAY[riPRL  ].GetCntStat(csUnkwn   ) ;

            bool isCycleGet     =  !OM.CmnOptn.bRLoadingStop &&
                                   MT_CmprPos(miLDR_ZElevR , LDR_R.GetMotrPos(miLDR_ZElevR , piLDR_ZElevRPick )) &&
                                  (bExistMidBfStg   || bStgNeedModule  )      &&
                                   DM.ARAY[riTRF  ].CheckAllStat(csNone)      &&
                                   DM.ARAY[riIDX  ].CheckAllStat(csNone)      &&
                                   DM.ARAY[riRTT  ].CheckAllStat(csNone)      &&
                                   DM.ARAY[riLDR_R].GetCntStat  (csUnkwn)     &&
                                   DM.ARAY[riLDR_R].GetCntStat  (csDetect) ==0&& //처음에로더 자제유무 센싱시퀜스.
                                   LDR_R.GetSeqStep() == CLoaderRear::scIdle ;//  !IO_GetX(xIDX_Detect)    ;
            bool isCyclePlace   = MT_CmprPos(miLDR_ZElevR , LDR_R.GetMotrPos(miLDR_ZElevR , piLDR_ZElevRPlce )) &&
                                  DM.ARAY[riRTT  ].CheckAllStat(csWorkEnd) &&
                                  LDR_R.GetSeqStep() == CLoaderRear::scIdle ;

    //다시 해야함..
    //bool isCycleRotator =  DM.ARAY[riIDX].CheckAllStat(csVisnAt) && DM.ARAY[riRTT].CheckAllStat(csNone) &&  IO_GetX(xIDX_RotatorDn) ;
    bool isCycleRotator = false;
    bool isCycleEnd     = DM.ARAY[riRTT].CheckAllStat(csNone) ;


    if(EM_IsErr()) return false ;

        //eiULD_MgzSupply 추가
//        if (!OM.MstOptn.bDebugMode) {
//            if (!DM.ARAY[Add.riIDX   ].CheckAllStat(csNone ) &&
//                !DM.ARAY[Add.riULDTop].GetCntStat  (csEmpty) &&
//                !DM.ARAY[Add.riULDBtm].GetCntStat  (csEmpty)) EM_SetErrMsg(eiULD_MgzSupply , "UnLoder Mgz Supply Error.") ;
//        }

         //Normal Decide Step.
             if (isCycleGet    ) {Trace(m_sPartName.c_str(),"CycleGet      Stt"); Step.iSeq = scGet        ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCyclePlace  ) {Trace(m_sPartName.c_str(),"CyclePlace    Stt"); Step.iSeq = scPlace      ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleRotator) {Trace(m_sPartName.c_str(),"CycleRotator  Stt"); Step.iSeq = scRotator    ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleEnd    ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
   }

    //Cycle.
    switch (Step.iSeq) {
        default           :                      Trace(m_sPartName.c_str(),"default     End");                                                   Step.iSeq = scIdle ;  return false ;
        case scIdle       :                                                                                                                                            return false ;
        case scGet        : if(CycleGet     ()){ Trace(m_sPartName.c_str(),"CycleGet    End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scPlace      : if(CyclePlace   ()){ Trace(m_sPartName.c_str(),"CyclePlace  End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scRotator    : if(CycleRotator ()){ Trace(m_sPartName.c_str(),"Rotator     End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}

bool CIndex::IsWorkPos()
{
//    return MT_CmprPos(miIDX_XIndxR ,GetMotrPos(miIDX_XIndxR , piIDX_XIndxWork)) ;

}

bool CIndex::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        sTemp  = sTemp.sprintf(" %s TIMEOUT Step.iHome=%02d" , __FUNC__ , Step.iHome );
        sTemp  = m_sPartName + sTemp ;
        EM_SetErrMsg(eiPRT_HomeTo,sTemp.c_str());
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

        case 10: IO_SetY(yIDX_RttPumpOn , true);
                 m_tmDelay.Clear();
                 Step.iHome++;
                 return false ;

        case 11: if(!m_tmDelay.OnDelay(true , 1000))return false ;
                 if(!DM.ARAY[riRTT].CheckAllStat(csNone) ){ //자제 있는경우.
                     Step.iHome=20 ;
                     return false ;
                 }
                 //자재가 없으면 30번.
                 IO_SetY(yIDX_RttPumpOn, false);
                 Step.iHome=30 ;
                 return false ;


        //위에서 씀.
        //Rotator 자재가 있을시 사용.
        case 20: MT_DoHome(miIDX_XIndxR);
                 Step.iHome++;
                 return false ;

        case 21: if(!MT_GetHomeEnd(miIDX_XIndxR)) return false ;
                 MoveActr(aiIDX_RoatorUpDn, ccFwd);
                 //if(!AT_Complete(aiIDX_RoatorUpDn, ccFwd)) return false ;
                 Step.iHome++;
                 return false ;

        case 22: if(!MoveActr(aiIDX_RoatorUpDn, ccFwd))return false;
                 MT_DoHome(miIDX_TRotat);
                 Step.iHome++;
                 return false;

        case 23: if(!MT_GetHomeEnd(miIDX_TRotat)) return false;
                 //if(!OM.DevOptn.bNotUseRotate && DM.ARAY[riSTG].GetCntStat(csWork) % 2){ //홀수  1,3,5
                 //    MoveMotr(miIDX_TRotat , piIDX_TRotatRotate);
                 //}
                 //else {
                 //    MoveMotr(miIDX_TRotat , piIDX_TRotatNormal);
                 //
                 //}

                 MoveMotr(miIDX_TRotat , piIDX_TRotatNormal);
                 Step.iHome++;
                 return false ;

        case 24: if(!MT_GetStopInpos(miIDX_TRotat)) return false ;
                 Step.iHome = 0;
                 return true;


        //위에서 씀.
        //Rotator 자재 없을시 사용.
        case 30: MoveActr(aiIDX_RoatorUpDn, ccBwd);
                 Step.iHome++;
                 return false ;

        case 31: if(!MoveActr(aiIDX_RoatorUpDn, ccBwd)) return false ;
                 MT_DoHome(miIDX_XIndxR);
                 Step.iHome++;
                 return false ;

        case 32: if(!MT_GetHomeEnd(miIDX_XIndxR)) return false ;
                 MT_DoHome(miIDX_TRotat);
                 Step.iHome++;
                 return false ;

        case 33: if(!MT_GetHomeEnd(miIDX_TRotat)) return false ;
                 MoveMotr(miIDX_TRotat , piIDX_TRotatWait);
                 Step.iHome++;
                 return false ;

        case 34: if(!MT_GetStopInpos(miIDX_TRotat)) return false ;
                 Step.iHome = 0;
                 return true;
    }
}

//One Cycle.
bool CIndex::CycleGet()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
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

    int r,c ;
    FindChip(r,c,csUnkwn);
    int iRc = OM.DevInfo.iLDRRMgzBayCnt - c - 1 ;
    String sModID ;


    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: //if(IO_GetX(xIDX_Detect)) {
                 //     EM_SetErrMsg(eiPRT_Crash, (m_sPartName + "CMOS 자재가 있습니다. 자재를 제거해주세요.").c_str());
                 //     Step.iCycle = 0 ;
                 //     return true ;
                 //}
                 MoveActr(aiIDX_RoatorUpDn , ccBwd);
                 MoveMotr(miIDX_TRotat , piIDX_TRotatWait);
                 LDR_R.MoveMotr(miLDR_YConvR,piLDR_YConvRWork);
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveActr(aiIDX_RoatorUpDn , ccBwd)) return false ;
                 if(!MoveMotr(miIDX_TRotat , piIDX_TRotatWait)) return false ;
                 if(!LDR_R.MoveMotr(miLDR_YConvR,piLDR_YConvRWork)) return false ;
                 //MoveMotr(miLDR_ZElevR, piLDR_ZElevRPickFrst);
                 Step.iCycle++;
                 return false ;

        case 12: //if(!MoveMotr(miLDR_ZElevR, piLDR_ZElevRPickFrst)) return false ;
                 //MoveMotr(miIDX_XIndxR, piIDX_XIndxRCheck1);
                 if(iRc == 0) {

                     MoveMotr(miIDX_XIndxR , piIDX_XIndxRCheck1);


                 }
                 if(iRc == 1) {
                     if(OM.CmnOptn.iWorkMode == wmHeight){
                         EM_SetErrMsg(eiPRT_Detect , "높이측정모드인데 로더에 1번째 베이에 자제가 있습니다.");
                         Step.iCycle++;
                         return false ;
                     }
                     MoveMotr(miIDX_XIndxR , piIDX_XIndxRCheck2);
                 }

                 Step.iCycle++;
                 return false ;

        case 13: if(IO_GetX(xIDX_PkgCrashDetect)){ //인덱스 끝트머리에 있는 충돌 감지센서 센싱시.
                     Step.iCycle = 50 ;
                     return false ;
                 }


                 if(!MT_GetStopInpos(miIDX_XIndxR)) return false ;
                 IO_SetY(yIDX_IDXIonizer , true);
                 if(!IO_GetX(xIDX_Detect)) { //가봤는데 감지 안된경우.
                     MT_GoInc(miIDX_XIndxR , 20.0 , 3);
                     Step.iCycle++ ;
                     return false ;
                 }
                 Step.iCycle++;
                 return false ;

        case 14: if(IO_GetX(xIDX_Detect)) { //가보다 감지가 됨.
                     MT_Stop(miIDX_XIndxR);
                 }
                 if(!MT_GetStopInpos(miIDX_XIndxR))return false ;
                 if(!IO_GetX(xIDX_Detect)) { //가봤는데 끝까지 감지 안된경우.
                     Step.iCycle =30 ;
                     return false ;
                 }


                 MT_GoInc(miIDX_XIndxR , -25.0 , 3);
                 Step.iCycle++;
                 return false ;

        case 15: if(!IO_GetX(xIDX_Detect)) { //가보다 감지 안된경우.
                     MT_Stop(miIDX_XIndxR);
                 }
                 if(!MT_GetStopInpos(miIDX_XIndxR))return false ;

                 MT_GoIncRun(miIDX_XIndxR , PM.GetValue(miIDX_XIndxR , poIDX_XIndxRGet));
                 MT_GoAbsMan(miLDR_YConvR , PM.GetValue(miLDR_YConvR , pvLDR_YConvRWork)-0.5);
                 Step.iCycle++;
                 return false ;

        case 16: if(!MT_GetStopInpos(miIDX_XIndxR)) return false ;
                 MT_GoIncRun(miLDR_ZElevR, -GET_OFFSET_Z );
                 Step.iCycle++;
                 return false ;

        case 17: if(!MT_GetStopInpos(miLDR_ZElevR)) return false ;
                 IO_SetY(yIDX_IdxVac , true);

                 MoveMotr(miIDX_XIndxR, piIDX_XIndxRRotator, true);
                 LDR_R.MoveMotr(miLDR_YConvR,piLDR_YConvRWork);
                 Step.iCycle++;
                 return false ;

        case 18: if(!MoveMotr(miIDX_XIndxR, piIDX_XIndxRRotator, true)) return false ;
                 if(!LDR_R.MoveMotr(miLDR_YConvR,piLDR_YConvRWork)) return false ; 
                 IO_SetY(yIDX_IdxVac , false ) ;
                 MoveActr(aiIDX_RoatorUpDn , ccFwd);
                 Step.iCycle++;
                 return false ;

        case 19: if(!MoveActr(aiIDX_RoatorUpDn , ccFwd))return false ;
                 if(OM.CmnOptn.iWorkMode == wmNormal)IO_SetY(yIDX_RttPumpOn , true);
                 MoveMotr(miIDX_XIndxR , piIDX_XIndxRWait);
                 Step.iCycle++;
                 return false ;

        case 20: if(!MoveMotr(miIDX_XIndxR , piIDX_XIndxRWait)) return false ;
                 IO_SetY(yIDX_IDXIonizer , false);
                 //if(!OM.DevOptn.bNotUseRotate && DM.ARAY[riSTG].GetCntStat(csWork) % 2){ //홀수  1,3,5

                 //}
                 MoveMotr(miIDX_TRotat , piIDX_TRotatNormal);

                 Step.iCycle++;
                 return false ;

        case 21: if(!MT_GetStopInpos(miIDX_TRotat)) return false ;
                 if(OM.CmnOptn.iWorkMode == wmNormal) {
                     sModID = DM.ARAY[riLDR_R].CHPS[r][c].GetID();
                     DM.ARAY[riLDR_R].SetStat(r,c,csEmpty);
                     DM.ARAY[riLDR_R].CHPS[r][c].SetID("");
                     DM.ARAY[riRTT].SetStat(csCmsLOri);
                     DM.ARAY[riRTT].CHPS[0][0].SetID(sModID);
                     DM.ARAY[riRTT].SetID(sModID);
                 }
                 else {
                     DM.ARAY[riLDR_R].SetStat(r,c,csMask);
                     DM.ARAY[riRTT].SetStat(csCmsLHeight);

                 }
                 Step.iCycle=0;
                 return true ;




        //위에서씀.
        //로더에 찝으러 갔더니 없음...
        case 30: MoveMotr(miIDX_XIndxR , piIDX_XIndxRGetWait);
                 Step.iCycle++;
                 return false ;

        case 31: if(!MoveMotr(miIDX_XIndxR , piIDX_XIndxRGetWait)) return false ;
                 IO_SetY(yIDX_IDXIonizer, false);

                 //없으면 에러로 처리.
                 //DM.ARAY[riLDR_R].SetStat(r,c,csEmpty);
                 EM_SetErrMsg(eiPKG_Dispr, "리어로더에 슬롯에 자제가 없습니다.");
                 Step.iCycle = 0 ;
                 return true ;

        //위에서씀.
        //인덱스 충돌 감지 센서 작동.
        case 50: MT_EmgStop(miIDX_XIndxR);
                 EM_SetErrMsg(eiPRT_Crash, "로더진입시에 인덱스 충돌 센서가 감지되었습니다.");
                 Step.iCycle++;
                 return false ;

        case 51: if(!MT_GetStopInpos(miIDX_XIndxR)) return false;
                 MoveMotr(miIDX_XIndxR , piIDX_XIndxRWait);
                 Step.iCycle=0;
                 return false ;




    }
}



bool CIndex::CyclePlace()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
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

    int r,c ;
    FindChip(r,c,csMask);
    int dPlaceStt ;
    int dPlace    ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: //if(IO_GetX(xIDX_Detect)) {
                 //     EM_SetErrMsg(eiPRT_Crash, (m_sPartName + "CMOS 자재가 있습니다. 자재를 제거해주세요.").c_str());
                 //     Step.iCycle = 0 ;
                 //     return true ;
                 //}

                 if(MT_GetCmdPos(miIDX_XIndxR) == GetMotrPos(miIDX_XIndxR , piIDX_XIndxRWait)){
                     Step.iCycle = 20 ;
                     return false ;
                 }

                 if(AT_Complete(aiIDX_RoatorUpDn , ccBwd)){
                     MoveMotr(miIDX_XIndxR , piIDX_XIndxRWait);
                     Step.iCycle = 20 ;
                     return false ;
                 }

                 EM_SetErrMsg(eiPRT_Crash, "인덱스가 대기위치가 아닉 로테이터가 업 되어 있습니다.");
                 Step.iCycle = 0 ;
                 return true ;

        case 20: if(!MoveMotr(miIDX_XIndxR , piIDX_XIndxRWait)) return false ;
                 MoveActr(aiIDX_RoatorUpDn , ccFwd);
                 MoveMotr(miIDX_TRotat , piIDX_TRotatWait);
                 Step.iCycle++;
                 return false ;

        case 21: if(!MoveActr(aiIDX_RoatorUpDn , ccFwd)) return false ;
                 if(!MoveMotr(miIDX_TRotat , piIDX_TRotatWait)) return false ;
                 MoveMotr(miIDX_XIndxR , piIDX_XIndxRRotator);
                 Step.iCycle++;
                 return false ;

        case 22: if(!MoveMotr(miIDX_XIndxR , piIDX_XIndxRRotator)) return false ;
                 IO_SetY(yIDX_RttPumpOn , false);
                 Step.iCycle++;
                 return false ;

        case 23:
                 MoveActr(aiIDX_RoatorUpDn, ccBwd);
                 Step.iCycle++;
                 return false ;

        case 24: if(!MoveActr(aiIDX_RoatorUpDn, ccBwd)) return false ;
                 IO_SetY(yIDX_IdxVac , true);


                 //Loader Position을 Empty로 움직여야 해서.
                 //포지션이 없어서 포지션 만들어서 사용.
                 //MoveMotr(miIDX_XIndxR , piIDX_XIndxRGetWait);
                 Step.iCycle++;
                 return false ;

        case 25: if(!MT_GetStopInpos(miLDR_ZElevR)) return false ;
                 if(!MT_GetStopInpos(miIDX_XIndxR)) return false ;

                 MoveMotr(miIDX_XIndxR , piIDX_XIndxRCheck1);

                 Step.iCycle++ ;
                 return false ;

        case 26: if(!MT_GetStopInpos(miIDX_XIndxR)) return false ;
                 MT_GoIncSlow(miIDX_XIndxR , PM.GetValue(miIDX_XIndxR , poIDX_XIndxRGet)+1.0);
                 MT_GoAbsRun(miLDR_YConvR , PM.GetValue(miLDR_YConvR , pvLDR_YConvRWork)-1.0);
                 Step.iCycle++;
                 return false ;

        case 27: if(!MT_GetStopInpos(miIDX_XIndxR)) return false ;
                 if(!MT_GetStopInpos(miLDR_YConvR)) return false ;

                 MT_GoIncRun(miLDR_ZElevR, GET_OFFSET_Z );
                 IO_SetY(yIDX_IdxVac , false);
                 Step.iCycle++;
                 return false ;

        case 28: if(!MT_GetStopInpos(miLDR_ZElevR)) return false ;
                 MT_GoAbsRun(miLDR_YConvR , PM.GetValue(miLDR_YConvR , pvLDR_YConvRWork));
                 MoveMotr(miIDX_XIndxR , piIDX_XIndxRGetWait);

        case 29: if(MoveMotr(miIDX_XIndxR , piIDX_XIndxRGetWait)) return false ;


                 DM.ARAY[riRTT  ].SetStat(csNone);
                 DM.ARAY[riLDR_R].SetStat(r,c,csWork);

                 Step.iCycle = 0;
                 return true ;

    }
}

//안씀.
bool CIndex::CycleRotator(void)//sss
{
/*
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
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

    int r ;
    EN_ARAY_ID riULD ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: MoveActr(aiIDX_RoatorUpDn, ccBwd);
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveActr(aiIDX_RoatorUpDn, ccBwd)) return false;
                 MoveMotr(miIDX_XIndxR, piIDX_XIndxRRotator);
                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(miIDX_XIndxR, piIDX_XIndxRRotator)) return false;
                 if(IO_GetX(xIDX_Detect)){
                     MoveActr(aiIDX_RoatorUpDn, ccFwd);
                     Step.iCycle = 15;
                     return false;
                 }
                 //Error ID 바뀜.
                 //EM_SetErrMsg(eiIDX_NotExistCMOS, (m_sPartName + "Index Transfer 부분에 CMOS 자재가 없습니다.").c_str());
                 Step.iCycle = 0;
                 return true ;

        case 15: if(!MoveActr(aiIDX_RoatorUpDn, ccFwd)) return false;
                 MoveMotr(miIDX_XIndxR, piIDX_TRotatWait);
                 Step.iCycle++;
                 return false;

        case 16: if(!MoveMotr(miIDX_XIndxR, piIDX_TRotatWait)) return false;
                 MoveMotr(miIDX_TRotat, piIDX_TRotatRotate);
                 Step.iCycle++;
                 return false;

        case 17: if(!MoveMotr(miIDX_TRotat, piIDX_TRotatRotate)) return false;
                 DM.ShiftArrayData(riRTT, riIDX);
                 Step.iCycle = 0;
                 return false;
    }
*/
}

//---------------------------------------------------------------------------
bool CIndex::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iMotr == miIDX_XIndxR){
        if(!MT_GetStop(miLDR_ZElevR)){
            sMsg = "리어 로더 Z축이 이동중입니다.";
            bRet = false;
        }
        else if(MT_GetCmdPos(miIDX_XIndxR) > PM.GetValue(miIDX_XIndxR,pvIDX_XIndxRRotator)){
            if(!AT_Complete(aiIDX_RoatorUpDn,ccBwd)){
                sMsg = "인덱스가 로테이션포지션보다 왼쪽에 있는데 로테이터가 상승중입니다.";
                bRet = false;
            }
        }
        else if(MT_GetCmdPos(miRTL_YTrspr) < (PM.GetValue(miRTL_YTrspr , pvRTL_YTrsfCheck)+100) && MT_GetCmdPos(miRTL_ZTrsfr) > PM.GetValue(miRTL_ZTrsfr , pvRTL_ZTrsfrWait)){
            sMsg = "트렌스퍼가 Wait포지션 보다 낮은 위치 입니다.";
            bRet = false;
        }
    }

    else if(_iMotr == miIDX_TRotat){
        if(MT_GetCmdPos(miIDX_XIndxR) != PM.GetValue(miIDX_XIndxR,pvIDX_XIndxRWait)){
            if(!AT_Complete(aiIDX_RoatorUpDn,ccBwd)){
                sMsg = "인덱스가 Wait위치가 아닙니다.";
                bRet = false;
            }
        }
    }

    else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        m_sCheckSafeMsg = sMsg ;
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iSeq)FM_MsgOk(MT_GetName(_iMotr),sMsg.c_str());
    }
    else {
        m_sCheckSafeMsg = "" ;
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CIndex::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

//    bool bPushUpDnFwd = AT_GetCmd(Add.aiPushUpDn) == ccFwd;
    if(_iActr == aiIDX_RoatorUpDn){
        if(_bFwd == ccBwd) {
            //if(IO_GetX(xIDX_Detect)){ sMsg = AnsiString("CMOS 자제 센서 감지 중!"); bRet = false ;}
        }
    }

    else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        m_sCheckSafeMsg = sMsg ;
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr),sMsg.c_str());
    }
    else {
        m_sCheckSafeMsg = "" ;
    }

    return bRet ;
}

bool CIndex::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow )
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(_bSlow) {
        return MT_GoAbsSlow(_iMotr , dPosition);
    }
    else {
        if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
        else            return MT_GoAbsMan(_iMotr , dPosition);
    }

    return false ;
}

void CIndex::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CIndex::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CIndex::SetLastCmd()
{
    //m_dLastIdxPos = MT_GetTrgPos(miIDX_TRotat);
    return ;

}

double CIndex::GetLastCmd(EN_MOTR_ID _iMotr)
{
    double dLastIdxPos = 0.0 ;
    if(!MT_GetAlarm(_iMotr) && !MT_GetNLimSnsr(_iMotr) && !MT_GetPLimSnsr(_iMotr) ) dLastIdxPos = MT_GetCmdPos(_iMotr) ;
    else                                                                            dLastIdxPos = GetMotrPos  (_iMotr , (EN_PSTN_ID)0) ;

    return dLastIdxPos ;
}

bool CIndex::CheckMoved()
{
//    if(m_dLastIdxPos != MT_GetCmdPos(miIDX_XIndxR)) return true  ; //이파트는 필요 없다.
//    else                                               return false ;

}

bool CIndex::CheckStop()
{
    if(!MT_GetStop(miIDX_XIndxR)) return false ;
    if(!MT_GetStop(miIDX_TRotat)) return false ;

    if(!AT_Done(aiIDX_RoatorUpDn)) return false ;

    return true ;
}

void CIndex::Load(bool _bLoad)
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





