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
#include "Work.h"
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

    m_bManSetting = false ;

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
    if(!IO_GetX(xETC_MainAir) || !IO_GetY(yETC_MainAir)) {FM_MsgOk("ERROR","Check Main Air!"); return false ;}

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

         if(m_iManNo == mcAllHome         ) {if(IO_GetX(xPRB_PkgIn )              ){FM_MsgOk("ERROR","Check Rail In Sensor!" ); bRet = false ;}
                                             if(IO_GetX(xPSB_PkgOut)              ){FM_MsgOk("ERROR","Check Rail Out Sensor!"); bRet = false ;}}
                                             //if(bNothingRalMask && !bNothingRalPakg){FM_MsgOk("ERROR","Check Rail On Package!"); bRet = false ;}}

    else if(m_iManNo == mcStopTODisprShot ) {}
    else if(m_iManNo == mcLDR_Home        ) {if(IO_GetX(xPRB_PkgIn )                      ) {FM_MsgOk("ERROR","Check Rail In Sensor!" ); bRet = false ;}}
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


 ///////////////////////////////////////////////////////////////////////////////
    else if(m_iManNo == mcPRB_Home        ) {}
    else if(m_iManNo == mcPRB_CycleIn     ) {}
    else if(m_iManNo == mcPRB_CycleOut    ) {}
    else if(m_iManNo == mcPRB_AirBlow     ) {}
    else if(m_iManNo == mcPRB_IdxUpDn     ) {}

    else if(m_iManNo == mcWRK_Home        ) {}
    else if(m_iManNo == mcWRK_CycleIn     ) {}
    else if(m_iManNo == mcWRK_CycleOut    ) {}
    else if(m_iManNo == mcWRK_CycleWork   ) {}
    else if(m_iManNo == mcWRK_LIdxUpDn    ) {}
    else if(m_iManNo == mcWRK_RIdxUpDn    ) {}
    else if(m_iManNo == mcWRK_DspsWait    ) {}
    else if(m_iManNo == mcWRK_DspsWork    ) {}


    else if(m_iManNo == mcPSB_Home        ) {}
    else if(m_iManNo == mcPSB_CycleOut    ) {}
    else if(m_iManNo == mcPSB_IndxDnUp    ) {}
 ///////////////////////////////////////////////////////////////////////////////

    else if(m_iManNo == mcULD_Home        ) {if(IO_GetX(xPSB_PkgOut)                        ) {FM_MsgOk("ERROR","Check Rail Out Sensor!"); bRet = false ;}}
    else if(m_iManNo == mcULD_Supply      ) {}
    else if(m_iManNo == mcULD_Pick        ) {}
    else if(m_iManNo == mcULD_SttPstn     ) {}
    else if(m_iManNo == mcULD_Place       ) {}
    else if(m_iManNo == mcULD_UpSlPitch   ) {if( MT_GetCmdPos(miULD_YClmp) != PM.GetValue(miULD_YClmp , pvULD_YClmpWork)) {FM_MsgOk("ERR","Y Axis is Not Work Position!" ); bRet = false ;}
                                             if(!ULD.CheckSafe(miULD_ZClmp, piULD_ZClmpWork)                             ) {                                                 bRet = false ;}}
    else if(m_iManNo == mcULD_DnSlPitch   ) {if( MT_GetCmdPos(miULD_YClmp) != PM.GetValue(miULD_YClmp , pvULD_YClmpWork)) {FM_MsgOk("ERR","Y Axis is Not Work Position!" ); bRet = false ;}
                                             if(!ULD.CheckSafe(miULD_ZClmp, piULD_ZClmpWork)                             ) {                                                 bRet = false ;}}
    else if(m_iManNo == mcULD_ClampDnUp   ) {}
    else if(m_iManNo == mcPSB_PusherFwBw  ) {}
    else if(m_iManNo == mcULD_TranInAC    ) {}
    else if(m_iManNo == mcULD_TranOutAC   ) {}
    else if(m_iManNo == mcETC_LDRULDPlace ) {if(IO_GetX(xPRB_PkgIn )              ){FM_MsgOk("ERROR","Check Rail In Sensor!" ); bRet = false ;}
                                             if(IO_GetX(xPSB_PkgOut)              ){FM_MsgOk("ERROR","Check Rail Out Sensor!"); bRet = false ;}}




    if(!bRet) Init();


    /********************/
    /* 처리..           */
    /********************/

         if(m_iManNo == mcAllHome         ) {PRB.InitHomeStep ();
                                             WRK.InitHomeStep ();
                                             PSB.InitHomeStep ();
                                             LDR.InitHomeStep ();
                                             ULD.InitHomeStep ();}

    else if(m_iManNo == mcStopTODisprShot ) {m_iManStep = 10; m_iPreManStep = 0;                       } 
    else if(m_iManNo == mcLDR_Home        ) {LDR.InitHomeStep ();                                      }
    else if(m_iManNo == mcLDR_Supply      ) {LDR.InitCycleStep();                                      }
    else if(m_iManNo == mcLDR_Pick        ) {LDR.InitCycleStep();                                      }
    else if(m_iManNo == mcLDR_SttPstn     ) {LDR.MoveMotr(miLDR_ZClmp , piLDR_ZClmpWorkStart);         }
    else if(m_iManNo == mcLDR_Place       ) {LDR.InitCycleStep();                                      }
    else if(m_iManNo == mcLDR_UpSlPitch   ) {MT_GoIncMan(miLDR_ZClmp ,-OM.DevInfo.dCsSlPitch);         }
    else if(m_iManNo == mcLDR_DnSlPitch   ) {MT_GoIncMan(miLDR_ZClmp , OM.DevInfo.dCsSlPitch);         }
    else if(m_iManNo == mcLDR_PusherFwBw  ) {LDR.MoveActr(aiLDR_PshFwBw  , !AT_GetCmd(aiLDR_PshFwBw ));}
    else if(m_iManNo == mcLDR_ClampDnUp   ) {LDR.MoveActr(aiLDR_CmpUpDn  , !AT_GetCmd(aiLDR_CmpUpDn ));}
    else if(m_iManNo == mcLDR_TranInAC    ) {IO_SetY(yLDR_MgzInAC  , !IO_GetY(yLDR_MgzInAC ));         }
    else if(m_iManNo == mcLDR_TranOutAC   ) {IO_SetY(yLDR_MgzOutAC , !IO_GetY(yLDR_MgzOutAC));         }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    else if(m_iManNo == mcPRB_Home        ) {PRB.InitHomeStep();                                       }
    else if(m_iManNo == mcPRB_CycleIn     ) {PRB.InitCycleStep();                                      }
    else if(m_iManNo == mcPRB_CycleOut    ) {PRB.InitCycleStep();                                      }
    else if(m_iManNo == mcPRB_AirBlow     ) {IO_SetY(yPRB_AirBlow , !IO_GetY(yPRB_AirBlow ));          }
    else if(m_iManNo == mcPRB_IdxUpDn     ) {PRB.MoveActr(aiPRB_IdxUpDn , !AT_GetCmd(aiPRB_IdxUpDn));  }


    else if(m_iManNo == mcWRK_Home        ) {WRK.InitHomeStep();                                       }
    else if(m_iManNo == mcWRK_CycleIn     ) {WRK.InitCycleStep();                                      }
    else if(m_iManNo == mcWRK_CycleOut    ) {WRK.InitCycleStep();                                      }
    else if(m_iManNo == mcWRK_CycleWork   ) {WRK.InitCycleStep();                                      }
    else if(m_iManNo == mcWRK_LIdxUpDn    ) {WRK.MoveActr(aiWRK_LIdUpDn , !AT_GetCmd(aiWRK_LIdUpDn));  }
    else if(m_iManNo == mcWRK_RIdxUpDn    ) {WRK.MoveActr(aiWRK_RIdUpDn , !AT_GetCmd(aiWRK_RIdUpDn));  }
    else if(m_iManNo == mcWRK_DspsWait    ) {m_iManStep = 10 ; m_iPreManStep = 0;                      } // 쏘고 있을 경우가 있을 수 있으니, 대기를 다르게 잡아라
    else if(m_iManNo == mcWRK_DspsWork    ) {m_iManStep = 10 ; m_iPreManStep = 0;                      } // DspsWork 동작 하나하나를 모두 적용시켜라


    else if(m_iManNo == mcPSB_Home        ) {PSB.InitHomeStep();                                       }
    else if(m_iManNo == mcPSB_CycleOut    ) {PSB.InitCycleStep();                                      }
    else if(m_iManNo == mcPSB_IndxDnUp    ) {PSB.MoveActr(aiPSB_IdxUpDn , !AT_GetCmd(aiPSB_IdxUpDn));  }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    else if(m_iManNo == mcULD_Home        ) {ULD.InitHomeStep ();                                      }
    else if(m_iManNo == mcULD_Supply      ) {ULD.InitCycleStep();                                      }
    else if(m_iManNo == mcULD_Pick        ) {ULD.InitCycleStep();                                      }
    else if(m_iManNo == mcULD_SttPstn     ) {ULD.MoveMotr(miULD_ZClmp , piULD_ZClmpWorkStart);         }
    else if(m_iManNo == mcULD_Place       ) {ULD.InitCycleStep();                                      }
    else if(m_iManNo == mcULD_UpSlPitch   ) {MT_GoIncMan(miULD_ZClmp ,-OM.DevInfo.dCsSlPitch);         }
    else if(m_iManNo == mcULD_DnSlPitch   ) {MT_GoIncMan(miULD_ZClmp , OM.DevInfo.dCsSlPitch);         }
    else if(m_iManNo == mcULD_ClampDnUp   ) {ULD.MoveActr(aiULD_CmpUpDn , !AT_GetCmd(aiULD_CmpUpDn));  }
    else if(m_iManNo == mcPSB_PusherFwBw  ) {PSB.MoveActr(aiPSB_PshFwBw , !AT_GetCmd(aiPSB_PshFwBw));  }
    else if(m_iManNo == mcULD_TranInAC    ) {IO_SetY(yULD_MgzInAC  , !IO_GetY(yULD_MgzInAC ));         }
    else if(m_iManNo == mcULD_TranOutAC   ) {IO_SetY(yULD_MgzOutAC , !IO_GetY(yULD_MgzOutAC));         }

    else if (m_iManNo == mcETC_LDRULDPlace) { LDR.InitCycleStep();   ULD.InitCycleStep();              }




    else                                    { m_iManNo = mcNoneCycle ; }


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
       m_iManNo != mcWRK_Home &&
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
    if      (m_iManNo == mcAllHome      ) { r1  = PRB.CycleHome();
                                            r2  = WRK.CycleHome();
                                            r3  = PSB.CycleHome();
                                            r4  = LDR.CycleHome();
                                            r5  = ULD.CycleHome();

                                            if(r1&&r2&&r3&&r4&&r5) {
                                                m_iManNo = mcNoneCycle;
                                                FM_MsgOk("Confirm","All Home Finished!");
                                            }
                                          }

    else if (m_iManNo == mcStopTODisprShot ) { if(CycleStopToDispShot()) m_iManNo = mcNoneCycle; } 
    else if (m_iManNo == mcETC_LDRULDPlace ) { r4 = LDR.CyclePlace();
                                               r5 = ULD.CyclePlace();
                                                if(r4&&r5) m_iManNo = mcNoneCycle; }





    else if(m_iManNo == mcLDR_Home        ) {if(LDR.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_Supply      ) {if(LDR.CycleSupply       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_Pick        ) {if(LDR.CyclePick         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_Place       ) {if(LDR.CyclePlace        ()) m_iManNo = mcNoneCycle; }

 /////////////////////////////////////////////////////////////////////////////////////////////////////
    else if(m_iManNo == mcPRB_Home        ) {if(PRB.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPRB_CycleIn     ) {if(PRB.CycleIn           ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPRB_CycleOut    ) {if(PRB.CycleOut          ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcWRK_Home        ) {if(WRK.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcWRK_CycleIn     ) {if(WRK.CycleIn           ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcWRK_CycleOut    ) {if(WRK.CycleOut          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcWRK_CycleWork   ) {if(WRK.CycleWork         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcWRK_DspsWait    ) {if(CycleDispWait         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcWRK_DspsWork    ) {if(CycleDispWork         ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcPSB_Home        ) {if(PSB.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPSB_CycleOut    ) {if(PSB.CycleOut          ()) m_iManNo = mcNoneCycle; }
 /////////////////////////////////////////////////////////////////////////////////////////////////////

    else if(m_iManNo == mcULD_Home        ) {if(ULD.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_Supply      ) {if(ULD.CycleSupply       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_Pick        ) {if(ULD.CyclePick         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_Place       ) {if(ULD.CyclePlace        ()) m_iManNo = mcNoneCycle; }

    else                                    {                             m_iManNo = mcNoneCycle; } //여기서 갱신됌.

    //Ok.
    return ;

}

//------------------------------------------------------------------------------
bool CManualMan::CycleDispWait()
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

        case  10: IO_SetY(yWRK_LDispensor, false);
                  IO_SetY(yWRK_RDispensor, false);
                  m_iManStep++;
                  return false ;

        case  11: WRK.MoveMotr(miWRK_YDsps, piWRK_YDspsWait);
                  m_iManStep++;
                  return false ;

        case  12: if(!WRK.MoveMotr(miWRK_YDsps , piWRK_YDspsWait)) return false;
                  m_iManStep = 0;
                  return true;
    }
}
//------------------------------------------------------------------------------

bool CManualMan::CycleDispWork()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    int iTimeOutTime = m_iManStep == 16 ? 30000 : 5000 ;
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

        case  10: WRK.MoveMotr(miWRK_YDsps, piWRK_YDspsWork);
                  m_iManStep++;
                  return false ;

        case  11: if(!WRK.MoveMotr(miWRK_YDsps, piWRK_YDspsWork)) return false ;
                  IO_SetY(yWRK_LDispensor, true);
                  IO_SetY(yWRK_RDispensor, true);
                  m_tmLDspsDelay.Clear() ;
                  m_tmRDspsDelay.Clear() ;
                  m_iManStep++;
                  return false ;

        case  12: if(!m_tmLDspsDelay.OnDelay(true , OM.DevOptn.iDispsrLTimer)) return false ;
                  if(!m_tmRDspsDelay.OnDelay(true , OM.DevOptn.iDispsrRTimer)) return false ;
                  IO_SetY(yWRK_LDispensor, false);
                  IO_SetY(yWRK_RDispensor, false);
                  m_iManStep = 0;
                  return true ;
    }
}


bool CManualMan::CycleStopToDispShot()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    int iTimeOutTime = m_iManStep == 16 ? 30000 : 5000 ;
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

        case  10: MT_GoAbsSlow(miWRK_YDsps , PM.GetValue(miWRK_YDsps , pvWRK_YDspsWait));
                  //WRK.MoveMotr(miWRK_YDsps, piWRK_YDspsWait);
                  m_iManStep++;
                  return false ;

        case  11: if(!MT_GoAbsSlow(miWRK_YDsps , PM.GetValue(miWRK_YDsps , pvWRK_YDspsWait))) return false;
                  //if(!WRK.MoveMotr(miWRK_YDsps, piWRK_YDspsWait)) return false ;
                  IO_SetY(yWRK_LDispensor, true);
                  IO_SetY(yWRK_RDispensor, true);
                  m_tmLDspsDelay.Clear() ;
                  m_tmRDspsDelay.Clear() ;
                  m_iManStep++;
                  return false ;

        case  12: if(!m_tmLDspsDelay.OnDelay(true , OM.DevOptn.iDispsrLTimer)) return false ;
                  if(!m_tmRDspsDelay.OnDelay(true , OM.DevOptn.iDispsrRTimer)) return false ;
                  IO_SetY(yWRK_LDispensor, false);
                  IO_SetY(yWRK_RDispensor, false);
                  m_iManStep = 0;
                  return true ;
    }
}


