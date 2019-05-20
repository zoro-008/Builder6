//---------------------------------------------------------------------------
#ifndef FormMainH
#define FormMainH
//---------------------------------------------------------------------------
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
//#include "TEdit.h"
//---------------------------------------------------------------------------
//#include "GThreadUnit.h"
#include "BarslerUnit.h"
#include "uRs232c.h"
//---------------------------------------------------------------------------
//모션 관련 Include
#include "NMC410S.h"
#include "MyUtil.h"
#include "PaixMotion.h"
#include "global.h"
#include "Timer.h"
#include <jpeg.hpp>
//typedef void (__closure *MsgFunc)(AnsiString);
//---------------------------------------------------------------------------

enum EN_OUTPUT_ID  {
    yETC_StartSw        =  0 , //
    yETC_StopSw         =  1 , //
    yETC_HomeSw         =  2 , //
    yETC_CoolingSw      =  3 , //
    yETC_CoolingFan     =  4 , //
    y005                =  5 , //
    y006                =  6 , //
    y007                =  7 , //
    y008                =  8 , //
};
enum EN_INPUT_ID {
    xETC_StartSw        =  0 , //
    xETC_StopSw         =  1 , //
    xETC_HomeSw         =  2 , //
    xETC_CoolingSw      =  3 , //
    xETC_Emergency      =  4 , //
    x005                =  5 , //
    x006                =  6 , //
    x007                =  7 , //
    x008                =  8 , //
};
class TFrmMain : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel2;
    TOpenPictureDialog *OpenPictureDialog1;
    TSavePictureDialog *SavePictureDialog1;
        TTimer *tmUpdate;
    TLabel *Label4;
    TLabel *Label5;
    TImage *Image1;
    TLabel *Label1;
    TLabel *lbStat;
    TLabel *lbVer;
    TLabel *lbDevice;
    TLabel *lbDate;
    TPanel *Panel3;
    TPanel *Panel5;
    TImage *Image2;
    TLabel *Label19;
    TPanel *pnView;
    TLabel *Label8;
    TPanel *Panel6;
    TPageControl *PageControl1;
    TTabSheet *TabSheet2;
    TLabel *Label12;
    TLabel *Label6;
    TLabel *Label7;
    TPanel *Panel11;
    TPanel *Panel21;
    TPanel *Panel22;
    TPanel *Panel27;
    TPanel *Panel28;
    TPanel *Panel29;
    TPanel *Panel30;
    TPanel *Panel31;
    TEdit *edAxis1_AccTime;
    TEdit *edAxis1_NorTime;
    TEdit *edAxis1_RPM;
    TPanel *Panel24;
    TPanel *Panel32;
    TPanel *Panel33;
    TPanel *Panel34;
    TPanel *Panel37;
    TEdit *edAxis1_Acc;
    TEdit *edAxis1_Vel;
    TEdit *edAxis1_Dist;
    TPanel *Panel25;
    TPanel *Panel38;
    TPanel *Panel39;
    TPanel *Panel40;
    TPanel *Panel41;
    TEdit *edAxis2_Acc;
    TEdit *edAxis2_Vel;
    TEdit *edAxis2_Dist;
    TTabSheet *TabSheet4;
    TLabel *lbCompile;
    TMemo *mmSeq;
    TButton *btSeqTest;
    TTabSheet *TabSheet5;
    TLabel *lbCompileEx;
    TMemo *mmSeqEx;
    TButton *btSeqExTest;
    TTabSheet *TabSheet3;
    TStringGrid *StringGrid1;
    TBitBtn *btRotorRun;
    TButton *Button1;
    TEdit *Edit1;
    TTabSheet *TabSheet1;
    TPanel *Panel1;
    TPanel *Panel4;
    TPanel *Panel17;
    TPanel *Panel10;
    TPanel *Panel12;
    TPanel *Panel13;
    TPanel *Panel14;
    TPanel *Panel15;
    TEdit *Edit_Field0;
    TEdit *Edit_Field1;
    TEdit *Edit_Field2;
    TEdit *Edit_Field3;
    TPanel *Panel16;
    TEdit *Edit_SecField0;
    TEdit *Edit_SecField1;
    TEdit *Edit_SecField2;
    TEdit *Edit_SecField3;
    TPanel *Panel7;
    TImage *Image3;
    TLabel *Label9;
    TPanel *Panel9;
    TMemo *mmMsg;
    TPanel *Panel23;
    TImage *Image4;
    TLabel *Label10;
    TPanel *Panel26;
    TPanel *Panel8;
    TLabel *Label2;
    TLabel *Label3;
    TEdit *Ed_Device;
    TEdit *Ed_CurDevice;
    TEdit *Ed_SelDevice;
    TStringGrid *sgDevice;
    TPanel *Panel42;
    TImage *Image5;
    TLabel *Label11;
    TPanel *Panel43;
    TBitBtn *btStop;
    TBitBtn *btAllHome;
    TBitBtn *btSeqRun;
    TBitBtn *btCooling;
    TBitBtn *btExit;
    TSpeedButton *btLive;
    TButton *Button4;
    TBitBtn *btAxis1RPMGo;
    TBitBtn *btAxis1DistGo;
    TBitBtn *btAxis2DistGo;
    TBitBtn *btAddDevice;
    TBitBtn *btDelete;
    TBitBtn *Bt_Load;
    TBitBtn *btSave;
        TTimer *tmHeat;
        TPanel *Panel60;
        TPanel *Panel66;
        TImage *Image6;
        TLabel *Label13;
        TPanel *Panel20;
        TPanel *Panel35;
        TPanel *Panel36;
        TSpeedButton *SBtn_Output00;
        TSpeedButton *SBtn_Output01;
        TSpeedButton *SBtn_Output02;
        TSpeedButton *SBtn_Output03;
        TSpeedButton *SBtn_Output04;
        TSpeedButton *SBtn_Output05;
        TSpeedButton *SBtn_Output06;
        TSpeedButton *SBtn_Output07;
        TSpeedButton *SBtn_Input00;
        TSpeedButton *SBtn_Input01;
        TSpeedButton *SBtn_Input02;
        TSpeedButton *SBtn_Input03;
        TSpeedButton *SBtn_Input04;
        TSpeedButton *SBtn_Input05;
        TSpeedButton *SBtn_Input06;
        TSpeedButton *SBtn_Input07;
        TPanel *Panel97;
        TPanel *Panel98;
        TPanel *Panel90;
        TPanel *Panel91;
        TPanel *Panel92;
        TPanel *Panel96;
        TPanel *Panel103;
        TPanel *Panel104;
        TPanel *Panel106;
        TPanel *Panel107;
        TPanel *Panel108;
        TPanel *Panel_EncX2;
        TPanel *Panel_EncY2;
        TPanel *Panel_PulseX2;
        TPanel *Panel_PulseY2;
        TTimer *tmLiveCam;
        TTabSheet *COOLING;
        TMemo *mmCooling;
        TButton *btChkErrCooling;
        TLabel *lbCompileC;
        TPanel *Panel189;
        TPanel *Panel190;
        TPanel *Panel191;
        TPanel *Panel192;
        TPanel *Panel193;
        TPanel *Panel194;
        TPanel *Panel197;
        TPanel *ALed_Busy1;
        TPanel *ALed_Busy2;
        TPanel *Panel205;
        TPanel *Panel206;
        TPanel *Panel207;
        TPanel *Panel208;
        TPanel *Panel209;
        TPanel *ALed_Z1;
        TPanel *ALed_Z2;
        TPanel *ALed_Near1;
        TPanel *ALed_Near2;
        TPanel *ALed_MLimit1;
        TPanel *ALed_MLimit2;
        TPanel *ALed_Alarm1;
        TPanel *ALed_Alarm2;
        TPanel *ALed_Inpo1;
        TPanel *ALed_Inpo2;
        TPanel *Panel252;
        TPanel *ALed_Emer;
        TPanel *Panel212;
        TPanel *ALed_PLimit1;
        TPanel *ALed_PLimit2;
        TPanel *Panel256;
        TPanel *ALed_Home1;
        TPanel *ALed_Home2;
        TPanel *Panel18;
        TPanel *Panel_Org1;
        TPanel *Panel_Org2;
        TPanel *Panel19;
        TPanel *Panel_ServoReady1;
        TPanel *Panel_ServoReady2;
        TPanel *ALed_SMLimit1;
        TPanel *ALed_SPLimit1;
        TPanel *ALed_SMLimit2;
        TPanel *ALed_SPLimit2;
        TPanel *Panel143;
        TSpeedButton *SBtn_MotionStop;
        TSpeedButton *SpeedButton1;
        TSpeedButton *SpeedButton5;
        TSpeedButton *SpeedButton9;
        TSpeedButton *SpeedButton2;
        TSpeedButton *SpeedButton6;
        TPanel *Panel152;
        TPanel *Panel153;
        TPanel *Panel154;
        TPanel *Panel155;
        TCheckBox *CB_X;
        TPanel *Panel159;
        TPanel *Panel164;
        TPanel *Panel_Pulse;
        TPanel *Panel170;
        TEdit *Edit_XDist;
        TPanel *Panel171;
        TEdit *Edit_YDist;
        TPanel *Panel174;
        TCheckBox *CheckBox4;
        TPanel *Panel179;
        TCheckBox *CheckBox3;
        TPanel *Panel249;
        TButton *Button3;
        TButton *Button7;
        TButton *HomeX;
        TButton *Button11;
        TButton *Button12;
        TButton *Homey;
        TButton *Button14;
        TButton *Button17;
        TButton *Button20;
        TButton *Button21;
        TPanel *Panel56;
        TCheckBox *CB_Y;
        TPanel *Panel94;
        TPanel *Panel95;
        TPanel *Panel99;
        TPanel *Panel100;
        TPanel *Panel101;
        TPanel *Panel102;
        TPanel *Panel105;
        TPanel *Panel117;
        TPanel *Panel_PulseY;
        TPanel *Panel_EncX;
        TPanel *Panel_EncY;
        TPanel *Panel_PulseX;
        TPanel *Panel44;
        TPanel *Panel45;
        TPanel *Panel46;
        TPanel *Panel47;
        TPanel *Panel48;
        TPanel *Panel49;
        TPanel *Panel50;
        TPanel *Panel51;
        TEdit *edAxis1_AccC;
        TEdit *edAxis1_VelC;
        TEdit *edAxis1_DistC;
        TPanel *Panel57;
        TEdit *edAxis1_TimeC;
        TBitBtn *btAxis1DistGoC;
    TTabSheet *TabSheet6;
    TStringGrid *sgTemp;
    TLabel *Label14;
    TEdit *edHeat1a;
    TEdit *edHeat1b;
    TLabel *Label15;
    TEdit *edHeat2a;
    TEdit *edHeat2b;
    TLabel *Label16;
    TLabel *Label17;
    TEdit *edHeat3a;
    TEdit *edHeat3b;
    TLabel *Label18;
    TLabel *Label20;
    TButton *btHeatCal;
    TTimer *Timer1;
    TBitBtn *btMotor1Sv;
        TBitBtn *BitBtn1;
        TPageControl *PageControl2;
        TTabSheet *TabSheet7;
        TTabSheet *TabSheet8;
        TPanel *Panel61;
        TPanel *Panel62;
        TPanel *Panel64;
        TPanel *Panel65;
        TPanel *edTemp2;
        TPanel *edTemp1;
        TPanel *Panel59;
        TPanel *edTemp3;
        TBitBtn *brSetHeat;
        TPanel *Panel52;
        TEdit *edInpHeat1;
        TEdit *edInpHeat2;
        TEdit *edInpHeat3;
        TPanel *Panel63;
        TEdit *edSetHeat1;
        TEdit *edSetHeat2;
        TEdit *edSetHeat3;
        TLabel *Label22;
        TLabel *Label23;
        TLabel *Label24;
        TLabel *Label25;
        TProgressBar *pbSeqTime;
        TStaticText *lbSeqTotalTime;
        TStaticText *lbSeqRemaing;
        TLabel *Label21;
        TLabel *Label26;
        TLabel *Label27;
        TLabel *Label28;
        TStaticText *lbSeqStartTime;
        TStaticText *lbSeqEndTime;
        TButton *Button2;
    void __fastcall FormCreate(TObject *Sender);
//    void __fastcall HomeX(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    //모션 관련.
    void __fastcall ReadLogic(void);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall Button14Click(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall Button17Click(TObject *Sender);
    void __fastcall Button11Click(TObject *Sender);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall SBtn_MotionStopClick(TObject *Sender);
    void __fastcall SpeedButton5MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall SpeedButton1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall SpeedButton5MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y);
//    void __fastcall HomeX(TObject *Sender);
    void __fastcall Button7Click(TObject *Sender);
    void __fastcall Button20Click(TObject *Sender);
    void __fastcall btAxis1RPMGoClick(TObject *Sender);
    void __fastcall btAxis1DistGoClick(TObject *Sender);
    void __fastcall Panel_EncXClick(TObject *Sender);
    void __fastcall SBtn_Output00Click(TObject *Sender);
    void __fastcall btRotorRunClick(TObject *Sender);
    void __fastcall btStopClick(TObject *Sender);
    void __fastcall btAddDeviceClick(TObject *Sender);
    void __fastcall sgDeviceSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall btDeleteClick(TObject *Sender);
    void __fastcall Bt_LoadClick(TObject *Sender);
    void __fastcall HomeXClick(TObject *Sender);
    void __fastcall bt_Lsave1Click(TObject *Sender);
    void __fastcall Button12Click(TObject *Sender);
//    void __fastcall LaserPosition1(TObject *Sender);
    void __fastcall btAxis2DistGoClick(TObject *Sender);
    void __fastcall mmMsgDblClick(TObject *Sender);
    void __fastcall btSaveClick(TObject *Sender);
        void __fastcall btSeqExTestClick(TObject *Sender);
        void __fastcall btSeqTestClick(TObject *Sender);
    void __fastcall btSeqRunClick(TObject *Sender);
    void __fastcall btLiveClick(TObject *Sender);
    void __fastcall btExitClick(TObject *Sender);
    void __fastcall btCoolingClick(TObject *Sender);
        void __fastcall brSetHeatClick(TObject *Sender);
        void __fastcall tmHeatTimer(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btAllHomeClick(TObject *Sender);
        void __fastcall tmLiveCamTimer(TObject *Sender);
        void __fastcall btChkErrCoolingClick(TObject *Sender);
        void __fastcall btMotor1SvClick(TObject *Sender);
    void __fastcall btHeatCalClick(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);


private:	// User declarations
//    typedef unsigned char U8 ;

    struct TStQuery {
        unsigned char  id;
        unsigned char  func;
        unsigned char  start_addr_hi;
        unsigned char  start_addr_lo;
        unsigned char  size_hi;
        unsigned char  size_lo;
        unsigned short CRC16;
    };
    struct TStResponse {
        unsigned char  id;
        unsigned char  func;
        unsigned char  start_addr_hi;
        unsigned char  start_addr_lo;
        unsigned char  size_hi;
        unsigned char  size_lo;
        unsigned short CRC16;
    };

    struct TData {
        unsigned char *hi;
        unsigned char *lo;
    };
    struct TStep {
        int iSeq     ; //Step.
        int iCycle   ; //싸이클동작의 스텝.
        int iHome    ; //홈 잡는 스텝.
        int iToStart ; //시작준비 스텝.
        int iToStop  ; //정지준비 스텝.
    } Step , PreStep ;


    TStQuery    m_StQuery   ;
    TStResponse m_StResponse  ;
    TData       m_Data      ;

    int         m_iData     ;


    bool        m_bComplite ;

    CDelayTimer m_CycleTimer;


    AnsiString GetLastJobName ( );
    void       LoadJobFile    (AnsiString _sName);
    void       SaveJobFile    (AnsiString _sName);

    void       LoadCooling    ();
    void       SaveCooling    ();

    int RowCount;
    bool m_bOpen;
    AnsiString	m_strModelName;
    AnsiString	m_strDeviceID;
    AnsiString  sMsg   [100] ;

//    AnsiString  sMsg   [100] ;
//    bool        m_bSend   ;
    bool        m_bRecive ;
    TWndMethod  OriginalProc;

    void __fastcall PanelProc(Messages::TMessage & msg);
    void __fastcall ContinuousShot();
    //모션 관련.
    bool __fastcall InputChanged  (NMCBIT   NmcBit      );
    void __fastcall OutputChanged (BYTE     Output      );
    int  __fastcall HexToInt      (String  &str         );
    bool __fastcall CheckHeat     (int _iId             );
    bool __fastcall SetHeatTimer  (int _iId, int _iData );
    BYTE OutputIO;
  	UINT m_nBit[8];
    int m_nDistMode[4];
    //
//---------------------------------------------------------------------------
public:		// User declarations

//Heater
    bool bSetHeat1 ;
    bool bSetHeat2 ;
    bool bSetHeat3 ;

    int  iDataHeat1;
    int  iDataHeat2;
    int  iDataHeat3;
//
    void Msg(AnsiString _sMsg)
    {
        if( mmMsg->Lines->Count > 1000) mmMsg -> Lines -> Delete(0) ;
        mmMsg -> Lines -> Add(FormatDateTime("(hh:nn:ss)",Now())+_sMsg) ;
    }

    AnsiString  m_sCrntJobName ;

    CDelayTimer m_tmCooling    ;
    bool m_bCoolingOn ;
    bool m_bCoolingOff;
    BYTE m_RxBuf[100];	// 통신 읽기 버퍼
    int  m_iMsgCnt;
//    int  m_iMsgCnt1;
//    int  m_iMsgCnt2;
//    int  m_iMsgCnt3;
    //bool BtCheck1;
    //bool BtCheck2;
    //bool BtCheck3;
    //bool BtCheck4;

    bool m_bAxis1Stop;
    AnsiString CurrntDevice;

    int m_nMotionMode[2];
    UINT OutputOld[8];

    bool m_bManualStart ;
    bool m_bStop;
    bool m_bLiveCam ;

    __fastcall TFrmMain(TComponent* Owner);

    void __fastcall GetSensorRead(TMessage msg) ;
    void __fastcall btSeqEnable (bool bEnable);
    void __fastcall btLiveEnable(bool bEnable);

    bool __fastcall SetHeat       (int _iId, int _iData );                    

    void __fastcall Heat1Ch(TMessage &Message);
    void __fastcall Heat2Ch(TMessage &Message);
    void __fastcall Heat3Ch(TMessage &Message);

    BEGIN_MESSAGE_MAP
        MESSAGE_HANDLER(UM_EVN_RXCHAR_PB1  , TMessage, Heat1Ch )
        MESSAGE_HANDLER(UM_EVN_RXCHAR_PB2  , TMessage, Heat2Ch )
        MESSAGE_HANDLER(UM_EVN_RXCHAR_PB3  , TMessage, Heat3Ch )
        MESSAGE_HANDLER(PAIX_SENSORREAD, TMessage, GetSensorRead)
    END_MESSAGE_MAP (TForm)

   //Heat Cal.
   double get_IP    (double* x, double *y, int size);
   double get_Dsc   (double* x, int     size);
   double get_Slope (double* x, double *y, int size, double ip, double D);
   double get_Interc(double *x, double *y, int size, double ip, double D);
   bool   LoadSaveTemp(bool _bLoad);
   double GetCalTemp(int _iHeaterNo , double _dx); 

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMain *FrmMain;
//---------------------------------------------------------------------------
#endif
