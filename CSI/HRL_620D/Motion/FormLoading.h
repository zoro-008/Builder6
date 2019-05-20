//---------------------------------------------------------------------------

#ifndef FormLoadingH
#define FormLoadingH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TFrmLoading : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TImage *Image1;
        TLabel *lbName;
    TProgressBar *pbProcess;
    TLabel *Label2;
    TLabel *lbVersion;
    TTimer *tmProcess;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall tmProcessTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations

public:		// User declarations
    __fastcall TFrmLoading(TComponent* Owner , bool _bLoad);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmLoading *FrmLoading;
//---------------------------------------------------------------------------
#endif
