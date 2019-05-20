//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Rail.h"
#include "Loader.h"
#include "Picker.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "DataMan.h"
#include "SpcSubUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
CRail RAL;

CRail::CRail(void)
{
    Init();
}
CRail::~CRail (void)
{
    Close();
}

void CRail::Init()
{
    m_sPartName = "Rail " ;
    m_lStrpCycleTime = 0 ;
    m_lPreCycleTime  = 0 ;

    m_iSortColOfs = 0 ;

    Reset();
    Load(true);
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
}

void CRail::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

bool CRail::CheckIdxPos(EN_CHIP_STAT _iStat)
{
    double dWorkPos = 0.0 ;
    bool   bStop    = false ;

    if(_iStat == csUnkwn) dWorkPos = GetMotrPos(miRAL_X , piRAL_XWork   ) ;
    else                  dWorkPos = GetMotrPos(miRAL_X , piRAL_XPutDown) ;

    bStop = MT_GetStopInpos(miRAL_X);

    return MT_CmprPos(miRAL_X , dWorkPos) && bStop ;

}

bool CRail::FindChip(EN_CHIP_STAT _iStat , int &r , int &c )
{

    //픽커의 개수.
    int iPckCnt = MAX_PICKER_CNT ; //3 DM.ARAY[riPCK].GetMaxCol() ;
    //플레이스 가능 지역.
    bool bPlaceOk = false ;
    bool bPickOk  = false ;

    //Pick Cycle
    if(_iStat == csUnkwn) { //트레이 오른쪽 밑에서 부터 작업 시작.
        //픽커가 트레이 범위를 넘어가면 안됌.
        for(int cc = OM.DevInfo.iColCnt - 1 ; 0 <= cc ; cc--) {
            for(int rr = OM.DevInfo.iRowCnt - 1 ; 0 <= rr ; rr--) {
                //
                if(DM.ARAY[riWRK].GetStat(rr,cc) == csUnkwn) {
                    if(cc < iPckCnt - 1) {
                        m_iSortColOfs = - cc  ;  //m_iSortColOfs 여기서 세팅하고 싸이클에서 어레이 마스킹 할때 쓴다.
                    }
                    else {
                        m_iSortColOfs = -(iPckCnt -1) ;
                    }
                    c = cc;
                    r = rr;
                    //c = cc + m_iSortColOfs ;
                    //r = rr + m_iSortColOfs ;
                    return true ;
                }
            }
        }
        return false ;
    }

    //Place Cycle
    if(_iStat == csEmpty) {
        //픽커가 트레이 범위를 넘어가면 안됌.
        for(int cc = 0 ; cc < OM.DevInfo.iColCnt ; cc++) {
            for(int rr = 0 ; rr < OM.DevInfo.iRowCnt ; rr++) {
                bPlaceOk = true ;
                for(int pp = 0 ; pp < iPckCnt ; pp++) {
                    //픽커에 로터가 있고 내려놓을곳에 로터가 있으면 찾는것을 계속 한다.
                    if(DM.ARAY[riPCK].GetStat(0,pp) != csNone && DM.ARAY[riWRK].GetStat(rr,cc+pp) != csEmpty ) bPlaceOk = false ;
                }
                if(bPlaceOk) {
                    c = cc ;
                    r = rr ;
                    return true ;
                }
            }
        }
        return false ;
    }


}

double CRail::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos      = 0.0 ;
    double dSetPos   = 0.0 ; //세팅되어 있는 포지션, 트레이 오른쪽 밑과 픽커 오른쪽끝에가 일치 하는 포지션.
    double dStartPos = 0.0 ; //dSetPos를 트레이 왼쪽 뒤 와 픽커 왼쪽이 일치하는 포지션. 계산 하기 편하게 한번 바꿔줌. 계산 기준 위치임.
    double dPickPos  = 0.0 ; //실제 작업 해야 하는 포지션.
    double dPlacePos = 0.0 ; //페일 자제 트레이에 놓는 옵션 사용시에 적용.

    int PckR  , PckC ;
    int PlcR  , PlcC ;
    int iColCnt;
    FindChip(csUnkwn , PckR  , PckC);
    FindChip(csEmpty , PlcR  , PlcC);

    int iColShftCnt = OM.DevInfo.iColCnt - 1;

    //픽커의 개수.
    int iPckCnt = MAX_PICKER_CNT ; //DM.ARAY[riPCK].GetMaxCol() ;

    dSetPos   = PM.GetValue (miRAL_X , pvRAL_XWorkStart ) ;
                           //8                     6    32.????
    dStartPos = dSetPos + (iColShftCnt - (iPckCnt-1)) * OM.DevInfo.dColPitch ;
                          //((OM.DevInfo.iColCnt -1) + (iPckCnt - 1))

                //6         //Pos 8       -2
    dPickPos   = dStartPos - ( PckC + m_iSortColOfs) * OM.DevInfo.dColPitch ;
    dPlacePos  = dStartPos - ( PlcC + m_iSortColOfs) * OM.DevInfo.dColPitch ;


    //dWorkPos  = dStartPos + ((iColShftCnt + PckUnkw) + m_iSortColOfs) * OM.DevInfo.dColPitch ;

    //dPlacePos = dStartPos + ((iColShftCnt + PlcUnkw) + m_iSortColOfs) * OM.DevInfo.dColPitch ;

    if(_iMotr == miRAL_X){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piRAL_XWait      : dPos = PM.GetValue (_iMotr , pvRAL_XWait      ); break ;
            case piRAL_XGet       : dPos = PM.GetValue (_iMotr , pvRAL_XGet       ); break ;
            case piRAL_XWorkIn    : dPos = PM.GetValue (_iMotr , pvRAL_XWorkIn    ); break ;
            case piRAL_XWorkStart : dPos = PM.GetValue (_iMotr , pvRAL_XWorkStart ); break ;
            case piRAL_XOut       : dPos = PM.GetValue (_iMotr , pvRAL_XOut       ); break ;
            case piRAL_XWork      : dPos = dPickPos                                ; break ;
            case piRAL_XPutDown   : dPos = dPlacePos                               ; break ;

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

    bool isPickerDn = !AT_Complete(aiPCK_PckrDU , ccBwd);

    if(_iMotr == miRAL_X){
        if(isPickerDn) {sMsg = "During Picker Down"; bRet = false ;}
        switch(_iPstnId) {
            default                   :  break ;
            case piRAL_XWait          :  break ;
            case piRAL_XWork          :  break ;
            case piRAL_XWorkStart     :  break ;
        }
    }
    else {
        sMsg = MT_GetName(_iMotr) + " is Not this part Motr!" ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool  CRail::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iActr == aiRAL_IndxUD){
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

bool CRail::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
        EM_SetErr(eiRAL_HomeTO);
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
        default: if(Step.iHome) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iHome = 0 ;
                 return true ;

        case 10: AT_MoveCyl(aiRAL_IndxUD   , ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiRAL_IndxUD   , ccBwd)) return false ;
                 MT_DoHome(miRAL_X);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miRAL_X))return false ;
                 MT_GoAbsMan(miRAL_X , PM.GetValue(miRAL_X , pvRAL_XWait  ));
                 Step.iHome++;
                 return false ;


        case 13: if(!MT_GoAbsMan(miRAL_X , PM.GetValue(miRAL_X , pvRAL_XWait  ))) return false ;

                 Step.iHome = 0;
                 return true ;
    }
}

bool CRail::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //Clear Timer.
    Stat.bReqStop = true ;

    if (Step.iSeq ) return false;

    Step.iToStop = 10 ;

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
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiRAL_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart);
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

        case 10: if(DM.ARAY[riWRK].GetCntStat(csNone)) {
                     if(!MoveActr(aiRAL_IndxUD , ccBwd)) return false ;
                     Step.iToStart = 0 ;
                     return true ;
                 }

                 //sun
                 if(CheckIdxPos(csUnkwn)){
                     Step.iToStart = 0;
                     return true ;
                 }

                 MoveActr(aiRAL_IndxUD , ccBwd);
                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveActr(aiRAL_IndxUD , ccBwd)) return false ;
                 //MoveMotr(miRAL_X , piRAL_XWork);//m_dLastIndexPos
                 MT_GoAbsMan(miRAL_X , m_dLastIndexPos) ;


                 Step.iToStart ++;
                 return false ;

        case 12: if(!MT_GoAbsMan(miRAL_X , m_dLastIndexPos)) return false ;
                 MoveActr(aiRAL_IndxUD , ccFwd);
                 Step.iToStart ++;
                 return false ;

        case 13: if(!MoveActr(aiRAL_IndxUD , ccFwd)) return false ;
                 Step.iToStart = 0;
                 return true ;
    }
}

bool CRail::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiRAL_ToStopTO);

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

        case 10:
                 Step.iToStop ++ ;
                 return false ;

        case 11:
                 Step.iToStop ++ ;
                 return false ;

        case 12: m_dLastIndexPos = MT_GetCmdPos(miRAL_X) ;
                 Step.iToStop = 0   ;
                 return true ;
    }
}


bool CRail::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop ) return false ;

        //int r,c ;
        //DM.ARAY[riWRK].FindLastColFrstRow(csUnkwn ,r,c) ; // 여기가 픽해도 된다기엔 좀 부족해. dd
        bool isLastColWork = DM.ARAY[riWRK].GetCntColStat(0,csEmpty) ||  //c == 0 ;//&&
                             DM.ARAY[riWRK].GetCntColStat(0,csMask ) ;

        bool isWorkEnd     = OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt == (DM.ARAY[riWRK].GetCntStat(csEmpty) + DM.ARAY[riWRK].GetCntStat(csMask)) ;

        //대기 위치 까지 전에
        bool isCycleIn      = !DM.ARAY[riPRB].CheckAllStat (csNone ) && AT_Complete(aiLDR_SplyFB , ccBwd) &&
                               IO_GetX(xRAL_Detect2)                 && IO_GetX(xRAL_Detect1) &&
                             ( DM.ARAY[riWRK].CheckAllStat (csNone ) || isLastColWork );



        bool isPickNone = DM.ARAY[riPCK].CheckAllStat(csNone) ; 
        //스테이터 측정이 없어서 필요 없어짐.
        //if(OM.CmnOptn.bUseBinFullIgnore) isPickNone = DM.ARAY[riPCK].CheckAllStat(csNone) ;


        bool isCycleWorkStt = !DM.ARAY[riPRB].CheckAllStat(csNone ) &&
                              !DM.ARAY[riWRK].GetCntStat  (csUnkwn) &&
                               //DM.ARAY[riWKE].CheckAllStat(csNone ) &&  //<<<< //중간에 워크 트레이 잡아 뺐을때 홀드 결려서.
                               IO_GetX(xRAL_Detect2)                &&
                              !IO_GetX(xRAL_Detect1)                &&
                               DM.ARAY[riPCK].CheckAllStat(csNone)  ; //트레이 데이터 맨마지막께 9.999로 남음.

        bool isCycleWork    =  DM.ARAY[riWRK].GetCntStat  (csUnkwn) &&
                              !isLastColWork && !CheckIdxPos(csUnkwn) ;//&& isPickNone;

        bool isCycleOut     =  DM.ARAY[riPRB].CheckAllStat(csNone ) && AT_Complete(aiLDR_SplyFB , ccBwd) &&
                             (!DM.ARAY[riWRK].CheckAllStat(csNone ) && isWorkEnd) &&
                               DM.ARAY[riWKE].CheckAllStat(csNone ) &&
                               DM.ARAY[riPSB].CheckAllStat(csNone ) ;


        bool isConEnd       =  DM.ARAY[riPRB].CheckAllStat(csNone ) &&
                               DM.ARAY[riWRK].CheckAllStat(csNone ) &&
                               DM.ARAY[riWKE].CheckAllStat(csNone ) ;


/*
        bool isCycleWork   =  DM.ARAY[riWRK].GetCntStat  (csUnkwn) && !CheckIdxPos() ;
        bool isCycleIn     = !DM.ARAY[riPRB].CheckAllStat(csNone ) &&
                              DM.ARAY[riWRK].CheckAllStat(csNone ) ;
        bool isConEnd      =  DM.ARAY[riPRB].CheckAllStat(csNone ) &&
                              DM.ARAY[riWRK].CheckAllStat(csNone ) &&
                              DM.ARAY[riWKE].CheckAllStat(csNone ) ;
*/

        if(isCycleIn) {
            unsigned long lCrntTime = GetTickCount() ;
            m_lStrpCycleTime = lCrntTime - m_lPreCycleTime ;
            m_lPreCycleTime  = lCrntTime ;
        }

        //모르는 스트립에러.
        if(DM.ARAY[riPRB ].CheckAllStat(csNone)  && ( IO_GetX(xRAL_Detect1) &&  IO_GetX(xRAL_Detect2))
                                                              && LDR.GetSeqStep() != CLoader::scSupply
                                                              && LDR.GetSeqStep() != CLoader::scBfSply ) EM_SetErr(eiPRB_Unknwn ) ;
        if(DM.ARAY[riWRK ].CheckAllStat(csNone)  &&   IO_GetX(xRAL_Detect3)                           ) EM_SetErr(eiWRK_Unknwn ) ;

        //스트립 사라짐 에러.
        if(!DM.ARAY[riPRB ].CheckAllStat(csNone) && (!IO_GetX(xRAL_Detect1) && !IO_GetX(xRAL_Detect2))) EM_SetErr(eiPRB_Dispr  ) ;
        if(!DM.ARAY[riWRK ].CheckAllStat(csNone) &&  !IO_GetX(xRAL_Detect3)                           ) EM_SetErr(eiWRK_Dispr  ) ;

        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleIn     ) {Trace(m_sPartName.c_str(),"CycleIn      Stt"); Step.iSeq = scIn      ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWorkStt) {Trace(m_sPartName.c_str(),"CycleWorkStt Stt"); Step.iSeq = scWorkStt ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork   ) {Trace(m_sPartName.c_str(),"CycleWork    Stt"); Step.iSeq = scWork    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleOut    ) {Trace(m_sPartName.c_str(),"CycleOut     Stt"); Step.iSeq = scOut     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd      ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default         :                         Trace(m_sPartName.c_str(),"default End"  );Step.iSeq = scIdle ;  return false ;
        case  scIdle    :                                                                                          return false ;
        case  scIn      : if(CycleIn      ()){ Trace(m_sPartName.c_str(),"CycleIn      End");Step.iSeq = scIdle ;} return false ;
        case  scWorkStt : if(CycleWorkStt ()){ Trace(m_sPartName.c_str(),"CycleWorkStt End");Step.iSeq = scIdle ;} return false ;
        case  scWork    : if(CycleWork    ()){ Trace(m_sPartName.c_str(),"CycleWork    End");Step.iSeq = scIdle ;} return false ;
        case  scOut     : if(CycleOut     ()){ Trace(m_sPartName.c_str(),"CycleOut     End");Step.iSeq = scIdle ;} return false ;

    }
}

bool CRail::CycleIn(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
        EM_SetErr(eiRAL_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    int r1 , r2 , r3 , r4 ;

    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiRAL_IndxUD , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiRAL_IndxUD , ccBwd)) return false ;
                  MoveMotr(miRAL_X , piRAL_XGet);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miRAL_X , piRAL_XGet)) return false ;
                  MoveActr(aiRAL_IndxUD , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiRAL_IndxUD , ccFwd)) return false ;
                  MoveMotr(miRAL_X , piRAL_XWorkIn);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miRAL_X , piRAL_XWorkIn)) return false ;
//                  DM.ARAY[riPRB].SetStep(DM.ARAY[riPRB].GetStep() + 1);

//                  DM.ShiftArrayData(riPRB , riWRK);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CRail::CycleWorkStt(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiRAL_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    bool r1 , r2 , r3 , r4 ;
    String sPath ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiRAL_IndxUD , ccFwd);
                  m_iTrayCnt++; //채크 싸이클 일때는 CycleIn이 안타서 여기다 넣어줌.
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiRAL_IndxUD , ccFwd)) return false ;
                  MoveMotr(miRAL_X , piRAL_XWorkStart);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miRAL_X , piRAL_XWorkStart)) return false ;
//                  if(DM.ARAY[riWRK].CheckAllStat(csEmpty)) {
                  if(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt == (DM.ARAY[riWRK].GetCntStat(csEmpty) + DM.ARAY[riWRK].GetCntStat(csMask))){
                      LT.LotEnd(); //End Time
                      SPCS.WriteDataCsv();
                      LT.WriteLotLog();
                  }
                  LT.LotOpen("",OM.m_sCrntDev);
                  if(!DM.ARAY[riWRK].CheckAllStat(csNone)) DM.ShiftArrayData(riWRK,riWKE); //중간에 워크 트레이 잡아 뺐을때 홀드 결려서.
                  DM.ShiftArrayData(riPRB,riWRK);
                  Step.iCycle = 0 ;
                  return true ;
    }
}


bool CRail::CycleWork(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiRAL_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    bool r1 , r2 , r3 , r4 ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiRAL_IndxUD , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiRAL_IndxUD , ccFwd)) return false ;
                  MoveMotr(miRAL_X , piRAL_XWork);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miRAL_X , piRAL_XWork)) return false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CRail::CycleOut(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiRAL_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    bool r1 , r2 , r3 , r4 ;
    String sPath , sSend;
    TDateTime tTemp ;

    const bool bUseAuto = OM.MstOptn.bCheckCycle ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(bUseAuto) {
                      Step.iCycle = 20 ;
                      return false ;
                  }
                  MoveActr(aiRAL_IndxUD , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiRAL_IndxUD , ccBwd)) return false ;
                  MoveMotr(miRAL_X , piRAL_XWait);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miRAL_X , piRAL_XWait)) return false ;
                  MoveActr(aiRAL_IndxUD , ccFwd);

//                  DM.ShiftArrayData(riWRK , riWKE);

                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiRAL_IndxUD , ccFwd)) return false ;
//                  if(DM.ARAY[riWRK].CheckAllStat(csEmpty)) {

                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miRAL_X , piRAL_XOut )) return false ;

                  if(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt == (DM.ARAY[riWRK].GetCntStat(csEmpty) + DM.ARAY[riWRK].GetCntStat(csMask))){
                      LT.LotEnd();
                      SPCS.WriteDataCsv();
                      LT.WriteLotLog();
                  }
                  MoveMotr(miRAL_X , piRAL_XOut);
                  DM.ShiftArrayData(riWRK , riWKE);

                  MoveActr(aiRAL_IndxUD,ccBwd);
                  MoveMotr(miRAL_X,piRAL_XWait);
                  Step.iCycle = 0 ;
                  return true ;







        // USE AUTO
        case  20: if(!MoveActr(aiRAL_IndxUD , ccFwd)) return false ;

                  /*sun
                  if(!OM.MstOptn.bRandomSort) {
                      Rs232_H->sData = "" ; //으아 넌 어떻게 해도 이상하다. dd
                      //sSend = "ID01R" ; //Hold 해제.
                      //Rs232_H->WriteData(sSend.Length(),sSend.c_str());
                      sSend = "ID01P" ; //현재값 전송.
                      Rs232_H->SendData(sSend.Length(),sSend.c_str());
                      //1TODO : 검사 시작.
                  }*/

                  Step.iCycle++ ;
                  return false ;

        case  21: //sun
                  //if(!OM.MstOptn.bRandomSort && Rs232_H->sData == "" /*TODO :&& 데이타 들어왔는지 확인*/) return false ;
                  //g_dCrntZeroPoint = StrToFloatDef(Rs232_H->sData,0.0) ;


                  MoveMotr(miRAL_X,piRAL_XWait);
                  Step.iCycle++;
                  return false ;

        case  22: if(!MoveMotr(miRAL_X,piRAL_XWait)) return false ;
//                  if(DM.ARAY[riWRK].CheckAllStat(csEmpty)) {
                  if(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt == (DM.ARAY[riWRK].GetCntStat(csEmpty) + DM.ARAY[riWRK].GetCntStat(csMask))){
                      LT.LotEnd();
                      SPCS.WriteDataCsv();
                      LT.WriteLotLog();
                  }

                  DM.ARAY[riWRK].SetStat(csNone);
                  DM.ARAY[riPRB].SetStat(csUnkwn);
                  Step.iCycle++;
                  return false ;


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
    if(!MT_GetStop(miRAL_X)) return false ;

    if(!AT_Done(aiRAL_IndxUD )) return false ;

    return true ;
}

void CRail::Load(bool IsLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() +".INI";

    //Load Device.
    Trace( "CRail","Load Start") ;

    if(IsLoad) {
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_dLastIndexPos" , m_dLastIndexPos);
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_iTrayCnt     " , m_iTrayCnt     );
    }

    else {
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_dLastIndexPos" , m_dLastIndexPos);
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_iTrayCnt     " , m_iTrayCnt     );
    }
    Trace( "CRail","Load End") ;
}

bool CRail::GetLastStrip()
{
    return false;
}

