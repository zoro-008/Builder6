//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDevice.h"
#include "FormDeviceSet.h"
#include "FormProbe.h"
#include "FormConversion.h"
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
#include "ProbeComUnit.h"
#include "FormMain.h"
#include "LanguageUnit.h"
#include "SVInterface.h"
#include "UserINI.h"
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
    if(LT.m_bLotOpen&& !OM.MstOptn.bNotUstLot) {
        FM_MsgOk("Error" , "Can't Change JobFile While LotOpen");
        return;
    }

    if (FM_MsgYesNo("Confirm","Are you Sure?") != mrYes) return ;


    //JOB File ����.
    AnsiString sName = sgDevice -> Cells[1][sgDevice -> Row];
    AnsiString sTemp = "DOWNLOAD JOB FILE. (" + lbSelDevice -> Caption + ")";

    //Vision Job File ����.
    AnsiString sPath = EXE_FOLDER + "JobFile\\" + sName;

    SV_LoadJobFile(sPath.c_str());

    OM.LoadJobFile (sName );                                                   pbStatus -> Position = 0   ;
    DM.ARAY[riLDR   ].SetMaxColRow(1                  , OM.DevInfo.iCsSlCnt);  pbStatus -> Position = 10  ;
    DM.ARAY[riULD   ].SetMaxColRow(1                  , OM.DevInfo.iCsSlCnt);  pbStatus -> Position = 20  ;
    DM.ARAY[riPRB   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );  pbStatus -> Position = 30  ;
    DM.ARAY[riWRK   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );  pbStatus -> Position = 40  ;
    DM.ARAY[riPSB   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );  pbStatus -> Position = 50  ;
    DM.ClearMap    (      );                                                   pbStatus -> Position = 60  ;
    OM.SaveLastInfo(      );                                                   pbStatus -> Position = 80  ;
    PM.Load        (sName );                                                   pbStatus -> Position = 100 ;

    //DM.ARAY[riPR2   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );  pbStatus -> Position = 40  ;
    //DM.ARAY[riWRP   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );  pbStatus -> Position = 50  ;
    //DM.ARAY[riPS2   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );  pbStatus -> Position = 80  ;
    DM.ARAY[riTmp].SetMaxColRow(OM.DevOptn.iContColCnt , OM.DevInfo.iRowCnt);

    /*
    if(OM.DevOptn.bLtHeaterNum2) IO_SetY(yETC_Heater1_2,true );
    else                         IO_SetY(yETC_Heater1_2,false);
    if(OM.DevOptn.bLtHeaterNum3) IO_SetY(yETC_Heater1_3,true );
    else                         IO_SetY(yETC_Heater1_3,false);
    if(OM.DevOptn.bRtHeaterNum2) IO_SetY(yETC_Heater2_2,true );
    else                         IO_SetY(yETC_Heater2_2,false);
    if(OM.DevOptn.bRtHeaterNum3) IO_SetY(yETC_Heater2_3,true );
    else                         IO_SetY(yETC_Heater2_3,false);

    //TODO :: ������... JS
    */
    FrmProbe -> Init();

//    PCM.Set1();
//    PCM.Set2();

    lbSelDevice  -> Caption = "";
    edFromName   -> Text    = "";
    lbCrntDevice -> Caption = sName ;

    //Trace Log.
    Trace("",sTemp.c_str());

    FrmConversion -> Show() ;

    //if(OM.MstOptn.bUseDaegyeomProbe) PCM.SetDaegyeom();
    //else                             FrmMain->tmMainSet->Enabled = true ;
    PCM.SetDaegyeom();


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
void __fastcall TFrmDevice::FormDestroy(TObject *Sender)
{
    tmUpdate->Enabled = false;
}
//---------------------------------------------------------------------------
