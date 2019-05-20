//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ManualMan.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "JigPick.h"
#include "Dispnr.h"
#include "Place.h"
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "Sequence.h"
#include "DataMan.h"
#include "Sequence.h"
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

    m_bManSetting  = false ;

    m_bUldNeedPick  = false;
    m_bUldNeedPlace = false;

    m_tmCycle.Clear() ;
    m_tmDelay.Clear() ;
}

//---------------------------------------------------------------------------
void __fastcall CManualMan::Close(void)
{
}

//---------------------------------------------------------------------------
bool __fastcall CManualMan::SetManCycle(EN_MANUAL_CYCLE _iNo)
{
    if( _iNo     <  0                ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( _iNo     >= MAX_MANUAL_CYCLE ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( m_iManNo != mcNoneCycle      ) {FM_MsgOk("ERROR","Doing Manual Cycle"   ); return false ;}
    if(SEQ._bRun                     ) {FM_MsgOk("ERROR","Autorunning"          ); return false ;}

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return false ; } //아 밑에 처리 하는 애 때문에 잠시 이렇게 함.

    //if(!IO_GetX(xETC_PowerOn)                          ) {FM_MsgOk("ERR","Power On Plz");      return false ;}
    //if(!IO_GetX(xETC_MainAir) || !IO_GetY(yETC_MainSol)) {FM_MsgOk("ERROR","Check Main Air!"); return false ;}

    EM_SetDisp(true);

    //double dWorkPitch ;
    //if(OM.DevOptn.iContColCnt == 2) dWorkPitch = (OM.DevInfo.dColPitch + OM.DevOptn.dWorkPtchOfs)* 2 ;
    //if(OM.DevOptn.iContColCnt == 4) dWorkPitch = (OM.DevInfo.dColPitch + OM.DevOptn.dWorkPtchOfs)* 4 ;
    //
    //double dTrimPitch ;
    //dTrimPitch = (OM.DevInfo.dColPitch + OM.DevOptn.dTrimPtchOfs) ;
    //
    //int c,r ;
    //int iMaxRow,iMaxCol;


    m_bManSetting = true ; //업데이트 함수에서 다른쓰레드로 들어와서 갱신하기에 플레그 걸어 막아둠.
    m_iManNo = _iNo ;

    bool bRet = true ;

    /********************/
    /* 프리프로세서     */
    /********************/

    //bool bNothingRalMask = !IO_GetX(xRAL_LMask ) && !IO_GetX(xRAL_VMask );
    //bool bNothingRalPakg = !IO_GetX(xPRB_PkgIn1) && !IO_GetX(xPRB_Pkg2  ) && !IO_GetX(xRAL_LPkg3) &&
    //                       !IO_GetX(xRAL_VPkg4 ) && !IO_GetX(xPSB_PkgIn5) && !IO_GetX(xPSB_Pkg5 ) && !IO_GetX(xPSB_PkgOut6 );

         if(m_iManNo == mcAllHome           ){if(!IO_GetX(xETC_MainPower  )) {FM_MsgOk("ERR","Power On Plz"); bRet = false ;}}
    else if(m_iManNo == mcJIP_CycleJigPick  ){ } //STG.CheckSafe(miSTG_ZPick, piSTG_ZPickZig);
    else if(m_iManNo == mcDIP_CycleDispenser){ }
    else if(m_iManNo == mcPLC_CyclePlace    ){ }
    else if(m_iManNo == mcSTG_IdxUpDn       ){ } // 생략
    else if(m_iManNo == mcSTG_PshFwBw       ){if(MT_GetCmdPos(miSTG_ZPick) >= PM.GetValue(miSTG_ZPick, pvSTG_ZPickJIG)) {FM_MsgOk("ERR", "Plz, Move Z Axis Motor Wait Position"); } }
    else if(m_iManNo == mcSTG_Vacuum        ){ } // 생략
    else if(m_iManNo == mcSTG_Eject         ){ } // 생략
    else if(m_iManNo == mcSTG_AllSttClear   ){ } // 생략


    if(!bRet) Init();


    /********************/
    /* 처리..           */
    /********************/

         if(m_iManNo == mcAllHome           ) {JIP.InitHomeStep();
                                               DIP.InitHomeStep();
                                               PLC.InitHomeStep(); }

    else if(m_iManNo == mcJIP_CycleJigPick  ) {JIP.InitCycleStep();}
    else if(m_iManNo == mcDIP_CycleDispenser) {DIP.InitCycleStep();}
    else if(m_iManNo == mcPLC_CyclePlace    ) {PLC.InitCycleStep();}
    else if(m_iManNo == mcSTG_IdxUpDn       ) {if(AT_Complete(aiDSP_IdxUpDn , ccFwd)) AT_MoveCyl(aiDSP_IdxUpDn , ccBwd);
                                               else                                   AT_MoveCyl(aiDSP_IdxUpDn , ccFwd); }
    else if(m_iManNo == mcSTG_PshFwBw       ) {if(AT_Complete(aiSTG_PshFwBw , ccFwd)) AT_MoveCyl(aiSTG_PshFwBw , ccBwd);
                                               else                                   AT_MoveCyl(aiSTG_PshFwBw , ccFwd); }
    else if(m_iManNo == mcSTG_Vacuum        ) {IO_SetY(yETC_Vacuum , !IO_GetY(yETC_Vacuum));}
    else if(m_iManNo == mcSTG_Eject         ) {IO_SetY(yETC_Eject  , !IO_GetY(yETC_Eject ));}
    else if(m_iManNo == mcSTG_AllSttClear   ) {DM.ARAY[riDSP].SetStat(csNone);
                                               DM.ARAY[riJIG].SetStat(csNone);
                                               DM.ARAY[riPIK].SetStat(csNone);
                                               SEQ.Pt_Step.bIdle      = false;
                                               SEQ.Pt_Step.bJigPick   = false;
                                               SEQ.Pt_Step.bDispenser = false;
                                               SEQ.Pt_Step.bPlace     = false;
                                               }


    m_bManSetting = false ; //m_bManSetting 중요함 리턴전에 꼭 펄스 시켜야함. 쓰레드가 달라서. ::Update에서 m_iManNo=0이 되므로 주의.
    return true ;
}

EN_MANUAL_CYCLE __fastcall CManualMan::GetManNo()
{
    return m_iManNo ;
}

int CManualMan::GetManStep()
{
    return m_iManStep ;
}

//---------------------------------------------------------------------------
void __fastcall CManualMan::Update(void)
{

    if( m_iManNo == mcNoneCycle ) return ;
    if(m_bManSetting            ) return ;

    if(m_iManNo != mcAllHome ) SEQ.InspectHomeEnd();
    //SEQ.InspectLightGrid();

    bool r1,r2,r3;
    //bool r2,r3,r4,r5,r6,r7 ;
    r1=r2=r3 = false ;
    //r2=r3=r4=r5=r6=r7 = false ;

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return ; }
    //Cycle Step.
    if      (m_iManNo == mcAllHome      ) { r1  = JIP.CycleHome();
                                            r2  = DIP.CycleHome();
                                            r3  = PLC.CycleHome();
                                           if(r1 && r2 && r3) {
                                               m_iManNo = mcNoneCycle ;
                                               FM_MsgOk("Confirm","All Home Finished!");
                                           }
                                          }
    else if(m_iManNo == mcJIP_CycleJigPick   ) {if(JIP.CycleJigPick        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcDIP_CycleDispenser ) {if(DIP.CycleDispenser      ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPLC_CyclePlace     ) {if(PLC.CyclePlace          ()) m_iManNo = mcNoneCycle; }

    else                                       {                               m_iManNo = mcNoneCycle; } //여기서 갱신됌.

    //Ok.
    return ;
}

























