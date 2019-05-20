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
    TImage *Image2;
    TLabel *Label19;
    TTimer *tmUpdate;
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TStringGrid *sgCycleStep;
    TCheckBox *cbPart1;
    TCheckBox *cbPart2;
    TCheckBox *cbPart3;
    TCheckBox *cbPart4;
    TButton *Button10;
    TButton *Button11;
    TButton *Button12;
    TTabSheet *TabSheet3;
    TGroupBox *GroupBox1;
    TCheckBox *cbDebugMode;
    TGroupBox *GroupBox2;
    TLabel *Label1;
    TEdit *edModelName;
    TBitBtn *btSaveMotor;
    TButton *btChipStatsCng;
    TCheckBox *cbInspSkip;
        TCheckBox *cbIgnrPkgDustChk;
    void __fastcall btSaveMotorClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall Button10Click(TObject *Sender);
    void __fastcall Button11Click(TObject *Sender);
    void __fastcall Button12Click(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall btChipStatsCngClick(TObject *Sender);
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
