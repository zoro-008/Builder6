//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TFrmCamera : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnTitle;
        TImage *Image2;
        TLabel *lbDate;
        TLabel *lbStat;
        TLabel *Label6;
        TLabel *lbDevice;
        TLabel *lbName;
        TPanel *Panel3;
        TPanel *Panel1;
        TPanel *Panel2;
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmCamera(TComponent* Owner);
        void Init();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmCamera *FrmCamera;
//---------------------------------------------------------------------------
#endif
