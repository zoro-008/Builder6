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
#include "FrmDeviceSet.h"
#include "FrmMain.h"
#include "FrmManual.h"
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

//    m_bManSetting = false ;

    m_tmCycle.Clear() ;
}

//---------------------------------------------------------------------------
bool __fastcall CManualMan::SetManCycle(EN_MANUAL_CYCLE _iNo)
{
    if( _iNo     <  0                ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( _iNo     >= MAX_MANUAL_CYCLE ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( m_iManNo != mcNoneCycle      ) {FM_MsgOk("ERROR","Doing Manual Cycle"   ); return false ;}
    if(SEQ._bRun                     ) {FM_MsgOk("ERROR","Autorunning"          ); return false ;}
    Trace("뻑","2");
    //Check Alar
    if ( EM_IsErr()      ) { Init(); return false  ; } //아 밑에 처리 하는 애 때문에 잠시 이렇게 함.

    if(_iNo  == mcAllHome  && ( IO_GetX(xPRB_PkgIn1 ) ||  IO_GetX(xPSB_PkgOut3) || IO_GetX(xPRB_PkgOut2))) {FM_MsgOk("ERROR","Check Rail In or Out Sensor!"); return false ;}
    if(_iNo  == mcAllHome  && (!IO_GetX(xETC_MainSol) || !IO_GetY(yETC_MainSol))) {FM_MsgOk("ERROR","Check Main Air!"); return false ;}
    if(_iNo  == mcLDR_Home && ( IO_GetX(xPRB_PkgIn1 )                          )) {FM_MsgOk("ERROR","Check Rail In  Sensor!" ); return false ;}
    if(_iNo  == mcULD_Home && ( IO_GetX(xPSB_PkgOut3)                          )) {FM_MsgOk("ERROR","Check Rail Out Sensor!" ); return false ;}

    Trace("뻑","3");
    EM_SetDisp(true);

    m_bManSetting = true ;
    m_iManNo = _iNo ;

    double VsnMaxColPich = OM.DevInfo.dColPitch * (OM.DevInfo.iColCnt - 2) ;
    double VsnMaxRowPich = OM.DevInfo.dRowPitch * (OM.DevInfo.iRowCnt - 2) ;

    double MaxColPich = OM.DevInfo.dColPitch * (OM.DevInfo.iColCnt - 1 );
    double MaxRowPich = OM.DevInfo.dRowPitch * (OM.DevInfo.iRowCnt - 1 );

    bool bRet = true ;

    /********************/
    /* 프리프로세서     */
    /********************/
         if (m_iManNo == mcAllHome            ){     if(!IO_GetX(xETC_PowerOn )) {FM_MsgOk("ERR","Power On Plz"); bRet = false ;}
                                                else if(!IO_GetY(xETC_LMainAir)) {FM_MsgOk("ERR","Air On Plz"  ); bRet = false ;}
                                                else if(!IO_GetY(xETC_RMainAir)) {FM_MsgOk("ERR","Air On Plz"  ); bRet = false ;}
                                                }


    else if (m_iManNo == mcLDR_Home            ){}
    else if (m_iManNo == mcLDR_UnClamp         ){
        if(IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Sure ?") != mrYes ) bRet = false;
    }
    else if (m_iManNo == mcLDR_Clamp           ){}
    else if (m_iManNo == mcLDR_PusherFw        ){if(!LDR.CheckSafe(aiLDR_Pusher,ccFwd)        ){ bRet = false ;} }
    else if (m_iManNo == mcLDR_PusherBw        ){}
    else if (m_iManNo == mcLDR_TransferInAC    ){}
    else if (m_iManNo == mcLDR_TransferOutAC   ){}
    else if (m_iManNo == mcLDR_Supply          ){}
    else if (m_iManNo == mcLDR_Pick            ){if(!DM.ARAY[riLDR].CheckAllStat(csNone)){FM_MsgOk("ERR","Remove Mgz"); bRet = false ;} }
    else if (m_iManNo == mcLDR_Place           ){
        if(!DM.ARAY[riLDR].CheckAllStat(csNone)){
                 if(LDR._iMgzCnt == 1){ PushTopLotNo(LT.GetCrntLot()); LDR._iMgzCnt = 0 ; }
            else if(LDR._iMgzCnt >  1){                                LDR._iMgzCnt--;    }
        }
    }





    else if (m_iManNo == mcLDR_Z1PichDn        ){if(!LDR.CheckSafe(miLDR_ZCmp,piLDR_ZWork)     ){ bRet = false ;} }
    else if (m_iManNo == mcLDR_Z1PichUp        ){if(!LDR.CheckSafe(miLDR_ZCmp,piLDR_ZWork)     ){ bRet = false ;} }
    else if (m_iManNo == mcLDR_SttPstn         ){if(!LDR.CheckSafe(miLDR_ZCmp,piLDR_ZWorkStart)){ bRet = false ;} }

    //Rail 부분
    else if (m_iManNo == mcRAL_Wk1Home         ){}
    else if (m_iManNo == mcRAL_PreStprUp       ){}//if(!RAL.CheckSafe(aiPRB_Stopper,ccFwd                )){ bRet = false ;}}
    else if (m_iManNo == mcRAL_PreStprDn       ){}//if(!RAL.CheckSafe(aiPRB_Stopper,ccBwd                )){ bRet = false ;}}
    else if (m_iManNo == mcRAL_Wk1CmpUp        ){}//if(!RAL.CheckSafe(aiWK1_FlpCmp,ccBwd                 )){ bRet = false ;}}
    else if (m_iManNo == mcRAL_Wk1CmpDn        ){}//if(!RAL.CheckSafe(aiWK1_FlpCmp,ccFwd                 )){ bRet = false ;}}
    else if (m_iManNo == mcRAL_Wk1StprUp       ){}//if(!RAL.CheckSafe(aiWK1_Stopper,ccFwd                )){ bRet = false ;}}
    else if (m_iManNo == mcRAL_Wk1StprDn       ){}//if(!RAL.CheckSafe(aiWK1_Stopper,ccBwd                )){ bRet = false ;}}
    else if (m_iManNo == mcRAL_Wk1AlignFB      ){}//if(!RAL.CheckSafe(aiWK1_Align,!AT_GetCmd(aiWK1_Align))){ bRet = false ;}}
    else if (m_iManNo == mcRAL_Wk1NomalPos     ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcRAL_Wk1ReversePos   ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcRAL_Wk2LiftUp       ){}//if(!RAL.CheckSafe(aiWK2_Lift,ccFwd                   )){ bRet = false ;}}
    else if (m_iManNo == mcRAL_Wk2LiftDn       ){}//if(!RAL.CheckSafe(aiWK2_Lift,ccBwd                   )){ bRet = false ;}}
    else if (m_iManNo == mcRAL_Wk2StprUp       ){}//if(!RAL.CheckSafe(aiWK2_Stopper,ccFwd                )){ bRet = false ;}}
    else if (m_iManNo == mcRAL_Wk2StprDn       ){}//if(!RAL.CheckSafe(aiWK2_Stopper,ccBwd                )){ bRet = false ;}}
    else if (m_iManNo == mcRAL_Wk2AlignFB      ){}//if(!RAL.CheckSafe(aiWK2_Align,!AT_GetCmd(aiWK2_Align))){ bRet = false ;}}
    else if (m_iManNo == mcRAL_Wk3LiftUp       ){}//if(!RAL.CheckSafe(aiWK3_Lift,ccFwd                   )){ bRet = false ;}}
    else if (m_iManNo == mcRAL_Wk3LiftDn       ){}//if(!RAL.CheckSafe(aiWK3_Lift,ccBwd                   )){ bRet = false ;}}
    else if (m_iManNo == mcRAL_Wk3StprUp       ){}//if(!RAL.CheckSafe(aiWK3_Stopper,ccFwd                )){ bRet = false ;}}
    else if (m_iManNo == mcRAL_Wk3StprDn       ){}//if(!RAL.CheckSafe(aiWK3_Stopper,ccBwd                )){ bRet = false ;}}
    else if (m_iManNo == mcRAL_Wk3AlignFB      ){}//if(!RAL.CheckSafe(aiWK3_Align,!AT_GetCmd(aiWK3_Align))){ bRet = false ;}}

    //PostBuff 부분
    else if (m_iManNo == mcPSB_Home            ){}
    else if (m_iManNo == mcPSB_InitPos         ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcPSB_WorkStPos       ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcPSB_LiftUp          ){}
    else if (m_iManNo == mcPSB_LiftDn          ){}
    else if (m_iManNo == mcPSB_StprUp          ){}
    else if (m_iManNo == mcPSB_StprDn          ){}
    else if (m_iManNo == mcPSB_MarkCmpUp       ){}
    else if (m_iManNo == mcPSB_MarkCmpDn       ){}
    else if (m_iManNo == mcPSB_UD1Cycle        ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcPSB_MarkingChange   ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcPSB_AlignFB         ){}
    else if (m_iManNo == mcPSB_TestMarking     ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcPSB_PusherFw        ){}
    else if (m_iManNo == mcPSB_PusherBw        ){}
    else if (m_iManNo == mcPSB_MrkVisnInsp     ){m_iManStep = 10 ; m_iPreManStep = 0;}

    //UnLoader 부분
    else if (m_iManNo == mcULD_Home            ){}
    else if (m_iManNo == mcULD_UnClamp         ){
        if(IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Sure ?") != mrYes ) bRet = false;
    }
    else if (m_iManNo == mcULD_Clamp           ){}
    else if (m_iManNo == mcULD_TransferInAC    ){}
    else if (m_iManNo == mcULD_TransferOutAC   ){}
    else if (m_iManNo == mcULD_Supply          ){}
    else if (m_iManNo == mcULD_Pick            ){if(!DM.ARAY[riULD].CheckAllStat(csNone)){FM_MsgOk("ERR","Remove Mgz"); bRet = false ;} }
    else if (m_iManNo == mcULD_Place           ){}
    else if (m_iManNo == mcULD_Z1PichUp        ){}
    else if (m_iManNo == mcULD_Z1PichDn        ){}
    else if (m_iManNo == mcULD_SttPstn         ){}

    //ETC 부분
    else if (m_iManNo == mcETC_RemoveIndex      ){}
    else if (m_iManNo == mcETC_FlipHome         ){
        bool isWRKXWait     = MT_CmprPos (miWRK_XVsn , PM.GetValue(miWRK_XVsn,pvWRK_XVsnWaitPs)) && MT_GetStop(miWRK_XVsn);
        bool isWRKYWait     = MT_CmprPos (miWRK_YVsn , PM.GetValue(miWRK_YVsn,pvWRK_YVsnWaitPs)) && MT_GetStop(miWRK_YVsn);
            if(!isWRKXWait && !isWRKYWait) {
            FM_MsgOk("ERR", "Vision X, Y Motor Position is not Wait Position"); bRet = false; }
        RAL.InitHomeStep();
        }
    else if (m_iManNo == mcHED_InspMan          ){m_iManStep = 10; m_iPreManStep = 0; }
    else if (m_iManNo == mcETC_Sampling1Cycle   ){}
    else if (m_iManNo == mcETC_AllBeltOn        ){}
    else if (m_iManNo == mcETC_AllBeltOff       ){}
    else if (m_iManNo == mcETC_AllStprUp        ){}
    else if (m_iManNo == mcETC_ALlStprDn        ){}
    else if (m_iManNo == mcETC_Wk1AlignLiftUp   ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcETC_Wk2AlignLiftUp   ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcETC_Wk3AlignLiftUp   ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcETC_MrkAlignLiftUp   ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcETC_AllAlignLiftUp   ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcETC_AllAlignLiftDn   ){}
    else if (m_iManNo == mcETC_TransferUp       ){}
    else if (m_iManNo == mcETC_TransferDn       ){}
    else if (m_iManNo == mcETC_MainAirOnOff     ){}
    else if (m_iManNo == mcETC_ManLotEnd        ){m_iManStep = 10 ; m_iPreManStep = 0;}

    if(!bRet) Init();

    Trace("뻑","4");



    /********************/
    /* 처리..           */
    /********************/
    if      (m_iManNo == mcAllHome            ) { //IO_SetY(yETC_MainSol ,true); 이러면 빠져나가는 상황이 있어서 그냥 윗단에서
                                                  //IO_SetY(yETC_MainSol2,true); 에러를 잡는다.
                                                  IO_SetY(yETC_LInitLp, !IO_GetY(yETC_LInitLp));
                                                  IO_SetY(yETC_RInitLp, !IO_GetY(yETC_RInitLp));

                                                  LDR.InitHomeStep();
                                                  RAL.InitHomeStep();
                                                  HED.InitHomeStep();
                                                  PSB.InitHomeStep();
                                                  ULD.InitHomeStep();}


    //Loader 부분
    else if (m_iManNo == mcLDR_Home            ) { LDR.InitHomeStep(); }
    else if (m_iManNo == mcLDR_UnClamp         ) { AT_MoveCyl(aiLDR_Cmp    , ccFwd); }
    else if (m_iManNo == mcLDR_Clamp           ) { AT_MoveCyl(aiLDR_Cmp    , ccBwd); }
    else if (m_iManNo == mcLDR_PusherFw        ) { AT_MoveCyl(aiLDR_Pusher , ccFwd); }
    else if (m_iManNo == mcLDR_PusherBw        ) { AT_MoveCyl(aiLDR_Pusher , ccBwd); }
    else if (m_iManNo == mcLDR_TransferInAC    ) { IO_SetY(yLDR_MgzInAC , !IO_GetY(yLDR_MgzInAC )); }
    else if (m_iManNo == mcLDR_TransferOutAC   ) { IO_SetY(yLDR_MgzOutAC, !IO_GetY(yLDR_MgzOutAC)); }
    else if (m_iManNo == mcLDR_Supply          ) { LDR.InitCycleStep(); }
    else if (m_iManNo == mcLDR_Pick            ) { LDR.InitCycleStep(); }
    else if (m_iManNo == mcLDR_Place           ) { LDR.InitCycleStep(); }
    else if (m_iManNo == mcLDR_Z1PichDn        ) { MT_GoIncMan(miLDR_ZCmp ,-OM.DevInfo.dCsSlPitch);}
    else if (m_iManNo == mcLDR_Z1PichUp        ) { MT_GoIncMan(miLDR_ZCmp , OM.DevInfo.dCsSlPitch);}
    else if (m_iManNo == mcLDR_SttPstn         ) { MT_GoAbsMan(miLDR_ZCmp,PM.GetValue(miLDR_ZCmp , pvLDR_ZWorkStartPs) ); }
    else if (m_iManNo == mcRAL_PreStprUp       ) { AT_MoveCyl(aiPRB_Stopper, ccFwd); }
    else if (m_iManNo == mcRAL_PreStprDn       ) { AT_MoveCyl(aiPRB_Stopper, ccBwd); }

    //Rail 부분
    else if (m_iManNo == mcRAL_Wk1Home         ) { HED.InitHomeStep(); }
    else if (m_iManNo == mcRAL_Wk1CmpUp        ) { AT_MoveCyl(aiWK1_FlpCmp , ccFwd); }
    else if (m_iManNo == mcRAL_Wk1CmpDn        ) { AT_MoveCyl(aiWK1_FlpCmp , ccBwd); }
    else if (m_iManNo == mcRAL_Wk1StprUp       ) { AT_MoveCyl(aiWK1_Stopper, ccFwd); }
    else if (m_iManNo == mcRAL_Wk1StprDn       ) { AT_MoveCyl(aiWK1_Stopper, ccBwd); }
    else if (m_iManNo == mcRAL_Wk1AlignFB      ) { if(AT_MoveCyl(aiWK1_Align , ccFwd)) { AT_MoveCyl(aiWK1_Align , ccBwd); }
                                                   if(AT_MoveCyl(aiWK1_Align , ccBwd)) { AT_MoveCyl(aiWK1_Align , ccFwd); } }
    else if (m_iManNo == mcRAL_Wk1NomalPos     ) { if(CycleManFlprNomalPs()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcRAL_Wk1ReversePos   ) { if(CycleManFlprInversePs()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcRAL_Wk2LiftUp       ) { AT_MoveCyl(aiWK2_Lift   , ccFwd); }
    else if (m_iManNo == mcRAL_Wk2LiftDn       ) { AT_MoveCyl(aiWK2_Lift   , ccBwd); }
    else if (m_iManNo == mcRAL_Wk2StprUp       ) { AT_MoveCyl(aiWK2_Stopper, ccFwd); }
    else if (m_iManNo == mcRAL_Wk2StprDn       ) { AT_MoveCyl(aiWK2_Stopper, ccBwd); }
    else if (m_iManNo == mcRAL_Wk2AlignFB      ) { if(AT_MoveCyl(aiWK2_Align , ccFwd)) { AT_MoveCyl(aiWK2_Align , ccBwd); }
                                                   if(AT_MoveCyl(aiWK2_Align , ccBwd)) { AT_MoveCyl(aiWK2_Align , ccFwd); } }
    else if (m_iManNo == mcRAL_Wk3LiftUp       ) { AT_MoveCyl(aiWK3_Lift   , ccFwd); }
    else if (m_iManNo == mcRAL_Wk3LiftDn       ) { AT_MoveCyl(aiWK3_Lift   , ccBwd); }
    else if (m_iManNo == mcRAL_Wk3StprUp       ) { AT_MoveCyl(aiWK3_Stopper, ccFwd); }
    else if (m_iManNo == mcRAL_Wk3StprDn       ) { AT_MoveCyl(aiWK3_Stopper, ccBwd); }
    else if (m_iManNo == mcRAL_Wk3AlignFB      ) { if(AT_MoveCyl(aiWK3_Align , ccFwd)) { AT_MoveCyl(aiWK3_Align , ccBwd); }
                                                   if(AT_MoveCyl(aiWK3_Align , ccBwd)) { AT_MoveCyl(aiWK3_Align , ccFwd); } }

    //PostBuff 부분
    else if (m_iManNo == mcPSB_Home            ) { PSB.InitHomeStep (); }
    else if (m_iManNo == mcPSB_InitPos         ) { if(CycleManPsbInitPos()) m_iManNo = mcNoneCycle; }  //아직 안함.dd
    else if (m_iManNo == mcPSB_WorkStPos       ) { if(CycleManWorkSttPos()) m_iManNo = mcNoneCycle; }  //아직 안함.dd

    else if (m_iManNo == mcPSB_LiftUp          ) { AT_MoveCyl(aiPSB_Lift   , ccFwd); }
    else if (m_iManNo == mcPSB_LiftDn          ) { AT_MoveCyl(aiPSB_Lift   , ccBwd); }
    else if (m_iManNo == mcPSB_StprUp          ) { AT_MoveCyl(aiPSB_Stopper, ccFwd); }
    else if (m_iManNo == mcPSB_StprDn          ) { AT_MoveCyl(aiPSB_Stopper, ccBwd); }
    else if (m_iManNo == mcPSB_MarkCmpUp       ) { AT_MoveCyl(aiPSB_Marking, ccBwd); }
    else if (m_iManNo == mcPSB_MarkCmpDn       ) { AT_MoveCyl(aiPSB_Marking, ccFwd); }
    else if (m_iManNo == mcPSB_UD1Cycle        ) { PSB.InitCycleStep(); }
    else if (m_iManNo == mcPSB_MarkingChange   ) { if(CycleManMrkChange()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_AlignFB         ) { if(AT_MoveCyl(aiPSB_Align , ccFwd)) { AT_MoveCyl(aiPSB_Align , ccBwd); }
                                                   if(AT_MoveCyl(aiPSB_Align , ccBwd)) { AT_MoveCyl(aiPSB_Align , ccFwd); } }
    else if (m_iManNo == mcPSB_TestMarking     ) { if(CycleManTestMrk())  m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_PusherFw        ) { AT_MoveCyl(aiPSB_Pusher, ccFwd); }
    else if (m_iManNo == mcPSB_PusherBw        ) { AT_MoveCyl(aiPSB_Pusher, ccBwd); }
    else if (m_iManNo == mcPSB_MrkVisnInsp     ) {if(CycleManMrkBindInsp()) m_iManNo = mcNoneCycle; }

    //UnLoader 부분
    else if (m_iManNo == mcULD_Home            ) { ULD.InitHomeStep(); }
    else if (m_iManNo == mcULD_UnClamp         ) { AT_MoveCyl(aiULD_Cmp, ccFwd); }
    else if (m_iManNo == mcULD_Clamp           ) { AT_MoveCyl(aiULD_Cmp, ccBwd); }
    else if (m_iManNo == mcULD_TransferInAC    ) { IO_SetY(yULD_MgzInAC , !IO_GetY(yULD_MgzInAC ) ); }
    else if (m_iManNo == mcULD_TransferOutAC   ) { IO_SetY(yULD_MgzOutAC, !IO_GetY(yULD_MgzOutAC) ); }
    else if (m_iManNo == mcULD_Supply          ) { ULD.InitCycleStep(); }
    else if (m_iManNo == mcULD_Pick            ) { ULD.InitCycleStep(); }
    else if (m_iManNo == mcULD_Place           ) { ULD.InitCycleStep(); }
    else if (m_iManNo == mcULD_Z1PichUp        ) { MT_GoIncMan(miULD_ZCmp ,-OM.DevInfo.dCsSlPitch); }
    else if (m_iManNo == mcULD_Z1PichDn        ) { MT_GoIncMan(miULD_ZCmp , OM.DevInfo.dCsSlPitch); }
    else if (m_iManNo == mcULD_SttPstn         ) { MT_GoAbsMan(miULD_YCmp,PM.GetValue(miULD_ZCmp , pvULD_ZWorkStartPs) ); }

    //Vision Switch
    else if (m_iManNo == mcHED_InspMan         ) {if(CycleManBindInsp()) m_iManNo = mcNoneCycle; }
    //ETC

    else if (m_iManNo == mcETC_FlipHome        ) {RAL.InitHomeStep(); }
    //else if (m_iManNo == mcETC_IndexRail1Cycle  ) {}               ??모르겠음
    //else if (m_iManNo == mcETC_FirstVision1Cycle) {                ??모르겠음
    //else if (m_iManNo == mcETC_MrkVision1Cycle   ) {                ??모르겠음
    else if (m_iManNo == mcETC_AllBeltOn        ) {IO_SetY(yPRB_FeedingAC,  true); IO_SetY(yWR1_FeedingAC,  true); IO_SetY(yWR2_FeedingAC,  true);}
    else if (m_iManNo == mcETC_AllBeltOff       ) {IO_SetY(yPRB_FeedingAC, false); IO_SetY(yWR1_FeedingAC, false); IO_SetY(yWR2_FeedingAC, false);}
    else if (m_iManNo == mcETC_AllStprUp        ) {AT_MoveCyl(aiPRB_Stopper, ccFwd); AT_MoveCyl(aiWK1_Stopper, ccFwd); AT_MoveCyl(aiWK2_Stopper, ccFwd);
                                                   AT_MoveCyl(aiWK3_Stopper, ccFwd); AT_MoveCyl(aiPSB_Stopper, ccFwd); }
    else if (m_iManNo == mcETC_ALlStprDn        ) {AT_MoveCyl(aiPRB_Stopper, ccBwd); AT_MoveCyl(aiWK1_Stopper, ccBwd); AT_MoveCyl(aiWK2_Stopper, ccBwd);
                                                   AT_MoveCyl(aiWK3_Stopper, ccBwd); AT_MoveCyl(aiPSB_Stopper, ccBwd); }
    else if (m_iManNo == mcETC_Wk1AlignLiftUp   ) {if(CycleMan1ndLift()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_Wk2AlignLiftUp   ) {if(CycleMan2ndLift()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_Wk3AlignLiftUp   ) {if(CycleMan3ndLift()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_MrkAlignLiftUp   ) {if(CycleManMrkLift()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_AllAlignLiftUp   ) {if(CycleManAllLift()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_AllAlignLiftDn   ) {AT_MoveCyl(aiWK1_FlpCmp , ccBwd); AT_MoveCyl(aiWK1_Align  , ccBwd);
                                                   AT_MoveCyl(aiWK2_Lift   , ccBwd); AT_MoveCyl(aiWK2_Align  , ccBwd);
                                                   AT_MoveCyl(aiWK3_Lift   , ccBwd); AT_MoveCyl(aiWK3_Align  , ccBwd);
                                                   AT_MoveCyl(aiPSB_Lift   , ccBwd); AT_MoveCyl(aiPSB_Align  , ccBwd); }
    else if (m_iManNo == mcETC_TransferUp      ) { AT_MoveCyl(aiWK1_Ins    , ccBwd); }
    else if (m_iManNo == mcETC_TransferDn      ) { AT_MoveCyl(aiWK1_Ins    , ccFwd); }
    else if (m_iManNo == mcETC_MainAirOnOff    ) { IO_SetY(yETC_MainSol, !IO_GetY(yETC_MainSol)); }
    else if (m_iManNo == mcETC_ManLotEnd       ) {  }
          //

    else                                    { m_iManNo = mcNoneCycle ; }
    Trace("뻑","5");
    m_bManSetting = false ;
    return true ;
}

EN_MANUAL_CYCLE __fastcall CManualMan::GetManNo()
{
    return m_iManNo ;
}

//---------------------------------------------------------------------------
void __fastcall CManualMan::Update(void)
{
    if( m_iManNo == mcNoneCycle ) return  ;
    if( m_bManSetting           ) return  ;

    if(!OM.CmnOptn.bDoorSkip    ) SEQ.InspectDoor    ();
    if(m_iManNo != mcLDR_Home     &&
       m_iManNo != mcRAL_Wk1Home  && //Rail Transfer Mt Home X;;;;;;;;;;
       m_iManNo != mcPSB_Home     &&
       m_iManNo != mcULD_Home     &&
       m_iManNo != mcETC_FlipHome &&
       m_iManNo != mcAllHome    ) SEQ.InspectHomeEnd ();
    SEQ.InspectLightGrid();

    bool r1,r2,r3,r4,r5 ;

    bool isWRKXWait     = MT_CmprPos (miWRK_XVsn , PM.GetValue(miWRK_XVsn,pvWRK_XVsnWaitPs)) && MT_GetStop(miWRK_XVsn) ;
    bool isWRKYWait     = MT_CmprPos (miWRK_YVsn , PM.GetValue(miWRK_YVsn,pvWRK_YVsnWaitPs)) && MT_GetStop(miWRK_YVsn);

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return ; }

    //Trace("뻑","6");

    //Cycle Step.
    if      (m_iManNo == mcAllHome         ) { r1 = LDR.CycleHome();

                                               if(isWRKXWait && isWRKYWait) r2 = RAL.CycleHome();

                                               r3 = HED.CycleHome();
                                               r4 = PSB.CycleHome();
                                               r5 = ULD.CycleHome();
                                                   if(r1&&r2&&r3&&r4&r5) {
                                                       m_iManNo = mcNoneCycle;
                                                       FM_MsgOk("Confirm","All Home Finished!");
                                                   }
                                              }

    else if (m_iManNo == mcLDR_Home        ) { if(LDR.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Supply      ) { if(LDR.CycleSupply       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Pick        ) { if(LDR.CyclePick         ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Place       ) { if(LDR.CyclePlace        ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcRAL_Wk1Home     ) { if(HED.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcRAL_Wk1NomalPos ) { if(CycleManFlprNomalPs   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcRAL_Wk1ReversePos) { if(CycleManFlprInversePs()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcPSB_Home        ) { if(PSB.CycleHome         ()) m_iManNo = mcNoneCycle; }
//    else if (m_iManNo == mcPSB_UD1Cycle    ) { if(CycleManMrkUpDn       ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcULD_Home        ) { if(ULD.CycleHome         ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Supply      ) { if(ULD.CycleSupply       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Pick        ) { if(ULD.CyclePick         ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Place       ) { if(ULD.CyclePlace        ()) m_iManNo = mcNoneCycle; }

//    else if (m_iManNo == mcETC_Wk1AlignLiftUp) { if(CycleMan1ndLift     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_FlipHome      ) { if(RAL.CycleHome       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_Wk2AlignLiftUp) { if(CycleMan2ndLift     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_Wk3AlignLiftUp) { if(CycleMan3ndLift     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_MrkAlignLiftUp) { if(CycleManMrkLift     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_AllAlignLiftUp) { if(CycleManAllLift     ()) m_iManNo = mcNoneCycle; }

    //Vision Switch
    else if (m_iManNo == mcETC_Wk1AlignLiftUp) { if(CycleMan1ndLift     ()) m_iManNo = mcNoneCycle;   }
    else if (m_iManNo == mcHED_InspMan       ) { if(CycleManBindInsp    ()) m_iManNo = mcNoneCycle;   }
    else if (m_iManNo == mcPSB_InitPos       ) { if(CycleManPsbInitPos  ()) m_iManNo = mcNoneCycle;   }
    else if (m_iManNo == mcPSB_WorkStPos     ) { if(CycleManWorkSttPos  ()) m_iManNo = mcNoneCycle;   }
//    else if (m_iManNo == mcPSB_UD1Cycle      ) { if(CycleManUD1         ()) m_iManNo = mcNoneCycle;   }
    else if (m_iManNo == mcPSB_TestMarking   ) { if(CycleManTestMrk     ()) m_iManNo = mcNoneCycle;   }
    else if (m_iManNo == mcPSB_UD1Cycle      ) { if(PSB.CycleWork       ()) m_iManNo = mcNoneCycle;   }
    else if (m_iManNo == mcPSB_MarkingChange ) { if(CycleManMrkChange   ()) m_iManNo = mcNoneCycle;   }
    else if (m_iManNo == mcPSB_MrkVisnInsp   ) { if(CycleManMrkBindInsp ()) m_iManNo = mcNoneCycle;   }
    else if (m_iManNo == mcETC_ManLotEnd     ) { if(CycleManLotEnd      ()) m_iManNo = mcNoneCycle;   }
    else                                       {                            m_iManNo = mcNoneCycle;   }

    //Ok.
    return ;
}

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
                  if( !MT_CmprPos (miPSB_XMrk, 100) || !MT_CmprPos (miPSB_YMrk, PM.GetValue(miPSB_YMrk, pvPSB_YMrkWaitPs)) ){
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
                  MT_GoAbsMan(miPSB_XMrk, 100);
                  MT_GoAbsMan(miPSB_YMrk, PM.GetValue(miPSB_YMrk, pvPSB_YMrkWaitPs));
                  m_iManStep ++;
                  return false;

        case  12: if(!MT_GoAbsMan(miPSB_XMrk, 100)) return false ;
                  if(!MT_GoAbsMan(miPSB_YMrk, PM.GetValue(miPSB_YMrk, pvPSB_YMrkWaitPs))) return false ;
                  m_iManStep = 0;
                  return true;

        case  13: if(!AT_MoveCyl(aiPSB_Marking, ccBwd)) return false;
                  MT_GoAbsMan(miPSB_XMrk, PM.GetValue(miPSB_XMrk, pvPSB_XMrkWaitPs));
                  MT_GoAbsMan(miPSB_YMrk, PM.GetValue(miPSB_YMrk, pvPSB_YMrkWaitPs));
                  m_iManStep ++;
                  return false;

        case  14: if(!MT_GoAbsMan(miPSB_XMrk, PM.GetValue(miPSB_XMrk, pvPSB_XMrkWaitPs))) return false ;
                  if(!MT_GoAbsMan(miPSB_YMrk, PM.GetValue(miPSB_YMrk, pvPSB_YMrkWaitPs))) return false ;
                  AT_MoveCyl(aiPSB_Marking, ccFwd);
                  m_iManStep ++;
                  return false;

        case  15: if(!AT_MoveCyl(aiPSB_Marking, ccFwd)) return false;
                  m_iManStep = 0;
                  return true;

        }

}

bool CManualMan::CycleMan1ndLift   ()
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

        case  10: MT_GoAbsMan(miWRK_XVsn,PM.GetValue(miWRK_XVsn,pvWRK_XVsnWaitPs));
                  MT_GoAbsMan(miWRK_YVsn,PM.GetValue(miWRK_YVsn,pvWRK_YVsnWaitPs));
                  m_iManStep++ ;
                  return false ;

        case  11: if(!MT_GoAbsMan(miWRK_XVsn,PM.GetValue(miWRK_XVsn,pvWRK_XVsnWaitPs))) return false ;
                  if(!MT_GoAbsMan(miWRK_YVsn,PM.GetValue(miWRK_YVsn,pvWRK_YVsnWaitPs))) return false ;

                  AT_MoveCyl(aiWK1_Ins   ,ccBwd);
                  m_iManStep++ ;
                  return false ;

        case  12: if(!AT_MoveCyl(aiWK1_Ins , ccBwd)) return false ;
                  RAL.MoveMotr(miWK1_XIns,piWK1_XInsWait);
                  m_iManStep++ ;
                  return false ;

        case  13: if(!RAL.MoveMotr(miWK1_XIns,piWK1_XInsWait)) return false ;
                  AT_MoveCyl(aiWK1_Ins    ,ccFwd);
                  AT_MoveCyl(aiWK1_FlpCmp ,ccBwd);
                  m_iManStep++ ;
                  return false ;

        case  14: if(!AT_MoveCyl(aiWK1_Ins    ,ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK1_FlpCmp ,ccBwd)) return false ;
//                  MT_GoAbs(miWK1_XIns,GetMotrPos(miWK1_XIns,piWK1_XInsStpr),OM.CmnOptn.dWK1InsStprPsSpeed ) ;
                  RAL.MoveMotr(miWK1_XIns,piWK1_XInsStpr);
                  m_iManStep++ ;
                  return false ;

        case  15: if(!RAL.MoveMotr(miWK1_XIns,piWK1_XInsStpr)) return false ;

                  AT_MoveCyl(aiWK1_Align,ccFwd);
                  m_iManStep++ ;
                  return false ;

        case  16: if(!AT_MoveCyl(aiWK1_Align,ccFwd)) return false ;
                  AT_MoveCyl(aiWK1_FlpCmp,ccFwd);
                  m_iManStep++ ;
                  return false ;

        case  17: if(!AT_MoveCyl(aiWK1_FlpCmp,ccFwd)) return false ;
                  AT_MoveCyl(aiWK1_Align,ccBwd);
                  RAL.MoveMotr(miWK1_XIns,piWK1_XInsWait);
                  m_iManStep++ ;
                  return false ;

        case  18: if(!AT_MoveCyl(aiWK1_Align,ccBwd)) return false ;
                  if(!RAL.MoveMotr(miWK1_XIns,piWK1_XInsWait)) return false ;
                  AT_MoveCyl(aiWK1_Ins,ccBwd);
                  m_iManStep++ ;
                  return false ;

        case  19: if(!AT_MoveCyl(aiWK1_Ins,ccBwd)) return false ;

                  m_iManStep = 0 ;
                  return true ;

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
                  MT_GoAbsMan(miPSB_XMrk , PM.GetValue(miPSB_XMrk,pvPSB_XMrkWaitPs)) ;
                  MT_GoAbsMan(miPSB_YMrk , PM.GetValue(miPSB_YMrk,pvPSB_YMrkWaitPs)) ;
                  m_iManStep++;
                  return false ;

        case 12 : if(!MT_GoAbsMan(miPSB_XMrk , PM.GetValue(miPSB_XMrk,pvPSB_XMrkWaitPs)))return false ;
                  if(!MT_GoAbsMan(miPSB_YMrk , PM.GetValue(miPSB_YMrk,pvPSB_YMrkWaitPs)))return false ;
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
                  MT_GoAbsMan(miPSB_XMrk , PM.GetValue(miPSB_XMrk,pvPSB_XMrkWorkStartPs)) ;
                  MT_GoAbsMan(miPSB_YMrk , PM.GetValue(miPSB_YMrk,pvPSB_YMrkWorkStartPs)) ;
                  m_iManStep++;
                  return false ;

        case 12 : if(!MT_GoAbsMan(miPSB_XMrk , PM.GetValue(miPSB_XMrk,pvPSB_XMrkWorkStartPs)))return false ;
                  if(!MT_GoAbsMan(miPSB_YMrk , PM.GetValue(miPSB_YMrk,pvPSB_YMrkWorkStartPs)))return false ;
//                  AT_MoveCyl(aiPSB_Marking  , ccFwd);
//                  m_iManStep++;
//                  return false ;

//        case 13 : if(!AT_MoveCyl(aiPSB_Marking    , ccFwd)) return false ;
                  m_iManStep = 0;
                  return true ;
    }
}

bool CManualMan::CycleManUD1       ()
{
//    //Check Cycle Time Out.
//    AnsiString sTemp ;
//    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 5000 )) {
//        EM_SetErr(eiETC_ManCycleTO);
//        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
//        Trace("Manual Man",sTemp.c_str());
//        m_iManStep = 0 ;
//        return true;
//    }
//
//    if(m_iManStep != m_iPreManStep) {
//        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
//        Trace("Manual Man",sTemp.c_str());
//    }
//
//    m_iPreManStep = m_iManStep ;
//
//    switch (m_iManStep) {
//
//        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
//                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
//                  m_iManStep = 0 ;
//                  return true ;
//
//        case  10: MT_GoAbsMan(miWRK_XVsn,PM.GetValue(miWRK_XVsn,pvWRK_XVsnWaitPs));

}

bool CManualMan::CycleManTestMrk   ()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        DM.ARAY[riPSB] = TempArray ;
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
                  TempArray = DM.ARAY[riPSB] ;
                  DM.ARAY[riPSB].SetStat(csRslt1);
                  AT_MoveCyl(aiPSB_Stopper,ccFwd);
                  AT_MoveCyl(aiPSB_Align  ,ccBwd);
                  m_iManStep++ ;
                  return false ;

        case  11: if(!AT_MoveCyl(aiPSB_Stopper,ccFwd)) return false ;
                  if(!AT_MoveCyl(aiPSB_Align  ,ccBwd)) return false ;
                  IO_SetY(yWR2_FeedingAC,true);
                  AT_MoveCyl(aiPSB_Align,ccFwd);
                  m_iManStep++ ;
                  return false ;

        case  12: if(!AT_MoveCyl(aiPSB_Align,ccFwd)) return false ;
                  AT_MoveCyl(aiPSB_Lift,ccFwd);
                  m_iManStep++ ;
                  return false ;

        case  13: if(!AT_MoveCyl(aiPSB_Lift,ccFwd)) return false ;
                  IO_SetY(yWR2_FeedingAC,false);
                  AT_MoveCyl(aiPSB_Align,ccBwd);
                  m_iManStep++ ;
                  return false ;

        case  14: //DM.ARAY[riPSB].SetStat(csRslt0);
                  PSB.InitCycleStep();
                  m_iManStep++ ;
                  return false ;

        case  15: m_tmCycle.Clear();
                  if(!PSB.CycleWork()) return false ;
                  if( DM.ARAY[riPSB].GetCntStat(csRslt1) && !m_bStopTestMrk ){
                      m_iManStep = 13 ;
                      return false ;
                  }
                  m_iManStep++ ;
                  return false ;

        case  16: AT_MoveCyl(aiPSB_Lift,ccBwd);
                  m_iManStep++ ;
                  return false ;

        case  17: if(!AT_MoveCyl(aiPSB_Lift,ccBwd)) return false ;
                  MT_GoAbsMan(miPSB_XMrk, PM.GetValue(miPSB_XMrk, pvPSB_XMrkWaitPs));
                  MT_GoAbsMan(miPSB_YMrk, PM.GetValue(miPSB_YMrk, pvPSB_YMrkWaitPs));
                  DM.ARAY[riPSB] = TempArray ;

                  m_iManStep++;
                  return false;

        case  18: if(!MT_GoAbsMan(miPSB_XMrk, PM.GetValue(miPSB_XMrk, pvPSB_XMrkWaitPs))) return false;
                  if(!MT_GoAbsMan(miPSB_YMrk, PM.GetValue(miPSB_YMrk, pvPSB_YMrkWaitPs))) return false;
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
                  IO_SetY(yWR1_FeedingAC , true);
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
                  AT_MoveCyl(aiWK2_Lift   , ccFwd);
                  AT_MoveCyl(aiWK3_Lift   , ccFwd);
                  AT_MoveCyl(aiWK1_FlpCmp , ccFwd);
                  m_iManStep++ ;
                  return false ;

        case  14: //if(!AT_MoveCyl(aiWK1_Lift , ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK2_Lift   , ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK3_Lift   , ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK1_FlpCmp , ccFwd)) return false ;
                  IO_SetY(yWR1_FeedingAC , false);
                  AT_MoveCyl(aiWK1_Align , ccBwd);
                  AT_MoveCyl(aiWK2_Align , ccBwd);
                  AT_MoveCyl(aiWK3_Align , ccBwd);
                  m_iManStep++ ;
                  return false ;

        case  15: if(!AT_MoveCyl(aiWK1_Align , ccBwd)) return false ;
                  if(!AT_MoveCyl(aiWK2_Align , ccBwd)) return false ;
                  if(!AT_MoveCyl(aiWK3_Align , ccBwd)) return false ;
                  //스테이지 올리고 바꿔야 바인딩 되고 HEAD에서 검사를 제때 시작 한다.
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
                  AT_MoveCyl(aiWK2_Lift   , ccBwd);
                  AT_MoveCyl(aiWK3_Lift   , ccBwd);
                  AT_MoveCyl(aiWK1_FlpCmp , ccBwd);
                  m_iManStep++ ;
                  return false ;

        case  18: //if(!AT_MoveCyl(aiWK1_Lift , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK2_Lift   , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK3_Lift   , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK1_FlpCmp , ccBwd)) return false;
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
bool CManualMan::CycleManMrkBindInsp()
{

    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 20000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        DM.ARAY[riPSB] = TempArray;
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

        case  10: AT_MoveCyl(aiPSB_Stopper , ccFwd);
                  TempArray = DM.ARAY[riPSB];

                  m_iManStep++;
                  return false ;

        case  11: if(!AT_MoveCyl(aiPSB_Stopper , ccFwd)) return false ;
                  IO_SetY(yWR2_FeedingAC , true);
                  m_tmTemp.Clear();
                  m_iManStep++;
                  return false ;

        case  12: if(!m_tmTemp.OnDelay(true , 300)) return false ;
                  //IO_SetY(yWR1_FeedingAC , false);
                  if(!IO_GetX(xPSB_Pkg) && !DM.ARAY[riPSB].GetCntExist() ) DM.ARAY[riVS4].SetStat(csUnkwn);

                  AT_MoveCyl(aiPSB_Align , ccFwd);

                  m_iManStep++ ;
                  return false ;

        case  13: if(!AT_MoveCyl(aiPSB_Align , ccFwd)) return false ;
                  //AT_MoveCyl(aiWK1_Lift , ccFwd);
                  AT_MoveCyl(aiPSB_Lift   , ccFwd);
                  m_iManStep++ ;
                  return false ;

        case  14: //if(!AT_MoveCyl(aiWK1_Lift , ccFwd)) return false ;
                  if(!AT_MoveCyl(aiPSB_Lift   , ccFwd)) return false ;
                  IO_SetY(yWR2_FeedingAC , false);
                  AT_MoveCyl(aiPSB_Align , ccBwd);
                  m_iManStep++ ;
                  return false ;

        case  15: if(!AT_MoveCyl(aiPSB_Align , ccBwd)) return false ;
                  //스테이지 올리고 바꿔야 바인딩 되고 HEAD에서 검사를 제때 시작 한다.
                  DM.ARAY[riVS4].SetStat(csUnkwn);
                  DM.ARAY[riPSB].SetStat(csUnkwn);

                  PSB.InitCycleStep();
                  m_iManStep++ ;
                  return false ;

        case  16: m_tmCycle.Clear();
                  if(!PSB.CycleVisn()) return false ;
                  AT_MoveCyl(aiPSB_Stopper , ccFwd);
                  m_iManStep++ ;
                  return false ;

        case  17: if(!AT_MoveCyl(aiPSB_Stopper , ccFwd)) return false;
//                  AT_MoveCyl(aiWK1_Lift , ccBwd);
                  AT_MoveCyl(aiPSB_Lift   , ccBwd);
                  PSB.MoveMotr(miPSB_XMrk , piPSB_XMrkWait);
                  PSB.MoveMotr(miPSB_YMrk , piPSB_YMrkWait);

                  m_iManStep++ ;
                  return false ;

        case  18: //if(!AT_MoveCyl(aiWK1_Lift , ccBwd)) return false;
                  if(!AT_MoveCyl(aiPSB_Lift   , ccBwd)) return false;
                  if(!PSB.MoveMotr(miPSB_XMrk , piPSB_XMrkWait)) return false ;
                  if(!PSB.MoveMotr(miPSB_YMrk , piPSB_YMrkWait)) return false ;

                  AT_MoveCyl(aiPSB_Marking , ccFwd);
                  m_iManStep++;
                  return false ;

        case  19: if(!AT_MoveCyl(aiPSB_Marking , ccFwd)) return false ;
                  DM.ARAY[riPSB] = TempArray ;
                  if(DM.ARAY[riVS4].GetCntExist())DM.ARAY[riVS4].SetStat(csUnkwn);
                  else                            DM.ARAY[riVS4].SetStat(csNone );
                  m_iManStep = 0 ;
                  return true ;
    }





}
bool CManualMan::CycleMan2ndLift()
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

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 m_iManStep = 0 ;
                 return true ;

        case  10: AT_MoveCyl(aiWK2_Align   , ccBwd);
                  AT_MoveCyl(aiWK2_Stopper , ccFwd);

                  m_iManStep++;
                  return false ;

        case  11: if(!AT_MoveCyl(aiWK2_Align   , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK2_Stopper , ccFwd)) return false;
                  IO_SetY(yWR1_FeedingAC,true );
                  m_tmCycle.Clear();
                  m_iManStep++;
                  return false ;

        case  12: if(!m_tmTemp.OnDelay(true , 300)) return false ;
                  AT_MoveCyl(aiWK2_Align, ccFwd);
                  m_iManStep++;
                  return false ;

        case  13: if(!AT_MoveCyl(aiWK2_Align, ccFwd)) return false;
                  AT_MoveCyl(aiWK2_Lift, ccFwd);
                  m_iManStep++;
                  return false ;

        case  14: if(!AT_MoveCyl(aiWK2_Lift, ccFwd)) return false;
                  IO_SetY(yWR1_FeedingAC,false );
                  AT_MoveCyl(aiWK2_Align, ccBwd);
                  m_iManStep++;
                  return false ;

        case  15: if(!AT_MoveCyl(aiWK2_Align, ccBwd)) return false;

                  m_iManStep = 0;
                  return true;
        }
}
//------------------------------------------------------------------
bool CManualMan::CycleMan3ndLift()
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

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 m_iManStep = 0 ;
                 return true ;

        case  10: AT_MoveCyl(aiWK3_Align   , ccBwd);
                  AT_MoveCyl(aiWK3_Stopper , ccFwd);

                  m_iManStep++;
                  return false ;

        case  11: if(!AT_MoveCyl(aiWK3_Align   , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK3_Stopper , ccFwd)) return false;
                  IO_SetY(yWR1_FeedingAC,true );
                  m_tmCycle.Clear();
                  m_iManStep++;
                  return false ;

        case  12: if(!m_tmTemp.OnDelay(true , 300)) return false ;
                  AT_MoveCyl(aiWK3_Align, ccFwd);
                  m_iManStep++;
                  return false ;

        case  13: if(!AT_MoveCyl(aiWK3_Align, ccFwd)) return false;
                  AT_MoveCyl(aiWK3_Lift, ccFwd);
                  m_iManStep++;
                  return false ;

        case  14: if(!AT_MoveCyl(aiWK3_Lift, ccFwd)) return false;
                  IO_SetY(yWR1_FeedingAC,false );
                  AT_MoveCyl(aiWK3_Align, ccBwd);
                  m_iManStep++;
                  return false ;

        case  15: if(!AT_MoveCyl(aiWK3_Align, ccBwd)) return false;

                  m_iManStep = 0;
                  return true;
        }
}
//--------------------------------------------------------------------------
bool CManualMan::CycleManMrkLift()
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

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 m_iManStep = 0 ;
                 return true ;

        case  10: AT_MoveCyl(aiPSB_Align   , ccBwd);
                  AT_MoveCyl(aiPSB_Stopper , ccFwd);

                  m_iManStep++;
                  return false ;

        case  11: if(!AT_MoveCyl(aiPSB_Align   , ccBwd)) return false;
                  if(!AT_MoveCyl(aiPSB_Stopper , ccFwd)) return false;
                  IO_SetY(yWR2_FeedingAC,true );
                  m_tmCycle.Clear();
                  m_iManStep++;
                  return false ;

        case  12: if(!m_tmTemp.OnDelay(true , 300)) return false ;
                  AT_MoveCyl(aiWK3_Align, ccFwd);
                  m_iManStep++;
                  return false ;

        case  13: if(!AT_MoveCyl(aiPSB_Align, ccFwd)) return false;
                  AT_MoveCyl(aiPSB_Lift, ccFwd);

                  m_iManStep++;
                  return false ;

        case  14: if(!AT_MoveCyl(aiPSB_Lift, ccFwd)) return false;
                  IO_SetY(yWR2_FeedingAC,false );
                  AT_MoveCyl(aiPSB_Align, ccBwd);
                  m_iManStep++;
                  return false ;

        case  15: if(!AT_MoveCyl(aiPSB_Align, ccBwd)) return false;

                  m_iManStep = 0;
                  return true;
        }
}
//----------------------------------------------------------------
bool CManualMan::CycleManAllLift()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 20000 )) {
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

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 m_iManStep = 0 ;
                 return true ;

        case  10: AT_MoveCyl(aiWK2_Align, ccBwd);
                  AT_MoveCyl(aiWK3_Align, ccBwd);
                  AT_MoveCyl(aiPSB_Align, ccBwd);
                  AT_MoveCyl(aiWK2_Stopper , ccFwd);
                  AT_MoveCyl(aiWK3_Stopper , ccFwd);
                  AT_MoveCyl(aiPSB_Stopper , ccFwd);

                  m_iManStep++;
                  return false ;

        case  11: if(!AT_MoveCyl(aiWK2_Align, ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK3_Align, ccBwd)) return false;
                  if(!AT_MoveCyl(aiPSB_Align, ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK2_Stopper , ccFwd)) return false;
                  if(!AT_MoveCyl(aiWK3_Stopper , ccFwd)) return false;
                  if(!AT_MoveCyl(aiPSB_Stopper , ccFwd)) return false;

                  IO_SetY(yWR1_FeedingAC,true );
                  IO_SetY(yWR2_FeedingAC,true );

                  m_tmCycle.Clear();
                  m_iManStep++;
                  return false ;

        case  12: if(!m_tmTemp.OnDelay(true , 300)) return false ;
                  AT_MoveCyl(aiWK2_Align, ccFwd);
                  AT_MoveCyl(aiWK3_Align, ccFwd);
                  AT_MoveCyl(aiPSB_Align, ccFwd);

                  m_iManStep++;
                  return false ;

        case  13: if(!AT_MoveCyl(aiWK2_Align, ccFwd)) return false;
                  if(!AT_MoveCyl(aiWK3_Align, ccFwd)) return false;
                  if(!AT_MoveCyl(aiPSB_Align, ccFwd)) return false;

                  AT_MoveCyl(aiWK2_Lift, ccFwd);
                  AT_MoveCyl(aiWK3_Lift, ccFwd);
                  AT_MoveCyl(aiPSB_Lift, ccFwd);

                  m_iManStep++;
                  return false ;

        case  14: if(!AT_MoveCyl(aiWK2_Lift, ccFwd)) return false;
                  if(!AT_MoveCyl(aiWK3_Lift, ccFwd)) return false;
                  if(!AT_MoveCyl(aiPSB_Lift, ccFwd)) return false;
                  IO_SetY(yWR1_FeedingAC,false );
                  IO_SetY(yWR2_FeedingAC,false );
                  AT_MoveCyl(aiWK2_Align, ccBwd);
                  AT_MoveCyl(aiWK3_Align, ccBwd);
                  AT_MoveCyl(aiPSB_Align, ccBwd);

                  m_iManStep++;
                  return false ;

        case  15: if(!AT_MoveCyl(aiWK2_Align, ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK3_Align, ccBwd)) return false;
                  if(!AT_MoveCyl(aiPSB_Align, ccBwd)) return false;
                  m_iManStep = 0;
                  return true;
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

        case  10: //bLDRExist = !DM.ARAY[riLDR].CheckAllStat(csNone) ; if(bLDRExist)
                  //bULDExist = !DM.ARAY[riULD].CheckAllStat(csNone) ; if(bULDExist)
                  LDR.InitCycleStep();
                  ULD.InitCycleStep();
                  m_iManStep++ ;
                  return false ;

        case  11: m_tmCycle.Clear();
                  bRet1 = LDR.CyclePlace() ;
                  bRet2 = ULD.CyclePlace() ;

                  if(!bRet1) return false ;
                  if(!bRet2) return false ;
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

                  m_iManStep ++;
                  return false;

        case  11: if(!AT_MoveCyl(aiWK1_FlpCmp, ccFwd)) return false;
                  if(AT_Complete(aiWK1_FlpCmp,ccFwd) && (IO_GetX(xWR1_ClampFw1) ||
                                                         IO_GetX(xWR1_ClampFw2) ||
                                                         IO_GetX(xWR1_ClampFw3) ||
                                                         IO_GetX(xWR1_ClampFw4) )){
                      EM_SetErr(eiWK1_FlpCmpTo ) ;
                      return false;
                  }
                  MT_GoAbsMan(miWK1_YFlp, PM.GetValue(miWK1_YFlp, pvWK1_YFlpNormalPs));

                  m_iManStep ++;
                  return false;
        case  12: if(!MT_GoAbsMan(miWK1_YFlp, PM.GetValue(miWK1_YFlp, pvWK1_YFlpNormalPs))) return false;
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

                  m_iManStep ++;
                  return false;

        case  11: if(!AT_MoveCyl(aiWK1_FlpCmp, ccFwd)) return false;
                  if(AT_Complete(aiWK1_FlpCmp,ccFwd) && (IO_GetX(xWR1_ClampFw1) ||
                                                         IO_GetX(xWR1_ClampFw2) ||
                                                         IO_GetX(xWR1_ClampFw3) ||
                                                         IO_GetX(xWR1_ClampFw4) )){
                      EM_SetErr(eiWK1_FlpCmpTo ) ;
                      return false;
                  }
                  MT_GoAbsMan(miWK1_YFlp, PM.GetValue(miWK1_YFlp, pvWK1_YFlpInversePs));

                  m_iManStep ++;
                  return false;
        case  12: if(!MT_GoAbsMan(miWK1_YFlp, PM.GetValue(miWK1_YFlp, pvWK1_YFlpInversePs))) return false;
                  AT_MoveCyl(aiWK1_FlpCmp, ccBwd);

                  m_iManStep = 0;
                  return true;
        }

}




//----------------------------------------------------------------
/*
bool CManualMan::CycleManMrkUpDn()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 20000 )) {
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

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 m_iManStep = 0 ;
                 return true ;

        case  10: AT_MoveCyl(aiPSB_Marking , ccFwd);

                  m_iManStep++;
                  return false ;

        case  11: if(!AT_MoveCyl(aiPSB_Marking, ccFwd)) return false;
                  ::Sleep(100);
                  AT_MoveCyl(aiPSB_Marking, ccBwd);

                  m_iManStep++;
                  return false ;

        case  12: if(!AT_MoveCyl(aiPSB_Marking, ccBwd)) return false;

                  m_iManStep = 0;
                  return true;
        }
}
//----------------------------------------------------------------
bool CManualMan::CycleManRemoveIndex()
{
/*    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 20000 )) {
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

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 m_iManStep = 0 ;
                 return true ;

        case  10: AT_MoveCyl(aiPRB_Stopper, ccFwd);
                  AT_MoveCyl(aiWK1_Stopper, ccFwd);
                  AT_MoveCyl(aiWK2_Stopper, ccFwd);
                  AT_MoveCyl(aiPSB_Stopper, ccFwd);
                  AT_MoveCyl(aiWK1_FlpCmp , ccBwd);
                  AT_MoveCyl(aiWK1_Ins    , ccBwd);


                  if(IO_GetX(xPRB_Pkg, false) || IO_GetX(xWR1_Pkg, false) ||
                     IO_GetX(xWR2_Pkg, false) || IO_GetX(xWR3_Pkg, false) ||
                     IO_GetX(xPSB_Pkg, false)){
                         m_iManStep = 0;
                         return true ;
                  }

                  m_iManStep++;
                  return false ;

        case  11: if(!AT_MoveCyl(aiPRB_Stopper, ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK1_Stopper, ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK2_Stopper, ccFwd)) return false ;
                  if(!AT_MoveCyl(aiPSB_Stopper, ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK1_FlpCmp , ccBwd)) return false ;
                  if(!AT_MoveCyl(aiWK1_Ins    , ccBwd)) return false ;

                  IO_SetY(yPRB_FeedingAC, true);
                  IO_SetY(yWR1_FeedingAC, true);
                  IO_SetY(yWR2_FeedingAC, true);

                  m_iManStep++;
                  return false;

        case  12: if(!IO_SetY(yPRB_FeedingAC, true)) return false;
                  if(!IO_SetY(yWR1_FeedingAC, true)) return false;
                  if(!IO_SetY(yWR2_FeedingAC, true)) return false;

                  if(IO_GetX(xPRB_Pkg, true) || IO_GetX(xWR1_Pkg, true) ||
                     IO_GetX(xWR2_Pkg, true) || IO_GetX(xWR3_Pkg, true) ||{
                  IO_SetY(yPRB_FeedingAC, false);
                  IO_SetY(yWR1_FeedingAC, false);
                  }

                  if(IO_GetX(xPSB_Pkg, true)){ PSB.CycleOut(); }

                  m_iManStep++;
                  return false;
}
*/
