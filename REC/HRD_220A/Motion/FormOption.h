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
#include <ComCtrls.hpp>
#include <Dialogs.hpp>

//---------------------------------------------------------------------------
class TFrmOption : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel3;
    TPanel *Panel5;
        TBitBtn *btSave;
    TCheckBox *cbIgnrDoor;
        TCheckBox *cbLoadingStop;
        TPanel *Panel1;
        TPanel *Panel2;
        TEdit *edDisprClnCnt;
        TLabel *Label1;
        TCheckBox *cbVisnNgStop;
        TPanel *Panel4;
        TPanel *Panel6;
        TCheckBox *cbTempErrorChk;
        TLabel *Label63;
        TLabel *Label64;
        TEdit *edTempErrorTol;
        TPanel *Panel7;
        TPanel *Panel8;
        TLabel *Label24;
        TEdit *edMaxPlaceZOfs2;
        TLabel *Label23;
        TLabel *Label13;
        TEdit *edMaxPlaceZOfs1;
        TLabel *Label14;
        TLabel *Label19;
        TEdit *edMaxAttachForce;
        TLabel *Label20;
        TLabel *lbVer;
        TLabel *lbDate;
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



   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   

    
    


    

    
    
    
    
    
    
    
    
    

