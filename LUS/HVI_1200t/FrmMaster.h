//---------------------------------------------------------------------------

#ifndef FrmMasterH
#define FrmMasterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormMaster : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
    TPanel *Panel3;
    TPanel *Panel5;
    TImage *Image2;
    TLabel *Label19;
        TTimer *tmMaster;
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TGroupBox *GroupBox6;
        TStringGrid *sgCycleStep;
        TCheckBox *cbLDR;
        TCheckBox *cbRAL;
        TCheckBox *cbHED;
        TCheckBox *cbPSB;
        TCheckBox *cbULD;
        TButton *Button17;
        TButton *Button18;
        TButton *Button19;
        TButton *Button16;
        TButton *Button20;
        TButton *Button21;
        TCheckBox *cbPRB;
        TButton *btWork1;
        TButton *btWork2;
        TButton *btWork3;
        TButton *Button12;
        TButton *Button15;
        TButton *Button22;
        TTabSheet *TabSheet2;
        TLabel *Label8;
        TLabel *Label12;
        TCheckBox *cbDebugMode;
        TEdit *edVisnPath;
        TGroupBox *GroupBox1;
        TCheckBox *cbUld_YHome;
        TCheckBox *cbLdr_YHome;
        TGroupBox *GroupBox2;
        TLabel *lbTrigerOffset;
        TButton *Button7;
        TButton *Button9;
        TButton *Button11;
        TEdit *edTrigerOffset;
        TGroupBox *GroupBox3;
        TCheckBox *cbDataAccEng;
        TGroupBox *GroupBox4;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label15;
        TLabel *Label17;
        TEdit *edT1_1;
        TEdit *edT1_2;
        TEdit *edT1_3;
        TEdit *edT1_4;
        TEdit *edT1_5;
        TEdit *edT1_6;
        TButton *Button1;
        TButton *Button2;
        TButton *Button13;
        TEdit *edT1_7;
        TButton *Button6;
        TGroupBox *GroupBox5;
        TLabel *Label7;
        TLabel *Label9;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label16;
        TLabel *Label18;
        TEdit *edT2_1;
        TEdit *edT2_2;
        TEdit *edT2_3;
        TEdit *edT2_4;
        TEdit *edT2_5;
        TEdit *edT2_6;
        TButton *Button3;
        TButton *Button4;
        TButton *Button5;
        TEdit *edT2_7;
        TButton *Button10;
        TCheckBox *cbDryRun;
        TCheckBox *cbPsbVsErrIgnr;
        TCheckBox *cbEmptyNoTrm;
        TBitBtn *btSaveMotor;
        TButton *Button23;
        TLabel *Label20;
        TEdit *edVisionNo;
        TCheckBox *cbUseRamVsData;
        TLabel *Label21;
        TLabel *Label22;
        TEdit *edControlPath;
        TCheckBox *cbLotOpenAuto;
        TCheckBox *cbUseZMarkMT;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btSaveMotorClick(TObject *Sender);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall Button9Click(TObject *Sender);
        void __fastcall FormHide(TObject *Sender);
        void __fastcall Button13Click(TObject *Sender);
        void __fastcall Label19Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall Button10Click(TObject *Sender);
        void __fastcall Button17Click(TObject *Sender);
        void __fastcall Button18Click(TObject *Sender);
        void __fastcall Button19Click(TObject *Sender);
        void __fastcall tmMasterTimer(TObject *Sender);
        void __fastcall Button16Click(TObject *Sender);
        void __fastcall Button20Click(TObject *Sender);
        void __fastcall Button21Click(TObject *Sender);
        void __fastcall Button11Click(TObject *Sender);
        void __fastcall btWork1Click(TObject *Sender);
        void __fastcall Button23Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormMaster(TComponent* Owner);
    void __fastcall UpdateMstOptn(bool bTable);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMaster *FormMaster;
//---------------------------------------------------------------------------
#endif
