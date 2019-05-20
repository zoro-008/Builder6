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
//---------------------------------------------------------------------------
class TFrmMaster : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
    TPanel *Panel3;
    TPanel *Panel5;
    TImage *Image2;
    TLabel *Label19;
    TLabel *Label1;
    TLabel *Label2;
    TCheckBox *cbDebugMode;
    TEdit *edPreEquipPath;
    TEdit *edVisnPath;
    TBitBtn *btSaveMotor;
    TLabel *Label3;
        TEdit *edPstEquipDrv;
    TEdit *edVisnDrv;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
        TEdit *edPstEquipIp;
    TEdit *edVisnIp;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TButton *Button1;
    TButton *Button2;
    TButton *Button3;
    TButton *Button4;
    TButton *Button5;
    TButton *Button6;
        TCheckBox *cbExistUnLoader;
        TCheckBox *cbExistLoader;
        TLabel *Label13;
        TEdit *Edit1;
        TLabel *Label14;
        TEdit *edPstEquipPath;
        TLabel *Label15;
        TButton *Button7;
        TButton *Button8;
        TButton *Button9;
        TStringGrid *sgCycleStep;
        TCheckBox *cbLDR;
        TCheckBox *cbRAL;
        TCheckBox *cbHED;
        TCheckBox *cbPSB;
        TCheckBox *cbULD;
        TButton *Button10;
        TTimer *tmMaster;
        TButton *Button11;
        TButton *Button12;
        TButton *Button13;
        TEdit *Edit2;
        TButton *Button14;
        TCheckBox *cbBarcodeOnly;
    TGroupBox *GroupBox1;
    TCheckBox *cbUld_YHome;
    TCheckBox *cbPSB_XYTrmHome;
    TCheckBox *cbLdr_YHome;
    TGroupBox *GroupBox2;
    TCheckBox *cbEmptyNoTrim;
        TCheckBox *cbUseHangul;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btSaveMotorClick(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall Button5Click(TObject *Sender);
    void __fastcall Button6Click(TObject *Sender);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall Button9Click(TObject *Sender);
        void __fastcall tmMasterTimer(TObject *Sender);
        void __fastcall FormHide(TObject *Sender);
        void __fastcall Button10Click(TObject *Sender);
        void __fastcall Button11Click(TObject *Sender);
        void __fastcall Button12Click(TObject *Sender);
        void __fastcall Button13Click(TObject *Sender);
        void __fastcall Button14Click(TObject *Sender);
        void __fastcall Label19Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmMaster(TComponent* Owner);
    void __fastcall UpdateMstOptn(bool bTable);
    void __fastcall UpdateEqpOptn(bool bTable);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
#endif
