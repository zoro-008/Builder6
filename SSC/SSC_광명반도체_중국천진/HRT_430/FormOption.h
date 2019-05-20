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
#include "SMDllDefine.h"
//---------------------------------------------------------------------------
class TFrmOption : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel3;
    TCheckBox *cbDryRun;
    TPanel *Panel5;
    TImage *Image2;
    TLabel *Label19;
        TBitBtn *btSave;
    TCheckBox *cbIgnrDoor;
    TCheckBox *cbIgnrInsp;
    TCheckBox *cbIgnrIdCheck;
    TCheckBox *cbIgnrHeatAlarm;
        TCheckBox *cbIgnrStrOverAlm;
        TCheckBox *cbAutoOperation;
        TCheckBox *cbAutoConvesion;
        TCheckBox *cbLoadingStop;
    TCheckBox *cbHeatFail;
    TEdit *edLimAllFailCnt;
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateComOptn(bool _bToTable);
    bool __fastcall LanguageSel(EN_LAN_SEL lvType);
public:		// User declarations
    __fastcall TFrmOption(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOption *FrmOption;
//---------------------------------------------------------------------------
#endif
































