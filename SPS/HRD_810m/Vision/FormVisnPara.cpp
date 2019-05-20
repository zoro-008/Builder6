//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormVisnPara.h"
#include "FormVisnMain.h"
#include "VOptnMan.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmVisnPara *FrmVisnPara;
//---------------------------------------------------------------------------
__fastcall TFrmVisnPara::TFrmVisnPara(TComponent* Owner)
        : TForm(Owner)
{
    Left = 100 ;
    Top  = 100 ;

    tsWfr     -> TabVisible = false ; //tsWfr -> PageIndex == _iPage ;
    tsWfo     -> TabVisible = false ; //tsWfo -> PageIndex == _iPage ;
    tsWfp     -> TabVisible = false ; //tsWfp -> PageIndex == _iPage ;
    tsCal     -> TabVisible = false ; //tsCal -> PageIndex == _iPage ;
    GroupBox1 ->    Visible = false;
    GroupBox2 ->    Visible = false;
    GroupBox3 ->    Visible = false;

    cbWfrFlipX -> Checked = true ;  cbWfoFlipX -> Checked = true ;
    cbWfrFlipY -> Checked = false;  cbWfoFlipY -> Checked = false;
    cbWfrFlipT -> Checked = true ;  cbWfoFlipT -> Checked = false;

    cbWfpFlipX -> Checked = true ;
    cbWfpFlipY -> Checked = false;



}
//---------------------------------------------------------------------------

void TFrmVisnPara::ApplyPara(bool _bToTable)
{

    if(_bToTable) {
        edWfrWidth      -> Text     = VOM.VDevOptn.fWfrWidth     ;
        edWfrHeight     -> Text     = VOM.VDevOptn.fWfrHeight    ;
        edWfrGap        -> Text     = VOM.VDevOptn.fWfrGap       ;
        edWfrAngle      -> Text     = VOM.VDevOptn.fWfrAngle     ;

        edWfrCCPxCnt    -> Text     = VOM.VDevOptn.iWfrCCPxCnt   ;
        edWfrOriSinc    -> Text     = VOM.VDevOptn.fWfrOriSinc   ;

        cbWfrFlipX      -> Checked  = VOM.VCmnOptn.bWfrFlipX     ;
        cbWfrFlipY      -> Checked  = VOM.VCmnOptn.bWfrFlipY     ;
        cbWfrFlipT      -> Checked  = VOM.VCmnOptn.bWfrFlipT     ;

        edWfoOriSinc    -> Text     = VOM.VDevOptn.fWfoOriSinc   ;

        edWfpPxCnt      -> Text     = VOM.VDevOptn.iWfpPxCnt     ;
        sbWfpPx         -> Position = VOM.VDevOptn.iWfpPxHigh    ;


        cbWfoFlipX      -> Checked = VOM.VCmnOptn.bWfoFlipX     ;
        cbWfoFlipY      -> Checked = VOM.VCmnOptn.bWfoFlipY     ;
        cbWfoFlipT      -> Checked = VOM.VCmnOptn.bWfoFlipT     ;


        cbWfpFlipX      -> Checked = VOM.VCmnOptn.bWfpFlipX     ;
        cbWfpFlipY      -> Checked = VOM.VCmnOptn.bWfpFlipY     ;

        cbWfrImageSave  -> ItemIndex = VOM.VCmnOptn.iWfrImageSave ;
        cbWfoImageSave  -> ItemIndex = VOM.VCmnOptn.iWfoImageSave ;
        cbWfpImageSave  -> ItemIndex = VOM.VCmnOptn.iWfpImageSave ;
        cbCalImageSave  -> ItemIndex = VOM.VCmnOptn.iCalImageSave ;

        cbWfrIgnRctInsp -> Checked = VOM.VDevOptn.bWfrIgnRctInsp ;
        cbWfoIgnRctInsp -> Checked = VOM.VDevOptn.bWfoIgnRctInsp ;
    }
    else {
        int iTemp ;
        bool bTemp ;
        double dTemp ;
        float fTemp ;

        //float형으로 남길시.... 소수점이 정확하게 나오지 않는 현상때문에 Double 형으로 비교함...JS
        dTemp = StrToFloatDef(edWfrWidth     -> Text , 9  ); if (VOM.VDevOptn.fWfrWidth     != dTemp) { Trace("Operator" , "Both VOM_WfrWidth values are different."); }
        dTemp = StrToFloatDef(edWfrHeight    -> Text , 9  ); if (VOM.VDevOptn.fWfrHeight    != dTemp) { Trace("Operator" , "Both VOM_WfrHeight values are different."); }
        dTemp = StrToFloatDef(edWfrGap       -> Text , 1  ); if (VOM.VDevOptn.fWfrGap       != dTemp) { Trace("Operator" , "Both VOM_WfrGap values are different."); }
        dTemp = StrToFloatDef(edWfrAngle     -> Text , 1  ); if (VOM.VDevOptn.fWfrAngle     != dTemp) { Trace("Operator" , "Both VOM_WfrAngle values are different."); }

        bTemp = cbWfrFlipX     -> Checked                  ; if (VOM.VCmnOptn.bWfrFlipX     != bTemp) { Trace("Operator" , "Both VOM_WfrFlipX values are different."); }
        bTemp = cbWfrFlipY     -> Checked                  ; if (VOM.VCmnOptn.bWfrFlipY     != bTemp) { Trace("Operator" , "Both VOM_WfrFlipY values are different."); }
        bTemp = cbWfrFlipT     -> Checked                  ; if (VOM.VCmnOptn.bWfrFlipT     != bTemp) { Trace("Operator" , "Both VOM_WfrFlipT values are different."); }
        dTemp = StrToFloatDef(edWfrOriSinc   -> Text , 90 ); if (VOM.VDevOptn.fWfrOriSinc   != dTemp) { Trace("Operator" , "Both VOM_WfrOriSinc values are different."); }


        dTemp = StrToFloatDef(edWfoOriSinc   -> Text , 90 ); if (VOM.VDevOptn.fWfoOriSinc   != dTemp) { Trace("Operator" , "Both VOM_WfoOriSinc values are different."); }

        iTemp = StrToIntDef  (edWfpPxCnt     -> Text , 100); if (VOM.VDevOptn.iWfpPxCnt     != iTemp) { Trace("Operator" , "Both VOM_WfpPxCnt values are different."); }
        iTemp = sbWfpPx        -> Position                 ; if (VOM.VDevOptn.iWfpPxHigh    != iTemp) { Trace("Operator" , "Both VOM_WfpPxHigh values are different."); }

        bTemp = cbWfoFlipX     -> Checked                  ; if (VOM.VCmnOptn.bWfoFlipX     != bTemp) { Trace("Operator" , "Both VOM_WfoFlipX values are different."); }
        bTemp = cbWfoFlipY     -> Checked                  ; if (VOM.VCmnOptn.bWfoFlipY     != bTemp) { Trace("Operator" , "Both VOM_WfoFlipY values are different."); }
        bTemp = cbWfoFlipT     -> Checked                  ; if (VOM.VCmnOptn.bWfoFlipT     != bTemp) { Trace("Operator" , "Both VOM_WfoFlipY values are different."); }

        bTemp = cbWfpFlipX     -> Checked                  ; if (VOM.VCmnOptn.bWfpFlipX     != bTemp) { Trace("Operator" , "Both VOM_WfpFlipX values are different."); }
        bTemp = cbWfpFlipY     -> Checked                  ; if (VOM.VCmnOptn.bWfpFlipY     != bTemp) { Trace("Operator" , "Both VOM_WfpFlipY values are different."); }

        iTemp = cbWfrImageSave -> ItemIndex                ; if (VOM.VCmnOptn.iWfrImageSave != iTemp) { Trace("Operator" , "Both VOM_WfrImageSave values are different."); }
        iTemp = cbWfoImageSave -> ItemIndex                ; if (VOM.VCmnOptn.iWfoImageSave != iTemp) { Trace("Operator" , "Both VOM_WfoImageSave values are different."); }
        iTemp = cbWfpImageSave -> ItemIndex                ; if (VOM.VCmnOptn.iWfpImageSave != iTemp) { Trace("Operator" , "Both VOM_WfpImageSave values are different."); }
        iTemp = cbCalImageSave -> ItemIndex                ; if (VOM.VCmnOptn.iCalImageSave != iTemp) { Trace("Operator" , "Both VOM_CalImageSave values are different."); }

        bTemp = cbWfrIgnRctInsp-> Checked                  ; if (VOM.VDevOptn.bWfrIgnRctInsp!= bTemp) { Trace("Operator" , "Both VOM_bWfrIgnRctInsp values are different."); }
        bTemp = cbWfoIgnRctInsp-> Checked                  ; if (VOM.VDevOptn.bWfoIgnRctInsp!= bTemp) { Trace("Operator" , "Both VOM_bWfoIgnRctInsp values are different."); }


        VOM.VDevOptn.fWfrWidth     = StrToFloatDef(edWfrWidth     -> Text , 9  );
        VOM.VDevOptn.fWfrHeight    = StrToFloatDef(edWfrHeight    -> Text , 9  );
        VOM.VDevOptn.fWfrGap       = StrToFloatDef(edWfrGap       -> Text , 1  );
        VOM.VDevOptn.fWfrAngle     = StrToFloatDef(edWfrAngle     -> Text , 1  );

        VOM.VCmnOptn.bWfrFlipX     = cbWfrFlipX -> Checked                      ;
        VOM.VCmnOptn.bWfrFlipY     = cbWfrFlipY -> Checked                      ;
        VOM.VCmnOptn.bWfrFlipT     = cbWfrFlipT -> Checked                      ;
        VOM.VDevOptn.fWfrOriSinc   = StrToFloatDef(edWfrOriSinc   -> Text , 90 );

        VOM.VDevOptn.fWfoOriSinc   = StrToFloatDef(edWfoOriSinc   -> Text , 90 );

        VOM.VDevOptn.iWfpPxCnt     = StrToIntDef  (edWfpPxCnt     -> Text , 100);
        VOM.VDevOptn.iWfpPxHigh    = sbWfpPx -> Position ; 

        VOM.VDevOptn.iWfrCCPxCnt   = StrToIntDef  (edWfrCCPxCnt   -> Text , 0  );


        VOM.VCmnOptn.bWfoFlipX     = cbWfoFlipX      -> Checked                 ;
        VOM.VCmnOptn.bWfoFlipY     = cbWfoFlipY      -> Checked                 ;
        VOM.VCmnOptn.bWfoFlipT     = cbWfoFlipT      -> Checked                 ;


        VOM.VCmnOptn.bWfpFlipX     = cbWfpFlipX      -> Checked                 ;
        VOM.VCmnOptn.bWfpFlipY     = cbWfpFlipY      -> Checked                 ;

        VOM.VCmnOptn.iWfrImageSave = cbWfrImageSave -> ItemIndex ;
        VOM.VCmnOptn.iWfoImageSave = cbWfoImageSave -> ItemIndex ;
        VOM.VCmnOptn.iWfpImageSave = cbWfpImageSave -> ItemIndex ;
        VOM.VCmnOptn.iCalImageSave = cbCalImageSave -> ItemIndex ;

        VOM.VDevOptn.bWfoIgnRctInsp = cbWfoIgnRctInsp -> Checked ;
        VOM.VDevOptn.bWfrIgnRctInsp = cbWfrIgnRctInsp -> Checked ;

        ApplyPara(true);
    }
}                                                                        


void __fastcall TFrmVisnPara::btSaveClick(TObject *Sender)
{
    ApplyPara(false);
    VOM.LoadVDevOptn(false);
    VOM.LoadVCmnOptn(false);
    Trace("Operator", "FormVisionPara Form_Save Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmVisnPara::FormShow(TObject *Sender)
{

    ApplyPara(true);
}
//---------------------------------------------------------------------------

void TFrmVisnPara::FormShow(int _iPage)
{
    pcVisnPara -> ActivePageIndex = _iPage ;
    tsWfr ->TabVisible = false ; //tsWfr -> PageIndex == _iPage ;
    tsWfo ->TabVisible = false ; //tsWfo -> PageIndex == _iPage ;
    tsWfp ->TabVisible = false ; //tsWfp -> PageIndex == _iPage ;
    tsCal ->TabVisible = false ; //tsCal -> PageIndex == _iPage ;

    Show();

    ApplyPara(true);
}



void __fastcall TFrmVisnPara::FormDestroy(TObject *Sender)
{
    static int i = 0 ;
    Trace("",AnsiString(i).c_str());
    i++;
}
//---------------------------------------------------------------------------

void __fastcall TFrmVisnPara::sbWfpPxChange(TObject *Sender)
{
    lbWfpPx -> Caption = sbWfpPx -> Position ;
    FrmVisnMain -> FrmImg[pcVisnPara->ActivePageIndex] -> sbThresh -> Position = sbWfpPx -> Position ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmVisnPara::sbWfpPxExit(TObject *Sender)
{
    FrmVisnMain -> FrmImg[pcVisnPara->ActivePageIndex] -> btGray -> Down = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmVisnPara::sbWfpPxEnter(TObject *Sender)
{
    FrmVisnMain -> FrmImg[pcVisnPara->ActivePageIndex] -> sbThresh -> Position= sbWfpPx -> Position ;
    FrmVisnMain -> FrmImg[pcVisnPara->ActivePageIndex] -> btBinary -> Down = true ;
}
//---------------------------------------------------------------------------

