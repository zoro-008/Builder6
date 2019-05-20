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
        TLabel *lbMan;
        TStringGrid *sgCycleStep;
        TButton *btPartAutorun;
        TButton *btPartReset;
        TButton *btAllReset;
        TButton *btPartHome;
        TStringGrid *sgTime;
        TComboBox *cbPartSel;
        TTimer *tmUpdate;
        TOpenDialog *dgFilePath;
        TTabSheet *TabSheet2;
        TButton *btAllCheck;
        TTabSheet *TabSheet3;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *VT;
        TLabel *Label4;
        TLabel *Label5;
        TCheckBox *cbDebugMode;
        TBitBtn *btSaveMotor;
        TCheckBox *cbVacErrIgnr;
        TEdit *edTrigerOffset;
        TEdit *edVisnRsltFile1;
        TButton *btVisnRsltFile1;
        TEdit *edVTDataFile1;
        TButton *Button1;
        TButton *Button3;
        TEdit *edVisnDeviceFile;
        TButton *Button2;
        TEdit *edInspRandMask;
        TLabel *Label3;
        TLabel *Label6;
        TLabel *Label7;
        TEdit *edVisnRsltFile2;
        TButton *btVisnRsltFile2;
        TLabel *Label8;
        TEdit *edVTDataFile2;
        TButton *Button21;
        TLabel *Label9;
        TEdit *edVTDataFile3;
        TButton *Button22;
        TLabel *Label10;
        TEdit *edVTDataFile4;
        TButton *Button23;
        TLabel *Label11;
        TLabel *Label12;
        TEdit *edVisnLotNoFile;
        TButton *Button4;
        TMemo *Memo1;
        TMemo *Memo2;
        TTimer *tmOSTMsg;
        TButton *Button5;
        TBitBtn *BitBtn1;
        TEdit *Edit3;
        TButton *Button6;
        TLabel *Label13;
        TButton *Button7;
        TButton *Button8;
        TEdit *edOSTOffset1;
        TEdit *edOSTGain1;
        TEdit *edOSTOffset2;
        TEdit *edOSTGain2;
        TEdit *edOSTOffset3;
        TEdit *edOSTGain3;
        TEdit *edOSTOffset4;
        TEdit *edOSTGain4;
        TLabel *Label14;
        TLabel *Label15;
        TLabel *Label16;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *Label19;
        TButton *Button9;
    void __fastcall btSaveMotorClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall btPartAutorunClick(TObject *Sender);
    void __fastcall btPartResetClick(TObject *Sender);
    void __fastcall btAllResetClick(TObject *Sender);
        void __fastcall btVisnRsltFile1Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
    void __fastcall btPartHomeClick(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall cbPartSelChange(TObject *Sender);
        void __fastcall btAllCheckClick(TObject *Sender);
        void __fastcall btVisnRsltFile2Click(TObject *Sender);
        void __fastcall Button21Click(TObject *Sender);
        void __fastcall Button22Click(TObject *Sender);
        void __fastcall Button23Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall tmOSTMsgTimer(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall Memo1Change(TObject *Sender);
        void __fastcall Button8Click(TObject *Sender);
        void __fastcall Button9Click(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateMstOptn(bool bTable);
    void __fastcall UpdateEqpOptn(bool bTable);

    void UpdateTimeName ();
public:		// User declarations
    __fastcall TFrmMaster(TComponent* Owner);

    TCheckBox * cbPart[MAX_PART] ;


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
#endif
