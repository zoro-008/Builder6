//---------------------------------------------------------------------------

#ifndef FormTestConH
#define FormTestConH
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
class TFrmTestCon : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel3;
    TPanel *Panel5;
        TBitBtn *btSave;
        TCheckBox *cbUseTiltFull;
        TCheckBox *cbUseTiltInit;
        TCheckBox *cbUseTiltDrive;
        TPanel *Panel7;
        TPanel *Panel8;
        TLabel *Label7;
        TEdit *edStableDelay;
        TLabel *Label8;
        TLabel *Label9;
        TEdit *edStepDelay;
        TLabel *Label10;
        TLabel *Label11;
        TEdit *edStepIncrement;
        TLabel *Label12;
        TEdit *edMinTiltInit;
        TEdit *edMaxTiltInit;
        TLabel *Label15;
        TLabel *Label16;
        TLabel *Label17;
        TEdit *edMinTiltFull;
        TEdit *edMaxTiltFull;
        TLabel *Label18;
        TEdit *edMinTiltDrive;
        TEdit *edMaxTiltDrive;
        TLabel *Label19;
        TPanel *Panel9;
        TLabel *Label20;
        TLabel *Label21;
        TLabel *Label22;
        TLabel *Label23;
        TLabel *Label24;
        TCheckBox *cbUsePreLoad;
        TPanel *Panel10;
        TCheckBox *cbUsePeakStroke;
        TCheckBox *cbUseCustomStroke;
        TComboBox *cbFailIOST;
        TEdit *edMinPreLoad;
        TEdit *edMaxPreLoad;
        TEdit *edMinPeakStroke;
        TEdit *edMaxPeakStroke;
        TEdit *edMinCustomStroke;
        TEdit *edMaxCustomStroke;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TCheckBox *cbUseSensitivity;
        TCheckBox *cbUseImpedance;
        TCheckBox *cbUseHysteresis;
        TEdit *edMinSensitivity;
        TEdit *edMaxSensitivity;
        TEdit *edMinImpedance;
        TEdit *edMaxImpedance;
        TEdit *edMinHysteresis;
        TEdit *edMaxHysteresis;
        TLabel *lbDrvTtTestCrnt;
        TLabel *lbDrvTtTestUnit;
        TEdit *edDrvTtTestCrnt;
        TLabel *Label6;
        TLabel *Label25;
        TCheckBox *cbUseTiltInitR;
        TCheckBox *cbUseTiltInitT;
        TEdit *edMinTiltInitR;
        TEdit *edMaxTiltInitR;
        TEdit *edMinTiltInitT;
        TEdit *edMaxTiltInitT;
        TLabel *Label26;
        TLabel *Label27;
        TEdit *edCustomStroke;
        TLabel *Label28;
        TCheckBox *cbUseTiltMax;
        TEdit *edMinTiltMax;
        TEdit *edMaxTiltMax;
        TLabel *lbPreLoadCheckHt;
        TLabel *Label30;
        TEdit *edPreLoadCheckHt;
        TLabel *lbImpdCheckCrnt;
        TLabel *Label32;
        TEdit *edImpdCheckCrnt;
        TEdit *edSensiSttCrnt;
        TLabel *lbSensiSttUnit;
        TLabel *lbSensiSttCrnt;
        TEdit *edSensiEndCrnt;
        TLabel *lbSensiEndUnit;
        TLabel *lbSensiEndCrnt;
        TCheckBox *cbUseInitBfCtt;
        TLabel *Label37;
        TCheckBox *cbUseTiltDriveAbs;
        TEdit *edMinTiltDriveAbs;
        TEdit *edMaxTiltDriveAbs;
        TTimer *tmUpdate;
        TLabel *Label39;
        TLabel *Label40;
        TLabel *Label41;
        TLabel *Label42;
        TEdit *edGraphMinY;
        TEdit *edGraphMaxY;
        TPanel *Panel1;
        TPanel *Panel2;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label38;
        TLabel *lbTestCrntMin;
        TLabel *lbTestCrntMinUnit;
        TEdit *edTestCrntMax;
        TComboBox *cbUseDuplex;
        TEdit *edTestCrntMin;
        TLabel *lbDrvTtStartCrnt;
        TLabel *lbDrvTtStartUnit;
        TEdit *edDrvTtStrtCrnt;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label33;
        TLabel *Label34;
        TEdit *edLinearSttCrnt;
        TEdit *edLinearEndCrnt;
        TLabel *um;
        TCheckBox *cbUseLinearity;
        TEdit *edMinLinearity;
        TEdit *edMaxLinearity;
        TCheckBox *cbUseCaseShort;
        TLabel *lbHysteSttUnit;
        TLabel *lbHysteSttCrnt;
        TLabel *lbHysteEndUnit;
        TLabel *lbHysteEndCrnt;
        TEdit *edHysteSttCrnt;
        TEdit *edHysteEndCrnt;
        TLabel *Label35;
        TCheckBox *cbUseTiltLength;
        TEdit *edMinTiltLength;
        TEdit *edMaxTiltLength;
        TLabel *Label36;
        TEdit *edPreHysteMaxOfst;
        TLabel *Label43;
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall tmUpdateTimer(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateDevOptn(bool _bToTable);
public:		// User declarations
    __fastcall TFrmTestCon(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmTestCon *FrmTestCon;
//---------------------------------------------------------------------------
#endif



   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   

    
    


    

    
    
    
    
    
    
    
    
    

