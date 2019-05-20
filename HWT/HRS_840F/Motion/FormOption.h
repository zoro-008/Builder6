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

//---------------------------------------------------------------------------
class TFrmOption : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel3;
    TPanel *Panel5;
        TBitBtn *btSave;
        TCheckBox *cbLoadingStop;
        TPanel *Panel1;
        TPanel *Panel2;
        TLabel *Label3;
        TEdit *edFirstAirBlowTime;
        TLabel *Label4;
        TEdit *edSecondAirBlowTime;
        TLabel *Label5;
        TEdit *edPickVccDely;
        TLabel *Label6;
        TEdit *edPackOpenVccDely;
        TLabel *Label7;
        TEdit *edPickEjctDely;
        TLabel *Label8;
        TEdit *edPackEjctDely;
        TCheckBox *cbFRunSkip;
        TCheckBox *cbRRunSkip;
        TLabel *Label9;
        TEdit *edLdrPickDely;
        TLabel *Label2;
        TEdit *edUldMskDetSnrDely;
        TLabel *Label10;
        TEdit *edBin1FullMaxCnt;
        TLabel *Label11;
        TEdit *edBin2FullMaxCnt;
        TLabel *Label1;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label15;
        TLabel *Label16;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label21;
        TEdit *edPickBwBfrDely;
        TLabel *Label22;
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



   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   

    
    


    

    
    
    
    
    
    
    
    
    

