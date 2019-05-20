//---------------------------------------------------------------------------

#ifndef FormManualH
#define FormManualH
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
class TFrmManual : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnManual;
    TPanel *Panel5;
        TTimer *tmUpdate;
        TBitBtn *btMan1_1;
        TBitBtn *btMan1_7;
        TBitBtn *btMan2_1;
        TBitBtn *btMan2_2;
        TBitBtn *btMan2_3;
        TBitBtn *btMan2_4;
        TBitBtn *btMan2_7;
        TBitBtn *btMan3_1;
        TBitBtn *btMan3_7;
        TBitBtn *btMan4_1;
        TBitBtn *btMan4_2;
        TBitBtn *btMan4_7;
        TBitBtn *btMan5_7;
        TBitBtn *btMan5_1;
        TBitBtn *btMan4_11;
        TBitBtn *btMan2_12;
        TBitBtn *btMan4_12;
        TBitBtn *btMan2_13;
        TBitBtn *btMan4_13;
        TBitBtn *btMan5_13;
        TBitBtn *btMan2_14;
        TBitBtn *btMan4_14;
        TBitBtn *btMan5_14;
        TBitBtn *btMan6_9;
        TBitBtn *btMan6_8;
        TBitBtn *btMan6_7;
        TBitBtn *btMan6_6;
        TBitBtn *btMan6_5;
        TBitBtn *btMan6_4;
        TBitBtn *btMan6_3;
        TBitBtn *btMan6_2;
        TBitBtn *btMan6_1;
        TBitBtn *btMan6_11;
        TBitBtn *btMan6_12;
        TBitBtn *btMan6_13;
        TBitBtn *btMan6_14;
        TBitBtn *btMan6_10;
        TBitBtn *btMan4_10;
        TBitBtn *btMan4_9;
        TBitBtn *btMan5_12;
        TBitBtn *btMan5_11;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall btMan1_8Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmManual(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmManual *FrmManual;
//---------------------------------------------------------------------------
#endif



   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   

    
    


    

    
    
    
    
    
    
    
    
    

