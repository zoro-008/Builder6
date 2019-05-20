//---------------------------------------------------------------------------
#ifndef FormOperationH
#define FormOperationH
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
class TFrmOperation : public TForm
{
__published:	// IDE-managed Components
    TTimer *tmUpdate;
        TPanel *Panel1;
        TImage *Image3;
        TLabel *Label11;
        TPanel *Panel4;
        TPanel *Panel5;
        TImage *Image7;
        TLabel *Label24;
        TPageControl *pcManual;
        TTabSheet *TabSheet1;
        TBitBtn *btMan1_1;
        TBitBtn *btMan1_2;
        TBitBtn *btMan1_7;
        TBitBtn *btMan1_3;
        TBitBtn *btMan1_4;
        TBitBtn *btMan1_5;
        TBitBtn *btMan1_6;
        TBitBtn *btMan1_8;
        TBitBtn *btMan1_9;
        TBitBtn *btMan1_10;
        TBitBtn *btMan1_11;
        TBitBtn *btMan1_12;
        TBitBtn *btMan1_13;
        TBitBtn *btMan1_14;
        TTabSheet *TabSheet2;
        TBitBtn *btMan2_1;
        TBitBtn *btMan2_2;
        TBitBtn *btMan2_7;
        TBitBtn *btMan2_3;
        TBitBtn *btMan2_4;
        TBitBtn *btMan2_5;
        TBitBtn *btMan2_6;
        TBitBtn *btMan2_8;
        TBitBtn *btMan2_9;
        TBitBtn *btMan2_10;
        TBitBtn *btMan2_11;
        TBitBtn *btMan2_12;
        TBitBtn *btMan2_13;
        TBitBtn *btMan2_14;
        TTabSheet *TabSheet3;
        TBitBtn *btMan3_1;
        TBitBtn *btMan3_2;
        TBitBtn *btMan3_7;
        TBitBtn *btMan3_3;
        TBitBtn *btMan3_4;
        TBitBtn *btMan3_5;
        TBitBtn *btMan3_6;
        TBitBtn *btMan3_8;
        TBitBtn *btMan3_9;
        TBitBtn *btMan3_10;
        TBitBtn *btMan3_11;
        TBitBtn *btMan3_12;
        TBitBtn *btMan3_13;
        TBitBtn *btMan3_14;
        TTabSheet *TabSheet4;
        TBitBtn *btMan4_1;
        TBitBtn *btMan4_2;
        TBitBtn *btMan4_7;
        TBitBtn *btMan4_3;
        TBitBtn *btMan4_4;
        TBitBtn *btMan4_5;
        TBitBtn *btMan4_6;
        TBitBtn *btMan4_8;
        TBitBtn *btMan4_9;
        TBitBtn *btMan4_10;
        TBitBtn *btMan4_11;
        TBitBtn *btMan4_12;
        TBitBtn *btMan4_13;
        TBitBtn *btMan4_14;
        TTabSheet *TabSheet11;
        TBitBtn *btMan5_1;
        TBitBtn *btMan5_2;
        TBitBtn *btMan5_7;
        TBitBtn *btMan5_3;
        TBitBtn *btMan5_4;
        TBitBtn *btMan5_5;
        TBitBtn *btMan5_6;
        TBitBtn *btMan5_8;
        TBitBtn *btMan5_9;
        TBitBtn *btMan5_10;
        TBitBtn *btMan5_11;
        TBitBtn *btMan5_12;
        TBitBtn *btMan5_13;
        TBitBtn *btMan5_14;
        TTabSheet *TabSheet12;
        TBitBtn *btMan7_1;
        TBitBtn *btMan7_8;
        TBitBtn *btMan7_2;
        TBitBtn *btMan7_9;
        TBitBtn *btMan7_3;
        TBitBtn *btMan7_10;
        TBitBtn *btMan7_4;
        TBitBtn *btMan7_11;
        TBitBtn *btMan7_5;
        TBitBtn *btMan7_12;
        TBitBtn *btMan7_6;
        TBitBtn *btMan7_13;
        TBitBtn *btMan7_7;
        TBitBtn *btMan7_14;
        TPanel *Panel6;
        TPanel *Panel7;
        TImage *Image8;
        TLabel *Label25;
        TBitBtn *btStart;
        TBitBtn *btStop;
        TBitBtn *btReset;
        TBitBtn *btOperator;
        TPanel *plPassWord;
        TPanel *Panel19;
        TLabel *Label17;
        TBitBtn *btPasswordClose;
        TBitBtn *btEngr;
        TBitBtn *btOper;
        TBitBtn *btMast;
        TPanel *Panel8;
        TLabel *Label1;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label31;
        TLabel *Label16;
        TLabel *Label8;
        TPanel *Panel9;
        TImage *Image2;
        TLabel *Label19;
        TPanel *pnDayRunTime;
        TPanel *pnDayIdleTime;
        TPanel *pnDayJamTime;
        TPanel *pnDayTotalTime;
        TPanel *pnDayChipUpeh;
        TPanel *pnDayWorkChip;
        TPanel *pnDayFailChip;
        TPanel *pnWorkCycleTime;
        TPanel *Panel12;
        TPanel *Panel13;
        TImage *Image4;
        TLabel *Label21;
        TPanel *Panel32;
        TLabel *Label14;
        TPanel *pnEmptyIgnr;
        TPanel *Panel2;
        TLabel *Label7;
        TPanel *pnLoadingStop;
        TPanel *Panel3;
        TLabel *Label10;
        TPanel *pnUseBinFullIgnore;
        TPanel *Panel20;
        TLabel *Label12;
        TPanel *pnChuckFailIgnore;
        TPanel *Panel18;
        TLabel *Label13;
        TPanel *pnUseFailBinIgnore;
        TPanel *Panel14;
        TPanel *Panel15;
        TImage *Image6;
        TLabel *Label23;
        TListBox *lbErr;
        TGroupBox *GroupBox1;
        TPanel *pnPRB;
        TGroupBox *GroupBox2;
        TPanel *pnWRK;
        TGroupBox *GroupBox3;
        TPanel *pnWKE;
        TGroupBox *GroupBox4;
        TPanel *pnPSB;
        TGroupBox *GroupBox5;
        TPanel *pnPCK;
        TGroupBox *GroupBox6;
        TPanel *pnSTN;
        TGroupBox *GroupBox7;
        TPanel *pnSRT;
        TButton *btPrbClear;
        TButton *btWrkClear;
        TButton *btWkeClear;
        TButton *btPsbClear;
        TButton *btPckClear;
        TButton *btSrtClear;
        TButton *btStnClear;
        TStringGrid *sgData;
        TGroupBox *GroupBox8;
        TPanel *pnLBLim0;
        TPanel *pnLBCnt0;
        TButton *btLbRa0;
        TGroupBox *GroupBox9;
        TPanel *pnLBLim1;
        TPanel *pnLBCnt1;
        TButton *btLbRa1;
        TGroupBox *GroupBox10;
        TPanel *pnLBLim2;
        TPanel *pnLBCnt2;
        TButton *btLbRa2;
        TGroupBox *GroupBox11;
        TPanel *pnLBLim3;
        TPanel *pnLBCnt3;
        TButton *btLbRa3;
        TGroupBox *GroupBox12;
        TPanel *pnLBLim4;
        TPanel *pnLBCnt4;
        TButton *btLbRa4;
        TGroupBox *GroupBox13;
        TPanel *pnLBLim5;
        TPanel *pnLBCnt5;
        TButton *btLbRa5;
        TGroupBox *GroupBox14;
        TPanel *pnLBLim6;
        TPanel *pnLBCnt6;
        TButton *btLbRa6;
        TGroupBox *GroupBox15;
        TPanel *pnLBLim7;
        TPanel *pnLBCnt7;
        TButton *btLbRa7;
        TGroupBox *GroupBox24;
        TPanel *pnLBLim9;
        TPanel *pnLBCnt9;
        TButton *btLbRa9;
        TGroupBox *GroupBox25;
        TPanel *pnLBLim8;
        TPanel *pnLBCnt8;
        TButton *btLbRa8;
        TPanel *Panel10;
        TLabel *lbGauge1;
        TPanel *Panel11;
        TCheckBox *cbPKGRepeat;
        TCheckBox *cbGazeRepeat;
        TPanel *Panel16;
        TLabel *lbHeight1;
        TPanel *Panel17;
        TButton *btSetBin;
        TPanel *Panel21;
        TLabel *lbZeroCheck1;
        TPanel *Panel22;
        TComboBox *cbBnL9Sel;
        TComboBox *cbBnL8Sel;
        TComboBox *cbBnL7Sel;
        TComboBox *cbBnL6Sel;
        TComboBox *cbBnL5Sel;
        TComboBox *cbBnL4Sel;
        TComboBox *cbBnL3Sel;
        TComboBox *cbBnL2Sel;
        TComboBox *cbBnL1Sel;
        TComboBox *cbBnL0Sel;
        TLabel *lbGauge2;
        TLabel *lbGauge3;
        TLabel *lbHeight2;
        TLabel *lbHeight3;
        TLabel *lbZeroCheck2;
        TLabel *lbZeroCheck3;
        TButton *btHeightTest;
        TButton *btBinCntClear;
        TButton *Button1;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btOperatorClick(TObject *Sender);
    void __fastcall btPasswordCloseClick(TObject *Sender);
    void __fastcall btOperClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall btStopClick(TObject *Sender);
        void __fastcall btResetClick(TObject *Sender);
    void __fastcall btMan1_1Click(TObject *Sender);
    void __fastcall btStartClick(TObject *Sender);
        void __fastcall pnEmptyIgnrClick(TObject *Sender);
        void __fastcall btLbRo0Click(TObject *Sender);
        void __fastcall btLbRa0Click(TObject *Sender);
        void __fastcall btRbRo0Click(TObject *Sender);
        void __fastcall btRbRa0Click(TObject *Sender);
        void __fastcall btPrbClearClick(TObject *Sender);
        void __fastcall pnLBCnt7Click(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall btSetBinClick(TObject *Sender);
    void __fastcall Label10Click(TObject *Sender);
    void __fastcall cbBnL9SelChange(TObject *Sender);
        void __fastcall btHeightTestMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btHeightTestMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btBinCntClearClick(TObject *Sender);
        void __fastcall btMan2_6Click(TObject *Sender);
        void __fastcall btMan2_7Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);

private:	// User declarations

public:		// User declarations
    __fastcall TFrmOperation(TComponent* Owner);
    void __fastcall FormUpdate   ();

    bool __fastcall CheckStripDisappear();
    bool __fastcall CheckStripUnknwon  ();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
#endif
