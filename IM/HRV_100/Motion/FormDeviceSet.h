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
        TStaticText *lbRowPitch;
        TEdit *edLDRRowPitch;
        TStaticText *lbColPitch;
        TEdit *edLDRColPitch;
        TStaticText *lbColCount;
        TEdit *edLDRColCnt;
        TStaticText *lbRowCount;
        TEdit *edLDRRowCnt;
        TTabSheet *tsLoader;
        TGroupBox *gbMotrPos18;
        TPanel *pnMotrPos1;
        TGroupBox *gbMotrPos17;
        TPanel *pnMotrPos0;
        TPanel *pnJog1;
        TPageControl *PageControl3;
        TTabSheet *TabSheet8;
        TGroupBox *gbMotrJog1;
        TGroupBox *gbMotrJog08;
        TPanel *pnMotrJog0;
        TPanel *pnJogUnit;
        TPanel *pnAtcr1;
        TPanel *pnAtcr0;
        TPanel *pnMotrPos99;
        TTabSheet *tsPrebuffer;
        TPanel *pnJog2;
        TPageControl *PageControl1;
        TTabSheet *TabSheet2;
        TGroupBox *gbMotrJog01;
        TPanel *pnMotrJog111;
        TTabSheet *tsRail;
        TGroupBox *gbMotrPos10;
        TPanel *pnMotrPos6;
        TPanel *pnProbeMotr;
        TPanel *pnJog3;
        TGroupBox *gbMotrPos11;
        TPanel *pnMotrPos5;
        TTabSheet *tsUnLoader;
        TImage *Image9;
        TGroupBox *gbMotrPos19;
        TPanel *pnMotrPos8;
        TGroupBox *gbMotrPos20;
        TPanel *pnMotrPos9;
        TPanel *pnJog4;
        TPageControl *PageControl4;
        TTabSheet *TabSheet10;
        TGroupBox *gbMotrJog09;
        TPanel *pnMotrJog9;
        TGroupBox *gbMotrJog10;
        TPanel *pnMotrJog8;
        TPanel *pnAtcr13;
        TTabSheet *tsOption;
        TPanel *Panel9;
        TLabel *Label9;
        TLabel *Label28;
        TPanel *Panel12;
        TEdit *edPickDealy;
        TPanel *pnMotrJog99;
        TBitBtn *btSaveDevice;
        TImage *imArray1;
        TGroupBox *GroupBox1;
        TStaticText *StaticText8;
        TEdit *edScktPitch;
        TLabel *Label1;
        TEdit *edPlceDelay;
        TLabel *Label2;
        TGroupBox *GroupBox6;
        TGroupBox *GroupBox11;
        TGroupBox *GroupBox7;
        TPanel *pnMotrJog2;
        TGroupBox *GroupBox8;
        TPanel *pnMotrJog3;
        TGroupBox *GroupBox9;
        TPanel *pnMotrJog7;
        TPanel *pnMotrJog1;
        TPanel *pnMotrJog4;
        TPageControl *PageControl2;
        TTabSheet *TabSheet1;
        TGroupBox *GroupBox12;
        TPanel *Panel1;
        TGroupBox *GroupBox13;
        TGroupBox *GroupBox14;
        TPanel *pnMotrJog5;
        TGroupBox *GroupBox15;
        TPanel *pnMotrJog6;
        TPanel *pnAtcr2;
        TPanel *pnAtcr14;
        TPanel *pnAtcr7;
        TPanel *pnAtcr8;
        TPanel *pnAtcr9;
        TPanel *pnAtcr10;
        TRadioGroup *rgJogUnit;
        TEdit *edJogUnit;
        TLabel *Loader;
        TImage *imArray2;
        TLabel *Label3;
        TLabel *UnLoader;
        TImage *Image1;
        TImage *Image2;
        TImage *Image3;
        TStaticText *StaticText3;
        TEdit *edULDColPitch;
        TEdit *edULDColCnt;
        TStaticText *StaticText2;
        TStaticText *StaticText1;
        TStaticText *StaticText4;
        TEdit *edULDRowCnt;
        TEdit *edULDRowPitch;
        TPanel *pnLDR1;
        TPanel *pnAtcr12;
        TPageControl *pcSorter;
        TTabSheet *TabSheet3;
        TTabSheet *TabSheet4;
        TGroupBox *GroupBox2;
        TPanel *pnMotrPos4;
        TGroupBox *GroupBox3;
        TPanel *pnMotrPos2;
        TGroupBox *GroupBox4;
        TPanel *pnMotrPos3;
        TGroupBox *GroupBox5;
        TPanel *pnMotrPos7;
        TPanel *pnSRT1;
        TPanel *pnSRT2;
        TPanel *pnSRT5;
        TPanel *pnSRT6;
        TPanel *pnSRT8;
        TPanel *pnSRT7;
        TPanel *pnSRT4;
        TPanel *pnSRT3;
        TPanel *pnSRT9;
        TPanel *pnSRT10;
        TPanel *pnAtcr11;
        TPanel *pnAtcr3;
        TPanel *pnAtcr4;
        TPanel *pnAtcr5;
        TPanel *pnAtcr6;
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TButton *Button4;
        TButton *Button5;
        TButton *Button8;
        TButton *Button9;
        TLabel *Label4;
        TGroupBox *GroupBox16;
        TButton *Button10;
        TButton *Button11;
        TButton *Button12;
        TButton *Button13;
        TButton *Button14;
        TButton *Button15;
        TButton *Button16;
        TButton *Button17;
        TButton *Button18;
        TButton *Button19;
        TButton *Button20;
        TButton *Button21;
        TButton *Button22;
        TButton *Button23;
        TButton *Button24;
        TButton *Button25;
        TLabel *Label5;
    void __fastcall btSavePosClick(TObject *Sender);
    void __fastcall tmUpdateTimerTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall pcDeviceSetChange(TObject *Sender);
        void __fastcall rgJogUnitClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall edJogUnitChange(TObject *Sender);
        void __fastcall Button10Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
    //Active Radio Button Page.
    //TRadioGroup  * rgUnitUser ;
    //TEdit        * edUnitUser ;
public:		// User declarations
    TFraMotr      *FraMotor   [MAX_MOTR];
    TFraCylOneBt  *FraCylinder[MAX_ACTR];

    TFraOutput    *foLDRAirBlow   ;

    TFraOutput    *foSRTVcc1    ;
    TFraOutput    *foSRTEjct1   ;
    TFraOutput    *foSRTVcc2    ;
    TFraOutput    *foSRTEjct2   ;
    TFraOutput    *foSRTVcc3    ;
    TFraOutput    *foSRTEjct3   ;
    TFraOutput    *foSRTVcc4    ;
    TFraOutput    *foSRTEjct4   ;
    TFraOutput    *foSRTVcc     ;
    TFraOutput    *foSRTAlgnAir ;

    __fastcall TFrmDeviceSet(TComponent* Owner);
    void __fastcall UpdateDevInfo(bool bTo);
    void __fastcall UpdateDevOptn(bool bTo);
    void __fastcall PaintStrpInfo();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
#endif
