//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDevice.h"
//#include "FormMain.h"
#include "FormDeviceSet.h"
//---------------------------------------------------------------------------
//Part Reference.
//---------------------------------------------------------------------------
#include "Loader.h"
#include "Index.h"
#include "Picker.h"
#include "Stage.h"
#include "Dispensor.h"
#include "Unloader.h"
//---------------------------------------------------------------------------

#include "Sequence.h"
//---------------------------------------------------------------------------

#include "UserFile.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "UtilDefine.h"
#include "SLogUnit.h"
#include "SMinterfaceUnit.h"
#include "SVinterface.h"
#include "LotUnit.h"
#include "Pattern.h"
#include "PstnMan.h"
#include "Rs232Temp.h"
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
    bool bAllArayNone = DM.ARAY[riLDR_FTop].CheckAllStat(csNone) && DM.ARAY[riLDR_FBtm].CheckAllStat(csNone) &&
                        DM.ARAY[riLDR_RTop].CheckAllStat(csNone) && DM.ARAY[riLDR_RBtm].CheckAllStat(csNone) &&
                        DM.ARAY[riPRI_F   ].CheckAllStat(csNone) && DM.ARAY[riPRI_R   ].CheckAllStat(csNone) &&
                        DM.ARAY[riIDX_F   ].CheckAllStat(csNone) && DM.ARAY[riIDX_R   ].CheckAllStat(csNone) &&
                        DM.ARAY[riPSI_F   ].CheckAllStat(csNone) && DM.ARAY[riPSI_R   ].CheckAllStat(csNone) &&
                        DM.ARAY[riULD_FTop].CheckAllStat(csNone) && DM.ARAY[riULD_FBtm].CheckAllStat(csNone) &&
                        DM.ARAY[riULD_RTop].CheckAllStat(csNone) && DM.ARAY[riULD_RBtm].CheckAllStat(csNone) &&
                        DM.ARAY[riPCK     ].CheckAllStat(csNone) && DM.ARAY[riALN     ].CheckAllStat(csNone) &&
                        DM.ARAY[riDSP     ].CheckAllStat(csNone) && DM.ARAY[riSTG     ].CheckAllStat(csNone) ;


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

    DM.ARAY[riLDR_FTop].SetMaxColRow(1                        , OM.DevInfo.iMgzSlotCnt ); DM.ARAY[riLDR_FTop].SetLotNo("");
    DM.ARAY[riLDR_FBtm].SetMaxColRow(1                        , OM.DevInfo.iMgzSlotCnt ); DM.ARAY[riLDR_FBtm].SetLotNo("");
    DM.ARAY[riLDR_RTop].SetMaxColRow(1                        , OM.DevInfo.iMgzSlotCnt ); DM.ARAY[riLDR_RTop].SetLotNo("");
    DM.ARAY[riLDR_RBtm].SetMaxColRow(1                        , OM.DevInfo.iMgzSlotCnt ); DM.ARAY[riLDR_RBtm].SetLotNo("");
    DM.ARAY[riPRI_F   ].SetMaxColRow(OM.DevInfo.iBoatColCnt   , 1                      ); DM.ARAY[riPRI_F   ].SetLotNo("");
    DM.ARAY[riPRI_R   ].SetMaxColRow(OM.DevInfo.iBoatColCnt   , 1                      ); DM.ARAY[riPRI_R   ].SetLotNo("");
    DM.ARAY[riIDX_F   ].SetMaxColRow(OM.DevInfo.iBoatColCnt   , 1                      ); DM.ARAY[riIDX_F   ].SetLotNo("");
    DM.ARAY[riIDX_R   ].SetMaxColRow(OM.DevInfo.iBoatColCnt   , 1                      ); DM.ARAY[riIDX_R   ].SetLotNo("");
    DM.ARAY[riPSI_F   ].SetMaxColRow(OM.DevInfo.iBoatColCnt   , 1                      ); DM.ARAY[riPSI_F   ].SetLotNo("");
    DM.ARAY[riPSI_R   ].SetMaxColRow(OM.DevInfo.iBoatColCnt   , 1                      ); DM.ARAY[riPSI_R   ].SetLotNo("");
    DM.ARAY[riULD_FTop].SetMaxColRow(1                        , OM.DevInfo.iMgzSlotCnt ); DM.ARAY[riULD_FTop].SetLotNo("");
    DM.ARAY[riULD_FBtm].SetMaxColRow(1                        , OM.DevInfo.iMgzSlotCnt ); DM.ARAY[riULD_FBtm].SetLotNo("");
    DM.ARAY[riULD_RTop].SetMaxColRow(1                        , OM.DevInfo.iMgzSlotCnt ); DM.ARAY[riULD_RTop].SetLotNo("");
    DM.ARAY[riULD_RBtm].SetMaxColRow(1                        , OM.DevInfo.iMgzSlotCnt ); DM.ARAY[riULD_RBtm].SetLotNo("");
    DM.ARAY[riPCK     ].SetMaxColRow(1                        , 1                      ); DM.ARAY[riPCK     ].SetLotNo("");
    DM.ARAY[riALN     ].SetMaxColRow(1                        , 1                      ); DM.ARAY[riALN     ].SetLotNo("");
    DM.ARAY[riDSP     ].SetMaxColRow(1                        , 1                      ); DM.ARAY[riDSP     ].SetLotNo("");
    DM.ARAY[riSTG     ].SetMaxColRow(1                        , 1                      ); DM.ARAY[riSTG     ].SetLotNo("");

    PM.Load(sName);

    PTT.Load(sName);

    //JS
    if(!SKIP_VISN)SV_LoadJobFile(sName.c_str());

    lbSelDevice  -> Caption = "";
    edFromName   -> Text    = "";
    lbCrntDevice -> Caption = sName ;

    Rs232_TempCh.SetTempAll(OM.DevOptn.iRailBtmTemp,
                            OM.DevOptn.iRailTopTemp,
                            OM.DevOptn.iStageTemp  );

    //���̺��ư ������ , ����̽��V ũ����Ʈ�Ҷ� , ����̽� ü���� ������...
    DSP.Rs232_Dispr.SetLoadCh(OM.DevOptn.iDspCh+1);
    CDelayTimer TimeOut;
    TimeOut.Clear();
    while(!DSP.Rs232_Dispr.GetMsgEnd()){ //�޼��� �� �ְ� ���������� ��ٸ�.
        Sleep(1);
        if(TimeOut.OnDelay(true , 1000)){
            break ;
        }
    }
    //DSP.Rs232_Dispr.SetPTV(OM.DevOptn.dDspPrsPres , 10 , OM.DevOptn.dDspVacPres);
    DSP.Rs232_Dispr.GetDispData(OM.DevOptn.iDspCh+1);
    TimeOut.Clear();
    while(!DSP.Rs232_Dispr.GetMsgEnd()){ //�޼��� �� �ְ� ���������� ��ٸ�.
        Sleep(1);
        if(TimeOut.OnDelay(true , 3000)){
            break ;
        }
    }

    OM.DevOptn.dDspVacPres = DSP.Rs232_Dispr.GetDispData().dVacPres ;//       ;
    OM.DevOptn.dDspPrsPres = DSP.Rs232_Dispr.GetDispData().dPrsPres ;//
    OM.SaveDevOptn(OM.GetCrntDev());







    //Rs232_TempCh.SetTemp(tcRailBtm , OM.DevOptn.iRailBtmTemp);
    //Rs232_TempCh.SetTemp(tcRailTop , OM.DevOptn.iRailTopTemp);
    //Rs232_TempCh.SetTemp(tcStage   , OM.DevOptn.iStageTemp  );



    //���...
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
            //JS
            if(!SKIP_VISN)SV_LoadJobFile(edToName -> Text.c_str());

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
    btDownload -> Enabled = SEQ._iSeqStat == ssStop || SEQ._iSeqStat == ssError;//g_bLaserChangeFinish && g_bMarkChangeFinish && 

    tmUpdate -> Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::FormDestroy(TObject *Sender)
{
    tmUpdate->Enabled = false;
}
//---------------------------------------------------------------------------

