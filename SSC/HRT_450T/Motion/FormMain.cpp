//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <string.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Form Reference
//---------------------------------------------------------------------------
#include "FormMain.h"
#include "FormMaster.h"
#include "FormProbe.h"
#include "FormSPC.h"
#include "FormInform.h"
//---------------------------------------------------------------------------
//Unit Reference
//---------------------------------------------------------------------------
#include "MainThread.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "DataMan.h"
#include "Sequence.h"
#include "LotUnit.h"
#include "LanguageUnit.h"
//---------------------------------------------------------------------------
//Temiral Reference
//---------------------------------------------------------------------------
#include "ProbeComUnit.h"
#include "TempConUnit.h"
//---------------------------------------------------------------------------
//Option Reference
//---------------------------------------------------------------------------
#include "OptionMan.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Sequence Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff1.h"
#include "PreBuff2.h"
#include "WorkZone.h"
#include "PostBuff.h"
#include "SortingTool.h"
#include "UnLoader.h"
//---------------------------------------------------------------------------
#include "VisnDefine.h"
#include "SAInterface.h"

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
    FM_MakeWin(pnBase) ;
    //init
    Init();

    //Visible = false ;  //ssss

}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::Init(void)
{
    m_iPageIndex = 0 ;

    //FrmMain
    Top    = 0 ;
    Left   = 0 ;
    Width  = 1280 ;
    Height = 1024 ;


/*
    const int disigne_width  = 1440;    // 개발당시 해상도
    const int disigne_height = 1024;
    if (Screen->Width <= 1280)            // 특정 해상도 이하이면.
    {
        Scaled = true;
        Height = Height * Screen->Height / disigne_height;
        Width  = Width  * Screen->Width  / disigne_width;
        ScaleBy(Screen->Width, disigne_width);

        // 강제로 맞춰보기.
//        int Screen_Width  = 1280;
//        int Screen_Height = 1024;
//        Width  = disigne_width;
//        Width  = Width  * Screen_Width  / disigne_width;
//        Height = Height * Screen_Height / disigne_height;
//        ScaleBy(Screen_Width, disigne_width);
    }
*/

    //Run State
    //pnIdleRun     ->  Left = 0 ;  pnIdleRun     -> Top = 0 ;
    //pnManualRun   ->  Left = 0 ;  pnManualRun   -> Top = 51;
    //pnReworkMode  ->  Left = 429; pnReworkMode  -> Top = 51;
    //pnLoadingStop ->  Left = 854; pnLoadingStop -> Top = 0 ;
    //pnIgnreDoor   ->  Left = 854; pnIgnreDoor   -> Top = 51;

    //Version
    lbDllVer -> Caption = "Dll " + FM_GetVer() ;
    lbDllAge -> Caption = FM_GetAge() ;
    lbVer -> Caption = "Ver " + OnGetVer("FileVersion") ;
    int Age = FileAge(Application -> ExeName);
    AnsiString Date = FileDateToDateTime(Age).FormatString("''yyyy'. 'm'. 'd'. 'AM/PM' 'h': 'nn''");
    lbDate -> Caption = Date ;

    //Last Device Load
    OM.LoadLastInfo();
//    MT_SetServoAll(true);
    IO_SetY(yLDR_ZBreak,false );
    IO_SetY(yULD_ZBreak,false );
    ::Sleep(100);

    if(OM.EqpOptn.bExistLoader  ) MT_SetServo((int)miLDR_YCmp , true) ;
    if(OM.EqpOptn.bExistLoader  ) MT_SetServo((int)miLDR_ZCmp , true) ;
    if(OM.EqpOptn.bExistUnLoader) MT_SetServo((int)miULD_YCmp , true) ;
    if(OM.EqpOptn.bExistUnLoader) MT_SetServo((int)miULD_ZCmp , true) ;
    MT_SetServo((int)miPR1_XCmp , true) ;
    MT_SetServo((int)miPR2_XCmp , true) ;
    MT_SetServo((int)miWRK_XCmp , true) ;
    MT_SetServo((int)miPSB_XCmp , true) ;
    MT_SetServo((int)miWRK_ZTop , true) ;
    MT_SetServo((int)miWRK_ZBtm , true) ;
    MT_SetServo((int)miSTL_YTop , true) ;
    MT_SetServo((int)miREJ_XBtm , true) ;

//    tmMainSet->Enabled = true ;

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
    static bool bFirst = true ;
    if(bFirst) {
        FrmOperation -> InitArrayPanel();
        FrmSPC -> InitArrayPanel();
        FrmOption -> InitArrayPanel();
        DM.LoadMap();
    }




    FrmOperation -> Parent = pnBase ; FrmOperation -> Left = 0 ; FrmOperation -> Top = 0 ; FrmOperation -> Width = 1280 ; FrmOperation -> Height = 863 ;
    FrmDevice    -> Parent = pnBase ; FrmDevice    -> Left = 0 ; FrmDevice    -> Top = 0 ; FrmDevice    -> Width = 1280 ; FrmDevice    -> Height = 863 ;
    FrmDeviceSet -> Parent = pnBase ; FrmDeviceSet -> Left = 0 ; FrmDeviceSet -> Top = 0 ; FrmDeviceSet -> Width = 1280 ; FrmDeviceSet -> Height = 863 ;
    FrmOption    -> Parent = pnBase ; FrmOption    -> Left = 0 ; FrmOption    -> Top = 0 ; FrmOption    -> Width = 1280 ; FrmOption    -> Height = 863 ;
    FrmSPC       -> Parent = pnBase ; FrmSPC       -> Left = 0 ; FrmSPC       -> Top = 0 ; FrmSPC       -> Width = 1280 ; FrmSPC       -> Height = 863 ;
    FrmMaster    -> Parent = pnBase ; FrmMaster    -> Left = 0 ; FrmMaster    -> Top = 0 ; FrmMaster    -> Width = 1280 ; FrmMaster    -> Height = 863 ;

    ShowPage (0);
    FrmProbe->Show();

    LAG.LanguageLoad(FrmOperation, FM_GetLan());
    LAG.LanguageLoad(FrmDevice   , FM_GetLan());
    LAG.LanguageLoad(FrmDeviceSet, FM_GetLan());
    LAG.LanguageLoad(FrmOption   , FM_GetLan());
    LAG.LanguageLoad(FrmSPC      , FM_GetLan());


    tmUpdate     -> Enabled = true ;
    tmTempuretue -> Enabled = true ;

    /*
    if(OM.DevOptn.bLtHeaterNum2) IO_SetY(yETC_Heater1_2,true );
    else                         IO_SetY(yETC_Heater1_2,false);
    if(OM.DevOptn.bLtHeaterNum3) IO_SetY(yETC_Heater1_3,true );
    else                         IO_SetY(yETC_Heater1_3,false);
    if(OM.DevOptn.bRtHeaterNum2) IO_SetY(yETC_Heater2_2,true );
    else                         IO_SetY(yETC_Heater2_2,false);
    if(OM.DevOptn.bRtHeaterNum3) IO_SetY(yETC_Heater2_3,true );
    else                         IO_SetY(yETC_Heater2_3,false);
    //TODO :: 광폭에서 협폭 조절 시 히터 화면 꺼지게 하는게 없어짐.. JS
    */
    //    SEQ.Reset();

}
//---------------------------------------------------------------------------
void __fastcall  TFrmMain::ShowPage (int index)
{
    switch(index)
    {
        case 0: FrmOperation -> Show() ;
                if(OM.MstOptn.bUsedVision){
                    if(tmUpdate -> Enabled) SV_SetImgForm(FrmOperation -> pnOperVisn -> Handle , false) ;
                }
                break ;
        case 1: FrmDevice    -> Show() ;
                if(OM.MstOptn.bUsedVision){
                    SV_SetImgForm(FrmDeviceSet -> pnSetVisn -> Handle , true ) ;
                }
                break ;
        case 2: FrmOption    -> Show() ; break ;
        case 3: FrmSPC       -> Show() ; break ;
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

    //SV_DelVisn();
    //if(!FreeSVA())ShowMessage(ExtractFilePath(Application->ExeName) + "\pSVApp Free Failed!");

    //Language Save.
    LAG.LanguageSave(FrmOperation, FM_GetLan());
    LAG.LanguageSave(FrmDevice   , FM_GetLan());
    LAG.LanguageSave(FrmDeviceSet, FM_GetLan());
    LAG.LanguageSave(FrmOption   , FM_GetLan());
    LAG.LanguageSave(FrmSPC      , FM_GetLan());

    //소멸자 삽입부.
    LDR.Close();
    PR1.Close();
    PR2.Close();
    WRK.Close();
    PSB.Close();
    ULD.Close();

    LT.Close();

    DM.Close();
    OM.Close();

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

    tmUpdate -> Enabled = false ;
    tmTempuretue -> Enabled = false ;

    Trace("<EXIT>","EXIT");    

    EndThread();

}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btUtilShowClick(TObject *Sender)
{
    ClosePage(m_iPageIndex);

    FM_SetWin(pnBase) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormCreate(TObject *Sender)
{
    bool bSuccess = true ;
//    FM_MakeWin(pnBase) ;

    StartThread();



    //Ip address check
//    IdIIcmp->Host = OM.MstOptn.sPstEquipIp;
//    IdIIcmp->Ping();



}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::lbNameDblClick(TObject *Sender)
{
    int iLevel;
    iLevel = FM_GetLevel();
    if (iLevel >= lvMaster) FrmMaster -> Show();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;


    static bool bFirst = true ;
    if(bFirst) {
        if(OM.MstOptn.bUsedVision){

            Application -> ProcessMessages();
            if(!LoadSVA("pSVApp.dll")) ShowMessage(ExtractFilePath(Application->ExeName) + "\pSVApp Loading Failed!");

            SV_InitVisn(FrmOperation ->   pnOperVisn -> Handle , FrmDeviceSet -> pnSetPara-> Handle , OM.m_sCrntDev.c_str());
        }

        bFirst = false ;
    }


    btExit -> Enabled = !SEQ._bRun ;

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
    pnLoadingStop -> Visible = OM.CmnOptn.bLoadingStop ? SEQ._bFlick : false ;
    pnIgnreDoor   -> Visible = OM.CmnOptn.bIgnrDoor    ? SEQ._bFlick : false ;
    pnDryRun      -> Visible = OM.CmnOptn.bDryRun      ? SEQ._bFlick : false ;
    pnInspSkip    -> Visible = OM.CmnOptn.bIgnrInsp    ? SEQ._bFlick : false ;
    pnIgnrTrim    -> Visible = OM.CmnOptn.bIgnrTrim    ? SEQ._bFlick : false ;
    pnSSI         -> Visible = OM.DevInfo.bSSI         ? SEQ._bFlick : false ;
    pnNoTrim      -> Visible = false ;

    btOptionShow -> Enabled = FM_GetLevel() != lvOperator ;
    btUtilShow   -> Enabled = FM_GetLevel() != lvOperator ;


    lbDevice -> Caption = OM.m_sCrntDev ;

    tmUpdate -> Enabled = true  ;



}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::IdIIcmpReply(TComponent *ASender,
      const TReplyStatus &AReplyStatus)
{
//    if(AReplyStatus.BytesReceived) {
//        if(!EC.NetDrvConnect(OM.MstOptn.sPstEquipDrv, OM.MstOptn.sPstEquipIp, OM.MstOptn.sPstEquipPath)) ShowMessage("Failed to create a network drive");
//    }
//    else ShowMessage("Failed to create a network drive");
}
//---------------------------------------------------------------------------
// 쓰레드에 의해 Data가 수신되면 SendMessage에 의해 호출된다. 즉 수신버퍼에
// TMessage의 WParam의 길이만큼 Data가 수신된 후 호출된다
//void __fastcall TFrmMain::HeatProbe1Ch(TMessage &Message)
//{
////    Sleep(1);
//    int iLen;
//    const int iBuffSize = 1024 ;
//    BYTE RcvBuff[iBuffSize];
//
//    static String sRcvMsg  = "" ;
//    static String sRcvTemp = "" ;
//    static AnsiString sRcvTestMsg = "" ;
//    static CDelayTimer tmDelay ;
//
//    memset(&RcvBuff, 0 , sizeof(RcvBuff));
//
//    iLen = RS232C_Pb1ch->ReadData(Message.WParam, RcvBuff);
////    if(OM.MstOptn.bDebugMode)Trace("RS232C_Pb1ch",RcvBuff);
//
//    //if(tmDelay.OnDelay(true , 400)) { sRcvMsg = "" ; tmDelay.Clear();}
//
//    sRcvMsg    += (char*)RcvBuff ;
//    sRcvTestMsg = (char*)RcvBuff ;
//    Trace("RS232C_Pb1ch_Msg",sRcvTestMsg.c_str());
//
//
//    bool bFinished = false ;
//    for(int i = 0 ; i < iBuffSize ; i++) {
//        if(RcvBuff[i] == '[') {
//            bFinished = true ;
//            break ;
//        }
//    }
//
//
//    if(sRcvMsg.Pos("OK\r")) sRcvMsg.Delete(sRcvMsg.Pos("OK\r"),3);
//    if(!bFinished/*!sRcvMsg.Pos("[")*/) return ;
//    //if(!OM.DevOptn.bSideLEDControl && bFinished/*!sRcvMsg.Pos("[")*/) return ;
//    if(OM.MstOptn.bDebugMode)Trace("RS232C_Pb1ch_Msg",sRcvMsg.c_str());
//
//    String sRcvQue[4];
//    String sTemp ;
//    int  iBuffCnt   = sRcvMsg.Pos("RDROP") + 4;
//
//    sRcvTemp = sRcvMsg ;
//
//    //bCheckRlst = sRcvMsg.Pos("RDROP");
//
//    //사이드 컨텍컨트롤러일 경우... 롱테스트 드랍 결과값.
//    g_bPbRelse1Rslt = true;
//
//    if(g_bPbRelse1Rslt){
//        //임시적인 드롭 확인. 나중에 3칩 다 적용 되게 바꾸거나 주석처리.
//        for(int i = 0 ; i <4 ; i++){
//            sRcvQue[i] = sRcvTemp.sprintf("%02x", RcvBuff[iBuffCnt+i]);
//        }
//        for(int j = 0 ; j < 4 ; j++){
//            sRcvQue[j].sprintf("%s", convertHex2Bin(sRcvQue[j].c_str()));
//        }
//        sTemp = sRcvQue[1] + sRcvQue[0];
//        g_sPb1Bin[0] = sTemp;
//        //Label1 -> Caption = sTemp;
//        sTemp = sRcvQue[3] + sRcvQue[2];
//        g_sPb1Bin[1] = sTemp;
//        //Label2 -> Caption = sTemp;
//        //sRcvTemp.Delete(sRcvMsg.Pos("RDROP"), 10);
//    }
//    /*
//
//    String sRcvQue[4];
//    String sTemp ;
//    int  iBuffCnt   = sRcvMsg.Pos("RDROP") + 4;
//
//    //bCheckRlst = sRcvMsg.Pos("RDROP");
//
//    //사이드 컨텍컨트롤러일 경우... 롱테스트 드랍 결과값.
//    g_bPbRelse1Rslt = sRcvMsg.Pos("RDROP");
//
//    //임시적인 드롭 확인. 나중에 3칩 다 적용 되게 바꾸거나 주석처리.
//    for(int i = 0 ; i <4 ; i++){
//        sRcvQue[i] = sRcvMsg.sprintf("%02x", RcvBuff[iBuffCnt+i]);
//    }
//    for(int j = 0 ; j < 4 ; j++){
//        sRcvQue[j].sprintf("%s", convertHex2Bin(sRcvQue[j].c_str()));
//    }
//    sTemp = sRcvQue[1] + sRcvQue[0];
//    g_sPb1Bin[0] = sTemp;
//    //Label1 -> Caption = sTemp;
//    sTemp = sRcvQue[3] + sRcvQue[2];
//    g_sPb1Bin[1] = sTemp;
//    //Label2 -> Caption = sTemp;
//    sRcvMsg.Delete(sRcvMsg.Pos("RDROP"), 10);
//    //}
//     */
//    //임시 화면 디스플레이용... 1칩만 고려 해서 만듬.
//    //확인 하고 나중에 주석 하던가 고쳐서 3칩 까지 쓸수 있게 하던가...
//    g_sPb1Vf[0] = "" ;
//    g_sPb1Vf[1] = "" ;
//
//    AnsiString sCh ;
//    AnsiString sChipData ;
//    AnsiString sRcvDataTemp = sRcvMsg ;
//    double     dChipData ;
//    int iCnt = -1 ;
//    for(int j=0; j>-OM.DevOptn.iContColCnt / 2; j-- ) {
//       for(int i = OM.DevInfo.iRowCnt - 1 ; i >= 0 ; i --) {
//           for(int k=0; k< OM.DevOptn.iDgChipCnt; k++ ) {//대겸 칩 카운트가 아니라 그냥 칩카운트로 바꾸고 GL 에서도 사용해야됨.
//               iCnt++ ;
//               sCh.sprintf("RL%02d",iCnt);
//               sChipData = sRcvDataTemp.SubString(sRcvMsg.Pos(sCh)+5,4); //숫자 데이터만 짤라냄.
//               sRcvDataTemp.Delete(sRcvMsg.Pos(sCh),9);
//               sChipData = sChipData.SubString(0,2) + "." + sChipData.SubString(3,4) + " ";
//               g_sPb1Vf[j] += sChipData ;
//           }
//       }
//    }
//
//
//
//
//
// /*
//
//               if(_iCh == PROBE_CH2) {
//                   if(OM.MstOptn.d2chLedGain) dChipData = dChipData * OM.MstOptn.d2chLedGain   ;
//                   dChipData = dChipData + OM.MstOptn.d2chLedOffset ;
//
//                   if(OM.MstOptn.bShortMonitor && dChipData < 2.0) EM_SetErr(eiWRK_ShrtMonitor) ;
//                   dCChipVfCrnt[i][OM.DevOptn.iContColCnt/2 - 1 + j][k] = dChipData ;
//               }
//
//               //LED Data Cal.
//               if(iCChipRslt[i][c+j][k] != prOk || _bRpt) {
//                   dCChipVf[i][c+j][k] = dChipData ;
//               }
//
//               if(OM.DevOptn.bSideLEDControl && _iCh == PROBE_CH1){
//                   iTemp[i]= iArray[-j][i];
//                   bHtRslt = !iArray[-j][i];
//               }
//               else if(OM.DevOptn.bSideLEDControl && _iCh == PROBE_CH2){
//                   iTemp[i]= iArray[OM.DevOptn.iContColCnt - 2 -j][i];
//                   bHtRslt = !iArray[OM.DevOptn.iContColCnt - 2 -j][i];
//               }
//               else {
//                   bHtRslt = true;
//               }
//*/
//
//
//
//
//
//
//
//
//
//
//
//
//    if(FrmProbe)FrmProbe -> edPb1ChData  -> Text = sRcvMsg ;
//    if(FrmProbe)FrmProbe -> edPb1ChData2 -> Text = sRcvMsg ;
//
//    g_sPb1Msg = sRcvMsg ;
//    sRcvMsg = "" ;
//    g_bPb1Rslt = true ;
//
//    //}
//    /*
//    else
//    {
//        if(sRcvMsg.Pos("\rPUT")) {
//            sRcvMsg.Delete(sRcvMsg.Pos("\rPUT"),5);
//        }
//
//        if(sRcvMsg.Pos("T")) {
//            FrmDeviceSet->mmProbeTest->Lines->Add(sRcvTestMsg) ;
//        }
//
//        if(!sRcvMsg.Pos("\r"))return ;
//
//        if(OM.MstOptn.bDebugMode)Trace("RS232C_Pb1ch_Msg",sRcvMsg.c_str());
//
//        FrmProbe -> edPb1ChData  -> Text = sRcvMsg ;
//        FrmProbe -> edPb1ChData2 -> Text = sRcvMsg ;
//
//        g_sPb1Msg = sRcvMsg ;
//        sRcvMsg = "" ;
//        g_bPb1Rslt = true ;
//    }
//    */
//
//
//}
//
//
//
//
//


void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
    if(OM.EqpOptn.bExistLoader  ) MT_SetServo((int)miLDR_YCmp , false) ;
    if(OM.EqpOptn.bExistLoader  ) MT_SetServo((int)miLDR_ZCmp , false) ;
    if(OM.EqpOptn.bExistUnLoader) MT_SetServo((int)miULD_YCmp , false) ;
    if(OM.EqpOptn.bExistUnLoader) MT_SetServo((int)miULD_ZCmp , false) ;

    MT_SetServo((int)miPR1_XCmp , false) ;
    MT_SetServo((int)miPR2_XCmp , false) ;
    MT_SetServo((int)miWRK_XCmp , false) ;
    MT_SetServo((int)miPSB_XCmp , false) ;
    MT_SetServo((int)miWRK_ZTop , false) ;
    MT_SetServo((int)miWRK_ZBtm , false) ;
    MT_SetServo((int)miSTL_YTop , false) ;
    MT_SetServo((int)miREJ_XBtm , false) ;

    FM_DelWin();
}
//---------------------------------------------------------------------------

//1칩이고 두채널 합처서 2열 컨텍일경우.
//2칩이거나  두채널 합처서 4열 컨텍일경우.
void __fastcall TFrmMain::lbVerDblClick(TObject *Sender)
{
    FrmInform->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::tmTempuretueTimer(TObject *Sender)
{
    if(!OM.MstOptn.bUsedTempRS232) {
        tmTempuretue -> Enabled = false ;
        TCU.RqstCrntTemp();
        tmTempuretue -> Enabled = true ;
    }
}
//---------------------------------------------------------------------------





