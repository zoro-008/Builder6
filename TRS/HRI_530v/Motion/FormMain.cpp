//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "FormMain.h"
#include "FormMaster.h"
#include "FormSPC.h"
#include "FormInform.h"
#include "FormLoading.h"

#include "MainThread.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "Sequence.h"
#include "OptionMan.h"
#include "ManualMan.h"
#include "LotUnit.h"
#include "StageVT.h"

#include<stdio.h>
#include<string.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMain *FrmMain;

AnsiString OnGetVer(const AnsiString &sQuery)
{

   //sQuery "CompanyName", "FileDescription", "FileVersion", "InternalName", "LegalCopyright", "LegalTradeMarks", "OriginalFileName", "ProductName", "ProductVersion", "Comments"

   DWORD dwHandle = 0, dwVersionInfoSize;
   UINT uLength;
   LPVOID pFileInfo, ptr;
   AnsiString sOut; // 리턴될 버전 정보.

   AnsiString filename = Application->ExeName;

   dwVersionInfoSize = GetFileVersionInfoSize(filename.c_str(), &dwHandle);

   pFileInfo = (LPVOID) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwVersionInfoSize);

   GetFileVersionInfo(filename.c_str(), dwHandle, dwVersionInfoSize, pFileInfo);
   VerQueryValue(pFileInfo, TEXT("\\VarFileInfo\\Translation"), &ptr, &uLength);

   WORD *id = (WORD *) ptr;
   AnsiString szString = "\\StringFileInfo\\" + IntToHex(id[0], 4) + IntToHex(id[1], 4) + "\\" + sQuery;

   VerQueryValue(pFileInfo, szString.c_str(), &ptr, &uLength);
   sOut = AnsiString((char *) ptr);
   HeapFree(GetProcessHeap(), 0, pFileInfo );
   return sOut;
}
//---------------------------------------------------------------------------
__fastcall TFrmMain::TFrmMain(TComponent* Owner)
    : TForm(Owner)
{

    //init
    Init();

    //Visible = false ;  //ssss

}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::Init(void)
{
    FM_MakeWin() ;
    
    m_iPageIndex = 0 ;

    //FrmMain
    Top    = 0 ;
    Left   = 0 ;
    Width  = 1280 ;
    Height = 1024 ;

    //return ;

    //Version
    lbDllVer -> Caption = "Dll " + AnsiString(FM_GetVer()) ;
    lbDllAge -> Caption = FM_GetAge() ;
    lbVer -> Caption = "Ver " + OnGetVer("FileVersion") ;
    int Age = FileAge(Application -> ExeName);
    AnsiString Date = FileDateToDateTime(Age).FormatString("''yyyy'_ 'm'_ 'd'_ 'AM/PM' 'h': 'n''");
    lbDate -> Caption = Date ;
    lbName -> Caption = OM.EqpOptn.sModelName;
    //Last Device Load
    OM.LoadLastInfo();
    IO_SetY(yLDR_ZBreak,false );
    IO_SetY(yULD_ZBreak,false );
    ::Sleep(100);

    MT_SetServoAll(true);
    IO_SetY(yETC_MainAir , true);

    pnSkipDisp -> Left = 0 ;
    pnSkipDisp -> Top  = 0 ;

}

//---------------------------------------------------------------------------
void __fastcall TFrmMain::btOperationShowClick(TObject *Sender)
{
    TButton *pButton = dynamic_cast<TButton *>(Sender);
    ClosePage(m_iPageIndex);
    m_iPageIndex = pButton->Tag;
    ShowPage (m_iPageIndex);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormShow(TObject *Sender)
{





    FrmOperation -> Parent = pnBase ; FrmOperation -> Left = 0 ; FrmOperation -> Top = 0 ; FrmOperation -> Width = 1280 ; FrmOperation -> Height = 863 ;
    FrmDevice    -> Parent = pnBase ; FrmDevice    -> Left = 0 ; FrmDevice    -> Top = 0 ; FrmDevice    -> Width = 1280 ; FrmDevice    -> Height = 863 ;
    FrmDeviceSet -> Parent = pnBase ; FrmDeviceSet -> Left = 0 ; FrmDeviceSet -> Top = 0 ; FrmDeviceSet -> Width = 1280 ; FrmDeviceSet -> Height = 863 ;
    FrmOption    -> Parent = pnBase ; FrmOption    -> Left = 0 ; FrmOption    -> Top = 0 ; FrmOption    -> Width = 1280 ; FrmOption    -> Height = 863 ;
    FrmSPC       -> Parent = pnBase ; FrmSPC       -> Left = 0 ; FrmSPC       -> Top = 0 ; FrmSPC       -> Width = 1280 ; FrmSPC       -> Height = 863 ;
    //FrmMaster    -> Parent = pnBase ; FrmMaster    -> Left = 0 ; FrmMaster    -> Top = 0 ; FrmMaster    -> Width = 1280 ; FrmMaster    -> Height = 863 ;


    ShowPage (0);

    static bool bFirst = true ;

    if(bFirst) {
        StartThread();
        bFirst = false ;
    }

    tmUpdate -> Enabled = true ;

}
//---------------------------------------------------------------------------
void __fastcall  TFrmMain::ShowPage (int index)
{
    switch(index)
    {
        case 0: FrmOperation -> Show()     ; break ;
        case 1: FrmDevice    -> Show()     ; break ;
        case 2: FrmOption    -> Show()     ; break ;
        case 3: FrmSPC       -> Show()     ; break ;
        case 4: FM_SetWin(pnBase-> Handle) ; break ;
    }
}
//---------------------------------------------------------------------------
void __fastcall  TFrmMain::ClosePage(int index)
{
    //FrmMotorPosition ->Close();
    switch(index)
    {
        case 0: FrmOperation -> Hide(); break ;
        case 1: FrmDevice    -> Hide();
                FrmDeviceSet -> Hide(); break ;
        case 2: FrmOption    -> Hide(); break ;
        case 3: FrmSPC       -> Hide(); break ;
        case 4: FM_HideWin()          ; break ;
    }
    FrmMaster -> Hide();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btExitClick(TObject *Sender)
{
    //Check Running Status.
    if (SEQ._bRun) {
        FM_MsgOk("Error" , "During Auto Running!!");
        return;
    }



    if(FM_MsgYesNo("Confirm","Are you Sure?") != mrYes) return ;
    tmUpdate -> Enabled = false ;
    EndThread();

    FrmLoading = new TFrmLoading(Application , false);
    ModalResult = FrmLoading->ShowModal();
    delete FrmLoading;





    FM_DelWin();
    //FreeSML();
    //


    Trace( "<EXIT>", "<EXIT>") ;

    FrmMain->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
    FrmOperation ->Close();
    FrmDevice    ->Close();
    FrmDeviceSet ->Close();
    FrmOption    ->Close();
    FrmMaster    ->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
   int i;
   i++    ;


}
//---------------------------------------------------------------------------
__fastcall TFrmMain::~TFrmMain()
{
}


void __fastcall TFrmMain::FormCreate(TObject *Sender)
{
    bool bSuccess = true ;



}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::lbNameDblClick(TObject *Sender)
{
    int iLevel;
    iLevel = FM_GetLevel();
    if (iLevel == lvMaster) FrmMaster -> Show();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    btExit -> Enabled = !SEQ._bRun ;
    String sSkipMsg = "" ;

    //Set Sequence State.
    if(!OM.MstOptn.bDebugMode){

        switch(SEQ._iSeqStat) {
            default        :
            break ;
            case ssInit    : lbStat -> Caption = "INIT"      ; lbStat -> Font -> Color = clBlue   ;
            break ;
            case ssError   : lbStat -> Caption = "ERROR"     ; lbStat -> Font -> Color = SEQ._bFlick ? clYellow : clRed ;
            break ;
            case ssRunning : lbStat -> Caption = "RUNNING"   ; lbStat -> Font -> Color = clLime   ;
            break ;
            case ssStop    : lbStat -> Caption = "STOP"      ; lbStat -> Font -> Color = clBlack  ;
            break ;
            case ssWorkEnd : lbStat -> Caption = "LOTEND"    ; lbStat -> Font -> Color = clGray   ;
            break ;
        }
    }
    else {
        switch(SEQ._iSeqStat) {
            default        :                                                                                                 break ;
            case ssInit    : lbStat -> Caption = "DEBUG INIT"    ; lbStat -> Font -> Color = clBlue   ;                      break ;
            case ssError   : lbStat -> Caption = "DEBUG ERROR"   ; lbStat -> Font -> Color = SEQ._bFlick ? clYellow : clRed ;break ;
            case ssRunning : lbStat -> Caption = "DEBUG RUNNING" ; lbStat -> Font -> Color = clLime   ;                      break ;
            case ssStop    : lbStat -> Caption = "DEBUG STOP"    ; lbStat -> Font -> Color = clBlack  ;                      break ;
            case ssWorkEnd : lbStat -> Caption = "DEBUG LOTEND"  ; lbStat -> Font -> Color = clGray   ;                      break ;
        }
    }
    // 상태 Panel 들 깜박 깜박.
    if(OM.CmnOptn.bIgnrDoor   ) sSkipMsg += "IGNR DOOR,"  ;
    if(OM.CmnOptn.bLoadingStop) sSkipMsg += "LOAD STOP,"  ;
    if(OM.MstOptn.bVacErrIgnr ) sSkipMsg += "IGNR VACM,"   ;
    if(OM.CmnOptn.bSkipOST    ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "OST,"   ;}
    if(OM.CmnOptn.bSkipVT     ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "VT,"    ;}
    if(OM.CmnOptn.bSkipVisn1  ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "VISN1," ;}
    if(OM.CmnOptn.bSkipVisn2  ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "VISN2," ;}
    if(OM.CmnOptn.bSkip1stTol ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "PCK1,"  ;}
    if(OM.CmnOptn.bSkip2ndTol ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "PCK2,"  ;}
    if(OM.CmnOptn.bSkip3thTol ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "PCK3,"  ;}
    if(OM.CmnOptn.bSkip4thTol ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "PCK4,"  ;}

    pnSkipDisp -> Caption = sSkipMsg ;


    pnSkipDisp -> Visible = sSkipMsg != "" ? SEQ._bFlick : false ;

    btOptionShow -> Enabled = FM_GetLevel() != lvOperator ;
    btUtilShow   -> Enabled = FM_GetLevel() != lvOperator ;


    lbDevice -> Caption = OM.GetCrntDev() ;


    lbLotNo -> Caption = LOT.GetLotNo() ;

    lbBLKCnt  -> Caption = String("BLK  CNT : ") + VTI.m_iBLKCntPock4  + "," + VTI.m_iBLKCntPock3  + "," + VTI.m_iBLKCntPock2  + "," + VTI.m_iBLKCntPock1  ;
    lbDFTCnt  -> Caption = String("DFT  CNT : ") + VTI.m_iDFTCntPock4  + "," + VTI.m_iDFTCntPock3  + "," + VTI.m_iDFTCntPock2  + "," + VTI.m_iDFTCntPock1  ;
    lbDFT2Cnt -> Caption = String("DFT2 CNT : ") + VTI.m_iDFT2CntPock4 + "," + VTI.m_iDFT2CntPock3 + "," + VTI.m_iDFT2CntPock2 + "," + VTI.m_iDFT2CntPock1 ;
    lbVHCnt   -> Caption = String("VH   CNT : ") + VTI.m_iVHCntPock4   + "," + VTI.m_iVHCntPock3   + "," + VTI.m_iVHCntPock2   + "," + VTI.m_iVHCntPock1   ;





    /*
        //연속 BLK 페일 카운트.
        int m_iBLKCntPock1 ;
        int m_iBLKCntPock2 ;
        int m_iBLKCntPock3 ;
        int m_iBLKCntPock4 ;

        //연속 Defect 페일 카운트.
        int m_iDFTCntPock1 ;
        int m_iDFTCntPock2 ;
        int m_iDFTCntPock3 ;
        int m_iDFTCntPock4 ;

        //연속 Defect 페일 카운트.
        int m_iDFT2CntPock1 ;
        int m_iDFT2CntPock2 ;
        int m_iDFT2CntPock3 ;
        int m_iDFT2CntPock4 ;


        //연속 VH페일 카운트/'
        int m_iVHCntPock1 ;
        int m_iVHCntPock2 ;
        int m_iVHCntPock3 ;
        int m_iVHCntPock4 ;
    */




    tmUpdate -> Enabled = true  ;
}
//---------------------------------------------------------------------------




void __fastcall TFrmMain::lbVerDblClick(TObject *Sender)
{
    FrmInform->Show();
}
//---------------------------------------------------------------------------












