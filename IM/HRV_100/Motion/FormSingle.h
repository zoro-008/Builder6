//---------------------------------------------------------------------------
#ifndef FormSingleH
#define FormSingleH
#include <Classes.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TFrmSingle : public TForm
{
__published:	// IDE-managed Components
        TTimer *tmUpdate;
        TTimer *Timer1;
        TTimer *Timer2;
        TMainMenu *MainMenu1;
        TMenuItem *miOption;
        TMenuItem *ChangeTheModel1;
        TMenuItem *LoadTestModel1;
        TGroupBox *GroupBox1;
        TPanel *Panel1;
        TPanel *Panel2;
        TMenuItem *Close1;
        TGroupBox *GroupBox3;
        TStringGrid *sgTilt;
        TGroupBox *GroupBox2;
        TStringGrid *sgDriveTest;
        void __fastcall ChangeTheModel1Click(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormHide(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall Close1Click(TObject *Sender);

private:	// User declarations

public:		// User declarations
    __fastcall TFrmSingle(TComponent* Owner);


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmSingle *FrmSingle;
//---------------------------------------------------------------------------
#endif
