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
#include "Loader.h"
#include "Rail.h"
#include "UnLoader.h"
#include "Picker.h"
#include "Sorter.h"
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

//    if(_iNo  == mcAllHome  && ( IO_GetX(xPRB_PkgIn1 ) ||  IO_GetX(xPSB_PkgOut3) || IO_GetX(xPSB_PkgOut4))) {FM_MsgOk("ERROR","Check Rail In or Out Sensor!"); return false ;}

    EM_SetDisp(true);

    m_bManSetting = true ;
    m_iManNo = _iNo ;

    bool bRet = true ;

//***********************************************************************************************
// 프리프로세서
//***********************************************************************************************

    if(m_iManNo == mcAllHome ){
        if(!IO_GetX(xETC_PowerOn )) {FM_MsgOk("ERR","Power On Plz"        ); bRet = false ;}
        if(!IO_GetX(xETC_MainAir )) {FM_MsgOk("ERR","Check Air Sensor Plz"); bRet = false ;}
    }
    else if(m_iManNo ==  mcPCK_ReZero){

        for(int i = 0 ; i < MAX_PICKER_CNT ; i++) {
            if(DM.ARAY[riPCK].GetStat(0,i) != csNone) { FM_MsgOk("Error","Remove Pkg Data.") ; bRet = false ;}
        }

        if(IO_GetX(xPCK_Vacuum1) || IO_GetX(xPCK_Vacuum1) || IO_GetX(xPCK_Vacuum1)) { FM_MsgOk("Error","Check Picker Vaccum Sensor!") ; bRet = false ;}

        if(FM_MsgYesNo("Confirm","Height Gauze Rezero. Are you Sure?")!=mrYes) bRet = false ;
    }

    else if (m_iManNo ==  mcPCK_Station           ){
        if(IO_GetX(xSRT_StnDetect1) || IO_GetX(xSRT_StnDetect2) || IO_GetX(xSRT_StnDetect3)) { FM_MsgOk("Error","Pkg Detected on Station!") ; bRet = false ;}
        if(IO_GetX(xPCK_Vacuum1   ) || IO_GetX(xPCK_Vacuum1   ) || IO_GetX(xPCK_Vacuum1   )) { FM_MsgOk("Error","Pkg Detected on Picker!" ) ; bRet = false ;}



    }

    if(!bRet) Init();

//***********************************************************************************************
// 처리..
//***********************************************************************************************
         if (m_iManNo == mcAllHome ) {
             LDR.InitHomeStep();
             RAL.InitHomeStep();
             ULD.InitHomeStep();
             PCK.InitHomeStep();
             SRT.InitHomeStep();
             //SPL.InitHomeStep();
             //SPR.InitHomeStep();
             //Home End False
             for(int i=0; i<MAX_MOTR; i++) MT_SetHomeEnd(i,false);
         }
    else if(m_iManNo ==  mcLDR_Home       ){LDR.InitHomeStep ();}
    else if(m_iManNo ==  mcLDR_Supply     ){LDR.InitCycleStep();}
    else if(m_iManNo ==  mcRAL_Home       ){RAL.InitHomeStep ();}
    else if(m_iManNo ==  mcRAL_In         ){RAL.InitCycleStep();}
    else if(m_iManNo ==  mcRAL_WorkStt    ){RAL.InitCycleStep();}
    else if(m_iManNo ==  mcRAL_Work       ){RAL.InitCycleStep();}
    else if(m_iManNo ==  mcRAL_Out        ){RAL.InitCycleStep();}
    else if(m_iManNo ==  mcULD_Home       ){ULD.InitHomeStep ();}
    else if(m_iManNo ==  mcULD_In         ){ULD.InitCycleStep();}
    else if(m_iManNo ==  mcULD_Stock      ){ULD.InitCycleStep();}
    else if(m_iManNo ==  mcPCK_Home       ){PCK.InitHomeStep ();}
    else if(m_iManNo ==  mcPCK_Pick       ){PCK.InitCycleStep();}
    else if(m_iManNo ==  mcPCK_Place      ){PCK.InitCycleStep();}
    else if(m_iManNo ==  mcPCK_ReZero     ){PCK.InitCycleStep();}
    else if(m_iManNo ==  mcPCK_Station    ){PCK.InitCycleStep();}
    else if(m_iManNo ==  mcPCK_ReZeroCheck){PCK.InitCycleStep();}
    else if(m_iManNo ==  mcSRT_Home       ){SRT.InitHomeStep ();}
    else if(m_iManNo ==  mcSRT_Pick       ){SRT.InitCycleStep();}
    else if(m_iManNo ==  mcSRT_Sort       ){SRT.InitCycleStep();}

    else                                   { m_iManNo = mcNoneCycle ; }

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

//    if(!OM.CmnOptn.bDoorSkip    ) SEQ.InspectDoor    ();
    if(m_iManNo != mcLDR_Home &&
       m_iManNo != mcRAL_Home &&
       m_iManNo != mcULD_Home &&
       m_iManNo != mcPCK_Home &&
       m_iManNo != mcSRT_Home &&
       m_iManNo != mcAllHome    ) SEQ.InspectHomeEnd ();
    SEQ.InspectLightGrid();

    bool r1,r2,r3,r4,r5,r6,r7 ;

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return ; }

    //Cycle Step.
    if(m_iManNo == mcAllHome   ) {
        r4 = PCK.CycleHome();
        r1 = LDR.CycleHome();

        if(!r4 || !r1) return ;

        r2 = RAL.CycleHome();
        r3 = ULD.CycleHome();
        r5 = SRT.CycleHome();







        if(r1&&r2&&r3&&r4&&r5) {
            m_iManNo = mcNoneCycle;
            FM_MsgOk("Confirm","All Home Finished!");
        }
    }
    else if(m_iManNo ==  mcLDR_Home       ){if(LDR.CycleHome       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo ==  mcLDR_Supply     ){if(LDR.CycleSupply     ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo ==  mcRAL_Home       ){if(RAL.CycleHome       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo ==  mcRAL_In         ){if(RAL.CycleIn         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo ==  mcRAL_WorkStt    ){if(RAL.CycleWorkStt    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo ==  mcRAL_Work       ){if(RAL.CycleWork       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo ==  mcRAL_Out        ){if(RAL.CycleOut        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo ==  mcULD_Home       ){if(ULD.CycleHome       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo ==  mcULD_In         ){if(ULD.CycleIn         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo ==  mcULD_Stock      ){if(ULD.CycleStock      ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo ==  mcPCK_Home       ){if(PCK.CycleHome       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo ==  mcPCK_Pick       ){if(PCK.CyclePick       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo ==  mcPCK_Place      ){if(PCK.CyclePlace      ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo ==  mcPCK_ReZero     ){if(PCK.CycleRezero     ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo ==  mcPCK_Station    ){if(PCK.CycleStation    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo ==  mcPCK_ReZeroCheck){if(PCK.CycleRezeroCheck()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo ==  mcSRT_Home       ){if(SRT.CycleHome       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo ==  mcSRT_Pick       ){if(SRT.CyclePick       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo ==  mcSRT_Sort       ){if(SRT.CycleSort       ()) m_iManNo = mcNoneCycle; }
    else                                   {                           m_iManNo = mcNoneCycle; }

    //Ok.
    return ;
}

