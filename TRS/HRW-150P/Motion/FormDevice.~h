//---------------------------------------------------------------------------

#ifndef FormDeviceH
#define FormDeviceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TFrmDevice : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnFormDevice;
        TLabel *lbModelName;
        TLabel *Label8;
        TLabel *Label1;
        TPanel *lbCrntDevice;
        TLabel *Label2;
        TPanel *lbSelDevice;
        TPanel *btDownload;
        TLabel *Label4;
        TPanel *Panel6;
        TStringGrid *sgDevice;
        TLabel *lbFromName;
        TEdit *edFromName;
        TLabel *lbToName;
        TEdit *edToName;
        TPanel *btNew;
        TPanel *btCopy;
        TPanel *btDelete;
        TPanel *btRename;
        TPanel *pnButton;
        TButton *bt1;
        TButton *bt2;
        TButton *bt3;
        TButton *bt4;
        TButton *bt5;
        TButton *bt6;
        TButton *bt7;
        TButton *bt8;
        TButton *bt9;
        TButton *btClear;
        TButton *bt0;
        TButton *btBack;
        TButton *btOK;
        void __fastcall btNewMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btNewMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall sgDeviceSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
        void __fastcall btDownloadClick(TObject *Sender);
        void __fastcall btNewClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btOKClick(TObject *Sender);
        void __fastcall btClearClick(TObject *Sender);
        void __fastcall btBackClick(TObject *Sender);
        void __fastcall bt1Click(TObject *Sender);
        void __fastcall edToNameClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmDevice(TComponent* Owner);

        int iEditNo;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmDevice *FrmDevice;
//---------------------------------------------------------------------------
#endif
