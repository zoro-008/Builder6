//---------------------------------------------------------------------------

#ifndef FormOptionH
#define FormOptionH
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmOption : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TCheckBox *cbbIgnrRalAtVs;
    TCheckBox *cbbUseStgTheta;
    TCheckBox *cbbIgnrRalBfVsEpoxyFail;
    TCheckBox *cbIgnrOriVs;
    TPanel *Panel4;
    TPanel *Panel5;
    TCheckBox *cbLoadingStop;
    TCheckBox *cbbDoorSkip;
    TCheckBox *cbbIgnrVcSsr;
    TBitBtn *btSave;
        TCheckBox *cbUseMapFile;
    TCheckBox *cbUseBtAlign;
    TCheckBox *cbIgnrHedOverload;
        TCheckBox *cbUseHeadTWfr;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *edEjctrDnAfterDelay;
        TLabel *Label4;
        TLabel *Label5;
        TEdit *edEjctrUpAfterDelay;
        TCheckBox *cbWfrVisnRetest;
    TPanel *Panel6;
    TPanel *Panel7;
    TLabel *Label9;
    TLabel *Label10;
    TEdit *edLotEndChipCnt;
        TCheckBox *cbUseVcCoil;
        TCheckBox *cbEpoxyStop;
        TCheckBox *cbUseHeadTRal;
        TCheckBox *cbInkDieWork;
        TLabel *Label67;
        TLabel *Label68;
        TLabel *Label6;
        TLabel *Label7;
        TEdit *edStageTLmtErr;
        TEdit *edHedTLmtErr;
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall cbLoadingStopClick(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateComOptn(bool _bToTable);
public:		// User declarations
    __fastcall TFrmOption(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOption *FrmOption;
//---------------------------------------------------------------------------
#endif
