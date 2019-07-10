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
        TEdit *ed1chLedGain;
        TStaticText *StaticText1;
        TEdit *ed1chZennerGain;
        TStaticText *StaticText3;
        TGroupBox *GroupBox2;
        TEdit *ed2chLedGain;
        TStaticText *StaticText5;
        TEdit *ed2chZennerGain;
        TStaticText *StaticText7;
    TGroupBox *GroupBox3;
    TCheckBox *cbGripTest;
    TGroupBox *GroupBox4;
        TCheckBox *cbUsedSmallStrip;
    TGroupBox *GroupBox5;
    TCheckBox *cbUseDaegyeomProbe;
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
        TCheckBox *cbSTL;
        TCheckBox *cbULD;
        TCheckBox *cbNotUstLot;
    TGroupBox *GroupBox9;
    TCheckBox *cbPRBCmp;
    TCheckBox *cbShortMonitor;
    TGroupBox *GroupBox10;
    TCheckBox *cbDataAccEng;
        TStaticText *StaticText9;
        TEdit *ed1chLedSetOfs;
        TEdit *ed2chLedSetOfs;
        TStaticText *StaticText10;
        TGroupBox *GroupBox11;
        TCheckBox *cbLdr_YHome;
        TGroupBox *GroupBox12;
        TCheckBox *cbUsedVfSortOption;
        TGroupBox *GroupBox13;
        TCheckBox *cbUsedTempRS232;
        TGroupBox *GroupBox14;
        TStaticText *StaticText2;
        TEdit *edTempOfs;
        TStaticText *StaticText4;
        TEdit *edMaxTmp;
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
