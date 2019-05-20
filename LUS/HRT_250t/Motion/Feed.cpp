//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Feed.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "Rail.h"
#include "Heater.h"
#include "ManualMan.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone�����.
//---------------------------------------------------------------------------
CFeed FED;

CFeed::CFeed()
{
    Init();
}
CFeed::~CFeed (void)
{
    Close();
}
void CFeed::Init()
{
    m_sPartName   = "Feed " ;
    bConEnd       = false;

    m_iHomeTO  = (int)eiFED_HomeTO    ;
    m_iStartTO = (int)eiFED_ToStartTO ;
    m_iStopTO  = (int)eiFED_ToStopTO  ;
    m_iCycleTO = (int)eiFED_CycleTO   ;

    m_iFeedFailed = 0;

    Reset();
    Load(true);
}
void CFeed::Close()
{
    Load(false);
}
void CFeed::Reset()
{
    ResetTimer();

    iFeedCount = 0;

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));

    //bConEnd = false;
}

void CFeed::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();

}

bool CFeed::FindChip(EN_ARAY_ID _riWhre , int &r , int &c )
{
    //int r1,c1;
    //if(_riWhre == riRAE) {
    //    c = DM.ARAY[riRAE].FindLastCol(csUnkwn) ;
    //    return (c > -1 && c > -1);
    //}
    return false ;
}

double CFeed::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    //if(_iMotr == miFED_TFed){
    //    switch(_iPstnId) {
    //        default             : dPos = MT_GetCmdPos(_iMotr                  ); break ;
    //        case piFED_TFedWait : dPos = PM.GetValue (_iMotr , pvFED_TFedWait ); break ;
    //        case piFED_TFedWork : dPos = PM.GetValue (_iMotr , pvFED_TFedWork ); break ;
    //    }
    //}
    //
    //else {
    //    dPos = MT_GetCmdPos(_iMotr);
    //}

    return dPos ;
}

//---------------------------------------------------------------------------
bool CFeed::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CFeed::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}

bool CFeed::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // ���͸� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CFeed::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CFeed::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iHomeTO);
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

        case 10: AT_MoveCyl(aiRAL_Stopper, ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: 
                 Step.iHome = 0;
                 return true ;
    }
}

bool CFeed::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CFeed::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{
    bool bRet = true ;

//    if(!OM.CmnOptn.bIgnrHeatErr) {
        //if(IO_GetY(yRAL_AirBlower)) IO_SetY(yRAL_AirBlower,false);
        //if(OM.DevOptn.iTempSet1 + OM.DevOptn.iTempGap < HTR.GetTemp1() ) bRet = false ;
        //if(OM.DevOptn.iTempSet1 - OM.DevOptn.iTempGap > HTR.GetTemp1() ) bRet = false ;
        //
        //if(OM.DevOptn.iTempSet2 + OM.DevOptn.iTempGap < HTR.GetTemp2() ) bRet = false ;
        //if(OM.DevOptn.iTempSet2 - OM.DevOptn.iTempGap > HTR.GetTemp2() ) bRet = false ;
//    }

    if(!bRet) return false ;


    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CFeed::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)m_iStartTO);

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

        case 10: m_tmNoPkg.Clear();
                 m_tmNoPkgDust.Clear();
                 bConEnd = false;
                 MoveActr(aiRAL_Stopper, ccBwd);
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CFeed::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)m_iStopTO);

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

        case 10: //MT_Stop(miFED_TFed);
                 MoveActr(aiRAL_Stopper , ccBwd);
                 if(!EM_IsErr()) IO_SetY(yETC_FeederOn,false);
                 Step.iToStop++;
                 return false;

        case 11: if(!EM_IsErr()) {
                     if(IO_GetY(yETC_FeederOn))return false;
                 }
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CFeed::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        //Judgment
        //bool isCycleFeed = DM.ARAY[riRAL].CheckAllStat(csNone) && !OM.CmnOptn.bFeedingStop && ( RAL.bFedStt || DM.ARAY[riWRK].CheckAllStat(csNone) )&&
        //                   IO_GetX(xRAL_PkgInChk) && AT_Done(aiRAL_IdxUpDn) && !IO_GetX(xRAL_PkgDustChk) ;

        bool isCycleFeed = DM.ARAY[riRAL].CheckAllStat(csNone ) && IO_GetX(xRAL_PkgInChk) && !IO_GetX(xRAL_Pkg);

//                           DM.ARAY[riRLO].CheckAllStat(csNone);
        bool isConEnd    = bConEnd ;//!isCycleFeed ;

        //���� ����.

        //if(m_tmNoPkgDust.OnDelay(IO_GetX(xRAL_PkgDustChk) && !OM.CmnOptn.bFeedingStop, 2000)) {
        //    EM_SetErr(eiRAL_DustChip);
        //    bConEnd = true ;
        //}

        if(isCycleFeed) m_tmNoPkg.Clear();// ���簡 ���� �ð� �ȵ����� Error

        if(m_tmNoPkg.OnDelay( !isCycleFeed , OM.DevOptn.iNoPkgSec * 1000)) {
            EM_SetErr(eiRAL_NoPkg);
            bConEnd = true ;
        }

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleFeed ) {Trace(m_sPartName.c_str(),"CycleFeed  Stt"); Step.iSeq = scFeed ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
//             if (isCycleFeed ) {Trace(m_sPartName.c_str(),"CycleFeed  Stt"); Step.iSeq = scFeed ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd    ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default        :                  Trace(m_sPartName.c_str(),"default   End");Step.iSeq = scIdle ;  return false ;
        case scIdle    :                                                                                   return false ;
        case scFeed    : if(CycleFeed()){ Trace(m_sPartName.c_str(),"CycleFeed End");Step.iSeq = scIdle ;} return false ;
    }

}
//One Cycle.
bool CFeed::CycleFeed(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //�̷��� ������ �ٲܼ� �ְ� �ؼ� ����Ŭ �̸����� �־��ָ� ���ٳ�...
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

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;


        case  10: MoveActr(aiRAL_Stopper , ccBwd);
                  m_iFeedFailed++;
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiRAL_Stopper , ccBwd)) return false;
                  MoveActr(aiRAL_Stopper , ccFwd);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveActr(aiRAL_Stopper , ccFwd)) return false;
                  if(m_tmTemp.OnDelay(!IO_GetX(xRAL_Pkg), 2 * 1000)){
                      if(m_iFeedFailed > 1){
                          EM_SetErr(eiFED_FeedFailed);
                          m_iFeedFailed = 0;
                          Step.iCycle = 0;
                          return true;
                      }
                      else {
                          Step.iCycle = 10;
                          return false;
                      }
                  }
                  if(!IO_GetX(xRAL_Pkg)) return false;
                  m_iFeedFailed = 0;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  13: Step.iCycle++;
                  return false;

        case  14: MoveActr(aiRAL_Stopper , ccBwd);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiRAL_Stopper , ccBwd)) return false;
                  DM.ARAY[riRAL].SetStat(csUnkwn);
                  Step.iCycle = 0;
                  return true;
    }
}
double CFeed::GetLastCmd(EN_MOTR_ID _iMotr)
{
    double _dLastIdxPos = 0.0 ;
    if(!MT_GetAlarm(_iMotr) && !MT_GetNLimSnsr(_iMotr) && !MT_GetPLimSnsr(_iMotr) ) _dLastIdxPos = MT_GetCmdPos(_iMotr) ;
    else                                                                            _dLastIdxPos = GetMotrPos  (_iMotr , (EN_PSTN_ID)0) ;

    return _dLastIdxPos ;
}

bool CFeed::CheckMoved()
{
    return true ; //����Ʈ�� �ʿ� ����.
}

bool CFeed::CheckStop()
{
    //if(!MT_GetStop(miFED_TFed)) return false ;

    //if(!AT_Done(aiFED_BAlign )) return false ;

    return true ;
}

void CFeed::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() + ".INI";

    if ( IsLoad ) {
        //UserINI.Load(sPath, "Member" , "m_dLastIdxPos " , m_dLastIdxPos );
    }
    else {
        //UserINI.ClearFile(sPath) ;
        //UserINI.Save(sPath, "Member" , "m_dLastIdxPos " , m_dLastIdxPos );
    }
}
//---------------------------------------------------------------------------