//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDevice.h"
#include "FormDeviceSet.h"
//---------------------------------------------------------------------------
#include "UserINI.h"
#include "OptionMan.h"
#include "DataMan.h"
//#include "UtilDefine.h"
#include "SLogUnit.h"
//#include "SMinterfaceUnit.h"
//#include "UtilDefine.h"
#include "Epoxy.h"
#include "LotUnit.h"
#include "PstnMan.h"
//#include "FormMain.h"
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

    Trace("Operator", "Device Form_Setting Button Click");
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

    if (FM_MsgYesNo("Confirm","�ٿ�ε� �Ͻ� �ڽ��ϱ�?") != mrYes) return ;

    //JOB File ����.
    AnsiString sName = sgDevice -> Cells[1][sgDevice -> Row];
    AnsiString sTemp = "DOWNLOAD JOB FILE. (" + lbSelDevice -> Caption + ")";

    OM.LoadJobFile (sName );                                                   pbStatus -> Position = 0   ;


    String sBinPath = EXE_FOLDER + "JobFile\\" + sName + "\\MaskMap.dat";
    String sIniPath = EXE_FOLDER + "JobFile\\" + sName + "\\MaskMap.ini";
    DM.ARAY[riMSK].Load(true , sBinPath , sIniPath , 0);





    //DataMan SetMaxColRow
    DM.ARAY[riCST].SetMaxColRow(1                    , OM.DevInfo.iCsSlotCnt);
    //DM.ARAY[riSTG].SetMaxColRow(OM.DevInfo.iWfColCnt , OM.DevInfo.iWfRowCnt );
    DM.ARAY[riMAP].SetMaxColRow(OM.DevInfo.iWfColCnt , OM.DevInfo.iWfRowCnt );
    //DM.ARAY[riMSK].SetMaxColRow(OM.DevInfo.iWfColCnt , OM.DevInfo.iWfRowCnt );
    DM.ARAY[riTST].SetMaxColRow(OM.DevInfo.iWfColCnt , OM.DevInfo.iWfRowCnt );
    DM.ARAY[riBRD].SetMaxColRow(OM.DevInfo.iBdXCnt   , OM.DevInfo.iBdYCnt   );

                                                                               pbStatus -> Position = 80  ;
//    DM.ClearMap    (      );
    //DM.ARAY[riCST].ClearMap();
    DM.ARAY[riSTG].ClearMap();
    DM.ARAY[riMAP].ClearMap();
    //DM.ARAY[riMSK].ClearMap();
    DM.ARAY[riTST].ClearMap();
    //DM.ARAY[riBRD].ClearMap();

    OM.SaveLastInfo(      );                                                   pbStatus -> Position = 95  ;
    PM.Load        (sName );                                                   pbStatus -> Position = 100 ;

    lbSelDevice  -> Caption = "";
    edFromName   -> Text    = "";
    lbCrntDevice -> Caption = sName ;




    //Trace Log.
    Trace("btDownloadClick",sTemp.c_str());
    Trace("Operator", "Device Form_Download Button Click");
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
             Trace("Operator", "Device Form_New Button Click");
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
            Trace("Operator", "Device Form_Copy Button Click");

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
            Trace("Operator", "Device Form_Rename Button Click");

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
            Trace("Operator", "Device Form_Delete Button Click");

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

void __fastcall TFrmDevice::btSearchClick(TObject *Sender)
{
    if(edSearch -> Text == ""){
        FM_MsgTime("DEVICE","�˻� �� �ؽ�Ʈ�� �Է��ϼ���.",1000);
        Trace("Operation" , "Device Search Button Click");
        return;
    }

    int iCount = 0;

    bool bTemp ;

    AnsiString Path = GetExeDir("JobFile");
    UserFile.GridSearchDir(Path, sgDevice, 0, true);

    AnsiString sText  = edSearch -> Text.UpperCase();

    for(int i = 0 ; i < sgDevice -> RowCount ; i++ ){
        AnsiString sName = sgDevice -> Cells[1][i].UpperCase() ;

        if(sName.Pos(sText)) { bTemp = true ; break; }
        else                 { bTemp = false;        }
    }

    if (bTemp) {
        for(int i = 0 ; i < sgDevice -> RowCount ; i++ ){
            AnsiString sName = sgDevice -> Cells[1][i].UpperCase() ;

            if(sName.Pos(sText)){
               sgDevice -> Cols[0] -> Add(1+i);
               sgDevice -> Cols[1] -> Add(sgDevice -> Cells[1][i]);
               sgDevice -> Cols[2] -> Add(sgDevice -> Cells[2][i]);
               iCount++;
            }
            else {
                sgDevice -> Rows[i] -> Clear();
            }
            if( i >= iCount){ sgDevice -> Rows[i] -> Clear(); }
        }
    }
    else {
        FM_MsgTime("DEVICE","�˻��� ����̽��� �����ϴ�.",1000) ;
    }
    Trace("Operation" , "Device Search Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmDevice::btAllListClick(TObject *Sender)
{
    AnsiString Path = GetExeDir("JobFile");
    UserFile.GridSearchDir(Path, sgDevice, 0, true);
    Trace("Operation" , "Device All List Button Click");
}
//---------------------------------------------------------------------------

