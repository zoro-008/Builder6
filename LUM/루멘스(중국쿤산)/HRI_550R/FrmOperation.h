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
        TPanel *Panel2;
        TLabel *lbR1;
        TLabel *lbR2;
        TLabel *lbR3;
        TLabel *lbR4;
        TLabel *lbR5;
        TLabel *lbR6;
        TLabel *lbR7;
        TLabel *lbR8;
        TLabel *lbR9;
        TLabel *lbR10;
        TLabel *lbR11;
        TLabel *lbR12;
        TLabel *lbR13;
        TLabel *lbR14;
        TPanel *Panel8;
    TPanel *pnColor1;
    TPanel *pnColor2;
    TPanel *pnColor3;
    TPanel *pnColor4;
    TPanel *pnColor5;
    TPanel *pnColor6;
    TPanel *pnColor7;
    TPanel *pnColor8;
    TPanel *pnColor9;
    TPanel *pnColor10;
    TPanel *pnColor11;
    TPanel *pnColor12;
    TPanel *pnColor13;
    TPanel *pnColor14;
        TGroupBox *GroupBox5;
        TPanel *pnLoader;
        TPanel *Panel3;
        TLabel *Label27;
        TLabel *Label28;
        TMemo *mmAction;
        TGroupBox *GroupBox8;
        TPanel *pnUnLoader;
        TGroupBox *GroupBox6;
        TLabel *Label25;
        TLabel *Label33;
        TPageControl *pcVsR1;
        TTabSheet *tsV1Result;
        TPanel *pnInsp1;
        TTabSheet *tsVs1;
        TPanel *pnVisn1;
        TButton *btVS1Reset;
        TButton *btWK1Clear;
        TEdit *edWk1LotNo;
        TButton *btWk1Input;
        TEdit *edWk1Id;
        TPanel *Panel4;
        TGroupBox *GroupBox1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TGroupBox *GroupBox2;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TGroupBox *GroupBox3;
        TCheckBox *cbMrk1Skip;
        TCheckBox *cbMrkAllSkip;
        TCheckBox *cbIgnrVsErr;
        TCheckBox *cbVsAllSkip;
        TCheckBox *cbLoadingStop;
        TGroupBox *GroupBox4;
        TCheckBox *cbVs1Skip;
        TCheckBox *cbVs2Skip;
        TCheckBox *cbVs3Skip;
        TCheckBox *cbVsMrkSkip;
        TGroupBox *GroupBox7;
        TLabel *Label26;
        TPageControl *pcVsR2;
        TTabSheet *tsV2Result;
        TPanel *pnInsp2;
        TTabSheet *tsVs2;
        TPanel *pnVisn2;
        TButton *btVS2Reset;
        TButton *btWK2Clear;
        TEdit *edWk2LotNo;
        TButton *btWk2Input;
        TGroupBox *GroupBox9;
        TLabel *Label32;
        TPageControl *pcVsR3;
        TTabSheet *tsV3Result;
        TPanel *pnInsp3;
        TTabSheet *tsVs3;
        TPanel *pnVisn3;
        TButton *btVS3Reset;
        TButton *btWK3Clear;
        TEdit *edWk3LotNo;
        TButton *btWk3Input;
        TGroupBox *GroupBox10;
        TLabel *Label37;
        TEdit *edWk4LotNo;
        TButton *btWk4Input;
        TTimer *tmUpdate;
        TCheckBox *cbMrkSam1Skip;
        TLabel *Label30;
        TEdit *edWk2Id;
        TLabel *Label31;
        TEdit *edWk3Id;
        TLabel *Label34;
        TEdit *edWk4Id;
        TPanel *pnLotNo;
        TPanel *pnLotJobFile;
        TPanel *pnLotWorkChip;
        TPanel *pnLotFailChip;
        TPanel *pnBadLimCnt;
        TPanel *pnLotYeild;
        TPanel *pnDayWorkChip;
        TPanel *pnDayFailChip;
        TPanel *pnTotalYield;
        TPanel *pnLotMGZCnt1;
        TListBox *lbErr;
        TPanel *pnLotMGZCnt2;
    TLabel *Label35;
    TEdit *edMeasure;
    TBitBtn *btnOK;
        TPageControl *pcVsR4;
        TTabSheet *tsV4Result;
        TTabSheet *tsVs4;
        TPanel *pnVisn4;
        TButton *btVb4Sel;
        TButton *btPSBReset;
        TButton *btPSBClear;
        TPanel *pnPsb;
        TTabSheet *tsV3R3;
        TTabSheet *tsV2R2;
        TPanel *pn2B2;
        TPanel *pn3B3;
        TStaticText *lbVs2FailCnt;
        TStaticText *lbVs1FailCnt;
        TStaticText *lbVs3FailCnt;
        TTabSheet *tsV1R1;
        TPanel *pn1B1;
        TStaticText *lbMrkFailCnt;
        TCheckBox *cbDoorSkip;
        TCheckBox *cbIdleRun;
        TGroupBox *GroupBox11;
        TPanel *pnPrb;
        TButton *btPRBClear;
        TLabel *Label11;
        TLabel *Label12;
        TEdit *edPrbId;
        TEdit *edPrbLotNo;
        TLabel *Label13;
        TPanel *pnLotGoodChip;
        TLabel *Label14;
        TPanel *pnTotalGoodCnt;
        TButton *Button1;
        void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall btnOKClick(TObject *Sender);
    void __fastcall pnLotMGZCnt1Click(TObject *Sender);
    void __fastcall pnBadLimCntClick(TObject *Sender);
    void __fastcall pnColor1Click(TObject *Sender);
        void __fastcall btVb4SelClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall cbMrk1SkipClick(TObject *Sender);
        void __fastcall btWK1ClearClick(TObject *Sender);
        void __fastcall btVS1ResetClick(TObject *Sender);
        void __fastcall btWk1InputClick(TObject *Sender);
        void __fastcall lbErrClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall pnDayWorkChipDblClick(TObject *Sender);
        void __fastcall pnDayFailChipDblClick(TObject *Sender);
        void __fastcall cbIdleRunClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateCmmOptn(bool bTable);
public:		// User declarations
    __fastcall TFormOperation(TComponent* Owner);
    void __fastcall LbResultChange();
    bool __fastcall CheckStripDisappear();    
    bool __fastcall CheckStripUnknwon  ();
    bool GetLastStrip();    
};
//---------------------------------------------------------------------------
extern PACKAGE TFormOperation *FormOperation;
//---------------------------------------------------------------------------
#endif

