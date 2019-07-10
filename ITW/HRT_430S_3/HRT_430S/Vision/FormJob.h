//---------------------------------------------------------------------------

#ifndef JAJobFormH
#define JAJobFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//#include "CGAUGES.h"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>

//---------------------------------------------------------------------------
class TFrmJob : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TLabel *Label1;
        TStringGrid *sgDevice;
        TEdit *edSelDevice;
        TEdit *edNewDevice;
        TEdit *edCrntDevice;
        TImage *Image2;
        TLabel *Label2;
        TLabel *Label3;
        TSpeedButton *btOk;
    TSpeedButton *btExit;
        TSpeedButton *btRename;
        TSpeedButton *btDelete;
        TSpeedButton *btCopy;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btOkClick(TObject *Sender);
    void __fastcall btExitClick(TObject *Sender);
    void __fastcall btNewClick(TObject *Sender);
    void __fastcall btRenameClick(TObject *Sender);
    void __fastcall btCopyClick(TObject *Sender);
    void __fastcall btDeleteClick(TObject *Sender);
    void __fastcall sgDeviceMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
public:		// User declarations

    AnsiString FolderName;
    __fastcall TFrmJob(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmJob *FrmJob;
//---------------------------------------------------------------------------
#endif
