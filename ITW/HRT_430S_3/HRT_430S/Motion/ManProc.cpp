//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ManProc.h"
#include "SMInterfaceUnit.h"
#include "Sequence.h"

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
    m_bRptMotr        = false ;
    m_bRptActr        = false ;
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
    m_iManNo      = mcNoneCycle;
    m_iPrevManNo  = mcNoneCycle;
    m_bLtProcOn   = false      ;
}

/***************************************************************************/
/* Inheritable Functions.                                                  */
/***************************************************************************/
//---------------------------------------------------------------------------
bool __fastcall TManProc::RptMotr(void)
{
    //Ok.
    return true;
}

//---------------------------------------------------------------------------
bool __fastcall TManProc::RptActr(void)
{
    //Ok.
    return true;
}

//---------------------------------------------------------------------------
void __fastcall TManProc::ManProcOn(int No , bool OnKey)
{
    //Check No. Error.
    if (No <  0               ) return;
    if (No >= MAX_MANUAL_CYCLE) return;

    //Check Running.
    if (SEQ._bRun              ) return; 
    if (m_iManNo != mcNoneCycle) return;



    //Set Manual No & Flag.
    m_iManNo    = No  ;
    m_bLtProcOn = true;

/*
         if (m_iManNo == mcAllHome            ){}

    else if (m_iManNo == mcLDR_Home           )
    else if (m_iManNo == mcLDR_Supply         )
    else if (m_iManNo == mcLDR_Pick           )if(!DM.ARAY[riLD1].CheckAllStat(csNone)       ){FM_MsgOk("ERR","Remove Mgz"          ); return false ;} }
    else if (m_iManNo == mcLDR_SttPstn        )if(!LD1.CheckSafe(miLD1_ZCmp,piLD1_ZWorkStart)){                                        return false ;} }
*/

    //Selection.


    //Reset Manual No & Flag.
    m_iManNo    = mcNoneCycle;
    m_bLtProcOn = false  ;
}

//---------------------------------------------------------------------------
bool __fastcall TManProc::ManCycleRun(void)
{
    //Check Run Flag.
    if (SEQ._bRun) return true;

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return true; }
    if (!MT_GetServoAll()) { Init(); return true; }

    //Check No. Cycle.
    if (!m_bLtProcOn       ) return true;

    //Cycle Step.
    if      (m_iManNo == 1) {m_iManNo = 0; } 
    else if (m_iManNo == 2) {m_iManNo = 0; }
    else if (m_iManNo == 3) {m_iManNo = 0; }
    else if (m_iManNo == 4) {m_iManNo = 0; }
    //추가사항으로 메뉴얼 확인 작업필요

    //Reset Flag.
    if (m_iManNo == mcNoneCycle) m_bLtProcOn = false;
    //Ok.
    return true;
}
//---------------------------------------------------------------------------
