//---------------------------------------------------------------------------

#ifndef FormAlgoParaH
#define FormAlgoParaH
//---------------------------------------------------------------------------
#include "VisnDefine.h"
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
#include <ValEdit.hpp>





//---------------------------------------------------------------------------
class TFrmAlgoPara : public TForm
{
__published:	// IDE-managed Components
        TValueListEditor *vePara;
        TPanel *Panel1;
        TBitBtn *btClose;
        TBitBtn *btSave;
        void __fastcall btCloseClick(TObject *Sender);
        void __fastcall btSaveClick(TObject *Sender);
private:	// User declarations
        int m_iVisnId ;
public:		// User declarations
        __fastcall TFrmAlgoPara(TComponent* Owner);

        void Init();

        void FormShow (EN_VISN_ID _iVisnId);


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAlgoPara *FrmAlgoPara;
//---------------------------------------------------------------------------
#endif
