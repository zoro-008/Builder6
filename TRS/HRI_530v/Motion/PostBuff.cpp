//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PostBuff.h"
#include "Rail.h"
#include "Laser.h"
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
CPostBuff PSB;

CPostBuff::CPostBuff(void)
{

}

CPostBuff::~CPostBuff (void)
{

}

void CPostBuff::Init()
{
    m_sPartName = "PostBuff " ;
    Reset();
    Load(true);

    InitCycleName();
}
void CPostBuff::Close()
{
    Load(false);
}

void CPostBuff::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CPostBuff::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}



bool CPostBuff::FindChip(int &r , int &c )
{
    return false ;
    //c = DM.ARAY[riPSB].FindLastCol(csFail) ;
    //
    //return (c > -1 && c > -1);
}

double CPostBuff::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos     = 0.0 ;

    if(_iMotr == miPSB_XIndx){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                   ); break ;
            case piPSB_XIndxWait  : dPos = PM.GetValue (_iMotr , pvPSB_XIndxWait ); break ;
            case piPSB_XIndxRjct  : dPos = PM.GetValue (_iMotr , pvPSB_XIndxRjct ); break ;
            case piPSB_XIndxOut   : dPos = PM.GetValue (_iMotr , pvPSB_XIndxOut  ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CPostBuff::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    //인덱스 핑거 올라와 있는지.
    bool isIdxDn   = AT_Complete(aiPSB_IndxDnUp  , ccFwd) ;
    bool isPushrDn = AT_Complete(aiPSB_PshrDnUp  , ccFwd) ;

    //모터 -방향으로 가는지.
    bool isMotrNeg = MT_GetCmdPos(miPSB_XIndx) > GetMotrPos(miPSB_XIndx , _iPstnId) ;


    if(_iMotr == miPSB_XIndx){
        if(isMotrNeg && isIdxDn) {sMsg = "Index Pinger is Down"; bRet = false ;}
        switch(_iPstnId) {
            default              :  break ;
            case piPSB_XIndxWait :  break ;
            case piPSB_XIndxRjct :  break ;
            case piPSB_XIndxOut  :  break ;
        }
    }

    else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CPostBuff::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    bool isXMoving = !MT_GetStopInpos(miPSB_XIndx);

    bool bRet = true ;
    AnsiString sMsg ;

    if(aiPSB_RailOpCl){}
    else if(aiPSB_IndxDnUp){}
    else if(aiPSB_ElvtUpDn){}
    else if(aiPSB_PshrDnUp){}
    else if(aiPSB_PshrFwBw){}
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

bool CPostBuff::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

         if(_bSlow     ) return MT_GoAbsSlow(_iMotr , dPosition);
    else if(Step.iCycle) return MT_GoAbsRun (_iMotr , dPosition);
    else                 return MT_GoAbsMan (_iMotr , dPosition);
}

bool CPostBuff::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPostBuff::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_HomeTo , m_sPartName.c_str());//EM_SetErr(eiPSB_HomeTo);
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

        case 10: AT_MoveCyl(aiPSB_ElvtUpDn  ,ccBwd);
                 AT_MoveCyl(aiPSB_GrprGrRl  ,ccBwd);
                 AT_MoveCyl(aiPSB_IndxDnUp  ,ccBwd);
                 AT_MoveCyl(aiPSB_PshrDnUp  ,ccBwd);
                 AT_MoveCyl(aiPSB_PshrFwBw  ,ccBwd);
                 AT_MoveCyl(aiPSB_RailOpCl  ,ccBwd);

                 IO_SetY(yPSB_RjcBelt, false );

                 Step.iHome++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPSB_ElvtUpDn  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_GrprGrRl  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_IndxDnUp  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_PshrDnUp  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_PshrFwBw  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_RailOpCl  ,ccBwd)) return false ;


                 MT_SetHomeEnd(miPSB_XIndx , false);
                 MT_DoHome(miPSB_XIndx);
                 Step.iHome++ ;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miPSB_XIndx)) return false;
                 MT_GoAbsMan(miPSB_XIndx, PM.GetValue(miPSB_XIndx , pvPSB_XIndxWait));
                 Step.iHome++ ;
                 return false ;

        case 13: if(!MT_GoAbsMan(miPSB_XIndx, PM.GetValue(miPSB_XIndx , pvPSB_XIndxWait))) return false ;
                 Step.iHome = 0;
                 return true ;

    }
}



bool CPostBuff::Autorun(void) //오토런닝시에 계속 타는 함수.
{

    //Check Cycle Time Out.
    AnsiString sTemp ;
    //static CDelayTimer tmWait ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;


    // 사이클로 빠진다고 되있는데..사이클에 없어서 주석 해제 함...JH

    if (Step.iSeq == 0) {
        if(Stat.bReqStop)return false ;

        int  iFailCnt      = DM.ARAY[riPSB].GetCntStat  (csRslt1) +
                             DM.ARAY[riPSB].GetCntStat  (csRslt2) +
                             DM.ARAY[riPSB].GetCntStat  (csRslt3) +
                             DM.ARAY[riPSB].GetCntStat  (csRslt4) +
                             DM.ARAY[riPSB].GetCntStat  (csRslt5) +
                             DM.ARAY[riPSB].GetCntStat  (csRslt6) ;
                             // 잡이 바뀌고 첫 자제는 오퍼가 확인 하기 위해...

        bool isCycleIn     = !DM.ARAY[riVSN].CheckAllStat(csNone) && AT_GetCmd(aiRAL_VStpUpDn) == ccBwd ;
        bool isCycleReject = (m_sWorkedLotNo != DM.ARAY[riPSB].GetLotNo() && OM.CmnOptn.bFrstStrpOut && !DM.ARAY[riPSB].CheckAllStat(csNone)) || iFailCnt > OM.DevOptn.iMarkInspFailCnt ;
        bool isCycleOut    = !DM.ARAY[riPSB].CheckAllStat(csNone) && DM.ARAY[riULD].GetCntStat(csEmpty) && ULD.GetSeqStep() == CUnLoader :: scIdle ;
        bool isCycleEnd    =  DM.ARAY[riPSB].CheckAllStat(csNone) && RAL.GetWorkEnd();



        //Unknown Strip
        /*
        if ( IO_GetX(xPSB_Pkg5) && DM.ARAY[riPRB].CheckAllStat(csNone))
            EM_SetErrMsg(eiPKG_Unknwn , DM.ARAY[riPRB].GetName().c_str()) ;

        //Dissapear Strip   //kyekye
        if(!OM.MstOptn.bDryRun) {
            if (!IO_GetX(xPSB_Pkg5) &&!DM.ARAY[riPSB].CheckAllStat(csNone)) {
                EM_SetErrMsg(eiPKG_Dispr , DM.ARAY[riPSB].GetName().c_str()) ;
            }
        }
        */

        if(EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCycleIn    ) {Trace(m_sPartName.c_str(),"CycleIn     Stt"); Step.iSeq = scIn    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleReject) {Trace(m_sPartName.c_str(),"CycleReject Stt"); Step.iSeq = scReject; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleOut   ) {Trace(m_sPartName.c_str(),"CycleOut    Stt"); Step.iSeq = scOut   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default        :                    Trace(m_sPartName.c_str(),"default     End");Step.iSeq = scIdle ;  return false ;
        case scIdle    :                                                                                       return false ;
        case scIn      : if(CycleIn    ()){ Trace(m_sPartName.c_str(),"CycleIn     End");Step.iSeq = scIdle ;} return false ;
        case scReject  : if(CycleReject()){ Trace(m_sPartName.c_str(),"CycleReject End");Step.iSeq = scIdle ;} return false ;
        case scOut     : if(CycleOut   ()){ Trace(m_sPartName.c_str(),"CycleOut    End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;

}

bool CPostBuff::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;

    if (Step.iSeq) return false;

    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CPostBuff::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CPostBuff::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: AT_MoveCyl(aiPSB_ElvtUpDn  ,ccBwd);
                 AT_MoveCyl(aiPSB_GrprGrRl  ,ccBwd);
                 AT_MoveCyl(aiPSB_IndxDnUp  ,ccBwd);
                 AT_MoveCyl(aiPSB_PshrDnUp  ,ccBwd);
                 AT_MoveCyl(aiPSB_PshrFwBw  ,ccBwd);
                 AT_MoveCyl(aiPSB_RailOpCl  ,ccBwd);

                 IO_SetY(yPSB_RjcBelt, false );
                 Step.iToStart++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPSB_ElvtUpDn  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_GrprGrRl  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_IndxDnUp  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_PshrDnUp  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_PshrFwBw  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_RailOpCl  ,ccBwd)) return false ;

                 MoveMotr(miPSB_XIndx , piPSB_XIndxWait);
                 Step.iToStart++ ;
                 return false ;

        case 12: if(!MoveMotr(miPSB_XIndx , piPSB_XIndxWait)) return false;
                 Step.iToStart = 0;
                 return true ;
    }
}

bool CPostBuff::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() , 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str());//EM_SetErr(eiPSB_ToStopTO);

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

        case 10: AT_MoveCyl(aiPSB_ElvtUpDn  ,ccBwd);
                 AT_MoveCyl(aiPSB_GrprGrRl  ,ccBwd);
                 AT_MoveCyl(aiPSB_IndxDnUp  ,ccBwd);
                 AT_MoveCyl(aiPSB_PshrDnUp  ,ccBwd);
                 AT_MoveCyl(aiPSB_PshrFwBw  ,ccBwd);
                 AT_MoveCyl(aiPSB_RailOpCl  ,ccBwd);

                 IO_SetY(yPSB_RjcBelt, false );

                 Step.iToStop++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPSB_ElvtUpDn  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_GrprGrRl  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_IndxDnUp  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_PshrDnUp  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_PshrFwBw  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_RailOpCl  ,ccBwd)) return false ;

                 MoveMotr(miPSB_XIndx , piPSB_XIndxWait);
                 Step.iToStop++ ;
                 return false ;

        case 12: if(!MoveMotr(miPSB_XIndx , piPSB_XIndxWait)) return false;
                 Step.iToStop = 0;
                 return true ;
    }
}

//자제 드루와~ 드루와~
bool CPostBuff::CycleIn(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiPSB_CycleTO);
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

        case  10: Step.iCycle++;
                  return false;

        case  11: if(!IO_GetX(xPSB_PkgIn5))return false ;
                  Step.iCycle++;
                  return false;

        case  12: if(IO_GetX(xPSB_PkgIn5)) return false ;
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false ;

        case  13: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iPSBGripDelay)) return false ;
                  MoveActr(aiPSB_GrprGrRl , ccFwd);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveActr(aiPSB_GrprGrRl , ccFwd)) return false ;
                  m_tmDelay.Clear();

                  Step.iCycle++;
                  return false;

        case  15: if(!m_tmDelay.OnDelay(true , 300)) return false ;
                  MoveActr(aiPSB_GrprGrRl , ccBwd);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveActr(aiPSB_GrprGrRl , ccBwd)) return false ;
                  //DM.ShiftArrayData(riVSN , riPSB); //riVSN에서 옮김.

                  Step.iCycle=0;
                  return true;
    }
}

//작업 위치로 이동.
bool CPostBuff::CycleReject()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiPSB_CycleTO);
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

    if (!IO_GetX(xPSB_IdxOvld ) ){
        EM_SetErr (eiPSB_IdxOvld );
        MT_Stop   (miPSB_XIndx);
        Step.iCycle = 0 ;
        return true;
    }






    String sCrntLotNo ;
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

        case  10: MoveActr(aiPSB_IndxDnUp , ccBwd);
                  MoveActr(aiPSB_PshrDnUp , ccBwd);
                  MoveActr(aiPSB_PshrFwBw , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPSB_IndxDnUp , ccBwd)) return false ; 
                  if(!MoveActr(aiPSB_PshrDnUp , ccBwd)) return false ; 
                  if(!MoveActr(aiPSB_PshrFwBw , ccBwd)) return false ; 

                  MoveMotr(miPSB_XIndx , piPSB_XIndxWait);
                  Step.iCycle++;
                  return false ;



        case  12: if(!MoveMotr(miPSB_XIndx , piPSB_XIndxWait)) return false ;

                  MoveActr(aiPSB_IndxDnUp , ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiPSB_IndxDnUp , ccFwd)) return false ;
                  MoveMotr(miPSB_XIndx , piPSB_XIndxRjct);

                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miPSB_XIndx , piPSB_XIndxRjct)) return false ;

                  MoveActr(aiPSB_IndxDnUp , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiPSB_IndxDnUp , ccBwd)) return false ;
                  MoveMotr(miPSB_XIndx , piPSB_XIndxWait); //싸이클 끝에서 확인.
                  MoveActr(aiPSB_ElvtUpDn , ccFwd);

                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveActr(aiPSB_ElvtUpDn , ccFwd)) return false ;
                  MoveActr(aiPSB_RailOpCl , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiPSB_RailOpCl , ccFwd))return false ;

                  MoveActr(aiPSB_ElvtUpDn , ccBwd);
                  IO_SetY(yPSB_RjcBelt, true );

                  //if(!OM.CmnOptn.bFrstStrpOut && !LSR.m_bChangeLotNo){
                  //    Step.iCycle++;
                  //    return false ;
                  //}
                  //if(
                  //


                  Step.iCycle++;
                  return false ;

        case  18: if(!MoveActr(aiPSB_ElvtUpDn , ccBwd)) return false ;
                  MoveActr(aiPSB_RailOpCl , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  19: if(!MoveActr(aiPSB_RailOpCl , ccBwd))return false ;

                  sCrntLotNo = DM.ARAY[riPSB].GetLotNo() ;
                  if(!OM.CmnOptn.bFrstStrpOut){
                      DM.ARAY[riPSB].ClearMap();
                  }
                  else {
                      if(m_sWorkedLotNo != sCrntLotNo){ //첫자제면 확인 해보라는 에러.
                          EM_SetErr(eiLSR_LotFrstStrip);
                      }
                      else {
                          DM.ARAY[riPSB].ClearMap();
                      }
                  }
                  m_sWorkedLotNo = sCrntLotNo ;

                  IO_SetY(yPSB_RjcBelt, false); //IO Off딜레이 넣는다. 

                  Step.iCycle=0;
                  return true ;
       //
       // case  18: if(!IO_GetX(xPSB_RjcPkg))return false ;
       //           //if(!MoveMotr(miPSB_XIndx , piPSB_XIndxWait)) return false ; 인사이클 못탈수 있음...CycleOut 혹은 Reject에서 처음에 확인.
       //           //if(!MoveActr(aiPSB_IndxDnUp , ccBwd)) return false ;
       //
       //
       //           Step.iCycle=0;
       //           return true;
    }
}

//Back위치에서 Out위치까지 밀어 넣음.
bool CPostBuff::CycleOut()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiPSB_CycleTO);
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

    if (!IO_GetX(xPSB_IdxOvld ) ){
        EM_SetErr (eiPSB_IdxOvld );
        MT_Stop   (miPSB_XIndx);
        Step.iCycle = 0 ;
        return true;
    }




    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    String sTemp2;
    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;
        case  10: MoveActr(aiPSB_IndxDnUp , ccBwd);
                  MoveActr(aiPSB_PshrDnUp , ccBwd);
                  MoveActr(aiPSB_PshrFwBw , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPSB_IndxDnUp , ccBwd)) return false ; 
                  if(!MoveActr(aiPSB_PshrDnUp , ccBwd)) return false ; 
                  if(!MoveActr(aiPSB_PshrFwBw , ccBwd)) return false ; 

                  MoveMotr(miPSB_XIndx , piPSB_XIndxWait);
                  Step.iCycle++;
                  return false ;



        case  12: if(!MoveMotr(miPSB_XIndx , piPSB_XIndxWait)) return false ;
                  MoveActr(aiPSB_IndxDnUp , ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiPSB_IndxDnUp , ccFwd)) return false ;
                  Step.iCycle++;
                  return false ;

        case  14: if(ULD.GetSeqStep() != CUnLoader :: scIdle) return false  ; //졸라짜증나는 1싸이클 사이 에 들어 올수 있어서 막아놓음.

                  MoveMotr(miPSB_XIndx , piPSB_XIndxOut);

                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miPSB_XIndx , piPSB_XIndxOut)) return false ;
                  MoveActr(aiPSB_IndxDnUp , ccBwd);
                  MoveActr(aiPSB_PshrDnUp , ccFwd);

                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveActr(aiPSB_PshrDnUp , ccFwd)) return false ;
                  if(!MoveActr(aiPSB_IndxDnUp , ccBwd)) return false ;
                  MoveMotr(miPSB_XIndx    , piPSB_XIndxWait);
                  MoveActr(aiPSB_PshrFwBw , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiPSB_PshrFwBw , ccFwd)) return false ;
                  m_sWorkedLotNo = DM.ARAY[riPSB].GetLotNo() ;

                  if(DM.ARAY[riULD].CheckAllStat(csEmpty)) {
                      DM.ARAY[riULD].SetLotNo(m_sWorkedLotNo);
                      DM.ARAY[riULD].SetID(DM.ARAY[riPSB].GetID());//Mgz Count
                  }
                  DM.ARAY[riULD].SetStat(DM.ARAY[riULD].FindFrstRow(csEmpty),0 , csWork);

                  DM.ARAY[riPSB].ClearMap();

                  MoveActr(aiPSB_PshrDnUp , ccBwd);
                  MoveActr(aiPSB_PshrFwBw , ccBwd);

                  Step.iCycle=0;
                  return false ;

        case  18: //if(!MoveActr(aiPSB_PshrDnUp , ccBwd)) return false ; //이거 확인 하면 벌써 다음 자제 들어와서
                  //if(!MoveActr(aiPSB_PshrFwBw , ccBwd)) return false ; //인싸이클 못탐.
                  //if(!MoveMotr(miPSB_XIndx    , piPSB_XIndxWait)) return false ;
                  //if(!MoveActr(aiPSB_IndxDnUp , ccBwd)) return false ;

                  Step.iCycle=0;
                  return true;
    }
}

void CPostBuff::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CPostBuff::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CPostBuff::CheckStop()
{
    if(!MT_GetStop(miPSB_XIndx)) return false ;


    if(!AT_Done(aiPSB_ElvtUpDn)) return false ;
    if(!AT_Done(aiPSB_GrprGrRl)) return false ;
    if(!AT_Done(aiPSB_IndxDnUp)) return false ;
    if(!AT_Done(aiPSB_PshrDnUp)) return false ;
    if(!AT_Done(aiPSB_PshrFwBw)) return false ;
    if(!AT_Done(aiPSB_RailOpCl)) return false ;

    return true ;
}

void CPostBuff::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\PostBuff.INI";

    if ( _bLoad ) UserINI.Load(sPath, "Member" , "m_sWorkedLotNo" , m_sWorkedLotNo );

    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_sWorkedLotNo" , m_sWorkedLotNo );
    }

}
//---------------------------------------------------------------------------

/*

    myGuy = new Entity();
    myGuy.name = "Jerry";
    myGuy.age = 0;
    myGuy.update = delegate( Entity self ){
        // do thing
        // do more things
        if( self.age > whatever ){
            self.update = delegate( Entity self ){
                // do thing
                // do other thing
                if( transition2_test < whatever ){
                    self.update = delegate( Entity self ){
                    // do last set of things
                }
            }
        }
    }
*/

