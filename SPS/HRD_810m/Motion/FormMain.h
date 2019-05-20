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
//#include <IdBaseComponent.hpp>
//#include <IdComponent.hpp>
//#include <IdIcmpClient.hpp>
//#include <IdRawBase.hpp>
//#include <IdRawClient.hpp>
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
    TPanel *pnIgnreDoor;
        TPanel *pnUseBarcode;
    TPanel *pnNoTrim;
    TBitBtn *btSpcShow;
    TLabel *lbVer;
    TPanel *pnOption4;
    TLabel *lbDate;
    TLabel *lbTime;
        TLabel *Label1;
        TLabel *lbWaferName;
        TButton *Button1;
        TLabel *lbCalPitch;
    void __fastcall btOperationShowClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btExitClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall lbNameDblClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall lbVerDblClick(TObject *Sender);
    void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations

public:		// User declarations
    __fastcall TFrmMain(TComponent* Owner);

    // Init함수
    void __fastcall Init(void);

    // Page show
    int m_iPageIndex;
    // Page show Func.
    void __fastcall ShowPage     (int index);
    void __fastcall ClosePage    (int index);


    //쓰레드에서 쓸수 있게 메세지 처리함.
    void __fastcall WmUser(TMessage &Message);
    //Message Handler.
    BEGIN_MESSAGE_MAP
        MESSAGE_HANDLER(WM_USER, TMessage, WmUser)
    END_MESSAGE_MAP(TForm)

};
AnsiString OnGetVer(const AnsiString &sQuery);
//---------------------------------------------------------------------------
extern PACKAGE TFrmMain *FrmMain;

//씨퀜스에서 써도 되는 함수들...
//쓰레드에서 쓸수 있게 메세지 처리함.
enum EN_MSG_CMD {
    mcCapAndShowSec = 0 ,


};
void SendCmdEndVisn   (EN_MSG_CMD _iCmd);
void GrabEndVisn      ();
bool GetGrabEndEndVisn();
bool SaveImageEndVisn (String _sFilePath);
//---------------------------------------------------------------------------
#endif
