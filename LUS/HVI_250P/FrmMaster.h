//---------------------------------------------------------------------------

#ifndef FrmMasterH
#define FrmMasterH
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
class TFormMaster : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
    TPanel *Panel3;
    TPanel *Panel5;
    TImage *Image2;
    TLabel *Label19;
    TCheckBox *cbDebugMode;
    TBitBtn *btSaveMotor;
        TTimer *tmMaster;
        TGroupBox *GroupBox6;
        TStringGrid *sgCycleStep;
        TCheckBox *cbPRI;
        TCheckBox *cbPRB;
        TCheckBox *cbWRK;
        TCheckBox *cbPSI;
        TButton *btAutoRun;
        TButton *btReset;
        TButton *btErrReset;
        TCheckBox *cbDryRun;
        TCheckBox *cbGoodChipAdd;
        TBitBtn *BitBtn1;
        TLabel *Label1;
        TEdit *edPreEquipPath;
        TLabel *Label2;
        TLabel *Label3;
        TButton *Button1;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btSaveMotorClick(TObject *Sender);
        void __fastcall FormHide(TObject *Sender);
        void __fastcall btAutoRunClick(TObject *Sender);
        void __fastcall btResetClick(TObject *Sender);
        void __fastcall btErrResetClick(TObject *Sender);
        void __fastcall tmMasterTimer(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormMaster(TComponent* Owner);
    void __fastcall UpdateMstOptn(bool bTable);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMaster *FormMaster;
//---------------------------------------------------------------------------
#endif
