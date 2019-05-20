//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ManProc.h"
#include "ErrProc.h"
#include "FileManager.h"
#include "Sequence.h"
#include "Function.h"
#include "DAQUnit.h"
#include "SLogUnit.h"

#include "CassetteLoader.h"
#include "CassetteUnloader.h"
#include "ConveyorLoader.h"
#include "ConveyorUnloader.h"
#include "Rail.h"
#include "PreRail.h"
#include "Head.h"


#include "FormCalTInfo.h"
#include "FormCalXYInfo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------
TManProc MAN;

/***************************************************************************/
/* 생성자 & 소멸자. (Constructor & Destructor)                             */
/***************************************************************************/
//---------------------------------------------------------------------------
__fastcall TManProc::TManProc(void)
{
    //Init. User Var.
    m_bRptMotr  = false ;
    m_bRptActr  = false ;
    m_iManStep  = 0     ;
    m_iManNo    = mcNoneCycle ;
    m_iManCycleNo  = mcNoneCycle ;    
    m_bHoming   = false ;
    

    //Init. Var.
    Init();
}

//---------------------------------------------------------------------------
__fastcall TManProc::~TManProc(void)
{

}

/***************************************************************************/
/* Init.                                                                   */
/***************************************************************************/
//---------------------------------------------------------------------------
void __fastcall TManProc::Init(void)
{
    //Init. Var.
    //m_bRptMotr  = false ;
    //m_bRptActr  = false ;
    m_iManStep  = 0     ;
    m_iManNo    = mcNoneCycle ;
    m_iManCycleNo  = mcNoneCycle ;
    m_bHoming   = false ;

    m_ManRunTimer.Clear();

    //Init. Buff.
    memset(&FuncArg, 0 , sizeof(FuncArg));
}

/***************************************************************************/
/* Inheritable Functions.                                                  */
/***************************************************************************/
//---------------------------------------------------------------------------
void __fastcall TManProc::SetAllHomeStep(void)
{

    //Clear Home Timer.
    m_HomeTimer.Clear();

    //Clear Home Step.
//    PR._iHomeStep = 10;
//    CL._iHomeStep = 10;
//    RL._iHomeStep = 10;
//    HD._iHomeStep = 10;
//    CU._iHomeStep = 10;

    RL._iHomeStep = 10;
    HD._iHomeStep = 10;


    if(FM.EqpOptn.iLoaderType == ltConvBelt) {
        VL._iHomeStep = 10;
        VU._iHomeStep = 10;
    }

    else if(FM.EqpOptn.iLoaderType == ltNoLoader) {
        PR._iHomeStep = 10;
        CU._iHomeStep = 10;
    }

    else if(FM.EqpOptn.iLoaderType == lt3Loader) {
        CL._iHomeStep = 10;
        CU._iHomeStep = 10;
    }
}

/***************************************************************************/
/* Direct Accessable Functions.                                            */
/***************************************************************************/
//---------------------------------------------------------------------------
bool __fastcall TManProc::RptMotr(void)
{
    TCamc::TStat  Stat;
    TCamc::TPara  Para;

    MOTR.GetPara(m_iRMotrID , Para);
    MOTR.GetStat(m_iRMotrID , Stat);

    //Check Motor Status.
    if ( Stat.bAlarm  ) {m_bRptMotr = false; UserMsg2("Error" , "모터 알람중입니다."          ); }
    if ( Stat.bCW     ) {m_bRptMotr = false; UserMsg2("Error" , "모터 +리밋센서 감지중입니다."); }
    if ( Stat.bCCW    ) {m_bRptMotr = false; UserMsg2("Error" , "모터 -리밋센서 감지중입니다."); }
    if (!Stat.bHomeEnd) {m_bRptMotr = false; UserMsg2("Error" , "모터 홈을 잡아주세요."       ); }
    if (!Stat.bServo  ) {m_bRptMotr = false; UserMsg2("Error" , "모터 서보온을 해주세요."     ); }

    //Changing Timer.
    m_ChngMotrOnTimer .OnDelay(MOTR.MotnDone(m_iRMotrID , true , Para.dInPos) && !m_bDirMotr , m_iChngMotrDlay);
    m_ChngMotrOffTimer.OnDelay(MOTR.MotnDone(m_iRMotrID , true , Para.dInPos) &&  m_bDirMotr , m_iChngMotrDlay);
    if (m_ChngMotrOnTimer .Out) { MOTR.MoveMotr(m_iRMotrID , m_dP1 , m_dV1 , m_dA1 , false); m_bDirMotr = true ; }
    if (m_ChngMotrOffTimer.Out) { MOTR.MoveMotr(m_iRMotrID , m_dP2 , m_dV2 , m_dA2 , false); m_bDirMotr = false; }
    //Ok.
    return true;
}

//---------------------------------------------------------------------------
bool __fastcall TManProc::RptActr(void)
{
    //Check Motor Status.
    if (ACTR.Err(m_iRActrID)) {m_bRptActr = false; UserMsg2("Error" , "실린더 알람중입니다."  ); }

    //Changing Timer.
    m_ChngActrOnTimer .OnDelay(ACTR.Complete(m_iRActrID , ccBwd) , m_iChngActrDlay);
    m_ChngActrOffTimer.OnDelay(ACTR.Complete(m_iRActrID , ccFwd) , m_iChngActrDlay);
    if (m_ChngActrOnTimer .Out) ACTR.Run(m_iRActrID , ccFwd);
    if (m_ChngActrOffTimer.Out) ACTR.Run(m_iRActrID , ccBwd);
    //Ok.
    return true;
}

//---------------------------------------------------------------------------
bool __fastcall TManProc::MoveAllHome(void)
{
    //Time Out.
    m_HomeTimer.OnDelay(m_bHoming , 40000);
    if (ERR.SetErr(eAllHomeTO , m_HomeTimer.Out))
    {
        m_iManNo  = mcNoneCycle;
        m_bHoming = false;
        return false;
    }

    //Avoid System Down.
    Application -> ProcessMessages();

    //Inspection.
    if (!SEQ.InspectMainAir  ()) { ERR._bUpdatedErrForm = false; m_iManNo = mcNoneCycle; m_bHoming = false; return false; }
    if (!SEQ.InspectSafety   ()) { ERR._bUpdatedErrForm = false; m_iManNo = mcNoneCycle; m_bHoming = false; return false; }
    if (!SEQ.InspectEmergency()) { ERR._bUpdatedErrForm = false; m_iManNo = mcNoneCycle; m_bHoming = false; return false; }
    if (!SEQ.InspectActuator ()) { ERR._bUpdatedErrForm = false; m_iManNo = mcNoneCycle; m_bHoming = false; return false; }

    //Cycle.
    bool R1 ;// = CL.MoveHome();
    bool R2 ;// = RL.MoveHome();
    bool R3 ;// = HD.MoveHome();
    bool R4 ;// = CU.MoveHome();
    bool bAllHomeEnd ;//= MOTR.IsAllHomeEnd();

    //Call ToStop.
    R2 = RL.MoveHome();
    R3 = HD.MoveHome();

    if(FM.EqpOptn.iLoaderType == ltConvBelt) {
        R1 = VL.MoveHome();
        R4 = VU.MoveHome();
    }

    else if(FM.EqpOptn.iLoaderType == ltNoLoader) {
        R1 = PR.MoveHome();
        R4 = CU.MoveHome();
        MOTR.CAMC[miCLD_Z].SetHomeEnd(true);
    }

    else if(FM.EqpOptn.iLoaderType == lt3Loader) {
        R1 = CL.MoveHome();
        R4 = CU.MoveHome();
        MOTR.CAMC[miVLD_Y].SetHomeEnd(true);
        MOTR.CAMC[miVUD_Y].SetHomeEnd(true);
    }

    else if(FM.EqpOptn.iLoaderType == ltNoLdUd) {
        R1 = true ;
        R4 = true ;

        MOTR.CAMC[miVLD_Z].SetHomeEnd(true);
        MOTR.CAMC[miVUD_Z].SetHomeEnd(true);
        MOTR.CAMC[miVLD_Y].SetHomeEnd(true);
        MOTR.CAMC[miVUD_Y].SetHomeEnd(true);

    }

    bAllHomeEnd = MOTR.IsAllHomeEnd();

    //Return.
    m_bHoming = !(R1 && R2 && R3 && R4);

    ERR.SetErr(eAllHoming , m_bHoming);

    if (!m_bHoming && bAllHomeEnd)
    {
        UserMsg2("Confirm" , "모든 모터가 초기화 되었습니다."  );
        m_iManNo = mcNoneCycle;
        return true ;
    }

    return false ;
}
//------------------------------------------------------------------------------
bool __fastcall TManProc::CycleAllCal(void)
{

    //Check Cycle Time Out.
    if (ERR.SetErr(eDHTCycleTO , m_ManRunTimer.OnDelay(m_iManStep > 10  && !FM.MstOptn.bDebugMode , 90000)))
    {
        AnsiString Temp;
        Temp = Temp.sprintf("Manual CycleAllCal CYCLE TIMEOUT STATUS : m_iManStep=%03d" , m_iManStep );
        Trace("SEQ",Temp.c_str());
        m_iManStep = 0 ;
        return false;
    }

    /*
    DM.ARAY[riHDT].SetStat(0,Index , csWork );
    if(DM.ARAY[riDMZ].GetMaxRow() == 1 && DM.ARAY[riDMZ].GetMaxCol() == 3) //3개로 세팅하게 되면 같은 자리만 찍도록 함.
    {
       DM.ARAY[riDMZ].SetStat(csWork);
       DM.ARAY[riDMZ].SetStat(0,Index , csUnkwn);
    }


    */
    if(DM.ARAY[riHDT].GetCnt(fmFail)) {m_iManStep = 0 ; return true;}
    if(DM.ARAY[riDMZ].GetCnt(fmFail)) {m_iManStep = 0 ; return true;}


    //Cycle.
    switch (m_iManStep) {
        case   0: m_iManStep = 0;
                  return false ;

        case  10: HD._iCycleStep = 10 ;
                  m_iManStep++;
                  return false ;

        case  11: if(!HD.CycleClean())return false ;
                  m_iManStep++;
                  return false ;

        case  12: DM.ARAY[riHDT].SetStat(csEmpty );
                  DM.ARAY[riHDT].SetStat(0,1,csWork );
                  HD._iCycleStep = 10;
                  m_iManStep++;
                  return false ;

        case  13: if(!HD.CycleCalZ())return false ;
                  HD._iCycleStep = 10;
                  m_iManStep++;
                  return false ;

        case  14: if(!HD.CycleClean())return false ;
                  DM.ARAY[riHDT].SetStat(0,1,csWork );
                  if(DM.ARAY[riDMZ].GetMaxRow() == 1 && DM.ARAY[riDMZ].GetMaxCol() == 3) //3개로 세팅하게 되면 같은 자리만 찍도록 함.
                  {
                     DM.ARAY[riDMZ].SetStat(csRslt1);
                     DM.ARAY[riDMZ].SetStat(0,1 , csUnkwn);
                  }
                  HD._iCycleStep = 10;
                  m_iManStep++;
                  return false ;

        case  15: if(!HD.CycleCalXY()) return false ;
                  DM.ARAY[riHDT].SetStat(0,0,csWork );

                  HD._iCycleStep = 10;
                  m_iManStep++;
                  return false ;

        case  16: if(!HD.CycleCalZ(true))return false ;
                  HD._iCycleStep = 10;
                  m_iManStep++;
                  return false ;

        case  17: if(!HD.CycleClean())return false ;
                  DM.ARAY[riHDT].SetStat(0,0,csWork );
                  if(DM.ARAY[riDMZ].GetMaxRow() == 1 && DM.ARAY[riDMZ].GetMaxCol() == 3) //3개로 세팅하게 되면 같은 자리만 찍도록 함.
                  {
                     DM.ARAY[riDMZ].SetStat(csRslt1);
                     DM.ARAY[riDMZ].SetStat(0,0 , csUnkwn);
                  }
                  HD._iCycleStep = 10;
                  m_iManStep++;
                  return false ;

        case  18: if(!HD.CycleCalXY()) return false ;
                  DM.ARAY[riHDT].SetStat(0,2,csWork );

                  HD._iCycleStep = 10;
                  m_iManStep++;
                  return false ;

        case  19: if(!HD.CycleCalZ(true))return false ;
                  HD._iCycleStep = 10;
                  m_iManStep++;
                  return false ;

        case  20: if(!HD.CycleClean())return false ;
                  DM.ARAY[riHDT].SetStat(0,2,csWork );
                  if(DM.ARAY[riDMZ].GetMaxRow() == 1 && DM.ARAY[riDMZ].GetMaxCol() == 3) //3개로 세팅하게 되면 같은 자리만 찍도록 함.
                  {
                     DM.ARAY[riDMZ].SetStat(csRslt1);
                     DM.ARAY[riDMZ].SetStat(0,2 , csUnkwn);
                  }
                  HD._iCycleStep = 10;
                  m_iManStep++;
                  return false ;

        case  21: if(!HD.CycleCalXY()) return false ;
                  HD._iCycleStep = 10 ;
                  m_iManStep++;
                  return false ;

        case  22: if(!HD.CycleClean())return false ;
                  m_iManStep=0 ;
                  return true ;
        }
    return false ;
}

bool __fastcall TManProc::CycleInToPb(void) //로더에서 프리버퍼까지 가는 싸이클 로더와 레일의 연동 작업.
{
    //Check Cycle Time Out.
    if (ERR.SetErr(eRBZCycleTO , m_ManRunTimer.OnDelay(m_iManStep > 10  && !FM.MstOptn.bDebugMode , 90000)))
    {
        AnsiString Temp;
        Temp = Temp.sprintf("Manual CycleInToPb CYCLE TIMEOUT STATUS : m_iManStep=%03d" , m_iManStep );
        Trace("SEQ",Temp.c_str());
        m_iManStep = 0 ;
        RL._iCycleStep = 0 ;
        CL._iCycleStep = 0 ;
        VL._iCycleStep = 0 ;
        return false;
    }

    //Cycle.
    switch (m_iManStep) {
        case   0: m_iManStep = 0;
                  return false ;

        case  10:
                       if (FM.EqpOptn.iLoaderType == lt3Loader ){CL._iCycleStep = 10 ; CU._iPreCycleStep = 0; }
                  else if (FM.EqpOptn.iLoaderType == ltConvBelt){VL._iCycleStep = 10 ; VU._iPreCycleStep = 0; }
                  else                                          {m_iManStep = 0; return false ;}
                  RL._iPreCycleStep = 0  ;
                  RL._iCycleStep    = 10 ;

                  m_iManStep++;
                  return false ;

        case  11:      if (FM.EqpOptn.iLoaderType == lt3Loader ) r1 = CL.CycleSuply() ;
                  else if (FM.EqpOptn.iLoaderType == ltConvBelt) r1 = VL.CyclePush () ;

                  r2 = RL.CycleInToPb() ;

                  if(!r1 || !r2) return false ;

                  m_iManStep = 0 ;
                  return true ;
        }
    return false ;
}

bool __fastcall TManProc::CycleWkToOt(void) //워크존에서 언로더 까지 가는 싸이클 언로더와 레일의 연동 작업.
{
    //Check Cycle Time Out.
    if (ERR.SetErr(eRBZCycleTO , m_ManRunTimer.OnDelay(m_iManStep > 10  && !FM.MstOptn.bDebugMode , 90000)))
    {
        AnsiString Temp;
        Temp = Temp.sprintf("Manual CycleInToPb CYCLE TIMEOUT STATUS : m_iManStep=%03d" , m_iManStep );
        Trace("SEQ",Temp.c_str());
        m_iManStep = 0 ;
        RL._iCycleStep = 0 ;
        CU._iCycleStep = 0 ;
        VU._iCycleStep = 0 ;
        return false;
    }

    //Cycle.
    switch (m_iManStep) {
        case   0: m_iManStep = 0;
                  return false ;

        case  10:
                       if (FM.EqpOptn.iLoaderType == lt3Loader ){CU._iCycleStep = 10 ; CU._iPreCycleStep = 0; }
                  else if (FM.EqpOptn.iLoaderType == ltConvBelt){VU._iCycleStep = 10 ; VU._iPreCycleStep = 0; }
                  else                                          {m_iManStep = 0; return false ;}
                  RL._iPreCycleStep = 0  ;
                  RL._iCycleStep    = 10 ;

                  m_iManStep++;
                  return false ;

        case  11:      if (FM.EqpOptn.iLoaderType == lt3Loader ) r1 = CU.CycleStep () ;
                  else if (FM.EqpOptn.iLoaderType == ltConvBelt) r1 = VU.CyclePush () ;

                  r2 = RL.CycleWkToOt() ;

                  if(!r1 || !r2) return false ;

                  m_iManStep = 0 ;
                  return true ;
        }
    return false ;
}

bool __fastcall TManProc::CycleFeedIn(void)
{
    //Check Cycle Time Out.
    if (ERR.SetErr(eRBZCycleTO , m_ManRunTimer.OnDelay(m_iManStep > 10  && !FM.MstOptn.bDebugMode , 90000)))
    {
        AnsiString Temp;
        Temp = Temp.sprintf("Manual CycleFeedIn CYCLE TIMEOUT STATUS : m_iManStep=%03d" , m_iManStep );
        Trace("SEQ",Temp.c_str());
        m_iManStep = 0 ;
        RL._iCycleStep = 0 ;
        return false;
    }

    //Cycle.
    switch (m_iManStep) {
        case   0: m_iManStep = 0;
                  return false ;

        case  10:
                  RL._iPreCycleStep = 0  ;
                  RL._iCycleStep    = 10 ;

                  m_iManStep++;
                  return false ;

        case  11: if(!RL.CyclePbToWk())return false ;
                  RL._iPreCycleStep = 0  ;
                  RL._iCycleStep    = 10 ;

                  m_iManStep++;
                  return false ;

        case  12: if(!RL.CycleRdWk())return false ;
                  m_iManStep = 0 ;
                  return true ;
        }
    return false ;




}

//---------------------------------------------------------------------------
bool bDrngSet = false ;
bool __fastcall TManProc::SetManCycle(EN_MANUAL_CYCLE No )
{
    bDrngSet = true ;

     //   m_iManNo = No ;
    if (No  < 0                 ) { bDrngSet = false ; return false ; }
    if (SEQ._bRun               ) { bDrngSet = false ; return false ; }
    if (m_bRptMotr              ) { bDrngSet = false ; UserMsg2("Error" , "모터 반복중입니다."    ); return false ;}
    if (m_bRptActr              ) { bDrngSet = false ; UserMsg2("Error" , "실린더 반복중입니다."  ); return false ;}
    if (m_iManNo !=  mcNoneCycle) { bDrngSet = false ; return false ; }
    if (!SEQ.InspectMainAir   ()) { bDrngSet = false ; UserMsg2("Error" , "메인에어센서를 확인하세요"); return false ;}

    m_iManNo =(int)No ;

         if (No == mcNoneCycle   ) { return false ;       }
    else if (No == mcAllHome     ) { SetAllHomeStep ();   m_iManNo = No ;}

    else if (No == mcDIS_XHome    ) { MOTR.ClearHomeEnd(miDIS_X); }
    else if (No == mcDIS_YHome    ) { MOTR.ClearHomeEnd(miDIS_Y); }
    else if (No == mcHT1_XHome    ) { MOTR.ClearHomeEnd(miHT1_X); }
    else if (No == mcHT3_XHome    ) { MOTR.ClearHomeEnd(miHT3_X); }
    else if (No == mcHT1_YHome    ) { MOTR.ClearHomeEnd(miHT1_Y); }
    else if (No == mcHT3_YHome    ) { MOTR.ClearHomeEnd(miHT3_Y); }
    else if (No == mcHT1_ZHome    ) { MOTR.ClearHomeEnd(miHT1_Z); }
    else if (No == mcHT2_ZHome    ) { MOTR.ClearHomeEnd(miHT2_Z); }
    else if (No == mcHT3_ZHome    ) { MOTR.ClearHomeEnd(miHT3_Z); }

    if(FM.EqpOptn.iLoaderType == ltConvBelt) {
             if (No == mcVLD_ZHome    ) { MOTR.ClearHomeEnd(miVLD_Z); }
        else if (No == mcVUD_ZHome    ) { MOTR.ClearHomeEnd(miVUD_Z); }
    }
    else {
             if (No == mcCLD_ZHome    ) { MOTR.ClearHomeEnd(miCLD_Z); }
        else if (No == mcCUD_ZHome    ) { MOTR.ClearHomeEnd(miCUD_Z); }
    }
         if (No == mcDH1_ZHome    ) { MOTR.ClearHomeEnd(miDH1_Z); }
    else if (No == mcDH2_ZHome    ) { MOTR.ClearHomeEnd(miDH2_Z); }
    else if (No == mcDH3_ZHome    ) { MOTR.ClearHomeEnd(miDH3_Z); }
    else if (No == mcVLD_YHome    ) { MOTR.ClearHomeEnd(miVLD_Y); }
    else if (No == mcVUD_YHome    ) { MOTR.ClearHomeEnd(miVUD_Y); }

    else if (No == mcCLHome       ) { CL._iHomeStep  = 10;                       }
    else if (No == mcCLSuply      ) { CL._iCycleStep = 10; CL._iPreCycleStep = 0;}

    else if (No == mcRLHome       ) { RL._iHomeStep  = 10;                       }
    else if (No == mcRLToCst      ) { RL._iCycleStep = 10; RL._iPreCycleStep = 0;}
    else if (No == mcRLToBuff2    ) { RL._iCycleStep = 10; RL._iPreCycleStep = 0;}
    else if (No == mcRLMask       ) { RL._iCycleStep = 10; RL._iPreCycleStep = 0;}
    else if (No == mcRLToWork     ) { RL._iCycleStep = 10; RL._iPreCycleStep = 0;}
    else if (No == mcRLToBuff     ) { RL._iCycleStep = 10; RL._iPreCycleStep = 0;}
    else if (No == mcRLHeat       ) { RL._iCycleStep = 10; RL._iPreCycleStep = 0;}
    else if (No == mcRLWork       ) { RL._iCycleStep = 10; RL._iPreCycleStep = 0;}

    else if (No == mcHDHome       ) { HD._iHomeStep  = 10;                       }
    else if (No == mcHDAlign      ) { HD._iCycleStep = 10; HD._iPreCycleStep = 0;}
    else if (No == mcHDProbe      ) { HD._iCycleStep = 10; HD._iPreCycleStep = 0;}
    else if (No == mcHDWork       ) { HD._iCycleStep = 10; HD._iPreCycleStep = 0;}
    else if (No == mcHDVisn       ) { HD._iCycleStep = 10; HD._iPreCycleStep = 0;}
    else if (No == mcHDClean      ) { HD._iCycleStep = 10; HD._iPreCycleStep = 0;}
    else if (No == mcHDCalZ       ) { HD._iCycleStep = 10; HD._iPreCycleStep = 0;}
    else if (No == mcHDCalXY      ) { HD._iCycleStep = 10; HD._iPreCycleStep = 0;}
    else if (No == mcHDPurge      ) { HD._iCycleStep = 10; HD._iPreCycleStep = 0;}
    else if (No == mcHDManWork    ) { HD._iCycleStep = 10; HD._iPreCycleStep = 0;}
    else if (No == mcHDGoToClean  ) { HD._iCycleStep = 10; HD._iPreCycleStep = 0;}
    else if (No == mcHDToolChange ) { HD._iCycleStep = 10; HD._iPreCycleStep = 0;}
    else if (No == mcHDGotoNdlWork) { HD._iCycleStep = 10; HD._iPreCycleStep = 0;}
    else if (No == mcHDPurgeAll   ) { HD._iCycleStep = 10; HD._iPreCycleStep = 0;}
    else if (No == mcHDCalT       ) { HD._iCycleStep = 10; HD._iPreCycleStep = 0;}
    else if (No == mcHDManProbe   ) { HD._iCycleStep = 10; HD._iPreCycleStep = 0;}
    else if (No == mcHDMoveWait   ) { HD._iCycleStep = 10; HD._iPreCycleStep = 0;}

    else if (No == mcCUHome       ) { CU._iHomeStep  = 10;                       }
    else if (No == mcCUStock      ) { CU._iCycleStep = 10; CU._iPreCycleStep = 0;}
    else if (No == mcCUStep       ) { CU._iCycleStep = 10; CU._iPreCycleStep = 0;}

    else if (No == mcMNAllCal     ) { m_iManStep     = 10;}
    else if (No == mcMNInToPb     ) { m_iManStep     = 10;}
    else if (No == mcMNWkToOt     ) { m_iManStep     = 10;}
    else if (No == mcMNFeedIn     ) { m_iManStep     = 10;}

    else if (No == mcLotOpen      ) { RL._iCycleStep = 10; RL._iPreCycleStep = 0;}
    else if (No == mcDummyDispn      ){ HD._iCycleStep = 10; HD._iPreCycleStep = 0;}

    bDrngSet = false ;
    return true ;
}


//---------------------------------------------------------------------------
bool __fastcall TManProc::ManCycleRun(void)
{
    //Check Run Flag.
    if (SEQ._bRun) return true ;
    if (bDrngSet ) return true ;

    //m_iManNo = 0 ;
//    //Master Manual Control. (간섭 확인 없음)
    if (FM.iCrntLevel == lvMaster || FM.iCrntLevel == lvEngineer){
        //Repeat Functions.
        if (m_bRptMotr ) RptMotr ();
        if (m_bRptActr ) RptActr ();
       }
//
//
//    //m_iManNo = 0 ;
    //Check Alarm.
    if (ERR ._bHasErr       ) { Init(); return true; }
    if (MOTR.IsAllServoOff()) { Init(); return true; }

    //Time Out.
    bool isChkCycleTO = (m_iManNo != mcNoneCycle);
    if (m_CycleTimer.OnDelay(isChkCycleTO && !m_bHoming && !FM.MstOptn.bDebugMode &&
                             m_iManNo != mcMNAllCal &&
                             m_iManNo != mcHDCalT
                             , 50000))
    {
        AnsiString Msg;
        Msg = m_iManNo + "\\n" + Msg + "CYCLE TimeOut이 발생 하였습니다.";
        UserMsg2("Error" ,  Msg);
        m_iManNo   = mcNoneCycle;
        m_iManStep = 0          ;
    }

    if((m_iManNo == mcMNInToPb ) ||(m_iManNo == mcMNWkToOt ))RL.AutoRunRoller();


    //Cycle Step.
    if      (m_iManNo == mcNoneCycle    ) {                                           m_bHoming = false;        }
    else if (m_iManNo == mcAllHome      ) { if(MoveAllHome       ()) {                m_iManNo  = mcNoneCycle; }}

///////////////////////////////////
    else if (m_iManNo == mcDIS_XHome    ) { if(HD.MoveMotr (miDIS_X , mcHome)) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcDIS_YHome    ) {
                                            bool a = HD.MoveMotr (miDIS_Y , mcHome) ;
                                            if(a) {
                                                m_iManNo  = mcNoneCycle;
                                                }
                                            }
    else if (m_iManNo == mcHT1_XHome    ) { if(HD.MoveMotr (miHT1_X , mcHome)) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcHT3_XHome    ) { if(HD.MoveMotr (miHT3_X , mcHome)) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcHT1_YHome    ) { if(HD.MoveMotr (miHT1_Y , mcHome)) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcHT3_YHome    ) { if(HD.MoveMotr (miHT3_Y , mcHome)) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcHT1_ZHome    ) { if(HD.MoveMotr (miHT1_Z , mcHome)) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcHT2_ZHome    ) { if(HD.MoveMotr (miHT2_Z , mcHome)) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcHT3_ZHome    ) { if(HD.MoveMotr (miHT3_Z , mcHome)) {                m_iManNo  = mcNoneCycle; }}

    if(FM.EqpOptn.iLoaderType == ltConvBelt) {
             if (m_iManNo == mcVLD_ZHome    ) { if(CL.MoveMotr (miCLD_Z , mcHome)) {                m_iManNo  = mcNoneCycle; }}
        else if (m_iManNo == mcVUD_ZHome    ) { if(CU.MoveMotr (miCUD_Z , mcHome)) {                m_iManNo  = mcNoneCycle; }}
    }
    else {
             if (m_iManNo == mcCLD_ZHome    ) { if(CL.MoveMotr (miCLD_Z , mcHome)) {                m_iManNo  = mcNoneCycle; }}
        else if (m_iManNo == mcCUD_ZHome    ) { if(CU.MoveMotr (miCUD_Z , mcHome)) {                m_iManNo  = mcNoneCycle; }}
    }
         if (m_iManNo == mcDH1_ZHome    ) { if(HD.MoveMotr (miDH1_Z , mcHome)) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcDH2_ZHome    ) { if(HD.MoveMotr (miDH2_Z , mcHome)) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcDH3_ZHome    ) { if(HD.MoveMotr (miDH3_Z , mcHome)) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcVLD_YHome    ) { if(VL.MoveMotr (miVLD_Y , mcHome)) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcVUD_YHome    ) { if(VU.MoveMotr (miVUD_Y , mcHome)) {                m_iManNo  = mcNoneCycle; }}

    else if (m_iManNo == mcCLHome       ) { if(CL.MoveHome       ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcCLSuply      ) { if(CL.CycleSuply     ()) {                m_iManNo  = mcNoneCycle; }}

    else if (m_iManNo == mcRLHome       ) { if(RL.MoveHome       ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcRLToCst      ) { if(RL.CycleWkToOt    ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcRLToBuff2    ) { if(RL.CycleWkToBf2   ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcRLMask       ) { if(RL.CycleMask      ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcRLToWork     ) { if(RL.CyclePbToWk    ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcRLToBuff     ) { if(RL.CycleInToPb    ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcRLHeat       ) { if(RL.CycleHeat      ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcRLWork       ) { if(RL.CycleRdWk      ()) {                m_iManNo  = mcNoneCycle; }}

    else if (m_iManNo == mcHDHome       ) { if(HD.MoveHome       ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcHDAlign      ) { if(HD.CycleAlign     ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcHDProbe      ) { if(HD.CycleProbe     ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcHDWork       ) { if(HD.CycleWork      ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcHDVisn       ) { if(HD.CycleVisn      ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcHDClean      ) { if(HD.CycleClean     ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcHDCalZ       ) { if(HD.CycleCalZ      ()) {                m_iManNo  = mcNoneCycle; DM.ARAY[riHDT].GetCnt(fmFail) ? UserMsg2("Confirm","Z축보정이 실패 했습니다."  ) :  UserMsg2("Confirm","Z축보정이 끝났습니다."  );}}
    else if (m_iManNo == mcHDCalXY      ) { if(HD.CycleCalXY     ()) {
                                                                m_iManNo  = mcNoneCycle;
                                                if(DM.ARAY[riDMZ].GetCnt(fmFail)){
                                                    UserMsg2("Confirm","XY축보정이 실패 했습니다." );
                                                    DM.ARAY[riDMZ].ChangeStat(csFail, csRslt1);
                                                }
                                                else {
                                                    PostMessage(FrmCalXYInfo -> Handle , WM_USER , 0 , 0);
                                                }

                                            }
                                          }
    else if (m_iManNo == mcHDPurge      ) { if(HD.CyclePurge     ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcHDManWork    ) { if(HD.CycleManWork   ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcHDGoToClean  ) { if(HD.CycleGotoClean ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcHDToolChange ) { if(HD.CycleChangeTool()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcHDGotoNdlWork) { if(HD.CycleFrstWork  ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcHDPurgeAll   ) { if(HD.CyclePurgeAll  ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcHDCalT       ) { if(HD.CycleCalT      ()) {
                                                               
                                                m_iManNo  = mcNoneCycle;
                                                PostMessage(FrmCalTInfo -> Handle , WM_USER , 0 , 0);
                                            }
                                          }
    else if (m_iManNo == mcHDManProbe   ) { if(HD.CycleManProbe  ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcHDMoveWait   ) { if(HD.CycleMoveWait  ()) {                m_iManNo  = mcNoneCycle; }}

    else if (m_iManNo == mcCUHome       ) { if(CU.MoveHome       ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcCUStock      ) { if(CU.CycleStock     ()) {                m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcCUStep       ) { if(CU.CycleStep      ()) {                m_iManNo  = mcNoneCycle; }}

    else if (m_iManNo == mcMNAllCal     ) {
                                            if(CycleAllCal       ()) {
                                              m_iManStep = 0; m_iManNo  = mcNoneCycle;
                                              if(DM.ARAY[riDMZ].GetCnt(fmFail)||DM.ARAY[riHDT].GetCnt(fmFail)){
                                                  UserMsg2("Confirm","XY축보정이 실패 했습니다." );
                                                  DM.ARAY[riDMZ].ChangeStat(csFail, csRslt1);
                                              }
                                              else {
                                                  PostMessage(FrmCalXYInfo -> Handle , WM_USER , 0 , 0);
                                              }
                                            }
                                          }
    else if (m_iManNo == mcMNInToPb     ) { if(CycleInToPb         ()) {m_iManStep = 0; m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcMNWkToOt     ) { if(CycleWkToOt         ()) {m_iManStep = 0; m_iManNo  = mcNoneCycle; }}
    else if (m_iManNo == mcMNFeedIn     ) { if(CycleFeedIn         ()) {m_iManStep = 0; m_iManNo  = mcNoneCycle; }}

    else if (m_iManNo == mcLotOpen      ) { if(RL.CycleLotOpenInit ()) {                m_iManNo  = mcNoneCycle; }}            //랏오픈시 실린더 초기화
    else if (m_iManNo == mcDummyDispn   ) { if(HD.CycleDummyDispn  ()) {                m_iManNo  = mcNoneCycle; }}

    //Ok.
    return true;
}



