//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ManualMan.h"

#include "SMInterfaceUnit.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "Sequence.h"
#include "PstnMan.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Feed.h"
#include "Rail.h"
#include "Work.h"
#include "Sort.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------
CManualMan MM;

//---------------------------------------------------------------------------
__fastcall CManualMan::CManualMan(void)
{
    //Init. Var.
    Init();
}

//---------------------------------------------------------------------------
__fastcall CManualMan::~CManualMan(void)
{

}

//---------------------------------------------------------------------------
void __fastcall CManualMan::Init(void)
{
    //Init. Var.
    m_iManStep    = 0     ;
    m_iPreManStep = 0     ;
    m_iManNo      = mcNoneCycle ;

//    m_bManSetting = false ;

    m_tmCycle.Clear() ;
}

//---------------------------------------------------------------------------
bool __fastcall CManualMan::SetManCycle(EN_MANUAL_CYCLE _iNo)
{
    if( _iNo     <  0                ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( _iNo     >= MAX_MANUAL_CYCLE ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( m_iManNo != mcNoneCycle      ) {FM_MsgOk("ERROR","Doing Manual Cycle"   ); return false ;}
    if(SEQ._bRun                     ) {FM_MsgOk("ERROR","Autorunning"          ); return false ;}

    //Check Alar
    if ( EM_IsErr()      ) { Init(); return false  ; } //아 밑에 처리 하는 애 때문에 잠시 이렇게 함.

    EM_SetDisp(true);

    m_bManSetting = true ;
    m_iManNo = _iNo ;

    bool bRet = true ;

//***********************************************************************************************
// 프리프로세서
//***********************************************************************************************

    if(m_iManNo == mcAllHome ){
        //if(!IO_GetX(xETC_PowerOn  )    ) {FM_MsgTime("ERR","Power On Plz        ",2000); bRet = false ;}
        if(!IO_GetY(yETC_MainAir  )    ) {FM_MsgTime("ERR","Check Air Button Plz",2000); bRet = false ;}
        //if( IO_GetX(xPRB_InDetect ) ||
        //    IO_GetX(xPSB_OutDetect)    ) {FM_MsgTime("ERROR","Check Rail In or Out Sensor!",2000); bRet = false ;}
    }
//    else if(m_iManNo ==  mcPCK_ReZero){
//        bRet = (FM_MsgYesNo("Confirm","Height Gaze Rezero. Are you Sure?")==mrYes);
//
//    }

//    else if (m_iManNo ==  mcLDR_Home           ){m_iManStep = 10 ; m_iPreManStep = 0;}

    if(!bRet) Init();

//***********************************************************************************************
// 처리..
//***********************************************************************************************
    if (m_iManNo == mcAllHome ) {
        FED.InitHomeStep();
        RAL.InitHomeStep();
        WRK.InitHomeStep();
        SRT.InitHomeStep();
        //Home End False
        for(int i=0; i<MAX_MOTR; i++) MT_SetHomeEnd(i,false);
    }

    else if(m_iManNo == mcFED_CycleFeed            ){FED.InitCycleStep();}

    else if(m_iManNo == mcRAL_Home                 ){RAL.InitHomeStep ();}
    else if(m_iManNo == mcRAL_CycleWorkIdx         ){RAL.InitCycleStep();}

    else if(m_iManNo == mcWRK_Home                 ){WRK.InitHomeStep ();}
    else if(m_iManNo == mcWRK_CycleWork            ){WRK.InitCycleStep();}

    else if(m_iManNo == mcSRT_Home                 ){SRT.InitHomeStep ();}
    else if(m_iManNo == mcSRT_Bin1                 ){SRT.InitCycleStep(); SRT.SetBin(1); }
    else if(m_iManNo == mcSRT_Bin2                 ){SRT.InitCycleStep(); SRT.SetBin(2); }
    else if(m_iManNo == mcSRT_Bin3                 ){SRT.InitCycleStep(); SRT.SetBin(3); }
    else if(m_iManNo == mcSRT_Bin4                 ){SRT.InitCycleStep(); SRT.SetBin(4); }
    else if(m_iManNo == mcSRT_Bin5                 ){SRT.InitCycleStep(); SRT.SetBin(5); }

    else if(m_iManNo == mcAllCycleOne              ){m_iManStep = 10 ; }

    else if(m_iManNo == mcMoveBin3                 ){m_iManStep = 10 ; }
    else if(m_iManNo == mcMoveBin4                 ){m_iManStep = 10 ; }
    else if(m_iManNo == mcMoveBin5                 ){m_iManStep = 10 ; }
    else if(m_iManNo == mcRalEndBlow               ){IO_SetY(yRAL_AirBlower,!IO_GetY(yRAL_AirBlower));}


    else { m_iManNo = mcNoneCycle ; }

    m_bManSetting = false ;
    return true ;
}

EN_MANUAL_CYCLE __fastcall CManualMan::GetManNo()
{
    return m_iManNo ;
}

//---------------------------------------------------------------------------
void __fastcall CManualMan::Update(void)
{
    if( m_iManNo == mcNoneCycle ) return  ;
    if( m_bManSetting           ) return  ;

    //if(!OM.CmnOptn.bDoorSkip    ) SEQ.InspectDoor    ();
    if(m_iManNo != mcRAL_Home &&
       m_iManNo != mcWRK_Home &&
       m_iManNo != mcSRT_Home &&
       m_iManNo != mcAllHome       ) SEQ.InspectHomeEnd ();
    //SEQ.InspectLightGrid();

    bool r1,r2,r3,r4,r5,r6,r7,r8,r9 ;

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return ; }

    //Cycle Step.
    if(m_iManNo == mcAllHome   ) {
        r1 = FED.CycleHome();
        r2 = RAL.CycleHome();
        r3 = WRK.CycleHome();
        r4 = SRT.CycleHome();
        if(r1 && r2 && r3 && r4) {
            m_iManNo = mcNoneCycle;
            FM_MsgOk("Confirm","All Home Finished!");
        }
    }
    else if(m_iManNo == mcFED_CycleFeed            ){if(FED.CycleFeed        ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcRAL_Home                 ){if(RAL.CycleHome        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRAL_CycleWorkIdx         ){if(RAL.CycleMove        ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcWRK_Home                 ){if(WRK.CycleHome        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcWRK_CycleWork            ){if(WRK.CycleWork        ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcSRT_Home                 ){if(SRT.CycleHome        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_Bin1                 ){if(SRT.CycleSort        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_Bin2                 ){if(SRT.CycleSort        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_Bin3                 ){if(SRT.CycleSort        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_Bin4                 ){if(SRT.CycleSort        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_Bin5                 ){if(SRT.CycleSort        ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcAllCycleOne              ){if(CycleOne             ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcMoveBin3                 ){if(CycleMoveBin3        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcMoveBin4                 ){if(CycleMoveBin4        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcMoveBin5                 ){if(CycleMoveBin5        ()) m_iManNo = mcNoneCycle; }

    else                                            {                            m_iManNo = mcNoneCycle; }

    //Ok.
    return ;
}

bool CManualMan::CycleMoveBin3()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        m_iManStep = 0 ;
        return true;
    }

    if(m_iManStep != m_iPreManStep) {
        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
    }

    m_iPreManStep = m_iManStep ;

    bool r1,r2,r3 ;


    //1. 워크존
    //2. 소팅존
    //3. 피딩존
    //4. 레일존

    switch (m_iManStep) {

        default : if(m_iManStep) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s m_iManStep=%02d" , __FUNC__ , m_iManStep );
                      Trace("Manual Man", sTemp.c_str());
                  }
                  m_iManStep = 0 ;
                  return true ;

        case  10: MT_GoAbsMan(miSRT_TSrt , SRT.GetMotrPos(miSRT_TSrt ,piSRT_TSrtBin3 ));
                  m_iManStep++ ;
                  return false ;

        case  11: if(!MT_GetStopInpos(miSRT_TSrt)) return false ;
                  MT_GoAbsMan(miWRK_ZPrb , PM.GetValue(miWRK_ZPrb, pvWRK_ZPrbWait));
                  m_iManStep++ ;
                  return false ;

        case  12: if(!MT_GoAbsMan(miWRK_ZPrb , PM.GetValue(miWRK_ZPrb, pvWRK_ZPrbWait))) return false;
                  MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWait ));
                  AT_MoveCyl(aiRAL_Index, ccFwd);
                  m_iManStep++;
                  return false ;

        case  13: if(!MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWait ))) return false;
                  if(!AT_MoveCyl(aiRAL_Index, ccFwd)) return false;
                  AT_MoveCyl(aiRAL_Index, ccBwd);
                  m_iManStep++;
                  return false ;

        case  14: if(!AT_MoveCyl(aiRAL_Index, ccBwd)) return false;
//                  if(!IO_GetX(xWRK_PkgExist)) SRT.SetBin(3);
                  MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWork ));
                  m_iManStep++;
                  return false ;

        case  15: if(!MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWork ))) return false;
                  MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWait ));
                  AT_MoveCyl(aiRAL_Index, ccFwd);
                  m_iManStep++;
                  return false ;

        case  16: if(!MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWait ))) return false;
                  if(!AT_MoveCyl(aiRAL_Index, ccFwd)) return false;
                  DM.ShiftArrayData(riRAB, riWRK);

                  //if( IO_GetX(xWRK_Detect1)) DM.ARAY[riWRK].SetStat(csUnkwn);
                  //else                       DM.ARAY[riWRK].SetStat(csNone );
                  //DM.ARAY[riRAL].SetStat(csNone);

//                  AT_MoveCyl(aiRAL_IdxUpDn, ccBwd);
//                  m_iManStep++;
//                  return false ;
//
//        case  17: if(!AT_MoveCyl(aiRAL_IdxUpDn, ccBwd)) return false;
//                  if(!IO_GetX(xWRK_PkgExist)) SRT.SetBin(3);
//                  MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxOut ));
//                  m_iManStep++;
//                  return false ;
//
//        case  18: if(!MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxOut ))) return false;
//                  DM.ARAY[riWRK].SetStat(csNone);
//                  DM.ARAY[riRAL].SetStat(csNone);
//                  MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWait ));
//                  AT_MoveCyl(aiRAL_IdxUpDn, ccFwd);

                  m_iManStep = 0 ;
                  return true ;
    }

}

bool CManualMan::CycleMoveBin4()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        m_iManStep = 0 ;
        return true;
    }

    if(m_iManStep != m_iPreManStep) {
        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
    }

    m_iPreManStep = m_iManStep ;

    bool r1,r2,r3 ;


    //1. 워크존
    //2. 소팅존
    //3. 피딩존
    //4. 레일존

    switch (m_iManStep) {

        default : if(m_iManStep) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s m_iManStep=%02d" , __FUNC__ , m_iManStep );
                      Trace("Manual Man", sTemp.c_str());
                  }
                  m_iManStep = 0 ;
                  return true ;

        case  10: MT_GoAbsMan(miSRT_TSrt , SRT.GetMotrPos(miSRT_TSrt ,piSRT_TSrtBin4 ));
                  m_iManStep++ ;
                  return false ;

        case  11: if(!MT_GetStopInpos(miSRT_TSrt)) return false ;
                  MT_GoAbsMan(miWRK_ZPrb , PM.GetValue(miWRK_ZPrb, pvWRK_ZPrbWait));
                  m_iManStep++ ;
                  return false ;

        case  12: if(!MT_GoAbsMan(miWRK_ZPrb , PM.GetValue(miWRK_ZPrb, pvWRK_ZPrbWait))) return false;
                  MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWait ));
                  AT_MoveCyl(aiRAL_Index, ccFwd);
                  m_iManStep++;
                  return false ;

        case  13: if(!MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWait ))) return false;
                  if(!AT_MoveCyl(aiRAL_Index, ccFwd)) return false;
                  AT_MoveCyl(aiRAL_Index, ccBwd);
                  m_iManStep++;
                  return false ;

        case  14: if(!AT_MoveCyl(aiRAL_Index, ccBwd)) return false;
//                  if(!IO_GetX(xWRK_PkgExist)) SRT.SetBin(4);
                  MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWork ));
                  m_iManStep++;
                  return false ;

        case  15: if(!MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWork ))) return false;
                  MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWait ));
                  AT_MoveCyl(aiRAL_Index, ccFwd);
                  m_iManStep++;
                  return false ;

        case  16: if(!MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWait ))) return false;
                  if(!AT_MoveCyl(aiRAL_Index, ccFwd)) return false;
                  DM.ShiftArrayData(riRAB, riWRK);
                  //DM.ARAY[riRAB].SetStat(csNone);

                  //if( IO_GetX(xWRK_Detect1)) DM.ARAY[riWRK].SetStat(csUnkwn);
                  //else                       DM.ARAY[riWRK].SetStat(csNone );
                  //DM.ARAY[riRAL].SetStat(csNone);
//
//
//                  AT_MoveCyl(aiRAL_Index, ccBwd);
//                  m_iManStep++;
//                  return false ;
//
//        case  17: if(!AT_MoveCyl(aiRAL_Index, ccBwd)) return false;
//                  if(!IO_GetX(xWRK_PkgExist)) SRT.SetBin(4);
//                  MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWork ));
//                  m_iManStep++;
//                  return false ;
//
//        case  18: if(!MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWork ))) return false;
//                  DM.ARAY[riWRK].SetStat(csNone);
//                  DM.ARAY[riRAL].SetStat(csNone);
//                  MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWait ));
//                  AT_MoveCyl(aiRAL_Index, ccFwd);

                  m_iManStep = 0 ;
                  return true ;
    }

}

bool CManualMan::CycleMoveBin5()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        m_iManStep = 0 ;
        return true;
    }

    if(m_iManStep != m_iPreManStep) {
        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
    }

    m_iPreManStep = m_iManStep ;

    bool r1,r2,r3 ;


    //1. 워크존
    //2. 소팅존
    //3. 피딩존
    //4. 레일존

    switch (m_iManStep) {

        default : if(m_iManStep) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s m_iManStep=%02d" , __FUNC__ , m_iManStep );
                      Trace("Manual Man", sTemp.c_str());
                  }
                  m_iManStep = 0 ;
                  return true ;

        case  10: MT_GoAbsMan(miSRT_TSrt , SRT.GetMotrPos(miSRT_TSrt ,piSRT_TSrtBin5 ));
                  m_iManStep++ ;
                  return false ;

        case  11: if(!MT_GetStopInpos(miSRT_TSrt)) return false ;
                  MT_GoAbsMan(miWRK_ZPrb , PM.GetValue(miWRK_ZPrb, pvWRK_ZPrbWait));
                  m_iManStep++ ;
                  return false ;

        case  12: if(!MT_GoAbsMan(miWRK_ZPrb , PM.GetValue(miWRK_ZPrb, pvWRK_ZPrbWait))) return false;
                  MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWait ));
                  AT_MoveCyl(aiRAL_Index, ccFwd);
                  m_iManStep++;
                  return false ;

        case  13: if(!MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWait ))) return false;
                  if(!AT_MoveCyl(aiRAL_Index, ccFwd)) return false;
                  AT_MoveCyl(aiRAL_Index, ccBwd);
                  m_iManStep++;
                  return false ;

        case  14: if(!AT_MoveCyl(aiRAL_Index, ccBwd)) return false;
//                  if(!IO_GetX(xWRK_PkgExist)) SRT.SetBin(5);
                  MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWork ));
                  m_iManStep++;
                  return false ;

        case  15: if(!MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWork ))) return false;
                  MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWait ));
                  AT_MoveCyl(aiRAL_Index, ccFwd);
                  m_iManStep++;
                  return false ;

        case  16: if(!MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWait ))) return false;
                  if(!AT_MoveCyl(aiRAL_Index, ccFwd)) return false;
                  DM.ShiftArrayData(riRAB, riWRK);

                  //if( IO_GetX(xWRK_Detect1)) DM.ARAY[riWRK].SetStat(csUnkwn);
                  //else                       DM.ARAY[riWRK].SetStat(csNone );
                  //DM.ARAY[riRAL].SetStat(csNone);

//                  AT_MoveCyl(aiRAL_Index, ccBwd);
//                  m_iManStep++;
//                  return false ;
//
//        case  17: if(!AT_MoveCyl(aiRAL_Index, ccBwd)) return false;
//                  if(!IO_GetX(xWRK_PkgExist)) SRT.SetBin(5);
//                  MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWork ));
//                  m_iManStep++;
//                  return false ;
//
//        case  18: if(!MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWork ))) return false;
//                  DM.ARAY[riWRK].SetStat(csNone);
//                  DM.ARAY[riRAL].SetStat(csNone);
//                  MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWait ));
//                  AT_MoveCyl(aiRAL_Index, ccFwd);

                  m_iManStep = 0 ;
                  return true ;
    }

}

bool CManualMan::CycleOne()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 20000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        m_iManStep = 0 ;
        return true;
    }

    if(m_iManStep != m_iPreManStep) {
        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
    }

    m_iPreManStep = m_iManStep ;

    bool r1,r2,r3 ;


    //1. 워크존
    //2. 소팅존
    //3. 피딩존
    //4. 레일존

    switch (m_iManStep) {

        default : if(m_iManStep) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s m_iManStep=%02d" , __FUNC__ , m_iManStep );
                      Trace("Manual Man", sTemp.c_str());
                  }
                  m_iManStep = 0 ;
                  return true ;

        case  10: if(!WRK.Autorun()) return false ;
                  m_iManStep++ ;
                  return false ;

        case  11: if(!SRT.Autorun()) return false ;
                  m_iManStep++ ;
                  return false ;

        case  12: //if(!FED.Autorun()) return false ;
                  MT_GoAbsMan(miWRK_ZPrb , PM.GetValue(miWRK_ZPrb, pvWRK_ZPrbWait));
                  m_iManStep++;
                  return false ;

        case  13: if(!MT_GoAbsMan(miWRK_ZPrb , PM.GetValue(miWRK_ZPrb, pvWRK_ZPrbWait))) return false;
                  if(!RAL.Autorun()) return false ;
                  m_iManStep = 0 ;
                  return true ;
    }

}

