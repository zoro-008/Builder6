//---------------------------------------------------------------------------

#ifndef FormMasterH
#define FormMasterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>

#include "UtilDefine.h"
#include "Rs232Dispr.h"
#include "Rs232Keyence.h"
#include "Rs232LoadCl.h"
#include "Rs232UVCuring.h"

#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmMaster : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TPanel *Panel3;
        TPanel *Panel5;
        TGroupBox *GroupBox8;
        TStringGrid *sgCycleStep;
        TButton *btPartAutorun;
        TButton *btPartReset;
        TButton *btAllReset;
        TButton *btPartHome;
        TTimer *tmUpdate;
        TButton *btAllCheck;
        TTabSheet *TabSheet3;
        TLabel *Label2;
        TCheckBox *cbDebugMode;
        TComboBox *cbPartSel;
        TStringGrid *sgTime;
        TBitBtn *btSaveMotor;
        TLabel *lbMan;
        TTabSheet *TabSheet2;
        TTabSheet *TabSheet4;
        TGroupBox *GroupBox9;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *lbDisprMsgEnd;
        TGroupBox *GroupBox1;
        TButton *btDisprOn;
        TButton *btDisprOff;
        TGroupBox *GroupBox2;
        TComboBox *cbSetMode;
        TButton *btDisprSetMode;
        TGroupBox *GroupBox3;
        TButton *btDisprSigmaTest;
        TGroupBox *GroupBox4;
        TButton *btDisprLoadCh;
        TComboBox *cbLoadCh;
        TGroupBox *GroupBox5;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TEdit *edPres;
        TEdit *edTime;
        TEdit *edVacc;
        TButton *btDisprSetPTV;
        TGroupBox *GroupBox6;
        TButton *btDisprSyrngSize;
        TComboBox *cbSyrgSize;
        TEdit *edDisprErrMsg;
        TGroupBox *GroupBox10;
        TGroupBox *GroupBox11;
        TButton *btHghtSetZero;
        TComboBox *cbSetZeroCh;
        TGroupBox *GroupBox12;
        TButton *btHghtCheck;
        TComboBox *cbChkHgtCh;
        TGroupBox *GroupBox13;
        TButton *btHghtGetDat;
        TComboBox *cbGetHghDataCh;
        TGroupBox *GroupBox7;
        TEdit *edHghDataCh0;
        TLabel *Label9;
        TLabel *Label8;
        TLabel *lbHghtMsgEnd;
        TLabel *Label11;
        TLabel *lbLoadClMsgEnd;
        TGroupBox *GroupBox14;
        TButton *btLoadClWghtChck;
        TGroupBox *GroupBox16;
        TButton *btLoadClHoldOn;
        TButton *btLoadClHoldOff;
        TLabel *Label10;
        TEdit *edLoadClData;
        TGroupBox *GroupBox15;
        TLabel *Label19;
        TEdit *edLoadClMsg;
        TLabel *Label20;
        TEdit *edHghDataCh1;
        TLabel *Label21;
        TEdit *edHghMsg;
        TGroupBox *GroupBox18;
        TButton *btUVOn;
        TButton *btUVOff;
        TGroupBox *GroupBox19;
        TEdit *edUVLightData;
        TButton *btUVLight;
        TLabel *Label16;
        TEdit *edUVCuringMsg;
        TLabel *lbMessageEndSignal;
        TLabel *lbUVMsgEnd;
        TComboBox *cbUVCh1;
        TLabel *Label12;
        TGroupBox *GroupBox17;
        TEdit *edUVDataChk;
        TButton *btUVLightDataChk;
        TLabel *Label1;
        TLabel *Label13;
        TLabel *Label14;
        TEdit *edLTL_RTL_Stroke;
        TEdit *edLTL_VISN_DISP_Strke;
        TEdit *edRTL_TRNS_VISN_Strke;
        TLabel *Label15;
        TLabel *Label17;
        TLabel *Label18;
        TComboBox *cbUVCh2;
        TComboBox *cbUVCh3;
        TButton *Button1;
        TLabel *Label22;
        TTabSheet *TabSheet5;
        TButton *btHome;
        TLabel *Label23;
        TLabel *Label24;
        TLabel *Label25;
        TLabel *Label26;
        TLabel *Label27;
        TLabel *Label28;
        TLabel *Label29;
        TLabel *lbErr;
        TButton *btMoveTo;
        TEdit *Edit1;
        TEdit *Edit2;
        TEdit *Edit3;
        TEdit *Edit4;
        TEdit *Edit5;
        TEdit *Edit6;
        TEdit *edEncX;
        TEdit *edEncY;
        TEdit *edEncZ;
        TEdit *edEncU;
        TEdit *edEncV;
        TEdit *edEncW;
        TLabel *Label30;
        TLabel *Label31;
        TLabel *Label32;
        TEdit *edPivotX;
        TEdit *edPivotY;
        TEdit *edPivotZ;
        TButton *Button2;
        TLabel *Label36;
        TLabel *Label37;
        TLabel *Label38;
        TLabel *Label39;
        TLabel *Label40;
        TLabel *Label41;
        TEdit *edLimMinX;
        TEdit *edLimMinY;
        TEdit *edLimMinZ;
        TEdit *edLimMinU;
        TEdit *edLimMinV;
        TEdit *edLimMinW;
        TEdit *edLimMaxX;
        TEdit *edLimMaxY;
        TEdit *edLimMaxZ;
        TEdit *edLimMaxU;
        TEdit *edLimMaxV;
        TEdit *edLimMaxW;
        TPanel *Panel1;
        TTabSheet *TabSheet6;
        TButton *Button3;
        TComboBox *cbSendMsg;
        TButton *Button4;
        TTimer *tmVisn;
        TEdit *edCmd;
        TLabel *Comman;
        TEdit *edJobName;
        TLabel *Label33;
        TPanel *pnInspEndL;
        TPanel *pnResetEndL;
        TPanel *pnCommandEndL;
        TPanel *pnJobChangeL;
        TPanel *pnInspEndR;
        TPanel *pnResetEndR;
        TPanel *pnCommandEndR;
        TPanel *pnJobChangeR;
        TLabel *Vsdss;
        TLabel *Label34;
        TLabel *lbLeftResult;
        TLabel *lbRightResult;
        TPanel *pnInspLRslt;
        TPanel *pnInspRRslt;
        TButton *btHexaServoOn;
        TButton *btHexaServoOff;
        TButton *btSaveTrgPos;
        TComboBox *cbDisprSelect;
        TLabel *Label35;
        TButton *btCMRectCenter;
        TButton *btCycleCmsLAlign;
        TButton *btDataCopy;
        TGroupBox *GroupBox20;
        TButton *Button5;
        TCheckBox *cbDonUseMasterT;
        TButton *Button6;
        TButton *Button7;
        TCheckBox *cbUseLotInfo;
        TGroupBox *GroupBox21;
        TButton *btConfocalRezeroOn;
        TButton *Button8;
    void __fastcall btSaveMotorClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall btPartAutorunClick(TObject *Sender);
    void __fastcall btPartResetClick(TObject *Sender);
    void __fastcall btAllResetClick(TObject *Sender);
    void __fastcall btPartHomeClick(TObject *Sender);
        void __fastcall cbPartSelChange(TObject *Sender);
        void __fastcall btAllCheckClick(TObject *Sender);
        void __fastcall btDisprOnClick(TObject *Sender);
        void __fastcall btHghtSetZeroClick(TObject *Sender);
        void __fastcall btLoadClWghtChckClick(TObject *Sender);
        void __fastcall btSetTempClick(TObject *Sender);
        void __fastcall btUVChClick(TObject *Sender);
        void __fastcall btHomeClick(TObject *Sender);
        void __fastcall btMoveToClick(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Panel1Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall tmVisnTimer(TObject *Sender);
        void __fastcall btHexaServoOffClick(TObject *Sender);
        void __fastcall btHexaServoOnClick(TObject *Sender);
        void __fastcall btSaveTrgPosClick(TObject *Sender);
        void __fastcall btCycleCmsLAlignClick(TObject *Sender);
        void __fastcall btCMRectCenterClick(TObject *Sender);
        void __fastcall btDataCopyClick(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall btConfocalRezeroOnClick(TObject *Sender);
        void __fastcall Button8Click(TObject *Sender);

private:	// User declarations
    void __fastcall UpdateMstOptn(bool bTable);
    void __fastcall UpdateEqpOptn(bool bTable);

    void UpdateTimeName ();

    enum BT_TAG{
        btTagNum1 = 1 ,
        btTagNum2 = 2 ,
        btTagNum3 = 3 ,
        btTagNum4 = 4 ,
        btTagNum5 = 5 ,
        btTagNum6 = 6 ,
        btTagNum7 = 7 ,
        btTagNum8 = 8 ,
        MAX_BT_Dispr
    };

public:		// User declarations
    __fastcall TFrmMaster(TComponent* Owner);

    int iHeightCh ;

    TCheckBox * cbPart[MAX_PART] ;
    bool        bReceive;

   // CRs232Dispr   Rs232_Dispr  ;
   // CRs232Keyence Rs232_HgtSnr ;
   // CRs232LoadCl  Rs232_LoadCl ;
    

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
#endif
