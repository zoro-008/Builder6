//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMasterErr.h"

#include "Sequence.h"
#include "WorkZone.h"
#include "LotUnit.h"
//#include "SortCmn.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
/*
enum EN_VISN_ERR_STAT { //현재 랏 런 하고 있는 상태.
    vsNomal         = 0 , //처음 검사.
    vsErrReTry      = 1 , //다시 검사.
    vsErrSkip       = 2 , //비전 스킵.
    vsGood          = 3 , //정상 일떄.
};

*/
TFrmVisnErr *FrmVisnErr;
//---------------------------------------------------------------------------
__fastcall TFrmVisnErr::TFrmVisnErr(TComponent* Owner)
    : TForm(Owner)
{


}
//---------------------------------------------------------------------------

void __fastcall TFrmVisnErr::btLDRRetryClick(TObject *Sender)
{
    Frm
    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmVisnErr::btLDRSkipThisPickClick(TObject *Sender)
{
    WRK.m_iVisnErr = vsErrSkip;
    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------


