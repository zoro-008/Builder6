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
#include "Rail.h"
#include "Head.h"
#include "PstBuff.h"
#include "UnLoader.h"
#include "UnLoaderMotor.h"

#include "PstnMan.h"
//---------------------------------------------------------------------------
//#include "FormDeviceSet.h"
#include "FormMain.h"
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

    m_bManSetting = false ;

    m_tmCycle.Clear() ;
}

//---------------------------------------------------------------------------
bool __fastcall CManualMan::SetManCycle(EN_MANUAL_CYCLE _iNo)
{
    if( _iNo     <  0                ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( _iNo     >= MAX_MANUAL_CYCLE ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( m_iManNo != mcNoneCycle      ) {FM_MsgOk("ERROR","Doing Manual Cycle"   ); return false ;}
    if(SEQ._bRun && !mcULD_TrayOpen  ) {FM_MsgOk("ERROR","Autorunning"          ); return false ;}

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return false ; } //아 밑에 처리 하는 애 때문에 잠시 이렇게 함.

    if(_iNo  == mcAllHome  && (!IO_GetX(xETC_MainSol) || !IO_GetY(yETC_MainSol))) {FM_MsgOk("ERROR","Check Main Air!"             ); return false ;}
    //if(_iNo  == mcAllHome  && ( IO_GetX(xULD_LTrayDetect)                      )) {FM_MsgOk("ERROR","Check Left Tray In Sensor!"  ); return false ;}
    //if(_iNo  == mcLDR_Home && ( IO_GetX(xPRB_PkgIn) /*|| IO_GetX(xPRB_2Pkg )*/))  {FM_MsgOk("ERROR","Check Rail In Sensor!"       ); return false ;}

    EM_SetDisp(true);

    int c,r ;
    int iMaxRow,iMaxCol;


    m_bManSetting = true ; //업데이트 함수에서 다른쓰레드로 들어와서 갱신하기에 플레그 걸어 막아둠.
    m_iManNo = _iNo ;

    bool bRet = true ;

    /********************/
    /* 프리프로세서     */
    /********************/
         if (m_iManNo == mcAllHome           ){     if(!IO_GetX(xETC_PowerOn  )) {FM_MsgOk("ERR","Power On Plz"); bRet = false ;}
                                              else if(!IO_GetX(xETC_MainSol) || !IO_GetY(yETC_MainSol) ) {FM_MsgOk("ERR","Air On Plz"); bRet = false ;}}
                                              //여기서 굳이 하는건 아닌거 같네...시퀀스에서 에러 띄우는게... ?


    else if (m_iManNo == mcLDR_Home             ){}
    else if (m_iManNo == mcLDR_Supply           ){}
    else if (m_iManNo == mcLDR_Work             ){}
    else if (m_iManNo == mcLDR_Pick             ){}
    else if (m_iManNo == mcLDR_Out              ){}
    else if (m_iManNo == mcLDR_WorkSttPs        ){if(!MT_CmprPos(miLDR_YFed , PM.GetValue(miLDR_YFed , pvLDR_YFedWorkStt))) {
                                                      if(!LDR.CheckSafe(miLDR_YFed , piLDR_YFedWorkStt)) bRet = false ;}}
    else if (m_iManNo == mcLDR_PusherUpDn       ){if(AT_Complete(aiLDR_Pusher  , ccFwd)) { if(!LDR.CheckSafe(aiLDR_Pusher  , ccBwd)) bRet = false ;}
                                                  if(AT_Complete(aiLDR_Pusher  , ccBwd)) { if(!LDR.CheckSafe(aiLDR_Pusher  , ccFwd)) bRet = false ;}}
    else if (m_iManNo == mcLDR_RotateFwBw       ){if(AT_Complete(aiLDR_Rotator , ccFwd)) { if(!LDR.CheckSafe(aiLDR_Rotator , ccBwd)) bRet = false ;}
                                                  if(AT_Complete(aiLDR_Rotator , ccBwd)) { if(!LDR.CheckSafe(aiLDR_Rotator , ccFwd)) bRet = false ;}}
    else if (m_iManNo == mcRAL_PreStprDn        ){if(AT_Complete(aiPRB_Stopper , ccFwd)) { if(!RAL.CheckSafe(aiPRB_Stopper , ccBwd)) bRet = false ;}
                                                  if(AT_Complete(aiPRB_Stopper , ccBwd)) { if(!RAL.CheckSafe(aiPRB_Stopper , ccFwd)) bRet = false ;}}

    else if (m_iManNo == mcHED_Home             ){}
    else if (m_iManNo == mcHED_InspMan          ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcHED_Wk1CmpFB         ){if(AT_Complete(aiWK1_FlpCmp  , ccFwd)) { if(!RAL.CheckSafe(aiWK1_FlpCmp  , ccBwd)) bRet = false ;}
                                                  if(AT_Complete(aiWK1_FlpCmp  , ccBwd)) { if(!RAL.CheckSafe(aiWK1_FlpCmp  , ccFwd)) bRet = false ;}}
    else if (m_iManNo == mcHED_Wk1InsFB         ){if(AT_Complete(aiWK1_Ins     , ccFwd)) { if(!RAL.CheckSafe(aiWK1_Ins     , ccBwd)) bRet = false ;}
                                                  if(AT_Complete(aiWK1_Ins     , ccBwd)) { if(!RAL.CheckSafe(aiWK1_Ins     , ccFwd)) bRet = false ;}}
    else if (m_iManNo == mcHED_Wk1AlgnFB        ){if(AT_Complete(aiWK1_Align   , ccFwd)) { if(!RAL.CheckSafe(aiWK1_Align   , ccBwd)) bRet = false ;}
                                                  if(AT_Complete(aiWK1_Align   , ccBwd)) { if(!RAL.CheckSafe(aiWK1_Align   , ccFwd)) bRet = false ;}}
    else if (m_iManNo == mcHED_Wk1StrpFB        ){if(AT_Complete(aiWK1_Stopper , ccFwd)) { if(!RAL.CheckSafe(aiWK1_Stopper , ccBwd)) bRet = false ;}
                                                  if(AT_Complete(aiWK1_Stopper , ccBwd)) { if(!RAL.CheckSafe(aiWK1_Stopper , ccFwd)) bRet = false ;}}
    else if (m_iManNo == mcHED_Wk1InversePs     ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcHED_Wk1NormalPs      ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcHED_Wk2AlgnFB        ){if(AT_Complete(aiWK2_Align   , ccFwd)) { if(!RAL.CheckSafe(aiWK2_Align   , ccBwd)) bRet = false ;}
                                                  if(AT_Complete(aiWK2_Align   , ccBwd)) { if(!RAL.CheckSafe(aiWK2_Align   , ccFwd)) bRet = false ;}}
    else if (m_iManNo == mcHED_Wk2StrpFB        ){if(AT_Complete(aiWK2_Stopper , ccFwd)) { if(!RAL.CheckSafe(aiWK2_Stopper , ccBwd)) bRet = false ;}
                                                  if(AT_Complete(aiWK2_Stopper , ccBwd)) { if(!RAL.CheckSafe(aiWK2_Stopper , ccFwd)) bRet = false ;}}
    else if (m_iManNo == mcHED_Wk3AlgnFB        ){if(AT_Complete(aiWK3_Align   , ccFwd)) { if(!RAL.CheckSafe(aiWK3_Align   , ccBwd)) bRet = false ;}
                                                  if(AT_Complete(aiWK3_Align   , ccBwd)) { if(!RAL.CheckSafe(aiWK3_Align   , ccFwd)) bRet = false ;}}
    else if (m_iManNo == mcHED_Wk3StrpFB        ){if(AT_Complete(aiWK3_Stopper , ccFwd)) { if(!RAL.CheckSafe(aiWK3_Stopper , ccBwd)) bRet = false ;}
                                                  if(AT_Complete(aiWK3_Stopper , ccBwd)) { if(!RAL.CheckSafe(aiWK3_Stopper , ccFwd)) bRet = false ;}}
    else if (m_iManNo == mcHED_FeedingOnOff     ){}

    else if (m_iManNo == mcPSB_Home             ){}
    else if (m_iManNo == mcPSB_InitPos          ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcPSB_MrkSttPs         ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcPSB_VisnSttPs        ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcPSB_AlgnFB           ){if(AT_Complete(aiPSB_Align   , ccFwd)) { if(!PSB.CheckSafe(aiPSB_Align   , ccBwd)) bRet = false ;}
                                                  if(AT_Complete(aiPSB_Align   , ccBwd)) { if(!PSB.CheckSafe(aiPSB_Align   , ccFwd)) bRet = false ;}}
    else if (m_iManNo == mcPSB_StprUD           ){if(AT_Complete(aiPSB_Stopper , ccFwd)) { if(!PSB.CheckSafe(aiPSB_Stopper , ccBwd)) bRet = false ;}
                                                  if(AT_Complete(aiPSB_Stopper , ccBwd)) { if(!PSB.CheckSafe(aiPSB_Stopper , ccFwd)) bRet = false ;}}
    else if (m_iManNo == mcPSB_MarkingUD        ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcPSB_MrkChangPs       ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcPSB_TestMarking      ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcPSB_FeedingOnOff     ){}
    else if (m_iManNo == mcPSB_Remove           ){}
    else if (m_iManNo == mcPSB_ReviewNext       ){}

    else if (m_iManNo == mcULD_Home             ){}
    else if (m_iManNo == mcULD_StprFwPs         ){}
    else if (m_iManNo == mcULD_StprBwPs         ){}
    else if (m_iManNo == mcULD_VaccumOnOff      ){}
    else if (m_iManNo == mcULD_EjectOnOff       ){}
    else if (m_iManNo == mcULD_TrayMove         ){}
    else if (m_iManNo == mcULD_TrayOpen         ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcULD_PaperSupply      ){}
    else if (m_iManNo == mcULD_PaperWorkStt     ){}
    else if (m_iManNo == mcULD_PickPs           ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcULD_FrstPlacePs      ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcULD_PaperPs          ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcULD_Pick             ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcULD_Place            ){m_iManStep = 10 ; m_iPreManStep = 0;}

    else if (m_iManNo == mcRAL_Home             ){}
    else if (m_iManNo == mcULM_Home             ){}
    else if (m_iManNo == mcETC_AllBeltOn        ){}
    else if (m_iManNo == mcETC_AllBeltOff       ){}
    else if (m_iManNo == mcHED_AllStprUp        ){}
    else if (m_iManNo == mcETC_ALlStprDn        ){}
    else if (m_iManNo == mcETC_MainAirOff       ){}
    else if (m_iManNo == mcETC_ManLotEnd        ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcETC_InTrayStt        ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcETC_OutTrayStt       ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcETC_PaperStt         ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcETC_PaperSupply      ){m_iManStep = 10 ; m_iPreManStep = 0;}

    if(!bRet) Init();



    /********************/
    /* 처리..           */
    /********************/
    if      (m_iManNo == mcAllHome           ) { LDR.InitHomeStep ();
                                                 RAL.InitHomeStep ();
                                                 HED.InitHomeStep ();
                                                 PSB.InitHomeStep ();
                                                 ULD.InitHomeStep ();
                                                 ULM.InitHomeStep (); }

    else if (m_iManNo == mcLDR_Home          ) { LDR.InitHomeStep (); }
    else if (m_iManNo == mcLDR_Supply        ) { LDR.InitCycleStep(); }
    else if (m_iManNo == mcLDR_Work          ) { if(DM.ARAY[riLDR].CheckAllStat(csNone)) {DM.ARAY[riLDR].SetStat(csUnkwn); LDR.InitCycleStep(); }
                                                 else                                    {LDR.InitCycleStep();                                  } }
    else if (m_iManNo == mcLDR_Pick          ) { if(DM.ARAY[riLDR].CheckAllStat(csNone)) {DM.ARAY[riLDR].SetStat(csUnkwn); LDR.InitCycleStep(); }
                                                 else                                    {LDR.InitCycleStep();                                  } }
    else if (m_iManNo == mcLDR_Out           ) { LDR.InitCycleStep(); }
    else if (m_iManNo == mcLDR_WorkSttPs     ) { MT_GoAbsMan(miLDR_YFed , PM.GetValue(miLDR_YFed , pvLDR_YFedWorkStt )); }

    else if (m_iManNo == mcLDR_PusherUpDn    ) { if(AT_Complete(aiLDR_Pusher  , ccBwd)) { AT_MoveCyl(aiLDR_Pusher  , ccFwd); }
                                                 if(AT_Complete(aiLDR_Pusher  , ccFwd)) { AT_MoveCyl(aiLDR_Pusher  , ccBwd); }}
    else if (m_iManNo == mcLDR_RotateFwBw    ) { if(AT_Complete(aiLDR_Rotator , ccBwd)) { AT_MoveCyl(aiLDR_Rotator , ccFwd); }
                                                 if(AT_Complete(aiLDR_Rotator , ccFwd)) { AT_MoveCyl(aiLDR_Rotator , ccBwd); }}
    else if (m_iManNo == mcRAL_PreStprDn     ) { if(AT_Complete(aiPRB_Stopper , ccBwd)) { AT_MoveCyl(aiPRB_Stopper , ccFwd); }
                                                 if(AT_Complete(aiPRB_Stopper , ccFwd)) { AT_MoveCyl(aiPRB_Stopper , ccBwd); }}

    else if (m_iManNo == mcHED_Home          ) { HED.InitHomeStep (); }
    else if (m_iManNo == mcHED_InspMan       ) { if(CycleManBindInsp()) m_iManNo = mcNoneCycle;}
    else if (m_iManNo == mcHED_Wk1CmpFB      ) { if(AT_Complete(aiWK1_FlpCmp , ccBwd)) { AT_MoveCyl(aiWK1_FlpCmp , ccFwd); }
                                                 if(AT_Complete(aiWK1_FlpCmp , ccFwd)) { AT_MoveCyl(aiWK1_FlpCmp , ccBwd); }}
    else if (m_iManNo == mcHED_Wk1InsFB      ) { if(AT_Complete(aiWK1_Ins    , ccBwd)) { AT_MoveCyl(aiWK1_Ins    , ccFwd); }
                                                 if(AT_Complete(aiWK1_Ins    , ccFwd)) { AT_MoveCyl(aiWK1_Ins    , ccBwd); }}
    else if (m_iManNo == mcHED_Wk1AlgnFB     ) { if(AT_Complete(aiWK1_Align  , ccBwd)) { AT_MoveCyl(aiWK1_Align  , ccFwd); }
                                                 if(AT_Complete(aiWK1_Align  , ccFwd)) { AT_MoveCyl(aiWK1_Align  , ccBwd); }}
    else if (m_iManNo == mcHED_Wk1StrpFB     ) { if(AT_Complete(aiWK1_Stopper, ccFwd)) { AT_MoveCyl(aiWK1_Stopper, ccBwd); }
                                                 if(AT_Complete(aiWK1_Stopper, ccBwd)) { AT_MoveCyl(aiWK1_Stopper, ccFwd); }}
    else if (m_iManNo == mcHED_Wk1InversePs  ) { if(CycleManFlprNomalPs()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcHED_Wk1NormalPs   ) { if(CycleManFlprInversePs()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcHED_Wk2AlgnFB     ) { if(AT_Complete(aiWK2_Align  , ccBwd)) { AT_MoveCyl(aiWK2_Align  , ccFwd); }
                                                 if(AT_Complete(aiWK2_Align  , ccFwd)) { AT_MoveCyl(aiWK2_Align  , ccBwd); }}
    else if (m_iManNo == mcHED_Wk2StrpFB     ) { if(AT_Complete(aiWK2_Stopper, ccFwd)) { AT_MoveCyl(aiWK2_Stopper, ccBwd); }
                                                 if(AT_Complete(aiWK2_Stopper, ccBwd)) { AT_MoveCyl(aiWK2_Stopper, ccFwd); }}
    else if (m_iManNo == mcHED_Wk3AlgnFB     ) { if(AT_Complete(aiWK3_Align  , ccBwd)) { AT_MoveCyl(aiWK3_Align  , ccFwd); }
                                                 if(AT_Complete(aiWK3_Align  , ccFwd)) { AT_MoveCyl(aiWK3_Align  , ccBwd); }}
    else if (m_iManNo == mcHED_Wk3StrpFB     ) { if(AT_Complete(aiWK3_Stopper, ccFwd)) { AT_MoveCyl(aiWK3_Stopper, ccBwd); }
                                                 if(AT_Complete(aiWK3_Stopper, ccBwd)) { AT_MoveCyl(aiWK3_Stopper, ccFwd); }}
    else if (m_iManNo == mcHED_FeedingOnOff  ) { if( IO_GetY(yWK1_FeedingAC)) { IO_SetY(yWK1_FeedingAC , false);}
                                                 if(!IO_GetY(yWK1_FeedingAC)) { IO_SetY(yWK1_FeedingAC , true );}}

    else if (m_iManNo == mcPSB_Home          ) { PSB.InitHomeStep (); }
    else if (m_iManNo == mcPSB_InitPos       ) { if(CycleManPsbInitPos()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_MrkSttPs      ) { if(CycleManWorkSttPos()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_VisnSttPs     ) { if(CycleManVisnSttPos()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_AlgnFB        ) { if(AT_Complete(aiPSB_Align  , ccBwd)) { AT_MoveCyl(aiPSB_Align  , ccFwd); }
                                                 if(AT_Complete(aiPSB_Align  , ccFwd)) { AT_MoveCyl(aiPSB_Align  , ccBwd); }}
    else if (m_iManNo == mcPSB_StprUD        ) { if(AT_Complete(aiPSB_Stopper, ccFwd)) { AT_MoveCyl(aiPSB_Stopper, ccBwd); }
                                                 if(AT_Complete(aiPSB_Stopper, ccBwd)) { AT_MoveCyl(aiPSB_Stopper, ccFwd); }}
    else if (m_iManNo == mcPSB_MarkingUD     ) { if(AT_Complete(aiPSB_Marking, ccFwd)) { AT_MoveCyl(aiPSB_Marking, ccBwd); }
                                                 if(AT_Complete(aiPSB_Marking, ccBwd)) { AT_MoveCyl(aiPSB_Marking, ccFwd); }}
    else if (m_iManNo == mcPSB_MrkChangPs    ) { if(CycleManMrkChange()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_TestMarking   ) { if(CycleManTestMrk())   m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_FeedingOnOff  ) { if( IO_GetY(yPSB_FeedingAC)) { IO_SetY(yPSB_FeedingAC , false);}
                                                 if(!IO_GetY(yPSB_FeedingAC)) { IO_SetY(yPSB_FeedingAC , true );}}
    else if (m_iManNo == mcPSB_Remove        ) { PSB.InitCycleStep(); }
    else if (m_iManNo == mcPSB_ReviewNext    ) { PSB.InitCycleStep(); }

    else if (m_iManNo == mcULD_Home          ) { ULD.InitHomeStep (); }
    else if (m_iManNo == mcULD_StprFwPs      ) { MT_GoAbsMan(miULD_XStp , PM.GetValue(miULD_XStp , pvULD_XStpFwd)); }
    else if (m_iManNo == mcULD_StprBwPs      ) { MT_GoAbsMan(miULD_XStp , PM.GetValue(miULD_XStp , pvULD_XStpBwd)); }
    else if (m_iManNo == mcULD_VaccumOnOff   ) { if(IO_GetY(yULD_PickVaccum1)) {
                                                 IO_SetY(yULD_PickVaccum1 , false);
                                                 IO_SetY(yULD_PickVaccum2 , false);
                                                 }
                                                 else {
                                                 IO_SetY(yULD_PickVaccum1 , true );
                                                 IO_SetY(yULD_PickVaccum2 , true );
                                                 }
                                               }
    else if (m_iManNo == mcULD_EjectOnOff    ) { if(IO_GetY(yULD_PickEjectr1)) {
                                                 IO_SetY(yULD_PickEjectr1 , false);
                                                 IO_SetY(yULD_PickEjectr2 , false);
                                                 }
                                                 else {
                                                 IO_SetY(yULD_PickEjectr1 , true );
                                                 IO_SetY(yULD_PickEjectr2 , true );
                                                 }
                                               }
    else if (m_iManNo == mcULD_TrayMove      ) { ULD.InitCycleStep(); }
    else if (m_iManNo == mcULD_TrayOpen      ) { if(CycleManTrayOpen())   m_iManNo = mcNoneCycle;}
    else if (m_iManNo == mcULD_PaperSupply   ) { MT_GoAbsMan(miULD_ZPpr , PM.GetValue(miULD_ZPpr , pvULD_ZPprWait));}
    else if (m_iManNo == mcULD_PaperWorkStt  ) { MT_GoAbsMan(miULD_ZPpr , PM.GetValue(miULD_ZPpr , pvULD_ZPprWorkStt));}
    else if (m_iManNo == mcULD_PickPs        ) { if(CycleManULDPickPs     ()) m_iManNo = mcNoneCycle;}
    else if (m_iManNo == mcULD_FrstPlacePs   ) { if(CycleManULDFrstPlacePs()) m_iManNo = mcNoneCycle;}
    else if (m_iManNo == mcULD_PaperPs       ) { if(CycleManULDPaperPs    ()) m_iManNo = mcNoneCycle;}
    else if (m_iManNo == mcULD_Pick          ) { if(CycleManULDPick       ()) m_iManNo = mcNoneCycle;}
    else if (m_iManNo == mcULD_Place         ) { if(CycleManULDPlace      ()) m_iManNo = mcNoneCycle;}


    else if (m_iManNo == mcRAL_Home          ) { RAL.InitHomeStep(); }
    else if (m_iManNo == mcULM_Home          ) { ULM.InitHomeStep(); }
    else if (m_iManNo == mcETC_AllBeltOn     ) { IO_SetY(yPSB_FeedingAC , true); IO_SetY(yWK1_FeedingAC , true);
                                                 IO_SetY(yPRB_FeedingAC , true); IO_SetY(yULD_FeedingAC , true); }
    else if (m_iManNo == mcETC_AllBeltOff    ) { IO_SetY(yPSB_FeedingAC , false); IO_SetY(yWK1_FeedingAC , false);
                                                 IO_SetY(yPRB_FeedingAC , false); IO_SetY(yULD_FeedingAC , false); }
    else if (m_iManNo == mcHED_AllStprUp     ) { AT_MoveCyl(aiPRB_Stopper , ccFwd); AT_MoveCyl(aiWK1_Stopper , ccFwd); AT_MoveCyl(aiWK2_Stopper , ccFwd);
                                                 AT_MoveCyl(aiWK3_Stopper , ccFwd); AT_MoveCyl(aiPSB_Stopper , ccFwd); }
    else if (m_iManNo == mcETC_ALlStprDn     ) { AT_MoveCyl(aiPRB_Stopper , ccBwd); AT_MoveCyl(aiWK1_Stopper , ccBwd); AT_MoveCyl(aiWK2_Stopper , ccBwd);
                                                 AT_MoveCyl(aiWK3_Stopper , ccBwd); AT_MoveCyl(aiPSB_Stopper , ccBwd); }
    else if (m_iManNo == mcETC_MainAirOff    ) { if( IO_GetY(yETC_MainSol)) {IO_SetY(yETC_MainSol , false);}
                                                 if(!IO_GetY(yETC_MainSol)) {IO_SetY(yETC_MainSol , true );} }

    else if (m_iManNo == mcETC_ManLotEnd     ) { if(CycleManLotEnd()) m_iManNo = mcNoneCycle;}
    else if (m_iManNo == mcETC_InTrayStt     ) { ULM.InitCycleStep();}
    else if (m_iManNo == mcETC_OutTrayStt    ) { ULM.InitCycleStep();}
    else if (m_iManNo == mcETC_PaperStt      ) { ULM.InitCycleStep();}
    else if (m_iManNo == mcETC_PaperSupply   ) { if(CycleManULMPprSupply ()) m_iManNo = mcNoneCycle;}

    else                                       { m_iManNo = mcNoneCycle ; }

    m_bManSetting = false ; //m_bManSetting 중요함 리턴전에 꼭 펄스 시켜야함. 쓰레드가 달라서. ::Update에서 m_iManNo=0이 되므로 주의.
    return true ;
}



EN_MANUAL_CYCLE __fastcall CManualMan::GetManNo()
{
    return m_iManNo ;
}

//---------------------------------------------------------------------------
void __fastcall CManualMan::Update(void)
{
    if( m_iManNo == mcNoneCycle ) return ;
    if( m_bManSetting           ) return ;

    if(!OM.CmnOptn.bDoorSkip ) SEQ.InspectDoor    ();

    if(m_iManNo != mcLDR_Home &&
       m_iManNo != mcRAL_Home &&
       m_iManNo != mcHED_Home &&
       m_iManNo != mcPSB_Home &&
       m_iManNo != mcULD_Home &&
       m_iManNo != mcULM_Home &&
       m_iManNo != mcAllHome    ) SEQ.InspectHomeEnd ();
    SEQ.InspectLightGrid();

    bool r1,r2,r3,r4,r5,r6 ;

    bool isWRKXWait     = MT_CmprPos (miWRK_XVsn , PM.GetValue(miWRK_XVsn,pvWRK_XVsnWait)) && MT_GetStop(miWRK_XVsn) ;
    bool isWRKYWait     = MT_CmprPos (miWRK_YVsn , PM.GetValue(miWRK_YVsn,pvWRK_YVsnWait)) && MT_GetStop(miWRK_YVsn) ;
    bool isLTrayWait    = MT_CmprPos (miULD_ZTrI , PM.GetValue(miULD_ZTrI,pvULD_ZTrIWait)) && MT_GetStop(miULD_ZTrI) ;

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return ; }
    //Cycle Step.
    if      (m_iManNo == mcAllHome         )  { r1 = LDR.CycleHome();
                                                if(isWRKXWait && isWRKYWait) r2 = RAL.CycleHome();
                                                r3 = HED.CycleHome();
                                                r4 = PSB.CycleHome();
                                                r5 = ULM.CycleHome();
                                                if(isLTrayWait) r6 = ULD.CycleHome();
                                                    if(r1&&r2&&r3&&r4&&r5&&r6) {
                                                        m_iManNo = mcNoneCycle;
                                                        FM_MsgOk("Confirm","All Home Finished!");
                                                   }
                                               }

    else if (m_iManNo == mcLDR_Home            ) { if(LDR.CycleHome     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Supply          ) { if(LDR.CycleSupply   ()) m_iManNo = mcNoneCycle; }
    //else if (m_iManNo == mcLDR_Work           ) { if(LDR.CycleWork     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Pick            ) { if(LDR.CyclePick     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Out             ) { if(LDR.CycleOut      ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcHED_Home            ) { if(HED.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcHED_InspMan         ) { if(CycleManBindInsp      ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcHED_Wk1InversePs    ) { if(CycleManFlprInversePs ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcHED_Wk1NormalPs     ) { if(CycleManFlprNomalPs   ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcPSB_Home            ) { if(PSB.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_Remove          ) { if(PSB.CycleRemove       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_InitPos         ) { if(CycleManPsbInitPos    ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_MrkSttPs        ) { if(CycleManWorkSttPos    ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_VisnSttPs       ) { if(CycleManVisnSttPos    ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_MrkChangPs      ) { if(CycleManMrkChange     ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcPSB_TestMarking     ) { if(CycleManTestMrk       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_ReviewNext      ) { if(PSB.CycleVisnMM       ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcRAL_Home            ) { if(RAL.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Home            ) { if(ULD.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULM_Home            ) { if(ULM.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_TrayMove        ) { if(ULD.CycleTrayMove     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_TrayOpen        ) { if(CycleManTrayOpen      ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_PickPs          ) { if(CycleManULDPickPs     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_FrstPlacePs     ) { if(CycleManULDFrstPlacePs()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_PaperPs         ) { if(CycleManULDPaperPs    ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Pick            ) { if(CycleManULDPick       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Place           ) { if(CycleManULDPlace      ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcETC_ManLotEnd       ) { if(CycleManLotEnd        ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_InTrayStt       ) { if(ULM.CycleInTray       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_OutTrayStt      ) { if(ULM.CycleOutTray      ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_PaperStt        ) { if(ULM.CyclePaper        ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_PaperSupply     ) { if(CycleManULMPprSupply  ()) m_iManNo = mcNoneCycle; }

    else                                         {                              m_iManNo = mcNoneCycle; } //여기서 갱신됌.

    //Ok.
    return ;
}

//---------------------------------------------------------------------------
bool CManualMan::CycleManMrkChange()
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

        case  10:
                  if( !MT_CmprPos (miPSB_XMrk, 100) || !MT_CmprPos (miPSB_YMrk, PM.GetValue(miPSB_YMrk, pvPSB_YMrkWait)) ){
                      AT_MoveCyl(aiPSB_Marking, ccBwd);
                  } //교체 위치가 아니면 교체 위치로 ㄱ
                  else {
                      AT_MoveCyl(aiPSB_Marking, ccBwd);
                      m_iManStep = 13;
                      return false ;
                  }
                  m_iManStep ++;
                  return false;

        case  11: if(!AT_MoveCyl(aiPSB_Marking, ccBwd)) return false;
                  MT_GoAbsMan(miPSB_XMrk, 200);
                  MT_GoAbsMan(miPSB_YMrk, PM.GetValue(miPSB_YMrk, pvPSB_YMrkWait));
                  m_iManStep ++;
                  return false;

        case  12: if(!MT_GoAbsMan(miPSB_XMrk, 200)) return false ;
                  if(!MT_GoAbsMan(miPSB_YMrk, PM.GetValue(miPSB_YMrk, pvPSB_YMrkWait))) return false ;
                  m_iManStep = 0;
                  return true;

        case  13: if(!AT_MoveCyl(aiPSB_Marking, ccBwd)) return false;
                  MT_GoAbsMan(miPSB_XMrk, PM.GetValue(miPSB_XMrk, pvPSB_XMrkWait));
                  MT_GoAbsMan(miPSB_YMrk, PM.GetValue(miPSB_YMrk, pvPSB_YMrkWait));
                  m_iManStep ++;
                  return false;

        case  14: if(!MT_GoAbsMan(miPSB_XMrk, PM.GetValue(miPSB_XMrk, pvPSB_XMrkWait))) return false ;
                  if(!MT_GoAbsMan(miPSB_YMrk, PM.GetValue(miPSB_YMrk, pvPSB_YMrkWait))) return false ;
                  AT_MoveCyl(aiPSB_Marking, ccFwd);
                  m_iManStep ++;
                  return false;

        case  15: if(!AT_MoveCyl(aiPSB_Marking, ccFwd)) return false;
                  m_iManStep = 0;
                  return true;
        }
}

bool CManualMan::CycleManPsbInitPos()
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

        case 10 : //MoveActr(aiPSB_Align   , ccBwd              );
                  //MoveActr(aiPSB_Lift    , ccBwd              );
                  AT_MoveCyl(aiPSB_Marking , ccBwd              );
                  //MoveActr(aiPSB_Pusher  , ccBwd              );
                  //MoveActr(aiPSB_Stopper , ccFwd              );
//                  if(AT_Complete(aiPSB_Marking,ccBwd)
                  m_iManStep++;
                  return false ;

        case 11 : //if(!MoveActr(aiPSB_Align   , ccBwd              )) return false ;
                  //if(!MoveActr(aiPSB_Lift    , ccBwd              )) return false ;
                  if(!AT_MoveCyl(aiPSB_Marking , ccBwd              )) return false ;
                  //if(!MoveActr(aiPSB_Pusher  , ccBwd              )) return false ;
                  //if(!MoveActr(aiPSB_Stopper , ccFwd              )) return false ;
                  MT_GoAbsMan(miPSB_XMrk , PM.GetValue(miPSB_XMrk,pvPSB_XMrkWait)) ;
                  MT_GoAbsMan(miPSB_YMrk , PM.GetValue(miPSB_YMrk,pvPSB_YMrkWait)) ;
                  m_iManStep++;
                  return false ;

        case 12 : if(!MT_GoAbsMan(miPSB_XMrk , PM.GetValue(miPSB_XMrk,pvPSB_XMrkWait)))return false ;
                  if(!MT_GoAbsMan(miPSB_YMrk , PM.GetValue(miPSB_YMrk,pvPSB_YMrkWait)))return false ;
                  AT_MoveCyl(aiPSB_Marking  , ccFwd);
                  m_iManStep++;
                  return false ;

        case 13 : if(!AT_MoveCyl(aiPSB_Marking    , ccFwd)) return false ;
                  m_iManStep = 0;
                  return true ;
    }
}

bool CManualMan::CycleManWorkSttPos()
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

        case 10 : AT_MoveCyl(aiPSB_Marking , ccBwd              );
                  m_iManStep++;
                  return false ;

        case 11 : if(!AT_MoveCyl(aiPSB_Marking , ccBwd              )) return false ;
                  MT_GoAbsMan(miPSB_XMrk , PM.GetValue(miPSB_XMrk,pvPSB_XMrkWorkStart)) ;
                  MT_GoAbsMan(miPSB_YMrk , PM.GetValue(miPSB_YMrk,pvPSB_YMrkWorkStart)) ;
                  m_iManStep++;
                  return false ;

        case 12 : if(!MT_GoAbsMan(miPSB_XMrk , PM.GetValue(miPSB_XMrk,pvPSB_XMrkWorkStart)))return false ;
                  if(!MT_GoAbsMan(miPSB_YMrk , PM.GetValue(miPSB_YMrk,pvPSB_YMrkWorkStart)))return false ;
//                  AT_MoveCyl(aiPSB_Marking  , ccFwd);
//                  m_iManStep++;
//                  return false ;

//        case 13 : if(!AT_MoveCyl(aiPSB_Marking    , ccFwd)) return false ;
                  m_iManStep = 0;
                  return true ;
    }
}
bool CManualMan::CycleManVisnSttPos()
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

        case 10 : AT_MoveCyl(aiPSB_Marking , ccBwd              );
                  m_iManStep++;
                  return false ;

        case 11 : if(!AT_MoveCyl(aiPSB_Marking , ccBwd              )) return false ;
                  MT_GoAbsMan(miPSB_XMrk , PM.GetValue(miPSB_XMrk,pvPSB_XMrkVisnStart)) ;
                  MT_GoAbsMan(miPSB_YMrk , PM.GetValue(miPSB_YMrk,pvPSB_YMrkVisnStart)) ;
                  m_iManStep++;
                  return false ;

        case 12 : if(!MT_GoAbsMan(miPSB_XMrk , PM.GetValue(miPSB_XMrk,pvPSB_XMrkVisnStart)))return false ;
                  if(!MT_GoAbsMan(miPSB_YMrk , PM.GetValue(miPSB_YMrk,pvPSB_YMrkVisnStart)))return false ;
                  m_iManStep = 0;
                  return true ;
    }
}
bool CManualMan::CycleManTestMrk   ()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        //DM.ARAY[riPSB] = TempArray ;
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

        case  10: m_bStopTestMrk = false ;
                  //TempArray = DM.ARAY[riPSB] ;
                  if(!DM.ARAY[riPSB].GetCntStat(csRslt1)){
                      DM.ARAY[riPSB].SetStat(csRslt1);
                  }
                  AT_MoveCyl(aiPSB_Stopper,ccFwd);
                  AT_MoveCyl(aiPSB_Align  ,ccBwd);
                  m_iManStep++ ;
                  return false ;

        case  11: if(!AT_MoveCyl(aiPSB_Stopper,ccFwd)) return false ;
                  if(!AT_MoveCyl(aiPSB_Align  ,ccBwd)) return false ;
                  IO_SetY(yPSB_FeedingAC , true);
                  AT_MoveCyl(aiPSB_Align,ccFwd);
                  m_iManStep++ ;
                  return false ;

        case  12: if(!AT_MoveCyl(aiPSB_Align,ccFwd)) return false ;
                  IO_SetY(yPSB_FeedingAC , false);
                  m_iManStep++ ;
                  return false ;

        case  13: //DM.ARAY[riPSB].SetStat(csRslt0);
                  PSB.InitCycleStep();
                  m_iManStep++ ;
                  return false ;

        case  14: m_tmCycle.Clear();
                  if(!PSB.CycleWork()) return false ;
                  if( DM.ARAY[riPSB].GetCntStat(csRslt1) && !m_bStopTestMrk ){
                      m_iManStep = 13 ;
                      return false ;
                  }
                  m_iManStep++ ;
                  return false ;

        case  15:
                  m_iManStep++ ;
                  return false ;

        case  16: 
                  MT_GoAbsMan(miPSB_XMrk, PM.GetValue(miPSB_XMrk, pvPSB_XMrkWait));
                  MT_GoAbsMan(miPSB_YMrk, PM.GetValue(miPSB_YMrk, pvPSB_YMrkWait));
                  //DM.ARAY[riPSB] = TempArray ;

                  m_iManStep++;
                  return false;

        case  17: if(!MT_GoAbsMan(miPSB_XMrk, PM.GetValue(miPSB_XMrk, pvPSB_XMrkWait))) return false;
                  if(!MT_GoAbsMan(miPSB_YMrk, PM.GetValue(miPSB_YMrk, pvPSB_YMrkWait))) return false;
                  AT_MoveCyl(aiPSB_Marking, ccFwd);

                  m_iManStep = 0 ;
                  return true ;


    }
}


//---------------------------------------------------------------------------
bool CManualMan::CycleManBindInsp()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 20000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        DM.ARAY[riPSB] = DM.ARAY[riWB1] ;
        DM.ARAY[riWR2] = DM.ARAY[riWB2] ;
        DM.ARAY[riWR3] = DM.ARAY[riWB3] ;
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

        case  10: AT_MoveCyl(aiWK1_Stopper , ccFwd);
                  AT_MoveCyl(aiWK2_Stopper , ccFwd);
                  AT_MoveCyl(aiWK3_Stopper , ccFwd);
                  AT_MoveCyl(aiWK1_FlpCmp  , ccBwd);
                  DM.ARAY[riWB1] = DM.ARAY[riWR1] ;
                  DM.ARAY[riWB2] = DM.ARAY[riWR2] ;
                  DM.ARAY[riWB3] = DM.ARAY[riWR3] ;

                  m_iManStep++;
                  return false ;

        case  11: if(!AT_MoveCyl(aiWK1_Stopper , ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK2_Stopper , ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK3_Stopper , ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK1_FlpCmp  , ccBwd)) return false ;
                  IO_SetY(yWK1_FeedingAC , true);
                  m_tmTemp.Clear();
                  m_iManStep++;
                  return false ;

        case  12: if(!m_tmTemp.OnDelay(true , 300)) return false ;
                  //IO_SetY(yWR1_FeedingAC , false);
                  if(!IO_GetX(xWR1_Pkg) && !DM.ARAY[riWR1].GetCntExist() ) DM.ARAY[riWR1].SetStat(csUnkwn);
                  if(!IO_GetX(xWR2_Pkg) && !DM.ARAY[riWR2].GetCntExist() ) DM.ARAY[riWR2].SetStat(csUnkwn);
                  if(!IO_GetX(xWR3_Pkg) && !DM.ARAY[riWR2].GetCntExist() ) DM.ARAY[riWR3].SetStat(csUnkwn);
                  AT_MoveCyl(aiWK1_Align , ccFwd);
                  AT_MoveCyl(aiWK2_Align , ccFwd);
                  AT_MoveCyl(aiWK3_Align , ccFwd);

                  m_iManStep++ ;
                  return false ;

        case  13: if(!AT_MoveCyl(aiWK1_Align , ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK2_Align , ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK3_Align , ccFwd)) return false ;
                  //AT_MoveCyl(aiWK1_Lift , ccFwd);
                  AT_MoveCyl(aiWK1_FlpCmp , ccFwd);
                  m_iManStep++ ;
                  return false ;

        case  14: //if(!AT_MoveCyl(aiWK1_Lift , ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK1_FlpCmp , ccFwd)) return false ;
                  IO_SetY(yWK1_FeedingAC , false);
                  m_iManStep++ ;
                  return false ;

        case  15: //스테이지 올리고 바꿔야 바인딩 되고 HEAD에서 검사를 제때 시작 한다.
                  DM.ARAY[riVS1].SetStat(csUnkwn);
                  DM.ARAY[riVS2].SetStat(csUnkwn);
                  DM.ARAY[riVS3].SetStat(csUnkwn);
                  DM.ARAY[riWR1].SetStat(csUnkwn);
                  DM.ARAY[riWR2].SetStat(csUnkwn);
                  DM.ARAY[riWR3].SetStat(csUnkwn);
                  HED.InitCycleStep();
                  m_iManStep++ ;
                  return false ;

        case  16:  m_tmCycle.Clear();
                  if(!HED.CycleInsp()) return false ;
                  AT_MoveCyl(aiWK1_Stopper , ccFwd);
                  AT_MoveCyl(aiWK2_Stopper , ccFwd);
                  AT_MoveCyl(aiWK3_Stopper , ccFwd);
                  m_iManStep++ ;
                  return false ;

        case  17: if(!AT_MoveCyl(aiWK1_Stopper , ccFwd)) return false;
                  if(!AT_MoveCyl(aiWK2_Stopper , ccFwd)) return false;
                  if(!AT_MoveCyl(aiWK3_Stopper , ccFwd)) return false;
//                  AT_MoveCyl(aiWK1_Lift , ccBwd);
                  AT_MoveCyl(aiWK1_FlpCmp , ccBwd);
                  m_iManStep++ ;
                  return false ;

        case  18: //if(!AT_MoveCyl(aiWK1_Lift , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK1_FlpCmp , ccBwd)) return false;
                  AT_MoveCyl(aiWK1_Align , ccBwd);
                  AT_MoveCyl(aiWK2_Align , ccBwd);
                  AT_MoveCyl(aiWK3_Align , ccBwd);
                  m_iManStep++;
                  return false;

        case  19: if(!AT_MoveCyl(aiWK1_Align , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK2_Align , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK3_Align , ccBwd)) return false;

                  DM.ARAY[riWR1] = DM.ARAY[riWB1] ;
                  DM.ARAY[riWR2] = DM.ARAY[riWB2] ;
                  DM.ARAY[riWR3] = DM.ARAY[riWB3] ;
                  if(DM.ARAY[riVS1].GetCntExist())DM.ARAY[riVS1].SetStat(csUnkwn);
                  else                            DM.ARAY[riVS1].SetStat(csNone );
                  if(DM.ARAY[riVS2].GetCntExist())DM.ARAY[riVS2].SetStat(csUnkwn);
                  else                            DM.ARAY[riVS2].SetStat(csNone );
                  if(DM.ARAY[riVS3].GetCntExist())DM.ARAY[riVS3].SetStat(csUnkwn);
                  else                            DM.ARAY[riVS3].SetStat(csNone );
                  m_iManStep = 0 ;
                  return true ;
    }
}
//----------------------------------------------------------------
bool CManualMan::CycleManLotEnd()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 20000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        DM.ARAY[riPSB] = DM.ARAY[riWB1] ;
        DM.ARAY[riWR2] = DM.ARAY[riWB2] ;
        DM.ARAY[riWR3] = DM.ARAY[riWB3] ;
        m_iManStep = 0 ;
        return true;
    }

    if(m_iManStep != m_iPreManStep) {
        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
    }

    m_iPreManStep = m_iManStep ;

    //static bool bLDRExist = false ;
    //static bool bULDExist = false ;

    bool bRet1 , bRet2 ;

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: LDR.InitCycleStep();
                  m_iManStep++ ;
                  return false ;

        case  11: m_tmCycle.Clear();
                  bRet1 = LDR.CycleOut() ;
                  if(!bRet1) return false ;
                  m_iManStep = 0 ;
                  return true ;
    }
}
//--------------------------------------------------------------------
bool CManualMan::CycleManFlprNomalPs       ()
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

        case  10: AT_MoveCyl(aiWK1_FlpCmp, ccFwd);
                  AT_MoveCyl(aiWK1_Ins   , ccBwd);
                  if(AT_Complete(aiWK1_FlpCmp,ccFwd) && (IO_GetX(xWK1_ClampFw1) ||
                                                         IO_GetX(xWK1_ClampFw2) ||
                                                         IO_GetX(xWK1_ClampFw3) ||
                                                         IO_GetX(xWK1_ClampFw4) )){
                      EM_SetErr(eiWK1_FlpCmpTo ) ;
                      return false;
                  }

                  m_iManStep ++;
                  return false;

        case  11: if(!AT_MoveCyl(aiWK1_FlpCmp, ccFwd)) return false;
                  if(!AT_MoveCyl(aiWK1_Ins , ccBwd  )) return false;
                  MT_GoAbsMan(miWK1_YFlp, PM.GetValue(miWK1_YFlp, pvWK1_YFlpNormal));

                  m_iManStep ++;
                  return false;
        case  12: if(!MT_GoAbsMan(miWK1_YFlp, PM.GetValue(miWK1_YFlp, pvWK1_YFlpNormal))) return false;
                  AT_MoveCyl(aiWK1_FlpCmp, ccBwd);

                  m_iManStep = 0;
                  return true;
        }

}
//-----------------------------------------------------------------------
bool CManualMan::CycleManFlprInversePs ()
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

        case  10: AT_MoveCyl(aiWK1_FlpCmp, ccFwd);
                  AT_MoveCyl(aiWK1_Ins   , ccBwd);

                  m_iManStep ++;
                  return false;

        case  11: if(!AT_MoveCyl(aiWK1_FlpCmp, ccFwd)) return false;
                  if(!AT_MoveCyl(aiWK1_Ins   , ccBwd)) return false;
                  if(AT_Complete(aiWK1_FlpCmp,ccFwd) && (!IO_GetX(xWK1_ClampFw1) ||
                                                         !IO_GetX(xWK1_ClampFw2) ||
                                                         !IO_GetX(xWK1_ClampFw3) ||
                                                         !IO_GetX(xWK1_ClampFw4) )){
                      EM_SetErr(eiWK1_FlpCmpTo ) ;
                      return false;
                  }
                  MT_GoAbsMan(miWK1_YFlp, PM.GetValue(miWK1_YFlp, pvWK1_YFlpInverse));

                  m_iManStep ++;
                  return false;
        case  12: if(!MT_GoAbsMan(miWK1_YFlp, PM.GetValue(miWK1_YFlp, pvWK1_YFlpInverse))) return false;
                  AT_MoveCyl(aiWK1_FlpCmp, ccBwd);

                  m_iManStep = 0;
                  return true;
        }

}

bool CManualMan::CycleManTraySupply ()
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

        case  10: IO_SetY(yULD_PickVaccum1 , false);
                  IO_SetY(yULD_PickVaccum2 , false);
                  AT_MoveCyl(aiULD_TrayClamp , ccBwd);
                  m_iManStep++;
                  return false;

        case  11: if(!AT_MoveCyl(aiULD_TrayClamp , ccBwd)) return false;
                  AT_MoveCyl(aiULD_TrayClampPick , ccBwd);
                  m_iManStep++;
                  return false;

        case  12: if(!AT_MoveCyl(aiULD_TrayClampPick , ccBwd)) return false;
                  AT_MoveCyl(aiULD_TrayClampMove , ccBwd);
                  m_iManStep++;
                  return false;

        case  13: if(!AT_MoveCyl(aiULD_TrayClampMove , ccBwd)) return false;
                  MT_GoAbsMan(miULD_ZTrI , PM.GetValue(miULD_ZTrI , pvULD_ZTrIWait));
                  MT_GoAbsMan(miULD_ZTrO , PM.GetValue(miULD_ZTrO , pvULD_ZTrOWait));
                  m_iManStep++;
                  return false;

        case  14: if(!MT_GoAbsMan(miULD_ZTrI , PM.GetValue(miULD_ZTrI , pvULD_ZTrIWait))) return false;
                  if(!MT_GoAbsMan(miULD_ZTrO , PM.GetValue(miULD_ZTrO , pvULD_ZTrOWait))) return false;
                  AT_MoveCyl(aiULD_Hold , ccBwd);
                  m_iManStep++;
                  return false;

        case  15: if(!AT_MoveCyl(aiULD_Hold , ccBwd)) return false;
                  AT_MoveCyl(aiULD_TrayOpen , ccFwd) ;
                  m_iManStep++;
                  return false;

        case  16: if(!AT_MoveCyl(aiULD_TrayOpen , ccFwd)) return false;

                  m_iManStep = 0;
                  return false;
    }

}
bool CManualMan::CycleManTrayOpen   ()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 10000 )) {
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

        case  10: AT_MoveCyl(aiULD_TrayClampPick , ccBwd);
                  m_iManStep++;
                  return false;

        case  11: if(!AT_MoveCyl(aiULD_TrayClampPick , ccBwd)) return false;
                  AT_MoveCyl(aiULD_TrayClampMove , ccBwd);
                  m_iManStep++;
                  return false;

        case  12: if(!AT_MoveCyl(aiULD_TrayClampMove , ccBwd)) return false;
                  if(AT_Complete(aiULD_TrayClamp , ccFwd)){
                      AT_MoveCyl(aiULD_TrayClampPick , ccFwd);
                  }
                  m_iManStep++;
                  return false;

        case  13: if(AT_Complete(aiULD_TrayClamp , ccFwd) && !AT_MoveCyl(aiULD_TrayClampPick , ccFwd))return false;
                  AT_MoveCyl(aiULD_TrayClamp , ccBwd);
                  m_iManStep++;
                  return false;

        case  14: if(!AT_MoveCyl(aiULD_TrayClamp , ccBwd)) return false;
                  AT_MoveCyl(aiULD_TrayClampPick , ccBwd);
                  m_iManStep++;
                  return false ;

        case  15: if(!AT_MoveCyl(aiULD_TrayClampPick , ccBwd)) return false;
                  MT_GoAbsMan(miULD_ZTrI , PM.GetValue(miULD_ZTrI , pvULD_ZTrIWait));
                  MT_GoAbsMan(miULD_ZTrO , PM.GetValue(miULD_ZTrO , pvULD_ZTrOWait));
                  m_iManStep++;
                  return false;

        case  16: if(!MT_GoAbsMan(miULD_ZTrI , PM.GetValue(miULD_ZTrI , pvULD_ZTrIWait))) return false;
                  if(!MT_GoAbsMan(miULD_ZTrO , PM.GetValue(miULD_ZTrO , pvULD_ZTrOWait))) return false;
                  AT_MoveCyl(aiULD_Hold , ccBwd);
                  m_iManStep++;
                  return false;

        case  17: if(!AT_MoveCyl(aiULD_Hold , ccBwd)) return false;
                  AT_MoveCyl(aiULD_TrayOpen , ccFwd);
                  m_iManStep++;
                  return false;

        case  18: if(!AT_MoveCyl(aiULD_TrayOpen , ccFwd)) return false;
                  AT_MoveCyl(aiULD_TrayOpen , ccBwd);
                  m_iManStep++;
                  return false;

        case  19: if(!AT_MoveCyl(aiULD_TrayOpen , ccBwd)) return false;
                  m_iManStep = 0;
                  return false;
    }
}

bool CManualMan::CycleManULDPickPs     ()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 10000 )) {
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

        case  10: ULD.MoveMotr(miULD_ZPck , piULD_ZPckMove);
                  m_iManStep++;
                  return false;

        case  11: if(!ULD.MoveMotr(miULD_ZPck , piULD_ZPckMove)) return false;
                  ULD.MoveMotr(miULD_YPck , piULD_YPckPick);
                  m_iManStep++;
                  return false;

        case  12: if(!ULD.MoveMotr(miULD_YPck , piULD_YPckPick)) return false;
                  m_iManStep = 0;
                  return true;

    }
}

bool CManualMan::CycleManULDFrstPlacePs()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 10000 )) {
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

        case  10: ULD.MoveMotr(miULD_ZPck , piULD_ZPckMove);
                  m_iManStep++;
                  return false;

        case  11: if(!ULD.MoveMotr(miULD_ZPck , piULD_ZPckMove)) return false;
                  ULD.MoveMotr(miULD_YPck , piULD_YPckPlaceStt);
                  m_iManStep++;
                  return false;

        case  12: if(!ULD.MoveMotr(miULD_YPck , piULD_YPckPlaceStt)) return false;
                  m_iManStep = 0;
                  return true;
    }
}

bool CManualMan::CycleManULDPaperPs    ()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 10000 )) {
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

        case  10: ULD.MoveMotr(miULD_ZPck , piULD_ZPckMove);
                  m_iManStep++;
                  return false;

        case  11: if(!ULD.MoveMotr(miULD_ZPck , piULD_ZPckMove)) return false;
                  ULD.MoveMotr(miULD_YPck , piULD_YPckPaper);
                  m_iManStep++;
                  return false;

        case  12: if(!ULD.MoveMotr(miULD_YPck , piULD_YPckPaper)) return false;
                  m_iManStep = 0;
                  return true;
    }
}
bool CManualMan::CycleManULDPick()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 10000 )) {
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

        case  10: IO_SetY(yULD_PickVaccum1 , true);
                  IO_SetY(yULD_PickVaccum2 , true);
                  m_tmTemp.Clear();
                  m_iManStep++;
                  return false;

        case  11: if(!m_tmTemp.OnDelay(true , 500)) return false;
                  ULD.MoveMotr(miULD_ZPck , piULD_ZPckPick);
                  m_tmTemp.Clear();
                  m_iManStep++;
                  return false;

        case  12: if(!ULD.MoveMotr(miULD_ZPck , piULD_ZPckPick)) return false;
                  if(!m_tmTemp.OnDelay(true , OM.CmnOptn.dULDVaccumDeley)) return false;
                  ULD.MoveMotr(miULD_ZPck , piULD_ZPckMove);
                  m_iManStep++;
                  return false;

        case  13: if(!ULD.MoveMotr(miULD_ZPck , piULD_ZPckMove)) return false;
                  m_iManStep = 0;
                  return true;
    }
}

bool CManualMan::CycleManULDPlace()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 10000 )) {
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

        case  10: ULD.MoveMotr(miULD_ZPck , piULD_ZPckPlace);
                  m_tmTemp.Clear();
                  m_iManStep++;
                  return false;

        case  11: if(!ULD.MoveMotr(miULD_ZPck , piULD_ZPckPlace)) return false;
                  IO_SetY(yULD_PickVaccum1 , false);
                  IO_SetY(yULD_PickVaccum2 , false);

                  IO_SetY(yULD_PickEjectr1 , true);
                  IO_SetY(yULD_PickEjectr2 , true);
                  if(!m_tmTemp.OnDelay(true , 300)) return false;
                  m_iManStep++;
                  return false;

        case  12: ULD.MoveMotr(miULD_ZPck , piULD_ZPckMove);
                  m_iManStep++;
                  return false;

        case  13: if(!ULD.MoveMotr(miULD_ZPck , piULD_ZPckMove)) return false;
                  IO_SetY(yULD_PickEjectr1 , false);
                  IO_SetY(yULD_PickEjectr2 , false);
                  m_iManStep = 0;
                  return true;

    }
}

bool CManualMan::CycleManULMPprSupply()
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

        case  10: ULD.MoveActr(aiULD_PprRotator , ccBwd);
                  m_iManStep++;
                  return false ;

        case  11: if(!ULD.MoveActr(aiULD_PprRotator , ccBwd)) return false;
                  ULM.MoveMotr(miULD_ZPpr , piULD_ZPprWait);
                  m_iManStep ++;
                  return false;

        case  12: if(!ULM.MoveMotr(miULD_ZPpr , piULD_ZPprWait))return false;
                  m_iManStep = 0;
                  return true;
    }
}
