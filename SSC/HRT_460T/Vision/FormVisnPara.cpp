//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormVisnPara.h"
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









}
//---------------------------------------------------------------------------

void TFrmVisnPara::ApplyPara(bool _bToTable)
{

    if(_bToTable) {
        edWfrWidth     -> Text    = VOM.VDevOptn.fWfrWidth     ;
        edWfrHeight    -> Text    = VOM.VDevOptn.fWfrHeight    ;
        edWfrGap       -> Text    = VOM.VDevOptn.fWfrGap       ;
        edWfrAngle     -> Text    = VOM.VDevOptn.fWfrAngle     ;

        edWfrFCPxCnt   -> Text    = VOM.VDevOptn.iWfrFcpPxCnt  ;
        edWfrCCPxCnt   -> Text    = VOM.VDevOptn.iWfrCCPxCnt   ;
        edWfrStdPxCnt  -> Text    = VOM.VDevOptn.iWfrStdPxCnt  ;

        cbWfrFlipX     -> Checked = VOM.VCmnOptn.bWfrFlipX     ;
        cbWfrFlipY     -> Checked = VOM.VCmnOptn.bWfrFlipY     ;
        cbWfrFlipT     -> Checked = VOM.VCmnOptn.bWfrFlipT     ;

        edWfoSinc      -> Text    = VOM.VDevOptn.fWfoSinc      ;

        edWfpPxCnt     -> Text    = VOM.VDevOptn.iWfpPxCnt     ;


        cbWfoFlipX     -> Checked = VOM.VCmnOptn.bWfoFlipX     ;
        cbWfoFlipY     -> Checked = VOM.VCmnOptn.bWfoFlipY     ;

        cbWfpFlipX     -> Checked = VOM.VCmnOptn.bWfpFlipX     ;
        cbWfpFlipY     -> Checked = VOM.VCmnOptn.bWfpFlipY     ;

        cbWfrImageSave -> ItemIndex = VOM.VCmnOptn.iWfrImageSave ;
        cbWfoImageSave -> ItemIndex = VOM.VCmnOptn.iWfoImageSave ;
        cbWfpImageSave -> ItemIndex = VOM.VCmnOptn.iWfpImageSave ;
        cbCalImageSave -> ItemIndex = VOM.VCmnOptn.iCalImageSave ;


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

        dTemp = StrToFloatDef(edWfoSinc      -> Text , 80 ); if (VOM.VDevOptn.fWfoSinc      != dTemp) { Trace("Operator" , "Both VOM_WfoSinc values are different."); }

        iTemp = StrToIntDef  (edWfpPxCnt     -> Text , 100); if (VOM.VDevOptn.iWfpPxCnt     != iTemp) { Trace("Operator" , "Both VOM_WfpPxCnt values are different."); }

        bTemp = cbWfoFlipX     -> Checked                  ; if (VOM.VCmnOptn.bWfoFlipX     != bTemp) { Trace("Operator" , "Both VOM_WfoFlipX values are different."); }
        bTemp = cbWfoFlipY     -> Checked                  ; if (VOM.VCmnOptn.bWfoFlipY     != bTemp) { Trace("Operator" , "Both VOM_WfoFlipY values are different."); }

        bTemp = cbWfpFlipX     -> Checked                  ; if (VOM.VCmnOptn.bWfpFlipX     != bTemp) { Trace("Operator" , "Both VOM_WfpFlipX values are different."); }
        bTemp = cbWfpFlipY     -> Checked                  ; if (VOM.VCmnOptn.bWfpFlipY     != bTemp) { Trace("Operator" , "Both VOM_WfpFlipY values are different."); }

        iTemp = cbWfrImageSave -> ItemIndex                ; if (VOM.VCmnOptn.iWfrImageSave != iTemp) { Trace("Operator" , "Both VOM_WfrImageSave values are different."); }
        iTemp = cbWfoImageSave -> ItemIndex                ; if (VOM.VCmnOptn.iWfoImageSave != iTemp) { Trace("Operator" , "Both VOM_WfoImageSave values are different."); }
        iTemp = cbWfpImageSave -> ItemIndex                ; if (VOM.VCmnOptn.iWfpImageSave != iTemp) { Trace("Operator" , "Both VOM_WfpImageSave values are different."); }
        iTemp = cbCalImageSave -> ItemIndex                ; if (VOM.VCmnOptn.iCalImageSave != iTemp) { Trace("Operator" , "Both VOM_CalImageSave values are different."); }


        VOM.VDevOptn.fWfrWidth     = StrToFloatDef(edWfrWidth     -> Text , 9  );
        VOM.VDevOptn.fWfrHeight    = StrToFloatDef(edWfrHeight    -> Text , 9  );
        VOM.VDevOptn.fWfrGap       = StrToFloatDef(edWfrGap       -> Text , 1  );
        VOM.VDevOptn.fWfrAngle     = StrToFloatDef(edWfrAngle     -> Text , 1  );

        VOM.VCmnOptn.bWfrFlipX     = cbWfrFlipX -> Checked                      ;
        VOM.VCmnOptn.bWfrFlipY     = cbWfrFlipY -> Checked                      ;
        VOM.VCmnOptn.bWfrFlipT     = cbWfrFlipT -> Checked                      ;

        VOM.VDevOptn.fWfoSinc      = StrToFloatDef(edWfoSinc      -> Text , 80 );

        VOM.VDevOptn.iWfpPxCnt     = StrToIntDef  (edWfpPxCnt     -> Text , 100);

        VOM.VDevOptn.iWfrCCPxCnt   = StrToIntDef  (edWfrCCPxCnt   -> Text , 0  );

        VOM.VDevOptn.iWfrStdPxCnt  = StrToFloatDef(edWfrStdPxCnt  -> Text , 10 );
        VOM.VDevOptn.iWfrFcpPxCnt  = StrToIntDef  (edWfrFCPxCnt   -> Text , 1  );

        VOM.VCmnOptn.bWfoFlipX     = cbWfoFlipX      -> Checked                 ;
        VOM.VCmnOptn.bWfoFlipY     = cbWfoFlipY      -> Checked                 ;

        VOM.VCmnOptn.bWfpFlipX     = cbWfpFlipX      -> Checked                 ;
        VOM.VCmnOptn.bWfpFlipY     = cbWfpFlipY      -> Checked                 ;

        VOM.VCmnOptn.iWfrImageSave = cbWfrImageSave -> ItemIndex ;
        VOM.VCmnOptn.iWfoImageSave = cbWfoImageSave -> ItemIndex ;
        VOM.VCmnOptn.iWfpImageSave = cbWfpImageSave -> ItemIndex ;
        VOM.VCmnOptn.iCalImageSave = cbCalImageSave -> ItemIndex ;

        ApplyPara(true);
    }
}

void __fastcall TFrmVisnPara::btCloseClick(TObject *Sender)
{
    Close();
    Trace("Operator", "FormVisionPara Form_Close Button Click");
}
//---------------------------------------------------------------------------

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
    tsWfr ->TabVisible = tsWfr -> PageIndex == _iPage ;
    tsWfo ->TabVisible = tsWfo -> PageIndex == _iPage ;
    tsWfp ->TabVisible = tsWfp -> PageIndex == _iPage ;
    tsCal ->TabVisible = tsCal -> PageIndex == _iPage ;
    Show();
}



void __fastcall TFrmVisnPara::FormDestroy(TObject *Sender)
{
    static int i = 0 ;
    Trace("",AnsiString(i).c_str());
    i++;
}
//---------------------------------------------------------------------------

