//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Loader.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CLoader LDR;

CLoader::CLoader()
{
    Init();
}
CLoader::~CLoader (void)
{
    Close();
}
void CLoader::Init()
{
    m_sPartName = "Loader " ;
    Reset();
    AT_MoveCyl(aiLDR_SptrFB , ccFwd);
    Load(true);
}
void CLoader::Close()
{
    Load(false);
}
void CLoader::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CLoader::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();

    //Bottom.
    m_tmBCycle .Clear();
    m_tmBTemp  .Clear();
}



bool CLoader::FindChip(EN_ARAY_ID _riWhre , int &r , int &c )
{
    return false;
}

double CLoader::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    if(_iMotr == miLDR_Z){
        switch(_iPstnId) {
            default             : dPos = MT_GetCmdPos(_iMotr                  ); break ;
            case piLDR_ZWait    : dPos = PM.GetValue (_iMotr , pvLDR_ZWait    ); break ;
            case piLDR_ZTop     : dPos = PM.GetValue (_iMotr , pvLDR_ZTop     ); break ;
            case piLDR_ZSparate : dPos = PM.GetValue (_iMotr , pvLDR_ZSparate ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CLoader::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;


    if(_iMotr == miLDR_Z){
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
bool CLoader::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;
    bool       bLDR_ZWait = MT_CmprPos(miLDR_Z , PM.GetValue(miLDR_Z , pvLDR_ZWait)) ;
    bool       bLDR_ZTop  = MT_CmprPos(miLDR_Z , PM.GetValue(miLDR_Z , pvLDR_ZTop )) ;
    bool       bRAL_XSafe = MT_CmprPos(miRAL_X , PM.GetValue(miRAL_X , pvRAL_XWait)) || MT_GetCmdPos(miRAL_X) > PM.GetValue(miRAL_X , pvRAL_XWait);

    if(_iActr == aiLDR_SptrFB){
        if(_bFwd == ccBwd && IO_GetX(xLDR_TrayDetect) && !bLDR_ZTop) {
            sMsg = "Tray Exist." ; bRet = false ;
        }
    }
    else if(_iActr == aiLDR_SplyFB) {
        if(_bFwd && !bLDR_ZWait) {
            sMsg = MT_GetName(miLDR_Z) + " is not Wait Position" ;
            bRet = false ;
        }
        if(_bFwd && !bRAL_XSafe) {
            sMsg = MT_GetName(miRAL_X) + " is not Safe Position" ;
            bRet = false;
        }
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

bool CLoader::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CLoader::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CLoader::CycleHome()     
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiLDR_HomeTO);
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

        default: if(Step.iHome) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iHome = 0 ;
                 return true ;

        case 10: AT_MoveCyl(aiLDR_SptrFB, ccFwd);
                 AT_MoveCyl(aiLDR_SplyFB, ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiLDR_SptrFB, ccFwd))return false ;
                 if(!AT_MoveCyl(aiLDR_SplyFB, ccBwd))return false ;
                 MT_DoHome(miLDR_Z);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miLDR_Z))return false ;
                 MT_GoAbsMan(miLDR_Z , PM.GetValue(miLDR_Z , pvLDR_ZWait  ));
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miLDR_Z , PM.GetValue(miLDR_Z , pvLDR_ZWait  )))return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CLoader::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CLoader::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CLoader::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr(eiLDR_ToStartTO);

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

        case 10: MoveActr(aiLDR_SptrFB, ccFwd);
                 MoveActr(aiLDR_SplyFB, ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiLDR_SptrFB, ccFwd)) return false;
                 if(!MoveActr(aiLDR_SplyFB, ccBwd)) return false;
                 MoveMotr(miLDR_Z , piLDR_ZWait);
                 Step.iToStart ++ ;
                 return false ;

        case 12: if(!MoveMotr(miLDR_Z , piLDR_ZWait)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CLoader::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErr(eiLDR_ToStopTO);

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

        case 10: MoveActr(aiLDR_SptrFB, ccFwd);
                 MoveActr(aiLDR_SplyFB, ccBwd);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MoveActr(aiLDR_SptrFB, ccFwd)) return false;
                 if(!MoveActr(aiLDR_SplyFB, ccBwd)) return false;
                 MoveMotr(miLDR_Z , piLDR_ZWait);
                 Step.iToStop ++ ;
                 return false ;

        case 12: if(!MoveMotr(miLDR_Z , piLDR_ZWait)) return false ;
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CLoader::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool isCycleSupply  =  DM.ARAY[riPRB].CheckAllStat(csNone) &&  IO_GetX(xLDR_TrayDetect) && !OM.CmnOptn.bLoadingStop &&!OM.MstOptn.bCheckCycle && !IO_GetX(xRAL_Detect1) && !IO_GetX(xRAL_Detect2) ;
        bool isCycleBfSply  =  DM.ARAY[riPRB].CheckAllStat(csNone) && !IO_GetX(xLDR_TrayDetect) && !OM.CmnOptn.bLoadingStop && IO_GetX(xLDR_BuffDetect) && !OM.MstOptn.bCheckCycle ;
        //&& (DM.ARAY[riWRK].FindLastCol(csUnkwn) <= 6 || DM.ARAY[riWRK].CheckAllStat(csNone));
        bool isConEnd       = !IO_GetX(xLDR_TrayDetect) && !IO_GetX(xLDR_BuffDetect) ;

        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleSupply ) {Trace(m_sPartName.c_str(),"CycleSupply  Stt"); Step.iSeq = scSupply ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleBfSply ) {Trace(m_sPartName.c_str(),"CycleBfSply  Stt"); Step.iSeq = scBfSply ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd      ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default        :                     Trace(m_sPartName.c_str(),"default     End");Step.iSeq = scIdle ;  return false ;
        case scIdle    :                                                                                        return false ;
        case scSupply  : if(CycleSupply ()){ Trace(m_sPartName.c_str(),"CycleSupply End");Step.iSeq = scIdle ;} return false ;
        case scBfSply  : if(CycleBfSply ()){ Trace(m_sPartName.c_str(),"CycleBfSply End");Step.iSeq = scIdle ;} return false ;
    }

}

//One Cycle.
bool CLoader::CycleSupply(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErr(eiLDR_CycleTO);
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

    bool r1,r2 ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiLDR_SptrFB, ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiLDR_SptrFB, ccFwd)) return false ;
                  MoveMotr(miLDR_Z , piLDR_ZTop);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLDR_Z,piLDR_ZTop)) return false ;
                  MoveActr(aiLDR_SptrFB, ccBwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiLDR_SptrFB, ccBwd)) return false ;
                  MoveMotr(miLDR_Z , piLDR_ZSparate);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miLDR_Z , piLDR_ZSparate)) return false ;
                  MoveActr(aiLDR_SptrFB, ccFwd);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiLDR_SptrFB, ccFwd)) return false ;
                  MoveMotr(miLDR_Z , piLDR_ZWait);
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miLDR_Z , piLDR_ZWait)) return false ;
                  DM.ARAY[riPRB].SetStat(csUnkwn);

                  //없어도 될듯.
                  if(!IO_GetX(xRAL_Detect1) || !IO_GetX(xRAL_Detect2)) EM_SetErr(eiLDR_Sply);

                  Step.iCycle = 0 ;
                  return true ;
    }
}

//One Cycle.
bool CLoader::CycleBfSply(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErr(eiLDR_CycleTO);
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

    bool r1,r2 ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiLDR_SptrFB, ccBwd);
                  MoveMotr(miLDR_Z , piLDR_ZWait);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiLDR_SptrFB, ccBwd)) return false ;
                  if(!MoveMotr(miLDR_Z,piLDR_ZWait)) return false ;
                  Step.iCycle++;
                  return false;

        case  12: MoveActr(aiLDR_SplyFB, ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiLDR_SplyFB, ccFwd)) return false ;
                  MoveMotr(miLDR_Z , piLDR_ZTop);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miLDR_Z , piLDR_ZTop)) return false ;
                  MoveActr(aiLDR_SplyFB, ccBwd);
                  MoveActr(aiLDR_SptrFB, ccFwd);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiLDR_SplyFB, ccBwd)) return false ;
                  if(!MoveActr(aiLDR_SptrFB, ccFwd)) return false ;
                  MoveMotr(miLDR_Z , piLDR_ZWait);
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miLDR_Z , piLDR_ZWait)) return false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

void CLoader::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CLoader::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CLoader::CheckStop()
{
    if(!MT_GetStop(miLDR_Z)) return false ;

    if(!AT_Done(aiLDR_SptrFB )) return false ;
    if(!AT_Done(aiLDR_SplyFB )) return false ;

    return true ;
}

void CLoader::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\Loader.INI";

    if ( IsLoad ) {
        //UserINI.Load(sPath, "Member" , "m_iMgzCnt " , m_iMgzCnt );
    }
    else {
        UserINI.ClearFile(sPath) ;
        //UserINI.Save(sPath, "Member" , "m_iMgzCnt " , m_iMgzCnt );
    }
}
//---------------------------------------------------------------------------
