//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmDevice.h"
#include "FrmDeviceSet.h"
#include "FrmMotorPos.h"
#include "FrmInOutState.h"
#include "FrmOption.h"
#include "FrmReviewSelect.h"
//---------------------------------------------------------------------------
#include "UserFile.h"
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
    
    //StringGrid를 Update한다.
    UserFile.GridSearchDir(ExtractFilePath(Application -> ExeName) + "JobFile" , sgDevice , 0 , true);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬

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

    if(!IO_GetX(xHED_1Ready) && !OM.CmnOptn.bVs1Skip   && OM.CmnOptn.bAutoVsChange ) { FM_MsgOk("ERROR","Vision1 is not Ready!") ; return ; }
    if(!IO_GetX(xHED_2Ready) && !OM.CmnOptn.bVs2Skip   && OM.CmnOptn.bAutoVsChange ) { FM_MsgOk("ERROR","Vision2 is not Ready!") ; return ; }
    if(!IO_GetX(xHED_3Ready) && !OM.CmnOptn.bVs3Skip   && OM.CmnOptn.bAutoVsChange ) { FM_MsgOk("ERROR","Vision3 is not Ready!") ; return ; }
    if(!IO_GetX(xHED_4Ready) && !OM.CmnOptn.bVsMrkSkip && OM.CmnOptn.bAutoVsChange ) { FM_MsgOk("ERROR","Vision4 is not Ready!") ; return ; }

    //JOB File 적용.
    AnsiString sName = sgDevice -> Cells[1][sgDevice -> Row];
    AnsiString sTemp = "DOWNLOAD JOB FILE. (" + lbSelDevice -> Caption + ")";

    OM.LoadJobFile (sName );                                                   pbStatus -> Position = 0   ;
    DM.ARAY[riLDR   ].SetMaxColRow(1                  , OM.DevInfo.iCsSlCnt);  pbStatus -> Position = 10  ;
    DM.ARAY[riPRB   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );  pbStatus -> Position = 15  ;
    DM.ARAY[riWR1   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );  pbStatus -> Position = 20  ;
    DM.ARAY[riVS1   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );  pbStatus -> Position = 25  ;
    DM.ARAY[riWR2   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );  pbStatus -> Position = 35  ;
    DM.ARAY[riVS2   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );  pbStatus -> Position = 40  ;
    DM.ARAY[riWR3   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );  pbStatus -> Position = 45  ;
    DM.ARAY[riVS3   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );  pbStatus -> Position = 55  ;
    DM.ARAY[riVS4   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );  pbStatus -> Position = 59  ;
    DM.ARAY[riVB4   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );  pbStatus -> Position = 59  ;
    DM.ARAY[riPSB   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );  pbStatus -> Position = 60  ;
    DM.ARAY[riULD   ].SetMaxColRow(1                  , OM.DevInfo.iCsSlCnt);  pbStatus -> Position = 70  ;

    HED.DpAray1.SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );  pbStatus -> Position = 73  ;
    HED.DpAray2.SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );  pbStatus -> Position = 80  ;
    HED.DpAray3.SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );  pbStatus -> Position = 88  ;

    DM.ClearMap    (      );                                                   pbStatus -> Position = 90  ;
    OM.SaveLastInfo(      );                                                   pbStatus -> Position = 95  ;
    PM.Load        (sName );                                                   pbStatus -> Position = 100 ;

    FormReviewSelect->ReadArayData(riVB4);

    lbSelDevice  -> Caption = "";
    edFromName   -> Text    = "";
    lbCrntDevice -> Caption = sName ;

    if(OM.CmnOptn.bAutoVsChange)VC.SendModelChange();

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


    Path = ExtractFilePath(Application -> ExeName) + "JobFile\\" ;
    if(!UserFile.DirExists(Path)) UserFile.CreateDir(Path) ;

    switch (index) {
    	case 1: //New.
             if (edToName -> Text == "") {   //아무것도 입력이 되지 않은 경우
                 FM_MsgOk("Error" , "입력된 JOB FILE 이름이 없습니다." );
                 return;
             }

             if (FM_MsgYesNo("Confirm" , edToName -> Text + " JOBFILE을 생성하시겠습니까?") == mrNo) return ;

             Path = ExtractFilePath(Application -> ExeName) + "JobFile\\" + edToName -> Text ;
             if (!UserFile.CreateDir(Path)) {
                 FM_MsgOk("Warning","같은 이름의 File이 존재합니다.");
                 return ;
             }

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
            if (FM_MsgYesNo("Confirm" , edFromName -> Text +"를"+ edToName -> Text + "로 JOB FILE을 복사 생성하시겠습니까?") == mrNo) return ;

            Path = ExtractFilePath(Application -> ExeName) + "JobFile\\" ;
            if (UserFile.DirExists(Path + edToName -> Text)) {
                FM_MsgOk("Warning" , "같은 이름의 JOB FILE이 존재합니다." );
                return ;
            }

            UserFile.CopyDir(Path + edFromName -> Text, Path + edToName -> Text);

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

            Path = ExtractFilePath(Application -> ExeName) + "JobFile\\";

            if (UserFile.DirExists(Path + edToName -> Text)) {
                FM_MsgOk("Warning" , "같은 이름의 JOB FILE이 존재합니다." );
                return ;
            }
            RenameFile(Path + edFromName -> Text , Path + edToName -> Text);

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
            if (edFromName -> Text == OM.m_sCrntDev) {
                FM_MsgOk("Warning","현재 사용중 인JOB FILE은 삭제할 수 없습니다.");
                return ;
            }

            Path = ExtractFilePath(Application -> ExeName) + "JobFile\\" + edFromName -> Text;
            UserFile.ClearDir  (Path , 1);

            //Trace Log.
            Trace("JobFile",(edToName -> Text + " is Deleted").c_str());

        	break;
    }
    pbStatus->Position = 0;
    UserFile.GridSearchDir(ExtractFilePath(Application -> ExeName)+"JobFile" , sgDevice , 1 , true);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
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

