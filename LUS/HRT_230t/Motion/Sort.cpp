//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Sort.h"
#include "Rail.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "LotUnit.h"
#include "ManualMan.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CSort SRT;

CSort::CSort()
{
    Init();
}
CSort::~CSort (void)
{
    Close();
}
void CSort::Init()
{
    m_sPartName   = "Sort " ;

    m_iSetBin  = 0 ; //Wait Bin
    m_bSetSort = false ;
    bNeedDetect= false ;
    m_iCrntBin = 0 ; //
    m_bUseDetect = false ;
    bFirstPkg    = false ;
    bSortEnd     = true  ;

    m_iHomeTO  = (int)eiSRT_HomeTO    ;
    m_iStartTO = (int)eiSRT_ToStartTO ;
    m_iStopTO  = (int)eiSRT_ToStopTO  ;
    m_iCycleTO = (int)eiSRT_CycleTO   ;

    m_iBin1Cnt = 0 ;
    m_iBin2Cnt = 0 ;
    m_iBin3Cnt = 0 ;
    m_iBin4Cnt = 0 ;
    m_iBin5Cnt = 0 ;

    Reset();
    Load(true);
}
void CSort::Close()
{
    Load(false);
}
void CSort::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CSort::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();

}

bool CSort::FindChip(EN_ARAY_ID _riWhre , int &r , int &c )
{
    //int r1,c1;
    //if(_riWhre == riRAE) {
    //    c = DM.ARAY[riRAE].FindLastCol(csUnkwn) ;
    //    return (c > -1 && c > -1);
    //}
    return false ;
}

double CSort::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    double dPosBin1 = PM.GetValue (_iMotr , pvSRT_TSrtBin1 ) + 0 * PM.GetValue (_iMotr , poSRT_TSrtPitch ) ;
    double dPosBin2 = PM.GetValue (_iMotr , pvSRT_TSrtBin1 ) + 1 * PM.GetValue (_iMotr , poSRT_TSrtPitch ) ;
    double dPosBin3 = PM.GetValue (_iMotr , pvSRT_TSrtBin1 ) + 2 * PM.GetValue (_iMotr , poSRT_TSrtPitch ) ;
    double dPosBin4 = PM.GetValue (_iMotr , pvSRT_TSrtBin1 ) + 3 * PM.GetValue (_iMotr , poSRT_TSrtPitch ) ;
    double dPosBin5 = PM.GetValue (_iMotr , pvSRT_TSrtBin1 ) + 4 * PM.GetValue (_iMotr , poSRT_TSrtPitch ) ;

    if(_iMotr == miSRT_TSrt){
        switch(_iPstnId) {
            default             : dPos = MT_GetCmdPos(_iMotr                  ); break ;
            case piSRT_TSrtWait : dPos = PM.GetValue (_iMotr , pvSRT_TSrtWait ); break ;
            case piSRT_TSrtBin1 : dPos = dPosBin1 ;                              break ;
            case piSRT_TSrtBin2 : dPos = dPosBin2 ;                              break ;
            case piSRT_TSrtBin3 : dPos = dPosBin3 ;                              break ;
            case piSRT_TSrtBin4 : dPos = dPosBin4 ;                              break ;
            case piSRT_TSrtBin5 : dPos = dPosBin5 ;                              break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CSort::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iMotr == miSRT_TSrt){
        //sMsg = "Epoxy_Z Axis is Not Wait Position" ;
        //bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CSort::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool isTMoving = !MT_GetStopInpos(miSRT_TSrt);

    //if(_iActr == aiSRT_IdxUpDn ){
    //    //sMsg = "Epoxy_X_Index is Moving" ;
    //    //bRet = false ;
    //}

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}

bool CSort::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CSort::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CSort::CycleHome()     //sun DLL direct access.
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

        case 10: MT_Reset(miSRT_TSrt);
                 MT_SetServo(miSRT_TSrt,true);
                 Step.iHome++;
                 return false ;

        case 11: MT_DoHome(miSRT_TSrt);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miSRT_TSrt))return false ;
                 MT_GoAbsMan(miSRT_TSrt , PM.GetValue(miSRT_TSrt , pvSRT_TSrtWait));
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miSRT_TSrt , PM.GetValue(miSRT_TSrt , pvSRT_TSrtWait))) return false ;
                 m_iCrntBin = 0 ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CSort::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CSort::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CSort::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: //bFirstPkg = false ;
//                 bSucDetect = true ;
                 bSortEnd = true ;
//                 Stat.bSortDetect = false ;
//                 bNeedDetect= false ;
                 Step.iToStart ++ ;
                 return false ;

        case 11: //if(m_iSetBin < 1 || m_iSetBin > 5) m_iSetBin = 5 ;
                 //
                 //     if(m_iSetBin == 1) MoveMotr(miSRT_TSrt , piSRT_TSrtBin1 );
                 //else if(m_iSetBin == 2) MoveMotr(miSRT_TSrt , piSRT_TSrtBin2 );
                 //else if(m_iSetBin == 3) MoveMotr(miSRT_TSrt , piSRT_TSrtBin3 );
                 //else if(m_iSetBin == 4) MoveMotr(miSRT_TSrt , piSRT_TSrtBin4 );
                 //else if(m_iSetBin == 5) MoveMotr(miSRT_TSrt , piSRT_TSrtBin5 );
                 //else                    MoveMotr(miSRT_TSrt , piSRT_TSrtBin3 );

                 MoveMotr(miSRT_TSrt , piSRT_TSrtBin3 );

                 Step.iToStart++;
                 return false;

        case 12: if(!MT_GetStop(miSRT_TSrt)) return false ;
//                 m_iCrntBin = m_iSetBin ;
//                 m_bSetSort = false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CSort::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10:
                 Step.iToStop ++ ;
                 return false ;

        case 11: Step.iToStop = 0 ;
                 return true ;
    }
}

bool CSort::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        //Judgment
        bool isCycleSort   = !bSortEnd ;//!Stat.bSortDetect ;//(m_bSetSort || m_bSetSortSub ) && !Stat.bSortDetect  ; //|| m_iSetBin != m_iCrntBin;//Stat.bSortMove && Stat.bSortDetect;//!MT_CmprPos(miSRT_TSrt,GetMotrPos(miSRT_TSrt,(EN_PSTN_ID)m_iSetBin ));//Stat.bSortMove; //m_iSetBin != m_iCrntBin ;
        bool isCycleDetect =  false ; //Stat.bSortDetect ;//&& m_bUseDetect ; //!Stat.bSortDetect;
        bool isConEnd      = !isCycleSort && !isCycleDetect;


        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleSort   ) {Trace(m_sPartName.c_str(),"CycleSort    Stt"); Step.iSeq = scSort   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleDetect ) {Trace(m_sPartName.c_str(),"CycleDetect  Stt"); Step.iSeq = scDetect ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd      ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default       :                    Trace(m_sPartName.c_str(),"default     End");Step.iSeq = scIdle ;  return false ;
        case scIdle   :                                                                                       return false ;
        case scSort   : if(CycleSort  ()){ Trace(m_sPartName.c_str(),"CycleSort   End");Step.iSeq = scIdle ;} return false ;
        case scDetect : if(CycleDetect()){ Trace(m_sPartName.c_str(),"CycleDetect End");Step.iSeq = scIdle ;} return false ;
    }
}

//One Cycle.
bool CSort::CycleSort(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
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
    //    return true;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;


        case  11: //if(m_tmTemp.OnDelay(Stat.bSortDetect , OM.DevOptn.iSortDeley )){
                  //    EM_SetErr(eiSRT_PkgNotDetected);
                  //    Stat.bSortDetect = false;
                  //    m_bSetSort = false ;
                  //    Step.iCycle = 0 ;
                  //    return true ;
                  //}

                  Step.iCycle++;
                  return false;

        case  12:
                       if(m_iSetBin == 1) {MoveMotr(miSRT_TSrt , piSRT_TSrtBin1 ); Trace(m_sPartName.c_str(),"*****1111*********"); }
                  else if(m_iSetBin == 2) {MoveMotr(miSRT_TSrt , piSRT_TSrtBin2 ); Trace(m_sPartName.c_str(),"*****2222*********"); }
                  else if(m_iSetBin == 3) {MoveMotr(miSRT_TSrt , piSRT_TSrtBin3 ); Trace(m_sPartName.c_str(),"*****3333*********"); }
                  else if(m_iSetBin == 4) {MoveMotr(miSRT_TSrt , piSRT_TSrtBin4 ); Trace(m_sPartName.c_str(),"*****4444*********"); }
                  else if(m_iSetBin == 5) {MoveMotr(miSRT_TSrt , piSRT_TSrtBin5 ); Trace(m_sPartName.c_str(),"*****5555********"); }
                  else                    {MoveMotr(miSRT_TSrt , piSRT_TSrtBin3 ); Trace(m_sPartName.c_str(),"******6666********"); }

                  Step.iCycle++;
                  return false;

        case  13: if(!MT_GetStop(miSRT_TSrt)) return false ;
                  IO_SetY(yRAL_Blower,true);
                  m_iCrntBin = m_iSetBin ;
//                  else {
                      m_tmTemp.Clear();
                      Step.iCycle++;
                      return false;
//                  }

        case  14: if(m_tmTemp.OnDelay(true , 4000)){//OM.DevOptn.iSortDeley )){
                      EM_SetErr(eiSRT_PkgNotDetected);
                      IO_SetY(yRAL_Blower,false);
                      bSortEnd = true ;
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  if(MM.GetManNo() == mcSRT_Bin1 || MM.GetManNo() == mcSRT_Bin2 || MM.GetManNo() == mcSRT_Bin3 ||
                     MM.GetManNo() == mcSRT_Bin4 || MM.GetManNo() == mcSRT_Bin5 ) {
                     IO_SetY(yRAL_Blower,false);
                     bSortEnd = true ;
                     Step.iCycle = 0 ;
                     return true ;
                  }

                  if(m_iCrntBin == OM.DevOptn.iEpmtyBin ) {
                      bSortEnd = true ;
                      IO_SetY(yRAL_Blower,false);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  if(!IO_GetXDn(xSRT_PkgDetect) && !IO_GetXUp(xSRT_PkgDetect)) return false ;
                  if(m_iCrntBin == 1) {m_iBin1Cnt += 1 ; LT.AddDayInfoChips(1,0,0,0,0); LT.AddLotInfoChips(1,0,0,0,0);}
                  if(m_iCrntBin == 2) {m_iBin2Cnt += 1 ; LT.AddDayInfoChips(0,1,0,0,0); LT.AddLotInfoChips(0,1,0,0,0);}
                  if(m_iCrntBin == 3) {m_iBin3Cnt += 1 ; LT.AddDayInfoChips(0,0,1,0,0); LT.AddLotInfoChips(0,0,1,0,0);}
                  if(m_iCrntBin == 4) {m_iBin4Cnt += 1 ; LT.AddDayInfoChips(0,0,0,1,0); LT.AddLotInfoChips(0,0,0,1,0);}
                  if(m_iCrntBin == 5) {m_iBin5Cnt += 1 ; LT.AddDayInfoChips(0,0,0,0,1); LT.AddLotInfoChips(0,0,0,0,1);}
                  IO_SetY(yRAL_Blower,false);
                  bSortEnd = true ;
                  Step.iCycle = 0 ;
                  return true ;

    }
}

bool CSort::CycleDetect(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode /*&& bNeedDetect*/, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
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
    //    return true;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //bSucDetect = false ;
                  //if(OM.DevOptn.bSortPKGSensor) {
                  m_tmTemp.Clear();
//                  if(m_iCrntBin == 3 && bNeedDetect) {
//                      Step.iCycle = 12;
//                      return true ;
//                  }
                  if(m_iCrntBin == 3){
                      Step.iCycle = 0;
                      return true;
                  }

                  Step.iCycle++;
                  return false;
                  //}
                  //else {
                  //    m_iCrntBin = m_iSetBin ;
                  //    m_bSetSort = false ;
                  //    Step.iCycle = 0;
                  //    return true;
                  //}

        case  11: if(m_tmTemp.OnDelay(true , OM.DevOptn.iSortDeley )) {
//                      m_iCrntBin = m_iSetBin ;
                  //    EM_SetErr(eiSRT_PkgNotDetected);
                  //    Stat.bSortDetect = false;
                      bSucDetect  = false;
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  if(!IO_GetXDn(xSRT_PkgDetect)) return false ;
//                  bFirstPkg =true ;
                  bSucDetect=true ;
                  Stat.bSortDetect = false;
                  bNeedDetect=false ;
                  Step.iCycle = 0;
                  return true ;

        case  12: if(!m_tmTemp.OnDelay(bNeedDetect , OM.DevOptn.iBin3Deley )) return false;
                  bSucDetect=true ;
                  Stat.bSortDetect = false;
                  bNeedDetect =false;
                  Step.iCycle = 0;
                  return true ;

    }
}

double CSort::GetLastCmd(EN_MOTR_ID _iMotr)
{
    double _dLastIdxPos = 0.0 ;
    if(!MT_GetAlarm(_iMotr) && !MT_GetNLimSnsr(_iMotr) && !MT_GetPLimSnsr(_iMotr) ) _dLastIdxPos = MT_GetCmdPos(_iMotr) ;
    else                                                                            _dLastIdxPos = GetMotrPos  (_iMotr , (EN_PSTN_ID)0) ;

    return _dLastIdxPos ;
}

bool CSort::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CSort::CheckStop()
{
    if(!MT_GetStop(miSRT_TSrt)) return false ;

    //if(!AT_Done(aiSRT_IdxUpDn )) return false ;

    return true ;
}

void CSort::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() + ".INI";

    if ( IsLoad ) {
        UserINI.Load(sPath, "Member" , "m_iBin1Cnt" , m_iBin1Cnt );
        UserINI.Load(sPath, "Member" , "m_iBin2Cnt" , m_iBin2Cnt );
        UserINI.Load(sPath, "Member" , "m_iBin3Cnt" , m_iBin3Cnt );
        UserINI.Load(sPath, "Member" , "m_iBin4Cnt" , m_iBin4Cnt );
        UserINI.Load(sPath, "Member" , "m_iBin5Cnt" , m_iBin5Cnt );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_iBin1Cnt" , m_iBin1Cnt );
        UserINI.Save(sPath, "Member" , "m_iBin2Cnt" , m_iBin2Cnt );
        UserINI.Save(sPath, "Member" , "m_iBin3Cnt" , m_iBin3Cnt );
        UserINI.Save(sPath, "Member" , "m_iBin4Cnt" , m_iBin4Cnt );
        UserINI.Save(sPath, "Member" , "m_iBin5Cnt" , m_iBin5Cnt );
    }
}
//---------------------------------------------------------------------------

void CSort::SetBin (int _iBinNo , bool _bWriteData)//, bool _bUseDetect)
{
    if(m_iSetBin < 1 || m_iSetBin > 5) m_iSetBin = 5 ;

//    if(m_bSetSort) m_bSetSortSub = true ;
//    m_bSetSort = true ;
    bSortEnd   = false ;
    m_iSetBin = _iBinNo ;

    if(_bWriteData) {
        if(_iBinNo == 1) {m_iBin1Cnt += 1 ; LT.AddDayInfoChips(1,0,0,0,0); LT.AddLotInfoChips(1,0,0,0,0);}
        if(_iBinNo == 2) {m_iBin2Cnt += 1 ; LT.AddDayInfoChips(0,1,0,0,0); LT.AddLotInfoChips(0,1,0,0,0);}
        if(_iBinNo == 3) {m_iBin3Cnt += 1 ; LT.AddDayInfoChips(0,0,1,0,0); LT.AddLotInfoChips(0,0,1,0,0);}
        if(_iBinNo == 4) {m_iBin4Cnt += 1 ; LT.AddDayInfoChips(0,0,0,1,0); LT.AddLotInfoChips(0,0,0,1,0);}
        if(_iBinNo == 5) {m_iBin5Cnt += 1 ; LT.AddDayInfoChips(0,0,0,0,1); LT.AddLotInfoChips(0,0,0,0,1);}
    }
    //m_bUseDetect = _bUseDetect ;

}
void CSort::SetSort (bool _bRet)
{
    m_bSetSort  = _bRet ;
}

int CSort::GetBin ()
{
    return m_iSetBin  ;
}
int CSort::GetCntBin ()
{
    return m_iCrntBin ;
}


int CSort::GetBinCnt (int _iBinNo)
{
    if(_iBinNo == 1) return m_iBin1Cnt ;
    if(_iBinNo == 2) return m_iBin2Cnt ;
    if(_iBinNo == 3) return m_iBin3Cnt ;
    if(_iBinNo == 4) return m_iBin4Cnt ;
    if(_iBinNo == 5) return m_iBin5Cnt ;
}

void CSort::AddBinCnt (int _iBinNo , int _iCnt)
{
    if(_iBinNo == 1) {m_iBin1Cnt += _iCnt ; LT.AddDayInfoChips(1,0,0,0,0); LT.AddLotInfoChips(1,0,0,0,0);}
    if(_iBinNo == 2) {m_iBin2Cnt += _iCnt ; LT.AddDayInfoChips(0,1,0,0,0); LT.AddLotInfoChips(0,1,0,0,0);}
    if(_iBinNo == 3) {m_iBin3Cnt += _iCnt ; LT.AddDayInfoChips(0,0,1,0,0); LT.AddLotInfoChips(0,0,1,0,0);}
    if(_iBinNo == 4) {m_iBin4Cnt += _iCnt ; LT.AddDayInfoChips(0,0,0,1,0); LT.AddLotInfoChips(0,0,0,1,0);}
    if(_iBinNo == 5) {m_iBin5Cnt += _iCnt ; LT.AddDayInfoChips(0,0,0,0,1); LT.AddLotInfoChips(0,0,0,0,1);}

}

void CSort::ClearBinCnt(int _iBinNo)
{
    if(_iBinNo == 1) m_iBin1Cnt = 0 ;
    if(_iBinNo == 2) m_iBin2Cnt = 0 ;
    if(_iBinNo == 3) m_iBin3Cnt = 0 ;
    if(_iBinNo == 4) m_iBin4Cnt = 0 ;
    if(_iBinNo == 5) m_iBin5Cnt = 0 ;
}


