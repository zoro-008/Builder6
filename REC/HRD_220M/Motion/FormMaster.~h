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
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
//---------------------------------------------------------------------------
class TFrmMaster : public TForm
{
__published:	// IDE-managed Components
    TTimer *tmUpdate;
        TPanel *Panel3;
        TPanel *Panel5;
        TCheckBox *cbDebugMode;
        TBitBtn *btSaveMotor;
        TGroupBox *GroupBox8;
        TLabel *lbMan;
        TStringGrid *sgCycleStep;
        TCheckBox *cb1;
        TCheckBox *cb2;
        TCheckBox *cb4;
        TCheckBox *cb6;
        TButton *btPartAutorun;
        TButton *btPartReset;
        TButton *btAllReset;
        TCheckBox *cb5;
        TCheckBox *cb7;
        TCheckBox *cb3;
        TCheckBox *cb8;
        TCheckBox *cb9;
        TCheckBox *cb11;
        TCheckBox *cb13;
        TCheckBox *cb12;
        TCheckBox *cb10;
        TButton *btPartHome;
        TCheckBox *cbDryRun;
    void __fastcall btSaveMotorClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall btPartAutorunClick(TObject *Sender);
    void __fastcall btPartResetClick(TObject *Sender);
    void __fastcall btAllResetClick(TObject *Sender);
    void __fastcall btPartHomeClick(TObject *Sender);
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
