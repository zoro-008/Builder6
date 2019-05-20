//---------------------------------------------------------------------------

#ifndef FormCamParaH
#define FormCamParaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmCamPara : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnBase;
        TPanel *Panel1;
        TButton *btSave;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btSaveClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmCamPara(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmCamPara *FrmCamPara;
//---------------------------------------------------------------------------
#endif
