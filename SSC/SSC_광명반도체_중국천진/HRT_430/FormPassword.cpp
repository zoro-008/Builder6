//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormPassword.h"
#include "UserIni.h"
//#include "FileManager.h"
//#include "Function.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmPassword *FrmPassword;
//---------------------------------------------------------------------------
__fastcall TFrmPassword::TFrmPassword(TComponent* Owner)
	: TForm(Owner)
{
    LoadPassword(true);
}
//---------------------------------------------------------------------------
void __fastcall TFrmPassword::CreateParams(Controls::TCreateParams &Params)
{
    // Main Form Caption Delete
	TForm::CreateParams(Params);
    Params.Style &= ~WS_CAPTION;
}
//---------------------------------------------------------------------------

void __fastcall TFrmPassword::FormShow(TObject *Sender)
{
    m_iCurrentMode = 0;
    Edit1->Clear();
    Edit1->SetFocus();
    Panel_Input ->BringToFront();
    SpeedButton_Change->Show();

    Label_Mode->Caption = "Input Password";
}
//---------------------------------------------------------------------------

void __fastcall TFrmPassword::FormKeyPress(TObject *Sender, char &Key)
{
    if(Key == VK_ESCAPE) ModalResult = mrCancel;
    if(Key == VK_RETURN) SpeedButton1Click(NULL);
}
//---------------------------------------------------------------------------


void __fastcall TFrmPassword::SpeedButton_ChangeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    Panel_Input->SendToBack();
    SpeedButton_Change->Hide();
    Edit2->Clear();
    Edit3->Clear();
    Edit2->SetFocus();
    m_iCurrentMode = 1;
}
//---------------------------------------------------------------------------

void __fastcall TFrmPassword::SpeedButton2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TFrmPassword::SpeedButton1Click(TObject *Sender)
{
    if(m_iCurrentMode == 0)
    {
        if(m_sUser1Pass==Edit1->Text)
        {
            Edit1->Clear();
            ModalResult = mrOk;
        }
        else
        {
            ShowMessage("Wrong Password") ;
            Edit1->Clear();
            Edit1->SetFocus();
        }
    }
    else
    {
        if(Edit2->Text == m_sUser1Pass) {
            m_sUser1Pass = Edit3->Text ;
            LoadPassword(false);
            Edit2->Clear();
            Edit3->Clear();
            ModalResult = mrOk;
        }
        else {
            ShowMessage("Wrong Password") ;
            Edit2->Clear();
            Edit3->Clear();
            Edit2->SetFocus();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmPassword::LoadPassword(bool IsLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Path   ;

    //Make Dir.
    if (!DirectoryExists(ExtractFilePath(Application->ExeName) + "Util"))
        CreateDir(ExtractFilePath(Application->ExeName) + "Util");
    Path = ExtractFilePath(Application->ExeName) + "Util\\" + "Password.INI";

    //Load Device.
    if (IsLoad) {
        UserINI.Load(Path.c_str() , "PASSWORD" , "m_sUser1Pass" , m_sUser1Pass);
    }
    else {
        UserINI.Save(Path.c_str() , "PASSWORD" , "m_sUser1Pass" , m_sUser1Pass);
    }
}


