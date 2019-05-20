//---------------------------------------------------------------------------
#ifndef FormOperatorH
#define FormOperatorH
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
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
class TFrmOperator : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TTimer *tmUpdate;
    TImage *Image5;
    TLabel *Label10;
    TGroupBox *GroupBox8;
    TPanel *pnLBLim0;
    TPanel *pnLBCnt0;
    TGroupBox *GroupBox9;
    TPanel *pnLBLim1;
    TPanel *pnLBCnt1;
    TGroupBox *GroupBox10;
    TPanel *pnLBLim2;
    TPanel *pnLBCnt2;
    TGroupBox *GroupBox11;
    TPanel *pnLBLim3;
    TPanel *pnLBCnt3;
    TGroupBox *GroupBox12;
    TPanel *pnLBLim4;
    TPanel *pnLBCnt4;
    TGroupBox *GroupBox13;
    TPanel *pnLBLim5;
    TPanel *pnLBCnt5;
    TGroupBox *GroupBox14;
    TPanel *pnLBLim6;
    TPanel *pnLBCnt6;
    TGroupBox *gbsadfg;
    TPanel *pnLBLim7;
    TPanel *pnLBCnt7;
    TGroupBox *GroupBox24;
        TPanel *pnLBLim9;
    TPanel *pnLBCnt9;
    TGroupBox *GroupBox25;
    TPanel *pnLBLim8;
    TPanel *pnLBCnt8;
    TPanel *plPassWord;
    TPanel *Panel19;
    TLabel *Label17;
    TBitBtn *btPasswordClose;
    TBitBtn *btEngr;
    TBitBtn *btOper;
    TBitBtn *btMast;
    TBitBtn *btOperator;
    TChart *ctBinCnt;
    TBarSeries *Series1;
    TChart *ctDist;
    TBarSeries *BarSeries1;
    TEdit *Edit1;
    TButton *Button1;
    TButton *Button2;
        TShape *Shape1;
        TButton *btBinCntClear;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btOperatorClick(TObject *Sender);
    void __fastcall btPasswordCloseClick(TObject *Sender);
    void __fastcall btOperClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall pnLBCnt7Click(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall GroupBox16Click(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
        void __fastcall btBinCntClearClick(TObject *Sender);

private:	// User declarations

public:		// User declarations
    __fastcall TFrmOperator(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOperator *FrmOperator;
//---------------------------------------------------------------------------
#endif
