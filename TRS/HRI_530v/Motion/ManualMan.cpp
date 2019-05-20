//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ManualMan.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "LoaderSupply.h"
#include "Loader.h"
#include "LoaderStock.h"

#include "Stage.h"
#include "StageOST.h"
#include "StageVision.h"
#include "StageVT.h"

#include "UnLoaderSupply.h"
#include "UnLoader.h"
#include "UnLoaderStock.h"

#include "Sort.h"
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

    if(!IO_GetX(xETC_MainPower)                        ) {FM_MsgOk("ERR","Power On Plz");      return false ;} //test
    if(!IO_GetX(xETC_MainAir) || !IO_GetY(yETC_MainAir)) {FM_MsgOk("ERROR","Check Main Air!"); return false ;} //test

    EM_SetDisp(true);

    m_bManSetting = true ; //SetManCycle함수는 화면 쓰레드. 업데이트 함수에서 다른쓰레드로 들어와서 갱신하기에 플레그 걸어 막아둠.
    m_iManNo = _iNo ;

    bool bRet = true ;

    /********************/
    /* 프리프로세서     */
    /********************/
         if(m_iManNo == mcNoneCycle          ){FM_MsgOk("ERROR","None Cycle이 선택되었습니다."    ); bRet = false ;}
    else if(m_iManNo == mcAllHome            ){}
    else if(m_iManNo == mcLSP_Home           ){}
    else if(m_iManNo == mcLSP_CycleSupply    ){if(IO_GetX(xLSP_Tray4)){FM_MsgOk("ERROR","트레이안착부 센서가 감지중입니다." ); bRet = false ;}}
    else if(m_iManNo == mcLSP_GuidFwBw       ){}
    else if(m_iManNo == mcLDR_Home           ){}
    else if(m_iManNo == mcLDR_CycleIn        ){}
    else if(m_iManNo == mcLDR_CycleWork      ){}
    else if(m_iManNo == mcLDR_CycleOut       ){}
    else if(m_iManNo == mcLDR_AirBlow        ){}
    else if(m_iManNo == mcLDR_IndxUpDn       ){}
    else if(m_iManNo == mcLST_Home           ){}
    else if(m_iManNo == mcLST_CycleStock     ){}
    else if(m_iManNo == mcLST_StckUpDn       ){}
    else if(m_iManNo == mcSRT_Home           ){}
    else if(m_iManNo == mcSRT_CyclePickLDR   ){}
    else if(m_iManNo == mcSRT_CyclePlceLDR   ){}
    else if(m_iManNo == mcSRT_CyclePickSTG   ){}
    else if(m_iManNo == mcSRT_CyclePlceSTG   ){}
    else if(m_iManNo == mcSRT_CyclePickRJT1  ){}
    else if(m_iManNo == mcSRT_CyclePlceRJT1  ){}
    else if(m_iManNo == mcSRT_CyclePickRJT4  ){}
    else if(m_iManNo == mcSRT_CyclePlceRJT4  ){}
    else if(m_iManNo == mcSRT_CyclePickULD   ){}
    else if(m_iManNo == mcSRT_CyclePlceULD   ){}
    else if(m_iManNo == mcSRT_CyclePickZIG   ){}
    else if(m_iManNo == mcSRT_CyclePlceZIG   ){}
    else if(m_iManNo == mcSRT_CycleMoveLDR   ){}
    else if(m_iManNo == mcSRT_CycleMoveSTG   ){}
    else if(m_iManNo == mcSRT_CycleMoveRJT1  ){}
    else if(m_iManNo == mcSRT_CycleMoveRJT4  ){}
    else if(m_iManNo == mcSRT_CycleMoveULD   ){}
    else if(m_iManNo == mcSRT_CycleMoveZIG   ){}
    else if(m_iManNo == mcSTG_Home           ){}
    else if(m_iManNo == mcSTG_CycleWork      ){}
    else if(m_iManNo == mcSTG_Turn90         ){}
    else if(m_iManNo == mcSTG_Turn90N        ){}
    else if(m_iManNo == mcSTG_Turn45         ){}
    else if(m_iManNo == mcSTG_Turn45N        ){}
    else if(m_iManNo == mcOST_Home           ){}
    else if(m_iManNo == mcOST_CycleWork      ){}
    else if(m_iManNo == mcOST_CycleTest      ){}
    else if(m_iManNo == mcOST_PshrDnUp       ){}
    else if(m_iManNo == mcOST_PTolUpDn       ){}
    else if(m_iManNo == mcVTI_Home           ){}
    else if(m_iManNo == mcVTI_CycleWork      ){}
    else if(m_iManNo == mcVTI_CycleTest      ){}
    else if(m_iManNo == mcVTI_CycleBind      ){}
    else if(m_iManNo == mcVTI_CycleUnBind    ){}
    else if(m_iManNo == mcVTI_PshrDnUp       ){}
    else if(m_iManNo == mcVTI_PTolUpDn       ){}
    else if(m_iManNo == mcVTI_MainPwr        ){}
    else if(m_iManNo == mcVSN_Home           ){}
    else if(m_iManNo == mcVSN_CycleWork      ){}
    else if(m_iManNo == mcVSN_MoveP          ){}
    else if(m_iManNo == mcVSN_MoveN          ){}
    else if(m_iManNo == mcUSP_Home           ){}
    else if(m_iManNo == mcUSP_CycleSupply    ){if(IO_GetX(xLSP_Tray4)){FM_MsgOk("ERROR","트레이안착부 센서가 감지중입니다." ); bRet = false ;}}
    else if(m_iManNo == mcUSP_GuidFwBw       ){}
    else if(m_iManNo == mcULD_Home           ){}
    else if(m_iManNo == mcULD_CycleIn        ){}
    else if(m_iManNo == mcULD_CycleWork      ){}
    else if(m_iManNo == mcULD_CycleOut       ){}
    else if(m_iManNo == mcULD_IndxUpDn       ){}
    else if(m_iManNo == mcUST_Home           ){}
    else if(m_iManNo == mcUST_CycleStock     ){}
    else if(m_iManNo == mcUST_StckUpDn       ){}


    if(!bRet) Init();


    /********************/
    /* 처리..           */
    /********************/

         if(m_iManNo == mcNoneCycle          ){FM_MsgOk("ERROR","None Cycle이 선택되었습니다."    ); bRet = false ;}
    else if(m_iManNo == mcAllHome            ){MT_SetServoAll(true);
                                               LDR.InitHomeStep ();
                                               LSP.InitHomeStep ();
                                               LST.InitHomeStep ();
                                               SRT.InitHomeStep ();
                                               STG.InitHomeStep ();
                                               OST.InitHomeStep ();
                                               VTI.InitHomeStep ();
                                               VSN.InitHomeStep ();
                                               ULD.InitHomeStep ();
                                               UST.InitHomeStep ();
                                               USP.InitHomeStep ();                                        }
    else if(m_iManNo == mcLSP_Home           ){LSP.InitHomeStep ();                                        }
    else if(m_iManNo == mcLSP_CycleSupply    ){LSP.InitCycleStep();                                        }
    else if(m_iManNo == mcLSP_GuidFwBw       ){LSP.MoveActr(aiLSP_GuidFwBw , !AT_GetCmd(aiLSP_GuidFwBw )); }
    else if(m_iManNo == mcLDR_Home           ){LDR.InitHomeStep ();                                        }
    else if(m_iManNo == mcLDR_CycleIn        ){LDR.InitCycleStep();                                        }
    else if(m_iManNo == mcLDR_CycleWork      ){LDR.InitCycleStep();                                        }
    else if(m_iManNo == mcLDR_CycleOut       ){LDR.InitCycleStep();                                        }
    else if(m_iManNo == mcLDR_AirBlow        ){IO_SetY     (yLDR_AirBlow   , !IO_GetY  (yLDR_AirBlow   )); }
    else if(m_iManNo == mcLDR_IndxUpDn       ){LDR.MoveActr(aiLDR_IndxUpDn , !AT_GetCmd(aiLDR_IndxUpDn )); }
    else if(m_iManNo == mcLST_Home           ){LST.InitHomeStep ();                                        }
    else if(m_iManNo == mcLST_CycleStock     ){LST.InitCycleStep();                                        }
    else if(m_iManNo == mcLST_StckUpDn       ){LST.MoveActr(aiLST_StckUpDn , !AT_GetCmd(aiLST_StckUpDn )); }
    else if(m_iManNo == mcSRT_Home           ){SRT.InitHomeStep ();                                        }
    else if(m_iManNo == mcSRT_CyclePickLDR   ){m_iManStep = 10; m_iPreManStep = 0;                         }
    else if(m_iManNo == mcSRT_CyclePlceLDR   ){m_iManStep = 10; m_iPreManStep = 0;                         }
    else if(m_iManNo == mcSRT_CyclePickSTG   ){m_iManStep = 10; m_iPreManStep = 0;                         }
    else if(m_iManNo == mcSRT_CyclePlceSTG   ){m_iManStep = 10; m_iPreManStep = 0;                         }
    else if(m_iManNo == mcSRT_CyclePickRJT1  ){m_iManStep = 10; m_iPreManStep = 0;                         }
    else if(m_iManNo == mcSRT_CyclePlceRJT1  ){m_iManStep = 10; m_iPreManStep = 0;                         }
    else if(m_iManNo == mcSRT_CyclePickRJT4  ){m_iManStep = 10; m_iPreManStep = 0;                         }
    else if(m_iManNo == mcSRT_CyclePlceRJT4  ){m_iManStep = 10; m_iPreManStep = 0;                         }
    else if(m_iManNo == mcSRT_CyclePickULD   ){m_iManStep = 10; m_iPreManStep = 0;                         }
    else if(m_iManNo == mcSRT_CyclePlceULD   ){m_iManStep = 10; m_iPreManStep = 0;                         }
    else if(m_iManNo == mcSRT_CyclePickZIG   ){m_iManStep = 10; m_iPreManStep = 0;                         }
    else if(m_iManNo == mcSRT_CyclePlceZIG   ){m_iManStep = 10; m_iPreManStep = 0;                         }
    else if(m_iManNo == mcSRT_CycleMoveLDR   ){m_iManStep = 10; m_iPreManStep = 0;                         }       
    else if(m_iManNo == mcSRT_CycleMoveSTG   ){m_iManStep = 10; m_iPreManStep = 0;                         }       
    else if(m_iManNo == mcSRT_CycleMoveRJT1  ){m_iManStep = 10; m_iPreManStep = 0;                         }       
    else if(m_iManNo == mcSRT_CycleMoveRJT4  ){m_iManStep = 10; m_iPreManStep = 0;                         }       
    else if(m_iManNo == mcSRT_CycleMoveULD   ){m_iManStep = 10; m_iPreManStep = 0;                         }       
    else if(m_iManNo == mcSRT_CycleMoveZIG   ){m_iManStep = 10; m_iPreManStep = 0;                         }       
    else if(m_iManNo == mcSTG_Home           ){STG.InitHomeStep ();                                        }
    else if(m_iManNo == mcSTG_CycleWork      ){STG.InitCycleStep();                                        }
    else if(m_iManNo == mcSTG_Turn90         ){m_iManStep = 10; m_iPreManStep = 0;                         }
    else if(m_iManNo == mcSTG_Turn90N        ){m_iManStep = 10; m_iPreManStep = 0;                         }
    else if(m_iManNo == mcSTG_Turn45         ){m_iManStep = 10; m_iPreManStep = 0;                         }
    else if(m_iManNo == mcSTG_Turn45N        ){m_iManStep = 10; m_iPreManStep = 0;                         }
    else if(m_iManNo == mcOST_Home           ){OST.InitHomeStep ();                                        }
    else if(m_iManNo == mcOST_CycleWork      ){OST.InitCycleStep();                                        }
    else if(m_iManNo == mcOST_CycleTest      ){OST.InitCycleStep();                                        }
    else if(m_iManNo == mcOST_PshrDnUp       ){OST.MoveActr(aiOST_PshrDnUp , !AT_GetCmd(aiOST_PshrDnUp )); }
    else if(m_iManNo == mcOST_PTolUpDn       ){OST.MoveActr(aiOST_PTolUpDn , !AT_GetCmd(aiOST_PTolUpDn )); }
    else if(m_iManNo == mcVTI_Home           ){VTI.InitHomeStep ();                                        }
    else if(m_iManNo == mcVTI_CycleWork      ){VTI.InitCycleStep();                                        }
    else if(m_iManNo == mcVTI_CycleTest      ){VTI.InitCycleStep();                                        }
    else if(m_iManNo == mcVTI_CycleBind      ){VTI.InitCycleStep();                                        }
    else if(m_iManNo == mcVTI_CycleUnBind    ){VTI.InitCycleStep();                                        }
    else if(m_iManNo == mcVTI_PshrDnUp       ){VTI.MoveActr(aiVTI_PshrDnUp , !AT_GetCmd(aiVTI_PshrDnUp )); }
    else if(m_iManNo == mcVTI_PTolUpDn       ){VTI.MoveActr(aiVTI_PTolUpDn , !AT_GetCmd(aiVTI_PTolUpDn )); }
    else if(m_iManNo == mcVTI_MainPwr        ){IO_SetY(yVTI_VTPower , !IO_GetY(yVTI_VTPower));             }
    else if(m_iManNo == mcVSN_Home           ){VSN.InitHomeStep ();                                        }
    else if(m_iManNo == mcVSN_CycleWork      ){VSN.InitCycleStep();                                        }
    else if(m_iManNo == mcVSN_MoveP          ){MT_GoIncMan(miSTG_YVisn , OM.DevInfo.dScktPitch);           }
    else if(m_iManNo == mcVSN_MoveN          ){MT_GoIncMan(miSTG_YVisn ,-OM.DevInfo.dScktPitch);           }
    else if(m_iManNo == mcVSN_MoveTrg1       ){VSN.MoveMotr(miSTG_YVisn , piSTG_YVisnTrgStt1);             }
    else if(m_iManNo == mcVSN_MoveTrg2       ){VSN.MoveMotr(miSTG_YVisn , piSTG_YVisnTrgStt2);             }
    else if(m_iManNo == mcUSP_Home           ){USP.InitHomeStep ();                                        }
    else if(m_iManNo == mcUSP_CycleSupply    ){USP.InitCycleStep();                                        }
    else if(m_iManNo == mcUSP_GuidFwBw       ){USP.MoveActr(aiUSP_GuidFwBw , !AT_GetCmd(aiUSP_GuidFwBw )); }
    else if(m_iManNo == mcULD_Home           ){ULD.InitHomeStep ();                                        }
    else if(m_iManNo == mcULD_CycleIn        ){ULD.InitCycleStep();                                        }
    else if(m_iManNo == mcULD_CycleWork      ){ULD.InitCycleStep();                                        }
    else if(m_iManNo == mcULD_CycleOut       ){ULD.InitCycleStep();                                        }
    else if(m_iManNo == mcULD_IndxUpDn       ){ULD.MoveActr(aiULD_IndxUpDn , !AT_GetCmd(aiULD_IndxUpDn )); }
    else if(m_iManNo == mcUST_Home           ){UST.InitHomeStep ();                                        }
    else if(m_iManNo == mcUST_CycleStock     ){UST.InitCycleStep();                                        }
    else if(m_iManNo == mcUST_StckUpDn       ){UST.MoveActr(aiUST_StckUpDn , !AT_GetCmd(aiUST_StckUpDn )); }
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

      if(!OM.CmnOptn.bIgnrDoor    ) SEQ.InspectDoor    ();    //test
      if(m_iManNo != mcLSP_Home &&                            //test
         m_iManNo != mcLDR_Home &&                            //test
         m_iManNo != mcLST_Home &&                            //test
         m_iManNo != mcSRT_Home &&                            //test
         m_iManNo != mcSTG_Home &&                            //test
         m_iManNo != mcOST_Home &&                            //test
         m_iManNo != mcVTI_Home &&                            //test
         m_iManNo != mcVSN_Home &&                            //test
         m_iManNo != mcUSP_Home &&                            //test
         m_iManNo != mcULD_Home &&                            //test
         m_iManNo != mcUST_Home &&                            //test
         m_iManNo != mcAllHome    ) SEQ.InspectHomeEnd ();    //test
                                                              //test
      SEQ.InspectLightGrid();                                 //test

      bool r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11 ;
      r1=r2=r3=r4=r5=r6=r7=r8=r9=r10=r11 = false ;

      //Check Alarm.
      if ( EM_IsErr()      ) { Init(); return ; }             //test
    //Cycle Step.
    if      (m_iManNo == mcAllHome      ) { r4  = SRT.CycleHome();
                                            r6  = OST.CycleHome();
                                            r7  = VTI.CycleHome();


                                            if(r6&&r7&&MT_GetHomeEnd(miSRT_ZPckr)&&MT_GetCmdPos(miSRT_ZPckr)==PM.GetValue(miSRT_ZPckr,pvSRT_ZPckrWait)){
                                                r1  = LDR.CycleHome();
                                                r2  = LSP.CycleHome();
                                                r3  = LST.CycleHome();

                                                r5  = STG.CycleHome();
                                                r8  = VSN.CycleHome();
                                                r9  = ULD.CycleHome();
                                                r10 = UST.CycleHome();
                                                r11 = USP.CycleHome();
                                            }

                                            if(r1&&r2&&r3&&r4&&r5&&r6&&r7&&r8&&r9&&r10&&r11) {
                                                //     MT_SetHomeEnd(miETC_Spare , true);
                                                m_iManNo = mcNoneCycle;
                                                FM_MsgOk("Confirm","All Home Finished!");
                                            }
                                          }





    else if(m_iManNo == mcLSP_Home           ){if(LSP.CycleHome    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLSP_CycleSupply    ){if(LSP.CycleSupply  ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLSP_GuidFwBw       ){                        m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_Home           ){if(LDR.CycleHome    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_CycleIn        ){if(LDR.CycleIn      ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_CycleWork      ){if(LDR.CycleWork    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_CycleOut       ){if(LDR.CycleOut     ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_AirBlow        ){                        m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_IndxUpDn       ){                        m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLST_Home           ){if(LST.CycleHome    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLST_CycleStock     ){if(LST.CycleStock   ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLST_StckUpDn       ){                        m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_Home           ){if(SRT.CycleHome    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_CyclePickLDR   ){if(CyclePick   (spLDR)) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_CyclePlceLDR   ){if(CyclePlace  (spLDR)) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_CyclePickSTG   ){if(CyclePick   (spSTG)) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_CyclePlceSTG   ){if(CyclePlace  (spSTG)) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_CyclePickRJT1  ){if(CyclePick   (spRJ1)) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_CyclePlceRJT1  ){if(CyclePlace  (spRJ1)) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_CyclePickRJT4  ){if(CyclePick   (spRJ4)) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_CyclePlceRJT4  ){if(CyclePlace  (spRJ4)) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_CyclePickULD   ){if(CyclePick   (spULD)) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_CyclePlceULD   ){if(CyclePlace  (spULD)) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_CyclePickZIG   ){if(CyclePick   (spZIG)) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_CyclePlceZIG   ){if(CyclePlace  (spZIG)) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_CycleMoveLDR   ){if(CycleMove   (spLDR)) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_CycleMoveSTG   ){if(CycleMove   (spSTG)) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_CycleMoveRJT1  ){if(CycleMove   (spRJ1)) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_CycleMoveRJT4  ){if(CycleMove   (spRJ4)) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_CycleMoveULD   ){if(CycleMove   (spULD)) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSRT_CycleMoveZIG   ){if(CycleMove   (spZIG)) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTG_Home           ){if(STG.CycleHome    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTG_CycleWork      ){if(STG.CycleWork    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTG_Turn90         ){if(CycleTurn   (90   )) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTG_Turn90N        ){if(CycleTurn   (-90  )) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTG_Turn45         ){if(CycleTurn   (45   )) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTG_Turn45N        ){if(CycleTurn   (-45  )) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcOST_Home           ){if(OST.CycleHome    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcOST_CycleWork      ){if(OST.CycleWork    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcOST_CycleTest      ){if(OST.CycleTest    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcOST_PshrDnUp       ){                        m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcOST_PTolUpDn       ){                        m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcVTI_Home           ){if(VTI.CycleHome    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcVTI_CycleWork      ){if(VTI.CycleWork    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcVTI_CycleBind      ){if(VTI.CycleBind    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcVTI_CycleUnBind    ){if(VTI.CycleUnBind  ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcVTI_PshrDnUp       ){                        m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcVTI_PTolUpDn       ){                        m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcVTI_MainPwr        ){                        m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcVSN_Home           ){if(VSN.CycleHome    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcVSN_CycleWork      ){if(VSN.CycleWork    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcVSN_MoveP          ){                        m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcVSN_MoveN          ){                        m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcVSN_MoveTrg1       ){                        m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcVSN_MoveTrg2       ){                        m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcUSP_Home           ){if(USP.CycleHome    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcUSP_CycleSupply    ){if(USP.CycleSupply  ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcUSP_GuidFwBw       ){                        m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_Home           ){if(ULD.CycleHome    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_CycleIn        ){if(ULD.CycleIn      ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_CycleWork      ){if(ULD.CycleWork    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_CycleOut       ){if(ULD.CycleOut     ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_IndxUpDn       ){                        m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcUST_Home           ){if(UST.CycleHome    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcUST_CycleStock     ){if(UST.CycleStock   ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcUST_StckUpDn       ){                        m_iManNo = mcNoneCycle; }
    else                                      {                        m_iManNo = mcNoneCycle; } //여기서 갱신됌.

    //Ok.
    return ;

}

//---------------------------------------------------------------------------
bool CManualMan::CycleTurn(double _dInc)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    int iTimeOutTime = 5000 ;
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

        case  10: OST.MoveActr(aiOST_PshrDnUp , ccBwd);
                  OST.MoveActr(aiOST_PTolUpDn , ccBwd);
                  VTI.MoveActr(aiVTI_PshrDnUp , ccBwd);
                  VTI.MoveActr(aiVTI_PTolUpDn , ccBwd);
                  m_iManStep++;
                  return false ;

        case  11: if(!OST.MoveActr(aiOST_PshrDnUp , ccBwd)) return false ;
                  if(!OST.MoveActr(aiOST_PTolUpDn , ccBwd)) return false ;
                  if(!VTI.MoveActr(aiVTI_PshrDnUp , ccBwd)) return false ;
                  if(!VTI.MoveActr(aiVTI_PTolUpDn , ccBwd)) return false ;
                  SRT.MoveMotr(miSRT_ZPckr , piSRT_ZPckrWait);
                  m_iManStep++;
                  return false ;

        case  12: if(!SRT.MoveMotr(miSRT_ZPckr , piSRT_ZPckrWait)) return false;
                  MT_GoIncMan(miSTG_TTble , _dInc);
                  m_iManStep=0;
                  return true;
    }
}

bool CManualMan::CyclePick(EN_SORT_POS_ID _iPosId)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    int iTimeOutTime = 15000 ;
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


    EN_PSTN_ID piSRT_XPckr ;
    EN_PSTN_ID piSRT_YPckr ;
    EN_PSTN_ID piSRT_XExpd ;
    EN_PSTN_ID piSRT_ZPckr ;

    //if(EN_SORT_POS_ID == spNone){
    //    piSRT_XPckr = piSRT_XPckrPlaceStt   ; piSRT_YPckr = piSRT_YPckrPlace      ; piSRT_XExpd = piSRT_XExpdULDTray ; piSRT_ZPckr = piSRT_ZPckrWait    ; }
    //}
         if(_iPosId == spLDR ){piSRT_XPckr = piSRT_XPckrPickStt    ; piSRT_YPckr = piSRT_YPckr           ; piSRT_XExpd = piSRT_XExpdLDRTray ; piSRT_ZPckr = piSRT_ZPckrLDRPick ;}
    else if(_iPosId == spZIG ){piSRT_XPckr = piSRT_XPckrZig        ; piSRT_YPckr = piSRT_YPckrZig        ; piSRT_XExpd = piSRT_XExpdZig     ; piSRT_ZPckr = piSRT_ZPckrZIGPick ;}
    else if(_iPosId == spSTG ){piSRT_XPckr = piSRT_XPckrPocket     ; piSRT_YPckr = piSRT_YPckrPocket     ; piSRT_XExpd = piSRT_XExpdPocket  ; piSRT_ZPckr = piSRT_ZPckrSTGPick ;}
    else if(_iPosId == spRJ1 ){piSRT_XPckr = piSRT_XPckrReject1Stt ; piSRT_YPckr = piSRT_YPckrReject1Stt ; piSRT_XExpd = piSRT_XExpdReject  ; piSRT_ZPckr = piSRT_ZPckrRJTPick ;}
    else if(_iPosId == spRJ2 ){piSRT_XPckr = piSRT_XPckrReject2Stt ; piSRT_YPckr = piSRT_YPckrReject2Stt ; piSRT_XExpd = piSRT_XExpdReject  ; piSRT_ZPckr = piSRT_ZPckrRJTPick ;}
    else if(_iPosId == spRJ3 ){piSRT_XPckr = piSRT_XPckrReject3Stt ; piSRT_YPckr = piSRT_YPckrReject3Stt ; piSRT_XExpd = piSRT_XExpdReject  ; piSRT_ZPckr = piSRT_ZPckrRJTPick ;}
    else if(_iPosId == spRJ4 ){piSRT_XPckr = piSRT_XPckrReject4Stt ; piSRT_YPckr = piSRT_YPckrReject4Stt ; piSRT_XExpd = piSRT_XExpdReject  ; piSRT_ZPckr = piSRT_ZPckrRJTPick ;}
    else if(_iPosId == spULD ){piSRT_XPckr = piSRT_XPckrPlaceStt   ; piSRT_YPckr = piSRT_YPckrPlace      ; piSRT_XExpd = piSRT_XExpdULDTray ; piSRT_ZPckr = piSRT_ZPckrULDPick ;}
    else                      {piSRT_XPckr = piSRT_XPckrPlaceStt   ; piSRT_YPckr = piSRT_YPckrPlace      ; piSRT_XExpd = piSRT_XExpdULDTray ; piSRT_ZPckr = piSRT_ZPckrWait    ;}

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: SRT.MoveMotr(miSRT_ZPckr , piSRT_ZPckrWait);
                  SRT.MoveActr(aiSRT_FrstDnUp , ccFwd);
                  SRT.MoveActr(aiSRT_ScndDnUp , ccFwd);
                  SRT.MoveActr(aiSRT_ThrdDnUp , ccFwd);
                  SRT.MoveActr(aiSRT_FrthDnUp , ccFwd);
                  m_iManStep++;
                  return false ;

        case  11: if(!SRT.MoveMotr(miSRT_ZPckr , piSRT_ZPckrWait)) return false ;
                  if(!SRT.MoveActr(aiSRT_FrstDnUp , ccFwd)) return false ;
                  if(!SRT.MoveActr(aiSRT_ScndDnUp , ccFwd)) return false ;
                  if(!SRT.MoveActr(aiSRT_ThrdDnUp , ccFwd)) return false ;
                  if(!SRT.MoveActr(aiSRT_FrthDnUp , ccFwd)) return false ;
                  SRT.MoveMotr(miSRT_XPckr , piSRT_XPckr);
                  SRT.MoveMotr(miSRT_YPckr , piSRT_YPckr);
                  SRT.MoveMotr(miSRT_XExpd , piSRT_XExpd);
                  IO_SetY(ySRT_Vcc , true);
                  m_iManStep++;
                  return false ;

        case  12: if(!SRT.MoveMotr(miSRT_XPckr , piSRT_XPckr)) return false ;
                  if(!SRT.MoveMotr(miSRT_YPckr , piSRT_YPckr)) return false ;
                  if(!SRT.MoveMotr(miSRT_XExpd , piSRT_XExpd)) return false ;
                  SRT.MoveMotr(miSRT_ZPckr , piSRT_ZPckr);
                  m_iManStep++;
                  return false;


        case  13: if(!SRT.MoveMotr(miSRT_ZPckr , piSRT_ZPckr)) return false ;
                  IO_SetY(ySRT_Ejct1 , false);
                  IO_SetY(ySRT_Ejct2 , false);
                  IO_SetY(ySRT_Ejct3 , false);
                  IO_SetY(ySRT_Ejct4 , false);

                  IO_SetY(ySRT_Vcc1 , true);
                  IO_SetY(ySRT_Vcc2 , true);
                  IO_SetY(ySRT_Vcc3 , true);
                  IO_SetY(ySRT_Vcc4 , true);

                  m_tmDelay.Clear();
                  m_iManStep++;
                  return false;

        case  14: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iPickDelay)) return false ;

                  SRT.MoveMotr(miSRT_ZPckr , piSRT_ZPckrWait);
                  m_iManStep++;
                  return false;

        case  15: if(!SRT.MoveMotr(miSRT_ZPckr , piSRT_ZPckrWait)) return false ;
                  m_iManStep = 0;
                  return true ;

    }
}


bool CManualMan::CyclePlace(EN_SORT_POS_ID _iPosId)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    int iTimeOutTime = 15000 ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, iTimeOutTime )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());

        IO_SetY(ySRT_Ejct1 , false);
        IO_SetY(ySRT_Ejct2 , false);
        IO_SetY(ySRT_Ejct3 , false);
        IO_SetY(ySRT_Ejct4 , false);

        m_iManStep = 0 ;
        return true;
    }

    if(m_iManStep != m_iPreManStep) {
        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
    }

    m_iPreManStep = m_iManStep ;

    EN_PSTN_ID piSRT_XPckr ;
    EN_PSTN_ID piSRT_YPckr ;
    EN_PSTN_ID piSRT_XExpd ;
    EN_PSTN_ID piSRT_ZPckr ;

         if(_iPosId == spLDR){piSRT_XPckr = piSRT_XPckrPickStt    ; piSRT_YPckr = piSRT_YPckr           ; piSRT_XExpd = piSRT_XExpdLDRTray ; piSRT_ZPckr = piSRT_ZPckrLDRPlace ; }
    else if(_iPosId == spZIG){piSRT_XPckr = piSRT_XPckrZig        ; piSRT_YPckr = piSRT_YPckrZig        ; piSRT_XExpd = piSRT_XExpdZig     ; piSRT_ZPckr = piSRT_ZPckrZIGPlace ; }
    else if(_iPosId == spSTG){piSRT_XPckr = piSRT_XPckrPocket     ; piSRT_YPckr = piSRT_YPckrPocket     ; piSRT_XExpd = piSRT_XExpdPocket  ; piSRT_ZPckr = piSRT_ZPckrSTGPlace ; }
    else if(_iPosId == spRJ1){piSRT_XPckr = piSRT_XPckrReject1Stt ; piSRT_YPckr = piSRT_YPckrReject1Stt ; piSRT_XExpd = piSRT_XExpdReject  ; piSRT_ZPckr = piSRT_ZPckrRJTPlace ; }
    else if(_iPosId == spRJ2){piSRT_XPckr = piSRT_XPckrReject2Stt ; piSRT_YPckr = piSRT_YPckrReject2Stt ; piSRT_XExpd = piSRT_XExpdReject  ; piSRT_ZPckr = piSRT_ZPckrRJTPlace ; }
    else if(_iPosId == spRJ3){piSRT_XPckr = piSRT_XPckrReject3Stt ; piSRT_YPckr = piSRT_YPckrReject3Stt ; piSRT_XExpd = piSRT_XExpdReject  ; piSRT_ZPckr = piSRT_ZPckrRJTPlace ; }
    else if(_iPosId == spRJ4){piSRT_XPckr = piSRT_XPckrReject4Stt ; piSRT_YPckr = piSRT_YPckrReject4Stt ; piSRT_XExpd = piSRT_XExpdReject  ; piSRT_ZPckr = piSRT_ZPckrRJTPlace ; }
    else if(_iPosId == spULD){piSRT_XPckr = piSRT_XPckrPlaceStt   ; piSRT_YPckr = piSRT_YPckrPlace      ; piSRT_XExpd = piSRT_XExpdULDTray ; piSRT_ZPckr = piSRT_ZPckrULDPlace ; }
    else                     {piSRT_XPckr = piSRT_XPckrPlaceStt   ; piSRT_YPckr = piSRT_YPckrPlace      ; piSRT_XExpd = piSRT_XExpdULDTray ; piSRT_ZPckr = piSRT_ZPckrWait     ; }

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: SRT.MoveMotr(miSRT_ZPckr , piSRT_ZPckrWait);
                  SRT.MoveActr(aiSRT_FrstDnUp , ccFwd);
                  SRT.MoveActr(aiSRT_ScndDnUp , ccFwd);
                  SRT.MoveActr(aiSRT_ThrdDnUp , ccFwd);
                  SRT.MoveActr(aiSRT_FrthDnUp , ccFwd);
                  m_iManStep++;
                  return false;

        case  11: if(!SRT.MoveMotr(miSRT_ZPckr , piSRT_ZPckrWait)) return false;
                  if(!SRT.MoveActr(aiSRT_FrstDnUp , ccFwd)) return false;
                  if(!SRT.MoveActr(aiSRT_ScndDnUp , ccFwd)) return false;
                  if(!SRT.MoveActr(aiSRT_ThrdDnUp , ccFwd)) return false;
                  if(!SRT.MoveActr(aiSRT_FrthDnUp , ccFwd)) return false;
                  SRT.MoveMotr(miSRT_XPckr , piSRT_XPckr);
                  SRT.MoveMotr(miSRT_YPckr , piSRT_YPckr);
                  SRT.MoveMotr(miSRT_XExpd , piSRT_XExpd);
                  m_iManStep++;
                  return false;

        case  12: if(!SRT.MoveMotr(miSRT_XPckr , piSRT_XPckr)) return false ;
                  if(!SRT.MoveMotr(miSRT_YPckr , piSRT_YPckr)) return false ;
                  if(!SRT.MoveMotr(miSRT_XExpd , piSRT_XExpd)) return false ;
                  SRT.MoveMotr(miSRT_ZPckr , piSRT_ZPckr);
                  IO_SetY(ySRT_Vcc , true);
                  m_iManStep++;
                  return false ;

        case  13: if(!SRT.MoveMotr(miSRT_ZPckr , piSRT_ZPckr)) return false ;
                  IO_SetY(ySRT_Vcc1 , false);
                  IO_SetY(ySRT_Vcc2 , false);
                  IO_SetY(ySRT_Vcc3 , false);
                  IO_SetY(ySRT_Vcc4 , false);

                  IO_SetY(ySRT_Ejct1 , true);
                  IO_SetY(ySRT_Ejct2 , true);
                  IO_SetY(ySRT_Ejct3 , true);
                  IO_SetY(ySRT_Ejct4 , true);
                  m_tmDelay.Clear();
                  m_iManStep++;
                  return false;

        case  14: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iPlceDelay)) return false;




                  SRT.MoveMotr(miSRT_ZPckr , piSRT_ZPckrWait);
                  m_iManStep++;
                  return false;

        case  15: if(!SRT.MoveMotr(miSRT_ZPckr , piSRT_ZPckrWait)) return false;
                  IO_SetY(ySRT_Ejct1 , false);
                  IO_SetY(ySRT_Ejct2 , false);
                  IO_SetY(ySRT_Ejct3 , false);
                  IO_SetY(ySRT_Ejct4 , false);

                  m_iManStep = 0;
                  return true;
    }

}

bool CManualMan::CycleMove(EN_SORT_POS_ID _iPosId)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    int iTimeOutTime = 15000 ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, iTimeOutTime )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());

        IO_SetY(ySRT_Ejct1 , false);
        IO_SetY(ySRT_Ejct2 , false);
        IO_SetY(ySRT_Ejct3 , false);
        IO_SetY(ySRT_Ejct4 , false);

        m_iManStep = 0 ;
        return true;
    }

    if(m_iManStep != m_iPreManStep) {
        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
    }

    m_iPreManStep = m_iManStep ;

    EN_PSTN_ID piSRT_XPckr ;
    EN_PSTN_ID piSRT_YPckr ;
    EN_PSTN_ID piSRT_XExpd ;
    EN_PSTN_ID piSRT_ZPckr ;

         if(_iPosId == spLDR){piSRT_XPckr = piSRT_XPckrPickStt    ; piSRT_YPckr = piSRT_YPckr           ; piSRT_XExpd = piSRT_XExpdLDRTray ; piSRT_ZPckr = piSRT_ZPckrWait ; }
    else if(_iPosId == spZIG){piSRT_XPckr = piSRT_XPckrZig        ; piSRT_YPckr = piSRT_YPckrZig        ; piSRT_XExpd = piSRT_XExpdZig     ; piSRT_ZPckr = piSRT_ZPckrWait ; }
    else if(_iPosId == spSTG){piSRT_XPckr = piSRT_XPckrPocket     ; piSRT_YPckr = piSRT_YPckrPocket     ; piSRT_XExpd = piSRT_XExpdPocket  ; piSRT_ZPckr = piSRT_ZPckrWait ; }
    else if(_iPosId == spRJ1){piSRT_XPckr = piSRT_XPckrReject1Stt ; piSRT_YPckr = piSRT_YPckrReject1Stt ; piSRT_XExpd = piSRT_XExpdReject  ; piSRT_ZPckr = piSRT_ZPckrWait ; }
    else if(_iPosId == spRJ2){piSRT_XPckr = piSRT_XPckrReject2Stt ; piSRT_YPckr = piSRT_YPckrReject2Stt ; piSRT_XExpd = piSRT_XExpdReject  ; piSRT_ZPckr = piSRT_ZPckrWait ; }
    else if(_iPosId == spRJ3){piSRT_XPckr = piSRT_XPckrReject3Stt ; piSRT_YPckr = piSRT_YPckrReject3Stt ; piSRT_XExpd = piSRT_XExpdReject  ; piSRT_ZPckr = piSRT_ZPckrWait ; }
    else if(_iPosId == spRJ4){piSRT_XPckr = piSRT_XPckrReject4Stt ; piSRT_YPckr = piSRT_YPckrReject4Stt ; piSRT_XExpd = piSRT_XExpdReject  ; piSRT_ZPckr = piSRT_ZPckrWait ; }
    else if(_iPosId == spULD){piSRT_XPckr = piSRT_XPckrPlaceStt   ; piSRT_YPckr = piSRT_YPckrPlace      ; piSRT_XExpd = piSRT_XExpdULDTray ; piSRT_ZPckr = piSRT_ZPckrWait ; }
    else                     {piSRT_XPckr = piSRT_XPckrPlaceStt   ; piSRT_YPckr = piSRT_YPckrPlace      ; piSRT_XExpd = piSRT_XExpdULDTray ; piSRT_ZPckr = piSRT_ZPckrWait ; }

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: SRT.MoveMotr(miSRT_ZPckr , piSRT_ZPckrWait);
                  SRT.MoveActr(aiSRT_FrstDnUp , ccFwd);
                  SRT.MoveActr(aiSRT_ScndDnUp , ccFwd);
                  SRT.MoveActr(aiSRT_ThrdDnUp , ccFwd);
                  SRT.MoveActr(aiSRT_FrthDnUp , ccFwd);
                  m_iManStep++;
                  return false;

        case  11: if(!SRT.MoveMotr(miSRT_ZPckr , piSRT_ZPckrWait)) return false;
                  if(!SRT.MoveActr(aiSRT_FrstDnUp , ccFwd)) return false;
                  if(!SRT.MoveActr(aiSRT_ScndDnUp , ccFwd)) return false;
                  if(!SRT.MoveActr(aiSRT_ThrdDnUp , ccFwd)) return false;
                  if(!SRT.MoveActr(aiSRT_FrthDnUp , ccFwd)) return false;
                  SRT.MoveMotr(miSRT_XPckr , piSRT_XPckr);
                  SRT.MoveMotr(miSRT_YPckr , piSRT_YPckr);
                  SRT.MoveMotr(miSRT_XExpd , piSRT_XExpd);
                  m_iManStep++;
                  return false;

        case  12: if(!SRT.MoveMotr(miSRT_XPckr , piSRT_XPckr)) return false ;
                  if(!SRT.MoveMotr(miSRT_YPckr , piSRT_YPckr)) return false ;
                  if(!SRT.MoveMotr(miSRT_XExpd , piSRT_XExpd)) return false ;
                  m_iManStep = 0;
                  return true;
    }
}






























































































