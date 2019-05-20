//---------------------------------------------------------------------------

#ifndef FrmReviewSelectH
#define FrmReviewSelectH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "Utildefine.h"
//---------------------------------------------------------------------------
class TFormReviewSelect : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *btYes;
        TBitBtn *btNo;
        TPanel *pnVb4;
        void __fastcall btNoClick(TObject *Sender);
        void __fastcall btYesClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormReviewSelect(TComponent* Owner);
        bool __fastcall WriteArayData(EN_ARAY_ID riId);
        bool __fastcall ReadArayData (EN_ARAY_ID riId);

};
//---------------------------------------------------------------------------
extern PACKAGE TFormReviewSelect *FormReviewSelect;
//---------------------------------------------------------------------------
#endif
