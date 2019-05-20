//---------------------------------------------------------------------------

#ifndef FormMainH
#define FormMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include <Graphics.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TFrmMain : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *pnBase;
    TPanel *pnMenu;
    TBitBtn *btOperationShow;
    TBitBtn *btDeviceShow;
    TBitBtn *btUtilShow;
    TBitBtn *btExit;
    TImage *imTileBar;
    TLabel *lbStat;
    TLabel *Label6;
    TLabel *lbDevice;
    TTimer *tmUpdate;
    TLabel *lbName;
    TPanel *pnLoadingStop;
    TPanel *pnIgnreDoor;
        TPanel *pnIgnrHeatErr;
    TPanel *pnNoTrim;
    TBitBtn *btSpcShow;
    TLabel *lbVer;
    TPanel *pnOption4;
    TLabel *lbDate;
    TLabel *lbTime;
        TBitBtn *btOptionShow;
    void __fastcall btOperationShowClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btExitClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btUtilShowClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall lbNameDblClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall lbVerDblClick(TObject *Sender);
    void __fastcall tmUpdateTimer(TObject *Sender);
private:	// User declarations

public:		// User declarations
    __fastcall TFrmMain(TComponent* Owner);

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
