//---------------------------------------------------------------------------

#ifndef FrmMainH
#define FrmMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------

class TFormMain : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *pnMain;
    TPanel *Panel2;
        TTimer *tmUpdate;
        TPanel *plPassWord;
        TPanel *Panel19;
        TLabel *Label17;
        TBitBtn *btPasswordClose;
        TBitBtn *btEngr;
        TBitBtn *btOper;
        TBitBtn *btMast;
        TImage *Image1;
        TLabel *lbMode;
        TLabel *lbUPH;
        TLabel *lbDevice;
        TLabel *lbDllAge;
        TLabel *lbDllVer;
        TLabel *lbStat;
        TLabel *Label1;
        TPanel *pnLotChipUpeh;
        TImage *Image2;
        TButton *btnMainMenu;
        TPanel *btnReset;
        TPanel *pnReset;
        TPanel *btnAuto;
        TPanel *btnManual;
        TPanel *pnManual;
        TPanel *btnRun;
        TPanel *pnRun;
        TPanel *btnStop;
        TPanel *pnStop;
        TButton *btnDevSet;
        TButton *btnLotApp;
        TButton *btnLotInfo;
        TBitBtn *btOperator;
        TImage *imAuto;
        TPanel *pnAuto;
        TImage *imManual;
        TImage *imRun;
        TImage *imStop;
        TImage *imReset;
        TImage *Image3;
        TButton *btnMachine;
        TButton *btnExit;
        TLabel *Label2;
    void __fastcall btnRunClick(TObject *Sender);
    void __fastcall btnStopClick(TObject *Sender);
    void __fastcall btnResetClick(TObject *Sender);
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnMenuClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall btnLotAppClick(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall btOperatorClick(TObject *Sender);
        void __fastcall btOperClick(TObject *Sender);
        void __fastcall btPasswordCloseClick(TObject *Sender);
        void __fastcall lbModeClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall pnLotChipUpehClick(TObject *Sender);
        void __fastcall btnRunMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnRunMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall btnStopMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnStopMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnResetMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnResetMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall imMenuClick(TObject *Sender);
        void __fastcall pnMenuClick(TObject *Sender);
private:	// User declarations
    // Page show
    int   m_iPageIndex;
    bool  m_bReset ;
public:		// User declarations
    __fastcall TFormMain(TComponent* Owner);

    void __fastcall   ShowPage  (int Index) ;
    void __fastcall   ClosePage (void     ) ;

    void __fastcall   YouDie() ;

};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif

