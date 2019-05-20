//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMain.h"
#include "UserFile.h"
#include "UserIni.h"
#include "SEQUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#define ONCOLOR (TColor)0x00DDFBFF
TFrmMain *FrmMain;
//---------------------------------------------------------------------------
__fastcall TFrmMain::TFrmMain(TComponent* Owner)
        : TForm(Owner)
{
	m_bOpen = false;
    m_bStop = false;
    for( int i = 0; i < 4; i ++)
    {
        m_nBit[i] = 1; m_nBit[i*2+1] = 1;
        OutputOld[i] = 1; OutputOld[i*2+1] = 1;
        m_nDistMode[i] = 0;
    }
    //BtCheck1 = false;
    //BtCheck2 = false;
    //BtCheck3 = false;
    //BtCheck4 = false;

    sgDevice -> ColWidths[0] = 30  ;
    sgDevice -> ColWidths[1] = 200 ;
    sgDevice -> ColWidths[2] = 200 ;


    RowCount = 0;
    m_bAxis1Stop = false;
    m_bManualStart = false ;


    //Vision.
    pGrabRun = new CBarsler();
    pGrabRun->Open(pnView -> Handle);

    //Motion
    pPaix = new CPaixMotion();
    pPaix -> SetMsgFunc(Msg) ;


    //Seq.
    pSeq = new CSEQ();
    pSeq -> SetMsgFunc(Msg) ;
    //pPaix->Resume();

    m_sCrntJobName = "NONE" ;

}

void __fastcall TFrmMain::FormCreate(TObject *Sender)
{
    Caption = String("HR System");

    pnView->DoubleBuffered = true;
    OriginalProc = pnView -> WindowProc;
    pnView->WindowProc = PanelProc;


    //StringGrid를 Update한다.
    UserFile.GridSearchDir(GetExeDir("JobFile") , sgDevice , 0 , true);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
    Ed_SelDevice -> Text = sgDevice->Cells[1][sgDevice->Row] ;
    LoadJobFile(GetLastJobName());

    TSpeedButton *pButton;
    String str;
    for( int i =0; i < 8; i ++)
    {
        str = "SBtn_Output0"+IntToStr(i);
        pButton = (TSpeedButton *)FindComponent( str );
        if( pButton != NULL )
        {
            pButton->Glyph->LoadFromResourceName((int)HInstance,"ROFF");
            pButton->Update();
        }
        str = "SBtn_Input0"+IntToStr(i);
        pButton = (TSpeedButton *)FindComponent( str );
        if( pButton != NULL )
        {
            pButton->Glyph->LoadFromResourceName((int)HInstance,"GOFF");
            pButton->Update();
        }
    }
    pButton = NULL;

    Msg("All Home 버튼을 클릭해 주세요.");

    tmUpdate -> Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false;
    tmLaser  -> Enabled = false;

    pnView->WindowProc = OriginalProc;
    ReleaseDC(this -> Handle ,Avi.hBackDC ) ;

    //모션 관련
    if(pPaix) {
        delete pPaix;
        pPaix = NULL;
    }

    //카메라.
    if(pGrabRun) {
        delete pGrabRun;
        pGrabRun = NULL;
    }

    if(pSeq) {
        delete pSeq ;
        pSeq = NULL ;
    }
}

AnsiString TFrmMain::GetLastJobName ( )
{
    //Local Var.
    TUserINI   UserINI  ;
    AnsiString FilePath ;
    AnsiString Name     ;

    //Make Dir.
    FilePath = ExtractFilePath(Application -> ExeName)  + "Util" ;
    if (!DirectoryExists(FilePath)) CreateDir(FilePath);

    FilePath = FilePath + "\\Status.INI" ;

    AnsiString sLastJobName ;

    //Load Device.
    UserINI.Load(FilePath.c_str() , "Status" , "LastJobName" , &sLastJobName);

    return sLastJobName ;
}

void TFrmMain::SaveJobFile(AnsiString _sName)
{
    //Local Var.
    TUserINI   UserINI    ;
    AnsiString FilePath   ;
    AnsiString FolderPath ;
    AnsiString Name       ;

    //Make Dir.
    FilePath = ExtractFilePath(Application -> ExeName)  + "Util" ;
    if (!DirectoryExists(FilePath)) CreateDir(FilePath);
    FilePath = FilePath + "\\Status.INI" ;
    UserINI.Save(FilePath.c_str() , "Status" , "LastJobName" , _sName);
    m_sCrntJobName = _sName  ;



    //Make Dir.
    FolderPath = ExtractFilePath(Application -> ExeName)  + "JobFile//" +_sName ;
    if (!DirectoryExists(FolderPath)) CreateDir(FolderPath);


    FilePath = FolderPath + "\\SeqCode.SEQ";
    mmSeq -> Lines -> SaveToFile(FilePath) ;

    FilePath = FolderPath + "\\ManSetting.INI";

    int    iTemp ;
    double dTemp ;

    //Load Device.
    dTemp = StrToFloatDef(edAxis1_1Pos    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_1Pos   " , dTemp);
    //dTemp = StrToFloatDef(edAxis1_2Pos    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_2Pos   " , dTemp);
    //dTemp = StrToFloatDef(edAxis1_3Pos    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_3Pos   " , dTemp);
    //dTemp = StrToFloatDef(edAxis1_4Pos    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_4Pos   " , dTemp);
    //dTemp = StrToFloatDef(edAxis1_5Pos    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_5Pos   " , dTemp);
    //dTemp = StrToFloatDef(edAxis1_6Pos    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_6Pos   " , dTemp);

    dTemp = StrToFloatDef(edAxis1_AccTime -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_AccTime" , dTemp);
    dTemp = StrToFloatDef(edAxis1_NorTime -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_NorTime" , dTemp);
    dTemp = StrToFloatDef(edAxis1_RPM     -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_RPM    " , dTemp);
    dTemp = StrToFloatDef(edAxis1_Acc     -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_Acc    " , dTemp);
    dTemp = StrToFloatDef(edAxis1_Vel     -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_Vel    " , dTemp);
    dTemp = StrToFloatDef(edAxis1_Dist    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_Dist   " , dTemp);

    dTemp = StrToFloatDef(edAxis2_1Pos    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis2" , "dAxis2_1Pos   " , dTemp);
    //dTemp = StrToFloatDef(edAxis2_2Pos    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis2" , "dAxis2_2Pos   " , dTemp);
    //dTemp = StrToFloatDef(edAxis2_3Pos    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis2" , "dAxis2_3Pos   " , dTemp);
    //dTemp = StrToFloatDef(edAxis2_4Pos    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis2" , "dAxis2_4Pos   " , dTemp);
    //dTemp = StrToFloatDef(edAxis2_5Pos    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis2" , "dAxis2_5Pos   " , dTemp);
    //dTemp = StrToFloatDef(edAxis2_6Pos    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis2" , "dAxis2_6Pos   " , dTemp);

    dTemp = StrToFloatDef(edAxis2_Acc     -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis2" , "dAxis2_Acc    " , dTemp);
    dTemp = StrToFloatDef(edAxis2_Vel     -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis2" , "dAxis2_Vel    " , dTemp);
    dTemp = StrToFloatDef(edAxis2_Dist    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis2" , "dAxis2_Dist   " , dTemp);

    iTemp = StrToIntDef  (edLaserTime     -> Text,0  ) ; UserINI.Save(FilePath.c_str() , "Laser" , "iLaserTime    " , iTemp);
}
void TFrmMain::LoadJobFile (AnsiString _sName)
{
    //Local Var.
    TUserINI   UserINI    ;
    AnsiString FilePath   ;
    AnsiString FolderPath ;
    AnsiString Name       ;

    //Make Dir.
    FolderPath = ExtractFilePath(Application -> ExeName)  + "Util" ;
    if (!DirectoryExists(FolderPath)) CreateDir(FolderPath);
    FilePath = FolderPath + "\\Status.INI" ;
    UserINI.Save(FilePath.c_str() , "Status" , "LastJobName" , _sName);

    //Make Dir.
    FolderPath = ExtractFilePath(Application -> ExeName)  + "JobFile\\" + _sName ;
    if (!DirectoryExists(FolderPath)) CreateDir(FolderPath);

    FilePath = FolderPath + "\\SeqCode.SEQ";
    mmSeq -> Lines -> LoadFromFile(FilePath) ;

    FilePath = FolderPath + "\\ManSetting.INI";

    int    iTemp ;
    double dTemp ;

    m_sCrntJobName = _sName  ;



    //Load Device.

    UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_1Pos   " , &dTemp); edAxis1_1Pos    -> Text = dTemp ;
    //UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_2Pos   " , &dTemp); edAxis1_2Pos    -> Text = dTemp ;
    //UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_3Pos   " , &dTemp); edAxis1_3Pos    -> Text = dTemp ;
    //UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_4Pos   " , &dTemp); edAxis1_4Pos    -> Text = dTemp ;
    //UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_5Pos   " , &dTemp); edAxis1_5Pos    -> Text = dTemp ;
    //UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_6Pos   " , &dTemp); edAxis1_6Pos    -> Text = dTemp ;

    UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_AccTime" , &dTemp); edAxis1_AccTime -> Text = dTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_NorTime" , &dTemp); edAxis1_NorTime -> Text = dTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_RPM    " , &dTemp); edAxis1_RPM     -> Text = dTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_Acc    " , &dTemp); edAxis1_Acc     -> Text = dTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_Vel    " , &dTemp); edAxis1_Vel     -> Text = dTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_Dist   " , &dTemp); edAxis1_Dist    -> Text = dTemp ;

    UserINI.Load(FilePath.c_str() , "Axis2" , "dAxis2_1Pos   " , &dTemp); edAxis2_1Pos    -> Text = dTemp ;
    //UserINI.Load(FilePath.c_str() , "Axis2" , "dAxis2_2Pos   " , &dTemp); edAxis2_2Pos    -> Text = dTemp ;
    //UserINI.Load(FilePath.c_str() , "Axis2" , "dAxis2_3Pos   " , &dTemp); edAxis2_3Pos    -> Text = dTemp ;
    //UserINI.Load(FilePath.c_str() , "Axis2" , "dAxis2_4Pos   " , &dTemp); edAxis2_4Pos    -> Text = dTemp ;
    //UserINI.Load(FilePath.c_str() , "Axis2" , "dAxis2_5Pos   " , &dTemp); edAxis2_5Pos    -> Text = dTemp ;
    //UserINI.Load(FilePath.c_str() , "Axis2" , "dAxis2_6Pos   " , &dTemp); edAxis2_6Pos    -> Text = dTemp ;

    UserINI.Load(FilePath.c_str() , "Axis2" , "dAxis2_Acc    " , &dTemp); edAxis2_Acc     -> Text = dTemp ;
    UserINI.Load(FilePath.c_str() , "Axis2" , "dAxis2_Vel    " , &dTemp); edAxis2_Vel     -> Text = dTemp ;
    UserINI.Load(FilePath.c_str() , "Axis2" , "dAxis2_Dist   " , &dTemp); edAxis2_Dist    -> Text = dTemp ;


    UserINI.Load(FilePath.c_str() , "Laser" , "iLaserTime " , &iTemp); edLaserTime  -> Text = iTemp ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btAddDeviceClick(TObject *Sender)
{
    AnsiString ToPath   = GetExeDir("JobFile\\" + Ed_Device    -> Text );
    AnsiString FromPath = GetExeDir("JobFile\\" + Ed_CurDevice -> Text );

    UserFile.CopyDir(FromPath , ToPath) ;

    UserFile.GridSearchDir(GetExeDir("JobFile") , sgDevice , 0 , true);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
}

//---------------------------------------------------------------------------
void __fastcall TFrmMain::GetSensorRead(TMessage msg)
{
    long pulse, enc;

    if( pPaix == NULL )
        return;
    switch( msg.WParam )
    {

	case PAIX_SENSORREAD:
        {
            InputChanged(NmcData.nmcBit);
        }
    break;
    }
}


//---------------------------------------------------------------------------
void __fastcall TFrmMain::BitBtn1Click(TObject *Sender)
{
    if(pGrabRun->Suspended)
    {
        Caption = String("HR-RView") + String("Live");
        Msg("Cam Live Start");
        pGrabRun->Resume();
    }
    else
    {
        Caption = String("HR-RView ") + String("Halt");
        Msg("Cam Live Stop");
        pGrabRun->Suspend();
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::PanelProc(Messages::TMessage & msg)
{
    OriginalProc( msg );

    if(msg.Msg == WM_PAINT)
    {
        pGrabRun->DisPlay();
    }
}

//---------------------------------------------------------------------------
void __fastcall TFrmMain::ReadLogic(void)
{
    if( pPaix == NULL ) return;

    pPaix->GetLogicAll(&NmcLogicData);
}

//---------------------------------------------------------------------------
void __fastcall TFrmMain::Button4Click(TObject *Sender)
{
    short nField0[2],nField1[2],nField2[2],nField3[2];
    if( pPaix->GetIpAddress(nField0,nField1,nField2,nField3) == true )
    {
        Edit_Field0->Text = nField0[0];
        Edit_Field1->Text = nField1[0];
        Edit_Field2->Text = nField2[0];
        Edit_Field3->Text = nField3[0];

        Edit_SecField0->Text = nField0[1];
        Edit_SecField1->Text = nField1[1];
        Edit_SecField2->Text = nField2[1];
        Edit_SecField3->Text = nField3[1];
    }
}
//---------------------------------------------------------------------------
bool __fastcall TFrmMain::InputChanged(NMCBIT NmcBit)
{
    static bool bPrenInput0 = false ;
    static bool bPrenInput1 = false ;

    static bool bPrenInput4 = false ;
    static bool bPrenInput5 = false ;



    //0번축. 로터.
    if( !NmcBit.nInput0 && bPrenInput0) {
        pPaix -> SetSpeedDist (Axis1,40,edAxis1_Acc->Text.ToIntDef(500), edAxis1_Vel->Text.ToIntDef(100),0 );    //StartSpeed , Acc , Speed , Jerk
        pPaix -> JogMove(Axis1,CW);
        //BtCheck1 =  true;
    }
    if( !NmcBit.nInput1 && bPrenInput1) {
        pPaix -> SetSpeedDist (Axis1,40,edAxis1_Acc->Text.ToIntDef(500), edAxis1_Vel->Text.ToIntDef(100),0 );    //StartSpeed , Acc , Speed , Jerk
        pPaix -> JogMove(Axis1,CCW);
        //BtCheck2 =  true;
    }
    if(NmcBit.nInput0 && !bPrenInput0) {
        pPaix->StopMotor(Axis1);
    }

    if(NmcBit.nInput1 && !bPrenInput1) {
        pPaix->StopMotor(Axis1);
    }


    if( !NmcBit.nInput4 && bPrenInput4) {
        pPaix -> SetSpeedDist (Axis2,40,edAxis2_Acc->Text.ToIntDef(50), edAxis2_Vel->Text.ToIntDef(5),0 );    //StartSpeed , Acc , Speed , Jerk
        pPaix->JogMove(Axis2,CCW);
        //BtCheck1 =  true;
    }
    if( !NmcBit.nInput5 && bPrenInput5) {
        pPaix -> SetSpeedDist (Axis2,40,edAxis2_Acc->Text.ToIntDef(50), edAxis2_Vel->Text.ToIntDef(5),0 );    //StartSpeed , Acc , Speed , Jerk
        pPaix->JogMove(Axis2,CW);
        //BtCheck2 =  true;
    }
    if(NmcBit.nInput4 && !bPrenInput4) {
        pPaix->StopMotor(Axis2);
    }

    if(NmcBit.nInput5 && !bPrenInput5) {
        pPaix->StopMotor(Axis2);
    }

    bPrenInput0 = NmcBit.nInput0;
    bPrenInput1 = NmcBit.nInput1;

    bPrenInput4 = NmcBit.nInput4;
    bPrenInput5 = NmcBit.nInput5;
    //
	if( m_nBit[0] != NmcBit.nInput0 )
    {
	    SBtn_Input00->Glyph->LoadFromResourceName((int)HInstance,NmcBit.nInput0?"GOFF":"GON");
        m_nBit[0] = NmcBit.nInput0;
    }
	if( m_nBit[1] != NmcBit.nInput1)
    {
	    SBtn_Input01->Glyph->LoadFromResourceName((int)HInstance,NmcBit.nInput1?"GOFF":"GON");
        m_nBit[1] = NmcBit.nInput1;
    }
	if( m_nBit[2] != NmcBit.nInput2)
    {
	    SBtn_Input02->Glyph->LoadFromResourceName((int)HInstance,NmcBit.nInput2?"GOFF":"GON");
        m_nBit[2] = NmcBit.nInput2;
    }
	if( m_nBit[3] != NmcBit.nInput3)
    {
    	SBtn_Input03->Glyph->LoadFromResourceName((int)HInstance,NmcBit.nInput3?"GOFF":"GON");
        m_nBit[3] = NmcBit.nInput3;
    }
	if( m_nBit[4] != NmcBit.nInput4)
    {
	    SBtn_Input04->Glyph->LoadFromResourceName((int)HInstance,NmcBit.nInput4?"GOFF":"GON");
        m_nBit[4] = NmcBit.nInput4;
    }
	if( m_nBit[5] != NmcBit.nInput5 )
    {
    	SBtn_Input05->Glyph->LoadFromResourceName((int)HInstance,NmcBit.nInput5?"GOFF":"GON");
        m_nBit[5] = NmcBit.nInput5;
    }
	if( m_nBit[6] != NmcBit.nInput6 )
    {
	    SBtn_Input06->Glyph->LoadFromResourceName((int)HInstance,NmcBit.nInput6?"GOFF":"GON");
        m_nBit[6] = NmcBit.nInput6;
    }
	if( m_nBit[7] != NmcBit.nInput7 )
    {
    	SBtn_Input07->Glyph->LoadFromResourceName((int)HInstance,NmcBit.nInput7?"GOFF":"GON");
        m_nBit[7] = NmcBit.nInput7;
    }

    return true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::Button14Click(TObject *Sender)
{
    pPaix->RelMove(Axis1,-Edit_XDist->Text.ToIntDef(0));
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::Button3Click(TObject *Sender)
{
    pPaix->RelMove(Axis1,Edit_XDist->Text.ToIntDef(0));
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::Button17Click(TObject *Sender)
{
    pPaix->RelMove(Axis2,-Edit_YDist->Text.ToIntDef(0));
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button11Click(TObject *Sender)
{
    pPaix->RelMove(Axis2,Edit_YDist->Text.ToIntDef(0));
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false;

         if(pSeq  -> IsRun()              )lbStat -> Caption = "RUN"  ;
    else if(pPaix -> Stat[Axis1].iHomeStep)lbStat -> Caption = "INIT" ;
    else if(pPaix -> Stat[Axis2].iHomeStep)lbStat -> Caption = "INIT" ;
    else                                   lbStat -> Caption = "STOP" ;


    Ed_CurDevice -> Text = m_sCrntJobName ;

    Label6 -> Caption = pPaix -> TotalAng ;

    ALed_Emer->Color = NmcData.nmcBit.nEmer?clRed:ONCOLOR;                               

    ALed_MLimit1->Color = NmcData.nmcBit.nNLimit0?clRed:ONCOLOR;
    ALed_MLimit2->Color = NmcData.nmcBit.nNLimit1?clRed:ONCOLOR;

    ALed_Near1->Color = NmcData.nmcBit.nNear0?clRed:ONCOLOR;
    ALed_Near2->Color = NmcData.nmcBit.nNear1?clRed:ONCOLOR;

    ALed_PLimit1->Color = NmcData.nmcBit.nPLimit0?clRed:ONCOLOR;
    ALed_PLimit2->Color = NmcData.nmcBit.nPLimit1?clRed:ONCOLOR;

    ALed_Busy1->Color = NmcData.nmcBit.nBusy0?clRed:ONCOLOR;
    ALed_Busy2->Color = NmcData.nmcBit.nBusy1?clRed:ONCOLOR;

    ALed_Z1->Color = NmcData.nmcBit.nEncZ0?clRed:ONCOLOR;
    ALed_Z2->Color = NmcData.nmcBit.nEncZ1?clRed:ONCOLOR;

    ALed_Inpo1->Color = NmcData.nmcBit.nInpo0?clRed:ONCOLOR;
    ALed_Inpo2->Color = NmcData.nmcBit.nInpo1?clRed:ONCOLOR;

    ALed_Alarm1->Color = NmcData.nmcBit.nAlarm0?clRed:ONCOLOR;
    ALed_Alarm2->Color = NmcData.nmcBit.nAlarm1?clRed:ONCOLOR;

    ALed_Home1->Color = NmcData.nmcBit.nHome0?clRed:ONCOLOR;
    ALed_Home2->Color = NmcData.nmcBit.nHome1?clRed:ONCOLOR;

    Panel_PulseX->Caption = NmcData.Cmd[0] *pPaix->GetLeed(0)/ pPaix->GetMotorRes(0);
    Panel_EncX  ->Caption = NmcData.Enc[0] *pPaix->GetLeed(0)/ pPaix->GetMotorRes(0);
    Panel_PulseY->Caption = NmcData.Cmd[1] *pPaix->GetLeed(1)/ pPaix->GetMotorRes(1);
    Panel_EncY  ->Caption = NmcData.Enc[1] *pPaix->GetLeed(1)/ pPaix->GetMotorRes(1);

    Panel_PulseX2->Caption = NmcData.Cmd[0] *pPaix->GetLeed(0)/ pPaix->GetMotorRes(0);
    Panel_EncX2  ->Caption = NmcData.Enc[0] *pPaix->GetLeed(0)/ pPaix->GetMotorRes(0);
    Panel_PulseY2->Caption = NmcData.Cmd[1] *pPaix->GetLeed(1)/ pPaix->GetMotorRes(1);
    Panel_EncY2  ->Caption = NmcData.Enc[1] *pPaix->GetLeed(1)/ pPaix->GetMotorRes(1);

    static bool bPreEmg = false ;
    if(NmcData.nmcBit.nEmer && !bPreEmg)
    {
        Msg("EMERGENCY STOP( CHECK EMERGENCY STOP )");
        pPaix -> Stat[0].bHomeDone = false ;
        pPaix -> Stat[1].bHomeDone = false ;

    }
    bPreEmg = NmcData.nmcBit.nEmer ;

    static bool bPre1HomeDone = false ;
    if (pPaix->Stat[0].bHomeDone && !bPre1HomeDone)
    {
        //Msg("Axis 1 Home End");
    }
    bPre1HomeDone = pPaix->Stat[0].bHomeDone ;

    static bool bPre2HomeDone = false ;
    if (pPaix->Stat[1].bHomeDone && !bPre2HomeDone)
    {
        //Msg("Axis 2 Home End");
    }
    bPre2HomeDone = pPaix->Stat[1].bHomeDone ;

    Label12 -> Caption = pPaix -> GetCmdPos(Axis1) ;


    static bool bPreStop = false ;
    if(!pPaix -> nmcData.nmcBit.nBusy0 && pPaix -> nmcData.nmcBit.nInpo0 && !bPreStop && m_bManualStart)
    {
        pGrabRun->StopGrab();
        Caption = String("HR-RView ") + String("Halt");
        Msg("Rotor Run End");
        pGrabRun -> MakeAviThread() ;
        pPaix -> SetServoOnOff   (Axis1,true); //Servo Off
        m_bManualStart = false ;
    }
    bPreStop = !pPaix -> nmcData.nmcBit.nBusy0 && pPaix -> nmcData.nmcBit.nInpo0 ;



    tmUpdate -> Enabled = true;
}

//---------------------------------------------------------------------------

void __fastcall TFrmMain::SBtn_MotionStopClick(TObject *Sender)
{
    int nMode;
    TSpeedButton *pButton;

    if( pPaix == NULL )
        return;
    pButton = dynamic_cast<TSpeedButton *>(Sender);
    if( CheckBox4->Checked )
        pPaix->SetSlowStop(pButton->Tag);
    else
        pPaix->StopMotor(pButton->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::SpeedButton5MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int nMode;
    TSpeedButton *pButton;

    if( pPaix == NULL )
        return;
    pButton = dynamic_cast<TSpeedButton *>(Sender);
    pPaix->JogMove(pButton->Tag,CCW);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::SpeedButton1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
//    if( m_bOpen == true ) TriggerSW();
    int nMode;
    TSpeedButton *pButton;

    if( pPaix == NULL )
        return;
    pButton = dynamic_cast<TSpeedButton *>(Sender);
    pPaix->JogMove(pButton->Tag,CW);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::SpeedButton5MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int nMode;
    TSpeedButton *pButton;

    if( pPaix == NULL )
        return;
    if( CheckBox3->Checked ) return;
    pButton = dynamic_cast<TSpeedButton *>(Sender);

    if( CheckBox4->Checked )
        pPaix->SetSlowStop(pButton->Tag);
    else
        pPaix->StopMotor(pButton->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::Button7Click(TObject *Sender)
{
    double fdist = StrToFloatDef(Edit_XDist->Text,0);

    if( pPaix == NULL )
        return;

    pPaix->AbsMove(0,fdist);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::Button20Click(TObject *Sender)
{
    double fdist = StrToFloatDef(Edit_XDist->Text,0);

    if( pPaix == NULL )
        return;

    pPaix->AbsMove(0,-fdist);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btAxis1RPMGoClick(TObject *Sender)
{
    pPaix-> MoveRPM(Axis1,edAxis1_RPM -> Text.ToIntDef(500) ,edAxis1_AccTime -> Text.ToIntDef(10000) , edAxis1_NorTime -> Text.ToIntDef(10000) );
//    pPaix-> MoveRPM(Axis1,6000 ,edAxis1_AccTime -> Text.ToIntDef(10000) , edAxis1_NorTime -> Text.ToIntDef(10000) );
//    pPaix->SetDriveSpeed(Axis1,edAxis1_RPM -> Text.ToIntDef(500));
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btAxis1DistGoClick(TObject *Sender)
{
    pPaix -> RotateReset(Axis1) ;

    pPaix -> SetSpeedDist (Axis1,40,edAxis1_Acc->Text.ToIntDef(500), edAxis1_Vel->Text.ToIntDef(100),0 );    //StartSpeed , Acc , Speed , Jerk

    double fdist = StrToFloatDef(edAxis1_Dist->Text,0);

    pPaix->AbsMove(Axis1,fdist);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::Panel_EncXClick(TObject *Sender)
{
    pPaix -> SetEnc(Axis1 , 0.0) ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::SBtn_Output00Click(TObject *Sender)
{
    if( pPaix == NULL )
        return;
    TSpeedButton *pButton;

    short nBit;
    pButton = dynamic_cast<TSpeedButton *>(Sender);

    if( pButton != NULL )
    {
        nBit = pButton->Tag;
		pPaix->GetOutput(&OutputIO);
        if( (OutputIO>>nBit)&0x01 )
            pPaix->SetOutputBit(nBit,0);
        else
            pPaix->SetOutputBit(nBit,1);

        pPaix->GetOutput(&OutputIO);
        OutputChanged(OutputIO);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::OutputChanged(BYTE Output)
{
    TSpeedButton *pButton;
    String str;
    UINT nBit;

    for( int i =0; i < 8; i ++)
    {
        str = "SBtn_Output0" +IntToStr(i);
        pButton = (TSpeedButton *)FindComponent( str );
        if( pButton != NULL )
        {
            nBit = (Output>>i)&0x01;
            if( nBit != OutputOld[i])
            {
	            pButton->Glyph->LoadFromResourceName((int)HInstance,nBit?"ROFF":"RON");
                FrmMain->Show();
            }
            OutputOld[i] = nBit;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btRotorRunClick(TObject *Sender)
{
    if(!pPaix -> Stat[0].bHomeDone || !pPaix -> Stat[1].bHomeDone)
    {
        Msg("ALL HOME 을 먼저 실행해 주세요.");
        return;
    }

    Msg("Rotor Run Start");

    pGrabRun->ContinuousShot();
    pPaix -> SetServoOnOff   (Axis1,false); //Servo On
    pPaix-> MoveRPM(Axis1,edAxis1_RPM -> Text.ToIntDef(500) ,edAxis1_AccTime -> Text.ToIntDef(10000) , edAxis1_NorTime -> Text.ToIntDef(10000) );
    //Axis1Stop 체크.
    m_bAxis1Stop = true;
    m_bManualStart = true ;

}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btStopClick(TObject *Sender)
{
    if( pPaix == NULL )
    {
        return;
    }

    if(pSeq -> IsRun()){
        pSeq -> ReqStop(); return ;
    }

    pPaix->SetSlowStop(0);
    pPaix->SetSlowStop(1);

    Msg("Running Stoped");
    btEnable(true);
    m_bStop = true;
//    if(m_bManualStart) {
//        pGrabRun -> StopGrab();
//        pGrabRun -> MakeAviThread() ;
//        //m_bManualStart = false ;
//    }

    pPaix -> SetServoOnOff(Axis1 , true) ; //servooff.

    Caption = String("HR-RView ") + String("Halt");
    //pGrabRun->Suspend();

}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btEnable(bool bEnable)
{
    btAllHome    ->Enabled = bEnable;
    btSeqRun     ->Enabled = bEnable;
    btAddDevice  ->Enabled = bEnable;
    btDelete     ->Enabled = bEnable;
    Bt_Load      ->Enabled = bEnable;

}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btAllHomeClick(TObject *Sender)
{
    if( pPaix == NULL )
    {
        return;
    }
    Msg("ALL Home Start");
    m_bStop = false;
    pPaix->MoveHome(0);
    pPaix->MoveHome(1);

}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::edAxis1_AccTimeChange(TObject *Sender)
{
//    TEdit *pTEdit;
//
//    pTEdit = dynamic_cast<TEdit *>(Sender);
//    int EditTag = pTEdit->Tag;
//
//    AnsiString sFilePath = ExtractFilePath(Application->ExeName) + "config.ini";
//    AnsiString Section = "Edit"+AnsiString(EditTag);
//    AnsiString EditValue = pTEdit->Text;
//    TIniFile* pif   = new TIniFile(sFilePath);
//    pif->WriteInteger("MortorSetting", Section, EditValue.ToDouble());
//
//    delete pif;
//
}

//---------------------------------------------------------------------------



void __fastcall TFrmMain::sgDeviceSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
    Ed_SelDevice -> Text = sgDevice->Cells[ACol][ARow].Delete(sgDevice->Cells[ACol][ARow].Pos(".INI"),4) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btDeleteClick(TObject *Sender)
{
    AnsiString Path = GetExeDir ("JobFile\\" + Ed_SelDevice -> Text);
    UserFile.ClearDir  (Path , 1);
    UserFile.GridSearchDir(GetExeDir("JobFile") , sgDevice , 0 , true);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::Bt_LoadClick(TObject *Sender)
{
    AnsiString sJobName = Ed_SelDevice -> Text ;

   // sJobName = sJobName.Delete(sJobName.Pos(".INI"),4);
    LoadJobFile(sJobName);
    UserFile.GridSearchDir(GetExeDir("JobFile") , sgDevice , 0 , true);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::HomeXClick(TObject *Sender)
{
//    if( m_bOpen == true) TriggerSW();
    int nMode;
    TButton *pButton;

    pButton = dynamic_cast<TButton *>(Sender);
    if( pPaix == NULL )
    {
        pButton = NULL;
        return;
    }

    //pPaix->HomeMove(pButton->Tag);

    pPaix->MoveHome(pButton->Tag) ;

    pButton = NULL;
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::bt_Lsave1Click(TObject *Sender)
{
    TButton *pButton;

    pButton = dynamic_cast<TButton *>(Sender);

    int Tag = pButton->Tag;
    pButton = NULL;
/*    switch ( Tag )
    {
        case 0:
        {
            Edit6->Text = Panel_PulseX->Caption;
            Edit7->Text = Panel_PulseY->Caption;

            WriteConfigDouble(CurrntDevice,"1thAxis1",StrToFloatDef(Panel_PulseX->Caption,0));
            WriteConfigDouble(CurrntDevice,"1thAxis2",StrToFloatDef(Panel_PulseY->Caption,0));
            break;
        }
        case 1:
        {
            Edit8->Text = Panel_PulseX->Caption;
            Edit9->Text = Panel_PulseY->Caption;
            WriteConfigDouble(CurrntDevice,"2thAxis1",StrToFloatDef(Panel_PulseX->Caption,0));
            WriteConfigDouble(CurrntDevice,"2thAxis2",StrToFloatDef(Panel_PulseY->Caption,0));
            break;
        }
        case 2:
        {
            Edit10->Text = Panel_PulseX->Caption;
            Edit11->Text = Panel_PulseY->Caption;
            WriteConfigDouble(CurrntDevice,"3thAxis1",StrToFloatDef(Panel_PulseX->Caption,0));
            WriteConfigDouble(CurrntDevice,"3thAxis2",StrToFloatDef(Panel_PulseY->Caption,0));
            break;
        }
        case 3:
        {
            Edit12->Text = Panel_PulseX->Caption;
            Edit13->Text = Panel_PulseY->Caption;
            WriteConfigDouble(CurrntDevice,"4thAxis1",StrToFloatDef(Panel_PulseX->Caption,0));
            WriteConfigDouble(CurrntDevice,"4thAxis2",StrToFloatDef(Panel_PulseY->Caption,0));
            break;
        }
        case 4:
        {
            Edit14->Text = Panel_PulseX->Caption;
            Edit15->Text = Panel_PulseY->Caption;
            WriteConfigDouble(CurrntDevice,"5thAxis1",StrToFloatDef(Panel_PulseX->Caption,0));
            WriteConfigDouble(CurrntDevice,"5thAxis2",StrToFloatDef(Panel_PulseY->Caption,0));
            break;
        }
        case 5:
        {
            Edit16->Text = Panel_PulseX->Caption;
            Edit17->Text = Panel_PulseY->Caption;
            WriteConfigDouble(CurrntDevice,"6thAxis1",StrToFloatDef(Panel_PulseX->Caption,0));
            WriteConfigDouble(CurrntDevice,"6thAxis2",StrToFloatDef(Panel_PulseY->Caption,0));
            break;
        }
        case 6:
        {
            WriteConfigDouble(CurrntDevice,"LaserTime",StrToFloatDef(Edit18->Text,0));
            break;
        }

    break;
    }
*/
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button12Click(TObject *Sender)
{
    double fdist = StrToFloatDef(Edit_YDist->Text,0);

    if( pPaix == NULL )
        return;

    pPaix->AbsMove(1,fdist);
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::btAxis2DistGoClick(TObject *Sender)
{
    pPaix -> SetSpeedDist (Axis2,40,edAxis2_Acc->Text.ToIntDef(50), edAxis2_Vel->Text.ToIntDef(5),0 );    //StartSpeed , Acc , Speed , Jerk

    double fdist = StrToFloatDef(edAxis2_Dist->Text,10);

    pPaix->AbsMove(Axis2,fdist);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::mmMsgDblClick(TObject *Sender)
{
    mmMsg->Clear();
}

//---------------------------------------------------------------------------







void __fastcall TFrmMain::btSaveClick(TObject *Sender)
{
    SaveJobFile(m_sCrntJobName);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btLaserPosClick(TObject *Sender)
{
    int iSel = ((TButton *) Sender) -> Tag ;
    double fdist ;

    TEdit * edAxis1_Pos = (TEdit *)FindComponent("edAxis1_"+AnsiString(iSel)+"Pos");
    TEdit * edAxis2_Pos = (TEdit *)FindComponent("edAxis2_"+AnsiString(iSel)+"Pos");

    if(!pPaix -> Stat[0].bHomeDone || !pPaix -> Stat[1].bHomeDone) {Msg("All Home 버튼을 클릭해 주세요."); return ; }
    if(pPaix -> nmcData.nmcBit.nBusy0 || pPaix -> nmcData.nmcBit.nBusy1) {Msg("장비 구동중입니다. 동작이 끝난후 다시시도 하세요."); return ; }


    pPaix -> RotateReset(Axis1) ;
    pPaix -> SetSpeedDist (Axis1,40,edAxis1_Acc->Text.ToIntDef(500), edAxis1_Vel->Text.ToIntDef(100),0 );    //StartSpeed , Acc , Speed , Jerk
    fdist = StrToFloatDef(edAxis1_Pos->Text,0);
    pPaix->AbsMove(Axis1,fdist);

    pPaix -> SetSpeedDist (Axis2,40,edAxis2_Acc->Text.ToIntDef(50), edAxis2_Vel->Text.ToIntDef(5),0 );    //StartSpeed , Acc , Speed , Jerk
    fdist = StrToFloatDef(edAxis2_Pos->Text,10);
    pPaix->AbsMove(Axis2,fdist);
}
//---------------------------------------------------------------------------




void __fastcall TFrmMain::btLaserOnClick(TObject *Sender)
{
    tmLaser -> Interval = StrToFloatDef(edLaserTime -> Text , 0) ;
    tmLaser -> Enabled = true ;

    pPaix->SetOutputBit(2,0);
}

//---------------------------------------------------------------------------
void __fastcall TFrmMain::tmLaserTimer(TObject *Sender)
{
    tmLaser->Enabled = false;
    pPaix->SetOutputBit(2,1);

}
//---------------------------------------------------------------------------



void __fastcall TFrmMain::btSeqExTestClick(TObject *Sender)
{
    bool bRet ;
    lbCompileEx -> Caption = "" ;
    bRet = pSeq -> CheckCode(mmSeqEx -> Lines ) ;
    lbCompileEx  -> Caption = pSeq -> GetErrName() ;

    Msg(pSeq -> GetErrName());

    pSeq -> DisplayFunc(StringGrid1) ;

}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::btSeqTestClick(TObject *Sender)
{
    bool bRet ;
    lbCompile -> Caption = "" ;
    bRet = pSeq -> CheckCode(mmSeq -> Lines ) ;

    lbCompile -> Caption = pSeq -> GetErrName() ;
    Msg(pSeq -> GetErrName());

    pSeq -> DisplayFunc(StringGrid1) ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btSeqRunClick(TObject *Sender)
{
    bool bRet ;

    if(pSeq -> IsRun()) {Msg("SEQUENCE 동작중 입니다."); return ; }

    lbCompileEx -> Caption = "" ;
    bRet = pSeq -> CheckCode(mmSeq  -> Lines ) ;
    lbCompile   -> Caption = pSeq -> GetErrName() ;
    pSeq -> DisplayFunc(StringGrid1) ;
    if(!bRet) {
        Msg(pSeq -> GetErrName());
        return ;
    }


    if(!pPaix -> Stat[0].bHomeDone || !pPaix -> Stat[1].bHomeDone)
    {
        Msg("ALL HOME 을 먼저 실행해 주세요.");
        return;
    }

    //m_sStartTime = MyDateTime.DateString() + FormatDateTime("(hh-nn-ss)",Now()) ;
    pSeq -> StartSeq() ;
    btEnable(false);
    m_bStop = false;
}
//---------------------------------------------------------------------------




void __fastcall TFrmMain::BitBtn4Click(TObject *Sender)
{
    String sDir =  "d:\\";

    HWND hwnd = NULL;
    LPCTSTR lpOperation = "open";
    LPCTSTR lpFile = NULL;
    LPCTSTR lpParameters = NULL;
    LPCTSTR lpDirectory = sDir.c_str();
    int nShowCmd = SW_SHOWDEFAULT;

    ShellExecute(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);

}
//---------------------------------------------------------------------------

                                                




