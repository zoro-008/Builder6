//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormOption.h"

#include <FileCtrl.hpp>

#include "Sequence.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOption *FrmOption;
//---------------------------------------------------------------------------
__fastcall TFrmOption::TFrmOption(TComponent* Owner)
    : TForm(Owner)
{
    //for (int i = 0; i < ComponentCount; i++)
    //{
    //    if (Components[i]->ClassNameIs("TEdit"))
    //    {
    //        ((TEdit*)FindComponent(Components[i]->Name))->ImeMode    = imDisable ;
    //        ((TEdit*)FindComponent(Components[i]->Name))->NumberOnly = true ;
    //        ((TEdit*)FindComponent(Components[i]->Name))->UseApoint  = true ;
    //        ((TEdit*)FindComponent(Components[i]->Name))->UseMinus   = true ;
    //    }
    //    else if(Components[i]->ClassNameIs("TPageControl"))
    //    {
    //        ((TPageControl*)FindComponent(Components[i]->Name))->ActivePageIndex = 0 ;
    //    }
    //}
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::btSaveClick(TObject *Sender)
{
    //Check Running Status.
    if (SEQ._bRun) {
        FM_MsgTime("Warning" , "�����߿��� ���� �� �� �����ϴ�.!" , 2000);
        return;
    }
    if (FM_MsgYesNo("Confirm" , "���� �ϻ�ڽ��ϱ�?") != mrYes)  return;

    UpdateComOptn(toBuff);
    OM.SaveCmnOptn();

    Trace("Operator", "FormOption Form_Save Button Click");

//    OM.SaveEqpOptn();
//    UserMsg2("Confirm" , "���� �Ǿ����ϴ�.");
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::UpdateComOptn(bool _bToTable)
{

    if (_bToTable == toTabl) {
        cbDoorSkip      -> Checked = OM.CmnOptn.bDoorSkip           ; //
        cbLoadingStop   -> Checked = OM.CmnOptn.bLoadingStop        ; //Loading Stop
        edVsBfGrabDly   -> Text    = OM.CmnOptn.iVsBfGrabDly        ; //���� �˻��� ������ . ���� ����ȭ ����.
        edServerPath    -> Text    = OM.CmnOptn.sSourcePath         ; //������ �ִ°��� �����´�.
        edWorkedPath    -> Text    = OM.CmnOptn.sWorkedPath         ; //������ �ִ°��� �������� ���.
        edStageTLmtErr  -> Text    = OM.CmnOptn.dStageTLmtErr       ; //�������� ���� ���� ���� ����.
        cbUseBarcode    -> Checked = OM.CmnOptn.bUseBarcode         ; //���ڵ� ��� ����.

        edVisnPenYOft   -> Text    = OM.CmnOptn.dVisnPenYOft        ; //������ ������� Y �����V.
        edPenDnCnt      -> Text    = OM.CmnOptn.iPenDnCnt           ; //ȭ��Ʈ ���� ��üũ�ÿ� ���ڸ����� ��������.
        edPenAtDnDly    -> Text    = OM.CmnOptn.iPenAtDnDly         ; //�� ��ŷ�ÿ� �ٿ� �ϰ� ������.
        edPenDnOverCnt  -> Text    = OM.CmnOptn.iPenDnOverCnt       ; //�� ��ŷ�ÿ� �ٿ� �ϰ� ������. 

        cbLoadOnlyOneMapPath -> Checked = OM.CmnOptn.bLoadOnlyOneMapMarking ;
        edLoadOnlyOneMapPath -> Text    = OM.CmnOptn.sLoadOnlyOneMapMrkPath ;

        cbUse1stBarcode      -> Checked = OM.CmnOptn.bUse1stBarcode;

        cbUseEndCam      -> Checked = OM.CmnOptn.bUseEndCam ;
    }
    else {
        int        iTemp ;
        bool       bTemp ;
        double     dTemp ;
        AnsiString sTemp ;

        bTemp = cbDoorSkip      -> Checked          ; if (OM.CmnOptn.bDoorSkip       != bTemp) { Trace("Operator" , "Both Cmn_DoorSkip values are different."); }
        bTemp = cbLoadingStop   -> Checked          ; if (OM.CmnOptn.bLoadingStop    != bTemp) { Trace("Operator" , "Both Cmn_LoadingStop values are different."); }
        iTemp = edVsBfGrabDly   -> Text.ToIntDef(10); if (OM.CmnOptn.iVsBfGrabDly    != iTemp) { Trace("Operator" , "Both Cmn_VsBfGrabDly values are different."); }
        sTemp = edServerPath    -> Text             ; if (OM.CmnOptn.sSourcePath     != sTemp) { Trace("Operator" , "Both Cmn_SourcePath values are different."); }
        sTemp = edWorkedPath    -> Text             ; if (OM.CmnOptn.sWorkedPath     != sTemp) { Trace("Operator" , "Both Cmn_WorkedPath values are different."); }
        dTemp = edStageTLmtErr  -> Text.ToDouble()  ; if (OM.CmnOptn.dStageTLmtErr   != dTemp) { Trace("Operator" , "Both Cmn_StageTLmtErr values are different."); }
        bTemp = cbUseBarcode    -> Checked          ; if (OM.CmnOptn.bUseBarcode     != bTemp) { Trace("Operator" , "Both Cmn_UseBarcode values are different."); }

        dTemp = edVisnPenYOft   -> Text.ToDouble()  ; if (OM.CmnOptn.dVisnPenYOft    != dTemp) { Trace("Operator" , "Both Cmn_VisnPenYOft values are different."); }
        iTemp = edPenDnCnt      -> Text.ToIntDef(1) ; if (OM.CmnOptn.iPenDnCnt       != iTemp) { Trace("Operator" , "Both Cmn_PenDnCnt values are different."); }
        iTemp = edPenAtDnDly    -> Text.ToIntDef(10); if (OM.CmnOptn.iPenAtDnDly     != iTemp) { Trace("Operator" , "Both Cmn_PenAtDnDly values are different."); }

        sTemp = edLoadOnlyOneMapPath -> Text    ; if (OM.CmnOptn.sLoadOnlyOneMapMrkPath != sTemp) { Trace("Operator" , "Both Cmn_sLoadOnlyOneMapMrkPath values are different."); }
        bTemp = cbLoadOnlyOneMapPath -> Checked ; if (OM.CmnOptn.bLoadOnlyOneMapMarking != bTemp) { Trace("Operator" , "Both Cmn_bLoadOnlyOneMapMarking values are different."); }

        OM.CmnOptn.bDoorSkip       = cbDoorSkip      -> Checked          ; //
        OM.CmnOptn.bLoadingStop    = cbLoadingStop   -> Checked          ; //Loading Stop
        OM.CmnOptn.iVsBfGrabDly    = edVsBfGrabDly   -> Text.ToIntDef(10); //���� �˻��� ������ . ���� ����ȭ ����.
        OM.CmnOptn.sSourcePath     = edServerPath    -> Text             ; //������ �ִ°��� �����´�.
        OM.CmnOptn.sWorkedPath     = edWorkedPath    -> Text             ; //������ �ִ°��� �������� ���.
        OM.CmnOptn.dStageTLmtErr   = edStageTLmtErr  -> Text.ToDouble()  ; //�������� ���� ���� ���� ����.
        OM.CmnOptn.bUseBarcode     = cbUseBarcode    -> Checked          ; //���ڵ� ��� ����.

        OM.CmnOptn.dVisnPenYOft    = edVisnPenYOft   -> Text.ToDouble()  ; //������ ������� Y �����V.
        OM.CmnOptn.iPenDnCnt       = edPenDnCnt      -> Text.ToIntDef(1) ; //ȭ��Ʈ ���� ��üũ�ÿ� ���ڸ����� ��������.
        OM.CmnOptn.iPenAtDnDly     = edPenAtDnDly    -> Text.ToIntDef(10); //�� ��ŷ�ÿ� �ٿ� �ϰ� ������.

        OM.CmnOptn.iPenDnOverCnt   = edPenDnOverCnt  -> Text.ToIntDef(OM.CmnOptn.iPenDnOverCnt); //�� ��ŷ�ÿ� �ٿ� �ϰ� ������.



        OM.CmnOptn.bLoadOnlyOneMapMarking = cbLoadOnlyOneMapPath -> Checked ;
        OM.CmnOptn.sLoadOnlyOneMapMrkPath = edLoadOnlyOneMapPath -> Text    ;

        OM.CmnOptn.bUse1stBarcode         = cbUse1stBarcode      -> Checked ;

        OM.CmnOptn.bUseEndCam      = cbUseEndCam      -> Checked  ;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::FormShow(TObject *Sender)
{
    UpdateComOptn(toTabl);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOption::cbLoadingStopClick(TObject *Sender)
{
    //UpdateComOptn(toBuff);
}
//---------------------------------------------------------------------------




void __fastcall TFrmOption::btOutputFolderClick(TObject *Sender)
{
    String Dir;
    SelectDirectory("Select a directory", "", Dir);
    ShowMessage(Dir);
    edWorkedPath -> Text = Dir ;
    Trace("Operator", "FormOption Form_ Output Folder Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmOption::btServerFolderClick(TObject *Sender)
{
    String Dir;
    SelectDirectory("Select a directory", "", Dir);
    edServerPath -> Text = Dir ;
    Trace("Operator", "FormOption Form_ Server Folder Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmOption::btLoadOnlyOneMapClick(TObject *Sender)
{
    if(!odLoadOnlyOneMapPath->Execute()) return ;

    String sPath = odLoadOnlyOneMapPath -> FileName;
    edLoadOnlyOneMapPath -> Text = sPath ;
    Trace("Operator", "FormOption Form_ Only One Mapfile Load Path Button Click");

}
//---------------------------------------------------------------------------

