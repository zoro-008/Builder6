//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDeviceSet.h"

//---------------------------------------------------------------------------
#include "Loader.h"
#include "LoaderSupply.h"
#include "LoaderStock.h"

#include "Stage.h"
#include "StageVT.h"
#include "StageOST.h"
#include "StageVision.h"

#include "Unloader.h"
#include "UnloaderSupply.h"
#include "UnloaderStock.h"

#include "Sort.h"
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
        //����������â.
        sMotrPanel = "pnMotrPos" + AnsiString(i) ;
        pMotrPos = ((TPanel*)FindComponent(sMotrPanel)) ;
        if(pMotrPos != NULL) PM.SetWindow(pMotrPos , i);

        //����
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

    //�Ǹ��� â.
    TPanel * pActr ;

     for(int i=0 ; i<MAX_ACTR ; i++) {
        pActr = ((TPanel*)FindComponent("pnAtcr" + AnsiString(i))) ;
        FraCylinder[i] = new TFraCylOneBt(pActr);

        switch(i){
            default :                                                                                            break;
            case 0  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , LSP.CheckSafe ); break;  //aiLSP_GuidFwBw
            case 1  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , LST.CheckSafe ); break;  //aiLST_StckUpDn
            case 2  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , LDR.CheckSafe ); break;  //aiLDR_IndxUpDn
            case 3  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , SRT.CheckSafe ); break;  //aiSRT_FrstDnUp
            case 4  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , SRT.CheckSafe ); break;  //aiSRT_ScndDnUp
            case 5  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , SRT.CheckSafe ); break;  //aiSRT_ThrdDnUp
            case 6  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , SRT.CheckSafe ); break;  //aiSRT_FrthDnUp
            case 7  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , OST.CheckSafe ); break;  //aiOST_PshrDnUp
            case 8  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , OST.CheckSafe ); break;  //aiOST_PTolUpDn
            case 9  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , VTI.CheckSafe ); break;  //aiVTI_PshrDnUp
            case 10 : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , VTI.CheckSafe ); break;  //aiVTI_PTolUpDn
            case 11 : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , SRT.CheckSafe ); break;  //aiSRT_AlgnFwBw
            case 12 : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , USP.CheckSafe ); break;  //aiUSP_GuidFwBw
            case 13 : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , UST.CheckSafe ); break;  //aiUST_StckUpDn
            case 14 : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , ULD.CheckSafe ); break;  //aiULD_IndxUpDn
        }
        /*                                                                                                       
             if(aiLSP_GuidFwBw)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , LSP.CheckSafe );  //
        else if(aiLST_StckUpDn)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , LST.CheckSafe );  //
        else if(aiLDR_IndxUpDn)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , LDR.CheckSafe );  //

        else if(aiSRT_FrstDnUp)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , SRT.CheckSafe );  //
        else if(aiSRT_ScndDnUp)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , SRT.CheckSafe );  //
        else if(aiSRT_ThrdDnUp)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , SRT.CheckSafe );  //
        else if(aiSRT_FrthDnUp)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , SRT.CheckSafe );  //
        else if(aiSRT_AlgnFwBw)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , SRT.CheckSafe );  //

        else if(aiOST_PshrDnUp)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , OST.CheckSafe );  //                                                               =
        else if(aiOST_PTolUpDn)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , OST.CheckSafe );  //
        else if(aiVTI_PshrDnUp)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , VTI.CheckSafe );  //
        else if(aiVTI_PTolUpDn)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , VTI.CheckSafe );  //

        else if(aiUSP_GuidFwBw)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , USP.CheckSafe );  //
        else if(aiUST_StckUpDn)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , UST.CheckSafe );  //
        else if(aiULD_IndxUpDn)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , ULD.CheckSafe );  //
        */
        FraCylinder[i] ->Show();
    }

    foLDRAirBlow = new TFraOutput(); foLDRAirBlow -> SetConfig(yLDR_AirBlow    , IO_GetYName(yLDR_AirBlow   ), pnLDR1);
                                                                                                                                            
    foSRTVcc1    = new TFraOutput(); foSRTVcc1    -> SetConfig(ySRT_Vcc1       , IO_GetYName(ySRT_Vcc1      ), pnSRT1 );                    
    foSRTEjct1   = new TFraOutput(); foSRTEjct1   -> SetConfig(ySRT_Ejct1      , IO_GetYName(ySRT_Ejct1     ), pnSRT2 );                    
    foSRTVcc2    = new TFraOutput(); foSRTVcc2    -> SetConfig(ySRT_Vcc2       , IO_GetYName(ySRT_Vcc2      ), pnSRT3 );                    
    foSRTEjct2   = new TFraOutput(); foSRTEjct2   -> SetConfig(ySRT_Ejct2      , IO_GetYName(ySRT_Ejct2     ), pnSRT4 );                    
    foSRTVcc3    = new TFraOutput(); foSRTVcc3    -> SetConfig(ySRT_Vcc3       , IO_GetYName(ySRT_Vcc3      ), pnSRT5 );
    foSRTEjct3   = new TFraOutput(); foSRTEjct3   -> SetConfig(ySRT_Ejct3      , IO_GetYName(ySRT_Ejct3     ), pnSRT6 );
    foSRTVcc4    = new TFraOutput(); foSRTVcc4    -> SetConfig(ySRT_Vcc4       , IO_GetYName(ySRT_Vcc4      ), pnSRT7 );
    foSRTEjct4   = new TFraOutput(); foSRTEjct4   -> SetConfig(ySRT_Ejct4      , IO_GetYName(ySRT_Ejct4     ), pnSRT8 );
    foSRTVcc     = new TFraOutput(); foSRTVcc     -> SetConfig(ySRT_Vcc        , IO_GetYName(ySRT_Vcc       ), pnSRT9 );
    foSRTAlgnAir = new TFraOutput(); foSRTAlgnAir -> SetConfig(ySRT_AlgnAirBlw , IO_GetYName(ySRT_AlgnAirBlw), pnSRT10);

    //DevInfo.
    /*�ù� ������̽� ������ �ȉ�.... ������Ʈ�� ���� �ؾ� �ҵ�...
    edColPitch      -> UseMinus = false ;
    edRowPitch      -> UseMinus = false ;
    edColGrCnt      -> UseMinus = false ; edColGrCnt  -> UseApoint = true  ;
    edColGrPitch    -> UseMinus = false ;
    edColCnt        -> UseMinus = false ; edColCnt    -> UseApoint = true  ;
    edRowCnt        -> UseMinus = false ; edRowCnt    -> UseApoint = true  ;

    edCsSlotCnt     -> UseMinus = false ; edCsSlotCnt -> UseApoint = true  ;
    edCsSlotPitch   -> UseMinus = false ;

    edZigColCnt     -> UseMinus = false ; edZigColCnt -> UseApoint = true  ;
    edZigRowCnt     -> UseMinus = false ; edZigRowCnt -> UseApoint = true  ;
    edZigColPitch   -> UseMinus = false ;
    edZigRowPitch   -> UseMinus = false ;

    edClipColGrPitch-> UseMinus = false ;
    edClipColPitch  -> UseMinus = false ;

    //Option
    edLotEndMgzCnt  -> UseMinus = false ; edLotEndMgzCnt -> UseApoint = true  ;
    */


    pcDeviceSet -> ActivePageIndex = 0 ;

    pcSorter -> ActivePageIndex = 0 ;


}

//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btSavePosClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;
    /*
    if(LTQ.GetLotOpen()){
        if(StrToIntDef(edLotEndMgzCnt-> Text , 1) != OM.DevOptn.iLotEndMgzCnt ) {
            edLotEndMgzCnt-> Text = OM.DevOptn.iLotEndMgzCnt ;
            FM_MsgOk("ERR","�� �����߿��� �ް���ī��Ʈ�� �ٲܼ� �����ϴ�.");
        }
    }
    else {
        LDR._iMgzCnt = edLotEndMgzCnt-> Text.ToIntDef(1) ; //���Ҷ� �̷��� �Ǿ�� Lot�� ť���� �����´�.
    }
    //kyekye
    */




    //GetLaserDataFromLotNot(AnsiString _sFormat , AnsiString _sLotNo)

    UpdateDevInfo(toBuff);
    OM.SaveDevInfo(OM.GetCrntDev());

    UpdateDevOptn(toBuff);
    OM.SaveDevOptn(OM.GetCrntDev());

    PM.UpdatePstn(toBuff);
    PM.Save(OM.GetCrntDev());

    DM.ARAY[riLDR].SetMaxColRow(OM.DevInfo.iLDRColCnt    , OM.DevInfo.iLDRRowCnt  ); DM.ARAY[riLDR].SetLotNo("");
    DM.ARAY[riPSL].SetMaxColRow(OM.DevInfo.iLDRColCnt    , OM.DevInfo.iLDRRowCnt  ); DM.ARAY[riPSL].SetLotNo("");
    DM.ARAY[riPRL].SetMaxColRow(OM.DevInfo.iLDRColCnt    , OM.DevInfo.iLDRRowCnt  ); DM.ARAY[riPRL].SetLotNo("");
    DM.ARAY[riULD].SetMaxColRow(OM.DevInfo.iULDColCnt    , OM.DevInfo.iULDRowCnt  ); DM.ARAY[riULD].SetLotNo("");
    DM.ARAY[riPRU].SetMaxColRow(OM.DevInfo.iULDColCnt    , OM.DevInfo.iULDRowCnt  ); DM.ARAY[riPRU].SetLotNo("");
    DM.ARAY[riPSU].SetMaxColRow(OM.DevInfo.iULDColCnt    , OM.DevInfo.iULDRowCnt  ); DM.ARAY[riPSU].SetLotNo("");
    DM.ARAY[riRJ1].SetMaxColRow(OM.DevInfo.iULDColCnt    , OM.DevInfo.iULDRowCnt  ); DM.ARAY[riRJ1].SetLotNo("");
    DM.ARAY[riRJ2].SetMaxColRow(OM.DevInfo.iULDColCnt    , OM.DevInfo.iULDRowCnt  ); DM.ARAY[riRJ2].SetLotNo("");
    DM.ARAY[riRJ3].SetMaxColRow(OM.DevInfo.iULDColCnt    , OM.DevInfo.iULDRowCnt  ); DM.ARAY[riRJ3].SetLotNo("");
    DM.ARAY[riRJ4].SetMaxColRow(OM.DevInfo.iULDColCnt    , OM.DevInfo.iULDRowCnt  ); DM.ARAY[riRJ4].SetLotNo("");
    DM.ARAY[riZIG].SetMaxColRow(MAX_TOOL_SOCK_COL        , 1                      ); DM.ARAY[riZIG].SetLotNo("");
    DM.ARAY[riSRT].SetMaxColRow(MAX_TOOL_SOCK_COL        , 1                      ); DM.ARAY[riSRT].SetLotNo("");

    DM.ARAY[riMOS].SetMaxColRow(1                        , MAX_TOOL_SOCK_COL      ); DM.ARAY[riMOS].SetLotNo("");
    DM.ARAY[riMVT].SetMaxColRow(MAX_TOOL_SOCK_COL        , 1                      ); DM.ARAY[riMVT].SetLotNo("");
    DM.ARAY[riMVS].SetMaxColRow(1                        , MAX_TOOL_SOCK_COL      ); DM.ARAY[riMVS].SetLotNo("");

    DM.ARAY[riSTF].SetMaxColRow(MAX_TOOL_SOCK_COL        , 1                      ); DM.ARAY[riSTF].SetLotNo("");
    DM.ARAY[riSTL].SetMaxColRow(1                        , MAX_TOOL_SOCK_COL      ); DM.ARAY[riSTL].SetLotNo("");
    DM.ARAY[riSTB].SetMaxColRow(MAX_TOOL_SOCK_COL        , 1                      ); DM.ARAY[riSTB].SetLotNo("");
    DM.ARAY[riSTR].SetMaxColRow(1                        , MAX_TOOL_SOCK_COL      ); DM.ARAY[riSTR].SetLotNo("");

    PaintStrpInfo();
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::tmUpdateTimerTimer(TObject *Sender)
{                                                                                   
    tmUpdateTimer->Enabled = false;                                                 
    btSaveDevice -> Enabled = !SEQ._bRun ;
    switch ( pcDeviceSet -> ActivePageIndex ) {
        default: break;
        case 0 : break;
        case 1 : break;
        case 2 : break;
        case 3 : break;                                                             
        case 4 : break;                                                             
        case 5 : break;                                                             
        case 6 : break;                                                             
        case 7 : break;
        case 8 : break;
        case 9 : break;                                                             
    }

    //edEmptyStrpCnt -> Enabled = cbEmptyStrpPlace -> Checked ;

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
        edLDRColPitch      -> Text = OM.DevInfo.dLDRColPitch    ;
        edLDRColCnt        -> Text = OM.DevInfo.iLDRColCnt      ;

        edLDRRowPitch      -> Text = OM.DevInfo.dLDRRowPitch    ;
        edLDRRowCnt        -> Text = OM.DevInfo.iLDRRowCnt      ;

        edScktPitch        -> Text = OM.DevInfo.dScktPitch      ;

        edULDColPitch      -> Text = OM.DevInfo.dULDColPitch    ;    // YOU
        edULDColCnt        -> Text = OM.DevInfo.iULDColCnt      ;

        edULDRowPitch      -> Text = OM.DevInfo.dULDRowPitch    ;
        edULDRowCnt        -> Text = OM.DevInfo.iULDRowCnt      ;


    }
    else {
        OM.DevInfo.dLDRColPitch       = StrToFloatDef(edLDRColPitch      -> Text,1.0);
        OM.DevInfo.iLDRColCnt         = StrToIntDef  (edLDRColCnt        -> Text,1  );

        OM.DevInfo.dLDRRowPitch       = StrToFloatDef(edLDRRowPitch      -> Text,1.0);
        OM.DevInfo.iLDRRowCnt         = StrToIntDef  (edLDRRowCnt        -> Text,1  );

        OM.DevInfo.dScktPitch         = StrToFloatDef(edScktPitch        -> Text,1.0);

        OM.DevInfo.dULDColPitch       = StrToFloatDef(edULDColPitch      -> Text,1.0);      // YOU
        OM.DevInfo.iULDColCnt         = StrToIntDef  (edULDColCnt        -> Text,1  );

        OM.DevInfo.dULDRowPitch       = StrToFloatDef(edULDRowPitch      -> Text,1.0);
        OM.DevInfo.iULDRowCnt         = StrToIntDef  (edULDRowCnt        -> Text,1  );


    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevOptn(bool bTo)
{


    if(bTo){
        edPickDealy -> Text      = OM.DevOptn.iPickDelay;
        edPlceDelay -> Text      = OM.DevOptn.iPlceDelay;



    }
    else{
        OM.DevOptn.iPickDelay = StrToIntDef(edPickDealy-> Text , 100  ) ;
        OM.DevOptn.iPlceDelay = StrToIntDef(edPlceDelay-> Text , 100 ) ;

    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormShow(TObject *Sender)
{
    UpdateDevInfo (toTabl);
    UpdateDevOptn (toTabl);
    PM.UpdatePstn (toTabl);

    PaintStrpInfo();

    tmUpdateTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::pcDeviceSetChange(TObject *Sender)
{
    switch ( pcDeviceSet -> ActivePageIndex ) {
        default:                               break;
        case 0 :                               break;
        case 1 : pnJogUnit -> Parent = pnJog1; break;
        case 2 : pnJogUnit -> Parent = pnJog2; break;
        case 3 : pnJogUnit -> Parent = pnJog3; break;
        case 4 : pnJogUnit -> Parent = pnJog4; break;
        case 5 :                               break;
        case 6 :                               break;
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





void __fastcall TFrmDeviceSet::PaintStrpInfo()
{
    // Loader //

    imArray1 -> Picture -> Bitmap =NULL; //�̹��� �ʱ�ȭ

    int uX1, uX2, uY1, uY2;

    int iColCnt, iRowCnt;

    iRowCnt   = OM.DevInfo.iLDRRowCnt  ;
    iColCnt   = OM.DevInfo.iLDRColCnt  ;

    int iGetWidth  = imArray1 -> Width ;  // �簢������ �̹����ȿ� �� ������ �ϱ� ����.. �׳� ���� ������� �ϸ� �������� �簢������..
    int iGetHeight = imArray1 -> Height ;

    int iSetWidth = 0 , iSetHeight = 0;

    double uGw   = (double)iGetWidth  / (double)(iColCnt);
    double uGh   = (double)iGetHeight / (double)(iRowCnt);
    int    iWOff = (int)((double)(iGetWidth  - uGw * (iColCnt)) / 2.0);
    int    iHOff = (int)((double)(iGetHeight - uGh * (iRowCnt)) / 2.0);
    TColor sColor;


      for (int r = 0 ; r < iRowCnt ; r++) {
        for (int c = 0 ; c < iColCnt ; c++) {

            sColor = 0x00BBFFFF ;


            uY1 = iHOff + r * uGh       + 1 ;
            uY2 = iHOff + r * uGh + uGh - 1 ;
            uX1 = iWOff + c * uGw       + 1 ;
            uX2 = iWOff + c * uGw + uGw - 1 ;


            imArray1 -> Canvas -> Brush -> Color = TColor(sColor );

            imArray1 -> Canvas -> Rectangle(uX1 +1 , uY1 +1 , uX2 -1 , uY2 -1);

            iSetWidth  += uY2;
            iSetHeight += uX2;
        }
    }

    imArray1 -> Canvas -> TextFlags;

    ////////////////////////////////////////////////////////////////////////////

    // UnLoader //

    imArray2 -> Picture -> Bitmap =NULL; //�̹��� �ʱ�ȭ


    //int uX3, uX4, uY3, uY4;

    //int iColCnt2, iRowCnt2;

    iRowCnt    = OM.DevInfo.iULDRowCnt ;
    iColCnt    = OM.DevInfo.iULDColCnt ;

    iGetWidth  = imArray2 -> Width     ;  // �簢������ �̹����ȿ� �� ������ �ϱ� ����.. �׳� ���� ������� �ϸ� �������� �簢������..
    iGetHeight = imArray2 -> Height    ;

    iSetWidth  = 0 , iSetHeight = 0;

    uGw   = (double)iGetWidth  / (double)(iColCnt);
    uGh   = (double)iGetHeight / (double)(iRowCnt);
    iWOff = (int)((double)(iGetWidth  - uGw * (iColCnt)) / 2.0);
    iHOff = (int)((double)(iGetHeight - uGh * (iRowCnt)) / 2.0);
    //TColor sColor2;


      for (int r = 0 ; r < iRowCnt ; r++) {
        for (int c = 0 ; c < iColCnt ; c++) {

            sColor = 0x00BBFFFF ;


            uY1 = iHOff + r * uGh        + 1 ;
            uY2 = iHOff + r * uGh + uGh - 1 ;
            uX1 = iWOff + c * uGw        + 1 ;
            uX2 = iWOff + c * uGw + uGw - 1 ;


            imArray2 -> Canvas -> Brush -> Color = TColor(sColor );

            imArray2 -> Canvas -> Rectangle(uX1 +1 , uY1 +1 , uX2 -1 , uY2 -1);

            iSetWidth  += uY2;
            iSetHeight += uX2;
        }
    }

    imArray2 -> Canvas -> TextFlags;

   // lbColCnt   -> Caption = OM.DevInfo.iColCnt  ;
   // lbRowCnt   -> Caption = OM.DevInfo.iRowCnt  ;


    ////////////////////////////////////////////////////////////////////////////


    // ���� �ҽ� //

    /*

    imArray -> Picture -> Bitmap =NULL; //�̹��� �ʱ�ȭ

    int uX1, uX2, uY1, uY2;

    int iColGrCnt , iRowGrCnt , iColCnt , iRowCnt , iColGrInCnt , iRowGrInCnt;

    iRowGrCnt = OM.DevInfo.iRowGrCnt;
    iColGrCnt = OM.DevInfo.iColGrCnt;
    iRowCnt   = OM.DevInfo.iRowCnt  ;
    iColCnt   = OM.DevInfo.iColCnt  ;

    if(!iColGrCnt) iColGrCnt = 1 ;
    if(!iRowGrCnt) iRowGrCnt = 1 ;

    iColGrInCnt = iColCnt / iColGrCnt;
    iRowGrInCnt = iRowCnt / iRowGrCnt;

    int iGetWidth  = imArray -> Width  -(5 * (iColGrCnt-1));  // �簢������ �̹����ȿ� �� ������ �ϱ� ����.. �׳� ���� ������� �ϸ� �������� �簢������..
    int iGetHeight = imArray -> Height -(5 * (iRowGrCnt-1));

    int iSetWidth = 0 , iSetHeight = 0;

    double uGw   = (double)iGetWidth  / (double)(iColCnt);
    double uGh   = (double)iGetHeight / (double)(iRowCnt);
    int    iWOff = (int)((double)(iGetWidth  - uGw * (iColCnt)) / 2.0);
    int    iHOff = (int)((double)(iGetHeight - uGh * (iRowCnt)) / 2.0);
    TColor sColor;

    int iGrCountR=0 , iGrCountC=0;
    for (int r = 0 ; r < iRowCnt ; r++) {
        for (int c = 0 ; c < iColCnt ; c++) {

            sColor = 0x00BBFFFF ;

            iGrCountR = (r/iRowGrInCnt < iRowGrCnt) ? (5 * (r/iRowGrInCnt)) :5 * (iRowGrCnt-1) ;
            iGrCountC = (c/iColGrInCnt < iColGrCnt) ? (5 * (c/iColGrInCnt)) :5 * (iColGrCnt-1) ;

            uY1 = iHOff + r * uGh       + 1 + iGrCountR;
            uY2 = iHOff + r * uGh + uGh - 1 + iGrCountR;
            uX1 = iWOff + c * uGw       + 1 + iGrCountC;
            uX2 = iWOff + c * uGw + uGw - 1 + iGrCountC;

            imArray -> Canvas -> Brush -> Color = TColor(sColor );

            imArray -> Canvas -> Rectangle(uX1 +1 , uY1 +1 , uX2 -1 , uY2 -1);

            iSetWidth  += uY2;
            iSetHeight += uX2;
        }
    }

    imArray -> Canvas -> TextFlags;

    lbColCnt   -> Caption = OM.DevInfo.iColCnt  ;
    lbRowCnt   -> Caption = OM.DevInfo.iRowCnt  ;
    lbColGrCnt -> Caption = OM.DevInfo.iColGrCnt;
    lbRowGrCnt -> Caption = OM.DevInfo.iRowGrCnt;

    */


}










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


void __fastcall TFrmDeviceSet::Button10Click(TObject *Sender)
{
    TButton * Button  = (TButton *)Sender ;
    int      iIndex =  Button -> Tag ;

    switch(iIndex) {
        case 0 : MM.SetManCycle(mcSTG_Turn90       ) ; break ;
        case 1 : MM.SetManCycle(mcSRT_CyclePickSTG ) ; break ;
        case 2 : MM.SetManCycle(mcSRT_CyclePlceSTG ) ; break ;
        case 3 : MM.SetManCycle(mcSRT_CyclePickZIG ) ; break ;
        case 4 : MM.SetManCycle(mcSRT_CyclePlceZIG ) ; break ;
        case 5 : MM.SetManCycle(mcSRT_CyclePickRJT1) ; break ;
        case 6 : MM.SetManCycle(mcSRT_CyclePlceRJT1) ; break ;
        case 7 : MM.SetManCycle(mcSTG_Turn90       ) ; break ;
        case 8 : MM.SetManCycle(mcSTG_Turn90N      ) ; break ;
        case 9 : MM.SetManCycle(mcSTG_Turn45       ) ; break ;
        case 10: MM.SetManCycle(mcSTG_Turn45N      ) ; break ;
        case 11: MM.SetManCycle(mcSRT_CyclePickRJT4) ; break ;
        case 12: MM.SetManCycle(mcSRT_CyclePlceRJT4) ; break ;
        case 13: MM.SetManCycle(mcSRT_CyclePickLDR ) ; break ;
        case 14: MM.SetManCycle(mcSRT_CyclePlceLDR ) ; break ;
        case 15: MM.SetManCycle(mcSRT_CyclePickULD ) ; break ;
        case 16: MM.SetManCycle(mcSRT_CyclePlceULD ) ; break ;
    }
}
//---------------------------------------------------------------------------






void __fastcall TFrmDeviceSet::Button1Click(TObject *Sender)
{
/*
    pvSRT_YPckrWait        = 0 ,
    pvSRT_YPckrPick            ,
    pvSRT_YPckrZig             ,
    pvSRT_YPckrPocket          ,
    pvSRT_YPckrReject1Stt      ,
    pvSRT_YPckrReject2Stt      ,
    pvSRT_YPckrReject3Stt      ,
    pvSRT_YPckrReject4Stt      ,
    pvSRT_YPckrPlace           ,
*/

    TButton * Button  = (TButton *)Sender ;
    int      iIndex =  Button -> Tag ;

    switch(iIndex) {
        case 0 : MM.SetManCycle(mcSRT_CycleMoveSTG ) ; break ;
        case 1 : MM.SetManCycle(mcSRT_CycleMoveLDR ) ; break ;
        case 2 : MM.SetManCycle(mcSRT_CycleMoveZIG ) ; break ;
        case 3 : MM.SetManCycle(mcSRT_CycleMoveSTG ) ; break ;
        case 4 : MM.SetManCycle(mcSRT_CycleMoveRJT1) ; break ;
//        case 5 : MM.SetManCycle(mcSRT_) ; break ;
//        case 6 : MM.SetManCycle(mcSRT_) ; break ;
        case 7 : MM.SetManCycle(mcSRT_CycleMoveRJT4) ; break ;
        case 8 : MM.SetManCycle(mcSRT_CycleMoveULD ) ; break ;
    }
}
//---------------------------------------------------------------------------
