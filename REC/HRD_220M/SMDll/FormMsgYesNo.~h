//---------------------------------------------------------------------------

#ifndef FormMsgYesNoH
#define FormMsgYesNoH

#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmMsgYesNo : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TImage *Image1;
        TLabel *lbMessage;
        TPanel *Panel2;
        TLabel *lbTitle;
        TBitBtn *btNo;
        TBitBtn *btYes;
        TImage *Image2;
        TTimer *Timer1;
        void __fastcall btYesClick(TObject *Sender);
        void __fastcall btNoClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmMsgYesNo(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMsgYesNo *FrmMsgYesNo;
int __fastcall MsgYesNo(AnsiString _sCaption, AnsiString _sMessage);
//---------------------------------------------------------------------------
#endif
