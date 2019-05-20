//---------------------------------------------------------------------------

#ifndef FormMasterPkgH
#define FormMasterPkgH
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFrmMstPkg : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TPanel *Panel1;
        TPanel *Panel5;
        TBitBtn *btInspMstPkg;
        TImage *Image1;
        TTimer *tmLotName;
        void __fastcall btInspMstPkgClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmMstPkg(TComponent* Owner);


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMstPkg *FrmMstPkg;
//---------------------------------------------------------------------------
#endif
