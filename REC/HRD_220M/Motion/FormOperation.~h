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

#include "FrameMotr.h"

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
        TPanel *pnDispnrSkip;
        TPanel *pnTotalTime;
        TLabel *Label11;
        TStringGrid *sgLotInfo;
        TStringGrid *sgDayInfo;
    TPanel *Panel2;
    TPanel *Panel3;
        TLabel *Label24;
        TBitBtn *btAllHome;
        TImage *Img1;
        TTimer *ImgTimer;
        TPanel *pnJIG;
        TPanel *pnDSP;
        TPanel *pnPIK;
        TLabel *pnPassword;
        TButton *btCycleJigPick;
        TButton *btCycleDispenser;
        TButton *btCyclePlace;
        TLabel *Label12;
        TRadioGroup *rgJogUnit;
        TEdit *edJogUnit;
        TPanel *pnMotrJog0;
        TPanel *pnMotrJog1;
        TEdit *edInputJigPos;
        TButton *btInputJigPos;
        TPanel *pnCycleTime;

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
        void __fastcall btAllHomeClick(TObject *Sender);
        void __fastcall btWorkEndClick(TObject *Sender);
        void __fastcall btCycleJigPickClick(TObject *Sender);
        void __fastcall btCycleDispenserClick(TObject *Sender);
        void __fastcall btCyclePlaceClick(TObject *Sender);
        void __fastcall ImgTimerTimer(TObject *Sender);
        void __fastcall rgJogUnitClick(TObject *Sender);
        void __fastcall btInputJigPosClick(TObject *Sender);


private:	// User declarations

public:		// User declarations
    __fastcall TFrmOperation(TComponent* Owner);
    void __fastcall ChangeImage();

    int iTemp ;
    TFraMotr      *FraMotor   [MAX_MOTR];
    int iiTemp;

    TDateTime tViewTime ;

    //bool m_bLotOpenMsgShow;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
#endif
