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
#include "UtilDefine.h"
#include "DataMan.h"
#include <Menus.hpp>
class TFormOperation : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TPanel *Panel2;
        TLabel *lbR0;
        TLabel *lbR1;
        TLabel *lbR2;
        TLabel *lbR3;
        TLabel *lbR4;
        TLabel *lbR5;
        TLabel *lbR6;
        TLabel *lbR7;
        TLabel *lbR8;
        TLabel *lbR9;
        TLabel *lbRA;
        TLabel *lbRB;
        TLabel *lbRC;
        TLabel *lbRD;
        TPanel *Panel8;
        TPanel *pnColor0;
        TPanel *pnColor1;
        TPanel *pnColor2;
        TPanel *pnColor3;
        TPanel *pnColor4;
        TPanel *pnColor5;
        TPanel *pnColor6;
        TPanel *pnColor7;
        TPanel *pnColor8;
        TPanel *pnColor9;
        TPanel *pnColorA;
        TPanel *pnColorB;
        TPanel *pnColorC;
        TPanel *pnColorD;
        TGroupBox *GroupBox5;
        TPanel *Panel3;
        TLabel *Label27;
        TLabel *Label28;
        TMemo *mmAction;
        TGroupBox *GroupBox8;
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
        TTimer *tmUpdate;
        TLabel *Label30;
        TEdit *edWk2Id;
        TLabel *Label31;
        TEdit *edWk3Id;
        TListBox *lbErr;
        TPanel *pnLotMGZCnt2;
    TLabel *Label35;
    TEdit *edMeasure;
    TBitBtn *btnOK;
        TTabSheet *tsV3R3;
        TTabSheet *tsV2R2;
        TPanel *pn2B2;
        TPanel *pn3B3;
        TStaticText *lbVs2FailCnt;
        TStaticText *lbVs1FailCnt;
        TStaticText *lbVs3FailCnt;
        TTabSheet *tsV1R1;
        TPanel *pn1B1;
        TGroupBox *GroupBox11;
        TPanel *pnPRB;
        TLabel *Label11;
        TLabel *Label12;
        TEdit *edPrbLotNo;
        TPanel *Panel4;
        TGroupBox *GroupBox2;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TLabel *Label13;
        TPanel *pnLotNo;
        TPanel *pnLotJobFile;
        TPanel *pnLotWorkChip;
        TPanel *pnLotFailChip;
        TPanel *pnBadLimCnt;
        TPanel *pnLotYeild;
        TPanel *pnLotGoodChip;
        TGroupBox *gbSkipInfo;
        TCheckBox *cbTrm1Skip;
        TCheckBox *cbTrmAllSkip;
        TCheckBox *cbLoadingStop;
        TCheckBox *cbDoorSkip;
        TCheckBox *cbIdleRun;
        TGroupBox *gbVsSkip;
        TCheckBox *cbVs1_1Skip;
        TCheckBox *cbVs2_1Skip;
        TCheckBox *cbVs3_1Skip;
        TGroupBox *GroupBox1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label14;
        TPanel *pnDayWorkChip;
        TPanel *pnDayFailChip;
        TPanel *pnTotalYield;
        TPanel *pnLotMGZCnt1;
        TPanel *pnTotalGoodCnt;
        TGroupBox *GroupBox10;
        TLabel *Label37;
        TLabel *Label34;
        TEdit *edWk4LotNo;
        TButton *btWk4Input;
        TEdit *edWk4Id;
        TCheckBox *cbVs1_2Skip;
        TCheckBox *cbVs2_2Skip;
        TCheckBox *cbVs3_2Skip;
        TCheckBox *cbCam2Skip;
        TLabel *lbRE;
        TLabel *lbRF;
        TLabel *lbRG;
        TLabel *lbRH;
        TLabel *lbRI;
        TLabel *lbRJ;
        TLabel *lbRK;
        TLabel *lbRL;
        TLabel *lbRM;
        TLabel *lbRN;
        TLabel *lbRO;
        TLabel *lbRP;
        TLabel *lbRQ;
        TLabel *lbRR;
        TPanel *Panel5;
        TPanel *pnColorF;
        TPanel *pnColorG;
        TPanel *pnColorH;
        TPanel *pnColorI;
        TPanel *pnColorJ;
        TPanel *pnColorK;
        TPanel *pnColorL;
        TPanel *pnColorM;
        TPanel *pnColorN;
        TPanel *pnColorO;
        TPanel *pnColorP;
        TPanel *pnColorQ;
        TPanel *pnColorR;
        TButton *btPRBClear;
        TLabel *Label17;
        TCheckBox *cbCam1Skip;
        TCheckBox *cbVisionAllSkip;
        TPanel *pnColorE;
        TStaticText *lbPSB_Row;
        TStaticText *lbPSB_Col;
        TStaticText *lbPSB_Status;
        TPanel *pnLDR;
        TPanel *pnULD;
        TEdit *edPrbId;
        TPageControl *pcVsR4;
        TTabSheet *tsV4Result;
        TPanel *pnPSB;
        TTabSheet *TabSheet5;
        TStaticText *lbTrmFailCnt;
        TButton *btPSBReset;
        TButton *btPSBClear;
        TCheckBox *cbVsTrmSkip;
        TButton *Button1;
        void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall btnOKClick(TObject *Sender);
    void __fastcall pnLotMGZCnt1Click(TObject *Sender);
    void __fastcall pnBadLimCntClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall cbTrm1SkipClick(TObject *Sender);
        void __fastcall btWK1ClearClick(TObject *Sender);
        void __fastcall btVS1ResetClick(TObject *Sender);
        void __fastcall btWk1InputClick(TObject *Sender);
        void __fastcall lbErrClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall pnDayWorkChipDblClick(TObject *Sender);
        void __fastcall pnDayFailChipDblClick(TObject *Sender);
        void __fastcall cbIdleRunClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall cbVisionAllSkipClick(TObject *Sender);
        void __fastcall pnColor1Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);

private:	// User declarations
    void __fastcall UpdateCmmOptn(bool bTable);
    bool            CbChecked     (TCheckBox *cbBox);
    bool            CbChecked     (TCheckBox *cbBox,bool bChecked);
public:		// User declarations
    __fastcall TFormOperation(TComponent* Owner);
    void  __fastcall LbResultChange();
    bool  __fastcall CheckStripDisappear();
    bool  __fastcall CheckStripUnknwon  ();
    bool  GetLastStrip();
    bool  SetDispItem();
    bool  SetDispItem(EN_ARAY_ID _Aray);
    bool  SetDispItem(CArray *  riAray);
    String GetRsltName(int _iStat);



};
//---------------------------------------------------------------------------
extern PACKAGE TFormOperation *FormOperation;
//---------------------------------------------------------------------------
#endif

