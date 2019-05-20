//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "Work.h"
#include "PostBuff.h"
//---------------------------------------------------------------------------
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"                                        ff
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
    bool isIdxDn   = AT_Complete(aiPSB_IdxUpDn  , ccBwd) ;
    bool isPushrDn =!AT_Complete(aiPSB_PshDnUp  , ccBwd) ;
    bool isPushrFw =!AT_Complete(aiPSB_PshFwBw  , ccBwd) ;

    if(_iMotr == miPSB_XIndx){
        if(isPushrDn && isIdxDn) {sMsg = "Index Pinger is Down"; bRet = false ;}
        switch(_iPstnId) {
            default              :  break ;
            case piPSB_XIndxWait : if(!isIdxDn              ) {sMsg = "Index Cylinder is Not Down"; bRet = false;} break ;
            case piPSB_XIndxOut  : if(isPushrFw || isPushrDn) {sMsg = "Pusher Cylinder is Not Bwd"; bRet = false;} break ;
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

    if(aiPSB_IdxUpDn){}
    else if(aiPSB_PshFwBw){}
    else if(aiPSB_PshDnUp){}

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
        Step.iHome = 0 ;
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

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                 //Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: AT_MoveCyl(aiPSB_IdxUpDn , ccBwd);
                 AT_MoveCyl(aiPSB_PshFwBw , ccBwd);
                 Step.iHome++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPSB_IdxUpDn , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_PshFwBw , ccBwd)) return false ;
                 AT_MoveCyl(aiPSB_PshDnUp , ccBwd);
                 Step.iHome++;
                 return false;

        case 12: if(!AT_MoveCyl(aiPSB_PshDnUp , ccBwd)) return false ;
                 MT_SetHomeEnd(miPSB_XIndx , false);
                 MT_DoHome(miPSB_XIndx);
                 Step.iHome++ ;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miPSB_XIndx)) return false;
                 MT_GoAbsMan(miPSB_XIndx, PM.GetValue(miPSB_XIndx , pvPSB_XIndxWait));
                 Step.iHome++ ;
                 return false ;

        case 14: if(!MT_GoAbsMan(miPSB_XIndx, PM.GetValue(miPSB_XIndx , pvPSB_XIndxWait))) return false ;
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

        bool isCycleOut    = !DM.ARAY[riPSB].CheckAllStat(csNone) && DM.ARAY[riULD].GetCntStat(csEmpty)
                             && ULD.GetSeqStep() == CUnLoader :: scIdle ;
        bool isCycleEnd    =  DM.ARAY[riPSB].CheckAllStat(csNone) && WRK.GetWorkEnd();



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
             if (isCycleOut   ) {Trace(m_sPartName.c_str(),"CycleOut    Stt"); Step.iSeq = scOut   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default        :                    Trace(m_sPartName.c_str(),"default     End");Step.iSeq = scIdle ;  return false ;
        case scIdle    :                                                                                       return false ;
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

        case 10: MoveActr(aiPSB_PshFwBw , ccBwd);
                 Step.iToStart++ ;
                 return false ;

        case 11: if(!MoveActr(aiPSB_PshFwBw , ccBwd))return false;
                 MoveActr(aiPSB_IdxUpDn , ccBwd);
                 MoveActr(aiPSB_PshDnUp , ccBwd);
                 Step.iToStart++ ;
                 return false ;

        case 12: if(!MoveActr(aiPSB_IdxUpDn , ccBwd)) return false;
                 if(!MoveActr(aiPSB_PshDnUp , ccBwd)) return false;
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

        case 10: MoveActr(aiPSB_PshFwBw , ccBwd);
                 Step.iToStop++ ;
                 return false ;

        case 11: if(!MoveActr(aiPSB_PshFwBw , ccBwd))return false;
                 MoveActr(aiPSB_IdxUpDn , ccBwd);
                 MoveActr(aiPSB_PshDnUp , ccBwd);
                 Step.iToStop++ ;
                 return false ;

        case 12: if(!MoveActr(aiPSB_IdxUpDn , ccBwd)) return false;
                 if(!MoveActr(aiPSB_PshDnUp , ccBwd)) return false;
                 Step.iToStop = 0;
                 return true ;
    }
}

//들어와도 할것 없어서 그냥 아웃.
bool CPostBuff::CycleOut(void)
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

    if(!OM.MstOptn.bSkipOvldSnsr){
        if(IO_GetX(xPSB_IdxOvld)){
            EM_SetErr(eiPSB_IndxOvld);
            Step.iCycle = 0;
            return true;
        }
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPSB_IdxUpDn , ccBwd);
                  MoveActr(aiPSB_PshFwBw , ccBwd);
                  MoveActr(aiPSB_PshDnUp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPSB_IdxUpDn , ccBwd)) return false;
                  if(!MoveActr(aiPSB_PshFwBw , ccBwd)) return false;
                  if(!MoveActr(aiPSB_PshDnUp , ccBwd)) return false;
                  MoveMotr(miPSB_XIndx , piPSB_XIndxWait);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPSB_XIndx , piPSB_XIndxWait)) return false;
                  MoveActr(aiPSB_IdxUpDn , ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiPSB_IdxUpDn , ccFwd)) return false;
                  if(ULD.GetSeqStep() != CUnLoader :: scIdle) return false  ;
                  MoveMotr(miPSB_XIndx , piPSB_XIndxOut);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miPSB_XIndx , piPSB_XIndxOut)) return false;
                  MoveActr(aiPSB_IdxUpDn , ccBwd);
                  MoveActr(aiPSB_PshDnUp , ccFwd);
                  MoveMotr(miPSB_XIndx , piPSB_XIndxWait);
                  Step.iCycle++;
                  return false;

        case  15: if(!IO_GetX(xPSB_PkgOut)){
                      EM_SetErr(eiPKG_Dispr);
                      Step.iCycle = 17;
                      return false;
                  }
                  if(!MoveActr(aiPSB_PshDnUp , ccFwd)) return false;
                  MoveActr(aiPSB_PshFwBw , ccFwd);

                  Step.iCycle++;
                  return false;

        case  16: if(!MoveActr(aiPSB_PshFwBw , ccFwd)) return false;
                  MoveActr(aiPSB_PshFwBw , ccBwd);
                  Step.iCycle++;
                  return false;

        case  17: if(!MoveActr(aiPSB_PshFwBw , ccBwd)) return false;
                  MoveActr(aiPSB_PshDnUp , ccBwd);
                  m_sWorkedLotNo = DM.ARAY[riPSB].GetLotNo() ;

                  if(DM.ARAY[riULD].CheckAllStat(csEmpty)) {
                      DM.ARAY[riULD].SetLotNo(m_sWorkedLotNo);
                      DM.ARAY[riULD].SetID(DM.ARAY[riPSB].GetID());//Mgz Count
                  }
                  DM.ARAY[riULD].SetStat(DM.ARAY[riULD].FindFrstRow(csEmpty),0 , csWork);
                  DM.ARAY[riPSB].ClearMap();
                  Step.iCycle++;
                  return false;

        case  18: if(!MoveActr(aiPSB_PshDnUp , ccBwd          )) return false;
                  if(!MoveMotr(miPSB_XIndx   , piPSB_XIndxWait)) return false;
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


    if(!AT_Done(aiPSB_IdxUpDn)) return false ;
    if(!AT_Done(aiPSB_PshFwBw)) return false ;
    if(!AT_Done(aiPSB_PshDnUp)) return false ;

    return true ;
}

void CPostBuff::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\PostBuff.INI";

    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_sWorkedLotNo" , m_sWorkedLotNo );
        if(m_dLastIndPos > MT_GetMaxPos(miPSB_XIndx)) m_dLastIndPos = MT_GetMaxPos(miPSB_XIndx);
    }

    else {
        UserINI.ClearFile(sPath) ;
        if(m_dLastIndPos > MT_GetMaxPos(miPSB_XIndx)) m_dLastIndPos = MT_GetMaxPos(miPSB_XIndx);
        UserINI.Save(sPath, "Member" , "m_sWorkedLotNo" , m_sWorkedLotNo );

    }

}
//---------------------------------------------------------------------------
