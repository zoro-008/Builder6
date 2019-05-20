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
        TCheckBox *cbIgnrTrimDecChk;
        TCheckBox *cbIgnrDrop;
        TCheckBox *cbUsedInsOptn;
        TPanel *pnOption;
        TCheckBox *cbUsedTopViewPnch;
        TPanel *Panel4;
        TPanel *Panel6;
        TImage *Image3;
        TCheckBox *cbBin1;
        TCheckBox *cbBin2;
        TCheckBox *cbBin3;
        TCheckBox *cbBin4;
        TCheckBox *cbBin5;
        TCheckBox *cbBin6;
        TCheckBox *cbBin7;
        TCheckBox *cbBin8;
        TCheckBox *cbBin9;
        TCheckBox *cbBin10;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TImage *Image4;
        TLabel *Label6;
        TEdit *edMinVf1;
        TEdit *edMinVf2;
        TEdit *edMinVf3;
        TEdit *edMinVf4;
        TEdit *edMinVf5;
        TEdit *edMinVf6;
        TEdit *edMinVf7;
        TEdit *edMinVf8;
        TEdit *edMinVf9;
        TEdit *edMinVf10;
        TEdit *edMaxVf1;
        TEdit *edMaxVf2;
        TEdit *edMaxVf3;
        TEdit *edMaxVf4;
        TEdit *edMaxVf5;
        TEdit *edMaxVf6;
        TEdit *edMaxVf7;
        TEdit *edMaxVf8;
        TEdit *edMaxVf9;
        TEdit *edMaxVf10;
        TEdit *edMinVr1;
        TEdit *edMinVr2;
        TEdit *edMinVr3;
        TEdit *edMinVr4;
        TEdit *edMinVr5;
        TEdit *edMinVr6;
        TEdit *edMinVr7;
        TEdit *edMinVr8;
        TEdit *edMinVr9;
        TEdit *edMinVr10;
        TEdit *edMaxVr1;
        TEdit *edMaxVr2;
        TEdit *edMaxVr3;
        TEdit *edMaxVr4;
        TEdit *edMaxVr5;
        TEdit *edMaxVr6;
        TEdit *edMaxVr7;
        TEdit *edMaxVr8;
        TEdit *edMaxVr9;
        TEdit *edMaxVr10;
        TPanel *Panel7;
        TPanel *Panel8;
        TImage *Image5;
        TLabel *Label7;
        TCheckBox *cbSkipPinCnt;
        TLabel *Label47;
        TButton *btSetBinCnt;
        TEdit *edRjtBinNoInput;
        TEdit *edRejectBinNo;
        TEdit *edBinCount1;
        TEdit *edBinCount2;
        TEdit *edBinCount10;
        TEdit *edBinCount9;
        TEdit *edBinCount8;
        TEdit *edBinCount7;
        TEdit *edBinCount6;
        TEdit *edBinCount5;
        TEdit *edBinCount4;
        TEdit *edBinCount3;
        TLabel *Label8;
        TTimer *tmOption;
        TCheckBox *cbSkipYieldErr;
        TEdit *edStandardYield;
        TLabel *Label9;
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall btSetBinCntClick(TObject *Sender);
        void __fastcall tmOptionTimer(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateCmnOptn(bool _bToTable);
public:		// User declarations
    __fastcall TFrmOption(TComponent* Owner);
    void InitArrayPanel();

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOption *FrmOption;
//---------------------------------------------------------------------------
#endif



   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   

    
    


    

    
    
    
    
    
    
    
    
    

