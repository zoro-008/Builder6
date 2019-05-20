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
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFrmLotOpen : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TEdit *edLotId;
    TEdit *edSelDevice;
        TBitBtn *btLotOpen;
        TBitBtn *btCancel;
    TPanel *Panel5;
    TImage *Image2;
    TLabel *Label19;
    TStaticText *StaticText6;
    TStaticText *StaticText1;
    TTimer *tmLotName;
        TStringGrid *sgData;
        TEdit *edLotFilePath;
        TButton *btLotFile;
        TStaticText *StaticText2;
        TOpenDialog *dgFilePath;
        TStaticText *StaticText3;
        TEdit *edEpoxyLotNo;
        void __fastcall btLotOpenClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btCancelClick(TObject *Sender);
        void __fastcall tmLotNameTimer(TObject *Sender);
        void __fastcall btLotFileClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmLotOpen(TComponent* Owner);

        void DispLotNo();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmLotOpen *FrmLotOpen;
//---------------------------------------------------------------------------
#endif
