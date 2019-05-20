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
#include "SortingTool.h"
#include "ULD_HRT430S.h"
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

    if(_iNo  == mcAllHome  && (IO_GetX(xPRB_1Pkg) || (IO_GetX(xPSB_PkgOut2 ) && OM.EqpOptn.bExistUnLoader) )) {FM_MsgOk("ERROR","Check Rail In or Out Sensor!"); return false ;}
    if(_iNo  == mcAllHome  && (!IO_GetX(xETC_MainSol) || !IO_GetY(yETC_MainSol) || !IO_GetY(yETC_MainSol2) )) {FM_MsgOk("ERROR","Check Main Air!"); return false ;}
    if(_iNo  == mcLDR_Home && (IO_GetX(xPRB_1Pkg) /*|| IO_GetX(xPRB_2Pkg )*/)) {FM_MsgOk("ERROR","Check Rail In Sensor!"       ); return false ;}

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
                                                else if(!IO_GetX(xETC_MainSol) || !IO_GetY(yETC_MainSol) || !IO_GetY(yETC_MainSol2) ) {FM_MsgOk("ERR","Air On Plz"); bRet = false ;}
                                                //여기서 굳이 하는건 아닌거 같네...시퀀스에서 에러 띄우는게... ?
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
                                                if(!LD1.CheckSafe(aiLD1_Psh , ccFwd)          ){                                        bRet = false ;} }
    else if (m_iManNo == mcLDR_PusherBw       ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
                                                if(!LD1.CheckSafe(aiLD1_Psh , ccBwd)          ){                                        bRet = false ;} }
    else if (m_iManNo == mcLDR_UnClamp        ){
        if(!OM.EqpOptn.bExistLoader                   ){
            FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;
        }
        if(IO_GetX(xLD1_MgzDetect1) || IO_GetX(xLD1_MgzDetect2))
            if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Sure ?") != mrYes )
                bRet = false;

        if(!LD1.CheckSafe(aiLD1_Cmp , ccFwd)          ){
            bRet = false ;
        }

    }

    /*
    if(IO_GetX(xLD1_MgzDetect1) || IO_GetX(xLD1_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Sure ?") != mrYes ) return ;
        bRet = LD1.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;

    */
    else if (m_iManNo == mcLDR_Clamp          ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
                                                if(!LD1.CheckSafe(aiLD1_Cmp , ccFwd)) bRet = false ;
                                               }
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
    else if (m_iManNo == mcWRK_RptCntct       ){}

    else if (m_iManNo == mcPSB_Home           ){}
    else if (m_iManNo == mcPSB_Drag           ){}
    else if (m_iManNo == mcPSB_Out            ){}
    else if (m_iManNo == mcPSB_Clamp          ){if(AT_Complete(aiPSB_Cmp   , ccBwd)         ) { if(!PSB.CheckSafe(aiPSB_Cmp   , ccBwd)) bRet = false ;}
                                                if(AT_Complete(aiPSB_Cmp   , ccFwd)         ) { if(!PSB.CheckSafe(aiPSB_Cmp   , ccFwd)) bRet = false ;} }
    else if (m_iManNo == mcPSB_PusherFw       ){if(!OM.EqpOptn.bExistUnLoader               ){FM_MsgOk("ERR","Post Buff doesn't Exist"); bRet = false ;}
                                                if(!PSB.CheckSafe(aiPSB_Psh , ccFwd)        ){                                           bRet = false ;} }
    else if (m_iManNo == mcPSB_PusherBw       ){if(!OM.EqpOptn.bExistUnLoader               ){FM_MsgOk("ERR","Post Buff doesn't Exist"); bRet = false ;}
                                                if(!PSB.CheckSafe(aiPSB_Psh , ccBwd)        ){                                           bRet = false ;} }
    else if (m_iManNo == mcSTL_YTopClamp      ){if(AT_Complete(aiSTL_Top   , ccFwd)         ) { if(!STL.CheckSafe(aiSTL_Top   , ccBwd)) bRet = false ;}
                                                if(AT_Complete(aiSTL_Top   , ccBwd)         ) { if(!STL.CheckSafe(aiSTL_Top   , ccFwd)) bRet = false ;} }
    else if (m_iManNo == mcSTL_YBtmClamp      ){if(AT_Complete(aiSTL_Btm   , ccFwd)         ) { if(!STL.CheckSafe(aiSTL_Btm   , ccBwd)) bRet = false ;}
                                                if(AT_Complete(aiSTL_Btm   , ccBwd)         ) { if(!STL.CheckSafe(aiSTL_Btm   , ccFwd)) bRet = false ;} }
    else if (m_iManNo == mcPSB_Feeding        ){}
    else if (m_iManNo == mcPSB_R1Pitch        ){if(!PSB.CheckSafe(miPSB_XCmp , piPSB_XWork) ){                                          bRet = false ;} }
    else if (m_iManNo == mcPSB_L1Pitch        ){if(!PSB.CheckSafe(miPSB_XCmp , piPSB_XWork) ){                                          bRet = false ;} }
    else if (m_iManNo == mcSTL_YTopUp1Pitch   ){if(!STL.CheckSafe(miSTL_YTop , piSTL_YTWork)){                                          bRet = false ;} }
    else if (m_iManNo == mcSTL_YTopDn1Pitch   ){if(!STL.CheckSafe(miSTL_YTop , piSTL_YTWork)){                                          bRet = false ;} }
    else if (m_iManNo == mcSTL_YBtmUp1pitch   ){if(!STL.CheckSafe(miSTL_YBtm , piSTL_YBWork)){                                          bRet = false ;} }
    else if (m_iManNo == mcSTL_YBtmDn1pitch   ){if(!STL.CheckSafe(miSTL_YBtm , piSTL_YBWork)){                                          bRet = false ;} }

    else if (m_iManNo == mcETC_AllClampOff    ){if(!PRB.CheckSafe(aiPRB_Cmp   , ccBwd)      )                                           bRet = false ;
                                                if(!PRB.CheckSafe(aiPRB_IdChk , ccFwd)      )                                           bRet = false ;
                                                if(!PRB.CheckSafe(aiWRK_Cmp   , ccBwd)      )                                           bRet = false ;
                                                if(!PRB.CheckSafe(aiPSB_Cmp   , ccBwd)      )                                           bRet = false ;  }

//    else if (m_iManNo == mcETC_RemoveStrip    ){Trace("Manual Man","mcETC_RemoveStrip    Stt");}
//    else if (m_iManNo == mcETC_PreRemoveStrip ){Trace("Manual Man","mcETC_PreRemoveStrip Stt");}
//    else if (m_iManNo == mcETC_WrkRemoveStrip ){Trace("Manual Man","mcETC_WrkRemoveStrip Stt");}
//    else if (m_iManNo == mcETC_PstRemoveStrip ){Trace("Manual Man","mcETC_PstRemoveStrip Stt");}
    else if (m_iManNo == mcETC_AirBlower      ){}
    else if (m_iManNo == mcETC_6              ){}

    else if (m_iManNo == mcETC_PrbStt         ){}
    else if (m_iManNo == mcETC_TPrbUp         ){}
    else if (m_iManNo == mcETC_TPrbDn         ){}
    else if (m_iManNo == mcETC_BPrbUp         ){}
    else if (m_iManNo == mcETC_BPrbDn         ){}

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

    else if (m_iManNo == mcSTL_Pick           ){if( DM.ARAY[riSTL].GetCntStat  (csNone))                                                bRet = false ;}
    else if (m_iManNo == mcSTL_Place          ){if( DM.ARAY[riSTL].GetCntStat  (csFail))                                                bRet = false ;}



    if(!bRet) Init();



    /********************/
    /* 처리..           */
    /********************/
    if      (m_iManNo == mcAllHome            ) { //IO_SetY(yETC_MainSol ,true); 이러면 빠져나가는 상황이 있어서 그냥 윗단에서
                                                  //IO_SetY(yETC_MainSol2,true); 에러를 잡는다.
                                                   if(OM.EqpOptn.bExistLoader) LD1.InitHomeStep ();
                                                                               PRB.InitHomeStep ();
                                                                               WRK.InitHomeStep ();
                                                                               PSB.InitHomeStep ();
                                                                               STL.InitHomeStep ();
                                                                               ULD.InitHomeStep (); }

    else if (m_iManNo == mcLDR_Home           ) { LD1.InitHomeStep (); }
    else if (m_iManNo == mcLDR_Supply         ) { LD1.InitCycleStep(); }
    else if (m_iManNo == mcLDR_Pick           ) { LD1.InitCycleStep(); }
    else if (m_iManNo == mcLDR_SttPstn        ) { MT_GoAbsMan(miLD1_ZCmp,PM.GetValue(miLD1_ZCmp , pvLD1_ZWorkStartPs) ); }
    else if (m_iManNo == mcLDR_Place          ) { DM.ARAY[riLD1].SetStat(csEmpty); LD1.InitCycleStep(); }
    else if (m_iManNo == mcLDR_UpSlPitch      ) { MT_GoIncMan(miLD1_ZCmp ,-OM.DevInfo.dCsSlPitch);}
    else if (m_iManNo == mcLDR_DnSlPitch      ) { MT_GoIncMan(miLD1_ZCmp , OM.DevInfo.dCsSlPitch);}
    else if (m_iManNo == mcLDR_PusherFw       ) { AT_MoveCyl(aiLD1_Psh , ccFwd); }
    else if (m_iManNo == mcLDR_PusherBw       ) { AT_MoveCyl(aiLD1_Psh , ccBwd); }
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
    else if (m_iManNo == mcWRK_R1Pitch        ) { MT_GoIncMan(miWRK_XCmp, dWorkPitch); }
    else if (m_iManNo == mcWRK_L1Pitch        ) { MT_GoIncMan(miWRK_XCmp,-dWorkPitch); }
    else if (m_iManNo == mcWRK_PostWork       ) { WRK.InitCycleStep(); }
    else if (m_iManNo == mcWRK_Clamp          ) { if(AT_Complete(aiWRK_Cmp   , ccFwd)) { AT_MoveCyl(aiWRK_Cmp   , ccBwd); }
                                                  if(AT_Complete(aiWRK_Cmp   , ccBwd)) { AT_MoveCyl(aiWRK_Cmp   , ccFwd); } }
    else if (m_iManNo == mcWRK_ToolChng       ) { if(MT_CmprPos(miWRK_ZTop, 0.0) && MT_CmprPos(miSTL_YBtm, 0.0) && MT_CmprPos(miSTL_YTop, 50.0)){
                                                      MT_GoAbsMan(miWRK_ZTop , PM.GetValue(miWRK_ZTop,pvWRK_ZTWaitPs));
                                                      MT_GoAbsMan(miWRK_ZBtm , PM.GetValue(miWRK_ZBtm,pvWRK_ZBWaitPs));
                                                      MT_GoAbsMan(miWRK_ZBtm , PM.GetValue(miSTL_YTop,pvWRK_ZBWaitPs));
                                                      MT_GoAbsMan(miWRK_ZBtm , PM.GetValue(miSTL_YBtm,pvWRK_ZBWaitPs));
                                                      }
                                                  else {
                                                       MT_GoAbsMan(miWRK_ZTop , 0.0 );
                                                       MT_GoAbsMan(miWRK_ZBtm , 40.0);
                                                       MT_GoAbsMan(miSTL_YTop , 50.0);
                                                       MT_GoAbsMan(miSTL_YBtm ,  0.0);
                                                  }
                                                }
                                                 //하바텍
    else if (m_iManNo == mcWRK_RptCntct       ){//if(DM.ARAY[riWRK].CheckAllStat(csNone)) {m_iManNo = mcNoneCycle; m_bManSetting = false ; return false; } //m_bManSetting 중요함 리턴전에 꼭 펄스 시켜야함. 쓰레드가 달라서. ::Update에서 m_iManNo=0이 되므로 주의.
                                                //iMaxRow = DM.ARAY[riWRK].GetMaxRow();
                                                //if(DM.ARAY[riWRK].GetCntStat(csWork) || DM.ARAY[riWRK].GetCntStat(csFail)) {
                                                //    c = DM.ARAY[riWRK].FindLastCol(csUnkwn) ;
                                                //    if(c > -1) {
                                                //        for(int j = 0 ; j < OM.DevOptn.iContColCnt ;j++) {
                                                //            for(int i =0 ; i < iMaxRow ; i++) DM.ARAY[riWRK].SetStat(i ,c - j, csUnkwn) ;
                                                //        }
                                                //    }
                                                //}

                                                CWorkZone::SStat Stat = WRK.GetStat(); Stat.bRepeat = true ; WRK.SetStat(Stat);
                                                WRK.InitCycleStep();}

    else if (m_iManNo == mcPSB_Home           ) { PSB.InitHomeStep (); STL.InitHomeStep(); }
    else if (m_iManNo == mcPSB_Drag           ) { PSB.InitCycleStep(); }
    else if (m_iManNo == mcPSB_Out            ) { PSB.InitCycleStep(); }
    else if (m_iManNo == mcPSB_Clamp          ) { if(AT_Complete(aiPSB_Cmp   , ccFwd)) { AT_MoveCyl(aiPSB_Cmp , ccBwd); }
                                                  if(AT_Complete(aiPSB_Cmp   , ccBwd)) { AT_MoveCyl(aiPSB_Cmp , ccFwd); } }

    else if (m_iManNo == mcSTL_YTopClamp      ) { if(AT_Complete(aiSTL_Top   , ccFwd)) { AT_MoveCyl(aiSTL_Top , ccBwd); }
                                                  if(AT_Complete(aiSTL_Top   , ccBwd)) { AT_MoveCyl(aiSTL_Top , ccFwd); } }
    else if (m_iManNo == mcSTL_YBtmClamp      ) { if(AT_Complete(aiSTL_Btm   , ccFwd)) { AT_MoveCyl(aiSTL_Btm , ccBwd); }
                                                  if(AT_Complete(aiSTL_Btm   , ccBwd)) { AT_MoveCyl(aiSTL_Btm , ccFwd); } }
    else if (m_iManNo == mcPSB_Feeding        ) { IO_SetY(yPSB_FeedingAC , !IO_GetY(yPSB_FeedingAC ) ); }
    else if (m_iManNo == mcPSB_R1Pitch        ) { MT_GoIncMan(miPSB_XCmp, dTrimPitch); }
    else if (m_iManNo == mcPSB_L1Pitch        ) { MT_GoIncMan(miPSB_XCmp,-dTrimPitch); }
    else if (m_iManNo == mcPSB_PusherFw       ) { AT_MoveCyl(aiPSB_Psh , ccFwd); }
    else if (m_iManNo == mcPSB_PusherBw       ) { AT_MoveCyl(aiPSB_Psh , ccBwd); }

    else if (m_iManNo == mcSTL_YTopUp1Pitch   ) { MT_GoIncMan(miSTL_YTop , OM.DevInfo.dRowPitch) ;}//STL.MoveMotr(miSTL_YTop, piSTL_YTStep); } //몬가좀 아니다...
    else if (m_iManNo == mcSTL_YTopDn1Pitch   ) { MT_GoIncMan(miSTL_YTop ,-OM.DevInfo.dRowPitch) ;}//STL.MoveMotr(miSTL_YTop, piSTL_YTStep); }
    else if (m_iManNo == mcSTL_YBtmUp1pitch   ) { MT_GoIncMan(miSTL_YBtm , OM.DevInfo.dRowPitch) ;}//STL.MoveMotr(miSTL_YBtm, piSTL_YBStep); }
    else if (m_iManNo == mcSTL_YBtmDn1pitch   ) { MT_GoIncMan(miSTL_YBtm ,-OM.DevInfo.dRowPitch) ;}//STL.MoveMotr(miSTL_YBtm, piSTL_YBStep); }



/*
    else if (m_iManNo == mcSTL_YTopUp1Pitch   ) {
        double dCrntPos     = MT_GetCmdPos(miSTL_YTop) ;
        double dTargetPos   ;
        double dTemp =  dCrntPos - PM.GetValue (miSTL_YTop , pvSTL_YTWorkStartPs) ;
        double dCrntRow     = dTemp/OM.DevInfo.dRowPitch ;
        int    iCrntRow     = (int)dCrntRow ;
        bool   bUpOnTheRail = PM.GetValue (miSTL_YTop , pvSTL_YTWorkStartPs) <= dCrntPos ;

        if( dCrntRow < OM.DevInfo.iRowCnt-1) { //범위.
            dTargetPos = PM.GetValue (miSTL_YTop , pvSTL_YTWorkStartPs) + (dCrntRow+1) * OM.DevInfo.dRowPitch  ;
            if((dCrntRow+1)/OM.DevInfo.iRowGrCnt)
                dTargetPos += ((iCrntRow+1)/OM.DevInfo.iRowGrCnt) *(OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) ;
            MT_GoAbsMan(miSTL_YTop , dTargetPos) ;
        }
    }
    else if (m_iManNo == mcSTL_YTopDn1Pitch   ) {
        double dCrntPos     = MT_GetCmdPos(miSTL_YTop) ;
        double dTargetPos   ;
        double dTemp =  dCrntPos - PM.GetValue (miSTL_YTop , pvSTL_YTWorkStartPs) ;
        double dCrntRow     = dTemp/OM.DevInfo.dRowPitch ;
        int    iCrntRow     = (int)dCrntRow ;
        bool   bUpOnTheRail = PM.GetValue (miSTL_YTop , pvSTL_YTWorkStartPs) <= dCrntPos ;

        if( dCrntRow > 1) { //범위.
            dTargetPos = PM.GetValue (miSTL_YTop , pvSTL_YTWorkStartPs) + (dCrntRow-1) * OM.DevInfo.dRowPitch  ;
            if((dCrntRow-1)/OM.DevInfo.iRowGrCnt)
                dTargetPos += ((iCrntRow-1)/OM.DevInfo.iRowGrCnt) *(OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) ;
            MT_GoAbsMan(miSTL_YTop , dTargetPos) ;
        }
    }








    else if (m_iManNo == mcSTL_YBtmUp1pitch   ) {
        double dCrntPos     = MT_GetCmdPos(miSTL_YBtm) ;
        double dTargetPos   ;

        double dTemp =  dCrntPos - PM.GetValue (miSTL_YBtm , pvSTL_YBWorkStartPs) ;
        double dCrntRow     = dTemp/OM.DevInfo.dRowPitch ;
        int    iCrntRow     = (int)dCrntRow ;
        bool   bUpOnTheRail = PM.GetValue (miSTL_YBtm , pvSTL_YBWorkStartPs) <= dCrntPos ;

        if( dCrntRow < OM.DevInfo.iRowCnt-1) { //범위.
            dTargetPos = PM.GetValue (miSTL_YBtm , pvSTL_YBWorkStartPs) + (dCrntRow+1) * OM.DevInfo.dRowPitch  ;
            if((dCrntRow+1)/OM.DevInfo.iRowGrCnt)
                dTargetPos += ((iCrntRow+1)/OM.DevInfo.iRowGrCnt) *(OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) ;
            MT_GoAbsMan(miSTL_YBtm , dTargetPos) ;
        }
    }
    else if (m_iManNo == mcSTL_YBtmDn1pitch   ) {
        double dCrntPos     = MT_GetCmdPos(miSTL_YBtm) ;
        double dTargetPos   ;

        double dTemp =  dCrntPos - PM.GetValue (miSTL_YBtm , pvSTL_YBWorkStartPs) ;
        double dCrntRow     = dTemp/OM.DevInfo.dRowPitch ;
        int    iCrntRow     = (int)dCrntRow ;
        bool   bUpOnTheRail = PM.GetValue (miSTL_YBtm , pvSTL_YBWorkStartPs) <= dCrntPos ;

        if( dCrntRow > 1) { //범위.
            dTargetPos = PM.GetValue (miSTL_YBtm , pvSTL_YBWorkStartPs) + (dCrntRow-1) * OM.DevInfo.dRowPitch  ;
            if((dCrntRow-1)/OM.DevInfo.iRowGrCnt)
                dTargetPos += ((iCrntRow-1)/OM.DevInfo.iRowGrCnt) *(OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) ;
            MT_GoAbsMan(miSTL_YBtm , dTargetPos) ;
        }
    }
    */
    else if (m_iManNo == mcULD_Home          ) { ULD.InitHomeStep (); }
    else if (m_iManNo == mcULD_Supply        ) { ULD.InitCycleStep(); }
    else if (m_iManNo == mcULD_Pick          ) { ULD.InitCycleStep(); }
    else if (m_iManNo == mcULD_SttPstn       ) { MT_GoAbsMan(miULD_ZCmp,PM.GetValue(miULD_ZCmp , pvULD_ZWorkStartPs) );  }
    else if (m_iManNo == mcULD_Place         ) { ULD.InitCycleStep(); }
    else if (m_iManNo == mcULD_UpSlPitch     ) { MT_GoIncMan(miULD_ZCmp ,-OM.DevInfo.dCsSlPitch);}
    else if (m_iManNo == mcULD_DnSlPitch     ) { MT_GoIncMan(miULD_ZCmp , OM.DevInfo.dCsSlPitch);}
    else if (m_iManNo == mcULD_Clamp         ) { if(AT_Complete(aiULD_Cmp   , ccFwd)) { AT_MoveCyl(aiULD_Cmp , ccBwd); }
                                                 if(AT_Complete(aiULD_Cmp   , ccBwd)) { AT_MoveCyl(aiULD_Cmp , ccFwd); } }
    else if (m_iManNo == mcULD_TranInAC      ) { IO_SetY(yULD_MgzInAC  , !IO_GetY(yULD_MgzInAC ) );}
    else if (m_iManNo == mcULD_TranOutAC     ) { IO_SetY(yULD_MgzOutAC , !IO_GetY(yULD_MgzOutAC) );}

    else if (m_iManNo == mcSTL_Pick          ) {STL.InitCycleStep(); }
    else if (m_iManNo == mcSTL_Place         ) {STL.InitCycleStep(); }


    else if (m_iManNo == mcETC_AllClampOff   ) {AT_MoveCyl(aiPRB_Cmp   , ccBwd);
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

    else if (m_iManNo == mcETC_LDRULDPlace    ) { if(!OM.EqpOptn.bExistLoader || !OM.EqpOptn.bExistUnLoader) {m_bManSetting = false ; return false ;  } //m_bManSetting 중요함 리턴전에 꼭 펄스 시켜야함. 쓰레드가 달라서. ::Update에서 m_iManNo=0이 되므로 주의.
                                                  LD1.InitCycleStep();
                                                  ULD.InitCycleStep();
                                                }

    else                                    { m_iManNo = mcNoneCycle ; }

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
       m_iManNo != mcPRE_Home &&
       m_iManNo != mcWRK_Home &&
       m_iManNo != mcPSB_Home &&
       m_iManNo != mcULD_Home &&
       m_iManNo != mcAllHome    ) SEQ.InspectHomeEnd ();
    SEQ.InspectLightGrid();

    bool r1,r2,r3,r4,r5,r6 ;

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return ; }
    //Cycle Step.
    if      (m_iManNo == mcAllHome         )  {if(OM.EqpOptn.bExistLoader)   r1 = LD1.CycleHome();
                                               else                          r1 = true ;
                                                                             r2 = PRB.CycleHome();
                                                                             r3 = WRK.CycleHome();
                                                                             r5 = STL.CycleHome();
                                                                             if(r5) r4 = PSB.CycleHome();
                                               if(OM.EqpOptn.bExistUnLoader) r6 = ULD.CycleHome();
                                               else                          r6 = true ;
                                                   if(r1&&r2&&r3&&r4&&r5&&r6) {
                                                       m_iManNo = mcNoneCycle;
                                                       FM_MsgOk("Confirm","All Home Finished!");
                                                   }
                                               }
    else if (m_iManNo == mcETC_LDRULDPlace )  { r1 = LD1.CycleHome();
                                                r6 = ULD.CycleHome();
                                                if(r1&&r6) m_iManNo = mcNoneCycle;
                                              }

    else if (m_iManNo == mcLDR_Home     && OM.EqpOptn.bExistLoader ) { if(LD1.CycleHome     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Supply   && OM.EqpOptn.bExistLoader ) { if(LD1.CycleSupply   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Pick     && OM.EqpOptn.bExistLoader ) { if(LD1.CyclePick     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Place    && OM.EqpOptn.bExistLoader ) { if(LD1.CyclePlace    ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcPRE_Home        ) { if(PRB.CycleHome      ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPRE_In          ) { if(PRB.CycleIn        ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPRE_Drag        ) { if(PRB.CycleDrag      ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPRE_Out         ) { if(PRB.CycleOut       ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcWRK_Home        ) { if(WRK.CycleHome      ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcWRK_PreWork     ) { if(WRK.CyclePreWork   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcWRK_PostWork    ) { if(WRK.CyclePostWork  ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcWRK_RptCntct    ) { if(WRK.CycleRepeat    ()) m_iManNo = mcNoneCycle; } //하바텍

    else if (m_iManNo == mcPSB_Home        ) { if(STL.CycleHome      ()) { if(PSB.CycleHome      ()) m_iManNo = mcNoneCycle; } }
    else if (m_iManNo == mcPSB_Drag        ) { if(PSB.CycleDrag      ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_Out         ) { if(PSB.CycleOut       ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcULD_Home        ) { if(ULD.CycleHome      ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Supply      ) { if(ULD.CycleSupply    ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Pick        ) { if(ULD.CyclePick      ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Place       ) { if(ULD.CyclePlace     ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcSTL_Pick        ) { if(STL.CyclePick      ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcSTL_Place       ) { if(STL.CyclePlce      ()) m_iManNo = mcNoneCycle; }

//    else if (m_iManNo == mcETC_RemoveStrip    ) { if(CycleStipRemove   () ){Trace("Manual Man","mcETC_RemoveStrip    End"); m_iManNo = mcNoneCycle; } }
//    else if (m_iManNo == mcETC_PreRemoveStrip ) { if(CyclePreStipRemove() ){Trace("Manual Man","mcETC_PreRemoveStrip End"); m_iManNo = mcNoneCycle; } }
//    else if (m_iManNo == mcETC_WrkRemoveStrip ) { if(CycleWrkStipRemove() ){Trace("Manual Man","mcETC_WrkRemoveStrip End"); m_iManNo = mcNoneCycle; } }
//    else if (m_iManNo == mcETC_PstRemoveStrip ) { if(CyclePstStipRemove() ){Trace("Manual Man","mcETC_PstRemoveStrip End"); m_iManNo = mcNoneCycle; } }

    else                                     {                          m_iManNo = mcNoneCycle; } //여기서 갱신됌.

    //Ok.
    return ;
}

//---------------------------------------------------------------------------

