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
#include "Head.h"
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

    OM.DMSetMaxColRow();
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

        edVisnIndexId    -> Text = OM.DevInfo.sVisnIndexId  ;

        edInsColCnt      -> Text = OM.DevInfo.iColInspCnt   ;
        edInsRowCnt      -> Text = OM.DevInfo.iRowInspCnt   ;

        edRowSubGroupCount -> Text = OM.DevInfo.iRowSubGroupCount   ;
        edRowSubGroupGap   -> Text = OM.DevInfo.dRowSubGroupGap     ;

        edColSubGroupCount -> Text = OM.DevInfo.iColSubGroupCount   ;
        edColSubGroupGap   -> Text = OM.DevInfo.dColSubGroupGap     ;

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

        OM.DevInfo.sVisnIndexId   = edVisnIndexId    -> Text            ;

        OM.DevInfo.iColInspCnt    = edInsColCnt      -> Text.ToInt() ;
        OM.DevInfo.iRowInspCnt    = edInsRowCnt      -> Text.ToInt() ;

        OM.DevInfo.iRowSubGroupCount = edRowSubGroupCount -> Text.ToIntDef(0);
        OM.DevInfo.dRowSubGroupGap   = edRowSubGroupGap   -> Text.ToDouble( );

        OM.DevInfo.iColSubGroupCount = edColSubGroupCount -> Text.ToIntDef(0);
        OM.DevInfo.dColSubGroupGap   = edColSubGroupGap   -> Text.ToDouble( );

    }

}

void __fastcall TFormDeviceSet::FormShow(TObject *Sender)
{
    UpdateDevInfo (toTabl);
}
//---------------------------------------------------------------------------

