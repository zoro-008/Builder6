//---------------------------------------------------------------------------

#ifndef FormMainH
#define FormMainH
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmMain : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *pnBase;
    TPanel *pnMenu;
    TBitBtn *btOperationShow;
    TBitBtn *btDeviceShow;
    TBitBtn *btExit;
    TImage *imTileBar;
    TLabel *lbStat;
    TLabel *Label6;
    TLabel *lbDevice;
    TTimer *tmUpdate;
    TLabel *lbName;
    TPanel *pnLoadingStop;
    TPanel *pnIgnreDoor;
    TPanel *pnIgnrOriInsp;
    TPanel *pnNoTrim;
    TLabel *lbVer;
    TPanel *pnOption4;
    TLabel *lbDate;
    TLabel *lbTime;
    TBitBtn *btDeviceSetShow;
    void __fastcall btOperationShowClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btExitClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall lbNameDblClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall lbVerDblClick(TObject *Sender);
    void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall lbDeviceDblClick(TObject *Sender);
private:	// User declarations

public:		// User declarations
    __fastcall TFrmMain(TComponent* Owner);
    __fastcall ~TFrmMain();

    // InitÇÔ¼ö
    void __fastcall Init(void);
    void __fastcall MotorBreakSet(void);

    // Page show
    int m_iPageIndex;
    // Page show Func.
    void __fastcall ShowPage     (int index);
    void __fastcall ClosePage    (int index);

};
AnsiString OnGetVer(const AnsiString &sQuery);
//---------------------------------------------------------------------------
extern PACKAGE TFrmMain *FrmMain;
//---------------------------------------------------------------------------
#endif
