//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormDeviceSet.h"
#include "SMInterfaceUnit.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "DataMan.h"
#include "Loader.h"
#include "Head.h"
#include "PstBuff.h"
#include "UnLoader.h"
#include "Rail.h"
#include "Sequence.h"
#include "ManualMan.h"
#include "UserINI.h"
//#include "LanguageUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
__fastcall TFrmDeviceSet::TFrmDeviceSet(TComponent* Owner)
    : TForm(Owner)
{
    PM.SetWindow (pnLDR_YPos,miLDR_YCmp);
    PM.SetWindow (pnLDR_ZPos,miLDR_ZCmp);
    PM.SetWindow (pnSRT_TPos,miPSB_TSrt);
    PM.SetWindow (pnVS1_YPos,miWK1_YVsn);
    PM.SetWindow (pnVS1_ZPos,miWK1_ZVsn);
    PM.SetWindow (pnVS2_YPos,miWK2_YVsn);
    PM.SetWindow (pnVSN_XPos,miWRK_XVsn);
    PM.SetWindow (pnULD_YPos,miULD_YCmp);
    PM.SetWindow (pnULD_ZPos,miULD_ZCmp);
    PM.SetWindow (pnTRM_XPos,miPSB_XTrm);
    PM.SetWindow (pnTRM_YPos,miPSB_YTrm);
    PM.SetWindow (pnIDX_XPos,miPSB_XIns);

    pcDeviceSet -> ActivePageIndex = 0 ;

    //edVisnIdIndex -> NumberOnly = false ;

    btLDR_In   -> Tag = (int)yLDR_MgzInAC  ;
    btLDR_Out  -> Tag = (int)yLDR_MgzOutAC ;
    btPRB_Belt -> Tag = (int)yPRB_Belt     ;
    btWRK_Belt -> Tag = (int)yWRK_Belt     ;
    btULD_In   -> Tag = (int)yULD_MgzInAC  ;
    btULD_Out  -> Tag = (int)yULD_MgzOutAC ;


    btLDR_CmpUp     -> Tag = (int)aiLDR_Cmp    ;
    btLDR_CmpDn     -> Tag = (int)aiLDR_Cmp    ;

    btLDR_PusherBw  -> Tag = (int)aiLDR_Pusher ;
    btLDR_PusherFw  -> Tag = (int)aiLDR_Pusher ;

    btPRB_StopperUp -> Tag = (int)aiPRB_Stopper ;
    btPRB_StopperDn -> Tag = (int)aiPRB_Stopper ;

    btWK1_StopperUp -> Tag = (int)aiWK1_Stopper ;
    btWK1_StopperDn -> Tag = (int)aiWK1_Stopper ;

    btWK2_StopperUp -> Tag = (int)aiWK2_Stopper ; 
    btWK2_StopperDn -> Tag = (int)aiWK2_Stopper ; 

    btPSB_FingerUp  -> Tag = (int)aiPSB_Finger ;
    btPSB_FingerDn  -> Tag = (int)aiPSB_Finger ;

    btPSB_PusherBw  -> Tag = (int)aiPSB_PusherFB ;
    btPSB_PusherFw  -> Tag = (int)aiPSB_PusherFB ;

    btWK1_AlignFw   -> Tag = (int)aiWK1_Align ;
    btWK1_AlignBw   -> Tag = (int)aiWK1_Align ;

    btWK2_AlignFw   -> Tag = (int)aiWK2_Align ;
    btWK2_AlignBw   -> Tag = (int)aiWK2_Align ;

    btPSB_AlignFw   -> Tag = (int)aiPSB_Align ;
    btPSB_AlignBw   -> Tag = (int)aiPSB_Align ;

    btPSB_PusherUp  -> Tag = (int)aiPSB_PusherUD ;
    btPSB_PusherDn  -> Tag = (int)aiPSB_PusherUD ;

    btWK1_LiftUp    -> Tag = (int)aiWK1_Lift ;
    btWK1_LiftDn    -> Tag = (int)aiWK1_Lift ;

    btWK2_LiftUp    -> Tag = (int)aiWK2_Lift ;
    btWK2_LiftDn    -> Tag = (int)aiWK2_Lift ;

    btHED_UVLightBw -> Tag = (int)aiHED_UVLight ;
    btHED_UVLightFw -> Tag = (int)aiHED_UVLight ;

    btPSB_FlprCmpUp -> Tag = (int)aiPSB_FlprCmp ;
    btPSB_FlprCmpDn -> Tag = (int)aiPSB_FlprCmp ;

    btPSB_FlprBw    -> Tag = (int)aiPSB_Flpr ;
    btPSB_FlprFw    -> Tag = (int)aiPSB_Flpr ;

    btPSB_TrimmerUp -> Tag = (int)aiPSB_Trimmer ;
    btPSB_TrimmerDn -> Tag = (int)aiPSB_Trimmer ;

    btULD_CmpUp     -> Tag = (int)aiULD_Cmp ;
    btULD_CmpDn     -> Tag = (int)aiULD_Cmp ;

//Jog
    btLdr_ZUp       -> Tag = (int)miLDR_ZCmp ;
    btLdr_ZDn       -> Tag = (int)miLDR_ZCmp ;
    btLdr_YBw       -> Tag = (int)miLDR_YCmp ;
    btLdr_YFw       -> Tag = (int)miLDR_YCmp ;
    btIdx_XBw       -> Tag = (int)miPSB_XIns ;
    btIdx_XFw       -> Tag = (int)miPSB_XIns ;
    btVs1_YBw       -> Tag = (int)miWK1_YVsn ;
    btVs1_YFw       -> Tag = (int)miWK1_YVsn ;
    btVs1_ZBw       -> Tag = (int)miWK1_ZVsn ;
    btVs1_ZFw       -> Tag = (int)miWK1_ZVsn ;
    btVs2_YBw       -> Tag = (int)miWK2_YVsn ;
    btVs2_YFw       -> Tag = (int)miWK2_YVsn ;
    btVs_XBw        -> Tag = (int)miWRK_XVsn ;
    btVs_XFw        -> Tag = (int)miWRK_XVsn ;
    btTrm_XBw       -> Tag = (int)miPSB_XTrm ;
    btTrm_XFw       -> Tag = (int)miPSB_XTrm ;
    btTrm_YBw       -> Tag = (int)miPSB_YTrm ;
    btTrm_YFw       -> Tag = (int)miPSB_YTrm ;
    btSrt_Bw        -> Tag = (int)miPSB_TSrt ;
    btSrt_Fw        -> Tag = (int)miPSB_TSrt ;
    btUld_YBw       -> Tag = (int)miULD_YCmp ;
    btUld_YFw       -> Tag = (int)miULD_YCmp ;
    btUld_ZDn       -> Tag = (int)miULD_ZCmp ;
    btUld_ZUp       -> Tag = (int)miULD_ZCmp ;

    /*
    edSortItRslt1  -> UseApoint = false ; edSortItRslt1  -> UseComma = false ; edSortItRslt1  -> UseMinus = false ;
    edSortItRslt2  -> UseApoint = false ; edSortItRslt2  -> UseComma = false ; edSortItRslt2  -> UseMinus = false ;
    edSortItRslt3  -> UseApoint = false ; edSortItRslt3  -> UseComma = false ; edSortItRslt3  -> UseMinus = false ;
    edSortItRslt4  -> UseApoint = false ; edSortItRslt4  -> UseComma = false ; edSortItRslt4  -> UseMinus = false ;
    edSortItRslt5  -> UseApoint = false ; edSortItRslt5  -> UseComma = false ; edSortItRslt5  -> UseMinus = false ;
    edSortItRslt6  -> UseApoint = false ; edSortItRslt6  -> UseComma = false ; edSortItRslt6  -> UseMinus = false ;
    edSortItRslt7  -> UseApoint = false ; edSortItRslt7  -> UseComma = false ; edSortItRslt7  -> UseMinus = false ;
    edSortItRslt8  -> UseApoint = false ; edSortItRslt8  -> UseComma = false ; edSortItRslt8  -> UseMinus = false ;
    edSortItRslt9  -> UseApoint = false ; edSortItRslt9  -> UseComma = false ; edSortItRslt9  -> UseMinus = false ;
    edSortItRslt10 -> UseApoint = false ; edSortItRslt10 -> UseComma = false ; edSortItRslt10 -> UseMinus = false ;
    edSortItRslt11 -> UseApoint = false ; edSortItRslt11 -> UseComma = false ; edSortItRslt11 -> UseMinus = false ;
    edSortItRslt12 -> UseApoint = false ; edSortItRslt12 -> UseComma = false ; edSortItRslt12 -> UseMinus = false ;
    edSortItRslt13 -> UseApoint = false ; edSortItRslt13 -> UseComma = false ; edSortItRslt13 -> UseMinus = false ;
    edSortItRslt14 -> UseApoint = false ; edSortItRslt14 -> UseComma = false ; edSortItRslt14 -> UseMinus = false ;
    edSortItFail   -> UseApoint = false ; edSortItFail   -> UseComma = false ; edSortItFail   -> UseMinus = false ;
    edLotMgzCnt    -> UseApoint = false ; edLotMgzCnt    -> UseComma = false ; edLotMgzCnt    -> UseMinus = false ;

    edLevelRslt1   -> UseApoint = false ; edLevelRslt1   -> UseComma = false ; edLevelRslt1   -> UseMinus = false ;
    edLevelRslt2   -> UseApoint = false ; edLevelRslt2   -> UseComma = false ; edLevelRslt2   -> UseMinus = false ;
    edLevelRslt3   -> UseApoint = false ; edLevelRslt3   -> UseComma = false ; edLevelRslt3   -> UseMinus = false ;
    edLevelRslt4   -> UseApoint = false ; edLevelRslt4   -> UseComma = false ; edLevelRslt4   -> UseMinus = false ;
    edLevelRslt5   -> UseApoint = false ; edLevelRslt5   -> UseComma = false ; edLevelRslt5   -> UseMinus = false ;
    edLevelRslt6   -> UseApoint = false ; edLevelRslt6   -> UseComma = false ; edLevelRslt6   -> UseMinus = false ;
    edLevelRslt7   -> UseApoint = false ; edLevelRslt7   -> UseComma = false ; edLevelRslt7   -> UseMinus = false ;
    edLevelRslt8   -> UseApoint = false ; edLevelRslt8   -> UseComma = false ; edLevelRslt8   -> UseMinus = false ;
    edLevelRslt9   -> UseApoint = false ; edLevelRslt9   -> UseComma = false ; edLevelRslt9   -> UseMinus = false ;
    edLevelRslt10  -> UseApoint = false ; edLevelRslt10  -> UseComma = false ; edLevelRslt10  -> UseMinus = false ;
    edLevelRslt11  -> UseApoint = false ; edLevelRslt11  -> UseComma = false ; edLevelRslt11  -> UseMinus = false ;
    edLevelRslt12  -> UseApoint = false ; edLevelRslt12  -> UseComma = false ; edLevelRslt12  -> UseMinus = false ;
    edLevelRslt13  -> UseApoint = false ; edLevelRslt13  -> UseComma = false ; edLevelRslt13  -> UseMinus = false ;
    edLevelRslt14  -> UseApoint = false ; edLevelRslt14  -> UseComma = false ; edLevelRslt14  -> UseMinus = false ;
    edLevelFail    -> UseApoint = false ; edLevelFail    -> UseComma = false ; edLevelFail    -> UseMinus = false ;
    */

    //Load Language
//    LAG.LoadFrmDeviceSet();
//    LanguageSel(FM_GetLan());
}

//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormShow(TObject *Sender)
{
    UpdateDevInfo (toTabl);
    UpdateDevOptn (toTabl);
    PM.UpdatePstn (toTabl);
    tmUpdate->Enabled = true;

    tsLoader -> TabVisible = OM.EqpOptn.bExistLoader ;
}

//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btSavePosClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    UpdateDevInfo(toBuff);
    OM.SaveDevInfo(OM.m_sCrntDev);
    UpdateDevOptn(toBuff);
    OM.SaveDevOptn(OM.m_sCrntDev);

    PM.UpdatePstn(toBuff);

    PM.Save(OM.m_sCrntDev);

    DM.ARAY[riLDR].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);
    DM.ARAY[riPRB].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riWK1].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riVS1].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riWK2].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riVS2].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riPSB].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riULD].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::tmUpdateTimer(TObject *Sender)
{
    tmUpdate->Enabled = false;

    btSaveDevice -> Enabled = !SEQ._bRun ;

    bool bFoward;
    switch ( pcDeviceSet -> ActivePageIndex ) {
        //Dev Info
        case 0 : break;

        //Loader
        case 1 : lbLDR_ZCmdPos    -> Caption = MT_GetCmdPos   (miLDR_ZCmp) ;
                 lbLDR_ZEncPos    -> Caption = MT_GetEncPos   (miLDR_ZCmp) ;
                 lbLDR_ZNLim      -> Color   = MT_GetNLimSnsr (miLDR_ZCmp) ? clLime : clSilver ;
                 lbLDR_ZHome      -> Color   = MT_GetHomeSnsr (miLDR_ZCmp) ? clLime : clSilver ;
                 lbLDR_ZPLim      -> Color   = MT_GetPLimSnsr (miLDR_ZCmp) ? clLime : clSilver ;
                 lbLDR_ZAlarm     -> Color   = MT_GetAlarm    (miLDR_ZCmp) ? clLime : clSilver ;
                 lbLDR_ZServo     -> Color   = MT_GetServo    (miLDR_ZCmp) ? clLime : clSilver ;
                 lbLDR_ZStop      -> Color   = MT_GetStopInpos(miLDR_ZCmp) ? clLime : clSilver ;
                 lbLDR_ZHomeDone  -> Color   = MT_GetHomeEnd  (miLDR_ZCmp) ? clLime : clSilver ;

                 lbLDR_YCmdPos    -> Caption = MT_GetCmdPos   (miLDR_YCmp) ;
                 lbLDR_YEncPos    -> Caption = MT_GetEncPos   (miLDR_YCmp) ;
                 lbLDR_YNLim      -> Color   = MT_GetNLimSnsr (miLDR_YCmp) ? clLime : clSilver ;
                 lbLDR_YHome      -> Color   = MT_GetHomeSnsr (miLDR_YCmp) ? clLime : clSilver ;
                 lbLDR_YPLim      -> Color   = MT_GetPLimSnsr (miLDR_YCmp) ? clLime : clSilver ;
                 lbLDR_YAlarm     -> Color   = MT_GetAlarm    (miLDR_YCmp) ? clLime : clSilver ;
                 lbLDR_YServo     -> Color   = MT_GetServo    (miLDR_YCmp) ? clLime : clSilver ;
                 lbLDR_YStop      -> Color   = MT_GetStopInpos(miLDR_YCmp) ? clLime : clSilver ;
                 lbLDR_YHomeDone  -> Color   = MT_GetHomeEnd  (miLDR_YCmp) ? clLime : clSilver ;

                 btLDR_In         ->Font->Color = IO_GetY(yLDR_MgzInAC ) ? clLime : clBlack ;
                 btLDR_In         ->Caption     = IO_GetY(yLDR_MgzInAC ) ? "ON"   : "OFF"   ;
                 btLDR_Out        ->Font->Color = IO_GetY(yLDR_MgzOutAC) ? clLime : clBlack ;
                 btLDR_Out        ->Caption     = IO_GetY(yLDR_MgzOutAC) ? "ON"   : "OFF"   ;

                 btLDR_CmpUp      ->Font->Color = AT_Complete(aiLDR_Cmp      ,ccFwd) ? clLime : clBlack ;
                 btLDR_CmpDn      ->Font->Color = AT_Complete(aiLDR_Cmp      ,ccBwd) ? clLime : clBlack ;
                 btLDR_PusherFw   ->Font->Color = AT_Complete(aiLDR_Pusher   ,ccFwd) ? clLime : clBlack ;
                 btLDR_PusherBw   ->Font->Color = AT_Complete(aiLDR_Pusher   ,ccBwd) ? clLime : clBlack ;

        //Rail
        case 2 : btPRB_Belt       ->Font->Color = IO_GetY(yPRB_Belt    ) ? clLime : clBlack ;
                 btPRB_Belt       ->Caption     = IO_GetY(yPRB_Belt    ) ? "ON"   : "OFF"   ;
                 btWRK_Belt       ->Font->Color = IO_GetY(yWRK_Belt    ) ? clLime : clBlack ;
                 btWRK_Belt       ->Caption     = IO_GetY(yWRK_Belt    ) ? "ON"   : "OFF"   ;

                 btPRB_StopperUp  ->Font->Color = AT_Complete(aiPRB_Stopper  ,ccFwd) ? clLime : clBlack ; btPRB_StopperDn  ->Font->Color = AT_Complete(aiPRB_Stopper  ,ccBwd) ? clLime : clBlack ;
                 btWK1_AlignFw    ->Font->Color = AT_Complete(aiWK1_Align    ,ccFwd) ? clLime : clBlack ; btWK1_AlignBw    ->Font->Color = AT_Complete(aiWK1_Align    ,ccBwd) ? clLime : clBlack ;
                 btWK1_StopperUp  ->Font->Color = AT_Complete(aiWK1_Stopper  ,ccFwd) ? clLime : clBlack ; btWK1_StopperDn  ->Font->Color = AT_Complete(aiWK1_Stopper  ,ccBwd) ? clLime : clBlack ;
                 btWK1_LiftUp     ->Font->Color = AT_Complete(aiWK1_Lift     ,ccFwd) ? clLime : clBlack ; btWK1_LiftDn     ->Font->Color = AT_Complete(aiWK1_Lift     ,ccBwd) ? clLime : clBlack ;
                 btWK2_AlignFw    ->Font->Color = AT_Complete(aiWK2_Align    ,ccFwd) ? clLime : clBlack ; btWK2_AlignBw    ->Font->Color = AT_Complete(aiWK2_Align    ,ccBwd) ? clLime : clBlack ;
                 btWK2_StopperUp  ->Font->Color = AT_Complete(aiWK2_Stopper  ,ccFwd) ? clLime : clBlack ; btWK2_StopperDn  ->Font->Color = AT_Complete(aiWK2_Stopper  ,ccBwd) ? clLime : clBlack ;
                 btWK2_LiftUp     ->Font->Color = AT_Complete(aiWK2_Lift     ,ccFwd) ? clLime : clBlack ; btWK2_LiftDn     ->Font->Color = AT_Complete(aiWK2_Lift     ,ccBwd) ? clLime : clBlack ;


                 break;

        //Head
        case 3 : lbVSN_XCmdPos    -> Caption = MT_GetCmdPos   (miWRK_XVsn) ;
                 lbVSN_XEncPos    -> Caption = MT_GetEncPos   (miWRK_XVsn) ;
                 lbVSN_XNLim      -> Color   = MT_GetNLimSnsr (miWRK_XVsn) ? clLime : clSilver ;
                 lbVSN_XHome      -> Color   = MT_GetHomeSnsr (miWRK_XVsn) ? clLime : clSilver ;
                 lbVSN_XPLim      -> Color   = MT_GetPLimSnsr (miWRK_XVsn) ? clLime : clSilver ;
                 lbVSN_XAlarm     -> Color   = MT_GetAlarm    (miWRK_XVsn) ? clLime : clSilver ;
                 lbVSN_XServo     -> Color   = MT_GetServo    (miWRK_XVsn) ? clLime : clSilver ;
                 lbVSN_XStop      -> Color   = MT_GetStopInpos(miWRK_XVsn) ? clLime : clSilver ;
                 lbVSN_XHomeDone  -> Color   = MT_GetHomeEnd  (miWRK_XVsn) ? clLime : clSilver ;

                 lbVS1_YCmdPos    -> Caption = MT_GetCmdPos   (miWK1_YVsn) ;
                 lbVS1_YEncPos    -> Caption = MT_GetEncPos   (miWK1_YVsn) ;
                 lbVS1_YNLim      -> Color   = MT_GetNLimSnsr (miWK1_YVsn) ? clLime : clSilver ;
                 lbVS1_YHome      -> Color   = MT_GetHomeSnsr (miWK1_YVsn) ? clLime : clSilver ;
                 lbVS1_YPLim      -> Color   = MT_GetPLimSnsr (miWK1_YVsn) ? clLime : clSilver ;
                 lbVS1_YAlarm     -> Color   = MT_GetAlarm    (miWK1_YVsn) ? clLime : clSilver ;
                 lbVS1_YServo     -> Color   = MT_GetServo    (miWK1_YVsn) ? clLime : clSilver ;
                 lbVS1_YStop      -> Color   = MT_GetStopInpos(miWK1_YVsn) ? clLime : clSilver ;
                 lbVS1_YHomeDone  -> Color   = MT_GetHomeEnd  (miWK1_YVsn) ? clLime : clSilver ;

                 lbVS2_YCmdPos    -> Caption = MT_GetCmdPos   (miWK2_YVsn) ;
                 lbVS2_YEncPos    -> Caption = MT_GetEncPos   (miWK2_YVsn) ;
                 lbVS2_YNLim      -> Color   = MT_GetNLimSnsr (miWK2_YVsn) ? clLime : clSilver ;
                 lbVS2_YHome      -> Color   = MT_GetHomeSnsr (miWK2_YVsn) ? clLime : clSilver ;
                 lbVS2_YPLim      -> Color   = MT_GetPLimSnsr (miWK2_YVsn) ? clLime : clSilver ;
                 lbVS2_YAlarm     -> Color   = MT_GetAlarm    (miWK2_YVsn) ? clLime : clSilver ;
                 lbVS2_YServo     -> Color   = MT_GetServo    (miWK2_YVsn) ? clLime : clSilver ;
                 lbVS2_YStop      -> Color   = MT_GetStopInpos(miWK2_YVsn) ? clLime : clSilver ;
                 lbVS2_YHomeDone  -> Color   = MT_GetHomeEnd  (miWK2_YVsn) ? clLime : clSilver ;

                 lbVS1_ZCmdPos    -> Caption = MT_GetCmdPos   (miWK1_ZVsn) ;
                 lbVS1_ZEncPos    -> Caption = MT_GetEncPos   (miWK1_ZVsn) ;
                 lbVS1_ZNLim      -> Color   = MT_GetNLimSnsr (miWK1_ZVsn) ? clLime : clSilver ;
                 lbVS1_ZHome      -> Color   = MT_GetHomeSnsr (miWK1_ZVsn) ? clLime : clSilver ;
                 lbVS1_ZPLim      -> Color   = MT_GetPLimSnsr (miWK1_ZVsn) ? clLime : clSilver ;
                 lbVS1_ZAlarm     -> Color   = MT_GetAlarm    (miWK1_ZVsn) ? clLime : clSilver ;
                 lbVS1_ZServo     -> Color   = MT_GetServo    (miWK1_ZVsn) ? clLime : clSilver ;
                 lbVS1_ZStop      -> Color   = MT_GetStopInpos(miWK1_ZVsn) ? clLime : clSilver ;
                 lbVS1_ZHomeDone  -> Color   = MT_GetHomeEnd  (miWK1_ZVsn) ? clLime : clSilver ;

                 btHED_UVLightFw  ->Font->Color = AT_Complete(aiHED_UVLight  ,ccFwd) ? clLime : clBlack ; btHED_UVLightBw  ->Font->Color = AT_Complete(aiHED_UVLight  ,ccBwd) ? clLime : clBlack ;

                 break;

        //Trim
        case 4 : lbTRM_XCmdPos    -> Caption = MT_GetCmdPos   (miPSB_XTrm) ;
                 lbTRM_XEncPos    -> Caption = MT_GetEncPos   (miPSB_XTrm) ;
                 lbTRM_XNLim      -> Color   = MT_GetNLimSnsr (miPSB_XTrm) ? clLime : clSilver ;
                 lbTRM_XHome      -> Color   = MT_GetHomeSnsr (miPSB_XTrm) ? clLime : clSilver ;
                 lbTRM_XPLim      -> Color   = MT_GetPLimSnsr (miPSB_XTrm) ? clLime : clSilver ;
                 lbTRM_XAlarm     -> Color   = MT_GetAlarm    (miPSB_XTrm) ? clLime : clSilver ;
                 lbTRM_XServo     -> Color   = MT_GetServo    (miPSB_XTrm) ? clLime : clSilver ;
                 lbTRM_XStop      -> Color   = MT_GetStopInpos(miPSB_XTrm) ? clLime : clSilver ;
                 lbTRM_XHomeDone  -> Color   = MT_GetHomeEnd  (miPSB_XTrm) ? clLime : clSilver ;

                 lbTRM_YCmdPos    -> Caption = MT_GetCmdPos   (miPSB_YTrm) ;
                 lbTRM_YEncPos    -> Caption = MT_GetEncPos   (miPSB_YTrm) ;
                 lbTRM_YNLim      -> Color   = MT_GetNLimSnsr (miPSB_YTrm) ? clLime : clSilver ;
                 lbTRM_YHome      -> Color   = MT_GetHomeSnsr (miPSB_YTrm) ? clLime : clSilver ;
                 lbTRM_YPLim      -> Color   = MT_GetPLimSnsr (miPSB_YTrm) ? clLime : clSilver ;
                 lbTRM_YAlarm     -> Color   = MT_GetAlarm    (miPSB_YTrm) ? clLime : clSilver ;
                 lbTRM_YServo     -> Color   = MT_GetServo    (miPSB_YTrm) ? clLime : clSilver ;
                 lbTRM_YStop      -> Color   = MT_GetStopInpos(miPSB_YTrm) ? clLime : clSilver ;
                 lbTRM_YHomeDone  -> Color   = MT_GetHomeEnd  (miPSB_YTrm) ? clLime : clSilver ;

                 lbSRT_TCmdPos    -> Caption = MT_GetCmdPos   (miPSB_TSrt) ;
                 lbSRT_TEncPos    -> Caption = MT_GetEncPos   (miPSB_TSrt) ;
                 lbSRT_TNLim      -> Color   = MT_GetNLimSnsr (miPSB_TSrt) ? clLime : clSilver ;
                 lbSRT_THome      -> Color   = MT_GetHomeSnsr (miPSB_TSrt) ? clLime : clSilver ;
                 lbSRT_TPLim      -> Color   = MT_GetPLimSnsr (miPSB_TSrt) ? clLime : clSilver ;
                 lbSRT_TAlarm     -> Color   = MT_GetAlarm    (miPSB_TSrt) ? clLime : clSilver ;
                 lbSRT_TServo     -> Color   = MT_GetServo    (miPSB_TSrt) ? clLime : clSilver ;
                 lbSRT_TStop      -> Color   = MT_GetStopInpos(miPSB_TSrt) ? clLime : clSilver ;
                 lbSRT_THomeDone  -> Color   = MT_GetHomeEnd  (miPSB_TSrt) ? clLime : clSilver ;

                 lbIDX_XCmdPos    -> Caption = MT_GetCmdPos   (miPSB_XIns) ;
                 lbIDX_XEncPos    -> Caption = MT_GetEncPos   (miPSB_XIns) ;
                 lbIDX_XNLim      -> Color   = MT_GetNLimSnsr (miPSB_XIns) ? clLime : clSilver ;
                 lbIDX_XHome      -> Color   = MT_GetHomeSnsr (miPSB_XIns) ? clLime : clSilver ;
                 lbIDX_XPLim      -> Color   = MT_GetPLimSnsr (miPSB_XIns) ? clLime : clSilver ;
                 lbIDX_XAlarm     -> Color   = MT_GetAlarm    (miPSB_XIns) ? clLime : clSilver ;
                 lbIDX_XServo     -> Color   = MT_GetServo    (miPSB_XIns) ? clLime : clSilver ;
                 lbIDX_XStop      -> Color   = MT_GetStopInpos(miPSB_XIns) ? clLime : clSilver ;
                 lbIDX_XHomeDone  -> Color   = MT_GetHomeEnd  (miPSB_XIns) ? clLime : clSilver ;

                 btULD_In         ->Font->Color = IO_GetY(yULD_MgzInAC ) ? clLime : clBlack ;
                 btULD_In         ->Caption     = IO_GetY(yULD_MgzInAC ) ? "ON"   : "OFF"   ;
                 btULD_Out        ->Font->Color = IO_GetY(yULD_MgzOutAC) ? clLime : clBlack ;
                 btULD_Out        ->Caption     = IO_GetY(yULD_MgzOutAC) ? "ON"   : "OFF"   ;

                 btPSB_FlprCmpUp  ->Font->Color = AT_Complete(aiPSB_FlprCmp  ,ccFwd) ? clLime : clBlack ; btPSB_FlprCmpDn  ->Font->Color = AT_Complete(aiPSB_FlprCmp  ,ccBwd) ? clLime : clBlack ;
                 btPSB_FlprFw     ->Font->Color = AT_Complete(aiPSB_Flpr     ,ccFwd) ? clLime : clBlack ; btPSB_FlprBw     ->Font->Color = AT_Complete(aiPSB_Flpr     ,ccBwd) ? clLime : clBlack ;
                 btPSB_TrimmerDn  ->Font->Color = AT_Complete(aiPSB_Trimmer  ,ccFwd) ? clLime : clBlack ; btPSB_TrimmerUp  ->Font->Color = AT_Complete(aiPSB_Trimmer  ,ccBwd) ? clLime : clBlack ;

                 btPSB_AlignFw    ->Font->Color = AT_Complete(aiPSB_Align    ,ccFwd) ? clLime : clBlack ; btPSB_AlignBw    ->Font->Color = AT_Complete(aiPSB_Align    ,ccBwd) ? clLime : clBlack ;
                 btPSB_FingerDn   ->Font->Color = AT_Complete(aiPSB_Finger   ,ccFwd) ? clLime : clBlack ; btPSB_FingerUp   ->Font->Color = AT_Complete(aiPSB_Finger   ,ccBwd) ? clLime : clBlack ;
                 btPSB_PusherUp   ->Font->Color = AT_Complete(aiPSB_PusherUD ,ccFwd) ? clLime : clBlack ; btPSB_PusherDn   ->Font->Color = AT_Complete(aiPSB_PusherUD ,ccBwd) ? clLime : clBlack ;
                 btPSB_PusherFw   ->Font->Color = AT_Complete(aiPSB_PusherFB ,ccFwd) ? clLime : clBlack ; btPSB_PusherBw   ->Font->Color = AT_Complete(aiPSB_PusherFB ,ccBwd) ? clLime : clBlack ;
                 break;

        //Un Loader
        case 5 : lbULD_ZCmdPos    -> Caption = MT_GetCmdPos   (miULD_ZCmp) ;
                 lbULD_ZEncPos    -> Caption = MT_GetEncPos   (miULD_ZCmp) ;
                 lbULD_ZNLim      -> Color   = MT_GetNLimSnsr (miULD_ZCmp) ? clLime : clSilver ;
                 lbULD_ZHome      -> Color   = MT_GetHomeSnsr (miULD_ZCmp) ? clLime : clSilver ;
                 lbULD_ZPLim      -> Color   = MT_GetPLimSnsr (miULD_ZCmp) ? clLime : clSilver ;
                 lbULD_ZAlarm     -> Color   = MT_GetAlarm    (miULD_ZCmp) ? clLime : clSilver ;
                 lbULD_ZServo     -> Color   = MT_GetServo    (miULD_ZCmp) ? clLime : clSilver ;
                 lbULD_ZStop      -> Color   = MT_GetStopInpos(miULD_ZCmp) ? clLime : clSilver ;
                 lbULD_ZHomeDone  -> Color   = MT_GetHomeEnd  (miULD_ZCmp) ? clLime : clSilver ;

                 lbULD_YCmdPos    -> Caption = MT_GetCmdPos   (miULD_YCmp) ;
                 lbULD_YEncPos    -> Caption = MT_GetEncPos   (miULD_YCmp) ;
                 lbULD_YNLim      -> Color   = MT_GetNLimSnsr (miULD_YCmp) ? clLime : clSilver ;
                 lbULD_YHome      -> Color   = MT_GetHomeSnsr (miULD_YCmp) ? clLime : clSilver ;
                 lbULD_YPLim      -> Color   = MT_GetPLimSnsr (miULD_YCmp) ? clLime : clSilver ;
                 lbULD_YAlarm     -> Color   = MT_GetAlarm    (miULD_YCmp) ? clLime : clSilver ;
                 lbULD_YServo     -> Color   = MT_GetServo    (miULD_YCmp) ? clLime : clSilver ;
                 lbULD_YStop      -> Color   = MT_GetStopInpos(miULD_YCmp) ? clLime : clSilver ;
                 lbULD_YHomeDone  -> Color   = MT_GetHomeEnd  (miULD_YCmp) ? clLime : clSilver ;

                 btULD_CmpUp      ->Font->Color = AT_Complete(aiULD_Cmp      ,ccFwd) ? clLime : clBlack ; btULD_CmpDn      ->Font->Color = AT_Complete(aiULD_Cmp      ,ccBwd) ? clLime : clBlack ;
                 break;
    }
    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormDestroy(TObject *Sender)
{
    tmUpdate->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevInfo(bool bTable)
{
    if(bTable) {
        edColPitch       -> Text = OM.DevInfo.dColPitch   ;
        edRowPitch       -> Text = OM.DevInfo.dRowPitch   ;

        edColGroupCount  -> Text = OM.DevInfo.iColGrCnt   ;
        edRowGroupCount  -> Text = OM.DevInfo.iRowGrCnt   ;

        edColGroupGap    -> Text = OM.DevInfo.dColGrGap   ;
        edRowGroupGap    -> Text = OM.DevInfo.dRowGrGap   ;

        edColCount       -> Text = OM.DevInfo.iColCnt     ;
        edRowCount       -> Text = OM.DevInfo.iRowCnt     ;

        edCsSlotCnt      -> Text = OM.DevInfo.iCsSlCnt    ;
        edCsSlotPitch    -> Text = OM.DevInfo.dCsSlPitch  ;
    }
    else {
        OM.DevInfo.dColPitch  = edColPitch       -> Text.ToDouble() ;
        OM.DevInfo.dRowPitch  = edRowPitch       -> Text.ToDouble() ;

        OM.DevInfo.iColGrCnt  = edColGroupCount  -> Text.ToInt()    ;
        OM.DevInfo.iRowGrCnt  = edRowGroupCount  -> Text.ToInt()    ;

        OM.DevInfo.dColGrGap  = edColGroupGap    -> Text.ToDouble() ;
        OM.DevInfo.dRowGrGap  = edRowGroupGap    -> Text.ToDouble() ;

        OM.DevInfo.iColCnt    = edColCount       -> Text.ToInt()    ;
        OM.DevInfo.iRowCnt    = edRowCount       -> Text.ToInt()    ;

        OM.DevInfo.iCsSlCnt   = edCsSlotCnt      -> Text.ToInt()    ;
        OM.DevInfo.dCsSlPitch = edCsSlotPitch    -> Text.ToDouble() ;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevOptn(bool bTable)
{
    if(bTable) {
        edVisnIndexId    -> Text    = OM.DevOptn.sVisnIndexId    ;
        edInsXVel        -> Text    = OM.DevOptn.dPSB_XInsVel    ;
        edInsRowCnt      -> Text    = OM.DevOptn.iRowInspCnt     ;
        edInsColCnt      -> Text    = OM.DevOptn.iColInspCnt     ;
        cbUvLight        -> Checked = OM.DevOptn.bUseUv          ;
        edLotEndMgzCnt   -> Text    = OM.DevOptn.iLotEndMgzCnt   ;

        /*
        edSortItRslt1    -> Text    = OM.DevOptn.iSortItRslt1    ;
        edSortItRslt2    -> Text    = OM.DevOptn.iSortItRslt2    ;
        edSortItRslt3    -> Text    = OM.DevOptn.iSortItRslt3    ;
        edSortItRslt4    -> Text    = OM.DevOptn.iSortItRslt4    ;
        edSortItRslt5    -> Text    = OM.DevOptn.iSortItRslt5    ;
        edSortItRslt6    -> Text    = OM.DevOptn.iSortItRslt6    ;
        edSortItRslt7    -> Text    = OM.DevOptn.iSortItRslt7    ;
        edSortItRslt8    -> Text    = OM.DevOptn.iSortItRslt8    ;
        edSortItRslt9    -> Text    = OM.DevOptn.iSortItRslt9    ;
        edSortItRslt10   -> Text    = OM.DevOptn.iSortItRslt10   ;
        edSortItRslt11   -> Text    = OM.DevOptn.iSortItRslt11   ;
        edSortItRslt12   -> Text    = OM.DevOptn.iSortItRslt12   ;
        edSortItRslt13   -> Text    = OM.DevOptn.iSortItRslt13   ;
        edSortItRslt14   -> Text    = OM.DevOptn.iSortItRslt14   ;
        edSortItFail     -> Text    = OM.DevOptn.iSortItFail     ;

        edLevelRslt1     -> Text    = OM.DevOptn.iLevelRslt1     ;
        edLevelRslt2     -> Text    = OM.DevOptn.iLevelRslt2     ;
        edLevelRslt3     -> Text    = OM.DevOptn.iLevelRslt3     ;
        edLevelRslt4     -> Text    = OM.DevOptn.iLevelRslt4     ;
        edLevelRslt5     -> Text    = OM.DevOptn.iLevelRslt5     ;
        edLevelRslt6     -> Text    = OM.DevOptn.iLevelRslt6     ;
        edLevelRslt7     -> Text    = OM.DevOptn.iLevelRslt7     ;
        edLevelRslt8     -> Text    = OM.DevOptn.iLevelRslt8     ;
        edLevelRslt9     -> Text    = OM.DevOptn.iLevelRslt9     ;
        edLevelRslt10    -> Text    = OM.DevOptn.iLevelRslt10    ;
        edLevelRslt11    -> Text    = OM.DevOptn.iLevelRslt11    ;
        edLevelRslt12    -> Text    = OM.DevOptn.iLevelRslt12    ;
        edLevelRslt13    -> Text    = OM.DevOptn.iLevelRslt13    ;
        edLevelRslt14    -> Text    = OM.DevOptn.iLevelRslt14    ;
        edLevelFail      -> Text    = OM.DevOptn.iLevelFail      ;
        */

        edWK1StprDnDelay -> Text    = OM.DevOptn.iWK1StprDnDelay ;
        edPRBStprDnDelay -> Text    = OM.DevOptn.iPRBStprDnDelay ;


    }
    else {
        OM.DevOptn.sVisnIndexId    = edVisnIndexId    -> Text            ;
        OM.DevOptn.dPSB_XInsVel    = edInsXVel        -> Text.ToDouble() ;
        OM.DevOptn.iRowInspCnt     = edInsRowCnt      -> Text.ToInt()    ;
        OM.DevOptn.iColInspCnt     = edInsColCnt      -> Text.ToInt()    ;
        OM.DevOptn.bUseUv          = cbUvLight        -> Checked         ;
        OM.DevOptn.iLotEndMgzCnt   = edLotEndMgzCnt   -> Text.ToInt()    ;
/*
        OM.DevOptn.iSortItRslt1    = edSortItRslt1    -> Text.ToInt()    ;
        OM.DevOptn.iSortItRslt2    = edSortItRslt2    -> Text.ToInt()    ;
        OM.DevOptn.iSortItRslt3    = edSortItRslt3    -> Text.ToInt()    ;
        OM.DevOptn.iSortItRslt4    = edSortItRslt4    -> Text.ToInt()    ;
        OM.DevOptn.iSortItRslt5    = edSortItRslt5    -> Text.ToInt()    ;
        OM.DevOptn.iSortItRslt6    = edSortItRslt6    -> Text.ToInt()    ;
        OM.DevOptn.iSortItRslt7    = edSortItRslt7    -> Text.ToInt()    ;
        OM.DevOptn.iSortItRslt8    = edSortItRslt8    -> Text.ToInt()    ;
        OM.DevOptn.iSortItRslt9    = edSortItRslt9    -> Text.ToInt()    ;
        OM.DevOptn.iSortItRslt10   = edSortItRslt10   -> Text.ToInt()    ;
        OM.DevOptn.iSortItRslt11   = edSortItRslt11   -> Text.ToInt()    ;
        OM.DevOptn.iSortItRslt12   = edSortItRslt12   -> Text.ToInt()    ;
        OM.DevOptn.iSortItRslt13   = edSortItRslt13   -> Text.ToInt()    ;
        OM.DevOptn.iSortItRslt14   = edSortItRslt14   -> Text.ToInt()    ;
        OM.DevOptn.iSortItFail     = edSortItFail     -> Text.ToInt()    ;

        OM.DevOptn.iLevelRslt1     = edLevelRslt1     -> Text.ToInt()    ;
        OM.DevOptn.iLevelRslt2     = edLevelRslt2     -> Text.ToInt()    ;
        OM.DevOptn.iLevelRslt3     = edLevelRslt3     -> Text.ToInt()    ;
        OM.DevOptn.iLevelRslt4     = edLevelRslt4     -> Text.ToInt()    ;
        OM.DevOptn.iLevelRslt5     = edLevelRslt5     -> Text.ToInt()    ;
        OM.DevOptn.iLevelRslt6     = edLevelRslt6     -> Text.ToInt()    ;
        OM.DevOptn.iLevelRslt7     = edLevelRslt7     -> Text.ToInt()    ;
        OM.DevOptn.iLevelRslt8     = edLevelRslt8     -> Text.ToInt()    ;
        OM.DevOptn.iLevelRslt9     = edLevelRslt9     -> Text.ToInt()    ;
        OM.DevOptn.iLevelRslt10    = edLevelRslt10    -> Text.ToInt()    ;
        OM.DevOptn.iLevelRslt11    = edLevelRslt11    -> Text.ToInt()    ;
        OM.DevOptn.iLevelRslt12    = edLevelRslt12    -> Text.ToInt()    ;
        OM.DevOptn.iLevelRslt13    = edLevelRslt13    -> Text.ToInt()    ;
        OM.DevOptn.iLevelRslt14    = edLevelRslt14    -> Text.ToInt()    ;
        OM.DevOptn.iLevelFail      = edLevelFail      -> Text.ToInt()    ;
*/

        OM.DevOptn.iWK1StprDnDelay = edWK1StprDnDelay -> Text.ToIntDef(0) ;
        OM.DevOptn.iPRBStprDnDelay = edPRBStprDnDelay -> Text.ToIntDef(0) ;



    }
}

//---------------------------------------------------------------------------
/*
     Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");

     int        iSel  = ((TBitBtn *)Sender) -> Tag ;
     bool       bFwd  = iSel / 10 ;
     int        iMotr = iSel % 10 ;
     bool       bJog  = false ; //= rgJogUnit -> ItemIndex == 0 ;
     double     dUnit ;

     switch ( pcDeviceSet -> ActivePageIndex ) {
        case 0 :                                                            break ;

        case 1 :    switch(rgJogUnit_Rail -> ItemIndex) {
                        case 0 : dUnit = 0.0                              ;
                                 bJog  = true                             ; break ;
                        case 1 : dUnit = 1                                ; break ;
                        case 2 : dUnit = 0.5                              ; break ;
                        case 3 : dUnit = 0.1                              ; break ;
                        case 4 : dUnit = 0.05                             ; break ;
                        case 5 : dUnit = edJogUsr_Rail -> Text.ToDouble() ; break ;
                    }

        case 2 :    switch(rgJogUnit_Rail -> ItemIndex) {
                        case 0 : dUnit = 0.0                              ;
                                 bJog  = true                             ; break ;
                        case 1 : dUnit = 1                                ; break ;
                        case 2 : dUnit = 0.5                              ; break ;
                        case 3 : dUnit = 0.1                              ; break ;
                        case 4 : dUnit = 0.05                             ; break ;
                        case 5 : dUnit = edJogUsr_Rail -> Text.ToDouble() ; break ;
                    }
        case 3 :    switch(rgJogUnit_Head -> ItemIndex) {
                        case 0 : dUnit = 0.0                              ;
                                 bJog  = true                             ; break ;
                        case 1 : dUnit = 1                                ; break ;
                        case 2 : dUnit = 0.5                              ; break ;
                        case 3 : dUnit = 0.1                              ; break ;
                        case 4 : dUnit = 0.05                             ; break ;
                        case 5 : dUnit = edJogUsr_Head -> Text.ToDouble() ; break ;
                    }
        case 4 :    switch(rgJogUnit_Trim -> ItemIndex) {
                        case 0 : dUnit = 0.0                              ;
                                 bJog  = true                             ; break ;
                        case 1 : dUnit = 1                                ; break ;
                        case 2 : dUnit = 0.5                              ; break ;
                        case 3 : dUnit = 0.1                              ; break ;
                        case 4 : dUnit = 0.05                             ; break ;
                        case 5 : dUnit = edJogUsr_Trim -> Text.ToDouble() ; break ;
                    }

        case 5 :    switch(rgJogUnit_Trim -> ItemIndex) {
                        case 0 : dUnit = 0.0                              ;
                                 bJog  = true                             ; break ;
                        case 1 : dUnit = 1                                ; break ;
                        case 2 : dUnit = 0.5                              ; break ;
                        case 3 : dUnit = 0.1                              ; break ;
                        case 4 : dUnit = 0.05                             ; break ;
                        case 5 : dUnit = edJogUsr_Trim -> Text.ToDouble() ; break ;
                    }
     }

     if(bJog) {
         if(bFwd)MT_JogP(iMotr) ;
         else    MT_JogN(iMotr) ;
         return ;

     }
     else {
         if(bFwd)MT_GoIncMan(iMotr, dUnit) ;
         else    MT_GoIncMan(iMotr,-dUnit) ;
     }
*/

/*
     int        iSel  = ((TBitBtn *)Sender) -> Tag ;
     int        iMotr = iSel % 10 ;

     MT_Stop(iMotr) ;


*/

void __fastcall TFrmDeviceSet::btATFwClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;

    bool bRet ;

    if(iSel == aiLDR_Cmp      ||
       iSel == aiLDR_Pusher   )  bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;

    if(iSel == aiPRB_Stopper  ||
       iSel == aiWK1_Align    ||
       iSel == aiWK1_Lift     ||
       iSel == aiWK1_Stopper  ||
       iSel == aiWK2_Align    ||
       iSel == aiWK2_Lift     ||
       iSel == aiWK2_Stopper  )  bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiHED_UVLight  )  bRet = HED.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiPSB_Align    ||
       iSel == aiPSB_Finger   ||
       iSel == aiPSB_Flpr     ||
       iSel == aiPSB_FlprCmp  ||
       iSel == aiPSB_PusherFB ||
       iSel == aiPSB_PusherUD ||
       iSel == aiPSB_Trimmer  )  bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiULD_Cmp      )  bRet = ULD.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;

    if(!bRet) return ;

    AT_MoveCyl(iSel , ccFwd) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btATBwClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;

    bool bRet ;

    if(iSel == aiLDR_Cmp      ||
       iSel == aiLDR_Pusher   )  bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;

    if(iSel == aiPRB_Stopper  ||
       iSel == aiWK1_Align    ||
       iSel == aiWK1_Lift     ||
       iSel == aiWK1_Stopper  ||
       iSel == aiWK2_Align    ||
       iSel == aiWK2_Lift     ||
       iSel == aiWK2_Stopper  )  bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiHED_UVLight  )  bRet = HED.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiPSB_Align    ||
       iSel == aiPSB_Finger   ||
       iSel == aiPSB_Flpr     ||
       iSel == aiPSB_FlprCmp  ||
       iSel == aiPSB_PusherFB ||
       iSel == aiPSB_PusherUD ||
       iSel == aiPSB_Trimmer  )  bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiULD_Cmp      )  bRet = ULD.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;

    if(!bRet) return ;

    AT_MoveCyl(iSel , ccBwd) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::pcDeviceSetChanging(TObject *Sender,
      bool &AllowChange)
{
/*
     switch ( pcDeviceSet -> ActivePageIndex ) {
         case 0 :                             break ;
         case 1 : rgUnitUser = rgLDRJogUnit ;
                  edUnitUser = edLDRJogUnit ; break ;
         case 2 : rgUnitUser = rgRALJogUnit ;
                  edUnitUser = edRALJogUnit ; break ;
         case 3 : rgUnitUser = rgHEDJogUnit ;
                  edUnitUser = edHEDJogUnit ; break ;
         case 4 : rgUnitUser = rgTRMJogUnit ;
                  edUnitUser = edTRMJogUnit ; break ;
         case 5 : rgUnitUser = rgULDJogUnit ;
                  edUnitUser = edULDJogUnit ; break ;
     }
*/
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btMTBwDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int    iSel  = ((TBitBtn *)Sender) -> Tag ;
    bool   bJog  = false ;
    double dUnit = 0.0 ;

    switch(rgUnitUser -> ItemIndex) {
        case 0 : dUnit = 0.0                           ;
                 bJog  = true                          ; break ;
        case 1 : dUnit = 1                             ; break ;
        case 2 : dUnit = 0.5                           ; break ;
        case 3 : dUnit = 0.1                           ; break ;
        case 4 : dUnit = 0.05                          ; break ;
        case 5 : dUnit = edUnitUser -> Text.ToDouble() ; break ;
    }

    if(bJog) {
        MT_JogN(iSel) ;

    }
    else {
        MT_GoIncMan(iSel , -dUnit) ;
    }





    /*
    if(iSel == miLDR_YCmp ||
       iSel == miLDR_ZCmp ){
    }

    else if(iSel == miWK1_YVsn ||
            iSel == miWK1_ZVsn ||
            iSel == miWK2_YVsn ||
            iSel == miWRK_XVsn ){
    }

    else if(iSel == miPSB_TSrt ||
            iSel == miPSB_XTrm ||
            iSel == miPSB_YTrm ||
            iSel == miPSB_XIns ){
    }

    else if(iSel == miULD_YCmp ||
            iSel == miULD_ZCmp ){

    }
    */
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btMTFwMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int    iSel  = ((TBitBtn *)Sender) -> Tag ;
    bool   bJog  = false ;
    double dUnit = 0.0 ;

    switch(rgUnitUser -> ItemIndex) {
        case 0 : dUnit = 0.0                           ;
                 bJog  = true                          ; break ;
        case 1 : dUnit = 1                             ; break ;
        case 2 : dUnit = 0.5                           ; break ;
        case 3 : dUnit = 0.1                           ; break ;
        case 4 : dUnit = 0.05                          ; break ;
        case 5 : dUnit = edUnitUser -> Text.ToDouble() ; break ;
    }

    if(bJog) {
        MT_JogP(iSel) ;
    }
    else {
        MT_GoIncMan(iSel , dUnit) ;
    }
}
//---------------------------------------------------------------------------



void __fastcall TFrmDeviceSet::btMTBwUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int    iSel  = ((TBitBtn *)Sender) -> Tag ;

    if(rgUnitUser -> ItemIndex == 0) MT_Stop(iSel) ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btMTFwMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;

    if(rgUnitUser -> ItemIndex == 0) MT_Stop(iSel) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btIOClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;

    IO_SetY(iSel , !IO_GetY(iSel));
}
//---------------------------------------------------------------------------


void __fastcall TFrmDeviceSet::pcDeviceSetChange(TObject *Sender)
{
     switch ( pcDeviceSet -> ActivePageIndex ) {
         case 0 :                             break ;
         case 1 : rgUnitUser = rgLDRJogUnit ;
                  edUnitUser = edLDRJogUnit ; break ;
         case 2 : rgUnitUser = rgRALJogUnit ;
                  edUnitUser = edRALJogUnit ; break ;
         case 3 : rgUnitUser = rgHEDJogUnit ;
                  edUnitUser = edHEDJogUnit ; break ;
         case 4 : rgUnitUser = rgTRMJogUnit ;
                  edUnitUser = edTRMJogUnit ; break ;
         case 5 : rgUnitUser = rgULDJogUnit ;
                  edUnitUser = edULDJogUnit ; break ;
     }
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::TrimMotrClick(TObject *Sender)
{
    int iSel   = ((TBitBtn *)Sender) -> Tag ;

    switch (iSel) {
        case 0 : MM.SetManCycle(mcPSB_WorkStartPos); break ;
        case 1 : MM.SetManCycle(mcPSB_PchYn       ); break ;
        case 2 : MM.SetManCycle(mcPSB_PchYp       ); break ;
        case 3 : MM.SetManCycle(mcPSB_PchXn       ); break ;
        case 4 : MM.SetManCycle(mcPSB_PchXp       ); break ;
    }


}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::BitBtn6Click(TObject *Sender)
{
    int iSel   = ((TBitBtn *)Sender) -> Tag ;

    switch (iSel) {
        case 0 : MM.SetManCycle(mcHED_StartPos); break ;
        case 1 : MM.SetManCycle(mcHED_PchYn   ); break ;
        case 2 : MM.SetManCycle(mcHED_PchYp   ); break ;
        case 3 : MM.SetManCycle(mcHED_PchXn   ); break ;
        case 4 : MM.SetManCycle(mcHED_PchXp   ); break ;
    }
}
//---------------------------------------------------------------------------
bool __fastcall TFrmDeviceSet::LanguageSel(EN_LAN_SEL lvType)
{
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sName    , sNameTemp    ;
    AnsiString sCaption , sCaptionTemp ;
    AnsiString sComName ;
    bool       bOri     ;

    for (int i = 0; i <= MAX_LAN_SEL   ; i++)
    {
             if(i == (int)lsEnglish ) {sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + "_E.ini" ; bOri = false ;}
        else if(i == (int)lsKorean  ) {sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + "_K.ini" ; bOri = false ;}
        else if(i == (int)lsChinese ) {sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + "_C.ini" ; bOri = false ;}
        else                          {sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + ".ini"   ;
                                       UserINI.ClearFile(sPath) ;
                                       bOri  = true ;                                                  }

        for (int i = 0; i < ComponentCount; i++)
        {
            if (Components[i]->ClassNameIs("TLabel"))
            {
                sComName = Components[i]->ClassName();
                sName    = ((TLabel*)FindComponent(Components[i]->Name))->Name          ;
                sCaption = ((TLabel*)FindComponent(Components[i]->Name))->Caption       ;
            }
            else if (Components[i]->ClassNameIs("TCheckBox")) {
                sComName = Components[i]->ClassName();
                sName    = ((TCheckBox*)FindComponent(Components[i]->Name))->Name       ;
                sCaption = ((TCheckBox*)FindComponent(Components[i]->Name))->Caption    ;
            }
            else if (Components[i]->ClassNameIs("TButton")) {
                sComName = Components[i]->ClassName();
                sName    = ((TButton*)FindComponent(Components[i]->Name))->Name         ;
                sCaption = ((TButton*)FindComponent(Components[i]->Name))->Caption      ;
            }
            else if (Components[i]->ClassNameIs("TBitBtn")) {
                sComName = Components[i]->ClassName();
                sName    = ((TBitBtn*)FindComponent(Components[i]->Name))->Name         ;
                sCaption = ((TBitBtn*)FindComponent(Components[i]->Name))->Caption      ;
            }
            else if (Components[i]->ClassNameIs("TSpeedButton")) {
                sComName = Components[i]->ClassName();
                sName    = ((TSpeedButton*)FindComponent(Components[i]->Name))->Name    ;
                sCaption = ((TSpeedButton*)FindComponent(Components[i]->Name))->Caption ;
            }
            else if (Components[i]->ClassNameIs("TTabSheet")) {
                sComName = Components[i]->ClassName();
                sName    = ((TTabSheet*)FindComponent(Components[i]->Name))->Name       ;
                sCaption = ((TTabSheet*)FindComponent(Components[i]->Name))->Caption    ;
            }
            else if (Components[i]->ClassNameIs("TGroupBox")) {
                sComName = Components[i]->ClassName();
                sName    = ((TGroupBox*)FindComponent(Components[i]->Name))->Name       ;
                sCaption = ((TGroupBox*)FindComponent(Components[i]->Name))->Caption    ;
            }
            else if (Components[i]->ClassNameIs("TStaticText")) {
                sComName = Components[i]->ClassName();
                sName    = ((TGroupBox*)FindComponent(Components[i]->Name))->Name       ;
                sCaption = ((TGroupBox*)FindComponent(Components[i]->Name))->Caption    ;
            }
            else continue ;

            if(sComName == "" || sName == "") continue ;

            //Save Device.
            if(bOri) UserINI.Save(sPath.c_str()  , sComName  , sName , sCaption);
            else if(sCaption != "") {
                UserINI.Load(sPath.c_str()  , sComName  , sName , sCaptionTemp);
                if( sCaptionTemp == "" ) UserINI.Save(sPath.c_str()  , sComName  , sName , sCaption);
            }
        }
    }

    //Set Dir. Load
         if(lvType == lsEnglish ) sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + "_E.ini" ;
    else if(lvType == lsKorean  ) sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + "_K.ini" ;
    else if(lvType == lsChinese ) sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + "_C.ini" ;
    else                          return true ;

    for (int i = 0; i < ComponentCount; i++)
    {
        if (Components[i]->ClassNameIs("TLabel"))
        {
            sComName = Components[i]->ClassName();
            sName    = ((TLabel*)FindComponent(Components[i]->Name))->Name          ;
            sCaption = ((TLabel*)FindComponent(Components[i]->Name))->Caption       ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TLabel*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TLabel*)FindComponent(Components[i]->Name))->Caption = sCaption       ;
        }
        else if (Components[i]->ClassNameIs("TCheckBox")) {
            sComName = Components[i]->ClassName();
            sName    = ((TCheckBox*)FindComponent(Components[i]->Name))->Name       ;
            sCaption = ((TCheckBox*)FindComponent(Components[i]->Name))->Caption    ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TCheckBox*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TCheckBox*)FindComponent(Components[i]->Name))->Caption = sCaption       ;

        }
        else if (Components[i]->ClassNameIs("TButton")) {
            sComName = Components[i]->ClassName();
            sName    = ((TButton*)FindComponent(Components[i]->Name))->Name         ;
            sCaption = ((TButton*)FindComponent(Components[i]->Name))->Caption      ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TButton*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TButton*)FindComponent(Components[i]->Name))->Caption = sCaption       ;

        }
        else if (Components[i]->ClassNameIs("TBitBtn")) {
            sComName = Components[i]->ClassName();
            sName    = ((TBitBtn*)FindComponent(Components[i]->Name))->Name         ;
            sCaption = ((TBitBtn*)FindComponent(Components[i]->Name))->Caption      ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TBitBtn*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TBitBtn*)FindComponent(Components[i]->Name))->Caption = sCaption       ;

        }
        else if (Components[i]->ClassNameIs("TSpeedButton")) {
            sComName = Components[i]->ClassName();
            sName    = ((TSpeedButton*)FindComponent(Components[i]->Name))->Name    ;
            sCaption = ((TSpeedButton*)FindComponent(Components[i]->Name))->Caption ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TSpeedButton*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TSpeedButton*)FindComponent(Components[i]->Name))->Caption = sCaption       ;
        }
        else if (Components[i]->ClassNameIs("TTabSheet")) {
            sComName = Components[i]->ClassName();
            sName    = ((TTabSheet*)FindComponent(Components[i]->Name))->Name       ;
            sCaption = ((TTabSheet*)FindComponent(Components[i]->Name))->Caption    ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TTabSheet*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TTabSheet*)FindComponent(Components[i]->Name))->Caption = sCaption       ;
        }
        else if (Components[i]->ClassNameIs("TGroupBox")) {
            sComName = Components[i]->ClassName();
            sName    = ((TGroupBox*)FindComponent(Components[i]->Name))->Name       ;
            sCaption = ((TGroupBox*)FindComponent(Components[i]->Name))->Caption    ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TGroupBox*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TGroupBox*)FindComponent(Components[i]->Name))->Caption = sCaption       ;
        }
        else if (Components[i]->ClassNameIs("TStaticText")) {
            sComName = Components[i]->ClassName();
            sName    = ((TStaticText*)FindComponent(Components[i]->Name))->Name     ;
            sCaption = ((TStaticText*)FindComponent(Components[i]->Name))->Caption  ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TStaticText*)FindComponent(Components[i]->Name))->Name    = sName     ;
            ((TStaticText*)FindComponent(Components[i]->Name))->Caption = sCaption  ;
        }
    }

    return true ;
}
//---------------------------------------------------------------------------

