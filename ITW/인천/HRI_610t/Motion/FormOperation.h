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
    TPanel *Panel8;
    TPanel *Panel9;
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
    TLabel *Label1;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label31;
    TLabel *Label32;
    TPanel *pnDayRunTime;
    TPanel *pnDayIdleTime;
    TPanel *pnDayJamTime;
    TPanel *pnDayTotalTime;
    TPanel *pnDayChipUpeh;
    TPanel *pnDayWorkChip;
    TPanel *pnDayLotCount;
        TPageControl *pcManual;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TTabSheet *TabSheet3;
    TTabSheet *TabSheet4;
    TLabel *Label16;
    TPanel *pnDayFailChip;
    TLabel *Label2;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label33;
    TLabel *Label34;
    TPanel *pnLotNo;
    TPanel *pnLotWorkChip;
    TPanel *pnLotChipUpeh;
    TPanel *pnLotStartTime;
    TPanel *pnLotRunTime;
    TPanel *pnLotFailChip;
    TPanel *pnLotJobFile;
    TTabSheet *TabSheet11;
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
    TPanel *Panel32;
    TLabel *Label14;
    TPanel *pnIgnrDoor;
    TPanel *Panel25;
    TLabel *Label36;
        TPanel *pnSkipLaser;
    TPanel *Panel23;
    TLabel *Label28;
    TPanel *pnLoadingStop;
        TPanel *pnTotalTime;
        TLabel *Label11;
        TStringGrid *sgLotInfo;
        TStringGrid *sgDayInfo;
    TPanel *Panel2;
    TPanel *Panel3;
    TGroupBox *GroupBox1;
    TPanel *pnLDR;
    TGroupBox *GroupBox2;
    TPanel *pnULD;
    TGroupBox *GroupBox3;
    TGroupBox *GroupBox4;
    TGroupBox *GroupBox5;
    TGroupBox *GroupBox6;
    TPanel *pnPRB;
    TPanel *pnLSR;
    TPanel *pnVSN;
    TPanel *pnPSB;
    TButton *btVsnClear;
    TButton *btPrbClear;
    TButton *btLsrClear;
    TButton *btPsbClear;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label15;
    TLabel *Label18;
    TLabel *lbPRBLotNo;
    TLabel *lbLSRLotNo;
    TLabel *lbVSNLotNo;
    TLabel *lbPSBLotNo;
    TButton *btPrbInput;
        TLabel *Label19;
        TLabel *Label20;
    TEdit *Edit1;
    TEdit *Edit2;
    TLabel *lbLDRLotNo;
    TLabel *lbULDLotNo;
        TButton *btVSNMakeGood;
        TEdit *edManMark;
        TCheckBox *cbManMark;
        TButton *btSetManMark;
    TButton *Button2;
        TButton *btPrbMask;
        TPanel *Panel17;
        TLabel *Label21;
        TPanel *pnSkipOriVsn;
        TPanel *Panel21;
        TLabel *Label22;
        TPanel *pnSkipAlgVsn;
        TPanel *Panel24;
        TLabel *Label23;
        TPanel *pnSkipMrkVsn;
        TLabel *Label24;
        TLabel *Label25;
        TLabel *Label26;
        TLabel *lbMgnCntPRB;
        TLabel *lbLaserDataPRB;
        TLabel *Label30;
        TLabel *lbMgnCntLSR;
        TLabel *Label37;
        TLabel *lbLaserDataLSR;
        TLabel *Label39;
        TLabel *lbMgnCntVSN;
        TLabel *Label41;
        TLabel *lbLaserDataVSN;
        TLabel *Label43;
        TLabel *lbMgnCntPSB;
        TLabel *Label45;
        TLabel *lbLaserDataPSB;
        TLabel *lbMgnCntLDR;
        TLabel *lbMgnCntULD;
        TBitBtn *btAllHome;
        TLabel *lbEmptyCnt;
        TBitBtn *btWorkEnd;
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
    void __fastcall btPR1LotInputClick(TObject *Sender);
        void __fastcall btPR1InputClick(TObject *Sender);
    void __fastcall pnDayRunTimeDblClick(TObject *Sender);
    void __fastcall btPrbClearClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall btPrbInputClick(TObject *Sender);
    void __fastcall Panel15DblClick(TObject *Sender);
        void __fastcall btVSNMakeGoodClick(TObject *Sender);
        void __fastcall cbManMarkClick(TObject *Sender);
        void __fastcall btSetManMarkClick(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
        void __fastcall btPrbMaskClick(TObject *Sender);
        void __fastcall btAllHomeClick(TObject *Sender);
        void __fastcall btWorkEndClick(TObject *Sender);

private:	// User declarations

public:		// User declarations
    __fastcall TFrmOperation(TComponent* Owner);
    bool __fastcall CheckStripDisappear();
    bool __fastcall CheckStripUnknwon  ();


    //bool m_bLotOpenMsgShow;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
#endif
