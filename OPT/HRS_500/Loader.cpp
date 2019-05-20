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
//sun MT.AllDone
//sun AT.AllDone�����.
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
    AT_MoveCyl(aiLDR_Sptr , ccFwd);
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

    if(_iActr == aiLDR_Sptr){
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

bool CLoader::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // ���͸� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CLoader::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CLoader::CycleHome()     //sun DLL direct access.
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

        case 10: AT_MoveCyl(aiLDR_Sptr, ccFwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiLDR_Sptr, ccFwd))return false ;
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

bool CLoader::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CLoader::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CLoader::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
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

        case 10: MoveActr(aiLDR_Sptr, ccFwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiLDR_Sptr, ccFwd)) return false;
                 MoveMotr(miLDR_Z , piLDR_ZWait);
                 Step.iToStart ++ ;
                 return false ;

        case 12: if(!MoveMotr(miLDR_Z , piLDR_ZWait)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CLoader::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
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

        case 10: MoveActr(aiLDR_Sptr, ccFwd);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MoveActr(aiLDR_Sptr, ccFwd)) return false;
                 MoveMotr(miLDR_Z , piLDR_ZWait);
                 Step.iToStop ++ ;
                 return false ;

        case 12: if(!MoveMotr(miLDR_Z , piLDR_ZWait)) return false ;
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CLoader::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool isCycleSupply  =  DM.ARAY[riPRB].CheckAllStat(csNone) && IO_GetX(xLDR_TrayDetect) && !OM.CmnOptn.bLoadingStop ;
        bool isConEnd       = !IO_GetX(xLDR_TrayDetect) ;

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleSupply ) {Trace(m_sPartName.c_str(),"CycleSupply  Stt"); Step.iSeq = scSupply ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd      ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default        :                     Trace(m_sPartName.c_str(),"default     End");Step.iSeq = scIdle ;  return false ;
        case scIdle    :                                                                                        return false ;
        case scSupply  : if(CycleSupply ()){ Trace(m_sPartName.c_str(),"CycleSupply End");Step.iSeq = scIdle ;} return false ;
    }

}

//One Cycle.
bool CLoader::CycleSupply(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
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

        case  10: MoveActr(aiLDR_Sptr, ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiLDR_Sptr, ccFwd)) return false ;
                  MoveMotr(miLDR_Z , piLDR_ZTop);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLDR_Z,piLDR_ZTop)) return false ;
                  MoveActr(aiLDR_Sptr, ccBwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiLDR_Sptr, ccBwd)) return false ;
                  MoveMotr(miLDR_Z , piLDR_ZSparate);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miLDR_Z , piLDR_ZSparate)) return false ;
                  MoveActr(aiLDR_Sptr, ccFwd);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiLDR_Sptr, ccFwd)) return false ;
                  MoveMotr(miLDR_Z , piLDR_ZWait);
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miLDR_Z , piLDR_ZWait)) return false ;
                  DM.ARAY[riPRB].SetStat(csUnkwn);

                  //��� �ɵ�.
                  if(!IO_GetX(xRAL_Detect1) || !IO_GetX(xRAL_Detect2)) EM_SetErr(eiLDR_Sply);

                  Step.iCycle = 0 ;
                  return true ;
    }
}

void CLoader::SetLastCmd()
{
    return ; //����Ʈ�� �ʿ� ����.

}

bool CLoader::CheckMoved()
{
    return true ; //����Ʈ�� �ʿ� ����.
}

bool CLoader::CheckStop()
{
    if(!MT_GetStop(miLDR_Z)) return false ;

    if(!AT_Done(aiLDR_Sptr )) return false ;

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
