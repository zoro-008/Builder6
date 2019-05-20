//---------------------------------------------------------------------------

#ifndef FormMsgOkH
#define FormMsgOkH
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmMsgOk : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TPanel *Panel2;
        TLabel *lbMessage;
        TLabel *lbTitle;
        TImage *Image1;
        TImage *Image2;
        TBitBtn *btOk;
    void __fastcall btOkClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmMsgOk(TComponent* Owner);

        //Message Process.
    	void __fastcall WmUser(TMessage &Message);

        //Message Handler.
        BEGIN_MESSAGE_MAP
            MESSAGE_HANDLER(WM_USER, TMessage, WmUser)
        END_MESSAGE_MAP(TForm)

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMsgOk *FrmMsgOk;
void __fastcall MsgOk     (AnsiString _sCaption , AnsiString _sMessage);
void __fastcall MsgOkModal(AnsiString _sCaption , AnsiString _sMessage);
//---------------------------------------------------------------------------
#endif
