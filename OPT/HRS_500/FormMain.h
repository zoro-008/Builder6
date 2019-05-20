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
//Rs232 Header Reference( CallBackReg Need )
//---------------------------------------------------------------------------
//#include "uRs232c.h"
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
        TImage *imTileBar;
    TLabel *lbStat;
    TLabel *Label6;
    TLabel *lbDevice;
    TTimer *tmUpdate;
    TLabel *lbName;
        TPanel *pnLoadingStop;
    TPanel *pnIgnoreChuckFail;
        TPanel *pnEmptyIgnr;
        TPanel *pnNoTrim;
        TBitBtn *btSpcShow;
    TLabel *lbVer;
    TPanel *pnUseFullBinIgnore;
        TLabel *lbDate;
        TLabel *lbTime;
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
    void __fastcall HeatProbe1Ch(TMessage &Message);
    void __fastcall HeatProbe2Ch(TMessage &Message);


    BEGIN_MESSAGE_MAP
//        MESSAGE_HANDLER(UM_EVN_RXCHAR_PB1  , TMessage, HeatProbe1Ch )
//        MESSAGE_HANDLER(UM_EVN_RXCHAR_PB2  , TMessage, HeatProbe2Ch )
    END_MESSAGE_MAP(TForm)

public:		// User declarations
    __fastcall TFrmMain(TComponent* Owner);

    // InitÇÔ¼ö
    void __fastcall Init(void);
    void __fastcall InitRs232(void);
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
//TRS232C *RS232C1 ;
//TRS232C *RS232C2 ;
//TRS232C *RS232C3 ;
//---------------------------------------------------------------------------
#endif
