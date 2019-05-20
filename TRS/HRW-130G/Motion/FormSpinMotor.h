//---------------------------------------------------------------------------

#ifndef FormSpinMotorH
#define FormSpinMotorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmSpinMT : public TForm
{
__published:	// IDE-managed Components
        TTimer *tmSpinMT;
        TPanel *pnMain;
        TLabel *Label8;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label9;
        TPanel *Panel1;
        TPanel *Panel2;
        TPanel *Panel3;
        TPanel *pnReady;
        TPanel *Panel4;
        TPanel *pnBusy;
        TPanel *Panel5;
        TPanel *Panel6;
        TLabel *Label6;
        TLabel *Label7;
        TPanel *pnJogSpeed;
        TPanel *Panel8;
        TPanel *btJogMove;
        TPanel *pnButton;
        TButton *bt1;
        TButton *bt2;
        TButton *bt3;
        TButton *bt4;
        TButton *bt5;
        TButton *bt6;
        TButton *bt7;
        TButton *bt8;
        TButton *bt9;
        TButton *btClear;
        TButton *bt0;
        TButton *btBack;
        TButton *btOK;
        TEdit *edJogSpeed;
        TLabel *lbModelName;
        void __fastcall btJogMoveMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btJogMoveMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btOKClick(TObject *Sender);
        void __fastcall bt1Click(TObject *Sender);
        void __fastcall btBackClick(TObject *Sender);
        void __fastcall btClearClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall tmSpinMTTimer(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Panel7MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall pnJogSpeedClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmSpinMT(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmSpinMT *FrmSpinMT;
//---------------------------------------------------------------------------
#endif
