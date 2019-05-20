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
#include <Graphics.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <jpeg.hpp>
#include "Timer.h"
class TFrmOperation : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel4;
    TPanel *Panel5;
    TPanel *Panel6;
    TPanel *Panel7;
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
    TPanel *Panel10;
    TPanel *Panel11;
    TPanel *Panel12;
    TPanel *Panel13;
    TPanel *Panel14;
    TPanel *Panel15;
    TListBox *lbErr;
    TPanel *Panel16;
    TPanel *Panel18;
    TTimer *tmUpdate;
    TBitBtn *btLotOpen;
    TBitBtn *btLotEnd;
        TPageControl *pcManual;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TTabSheet *TabSheet3;
    TTabSheet *TabSheet4;
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
    TPanel *Panel32;
    TLabel *Label14;
    TPanel *pnIgnrDoor;
    TPanel *Panel25;
    TLabel *Label36;
        TPanel *pnIgnrDisps;
    TPanel *Panel23;
    TLabel *Label28;
    TPanel *pnLoadingStop;
    TPanel *Panel2;
    TGroupBox *GroupBox1;
    TPanel *pnLDR;
    TGroupBox *GroupBox2;
    TPanel *pnULD;
    TGroupBox *GroupBox3;
    TGroupBox *GroupBox5;
    TPanel *pnPRB;
        TPanel *pnRWK;
        TButton *btRWKClear;
        TButton *btPRBClear;
    TLabel *lbPRBLotNo;
        TLabel *lbRWKLotNo;
        TButton *btPRBInput;
    TLabel *lbLDRLotNo;
    TLabel *lbULDLotNo;
        TLabel *Label24;
        TGroupBox *GroupBox6;
        TLabel *lbPSBLotNo;
        TPanel *pnPSB;
        TButton *btPSBClear;
        TLabel *Label12;
        TLabel *Label15;
        TLabel *Label18;
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
        TLabel *Label25;
        TLabel *Label26;
        TPanel *Panel17;
        TLabel *Label21;
        TPanel *pnSkipDisprSnsr;
        TEdit *edLQSupplyDay;
        TButton *btDayReset;
        TLabel *Label22;
        TPanel *Panel20;
        TLabel *Label23;
        TPanel *pnSkipDisprExpr;
        TEdit *edSettingDay;
        TLabel *Label27;
        TLabel *Label29;
        TTimer *tmLabelTimer;
        TPanel *Panel8;
        TPanel *Panel9;
        TStringGrid *sgDayInfo;
        TStringGrid *sgLotInfo;
        TPanel *Panel3;
        TLabel *Label19;
        TLabel *Label20;
        TGroupBox *GroupBox4;
        TLabel *lbLWKLotNo;
        TLabel *Label13;
        TPanel *pnLWK;
        TButton *btLWKClear;
        TLabel *Label1;
        TLabel *Label30;
        TButton *btULDMask;
        TTimer *tmStopToShot;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btOperatorClick(TObject *Sender);
    void __fastcall btPasswordCloseClick(TObject *Sender);
    void __fastcall btOperClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall btLotOpenClick(TObject *Sender);
        void __fastcall btStartClick(TObject *Sender);
        void __fastcall btStopClick(TObject *Sender);
        void __fastcall btResetClick(TObject *Sender);
        void __fastcall btLotEndClick(TObject *Sender);
    void __fastcall pnIgnoreTrimDblClick(TObject *Sender);
    void __fastcall btMan1_1Click(TObject *Sender);
    void __fastcall pnDayRunTimeDblClick(TObject *Sender);
    void __fastcall btPRBClearClick(TObject *Sender);
        void __fastcall btPRBInputClick(TObject *Sender);
        void __fastcall btDayResetClick(TObject *Sender);
        void __fastcall tmLabelTimerTimer(TObject *Sender);
        void __fastcall btULDMaskClick(TObject *Sender);
        void __fastcall tmStopToShotTimer(TObject *Sender);

private:	// User declarations

public:		// User declarations
    __fastcall TFrmOperation(TComponent* Owner);
    bool __fastcall CheckStripDisappear();
    bool __fastcall CheckStripUnknwon  ();

    CDelayTimer m_tmTemp   ;


    //bool m_bLotOpenMsgShow;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
#endif
