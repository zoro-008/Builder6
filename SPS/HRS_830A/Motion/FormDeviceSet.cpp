//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDeviceSet.h"

//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff.h"
#include "Work.h"
#include "PostBuff.h"
#include "UnLoader.h"
//---------------------------------------------------------------------------
#include "LotUnit.h"
#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "ManualMan.h"

#include "Unit1.h"

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

             if(aiPRB_IdxUpDn)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , PRB.CheckSafe );  //
        else if(aiWRK_LIdUpDn)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , WRK.CheckSafe );  //
        else if(aiWRK_RIdUpDn)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , WRK.CheckSafe );  //
        else if(aiPSB_IdxUpDn)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , PSB.CheckSafe );  //
        else if(aiPSB_PshFwBw)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , PSB.CheckSafe );  //
        else if(aiPSB_PshDnUp)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , PSB.CheckSafe );  //
        else if(aiLDR_CmpUpDn)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , LDR.CheckSafe );  //
        else if(aiLDR_PshFwBw)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , LDR.CheckSafe );  //
        else if(aiULD_CmpUpDn)FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , ULD.CheckSafe );  //
        FraCylinder[i] ->Show();
    }

    foLDRMgzInAC  = new TFraOutput(); foLDRMgzInAC  -> SetConfig(yLDR_MgzInAC    ,IO_GetYName(yLDR_MgzInAC   ),pnLDR1);
    foLDRMgzOutAC = new TFraOutput(); foLDRMgzOutAC -> SetConfig(yLDR_MgzOutAC   ,IO_GetYName(yLDR_MgzOutAC  ),pnLDR2);
    foPRBAirBlwOn = new TFraOutput(); foPRBAirBlwOn -> SetConfig(yPRB_AirBlow    ,IO_GetYName(yPRB_AirBlow   ),pnPRB1);
    foWRKEjctOn   = new TFraOutput(); foWRKEjctOn   -> SetConfig(yETC_EjctAirSol ,IO_GetYName(yETC_EjctAirSol),pnWRK1);
    foWRKDspsROn  = new TFraOutput(); foWRKDspsROn  -> SetConfig(yWRK_RDispensor ,IO_GetYName(yWRK_RDispensor),pnWRK2);
    foWRKDspsLOn  = new TFraOutput(); foWRKDspsLOn  -> SetConfig(yWRK_LDispensor ,IO_GetYName(yWRK_LDispensor),pnWRK3);
    foULDMgzInAC  = new TFraOutput(); foULDMgzInAC  -> SetConfig(yULD_MgzOutAC   ,IO_GetYName(yULD_MgzOutAC  ),pnULD1);
    foULDMgzOutAC = new TFraOutput(); foULDMgzOutAC -> SetConfig(yULD_MgzInAC    ,IO_GetYName(yULD_MgzInAC   ),pnULD2);

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


}

//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btSavePosClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;

    if(LTQ.GetLotOpen()){
        if(StrToIntDef(edLotEndMgzCnt-> Text , 1) != OM.DevOptn.iLotEndMgzCnt ) {
            edLotEndMgzCnt-> Text = OM.DevOptn.iLotEndMgzCnt ;
            FM_MsgOk("ERR","�� �����߿��� �ް���ī��Ʈ�� �ٲܼ� �����ϴ�.");
        }
    }
    else {
        LDR._iMgzCnt = edLotEndMgzCnt-> Text.ToIntDef(1) ; //���Ҷ� �̷��� �Ǿ�� Lot�� ť���� �����´�.
    }

    UpdateDevInfo(toBuff);
    OM.SaveDevInfo(OM.GetCrntDev());

    UpdateDevOptn(toBuff);
    OM.SaveDevOptn(OM.GetCrntDev());

    PM.UpdatePstn(toBuff);
    PM.Save(OM.GetCrntDev());

    DM.ARAY[riPRB].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );  
    DM.ARAY[riLWK].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );  
    DM.ARAY[riRWK].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );  
    DM.ARAY[riPSB].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );  
    DM.ARAY[riLDR].SetMaxColRow(1                     , OM.DevInfo.iCsSlCnt   );  
    DM.ARAY[riULD].SetMaxColRow(1                     , OM.DevInfo.iCsSlCnt   );  

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
        edColPitch      -> Text = OM.DevInfo.dColPitch      ;
        edColCnt        -> Text = OM.DevInfo.iColCnt        ;
        edColGrCnt      -> Text = OM.DevInfo.iColGrCnt      ;
        edColGrGap      -> Text = OM.DevInfo.dColGrGap      ;

        edRowPitch      -> Text = OM.DevInfo.dRowPitch      ;
        edRowCnt        -> Text = OM.DevInfo.iRowCnt        ;
        edRowGrCnt      -> Text = OM.DevInfo.iRowGrCnt      ;
        edRowGrGap      -> Text = OM.DevInfo.dRowGrGap      ;

        edCsSlotCnt     -> Text = OM.DevInfo.iCsSlCnt       ;
        edCsSlotPitch   -> Text = OM.DevInfo.dCsSlPitch     ;
    }
    else {
        OM.DevInfo.dColPitch       = StrToFloatDef(edColPitch      -> Text,1.0);
        OM.DevInfo.iColCnt         = StrToIntDef  (edColCnt        -> Text,1  );
        OM.DevInfo.iColGrCnt       = StrToIntDef  (edColGrCnt      -> Text,1  );
        OM.DevInfo.dColGrGap       = StrToFloatDef(edColGrGap      -> Text,1.0);

        OM.DevInfo.dRowPitch       = StrToFloatDef(edRowPitch      -> Text,1.0);
        OM.DevInfo.iRowCnt         = StrToIntDef  (edRowCnt        -> Text,1  );
        OM.DevInfo.iRowGrCnt       = StrToIntDef  (edRowGrCnt      -> Text,1  );
        OM.DevInfo.dRowGrGap       = StrToFloatDef(edRowGrGap      -> Text,1.0);

        OM.DevInfo.iCsSlCnt        = StrToIntDef  (edCsSlotCnt     -> Text,10 );
        OM.DevInfo.dCsSlPitch      = StrToFloatDef(edCsSlotPitch   -> Text,10 );
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevOptn(bool bTo)
{
    if(bTo){
        edLotEndMgzCnt       -> Text      = OM.DevOptn.iLotEndMgzCnt     ;
        edDispsrLTimer       -> Text      = OM.DevOptn.iDispsrLTimer     ;
        edDispsrRTimer       -> Text      = OM.DevOptn.iDispsrRTimer     ;
        edOutPshtDelay       -> Text      = OM.DevOptn.iPSBOutPshrDelay  ;
        edUpDnPshDelay       -> Text      = OM.DevOptn.iPSBUpDnPshrDelay ;
        edDspsWrkCnt         -> Text      = OM.DevOptn.iDispsrCnt        ;
        edDisprSnsrTimer     -> Text      = OM.DevOptn.iDispsrSnsrTimer  ;
    }
    else{
        OM.DevOptn.iLotEndMgzCnt      = StrToIntDef(edLotEndMgzCnt      -> Text , 1  ) ;
        OM.DevOptn.iPSBOutPshrDelay   = StrToIntDef(edOutPshtDelay      -> Text , 200) ;
        OM.DevOptn.iPSBUpDnPshrDelay  = StrToIntDef(edUpDnPshDelay      -> Text , 200) ;
        OM.DevOptn.iDispsrLTimer      = StrToIntDef(edDispsrLTimer      -> Text , 100) ;
        OM.DevOptn.iDispsrRTimer      = StrToIntDef(edDispsrRTimer      -> Text , 100) ;
        OM.DevOptn.iDispsrCnt         = StrToIntDef(edDspsWrkCnt        -> Text , 1  ) ;
        OM.DevOptn.iDispsrSnsrTimer   = StrToIntDef(edDisprSnsrTimer    -> Text , 100) ;
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
        case 5 : pnJogUnit -> Parent = pnJog5; break;
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

}


