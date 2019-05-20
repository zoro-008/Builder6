//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ManualMan.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff1.h"
#include "PreBuff2.h"
#include "WorkZone.h"
#include "PostBuff.h"
#include "PstnMan.h"
#include "SortingTool.h"
#include "UnLoader.h"
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "Sequence.h"
//---------------------------------------------------------------------------
#include "FormDeviceSet.h"
#include "FormMain.h"
#pragma package(smart_init)
int    iTrmManRow = -1;
int    iDevManRow = -1;
int    iTypeMan   = -1;
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
    if(SEQ._bRun                     ) {FM_MsgOk("ERROR","Autorunning"          ); return false ;}

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return false ; } //아 밑에 처리 하는 애 때문에 잠시 이렇게 함.

    if(_iNo  == mcAllHome  && (IO_GetX(xPRB_PkgIn1) || (IO_GetX(xPSB_PkgOut2 ) && OM.EqpOptn.bExistUnLoader) )) {FM_MsgOk("ERROR","Check Rail In or Out Sensor!"); return false ;}
    if(_iNo  == mcAllHome  && (!IO_GetX(xETC_MainSol) || !IO_GetY(yETC_MainSol))) {FM_MsgOk("ERROR","Check Main Air!"); return false ;}
    if(_iNo  == mcLDR_Home && (IO_GetX(xPRB_PkgIn1) /*|| IO_GetX(xPRB_2Pkg )*/)) {FM_MsgOk("ERROR","Check Rail In Sensor!"       ); return false ;}

    EM_SetDisp(true);

    double dWorkPitch ;
    if(OM.DevOptn.iContColCnt == 2) dWorkPitch = (OM.DevInfo.dColPitch + OM.DevOptn.dWorkPtchOfs)* 2 ;
    if(OM.DevOptn.iContColCnt == 4) dWorkPitch = (OM.DevInfo.dColPitch + OM.DevOptn.dWorkPtchOfs)* 4 ;

    double dTrimPitch ;
    dTrimPitch = (OM.DevInfo.dColPitch + OM.DevOptn.dTrimPtchOfs) ;

    int c,r ;
    int iMaxRow,iMaxCol;


    m_bManSetting = true ; //업데이트 함수에서 다른쓰레드로 들어와서 갱신하기에 플레그 걸어 막아둠.
    m_iManNo = _iNo ;

    bool bRet = true ;

    /********************/
    /* 프리프로세서     */
    /********************/
         if (m_iManNo == mcAllHome            ){     if(!IO_GetX(xETC_PowerOn  )) {FM_MsgOk("ERR","Power On Plz"); bRet = false ;}
                                                else if(!IO_GetX(xETC_MainSol) || !IO_GetY(yETC_MainSol)) {FM_MsgOk("ERR","Air On Plz"); bRet = false ;}
                                                //여기서 굳이 하는건 아닌거 같네...시퀀스에서 에러 띄우는게... ?
                                               }

    else if (m_iManNo == mcLDR_Home           ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;} }
    else if (m_iManNo == mcLDR_Supply         ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;} }
    else if (m_iManNo == mcLDR_Pick           ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
                                                if(!DM.ARAY[riLDR].CheckAllStat(csNone)       ){FM_MsgOk("ERR","Remove Mgz"          ); bRet = false ;} }
    else if (m_iManNo == mcLDR_SttPstn        ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
                                                if(!LDR.CheckSafe(miLDR_ZCmp,piLDR_ZWorkStart)){                                        bRet = false ;} }

    else if (m_iManNo == mcLDR_Place          ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
                                                if(FM_MsgYesNo("Config" , "Remove Magazine?") != mrYes){
                                                    LDR._iMgzCnt -= 1;
                                                }
                                               }
    else if (m_iManNo == mcLDR_UpSlPitch      ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
                                                if(!LDR.CheckSafe(miLDR_ZCmp,piLDR_ZWork)     ){                                        bRet = false ;} }
    else if (m_iManNo == mcLDR_DnSlPitch      ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
                                                if(!LDR.CheckSafe(miLDR_ZCmp,piLDR_ZWork)     ){                                        bRet = false ;} }
    else if (m_iManNo == mcLDR_Pusher         ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
                                                if(!LDR.CheckSafe(aiLDR_Psh , ccFwd)          ){                                        bRet = false ;}
                                                if(!LDR.CheckSafe(aiLDR_Psh , ccBwd)          ){                                        bRet = false ;} }
    //else if (m_iManNo == mcLDR_PusherBw       ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
    //                                            if(!LDR.CheckSafe(aiLDR_Psh , ccBwd)          ){                                        bRet = false ;} }

    else if (m_iManNo == mcLDR_Clamp          ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
                                                if(AT_Complete(aiULD_Cmp   , ccFwd)           ) { if(!ULD.CheckSafe(aiULD_Cmp   , ccBwd)) bRet = false ;}
                                                if(AT_Complete(aiULD_Cmp   , ccBwd)           ) {
                                                    if(IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Sure ?") != mrYes ) bRet = false;
                                                    if(!ULD.CheckSafe(aiULD_Cmp   , ccFwd)) bRet = false ;
                                                    }
                                               }

    /*
    if(IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Sure ?") != mrYes ) return ;
        bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;

    */
    //else if (m_iManNo == mcLDR_Clamp          ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
    //                                            if(!LDR.CheckSafe(aiLDR_Cmp , ccFwd)) bRet = false ;
    //                                           }
    else if (m_iManNo == mcLDR_TranInAC       ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;} }
    else if (m_iManNo == mcLDR_TranOutAC      ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;} }
    else if (m_iManNo == mcPR1_Home           ){}
    else if (m_iManNo == mcPR2_Home           ){}
    else if (m_iManNo == mcPR1_In             ){}
    else if (m_iManNo == mcPR1_Drag           ){}
    else if (m_iManNo == mcPR1_Out            ){}
    else if (m_iManNo == mcPR1_Clamp          ){if(AT_Complete(aiPR1_Cmp   , ccFwd)         ) { if(!PR1.CheckSafe(aiPR1_Cmp   , ccBwd)) bRet = false ;}
                                                if(AT_Complete(aiPR1_Cmp   , ccBwd)         ) { if(!PR1.CheckSafe(aiPR1_Cmp   , ccFwd)) bRet = false ;} }
    //else if (m_iManNo == mcPRE_IDCyl          ){if(AT_Complete(aiPRB_IdChk , ccFwd)         ) { if(!PR1.CheckSafe(aiPRB_IdChk , ccBwd)) bRet = false ;}
    //                                            if(AT_Complete(aiPRB_IdChk , ccBwd)         ) { if(!PR1.CheckSafe(aiPRB_IdChk , ccFwd)) bRet = false ;} }
    else if (m_iManNo == mcPR1_Feeding        ){}

    else if (m_iManNo == mcPR2_In             ){}
    else if (m_iManNo == mcPR2_Drag           ){}
    else if (m_iManNo == mcPR2_Out            ){}

    else if (m_iManNo == mcPR2_Clamp          ){if(AT_Complete(aiPR2_Cmp   , ccFwd)         ) { if(!PR2.CheckSafe(aiPR2_Cmp   , ccBwd)) bRet = false ;}
                                                if(AT_Complete(aiPR2_Cmp   , ccBwd)         ) { if(!PR2.CheckSafe(aiPR2_Cmp   , ccFwd)) bRet = false ;} }

    else if (m_iManNo == mcWRK_Home           ){}
    else if (m_iManNo == mcWRK_Work           ){}
    else if (m_iManNo == mcWRK_NextMove       ){if(!IO_GetX(xWRK_Detect1)) bRet = false;}
    //else if (m_iManNo == mcWRK_R1Pitch        ){if(!WRK.CheckSafe(miWRK_XCmp , piWRK_XWork)   ){                                        bRet = false ;} }
    //else if (m_iManNo == mcWRK_L1Pitch        ){if(!WRK.CheckSafe(miWRK_XCmp , piWRK_XWork)   ){                                        bRet = false ;} }
    //else if (m_iManNo == mcWRK_PostWork       ){}
    else if (m_iManNo == mcWRK_ToolChng       ){if(!WRK.CheckSafe(miWRK_YBtm , piWRK_YBWait)  ){                                        bRet = false ;} }
    else if (m_iManNo == mcWRK_ToolContect    ){if(!WRK.CheckSafe(miWRK_YBtm , piWRK_YBWait)  ){                                        bRet = false ;} }

    else if (m_iManNo == mcWRK_Clamp          ){if(AT_Complete(aiWRK_Cmp   , ccFwd)         ) { if(!WRK.CheckSafe(aiWRK_Cmp   , ccBwd)) bRet = false ;}
                                                if(AT_Complete(aiWRK_Cmp   , ccBwd)         ) { if(!WRK.CheckSafe(aiWRK_Cmp   , ccFwd)) bRet = false ;} }
    else if (m_iManNo == mcWRK_RptCntct       ){}

    else if (m_iManNo == mcPSB_Home           ){}
    else if (m_iManNo == mcPSB_WorkZoneCycle  ){}
    else if (m_iManNo == mcPSB_Drag           ){}
    else if (m_iManNo == mcPSB_Out            ){}
    else if (m_iManNo == mcPSB_Clamp          ){if(AT_Complete(aiPSB_Cmp   , ccBwd)         ) { if(!PSB.CheckSafe(aiPSB_Cmp   , ccBwd)) bRet = false ;}
                                                if(AT_Complete(aiPSB_Cmp   , ccFwd)         ) { if(!PSB.CheckSafe(aiPSB_Cmp   , ccFwd)) bRet = false ;} }
    else if (m_iManNo == mcPSB_NextMove       ){if(!IO_GetX(xWRK_Detect2)) bRet = false;}
    else if (m_iManNo == mcPSB_Pusher         ){if(!OM.EqpOptn.bExistUnLoader               ){FM_MsgOk("ERR","Post Buff doesn't Exist"); bRet = false ;}
                                                if(!PSB.CheckSafe(aiPSB_Psh , ccFwd)        ){                                           bRet = false ;}
                                                if(!PSB.CheckSafe(aiPSB_Psh , ccBwd)        ){                                           bRet = false ;} }
    //else if (m_iManNo == mcPSB_PusherBw       ){if(!OM.EqpOptn.bExistUnLoader               ){FM_MsgOk("ERR","Post Buff doesn't Exist"); bRet = false ;}
    //                                            if(!PSB.CheckSafe(aiPSB_Psh , ccBwd)        ){                                           bRet = false ;} }
    else if (m_iManNo == mcSTL_YTopClamp      ){if(AT_Complete(aiSTL_Top   , ccFwd)         ) { if(!STL.CheckSafe(aiSTL_Top   , ccBwd)) bRet = false ;}
                                                if(AT_Complete(aiSTL_Top   , ccBwd)         ) { if(!STL.CheckSafe(aiSTL_Top   , ccFwd)) bRet = false ;} }
    else if (m_iManNo == mcSTL_YBtmClamp      ){if(AT_Complete(aiSTL_Btm   , ccFwd)         ) { if(!STL.CheckSafe(aiSTL_Btm   , ccBwd)) bRet = false ;}
                                                if(AT_Complete(aiSTL_Btm   , ccBwd)         ) { if(!STL.CheckSafe(aiSTL_Btm   , ccFwd)) bRet = false ;} }
    else if (m_iManNo == mcPSB_Feeding        ){}
    else if (m_iManNo == mcPSB_R1Pitch        ){if(!PSB.CheckSafe(miPSB_XCmp , piPSB_XWork) ){                                          bRet = false ;} }
    else if (m_iManNo == mcPSB_L1Pitch        ){if(!PSB.CheckSafe(miPSB_XCmp , piPSB_XWork) ){                                          bRet = false ;} }
    else if (m_iManNo == mcSTL_YPchUp1Pitch   ){if( AT_Complete(aiSTL_Top   , ccFwd        )){FM_MsgOk("ERR","STL_Top is not BWD"     );bRet = false ;} }
    else if (m_iManNo == mcSTL_YPchDn1Pitch   ){if( AT_Complete(aiSTL_Top   , ccFwd        )){FM_MsgOk("ERR","STL_Top is not BWD"     );bRet = false ;} }
    else if (m_iManNo == mcSTL_YDevUp1Pitch   ){if( AT_Complete(aiSTL_Top   , ccFwd        )){FM_MsgOk("ERR","STL_Top is not BWD"     );bRet = false ;} }
    else if (m_iManNo == mcSTL_YDevDn1Pitch   ){if( AT_Complete(aiSTL_Top   , ccFwd        )){FM_MsgOk("ERR","STL_Top is not BWD"     );bRet = false ;} }
    else if (m_iManNo == mcSTL_Pick           ){}

    else if (m_iManNo == mcETC_AllClampOff    ){if(!PR1.CheckSafe(aiPR1_Cmp   , ccBwd)      )                                           bRet = false ;
                                                //if(!PR1.CheckSafe(aiPRB_IdChk , ccFwd)      )                                           bRet = false ;
                                                if(!WRK.CheckSafe(aiWRK_Cmp   , ccBwd)      )                                           bRet = false ;
                                                if(!PSB.CheckSafe(aiPSB_Cmp   , ccBwd)      )                                           bRet = false ;  }

//    else if (m_iManNo == mcETC_RemoveStrip    ){Trace("Manual Man","mcETC_RemoveStrip    Stt");}
//    else if (m_iManNo == mcETC_PreRemoveStrip ){Trace("Manual Man","mcETC_PreRemoveStrip Stt");}
//    else if (m_iManNo == mcETC_WrkRemoveStrip ){Trace("Manual Man","mcETC_WrkRemoveStrip Stt");}
//    else if (m_iManNo == mcETC_PstRemoveStrip ){Trace("Manual Man","mcETC_PstRemoveStrip Stt");}
    else if (m_iManNo == mcETC_AirBlower      ){}
    //else if (m_iManNo == mcETC_6              ){}

    else if (m_iManNo == mcETC_PrbStt         ){}
    else if (m_iManNo == mcETC_LDRULDPlace    ){}
    else if (m_iManNo == mcETC_TPrbUp         ){}
    else if (m_iManNo == mcETC_TPrbDn         ){}
    else if (m_iManNo == mcETC_BPrbUp         ){if(!WRK.CheckSafe(miWRK_ZBtm , piWRK_ZTWait)){                                          bRet = false ;} }
    else if (m_iManNo == mcETC_BPrbDn         ){if(!WRK.CheckSafe(miWRK_ZBtm , piWRK_ZTWait)){                                          bRet = false ;} }

    else if (m_iManNo == mcULD_Home           ){}
    else if (m_iManNo == mcULD_Supply         ){}
    else if (m_iManNo == mcULD_Pick           ){if(!DM.ARAY[riULD].CheckAllStat(csNone))                                                bRet = false ;}
    else if (m_iManNo == mcULD_SttPstn        ){if(!ULD.CheckSafe(miULD_ZCmp,piULD_ZWorkStart))                                         bRet = false ;}
    else if (m_iManNo == mcULD_Place          ){}
    else if (m_iManNo == mcULD_UpSlPitch      ){if(!ULD.CheckSafe(miULD_ZCmp,piULD_ZWork))                                              bRet = false ;}
    else if (m_iManNo == mcULD_DnSlPitch      ){if(!ULD.CheckSafe(miULD_ZCmp,piULD_ZWork))                                              bRet = false ;}
    else if (m_iManNo == mcULD_Clamp          ){if(AT_Complete(aiULD_Cmp   , ccFwd)         ) { if(!ULD.CheckSafe(aiULD_Cmp   , ccBwd)) bRet = false ;}
                                                if(AT_Complete(aiULD_Cmp   , ccBwd)         ) {
                                                    if(IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Sure ?") != mrYes ) bRet = false;
                                                    if(!ULD.CheckSafe(aiULD_Cmp   , ccFwd)) bRet = false ;
                                                }
                                               }
    else if (m_iManNo == mcULD_TranInAC       ){}
    else if (m_iManNo == mcULD_TranOutAC      ){}
    else if (m_iManNo == mcETC_PSBPlace       ){if((!IO_GetX(xWRK_Detect1) || !IO_GetX(xWRK_Detect2)) &&
                                                    !DM.ARAY[riPSB].GetCntStat(csNone) && !DM.ARAY[riULD].GetCntStat(csEmpty))          bRet = false ;
                                                 else { if(FM_MsgYesNo("Check", "Real Strip Remove?")==mrYes) m_iManStep=10;}
                                               }

    //else if (m_iManNo == mcSTL_Pick           ){if( DM.ARAY[riSTL].GetCntStat  (csNone))                                                bRet = false ;}
    //else if (m_iManNo == mcSTL_Place          ){if( DM.ARAY[riSTL].GetCntStat  (csFail))                                                bRet = false ;}



    if(!bRet) Init();



    /********************/
    /* 처리..           */
    /********************/
    if      (m_iManNo == mcAllHome            ) { //IO_SetY(yETC_MainSol ,true); 이러면 빠져나가는 상황이 있어서 그냥 윗단에서
                                                  //IO_SetY(yETC_MainSol2,true); 에러를 잡는다.
                                                   if(OM.EqpOptn.bExistLoader) LDR.InitHomeStep ();
                                                                               PR1.InitHomeStep ();
                                                                               PR2.InitHomeStep ();
                                                                               WRK.InitHomeStep ();
                                                                               PSB.InitHomeStep ();
                                                                               STL.InitHomeStep ();
                                                                               ULD.InitHomeStep (); }

    else if (m_iManNo == mcLDR_Home           ) { LDR.InitHomeStep (); }
    else if (m_iManNo == mcLDR_Supply         ) { LDR.InitCycleStep(); }
    else if (m_iManNo == mcLDR_Pick           ) { LDR.InitCycleStep(); }
    else if (m_iManNo == mcLDR_SttPstn        ) { MT_GoAbsMan(miLDR_ZCmp,PM.GetValue(miLDR_ZCmp , pvLDR_ZWorkStart) ); }
    else if (m_iManNo == mcLDR_Place          ) { DM.ARAY[riLDR].SetStat(csEmpty); LDR.InitCycleStep(); }
    else if (m_iManNo == mcLDR_UpSlPitch      ) { MT_GoIncMan(miLDR_ZCmp ,-OM.DevInfo.dCsSlPitch);}
    else if (m_iManNo == mcLDR_DnSlPitch      ) { MT_GoIncMan(miLDR_ZCmp , OM.DevInfo.dCsSlPitch);}
    else if (m_iManNo == mcLDR_Pusher         ) { if(AT_Complete(aiLDR_Psh , ccFwd )) { AT_MoveCyl(aiLDR_Psh , ccBwd); }
                                                  if(AT_Complete(aiLDR_Psh , ccBwd )) { AT_MoveCyl(aiLDR_Psh , ccFwd); } }
    //else if (m_iManNo == mcLDR_PusherBw       ) { AT_MoveCyl(aiLDR_Psh , ccBwd); }
    //else if (m_iManNo == mcLDR_UnClamp        ) { AT_MoveCyl(aiLDR_Cmp , ccFwd); }

    else if (m_iManNo == mcLDR_Clamp          ) { if(AT_Complete(aiLDR_Cmp , ccFwd )) { AT_MoveCyl(aiLDR_Cmp , ccBwd); }
                                                  if(AT_Complete(aiLDR_Cmp , ccBwd )) { AT_MoveCyl(aiLDR_Cmp , ccFwd); } }


    else if (m_iManNo == mcLDR_TranInAC       ) { IO_SetY(yLDR_MgzInAC  , !IO_GetY(yLDR_MgzInAC ) ); }
    else if (m_iManNo == mcLDR_TranOutAC      ) { IO_SetY(yLDR_MgzOutAC , !IO_GetY(yLDR_MgzOutAC) ); }

    else if (m_iManNo == mcPR1_Home           ) { PR1.InitHomeStep (); }
    else if (m_iManNo == mcPR2_Home           ) { PR2.InitHomeStep (); }
    else if (m_iManNo == mcPR1_In             ) { PR1.InitCycleStep(); }
    else if (m_iManNo == mcPR1_Drag           ) { PR1.InitCycleStep(); }
    else if (m_iManNo == mcPR1_Out            ) { PR1.InitCycleStep(); }
    else if (m_iManNo == mcPR1_Clamp          ) { if(AT_Complete(aiPR1_Cmp   , ccFwd)) { AT_MoveCyl(aiPR1_Cmp   , ccBwd); }
                                                  if(AT_Complete(aiPR1_Cmp   , ccBwd)) { AT_MoveCyl(aiPR1_Cmp   , ccFwd); } }
    //else if (m_iManNo == mcPRE_IDCyl          ) { if(AT_Complete(aiPRB_IdChk , ccFwd)) { AT_MoveCyl(aiPRB_IdChk , ccBwd); }
    //                                              if(AT_Complete(aiPRB_IdChk , ccBwd)) { AT_MoveCyl(aiPRB_IdChk , ccFwd); } }
    else if (m_iManNo == mcPR1_Feeding        ) { IO_SetY(yPRB_FeedingAC , !IO_GetY(yPRB_FeedingAC ) ); }

    else if (m_iManNo == mcPR2_In             ) { PR2.InitCycleStep(); }
    else if (m_iManNo == mcPR2_Drag           ) { PR2.InitCycleStep(); }
    else if (m_iManNo == mcPR2_Out            ) { PR2.InitCycleStep(); }
    else if (m_iManNo == mcPR2_Clamp          ) { if(AT_Complete(aiPR2_Cmp   , ccFwd)) { AT_MoveCyl(aiPR2_Cmp   , ccBwd); }
                                                  if(AT_Complete(aiPR2_Cmp   , ccBwd)) { AT_MoveCyl(aiPR2_Cmp   , ccFwd); } }

    else if (m_iManNo == mcWRK_Home           ) { WRK.InitHomeStep (); }
    else if (m_iManNo == mcWRK_Work           ) { WRK.InitCycleStep(); }
    else if (m_iManNo == mcWRK_NextMove       ) { DM.ARAY[riWRK].SetStep(1); WRK.InitCycleStep(); }
    else if (m_iManNo == mcWRK_Clamp          ) { if(AT_Complete(aiWRK_Cmp   , ccFwd)) { AT_MoveCyl(aiWRK_Cmp   , ccBwd); }
                                                  if(AT_Complete(aiWRK_Cmp   , ccBwd)) { AT_MoveCyl(aiWRK_Cmp   , ccFwd); } }
    else if (m_iManNo == mcWRK_ToolChng       ) { WRK.InitCycleStep(); }
    else if (m_iManNo == mcWRK_ToolContect    ) { WRK.InitCycleStep(); }

    else if (m_iManNo == mcWRK_RptCntct       ){if(DM.ARAY[riWRK].CheckAllStat(csNone)) {m_iManNo = mcNoneCycle; m_bManSetting = false ; return false; } //m_bManSetting 중요함 리턴전에 꼭 펄스 시켜야함. 쓰레드가 달라서. ::Update에서 m_iManNo=0이 되므로 주의.
                                                iMaxRow = DM.ARAY[riWRK].GetMaxRow();
                                                if(DM.ARAY[riWRK].GetCntStat(csWork) || DM.ARAY[riWRK].GetCntStat(csFail)) {
                                                    c = DM.ARAY[riWRK].FindLastCol(csUnkwn) ;
                                                    if(c > -1) {
                                                        for(int j = 0 ; j < OM.DevOptn.iContColCnt ;j++) {
                                                            for(int i =0 ; i < iMaxRow ; i++) DM.ARAY[riWRK].SetStat(i ,c - j, csUnkwn) ;
                                                        }
                                                    }
                                                }

                                                CWorkZone::SStat Stat = WRK.GetStat(); Stat.bRepeat = true ; WRK.SetStat(Stat);
                                                WRK.InitCycleStep();}

    else if (m_iManNo == mcPSB_Home           ) { PSB.InitHomeStep (); STL.InitHomeStep(); }
    else if (m_iManNo == mcPSB_WorkZoneCycle  ) { PSB.InitCycleStep(); }
    else if (m_iManNo == mcPSB_Drag           ) { PSB.InitCycleStep(); }
    else if (m_iManNo == mcPSB_Out            ) { PSB.InitCycleStep(); }
    else if (m_iManNo == mcPSB_Clamp          ) { if(AT_Complete(aiPSB_Cmp   , ccFwd)) { AT_MoveCyl(aiPSB_Cmp , ccBwd); }
                                                  if(AT_Complete(aiPSB_Cmp   , ccBwd)) { AT_MoveCyl(aiPSB_Cmp , ccFwd); } }
    else if (m_iManNo == mcPSB_NextMove       ) { DM.ARAY[riWRK].SetStep(1); PSB.InitCycleStep(); }

    else if (m_iManNo == mcSTL_YTopClamp      ) { if(AT_Complete(aiSTL_Top   , ccFwd)) { AT_MoveCyl(aiSTL_Top , ccBwd); }
                                                  if(AT_Complete(aiSTL_Top   , ccBwd)) { AT_MoveCyl(aiSTL_Top , ccFwd); } }
    else if (m_iManNo == mcSTL_YBtmClamp      ) { if(AT_Complete(aiSTL_Btm   , ccFwd)) { AT_MoveCyl(aiSTL_Btm , ccBwd); }
                                                  if(AT_Complete(aiSTL_Btm   , ccBwd)) { AT_MoveCyl(aiSTL_Btm , ccFwd); } }
    else if (m_iManNo == mcPSB_Feeding        ) { IO_SetY(yPSB_FeedingAC , !IO_GetY(yPSB_FeedingAC ) ); }
    else if (m_iManNo == mcPSB_R1Pitch        ) { MT_GoIncMan(miPSB_XCmp, dTrimPitch); }
    else if (m_iManNo == mcPSB_L1Pitch        ) { MT_GoIncMan(miPSB_XCmp,-dTrimPitch); }
    else if (m_iManNo == mcPSB_Pusher         ) { if(AT_Complete(aiPSB_Psh   , ccFwd)) { AT_MoveCyl(aiPSB_Psh , ccBwd); }
                                                  if(AT_Complete(aiPSB_Psh   , ccBwd)) { AT_MoveCyl(aiPSB_Psh , ccFwd); } }
    else if (m_iManNo == mcSTL_YPchUp1Pitch   ) {
        iTrmManRow++ ;
        iTypeMan = 0;
        if(iTrmManRow < 0 ) iTrmManRow = 0 ;
        if(iTrmManRow >= OM.DevInfo.iRowCnt) iTrmManRow = OM.DevInfo.iRowCnt - 1;
        MT_GoAbsMan(miSTL_YTop , GetMotrPosTable(iTrmManRow, iTypeMan));
        if(!IO_GetX(xSTL_DevDetect)){ }
    }
    else if (m_iManNo == mcSTL_YPchDn1Pitch   ) {
        iTrmManRow-- ;
        iTypeMan = 0;
        if(iTrmManRow < 0 ) iTrmManRow = 0 ;
        if(iTrmManRow >= OM.DevInfo.iRowCnt) iTrmManRow = OM.DevInfo.iRowCnt - 1;
        MT_GoAbsMan(miSTL_YTop , GetMotrPosTable(iTrmManRow, iTypeMan)) ;
        if(!IO_GetX(xSTL_DevDetect)){ }
    }
    else if (m_iManNo == mcSTL_YDevUp1Pitch   ) {
        iDevManRow++ ;
        iTypeMan = 1;
        if(iDevManRow < 0 ) iDevManRow = 0 ;
        if(iDevManRow >= OM.DevInfo.iRowCnt) iDevManRow = OM.DevInfo.iRowCnt - 1;
        MT_GoAbsMan(miSTL_YTop , GetMotrPosTable(iDevManRow, iTypeMan));
        if(!IO_GetX(xSTL_DevDetect)){ }
    }
    else if (m_iManNo == mcSTL_YDevDn1Pitch   ) {
        iDevManRow-- ;
        iTypeMan = 1;
        if(iDevManRow < 0 ) iDevManRow = 0 ;
        if(iDevManRow >= OM.DevInfo.iRowCnt) iDevManRow = OM.DevInfo.iRowCnt - 1;
        MT_GoAbsMan(miSTL_YTop , GetMotrPosTable(iDevManRow, iTypeMan)) ;
        if(!IO_GetX(xSTL_DevDetect)){ }
    }
    else if (m_iManNo == mcULD_Home          ) { ULD.InitHomeStep (); }
    else if (m_iManNo == mcULD_Supply        ) { ULD.InitCycleStep(); }
    else if (m_iManNo == mcULD_Pick          ) { ULD.InitCycleStep(); }
    else if (m_iManNo == mcULD_SttPstn       ) { MT_GoAbsMan(miULD_ZCmp,PM.GetValue(miULD_ZCmp , pvULD_ZWorkStart) );  }
    else if (m_iManNo == mcULD_Place         ) { ULD.InitCycleStep(); }
    else if (m_iManNo == mcULD_UpSlPitch     ) { MT_GoIncMan(miULD_ZCmp ,-OM.DevInfo.dCsSlPitch);}
    else if (m_iManNo == mcULD_DnSlPitch     ) { MT_GoIncMan(miULD_ZCmp , OM.DevInfo.dCsSlPitch);}
    else if (m_iManNo == mcULD_Clamp         ) { if(AT_Complete(aiULD_Cmp   , ccFwd)) { AT_MoveCyl(aiULD_Cmp , ccBwd); }
                                                 if(AT_Complete(aiULD_Cmp   , ccBwd)) { AT_MoveCyl(aiULD_Cmp , ccFwd); } }
    else if (m_iManNo == mcULD_TranInAC      ) { IO_SetY(yULD_MgzInAC  , !IO_GetY(yULD_MgzInAC ) );}
    else if (m_iManNo == mcULD_TranOutAC     ) { IO_SetY(yULD_MgzOutAC , !IO_GetY(yULD_MgzOutAC) );}

    else if (m_iManNo == mcSTL_Pick          ) {STL.InitCycleStep(); }
    //else if (m_iManNo == mcSTL_Place         ) {STL.InitCycleStep(); }


    else if (m_iManNo == mcETC_AllClampOff   ) {AT_MoveCyl(aiPR1_Cmp   , ccBwd);
                                                //AT_MoveCyl(aiPRB_IdChk , ccFwd);
                                                AT_MoveCyl(aiWRK_Cmp   , ccBwd);
                                                AT_MoveCyl(aiPSB_Cmp   , ccBwd);
    }

//    else if (m_iManNo == mcETC_RemoveStrip    ) { m_iManStep = 10 ; m_iPreManStep = 0 ; }
//    else if (m_iManNo == mcETC_PreRemoveStrip ) { m_iManStep = 10 ; m_iPreManStep = 0 ; }
//    else if (m_iManNo == mcETC_WrkRemoveStrip ) { m_iManStep = 10 ; m_iPreManStep = 0 ; }
//    else if (m_iManNo == mcETC_PstRemoveStrip ) { m_iManStep = 10 ; m_iPreManStep = 0 ; }

    else if (m_iManNo == mcETC_AirBlower      ) {  IO_SetY(yPRB_AirBlower  , !IO_GetY(yPRB_AirBlower ) );  }

    else if (m_iManNo == mcETC_PrbStt         ) {
        AnsiString sMsg;
        sMsg = "START#";
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
    }
    else if (m_iManNo == mcETC_BPrbDn         ) { if(AT_Complete(aiSTL_Btm , ccFwd)) { AT_MoveCyl(aiSTL_Btm , ccBwd); }}
    else if (m_iManNo == mcETC_BPrbUp         ) { if(AT_Complete(aiSTL_Btm , ccBwd)) { AT_MoveCyl(aiSTL_Btm , ccFwd); }}
    else if (m_iManNo == mcETC_TPrbDn         ) { if(AT_Complete(aiSTL_Top , ccBwd)) { AT_MoveCyl(aiSTL_Top , ccFwd); }}
    else if (m_iManNo == mcETC_TPrbUp         ) { if(AT_Complete(aiSTL_Top , ccFwd)) { AT_MoveCyl(aiSTL_Top , ccBwd); }}

    else if (m_iManNo == mcETC_LDRULDPlace    ) { if(!OM.EqpOptn.bExistLoader || !OM.EqpOptn.bExistUnLoader) {m_bManSetting = false ; return false ;  } //m_bManSetting 중요함 리턴전에 꼭 펄스 시켜야함. 쓰레드가 달라서. ::Update에서 m_iManNo=0이 되므로 주의.
                                                  LDR.InitCycleStep();
                                                  ULD.InitCycleStep();
                                                }
    else if (m_iManNo == mcETC_PSBPlace       ) { CyclePlaceStrp();}

    else                                        { m_iManNo = mcNoneCycle ; }

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
    if(m_bManSetting            ) return ;

    if(!OM.CmnOptn.bIgnrHeatAlm ) SEQ.InspectTemp    ();
    if(!OM.CmnOptn.bIgnrDoor    ) SEQ.InspectDoor    ();
    if(m_iManNo != mcLDR_Home &&
       m_iManNo != mcPR1_Home &&
       m_iManNo != mcPR2_Home &&
       m_iManNo != mcWRK_Home &&
       m_iManNo != mcPSB_Home &&
       m_iManNo != mcULD_Home &&
       m_iManNo != mcAllHome    ) SEQ.InspectHomeEnd ();
    SEQ.InspectLightGrid();

    bool r1,r2,r3,r4,r5,r6,r7 ;

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return ; }
    //Cycle Step.
    if      (m_iManNo == mcAllHome         )  {if(OM.EqpOptn.bExistLoader)   r1 = LDR.CycleHome();
                                               else                          r1 = true ;
                                                                             r2 = PR1.CycleHome();
                                                                             r3 = PR2.CycleHome();
                                                                             r4 = WRK.CycleHome();
                                                                             r6 = STL.CycleHome();
                                                                             if(r6) r5 = PSB.CycleHome();
                                               if(OM.EqpOptn.bExistUnLoader) r7 = ULD.CycleHome();
                                               else                          r7 = true ;
                                                   if(r1&&r2&&r3&&r4&&r5&&r6&&r7) {
                                                       m_iManNo = mcNoneCycle;
                                                       FM_MsgOk("Confirm","All Home Finished!");
                                                   }
                                               }
    else if (m_iManNo == mcETC_LDRULDPlace )  { r1 = LDR.CyclePlace();
                                                r7 = ULD.CyclePlace();
                                                if(r1&&r7) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcLDR_Home     && OM.EqpOptn.bExistLoader ) { if(LDR.CycleHome     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Supply   && OM.EqpOptn.bExistLoader ) { if(LDR.CycleSupply   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Pick     && OM.EqpOptn.bExistLoader ) { if(LDR.CyclePick     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Place    && OM.EqpOptn.bExistLoader ) { if(LDR.CyclePlace    ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcPR1_Home         ) { if(PR1.CycleHome       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPR2_Home         ) { if(PR2.CycleHome       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPR1_In           ) { if(PR1.CycleIn         ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPR1_Drag         ) { if(PR1.CycleDrag       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPR1_Out          ) { if(PR1.CycleOut        ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPR2_In           ) { if(PR2.CycleIn         ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPR2_Drag         ) { if(PR2.CycleDrag       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPR2_Out          ) { if(PR2.CycleOut        ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcWRK_Home         ) { if(WRK.CycleHome       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcWRK_Work         ) { if(WRK.CycleWork       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcWRK_NextMove     ) { if(WRK.CycleNextMove   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcWRK_ToolChng     ) { if(WRK.CycleToolChange ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcWRK_ToolContect  ) { if(WRK.CycleToolWait   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcWRK_RptCntct     ) { if(WRK.CycleRptCntct   ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcPSB_Home         ) { if(STL.CycleHome       ()) { if(PSB.CycleHome()) m_iManNo = mcNoneCycle; } }
    else if (m_iManNo == mcPSB_WorkZoneCycle) { if(PSB.CycleContact    ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_Drag         ) { if(PSB.CycleDrag       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_NextMove     ) { if(PSB.CycleNextMove   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_Out          ) { if(PSB.CycleOut        ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcULD_Home         ) { if(ULD.CycleHome       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Supply       ) { if(ULD.CycleSupply     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Pick         ) { if(ULD.CyclePick       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Place        ) { if(ULD.CyclePlace      ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcSTL_Pick         ) { if(STL.CyclePunch      ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_LDRULDPlace  ) { if(LDR.CyclePlace      ()) { if(ULD.CyclePlace     ()) m_iManNo = mcNoneCycle; } }
    else if (m_iManNo == mcETC_LDRULDPlace  ) { if(LDR.CyclePlace      ()) { if(ULD.CyclePlace     ()) m_iManNo = mcNoneCycle; } }
    else if (m_iManNo == mcETC_PSBPlace     ) { if(CyclePlaceStrp      ()) m_iManNo = mcNoneCycle; }


//    else if (m_iManNo == mcETC_RemoveStrip    ) { if(CycleStipRemove   () ){Trace("Manual Man","mcETC_RemoveStrip    End"); m_iManNo = mcNoneCycle; } }
//    else if (m_iManNo == mcETC_PreRemoveStrip ) { if(CyclePreStipRemove() ){Trace("Manual Man","mcETC_PreRemoveStrip End"); m_iManNo = mcNoneCycle; } }
//    else if (m_iManNo == mcETC_WrkRemoveStrip ) { if(CycleWrkStipRemove() ){Trace("Manual Man","mcETC_WrkRemoveStrip End"); m_iManNo = mcNoneCycle; } }
//    else if (m_iManNo == mcETC_PstRemoveStrip ) { if(CyclePstStipRemove() ){Trace("Manual Man","mcETC_PstRemoveStrip End"); m_iManNo = mcNoneCycle; } }

    else                                     {                          m_iManNo = mcNoneCycle; } //여기서 갱신됌.

    //Ok.
    return ;
}

//---------------------------------------------------------------------------

double CManualMan::GetMotrPosTable(int _iRow , int _iType)
{
    int    r;
    double dStartPos ;
    double dPos      ;

    r = _iRow ;//OM.DevOptn.iRowInspCnt ? _iRow/OM.DevOptn.iRowInspCnt * OM.DevOptn.iRowInspCnt : _iRow ;

    if(_iType == 0) dStartPos = PM.GetValue(miSTL_YTop , pvSTL_YTWorkStart) ;
    else            dStartPos = PM.GetValue(miSTL_YTop , pvSTL_YTDevCheck ) ;

    if(r >= OM.DevInfo.iRowGrCnt) dPos = dStartPos + (OM.DevInfo.dRowPitch * r) + (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch);
    else                          dPos = dStartPos + (OM.DevInfo.dRowPitch * r);

    return dPos;
}
bool CManualMan::CyclePlaceStrp()
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

        case  10: ULD.InitCycleStep();
                  PSB.InitCycleStep();
                  m_iManStep++;
                  return false;

        case  11: if(!ULD.CycleWork())return false;
                  if(!PSB.CycleIn()) return false;
                  PSB.InitCycleStep();

                  m_iManStep++;
                  return false;

        case  12: if(!PSB.CycleDrag())return false;
                  PSB.InitCycleStep();

                  m_iManStep++;
                  return false;

        case  13: if(!PSB.CycleOut())return false;
                  m_iManStep = 0;
                  return true;

        }
}
