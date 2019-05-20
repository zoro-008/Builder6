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
//#include "TEdit.h"  시발 폼디바이스 컴파일 안됌.... 컴포넌트로 제작 해야 할듯...
#include <ImgList.hpp>
class TFrmDeviceSet : public TForm
{
__published:	// IDE-managed Components
    TTimer *tmUpdateTimer;
    TPageControl *pcDeviceSet;
    TBitBtn *btSaveDevice;
        TTabSheet *tsFeed;
    TTabSheet *tsLaser;
    TPanel *Panel21;
    TPanel *Panel26;
    TPanel *Panel27;
    TPanel *Panel28;
    TPanel *Panel29;
    TPanel *Panel34;
        TPanel *pnJog1;
    TPageControl *PageControl7;
    TTabSheet *TabSheet15;
    TPanel *Panel49;
        TPanel *pnJog2;
    TPageControl *PageControl8;
    TTabSheet *TabSheet16;
    TGroupBox *gbMotrPos02;
        TPanel *pnMotrPos0;
    TGroupBox *gbMotrPos03;
        TPanel *pnMotrPos1;
    TGroupBox *gbMotrJog02;
        TPanel *pnMotrJog0;
    TGroupBox *GroupBox1;
        TPanel *pnMotrJog1;
    TGroupBox *GroupBox21;
        TPanel *pnMotrPos2;
    TGroupBox *GroupBox22;
        TPanel *pnMotrPos3;
    TGroupBox *GroupBox23;
        TPanel *pnMotrJog2;
    TGroupBox *GroupBox24;
        TPanel *pnMotrJog3;
    TTimer *Timer1;
    TGroupBox *GroupBox18;
    TCheckBox *cbUsedLsrPos1;
    TEdit *edFrntLsrNo1;
    TCheckBox *cbUsedLsrPos2;
    TEdit *edFrntLsrNo2;
    TCheckBox *cbUsedLsrPos3;
    TEdit *edFrntLsrNo3;
    TCheckBox *cbUsedLsrPos4;
    TEdit *edFrntLsrNo4;
    TCheckBox *cbUsedLsrPos5;
    TEdit *edFrntLsrNo5;
    TGroupBox *GroupBox19;
    TEdit *edBackLsrNo1;
    TEdit *edBackLsrNo2;
    TEdit *edBackLsrNo3;
    TEdit *edBackLsrNo4;
    TEdit *edBackLsrNo5;
        TEdit *edSendGA;
        TButton *btSend;
        TLabel *Label1;
        TPanel *pnJogUnit;
        TRadioGroup *rgJogUnit1;
        TEdit *edJogUnit;
        TTabSheet *TabSheet1;
        TPanel *pnSetVisn;
        TPanel *Panel4;
        TPanel *Panel5;
        TPanel *pnAtcr0;
        TPanel *pnAtcr1;
        TPanel *pnAtcr2;
        TPanel *pnAtcr3;
        TPanel *pnFED1;
        TPanel *pnFED2;
        TPanel *pnFED3;
        TPanel *pnFED4;
        TPanel *Panel1;
        TPanel *Panel2;
        TPanel *pnLSR2;
        TPanel *pnLSR3;
        TPanel *pnLSR4;
        TPanel *Panel9;
        TPanel *Panel10;
        TPanel *pnAtcr4;
        TPanel *pnAtcr5;
        TPanel *pnLSR1;
        TGroupBox *GroupBox2;
        TPanel *pnSetPara;
        TPanel *pnFED5;
        TBitBtn *btDustSuck;
    void __fastcall btSavePosClick(TObject *Sender);
    void __fastcall tmUpdateTimerTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall pcDeviceSetChange(TObject *Sender);
        void __fastcall rgJogUnit1Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall btSendClick(TObject *Sender);
        void __fastcall btDustSuckClick(TObject *Sender);
        //void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
    //Active Radio Button Page.
    //TRadioGroup  * rgUnitUser ;
    //TEdit        * edUnitUser ;
public:		// User declarations
    TFraMotr      *FraMotor   [MAX_MOTR];
    TFraCylOneBt  *FraCylinder[MAX_ACTR];

    TFraOutput    *foFeedAirBlow1 ;
    TFraOutput    *foFeedVaccum1  ;
    TFraOutput    *foBawlFeeder   ;
    TFraOutput    *foFeedAirBlow2 ;
    TFraOutput    *foFrontSTGVcc  ;
    TFraOutput    *foLearSTGVcc   ;
    TFraOutput    *foFrontSTGEjt  ;
    TFraOutput    *foLearSTGEjt   ;
    TFraOutput    *foFeedVaccum2  ;

    __fastcall TFrmDeviceSet(TComponent* Owner);
    void __fastcall UpdateDevInfo(bool bTo);
    void __fastcall UpdateDevOptn(bool bTo);
    void __fastcall EditEnable();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
#endif
