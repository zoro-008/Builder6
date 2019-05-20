//---------------------------------------------------------------------------

#ifndef FormTestSetH
#define FormTestSetH
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
class TFrmTestSet : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel3;
    TPanel *Panel5;
        TBitBtn *btSave;
    TCheckBox *cbIgnrDoor;
        TCheckBox *cbLoadingStop;
        TCheckBox *cbSkipVT;
        TCheckBox *cbSkipVSN1;
        TCheckBox *cbSkipOST;
        TCheckBox *cbSkipVSN2;
        TPanel *Panel1;
        TPanel *Panel2;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TComboBox *cbFailIOST;
        TComboBox *cbFailIDFT;
        TComboBox *cbFailIDIM;
        TComboBox *cbFailIBLK;
        TComboBox *cbFailIVH;
        TComboBox *cbFailIVSN;
        TPanel *Panel4;
        TPanel *Panel6;
        TCheckBox *cbSkip2ndTol;
        TCheckBox *cbSkip1stTol;
        TCheckBox *cbSkip4thTol;
        TCheckBox *cbSkip3thTol;
        TPanel *Panel7;
        TPanel *Panel8;
        TLabel *Label7;
        TEdit *edBfrVTPowrDelay;
        TLabel *Label8;
        TLabel *Label9;
        TEdit *edBfrVTInspDelay;
        TLabel *Label10;
        TLabel *Label11;
        TEdit *edVTContBlkCnt;
        TLabel *Label12;
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateComOptn(bool _bToTable);
public:		// User declarations
    __fastcall TFrmTestSet(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmTestSet *FrmTestSet;
//---------------------------------------------------------------------------
#endif



   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   

    
    


    

    
    
    
    
    
    
    
    
    

