//---------------------------------------------------------------------------

#ifndef FormPosSetH
#define FormPosSetH
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
class TFrmPosSet : public TForm
{
__published:	// IDE-managed Components
    TTimer *tmUpdateTimer;
        TImageList *ImageList1;
        TBitBtn *btSaveDevice;
        TPanel *pnJog1;
        TPageControl *PageControl3;
        TTabSheet *TabSheet8;
        TGroupBox *gbMotrJog1;
        TPanel *pnMotrJog0;
        TPanel *pnJogUnit;
        TRadioGroup *rgJogUnit;
        TEdit *edJogUnit;
        TPanel *pnAtcr1;
        TPanel *pnAtcr0;
        TPanel *pnAtcr2;
        TGroupBox *gbMotrPos18;
        TPanel *pnMotrPos0;
    void __fastcall btSavePosClick(TObject *Sender);
    void __fastcall tmUpdateTimerTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall rgJogUnitClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall edJogUnitChange(TObject *Sender);
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

    __fastcall TFrmPosSet(TComponent* Owner);
    void __fastcall UpdateDevInfo(bool bTo);
    void __fastcall UpdateDevOptn(bool bTo);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmPosSet *FrmPosSet;
//---------------------------------------------------------------------------
#endif
