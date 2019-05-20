//---------------------------------------------------------------------------

#ifndef FormDisplayH
#define FormDisplayH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmDisplay : public TForm
{
__published:	// IDE-managed Components
        TTimer *tmUpdate;
        TPanel *Panel2;
        TPanel *Panel3;
        TPanel *Panel4;
        TPanel *Panel6;
        TPanel *Panel1;
        TPanel *Panel7;
        TPanel *Panel8;
        TPanel *Panel5;
        TPanel *Panel9;
        TPanel *pnLTotal;
        TPanel *pnLYield;
        TPanel *pnLAvr;
        TPanel *pnLAdj;
        TPanel *pnLPass;
        TPanel *pnLBond;
        TPanel *pnLWait;
        TPanel *pnLTotalTime;
        TPanel *pnLUv;
        TPanel *Panel19;
        TPanel *Panel20;
        TPanel *Panel21;
        TPanel *Panel22;
        TPanel *Panel23;
        TPanel *Panel24;
        TPanel *Panel25;
        TPanel *Panel26;
        TPanel *Panel27;
        TPanel *pnRTotal;
        TPanel *pnRYield;
        TPanel *pnRAvr;
        TPanel *pnRAdj;
        TPanel *pnRPass;
        TPanel *pnRBond;
        TPanel *pnRWait;
        TPanel *pnRTotalTime;
        TPanel *pnRUv;
        TPanel *Panel10;
        TPanel *pnLWork;
        TPanel *Panel12;
        TPanel *pnRWork;
        TTimer *tmSave;
        TButton *btClr;
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormDeactivate(TObject *Sender);
        void __fastcall Panel2DblClick(TObject *Sender);
        void __fastcall Panel19DblClick(TObject *Sender);
        void __fastcall tmSaveTimer(TObject *Sender);
        void __fastcall btClrClick(TObject *Sender);
private:	// User declarations
protected:
        void __fastcall CreateParams(TCreateParams &Params);  

public:		// User declarations
        __fastcall TFrmDisplay(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmDisplay *FrmDisplay;
//---------------------------------------------------------------------------
#endif
