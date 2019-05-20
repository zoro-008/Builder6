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
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmMaster : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel3;
    TPanel *Panel5;
    TTimer *tmUpdate;
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TStringGrid *sgCycleStep;
    TCheckBox *cbPart1;
    TCheckBox *cbPart2;
    TButton *Button10;
    TButton *Button11;
    TButton *Button12;
    TBitBtn *btSaveMotor;
        TGroupBox *GroupBox3;
        TLabel *Label23;
        TEdit *edDeviceBackUpPath;
        TGroupBox *GroupBox2;
        TCheckBox *cbDebugMode;
        TCheckBox *cbDryRun;
        TGroupBox *GroupBox4;
        TLabel *Label1;
        TEdit *edModelName;
        TTabSheet *TabSheet3;
        TStringGrid *sgCycleTime;
        TComboBox *cbPart;
        TCheckBox *cbVacuum;
        TCheckBox *cbHopJumpChip;
        TTabSheet *TabSheet4;
        TStringGrid *StringGrid1;
        TButton *Button1;
    void __fastcall btSaveMotorClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall Button10Click(TObject *Sender);
    void __fastcall Button11Click(TObject *Sender);
    void __fastcall Button12Click(TObject *Sender);
        void __fastcall btWChipClearClick(TObject *Sender);
        void __fastcall btFChipClearClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall btChipStatsCngClick(TObject *Sender);
        void __fastcall cbPartChange(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateMstOptn(bool bTable);
    void __fastcall UpdateEqpOptn(bool bTable);

    void SetPartTime();


public:		// User declarations
    __fastcall TFrmMaster(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
#endif
