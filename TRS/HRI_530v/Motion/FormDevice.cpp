//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDevice.h"
//#include "FormMain.h"
#include "FormDeviceSet.h"
//---------------------------------------------------------------------------
#include "Loader.h"
#include "LoaderSupply.h"
#include "LoaderStock.h"

#include "Stage.h"
#include "StageVT.h"
#include "StageOST.h"
#include "StageVision.h"

#include "Unloader.h"
#include "UnloaderSupply.h"
#include "UnloaderStock.h"

#include "Sort.h"
#include "Sequence.h"
//---------------------------------------------------------------------------

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
    bool bAllArayNone = DM.ARAY[riSRT].CheckAllStat(csNone) && DM.ARAY[riZIG].CheckAllStat(csNone) && DM.ARAY[riSTF].CheckAllStat(csNone) &&
                        DM.ARAY[riSTL].CheckAllStat(csNone) && DM.ARAY[riSTB].CheckAllStat(csNone) && DM.ARAY[riSTR].CheckAllStat(csNone) &&
                        DM.ARAY[riRJ1].CheckAllStat(csNone) && DM.ARAY[riRJ2].CheckAllStat(csNone) && DM.ARAY[riRJ3].CheckAllStat(csNone) &&
                        DM.ARAY[riRJ4].CheckAllStat(csNone) && DM.ARAY[riPRL].CheckAllStat(csNone) && DM.ARAY[riLDR].CheckAllStat(csNone) &&
                        DM.ARAY[riPSL].CheckAllStat(csNone) && DM.ARAY[riPRU].CheckAllStat(csNone) && DM.ARAY[riULD].CheckAllStat(csNone) &&
                        DM.ARAY[riPSU].CheckAllStat(csNone) ;


    //if(LTQ.GetLotOpen()) {
   // if(!bAllArayNone){
   //      FM_MsgOk("Error" , "������ �ް����� ���� ������ �������� �ٲܼ� �����ϴ�.");
   //      return;
   //  }

    //JOB File ����.
    AnsiString sName = sgDevice -> Cells[1][sgDevice -> Row];
    AnsiString sTemp = "DOWNLOAD JOB FILE. (" + lbSelDevice -> Caption + ")";

    if(OM.GetCrntDev() == sName) {
        FM_MsgOk("ERROR","���� �����ϰ� ���� ������ �Դϴ�.");
        return ;
    }

    if (FM_MsgYesNo("Confirm","Are you Sure?") != mrYes) return ;

    OM.LoadJobFile (sName );                                                      pbStatus -> Position = 0  ;

    DM.ARAY[riLDR].SetMaxColRow(OM.DevInfo.iLDRColCnt    , OM.DevInfo.iLDRRowCnt  ); DM.ARAY[riLDR].SetLotNo("");
    DM.ARAY[riPSL].SetMaxColRow(OM.DevInfo.iLDRColCnt    , OM.DevInfo.iLDRRowCnt  ); DM.ARAY[riPSL].SetLotNo("");
    DM.ARAY[riPRL].SetMaxColRow(OM.DevInfo.iLDRColCnt    , OM.DevInfo.iLDRRowCnt  ); DM.ARAY[riPRL].SetLotNo("");
    DM.ARAY[riULD].SetMaxColRow(OM.DevInfo.iULDColCnt    , OM.DevInfo.iULDRowCnt  ); DM.ARAY[riULD].SetLotNo("");
    DM.ARAY[riPRU].SetMaxColRow(OM.DevInfo.iULDColCnt    , OM.DevInfo.iULDRowCnt  ); DM.ARAY[riPRU].SetLotNo("");
    DM.ARAY[riPSU].SetMaxColRow(OM.DevInfo.iULDColCnt    , OM.DevInfo.iULDRowCnt  ); DM.ARAY[riPSU].SetLotNo("");
    DM.ARAY[riRJ1].SetMaxColRow(OM.DevInfo.iULDColCnt    , OM.DevInfo.iULDRowCnt  ); DM.ARAY[riRJ1].SetLotNo("");
    DM.ARAY[riRJ2].SetMaxColRow(OM.DevInfo.iULDColCnt    , OM.DevInfo.iULDRowCnt  ); DM.ARAY[riRJ2].SetLotNo("");
    DM.ARAY[riRJ3].SetMaxColRow(OM.DevInfo.iULDColCnt    , OM.DevInfo.iULDRowCnt  ); DM.ARAY[riRJ3].SetLotNo("");
    DM.ARAY[riRJ4].SetMaxColRow(OM.DevInfo.iULDColCnt    , OM.DevInfo.iULDRowCnt  ); DM.ARAY[riRJ4].SetLotNo("");
    DM.ARAY[riZIG].SetMaxColRow(MAX_TOOL_SOCK_COL        , 1                      ); DM.ARAY[riZIG].SetLotNo("");
    DM.ARAY[riSRT].SetMaxColRow(MAX_TOOL_SOCK_COL        , 1                      ); DM.ARAY[riSRT].SetLotNo("");

    DM.ARAY[riMOS].SetMaxColRow(1                        , MAX_TOOL_SOCK_COL      ); DM.ARAY[riMOS].SetLotNo("");
    DM.ARAY[riMVT].SetMaxColRow(MAX_TOOL_SOCK_COL        , 1                      ); DM.ARAY[riMVT].SetLotNo("");
    DM.ARAY[riMVS].SetMaxColRow(1                        , MAX_TOOL_SOCK_COL      ); DM.ARAY[riMVS].SetLotNo("");


    DM.ARAY[riMOS].SetMaxColRow(1                        , MAX_TOOL_SOCK_COL      ); DM.ARAY[riMOS].SetLotNo("");
    DM.ARAY[riMVT].SetMaxColRow(MAX_TOOL_SOCK_COL        , 1                      ); DM.ARAY[riMVT].SetLotNo("");
    DM.ARAY[riMVS].SetMaxColRow(1                        , MAX_TOOL_SOCK_COL      ); DM.ARAY[riMVS].SetLotNo("");

    DM.ARAY[riSTF].SetMaxColRow(MAX_TOOL_SOCK_COL        , 1                      ); DM.ARAY[riSTF].SetLotNo("");
    DM.ARAY[riSTL].SetMaxColRow(1                        , MAX_TOOL_SOCK_COL      ); DM.ARAY[riSTL].SetLotNo("");
    DM.ARAY[riSTB].SetMaxColRow(MAX_TOOL_SOCK_COL        , 1                      ); DM.ARAY[riSTB].SetLotNo("");
    DM.ARAY[riSTR].SetMaxColRow(1                        , MAX_TOOL_SOCK_COL      ); DM.ARAY[riSTR].SetLotNo("");
   
    PM.Load(sName);

    lbSelDevice  -> Caption = "";
    edFromName   -> Text    = "";
    lbCrntDevice -> Caption = sName ;

    //���...
    if(OM.CmnOptn.bVisnAutoConv)VSN.VisnJobChange();

    String sMsg = "[DVC,"+AnsiString(OM.DevOptn.iOSTDeviceNo)+"#" ;
    OST.m_pRs232_OST -> SendData(sMsg.Length(),sMsg.c_str());


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
                 FM_MsgOk("Error" , "�Էµ� JOB FILE �̸��� �����ϴ�." );
                 return;
             }

             if (FM_MsgYesNo("Confirm" , (edToName -> Text + " JOBFILE�� �����Ͻðڽ��ϱ�?").c_str()) == mrNo) return ;

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
            if (FM_MsgYesNo("Confirm" , (edFromName -> Text +"��"+ edToName -> Text + "�� JOB FILE�� ���� �����Ͻðڽ��ϱ�?").c_str()) == mrNo) return ;

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
            if (edFromName -> Text == OM.GetCrntDev()) {
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
    lbCrntDevice -> Caption = OM.GetCrntDev() ;

    //������ ü���� �Ҷ� SPC���� ��� ���� �Ҵ� �ٽ� �ϱ⶧���� ������� �Ѵ�   SEQ._iSeqStat == ssStop �߿�.
    btDownload -> Enabled = SEQ._iSeqStat == ssStop;//g_bLaserChangeFinish && g_bMarkChangeFinish && 

    tmUpdate -> Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::FormDestroy(TObject *Sender)
{
    tmUpdate->Enabled = false;
}
//---------------------------------------------------------------------------

