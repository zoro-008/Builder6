//---------------------------------------------------------------------------

#ifndef FormMain1cppH
#define FormMain1cppH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TFrmMain : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TImage *Image2;
        TLabel *lbDate;
        TLabel *lbStat;
        TLabel *Label6;
        TLabel *lbDevice;
        TLabel *lbName;
        TLabel *lbVer;
        TLabel *lbDllVer;
        TLabel *lbDllAge;
        TPanel *pnLoadingStop;
        TPanel *pnIgnreDoor;
        TPanel *pnDryRun;
        TPanel *pnNoTrim;
        TPanel *pnIgnrTrim;
        TPanel *pnInspSkip;
        TPanel *pnBase;
        TPanel *pnMenu;
        TBitBtn *BitBtn2;
        TBitBtn *BitBtn3;
        TBitBtn *BitBtn4;
        TBitBtn *BitBtn5;
        TBitBtn *BitBtn6;
        TBitBtn *BitBtn7;
private:	// User declarations
public:		// User declarations
        __fastcall TFrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMain *FrmMain;
//---------------------------------------------------------------------------
#endif
