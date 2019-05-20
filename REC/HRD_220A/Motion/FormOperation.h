//---------------------------------------------------------------------------
#ifndef FormOperationH
#define FormOperationH
//---------------------------------------------------------------------------
//#include "SortCmn.h"
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
class TFrmOperation : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel6;
    TPanel *Panel7;
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
    TPanel *Panel16;
    TPanel *Panel18;
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
        TPanel *pnTotalTime;
        TLabel *Label11;
        TStringGrid *sgLotInfo;
        TStringGrid *sgDayInfo;
        TTimer *tmUpdate;
        TTimer *Timer1;
        TPanel *Panel2;
        TPanel *Panel3;
        TPanel *pnDataView;
        TPageControl *pcDataView;
        TTabSheet *TabSheet5;
        TPanel *pnPCKVccm;
        TPanel *pnSTGVccm;
        TSpeedButton *btStart;
        TSpeedButton *btStop;
        TSpeedButton *btReset;
        TSpeedButton *btAllHome;
        TSpeedButton *btOperator;
        TGroupBox *GroupBox2;
        TPanel *pnLDR_RTop;
        TGroupBox *GroupBox15;
        TPanel *pnLDR_RBtm;
        TGroupBox *GroupBox1;
        TPanel *pnPRI_R;
        TGroupBox *GroupBox11;
        TPanel *pnIDX_R;
        TGroupBox *GroupBox5;
        TPanel *pnPSI_R;
        TGroupBox *GroupBox8;
        TPanel *pnULD_RTop;
        TGroupBox *GroupBox17;
        TPanel *pnULD_RBtm;
        TGroupBox *gbSTG;
        TPanel *pnSTG;
        TGroupBox *gbDSP;
        TPanel *pnDSP;
        TGroupBox *GroupBox21;
        TPanel *pnLDR_FTop;
        TGroupBox *GroupBox22;
        TPanel *pnLDR_FBtm;
        TGroupBox *GroupBox23;
        TPanel *pnPRI_F;
        TGroupBox *GroupBox24;
        TPanel *pnIDX_F;
        TGroupBox *GroupBox25;
        TPanel *pnPSI_F;
        TGroupBox *GroupBox26;
        TPanel *pnULD_FTop;
        TGroupBox *GroupBox27;
        TPanel *pnULD_FBtm;
        TGroupBox *gbALN;
        TPanel *pnALN;
        TGroupBox *gbPCK;
        TPanel *pnPCK;
        TPanel *Panel5;
        TPanel *Panel12;
        TPanel *Panel13;
        TLabel *lbHICTitle;
        TLabel *lbStgThickness;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *lbFinishTitle;
        TLabel *lbStgWorkEndThickness;
        TLabel *Label18;
        TLabel *lbStgEpoxyThickness;
        TLabel *lbDisprShowTime;
        TLabel *Label21;
        TLabel *lbPckThickness;
        TLabel *lbDIETitle;
        TLabel *lbPckEmptyToolHeight;
        TLabel *Label15;
        TPanel *Panel14;
        TPanel *Panel15;
        TPanel *pnRecipe;
        TPanel *Panel21;
        TPanel *Panel22;
        TSpeedButton *btLight;
        TSpeedButton *btNiddleZCal;
        TGroupBox *gbVSN_R;
        TPanel *pnVSN_R;
        TGroupBox *gbVSN_F;
        TPanel *pnVSN_F;
        TPanel *Panel17;
        TPanel *Panel20;
        TPanel *pnCycleMsg;
        TSpeedButton *btMonitor;
        TLabel *Label14;
        TLabel *lbDispPressure;
        TLabel *lbPushWeight;
        TLabel *Label22;
        TLabel *lbAttachGap;
        TLabel *Label24;
        TSpeedButton *btLdrUldMoveWait;
        TSpeedButton *btStgFlatness;
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
        void __fastcall btPR1InputClick(TObject *Sender);
    void __fastcall pnDayRunTimeDblClick(TObject *Sender);
        void __fastcall btAllHomeClick(TObject *Sender);
        void __fastcall GroupBox23Click(TObject *Sender);
        void __fastcall GroupBox24Click(TObject *Sender);
        void __fastcall GroupBox11Click(TObject *Sender);
        void __fastcall pnDataViewClick(TObject *Sender);
        void __fastcall pnPRI_R_Click(TObject *Sender);
        void __fastcall btLightClick(TObject *Sender);
        void __fastcall btNiddleZCalClick(TObject *Sender);
        void __fastcall btMonitorClick(TObject *Sender);
        void __fastcall GroupBox1Click(TObject *Sender);
        void __fastcall pnCycleMsgClick(TObject *Sender);
        void __fastcall btLdrUldMoveWaitClick(TObject *Sender);
        void __fastcall btStgFlatnessClick(TObject *Sender);

private:	// User declarations

public:		// User declarations
    __fastcall TFrmOperation(TComponent* Owner);


public:
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
#endif
