//---------------------------------------------------------------------------

#ifndef FormMainH
#define FormMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Menus.hpp>

#include <ComCtrls.hpp>
#include <Grids.hpp>
#include "TEdit.h"
#include "uRs232c.h"
#include <ImgList.hpp>
#include "WorkTable.h"

//#include "Axt_FS20.h"
//---------------------------------------------------------------------------
enum EN_VISN_MOTION_CMD {
    vmAllHome     = 1 ,
    vmReset       = 2 ,
    vmClose       = 3 ,
    vmRslt        = 4 ,
    vmShow        = 5 ,
    vmTestModeOn  = 6 ,
    vmTestModeOff = 7 ,
    vmTestLeft    = 8 ,
    vmTestRight   = 9 ,
    vmAPCUp       = 10,
    vmAPCDn       = 11
};

enum EN_MOTION_VISION_CMD {
    mvInsp    = 1 ,
    mvAuto    = 2 ,
    mvTest    = 3 ,
    mvMsg     = 4 ,
    mvLT      = 5 ,
    mvRT      = 6 ,
    mvReInsp  = 7 ,
    mv3Insp   = 8
};




/*
Vision -> Motion
(VC01)             //ALLHOME
(VC02)             //RESET
(VC03)             //CLOSE AP
(VC04)<좌표,좌표,....>  //검사결과.
(VC05)             //UI Show

Motion -> Vision
(MC01)             //검사.
*/







class TFrmMain : public TForm
{
__published:	// IDE-managed Components
        TPopupMenu *pmTray;
        TMenuItem *Exit1;
        TTimer *tmUpdate;
        TPanel *Panel1;
        TPanel *pnTitleBar;
        TImage *imTileBar;
        TPanel *pnBase;
        TSpeedButton *btMini;
        TLabel *lbDate;
        TLabel *lbStat;
        TPageControl *pcMain;
        TTabSheet *tsPosition;
        TPanel *Panel5;
        TPanel *pnLT_Z_Pos;
        TPanel *Panel7;
        TPanel *pnLT_Y_Pos;
        TPanel *Panel8;
        TPanel *pnLT_IN_Pos;
        TPanel *Panel9;
        TLabel *lbEnc1;
        TPanel *Panel10;
        TLabel *lbEnc2;
        TPanel *Panel11;
        TPanel *pnRT_Y_Pos;
        TPanel *Panel12;
        TPanel *pnRT_Z_Pos;
        TPanel *Panel13;
        TPanel *pnRT_IN_Pos;
        TPanel *Panel14;
        TLabel *lbEnc4;
        TPanel *Panel15;
        TLabel *lbEnc5;
        TPanel *Panel6;
        TLabel *lbEnc0;
        TPanel *Panel16;
        TLabel *lbEnc3;
        TBitBtn *btSavePos;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn3;
        TBitBtn *BitBtn6;
        TBitBtn *BitBtn7;
        TBitBtn *BitBtn8;
        TBitBtn *BitBtn9;
        TBitBtn *BitBtn10;
        TBitBtn *BitBtn11;
        TBitBtn *BitBtn12;
        TBitBtn *BitBtn13;
        TBitBtn *BitBtn14;
        TBitBtn *BitBtn15;
        TRadioGroup *rgJogUnit;
        TEdit *edJogUsr;
        TTabSheet *tsOption;
        TBitBtn *btSaveOptn;
        TGroupBox *GroupBox5;
        TLabel *Label52;
        TLabel *Label59;
        TLabel *Label60;
        TLabel *Label61;
        TLabel *Label62;
        TLabel *Label63;
        TLabel *Label64;
        TLabel *Label65;
        TLabel *Label66;
        TLabel *Label67;
        TLabel *Label68;
        TLabel *Label69;
        TLabel *Label70;
        TLabel *Label71;
        TLabel *Label72;
        TLabel *Label73;
        TLabel *Label74;
        TLabel *Label75;
        TLabel *Label76;
        TLabel *Label77;
        TLabel *Label78;
        TLabel *Label79;
        TStringGrid *sgLTDispTime;
        TStringGrid *sgRTDispTime;
        TPanel *Panel22;
        TLabel *Label23;
        TEdit *edSuckTime;
        TPanel *Panel21;
        TLabel *Label22;
        TEdit *edDispEndDelay;
        TGroupBox *GroupBox4;
        TPanel *Panel24;
        TLabel *Label46;
        TEdit *edMaxInspCnt;
        TGroupBox *GroupBox6;
        TPanel *Panel4;
        TLabel *Label80;
        TEdit *edUVTime;
        TPanel *Panel3;
        TLabel *Label35;
        TEdit *edSptrDnDelay;
        TTabSheet *tsActuator;
        TLabel *Label15;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label21;
        TLabel *Label24;
        TLabel *Label25;
        TLabel *Label26;
        TLabel *Label27;
        TLabel *Label28;
        TLabel *Label29;
        TLabel *Label30;
        TLabel *Label31;
        TLabel *lbATFw;
        TLabel *lbATBw;
        TLabel *lbATAlram;
        TLabel *Label81;
        TStringGrid *sgActuator;
        TBitBtn *btBwd;
        TBitBtn *btFwd;
        TEdit *edActrName;
        TEdit *edActrAddrIF;
        TEdit *edActrAddrOF;
        TEdit *edActrOnDelayF;
        TEdit *edActrTimeOutB;
        TEdit *edActrApplyTO;
        TEdit *edActrInv;
        TEdit *edActrComt;
        TEdit *edActrAddrIB;
        TEdit *edActrAddrOB;
        TEdit *edActrOnDelayB;
        TEdit *edActrTimeOutF;
        TEdit *edActrApplyOC;
        TBitBtn *btSaveActr;
        TBitBtn *btATReset;
        TBitBtn *btApplyActr;
        TEdit *edActrInitStat;
        TTabSheet *tsIo;
        TLabel *INDEX;
        TLabel *Label37;
        TLabel *Label38;
        TLabel *ADDRESS;
        TLabel *Label44;
        TLabel *Label45;
        TStringGrid *sgIO;
        TComboBox *cbIO;
        TEdit *edIOIndex;
        TEdit *edIOName;
        TEdit *edIOInv;
        TEdit *edIOAdd;
        TEdit *edIOComt;
        TEdit *edIODelay;
        TBitBtn *btSaveIO;
        TCheckBox *cbHex;
        TBitBtn *btApplyIO;
        TTabSheet *tsMotor;
        TComboBox *cbMotor;
        TPanel *pnMotionParamSet;
        TLabel *Label3;
        TLabel *labelRunSpeed;
        TLabel *labelACCTime;
        TLabel *Label4;
        TLabel *Home;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label5;
        TEdit *edACCTime;
        TEdit *edRunSpeed;
        TEdit *edManualSpeed;
        TEdit *edHomeSpeed;
        TEdit *edJogSpeed;
        TEdit *edInposition;
        TEdit *edHomeOffset;
        TPanel *pnRepeatTest;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TLabel *Label11;
        TEdit *edFirstPos;
        TEdit *edSecondPos;
        TEdit *edStopDelay;
        TButton *btTest;
        TButton *btStop;
        TPanel *pnMotorStatus;
        TLabel *Label12;
        TLabel *lbServoOn;
        TLabel *lbHomeSnsr;
        TLabel *lbPEndLimSnsr;
        TLabel *lbNEndLimSnsr;
        TLabel *lbPackInPosn;
        TLabel *lbStop;
        TLabel *lbBusy;
        TLabel *lbHomeEnded;
        TLabel *lbAlarm;
        TLabel *lbReady;
        TLabel *Label16;
        TLabel *lbCmdPos;
        TLabel *lbJogN;
        TLabel *lbJogP;
        TLabel *lbOk;
        TLabel *Label34;
        TLabel *lbEncPos;
        TLabel *Label36;
        TLabel *lbTrgPos;
        TPanel *pnJogTest;
        TLabel *Label13;
        TButton *btJogN;
        TButton *btJogP;
        TPanel *pnWorks;
        TLabel *Label14;
        TButton *btMtHome;
        TButton *btMtReset;
        TButton *btMtStop;
        TButton *btMtSvOn;
        TButton *btMtSvOff;
        TButton *btMtClrPos;
        TButton *Button4;
        TBitBtn *btSaveMotor;
        TTabSheet *tsTest;
        TGroupBox *GroupBox1;
        TLabel *Data;
        TLabel *Label33;
        TLabel *Label82;
        TMemo *Memo1;
        TButton *SEND;
        TEdit *edData;
        TEdit *edCmd;
        TEdit *edVal;
        TButton *Button5;
        TButton *Button6;
        TEdit *edRowTest;
        TButton *Button7;
        TGroupBox *GroupBox2;
        TLabel *lbVisnReq;
        TButton *btVisn;
        TGroupBox *GroupBox3;
        TLabel *lbErrName;
        TLabel *lbErr;
        TButton *btErrReset;
        TButton *Button8;
        TButton *Button9;
        TTabSheet *tsControl;
        TRadioGroup *rgJogUnit2;
        TBitBtn *btLT_Dn;
        TBitBtn *btLT_Up;
        TBitBtn *btLT_Bwd;
        TBitBtn *btLT_Fwd;
        TBitBtn *btLT_I_Bwd;
        TBitBtn *btLT_I_Fwd;
        TBitBtn *btRT_Dn;
        TBitBtn *btRT_Up;
        TBitBtn *btRT_Bwd;
        TBitBtn *btRT_Fwd;
        TBitBtn *btRT_I_Bwd;
        TBitBtn *btRT_I_Fwd;
        TEdit *Edit1;
        TButton *Button10;
        TButton *Button11;
        TBitBtn *BitBtn28;
        TBitBtn *BitBtn29;
        TBitBtn *btRTVac6;
        TPanel *Panel36;
        TLabel *Label123;
        TEdit *edInspDelay;
        TLabel *Label124;
        TGroupBox *GroupBox8;
        TLabel *Label125;
        TLabel *lbLTStepSeq;
        TLabel *Label127;
        TLabel *lbLTStepCycle;
        TLabel *Label129;
        TLabel *lbLTStepStop;
        TLabel *Label131;
        TLabel *lbLTStepStart;
        TLabel *Label133;
        TLabel *lbRTStepSeq;
        TLabel *Label135;
        TLabel *lbRTStepCycle;
        TLabel *Label137;
        TLabel *lbRTStepStop;
        TLabel *Label139;
        TLabel *lbRTStepStart;
        TLabel *Label126;
        TLabel *lbLTStepHome;
        TLabel *Label130;
        TLabel *lbRTStepHome;
        TButton *btAPCOff;
        TLabel *Label136;
        TSpeedButton *btLevel;
        TPanel *pnLevel;
        TBitBtn *btOper;
        TBitBtn *btEng;
        TBitBtn *btHanra;
        TLabel *lbVer;
        TGroupBox *GroupBox9;
        TLabel *Label97;
        TLabel *Label114;
        TLabel *Label98;
        TLabel *Label99;
        TLabel *Label100;
        TLabel *Label101;
        TLabel *Label102;
        TLabel *Label103;
        TLabel *Label104;
        TLabel *Label105;
        TLabel *Label106;
        TLabel *Label107;
        TLabel *Label108;
        TLabel *Label120;
        TLabel *Label122;
        TLabel *Label141;
        TLabel *Label142;
        TBitBtn *btRtDp1Out;
        TBitBtn *btRtDp1In;
        TBitBtn *btRtDp2Up;
        TBitBtn *btRtDp2Dn;
        TBitBtn *btRTSol1;
        TBitBtn *btRTSol2;
        TBitBtn *btRTSol3;
        TBitBtn *btRTSol4;
        TBitBtn *btRTSol5;
        TBitBtn *btRTSol6;
        TBitBtn *btRtDp1Up;
        TBitBtn *btRtDp1Dn;
        TBitBtn *BitBtn62;
        TBitBtn *BitBtn63;
        TBitBtn *btRtDp2Out;
        TBitBtn *btRtDp2In;
        TBitBtn *BitBtn66;
        TBitBtn *BitBtn67;
        TBitBtn *BitBtn68;
        TBitBtn *BitBtn69;
        TBitBtn *BitBtn70;
        TBitBtn *BitBtn71;
        TBitBtn *BitBtn72;
        TBitBtn *BitBtn73;
        TBitBtn *BitBtn74;
        TBitBtn *BitBtn75;
        TBitBtn *BitBtn76;
        TBitBtn *BitBtn77;
        TBitBtn *BitBtn78;
        TBitBtn *BitBtn79;
        TBitBtn *BitBtn80;
        TBitBtn *BitBtn81;
        TBitBtn *btRTUVStart;
        TBitBtn *BitBtn85;
        TBitBtn *BitBtn86;
        TButton *btrRT_UVCycle;
        TButton *btRT_DispCycle;
        TButton *btRT_InspCycle;
        TButton *btRT_Disp;
        TButton *btRT_APCOn;
        TBitBtn *btRT_SampleBind;
        TBitBtn *btRT_SampleRel;
        TBitBtn *btRTVacStart;
        TLabel *Label96;
        TButton *btAPCp;
        TButton *btAPCm;
        TGroupBox *GroupBox10;
        TLabel *Label83;
        TLabel *Label84;
        TLabel *Label85;
        TLabel *Label86;
        TLabel *Label87;
        TLabel *Label88;
        TLabel *Label89;
        TLabel *Label90;
        TLabel *Label91;
        TLabel *Label92;
        TLabel *Label93;
        TLabel *Label94;
        TLabel *Label95;
        TLabel *Label111;
        TLabel *Label117;
        TLabel *Label121;
        TLabel *Label140;
        TLabel *Label138;
        TBitBtn *btLtDp1Up;
        TBitBtn *btLtDp1Out;
        TBitBtn *btLtDp1Dn;
        TBitBtn *btLtDp1In;
        TBitBtn *btLtDp2Up;
        TBitBtn *btLtDp2Dn;
        TBitBtn *BitBtn36;
        TBitBtn *BitBtn37;
        TBitBtn *btLtDp2Out;
        TBitBtn *btLtDp2In;
        TBitBtn *BitBtn40;
        TBitBtn *BitBtn41;
        TBitBtn *BitBtn42;
        TBitBtn *BitBtn43;
        TBitBtn *BitBtn44;
        TBitBtn *BitBtn45;
        TBitBtn *BitBtn46;
        TBitBtn *BitBtn47;
        TBitBtn *BitBtn48;
        TBitBtn *BitBtn49;
        TBitBtn *BitBtn50;
        TBitBtn *BitBtn51;
        TBitBtn *BitBtn52;
        TBitBtn *BitBtn53;
        TBitBtn *BitBtn54;
        TBitBtn *BitBtn55;
        TBitBtn *btLTSol1;
        TBitBtn *btLTSol2;
        TBitBtn *btLTSol4;
        TBitBtn *btLTSol5;
        TBitBtn *btLTSol6;
        TBitBtn *btLTUVStart;
        TBitBtn *BitBtn83;
        TBitBtn *BitBtn84;
        TBitBtn *btLTSol3;
        TButton *btLT_UVCycle;
        TButton *btLT_DispCycle;
        TButton *btLT_InspCycle;
        TButton *btLT_Disp;
        TButton *btLT_APCOn;
        TBitBtn *btLT_SampleBind;
        TBitBtn *btLT_SampleRel;
        TBitBtn *btLTVacStart;
        TButton *Button3;
        TButton *Button12;
        TButton *Button13;
        TButton *Button14;
        TButton *Button15;
        TButton *Button16;
        TButton *Button17;
        TButton *Button18;
        TButton *Button19;
        TButton *Button20;
        TButton *Button21;
        TButton *Button22;
        TLabel *Label134;
        TEdit *edLTSolTime;
        TLabel *Label132;
        TLabel *Label2;
        TEdit *edRTSolTime;
        TLabel *Label109;
        TLabel *Label32;
        TLabel *Label110;
        TLabel *Label143;
        TLabel *lbLTWorkTime;
        TLabel *Label145;
        TLabel *lbRTWorkTime;
        TLabel *Label144;
        TBitBtn *BitBtn2;
        TBitBtn *BitBtn4;
        TLabel *Label146;
        TBitBtn *BitBtn5;
        TBitBtn *BitBtn16;
        TEdit *edBar1;
        TLabel *Label1;
        TEdit *edBar2;
        TLabel *Label50;
        TCheckBox *cbGrabMode;
        TCheckBox *cbInfinityInsp;
        TCheckBox *cbInspBefrDisp;
        TCheckBox *cbInspAftrDisp;
        TGroupBox *GroupBox12;
        TCheckBox *cbUseShiftCal;
        TPanel *Panel23;
        TLabel *Label43;
        TEdit *edMaxIPos;
        TPanel *Panel18;
        TLabel *Label40;
        TEdit *edSfRes;
        TPanel *Panel30;
        TLabel *Label54;
        TEdit *edSfVibCnt;
        TPanel *Panel35;
        TLabel *Label118;
        TLabel *Label119;
        TEdit *edSfVibRng;
        TGroupBox *GroupBox11;
        TCheckBox *cbUseAPCCal;
        TPanel *Panel25;
        TLabel *Label47;
        TEdit *edApcRes;
        TPanel *Panel28;
        TEdit *edAPCCalLim;
        TPanel *Panel27;
        TLabel *Label49;
        TEdit *edAPCSetVal;
        TPanel *Panel2;
        TEdit *edAPCUnit;
        TPanel *Panel26;
        TLabel *Label58;
        TEdit *edAPCOnDelay;
        TPanel *Panel38;
        TEdit *edApcLim;
        TCheckBox *cbROMType;
        TGroupBox *GroupBox13;
        TCheckBox *cbUseAngleCal;
        TPanel *Panel20;
        TLabel *Label42;
        TEdit *edMaxYPos;
        TPanel *Panel19;
        TLabel *Label41;
        TLabel *Label48;
        TEdit *edMaxZPos;
        TPanel *Panel17;
        TLabel *Label39;
        TEdit *edAgRes;
        TPanel *Panel29;
        TLabel *Label53;
        TEdit *edAngZVibCnt;
        TPanel *Panel34;
        TLabel *Label115;
        TLabel *Label116;
        TEdit *edAngVibRng;
        TPanel *Panel37;
        TLabel *Label128;
        TEdit *edAngYVibCnt;
        TPanel *Panel39;
        TLabel *Label51;
        TLabel *Label147;
        TEdit *edFindAngle;
        TGroupBox *GroupBox14;
        TCheckBox *cbUseBeamRslt;
        TPanel *Panel31;
        TLabel *Label55;
        TEdit *edGTDnVibCnt;
        TPanel *Panel32;
        TLabel *Label56;
        TEdit *edGTDnVibRng;
        TPanel *Panel33;
        TLabel *Label57;
        TEdit *edGTUpOffset;
        TCheckBox *cbBarcodeScan;
        TComboBox *cbModeSelect;
        TPanel *Panel40;
        TLabel *Label148;
        TCheckBox *cbAskInsp;
        TCheckBox *cbDryRun;
        TCheckBox *cbInspDrngDisp;
        TCheckBox *cbAdjGT2ndInsp;
        TCheckBox *cbDataCheck;
        TLabel *Label112;
        TBitBtn *btRTVac1;
        TBitBtn *btRTVac2;
        TBitBtn *btRTVac3;
        TBitBtn *btRTVac4;
        TBitBtn *btRTVac5;
        TBitBtn *btLTVac1;
        TBitBtn *btLTVac2;
        TBitBtn *btLTVac3;
        TBitBtn *btLTVac4;
        TBitBtn *btLTVac5;
        TBitBtn *btLTVac6;
        TLabel *Label113;
        TButton *Button1;
        TButton *Button23;
        TEdit *edErr;
        TCheckBox *cbDispFirst;
        TCheckBox *cbUseAvrPos;
        TLabel *Label149;
        TEdit *edAvrPosCnt;
        TCheckBox *cbUseHomming;
        TEdit *edActrAddrIF_;
        TEdit *edActrAddrOF_;
        TEdit *edActrAddrIB_;
        TEdit *edActrAddrOB_;
        TCheckBox *cbIgnrDisp;
        TEdit *edLtCcDpOlTtDispCnt;
        TButton *btErrList;
        TEdit *edRtCcDpOlTtDispCnt;
        TBitBtn *BitBtn17;
        TCheckBox *cbAPCOnReadjust;
    TTimer *tmMode;
    TPanel *Panel41;
    TLabel *Label150;
    TEdit *edSptrEndUpDelay;
    TPanel *Panel42;
    TEdit *edGT3rdInspCnt;
    TLabel *Label151;
    TCheckBox *cbInspAfterDpDone;
        void __fastcall imTileBarMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall TrayIcon1Click(TObject *Sender);
        void __fastcall Exit1Click(TObject *Sender);
        void __fastcall btMiniClick(TObject *Sender);
        void __fastcall cbMotorChange(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall btSaveMotorClick(TObject *Sender);
        void __fastcall btSaveOptnClick(TObject *Sender);
        void __fastcall btSavePosClick(TObject *Sender);
        void __fastcall cbIOChange(TObject *Sender);
        void __fastcall sgIODrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
        void __fastcall sgIODblClick(TObject *Sender);
        void __fastcall btBwdClick(TObject *Sender);
        void __fastcall btFwdClick(TObject *Sender);
        void __fastcall Label1Click(TObject *Sender);
        void __fastcall btApplyActrClick(TObject *Sender);
        void __fastcall btSaveActrClick(TObject *Sender);
        void __fastcall sgActuatorClick(TObject *Sender);
        void __fastcall sgActuatorDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall btATResetClick(TObject *Sender);
        void __fastcall btJogNMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btJogNMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall btJogPMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btJogPMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall btMtSvOnClick(TObject *Sender);
        void __fastcall btMtSvOffClick(TObject *Sender);
        void __fastcall btMtStopClick(TObject *Sender);
        void __fastcall btStopClick(TObject *Sender);
        void __fastcall btMtResetClick(TObject *Sender);
        void __fastcall btTestClick(TObject *Sender);
        void __fastcall lbHomeEndedDblClick(TObject *Sender);
        void __fastcall btMtHomeClick(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall SENDClick(TObject *Sender);
        void __fastcall btVisnClick(TObject *Sender);
        void __fastcall sgIOClick(TObject *Sender);
        void __fastcall btSaveIOClick(TObject *Sender);
        void __fastcall btApplyIOClick(TObject *Sender);
        void __fastcall btErrResetClick(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall Button9Click(TObject *Sender);
        void __fastcall Button8Click(TObject *Sender);
        void __fastcall BitBtn1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall BitBtn1MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btLT_DnMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btLT_DnMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Button10Click(TObject *Sender);
        void __fastcall Button11Click(TObject *Sender);
        void __fastcall BitBtn28Click(TObject *Sender);
        void __fastcall BitBtn29Click(TObject *Sender);
        void __fastcall btLTVacClick(TObject *Sender);
        void __fastcall btLTSolClick(TObject *Sender);
        void __fastcall btRTVacClick(TObject *Sender);
        void __fastcall btRTSolClick(TObject *Sender);
        void __fastcall btLTUVStartMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btLTUVStartMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ActrBtnBwdClick(TObject *Sender);
        void __fastcall ActrBtnFwdClick(TObject *Sender);
        void __fastcall btLT_UVCycleClick(TObject *Sender);
        void __fastcall btRT_SampleBindClick(TObject *Sender);
        void __fastcall btMtClrPosClick(TObject *Sender);
        void __fastcall btLevelClick(TObject *Sender);
        void __fastcall btOperClick(TObject *Sender);
        void __fastcall btVacStartClick(TObject *Sender);
        void __fastcall Button13Click(TObject *Sender);
        void __fastcall Button17Click(TObject *Sender);
        void __fastcall lbVerClick(TObject *Sender);
        void __fastcall cbROMTypeClick(TObject *Sender);
        void __fastcall edMaxInspCntExit(TObject *Sender);
        void __fastcall sgRTDispTimeKeyPress(TObject *Sender, char &Key);
        void __fastcall pcMainChange(TObject *Sender);
        void __fastcall FormDeactivate(TObject *Sender);
        void __fastcall btOperChangeClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall edBar1Change(TObject *Sender);
        void __fastcall edBar2Change(TObject *Sender);
        void __fastcall cbModeSelectChange(TObject *Sender);
        void __fastcall btErrListClick(TObject *Sender);
private:	// User declarations

   int m_iAddCount;    // ListView 에 Log를 추가한 횟수를 저장
   int m_iLastLogAddTick;  // 마지막으로 Log를 추가한 Tick를 저장

   TRS232C * RS232C_APC  ;
   TRS232C * RS232C_BAR1 ;
   TRS232C * RS232C_BAR2 ;

   CDelayTimer Bar1Delay ;
   CDelayTimer Bar2Delay ;


   void UpdateDevOptn (bool _bToTable );
//   void __fastcall UMEvnRxChar (TMessage &Message); //Rs232 Receive
   void __fastcall UMEvnRxCharAPC (TMessage &Message); //Rs232 Receive
   void __fastcall UMEvnRxCharBAR1(TMessage &Message); //Rs232 Receive
   void __fastcall UMEvnRxCharBAR2(TMessage &Message); //Rs232 Receive

   void __fastcall OnCopyDataEvent(TMessage &Msg); //Vision Receive
   void __fastcall TrayEvent      (TMessage &Msg);

   BEGIN_MESSAGE_MAP
      MESSAGE_HANDLER(WM_USER+1, TMessage, TrayEvent)
      MESSAGE_HANDLER(WM_COPYDATA, TMessage, OnCopyDataEvent)
//      MESSAGE_HANDLER(UM_EVN_RXCHAR, TMessage, UMEvnRxChar)
      MESSAGE_HANDLER(UM_EVN_RXCHAR_APC  , TMessage, UMEvnRxCharAPC )
      MESSAGE_HANDLER(UM_EVN_RXCHAR_BAR1 , TMessage, UMEvnRxCharBAR1)
      MESSAGE_HANDLER(UM_EVN_RXCHAR_BAR2 , TMessage, UMEvnRxCharBAR2)

   END_MESSAGE_MAP(TForm)

   void  OnAddTrayIcon  (void);
   void  OnDelTrayIcon  (void);

   


public:		// User declarations
        __fastcall TFrmMain(TComponent* Owner);

    AnsiString OnGetVer(const AnsiString &sQuery);

    void __fastcall CreateParams(TCreateParams &Params);

    int m_iCrntLevel ;

    void DelayedOff();

    void __stdcall  OnSendVisnMsg(EN_MOTION_VISION_CMD cmd,  char * data = NULL);
    void __fastcall OnSendAPCMsg (AnsiString _sCmd , AnsiString _sData , AnsiString _sVal="" , bool _bInc = false );

    void __fastcall Save();
    void __fastcall Load();

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMain *FrmMain;
//---------------------------------------------------------------------------
#endif
