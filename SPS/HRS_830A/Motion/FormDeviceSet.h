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
        TPanel *pnMotrPos5;
        TGroupBox *gbMotrPos17;
        TPanel *pnMotrPos4;
        TPanel *pnJog1;
        TPageControl *PageControl3;
        TTabSheet *TabSheet8;
        TGroupBox *gbMotrJog17;
        TPanel *pnMotrJog4;
        TGroupBox *gbMotrJog08;
        TPanel *pnMotrJog5;
        TPanel *pnJogUnit;
        TRadioGroup *rgJogUnit;
        TEdit *edJogUnit;
        TPanel *pnLDR1;
        TPanel *pnLDR2;
        TPanel *pnAtcr7;
        TPanel *pnAtcr6;
        TTabSheet *tsPrebuffer;
        TGroupBox *gbMotrPos01;
        TPanel *pnMotrPos0;
        TPanel *pnJog2;
        TPageControl *PageControl1;
        TTabSheet *TabSheet2;
        TGroupBox *gbMotrJog01;
        TPanel *pnMotrJog0;
        TPanel *pnAtcr0;
        TTabSheet *tsRail;
        TGroupBox *gbMotrPos10;
        TPanel *pnMotrPos3;
        TPanel *pnProbeMotr;
        TPanel *pnJog3;
        TPageControl *PageControl2;
        TTabSheet *TabSheet6;
        TGroupBox *gbMotrJog11;
        TPanel *pnMotrJog2;
        TGroupBox *gbMotrJog13;
        TPanel *pnMotrJog3;
        TGroupBox *gbMotrPos11;
        TPanel *pnMotrPos2;
        TPanel *pnAtcr2;
        TPanel *pnAtcr1;
        TPanel *pnWRK1;
        TTabSheet *tsPostBuffer;
        TPanel *pnJog4;
        TPageControl *PageControl12;
        TTabSheet *TabSheet13;
        TGroupBox *gbMotrJog04;
        TPanel *pnMotrJog1;
        TGroupBox *gbMotrPos09;
        TPanel *pnMotrPos1;
        TPanel *pnAtcr5;
        TPanel *pnAtcr4;
        TPanel *pnAtcr3;
        TPanel *pnPSB1;
        TTabSheet *tsUnLoader;
        TImage *Image9;
        TGroupBox *gbMotrPos19;
        TPanel *pnMotrPos6;
        TGroupBox *gbMotrPos20;
        TPanel *pnMotrPos7;
        TPanel *pnJog5;
        TPageControl *PageControl4;
        TTabSheet *TabSheet10;
        TGroupBox *gbMotrJog09;
        TPanel *pnMotrJog6;
        TGroupBox *gbMotrJog10;
        TPanel *pnMotrJog7;
        TPanel *pnULD1;
        TPanel *pnULD2;
        TPanel *pnAtcr8;
        TTabSheet *tsOption;
        TPanel *Panel9;
        TLabel *Label9;
        TLabel *Label28;
        TPanel *Panel12;
        TEdit *edLotEndMgzCnt;
        TPanel *Panel1;
        TLabel *Label15;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *Label20;
        TPanel *Panel2;
        TEdit *edOutPshtDelay;
        TEdit *edUpDnPshDelay;
        TPanel *Panel3;
        TLabel *Label1;
        TLabel *Label3;
        TPanel *Panel4;
        TEdit *edDispsrLTimer;
        TBitBtn *btSaveDevice;
    TLabel *lbColGrCnt;
    TLabel *lbRowGrCnt;
    TLabel *lbColCnt;
    TLabel *lbRowCnt;
    TImage *imArray;
    TImage *Image2;
    TStaticText *StaticText1;
        TLabel *Label2;
        TLabel *Label4;
        TEdit *edDispsrRTimer;
        TPanel *pnWRK3;
        TPanel *pnWRK2;
        TPanel *pnPRB1;
        TPanel *Panel5;
        TPanel *Panel6;
        TLabel *Label7;
        TEdit *edDspsWrkCnt;
        TLabel *Label5;
        TEdit *edDisprSnsrTimer;
        TLabel *Label6;
    void __fastcall btSavePosClick(TObject *Sender);
    void __fastcall tmUpdateTimerTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall pcDeviceSetChange(TObject *Sender);
        void __fastcall rgJogUnitClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
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
    TFraOutput    *foWRKEjctOn    ;
    TFraOutput    *foWRKDspsLOn   ;
    TFraOutput    *foWRKDspsROn   ;
    TFraOutput    *foPRBAirBlwOn  ;

    __fastcall TFrmDeviceSet(TComponent* Owner);
    void __fastcall UpdateDevInfo(bool bTo);
    void __fastcall UpdateDevOptn(bool bTo);
    void __fastcall PaintStrpInfo();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
#endif
