//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ManualMan.h"

#include "SMInterfaceUnit.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "Sequence.h"
#include "PstnMan.h"
#include "VisnComUnit.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff.h"
#include "Head.h"
#include "Rail.h"
#include "PstBuff.h"
#include "UnLoader.h"
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
    m_bStop       = false ;
    bULDPlace     = false ;

    iTrmManCol = -1;
    iTrmManRow = -1;

    m_tmCycle.Clear() ;
}
//---------------------------------------------------------------------------
bool __fastcall CManualMan::CheckSafe(EN_MANUAL_CYCLE _iNo)
{
//    bool bRet = true ;

    if( _iNo <  0                ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( _iNo >= MAX_MANUAL_CYCLE &&
       _iNo != mcAllHome         ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( m_iManNo != mcNoneCycle  ) {FM_MsgOk("ERROR","Doing Manual Cycle"   ); return false ;}
    if( SEQ._bRun                ) {FM_MsgOk("ERROR","Autorunning"          ); return false ;}
    //Check Alarm
    if( EM_IsErr() && _iNo !=mcETC_MainAirOnOff ) { return false  ; } //아 밑에 처리 하는 애 때문에 잠시 이렇게 함.

    if(_iNo  == mcAllHome) {
        if( IO_GetX(xPRB_PkgIn1 ) ||  IO_GetX(xPSB_PkgOut3) || IO_GetX(xPSB_PkgOut4)) {FM_MsgOk("ERR","Check Rail In or Out Sensor!" ); return false ;}
        if(!IO_GetX(xETC_MainSol) || !IO_GetY(yETC_MainSol)                         ) {FM_MsgOk("ERR","Check Main Air!"              ); return false ;}
        if(!IO_GetX(xETC_PowerOn)                                                   ) {FM_MsgOk("ERR","Check Power Switch On!"       ); return false ;}
        if(!IO_GetY(xETC_LMainAir)                                                  ) {FM_MsgOk("ERR","Air On Plz"                   ); return false ;}
        if(!IO_GetY(xETC_RMainAir)                                                  ) {FM_MsgOk("ERR","Air On Plz"                   ); return false ;}
        if(!SEQ.InspectMotorServo()                                                 ) {FM_MsgOk("ERR","Motor Check Plz (Sevor On)"   ); return false ;}
        if((IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2))                   ) {FM_MsgOk("CheckSafe","Unloader Mgz Remove Plz (Now Checked Use Punch Machine)") ; return false ;}
        if((IO_GetX(xULD_MgzIn     ) && OM.CmnOptn.bUseTrimMachine)                 ) {FM_MsgOk("CheckSafe","Unloader Wait Mgz Remove Plz (Now Checked Use Punch Machine)") ; return false ;}

        for(int i = 0 ; i<MAX_MOTR ; i++ ) MT_SetHomeEnd(i,false);
    }
    if(_iNo  == mcLDR_Home ) {
        if( IO_GetX(xPRB_PkgIn1 ) ) {FM_MsgOk("ERROR","Check Rail In  Sensor!" ); return false ;}
    }
    if(_iNo  == mcULD_Home ) {
        if( IO_GetX(xPSB_PkgOut4) ) {FM_MsgOk("ERROR","Check Rail Out Sensor!" ); return false ;}
    }

    EM_SetDisp(true);

    return true ;
}
//---------------------------------------------------------------------------
bool __fastcall CManualMan::SetManCycle(EN_MANUAL_CYCLE _iNo)
{
    if ( !CheckSafe(_iNo) ) {
        Init();
        return false ;  // 일단 자주 쓰는것만 좀 빼자 dd
    }

    m_iManNo = _iNo ;
        
    m_bManSetting = true ;
//    m_iManNo = _iNo ;

    double VsnMaxColPich = OM.DevInfo.dColPitch * (OM.DevInfo.iColCnt - 2) ;
    double VsnMaxRowPich = OM.DevInfo.dRowPitch * (OM.DevInfo.iRowCnt - 2) ;

    double MaxColPich = OM.DevInfo.dColPitch * (OM.DevInfo.iColCnt - 1 );
    double MaxRowPich = OM.DevInfo.dRowPitch * (OM.DevInfo.iRowCnt - 1 );

    bool bCanMvWK3 = false ;
    bool bCanMvWK2 = false ;
    bool bCanMvWK1 = false ;
    bool bCanMvPRB = false ;

    bool bRet = true ;

//***********************************************************************************************
// 프리프로세서
//***********************************************************************************************

         if (m_iManNo == mcAllHome            ){     if(!IO_GetX(xETC_PowerOn  )) {FM_MsgOk("ERR","Power On Plz"); bRet = false ;}
                                                else if(!IO_GetX(xETC_MainSol) || !IO_GetY(yETC_MainSol)) {FM_MsgOk("ERR","Air On Plz"); bRet = false ;}
                                                //여기서 굳이 하는건 아닌거 같네...시퀀스에서 에러 띄우는게... ?
                                               }

    else if (m_iManNo == mcLDR_Home           ){}
    else if (m_iManNo == mcLDR_Supply         ){}
    else if (m_iManNo == mcLDR_Pick           ){if(!DM.ARAY[riLDR].CheckAllStat(csNone)       ){FM_MsgOk("ERR","Remove Mgz"          ); bRet = false ;} }
    else if (m_iManNo == mcLDR_SttPstn        ){if(!LDR.CheckSafe(miLDR_ZCmp,piLDR_ZWorkStart)){                                        bRet = false ;} }

    else if (m_iManNo == mcLDR_Place          ){}
    else if (m_iManNo == mcLDR_UpSlPitch      ){
                                                if(!LDR.CheckSafe(miLDR_ZCmp,piLDR_ZWork)     ){                                        bRet = false ;} }
    else if (m_iManNo == mcLDR_DnSlPitch      ){if(!LDR.CheckSafe(miLDR_ZCmp,piLDR_ZWork)     ){                                        bRet = false ;} }
    else if (m_iManNo == mcLDR_Pusher         ){if(!LDR.CheckSafe(aiLDR_PshrFwBw , ccFwd)     ){                                        bRet = false ;}
                                                if(!LDR.CheckSafe(aiLDR_PshrFwBw , ccBwd)     ){                                        bRet = false ;} }

    else if (m_iManNo == mcLDR_Clamp          ){if(AT_Complete(aiULD_ClmpUpDn , ccFwd)        ){ if(!ULD.CheckSafe(aiULD_ClmpUpDn   , ccBwd)) bRet = false ;}
                                                if(AT_Complete(aiULD_ClmpUpDn , ccBwd)        ){
                                                    if(IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Sure ?") != mrYes ) bRet = false;
                                                    if(!ULD.CheckSafe(aiULD_ClmpUpDn , ccFwd)) bRet = false ;
                                                    }
                                               }

    /*
    if(IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Sure ?") != mrYes ) return ;
        bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;

    */
    //else if (m_iManNo == mcLDR_Clamp          ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
    //                                            if(!LDR.CheckSafe(aiLDR_Cmp , ccFwd)) bRet = false ;
    //                                           }
    else if (m_iManNo == mcLDR_TranInAC       ){}
    else if (m_iManNo == mcLDR_TranOutAC      ){}

    else if (m_iManNo ==  mcRAL_Home           ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo ==  mcRAL_WR1LiftUp      ){}
    else if (m_iManNo ==  mcRAL_WR1LiftDn      ){}
    else if (m_iManNo ==  mcRAL_WR1StprUp      ){}
    else if (m_iManNo ==  mcRAL_WR1StprDn      ){}
    else if (m_iManNo ==  mcRAL_WR1AlgnFw      ){}
    else if (m_iManNo ==  mcRAL_WR1AlgnBw      ){}
    else if (m_iManNo ==  mcRAL_WR2LiftUp      ){}
    else if (m_iManNo ==  mcRAL_WR2LiftDn      ){}
    else if (m_iManNo ==  mcRAL_WR2StprUp      ){}
    else if (m_iManNo ==  mcRAL_WR2StprDn      ){}
    else if (m_iManNo ==  mcRAL_WR2AlgnFw      ){}
    else if (m_iManNo ==  mcRAL_WR2AlgnBw      ){}
    else if (m_iManNo ==  mcRAL_WR3LiftUp      ){}
    else if (m_iManNo ==  mcRAL_WR3LiftDn      ){}
    else if (m_iManNo ==  mcRAL_WR3StprUp      ){}
    else if (m_iManNo ==  mcRAL_WR3StprDn      ){}
    else if (m_iManNo ==  mcRAL_WR3AlgnFw      ){}
    else if (m_iManNo ==  mcRAL_WR3AlgnBw      ){}
    else if (m_iManNo ==  mcHED_WorkFstPs      ){}
    else if (m_iManNo ==  mcRAL_VisnYZeroPs    ){}

    else if (m_iManNo ==  mcPSB_Home           ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo ==  mcPSB_TWaitPs        ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo ==  mcPSB_TFirstPs       ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo ==  mcPSB_TFlpCmpUp      ){}
    else if (m_iManNo ==  mcPSB_TFlpCmpDn      ){}
    else if (m_iManNo ==  mcPSB_TAlignFw       ){}
    else if (m_iManNo ==  mcPSB_TFlpNorPs      ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo ==  mcPSB_TFlpInsPs      ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo ==  mcPSB_TTrimAllTest   ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo ==  mcPSB_TTrimUpDn      ){}
    else if (m_iManNo ==  mcPSB_TTrimXMv       ){if(!AT_Complete(aiPSB_PnchDnUp , ccBwd)) {FM_MsgOk("CheckSafe","Trim Cylinder Check Plz."); bRet = false;} }
    else if (m_iManNo ==  mcPSB_TTrimYMv       ){if(!AT_Complete(aiPSB_PnchDnUp , ccBwd)) {FM_MsgOk("CheckSafe","Trim Cylinder Check Plz."); bRet = false;} }
    else if (m_iManNo ==  mcPSB_IdxUp          ){}
    else if (m_iManNo ==  mcPSB_IdxDn          ){}

    else if (m_iManNo ==  mcULD_Home           ){
        if((IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2)) && OM.CmnOptn.bUseTrimMachine){
            FM_MsgOk("CheckSafe","Unloader Mgz Remove Plz (Now Checked Use Punch Machine)") ;
            bRet = false ;
        }
        if((IO_GetX(xULD_MgzIn     ) && OM.CmnOptn.bUseTrimMachine)) {
            FM_MsgOk("CheckSafe","Unloader Wait Mgz Remove Plz (Now Checked Use Punch Machine)") ;
            bRet = false ;
        }
        m_iManStep = 10 ; m_iPreManStep = 0;
    }
    else if (m_iManNo == mcULD_Home           ){}
    else if (m_iManNo == mcULD_Supply         ){}
    else if (m_iManNo == mcULD_Pick           ){if(!DM.ARAY[riULD].CheckAllStat(csNone))                                                        bRet = false ;}
    else if (m_iManNo == mcULD_SttPstn        ){if(!ULD.CheckSafe(miULD_ZCmp,piULD_ZWorkStart))                                                 bRet = false ;}
    else if (m_iManNo == mcULD_Place          ){}
    else if (m_iManNo == mcULD_UpSlPitch      ){if(!ULD.CheckSafe(miULD_ZCmp,piULD_ZWork))                                                      bRet = false ;}
    else if (m_iManNo == mcULD_DnSlPitch      ){if(!ULD.CheckSafe(miULD_ZCmp,piULD_ZWork))                                                      bRet = false ;}
    else if (m_iManNo == mcULD_Clamp          ){if(AT_Complete(aiULD_ClmpUpDn   , ccFwd)         ) { if(!ULD.CheckSafe(aiULD_ClmpUpDn , ccBwd)) bRet = false ;}
                                                if(AT_Complete(aiULD_ClmpUpDn   , ccBwd)         ) {
                                                    if(IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Sure ?") != mrYes ) bRet = false;
                                                    if(!ULD.CheckSafe(aiULD_ClmpUpDn   , ccFwd)) bRet = false ;
                                                }
                                               }
    else if (m_iManNo == mcPSB_PshrUpDn       ){}
    else if (m_iManNo == mcPSB_PshrFwBw       ){}
    else if (m_iManNo == mcULD_TranInAC       ){}
    else if (m_iManNo == mcULD_TranOutAC      ){}


    else if (m_iManNo ==  mcETC_Sampling1Cycle ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo ==  mcETC_AllBeltOn      ){}
    else if (m_iManNo ==  mcETC_AllBeltOff     ){}
    else if (m_iManNo ==  mcETC_AllStprUp      ){}
    else if (m_iManNo ==  mcETC_AllStprDn      ){}
    else if (m_iManNo ==  mcETC_Wk1AlignLiftUp ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo ==  mcETC_Wk2AlignLiftUp ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo ==  mcETC_Wk3AlignLiftUp ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo ==  mcETC_InsAlignFlp    ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo ==  mcETC_AllAlignLiftUp ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo ==  mcETC_AllLiftDn      ){}
    else if (m_iManNo ==  mcETC_MainAirOnOff   ){}
    else if (m_iManNo ==  mcETC_ManLotEnd      ){
        LDRExist = !DM.ARAY[riLDR].CheckAllStat(csNone)  || IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2);
        ULDExist = !DM.ARAY[riULD].CheckAllStat(csNone)  || IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2);

        if( LDRExist ){ LDR.InitCycleStep(); bLDRPlace = true;                    }
        if( ULDExist ){ ULD.InitCycleStep(); bULDPlace = true;                    }
    }

    else if (m_iManNo ==  mcETC_AllStripOut    ){
         if(DM.ARAY[riPRB].CheckAllStat(csNone) &&
            DM.ARAY[riWR1].CheckAllStat(csNone) &&
            DM.ARAY[riWR2].CheckAllStat(csNone) &&
            DM.ARAY[riWR3].CheckAllStat(csNone) &&
            DM.ARAY[riPSB].CheckAllStat(csNone) ) {FM_MsgOk("Confirm","Rail Strip Is Not Exist") ;bRet = false ;}


         ULDExist = !DM.ARAY[riULD].CheckAllStat(csNone)  || IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2);
         ULDEmpty =  DM.ARAY[riULD].CheckAllStat(csEmpty) ;

         if( ULDExist             ){ ULD.InitCycleStep(); }
         if(!ULDExist && !ULDEmpty){ ULD.InitCycleStep(); }

           m_iManStep = 10 ; m_iPreManStep = 0;
         bULDPlace = true ;
         bULDPick  = true ;
    }
    else if (m_iManNo ==  mcETC_1CycleMv    ){

        bCanMvWK3 = (!DM.ARAY[riWR3].CheckAllStat(csNone) && (DM.ARAY[riPSB].CheckAllStat(csNone)            )) ;
        bCanMvWK2 = (!DM.ARAY[riWR2].CheckAllStat(csNone) && (DM.ARAY[riWR3].CheckAllStat(csNone) ||bCanMvWK3)) ;
        bCanMvWK1 = (!DM.ARAY[riWR1].CheckAllStat(csNone) && (DM.ARAY[riWR2].CheckAllStat(csNone) ||bCanMvWK2)) ;
        bCanMvPRB = (!DM.ARAY[riPRB].CheckAllStat(csNone) && (DM.ARAY[riWR1].CheckAllStat(csNone) ||bCanMvWK1)) ;

        if(!bCanMvWK1 && !bCanMvWK2 && !bCanMvWK3 && !bCanMvPRB ){
            FM_MsgOk("Confirm","Trim Zone Strip Remove Plz") ;bRet = false ;
        }
        m_iManStep = 10 ; m_iPreManStep = 0;

    }




/*    //ETC 부분
    else if (m_iManNo == mcHED_InspMan         ){m_iManStep = 10; m_iPreManStep = 0; }
    else if (m_iManNo == mcETC_AllBeltOn       ){}
    else if (m_iManNo == mcETC_AllBeltOff      ){}
    else if (m_iManNo == mcETC_AllStprUp       ){}
    else if (m_iManNo == mcETC_ALlStprDn       ){}
    else if (m_iManNo == mcETC_Wk1AlignLiftUp  ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcETC_Wk2AlignLiftUp  ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcETC_Wk3AlignLiftUp  ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcETC_TrmAlignLiftUp  ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcETC_AllAlignLiftUp  ){m_iManStep = 10 ; m_iPreManStep = 0;}
    else if (m_iManNo == mcETC_AllAlignLiftDn  ){}
    else if (m_iManNo == mcETC_ManLotEnd       ){m_iManStep = 10 ; m_iPreManStep = 0;}
*/

    if(!bRet) Init();

//***********************************************************************************************
// 처리..(처리만 밑에다가 넣는게 맞는듯...dd다 이상하넹...
//***********************************************************************************************
    if      (m_iManNo == mcAllHome            ) { //IO_SetY(yETC_MainSol ,true); 이러면 빠져나가는 상황이 있어서 그냥 윗단에서
                                                  //IO_SetY(yETC_MainSol2,true); 에러를 잡는다.
                                                  IO_SetY(yETC_LInitLp, !IO_GetY(yETC_LInitLp));
                                                  IO_SetY(yETC_RInitLp, !IO_GetY(yETC_RInitLp));

                                                  LDR.InitHomeStep();
                                                  PRB.InitHomeStep();
                                                  RAL.InitHomeStep();
                                                  HED.InitHomeStep();
                                                  PSB.InitHomeStep();
                                                  ULD.InitHomeStep();}

    else if (m_iManNo == mcLDR_Home            ) { LDR.InitHomeStep (); }
    else if (m_iManNo == mcLDR_Supply          ) { LDR.InitCycleStep(); }
    else if (m_iManNo == mcLDR_Pick            ) { LDR.InitCycleStep(); }
    else if (m_iManNo == mcLDR_SttPstn         ) { MT_GoAbsMan(miLDR_ZCmp,PM.GetValue(miLDR_ZCmp , pvLDR_ZWorkStart) ); }
    else if (m_iManNo == mcLDR_Place           ) { DM.ARAY[riLDR].SetStat(csEmpty); LDR.InitCycleStep(); }
    else if (m_iManNo == mcLDR_UpSlPitch       ) { MT_GoIncMan(miLDR_ZCmp ,-OM.DevInfo.dCsSlPitch);}
    else if (m_iManNo == mcLDR_DnSlPitch       ) { MT_GoIncMan(miLDR_ZCmp , OM.DevInfo.dCsSlPitch);}
    else if (m_iManNo == mcLDR_Pusher          ) { if(AT_Complete(aiLDR_PshrFwBw , ccFwd )) { AT_MoveCyl(aiLDR_PshrFwBw , ccBwd); }
                                                   if(AT_Complete(aiLDR_PshrFwBw , ccBwd )) { AT_MoveCyl(aiLDR_PshrFwBw , ccFwd); } }

    else if (m_iManNo == mcLDR_Clamp           ) { if(AT_Complete(aiLDR_ClmpUpDn , ccFwd )) { AT_MoveCyl(aiLDR_ClmpUpDn , ccBwd); }
                                                   if(AT_Complete(aiLDR_ClmpUpDn , ccBwd )) { AT_MoveCyl(aiLDR_ClmpUpDn , ccFwd); } }
    else if (m_iManNo == mcLDR_TranInAC        ) { IO_SetY(yLDR_MgzInAC  , !IO_GetY(yLDR_MgzInAC ) ); }
    else if (m_iManNo == mcLDR_TranOutAC       ) { IO_SetY(yLDR_MgzOutAC , !IO_GetY(yLDR_MgzOutAC) ); }

    else if (m_iManNo ==  mcRAL_Home           ){HED.InitHomeStep();}
    else if (m_iManNo ==  mcRAL_WR1LiftUp      ){AT_MoveCyl(aiWK1_SttnUpDn , ccFwd);}
    else if (m_iManNo ==  mcRAL_WR1LiftDn      ){AT_MoveCyl(aiWK1_SttnUpDn , ccBwd);}
    else if (m_iManNo ==  mcRAL_WR1StprUp      ){AT_MoveCyl(aiWK1_StprUpDn , ccFwd);}
    else if (m_iManNo ==  mcRAL_WR1StprDn      ){AT_MoveCyl(aiWK1_StprUpDn , ccBwd);}
    else if (m_iManNo ==  mcRAL_WR1AlgnFw      ){AT_MoveCyl(aiWK1_AlgnFwBw , ccFwd);}
    else if (m_iManNo ==  mcRAL_WR1AlgnBw      ){AT_MoveCyl(aiWK1_AlgnFwBw , ccBwd);}
    else if (m_iManNo ==  mcRAL_WR2LiftUp      ){AT_MoveCyl(aiWK2_SttnUpDn , ccFwd);}
    else if (m_iManNo ==  mcRAL_WR2LiftDn      ){AT_MoveCyl(aiWK2_SttnUpDn , ccBwd);}
    else if (m_iManNo ==  mcRAL_WR2StprUp      ){AT_MoveCyl(aiWK2_StprUpDn , ccFwd);}
    else if (m_iManNo ==  mcRAL_WR2StprDn      ){AT_MoveCyl(aiWK2_StprUpDn , ccBwd);}
    else if (m_iManNo ==  mcRAL_WR2AlgnFw      ){AT_MoveCyl(aiWK2_AlgnFwBw , ccFwd);}
    else if (m_iManNo ==  mcRAL_WR2AlgnBw      ){AT_MoveCyl(aiWK2_AlgnFwBw , ccBwd);}
    else if (m_iManNo ==  mcRAL_WR3LiftUp      ){AT_MoveCyl(aiWK3_SttnUpDn , ccFwd);}
    else if (m_iManNo ==  mcRAL_WR3LiftDn      ){AT_MoveCyl(aiWK3_SttnUpDn , ccBwd);}
    else if (m_iManNo ==  mcRAL_WR3StprUp      ){AT_MoveCyl(aiWK3_StprUpDn , ccFwd);}
    else if (m_iManNo ==  mcRAL_WR3StprDn      ){AT_MoveCyl(aiWK3_StprUpDn , ccBwd);}
    else if (m_iManNo ==  mcRAL_WR3AlgnFw      ){AT_MoveCyl(aiWK3_AlgnFwBw , ccFwd);}
    else if (m_iManNo ==  mcRAL_WR3AlgnBw      ){AT_MoveCyl(aiWK3_AlgnFwBw , ccBwd);}
    else if (m_iManNo ==  mcHED_WorkFstPs      ){MT_GoAbsMan(miWRK_XVsn, PM.GetValue(miWRK_XVsn,pvWRK_XVsnWorkStart));
                                                 MT_GoAbsMan(miWRK_YVsn, PM.GetValue(miWRK_YVsn,pvWRK_YVsnWorkStart)); } //
    else if (m_iManNo ==  mcRAL_VisnYZeroPs    ){MT_GoAbsRun(miWRK_YVsn, 0 ); }

    else if (m_iManNo ==  mcPSB_Home           ){PSB.InitHomeStep();}
    else if (m_iManNo ==  mcPSB_TWaitPs        ){} //움직이는중에는 안되네...위에 넣을까? dd
    else if (m_iManNo ==  mcPSB_TFirstPs       ){}
    else if (m_iManNo ==  mcPSB_TFlpCmpUp      ){AT_MoveCyl(aiPSB_FlprOpCl , ccBwd);}
    else if (m_iManNo ==  mcPSB_TFlpCmpDn      ){AT_MoveCyl(aiPSB_FlprOpCl , ccFwd);}
    else if (m_iManNo ==  mcPSB_TAlignFw       ){if(AT_Complete(aiPSB_AlgnFwBw , ccBwd)) AT_MoveCyl(aiPSB_AlgnFwBw , ccFwd);
                                                 else                                    AT_MoveCyl(aiPSB_AlgnFwBw , ccBwd);}
    else if (m_iManNo ==  mcPSB_TFlpNorPs      ){}
    else if (m_iManNo ==  mcPSB_TFlpInsPs      ){}
    else if (m_iManNo ==  mcPSB_TTrimAllTest   ){}
    else if (m_iManNo ==  mcPSB_TTrimUpDn      ){if(AT_Complete(aiPSB_PnchDnUp , ccBwd)) AT_MoveCyl(aiPSB_PnchDnUp , ccFwd);
                                                 else                                    AT_MoveCyl(aiPSB_PnchDnUp , ccBwd);}
    else if (m_iManNo ==  mcPSB_TTrimXMv       ){ItemCheck = 0;
                                                 iTrmManCol++ ;
                                                 if(iTrmManCol < 0 ) iTrmManCol = 0 ;
                                                 if(iTrmManCol >= OM.DevInfo.iColCnt) iTrmManCol = 0;
                                                 MT_GoAbsMan(miPSB_XTrm , GetMotrPosTable(miPSB_XTrm , iTrmManRow , iTrmManCol)) ; }
    else if (m_iManNo ==  mcPSB_TTrimYMv       ){ItemCheck = 0;
                                                 iTrmManRow ++ ;
                                                 if(iTrmManRow < 0 ) iTrmManRow = 0 ;
                                                 if(iTrmManRow >= OM.DevInfo.iRowCnt) iTrmManRow = 0;
                                                 MT_GoAbsMan(miPSB_YTrm , GetMotrPosTable(miPSB_YTrm , iTrmManRow , iTrmManCol)) ; }
//    else if (m_iManNo ==  mcPSB_TTrimDn        ){AT_MoveCyl(aiPSB_Trim  ,ccFwd);}
    else if (m_iManNo ==  mcPSB_IdxUp          ){AT_MoveCyl(aiPSB_IndxUpDn , ccFwd);}
    else if (m_iManNo ==  mcPSB_IdxDn          ){AT_MoveCyl(aiPSB_IndxUpDn , ccBwd);}
    else if (m_iManNo ==  mcULD_Home           ){ULD.InitHomeStep();}
    else if (m_iManNo == mcULD_Supply        ) { ULD.InitCycleStep(); }
    else if (m_iManNo == mcULD_Pick          ) { ULD.InitCycleStep(); }
    else if (m_iManNo == mcULD_SttPstn       ) { MT_GoAbsMan(miULD_ZCmp,PM.GetValue(miULD_ZCmp , pvULD_ZWorkStart) );  }
    else if (m_iManNo == mcULD_Place         ) { ULD.InitCycleStep(); }
    else if (m_iManNo == mcULD_UpSlPitch     ) { MT_GoIncMan(miULD_ZCmp ,-OM.DevInfo.dCsSlPitch);}
    else if (m_iManNo == mcULD_DnSlPitch     ) { MT_GoIncMan(miULD_ZCmp , OM.DevInfo.dCsSlPitch);}
    else if (m_iManNo == mcULD_Clamp         ) { if(AT_Complete(aiULD_ClmpUpDn   , ccFwd)) { AT_MoveCyl(aiULD_ClmpUpDn , ccBwd); }
                                                 if(AT_Complete(aiULD_ClmpUpDn   , ccBwd)) { AT_MoveCyl(aiULD_ClmpUpDn , ccFwd); } }
    else if (m_iManNo == mcPSB_PshrUpDn       ){ if(AT_Complete(aiPSB_PshrDnUp , ccFwd)){ AT_MoveCyl(aiPSB_PshrDnUp , ccBwd); }
                                                 if(AT_Complete(aiPSB_PshrDnUp , ccFwd)){ AT_MoveCyl(aiPSB_PshrDnUp , ccFwd); } }
    else if (m_iManNo == mcPSB_PshrFwBw       ){ if(AT_Complete(aiPSB_PshrFwBw , ccFwd)){ AT_MoveCyl(aiPSB_PshrFwBw , ccBwd); }
                                                 if(AT_Complete(aiPSB_PshrFwBw , ccFwd)){ AT_MoveCyl(aiPSB_PshrFwBw , ccFwd); } }
    else if (m_iManNo == mcULD_TranInAC      ) { IO_SetY(yULD_MgzInAC  , !IO_GetY(yULD_MgzInAC ) );}
    else if (m_iManNo == mcULD_TranOutAC     ) { IO_SetY(yULD_MgzOutAC , !IO_GetY(yULD_MgzOutAC) );}

    else if (m_iManNo ==  mcETC_Sampling1Cycle ){}//if(CycleManBindInsp()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo ==  mcETC_AllBeltOn      ){IO_SetY(yPRB_FeedingAC, true ); IO_SetY(yWRK_FeedingAC, true ); }
    else if (m_iManNo ==  mcETC_AllBeltOff     ){IO_SetY(yPRB_FeedingAC, false); IO_SetY(yWRK_FeedingAC, false); }
    else if (m_iManNo ==  mcETC_AllStprUp      ){AT_MoveCyl(aiPRB_StprUpDn  ,ccFwd); AT_MoveCyl(aiWK1_StprUpDn  ,ccFwd); AT_MoveCyl(aiWK2_StprUpDn  ,ccFwd); AT_MoveCyl(aiWK3_StprUpDn  ,ccFwd); }
    else if (m_iManNo ==  mcETC_AllStprDn      ){AT_MoveCyl(aiPRB_StprUpDn  ,ccBwd); AT_MoveCyl(aiWK1_StprUpDn  ,ccBwd); AT_MoveCyl(aiWK2_StprUpDn  ,ccBwd); AT_MoveCyl(aiWK3_StprUpDn  ,ccBwd); }
    else if (m_iManNo ==  mcETC_Wk1AlignLiftUp ){}//if(CycleManLift(1)) m_iManNo = mcNoneCycle; }
    else if (m_iManNo ==  mcETC_Wk2AlignLiftUp ){}//if(CycleManLift(2)) m_iManNo = mcNoneCycle; }
    else if (m_iManNo ==  mcETC_Wk3AlignLiftUp ){}//if(CycleManLift(3)) m_iManNo = mcNoneCycle; }
    else if (m_iManNo ==  mcETC_InsAlignFlp    ){PSB.InitCycleStep();}
//    else if (m_iManNo ==  mcPSB_TFlpNorPs
//리뷰비전 1사이클이 빠졋구낭...dd
//트림     1사이클이랑.dd 하지만 하지 않겟다...dd;;;
    else if (m_iManNo ==  mcETC_AllAlignLiftUp ){}//if(CycleManLift(0)) m_iManNo = mcNoneCycle; }
    else if (m_iManNo ==  mcETC_AllLiftDn      ){AT_MoveCyl(aiWK1_SttnUpDn , ccBwd) ; AT_MoveCyl(aiWK1_AlgnFwBw , ccBwd) ;
                                                 AT_MoveCyl(aiWK2_SttnUpDn , ccBwd) ; AT_MoveCyl(aiWK2_AlgnFwBw , ccBwd) ;
                                                 AT_MoveCyl(aiWK3_SttnUpDn , ccBwd) ; AT_MoveCyl(aiWK3_AlgnFwBw , ccBwd) ; }
    else if (m_iManNo ==  mcETC_MainAirOnOff   ){IO_SetY(yETC_MainSol,!IO_GetY(xETC_MainSol));}
    else if (m_iManNo ==  mcETC_ManLotEnd      ){m_iManNo ==  mcETC_ManLotEnd ; }
    else if (m_iManNo ==  mcETC_AllStripOut    ){}//if(CycleManAllStripOut()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo ==  mcETC_1CycleMv       ){}//if(CycleManAllStripOut()) m_iManNo = mcNoneCycle; }

    else                                        { m_iManNo = mcNoneCycle ; }

    m_bStop = false ;
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
    if( m_bStop                 ) { m_iManNo = mcNoneCycle; m_bStop = false ;return ; } // Stop 추가 dd 0523 . PM 1h

    //St

    if(!OM.CmnOptn.bDoorSkip    ) SEQ.InspectDoor    ();
    if(m_iManNo != mcLDR_Home &&
//       m_iManNo != mcPRB_Home &&
       m_iManNo != mcRAL_Home &&
       m_iManNo != mcPSB_Home &&
       m_iManNo != mcULD_Home &&
       m_iManNo != mcAllHome    ) SEQ.InspectHomeEnd ();
    SEQ.InspectLightGrid();

    bool r1,r2,r3,r4,r5,r6 ;

    bool bRalAllNone = false ;


//    bool UdrTopExist = false ;
//    bool UdrBtmExist = false ;

    //Check Alarm.
    //if ( EM_IsErr() &&
    //     m_iManNo != mcPSB_ReviewNext &&
    //     m_iManNo != mcPSB_ReviewNextMM   ) { Init(); return ; }
    
    //Cycle Step.
    if      (m_iManNo == mcAllHome         ) { r1 = LDR.CycleHome();
                                               r2 = RAL.CycleHome();
                                               r3 = HED.CycleHome();
                                               r4 = PSB.CycleHome();
                                               r5 = ULD.CycleHome();
                                               r6 = PRB.CycleHome();
                                                   if(r1&&r2&&r3&&r4&&r5&&r6) {
                                                       m_iManNo = mcNoneCycle;
                                                       FM_MsgOk("Confirm","All Home Finished!");
                                                   }
                                              }

    else if (m_iManNo == mcLDR_Home           ) { if(LDR.CycleHome      ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Home           ) { if(LDR.CycleHome     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Supply         ) { if(LDR.CycleSupply   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Pick           ) { if(LDR.CyclePick     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Place          ) { if(LDR.CyclePlace    ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcULD_Home           ) { if(ULD.CycleHome       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Supply         ) { if(ULD.CycleSupply     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Pick           ) { if(ULD.CyclePick       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Place          ) { if(ULD.CyclePlace      ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcRAL_Home           ) { if(HED.CycleHome      ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_Home           ) { if(PSB.CycleHome      ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_Home           ) { if(PSB.CycleHome      ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_TWaitPs        ) { if(CycleManTWaitPs    ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_TFirstPs       ) { if(CycleManTFirstPs   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_TFlpInsPs      ) { if(CycleManTrmInsPs   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_TFlpNorPs      ) { if(CycleManTrmNorPs   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_TTrimAllTest   ) { if(CycleManTestTrm    ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcETC_Sampling1Cycle ) { if(CycleManBindInsp   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_Wk1AlignLiftUp ) { if(CycleManLift(1)      ) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_Wk2AlignLiftUp ) { if(CycleManLift(2)      ) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_Wk3AlignLiftUp ) { if(CycleManLift(3)      ) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_InsAlignFlp    ) { if(PSB.CycleFlip      ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_AllAlignLiftUp ) { if(CycleManLift(0)      ) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcETC_ManLotEnd      ) {
        if(bLDRPlace){
            if(LDRExist){ if(!LDR.CyclePlace()) return ; bLDRPlace = false; }
        }

        if(bULDPlace){
            if(ULDExist){ if(!ULD.CyclePlace()) return ; bULDPlace = false; }
        }
        m_iManNo = mcNoneCycle;
    }
    else if (m_iManNo == mcETC_AllStripOut    ) {
        if(bULDPlace) {
            if(ULDExist && !ULDEmpty ){ if(!ULD.CyclePlace ()) return ; }
            bULDPlace = false ;
            ULD.InitCycleStep();
         }
         if(bULDPick && !ULDEmpty) {
            if(!ULD.CyclePick  ()) return ;
            bULDPick = false ;
            m_iManStep = 10 ; m_iPreManStep = 0;

            if(IO_GetX(xPRB_Pkg)) DM.ARAY[riPRB].SetStat(csUnkwn);
            if(IO_GetX(xWR1_Pkg)) DM.ARAY[riWR1].SetStat(csUnkwn);
            if(IO_GetX(xWR2_Pkg)) DM.ARAY[riWR2].SetStat(csUnkwn);
            if(IO_GetX(xWR3_Pkg)) DM.ARAY[riWR3].SetStat(csUnkwn);
            DM.ARAY[riPSB].SetStat(csUnkwn); //PSB 는 센서가 없어요.dd

        }
        bRalAllNone = DM.ARAY[riPRB].CheckAllStat(csNone) &&
                      DM.ARAY[riWR1].CheckAllStat(csNone) &&
                      DM.ARAY[riWR2].CheckAllStat(csNone) &&
                      DM.ARAY[riWR3].CheckAllStat(csNone) &&
                      DM.ARAY[riPSB].CheckAllStat(csNone) ;

        if(!bRalAllNone || m_iManStep) {
            if(!CycleManStripRemove()) return ;
            if(!bRalAllNone) {m_iManStep = 10 ; m_iPreManStep = 0;}
        }
        if( bRalAllNone && !m_iManStep){ if(ULD.CyclePlace ()) m_iManNo = mcNoneCycle; }

    }
    else if (m_iManNo == mcETC_1CycleMv    ) { if(CycleManStripMove()    ) m_iManNo = mcNoneCycle; }
    else                                     {                             m_iManNo = mcNoneCycle; }

    //Ok.
    return ;
}

bool CManualMan::CycleManLift   (int iNo)
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

        default : if(m_iManStep) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s m_iManStep=%02d" , __FUNC__ , m_iManStep );
                      Trace("Manual Man", sTemp.c_str());
                  }
                  m_iManStep = 0 ;
                  return true ;

        case  10: if(iNo == 0 || iNo == 1){ AT_MoveCyl(aiWK1_StprUpDn , ccFwd);
                                            AT_MoveCyl(aiWK1_SttnUpDn , ccBwd);
                                            AT_MoveCyl(aiWK1_AlgnFwBw , ccBwd); }

                  if(iNo == 0 || iNo == 2){ AT_MoveCyl(aiWK2_StprUpDn , ccFwd);
                                            AT_MoveCyl(aiWK2_SttnUpDn , ccBwd);
                                            AT_MoveCyl(aiWK2_AlgnFwBw , ccBwd); }

                  if(iNo == 0 || iNo == 3){ AT_MoveCyl(aiWK3_StprUpDn , ccFwd);
                                            AT_MoveCyl(aiWK3_SttnUpDn , ccBwd);
                                            AT_MoveCyl(aiWK3_AlgnFwBw , ccBwd); }

                  m_iManStep++ ;
                  return false ;

        case  11: if(iNo == 0 || iNo == 1){ if(!AT_MoveCyl(aiWK1_StprUpDn , ccFwd)) return false;
                                            if(!AT_MoveCyl(aiWK1_SttnUpDn , ccBwd)) return false;
                                            if(!AT_MoveCyl(aiWK1_AlgnFwBw , ccBwd)) return false; }

                  if(iNo == 0 || iNo == 2){ if(!AT_MoveCyl(aiWK2_StprUpDn , ccFwd)) return false;
                                            if(!AT_MoveCyl(aiWK2_SttnUpDn , ccBwd)) return false;
                                            if(!AT_MoveCyl(aiWK2_AlgnFwBw , ccBwd)) return false; }

                  if(iNo == 0 || iNo == 3){ if(!AT_MoveCyl(aiWK3_StprUpDn , ccFwd)) return false;
                                            if(!AT_MoveCyl(aiWK3_SttnUpDn , ccBwd)) return false;
                                            if(!AT_MoveCyl(aiWK3_AlgnFwBw , ccBwd)) return false; }

                  IO_SetY(yWRK_FeedingAC,true);

                  m_tmTemp.Clear();
                  m_iManStep++ ;
                  return false ;

        case  12: if(!m_tmTemp.OnDelay(true , 500)) return false ;

                  if(iNo == 0 || iNo == 1){ AT_MoveCyl(aiWK1_AlgnFwBw , ccFwd); }
                  if(iNo == 0 || iNo == 2){ AT_MoveCyl(aiWK2_AlgnFwBw , ccFwd); }
                  if(iNo == 0 || iNo == 3){ AT_MoveCyl(aiWK3_AlgnFwBw , ccFwd); }

                  m_iManStep++ ;
                  return false ;

        case  13: if(iNo == 0 || iNo == 1){ if(!AT_MoveCyl(aiWK1_AlgnFwBw  ,ccFwd)) return false; }
                  if(iNo == 0 || iNo == 2){ if(!AT_MoveCyl(aiWK2_AlgnFwBw  ,ccFwd)) return false; }
                  if(iNo == 0 || iNo == 3){ if(!AT_MoveCyl(aiWK3_AlgnFwBw  ,ccFwd)) return false; }

                  if(iNo == 0 || iNo == 1){ AT_MoveCyl(aiWK1_SttnUpDn , ccFwd); }
                  if(iNo == 0 || iNo == 2){ AT_MoveCyl(aiWK2_SttnUpDn , ccFwd); }
                  if(iNo == 0 || iNo == 3){ AT_MoveCyl(aiWK3_SttnUpDn , ccFwd); }

                  m_iManStep++ ;
                  return false ;

        case  14: if(iNo == 0 || iNo == 1){ if(!AT_MoveCyl(aiWK1_SttnUpDn , ccFwd)) return false; }
                  if(iNo == 0 || iNo == 2){ if(!AT_MoveCyl(aiWK2_SttnUpDn , ccFwd)) return false; }
                  if(iNo == 0 || iNo == 3){ if(!AT_MoveCyl(aiWK3_SttnUpDn , ccFwd)) return false; }

                  if(iNo == 0 || iNo == 1){ AT_MoveCyl(aiWK1_AlgnFwBw , ccBwd); }
                  if(iNo == 0 || iNo == 2){ AT_MoveCyl(aiWK2_AlgnFwBw , ccBwd); }
                  if(iNo == 0 || iNo == 3){ AT_MoveCyl(aiWK3_AlgnFwBw , ccBwd); }

                  m_iManStep++ ;
                  return false ;

        case  15: if(iNo == 0 || iNo == 1){ if(!AT_MoveCyl(aiWK1_AlgnFwBw , ccBwd)) return false; }
                  if(iNo == 0 || iNo == 2){ if(!AT_MoveCyl(aiWK2_AlgnFwBw , ccBwd)) return false; }
                  if(iNo == 0 || iNo == 3){ if(!AT_MoveCyl(aiWK3_AlgnFwBw , ccBwd)) return false; }

                  IO_SetY(yWRK_FeedingAC,false);

                  m_iManStep = 0 ;
                  return true ;
    }
}


//----------------------------------------------------------------
bool CManualMan::CycleManTWaitPs ()
{
    AnsiString sTemp ;
    if(m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 10000 )) {
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

        default : if(m_iManStep) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s m_iManStep=%02d" , __FUNC__ , m_iManStep );
                      Trace("Manual Man", sTemp.c_str());
                  }
                  m_iManStep = 0 ;
                  return true ;


        case  10: AT_MoveCyl(aiPSB_PnchDnUp , ccBwd);
                  iTrmManCol = -1;
                  iTrmManRow = -1;
                  m_iManStep++;
                  return false;

        case  11: if(!AT_MoveCyl(aiPSB_PnchDnUp , ccBwd))return false;
                  MT_GoAbsMan(miPSB_XTrm , PM.GetValue(miPSB_XTrm , pvPSB_XTrmWait));
                  MT_GoAbsMan(miPSB_YTrm , PM.GetValue(miPSB_YTrm , pvPSB_YTrmWait));
                  m_iManStep++;
                  return false;

        case  12: if(!MT_GoAbsMan(miPSB_XTrm , PM.GetValue(miPSB_XTrm , pvPSB_XTrmWait))) return false;
                  if(!MT_GoAbsMan(miPSB_YTrm , PM.GetValue(miPSB_YTrm , pvPSB_YTrmWait))) return false;
                  m_iManStep = 0;
                  return true;
    }
}

bool CManualMan::CycleManTFirstPs()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 20000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        DM.ARAY[riWR1] = TempArray1 ;
        DM.ARAY[riWR2] = TempArray2 ;
        DM.ARAY[riWR3] = TempArray3 ;
        m_iManStep = 0 ;
        return true;
    }

    if(m_iManStep != m_iPreManStep) {
        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
    }

    m_iPreManStep = m_iManStep ;

    switch (m_iManStep) {

        default : if(m_iManStep) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s m_iManStep=%02d" , __FUNC__ , m_iManStep );
                      Trace("Manual Man", sTemp.c_str());
                  }
                  m_iManStep = 0 ;
                  return true ;

         case  10: AT_MoveCyl(aiPSB_PnchDnUp , ccBwd);
                   m_iManStep++;
                   return false;

         case  11: if(!AT_MoveCyl(aiPSB_PnchDnUp , ccBwd))return false;
                   MT_GoAbsMan(miPSB_XTrm , PM.GetValue(miPSB_XTrm , pvPSB_XTrmWorkStart));
                   MT_GoAbsMan(miPSB_YTrm , PM.GetValue(miPSB_YTrm , pvPSB_YTrmWorkStart));
                   m_iManStep++;
                   return false;

         case  12: if(!MT_GoAbsMan(miPSB_XTrm , PM.GetValue(miPSB_XTrm , pvPSB_XTrmWorkStart)))return false;
                   if(!MT_GoAbsMan(miPSB_YTrm , PM.GetValue(miPSB_YTrm , pvPSB_YTrmWorkStart)))return false;
                   m_iManStep = 0;
                   return true;
    }
}




bool CManualMan::CycleManBindInsp()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 20000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        DM.ARAY[riWR1] = TempArray1 ;
        DM.ARAY[riWR2] = TempArray2 ;
        DM.ARAY[riWR3] = TempArray3 ;
        m_iManStep = 0 ;
        return true;
    }

    if(m_iManStep != m_iPreManStep) {
        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
    }

    m_iPreManStep = m_iManStep ;

    switch (m_iManStep) {

        default : if(m_iManStep) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s m_iManStep=%02d" , __FUNC__ , m_iManStep );
                      Trace("Manual Man", sTemp.c_str());
                  }
                  m_iManStep = 0 ;
                  return true ;

        case  10: AT_MoveCyl(aiWK1_StprUpDn , ccFwd);
                  AT_MoveCyl(aiWK2_StprUpDn , ccFwd);
                  AT_MoveCyl(aiWK3_StprUpDn , ccFwd);
//                  AT_MoveCyl(aiWK1_FlpCmp  , ccBwd);
                  TempArray1 = DM.ARAY[riWR1] ;
                  TempArray2 = DM.ARAY[riWR2] ;
                  TempArray3 = DM.ARAY[riWR3] ;

                  m_iManStep++;
                  return false ;

        case  11: if(!AT_MoveCyl(aiWK1_StprUpDn , ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK2_StprUpDn , ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK3_StprUpDn , ccFwd)) return false ;
//                  if(!AT_MoveCyl(aiWK1_FlpCmp  , ccBwd)) return false ;
                  IO_SetY(yWRK_FeedingAC , true);

                  m_tmTemp.Clear();
                  m_iManStep++;
                  return false ;

        case  12: if(!m_tmTemp.OnDelay(true , 500)) return false ;
                  //IO_SetY(yWR1_FeedingAC , false);
                  if(!IO_GetX(xWR1_Pkg) && !DM.ARAY[riWR1].GetCntExist() ) DM.ARAY[riWR1].SetStat(csUnkwn);
                  if(!IO_GetX(xWR2_Pkg) && !DM.ARAY[riWR2].GetCntExist() ) DM.ARAY[riWR2].SetStat(csUnkwn);
                  if(!IO_GetX(xWR3_Pkg) && !DM.ARAY[riWR2].GetCntExist() ) DM.ARAY[riWR3].SetStat(csUnkwn);
                  AT_MoveCyl(aiWK1_AlgnFwBw , ccFwd);
                  AT_MoveCyl(aiWK2_AlgnFwBw , ccFwd);
                  AT_MoveCyl(aiWK3_AlgnFwBw , ccFwd);

                  m_iManStep++ ;
                  return false ;

        case  13: if(!AT_MoveCyl(aiWK1_AlgnFwBw , ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK2_AlgnFwBw , ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK3_AlgnFwBw , ccFwd)) return false ;
                  AT_MoveCyl(aiWK1_SttnUpDn , ccFwd);
                  AT_MoveCyl(aiWK2_SttnUpDn , ccFwd);
                  AT_MoveCyl(aiWK3_SttnUpDn , ccFwd);
                  //AT_MoveCyl(aiWK1_FlpCmp , ccFwd);

                  m_iManStep++ ;
                  return false ;

        case  14: if(!AT_MoveCyl(aiWK1_SttnUpDn , ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK2_SttnUpDn , ccFwd)) return false ;
                  if(!AT_MoveCyl(aiWK3_SttnUpDn , ccFwd)) return false ;
                  //if(!AT_MoveCyl(aiWK1_FlpCmp , ccFwd)) return false ;
                  IO_SetY(yWRK_FeedingAC , false);
                  AT_MoveCyl(aiWK1_AlgnFwBw , ccBwd);
                  AT_MoveCyl(aiWK2_AlgnFwBw , ccBwd);
                  AT_MoveCyl(aiWK3_AlgnFwBw , ccBwd);

                  m_iManStep++ ;
                  return false ;

        case  15: if(!AT_MoveCyl(aiWK1_AlgnFwBw , ccBwd)) return false ;
                  if(!AT_MoveCyl(aiWK2_AlgnFwBw , ccBwd)) return false ;
                  if(!AT_MoveCyl(aiWK3_AlgnFwBw , ccBwd)) return false ;
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
                  AT_MoveCyl(aiWK1_StprUpDn , ccFwd);
                  AT_MoveCyl(aiWK2_StprUpDn , ccFwd);
                  AT_MoveCyl(aiWK3_StprUpDn , ccFwd);

                  m_iManStep++ ;
                  return false ;

        case  17: if(!AT_MoveCyl(aiWK1_StprUpDn , ccFwd)) return false;
                  if(!AT_MoveCyl(aiWK2_StprUpDn , ccFwd)) return false;
                  if(!AT_MoveCyl(aiWK3_StprUpDn , ccFwd)) return false;
                  AT_MoveCyl(aiWK1_SttnUpDn , ccBwd);
                  AT_MoveCyl(aiWK2_SttnUpDn , ccBwd);
                  AT_MoveCyl(aiWK3_SttnUpDn , ccBwd);
//                  AT_MoveCyl(aiWK1_FlpCmp , ccBwd);

                  m_iManStep++ ;
                  return false ;

        case  18: if(!AT_MoveCyl(aiWK1_SttnUpDn , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK2_SttnUpDn , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK3_SttnUpDn , ccBwd)) return false;
                  //if(!AT_MoveCyl(aiWK1_FlpCmp , ccBwd)) return false;
                  HED.DpAray1 = DM.ARAY[riWR1] ;
                  HED.DpAray2 = DM.ARAY[riWR2] ;
                  HED.DpAray3 = DM.ARAY[riWR3] ;

                  DM.ARAY[riWR1] = TempArray1 ;
                  DM.ARAY[riWR2] = TempArray2 ;
                  DM.ARAY[riWR3] = TempArray3 ;
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
//      DM.ARAY[riPSB] = DM.ARAY[riWB1] ;
//      DM.ARAY[riWR2] = DM.ARAY[riWB2] ;
//      DM.ARAY[riWR3] = DM.ARAY[riWB3] ; 이거 몬지잘 모르겟음.dd
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

        default : if(m_iManStep) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s m_iManStep=%02d" , __FUNC__ , m_iManStep );
                      Trace("Manual Man", sTemp.c_str());
                  }
                  m_iManStep = 0 ;
                  return true ;

        case  10: if(bLDRPlace){
                      if(LDRExist){if(!LDR.CyclePlace()) return false; bLDRPlace = false; }
                  }

                  m_iManStep++;
                  return false;

        case  11:
                  m_iManStep++;
                  return false;

        case  12: if(bULDPlace){
                      if(ULDExist){ if(!ULD.CyclePlace()) return false; bULDPlace = false; }
                  }
                  m_iManStep++;
                  return false;

        case  13: 
                  m_iManStep++;
                  return false;

        case  14: m_iManStep = 0 ;
                  return true ;
    }
}
//--------------------------------------------------------------------
bool CManualMan::CycleManStripRemove()
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

    static bool bCanMvPRB   = false ;
    static bool bCanMvWK1   = false ;
    static bool bCanMvWK2   = false ;
    static bool bCanMvWK3   = false ;
    static bool bCanMvPSB   = false ;

    static bool bPSBPassIn  = false ;
    static bool bPSBPassOut = false ;

    static bool bTm1 = false;
    static bool bTm2 = false;

    bool r1 , r2 ;
//    bool bULDPlace = false;

    if(IO_GetXDn(xPSB_PkgIn2)) bPSBPassIn = true ;
    if(bPSBPassIn) if(!IO_GetX(xPSB_PkgIn2)) bPSBPassOut = true ;

    switch (m_iManStep) {

        default : if(m_iManStep) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s m_iManStep=%02d" , __FUNC__ , m_iManStep );
                      Trace("Manual Man", sTemp.c_str());
                  }
                  m_iManStep = 0 ;
                  return true ;

        case  10: bPSBPassIn  = false ;
                  bPSBPassOut = false ;
                  m_iManStep++;
                  return false;

        case  11:
                  bTm1 = !DM.ARAY[riPSB].CheckAllStat(csNone);
                  bCanMvPSB =  !DM.ARAY[riPSB].CheckAllStat(csNone) && !DM.ARAY[riULD].CheckAllStat(csNone) ; //워크위치는 태워 줘야 함.dd
                  bCanMvWK3 = (!DM.ARAY[riWR3].CheckAllStat(csNone) && (DM.ARAY[riPSB].CheckAllStat(csNone) ));//||bCanMvPSB)) ;
                  bCanMvWK2 = (!DM.ARAY[riWR2].CheckAllStat(csNone) && (DM.ARAY[riWR3].CheckAllStat(csNone) ||bCanMvWK3)) ;
                  bCanMvWK1 = (!DM.ARAY[riWR1].CheckAllStat(csNone) && (DM.ARAY[riWR2].CheckAllStat(csNone) ||bCanMvWK2)) ;
                  bCanMvPRB = (!DM.ARAY[riPRB].CheckAllStat(csNone) && (DM.ARAY[riWR1].CheckAllStat(csNone) ||bCanMvWK1)) ;

                  AT_MoveCyl(aiWK1_SttnUpDn , ccBwd);
                  AT_MoveCyl(aiWK2_SttnUpDn , ccBwd);
                  AT_MoveCyl(aiWK3_SttnUpDn , ccBwd);
                  AT_MoveCyl(aiWK1_AlgnFwBw , ccBwd);
                  AT_MoveCyl(aiWK2_AlgnFwBw , ccBwd);
                  AT_MoveCyl(aiWK3_AlgnFwBw , ccBwd);
                  AT_MoveCyl(aiPRB_StprUpDn , ccFwd);
                  AT_MoveCyl(aiWK1_StprUpDn , ccFwd);
                  AT_MoveCyl(aiWK2_StprUpDn , ccFwd);
                  AT_MoveCyl(aiWK1_StprUpDn , ccFwd);
                  AT_MoveCyl(aiPSB_FlprOpCl , ccFwd);
                  AT_MoveCyl(aiPSB_IndxUpDn , ccBwd);
                  AT_MoveCyl(aiPSB_PshrFwBw , ccBwd);
                  AT_MoveCyl(aiPSB_PshrDnUp , ccBwd);
                  AT_MoveCyl(aiPSB_PnchDnUp , ccBwd);

                  m_iManStep++;
                  return false;

        case  12: if(!AT_MoveCyl(aiWK1_SttnUpDn , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK2_SttnUpDn , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK3_SttnUpDn , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK1_AlgnFwBw , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK2_AlgnFwBw , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK3_AlgnFwBw , ccBwd)) return false;
                  if(!AT_MoveCyl(aiPRB_StprUpDn , ccFwd)) return false;
                  if(!AT_MoveCyl(aiWK1_StprUpDn , ccFwd)) return false;
                  if(!AT_MoveCyl(aiWK2_StprUpDn , ccFwd)) return false;
                  if(!AT_MoveCyl(aiWK1_StprUpDn , ccFwd)) return false;
                  if(!AT_MoveCyl(aiPSB_FlprOpCl , ccFwd)) return false;
                  if(!AT_MoveCyl(aiPSB_IndxUpDn , ccBwd)) return false;
                  if(!AT_MoveCyl(aiPSB_PshrFwBw , ccBwd)) return false;
                  if(!AT_MoveCyl(aiPSB_PshrDnUp , ccBwd)) return false;
                  if(!AT_MoveCyl(aiPSB_PnchDnUp , ccBwd)) return false;

                  MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns,pvPSB_XInsWait));
                  MT_GoAbsMan(miPSB_XTrm , PM.GetValue(miPSB_XTrm,pvPSB_XTrmWait));
                  MT_GoAbsMan(miPSB_YTrm , PM.GetValue(miPSB_YTrm,pvPSB_YTrmWait));

                  m_iManStep++;
                  return false;

        case  13: if(!MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns,pvPSB_XInsWait))) return false;
                  if(!MT_GoAbsMan(miPSB_XTrm , PM.GetValue(miPSB_XTrm,pvPSB_XTrmWait))) return false;
                  if(!MT_GoAbsMan(miPSB_YTrm , PM.GetValue(miPSB_YTrm,pvPSB_YTrmWait))) return false;
                  MT_GoAbsMan(miPSB_TFlp , PM.GetValue(miPSB_TFlp,pvPSB_TFlpWait));
                  m_iManStep++;
                  return false;


        case  14: if(!MT_GoAbsMan(miPSB_TFlp , PM.GetValue(miPSB_TFlp,pvPSB_TFlpWait))) return false; //Ready dd
                  if(!bCanMvPSB && bCanMvWK3) AT_MoveCyl(aiPSB_FlprOpCl,ccBwd);
        //PostBuff Out Motion
                  if(bCanMvPSB) {
                      ULD.MoveMotr(miULD_ZCmp,piULD_ZWork);
                  }
                  m_iManStep++;
                  return false;

        case  15: if(bCanMvPSB) if(!ULD.MoveMotr(miULD_ZCmp,piULD_ZWork)) return false ;

                  m_iManStep++;
                  return false;

        case  16: if(bCanMvPSB) AT_MoveCyl(aiPSB_IndxUpDn , ccFwd);
                  if(bCanMvPSB) AT_MoveCyl(aiPSB_FlprOpCl , ccBwd);

                  m_iManStep++;
                  return false;

        case  17: if(bCanMvPSB) if(!AT_MoveCyl(aiPSB_IndxUpDn , ccFwd)) return false ;
                  if(bCanMvPSB) if(!AT_MoveCyl(aiPSB_FlprOpCl , ccBwd)) return false ;
                  if(bCanMvPSB) MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns,pvPSB_XInsOut));
                  m_iManStep++;
                  return false;


        case  18: if(IO_GetX(xPSB_InsertOverload)) {
                      EM_SetErr( eiPSB_InsertOverload );
                      MT_Stop(miPSB_XIns);
                      m_iManStep = 0 ;
                      return true ;
                  }
                  if(bCanMvPSB) if(!MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns,pvPSB_XInsOut))) return false ;

                  if(bCanMvPSB) MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns,pvPSB_XInsWait)) ;
                  if(bCanMvPSB) AT_MoveCyl(aiPSB_PshrDnUp , ccFwd);

                  m_iManStep++;
                  return false;

        case  19: if(bCanMvPSB) if(!AT_MoveCyl(aiPSB_PshrDnUp , ccFwd)) return false ;
                  if(bCanMvPSB) AT_MoveCyl(aiPSB_PshrFwBw , ccFwd);

                  m_iManStep++;
                  return false;

        case  20: if(bCanMvPSB) if(!AT_MoveCyl(aiPSB_PshrFwBw , ccFwd)) return false ;
                  if(bCanMvPSB) if(!MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns,pvPSB_XInsWait))) return false ;

                  if(bCanMvPSB) {
                      if( IO_GetX(xPSB_PkgOut3) || IO_GetX(xPSB_PkgOut4)) {
                          EM_SetErr(eiPSB_PusherMiss);
                      }
                  }

                  if(bCanMvPSB) {
                      if(DM.ARAY[riULD].GetCntStat(csEmpty)) DM.ARAY[riULD].SetStat(DM.ARAY[riULD].FindLastRow(csEmpty),0 , csWork);
                  }

                  if(bCanMvPSB) DM.ARAY[riPSB].SetStat(csNone) ;

                  if(bCanMvPSB) AT_MoveCyl(aiPSB_PshrFwBw , ccBwd);
                  if(bCanMvPSB) AT_MoveCyl(aiPSB_PshrDnUp , ccBwd);
                  if(bCanMvPSB) AT_MoveCyl(aiPSB_IndxUpDn , ccBwd);
                  m_iManStep++;
                  return false;

        case  21: if(bCanMvPSB) if(!AT_MoveCyl(aiPSB_PshrFwBw , ccBwd))return false;
                  if(bCanMvPSB) if(!AT_MoveCyl(aiPSB_PshrDnUp , ccBwd))return false;
                  if(bCanMvPSB) if(!AT_MoveCyl(aiPSB_IndxUpDn , ccBwd))return false;

                  if(!bCanMvPSB && bCanMvWK3) if(!AT_MoveCyl(aiPSB_FlprOpCl,ccBwd)) return false ;

                  IO_SetY(yWRK_FeedingAC, true);
                  if(bCanMvWK3){
                      AT_MoveCyl(aiWK3_StprUpDn , ccBwd);

                      RAL.m_sWorkedLotNo = DM.ARAY[riWR3].GetLotNo();
                      RAL.m_sWorkedId    = DM.ARAY[riWR3].GetID   ();

                      DM.ShiftArrayData(riWR3 , riPSB);
                      DM.ARAY[riLot] = DM.ARAY[riPSB] ;

                      if(OM.CmnOptn.bTrmAllSkip) DM.ARAY[riPSB].SetStat(csWork);

                  }
                  m_tmTemp.Clear();
                  m_iManStep++;
                  return false;

        case  22: if(!m_tmTemp.OnDelay(true,OM.CmnOptn.iWK2StprDnDelay)) return false ;
                  if(bCanMvWK2){
                      AT_MoveCyl(aiWK2_StprUpDn , ccBwd);
                      DM.ShiftArrayData(riWR2 , riWR3);
                      DM.ARAY[riWR3].ChangeStat(csWork , csUnkwn);
                      DM.ARAY[riWR3].SetStep(asInspNeed);
                      RAL.riWR3Buff = DM.ARAY[riWR3] ;
                  }

                  m_tmTemp.Clear();
                  m_iManStep++;
                  return false;

        case  23: if(!m_tmTemp.OnDelay(true,OM.CmnOptn.iWK1StprDnDelay)) return false ;
                  if(bCanMvWK1){
                      AT_MoveCyl(aiWK1_StprUpDn , ccBwd);
                      DM.ShiftArrayData(riWR1 , riWR2);
                      DM.ARAY[riWR2].ChangeStat(csWork , csUnkwn);
                      DM.ARAY[riWR2].SetStep(asInspNeed);
                      RAL.riWR2Buff = DM.ARAY[riWR2] ;
                  }
                  m_iManStep++;
                  return false;

        case  24: if(bCanMvPRB){
                      IO_SetY(yPRB_FeedingAC , true );
                      AT_MoveCyl(aiPRB_StprUpDn,ccBwd);
                      PRB.m_sWorkedLotNo = DM.ARAY[riPRB].GetLotNo() ;
                      PRB.m_sWorkedId    = DM.ARAY[riPRB].GetID   () ;
                      DM.ShiftArrayData(riPRB , riWR1);
                      DM.ARAY[riWR1].SetStep(asInspNeed);
                      RAL.riWR1Buff = DM.ARAY[riWR1] ;

                  }
                  m_iManStep++;
                  return false;

        case  25: if(!IO_GetX(xWR3_Pkg)) AT_MoveCyl(aiWK3_StprUpDn , ccFwd);
                  if(!IO_GetX(xWR2_Pkg)) AT_MoveCyl(aiWK2_StprUpDn , ccFwd);
                  if(!IO_GetX(xWR1_Pkg)) AT_MoveCyl(aiWK1_StprUpDn , ccFwd);
                  if(!IO_GetX(xPRB_Pkg)) AT_MoveCyl(aiPRB_StprUpDn , ccFwd);

                  if(AT_GetCmd(aiPRB_StprUpDn) != ccFwd) return false ;
                  if(AT_GetCmd(aiWK1_StprUpDn) != ccFwd) return false ;
                  if(AT_GetCmd(aiWK2_StprUpDn) != ccFwd) return false ;
                  if(AT_GetCmd(aiWK3_StprUpDn) != ccFwd) return false ;

                  if(!bPSBPassIn  && bCanMvWK3) return false ; // dd
                  if(!bPSBPassOut && bCanMvWK3) return false ; // dd

                  if(bCanMvWK2 && !IO_GetX(xWR3_Pkg)) return false ; //dd
                  if(bCanMvWK1 && !IO_GetX(xWR2_Pkg)) return false ; //dd
                  if(bCanMvPRB && !IO_GetX(xWR1_Pkg)) return false ;
                  IO_SetY(yWRK_FeedingAC,false);
                  IO_SetY(yPRB_FeedingAC,false);
                  if(m_iManNo == mcETC_AllStripOut && DM.ARAY[riPSB].CheckAllStat(csNone)) {
                      ULD.InitCycleStep();
                  }
                  m_iManStep = 0;
                  return true ;
        }
}
//--------------------------------------------------------------------
bool CManualMan::CycleManStripMove()
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

    static bool bCanMvPRB   = false ;
    static bool bCanMvWK1   = false ;
    static bool bCanMvWK2   = false ;
    static bool bCanMvWK3   = false ;
    static bool bCanMvPSB   = false ;

    static bool bPSBPassIn  = false ;
    static bool bPSBPassOut = false ;

    bool r1 , r2 ;
//    bool bULDPlace = false;

    if(IO_GetXDn(xPSB_PkgIn2)) bPSBPassIn = true ;
    if(bPSBPassIn) if(!IO_GetX(xPSB_PkgIn2)) bPSBPassOut = true ;

    switch (m_iManStep) {

        default : if(m_iManStep) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s m_iManStep=%02d" , __FUNC__ , m_iManStep );
                      Trace("Manual Man", sTemp.c_str());
                  }
                  m_iManStep = 0 ;
                  return true ;

        case  10: bPSBPassIn  = false ;
                  bPSBPassOut = false ;
                  m_iManStep++;
                  return false;

        case  11:
                  bCanMvPSB =  !DM.ARAY[riPSB].CheckAllStat(csNone) && !DM.ARAY[riULD].CheckAllStat(csNone) ; //워크위치는 태워 줘야 함.dd
                  bCanMvWK3 = (!DM.ARAY[riWR3].CheckAllStat(csNone) && (DM.ARAY[riPSB].CheckAllStat(csNone) ||bCanMvPSB)) ;
                  bCanMvWK2 = (!DM.ARAY[riWR2].CheckAllStat(csNone) && (DM.ARAY[riWR3].CheckAllStat(csNone) ||bCanMvWK3)) ;
                  bCanMvWK1 = (!DM.ARAY[riWR1].CheckAllStat(csNone) && (DM.ARAY[riWR2].CheckAllStat(csNone) ||bCanMvWK2)) ;
                  bCanMvPRB = (!DM.ARAY[riPRB].CheckAllStat(csNone) && (DM.ARAY[riWR1].CheckAllStat(csNone) ||bCanMvWK1)) ;

                  AT_MoveCyl(aiWK1_SttnUpDn , ccBwd);
                  AT_MoveCyl(aiWK2_SttnUpDn , ccBwd);
                  AT_MoveCyl(aiWK3_SttnUpDn , ccBwd);
                  AT_MoveCyl(aiWK1_AlgnFwBw , ccBwd);
                  AT_MoveCyl(aiWK2_AlgnFwBw , ccBwd);
                  AT_MoveCyl(aiWK3_AlgnFwBw , ccBwd);
                  AT_MoveCyl(aiPRB_StprUpDn , ccFwd);
                  AT_MoveCyl(aiWK1_StprUpDn , ccFwd);
                  AT_MoveCyl(aiWK2_StprUpDn , ccFwd);
                  AT_MoveCyl(aiWK1_StprUpDn , ccFwd);
                  AT_MoveCyl(aiPSB_FlprOpCl , ccFwd);
                  AT_MoveCyl(aiPSB_IndxUpDn , ccBwd);
                  AT_MoveCyl(aiPSB_PshrFwBw , ccBwd);
                  AT_MoveCyl(aiPSB_PshrDnUp , ccBwd);
                  AT_MoveCyl(aiPSB_PnchDnUp , ccBwd);
                  m_iManStep++;
                  return false;

        case  12: if(!AT_MoveCyl(aiWK1_SttnUpDn , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK2_SttnUpDn , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK3_SttnUpDn , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK1_AlgnFwBw , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK2_AlgnFwBw , ccBwd)) return false;
                  if(!AT_MoveCyl(aiWK3_AlgnFwBw , ccBwd)) return false;
                  if(!AT_MoveCyl(aiPRB_StprUpDn , ccFwd)) return false;
                  if(!AT_MoveCyl(aiWK1_StprUpDn , ccFwd)) return false;
                  if(!AT_MoveCyl(aiWK2_StprUpDn , ccFwd)) return false;
                  if(!AT_MoveCyl(aiWK1_StprUpDn , ccFwd)) return false;
                  if(!AT_MoveCyl(aiPSB_FlprOpCl , ccFwd)) return false;
                  if(!AT_MoveCyl(aiPSB_IndxUpDn , ccBwd)) return false;
                  if(!AT_MoveCyl(aiPSB_PshrFwBw , ccBwd)) return false;
                  if(!AT_MoveCyl(aiPSB_PshrDnUp , ccBwd)) return false;
                  if(!AT_MoveCyl(aiPSB_PnchDnUp , ccBwd)) return false;

//                  MT_GoAbsMan(miPSB_TFlp , PM.GetValue(miPSB_TFlp,pvPSB_TFlpWait));
                  MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns,pvPSB_XInsWait));
                  MT_GoAbsMan(miPSB_YTrm , PM.GetValue(miPSB_YTrm,pvPSB_YTrmWait));

                  m_iManStep++;
                  return false;

        case  13: //if(!MT_GoAbsMan(miPSB_TFlp , PM.GetValue(miPSB_TFlp,pvPSB_TFlpWait))) return false;
                  if(!MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns,pvPSB_XInsWait))) return false;
                  if(!MT_GoAbsMan(miPSB_YTrm , PM.GetValue(miPSB_YTrm,pvPSB_YTrmWait))) return false;
                  MT_GoAbsMan(miPSB_XTrm , PM.GetValue(miPSB_XTrm,pvPSB_XTrmWait));
                  m_iManStep++;
                  return false;

        case  14: if(!MT_GoAbsMan(miPSB_XTrm , PM.GetValue(miPSB_XTrm,pvPSB_XTrmWait))) return false;
                  MT_GoAbsMan(miPSB_TFlp , PM.GetValue(miPSB_TFlp,pvPSB_TFlpWait));
                  m_iManStep++;
                  return false;


        case  15: if(!MT_GoAbsMan(miPSB_TFlp , PM.GetValue(miPSB_TFlp,pvPSB_TFlpWait))) return false; //Ready dd
                  if(bCanMvWK3) AT_MoveCyl(aiPSB_FlprOpCl,ccBwd);
                  m_iManStep = 21;
                  return false;
        //PostBuff Out Motion
//                  if(bCanMvPSB) {
////                      if(!ULD.Autorun()) {
////                          if(ULD.GetSeqStep()  != CUnLoader::scIdle) return false ; //ULD.MoveMotr(miULD_ZTop,piULD_ZTopWork);
////                          if(ULD.GetSeqBStep() != CUnLoader::scIdle) return false ; //ULD.MoveMotr(miULD_ZTop,piULD_ZTopWork);
//                      ULD.MoveMotr(miULD_ZTop,piULD_ZTopWork);
//                  }
//                  m_iManStep++;
//                  return false;
//
//        case  15: if(bCanMvPSB) if(!ULD.MoveMotr(miULD_ZTop,piULD_ZTopWork)) return false ;
//
//                  m_iManStep++;
//                  return false;
//
//        case  16: if(bCanMvPSB) AT_MoveCyl(aiPSB_Insert  , ccFwd);
//                  if(bCanMvPSB) AT_MoveCyl(aiPSB_FlprCmp , ccBwd);
//
//                  m_iManStep++;
//                  return false;
//
//        case  17: if(bCanMvPSB) if(!AT_MoveCyl(aiPSB_Insert  , ccFwd)) return false ;
//                  if(bCanMvPSB) if(!AT_MoveCyl(aiPSB_FlprCmp , ccBwd)) return false ;
//                  if(bCanMvPSB) MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns,pvPSB_XInsOut));
//                  m_iManStep++;
//                  return false;
//
//
//        case  18: if(IO_GetX(xPSB_InsertOverload)) {
//                      EM_SetErr( eiPSB_InsertOverload );
//                      MT_Stop(miPSB_XIns);
//                      m_iManStep = 0 ;
//                      return true ;
//                  }
//                  if(bCanMvPSB) if(!MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns,pvPSB_XInsOut))) return false ;
//
//                  if(bCanMvPSB) MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns,pvPSB_XInsWait)) ;
//                  if(bCanMvPSB) AT_MoveCyl(aiPSB_PusherUD , ccFwd);
//
//                  m_iManStep++;
//                  return false;
//
//        case  19: if(bCanMvPSB) if(!AT_MoveCyl(aiPSB_PusherUD , ccFwd)) return false ;
//                  if(bCanMvPSB) AT_MoveCyl(aiPSB_PusherFB , ccFwd);
//
//                  m_iManStep++;
//                  return false;
//
//        case  20: if(bCanMvPSB) if(!AT_MoveCyl(aiPSB_PusherFB , ccFwd)) return false ;
//                  if(bCanMvPSB) if(!MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns,pvPSB_XInsWait))) return false ;
//
//                  if(bCanMvPSB) {
//                      if( IO_GetX(xPSB_PkgOut3) || IO_GetX(xPSB_PkgOut4)) {
//                          EM_SetErr(eiPSB_PusherMiss);
//                      }
//                  }
//
//                  if(bCanMvPSB) {
//                      if(DM.ARAY[riULT].GetCntStat(csEmpty)) DM.ARAY[riULT].SetStat(DM.ARAY[riULT].FindLastRow(csEmpty),0 , csWork);
//                  }
//
//                  if(bCanMvPSB) DM.ARAY[riPSB].SetStat(csNone) ;
//                  if(bCanMvPSB) DM.ARAY[riVS4].SetStat(csNone) ;
//
//                  if(bCanMvPSB) AT_MoveCyl(aiPSB_PusherFB , ccBwd);
//                  if(bCanMvPSB) AT_MoveCyl(aiPSB_PusherUD , ccBwd);
//                  if(bCanMvPSB) AT_MoveCyl(aiPSB_Insert   , ccBwd);
//                  m_iManStep++;
//                  return false;
//
//        case  21: if(bCanMvPSB) if(!AT_MoveCyl(aiPSB_PusherFB , ccBwd))return false;
//                  if(bCanMvPSB) if(!AT_MoveCyl(aiPSB_PusherUD , ccBwd))return false;
//                  if(bCanMvPSB) if(!AT_MoveCyl(aiPSB_Insert   , ccBwd))return false;
        case  21: if(bCanMvWK3) if(!AT_MoveCyl(aiPSB_FlprOpCl,ccBwd)) return false ;
                  IO_SetY(yWRK_FeedingAC , true);
                  if(bCanMvWK3){

                      AT_MoveCyl(aiWK3_StprUpDn , ccBwd);

                      RAL.m_sWorkedLotNo = DM.ARAY[riWR3].GetLotNo();
                      RAL.m_sWorkedId    = DM.ARAY[riWR3].GetID   ();

                      DM.ShiftArrayData(riWR3 , riPSB);
                      DM.ARAY[riLot] = DM.ARAY[riPSB] ;

                      //Option Add
//                      if(OM.CmnOptn.bTrm1Skip) {//마킹 하는 자재를 스킵 하거나 없으면 지금 자재를 스킵.
//                          DM.ARAY[riPSB].SetStat(csWork);
//                          OM.CmnOptn.bTrm1Skip = false ;
//                          bTrmOneSkip          = true  ; //체크박스 풀기용.Operation 폼
//                      }

                      if(OM.CmnOptn.bTrmAllSkip) DM.ARAY[riPSB].SetStat(csWork);

                  }
                  m_tmTemp.Clear();
                  m_iManStep++;
                  return false;

        case  22: if(!m_tmTemp.OnDelay(true,OM.CmnOptn.iWK2StprDnDelay)) return false ;
                  if(bCanMvWK2){
                      AT_MoveCyl(aiWK2_StprUpDn , ccBwd);
                      DM.ShiftArrayData(riWR2 , riWR3);
                      DM.ARAY[riWR3].ChangeStat(csWork , csUnkwn);
                      DM.ARAY[riWR3].SetStep(asInspNeed);
                      RAL.riWR3Buff = DM.ARAY[riWR3] ;
                  }

                  m_tmTemp.Clear();
                  m_iManStep++;
                  return false;

        case  23: if(!m_tmTemp.OnDelay(true,OM.CmnOptn.iWK1StprDnDelay)) return false ;
                  if(bCanMvWK1){
                      AT_MoveCyl(aiWK1_StprUpDn , ccBwd);
                      DM.ShiftArrayData(riWR1 , riWR2);
                      DM.ARAY[riWR2].ChangeStat(csWork , csUnkwn);
                      DM.ARAY[riWR2].SetStep(asInspNeed);
                      RAL.riWR2Buff = DM.ARAY[riWR2] ;
                  }
                  m_iManStep++;
                  return false;

        case  24: if(bCanMvPRB){
                      IO_SetY(yPRB_FeedingAC , true );
                      AT_MoveCyl(aiPRB_StprUpDn,ccBwd);
                      PRB.m_sWorkedLotNo = DM.ARAY[riPRB].GetLotNo() ;
                      PRB.m_sWorkedId    = DM.ARAY[riPRB].GetID   () ;
                      DM.ShiftArrayData(riPRB , riWR1);
                      DM.ARAY[riWR1].SetStep(asInspNeed);
                      RAL.riWR1Buff = DM.ARAY[riWR1] ;

                  }
                  m_iManStep++;
                  return false;

        case  25: if(!IO_GetX(xWR3_Pkg)) AT_MoveCyl(aiWK3_StprUpDn , ccFwd);
                  if(!IO_GetX(xWR2_Pkg)) AT_MoveCyl(aiWK2_StprUpDn , ccFwd);
                  if(!IO_GetX(xWR1_Pkg)) AT_MoveCyl(aiWK1_StprUpDn , ccFwd);
                  if(!IO_GetX(xPRB_Pkg)) AT_MoveCyl(aiPRB_StprUpDn , ccFwd);

                  if(AT_GetCmd(aiPRB_StprUpDn) != ccFwd) return false ;
                  if(AT_GetCmd(aiWK1_StprUpDn) != ccFwd) return false ;
                  if(AT_GetCmd(aiWK2_StprUpDn) != ccFwd) return false ;
                  if(AT_GetCmd(aiWK3_StprUpDn) != ccFwd) return false ;

                  if(!bPSBPassIn  && bCanMvWK3) return false ; // dd
                  if(!bPSBPassOut && bCanMvWK3) return false ; // dd

                  if(bCanMvWK2 && !IO_GetX(xWR3_Pkg)) return false ; //dd
                  if(bCanMvWK1 && !IO_GetX(xWR2_Pkg)) return false ; //dd
                  if(bCanMvPRB && !IO_GetX(xWR1_Pkg)) return false ;
                  IO_SetY(yWRK_FeedingAC,false);
                  IO_SetY(yPRB_FeedingAC,false);
                  if(m_iManNo == mcETC_AllStripOut && DM.ARAY[riPSB].CheckAllStat(csNone)) {
                      ULD.InitCycleStep();
                  }
                  m_iManStep = 0;
                  return true ;
        }
}


//--------------------------------------------------------------------

bool CManualMan::CycleManTestTrm()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        DM.ARAY[riPSB] = TempArray1 ;
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

        case  10: m_bStopTestTrm = false ;
                  TempArray1 = DM.ARAY[riPSB] ;
                  DM.ARAY[riPSB].SetStat(csRslt1);

                  m_iManStep++ ;
                  return false ;

        case  11: AT_MoveCyl(aiPSB_IndxUpDn , ccBwd);
                  AT_MoveCyl(aiPSB_PnchDnUp , ccBwd);

                  m_iManStep++;
                  return false;

        case  12: if(!AT_MoveCyl(aiPSB_IndxUpDn , ccBwd)) return false ;
                  if(!AT_MoveCyl(aiPSB_PnchDnUp , ccBwd)) return false;
                  MT_GoAbsMan(miPSB_XTrm , PM.GetValue(miPSB_XTrm , pvPSB_XTrmWait));
                  MT_GoAbsMan(miPSB_YTrm , PM.GetValue(miPSB_YTrm , pvPSB_YTrmWait));
                  MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns , pvPSB_XInsWait));

                  m_iManStep++;
                  return false;

        case  13: if(!MT_GoAbsMan(miPSB_XTrm , PM.GetValue(miPSB_XTrm , pvPSB_XTrmWait))) return false;
                  if(!MT_GoAbsMan(miPSB_YTrm , PM.GetValue(miPSB_YTrm , pvPSB_YTrmWait))) return false;
                  AT_MoveCyl(aiPSB_FlprOpCl , ccBwd);
                  AT_MoveCyl(aiPSB_IndxUpDn , ccFwd);

                  m_iManStep++;
                  return false;

        case  14: if(!AT_MoveCyl(aiPSB_FlprOpCl , ccBwd)) return false ;
                  if(!AT_MoveCyl(aiPSB_IndxUpDn , ccFwd)) return false ;
                  MT_GoAbsRun(miPSB_XIns , PM.GetValue(miPSB_XIns , pvPSB_XInsInsert)-5.0);

                  m_iManStep++;
                  return false;

        case  15: if(!MT_GetStopInpos(miPSB_XIns)) return false ;
                  MT_GoAbs(miPSB_XIns , PM.GetValue(miPSB_XIns , pvPSB_XInsInsert), 10.0);

                  m_iManStep++;
                  return false;

        case  16: if(!MT_GetStopInpos(miPSB_XIns)) return false ;
                  AT_MoveCyl(aiPSB_AlgnFwBw , ccFwd);

                  m_iManStep++;
                  return false;

        case  17: if(!AT_MoveCyl(aiPSB_AlgnFwBw , ccFwd)) return false ;
                  AT_MoveCyl(aiPSB_FlprOpCl , ccFwd);

                  m_iManStep++;
                  return false;

        case  18: if(!AT_MoveCyl(aiPSB_FlprOpCl ,ccFwd)) return false ;
                  MT_GoAbsMan(miPSB_TFlp , PM.GetValue(miPSB_TFlp , pvPSB_TFlpWork));
                  MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns , pvPSB_XInsWait));
                  AT_MoveCyl(aiPSB_AlgnFwBw , ccBwd);
                  AT_MoveCyl(aiPSB_IndxUpDn , ccBwd);

                  m_iManStep++;
                  return false;

        case  19: if(!MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns , pvPSB_XInsWait)))return false;
                  if(!MT_GoAbsMan(miPSB_TFlp , PM.GetValue(miPSB_TFlp , pvPSB_TFlpWork)))return false;
                  AT_MoveCyl(aiPSB_GripUpDn , ccFwd);
                  OM.CmnOptn.bTrm1Skip   = false;
                  OM.CmnOptn.bTrmAllSkip = false;
                  PSB.InitCycleStep();

                  m_iManStep++ ;
                  return false ;

        case  20: if(!AT_MoveCyl(aiPSB_GripUpDn , ccFwd)) return false;
                  m_tmCycle.Clear();
                  if(!PSB.CycleWork()) return false ;
                  if(DM.ARAY[riPSB].GetCntStat(csRslt1) && !m_bStopTestTrm ){
                      m_iManStep = 18 ;
                      return false ;
                  }
                  m_iManStep++ ;
                  return false ;

//        case  21: AT_MoveCyl(aiPSB_FlprCmp , ccBwd);
//                  m_iManStep++ ;
//                  return false ;

//        case  22: if(!AT_MoveCyl(aiPSB_FlprCmp , ccBwd)) return false ;
        case  21: MT_GoAbsMan(miPSB_YTrm, PM.GetValue(miPSB_YTrm , pvPSB_YTrmWait));

                  m_iManStep++;
                  return false;

        case  22: if(!MT_GoAbsMan(miPSB_YTrm, PM.GetValue(miPSB_YTrm , pvPSB_YTrmWait))) return false;
                  MT_GoAbsMan(miPSB_XTrm, PM.GetValue(miPSB_XTrm , pvPSB_XTrmWait));
                  m_iManStep++;
                  return false;

        case  23: if(!MT_GoAbsMan(miPSB_XTrm, PM.GetValue(miPSB_XTrm , pvPSB_XTrmWait))) return false;
                  DM.ARAY[riPSB] = TempArray1 ;
                  MT_GoAbsMan(miPSB_TFlp , PM.GetValue(miPSB_TFlp , pvPSB_TFlpWait));

                  m_iManStep++;
                  return false;

        case  24: if(!MT_GoAbsMan(miPSB_TFlp , PM.GetValue(miPSB_TFlp , pvPSB_TFlpWait))) return false;
                  AT_MoveCyl(aiPSB_FlprOpCl , ccBwd);

                  m_iManStep++;
                  return false;

        case  25: if(!AT_MoveCyl(aiPSB_FlprOpCl , ccBwd))return false;
                  m_iManStep = 0 ;
                  return true ;


    }
}


bool CManualMan::CycleManTrmInsPs()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        DM.ARAY[riPSB] = TempArray1 ;
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


        case  10: AT_MoveCyl(aiPSB_PnchDnUp , ccBwd);
                  m_iManStep++;
                  return false;

        case  11: if(!AT_MoveCyl(aiPSB_PnchDnUp , ccBwd))return false;
                  MT_GoAbsMan(miPSB_XTrm , PM.GetValue(miPSB_XTrm , pvPSB_XTrmWait));
                  MT_GoAbsMan(miPSB_YTrm , PM.GetValue(miPSB_YTrm , pvPSB_YTrmWait));
                  MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns , pvPSB_XInsWait));
                  m_iManStep++;
                  return false;

        case  12: if(!MT_GoAbsMan(miPSB_XTrm , PM.GetValue(miPSB_XTrm , pvPSB_XTrmWait)))return false;
                  if(!MT_GoAbsMan(miPSB_YTrm , PM.GetValue(miPSB_YTrm , pvPSB_YTrmWait)))return false;
                  if(!MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns , pvPSB_XInsWait)))return false;

                  m_iManStep++;
                  return false;

        case  13: AT_MoveCyl(aiPSB_IndxUpDn , ccFwd);
                  m_iManStep++;
                  return false;

        case  14: if(!AT_MoveCyl(aiPSB_IndxUpDn  , ccFwd))return false;
                  MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns , pvPSB_XInsInsert));
                  m_iManStep++;
                  return false;

        case  15: if(!MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns , pvPSB_XInsInsert)))return false;
                  AT_MoveCyl(aiPSB_FlprOpCl , ccFwd);
                  m_iManStep++;
                  return false;

        case  16: if(!AT_MoveCyl(aiPSB_FlprOpCl , ccFwd))return false;
                  MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns , pvPSB_XInsWait));
                  MT_GoAbsMan(miPSB_TFlp , PM.GetValue(miPSB_TFlp , pvPSB_TFlpWork));
                  AT_MoveCyl(aiPSB_IndxUpDn , ccBwd);
                  m_iManStep++;
                  return false;

        case  17: if(MT_GoAbsMan(miPSB_TFlp , PM.GetValue(miPSB_TFlp , pvPSB_TFlpWork)))return false;
                  m_iManStep = 0;
                  return true;
    }
}
bool CManualMan::CycleManTrmNorPs()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        DM.ARAY[riPSB] = TempArray1 ;
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

        case  10: AT_MoveCyl(aiPSB_FlprOpCl , ccFwd);
                  AT_MoveCyl(aiPSB_PnchDnUp , ccBwd);
                  m_iManStep ++;
                  return false;

        case  11: if(!AT_MoveCyl(aiPSB_FlprOpCl , ccFwd))return false;
                  if(!AT_MoveCyl(aiPSB_PnchDnUp , ccBwd))return false;
                  MT_GoAbsMan(miPSB_XTrm , PM.GetValue(miPSB_XTrm , pvPSB_XTrmWait));
                  MT_GoAbsMan(miPSB_YTrm , PM.GetValue(miPSB_YTrm , pvPSB_YTrmWait));
                  m_iManStep ++;
                  return false;

        case  12: if(!MT_GoAbsMan(miPSB_XTrm , PM.GetValue(miPSB_XTrm , pvPSB_XTrmWait)))return false;
                  if(!MT_GoAbsMan(miPSB_YTrm , PM.GetValue(miPSB_YTrm , pvPSB_YTrmWait)))return false;
                  m_iManStep++;
                  return false;

        case  13:
                  MT_GoAbsMan(miPSB_TFlp , PM.GetValue(miPSB_TFlp , pvPSB_TFlpWait));
                  m_iManStep++;
                  return false;

        case  14: if(!MT_GoAbsMan(miPSB_TFlp , PM.GetValue(miPSB_TFlp , pvPSB_TFlpWait))) return false;
                  AT_MoveCyl(aiPSB_FlprOpCl , ccBwd);
                  m_iManStep++;
                  return false;

        case  15: if(!AT_MoveCyl(aiPSB_FlprOpCl , ccBwd))return false;
                  m_iManStep = 0;
                  return false;
    }
}

double CManualMan::GetMotrPosTable(EN_MOTR_ID _iMotr , int _iRow , int _iCol)
{
    int    r , c;
    double dStartYPos ;
    double dStartXPos ;
    double dYPos      ;
    double dXPos      ;

    r = _iRow ;//OM.DevOptn.iRowInspCnt ? _iRow/OM.DevOptn.iRowInspCnt * OM.DevOptn.iRowInspCnt : _iRow ;
    c = _iCol ;//OM.DevOptn.iColInspCnt ? _iCol/OM.DevOptn.iColInspCnt * OM.DevOptn.iColInspCnt : _iCol ;

    dStartYPos = PM.GetValue(_iMotr , pvPSB_YTrmWorkStart ) ;
    dStartXPos = PM.GetValue(_iMotr , pvPSB_XTrmWorkStart ) ;

    if(_iMotr != miPSB_YTrm && _iMotr != miPSB_XTrm) return MT_GetCmdPos(_iMotr) ;

    dXPos = dStartXPos + c * OM.DevInfo.dColPitch ;
    dYPos = dStartYPos + r * OM.DevInfo.dRowPitch ;

    if(OM.DevInfo.dColGrGap && OM.DevInfo.iColGrCnt) {
        dXPos = dXPos + (c / OM.DevInfo.iColGrCnt) * (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) ;
    }


    if(OM.DevInfo.dRowGrGap && OM.DevInfo.iRowGrCnt) {
        dYPos = dYPos + (r / OM.DevInfo.iRowGrCnt) * (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) ;
    }

    if     (_iMotr == miPSB_YTrm) return dYPos ;
    else if(_iMotr == miPSB_XTrm) return dXPos ;
}





