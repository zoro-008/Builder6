//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmDevice.h"
#include "FrmDeviceSet.h"
#include "FrmMotorPos.h"
#include "FrmInOutState.h"
#include "FrmOption.h"
//---------------------------------------------------------------------------
#include "UserFile.h"
#include "UserINI.h"
#include "SLogUnit.h"
#include "DataMan.h"
#include "OptionMan.h"
#include "LotUnit.h"
#include "PstnMan.h"
#include "VisnComUnit.h"
#include "Head.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDevice *FormDevice;
//---------------------------------------------------------------------------
__fastcall TFormDevice::TFormDevice(TComponent* Owner)
    : TForm(Owner)
{
    sgDevice -> ColWidths[0] = 30 ;
    sgDevice -> ColWidths[1] = 305;
    sgDevice -> ColWidths[2] = 200;

    m_iPageIndex = 0 ;
}
//---------------------------------------------------------------------------
void __fastcall TFormDevice::btnDevSetClick(TObject *Sender)
{
    TButton *pButton = dynamic_cast<TButton *>(Sender);
    ClosePage();
    m_iPageIndex = pButton->Tag;
    ShowPage (m_iPageIndex);
}
//---------------------------------------------------------------------------
void __fastcall TFormDevice::FormShow(TObject *Sender)
{
    FormDeviceSet -> Parent = pnDevice; FormDeviceSet  -> Left = 0 ; FormDeviceSet  -> Top = 0 ;
    FormMotorPos  -> Parent = pnDevice; FormMotorPos   -> Left = 0 ; FormMotorPos   -> Top = 0 ;
    FormInOutState-> Parent = pnDevice; FormInOutState -> Left = 0 ; FormInOutState -> Top = 0 ;
    FormOption    -> Parent = pnDevice; FormOption     -> Left = 0 ; FormOption     -> Top = 0 ;
    
    //StringGrid�� Update�Ѵ�.
    UserFile.GridSearchDir(EXE_FOLDER + "JobFile" , sgDevice , 0 , true);     // ���丮 �о�ͼ� ��¥�� ���ĺ� ������ ����

    for(int i = 0 ; i < sgDevice -> RowCount ; i++) {
        if (sgDevice->Cells[1][i] == OM.m_sCrntDev) {
            sgDevice -> Row = i;
            lbSelDevice -> Caption = sgDevice->Cells[1][i];
        }
    }

    lbCrntDevice -> Caption = OM.m_sCrntDev ;
    lbSelDevice  -> Caption = sgDevice -> Cells[1][0] ;
    edFromName   -> Text    = sgDevice -> Cells[1][0] ;

    tmUpdate -> Enabled = true ;

}
//---------------------------------------------------------------------------
void __fastcall TFormDevice::btnDeviceClick(TObject *Sender)
{
    ClosePage();
    Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormDevice::ShowPage(int Index)
{
    switch(Index)
    {
        case 0: FormDeviceSet  -> Show();  break ;
        case 1: FormMotorPos   -> Show();  break ;
        case 2: FormInOutState -> Show();  break ;
        case 3: FormOption     -> Show();  break ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormDevice::ClosePage()
{
    FormDeviceSet -> Hide ();  
    FormMotorPos  -> Close();  
    FormInOutState-> Hide ();  
    FormOption    -> Hide ();  

}
//---------------------------------------------------------------------------
void __fastcall TFormDevice::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false;
    lbCrntDevice -> Caption = OM.m_sCrntDev ;
    tmUpdate -> Enabled = true;        
}
//---------------------------------------------------------------------------

void __fastcall TFormDevice::btDownloadClick(TObject *Sender)
{
    if (edFromName -> Text == "") return;

//     //Check Running Status.
    if(LT.m_bLotOpen) {
        FM_MsgOk("Error" , "Can't Change JobFile While LotOpen");
        return;
    }

    if (FM_MsgYesNo("Confirm","Are you Sure?") != mrYes) return ;


    //JOB File ����.
    AnsiString sName = sgDevice -> Cells[1][sgDevice -> Row];
    AnsiString sTemp = "DOWNLOAD JOB FILE. (" + lbSelDevice -> Caption + ")";

    OM.LoadJobFile (sName );                                                   pbStatus -> Position = 0   ;

    OM.DMSetMaxColRow(true);                                                   pbStatus -> Position = 30  ;

//    DM.ClearMap    (      );                                                   pbStatus -> Position = 90  ;
    OM.SaveLastInfo(      );                                                   pbStatus -> Position = 95  ;
    PM.Load        (sName );                                                   pbStatus -> Position = 100 ;

    lbSelDevice  -> Caption = "";
    edFromName   -> Text    = "";
    lbCrntDevice -> Caption = sName ;

    if(!OM.CmnOptn.bVs1_1Skip && OM.CmnOptn.bAutoVsChange) VC.SendModelChange(vsVisn1_1);
    if(!OM.CmnOptn.bVs1_2Skip && OM.CmnOptn.bAutoVsChange) VC.SendModelChange(vsVisn1_2);
    if(!OM.CmnOptn.bVs2_1Skip && OM.CmnOptn.bAutoVsChange) VC.SendModelChange(vsVisn2_1);
    if(!OM.CmnOptn.bVs2_2Skip && OM.CmnOptn.bAutoVsChange) VC.SendModelChange(vsVisn2_2);
    if(!OM.CmnOptn.bVs3_1Skip && OM.CmnOptn.bAutoVsChange) VC.SendModelChange(vsVisn3_1);
    if(!OM.CmnOptn.bVs3_2Skip && OM.CmnOptn.bAutoVsChange) VC.SendModelChange(vsVisn3_2);


    //Trace Log.
    Trace("",sTemp.c_str());

}
//---------------------------------------------------------------------------

void __fastcall TFormDevice::btNewClick(TObject *Sender)
{
    TButton *Btn = dynamic_cast<TButton *>(Sender);
    int index = Btn->Tag ;

    AnsiString Path;
    AnsiString sLog;


    Path = EXE_FOLDER + "JobFile\\" ;
    if(!UserFile.DirExists(Path)) UserFile.CreateDir(Path) ;

    switch (index) {
    	case 1: //New.
             if (edToName -> Text == "") {   //�ƹ��͵� �Է��� ���� ���� ���
                 FM_MsgOk("Error" , "�Էµ� JOB FILE �̸��� �����ϴ�." );
                 return;
             }

             if (FM_MsgYesNo("Confirm" , edToName -> Text + " JOBFILE�� �����Ͻðڽ��ϱ�?") == mrNo) return ;

             Path = EXE_FOLDER + "JobFile\\" + edToName -> Text ;
             if (!UserFile.CreateDir(Path)) {
                 FM_MsgOk("Warning","���� �̸��� File�� �����մϴ�.");
                 return ;
             }

             //Trace Log.
             Trace("JobFile",(edToName -> Text + " is Maked").c_str());
             break;

        case 2: //Copy.
            //Check None Name.
            if (edFromName -> Text == "") {   //�ƹ��͵� ���õ��� ���� ���.
                FM_MsgOk("Error" , "���õ� JOB FILE �̸��� �����ϴ�." );
                return;
            }

            if (edToName -> Text == "") {   //�ƹ��͵� �Է��� ���� ���� ���
                 FM_MsgOk("Error" , "�Էµ� JOB FILE �̸��� �����ϴ�." );
                 return;
            }
            if (FM_MsgYesNo("Confirm" , edFromName -> Text +"��"+ edToName -> Text + "�� JOB FILE�� ���� �����Ͻðڽ��ϱ�?") == mrNo) return ;

            Path = EXE_FOLDER + "JobFile\\" ;
            if (UserFile.DirExists(Path + edToName -> Text)) {
                FM_MsgOk("Warning" , "���� �̸��� JOB FILE�� �����մϴ�." );
                return ;
            }

            UserFile.CopyDir(Path + edFromName -> Text, Path + edToName -> Text);

            //Trace Log.
            Trace("JobFile",(edToName -> Text + " is Maked by Copy").c_str());

        	break;
        case 3: //Rename.
            //Check None Name.
            if (edFromName -> Text == "") {   //�ƹ��͵� ���õ��� ���� ���.
                FM_MsgOk("Error" , "���õ� JOB FILE �̸��� �����ϴ�." );
                return;
            }
            if (edToName -> Text == "") {   //�ƹ��͵� �Է��� ���� ���� ���
	        FM_MsgOk("Error" , "�Էµ� JOB FILE �̸��� �����ϴ�." );
                return;
            }

            if (FM_MsgYesNo("Confirm","JOB FILE �̸��� ���� �Ͻðڽ��ϱ�?") == mrNo) return ;

            Path = EXE_FOLDER + "JobFile\\";

            if (UserFile.DirExists(Path + edToName -> Text)) {
                FM_MsgOk("Warning" , "���� �̸��� JOB FILE�� �����մϴ�." );
                return ;
            }
            RenameFile(Path + edFromName -> Text , Path + edToName -> Text);

            OM.SetCrntDev(edToName -> Text);

            //Trace Log.
            Trace("JobFile",(edToName -> Text + " is Maked by Rename").c_str());

        	break;
        case 4: //Delete
            //Check None Name.
            if (edFromName -> Text == "") {   //�ƹ��͵� ���õ��� ���� ���.
        	FM_MsgOk("Error" , "���õ� JOB FILE �̸��� �����ϴ�." );
                return;
            }

            if (FM_MsgYesNo("Confirm" , "������ JOB FILE�� ����ðڽ��ϱ�?" ) == mrNo) return ;
            if (edFromName -> Text == OM.m_sCrntDev) {
                FM_MsgOk("Warning","���� ����� ��JOB FILE�� ������ �� �����ϴ�.");
                return ;
            }

            Path = EXE_FOLDER + "JobFile\\" + edFromName -> Text;
            UserFile.ClearDir  (Path , 1);

            //Trace Log.
            Trace("JobFile",(edToName -> Text + " is Deleted").c_str());

        	break;
    }
    pbStatus->Position = 0;
    UserFile.GridSearchDir(EXE_FOLDER+"JobFile" , sgDevice , 1 , true);     // ���丮 �о�ͼ� ��¥�� ���ĺ� ������ ����
    sgDevice -> Row = 0 ;
}
//---------------------------------------------------------------------------

void __fastcall TFormDevice::sgDeviceSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
    AnsiString sTemp = sgDevice->Cells[1][ARow];

    lbSelDevice -> Caption  = sTemp;
    edFromName  -> Text     = sTemp;
    pbStatus    -> Position = 0;        
}
//---------------------------------------------------------------------------

void __fastcall TFormDevice::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate->Enabled = false ;        
}
//---------------------------------------------------------------------------


