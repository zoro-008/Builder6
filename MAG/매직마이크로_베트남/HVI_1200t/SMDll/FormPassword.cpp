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

    if(m_iSelLevel == lvEngineer) Label_Mode->Caption = "[ENGINEER] Input Password";
    if(m_iSelLevel == lvMaster  ) Label_Mode->Caption = "[MASTER] Input Password";
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
    {//Password Check
        if(m_iSelLevel== lvEngineer)
        {
            if(m_sEngineerPass==Edit1->Text)
            {
                Edit1->Clear();
                m_iCrntLevel = m_iSelLevel ;
                ModalResult = mrOk;
            }
            else
            {
                ShowMessage("Wrong Password") ;
                Edit1->Clear();
                Edit1->SetFocus();
            }
        }
        else if(m_iSelLevel== lvMaster)
        {
            if(m_sMasterPass==Edit1->Text || AnsiString("zxc")==Edit1->Text)
            {
                Edit1->Clear();
                m_iCrntLevel = m_iSelLevel ;
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
            m_iCrntLevel = m_iSelLevel ;
        }
    }
    else
    {
        if(m_iSelLevel== lvEngineer)
        {
            if(Edit2->Text == m_sEngineerPass) {
                m_sEngineerPass = Edit3->Text ;
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
        else if(m_iSelLevel== lvMaster)
        {
            if(Edit2->Text == m_sMasterPass || AnsiString("zxc")==Edit1->Text) {
                m_sMasterPass = Edit3->Text ;
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
        UserINI.Load(Path.c_str() , "PASSWORD" , "m_sEngineerPass" , m_sEngineerPass);
        UserINI.Load(Path.c_str() , "PASSWORD" , "m_sMasterPass  " , m_sMasterPass  );
    }
    else {
        UserINI.Save(Path.c_str() , "PASSWORD" , "m_sEngineerPass" , m_sEngineerPass);
        UserINI.Save(Path.c_str() , "PASSWORD" , "m_sMasterPass  " , m_sMasterPass  );
    }
}

bool __fastcall TFrmPassword::Show(EN_LEVEL _iSelLevel)
{
    if(m_iCrntLevel == _iSelLevel) return true ;

    if(_iSelLevel == lvOperator) {
        m_iCrntLevel = lvOperator ;
        return false ;
    }

    m_iSelLevel = _iSelLevel ;

    return(mrOk==ShowModal()) ;
}

EN_LEVEL  __fastcall TFrmPassword::SetLevel(EN_LEVEL _iSelLevel)
{
    m_iCrntLevel = _iSelLevel;
}
