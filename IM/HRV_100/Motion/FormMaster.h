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
        TGroupBox *GroupBox1;
        TButton *Button10;
        TEdit *Edit1;
        TButton *Button11;
        TGroupBox *GroupBox2;
        TButton *Button12;
        TButton *Button13;
        TEdit *Edit2;
        TButton *Button14;
        TBitBtn *btSaveMotor;
        TCheckBox *cbDebugMode;
        TLabel *Label7;
        TLabel *Label8;
        TEdit *edVisnYMaxMin;
        TComboBox *cbSetUnitHeight;
        TLabel *Label23;
        TLabel *Label24;
        TGroupBox *GroupBox3;
        TEdit *edGainPreLoad;
        TEdit *edOfstPreLoad;
        TEdit *edGainPeakStroke;
        TEdit *edOfstPeakStroke;
        TEdit *edGainCustomStroke;
        TEdit *edOfstCustomStroke;
        TEdit *edGainSensitivity;
        TEdit *edOfstSensitivity;
        TEdit *edGainImpedance;
        TEdit *edOfstImpedance;
        TEdit *edGainHysteresis;
        TEdit *edOfstHysteresis;
        TEdit *edGainLinearity;
        TEdit *edOfstLinearity;
        TLabel *lbPreLoad;
        TLabel *lbPeakStroke;
        TLabel *Label27;
        TLabel *Label28;
        TLabel *Label29;
        TLabel *lbImpedance;
        TLabel *Label31;
        TLabel *Label32;
        TLabel *Label33;
        TGroupBox *GroupBox4;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label9;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label15;
        TLabel *Label16;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label21;
        TLabel *Label22;
        TEdit *edMstOfsX1;
        TEdit *edMstOfsY1;
        TEdit *edMstOfsX2;
        TEdit *edMstOfsY2;
        TEdit *edMstOfsX3;
        TEdit *edMstOfsY3;
        TEdit *edMstOfsX4;
        TEdit *edMstOfsY4;
        TEdit *edMstOfsX5;
        TEdit *edMstOfsY5;
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
        void __fastcall Button10Click(TObject *Sender);
        void __fastcall Button11Click(TObject *Sender);
        void __fastcall Button12Click(TObject *Sender);
        void __fastcall Button13Click(TObject *Sender);
        void __fastcall Button14Click(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateMstDevOptn(bool bTable);
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
