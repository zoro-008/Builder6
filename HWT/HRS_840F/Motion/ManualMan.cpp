//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ManualMan.h"
//---------------------------------------------------------------------------
//Part Reference
#include "Loader.h"
#include "PreRail.h"
#include "PostRail.h"
#include "Unloader.h"
//---------------------------------------------------------------------------
#include "Sequence.h"

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
    if ( EM_IsErr()      ) { Init(); return false ; } //아 밑에 처리 하는 애 때문에 잠시 이렇게 함.          //test

    EM_SetDisp(true);

    m_bManSetting = true ; //SetManCycle함수는 화면 쓰레드. 업데이트 함수에서 다른쓰레드로 들어와서 갱신하기에 플레그 걸어 막아둠.
    m_iManNo = _iNo ;

    bool bRet = true ;

    /********************/
    /* 프리프로세서     */
    /********************/

         if(m_iManNo == mcNoneCycle           ){}
    else if(m_iManNo == mcAllHome             ){}
    else if(m_iManNo == mcLDR_CycleManLoadChg ){}

    // LOADER FRONT
    else if(m_iManNo == mcLDR_FHome           ){}
    else if(m_iManNo == mcLDR_FCycleWork      ){}
    else if(m_iManNo == mcLDR_FCycleSupply    ){}
    else if(m_iManNo == mcLDR_FGrprFwBw       ){LDR_F.CheckSafe(aiLDR_FGrprFwBw    , !AT_GetCmd(aiLDR_FGrprFwBw   ));}
    else if(m_iManNo == mcLDR_FPickUpFwBw     ){LDR_F.CheckSafe(aiLDR_FPickUpFwBw  , !AT_GetCmd(aiLDR_FPickUpFwBw ));}
    else if(m_iManNo == mcLDR_FGrprUpDn       ){LDR_F.CheckSafe(aiLDR_FGrprUpDn    , !AT_GetCmd(aiLDR_FGrprUpDn   ));}
    else if(m_iManNo == mcLDR_FGrprXFwBw      ){LDR_F.CheckSafe(aiLDR_FGrprXFwBw   , !AT_GetCmd(aiLDR_FGrprXFwBw  ));}
    else if(m_iManNo == mcLDR_FLoadingFwBw    ){LDR_F.CheckSafe(aiLDR_FLoadingFwBw , !AT_GetCmd(aiLDR_FLoadingFwBw));}
    else if(m_iManNo == mcLDR_FAirBlow        ){}

    // RAIL FRONT
    else if(m_iManNo == mcRAL_FHome           ){}
    else if(m_iManNo == mcRAL_FFstAirBlower   ){}
    else if(m_iManNo == mcRAL_FSndAirBlower   ){}

    // UNLOADER FRONT
    else if(m_iManNo == mcULD_FHome           ){}
    else if(m_iManNo == mcULD_FCycleIn        ){}
    else if(m_iManNo == mcULD_FCycleWork      ){}
    else if(m_iManNo == mcULD_FCycleSupply    ){}
    else if(m_iManNo == mcULD_FFoldUpDn       ){ULD_F.CheckSafe(aiULD_FFoldUpDn     , !AT_GetCmd(aiULD_FFoldUpDn     ));}
    else if(m_iManNo == mcULD_FPickUpFwBw     ){ULD_F.CheckSafe(aiULD_FPickUpFwBw   , !AT_GetCmd(aiULD_FPickUpFwBw   ));}
    else if(m_iManNo == mcULD_FRotateFwBw     ){ULD_F.CheckSafe(aiULD_FRotateFwBw   , !AT_GetCmd(aiULD_FRotateFwBw   ));}
    else if(m_iManNo == mcULD_FPackOpenFwBw   ){ULD_F.CheckSafe(aiULD_FPackOpenFwBw , !AT_GetCmd(aiULD_FPackOpenFwBw ));}
    else if(m_iManNo == mcULD_FPackSrtFwBw    ){ULD_F.CheckSafe(aiULD_FPackSrtFwBw  , !AT_GetCmd(aiULD_FPackSrtFwBw  ));}

    // LOADER FRONT
    else if(m_iManNo == mcLDR_RHome           ){}
    else if(m_iManNo == mcLDR_RCycleWork      ){}
    else if(m_iManNo == mcLDR_RCycleSupply    ){}
    else if(m_iManNo == mcLDR_RGrprFwBw       ){LDR_R.CheckSafe(aiLDR_RGrprFwBw    , !AT_GetCmd(aiLDR_RGrprFwBw   ));}
    else if(m_iManNo == mcLDR_RPickUpFwBw     ){LDR_R.CheckSafe(aiLDR_RPickUpFwBw  , !AT_GetCmd(aiLDR_RPickUpFwBw ));}
    else if(m_iManNo == mcLDR_RGrprUpDn       ){LDR_R.CheckSafe(aiLDR_RGrprUpDn    , !AT_GetCmd(aiLDR_RGrprUpDn   ));}
    else if(m_iManNo == mcLDR_RGrprXFwBw      ){LDR_R.CheckSafe(aiLDR_RGrprXFwBw   , !AT_GetCmd(aiLDR_RGrprXFwBw  ));}
    else if(m_iManNo == mcLDR_RLoadingFwBw    ){LDR_R.CheckSafe(aiLDR_RLoadingFwBw , !AT_GetCmd(aiLDR_RLoadingFwBw));}
    else if(m_iManNo == mcLDR_RAirBlow        ){}

    // RAIL FRONT
    else if(m_iManNo == mcRAL_RHome           ){}
    else if(m_iManNo == mcRAL_RFstAirBlower   ){}
    else if(m_iManNo == mcRAL_RSndAirBlower   ){}

    // UNLOADER FRONT
    else if(m_iManNo == mcULD_RHome           ){}
    else if(m_iManNo == mcULD_RCycleIn        ){}
    else if(m_iManNo == mcULD_RCycleWork      ){}
    else if(m_iManNo == mcULD_RCycleSupply    ){}
    else if(m_iManNo == mcULD_RFoldUpDn       ){ULD_R.CheckSafe(aiULD_RFoldUpDn     , !AT_GetCmd(aiULD_RFoldUpDn    ));}
    else if(m_iManNo == mcULD_RPickUpFwBw     ){ULD_R.CheckSafe(aiULD_RPickUpFwBw   , !AT_GetCmd(aiULD_RPickUpFwBw  ));}
    else if(m_iManNo == mcULD_RRotateFwBw     ){ULD_R.CheckSafe(aiULD_RRotateFwBw   , !AT_GetCmd(aiULD_RRotateFwBw  ));}
    else if(m_iManNo == mcULD_RPackOpenFwBw   ){ULD_R.CheckSafe(aiULD_RPackOpenFwBw , !AT_GetCmd(aiULD_RPackOpenFwBw));}
    else if(m_iManNo == mcULD_RPackSrtFwBw    ){ULD_R.CheckSafe(aiULD_RPackSrtFwBw  , !AT_GetCmd(aiULD_RPackSrtFwBw ));}

    if(!bRet) Init();


    /********************/
    /* 처리..           */
    /********************/

         if(m_iManNo == mcNoneCycle           ){}
    else if(m_iManNo == mcAllHome             ){LDR_F.InitHomeStep();
                                                LDR_R.InitHomeStep();
                                                PRE_F.InitHomeStep();
                                                PRE_R.InitHomeStep();
                                                ULD_F.InitHomeStep();
                                                ULD_R.InitHomeStep();
                                               }
    else if(m_iManNo == mcLDR_CycleManLoadChg ){m_iManStep = 10; m_iPreManStep = 0; }

    // LOADER FRONT
    else if(m_iManNo == mcLDR_FHome           ){LDR_F.InitHomeStep ();                                             }
    else if(m_iManNo == mcLDR_FCycleWork      ){LDR_F.InitCycleStep();                                             }
    else if(m_iManNo == mcLDR_FCycleSupply    ){LDR_F.InitCycleStep();                                             }
    else if(m_iManNo == mcLDR_FGrprFwBw       ){AT_MoveCyl(aiLDR_FGrprFwBw    , !AT_GetCmd(aiLDR_FGrprFwBw   ));   }
    else if(m_iManNo == mcLDR_FPickUpFwBw     ){AT_MoveCyl(aiLDR_FPickUpFwBw  , !AT_GetCmd(aiLDR_FPickUpFwBw ));   }
    else if(m_iManNo == mcLDR_FGrprUpDn       ){AT_MoveCyl(aiLDR_FGrprUpDn    , !AT_GetCmd(aiLDR_FGrprUpDn   ));   }
    else if(m_iManNo == mcLDR_FGrprXFwBw      ){AT_MoveCyl(aiLDR_FGrprXFwBw   , !AT_GetCmd(aiLDR_FGrprXFwBw  ));   }
    else if(m_iManNo == mcLDR_FLoadingFwBw    ){AT_MoveCyl(aiLDR_FLoadingFwBw , !AT_GetCmd(aiLDR_FLoadingFwBw));   }
    else if(m_iManNo == mcLDR_FAirBlow        ){IO_SetY(yLDR_FAirBlowOn, !IO_GetY(yLDR_FAirBlowOn ));              }

    // RAIL FRONT
    else if(m_iManNo == mcRAL_FHome           ){PRE_F.InitHomeStep();                                              }
    else if(m_iManNo == mcRAL_FFstAirBlower   ){IO_SetY(yRAL_FFoldAirBlow1, !IO_GetY(yRAL_FFoldAirBlow1));         }
    else if(m_iManNo == mcRAL_FSndAirBlower   ){IO_SetY(yRAL_FFoldAirBlow2, !IO_GetY(yRAL_FFoldAirBlow2));         }

    // UNLOADER FRONT
    else if(m_iManNo == mcULD_FHome           ){ULD_F.InitHomeStep ();                                             }
    else if(m_iManNo == mcULD_FCycleIn        ){ULD_F.InitCycleStep();                                             }
    else if(m_iManNo == mcULD_FCycleSupply    ){ULD_F.InitCycleStep();                                             }
    else if(m_iManNo == mcULD_FCycleWork      ){ULD_F.InitCycleStep();                                             }
    else if(m_iManNo == mcULD_FFoldUpDn       ){AT_MoveCyl(aiULD_FFoldUpDn     , !AT_GetCmd(aiULD_FFoldUpDn    )); }
    else if(m_iManNo == mcULD_FPickUpFwBw     ){AT_MoveCyl(aiULD_FPickUpFwBw   , !AT_GetCmd(aiULD_FPickUpFwBw  )); }
    else if(m_iManNo == mcULD_FRotateFwBw     ){AT_MoveCyl(aiULD_FRotateFwBw   , !AT_GetCmd(aiULD_FRotateFwBw  )); }
    else if(m_iManNo == mcULD_FPackOpenFwBw   ){AT_MoveCyl(aiULD_FPackOpenFwBw , !AT_GetCmd(aiULD_FPackOpenFwBw)); }
    else if(m_iManNo == mcULD_FPackSrtFwBw    ){AT_MoveCyl(aiULD_FPackSrtFwBw  , !AT_GetCmd(aiULD_FPackSrtFwBw )); }

    // LOADER REAR
    else if(m_iManNo == mcLDR_RHome           ){LDR_R.InitHomeStep ();                                             }
    else if(m_iManNo == mcLDR_RCycleWork      ){LDR_R.InitCycleStep();                                             }
    else if(m_iManNo == mcLDR_RCycleSupply    ){LDR_R.InitCycleStep();                                             }
    else if(m_iManNo == mcLDR_RGrprFwBw       ){AT_MoveCyl(aiLDR_RGrprFwBw    , !AT_GetCmd(aiLDR_RGrprFwBw   ));   }
    else if(m_iManNo == mcLDR_RPickUpFwBw     ){AT_MoveCyl(aiLDR_RPickUpFwBw  , !AT_GetCmd(aiLDR_RPickUpFwBw ));   }
    else if(m_iManNo == mcLDR_RGrprUpDn       ){AT_MoveCyl(aiLDR_RGrprUpDn    , !AT_GetCmd(aiLDR_RGrprUpDn   ));   }
    else if(m_iManNo == mcLDR_RGrprXFwBw      ){AT_MoveCyl(aiLDR_RGrprXFwBw   , !AT_GetCmd(aiLDR_RGrprXFwBw  ));   }
    else if(m_iManNo == mcLDR_RLoadingFwBw    ){AT_MoveCyl(aiLDR_RLoadingFwBw , !AT_GetCmd(aiLDR_RLoadingFwBw));   }
    else if(m_iManNo == mcLDR_RAirBlow        ){IO_SetY(yLDR_RAirBlowOn, !IO_GetY(yLDR_RAirBlowOn ));              }

    // RAIL REAR
    else if(m_iManNo == mcRAL_RHome           ){PRE_R.InitHomeStep();                                              }
    else if(m_iManNo == mcRAL_RFstAirBlower   ){IO_SetY(yRAL_RFoldAirBlow1, !IO_GetY(yRAL_RFoldAirBlow1));         }
    else if(m_iManNo == mcRAL_RSndAirBlower   ){IO_SetY(yRAL_RFoldAirBlow2, !IO_GetY(yRAL_RFoldAirBlow2));         }

    // UNLOADER REAR
    else if(m_iManNo == mcULD_RHome           ){ULD_R.InitHomeStep ();                                             }
    else if(m_iManNo == mcULD_RCycleIn        ){ULD_R.InitCycleStep();                                             }
    else if(m_iManNo == mcULD_RCycleSupply    ){ULD_R.InitCycleStep();                                             }
    else if(m_iManNo == mcULD_RCycleWork      ){ULD_R.InitCycleStep();                                             }
    else if(m_iManNo == mcULD_RFoldUpDn       ){AT_MoveCyl(aiULD_RFoldUpDn     , !AT_GetCmd(aiULD_RFoldUpDn    )); }
    else if(m_iManNo == mcULD_RPickUpFwBw     ){AT_MoveCyl(aiULD_RPickUpFwBw   , !AT_GetCmd(aiULD_RPickUpFwBw  )); }
    else if(m_iManNo == mcULD_RRotateFwBw     ){AT_MoveCyl(aiULD_RRotateFwBw   , !AT_GetCmd(aiULD_RRotateFwBw  )); }
    else if(m_iManNo == mcULD_RPackOpenFwBw   ){AT_MoveCyl(aiULD_RPackOpenFwBw , !AT_GetCmd(aiULD_RPackOpenFwBw)); }
    else if(m_iManNo == mcULD_RPackSrtFwBw    ){AT_MoveCyl(aiULD_RPackSrtFwBw  , !AT_GetCmd(aiULD_RPackSrtFwBw )); }

    else    { m_iManNo = mcNoneCycle; }
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

    bool r1 , r2, r3, r4, r5, r6;

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return ; }

    //Cycle Step.
    if      (m_iManNo == mcAllHome         ) { r1 = LDR_F.CycleHome();
                                               r2 = LDR_R.CycleHome();
                                               r3 = PRE_F.CycleHome();
                                               r4 = PRE_R.CycleHome();
                                               r5 = ULD_F.CycleHome();
                                               r6 = ULD_R.CycleHome();
                                               if(r1 && r2 && r3 && r4 && r5 && r6 ){
                                                   m_iManNo = mcNoneCycle;
                                                   FM_MsgOk("Confirm","All Home Finished!");
                                               }
                                             }
    else if(m_iManNo == mcLDR_CycleManLoadChg ){if(CycleManLoadChg       ()) m_iManNo = mcNoneCycle; }

    // LOADER FRONT
    else if(m_iManNo == mcLDR_FHome           ){if(LDR_F.CycleHome       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_FCycleWork      ){if(LDR_F.CycleWork       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_FCycleSupply    ){if(LDR_F.CycleSupply     ()) m_iManNo = mcNoneCycle; }
    /* 로더 실린더들 추가해야함 */

    // RAIL FRONT
    else if(m_iManNo == mcRAL_FHome           ){if(PRE_F.CycleHome       ()) m_iManNo = mcNoneCycle; }

    // UNLOADER FRONT
    else if(m_iManNo == mcULD_FHome           ){if(ULD_F.CycleHome       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_FCycleIn        ){if(ULD_F.CycleIn         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_FCycleWork      ){if(ULD_F.CycleWork       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_FCycleSupply    ){if(ULD_F.CycleSupply     ()) m_iManNo = mcNoneCycle; }

    // LOADER REAR
    else if(m_iManNo == mcLDR_RHome           ){if(LDR_R.CycleHome       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_RCycleWork      ){if(LDR_R.CycleWork       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_RCycleSupply    ){if(LDR_R.CycleSupply     ()) m_iManNo = mcNoneCycle; }

    // RAIL REAR
    else if(m_iManNo == mcRAL_RHome           ){if(PRE_R.CycleHome       ()) m_iManNo = mcNoneCycle; }

    // UNLOADER REAR
    else if(m_iManNo == mcULD_RHome           ){if(ULD_R.CycleHome       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_RCycleIn        ){if(ULD_R.CycleIn         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_RCycleWork      ){if(ULD_R.CycleWork       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_RCycleSupply    ){if(ULD_R.CycleSupply     ()) m_iManNo = mcNoneCycle; }

    else    { m_iManNo = mcNoneCycle; }
    //Ok.
    return ;

}

//---------------------------------------------------------------------------
bool CManualMan::CycleManLoadChg()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    int iTimeOutTime = 15000 ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, iTimeOutTime )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , m_iManStep );
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
                  if(m_iManStep != m_iPreManStep)Trace("Manual Default", sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: IO_SetY(yLDR_FMaskUpDnCcw, true) ;
                  m_iManStep++;
                  return false ;

        case  11: if(!IO_GetX(xLDR_FMaskDnLmt))  return false ;
                  IO_SetY(yLDR_FMaskUpDnCcw, false);
                  m_iManStep++;
                  return false ;

        case  12: IO_SetY(yLDR_RMaskUpDnCcw, true) ;
                  m_iManStep++;
                  return false ;

        case  13: if(!IO_GetX(xLDR_RMaskDnLmt)) return false ;
                  IO_SetY(yLDR_RMaskUpDnCcw, false);
                  m_iManStep++;
                  return false ;

        case  14: LDR_F.MoveActr(aiLDR_FPickUpFwBw , ccBwd);
                  LDR_F.MoveActr(aiLDR_FGrprUpDn   , ccBwd);
                  LDR_F.MoveActr(aiLDR_FGrprXFwBw  , ccBwd);
                  LDR_F.MoveActr(aiLDR_FGrprFwBw   , ccBwd);
                  LDR_R.MoveActr(aiLDR_RPickUpFwBw , ccBwd);
                  LDR_R.MoveActr(aiLDR_RGrprUpDn   , ccBwd);
                  LDR_R.MoveActr(aiLDR_RGrprXFwBw  , ccBwd);
                  LDR_R.MoveActr(aiLDR_RGrprFwBw   , ccBwd);
                  m_iManStep++;
                  return false ;

        case  15: if(!LDR_F.MoveActr(aiLDR_FPickUpFwBw , ccBwd)) return false ;
                  if(!LDR_F.MoveActr(aiLDR_FGrprUpDn   , ccBwd)) return false ;
                  if(!LDR_F.MoveActr(aiLDR_FGrprXFwBw  , ccBwd)) return false ;
                  if(!LDR_F.MoveActr(aiLDR_FGrprFwBw   , ccBwd)) return false ;
                  if(!LDR_R.MoveActr(aiLDR_RPickUpFwBw , ccBwd)) return false ;
                  if(!LDR_R.MoveActr(aiLDR_RGrprUpDn   , ccBwd)) return false ;
                  if(!LDR_R.MoveActr(aiLDR_RGrprXFwBw  , ccBwd)) return false ;
                  if(!LDR_R.MoveActr(aiLDR_RGrprFwBw   , ccBwd)) return false ;
                  m_iManStep++;
                  return false ;

        case  16: LDR_F.MoveActr(aiLDR_FLoadingFwBw, ccFwd);
                  LDR_R.MoveActr(aiLDR_RLoadingFwBw, ccFwd);
                  m_iManStep++;
                  return false;

        case  17: if(!LDR_F.MoveActr(aiLDR_FLoadingFwBw, ccFwd)) return false ;
                  if(!LDR_R.MoveActr(aiLDR_RLoadingFwBw, ccFwd)) return false ;
                  m_iManStep = 0;
                  return true ;
    }
}
