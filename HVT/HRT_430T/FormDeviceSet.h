//---------------------------------------------------------------------------

#ifndef FormDeviceSetH
#define FormDeviceSetH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Buttons.hpp>
#include <jpeg.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
#include "Timer.h"
#include <ImgList.hpp>
class TFrmDeviceSet : public TForm
{
__published:	// IDE-managed Components
    TTimer *tmUpdateTimer;
        TImageList *ImageList1;
        TPageControl *pcDeviceSet;
        TTabSheet *tsDeviceInfo;
        TGroupBox *GroupBox10;
        TImage *Image2;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TStaticText *lbRowPitch;
        TEdit *edRowPitch;
        TStaticText *lbColPitch;
        TEdit *edColPitch;
        TStaticText *lbRowGroupCount;
        TEdit *edRowGroupCount;
        TStaticText *lbColCount;
        TEdit *edColCount;
        TStaticText *lbRowGroupGap;
        TEdit *edRowGroupGap;
        TStaticText *lbColGroupGap;
        TEdit *edColGroupGap;
        TStaticText *lbRowCount;
        TEdit *edRowCount;
        TStaticText *lbSrColCntrPitch;
        TEdit *edColGroupCount;
        TGroupBox *GroupBox11;
        TImage *Image3;
        TStaticText *StaticText8;
        TEdit *edCsSlotCnt;
        TStaticText *StaticText10;
        TEdit *edCsSlotPitch;
        TTabSheet *TabSheet3;
        TImage *Image7;
        TGroupBox *GroupBox1;
        TPanel *pnLd1Z;
        TGroupBox *GroupBox23;
        TPanel *pnLd1Y;
        TPanel *Panel6;
        TRadioGroup *rgLDRJogUnit;
        TEdit *edLDRJogUnit;
        TPageControl *PageControl3;
        TTabSheet *TabSheet8;
        TGroupBox *GroupBox21;
        TLabel *Label70;
        TLabel *Label71;
        TLabel *lbLDR_YCmdPos;
        TLabel *lbLDR_YEncPos;
        TLabel *lbLDR_YHome;
        TLabel *lbLDR_YPLim;
        TLabel *lbLDR_YNLim;
        TLabel *lbLDR_YAlarm;
        TLabel *lbLDR_YServo;
        TLabel *lbLDR_YStop;
        TLabel *lbLDR_YHomeDone;
        TBitBtn *btLdr_YBw;
        TBitBtn *btLdr_YFw;
        TGroupBox *GroupBox22;
        TLabel *Label81;
        TLabel *Label82;
        TLabel *lbLDR_ZCmdPos;
        TLabel *lbLDR_ZEncPos;
        TLabel *lbLDR_ZHome;
        TLabel *lbLDR_ZPLim;
        TLabel *lbLDR_ZNLim;
        TLabel *lbLDR_ZAlarm;
        TLabel *lbLDR_ZServo;
        TLabel *lbLDR_ZStop;
        TLabel *lbLDR_ZHomeDone;
        TBitBtn *btLdr_ZUp;
        TBitBtn *btLdr_ZDn;
        TGroupBox *GroupBox31;
        TPanel *Panel11;
        TBitBtn *btLDR_In;
        TStaticText *StaticText17;
        TPanel *Panel22;
        TBitBtn *btLDR_Out;
        TStaticText *StaticText18;
        TPanel *Panel8;
        TBitBtn *btLDR_PusherBW;
        TBitBtn *btLDR_PusherFW;
        TStaticText *StaticText27;
        TTabSheet *tsPosition;
        TGroupBox *GroupBox5;
        TPanel *pnRl1X;
        TGroupBox *GroupBox6;
        TPanel *pnRl2X;
        TGroupBox *GroupBox24;
        TPanel *pnRl3X;
        TPanel *Panel1;
        TPageControl *PageControl1;
        TTabSheet *TabSheet2;
        TGroupBox *GroupBox12;
        TLabel *Label22;
        TLabel *Label21;
        TLabel *lbCmdP_Rl1X;
        TLabel *lbEncP_Rl1X;
        TLabel *lbHome_Rl1X;
        TLabel *lbPLim_Rl1X;
        TLabel *lbNLim_Rl1X;
        TLabel *lbAlar_Rl1X;
        TLabel *lbServ_Rl1X;
        TLabel *lbStop_Rl1X;
        TLabel *lbHDon_Rl1X;
        TBitBtn *btPrb_XBw;
        TBitBtn *btPrb_XFw;
        TGroupBox *GroupBox2;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *lbCmdP_Rl2X;
        TLabel *lbEncP_Rl2X;
        TLabel *lbHome_Rl2X;
        TLabel *lbPLim_Rl2X;
        TLabel *lbNLim_Rl2X;
        TLabel *lbAlar_Rl2X;
        TLabel *lbServ_Rl2X;
        TLabel *lbStop_Rl2X;
        TLabel *lbHDon_Rl2X;
        TBitBtn *btWrk_XBw;
        TBitBtn *btWrk_XFw;
        TGroupBox *GroupBox3;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *lbCmdP_Rl3X;
        TLabel *lbEncP_Rl3X;
        TLabel *lbHome_Rl3X;
        TLabel *lbPLim_Rl3X;
        TLabel *lbNLim_Rl3X;
        TLabel *lbAlar_Rl3X;
        TLabel *lbServ_Rl3X;
        TLabel *lbStop_Rl3X;
        TLabel *lbHDon_Rl3X;
        TBitBtn *btPsb_XBw;
        TBitBtn *btPsb_XFw;
        TRadioGroup *rgRALJogUnit;
        TEdit *edRALJogUnit;
        TGroupBox *GroupBox9;
        TPanel *Panel19;
        TBitBtn *btUp_RlClamp2;
        TBitBtn *btDn_RlClamp2;
        TStaticText *StaticText23;
        TPanel *Panel20;
        TBitBtn *btUp_RlClamp1;
        TBitBtn *btDn_RlClamp1;
        TStaticText *StaticText24;
        TPanel *Panel21;
        TBitBtn *btUp_RlClamp3;
        TBitBtn *btDn_RlClamp3;
        TStaticText *StaticText25;
        TPanel *Panel3;
        TBitBtn *btUp_IdCheck;
        TBitBtn *btDn_IdCheck;
        TStaticText *StaticText2;
        TPanel *Panel4;
        TBitBtn *btPRB_In;
        TStaticText *StaticText3;
        TTabSheet *TabSheet4;
        TGroupBox *GroupBox7;
        TPanel *pnPbT;
        TGroupBox *GroupBox8;
        TPanel *pnPbB;
        TButton *btInspStart;
        TButton *btToolChange;
        TButton *btCntctRpt;
        TButton *btResetRpt;
        TStringGrid *sgProbeCrnt;
        TPanel *pnProbeMotr;
        TPanel *Panel2;
        TPageControl *PageControl2;
        TTabSheet *TabSheet6;
        TGroupBox *GroupBox18;
        TLabel *Label53;
        TLabel *Label54;
        TLabel *lbCmdP_PbT;
        TLabel *lbEncP_PbT;
        TLabel *lbHome_PbT;
        TLabel *lbPLim_PbT;
        TLabel *lbNLim_PbT;
        TLabel *lbAlar_PbT;
        TLabel *lbServ_PbT;
        TLabel *lbStop_PbT;
        TLabel *lbHDon_PbT;
        TBitBtn *btWrk_TUp;
        TBitBtn *btWrk_TDn;
        TGroupBox *GroupBox13;
        TLabel *Label30;
        TLabel *Label31;
        TLabel *lbCmdP_PbB;
        TLabel *lbEncP_PbB;
        TLabel *lbHome_PbB;
        TLabel *lbPLim_PbB;
        TLabel *lbNLim_PbB;
        TLabel *lbAlar_PbB;
        TLabel *lbServ_PbB;
        TLabel *lbStop_PbB;
        TLabel *lbHDon_PbB;
        TBitBtn *btWrk_BUp;
        TBitBtn *btWrk_BDn;
        TRadioGroup *rgWRKJogUnit;
        TEdit *edWRKJogUnit;
        TTabSheet *TabSheet1;
        TGroupBox *GroupBox29;
        TPanel *pnSTLYT;
        TGroupBox *GroupBox30;
        TPanel *pnSTLYB;
        TGroupBox *GroupBox32;
        TPanel *pnREJXB;
        TPanel *Panel31;
        TRadioGroup *rgSTLJogUnit;
        TEdit *edSTLJogUnit;
        TPageControl *PageControl5;
        TTabSheet *TabSheet11;
        TGroupBox *GroupBox33;
        TLabel *Label44;
        TLabel *Label45;
    TLabel *lbSTL_YBCmdPos;
    TLabel *lbSTL_YBEncPos;
    TLabel *lbSTL_YBHome;
    TLabel *lbSTL_YBPLim;
    TLabel *lbSTL_YBNLim;
    TLabel *lbSTL_YBAlarm;
    TLabel *lbSTL_YBServo;
    TLabel *lbSTL_YBStop;
    TLabel *lbSTL_YBHomeDone;
    TBitBtn *btSTL_YBFw;
    TBitBtn *btSTL_YBBw;
        TGroupBox *GroupBox34;
        TLabel *Label59;
        TLabel *Label60;
    TLabel *lbSTL_YTCmdPos;
    TLabel *lbSTL_YTEncPos;
    TLabel *lbSTL_YTHome;
    TLabel *lbSTL_YTPLim;
    TLabel *lbSTL_YTNLim;
    TLabel *lbSTL_YTAlarm;
    TLabel *lbSTL_YTServo;
    TLabel *lbSTL_YTStop;
    TLabel *lbSTL_YTHomeDone;
    TBitBtn *btSTL_YTBw;
    TBitBtn *btSTL_YTFw;
        TGroupBox *GroupBox35;
        TLabel *Label72;
        TLabel *Label73;
        TLabel *lbREJ_XCmdPos;
        TLabel *lbREJ_XEncPos;
        TLabel *lbREJ_XHome;
        TLabel *lbREJ_XPLim;
        TLabel *lbREJ_XNLim;
        TLabel *lbREJ_XAlarm;
        TLabel *lbREJ_XServo;
        TLabel *lbREJ_XStop;
        TLabel *lbREJ_XHomeDone;
        TBitBtn *btREJ_XFw;
        TBitBtn *btREJ_XBw;
        TGroupBox *GroupBox36;
        TPanel *Panel5;
        TBitBtn *btPSB_PusherBw;
        TBitBtn *btPSB_PusherFw;
        TStaticText *StaticText5;
        TPanel *Panel36;
    TBitBtn *btSTL_YTCmpUp;
    TBitBtn *btSTL_YTCmpDn;
        TStaticText *StaticText56;
        TPanel *Panel37;
    TBitBtn *btSTL_YBCmpUp;
    TBitBtn *btSTL_YBCmpDn;
        TStaticText *StaticText57;
        TPanel *Panel7;
        TBitBtn *btPSB_Out;
        TStaticText *StaticText45;
        TTabSheet *TabSheet7;
        TImage *Image9;
        TGroupBox *GroupBox25;
        TPanel *pnULDY;
        TGroupBox *GroupBox26;
        TPanel *pnULDZ;
        TPanel *Panel26;
        TRadioGroup *rgULDJogUnit;
        TEdit *edULDJogUnit;
        TPageControl *PageControl4;
        TTabSheet *TabSheet10;
        TGroupBox *GroupBox4;
        TLabel *Label41;
        TLabel *Label43;
        TLabel *lbULD_YCmdPos;
        TLabel *lbULD_YEncPos;
        TLabel *lbULD_YHome;
        TLabel *lbULD_YPLim;
        TLabel *lbULD_YNLim;
        TLabel *lbULD_YAlarm;
        TLabel *lbULD_YServo;
        TLabel *lbULD_YStop;
        TLabel *lbULD_YHomeDone;
        TBitBtn *btULD_YBw;
        TBitBtn *btULD_YFw;
        TGroupBox *GroupBox27;
        TLabel *Label55;
        TLabel *Label56;
        TLabel *lbULD_ZCmdPos;
        TLabel *lbULD_ZEncPos;
        TLabel *lbULD_ZHome;
        TLabel *lbULD_ZPLim;
        TLabel *lbULD_ZNLim;
        TLabel *lbULD_ZAlarm;
        TLabel *lbULD_ZServo;
        TLabel *lbULD_ZStop;
        TLabel *lbULD_ZHomeDone;
        TBitBtn *btULD_ZUp;
        TBitBtn *btULD_ZDn;
        TGroupBox *GroupBox28;
        TPanel *Panel27;
        TBitBtn *btULD_CmpUp;
        TBitBtn *btULD_CmpDn;
        TStaticText *StaticText4;
        TPanel *Panel29;
        TBitBtn *btULD_In;
        TStaticText *StaticText50;
        TPanel *Panel30;
        TBitBtn *btULD_Out;
        TStaticText *StaticText51;
        TBitBtn *btSaveDevice;
        TTabSheet *TabSheet12;
        TPageControl *PageControl6;
        TTabSheet *TabSheet13;
        TTabSheet *TabSheet14;
        TPanel *Panel9;
        TLabel *Label9;
        TLabel *Label14;
        TLabel *Label27;
        TLabel *Label28;
        TPanel *Panel12;
        TImage *Image6;
        TLabel *Label32;
        TEdit *edLotEndMgzCnt;
        TEdit *edWaitDelay;
        TPanel *Panel16;
        TLabel *Label29;
        TLabel *Label33;
        TLabel *Label34;
        TLabel *Label35;
        TPanel *Panel17;
        TImage *Image5;
        TLabel *Label36;
        TEdit *edPreRailOutVel;
        TEdit *edPostRailOutVel;
        TPanel *Panel24;
        TLabel *Label39;
        TLabel *Label40;
        TPanel *Panel25;
        TImage *Image10;
        TLabel *Label38;
        TEdit *edREJPitch;
        TPanel *Panel18;
        TImage *Image11;
        TPanel *Panel23;
        TImage *Image8;
        TLabel *Label42;
        TCheckBox *cbLtHeaterNum2;
        TCheckBox *cbLtHeaterNum3;
        TCheckBox *cbRtHeaterNum2;
        TCheckBox *cbRtHeaterNum3;
        TGroupBox *gbPinSetting;
        TStaticText *StaticText6;
        TStaticText *StaticText7;
        TComboBox *cb2Ch1P;
        TStaticText *StaticText12;
        TStaticText *StaticText9;
        TComboBox *cb2Ch1N;
        TComboBox *cb1Ch1P;
        TComboBox *cb1Ch1N;
        TComboBox *cb2Ch2P;
        TStaticText *StaticText32;
        TStaticText *StaticText33;
        TComboBox *cb2Ch2N;
        TComboBox *cb1Ch2P;
        TComboBox *cb1Ch2N;
        TComboBox *cb2Ch3P;
        TStaticText *StaticText34;
        TStaticText *StaticText35;
        TComboBox *cb2Ch3N;
        TComboBox *cb1Ch3P;
        TComboBox *cb1Ch3N;
        TImage *Image4;
        TImage *Image1;
        TGroupBox *GroupBox16;
        TEdit *edZennerHigh;
        TEdit *edZennerLow;
        TEdit *ed1ChLow;
        TEdit *ed1ChHigh;
        TStaticText *StaticText11;
        TStaticText *StaticText14;
        TEdit *ed2ChLow;
        TEdit *ed2ChHigh;
        TStaticText *StaticText19;
        TStaticText *StaticText20;
        TStaticText *StaticText21;
        TCheckBox *cbUseZenner;
        TGroupBox *GroupBox17;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label23;
        TLabel *Label24;
        TLabel *Label25;
        TLabel *Label26;
        TStaticText *StaticText39;
        TEdit *ed1ChDelay;
        TStaticText *StaticText40;
        TEdit *ed1ChLedTime;
        TStaticText *StaticText41;
        TEdit *ed1ChZenTime;
        TStaticText *StaticText42;
        TEdit *ed2ChDelay;
        TStaticText *StaticText43;
        TEdit *ed2ChLedTime;
        TStaticText *StaticText44;
        TEdit *ed2ChZenTime;
        TGroupBox *GroupBox14;
        TLabel *Label5;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label8;
        TLabel *Label37;
        TEdit *edContactCnt;
        TStaticText *StaticText22;
        TEdit *edReGrabCnt1;
        TStaticText *StaticText26;
        TEdit *edReGrabCnt2;
        TStaticText *StaticText28;
        TStaticText *StaticText1;
        TEdit *edReInspCnt;
        TEdit *edReGrabPs1;
        TStaticText *StaticText30;
        TEdit *edReGrabPs2;
        TStaticText *StaticText31;
        TStaticText *StaticText36;
        TEdit *edLimInsFailCnt;
        TEdit *edPinCnt;
        TStaticText *StaticText29;
        TEdit *edPrbSttDelay;
        TStaticText *StaticText38;
    TButton *btAutoCal1;
    TButton *btAutoCal2;
        TGroupBox *gbProbeTest;
        TLabel *Label15;
        TLabel *Label16;
        TEdit *edProbeTime;
        TBitBtn *btStartTest;
        TBitBtn *btStopTest;
        TEdit *edPinNumber;
        TMemo *mmProbeTest;
        TGroupBox *GroupBox20;
        TLabel *Label47;
        TLabel *Label48;
        TLabel *lbPSB_YCmdPos;
        TLabel *lbPSB_YEncPos;
        TLabel *lbPSB_YHome;
        TLabel *lbPSB_YPLim;
        TLabel *lbPSB_YNLim;
        TLabel *lbPSB_YAlarm;
        TLabel *lbPSB_YServo;
        TLabel *lbPSB_YStop;
        TLabel *lbPSB_YHomeDone;
        TBitBtn *btPSB_YBw;
        TBitBtn *btPSB_YFw;
    TPanel *Panel13;
    TBitBtn *btPSB_CmpUp;
    TBitBtn *btPSB_CmpDn;
    TStaticText *StaticText46;
    TPanel *Panel14;
    TLabel *Label49;
    TLabel *Label50;
    TLabel *Label51;
    TLabel *Label52;
    TPanel *Panel15;
    TImage *Image12;
    TLabel *Label57;
    TEdit *edPSBReGripCol;
    TEdit *edPSBReGripOfs;
    TButton *btPSBAutoCAL;
    TImage *Image13;
    TImage *Image14;
    TGroupBox *gbDaegyeom;
    TLabel *Label46;
    TStaticText *lbSwitchingDly;
    TEdit *edSwitchingDly;
    TStaticText *StaticText47;
    TEdit *edDgChipCnt;
    TLabel *Label58;
    TGroupBox *GroupBox37;
    TPanel *Panel28;
    TBitBtn *btWRK_2ClampUp;
    TBitBtn *btWRK_2ClampDn;
    TStaticText *StaticText48;
    TPanel *Panel32;
    TBitBtn *btWRK_1ClampUp;
    TBitBtn *btWRK_1ClampDn;
    TStaticText *StaticText49;
    TPanel *Panel33;
    TBitBtn *btWRK_3ClampUp;
    TBitBtn *btWRK_3ClampDn;
    TStaticText *StaticText52;
    TPanel *Panel34;
    TBitBtn *btPSB_Out2;
    TStaticText *StaticText53;
    TPanel *Panel10;
    TBitBtn *btLDR_CmpUp;
    TBitBtn *btLDR_CmpDn;
    TStaticText *StaticText16;
    TLabel *lbTrimPtchOfs;
    TLabel *Label62;
    TEdit *edTrimPtchOfs;
    TStaticText *stWorkptchOfs;
    TEdit *edWorkPtchOfs;
    TLabel *Label61;
    TPanel *Panel35;
    TBitBtn *BtVaccumIn;
    TStaticText *lbVaccum;
    TPanel *Panel38;
    TBitBtn *btVaccumOut;
    TStaticText *lbEjector;
        TLabel *Label63;
        TLabel *Label64;
        TEdit *edTrimRjtDly;
        TLabel *Label65;
        TLabel *Label66;
        TEdit *edTrimSnrDly;
        TGroupBox *gbProbeComTest;
        TButton *btPbSet;
        TButton *btPbStart;
        TButton *btPbOpen;
        TButton *btPbClose;
        TCheckBox *cbChRvsCrnt;
        TCheckBox *cbUseTwiceInsp;
    TStaticText *StaticText13;
    TEdit *ed1ChLedCur;
    TEdit *ed2ChLedCur;
    TLabel *Label6;
    TLabel *Label7;
    TStaticText *StaticText37;
    TEdit *ed1ChZenCur;
    TEdit *ed2ChZenCur;
        TGroupBox *GroupBox15;
        TStaticText *StaticText15;
        TEdit *edContactPinCnt;
        TBitBtn *brContactCntReset;
        TStaticText *StaticText54;
        TEdit *edLimContactPinCnt;
    TPanel *Panel39;
    TBitBtn *btPSB_SrtTFw;
    TBitBtn *btPSB_SrtTBw;
    TStaticText *StaticText55;
    TPanel *Panel40;
    TBitBtn *btPSB_SrtBFw;
    TBitBtn *btPSB_SrtBBw;
    TStaticText *StaticText58;
    void __fastcall btSavePosClick(TObject *Sender);
    void __fastcall tmUpdateTimerTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btMtFwDn(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btMtUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btMtBwDn(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall pcDeviceSetChange(TObject *Sender);
        void __fastcall btLDR_CmpUpClick(TObject *Sender);
        void __fastcall btLDR_CmpDnClick(TObject *Sender);
    void __fastcall btStartTestClick(TObject *Sender);
        void __fastcall mmProbeTestDblClick(TObject *Sender);
        void __fastcall btToolChangeClick(TObject *Sender);
        void __fastcall btResetRptClick(TObject *Sender);
        void __fastcall btCntctRptClick(TObject *Sender);
        void __fastcall btAutoCal1Click(TObject *Sender);
        void __fastcall btAutoCal2Click(TObject *Sender);
    void __fastcall btLDR_InClick(TObject *Sender);
    void __fastcall btInspStartClick(TObject *Sender);
    void __fastcall btStopTestClick(TObject *Sender);
    void __fastcall btPSBAutoCALClick(TObject *Sender);
        void __fastcall brContactCntResetClick(TObject *Sender);
    void __fastcall btPSB_SrtTFwClick(TObject *Sender);
private:	// User declarations
    //Active Radio Button Page.
    TRadioGroup  * rgUnitUser ;
    TEdit        * edUnitUser ;
    CDelayTimer    m_tmProbeTest ;
public:		// User declarations
    __fastcall TFrmDeviceSet(TComponent* Owner);
    void __fastcall UpdateDevInfo(bool bTo);
    void __fastcall UpdateDevOptn(bool bTo);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
#endif
