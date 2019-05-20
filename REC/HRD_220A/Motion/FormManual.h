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
        TBitBtn *btSTGVccm;
        TBitBtn *btSTGEjct;
        TBitBtn *btPCKEjct;
        TBitBtn *btPCKVccmOn;
        TBitBtn *btMan3_1;
        TBitBtn *btMan3_2;
        TBitBtn *btMan3_7;
        TBitBtn *btMan3_3;
        TBitBtn *btMan3_4;
        TBitBtn *btMan3_5;
        TBitBtn *btMan3_6;
        TBitBtn *btMan3_8;
        TBitBtn *btMan3_9;
        TBitBtn *btMan3_10;
        TBitBtn *btMan3_11;
        TBitBtn *btMan3_12;
        TBitBtn *btMan3_13;
        TBitBtn *btMan3_14;
        TBitBtn *btMan4_1;
        TBitBtn *btMan4_2;
        TBitBtn *btMan4_7;
        TBitBtn *btMan4_3;
        TBitBtn *btMan4_4;
        TBitBtn *btMan4_5;
        TBitBtn *btMan4_6;
        TBitBtn *btMan4_8;
        TBitBtn *btMan4_9;
        TBitBtn *btMan4_10;
        TBitBtn *btMan4_11;
        TBitBtn *btMan4_12;
        TBitBtn *btMan4_13;
        TBitBtn *btMan4_14;
        TBitBtn *btPCKVccmOff;
        void __fastcall btSTGVccmClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall btMan3_1Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmManual(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmManual *FrmManual;
//---------------------------------------------------------------------------
#endif



   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   

    
    


    

    
    
    
    
    
    
    
    
    

