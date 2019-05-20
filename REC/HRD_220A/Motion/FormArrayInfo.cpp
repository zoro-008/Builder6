//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormArrayInfo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmArrayInfo *FrmArrayInfo;
//---------------------------------------------------------------------------
__fastcall TFrmArrayInfo::TFrmArrayInfo(TComponent* Owner)
        : TForm(Owner)
{


}
//---------------------------------------------------------------------------
void __fastcall TFrmArrayInfo::FormShow(TObject *Sender)
{

     sgInfo -> ColCount = 2 ;
     sgInfo -> RowCount = 9 ;

     sgInfo -> Cells[0][0 ] = "m_sArayName   " ;
     sgInfo -> Cells[0][1 ] = "m_sArayLotNo  " ;
     sgInfo -> Cells[0][2 ] = "m_sArayID     " ;
     sgInfo -> Cells[0][3 ] = "m_iArayStep   " ;
     sgInfo -> Cells[0][4 ] = "m_iAraySubStep" ;

     sgInfo -> Cells[0][5 ] = "m_sChipID     " ;
     sgInfo -> Cells[0][6 ] = "m_iChipStat   " ;

     sgInfo -> Cells[0][7 ] = "m_iCol        " ;
     sgInfo -> Cells[0][8 ] = "m_iRow        " ;




     UpdateVal();

}
//---------------------------------------------------------------------------
void TFrmArrayInfo::UpdateVal()
{
     sgInfo -> Cells[1][0 ] = m_sArayName     ;
     sgInfo -> Cells[1][1 ] = m_sArayLotNo    ;
     sgInfo -> Cells[1][2 ] = m_sArayID       ;
     sgInfo -> Cells[1][3 ] = m_iArayStep     ;
     sgInfo -> Cells[1][4 ] = m_iAraySubStep  ;

     sgInfo -> Cells[1][5 ] = m_sChipID       ;
     sgInfo -> Cells[1][6 ] = m_iChipStat     ;

     sgInfo -> Cells[1][7 ] = m_iCol          ;
     sgInfo -> Cells[1][8 ] = m_iRow          ;

}

