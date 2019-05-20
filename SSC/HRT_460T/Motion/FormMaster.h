//---------------------------------------------------------------------------

#ifndef FormMasterH
#define FormMasterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TFrmMaster : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel3;
    TPanel *Panel5;
    TImage *Image2;
    TLabel *Label19;
    TCheckBox *cbDebugMode;
    TBitBtn *btSaveMotor;
    TTimer *tmUpdate;
    TLabel *Label7;
    TLabel *Label11;
    TEdit *edPostEquipPath;
        TCheckBox *cbHeattingTest;
        TGroupBox *GroupBox1;
        TStaticText *StaticText19;
        TEdit *ed1chLedOffset;
        TEdit *ed1chLedGain;
        TStaticText *StaticText1;
        TStaticText *StaticText2;
        TEdit *ed1chZennerOffset;
        TEdit *ed1chZennerGain;
        TStaticText *StaticText3;
        TGroupBox *GroupBox2;
        TStaticText *StaticText4;
        TEdit *ed2chLedOffset;
        TEdit *ed2chLedGain;
        TStaticText *StaticText5;
        TStaticText *StaticText6;
        TEdit *ed2chZennerOffset;
        TEdit *ed2chZennerGain;
        TStaticText *StaticText7;
    TGroupBox *GroupBox5;
    TGroupBox *GroupBox6;
    TEdit *edBarcodeNum;
    TStaticText *StaticText8;
        TCheckBox *cbByChInsp;
        TCheckBox *cbBarcodeUseOnly;
        TGroupBox *GroupBox7;
        TCheckBox *cbExistLoader;
        TCheckBox *cbExistUnLoader;
        TCheckBox *cbNoLdUl;
        TGroupBox *GroupBox8;
        TStringGrid *sgCycleStep;
        TCheckBox *cbLDR;
        TCheckBox *cbPRB;
        TCheckBox *cbWRK;
        TCheckBox *cbPSB;
        TButton *Button10;
        TButton *Button11;
        TButton *Button12;
        TCheckBox *cbULD;
        TCheckBox *cbNotUstLot;
    TCheckBox *cbShortMonitor;
        TGroupBox *GroupBox3;
        TStaticText *StaticText9;
        TEdit *edTempOfs;
        TStaticText *StaticText10;
        TEdit *edMaxTmp;
        TStaticText *StaticText11;
        TEdit *ed2chLedSetOfs;
        TStaticText *StaticText12;
        TEdit *ed2chZrSetOfs;
        TStaticText *StaticText13;
        TStaticText *StaticText14;
        TEdit *ed1chLedSetOfs;
        TEdit *ed1chZrSetOfs;
        TGroupBox *GroupBox13;
        TCheckBox *cbUsedEngVersion;
        TGroupBox *GroupBox16;
        TCheckBox *cbUsedBinSorting;
        TGroupBox *GroupBox4;
        TCheckBox *cbUsed4RowCntct;
    void __fastcall btSaveMotorClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall Button10Click(TObject *Sender);
    void __fastcall Button11Click(TObject *Sender);
    void __fastcall Button12Click(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateMstOptn(bool bTable);
    void __fastcall UpdateEqpOptn(bool bTable);
public:		// User declarations
    __fastcall TFrmMaster(TComponent* Owner);

    
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
#endif
