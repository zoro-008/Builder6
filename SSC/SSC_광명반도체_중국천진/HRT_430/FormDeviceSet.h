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
#include "SMDllDefine.h"
#include "Timer.h"
class TFrmDeviceSet : public TForm
{
__published:	// IDE-managed Components
    TPageControl *pcDeviceSet;
    TTabSheet *tsDeviceInfo;
    TTabSheet *tsPosition;
    TTabSheet *TabSheet3;
    TTabSheet *TabSheet4;
    TTabSheet *TabSheet5;
    TTimer *tmUpdateTimer;
    TGroupBox *GroupBox5;
    TPanel *pnRl1X;
    TGroupBox *GroupBox6;
    TPanel *pnRl2X;
    TGroupBox *GroupBox7;
    TPanel *pnPbT;
    TGroupBox *GroupBox8;
    TPanel *pnPbB;
    TEdit *edJogUsr_Rail;
    TGroupBox *GroupBox9;
    TGroupBox *GroupBox10;
    TGroupBox *GroupBox11;
    TImage *Image3;
    TImage *Image2;
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
    TStaticText *StaticText8;
    TEdit *edCsSlotCnt;
    TStaticText *StaticText10;
    TEdit *edCsSlotPitch;
    TEdit *edJogUsr_Head;
    TEdit *edJogUsr_Trim;
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
    TGroupBox *GroupBox4;
    TPanel *pnRl3X;
    TGroupBox *GroupBox15;
    TTabSheet *TabSheet1;
    TBitBtn *btSaveDevice;
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
    TPanel *Panel1;
    TPageControl *PageControl1;
    TTabSheet *TabSheet2;
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
    TGroupBox *GroupBox1;
    TPanel *pnLd1Z;
    TGroupBox *GroupBox23;
    TPanel *pnLd1Y;
    TGroupBox *GroupBox31;
    TPanel *Panel10;
    TBitBtn *btLDR_CmpUp;
    TBitBtn *btLDR_CmpDn;
    TStaticText *StaticText16;
    TPanel *Panel11;
    TBitBtn *btLDR_In;
    TStaticText *StaticText17;
    TPanel *Panel22;
    TBitBtn *btLDR_Out;
    TStaticText *StaticText18;
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
    TRadioGroup *rgRALJogUnit;
    TEdit *edRALJogUnit;
    TRadioGroup *rgWRKJogUnit;
    TEdit *edWRKJogUnit;
    TPanel *Panel3;
    TBitBtn *btUp_IdCheck;
    TBitBtn *btDn_IdCheck;
    TStaticText *StaticText2;
    TPanel *Panel4;
    TBitBtn *btPRB_In;
    TStaticText *StaticText3;
    TImage *Image1;
    TStaticText *StaticText6;
    TStaticText *StaticText7;
    TComboBox *cb2Ch1P;
    TStaticText *StaticText12;
    TStaticText *StaticText13;
    TStaticText *StaticText9;
    TComboBox *cb2Ch1N;
    TComboBox *cb1Ch1P;
    TComboBox *cb1Ch1N;
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
    TGroupBox *GroupBox14;
    TEdit *edContactCnt;
    TStaticText *StaticText22;
    TEdit *edReGrabCnt1;
    TStaticText *StaticText26;
    TEdit *edReGrabCnt2;
    TStaticText *StaticText28;
    TStaticText *StaticText1;
    TEdit *edReInspCnt;
    TEdit *edLimAllFailCnt;
    TStaticText *StaticText15;
    TEdit *edReGrabPs1;
    TStaticText *StaticText30;
    TEdit *edReGrabPs2;
    TStaticText *StaticText31;
    TLabel *Label5;
    TLabel *Label3;
    TLabel *Label4;
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
        TEdit *ed1ChLedCur;
        TEdit *ed2ChLedCur;
    TLabel *Label6;
    TLabel *Label7;
    TStaticText *StaticText36;
    TEdit *edLimInsFailCnt;
    TLabel *Label8;
    TEdit *edPinCnt;
    TStaticText *StaticText29;
    TGroupBox *GroupBox17;
    TButton *btPbSet;
    TButton *btPbStart;
    TButton *btPbOpen;
    TButton *btPbClose;
    TCheckBox *cbUseZenner;
        TStaticText *StaticText37;
        TEdit *ed1ChZenCur;
        TEdit *ed2ChZenCur;
        TEdit *edPrbSttDelay;
        TStaticText *StaticText38;
        TImage *Image4;
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
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
    TGroupBox *GroupBox19;
    TEdit *edProbeTime;
    TBitBtn *btStartTest;
    TBitBtn *btStopTest;
    TLabel *Label15;
    TTimer *tmProbeTest;
        TEdit *edPinNumber;
        TLabel *Label16;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label23;
        TLabel *Label24;
        TLabel *Label25;
        TLabel *Label26;
        TMemo *mmProbeTest;
        TPanel *Panel9;
        TPanel *Panel12;
        TImage *Image6;
        TLabel *Label32;
        TLabel *Label9;
        TLabel *Label14;
        TEdit *edLotEndMgzCnt;
        TEdit *edWaitDelay;
        TLabel *Label27;
        TLabel *Label28;
        TButton *btInspStart;
        TButton *btToolChange;
        TPanel *Panel16;
        TPanel *Panel17;
        TImage *Image5;
        TLabel *Label36;
        TLabel *Label29;
        TLabel *Label33;
        TLabel *Label34;
        TLabel *Label35;
        TEdit *edPreRailOutVel;
        TEdit *edPostRailOutVel;
        TGroupBox *GroupBox20;
        TPanel *Panel5;
        TBitBtn *btUp_RlClamp2_;
        TBitBtn *btDn_RlClamp2_;
        TStaticText *StaticText45;
        TPanel *Panel7;
        TBitBtn *btUp_RlClamp1_;
        TBitBtn *btDn_RlClamp1_;
        TStaticText *StaticText46;
        TPanel *Panel13;
        TBitBtn *btUp_RlClamp3_;
        TBitBtn *btDn_RlClamp3_;
        TStaticText *StaticText47;
        TPanel *Panel14;
        TBitBtn *btUp_IdCheck_;
        TBitBtn *btDn_IdCheck_;
        TStaticText *StaticText48;
        TPanel *Panel15;
        TBitBtn *btPRB_In_;
        TStaticText *StaticText49;
        TImage *Image7;
        TButton *btCntctRpt;
        TButton *btResetRpt;
        TStringGrid *sgProbeCrnt;
        TButton *Button1;
        TButton *Button2;
        TLabel *Label37;
        TCheckBox *cbUseTwiceInsp;
    TBitBtn *btLDR_PusherBw;
    TBitBtn *btLDR_PusherFw;
    TStaticText *StaticText4;
    TStaticText *StaticText5;
        TPanel *Panel8;
        TStaticText *StaticText27;
        TBitBtn *btLD1_PusherBw;
        TBitBtn *btLD1_PusherFw;
        TGroupBox *GroupBox24;
        TBitBtn *btPinContactCntReset;
        TStaticText *StaticText50;
        TStaticText *StaticText51;
        TEdit *edPinCtCnt;
        TEdit *edLimPinCtCnt;
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
    void __fastcall btPbOpenClick(TObject *Sender);
        void __fastcall pcDeviceSetChange(TObject *Sender);
        void __fastcall btLDR_CmpUpClick(TObject *Sender);
        void __fastcall btLDR_CmpDnClick(TObject *Sender);
        void __fastcall btLDR_PusherFwClick(TObject *Sender);
    void __fastcall tmProbeTestTimer(TObject *Sender);
    void __fastcall btStartTestClick(TObject *Sender);
    void __fastcall btStopTestClick(TObject *Sender);
        void __fastcall mmProbeTestDblClick(TObject *Sender);
        void __fastcall btToolChangeClick(TObject *Sender);
        void __fastcall btResetRptClick(TObject *Sender);
        void __fastcall btCntctRptClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
    void __fastcall btLD1_PusherBwClick(TObject *Sender);
    void __fastcall btLD1_PusherFwClick(TObject *Sender);
        void __fastcall btPinContactCntResetClick(TObject *Sender);
private:	// User declarations
    bool __fastcall LanguageSel(EN_LAN_SEL lvType);
    //Active Radio Button Page.
    TRadioGroup  * rgUnitUser ;
    TEdit        * edUnitUser ;
    CDelayTimer    m_tmProbeTest ;
public:		// User declarations
    bool bComTest ;
    __fastcall TFrmDeviceSet(TComponent* Owner);
    void __fastcall UpdateDevInfo(bool bTo);
    void __fastcall UpdateDevOptn(bool bTo);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
#endif
































































































































































































































































































































































