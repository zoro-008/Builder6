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
    TImage *Image2;
    TLabel *Label19;
        TBitBtn *btSave;
    TCheckBox *cbIgnrDoor;
    TCheckBox *cbSkipLaserWrk;
    TPanel *Panel1;
    TPanel *Panel2;
    TImage *Image1;
    TLabel *Label1;
        TEdit *edVsnInspCnt;
    TLabel *Label4;
    TRadioGroup *rgUseLsrPos;
        TEdit *edAirBlowTm;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *edAlignTm;
        TLabel *Label5;
        TLabel *Label6;
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



   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   

    
    


    

    
    
    
    
    
    
    
    
    

