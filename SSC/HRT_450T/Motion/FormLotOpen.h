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
    TEdit *edOperator;
    TEdit *edLotId;
    TEdit *edSelDevice;
        TBitBtn *btLotOpen;
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
    TBitBtn *btAuto;
    TStaticText *StaticText3;
    TTimer *tmLotName;
        TStringGrid *sgLotAdd;
        TBitBtn *btAdd;
    TLabel *Label1;
    TLabel *Label2;
    TBitBtn *btLotNoDel;
    TBitBtn *btLotAddDel;
        TEdit *edBarcode;
        TStaticText *StaticText4;
        TTimer *tmLotOpen;
        void __fastcall btLotOpenClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btCancelClick(TObject *Sender);
    void __fastcall btAutoClick(TObject *Sender);
    void __fastcall tmLotNameTimer(TObject *Sender);
    void __fastcall btAddClick(TObject *Sender);
    void __fastcall btLotNoDelClick(TObject *Sender);
    void __fastcall btLotAddDelClick(TObject *Sender);
        void __fastcall edBarcodeKeyPress(TObject *Sender, char &Key);
        void __fastcall tmLotOpenTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmLotOpen(TComponent* Owner);
        bool bLotOpenEnable ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmLotOpen *FrmLotOpen;
//---------------------------------------------------------------------------
#endif