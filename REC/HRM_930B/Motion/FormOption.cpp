//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormOption.h"

#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "UtilDefine.h"
#include "SPCUnit.h"
#include "DataMan.h"
#include "Rs232UVCuring.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOption *FrmOption;
//---------------------------------------------------------------------------
AnsiString OnGetVer(const AnsiString &sQuery)
{

   //sQuery "CompanyName", "FileDescription", "FileVersion", "InternalName", "LegalCopyright", "LegalTradeMarks", "OriginalFileName", "ProductName", "ProductVersion", "Comments"

   DWORD dwHandle = 0, dwVersionInfoSize;
   UINT uLength;
   LPVOID pFileInfo, ptr;
   AnsiString sOut; // 리턴될 버전 정보.

   AnsiString filename = Application->ExeName;

   dwVersionInfoSize = GetFileVersionInfoSize(filename.c_str(), &dwHandle);

   pFileInfo = (LPVOID) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwVersionInfoSize);

   GetFileVersionInfo(filename.c_str(), dwHandle, dwVersionInfoSize, pFileInfo);
   VerQueryValue(pFileInfo, TEXT("\\VarFileInfo\\Translation"), &ptr, &uLength);

   WORD *id = (WORD *) ptr;
   AnsiString szString = "\\StringFileInfo\\" + IntToHex(id[0], 4) + IntToHex(id[1], 4) + "\\" + sQuery;

   VerQueryValue(pFileInfo, szString.c_str(), &ptr, &uLength);
   sOut = AnsiString((char *) ptr);
   HeapFree(GetProcessHeap(), 0, pFileInfo );
   return sOut;
}
__fastcall TFrmOption::TFrmOption(TComponent* Owner)
    : TForm(Owner)
{
    lbVer -> Caption = "Ver " + OnGetVer("FileVersion") ;
    int Age = FileAge(Application -> ExeName);
    AnsiString Date = FileDateToDateTime(Age).FormatString("''yyyy'_ 'm'_ 'd'_ 'AM/PM' 'h': 'n''");
    lbDate -> Caption = Date ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::btSaveClick(TObject *Sender)
{
    //Check Running Status.
    if (SEQ._bRun) {
        FM_MsgOk("Warning" , "Can't Save during Autorunning!");
        return;
    }
    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;

    UpdateComOptn(toBuff);
    OM.SaveCmnOptn();

}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::UpdateComOptn(bool _bToTable)
{                                                                                   
    if (_bToTable == toTabl) {
        cbWorkMode   -> ItemIndex = OM.CmnOptn.iWorkMode;



        cbFLoadingStop         -> Checked   = OM.CmnOptn.bFLoadingStop         ;
        cbRLoadingStop         -> Checked   = OM.CmnOptn.bRLoadingStop         ;
        cbIgnrDoor             -> Checked   = OM.CmnOptn.bIgnrDoor             ;
        cbUseConfocal          -> Checked   = OM.CmnOptn.bUseConfocal          ;
        cbIgnrCmsREndAlign     -> Checked   = OM.CmnOptn.bIgnrCmsREndAlign     ;
        cbIgnrCmsREndHeight    -> Checked   = OM.CmnOptn.bIgnrCmsREndHeight    ;
        cbIgnrCmsRFixAlign     -> Checked   = OM.CmnOptn.bIgnrCmsRFixAlign     ;
        cbIgnrCmsRFixHeight    -> Checked   = OM.CmnOptn.bIgnrCmsRFixHeight    ;
        cbIgnrMidREndHeight    -> Checked   = OM.CmnOptn.bIgnrMidREndHeight    ;

        edMaxPlaceZOfs1        -> Text      = OM.CmnOptn.dMaxPlaceZOfs1        ;
        edMaxPlaceZOfs2        -> Text      = OM.CmnOptn.dMaxPlaceZOfs2        ;
        //edMaxAttachForce       -> Text      = OM.CmnOptn.dMaxAttachForce       ;

        edRightVisnDelay       -> Text      = OM.CmnOptn.iRightVisnDelay       ;
        edRightHeightDelay     -> Text      = OM.CmnOptn.iRightHeightDelay     ;
        edRightHeightStepDelay -> Text      = OM.CmnOptn.iRightHeightStepDelay ;
        edHexaPotRelDelay      -> Text      = OM.CmnOptn.iHexaPotRelDelay      ;

        edDisprClnTime         -> Text      = OM.CmnOptn.iDisprClnTime         ;

        edTopUVLimitTime       -> Text      = OM.CmnOptn.iTopUVLimitTime       ;
        edBtmUVLimitTime       -> Text      = OM.CmnOptn.iBtmUVLimitTime       ;

    }
    else {
        OM.CmnOptn.iWorkMode             = cbWorkMode         -> ItemIndex ;
        OM.CmnOptn.bFLoadingStop         = cbFLoadingStop     -> Checked   ;
        OM.CmnOptn.bRLoadingStop         = cbRLoadingStop     -> Checked   ;
        OM.CmnOptn.bIgnrDoor             = cbIgnrDoor         -> Checked   ;
        OM.CmnOptn.bUseConfocal          = cbUseConfocal      -> Checked   ;
        OM.CmnOptn.bIgnrCmsREndAlign     = cbIgnrCmsREndAlign -> Checked   ;
        OM.CmnOptn.bIgnrCmsREndHeight    = cbIgnrCmsREndHeight-> Checked   ;
        OM.CmnOptn.bIgnrCmsRFixAlign     = cbIgnrCmsRFixAlign -> Checked   ;
        OM.CmnOptn.bIgnrCmsRFixHeight    = cbIgnrCmsRFixHeight-> Checked   ;
        OM.CmnOptn.bIgnrMidREndHeight    = cbIgnrMidREndHeight-> Checked   ;

        OM.CmnOptn.dMaxPlaceZOfs1        = StrToFloatDef(edMaxPlaceZOfs1 -> Text,OM.CmnOptn.dMaxPlaceZOfs1 );
        OM.CmnOptn.dMaxPlaceZOfs2        = StrToFloatDef(edMaxPlaceZOfs2 -> Text,OM.CmnOptn.dMaxPlaceZOfs2 );
        //OM.CmnOptn.dMaxAttachForce       = StrToFloatDef(edMaxAttachForce-> Text,OM.CmnOptn.dMaxAttachForce);
        //if(OM.CmnOptn.dMaxAttachForce > 3000) {
        //    OM.CmnOptn.dMaxAttachForce = 3000 ;
        //    FM_MsgOk("Limit Over" , "Attach Force Limit의 최대값은 3000입니다.");
        //}

        OM.CmnOptn.iRightVisnDelay       = StrToFloatDef(edRightVisnDelay       -> Text,OM.CmnOptn.iRightVisnDelay      );
        OM.CmnOptn.iRightHeightDelay     = StrToFloatDef(edRightHeightDelay     -> Text,OM.CmnOptn.iRightHeightDelay    );
        OM.CmnOptn.iRightHeightStepDelay = StrToFloatDef(edRightHeightStepDelay -> Text,OM.CmnOptn.iRightHeightStepDelay);
        OM.CmnOptn.iHexaPotRelDelay      = StrToFloatDef(edHexaPotRelDelay      -> Text,OM.CmnOptn.iHexaPotRelDelay     );
        OM.CmnOptn.iDisprClnTime         = StrToIntDef  (edDisprClnTime         -> Text,OM.CmnOptn.iDisprClnTime        );

        OM.CmnOptn.iTopUVLimitTime       = StrToIntDef  (edTopUVLimitTime       -> Text,OM.CmnOptn.iTopUVLimitTime      );
        OM.CmnOptn.iBtmUVLimitTime       = StrToIntDef  (edBtmUVLimitTime       -> Text,OM.CmnOptn.iBtmUVLimitTime      );

        UpdateComOptn(toTabl);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::FormShow(TObject *Sender)
{

    cbIgnrDoor -> Visible = FM_GetLevel() == lvMaster ;
    UpdateComOptn(toTabl);
}
//---------------------------------------------------------------------------








void __fastcall TFrmOption::Button1Click(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm" , "Top UV 사용시간을 리셋하시겠습니까?")!=mrYes) return;

    Rs232UV.ClearTopUvUseTime();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOption::Button2Click(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm" , "Bottom UV 사용시간을 리셋하시겠습니까?")!=mrYes) return;
    Rs232UV.ClearBtmUvUseTime();
}
//---------------------------------------------------------------------------

