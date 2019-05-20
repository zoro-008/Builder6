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
#include "FormOperation.h"
#include "FormDevice.h"
#include "FormDeviceSet.h"
#include "FormOption.h"

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
    TPanel *Panel1;
    TPanel *pnBase;
    TPanel *pnMenu;
    TImage *Image2;
    TLabel *lbStat;
    TTimer *tmUpdate;
    TLabel *lbName;
        TPanel *pnSkipDisp;
        TSpeedButton *btManualShow;
        TSpeedButton *btReportShow;
        TSpeedButton *btOptionShow;
        TSpeedButton *btUtilShow;
        TSpeedButton *btRecipeShow;
        TSpeedButton *sbOperationShow;
        TSpeedButton *btExit;
        TLabel *lbCrntTime;
        TLabel *lbDevice;
        TLabel *Label6;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btExitClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall lbNameDblClick(TObject *Sender);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall lbVerDblClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall btManualShowClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall lbStatClick(TObject *Sender);
private:	// User declarations

public:		// User declarations
    __fastcall TFrmMain(TComponent* Owner);
    __fastcall ~TFrmMain();

    // InitÇÔ¼ö
    void __fastcall Init(void);

    // Page show
    int m_iPageIndex;
    // Page show Func.
    void __fastcall ShowPage     (int index);
    void __fastcall ClosePage    (int index);

};
AnsiString OnGetVer(const AnsiString &sQuery);


extern PACKAGE TFrmMain *FrmMain;
//---------------------------------------------------------------------------
#endif
