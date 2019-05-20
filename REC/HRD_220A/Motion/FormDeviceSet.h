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
#include <Dialogs.hpp>

class TFrmDeviceSet : public TForm
{
__published:	// IDE-managed Components
    TTimer *tmUpdateTimer;
        TImageList *ImageList1;
        TPageControl *pcDeviceSet;
        TTabSheet *tsDeviceInfo;
        TTabSheet *tsRear;
        TTabSheet *tsDispensor;
        TPanel *pnJog3;
        TTabSheet *tsPICKER;
        TPanel *pnJog2;
        TTabSheet *tsOption;
        TPanel *Panel9;
        TPanel *Panel12;
        TBitBtn *btSaveDevice;
        TPageControl *PageControl2;
        TTabSheet *TabSheet1;
        TGroupBox *GroupBox12;
        TPanel *Panel1;
        TGroupBox *GroupBox13;
        TGroupBox *GroupBox14;
        TPanel *pnMotrJog4;
        TGroupBox *GroupBox15;
        TPanel *pnMotrJog5;
        TPanel *pnAtcr2;
        TLabel *Label3;
        TTabSheet *tsPattern;
        TTabSheet *tsFront;
        TGroupBox *GroupBox4;
        TPanel *pnMotrPos4;
        TGroupBox *GroupBox5;
        TPanel *pnMotrPos5;
        TGroupBox *GroupBox7;
        TPanel *pnMotrPos10;
        TGroupBox *GroupBox3;
        TPanel *pnMotrJog10;
        TGroupBox *GroupBox6;
        TPanel *pnMotrPos6;
        TGroupBox *GroupBox8;
        TPanel *pnMotrPos7;
        TGroupBox *GroupBox9;
        TPanel *pnMotrPos14;
        TGroupBox *GroupBox11;
        TPanel *pnMotrPos15;
        TPageControl *PageControl1;
        TTabSheet *TabSheet2;
        TGroupBox *GroupBox35;
        TPanel *Panel7;
        TGroupBox *GroupBox36;
        TGroupBox *GroupBox37;
        TPanel *pnMotrJog6;
        TGroupBox *GroupBox38;
        TPanel *pnMotrJog7;
        TGroupBox *GroupBox39;
        TPanel *pnMotrJog14;
        TGroupBox *GroupBox40;
        TPanel *pnMotrJog15;
        TTabSheet *tsStage;
        TGroupBox *GroupBox43;
        TPanel *pnMotrPos11;
        TGroupBox *GroupBox44;
        TPanel *pnMotrPos12;
        TGroupBox *GroupBox45;
        TPanel *pnMotrPos13;
        TGroupBox *GroupBox46;
        TPanel *pnMotrPos18;
        TPanel *pnJog4;
        TPageControl *PageControl4;
        TTabSheet *TabSheet10;
        TGroupBox *GroupBox47;
        TPanel *Panel31;
        TGroupBox *GroupBox48;
        TGroupBox *GroupBox49;
        TPanel *pnMotrJog11;
        TGroupBox *GroupBox50;
        TPanel *pnMotrJog12;
        TGroupBox *GroupBox51;
        TPanel *pnMotrJog13;
        TGroupBox *GroupBox52;
        TPanel *pnMotrJog18;
        TPanel *pnAtcr1;
        TPanel *pnAtcr0;
        TPanel *pnIO4;
        TPanel *pnIO5;
        TPanel *pnIO6;
        TPanel *pnIO7;
        TLabel *Label84;
        TLabel *Label85;
        TEdit *edDisprSpeed;
        TPanel *Panel2;
        TPanel *Panel3;
        TCheckBox *cbPkIgnrAlign;
        TCheckBox *cbPkIgnrVisnAlign;
        TCheckBox *cbPkIgnrVisnDefect;
        TCheckBox *cbStIgnrVisnBefore;
        TCheckBox *cbStIgnrVisnAfter;
        TGroupBox *GroupBox22;
        TPanel *pnMotrPos0;
        TPanel *pnJog0;
        TPageControl *PageControl7;
        TTabSheet *TabSheet13;
        TGroupBox *GroupBox16;
        TPanel *pnMotrJog0;
        TGroupBox *GroupBox32;
        TPanel *pnMotrPos2;
        TGroupBox *GroupBox30;
        TPanel *pnMotrPos8;
        TPanel *pnAtcr3;
        TPanel *pnAtcr5;
        TGroupBox *GroupBox27;
        TPanel *pnMotrJog2;
        TGroupBox *GroupBox28;
        TPanel *pnMotrJog8;
        TGroupBox *GroupBox41;
        TPanel *pnMotrPos16;
        TPanel *pnAtcr7;
        TPanel *pnAtcr9;
        TGroupBox *GroupBox42;
        TPanel *pnMotrJog16;
        TPanel *pnIO2;
        TPanel *pnIO0;
        TGroupBox *GroupBox19;
        TPanel *pnMotrPos1;
        TGroupBox *GroupBox31;
        TPanel *pnMotrPos3;
        TGroupBox *GroupBox29;
        TPanel *pnMotrPos9;
        TGroupBox *GroupBox33;
        TPanel *pnMotrPos17;
        TPanel *pnAtcr10;
        TPanel *pnAtcr8;
        TPanel *pnAtcr4;
        TPanel *pnAtcr6;
        TPanel *pnIO3;
        TPanel *pnIO1;
        TPanel *pnJog1;
        TPageControl *PageControl9;
        TTabSheet *TabSheet4;
        TGroupBox *GroupBox2;
        TPanel *pnMotrJog1;
        TGroupBox *GroupBox23;
        TPanel *pnMotrJog3;
        TGroupBox *GroupBox24;
        TPanel *pnMotrJog9;
        TGroupBox *GroupBox34;
        TPanel *pnMotrJog17;
        TPanel *pnJogUnit;
        TRadioGroup *rgJogUnit;
        TEdit *edJogUnit;
        TComboBox *cbWorkMode;
        TLabel *Label8;
        TGroupBox *GroupBox1;
        TLabel *UnLoader;
        TImage *Image4;
        TImage *Image5;
        TImage *Image3;
        TLabel *Label82;
        TImage *Image8;
        TStaticText *StaticText3;
        TEdit *edHicHeight;
        TStaticText *StaticText2;
        TEdit *edHicWidth;
        TEdit *edDieHeight;
        TEdit *edDieWidth;
        TEdit *edHicChamper;
        TEdit *edDieChamper;
        TStaticText *StaticText1;
        TStaticText *StaticText4;
        TStaticText *StaticText10;
        TStaticText *StaticText9;
        TImage *Image9;
        TGroupBox *GroupBox10;
        TImage *imArray1;
        TLabel *Loader;
        TImage *Image2;
        TImage *Image1;
        TLabel *Label81;
        TImage *Image7;
        TStaticText *lbRowPitch;
        TEdit *edBoatColPitch;
        TStaticText *lbRowCount;
        TEdit *edBoatColCnt;
        TEdit *edMgzSlotPitch;
        TEdit *edMgzSlotCnt;
        TStaticText *StaticText5;
        TStaticText *StaticText6;
        TPanel *pnFOS;
        TImage *Image12;
        TLabel *Label83;
        TImage *Image10;
        TEdit *edFosHeight;
        TEdit *edFosWidth;
        TEdit *edFosChamper;
        TStaticText *StaticText11;
        TStaticText *StaticText7;
        TStaticText *StaticText8;
        TLabel *Label7;
        TLabel *Label10;
        TEdit *edPlaceTolXY;
        TLabel *Label21;
        TLabel *Label22;
        TEdit *edPlaceTolAng;
        TImage *Image6;
        TLabel *Label5;
        TLabel *Label25;
        TEdit *edDispZOfs;
        TImage *Image11;
        TLabel *Label13;
        TLabel *Label14;
        TEdit *edPlaceZOfs1;
        TLabel *Label23;
        TLabel *Label24;
        TEdit *edPlaceZOfs2;
        TLabel *Label15;
        TLabel *Label16;
        TEdit *edAttachSpeed1;
        TLabel *Label17;
        TLabel *Label18;
        TEdit *edAttachSpeed2;
        TImage *Image13;
        TImage *Image14;
        TLabel *Label19;
        TLabel *Label20;
        TEdit *edAttachForce;
        TLabel *Label1;
        TLabel *Label2;
        TEdit *edPkPushDelay;
        TLabel *Label9;
        TLabel *Label28;
        TEdit *edPkPickDelay;
        TPanel *Panel4;
        TLabel *Label29;
        TLabel *Label31;
        TLabel *Label32;
        TPanel *Panel5;
        TEdit *edRailBtmTemp;
        TEdit *edStageTemp;
        TLabel *Label30;
        TImage *Image15;
        TBitBtn *btPckrStdCal;
        TBitBtn *btDsprStdCal;
        TBitBtn *btStageVisnPos;
        TLabel *Label33;
        TLabel *Label34;
        TEdit *edDisprAcc;
        TEdit *edDisprDec;
        TLabel *Label35;
        TLabel *Label36;
        TEdit *edRailTopTemp;
        TLabel *Label37;
        TLabel *Label38;
        TLabel *Label39;
        TLabel *Label40;
        TEdit *edPickerTolXY;
        TEdit *edPickerTolAng;
        TLabel *Label41;
        TLabel *Label42;
        TLabel *edStageTolT;
        TLabel *Label44;
        TEdit *edStageTolXY;
        TEdit *edStageTolAng;
        TLabel *Label4;
        TLabel *Label6;
        TEdit *edPlaceYOfs;
        TPanel *pnFOS2;
        TLabel *Label12;
        TEdit *edFosPlaceSttYOfs;
        TLabel *Label11;
        TBitBtn *btVisnCal;
        TPanel *pnFOS3;
        TLabel *Label50;
        TEdit *edReptSttXPos;
        TLabel *Label51;
        TEdit *edReptSttYPos;
        TLabel *Label52;
        TEdit *edReptEndXPos;
        TLabel *Label53;
        TEdit *edReptEndYPos;
        TLabel *Label54;
        TEdit *edReptCnt;
        TLabel *Label55;
        TEdit *edReptSpd;
        TLabel *Label56;
        TLabel *Label57;
        TLabel *Label58;
        TLabel *Label59;
        TLabel *Label60;
        TLabel *Label61;
        TEdit *edAttachStageOfsX;
        TEdit *edAttachStageOfsY;
        TLabel *Label62;
        TEdit *edEpoxyName;
        TBitBtn *btAlignerCal;
        TImage *Image16;
        TBitBtn *btPckAlignPos;
        TBitBtn *btPckDefectPos;
        TLabel *Label64;
        TEdit *edFosPlaceSttYSpd;
        TLabel *Label63;
        TLabel *Label65;
        TEdit *edFosPlaceBfDelay;
        TLabel *Label66;
        TLabel *Label67;
        TLabel *Label68;
        TEdit *edAttachForceOfs;
        TGroupBox *gbPattern;
        TLabel *Label76;
        TLabel *Label77;
        TLabel *Label78;
        TLabel *Label79;
        TLabel *Label87;
        TLabel *Label88;
        TLabel *Label89;
        TLabel *Label90;
        TLabel *Label26;
        TLabel *Label27;
        TEdit *edScalePatternX;
        TEdit *edScalePatternY;
        TEdit *edSttOffSetX;
        TEdit *edSttOffSetY;
        TEdit *edPosCnt;
        TStringGrid *sgPatternPos;
        TEdit *edPlaceXOfs;
        TImage *imPatternImage;
        TOpenDialog *dgLoadFilePath;
        TSaveDialog *dgSaveFilePath;
        TButton *btSavePattern;
        TButton *btOpenPattern;
        TLabel *Label43;
        TLabel *Label45;
        TEdit *edAttachStageOfsT;
        TLabel *Label69;
        TLabel *lbCrntNode;
        TLabel *Label70;
        TLabel *Label71;
        TEdit *edFrstPkgDelay;
        TLabel *Label72;
        TComboBox *cbDspCh;
        TButton *Button1;
        TEdit *edDspMinAmount;
        TLabel *Label46;
        TLabel *Label47;
        TPanel *pnIO8;
        TComboBox *cbLDR_FSlot;
        TButton *btLDR_FSlot;
        TComboBox *cbIDX_FPitch;
        TButton *btIDX_FPitch;
        TComboBox *cbULD_FSlot;
        TButton *btULD_FSlot;
        TComboBox *cbLDR_RSlot;
        TButton *btLDR_RSlot;
        TComboBox *cbIDX_RPitch;
        TButton *btIDX_RPitch;
        TComboBox *cbULD_RSlot;
        TButton *btULD_RSlot;
        TPanel *pnScreen;
        TPanel *pnAtcr11;
    void __fastcall btSavePosClick(TObject *Sender);
    void __fastcall tmUpdateTimerTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall pcDeviceSetChange(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall edJogUnitChange(TObject *Sender);
        void __fastcall rgJogUnitClick(TObject *Sender);
        void __fastcall sgPatternPosSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
        void __fastcall sgPatternPosDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall btOpenPatternClick(TObject *Sender);
        void __fastcall btSavePatternClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall cbDspChChange(TObject *Sender);
        void __fastcall btPckrStdCalClick(TObject *Sender);
        void __fastcall btLDR_FSlotClick(TObject *Sender);
private:	// User declarations
    //Active Radio Button Page.
    //TRadioGroup  * rgUnitUser ;
    //TEdit        * edUnitUser ;
public:		// User declarations
    TFraMotr      *FraMotor   [MAX_MOTR];
    TFraCylOneBt  *FraCylinder[MAX_ACTR];

    TFraOutput    *foIonizerFrnot ;
    TFraOutput    *foIonizerRear  ;
    TFraOutput    *foIdxIonFront  ;
    TFraOutput    *foIdxIonRear   ;
    TFraOutput    *foSTGVccm      ;
    TFraOutput    *foSTGEjct      ;
    TFraOutput    *foPCKVccmOn    ;
    TFraOutput    *foPCKEjct      ;
    TFraOutput    *foPCKVccmOff   ;

    __fastcall TFrmDeviceSet(TComponent* Owner);
    void __fastcall UpdateDevInfo(bool bTo);
    void __fastcall UpdateDevOptn(bool bTo);
    void __fastcall UpdatePattern(bool bTo);

    // Pattern
    void PaintPattern() ;

    void SetComboItem();

    //메뉴얼 동작 후에 화면 갱신 한번 시키기 위해서 추가.JS


};

//---------------------------------------------------------------------------
extern PACKAGE TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
#endif
