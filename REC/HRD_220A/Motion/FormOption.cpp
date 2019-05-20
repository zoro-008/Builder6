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
        cbLoadingStop   -> Checked   = OM.CmnOptn.bLoadingStop      ;
        cbIgnrDoor      -> Checked   = OM.CmnOptn.bIgnrDoor         ;
        cbTempErrorChk  -> Checked   = OM.CmnOptn.bTempIgnrErrorChk ;
        edTempErrorTol  -> Text      = OM.CmnOptn.dTempErrorTol     ;
        edDisprClnCnt   -> Text      = OM.CmnOptn.iDisprClnCnt      ;
        cbVisnNgStop    -> Checked   = OM.CmnOptn.bVisnNgStop       ;

        edMaxPlaceZOfs1 -> Text      = OM.CmnOptn.dMaxPlaceZOfs1    ;
        edMaxPlaceZOfs2 -> Text      = OM.CmnOptn.dMaxPlaceZOfs2    ;
        edMaxAttachForce-> Text      = OM.CmnOptn.dMaxAttachForce   ;
    }
    else {
        OM.CmnOptn.bLoadingStop      = cbLoadingStop   -> Checked  ;
        OM.CmnOptn.bIgnrDoor         = cbIgnrDoor      -> Checked  ;
        OM.CmnOptn.bTempIgnrErrorChk = cbTempErrorChk  -> Checked  ;
        OM.CmnOptn.dTempErrorTol     = StrToFloatDef(edTempErrorTol-> Text, OM.CmnOptn.dTempErrorTol) ;
        OM.CmnOptn.iDisprClnCnt      = StrToIntDef  (edDisprClnCnt -> Text, OM.CmnOptn.iDisprClnCnt ) ;
        OM.CmnOptn.bVisnNgStop       = cbVisnNgStop    -> Checked  ;

        OM.CmnOptn.dMaxPlaceZOfs1    = StrToFloatDef(edMaxPlaceZOfs1 -> Text,OM.CmnOptn.dMaxPlaceZOfs1 );
        OM.CmnOptn.dMaxPlaceZOfs2    = StrToFloatDef(edMaxPlaceZOfs2 -> Text,OM.CmnOptn.dMaxPlaceZOfs2 );
        OM.CmnOptn.dMaxAttachForce   = StrToFloatDef(edMaxAttachForce-> Text,OM.CmnOptn.dMaxAttachForce);
        if(OM.CmnOptn.dMaxAttachForce > 3000) {
            OM.CmnOptn.dMaxAttachForce = 3000 ;
            FM_MsgOk("Limit Over" , "Attach Force Limit의 최대값은 3000입니다.");
        }

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








