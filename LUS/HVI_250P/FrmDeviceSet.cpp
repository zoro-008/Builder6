//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmDeviceSet.h"
//---------------------------------------------------------------------------
#include "SlogUnit.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "UtilDefine.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDeviceSet *FormDeviceSet;
//---------------------------------------------------------------------------
__fastcall TFormDeviceSet::TFormDeviceSet(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormDeviceSet::btSaveDeviceClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    UpdateDevInfo(toBuff);
    OM.SaveDevInfo(OM.m_sCrntDev);

    UpdateDevOptn(toBuff);
    OM.SaveDevOptn(OM.m_sCrntDev);
    
//    DM.ARAY[riLDR].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);
    DM.ARAY[riPRB].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riPRI].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riINB].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riPSI].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
}
//---------------------------------------------------------------------------
void __fastcall TFormDeviceSet::UpdateDevInfo(bool bTable)
{
    if(bTable) {
        edColPitch       -> Text = OM.DevInfo.dColPitch     ;
        edRowPitch       -> Text = OM.DevInfo.dRowPitch     ;

        edColGroupCount  -> Text = OM.DevInfo.iColGrCnt     ;
        edRowGroupCount  -> Text = OM.DevInfo.iRowGrCnt     ;

        edColGroupGap    -> Text = OM.DevInfo.dColGrGap     ;
        edRowGroupGap    -> Text = OM.DevInfo.dRowGrGap     ;

        edColCount       -> Text = OM.DevInfo.iColCnt       ;
        edRowCount       -> Text = OM.DevInfo.iRowCnt       ;

        edCsSlotCnt      -> Text = OM.DevInfo.iCsSlCnt      ;
        edCsSlotPitch    -> Text = OM.DevInfo.dCsSlPitch    ;
//        ed1IdxReWorkCnt  -> Text = OM.DevInfo.i1IdxReWorkCnt;

    }
    else {
        OM.DevInfo.dColPitch      = edColPitch       -> Text.ToDouble() ;
        OM.DevInfo.dRowPitch      = edRowPitch       -> Text.ToDouble() ;

        OM.DevInfo.iColGrCnt      = edColGroupCount  -> Text.ToInt()    ;
        OM.DevInfo.iRowGrCnt      = edRowGroupCount  -> Text.ToInt()    ;

        OM.DevInfo.dColGrGap      = edColGroupGap    -> Text.ToDouble() ;
        OM.DevInfo.dRowGrGap      = edRowGroupGap    -> Text.ToDouble() ;

        OM.DevInfo.iColCnt        = edColCount       -> Text.ToInt()    ;
        OM.DevInfo.iRowCnt        = edRowCount       -> Text.ToInt()    ;

        OM.DevInfo.iCsSlCnt       = edCsSlotCnt      -> Text.ToInt()    ;
        OM.DevInfo.dCsSlPitch     = edCsSlotPitch    -> Text.ToDouble() ;
//        OM.DevInfo.i1IdxReWorkCnt = ed1IdxReWorkCnt  -> Text.ToIntDef(1);

    }

}
void __fastcall TFormDeviceSet::UpdateDevOptn(bool bTable) //¹Ì¾áÇÏ´Ù ³Î ÀÌ»ç½ÃÄÖ´Ù...dd;
{
    if(bTable) {
        edLotEndMgzCnt  -> Text   = OM.DevOptn.iLotEndMgzCnt ;
        edTrimColCnt    -> Text   = OM.DevOptn.iTrimColCnt   ;
        ed1IdxReWorkCnt -> Text   = OM.DevOptn.i1IdxReWorkCnt;
        ed2IdxReWorkCnt -> Text   = OM.DevOptn.i2IdxReWorkCnt;
    }
    else {
        OM.DevOptn.iLotEndMgzCnt  = edLotEndMgzCnt   -> Text.ToIntDef(0) ;
        OM.DevOptn.iTrimColCnt    = edTrimColCnt     -> Text.ToIntDef(0) ;
        OM.DevOptn.i1IdxReWorkCnt = ed1IdxReWorkCnt  -> Text.ToIntDef(1) ;
        OM.DevOptn.i2IdxReWorkCnt = ed2IdxReWorkCnt  -> Text.ToIntDef(1) ;
    }

}
void __fastcall TFormDeviceSet::FormShow(TObject *Sender)
{
    UpdateDevInfo (toTabl);
    UpdateDevOptn (toTabl);
}
//---------------------------------------------------------------------------

