//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormErrList.h"
#include "ErrMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmErrList *FrmErrList;
//---------------------------------------------------------------------------
__fastcall TFrmErrList::TFrmErrList(TComponent* Owner)
        : TForm(Owner)
{

    sgErrList -> ColWidths[0] = 50  ;
    sgErrList -> ColWidths[1] = 400 ;
    sgErrList -> ColWidths[2] = 50  ;
    sgErrList -> Cells[0][0] = "Err No"      ;
    sgErrList -> Cells[1][0] = "Err Comment" ;
    sgErrList -> Cells[2][0] = "Err Count"   ;
}



//---------------------------------------------------------------------------
void __fastcall TFrmErrList::FormShow(TObject *Sender)
{
    int iMaxErrCnt = EM.GetMaxErrCnt() ;
    int iErrOcrCnt = 0 ;

    for(int i = 1 ; i < sgErrList -> RowCount ; i++) {
        sgErrList -> Cells[0][i] = "" ;
        sgErrList -> Cells[1][i] = "" ;
        sgErrList -> Cells[2][i] = "" ;
    }

    for( int i = 0 ; i < iMaxErrCnt ; i++) {
        if(EM.GetOcrCnt(i)) iErrOcrCnt++;
    }

    sgErrList -> RowCount = iErrOcrCnt <= 0 ? 2 : iErrOcrCnt + 1 ;
    iErrOcrCnt = 0 ;

    for( int i = 0 ; i < iMaxErrCnt ; i++) {
        if(EM.GetOcrCnt(i)) {
            iErrOcrCnt++;
            sgErrList -> Cells[0][iErrOcrCnt] = i ;
            sgErrList -> Cells[1][iErrOcrCnt] = EM.GetErrCmnt(i) ;
            sgErrList -> Cells[2][iErrOcrCnt] = EM.GetOcrCnt (i) ;
        }
    }
}
//---------------------------------------------------------------------------
