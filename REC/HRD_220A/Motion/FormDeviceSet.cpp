//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDeviceSet.h"
#include "FormDispCh.h"

//---------------------------------------------------------------------------
//Part Reference.
//---------------------------------------------------------------------------
#include "Loader.h"
#include "Index.h"
#include "Picker.h"
#include "Stage.h"
#include "Dispensor.h"
#include "Unloader.h"
//---------------------------------------------------------------------------
//Rs232 Part.
//---------------------------------------------------------------------------
#include "Rs232Dispr.h"
#include "Rs232Keyence.h"
#include "Rs232LoadCl.h"
#include "Rs232Temp.h"
//---------------------------------------------------------------------------
#include "Pattern.h"
//---------------------------------------------------------------------------
#include "Sequence.h"
#include "SPCUnit.h"

//---------------------------------------------------------------------------
#include "LotUnit.h"
#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "ManualMan.h"

#include "Rs232Temp.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
__fastcall TFrmDeviceSet::TFrmDeviceSet(TComponent* Owner)
    : TForm(Owner)
{

}

void __fastcall TFrmDeviceSet::FormCreate(TObject *Sender)
{
    TPanel * pMotrPos ;
    TPanel * pMotrJog ;

    AnsiString sMotrPanel ;

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
            FraMotor[i] -> SetIdType(i ,MT_GetDirType(i));
            FraMotor[i] -> Parent = pMotrJog  ;
            FraMotor[i] -> Show();
            FraMotor[i] -> SetUnit(utJog ,0 );
        }
    }

    //실린더 창.
    TPanel * pActr ;

     for(int i=0 ; i<MAX_ACTR ; i++) {
        pActr = ((TPanel*)FindComponent("pnAtcr" + AnsiString(i))) ;
        FraCylinder[i] = new TFraCylOneBt(pActr);

        switch(i){
            default :                                                                                                           break;
            case aiPCK_XAlgnFwBw : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , PCK  .CheckSafe ); break;  //aiPCK_XAlgnFwBw
            case aiPCK_YAlgnFwBw : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , PCK  .CheckSafe ); break;  //aiPCK_YAlgnFwBw
            case aiDSP_NdlCrFwBw : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , DSP  .CheckSafe ); break;  //aiDSP_NdlCrFwBw
            case aiIDX_FIndxUpDn : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , IDX_F.CheckSafe ); break;  //aiIDX_FIndxUpDn
            case aiIDX_RIndxUpDn : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , IDX_R.CheckSafe ); break;  //aiIDX_RIndxUpDn
            case aiPRI_FIndxUpDn : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , PRI_F.CheckSafe ); break;  //aiPRI_FIndxUpDn
            case aiPRI_RIndxUpDn : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , PRI_R.CheckSafe ); break;  //aiPRI_RIndxUpDn
            case aiULD_FPushLtRt : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , ULD_F.CheckSafe ); break;  //aiULD_FPushLtRt
            case aiULD_RPushLtRt : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , ULD_R.CheckSafe ); break;  //aiULD_RPushLtRt
            case aiULD_FPushUpDn : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , ULD_F.CheckSafe ); break;  //aiULD_FPushUpDn
            case aiULD_RPushUpDn : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , ULD_R.CheckSafe ); break;  //aiULD_RPushUpDn
            case aiDSP_NdlCvFwBw : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , DSP  .CheckSafe ); break;  //aiULD_RPushUpDn
        }

        FraCylinder[i] ->Show();
    }

    foIonizerFrnot = new TFraOutput(); foIonizerFrnot -> SetConfig(yIDX_FIonizer  , IO_GetYName(yIDX_FIonizer  ), pnIO0 );
    foIonizerRear  = new TFraOutput(); foIonizerRear  -> SetConfig(yIDX_RIonizer  , IO_GetYName(yIDX_RIonizer  ), pnIO1 );
    foIdxIonFront  = new TFraOutput(); foIdxIonFront  -> SetConfig(yIDX_IonVccm   , IO_GetYName(yIDX_IonVccm   ), pnIO2 );
    foIdxIonRear   = new TFraOutput(); foIdxIonRear   -> SetConfig(yIDX_IonVccm   , IO_GetYName(yIDX_IonVccm   ), pnIO3 );
    foSTGVccm      = new TFraOutput(); foSTGVccm      -> SetConfig(ySTG_Vccm      , IO_GetYName(ySTG_Vccm      ), pnIO4 );
    foSTGEjct      = new TFraOutput(); foSTGEjct      -> SetConfig(ySTG_Ejct      , IO_GetYName(ySTG_Ejct      ), pnIO5 );
    foPCKVccmOn    = new TFraOutput(); foPCKVccmOn    -> SetConfig(yPCK_VccmOn    , IO_GetYName(yPCK_VccmOn    ), pnIO6 );
    foPCKEjct      = new TFraOutput(); foPCKEjct      -> SetConfig(yPCK_Ejct      , IO_GetYName(yPCK_Ejct      ), pnIO7 );
    foPCKVccmOff   = new TFraOutput(); foPCKVccmOff   -> SetConfig(yPCK_VccmOff   , IO_GetYName(yPCK_VccmOff   ), pnIO8 );

    //Pattern StringGrid .
    sgPatternPos -> ColCount         = 6   ;
    sgPatternPos -> DefaultRowHeight = 22  ;
    sgPatternPos -> DefaultColWidth  = 80  ;
    sgPatternPos -> ColWidths[0]     = sgPatternPos -> DefaultColWidth ;
    sgPatternPos -> ColWidths[1]     = sgPatternPos -> DefaultColWidth ;
    sgPatternPos -> ColWidths[2]     = sgPatternPos -> DefaultColWidth ;
    sgPatternPos -> ColWidths[3]     = sgPatternPos -> DefaultColWidth ;
    sgPatternPos -> ColWidths[4]     = sgPatternPos -> DefaultColWidth ;
    sgPatternPos -> FixedRows        = 1 ;
    sgPatternPos -> FixedCols        = 1 ;
    sgPatternPos -> Options          >> goRangeSelect ;
    sgPatternPos -> ScrollBars       = ssVertical     ;
    //sgPatternPos -> Align            = alClient       ;
    sgPatternPos -> Font -> Charset  = ARABIC_CHARSET ;
    sgPatternPos -> Font -> Size     = 11             ;
    sgPatternPos -> RowCount         = MAX_PATTERN_POS + 1 ;
    sgPatternPos -> Visible          = true ;
    sgPatternPos -> Cells[1][0]      = "X Pos"   ;
    sgPatternPos -> Cells[2][0]      = "Y Pos"   ;
    sgPatternPos -> Cells[3][0]      = "X Scale" ;
    sgPatternPos -> Cells[4][0]      = "Y Scale" ;
    sgPatternPos -> Cells[5][0]      = "Disp"    ;

    for(int i = 1 ; i < MAX_PATTERN_POS + 1 ; i++) {
        AnsiString sNum = i;
        sgPatternPos -> Cells[0][i] = "Node" + sNum ;
    }

    const bool bKorean = true;

    pcDeviceSet      -> ActivePageIndex = 0 ;

    //Disp
    btDsprStdCal   -> Tag = (int)mcDSPCycleManStdCal;
    btStageVisnPos -> Tag = (int)mcDSPCycleVisnPos  ;

    //Picker
    btPckrStdCal   -> Tag = (int)mcPCKCycleManStdCal;
    btVisnCal      -> Tag = (int)mcVisnCal          ;
    btAlignerCal   -> Tag = (int)mcPCKCycleManAlignerCal ;

    btPckAlignPos  -> Tag = (int)mcPCKCycleManVsAlignPos  ;
    btPckDefectPos -> Tag = (int)mcPCKCycleManVsDefectPos ;


    //세이브버튼 누를때 , 디바이스셑 크리에트할때 , 디바이스 체인지 누를때...
    DSP.Rs232_Dispr.SetLoadCh(OM.DevOptn.iDspCh+1);
    CDelayTimer TimeOut;
    TimeOut.Clear();
    while(!DSP.Rs232_Dispr.GetMsgEnd()){ //메세지 다 주고 받을때까지 기다림.
        Sleep(1);
        if(TimeOut.OnDelay(true , 1000)){
            break ;
        }
    }

}

//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btSavePosClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;

    UpdateDevInfo(toBuff);
    OM.SaveDevInfo(OM.GetCrntDev());

    UpdateDevOptn(toBuff);
    OM.SaveDevOptn(OM.GetCrntDev());

    PM.UpdatePstn(toBuff);
    PM.Save(OM.GetCrntDev());

    // Device 관련
    // HIC
    if(edHicWidth   -> Text.ToDouble() > 50) edHicWidth   -> Text = 50 ;
    if(edHicHeight  -> Text.ToDouble() > 50) edHicHeight  -> Text = 50 ;
    if(edHicChamper -> Text.ToDouble() > 50) edHicChamper -> Text = 50 ;

    // DIE
    if(edDieWidth   -> Text.ToDouble() > 50) edDieWidth   -> Text = 50 ;
    if(edDieHeight  -> Text.ToDouble() > 50) edDieHeight  -> Text = 50 ;
    if(edDieChamper -> Text.ToDouble() > 50) edDieChamper -> Text = 50 ;

    // FOS
    if(edFosWidth   -> Text.ToDouble() > 50) edFosWidth   -> Text = 50 ;
    if(edFosHeight  -> Text.ToDouble() > 50) edFosHeight  -> Text = 50 ;
    if(edFosChamper -> Text.ToDouble() > 50) edFosChamper -> Text = 50 ;

    // 이곳에 Device에 따라서 SetDevPitch 되게... 필요...
    UpdatePattern (toBuff);
    PTT.Save(OM.GetCrntDev());
    PaintPattern();


    DM.ARAY[riLDR_FTop].SetMaxColRow(1                        , OM.DevInfo.iMgzSlotCnt ); DM.ARAY[riLDR_FTop].SetLotNo("");
    DM.ARAY[riLDR_FBtm].SetMaxColRow(1                        , OM.DevInfo.iMgzSlotCnt ); DM.ARAY[riLDR_FBtm].SetLotNo("");
    DM.ARAY[riLDR_RTop].SetMaxColRow(1                        , OM.DevInfo.iMgzSlotCnt ); DM.ARAY[riLDR_RTop].SetLotNo("");
    DM.ARAY[riLDR_RBtm].SetMaxColRow(1                        , OM.DevInfo.iMgzSlotCnt ); DM.ARAY[riLDR_RBtm].SetLotNo("");
    DM.ARAY[riPRI_F   ].SetMaxColRow(OM.DevInfo.iBoatColCnt   , 1                      ); DM.ARAY[riPRI_F   ].SetLotNo("");
    DM.ARAY[riPRI_R   ].SetMaxColRow(OM.DevInfo.iBoatColCnt   , 1                      ); DM.ARAY[riPRI_R   ].SetLotNo("");
    DM.ARAY[riIDX_F   ].SetMaxColRow(OM.DevInfo.iBoatColCnt   , 1                      ); DM.ARAY[riIDX_F   ].SetLotNo("");
    DM.ARAY[riIDX_R   ].SetMaxColRow(OM.DevInfo.iBoatColCnt   , 1                      ); DM.ARAY[riIDX_R   ].SetLotNo("");
    DM.ARAY[riPSI_F   ].SetMaxColRow(OM.DevInfo.iBoatColCnt   , 1                      ); DM.ARAY[riPSI_F   ].SetLotNo("");
    DM.ARAY[riPSI_R   ].SetMaxColRow(OM.DevInfo.iBoatColCnt   , 1                      ); DM.ARAY[riPSI_R   ].SetLotNo("");
    DM.ARAY[riULD_FTop].SetMaxColRow(1                        , OM.DevInfo.iMgzSlotCnt ); DM.ARAY[riULD_FTop].SetLotNo("");
    DM.ARAY[riULD_FBtm].SetMaxColRow(1                        , OM.DevInfo.iMgzSlotCnt ); DM.ARAY[riULD_FBtm].SetLotNo("");
    DM.ARAY[riULD_RTop].SetMaxColRow(1                        , OM.DevInfo.iMgzSlotCnt ); DM.ARAY[riULD_RTop].SetLotNo("");
    DM.ARAY[riULD_RBtm].SetMaxColRow(1                        , OM.DevInfo.iMgzSlotCnt ); DM.ARAY[riULD_RBtm].SetLotNo("");
    DM.ARAY[riPCK     ].SetMaxColRow(1                        , 1                      ); DM.ARAY[riPCK     ].SetLotNo("");
    DM.ARAY[riALN     ].SetMaxColRow(1                        , 1                      ); DM.ARAY[riALN     ].SetLotNo("");
    DM.ARAY[riDSP     ].SetMaxColRow(1                        , 1                      ); DM.ARAY[riDSP     ].SetLotNo("");
    DM.ARAY[riSTG     ].SetMaxColRow(1                        , 1                      ); DM.ARAY[riSTG     ].SetLotNo("");

    pnFOS -> Visible = OM.DevInfo.iWorkMode == wmFos ;
    pnFOS2-> Visible = OM.DevInfo.iWorkMode == wmFos ;
    pnFOS3-> Visible = OM.DevInfo.iWorkMode == wmFos ;

    Rs232_TempCh.SetTempAll(OM.DevOptn.iRailBtmTemp,
                            OM.DevOptn.iRailTopTemp,
                            OM.DevOptn.iStageTemp  );



    //세이브버튼 누를때 , 디바이스셑 크리에트할때 , 디바이스 체인지 누를때...
    DSP.Rs232_Dispr.SetLoadCh(OM.DevOptn.iDspCh+1);
    CDelayTimer TimeOut;
    TimeOut.Clear();
    while(!DSP.Rs232_Dispr.GetMsgEnd()){ //메세지 다 주고 받을때까지 기다림.
        Sleep(1);
        if(TimeOut.OnDelay(true , 1000)){
            break ;
        }
    }

    SetComboItem();
    

    //Rs232_TempCh.SetTemp(tcRailBtm , OM.DevOptn.iRailBtmTemp);
    //Rs232_TempCh.SetTemp(tcRailTop , OM.DevOptn.iRailTopTemp);
    //Rs232_TempCh.SetTemp(tcStage   , OM.DevOptn.iStageTemp  );
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::tmUpdateTimerTimer(TObject *Sender)
{
    tmUpdateTimer->Enabled = false;
    btSaveDevice -> Enabled = !SEQ._bRun ;
    switch ( pcDeviceSet -> ActivePageIndex ) {
        default:                break;
        case 0 :                break;
        case 1 :                break;
        case 2 :                break;
        case 3 :                break;
        case 4 :                break;
        case 5 :                break;
        case 6 :                break;
        case 7 :                break;
        case 8 :                break;
        case 9 :                break;
    }
    if(PCK.m_bRqstPMSave || DSP.m_bRqstPMSave) {
        Trace("Update","PM Save Start");
        PM.UpdatePstn(toTabl);
        PM.Save(OM.GetCrntDev());
        DSP.m_bRqstPMSave = false ;
        PCK.m_bRqstPMSave = false ;
        Trace("Update","PM Save End");
        //FM_MsgOk("Confirm", "위치 보정치를 적용 하였습니다.");
    }

    lbCrntNode -> Caption = DSP.m_iCrntDispNode ;
    lbCrntNode -> Color   = IO_GetY(yDSP_DispOn)? clLime : clBlack ;

    tsRear      -> TabVisible = FM_GetLevel() == lvMaster ;
    tsDispensor -> TabVisible = FM_GetLevel() == lvMaster ;
    tsStage     -> TabVisible = FM_GetLevel() == lvMaster ;
    tsFront     -> TabVisible = FM_GetLevel() == lvMaster ;

    btPckrStdCal -> Visible = FM_GetLevel() == lvMaster ;
    btAlignerCal -> Visible = FM_GetLevel() == lvMaster ;
    btVisnCal    -> Visible = FM_GetLevel() == lvMaster ;




    pnScreen -> Visible  = FM_GetLevel() != lvMaster  ;

    if(FM_GetLevel() != lvMaster && rgJogUnit -> ItemIndex == 5) {
        rgJogUnit -> ItemIndex = 4 ;
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
    if(bTo){
        cbWorkMode   -> ItemIndex = OM.DevInfo.iWorkMode;

        //die Width, Height, Champer 불러오기는 같이 해도 상관없다.
        edDieWidth      -> Text   = OM.DevInfo.dDieWidth    ;
        edDieHeight     -> Text   = OM.DevInfo.dDieHeight   ;
        edDieChamper    -> Text   = OM.DevInfo.dDieChamper  ;

        edHicWidth      -> Text = OM.DevInfo.dHicWidth      ;
        edHicHeight     -> Text = OM.DevInfo.dHicHeight     ;
        edHicChamper    -> Text = OM.DevInfo.dHicChamper    ;

        edFosWidth      -> Text = OM.DevInfo.dFosWidth      ;
        edFosHeight     -> Text = OM.DevInfo.dFosHeight     ;
        edFosChamper    -> Text = OM.DevInfo.dFosChamper    ;

        edBoatColPitch  -> Text = OM.DevInfo.dBoatColPitch  ;
        edBoatColCnt    -> Text = OM.DevInfo.iBoatColCnt    ;

        edMgzSlotPitch  -> Text = OM.DevInfo.dMgzSlotPitch  ;
        edMgzSlotCnt    -> Text = OM.DevInfo.iMgzSlotCnt    ;

        //PATTERN 영역은. PATTERN.h에서 저장 시킨다.
        //edScalePatternX -> Text = OM.DevInfo.dScalePatternX ;
        //edScalePatternY -> Text = OM.DevInfo.dScalePatternY ;
        //
        //edSttOffSetX    -> Text = OM.DevInfo.dSttOffSetX    ;
        //edSttOffSetY    -> Text = OM.DevInfo.dSttOffSetY    ;

    }
    else {
        OM.DevInfo.iWorkMode       =  cbWorkMode -> ItemIndex;
        OM.DevInfo.dDieWidth       =  StrToFloatDef(edDieWidth      -> Text,1.0) ;
        OM.DevInfo.dDieHeight      =  StrToFloatDef(edDieHeight     -> Text,1.0) ;
        OM.DevInfo.dDieChamper     =  StrToFloatDef(edDieChamper    -> Text,1.0) ;

        OM.DevInfo.dHicWidth       =  StrToFloatDef(edHicWidth      -> Text,1.0) ;
        OM.DevInfo.dHicHeight      =  StrToFloatDef(edHicHeight     -> Text,1.0) ;
        OM.DevInfo.dHicChamper     =  StrToFloatDef(edHicChamper    -> Text,1.0) ;

        OM.DevInfo.dFosWidth       =  StrToFloatDef(edFosWidth      -> Text,1.0) ;
        OM.DevInfo.dFosHeight      =  StrToFloatDef(edFosHeight     -> Text,1.0) ; 
        OM.DevInfo.dFosChamper     =  StrToFloatDef(edFosChamper    -> Text,1.0) ;

        OM.DevInfo.dBoatColPitch   =  StrToFloatDef(edBoatColPitch  -> Text,1.0) ;
        OM.DevInfo.iBoatColCnt     =  StrToIntDef  (edBoatColCnt    -> Text,1  ) ;

        OM.DevInfo.dMgzSlotPitch   =  StrToFloatDef(edMgzSlotPitch  -> Text,1.0) ;
        OM.DevInfo.iMgzSlotCnt     =  StrToIntDef  (edMgzSlotCnt    -> Text,1  ) ;

        //PATTERN 영역은. PATTERN.h에서 저장 시킨다.
        //OM.DevInfo.dScalePatternX  =  StrToFloatDef(edScalePatternX -> Text,1.0) ;
        //OM.DevInfo.dScalePatternY  =  StrToFloatDef(edScalePatternY -> Text,1.0) ;
        //
        //OM.DevInfo.dSttOffSetX     =  StrToFloatDef(edSttOffSetX    -> Text,1.0) ;
        //OM.DevInfo.dSttOffSetY     =  StrToFloatDef(edSttOffSetY    -> Text,1.0) ;

    }

}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::UpdateDevOptn(bool bTo)
{

    if(bTo){
        cbPkIgnrAlign        -> Checked = OM.DevOptn.bPkIgnrAlign      ;
        cbPkIgnrVisnAlign    -> Checked = OM.DevOptn.bPkIgnrVisnAlign  ;
        cbPkIgnrVisnDefect   -> Checked = OM.DevOptn.bPkIgnrVisnDefect ;
        cbStIgnrVisnBefore   -> Checked = OM.DevOptn.bStIgnrVisnBefore ;
        cbStIgnrVisnAfter    -> Checked = OM.DevOptn.bStIgnrVisnAfter  ;

        edPkPickDelay        -> Text    = OM.DevOptn.iPkPickDelay/1000.0;
        edFrstPkgDelay       -> Text    = OM.DevOptn.iFrstPkgDelay/1000.0;
        edPlaceYOfs          -> Text    = OM.DevOptn.dPlaceYOfs        ;
        edPlaceXOfs          -> Text    = OM.DevOptn.dPlaceXOfs        ;
        edStageTolXY         -> Text    = OM.DevOptn.dStageTolXY       ;
        edStageTolAng        -> Text    = OM.DevOptn.dStageTolAng      ;
        edPickerTolXY        -> Text    = OM.DevOptn.dPickerTolXY      ;
        edPickerTolAng       -> Text    = OM.DevOptn.dPickerTolAng     ;

        edPlaceTolXY         -> Text    = OM.DevOptn.dPlaceTolXY       ;
        edPlaceTolAng        -> Text    = OM.DevOptn.dPlaceTolAng      ;
        edFosPlaceSttYOfs    -> Text    = OM.DevOptn.dFosPlaceSttYOfs  ;
        edFosPlaceSttYSpd    -> Text    = OM.DevOptn.dFosPlaceSttYSpd  ;
        edFosPlaceBfDelay    -> Text    = OM.DevOptn.iFosPlaceBfDelay/1000.0  ;

        edPlaceZOfs1         -> Text    = OM.DevOptn.dPlaceZOfs1       ;
        edPlaceZOfs2         -> Text    = OM.DevOptn.dPlaceZOfs2       ;
        edAttachSpeed1       -> Text    = OM.DevOptn.dAttachSpeed1     ;
        edAttachSpeed2       -> Text    = OM.DevOptn.dAttachSpeed2     ;
        edAttachForce        -> Text    = OM.DevOptn.dAttachForce      ;
        edAttachForceOfs     -> Text    = OM.DevOptn.dAttachForceOfs   ;

        edDispZOfs           -> Text    = OM.DevOptn.dDispZOfs         ;
        edPkPushDelay        -> Text    = OM.DevOptn.iPkPushDelay /1000.0 ;

        edAttachStageOfsX    -> Text    = OM.DevOptn.dAttachStageOfsX  ;
        edAttachStageOfsY    -> Text    = OM.DevOptn.dAttachStageOfsY  ;
        edAttachStageOfsT    -> Text    = OM.DevOptn.dAttachStageOfsT  ;

        edRailTopTemp        -> Text    = OM.DevOptn.iRailTopTemp      ;
        edRailBtmTemp        -> Text    = OM.DevOptn.iRailBtmTemp      ;
        edStageTemp          -> Text    = OM.DevOptn.iStageTemp        ;

        cbDspCh              -> ItemIndex = OM.DevOptn.iDspCh          ;
        edDspMinAmount       -> Text    = OM.DevOptn.iDspMinAmount     ;

        edReptSttXPos        -> Text    = OM.DevOptn.dReptSttXPos      ;
        edReptSttYPos        -> Text    = OM.DevOptn.dReptSttYPos      ;
        edReptEndXPos        -> Text    = OM.DevOptn.dReptEndXPos      ;
        edReptEndYPos        -> Text    = OM.DevOptn.dReptEndYPos      ;
        edReptCnt            -> Text    = OM.DevOptn.iReptCnt          ;
        edReptSpd            -> Text    = OM.DevOptn.dReptSpd          ;

        edEpoxyName          -> Text    = OM.DevOptn.sEpoxyName        ;

    }
    else{
        OM.DevOptn.bPkIgnrAlign      = cbPkIgnrAlign        -> Checked;
        OM.DevOptn.bPkIgnrVisnAlign  = cbPkIgnrVisnAlign    -> Checked;
        OM.DevOptn.bPkIgnrVisnDefect = cbPkIgnrVisnDefect   -> Checked;
        OM.DevOptn.bStIgnrVisnBefore = cbStIgnrVisnBefore   -> Checked;
        OM.DevOptn.bStIgnrVisnAfter  = cbStIgnrVisnAfter    -> Checked;

        OM.DevOptn.iPkPickDelay      = StrToFloatDef(edPkPickDelay      -> Text , OM.DevOptn.iPkPickDelay /1000.0)*1000 ;
        OM.DevOptn.iFrstPkgDelay     = StrToFloatDef(edFrstPkgDelay     -> Text , OM.DevOptn.iFrstPkgDelay/1000.0)*1000 ;
        OM.DevOptn.dPlaceYOfs        = StrToFloatDef(edPlaceYOfs        -> Text , OM.DevOptn.dPlaceYOfs      ) ;

        OM.DevOptn.dPlaceXOfs        = StrToFloatDef(edPlaceXOfs        -> Text , OM.DevOptn.dPlaceXOfs      ) ;
        OM.DevOptn.dStageTolXY       = StrToFloatDef(edStageTolXY       -> Text , OM.DevOptn.dStageTolXY     ) ;
        OM.DevOptn.dStageTolAng      = StrToFloatDef(edStageTolAng      -> Text , OM.DevOptn.dStageTolAng    ) ;
        OM.DevOptn.dPickerTolXY      = StrToFloatDef(edPickerTolXY      -> Text , OM.DevOptn.dPickerTolXY    ) ;
        OM.DevOptn.dPickerTolAng     = StrToFloatDef(edPickerTolAng     -> Text , OM.DevOptn.dPickerTolAng   ) ;

        OM.DevOptn.dPlaceTolXY       = StrToFloatDef(edPlaceTolXY       -> Text , OM.DevOptn.dPlaceTolXY     ) ;
        OM.DevOptn.dPlaceTolAng      = StrToFloatDef(edPlaceTolAng      -> Text , OM.DevOptn.dPlaceTolAng    ) ;

        OM.DevOptn.dFosPlaceSttYOfs  = StrToFloatDef(edFosPlaceSttYOfs  -> Text , OM.DevOptn.dFosPlaceSttYOfs) ;
        OM.DevOptn.dFosPlaceSttYSpd  = StrToFloatDef(edFosPlaceSttYSpd  -> Text , OM.DevOptn.dFosPlaceSttYSpd) ;
        OM.DevOptn.iFosPlaceBfDelay  = StrToFloatDef(edFosPlaceBfDelay  -> Text , OM.DevOptn.iFosPlaceBfDelay/1000.0)*1000 ;


        OM.DevOptn.dPlaceZOfs1       = StrToFloatDef(edPlaceZOfs1       -> Text , OM.DevOptn.dPlaceZOfs1     ) ;
        OM.DevOptn.dPlaceZOfs2       = StrToFloatDef(edPlaceZOfs2       -> Text , OM.DevOptn.dPlaceZOfs2     ) ;
        if(OM.DevOptn.dPlaceZOfs1 > OM.CmnOptn.dMaxPlaceZOfs1) {
            OM.DevOptn.dPlaceZOfs1 = OM.CmnOptn.dMaxPlaceZOfs1 ;
            FM_MsgOk("Limit Over" , String("Place Z Offset1의 최대값은" + String(OM.CmnOptn.dMaxPlaceZOfs1) + "입니다.").c_str());
        }
        if(OM.DevOptn.dPlaceZOfs1 < 0) {
            OM.DevOptn.dPlaceZOfs1 = 0 ;
            FM_MsgOk("Limit Over" , String("Place Z Offset1의 최소값은" + String(0) + "입니다.").c_str());
        }

        if(OM.DevOptn.dPlaceZOfs2 > OM.CmnOptn.dMaxPlaceZOfs2) {
            OM.DevOptn.dPlaceZOfs2 = OM.CmnOptn.dMaxPlaceZOfs2 ;
            FM_MsgOk("Limit Over" , String("Place Z Offset2의 최대값은" + String(OM.CmnOptn.dMaxPlaceZOfs2) + "입니다.").c_str());
        }
        if(OM.DevOptn.dPlaceZOfs2 < 0) {
            OM.DevOptn.dPlaceZOfs2 = 0 ;
            FM_MsgOk("Limit Over" , String("Place Z Offset2의 최소값은" + String(0) + "입니다.").c_str());
        }

        OM.DevOptn.dAttachSpeed1     = StrToFloatDef(edAttachSpeed1     -> Text , OM.DevOptn.dAttachSpeed1   ) ;
        OM.DevOptn.dAttachSpeed2     = StrToFloatDef(edAttachSpeed2     -> Text , OM.DevOptn.dAttachSpeed2   ) ;
        OM.DevOptn.dAttachForce      = StrToFloatDef(edAttachForce      -> Text , OM.DevOptn.dAttachForce    ) ;
        if(OM.DevOptn.dAttachForce > OM.CmnOptn.dMaxAttachForce) {
            OM.DevOptn.dAttachForce = OM.CmnOptn.dMaxAttachForce ;
            FM_MsgOk("Limit Over" , String("Attach Force의 최대값은" + String(OM.CmnOptn.dMaxAttachForce) + "입니다.").c_str());
        }
        if(OM.DevOptn.dAttachForce < 0) {
            OM.DevOptn.dAttachForce = 0 ;
            FM_MsgOk("Limit Over" , String("Attach Force의 최소값은" + String(0) + "입니다.").c_str());
        }

        OM.DevOptn.dAttachForceOfs   = StrToFloatDef(edAttachForceOfs   -> Text , OM.DevOptn.dAttachForceOfs ) ;

        OM.DevOptn.dDispZOfs         = StrToFloatDef(edDispZOfs         -> Text , OM.DevOptn.dDispZOfs       ) ;
        if(OM.DevOptn.dDispZOfs < 0) OM.DevOptn.dDispZOfs = 0 ;

        OM.DevOptn.iPkPushDelay      = StrToFloatDef(edPkPushDelay      -> Text , OM.DevOptn.iPkPushDelay/1000.0)*1000 ;

        OM.DevOptn.dAttachStageOfsX  = StrToFloatDef(edAttachStageOfsX  -> Text , OM.DevOptn.dAttachStageOfsX) ;
        OM.DevOptn.dAttachStageOfsY  = StrToFloatDef(edAttachStageOfsY  -> Text , OM.DevOptn.dAttachStageOfsY) ;
        OM.DevOptn.dAttachStageOfsT  = StrToFloatDef(edAttachStageOfsT  -> Text , OM.DevOptn.dAttachStageOfsT) ;

        //300 미크론 이상 넣으면 뭔가 문제가 있다.
        //평탄도 우선 살펴 볼것.
        if(OM.DevOptn.dAttachStageOfsX > 1 ) OM.DevOptn.dAttachStageOfsX = 1 ;
        if(OM.DevOptn.dAttachStageOfsX <-1 ) OM.DevOptn.dAttachStageOfsX =-1 ;
        if(OM.DevOptn.dAttachStageOfsY > 1 ) OM.DevOptn.dAttachStageOfsY = 1 ;
        if(OM.DevOptn.dAttachStageOfsY <-1 ) OM.DevOptn.dAttachStageOfsY =-1 ;
        if(OM.DevOptn.dAttachStageOfsT > 1 ) OM.DevOptn.dAttachStageOfsT = 1 ;
        if(OM.DevOptn.dAttachStageOfsT <-1 ) OM.DevOptn.dAttachStageOfsT =-1 ;



        OM.DevOptn.iRailTopTemp      = StrToIntDef  (edRailTopTemp      -> Text , OM.DevOptn.iRailTopTemp    ) ;
        OM.DevOptn.iRailBtmTemp      = StrToIntDef  (edRailBtmTemp      -> Text , OM.DevOptn.iRailBtmTemp    ) ;
        OM.DevOptn.iStageTemp        = StrToIntDef  (edStageTemp        -> Text , OM.DevOptn.iStageTemp      ) ;




        OM.DevOptn.iDspCh            = cbDspCh            -> ItemIndex ;
        OM.DevOptn.iDspMinAmount     = StrToIntDef  (edDspMinAmount     -> Text , OM.DevOptn.iDspMinAmount   );
        if(OM.DevOptn.iDspMinAmount < 0 ) OM.DevOptn.iDspMinAmount = 0  ;
        if(OM.DevOptn.iDspMinAmount > 99) OM.DevOptn.iDspMinAmount = 99 ;


        OM.DevOptn.dReptSttXPos      = StrToFloatDef  (edReptSttXPos      -> Text , OM.DevOptn.dReptSttXPos    ) ;
        OM.DevOptn.dReptSttYPos      = StrToFloatDef  (edReptSttYPos      -> Text , OM.DevOptn.dReptSttYPos    ) ;
        OM.DevOptn.dReptEndXPos      = StrToFloatDef  (edReptEndXPos      -> Text , OM.DevOptn.dReptEndXPos    ) ;
        OM.DevOptn.dReptEndYPos      = StrToFloatDef  (edReptEndYPos      -> Text , OM.DevOptn.dReptEndYPos    ) ;
        OM.DevOptn.iReptCnt          = StrToFloatDef  (edReptCnt          -> Text , OM.DevOptn.iReptCnt        ) ;
        OM.DevOptn.dReptSpd          = StrToFloatDef  (edReptSpd          -> Text , OM.DevOptn.dReptSpd        ) ;

        if(edEpoxyName -> Text == "") edEpoxyName -> Text = "Epoxy";
        OM.DevOptn.sEpoxyName       = edEpoxyName -> Text;
        UpdateDevOptn(toTabl);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdatePattern(bool bTo)
{
    if(bTo){
        edScalePatternX -> Text = PTT.GetScaleX() * 100;
        edScalePatternY -> Text = PTT.GetScaleY() * 100;

        edSttOffSetX    -> Text = PTT.GetSttOffsetX() ;
        edSttOffSetY    -> Text = PTT.GetSttOffsetY() ;

        edDisprSpeed    -> Text = PTT.GetSpeed() ;

        edPosCnt        -> Text = PTT.GetPosCnt() ;

        edDisprAcc      -> Text = PTT.GetAcc();
        edDisprDec      -> Text = PTT.GetDec();

        sgPatternPos -> RowCount = PTT.GetPosCnt()+1;

        const int iPosCnt = PTT.GetPosCnt();
        for(int i = 0 ; i < iPosCnt ; i++) {
            sgPatternPos -> Cells[1][i+1] = PTT.GetPosX(i);
            sgPatternPos -> Cells[2][i+1] = PTT.GetPosY(i);
            sgPatternPos -> Cells[3][i+1] = PTT.GetScalePosX(i);
            sgPatternPos -> Cells[4][i+1] = PTT.GetScalePosY(i);
            sgPatternPos -> Cells[5][i+1] = PTT.GetDispOn(i)?"1":"0";
        }
        
    }
    else{
        PTT.SetScale    (StrToFloatDef(edScalePatternX -> Text * 0.01 , PTT.GetScaleX()) , StrToFloatDef(edScalePatternY -> Text * 0.01 , PTT.GetScaleY())) ;
        PTT.SetSttOffset(StrToFloatDef(edSttOffSetX -> Text , PTT.GetSttOffsetX()) , StrToFloatDef(edSttOffSetY -> Text , PTT.GetSttOffsetY()));
        PTT.SetSpeed    (StrToFloatDef(edDisprSpeed -> Text , PTT.GetSpeed()));

        PTT.SetPosCnt   (StrToIntDef(edPosCnt -> Text , PTT.GetPosCnt()));
        if(PTT.GetPosCnt() > MAX_PATTERN_POS) PTT.SetPosCnt(MAX_PATTERN_POS);
        if(PTT.GetPosCnt() < 1              ) PTT.SetPosCnt(1              );

        PTT.SetAccDec   (StrToIntDef(edDisprAcc -> Text , PTT.GetAcc()), StrToIntDef(edDisprDec -> Text , PTT.GetDec()));

        sgPatternPos -> RowCount = PTT.GetPosCnt()+1;

        const int iPosCnt = PTT.GetPosCnt();
        for(int i = 0 ; i < iPosCnt ; i++) {
            PTT.SetPosX(i, StrToFloatDef(sgPatternPos -> Cells[1][i+1],PTT.GetPosX(i)));
            PTT.SetPosY(i, StrToFloatDef(sgPatternPos -> Cells[2][i+1],PTT.GetPosY(i)));
            PTT.SetDispOn(i, sgPatternPos -> Cells[5][i+1] != "0");
        }

        UpdatePattern(toTabl);
    }

}

void TFrmDeviceSet::SetComboItem()
{
    //Front================================
    //Loader
    int iIndex = cbLDR_FSlot -> ItemIndex ;
    cbLDR_FSlot -> Items -> Clear() ;
    for(int r = 0 ; r < OM.DevInfo.iMgzSlotCnt; r++){ //메거진 버텀.
        cbLDR_FSlot -> Items -> Add(String("Bottom")+(r+1));
    }
    for(int r = 0 ; r < OM.DevInfo.iMgzSlotCnt; r++){ //메거진
        cbLDR_FSlot -> Items -> Add(String("Top")+(r+1));
    }
    cbLDR_FSlot -> ItemIndex = iIndex ;


    //Unloader
    iIndex = cbULD_FSlot -> ItemIndex ;
    cbULD_FSlot -> Items -> Clear() ;
    for(int r = 0 ; r < OM.DevInfo.iMgzSlotCnt; r++){ //메거진 버텀.
        cbULD_FSlot -> Items -> Add(String("Bottom")+(r+1));
    }
    for(int r = 0 ; r < OM.DevInfo.iMgzSlotCnt; r++){ //메거진
        cbULD_FSlot -> Items -> Add(String("Top")+(r+1));
    }
    cbULD_FSlot -> ItemIndex = iIndex ;

    //Pitch.
    iIndex = cbIDX_FPitch -> ItemIndex ;
    cbIDX_FPitch -> Items -> Clear() ;
    for(int c = 0 ; c < OM.DevInfo.iBoatColCnt; c++){ //메거진 버텀.
        cbIDX_FPitch -> Items -> Add(String("Pocket")+(c+1));
    }
    cbIDX_FPitch -> ItemIndex = iIndex ;
//=======================================
//Rear================================
    //Loader
    iIndex = cbLDR_RSlot -> ItemIndex ;
    cbLDR_RSlot -> Items -> Clear() ;
    for(int r = 0 ; r < OM.DevInfo.iMgzSlotCnt; r++){ //메거진 버텀.
        cbLDR_RSlot -> Items -> Add(String("Bottom")+(r+1));
    }
    for(int r = 0 ; r < OM.DevInfo.iMgzSlotCnt; r++){ //메거진
        cbLDR_RSlot -> Items -> Add(String("Top")+(r+1));
    }
    cbLDR_RSlot -> ItemIndex = iIndex ;


    //Unloader
    iIndex = cbULD_RSlot -> ItemIndex ;
    cbULD_RSlot -> Items -> Clear() ;
    for(int r = 0 ; r < OM.DevInfo.iMgzSlotCnt; r++){ //메거진 버텀.
        cbULD_RSlot -> Items -> Add(String("Bottom")+(r+1));
    }
    for(int r = 0 ; r < OM.DevInfo.iMgzSlotCnt; r++){ //메거진
        cbULD_RSlot -> Items -> Add(String("Top")+(r+1));
    }
    cbULD_RSlot -> ItemIndex = iIndex ;

    //Pitch.
    iIndex = cbIDX_RPitch -> ItemIndex ;
    cbIDX_RPitch -> Items -> Clear() ;
    for(int c = 0 ; c < OM.DevInfo.iBoatColCnt; c++){ //메거진 버텀.
        cbIDX_RPitch -> Items -> Add(String("Pocket")+(c+1));
    }
    cbIDX_RPitch -> ItemIndex = iIndex ;
//=======================================


}

void __fastcall TFrmDeviceSet::FormShow(TObject *Sender)
{
    UpdateDevInfo (toTabl);
    UpdateDevOptn (toTabl);
    PM.UpdatePstn (toTabl);

    UpdatePattern (toTabl);
    PaintPattern();

    pnFOS -> Visible = OM.DevInfo.iWorkMode == wmFos ;
    pnFOS2-> Visible = OM.DevInfo.iWorkMode == wmFos ;
    pnFOS3-> Visible = OM.DevInfo.iWorkMode == wmFos ;

    edJogUnit -> Text = "0.01" ;

    SetComboItem();

    tmUpdateTimer  -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::pcDeviceSetChange(TObject *Sender)
{
    switch ( pcDeviceSet -> ActivePageIndex ) {
        default:                               break; //
        case 0 :                               break; //Device
        case 1 :                               break; //Optin
        case 2 :                               break; //pattern
        case 3 : pnJogUnit -> Parent = pnJog2; break; //picker
        case 4 : pnJogUnit -> Parent = pnJog0; break; //front
        case 5 : pnJogUnit -> Parent = pnJog1; break; //rear
        case 6 : pnJogUnit -> Parent = pnJog3; break; //disp
        case 7 : pnJogUnit -> Parent = pnJog4; break; //stage
    }
}
//---------------------------------------------------------------------------


void __fastcall TFrmDeviceSet::edJogUnitChange(TObject *Sender)
{
    for(int i=0;i<MAX_MOTR;i++) {
        if(rgJogUnit->ItemIndex == 0) FraMotor[i]->SetUnit(utJog ,0.0 );
        if(rgJogUnit->ItemIndex == 1) FraMotor[i]->SetUnit(utMove,1.0 );
        if(rgJogUnit->ItemIndex == 2) FraMotor[i]->SetUnit(utMove,0.5 );
        if(rgJogUnit->ItemIndex == 3) FraMotor[i]->SetUnit(utMove,0.1 );
        if(rgJogUnit->ItemIndex == 4) FraMotor[i]->SetUnit(utMove,0.05);
        if(rgJogUnit->ItemIndex == 5) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edJogUnit->Text,0.0));
    }
}
//---------------------------------------------------------------------------




void __fastcall TFrmDeviceSet::rgJogUnitClick(TObject *Sender)
{
    

    for(int i=0;i<MAX_MOTR;i++) {
        if(rgJogUnit->ItemIndex == 0) FraMotor[i]->SetUnit(utJog ,0.0 );
        if(rgJogUnit->ItemIndex == 1) FraMotor[i]->SetUnit(utMove,1.0 );
        if(rgJogUnit->ItemIndex == 2) FraMotor[i]->SetUnit(utMove,0.5 );
        if(rgJogUnit->ItemIndex == 3) FraMotor[i]->SetUnit(utMove,0.1 );
        if(rgJogUnit->ItemIndex == 4) FraMotor[i]->SetUnit(utMove,0.05);
        if(rgJogUnit->ItemIndex == 5) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edJogUnit->Text,0.0));
    }
}
//---------------------------------------------------------------------------
void TFrmDeviceSet::PaintPattern()
{
    // 이미지 초기화
    imPatternImage -> Width  = PANEL_DRAW_WIDTH ;
    imPatternImage -> Height = PANEL_DRAW_WIDTH ;

    TRect DrawRect ;
    DrawRect.left   = 0 ;
    DrawRect.top    = 0 ;
    DrawRect.right  = imPatternImage -> Width  ;
    DrawRect.bottom = imPatternImage -> Height ;

    imPatternImage -> Canvas -> Brush -> Color = clGray ;
    imPatternImage -> Canvas -> FillRect(DrawRect) ;

    //각 레졸루션.
    const double dPxFromMm = PANEL_DRAW_WIDTH / (double)REAL_DRAW_WIDTH  ;
    const double dMmFromPx = REAL_DRAW_WIDTH  / (double)PANEL_DRAW_WIDTH ;



    // 이미지 센터 좌표 값 구하기
    const double dCenterXPx = (double)PANEL_DRAW_WIDTH / 2.0 ;
    const double dCenterYPx = (double)PANEL_DRAW_WIDTH / 2.0 ;

    const double dCenterXMm = (double)REAL_DRAW_WIDTH  / 2.0 ;
    const double dCenterYMm = (double)REAL_DRAW_WIDTH  / 2.0 ;



    //자제 그림 HIC=============================================================
    const double dHICSttX = (REAL_DRAW_WIDTH - OM.DevInfo.dHicWidth )/2.0 ;
    const double dHICSttY = (REAL_DRAW_WIDTH - OM.DevInfo.dHicHeight)/2.0 ;
    const double dHICEndX = dHICSttX + OM.DevInfo.dHicWidth  ;
    const double dHICEndY = dHICSttY + OM.DevInfo.dHicHeight ;
    const int iPolygonPnt = 6 ;
    TPoint aPnts[iPolygonPnt] ;
    //왼쪽위.
    aPnts[0].x = dPxFromMm * dHICSttX ;
    aPnts[0].y = dPxFromMm * dHICSttY ;
    aPnts[1].x = dPxFromMm *(dHICEndX - OM.DevInfo.dHicChamper) ;
    aPnts[1].y = dPxFromMm * dHICSttY ;
    aPnts[2].x = dPxFromMm * dHICEndX ;
    aPnts[2].y = dPxFromMm *(dHICSttY + OM.DevInfo.dHicChamper) ;
    aPnts[3].x = dPxFromMm * dHICEndX ;
    aPnts[3].y = dPxFromMm *(dHICEndY - OM.DevInfo.dHicChamper) ;
    aPnts[4].x = dPxFromMm *(dHICEndX - OM.DevInfo.dHicChamper) ;
    aPnts[4].y = dPxFromMm * dHICEndY ;
    aPnts[5].x = dPxFromMm * dHICSttX ;
    aPnts[5].y = dPxFromMm * dHICEndY ;

    imPatternImage -> Canvas -> Pen   -> Color = clHotLight ;
    imPatternImage -> Canvas -> Brush -> Color = clHotLight ;
    imPatternImage -> Canvas -> Pen -> Width = 1 ;
    imPatternImage -> Canvas -> Pen -> Style = psSolid ;
    imPatternImage -> Canvas -> Polygon(aPnts , iPolygonPnt-1);
    //==========================================================================

    if(OM.DevInfo.iWorkMode == wmFos) {
        //자제 그림 DIE=============================================================
        const double dDIESttX = (REAL_DRAW_WIDTH - OM.DevInfo.dDieWidth )/2.0 ;
        const double dDIESttY = (REAL_DRAW_WIDTH - OM.DevInfo.dDieHeight)/2.0 ;
        const double dDIEEndX = dDIESttX + OM.DevInfo.dDieWidth  ;
        const double dDIEEndY = dDIESttY + OM.DevInfo.dDieHeight ;
        //왼쪽위.
        aPnts[0].x = dPxFromMm * dDIESttX ;
        aPnts[0].y = dPxFromMm * dDIESttY ;
        aPnts[1].x = dPxFromMm *(dDIEEndX - OM.DevInfo.dDieChamper) ;
        aPnts[1].y = dPxFromMm * dDIESttY ;
        aPnts[2].x = dPxFromMm * dDIEEndX ;
        aPnts[2].y = dPxFromMm *(dDIESttY + OM.DevInfo.dDieChamper) ;
        aPnts[3].x = dPxFromMm * dDIEEndX ;
        aPnts[3].y = dPxFromMm *(dDIEEndY - OM.DevInfo.dDieChamper) ;
        aPnts[4].x = dPxFromMm *(dDIEEndX - OM.DevInfo.dDieChamper) ;
        aPnts[4].y = dPxFromMm * dDIEEndY ;
        aPnts[5].x = dPxFromMm * dDIESttX ;
        aPnts[5].y = dPxFromMm * dDIEEndY ;

        imPatternImage -> Canvas -> Pen   -> Color = clWindowFrame ;
        imPatternImage -> Canvas -> Brush -> Color = clWindowFrame ;
        imPatternImage -> Canvas -> Pen -> Width = 1 ;
        imPatternImage -> Canvas -> Pen -> Style = psSolid ;
        imPatternImage -> Canvas -> Polygon(aPnts , iPolygonPnt-1);
        //==========================================================================

        //자제 그림 FOS=============================================================
        const double dFOSSttSttX = (REAL_DRAW_WIDTH - OM.DevInfo.dFosWidth )/2.0 - OM.DevOptn.dFosPlaceSttYOfs - OM.DevOptn.dPlaceYOfs;
        const double dFOSSttSttY = (REAL_DRAW_WIDTH - OM.DevInfo.dFosHeight)/2.0 ;
        const double dFOSSttEndX = dFOSSttSttX + OM.DevInfo.dFosWidth  ;
        const double dFOSSttEndY = dFOSSttSttY + OM.DevInfo.dFosHeight ;
        //왼쪽위.
        aPnts[0].x = dPxFromMm *(dFOSSttSttX                         ) ;
        aPnts[0].y = dPxFromMm *(dFOSSttSttY                         ) ;
        aPnts[1].x = dPxFromMm *(dFOSSttEndX - OM.DevInfo.dFosChamper) ;
        aPnts[1].y = dPxFromMm *(dFOSSttSttY                         ) ;
        aPnts[2].x = dPxFromMm *(dFOSSttEndX                         ) ;
        aPnts[2].y = dPxFromMm *(dFOSSttSttY + OM.DevInfo.dFosChamper) ;
        aPnts[3].x = dPxFromMm *(dFOSSttEndX                         ) ;
        aPnts[3].y = dPxFromMm *(dFOSSttEndY - OM.DevInfo.dFosChamper) ;
        aPnts[4].x = dPxFromMm *(dFOSSttEndX - OM.DevInfo.dFosChamper) ;
        aPnts[4].y = dPxFromMm *(dFOSSttEndY                         ) ;
        aPnts[5].x = dPxFromMm *(dFOSSttSttX                         ) ;
        aPnts[5].y = dPxFromMm *(dFOSSttEndY                         ) ;

        imPatternImage -> Canvas -> Pen   -> Color = 0x00B3F9F7 ;
        imPatternImage -> Canvas -> Brush -> Color = 0x00B3F9F7 ;
        imPatternImage -> Canvas -> Pen -> Width = 1 ;
        imPatternImage -> Canvas -> Pen -> Style = psSolid ;
        imPatternImage -> Canvas -> Polygon(aPnts , iPolygonPnt-1);
        //==========================================================================
        //안착 상태 점선  그림 FOS=============================================================
        const double dFOSSttX = (REAL_DRAW_WIDTH - OM.DevInfo.dFosWidth )/2.0 ;
        const double dFOSSttY = (REAL_DRAW_WIDTH - OM.DevInfo.dFosHeight)/2.0 ;
        const double dFOSEndX = dFOSSttX + OM.DevInfo.dFosWidth  ;
        const double dFOSEndY = dFOSSttY + OM.DevInfo.dFosHeight ;
        aPnts[0].x = dPxFromMm *(-OM.DevOptn.dPlaceYOfs + dFOSSttX                         ) ;
        aPnts[0].y = dPxFromMm *(-OM.DevOptn.dPlaceXOfs + dFOSSttY                         ) ;
        aPnts[1].x = dPxFromMm *(-OM.DevOptn.dPlaceYOfs + dFOSEndX - OM.DevInfo.dFosChamper) ;
        aPnts[1].y = dPxFromMm *(-OM.DevOptn.dPlaceXOfs + dFOSSttY                         ) ;
        aPnts[2].x = dPxFromMm *(-OM.DevOptn.dPlaceYOfs + dFOSEndX                         ) ;
        aPnts[2].y = dPxFromMm *(-OM.DevOptn.dPlaceXOfs + dFOSSttY + OM.DevInfo.dFosChamper) ;
        aPnts[3].x = dPxFromMm *(-OM.DevOptn.dPlaceYOfs + dFOSEndX                         ) ;
        aPnts[3].y = dPxFromMm *(-OM.DevOptn.dPlaceXOfs + dFOSEndY - OM.DevInfo.dFosChamper) ;
        aPnts[4].x = dPxFromMm *(-OM.DevOptn.dPlaceYOfs + dFOSEndX - OM.DevInfo.dFosChamper) ;
        aPnts[4].y = dPxFromMm *(-OM.DevOptn.dPlaceXOfs + dFOSEndY                         ) ;
        aPnts[5].x = dPxFromMm *(-OM.DevOptn.dPlaceYOfs + dFOSSttX                         ) ;
        aPnts[5].y = dPxFromMm *(-OM.DevOptn.dPlaceXOfs + dFOSEndY                         ) ;

        imPatternImage -> Canvas -> Pen   -> Color = clBlack ;
        imPatternImage -> Canvas -> Brush -> Color = clWindowFrame ;
        imPatternImage -> Canvas -> Brush -> Style = bsClear ;
        imPatternImage -> Canvas -> Pen -> Width = 1 ;
        imPatternImage -> Canvas -> Pen -> Style = psDash ;
        imPatternImage -> Canvas -> Polygon(aPnts , iPolygonPnt-1);
    }
    else {
        //안착 상태 점선  그림 DIE=============================================================
        const double dDIESttX = (REAL_DRAW_WIDTH - OM.DevInfo.dDieWidth )/2.0 ;
        const double dDIESttY = (REAL_DRAW_WIDTH - OM.DevInfo.dDieHeight)/2.0 ;
        const double dDIEEndX = dDIESttX + OM.DevInfo.dDieWidth  ;
        const double dDIEEndY = dDIESttY + OM.DevInfo.dDieHeight ;
        //왼쪽위.
        aPnts[0].x = dPxFromMm *(-OM.DevOptn.dPlaceYOfs + dDIESttX                         ) ;
        aPnts[0].y = dPxFromMm *(-OM.DevOptn.dPlaceXOfs + dDIESttY                         ) ;
        aPnts[1].x = dPxFromMm *(-OM.DevOptn.dPlaceYOfs + dDIEEndX - OM.DevInfo.dDieChamper) ;
        aPnts[1].y = dPxFromMm *(-OM.DevOptn.dPlaceXOfs + dDIESttY                         ) ;
        aPnts[2].x = dPxFromMm *(-OM.DevOptn.dPlaceYOfs + dDIEEndX                         ) ;
        aPnts[2].y = dPxFromMm *(-OM.DevOptn.dPlaceXOfs + dDIESttY + OM.DevInfo.dDieChamper) ;
        aPnts[3].x = dPxFromMm *(-OM.DevOptn.dPlaceYOfs + dDIEEndX                         ) ;
        aPnts[3].y = dPxFromMm *(-OM.DevOptn.dPlaceXOfs + dDIEEndY - OM.DevInfo.dDieChamper) ;
        aPnts[4].x = dPxFromMm *(-OM.DevOptn.dPlaceYOfs + dDIEEndX - OM.DevInfo.dDieChamper) ;
        aPnts[4].y = dPxFromMm *(-OM.DevOptn.dPlaceXOfs + dDIEEndY                         ) ;
        aPnts[5].x = dPxFromMm *(-OM.DevOptn.dPlaceYOfs + dDIESttX                         ) ;
        aPnts[5].y = dPxFromMm *(-OM.DevOptn.dPlaceXOfs + dDIEEndY                         ) ;

        imPatternImage -> Canvas -> Pen   -> Color = clBlack ;
        imPatternImage -> Canvas -> Brush -> Color = clWindowFrame ;
        imPatternImage -> Canvas -> Brush -> Style = bsClear ;
        imPatternImage -> Canvas -> Pen -> Width = 1 ;
        imPatternImage -> Canvas -> Pen -> Style = psDash ;
        imPatternImage -> Canvas -> Polygon(aPnts , iPolygonPnt-1);
    }

    // 실제 Line 그리는 부분
    double dDrawX , dDrawY ;

    imPatternImage -> Canvas -> Pen -> Width = 3 ;
    dDrawX = dPxFromMm * (PTT.GetScalePosY(0) + dCenterXMm);// + PTT.GetSttOffsetY());
    dDrawY = dPxFromMm * (PTT.GetScalePosX(0) + dCenterYMm);// + PTT.GetSttOffsetX());
    imPatternImage -> Canvas -> MoveTo(dDrawX, dDrawY) ;
    const int iPosCnt = PTT.GetPosCnt() ;
    for(int i = 0 ; i < iPosCnt ; i++) {
        //if(PTT.GetScalePosX(i) == 0 && PTT.GetScalePosY(i) == 0) continue ;
        //dDrawX = dPxFromMm * (PTT.GetScalePosX(i) + dCenterXMm + PTT.GetSttOffsetX());
        //dDrawY = dPxFromMm * (PTT.GetScalePosY(i) + dCenterYMm + PTT.GetSttOffsetY());
        imPatternImage -> Canvas -> Pen -> Color = PTT.GetDispOn(i) ? clYellow : clLime; // : clYellow ;
        dDrawX = dPxFromMm * (PTT.GetScalePosY(i) + dCenterXMm ); //+ PTT.GetSttOffsetY()
        dDrawY = dPxFromMm * (PTT.GetScalePosX(i) + dCenterYMm ); //+ PTT.GetSttOffsetX()
        imPatternImage -> Canvas -> LineTo(dDrawX,dDrawY) ;
    }

    imPatternImage -> Canvas -> Pen -> Color = clRed ;
    for(int i = 0 ; i < iPosCnt ; i++) {

        //if(PTT.GetScalePosX(i) == 0 && PTT.GetScalePosY(i) == 0) continue ;
        //dDrawX = dPxFromMm * (PTT.GetScalePosX(i) + dCenterXMm + PTT.GetSttOffsetX());
        //dDrawY = dPxFromMm * (PTT.GetScalePosY(i) + dCenterYMm + PTT.GetSttOffsetY());
        dDrawX = dPxFromMm * (PTT.GetScalePosY(i) + dCenterXMm);// + PTT.GetSttOffsetY());
        dDrawY = dPxFromMm * (PTT.GetScalePosX(i) + dCenterYMm);// + PTT.GetSttOffsetX());
        imPatternImage -> Canvas -> Ellipse(dDrawX-1, dDrawY-1, dDrawX+1, dDrawY+1);
    }

    //Center 점...
    imPatternImage -> Canvas -> Pen -> Color = clLime ; // 실제 디바이스 크기의 사각형 그릴 때 필요한 부분, Device 가 여러개라 수정해야함...
    imPatternImage -> Canvas -> Pen -> Width = 1 ;
    imPatternImage -> Canvas -> Pen -> Style = psDashDot ;
    imPatternImage -> Canvas -> MoveTo(dCenterXPx              , 0                      ) ;
    imPatternImage -> Canvas -> LineTo(dCenterXPx              , imPatternImage -> Width) ;
    imPatternImage -> Canvas -> MoveTo(0                       , dCenterYPx             ) ;
    imPatternImage -> Canvas -> LineTo(imPatternImage -> Width , dCenterYPx             ) ;

    // Node 숫자 출력 부분
    imPatternImage -> Canvas -> Font -> Size = 10 ;
    imPatternImage -> Canvas -> Font -> Height = 16 ;
    imPatternImage -> Canvas -> Font -> Color = clBlack ;
    //imPatternImage -> Canvas -> Font -> Style = clBlack ;
    imPatternImage -> Canvas -> Font -> Style = TFontStyles() >> fsBold;

    for(int i = 0 ; i < iPosCnt ; i++) {
        //if(PTT.GetScalePosX(i) == 0 && PTT.GetScalePosY(i) == 0) continue ;
        //dDrawX = dPxFromMm * (PTT.GetScalePosX(i) + dCenterXMm + PTT.GetSttOffsetX());
        //dDrawY = dPxFromMm * (PTT.GetScalePosY(i) + dCenterYMm + PTT.GetSttOffsetY());
        dDrawX = dPxFromMm * (PTT.GetScalePosY(i) + dCenterXMm);// + PTT.GetSttOffsetY());
        dDrawY = dPxFromMm * (PTT.GetScalePosX(i) + dCenterYMm);// + PTT.GetSttOffsetX());
        imPatternImage -> Canvas -> TextOutA(dDrawX + 5, dDrawY + 5 , i) ;
    }

    // 자재 X, Y 화살표 방향 그리기 부분
    imPatternImage -> Canvas -> Pen -> Width = 2 ;
    imPatternImage -> Canvas -> Pen -> Color = clBlack ;

    // (X 화살표)
    imPatternImage -> Canvas -> MoveTo(40,  10);
    imPatternImage -> Canvas -> LineTo(180, 10);
    imPatternImage -> Canvas -> MoveTo(180, 10);
    imPatternImage -> Canvas -> LineTo(175,  5);
    imPatternImage -> Canvas -> MoveTo(180, 10);
    imPatternImage -> Canvas -> LineTo(175, 15);

    // (Y 화살표)
    imPatternImage -> Canvas -> MoveTo(10,  40);
    imPatternImage -> Canvas -> LineTo(10, 180);
    imPatternImage -> Canvas -> MoveTo(10, 180);
    imPatternImage -> Canvas -> LineTo(5,  175);
    imPatternImage -> Canvas -> MoveTo(10, 180);
    imPatternImage -> Canvas -> LineTo(15, 175);


    // X, Y 문자 그리기 부분
    imPatternImage -> Canvas -> Font -> Size = 24;
    imPatternImage -> Canvas -> Font -> Height = 20 ;
    imPatternImage -> Canvas -> Font -> Style = TFontStyles() << fsBold;
    imPatternImage -> Canvas -> Font -> Color = clRed ;

    // (X & Y 문자 표시)
    imPatternImage -> Canvas -> TextOutA(20, 2, "Y");
    imPatternImage -> Canvas -> TextOutA(5, 17, "X");

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::sgPatternPosSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
    //if(ARow == 0){
    //    sgPatternPos -> Options >> goEditing ;
    //    return ;
    //}
    if(ACol == 3 || ACol == 4){
        sgPatternPos -> Options >> goEditing ;
        return ;
    }
    sgPatternPos -> Options << goEditing ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::sgPatternPosDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
    AnsiString sItem ;
    if(ARow > 0){
        if(ACol == 3 && ACol ==4) { //Scale X, Y
            sItem = sgPatternPos -> Cells[ACol][ARow];
            sgPatternPos -> Canvas -> Brush -> Color = clMedGray ;
            sgPatternPos -> Canvas -> FillRect(Rect) ;
            sgPatternPos -> Canvas -> TextOut(Rect.Left + 3, Rect.Top + (Rect.Height() - sgPatternPos -> Canvas -> TextHeight(sItem)) / 2, sItem) ;
        }
        if(ACol == 5 ) { //OnOff
            sItem = sgPatternPos -> Cells[ACol][ARow];
            sgPatternPos -> Canvas -> Brush -> Color = sItem == "0" ? clGray : clLime ;
            sgPatternPos -> Canvas -> FillRect(Rect) ;
            sgPatternPos -> Canvas -> TextOut(Rect.Left + 3, Rect.Top + (Rect.Height() - sgPatternPos -> Canvas -> TextHeight(sItem)) / 2, sItem) ;
        }
    }
}
//---------------------------------------------------------------------------




void __fastcall TFrmDeviceSet::btOpenPatternClick(TObject *Sender)
{
    if(!dgLoadFilePath->Execute())    return ;

    PTT.LoadPat(dgLoadFilePath->FileName);
    UpdatePattern(toTabl);
    PaintPattern();

}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btSavePatternClick(TObject *Sender)
{
    if(!dgSaveFilePath->Execute())    return ;
    PTT.SavePat(dgSaveFilePath->FileName);
}
//---------------------------------------------------------------------------


void __fastcall TFrmDeviceSet::Button1Click(TObject *Sender)
{
    FrmDispCh -> ShowModal() ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::cbDspChChange(TObject *Sender)
{
    btSavePosClick(Sender);
}
//---------------------------------------------------------------------------







void __fastcall TFrmDeviceSet::btPckrStdCalClick(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;

         if(iManNo == mcDSPCycleManStdCal    ){FM_MsgOkModal("디스펜서 Z축 보정","실린지의 하단 밀착 여부와 스테이지에 자제 없음을 확인해주세요");}
    else if(iManNo == mcDSPCycleVisnPos      ){FM_MsgOkModal("스테이지 비젼위치","스테이지 높이를 측정하여 비젼 포커스 위치로 이동합니다.");}

    else if(iManNo ==mcPCKCycleManStdCal     ){FM_MsgOkModal("픽커 Z축 보정" ,"피커에 자제 없음을 확인해주세요");}
    else if(iManNo ==mcVisnCal               ){FM_MsgOkModal("비전 센터 보정","얼라이너에 자제를 놓으시고 피커와 스테이지에 자제 없음을 확인해주세요");}
    else if(iManNo ==mcPCKCycleManAlignerCal ){FM_MsgOkModal("얼라이너 보정" ,"얼라이너에 자제를 놓으시고 피커에 자제 없음을 확인해주세요");}

    else if(iManNo ==mcPCKCycleManVsAlignPos ){FM_MsgOkModal("피커 얼라인 검사위치","피커의 높이를 측정 하여 얼라인 검사 높이로 이동합니다.");}
    else if(iManNo ==mcPCKCycleManVsDefectPos){FM_MsgOkModal("피커 디펙트 검사위치","피커의 높이를 측정 하여 디펙트 검사 높이로 이동합니다.");}


    MM.SetManCycle(iManNo);

    //if(iManNo == mcDSPCycleManNdlCheck  ||
    //   iManNo == mcDSPCycleManStdCal    ||
    //   iManNo == mcPCKCycleManStdCal    ||
    //   iManNo == mcPCKCycleManAlignerCal){
    //    m_bFormPosUpdate = true;
    //}



}
//---------------------------------------------------------------------------






void __fastcall TFrmDeviceSet::btLDR_FSlotClick(TObject *Sender)
{
    int iTag = ((TButton *)Sender) -> Tag ;
    int iSlot ;
    bool bTop ;
    double dPos ;

    if(iTag == 0) {//프론트로더 Z
        if(!LDR_F.CheckSafe(miLDR_ZElevF,piLDR_ZElevFWait)) return  ;
        iSlot = cbLDR_FSlot -> ItemIndex % OM.DevInfo.iMgzSlotCnt ;
        bTop  = (cbLDR_FSlot -> ItemIndex / OM.DevInfo.iMgzSlotCnt) >0;
        if(bTop) { dPos = PM.GetValue(miLDR_ZElevF , pvLDR_ZElevFWorkStartTop);}
        else     { dPos = PM.GetValue(miLDR_ZElevF , pvLDR_ZElevFWorkStartBtm);}
        dPos += OM.DevInfo.dMgzSlotPitch * iSlot;
        MT_GoAbsMan(miLDR_ZElevF , dPos);
    }
    else if(iTag == 1){ //프론트인덱스 X
        if(!IDX_F.CheckSafe(miIDX_XIndxF,piIDX_XIndxFWork)) return  ;
        iSlot = cbIDX_FPitch -> ItemIndex ;
        dPos  = PM.GetValue(miIDX_XIndxF , pvIDX_XIndxFWorkStart);
        dPos += OM.DevInfo.dBoatColPitch * iSlot;
        MT_GoAbsMan(miIDX_XIndxF , dPos);
    }
    else if(iTag == 2){ //프론트언로더 Z
        if(!ULD_F.CheckSafe(miULD_ZElevF,piULD_ZElevFWait)) return  ;
        iSlot = cbULD_FSlot -> ItemIndex % OM.DevInfo.iMgzSlotCnt ;
        bTop  = (cbULD_FSlot -> ItemIndex / OM.DevInfo.iMgzSlotCnt) >0;
        if(bTop) { dPos = PM.GetValue(miULD_ZElevF , pvULD_ZElevFWorkStartTop);}
        else     { dPos = PM.GetValue(miULD_ZElevF , pvULD_ZElevFWorkStartBtm);}
        dPos += OM.DevInfo.dMgzSlotPitch * iSlot;
        MT_GoAbsMan(miULD_ZElevF , dPos);
    }
    else if(iTag == 3){ //리어 로더Z
        if(!LDR_R.CheckSafe(miLDR_ZElevR,piLDR_ZElevRWait)) return  ;
        iSlot = cbLDR_RSlot -> ItemIndex % OM.DevInfo.iMgzSlotCnt ;
        bTop  = (cbLDR_RSlot -> ItemIndex / OM.DevInfo.iMgzSlotCnt) >0;
        if(bTop) { dPos = PM.GetValue(miLDR_ZElevR , pvLDR_ZElevRWorkStartTop);}
        else     { dPos = PM.GetValue(miLDR_ZElevR , pvLDR_ZElevRWorkStartBtm);}
        dPos += OM.DevInfo.dMgzSlotPitch * iSlot;
        MT_GoAbsMan(miLDR_ZElevR , dPos);
    }
    else if(iTag == 4){ //리어 인덱스 X
        if(!IDX_R.CheckSafe(miIDX_XIndxR,piIDX_XIndxRWork)) return  ;
        iSlot = cbIDX_RPitch -> ItemIndex ;
        dPos  = PM.GetValue(miIDX_XIndxR , pvIDX_XIndxRWorkStart);
        dPos += OM.DevInfo.dBoatColPitch * iSlot;
        MT_GoAbsMan(miIDX_XIndxR , dPos);
    }
    else if(iTag == 5){ //리어 언로더 Z
        if(!ULD_R.CheckSafe(miULD_ZElevR,piULD_ZElevRWait)) return ;
        iSlot = cbULD_RSlot -> ItemIndex % OM.DevInfo.iMgzSlotCnt ;
        bTop  = (cbULD_RSlot -> ItemIndex / OM.DevInfo.iMgzSlotCnt) >0;
        if(bTop) { dPos = PM.GetValue(miULD_ZElevR , pvULD_ZElevRWorkStartTop);}
        else     { dPos = PM.GetValue(miULD_ZElevR , pvULD_ZElevRWorkStartBtm);}
        dPos += OM.DevInfo.dMgzSlotPitch * iSlot;
        MT_GoAbsMan(miULD_ZElevR , dPos);
    }





}
//---------------------------------------------------------------------------

