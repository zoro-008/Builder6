//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ManualMan.h"

#include "SMInterfaceUnit.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "Sequence.h"
#include "Loader.h"
#include "Head.h"
#include "Rail.h"
#include "PstBuff.h"
#include "UnLoader.h"
#include "PstnMan.h"
#include "VisnComUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------
CManualMan MM;

int iHedManCol = -1 ;
int iHedManRow = -1 ;

int iTrmManCol = -1 ;
int iTrmManRow = -1 ;

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

    m_tmCycle.Clear() ;
}

//---------------------------------------------------------------------------
bool __fastcall CManualMan::SetManCycle(EN_MANUAL_CYCLE _iNo)
{
    if( _iNo    <  0                ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( _iNo    >= MAX_MANUAL_CYCLE ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if(m_iManNo != mcNoneCycle      ) {FM_MsgOk("ERROR","Doing Manual Cycle"   ); return false ;}
    if(SEQ._bRun                    ) {FM_MsgOk("ERROR","Autorunning"          ); return false ;}

    //if(!OM.EqpOptn.bExistLoader && _iNo  == mcLDR_Home) {FM_MsgOk("ERROR","No Loader Model"); return false ;}

    if(_iNo  == mcLDR_Home &&  IO_GetX(xPRB_PkgIn1)      && OM.EqpOptn.bExistLoader) {FM_MsgOk("ERROR","Check Rail In Sensor!" ); return false ;}
    if(_iNo  == mcULD_Home &&  OM.EqpOptn.bExistUnLoader && IO_GetX(xPSB_PkgOut2 ) ) {FM_MsgOk("ERROR","Check Rail Out Sensor!"); return false ;}
    if(_iNo  == mcAllHome  &&  IO_GetX(xPRB_PkgIn1)      && OM.EqpOptn.bExistLoader) {FM_MsgOk("ERROR","Check Rail In Sensor!" ); return false ;}
    if(_iNo  == mcAllHome  &&  OM.EqpOptn.bExistUnLoader && IO_GetX(xPSB_PkgOut2 ) ) {FM_MsgOk("ERROR","Check Rail Out Sensor!"); return false ;}

    EM_SetDisp(true);

    bool bRet ;
    m_iManNo = _iNo ;

    //ETC
    if (m_iManNo == mcAllHome            ){     if(!IO_GetX(xETC_PowerOn )) {FM_MsgOk("ERR","Power On Plz"); bRet = false ;}
                                           else if(!IO_GetY(yETC_MainSol )) {FM_MsgOk("ERR","Air On Plz"); bRet = false ;}
//                                                 else if(!IO_GetX(xETC_RMainAir)) {FM_MsgOk("ERR","Air On Plz"  ); bRet = false ;}
                                           //여기서 굳이 하는건 아닌거 같네...시퀀스에서 에러 띄우는게...
                                          }


    if      (m_iManNo == mcAllHome          ) { LDR.InitHomeStep ();
                                                RAL.InitHomeStep ();
                                                HED.InitHomeStep ();
                                                PSB.InitHomeStep ();
                                                ULD.InitHomeStep (); }

    //LDR
    else if (m_iManNo == mcLDR_Home         ) { LDR.InitHomeStep (); }
    else if (m_iManNo == mcLDR_Supply       ) { LDR.InitCycleStep(); }
    else if (m_iManNo == mcLDR_Pick         ) { LDR.InitCycleStep(); }
    else if (m_iManNo == mcLDR_Work         ) { LDR.InitCycleStep();
                                                RAL.InitCycleStep(); }
    else if (m_iManNo == mcLDR_Place        ) { LDR.InitCycleStep(); }
    else if (m_iManNo == mcLDR_InAc         ) { IO_SetY(yLDR_MgzInAC  , !IO_GetY(yLDR_MgzInAC ));   }
    else if (m_iManNo == mcLDR_OutAc        ) { IO_SetY(yLDR_MgzOutAC , !IO_GetY(yLDR_MgzOutAC));   }
    else if (m_iManNo == mcLDR_CmpUD        ) { AT_MoveCyl(aiLDR_Cmp    , !AT_GetCmd(aiLDR_Cmp   ));}
    else if (m_iManNo == mcLDR_PusherFB     ) { AT_MoveCyl(aiLDR_Pusher , !AT_GetCmd(aiLDR_Pusher));}

    //RAL
    else if (m_iManNo == mcRAL_Home         ) { RAL.InitHomeStep (); }
    else if (m_iManNo == mcRAL_MvAuto       ) { RAL.InitCycleStep(); }
    else if (m_iManNo == mcRAL_Bind         ) { RAL.InitCycleStep(); }
    else if (m_iManNo == mcRAL_UnBind       ) { AT_MoveCyl(aiWK1_Lift    , ccBwd);
                                                AT_MoveCyl(aiWK2_Lift    , ccBwd);
                                                AT_MoveCyl(aiWK1_Stopper , ccBwd);
                                                AT_MoveCyl(aiWK2_Stopper , ccBwd);}
    else if (m_iManNo == mcRAL_PreStprUD    ) { AT_MoveCyl(aiPRB_Stopper , !AT_GetCmd(aiPRB_Stopper));}
    else if (m_iManNo == mcRAL_Wk1StprUD    ) { AT_MoveCyl(aiWK1_Stopper , !AT_GetCmd(aiWK1_Stopper));}
    else if (m_iManNo == mcRAL_Wk2StprUD    ) { AT_MoveCyl(aiWK2_Stopper , !AT_GetCmd(aiWK2_Stopper));}
    else if (m_iManNo == mcRAL_Wk1LiftUD    ) { AT_MoveCyl(aiWK1_Lift    , !AT_GetCmd(aiWK1_Lift   ));}
    else if (m_iManNo == mcRAL_Wk2LiftUD    ) { AT_MoveCyl(aiWK2_Lift    , !AT_GetCmd(aiWK2_Lift   ));}


    //HED
    else if (m_iManNo == mcHED_Home         ) { HED.InitHomeStep (); }
    else if (m_iManNo == mcHED_Insp         ) { HED.InitCycleStep(); } //
    else if (m_iManNo == mcHED_InspMan      ) { m_iManStep = 10; m_iPreManStep = 0; }

    else if (m_iManNo == mcHED_PchXp        ) { iHedManCol += OM.DevOptn.iColInspCnt ;
                                                if(iHedManCol < 0 ) iHedManCol = 0 ;
                                                if(iHedManRow < 0 ) iHedManRow = 0 ;
                                                if(iHedManCol >= OM.DevInfo.iColCnt) iHedManCol = OM.DevInfo.iColCnt - OM.DevOptn.iColInspCnt;
                                                if(iHedManRow >= OM.DevInfo.iRowCnt) iHedManRow = OM.DevInfo.iRowCnt - OM.DevOptn.iRowInspCnt;
                                                MT_GoAbsMan(miWRK_XVsn , HED.GetMotrPosTable(miWRK_XVsn , iHedManRow , iHedManCol)) ;
                                                MT_GoAbsMan(miWK1_YVsn , HED.GetMotrPosTable(miWK1_YVsn , iHedManRow , iHedManCol)) ;
                                                MT_GoAbsMan(miWK2_YVsn , HED.GetMotrPosTable(miWK2_YVsn , iHedManRow , iHedManCol)) ;}

    else if (m_iManNo == mcHED_PchXn        ) { iHedManCol -= OM.DevOptn.iColInspCnt ;
                                                if(iHedManCol < 0 ) iHedManCol = 0 ;
                                                if(iHedManRow < 0 ) iHedManRow = 0 ;
                                                if(iHedManCol >= OM.DevInfo.iColCnt) iHedManCol = OM.DevInfo.iColCnt - OM.DevOptn.iColInspCnt;
                                                if(iHedManRow >= OM.DevInfo.iRowCnt) iHedManRow = OM.DevInfo.iRowCnt - OM.DevOptn.iRowInspCnt;
                                                MT_GoAbsMan(miWRK_XVsn , HED.GetMotrPosTable(miWRK_XVsn , iHedManRow , iHedManCol)) ;
                                                MT_GoAbsMan(miWK1_YVsn , HED.GetMotrPosTable(miWK1_YVsn , iHedManRow , iHedManCol)) ;
                                                MT_GoAbsMan(miWK2_YVsn , HED.GetMotrPosTable(miWK2_YVsn , iHedManRow , iHedManCol)) ;}

    else if (m_iManNo == mcHED_PchYp        ) { iHedManRow += OM.DevOptn.iRowInspCnt ;
                                                if(iHedManCol < 0 ) iHedManCol = 0 ;
                                                if(iHedManRow < 0 ) iHedManRow = 0 ;
                                                if(iHedManCol >= OM.DevInfo.iColCnt) iHedManCol = OM.DevInfo.iColCnt - OM.DevOptn.iColInspCnt;
                                                if(iHedManRow >= OM.DevInfo.iRowCnt) iHedManRow = OM.DevInfo.iRowCnt - OM.DevOptn.iRowInspCnt;
                                                MT_GoAbsMan(miWRK_XVsn , HED.GetMotrPosTable(miWRK_XVsn , iHedManRow , iHedManCol)) ;
                                                MT_GoAbsMan(miWK1_YVsn , HED.GetMotrPosTable(miWK1_YVsn , iHedManRow , iHedManCol)) ;
                                                MT_GoAbsMan(miWK2_YVsn , HED.GetMotrPosTable(miWK2_YVsn , iHedManRow , iHedManCol)) ;}

    else if (m_iManNo == mcHED_PchYn        ) { iHedManRow -= OM.DevOptn.iRowInspCnt ;
                                                if(iHedManCol < 0 ) iHedManCol = 0 ;
                                                if(iHedManRow < 0 ) iHedManRow = 0 ;
                                                if(iHedManCol >= OM.DevInfo.iColCnt) iHedManCol = OM.DevInfo.iColCnt - OM.DevOptn.iColInspCnt;
                                                if(iHedManRow >= OM.DevInfo.iRowCnt) iHedManRow = OM.DevInfo.iRowCnt - OM.DevOptn.iRowInspCnt;
                                                MT_GoAbsMan(miWRK_XVsn , HED.GetMotrPosTable(miWRK_XVsn , iHedManRow , iHedManCol)) ;
                                                MT_GoAbsMan(miWK1_YVsn , HED.GetMotrPosTable(miWK1_YVsn , iHedManRow , iHedManCol)) ;
                                                MT_GoAbsMan(miWK2_YVsn , HED.GetMotrPosTable(miWK2_YVsn , iHedManRow , iHedManCol)) ;}

    else if (m_iManNo == mcHED_StartPos     ) { iHedManCol = 0 ;
                                                iHedManRow = 0 ;
                                                MT_GoAbsMan(miWRK_XVsn , PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkSttPs)) ;
                                                MT_GoAbsMan(miWK1_YVsn , PM.GetValue(miWK1_YVsn , pvWK1_YVsnWorkSttPs)) ;
                                                MT_GoAbsMan(miWK2_YVsn , PM.GetValue(miWK2_YVsn , pvWK2_YVsnWorkSttPs)) ; }


    //PSB
    else if (m_iManNo == mcPSB_Home         ) { PSB.InitHomeStep (); }
    else if (m_iManNo == mcPSB_Flip         ) { PSB.InitCycleStep(); }
    else if (m_iManNo == mcPSB_Work         ) { PSB.InitCycleStep(); }
    else if (m_iManNo == mcPSB_Out          ) { PSB.InitCycleStep(); }
    else if (m_iManNo == mcPSB_FingerFB     ) { AT_MoveCyl(aiPSB_Finger   , !AT_GetCmd(aiPSB_Finger  )); }
    else if (m_iManNo == mcPSB_AlignFB      ) { AT_MoveCyl(aiPSB_Align    , !AT_GetCmd(aiPSB_Align   )); }
    else if (m_iManNo == mcPSB_FlprFB       ) { PSB.MoveActr(aiPSB_Flpr   , !AT_GetCmd(aiPSB_Flpr    )); }
    else if (m_iManNo == mcPSB_FlprCmpFB    ) { AT_MoveCyl(aiPSB_FlprCmp  , !AT_GetCmd(aiPSB_FlprCmp )); }
    else if (m_iManNo == mcPSB_PusherUD     ) { AT_MoveCyl(aiPSB_PusherUD , !AT_GetCmd(aiPSB_PusherUD)); }
    else if (m_iManNo == mcPSB_PusherFB     ) { AT_MoveCyl(aiPSB_PusherFB , !AT_GetCmd(aiPSB_PusherFB)); }

    else if (m_iManNo == mcPSB_PchXp        ) { iTrmManCol ++ ;
                                                if(iTrmManCol < 0 ) iTrmManCol = 0 ;
                                                if(iTrmManRow < 0 ) iTrmManRow = 0 ;
                                                if(iTrmManCol >= OM.DevInfo.iColCnt) iTrmManCol = OM.DevInfo.iColCnt - 1;
                                                if(iTrmManRow >= OM.DevInfo.iRowCnt) iTrmManRow = OM.DevInfo.iRowCnt - 1;
                                                MT_GoAbsMan(miPSB_XTrm , PSB.GetMotrPosTable(miPSB_XTrm , iTrmManRow , iTrmManCol)) ;
                                                MT_GoAbsMan(miPSB_YTrm , PSB.GetMotrPosTable(miPSB_YTrm , iTrmManRow , iTrmManCol)) ; }

    else if (m_iManNo == mcPSB_PchXn        ) { iTrmManCol -- ;
                                                if(iTrmManCol < 0 ) iTrmManCol = 0 ;
                                                if(iTrmManRow < 0 ) iTrmManRow = 0 ;
                                                if(iTrmManCol >= OM.DevInfo.iColCnt) iTrmManCol = OM.DevInfo.iColCnt - 1;
                                                if(iTrmManRow >= OM.DevInfo.iRowCnt) iTrmManRow = OM.DevInfo.iRowCnt - 1;
                                                MT_GoAbsMan(miPSB_XTrm , PSB.GetMotrPosTable(miPSB_XTrm , iTrmManRow , iTrmManCol)) ;
                                                MT_GoAbsMan(miPSB_YTrm , PSB.GetMotrPosTable(miPSB_YTrm , iTrmManRow , iTrmManCol)) ; }

    else if (m_iManNo == mcPSB_PchYp        ) { iTrmManRow ++ ;
                                                if(iTrmManCol < 0 ) iTrmManCol = 0 ;
                                                if(iTrmManRow < 0 ) iTrmManRow = 0 ;
                                                if(iTrmManCol >= OM.DevInfo.iColCnt) iTrmManCol = OM.DevInfo.iColCnt - 1;
                                                if(iTrmManRow >= OM.DevInfo.iRowCnt) iTrmManRow = OM.DevInfo.iRowCnt - 1;
                                                MT_GoAbsMan(miPSB_XTrm , PSB.GetMotrPosTable(miPSB_XTrm , iTrmManRow , iTrmManCol)) ;
                                                MT_GoAbsMan(miPSB_YTrm , PSB.GetMotrPosTable(miPSB_YTrm , iTrmManRow , iTrmManCol)) ; }

    else if (m_iManNo == mcPSB_PchYn        ) { iTrmManRow -- ;
                                                if(iTrmManCol < 0 ) iTrmManCol = 0 ;
                                                if(iTrmManRow < 0 ) iTrmManRow = 0 ;
                                                if(iTrmManCol >= OM.DevInfo.iColCnt) iTrmManCol = OM.DevInfo.iColCnt - 1;
                                                if(iTrmManRow >= OM.DevInfo.iRowCnt) iTrmManRow = OM.DevInfo.iRowCnt - 1;
                                                MT_GoAbsMan(miPSB_XTrm , PSB.GetMotrPosTable(miPSB_XTrm , iTrmManRow , iTrmManCol)) ;
                                                MT_GoAbsMan(miPSB_YTrm , PSB.GetMotrPosTable(miPSB_YTrm , iTrmManRow , iTrmManCol)) ; }
    else if (m_iManNo == mcPSB_WorkStartPos ) { iTrmManCol = 0 ;
                                                iTrmManRow = 0 ;
                                                MT_GoAbsMan(miPSB_XTrm , PM.GetValue(miPSB_XTrm , pvPSB_XTrmWorkSttPs)) ;
                                                MT_GoAbsMan(miPSB_YTrm , PM.GetValue(miPSB_YTrm , pvPSB_YTrmWorkSttPs)) ; }

    else if (m_iManNo == mcPSB_TrmFB        ) { AT_MoveCyl(aiPSB_Trimmer , !AT_GetCmd(aiPSB_Trimmer)); }


    //ULD
    else if (m_iManNo == mcULD_Home         ) { ULD.InitHomeStep (); }
    else if (m_iManNo == mcULD_Supply       ) { ULD.InitCycleStep(); }
    else if (m_iManNo == mcULD_Pick         ) { if(!DM.ARAY[riULD].CheckAllStat(csNone)) {FM_MsgOk("ERR","Remove MGZ"); bRet = false ;}
                                                ULD.InitCycleStep(); }
    else if (m_iManNo == mcULD_Work         ) { ULD.InitCycleStep(); }
    else if (m_iManNo == mcULD_Place        ) { ULD.InitCycleStep(); }
    else if (m_iManNo == mcULD_InAc         ) { IO_SetY(yULD_MgzInAC  , !IO_GetY(yULD_MgzInAC )); }
    else if (m_iManNo == mcULD_OutAc        ) { IO_SetY(yULD_MgzOutAC , !IO_GetY(yULD_MgzOutAC)); }
    else if (m_iManNo == mcULD_CmpUD        ) { AT_MoveCyl(aiULD_Cmp  , !AT_GetCmd(aiULD_Cmp  )); }
    else if (m_iManNo == mcULD_WorkPos      ) { if(ULD.CheckSafe(miULD_ZCmp , piULD_ZCmpWorkStt))MT_GoAbsMan(miULD_ZCmp , PM.GetValue(miULD_ZCmp , pvULD_ZCmpWorkSttPs)); }
    else if (m_iManNo == mcULD_CmpPchUp     ) { if(ULD.CheckSafe(miULD_ZCmp , piULD_ZCmpWorkStt))MT_GoIncMan(miULD_ZCmp ,-OM.DevInfo.dCsSlPitch);   }
    else if (m_iManNo == mcULD_CmpPchDn     ) { if(ULD.CheckSafe(miULD_ZCmp , piULD_ZCmpWorkStt))MT_GoIncMan(miULD_ZCmp , OM.DevInfo.dCsSlPitch);   }
/*  else if (m_iManNo == mcETC_VisnTest     ) { VC.SendReset(vrVisn1);
                                                VC.SendReset(vrVisn2);
                                                DM.ARAY[riWK2].SetStat(csUnkwn);
                                                DM.ARAY[riWK1].SetStat(csUnkwn);
                                                RAL.InitCycleStep();
                                                HED.InitCycleStep();
                                              }*/





    //Default
    else                                      { m_iManNo = mcNoneCycle ; }

    if(!bRet) {
        Init();
        return false;
    }

    return true ;
}

EN_MANUAL_CYCLE __fastcall CManualMan::GetManNo()
{
    return m_iManNo ;
}

//---------------------------------------------------------------------------
void __fastcall CManualMan::Update(void)
{
    if( SEQ._bRun) {
        iHedManCol = -1 ;
        iHedManRow = -1 ;

        iTrmManCol = -1 ;
        iTrmManRow = -1 ;
    }

    if( m_iManNo == mcNoneCycle ) return  ;

    if(!OM.CmnOptn.bDryRun  )SEQ.InspectTemp ();
    if(!OM.CmnOptn.bIgnrDoor)SEQ.InspectDoor ();
    if( m_iManNo != mcLDR_Home &&
        m_iManNo != mcRAL_Home &&
        m_iManNo != mcHED_Home &&
        m_iManNo != mcPSB_Home &&
        m_iManNo != mcULD_Home &&
        m_iManNo != mcAllHome  ) SEQ.InspectHomeEnd  ();

    SEQ.InspectLightGrid();

    bool r1,r2,r3,r4,r5 ;

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return ; }
    if (!MT_GetServoAll()) { MT_SetServoAll(true);}

    //Cycle Step.
    if      (m_iManNo == mcAllHome    ) { r1 = LDR.CycleHome() || !OM.EqpOptn.bExistLoader ;
                                          r2 = RAL.CycleHome();
                                          r3 = HED.CycleHome();
                                          r4 = PSB.CycleHome();
                                          r5 = ULD.CycleHome() || !OM.EqpOptn.bExistUnLoader ;

                                          if(r1&&r2&&r3&&r4&&r5) {
                                              m_iManNo = mcNoneCycle;
                                              FM_MsgOk("Confirm","All Home Finished!");
                                          }
                                        }

    //LDR
    else if (m_iManNo == mcLDR_Home         ) { if(LDR.CycleHome   ()) m_iManNo = mcNoneCycle;   }        //LDR.CycleHome (); }
    else if (m_iManNo == mcLDR_Supply       ) { if(LDR.CycleSupply ()) m_iManNo = mcNoneCycle;   }        //LDR.InitCycleStep(); }
    else if (m_iManNo == mcLDR_Pick         ) { if(LDR.CyclePick   ()) m_iManNo = mcNoneCycle;   }        //LDR.InitCycleStep(); }
    else if (m_iManNo == mcLDR_Work         ) { if(LDR.CycleWork   ()&&
                                                   RAL.CycleIn     ()) m_iManNo = mcNoneCycle;   }        //LDR.InitCycleStep();
    else if (m_iManNo == mcLDR_Place        ) { if(LDR.CyclePlace  ()) m_iManNo = mcNoneCycle;   }        //LDR.InitCycleStep(); }

    //RAL
    else if (m_iManNo == mcRAL_Home         ) { if(RAL.CycleHome   ()) m_iManNo = mcNoneCycle;   }
    else if (m_iManNo == mcRAL_MvAuto       ) { if(RAL.CycleMvAuto ()) m_iManNo = mcNoneCycle;   }
    else if (m_iManNo == mcRAL_Bind         ) { if(RAL.CycleBind   ()) m_iManNo = mcNoneCycle;   }

    //HED
    else if (m_iManNo == mcHED_Home         ) { if(HED.CycleHome   ()) m_iManNo = mcNoneCycle;   }
    else if (m_iManNo == mcHED_Insp         ) { if(HED.CycleInsp   ()) m_iManNo = mcNoneCycle;   }

    //PSB
    else if (m_iManNo == mcPSB_Home         ) { if(PSB.CycleHome   ()) m_iManNo = mcNoneCycle;   }
    else if (m_iManNo == mcPSB_Flip         ) { if(PSB.CycleFlip   ()) m_iManNo = mcNoneCycle;   }
    else if (m_iManNo == mcPSB_Work         ) { if(PSB.CycleWork   ()) m_iManNo = mcNoneCycle;   }
    else if (m_iManNo == mcPSB_Out          ) { if(PSB.CycleOut    ()) m_iManNo = mcNoneCycle;   }



    //ULD
    else if (m_iManNo == mcULD_Home         ) { if(ULD.CycleHome   ()) m_iManNo = mcNoneCycle;   }
    else if (m_iManNo == mcULD_Supply       ) { if(ULD.CycleSupply ()) m_iManNo = mcNoneCycle;   }
    else if (m_iManNo == mcULD_Pick         ) { if(ULD.CyclePick   ()) m_iManNo = mcNoneCycle;   }
    else if (m_iManNo == mcULD_Work         ) { if(ULD.CycleWork   ()) m_iManNo = mcNoneCycle;   }
    else if (m_iManNo == mcULD_Place        ) { if(ULD.CyclePlace  ()) m_iManNo = mcNoneCycle;   }
//  else if (m_iManNo == mcETC_VisnTest     ) { if(RAL.CycleBind   ()) { if(HED.CycleInsp  ()) m_iManNo = mcNoneCycle; } }

    //ETC (S/W)
    else if (m_iManNo == mcHED_InspMan      ) { if(CycleManBindInsp()) m_iManNo = mcNoneCycle;   }

    //Default
    else                                      { m_iManNo = mcNoneCycle ; }














/*    else if (m_iManNo == mcLDR_8  ) { if(LDR.CycleSupply ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_9  ) { if(LDR.CyclePick   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_10 ) { if(LDR.CyclePlace  ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcLDR_11 ) { r1 = LDR.CycleWork ();
                                      r2 = RAL.CycleIn   ();
                                      if(r1 && r2         )  m_iManNo = mcNoneCycle; }


    else if (m_iManNo == mcMRK_97 ) { if(PSB.CycleWork()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcRAL_Home   ) { if(RAL.CycleHome   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcRAL_MvAuto ) { r1 = RAL.CycleMvAuto ();
                                          r2 = PSB.GetCycleStep() ? PSB.CycleIn() : true ;
                                          if(r1 && r2          ) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcULD_115 ) { if(ULD.CycleSupply ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_116 ) { if(ULD.CyclePick   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_117 ) { if(ULD.CyclePlace  ()) m_iManNo = mcNoneCycle; }
*/

/*
    else if (m_iManNo == mcHED_Home   ) { if(HED.CycleHome   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcHED_Insp   ) { if(HED.CycleInsp   ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcPSB_Home   ) { if(PSB.CycleHome   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_In     ) { if(PSB.CycleIn     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_Flip   ) { if(PSB.CycleFlip   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_Work   ) { if(PSB.CycleWork   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_Out    ) { if(PSB.CycleOut    ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcULD_Home   ) { if(ULD.CycleHome   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Supply ) { if(ULD.CycleSupply ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Pick   ) { if(ULD.CyclePick   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Work   ) { if(ULD.CycleWork   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Place  ) { if(ULD.CyclePlace  ()) m_iManNo = mcNoneCycle; }
    else                                {                        m_iManNo = mcNoneCycle; }
*/
    //Ok.
    return ;
}
bool CManualMan::CycleManBindInsp()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 20000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        DM.ARAY[riWK1] = DM.ARAY[riWB1] ;
        DM.ARAY[riWK2] = DM.ARAY[riWB2] ;
        m_iManStep = 0 ;
        return true;
    }

    if(m_iManStep != m_iPreManStep) {
        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
    }

    m_iPreManStep = m_iManStep ;

    switch (m_iManStep) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 m_iManStep = 0 ;
                 return true ;

        case  10: AT_MoveCyl(aiWK1_Stopper , ccFwd);
                  AT_MoveCyl(aiWK2_Stopper , ccFwd);

                  DM.ARAY[riWB1] = DM.ARAY[riWK1] ;
                  DM.ARAY[riWB2] = DM.ARAY[riWK2] ;

                  m_iManStep++;
                  return false ;

        case  11: if(!AT_MoveCyl(aiWK1_Stopper , ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK2_Stopper , ccFwd)) return false ;
                  IO_SetY(yWRK_Belt , true);
                  m_tmTemp.Clear();
                  m_iManStep++;
                  return false ;

        case  12: if(!m_tmTemp.OnDelay(true , 50)) return false ;
                  IO_SetY(yWRK_Belt , false);
                  if(!IO_GetX(xWK1_Pkg) && !DM.ARAY[riWK1].GetCntExist() ) DM.ARAY[riWK1].SetStat(csUnkwn);
                  if(!IO_GetX(xWK2_Pkg) && !DM.ARAY[riWK2].GetCntExist() ) DM.ARAY[riWK2].SetStat(csUnkwn);
                  AT_MoveCyl(aiWK1_Align , ccFwd);
                  AT_MoveCyl(aiWK2_Align , ccFwd);

                  m_iManStep++ ;
                  return false ;

        case  13: if(!AT_MoveCyl(aiWK1_Align , ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK2_Align , ccFwd)) return false ;
                  AT_MoveCyl(aiWK1_Lift , ccFwd);
                  AT_MoveCyl(aiWK2_Lift , ccFwd);
                  m_iManStep++ ;
                  return false ;

        case  14: if(!AT_MoveCyl(aiWK1_Lift , ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK2_Lift , ccFwd)) return false ;
                  AT_MoveCyl(aiWK1_Align , ccBwd);
                  AT_MoveCyl(aiWK2_Align , ccBwd);
                  m_iManStep++ ;
                  return false ;

        case  15: if(!AT_MoveCyl(aiWK1_Align , ccBwd)) return false ;
                  if(!AT_MoveCyl(aiWK2_Align , ccBwd)) return false ;
                  //스테이지 올리고 바꿔야 바인딩 되고 HEAD에서 검사를 제때 시작 한다.
                  DM.ARAY[riVS1].SetStat(csUnkwn);
                  DM.ARAY[riVS2].SetStat(csUnkwn);
                  DM.ARAY[riWK1].SetStat(csUnkwn);
                  DM.ARAY[riWK2].SetStat(csUnkwn);



                  HED.InitCycleStep();
                  m_iManStep++ ;
                  return false ;

        case  16: m_tmCycle.Clear();
                  if(!HED.CycleInsp()) return false ;
                  AT_MoveCyl(aiWK1_Stopper , ccFwd);
                  AT_MoveCyl(aiWK2_Stopper , ccFwd);
                  m_iManStep++ ;
                  return false ;

        case  17: if(!AT_MoveCyl(aiWK1_Stopper , ccFwd)) return false;
                  if(!AT_MoveCyl(aiWK2_Stopper , ccFwd)) return false;
                  AT_MoveCyl(aiWK1_Lift , ccBwd);
                  AT_MoveCyl(aiWK2_Lift , ccBwd);
                  m_iManStep++ ;
                  return false ;

        case  18: if(!AT_MoveCyl(aiWK1_Lift , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK2_Lift , ccBwd)) return false;
                  DM.ARAY[riWK1] = DM.ARAY[riWB1] ;
                  DM.ARAY[riWK2] = DM.ARAY[riWB2] ;
                  DM.ARAY[riVS1].SetStat(csNone);
                  DM.ARAY[riVS2].SetStat(csNone);
                  m_iManStep = 0 ;
                  return true ;
    }
}


