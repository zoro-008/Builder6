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
#include "PreBuff.h"
#include "WorkZone.h"
#include "PostBuff.h"
#include "PstnMan.h"
//---------------------------------------------------------------------------
#include "FormDeviceSet.h"
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
    if ( EM_IsErr()                  ) { Init(); return false; }

    if(_iNo  == mcAllHome  && (IO_GetX(xPRB_1Pkg) || (IO_GetX(xPSB_2Pkg ) && OM.EqpOptn.bExistUnLoader) )) {FM_MsgOk("ERROR","Check Rail In or Out Sensor!"); return false ;}
    if(_iNo  == mcLDR_Home && (IO_GetX(xPRB_1Pkg) || IO_GetX(xPRB_2Pkg ))) {FM_MsgOk("ERROR","Check Rail In Sensor!"       ); return false ;}
//    if(_iNo  == mcULD_Home &&                          IO_GetX(xPSB_PkgOut2 ) ) {FM_MsgOk("ERROR","Check Rail In Sensor!"       ); return false ;}

    EM_SetDisp(true);

    double dPitch ;
    if(OM.DevOptn.iContColCnt == 2) dPitch = OM.DevInfo.dColPitch * 2 ;
    if(OM.DevOptn.iContColCnt == 4) dPitch = OM.DevInfo.dColPitch * 4 ;

    int  c,r ;
    int  iMaxRow,iMaxCol;
    bool bRet = true;


    m_iManNo = _iNo ;


    /********************/
    /* 프리프로세서     */
    /********************/
         if (m_iManNo == mcAllHome            ){     if(!IO_GetX(xETC_PowerOn )) {FM_MsgOk("ERR","Power On Plz"); bRet = false ;}
                                                else if(!IO_GetX(xLD1_MainAirCheck) && !IO_GetY(yETC_MainSol)) {FM_MsgOk("ERR","Air On Plz"); bRet = false ;}
//                                                else if(!IO_GetX(xETC_RMainAir)) {FM_MsgOk("ERR","Air On Plz"  ); bRet = false ;}
                                                //여기서 굳이 하는건 아닌거 같네...시퀀스에서 에러 띄우는게...
                                               }

    else if (m_iManNo == mcLDR_Home           ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;} }
    else if (m_iManNo == mcLDR_Supply         ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;} }
    else if (m_iManNo == mcLDR_Pick           ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
                                                if(!DM.ARAY[riLD1].CheckAllStat(csNone)       ){FM_MsgOk("ERR","Remove Mgz"          ); bRet = false ;} }
    else if (m_iManNo == mcLDR_SttPstn        ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
                                                if(!LD1.CheckSafe(miLD1_ZCmp,piLD1_ZWorkStart)){                                        bRet = false ;} }

    else if (m_iManNo == mcLDR_Place          ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;} }
    else if (m_iManNo == mcLDR_UpSlPitch      ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
                                                if(!LD1.CheckSafe(miLD1_ZCmp,piLD1_ZWork)     ){                                        bRet = false ;} }
    else if (m_iManNo == mcLDR_DnSlPitch      ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
                                                if(!LD1.CheckSafe(miLD1_ZCmp,piLD1_ZWork)     ){                                        bRet = false ;} }
    else if (m_iManNo == mcLDR_PusherFw       ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
                                                if(!LD1.CheckSafe(aiLD1_Cmp , ccFwd)          ){                                        bRet = false ;} }
    else if (m_iManNo == mcLDR_PusherBw       ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
                                                if(!LD1.CheckSafe(aiLD1_Cmp , ccBwd)          ){                                        bRet = false ;} }
    else if (m_iManNo == mcLDR_UnClamp        ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;} }
    else if (m_iManNo == mcLDR_Clamp          ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;} }
    else if (m_iManNo == mcLDR_TranInAC       ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;} }
    else if (m_iManNo == mcLDR_TranOutAC      ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;} }

    else if (m_iManNo == mcPRE_Home           ){}
    else if (m_iManNo == mcPRE_In             ){}
    else if (m_iManNo == mcPRE_Drag           ){}
    else if (m_iManNo == mcPRE_Out            ){}
    else if (m_iManNo == mcPRE_Clamp          ){if(AT_Complete(aiPRB_Cmp   , ccFwd)         ) { if(!PRB.CheckSafe(aiPRB_Cmp   , ccBwd)) bRet = false ;}
                                                if(AT_Complete(aiPRB_Cmp   , ccBwd)         ) { if(!PRB.CheckSafe(aiPRB_Cmp   , ccFwd)) bRet = false ;} }
    else if (m_iManNo == mcPRE_IDCyl          ){if(AT_Complete(aiPRB_IdChk , ccFwd)         ) { if(!PRB.CheckSafe(aiPRB_IdChk , ccBwd)) bRet = false ;}
                                                if(AT_Complete(aiPRB_IdChk , ccBwd)         ) { if(!PRB.CheckSafe(aiPRB_IdChk , ccFwd)) bRet = false ;} }
    else if (m_iManNo == mcPRE_Feeding        ){}

    else if (m_iManNo == mcWRK_Home           ){}
    else if (m_iManNo == mcWRK_PreWork        ){}
    else if (m_iManNo == mcWRK_R1Pitch        ){if(!WRK.CheckSafe(miWRK_XCmp , piWRK_XWork)   ){                                        bRet = false ;} }
    else if (m_iManNo == mcWRK_L1Pitch        ){if(!WRK.CheckSafe(miWRK_XCmp , piWRK_XWork)   ){                                        bRet = false ;} }
    else if (m_iManNo == mcWRK_PostWork       ){}

    else if (m_iManNo == mcWRK_Clamp          ){if(AT_Complete(aiWRK_Cmp   , ccFwd)         ) { if(!WRK.CheckSafe(aiWRK_Cmp   , ccBwd)) bRet = false ;}
                                                if(AT_Complete(aiWRK_Cmp   , ccBwd)         ) { if(!WRK.CheckSafe(aiWRK_Cmp   , ccFwd)) bRet = false ;} }
    else if (m_iManNo == mcWRK_ToolChng       ){}
    else if (m_iManNo == mcWRK_RptCntct       ){}

    else if (m_iManNo == mcPST_Home           ){}
    else if (m_iManNo == mcPST_Drag           ){}
    else if (m_iManNo == mcPST_Out            ){}
    else if (m_iManNo == mcPST_Clamp          ){if(AT_Complete(aiPSB_Cmp   , ccBwd)         ) { if(!PSB.CheckSafe(aiPSB_Cmp   , ccBwd)) bRet = false ;}
                                                if(AT_Complete(aiPSB_Cmp   , ccFwd)         ) { if(!PSB.CheckSafe(aiPSB_Cmp   , ccFwd)) bRet = false ;} }
    else if (m_iManNo == mcPST_4              ){}
    else if (m_iManNo == mcPST_5              ){}
    else if (m_iManNo == mcPST_6              ){}

    else if (m_iManNo == mcETC_AllClampOff    ){if(!PRB.CheckSafe(aiPRB_Cmp   , ccBwd)      )                                           bRet = false ;
                                                if(!PRB.CheckSafe(aiPRB_IdChk , ccFwd)      )                                           bRet = false ;
                                                if(!PRB.CheckSafe(aiWRK_Cmp   , ccBwd)      )                                           bRet = false ;
                                                if(!PRB.CheckSafe(aiPSB_Cmp   , ccBwd)      )                                           bRet = false ;  }



    else if (m_iManNo == mcETC_RemoveStrip    ){Trace("Manual Man","mcETC_RemoveStrip    Stt");}
    else if (m_iManNo == mcETC_PreRemoveStrip ){Trace("Manual Man","mcETC_PreRemoveStrip Stt");}
    else if (m_iManNo == mcETC_WrkRemoveStrip ){Trace("Manual Man","mcETC_WrkRemoveStrip Stt");}
    else if (m_iManNo == mcETC_PstRemoveStrip ){Trace("Manual Man","mcETC_PstRemoveStrip Stt");}
    else if (m_iManNo == mcETC_AirBlower      ){}
    else if (m_iManNo == mcETC_6              ){}

    else if (m_iManNo == mcETC_PrbStt         ){}
    else if (m_iManNo == mcETC_TPrbUp         ){}
    else if (m_iManNo == mcETC_TPrbDn         ){}
    else if (m_iManNo == mcETC_BPrbUp         ){}
    else if (m_iManNo == mcETC_BPrbDn         ){}

    if(!bRet) {
        Init();
        return false;
    }




    /********************/
    /* 처리..           */
    /********************/
    if      (m_iManNo == mcAllHome            ) { IO_SetY(yETC_MainSol,true);
                                                   if(OM.EqpOptn.bExistLoader) LD1.InitHomeStep ();
                                                                               PRB.InitHomeStep ();
                                                                               WRK.InitHomeStep ();
                                                                               PSB.InitHomeStep (); }

    else if (m_iManNo == mcLDR_Home           ) { LD1.InitHomeStep (); }
    else if (m_iManNo == mcLDR_Supply         ) { LD1.InitCycleStep(); }
    else if (m_iManNo == mcLDR_Pick           ) { LD1.InitCycleStep(); }
    else if (m_iManNo == mcLDR_SttPstn        ) { MT_GoAbsMan(miLD1_ZCmp,PM.GetValue(miLD1_ZCmp , pvLD1_ZWorkStartPs) ); }
    else if (m_iManNo == mcLDR_Place          ) { DM.ARAY[riLD1].SetStat(csEmpty); LD1.InitCycleStep(); }
    else if (m_iManNo == mcLDR_UpSlPitch      ) { MT_GoIncMan(miLD1_ZCmp ,-OM.DevInfo.dCsSlPitch);}
    else if (m_iManNo == mcLDR_DnSlPitch      ) { MT_GoIncMan(miLD1_ZCmp , OM.DevInfo.dCsSlPitch);}
    else if (m_iManNo == mcLDR_PusherFw       ) { m_iManStep = 10 ; m_iPreManStep = 0 ; }
    else if (m_iManNo == mcLDR_PusherBw       ) { m_iManStep = 10 ; m_iPreManStep = 0 ; }
    else if (m_iManNo == mcLDR_UnClamp        ) { AT_MoveCyl(aiLD1_Cmp , ccFwd); }
    else if (m_iManNo == mcLDR_Clamp          ) { AT_MoveCyl(aiLD1_Cmp , ccBwd); }
    else if (m_iManNo == mcLDR_TranInAC       ) { IO_SetY(yLD1_MgzInAC  , !IO_GetY(yLD1_MgzInAC ) ); }
    else if (m_iManNo == mcLDR_TranOutAC      ) { IO_SetY(yLD1_MgzOutAC , !IO_GetY(yLD1_MgzOutAC) ); }

    else if (m_iManNo == mcPRE_Home           ) { PRB.InitHomeStep (); }
    else if (m_iManNo == mcPRE_In             ) { PRB.InitCycleStep(); }
    else if (m_iManNo == mcPRE_Drag           ) { PRB.InitCycleStep(); }
    else if (m_iManNo == mcPRE_Out            ) { PRB.InitCycleStep(); }
    else if (m_iManNo == mcPRE_Clamp          ) { if(AT_Complete(aiPRB_Cmp   , ccFwd)) { AT_MoveCyl(aiPRB_Cmp   , ccBwd); }
                                                  if(AT_Complete(aiPRB_Cmp   , ccBwd)) { AT_MoveCyl(aiPRB_Cmp   , ccFwd); } }
    else if (m_iManNo == mcPRE_IDCyl          ) { if(AT_Complete(aiPRB_IdChk , ccFwd)) { AT_MoveCyl(aiPRB_IdChk , ccBwd); }
                                                  if(AT_Complete(aiPRB_IdChk , ccBwd)) { AT_MoveCyl(aiPRB_IdChk , ccFwd); } }
    else if (m_iManNo == mcPRE_Feeding        ) { IO_SetY(yPRB_FeedingAC , !IO_GetY(yPRB_FeedingAC ) ); }

    else if (m_iManNo == mcWRK_Home           ) { WRK.InitHomeStep (); }
    else if (m_iManNo == mcWRK_PreWork        ) { WRK.InitCycleStep(); }
    else if (m_iManNo == mcWRK_R1Pitch        ) { MT_GoIncMan(miWRK_XCmp, dPitch); }
    else if (m_iManNo == mcWRK_L1Pitch        ) { MT_GoIncMan(miWRK_XCmp,-dPitch); }
    else if (m_iManNo == mcWRK_PostWork       ) { WRK.InitCycleStep(); }
    else if (m_iManNo == mcWRK_Clamp          ) { if(AT_Complete(aiWRK_Cmp   , ccFwd)) { AT_MoveCyl(aiWRK_Cmp   , ccBwd); }
                                                  if(AT_Complete(aiWRK_Cmp   , ccBwd)) { AT_MoveCyl(aiWRK_Cmp   , ccFwd); } }
    else if (m_iManNo == mcWRK_ToolChng       ) { if(MT_CmprPos(miWRK_ZTop, 0.0)){
                                                      MT_GoAbsMan(miWRK_ZTop , PM.GetValue(miWRK_ZTop,pvWRK_ZTWaitPs));
                                                      MT_GoAbsMan(miWRK_ZBtm , PM.GetValue(miWRK_ZBtm,pvWRK_ZBWaitPs));
                                                      }
                                                  else {
                                                       MT_GoAbsMan(miWRK_ZTop , 0.0);
                                                       MT_GoAbsMan(miWRK_ZBtm , 0.0);
                                                  }
                                                }

    else if (m_iManNo == mcWRK_RptCntct       ){if(DM.ARAY[riWRK].CheckAllStat(csNone)) {m_iManNo = mcNoneCycle; return false;}
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

    else if (m_iManNo == mcPST_Home           ) { PSB.InitHomeStep (); }
    else if (m_iManNo == mcPST_Drag           ) { PSB.InitCycleStep(); }
    else if (m_iManNo == mcPST_Out            ) { PSB.InitCycleStep(); }
    else if (m_iManNo == mcPST_Clamp          ) { if(AT_Complete(aiPSB_Cmp   , ccFwd)) { AT_MoveCyl(aiPSB_Cmp , ccBwd); }
                                                  if(AT_Complete(aiPSB_Cmp   , ccBwd)) { AT_MoveCyl(aiPSB_Cmp , ccFwd); } }

    else if (m_iManNo == mcETC_AllClampOff    ) {AT_MoveCyl(aiPRB_Cmp   , ccBwd);
                                                 AT_MoveCyl(aiPRB_IdChk , ccFwd);
                                                 AT_MoveCyl(aiWRK_Cmp   , ccBwd);
                                                 AT_MoveCyl(aiPSB_Cmp   , ccBwd);
    }

//    else if (m_iManNo == mcETC_RemoveStrip    ) { m_iManStep = 10 ; m_iPreManStep = 0 ; }
//    else if (m_iManNo == mcETC_PreRemoveStrip ) { m_iManStep = 10 ; m_iPreManStep = 0 ; }
//    else if (m_iManNo == mcETC_WrkRemoveStrip ) { m_iManStep = 10 ; m_iPreManStep = 0 ; }
//    else if (m_iManNo == mcETC_PstRemoveStrip ) { m_iManStep = 10 ; m_iPreManStep = 0 ; }

    else if (m_iManNo == mcETC_AirBlower      ) {  IO_SetY(yETC_BlowerAC  , !IO_GetY(yETC_BlowerAC ) );  }

    else if (m_iManNo == mcETC_PrbStt         ) {
        AnsiString sMsg;
        sMsg = "START#";
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
    }

//    else if (m_iManNo == mcETC_TPrbUp         ) { FrmDeviceSet -> btWrk_TUp ->Click(); }
//    else if (m_iManNo == mcETC_TPrbDn         ) { FrmDeviceSet -> btWrk_TDn ->Click(); }
//    else if (m_iManNo == mcETC_BPrbUp         ) { FrmDeviceSet -> btWrk_BUp ->Click(); }
//    else if (m_iManNo == mcETC_BPrbDn         ) { FrmDeviceSet -> btWrk_BDn ->Click(); }


    else                                    { m_iManNo = mcNoneCycle ; }

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

    if(!OM.CmnOptn.bIgnrHeatAlm ) SEQ.InspectTemp    ();
    if(!OM.CmnOptn.bIgnrDoor    ) SEQ.InspectDoor    ();
    if(m_iManNo != mcLDR_Home &&
       m_iManNo != mcPRE_Home &&
       m_iManNo != mcWRK_Home &&
       m_iManNo != mcPST_Home &&
       m_iManNo != mcAllHome    ) SEQ.InspectHomeEnd ();
    SEQ.InspectLightGrid();

    bool r1,r2,r3,r4,r5 ;

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return ; }
//    if (!MT_GetServoAll()) { MT_SetServoAll(true);}

    //Cycle Step.
    if      (m_iManNo == mcAllHome         )  {if(OM.EqpOptn.bExistLoader) r1 = LD1.CycleHome();
                                               else r1 = true ;
                                                                           r2 = PRB.CycleHome();
                                                                           r3 = WRK.CycleHome();
                                                                           r4 = PSB.CycleHome();

                                                   if(r1&&r2&&r3&&r4) {
                                                       m_iManNo = mcNoneCycle;
                                                       FM_MsgOk("Confirm","All Home Finished!");
                                                   }
                                               }


    else if (m_iManNo == mcLDR_Home     && OM.EqpOptn.bExistLoader ) { if(LD1.CycleHome     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Supply   && OM.EqpOptn.bExistLoader ) { if(LD1.CycleSupply   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Pick     && OM.EqpOptn.bExistLoader ) { if(LD1.CyclePick     ()) m_iManNo = mcNoneCycle; }
//    else if (m_iManNo == mcLDR_SttPstn  && OM.EqpOptn.bExistLoader ) { if(LD1.CycleWork     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Place    && OM.EqpOptn.bExistLoader ) { if(LD1.CyclePlace    ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_PusherFw && OM.EqpOptn.bExistLoader ) { if(CyclePusherFw     () ){Trace("Manual Man","mcLDR_PusherFw End"); m_iManNo = mcNoneCycle; } }//if(IO_GetX(xLD1_PusherFw)) IO_SetY(yLD1_PusherFw , false); m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_PusherBw && OM.EqpOptn.bExistLoader ) { if(CyclePusherBw     () ){Trace("Manual Man","mcLDR_PusherBw End"); m_iManNo = mcNoneCycle; } }//if(IO_GetX(xLD1_PusherBw)) IO_SetY(yLD1_PusherBw , false); m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcPRE_Home        ) { if(PRB.CycleHome     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPRE_In          ) { if(PRB.CycleIn       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPRE_Drag        ) { if(PRB.CycleDrag     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPRE_Out         ) { if(PRB.CycleOut      ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcWRK_Home        ) { if(WRK.CycleHome     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcWRK_PreWork     ) { if(WRK.CyclePreWork  ()) m_iManNo = mcNoneCycle; }
//    else if (m_iManNo == mcWRK_Work        ) { if(WRK.CycleWork     ()) m_iManNo = mcNoneCycle; }
//    else if (m_iManNo == mcWRK_ReGrip      ) { if(WRK.CycleWkReGrip ()) m_iManNo = mcNoneCycle; }
//    else if (m_iManNo == mcWRK_ReWork      ) { if(WRK.CycleReWork   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcWRK_PostWork    ) { if(WRK.CyclePostWork ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcWRK_RptCntct    ) { if(WRK.CycleWork     ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcPST_Home        ) { if(PSB.CycleHome     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPST_Drag        ) { if(PSB.CycleDrag     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPST_Out         ) { if(PSB.CycleOut      ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcETC_RemoveStrip    ) { if(CycleStipRemove   () ){Trace("Manual Man","mcETC_RemoveStrip    End"); m_iManNo = mcNoneCycle; } }
    else if (m_iManNo == mcETC_PreRemoveStrip ) { if(CyclePreStipRemove() ){Trace("Manual Man","mcETC_PreRemoveStrip End"); m_iManNo = mcNoneCycle; } }
    else if (m_iManNo == mcETC_WrkRemoveStrip ) { if(CycleWrkStipRemove() ){Trace("Manual Man","mcETC_WrkRemoveStrip End"); m_iManNo = mcNoneCycle; } }
    else if (m_iManNo == mcETC_PstRemoveStrip ) { if(CyclePstStipRemove() ){Trace("Manual Man","mcETC_PstRemoveStrip End"); m_iManNo = mcNoneCycle; } }

    else                                     {                          m_iManNo = mcNoneCycle; }

    //Ok.
    return ;
}

//---------------------------------------------------------------------------
bool CManualMan::CyclePusherFw()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiMNR_StripRemoveTo);
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

        case 10: IO_SetY(yLD1_PusherBw,false);
                 IO_SetY(yLD1_PusherFw,true );
                 m_tmTemp.Clear();
                 m_iManStep++;
                 return false ;

        case 11: if(!IO_GetX(xLD1_PusherFw) && !m_tmTemp.OnDelay(true,2000) ) return false ;
//                 IO_SetY(yLD1_PusherBw,false);
                 IO_SetY(yLD1_PusherFw,false);
                 m_iManStep = 0;
                 return true ;

    }
}
bool CManualMan::CyclePusherBw()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiMNR_StripRemoveTo);
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

        case 10: IO_SetY(yLD1_PusherBw,true );
                 IO_SetY(yLD1_PusherFw,false);
                 m_tmTemp.Clear();
                 m_iManStep++;
                 return false ;

        case 11: if(!IO_GetX(xLD1_PusherBw) && !m_tmTemp.OnDelay(true,2000) ) return false ;
                 IO_SetY(yLD1_PusherBw,false);
//                 IO_SetY(yLD1_PusherFw,false);
                 m_iManStep = 0;
                 return true ;

    }
}
bool CManualMan::CycleStipRemove()
{
/*    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiMNR_StipRemoveTo);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        m_iManStep = 0 ;
        return true;
    }

    if(m_iManStep != m_iPreManStep) {
        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    m_iPreManStep = m_iManStep ;

    switch (m_iManStep) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 m_iManStep = 0 ;
                 return true ;

        case 10: AT_MoveCyl(aiPRB_Cmp  ,false);
                 AT_MoveCyl(aiWRK_Cmp  ,false);
                 AT_MoveCyl(aiPSB_Cmp  ,false);
                 AT_MoveCyl(aiPRB_IdChk,true );
                 m_iManStep++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPRB_Cmp  ,false))return false ;
                 if(!AT_MoveCyl(aiWRK_Cmp  ,false))return false ;
                 if(!AT_MoveCyl(aiPSB_Cmp  ,false))return false ;
                 if(!AT_MoveCyl(aiPRB_IdChk,true ))return false ;
                 MT_GoAbsMan(miPRB_XCmp,PM.GetValue(miPRB_XCmp,0));
                 MT_GoAbsMan(miWRK_XCmp,PM.GetValue(miWRK_XCmp,0));
                 MT_GoAbsMan(miPSB_XCmp,PM.GetValue(miPSB_XCmp,0));
                 m_iManStep++;
                 return false ;

        case 12: if(!MT_GoAbsMan(miPRB_XCmp,PM.GetValue(miPRB_XCmp,0)))return false;
                 if(!MT_GoAbsMan(miWRK_XCmp,PM.GetValue(miWRK_XCmp,0)))return false;
                 if(!MT_GoAbsMan(miPSB_XCmp,PM.GetValue(miPSB_XCmp,0)))return false;
                 AT_MoveCyl(aiPSB_Cmp,true);
                 m_iManStep++;
                 return false ;

        case 13: if(!AT_MoveCyl(aiPSB_Cmp,true))return false ;
                 MT_GoAbsMan(miPSB_XCmp,PM.GetValue(miPSB_XCmp,pvPSB_XPullPs));
                 AT_MoveCyl(aiWRK_Cmp,true);
                 m_iManStep++;
                 return false ;

        case 14: if(!MT_GoAbsMan(miPSB_XCmp,PM.GetValue(miPSB_XCmp,pvPSB_XPullPs)))return false ;
                 if(!AT_MoveCyl(aiPSB_Cmp,true))return false ;
                 AT_MoveCyl(aiPSB_Cmp,false);
                 AT_MoveCyl(aiWRK_Cmp,true );
                 m_iManStep++;
                 return false ;

        case 14: if(!AT_MoveCyl(aiPSB_Cmp,false)) return false;
                 if(!AT_MoveCyl(aiWRK_Cmp,true )) return false;
                 MT_GoAbsMan(miPSB_XCmp,PM.GetValue(miPSB_XCmp,pvPSB_XBackPs));
                 MT_GoAbsMan(miWRK_XCmp,PM.GetValue(miWRK_XCmp,pvWRK_XOutPs));
                 m_iManStep++;
                 return false ;

        case 14: if(!MT_GoAbsMan(miPSB_XCmp,PM.GetValue(miPSB_XCmp,pvPSB_XBackPs))) return false;
                 if(!MT_GoAbsMan(miWRK_XCmp,PM.GetValue(miWRK_XCmp,pvWRK_XOutPs ))) return false;
                 AT_MoveCyl(aiPSB_Cmp,true);
                 AT_MoveCyl(aiWRK_Cmp,true);


                 m_iManStep++;
                 return false ;

                 MT_GoAbsMan(miPSB_XCmp,PM.GetValue(miPSB_XCmp,pvPSB_XBackPs));
                 MT_GoAbsMan(miWRK_XCmp,PM.GetValue(miWRK_XCmp,pvWRK_XOutPs ));
                 m_iManStep++;
                 return false ;

        case 15: if(!MT_GoAbsMan(miPSB_XCmp,PM.GetValue(miPSB_XCmp,pvPSB_XBackPs)))return false ;
                 if(!MT_GoAbsMan(miWRK_XCmp,PM.GetValue(miWRK_XCmp,pvWRK_XOutPs )))return false ;
                 AT_MoveCyl(aiWRK_Cmp,true));
                 m_iManStep++;
                 return false ;

        case 16:














                 AT_MoveCyl(aiPSB_Cmp,false);
                 AT_MoveCyl(aiWRK_Cmp,true);
                 m_iManStep++;
                 return false ;

        case 15: if(!AT_MoveCyl(aiPSB_Cmp,false))return false;
                 if(!AT_MoveCyl(aiWRK_Cmp,true ))return false;
                 MT_GoAbsMan(miWRK_XCmp,PM.GetValue(miWRK_XCmp,pvWRK_XOutPs));
                 MT_GoAbsMan(miPSB_XCmp,0                                   );
                 m_iManStep++;
                 return false ;

        case 16: if(!MT_GoAbsMan(miWRK_XCmp,PM.GetValue(miWRK_XCmp,pvWRK_XOutPs)))return false ;
                 if(!MT_GoAbsMan(miPSB_XCmp,0                                   ))return false ;
                 AT_MoveCyl(aiWRK_Cmp,false);
                 AT_MoveCyl(aiPRB_Cmp,true );
                 m_iManStep++;
                 return false ;

        case 17: if(!AT_MoveCyl(aiWRK_Cmp,false))return false ;
                 if(!AT_MoveCyl(aiPRB_Cmp,true ))return false ;
                 MT_GoAbsMan(miWRK_XCmp,0                                    );
                 MT_GoAbsMan(miPRB_XCmp,PM.GetValue(miPRB_XCmp,pvPRB_XOut2Ps));
                 m_iManStep++;
                 return false ;

        case 18: if(!MT_GoAbsMan(miWRK_XCmp,0                                    ))return false ;
                 if(!MT_GoAbsMan(miPRB_XCmp,PM.GetValue(miPRB_XCmp,pvPRB_XOut2Ps)))return false ;
                 AT_MoveCyl(aiPRB_Cmp,false );
                 m_iManStep++;
                 return false ;

        case 19: if(!AT_MoveCyl(aiWRK_Cmp,false))return false ;
                 m_iManStep = 0;
                 return true ;

    }
*/
}

bool CManualMan::CyclePreStipRemove()
{
/*
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiMNR_StripRemoveTo);
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

        case 10: AT_MoveCyl (aiPRB_Cmp ,false);
                 m_iManStep++;
                 return false ;

        case 11: if(!AT_MoveCyl (aiPRB_Cmp ,false)) return false;
                 MT_GoAbsMan(miPRB_XCmp,PM.GetValue(miPRB_XCmp,pvPRB_XWaitPs));
                 m_iManStep++;
                 return false ;

        case 12: if(!MT_GoAbsMan(miPRB_XCmp,PM.GetValue(miPRB_XCmp,pvPRB_XWaitPs))) return false;
                 AT_MoveCyl (aiPRB_Cmp ,true);
                 m_iManStep++;
                 return false ;

        case 13: if(!AT_MoveCyl (aiPRB_Cmp ,true)) return false;
                 MT_GoAbsMan(miPRB_XCmp,PM.GetValue(miPRB_XCmp,pvPRB_XPullPs));
                 m_iManStep++;
                 return false ;

        case 14: if(!MT_GoAbsMan(miPRB_XCmp,PM.GetValue(miPRB_XCmp,pvPRB_XPullPs))) return false;
                 AT_MoveCyl (aiPRB_Cmp ,false);
                 m_iManStep++;
                 return false ;

        case 15: if(!AT_MoveCyl (aiPRB_Cmp ,false)) return false;
                 MT_GoAbsMan(miPRB_XCmp,PM.GetValue(miPRB_XCmp,pvPRB_XBackPs));
                 m_iManStep++;
                 return false ;

        case 16: if(!MT_GoAbsMan(miPRB_XCmp,PM.GetValue(miPRB_XCmp,pvPRB_XBackPs))) return false;
                 AT_MoveCyl (aiPRB_Cmp ,true);
                 m_iManStep++;
                 return false ;

        case 17: if(!AT_MoveCyl (aiPRB_Cmp ,true)) return false;
                 MT_GoAbsMan(miPRB_XCmp,PM.GetValue(miPRB_XCmp,pvPRB_XOut2Ps));
                 m_iManStep++;
                 return false ;

        case 18: if(!MT_GoAbsMan(miPRB_XCmp,PM.GetValue(miPRB_XCmp,pvPRB_XOut2Ps))) return false;
                 AT_MoveCyl(aiPRB_Cmp,false);
                 m_iManStep++;
                 return false ;

        case 19: if(!AT_MoveCyl(aiPRB_Cmp,false)) return false;
                 m_iManStep = 0;
                 return true ;
    }
*/
}


bool CManualMan::CycleWrkStipRemove()
{
/*
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiMNR_StripRemoveTo);
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

        case 10: AT_MoveCyl (aiWRK_Cmp ,false);
                 MT_GoAbsMan(miWRK_ZBtm,PM.GetValue(miWRK_ZBtm,pvWRK_ZBWaitPs));
                 MT_GoAbsMan(miWRK_ZTop,PM.GetValue(miWRK_ZTop,pvWRK_ZTWaitPs));
                 m_iManStep++;
                 return false ;

        case 11: if(!AT_MoveCyl (aiWRK_Cmp ,false)) return false;
                 if(!MT_GoAbsMan(miWRK_ZBtm,PM.GetValue(miWRK_ZBtm,pvWRK_ZBWaitPs))) return false;
                 if(!MT_GoAbsMan(miWRK_ZTop,PM.GetValue(miWRK_ZTop,pvWRK_ZTWaitPs))) return false;
                 MT_GoAbsMan(miWRK_XCmp,PM.GetValue(miWRK_XCmp,pvWRK_XWaitPs));
                 m_iManStep++;
                 return false ;

        case 12: if(!MT_GoAbsMan(miWRK_XCmp,PM.GetValue(miWRK_XCmp,pvWRK_XWaitPs))) return false;
                 AT_MoveCyl (aiWRK_Cmp ,true);
                 m_iManStep++;
                 return false ;

        case 13: if(!AT_MoveCyl (aiWRK_Cmp ,true)) return false;
                 MT_GoAbsMan(miWRK_XCmp,PM.GetValue(miWRK_XCmp,pvWRK_XOutPs));
                 m_iManStep++;
                 return false ;

        case 14: if(!MT_GoAbsMan(miWRK_XCmp,PM.GetValue(miWRK_XCmp,pvWRK_XOutPs))) return false;
                 AT_MoveCyl (aiWRK_Cmp ,false);
                 m_iManStep++;
                 return false ;

        case 15: if(!AT_MoveCyl (aiWRK_Cmp ,false)) return false;
                 MT_GoAbsMan(miWRK_XCmp,PM.GetValue(miWRK_XCmp,pvWRK_XWaitPs));
                 m_iManStep++;
                 return false ;

        case 16: if(!MT_GoAbsMan(miWRK_XCmp,PM.GetValue(miWRK_XCmp,pvWRK_XWaitPs))) return false;
                 AT_MoveCyl (aiWRK_Cmp ,true);
                 m_iManStep++;
                 return false ;

        case 17: if(!AT_MoveCyl (aiWRK_Cmp ,true)) return false;
                 MT_GoAbsMan(miWRK_XCmp,PM.GetValue(miWRK_XCmp,pvWRK_XOutPs));
                 m_iManStep++;
                 return false ;

        case 18: if(!MT_GoAbsMan(miWRK_XCmp,PM.GetValue(miWRK_XCmp,pvWRK_XOutPs))) return false;
                 AT_MoveCyl(aiWRK_Cmp,false);
                 m_iManStep++;
                 return false ;

        case 19: if(!AT_MoveCyl(aiWRK_Cmp,false)) return false;
                 m_iManStep = 0;
                 return true ;
    }

*/
}

bool CManualMan::CyclePstStipRemove()
{
/*
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiMNR_StripRemoveTo);
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

        case 10: AT_MoveCyl (aiPSB_Cmp ,false);
                 m_iManStep++;
                 return false ;

        case 11: if(!AT_MoveCyl (aiPSB_Cmp ,false)) return false;
                 MT_GoAbsMan(miPSB_XCmp,PM.GetValue(miPSB_XCmp,pvPSB_XWaitPs));
                 m_iManStep++;
                 return false ;

        case 12: if(!MT_GoAbsMan(miPSB_XCmp,PM.GetValue(miPSB_XCmp,pvPSB_XWaitPs))) return false;
                 AT_MoveCyl (aiPSB_Cmp ,true);
                 m_iManStep++;
                 return false ;

        case 13: if(!AT_MoveCyl (aiPSB_Cmp ,true)) return false;
                 MT_GoAbsMan(miPSB_XCmp,PM.GetValue(miPSB_XCmp,pvPSB_XPullPs));
                 m_iManStep++;
                 return false ;

        case 14: if(!MT_GoAbsMan(miPSB_XCmp,PM.GetValue(miPSB_XCmp,pvPSB_XPullPs))) return false;
                 AT_MoveCyl (aiPSB_Cmp ,false);
                 m_iManStep++;
                 return false ;

        case 15: if(!AT_MoveCyl (aiPSB_Cmp ,false)) return false;
                 MT_GoAbsMan(miPSB_XCmp,PM.GetValue(miPSB_XCmp,pvPSB_XBackPs));
                 m_iManStep++;
                 return false ;

        case 16: if(!MT_GoAbsMan(miPSB_XCmp,PM.GetValue(miPSB_XCmp,pvPSB_XBackPs))) return false;
                 AT_MoveCyl (aiPSB_Cmp ,true);
                 m_iManStep++;
                 return false ;

        case 17: if(!AT_MoveCyl (aiPSB_Cmp ,true)) return false;
                 MT_GoAbsMan(miPSB_XCmp,PM.GetValue(miPSB_XCmp,pvPSB_XOutPs));
                 m_iManStep++;
                 return false ;

        case 18: if(!MT_GoAbsMan(miPSB_XCmp,PM.GetValue(miPSB_XCmp,pvPSB_XOutPs))) return false;
                 AT_MoveCyl(aiPSB_Cmp,false);
                 m_iManStep++;
                 return false ;

        case 19: if(!AT_MoveCyl(aiPSB_Cmp,false)) return false;
                 m_iManStep = 0;
                 return true ;
    }
*/
}
