//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmDeviceSet.h"
#include "FrmReviewSelect.h"
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

    DM.ARAY[riLDR].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);
    DM.ARAY[riPRB].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riWR1].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riVS1].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riWR2].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riVS2].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riWR3].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riVS3].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riVS4].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riVB4].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riPSB].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riULD].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);

    HED.DpAray1.SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    HED.DpAray2.SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    HED.DpAray3.SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );

    FormReviewSelect->ReadArayData(riVB4);

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

        edInsMrkColCnt   -> Text = OM.DevInfo.iColInspMrkCnt;
        edInsMrkRowCnt   -> Text = OM.DevInfo.iRowInspMrkCnt;

        edInsMrkVsColCnt -> Text = OM.DevInfo.iInsMrkVsColCnt;
        edInsMrkVsRowCnt -> Text = OM.DevInfo.iInsMrkVsRowCnt;


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

        OM.DevInfo.iColInspMrkCnt = edInsMrkColCnt   -> Text.ToInt() ;
        OM.DevInfo.iRowInspMrkCnt = edInsMrkRowCnt   -> Text.ToInt() ;

        OM.DevInfo.iInsMrkVsColCnt= edInsMrkVsColCnt -> Text.ToIntDef(1) ;
        OM.DevInfo.iInsMrkVsRowCnt= edInsMrkVsRowCnt -> Text.ToIntDef(1) ;
    }

}

void __fastcall TFormDeviceSet::FormShow(TObject *Sender)
{
    UpdateDevInfo (toTabl);
}
//---------------------------------------------------------------------------

