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
#include "Rs232Temp.h"

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
        TOpenDialog *dgFilePath;
        TButton *btAllCheck;
        TTabSheet *TabSheet3;
        TLabel *Label2;
        TCheckBox *cbDebugMode;
        TComboBox *cbPartSel;
        TStringGrid *sgTime;
        TBitBtn *btSaveMotor;
        TLabel *Label13;
        TEdit *edPckDspStroke;
        TLabel *Label1;
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
        TGroupBox *GroupBox17;
        TComboBox *cbTempCh;
        TButton *btSetTemp;
        TEdit *edSetTemp;
        TGroupBox *GroupBox18;
        TEdit *edCrntTemp1;
        TEdit *edCrntTemp2;
        TEdit *edCrntTemp3;
        TLabel *Label12;
        TLabel *Label14;
        TLabel *Label15;
        TGroupBox *GroupBox20;
        TLabel *Label16;
        TLabel *Label17;
        TLabel *Label18;
        TEdit *edGetTemp1;
        TEdit *edGetTemp2;
        TEdit *edGetTemp3;
        TLabel *Label19;
        TEdit *edLoadClMsg;
        TLabel *Label20;
        TEdit *edHghDataCh1;
        TLabel *Label21;
        TEdit *edHghMsg;
        TTabSheet *TabSheet5;
        TButton *Button1;
        TCheckBox *cbSkipPickrDispr;
        TEdit *edPckToStgAngleOfs;
        TLabel *Label22;
        TLabel *Label23;
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
        void __fastcall Button1Click(TObject *Sender);

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

    //CRs232Dispr   Rs232_Dispr  ;
    //CRs232Keyence Rs232_HgtSnr ;
    //CRs232LoadCl  Rs232_LoadCl ;
    

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
#endif
