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
        TBitBtn *btSaveDevice;
        TPageControl *pcDeviceSet;
        TTabSheet *tsLoader;
        TGroupBox *gbMotrPos18;
        TPanel *pnMotrPos0;
        TGroupBox *gbMotrPos17;
        TPanel *pnMotrPos1;
        TPanel *pnJog1;
        TPageControl *PageControl3;
        TTabSheet *TabSheet8;
        TGroupBox *gbMotrJog17;
        TPanel *pnMotrJog0;
        TGroupBox *gbMotrJog08;
        TPanel *pnMotrJog1;
        TPanel *pnJogUnit;
        TRadioGroup *rgJogUnit;
        TEdit *edJogUnit;
        TPanel *pnVaccm;
        TPanel *pnAtcr1;
        TPanel *pnAtcr0;
        TTabSheet *tsOption;
        TPanel *Panel1;
        TLabel *Label15;
        TLabel *Label17;
        TPanel *Panel2;
        TEdit *edVaccmdelay;
        TBitBtn *BitBtn1;
        TLabel *Label2;
        TEdit *edPlaceDelay;
        TLabel *Label1;
        TLabel *Label3;
        TLabel *Label4;
        TEdit *edEjectdelay;
        TPanel *pnEject;
        TPanel *Panel3;
        TPanel *Panel4;
        TLabel *Label18;
        TEdit *edDispnrdelay;
        TLabel *Label20;
        TPanel *Panel5;
        TLabel *Label13;
        TLabel *Label14;
        TPanel *Panel6;
        TEdit *edJigPCLOfs;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TLabel *Label16;
        TLabel *Label19;
        TEdit *edDisprShotCnt;
        TEdit *edDisprRptCnt;
        TEdit *edDisprIncPosCnt;
        TLabel *Label5;
        TLabel *Label6;
        TEdit *edDisprSpd;
        TLabel *Label11;
        TLabel *Label12;
        TEdit *edEndDelay;
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

    TFraOutput    *foVaccm   ;
    TFraOutput    *foEject   ;

    __fastcall TFrmDeviceSet(TComponent* Owner);
    void __fastcall UpdateDevInfo(bool bTo);
    void __fastcall UpdateDevOptn(bool bTo);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
#endif
