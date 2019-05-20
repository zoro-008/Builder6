//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "FormMain.h"
#include "FormMaster.h"
#include "FormProbe.h"
#include "FormSPC.h"
#include "FormInform.h"

#include "MainThread.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "Sequence.h"
#include "OptionMan.h"
#include "uRs232c.h"
#include "ProbeComUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMain *FrmMain;
TRS232C  *RS232C_Pb1ch ;
TRS232C  *RS232C_Pb2ch ;

bool g_bPb1Rslt ;
bool g_bPb2Rslt ;
AnsiString g_sPb1Msg ;
AnsiString g_sPb2Msg ;

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
    AnsiString Date = FileDateToDateTime(Age).FormatString("''yyyy'년 'm'월 'd'일 'AM/PM' 'h'시 'n'분'");
    lbDate -> Caption = Date ;

    //Last Device Load
    OM.LoadLastInfo();
//    MT_SetServoAll(true);
    IO_SetY(yLD1_ZBreak,false );
    IO_SetY(yULD_ZBreak,false );
    ::Sleep(100);

    if(OM.EqpOptn.bExistLoader  ) MT_SetServo((int)miLD1_YCmp , true) ;
    if(OM.EqpOptn.bExistLoader  ) MT_SetServo((int)miLD1_ZCmp , true) ;
    if(OM.EqpOptn.bExistUnLoader) MT_SetServo((int)miULD_YCmp , true) ;
    if(OM.EqpOptn.bExistUnLoader) MT_SetServo((int)miULD_ZCmp , true) ;
    MT_SetServo((int)miPRB_XCmp , true) ;
    MT_SetServo((int)miWRK_XCmp , true) ;
    MT_SetServo((int)miPSB_XCmp , true) ;
    MT_SetServo((int)miWRK_ZTop , true) ;
    MT_SetServo((int)miWRK_ZBtm , true) ;
    MT_SetServo((int)miSTL_YTop , true) ;
    MT_SetServo((int)miSTL_YBtm , true) ;
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
    FrmOperation -> Parent = pnBase ; FrmOperation -> Left = 0 ; FrmOperation -> Top = 0 ; FrmOperation -> Width = 1280 ; FrmOperation -> Height = 863 ;
    FrmDevice    -> Parent = pnBase ; FrmDevice    -> Left = 0 ; FrmDevice    -> Top = 0 ; FrmDevice    -> Width = 1280 ; FrmDevice    -> Height = 863 ;
    FrmDeviceSet -> Parent = pnBase ; FrmDeviceSet -> Left = 0 ; FrmDeviceSet -> Top = 0 ; FrmDeviceSet -> Width = 1280 ; FrmDeviceSet -> Height = 863 ;
    FrmOption    -> Parent = pnBase ; FrmOption    -> Left = 0 ; FrmOption    -> Top = 0 ; FrmOption    -> Width = 1280 ; FrmOption    -> Height = 863 ;
    FrmSPC       -> Parent = pnBase ; FrmSPC       -> Left = 0 ; FrmSPC       -> Top = 0 ; FrmSPC       -> Width = 1280 ; FrmSPC       -> Height = 863 ;
    FrmMaster    -> Parent = pnBase ; FrmMaster    -> Left = 0 ; FrmMaster    -> Top = 0 ; FrmMaster    -> Width = 1280 ; FrmMaster    -> Height = 863 ;

    ShowPage (0);
    FrmProbe->Show();

    tmUpdate -> Enabled = true ;

    if(OM.DevOptn.bLtHeaterNum2) IO_SetY(yETC_Heater1_2,true );
    else                         IO_SetY(yETC_Heater1_2,false);
    if(OM.DevOptn.bLtHeaterNum3) IO_SetY(yETC_Heater1_3,true );
    else                         IO_SetY(yETC_Heater1_3,false);
    if(OM.DevOptn.bRtHeaterNum2) IO_SetY(yETC_Heater2_2,true );
    else                         IO_SetY(yETC_Heater2_2,false);
    if(OM.DevOptn.bRtHeaterNum3) IO_SetY(yETC_Heater2_3,true );
    else                         IO_SetY(yETC_Heater2_3,false);

    //    SEQ.Reset();
}
//---------------------------------------------------------------------------
void __fastcall  TFrmMain::ShowPage (int index)
{
    switch(index)
    {
        case 0: FrmOperation -> Show() ;
//                FM_SetLevel(lvOperator); break ;
                break ;
        case 1: FrmDevice    -> Show() ; break ;
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

    FrmMain->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{

//    OM.SaveLastInfo();
    FrmOperation ->Close();
    FrmDevice    ->Close();
    FrmDeviceSet ->Close();
    FrmOption    ->Close();
    FrmMaster    ->Close();

    tmUpdate -> Enabled = false ;

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

    //APC Rs-232
    RS232C_Pb1ch = new TRS232C ;
    if(!RS232C_Pb1ch->Open(0,Handle)) {ShowMessage("RS-232C Probe 1 Ch Open Failed!"); bSuccess = false;}
    RS232C_Pb2ch = new TRS232C ;
    if(!RS232C_Pb2ch->Open(1,Handle)) {ShowMessage("RS-232C Probe 2 Ch Open Failed!"); bSuccess = false;}

//         if((bSuccess && OM.MstOptn.bUseDaegyeomProbe)) PCM.SetDaegyeom();
//    else if
    if(OM.MstOptn.bUseDaegyeomProbe) PCM.SetDaegyeom();
    else tmMainSet->Enabled = true ;

    //Ip address check
//    IdIIcmp->Host = OM.MstOptn.sPstEquipIp;
//    IdIIcmp->Ping();

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
void __fastcall TFrmMain::HeatProbe1Ch(TMessage &Message)
{
//    Sleep(1);
    int iLen;
    BYTE RcvBuff[300];

    static AnsiString sRcvMsg = "" ;
    static AnsiString sRcvTestMsg = "" ;
    static CDelayTimer tmDelay ;

    memset(&RcvBuff, 0 , sizeof(RcvBuff));

    iLen = RS232C_Pb1ch->ReadData(Message.WParam, RcvBuff);
//    if(OM.MstOptn.bDebugMode)Trace("RS232C_Pb1ch",RcvBuff);

    //if(tmDelay.OnDelay(true , 400)) { sRcvMsg = "" ; tmDelay.Clear();}

    sRcvMsg += (char*)RcvBuff ;

    sRcvTestMsg = (char*)RcvBuff ;
    Trace("RS232C_Pb1ch_Msg",sRcvTestMsg.c_str());

    if(OM.MstOptn.bUseDaegyeomProbe)
    {
        if(sRcvMsg.Pos("OK\r")) sRcvMsg.Delete(sRcvMsg.Pos("OK\r"),3);
        if(!sRcvMsg.Pos("[")) return ;
        if(OM.MstOptn.bDebugMode)Trace("RS232C_Pb1ch_Msg",sRcvMsg.c_str());

        if(FrmProbe)FrmProbe -> edPb1ChData -> Text = sRcvMsg ;

        g_sPb1Msg = sRcvMsg ;
        sRcvMsg = "" ;
        g_bPb1Rslt = true ;
    }
    else
    {
        if(sRcvMsg.Pos("\rPUT")) {
            sRcvMsg.Delete(sRcvMsg.Pos("\rPUT"),5);
        }

        if(sRcvMsg.Pos("T")) {
            FrmDeviceSet->mmProbeTest->Lines->Add(sRcvTestMsg) ;
        }

        if(!sRcvMsg.Pos("\r"))return ;

        if(OM.MstOptn.bDebugMode)Trace("RS232C_Pb1ch_Msg",sRcvMsg.c_str());

        FrmProbe -> edPb1ChData -> Text = sRcvMsg ;

        g_sPb1Msg = sRcvMsg ;
        sRcvMsg = "" ;
        g_bPb1Rslt = true ;
    }

}
//---------------------------------------------------------------------------
// 쓰레드에 의해 Data가 수신되면 SendMessage에 의해 호출된다. 즉 수신버퍼에
// TMessage의 WParam의 길이만큼 Data가 수신된 후 호출된다
void __fastcall TFrmMain::HeatProbe2Ch(TMessage &Message)
{
//    Sleep(1);
    int iLen;
    BYTE RcvBuff[300];

    static AnsiString sRcvMsg = "" ;
    static AnsiString sRcvTestMsg = "" ;
    static CDelayTimer tmDelay ;

    memset(&RcvBuff, 0 , sizeof(RcvBuff));

    iLen = RS232C_Pb2ch->ReadData(Message.WParam, RcvBuff);

//    if(OM.MstOptn.bDebugMode)Trace("RS232C_Pb2ch",RcvBuff);

    //if(tmDelay.OnDelay(true ,2000)) { sRcvMsg = "" ; tmDelay.Clear();}

    sRcvMsg += (char*)RcvBuff ;

    sRcvTestMsg = (char*)RcvBuff ;

    Trace("RS232C_Pb2ch_Msg",sRcvTestMsg.c_str());

    if(OM.MstOptn.bUseDaegyeomProbe)
    {
        if(sRcvMsg.Pos("OK\r")) sRcvMsg.Delete(sRcvMsg.Pos("OK\r"),3);
        if(!sRcvMsg.Pos("[")) return ;
        if(OM.MstOptn.bDebugMode)Trace("RS232C_Pb1ch_Msg",sRcvMsg.c_str());

        if(FrmProbe)FrmProbe -> edPb2ChData -> Text = sRcvMsg ;
        g_sPb2Msg = sRcvMsg ;
        sRcvMsg = "" ;
        g_bPb2Rslt = true ;
    }
    else
    {
        if(sRcvMsg.Pos("\rPUT")) {
            sRcvMsg.Delete(sRcvMsg.Pos("\rPUT"),5);
        }

        if(sRcvMsg.Pos("T")) {
            FrmDeviceSet->mmProbeTest->Lines->Add(sRcvTestMsg) ;
        }

        if(!sRcvMsg.Pos("\r"))return ;

        if(OM.MstOptn.bDebugMode)Trace("RS232C_Pb2ch_Msg",sRcvMsg.c_str());

        FrmProbe -> edPb2ChData -> Text = sRcvMsg ;

        g_sPb2Msg = sRcvMsg ;
        sRcvMsg = "" ;
        g_bPb2Rslt = true ;
    }

}

void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
    if(OM.EqpOptn.bExistLoader  ) MT_SetServo((int)miLD1_YCmp , false) ;
    if(OM.EqpOptn.bExistLoader  ) MT_SetServo((int)miLD1_ZCmp , false) ;
    if(OM.EqpOptn.bExistUnLoader) MT_SetServo((int)miULD_YCmp , false) ;
    if(OM.EqpOptn.bExistUnLoader) MT_SetServo((int)miULD_ZCmp , false) ;

    MT_SetServo((int)miPRB_XCmp , false) ;
    MT_SetServo((int)miWRK_XCmp , false) ;
    MT_SetServo((int)miPSB_XCmp , false) ;
    MT_SetServo((int)miWRK_ZTop , false) ;
    MT_SetServo((int)miWRK_ZBtm , false) ;
    MT_SetServo((int)miSTL_YTop , false) ;
    MT_SetServo((int)miSTL_YBtm , false) ;
    MT_SetServo((int)miREJ_XBtm , false) ;

    FM_DelWin();
    delete RS232C_Pb1ch ;
    delete RS232C_Pb2ch ;
}
//---------------------------------------------------------------------------

//1칩이고 두채널 합처서 2열 컨텍일경우.
void __fastcall TFrmMain::tmMainSetTimer(TObject *Sender)
{
//    tmOpen -> Enabled = false ;

    static int         iStep    = 10 ;
    static int         iPreStep = 0  ;
    static CDelayTimer tmCycle ;

    //SET,12,0400,0100,15,T#
    AnsiString sMsg,sMsg1,sMsg2;
    AnsiString sTemp ;
    int i1ChLedCur, i2ChLedCur , i1ChZenCur, i2ChZenCur ;



    i1ChLedCur = (OM.DevOptn.i1ChLedCur * 1000 ) / 50 ;
    i1ChZenCur = (OM.DevOptn.i1ChZenCur * 1000 ) / 50 ;

    i2ChLedCur = (OM.DevOptn.i2ChLedCur * 1000 ) / 50 ;
    i2ChZenCur = (OM.DevOptn.i2ChZenCur * 1000 ) / 50 ;

    sMsg1 = "SET,";

    if( OM.DevOptn.i1Ch1P && OM.DevOptn.i1Ch1N){ sMsg1 = sMsg1 + "C1" ;}
//  if( OM.DevOptn.i1Ch2P && OM.DevOptn.i1Ch2N){ sMsg1 = sMsg1 + "C2" ;}
//  if( OM.DevOptn.i1Ch3P && OM.DevOptn.i1Ch3N){ sMsg1 = sMsg1 + "C3" ;}

    if( OM.DevOptn.i1Ch1P && OM.DevOptn.i1Ch1N ){ sMsg1 = sMsg1 + sTemp.sprintf("%01d%01d,",OM.DevOptn.i1Ch1P,OM.DevOptn.i1Ch1N ) ; }
//  if( OM.DevOptn.i1Ch2P && OM.DevOptn.i1Ch2N ){ sMsg1 = sMsg1 + sTemp.sprintf("%01d%01d,",OM.DevOptn.i1Ch2P,OM.DevOptn.i1Ch2N ) ; }
//  if( OM.DevOptn.i1Ch3P && OM.DevOptn.i1Ch3N ){ sMsg1 = sMsg1 + sTemp.sprintf("%01d%01d,",OM.DevOptn.i1Ch3P,OM.DevOptn.i1Ch3N ) ; }
    if( OM.DevOptn.i1ChLedCur                  ){ sMsg1 = sMsg1 + sTemp.sprintf("%04d,"    ,i1ChLedCur                          ) ; }
    if( OM.DevOptn.i1ChZenCur                  ){ sMsg1 = sMsg1 + sTemp.sprintf("%04d,"    ,i1ChZenCur                          ) ; }
    //if( OM.DevInfo.iRowCnt                     ){ sMsg1 = sMsg1 + sTemp.sprintf("%02d,"    ,OM.DevInfo.iRowCnt                ) ; }
    if( OM.DevOptn.bUseZener                   ){ sMsg1 = sMsg1 + "T#"                                                            ; }
    else                                        { sMsg1 = sMsg1 + "F#"                                                            ; }

    sMsg2 = "SET,";
    if( OM.DevOptn.i2Ch1P && OM.DevOptn.i1Ch1N ){ sMsg2 = sMsg2 + "C1" ; }
//  if( OM.DevOptn.i2Ch2P && OM.DevOptn.i1Ch2N ){ sMsg2 = sMsg2 + "C2" ; }
//  if( OM.DevOptn.i2Ch3P && OM.DevOptn.i1Ch3N ){ sMsg2 = sMsg2 + "C3" ; }

    if( OM.DevOptn.i2Ch1P && OM.DevOptn.i1Ch1N ){ sMsg2 = sMsg2 + sTemp.sprintf("%01d%01d,",OM.DevOptn.i2Ch1P,OM.DevOptn.i1Ch1N ) ; }
//  if( OM.DevOptn.i2Ch2P && OM.DevOptn.i1Ch2N ){ sMsg2 = sMsg2 + sTemp.sprintf("%01d%01d,",OM.DevOptn.i2Ch2P,OM.DevOptn.i1Ch2N ) ; }
//  if( OM.DevOptn.i2Ch3P && OM.DevOptn.i1Ch3N ){ sMsg2 = sMsg2 + sTemp.sprintf("%01d%01d,",OM.DevOptn.i2Ch3P,OM.DevOptn.i1Ch3N ) ; }
    if( OM.DevOptn.i2ChLedCur                  ){ sMsg2 = sMsg2 + sTemp.sprintf("%04d,"    ,i2ChLedCur                          ) ; }
    if( OM.DevOptn.i2ChZenCur                  ){ sMsg2 = sMsg2 + sTemp.sprintf("%04d,"    ,i2ChZenCur                          ) ; }
    //if( OM.DevInfo.iRowCnt                     ){ sMsg2 = sMsg2 + sTemp.sprintf("%02d,"    ,OM.DevInfo.iRowCnt                ) ; }
    if( OM.DevOptn.bUseZener                   ){ sMsg2 = sMsg2 + "T#"                                                            ; }
    else                                        { sMsg2 = sMsg2 + "F#"                                                            ; }

    if (tmCycle.OnDelay(iStep>10 && iStep == iPreStep && !OM.MstOptn.bDebugMode, 4000 )) {
        EM_SetErr(eiWRK_ProbeSetFail);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : iStep=%02d" , __FUNC__ , iStep );
        Trace("Probe",sTemp.c_str());
        iStep = 10 ;
        tmMainSet -> Enabled = false ;
        return ;
    }

    iPreStep = iStep ;

    switch (iStep)
    {
        default : iStep = 10 ;
                  tmMainSet -> Enabled = false ;

        case  10: g_bPb1Rslt = false ;
                  sMsg = "SET,RLL0800#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  m_tmTemp.Clear();
                  iStep++;
                  return ;

        case  11: //if(m_tmTemp.OnDelay(true,4000)) {
                  //    EM_SetErr(eiWRK_ProbeSetFail);
                  //    tmMainSet -> Enabled = false ;
                  //    iStep = 10 ;
                  //}
                  if(!g_bPb1Rslt) return ;
                  g_bPb1Rslt = false ;
                  sMsg = "SET,RLH1200#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  12: if(!g_bPb1Rslt) return ;
                  g_bPb1Rslt = false ;
                  sMsg = "SET,RZL0300#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  13: if(!g_bPb1Rslt) return ;
                  g_bPb1Rslt = false ;
                  sMsg = "SET,RZH0600#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  //iStep = 17 ;
                  return ;

        case  14: if(!g_bPb1Rslt) return ;
                  g_bPb1Rslt = false ;

                  sMsg = sTemp.sprintf("%04d" ,OM.DevOptn.i1ChDelay ) ;

                  sMsg = "SET,DLY"+ sMsg + "#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());

                  /*
                  sMsg = "SET,DRY"+(AnsiString)OM.DevOptn.i1ChDelay + "#" ;
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  */
                  iStep++;
                  return ;

        case  15: if(!g_bPb1Rslt) return ;
                  g_bPb1Rslt = false ;
                  sMsg = sTemp.sprintf("%04d" ,OM.DevOptn.i1ChLedTime ) ;

                  sMsg = "SET,LIT"+ sMsg + "#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  /*
                  sMsg = "SET,LIT"+(AnsiString)OM.DevOptn.i1ChLedTime + "#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  */
                  iStep++;
                  return ;

        case  16: if(!g_bPb1Rslt) return ;
                  g_bPb1Rslt = false ;

                  sMsg = sTemp.sprintf("%04d" ,OM.DevOptn.i1ChZenTime ) ;

                  sMsg = "SET,ZIT"+ sMsg + "#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  /*
                  sMsg = "SET,ZIT"+(AnsiString)OM.DevOptn.i1ChZenTime + "#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  */
                  iStep++;
                  return ;

        case  17: if(!g_bPb1Rslt) return ;
                  g_bPb1Rslt = false ;

                  sMsg = sTemp.sprintf("%02d" ,OM.DevInfo.iRowCnt ) ;

                  sMsg = "SET,LN"+ sMsg + "#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  18: if(!g_bPb1Rslt) return ;
                  g_bPb1Rslt = false ;

                  if(OM.DevOptn.bUseTwiceInsp) sMsg = "SET,PIS0001#";
                  else                         sMsg = "SET,PIS0000#";

                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  19: if(!g_bPb1Rslt) return ;
                  g_bPb1Rslt = false ;
                  RS232C_Pb1ch->WriteData(sMsg1.Length(),sMsg1.c_str());
                  iStep++;
                  return ;

//Chanel 2
///---------------------------------------------------------------------------

        case  20: g_bPb2Rslt = false ;
                  sMsg = "SET,RLL0800#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  21: if(!g_bPb2Rslt) return ;
                  g_bPb2Rslt = false ;
                  sMsg = "SET,RLH1200#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  22: if(!g_bPb2Rslt) return ;
                  g_bPb2Rslt = false ;
                  sMsg = "SET,RZL0300#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  23: if(!g_bPb2Rslt) return ;
                  g_bPb2Rslt = false ;
                  sMsg = "SET,RZH0600#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  24: if(!g_bPb2Rslt) return ;
                  g_bPb2Rslt = false ;

                  sMsg = sTemp.sprintf("%04d" ,OM.DevOptn.i2ChZenTime ) ;

                  sMsg = "SET,DLY"+ sMsg + "#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());

                  /*
                  sMsg = "SET,DRY"+(AnsiString)OM.DevOptn.i2ChDelay + "#" ;
//                  sMsg = "SET,DLY1000#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
                  */
                  iStep++;
                  return ;

        case  25: if(!g_bPb2Rslt) return ;
                  g_bPb2Rslt = false ;

                  sMsg = sTemp.sprintf("%04d" ,OM.DevOptn.i2ChLedTime ) ;

                  sMsg = "SET,LIT"+ sMsg + "#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());

                  /*
                  sMsg = "SET,LIT"+(AnsiString)OM.DevOptn.i2ChLedTime + "#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
                  */
                  iStep++;
                  return ;

        case  26: if(!g_bPb2Rslt) return ;
                  g_bPb2Rslt = false ;

                  sMsg = sTemp.sprintf("%04d" ,OM.DevOptn.i2ChZenTime ) ;

                  sMsg = "SET,ZIT"+ sMsg + "#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());

                  /*
                  sMsg = "SET,ZIT"+(AnsiString)OM.DevOptn.i2ChZenTime + "#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
                  */
                  iStep++;
                  return ;

        case  27: if(!g_bPb2Rslt) return ;
                  g_bPb2Rslt = false ;

                  sMsg = sTemp.sprintf("%02d" ,OM.DevInfo.iRowCnt ) ;

                  sMsg = "SET,LN"+ sMsg + "#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  28: if(!g_bPb2Rslt) return ;
                  RS232C_Pb2ch->WriteData(sMsg2.Length(),sMsg2.c_str());
                  iStep++;
                  return ;

        case  29: if(!g_bPb2Rslt) return ;
                  g_bPb2Rslt = false ;

                  if(OM.DevOptn.bUseTwiceInsp) sMsg = "SET,PIS0001#";
                  else                         sMsg = "SET,PIS0000#";

                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  30: if(!g_bPb2Rslt) return ;
                  iStep = 10;
                  tmMainSet -> Enabled = false ;

                  //2칩이거나  두채널 합처서 4열 컨텍일경우.
                  if( OM.DevOptn.i1Ch2P && OM.DevOptn.i1Ch2N){
                      tmSubSet -> Enabled = true  ;
                  }
                  return ;
    }
}
//---------------------------------------------------------------------------

//2칩이거나  두채널 합처서 4열 컨텍일경우.
void __fastcall TFrmMain::tmSubSetTimer(TObject *Sender)
{
//    tmOpen -> Enabled = false ;

    static int         iStep    = 10 ;
    static int         iPreStep = 0  ;
    static CDelayTimer tmCycle ;

    //SET,12,0400,0100,15,T#
    AnsiString sMsg,sMsg1,sMsg2;
    AnsiString sTemp ;
    int i1ChLedCur, i2ChLedCur , i1ChZenCur, i2ChZenCur ;
    i1ChLedCur = (OM.DevOptn.i1ChLedCur * 1000 ) / 50 ;
    i1ChZenCur = (OM.DevOptn.i1ChZenCur * 1000 ) / 50 ;

    i2ChLedCur = (OM.DevOptn.i2ChLedCur * 1000 ) / 50 ;
    i2ChZenCur = (OM.DevOptn.i2ChZenCur * 1000 ) / 50 ;

    sMsg1 = "SET,";

//  if( OM.DevOptn.i1Ch1P && OM.DevOptn.i1Ch1N){ sMsg1 = sMsg1 + "C2" ;}
    if( OM.DevOptn.i1Ch2P && OM.DevOptn.i1Ch2N){ sMsg1 = sMsg1 + "C2" ;}
//  if( OM.DevOptn.i1Ch3P && OM.DevOptn.i1Ch3N){ sMsg1 = sMsg1 + "C3" ;}

//  if( OM.DevOptn.i1Ch1P && OM.DevOptn.i1Ch1N ){ sMsg1 = sMsg1 + sTemp.sprintf("%01d%01d,",OM.DevOptn.i1Ch1P,OM.DevOptn.i1Ch1N ) ; }
    if( OM.DevOptn.i1Ch2P && OM.DevOptn.i1Ch2N ){ sMsg1 = sMsg1 + sTemp.sprintf("%01d%01d,",OM.DevOptn.i1Ch2P,OM.DevOptn.i1Ch2N ) ; }
//  if( OM.DevOptn.i1Ch3P && OM.DevOptn.i1Ch3N ){ sMsg1 = sMsg1 + sTemp.sprintf("%01d%01d,",OM.DevOptn.i1Ch3P,OM.DevOptn.i1Ch3N ) ; }
    if( OM.DevOptn.i1ChLedCur                  ){ sMsg1 = sMsg1 + sTemp.sprintf("%04d,"    ,i1ChLedCur                          ) ; }
    if( OM.DevOptn.i1ChZenCur                  ){ sMsg1 = sMsg1 + sTemp.sprintf("%04d,"    ,i1ChZenCur                          ) ; }
    //if( OM.DevInfo.iRowCnt                   ){ sMsg1 = sMsg1 + sTemp.sprintf("%02d,"    ,OM.DevInfo.iRowCnt                  ) ; }
    if( OM.DevOptn.bUseZener                   ){ sMsg1 = sMsg1 + "T#"                                                            ; }
    else                                        { sMsg1 = sMsg1 + "F#"                                                            ; }

    sMsg2 = "SET,";
//  if( OM.DevOptn.i2Ch1P && OM.DevOptn.i1Ch1N ){ sMsg2 = sMsg2 + "C2" ; }
    if( OM.DevOptn.i2Ch2P && OM.DevOptn.i1Ch2N ){ sMsg2 = sMsg2 + "C2" ; }
//  if( OM.DevOptn.i2Ch3P && OM.DevOptn.i1Ch3N ){ sMsg2 = sMsg2 + "C3" ; }

//  if( OM.DevOptn.i2Ch1P && OM.DevOptn.i1Ch1N ){ sMsg2 = sMsg2 + sTemp.sprintf("%01d%01d,",OM.DevOptn.i2Ch1P,OM.DevOptn.i1Ch1N ) ; }
    if( OM.DevOptn.i2Ch2P && OM.DevOptn.i1Ch2N ){ sMsg2 = sMsg2 + sTemp.sprintf("%01d%01d,",OM.DevOptn.i2Ch2P,OM.DevOptn.i1Ch2N ) ; }
//  if( OM.DevOptn.i2Ch3P && OM.DevOptn.i1Ch3N ){ sMsg2 = sMsg2 + sTemp.sprintf("%01d%01d,",OM.DevOptn.i2Ch3P,OM.DevOptn.i1Ch3N ) ; }
    if( OM.DevOptn.i2ChLedCur                  ){ sMsg2 = sMsg2 + sTemp.sprintf("%04d,"    ,i2ChLedCur                          ) ; }
    if( OM.DevOptn.i2ChZenCur                  ){ sMsg2 = sMsg2 + sTemp.sprintf("%04d,"    ,i2ChZenCur                          ) ; }
    //if( OM.DevInfo.iRowCnt                   ){ sMsg2 = sMsg2 + sTemp.sprintf("%02d,"    ,OM.DevInfo.iRowCnt                  ) ; }
    if( OM.DevOptn.bUseZener                   ){ sMsg2 = sMsg2 + "T#"                                                            ; }
    else                                        { sMsg2 = sMsg2 + "F#"                                                            ; }


    if (tmCycle.OnDelay(iStep>10 && iStep == iPreStep && !OM.MstOptn.bDebugMode, 4000 )) {
        EM_SetErr(eiWRK_ProbeSetFail);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : iStep=%02d" , __FUNC__ , iStep );
        Trace("Probe",sTemp.c_str());
        iStep = 10 ;
        tmSubSet -> Enabled = false ;
        return ;
    }

    iPreStep = iStep ;

//    No Insp 0000
//    SET,PIS0000#
//    SET,PIS0001#








    switch (iStep)
    {
        default : iStep = 10 ;
                  tmSubSet -> Enabled = false ;

        case  10: g_bPb1Rslt = false ;
                  sMsg = "SET,RLL0800#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  m_tmTemp.Clear();
                  iStep++;
                  return ;

        case  11: //if(m_tmTemp.OnDelay(true,4000)) {
                  //    EM_SetErr(eiWRK_ProbeSetFail);
                  //    iStep = 0 ;
                  //}
                  if(!g_bPb1Rslt) return ;
                  g_bPb1Rslt = false ;
                  sMsg = "SET,RLH1200#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  12: if(!g_bPb1Rslt) return ;
                  g_bPb1Rslt = false ;
                  sMsg = "SET,RZL0300#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  13: if(!g_bPb1Rslt) return ;
                  g_bPb1Rslt = false ;
                  sMsg = "SET,RZH0600#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  //iStep = 17 ;
                  return ;

        case  14: if(!g_bPb1Rslt) return ;
                  g_bPb1Rslt = false ;

                  sMsg = sTemp.sprintf("%04d" ,OM.DevOptn.i1ChDelay ) ;

                  sMsg = "SET,DLY"+ sMsg + "#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());

                  /*
                  sMsg = "SET,DRY"+(AnsiString)OM.DevOptn.i1ChDelay + "#" ;
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  */
                  iStep++;
                  return ;

        case  15: if(!g_bPb1Rslt) return ;
                  g_bPb1Rslt = false ;
                  sMsg = sTemp.sprintf("%04d" ,OM.DevOptn.i1ChLedTime ) ;

                  sMsg = "SET,LIT"+ sMsg + "#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  /*
                  sMsg = "SET,LIT"+(AnsiString)OM.DevOptn.i1ChLedTime + "#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  */
                  iStep++;
                  return ;

        case  16: if(!g_bPb1Rslt) return ;
                  g_bPb1Rslt = false ;

                  sMsg = sTemp.sprintf("%04d" ,OM.DevOptn.i1ChZenTime ) ;

                  sMsg = "SET,ZIT"+ sMsg + "#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  /*
                  sMsg = "SET,ZIT"+(AnsiString)OM.DevOptn.i1ChZenTime + "#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  */
                  iStep++;
                  return ;

        case  17: if(!g_bPb1Rslt) return ;
                  g_bPb1Rslt = false ;

                  sMsg = sTemp.sprintf("%02d" ,OM.DevInfo.iRowCnt ) ;

                  sMsg = "SET,LN"+ sMsg + "#";
                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  18: if(!g_bPb1Rslt) return ;
                  g_bPb1Rslt = false ;

                  if(OM.DevOptn.bUseTwiceInsp) sMsg = "SET,PIS0001#";
                  else                         sMsg = "SET,PIS0000#";

                  RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  19: if(!g_bPb1Rslt) return ;
                  g_bPb1Rslt = false ;
                  RS232C_Pb1ch->WriteData(sMsg1.Length(),sMsg1.c_str());
                  iStep++;
                  return ;



//Chanel 2
///---------------------------------------------------------------------------




        case  20: g_bPb2Rslt = false ;
                  sMsg = "SET,RLL0800#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  21: if(!g_bPb2Rslt) return ;
                  g_bPb2Rslt = false ;
                  sMsg = "SET,RLH1200#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  22: if(!g_bPb2Rslt) return ;
                  g_bPb2Rslt = false ;
                  sMsg = "SET,RZL0300#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  23: if(!g_bPb2Rslt) return ;
                  g_bPb2Rslt = false ;
                  sMsg = "SET,RZH0600#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  24: if(!g_bPb2Rslt) return ;
                  g_bPb2Rslt = false ;

                  sMsg = sTemp.sprintf("%04d" ,OM.DevOptn.i2ChZenTime ) ;

                  sMsg = "SET,DLY"+ sMsg + "#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());

                  /*
                  sMsg = "SET,DRY"+(AnsiString)OM.DevOptn.i2ChDelay + "#" ;
//                  sMsg = "SET,DLY1000#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
                  */
                  iStep++;
                  return ;

        case  25: if(!g_bPb2Rslt) return ;
                  g_bPb2Rslt = false ;

                  sMsg = sTemp.sprintf("%04d" ,OM.DevOptn.i2ChLedTime ) ;

                  sMsg = "SET,LIT"+ sMsg + "#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());

                  /*
                  sMsg = "SET,LIT"+(AnsiString)OM.DevOptn.i2ChLedTime + "#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
                  */
                  iStep++;
                  return ;

        case  26: if(!g_bPb2Rslt) return ;
                  g_bPb2Rslt = false ;

                  sMsg = sTemp.sprintf("%04d" ,OM.DevOptn.i2ChZenTime ) ;

                  sMsg = "SET,ZIT"+ sMsg + "#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());

                  /*
                  sMsg = "SET,ZIT"+(AnsiString)OM.DevOptn.i2ChZenTime + "#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
                  */
                  iStep++;
                  return ;

        case  27: if(!g_bPb2Rslt) return ;
                  g_bPb2Rslt = false ;

                  sMsg = sTemp.sprintf("%02d" ,OM.DevInfo.iRowCnt ) ;

                  sMsg = "SET,LN"+ sMsg + "#";
                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  28: if(!g_bPb2Rslt) return ;
                  g_bPb2Rslt = false ;
                  RS232C_Pb2ch->WriteData(sMsg2.Length(),sMsg2.c_str());
                  iStep++;
                  return ;

        case  29: if(!g_bPb2Rslt) return ;
                  g_bPb2Rslt = false ;

                  if(OM.DevOptn.bUseTwiceInsp) sMsg = "SET,PIS0001#";
                  else                         sMsg = "SET,PIS0000#";

                  RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
                  iStep++;
                  return ;

        case  30: if(!g_bPb2Rslt) return ;
                  iStep = 10;
                  tmSubSet -> Enabled = false ;
                  return ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::lbVerDblClick(TObject *Sender)
{
    FrmInform->Show();
}
//---------------------------------------------------------------------------


