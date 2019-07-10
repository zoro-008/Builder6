//---------------------------------------------------------------------------

#ifndef FormMsgTimeH
#define FormMsgTimeH
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmMsgTime : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TPanel *Panel2;
        TLabel *lbMessage;
        TLabel *lbTitle;
        TImage *Image1;
        TImage *Image2;
    TTimer *tmClose;
        void __fastcall FormDestroy(TObject *Sender);
    void __fastcall tmCloseTimer(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormHide(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmMsgTime(TComponent* Owner);

        //Message Process.
    	void __fastcall WmUser(TMessage &Message);

        //Message Handler.
        BEGIN_MESSAGE_MAP
            MESSAGE_HANDLER(WM_USER, TMessage, WmUser)
        END_MESSAGE_MAP(TForm)

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMsgTime *FrmMsgTime;
void __fastcall MsgTime(AnsiString _sCaption , AnsiString _sMessage , int _iShowTime);
void __fastcall CloseMsgOk();
//---------------------------------------------------------------------------
#endif
