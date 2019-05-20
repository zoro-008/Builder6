//---------------------------------------------------------------------------

#ifndef FormMapViewerH
#define FormMapViewerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmMapViewer : public TForm
{
__published:	// IDE-managed Components
    TButton *btLoadHexMapFile;
        TOpenDialog *odMap;
        TMemo *Memo1;
        TMemo *Memo2;
    TMemo *Memo3;
    TPanel *Panel1;
    TPanel *Panel2;
        void __fastcall btLoadHexMapFileClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmMapViewer(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMapViewer *FrmMapViewer;
//---------------------------------------------------------------------------
#endif
