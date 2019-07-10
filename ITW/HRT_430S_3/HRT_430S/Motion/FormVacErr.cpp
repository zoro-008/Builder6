//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormVacErr.h"

#include "Sequence.h"
#include "WorkZone.h"
#include "LotUnit.h"
//#include "SortCmn.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
/*
enum EN_VISN_ERR_STAT { //���� �� �� �ϰ� �ִ� ����.
    vsNomal         = 0 , //ó�� �˻�.
    vsErrReTry      = 1 , //�ٽ� �˻�.
    vsErrSkip       = 2 , //���� ��ŵ.
    vsGood          = 3 ,
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
    WRK.m_iVisnErr = vsErrReTry;
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


