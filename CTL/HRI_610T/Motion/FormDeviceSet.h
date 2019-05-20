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
#include "FrameMotr.h"
#include "FrameCylOneBt.h"
#include "FrameOutput.h"
#include "Chip.h"
//#include "TEdit.h"  시발 폼디바이스 컴파일 안됌.... 컴포넌트로 제작 해야 할듯...
#include <ImgList.hpp>
class TFrmDeviceSet : public TForm
{
__published:	// IDE-managed Components
    TTimer *tmUpdateTimer;
        TImageList *ImageList1;
        TPageControl *pcDeviceSet;
        TTabSheet *tsDeviceInfo;
        TGroupBox *GroupBox10;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TStaticText *lbRowPitch;
        TEdit *edRowPitch;
        TStaticText *lbColPitch;
        TEdit *edColPitch;
        TStaticText *lbRowGroupCount;
        TEdit *edRowGrCnt;
        TStaticText *lbColCount;
        TEdit *edColCnt;
        TStaticText *lbRowGroupGap;
        TEdit *edRowGrGap;
        TStaticText *lbColGroupGap;
        TEdit *edColGrGap;
        TStaticText *lbRowCount;
        TEdit *edRowCnt;
        TStaticText *lbSrColCntrPitch;
        TEdit *edColGrCnt;
        TGroupBox *GroupBox11;
        TImage *Image3;
        TStaticText *StaticText8;
        TEdit *edCsSlotCnt;
        TStaticText *StaticText10;
        TEdit *edCsSlotPitch;
        TTabSheet *tsLoader;
        TImage *Image7;
        TGroupBox *gbMotrPos18;
        TPanel *pnMotrPos9;
        TGroupBox *gbMotrPos17;
        TPanel *pnMotrPos8;
        TPanel *pnJog1;
        TPageControl *PageControl3;
        TTabSheet *TabSheet8;
        TGroupBox *gbMotrJog17;
        TPanel *pnMotrJog8;
        TGroupBox *gbMotrJog08;
        TPanel *pnMotrJog9;
        TPanel *pnJogUnit;
        TRadioGroup *rgJogUnit;
        TEdit *edJogUnit;
        TPanel *pnLDR1;
        TPanel *pnLDR2;
        TPanel *pnAtcr13;
        TPanel *pnAtcr12;
        TPanel *pnMotrPos7;
        TTabSheet *tsPrebuffer;
        TGroupBox *gbMotrPos01;
        TPanel *pnMotrPos1;
        TPanel *pnJog2;
        TPageControl *PageControl1;
        TTabSheet *TabSheet2;
        TGroupBox *gbMotrJog01;
        TPanel *pnMotrJog1;
        TPanel *pnPRB1;
        TPanel *pnAtcr0;
        TPanel *pnPRB2;
        TTabSheet *tsRail;
        TGroupBox *gbMotrPos10;
        TPanel *pnMotrPos6;
        TPanel *pnProbeMotr;
        TPanel *pnJog3;
        TPageControl *PageControl2;
        TTabSheet *TabSheet6;
        TGroupBox *gbMotrJog11;
        TPanel *pnMotrJog15;
        TGroupBox *gbMotrJog16;
        TPanel *pnMotrJog16;
        TGroupBox *gbMotrJog13;
        TPanel *pnMotrJog13;
        TTabSheet *TabSheet7;
        TGroupBox *gbMotrJog05;
        TPanel *pnMotrJog5;
        TGroupBox *gbMotrJog18;
        TPanel *pnMotrJog6;
        TGroupBox *gbMotrPos11;
        TPanel *pnMotrPos5;
        TPanel *pnAtcr2;
        TPanel *pnAtcr4;
        TPanel *pnAtcr3;
        TPanel *pnAtcr1;
        TPanel *pnAtcr6;
        TPanel *pnAtcr5;
        TPanel *pnRAL1;
        TTabSheet *tsLaser;
        TImage *Image14;
        TPanel *pnJog4;
        TPageControl *PageControl5;
        TTabSheet *TabSheet11;
        TGroupBox *GroupBox1;
        TPanel *pnMotrJog0;
        TPanel *Panel39;
        TBitBtn *btPSB_SrtTFw;
        TBitBtn *btPSB_SrtTBw;
        TStaticText *StaticText55;
        TPanel *Panel40;
        TBitBtn *btPSB_SrtBFw;
        TBitBtn *btPSB_SrtBBw;
        TStaticText *StaticText58;
        TGroupBox *gbMotrPos15;
        TPanel *pnMotrPos0;
        TTabSheet *tsVision;
        TPanel *pnJog5;
        TPageControl *PageControl6;
        TTabSheet *TabSheet12;
        TGroupBox *gbMotrJog03;
        TPanel *pnMotrJog3;
        TGroupBox *gbMotrJog02;
        TPanel *pnMotrJog2;
        TGroupBox *gbMotrPos04;
        TPanel *pnMotrPos2;
        TGroupBox *gbMotrPos05;
        TPanel *pnMotrPos3;
        TTabSheet *tsPostBuffer;
        TPanel *pnJog6;
        TPageControl *PageControl12;
        TTabSheet *TabSheet13;
        TGroupBox *gbMotrJog04;
        TPanel *pnMotrJog4;
        TGroupBox *gbMotrPos09;
        TPanel *pnMotrPos4;
        TPanel *pnAtcr99;
        TPanel *pnAtcr11;
        TPanel *pnAtcr9;
        TPanel *pnAtcr10;
        TPanel *pnAtcr8;
        TPanel *pnAtcr7;
        TPanel *pnPSB1;
        TTabSheet *tsUnLoader;
        TImage *Image9;
        TGroupBox *gbMotrPos19;
        TPanel *pnMotrPos10;
        TGroupBox *gbMotrPos20;
        TPanel *pnMotrPos11;
        TPanel *pnJog7;
        TPageControl *PageControl4;
        TTabSheet *TabSheet10;
        TGroupBox *gbMotrJog09;
        TPanel *pnMotrJog10;
        TGroupBox *gbMotrJog10;
        TPanel *pnMotrJog11;
        TPanel *pnULD1;
        TPanel *pnULD2;
        TPanel *pnAtcr14;
        TTabSheet *tsOption;
        TPanel *Panel9;
        TLabel *Label9;
        TLabel *Label27;
        TLabel *Label28;
        TLabel *Label5;
        TLabel *Label8;
        TPanel *Panel12;
        TEdit *edLotEndMgzCnt;
        TPanel *Panel1;
        TLabel *Label15;
        TLabel *Label16;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *Label19;
        TLabel *Label20;
        TPanel *Panel2;
        TEdit *edRailStprDnDelay;
        TEdit *edPSBGripDelay;
        TPanel *Panel3;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TPanel *Panel4;
        TEdit *edMarkInspFailCnt;
        TPanel *Panel5;
        TLabel *Label6;
        TLabel *Label14;
        TLabel *Label21;
        TLabel *Label4;
        TLabel *Label7;
        TLabel *Label22;
        TLabel *Label23;
        TLabel *Label24;
        TPanel *Panel6;
        TEdit *edLaserLotFormat;
        TEdit *edLotNo;
        TEdit *edLaser;
        TButton *btTestLaserText;
        TPanel *pnMotrJog7;
        TBitBtn *btSaveDevice;
    TGroupBox *GroupBox2;
    TBitBtn *btPitchY_N;
    TBitBtn *btPitchX_P;
    TBitBtn *btPitchY_P;
    TBitBtn *btPitchX_N;
    TPanel *Panel7;
    TPanel *Panel8;
    TPanel *Panel10;
    TPanel *Panel11;
    TPanel *Panel13;
    TLabel *lbColGrCnt;
    TLabel *lbRowGrCnt;
    TLabel *lbColCnt;
    TLabel *lbRowCnt;
    TImage *imArray;
    TImage *Image2;
    TStaticText *StaticText1;
        TGroupBox *GroupBox3;
        TLabel *Label25;
        TEdit *edEmptyStrpCnt;
        TLabel *Label26;
        TCheckBox *cbEmptyStrpPlace;
        TPanel *Panel14;
        TLabel *Label29;
        TPanel *Panel15;
        TEdit *edHoleXCntrOfs;
        TLabel *Label31;
        TLabel *Label30;
        TEdit *edHoleYCntrOfs;
        TLabel *Label32;
        TLabel *Label33;
        TEdit *edHoleTCntrOfs;
        TLabel *Label34;
        TBitBtn *btOfsHelp;
        TBitBtn *btFormatHelp;
        TLabel *Label35;
        TLabel *Label36;
        TLabel *Label37;
        TLabel *Label38;
        TLabel *Label39;
        TLabel *Label40;
        TEdit *edVSNXLSRCntrOfs;
        TEdit *edVSNYLSRCntrOfs;
        TEdit *edVSNTLSRCntrOfs;
        TCheckBox *cbLSRTest;
        TLabel *Label41;
        TEdit *edOriVsnDelay;
        TLabel *Label42;
    void __fastcall btSavePosClick(TObject *Sender);
    void __fastcall tmUpdateTimerTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall pcDeviceSetChange(TObject *Sender);
        void __fastcall rgJogUnitClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
    void __fastcall btTestLaserTextClick(TObject *Sender);
        void __fastcall btOfsHelpClick(TObject *Sender);
        void __fastcall btFormatHelpClick(TObject *Sender);
private:	// User declarations
    //Active Radio Button Page.
    //TRadioGroup  * rgUnitUser ;
    //TEdit        * edUnitUser ;
public:		// User declarations
    TFraMotr      *FraMotor   [MAX_MOTR];
    TFraCylOneBt  *FraCylinder[MAX_ACTR];

    TFraOutput    *foLDRMgzInAC   ;
    TFraOutput    *foLDRMgzOutAC  ;
    TFraOutput    *foULDMgzInAC   ;
    TFraOutput    *foULDMgzOutAC  ;
    TFraOutput    *foPRBFeedAC    ;
    TFraOutput    *foRALFeedAC    ;
    TFraOutput    *foPRBAirBlow   ;
    TFraOutput    *foPSBRjcBelt   ;

    __fastcall TFrmDeviceSet(TComponent* Owner);
    void __fastcall UpdateDevInfo(bool bTo);
    void __fastcall UpdateDevOptn(bool bTo);
    void __fastcall PaintStrpInfo();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
#endif
