//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Sequence.h"
#include "ErrMan.h"
#include "PstnMan.h"
#include "IOs.h"
#include "Motors.h"
#include "Actuators.h"
#include "TimeProc.h"
#include "FormMsgOk.h"
#include "SLogUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------
TSequence SEQ;

/***************************************************************************/
/* 생성자 & 소멸자. (Constructor & Destructor)                             */
/***************************************************************************/
//---------------------------------------------------------------------------
__fastcall TSequence::TSequence(void)
{

}

//---------------------------------------------------------------------------
__fastcall TSequence::~TSequence(void)
{
    //EM.Save();
    EndThread();
}

/***************************************************************************/
/* Init.                                                                   */
/***************************************************************************/
//---------------------------------------------------------------------------
void __fastcall TSequence::Init(void)
{
    IO.Load();
    AT.Load();
    MT.Load();
    EM.Load();
    PM.Load();
    OM.LoadJobFile("NONE");

    OM.LoadMstOptn();

    m_bFlick   = false ;

    LT.m_sPartName = "Left Table"  ;  RT.m_sPartName = "Right Table" ;
    LT.m_iPartId   = piLT          ;  RT.m_iPartId   = piRT          ;
    LT.miZ         = miLT_Z        ;  RT.miZ         = miRT_Z        ;
    LT.miY         = miLT_Y        ;  RT.miY         = miRT_Y        ;
    LT.miI         = miLT_I        ;  RT.miI         = miRT_I        ;

    LT.eiHomeTO    = eiLT_HomeTO   ;  RT.eiHomeTO    = eiRT_HomeTO   ;
    LT.eiStartTO   = eiLT_StartTO  ;  RT.eiStartTO   = eiRT_StartTO  ;
    LT.eiStopTO    = eiLT_StopTO   ;  RT.eiStopTO    = eiRT_StopTO   ;
    LT.eiCkBtnTO   = eiLT_CkBtnTO  ;  RT.eiCkBtnTO   = eiRT_CkBtnTO  ;
    LT.eiCycleTO   = eiLT_CycleTO  ;  RT.eiCycleTO   = eiRT_CycleTO  ;
    LT.eiXAlarm    = eiLT_ZAlarm   ;  RT.eiXAlarm    = eiRT_ZAlarm   ;
    LT.eiYAlarm    = eiLT_YAlarm   ;  RT.eiYAlarm    = eiRT_YAlarm   ;
    LT.eiIAlarm    = eiLT_IAlarm   ;  RT.eiIAlarm    = eiRT_IAlarm   ;
    LT.eiXNegLim   = eiLT_ZNegLim  ;  RT.eiXNegLim   = eiRT_ZNegLim  ;
    LT.eiYNegLim   = eiLT_YNegLim  ;  RT.eiYNegLim   = eiRT_YNegLim  ;
    LT.eiINegLim   = eiLT_INegLim  ;  RT.eiINegLim   = eiRT_INegLim  ;
    LT.eiXPosLim   = eiLT_ZPosLim  ;  RT.eiXPosLim   = eiRT_ZPosLim  ;
    LT.eiYPosLim   = eiLT_YPosLim  ;  RT.eiYPosLim   = eiRT_YPosLim  ;
    LT.eiIPosLim   = eiLT_IPosLim  ;  RT.eiIPosLim   = eiRT_IPosLim  ;
    LT.eiMainTO    = eiLT_MainTO   ;  RT.eiMainTO    = eiRT_MainTO   ;
    LT.eiSubTO     = eiLT_SubTO    ;  RT.eiSubTO     = eiRT_SubTO    ;
    LT.eiLdCalTO   = eiLT_LdCalTO  ;  RT.eiLdCalTO   = eiRT_LdCalTO  ;
    LT.eiGp1TO     = eiLT_Gp1TO    ;  RT.eiGp1TO     = eiRT_Gp1TO    ;
    LT.eiSupportTO = eiLT_SupportTO;  RT.eiSupportTO = eiRT_SupportTO;
    LT.eiPcbCmpTO  = eiLT_PcbCmpTO ;  RT.eiPcbCmpTO  = eiRT_PcbCmpTO ;
    LT.eiGtCalTO   = eiLT_GtCalTO  ;  RT.eiGtCalTO   = eiRT_GtCalTO  ;
    LT.eiUvFBTO    = eiLT_UvFBTO   ;  RT.eiUvFBTO    = eiRT_UvFBTO   ;
    LT.eiUvUDTO    = eiLT_UvUDTO   ;  RT.eiUvUDTO    = eiRT_UvUDTO   ;
    LT.eiDp1IOTO   = eiLT_Dp1IOTO  ;  RT.eiDp1IOTO   = eiRT_Dp1IOTO  ;
    LT.eiDp2IOTO   = eiLT_Dp2IOTO  ;  RT.eiDp2IOTO   = eiRT_Dp2IOTO  ;
    LT.eiDp1UDTO   = eiLT_Dp1UDTO  ;  RT.eiDp1UDTO   = eiRT_Dp1UDTO  ;
    LT.eiDp2UDTO   = eiLT_Dp2UDTO  ;  RT.eiDp2UDTO   = eiRT_Dp2UDTO  ;
    LT.eiXHomeEnd  = eiLT_ZHomeEnd ;  RT.eiXHomeEnd  = eiRT_ZHomeEnd ;
    LT.eiYHomeEnd  = eiLT_YHomeEnd ;  RT.eiYHomeEnd  = eiRT_YHomeEnd ;
    LT.eiIHomeEnd  = eiLT_IHomeEnd ;  RT.eiIHomeEnd  = eiRT_IHomeEnd ;
    LT.eiPadUDTO   = eiLT_PadUDTO  ;  RT.eiPadUDTO   = eiRT_PadUDTO  ;

    LT.yStartLp    = yLT_StartLp   ;  RT.yStartLp    = yRT_StartLp   ;
    LT.yPickupLp   = yLT_PickupLp  ;  RT.yPickupLp   = yRT_PickupLp  ;
    LT.yUpDnLp     = yLT_UpDnLp    ;  RT.yUpDnLp     = yRT_UpDnLp    ;
    LT.yVacLp      = yLT_VacLp     ;  RT.yVacLp      = yRT_VacLp     ;
    LT.yChuckLp    = yLT_ChuckLp   ;  RT.yChuckLp    = yRT_ChuckLp   ;
    LT.yStopLp     = yLT_StopLp    ;  RT.yStopLp     = yRT_StopLp    ;
    LT.yResetLp    = yLT_ResetLp   ;  RT.yResetLp    = yRT_ResetLp   ;
    LT.ySuckOn     = yLT_SuckOn    ;  RT.ySuckOn     = yRT_SuckOn    ;
    LT.yHdStart    = yLT_HdStart   ;  RT.yHdStart    = yRT_HdStart   ;

    LT.ySol1       = yLT_Sol1      ;  RT.ySol1       = yRT_Sol1      ;
    LT.ySol2       = yLT_Sol2      ;  RT.ySol2       = yRT_Sol2      ;
    LT.ySol3       = yLT_Sol3      ;  RT.ySol3       = yRT_Sol3      ;
    LT.ySol4       = yLT_Sol4      ;  RT.ySol4       = yRT_Sol4      ;
    LT.ySol5       = yLT_Sol5      ;  RT.ySol5       = yRT_Sol5      ;
    LT.ySol6       = yLT_Sol6      ;  RT.ySol6       = yRT_Sol6      ;
//    LT.yVac1       = yLT_Vac1      ;  RT.yVac1       = yRT_Vac1      ;
//    LT.yVac2       = yLT_Vac2      ;  RT.yVac2       = yRT_Vac2      ;
//    LT.yVac3       = yLT_Vac3      ;  RT.yVac3       = yRT_Vac3      ;
//    LT.yVac4       = yLT_Vac4      ;  RT.yVac4       = yRT_Vac4      ;
//    LT.yVac5       = yLT_Vac5      ;  RT.yVac5       = yRT_Vac5      ;
//    LT.yVac6       = yLT_Vac6      ;  RT.yVac6       = yRT_Vac6      ;


    LT.xStartSw    = xLT_StartSw   ;  RT.xStartSw    = xRT_StartSw   ;
    LT.xPickupSw   = xLT_PickupSw  ;  RT.xPickupSw   = xRT_PickupSw  ;
    LT.xUpDnSw     = xLT_UpDnSw    ;  RT.xUpDnSw     = xRT_UpDnSw    ;
    LT.xVacSw      = xLT_VacSw     ;  RT.xVacSw      = xRT_VacSw     ;
    LT.xChuckSw    = xLT_ChuckSw   ;  RT.xChuckSw    = xRT_ChuckSw   ;
    LT.xStopSw     = xLT_StopSw    ;  RT.xStopSw     = xRT_StopSw    ;
    LT.xResetSw    = xLT_ResetSw   ;  RT.xResetSw    = xRT_ResetSw   ;
    LT.xHdEnd      = xLT_HdEnd     ;  RT.xHdEnd      = xRT_HdEnd     ;

    LT.aiMain      = aiLT_Main     ;  RT.aiMain      = aiRT_Main     ;
    LT.aiSub       = aiLT_Sub      ;  RT.aiSub       = aiRT_Sub      ;
    LT.aiLdCal     = aiLT_LdCal    ;  RT.aiLdCal     = aiRT_LdCal    ;
    LT.aiGp1       = aiLT_Gp1      ;  RT.aiGp1       = aiRT_Gp1      ;
    LT.aiSupport   = aiLT_Support  ;  RT.aiSupport   = aiRT_Support  ;
    LT.aiPcbCmp    = aiLT_PcbCmp   ;  RT.aiPcbCmp    = aiRT_PcbCmp   ;
    LT.aiGtCal     = aiLT_GtCal    ;  RT.aiGtCal     = aiRT_GtCal    ;
    LT.aiUvFB      = aiLT_UvFB     ;  RT.aiUvFB      = aiRT_UvFB     ;
    LT.aiUvUD      = aiLT_UvUD     ;  RT.aiUvUD      = aiRT_UvUD     ;
    LT.aiDp1IO     = aiLT_Dp1IO    ;  RT.aiDp1IO     = aiRT_Dp1IO    ;
    LT.aiDp2IO     = aiLT_Dp2IO    ;  RT.aiDp2IO     = aiRT_Dp2IO    ;
    LT.aiDp1UD     = aiLT_Dp1UD    ;  RT.aiDp1UD     = aiRT_Dp1UD    ;
    LT.aiDp2UD     = aiLT_Dp2UD    ;  RT.aiDp2UD     = aiRT_Dp2UD    ;
    LT.aiPadUD     = aiLT_PadUD    ;  RT.aiPadUD     = aiRT_PadUD    ;
    LT.aiLdIO      = aiLT_LdIO     ;  RT.aiLdIO      = aiRT_LdIO     ;

    LT.pi_Z_Wait   = piLT_Z_Wait   ;  RT.pi_Z_Wait   = piRT_Z_Wait   ;
    LT.pi_Z_Insp   = piLT_Z_Insp   ;  RT.pi_Z_Insp   = piRT_Z_Insp   ;
    LT.pi_Y_Wait   = piLT_Y_Wait   ;  RT.pi_Y_Wait   = piRT_Y_Wait   ;
    LT.pi_Y_Insp   = piLT_Y_Insp   ;  RT.pi_Y_Insp   = piRT_Y_Insp   ;
    LT.pi_I_Wait   = piLT_I_Wait   ;  RT.pi_I_Wait   = piRT_I_Wait   ;
    LT.pi_I_Insp   = piLT_I_Insp   ;  RT.pi_I_Insp   = piRT_I_Insp   ;

    //memcpy(LT.iOptnDisp , OM.DevOptn.iLTDisp , sizeof(int)*MAX_DISP_NOZZLE*MAX_DISP_CNT);
    //memcpy(RT.iOptnDisp , OM.DevOptn.iRTDisp , sizeof(int)*MAX_DISP_NOZZLE*MAX_DISP_CNT);
    SetOptn();


    m_FlickOnTimer  .Clear();
    m_FlickOffTimer .Clear();


    Reset();

    StartThread();
}

//---------------------------------------------------------------------------
void __fastcall TSequence::SetOptn(void)
{
    int l ,k ;
    for(int i = 0 ;i < MAX_DISP_NOZZLE ; i++) {
        for(int j = 0 ; j < MAX_DISP_CNT ; j++) {
            l = OM.DevOptn.iLTDisp[i][j] ;
            k = OM.DevOptn.iRTDisp[i][j] ;
            LT.iOptnDisp[i][j] = l;//OM.DevOptn.iLTDisp[i][j] ;
            RT.iOptnDisp[i][j] = k;//OM.DevOptn.iRTDisp[i][j] ;
        }

    }

}

//---------------------------------------------------------------------------
void __fastcall TSequence::Reset(void)
{
    m_FlickOnTimer  .Clear();
    m_FlickOffTimer .Clear();
    m_StopBtTimer   .Clear();

    //System.
    MT.ResetAll();
    AT.ResetAll();

    LT.Reset();
    RT.Reset();

    //Error.
    EM.Clear();

    if(FrmMsgOk && FrmMsgOk -> Visible) FrmMsgOk -> Close() ;

    //요거 Sequence UpdateStat으로 이동
    //  if(LT.Cycle == NULL && RT.Cycle == NULL) bUseAPC = false ;
}

/***************************************************************************/
/* Member Functions.                                                       */
/***************************************************************************/
/***************************************************************************/
/* Inheritable Functions.                                                  */
/***************************************************************************/
//---------------------------------------------------------------------------
bool __fastcall TSequence::InspectMainAir(void)
{
    if(!IO.GetX(xMainSol)){EM.SetErr(eiMainAir) ; return false ;}

    return true ;
}

//---------------------------------------------------------------------------
bool __fastcall TSequence::InspectEmergency(void)
{
    if(IO.GetX(xEmgSw)) {
        EM.SetErr  (eiEmgSw);
        MT.SetServo(miLT_I , false);
        MT.SetServo(miLT_Y , false);
        MT.SetServo(miLT_Z , false);
        MT.SetServo(miRT_I , false);
        MT.SetServo(miRT_Y , false);
        MT.SetServo(miRT_Z , false);
        return false ;
    }

    return true;
}

//---------------------------------------------------------------------------
void __fastcall TSequence::Update(void)
{
    dwUD_Strt[0] = (DWORD)GetTime_us(); IO.Update        (); dwUP_Scan[0] = (DWORD)GetTime_us() - dwUD_Strt[0];
    dwUD_Strt[1] = (DWORD)GetTime_us(); InspectMainAir   (); dwUP_Scan[1] = (DWORD)GetTime_us() - dwUD_Strt[1];
    dwUD_Strt[2] = (DWORD)GetTime_us(); InspectEmergency (); dwUP_Scan[2] = (DWORD)GetTime_us() - dwUD_Strt[2];
    dwUD_Strt[3] = (DWORD)GetTime_us(); AT.Update        (); dwUP_Scan[3] = (DWORD)GetTime_us() - dwUD_Strt[3];
    dwUD_Strt[4] = (DWORD)GetTime_us(); MT.Update        (); dwUP_Scan[4] = (DWORD)GetTime_us() - dwUD_Strt[4];
    dwUD_Strt[5] = (DWORD)GetTime_us(); UpdateSeqState   (); dwUP_Scan[5] = (DWORD)GetTime_us() - dwUD_Strt[5];

    dwUD_Strt[6] = (DWORD)GetTime_us(); LT.Update        (); dwUP_Scan[6] = (DWORD)GetTime_us() - dwUD_Strt[6];
    dwUD_Strt[7] = (DWORD)GetTime_us(); RT.Update        (); dwUP_Scan[7] = (DWORD)GetTime_us() - dwUD_Strt[7];
    dwUD_Strt[8] = (DWORD)GetTime_us(); LT.UpdateSeqState(); dwUP_Scan[8] = (DWORD)GetTime_us() - dwUD_Strt[8];
    dwUD_Strt[9] = (DWORD)GetTime_us(); RT.UpdateSeqState(); dwUP_Scan[9] = (DWORD)GetTime_us() - dwUD_Strt[9];
}


void __fastcall TSequence::UpdateSeqState(void)
{
    //Flicking Timer.
    if (m_bFlick) { m_FlickOnTimer .Clear(); if (m_FlickOffTimer.OnDelay( m_bFlick , 500)) m_bFlick = false; }
    else          { m_FlickOffTimer.Clear(); if (m_FlickOnTimer .OnDelay(!m_bFlick , 500)) m_bFlick = true ; }

    bool bKeepStopSw   = m_StopBtTimer  .OnDelay(IO.GetX(xLT_StopSw  )||IO.GetX(xRT_StopSw  ),500 ) ; if(bKeepStopSw  ) m_StopBtTimer  .Clear();
    bool b2StopSw      = IO.GetX(xLT_StopSw  )&&IO.GetX(xRT_StopSw  ) ;
    bool bResetSw      = IO.GetX(xLT_ResetSw) || IO.GetX(xRT_ResetSw) ;

    if(bKeepStopSw                      ) Reset();                       //{Reset() ; if(LT.Cycle == NULL && RT.Cycle == NULL) bUseAPC = false ; }
    if(bResetSw                         ) Reset();                       //{Reset() ; if(LT.Cycle == NULL && RT.Cycle == NULL) bUseAPC = false ; }
    if(b2StopSw && m_iSeqStat == ssStop ) {LT.ReqHome(); RT.ReqHome(); } //bUseAPC = false ;  }

    if(LT.Cycle == NULL && RT.Cycle == NULL) bUseAPC = false ;

    //Local Var.
    bool isErr     = EM.IsErr() ;
    bool isRun     = LT.GetStepSeq()  || RT.GetStepSeq()  ;
    bool isHome    = LT.GetStepHome() || RT.GetStepHome() ;

    static bool isPreHome = false ;

    if(isPreHome == true && !isHome && !isErr)MsgOk("HOME","INITIAL FINISHED");

    isPreHome = isHome ;

    //Set Stat.
         if (isHome ) { m_iSeqStat = ssInit    ;}
    else if (isErr  ) { m_iSeqStat = ssError   ;}
    else if (isRun  ) { m_iSeqStat = ssRunning ;}
    else              { m_iSeqStat = ssStop    ;}
}

void __fastcall TSequence::ReqHome()
{
    LT.ReqHome() ;
    RT.ReqHome() ;
}

