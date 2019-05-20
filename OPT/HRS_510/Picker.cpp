//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Picker.h"
#include "Rail.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserINI.h"
#include "SPCUnit.h"
#include "Sorter.h"
#include "Sequence.h"
//---------------------------------------------------------------------------
//Rs232 Header Reference( CallBackReg Need )
//---------------------------------------------------------------------------

#include "SpcSubUnit.h"
//#include "FormMaster.h"
#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CPicker  PCK;

void CPicker::procComH(DWORD _cbInQue)
{
//Ex : OK,05,+0.001,+0.000,+0.009,+0.001,+0.001@@

// EX
//NG,03,+0.000,+0.050,+0.010,+0.001,+0.001@@
//OK,03,+0.001,+0.000,+0.009,+0.001,+0.001@@
//NG,03,+0.072,+0.000,+0.001,+0.001,+0.000@@
//NG,03,+0.000,+0.049,+0.001,+0.024,+0.001@@
//NG,03,+0.001,+0.051,+0.010,+0.001,+0.001@@

//    String sRcvMsg = "NG,05,+0.071,+0.048,+0.009,+0.025,+0.023@@";
//
//    //EX
//    01,01,+0002@@
//    x0501,01,-2005|x03@@\r\n
//    01,01,로 짜름.

    static AnsiString sRcvMsg = "" ;
    String sTemp ;
    double dTemp = 0.0 ;

    BYTE RcvBuff[300];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));
    PCK.Rs232_H->ReadData(_cbInQue, RcvBuff);
    sRcvMsg += (char*)RcvBuff ;

    Trace("procCom1",RcvBuff);

    if(!sRcvMsg.Pos(",03,") ) return ;
    if(!sRcvMsg.Pos("@@"  ) ) return ;

    Trace("procCom1 Total",sRcvMsg.c_str());
    sTemp = sRcvMsg.SubString(sRcvMsg.Pos(",03,")+ 4,2) + "." + sRcvMsg.SubString(sRcvMsg.Pos(",03,")+6,3);
    if(TryStrToFloat(sTemp , dTemp)) PCK.m_dPrbData[0] =  dTemp ;
    else                             PCK.m_dPrbData[0] = -9.999 ;

    sTemp = sRcvMsg.SubString(sRcvMsg.Pos(",03,")+10,2) + "." + sRcvMsg.SubString(sRcvMsg.Pos(",03,")+12,3);
    if(TryStrToFloat(sTemp , dTemp)) PCK.m_dPrbData[1] =  dTemp ;
    else                             PCK.m_dPrbData[1] = -9.999 ;

    sTemp = sRcvMsg.SubString(sRcvMsg.Pos(",03,")+16,2) + "." + sRcvMsg.SubString(sRcvMsg.Pos(",03,")+18,3);
    if(TryStrToFloat(sTemp , dTemp)) PCK.m_dPrbData[2] =  dTemp ;
    else                             PCK.m_dPrbData[2] = -9.999 ;


//    sTemp = sRcvMsg.SubString(sRcvMsg.Pos("01,01,")+6,5);
//    동도 이것이 소숫점을 안붙여서 오옴...아나.

    PCK.Rs232_H->sData = sRcvMsg ;

    sRcvMsg  = "" ;

//    FrmMaster->mmRs232->Lines->Add("Resut" + Rs232_L->sData);

}


EN_GRADE_STAT CPicker::GetBin(double _dVal)
{

             //-0.00500000012 이런 값이 들어 올때 있어서 인티져로 변환후 비교.
         if(int(OM.DevOptn.dMinBn[0] * 1000) <= int(_dVal* 1000) && int(_dVal* 1000) <= int(OM.DevOptn.dMaxBn[0]* 1000)) return gsGradeA ; //A
    else if(int(OM.DevOptn.dMinBn[1] * 1000) <= int(_dVal* 1000) && int(_dVal* 1000) <= int(OM.DevOptn.dMaxBn[1]* 1000)) return gsGradeB ; //B
    else if(int(OM.DevOptn.dMinBn[2] * 1000) <= int(_dVal* 1000) && int(_dVal* 1000) <= int(OM.DevOptn.dMaxBn[2]* 1000)) return gsGradeC ; //C
    else if(int(OM.DevOptn.dMinBn[3] * 1000) <= int(_dVal* 1000) && int(_dVal* 1000) <= int(OM.DevOptn.dMaxBn[3]* 1000)) return gsGradeD ; //D
    else if(int(OM.DevOptn.dMinBn[4] * 1000) <= int(_dVal* 1000) && int(_dVal* 1000) <= int(OM.DevOptn.dMaxBn[4]* 1000)) return gsGradeE ; //E
    else if(int(OM.DevOptn.dMinBn[5] * 1000) <= int(_dVal* 1000) && int(_dVal* 1000) <= int(OM.DevOptn.dMaxBn[5]* 1000)) return gsGradeF ; //F
    else if(int(OM.DevOptn.dMinBn[6] * 1000) <= int(_dVal* 1000) && int(_dVal* 1000) <= int(OM.DevOptn.dMaxBn[6]* 1000)) return gsGradeG ; //G
    else if(int(OM.DevOptn.dMinBn[7] * 1000) <= int(_dVal* 1000) && int(_dVal* 1000) <= int(OM.DevOptn.dMaxBn[7]* 1000)) return gsGradeH ; //H
    else if(int(OM.DevOptn.dMinBn[8] * 1000) <= int(_dVal* 1000) && int(_dVal* 1000) <= int(OM.DevOptn.dMaxBn[8]* 1000)) return gsGradeI ; //I
    else if(int(OM.DevOptn.dMinBn[9] * 1000) <= int(_dVal* 1000) && int(_dVal* 1000) <= int(OM.DevOptn.dMaxBn[9]* 1000)) return gsGradeJ ; //J

    Trace("Fail",AnsiString(_dVal).c_str() ) ;
    return gsFail   ; //Fail

}

int CPicker::GetGraphBin(double _dVal)
{
    const double iGap = 0.005 ;
    int    iCenterBin = MAX_VALUE_GRAPH_CNT / 2 ;
    double dBinMinVal = -(iGap * iCenterBin) ;

    for(int i = 0 ; i < MAX_VALUE_GRAPH_CNT ; i++) {
        if( dBinMinVal + iGap * i < _dVal && _dVal < dBinMinVal + iGap * (i+1)) return i ;
    }

    return -1 ;
}

CPicker::CPicker(void)
{
    Init();
}

CPicker::~CPicker (void)
{
    Close();
}



void CPicker::Init()
{
    m_sPartName = "Picker " ;

    Reset();

    Load(true);

    m_dWorkCycleTime = 0.0;

    Rs232_H = new TRS232C;

    if(!Rs232_H->Open(0)) ShowMessage("First  Rs232 Port Open Fail") ;
    else                  Rs232_H->CallBackReg(procComH);
}

void CPicker::Close()
{
    delete Rs232_H ;
    Load(false);
}

void CPicker::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CPicker::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();

}



bool CPicker::FindChip(EN_CHIP_STAT _iStat , int &r , int &c )
{

    bool bRet = RAL.FindChip(_iStat , r , c);

    return bRet ;
}


double CPicker::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos      = 0.0 ;
    double dSetPos   = 0.0 ; //세팅되어 있는 포지션, 트레이 오른쪽 밑과 픽커 오른쪽끝에가 일치 하는 포지션.
    double dStartPos = 0.0 ; //dSetPos를 트레이 왼쪽 뒤 와 픽커 왼쪽이 일치하는 포지션. 계산 하기 편하게 한번 바꿔줌. 계산 기준 위치임.
    double dPickPos  = 0.0 ; //실제 작업 해야 하는 포지션.
    double dPlacePos = 0.0 ; //페일 자제 트레이에 놓는 옵션 사용시에 적용.

    int r,c ;

    dSetPos   = PM.GetValue (_iMotr , pvPCK_YPickStart ) ;
    dStartPos = dSetPos ; //500에서 510은 축방향이 바뀜. dSetPos - (OM.DevInfo.iRowCnt - 1) * OM.DevInfo.dRowPitch ;

    FindChip(csUnkwn , r,c);
    dPickPos  = dStartPos + (DM.ARAY[riWRK].GetMaxRow() - 1 - r) * OM.DevInfo.dRowPitch ;

    FindChip(csEmpty ,r,c);
    dPlacePos = dStartPos + (DM.ARAY[riWRK].GetMaxRow() - 1 - r) * OM.DevInfo.dRowPitch ;

    if(_iMotr == miPCK_Y){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piPCK_YWait      : dPos = PM.GetValue (_iMotr , pvPCK_YWait      ); break ;
            case piPCK_YPickStart : dPos = PM.GetValue (_iMotr , pvPCK_YPickStart ); break ;
            case piPCK_YPlce      : dPos = PM.GetValue (_iMotr , pvPCK_YPlce      ); break ;
            case piPCK_YZig       : dPos = PM.GetValue (_iMotr , pvPCK_YZig       ); break ;
            case piPCK_YPick      : dPos = dPickPos ;                                break ;
            case piPCK_YPutDown   : dPos = dPlacePos ;                               break ;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;


}

//---------------------------------------------------------------------------
bool CPicker::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    if(_iMotr == miPCK_Y){
        if(!AT_Complete(aiPCK_PckrDU , ccBwd)) { sMsg = "Picker Up/Dn is Fwd" ; bRet = false ; }

        switch(_iPstnId) {
            default               :                                                                                                  break ;
            case piPCK_YWait      :                                                                                                  break ;
            case piPCK_YPickStart :                                                                                                  break ;
            case piPCK_YPlce      :  if(!AT_Complete(aiSRT_FlipFB , ccBwd)) { sMsg = "Sort Flipper Cylinder is Fwd" ; bRet = false;} break ;
            case piPCK_YZig       :                                                                                                  break ;
            case piPCK_YPick      :                                                                                                  break ;
        }
    }
    else {
        sMsg = MT_GetName(_iMotr) + " is Not this part Motr!" ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
    }

    return bRet ;

}

//---------------------------------------------------------------------------
bool CPicker::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool       bPlacePos   = MT_CmprPos(miPCK_Y , PM.GetValue(miPCK_Y , pvPCK_YPlce)) ;
    bool       bStnGuidBwd = AT_Complete(aiSTN_GuidFB , ccBwd);

    if(_iActr == aiPCK_PckrDU){
        if(!MT_GetStop(miPCK_Y)){ sMsg = MT_GetName(miPCK_Y) + " is Moving" ; bRet = false ; }

        if(_bFwd) {
            if(bPlacePos &&  !bStnGuidBwd) {
                 sMsg = AT_GetName(aiSTN_GuidFB) + " is not Bwd" ; bRet = false ;

            }
        }
    }
    else {
        sMsg = AT_GetName(_iActr) + " is Not this part Cylinder!" ;
        bRet =  false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}

bool CPicker::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CPicker::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPicker::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiPCK_HomeTO);
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

        case 10: AT_MoveCyl(aiPCK_PckrDU , ccBwd);
                 Step.iHome++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPCK_PckrDU , ccBwd))return false ;
                 MT_DoHome(miPCK_Y);
                 Step.iHome++ ;
                 return false ;


        case 12: if(!MT_GetHomeEnd(miPCK_Y))return false ;
                 MT_GoAbsMan(miPCK_Y , PM.GetValue(miPCK_Y , pvPCK_YWait  ));
                 Step.iHome++ ;
                 return false ;

        case 13: if(!MT_GoAbsMan(miPCK_Y , PM.GetValue(miPCK_Y , pvPCK_YWait  ))) return false ;
                 Step.iHome = 0 ;
                 return true ;
    }
}

bool CPicker::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10 ;

    //Ok.
    return true;

}

bool CPicker::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CPicker::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiPCK_ToStartTO);

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

        case 10: MoveActr(aiPCK_PckrDU , ccBwd);
                 //IO_SetY(yETC_VacuSol , true);
                 IO_SetY(yPRB_Start  , false);
                 IO_SetY(yPRB_Rezero , false);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CPicker::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiPCK_ToStopTO);

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

        case 10: MoveActr(aiPCK_PckrDU , ccBwd);

                 IO_SetY(yPRB_Start  , false);
                 IO_SetY(yPRB_Rezero , false);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false;
                 Step.iToStop = 0;
                 return true ;
    }
}

bool CPicker::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        if(EM_IsErr()) return false ;

        int r,c ;

        bool bStationEmpty = !IO_GetX(xSRT_StnDetect1) && !IO_GetX(xSRT_StnDetect2) && !IO_GetX(xSRT_StnDetect3)   ;

        DM.ARAY[riWRK].FindLastColFrstRow(csUnkwn ,r,c) ;
        bool isLastColEmpty = c < 3 && (DM.ARAY[riWRK].GetCntColStat(0,csEmpty) || DM.ARAY[riWRK].GetCntColStat(0,csMask));

        bool isCycleRezero = OM.CmnOptn.iRezeroTrayCnt && RAL.m_iTrayCnt >= OM.CmnOptn.iRezeroTrayCnt && DM.ARAY[riPCK].CheckAllStat(csNone) && DM.ARAY[riSTN].CheckAllStat(csNone);


        bool isCyclePick   = DM.ARAY[riWRK].GetCntStat  (csUnkwn) && DM.ARAY[riPCK].CheckAllStat(csNone) &&  (!OM.CmnOptn.iRezeroTrayCnt || (OM.CmnOptn.iRezeroTrayCnt && RAL.m_iTrayCnt < OM.CmnOptn.iRezeroTrayCnt)) &&
                            (RAL.CheckIdxPos(csUnkwn) || isLastColEmpty);

        bool isCycleCheck  = isCyclePick && OM.MstOptn.bCheckCycle ;

        bool isCyclePlace  =!DM.ARAY[riPCK].CheckAllStat(csNone ) && DM.ARAY[riSTN].CheckAllStat(csNone) && bStationEmpty;

        bool isConEnd      = DM.ARAY[riPCK].CheckAllStat(csNone ) ;

        //Normal Decide Step.
             if (isCycleRezero) {Trace(m_sPartName.c_str(),"CycleRezero Stt"); Step.iSeq = scRezero ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleCheck ) {Trace(m_sPartName.c_str(),"CycleCheck  Stt"); Step.iSeq = scCheck  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePick  ) {Trace(m_sPartName.c_str(),"CyclePick   Stt"); Step.iSeq = scPick   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePlace ) {Trace(m_sPartName.c_str(),"CyclePlace  Stt"); Step.iSeq = scPlace  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd     ) {Stat.bWorkEnd = true ; return true ;}

        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default     End");Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                       return false ;
        case scRezero: if(CycleRezero()){ Trace(m_sPartName.c_str(),"CycleRezero End");Step.iSeq = scIdle ;} return false ;
        case scCheck : if(CycleCheck ()){ Trace(m_sPartName.c_str(),"CycleCheck  End");Step.iSeq = scIdle ;} return false ;
        case scPick  : if(CyclePick  ()){ Trace(m_sPartName.c_str(),"CyclePick   End");Step.iSeq = scIdle ;} return false ;
        case scPlace : if(CyclePlace ()){ Trace(m_sPartName.c_str(),"CyclePlace  End");Step.iSeq = scIdle ;} return false ;

    }
}

bool CPicker::CyclePick(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 6000 )) {
        EM_SetErr(eiPCK_CycleTO);
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

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    bool r1 , r2 , r3 , r4 ;



    static double dPreTime = GetTickTime();
    double dCrntTime ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPCK_PckrDU , ccBwd);
                  dCrntTime = GetTickTime();
                  m_dWorkCycleTime = dCrntTime - dPreTime ;
                  dPreTime = dCrntTime ;
                  if(!DM.ARAY[riWRK].GetCntStat(csEmpty) && !DM.ARAY[riWRK].GetCntStat(csMask)) {
                      SPCS.ClearData();
                  }
                  //m_iPickCnt++;


                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YPick);

                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miPCK_Y , piPCK_YPick)) return false ;

                  //배큠에러시에 다시 시도 할때 떨어 트려서 없앰.
                  //IO_SetY(yPCK_Vacuum1 , false);
                  //IO_SetY(yPCK_Vacuum2 , false);
                  //IO_SetY(yPCK_Vacuum3 , false);

                  MoveActr(aiPCK_PckrDU , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;
                  FindChip(csUnkwn, m_iFindRow , m_iFindCol);



                  //여기다 픽커 선택 옵션 역은다.
                  IO_SetY(yPCK_Vacuum1 , DM.ARAY[riWRK].GetStat(m_iFindRow , m_iFindCol + RAL.m_iSortColOfs + 0) == csUnkwn && !OM.CmnOptn.bNotUseTool1);
                  IO_SetY(yPCK_Vacuum2 , DM.ARAY[riWRK].GetStat(m_iFindRow , m_iFindCol + RAL.m_iSortColOfs + 1) == csUnkwn && !OM.CmnOptn.bNotUseTool2);
                  IO_SetY(yPCK_Vacuum3 , DM.ARAY[riWRK].GetStat(m_iFindRow , m_iFindCol + RAL.m_iSortColOfs + 2) == csUnkwn && !OM.CmnOptn.bNotUseTool3);

                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  14: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;
                  MoveActr(aiPCK_PckrDU , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;

                  if((!IO_GetX(xPCK_Vacuum1) && IO_GetY(yPCK_Vacuum1 )) ||
                     (!IO_GetX(xPCK_Vacuum2) && IO_GetY(yPCK_Vacuum2 )) ||
                     (!IO_GetX(xPCK_Vacuum3) && IO_GetY(yPCK_Vacuum3 )) ) {
                      if(!OM.CmnOptn.bEmptyIgnr) {
                          EM_SetErr(eiPCK_Failed);
                          MoveActr(aiPCK_PckrDU , ccFwd);
                          Step.iCycle=20 ;
                          return false ;

                      }
                  }

                  if(DM.ARAY[riWRK].GetStat(m_iFindRow , m_iFindCol + RAL.m_iSortColOfs + 0) == csUnkwn )DM.ARAY[riWRK].SetStat(m_iFindRow , m_iFindCol + RAL.m_iSortColOfs + 0 ,csEmpty);
                  if(DM.ARAY[riWRK].GetStat(m_iFindRow , m_iFindCol + RAL.m_iSortColOfs + 1) == csUnkwn )DM.ARAY[riWRK].SetStat(m_iFindRow , m_iFindCol + RAL.m_iSortColOfs + 1 ,csEmpty);
                  if(DM.ARAY[riWRK].GetStat(m_iFindRow , m_iFindCol + RAL.m_iSortColOfs + 2) == csUnkwn )DM.ARAY[riWRK].SetStat(m_iFindRow , m_iFindCol + RAL.m_iSortColOfs + 2 ,csEmpty);

                  if(IO_GetX(xPCK_Vacuum1)) {DM.ARAY[riPCK].SetStat(0 , 0 , csUnkwn); }
                  if(IO_GetX(xPCK_Vacuum2)) {DM.ARAY[riPCK].SetStat(0 , 1 , csUnkwn); }
                  if(IO_GetX(xPCK_Vacuum3)) {DM.ARAY[riPCK].SetStat(0 , 2 , csUnkwn); }

                  //안들어 오면 배큠끈다.
                  IO_SetY(yPCK_Vacuum1 , IO_GetX(xPCK_Vacuum1));
                  IO_SetY(yPCK_Vacuum2 , IO_GetX(xPCK_Vacuum2));
                  IO_SetY(yPCK_Vacuum3 , IO_GetX(xPCK_Vacuum3));

                  if(!DM.ARAY[riPCK].CheckAllStat(csNone)){

                      if(!DM.ARAY[riSTN].CheckAllStat(csNone)) MoveMotr(miPCK_Y , piPCK_YZig );
                      else                                     MoveMotr(miPCK_Y , piPCK_YPlce);
                  }

                  Step.iCycle++;
                  return false ;

        case 16: if(!MT_GetStop(miPCK_Y)) return false ;

                  Step.iCycle=0 ;
                  return true  ;








        /*배큠에러시에 다시 내려 놓는 패턴.*/
        //15번에서 씀 스텝번호 바꿀때 유의.
        case 20:  if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;
                  IO_SetY(yPCK_Vacuum1 , false ) ;
                  IO_SetY(yPCK_Vacuum2 , false ) ;
                  IO_SetY(yPCK_Vacuum3 , false ) ;
                  Step.iCycle++;
                  return false ;

        case 21:  if(OM.CmnOptn.iPlaceEjtTime){
                      IO_SetY(yPCK_VacEjt1 , true );
                      IO_SetY(yPCK_VacEjt2 , true );
                      IO_SetY(yPCK_VacEjt3 , true );
                  }


                  m_tmTemp .Clear();
                  m_tmTemp2.Clear();
                  Step.iCycle++ ;
                  return false ;

        case 22:  if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceEjtTime)) return false ;
                  IO_SetY(yPCK_VacEjt1 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  IO_SetY(yPCK_VacEjt2 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  IO_SetY(yPCK_VacEjt3 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );

                  if(!m_tmTemp2.OnDelay(true , OM.CmnOptn.iPlaceDelay  )) return false ;

                  Step.iCycle++ ;
                  return false ;

        case 23:  MoveActr(aiPCK_PckrDU , ccBwd);
                  IO_SetY(yPCK_VacEjt1 , true );
                  IO_SetY(yPCK_VacEjt2 , true );
                  IO_SetY(yPCK_VacEjt3 , true );
                  Step.iCycle++;
                  return false ;

        case 24:  if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;

                  Step.iCycle= 0 ;
                  return true ;







    }
}

bool CPicker::CyclePlace(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 6000 )) {
        EM_SetErr(eiPCK_CycleTO);
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

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    bool r1 , r2 , r3 , r4 ;
    int iRslt ;
    String sSend ;
    double dTemp ;

    static double dCrntZeroPoint = 0.0 ;

    double dOffset = 0.0 ;
    double dValue  = 0.0 ;

    static bool   bChuckFail[MAX_PICKER_CNT] ; // 로터 후크 페일 센서...


    int iBinNo ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10:
                  for(int i = 0 ; i < MAX_PICKER_CNT ; i++) {
                     bChuckFail[i] = false ;
                  }

                  IO_SetY(yPRB_Start   , false);

                  MoveActr(aiPCK_PckrDU , ccBwd);
                  SRT.MoveActr(aiSTN_GuidFB , ccBwd) ;
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;
                  if(!SRT.MoveActr(aiSTN_GuidFB , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YPlce);

                  Step.iCycle++ ;
                  return false ;

        case  12: if(!IO_GetX(xPRB_Ready)) { //측정기 레디 시그널
                      EM_SetErr(eiPRB_NotReady) ;
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveMotr(miPCK_Y , piPCK_YPlce)) return false ;
                  MoveActr(aiPCK_PckrDU , ccFwd);
//                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  14: if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;
                  if(!OM.CmnOptn.bChuckFailIgnore){
//                      if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceChkTime)) return false ;
                      if(IO_GetX(xSRT_ChuckFail1)&& DM.ARAY[riPCK].GetStat(0,0) != csNone) bChuckFail[0] = true ;
                      if(IO_GetX(xSRT_ChuckFail2)&& DM.ARAY[riPCK].GetStat(0,1) != csNone) bChuckFail[1] = true ;
                      if(IO_GetX(xSRT_ChuckFail3)&& DM.ARAY[riPCK].GetStat(0,2) != csNone) bChuckFail[2] = true ;
                  }

                  IO_SetY(yPCK_Vacuum1 , false);
                  IO_SetY(yPCK_Vacuum2 , false);
                  IO_SetY(yPCK_Vacuum3 , false);


                  if(OM.CmnOptn.iPlaceEjtTime){
                      IO_SetY(yPCK_VacEjt1 , true );
                      IO_SetY(yPCK_VacEjt2 , true );
                      IO_SetY(yPCK_VacEjt3 , true );
                  }


                  m_tmTemp .Clear();
                  m_tmTemp2.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceEjtTime)) return false ;
                  IO_SetY(yPCK_VacEjt1 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  IO_SetY(yPCK_VacEjt2 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  IO_SetY(yPCK_VacEjt3 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );

                  if(!m_tmTemp2.OnDelay(true , OM.CmnOptn.iPlaceDelay  )) return false ;


                  if(!OM.MstOptn.bRandomSort) {
                      Rs232_H->sData = ""; //리시브 클리어 ... 데이터가 3개다 보니 플레그 처럼 쓰게 되었음.
                      IO_SetY(yPRB_Start , true) ; //동도는 아이오로 스타트.
                  }

                  Step.iCycle++ ;
                  return false ;

        case  16: if(!OM.MstOptn.bRandomSort && Rs232_H->sData == "") return false ;
                  if(!OM.MstOptn.bRandomSort && IO_GetX(xPRB_Ready) ) return false ;
                  IO_SetY(yPRB_Start   , false);
                  MoveActr(aiPCK_PckrDU , ccBwd);
                  IO_SetY(yPCK_VacEjt1 , true );
                  IO_SetY(yPCK_VacEjt2 , true );
                  IO_SetY(yPCK_VacEjt3 , true );

                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;

                  //미리 벌려 놓는다. 쩍!!
                  SRT.MoveActr(aiSTN_GuidFB , ccFwd);


                  IO_SetY(yPCK_VacEjt1 , false );
                  IO_SetY(yPCK_VacEjt2 , false );
                  IO_SetY(yPCK_VacEjt3 , false );

                  //일단 제끼럽 나우.
                  //if(OM.CmnOptn.bUseBinFullIgnore ){
                  //    dOffset = OM.DevOptn.dMasterOffset ;
                  //    dValue  = ((int)(StrToFloatDef(Rs232_H->sData,0.0)*1000 + 0.1)) / 1000.0;
                  //    iBinNo = SRT.GetEmptyBin((EN_CHIP_STAT)GetBin(dValue) ) ;
                  //    if(iBinNo < 0 ) {
                  //        Step.iCycle = 20;
                  //        return false ;
                  //    }
                  //}


                  if(!OM.MstOptn.bRandomSort) {


                      for(int i = 0 ; i < MAX_PICKER_CNT ; i++) {
                          dOffset = OM.DevOptn.dMasterOffset[i] ;
                          dValue  = m_dPrbData[i] ;

                          //딱 빈 경계값에 걸리면 Fail로 떨군다. 그래서 +값이면 쪼금 올려주고 -값이면 조금 내려준다.
                          //if(m_dPrbData[i] < 0) dValue  = ((int)(m_dPrbData[i]*1000 - 0.1)) / 1000.0;
                          //else                  dValue  = ((int)(m_dPrbData[i]*1000 + 0.1)) / 1000.0;


                          if(DM.ARAY[riPCK].GetStat(0,i) != csNone) dRotorH[m_iFindRow][m_iFindCol + RAL.m_iSortColOfs + i] = dValue + dOffset ;
                          else                                      dRotorH[m_iFindRow][m_iFindCol + RAL.m_iSortColOfs + i] = -9.999           ;

                          if(DM.ARAY[riPCK].GetStat(0,i) == csNone) continue ;

                          if(bChuckFail[i] ) DM.ARAY[riSTN].SetStat(0,i,(EN_CHIP_STAT)gsCFail);
                          else               DM.ARAY[riSTN].SetStat(0,i,(EN_CHIP_STAT)GetBin(dRotorH[m_iFindRow][m_iFindCol + RAL.m_iSortColOfs + i]));

                          if(GetBin     (dRotorH[m_iFindRow][m_iFindCol + RAL.m_iSortColOfs + i]) != gsFail ) m_iBinGraphCnt[(int)GetBin(dRotorH[m_iFindRow][m_iFindCol + RAL.m_iSortColOfs + i])]++;
                          if(GetGraphBin(dRotorH[m_iFindRow][m_iFindCol + RAL.m_iSortColOfs + i]) >  -1     ) m_iValGraphCnt[GetGraphBin(dRotorH[m_iFindRow][m_iFindCol + RAL.m_iSortColOfs + i])]++;

                          if(OM.MstOptn.bDebugMode){
                              SPCS.WritePlaceDataCsv(dRotorH[m_iFindRow][m_iFindCol + RAL.m_iSortColOfs + i]);
                          }

                          SPC.WriteHeightData(dRotorH[m_iFindRow][m_iFindCol + RAL.m_iSortColOfs + i]);
                      }
                  }
                  else {
                      for(int i = 0 ; i < MAX_PICKER_CNT ; i++) {  //없는 픽커들... 고려    여기부터
                          if(DM.ARAY[riPCK].GetStat(0,i) == csNone) continue ;
                          iRslt = rand()%6 ;
                               if(iRslt == 0) DM.ARAY[riSTN].SetStat(0,i,(EN_CHIP_STAT)gsFail  );
                          else if(iRslt == 1) DM.ARAY[riSTN].SetStat(0,i,(EN_CHIP_STAT)gsGradeA);
                          else if(iRslt == 2) DM.ARAY[riSTN].SetStat(0,i,(EN_CHIP_STAT)gsGradeB);
                          else if(iRslt == 3) DM.ARAY[riSTN].SetStat(0,i,(EN_CHIP_STAT)gsGradeC);
                          else if(iRslt == 4) DM.ARAY[riSTN].SetStat(0,i,(EN_CHIP_STAT)gsGradeD);
                          else if(iRslt == 5) DM.ARAY[riSTN].SetStat(0,i,(EN_CHIP_STAT)gsGradeE);

                      }
                  }

                  DM.ARAY[riPCK].SetStat(csNone);

                  //TODO : 자재가 없엇다면 ? dd
                  if(DM.ARAY[riWRK].GetCntStat(csUnkwn)) MoveMotr(miPCK_Y , piPCK_YPick     );
                  else                                   MoveMotr(miPCK_Y , piPCK_YPickStart);

                  Step.iCycle++;
                  return false ;

        case  18: if(DM.ARAY[riWRK].GetCntStat(csUnkwn)) {if(!MoveMotr(miPCK_Y , piPCK_YPick     )) return false ; }
                  else                                   {if(!MoveMotr(miPCK_Y , piPCK_YPickStart)) return false ; }



                  Step.iCycle=0 ;
                  return true  ;



        //18번에서 사용 스텝 바꿀때 주의.
        /*  일단 제끼럽 나우.
        case  20:
                  IO_SetY(yPCK_Vacuum , false);
                  MoveActr(aiPCK_PckrDU , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  21: if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;
                  IO_SetY(yPCK_Vacuum , true);
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  22: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;
                  MoveActr(aiPCK_PckrDU , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  23: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;
                  MoveMotr(miPCK_Y,piPCK_YPutDown);
                  Step.iCycle++ ;
                  return false ;

        case  24: if(!MoveMotr(miPCK_Y,piPCK_YPutDown)) return false ;
                  MoveActr(aiPCK_PckrDU , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  25: if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;

                  IO_SetY(yPCK_Vacuum , false);
                  if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  m_tmTemp.Clear();
                  m_tmTemp2.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  26: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceEjtTime)) return false ;
                  IO_SetY(yPCK_VacEjt , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  if(!m_tmTemp2.OnDelay(true , OM.CmnOptn.iPlaceDelay  )) return false ;
                  //IO_SetY(yPCK_VacEjt , false );

                  MoveActr(aiPCK_PckrDU , ccBwd);
                  IO_SetY(yPCK_VacEjt , true );
                  Step.iCycle++;
                  return false ;

        case  27: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;
                  IO_SetY(yPCK_VacEjt , false );
                  FindChip(csEmpty,r,c);
                  DM.ARAY[riWRK].SetStat(r,c,csMask);
                  DM.ARAY[riPCK].SetStat(csNone);
                  Step.iCycle = 0;
                  return true ;
        */
    }
}


bool CPicker::CycleRezeroCheck(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 6000 )) {
        EM_SetErr(eiPCK_CycleTO);
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

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    bool r1 , r2 , r3 , r4 ;
    int iRslt ;
    String sSend ;
    double dTemp ;


    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPCK_PckrDU , ccBwd);
                  MoveActr(aiSRT_FlipFB , ccBwd);
                  IO_SetY(yPRB_Start , false);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;
                  if(!MoveActr(aiSRT_FlipFB , ccBwd)) return false ;
                  IO_SetY(ySRT_StnAir , true);


                  if(!IO_GetX(xPRB_Ready)) { //측정기 레디 시그널
                      EM_SetErr(eiPRB_NotReady) ;
                      Step.iCycle = 0 ;
                      return true ;
                  }





                  MoveMotr(miPCK_Y , piPCK_YZig);
                  SRT.MoveActr(aiSTN_GuidFB , ccBwd);

                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miPCK_Y , piPCK_YZig)) return false ;

                  if(!SRT.MoveActr(aiSTN_GuidFB , ccBwd)) return false ;

                  MoveActr(aiPCK_PckrDU , ccFwd);
                  IO_SetY(yPCK_Vacuum1 , false);
                  IO_SetY(yPCK_Vacuum2 , false);
                  IO_SetY(yPCK_Vacuum3 , false);

                  //IO_SetY(yPCK_Vacuum , false);
                  //IO_SetY(yPCK_VacEjt , true );
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  IO_SetY(yPCK_Vacuum1 , true);
                  IO_SetY(yPCK_Vacuum2 , true);
                  IO_SetY(yPCK_Vacuum3 , true);

                  Step.iCycle++ ;
                  return false ;

        case  14: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;
                  MoveActr(aiPCK_PckrDU , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YPlce);
                  Step.iCycle++ ;
                  return false ;

        case  16: if(!MoveMotr(miPCK_Y , piPCK_YPlce)) return false ;
                  IO_SetY(ySRT_StnAir , false );
                  MoveActr(aiPCK_PckrDU , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  IO_SetY(yPCK_Vacuum1 , false);
                  IO_SetY(yPCK_Vacuum2 , false);
                  IO_SetY(yPCK_Vacuum3 , false);

                  Step.iCycle++ ;
                  return false ;

        case  18: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;

                  MoveActr(aiPCK_PckrDU , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  19: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;
                  MoveActr(aiPCK_PckrDU , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  20: if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  21: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceDelay)) return false ;

                  m_tmTemp.Clear();
                  m_tmTemp2.Clear();

                  IO_SetY(yPCK_Vacuum1 , false);
                  IO_SetY(yPCK_Vacuum2 , false);
                  IO_SetY(yPCK_Vacuum3 , false);

                  if(OM.CmnOptn.iPlaceEjtTime){
                      IO_SetY(yPCK_VacEjt1 , true );
                      IO_SetY(yPCK_VacEjt2 , true );
                      IO_SetY(yPCK_VacEjt3 , true );
                  }


                  Step.iCycle++ ;
                  return false ;

        case  22: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceEjtTime)) return false ;
                  IO_SetY(yPCK_VacEjt1 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  IO_SetY(yPCK_VacEjt2 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  IO_SetY(yPCK_VacEjt3 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );


                  if(!m_tmTemp2.OnDelay(true , OM.CmnOptn.iPlaceDelay)) return false ;

                  m_tmTemp.Clear();

                  Step.iCycle++ ;
                  return false ;

        case  23: if(!m_tmTemp.OnDelay(true , 1000)) return false ; //리제로 때까지 안정화 시간.

                  IO_SetY(yPRB_Start , true);
                  Rs232_H->sData = ""; //리시브 클리어 ... 데이터가 3개다 보니 플레그 처럼 쓰게 되었음.

                  Step.iCycle++ ;
                  return false ;

        case  24: if(IO_GetX(xPRB_Ready) || Rs232_H->sData == "") return false ;

                  IO_SetY(yPRB_Start , false);

                  for(int i = 0 ; i < MAX_PICKER_CNT ; i++) {
                      m_dZeroPointCheck[i] = m_dPrbData[i] ; //무부하 상태의 값을 기억.
                  }

                  MoveActr(aiPCK_PckrDU , ccBwd);
                  IO_SetY(yPCK_Vacuum1 , true );
                  IO_SetY(yPCK_Vacuum2 , true );
                  IO_SetY(yPCK_Vacuum3 , true );

                  m_tmTemp.Clear();

                  Step.iCycle++;
                  return false ;

        case  25: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;
                  MoveActr(aiPCK_PckrDU , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  26: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YZig);

                  Step.iCycle++ ;
                  return false ;

        case  27: if(!MoveMotr(miPCK_Y , piPCK_YZig)) return false ;
                  MoveActr(aiPCK_PckrDU , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  28: if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;
                  m_tmTemp .Clear();
                  m_tmTemp2.Clear();
                  IO_SetY(yPCK_Vacuum1 , false);
                  IO_SetY(yPCK_Vacuum2 , false);
                  IO_SetY(yPCK_Vacuum3 , false);

                  IO_SetY(yPCK_VacEjt1 , true);
                  IO_SetY(yPCK_VacEjt2 , true);
                  IO_SetY(yPCK_VacEjt3 , true);


                  Step.iCycle++ ;
                  return false ;

        case  29: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceEjtTime)) return false ;
                  IO_SetY(yPCK_VacEjt1 , false );
                  IO_SetY(yPCK_VacEjt2 , false );
                  IO_SetY(yPCK_VacEjt3 , false );

                  if(!m_tmTemp2.OnDelay(true , OM.CmnOptn.iPlaceDelay  )) return false ;

                  MoveActr(aiPCK_PckrDU , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  30: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  31: if(!m_tmTemp.OnDelay(true , 500)) return false ;
                  //Rs232_H->sData = "" ;
                  //sSend = "ID01P" ; //현재값 전송.
                  //Rs232_H->SendData(sSend.Length(),sSend.c_str());
                  IO_SetY(yPRB_Start , true);
                  Rs232_H->sData = ""; //리시브 클리어 ... 데이터가 3개다 보니 플레그 처럼 쓰게 되었음.

                  Step.iCycle++ ;
                  return false ;

        case  32: if(IO_GetX(xPRB_Ready) || Rs232_H->sData == "") return false ;
                  IO_SetY(yPRB_Start , false);

                  //if(Rs232_H->sData == "") return false ;
                  for(int i = 0 ; i < MAX_PICKER_CNT ; i++) {
                      m_dZeroPoint[i] = m_dPrbData[i] ; //픽커를 들어서 무부하 상태의 값을 기억.
                  }
                  //m_dZeroPoint = StrToFloatDef(Rs232_H->sData,0.0) ; //무부하 상태의 값을 기억.
                  MoveMotr(miPCK_Y , piPCK_YWait);
                  Step.iCycle++;
                  return false ;

        case  33: //if(!MoveMotr(miPCK_Y , piPCK_YWait)) return false ;
//                  m_iPickCnt = 0 ;
                  //m_iPickCnt =0 ; //Rezero Cycle 타고 나서 Max카운트가 되면
                                  //다시 Rezero타는 현상 때문에 추가함..JSs
                  //RAL.m_iTrayCnt = 0;
                  Step.iCycle=0 ;
                  return true   ;
    }
}

bool CPicker::CycleRezero(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 6000 )) {
        EM_SetErr(eiPCK_CycleTO);
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

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    int r,c ;

    bool r1 , r2 , r3 , r4 ;
    int iRslt ;
    String sSend ;
    double dTemp ;


    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPCK_PckrDU , ccBwd);
                  MoveActr(aiSRT_FlipFB , ccBwd);

                  IO_SetY(yPRB_Rezero , false);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;
                  if(!MoveActr(aiSRT_FlipFB , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YZig);

                  IO_SetY(ySRT_StnAir , true);

                  SRT.MoveActr(aiSTN_GuidFB , ccBwd);

                  if(!IO_GetX(xPRB_Ready)) { //측정기 레디 시그널
                      EM_SetErr(eiPRB_NotReady) ;
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miPCK_Y , piPCK_YZig)) return false ;
                  if(!SRT.MoveActr(aiSTN_GuidFB , ccBwd)) return false ;
                  MoveActr(aiPCK_PckrDU , ccFwd);
                  IO_SetY(yPCK_Vacuum1 , false);
                  IO_SetY(yPCK_Vacuum2 , false);
                  IO_SetY(yPCK_Vacuum3 , false);

                  //IO_SetY(yPCK_Vacuum , false);
                  //IO_SetY(yPCK_VacEjt , true );
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  IO_SetY(yPCK_Vacuum1 , true);
                  IO_SetY(yPCK_Vacuum2 , true);
                  IO_SetY(yPCK_Vacuum3 , true);

                  Step.iCycle++ ;
                  return false ;

        case  14: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;
                  MoveActr(aiPCK_PckrDU , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YPlce);
                  Step.iCycle++ ;
                  return false ;

        case  16: if(!MoveMotr(miPCK_Y , piPCK_YPlce)) return false ;
                  IO_SetY(ySRT_StnAir , false);
                  MoveActr(aiPCK_PckrDU , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  IO_SetY(yPCK_Vacuum1 , false);
                  IO_SetY(yPCK_Vacuum2 , false);
                  IO_SetY(yPCK_Vacuum3 , false);

                  Step.iCycle++ ;
                  return false ;

        case  18: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;

                  MoveActr(aiPCK_PckrDU , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  19: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;
                  MoveActr(aiPCK_PckrDU , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  20: if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  21: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceDelay)) return false ;

                  m_tmTemp.Clear();
                  m_tmTemp2.Clear();
                  IO_SetY(yPCK_Vacuum1 , false);
                  IO_SetY(yPCK_Vacuum2 , false);
                  IO_SetY(yPCK_Vacuum3 , false);

                  if(OM.CmnOptn.iPlaceEjtTime){
                      IO_SetY(yPCK_VacEjt1 , true );
                      IO_SetY(yPCK_VacEjt2 , true );
                      IO_SetY(yPCK_VacEjt3 , true );
                  }

                  Step.iCycle++ ;
                  return false ;

        case  22: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceEjtTime)) return false ;
                  IO_SetY(yPCK_VacEjt1 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  IO_SetY(yPCK_VacEjt2 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  IO_SetY(yPCK_VacEjt3 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );


                  if(!m_tmTemp2.OnDelay(true , OM.CmnOptn.iPlaceDelay)) return false ;
                  //IO_SetY(yPCK_VacEjt , false );

                  //Rs232_H->sData = "" ; //으아 넌 어떻게 해도 이상하다. dd
                  //sSend = "ID01Z" ; //리제로.
                  //Rs232_H->SendData(sSend.Length(),sSend.c_str());
                  IO_SetY(yPRB_Rezero , true );

                  m_tmTemp.Clear();

                  Step.iCycle++ ;
                  return false ;

        case  23: if(!m_tmTemp.OnDelay(true , 1000) || !IO_GetX(xPRB_Ready)) return false ; //리제로 때까지 안정화 시간.
                  IO_SetY(yPRB_Rezero , false);
                  MoveActr(aiPCK_PckrDU , ccBwd);
                  IO_SetY(yPCK_Vacuum1 , true );
                  IO_SetY(yPCK_Vacuum2 , true );
                  IO_SetY(yPCK_Vacuum3 , true );

                  m_tmTemp.Clear();

                  Step.iCycle++;
                  return false ;

        case  24: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;
                  MoveActr(aiPCK_PckrDU , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  25: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YZig);

                  Step.iCycle++ ;
                  return false ;

        case  26: if(!MoveMotr(miPCK_Y , piPCK_YZig)) return false ;
                  MoveActr(aiPCK_PckrDU , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  27: if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;
                  m_tmTemp .Clear();
                  m_tmTemp2.Clear();
                  IO_SetY(yPCK_Vacuum1 , false);
                  IO_SetY(yPCK_Vacuum2 , false);
                  IO_SetY(yPCK_Vacuum3 , false);

                  IO_SetY(yPCK_VacEjt1 , true);
                  IO_SetY(yPCK_VacEjt2 , true);
                  IO_SetY(yPCK_VacEjt3 , true);


                  Step.iCycle++ ;
                  return false ;

        case  28: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceEjtTime)) return false ;
                  IO_SetY(yPCK_VacEjt1 , false );
                  IO_SetY(yPCK_VacEjt2 , false );
                  IO_SetY(yPCK_VacEjt3 , false );


                  if(!m_tmTemp2.OnDelay(true , OM.CmnOptn.iPlaceDelay  )) return false ;

                  MoveActr(aiPCK_PckrDU , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  29: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  30: if(!m_tmTemp.OnDelay(true , 500)) return false ;
                  Rs232_H->sData = "" ;
                  //sSend = "ID01P" ; //현재값 전송.
                  //Rs232_H->SendData(sSend.Length(),sSend.c_str());
                  IO_SetY(yPRB_Start , true);


                  Step.iCycle++ ;
                  return false ;

        case  31: if(Rs232_H->sData == "" || IO_GetX(xPRB_Ready)) return false ;
                  IO_SetY(yPRB_Start , false);
                  for(int i = 0 ; i < MAX_PICKER_CNT ; i++) {
                      m_dZeroPoint[i] = m_dPrbData[i] ; //무부하 상태의 값을 기억.
                  }

                  MoveMotr(miPCK_Y , piPCK_YWait);
                  Step.iCycle++;
                  return false ;

        case  32: //if(!MoveMotr(miPCK_Y , piPCK_YWait)) return false ;
                  //if(SEQ._bRun) m_iPickCnt = 0 ;
                  if(SEQ._bRun) RAL.m_iTrayCnt = 0 ;
                  Step.iCycle=0 ;
                  return true  ;
    }
}

bool CPicker::CycleCheck(void) //트레이에서 찝어다가 검사하고 다시 트레이에 가져다 놓음.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 6000 )) {
        EM_SetErr(eiPCK_CycleTO);
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

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    bool r1 , r2 , r3 , r4 ;
    String sSend ;
    double dTemp ;
    int iRslt ;
    double     dOffset = 0.0 ;
    double     dValue  = 0.0 ;

    static double dPreTime = GetTickTime();
    double dCrntTime ;
    const bool bUseUpDnCheck = true ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPCK_PckrDU , ccBwd);
                  IO_SetY(yPRB_Start , false) ; 
                  dCrntTime = GetTickTime();
                  m_dWorkCycleTime = dCrntTime - dPreTime ;
                  dPreTime = dCrntTime ;
                  if(!DM.ARAY[riWRK].GetCntStat(csEmpty) && !DM.ARAY[riWRK].GetCntStat(csMask)) {
                      SPCS.ClearData();
                  }

                  SRT.MoveActr(aiSTN_GuidFB , ccBwd);

                  //m_iPickCnt++;
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;
                  if(!SRT.MoveActr(aiSTN_GuidFB , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YPick);
                  IO_SetY(ySRT_StnAir , true);

                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miPCK_Y , piPCK_YPick)) return false ;
                  IO_SetY(yPCK_Vacuum1 , false);
                  IO_SetY(yPCK_Vacuum2 , false);
                  IO_SetY(yPCK_Vacuum3 , false);

                  MoveActr(aiPCK_PckrDU , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;
                  IO_SetY(yPCK_Vacuum1 , true );
                  IO_SetY(yPCK_Vacuum2 , true );
                  IO_SetY(yPCK_Vacuum3 , true );

                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  14: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;
                  MoveActr(aiPCK_PckrDU , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;

                  if((!IO_GetX(xPCK_Vacuum1) && IO_GetY(yPCK_Vacuum1 )) ||
                     (!IO_GetX(xPCK_Vacuum2) && IO_GetY(yPCK_Vacuum2 )) ||
                     (!IO_GetX(xPCK_Vacuum3) && IO_GetY(yPCK_Vacuum3 )) ) {
                       EM_SetErr(eiPCK_Failed);
                       Step.iCycle=50 ;
                       return false ;

                  }

                  DM.ARAY[riPCK].SetStat(csUnkwn);





                  Step.iCycle++;
                  return false ;





                  //DM.ARAY[riWRK].SetStat(iR,iC,csEmpty);
                  //DM.ARAY[riPCK].SetStat(csUnkwn);

                  //LT.AddDayInfoWorkChip(1);
                  //LT.AddLotInfoWorkChip(1);

        case  16: MoveActr(aiPCK_PckrDU , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YPlce);

                  Step.iCycle++ ;
                  return false ;

        case  18: if(!MoveMotr(miPCK_Y , piPCK_YPlce)) return false ;
                  IO_SetY(ySRT_StnAir , false);



                  /*   한번 내려놓고 다시잡기.
                  MoveActr(aiPCK_PckrDU , ccFwd);
                  //IO_SetY(yPCK_Vacuum , false);
                  //IO_SetY(yPCK_VacEjt , true );
                  Step.iCycle++ ;
                  return false ;

        case  19: if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  IO_SetY(yPCK_Vacuum , false);
                  Step.iCycle++ ;
                  return false ;

        case  20: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;

                  MoveActr(aiPCK_PckrDU , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  21: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ; */


                  MoveActr(aiPCK_PckrDU , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  19: if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;
                  IO_SetY(yPCK_Vacuum1 , false);
                  IO_SetY(yPCK_Vacuum2 , false);
                  IO_SetY(yPCK_Vacuum3 , false);

                  if(OM.CmnOptn.iPlaceEjtTime){
                      IO_SetY(yPCK_VacEjt1 , true );
                      IO_SetY(yPCK_VacEjt2 , true );
                      IO_SetY(yPCK_VacEjt3 , true );

                  }



                  m_tmTemp.Clear();
                  m_tmTemp2.Clear();
                  Step.iCycle++ ;
                  return false ;



        case  20: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceEjtTime)) return false ;
                  IO_SetY(yPCK_VacEjt1 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  IO_SetY(yPCK_VacEjt2 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  IO_SetY(yPCK_VacEjt3 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );


                  if(!m_tmTemp2.OnDelay(true , OM.CmnOptn.iPlaceDelay)) return false ;
                  //IO_SetY(yPCK_VacEjt , false );

                  if(!OM.MstOptn.bRandomSort) {
                      Rs232_H->sData = ""; //리시브 클리어 ... 데이터가 3개다 보니 플레그 처럼 쓰게 되었음.
                      IO_SetY(yPRB_Start , true) ; //동도는 아이오로 스타트.
                  }

                  Step.iCycle++ ;
                  return false ;

        case  21: if(!OM.MstOptn.bRandomSort && Rs232_H->sData == "") return false ;
                  if(!OM.MstOptn.bRandomSort && IO_GetX(xPRB_Ready) ) return false ;
                  IO_SetY(yPRB_Start , false) ; //동도는 아이오로 스타트.








                  /*
                  if(!OM.MstOptn.bRandomSort) {
                      Rs232_H->sData = "" ; //으아 넌 어떻게 해도 이상하다. dd
                      //sSend = "ID01R" ; //Hold 해제.          z
                      //Rs232_H->WriteData(sSend.Length(),sSend.c_str());
                      sSend = "ID01P" ; //현재값 전송.
                      Rs232_H->SendData(sSend.Length(),sSend.c_str());
                      //1TODO : 검사 시작.
                  }



                  Step.iCycle++ ;
                  return false ;

        case  21: if(!OM.MstOptn.bRandomSort && Rs232_H->sData == "" ) return false ;   //TODO :&& 데이타 들어왔는지 확인*/
                  IO_SetY(yPCK_Vacuum1 , true);
                  IO_SetY(yPCK_Vacuum2 , true);
                  IO_SetY(yPCK_Vacuum3 , true);


                  IO_SetY(yPCK_VacEjt1 , false);
                  IO_SetY(yPCK_VacEjt2 , false);
                  IO_SetY(yPCK_VacEjt3 , false);


                  MoveActr(aiPCK_PckrDU , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  22: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;

                  FindChip(csUnkwn, m_iFindRow , m_iFindCol);
                  for(int i = 0 ; i < MAX_PICKER_CNT ; i++) {
                      dOffset = OM.DevOptn.dMasterOffset[i] ;

                      dValue  = m_dPrbData[i] ;

                      //딱 빈 경계값에 걸리면 Fail로 떨군다. 그래서 +값이면 쪼금 올려주고 -값이면 조금 내려준다.
                      //if(m_dPrbData[i] < 0) dValue  = ((int)(m_dPrbData[i]*1000 - 0.1)) / 1000.0;
                      //else                  dValue  = ((int)(m_dPrbData[i]*1000 + 0.1)) / 1000.0;
                      //if(m_dPrbData[i] < 0) dValue  = ((int)(m_dPrbData[i]*1000 - 0.1)) / 1000.0;
                      //else                  dValue  = ((int)(m_dPrbData[i]*1000 + 0.1)) / 1000.0;




                      if(DM.ARAY[riPCK].GetStat(0,i) != csNone) dRotorH[m_iFindRow][m_iFindCol + RAL.m_iSortColOfs + i] = dValue + dOffset ;
                      else                                      dRotorH[m_iFindRow][m_iFindCol + RAL.m_iSortColOfs + i] = -9.999           ;
                      //                          if(DM.ARAY[riPCK].GetStat(0,i) != csNone) dRotorH[m_iFindRow][m_iFindCol + RAL.m_iSortColOfs + i] = dValue + dOffset ;
                      //    else                                                            dRotorH[m_iFindRow][m_iFindCol + RAL.m_iSortColOfs + i] = -9.999           ;

                      if(DM.ARAY[riPCK].GetStat(0,i) == csNone) continue ;
                      SPC.WriteHeightData(dRotorH[m_iFindRow][m_iFindCol + RAL.m_iSortColOfs + i]);
                  }

                  MoveMotr(miPCK_Y , piPCK_YPick );

                  Step.iCycle++;
                  return false ;

        case  23: if(!MoveMotr(miPCK_Y , piPCK_YPick )) return false ;
                  MoveActr(aiPCK_PckrDU , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  24: if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;
                  IO_SetY(yPCK_Vacuum1 , false);
                  IO_SetY(yPCK_Vacuum2 , false);
                  IO_SetY(yPCK_Vacuum3 , false);

                  if(OM.CmnOptn.iPlaceEjtTime){
                      IO_SetY(yPCK_VacEjt1 , true );
                      IO_SetY(yPCK_VacEjt2 , true );
                      IO_SetY(yPCK_VacEjt3 , true );
                  }



                  m_tmTemp.Clear();
                  m_tmTemp2.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  25: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceEjtTime)) return false ;
                  IO_SetY(yPCK_VacEjt1 , false );
                  IO_SetY(yPCK_VacEjt2 , false );
                  IO_SetY(yPCK_VacEjt3 , false );


                  if(!m_tmTemp2.OnDelay(true , OM.CmnOptn.iPlaceDelay)) return false ;
                  IO_SetY(yPCK_VacEjt1 , false );
                  IO_SetY(yPCK_VacEjt2 , false );
                  IO_SetY(yPCK_VacEjt3 , false );


                  MoveActr(aiPCK_PckrDU , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  26: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;
                  DM.ARAY[riPCK].SetStat(csNone);

                  FindChip(csUnkwn, m_iFindRow , m_iFindCol);

                  DM.ARAY[riWRK].SetStat(m_iFindRow , m_iFindCol + RAL.m_iSortColOfs + 0 ,csEmpty);
                  DM.ARAY[riWRK].SetStat(m_iFindRow , m_iFindCol + RAL.m_iSortColOfs + 1 ,csEmpty);
                  DM.ARAY[riWRK].SetStat(m_iFindRow , m_iFindCol + RAL.m_iSortColOfs + 2 ,csEmpty);

                  Step.iCycle=0 ;
                  return true  ;







        /*배큠에러시에 다시 내려 놓는 패턴.*/
        //15번에서 씀 스텝번호 바꿀때 유의.
        case 50:  if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;
                  IO_SetY(yPCK_Vacuum1 , false ) ;
                  IO_SetY(yPCK_Vacuum2 , false ) ;
                  IO_SetY(yPCK_Vacuum3 , false ) ;
                  Step.iCycle++;
                  return false ;

        case 51:  if(OM.CmnOptn.iPlaceEjtTime){
                      IO_SetY(yPCK_VacEjt1 , true );
                      IO_SetY(yPCK_VacEjt2 , true );
                      IO_SetY(yPCK_VacEjt3 , true );
                  }


                  m_tmTemp .Clear();
                  m_tmTemp2.Clear();
                  Step.iCycle++ ;
                  return false ;

        case 52:  if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceEjtTime)) return false ;
                  IO_SetY(yPCK_VacEjt1 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  IO_SetY(yPCK_VacEjt2 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  IO_SetY(yPCK_VacEjt3 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );

                  if(!m_tmTemp2.OnDelay(true , OM.CmnOptn.iPlaceDelay  )) return false ;

                  Step.iCycle++ ;
                  return false ;

        case 53:  MoveActr(aiPCK_PckrDU , ccBwd);
                  IO_SetY(yPCK_VacEjt1 , true );
                  IO_SetY(yPCK_VacEjt2 , true );
                  IO_SetY(yPCK_VacEjt3 , true );

                  Step.iCycle++;
                  return false ;

        case 54:  if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;
                  IO_SetY(yPCK_VacEjt1 , false );
                  IO_SetY(yPCK_VacEjt2 , false );
                  IO_SetY(yPCK_VacEjt3 , false );
                  Step.iCycle= 0 ;
                  return true ;
    }
}

bool CPicker::CycleStation()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 6000 )) {
        EM_SetErr(eiPCK_CycleTO);
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

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    int r,c ;

    bool r1 , r2 , r3 , r4 ;
    int iRslt ;
    String sSend ;
    double dTemp ;


    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPCK_PckrDU , ccBwd);
                  MoveActr(aiSRT_FlipFB , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;
                  if(!MoveActr(aiSRT_FlipFB , ccBwd)) return false ;

                  MoveMotr(miPCK_Y , piPCK_YPlce);
                  SRT.MoveActr(aiSTN_GuidFB , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miPCK_Y , piPCK_YPlce)) return false ;
                  if(!SRT.MoveActr(aiSTN_GuidFB , ccBwd)) return false ;

                  MoveActr(aiPCK_PckrDU , ccFwd);
                  IO_SetY(yPCK_Vacuum1 , false);
                  IO_SetY(yPCK_Vacuum2 , false);
                  IO_SetY(yPCK_Vacuum3 , false);


                  //IO_SetY(yPCK_Vacuum , false);
                  //IO_SetY(yPCK_VacEjt , true );
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  IO_SetY(yPCK_Vacuum1 , true);
                  IO_SetY(yPCK_Vacuum2 , true);
                  IO_SetY(yPCK_Vacuum3 , true);

                  Step.iCycle++ ;
                  return false ;

        case  14: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;
                  MoveActr(aiPCK_PckrDU , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  16: if(!m_tmTemp.OnDelay(true , 500)) return false ;
                  MoveActr(aiPCK_PckrDU , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiPCK_PckrDU , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  m_tmTemp2.Clear();
                  IO_SetY(yPCK_Vacuum1 , false);
                  IO_SetY(yPCK_Vacuum2 , false);
                  IO_SetY(yPCK_Vacuum3 , false);

                  if(OM.CmnOptn.iPlaceEjtTime){
                      IO_SetY(yPCK_VacEjt1 , true );
                      IO_SetY(yPCK_VacEjt2 , true );
                      IO_SetY(yPCK_VacEjt3 , true );
                  }
                  Step.iCycle++ ;
                  return false ;

        case  18: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceEjtTime)) return false ;
                  IO_SetY(yPCK_VacEjt1 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  IO_SetY(yPCK_VacEjt2 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  IO_SetY(yPCK_VacEjt3 , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );


                  if(!m_tmTemp2.OnDelay(true , OM.CmnOptn.iPlaceDelay)) return false ;
                  //IO_SetY(yPCK_VacEjt , false );

                  Rs232_H->sData = "" ; //으아 넌 어떻게 해도 이상하다. dd
                  //sSend = "ID01R" ; //Hold 해제.
                  //Rs232_H->WriteData(sSend.Length(),sSend.c_str());
                  //sSend = "ID01P" ; //현재값 전송.
                  //Rs232_H->SendData(sSend.Length(),sSend.c_str());
                  //1TODO : 검사 시작.

                  IO_SetY(yPRB_Start , true) ; //동도는 아이오로 스타트.

                  Step.iCycle++ ;
                  return false ;


        case  19: if(!OM.MstOptn.bRandomSort && Rs232_H->sData == "") return false ;
                  if(!OM.MstOptn.bRandomSort && IO_GetX(xPRB_Ready) ) return false ;
                  IO_SetY(yPRB_Start   , false);

                  MoveActr(aiPCK_PckrDU , ccBwd);
                  IO_SetY(yPCK_VacEjt1 , true );
                  IO_SetY(yPCK_VacEjt2 , true );
                  IO_SetY(yPCK_VacEjt3 , true );


                  Step.iCycle++;
                  return false ;

        case  20: if(!MoveActr(aiPCK_PckrDU , ccBwd)) return false ;
                  IO_SetY(yPCK_VacEjt1 , false );
                  IO_SetY(yPCK_VacEjt2 , false );
                  IO_SetY(yPCK_VacEjt3 , false );


                  MoveMotr(miPCK_Y , piPCK_YWait);
                  Step.iCycle++;
                  return false ;

        case  21: if(!MoveMotr(miPCK_Y , piPCK_YWait)) return false ;

                  Step.iCycle=0 ;
                  return true  ;
    }

}


void CPicker::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CPicker::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CPicker::CheckStop()
{
    if(!MT_GetStop(miPCK_Y)) return false ;

    if(!AT_Done(aiPCK_PckrDU )) return false ;

    return true ;
}

void CPicker::Load(bool IsLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() +".INI";

    //Load Device.
    Trace( "CPicker","`Load Start") ;

    int iTemp ;

    if(IsLoad) {
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_dZeroPoint" , m_dZeroPoint);
        //UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_iPickCnt  " , m_iPickCnt  );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sPreDate  " , m_sPreDate  );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_iFindRow  " , m_iFindRow  );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_iFindCol  " , m_iFindCol  );

        for(int i = 0 ; i < MAX_VALUE_GRAPH_CNT ; i++) {
            UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , AnsiString("m_iValGraphCnt")+i , iTemp  );
            m_iValGraphCnt[i] = iTemp ;
        }

        for(int i = 0 ; i < MAX_BIN_GRAPH_CNT ; i++) {
            UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , AnsiString("m_iBinGraphCnt")+i , iTemp  );
            m_iBinGraphCnt[i] = iTemp ;
        }
    }
    else {
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_dZeroPoint" , m_dZeroPoint);
        //UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_iPickCnt  " , m_iPickCnt  );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sPreDate  " , m_sPreDate  );

        for(int i = 0 ; i < MAX_VALUE_GRAPH_CNT ; i++) {
            iTemp = m_iValGraphCnt[i] ;
            UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , AnsiString("m_iValGraphCnt")+i , iTemp  );
        }

        for(int i = 0 ; i < MAX_BIN_GRAPH_CNT ; i++) {
            iTemp = m_iBinGraphCnt[i] ;
            UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , AnsiString("m_iBinGraphCnt")+i , iTemp  );
        }
    }
    Trace( "CPicker","Load End") ;
}
















