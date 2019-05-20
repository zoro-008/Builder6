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
#include "LotUnit.h"
#include "Rs232Man.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff.h"
#include "RailEpx.h"
#include "Epoxy.h"
#include "RailDie.h"
#include "Head.h"
#include "Stage.h"
#include "PostBuff.h"
#include "UnLoader.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------
CManualMan MM;
//---------------------------------------------------------------------------
__fastcall CManualMan::CManualMan(void)
{
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

    DM.ARAY[riSTG].CallBackReg(&SetWaferClickMove);
}

void CManualMan::SetWaferClickMove(int _iCol , int _iRow)
{

    if(!OM.CmnOptn.bUseMapFile || FM_GetLevel() != lvMaster) return ;


    m_iClickMoveCol = _iCol ;
    m_iClickMoveRow = _iRow ;

    SetManCycle(mcSTG_ClickMove);


}

//---------------------------------------------------------------------------
bool __fastcall CManualMan::SetManCycle(EN_MANUAL_CYCLE _iNo)
{
    if( _iNo     <  0                ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( _iNo     >= MAX_MANUAL_CYCLE ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( m_iManNo != mcNoneCycle      ) {FM_MsgOk("ERROR","Doing Manual Cycle"   ); return false ;}
    if(SEQ._bRun                     ) {FM_MsgOk("ERROR","Autorunning"          ); return false ;}
    //if(!MT_GetHomeEndAll()           ) {FM_MsgOk("ERR","초기화를 수행하세요!"   ); return false ;}

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
        if(!IO_GetX(xETC_PowerOn  )    ) {FM_MsgTime("ERR","Power On Plz        ",2000); bRet = false ;}
        if(!IO_GetX(xETC_MainSol  )    ) {FM_MsgTime("ERR","Check Air Sensor Plz",2000); bRet = false ;}
        if(!IO_GetY(yETC_MainSol  )    ) {FM_MsgTime("ERR","Check Air Button Plz",2000); bRet = false ;}
        if( IO_GetX(xPRB_InDetect ) ||
            IO_GetX(xPSB_OutDetect)    ) {FM_MsgTime("ERROR","Check Rail In or Out Sensor!",2000); bRet = false ;}

        if( OM.DevOptn.iLdrCanHomePrbCnt <= DM.ARAY[riPRB].FindLastCol(csUnkwn)){
                                          FM_MsgTime("ERROR","Remove PreBuff Strip!",2000); bRet = false ;}
        if(!MT_GetServoAll()           ) {FM_MsgTime("ERR","Check Servo On"      ,2000); bRet = false ;}
    }
    else if(m_iManNo ==  mcLDR_CyclePlace ){
        if( IO_GetX(xPRB_InDetect )  ) {FM_MsgTime("ERROR","Check Rail In Sensor!",2000); bRet = false ;}

        if( OM.DevOptn.iLdrCanHomePrbCnt <= DM.ARAY[riPRB].FindLastCol(csUnkwn)){
                                          FM_MsgTime("ERROR","Remove PreBuff Strip!",2000); bRet = false ;}
        if(!DM.ARAY[riLDR].CheckAllStat(csNone)){
                 if(LDR._iMgzCnt == 1){ PushTopLotNo(LT.GetCrntLot()); LDR._iMgzCnt = 0 ; }
            else if(LDR._iMgzCnt >  1){                                LDR._iMgzCnt--;    }
        }
    }

    else if(m_iManNo == mcLDR_CyclePitchDn || m_iManNo == mcLDR_CyclePitchUp ){
        if( IO_GetX(xPRB_InDetect )  ) {FM_MsgTime("ERROR","Check Rail In Sensor!",2000); bRet = false ;}

        if( OM.DevOptn.iLdrCanHomePrbCnt <= DM.ARAY[riPRB].FindLastCol(csUnkwn)){
                                          FM_MsgTime("ERROR","Remove PreBuff Strip!",2000); bRet = false ;
        }
    }

    else if (m_iManNo ==  mcLDR_CycleHome      ){
        if( IO_GetX(xPRB_InDetect ) ) {FM_MsgTime("ERROR","Check Rail In Sensor!",2000); bRet = false ;}
        if( OM.DevOptn.iLdrCanHomePrbCnt <= DM.ARAY[riPRB].FindLastCol(csUnkwn)){
                                          FM_MsgTime("ERROR","Remove PreBuff Strip!",2000); bRet = false ;}
    }
    else if (m_iManNo ==  mcULD_CycleHome      ){
        if( IO_GetX(xPSB_OutDetect) ) {FM_MsgTime("ERROR","Check Rail Out Sensor!",2000); bRet = false ;}
    }

    else if(m_iManNo == mcLDR_CyclePick            ){
        if( IO_GetX(xLDR_InDetect ) || IO_GetX(xLDR_MgzDetect1 ) || IO_GetX(xLDR_MgzDetect2 )) {FM_MsgTime("ERROR","Check Mgz Detect Sensor!",2000); bRet = false ;}
        else LDR.InitCycleStep();
    }

    else if(m_iManNo == mcULD_CyclePick            ){
        if( IO_GetX(xULD_InDetect ) || IO_GetX(xULD_MgzDetect1 ) || IO_GetX(xULD_MgzDetect2 )) {FM_MsgTime("ERROR","Check Mgz Detect Sensor!",2000); bRet = false ;}
    }
    else if(m_iManNo == mcULD_CyclePlace           ){
        if( IO_GetX(xPSB_OutDetect) ) {FM_MsgTime("ERROR","Check Rail Out Sensor!",2000); bRet = false ;}
    }
    else if(m_iManNo == mcULD_CyclePitchDn || m_iManNo == mcULD_CyclePitchUp ){
        if( IO_GetX(xPSB_OutDetect) ) {FM_MsgTime("ERROR","Check Rail Out Sensor!",2000); bRet = false ;}
    }


    else if(m_iManNo == mcSTG_CycleStgTAlign        ){
        if(DM.ARAY[riSTG].CheckAllStat(csNone)) {FM_MsgTime("ERROR","Check Stage Map Aray!",2000); bRet = false ;}
    }

    else if(m_iManNo == mcSTG_ClickMove            ){
        if(!STG.GetStartSet() || !STG.GetWfrTAligned() ) {FM_MsgTime("ERROR","Check Stage Start Set Or Align Set !",2000); bRet = false ;}
    }

    if(!bRet) Init();

//***********************************************************************************************
// 처리..
//***********************************************************************************************
         if (m_iManNo == mcAllHome ) {
             LDR.InitHomeStep();
             PRB.InitHomeStep();
             RAE.InitHomeStep();
             EPX.InitHomeStep();
             RAD.InitHomeStep();
             HED.InitHomeStep();
             STG.InitHomeStep();
             PSB.InitHomeStep();
             ULD.InitHomeStep();
             //Home End False
             for(int i=0; i<MAX_MOTR; i++) MT_SetHomeEnd(i,false);
         }

    else if(m_iManNo == mcLDR_CycleHome            ){LDR.InitHomeStep ();}
    else if(m_iManNo == mcLDR_CyclePick            ){LDR.InitCycleStep();}
    else if(m_iManNo == mcLDR_CycleWork            ){LDR.InitCycleStep();}
    else if(m_iManNo == mcLDR_CyclePush            ){LDR.InitCycleStep();}
    else if(m_iManNo == mcLDR_CyclePlace           ){LDR.InitCycleStep();}
    else if(m_iManNo == mcLDR_CyclePitchUp         ){LDR.InitCycleStep();}
    else if(m_iManNo == mcLDR_CyclePitchDn         ){LDR.InitCycleStep();}

    else if(m_iManNo == mcPRB_CycleHome            ){PRB.InitHomeStep ();}
    else if(m_iManNo == mcPRB_CycleWorkStt         ){PRB.InitCycleStep();}
    else if(m_iManNo == mcPRB_CycleWork            ){PRB.InitCycleStep();}
    else if(m_iManNo == mcPRB_CycleReGrip          ){PRB.InitCycleStep();}
    else if(m_iManNo == mcPRB_CycleWorkEnd         ){PRB.InitCycleStep();}

    else if(m_iManNo == mcRAE_CycleHome            ){RAE.InitHomeStep ();}
    else if(m_iManNo == mcRAE_CycleWorkStt         ){RAE.InitCycleStep();}
    else if(m_iManNo == mcRAE_CycleReGrip          ){RAE.InitCycleStep();}
    else if(m_iManNo == mcRAE_CycleWork            ){RAE.InitCycleStep();}
    else if(m_iManNo == mcRAE_CycleWorkEnd         ){RAE.InitCycleStep();}

    else if(m_iManNo == mcEPX_CycleHome            ){EPX.InitHomeStep ();}
    else if(m_iManNo == mcEPX_CycleClean           ){EPX.InitCycleStep();}
    else if(m_iManNo == mcEPX_CycleOri             ){EPX.InitCycleStep();}
    else if(m_iManNo == mcEPX_CycleWork            ){EPX.InitCycleStep();}
    else if(m_iManNo == mcETC_EpoxyOneShot         ){                    }

    else if(m_iManNo == mcETC_ToolChange           ){m_iManStep = 10;}

    else if(m_iManNo == mcRAD_CycleHome            ){RAD.InitHomeStep ();}
    else if(m_iManNo == mcRAD_CycleWorkStt         ){RAD.InitCycleStep();}
    else if(m_iManNo == mcRAD_CycleReGrip          ){RAD.InitCycleStep();}
    else if(m_iManNo == mcRAD_CycleWork            ){RAD.InitCycleStep();}
    else if(m_iManNo == mcRAD_CycleAtVisn          ){RAD.InitCycleStep();}
    else if(m_iManNo == mcRAD_CycleBfVisn          ){RAD.InitCycleStep();}

    else if(m_iManNo == mcRAD_CycleWorkEnd         ){RAD.InitCycleStep();}

    else if(m_iManNo == mcHED_CycleHome            ){HED.InitHomeStep ();}
    else if(m_iManNo == mcHED_CyclePick            ){HED.InitCycleStep();}
    else if(m_iManNo == mcHED_CyclePickNoT         ){HED.InitCycleStep();}
    else if(m_iManNo == mcHED_CyclePlace           ){HED.InitCycleStep();}
    else if(m_iManNo == mcHED_CycleWfrPlace        ){HED.InitCycleStep();}
    else if(m_iManNo == mcHED_CycleWfrCal          ){HED.InitCycleStep();}
    else if(m_iManNo == mcHED_CycleStpCal          ){HED.InitCycleStep();}
    else if(m_iManNo == mcHED_CycleRalPlace        ){HED.InitCycleStep();}

    else if(m_iManNo == mcSTG_CycleHome            ){STG.InitHomeStep ();}
    else if(m_iManNo == mcSTG_CycleCsSupply        ){STG.InitCycleStep();}
    else if(m_iManNo == mcSTG_CycleStgExpend       ){STG.InitCycleStep();}
    else if(m_iManNo == mcSTG_CycleStgVision       ){STG.InitCycleStep();}
    else if(m_iManNo == mcSTG_CycleStgNextMove     ){STG.InitCycleStep();}
    else if(m_iManNo == mcSTG_CycleCsPlace         ){STG.InitCycleStep();}
    else if(m_iManNo == mcSTG_CycleCenterMove      ){m_iManStep = 10;}//STG.InitCycleStep();}
    else if(m_iManNo == mcSTG_CycleCenterMoveT     ){m_iManStep = 10;}
    else if(m_iManNo == mcSTG_CycleUnExpend        ){STG.InitCycleStep();}
    else if(m_iManNo == mcSTG_CycleStartSetMc      ){STG.InitCycleStep();}
    else if(m_iManNo == mcSTG_CycleStartSetEg      ){STG.InitCycleStep();}
    else if(m_iManNo == mcSTG_CycleStgTAlign       ){STG.InitCycleStep();}

    else if(m_iManNo == mcSTG_ClickMove            ){m_iManStep = 10;}

    else if(m_iManNo == mcPSB_CycleHome            ){PSB.InitHomeStep ();}
    else if(m_iManNo == mcPSB_CyclePull            ){PSB.InitCycleStep();}
    else if(m_iManNo == mcPSB_CycleBackOut         ){PSB.InitCycleStep();}

    else if(m_iManNo == mcULD_CycleHome            ){ULD.InitHomeStep ();}
    else if(m_iManNo == mcULD_CyclePick            ){ULD.InitCycleStep();}
    else if(m_iManNo == mcULD_CycleWork            ){ULD.InitCycleStep();}
    else if(m_iManNo == mcULD_CyclePlace           ){ULD.InitCycleStep();}
    else if(m_iManNo == mcULD_CyclePitchUp         ){ULD.InitCycleStep();}
    else if(m_iManNo == mcULD_CyclePitchDn         ){ULD.InitCycleStep();}


    else if(m_iManNo == mcPickPlaceHome            ){m_iManStep = 10;}
    else if(m_iManNo == mcPickPlaceTest            ){m_iManStep = 10;}

    else if(m_iManNo == mcETC_CycleTest            ){m_iManStep = 10;}


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
    if( m_bStop                 ) { m_iManNo = mcNoneCycle; m_bStop = false ; return ; } // Stop 추가 dd 0523 . PM 1h

    if(!OM.CmnOptn.bDoorSkip    ) SEQ.InspectDoor    ();
    if(m_iManNo != mcLDR_CycleHome && m_iManNo != mcLDR_CyclePick &&
       m_iManNo != mcPRB_CycleHome &&
       m_iManNo != mcRAE_CycleHome &&
       m_iManNo != mcEPX_CycleHome &&
       m_iManNo != mcRAD_CycleHome &&
       m_iManNo != mcHED_CycleHome &&
       m_iManNo != mcSTG_CycleHome &&
       m_iManNo != mcPSB_CycleHome &&
       m_iManNo != mcULD_CycleHome && m_iManNo != mcULD_CyclePick &&
       m_iManNo != mcAllHome       &&
       !OM.MstOptn.bDebugMode  ) SEQ.InspectHomeEnd ();
    SEQ.InspectLightGrid();

    bool r0,r1,r2,r3,r4,r5,r6,r7,r8,r9 ;

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return ; }

    //Cycle Step.
    if(m_iManNo == mcAllHome   ) {
        r1 = LDR.CycleHome();
        r2 = PRB.CycleHome();
        r3 = RAE.CycleHome();
        r4 = EPX.CycleHome();
        r5 = RAD.CycleHome();
        r6 = HED.CycleHome();
        r8 = PSB.CycleHome();
        r9 = ULD.CycleHome();

        //
        if(MT_GetHomeEnd(miWRD_ZHed)) {
            r7 = STG.CycleHome();
        }

        //if(!(r3 && r5 && r7 )) return ;
        if(!(r1 && r2 && r3 && r4 && r5 && r6 && r7 && r8 && r9)) return ;

        //r0 = HED.CycleHomeSub() ;

        //if(!r0) return ;


        m_iManNo = mcNoneCycle;
        FM_MsgOk("Confirm","All Home Finished!");
    }

    else if(m_iManNo == mcLDR_CycleHome            ){if(LDR.CycleHome          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_CyclePick            ){if(LDR.CyclePick          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_CycleWork            ){if(LDR.CycleWork          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_CyclePush            ){if(LDR.CyclePush          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_CyclePlace           ){if(LDR.CyclePlace         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_CyclePitchUp         ){if(LDR.CyclePitchUp       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_CyclePitchDn         ){if(LDR.CyclePitchDn       ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcPRB_CycleHome            ){if(PRB.CycleHome          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPRB_CycleWorkStt         ){if(PRB.CycleWorkStt       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPRB_CycleWork            ){if(PRB.CycleWork          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPRB_CycleReGrip          ){if(PRB.CycleReGrip        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPRB_CycleWorkEnd         ){if(PRB.CycleWorkEnd       ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcRAE_CycleHome            ){if(RAE.CycleHome          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRAE_CycleWorkStt         ){if(RAE.CycleWorkStt       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRAE_CycleReGrip          ){if(RAE.CycleReGrip        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRAE_CycleWork            ){if(RAE.CycleWork          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRAE_CycleWorkEnd         ){if(RAE.CycleWorkEnd       ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcEPX_CycleHome            ){if(EPX.CycleHome          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcEPX_CycleClean           ){if(EPX.CycleClean         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcEPX_CycleOri             ){if(EPX.CycleOri           ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcEPX_CycleWork            ){if(EPX.CycleWork          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcETC_EpoxyOneShot         ){   RSM.Shot               (); m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcETC_ToolChange           ){if(ETC_ToolChange         ()) m_iManNo = mcNoneCycle; }


    else if(m_iManNo == mcRAD_CycleHome            ){if(RAD.CycleHome          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRAD_CycleWorkStt         ){if(RAD.CycleWorkStt       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRAD_CycleReGrip          ){if(RAD.CycleReGrip        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRAD_CycleWork            ){if(RAD.CycleWork          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRAD_CycleAtVisn          ){if(RAD.CycleAtVisn        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRAD_CycleBfVisn          ){if(RAD.CycleBfVisn        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRAD_CycleWorkEnd         ){if(RAD.CycleWorkEnd       ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcHED_CycleHome            ){if(HED.CycleHome          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcHED_CyclePick            ){if(HED.CyclePick          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcHED_CyclePickNoT         ){if(HED.CyclePick          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcHED_CyclePlace           ){if(HED.CyclePlace         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcHED_CycleWfrPlace        ){if(HED.CycleWfrPlace      ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcHED_CycleWfrCal          ){if(HED.CyclePickCal       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcHED_CycleStpCal          ){if(HED.CyclePlaceCal      ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcHED_CycleRalPlace        ){if(HED.CycleRalPlace      ()) m_iManNo = mcNoneCycle; }


    else if(m_iManNo == mcSTG_CycleHome            ){if(STG.CycleHome          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTG_CycleCsSupply        ){if(STG.CycleCstSupply     ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTG_CycleStgExpend       ){if(STG.CycleStgExpend     ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTG_CycleStgVision       ){if(STG.CycleStgVision     ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTG_CycleStgNextMove     ){if(STG.CycleStgNextMove   ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTG_CycleCsPlace         ){if(STG.CycleCstPlace      ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTG_CycleUnExpend        ){if(STG.CycleUnExpend      ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTG_CycleCenterMove      ){if(STG_CycleCenterMove    ()) m_iManNo = mcNoneCycle; } //if(STG.CycleCenterMove  ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTG_CycleCenterMoveT     ){if(STG_CycleCenterMove    ()) m_iManNo = mcNoneCycle; } //if(STG.CycleCenterMove  ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTG_CycleStartSetMc      ){if(STG.CycleStgStartSetMc ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTG_CycleStartSetEg      ){if(STG.CycleStgStartSetEg ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTG_CycleStgTAlign       ){if(STG.CycleStgTAlign     ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcSTG_ClickMove            ){if(STG_CycleClickMove     ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcPSB_CycleHome            ){if(PSB.CycleHome          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPSB_CyclePull            ){if(PSB.CyclePull          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPSB_CycleBackOut         ){if(PSB.CycleBackOut       ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcULD_CycleHome            ){if(ULD.CycleHome          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_CyclePick            ){if(ULD.CyclePick          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_CycleWork            ){if(ULD.CycleWork          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_CyclePlace           ){if(ULD.CyclePlace         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_CyclePitchUp         ){if(ULD.CyclePitchUp       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_CyclePitchDn         ){if(ULD.CyclePitchDn       ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcPickPlaceHome            ){if(PickPlaceHome          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPickPlaceTest            ){if(PickPlaceTest          ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcETC_CycleTest            ){if(ETC_CycleTest          ()) m_iManNo = mcNoneCycle; }



    else                               {                         m_iManNo = mcNoneCycle; }

    //Ok.
    return ;
}
bool CManualMan::ETC_ToolChange()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 20000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        m_iManStep = 0 ;
        return true;
    }

    if(m_iManStep != m_iPreManStep) {
        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
    }

    m_iPreManStep = m_iManStep ;

    double dTemp1 = 0.0 ;
    double dTemp2 = 0.0 ;

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case 10:  MT_GoAbsMan(miWRD_ZHed,0.0);
                  MT_GoAbsMan(miWRE_ZEpx,0.0);

                  m_iManStep ++;
                  return false ;

        case 11:  if(!MT_GoAbsMan(miWRD_ZHed,0.0)) return false ;
                  if(!MT_GoAbsMan(miWRE_ZEpx,0.0)) return false ;
                  dTemp1 = MT_GetMaxPos(miWRD_YHed) - 1;
                  dTemp2 = MT_GetMaxPos(miWRE_YEpx) - 1;
                  MT_GoAbsMan(miWRD_YHed,dTemp1);
                  MT_GoAbsMan(miWRE_YEpx,dTemp2);

                  m_iManStep ++;
                  return false;

        case  12: if(!MT_GetStopInpos(miWRD_YHed)) return false ;
                  if(!MT_GetStopInpos(miWRE_YEpx)) return false ;
                  m_iManStep  = 0 ;
                  return true ;

    }

}

bool CManualMan::STG_CycleClickMove()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 20000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        m_iManStep = 0 ;
        return true;
    }

    if(m_iManStep != m_iPreManStep) {
        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
    }

    m_iPreManStep = m_iManStep ;

    double dTemp1 = 0.0 ;
    double dTemp2 = 0.0 ;

    double dMoveX , dMoveY , dMoveT ;

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: if(!OM.CmnOptn.bUseMapFile) {
                      m_iManStep = 0 ;
                      return true;
                  }
                  IO_SetY(ySTG_Dryer  ,false);
                  IO_SetY(yEJT_Suction,false);
                  m_iManStep ++;
                  return false ;

        case  11: if(IO_GetX(xEJT_Vaccum)) return false ;
                  AT_MoveCyl(aiEJT_UpDn,ccBwd);
                  m_iManStep ++;
                  return false;

        case  12: if(!AT_MoveCyl(aiEJT_UpDn,ccBwd)) return false ;

                  MT_GoAbsMan(miWRD_XStg , STG.GetLastPickPosX());
                  MT_GoAbsMan(miWRD_YStg , STG.GetLastPickPosY());

                  m_iManStep++;
                  return false ;

        case  13: if(!MT_GetStop(miWRD_XStg))return false ;
                  if(!MT_GetStop(miWRD_YStg))return false ;


                  STG.CycleStepMove(true , m_iClickMoveRow - STG.GetLastPickRow() , m_iClickMoveCol - STG.GetLastPickCol() , 3);

                  m_iManStep ++;
                  return false;

        case  14: if(!STG.CycleStepMove(false , m_iClickMoveRow - STG.GetLastPickRow() , m_iClickMoveCol - STG.GetLastPickCol() , 3)) return false ;

                  STG.CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  m_iManStep++;
                  return false ;

        case  15: if(!STG.CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;
                  TRetResult RsltWfr  ;
                  SV_GetRslt(viWfr, &RsltWfr);
                  /*
                      veWfrOk       = 0 ,
                      veWfrRctFail  = 1 ,
                      veWfrRct      = 2 ,
                      veWfrAngle    = 3 ,
                      veWfrCrack    = 4 ,
                      veWfrTsdFail  = 5 ,
                      veWfrDot      = 6 ,
                  */




                  if(RsltWfr.iRet == veWfrRctFail || RsltWfr.iRet == veWfrRct) { //사각형 못찾았으면 그냥 무시
                      STG.SetLastPickPosX(MT_GetCmdPos(miWRD_XStg));
                      STG.SetLastPickPosY(MT_GetCmdPos(miWRD_YStg));
                      STG.SetLastPickCol(m_iClickMoveCol);
                      STG.SetLastPickRow(m_iClickMoveRow);
                      m_iManStep  = 0 ;
                      return true ;
                  }

                  //센터칩 찾았으면 보정.
                  STG.GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                                   RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                                   dMoveX                   , dMoveY                   , dMoveT                   ,
                                   false );  //세타보정 씀.

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);
                  m_iManStep++;
                  return false ;

        case  16: if(!MT_GetStop(miWRD_XStg))return false ;
                  if(!MT_GetStop(miWRD_YStg))return false ;

                  //이거 왜 해줬더라 ;;; 일단 삭제.
                  //STG.SetLastPickPosX(MT_GetCmdPos(miWRD_XStg));
                  //STG.SetLastPickPosY(MT_GetCmdPos(miWRD_YStg));
                  //STG.SetLastPickCol(m_iClickMoveCol);
                  //STG.SetLastPickRow(m_iClickMoveRow);
                  m_iManStep  = 0 ;
                  return true ;

    }

}

//---------------------------------------------------------------------------
bool CManualMan::STG_CycleCenterMove()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 20000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        m_iManStep = 0 ;
        return true;
    }

    if(m_iManStep != m_iPreManStep) {
        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
    }

    m_iPreManStep = m_iManStep ;

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: MT_GoAbsMan(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWait));
                  m_iManStep ++;
                  return false;

        case  11: if(!MT_GoAbsMan(miWRD_ZHed,PM.GetValue(miWRD_ZHed,pvWRD_ZHedWait))) return false ;
                  if(MT_GetCmdPos(miWRD_YHed) > PM.GetValue(miWRD_YHed,pvWRD_YHedWaferWait)) {
                      MT_GoAbsMan(miWRD_YHed,PM.GetValue(miWRD_YHed,pvWRD_YHedWaferWait));
                      m_iManStep ++;
                      return false;
                  }
                  else {
                      m_iManStep = 13;
                      return false;
                  }

        case  12: if(!MT_GoAbsMan(miWRD_YHed,PM.GetValue(miWRD_YHed,pvWRD_YHedWaferWait))) return false ;
                  m_iManStep ++;
                  return false;

        case  13: STG.InitCycleStep();
                  m_iManStep ++;
                  return false;

        case  14: if(!STG.CycleCenterMove  ()) return false ;
        //          STG.CycleVisn(viWfr,true,OM.MstOptn.bDryRun);
        //          m_iManStep++;
        //          return false ;
        //
        //case  15: if(!STG.CycleVisn(viWfr,false,OM.MstOptn.bDryRun)) return false ;
                  m_iManStep  = 0 ;
                  return true ;

    }

}

//---------------------------------------------------------------------------
bool CManualMan::PickPlaceHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 20000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        m_iManStep = 0 ;
        return true;
    }

    if(m_iManStep != m_iPreManStep) {
        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
    }

    m_iPreManStep = m_iManStep ;

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: MT_SetServoAll(false);
                  MT_SetServo(miWRD_XHed,true);
                  MT_SetServo(miWRD_YHed,true);
                  MT_SetServo(miWRD_ZHed,true);
                  MT_SetServo(miWRD_XStg,true);
                  MT_SetServo(miWRD_YStg,true);

                  m_iManStep ++;
                  return false;

        case  11:
                  MT_DoHome(miWRD_XHed);
                  MT_DoHome(miWRD_YHed);
                  MT_DoHome(miWRD_ZHed);
                  MT_DoHome(miWRD_XStg);
                  MT_DoHome(miWRD_YStg);

                  m_iManStep ++;
                  return false;

        case  12: if(!MT_GetHomeEnd(miWRD_XHed)) return false ;
                  if(!MT_GetHomeEnd(miWRD_YHed)) return false ;
                  if(!MT_GetHomeEnd(miWRD_ZHed)) return false ;
                  if(!MT_GetHomeEnd(miWRD_XStg)) return false ;
                  if(!MT_GetHomeEnd(miWRD_YStg)) return false ;
                  ShowMessageT("Hed Home Complete");
                  m_iManStep  = 0 ;
                  return true ;

    }

}

bool CManualMan::PickPlaceTest()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        m_iManStep = 0 ;
        return true;
    }

    if(m_iManStep != m_iPreManStep) {
        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
    }

    m_iPreManStep = m_iManStep ;

    static int x1 = 0 ;
    static int y1 = 0 ;

    static int x2 = 0 ;
    static int y2 = 0 ;

    static int iCnt = 0 ;

    double dTempX = 0.0 ;
    double dTempY = 0.0 ;

    const double dWPitchX = 5.5 ;
    const double dWPitchY = 5.5 ;

    static bool bRev = false ;

//    const double dSPitch =

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: x1 = 0 ;
                  y1 = 0 ;

                  x2 = 0 ;
                  y2 = 0 ;

                  iCnt = 0 ;

//                  bRev = false ;

                  AT_MoveCyl(aiEJT_UpDn , ccBwd);
                  MT_GoAbsMan(miWRD_ZHed , PM.GetValue(miWRD_ZHed , pvWRD_ZHedWait));

                  m_iManStep ++;
                  return false;

        case  11: if(!AT_MoveCyl(aiEJT_UpDn , ccBwd)) return false ;
                  if(!MT_GoAbsMan(miWRD_ZHed , PM.GetValue(miWRD_ZHed , pvWRD_ZHedWait))) return false ;
                  m_iManStep ++;
                  return false;


        case  12: 
                  
                  MT_GoAbsMan(miWRD_XStg , PM.GetValue(miWRD_XStg , pvWRD_XStgAlign));
                  MT_GoAbsMan(miWRD_YStg , PM.GetValue(miWRD_YStg , pvWRD_YStgAlign));
                  m_iManStep ++;
                  return false;


        case  13: if(!MT_GoAbsMan(miWRD_XStg , PM.GetValue(miWRD_XStg , pvWRD_XStgAlign))) return false ;
                  if(!MT_GoAbsMan(miWRD_YStg , PM.GetValue(miWRD_YStg , pvWRD_YStgAlign))) return false ;

                  MT_GoAbsMan(miWRD_XHed , PM.GetValue(miWRD_XHed , pvWRD_XHedWafer));
                  MT_GoAbsMan(miWRD_YHed , PM.GetValue(miWRD_YHed , pvWRD_YHedWafer));

                  m_iManStep ++;
                  return false;

        case  14: if(!MT_GoAbsMan(miWRD_XHed , PM.GetValue(miWRD_XHed , pvWRD_XHedWafer))) return false ;
                  if(!MT_GoAbsMan(miWRD_YHed , PM.GetValue(miWRD_YHed , pvWRD_YHedWafer))) return false ;

                  MT_GoAbsMan(miWRD_ZHed , PM.GetValue(miWRD_ZHed , pvWRD_ZHedWfWork));

                  m_iManStep ++;
                  return false;

        case  15:
                  if(!MT_GoAbsMan(miWRD_ZHed , PM.GetValue(miWRD_ZHed , pvWRD_ZHedWfWork))) return false ;

                  MT_GoAbsMan(miWRD_ZHed , PM.GetValue(miWRD_ZHed , pvWRD_ZHedWait));
                  m_iManStep ++;
                  return false;

        case  16: if(!MT_GoAbsMan(miWRD_ZHed , PM.GetValue(miWRD_ZHed , pvWRD_ZHedWait))) return false ;

                  MT_GoAbsMan(miWRD_XHed , PM.GetValue(miWRD_XHed , pvWRD_XHedStrip));
                  MT_GoAbsMan(miWRD_YHed , PM.GetValue(miWRD_YHed , pvWRD_YHedStripStt));

                  m_iManStep ++;
                  return false;

        case  17: if(!MT_GoAbsMan(miWRD_XHed , PM.GetValue(miWRD_XHed , pvWRD_XHedStrip))) return false ;
                  if(!MT_GoAbsMan(miWRD_YHed , PM.GetValue(miWRD_YHed , pvWRD_YHedStripStt))) return false ;
                  MT_GoAbsMan(miWRD_ZHed , PM.GetValue(miWRD_ZHed , pvWRD_ZHedStWork));
                  m_iManStep ++;
                  return false;


        case  18:
                  if(!MT_GoAbsMan(miWRD_ZHed , PM.GetValue(miWRD_ZHed , pvWRD_ZHedStWork))) return false ;

                  MT_GoAbsMan(miWRD_ZHed , PM.GetValue(miWRD_ZHed , pvWRD_ZHedWait));
                  m_iManStep ++;
                  return false;


        case  19: if(!MT_GoAbsMan(miWRD_ZHed , PM.GetValue(miWRD_ZHed , pvWRD_ZHedWait))) return false ;

//                  iCnt ++ ;
//                  dTempX = PM.GetValue(miWRD_XStg , pvWRD_XStgWorkStt) + dWPitchX * iCnt ;
//                  dTempY = PM.GetValue(miWRD_YStg , pvWRD_YStgWorkStt) + dWPitchY * iCnt ;
                  if( iCnt > 10 && !bRev) {
                      bRev = true ;
//                      iCnt = 0 ;
                      dTempX = PM.GetValue(miWRD_XStg , pvWRD_XStgAlign) + dWPitchX * iCnt ;
//                      dTempY = PM.GetValue(miWRD_YStg , pvWRD_YStgWorkStt) + dWPitchY * iCnt ;
                  }
                  if( iCnt <= 0 &&  bRev) {
                      bRev = false ;
                  }
                  if(bRev) {
                      iCnt -- ;
                      dTempX = PM.GetValue(miWRD_XStg , pvWRD_XStgAlign) + dWPitchX * iCnt ;
                  }
                  else {
                      iCnt ++ ;
                      dTempX = PM.GetValue(miWRD_XStg , pvWRD_XStgAlign) + dWPitchX * iCnt ;
                  }


                  MT_GoAbsMan(miWRD_XStg , dTempX);
//                  MT_GoAbsMan(miWRD_YStg , dTempY);
                  m_iManStep = 14 ;
                  return false;


    }

}

bool CManualMan::ETC_CycleTest()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        m_iManStep = 0 ;
        return true;
    }

    if(m_iManStep != m_iPreManStep) {
        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
    }

    m_iPreManStep = m_iManStep ;

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: //MT_GoAbsMan(miWRD_YHed,piWRD_YHedStripWait); //Vision Grab Enable
                  MT_GoAbsMan(miWRD_YHed,PM.GetValue(miWRD_YHed,pvWRD_YHedWaferWait)); //Vision Grab Enable
                  m_iManStep ++;
                  return false;

        case  11: //if(!MT_GoAbsMan(miWRD_YHed,piWRD_YHedWaferWait)) return false ;
                  if(!MT_GoAbsMan(miWRD_YHed,PM.GetValue(miWRD_YHed,pvWRD_YHedStripWait    ))) return false ;

                  m_iManStep =0;
                  return true;



    }

}

