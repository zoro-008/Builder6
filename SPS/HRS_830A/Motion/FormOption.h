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
//#include "TEdit.h" //  에러가 떠서...JH
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
        TCheckBox *cbIgnrDispsr;
        TCheckBox *cbEmptyStrpCnt;
        TEdit *edEmptyStrpCnt;
        TLabel *Label1;
        TCheckBox *cbSkipDspsSnsr;
        TGroupBox *GroupBox1;
        TEdit *edExpiryDay;
        TLabel *Label3;
        TButton *btLiquidReset;
        TEdit *edSettingDay;
        TLabel *Label4;
        TCheckBox *cbIngrDsprExpr;
        TEdit *edDisprCleanCnt;
        TLabel *Label5;
        TLabel *Label6;
        TOpenDialog *odLoadOnlyOneMapPath;
        TLabel *Label7;
        TGroupBox *GroupBox2;
        TEdit *edServerPath;
        TButton *btServerFolder;
        TLabel *Label2;
        TLabel *Label8;
        TEdit *edStopToShotTime;
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall btLiquidResetClick(TObject *Sender);
        void __fastcall btServerFolderClick(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateComOptn(bool _bToTable);
public:		// User declarations
    __fastcall TFrmOption(TComponent* Owner);
    TDateTime SetDay;  //액체 설정 기간 표시.
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOption *FrmOption;
//---------------------------------------------------------------------------
#endif



   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   

    
    


    

    
    
    
    
    
    
    
    
    

