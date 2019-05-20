//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "PostRail.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "LotUnit.h"
#include "UserIni.h"
#include "Loader.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CPostRail PST_F;
CPostRail PST_R;

CPostRail::CPostRail(void)
{

}
CPostRail::~CPostRail (void)
{

}

void CPostRail::Init(bool _bFront)
{
    //m_sPartName = "Rail " ;
    //m_lStrpCycleTime = 0 ;
    //m_lPreCycleTime  = 0 ;

    if(_bFront){
        m_sPartName                 = "Front Post Rail"         ;
        Add.iPartId                 = piPST_F                  ;
//        Add.riPST                   = riPST_F                  ;
        Add.xRAL_MaskDetect         = xRAL_FMaskDetect2        ;
        Add.yRAL_FoldAirBlow        = yRAL_FFoldAirBlow2       ;
        Add.riULD                   = riULD_F                  ;
    }

    else {
        m_sPartName                 = "Rear Post Rail"         ;
        Add.iPartId                 = piPST_R                  ;
//        Add.riPST                   = riPST_R                  ;
        Add.xRAL_MaskDetect         = xRAL_RMaskDetect2        ;
        Add.yRAL_FoldAirBlow        = yRAL_RFoldAirBlow2       ;
        Add.riULD                   = riULD_R                  ;

    }

    m_sCheckSafeMsg = "" ;

    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();

}

void CPostRail::Close()
{
    Load(false);

    Trace("CPostRail", "Close");
}

void CPostRail::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CPostRail::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}

bool CPostRail::FindChip(int &r , int &c )
{
    return false;
}

double CPostRail::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos     = 0.0 ;

    return dPos ;
}

//---------------------------------------------------------------------------
bool CPostRail::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

//    bool isWK1Exist   = DM.ARAY[riWR1].GetCntExist();

//    if(!bRet){
//        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
//        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
//    }


    return bRet ;
}

//---------------------------------------------------------------------------
bool  CPostRail::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    return bRet ;
}

bool CPostRail::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CPostRail::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPostRail::CycleHome()
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


    switch (Step.iHome) {
        default: if(Step.iHome) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iHome = 0 ;
                 return true ;

        case 10: IO_SetY(yRAL_ConMotorCw      , false);
                 IO_SetY(Add.yRAL_FoldAirBlow , false);
                 Step.iHome++;
                 return false ;

        case 11:
                 Step.iHome = 0;
                 return true ;
    }
}

bool CPostRail::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //Clear Timer.
    Stat.bReqStop = true ;

    if (Step.iSeq ) return false;

    Step.iToStop = 10 ;

    //Ok.
    return true;

}

bool CPostRail::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    return true ;
}

bool CPostRail::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiRAL_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart);
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    static bool bWK1_FlpCmpBw = false ;

    //Move Home.
    switch (Step.iToStart) {
        default: if(Step.iToStart) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStart = 0 ;
                 return true ;

        case 10: IO_SetY(yRAL_ConMotorCw , true);
                 Step.iToStart++;
                 return false ;

        case 11: IO_SetY(Add.yRAL_FoldAirBlow, false);
                 Step.iToStart++;
                 return false ;

        case 12:
                 Step.iToStart = 0;
                 return true ;
    }
}

bool CPostRail::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10: IO_SetY(yRAL_ConMotorCw     , false);
                 IO_SetY(Add.yRAL_FoldAirBlow, false);
                 Step.iToStop ++ ;
                 return false ;

        case 11:
                 Step.iToStop = 0 ;
                 return true ;
    }
}


bool CPostRail::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop) return false ;

        bool isCycleWork = IO_GetX(Add.xRAL_MaskDetect);  /* && DM.ARAY[Add.riPST].CheckAllStat(csUnkwn); */
        bool isConEnd     ; //DM.ARAY[Add.riPST].GetCntStat(csNone) ;

        if(EM_IsErr()) return false ;


             if (isCycleWork) { Trace(m_sPartName.c_str(),"CycleWork   Stt"); Step.iSeq = scWork ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isConEnd   ) { Stat.bWorkEnd = true ; return true ; }
        //else if (isConEnd   ) { Stat.bWorkEnd = true ; return true ; }

    }
//    //Cycle.
    switch (Step.iSeq) {
        default        :                  Trace(m_sPartName.c_str(),"default      End"); Step.iSeq = scIdle ;  return false ;
        case scIdle    :                                                                                       return false ;
        case scWork    : if(CycleWork()) { Trace(m_sPartName.c_str(),"CycleWork   End"); Step.iSeq = scIdle ;} return false ;
    }

    return false ;

}
bool CPostRail::CycleWork(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiULD_CycleTO , sTemp.c_str());
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
        //Step.iCycle = 0;
        //return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(Add.yRAL_FoldAirBlow , true);
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  11: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.dSecondAirBlowTime)) return false;
                  IO_SetY(Add.yRAL_FoldAirBlow , false);
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false ;

        case  12: if(!m_tmDelay.OnDelay(true , 3000)) return false ; 
                  //DM.ARAY[Add.riPST].SetStat(csNone);
                  Step.iCycle=0;
                  return true;
    }
}
void CPostRail::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CPostRail::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CPostRail::CheckStop()
{
//    if(!AT_Done(aiWK1_AlgnFwBw )) return false ;

    return true ;
}

void CPostRail::Load(bool IsLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() +".INI";

    //Load Device.
    Trace( "CPostRail","Load Start") ;

    if(IsLoad) {
        //UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedLotNo" , m_sWorkedLotNo);
        //UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedId   " , m_sWorkedId   );
    }

    else {
        //UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedLotNo" , m_sWorkedLotNo);
        //UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedId   " , m_sWorkedId   );
    }
    Trace( "CPostRail","Load End") ;
}

bool CPostRail::GetLastStrip()
{
//    AnsiString sLastLotNo = DM.ARAY[riWR3].GetCntExist() ? DM.ARAY[riWR3].GetLotNo() : m_sWorkedLotNo ;
//
//    bool bLDRExist = !DM.ARAY[riLDR].CheckAllStat(csNone) ;
//    bool bPRBExist = !DM.ARAY[riPRB].CheckAllStat(csNone) ;
//    bool bWR1Exist = !DM.ARAY[riWR1].CheckAllStat(csNone) ;
//    bool bWR2Exist = !DM.ARAY[riWR2].CheckAllStat(csNone) ;
//
//    bool bLDRDiff  = sLastLotNo != DM.ARAY[riLDR].GetLotNo() ;
//    bool bPRBDiff  = sLastLotNo != DM.ARAY[riPRB].GetLotNo() ;
//    bool bWR1Diff  = sLastLotNo != DM.ARAY[riWR1].GetLotNo() ;
//    bool bWR2Diff  = sLastLotNo != DM.ARAY[riWR2].GetLotNo() ;
//
//    bool bLastMgz  = LDR._iMgzCnt >= OM.DevOptn.iLotEndMgzCnt ;
//
//         if( bWR2Exist) return bWR2Diff ;
//    else if( bWR1Exist) return bWR1Diff ;
//    else if( bPRBExist) return bPRBDiff ;
//    else if( bLDRExist) return bLDRDiff ;
//    else if(!bLastMgz ) return false    ;
//    else                return true     ;
}

