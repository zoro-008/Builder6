//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ManualMan.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff.h"
#include "Rail.h"
#include "Laser.h"
#include "MarkVisn.h"
#include "PostBuff.h"
#include "Unloader.h"
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
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

    if(!IO_GetX(xETC_PowerOn)                          ) {FM_MsgOk("ERR","Power On Plz");      return false ;}
    if(!IO_GetX(xETC_MainAir) || !IO_GetY(yETC_MainSol)) {FM_MsgOk("ERROR","Check Main Air!"); return false ;}

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

         if(m_iManNo == mcAllHome         ) {if(IO_GetX(xPRB_PkgIn1 )              ){FM_MsgOk("ERROR","Check Rail In Sensor!" ); bRet = false ;}
                                             if(IO_GetX(xPSB_PkgOut6)              ){FM_MsgOk("ERROR","Check Rail Out Sensor!"); bRet = false ;}}
                                             //if(bNothingRalMask && !bNothingRalPakg){FM_MsgOk("ERROR","Check Rail On Package!"); bRet = false ;}}
    else if(m_iManNo == mcRailOutAll      ) {}

    else if(m_iManNo == mcLDR_Home        ) {if(IO_GetX(xPRB_PkgIn1 )                      ) {FM_MsgOk("ERROR","Check Rail In Sensor!" ); bRet = false ;}}
    else if(m_iManNo == mcLDR_Supply      ) {}
    else if(m_iManNo == mcLDR_Pick        ) {if(!DM.ARAY[riLDR].CheckAllStat(csNone)       ) {FM_MsgOk("ERR","Remove Mgz"              ); bRet = false ;}}
    else if(m_iManNo == mcLDR_SttPstn     ) {}
    else if(m_iManNo == mcLDR_Place       ) {}
    else if(m_iManNo == mcLDR_UpSlPitch   ) {if( MT_GetCmdPos(miLDR_YClmp) != PM.GetValue(miLDR_YClmp , pvLDR_YClmpWork)) {FM_MsgOk("ERR","Y Axis is Not Work Position!" ); bRet = false ;}
                                             if(!LDR.CheckSafe(miLDR_ZClmp,piLDR_ZClmpClampOff)                         ) {                                                 bRet = false ;}}
    else if(m_iManNo == mcLDR_DnSlPitch   ) {if( MT_GetCmdPos(miLDR_YClmp) != PM.GetValue(miLDR_YClmp , pvLDR_YClmpWork)) {FM_MsgOk("ERR","Y Axis is Not Work Position!" ); bRet = false ;}
                                             if(!LDR.CheckSafe(miLDR_ZClmp,piLDR_ZClmpClampOff)                         ) {                                                 bRet = false ;}}
    else if(m_iManNo == mcLDR_PusherFwBw  ) {}
    else if(m_iManNo == mcLDR_ClampDnUp   ) {}
    else if(m_iManNo == mcLDR_TranInAC    ) {}
    else if(m_iManNo == mcLDR_TranOutAC   ) {}

    else if(m_iManNo == mcPRB_Home        ) {}
    else if(m_iManNo == mcPRB_StprUpDn    ) {if(IO_GetY(yPRB_FeedAc) == true){ FM_MsgOk("ERROR","PreBuffer AC Motor is Running" ); bRet = false ; }}
    else if(m_iManNo == mcPRB_FeedAC      ) {}
    else if(m_iManNo == mcPRB_AirBlow     ) {}
    else if(m_iManNo == mcPRB_CycleVisn   ) {}

    else if(m_iManNo == mcRAL_Home        ) {}
    else if(m_iManNo == mcRAL_LStpUpDn    ) {if(IO_GetY(yRAL_FeedAc) == true){ FM_MsgOk("ERROR","Rail AC Motor is Running" ); bRet = false ; }}
    else if(m_iManNo == mcRAL_LStnUpDn    ) {}
    else if(m_iManNo == mcRAL_LAlnFwBw    ) {}
    else if(m_iManNo == mcRAL_VStpUpDn    ) {if(IO_GetY(yRAL_FeedAc) == true){ FM_MsgOk("ERROR","Rail AC Motor is Running" ); bRet = false ; }}
    else if(m_iManNo == mcRAL_VStnUpDn    ) {}
    else if(m_iManNo == mcRAL_VAlnFwBw    ) {}
    else if(m_iManNo == mcRAL_Bind        ) {}
    else if(m_iManNo == mcRAL_UnBind      ) {}
    else if(m_iManNo == mcRAL_FeedAC      ) {}
    else if(m_iManNo == mcRAL_MoveNextPart) {if(!IO_GetX(xRAL_LPkg3)){ FM_MsgOk("ERROR","Laser Zone is Empty"     ); bRet = false ; }
                                             if( IO_GetX(xRAL_VPkg4)){ FM_MsgOk("ERROR","Vision Zone exist Strip" ); bRet = false ; }}

    else if(m_iManNo == mcLSR_Home        ) {}
    else if(m_iManNo == mcLSR_AVsnFwBw    ) {}
    else if(m_iManNo == mcLSR_CycleLsr    ) {}
    else if(m_iManNo == mcLSR_CycleAVsn   ) {}
    else if(m_iManNo == mcLSR_CycleWork   ) {if(!IO_GetX(xRAL_LPkg3)){ FM_MsgOk("ERROR","Laser Zone is Empty");  bRet = false ; }}

    else if(m_iManNo == mcVSN_Home        ) {}
    else if(m_iManNo == mcVSN_XPitchP     ) {}
    else if(m_iManNo == mcVSN_XPitchN     ) {}
    else if(m_iManNo == mcVSN_YPitchP     ) {}
    else if(m_iManNo == mcVSN_YPitchN     ) {}
    else if(m_iManNo == mcVSN_CycleVisn   ) {}
    else if(m_iManNo == mcVSN_CycleSttPos ) {}
    else if(m_iManNo == mcVSN_CycleTrain  ) {}
    else if(m_iManNo == mcVSN_MaskChange  ) {}

    else if(m_iManNo == mcPSB_Home        ) {}
    else if(m_iManNo == mcPSB_GrprGrRl    ) {}
    else if(m_iManNo == mcPSB_RailOpCl    ) {}
    else if(m_iManNo == mcPSB_IndxDnUp    ) {}
    else if(m_iManNo == mcPSB_ElvtUpDn    ) {}
    else if(m_iManNo == mcPSB_PshrDnUp    ) {}
    else if(m_iManNo == mcPSB_PshrFwBw    ) {}
    else if(m_iManNo == mcPSB_CycleReject ) {}

    else if(m_iManNo == mcULD_Home        ) {if(IO_GetX(xPSB_PkgOut6)                        ) {FM_MsgOk("ERROR","Check Rail Out Sensor!"); bRet = false ;}}
    else if(m_iManNo == mcULD_Supply      ) {}
    else if(m_iManNo == mcULD_Pick        ) {}
    else if(m_iManNo == mcULD_SttPstn     ) {}
    else if(m_iManNo == mcULD_Place       ) {}
    else if(m_iManNo == mcULD_UpSlPitch   ) {if( MT_GetCmdPos(miULD_YClmp) != PM.GetValue(miULD_YClmp , pvULD_YClmpWork)) {FM_MsgOk("ERR","Y Axis is Not Work Position!" ); bRet = false ;}
                                             if(!ULD.CheckSafe(miULD_ZClmp,piULD_ZClmpWork)                             ) {                                                 bRet = false ;}}
    else if(m_iManNo == mcULD_DnSlPitch   ) {if( MT_GetCmdPos(miULD_YClmp) != PM.GetValue(miULD_YClmp , pvULD_YClmpWork)) {FM_MsgOk("ERR","Y Axis is Not Work Position!" ); bRet = false ;}
                                             if(!ULD.CheckSafe(miULD_ZClmp,piULD_ZClmpWork)                             ) {                                                 bRet = false ;}}
    else if(m_iManNo == mcULD_ClampDnUp   ) {}
    else if(m_iManNo == mcPSB_PusherFwBw  ) {}
    else if(m_iManNo == mcULD_TranInAC    ) {}
    else if(m_iManNo == mcULD_TranOutAC   ) {}

    if(!bRet) Init();


    /********************/
    /* 처리..           */
    /********************/

    bool bLsrYWaitPos = PM.GetValue(miLSR_YAlVs , pvLSR_YAlVsWait) - 0.1 <= MT_GetCmdPos(miLSR_YAlVs) &&
                        MT_GetCmdPos(miLSR_YAlVs) <= PM.GetValue(miLSR_YAlVs , pvLSR_YAlVsWait) + 0.1  ;
    //bool bLsrYWorkPos = PM.GetValue(miLSR_YAlVs , pvLSR_YAlVsWork) - 0.1 <= MT_GetCmdPos(miLSR_YAlVs) &&
    //                    MT_GetCmdPos(miLSR_YAlVs) <= PM.GetValue(miLSR_YAlVs , pvLSR_YAlVsWork) + 0.1  ;




         if(m_iManNo == mcAllHome         ) {PRB.InitHomeStep ();
                                             RAL.InitHomeStep ();
                                             LSR.InitHomeStep ();
                                             VSN.InitHomeStep ();
                                             PSB.InitHomeStep ();
                                             LDR.InitHomeStep ();
                                             ULD.InitHomeStep ();}

    else if(m_iManNo == mcRailOutAll      ) {m_iManStep = 10 ; m_iPreManStep = 0;                      }

    else if(m_iManNo == mcLDR_Home        ) {LDR.InitHomeStep ();                                      }
    else if(m_iManNo == mcLDR_Supply      ) {LDR.InitCycleStep();                                      }
    else if(m_iManNo == mcLDR_Pick        ) {LDR.InitCycleStep();                                      }
    else if(m_iManNo == mcLDR_SttPstn     ) {LDR.MoveMotr(miLDR_ZClmp , piLDR_ZClmpWorkStart);         }
    else if(m_iManNo == mcLDR_Place       ) {LDR.InitCycleStep();                                      }
    else if(m_iManNo == mcLDR_UpSlPitch   ) {MT_GoIncMan(miLDR_ZClmp ,-OM.DevInfo.dCsSlPitch);         }
    else if(m_iManNo == mcLDR_DnSlPitch   ) {MT_GoIncMan(miLDR_ZClmp , OM.DevInfo.dCsSlPitch);         }
    else if(m_iManNo == mcLDR_PusherFwBw  ) {LDR.MoveActr(aiLDR_PshrFwBw , !AT_GetCmd(aiLDR_PshrFwBw));}
    else if(m_iManNo == mcLDR_ClampDnUp   ) {LDR.MoveActr(aiLDR_ClmpUpDn , !AT_GetCmd(aiLDR_ClmpUpDn));}
    else if(m_iManNo == mcLDR_TranInAC    ) {IO_SetY(yLDR_MgzInAC  , !IO_GetY(yLDR_MgzInAC ));         }
    else if(m_iManNo == mcLDR_TranOutAC   ) {IO_SetY(yLDR_MgzOutAC , !IO_GetY(yLDR_MgzOutAC));         }

    else if(m_iManNo == mcPRB_Home        ) {PRB.InitHomeStep();                                       }
    else if(m_iManNo == mcPRB_StprUpDn    ) {PRB.MoveActr(aiPRB_StprUpDn , !AT_GetCmd(aiPRB_StprUpDn));}
    else if(m_iManNo == mcPRB_FeedAC      ) {IO_SetY(yPRB_FeedAc  , !IO_GetY(yPRB_FeedAc  ));          }
    else if(m_iManNo == mcPRB_AirBlow     ) {IO_SetY(yPRB_AirBlow , !IO_GetY(yPRB_AirBlow ));          }
    else if(m_iManNo == mcPRB_CycleVisn   ) {PRB.InitCycleStep ();}


    else if(m_iManNo == mcRAL_Home        ) {RAL.InitHomeStep();                                       }
    else if(m_iManNo == mcRAL_LStpUpDn    ) {RAL.MoveActr(aiRAL_LStpUpDn , !AT_GetCmd(aiRAL_LStpUpDn));}
    else if(m_iManNo == mcRAL_LStnUpDn    ) {RAL.MoveActr(aiRAL_LStnUpDn , !AT_GetCmd(aiRAL_LStnUpDn));}
    else if(m_iManNo == mcRAL_LAlnFwBw    ) {RAL.MoveActr(aiRAL_LAlnFwBw , !AT_GetCmd(aiRAL_LAlnFwBw));}
    else if(m_iManNo == mcRAL_VStpUpDn    ) {RAL.MoveActr(aiRAL_VStpUpDn , !AT_GetCmd(aiRAL_VStpUpDn));}
    else if(m_iManNo == mcRAL_VStnUpDn    ) {RAL.MoveActr(aiRAL_VStnUpDn , !AT_GetCmd(aiRAL_VStnUpDn));}
    else if(m_iManNo == mcRAL_VAlnFwBw    ) {RAL.MoveActr(aiRAL_VAlnFwBw , !AT_GetCmd(aiRAL_VAlnFwBw));}
    else if(m_iManNo == mcRAL_Bind        ) {RAL.InitCycleStep();                                      }
    else if(m_iManNo == mcRAL_UnBind      ) {RAL.MoveActr(aiRAL_LStnUpDn , ccBwd); RAL.MoveActr(aiRAL_VStnUpDn , ccBwd);}
    else if(m_iManNo == mcRAL_FeedAC      ) {IO_SetY(yRAL_FeedAc  , !IO_GetY(yRAL_FeedAc ));           }
    else if(m_iManNo == mcRAL_MoveNextPart) {m_iManStep = 10 ; m_iPreManStep = 0;                      }

    else if(m_iManNo == mcLSR_Home        ) {LSR.InitHomeStep();                                       }
    else if(m_iManNo == mcLSR_AVsnFwBw    ) {if(bLsrYWaitPos){LSR.MoveMotr(miLSR_YAlVs,piLSR_YAlVsWork);}
                                             else            {LSR.MoveMotr(miLSR_YAlVs,piLSR_YAlVsWait);}}
    else if(m_iManNo == mcLSR_CycleLsr    ) {LSR.InitCycleStep();                                      }
    else if(m_iManNo == mcLSR_CycleAVsn   ) {LSR.InitCycleStep();                                      }
    else if(m_iManNo == mcLSR_CycleWork   ) {m_iManStep = 10 ; m_iPreManStep = 0;                      }

    else if(m_iManNo == mcVSN_Home        ) {VSN.InitHomeStep();                                       }
    else if(m_iManNo == mcVSN_XPitchP     ) {MT_GoIncMan(miVSN_XInsp , OM.DevInfo.dColPitch);          }  //sunsun 피치계산 해서 움직이게.
    else if(m_iManNo == mcVSN_XPitchN     ) {MT_GoIncMan(miVSN_XInsp ,-OM.DevInfo.dColPitch);          }
    else if(m_iManNo == mcVSN_YPitchP     ) {MT_GoIncMan(miVSN_YInsp , OM.DevInfo.dRowPitch);          }
    else if(m_iManNo == mcVSN_YPitchN     ) {MT_GoIncMan(miVSN_YInsp ,-OM.DevInfo.dRowPitch);          }
    else if(m_iManNo == mcVSN_CycleVisn   ) {m_iManStep = 10 ; m_iPreManStep = 0;                      }
    else if(m_iManNo == mcVSN_CycleTrain  ) {VSN.InitCycleStep();                                      }
    else if(m_iManNo == mcVSN_CycleSttPos ) {VSN.MoveMotr(miVSN_XInsp , piVSN_XInspWorkStt);
                                             VSN.MoveMotr(miVSN_YInsp , piVSN_YInspWorkStt);           }
    else if(m_iManNo == mcVSN_MaskChange  ) {VSN.MoveMotr(miVSN_XInsp , piVSN_XInspMskChange);
                                             VSN.MoveMotr(miVSN_YInsp , piVSN_YInspMskChange);         }

    else if(m_iManNo == mcPSB_Home        ) {PSB.InitHomeStep();                                       }
    else if(m_iManNo == mcPSB_GrprGrRl    ) {PSB.MoveActr(aiPSB_GrprGrRl , !AT_GetCmd(aiPSB_GrprGrRl));}
    else if(m_iManNo == mcPSB_RailOpCl    ) {PSB.MoveActr(aiPSB_RailOpCl , !AT_GetCmd(aiPSB_RailOpCl));}
    else if(m_iManNo == mcPSB_IndxDnUp    ) {PSB.MoveActr(aiPSB_IndxDnUp , !AT_GetCmd(aiPSB_IndxDnUp));}
    else if(m_iManNo == mcPSB_ElvtUpDn    ) {PSB.MoveActr(aiPSB_ElvtUpDn , !AT_GetCmd(aiPSB_ElvtUpDn));}
    else if(m_iManNo == mcPSB_PshrDnUp    ) {PSB.MoveActr(aiPSB_PshrDnUp , !AT_GetCmd(aiPSB_PshrDnUp));}
    else if(m_iManNo == mcPSB_PshrFwBw    ) {PSB.MoveActr(aiPSB_PshrFwBw , !AT_GetCmd(aiPSB_PshrFwBw));}
    else if(m_iManNo == mcPSB_CycleReject ) {PSB.InitCycleStep();                                      }

    else if(m_iManNo == mcULD_Home        ) {ULD.InitHomeStep ();                                      }
    else if(m_iManNo == mcULD_Supply      ) {ULD.InitCycleStep();                                      }
    else if(m_iManNo == mcULD_Pick        ) {ULD.InitCycleStep();                                      }
    else if(m_iManNo == mcULD_SttPstn     ) {ULD.MoveMotr(miULD_ZClmp , piULD_ZClmpWorkStart);         }
    else if(m_iManNo == mcULD_Place       ) {ULD.InitCycleStep();                                      }
    else if(m_iManNo == mcULD_UpSlPitch   ) {MT_GoIncMan(miULD_ZClmp ,-OM.DevInfo.dCsSlPitch);         }
    else if(m_iManNo == mcULD_DnSlPitch   ) {MT_GoIncMan(miULD_ZClmp , OM.DevInfo.dCsSlPitch);         }
    else if(m_iManNo == mcULD_ClampDnUp   ) {ULD.MoveActr(aiULD_ClmpUpDn , !AT_GetCmd(aiULD_ClmpUpDn));}
    else if(m_iManNo == mcPSB_PusherFwBw  ) {PSB.MoveActr(aiPSB_PshrDnUp , !AT_GetCmd(aiPSB_PshrDnUp));}
    else if(m_iManNo == mcULD_TranInAC    ) {IO_SetY(yULD_MgzInAC  , !IO_GetY(yULD_MgzInAC ));         }
    else if(m_iManNo == mcULD_TranOutAC   ) {IO_SetY(yULD_MgzOutAC , !IO_GetY(yULD_MgzOutAC));         }

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

    if(!OM.CmnOptn.bIgnrDoor    ) SEQ.InspectDoor    ();
    if(m_iManNo != mcPRB_Home &&
       m_iManNo != mcRAL_Home &&
       m_iManNo != mcLSR_Home &&
       m_iManNo != mcVSN_Home &&
       m_iManNo != mcPSB_Home &&
       m_iManNo != mcLDR_Home &&
       m_iManNo != mcULD_Home &&
       m_iManNo != mcAllHome    ) SEQ.InspectHomeEnd ();
    SEQ.InspectLightGrid();

    bool r1,r2,r3,r4,r5,r6,r7 ;
    r1=r2=r3=r4=r5=r6=r7 = false ;

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return ; }
    //Cycle Step.
    if      (m_iManNo == mcAllHome      ) { r1  = RAL.CycleHome();
                                            r2  = PSB.CycleHome();
                                            r3  = LDR.CycleHome();
                                            r4  = PRB.CycleHome();
                                            r5  = LSR.CycleHome();
                                            r6  = VSN.CycleHome();
                                            r7  = ULD.CycleHome();

                                            if(r1&&r2&&r3&&r4&&r5&&r6&&r7) {
                                                MT_SetHomeEnd(miETC_Spare , true);
                                                m_iManNo = mcNoneCycle;
                                                FM_MsgOk("Confirm","All Home Finished!");
                                            }
                                          }
    else if(m_iManNo == mcRailOutAll      ) {if(CycleRailOutAll       ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcLDR_Home        ) {if(LDR.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_Supply      ) {if(LDR.CycleSupply       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_Pick        ) {if(LDR.CyclePick         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_Place       ) {if(LDR.CyclePlace        ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcPRB_Home        ) {if(PRB.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPRB_CycleVisn   ) {if(PRB.CycleOriVisn      ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcRAL_Home        ) {if(RAL.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRAL_Bind        ) {if(RAL.CycleBind         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRAL_MoveNextPart) {if(CycleRailMoveNextPart ()) m_iManNo = mcNoneCycle; }






    else if(m_iManNo == mcLSR_Home        ) {if(LSR.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLSR_CycleLsr    ) {if(LSR.CycleLaser        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLSR_CycleAVsn   ) {if(LSR.CycleVisn         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLSR_CycleWork   ) {if(CycleAlgnLaser        ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcVSN_Home        ) {if(VSN.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcVSN_CycleVisn   ) {if(CycleMarkVisn         ()) m_iManNo = mcNoneCycle; } //sun sun 이거 동작 제대로 하게 되어 있는지 확인.
    else if(m_iManNo == mcVSN_CycleTrain  ) {if(VSN.CycleTrain        ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcPSB_Home        ) {if(PSB.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPSB_CycleReject ) {if(PSB.CycleReject       ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcULD_Home        ) {if(ULD.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_Supply      ) {if(ULD.CycleSupply       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_Pick        ) {if(ULD.CyclePick         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_Place       ) {if(ULD.CyclePlace        ()) m_iManNo = mcNoneCycle; }

    else                                    {                        m_iManNo = mcNoneCycle; } //여기서 갱신됌.

    //Ok.
    return ;

}

//---------------------------------------------------------------------------
bool CManualMan::CycleAlgnLaser()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    int iTimeOutTime = m_iManStep == 16 ? 30000 : 5000 ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, iTimeOutTime )) {
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

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: RAL.MoveActr(aiRAL_LStpUpDn , ccFwd);
                  m_iManStep++;
                  return false ;

        case  11: if(!RAL.MoveActr(aiRAL_LStpUpDn, ccFwd)) return false ;
                  IO_SetY(yRAL_FeedAc , true);
                  m_tmDelay.Clear();
                  m_iManStep++;
                  return false ;



        //모든 종료 확인. 위에서 숫자씀 조심.
        case  12: if(!m_tmDelay.OnDelay(true , 500)) return false ;
                  RAL.MoveActr(aiRAL_LAlnFwBw , ccFwd);

                  m_iManStep++ ;
                  return false ;

        case  13: if(!RAL.MoveActr(aiRAL_LAlnFwBw , ccFwd)) return false ;

                  RAL.MoveActr(aiRAL_LStnUpDn , ccFwd);
                  m_iManStep++;
                  return false ;

        case  14: if(!RAL.MoveActr(aiRAL_LStnUpDn , ccFwd)) return false ;
                  RAL.MoveActr(aiRAL_LAlnFwBw , ccBwd);
                  IO_SetY(yRAL_FeedAc , false);
                  m_iManStep++ ;
                  return false ;

        case  15: if(!RAL.MoveActr(aiRAL_LAlnFwBw , ccBwd)) return false ;

                  LSR.InitCycleStep();
                  m_iManStep++;
                  return false ;

        //번호 타임아웃에서 씀 주의..
        case  16: if(!LSR.CycleWork()) return false ;
                  RAL.MoveActr(aiRAL_LStnUpDn , ccBwd);
                  RAL.MoveActr(aiRAL_VStnUpDn , ccBwd); //바인딩 동작 하고 했을경우를 위해.
                  m_iManStep++;
                  return false ;

        case  17: if(!RAL.MoveActr(aiRAL_LStnUpDn , ccBwd)) return false ;
                  if(!RAL.MoveActr(aiRAL_VStnUpDn , ccBwd)) return false ;
                  m_iManStep = 0 ;
                  return true;

        }

}

bool CManualMan::CycleMarkVisn ()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    int iTimeOutTime = m_iManStep == 15 ? 20000 : 5000 ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, iTimeOutTime )) {
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

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: RAL.MoveActr(aiRAL_VStpUpDn , ccFwd);
                  m_iManStep++;
                  return false ;

        case  11: if(!RAL.MoveActr(aiRAL_VStpUpDn , ccFwd)) return false ;
                  IO_SetY(yRAL_FeedAc , true);
                  m_tmDelay.Clear();
                  m_iManStep++;
                  return false ;



        //모든 종료 확인. 위에서 숫자씀 조심.
        case  12: if(!m_tmDelay.OnDelay(true , 500)) return false ;
                  RAL.MoveActr(aiRAL_VAlnFwBw , ccFwd);

                  m_iManStep++ ;
                  return false ;

        case  13: if(!RAL.MoveActr(aiRAL_VAlnFwBw , ccFwd)) return false ;

                  RAL.MoveActr(aiRAL_VStnUpDn , ccFwd);
                  m_iManStep++;
                  return false ;

        case  14: if(!RAL.MoveActr(aiRAL_VStnUpDn , ccFwd)) return false ;

                  IO_SetY(yRAL_FeedAc , false);
                  VSN.InitCycleStep();
                  m_iManStep++;
                  return false ;

        case  15: if(!VSN.CycleVisn()) return false ;


                  m_iManStep = 0 ;
                  return true;

        }

}


bool CManualMan::CycleRailMoveNextPart()
{
        //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 5000 )) {
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

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: IO_SetY(yRAL_FeedAc , true);

                  m_iManStep++;
                  return false ;

        case  11: RAL.MoveActr(aiRAL_VStnUpDn , ccBwd);
                  RAL.MoveActr(aiRAL_VAlnFwBw , ccBwd);
                  RAL.MoveActr(aiRAL_LStnUpDn , ccBwd);
                  RAL.MoveActr(aiRAL_LAlnFwBw , ccBwd);
                  RAL.MoveActr(aiRAL_LStpUpDn , ccFwd);

                  m_iManStep++;
                  return false ;

        case  12: if(!RAL.MoveActr(aiRAL_VStnUpDn , ccBwd)) return false;
                  if(!RAL.MoveActr(aiRAL_VAlnFwBw , ccBwd)) return false;
                  if(!RAL.MoveActr(aiRAL_LStnUpDn , ccBwd)) return false;
                  if(!RAL.MoveActr(aiRAL_LAlnFwBw , ccBwd)) return false;
                  if(!RAL.MoveActr(aiRAL_LStpUpDn , ccFwd)) return false;

                  RAL.MoveActr(aiRAL_LStpUpDn , ccBwd);

                  m_iManStep++ ;
                  return false ;

        case  13: if(!RAL.MoveActr(aiRAL_LStpUpDn , ccBwd)) return false ;

                  m_tmDelay.Clear();

                  m_iManStep++;
                  return false ;

        case  14: if(!m_tmDelay.OnDelay(true , 2000)) return false ;

                  RAL.MoveActr(aiRAL_LStpUpDn , ccFwd);

                  m_iManStep++;
                  return false ;

        case  15: if(!RAL.MoveActr(aiRAL_LStpUpDn , ccFwd)) return false ;

                  IO_SetY(yRAL_FeedAc , false);
                  DM.ShiftArrayData(riLSR , riVSN);

                  m_iManStep = 0 ;
                  return true;

        }



}

bool CManualMan::CycleRailOutAll()
{
        //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 5000 )) {
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

    bool bNeedPSB ;
    bool bNeedPRB ;

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: if(!DM.ARAY[riVSN].CheckAllStat(csNone)) DM.ARAY[riVSN].ChangeStat(csWait,csRslt0); //굿으로 바꿈.
                  if(!DM.ARAY[riLSR].CheckAllStat(csNone)) DM.ARAY[riLSR].SetStat(csWork);
                  if(!DM.ARAY[riPRB].CheckAllStat(csNone)) DM.ARAY[riPRB].SetStat(csMask);

                  if(!DM.ARAY[riPSB].CheckAllStat(csNone)) { //포스트버퍼에 있으면 Reject 싸이클 부터.
                      m_iManStep = 20 ;
                      return false ;
                  }
                  else {
                      m_iManStep = 30 ;
                      return false ;
                  }

                  return false ;



        case  20: PSB.InitCycleStep() ;
                  m_iManStep++;
                  return false ;

        case  21: if(!PSB.CycleReject()) return false ;  //배출 사이클.
                  m_iManStep = 30 ;
                  return false ;



        case  30: if(DM.ARAY[riPRB].CheckAllStat(csNone) &&
                     DM.ARAY[riLSR].CheckAllStat(csNone) &&
                     DM.ARAY[riVSN].CheckAllStat(csNone) &&
                     DM.ARAY[riPSB].CheckAllStat(csNone) ){
                      m_iManStep = 0 ;
                      return true ;

                  }

                  PSB.InitCycleStep();
                  RAL.InitCycleStep();
                  PRB.InitCycleStep();

                  bNeedPSB = false ;
                  bNeedPRB = false ;


                  m_iManStep++;
                  return false ;

        case  31: r1=RAL.CycleFeed();

                  if(AT_GetCmd(aiRAL_VStpUpDn) == ccBwd ) bNeedPSB = true ;
                  if(RAL._bNeedStrip                    ) bNeedPRB = true ;

                  if(bNeedPSB){r2=PSB.CycleIn ();}
                  if(bNeedPRB){r3=PRB.CycleOut();}


                  if(!r1 || !r2 || !r3) return false ;

                  m_iManStep++;
                  return false ;

        case  32: m_iManStep=10;
                  return false ;

        }



}


































































































