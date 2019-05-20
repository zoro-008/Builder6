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
#include <Dialogs.hpp>
class TFrmOperation : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel6;
    TPanel *Panel7;
    TBitBtn *btStart;
    TBitBtn *btStop;
    TBitBtn *btReset;
    TBitBtn *btOperator;
    TImage *Image8;
    TTimer *tmUpdate;
    TPanel *pnControl;
    TPanel *Panel8;
    TLabel *lbDInfo0;
    TLabel *lbDInfo1;
    TLabel *lbDInfo2;
    TLabel *lbDInfo3;
    TLabel *lbDInfo4;
    TLabel *lbDInfo5;
    TLabel *lbDInfo7;
    TLabel *lbDInfo6;
    TPanel *Panel9;
    TImage *Image2;
    TLabel *Label19;
    TPanel *pnDayRunTime;
    TPanel *pnDayIdleTime;
    TPanel *pnDayJamTime;
    TPanel *pnDayTotalTime;
    TPanel *pnDayChipUpeh;
    TPanel *pnDayWorkChip;
    TPanel *pnDayLotCount;
    TPanel *pnDayFailChip;
    TPanel *Panel2;
    TLabel *lbLInfo0;
    TLabel *lbLInfo2;
    TLabel *lbLInfo4;
    TLabel *lbLInfo5;
    TLabel *lbLInfo6;
    TLabel *lbLInfo3;
    TLabel *lbLInfo1;
    TPanel *Panel11;
    TImage *Image4;
    TLabel *Label20;
    TPanel *pnLotNo;
    TPanel *pnLotWorkChip;
    TPanel *pnLotChipUpeh;
    TPanel *pnLotStartTime;
    TPanel *pnLotRunTime;
    TPanel *pnLotFailChip;
    TPanel *pnLotJobFile;
    TPanel *Panel12;
    TPanel *Panel13;
    TImage *Image5;
    TLabel *Label21;
    TPanel *Panel32;
    TLabel *lbOption0;
    TPanel *pnOptn0;
    TPanel *Panel4;
    TLabel *lbOption3;
    TPanel *pnOptn3;
    TPanel *Panel34;
    TLabel *lbOption4;
    TPanel *pnOptn4;
    TPanel *Panel22;
    TLabel *lbOption7;
    TPanel *pnOptn7;
    TPanel *Panel25;
    TLabel *lbOption2;
    TPanel *pnOptn2;
    TPanel *Panel23;
    TLabel *lbOption1;
    TPanel *pnOptn1;
    TPanel *Panel21;
    TLabel *lbOption5;
    TPanel *pnOptn5;
    TPanel *Panel17;
    TLabel *lbOption6;
    TPanel *pnOptn6;
    TPanel *Panel14;
    TPanel *Panel15;
    TImage *Image6;
    TLabel *Label23;
    TListBox *lbErr;
    TPanel *Panel16;
    TPanel *Panel18;
    TImage *Image7;
    TLabel *Label24;
    TBitBtn *btLotOpen;
    TBitBtn *btLotEnd;
    TPanel *plPassWord;
    TPanel *Panel19;
    TLabel *Label17;
    TBitBtn *btPasswordClose;
    TBitBtn *btEngr;
    TBitBtn *btOper;
    TBitBtn *btMast;
    TLabel *Label25;
    TPanel *pnTControl;
    TImage *Image1;
    TLabel *lbTControl;
        TPanel *Panel3;
        TPanel *Panel49;
        TImage *Image20;
        TLabel *Label12;
        TPanel *pnRAL;
        TPanel *Panel10;
        TPanel *Panel24;
        TImage *Image3;
        TLabel *Label1;
        TPanel *pnWRK;
        TImage *Image10;
        TPanel *Panel5;
        TPanel *Panel20;
        TImage *Image9;
        TLabel *Label26;
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
        TBitBtn *btMan1_A;
        TBitBtn *btMan1_B;
        TBitBtn *btMan1_C;
        TBitBtn *btMan1_D;
        TBitBtn *btMan1_E;
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
        TBitBtn *btMan2_A;
        TBitBtn *btMan2_B;
        TBitBtn *btMan2_C;
        TBitBtn *btMan2_D;
        TBitBtn *btMan2_E;
        TPanel *Panel26;
        TPanel *Panel27;
        TImage *Image11;
        TLabel *Label2;
        TPanel *pnBinCnt1;
        TPanel *Panel29;
        TPanel *Panel30;
        TImage *Image12;
        TLabel *Label3;
        TPanel *pnBinCnt2;
        TPanel *Panel33;
        TPanel *Panel35;
        TImage *Image13;
        TLabel *Label4;
        TPanel *pnBinCnt3;
        TPanel *Panel37;
        TPanel *Panel38;
        TImage *Image14;
        TLabel *Label5;
        TPanel *pnBinCnt4;
        TPanel *Panel40;
        TPanel *Panel41;
        TImage *Image15;
        TLabel *Label6;
        TPanel *pnBinCnt5;
        TPanel *Panel28;
        TPanel *Panel31;
        TImage *Image16;
        TLabel *Label7;
        TPanel *pnRailTemp;
        TPanel *Panel39;
        TPanel *Panel42;
        TImage *Image17;
        TLabel *Label8;
        TPanel *pnWorkTemp;
        TBitBtn *btHeatOn;
        TPanel *pnSetTemp1;
        TPanel *pnSetTemp2;
        TLabel *Label9;
        TPanel *pnWorkCycle;
        TButton *Button1;
        TPanel *Panel36;
        TPanel *Panel43;
        TImage *Image18;
        TLabel *Label10;
        TPanel *pnRAB;
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
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall pnOptn0Click(TObject *Sender);
        void __fastcall pnBinCnt1DblClick(TObject *Sender);
        void __fastcall btLotEndClick(TObject *Sender);
        void __fastcall btLotOpenClick(TObject *Sender);
        void __fastcall btHeatOnClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);

private:	// User declarations
    void DM_Init   (); //Data Man Init.
    void Man_Init  (); //Manual Button , Tag Init.
    void LT_Display(); //Lot Display
    void OP_Dispaly(); //Option Display
    void LV_Display(); //Level Display
    void __fastcall SetManualBtn(TBitBtn *_btn , String _sCaption , int _iTag) ;

public:		// User declarations
    __fastcall TFrmOperation(TComponent* Owner);
    bool __fastcall CheckStripDisappear();
    bool __fastcall CheckStripUnknwon  ();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
#endif
