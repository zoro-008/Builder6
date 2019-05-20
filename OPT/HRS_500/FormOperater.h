//---------------------------------------------------------------------------
#ifndef FormOperaterH
#define FormOperaterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
//#include "FormPassword.h"
#include <Graphics.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <jpeg.hpp>
#include "CSPIN.h"
class TFrmOperater : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TTimer *tmUpdate;
    TTimer *Timer1;
    TImage *Image5;
    TLabel *Label10;
    TGroupBox *GroupBox8;
    TPanel *pnLBLim0;
    TPanel *pnLBCnt0;
    TButton *btLbRo0;
    TButton *btLbRa0;
    TGroupBox *GroupBox9;
    TPanel *pnLBLim1;
    TPanel *pnLBCnt1;
    TButton *btLbRo1;
    TButton *btLbRa1;
    TGroupBox *GroupBox10;
    TPanel *pnLBLim2;
    TPanel *pnLBCnt2;
    TButton *btLbRo2;
    TButton *btLbRa2;
    TGroupBox *GroupBox11;
    TPanel *pnLBLim3;
    TPanel *pnLBCnt3;
    TButton *btLbRo3;
    TButton *btLbRa3;
    TGroupBox *GroupBox12;
    TPanel *pnLBLim4;
    TPanel *pnLBCnt4;
    TButton *btLbRo4;
    TButton *btLbRa4;
    TGroupBox *GroupBox13;
    TPanel *pnLBLim5;
    TPanel *pnLBCnt5;
    TButton *btLbRo5;
    TButton *btLbRa5;
    TGroupBox *GroupBox14;
    TPanel *pnLBLim6;
    TPanel *pnLBCnt6;
    TButton *btLbRo6;
    TButton *btLbRa6;
    TGroupBox *GroupBox15;
    TPanel *pnLBLim7;
    TPanel *pnLBCnt7;
    TButton *btLbRo7;
    TButton *btLbRa7;
    TGroupBox *GroupBox16;
    TPanel *pnRBLim0;
    TPanel *pnRBCnt0;
    TButton *btRbRo0;
    TButton *btRbRa0;
    TGroupBox *GroupBox17;
    TPanel *pnRBLim1;
    TPanel *pnRBCnt1;
    TButton *btRbRo1;
    TButton *btRbRa1;
    TGroupBox *GroupBox18;
    TPanel *pnRBLim2;
    TPanel *pnRBCnt2;
    TButton *btRbRo2;
    TButton *btRbRa2;
    TGroupBox *GroupBox19;
    TPanel *pnRBLim3;
    TPanel *pnRBCnt3;
    TButton *btRbRo3;
    TButton *btRbRa3;
    TGroupBox *GroupBox20;
    TPanel *pnRBLim4;
    TPanel *pnRBCnt4;
    TButton *btRbRo4;
    TButton *btRbRa4;
    TGroupBox *GroupBox21;
    TPanel *pnRBLim5;
    TPanel *pnRBCnt5;
    TButton *btRbRo5;
    TButton *btRbRa5;
    TGroupBox *GroupBox22;
    TPanel *pnRBLim6;
    TPanel *pnRBCnt6;
    TButton *btRbRo6;
    TButton *btRbRa6;
    TGroupBox *GroupBox23;
    TPanel *pnRBLim7;
    TPanel *pnRBCnt7;
    TButton *btRbRo7;
    TButton *btRbRa7;
    TGroupBox *GroupBox24;
    TPanel *pnLBLim9;
    TPanel *pnLBCnt9;
    TButton *btLbRo9;
    TButton *btLbRa9;
    TGroupBox *GroupBox25;
    TPanel *pnLBLim8;
    TPanel *pnLBCnt8;
    TButton *btLbRo8;
    TButton *btLbRa8;
    TGroupBox *GroupBox26;
    TPanel *pnRBLim8;
    TPanel *pnRBCnt8;
    TButton *btRbRo8;
    TButton *btRbRa8;
    TGroupBox *GroupBox27;
    TPanel *pnRBLim9;
    TPanel *pnRBCnt9;
    TButton *btRbRo9;
    TButton *btRbRa9;
    TButton *btCountAllClear;
    TButton *btSupplyAll;
    TCSpinEdit *seBn9Sel;
    TCSpinEdit *seBn8Sel;
    TCSpinEdit *seBn7Sel;
    TCSpinEdit *seBn6Sel;
    TButton *btSetBin;
    TPanel *plPassWord;
    TPanel *Panel19;
    TLabel *Label17;
    TBitBtn *btPasswordClose;
    TBitBtn *btEngr;
    TBitBtn *btOper;
    TBitBtn *btMast;
    TBitBtn *btOperator;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btOperatorClick(TObject *Sender);
    void __fastcall btPasswordCloseClick(TObject *Sender);
    void __fastcall btOperClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall btLbRo0Click(TObject *Sender);
        void __fastcall btLbRa0Click(TObject *Sender);
        void __fastcall btRbRo0Click(TObject *Sender);
        void __fastcall btRbRa0Click(TObject *Sender);
        void __fastcall pnLBCnt7Click(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall btCountAllClearClick(TObject *Sender);
    void __fastcall btSupplyAllClick(TObject *Sender);
    void __fastcall btSetBinClick(TObject *Sender);

private:	// User declarations

public:		// User declarations
    __fastcall TFrmOperater(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOperater *FrmOperater;
//---------------------------------------------------------------------------
#endif
