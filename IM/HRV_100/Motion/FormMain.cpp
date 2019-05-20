//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMain.h"
#include "FormLoading.h"
#include "ManualMan.h"
#include "FormDevice.h"
#include "FormPosSet.h"
#include "FormOption.h"
#include "FormTestCon.h"
#include "FormMulti.h"
#include "FormCamPara.h"
#include "FormMaster.h"
//#include "FormSingle.h"
#include "FormImage.h"
#include "DataMan.h"
#include "FormGraph.h"
#include "FormRslt.h"
#include "FormRepeat.h"
#include "OptionMan.h"
#include "FrameCylOneBt.h"


#include "MainThread.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "Sequence.h"
#include "UserFile.h"
#include "Stage.h"

//#include "CamUnit.h"

#include "Image.h"
#include "Vision.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFrmMain *FrmMain;
//---------------------------------------------------------------------------
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
__fastcall TFrmMain::TFrmMain(TComponent* Owner)
        : TForm(Owner)
{
        //init
    Init();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::Init(void)
{
    FM_MakeWin() ;
    

    //FrmMain
    Top    = 0 ;
    Left   = 0 ;
    Width  = 1920 ;
    Height = 1080 ;



    //Version
    lbDllVer -> Caption = "Dll " + AnsiString(FM_GetVer()) ;
    lbDllAge -> Caption = FM_GetAge() ;
    lbVer -> Caption = String("Ver ") + FILEVERSION ;//OnGetVer("FileVersion") ;
    int Age = FileAge(Application -> ExeName);
    AnsiString Date = FILEDATE ; //FileDateToDateTime(Age).FormatString("''yyyy'_ 'm'_ 'd'_ 'AM/PM' 'h': 'n''");
    lbDate -> Caption = Date ;
    //lbName -> Caption = OM.EqpOptn.sModelName;
    //Last Device Load
    //OM.LoadLastInfo();

    MT_SetServoAll(true);

    m_sLastSaveData ="" ;

    StartThread();
}
void __fastcall TFrmMain::btExitClick(TObject *Sender)
{

    Trace( "<EXIT>", "<EXIT>") ;

    VSN.Live(false,0);

    while(!VSN.GetGrabEnd()){
        Sleep(1);
    }

    FrmMain->Close();
}
//---------------------------------------------------------------------------
__fastcall TFrmMain::~TFrmMain()
{
}

//---------------------------------------------------------------------------

void __fastcall TFrmMain::btOperatorClick(TObject *Sender)
{
    plPassWord->Visible = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btOperClick(TObject *Sender)
{
    FM_PswdShow((EN_LEVEL)((TBitBtn *)Sender)->Tag);
    plPassWord->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btResetClick(TObject *Sender)
{
    SEQ._bBtnReset = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btStartClick(TObject *Sender)
{
    SEQ._bBtnStart = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btStopClick(TObject *Sender)
{
    SEQ._bBtnStop = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btAllHomeClick(TObject *Sender)
{
    if (FM_MsgYesNo("Confirm" , "전체 홈을 잡으시겠습니까?") != mrYes)  return;
    MM.SetManCycle(mcAllHome);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Model1Click(TObject *Sender)
{
    FrmDevice -> Show()     ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::tmUpdateTimer(TObject *Sender)
{
    int iLevel;
    static int iLev = -1;

    iLevel = FM_GetLevel();
    switch (iLevel) {
        case lvOperator : btOperator -> Caption = "OPERATOR"; break ;
        case lvEngineer : btOperator -> Caption = "ENGINEER"; break ;
        case lvMaster   : btOperator -> Caption = " MASTER "; break ;
        default         : btOperator -> Caption = " ERROR  "; break ;
    }

    DM.ARAY[riSTG].UpdateAray() ;

    static int iPreActiveViewer = -1 ;
    int r,c  ;
    //if(SEQ._bRun){
        c = DM.ARAY[riSTG].FindFrstCol(csUnkwn);
        if(c != iPreActiveViewer){
            if(iPreActiveViewer!=-1){            
                FrmImage[iPreActiveViewer]->m_iActiveImg = iiSTG1Rslt+ iPreActiveViewer;
                FrmImage[iPreActiveViewer]->OnPaint();
            }

            if(c!=-1){
                VSN.SetPaintCallback(FrmImage[c]->OnPaint);
                FrmImage[c]->m_iActiveImg = iiSTGCrnt ;
            }

            iPreActiveViewer = c ;
        }
    //}       ''

    /*
    static bool bPreRun = false ;
    bool bRun = SEQ._iStep != CSequence::scIdle ;
    static CDelayTimer Delay ;
    if(Delay.OnDelay(bPreRun != bRun , 500)){
        if(bPreRun && !bRun) {
            Trace("RepeatCnt0",String(STG.m_iRepeatCnt).c_str());
            if(DM.ARAY[riSTG].GetCntStat(csUnkwn)==0){
                FrmMain -> AutoSave();
                Trace("RepeatCnt1",String(STG.m_iRepeatCnt).c_str());
                if(STG.m_iRepeatCnt>0){
                    STG.m_iRepeatCnt-- ;
                    SEQ._bBtnStart = true ;
                    Trace("RepeatCnt2",String(STG.m_iRepeatCnt).c_str());
                }
                else {
                    STG.m_bNoContactTest = false ;
                }
            }
        }
        bPreRun = bRun ;
    }
    */
    static bool bPreRun = false ;
    bool bRun = SEQ._iStep != CSequence::scIdle ;
    if(bPreRun && !bRun){
        if(DM.ARAY[riSTG].GetCntStat(csUnkwn)==0){
            FrmMain -> AutoSave();
            Trace("RepeatCnt1",String(STG.m_iRepeatCnt).c_str());
        }
    }
    bPreRun = bRun ;



    static CDelayTimer Delay ;
    if(Delay.OnDelay(!bRun , 500)){
        Trace("RepeatCnt0",String(STG.m_iRepeatCnt).c_str());
        if(STG.m_iRepeatCnt>0){
            SEQ._bBtnStart = true ;
            Trace("RepeatCnt2",String(STG.m_iRepeatCnt).c_str());
        }
        else {
            STG.m_eRepeatType = CStage::rtNone ;
        }
    }





    btJogN -> Enabled = !AT_Complete(aiSTG_ChckDnUp , ccFwd)&&!AT_Complete(aiSTG_ProbFwBw , ccFwd);
    btJogP -> Enabled = !AT_Complete(aiSTG_ChckDnUp , ccFwd)&&!AT_Complete(aiSTG_ProbFwBw , ccFwd);



    lbInspTime -> Caption = "INSP TIME : " + String(STG.m_iInspTime) + "ms";

    lbDevice -> Caption =  OM.GetCrntDev() ;

    lbDataFileName -> Caption = m_sLastSaveData ;



    ModelTestCondition1 -> Enabled = FM_GetLevel() == lvEngineer || FM_GetLevel() == lvMaster ;
    ModelPosition1      -> Enabled = FM_GetLevel() == lvEngineer || FM_GetLevel() == lvMaster ;
    CommonOption1       -> Enabled = FM_GetLevel() == lvEngineer || FM_GetLevel() == lvMaster ;
    MotionSettng1       -> Enabled = FM_GetLevel() == lvEngineer || FM_GetLevel() == lvMaster ;
    CameraSetting1      -> Enabled =                                FM_GetLevel() == lvMaster ;
    MasterSetting1      -> Enabled =                                FM_GetLevel() == lvMaster ;

    btExit -> Enabled = !SEQ._bRun ;

    pnShort -> Color   = IO_GetX(xSTG_CaseShort) ? clRed   : clBtnFace ;
    pnShort -> Caption = IO_GetX(xSTG_CaseShort) ? "Short" : ""        ;



}
//---------------------------------------------------------------------------



void __fastcall TFrmMain::ModelPosition1Click(TObject *Sender)
{
    FrmPosSet -> Show() ;        
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::ModelTestCondition1Click(TObject *Sender)
{
    FrmTestCon -> Show() ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::FormShow(TObject *Sender)
{
    FrmMulti -> Parent = pnStg ;
    FrmMulti -> Show() ;


    for(int i = 0 ; i < MAX_STAGE_CNT ; i++) {
        FrmImage[i] -> SetMaxParent(pnStg);
        FrmGraph[i] -> SetMaxParent(pnStg);                           

    }





}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::MotionSettng1Click(TObject *Sender)
{
    FM_SetWin(Application) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
    //Check Running Status.
    //if (SEQ._bRun) {
    //    FM_MsgOk("Error" , "During Auto Running!!");
    //    return;
    //}

    //if(FM_MsgYesNo("Confirm","Are you Sure?") != mrYes) return ;

    VSN.SetPaintCallback(NULL);

    tmUpdate -> Enabled = false ;
    EndThread();

    FrmLoading = new TFrmLoading(Application , false);
    ModalResult = FrmLoading->ShowModal();
    delete FrmLoading;


    FM_DelWin();
    //FreeSML();
    //
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::CameraSetting1Click(TObject *Sender)
{
    FrmCamPara -> Show() ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::MasterSetting1Click(TObject *Sender)
{
    FrmMaster -> Show() ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::GetPanelScreenShot(TPanel * _pPanel, String _sPath)
{
    Graphics::TBitmap *Bm = new Graphics::TBitmap();

    tagRECT Rect ;
    GetWindowRect(_pPanel -> Handle , &Rect);

    HDC dc;
    LOGPALETTE *lpPal;

    Bm->Width  = Rect.right - Rect.left;
    Bm->Height = Rect.bottom - Rect.top;
    dc = GetDC(0); 
    if(dc == 0) return; 

    BitBlt(Bm->Canvas->Handle,0,0,Bm->Width,Bm->Height,dc,Rect.left,Rect.top,SRCCOPY);

    ReleaseDC(0,dc);


    Bm -> SaveToFile(_sPath);

    m_sLastSaveData = _sPath ;

    delete Bm;
}; 



//---------------------------------------------------------------------------


void __fastcall TFrmMain::Multi1Click(TObject *Sender)
{
    FrmMulti  -> Parent = pnStg ;
    FrmMulti  -> Show() ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormCreate(TObject *Sender)
{
    //DM.ARAY[riSTG].SetMaxColRow(MAX_STAGE_CNT,1);

    DM.ARAY[riSTG].SetMaxColRow(5,1);
    DM.ARAY[riSTG].SetParent(pnStgAray); DM.ARAY[riSTG].SetCellSize(false); DM.ARAY[riSTG].SetName("STG"); DM.ARAY[riSTG].ClearDispItem();

    DM.ARAY[riSTG].SetDispItem(csUnkwn  ,"Unknown") ;
    DM.ARAY[riSTG].SetDispItem(csWork   ,"Work"   ) ;

    DM.ARAY[riSTG].SetStat(csUnkwn);

    //실린더 창.
    TPanel * pActr ;
    for(int i=0 ; i<MAX_ACTR ; i++) {
        pActr = ((TPanel*)FindComponent("pnAtcr" + AnsiString(i))) ;
        FraCylinder[i] = new TFraCylOneBt(pActr);

        switch(i){
            default :                                                                                            break;
            case 0  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , STG.CheckSafe ); break;  //aiLSP_GuidFwBw
            case 1  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , STG.CheckSafe ); break;  //aiLST_StckUpDn
            case 2  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , STG.CheckSafe ); break;  //aiLDR_IndxUpDn
        }
        FraCylinder[i] ->Show();
    }





    MM.SetManCycle(mcAllHome);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------





void __fastcall TFrmMain::Graph51Click(TObject *Sender)
{
    int iStgId = ((TMenuItem *)Sender) -> Tag ;
    if(FrmGraph[iStgId] -> Parent) {
        FrmGraph[iStgId] -> Parent = NULL ;
        FrmGraph[iStgId] -> BorderStyle = bsSizeable ;
        FrmGraph[iStgId] -> Align = alNone ;
    }
    else {
        FrmGraph[iStgId] -> Parent = FrmRslt[iStgId]-> pnGraph ;
        FrmGraph[iStgId] -> BorderStyle = bsNone ;
        FrmGraph[iStgId] -> Align = alClient ;
    }
    FrmGraph[iStgId] -> Show();

}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::CommonOption1Click(TObject *Sender)
{
    FrmOption -> Show() ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::btJogNClick(TObject *Sender)
{
    MT_GoIncRun(miSTG_XIndx , 15.0);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btJogPClick(TObject *Sender)
{
    MT_GoIncRun(miSTG_XIndx , -15.0);
}
//---------------------------------------------------------------------------




void __fastcall TFrmMain::SaveMeasureData1Click(TObject *Sender)
{
    AnsiString sFPath = OM.CmnOptn.sDataSavePath +"//" ;
    if(!UserFile.DirExists(sFPath)) UserFile.CreateDir(sFPath);

    TDateTime Time = Now() ;

    //기존에 있던것들 지우기.
    UserFile.ClearDirDate(sFPath , Now() - 30 ); //30일.

    //SaveDriveData.
    AnsiString sPath ;
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Drive1.csv" ; UserFile.SaveGridToCsv(FrmRslt[0]->sgDriveRslt,sPath);
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Drive2.csv" ; UserFile.SaveGridToCsv(FrmRslt[1]->sgDriveRslt,sPath);
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Drive3.csv" ; UserFile.SaveGridToCsv(FrmRslt[2]->sgDriveRslt,sPath);
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Drive4.csv" ; UserFile.SaveGridToCsv(FrmRslt[3]->sgDriveRslt,sPath);
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Drive5.csv" ; UserFile.SaveGridToCsv(FrmRslt[4]->sgDriveRslt,sPath);

    //SaveTiltData.
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Tilt1.csv" ; UserFile.SaveGridToCsv(FrmRslt[0]->sgTiltRslt,sPath);
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Tilt2.csv" ; UserFile.SaveGridToCsv(FrmRslt[1]->sgTiltRslt,sPath);
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Tilt3.csv" ; UserFile.SaveGridToCsv(FrmRslt[2]->sgTiltRslt,sPath);
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Tilt4.csv" ; UserFile.SaveGridToCsv(FrmRslt[3]->sgTiltRslt,sPath);
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Tilt5.csv" ; UserFile.SaveGridToCsv(FrmRslt[4]->sgTiltRslt,sPath);
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::SaveImage1Click(TObject *Sender)
{
    AnsiString sFPath = OM.CmnOptn.sDataSavePath+"//" ;
    if(!UserFile.DirExists(sFPath)) UserFile.CreateDir(sFPath);

    TDateTime Time = Now() ;

    //기존에 있던것들 지우기.
    UserFile.ClearDirDate(sFPath , Now() - 30 ); //30일.

    AnsiString sPath ;
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Img1.bmp" ; g_pImg[iiSTG1Rslt + 0]->SaveBmp(sPath.c_str());
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Img2.bmp" ; g_pImg[iiSTG1Rslt + 1]->SaveBmp(sPath.c_str());
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Img3.bmp" ; g_pImg[iiSTG1Rslt + 2]->SaveBmp(sPath.c_str());
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Img4.bmp" ; g_pImg[iiSTG1Rslt + 3]->SaveBmp(sPath.c_str());
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Img5.bmp" ; g_pImg[iiSTG1Rslt + 4]->SaveBmp(sPath.c_str());

}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::SavePrintScreen1Click(TObject *Sender)
{

    AnsiString sFPath = OM.CmnOptn.sDataSavePath+"//" ;
    if(!UserFile.DirExists(sFPath)) UserFile.CreateDir(sFPath);

    TDateTime Time = Now() ;

    //기존에 있던것들 지우기.
    UserFile.ClearDirDate(sFPath , Now() - 30 ); //30일.

    AnsiString sPath ;
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz") + ".bmp" ;

    GetPanelScreenShot(pnStg , sPath);
}
void __fastcall TFrmMain::SaveAllData1Click(TObject *Sender)
{
    AnsiString sFPath = OM.CmnOptn.sDataSavePath +"//" ;
    if(!UserFile.DirExists(sFPath)) UserFile.CreateDir(sFPath);

    TDateTime Time = Now() ;

    //기존에 있던것들 지우기.
    UserFile.ClearDirDate(sFPath , Now() - 30 ); //30일.

    AnsiString sPath ;
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz") + ".bmp" ; GetPanelScreenShot(pnStg , sPath);

    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Img1.bmp" ; g_pImg[iiSTG1Rslt + 0]->SaveBmp(sPath.c_str());
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Img2.bmp" ; g_pImg[iiSTG1Rslt + 1]->SaveBmp(sPath.c_str());
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Img3.bmp" ; g_pImg[iiSTG1Rslt + 2]->SaveBmp(sPath.c_str());
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Img4.bmp" ; g_pImg[iiSTG1Rslt + 3]->SaveBmp(sPath.c_str());
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Img5.bmp" ; g_pImg[iiSTG1Rslt + 4]->SaveBmp(sPath.c_str());

    //SaveDriveData.
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Drive1.csv" ; UserFile.SaveGridToCsv(FrmRslt[0]->sgDriveRslt,sPath);
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Drive2.csv" ; UserFile.SaveGridToCsv(FrmRslt[1]->sgDriveRslt,sPath);
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Drive3.csv" ; UserFile.SaveGridToCsv(FrmRslt[2]->sgDriveRslt,sPath);
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Drive4.csv" ; UserFile.SaveGridToCsv(FrmRslt[3]->sgDriveRslt,sPath);
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Drive5.csv" ; UserFile.SaveGridToCsv(FrmRslt[4]->sgDriveRslt,sPath);

    //SaveTiltData.
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Tilt1.csv" ; UserFile.SaveGridToCsv(FrmRslt[0]->sgTiltRslt,sPath);
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Tilt2.csv" ; UserFile.SaveGridToCsv(FrmRslt[1]->sgTiltRslt,sPath);
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Tilt3.csv" ; UserFile.SaveGridToCsv(FrmRslt[2]->sgTiltRslt,sPath);
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Tilt4.csv" ; UserFile.SaveGridToCsv(FrmRslt[3]->sgTiltRslt,sPath);
    sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Tilt5.csv" ; UserFile.SaveGridToCsv(FrmRslt[4]->sgTiltRslt,sPath);


}
//---------------------------------------------------------------------------


void TFrmMain::AutoSave()
{
    AnsiString sFPath = OM.CmnOptn.sDataSavePath +"//" ;
    if(!UserFile.DirExists(sFPath)) UserFile.CreateDir(sFPath);

    TDateTime Time = Now() ;

    //기존에 있던것들 지우기.
    UserFile.ClearDirDate(sFPath , Now() - 30 ); //30일.

    AnsiString sPath ;
    if(OM.CmnOptn.bAutoSaveRsltSc) {
        sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz") + ".bmp" ;
        GetPanelScreenShot(pnStg , sPath);
    }

    if(OM.CmnOptn.bAutoSaveRsltImg){
        sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Img1.bmp" ; g_pImg[iiSTG1Rslt + 0]->SaveBmp(sPath.c_str());
        sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Img2.bmp" ; g_pImg[iiSTG1Rslt + 1]->SaveBmp(sPath.c_str());
        sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Img3.bmp" ; g_pImg[iiSTG1Rslt + 2]->SaveBmp(sPath.c_str());
        sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Img4.bmp" ; g_pImg[iiSTG1Rslt + 3]->SaveBmp(sPath.c_str());
        sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Img5.bmp" ; g_pImg[iiSTG1Rslt + 4]->SaveBmp(sPath.c_str());
    }

    if(OM.CmnOptn.bAutoSaveRsltData){
        //SaveDriveData.
        sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_TotalData.csv"  ;
        SaveRsltData (sPath);

        /*
        sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Drive1.csv" ; UserFile.SaveGridToCsv(FrmRslt[0]->sgDriveRslt,sPath);
        sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Drive2.csv" ; UserFile.SaveGridToCsv(FrmRslt[1]->sgDriveRslt,sPath);
        sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Drive3.csv" ; UserFile.SaveGridToCsv(FrmRslt[2]->sgDriveRslt,sPath);
        sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Drive4.csv" ; UserFile.SaveGridToCsv(FrmRslt[3]->sgDriveRslt,sPath);
        sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Drive5.csv" ; UserFile.SaveGridToCsv(FrmRslt[4]->sgDriveRslt,sPath);

        //SaveTiltData.
        sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Tilt1.csv" ; UserFile.SaveGridToCsv(FrmRslt[0]->sgTiltRslt,sPath);
        sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Tilt2.csv" ; UserFile.SaveGridToCsv(FrmRslt[1]->sgTiltRslt,sPath);
        sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Tilt3.csv" ; UserFile.SaveGridToCsv(FrmRslt[2]->sgTiltRslt,sPath);
        sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Tilt4.csv" ; UserFile.SaveGridToCsv(FrmRslt[3]->sgTiltRslt,sPath);
        sPath = sFPath + Time.FormatString("yyyymmdd_")+ OM.GetCrntDev()+"_"+Time.FormatString("hhnnss_zzz")+"_Tilt5.csv" ; UserFile.SaveGridToCsv(FrmRslt[4]->sgTiltRslt,sPath);
        */
    }



}

void __fastcall TFrmMain::lbDataFileNameClick(TObject *Sender)
{
    double dTemp = GetTickCount();
    //AutoSave();


    dTemp = GetTickCount()-dTemp ;
}
//---------------------------------------------------------------------------


bool TFrmMain::SaveRsltData (AnsiString _sDestName)
{
    //Set Data.
    AnsiString sData = "" ;
    int hFile;

    hFile = FileOpen(_sDestName.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(_sDestName.c_str());
        if (hFile == -1) {
            ShowMessage("Could not make "+ _sDestName);
            return false;
        }

        //14
#ifdef COWELL  //코웰향 장비는 데이터를 선택한것만 남긴다. 20150723 남우석 차장 요청.
        sData = "" ;
        if(OM.DevOptn.bUseTiltInitR   )  sData+= "Init Tilt X Abs ," ;
        if(OM.DevOptn.bUseTiltInitT   )  sData+= "Init Tilt Y Abs ," ;
        if(OM.DevOptn.bUseTiltInit    )  sData+= "Init Tilt Abs   ," ;
        if(OM.DevOptn.bUseTiltDrive   )  sData+= "Drive Tilt      ," ;
        if(OM.DevOptn.bUseTiltDriveAbs)  sData+= "Total Tilt Abs  ," ;
        if(OM.DevOptn.bUseTiltFull    )  sData+= "Full Tilt       ," ;
        if(OM.DevOptn.bUseTiltMax     )  sData+= "Max Tilt        ," ;
        if(OM.DevOptn.bUseTiltLength  )  sData+= "Tilt Length     ," ;

        if(OM.DevOptn.bUsePreLoad     )  sData+= "StartingCurrent ," ; //"Preload         ," ;
        if(OM.DevOptn.bUsePeakStroke  )  sData+= "MaxStroke       ," ; //"PeakStroke      ," ;
        if(OM.DevOptn.bUseCustomStroke)  sData+= "RatedStroke     ," ;
        if(OM.DevOptn.bUseSensitivity )  sData+= "Sensitivity     ," ;
        if(OM.DevOptn.bUseLinearity   )  sData+= "Linearity       ," ;
        if(OM.DevOptn.bUseImpedance   )  sData+= "Resistance      ," ; //Impedance       ," ;
        if(OM.DevOptn.bUseHysteresis  )  sData+= "Hysteresis      ," ;
        if(OM.DevOptn.bUseCaseShort   )  sData+= "Case Short      ," ;
                                         sData+= "StageId         ," ;
                                         sData+= "Time            ," ;
                                         sData+= "Result          ," ;

        if(OM.DevOptn.bUseTiltInitR   )  sData+= "Init Tilt X Abs Spec," ;
        if(OM.DevOptn.bUseTiltInitT   )  sData+= "Init Tilt Y Abs Spec," ;
        if(OM.DevOptn.bUseTiltInit    )  sData+= "Init Tilt Abs   Spec," ;
        if(OM.DevOptn.bUseTiltDrive   )  sData+= "Drive Tilt      Spec," ;
        if(OM.DevOptn.bUseTiltDriveAbs)  sData+= "Total Tilt Abs  Spec," ;
        if(OM.DevOptn.bUseTiltFull    )  sData+= "Full Tilt       Spec," ;
        if(OM.DevOptn.bUseTiltMax     )  sData+= "Max Tilt        Spec," ;
        if(OM.DevOptn.bUseTiltLength  )  sData+= "Tilt Length     Spec," ;

        if(OM.DevOptn.bUsePreLoad     )  sData+= "StartingCurrent Spec," ;//Preload         Spec," ;
        if(OM.DevOptn.bUsePeakStroke  )  sData+= "MaxStroke       Spec," ; //"PeakStroke`      Spec," ;
        if(OM.DevOptn.bUseCustomStroke)  sData+= "RatedStroke     Spec," ;
        if(OM.DevOptn.bUseSensitivity )  sData+= "Sensitivity     Spec," ;
        if(OM.DevOptn.bUseLinearity   )  sData+= "Linearity       Spec," ;
        if(OM.DevOptn.bUseImpedance   )  sData+= "Resistance      Spec," ;//Impedance       Spec," ;
        if(OM.DevOptn.bUseHysteresis  )  sData+= "Hysteresis      Spec," ;
        if(OM.DevOptn.bUseCaseShort   )  sData+= "Case Short      Spec"  ;
                                         sData+= "\r\n" ;
#else
        sData = "" ;
        sData+= "Init Tilt X Abs ," ;
        sData+= "Init Tilt Y Abs ," ;
        sData+= "Init Tilt Abs   ," ;   
        sData+= "Drive Tilt      ," ;   
        sData+= "Total Tilt Abs  ," ;
        sData+= "Full Tilt       ," ;   
        sData+= "Max Tilt        ," ;
        sData+= "Tilt Length     ," ;

        sData+= "StartingCurrent ," ;
        sData+= "MaxStroke       ," ;
        sData+= "RatedStroke     ," ;
        sData+= "Sensitivity     ," ;   
        sData+= "Linearity       ," ;   
        sData+= "Resistance      ," ;
        sData+= "Hysteresis      ," ;   
        sData+= "Case Short      ," ;
        sData+= "StageId         ," ;
        sData+= "Time            ," ;
        sData+= "Result          ," ;

        sData+= "Init Tilt X Abs Spec," ;
        sData+= "Init Tilt Y Abs Spec," ;
        sData+= "Init Tilt Abs   Spec," ;
        sData+= "Drive Tilt      Spec," ;
        sData+= "Total Tilt Abs  Spec," ;
        sData+= "Full Tilt       Spec," ;
        sData+= "Max Tilt        Spec," ;
        sData+= "Tilt Length     Spec," ;

        sData+= "StartingCurrent Spec," ;
        sData+= "MaxStroke       Spec," ;
        sData+= "RatedStroke     Spec," ;
        sData+= "Sensitivity     Spec," ;
        sData+= "Linearity       Spec," ;
        sData+= "Resistance      Spec," ;
        sData+= "Hysteresis      Spec," ;
        sData+= "Case Short      Spec\r\n" ;
#endif



        /*
        for(int r=1; r<FrmRslt[0]->sgTiltRslt->RowCount;r++) {
            sData += FrmRslt[0]->sgTiltRslt->Cells[1][r] + "," ;
        }
        for(int r=1; r<FrmRslt[0]->sgDriveRslt->RowCount;r++) {
            sData += FrmRslt[0]->sgDriveRslt->Cells[1][r] + "," ;
        }
        sData += "StageId," ;
        sData += "Time\r\n" ;
        */
        FileSeek (hFile , 0             , SEEK_END      );
        FileWrite(hFile , sData.c_str() , sData.Length());
    }

    for(int i = 0 ; i < MAX_STAGE_CNT ; i++) {
        if(!OM.CmnOptn.bUseStage1 && i==0)continue ;
        if(!OM.CmnOptn.bUseStage2 && i==1)continue ;
        if(!OM.CmnOptn.bUseStage3 && i==2)continue ;
        if(!OM.CmnOptn.bUseStage4 && i==3)continue ;
        if(!OM.CmnOptn.bUseStage5 && i==4)continue ;

        sData="";
        String sRslt = "OK";
    //==============================================Tilt=============================
        double dTiltInitR    = g_TiltData[i].dTiltInitR    * OM.MstOptn.d2GainTiltInitR    + OM.MstOptn.d2OfstTiltInitR   ;
        double dTiltInitT    = g_TiltData[i].dTiltInitT    * OM.MstOptn.d2GainTiltInitT    + OM.MstOptn.d2OfstTiltInitT   ;
        double dTiltInit     = g_TiltData[i].dTiltInit     * OM.MstOptn.d2GainTiltInit     + OM.MstOptn.d2OfstTiltInit    ;
        double dTiltDrive    = g_TiltData[i].dTiltDrive    * OM.MstOptn.d2GainTiltDrive    + OM.MstOptn.d2OfstTiltDrive   ;
        double dTiltDriveAbs = g_TiltData[i].dTiltDriveAbs * OM.MstOptn.d2GainTiltDriveAbs + OM.MstOptn.d2OfstTiltDriveAbs;
        double dTiltFull     = g_TiltData[i].dTiltFull     * OM.MstOptn.d2GainTiltFull     + OM.MstOptn.d2OfstTiltFull    ;
        double dTiltMax      = g_TiltData[i].dTiltMax      * OM.MstOptn.d2GainTiltMax      + OM.MstOptn.d2OfstTiltMax     ;
        double dTiltLength   = g_TiltData[i].dTiltLength   * OM.MstOptn.d2GainTiltLength   + OM.MstOptn.d2OfstTiltLength  ;

        dTiltInitR    = int(dTiltInitR    * 10)/10.0 ;
        dTiltInitT    = int(dTiltInitT    * 10)/10.0 ;
        dTiltInit     = int(dTiltInit     * 10)/10.0 ;
        dTiltDrive    = int(dTiltDrive    * 10)/10.0 ;
        dTiltDriveAbs = int(dTiltDriveAbs * 10)/10.0 ;
        dTiltFull     = int(dTiltFull     * 10)/10.0 ;
        dTiltMax      = int(dTiltMax      * 10)/10.0 ;
        dTiltLength   = int(dTiltLength   * 10)/10.0 ;

        //20150616 드라이브 데이터에 잡파일별로 게인 오프셑 넣기... 요청사항.
        //20150825 코헬향 숨김 게인 오프셑 추가 요청.
        double dPreLoad      = g_DriveData[i].dPreLoad      * OM.MstOptn.d2GainPreLoad      + OM.MstOptn.d2OfstPreLoad      ;
        double dPeakStroke   = g_DriveData[i].dPeakStroke   * OM.MstOptn.d2GainPeakStroke   + OM.MstOptn.d2OfstPeakStroke   ;
        double dCustomStroke = g_DriveData[i].dCustomStroke * OM.MstOptn.d2GainCustomStroke + OM.MstOptn.d2OfstCustomStroke ;
        double dSensitivity  = g_DriveData[i].dSensitivity  * OM.MstOptn.d2GainSensitivity  + OM.MstOptn.d2OfstSensitivity  ;
        double dLinearity    = g_DriveData[i].dLinearity    * OM.MstOptn.d2GainLinearity    + OM.MstOptn.d2OfstLinearity    ;
        double dImpedance    = g_DriveData[i].dImpedance    * OM.MstOptn.d2GainImpedance    + OM.MstOptn.d2OfstImpedance    ;
        double dHysteresis   = g_DriveData[i].dHysteresis   * OM.MstOptn.d2GainHysteresis   + OM.MstOptn.d2OfstHysteresis   ;
        bool   bCaseShortOk  =!g_DriveData[i].bCaseShort ;

        dPreLoad      = g_DriveData[i].dPreLoad      * OM.MstDevOptn.dGainPreLoad      + OM.MstDevOptn.dOfstPreLoad      ;
        dPeakStroke   = g_DriveData[i].dPeakStroke   * OM.MstDevOptn.dGainPeakStroke   + OM.MstDevOptn.dOfstPeakStroke   ;
        dCustomStroke = g_DriveData[i].dCustomStroke * OM.MstDevOptn.dGainCustomStroke + OM.MstDevOptn.dOfstCustomStroke ;
        dSensitivity  = g_DriveData[i].dSensitivity  * OM.MstDevOptn.dGainSensitivity  + OM.MstDevOptn.dOfstSensitivity  ;
        dLinearity    = g_DriveData[i].dLinearity    * OM.MstDevOptn.dGainLinearity    + OM.MstDevOptn.dOfstLinearity    ;
        dImpedance    = g_DriveData[i].dImpedance    * OM.MstDevOptn.dGainImpedance    + OM.MstDevOptn.dOfstImpedance    ;
        dHysteresis   = g_DriveData[i].dHysteresis   * OM.MstDevOptn.dGainHysteresis   + OM.MstDevOptn.dOfstHysteresis   ;

        dPreLoad      = int(dPreLoad      * 10)/10.0 ;
        dPeakStroke   = int(dPeakStroke   * 10)/10.0 ;
        dCustomStroke = int(dCustomStroke * 10)/10.0 ;
        dSensitivity  = int(dSensitivity  * 10)/10.0 ;
        dLinearity    = int(dLinearity    * 10)/10.0 ;
        dImpedance    = int(dImpedance    * 10)/10.0 ;
        dHysteresis   = int(dHysteresis   * 10)/10.0 ;






        bool bTemp ;
        if(OM.DevOptn.bUseTiltInitR    ){bTemp = (OM.DevOptn.dMinTiltInitR    <= dTiltInitR    && dTiltInitR    <=OM.DevOptn.dMaxTiltInitR    ) ; if(!bTemp) sRslt = "NG" ;}
        if(OM.DevOptn.bUseTiltInitT    ){bTemp = (OM.DevOptn.dMinTiltInitT    <= dTiltInitT    && dTiltInitT    <=OM.DevOptn.dMaxTiltInitT    ) ; if(!bTemp) sRslt = "NG" ;}
        if(OM.DevOptn.bUseTiltInit     ){bTemp = (OM.DevOptn.dMinTiltInit     <= dTiltInit     && dTiltInit     <=OM.DevOptn.dMaxTiltInit     ) ; if(!bTemp) sRslt = "NG" ;}
        if(OM.DevOptn.bUseTiltDrive    ){bTemp = (OM.DevOptn.dMinTiltDrive    <= dTiltDrive    && dTiltDrive    <=OM.DevOptn.dMaxTiltDrive    ) ; if(!bTemp) sRslt = "NG" ;}
        if(OM.DevOptn.bUseTiltDriveAbs ){bTemp = (OM.DevOptn.dMinTiltDriveAbs <= dTiltDriveAbs && dTiltDriveAbs <=OM.DevOptn.dMaxTiltDriveAbs ) ; if(!bTemp) sRslt = "NG" ;}
        if(OM.DevOptn.bUseTiltFull     ){bTemp = (OM.DevOptn.dMinTiltFull     <= dTiltFull     && dTiltFull     <=OM.DevOptn.dMaxTiltFull     ) ; if(!bTemp) sRslt = "NG" ;}
        if(OM.DevOptn.bUseTiltMax      ){bTemp = (OM.DevOptn.dMinTiltMax      <= dTiltMax      && dTiltMax      <=OM.DevOptn.dMaxTiltMax      ) ; if(!bTemp) sRslt = "NG" ;}
        if(OM.DevOptn.bUseTiltLength   ){bTemp = (OM.DevOptn.dMinTiltLength   <= dTiltLength   && dTiltLength   <=OM.DevOptn.dMaxTiltLength   ) ; if(!bTemp) sRslt = "NG" ;}

        if(OM.DevOptn.bUsePreLoad     ){ bTemp = (OM.DevOptn.dMinPreLoad      <= dPreLoad      && dPreLoad      <=OM.DevOptn.dMaxPreLoad      ) ; if(!bTemp) sRslt = "NG" ;}
        if(OM.DevOptn.bUsePeakStroke  ){ bTemp = (OM.DevOptn.dMinPeakStroke   <= dPeakStroke   && dPeakStroke   <=OM.DevOptn.dMaxPeakStroke   ) ; if(!bTemp) sRslt = "NG" ;}
        if(OM.DevOptn.bUseCustomStroke){ bTemp = (OM.DevOptn.dMinCustomStroke <= dCustomStroke && dCustomStroke <=OM.DevOptn.dMaxCustomStroke ) ; if(!bTemp) sRslt = "NG" ;}
        if(OM.DevOptn.bUseSensitivity ){ bTemp = (OM.DevOptn.dMinSensitivity  <= dSensitivity  && dSensitivity  <=OM.DevOptn.dMaxSensitivity  ) ; if(!bTemp) sRslt = "NG" ;}
        if(OM.DevOptn.bUseLinearity   ){ bTemp = (OM.DevOptn.dMinLinearity    <= dLinearity    && dLinearity    <=OM.DevOptn.dMaxLinearity    ) ; if(!bTemp) sRslt = "NG" ;}
        if(OM.DevOptn.bUseImpedance   ){ bTemp = (OM.DevOptn.dMinImpedance    <= dImpedance    && dImpedance    <=OM.DevOptn.dMaxImpedance    ) ; if(!bTemp) sRslt = "NG" ;}
        if(OM.DevOptn.bUseHysteresis  ){ bTemp = (OM.DevOptn.dMinHysteresis   <= dHysteresis   && dHysteresis   <=OM.DevOptn.dMaxHysteresis   ) ; if(!bTemp) sRslt = "NG" ;}
        if(OM.DevOptn.bUseCaseShort   ){ bTemp = bCaseShortOk;                                                                                    if(!bTemp) sRslt = "NG" ;}


        //결과값.
        String sTemp ;
#ifdef COWELL
        if(OM.DevOptn.bUseTiltInitR   ){sTemp.sprintf("%.1f" ,dTiltInitR    ); sTemp = sTemp != "999.0" ? sTemp : String("FAIL"); sData += sTemp ; sData += "," ;} 
        if(OM.DevOptn.bUseTiltInitT   ){sTemp.sprintf("%.1f" ,dTiltInitT    ); sTemp = sTemp != "999.0" ? sTemp : String("FAIL"); sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseTiltInit    ){sTemp.sprintf("%.1f" ,dTiltInit     ); sTemp = sTemp != "999.0" ? sTemp : String("FAIL"); sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseTiltDrive   ){sTemp.sprintf("%.1f" ,dTiltDrive    ); sTemp = sTemp != "999.0" ? sTemp : String("FAIL"); sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseTiltDriveAbs){sTemp.sprintf("%.1f" ,dTiltDriveAbs ); sTemp = sTemp != "999.0" ? sTemp : String("FAIL"); sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseTiltFull    ){sTemp.sprintf("%.1f" ,dTiltFull     ); sTemp = sTemp != "999.0" ? sTemp : String("FAIL"); sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseTiltMax     ){sTemp.sprintf("%.1f" ,dTiltMax      ); sTemp = sTemp != "999.0" ? sTemp : String("FAIL"); sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseTiltLength  ){sTemp.sprintf("%.1f" ,dTiltLength   ); sTemp = sTemp != "999.0" ? sTemp : String("FAIL"); sData += sTemp ; sData += "," ;}

        if(OM.DevOptn.bUsePreLoad     ){sTemp.sprintf("%.1f" ,dPreLoad      ); sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUsePeakStroke  ){sTemp.sprintf("%.1f" ,dPeakStroke   ); sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseCustomStroke){sTemp.sprintf("%.1f" ,dCustomStroke ); sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseSensitivity ){sTemp.sprintf("%.1f" ,dSensitivity  ); sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseLinearity   ){sTemp.sprintf("%.1f" ,dLinearity    ); sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseImpedance   ){sTemp.sprintf("%.1f" ,dImpedance    ); sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseHysteresis  ){sTemp.sprintf("%.1f" ,dHysteresis   ); sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseCaseShort   ){sTemp= bCaseShortOk ? "OK" : "NG"    ; sData += sTemp ; sData += "," ;}
        sTemp= "";                                                                                                                                                 
#else                                                                                                                                                              
        sTemp.sprintf("%.1f" ,dTiltInitR    ); sTemp = sTemp != "999.0" ? sTemp : String("FAIL"); sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f" ,dTiltInitT    ); sTemp = sTemp != "999.0" ? sTemp : String("FAIL"); sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f" ,dTiltInit     ); sTemp = sTemp != "999.0" ? sTemp : String("FAIL"); sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f" ,dTiltDrive    ); sTemp = sTemp != "999.0" ? sTemp : String("FAIL"); sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f" ,dTiltDriveAbs ); sTemp = sTemp != "999.0" ? sTemp : String("FAIL"); sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f" ,dTiltFull     ); sTemp = sTemp != "999.0" ? sTemp : String("FAIL"); sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f" ,dTiltMax      ); sTemp = sTemp != "999.0" ? sTemp : String("FAIL"); sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f" ,dTiltLength   ); sTemp = sTemp != "999.0" ? sTemp : String("FAIL"); sData += sTemp ; sData += "," ;

        sTemp.sprintf("%.1f" ,dPreLoad      ); sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f" ,dPeakStroke   ); sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f" ,dCustomStroke ); sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f" ,dSensitivity  ); sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f" ,dLinearity    ); sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f" ,dImpedance    ); sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f" ,dHysteresis   ); sData += sTemp ; sData += "," ;
        sTemp= bCaseShortOk ? "OK" : "NG"    ; sData += sTemp ; sData += "," ;


#endif

        sData += AnsiString (i+1) +",";
        sData += Now().FormatString("hh:nn:ss")+"," ;
        sData += sRslt ; sData += "," ;
#ifdef COWELL
        if(OM.DevOptn.bUseTiltInitR   ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltInitR    ,OM.DevOptn.dMaxTiltInitR   );sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseTiltInitT   ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltInitT    ,OM.DevOptn.dMaxTiltInitT   );sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseTiltInit    ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltInit     ,OM.DevOptn.dMaxTiltInit    );sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseTiltDrive   ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltDrive    ,OM.DevOptn.dMaxTiltDrive   );sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseTiltDriveAbs){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltDriveAbs ,OM.DevOptn.dMaxTiltDriveAbs);sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseTiltFull    ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltFull     ,OM.DevOptn.dMaxTiltFull    );sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseTiltMax     ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltMax      ,OM.DevOptn.dMaxTiltMax     );sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseTiltLength  ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltLength   ,OM.DevOptn.dMaxTiltLength  );sData += sTemp ; sData += "," ;}

        if(OM.DevOptn.bUsePreLoad     ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinPreLoad      ,OM.DevOptn.dMaxPreLoad     );sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUsePeakStroke  ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinPeakStroke   ,OM.DevOptn.dMaxPeakStroke  );sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseCustomStroke){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinCustomStroke ,OM.DevOptn.dMaxCustomStroke);sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseSensitivity ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinSensitivity  ,OM.DevOptn.dMaxSensitivity );sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseLinearity   ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinLinearity    ,OM.DevOptn.dMaxLinearity   );sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseImpedance   ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinImpedance    ,OM.DevOptn.dMaxImpedance   );sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseHysteresis  ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinHysteresis   ,OM.DevOptn.dMaxHysteresis  );sData += sTemp ; sData += "," ;}
        if(OM.DevOptn.bUseCaseShort   ){                                                                                    sData += "OK"  ;}sData += "\r\n" ;
#else
        sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltInitR    ,OM.DevOptn.dMaxTiltInitR   );sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltInitT    ,OM.DevOptn.dMaxTiltInitT   );sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltInit     ,OM.DevOptn.dMaxTiltInit    );sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltDrive    ,OM.DevOptn.dMaxTiltDrive   );sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltDriveAbs ,OM.DevOptn.dMaxTiltDriveAbs);sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltFull     ,OM.DevOptn.dMaxTiltFull    );sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltMax      ,OM.DevOptn.dMaxTiltMax     );sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltLength   ,OM.DevOptn.dMaxTiltLength  );sData += sTemp ; sData += "," ;

        sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinPreLoad      ,OM.DevOptn.dMaxPreLoad     );sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinPeakStroke   ,OM.DevOptn.dMaxPeakStroke  );sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinCustomStroke ,OM.DevOptn.dMaxCustomStroke);sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinSensitivity  ,OM.DevOptn.dMaxSensitivity );sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinLinearity    ,OM.DevOptn.dMaxLinearity   );sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinImpedance    ,OM.DevOptn.dMaxImpedance   );sData += sTemp ; sData += "," ;
        sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinHysteresis   ,OM.DevOptn.dMaxHysteresis  );sData += sTemp ; sData += "," ;
                                                                                            sData += "OK"  ; sData += "\r\n" ;
#endif






        FileSeek (hFile , 0             , SEEK_END      );
        FileWrite(hFile , sData.c_str() , sData.Length());

    }

    //Close File.
    FileClose(hFile);

    return true;


}









void __fastcall TFrmMain::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(Shift.Contains(ssCtrl)&&Shift.Contains(ssShift)) {
        AnsiString sTemp;
        sTemp.sprintf("%c", Key);

        //시뮬레이션용으로 만듬.... 너무 디버깅이 어려워 만듬...
        if(sTemp=="1"){
            ShowMessage(sTemp);
        }
        if(sTemp=="2"){
            ShowMessage(sTemp);
        }
        if(sTemp=="3"){
            ShowMessage(sTemp);
        }
        if(sTemp=="4"){
            ShowMessage(sTemp);
        }
        if(sTemp=="5"){
            ShowMessage(sTemp);
        }


    }
}
//---------------------------------------------------------------------------




void __fastcall TFrmMain::lbDllAgeClick(TObject *Sender)
{

    AnsiString sFileName = OM.CmnOptn.sDataSavePath + "\\" + Now().FormatString("YYYY_MM_DD")+"_TotalData.csv"  ;
    SaveRsltData (sFileName);
}
//---------------------------------------------------------------------------






void __fastcall TFrmMain::lbNameClick(TObject *Sender)
{
    if(FM_GetLevel() == lvMaster) {
        FrmRepeat -> Show();
    }
}
//---------------------------------------------------------------------------

