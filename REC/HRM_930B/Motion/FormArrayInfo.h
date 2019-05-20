//---------------------------------------------------------------------------

#ifndef FormArrayInfoH
#define FormArrayInfoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include "Chip.h"
//---------------------------------------------------------------------------
class TFrmArrayInfo : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *sgInfo;
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmArrayInfo(TComponent* Owner);

        //Array Info
        String  m_sArayName    ;
        String  m_sArayLotNo   ;
        String  m_sArayID      ;
        int     m_iArayStep    ;
        int     m_iAraySubStep ;


        //Chip Info
        String  m_sChipID      ;
        int     m_iChipStat    ; //Chip Status.

        int     m_iCol         ;
        int     m_iRow         ;

        void   UpdateVal();


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmArrayInfo *FrmArrayInfo;
//---------------------------------------------------------------------------
#endif
