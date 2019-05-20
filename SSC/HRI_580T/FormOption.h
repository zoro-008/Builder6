//---------------------------------------------------------------------------

#ifndef FormOptionH
#define FormOptionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "TEdit.h"
#include "SMDllDefine.h"
//---------------------------------------------------------------------------
class TFrmOption : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel3;
        TBitBtn *btSave;
        TCheckBox *cbDryRun;
        TCheckBox *cbNoTrimFail;
        TCheckBox *cbIgnrDoor;
        TCheckBox *cbVisn1Skip;
        TCheckBox *cbSortByItem;
    TPanel *Panel5;
    TImage *Image2;
    TLabel *Label19;
        TCheckBox *cbAutoVsChange;
        TCheckBox *cbAutoOperation;
        TCheckBox *cbCheckFailCnt;
        TEdit *edCheckFailCnt;
        TCheckBox *cbCheckSFailCnt;
        TEdit *edCheckSFailCnt;
        TLabel *Label1;
        TEdit *edSrtMotrDelay;
        TPanel *Panel1;
        TLabel *Label24;
        TLabel *Label25;
        TLabel *Label26;
        TLabel *Label27;
        TLabel *Label28;
        TLabel *Label29;
        TLabel *Label30;
        TLabel *Label31;
        TLabel *Label32;
        TLabel *Label36;
        TLabel *Label37;
        TLabel *Label40;
        TLabel *Label41;
        TLabel *Label42;
        TLabel *Label43;
        TLabel *Label44;
        TLabel *Label45;
        TLabel *Label46;
        TLabel *Label47;
        TLabel *Label48;
        TLabel *Label50;
        TLabel *Label51;
        TLabel *Label52;
        TLabel *Label53;
        TLabel *Label54;
        TLabel *Label55;
        TLabel *Label56;
        TLabel *Label57;
        TLabel *Label58;
        TLabel *Label59;
        TLabel *Label60;
        TLabel *Label62;
        TLabel *Label63;
        TEdit *edSortItRslt1;
        TEdit *edSortItRslt2;
        TEdit *edSortItRslt3;
        TEdit *edSortItRslt4;
        TEdit *edSortItRslt5;
        TEdit *edSortItRslt6;
        TEdit *edSortItRslt7;
        TEdit *edSortItRslt8;
        TEdit *edSortItRslt9;
        TEdit *edSortItRslt10;
        TEdit *edSortItRslt11;
        TEdit *edSortItRslt12;
        TEdit *edSortItRslt13;
        TEdit *edSortItRslt14;
        TEdit *edSortItFail;
        TPanel *Panel12;
        TImage *Image4;
        TLabel *Label61;
        TEdit *edLevelRslt1;
        TEdit *edLevelRslt2;
        TEdit *edLevelRslt3;
        TEdit *edLevelRslt4;
        TEdit *edLevelRslt5;
        TEdit *edLevelRslt6;
        TEdit *edLevelRslt7;
        TEdit *edLevelRslt8;
        TEdit *edLevelRslt9;
        TEdit *edLevelRslt10;
        TEdit *edLevelRslt11;
        TEdit *edLevelRslt12;
        TEdit *edLevelRslt13;
        TEdit *edLevelRslt14;
        TEdit *edLevelFail;
        TCheckBox *cbVisn2Skip;
    TCheckBox *cbTrimFailCheck;
        void __fastcall btSaveClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    bool __fastcall LanguageSel(EN_LAN_SEL lvType);
public:		// User declarations
    __fastcall TFrmOption(TComponent* Owner);

    void __fastcall UpdateComOptn     (bool _bToTable);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOption *FrmOption;
//---------------------------------------------------------------------------
#endif
























































