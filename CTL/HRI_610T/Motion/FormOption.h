//---------------------------------------------------------------------------

#ifndef FormOptionH
#define FormOptionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "TEdit.h"

//---------------------------------------------------------------------------
class TFrmOption : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel3;
    TPanel *Panel5;
        TBitBtn *btSave;
    TCheckBox *cbIgnrDoor;
        TCheckBox *cbSkipLaser;
        TCheckBox *cbLoadingStop;
    TPanel *Panel1;
    TPanel *Panel2;
        TCheckBox *cbSkipOriVisn;
        TCheckBox *cbSkipAlnVisn;
        TRadioGroup *rgMarkInsp;
        TCheckBox *cbFrstStrpOut;
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateComOptn(bool _bToTable);
public:		// User declarations
    __fastcall TFrmOption(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOption *FrmOption;
//---------------------------------------------------------------------------
#endif



   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   

    
    


    

    
    
    
    
    
    
    
    
    

