//---------------------------------------------------------------------------

#ifndef FormDeviceSetH
#define FormDeviceSetH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Buttons.hpp>
#include <jpeg.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
//Motr Window Cylinder Window Frame Unit Refernce
//---------------------------------------------------------------------------
#include "FrameMotr.h"
#include "FrameCyl.h"
//---------------------------------------------------------------------------
#include "Timer.h"
#include <ImgList.hpp>
//#include "LabelS.h"
#include <ValEdit.hpp>
class TFrmDeviceSet : public TForm
{
__published:	// IDE-managed Components
    TTimer *tmUpdateTimer;
        TPageControl *pcDeviceSet;
        TTabSheet *tsDeviceInfo;
        TGroupBox *GroupBox10;
        TImage *Image2;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TStaticText *lbRowPitch;
        TEdit *edRowPitch;
        TStaticText *lbColPitch;
        TEdit *edColPitch;
        TStaticText *lbRowGroupCount;
        TEdit *edRowGroupCount;
        TStaticText *lbColCount;
        TEdit *edColCount;
        TStaticText *lbRowGroupGap;
        TEdit *edRowGroupGap;
        TStaticText *lbColGroupGap;
        TEdit *edColGroupGap;
        TStaticText *lbRowCount;
        TEdit *edRowCount;
        TStaticText *lbSrColCntrPitch;
        TEdit *edColGroupCount;
        TGroupBox *GroupBox11;
        TImage *Image3;
        TStaticText *StaticText8;
        TEdit *edCsSlotCnt;
        TStaticText *StaticText10;
        TEdit *edCsSlotPitch;
        TTabSheet *tsPosition;
        TGroupBox *GroupBox5;
        TPanel *pnLDR;
        TGroupBox *GroupBox6;
        TPanel *pnPCK;
        TGroupBox *GroupBox24;
        TPanel *pnRAL;
        TPanel *Panel1;
        TRadioGroup *rgRALJogUnit;
        TEdit *edRALJogUnit;
        TGroupBox *GroupBox9;
        TGroupBox *GroupBox1;
        TPanel *pnSRT;
        TTabSheet *TabSheet12;
        TPanel *Panel14;
        TPanel *Panel15;
        TImage *Image12;
        TLabel *Label57;
        TBitBtn *btSaveDevice;
        TGroupBox *GroupBox3;
        TPanel *pnLDR_Sptr;
        TGroupBox *GroupBox2;
        TPanel *pnPCK_UpDn;
        TGroupBox *GroupBox4;
        TPanel *pnRAL_Indx;
        TGroupBox *GroupBox7;
        TPanel *pnSRT_Grip;
        TGroupBox *GroupBox8;
        TPanel *pnSRT_Flip;
        TGroupBox *GroupBox12;
        TPanel *pnULD_Stck;
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TGroupBox *GroupBox22;
        TPanel *pnLDR_Z;
        TGroupBox *GroupBox14;
        TPanel *pnRAL_X;
        TGroupBox *GroupBox13;
        TPanel *pnPCK_Y;
        TGroupBox *GroupBox15;
        TPanel *pnSRT_X;
        TGroupBox *GroupBox16;
        TLabel *Label49;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label5;
        TLabel *Label15;
        TLabel *Label16;
        TGroupBox *GroupBox17;
        TLabel *Label17;
        TEdit *edBinMaxCnt;
        TLabel *Label18;
    TPanel *Panel7;
    TBitBtn *btRAL_Out;
    TStaticText *StaticText45;
    TPanel *Panel35;
    TBitBtn *BtVaccumIn;
    TStaticText *lbVaccum;
    TPanel *Panel38;
    TBitBtn *btVaccumOut;
    TStaticText *lbEjector;
    TLabel *Label21;
    TLabel *Label22;
    TImage *Image1;
    TPanel *Panel2;
    TBitBtn *BitBtn1;
    TStaticText *StaticText1;
    TPanel *Panel3;
    TBitBtn *BitBtn2;
    TStaticText *StaticText2;
    TGroupBox *GroupBox18;
    TLabel *Label3;
    TLabel *Label4;
    TEdit *edMaxBn1S;
    TEdit *edMinBn1S;
    TEdit *edMaxBn2S;
    TEdit *edMinBn2S;
    TEdit *edMaxBn3S;
    TEdit *edMinBn3S;
    TEdit *edMaxBn4S;
    TEdit *edMinBn4S;
    TEdit *edMaxBn5S;
    TEdit *edMinBn5S;
    TLabel *Label23;
    TLabel *Label24;
    TLabel *Label25;
    TLabel *Label26;
    TLabel *Label27;
    TLabel *Label33;
    TLabel *Label34;
    TLabel *Label37;
    TLabel *Label38;
    TEdit *edMaxBn6S;
    TEdit *edMinBn6S;
    TEdit *edMaxBn7S;
    TEdit *edMinBn7S;
    TLabel *Label39;
    TLabel *Label40;
    TLabel *Label41;
    TEdit *edMaxBn8S;
    TEdit *edMinBn8S;
    TEdit *edMaxBn9S;
    TEdit *edMinBn9S;
    TEdit *edMaxBnAS;
    TEdit *edMinBnAS;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *edMinBn1;
    TEdit *edMaxBn1;
    TEdit *edMinBn2;
    TEdit *edMaxBn2;
    TEdit *edMinBn3;
    TEdit *edMaxBn3;
    TEdit *edMinBn4;
    TEdit *edMaxBn4;
    TEdit *edMinBn5;
    TEdit *edMaxBn5;
    TEdit *edMinBn6;
    TEdit *edMaxBn6;
    TEdit *edMinBn7;
    TEdit *edMaxBn7;
    TEdit *edMinBn8;
    TEdit *edMaxBn8;
    TEdit *edMinBn9;
    TEdit *edMaxBn9;
    TEdit *edMinBnA;
    TEdit *edMaxBnA;
    TLabel *Label42;
    TLabel *Label43;
    TLabel *Label28;
    TLabel *Label29;
    TLabel *Label30;
    TComboBox *cbBnL1Sel;
    TComboBox *cbBnL2Sel;
    TComboBox *cbBnL3Sel;
    TComboBox *cbBnL4Sel;
    TComboBox *cbBnL5Sel;
    TComboBox *cbBnL6Sel;
    TComboBox *cbBnL7Sel;
    TComboBox *cbBnL8Sel;
    TComboBox *cbBnL9Sel;
    TComboBox *cbBnLASel;
    TLabel *Label14;
    TLabel *Label19;
    TLabel *Label20;
    TLabel *Label31;
    TLabel *Label32;
    TLabel *Label35;
    TLabel *Label36;
    TLabel *Label44;
    TLabel *Label45;
    TLabel *Label46;
    TLabel *Label47;
    TLabel *Label48;
    TLabel *Label50;
    TLabel *Label51;
    TLabel *Label52;
    TGroupBox *GroupBox19;
    TLabel *Label53;
    TLabel *Label54;
    TLabel *Label55;
    TLabel *Label56;
    TLabel *Label58;
    TLabel *Label59;
    TLabel *Label60;
    TLabel *Label61;
    TLabel *Label62;
    TLabel *Label63;
    TLabel *Label64;
    TLabel *Label65;
    TLabel *Label66;
    TLabel *Label67;
    TLabel *Label68;
    TLabel *Label69;
    TLabel *Label70;
    TLabel *Label71;
    TLabel *Label72;
    TLabel *Label73;
    TLabel *Label74;
    TLabel *Label75;
    TLabel *Label76;
    TLabel *Label77;
    TLabel *Label78;
    TLabel *Label79;
    TLabel *Label80;
    TLabel *Label81;
    TLabel *Label82;
    TLabel *Label83;
    TComboBox *cbBnR1Sel;
    TComboBox *cbBnR2Sel;
    TComboBox *cbBnR3Sel;
    TComboBox *cbBnR4Sel;
    TComboBox *cbBnR5Sel;
    TComboBox *cbBnR6Sel;
    TComboBox *cbBnR7Sel;
    TComboBox *cbBnR8Sel;
    TComboBox *cbBnR9Sel;
    TComboBox *cbBnRASel;
    TComboBox *cbBBnL1Sel;
    TComboBox *cbBBnL2Sel;
    TComboBox *cbBBnL3Sel;
    TComboBox *cbBBnL4Sel;
    TComboBox *cbBBnL5Sel;
    TComboBox *cbBBnL6Sel;
    TComboBox *cbBBnL7Sel;
    TComboBox *cbBBnL8Sel;
    TComboBox *cbBBnL9Sel;
    TComboBox *cbBBnLASel;
    TComboBox *cbBBnR1Sel;
    TComboBox *cbBBnR2Sel;
    TComboBox *cbBBnR3Sel;
    TComboBox *cbBBnR4Sel;
    TComboBox *cbBBnR5Sel;
    TComboBox *cbBBnR6Sel;
    TComboBox *cbBBnR7Sel;
    TComboBox *cbBBnR8Sel;
    TComboBox *cbBBnR9Sel;
    TComboBox *cbBBnRASel;
    TPanel *pnLBCnt0;
    TLabel *Label84;
    TPanel *pnLBCnt1;
    TPanel *pnLBCnt2;
    TPanel *pnLBCnt3;
    TPanel *pnLBCnt4;
    TPanel *pnLBCnt5;
    TPanel *pnLBCnt6;
    TPanel *pnLBCnt7;
    TPanel *pnLBCnt8;
    TPanel *pnLBCnt9;
    TPanel *pnRBCnt0;
    TLabel *Label85;
    TPanel *pnRBCnt1;
    TPanel *pnRBCnt2;
    TPanel *pnRBCnt3;
    TPanel *pnRBCnt4;
    TPanel *pnRBCnt5;
    TPanel *pnRBCnt6;
    TPanel *pnRBCnt7;
    TPanel *pnRBCnt8;
    TPanel *pnRBCnt9;
    void __fastcall btSavePosClick(TObject *Sender);
    void __fastcall tmUpdateTimerTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall pcDeviceSetChange(TObject *Sender);
        void __fastcall rgRALJogUnitClick(TObject *Sender);
    void __fastcall btRAL_OutClick(TObject *Sender);
        void __fastcall edRALJogUnitChange(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
private:	// User declarations
    //Active Radio Button Page.
    TRadioGroup  * rgUnitUser ;
    TEdit        * edUnitUser ;
    CDelayTimer    m_tmProbeTest ;
public:		// User declarations
    TFraMotr *FraMotor   [MAX_MOTR];
    TFraCyl  *FraCylinder[MAX_ACTR];

    __fastcall TFrmDeviceSet(TComponent* Owner);
    void __fastcall UpdateDevInfo(bool bTo);
    void __fastcall UpdateDevOptn(bool bTo);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
#endif
