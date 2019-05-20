//---------------------------------------------------------------------------

#ifndef FrmLotOpenH
#define FrmLotOpenH
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
#include "UserFile.h"
//---------------------------------------------------------------------------
class TFormLotOpen : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TEdit *edOperator;
    TEdit *edLotId;
    TEdit *edSelDevice;
        TBitBtn *btLotOpen;
        TBitBtn *btCancel;
    TPanel *Panel2;
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
        TButton *Button1;
        TStringGrid *sgDevice;
        void __fastcall btLotOpenClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btCancelClick(TObject *Sender);
    void __fastcall btAutoClick(TObject *Sender);
    void __fastcall tmLotNameTimer(TObject *Sender);
    void __fastcall btAddClick(TObject *Sender);
    void __fastcall btLotNoDelClick(TObject *Sender);
    void __fastcall btLotAddDelClick(TObject *Sender);
        void __fastcall edBarcodeKeyPress(TObject *Sender, char &Key);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormLotOpen(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormLotOpen *FormLotOpen;
//---------------------------------------------------------------------------
#endif
