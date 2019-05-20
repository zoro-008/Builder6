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
    TPanel *Panel32;
    TLabel *Label14;
    TPanel *pnIgnrDoor;
    TPanel *Panel25;
    TLabel *Label36;
    TPanel *pnSkipLaserWrk;
        TPageControl *PageControl1;
        TPanel *pnTotalTime;
        TLabel *Label11;
        TStringGrid *sgLotInfo;
        TStringGrid *sgDayInfo;
    TPanel *pnDataMap;
    TGroupBox *GroupBox3;
    TButton *btPR1Clear;
        TPanel *pnLerFed;
    TGroupBox *GroupBox2;
    TButton *Button4;
        TPanel *pnLerLsr;
    TGroupBox *GroupBox4;
    TButton *Button6;
        TPanel *pnFrtFed;
    TGroupBox *GroupBox6;
    TButton *Button10;
        TPanel *pnFrtLsr;
    TPanel *Panel2;
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
        TLabel *lbBackCnt;
        TLabel *lbFrntCnt;
        TLabel *Label12;
        TLabel *Label13;
        TButton *btAddPkgF;
        TButton *btAddPkgB;
        TBitBtn *btOperator;
        TBitBtn *btAllHome;
        TBitBtn *btReset;
        TBitBtn *btStop;
        TBitBtn *btStart;
        TPanel *plPassWord;
        TPanel *Panel19;
        TLabel *Label17;
        TBitBtn *btPasswordClose;
        TBitBtn *btEngr;
        TBitBtn *btOper;
        TBitBtn *btMast;
        TPanel *pnVisn;
        TButton *btUWorkCntReset;
        TPanel *Panel3;
        TPanel *Panel17;
        TBitBtn *BitBtn1;
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
        void __fastcall btPR1ClearClick(TObject *Sender);
    void __fastcall pnDayRunTimeDblClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall btUWorkCntResetClick(TObject *Sender);
        void __fastcall btAllHomeClick(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);

private:	// User declarations

public:		// User declarations
    __fastcall TFrmOperation(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
#endif
