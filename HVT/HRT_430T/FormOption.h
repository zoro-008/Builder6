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

//---------------------------------------------------------------------------
class TFrmOption : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel3;
    TCheckBox *cbDryRun;
    TPanel *Panel5;
    TImage *Image2;
    TLabel *Label19;
        TBitBtn *btSave;
    TCheckBox *cbIgnrDoor;
    TCheckBox *cbIgnrInsp;
    TCheckBox *cbIgnrIdCheck;
    TCheckBox *cbIgnrHeatAlarm;
        TCheckBox *cbIgnrStrOverAlm;
        TCheckBox *cbAutoOperation;
        TCheckBox *cbAutoConvesion;
        TCheckBox *cbLoadingStop;
    TCheckBox *cbHeatFail;
    TEdit *edLimAllFailCnt;
    TCheckBox *cbSortSersor;
    TEdit *edSortSensorDelay;
        TPanel *Panel1;
        TPanel *Panel2;
        TImage *Image1;
        TLabel *Label1;
        TCheckBox *cbIgnrTrim;
        TCheckBox *cbIgnrDecChk;
        TCheckBox *cbIgnrVacuum;
        TCheckBox *cbIgnrTrimDecChk;
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



   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   

    
    


    

    
    
    
    
    
    
    
    
    

