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
//#include <Classes.hpp>
//#include <Controls.hpp>
//#include <StdCtrls.hpp>
//#include <Forms.hpp>
//#include <ExtCtrls.hpp>
//#include <Buttons.hpp>
//#include <Graphics.hpp>
//#include <Grids.hpp>
//---------------------------------------------------------------------------
class TFrmLotOpen : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TEdit *edOperator;
        TLabel *Label2;
        TLabel *edit;
        TEdit *edLotId;
        TEdit *edSelDevice;
        TLabel *DEVICE;
        TBitBtn *BitBtn11;
        TBitBtn *BitBtn12;
        TPanel *Panel2;
        TStringGrid *sgDevice;
    TPanel *Panel5;
    TImage *Image2;
    TLabel *Label19;
        TStringGrid *sgLotName;
        TEdit *edMgzCnt;
        TLabel *Label1;
        TBitBtn *btAuto;
        TTimer *tmLotName;
        TBitBtn *btClear;
        void __fastcall BitBtn11Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall BitBtn12Click(TObject *Sender);
        void __fastcall btAutoClick(TObject *Sender);
        void __fastcall tmLotNameTimer(TObject *Sender);
        void __fastcall btClearClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmLotOpen(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmLotOpen *FrmLotOpen;
//---------------------------------------------------------------------------
#endif
