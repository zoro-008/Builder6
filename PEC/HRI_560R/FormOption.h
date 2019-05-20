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
    TGroupBox *GroupBox56;
    TLabel *Label85;
    TLabel *Label6;
    TLabel *Label16;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label14;
    TEdit *edWK2StprDnDelay;
    TEdit *edPSB_VisionTime;
    TCheckBox *cbAutoVsChange;
    TEdit *edPrbToWk1Delay;
    TEdit *edPsbToOutDelay;
    TCheckBox *cbAfterMrkRemove;
    TCheckBox *cbAutoReview;
    TGroupBox *GroupBox1;
    TCheckBox *cbVision1Skip;
    TCheckBox *cbVision2Skip;
    TCheckBox *cbVision3Skip;
    TLabel *Label1;
    TLabel *Label2;
        TEdit *edPSBReject;
        TLabel *Label12;
        TEdit *edLDRInDeley;
        TLabel *Label13;
    TLabel *Label11;
    TGroupBox *GroupBox2;
    TLabel *Label15;
    TLabel *Label17;
    TLabel *Label18;
    TLabel *Label19;
    TCheckBox *cbUsePenMrkMvY;
    TEdit *edPenMrkMvY;
    TCheckBox *cbUsePenMrkMvX;
    TEdit *edPenMrkMvX;
    TGroupBox *GroupBox3;
    TLabel *Label10;
    TEdit *edLDR_EmptyCnt;
    TLabel *Label20;
    TCheckBox *cbUseLDREmpty;
    TCheckBox *cbVision4Skip;
    TGroupBox *GroupBox4;
    TLabel *Label3;
    TLabel *Label4;
    TEdit *edULD_VaccumDeley;
    TBitBtn *btSave;
    TLabel *Label5;
    TLabel *Label21;
    TEdit *edUldPkgEjtTime;
    TLabel *Label22;
    TLabel *Label23;
    TEdit *edUldPprEjtTime;
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateCmmOptn(bool _bToTable);
public:		// User declarations
    __fastcall TFrmOption(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOption *FrmOption;
//---------------------------------------------------------------------------
#endif



   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   

    
    


    

    
    
    
    
    
    
    
    
    

