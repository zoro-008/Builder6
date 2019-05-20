//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ManualMan.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "LoaderFront.h"
#include "LoaderRear.h"
#include "Index.h"
#include "Stage.h"
#include "LeftTool.h"
#include "RightTool.h"
#include "Unloader.h"
//---------------------------------------------------------------------------
#include "Sequence.h"
#include "Rs232Keyence.h"

//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "HexaPot.h"
#include "CadPosition.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------
CManualMan MM;

//---------------------------------------------------------------------------
CManualMan::CManualMan(void)
{

}

//---------------------------------------------------------------------------
CManualMan::~CManualMan(void)
{

}

//---------------------------------------------------------------------------
void CManualMan::Init(void)
{
    //Init. Var.
    m_iManStep    = 0     ;
    m_iPreManStep = 0     ;
    m_iManNo      = mcNoneCycle ;

    m_bManSetting  = false ;

    m_tmCycle.Clear() ;
    m_tmDelay.Clear() ;
}

//---------------------------------------------------------------------------
void CManualMan::Close(void)
{
}

//---------------------------------------------------------------------------
bool CManualMan::SetManCycle(EN_MANUAL_CYCLE _iNo)
{
    if( _iNo     <  0                ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( _iNo     >= MAX_MANUAL_CYCLE ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( m_iManNo != mcNoneCycle      ) {FM_MsgOk("ERROR","Doing Manual Cycle"   ); return false ;}
    if(SEQ._bRun                     ) {FM_MsgOk("ERROR","Autorunning"          ); return false ;}

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return false ; } //아 밑에 처리 하는 애 때문에 잠시 이렇게 함.          //test

//    if(!IO_GetX(xETC_MainPower) ) {FM_MsgOk("ERR","Power On Plz");      return false ;} //test
    if(!IO_GetX(xETC_MainAirChk  ) ) {FM_MsgOk("ERROR","Check Main Air!"); return false ;} //test

    EM_SetDisp(true);

    m_bStdCalPick = false;                                                                                                            //   mcLDR_FHome
                                                                                                                                      //   mcLDR_RHome
    m_bManSetting = true ; //SetManCycle함수는 화면 쓰레드. 업데이트 함수에서 다른쓰레드로 들어와서 갱신하기에 플레그 걸어 막아둠.    //   mcIDX_Home
    m_iManNo = _iNo ;                                                                                                                 //   mcLTL_Home
                                                                                                                                      //   mcRTL_Home
    bool bRet = true ;                                                                                                                //   mcSTG_Home 
                                                                                                                                      //   mcULD_Home
    /********************/
    /* 프리프로세서     */
    /********************/
         if(m_iManNo == mcNoneCycle             ){bRet = false ;}
    else if(m_iManNo == mcAllHome               ){}
    else if(m_iManNo == mcLDR_FHome             ){}
    else if(m_iManNo == mcLDR_RHome             ){if(!MT_GetHomeEnd(miIDX_XIndxR)) {FM_MsgOk("ERROR","Rear Index 홈을 먼저 잡아주세요"); bRet = false;}
                                                  if( MT_GetCmdPos(miIDX_XIndxR)>PM.GetValue(miIDX_XIndxR,pvIDX_XIndxRWait)) {FM_MsgOk("ERROR","Rear Index Postion이 Wait포지션 보다 높습니다.");bRet = false; }}
    else if(m_iManNo == mcIDX_Home              ){}
    else if(m_iManNo == mcLTL_Home              ){}
    else if(m_iManNo == mcRTL_Home              ){}
    else if(m_iManNo == mcSTG_Home              ){}
    else if(m_iManNo == mcULD_Home              ){}

    else if(m_iManNo == mcLDR_FCyclePick        ){}
    else if(m_iManNo == mcLDR_FPitchUp          ){if(!LDR_F.CheckSafe(miLDR_ZElevF, piLDR_ZElevFPick)) bRet = false;}  //20160426 진섭
    else if(m_iManNo == mcLDR_FPitchDn          ){if(!LDR_F.CheckSafe(miLDR_ZElevF, piLDR_ZElevFPick)) bRet = false;}



    else if(m_iManNo == mcLDR_RCyclePick        ){}
    else if(m_iManNo == mcLDR_RCyclePlce        ){}
    else if(m_iManNo == mcLDR_RPitchUp          ){if(!LDR_R.CheckSafe(miLDR_ZElevR, piLDR_ZElevRPick)) bRet = false;}   //20160426 진섭
    else if(m_iManNo == mcLDR_RPitchDn          ){if(!LDR_R.CheckSafe(miLDR_ZElevR, piLDR_ZElevRPick)) bRet = false;}

    else if(m_iManNo == mcCycleLDR_IdxGet       ){}
    else if(m_iManNo == mcIDXCycleRotator       ){}

    else if(m_iManNo == mcLTLCycleMidSupply     ){}
    else if(m_iManNo == mcLTLCycleMidToWork     ){}
    else if(m_iManNo == mcLTLCycleClean         ){}
    else if(m_iManNo == mcLTLCycleMidLOri       ){}
    else if(m_iManNo == mcLTLCycleMidLAlign     ){}
    else if(m_iManNo == mcLTLCycleTopLUV        ){}
    else if(m_iManNo == mcLTLCycleCmsLDisp      ){}
    else if(m_iManNo == mcLTLCycleCmsLAlign     ){}
    else if(m_iManNo == mcLTLCycleCmsLOri       ){}
    else if(m_iManNo == mcLTLCycleHtReZero      ){}
    else if(m_iManNo == mcLTLCycleManNdlCheck   ){}
    else if(m_iManNo == mcLTLCycleWorkOut       ){}

    else if(m_iManNo == mcRTLCycleMidRHeight    ){}
    else if(m_iManNo == mcRTLCycleTrsfPick      ){}
    else if(m_iManNo == mcRTLCycleTrsfPlace     ){}
    else if(m_iManNo == mcRTLCycleCmsRHeight    ){}
    else if(m_iManNo == mcRTLCycleCmsRAlign     ){}
    else if(m_iManNo == mcRTLCycleCmsRGap       ){}
//    else if(m_iManNo == mcRTLCycleCmsRFixEnd    ){}
    else if(m_iManNo == mcRTLCycleMidREndHeight ){}
    else if(m_iManNo == mcRTLCycleOut           ){}
    else if(m_iManNo == mcRTLCycleHtReZero      ){}
    else if(m_iManNo == mcRTLCycleWkoToPtl      ){}
    else if(m_iManNo == mcRTLCycleCmsREndAlign  ){}
    else if(m_iManNo == mcRTLCycleCmsRZHeight   ){}

    else if(m_iManNo == mcULDCyclePick          ){}
    else if(m_iManNo == mcULDCyclePlce          ){}
    else if(m_iManNo == mcULDPitchUp            ){if(!ULD.CheckSafe(miULD_ZElevt, piULD_ZElevtPick)) bRet = false;}  //20160426 진섭
    else if(m_iManNo == mcULDPitchDn            ){if(!ULD.CheckSafe(miULD_ZElevt, piULD_ZElevtPick)) bRet = false;}

    else if(m_iManNo == mcETCCycleCnvrs         ){}
    else if(m_iManNo == mcCycleCadPos           ){}

    else if(m_iManNo == mcCycleManualEndHeight  ){}  //Jinseop LTL_CycleMidToWork - LTL_MidLAlign - RTL_MidREndHeight 연속동작하는 매뉴얼 사이클
    else if(m_iManNo == mcCycleLDR_IdxGet       ){}
    else if(m_iManNo == mcCycleLDR_IdxPlce      ){}
    else if(m_iManNo == mcCycleLDR_FPreGet      ){}
    else if(m_iManNo == mcCyclePstRailULDPlce   ){}
    else if(m_iManNo == mcCycleSTGChange        ){}



    if(!bRet)Init();

    /********************/
    /* 처리..           */
    /********************/

         if(m_iManNo == mcNoneCycle             ){}
    else if(m_iManNo == mcAllHome               ){MT_SetServoAll(true);
                                                    LDR_F.InitHomeStep ();
                                                    LDR_R.InitHomeStep ();
                                                    IDX.InitHomeStep   ();
                                                    LTL.InitHomeStep   ();
                                                    RTL.InitHomeStep   ();
                                                    STG.InitHomeStep   ();
                                                    ULD.InitHomeStep   ();
                                                   }
    else if(m_iManNo == mcLDR_FHome             ){LDR_F.InitHomeStep   ();}
    else if(m_iManNo == mcLDR_RHome             ){LDR_R.InitHomeStep   ();}
    else if(m_iManNo == mcIDX_Home              ){IDX  .InitHomeStep   ();}
    else if(m_iManNo == mcLTL_Home              ){LTL  .InitHomeStep   ();}
    else if(m_iManNo == mcRTL_Home              ){RTL  .InitHomeStep   ();}
    else if(m_iManNo == mcSTG_Home              ){STG  .InitHomeStep   ();}
    else if(m_iManNo == mcULD_Home              ){ULD  .InitHomeStep   ();}

    else if(m_iManNo == mcLDR_FCyclePick        ){LDR_F.InitCycleStep  ();}
    else if(m_iManNo == mcLDR_FPitchUp          ){MT_GoIncMan(miLDR_ZElevF ,   OM.DevInfo.dLDRFMgzSlotPitch) ;} //20160426 진섭
    else if(m_iManNo == mcLDR_FPitchDn          ){MT_GoIncMan(miLDR_ZElevF , -(OM.DevInfo.dLDRFMgzSlotPitch));}


    else if(m_iManNo == mcLDR_RCyclePick        ){LDR_R.InitCycleStep  ();}
    else if(m_iManNo == mcLDR_RCyclePlce        ){LDR_R.InitCycleStep  ();}
    else if(m_iManNo == mcLDR_RPitchUp          ){MT_GoIncMan(miLDR_ZElevR ,   OM.DevInfo.dLDRRMgzSlotPitch) ;} //20160426 진섭
    else if(m_iManNo == mcLDR_RPitchDn          ){MT_GoIncMan(miLDR_ZElevR , -(OM.DevInfo.dLDRRMgzSlotPitch));}

    else if(m_iManNo == mcIDXCycleRotator       ){IDX.InitCycleStep    ();}

    else if(m_iManNo == mcLTLCycleMidSupply     ){LTL.InitCycleStep    ();}
    else if(m_iManNo == mcLTLCycleMidToWork     ){LTL.InitCycleStep    ();}
    else if(m_iManNo == mcLTLCycleClean         ){LTL.InitCycleStep    ();}
    else if(m_iManNo == mcLTLCycleMidLOri       ){LTL.InitCycleStep    ();}
    else if(m_iManNo == mcLTLCycleMidLAlign     ){LTL.InitCycleStep    ();}
    else if(m_iManNo == mcLTLCycleTopLUV        ){LTL.InitCycleStep    ();}
    else if(m_iManNo == mcLTLCycleCmsLDisp      ){LTL.InitCycleStep    ();}
    else if(m_iManNo == mcLTLCycleCmsLAlign     ){LTL.InitCycleStep    ();}
    else if(m_iManNo == mcLTLCycleCmsLOri       ){LTL.InitCycleStep    ();}
    else if(m_iManNo == mcLTLCycleHtReZero      ){LTL.InitCycleStep    ();}
    else if(m_iManNo == mcLTLCycleManNdlCheck   ){m_iManStep = 10; m_iPreManStep = 0;}
    else if(m_iManNo == mcLTLCycleManUVTest     ){LTL.InitCycleStep    ();}
    else if(m_iManNo == mcLTLCycleWorkOut       ){LTL.InitCycleStep    ();}

    else if(m_iManNo == mcRTLCycleMidRHeight    ){RTL.InitCycleStep    ();}
    else if(m_iManNo == mcRTLCycleTrsfPick      ){RTL.InitCycleStep    ();}
    else if(m_iManNo == mcRTLCycleTrsfPlace     ){RTL.InitCycleStep    ();}
    else if(m_iManNo == mcRTLCycleCmsRHeight    ){RTL.InitCycleStep    ();}
    else if(m_iManNo == mcRTLCycleCmsRAlign     ){RTL.InitCycleStep    ();}
    else if(m_iManNo == mcRTLCycleCmsRGap       ){RTL.InitCycleStep    ();}
//    else if(m_iManNo == mcRTLCycleCmsRFixEnd    ){RTL.InitCycleStep    ();}
    else if(m_iManNo == mcRTLCycleMidREndHeight ){RTL.InitCycleStep    ();}
    else if(m_iManNo == mcRTLCycleOut           ){RTL.InitCycleStep    ();}
    else if(m_iManNo == mcRTLCycleHtReZero      ){RTL.InitCycleStep    ();}
    else if(m_iManNo == mcRTLCycleWkoToPtl      ){RTL.InitCycleStep    ();}
    else if(m_iManNo == mcRTLCycleCmsREndAlign  ){RTL.InitCycleStep    ();}
    else if(m_iManNo == mcRTLCycleCmsRZHeight   ){RTL.InitCycleStep    ();}

    else if(m_iManNo == mcULDCyclePick          ){ULD.InitCycleStep    ();}
    else if(m_iManNo == mcULDCyclePlce          ){ULD.InitCycleStep    ();}
    else if(m_iManNo == mcULDPitchUp            ){MT_GoIncMan(miULD_ZElevt ,   OM.DevInfo.dULDMgzSlotPitch) ;}  //20160426 진섭
    else if(m_iManNo == mcULDPitchDn            ){MT_GoIncMan(miULD_ZElevt , -(OM.DevInfo.dULDMgzSlotPitch));}

    else if(m_iManNo == mcETCCycleCnvrs         ){MT_SetServo(miLDR_YConvR, true); MT_SetServo(miSTG_YConvs, true);
                                                  m_iManStep = 10; m_iPreManStep = 0;
                                                 }
    else if(m_iManNo == mcCycleCadPos           ){m_iManStep = 10; m_iPreManStep = 0;}

    else if(m_iManNo == mcCycleManualEndHeight  ){m_iManStep = 10; m_iPreManStep = 0;}

    else if(m_iManNo == mcCycleLDR_IdxGet       ){m_iManStep = 10; m_iPreManStep = 0;}
    else if(m_iManNo == mcCycleLDR_IdxPlce      ){m_iManStep = 10; m_iPreManStep = 0;}
    else if(m_iManNo == mcCycleLDR_FPreGet      ){m_iManStep = 10; m_iPreManStep = 0;}
    else if(m_iManNo == mcCyclePstRailULDPlce   ){m_iManStep = 10; m_iPreManStep = 0;}
    else if(m_iManNo == mcCycleSTGChange        ){m_iManStep = 10; m_iPreManStep = 0;}




    m_bManSetting = false ; //m_bManSetting 중요함 리턴전에 꼭 펄스 시켜야함. 쓰레드가 달라서. ::Update에서 m_iManNo=0이 되므로 주의.
    return true ;
}

EN_MANUAL_CYCLE CManualMan::GetManNo()
{
    return m_iManNo ;
}

int CManualMan::GetManStep()
{
    return m_iManStep ;
}

//---------------------------------------------------------------------------
void CManualMan::Update(void)
{

    if( m_iManNo == mcNoneCycle ) return ;
    if(m_bManSetting            ) return ;

      if(!OM.CmnOptn.bIgnrDoor   ) SEQ.InspectDoor    ();    
      if(m_iManNo != mcLDR_FHome &&
         m_iManNo != mcLDR_RHome &&
         m_iManNo != mcIDX_Home  &&
         m_iManNo != mcLTL_Home  &&
         m_iManNo != mcRTL_Home  &&
         m_iManNo != mcSTG_Home  &&
         m_iManNo != mcULD_Home  &&
         m_iManNo != mcAllHome   &&
         m_iManNo != mcETCCycleCnvrs ) SEQ.InspectHomeEnd ();

      SEQ.InspectLightGrid();

      bool r1,r2,r3,r4,r5,r6,r7,r8,r9 ;
      r1=r2=r3=r4=r5=r6=r7=r8=r9 = false ;

      //Check Alarm.
      if ( EM_IsErr()      ) { Init(); return ; }             //test
    //Cycle Step.
    if      (m_iManNo == mcAllHome      ) { r1 = LTL  .CycleHome();
                                            r2 = RTL  .CycleHome();
                                            r3 = STG  .CycleHome();


                                            if(MT_GetHomeEnd(miRTL_ZTrsfr)) {IDX  .CycleHome();}
                                            else                            {r6 = false;          }
                                            if(!r1||!r2||!r3) {
                                                return ;
                                            }

                                            r6  = IDX  .CycleHome();

                                            if(!r6){
                                                return ;
                                            }


                                            r7  = LDR_F.CycleHome();
                                            r8  = LDR_R.CycleHome();
                                            r9  = ULD  .CycleHome();

                                            if(r7&&r8&&r9) {
                                                MT_SetHomeEnd (miSpare1 , true);
                                                MT_SetHomeEnd (miSpare2 , true);

                                                m_iManNo = mcNoneCycle;
                                                FM_MsgOk("Confirm","All Home Finished!");
                                            }
                                          }

    else if(m_iManNo == mcLDR_FHome             ){if(LDR_F.CycleHome        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_RHome             ){if(LDR_R.CycleHome        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLTL_Home              ){if(LTL  .CycleHome        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRTL_Home              ){if(RTL  .CycleHome        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcIDX_Home              ){if(IDX  .CycleHome        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_Home              ){if(ULD  .CycleHome        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTG_Home              ){if(STG  .CycleHome        ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcLDR_FCyclePick        ){if(LDR_F.CyclePick        ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcLDR_RCyclePick        ){if(LDR_R.CyclePick        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_RCyclePlce        ){if(LDR_R.CyclePlce        ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcIDXCycleRotator       ){if(IDX.CycleRotator       ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcLTLCycleMidSupply     ){if(LTL.CycleMidSupply     ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLTLCycleMidToWork     ){if(LTL.CycleMidToWork     ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLTLCycleClean         ){if(LTL.CycleClean         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLTLCycleMidLOri       ){if(LTL.CycleMidLOri       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLTLCycleMidLAlign     ){if(LTL.CycleMidLAlign     ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLTLCycleTopLUV        ){if(LTL.CycleTopLUV        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLTLCycleCmsLDisp      ){if(LTL.CycleCmsLDisp      ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLTLCycleCmsLAlign     ){if(LTL.CycleCmsLAlign     ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLTLCycleCmsLOri       ){if(LTL.CycleCmsLOri       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLTLCycleHtReZero      ){if(LTL.CycleHtReZero      ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLTLCycleManNdlCheck   ){if(CycleNdlCheck          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLTLCycleManUVTest     ){if(LTL.CycleManUVTest     ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLTLCycleWorkOut       ){if(LTL.CycleWorkOut       ()) m_iManNo = mcNoneCycle; }
     //{if(LTL.CycleManNdlCheck   ()) m_iManNo = mcNoneCycle; }


    else if(m_iManNo == mcRTLCycleMidRHeight    ){if(RTL.CycleMidRHeight    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRTLCycleTrsfPick      ){if(RTL.CycleTrsfPick      ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRTLCycleTrsfPlace     ){if(RTL.CycleTrsfPlace     ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRTLCycleCmsRHeight    ){if(RTL.CycleCmsRHeight    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRTLCycleCmsRAlign     ){if(RTL.CycleCmsRAlign     ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRTLCycleCmsRGap       ){if(RTL.CycleCmsREndAlign  ()) m_iManNo = mcNoneCycle; }
//    else if(m_iManNo == mcRTLCycleCmsRFixEnd    ){if(RTL.CycleCmsRFixEnd    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRTLCycleMidREndHeight ){if(RTL.CycleMidREndHeight ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRTLCycleOut           ){if(RTL.CycleOut           ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRTLCycleHtReZero      ){if(RTL.CycleHtReZero      ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRTLCycleWkoToPtl      ){if(RTL.CycleWkoToPtl      ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRTLCycleCmsREndAlign  ){if(RTL.CycleCmsREndAlign  ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcRTLCycleCmsRZHeight   ){if(RTL.CycleCmsRZHeight    ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcULDCyclePick          ){if(ULD.CyclePick          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULDCyclePlce          ){if(ULD.CyclePlce          ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcETCCycleCnvrs         ){if(CycleCnvr              ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcCycleCadPos           ){if(CycleCadMove           ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcCycleManualEndHeight  ){if(CycleManualEndHeight   ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcCycleLDR_IdxGet       ){if(CycleLDR_IdxGet        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcCycleLDR_IdxPlce      ){if(CycleLDR_IdxPlce       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcCycleLDR_FPreGet      ){if(CycleLDR_FPreGet       ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcCyclePstRailULDPlce   ){if(CyclePstRailULDPlce    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcCycleSTGChange        ){if(CycleSTGChange         ()) m_iManNo = mcNoneCycle; }






    else                                         {                              m_iManNo = mcNoneCycle; } //여기서 갱신됌.

    //Ok.
    return ;

}


//컨버전 Cycle..
bool CManualMan::CycleCnvr()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 30000 )) {
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

    if(EM_IsErr()){ //에러면 중지시킨다.
        m_iManStep = 0 ;
        return true ;
    }

    m_iPreManStep = m_iManStep ;

    static int iLDR_FCnt ;
    static int iLDR_RCnt ;
    static int iULDCnt   ;

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: if(!MT_GetHomeEnd(miSTG_XStage)){EM_SetErrMsg(eiMTR_HomeEnd , "스테이지 X모터 홈을 잡지 않았습니다.");m_iManStep=0; return true;}
                  if(!MT_GetHomeEnd(miSTG_YStage)){EM_SetErrMsg(eiMTR_HomeEnd , "스테이지 Y모터 홈을 잡지 않았습니다.");m_iManStep=0; return true;}

                  if(IO_GetX(xLDR_Detect1)){EM_SetErrMsg(eiPRT_RemovePkg , "로더인입부 센서가 감지중입니다."   ); m_iManStep=0; return true ;}
                  if(IO_GetX(xPRL_Detect2)){EM_SetErrMsg(eiPRT_RemovePkg , "프리레일 센서가 감지중입니다."     ); m_iManStep=0; return true ;}
                  if(IO_GetX(xSTG_Detect3)){EM_SetErrMsg(eiPRT_RemovePkg , "스테이지 좌측 센서가 감지중입니다."); m_iManStep=0; return true ;}
                  if(IO_GetX(xSTG_Detect4)){EM_SetErrMsg(eiPRT_RemovePkg , "스테이지 센터 센서가 감지중입니다."); m_iManStep=0; return true ;}
                  if(IO_GetX(xPTL_Detect6)){EM_SetErrMsg(eiPRT_RemovePkg , "스테이지 우측 센서가 감지중입니다."); m_iManStep=0; return true ;}
                  if(IO_GetX(xPTL_Detect6)){EM_SetErrMsg(eiPRT_RemovePkg , "포스트레일 센서가 감지중입니다."   ); m_iManStep=0; return true ;}
                  if(IO_GetX(xULD_Detect7)){EM_SetErrMsg(eiPRT_RemovePkg , "언로더 배출부 센서가 감지중입니다."); m_iManStep=0; return true ;}

                  LDR_F.MoveMotr(miLDR_ZElevF , piLDR_ZElevFPickFrst );
                  LDR_R.MoveMotr(miLDR_ZElevR , piLDR_ZElevRCheckFrst);
                  ULD  .MoveMotr(miULD_ZElevt , piULD_ZElevtPickFrst );

                  m_iManStep++;
                  return false ;

        case  11: if(!LDR_F.MoveMotr(miLDR_ZElevF , piLDR_ZElevFPickFrst ))return false ;
                  if(!LDR_R.MoveMotr(miLDR_ZElevR , piLDR_ZElevRCheckFrst))return false ;
                  if(!ULD  .MoveMotr(miULD_ZElevt , piULD_ZElevtPickFrst ))return false ;

                  iLDR_FCnt = 0 ;
                  iLDR_RCnt = 0 ;
                  iULDCnt   = 0 ;

                  m_iManStep++;
                  return false ;

        //현 스텝에서 루프 돔밑에서 씀.
        case  12: if(!MT_GetStopInpos(miLDR_ZElevF))return false ;
                  if(!MT_GetStopInpos(miLDR_ZElevR))return false ;
                  if(!MT_GetStopInpos(miULD_ZElevt))return false ;

                  if(IO_GetX(xLDR_FMidDetectL)){EM_SetErrMsg(eiPRT_RemovePkg , "로더 프론트 왼쪽 센서가 감지중입니다."  ); m_iManStep=0; return true ;}
                  if(IO_GetX(xLDR_FMidDetectR)){EM_SetErrMsg(eiPRT_RemovePkg , "로더 프론트 오른쪽 센서가 감지중입니다."); m_iManStep=0; return true ;}

                  if(IO_GetX(xLDR_RPcbDetectL)){EM_SetErrMsg(eiPRT_RemovePkg , "로더 리어 왼쪽 센서가 감지중입니다."    ); m_iManStep=0; return true ;}
                  if(IO_GetX(xLDR_RPcbDetectR)){EM_SetErrMsg(eiPRT_RemovePkg , "로더 리어 오른쪽 센서가 감지중입니다."  ); m_iManStep=0; return true ;}

                  if(IO_GetX(xULD_MidDetect  )){EM_SetErrMsg(eiPRT_RemovePkg , "언로더 센서가 감지중입니다."            ); m_iManStep=0; return true ;}


                  if(iLDR_FCnt < OM.DevInfo.iLDRFMgzSlotCnt)iLDR_FCnt ++ ;
                  if(iLDR_RCnt < OM.DevInfo.iLDRRMgzSlotCnt)iLDR_RCnt ++ ;
                  if(iULDCnt   < OM.DevInfo.iULDMgzSlotCnt )iULDCnt   ++ ;

                  if(iLDR_FCnt < OM.DevInfo.iLDRFMgzSlotCnt)MT_GoIncRun(miLDR_ZElevF , OM.DevInfo.dLDRFMgzSlotPitch);
                  if(iLDR_RCnt < OM.DevInfo.iLDRRMgzSlotCnt)MT_GoIncRun(miLDR_ZElevR , OM.DevInfo.dLDRRMgzSlotPitch);
                  if(iULDCnt   < OM.DevInfo.iULDMgzSlotCnt )MT_GoIncRun(miULD_ZElevt , OM.DevInfo.dULDMgzSlotPitch );

                  if(iLDR_FCnt < OM.DevInfo.iLDRFMgzSlotCnt ||
                     iLDR_RCnt < OM.DevInfo.iLDRRMgzSlotCnt ||
                     iULDCnt   < OM.DevInfo.iULDMgzSlotCnt  ){
                      return false ;

                  }

                  LDR_F.MoveMotr(miLDR_ZElevF , piLDR_ZElevFWait);
                  LDR_R.MoveMotr(miLDR_ZElevR , piLDR_ZElevRWait);
                  ULD  .MoveMotr(miULD_ZElevt , piULD_ZElevtWait);

                  m_iManStep++;
                  return false ;

        case  13: if(!LDR_F.MoveMotr(miLDR_ZElevF , piLDR_ZElevFWait))return false ;
                  if(!LDR_R.MoveMotr(miLDR_ZElevR , piLDR_ZElevRWait))return false ;
                  if(!ULD  .MoveMotr(miULD_ZElevt , piULD_ZElevtWait))return false ;

                  HexaPot.MoveAbsRes(paX , HEX_X_WAIT);
                  HexaPot.MoveAbsRes(paY , HEX_Y_WAIT);
                  HexaPot.MoveAbsRes(paZ , HEX_Z_WAIT);
                  HexaPot.MoveAbsRes(paU , HEX_U_WAIT);
                  HexaPot.MoveAbsRes(paV , HEX_V_WAIT);
                  HexaPot.MoveAbsRes(paW , HEX_W_WAIT);
                  HexaPot.Move();
                  m_iManStep++;
                  return false ;

        case  14: if(!HexaPot.GetStopInpos()) return false ;


                  STG.MoveMotr(miSTG_XStage , piSTG_XStageWait);
                  STG.MoveMotr(miSTG_YStage , piSTG_YStageWait);
                  m_iManStep++;
                  return false ;

        case  15: if(!STG.MoveMotr(miSTG_XStage , piSTG_XStageWait)) return false ;
                  if(!STG.MoveMotr(miSTG_YStage , piSTG_YStageWait)) return false ;


                  LTL.MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                  RTL.MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                  RTL.MoveActr(aiRTL_RWkPshUpDn , ccBwd);
                  m_iManStep++;
                  return false;

        case  16: if(!LTL.MoveActr(aiLTL_WrkPshDnUp , ccBwd))return false;
                  if(!RTL.MoveActr(aiRTL_LWkPshUpDn , ccBwd))return false;
                  if(!RTL.MoveActr(aiRTL_RWkPshUpDn , ccBwd))return false;
                  MT_DoHome(miLDR_YConvR);
                  MT_DoHome(miSTG_YConvs);
                  m_iManStep++;
                  return false;

        case  17: if(!MT_GetHomeEnd(miLDR_YConvR)) return false;
                  if(!MT_GetHomeEnd(miSTG_YConvs)) return false;
                  //Loader Sensor 확인해야 할수도 있따.
                  MT_GoAbsMan(miLDR_YConvR , PM.GetValue(miLDR_YConvR , pvLDR_YConvRWork));
                  MT_GoAbsMan(miSTG_YConvs , PM.GetValue(miSTG_YConvs , pvSTG_YConvsWork));
                  m_iManStep++;
                  return false;

        case  18: if(!MT_GoAbsMan(miLDR_YConvR , PM.GetValue(miLDR_YConvR , pvLDR_YConvRWork)))return false;
                  if(!MT_GoAbsMan(miSTG_YConvs , PM.GetValue(miSTG_YConvs , pvSTG_YConvsWork)))return false;
                  m_iManStep = 0;
                  return true;

        }
}

bool CManualMan::CycleVisnCal()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 30000 )) {
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

    if(EM_IsErr()){ //에러면 중지시킨다.
        m_iManStep = 0 ;
        return true ;
    }

    m_iPreManStep = m_iManStep ;

    static int iPckDnCnt = 0 ;
    const int iPckDnMaxCnt = 2 ;

    double dPckVisnRsltX , dPckVisnRsltY , dPckVisnRsltT ;
    double dDspVisnRsltX , dDspVisnRsltY , dDspVisnRsltT ;

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10:
                  m_iManStep++;
                  return false;

        case  11: m_iManStep = 0;
                  return true;

        }
}
bool CManualMan::CycleNdlCheck()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 30000 )) {
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

    if(EM_IsErr()){ //에러면 중지시킨다.
        m_iManStep = 0 ;
        return true ;
    }

    m_iPreManStep = m_iManStep ;

    static int iPckDnCnt = 0 ;
    const int iPckDnMaxCnt = 2 ;

    double dPckVisnRsltX , dPckVisnRsltY , dPckVisnRsltT ;
    double dDspVisnRsltX , dDspVisnRsltY , dDspVisnRsltT ;

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: RTL.InitCycleStep();
                  m_iManStep++;
                  return false ;

        case  11: if(!RTL.CycleToolVoid())return false ;
                  m_iManStep++;
                  return false;

        case  12: LTL.InitCycleStep();
                  m_iManStep++;
                  return false;

        case  13: if(!LTL.CycleClean())return false ;
                  LTL.InitCycleStep();
                  m_iManStep++;
                  return false;

        case  14: LTL.CycleManNdlCheck();
                  m_iManStep++;
                  return false;

        case  15: if(!LTL.CycleManNdlCheck()) return false;
                  m_iManStep = 0;
                  return true;

        }
}

bool CManualMan::GetVisnGap(double * _dVisnX , double * _dVisnY , double * _dVisnT)
{
    if(m_dVisnRsltGapX == 999)return false ;
    if(m_dVisnRsltGapY == 999)return false ;
    if(m_dVisnRsltGapT == 999)return false ;


    * _dVisnX = m_dVisnRsltGapX ;
    * _dVisnX = m_dVisnRsltGapY ;
    * _dVisnX = m_dVisnRsltGapT ;

    return true ;
}


bool CManualMan::CycleManualEndHeight()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    bool bCheckStop = MT_GetStop(miRTL_YVisn) && MT_GetStop(miRTL_XGenRr) ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode && bCheckStop , 30000 )) {
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

    if(EM_IsErr()){ //에러면 중지시킨다.
        m_iManStep = 0 ;
        return true ;
    }

    m_iPreManStep = m_iManStep ;

    static int iPckDnCnt = 0 ;
    const int iPckDnMaxCnt = 2 ;

    double dPckVisnRsltX , dPckVisnRsltY , dPckVisnRsltT ;
    double dDspVisnRsltX , dDspVisnRsltY , dDspVisnRsltT ;

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: LTL.InitCycleStep();
                  m_iManStep++;
                  return false;

        case  11: if(!LTL.CycleMidToWork()) return false;
                  LTL.InitCycleStep();
                  m_iManStep ++;
                  return false;

        case  12: if(!LTL.CycleMidLAlign()) return false;
                  RTL.InitCycleStep();
                  DM.ARAY[riSTG].SetStat(csMidREndHeight);
                  m_iManStep ++;
                  return false;

        //밑에서 씀.
        case  13: if(!RTL.CycleMidREndHeight()) return false;
                  m_iManStep++;
                  return false ;

        case  14: if(DM.ARAY[riSTG].GetCntStat(csMidREndHeight)){
                      RTL.InitCycleStep();
                      m_iManStep = 13 ;
                      return false ;
                  }


                  DM.ARAY[riSTG].SetStat(csNone);
                  m_iManStep = 0;
                  return true;
        }
}

//진섭 20160205
bool CManualMan::CycleLDR_IdxGet()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 30000 )) {
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

    if(EM_IsErr()){ //에러면 중지시킨다.
        m_iManStep = 0 ;
        return true ;
    }

    m_iPreManStep = m_iManStep ;

    //static int iPckDnCnt = 0 ;
    //const int iPckDnMaxCnt = 2 ;

    //double dPckVisnRsltX , dPckVisnRsltY , dPckVisnRsltT ;
    //double dDspVisnRsltX , dDspVisnRsltY , dDspVisnRsltT ;

    int r,c ; //FindChip 사용방법 물어보자!

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: if(OM.CmnOptn.bRLoadingStop) {
                      FM_MsgOk("Error","리어 로더 로딩 스탑중 입니다.");
                      m_iManStep = 0 ;
                      return true ;
                  }

                  if(DM.ARAY[riLDR_R].GetCntStat  (csUnkwn) <= 0) {
                      FM_MsgOk("Error","리어 로더에 UnKnwn(작업할 자제)이 없습니다.");
                      m_iManStep = 0 ;
                      return true ;
                  }

                  if(!DM.ARAY[riIDX].CheckAllStat(csNone)){
                      FM_MsgOk("Error","인덱스에 자제가 있습니다.");
                      m_iManStep = 0 ;
                      return true ;
                  }

                  if(!DM.ARAY[riRTT].CheckAllStat(csNone)){
                      FM_MsgOk("Error","로테이터에 자제가 있습니다.");
                      m_iManStep = 0 ;
                      return true ;
                  }

                  LDR_R.InitCycleStep();
                  m_iManStep++;
                  return false ;

        case  11: if(!LDR_R.CyclePick()) return false ;
                  IDX.InitCycleStep();
                  m_iManStep++;
                  return false;

        case  12: if(!IDX.CycleGet()) return false;
                  m_iManStep = 0;
                  return true;

        }
}

bool CManualMan::CycleLDR_IdxPlce()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 30000 )) {
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

    if(EM_IsErr()){ //에러면 중지시킨다.
        m_iManStep = 0 ;
        return true ;
    }

    m_iPreManStep = m_iManStep ;

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: if(DM.ARAY[riRTT].CheckAllStat(csNone)) {
                      FM_MsgOk("Error","로테이터에 자재가 없습니다.");
                      m_iManStep = 0 ;
                      return true ;
                  }

                  if(!IDX.MoveActr(aiIDX_RoatorUpDn , ccFwd)) {
                      FM_MsgOk("Error","로테이터가 다운되어 있습니다.");
                      m_iManStep = 0 ;
                      return true ;
                  }

                  if(DM.ARAY[riLDR_R].GetCntStat  (csEmpty) <= 0) {
                      FM_MsgOk("Error","리어 로더에 자재가 있습니다.");
                      m_iManStep = 0 ;
                      return true ;
                  }

                  LDR_R.InitCycleStep();
                  m_iManStep++;
                  return false;

        case  11: if(!LDR_R.CyclePlce()) return false;
                  IDX.InitCycleStep();
                  m_iManStep++;
                  return false;

        case  12: if(!IDX.CyclePlace()) return false;
                  m_iManStep = 0;
                  return true;

        }
}

bool CManualMan::CycleLDR_FPreGet()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 30000 )) {
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

    if(EM_IsErr()){ //에러면 중지시킨다.
        m_iManStep = 0 ;
        return true ;
    }

    m_iPreManStep = m_iManStep ;


    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: if(OM.CmnOptn.bFLoadingStop) {
                      FM_MsgOk("Error","프론트 로더 로딩 스탑중 입니다.");
                      m_iManStep = 0 ;
                      return true ;
                  }

                  if(DM.ARAY[riLDR_F].GetCntStat(csUnkwn) <= 0) {
                      FM_MsgOk("Error","프론트 로더에 UnKnwn(작업할 자제)이 없습니다.");
                      m_iManStep = 0 ;
                      return true ;
                  }

                  if(!DM.ARAY[riPRL].CheckAllStat(csNone) ||
                     !DM.ARAY[riSTG].CheckAllStat(csNone) ||
                     !DM.ARAY[riWKO].CheckAllStat(csNone) ||
                     !DM.ARAY[riPTL].CheckAllStat(csNone)){
                      FM_MsgOk("Error","레일 내에 자재가 있습니다.");
                      m_iManStep = 0 ;
                      return true ;
                  }

                  LDR_F.InitCycleStep();
                  m_iManStep++;
                  return false ;

        case  11: if(!LDR_F.CyclePick()) return false;
                  LTL.InitCycleStep();
                  m_iManStep++;
                  return false;

        case  12: if(!LTL.CycleMidSupply()) return false;
                  m_iManStep = 0;
                  return true;

        }
}

bool CManualMan::CyclePstRailULDPlce()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 30000 )) {
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

    if(EM_IsErr()){ //에러면 중지시킨다.
        m_iManStep = 0 ;
        return true ;
    }

    m_iPreManStep = m_iManStep ;

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: if(!DM.ARAY[riPTL].CheckAllStat(csWork)) {
                      FM_MsgOk("Error","포스트 레일에 자재가 없습니다.");
                      m_iManStep = 0;
                      return true;
                  }

                  if(DM.ARAY[riULD].GetCntStat  (csEmpty) <=0) {
                     FM_MsgOk("Error","언로더가 빈공간(Empty)가 없습니다.");
                     m_iManStep = 0;
                     return true;
                  }

                  ULD.InitCycleStep();
                  m_iManStep++;
                  return false;

        case  11: if(!ULD.CyclePlce()) return false;
                  RTL.InitCycleStep();
                  m_iManStep++;
                  return false;

        case  12: if(!RTL.CycleOut()) return false;
                  m_iManStep = 0;
                  return true;

        }
}


bool CManualMan::CycleSTGChange()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 30000 )) {
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

    if(EM_IsErr()){ //에러면 중지시킨다.
        m_iManStep = 0 ;
        return true ;
    }

    m_iPreManStep = m_iManStep ;

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: if(IO_GetX(xSTG_Detect3) || IO_GetX(xSTG_Detect4) || IO_GetX(xWKO_Detect5)) {
                      FM_MsgOk("Error","스테이지 자재 감지 센서가 감지 중 입니다.");
                      m_iManStep = 0;
                      return true;
                  }

                  if(IO_GetX(xSTG_StgVcum1) || IO_GetX(xSTG_StgVcum2) || IO_GetX(xSTG_StgVcum3)){
                     FM_MsgOk("Error","스테이지 배큠 센서가 감지 중 입니다.");
                     m_iManStep = 0;
                     return true;
                  }

                  STG.InitCycleStep();

                  m_iManStep++;
                  return false;

        case  11: STG.MoveMotr(miSTG_XStage , piSTG_XStageWait) ;
                  STG.MoveMotr(miSTG_YStage , piSTG_YStageWait) ;
                  m_iManStep++;
                  return false;

        case  12: if(!STG.MoveMotr(miSTG_XStage , piSTG_XStageWait)) return false;
                  if(!STG.MoveMotr(miSTG_YStage , piSTG_YStageWait)) return false;
                  m_iManStep++;
                  return false;

        case  13: if(!HexaPot.DoHome()){
                      EM_SetErrMsg(eiSTG_HexaPot , "헥사포트 홈동작 에러 입니다.");
                      Trace("HexaPotErr",HexaPot.GetLastErrMsg().c_str());
                      m_iManStep = 0 ;
                      return true ;
                  }
                  m_iManStep++;
                  return false ;

        case  14: if(!HexaPot.GetStopInpos()) return false ;
                  HexaPot.SetPivotPnt(0.0,0.0,HEX_Z_PIVOT_PLACE_ZIG_TOP);
                  HexaPot.MoveAbsRes(paZ , HEX_Z_WAIT); //MIN -12.5
                  HexaPot.Move();
                  m_iManStep = 0;
                  return true;

        }
}


//---------------------------------------------------------------------------
void CManualMan::SetCadMove(EN_TOOL_ID _tiTool, double _dX, double _dY)
{
    m_eToolId   = _tiTool ;
    m_dCadMoveX = _dX ;
    m_dCadMoveY = _dY ;

    SetManCycle(mcCycleCadPos) ;

    //SetManCycle(CycleCadMove());

}
//---------------------------------------------------------------------------
bool CManualMan::CycleCadMove()
{
     //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 30000 )) {
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

    if(EM_IsErr()){ //에러면 중지시킨다.
        m_iManStep = 0 ;
        return true ;
    }

    m_iPreManStep = m_iManStep ;

    EN_MOTR_ID eXMotrId ;
    EN_MOTR_ID eYMotrId  ;

    EN_TOOL_ID eToolId = m_eToolId ;
    double dCadMoveX   = m_dCadMoveX ;
    double dCadMoveY   = m_dCadMoveY ;

    
    static double dMidPosOfsX ;
    static double dMidPosOfsY ;

         if(eToolId == tlLVisn    ) { eXMotrId = miLTL_XGenRr; eYMotrId = miLTL_YVisn ; }
    else if(eToolId == tlLHght    ) { eXMotrId = miLTL_XGenRr; eYMotrId = miLTL_YVisn ; }
    else if(eToolId == tlLUV      ) { eXMotrId = miLTL_XGenRr; eYMotrId = miLTL_YDispr; }
    else if(eToolId == tlLDspR    ) { eXMotrId = miLTL_XGenRr; eYMotrId = miLTL_YDispr; }
    else if(eToolId == tlLDspF    ) { eXMotrId = miLTL_XGenRr; eYMotrId = miLTL_YDispr; }
    else if(eToolId == tlRTrns    ) { eXMotrId = miRTL_XGenRr; eYMotrId = miRTL_YTrspr; }
    else if(eToolId == tlRVisn    ) { eXMotrId = miRTL_XGenRr; eYMotrId = miRTL_YVisn ; }
    else if(eToolId == tlRHght    ) { eXMotrId = miRTL_XGenRr; eYMotrId = miRTL_YVisn ; }
    else if(eToolId == tlStage    ) { eXMotrId = miSTG_XStage; eYMotrId = miSTG_YStage; }
    else if(eToolId == tlRConfocal) { eXMotrId = miRTL_XGenRr; eYMotrId = miRTL_YVisn ; }
    else                            { m_iManStep = 0; return true;                      }

    double dMaxVal ;
    double dMinVal ;

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: if(!DM.ARAY[riSTG].CheckAllStat(csNone     ) &&
                     !DM.ARAY[riSTG].CheckAllStat(csMidLOri  ) &&
                     !DM.ARAY[riSTG].CheckAllStat(csMidLAlign) ){
                      //레프트 리어.
                      if(eToolId == tlLVisn){dMidPosOfsX = g_tMidPosOfs.dX ; dMidPosOfsY = g_tMidPosOfs.dY ;}
                      if(eToolId == tlLHght){dMidPosOfsX = g_tMidPosOfs.dX ; dMidPosOfsY = g_tMidPosOfs.dY ;}

                      //레프트 프론트
                      if(eToolId == tlLUV  ){dMidPosOfsX = g_tMidPosOfs.dX ; dMidPosOfsY =-g_tMidPosOfs.dY ;}
                      if(eToolId == tlLDspR){dMidPosOfsX = g_tMidPosOfs.dX ; dMidPosOfsY =-g_tMidPosOfs.dY ;}
                      if(eToolId == tlLDspF){dMidPosOfsX = g_tMidPosOfs.dX ; dMidPosOfsY =-g_tMidPosOfs.dY ;}

                      //라이트 리어.
                      if(eToolId == tlRTrns){dMidPosOfsX =-g_tMidPosOfs.dX ; dMidPosOfsY = g_tMidPosOfs.dY ;}

                      //라이트 프론트.
                      if(eToolId == tlRVisn    ){dMidPosOfsX =-g_tMidPosOfs.dX ; dMidPosOfsY =-g_tMidPosOfs.dY ;}
                      if(eToolId == tlRHght    ){dMidPosOfsX =-g_tMidPosOfs.dX ; dMidPosOfsY =-g_tMidPosOfs.dY ;}
                      if(eToolId == tlRConfocal){dMidPosOfsX =-g_tMidPosOfs.dX ; dMidPosOfsY =-g_tMidPosOfs.dY ;}

                      //스테이지.
                      if(eToolId == tlStage){dMidPosOfsX = g_tMidPosOfs.dX ; dMidPosOfsY = g_tMidPosOfs.dY ;}

                  }
                  else {
                      dMidPosOfsX = 0.0;
                      dMidPosOfsY = 0.0;

                  }


                  if(eToolId == tlLVisn ||
                     eToolId == tlLHght ){

                     m_iManStep = 20;
                     return false;
                  }
                  else if(eToolId == tlLUV   ||
                          eToolId == tlLDspR ||
                          eToolId == tlLDspF ){
                      m_iManStep = 30;
                      return false;
                  }
                  else if(eToolId == tlRTrns){
                      m_iManStep = 40;
                      return false;
                  }
                  else if(eToolId == tlRVisn ||
                          eToolId == tlRHght ||
                          eToolId == tlRConfocal ){
                      m_iManStep = 50;
                      return false;
                  }
                  else if(eToolId == tlStage){
                      m_iManStep = 60;
                      return false;
                  }

                  //if(eToolId == tlLVisn ||
                  //   eToolId == tlLHght ||
                  //   eToolId == tlLUV   ||
                  //   eToolId == tlLDspR ||
                  //   eToolId == tlLDspF ){
                  //    //LTL.SetCadOfsGain(eToolId);
                  //    m_iManStep = 20;
                  //    return false;
                  //}
                  //else if(eToolId == tlRTrns ||
                  //        eToolId == tlRVisn ||
                  //        eToolId == tlRHght ){
                  //    //RTL.SetCadOfsGain(eToolId);
                  //    m_iManStep = 30;
                  //    return false;
                  //}
                  //else if(eToolId == tlStage){
                  //    //RTL.SetCadOfsGain(eToolId);
                  //    m_iManStep = 40;
                  //    return false;
                  //}
                  m_iManStep = 0;
                  return true;

        //위에서 씀.
        // Left Tool Visn
        case  20: RTL.InitCycleStep();
                  m_iManStep++;
                  return false ;

        case  21: if(!RTL.CycleToolVoid())return false ;
                  LTL.MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                  LTL.MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);

                  LTL.MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                  LTL.MoveActr(aiLTL_MGrperDnUp , ccBwd);
                  LTL.MoveActr(aiLTL_SGrperDnUp , ccBwd);

                  LTL.MoveActr(aiLTL_DisprFDnUp , ccBwd);
                  LTL.MoveActr(aiLTL_DisprRDnUp , ccBwd);
                  LTL.MoveActr(aiLTL_UVCureDnUp , ccBwd);

                  m_iManStep++;
                  return false ;

        case  22: if(!LTL.MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                  if(!LTL.MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;

                  if(!LTL.MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;
                  if(!LTL.MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                  if(!LTL.MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;

                  if(!LTL.MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                  if(!LTL.MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                  if(!LTL.MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;

                  LTL.MoveMotr(miLTL_YDispr , piLTL_YDispWait) ;

                  m_iManStep++ ;
                  return false ;

        case  23: if(!LTL.MoveMotr(miLTL_YDispr , piLTL_YDispWait)) return false ;
                  MT_GoAbsMan(eXMotrId, dCadMoveX );
                  MT_GoAbsMan(eYMotrId, dCadMoveY );
                  m_iManStep++;
                  return false;

        case  24: if(!MT_GetStop(eXMotrId)) return false ;
                  if(!MT_GetStop(eYMotrId)) return false ;
                  m_iManStep = 100;
                  return false;


        // Left Tool Dispr
        case  30: RTL.InitCycleStep();
                  m_iManStep++;
                  return false ;

        case  31: if(!RTL.CycleToolVoid())return false ;
                  LTL.MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                  LTL.MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);

                  LTL.MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                  LTL.MoveActr(aiLTL_MGrperDnUp , ccBwd);
                  LTL.MoveActr(aiLTL_SGrperDnUp , ccBwd);

                  LTL.MoveActr(aiLTL_DisprFDnUp , ccBwd);
                  LTL.MoveActr(aiLTL_DisprRDnUp , ccBwd);
                  LTL.MoveActr(aiLTL_UVCureDnUp , ccBwd);

                  m_iManStep++;
                  return false ;

        case  32: if(!LTL.MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                  if(!LTL.MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;

                  if(!LTL.MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;
                  if(!LTL.MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                  if(!LTL.MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;

                  if(!LTL.MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                  if(!LTL.MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                  if(!LTL.MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;

                  LTL.MoveMotr(miLTL_YVisn , piLTL_YVisnWait) ;

                  m_iManStep++ ;
                  return false ;

        case  33: if(!LTL.MoveMotr(miLTL_YVisn , piLTL_YVisnWait)) return false ;
                  MT_GoAbsMan(eXMotrId, dCadMoveX );
                  MT_GoAbsMan(eYMotrId, dCadMoveY );
                  m_iManStep++;
                  return false;

        case  34: if(!MT_GetStop(eXMotrId)) return false ;
                  if(!MT_GetStop(eYMotrId)) return false ;
                  m_iManStep = 100;
                  return false;


        //위에서 씀.
        // Right Tool Trnsfr
        case  40: LTL.InitCycleStep();
                  m_iManStep++;
                  return false ;

        case  41: if(!LTL.CycleToolVoid()) return false ;
                  RTL.MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);
                  RTL.MoveMotr(miRTL_ZVisn  , piRTL_ZVisnWait );
                  RTL.MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                  RTL.MoveActr(aiRTL_RWkPshUpDn , ccBwd);

                  m_iManStep++;
                  return false ;

        case  42: if(!RTL.MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait)) return false ;
                  if(!RTL.MoveMotr(miRTL_ZVisn  , piRTL_ZVisnWait )) return false ;
                  if(!RTL.MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                  if(!RTL.MoveActr(aiRTL_RWkPshUpDn , ccBwd)) return false ;

                  RTL.MoveMotr(miRTL_YVisn  , piRTL_YVisnWait );
                  m_iManStep++;
                  return false;

        case  43: if(!RTL.MoveMotr(miRTL_YVisn  , piRTL_YVisnWait )) return false ;
                  MT_GoAbsMan(eYMotrId, dCadMoveY);
                  MT_GoAbsMan(eXMotrId, dCadMoveX);
                  m_iManStep++;
                  return false;

        case  44: if(!MT_GetStop(eYMotrId)) return false ;
                  if(!MT_GetStop(eXMotrId)) return false ;
                  m_iManStep = 100;
                  return false;


        // Right Tool Visn
        case  50: LTL.InitCycleStep();
                  m_iManStep++;
                  return false ;

        case  51: if(!LTL.CycleToolVoid()) return false ;
                  RTL.MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);
                  RTL.MoveMotr(miRTL_ZVisn  , piRTL_ZVisnWait );
                  RTL.MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                  RTL.MoveActr(aiRTL_RWkPshUpDn , ccBwd);

                  m_iManStep++;
                  return false ;

        case  52: if(!RTL.MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait)) return false ;
                  if(!RTL.MoveMotr(miRTL_ZVisn  , piRTL_ZVisnWait )) return false ;
                  if(!RTL.MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                  if(!RTL.MoveActr(aiRTL_RWkPshUpDn , ccBwd)) return false ;
                  RTL.MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait);
                  m_iManStep++;
                  return false;

        case  53: if(!RTL.MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait)) return false;
                  MT_GoAbsMan(eYMotrId, dCadMoveY);
                  MT_GoAbsMan(eXMotrId, dCadMoveX);
                  m_iManStep++;
                  return false;

        case  54: if(!MT_GetStop(eYMotrId)) return false ;
                  if(!MT_GetStop(eXMotrId)) return false ;


                  m_iManStep = 100;
                  return false;


        //위에서씀 Stage 구동.
        case  60: //STG.InitCycleStep();
                  //HexaPot.MoveAbsRes(paZ ,HEX_Z_WAIT );
                  //HexaPot.Move();
                  m_iManStep++;
                  return false ;

        case  61: if(!HexaPot.GetStopInpos()) return false ;

                  //스테이지와 레일 밑단 충돌 방지.
                  RTL.SetCadOfsGain(tlStage);
                  for(int r = 0 ; r < CAD.GetRow() ; r++){
                      for(int c = 0 ; c < CAD.GetCol() ; c++){
                          if(CAD.CM[c][r].CM_Rect->GetCntrPosY() > dMaxVal){
                              dMaxVal = CAD.CM[c][r].CM_Rect->GetCntrPosY() ;
                          }
                          if(CAD.CM[c][r].CM_Rect->GetCntrPosY() < dMinVal){
                              dMinVal = CAD.CM[c][r].CM_Rect->GetCntrPosY() ;
                          }
                      }
                  }
                  if(dCadMoveY > dMaxVal +1) {EM_SetErrMsg(eiPRT_Crash , "갈수 있는 포지션을 넘어섰습니다."); m_iManStep = 0 ; return true ;}
                  if(dCadMoveY < dMinVal -1) {EM_SetErrMsg(eiPRT_Crash , "갈수 있는 포지션을 넘어섰습니다."); m_iManStep = 0 ; return true ;}
                  MT_GoAbsMan(eYMotrId, dCadMoveY);
                  MT_GoAbsMan(eXMotrId, dCadMoveX);
                  m_iManStep++;
                  return false;

        case  62: if(!MT_GetStop(eYMotrId)) return false ;
                  if(!MT_GetStop(eXMotrId)) return false ;
                  m_iManStep = 0;
                  return true ;



        case 100: //Z축 움직일려면 찾아서 움직여야 한다!JS
                       if(eToolId == tlLVisn) {

                           LTL.MoveMotr(miLTL_ZVisn  , piLTL_ZVisnCmsAlign );
                  }
                  else if(eToolId == tlLUV      ) LTL.MoveMotr(miLTL_ZDispr , piLTL_ZDispUVWork  );
                  else if(eToolId == tlLDspR    ) LTL.MoveMotr(miLTL_ZDispr , piLTL_ZDispRrCheck );
                  else if(eToolId == tlLDspF    ) LTL.MoveMotr(miLTL_ZDispr , piLTL_ZDispFtCheck );
                  else if(eToolId == tlRVisn    ) RTL.MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCmsAlign);
                  else if(eToolId == tlRConfocal) RTL.MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCmsConfocal);

                  m_iManStep++;
                  return false ;

        case 101: if(eToolId == tlLVisn     && !LTL.MoveMotr(miLTL_ZVisn  , piLTL_ZVisnCmsAlign   ))return false;
                  if(eToolId == tlLUV       && !LTL.MoveMotr(miLTL_ZDispr , piLTL_ZDispUVWork     ))return false;
                  if(eToolId == tlLDspR     && !LTL.MoveMotr(miLTL_ZDispr , piLTL_ZDispRrCheck    ))return false;
                  if(eToolId == tlLDspF     && !LTL.MoveMotr(miLTL_ZDispr , piLTL_ZDispFtCheck    ))return false;
                  if(eToolId == tlRVisn     && !RTL.MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCmsAlign   ))return false;
                  if(eToolId == tlRConfocal && !RTL.MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCmsConfocal))return false;

                  m_iManStep = 0;
                  return true ;


    }


}
//---------------------------------------------------------------------------






