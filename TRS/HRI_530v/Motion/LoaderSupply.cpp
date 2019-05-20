//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "LoaderSupply.h"
#include "Loader.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "LotUnit.h"
#include "UserIni.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CLoaderSupply LSP;
CLoaderSupply::CLoaderSupply(void)
{

}

CLoaderSupply::~CLoaderSupply (void)
{

}

void CLoaderSupply::Init()
{
    m_sPartName = "LoaderSupply" ;
    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();

    m_iTrayCnt    = 0     ;
    //m_bRqstLotEnd = false ;

    m_bNeedTrayErrDisp = false;


}

void CLoaderSupply::Close()
{
    Load(false);
}

void CLoaderSupply::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}


bool CLoaderSupply::FindChip(int &r , int &c )
{
    //r = DM.ARAY[riLDR].FindLastRow(csUnkwn) ;
    r=0 ; c=0;
    return false ;

}

double CLoaderSupply::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    if(_iMotr == miLSP_ZElev){
        switch(_iPstnId) {
            default                   : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piLSP_ZElevWait      : dPos = PM.GetValue (_iMotr , pvLSP_ZElevWait      ); break ;
            case piLSP_ZElevTop       : dPos = PM.GetValue (_iMotr , pvLSP_ZElevTop       ); break ;
            case piLSP_ZElevSperate   : dPos = PM.GetValue (_iMotr , pvLSP_ZElevSperate   ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CLoaderSupply::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

bool CLoaderSupply::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CLoaderSupply::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CLoaderSupply::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: MoveActr(aiLSP_GuidFwBw, ccFwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiLSP_GuidFwBw, ccFwd)) return false;
                 MoveMotr(miLSP_ZElev , piLSP_ZElevWait);
                 Step.iToStart++;
                 return false ;

        case 12: if(!MoveMotr(miLSP_ZElev , piLSP_ZElevWait)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CLoaderSupply::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10: MoveActr(aiLSP_GuidFwBw, ccFwd);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MoveActr(aiLSP_GuidFwBw, ccFwd)) return false;
                 MoveMotr(miLSP_ZElev , piLSP_ZElevWait);
                 Step.iToStop++;
                 return false ;

        case 12: if(!MoveMotr(miLSP_ZElev , piLSP_ZElevWait)) return false ;
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CLoaderSupply::Autorun(void) //오토런닝시에 계속 타는 함수.
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

        // 수정이 필요한 부분입니다. // UU


        bool isCycleSupply =  DM.ARAY[riPRL].CheckAllStat(csNone) && IO_GetX(xLSP_Tray5Sply) && !OM.CmnOptn.bLoadingStop &&
                              DM.ARAY[riLDR].CheckAllStat(csNone) && !LOT.GetRqstLotEnd();
                             //(DM.ARAY[riLDR].GetCntStat  (csEmpty) || DM.ARAY[riLDR].CheckAllStat(csNone)) ;                              //없으면.
        bool isCycleEnd    =  DM.ARAY[riPRL].CheckAllStat(csNone) && (!IO_GetX(xLSP_Tray5Sply) || LOT.GetRqstLotEnd());


        //모르는 스트립에러.
        if(  DM.ARAY[riPRL].CheckAllStat(csNone) && IO_GetX(xLSP_Tray4))
            EM_SetErrMsg(eiPKG_Unknwn , DM.ARAY[riPRL].GetName().c_str());

        //카세트 사라짐.
        if(LDR.GetSeqStep() != CLoader::scIn){
            if( !DM.ARAY[riPRL].CheckAllStat(csNone) &&!IO_GetX(xLSP_Tray4))                   //jh  loader sensor detect supply man
                EM_SetErrMsg(eiPKG_Dispr  , DM.ARAY[riPRL].GetName().c_str());
        }
        //카세트 OUT 매거진 풀


        if ((DM.ARAY[riLDR].CheckAllStat(csNone) && DM.ARAY[riPRL].CheckAllStat(csNone)) &&
            !IO_GetX(xLSP_Tray5Sply) && !LOT.GetRqstLotEnd() && !OM.CmnOptn.bLoadingStop) {
                EM_SetDisp(false);
                EM_SetErr(eiLSP_NeedTray) ;
                m_bNeedTrayErrDisp = true ;
        }

        if(EM_IsErr()) return false ;


         //Normal Decide Step.
             if (isCycleSupply) {Trace(m_sPartName.c_str(),"CycleSupply Stt"); Step.iSeq = scSupply ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;

        
  }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default     End");                                                      Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                             return false ;
        case scSupply: if(CycleSupply()){ Trace(m_sPartName.c_str(),"CycleSupply End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}

bool CLoaderSupply::CycleHome()     //sun DLL direct access.
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

        case 10: AT_MoveCyl(aiLSP_GuidFwBw, ccFwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiLSP_GuidFwBw, ccFwd)) return false ;
                 MT_DoHome(miLSP_ZElev);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miLSP_ZElev)) return false ;
                 MT_GoAbsMan(miLSP_ZElev , PM.GetValue(miLSP_ZElev , pvLSP_ZElevWait ));
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miLSP_ZElev , PM.GetValue(miLSP_ZElev , pvLSP_ZElevWait ))) return false ;
                 Step.iHome = 0;
                 return true ;

    }
}




//One Cycle.
bool CLoaderSupply::CycleSupply(void)
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

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: LOT.SetRqstLotEnd(false); 
                  MoveMotr(miLSP_ZElev , piLSP_ZElevTop);
                  Step.iCycle++;
                  return false ;




        case  11: if(!MoveMotr(miLSP_ZElev , piLSP_ZElevTop)) return false ;
                  MoveActr(aiLSP_GuidFwBw , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveActr(aiLSP_GuidFwBw , ccBwd)) return false ;
                  MoveMotr(miLSP_ZElev,piLSP_ZElevSperate);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miLSP_ZElev,piLSP_ZElevSperate)) return false ;
                  MoveActr(aiLSP_GuidFwBw , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveActr(aiLSP_GuidFwBw , ccFwd)) return false ;
                  MoveMotr(miLSP_ZElev,piLSP_ZElevWait);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveMotr(miLSP_ZElev,piLSP_ZElevWait)) return false ;
                  if(!IO_GetX(xLSP_Tray4)) return false ;
                  m_iTrayCnt++;
                  DM.ARAY[riPRL].SetStat(csUnkwn);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

//---------------------------------------------------------------------------
bool CLoaderSupply::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool bExistPRL = !DM.ARAY[riPRL].CheckAllStat(csNone) ;

    if(_iMotr == miLSP_ZElev){
        switch(_iPstnId) {
            default                   :                 { sMsg = "Not " +AnsiString(MT_GetName(_iMotr))+"'s Postion" ; bRet = false ;} break ;
            case piLSP_ZElevWait      :                                                                                                break ;
            case piLSP_ZElevTop       : if(bExistPRL )  { sMsg = AnsiString(DM.ARAY[riPRL].GetName())+" is Exist"    ; bRet = false ;} break ;
            case piLSP_ZElevSperate   :                                                                                                break ;



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
bool CLoaderSupply::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;
    double dLSP_ZTopOfs = 0.5;
    double dLSP_ZTopPos = PM.GetValue(miLSP_ZElev , pvLSP_ZElevTop) - dLSP_ZTopOfs;

    bool bExistSply = IO_GetX(xLSP_Tray5Sply) ;
    bool bLSP_ZTop  = MT_GetCmdPos(miLSP_ZElev) >= dLSP_ZTopPos;


    if(_iActr == aiLSP_GuidFwBw){
        if(_bFwd == ccBwd) {
            if(bExistSply && !bLSP_ZTop) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
        }
    }
    else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    }


    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr),sMsg.c_str());
    }

    return bRet ;
}

bool CLoaderSupply::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

void CLoaderSupply::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CLoaderSupply::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CLoaderSupply::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CLoaderSupply::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CLoaderSupply::CheckStop()
{
    if(!MT_GetStop(miLSP_ZElev)) return false ;

    if(!AT_Done(aiLSP_GuidFwBw)) return false ;

    return true ;
}

void CLoaderSupply::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";

    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_iTrayCnt" , m_iTrayCnt );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_iTrayCnt" , m_iTrayCnt );
    }
}





