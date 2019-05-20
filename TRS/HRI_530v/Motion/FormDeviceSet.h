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
        TPanel *Panel2;
        TLabel *Label6;
        TPanel *Panel3;
        TEdit *edOSTDeviceNo;
        TGroupBox *GroupBox17;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label15;
        TLabel *Label16;
        TLabel *Label17;
        TLabel *Label18;
        TEdit *edIDD_AVDDOkMin;
        TEdit *edIDD_CVDDOkMin;
        TLabel *Label19;
        TLabel *Label20;
        TEdit *edIDD_AVDDOkMax;
        TEdit *edIDD_CVDDOkMax;
        TLabel *Label21;
        TLabel *Label22;
        TEdit *edIDD_HVDDOkMin;
        TEdit *edIDD_DVDDOkMin;
        TLabel *Label23;
        TLabel *Label24;
        TEdit *edIDD_HVDDOkMax;
        TEdit *edIDD_DVDDOkMax;
        TGroupBox *GroupBox18;
        TLabel *Label25;
        TLabel *Label26;
        TLabel *Label27;
        TLabel *Label29;
        TLabel *Label30;
        TLabel *Label31;
        TLabel *Label32;
        TLabel *Label33;
        TLabel *Label34;
        TLabel *Label35;
        TLabel *Label36;
        TLabel *Label37;
        TLabel *Label38;
        TLabel *Label39;
        TEdit *edIDS_AVDDOkMin;
        TEdit *edIDS_CVDDOkMin;
        TEdit *edIDS_AVDDOkMax;
        TEdit *edIDS_CVDDOkMax;
        TEdit *edIDS_HVDDOkMin;
        TEdit *edIDS_DVDDOkMin;
        TEdit *edIDS_HVDDOkMax;
        TEdit *edIDS_DVDDOkMax;
        TLabel *Label7;
        TGroupBox *GroupBox19;
        TButton *Button6;
        TButton *Button7;
        TButton *Button26;
        TButton *Button27;
        TButton *Button28;
        TButton *Button29;
        TButton *Button30;
        TButton *Button31;
        TButton *Button32;
        TButton *Button33;
        TButton *Button34;
        TButton *Button35;
        TButton *Button36;
        TButton *Button37;
        TButton *Button38;
        TButton *Button39;
        TBitBtn *btMan3_5;
        TBitBtn *btMan3_6;
        TBitBtn *btMan3_7;
        TBitBtn *btMan3_8;
        TBitBtn *btMan3_9;
        TBitBtn *btMan3_10;
        TBitBtn *btMan3_11;
        TLabel *Label8;
        TLabel *Label10;
        TEdit *edOST_I2cFailTime;
        TPanel *Panel4;
        TPanel *Panel5;
        TCheckBox *cbVTPwrAllwaysOn;
        TStringGrid *sgOSTPinName;
        TLabel *Label40;
        TLabel *Label41;
        TLabel *Label42;
        TLabel *Label43;
        TPageControl *PageControl5;
        TTabSheet *tsStg1;
        TLabel *Label44;
        TLabel *Label45;
        TLabel *Label46;
        TLabel *Label47;
        TLabel *Label48;
        TLabel *Label49;
        TLabel *Label50;
        TLabel *Label51;
        TEdit *edAVDD_IDDGain0;
        TEdit *edAVDD_IDDOfst0;
        TEdit *edAVDD_IDSGain0;
        TEdit *edAVDD_IDSOfst0;
        TEdit *edCVDD_IDDGain0;
        TEdit *edCVDD_IDDOfst0;
        TEdit *edCVDD_IDSGain0;
        TEdit *edCVDD_IDSOfst0;
        TEdit *edHVDD_IDDGain0;
        TEdit *edHVDD_IDDOfst0;
        TEdit *edHVDD_IDSGain0;
        TEdit *edHVDD_IDSOfst0;
        TEdit *edDVDD_IDDGain0;
        TEdit *edDVDD_IDDOfst0;
        TEdit *edDVDD_IDSGain0;
        TEdit *edDVDD_IDSOfst0;
        TTabSheet *TabSheet5;
        TLabel *Label52;
        TLabel *Label53;
        TLabel *Label54;
        TLabel *Label55;
        TLabel *Label56;
        TLabel *Label57;
        TLabel *Label58;
        TLabel *Label59;
        TEdit *edAVDD_IDDGain1;
        TEdit *edAVDD_IDDOfst1;
        TEdit *edAVDD_IDSGain1;
        TEdit *edAVDD_IDSOfst1;
        TEdit *edCVDD_IDDGain1;
        TEdit *edCVDD_IDDOfst1;
        TEdit *edCVDD_IDSGain1;
        TEdit *edCVDD_IDSOfst1;
        TEdit *edHVDD_IDDGain1;
        TEdit *edHVDD_IDDOfst1;
        TEdit *edHVDD_IDSGain1;
        TEdit *edHVDD_IDSOfst1;
        TEdit *edDVDD_IDDGain1;
        TEdit *edDVDD_IDDOfst1;
        TEdit *edDVDD_IDSGain1;
        TEdit *edDVDD_IDSOfst1;
        TTabSheet *TabSheet6;
        TLabel *Label60;
        TLabel *Label61;
        TLabel *Label62;
        TLabel *Label63;
        TLabel *Label64;
        TLabel *Label65;
        TLabel *Label66;
        TLabel *Label67;
        TEdit *edAVDD_IDDGain2;
        TEdit *edAVDD_IDDOfst2;
        TEdit *edAVDD_IDSGain2;
        TEdit *edAVDD_IDSOfst2;
        TEdit *edCVDD_IDDGain2;
        TEdit *edCVDD_IDDOfst2;
        TEdit *edCVDD_IDSGain2;
        TEdit *edCVDD_IDSOfst2;
        TEdit *edHVDD_IDDGain2;
        TEdit *edHVDD_IDDOfst2;
        TEdit *edHVDD_IDSGain2;
        TEdit *edHVDD_IDSOfst2;
        TEdit *edDVDD_IDDGain2;
        TEdit *edDVDD_IDDOfst2;
        TEdit *edDVDD_IDSGain2;
        TEdit *edDVDD_IDSOfst2;
        TTabSheet *TabSheet7;
        TLabel *Label68;
        TLabel *Label69;
        TLabel *Label70;
        TLabel *Label71;
        TLabel *Label72;
        TLabel *Label73;
        TLabel *Label74;
        TLabel *Label75;
        TEdit *edAVDD_IDDGain3;
        TEdit *edAVDD_IDDOfst3;
        TEdit *edAVDD_IDSGain3;
        TEdit *edAVDD_IDSOfst3;
        TEdit *edCVDD_IDDGain3;
        TEdit *edCVDD_IDDOfst3;
        TEdit *edCVDD_IDSGain3;
        TEdit *edCVDD_IDSOfst3;
        TEdit *edHVDD_IDDGain3;
        TEdit *edHVDD_IDDOfst3;
        TEdit *edHVDD_IDSGain3;
        TEdit *edHVDD_IDSOfst3;
        TEdit *edDVDD_IDDGain3;
        TEdit *edDVDD_IDDOfst3;
        TEdit *edDVDD_IDSGain3;
        TEdit *edDVDD_IDSOfst3;
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
        void __fastcall btMan3_5Click(TObject *Sender);
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
