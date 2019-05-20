//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDeviceSet.h"
#include "FormProbe.h"
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuffer.h"
#include "WorkZone.h"
#include "PostBuff.h"
#include "UnLoader.h"
//---------------------------------------------------------------------------
#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "ManualMan.h"
#include "ProbeComUnit.h"
#include "RS232Man.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
__fastcall TFrmDeviceSet::TFrmDeviceSet(TComponent* Owner)
    : TForm(Owner)
{

    PM.SetWindow(pnPRB_XIdxPos , miPRB_XIdx);
    PM.SetWindow(pnPSB_XIdxPos , miPSB_XIdx);
    PM.SetWindow(pnWRK_ZTopPos , miWRK_ZTop);
    PM.SetWindow(pnWRK_YCamPos , miWRK_YCam);
    PM.SetWindow(pnWRK_XBtmPos , miWRK_XBtm);
    PM.SetWindow(pnWRK_YBtmPos , miWRK_YBtm);
    PM.SetWindow(pnWRK_ZBtmPos , miWRK_ZBtm);

    PM.SetWindow(pnRJT_XBtmPos , miREJ_XBtm);
    PM.SetWindow(pnSTL_YTopPos , miSTL_YTop);
    PM.SetWindow(pnSpare       , miSpare   );
    PM.SetWindow(pnLDR_YCmpPos , miLDR_YCmp);
    PM.SetWindow(pnLDR_ZCmpPos , miLDR_ZCmp);
    PM.SetWindow(pnULD_YCmpPos , miULD_YCmp);
    PM.SetWindow(pnULD_ZCmpPos , miULD_ZCmp);
//    PM.SetWindow(pnLd2Z,miLD2_Z);

  //  pcDeviceSet -> ActivePageIndex = 0 ;
    //Motor Window Set to Panel
    MtWindowSet();
    AtWindowSet();

    ShowSet();

    foLDRAcIn  = new TFraOutput(); foLDRAcIn   -> SetConfig(yLDR_MgzInAC    , IO_GetYName(yLDR_MgzInAC    ), pnIO1  );
    foLDRAcOut = new TFraOutput(); foLDRAcOut  -> SetConfig(yLDR_MgzOutAC   , IO_GetYName(yLDR_MgzOutAC   ), pnIO2  );
    foPREAc    = new TFraOutput(); foPREAc     -> SetConfig(yPRB_FeedingAC  , IO_GetYName(yPRB_FeedingAC  ), pnIO3  );
    foPSBAc    = new TFraOutput(); foPSBAc     -> SetConfig(yPSB_FeedingAC  , IO_GetYName(yPSB_FeedingAC  ), pnIO4  );
    foPSBBlwr  = new TFraOutput(); foPSBBlwr   -> SetConfig(ySTL_Blower     , IO_GetYName(ySTL_Blower     ), pnIO5  );
    foULDACIn  = new TFraOutput(); foULDACIn   -> SetConfig(yULD_MgzInAC    , IO_GetYName(yULD_MgzInAC    ), pnIO6  );
    foULDAcOut = new TFraOutput(); foULDAcOut  -> SetConfig(yULD_MgzOutAC   , IO_GetYName(yULD_MgzOutAC   ), pnIO7  );

}


void __fastcall TFrmDeviceSet::MtWindowSet()
{
    
}


void __fastcall TFrmDeviceSet::AtWindowSet()
{
    //실린더 창.
    TPanel * pActr ;

    for(int i=0 ; i<MAX_ACTR ; i++) {
        pActr = ((TPanel*)FindComponent("pnAtcr" + AnsiString(i))) ;
        FraCylinder[i] = new TFraCylOneBt(pActr);

        switch(i){ // juhyeon
            default :                                                                                                    break;
            case aiLDR_Cmp  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , LDR.CheckSafe ); break;
            case aiLDR_Psh  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , LDR.CheckSafe ); break;
            case aiPRB_Rlr  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , PRB.CheckSafe ); break;
            case aiPRB_Lft  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , PRB.CheckSafe ); break;
            case aiPRB_Rgt  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , PRB.CheckSafe ); break;
            case aiWRK_Idx  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , WRK.CheckSafe ); break;
            case aiPSB_Lft  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , PSB.CheckSafe ); break;
            case aiPSB_Rgt  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , PSB.CheckSafe ); break;
            case aiSTL_Top  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , PSB.CheckSafe ); break;
            case aiSTL_Btm  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , PSB.CheckSafe ); break;
            case aiPSB_Hld  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , PSB.CheckSafe ); break;
            case aiPSB_Rolr : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , PSB.CheckSafe ); break;
            case aiULD_Cmp  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , ULD.CheckSafe ); break;
        }

        FraCylinder[i] ->Show();
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btSavePosClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
                                                                        
    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;                  

    UpdateDevInfo(toBuff);
    OM.SaveDevInfo(OM.m_sCrntDev);

    UpdateDevOptn(toBuff);
    OM.SaveDevOptn(OM.m_sCrntDev);

    PM.UpdatePstn(toBuff);
    PM.Save(OM.m_sCrntDev);

    
    if(DM.ARAY[riLDR].GetMaxRow() != OM.DevInfo.iCsSlCnt                                                       ) DM.ARAY[riLDR].SetMaxColRow(1                      , OM.DevInfo.iCsSlCnt);
    if(DM.ARAY[riPRB].GetMaxRow() != OM.DevInfo.iRowCnt || DM.ARAY[riPRB].GetMaxCol() != OM.DevInfo.iColCnt    ) DM.ARAY[riPRB].SetMaxColRow(OM.DevInfo.iColCnt     , OM.DevInfo.iRowCnt );
    if(DM.ARAY[riWRK].GetMaxRow() != OM.DevInfo.iRowCnt || DM.ARAY[riWRK].GetMaxCol() != OM.DevInfo.iColCnt    ) DM.ARAY[riWRK].SetMaxColRow(OM.DevInfo.iColCnt     , OM.DevInfo.iRowCnt );
    if(DM.ARAY[riPSB].GetMaxRow() != OM.DevInfo.iRowCnt || DM.ARAY[riPSB].GetMaxCol() != OM.DevInfo.iColCnt    ) DM.ARAY[riPSB].SetMaxColRow(OM.DevInfo.iColCnt     , OM.DevInfo.iRowCnt );
    //if(DM.ARAY[riTmp].GetMaxRow() != OM.DevInfo.iRowCnt || DM.ARAY[riTmp].GetMaxCol() != OM.DevOptn.iContColCnt) DM.ARAY[riTmp].SetMaxColRow(OM.DevOptn.iContColCnt , OM.DevInfo.iRowCnt );
    if(DM.ARAY[riULD].GetMaxRow() != OM.DevInfo.iCsSlCnt                                                       ) DM.ARAY[riULD].SetMaxColRow(1                      , OM.DevInfo.iCsSlCnt);
    if(DM.ARAY[riOPT].GetMaxRow() != OM.DevInfo.iRowCnt || DM.ARAY[riPSB].GetMaxCol() != OM.DevInfo.iColCnt    ) DM.ARAY[riOPT].SetMaxColRow(OM.DevInfo.iColCnt     , OM.DevInfo.iRowCnt );
    //Option으로 검사 설정 바꿀수 있도록 수정함 JS

   // DM.ARAY[riLDR].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);
   // DM.ARAY[riPR1].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
   // DM.ARAY[riPR2].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
   // DM.ARAY[riPSB].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
   // DM.ARAY[riWRK].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
   // DM.ARAY[riULD].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);
    //DM.ARAY[riSTL].SetMaxColRow(1                 ,1                  );

    //DM.ARAY[riPR2].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    //DM.ARAY[riWRP].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    //DM.ARAY[riPS2].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );

    /*
    if(OM.DevOptn.bLtHeaterNum2) IO_SetY(yETC_Heater1_2,true );
    else                         IO_SetY(yETC_Heater1_2,false);
    if(OM.DevOptn.bLtHeaterNum3) IO_SetY(yETC_Heater1_3,true );
    else                         IO_SetY(yETC_Heater1_3,false);
    if(OM.DevOptn.bRtHeaterNum2) IO_SetY(yETC_Heater2_2,true );
    else                         IO_SetY(yETC_Heater2_2,false);
    if(OM.DevOptn.bRtHeaterNum3) IO_SetY(yETC_Heater2_3,true );
    else                         IO_SetY(yETC_Heater2_3,false);
    광폭에서 협폭 조절시 히터 센서 끄는 용도인데 정부과제에서 빠짐. JS
    */
    FrmProbe -> Init() ;

    PCM.SetDaegyeom();
    //if(OM.MstOptn.bUseDaegyeomProbe) PCM.SetDaegyeom();  //대겸 Eng 프로브 사용시.
    //else                             FrmMain -> tmMainSet -> Enabled = true ;

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::tmUpdateTimerTimer(TObject *Sender)
{
    tmUpdateTimer->Enabled = false;

    bool bFoward;
    AnsiString sMsg;

    btSaveDevice -> Enabled = !SEQ._bRun;

    switch ( pcDeviceSet -> ActivePageIndex )
    {
        case 0 :
        break;

        case 1 : break;

        case 2 : break;

        case 3 : break;

        case 4 : break;

        case 5 : break;

        case 6 : break;
    }

    tmUpdateTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormDestroy(TObject *Sender)
{
    tmUpdateTimer->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevInfo(bool bTo)
{
    if(bTo)
    {
        edColPitch       -> Text = OM.DevInfo.dColPitch    ;
        edRowPitch       -> Text = OM.DevInfo.dRowPitch    ;
        edColGroupCount  -> Text = OM.DevInfo.iColGrCnt    ;
        edRowGroupCount  -> Text = OM.DevInfo.iRowGrCnt    ;

        edColGroupGap    -> Text = OM.DevInfo.dColGrGap    ;
        edRowGroupGap    -> Text = OM.DevInfo.dRowGrGap    ;

        edColCount       -> Text = OM.DevInfo.iColCnt      ;
        edRowCount       -> Text = OM.DevInfo.iRowCnt      ;

        edCsSlotCnt      -> Text = OM.DevInfo.iCsSlCnt     ;
        edCsSlotPitch    -> Text = OM.DevInfo.dCsSlPitch   ;

        edRowSubGroupGap -> Text = OM.DevInfo.dRowSubGrGap ;
        edRowSubGrCount  -> Text = OM.DevInfo.iRowSubGrCnt ;
    }
    else
    {
        OM.DevInfo.dColPitch    = edColPitch      -> Text.ToDouble() ;
        OM.DevInfo.dRowPitch    = edRowPitch      -> Text.ToDouble() ;

        OM.DevInfo.iColGrCnt    = edColGroupCount -> Text.ToInt()    ;
        OM.DevInfo.iRowGrCnt    = edRowGroupCount -> Text.ToInt()    ;

        OM.DevInfo.dColGrGap    = edColGroupGap   -> Text.ToDouble() ;
        OM.DevInfo.dRowGrGap    = edRowGroupGap   -> Text.ToDouble() ;

        OM.DevInfo.iColCnt      = edColCount      -> Text.ToInt()    ;
        OM.DevInfo.iRowCnt      = edRowCount      -> Text.ToInt()    ;

        OM.DevInfo.iCsSlCnt     = edCsSlotCnt     -> Text.ToInt()    ;
        OM.DevInfo.dCsSlPitch   = edCsSlotPitch   -> Text.ToDouble() ;

        OM.DevInfo.dRowSubGrGap = edRowSubGroupGap -> Text.ToDouble();
        OM.DevInfo.iRowSubGrCnt = edRowSubGrCount  -> Text.ToInt()   ;
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevOptn(bool bTo)
{
    if(bTo)
    {
        //Probe.
        edWorkXWaitOfs       -> Text      = OM.DevOptn.dWorkXWaitOfs     ;

        ed1ChHigh            -> Text      = OM.DevOptn.d1ChHigh          ;
        ed1ChLow             -> Text      = OM.DevOptn.d1ChLow           ;
        ed2ChHigh            -> Text      = OM.DevOptn.d2ChHigh          ;
        ed2ChLow             -> Text      = OM.DevOptn.d2ChLow           ;
        edZennerHigh         -> Text      = OM.DevOptn.dZenerHigh        ;
        edZennerLow          -> Text      = OM.DevOptn.dZenerLow         ;
        edPinCnt             -> Text      = OM.DevOptn.iPinCnt           ;
        edLimInsFailCnt      -> Text      = OM.DevOptn.iLimInsFailCnt    ;
        edReInspCnt          -> Text      = OM.DevOptn.iReInspCnt        ;
        edFrtPrcFailCnt      -> Text      = OM.DevOptn.iFrtPrcFailCnt    ;
        edContactCnt         -> Text      = OM.DevOptn.iContColCnt       ;

        edPrbSttDelay        -> Text      = OM.DevOptn.iPrbSttDelay      ;
        cbUseZenner          -> Checked   = OM.DevOptn.bUseZener         ;
        cb1Ch1P              -> ItemIndex = OM.DevOptn.i1Ch1P            ;
        cb1Ch1N              -> ItemIndex = OM.DevOptn.i1Ch1N            ;
        cb1Ch2P              -> ItemIndex = OM.DevOptn.i1Ch2P            ;
        cb1Ch2N              -> ItemIndex = OM.DevOptn.i1Ch2N            ;
        cb1Ch3P              -> ItemIndex = OM.DevOptn.i1Ch3P            ;
        cb1Ch3N              -> ItemIndex = OM.DevOptn.i1Ch3N            ;
        ed1ChLedCur          -> Text      = OM.DevOptn.i1ChLedCur        ;
        ed1ChZenCur          -> Text      = OM.DevOptn.i1ChZenCur        ;
        cb2Ch1P              -> ItemIndex = OM.DevOptn.i2Ch1P            ;
        cb2Ch1N              -> ItemIndex = OM.DevOptn.i2Ch1N            ;
        cb2Ch2P              -> ItemIndex = OM.DevOptn.i2Ch2P            ;
        cb2Ch2N              -> ItemIndex = OM.DevOptn.i2Ch2N            ;
        cb2Ch3P              -> ItemIndex = OM.DevOptn.i2Ch3P            ;
        cb2Ch3N              -> ItemIndex = OM.DevOptn.i2Ch3N            ;
        ed2ChLedCur          -> Text      = OM.DevOptn.i2ChLedCur        ;
        ed2ChZenCur          -> Text      = OM.DevOptn.i2ChZenCur        ;
        cbUseTwiceInsp       -> Checked   = OM.DevOptn.bUseTwiceInsp     ;
        cbSideLEDControl     -> Checked   = OM.DevOptn.bSideLEDControl   ;
        edWRKZTopSpeed       -> Text      = OM.DevOptn.dWRKZTopSpeed     ;
        cbWRKZTopRepeat      -> Checked   = OM.DevOptn.bWRKZTopRepeat    ;


        if(OM.DevOptn.bSideLEDControl){
            ed1ChDelay           -> Text      = OM.DevOptn.i1ChDelay        ;
            ed1ChLedTime         -> Text      = OM.DevOptn.i1ChLedTime      ;
            ed1ChZenTime         -> Text      = OM.DevOptn.i1ChZenTime      ;
            ed2ChDelay           -> Text      = OM.DevOptn.i2ChDelay        ;
            ed2ChLedTime         -> Text      = OM.DevOptn.i2ChLedTime      ;
            ed2ChZenTime         -> Text      = OM.DevOptn.i2ChZenTime      ;
        }
        else {
            ed1ChDelay           -> Text      = OM.DevOptn.i1ChDelay        ;
            ed1ChLedTime         -> Text      = OM.DevOptn.i1ChLedTime / 100;
            ed1ChZenTime         -> Text      = OM.DevOptn.i1ChZenTime / 100;
            ed2ChDelay           -> Text      = OM.DevOptn.i2ChDelay        ;
            ed2ChLedTime         -> Text      = OM.DevOptn.i2ChLedTime / 100;
            ed2ChZenTime         -> Text      = OM.DevOptn.i2ChZenTime / 100;
        }




        cbChRvsCrnt          -> Checked   = OM.DevOptn.bChRvsCrnt        ;

        //Daegyoem
        edDgChipCnt          -> Text      = OM.DevOptn.iDgChipCnt        ;
        edSwitchingDly       -> Text      = OM.DevOptn.iSwitchingDly     ;


        //Dev Option
        edLotEndMgzCnt       -> Text      = OM.DevOptn.iLotEndMgzCnt     ;
        edWaitDelay          -> Text      = OM.DevOptn.iWaitDelay        ;

        //cbLtHeaterNum2       -> Checked   = OM.DevOptn.bLtHeaterNum2     ;
        //cbLtHeaterNum3       -> Checked   = OM.DevOptn.bLtHeaterNum3     ;
        //cbRtHeaterNum2       -> Checked   = OM.DevOptn.bRtHeaterNum2     ;
        //cbRtHeaterNum3       -> Checked   = OM.DevOptn.bRtHeaterNum3     ;

        edREJPitch           -> Text      = OM.DevOptn.dREJPitch         ;

        edTrimPtchOfs        -> Text      = OM.DevOptn.dTrimPtchOfs      ;
        edWRKPtchOfs         -> Text      = OM.DevOptn.dWRKPtchOfs       ;
        //edTrimMoveOfs        -> Text      = OM.DevOptn.dTrimMoveOfs      ;

        edPSBReGripOfs       -> Text      = OM.DevOptn.dPSBReGripOfs     ;
        edPSBReGripCol       -> Text      = OM.DevOptn.iPSBReGripCol     ;

        edTrimRjtDly         -> Text      = OM.DevOptn.iTrimRjtDly       ;
        //edTrimSnrDly         -> Text      = OM.DevOptn.iTrimSnrDly       ;

        edContactPinCnt      -> Text      = OM.DevOptn.iContactPinCnt    ;
        edLimContactPinCnt   -> Text      = OM.DevOptn.iLimContactPinCnt ;

        edRejectMoveDeley    -> Text      = OM.DevOptn.iRejectMoveDeley  ;

        edPRBRlrDelay        -> Text      = OM.DevOptn.iPRBRlrDelay      ;

        edPRBRlrOffDelay     -> Text      = OM.DevOptn.iPRBRlrOffDelay   ;

    }
    else
    {
        // Probe
        OM.DevOptn.dWorkXWaitOfs     =  edWorkXWaitOfs      -> Text.ToDouble() ;

        OM.DevOptn.d1ChHigh          =  ed1ChHigh           -> Text.ToDouble()  ;
        OM.DevOptn.d1ChLow           =  ed1ChLow            -> Text.ToDouble()  ;
        OM.DevOptn.d2ChHigh          =  ed2ChHigh           -> Text.ToDouble()  ;
        OM.DevOptn.d2ChLow           =  ed2ChLow            -> Text.ToDouble()  ;
        OM.DevOptn.dZenerHigh        =  edZennerHigh        -> Text.ToDouble()  ;
        OM.DevOptn.iPrbSttDelay      =  edPrbSttDelay       -> Text.ToIntDef(0) ;
        OM.DevOptn.dZenerLow         =  edZennerLow         -> Text.ToDouble()  ;
        OM.DevOptn.iPinCnt           =  edPinCnt            -> Text.ToIntDef(0) ;
        OM.DevOptn.iLimInsFailCnt    =  edLimInsFailCnt     -> Text.ToIntDef(0) ;
        OM.DevOptn.iReInspCnt        =  edReInspCnt         -> Text.ToIntDef(0) ;
        OM.DevOptn.iFrtPrcFailCnt    =  edFrtPrcFailCnt     -> Text.ToIntDef(100);

        OM.DevOptn.iContColCnt       =  edContactCnt        -> Text.ToIntDef(0) ;
        OM.DevOptn.bUseZener         =  cbUseZenner         -> Checked          ;
        OM.DevOptn.i1Ch1P            =  cb1Ch1P             -> ItemIndex        ;
        OM.DevOptn.i1Ch1N            =  cb1Ch1N             -> ItemIndex        ;
        OM.DevOptn.i1Ch2P            =  cb1Ch2P             -> ItemIndex        ;
        OM.DevOptn.i1Ch2N            =  cb1Ch2N             -> ItemIndex        ;
        OM.DevOptn.i1Ch3P            =  cb1Ch3P             -> ItemIndex        ;
        OM.DevOptn.i1Ch3N            =  cb1Ch3N             -> ItemIndex        ;
        OM.DevOptn.i1ChLedCur        =  ed1ChLedCur         -> Text.ToIntDef(0) ;
        OM.DevOptn.i1ChZenCur        =  ed1ChZenCur         -> Text.ToIntDef(0) ;
        OM.DevOptn.i2Ch1P            =  cb2Ch1P             -> ItemIndex        ;
        OM.DevOptn.i2Ch1N            =  cb2Ch1N             -> ItemIndex        ;
        OM.DevOptn.i2Ch2P            =  cb2Ch2P             -> ItemIndex        ;
        OM.DevOptn.i2Ch2N            =  cb2Ch2N             -> ItemIndex        ;
        OM.DevOptn.i2Ch3P            =  cb2Ch3P             -> ItemIndex        ;
        OM.DevOptn.i2Ch3N            =  cb2Ch3N             -> ItemIndex        ;
        OM.DevOptn.i2ChLedCur        =  ed2ChLedCur         -> Text.ToIntDef(0) ;
        OM.DevOptn.i2ChZenCur        =  ed2ChZenCur         -> Text.ToIntDef(0) ;
        OM.DevOptn.bSideLEDControl   =  cbSideLEDControl    -> Checked          ;
        OM.DevOptn.dWRKZTopSpeed     =  edWRKZTopSpeed      -> Text.ToIntDef(1) ;
        OM.DevOptn.bWRKZTopRepeat    =  cbWRKZTopRepeat     -> Checked          ;

        if(OM.DevOptn.bSideLEDControl){
            OM.DevOptn.i1ChDelay         =  ed1ChDelay          -> Text.ToIntDef(0) ;
            OM.DevOptn.i1ChLedTime       =  ed1ChLedTime        -> Text.ToIntDef(0) ;
            OM.DevOptn.i1ChZenTime       =  ed1ChZenTime        -> Text.ToIntDef(0) ;
            OM.DevOptn.i2ChDelay         =  ed2ChDelay          -> Text.ToIntDef(0) ;
            OM.DevOptn.i2ChLedTime       =  ed2ChLedTime        -> Text.ToIntDef(0) ;
            OM.DevOptn.i2ChZenTime       =  ed2ChZenTime        -> Text.ToIntDef(0) ;
        }
        else {
            OM.DevOptn.i1ChDelay         =  ed1ChDelay          -> Text.ToIntDef(0) ;
            OM.DevOptn.i1ChLedTime       =  ed1ChLedTime        -> Text.ToIntDef(0) * 100;
            OM.DevOptn.i1ChZenTime       =  ed1ChZenTime        -> Text.ToIntDef(0) * 100;
            OM.DevOptn.i2ChDelay         =  ed2ChDelay          -> Text.ToIntDef(0) ;
            OM.DevOptn.i2ChLedTime       =  ed2ChLedTime        -> Text.ToIntDef(0) * 100;
            OM.DevOptn.i2ChZenTime       =  ed2ChZenTime        -> Text.ToIntDef(0) * 100;
        }
        OM.DevOptn.bChRvsCrnt        =  cbChRvsCrnt         -> Checked          ;
        OM.DevOptn.bUseTwiceInsp     =  cbUseTwiceInsp      -> Checked          ;

        //Daegyoem
        OM.DevOptn.iDgChipCnt        =  edDgChipCnt         -> Text.ToIntDef(1) ;
        OM.DevOptn.iSwitchingDly     =  edSwitchingDly      -> Text.ToIntDef(1) ;

        //Dev Option
        //OM.DevOptn.bLtHeaterNum2     =  cbLtHeaterNum2      -> Checked          ;
        //OM.DevOptn.bLtHeaterNum3     =  cbLtHeaterNum3      -> Checked          ;
        //OM.DevOptn.bRtHeaterNum2     =  cbRtHeaterNum2      -> Checked          ;
        //OM.DevOptn.bRtHeaterNum3     =  cbRtHeaterNum3      -> Checked          ;

        OM.DevOptn.dREJPitch         =  edREJPitch          -> Text.ToDouble( ) ;

        if(OM.DevOptn.iLotEndMgzCnt != edLotEndMgzCnt       -> Text.ToIntDef(0)){
            LDR._iMgzCnt             = edLotEndMgzCnt       -> Text.ToIntDef(0);
        }
        OM.DevOptn.iLotEndMgzCnt     =  edLotEndMgzCnt      -> Text.ToIntDef(0) ;
        OM.DevOptn.iWaitDelay        =  edWaitDelay         -> Text.ToIntDef(0) ;

        OM.DevOptn.dPSBReGripOfs     =  edPSBReGripOfs      -> Text.ToDouble( ) ;
        OM.DevOptn.iPSBReGripCol     =  edPSBReGripCol      -> Text.ToIntDef(0) ;

        OM.DevOptn.dTrimPtchOfs      =  edTrimPtchOfs       -> Text.ToDouble( ) ;
        OM.DevOptn.dWRKPtchOfs       =  edWRKPtchOfs        -> Text.ToDouble( ) ;

        OM.DevOptn.iTrimRjtDly       =  edTrimRjtDly        -> Text.ToIntDef(0) ;
        //OM.DevOptn.iTrimSnrDly       =  edTrimSnrDly        -> Text.ToIntDef(0) ;

        OM.DevOptn.iContactPinCnt    =  edContactPinCnt     -> Text.ToIntDef(0) ;
        OM.DevOptn.iLimContactPinCnt =  edLimContactPinCnt  -> Text.ToIntDef(0) ;
        OM.DevOptn.iRejectMoveDeley  =  edRejectMoveDeley   -> Text.ToIntDef(0) ;

        OM.DevOptn.iPRBRlrDelay      =  edPRBRlrDelay       -> Text.ToIntDef(0) ;

        OM.DevOptn.iPRBRlrOffDelay   =  edPRBRlrOffDelay    -> Text.ToIntDef(0) ;

    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormShow(TObject *Sender)
{
    UpdateDevInfo (toTabl);
    UpdateDevOptn (toTabl);
    PM.UpdatePstn (toTabl);

    pcDeviceSet -> ActivePageIndex = 1 ;


    switch ( pcDeviceSet -> ActivePageIndex ) {
        default: rgUnitUser = rgLDRJogUnit ;
                 edUnitUser = edLDRJogUnit ; break ;
        case 0 : rgUnitUser = rgLDRJogUnit ;
                 edUnitUser = edLDRJogUnit ; break ;
        case 1 : rgUnitUser = rgLDRJogUnit ;
                 edUnitUser = edLDRJogUnit ; break ;
        case 2 : rgUnitUser = rgRALJogUnit ;
                 edUnitUser = edRALJogUnit ; break ;
        case 3 : rgUnitUser = rgWRKJogUnit ;
                 edUnitUser = edWRKJogUnit ; break ;
        case 4 : rgUnitUser = rgSTLJogUnit ;
                 edUnitUser = edSTLJogUnit ; break ;
        case 5 : rgUnitUser = rgULDJogUnit ;
                 edUnitUser = edULDJogUnit ; break ;
        case 6 : rgUnitUser = rgULDJogUnit ;
                 edUnitUser = edULDJogUnit ; break ;
     }

    if(rgUnitUser->ItemIndex == 0)
        for(int i=0;i<MAX_MOTR;i++)
            FraMotor[i]->SetUnit(utJog ,0   );
    if(rgUnitUser->ItemIndex == 1)
        for(int i=0;i<MAX_MOTR;i++)
            FraMotor[i]->SetUnit(utMove,1   );
    if(rgUnitUser->ItemIndex == 2)
        for(int i=0;i<MAX_MOTR;i++)
            FraMotor[i]->SetUnit(utMove,0.5 );
    if(rgUnitUser->ItemIndex == 3)
        for(int i=0;i<MAX_MOTR;i++)
            FraMotor[i]->SetUnit(utMove,0.1 );
    if(rgUnitUser->ItemIndex == 4)
        for(int i=0;i<MAX_MOTR;i++)
            FraMotor[i]->SetUnit(utMove,0.05);
    if(rgUnitUser->ItemIndex == 5)
        for(int i=0;i<MAX_MOTR;i++)
            FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0));

    ShowSet();


    tmUpdateTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::pcDeviceSetChange(TObject *Sender)
{
    switch ( pcDeviceSet -> ActivePageIndex )
    {
        case 0 :

        break;

        case 1 : PM.SetParent(pnLDR_YCmpPos , miLDR_YCmp);
                 PM.SetParent(pnLDR_ZCmpPos , miLDR_ZCmp);
        break;

        case 2 : PM.SetParent(pnPRB_XIdxPos , miPRB_XIdx);
                 PM.SetParent(pnPSB_XIdxPos , miPSB_XIdx);
        break;

        case 3 : PM.SetParent(pnWRK_ZTopPos , miWRK_ZTop);
                 PM.SetParent(pnWRK_ZBtmPos , miWRK_ZBtm);
                 PM.SetParent(pnWRK_YBtmPos , miWRK_YBtm);
                 PM.SetParent(pnWRK_XBtmPos , miWRK_XBtm);
                 PM.SetParent(pnWRK_YCamPos , miWRK_YCam);

                 FraMotor[miWRK_YCam] -> Parent = pnMotrJog3  ;
        break;

        case 4 : PM.SetParent(pnSTL_YTopPos , miSTL_YTop);
                 PM.SetParent(pnRJT_XBtmPos , miREJ_XBtm);
        break;

        case 5 : PM.SetParent(pnULD_YCmpPos , miULD_YCmp);
                 PM.SetParent(pnULD_ZCmpPos , miULD_ZCmp);
        break;
        case 7 : FraMotor[miWRK_YCam] -> Parent = pnWRK_Visn  ;
        break;
    }

    switch ( pcDeviceSet -> ActivePageIndex ) {
         case 0 :                             break ;
         case 1 : rgUnitUser = rgLDRJogUnit ;
                  edUnitUser = edLDRJogUnit ; break ;
         case 2 : rgUnitUser = rgRALJogUnit ;
                  edUnitUser = edRALJogUnit ; break ;
         case 3 : rgUnitUser = rgWRKJogUnit ;
                  edUnitUser = edWRKJogUnit ; break ;
         case 4 : rgUnitUser = rgSTLJogUnit ;
                  edUnitUser = edSTLJogUnit ; break ;
         case 5 : rgUnitUser = rgULDJogUnit ;
                  edUnitUser = edULDJogUnit ; break ;
         case 6 : rgUnitUser = rgULDJogUnit ;
                  edUnitUser = edULDJogUnit ; break ;
     }

    if(rgUnitUser->ItemIndex == 0) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utJog ,0   );
    if(rgUnitUser->ItemIndex == 1) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,1   );
    if(rgUnitUser->ItemIndex == 2) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.5 );
    if(rgUnitUser->ItemIndex == 3) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.1 );
    if(rgUnitUser->ItemIndex == 4) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.05);
    if(rgUnitUser->ItemIndex == 5) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0));
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btStartTestClick(TObject *Sender)
{
//    g_bPb1Rslt = true ;
//   g_bPb2Rslt = true ;
//    tmProbeTest -> Enabled = true ;
    AnsiString sTemp , sMsg;
    sMsg = "TEST,";

    sTemp.sprintf("%02d,",edPinNumber -> Text.ToIntDef(0));

    sMsg += sTemp ;

    sTemp.sprintf("%04d#",edProbeTime -> Text.ToIntDef(0));

    sMsg += sTemp ;

    RS232C_Pb1ch->SendData(sMsg.Length(),sMsg.c_str());
    RS232C_Pb2ch->SendData(sMsg.Length(),sMsg.c_str());

    
//    m_tmProbeTest.Clear();
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::mmProbeTestDblClick(TObject *Sender)
{
    mmProbeTest->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btToolChangeClick(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btResetRptClick(TObject *Sender)
{
    //SEQ.Reset();
    CWorkZone::SStat Stat = WRK.GetStat(); Stat.bRepeat = false ; WRK.SetStat(Stat);
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btLDR_InClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;
    bool bState =  IO_GetY(iSel);
    IO_SetY(iSel , !bState);
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btInspStartClick(TObject *Sender)
{
    int iSel = ((TButton *)Sender) -> Tag ;
//    AnsiString sMsg, sTemp;

    //SET,12,0400,0100,15,T#
    AnsiString sMsg  = "" ;
    AnsiString sTemp ;
    int i1ChLedCur, i2ChLedCur , i1ChZenCur, i2ChZenCur ;
    i1ChLedCur = (OM.DevOptn.i1ChLedCur * 1000 ) / 50 ;
    i1ChZenCur = (OM.DevOptn.i1ChZenCur * 1000 ) / 50 ;

    i2ChLedCur = (OM.DevOptn.i2ChLedCur * 1000 ) / 50 ;
    i2ChZenCur = (OM.DevOptn.i2ChZenCur * 1000 ) / 50 ;

    switch(iSel) {
        case 0 : RS232C_Pb1ch->Open(0);
                 if(OM.MstOptn.bUsed4RowCntct)RS232C_Pb2ch->Open(1);
                 break;
        case 1 : RS232C_Pb1ch->Close();
                 if(OM.MstOptn.bUsed4RowCntct)RS232C_Pb2ch->Close();
                 break;
        case 2 : PCM.SetDaegyeom();
                 //if(OM.MstOptn.bUseDaegyeomProbe) PCM.SetDaegyeom();
                 //else                             FrmMain-> tmMainSet -> Enabled = true ;
                 break;
        case 3 : PCM.Start(1);
                 if(OM.MstOptn.bUsed4RowCntct)PCM.Start(2);
                 break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btStopTestClick(TObject *Sender)
{
    AnsiString sTemp , sMsg;
    sMsg = "STOP#";

    RS232C_Pb1ch->SendData(sMsg.Length(),sMsg.c_str());
    RS232C_Pb2ch->SendData(sMsg.Length(),sMsg.c_str());

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btPSBAutoCALClick(TObject *Sender)
{
    double dReGrabPs ;
    int    iReGrabCol;

    iReGrabCol = edPSBReGripCol -> Text.ToIntDef(0) ;
    dReGrabPs  = iReGrabCol * (OM.DevInfo.dColPitch + OM.DevOptn.dTrimPtchOfs)  ;

    edPSBReGripOfs -> Text = dReGrabPs ;

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::brContactCntResetClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","Do you really want to Reset?") != mrYes) return ;
    edContactPinCnt -> Text = 0 ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btSTL_YPchFwClick(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::rgLDRJogUnitClick(TObject *Sender)
{
    if(rgUnitUser->ItemIndex == 0) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utJog ,0   );
    if(rgUnitUser->ItemIndex == 1) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,1   );
    if(rgUnitUser->ItemIndex == 2) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.5 );
    if(rgUnitUser->ItemIndex == 3) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.1 );
    if(rgUnitUser->ItemIndex == 4) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.05);
    if(rgUnitUser->ItemIndex == 5) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0));
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::ShowSet()
{
    switch ( pcDeviceSet -> ActivePageIndex )
    {
        case 0 :

        break;

        case 1 : PM.SetParent(pnLDR_YCmpPos , miLDR_YCmp);
                 PM.SetParent(pnLDR_ZCmpPos , miLDR_ZCmp);

        break;

        case 2 : PM.SetParent(pnPRB_XIdxPos , miPRB_XIdx);
                 PM.SetParent(pnPSB_XIdxPos , miPSB_XIdx);
        break;

        case 3 : PM.SetParent(pnWRK_ZTopPos , miWRK_ZTop);
                 PM.SetParent(pnWRK_ZBtmPos , miWRK_ZBtm);
                 PM.SetParent(pnWRK_YBtmPos , miWRK_YBtm);
                 PM.SetParent(pnWRK_XBtmPos , miWRK_XBtm);
                 PM.SetParent(pnWRK_XBtmPos , miWRK_YCam);
        break;

        case 4 : PM.SetParent(pnSTL_YTopPos , miSTL_YTop);
                 PM.SetParent(pnRJT_XBtmPos , miREJ_XBtm);
        break;

        case 5 : PM.SetParent(pnULD_YCmpPos , miULD_YCmp);
                 PM.SetParent(pnULD_ZCmpPos , miULD_ZCmp);


        break;
    }
}
//-------------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btLDR_CmpClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;
    bool bState =  AT_GetCmd(iSel);

    bool bRet ;

    if(iSel == aiLDR_Cmp )  bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiLDR_Psh )  bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiPRB_Rlr )  bRet = PRB.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiPRB_Lft )  bRet = PRB.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiPRB_Rgt )  bRet = PRB.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiWRK_Idx )  bRet = WRK.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiPSB_Lft )  bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiPSB_Rgt )  bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiSTL_Top )  bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiSTL_Btm )  bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiPSB_Hld )  bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiPSB_Rolr)  bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiULD_Cmp )  bRet = ULD.CheckSafe((EN_ACTR_ID)iSel , !bState) ;



    if(iSel == aiLDR_Cmp      ) {
        if(bState){
            if(IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Sure ?") != mrYes ) return ;
            bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , bState) ;
        }
        else { bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , !bState);}
    }
    if(iSel == aiULD_Cmp      ) {
        if(bState) {
            if(IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Sure ?") != mrYes ) return ;
            bRet = ULD.CheckSafe((EN_ACTR_ID)iSel , bState) ;
        }
        else { bRet = ULD.CheckSafe((EN_ACTR_ID)iSel , !bState) ;}
    }


    if(!bRet) return ;

    AT_MoveCyl(iSel , !bState) ;        
}
//---------------------------------------------------------------------------





void __fastcall TFrmDeviceSet::FormCreate(TObject *Sender)
{
    TPanel * pMotrPos ;
    TPanel * pMotrJog ;

    AnsiString sMotrPanel;
    for(int i = 0 ; i < MAX_MOTR ; i++) {
        //모터포지션창.
        sMotrPanel = "pnMotrPos" + AnsiString(i) ;
        pMotrPos = ((TPanel*)FindComponent(sMotrPanel)) ;
        if(pMotrPos != NULL) PM.SetWindow(pMotrPos , i);

        //모터
        sMotrPanel = "pnMotrJog" + AnsiString(i) ;
        pMotrJog = ((TPanel*)FindComponent(sMotrPanel)) ;
        if(pMotrJog != NULL) {
            FraMotor[i] = new TFraMotr(pMotrJog);
            FraMotor[i] -> SetIdType((EN_MOTR_ID)i ,MT_GetDirType(i));
            FraMotor[i] -> Parent = pMotrJog  ;
            FraMotor[i] -> Show();
            FraMotor[i] -> SetUnit(utJog ,0 );
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    for(int i = 0 ; i < MAX_MOTR ; i++) {
        delete FraMotor[i];
    }
    for(int i = 0 ; i < MAX_ACTR ; i++) {
        delete FraCylinder[i];
    }

    delete foLDRAcIn  ;
    delete foLDRAcOut ;
    delete foPREAc    ;
    delete foPSBAc    ;
    delete foPSBBlwr  ;
    delete foULDACIn  ;
    delete foULDAcOut ;


}
//---------------------------------------------------------------------------



void __fastcall TFrmDeviceSet::Button1Click(TObject *Sender)
{
    PCM.GetDataCh(riWRK , PCM.sPb1Msg , PROBE_CH1);        
}
//---------------------------------------------------------------------------

