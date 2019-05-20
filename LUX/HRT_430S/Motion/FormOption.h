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
    TPanel *Panel5;
    TImage *Image2;
    TLabel *Label19;
        TBitBtn *btSave;
    TCheckBox *cbIgnrDoor;
    TCheckBox *cbIgnrInsp;
        TCheckBox *cbVisionSkip;
    TCheckBox *cbIgnrHeatAlarm;
        TCheckBox *cbIgnrStrOverAlm;
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
        TPanel *Panel4;
        TPanel *Panel6;
        TImage *Image3;
        TLabel *Label2;
        TPanel *Panel7;
        TPanel *Panel8;
        TImage *Image4;
        TLabel *Label3;
        TCheckBox *cbDryRun;
        TLabel *Label4;
        TEdit *edLedFailBin;
        TLabel *Label5;
        TEdit *edZnnrFailBin;
        TLabel *Label6;
        TEdit *edDropFailBin;
        TLabel *Label7;
        TEdit *edAllFailBin;
        TLabel *Label8;
        TEdit *edLowVfBin;
        TLabel *Label9;
        TEdit *edHighVfBin;
        TEdit *edLowVf;
        TEdit *edHighVf;
        TTimer *Timer1;
        TEdit *edBinCount2;
        TEdit *edBinCount4;
        TEdit *edBinCount3;
        TEdit *edBinCount1;
        TEdit *edBinCount6;
        TEdit *edBinCount5;
        TPanel *Panel9;
        TPanel *Panel10;
        TImage *Image5;
        TLabel *Label16;
        TLabel *Label15;
        TEdit *edMESPath;
        TButton *btServerFolder;
        TPanel *Panel11;
        TPanel *Panel12;
        TImage *Image6;
        TLabel *Label18;
        TRadioGroup *rgOperator;
        TEdit *edUser1;
        TEdit *edUser7;
        TEdit *edUser2;
        TEdit *edUser3;
        TEdit *edUser4;
        TEdit *edUser5;
        TEdit *edUser6;
        TEdit *edUser8;
        TEdit *edUser9;
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall btServerFolderClick(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateComOptn(bool _bToTable);
public:		// User declarations
    __fastcall TFrmOption(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOption *FrmOption;
//---------------------------------------------------------------------------
#endif



   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   

    
    


    

    
    
    
    
    
    
    
    
    

