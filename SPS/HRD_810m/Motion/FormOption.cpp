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
        FM_MsgTime("Warning" , "동작중에는 저장 할 수 없습니다.!" , 2000);
        return;
    }
    if (FM_MsgYesNo("Confirm" , "저장 하사겠습니까?") != mrYes)  return;

    UpdateComOptn(toBuff);
    OM.SaveCmnOptn();

    Trace("Operator", "FormOption Form_Save Button Click");

//    OM.SaveEqpOptn();
//    UserMsg2("Confirm" , "저장 되었습니다.");
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::UpdateComOptn(bool _bToTable)
{

    if (_bToTable == toTabl) {
        cbDoorSkip      -> Checked = OM.CmnOptn.bDoorSkip           ; //
        cbLoadingStop   -> Checked = OM.CmnOptn.bLoadingStop        ; //Loading Stop
        edVsBfGrabDly   -> Text    = OM.CmnOptn.iVsBfGrabDly        ; //비전 검사전 딜레이 . 모터 안정화 정도.
        edServerPath    -> Text    = OM.CmnOptn.sSourcePath         ; //서버에 있는것을 가져온다.
        edWorkedPath    -> Text    = OM.CmnOptn.sWorkedPath         ; //서버에 있는것을 가져오면 백업.
        edStageTLmtErr  -> Text    = OM.CmnOptn.dStageTLmtErr       ; //스테이지 각도 리밋 에러 범위.
        cbUseBarcode    -> Checked = OM.CmnOptn.bUseBarcode         ; //바코드 사용 여부.

        edVisnPenYOft   -> Text    = OM.CmnOptn.dVisnPenYOft        ; //비전과 펜사이의 Y 오프셑.
        edPenDnCnt      -> Text    = OM.CmnOptn.iPenDnCnt           ; //화이트 보드 팬체크시에 제자리에서 몇번찍는지.
        edPenAtDnDly    -> Text    = OM.CmnOptn.iPenAtDnDly         ; //펜 마킹시에 다운 하고 딜레이.
        edPenDnOverCnt  -> Text    = OM.CmnOptn.iPenDnOverCnt       ; //펜 마킹시에 다운 하고 딜레이. 

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
        OM.CmnOptn.iVsBfGrabDly    = edVsBfGrabDly   -> Text.ToIntDef(10); //비전 검사전 딜레이 . 모터 안정화 정도.
        OM.CmnOptn.sSourcePath     = edServerPath    -> Text             ; //서버에 있는것을 가져온다.
        OM.CmnOptn.sWorkedPath     = edWorkedPath    -> Text             ; //서버에 있는것을 가져오면 백업.
        OM.CmnOptn.dStageTLmtErr   = edStageTLmtErr  -> Text.ToDouble()  ; //스테이지 각도 리밋 에러 범위.
        OM.CmnOptn.bUseBarcode     = cbUseBarcode    -> Checked          ; //바코드 사용 여부.

        OM.CmnOptn.dVisnPenYOft    = edVisnPenYOft   -> Text.ToDouble()  ; //비전과 펜사이의 Y 오프셑.
        OM.CmnOptn.iPenDnCnt       = edPenDnCnt      -> Text.ToIntDef(1) ; //화이트 보드 팬체크시에 제자리에서 몇번찍는지.
        OM.CmnOptn.iPenAtDnDly     = edPenAtDnDly    -> Text.ToIntDef(10); //펜 마킹시에 다운 하고 딜레이.

        OM.CmnOptn.iPenDnOverCnt   = edPenDnOverCnt  -> Text.ToIntDef(OM.CmnOptn.iPenDnOverCnt); //펜 마킹시에 다운 하고 딜레이.



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

