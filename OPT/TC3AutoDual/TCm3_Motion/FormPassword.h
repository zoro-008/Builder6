//---------------------------------------------------------------------------

#ifndef FormPasswordH
#define FormPasswordH
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------

enum {lvOperator=0 , lvEngineer , lvMaster };
class TFrmPassword : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel_Change;
    TPanel *Panel2;
    TSpeedButton *SpeedButton1;
    TSpeedButton *SpeedButton2;
    TImage *Image3;
    TLabel *Label1;
    TEdit *Edit2;
    TLabel *Label6;
    TEdit *Edit3;
    TPanel *Panel_Input;
    TPanel *Panel4;
    TImage *Image4;
    TLabel *Label_Mode;
    TEdit *Edit1;
    TSpeedButton *SpeedButton_Change;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormKeyPress(TObject *Sender, char &Key);
    void __fastcall SpeedButton_ChangeMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall SpeedButton2MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall SpeedButton1Click(TObject *Sender);
protected:
    void __fastcall CreateParams(Controls::TCreateParams &Params);
private:	// User declarations

    int m_iCurrentMode;

    int m_iSelLevel ;
    int m_iCrntLevel ;
    AnsiString m_sEngineerPass ;
    AnsiString m_sMasterPass   ;

    void __fastcall LoadPassword(bool IsLoad);



public:		// User declarations

	__fastcall TFrmPassword(TComponent* Owner);

    bool __fastcall Show(int _iSelLevel) ;
    int  __fastcall GetLevel(){return m_iCrntLevel;}

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmPassword *FrmPassword;
//---------------------------------------------------------------------------
#endif
