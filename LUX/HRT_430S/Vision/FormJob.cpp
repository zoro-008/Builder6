//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormJob.h"
#include "UserFile.h"
#include "GlobalMan.h"

//#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "CGAUGES"
#pragma resource "*.dfm"
TFrmJob *FrmJob=NULL;
//---------------------------------------------------------------------------
__fastcall TFrmJob::TFrmJob(TComponent* Owner)
        : TForm(Owner)
{
    Left = 100 ;
    Top  = 100 ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmJob::FormShow(TObject *Sender)
{


    sgDevice->ColWidths[0] =  30;
    sgDevice->ColWidths[1] = 200;
    sgDevice->ColWidths[2] = 150;

    AnsiString Path = GetExeDir("JobFile");
    UserFile.GridSearchDir(Path, sgDevice, 0, true);

    for (int i=0 ; i<sgDevice->RowCount; i++) {
        if (sgDevice->Cells[1][i]==GM.GetCrntJobFile()) {
            sgDevice->Row = i;
        }
    }
    edCrntDevice->Text = GM.GetCrntJobFile();
    edSelDevice ->Text = GM.GetCrntJobFile();

    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmJob::btOkClick(TObject *Sender)
{
    if (MessageBox(this->Handle, "DownLoad �Ͻðڽ��ϱ�?", "Confirmation", MB_YESNO)!=IDYES) return;

    //AnsiString Path = GetExeDir("JobFile\\") + sgDevice->Cells[1][sgDevice->Row] ;
    AnsiString Path = sgDevice->Cells[1][sgDevice->Row] ;
    fpLoadJobFile(Path.c_str());
    //sunsun VSM.LoadJobFile(true , Path);

    //for (int k=0; k<=100; k+=10) cgProgress->Progress =k; Sleep(20);

    //Trace("Operator", "FormJob Form_OK Button Click");
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmJob::btExitClick(TObject *Sender)
{
    //Trace("Operator", "FormJob Form_Close Button Click");
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmJob::btNewClick(TObject *Sender)
{
    AnsiString Path;

    //Check None Name.
    if (edNewDevice->Text.Trim()=="") {ShowMessage("�������̽��� �Էµ� �̸��� �����ϴ�."); return;}

    Path = GetExeDir("JobFile\\" + edNewDevice->Text);
    if (!UserFile.CreateDir(Path)) ShowMessage("���� �̸��� Parameter File�� �����մϴ�.");

    Path = GetExeDir("JobFile");
    UserFile.GridSearchDir(Path, sgDevice, 0, true);

    for (int i=0; i<sgDevice->RowCount; i++) {
        if (sgDevice->Cells[1][i] == GM.GetCrntJobFile()) sgDevice->Row = i;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmJob::btRenameClick(TObject *Sender)
{
    AnsiString Path;

    Path = GetExeDir("JobFile\\");

    if (edNewDevice->Text.Trim() == ""              ) {ShowMessage("�������̽��� �Էµ� �̸��� �����ϴ�."      ); return;}
    if (UserFile.DirExists(Path + edNewDevice->Text)) {ShowMessage("���� �̸��� JOB FILE�� �����մϴ�."          ); return;}

    UserFile.RenameDir(Path + edSelDevice->Text , Path + edNewDevice->Text);

    sgDevice->Cells[1][sgDevice->Row] = edSelDevice->Text;

    Path = GetExeDir("JobFile") ;
    UserFile.GridSearchDir(Path, sgDevice, 0, true);

    GM.SetCrntJobFile(edNewDevice -> Text)  ;

    for (int i=0; i<sgDevice->RowCount; i++) {
        if (sgDevice->Cells[1][i] == GM.GetCrntJobFile()) sgDevice->Row = i;
    }

    edSelDevice ->Text = GM.GetCrntJobFile();



    //Trace("Operator", "FormJob Form_Rename Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmJob::btCopyClick(TObject *Sender)
{
    AnsiString Path;

    Path = GetExeDir("JobFile\\");

    if (edNewDevice->Text.Trim() == ""              ) {ShowMessage("JOB FILE�̸��� �Է����� �����̽��ϴ�." ); return;}
    if (UserFile.DirExists(Path + edNewDevice->Text)) {ShowMessage("���� �̸��� JOB FILE�� �����մϴ�."    ); return;}

    UserFile.CopyDir(Path + edSelDevice->Text, Path + edNewDevice->Text);

    Path = GetExeDir("JobFile") ;
    UserFile.GridSearchDir(Path, sgDevice, 0, true);

    for (int i=0; i<sgDevice->RowCount; i++) {
        if (sgDevice->Cells[1][i] == GM.GetCrntJobFile()) sgDevice->Row = i;
    }
    //Trace("Operator", "FormJob Form_Copy Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmJob::btDeleteClick(TObject *Sender)
{
    AnsiString Path;

    Path = GetExeDir("JobFile\\");

    if (edSelDevice->Text == GM.GetCrntJobFile()) {ShowMessage("���� ������� JOB FILE�� ������ �� �����ϴ�."); return;}
    if (MessageBox(this->Handle, "�����Ͻðڽ��ϱ�?", "Confirmation", MB_YESNO)!=IDYES) return ;

    UserFile.ClearDir(Path + edSelDevice->Text, 1);

    Path = GetExeDir("JobFile");
    UserFile.GridSearchDir(Path, sgDevice, 0, true);

    for (int i=0; i<sgDevice->RowCount; i++) {
        if (sgDevice->Cells[1][i] == GM.GetCrntJobFile()) sgDevice -> Row = i;
    }
    //Trace("Operator", "FormJob Form_Delete Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmJob::sgDeviceMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    edSelDevice->Text = sgDevice->Cells[1][sgDevice->Row];
}
//---------------------------------------------------------------------------

void __fastcall TFrmJob::FormDestroy(TObject *Sender)
{
    static int i = 0 ;
    //Trace("",AnsiString(i).c_str());
    i++;


}
//---------------------------------------------------------------------------

bool TFrmJob::SetLoadJobFileFunc( FLoadJobFile _fpLoadJobFile)
{
    fpLoadJobFile = _fpLoadJobFile ;
    return true ;
}

void __fastcall TFrmJob::tmUpdateTimer(TObject *Sender)
{
    edCrntDevice -> Text = GM.GetCrntJobFile() ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmJob::FormClose(TObject *Sender, TCloseAction &Action)
{
    tmUpdate -> Enabled = false ;        
}
//---------------------------------------------------------------------------

