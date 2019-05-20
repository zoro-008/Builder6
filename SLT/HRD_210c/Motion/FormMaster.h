//---------------------------------------------------------------------------

#ifndef FormMasterH
#define FormMasterH
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class TFrmMaster : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel3;
    TPanel *Panel5;
    TTimer *tmUpdate;
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TStringGrid *sgCycleStep;
    TCheckBox *cbPart1;
    TCheckBox *cbPart2;
    TCheckBox *cbPart3;
    TCheckBox *cbPart4;
    TButton *Button10;
    TButton *Button11;
    TButton *Button12;
    TCheckBox *cbPart5;
    TTabSheet *TabSheet3;
    TGroupBox *GroupBox1;
    TCheckBox *cbPart6;
    TCheckBox *cbPart7;
    TCheckBox *cbPart8;
    TCheckBox *cbPart9;
    TEdit *ediEpxOriSpd;
    TEdit *eddEpxOriOfs;
    TLabel *Label69;
    TLabel *Label70;
    TLabel *Label2;
    TLabel *Label3;
    TButton *btChipStatsCng;
    TLabel *Label4;
    TLabel *Label5;
    TEdit *eddVoiceCoilOffset;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TEdit *edd1stDis;
    TEdit *edd2stDis;
    TEdit *edd3stDis;
    TButton *Button1;
    TButton *Button2;
    TButton *Button3;
    TButton *Button4;
    TButton *Button5;
    TButton *Button6;
    TButton *Button7;
    TButton *Button8;
    TEdit *Edit1;
    TEdit *Edit2;
    TEdit *Edit3;
    TEdit *Edit4;
    TEdit *Edit5;
    TEdit *Edit6;
    TEdit *Edit7;
    TEdit *Edit8;
    TLabel *Label12;
    TLabel *Label13;
    TEdit *edStgXEjtCt;
    TLabel *Label14;
    TLabel *Label15;
    TEdit *edStgYEjtCt;
    TEdit *Edit9;
    TButton *Button9;
        TLabel *Label16;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *Label20;
        TEdit *edStgDia;
        TEdit *edEjtDia;
    TLabel *Label21;
    TLabel *Label22;
    TEdit *edStgYMaxExdAble;
    TGroupBox *GroupBox2;
    TLabel *Label1;
    TEdit *edModelName;
    TButton *btWChipClear;
    TButton *btFChipClear;
    TGroupBox *GroupBox3;
    TLabel *Label23;
    TEdit *edDeviceBackUpPath;
        TImage *Image1;
        TLabel *Label24;
        TLabel *Label26;
        TEdit *edToolCrashWfrY;
        TEdit *edToolCrashZ;
        TLabel *Label25;
        TEdit *edToolCrashRalY;
        TButton *Button13;
        TButton *Button14;
        TButton *Button15;
        TButton *Button16;
        TButton *Button17;
    TButton *btFix;
    TButton *btNotFix;
        TLabel *Label27;
    TCheckBox *cbDebugMode;
    TCheckBox *cbbIgnrEpoxy;
    TCheckBox *cbbIgnrDieAttach;
    TCheckBox *cbIgnrBfVision;
    TCheckBox *cbPickUpTestMode;
    TCheckBox *cbIgnrWfVision;
    TBitBtn *btSaveMotor;
    TLabel *Label19;
    TEdit *edVoiceCoilA;
    TLabel *Label29;
    TEdit *edVoiceCoilB;
        TCheckBox *cbEjectorDnFirst;
        TCheckBox *cbDryRun;
        TLabel *Label28;
        TLabel *Label30;
        TEdit *edRegripOfs;
    void __fastcall btSaveMotorClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall Button10Click(TObject *Sender);
    void __fastcall Button11Click(TObject *Sender);
    void __fastcall Button12Click(TObject *Sender);
        void __fastcall btWChipClearClick(TObject *Sender);
        void __fastcall btFChipClearClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall btChipStatsCngClick(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button5Click(TObject *Sender);
    void __fastcall Button9Click(TObject *Sender);
        void __fastcall Button13Click(TObject *Sender);
        void __fastcall Button15Click(TObject *Sender);
        void __fastcall Button14Click(TObject *Sender);
        void __fastcall Button16Click(TObject *Sender);
        void __fastcall Button17Click(TObject *Sender);
    void __fastcall btFixClick(TObject *Sender);
    void __fastcall btNotFixClick(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateMstOptn(bool bTable);
    void __fastcall UpdateEqpOptn(bool bTable);
public:		// User declarations
    __fastcall TFrmMaster(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
#endif
