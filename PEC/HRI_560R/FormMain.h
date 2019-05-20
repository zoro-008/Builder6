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
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdIcmpClient.hpp>
#include <IdRawBase.hpp>
#include <IdRawClient.hpp>
//---------------------------------------------------------------------------
class TFrmMain : public TForm
{
__published:	// IDE-managed Components
    TPanel *pnBase;
    TPanel *pnMenu;
    TBitBtn *btOperationShow;
    TBitBtn *btDeviceShow;
    TBitBtn *btOptionShow;
    TBitBtn *btUtilShow;
    TBitBtn *btExit;
    TTimer *tmUpdate;
    TBitBtn *BitBtn1;
    TPanel *Panel1;
    TImage *Image2;
    TLabel *Label6;
    TLabel *lbStat;
    TPanel *pnIgnreDoor;
    TPanel *pnNoTrim;
    TPanel *pnInspSkip;
    TPanel *pnLotChipUpeh;
    TPanel *pnIgnrMrk;
    TLabel *lbName;
    TLabel *lbDate;
    TLabel *lbDevice;
    TLabel *lbDllAge;
    TLabel *lbDllVer;
    TLabel *lbUPH;
    TLabel *lbVer;
    TPanel *pnDryRun;
        TPanel *pnAfterMark;
    void __fastcall btOperationShowClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btExitClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btUtilShowClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall lbNameDblClick(TObject *Sender);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall pnLotChipUpehClick(TObject *Sender);
private:	// User declarations
//    CDelayTimer m_tmTemp    ; //Probe Setting Delay

//TODO :: ??    BEGIN_MESSAGE_MAP
//    END_MESSAGE_MAP(TForm)

public:		// User declarations
    __fastcall TFrmMain(TComponent* Owner);

    // InitÇÔ¼ö
    void __fastcall Init(void);

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
