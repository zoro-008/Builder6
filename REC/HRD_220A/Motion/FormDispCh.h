//---------------------------------------------------------------------------

#ifndef FormDispChH
#define FormDispChH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFrmDispCh : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnChSetting;
        TLabel *Label46;
        TLabel *Label47;
        TLabel *Label48;
        TLabel *Label49;
        TEdit *edDspPrsPres;
        TEdit *edDspVacPres;
        TTimer *tmUpdate;
        TButton *btSetDspData;
        TBitBtn *btSigmaMode;
        TButton *btCheckAmount;
        TLabel *lbAmount;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall btSetDspDataClick(TObject *Sender);
        void __fastcall btSigmaModeClick(TObject *Sender);
        void __fastcall btCheckAmountClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmDispCh(TComponent* Owner);

        void UpdateDispData(bool _toTable);

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmDispCh *FrmDispCh;
//---------------------------------------------------------------------------
#endif
