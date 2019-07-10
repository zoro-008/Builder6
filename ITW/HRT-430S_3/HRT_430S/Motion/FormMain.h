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
//#include "FormUtil.h"

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
    TBitBtn *btOperationShow;
    TBitBtn *btDeviceShow;
    TBitBtn *btOptionShow;
    TBitBtn *btUtilShow;
    TBitBtn *btExit;
    TImage *Image2;
    TLabel *lbDate;
    TLabel *lbStat;
    TLabel *Label6;
    TLabel *lbDevice;
    TTimer *tmUpdate;
    TLabel *lbName;
        TTimer *tmMainSet;
        TTimer *tmSubSet;
        TPanel *pnLoadingStop;
        TPanel *pnIgnreDoor;
        TPanel *pnDryRun;
        TPanel *pnNoTrim;
    TBitBtn *BitBtn1;
    TLabel *lbVer;
    TPanel *pnIgnrTrim;
    TPanel *pnInspSkip;
    TLabel *lbDllVer;
    TLabel *lbDllAge;
        TTimer *tmTempuretue;
    void __fastcall btOperationShowClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btExitClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btUtilShowClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall lbNameDblClick(TObject *Sender);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall IdIIcmpReply(TComponent *ASender,
          const TReplyStatus &AReplyStatus);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall lbVerDblClick(TObject *Sender);
        void __fastcall tmTempuretueTimer(TObject *Sender);
private:	// User declarations
    void __fastcall HeatProbe1Ch(TMessage &Message);
    void __fastcall HeatProbe2Ch(TMessage &Message);

    CDelayTimer m_tmTemp    ; //Probe Setting Delay

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
