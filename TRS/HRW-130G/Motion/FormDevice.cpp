//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormDevice.h"
#include "OptionMan.h"
#include "UserFile.h"
#include "FormMsgOk.h"
#include "global.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmDevice *FrmDevice;
//---------------------------------------------------------------------------
__fastcall TFrmDevice::TFrmDevice(TComponent* Owner)
        : TForm(Owner)
{
    sgDevice -> ColWidths[0] = 30 ;
    sgDevice -> ColWidths[1] = 200;
    sgDevice -> ColWidths[2] = 200;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::btNewMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int iPanelNo = ((TPanel *)Sender) -> Tag ;

    switch(iPanelNo){
        case 1 : btNew      -> BevelOuter = bvLowered ; break ;
        case 2 : btCopy     -> BevelOuter = bvLowered ; break ;
        case 3 : btRename   -> BevelOuter = bvLowered ; break ;
        case 4 : btDelete   -> BevelOuter = bvLowered ; break ;
        case 5 : btDownload -> BevelOuter = bvLowered ; break ;
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::btNewMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    btNew      -> BevelOuter = bvRaised ;
    btCopy     -> BevelOuter = bvRaised ;
    btRename   -> BevelOuter = bvRaised ;
    btDelete   -> BevelOuter = bvRaised ;
    btDownload -> BevelOuter = bvRaised ;


}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::sgDeviceSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
    AnsiString sTemp = sgDevice->Cells[1][ARow];

    lbSelDevice -> Caption  = sTemp;
    edFromName  -> Text     = sTemp;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::btDownloadClick(TObject *Sender)
{
    if (edFromName -> Text == "") return;

    AnsiString sName = sgDevice -> Cells[1][sgDevice -> Row];
    AnsiString sTemp = "DOWNLOAD JOB FILE. (" + lbSelDevice -> Caption + ")";

    OM.LoadJobFile (sName );

    lbSelDevice  -> Caption = "";
    edFromName   -> Text    = "";
    lbCrntDevice -> Caption = sName ;
            
}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::btNewClick(TObject *Sender)
{
    TPanel *Pnl = dynamic_cast<TPanel *>(Sender);
    int index = Pnl->Tag ;

    AnsiString Path;
    AnsiString sLog;


    Path = ExtractFilePath(Application -> ExeName) + "JobFile\\" ;
    if(!UserFile.DirExists(Path)) UserFile.CreateDir(Path) ;

    switch (index) {
    	case 1: //New.
             if (edToName -> Text == "") {   //�ƹ��͵� �Է��� ���� ���� ���
                 FrmMsgOk->MsgOk("Error" , "�Էµ� JOB FILE �̸��� �����ϴ�." );
                 Trace("Error" , "�Էµ� JOB FILE �̸��� �����ϴ�." );
                 return;
             }

             Path = ExtractFilePath(Application -> ExeName) + "JobFile\\" + edToName -> Text ;
             if (!UserFile.CreateDir(Path)) {
                 FrmMsgOk->MsgOk("Warning","���� �̸��� File�� �����մϴ�.");
                 Trace("Warning","���� �̸��� File�� �����մϴ�.");
                 return ;
             }
             break;

        case 2: //Copy.
            //Check None Name.
            if (edFromName -> Text == "") {   //�ƹ��͵� ���õ��� ���� ���.
                FrmMsgOk->MsgOk("Error" , "���õ� JOB FILE �̸��� �����ϴ�." );
                Trace("Error" , "���õ� JOB FILE �̸��� �����ϴ�." );
                return;
            }

            if (edToName -> Text == "") {   //�ƹ��͵� �Է��� ���� ���� ���
                FrmMsgOk->MsgOk("Error" , "�Էµ� JOB FILE �̸��� �����ϴ�." );
                Trace("Error" , "�Էµ� JOB FILE �̸��� �����ϴ�." );
                return;
            }
            Path = ExtractFilePath(Application -> ExeName) + "JobFile\\" ;
            if (UserFile.DirExists(Path + edToName -> Text)) {
                FrmMsgOk->MsgOk("Warning" , "���� �̸��� JOB FILE�� �����մϴ�." );
                Trace("Warning" , "���� �̸��� JOB FILE�� �����մϴ�." );
                return ;
            }

            UserFile.CopyDir(Path + edFromName -> Text, Path + edToName -> Text);

            break;
        case 3: //Rename.
            //Check None Name.
            if (edFromName -> Text == "") {   //�ƹ��͵� ���õ��� ���� ���.
                FrmMsgOk->MsgOk("Error" , "���õ� JOB FILE �̸��� �����ϴ�." );
                Trace("Error" , "���õ� JOB FILE �̸��� �����ϴ�." );
                return;
            }
            if (edToName -> Text == "") {   //�ƹ��͵� �Է��� ���� ���� ���
	        FrmMsgOk->MsgOk("Error" , "�Էµ� JOB FILE �̸��� �����ϴ�." );
                Trace("Error" , "�Էµ� JOB FILE �̸��� �����ϴ�." );
                return;
            }

            Path = ExtractFilePath(Application -> ExeName) + "JobFile\\";

            if (UserFile.DirExists(Path + edToName -> Text)) {
                FrmMsgOk->MsgOk("Warning" , "���� �̸��� JOB FILE�� �����մϴ�." );
                Trace("Warning" , "���� �̸��� JOB FILE�� �����մϴ�." );
                return ;
            }
            RenameFile(Path + edFromName -> Text , Path + edToName -> Text);

            OM.SetCrntDev(edToName -> Text);

            break;
        case 4: //Delete
            //Check None Name.
            if (edFromName -> Text == "") {   //�ƹ��͵� ���õ��� ���� ���.
        	FrmMsgOk->MsgOk("Error" , "���õ� JOB FILE �̸��� �����ϴ�." );
                Trace("Error" , "���õ� JOB FILE �̸��� �����ϴ�." );
                return;
            }

            if (edFromName -> Text == OM.m_sCrntDev) {
                FrmMsgOk->MsgOk("Warning","���� ����� ��JOB FILE�� ������ �� �����ϴ�.");
                Trace("Warning","���� ����� ��JOB FILE�� ������ �� �����ϴ�.");
                return ;
            }

            Path = ExtractFilePath(Application -> ExeName) + "JobFile\\" + edFromName -> Text;
            UserFile.ClearDir  (Path , 1);

            break;
    }
    UserFile.GridSearchDir(ExtractFilePath(Application -> ExeName)+"JobFile" , sgDevice , 1 , true);     // ���丮 �о�ͼ� ��¥�� ���ĺ� ������ ����
    sgDevice -> Row = 0 ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::FormShow(TObject *Sender)
{
    lbModelName -> Caption = g_sModelName;
    UserFile.GridSearchDir(ExtractFilePath(Application -> ExeName) + "JobFile" , sgDevice , 1 , true);     // ���丮 �о�ͼ� ��¥�� ���ĺ� ������ ����

    for(int i = 0 ; i < sgDevice -> RowCount ; i++) {
        if (sgDevice->Cells[1][i] == OM.m_sCrntDev) {
            sgDevice -> Row = i;
            lbSelDevice -> Caption = sgDevice->Cells[1][i];
        }
    }

    lbCrntDevice -> Caption = OM.m_sCrntDev ;
    lbSelDevice  -> Caption = sgDevice -> Cells[1][0] ;
    edFromName   -> Text    = sgDevice -> Cells[1][0] ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::btOKClick(TObject *Sender)
{
    if(edToName -> Text == ""){edToName -> Text = 0;}

    pnButton -> Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDevice::btClearClick(TObject *Sender)
{
    edToName -> Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TFrmDevice::btBackClick(TObject *Sender)
{
    AnsiString sEditText ;

    sEditText = edToName -> Text;
    
    sEditText = sEditText.Delete(sEditText.Length(), 1);

}
//---------------------------------------------------------------------------

void __fastcall TFrmDevice::bt1Click(TObject *Sender)
{
    AnsiString sEditText ;
    int iManNo = ((TButton *)Sender) -> Tag ;

    sEditText = edToName    -> Text; 

    if(sEditText == "0") sEditText = "";

    sEditText += iManNo;

    edToName    -> Text = sEditText ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmDevice::edToNameClick(TObject *Sender)
{
    pnButton -> Visible = true;           
}
//---------------------------------------------------------------------------


