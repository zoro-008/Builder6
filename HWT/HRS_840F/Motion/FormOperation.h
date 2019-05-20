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
        TTabSheet *TabSheet3;
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
        TPanel *Panel6;
        TPanel *Panel7;
        TBitBtn *btStart;
        TBitBtn *btStop;
        TBitBtn *btReset;
        TBitBtn *btOperator;
        TBitBtn *btAllHome;
        TPanel *plPassWord;
        TPanel *Panel19;
        TLabel *Label17;
        TBitBtn *btPasswordClose;
        TBitBtn *btEngr;
        TBitBtn *btOper;
        TBitBtn *btMast;
        TPanel *Panel8;
        TPanel *Panel9;
        TStringGrid *sgDayInfo;
        TPanel *Panel10;
        TPanel *Panel11;
        TStringGrid *sgLotInfo;
        TPanel *Panel14;
        TPanel *Panel15;
        TPanel *Panel2;
        TPanel *Panel3;
        TGroupBox *GroupBox1;
        TGroupBox *GroupBox2;
        TPanel *pnLDR_R;
        TBitBtn *btRPackVccm;
        TBitBtn *btFPackVccm;
        TPanel *Panel12;
        TPanel *Panel13;
        TPanel *Panel23;
        TLabel *Label28;
        TPanel *pnLoadingStop;
        TGroupBox *GroupBox3;
        TGroupBox *GroupBox4;
        TGroupBox *GroupBox5;
        TPanel *pnULD_R;
        TGroupBox *GroupBox6;
        TPanel *Panel18;
        TBitBtn *btLotOpen;
        TBitBtn *btLotEnd;
        TPanel *Panel39;
        TImage *Image5;
        TPanel *Panel16;
        TGroupBox *GroupBox11;
        TPanel *pnULD_RPick;
        TGroupBox *GroupBox7;
        TPanel *pnLDR_F;
        TGroupBox *GroupBox8;
        TGroupBox *GroupBox9;
        TGroupBox *GroupBox10;
        TPanel *pnULD_F;
        TGroupBox *GroupBox12;
        TPanel *pnULD_FPick;
        TBitBtn *btRPackEjct;
        TBitBtn *btFPackEjct;
        TBitBtn *btRPickVccm;
        TBitBtn *btFPickVccm;
        TBitBtn *btRPickEjct;
        TBitBtn *btFPickEjct;
        TGroupBox *GroupBox13;
        TPanel *pnROpenVccm;
        TGroupBox *GroupBox14;
        TPanel *pnFOpenVccm;
        TGroupBox *GroupBox15;
        TPanel *pnRPickVccm;
        TGroupBox *GroupBox16;
        TPanel *pnFPickVccm;
        TTimer *tmUpdate;
        TPanel *Panel26;
        TGroupBox *GroupBox17;
        TPanel *pnBinCnt1;
        TGroupBox *GroupBox18;
        TPanel *pnBinCnt2;
        TGroupBox *GroupBox19;
        TListBox *lbErr;
        TGroupBox *GroupBox20;
        TPanel *pnRMaskDetect;
        TGroupBox *GroupBox21;
        TPanel *pnFMaskDetect;
        TTimer *tmTowerLamp;
        TBitBtn *btAllBinClear;
        TPanel *Panel17;
        TLabel *lbRed;
        TLabel *lbGrn;
        TLabel *lbYel;
        TPanel *Panel20;
        TLabel *Label2;
        TPanel *pnFRunSkip;
        TPanel *Panel22;
        TLabel *Label5;
        TPanel *pnRRunSkip;
        TBitBtn *btAirBlwr1_R;
        TBitBtn *btAirBlwr1_F;
        TBitBtn *btAirBlwr2_R;
        TBitBtn *btAirBlwr2_F;
        TTabSheet *TabSheet4;
        TTabSheet *TabSheet5;
        TTabSheet *TabSheet6;
        TBitBtn *btMan2_1;
        TBitBtn *btMan2_8;
        TBitBtn *btMan2_9;
        TBitBtn *btMan2_2;
        TBitBtn *btMan2_3;
        TBitBtn *btMan2_10;
        TBitBtn *btMan2_11;
        TBitBtn *btMan2_4;
        TBitBtn *btMan2_5;
        TBitBtn *btMan2_12;
        TBitBtn *btMan2_6;
        TBitBtn *btMan2_13;
        TBitBtn *btMan2_7;
        TBitBtn *btMan2_14;
        TBitBtn *btMan4_8;
        TBitBtn *btMan4_1;
        TBitBtn *btMan4_2;
        TBitBtn *btMan4_9;
        TBitBtn *btMan4_3;
        TBitBtn *btMan4_10;
        TBitBtn *btMan4_4;
        TBitBtn *btMan4_11;
        TBitBtn *btMan4_5;
        TBitBtn *btMan4_12;
        TBitBtn *btMan4_6;
        TBitBtn *btMan4_13;
        TBitBtn *btMan4_7;
        TBitBtn *btMan4_14;
        TBitBtn *btMan6_8;
        TBitBtn *btMan6_1;
        TBitBtn *btMan6_2;
        TBitBtn *btMan6_9;
        TBitBtn *btMan6_3;
        TBitBtn *btMan6_10;
        TBitBtn *btMan6_4;
        TBitBtn *btMan6_11;
        TBitBtn *btMan6_5;
        TBitBtn *btMan6_12;
        TBitBtn *btMan6_6;
        TBitBtn *btMan6_13;
        TBitBtn *btMan6_7;
        TBitBtn *btMan6_14;
        TBitBtn *btLdrAirBlw_R;
        TBitBtn *btLdrAirBlw_F;
        void __fastcall btStopClick(TObject *Sender);
        void __fastcall btResetClick(TObject *Sender);
        void __fastcall btAllHomeClick(TObject *Sender);
        void __fastcall btOperatorClick(TObject *Sender);
        void __fastcall btOperClick(TObject *Sender);
        void __fastcall btPasswordCloseClick(TObject *Sender);
        void __fastcall btEngrClick(TObject *Sender);
        void __fastcall btMan1_1Click(TObject *Sender);
        void __fastcall pnLoadingStopDblClick(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall btRPackVccmClick(TObject *Sender);
        void __fastcall btLotOpenClick(TObject *Sender);
        void __fastcall btLotEndClick(TObject *Sender);
        void __fastcall btMastClick(TObject *Sender);
        void __fastcall pnLoadingStopClick(TObject *Sender);
        void __fastcall pnBinCnt2DblClick(TObject *Sender);
        void __fastcall pnBinCnt1DblClick(TObject *Sender);
        void __fastcall btAllBinClearClick(TObject *Sender);
        void __fastcall tmTowerLampUpdate(TObject *Sender);
        void __fastcall btStartClick(TObject *Sender);


private:	// User declarations

protected:
    AnsiString  m_sPartName ;

public:		// User declarations
    __fastcall TFrmOperation(TComponent* Owner);

    bool bFlip;

    TFraMotr      *FraMotor   [MAX_MOTR];

    int iTemp ;

    TDateTime tViewTime ;

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
#endif
