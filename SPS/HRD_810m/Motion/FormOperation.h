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
//---------------------------------------------------------------------------
#include "FrameMotr.h"
#include "FrameCyl.h"
//---------------------------------------------------------------------------
#include "Vision.h"
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
    TTimer *tmUpdate;
    TPanel *pnVisn;
    TPanel *pnControl;
    TPanel *Panel8;
    TLabel *lbDInfo0;
    TLabel *lbDInfo1;
    TLabel *lbDInfo2;
    TLabel *lbDInfo3;
    TLabel *lbDInfo5;
    TLabel *lbDInfo7;
    TLabel *lbDInfo6;
    TPanel *Panel9;
    TPanel *pnDayRunTime;
    TPanel *pnDayIdleTime;
    TPanel *pnDayJamTime;
    TPanel *pnDayTotalTime;
        TPanel *pnDayMarkChip;
    TPanel *pnDayLotCount;
    TPanel *pnDayFailChip;
    TPanel *Panel2;
    TLabel *lbLInfo0;
    TLabel *lbLInfo2;
    TLabel *lbLInfo5;
    TLabel *lbLInfo6;
    TLabel *lbLInfo3;
    TLabel *lbLInfo1;
    TPanel *Panel11;
    TPanel *pnLotNo;
    TPanel *pnLotWorkChip;
    TPanel *pnLotStartTime;
    TPanel *pnLotRunTime;
    TPanel *pnLotFailChip;
    TPanel *pnLotJobFile;
    TPanel *Panel12;
    TPanel *Panel13;
    TPanel *Panel32;
    TLabel *lbOption0;
        TPanel *pnOptn1;
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
        TPanel *pnOptn0;
    TPanel *Panel21;
    TLabel *lbOption5;
    TPanel *pnOptn5;
    TPanel *Panel17;
    TLabel *lbOption6;
    TPanel *pnOptn6;
    TPanel *Panel14;
    TPanel *Panel15;
    TPanel *Panel16;
    TPanel *Panel18;
    TBitBtn *btLotOpen;
    TBitBtn *btLotEnd;
        TPanel *pnManual;
    TPanel *Panel20;
    TPanel *plPassWord;
    TPanel *Panel19;
    TLabel *Label17;
    TBitBtn *btPasswordClose;
    TBitBtn *btEngr;
    TBitBtn *btOper;
    TBitBtn *btMast;
    TPanel *pnTVisn;
    TPanel *pnTControl;
    TPanel *pnVisnBase;
        TLabel *Label11;
        TPanel *pnPickCycleTime;
        TTimer *tmUpdateAray;
        TOpenDialog *odMap;
        TSaveDialog *sdMap;
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
        TTabSheet *TabSheet3;
        TBitBtn *btMan3_1;
        TBitBtn *btMan3_2;
        TBitBtn *btMan3_7;
        TBitBtn *btMan3_3;
        TBitBtn *btMan3_4;
        TBitBtn *btMan3_5;
        TBitBtn *btMan3_6;
        TBitBtn *btMan3_8;
        TBitBtn *btMan3_9;
        TBitBtn *btMan3_A;
        TBitBtn *btMan3_B;
        TBitBtn *btMan3_C;
        TBitBtn *btMan3_D;
        TBitBtn *btMan3_E;
        TLabel *lbLInfo4;
        TPanel *pnLotWaferUph;
        TPanel *pnLotWaferUpeh;
        TLabel *Label12;
        TLabel *Label31;
        TPanel *pnDayWaferUpeh;
        TPanel *pnDayWaferUph;
        TLabel *lbDInfo4;
        TMemo *mmMemo;
    TPageControl *pcControl;
    TTabSheet *TabSheet9;
        TPanel *pnCapture;
    TPanel *Panel5;
    TPanel *Panel10;
    TPanel *pnSTG;
    TButton *btSTG_ClearClick;
    TPanel *Panel28;
    TStaticText *StaticText8;
    TStaticText *lbWfrGoodCnt;
    TStaticText *StaticText13;
    TStaticText *lbWfrMaskCnt;
    TStaticText *StaticText15;
    TStaticText *lbWfrFailCnt;
    TPanel *Panel26;
    TStaticText *StaticText3;
    TStaticText *lbWfrWorkCnt;
    TBitBtn *btSaveAs;
    TBitBtn *btConvertOpen;
    TBitBtn *btOpenAs;
    TPanel *Panel30;
    TPanel *Panel31;
    TPanel *pnCST;
    TPanel *Panel35;
    TPanel *Panel36;
    TButton *btBRD_Clear;
    TPanel *pnBRD;
    TStringGrid *sgTmp;
    TTabSheet *TabSheet14;
    TPanel *Panel43;
    TPanel *Panel46;
    TPanel *pnMAP;
    TButton *btMAP_ClearClick;
    TPanel *Panel60;
    TPanel *Panel61;
    TStaticText *StaticText9;
    TStaticText *lbGoodCnt;
    TStaticText *StaticText2;
    TStaticText *lbFailCnt;
    TStaticText *StaticText4;
    TStaticText *lbMarkCnt;
    TBitBtn *btMapOpen;
    TTabSheet *TabSheet4;
    TBitBtn *btRawMapOpen;
    TBitBtn *btRawMapClear;
    TTabSheet *tsSetup;
    TGroupBox *GroupBox1;
    TBitBtn *btTRS_XRt;
    TBitBtn *btTRS_XLf;
    TGroupBox *GroupBox3;
    TBitBtn *btCST_ZUp;
    TBitBtn *btCST_ZDn;
    TGroupBox *GroupBox9;
    TBitBtn *btSTG_WfBw;
    TBitBtn *btSTG_WfFw;
    TShape *spManOri;
    TShape *spReMark;
    TGroupBox *GroupBox2;
    TBitBtn *btHed_ZUp;
    TBitBtn *btHed_ZDn;
    TGroupBox *GroupBox6;
    TBitBtn *btTRS_CmpFw;
    TBitBtn *btTRS_CmpBw;
    TButton *btManOri;
    TButton *btSetT;
    TButton *btTraceBwd;
    TButton *btTraceFwd;
    TButton *btReMark;
    TGroupBox *GroupBox8;
    TBitBtn *btHed_CovDn;
    TBitBtn *btHed_CovUp;
    TGroupBox *GroupBox4;
    TBitBtn *btHED_TCl;
    TBitBtn *btHED_TUc;
    TRadioGroup *rgJogUnit;
    TEdit *edJogUnit;
    TGroupBox *GroupBox20;
    TBitBtn *btHED_YUp;
    TBitBtn *btStopHed;
    TBitBtn *btHED_XRt;
    TBitBtn *btSTG_TCl;
    TBitBtn *btHED_YDn;
    TBitBtn *btSTG_TUc;
    TBitBtn *btHED_XLt;
    TGroupBox *GroupBox5;
    TBitBtn *btSTG_ChkBw;
    TBitBtn *btSTG_ChkFw;
    TGroupBox *GroupBox7;
    TBitBtn *btHED_PenFw;
    TBitBtn *btHED_PenBw;
    TListBox *lbErr;
    TButton *Button1;
        TLabel *Label1;
        TPanel *Panel3;
        TLabel *lbPenCnt;
        TLabel *Label3;
        TLabel *Label4;
        TPanel *pnPenCntReset;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btOperatorClick(TObject *Sender);
    void __fastcall btPasswordCloseClick(TObject *Sender);
    void __fastcall btOperClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action); 
        void __fastcall btStopClick(TObject *Sender);
        void __fastcall btResetClick(TObject *Sender);
    void __fastcall btMan1_1Click(TObject *Sender);
    void __fastcall btStartClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall pnOptn1Click(TObject *Sender);
    void __fastcall btSourceClick(TObject *Sender);
    void __fastcall btWorkedClick(TObject *Sender);
    void __fastcall btLotOpenClick(TObject *Sender);
    void __fastcall btLotEndClick(TObject *Sender);
    void __fastcall rgJogUnitClick(TObject *Sender);
        void __fastcall btJogNMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btJogNMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall btJogPMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btJogPMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall tmUpdateArayTimer(TObject *Sender);
    void __fastcall btStopHedClick(TObject *Sender);
        void __fastcall btSaveClick(TObject *Sender);
        void __fastcall btBRD_ClearClick(TObject *Sender);
        void __fastcall btSTG_ChkFwClick(TObject *Sender);
        void __fastcall btSTG_ChkBwClick(TObject *Sender);
    void __fastcall btRawMapOpenClick(TObject *Sender);
        void __fastcall btMapOpenClick(TObject *Sender);
        void __fastcall btConvertOpenClick(TObject *Sender);
        void __fastcall btSaveAsClick(TObject *Sender);
        void __fastcall btOpenAsClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall btManOriClick(TObject *Sender);
        void __fastcall btSetTClick(TObject *Sender);
        void __fastcall btSTG_ClearClickClick(TObject *Sender);
        void __fastcall btRawMapClearClick(TObject *Sender);
        void __fastcall btMAP_ClearClickClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall btTraceBwdClick(TObject *Sender);
        void __fastcall btTraceFwdClick(TObject *Sender);
        void __fastcall btReMarkClick(TObject *Sender);
        void __fastcall pnTControlClick(TObject *Sender);
        void __fastcall Panel20Click(TObject *Sender);
        void __fastcall Panel15Click(TObject *Sender);
        void __fastcall Panel18Click(TObject *Sender);
        void __fastcall Panel9Click(TObject *Sender);
        void __fastcall Panel11Click(TObject *Sender);
        void __fastcall Panel13Click(TObject *Sender);
        void __fastcall pnPenCntResetClick(TObject *Sender);

private:	// User declarations
    double m_dUnit    ;
    double m_dPitch   ;
    int    m_iUnitType;



    void DM_Init   (); //Data Man Init.
    void Man_Init  (); //Manual Button , Tag Init.
    void LT_Display(); //Lot Display
    void OP_Dispaly(); //Option Display
    void LV_Display(); //Level Display
    void __fastcall SetManualBtn(TBitBtn *_btn , String _sCaption , int _iTag) ;

    void SetUnitInit();
    void SetPitch(double _dUnit);
    void SetUnit(EN_UNIT_TYPE _iUnitType , double _dUnit);

    void UpdateAray();

    EN_VISN_ID m_iVisnId ;
    
    void PaintCallback(); //Camera Callback


public:		// User declarations
    __fastcall TFrmOperation(TComponent* Owner);

    void __fastcall GetPanelScreenShot(TPanel * _pPanel, String _sPath);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
#endif
