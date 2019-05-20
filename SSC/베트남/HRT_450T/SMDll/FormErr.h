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
        TTimer *tmUpdate;
        TPanel *Panel2;
        TLabel *lbErrNo;
        TPanel *Panel3;
        TPanel *Panel4;
        TLabel *Label2;
        TLabel *lbName;
        TPanel *Panel5;
        TPanel *Panel6;
        TLabel *Label5;
        TPanel *Panel7;
    TImage *imageErr;
        TShape *spErrRect;
        TMemo *mmErrAction;
        TPanel *Panel8;
        TBitBtn *btClose;
    TImage *Image1;
    void __fastcall btResetClick(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall btCloseClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormHide(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
    void __fastcall DisplayErrForm (TLabel *_lbErrNo    ,
                                    TLabel *_lbName     ,
                                    TMemo  *_mmAction   ,
                                    TImage *_imErr      ,
                                    TShape *_spErrRect  );
public:		// User declarations
    __fastcall TFrmErr(TComponent* Owner);
    __fastcall ~TFrmErr();

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
