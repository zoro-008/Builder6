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
    TLabel *lbDate;
    TLabel *lbStat;
    TLabel *Label6;
    TLabel *lbDevice;
    TTimer *tmUpdate;
    TLabel *lbName;
    TLabel *lbVer;
    TLabel *lbDllVer;
    TLabel *lbDllAge;
        TLabel *Label2;
        TLabel *lbLotNo;
        TBitBtn *btOperationShow;
        TBitBtn *btOptionShow;
        TBitBtn *btUtilShow;
        TBitBtn *BitBtn6;
        TBitBtn *btDeviceShow;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btExitClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall lbNameDblClick(TObject *Sender);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall lbVerDblClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall btOperationShowClick(TObject *Sender);
private:	// User declarations

public:		// User declarations
    __fastcall TFrmMain(TComponent* Owner);
    __fastcall ~TFrmMain();

    // Init�Լ�
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
