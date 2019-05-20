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
        TPanel *Panel1;
        TBitBtn *btOk;
        TPanel *pnTitle;
        TLabel *lbName;
        TLabel *lbTitle;
        TImage *Image1;
        TTimer *tmUpdate;
        TLabel *lbSolut;
        TLabel *lbSetTime;
    void __fastcall btOkClick(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
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
void __fastcall ShowErr(int _iNo , AnsiString _sCmmt , AnsiString _sSolut , AnsiString _sSetTime);
void __fastcall HideErr();
//---------------------------------------------------------------------------
#endif
