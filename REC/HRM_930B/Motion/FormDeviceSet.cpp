//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDeviceSet.h"
#include "FormCadPos.h"
#include "FormCadPosRtt.h"
#include "FormDispCh.h"

//---------------------------------------------------------------------------
//Part Reference.
//---------------------------------------------------------------------------
#include "LoaderFront.h"
#include "LoaderRear.h"
#include "Index.h"
#include "Stage.h"
#include "LeftTool.h"
#include "RightTool.h"
#include "Unloader.h"
//---------------------------------------------------------------------------
//Rs232 Part.
//---------------------------------------------------------------------------
#include "Rs232Dispr.h"
#include "Rs232Keyence.h"
#include "Rs232LoadCl.h"
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
#include "CadPosition.h"
#include "HexaPot.h"

#include "Confocal.h"

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
            FraMotor[i] -> SetIdType((EN_MOTR_ID)i ,MT_GetDirType(i));
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
            default :                                                                                                            break;
            case aiLTL_MGrperDnUp : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , LTL  .CheckSafe ); break;
            case aiLTL_SGrperDnUp : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , LTL  .CheckSafe ); break;
            case aiLTL_WrkPshDnUp : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , LTL  .CheckSafe ); break;
            case aiRTL_LWkPshUpDn : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , RTL  .CheckSafe ); break;
            case aiRTL_RWkPshUpDn : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , RTL  .CheckSafe ); break;
            case aiIDX_RoatorUpDn : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , IDX  .CheckSafe ); break;
            case aiLTL_DisprFDnUp : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , LTL  .CheckSafe ); break;
            case aiLTL_DisprRDnUp : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , LTL  .CheckSafe ); break;
            case aiLTL_UVCureDnUp : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , LTL  .CheckSafe ); break;
            case aiSTG_Fix1stFwBw : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , STG  .CheckSafe ); break;
            case aiSTG_Fix2ndFwBw : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , STG  .CheckSafe ); break;
            case aiSTG_Fix3rdFwBw : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , STG  .CheckSafe ); break;
            case aiSTG_Fix4thFwBw : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , STG  .CheckSafe ); break;
            case aiSTG_Fix5thFwBw : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , STG  .CheckSafe ); break;
            case aiLTL_GriperFwBW : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , STG  .CheckSafe ); break;
        }

        FraCylinder[i] ->Show();
    }
    // juhyeon
    foIDXIonizer       = new TFraOutput(); foIDXIonizer       -> SetConfig(yIDX_IDXIonizer   , IO_GetYName(yIDX_IDXIonizer   ), pnIO1  );
    foTRSIonizer       = new TFraOutput(); foTRSIonizer       -> SetConfig(yTRS_TRSIonizer   , IO_GetYName(yTRS_TRSIonizer   ), pnIO2  );
    foSTGVcumPump1     = new TFraOutput(); foSTGVcumPump1     -> SetConfig(ySTG_VcumPump1On  , IO_GetYName(ySTG_VcumPump1On  ), pnIO3  );
    foSTGVcumPump2     = new TFraOutput(); foSTGVcumPump2     -> SetConfig(ySTG_VcumPump2On  , IO_GetYName(ySTG_VcumPump2On  ), pnIO4  );
    foSTGVcumPump3     = new TFraOutput(); foSTGVcumPump3     -> SetConfig(ySTG_VcumPump3On  , IO_GetYName(ySTG_VcumPump3On  ), pnIO14 );
    foSTGUVAirSol      = new TFraOutput(); foSTGUVAirSol      -> SetConfig(ySTG_UVAirSol     , IO_GetYName(ySTG_UVAirSol     ), pnIO13 );
    foIDXVcumPumpOn    = new TFraOutput(); foIDXVcumPumpOn    -> SetConfig(yIDX_RttPumpOn    , IO_GetYName(yIDX_RttPumpOn    ), pnIO5  );
    foIDXEjct          = new TFraOutput(); foIDXEjct          -> SetConfig(yIDX_IdxVac       , IO_GetYName(yIDX_IdxVac       ), pnIO6  );
    foLTLDispr1OnOff   = new TFraOutput(); foLTLDispr1OnOff   -> SetConfig(yLTL_Dispr1OnOff  , IO_GetYName(yLTL_Dispr1OnOff  ), pnIO7  );
    foLTLDispr2OnOff   = new TFraOutput(); foLTLDispr2OnOff   -> SetConfig(yLTL_Dispr2OnOff  , IO_GetYName(yLTL_Dispr2OnOff  ), pnIO12 );
    foLTLUVHumeSuction = new TFraOutput(); foLTLUVHumeSuction -> SetConfig(yLTL_UVHumeSuction, IO_GetYName(yLTL_UVHumeSuction), pnIO8  );
    foRTLWeightOff     = new TFraOutput(); foRTLWeightOff     -> SetConfig(yRTL_WeightOff    , IO_GetYName(yRTL_WeightOff    ), pnIO9  );
    foRTLTransfrVccm1  = new TFraOutput(); foRTLTransfrVccm1  -> SetConfig(yRTL_TransfrVac1  , IO_GetYName(yRTL_TransfrVac1  ), pnIO10 );
    foRTLTransfrVccm2  = new TFraOutput(); foRTLTransfrVccm2  -> SetConfig(yRTL_TransfrVac2  , IO_GetYName(yRTL_TransfrVac2  ), pnIO11 );

    pcDeviceSet      -> ActivePageIndex = 0 ;

    //세이브버튼 누를때 , 디바이스셑 크리에트할때 , 디바이스 체인지 누를때...
    //Rs232_DisprFt.SetPTV(OM.DevOptn.dDspPrsPres , 10 , OM.DevOptn.dDspVacPres);
    //Rs232_DisprRr.SetPTV(OM.DevOptn.dDspPrsPres , 10 , OM.DevOptn.dDspVacPres);
    CDelayTimer TimeOut;
    Rs232_DisprFt.SetLoadCh(OM.DevOptn.iDspChFt+1);
    TimeOut.Clear();
    while(!Rs232_DisprFt.GetMsgEnd()){ //메세지 다 주고 받을때까지 기다림.
        Sleep(1);
        if(TimeOut.OnDelay(true , 1000)){
            FM_MsgOk("Error","프론트 디스펜서 채널 로드 통신타임아웃");
            break ;
        }
    }
    if(Rs232_DisprFt.GetErrMsg() != "") {
        FM_MsgOk("Disp Ft Error" , Rs232_DisprFt.GetErrMsg().c_str());
    }


    Rs232_DisprRr.SetLoadCh(OM.DevOptn.iDspChRr+1);
    TimeOut.Clear();
    while(!Rs232_DisprRr.GetMsgEnd()){ //메세지 다 주고 받을때까지 기다림.
        Sleep(1);
        if(TimeOut.OnDelay(true , 1000)){
            FM_MsgOk("Error","리어 디스펜서 채널 로드 통신타임아웃");
            break ;
        }
    }
    if(Rs232_DisprRr.GetErrMsg() != "") {
        FM_MsgOk("Disp Rr Error" , Rs232_DisprRr.GetErrMsg().c_str());
    }

    edLDRFPitch -> Text = OM.DevInfo.dLDRFMgzSlotPitch;
    edLDRRPitch -> Text = OM.DevInfo.dLDRRMgzSlotPitch;
    edULDPitch  -> Text = OM.DevInfo.dULDMgzSlotPitch;

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
//   if(edHicWidth   -> Text.ToDouble() > 50) edHicWidth   -> Text = 50 ;
//    if(edHicHeight  -> Text.ToDouble() > 50) edHicHeight  -> Text = 50 ;
//    if(edHicChamper -> Text.ToDouble() > 50) edHicChamper -> Text = 50 ;

    // DIE
//    if(edDieWidth   -> Text.ToDouble() > 50) edDieWidth   -> Text = 50 ;
//    if(edDieHeight  -> Text.ToDouble() > 50) edDieHeight  -> Text = 50 ;
//    if(edDieChamper -> Text.ToDouble() > 50) edDieChamper -> Text = 50 ;

    // FOS
//    if(edFosWidth   -> Text.ToDouble() > 50) edFosWidth   -> Text = 50 ;
//    if(edFosHeight  -> Text.ToDouble() > 50) edFosHeight  -> Text = 50 ;
//    if(edFosChamper -> Text.ToDouble() > 50) edFosChamper -> Text = 50 ;

    //시간이 오래 걸려서 개별로 하기로 한다.
    //CAD.LoadSave(false , OM.GetCrntDev());

    DM .ARAY[riLDR_F].SetMaxColRow(OM.DevInfo.iLDRFMgzBayCnt, OM.DevInfo.iLDRFMgzSlotCnt ); DM.ARAY[riLDR_F].SetLotNo("");
    DM .ARAY[riLDR_R].SetMaxColRow(OM.DevInfo.iLDRRMgzBayCnt, OM.DevInfo.iLDRRMgzSlotCnt ); DM.ARAY[riLDR_R].SetLotNo("");
    DM .ARAY[riIDX  ].SetMaxColRow(1                        , 1                          ); DM.ARAY[riIDX  ].SetLotNo("");
    DM .ARAY[riRTT  ].SetMaxColRow(1                        , 1                          ); DM.ARAY[riRTT  ].SetLotNo("");
    DM .ARAY[riTRF  ].SetMaxColRow(1                        , 1                          ); DM.ARAY[riTRF  ].SetLotNo("");
    DM .ARAY[riTRF  ].SetMaxColRow(1                        , 1                          ); DM.ARAY[riTRF  ].SetLotNo("");
    DM .ARAY[riSTG  ].SetMaxColRow(OM.DevInfo.iMidColCnt    , OM.DevInfo.iMidRowCnt      ); DM.ARAY[riSTG  ].SetLotNo("");
    DM.ARAY [riWKO  ].SetMaxColRow(1                        , 1                          ); DM.ARAY[riWKO  ].SetLotNo("");
    DM.ARAY [riPTL  ].SetMaxColRow(1                        , 1                          ); DM.ARAY[riPTL  ].SetLotNo("");
    DM .ARAY[riULD  ].SetMaxColRow(OM.DevInfo.iULDMgzBayCnt , OM.DevInfo.iULDMgzSlotCnt  ); DM.ARAY[riULD  ].SetLotNo("");
    CAD.SetRowCol(OM.DevInfo.iMidColCnt , OM.DevInfo.iMidRowCnt);




    //세이브버튼 누를때 , 디바이스셑 크리에트할때 , 디바이스 체인지 누를때...
    //Rs232_DisprFt.SetPTV(OM.DevOptn.dDspPrsPres , 10 , OM.DevOptn.dDspVacPres);
    //Rs232_DisprRr.SetPTV(OM.DevOptn.dDspPrsPres , 10 , OM.DevOptn.dDspVacPres);
    CDelayTimer TimeOut;
    Rs232_DisprFt.SetLoadCh(OM.DevOptn.iDspChFt+1);
    TimeOut.Clear();
    while(!Rs232_DisprFt.GetMsgEnd()){ //메세지 다 주고 받을때까지 기다림.
        Sleep(1);
        if(TimeOut.OnDelay(true , 1000)){
            FM_MsgOk("Error","프론트 디스펜서 채널 로드 통신타임아웃");
            break ;
        }
    }
    if(Rs232_DisprFt.GetErrMsg() != "") {
        FM_MsgOk("Disp Ft Error" , Rs232_DisprFt.GetErrMsg().c_str());
    }


    Rs232_DisprRr.SetLoadCh(OM.DevOptn.iDspChRr+1);
    TimeOut.Clear();
    while(!Rs232_DisprRr.GetMsgEnd()){ //메세지 다 주고 받을때까지 기다림.
        Sleep(1);
        if(TimeOut.OnDelay(true , 1000)){
            FM_MsgOk("Error","리어 디스펜서 채널 로드 통신타임아웃");
            break ;
        }
    }
    if(Rs232_DisprRr.GetErrMsg() != "") {
        FM_MsgOk("Disp Rr Error" , Rs232_DisprRr.GetErrMsg().c_str());
    }

    if(OM.DevOptn.dAttachForce > 5000) {
        OM.DevOptn.dAttachForce = 5000;
        FM_MsgOk("Error" , "Attach Force가 5000g을 초과하였습니다.");
        edAttachForce -> Text = OM.DevOptn.dAttachForce;
    }

    //20160419 수정 진섭
    //if(OM.DevOptn.dAttachDelay > 10000) {
    //    OM.DevOptn.dAttachDelay = 10;
    //    FM_MsgOk("Error" , "Attach Delay가 10sec를 초과하였습니다.");
    //    edAttachDelay -> Text = OM.DevOptn.dAttachDelay;
    //}

    

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

    edHghMsg2     -> Text = Rs232Keyence.GetMsg();


//    if(PCK.m_bRqstPMSave || DSP.m_bRqstPMSave) {
//        Trace("Update","PM Save Start");
//        PM.UpdatePstn(toTabl);
//        PM.Save(OM.GetCrntDev());
//        DSP.m_bRqstPMSave = false ;
//        PCK.m_bRqstPMSave = false ;
//        Trace("Update","PM Save End");
//        FM_MsgOk("Confirm", "위치 보정치를 적용 하였습니다.");
//    }

//    lbCrntNode -> Caption = DSP.m_iCrntDispNode ;
//    lbCrntNode -> Color   = IO_GetY(yLTL_DisprOnOff)? clLime : clBlack ;
//
//    tsRear      -> TabVisible = FM_GetLevel() == lvMaster ;
//    tsDispensor -> TabVisible = FM_GetLevel() == lvMaster ;
//    tsStage     -> TabVisible = FM_GetLevel() == lvMaster ;
//    tsFront     -> TabVisible = FM_GetLevel() == lvMaster ;

    MakeMidBlkImg();
    MakeLDRFImg();
    MakeLDRRImg()  ;
    MakeULDImg()   ;

    pnScreen -> Visible  = FM_GetLevel() != lvMaster  ;

    if(FM_GetLevel() != lvMaster && rgJogUnit -> ItemIndex == 5) {
        rgJogUnit -> ItemIndex = 0 ;
    }


    tmUpdateTimer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::FormDestroy(TObject *Sender)
{
    //FrmCadPos -> Close();
    //FrmCadPosRtt -> Close();
    tmUpdateTimer->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::UpdateDevInfo(bool bTo)
{
    if(bTo){
        //die Width, Height, Champer 불러오기는 같이 해도 상관없다.
        edMidColCnt        -> Text = OM.DevInfo.iMidColCnt    ;
        edMidRowCnt        -> Text = OM.DevInfo.iMidRowCnt    ;

        edLDRFMgzSlotPitch -> Text = OM.DevInfo.dLDRFMgzSlotPitch ;
        edLDRFMgzSlotCnt   -> Text = OM.DevInfo.iLDRFMgzSlotCnt   ;
        edLDRFMgzBayCnt    -> Text = OM.DevInfo.iLDRFMgzBayCnt    ;

        edLDRRMgzSlotPitch -> Text = OM.DevInfo.dLDRRMgzSlotPitch ;
        edLDRRMgzSlotCnt   -> Text = OM.DevInfo.iLDRRMgzSlotCnt   ;
        edLDRRMgzBayCnt    -> Text = OM.DevInfo.iLDRRMgzBayCnt    ;

        edULDMgzSlotPitch  -> Text = OM.DevInfo.dULDMgzSlotPitch  ;
        edULDMgzSlotCnt    -> Text = OM.DevInfo.iULDMgzSlotCnt    ;
        edULDMgzBayCnt     -> Text = OM.DevInfo.iULDMgzBayCnt     ;


        //PATTERN 영역은. PATTERN.h에서 저장 시킨다.
        //edScalePatternX -> Text = OM.DevInfo.dScalePatternX ;
        //edScalePatternY -> Text = OM.DevInfo.dScalePatternY ;
        //
        //edSttOffSetX    -> Text = OM.DevInfo.dSttOffSetX    ;
        //edSttOffSetY    -> Text = OM.DevInfo.dSttOffSetY    ;

    }
    else {
        OM.DevInfo.iMidColCnt          =  StrToIntDef  (edMidColCnt        -> Text, 1  ) ;
        OM.DevInfo.iMidRowCnt          =  StrToIntDef  (edMidRowCnt        -> Text, 1  ) ;

        OM.DevInfo.dLDRFMgzSlotPitch   =  StrToFloatDef(edLDRFMgzSlotPitch -> Text, 1.0) ;
        OM.DevInfo.iLDRFMgzSlotCnt     =  StrToIntDef  (edLDRFMgzSlotCnt   -> Text, 1  ) ;
        OM.DevInfo.iLDRFMgzBayCnt      =  StrToIntDef  (edLDRFMgzBayCnt    -> Text, 1  ) ;

        OM.DevInfo.dLDRRMgzSlotPitch   =  StrToFloatDef(edLDRRMgzSlotPitch -> Text, 1.0) ;
        OM.DevInfo.iLDRRMgzSlotCnt     =  StrToIntDef  (edLDRRMgzSlotCnt   -> Text, 1  ) ;
        OM.DevInfo.iLDRRMgzBayCnt      =  StrToIntDef  (edLDRRMgzBayCnt    -> Text, 1  ) ;

        OM.DevInfo.dULDMgzSlotPitch    =  StrToFloatDef(edULDMgzSlotPitch  -> Text, 1.0) ;
        OM.DevInfo.iULDMgzSlotCnt      =  StrToIntDef  (edULDMgzSlotCnt    -> Text, 1  ) ;
        OM.DevInfo.iULDMgzBayCnt       =  StrToIntDef  (edULDMgzBayCnt     -> Text, 1  ) ;

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
        //edStageTolXY       -> Text = OM.DevOptn.dStageTolXY          ;
        //edStageTolAng      -> Text = OM.DevOptn.dStageTolAng         ;
        //edPickerTolXY      -> Text = OM.DevOptn.dPickerTolXY         ;
        //edPickerTolAng     -> Text = OM.DevOptn.dPickerTolAng        ;
        //edPlaceTolXY       -> Text = OM.DevOptn.dPlaceTolXY          ;
        //edPlaceTolAng      -> Text = OM.DevOptn.dPlaceTolAng         ;

        edPlaceZOfs          -> Text = OM.DevOptn.dPlaceZOfs           ;
        edAttachSpeed1       -> Text = OM.DevOptn.dAttachSpeed1        ;
        edAttachSpeed2       -> Text = OM.DevOptn.dAttachSpeed2        ;
        edAttachForce        -> Text = OM.DevOptn.dAttachForce         ;
        edAttachForceOfs     -> Text = OM.DevOptn.dAttachForceOfs      ;
        edAttachDelay        -> Text = OM.DevOptn.dAttachDelay / 1000.0;

        edFixDelay           -> Text = OM.DevOptn.iFixDelay    / 1000.0;

        edDispZOfs           -> Text = OM.DevOptn.dDispZOfs            ;

        edStageInspOfsX      -> Text = OM.DevOptn.dStageInspOfsX       ;
        edStageInspOfsY      -> Text = OM.DevOptn.dStageInspOfsY       ;

        cbDspChFt            -> ItemIndex = OM.DevOptn.iDspChFt        ;
        cbDspChRr            -> ItemIndex = OM.DevOptn.iDspChRr        ;
        edDspMinAmount       -> Text      = OM.DevOptn.iDspMinAmount   ;

        //2015.11.05 Jinseop
        edMidCmsGapFirst   -> Text    = OM.DevOptn.dMidCmsGapFrst       ;
        edMidCmsGap        -> Text    = OM.DevOptn.dMidCmsGap           ;
        //cbEpoxyFirst       -> Checked = OM.DevOptn.bEpoxyFirst          ;
        cbUsePush          -> Checked = OM.DevOptn.bUsePush             ;
        //cbUseDispRear      -> Checked = OM.DevOptn.bUseDispRear         ;
        edDispAccDcc       -> Text    = OM.DevOptn.dDispAccDcc          ;

        edUVTopBrightness  -> Text    = OM.DevOptn.iUVTopBrightness     ;
        edUVBtmBrightness1 -> Text    = OM.DevOptn.iUVBtmBrightness1    ;
        edUVBtmBrightness2 -> Text    = OM.DevOptn.iUVBtmBrightness2    ;
        edUVBtmBrightness3 -> Text    = OM.DevOptn.iUVBtmBrightness3    ;
        edUVBtmBrightness4 -> Text    = OM.DevOptn.iUVBtmBrightness4    ;
        cbUseBtmUV         -> Checked = OM.DevOptn.bUseBtmUV            ;

        edLVisnTolXY       -> Text    = OM.DevOptn.dLVisnTolXY          ;
        edLVisnTolAng      -> Text    = OM.DevOptn.dLVisnTolAng         ;
        edRVisnTolXY       -> Text    = OM.DevOptn.dRVisnTolXY          ;
        edRVisnTolAng      -> Text    = OM.DevOptn.dRVisnTolAng         ;
        edREndVisnTolXY    -> Text    = OM.DevOptn.dREndVisnTolXY       ;
        edREndVisnTolAng   -> Text    = OM.DevOptn.dREndVisnTolAng      ;
        edRHgtTolZ         -> Text    = OM.DevOptn.dRHgtTolZ            ;

        edActiveEdgeGapX   -> Text    = OM.DevOptn.dActiveEdgeGapX      ;
        edActiveEdgeGapY   -> Text    = OM.DevOptn.dActiveEdgeGapY      ;

        edCmsAtachGapX     -> Text    = OM.DevOptn.dCmsAtachGapX        ;
        edCmsAtachGapY     -> Text    = OM.DevOptn.dCmsAtachGapY        ;

        edLineHeightCnt    -> Text    = OM.DevOptn.iLineHeightCnt       ;

        edEpoxyDispCntF    -> Text    = OM.DevOptn.iEpoxyDispCntF       ;
        edEpoxyDispCntR    -> Text    = OM.DevOptn.iEpoxyDispCntR       ;

        cbUseSTG_Fix1      -> Checked = OM.DevOptn.bUseSTG_Fix1         ;
        cbUseSTG_Fix2      -> Checked = OM.DevOptn.bUseSTG_Fix2         ;
        cbUseSTG_Fix3      -> Checked = OM.DevOptn.bUseSTG_Fix3         ;
        cbUseSTG_Fix4      -> Checked = OM.DevOptn.bUseSTG_Fix4         ;
        cbUseSTG_Fix5      -> Checked = OM.DevOptn.bUseSTG_Fix5         ;

        edDspVacPres       -> Text    = OM.DevOptn.dDspVacPres          ;
        edDspPrsPres       -> Text    = OM.DevOptn.dDspPrsPres          ;

        cbNotUseRotate     -> Checked = OM.DevOptn.bNotUseRotate        ;

        edUvCntFrst        -> Text    = OM.DevOptn.iUvCntFrst           ;
        edUvCntScnd        -> Text    = OM.DevOptn.iUvCntScnd           ;

        edSlaveHtOfsZ      -> Text    = OM.DevOptn.dSlaveHtOfsZ         ;

        edEpoxyName        -> Text    = OM.DevOptn.sEpoxyName           ;

        edHexaTopHeightOfs -> Text    = OM.DevOptn.dHexaTopHeightOfs    ;

        cbHeightMethod     -> ItemIndex = OM.DevOptn.iHeightMethod      ;
        edHeightPitchX     -> Text    = OM.DevOptn.dHeightPitchX        ;
        edHeightPitchY     -> Text    = OM.DevOptn.dHeightPitchY        ;
        edHeightStepCnt    -> Text    = OM.DevOptn.iHeightStepCnt       ;

        cbUseSTG_Vac1      -> Checked = OM.DevOptn.bUseSTG_Vac1         ;
        cbUseSTG_Vac2      -> Checked = OM.DevOptn.bUseSTG_Vac2         ;
        cbUseSTG_Vac3      -> Checked = OM.DevOptn.bUseSTG_Vac3         ;

        cbUseSTG_Vac1Frst  -> Checked = OM.DevOptn.bUseSTG_Vac1Frst     ;
        cbUseSTG_Vac2Frst  -> Checked = OM.DevOptn.bUseSTG_Vac2Frst     ;
        cbUseSTG_Vac3Frst  -> Checked = OM.DevOptn.bUseSTG_Vac3Frst     ;


        edBfUVOfsX00       -> Text    = OM.DevOptn.dBfUVOfsX00          ;
        edBfUVOfsX10       -> Text    = OM.DevOptn.dBfUVOfsX10          ;
        edBfUVOfsX01       -> Text    = OM.DevOptn.dBfUVOfsX01          ;
        edBfUVOfsX11       -> Text    = OM.DevOptn.dBfUVOfsX11          ;

        edBfUVOfsY00       -> Text    = OM.DevOptn.dBfUVOfsY00          ;
        edBfUVOfsY10       -> Text    = OM.DevOptn.dBfUVOfsY10          ;
        edBfUVOfsY01       -> Text    = OM.DevOptn.dBfUVOfsY01          ;
        edBfUVOfsY11       -> Text    = OM.DevOptn.dBfUVOfsY11          ;

        edUVTestSpeed      -> Text    = OM.DevOptn.dUVTestSpeed         ;
        edUVTestDist       -> Text    = OM.DevOptn.dUVTestDist          ;

        edMBStdHght        -> Text    = OM.DevOptn.dMBStdHght           ;
        edMBHghtTol        -> Text    = OM.DevOptn.dMBHghtTol           ;
        edHexaTopStdHght   -> Text    = OM.DevOptn.dHexaTopStdHght      ;
        edHexaTopHghtTol   -> Text    = OM.DevOptn.dHexaTopHghtTol      ;
        edCmsHeightOfs     -> Text    = OM.DevOptn.dCmsHeightOfs        ;

        cbUseFstDisp       -> Checked  = OM.DevOptn.bUseFstDisp       ;
        cbUseSecDisp       -> Checked  = OM.DevOptn.bUseSecDisp         ;

        cbFirstDisp        -> ItemIndex = OM.DevOptn.iFstDisp         ;
        cbSecDisp          -> ItemIndex = OM.DevOptn.iSecDisp           ;


    }
    else{
        //OM.DevOptn.dStageTolXY        = StrToFloatDef(edStageTolXY      -> Text,  OM.DevOptn.dStageTolXY     ) ;
        //OM.DevOptn.dStageTolAng       = StrToFloatDef(edStageTolAng     -> Text,  OM.DevOptn.dStageTolAng    ) ;
        //OM.DevOptn.dPickerTolXY       = StrToFloatDef(edPickerTolXY     -> Text,  OM.DevOptn.dPickerTolXY    ) ;
        //OM.DevOptn.dPickerTolAng      = StrToFloatDef(edPickerTolAng    -> Text,  OM.DevOptn.dPickerTolAng   ) ;
        //OM.DevOptn.dPlaceTolXY        = StrToFloatDef(edPlaceTolXY      -> Text,  OM.DevOptn.dPlaceTolXY     ) ;
        //OM.DevOptn.dPlaceTolAng       = StrToFloatDef(edPlaceTolAng     -> Text,  OM.DevOptn.dPlaceTolAng    ) ;

        OM.DevOptn.dPlaceZOfs        = StrToFloatDef(edPlaceZOfs       -> Text,  OM.DevOptn.dPlaceZOfs      ) ;
        OM.DevOptn.dAttachSpeed1     = StrToFloatDef(edAttachSpeed1    -> Text,  OM.DevOptn.dAttachSpeed1   ) ;
        OM.DevOptn.dAttachSpeed2     = StrToFloatDef(edAttachSpeed2    -> Text,  OM.DevOptn.dAttachSpeed2   ) ;
        OM.DevOptn.dAttachForce      = StrToFloatDef(edAttachForce     -> Text,  OM.DevOptn.dAttachForce    ) ;
        OM.DevOptn.dAttachForceOfs   = StrToFloatDef(edAttachForceOfs  -> Text,  OM.DevOptn.dAttachForceOfs ) ;
        OM.DevOptn.dAttachDelay      = StrToFloatDef(edAttachDelay     -> Text,  OM.DevOptn.dAttachDelay    )*1000.0 ;
        OM.DevOptn.iFixDelay         = StrToIntDef  (edFixDelay        -> Text,  OM.DevOptn.iFixDelay       )*1000.0 ;

        OM.DevOptn.dDispZOfs         = StrToFloatDef(edDispZOfs        -> Text,  OM.DevOptn.dDispZOfs       ) ;

        OM.DevOptn.dStageInspOfsX    = StrToFloatDef(edStageInspOfsX   -> Text,  OM.DevOptn.dStageInspOfsX) ;
        OM.DevOptn.dStageInspOfsY    = StrToFloatDef(edStageInspOfsY   -> Text,  OM.DevOptn.dStageInspOfsY) ;

        OM.DevOptn.iDspChFt          = cbDspChFt                       -> ItemIndex ;
        OM.DevOptn.iDspChRr          = cbDspChRr                       -> ItemIndex ;
        OM.DevOptn.iDspMinAmount     = StrToFloatDef(edDspMinAmount    -> Text,  OM.DevOptn.iDspMinAmount      ) ;

        //2015.11.05 Jinseop
        OM.DevOptn.dMidCmsGapFrst    = StrToFloatDef(edMidCmsGapFirst  -> Text,  OM.DevOptn.dMidCmsGapFrst     ) ;
        OM.DevOptn.dMidCmsGap        = StrToFloatDef(edMidCmsGap       -> Text,  OM.DevOptn.dMidCmsGap         ) ;
        //OM.DevOptn.bEpoxyFirst       = cbEpoxyFirst                    -> Checked                                ;
        OM.DevOptn.bUsePush          = cbUsePush                       -> Checked                                ;
        //OM.DevOptn.bUseDispRear      = cbUseDispRear                   -> Checked                                ;
        OM.DevOptn.dDispAccDcc       = StrToFloatDef(edDispAccDcc      -> Text,  OM.DevOptn.dDispAccDcc        ) ;

        OM.DevOptn.iUVTopBrightness  = StrToIntDef  (edUVTopBrightness -> Text,  OM.DevOptn.iUVTopBrightness   ) ;
        OM.DevOptn.iUVBtmBrightness1 = StrToIntDef  (edUVBtmBrightness1-> Text,  OM.DevOptn.iUVBtmBrightness1  ) ;
        OM.DevOptn.iUVBtmBrightness2 = StrToIntDef  (edUVBtmBrightness2-> Text,  OM.DevOptn.iUVBtmBrightness2  ) ;
        OM.DevOptn.iUVBtmBrightness3 = StrToIntDef  (edUVBtmBrightness3-> Text,  OM.DevOptn.iUVBtmBrightness3  ) ;
        OM.DevOptn.iUVBtmBrightness4 = StrToIntDef  (edUVBtmBrightness4-> Text,  OM.DevOptn.iUVBtmBrightness4  ) ;
        OM.DevOptn.bUseBtmUV         = cbUseBtmUV                      -> Checked                                ;

        OM.DevOptn.dLVisnTolXY          = StrToFloatDef(edLVisnTolXY      -> Text,  OM.DevOptn.dLVisnTolXY     ) ;
        OM.DevOptn.dLVisnTolAng         = StrToFloatDef(edLVisnTolAng     -> Text,  OM.DevOptn.dLVisnTolAng    ) ;
        OM.DevOptn.dRVisnTolXY          = StrToFloatDef(edRVisnTolXY      -> Text,  OM.DevOptn.dRVisnTolXY     ) ;
        OM.DevOptn.dRVisnTolAng         = StrToFloatDef(edRVisnTolAng     -> Text,  OM.DevOptn.dRVisnTolAng    ) ;
        OM.DevOptn.dREndVisnTolXY       = StrToFloatDef(edREndVisnTolXY   -> Text,  OM.DevOptn.dRVisnTolXY     ) ;
        OM.DevOptn.dREndVisnTolAng      = StrToFloatDef(edREndVisnTolAng  -> Text,  OM.DevOptn.dRVisnTolAng    ) ;
        OM.DevOptn.dRHgtTolZ            = StrToFloatDef(edRHgtTolZ        -> Text,  OM.DevOptn.dRHgtTolZ       ) ;


        OM.DevOptn.dCmsAtachGapX        = StrToFloatDef(edCmsAtachGapX    -> Text,  OM.DevOptn.dCmsAtachGapX   ) ;
        OM.DevOptn.dCmsAtachGapY        = StrToFloatDef(edCmsAtachGapY    -> Text,  OM.DevOptn.dCmsAtachGapY   ) ;

        OM.DevOptn.dActiveEdgeGapX      = StrToFloatDef(edActiveEdgeGapX  -> Text,  OM.DevOptn.dActiveEdgeGapX ) ;
        OM.DevOptn.dActiveEdgeGapY      = StrToFloatDef(edActiveEdgeGapY  -> Text,  OM.DevOptn.dActiveEdgeGapY ) ;


        OM.DevOptn.iLineHeightCnt       = StrToIntDef  (edLineHeightCnt   -> Text,  OM.DevOptn.iLineHeightCnt  ) ;
        OM.DevOptn.iEpoxyDispCntF       = StrToIntDef  (edEpoxyDispCntF   -> Text,  OM.DevOptn.iEpoxyDispCntF  ) ;
        OM.DevOptn.iEpoxyDispCntR       = StrToIntDef  (edEpoxyDispCntR   -> Text,  OM.DevOptn.iEpoxyDispCntR  ) ;
        OM.DevOptn.dSlaveHtOfsZ         = StrToFloatDef(edSlaveHtOfsZ     -> Text,  OM.DevOptn.dSlaveHtOfsZ    ) ;


        OM.DevOptn.sEpoxyName           = edEpoxyName                     -> Text ;

        OM.DevOptn.bUseSTG_Fix1         = cbUseSTG_Fix1     -> Checked     ;
        OM.DevOptn.bUseSTG_Fix2         = cbUseSTG_Fix2     -> Checked     ;
        OM.DevOptn.bUseSTG_Fix3         = cbUseSTG_Fix3     -> Checked     ;
        OM.DevOptn.bUseSTG_Fix4         = cbUseSTG_Fix4     -> Checked     ;
        OM.DevOptn.bUseSTG_Fix5         = cbUseSTG_Fix5     -> Checked     ;

        OM.DevOptn.dDspVacPres          = StrToFloatDef  (edDspVacPres      -> Text,  OM.DevOptn.dDspVacPres     ) ;
        OM.DevOptn.dDspPrsPres          = StrToFloatDef  (edDspPrsPres      -> Text,  OM.DevOptn.dDspPrsPres     ) ;

        OM.DevOptn.iUvCntFrst           = StrToIntDef    (edUvCntFrst       -> Text,  OM.DevOptn.iUvCntFrst      ) ;
        OM.DevOptn.iUvCntScnd           = StrToIntDef    (edUvCntScnd       -> Text,  OM.DevOptn.iUvCntScnd      ) ;
        if( OM.DevOptn.dDspPrsPres < 30) {
            OM.DevOptn.dDspPrsPres = 30;
        }
        if( OM.DevOptn.dDspPrsPres > 300) {
            OM.DevOptn.dDspPrsPres = 300;
        }

        OM.DevOptn.bNotUseRotate        = cbNotUseRotate    -> Checked     ;
        OM.DevOptn.dHexaTopHeightOfs    = StrToFloatDef  (edHexaTopHeightOfs-> Text,  OM.DevOptn.dHexaTopHeightOfs) ;

        OM.DevOptn.iHeightMethod        = cbHeightMethod    -> ItemIndex   ;
        OM.DevOptn.dHeightPitchX        = StrToFloatDef  (edHeightPitchX    -> Text,  OM.DevOptn.dHeightPitchX    ) ;
        OM.DevOptn.dHeightPitchY        = StrToFloatDef  (edHeightPitchY    -> Text,  OM.DevOptn.dHeightPitchY    ) ;
        OM.DevOptn.iHeightStepCnt       = StrToIntDef    (edHeightStepCnt   -> Text,  OM.DevOptn.iHeightStepCnt   ) ;

        OM.DevOptn.bUseSTG_Vac1         = cbUseSTG_Vac1     -> Checked      ;
        OM.DevOptn.bUseSTG_Vac2         = cbUseSTG_Vac2     -> Checked      ;
        OM.DevOptn.bUseSTG_Vac3         = cbUseSTG_Vac3     -> Checked      ;

        OM.DevOptn.bUseSTG_Vac1Frst     = cbUseSTG_Vac1Frst -> Checked    ;
        OM.DevOptn.bUseSTG_Vac2Frst     = cbUseSTG_Vac2Frst -> Checked    ;
        OM.DevOptn.bUseSTG_Vac3Frst     = cbUseSTG_Vac3Frst -> Checked    ;



        OM.DevOptn.dBfUVOfsX00          = StrToFloatDef(edBfUVOfsX00 -> Text,OM.DevOptn.dBfUVOfsX00)       ;
        OM.DevOptn.dBfUVOfsX10          = StrToFloatDef(edBfUVOfsX10 -> Text,OM.DevOptn.dBfUVOfsX10)       ;
        OM.DevOptn.dBfUVOfsX01          = StrToFloatDef(edBfUVOfsX01 -> Text,OM.DevOptn.dBfUVOfsX01)       ;
        OM.DevOptn.dBfUVOfsX11          = StrToFloatDef(edBfUVOfsX11 -> Text,OM.DevOptn.dBfUVOfsX11)       ;

        OM.DevOptn.dBfUVOfsY00          = StrToFloatDef(edBfUVOfsY00 -> Text,OM.DevOptn.dBfUVOfsY00)       ;
        OM.DevOptn.dBfUVOfsY10          = StrToFloatDef(edBfUVOfsY10 -> Text,OM.DevOptn.dBfUVOfsY10)       ;
        OM.DevOptn.dBfUVOfsY01          = StrToFloatDef(edBfUVOfsY01 -> Text,OM.DevOptn.dBfUVOfsY01)       ;
        OM.DevOptn.dBfUVOfsY11          = StrToFloatDef(edBfUVOfsY11 -> Text,OM.DevOptn.dBfUVOfsY11)       ;

        OM.DevOptn.dUVTestSpeed         = StrToFloatDef(edUVTestSpeed -> Text,OM.DevOptn.dUVTestSpeed)       ;
        OM.DevOptn.dUVTestDist          = StrToFloatDef(edUVTestDist  -> Text,OM.DevOptn.dUVTestDist )       ;

        OM.DevOptn.dMBStdHght           = StrToFloatDef(edMBStdHght      -> Text,OM.DevOptn.dMBStdHght)      ;
        OM.DevOptn.dMBHghtTol           = StrToFloatDef(edMBHghtTol      -> Text,OM.DevOptn.dMBHghtTol)      ;
        OM.DevOptn.dHexaTopStdHght      = StrToFloatDef(edHexaTopStdHght -> Text,OM.DevOptn.dHexaTopStdHght) ;
        OM.DevOptn.dHexaTopHghtTol      = StrToFloatDef(edHexaTopHghtTol -> Text,OM.DevOptn.dHexaTopHghtTol) ;
        OM.DevOptn.dCmsHeightOfs        = StrToFloatDef(edCmsHeightOfs   -> Text,OM.DevOptn.dCmsHeightOfs)   ;

        OM.DevOptn.bUseFstDisp          = cbUseFstDisp -> Checked ;
        OM.DevOptn.bUseSecDisp          = cbUseSecDisp   -> Checked ;

        OM.DevOptn.iFstDisp             = cbFirstDisp -> ItemIndex ;
        OM.DevOptn.iSecDisp             = cbSecDisp   -> ItemIndex ;

        UpdateDevOptn(toTabl);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::FormShow(TObject *Sender)
{
    UpdateDevInfo (toTabl);
    UpdateDevOptn (toTabl);
    PM.UpdatePstn (toTabl);

    tmUpdateTimer  -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::pcDeviceSetChange(TObject *Sender)
{
    switch ( pcDeviceSet -> ActivePageIndex ) {
        default:                               break; //
        case 0 :                               break; //Device
        case 1 : pnJogUnit -> Parent = pnJog0; break; //LDR_F
        case 2 : pnJogUnit -> Parent = pnJog1; break; //LDR_R
        case 3 : pnJogUnit -> Parent = pnJog2; break; //INDEX
        case 4 : pnJogUnit -> Parent = pnJog3; break; //STAGE
        case 5 : pnJogUnit -> Parent = pnJog4; break; //ULD 
        case 6 : pnJogUnit -> Parent = pnJog5; break; //TOOL LEFT
        case 7 : pnJogUnit -> Parent = pnJog6; break; //TOOL RIGHT
        case 8 :                               break; //OPTION
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
void __fastcall TFrmDeviceSet::sgPatternPosSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
    //if(ARow == 0){
    //    sgPatternPos -> Options >> goEditing ;
    //    return ;
    //}
    if(ACol == 3 || ACol == 4){
//        sgPatternPos -> Options >> goEditing ;
        return ;
    }
//    sgPatternPos -> Options << goEditing ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::sgPatternPosDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
    AnsiString sItem ;
    if(ARow > 0){
        if(ACol == 3 && ACol ==4) { //Scale X, Y
//            sItem = sgPatternPos -> Cells[ACol][ARow];
//            sgPatternPos -> Canvas -> Brush -> Color = clMedGray ;
//            sgPatternPos -> Canvas -> FillRect(Rect) ;
//            sgPatternPos -> Canvas -> TextOut(Rect.Left + 3, Rect.Top + (Rect.Height() - sgPatternPos -> Canvas -> TextHeight(sItem)) / 2, sItem) ;
        }
        if(ACol == 5 ) { //OnOff
//            sItem = sgPatternPos -> Cells[ACol][ARow];
//            sgPatternPos -> Canvas -> Brush -> Color = sItem == "0" ? clGray : clLime ;
//            sgPatternPos -> Canvas -> FillRect(Rect) ;
//            sgPatternPos -> Canvas -> TextOut(Rect.Left + 3, Rect.Top + (Rect.Height() - sgPatternPos -> Canvas -> TextHeight(sItem)) / 2, sItem) ;
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btSttCadPosClick(TObject *Sender)
{
    FrmCadPos -> Show() ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btRttCadPosClick(TObject *Sender)
{
    FrmCadPosRtt -> Show() ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmDeviceSet::cbDspChFtChange(TObject *Sender)
{
    btSavePosClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::cbDspChRrChange(TObject *Sender)
{
    btSavePosClick(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFrmDeviceSet::btHxZWaitClick(TObject *Sender)
{
    HexaPot.MoveAbsRes(paZ , HEX_Z_WAIT);

    HexaPot.Move();
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btHxGetClick(TObject *Sender)
{
    HexaPot.MoveAbsRes(paX , HEX_X_WAIT);
    HexaPot.MoveAbsRes(paY , HEX_Y_WAIT);
    HexaPot.MoveAbsRes(paZ , HEX_Z_GET );
    HexaPot.MoveAbsRes(paU , HEX_U_WAIT);
    HexaPot.MoveAbsRes(paV , HEX_V_WAIT);
    HexaPot.MoveAbsRes(paW , HEX_W_WAIT);

    HexaPot.Move();
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btHxWaitClick(TObject *Sender)
{
    HexaPot.MoveAbsRes(paX , HEX_X_WAIT);
    HexaPot.MoveAbsRes(paY , HEX_Y_WAIT);
    HexaPot.MoveAbsRes(paZ , HEX_Z_WAIT);
    HexaPot.MoveAbsRes(paU , HEX_U_WAIT);
    HexaPot.MoveAbsRes(paV , HEX_V_WAIT);
    HexaPot.MoveAbsRes(paW , HEX_W_WAIT);

    HexaPot.Move();
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btHxZGetClick(TObject *Sender)
{
    HexaPot.MoveAbsRes(paZ , HEX_Z_GET);

    HexaPot.Move();
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btHEXJogNClick(TObject *Sender)
{
    double JogPos = edHEXPos -> Text.ToDouble();
    HexaPot.MoveIncRes(cbHEXAxis -> ItemIndex, -JogPos);

    HexaPot.Move();
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btHEXJogPClick(TObject *Sender)
{
    double JogPos = edHEXPos -> Text.ToDouble();
    HexaPot.MoveIncRes(cbHEXAxis -> ItemIndex, JogPos);

    HexaPot.Move();
}

//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btBtmUVOnClick(TObject *Sender)
{
    Rs232UV.SetCuringOnOff(1, true );
    Rs232UV.SetCuringOnOff(2, true );
    Rs232UV.SetCuringOnOff(3, true );
    Rs232UV.SetCuringOnOff(4, true );
    IO_SetY(ySTG_UVAirSol , true);


}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btBtmUVOffClick(TObject *Sender)
{
    Rs232UV.SetCuringOnOff(1, false );
    Rs232UV.SetCuringOnOff(2, false );
    Rs232UV.SetCuringOnOff(3, false );
    Rs232UV.SetCuringOnOff(4, false );
    IO_SetY(ySTG_UVAirSol , false);
}
//---------------------------------------------------------------------------




void __fastcall TFrmDeviceSet::btUVTestClick(TObject *Sender)
{
    UpdateDevOptn(toBuff);
    OM.SaveDevOptn(OM.GetCrntDev());

    MM.SetManCycle(mcLTLCycleManUVTest);
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::MakeMidBlkImg()
{
    ImMidBlk -> Picture -> Bitmap =NULL; //이미지 초기화


    int uX1, uX2, uY1, uY2;

    int iMidColCnt , iMidRowCnt;

    iMidRowCnt = OM.DevInfo.iMidRowCnt ;
    iMidColCnt  = OM.DevInfo.iMidColCnt  ;

    int iGetWidth  = ImMidBlk -> Width ;
    int iGetHeight = ImMidBlk -> Height;

    int iSetWidth = 0 , iSetHeight = 0;

    double uGw   = (double)iGetWidth  / (double)(iMidColCnt);
    double uGh   = (double)iGetHeight / (double)(iMidRowCnt);
    int    iWOff = (int)((double)(iGetWidth  - uGw * (iMidColCnt)) / 2.0);
    int    iHOff = (int)((double)(iGetHeight - uGh * (iMidRowCnt)) / 2.0);
    TColor sColor;

    for (int r = 0 ; r < iMidRowCnt ; r++) {
        for (int c = 0 ; c < iMidColCnt ; c++) {

            sColor = 0x00BBFFFF ;

            uY1 = iHOff + r * uGh       + 1;
            uY2 = iHOff + r * uGh + uGh - 1;
            uX1 = iWOff + c * uGw       + 1;
            uX2 = iWOff + c * uGw + uGw - 1;

            ImMidBlk -> Canvas -> Brush -> Color = TColor(sColor );

            ImMidBlk -> Canvas -> Rectangle(uX1 +1 , uY1 +1 , uX2 -1 , uY2 -1);

            iSetWidth  += uY2;
            iSetHeight += uX2;
        }
    }

    ImMidBlk -> Canvas -> TextFlags;
}

//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::MakeLDRFImg()
{
    ImLDRF -> Picture -> Bitmap =NULL; //이미지 초기화


    int uX1, uX2, uY1, uY2;

    int iLDRFMgzBayCnt , iLDRFMgzSlotCnt;

    iLDRFMgzSlotCnt = OM.DevInfo.iLDRFMgzSlotCnt ;
    iLDRFMgzBayCnt  = OM.DevInfo.iLDRFMgzBayCnt  ;

    int iGetWidth  = ImLDRF -> Width ;
    int iGetHeight = ImLDRF -> Height;

    int iSetWidth = 0 , iSetHeight = 0;

    double uGw   = (double)iGetWidth  / (double)(iLDRFMgzBayCnt);
    double uGh   = (double)iGetHeight / (double)(iLDRFMgzSlotCnt);
    int    iWOff = (int)((double)(iGetWidth  - uGw * (iLDRFMgzBayCnt)) / 2.0);
    int    iHOff = (int)((double)(iGetHeight - uGh * (iLDRFMgzSlotCnt)) / 2.0);
    TColor sColor;

    for (int r = 0 ; r < iLDRFMgzSlotCnt ; r++) {
        for (int c = 0 ; c < iLDRFMgzBayCnt ; c++) {

            sColor = 0x00BBFFFF ;

            uY1 = iHOff + r * uGh       + 1;
            uY2 = iHOff + r * uGh + uGh - 1;
            uX1 = iWOff + c * uGw       + 1;
            uX2 = iWOff + c * uGw + uGw - 1;

            ImLDRF -> Canvas -> Brush -> Color = TColor(sColor );

            ImLDRF -> Canvas -> Rectangle(uX1 +1 , uY1 +1 , uX2 -1 , uY2 -1);

            iSetWidth  += uY2;
            iSetHeight += uX2;
        }
    }

    ImLDRF -> Canvas -> TextFlags;
}

//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::MakeLDRRImg()
{
    ImLDRR -> Picture -> Bitmap =NULL; //이미지 초기화


    int uX1, uX2, uY1, uY2;

    int iLDRRMgzBayCnt , iLDRRMgzSlotCnt;

    iLDRRMgzSlotCnt = OM.DevInfo.iLDRRMgzSlotCnt ;
    iLDRRMgzBayCnt  = OM.DevInfo.iLDRRMgzBayCnt  ;

    int iGetWidth  = ImLDRR -> Width ;
    int iGetHeight = ImLDRR -> Height;

    int iSetWidth = 0 , iSetHeight = 0;

    double uGw   = (double)iGetWidth  / (double)(iLDRRMgzBayCnt);
    double uGh   = (double)iGetHeight / (double)(iLDRRMgzSlotCnt);
    int    iWOff = (int)((double)(iGetWidth  - uGw * (iLDRRMgzBayCnt)) / 2.0);
    int    iHOff = (int)((double)(iGetHeight - uGh * (iLDRRMgzSlotCnt)) / 2.0);
    TColor sColor;

    for (int r = 0 ; r < iLDRRMgzSlotCnt ; r++) {
        for (int c = 0 ; c < iLDRRMgzBayCnt ; c++) {

            sColor = 0x00BBFFFF ;

            uY1 = iHOff + r * uGh       + 1 ;
            uY2 = iHOff + r * uGh + uGh - 1 ;
            uX1 = iWOff + c * uGw       + 1 ;
            uX2 = iWOff + c * uGw + uGw - 1 ;

            ImLDRR -> Canvas -> Brush -> Color = TColor(sColor );

            ImLDRR -> Canvas -> Rectangle(uX1 +1, uY1 +1, uX2 -1, uY2 -1);

            iSetWidth  += uY2;
            iSetHeight += uX2;
        }
    }

    ImLDRR -> Canvas -> TextFlags;
}

//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::MakeULDImg()
{
    ImULD -> Picture -> Bitmap =NULL; //이미지 초기화


    int uX1, uX2, uY1, uY2;

    int iULDMgzBayCnt , iULDMgzSlotCnt;

    iULDMgzSlotCnt = OM.DevInfo.iULDMgzSlotCnt ;
    iULDMgzBayCnt  = OM.DevInfo.iULDMgzBayCnt  ;

    int iGetWidth  = ImULD -> Width ;
    int iGetHeight = ImULD -> Height;

    int iSetWidth = 0 , iSetHeight = 0;

    double uGw   = (double)iGetWidth  / (double)(iULDMgzBayCnt);
    double uGh   = (double)iGetHeight / (double)(iULDMgzSlotCnt);
    int    iWOff = (int)((double)(iGetWidth  - uGw * (iULDMgzBayCnt)) / 2.0);
    int    iHOff = (int)((double)(iGetHeight - uGh * (iULDMgzSlotCnt)) / 2.0);
    TColor sColor;

    for (int r = 0 ; r < iULDMgzSlotCnt ; r++) {
        for (int c = 0 ; c < iULDMgzBayCnt ; c++) {

            sColor = 0x00BBFFFF ;

            uY1 = iHOff + r * uGh       + 1;
            uY2 = iHOff + r * uGh + uGh - 1;
            uX1 = iWOff + c * uGw       + 1;
            uX2 = iWOff + c * uGw + uGw - 1;

            ImULD -> Canvas -> Brush -> Color = TColor(sColor );

            ImULD -> Canvas -> Rectangle(uX1 +1 , uY1 +1 , uX2 -1 , uY2 -1);

            iSetWidth  += uY2;
            iSetHeight += uX2;
        }
    }

    ImULD -> Canvas -> TextFlags;
}
















//---------------------------------------------------------------------------


void __fastcall TFrmDeviceSet::btHghtChkClick(TObject *Sender)
{
    Rs232Keyence.CheckHeight  (0) ;
}


void __fastcall TFrmDeviceSet::btConfocalCheckClick(TObject *Sender)
{
    edConfocalCheck -> Text = Confocal.GetHeight() ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::Button1Click(TObject *Sender)
{
    HexaPot.MoveAbsRes(paZ , HEX_Z_LORI);

    HexaPot.Move();
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btFtDispChSettingClick(TObject *Sender)
{
    FrmDispCh -> Show(&Rs232_DisprFt) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btRrDispChSettingClick(TObject *Sender)
{
    FrmDispCh -> Show(&Rs232_DisprRr) ;
}
//---------------------------------------------------------------------------







void __fastcall TFrmDeviceSet::btMBPitchUpClick(TObject *Sender)
{
    MM.SetManCycle(mcLDR_FPitchUp);        
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btMBPitchDnClick(TObject *Sender)
{
    MM.SetManCycle(mcLDR_FPitchDn);        
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btCmsPitchUpClick(TObject *Sender)
{
    MM.SetManCycle(mcLDR_RPitchUp);
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btCmsPitchDnClick(TObject *Sender)
{
    MM.SetManCycle(mcLDR_RPitchDn);
}
//---------------------------------------------------------------------------



void __fastcall TFrmDeviceSet::btULDPitchUpClick(TObject *Sender)
{
    MM.SetManCycle(mcULDPitchUp);        
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btULDPitchDnClick(TObject *Sender)
{
    MM.SetManCycle(mcULDPitchDn);        
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::cbFirstDispSelect(TObject *Sender)
{
    if(cbFirstDisp -> ItemIndex == 0) cbSecDisp -> ItemIndex = 1;
    else                              cbSecDisp -> ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::cbSecDispSelect(TObject *Sender)
{
    if(cbSecDisp -> ItemIndex == 0) cbFirstDisp -> ItemIndex = 1;
    else                            cbFirstDisp -> ItemIndex = 0;
}
//---------------------------------------------------------------------------

