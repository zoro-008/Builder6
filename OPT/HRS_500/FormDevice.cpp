//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDevice.h"
#include "FormDeviceSet.h"
//---------------------------------------------------------------------------
#include "UserFile.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "UtilDefine.h"
#include "SLogUnit.h"
#include "SMinterfaceUnit.h"
#include "UtilDefine.h"
#include "LotUnit.h"
#include "PstnMan.h"
#include "FormMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFrmDevice *FrmDevice;
//---------------------------------------------------------------------------
__fastcall TFrmDevice::TFrmDevice(TComponent* Owner)
    : TForm(Owner)
{
    //Set String Grid Width.
    sgDevice -> ColWidths[0] = 30 ;
    sgDevice -> ColWidths[1] = 400;
    sgDevice -> ColWidths[2] = 200;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::btSettingClick(TObject *Sender)
{
    FrmDeviceSet -> Show();
}
//---------------------------------------------------------------------------

void __fastcall TFrmDevice::sgDeviceSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
    AnsiString sTemp = sgDevice->Cells[1][ARow];

    lbSelDevice -> Caption  = sTemp;
    edFromName  -> Text     = sTemp;
    pbStatus    -> Position = 0;
}

//---------------------------------------------------------------------------
void __fastcall TFrmDevice::btDownloadClick(TObject *Sender)
{
    if (edFromName -> Text == "") return;

//     //Check Running Status.
//    if(LT.m_bLotOpen) {
//        FM_MsgOk("Error" , "Can't Change JobFile While LotOpen");
//        return;
//    }

    if (FM_MsgYesNo("Confirm","Are you Sure?") != mrYes) return ;

    //JOB File ����.
    AnsiString sName = sgDevice -> Cells[1][sgDevice -> Row];
    AnsiString sTemp = "DOWNLOAD JOB FILE. (" + lbSelDevice -> Caption + ")";

    OM.LoadJobFile (sName );                                                   pbStatus -> Position = 0   ;
    //DataMan SetMaxColRow
    DM.ARAY[riPRB].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riWRK].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riWKE].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riPSB].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riPCK].SetMaxColRow(1,1);
    DM.ARAY[riSTN].SetMaxColRow(1,1);
    DM.ARAY[riSRT].SetMaxColRow(1,1);
                                                                               pbStatus -> Position = 80  ;
    DM.ClearMap    (      );                                                   pbStatus -> Position = 90  ;
    OM.SaveLastInfo(      );                                                   pbStatus -> Position = 95  ;
    PM.Load        (sName );                                                   pbStatus -> Position = 100 ;

    lbSelDevice  -> Caption = "";
    edFromName   -> Text    = "";
    lbCrntDevice -> Caption = sName ;

    //Trace Log.
    Trace("btDownloadClick",sTemp.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::FormShow(TObject *Sender)
{
    //StringGrid�� Update�Ѵ�.
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

    int iLevel;
    iLevel = FM_GetLevel();
    switch (iLevel) {
        case lvOperator : btSetting -> Enabled = false; break ;
        case lvEngineer : btSetting -> Enabled = true ; break ;
        case lvMaster   : btSetting -> Enabled = true ; break ;
        default         :                               break ;
    }
    tmUpdate -> Enabled = true;

}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::btNewClick(TObject *Sender)
{
    TButton *Btn = dynamic_cast<TButton *>(Sender);
    int index = Btn->Tag ;

    AnsiString Path;
    AnsiString sLog;


    Path = ExtractFilePath(Application -> ExeName) + "JobFile\\" ;
    if(!UserFile.DirExists(Path)) UserFile.CreateDir(Path) ;

    switch (index) {
    	case 1: //New.
             if (edToName -> Text == "") {   //�ƹ��͵� �Է��� ���� ���� ���
                 FM_MsgOk("Error" , "�Էµ� JOB FILE �̸��� �����ϴ�." );
                 return;
             }

             if (FM_MsgYesNo("Confirm" , edToName -> Text + " JOBFILE�� �����Ͻðڽ��ϱ�?") == mrNo) return ;

             Path = ExtractFilePath(Application -> ExeName) + "JobFile\\" + edToName -> Text ;
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

            Path = ExtractFilePath(Application -> ExeName) + "JobFile\\" ;
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

            Path = ExtractFilePath(Application -> ExeName) + "JobFile\\";

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

            Path = ExtractFilePath(Application -> ExeName) + "JobFile\\" + edFromName -> Text;
            UserFile.ClearDir  (Path , 1);

            //Trace Log.
            Trace("JobFile",(edToName -> Text + " is Deleted").c_str());

        	break;
    }
    pbStatus->Position = 0;
    UserFile.GridSearchDir(ExtractFilePath(Application -> ExeName)+"JobFile" , sgDevice , 1 , true);     // ���丮 �о�ͼ� ��¥�� ���ĺ� ������ ����
    sgDevice -> Row = 0 ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false;
    lbCrntDevice -> Caption = OM.m_sCrntDev ;
    tmUpdate -> Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDevice::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate->Enabled = false;        
}
//---------------------------------------------------------------------------

