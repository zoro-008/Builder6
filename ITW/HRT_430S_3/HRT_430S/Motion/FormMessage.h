//---------------------------------------------------------------------------

#ifndef FormMessageH
#define FormMessageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TFrmMessage : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TImage *Image1;
    TLabel *lbMessage;
    TPanel *Panel2;
    TLabel *lbTitle;
    TBitBtn *btNo;
    TBitBtn *btYes;
private:	// User declarations
public:		// User declarations
    __fastcall TFrmMessage(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMessage *FrmMessage;
//---------------------------------------------------------------------------
#endif
