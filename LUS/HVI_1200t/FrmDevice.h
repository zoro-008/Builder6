//---------------------------------------------------------------------------

#ifndef FrmDeviceH
#define FrmDeviceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TFormDevice : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TButton *btnDevSet;
    TButton *btnMotrPos;
    TPanel *pnDevice;
    TLabel *lbCurntDevice;
    TLabel *lbSelectedDevice;
    TProgressBar *pbStatus;
    TPanel *lbSelDevice;
    TPanel *lbCrntDevice;
    TBitBtn *btDownload;
    TStringGrid *sgDevice;
    TLabel *lbFromName;
    TLabel *lbToName;
    TBitBtn *btNew;
    TBitBtn *btRename;
    TBitBtn *btCopy;
    TBitBtn *btDelete;
    TEdit *edFromName;
    TEdit *edToName;
    TButton *btnDevice;
        TButton *btInOutState;
        TTimer *tmUpdate;
    TButton *btnOption;
    void __fastcall btnDevSetClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnDeviceClick(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall btDownloadClick(TObject *Sender);
        void __fastcall btNewClick(TObject *Sender);
        void __fastcall sgDeviceSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
    int m_iPageIndex ;
public:		// User declarations
    __fastcall TFormDevice(TComponent* Owner);

    void __fastcall   ShowPage  (int Index) ;
    void __fastcall   ClosePage () ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDevice *FormDevice;
//---------------------------------------------------------------------------
#endif

