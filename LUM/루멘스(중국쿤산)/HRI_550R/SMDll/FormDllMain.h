//---------------------------------------------------------------------------

#ifndef FormDllMainH
#define FormDllMainH
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
#include <ImgList.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include "SMDllDefine.h"

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
    mvInsp = 1 ,
    mvAuto = 2 ,
    mvTest = 3 ,
    mvMsg  = 4 ,
    mvLT   = 5 ,
    mvRT   = 6 ,
};

//For Rect Display.
enum EN_CLICK_POSITION{
    cpLT   ,
    cpLB   ,
    cpRT   ,
    cpRB   ,
    cpT    ,
    cpB    ,
    cpR    ,
    cpL    ,
    cpMove ,









//    RESIZ_LTOP,
//    RESIZ_RBTM,
//    RESIZ_RTOP,
//    RESIZ_LBTM,
//    MOVE_SHAPE
};



/*
Vision -> Motion
(VC01)             //ALLHOME
(VC02)             //RESET
(VC03)             //CLOSE AP
(VC04)<ÁÂÇ¥,ÁÂÇ¥,....>  //°Ë»ç°á°ú.
(VC05)             //UI Show

Motion -> Vision
(MC01)             //°Ë»ç.
*/







class TFrmDllMain : public TForm
{
__published:	// IDE-managed Components
        TTimer *tmUpdate;
        TPanel *Panel1;
        TPageControl *pcMain;
        TTabSheet *tsActuator;
        TTabSheet *tsIo;
    TTabSheet *AAAAA;
        TTabSheet *TabSheet1;
        TTabSheet *TabSheet2;
        TOpenPictureDialog *dgOpenPictur;
        TTabSheet *TabSheet3;
    TPanel *Panel3;
    TStringGrid *sgErrList;
    TPanel *Panel18;
    TPanel *Panel6;
    TBitBtn *btApplyErr;
    TBitBtn *btSaveErr;
    TPanel *Panel42;
    TPanel *Panel43;
    TImage *Image22;
    TLabel *Label3;
    TEdit *edMotorCount;
    TEdit *deActuatorCount;
    TEdit *edIoOutputCount;
    TEdit *edIoInputCount;
    TBitBtn *btAllCountSave;
    TEdit *edErrorCount;
    TLabel *Label42;
    TLabel *Label41;
    TLabel *Label40;
    TLabel *Label39;
    TLabel *Label35;
    TLabel *Label2;
    TImage *Image2;
    TLabel *Label43;
    TPanel *Panel22;
    TPanel *Panel23;
    TPanel *Panel47;
    TImage *Image7;
    TLabel *Label57;
    TStringGrid *sgIO;
    TComboBox *cbIO;
    TPanel *Panel48;
    TLabel *Label38;
    TPanel *Panel50;
    TImage *Image24;
    TLabel *Label13;
    TEdit *edIOAdd;
    TEdit *edIOName;
    TEdit *edIOInv;
    TEdit *edIOComt;
    TCheckBox *cbHex;
    TPanel *Panel8;
    TPanel *Panel21;
    TImage *Image5;
    TLabel *lbGrn;
    TLabel *lbYel;
    TLabel *lbRed;
    TPanel *Panel32;
    TImage *Image13;
    TLabel *Label56;
    TPanel *pnBuzz1;
    TPanel *pnBuzz2;
    TPanel *pnBuzz3;
    TPanel *pnBuzzMute;
    TCheckBox *cbTestMode;
    TPanel *Panel9;
    TPanel *Panel56;
    TImage *Image4;
    TLabel *Label50;
    TStringGrid *sgTower;
    TPanel *Panel24;
    TPanel *Panel27;
    TPanel *Panel45;
    TImage *Image9;
    TLabel *Label58;
    TStringGrid *sgActuator;
    TPanel *Panel46;
    TPanel *Panel49;
    TImage *Image23;
    TLabel *Label11;
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
    TEdit *edActrInitStat;
    TPanel *Panel57;
    TBitBtn *btApplyActr;
    TBitBtn *btSaveActr;
    TPanel *Panel58;
    TImage *Image29;
    TLabel *Label65;
    TEdit *edIOIndex;
    TPanel *Panel2;
        TPanel *pnMtStat;
    TLabel *Label63;
    TLabel *Label64;
    TLabel *Label66;
    TLabel *Label67;
    TLabel *Label68;
    TEdit *edPulsePerRev;
    TEdit *edUnitPerRev;
    TEdit *edMaxPos;
    TEdit *edMinPos;
    TCheckBox *cbExistEnc;
    TCheckBox *cbHomeWithNSnsr;
    TCheckBox *cbSrvOnLevelInv;
    TComboBox *cbMotionType;
    TPanel *Panel36;
    TImage *Image19;
    TLabel *Label61;
    TPanel *Panel31;
    TPanel *Panel33;
    TImage *Image16;
    TLabel *Label15;
        TPanel *pnAxtPara;
    TPanel *Panel35;
    TImage *Image18;
    TLabel *Label69;
    TPanel *Panel34;
    TLabel *Label136;
    TPanel *Panel37;
    TImage *Image17;
    TLabel *Label59;
    TEdit *edACCTime;
    TEdit *edRunSpeed;
    TEdit *edManualSpeed;
    TEdit *edHomeSpeed;
    TEdit *edJogSpeed;
    TEdit *edInposition;
    TEdit *edHomeOffset;
    TGroupBox *GroupBox1;
    TLabel *lbTrgPos;
    TLabel *Label36;
    TLabel *Label34;
    TLabel *lbEncPos;
    TLabel *Label16;
    TLabel *lbCmdPos;
    TPanel *pnMotorStatus;
    TLabel *lbServoOn;
    TLabel *lbHomeSnsr;
    TLabel *lbPEndLimSnsr;
    TLabel *lbNEndLimSnsr;
    TLabel *lbPackInPosn;
    TLabel *lbStop;
    TLabel *lbHomeEnded;
    TLabel *lbAlarm;
    TLabel *lbReady;
    TLabel *lbJogN;
    TLabel *lbJogP;
    TLabel *lbOk;
    TPanel *Panel28;
    TPanel *Panel38;
    TImage *Image20;
    TLabel *Label1;
    TEdit *edFirstPos;
    TEdit *edSecondPos;
    TEdit *edStopDelay;
    TBitBtn *btTest;
    TBitBtn *btStop;
    TPanel *Panel39;
    TImage *Image21;
    TLabel *Label12;
    TGroupBox *GroupBox2;
    TBitBtn *btMtSvOff;
    TBitBtn *btMtSvOn;
    TBitBtn *btMtHome;
    TBitBtn *btMtClrPos;
    TBitBtn *btMtStop;
    TBitBtn *btMtReset;
    TBitBtn *btJogN;
    TBitBtn *btJogP;
    TBitBtn *btAllHome;
    TPanel *pnSubPara;
    TGroupBox *CONTROL;
    TBitBtn *btActrRpt;
    TBitBtn *btATReset;
    TGroupBox *STATUS;
    TLabel *lbATBw;
    TLabel *lbATFw;
    TLabel *lbATAlarm;
    TPanel *Panel4;
    TPanel *Panel7;
    TImage *Image3;
    TLabel *Label49;
    TEdit *edErrNo;
    TEdit *edErrName;
    TPanel *Panel5;
    TPanel *Panel25;
    TImage *Image1;
    TLabel *Label48;
    TMemo *mmErrAction;
    TPanel *Panel26;
    TPanel *Panel29;
    TImage *Image6;
    TLabel *Label47;
    TEdit *edImgPath;
    TBitBtn *BitBtn8;
    TPanel *Panel30;
    TPanel *Panel41;
    TLabel *Label46;
    TImage *Image8;
    TLabel *Label14;
    TPanel *pnErrImg;
    TImage *imErrPicture;
    TShape *spErrRect;
    TBitBtn *btApplyIO;
    TBitBtn *btSaveIO;
    TEdit *edIOHexAdd;
    TCheckBox *cbIOInverse;
    TStaticText *StaticText1;
    TStaticText *StaticText2;
    TStaticText *StaticText3;
    TStaticText *StaticText4;
    TStaticText *StaticText5;
    TStaticText *StaticText6;
    TStaticText *StaticText7;
    TStaticText *StaticText8;
    TStaticText *StaticText9;
    TStaticText *StaticText10;
    TStaticText *StaticText11;
    TStaticText *StaticText12;
    TStaticText *StaticText13;
    TStaticText *StaticText14;
    TStaticText *StaticText15;
    TStaticText *StaticText16;
    TStaticText *StaticText17;
    TStaticText *StaticText18;
    TPanel *Panel40;
    TComboBox *cbMotor;
    TLabel *Label60;
    TEdit *edMotorName;
    TBitBtn *btSaveMotor;
    TStaticText *StaticText20;
    TStaticText *StaticText21;
    TStaticText *StaticText22;
    TStaticText *StaticText23;
    TStaticText *StaticText24;
    TStaticText *StaticText25;
    TStaticText *StaticText26;
    TStaticText *StaticText27;
    TStaticText *StaticText28;
    TStaticText *StaticText29;
    TStaticText *StaticText30;
    TBitBtn *btActrStop;
    TCheckBox *cbActrSync;
    TEdit *edActrSync;
    TEdit *edRptDelay;
    TLabel *Label4;
    TPanel *Panel12;
    TPanel *Panel13;
    TImage *Image11;
    TLabel *Label52;
    TRadioButton *rbYelFlick;
    TRadioButton *rbYelOff;
    TRadioButton *rbYelOn;
    TPanel *Panel14;
    TPanel *Panel15;
    TImage *Image12;
    TLabel *Label53;
    TRadioButton *rbGrnOn;
    TRadioButton *rbGrnOff;
    TRadioButton *rbGrnFlick;
    TPanel *Panel19;
    TLabel *Label22;
    TLabel *Label23;
    TLabel *Label32;
    TLabel *Label33;
    TPanel *Panel20;
    TImage *Image14;
    TLabel *Label55;
    TEdit *edRedAdd;
    TEdit *edYelAdd;
    TEdit *edGrnAdd;
    TEdit *edBuzzAdd;
    TPanel *Panel16;
    TPanel *Panel17;
    TImage *Image15;
    TLabel *Label54;
    TRadioButton *rbBuzMute;
    TRadioButton *rbBuz3;
    TRadioButton *rbBuz2;
    TRadioButton *rbBuz1;
    TPanel *Panel10;
    TPanel *Panel11;
    TImage *Image10;
    TLabel *Label51;
    TRadioButton *rbRedFlick;
    TRadioButton *rbRedOff;
    TRadioButton *rbRedOn;
    TBitBtn *btUpdate;
    TBitBtn *btSave;
    TLabel *Label5;
    TComboBox *cbHomeMethod;
    TLabel *lbZphase;
    TStaticText *StaticText31;
    TEdit *edLastHomeSpeed;
        TEdit *edBreakAdd;
        TLabel *Label6;
        TLabel *lbBreakOff;
        TButton *btBreakOff;
        TPanel *Panel51;
        TPanel *Panel52;
        TImage *Image25;
        TLabel *Label7;
        TEdit *edErrEnum;
        TCheckBox *cbIOTestMode;
        TBitBtn *btAllSrvOn;
        TBitBtn *btAllSrvOff;
        TBitBtn *btStopAll;
        TBitBtn *btFirstGo;
        TBitBtn *btScndGo;
        TLabel *Label8;
        TPanel *Panel53;
        TLabel *lbVer;
        TLabel *lbDate;
        TEdit *edIOHanraAdd;
        TEdit *edActrAddrIF_;
        TEdit *edActrAddrIB_;
        TEdit *edActrAddrOB_;
        TEdit *edActrAddrOF_;
    TImageList *imgDirection;
    TComboBox *cbActCtrType;
    TStaticText *StaticText33;
    TPanel *pnActCnt;
    TPanel *pnIODelay;
    TLabel *Label70;
    TLabel *Label9;
    TEdit *edIOOnDelay;
    TStaticText *StaticText19;
    TEdit *edIOOffDelay;
    TStaticText *StaticText32;
    TComboBox *cbLan;
    TLabel *Label10;
        TEdit *edIOEnum;
        TStaticText *StaticText34;
    TEdit *edActrEnum;
    TStaticText *StaticText35;
        TCheckBox *cbIODirectMode;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall cbMotorChange(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall btSaveMotorClick(TObject *Sender);
        void __fastcall cbIOChange(TObject *Sender);
        void __fastcall sgIODrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
        void __fastcall sgIODblClick(TObject *Sender);
        void __fastcall btBwdClick(TObject *Sender);
        void __fastcall btFwdClick(TObject *Sender);
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
        void __fastcall sgIOClick(TObject *Sender);
        void __fastcall btSaveIOClick(TObject *Sender);
        void __fastcall btApplyIOClick(TObject *Sender);
        void __fastcall btMtClrPosClick(TObject *Sender);
        void __fastcall lbVerClick(TObject *Sender);
        void __fastcall pcMainChange(TObject *Sender);
    void __fastcall btApplyErrClick(TObject *Sender);
    void __fastcall btSaveErrClick(TObject *Sender);
    void __fastcall BitBtn8Click(TObject *Sender);
    void __fastcall sgErrListClick(TObject *Sender);
    void __fastcall spErrRectMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall spErrRectMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall spErrRectMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall imErrPictureDblClick(TObject *Sender);
    void __fastcall btAllCountSaveClick(TObject *Sender);
    void __fastcall btActrRptClick(TObject *Sender);
    void __fastcall cbActrSyncClick(TObject *Sender);
    void __fastcall btActrStopClick(TObject *Sender);
    void __fastcall sgTowerClick(TObject *Sender);
    void __fastcall btUpdateClick(TObject *Sender);
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall cbTestModeClick(TObject *Sender);
        void __fastcall btBreakOffClick(TObject *Sender);
        void __fastcall pnErrImgClick(TObject *Sender);
        void __fastcall btAllHomeClick(TObject *Sender);
        void __fastcall cbIOTestModeClick(TObject *Sender);
        void __fastcall btAllSrvOnClick(TObject *Sender);
        void __fastcall btAllSrvOffClick(TObject *Sender);
        void __fastcall FormHide(TObject *Sender);
        void __fastcall btStopAllClick(TObject *Sender);
        void __fastcall btFirstGoClick(TObject *Sender);
        void __fastcall btScndGoClick(TObject *Sender);
        void __fastcall cbLanChange(TObject *Sender);
private:	// User declarations
//    TRectTracker* pTracker;
    void __fastcall AllDynamicCountSave();
    void __fastcall AllDynamicCountLoad();

    EN_LAN_SEL  m_iLanSel ;
public:		// User declarations


        __fastcall TFrmDllMain(TComponent* Owner);
    AnsiString OnGetVer(const AnsiString &sQuery);
    AnsiString OnGetAge();
    //For Rect Display.
    void __fastcall RectMouseDown (TShape *spErrRect, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall RectMouseMove (TShape *spErrRect, TShiftState Shift, int X, int Y                     );
    void __fastcall SetCursor     (TShape *spErrRect, int X, int Y                                        );
    void __fastcall RectMouseUp   (TShape *spErrRect, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall LvUpdate      (                                                                       );

    EN_LAN_SEL GetLang(){return m_iLanSel;}

    Graphics::TBitmap *pLtImg ;
    Graphics::TBitmap *pRtImg ;
    Graphics::TBitmap *pUpImg ;
    Graphics::TBitmap *pDnImg ;
    Graphics::TBitmap *pAwImg ;
    Graphics::TBitmap *pCwImg ;

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmDllMain *FrmDllMain;
//---------------------------------------------------------------------------
#endif
