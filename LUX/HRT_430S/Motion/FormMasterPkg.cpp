//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMasterPkg.h"

#include "ManualMan.h"
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
TFrmMstPkg *TFrmMstPkg;
//---------------------------------------------------------------------------
__fastcall TFrmMstPkg::TFrmMstPkg(TComponent* Owner)
    : TForm(Owner)
{


}
//---------------------------------------------------------------------------
void __fastcall TFrmMstPkg::btInspMstPkgClick(TObject *Sender)
{
    MM.SetManCycle(mcETC_MasterInsp);
}
//---------------------------------------------------------------------------
