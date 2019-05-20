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
#include <Dialogs.hpp>

//---------------------------------------------------------------------------
class TFrmOption : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel3;
    TPanel *Panel5;
        TBitBtn *btSave;
    TCheckBox *cbIgnrDoor;
        TCheckBox *cbFLoadingStop;
        TPanel *Panel1;
        TPanel *Panel2;
        TPanel *Panel7;
        TPanel *Panel8;
        TLabel *Label24;
        TEdit *edMaxPlaceZOfs2;
        TLabel *Label23;
        TLabel *Label13;
        TEdit *edMaxPlaceZOfs1;
        TLabel *Label14;
        TLabel *lbVer;
        TLabel *lbDate;
        TPanel *Panel4;
        TPanel *Panel6;
        TLabel *Label8;
        TComboBox *cbWorkMode;
        TCheckBox *cbRLoadingStop;
        TPanel *Panel9;
        TPanel *Panel10;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *edRightHeightDelay;
        TEdit *edRightVisnDelay;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label7;
        TEdit *edDisprClnTime;
        TLabel *Label1;
        TLabel *Label6;
        TLabel *Label9;
        TEdit *edRightHeightStepDelay;
        TLabel *Label10;
        TLabel *Label11;
        TEdit *edHexaPotRelDelay;
        TCheckBox *cbUseConfocal;
        TCheckBox *cbIgnrCmsREndAlign;
        TCheckBox *cbIgnrCmsREndHeight;
        TCheckBox *cbIgnrMidREndHeight;
        TCheckBox *cbIgnrCmsRFixHeight;
        TCheckBox *cbIgnrCmsRFixAlign;
        TLabel *Label12;
        TEdit *edTopUVLimitTime;
        TLabel *Label15;
        TEdit *edBtmUVLimitTime;
        TLabel *Label16;
        TLabel *Label17;
        TPanel *Panel11;
        TPanel *Panel12;
        TButton *Button1;
        TButton *Button2;
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateComOptn(bool _bToTable);
public:		// User declarations
    __fastcall TFrmOption(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOption *FrmOption;
//---------------------------------------------------------------------------
#endif



   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   

    
    


    

    
    
    
    
    
    
    
    
    

