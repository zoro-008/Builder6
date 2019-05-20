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
#include "UtilDefine.h"
//---------------------------------------------------------------------------
class TFrmMaster : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel3;
    TPanel *Panel5;
    TTimer *tmUpdate;
        TGroupBox *GroupBox8;
        TStringGrid *sgCycleStep;
        TButton *btPartAutorun;
        TButton *btPartReset;
        TButton *btAllReset;
        TBitBtn *btSaveMotor;
        TCheckBox *cbDebugMode;
        TCheckBox *cbDryRun;
        TEdit *edModelName;
        TStaticText *StaticText3;
        TButton *btPartHome;
        TStringGrid *sgTime;
        TComboBox *cbPartSel;
        TEdit *edMtrOfset;
        TStaticText *StaticText1;
        TStaticText *StaticText2;
        TEdit *edPkgCnt;
        TGroupBox *GroupBox1;
        TEdit *edGAMsg;
        TButton *btSend;
        TButton *Button5;
    void __fastcall btSaveMotorClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall btPartAutorunClick(TObject *Sender);
    void __fastcall btPartResetClick(TObject *Sender);
    void __fastcall btAllResetClick(TObject *Sender);
        void __fastcall btPartHomeClick(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall btSendClick(TObject *Sender);
        void __fastcall cbPartSelChange(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateMstOptn(bool bTable);
    void __fastcall UpdateEqpOptn(bool bTable);

    void UpdateTimeName ();
public:		// User declarations
    __fastcall TFrmMaster(TComponent* Owner);

    TCheckBox * cbPart[MAX_PART] ;
    
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
#endif
