//---------------------------------------------------------------------------

#ifndef FormCalParaH
#define FormCalParaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>

#include "VisnDefine.h"
#include <Buttons.hpp>
#include <Grids.hpp>
#include <ValEdit.hpp>




//---------------------------------------------------------------------------
class TFrmCalPara : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TBitBtn *btClose;
        TBitBtn *btSave;
        TPanel *pnBase;
        void __fastcall btCloseClick(TObject *Sender);
        void __fastcall btSaveClick(TObject *Sender);
private:	// User declarations
        EN_VISN_ID m_iVisnId ;
public:		// User declarations
        __fastcall TFrmCalPara(TComponent* Owner);

        void Init();

        void FormShow (EN_VISN_ID  _iVisnId);


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmCalPara *FrmCalPara;
//---------------------------------------------------------------------------
#endif
