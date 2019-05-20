//---------------------------------------------------------------------------

#ifndef FormLotOpenH
#define FormLotOpenH
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
#include "UserFile.h"
//---------------------------------------------------------------------------
class TFrmLotOpen : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TLabel *Label1;
    TSpeedButton *btLotAdd;
    TEdit *edOperator;
    TEdit *edLotId;
    TEdit *edSelDevice;
    TBitBtn *btCancel;
    TPanel *Panel2;
    TStringGrid *sgDevice;
    TPanel *Panel5;
    TImage *Image2;
    TLabel *Label19;
    TStaticText *StaticText6;
    TStaticText *StaticText1;
    TStaticText *StaticText2;
    TEdit *edMgzCnt;
    TStringGrid *sgLotName;
    TStaticText *StaticText3;
    TBitBtn *btLotNoDel;
    TEdit *edBarcode;
    TStaticText *StaticText4;
    TBitBtn *btLotOpen;
    TBitBtn *btLotNoAllDel;
    TTimer *tmLotName;
        void __fastcall btLotOpenClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btCancelClick(TObject *Sender);
    void __fastcall tmLotNameTimer(TObject *Sender);
    void __fastcall btAddClick(TObject *Sender);
    void __fastcall btLotNoDelClick(TObject *Sender);
        void __fastcall edBarcodeKeyPress(TObject *Sender, char &Key);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btLotNoAllDelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmLotOpen(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmLotOpen *FrmLotOpen;
//---------------------------------------------------------------------------
#endif
