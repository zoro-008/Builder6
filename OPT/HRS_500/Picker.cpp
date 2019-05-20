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
#include "Rs232Man.h"
#include "SpcSubUnit.h"
//#include "FormMaster.h"
#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CPicker  PCK;
//TRS232C *Rs232_H;
/*
void CPicker::procCom1(DWORD _cbInQue)
{
    static AnsiString sRcvMsg = "" ;
    String sTemp ;
    int    iLen = 0 ;

    BYTE RcvBuff[300];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));

    iLen = Rs232_H->ReadData(_cbInQue, RcvBuff);

    sRcvMsg += (char*)RcvBuff ;
                          s
    if(!sRcvMsg.Pos("ID001,")) return ;
    if(!sRcvMsg.Pos("\r\n"  )) return ;

    sRcvMsg.Delete(sRcvMsg.Pos("ID001,"),6);
    sRcvMsg.Delete(sRcvMsg.Pos("\r\n"  ),4);

    Trace("procCom1",sRcvMsg.c_str());
    Rs232_H->sData = sRcvMsg ;

    //정지 시켜 놓아서 컨트롤러에서 보여주기용.
    //Hold 해도 높은값은 들어옴. 이용도가 아니겟지? dd
    sTemp = "ID01H";
    Rs232_H->WriteData(sTemp.Length(),sTemp.c_str());

//    FrmMaster->mmRs232->Lines->Add(Rs232_H->sData);

    sRcvMsg  = "" ;
//    Rs232_H->bComplete = true ;
}
*/

EN_GRADE_STAT CPicker::GetBin(double _dVal)
{
/*
         if(OM.DevOptn.dMinBn1 <= _dVal && _dVal <= OM.DevOptn.dMaxBn1) return csRslt1 ;
    else if(OM.DevOptn.dMinBn2 <= _dVal && _dVal <= OM.DevOptn.dMaxBn2) return csRslt2 ;
    else if(OM.DevOptn.dMinBn3 <= _dVal && _dVal <= OM.DevOptn.dMaxBn3) return csRslt3 ;
    else if(OM.DevOptn.dMinBn4 <= _dVal && _dVal <= OM.DevOptn.dMaxBn4) return csRslt4 ;
    else if(OM.DevOptn.dMinBn5 <= _dVal && _dVal <= OM.DevOptn.dMaxBn5) return csRslt5 ;
    else if(OM.DevOptn.dMinBn6 <= _dVal && _dVal <= OM.DevOptn.dMaxBn6) return csRslt6 ;
    else if(OM.DevOptn.dMinBn7 <= _dVal && _dVal <= OM.DevOptn.dMaxBn7) return csRslt7 ;
    else                                                                return csRslt0 ; //Fail
*/
//    int iRslt = -1 ;

         if(OM.DevOptn.dMinBn[0] <= _dVal && _dVal <= OM.DevOptn.dMaxBn[0]) return gsGradeA ; //A
    else if(OM.DevOptn.dMinBn[1] <= _dVal && _dVal <= OM.DevOptn.dMaxBn[1]) return gsGradeB ; //B
    else if(OM.DevOptn.dMinBn[2] <= _dVal && _dVal <= OM.DevOptn.dMaxBn[2]) return gsGradeC ; //C
    else if(OM.DevOptn.dMinBn[3] <= _dVal && _dVal <= OM.DevOptn.dMaxBn[3]) return gsGradeD ; //D
    else if(OM.DevOptn.dMinBn[4] <= _dVal && _dVal <= OM.DevOptn.dMaxBn[4]) return gsGradeE ; //E
    else if(OM.DevOptn.dMinBn[5] <= _dVal && _dVal <= OM.DevOptn.dMaxBn[5]) return gsGradeF ; //F
    else if(OM.DevOptn.dMinBn[6] <= _dVal && _dVal <= OM.DevOptn.dMaxBn[6]) return gsGradeG ; //G
    else if(OM.DevOptn.dMinBn[7] <= _dVal && _dVal <= OM.DevOptn.dMaxBn[7]) return gsGradeH ; //H
    else if(OM.DevOptn.dMinBn[8] <= _dVal && _dVal <= OM.DevOptn.dMaxBn[8]) return gsGradeI ; //I
    else if(OM.DevOptn.dMinBn[9] <= _dVal && _dVal <= OM.DevOptn.dMaxBn[9]) return gsGradeJ ; //J

    else                                                                    return gsFail   ; //Fail


/*
    //Fail
    if(iRslt == (int)gsFail       ) return csRslt0 ; // Fail

    //for(int i = 0 ; i < (int)gsGradeJ + 1 ; i++ )
    if(OM.DevOptn.iBn1Idx == iRslt) return csRslt1 ;
    if(OM.DevOptn.iBn2Idx == iRslt) return csRslt2 ;
    if(OM.DevOptn.iBn3Idx == iRslt) return csRslt3 ;
    if(OM.DevOptn.iBn4Idx == iRslt) return csRslt4 ;
    if(OM.DevOptn.iBn5Idx == iRslt) return csRslt5 ;
    if(OM.DevOptn.iBn6Idx == iRslt) return csRslt6 ;
    if(OM.DevOptn.iBn7Idx == iRslt) return csRslt7 ;
    if(OM.DevOptn.iBn8Idx == iRslt) return csRslt8 ;
    if(OM.DevOptn.iBn9Idx == iRslt) return csRslt9 ;
    if(OM.DevOptn.iBnAIdx == iRslt) return csRsltA ;

    return csRslt0 ; // Fail
*/
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

    iR = -1 ;
    iC = -1 ;

    m_dWorkCycleTime = 0.0;

//    Rs232_H = new TRS232C;

//    if(!Rs232_H->Open(0)) ShowMessage("First  Rs232 Port Open Fail") ;
//    else                  Rs232_H->CallBackReg(procCom1);
}

void CPicker::Close()
{
//    delete Rs232_H ;
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
    c = DM.ARAY[riWRK].FindLastCol(_iStat);

    //if(c % 2)DM.ARAY[riWRK].FindLastColFrstRow (csUnkwn , r , c) ;
    //else     DM.ARAY[riWRK].FindLastColRow     (csUnkwn , r , c) ;

    if(_iStat == csUnkwn) DM.ARAY[riWRK].FindLastColRow (_iStat , r , c) ;

    if(_iStat == csEmpty) DM.ARAY[riWRK].FindFrstColRow (_iStat , r , c) ;

    return (r > -1 && c > -1);
}


double CPicker::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos    = 0.0 ;
    double dPickPs = 0.0 ;
    double dPutPs  = 0.0 ;
    int r,c ;
    FindChip(csUnkwn , r,c);
//    int r1,c1 ;

    //위쪽에 있음.
//    dPickPs = PM.GetValue (_iMotr , pvPCK_YPickStart ) + (OM.DevInfo.dRowPitch * r) ;
    //아래쪽에 있으면.
//    dPickPs = PM.GetValue (_iMotr , pvPCK_YPickStart ) - (OM.DevInfo.dRowPitch * r) ;
    dPickPs = PM.GetValue (_iMotr , pvPCK_YPickStart ) - (OM.DevInfo.dRowPitch * (OM.DevInfo.iRowCnt - ( r + 1 ))) ;

    FindChip(csEmpty , r,c);
    dPutPs  = PM.GetValue (_iMotr , pvPCK_YPickStart ) - (OM.DevInfo.dRowPitch * (OM.DevInfo.iRowCnt - ( r + 1 ))) ;

    if(_iMotr == miPCK_Y){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piPCK_YWait      : dPos = PM.GetValue (_iMotr , pvPCK_YWait      ); break ;
            case piPCK_YPickStart : dPos = PM.GetValue (_iMotr , pvPCK_YPickStart ); break ;
            case piPCK_YPlce      : dPos = PM.GetValue (_iMotr , pvPCK_YPlce      ); break ;
            case piPCK_YZig       : dPos = PM.GetValue (_iMotr , pvPCK_YZig       ); break ;
            case piPCK_YPick      : dPos = dPickPs;                                  break ;
            case piPCK_YPutDown   : dPos = dPutPs ;                                  break ;

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
        if(!AT_Complete(aiPCK_UpDn , ccBwd)) { sMsg = "Picker Up/Dn is Fwd" ; bRet = false ; }
        switch(_iPstnId) {
            default               :  break ;
            case piPCK_YWait      :  break ;
            case piPCK_YPickStart :  break ;
            case piPCK_YPlce      :  break ;
            case piPCK_YZig       :  break ;
            case piPCK_YPick      :  break ;
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

    if(_iActr == aiPCK_UpDn){
        if(!MT_GetStop(miPCK_Y)){ sMsg = MT_GetName(miPCK_Y) + " is Moving" ; bRet = false ; }
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

        case 10: AT_MoveCyl(aiPCK_UpDn , ccBwd);
                 Step.iHome++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPCK_UpDn , ccBwd))return false ;
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

        case 10: MoveActr(aiPCK_UpDn , ccBwd);
                 IO_SetY(yETC_VacuSol , true);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false;
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

        case 10: 

                 Step.iToStop ++ ;
                 return false ;

        case 11: Step.iToStop = 0   ;
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
        DM.ARAY[riWRK].FindLastColFrstRow(csUnkwn ,r,c) ;
        bool isLastColEmpty = c == 0 && (DM.ARAY[riWRK].GetCntColStat(0,csEmpty) || DM.ARAY[riWRK].GetCntColStat(0,csMask));

        bool isCycleRezero = OM.CmnOptn.iRezeroPickCnt && m_iPickCnt >= OM.CmnOptn.iRezeroPickCnt && DM.ARAY[riPCK].CheckAllStat(csNone) && DM.ARAY[riSTN].CheckAllStat(csNone);


        bool isCyclePick   = DM.ARAY[riWRK].GetCntStat  (csUnkwn) && DM.ARAY[riPCK].CheckAllStat(csNone) &&  m_iPickCnt < OM.CmnOptn.iRezeroPickCnt &&
                            (RAL.CheckIdxPos() || isLastColEmpty);

        bool isCycleCheck  = isCyclePick && OM.MstOptn.bCheckCycle ;

        bool isCyclePlace  =!DM.ARAY[riPCK].CheckAllStat(csNone ) && DM.ARAY[riSTN].CheckAllStat(csNone) ;

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

    int r,c ;

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

        case  10: MoveActr(aiPCK_UpDn , ccBwd);
                  dCrntTime = GetTickTime();
                  m_dWorkCycleTime = dCrntTime - dPreTime ;
                  dPreTime = dCrntTime ;
                  if(!DM.ARAY[riWRK].GetCntStat(csEmpty) && !DM.ARAY[riWRK].GetCntStat(csMask)) {
                      SPCS.ClearData();
                  }
                  m_iPickCnt++;

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YPick);

                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miPCK_Y , piPCK_YPick)) return false ;
                  IO_SetY(yPCK_Vacuum , false);
                  MoveActr(aiPCK_UpDn , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiPCK_UpDn , ccFwd)) return false ;
                  IO_SetY(yPCK_Vacuum , true);
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  14: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;
                  MoveActr(aiPCK_UpDn , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  if(!IO_GetX(xPCK_Vacuum)) {
                      if(!OM.CmnOptn.bEmptyIgnr) {
                          EM_SetErr(eiPCK_Failed);
                      }
                      else {
                          FindChip(csUnkwn, iR,iC);
                          DM.ARAY[riWRK].SetStat(iR,iC,csEmpty);
                      }
                      IO_SetY(yPCK_Vacuum , false);
                      Step.iCycle=0 ;
                      return true ;
                  }

                  FindChip(csUnkwn,iR,iC);

                  DM.ARAY[riWRK].SetStat(iR,iC,csEmpty);
                  DM.ARAY[riPCK].SetStat(csUnkwn);

//                  LT.AddDayInfoWorkChip(1);
//                  LT.AddLotInfoWorkChip(1);


                  Step.iCycle=0 ;
                  return true  ;
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

    int r,c ;

    bool r1 , r2 , r3 , r4 ;
    int iRslt ;
    String sSend ;
    double dTemp ;

    static double dCrntZeroPoint = 0.0 ;

    double dOffset = 0.0 ;
    double dValue  = 0.0 ;

    static bool   bChuckFail = false ;
    int iBinNo ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: bChuckFail = false ;
                  MoveActr(aiPCK_UpDn , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YPlce);

                  Step.iCycle++ ;
                  return false ;

        case  12: Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveMotr(miPCK_Y , piPCK_YPlce)) return false ;
                  MoveActr(aiPCK_UpDn , ccFwd);
                  //IO_SetY(yPCK_Vacuum , false);
                  //IO_SetY(yPCK_VacEjt , true );
//                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  14: if(!MoveActr(aiPCK_UpDn , ccFwd)) return false ;
                  if(!OM.CmnOptn.bChuckFailIgnore){
//                      if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceChkTime)) return false ;
                      if(IO_GetX(xSRT_ChuckFail)) bChuckFail = true ;
                  }

                  IO_SetY(yPCK_Vacuum , false);
                  if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  m_tmTemp.Clear();
                  m_tmTemp2.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceEjtTime)) return false ;
                  IO_SetY(yPCK_VacEjt , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  if(!m_tmTemp2.OnDelay(true , OM.CmnOptn.iPlaceDelay  )) return false ;
                  //IO_SetY(yPCK_VacEjt , false );

                  if(!OM.MstOptn.bRandomSort) {
                      Rs232_H->sData = "" ; //으아 넌 어떻게 해도 이상하다. dd
                      //sSend = "ID01R" ; //Hold 해제.
                      //Rs232_H->WriteData(sSend.Length(),sSend.c_str());
                      sSend = "ID01P" ; //현재값 전송.
                      Rs232_H->SendData(sSend.Length(),sSend.c_str());
                  }

                  Step.iCycle++ ;
                  return false ;

        case  16: if(!OM.MstOptn.bRandomSort && Rs232_H->sData == "" /*TODO :&& 데이타 들어왔는지 확인*/) return false ;

                  MoveActr(aiPCK_UpDn , ccBwd);
                  IO_SetY(yPCK_VacEjt , true );
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  IO_SetY(yPCK_VacEjt , false );

                  if(OM.CmnOptn.bUseBinFullIgnore ){
                      dOffset = OM.CmnOptn.dMasterOffset ;
                      dValue  = ((int)(StrToFloatDef(Rs232_H->sData,0.0)*1000 + 0.1)) / 1000.0;
                      iBinNo = SRT.GetEmptyBin((EN_CHIP_STAT)GetBin(dValue) ) ;
                      if(iBinNo < 0 ) {
                          Step.iCycle = 20;
                          return false ;
                      }
                  }

                  DM.ARAY[riPCK].SetStat(csNone);
                  if(!OM.MstOptn.bRandomSort) {
                      dOffset = OM.CmnOptn.dMasterOffset ;
                      dValue  = ((int)(StrToFloatDef(Rs232_H->sData,0.0)*1000 + 0.1)) / 1000.0;
                      dRotorH[iR][iC] = dValue +  dOffset ;
                      if(bChuckFail) DM.ARAY[riSTN].SetStat((EN_CHIP_STAT)gsCFail);
                      else           DM.ARAY[riSTN].SetStat((EN_CHIP_STAT)GetBin(dRotorH[iR][iC]));
                      //DM.ARAY[riSTN].SetStat(GetBin(dRotorH[iR][iC]));
                      //GetBin(double _dVal); 데이터값 으로 리절트 세팅.

                      if(GetBin(dRotorH[iR][iC]) != gsFail   ) m_iBinGraphCnt[(int)GetBin(dRotorH[iR][iC])]++;
                      if(GetGraphBin(dRotorH[iR][iC]) > -1  ) m_iValGraphCnt[GetGraphBin(dRotorH[iR][iC])]++ ;


                      if(OM.MstOptn.bDebugMode)SPCS.WritePlaceDataCsv(dValue);
                      SPC.WriteHeightData(dValue +  dOffset);
                  }


                  else {

                      iRslt = rand()%6 ;

                           if(iRslt == 0) DM.ARAY[riSTN].SetStat((EN_CHIP_STAT)gsFail  );
                      else if(iRslt == 1) DM.ARAY[riSTN].SetStat((EN_CHIP_STAT)gsGradeA);
                      else if(iRslt == 2) DM.ARAY[riSTN].SetStat((EN_CHIP_STAT)gsGradeB);
                      else if(iRslt == 3) DM.ARAY[riSTN].SetStat((EN_CHIP_STAT)gsGradeC);
                      else if(iRslt == 4) DM.ARAY[riSTN].SetStat((EN_CHIP_STAT)gsGradeD);
                      else if(iRslt == 5) DM.ARAY[riSTN].SetStat((EN_CHIP_STAT)gsGradeE);

                  }

                  //TODO : 자재가 없엇다면 ? dd
                  if(DM.ARAY[riWRK].GetCntStat(csUnkwn)) MoveMotr(miPCK_Y , piPCK_YPick     );
                  else                                   MoveMotr(miPCK_Y , piPCK_YPickStart);

                  Step.iCycle++;
                  return false ;

//        case  17: if(!MT_GetStop(miPCK_Y)) return false ;
        case  18: if(DM.ARAY[riWRK].GetCntStat(csUnkwn)) {if(!MoveMotr(miPCK_Y , piPCK_YPick     )) return false ; }
                  else                                   {if(!MoveMotr(miPCK_Y , piPCK_YPickStart)) return false ; }

                  Step.iCycle=0 ;
                  return true  ;



        //17번에서 사용 스텝 바꿀때 주의.
        case  20:
                  IO_SetY(yPCK_Vacuum , false);
                  MoveActr(aiPCK_UpDn , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  21: if(!MoveActr(aiPCK_UpDn , ccFwd)) return false ;
                  IO_SetY(yPCK_Vacuum , true);
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  22: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;
                  MoveActr(aiPCK_UpDn , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  23: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  MoveMotr(miPCK_Y,piPCK_YPutDown);
                  Step.iCycle++ ;
                  return false ;

        case  24: if(!MoveMotr(miPCK_Y,piPCK_YPutDown)) return false ;
                  MoveActr(aiPCK_UpDn , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  25: if(!MoveActr(aiPCK_UpDn , ccFwd)) return false ;

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

                  MoveActr(aiPCK_UpDn , ccBwd);
                  IO_SetY(yPCK_VacEjt , true );
                  Step.iCycle++;
                  return false ;

        case  27: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  IO_SetY(yPCK_VacEjt , false );
                  FindChip(csEmpty,r,c);
                  DM.ARAY[riWRK].SetStat(r,c,csMask);
                  DM.ARAY[riPCK].SetStat(csNone);
                  Step.iCycle = 0;
                  return true ;

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

        case  10: MoveActr(aiPCK_UpDn , ccBwd);
                  MoveActr(aiSRT_Flip , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  if(!MoveActr(aiSRT_Flip , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YZig);

                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miPCK_Y , piPCK_YZig)) return false ;
                  MoveActr(aiPCK_UpDn , ccFwd);
                  IO_SetY(yPCK_Vacuum , false);
                  //IO_SetY(yPCK_Vacuum , false);
                  //IO_SetY(yPCK_VacEjt , true );
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiPCK_UpDn , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  IO_SetY(yPCK_Vacuum , true);
                  Step.iCycle++ ;
                  return false ;

        case  14: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;
                  MoveActr(aiPCK_UpDn , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YPlce);
                  Step.iCycle++ ;
                  return false ;

        case  16: if(!MoveMotr(miPCK_Y , piPCK_YPlce)) return false ;
                  MoveActr(aiPCK_UpDn , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiPCK_UpDn , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  IO_SetY(yPCK_Vacuum , false);
                  Step.iCycle++ ;
                  return false ;

        case  18: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;

                  MoveActr(aiPCK_UpDn , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  19: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  MoveActr(aiPCK_UpDn , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  20: if(!MoveActr(aiPCK_UpDn , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  21: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceDelay)) return false ;

                  m_tmTemp.Clear();
                  m_tmTemp2.Clear();
                  IO_SetY(yPCK_Vacuum , false);
                  if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  Step.iCycle++ ;
                  return false ;

        case  22: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceEjtTime)) return false ;
                  IO_SetY(yPCK_VacEjt , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );

                  if(!m_tmTemp2.OnDelay(true , OM.CmnOptn.iPlaceDelay)) return false ;
                  //IO_SetY(yPCK_VacEjt , false );

//                  Rs232_H->sData = "" ; //으아 넌 어떻게 해도 이상하다. dd
//                  sSend = "ID01Z" ; //리제로.
//                  Rs232_H->SendData(sSend.Length(),sSend.c_str());
                  m_tmTemp.Clear();

                  Step.iCycle++ ;
                  return false ;

        case  23: if(!m_tmTemp.OnDelay(true , 700)) return false ; //리제로 때까지 안정화 시간.
                  Rs232_H->sData = "" ;
                  sSend = "ID01P" ; //현재값 전송.
                  Rs232_H->SendData(sSend.Length(),sSend.c_str());

                  Step.iCycle++ ;
                  return false ;

        case  24: if(Rs232_H->sData == "") return false ;

                  m_dZeroPointCheck = StrToFloatDef(Rs232_H->sData,0.0) ; //무부하 상태의 값을 기억.




                  MoveActr(aiPCK_UpDn , ccBwd);
                  IO_SetY(yPCK_Vacuum , true );
                  m_tmTemp.Clear();

                  Step.iCycle++;
                  return false ;

        case  25: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;
                  MoveActr(aiPCK_UpDn , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  26: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YZig);

                  Step.iCycle++ ;
                  return false ;

        case  27: if(!MoveMotr(miPCK_Y , piPCK_YZig)) return false ;
                  MoveActr(aiPCK_UpDn , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  28: if(!MoveActr(aiPCK_UpDn , ccFwd)) return false ;
                  m_tmTemp .Clear();
                  m_tmTemp2.Clear();
                  IO_SetY(yPCK_Vacuum , false);
                  IO_SetY(yPCK_VacEjt , true);
                  Step.iCycle++ ;
                  return false ;

        case  29: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceEjtTime)) return false ;
                  IO_SetY(yPCK_VacEjt , false );
                  if(!m_tmTemp2.OnDelay(true , OM.CmnOptn.iPlaceDelay  )) return false ;

                  MoveActr(aiPCK_UpDn , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  30: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  31: if(!m_tmTemp.OnDelay(true , 500)) return false ;
                  Rs232_H->sData = "" ;
                  sSend = "ID01P" ; //현재값 전송.
                  Rs232_H->SendData(sSend.Length(),sSend.c_str());

                  Step.iCycle++ ;
                  return false ;

        case  32: if(Rs232_H->sData == "") return false ;

                  m_dZeroPoint = StrToFloatDef(Rs232_H->sData,0.0) ; //무부하 상태의 값을 기억.
                  MoveMotr(miPCK_Y , piPCK_YWait);
                  Step.iCycle++;
                  return false ;

        case  33: //if(!MoveMotr(miPCK_Y , piPCK_YWait)) return false ;
//                  m_iPickCnt = 0 ;
                  Step.iCycle=0 ;
                  return true  ;
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

        case  10: MoveActr(aiPCK_UpDn , ccBwd);
                  MoveActr(aiSRT_Flip , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  if(!MoveActr(aiSRT_Flip , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YZig);

                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miPCK_Y , piPCK_YZig)) return false ;
                  MoveActr(aiPCK_UpDn , ccFwd);
                  IO_SetY(yPCK_Vacuum , false);
                  //IO_SetY(yPCK_Vacuum , false);
                  //IO_SetY(yPCK_VacEjt , true );
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiPCK_UpDn , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  IO_SetY(yPCK_Vacuum , true);
                  Step.iCycle++ ;
                  return false ;

        case  14: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;
                  MoveActr(aiPCK_UpDn , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YPlce);
                  Step.iCycle++ ;
                  return false ;

        case  16: if(!MoveMotr(miPCK_Y , piPCK_YPlce)) return false ;
                  MoveActr(aiPCK_UpDn , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiPCK_UpDn , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  IO_SetY(yPCK_Vacuum , false);
                  Step.iCycle++ ;
                  return false ;

        case  18: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;

                  MoveActr(aiPCK_UpDn , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  19: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  MoveActr(aiPCK_UpDn , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  20: if(!MoveActr(aiPCK_UpDn , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  21: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceDelay)) return false ;

                  m_tmTemp.Clear();
                  m_tmTemp2.Clear();
                  IO_SetY(yPCK_Vacuum , false);
                  if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  Step.iCycle++ ;
                  return false ;

        case  22: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceEjtTime)) return false ;
                  IO_SetY(yPCK_VacEjt , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );

                  if(!m_tmTemp2.OnDelay(true , OM.CmnOptn.iPlaceDelay)) return false ;
                  //IO_SetY(yPCK_VacEjt , false );

                  Rs232_H->sData = "" ; //으아 넌 어떻게 해도 이상하다. dd
                  sSend = "ID01Z" ; //리제로.
                  Rs232_H->SendData(sSend.Length(),sSend.c_str());
                  m_tmTemp.Clear();

                  Step.iCycle++ ;
                  return false ;

        case  23: if(!m_tmTemp.OnDelay(true , 700)) return false ; //리제로 때까지 안정화 시간.

                  MoveActr(aiPCK_UpDn , ccBwd);
                  IO_SetY(yPCK_Vacuum , true );
                  m_tmTemp.Clear();

                  Step.iCycle++;
                  return false ;

        case  24: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;
                  MoveActr(aiPCK_UpDn , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  25: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YZig);

                  Step.iCycle++ ;
                  return false ;

        case  26: if(!MoveMotr(miPCK_Y , piPCK_YZig)) return false ;
                  MoveActr(aiPCK_UpDn , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  27: if(!MoveActr(aiPCK_UpDn , ccFwd)) return false ;
                  m_tmTemp .Clear();
                  m_tmTemp2.Clear();
                  IO_SetY(yPCK_Vacuum , false);
                  IO_SetY(yPCK_VacEjt , true);
                  Step.iCycle++ ;
                  return false ;

        case  28: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceEjtTime)) return false ;
                  IO_SetY(yPCK_VacEjt , false );
                  if(!m_tmTemp2.OnDelay(true , OM.CmnOptn.iPlaceDelay  )) return false ;

                  MoveActr(aiPCK_UpDn , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  29: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  30: if(!m_tmTemp.OnDelay(true , 500)) return false ;
                  Rs232_H->sData = "" ;
                  sSend = "ID01P" ; //현재값 전송.
                  Rs232_H->SendData(sSend.Length(),sSend.c_str());

                  Step.iCycle++ ;
                  return false ;

        case  31: if(Rs232_H->sData == "") return false ;

                  m_dZeroPoint = StrToFloatDef(Rs232_H->sData,0.0) ; //무부하 상태의 값을 기억.
                  MoveMotr(miPCK_Y , piPCK_YWait);
                  Step.iCycle++;
                  return false ;

        case  32: //if(!MoveMotr(miPCK_Y , piPCK_YWait)) return false ;
                  if(SEQ._bRun) m_iPickCnt = 0 ;
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

    int r,c ;

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

        case  10: MoveActr(aiPCK_UpDn , ccBwd);
                  dCrntTime = GetTickTime();
                  m_dWorkCycleTime = dCrntTime - dPreTime ;
                  dPreTime = dCrntTime ;
                  if(!DM.ARAY[riWRK].GetCntStat(csEmpty) && !DM.ARAY[riWRK].GetCntStat(csMask)) {
                      SPCS.ClearData();
                  }
                  m_iPickCnt++;
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YPick);

                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miPCK_Y , piPCK_YPick)) return false ;
                  IO_SetY(yPCK_Vacuum , false);
                  MoveActr(aiPCK_UpDn , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiPCK_UpDn , ccFwd)) return false ;
                  IO_SetY(yPCK_Vacuum , true );
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  14: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;
                  MoveActr(aiPCK_UpDn , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  if(!IO_GetX(xPCK_Vacuum)) {
                      if(!OM.CmnOptn.bEmptyIgnr) {
                          EM_SetErr(eiPCK_Failed);
                      }
                      else {
                          FindChip(csUnkwn,iR,iC);
                          DM.ARAY[riWRK].SetStat(iR,iC,csEmpty);
                      }
                      IO_SetY(yPCK_Vacuum , false);
                      Step.iCycle=0 ;
                      return true ;
                  }
                  FindChip(csUnkwn,iR,iC);
                  Step.iCycle++;
                  return false ;





                  //DM.ARAY[riWRK].SetStat(iR,iC,csEmpty);
                  //DM.ARAY[riPCK].SetStat(csUnkwn);

                  //LT.AddDayInfoWorkChip(1);
                  //LT.AddLotInfoWorkChip(1);

        case  16: MoveActr(aiPCK_UpDn , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YPlce);

                  Step.iCycle++ ;
                  return false ;

        case  18: if(!MoveMotr(miPCK_Y , piPCK_YPlce)) return false ;




                  /*   한번 내려놓고 다시잡기.
                  MoveActr(aiPCK_UpDn , ccFwd);
                  //IO_SetY(yPCK_Vacuum , false);
                  //IO_SetY(yPCK_VacEjt , true );
                  Step.iCycle++ ;
                  return false ;

        case  19: if(!MoveActr(aiPCK_UpDn , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  IO_SetY(yPCK_Vacuum , false);
                  Step.iCycle++ ;
                  return false ;

        case  20: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;

                  MoveActr(aiPCK_UpDn , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  21: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ; */


                  MoveActr(aiPCK_UpDn , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  19: if(!MoveActr(aiPCK_UpDn , ccFwd)) return false ;
                  IO_SetY(yPCK_Vacuum , false);
                  if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  m_tmTemp.Clear();
                  m_tmTemp2.Clear();
                  Step.iCycle++ ;
                  return false ;



        case  20: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceEjtTime)) return false ;
                  IO_SetY(yPCK_VacEjt , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );

                  if(!m_tmTemp2.OnDelay(true , OM.CmnOptn.iPlaceDelay)) return false ;
                  //IO_SetY(yPCK_VacEjt , false );

                  if(!OM.MstOptn.bRandomSort) {
                      Rs232_H->sData = "" ; //으아 넌 어떻게 해도 이상하다. dd
                      //sSend = "ID01R" ; //Hold 해제.
                      //Rs232_H->WriteData(sSend.Length(),sSend.c_str());
                      sSend = "ID01P" ; //현재값 전송.
                      Rs232_H->SendData(sSend.Length(),sSend.c_str());
                      //1TODO : 검사 시작.
                  }



                  Step.iCycle++ ;
                  return false ;

        case  21: if(!OM.MstOptn.bRandomSort && Rs232_H->sData == "" /*TODO :&& 데이타 들어왔는지 확인*/) return false ;
                  IO_SetY(yPCK_Vacuum , true);
                  IO_SetY(yPCK_VacEjt , false);
                  MoveActr(aiPCK_UpDn , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  22: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;

                  if(!OM.MstOptn.bRandomSort) {
                      dOffset = OM.CmnOptn.dMasterOffset ;
                      dValue  = ((int)(StrToFloatDef(Rs232_H->sData,0.0)*1000 + 0.1)) / 1000.0;
                      dRotorH[iR][iC] = dValue +  dOffset ;
                      SPC.WriteHeightData(dValue +  dOffset);
                  }
                  MoveMotr(miPCK_Y , piPCK_YPick );

                  Step.iCycle++;
                  return false ;

        case  23: if(!MoveMotr(miPCK_Y , piPCK_YPick )) return false ;
                  MoveActr(aiPCK_UpDn , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  24: if(!MoveActr(aiPCK_UpDn , ccFwd)) return false ;
                  IO_SetY(yPCK_Vacuum , false);
                  if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  m_tmTemp.Clear();
                  m_tmTemp2.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  25: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceEjtTime)) return false ;
                  IO_SetY(yPCK_VacEjt , false );
                  if(!m_tmTemp2.OnDelay(true , OM.CmnOptn.iPlaceDelay)) return false ;
                  IO_SetY(yPCK_VacEjt , false );
                  MoveActr(aiPCK_UpDn , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  26: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  FindChip(csUnkwn,iR,iC);
                  DM.ARAY[riWRK].SetStat(iR,iC,csEmpty);
                  Step.iCycle=0 ;
                  return true  ;
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

        case  10: MoveActr(aiPCK_UpDn , ccBwd);
                  MoveActr(aiSRT_Flip , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  if(!MoveActr(aiSRT_Flip , ccBwd)) return false ;
                  MoveMotr(miPCK_Y , piPCK_YPlce);

                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miPCK_Y , piPCK_YPlce)) return false ;
                  MoveActr(aiPCK_UpDn , ccFwd);
                  IO_SetY(yPCK_Vacuum , false);
                  //IO_SetY(yPCK_Vacuum , false);
                  //IO_SetY(yPCK_VacEjt , true );
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiPCK_UpDn , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  IO_SetY(yPCK_Vacuum , true);
                  Step.iCycle++ ;
                  return false ;

        case  14: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iVacDelay)) return false ;
                  MoveActr(aiPCK_UpDn , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  16: if(!m_tmTemp.OnDelay(true , 500)) return false ;
                  MoveActr(aiPCK_UpDn , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiPCK_UpDn , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  m_tmTemp2.Clear();
                  IO_SetY(yPCK_Vacuum , false);
                  if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );
                  Step.iCycle++ ;
                  return false ;

        case  18: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPlaceEjtTime)) return false ;
                  IO_SetY(yPCK_VacEjt , false ); //if(OM.CmnOptn.iPlaceEjtTime)IO_SetY(yPCK_VacEjt , true );

                  if(!m_tmTemp2.OnDelay(true , OM.CmnOptn.iPlaceDelay)) return false ;
                  //IO_SetY(yPCK_VacEjt , false );

                  Rs232_H->sData = "" ; //으아 넌 어떻게 해도 이상하다. dd
                  //sSend = "ID01R" ; //Hold 해제.
                  //Rs232_H->WriteData(sSend.Length(),sSend.c_str());
                  sSend = "ID01P" ; //현재값 전송.
                  Rs232_H->SendData(sSend.Length(),sSend.c_str());
                  //1TODO : 검사 시작.

                  Step.iCycle++ ;
                  return false ;

        case  19: if(!OM.MstOptn.bRandomSort && Rs232_H->sData == "" /*TODO :&& 데이타 들어왔는지 확인*/) return false ;

                  MoveActr(aiPCK_UpDn , ccBwd);
                  IO_SetY(yPCK_VacEjt , true );
                  Step.iCycle++;
                  return false ;

        case  20: if(!MoveActr(aiPCK_UpDn , ccBwd)) return false ;
                  IO_SetY(yPCK_VacEjt , false );

                  MoveMotr(miPCK_Y , piPCK_YWait);
                  Step.iCycle++;
                  return false ;

//        case  17: if(!MT_GetStop(miPCK_Y)) return false ;
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

    if(!AT_Done(aiPCK_UpDn )) return false ;

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
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "iR          " , iR          );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "iC          " , iC          );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_dZeroPoint" , m_dZeroPoint);
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_iPickCnt  " , m_iPickCnt  );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sPreDate  " , m_sPreDate  );

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
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "iR          " , iR          );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "iC          " , iC          );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_dZeroPoint" , m_dZeroPoint);
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_iPickCnt  " , m_iPickCnt  );
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
















