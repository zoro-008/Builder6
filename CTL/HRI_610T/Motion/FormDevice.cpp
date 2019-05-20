//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDevice.h"
//#include "FormMain.h"
#include "FormDeviceSet.h"
//---------------------------------------------------------------------------
#include "LaserEO.h"
#include "MarkVisn.h"

#include "UserFile.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "UtilDefine.h"
#include "SLogUnit.h"
#include "SMinterfaceUnit.h"
#include "LotUnit.h"
#include "PstnMan.h"
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

//    LAG.ReadFrmDevice();
//    if(FM_GetLan() == lsKorean) LAG.LoadFrmDeviceHan();
//    switch(FM_GetLan())
//    {
//        default        :                             break ;
//        case lsEnglish :                             break ;
//        case lsKorean  : LAG.ReadFrmDevice();        break ;
//        case lsChinese :                             break ;
//    }
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
    bool bAllArayNone = DM.ARAY[riLDR].CheckAllStat(csNone) && DM.ARAY[riLSR].CheckAllStat(csNone) && DM.ARAY[riPRB].CheckAllStat(csNone) &&
                        DM.ARAY[riPSB].CheckAllStat(csNone) && DM.ARAY[riULD].CheckAllStat(csNone) && DM.ARAY[riVSN].CheckAllStat(csNone)  ;

    //if(LTQ.GetLotOpen()) {
    if(!bAllArayNone){
        if(FM_GetLan() == lsEnglish) FM_MsgOk("Error" , "Cannot change the working files.");
        else                         FM_MsgOk("Error" , "������ �ް����� ���� ������ �������� �ٲܼ� �����ϴ�.");
        return;
    }

    //JOB File ����.
    AnsiString sName = sgDevice -> Cells[1][sgDevice -> Row];
    AnsiString sTemp = "DOWNLOAD JOB FILE. (" + lbSelDevice -> Caption + ")";

    if(OM.GetCrntDev() == sName) {
        if(FM_GetLan() == lsEnglish) FM_MsgOk("ERROR","This is the current file with the same file.") ;
        else                         FM_MsgOk("ERROR","���� �����ϰ� ���� ������ �Դϴ�.");
        return ;
    }

    if (FM_MsgYesNo("Confirm","Are you Sure?") != mrYes) return ;


    OM.LoadJobFile (sName );                                                      pbStatus -> Position = 0  ;
    DM.ARAY[riLDR].SetMaxColRow(1                     , OM.DevInfo.iCsSlCnt   );  pbStatus -> Position = 5  ;
    DM.ARAY[riPRB].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );  pbStatus -> Position = 10 ;
    DM.ARAY[riLSR].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );  pbStatus -> Position = 25 ;
    DM.ARAY[riVSN].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );  pbStatus -> Position = 45 ;
    DM.ARAY[riPSB].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );  pbStatus -> Position = 70 ;
    DM.ARAY[riULD].SetMaxColRow(1                     , OM.DevInfo.iCsSlCnt   );  pbStatus -> Position =100 ;

    PM.Load(sName);

    lbSelDevice  -> Caption = "";
    edFromName   -> Text    = "";
    lbCrntDevice -> Caption = sName ;


    //���...
    if(!LSR.SaveLaserLotNo(OM.DevOptn.sSampleLotNo)){
        FM_MsgOk("ERR","Laser LotNo File writing failed!");
    }
    LSR.LaserJobChange(sName);
    VSN.VisnJobChange (sName);

    //Trace Log.
    Trace("",sTemp.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::FormShow(TObject *Sender)
{
    //StringGrid�� Update�Ѵ�.
    UserFile.GridSearchDir(ExtractFilePath(Application -> ExeName) + "JobFile" , sgDevice , 1 , true);     // ���丮 �о�ͼ� ��¥�� ���ĺ� ������ ����

    for(int i = 0 ; i < sgDevice -> RowCount ; i++) {
        if (sgDevice->Cells[1][i] == OM.GetCrntDev()) {
            sgDevice -> Row = i;
            lbSelDevice -> Caption = sgDevice->Cells[1][i];
        }
    }

    lbCrntDevice -> Caption = OM.GetCrntDev() ;
    lbSelDevice  -> Caption = sgDevice -> Cells[1][0] ;
    edFromName   -> Text    = sgDevice -> Cells[1][0] ;

    int iLevel;
//    FrmPassword->GetLevel();
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
                 if(FM_GetLan() == lsEnglish) FM_MsgOk("Error" , "Enter JOB FILE name is missing.");
                 else                         FM_MsgOk("Error" , "�Էµ� JOB FILE �̸��� �����ϴ�." );
                 return;
             }

             if(FM_GetLan() == lsEnglish){
                 if (FM_MsgYesNo("Confirm" , (edToName -> Text + " Create a JOBFILE?").c_str()) == mrNo) return ;
             }
             else {
                 if (FM_MsgYesNo("Confirm" , (edToName -> Text + " JOBFILE�� �����Ͻðڽ��ϱ�?").c_str()) == mrNo) return ;
             }


             Path = ExtractFilePath(Application -> ExeName) + "JobFile\\" + edToName -> Text ;
             if (!UserFile.CreateDir(Path)) {
                 if(FM_GetLan() == lsEnglish) FM_MsgOk("Error"  , "File with the same name exists.");
                 else                         FM_MsgOk("Warning","���� �̸��� File�� �����մϴ�.");
                 return ;
             }

             //Trace Log.
             Trace("JobFile",(edToName -> Text + " is Maked").c_str());
             break;

        case 2: //Copy.
            //Check None Name.
            if (edFromName -> Text == "") {   //�ƹ��͵� ���õ��� ���� ���.
                if(FM_GetLan() == lsEnglish) FM_MsgOk("Error"  , "The selected JOB FILE name is missing.");
                else                         FM_MsgOk("Error" , "���õ� JOB FILE �̸��� �����ϴ�." );
                return;
            }

            if (edToName -> Text == "") {   //�ƹ��͵� �Է��� ���� ���� ���
                 if(FM_GetLan() == lsEnglish) FM_MsgOk("Error" , "Enter JOB FILE name is missing." );
                 else                         FM_MsgOk("Error" , "�Էµ� JOB FILE �̸��� �����ϴ�." );
                 return;
            }

            if(FM_GetLan() == lsEnglish){
                if (FM_MsgYesNo("Confirm" , ("You want to copy the " + edToName -> Text + " created from " + edFromName -> Text + "?").c_str()) == mrNo) return ;
            }
            else {
                if (FM_MsgYesNo("Confirm" , (edFromName -> Text +"��"+ edToName -> Text + "�� JOB FILE�� ���� �����Ͻðڽ��ϱ�?").c_str()) == mrNo) return ;
            }

            Path = ExtractFilePath(Application -> ExeName) + "JobFile\\" ;
            if (UserFile.DirExists(Path + edToName -> Text)) {
                if(FM_GetLan() == lsEnglish) FM_MsgOk("Warning" , "File with the same name exists." );
                else                         FM_MsgOk("Warning" , "���� �̸��� JOB FILE�� �����մϴ�." );
                return ;
            }

            UserFile.CopyDir(Path + edFromName -> Text, Path + edToName -> Text);

            //Trace Log.
            Trace("JobFile",(edToName -> Text + " is Maked by Copy").c_str());

        	break;
        case 3: //Rename.
            //Check None Name.
            if (edFromName -> Text == "") {   //�ƹ��͵� ���õ��� ���� ���.
                if(FM_GetLan() == lsEnglish) FM_MsgOk("Error" , "The selected JOB FILE name is missing." );
                else                         FM_MsgOk("Error" , "���õ� JOB FILE �̸��� �����ϴ�." );
                return;
            }
            if (edToName -> Text == "") {   //�ƹ��͵� �Է��� ���� ���� ���
                if(FM_GetLan() == lsEnglish) FM_MsgOk("Error" , "Enter JOB FILE name is missing." );
	        else                         FM_MsgOk("Error" , "�Էµ� JOB FILE �̸��� �����ϴ�." );
                return;
            }

            if(FM_GetLan() == lsEnglish){
                if (FM_MsgYesNo("Confirm","Change the name of the JOB FILE?") == mrNo) return ;
            }
            else {
                if (FM_MsgYesNo("Confirm","JOB FILE �̸��� ���� �Ͻðڽ��ϱ�?") == mrNo) return ;
            }

            Path = ExtractFilePath(Application -> ExeName) + "JobFile\\";

            if (UserFile.DirExists(Path + edToName -> Text)) {
                if(FM_GetLan() == lsEnglish) FM_MsgOk("Warning" , "File with the same name exists." );
                else                         FM_MsgOk("Warning" , "���� �̸��� JOB FILE�� �����մϴ�." );
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
                if(FM_GetLan() == lsEnglish) FM_MsgOk("Error" , "The selected JOB FILE name is missing." );
        	else                         FM_MsgOk("Error" , "���õ� JOB FILE �̸��� �����ϴ�." );
                return;
            }

            if(FM_GetLan() == lsEnglish){
                if (FM_MsgYesNo("Confirm" , "Removes the selected JOB FILE?" ) == mrNo) return ;
            }
            else {
                if (FM_MsgYesNo("Confirm" , "������ JOB FILE�� ����ðڽ��ϱ�?" ) == mrNo) return ;
            }

            if (edFromName -> Text == OM.GetCrntDev()) {
                if(FM_GetLan() == lsEnglish) FM_MsgOk("Warning","JOB FILE currently in use cannot be deleted.");
                else                         FM_MsgOk("Warning","���� ����� ��JOB FILE�� ������ �� �����ϴ�.");
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
    lbCrntDevice -> Caption = OM.GetCrntDev() ;
    tmUpdate -> Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::FormDestroy(TObject *Sender)
{
    tmUpdate->Enabled = false;
}
//---------------------------------------------------------------------------

