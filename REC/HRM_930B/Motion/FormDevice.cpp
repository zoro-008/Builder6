//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDevice.h"
//#include "FormMain.h"
#include "FormDeviceSet.h"
//---------------------------------------------------------------------------
//Part Reference.
//---------------------------------------------------------------------------
#include "LoaderFront.h"
#include "LoaderRear.h"
#include "Index.h"
#include "Stage.h"
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
#include "LotUnit.h"
#include "PstnMan.h"
#include "VisionCom.h"

#include "CadPosition.h"
#include "Rs232Dispr.h"
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



    //Check Running Status.
    bool bAllArayNone = DM.ARAY[riLDR_F].CheckAllStat(csNone) && DM.ARAY[riLDR_F].CheckAllStat(csNone) &&
                        DM.ARAY[riLDR_R].CheckAllStat(csNone) && DM.ARAY[riLDR_R].CheckAllStat(csNone) &&
                        DM.ARAY[riIDX  ].CheckAllStat(csNone) && DM.ARAY[riIDX  ].CheckAllStat(csNone) &&
                        DM.ARAY[riRTT  ].CheckAllStat(csNone) && DM.ARAY[riRTT  ].CheckAllStat(csNone) &&
                        DM.ARAY[riTRF  ].CheckAllStat(csNone) && DM.ARAY[riTRF  ].CheckAllStat(csNone) &&
                        DM.ARAY[riSTG  ].CheckAllStat(csNone) && DM.ARAY[riSTG  ].CheckAllStat(csNone) &&
                        DM.ARAY[riULD  ].CheckAllStat(csNone) && DM.ARAY[riULD  ].CheckAllStat(csNone) ;
    

    if(LOT.GetLotOpen()) {
        if(!bAllArayNone){
            FM_MsgOk("Error" , "������ �ް����� ���� ������ �������� �ٲܼ� �����ϴ�.");
            return;
        }
    }

    //JOB File ����.
    AnsiString sName = sgDevice -> Cells[1][sgDevice -> Row];
    AnsiString sTemp = "DOWNLOAD JOB FILE. (" + lbSelDevice -> Caption + ")";

    if(OM.GetCrntDev() == sName) {
        FM_MsgOk("ERROR","���� �����ϰ� ���� ������ �Դϴ�.");
        return ;
    }

    if (FM_MsgYesNo("Confirm","Are you Sure?") != mrYes) return ;


    OM .LoadJobFile (sName );                                                      pbStatus -> Position = 0   ;
    CAD.SetRowCol(OM.DevInfo.iMidColCnt , OM.DevInfo.iMidRowCnt);                  pbStatus -> Position = 20  ;
    CAD.LoadSave(true , sName);                                                    pbStatus -> Position = 30  ;
    CAD_Rtt.LoadSave(true , sName);                                                pbStatus -> Position = 50  ;
    PM .Load(sName);                                                               pbStatus -> Position = 70  ;
    DM .ARAY[riLDR_F].SetMaxColRow(OM.DevInfo.iLDRFMgzBayCnt, OM.DevInfo.iLDRFMgzSlotCnt ); DM.ARAY[riLDR_F].SetLotNo("");
    DM .ARAY[riLDR_R].SetMaxColRow(OM.DevInfo.iLDRRMgzBayCnt, OM.DevInfo.iLDRRMgzSlotCnt ); DM.ARAY[riLDR_R].SetLotNo("");
    DM .ARAY[riIDX  ].SetMaxColRow(1                        , 1                          ); DM.ARAY[riIDX  ].SetLotNo("");
    DM .ARAY[riRTT  ].SetMaxColRow(1                        , 1                          ); DM.ARAY[riRTT  ].SetLotNo("");
    DM .ARAY[riTRF  ].SetMaxColRow(1                        , 1                          ); DM.ARAY[riTRF  ].SetLotNo("");
    DM .ARAY[riWKO  ].SetMaxColRow(1                        , 1                          ); DM.ARAY[riWKO  ].SetLotNo("");
    DM .ARAY[riPTL  ].SetMaxColRow(1                        , 1                          ); DM.ARAY[riPTL  ].SetLotNo("");
	DM .ARAY[riSTG  ].SetMaxColRow(OM.DevInfo.iMidColCnt    , OM.DevInfo.iMidRowCnt      ); DM.ARAY[riSTG  ].SetLotNo("");
    DM .ARAY[riULD  ].SetMaxColRow(OM.DevInfo.iULDMgzBayCnt , OM.DevInfo.iULDMgzSlotCnt  ); DM.ARAY[riULD  ].SetLotNo("");
                                                                                   pbStatus -> Position = 100 ;












    VSN_L.SendJobChange(sName);
    VSN_R.SendJobChange(sName);

    lbSelDevice  -> Caption = "";
    edFromName   -> Text    = "";
    lbCrntDevice -> Caption = sName ;

    //���̺��ư ������ , ����̽��V ũ����Ʈ�Ҷ� , ����̽� ü���� ������...
    //Rs232_DisprFt.SetPTV(OM.DevOptn.dDspPrsPres , 10 , OM.DevOptn.dDspVacPres);
    //Rs232_DisprRr.SetPTV(OM.DevOptn.dDspPrsPres , 10 , OM.DevOptn.dDspVacPres);
    CDelayTimer TimeOut;
    Rs232_DisprFt.SetLoadCh(OM.DevOptn.iDspChFt+1);
    TimeOut.Clear();
    while(!Rs232_DisprFt.GetMsgEnd()){ //�޼��� �� �ְ� ���������� ��ٸ�.
        Sleep(1);
        if(TimeOut.OnDelay(true , 1000)){
            FM_MsgOk("Error","����Ʈ ���漭 ä�� �ε� ���Ÿ�Ӿƿ�");
            break ;
        }
    }
    if(Rs232_DisprFt.GetErrMsg() != "") {
        FM_MsgOk("Disp Ft Error" , Rs232_DisprFt.GetErrMsg().c_str());
    }


    Rs232_DisprRr.SetLoadCh(OM.DevOptn.iDspChRr+1);
    TimeOut.Clear();
    while(!Rs232_DisprRr.GetMsgEnd()){ //�޼��� �� �ְ� ���������� ��ٸ�.
        Sleep(1);
        if(TimeOut.OnDelay(true , 1000)){
            FM_MsgOk("Error","���� ���漭 ä�� �ε� ���Ÿ�Ӿƿ�");
            break ;
        }
    }
    if(Rs232_DisprRr.GetErrMsg() != "") {
        FM_MsgOk("Disp Rr Error" , Rs232_DisprRr.GetErrMsg().c_str());
    }

    DM.ARAY[riLDR_F].SetStat(csNone) ;
    DM.ARAY[riLDR_R].SetStat(csNone) ;
    DM.ARAY[riIDX  ].SetStat(csNone) ;
    DM.ARAY[riRTT  ].SetStat(csNone) ;
    DM.ARAY[riTRF  ].SetStat(csNone) ;
    DM.ARAY[riSTG  ].SetStat(csNone) ;
    DM.ARAY[riULD  ].SetStat(csNone) ;



    //JobChange����..
    //VSN_L.SendJobChange(OM.GetCrntDev());
    //VSN_R.SendJobChange(OM.GetCrntDev());

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
    btDownload -> Enabled = (SEQ._iSeqStat == ssStop || SEQ._iSeqStat == ssError) &&
                             !IO_GetX(xVisnL_Busy) && !IO_GetX(xVisnR_Busy);//g_bLaserChangeFinish && g_bMarkChangeFinish &&

    tmUpdate -> Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::FormDestroy(TObject *Sender)
{
    tmUpdate->Enabled = false;
}
//---------------------------------------------------------------------------







