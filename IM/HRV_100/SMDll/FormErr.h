//---------------------------------------------------------------------------

#ifndef FormErrH
#define FormErrH
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmErr : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnPicture;
        TTimer *tmUpdate;
        TPanel *Panel2;
        TLabel *lbErrNo;
        TPanel *Panel5;
        TPanel *Panel6;
        TLabel *Label5;
        TMemo *mmErrAction;
        TPanel *Panel8;
        TBitBtn *btClose;
    TImage *Image1;
        TPanel *Panel7;
        TShape *spErrRect;
        TImage *imageErr;
    TPanel *Panel3;
    TLabel *lbName;
    TPanel *Panel4;
    TLabel *Label2;
    TPanel *Panel9;
    TPanel *Panel10;
    TLabel *Label1;
    TMemo *mmErrMsg;
        TPanel *Panel11;
        TLabel *lbEnum;
        TPanel *Panel12;
        TLabel *Label4;
    void __fastcall btResetClick(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall btCloseClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmErr(TComponent* Owner);

        //Message Process.
    	void __fastcall WmUser(TMessage &Message);

        //Message Handler.
        BEGIN_MESSAGE_MAP
            MESSAGE_HANDLER(WM_USER, TMessage, WmUser)
        END_MESSAGE_MAP(TForm)

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmErr *FrmErr;
void __fastcall ShowErr(int _iNo);
void __fastcall HideErr();
//---------------------------------------------------------------------------
#endif