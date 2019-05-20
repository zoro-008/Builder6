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
    TCheckBox *cbIgnrDoor;
        TCheckBox *cbLoadingStop;
        TCheckBox *cbSkipVT;
        TCheckBox *cbSkipVSN1;
        TCheckBox *cbSkipOST;
        TCheckBox *cbSkipVSN2;
        TPanel *pnRejOpotn;
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
        TCheckBox *cbOSTNotUseCrnt;
        TPanel *Panel9;
        TPanel *Panel10;
        TCheckBox *cbVisnAutoConv;
        TLabel *Label13;
        TLabel *Label14;
        TEdit *edVSContNGCnt;
        TPanel *Panel11;
        TLabel *Label19;
        TLabel *Label20;
        TPanel *Panel12;
        TEdit *edOSContNGCnt;
        TPanel *Panel13;
        TLabel *Label15;
        TLabel *Label16;
        TPanel *Panel14;
        TEdit *edStgAftStopDelay;
        TLabel *Label51;
        TComboBox *cbFailICrnt;
        TLabel *Label52;
        TComboBox *cbFailIDFT2;
        TButton *btRejectOptn;
        TLabel *Label41;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *Label21;
        TLabel *Label22;
        TEdit *edOSTLogDays;
        TCheckBox *cbOSTNotUseOst;
        TLabel *Label23;
        TLabel *Label24;
        TEdit *edVTContDftCnt;
        TLabel *Label25;
        TLabel *Label26;
        TEdit *edVTContVHCnt;
        TLabel *Label27;
        TLabel *Label28;
        TEdit *edVTContDft2Cnt;
        TLabel *Label29;
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall btRejectOptnClick(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateComOptn(bool _bToTable);
public:		// User declarations
    __fastcall TFrmOption(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOption *FrmOption;
//---------------------------------------------------------------------------
#endif



   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   

    
    


    

    
    
    
    
    
    
    
    
    

