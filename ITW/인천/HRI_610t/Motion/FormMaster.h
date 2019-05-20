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
        TOpenDialog *dgFilePath;
        TPanel *Panel3;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TPanel *Panel5;
        TCheckBox *cbDebugMode;
        TBitBtn *btSaveMotor;
        TGroupBox *GroupBox8;
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
        TCheckBox *cbLdr_YHome;
        TCheckBox *cbDryRun;
        TEdit *edTrigerOffset;
        TEdit *edVisnRsltFile;
        TButton *btVisnRsltFile;
        TEdit *edLaserDataFile;
        TButton *Button1;
        TButton *Button3;
        TEdit *edVisnDeviceFile;
        TButton *Button2;
        TLabel *lbMan;
        TButton *LaserReset;
        TCheckBox *cbAllStrpSendLsrJob;
    void __fastcall btSaveMotorClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall btPartAutorunClick(TObject *Sender);
    void __fastcall btPartResetClick(TObject *Sender);
    void __fastcall btAllResetClick(TObject *Sender);
        void __fastcall btVisnRsltFileClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
    void __fastcall btPartHomeClick(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
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
