//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "stdlib.h"


#include "FormSingle.h"
//#include "FormSetCon.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmSingle *FrmSingle;
//---------------------------------------------------------------------------
__fastcall TFrmSingle::TFrmSingle(TComponent* Owner)                       // 실행 시킬 때, 한 번만 실행하는 부분
    : TForm(Owner)
{
    int iWidth = (sgDriveTest -> Width-36) / 4;

    sgDriveTest -> ColWidths[0] = 25 ;
    sgDriveTest -> ColWidths[1] = iWidth;
    sgDriveTest -> ColWidths[2] = iWidth;
    sgDriveTest -> ColWidths[3] = iWidth;                        
    sgDriveTest -> ColWidths[4] = iWidth;

    sgTilt      -> ColWidths[0] = 25 ;
    sgTilt      -> ColWidths[1] = iWidth;
    sgTilt      -> ColWidths[2] = iWidth;
    sgTilt      -> ColWidths[3] = iWidth;
    sgTilt      -> ColWidths[4] = iWidth;

    for(int i = 0 ; i < sgDriveTest -> RowCount ; i++) {
                sgDriveTest -> RowHeights[i]  = 28;
        if(i!=0)sgDriveTest -> Cells[0][i] = i ;
    }
    for(int i = 0 ; i < sgTilt -> RowCount ; i++) {
                sgTilt -> RowHeights[i] = 28;
        if(i!=0)sgTilt -> Cells[0][i]   = i ;
    }
    sgDriveTest -> Cells[1][1] = "Start Current"; sgDriveTest -> Cells[1][0] = "Item"   ;
    sgDriveTest -> Cells[1][2] = "Rated Stroke" ; sgDriveTest -> Cells[2][0] = "Measure";
    sgDriveTest -> Cells[1][2] = "Peak Stroke"  ; sgDriveTest -> Cells[2][0] = "Measure";
    sgDriveTest -> Cells[1][3] = "Custom Stroke"; sgDriveTest -> Cells[3][0] = "Spec"   ;
    sgDriveTest -> Cells[1][4] = "Sensitivity"  ; sgDriveTest -> Cells[4][0] = "Result" ;
    sgDriveTest -> Cells[1][5] = "Impedance"    ;
    sgDriveTest -> Cells[1][6] = "Hysteresis"   ; sgTilt      -> Cells[4][0] = "Result" ;

    sgTilt      -> Cells[1][1] = "Initial Tilt" ; sgTilt      -> Cells[1][0] = "Item"   ;
    sgTilt      -> Cells[1][2] = "Drive Tilt"   ; sgTilt      -> Cells[2][0] = "Time"   ;
    sgTilt      -> Cells[1][3] = "Total"        ; sgTilt      -> Cells[3][0] = "Spec"   ;

}
//---------------------------------------------------------------------------


void __fastcall TFrmSingle::ChangeTheModel1Click(TObject *Sender)
{
 //   FrmSetCon -> Show();
}
//---------------------------------------------------------------------------




void __fastcall TFrmSingle::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false;


    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSingle::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate -> Enabled = false;        
}
//---------------------------------------------------------------------------

void __fastcall TFrmSingle::FormHide(TObject *Sender)
{
    tmUpdate -> Enabled = false;        
}
//---------------------------------------------------------------------------

void __fastcall TFrmSingle::FormShow(TObject *Sender)
{
    tmUpdate -> Enabled = true;        
}
//---------------------------------------------------------------------------



void __fastcall TFrmSingle::Close1Click(TObject *Sender)
{
    FrmSingle -> Hide();        
}
//---------------------------------------------------------------------------

