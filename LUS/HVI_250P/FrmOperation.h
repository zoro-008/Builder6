//---------------------------------------------------------------------------

#ifndef FrmOperationH
#define FrmOperationH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>

//---------------------------------------------------------------------------

class TFormOperation : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TPanel *Panel3;
        TLabel *Label27;
        TLabel *Label28;
        TMemo *mmAction;
        TPanel *Panel4;
        TGroupBox *GroupBox2;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label8;
        TLabel *Label9;
        TGroupBox *GroupBox3;
        TGroupBox *GroupBox4;
        TCheckBox *cbBarCodePrintSkip;
        TCheckBox *cbDoorSkip;
        TTimer *tmUpdate;
        TPanel *pnLotNo;
        TPanel *pnLotJobFile;
        TPanel *pnLotWorkStrip;
        TPanel *pnLotWorkChip;
        TListBox *lbErr;
        TLabel *Label7;
        TLabel *lbWRK_ZPch;
        TCheckBox *cbUseOnly1Bin;
        TPanel *Panel2;
        TGroupBox *GroupBox9;
        TLabel *Label32;
        TLabel *Label31;
        TButton *btPRBInput;
        TButton *btPRBClear;
        TPanel *pnPreBuff;
        TGroupBox *GroupBox6;
        TLabel *Label13;
        TLabel *Label15;
        TEdit *edWk2LotNo;
        TButton *btWk2Input;
        TEdit *edWk2Id;
        TButton *btPWKClear;
        TPanel *pnWorkZone;
        TGroupBox *GroupBox7;
        TButton *Button1;
        TButton *btINBClear;
        TPanel *pnInBuff;
        TGroupBox *GroupBox5;
        TLabel *Label3;
        TLabel *Label4;
        TEdit *Edit1;
        TButton *Button2;
        TEdit *Edit2;
        TButton *btWRKClear;
        TPanel *pnWorkLast;
        TGroupBox *GroupBox8;
    TLabel *lbBin5;
        TLabel *Label11;
    TLabel *lbBin4;
    TLabel *lbBin3;
    TLabel *lbBin2;
        TLabel *Label18;
        TBitBtn *btBinBoxRemove;
        TPanel *pnBin5Cnt;
        TPanel *pnBin5Per;
        TPanel *pnTotalBinPer;
        TPanel *pnBin4Cnt;
        TPanel *pnBin4Per;
        TPanel *pnBin3Cnt;
        TPanel *pnBin3Per;
        TPanel *pnBin1Per;
        TPanel *pnBin1Cnt;
        TPanel *pnBin2Per;
        TPanel *pnBin2Cnt;
        TGroupBox *GroupBox1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label14;
        TPanel *pnTotalFrmCnt;
        TPanel *pnTotalLedCnt;
        TPanel *pnTotalToolCnt;
        TEdit *edPRBId;
        TEdit *edPRBLotNo;
        TLabel *Label19;
        TEdit *edINBLotNo;
        TLabel *Label20;
        TEdit *edINBId;
        TLabel *Label21;
        TEdit *edPWKLotNo;
        TLabel *Label22;
        TEdit *edPWKId;
        TLabel *Label23;
        TEdit *edWRKLotNo;
        TLabel *Label24;
        TEdit *edWRKId;
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btPRBClearClick(TObject *Sender);
        void __fastcall btPRBInputClick(TObject *Sender);
        void __fastcall lbErrClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall cbDoorSkipClick(TObject *Sender);
        void __fastcall btBinBoxRemoveClick(TObject *Sender);
        void __fastcall pnTotalFrmCntDblClick(TObject *Sender);
        void __fastcall pnBin1CntClick(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateCmmOptn(bool bTable);
public:		// User declarations
    __fastcall TFormOperation(TComponent* Owner);
    bool __fastcall CheckStripDisappear();
    bool __fastcall CheckStripUnknwon  ();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormOperation *FormOperation;
//---------------------------------------------------------------------------
#endif



