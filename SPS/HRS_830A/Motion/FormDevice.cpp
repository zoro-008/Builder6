//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDevice.h"
//#include "FormMain.h"
#include "FormDeviceSet.h"
//---------------------------------------------------------------------------
#include "UserFile.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "UtilDefine.h"
#include "SLogUnit.h"
#include "SMinterfaceUnit.h"
#include "LotUnit.h"
#include "PstnMan.h"

#include "Unit1.h"
#include "LanguageUnit.h"
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

    m_WorkNewBackUp    = false;
    m_WorkCopyBackUp   = false;
    m_WorkRenameBackUp = false;


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
    if(LTQ.GetLotOpen()) {
        FM_MsgOk("Error" , "Can't Change JobFile While LotOpen");
        return;
    }

    if (FM_MsgYesNo("Confirm","Are you Sure?") != mrYes) return ;

    //JOB File 적용.
    AnsiString sName = sgDevice -> Cells[1][sgDevice -> Row];
    AnsiString sTemp = "DOWNLOAD JOB FILE. (" + lbSelDevice -> Caption + ")";

    OM.LoadJobFile (sName );                                                      pbStatus -> Position = 0  ;
    DM.ARAY[riLDR].SetMaxColRow(1                     , OM.DevInfo.iCsSlCnt   );  pbStatus -> Position = 5  ;
    DM.ARAY[riPRB].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );  pbStatus -> Position = 10 ;
    DM.ARAY[riLWK].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );  pbStatus -> Position = 25 ;
    DM.ARAY[riRWK].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );  pbStatus -> Position = 45 ;
    DM.ARAY[riPSB].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );  pbStatus -> Position = 70 ;
    DM.ARAY[riULD].SetMaxColRow(1                     , OM.DevInfo.iCsSlCnt   );  pbStatus -> Position =100 ;

    PM.Load(sName);

    lbSelDevice  -> Caption = "";
    edFromName   -> Text    = "";
    lbCrntDevice -> Caption = sName ;

    //Trace Log.
    Trace("",sTemp.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::FormShow(TObject *Sender)
{
    //StringGrid를 Update한다.
    UserFile.GridSearchDir(ExtractFilePath(Application -> ExeName) + "JobFile" , sgDevice , 1 , true);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬

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

    edAutoDevice -> SetFocus();

    tmUpdate -> Enabled = true;

}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::btNewClick(TObject *Sender)
{
    TButton *Btn = dynamic_cast<TButton *>(Sender);
    int index = Btn->Tag ;

    AnsiString Path      ;
    AnsiString ServerPath;
    AnsiString sLog;


    Path       = ExtractFilePath(Application -> ExeName) + "JobFile\\" ;
    ServerPath = OM.CmnOptn.sServerPath                  + "\\"        ;

    if(!UserFile.DirExists(Path)) UserFile.CreateDir(Path      ) ;
    if(!UserFile.DirExists(Path)) UserFile.CreateDir(ServerPath) ;

    switch (index) {
    	case 1: //New.
             if (edToName -> Text == "") {   //아무것도 입력이 되지 않은 경우
                 FM_MsgOk("Error" , "입력된 JOB FILE 이름이 없습니다." );
                 return;
             }

             if (FM_MsgYesNo("Confirm" , (edToName -> Text + " JOBFILE을 생성하시겠습니까?").c_str()) == mrNo) return ;

             Path       = ExtractFilePath(Application -> ExeName) + "JobFile\\" + edToName -> Text ;
             ServerPath = OM.CmnOptn.sServerPath                  + "\\"        + edToName -> Text ;


             if(!m_WorkNewBackUp){
                 if (!UserFile.CreateDir(Path)) {
                     FM_MsgOk("Warning","백업 폴더에 같은 이름의 File이 존재합니다.");
                     m_WorkNewBackUp = true;
                     return ;
                 }
             }

             if (!UserFile.CreateDir(ServerPath)) {
                 FM_MsgOk("Warning","서버 폴더에 같은 이름의 File이 존재합니다.");
                 return ;
             }
             m_WorkNewBackUp = false;

             //Trace Log.
             Trace("JobFile",(edToName -> Text + " is Maked").c_str());
             break;

        case 2: //Copy.
            //Check None Name.
            if (edFromName -> Text == "") {   //아무것도 선택되지 않은 경우.
                FM_MsgOk("Error" , "선택된 JOB FILE 이름이 없습니다." );
                return;
            }

            if (edToName -> Text == "") {   //아무것도 입력이 되지 않은 경우
                 FM_MsgOk("Error" , "입력된 JOB FILE 이름이 없습니다." );
                 return;
            }
            if (FM_MsgYesNo("Confirm" , (edFromName -> Text +"를"+ edToName -> Text + "로 JOB FILE을 복사 생성하시겠습니까?").c_str()) == mrNo) return ;

            Path       = ExtractFilePath(Application -> ExeName) + "JobFile\\" ;
            ServerPath = OM.CmnOptn.sServerPath                  + "\\"        ;


            if(!m_WorkCopyBackUp){
                if (UserFile.DirExists(Path + edToName -> Text)) {
                    FM_MsgOk("Warning" , "백업 폴더에 같은 이름의 JOB FILE이 존재합니다." );
                    return ;
                }
                UserFile.CopyDir(Path + edFromName -> Text, Path + edToName -> Text);
                m_WorkCopyBackUp = true;
            }

            if (UserFile.DirExists(ServerPath + edToName -> Text)) {
                FM_MsgOk("Warning" , "서버 폴더에 같은 이름의 JOB FILE이 존재합니다." );
                return ;
            }
            UserFile.CopyDir(ServerPath + edFromName -> Text, ServerPath + edToName -> Text);
            m_WorkCopyBackUp = false;

            //Trace Log.
            Trace("JobFile",(edToName -> Text + " is Maked by Copy").c_str());
            break;

        case 3: //Rename.
            //Check None Name.
            if (edFromName -> Text == "") {   //아무것도 선택되지 않은 경우.
                FM_MsgOk("Error" , "선택된 JOB FILE 이름이 없습니다." );
                return;
            }
            if (edToName -> Text == "") {   //아무것도 입력이 되지 않은 경우
	        FM_MsgOk("Error" , "입력된 JOB FILE 이름이 없습니다." );
                return;
            }

            if (FM_MsgYesNo("Confirm","JOB FILE 이름을 변경 하시겠습니까?") == mrNo) return ;

            Path       = ExtractFilePath(Application -> ExeName) + "JobFile\\";
            ServerPath = OM.CmnOptn.sServerPath                  + "\\"       ;

            if(!m_WorkRenameBackUp){
                if (UserFile.DirExists(Path + edToName -> Text)) {
                    FM_MsgOk("Warning" , "백업 폴더에 같은 이름의 JOB FILE이 존재합니다." );
                    return ;
                }
                RenameFile(Path + edFromName -> Text , Path + edToName -> Text);
                m_WorkRenameBackUp = true;
            }

            if (UserFile.DirExists(ServerPath + edToName -> Text)) {
                FM_MsgOk("Warning" , "서버 폴더에 같은 이름의 JOB FILE이 존재합니다." );
                return ;
            }
            RenameFile(ServerPath + edFromName -> Text , Path + edToName -> Text);
            m_WorkRenameBackUp = false;

            OM.SetCrntDev(edToName -> Text);

            //Trace Log.
            Trace("JobFile",(edToName -> Text + " is Maked by Rename").c_str());

        	break;
        case 4: //Delete
            //Check None Name.
            if (edFromName -> Text == "") {   //아무것도 선택되지 않은 경우.
        	FM_MsgOk("Error" , "선택된 JOB FILE 이름이 없습니다." );
                return;
            }

            if (FM_MsgYesNo("Confirm" , "선택한 JOB FILE을 지우시겠습니까?" ) == mrNo) return ;
            if (edFromName -> Text == OM.GetCrntDev()) {
                FM_MsgOk("Warning","현재 사용중 인JOB FILE은 삭제할 수 없습니다.");
                return ;
            }

            Path       = ExtractFilePath(Application -> ExeName) + "JobFile\\" + edFromName -> Text;
            ServerPath = OM.CmnOptn.sServerPath                  + "\\"        + edFromName -> Text;

            UserFile.ClearDir  (Path       , 1);
            UserFile.ClearDir  (ServerPath , 1);

            //Trace Log.
            Trace("JobFile",(edToName -> Text + " is Deleted").c_str());

        	break;
    }
    pbStatus->Position = 0;
    UserFile.GridSearchDir(ExtractFilePath(Application -> ExeName)+"JobFile" , sgDevice , 1 , true);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
    sgDevice -> Row = 0 ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false;

    if(edAutoDevice -> Text != ""){
        if(LTQ.GetLotOpen()) {
            FM_MsgOk("Error" , "Can't Change JobFile While LotOpen");
            edAutoDevice -> Text   = "";
            tmUpdate -> Enabled = true;
            return;
        }
        if(!UserFile.DirExists(OM.CmnOptn.sServerPath + edToName -> Text)){
            FM_MsgOk("Error" , "JobFile is Not Exist");
            edAutoDevice -> Text   = "";
            tmUpdate -> Enabled = true;
            return;
        }

        if (FM_MsgYesNo("Confirm","Are you Sure?") != mrYes){
            edAutoDevice -> Text   = "";
            tmUpdate -> Enabled = true;
            return ;
        }

        //JOB File 적용.
        AnsiString sName = edAutoDevice -> Text;
        AnsiString sTemp = "DOWNLOAD JOB FILE. (" + edAutoDevice -> Text + ")";

        OM.LoadJobFile (sName );                                                      pbStatus -> Position = 0  ;
        DM.ARAY[riLDR].SetMaxColRow(1                     , OM.DevInfo.iCsSlCnt   );  pbStatus -> Position = 5  ;
        DM.ARAY[riPRB].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );  pbStatus -> Position = 10 ;
        DM.ARAY[riLWK].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );  pbStatus -> Position = 25 ;
        DM.ARAY[riRWK].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );  pbStatus -> Position = 45 ;
        DM.ARAY[riPSB].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );  pbStatus -> Position = 70 ;
        DM.ARAY[riULD].SetMaxColRow(1                     , OM.DevInfo.iCsSlCnt   );  pbStatus -> Position =100 ;

        PM.Load(sName);

        lbSelDevice  -> Caption = "";
        edFromName   -> Text    = "";
        edAutoDevice -> Text   = "";
        lbCrntDevice -> Caption = sName ;

    }




    lbCrntDevice -> Caption = OM.GetCrntDev() ;
    tmUpdate -> Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::FormDestroy(TObject *Sender)
{
    tmUpdate->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDevice::edAutoDeviceKeyPress(TObject *Sender,
      char &Key)
{

    if(Key == VK_RETURN){
        AnsiString sLotId;
        sLotId = ((TEdit*)Sender)->Text;
        edAutoDevice  -> Text = sLotId;
        ((TEdit*)Sender)->Text = "";
    }

}
//---------------------------------------------------------------------------

