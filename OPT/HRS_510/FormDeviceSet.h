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
        TGroupBox *GroupBox1;
        TPanel *pnSRT;
        TTabSheet *TabSheet12;
        TPanel *Panel14;
        TPanel *Panel15;
        TImage *Image12;
        TLabel *Label57;
        TBitBtn *btSaveDevice;
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
        TLabel *Label15;
        TLabel *Label16;
    TPanel *Panel7;
    TBitBtn *btRAL_Out;
    TStaticText *StaticText45;
    TPanel *Panel35;
        TBitBtn *btPckVac1;
    TStaticText *lbVaccum;
    TPanel *Panel38;
        TBitBtn *btPckEjt1;
    TStaticText *lbEjector;
    TLabel *Label21;
    TLabel *Label22;
    TImage *Image1;
    TGroupBox *GroupBox18;
    TLabel *Label23;
    TLabel *Label24;
    TLabel *Label25;
    TLabel *Label26;
    TLabel *Label27;
    TLabel *Label33;
    TLabel *Label34;
    TLabel *Label37;
    TLabel *Label38;
    TLabel *Label39;
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
    TLabel *Label42;
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
    TLabel *Label32;
    TLabel *Label35;
    TLabel *Label36;
    TLabel *Label44;
    TLabel *Label48;
    TLabel *Label50;
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
        TPanel *Panel2;
        TBitBtn *btPckVac2;
        TStaticText *StaticText1;
        TPanel *Panel3;
        TBitBtn *btPckEjt2;
        TStaticText *StaticText2;
        TPanel *Panel4;
        TBitBtn *btPckVac3;
        TStaticText *StaticText3;
        TPanel *Panel5;
        TBitBtn *btPckEjt3;
        TStaticText *StaticText4;
        TPageControl *PageControl2;
        TTabSheet *TabSheet2;
        TTabSheet *TabSheet3;
        TGroupBox *GroupBox9;
        TGroupBox *GroupBox3;
        TPanel *pnLDR_SptrFB;
        TGroupBox *GroupBox2;
        TPanel *pnPCK_PckrDU;
        TGroupBox *GroupBox4;
        TPanel *pnRAL_IndxUD;
        TGroupBox *GroupBox7;
        TPanel *pnSRT_Grip1FB;
        TGroupBox *GroupBox8;
        TPanel *pnSRT_FlipFB;
        TGroupBox *GroupBox12;
        TPanel *pnULD_StckUD;
        TGroupBox *GroupBox17;
        TPanel *pnLDR_SplyFB;
        TGroupBox *GroupBox19;
        TPanel *pnSRT_Grip2FB;
        TGroupBox *GroupBox20;
        TPanel *pnSRT_Grip3FB;
        TGroupBox *GroupBox21;
        TPanel *pnSTN_GuidFB;
        TGroupBox *GroupBox23;
        TPanel *Panel6;
        TGroupBox *GroupBox25;
        TPanel *Panel8;
        TGroupBox *GroupBox26;
        TPanel *Panel9;
        TGroupBox *GroupBox27;
        TPanel *Panel10;
        TGroupBox *GroupBox28;
        TPanel *Panel11;
        TGroupBox *GroupBox29;
        TPanel *Panel12;
        TGroupBox *GroupBox30;
        TPanel *Panel13;
        TGroupBox *GroupBox31;
        TPanel *Panel16;
        TGroupBox *GroupBox32;
        TPanel *Panel17;
        TGroupBox *GroupBox33;
        TPanel *Panel18;
        TBitBtn *btLDR_SplyFB;
        TBitBtn *btLDR_SptrFB;
        TBitBtn *btPCK_PckrDU;
        TBitBtn *btRAL_IndxUD;
        TBitBtn *btSTN_GuidFB;
        TBitBtn *btSRT_FlipFB;
        TBitBtn *btULD_StckUD;
        TBitBtn *btSRT_Grip1FB;
        TBitBtn *btSRT_Grip2FB;
        TBitBtn *btSRT_Grip3FB;
        TPanel *Panel19;
        TBitBtn *btSTN_Air;
        TStaticText *StaticText5;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label14;
        TLabel *Label17;
        TLabel *Label40;
        TLabel *Label41;
        TEdit *edMinBn9;
        TEdit *edMaxBn9;
        TEdit *edMinBnA;
        TEdit *edMaxBnA;
        TGroupBox *GroupBox34;
        TLabel *Label18;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label31;
        TLabel *Label43;
        TLabel *Label45;
        TEdit *edMasterOffset1;
        TEdit *edMasterOffset2;
        TEdit *edMasterOffset3;
    void __fastcall btSavePosClick(TObject *Sender);
    void __fastcall tmUpdateTimerTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall pcDeviceSetChange(TObject *Sender);
        void __fastcall rgRALJogUnitClick(TObject *Sender);
    void __fastcall btRAL_OutClick(TObject *Sender);
        void __fastcall edRALJogUnitChange(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall btLDR_SplyFBClick(TObject *Sender);
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
