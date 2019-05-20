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
//---------------------------------------------------------------------------
//葛记 包访 Include
#include "NMC410S.h"
#include "MyUtil.h"
#include "PaixMotion.h"
#include "global.h"
#include "Timer.h"
//typedef void (__closure *MsgFunc)(AnsiString);
//---------------------------------------------------------------------------
class TFrmMain : public TForm
{
__published:	// IDE-managed Components
    TPanel *pnView;
    TPanel *Panel2;
    TImage *Image1;
    TLabel *Label1;
    TPanel *Panel3;
    TImage *Image2;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TBitBtn *BitBtn3;
    TPanel *Panel5;
    TImage *Image3;
    TPanel *Panel6;
    TPanel *Panel7;
    TImage *Image4;
    TOpenPictureDialog *OpenPictureDialog1;
    TSavePictureDialog *SavePictureDialog1;
    TPageControl *PageControl1;
    TTabSheet *TabSheet2;
        TTimer *tmUpdate;
    TPanel *Panel11;
    TButton *btAxis1RPMGo;
    TButton *btAxis1DistGo;
    TButton *btAxis2DistGo;
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
    TTabSheet *TabSheet3;
    TLabel *Label12;
    TImage *Image7;
        TBitBtn *btStop;
    TPanel *Panel8;
    TButton *btAddDevice;
    TButton *Bt_Load;
    TEdit *Ed_CurDevice;
        TEdit *Ed_SelDevice;
    TLabel *Label2;
    TLabel *Label3;
        TBitBtn *btAllHome;
    TEdit *Ed_Device;
    TStringGrid *sgDevice;
    TButton *btDelete;
    TMemo *mmMsg;
        TBitBtn *btSeqRun;
    TLabel *Label6;
    TTimer *tmLaser;
    TLabel *Label7;
        TTabSheet *TabSheet4;
        TMemo *mmSeq;
        TTabSheet *TabSheet5;
        TMemo *mmSeqEx;
        TButton *btSeqExTest;
        TButton *btSeqTest;
        TPanel *Panel43;
        TPanel *Panel44;
        TPanel *Panel45;
        TPanel *Panel47;
        TPanel *Panel48;
        TPanel *Panel49;
        TEdit *edAxis1_1Pos;
        TEdit *edAxis2_1Pos;
        TPanel *Panel9;
        TEdit *edLaserTime;
        TButton *btLaserPos1;
        TButton *btLaserOn;
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
        TPanel *Panel_PulseY2;
        TPanel *Panel_EncX2;
        TPanel *Panel_EncY2;
        TPanel *Panel_PulseX2;
        TLabel *lbCompileEx;
        TLabel *lbCompile;
        TStringGrid *StringGrid1;
        TPanel *Panel23;
        TImage *Image5;
    TTabSheet *TabSheet1;
    TPanel *Panel1;
    TPanel *Panel184;
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
    TPanel *Panel142;
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
    TPanel *Panel156;
    TCheckBox *CB_Y;
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
    TPanel *Panel93;
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
    TPanel *Panel75;
    TPanel *Panel76;
    TSpeedButton *SpeedButton3;
    TSpeedButton *SpeedButton4;
    TSpeedButton *SpeedButton7;
    TSpeedButton *SpeedButton8;
    TSpeedButton *SpeedButton10;
    TSpeedButton *SpeedButton11;
    TPanel *Panel77;
    TPanel *Panel78;
    TPanel *Panel79;
    TPanel *Panel80;
    TCheckBox *CheckBox1;
    TPanel *Panel81;
    TCheckBox *CheckBox2;
    TPanel *Panel82;
    TPanel *Panel83;
    TPanel *Panel84;
    TPanel *Panel85;
    TEdit *Edit42;
    TPanel *Panel86;
    TEdit *Edit43;
    TPanel *Panel87;
    TCheckBox *CheckBox5;
    TPanel *Panel88;
    TCheckBox *CheckBox6;
    TPanel *Panel89;
    TButton *Button33;
    TButton *Button34;
    TButton *Button35;
    TButton *Button36;
    TButton *Button37;
    TButton *Button38;
    TButton *Button39;
    TButton *Button40;
    TButton *Button41;
    TButton *Button42;
    TPanel *Panel4;
    TPanel *Panel17;
    TButton *Button4;
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
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *lbStat;
    TButton *btSave;
    TBitBtn *BitBtn4;
    TLabel *Label4;
    TLabel *Label5;
    TPanel *Panel26;
    TBitBtn *btRotorRun;
    TButton *Button1;
    TEdit *Edit1;
    void __fastcall FormCreate(TObject *Sender);
//    void __fastcall HomeX(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    //葛记 包访.
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
    void __fastcall btAllHomeClick(TObject *Sender);
    void __fastcall edAxis1_AccTimeChange(TObject *Sender);
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
    void __fastcall btLaserPosClick(TObject *Sender);
    void __fastcall btLaserOnClick(TObject *Sender);
    void __fastcall tmLaserTimer(TObject *Sender);
        void __fastcall btSeqExTestClick(TObject *Sender);
        void __fastcall btSeqTestClick(TObject *Sender);
    void __fastcall btSeqRunClick(TObject *Sender);
    void __fastcall BitBtn4Click(TObject *Sender);



private:	// User declarations
    AnsiString GetLastJobName ( );
    void       LoadJobFile    (AnsiString _sName);
    void       SaveJobFile    (AnsiString _sName);

    int RowCount;
	bool m_bOpen;
  	AnsiString	m_strModelName;
	AnsiString	m_strDeviceID;
    TWndMethod  OriginalProc;

    void __fastcall PanelProc(Messages::TMessage & msg);
    void __fastcall ContinuousShot();
    //葛记 包访.
    bool __fastcall InputChanged(NMCBIT NmcBit);
    void __fastcall OutputChanged(BYTE Output);
    BYTE OutputIO;
  	UINT m_nBit[8];
    int m_nDistMode[4];
    //
//---------------------------------------------------------------------------
public:		// User declarations

    void Msg(AnsiString _sMsg)
    {
        if( mmMsg->Lines->Count > 1000) mmMsg -> Lines -> Delete(0) ;
        mmMsg -> Lines -> Add(FormatDateTime("(hh:nn:ss)",Now())+_sMsg) ;
    }

    AnsiString m_sCrntJobName ;


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

    __fastcall TFrmMain(TComponent* Owner);

    void __fastcall GetSensorRead(TMessage msg) ;
    void __fastcall btEnable(bool bEnable);
    BEGIN_MESSAGE_MAP
        MESSAGE_HANDLER (PAIX_SENSORREAD, TMessage, GetSensorRead)
    END_MESSAGE_MAP (TForm)

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMain *FrmMain;
//---------------------------------------------------------------------------
#endif
