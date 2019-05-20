//---------------------------------------------------------------------------

#ifndef FormMasterH
#define FormMasterH
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------

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
    TStringGrid *sgCycleStep;
    TButton *Button10;
    TButton *Button11;
    TButton *Button12;
    TBitBtn *btSaveMotor;
        TEdit *edCycleTm;
        TLabel *Label1;
        TLabel *Label2;
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
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button5Click(TObject *Sender);
    void __fastcall Button9Click(TObject *Sender);
        void __fastcall Button13Click(TObject *Sender);
        void __fastcall Button15Click(TObject *Sender);
        void __fastcall Button14Click(TObject *Sender);
        void __fastcall Button16Click(TObject *Sender);
        void __fastcall Button17Click(TObject *Sender);
    void __fastcall btFixClick(TObject *Sender);
    void __fastcall btNotFixClick(TObject *Sender);
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
