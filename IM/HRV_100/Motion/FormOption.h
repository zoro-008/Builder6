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
        TBitBtn *btSave;
        TPanel *Panel7;
        TPanel *Panel8;
        TButton *btVisnRsltFile1;
        TEdit *edDataSavePath;
        TLabel *Label2;
        TCheckBox *cbAutoSaveRsltData;
        TCheckBox *cbAutoSaveRsltImg;
        TCheckBox *cbAutoSaveRsltSc;
        TCheckBox *cbUseStage1;
        TCheckBox *cbUseStage2;
        TCheckBox *cbUseStage3;
        TCheckBox *cbUseStage4;
        TCheckBox *cbUseStage5;
        TLabel *lbRegiGain;
        TEdit *edRegiGain;
        TLabel *lbRegiofset;
        TEdit *edRegiofset;
        TEdit *edVisnYMaxDpMin;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TEdit *edCrntGain;
        TEdit *edCrntOfst;
        TLabel *Label8;
        TLabel *Label9;
        TEdit *edHghtGain;
        TEdit *edHghtOfst;
        TCheckBox *cbDoubleHold;
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall btVisnRsltFile1Click(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateComOptn(bool _bToTable);
public:		// User declarations
    __fastcall TFrmOption(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOption *FrmOption;
//---------------------------------------------------------------------------
#endif



   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   

    
    


    

    
    
    
    
    
    
    
    
    

