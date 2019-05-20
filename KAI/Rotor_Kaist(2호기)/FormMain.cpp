//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMain.h"
#include "UserFile.h"
#include "UserIni.h"
#include "SEQUnit.h"
//#include "FormMsgYesNo.h"
#include "Crc16Unit.h"
#include "math.h"
#include "PaixMotion.h"
#include "uRs232c.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#define ONCOLOR (TColor)0x00DDFBFF
TFrmMain *FrmMain;
TRS232C  *RS232C_H1ch ;

//#define HEATER_CNT 3
#define HEATER_CNT 1
#define HEATER_DATA_CNT 8

#define ROUNDING(x, dig)    ( floor((x) * pow(float(10), dig) + 0.5f) ? pow(float(10), dig) )



//---------------------------------------------------------------------------

bool TFrmMain::LoadSaveTemp(bool _bLoad)
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sPath   ;
    AnsiString sTemp   ;

    //Save Device.
    sPath = ExtractFilePath(Application -> ExeName) + "Util\\Temp.ini" ;

    if(_bLoad){
        for(int c = 0 ; c < HEATER_CNT ; c++) {
            for(int r = 0 ; r < HEATER_DATA_CNT ; r++) {

                UserINI.Load(sPath, (AnsiString)c , (AnsiString)r , sTemp );
                sgTemp -> Cells[c+1][r+1] = sTemp ;
            }
        }

        UserINI.Load(sPath, "TempCal" , edHeat1a->Name , sTemp ); edHeat1a->Text = sTemp ;
        UserINI.Load(sPath, "TempCal" , edHeat2a->Name , sTemp ); edHeat2a->Text = sTemp ;
        UserINI.Load(sPath, "TempCal" , edHeat3a->Name , sTemp ); edHeat3a->Text = sTemp ;

        UserINI.Load(sPath, "TempCal" , edHeat1b->Name , sTemp ); edHeat1b->Text = sTemp ;
        UserINI.Load(sPath, "TempCal" , edHeat2b->Name , sTemp ); edHeat2b->Text = sTemp ;
        UserINI.Load(sPath, "TempCal" , edHeat3b->Name , sTemp ); edHeat3b->Text = sTemp ;
    }
    else {
        for(int c = 0 ; c < HEATER_CNT ; c++) {
            for(int r = 0 ; r < HEATER_DATA_CNT ; r++) {
                UserINI.Save(sPath, (AnsiString)c , (AnsiString)r , sgTemp -> Cells[c+1][r+1] );
            }
        }

        UserINI.Save(sPath, "TempCal" , edHeat1a->Name , edHeat1a->Text );
        UserINI.Save(sPath, "TempCal" , edHeat2a->Name , edHeat2a->Text );
        UserINI.Save(sPath, "TempCal" , edHeat3a->Name , edHeat3a->Text );

        UserINI.Save(sPath, "TempCal" , edHeat1b->Name , edHeat1b->Text );  
        UserINI.Save(sPath, "TempCal" , edHeat2b->Name , edHeat2b->Text );  
        UserINI.Save(sPath, "TempCal" , edHeat3b->Name , edHeat3b->Text );
    }
}

double TFrmMain::get_IP(double* x, double *y, int size)

{
  int i;

  double ip = 0.0;

  for (i = 0; i < size; i++, x++, y++)

    ip += (*x * *y);

  return ip;

}

// discriminant of a vector

double TFrmMain::get_Dsc(double* x, int size)
{
  int i;

  double sum1 = 0.0, sum2 = 0.0;

  for (i = 0; i < size; i++, x++)
  {

    sum1 += *x;
    sum2 += *x * *x;
  }

  sum1 *= sum1;

  return size * sum2 - sum1;
}

// calculate slope using mean-square method

double TFrmMain::get_Slope(double* x, double* y, int size, double ip, double D)

{

  int i;

  double sumx = 0.0, sumy = 0.0;

  for (i = 0; i < size; i++, x++, y++)

  {

    sumx += *x;

    sumy += *y;

  }

  ip *= size;

  return (ip - sumx * sumy) / D;

}

// calculate intercept using mean-square

double TFrmMain::get_Interc(double *x, double *y, int size, double ip, double D)

{

  int i;

  double sumx = 0.0, sumy = 0.0, sumx2 = 0.0;

  for (i = 0; i < size; i++, x++, y++)

  {

    sumx += *x;

    sumy += *y;

    sumx2 += *x * *x;

  }

  return (sumx2 * sumy - sumx * ip) / D;

}
//---------------------------------------------------------------------------
AnsiString OnGetVer(const AnsiString &sQuery)
{
/*
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
 */
   }
__fastcall TFrmMain::TFrmMain(TComponent* Owner)
        : TForm(Owner)
{
//    lbVer -> Caption = "Ver " + OnGetVer("FileVersion") ;
    int Age = FileAge(Application -> ExeName);
    AnsiString Date = FileDateToDateTime(Age).FormatString("''yyyy'년 'm'월 'd'일 'AM/PM' 'h'시 'n'분'");
    lbDate -> Caption = Date ;

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
    m_bAxis1Stop   = false ;
    m_bManualStart = false ;
    m_bCoolingOn   = false ;
    m_bCoolingOff  = false ;

    //Vision. 잠시수정.
    m_BarCam = new CBarsler();
    m_BarCam->Open(pnView -> Handle);

    //Motion
    pPaix = new CPaixMotion();
    pPaix -> SetMsgFunc(Msg) ;

    //Seq.
    pSeq = new CSEQ();
    pSeq -> SetMsgFunc(Msg) ;
    //pPaix->Resume();

    m_sCrntJobName = "NONE" ;

    m_iMsgCnt  = 0 ;

    bSetHeat1  = false ; iDataHeat1 = 0 ;
    bSetHeat2  = false ; iDataHeat2 = 0 ;
    bSetHeat3  = false ; iDataHeat3 = 0 ;

//    m_iMsgCnt1 = 0 ;
//    m_iMsgCnt2 = 0 ;
//    m_iMsgCnt3 = 0 ;
    m_bLiveCam = false ;

    m_bGoCheck = false ;


}

void __fastcall TFrmMain::FormCreate(TObject *Sender)
{
//    Caption = String("HR System");

    pnView->DoubleBuffered = true;
    OriginalProc = pnView -> WindowProc;
    pnView->WindowProc = PanelProc;

    //StringGrid를 Update한다.
    UserFile.GridSearchDir(GetExeDir("JobFile") , sgDevice , 0 , true);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
    Ed_SelDevice -> Text = sgDevice->Cells[1][sgDevice->Row] ;
    LoadJobFile(GetLastJobName());
    LoadCooling();

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

    //APC Rs-232
    RS232C_H1ch = new TRS232C ;

    if(!RS232C_H1ch->Open(0)) ShowMessage("RS-232C Heat 1 Ch Open Failed!");
    else                      RS232C_H1ch->CallBackReg(Heat1Ch);

//    if(!RS232C_H1ch->Open(0,Handle)) {ShowMessage("RS-232C Probe 1 Ch Open Failed!"); }
//    RS232C_Pb2ch = new TRS232C ;
//    if(!RS232C_Pb2ch->Open(1,Handle)) {ShowMessage("RS-232C Probe 2 Ch Open Failed!"); bSuccess = false;}


    Msg("All Home 버튼을 클릭해 주세요.");

    //Heat Cal.
    sgTemp -> RowCount = HEATER_DATA_CNT + 1 ;
    sgTemp -> ColCount = HEATER_CNT + 1 ;
//    sgTemp -> DefaultColWidth = 72 ;
    sgTemp -> Cells[0][0] = "세팅온도";
    sgTemp -> Cells[1][0] = "1번실측온도";
    sgTemp -> Cells[2][0] = "2번실측온도";
    sgTemp -> Cells[3][0] = "3번실측온도";
    sgTemp -> Cells[0][1] = "30";
    sgTemp -> Cells[0][2] = "40";
    sgTemp -> Cells[0][3] = "50";
    sgTemp -> Cells[0][4] = "60";
    sgTemp -> Cells[0][5] = "70";
    sgTemp -> Cells[0][6] = "80";
    sgTemp -> Cells[0][7] = "90";
    sgTemp -> Cells[0][8] = "100";

    LoadSaveTemp(true);

    m_bBtnCooling = false;
    m_bBtnHome    = false;
    m_bBtnStart   = false;
    m_bBtnStop    = false;

    bSaveImg = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false;
    tmHeat   -> Enabled = false;

    pnView->WindowProc = OriginalProc;


    //모션 관련
    if(pPaix) {
        delete pPaix;
        pPaix = NULL;
    }

    //카메라.
    if(m_BarCam) {
        m_BarCam -> Close();
        delete m_BarCam;
        m_BarCam = NULL;
    }

    if(pSeq) {
        delete pSeq ;
        pSeq = NULL ;
    }

    delete RS232C_H1ch ;

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
    bool   bTemp ;

    //Load Device.
    dTemp = StrToFloatDef(edAxis1_AccTime -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_AccTime" , dTemp);
    dTemp = StrToFloatDef(edAxis1_NorTime -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_NorTime" , dTemp);
    dTemp = StrToFloatDef(edAxis1_RPM     -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_RPM    " , dTemp);
    dTemp = StrToFloatDef(edAxis1_Acc     -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_Acc    " , dTemp);
    dTemp = StrToFloatDef(edAxis1_Vel     -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_Vel    " , dTemp);
    dTemp = StrToFloatDef(edAxis1_Dist    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_Dist   " , dTemp);

    //Cooling Zone
    dTemp = StrToFloatDef(edAxis1_AccC    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_AccC   " , dTemp);
    dTemp = StrToFloatDef(edAxis1_VelC    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_VelC   " , dTemp);
    dTemp = StrToFloatDef(edAxis1_DistC   -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis1" , "dAxis1_DistC  " , dTemp);
    iTemp = StrToIntDef  (edAxis1_DistC   -> Text,0  ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iAxis1_TimeC  " , iTemp);

    dTemp = StrToFloatDef(edAxis2_Acc     -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis2" , "dAxis2_Acc    " , dTemp);
    dTemp = StrToFloatDef(edAxis2_Vel     -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis2" , "dAxis2_Vel    " , dTemp);
    dTemp = StrToFloatDef(edAxis2_Dist    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis2" , "dAxis2_Dist   " , dTemp);

    iTemp = StrToInt     (edRTTime1       -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTTime1      " , iTemp);
    iTemp = StrToInt     (edRTTime2       -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTTime2      " , iTemp);
    iTemp = StrToInt     (edRTTime3       -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTTime3      " , iTemp);
    iTemp = StrToInt     (edRTTime4       -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTTime4      " , iTemp);
    iTemp = StrToInt     (edRTTime5       -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTTime5      " , iTemp);
    iTemp = StrToInt     (edRTTime6       -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTTime6      " , iTemp);
    iTemp = StrToInt     (edRTTime7       -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTTime7      " , iTemp);
    iTemp = StrToInt     (edRTTime8       -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTTime8      " , iTemp);
    iTemp = StrToInt     (edRTTime9       -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTTime9      " , iTemp);
    iTemp = StrToInt     (edRTTime10      -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTTime10     " , iTemp);

    iTemp = StrToInt     (edRTRPM1        -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTRPM1       " , iTemp);
    iTemp = StrToInt     (edRTRPM2        -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTRPM2       " , iTemp);
    iTemp = StrToInt     (edRTRPM3        -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTRPM3       " , iTemp);
    iTemp = StrToInt     (edRTRPM4        -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTRPM4       " , iTemp);
    iTemp = StrToInt     (edRTRPM5        -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTRPM5       " , iTemp);
    iTemp = StrToInt     (edRTRPM6        -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTRPM6       " , iTemp);
    iTemp = StrToInt     (edRTRPM7        -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTRPM7       " , iTemp);
    iTemp = StrToInt     (edRTRPM8        -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTRPM8       " , iTemp);
    iTemp = StrToInt     (edRTRPM9        -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTRPM9       " , iTemp);
    iTemp = StrToInt     (edRTRPM10       -> Text    ) ; UserINI.Save(FilePath.c_str() , "Axis1" , "iRTRPM10      " , iTemp);

    iTemp = StrToInt     (edShakeTime     -> Text    ) ; UserINI.Save(FilePath.c_str() , "Time"  , "iShakeTime    " , iTemp);
    iTemp = StrToInt     (edShakeRage     -> Text    ) ; UserINI.Save(FilePath.c_str() , "Range" , "iShakeRage    " , iTemp);
    iTemp = StrToInt     (edShakeAcc      -> Text    ) ; UserINI.Save(FilePath.c_str() , "Acc"   , "iShakeAcc     " , iTemp);
    iTemp = StrToInt     (edShakeSpeed    -> Text    ) ; UserINI.Save(FilePath.c_str() , "Speed" , "iShakeSpeed   " , iTemp);

    iTemp = StrToInt     (edPx            -> Text    ) ; UserINI.Save(FilePath.c_str() , "PointX", "iPx           " , iTemp);
    iTemp = StrToInt     (edPy            -> Text    ) ; UserINI.Save(FilePath.c_str() , "PointY", "iPy           " , iTemp);

    bTemp = pPaix -> m_bUseOVSave    ;                   UserINI.Save(FilePath.c_str() , "Option", "m_bUseOVSave   " , bTemp);
    bTemp =          m_bUsedCoverSEQ ;                   UserINI.Save(FilePath.c_str() , "Option", "m_bUsedCoverSEQ" , bTemp);


    iTemp = StrToInt     (edAngleCnt      -> Text    ) ; UserINI.Save(FilePath.c_str() , "AngleCnt", "iAngleCnt   " , iTemp);

    iTemp = StrToInt     (edAngDeleyTime  -> Text    ) ; UserINI.Save(FilePath.c_str() , "AngDeleyTime" , "iAngDeleyTime" , iTemp);
    iTemp = StrToInt     (edFirstAngle    -> Text    ) ; UserINI.Save(FilePath.c_str() , "FirstAngle  " , "iFirstAngle  " , iTemp);
    iTemp = StrToInt     (edAngle         -> Text    ) ; UserINI.Save(FilePath.c_str() , "Angle       " , "iAngle       " , iTemp);



    //dTemp = StrToFloatDef(edAngDeleyTime  -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis2" , "dAxis2Acc     " , dTemp);
    //dTemp = StrToFloatDef(edAxis2Vel      -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis2" , "dAxis2Vel     " , dTemp);
    //dTemp = StrToFloatDef(edAxis2Pos      -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis2" , "dAxis2Pos     " , dTemp);

//    dTemp = StrToIntDef  (edAxis2_Dist    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis2" , "dAxis2_Dist   " , dTemp);
//    dTemp = StrToIntDef  (edAxis2_Dist    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis2" , "dAxis2_Dist   " , dTemp);
//    dTemp = StrToIntDef  (edAxis2_Dist    -> Text,0.0) ; UserINI.Save(FilePath.c_str() , "Axis2" , "dAxis2_Dist   " , dTemp);
//    iTemp = StrToIntDef  (edLaserTime     -> Text,0  ) ; UserINI.Save(FilePath.c_str() , "Laser" , "iLaserTime    " , iTemp);
}
void TFrmMain::LoadCooling ()
{
    //Local Var.
    TUserINI   UserINI    ;
    AnsiString FilePath   ;
    AnsiString FolderPath ;
    AnsiString Name       ;

    //Make Dir.
    FolderPath = ExtractFilePath(Application -> ExeName)  + "Util" ;
    if (!DirectoryExists(FolderPath)) CreateDir(FolderPath);
    FilePath = FolderPath + "\\Cooling.SEQ" ;
    mmCooling -> Lines -> LoadFromFile(FilePath) ;
}
void TFrmMain::SaveCooling()
{
    //Local Var.
    TUserINI   UserINI    ;
    AnsiString FilePath   ;
    AnsiString FolderPath ;
    AnsiString Name       ;

    //Make Dir.
    FolderPath = ExtractFilePath(Application -> ExeName)  + "Util" ;
    if (!DirectoryExists(FolderPath)) CreateDir(FolderPath);
    FilePath = FolderPath + "\\Cooling.SEQ" ;
    mmCooling -> Lines -> SaveToFile(FilePath) ;
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
    bool   bTemp ;

    m_sCrntJobName = _sName  ;

    //Load Device.

    UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_AccTime" , &dTemp); edAxis1_AccTime -> Text = dTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_NorTime" , &dTemp); edAxis1_NorTime -> Text = dTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_RPM    " , &dTemp); edAxis1_RPM     -> Text = dTemp ;

    UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_Acc    " , &dTemp); edAxis1_Acc     -> Text = dTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_Vel    " , &dTemp); edAxis1_Vel     -> Text = dTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_Dist   " , &dTemp); edAxis1_Dist    -> Text = dTemp ;

//Cooling Zone
    UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_AccC   " , &dTemp); edAxis1_AccC    -> Text = dTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_VelC   " , &dTemp); edAxis1_VelC    -> Text = dTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "dAxis1_DistC  " , &dTemp); edAxis1_DistC   -> Text = dTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "iAxis1_TimeC  " , &iTemp); edAxis1_TimeC   -> Text = iTemp ;

    UserINI.Load(FilePath.c_str() , "Axis2" , "dAxis2_Acc    " , &dTemp); edAxis2_Acc     -> Text = dTemp ;
    UserINI.Load(FilePath.c_str() , "Axis2" , "dAxis2_Vel    " , &dTemp); edAxis2_Vel     -> Text = dTemp ;
    UserINI.Load(FilePath.c_str() , "Axis2" , "dAxis2_Dist   " , &dTemp); edAxis2_Dist    -> Text = dTemp ;

    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTTime1      " , &iTemp); edRTTime1       -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTTime2      " , &iTemp); edRTTime2       -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTTime3      " , &iTemp); edRTTime3       -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTTime4      " , &iTemp); edRTTime4       -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTTime5      " , &iTemp); edRTTime5       -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTTime6      " , &iTemp); edRTTime6       -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTTime7      " , &iTemp); edRTTime7       -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTTime8      " , &iTemp); edRTTime8       -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTTime9      " , &iTemp); edRTTime9       -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTTime10     " , &iTemp); edRTTime10      -> Text = iTemp ;

    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTRPM1       " , &iTemp); edRTRPM1        -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTRPM2       " , &iTemp); edRTRPM2        -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTRPM3       " , &iTemp); edRTRPM3        -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTRPM4       " , &iTemp); edRTRPM4        -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTRPM5       " , &iTemp); edRTRPM5        -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTRPM6       " , &iTemp); edRTRPM6        -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTRPM7       " , &iTemp); edRTRPM7        -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTRPM8       " , &iTemp); edRTRPM8        -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTRPM9       " , &iTemp); edRTRPM9        -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Axis1" , "iRTRPM10      " , &iTemp); edRTRPM10       -> Text = iTemp ;

    UserINI.Load(FilePath.c_str() , "Time"  , "iShakeTime    " , &iTemp); edShakeTime     -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Range" , "iShakeRage    " , &iTemp); edShakeRage     -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Acc"   , "iShakeAcc     " , &iTemp); edShakeAcc      -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Speed" , "iShakeSpeed   " , &iTemp); edShakeSpeed    -> Text = iTemp ;

    UserINI.Load(FilePath.c_str() , "PointX", "iPx           " , &iTemp); edPx            -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "PointY", "iPy           " , &iTemp); edPy            -> Text = iTemp ;

    UserINI.Load(FilePath.c_str() , "Option", "m_bUseOVSave   " , &bTemp); cbUseOVSave    -> Checked = bTemp ;
    UserINI.Load(FilePath.c_str() , "Option", "m_bUsedCoverSEQ" , &bTemp); cbUsedCoverSEQ -> Checked = bTemp ;

    UserINI.Load(FilePath.c_str() , "AngleCnt", "iAngleCnt   " , &iTemp); edAngleCnt      -> Text = iTemp ;

    UserINI.Load(FilePath.c_str() , "AngDeleyTime", "iAngDeleyTime   " , &iTemp); edAngDeleyTime -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "FirstAngle  ", "iFirstAngle     " , &iTemp); edFirstAngle   -> Text = iTemp ;
    UserINI.Load(FilePath.c_str() , "Angle       ", "iAngle          " , &iTemp); edAngle        -> Text = iTemp ;

    //    UserINI.Load(FilePath.c_str() , "Laser" , "iLaserTime " , &iTemp); edLaserTime  -> Text = iTemp ;
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

    if( pPaix == NULL ) return;
    switch( msg.WParam )
    {
	case PAIX_SENSORREAD:
        {
            InputChanged(NmcData.nmcBit);
            break;
        }

    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::PanelProc(Messages::TMessage & msg)
{
    OriginalProc( msg );

    if(msg.Msg == WM_PAINT)
    {
        OnPaint(); //잠시수정.
    }
}

void TFrmMain::OnPaint()
{
    //if(m_ViewHandle)PostMessage(m_ViewHandle , WM_PAINT , 0 , 0 );


    HDC hdc= GetDC(pnView->Handle);

    m_BarCam -> DisPlay(hdc);


    ReleaseDC(pnView->Handle ,hdc ) ;
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

    static bool bPrenInput0 = NmcBit.nInput0 ;
    static bool bPrenInput1 = NmcBit.nInput1 ;
    static bool bPrenInput2 = NmcBit.nInput2 ;
    static bool bPrenInput3 = NmcBit.nInput3 ;
    static bool bPrenInput4 = NmcBit.nInput4 ;
    static bool bPrenInput5 = NmcBit.nInput5 ;
    static bool bPrenInput6 = NmcBit.nInput6 ;
    static bool bPrenInput7 = NmcBit.nInput7 ;

    //up edge Action == push
    if(!bPrenInput0 && NmcBit.nInput0) {btSeqRun  -> Click(); pPaix -> SetOutputBit(0,true);}
    if(!bPrenInput1 && NmcBit.nInput1) {btStop    -> Click(); pPaix -> SetOutputBit(1,true);}
    if(!bPrenInput2 && NmcBit.nInput2) {btAllHome -> Click(); /*pPaix -> SetOutputBit(2,true);*/}
    if(!bPrenInput3 && NmcBit.nInput3) {btCooling -> Click(); /*pPaix -> SetOutputBit(3,true);*/}
    if(!bPrenInput4 && NmcBit.nInput4) {/*이머전시*/}
    if(!bPrenInput5 && NmcBit.nInput5) {pPaix -> SetOutputBit(2,false);}
    if(!bPrenInput6 && NmcBit.nInput6) {}
    if(!bPrenInput7 && NmcBit.nInput7) {}

    //Down edge Action == release
    if( bPrenInput0 && !NmcBit.nInput0) {pPaix -> SetOutputBit(0,false);}
    if( bPrenInput1 && !NmcBit.nInput1) {pPaix -> SetOutputBit(1,false);}
    if( bPrenInput2 && !NmcBit.nInput2) {}//pPaix -> SetOutputBit(2,false);}
    if( bPrenInput3 && !NmcBit.nInput3) {}//pPaix -> SetOutputBit(3,false);}
    if( bPrenInput4 && !NmcBit.nInput4) {}
    if( bPrenInput5 && !NmcBit.nInput5) {pPaix -> SetOutputBit(2,true);}
    if( bPrenInput6 && !NmcBit.nInput6) {}
    if( bPrenInput7 && !NmcBit.nInput7) {}

    if( bPrenInput0 != NmcBit.nInput0 ) SBtn_Input00->Glyph->LoadFromResourceName((int)HInstance,NmcBit.nInput0?"GOFF":"GON");
    if( bPrenInput1 != NmcBit.nInput1 ) SBtn_Input01->Glyph->LoadFromResourceName((int)HInstance,NmcBit.nInput1?"GOFF":"GON");
    if( bPrenInput2 != NmcBit.nInput2 ) SBtn_Input02->Glyph->LoadFromResourceName((int)HInstance,NmcBit.nInput2?"GOFF":"GON");
    if( bPrenInput3 != NmcBit.nInput3 ) SBtn_Input03->Glyph->LoadFromResourceName((int)HInstance,NmcBit.nInput3?"GOFF":"GON");
    if( bPrenInput4 != NmcBit.nInput4 ) SBtn_Input04->Glyph->LoadFromResourceName((int)HInstance,NmcBit.nInput4?"GOFF":"GON");
    if( bPrenInput5 != NmcBit.nInput5 ) SBtn_Input05->Glyph->LoadFromResourceName((int)HInstance,NmcBit.nInput5?"GOFF":"GON");
    if( bPrenInput6 != NmcBit.nInput6 ) SBtn_Input06->Glyph->LoadFromResourceName((int)HInstance,NmcBit.nInput6?"GOFF":"GON");
    if( bPrenInput7 != NmcBit.nInput7 ) SBtn_Input07->Glyph->LoadFromResourceName((int)HInstance,NmcBit.nInput7?"GOFF":"GON");

    bPrenInput0 = NmcBit.nInput0 ;
    bPrenInput1 = NmcBit.nInput1 ;
    bPrenInput2 = NmcBit.nInput2 ;
    bPrenInput3 = NmcBit.nInput3 ;
    bPrenInput4 = NmcBit.nInput4 ;
    bPrenInput5 = NmcBit.nInput5 ;
    bPrenInput6 = NmcBit.nInput6 ;
    bPrenInput7 = NmcBit.nInput7 ;

    return true ;


/*
    //0번축. 로터.
    if( !NmcBit.nInput0 && bPrenInput0) {
        //pPaix -> SetSpeedDist (Axis1,40,edAxis1_Acc->Text.ToIntDef(500), edAxis1_Vel->Text.ToIntDef(100),0 );    //StartSpeed , Acc , Speed , Jerk
        //pPaix -> JogMove(Axis1,CW);
        //BtCheck1 =  true;
    }
    if( !NmcBit.nInput1 && bPrenInput1) {
        //pPaix -> SetSpeedDist (Axis1,40,edAxis1_Acc->Text.ToIntDef(500), edAxis1_Vel->Text.ToIntDef(100),0 );    //StartSpeed , Acc , Speed , Jerk
        //pPaix -> JogMove(Axis1,CCW);
        //BtCheck2 =  true;
    }
    if(NmcBit.nInput0 && !bPrenInput0) {
        pPaix->StopMotor(Axis1);
    }

    if(NmcBit.nInput1 && !bPrenInput1) {
        pPaix->StopMotor(Axis1);
    }


    if( !NmcBit.nInput4 && bPrenInput4) {
        //pPaix -> SetSpeedDist (Axis2,40,edAxis2_Acc->Text.ToIntDef(50), edAxis2_Vel->Text.ToIntDef(5),0 );    //StartSpeed , Acc , Speed , Jerk
        //pPaix->JogMove(Axis2,CCW);
        //BtCheck1 =  true;
    }
    if( !NmcBit.nInput5 && bPrenInput5) {
        //pPaix -> SetSpeedDist (Axis2,40,edAxis2_Acc->Text.ToIntDef(50), edAxis2_Vel->Text.ToIntDef(5),0 );    //StartSpeed , Acc , Speed , Jerk
        //pPaix->JogMove(Axis2,CW);
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
    bPrenInput2 = NmcBit.nInput2;
    bPrenInput3 = NmcBit.nInput3;
    bPrenInput4 = NmcBit.nInput4;
    bPrenInput5 = NmcBit.nInput5;
    bPrenInput6 = NmcBit.nInput6;
    bPrenInput7 = NmcBit.nInput7;

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
*/
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::Button14Click(TObject *Sender)
{
    pPaix->RelMove(Axis1,-Edit_XDist->Text.ToIntDef(0) / MOTOR_CONVERSION);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::Button3Click(TObject *Sender)
{
    pPaix->RelMove(Axis1,Edit_XDist->Text.ToIntDef(0) / MOTOR_CONVERSION);
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

    bSaveImg = cbSaveImage -> Checked;
    if(bSaveImg && pPaix -> GetBusy(Axis1) && pSeq -> m_bSaveImg){
        tmSaveBmpAvi -> Enabled = true;
    }
    else if(!pPaix -> GetBusy(Axis1)) {
        tmSaveBmpAvi -> Enabled = false;
    }
    //else {
    //    tmSaveBmpAvi -> Enabled = false;
    //}

    Label29 -> Caption = (int)m_BarCam ->m_dSaveTime ;
    Label30 -> Caption = (int)m_BarCam ->m_dFramePerSec ;

         if(pSeq  -> IsRun()              )lbStat -> Caption = "RUN"  ;
    else if(pPaix -> Stat[Axis1].iHomeStep)lbStat -> Caption = "INIT" ;
    else if(pPaix -> Stat[Axis2].iHomeStep)lbStat -> Caption = "INIT" ;
    else                                   lbStat -> Caption = "STOP" ;

    lbAngleTime -> Caption = pPaix -> m_dMoveAng;
    lbDevice -> Caption = m_sCrntJobName ; //타이틀에 디바이스 이름 표시.

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

    ALed_Home1->Color = pPaix -> Stat[0].bHomeDone?clRed:ONCOLOR;//NmcData.nmcBit.nHome0?clRed:ONCOLOR;
    ALed_Home2->Color = pPaix -> Stat[0].bHomeDone?clRed:ONCOLOR;//NmcData.nmcBit.nHome1?clRed:ONCOLOR;

    Panel_Org1->Color = NmcData.nmcBit.nOrg0?clRed:ONCOLOR;
    Panel_Org2->Color = NmcData.nmcBit.nOrg1?clRed:ONCOLOR;


    Panel_PulseX->Caption = NmcData.Cmd[0] *pPaix->GetLeed(0)/ pPaix->GetMotorRes(0) * MOTOR_CONVERSION;
    Panel_EncX  ->Caption = NmcData.Enc[0] *pPaix->GetLeed(0)/ pPaix->GetMotorRes(0) * MOTOR_CONVERSION;
    Panel_PulseY->Caption = NmcData.Cmd[1] *pPaix->GetLeed(1)/ pPaix->GetMotorRes(1);
    Panel_EncY  ->Caption = NmcData.Enc[1] *pPaix->GetLeed(1)/ pPaix->GetMotorRes(1);

    Panel_PulseX2->Caption = NmcData.Cmd[0] *pPaix->GetLeed(0)/ pPaix->GetMotorRes(0) * MOTOR_CONVERSION;
    Panel_EncX2  ->Caption = NmcData.Enc[0] *pPaix->GetLeed(0)/ pPaix->GetMotorRes(0) * MOTOR_CONVERSION;
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

    Label12 -> Caption = pPaix -> GetCmdPos(Axis1) ;

    //button handling.
    static bool bPreRun = pSeq  -> IsRun();
    if(bPreRun != pSeq  -> IsRun()){
        btSeqEnable(!pSeq  -> IsRun());
    }
    bPreRun = pSeq -> IsRun() ;

    static bool bPreLive = m_bLiveCam ;
    if(bPreLive != m_bLiveCam){
        btSeqEnable(!m_bLiveCam);
    }
    bPreLive = m_bLiveCam ;


    //TimeCheck
    lbSeqTotalTime -> Caption  = pSeq->GetTotalTime ();
    lbSeqRemaing   -> Caption  = pSeq->GetRemainTime();
    lbSeqStartTime -> Caption  = pSeq->GetStartTime ();
    lbSeqEndTime   -> Caption  = pSeq->GetEndTime   ();
    double dTemp = pSeq->GetTotalTime () ;
    if(dTemp == 0 ) dTemp =1 ;
    double dTemp3 = pSeq->GetRemainTime() / dTemp * 100;

    pbSeqTime      -> Position = 100 - ( dTemp3 ) ;

    //
    Label31 -> Caption = m_BarCam -> m_iThreadCnt;

    pPaix->GetOutput(&OutputIO);
    OutputChanged(OutputIO);

    InputChanged(NmcData.nmcBit);

    POINT Point ;
    GetCursorPos(&Point);

    lbPx -> Caption = Point.x;
    lbPy -> Caption = Point.y;

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

    pPaix->AbsMove(0,(fdist/MOTOR_CONVERSION));
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::Button20Click(TObject *Sender)
{
    double fdist = StrToFloatDef(Edit_XDist->Text,0);

    if( pPaix == NULL )
        return;

    pPaix->AbsMove(0,-(fdist/MOTOR_CONVERSION));
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btAxis1RPMGoClick(TObject *Sender)
{
    pPaix -> RotateReset(Axis1) ;

//    pPaix -> SetSpeedDist (Axis1,1,edAxis1_AccTime->Text.ToIntDef(500), edAxis1_RPM->Text.ToIntDef(100),0 );    //StartSpeed , Acc , Speed , Jerk

    pPaix-> MoveRPM(Axis1,(edAxis1_RPM -> Text.ToIntDef(500) / 2) ,edAxis1_AccTime -> Text.ToIntDef(1000) , edAxis1_NorTime -> Text.ToIntDef(10000) );

    //20160204. 우선 수정 JS.

    m_BarCam -> SetStartTime() ;
    tmSaveBmpAvi -> Enabled = true;
//    double fdist = StrToFloatDef(edAxis1_Dist->Text,0);

//    pPaix->RelMove()(Axis1,fdist);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btAxis1DistGoClick(TObject *Sender)
{
    pPaix -> RotateReset(Axis1) ;

    pPaix -> SetSpeedDist (Axis1,1,edAxis1_Acc->Text.ToIntDef(500), edAxis1_Vel->Text.ToIntDef(100),0 );    //StartSpeed , Acc , Speed , Jerk

    double fdist = StrToFloatDef(edAxis1_Dist->Text.ToInt() / MOTOR_CONVERSION ,0);

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

//    m_BarCam->AcquisitionStart();
//    pPaix -> SetServoOnOff   (Axis1,false); //Servo On
    pPaix-> MoveRPM(Axis1,edAxis1_RPM -> Text.ToIntDef(500) ,edAxis1_AccTime -> Text.ToIntDef(10000) , edAxis1_NorTime -> Text.ToIntDef(10000) );
    //Axis1Stop 체크.
    m_bAxis1Stop = true;
    m_bManualStart = true ;

}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btStopClick(TObject *Sender)
{
    SeqStop();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btSeqEnable(bool bEnable)
{
    btAllHome    ->Enabled = bEnable;
    btSeqRun     ->Enabled = bEnable;
    btAddDevice  ->Enabled = bEnable;
    btDelete     ->Enabled = bEnable;
    Bt_Load      ->Enabled = bEnable;
    btLive       ->Enabled = bEnable;
    btCooling    ->Enabled = bEnable;
    btExit       ->Enabled = bEnable;
}

//---------------------------------------------------------------------------
void __fastcall TFrmMain::btLiveEnable(bool bEnable)
{
    btAllHome    ->Enabled = bEnable;
    btSeqRun     ->Enabled = bEnable;
    btAddDevice  ->Enabled = bEnable;
    btDelete     ->Enabled = bEnable;
    Bt_Load      ->Enabled = bEnable;
    btCooling    ->Enabled = bEnable;
    btExit       ->Enabled = bEnable;
}

//---------------------------------------------------------------------------
void __fastcall TFrmMain::sgDeviceSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
    Ed_SelDevice -> Text = sgDevice->Cells[1][ARow].Delete(sgDevice->Cells[ACol][ARow].Pos(".INI"),4) ;
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
    pPaix -> SetSpeedDist (Axis2, 1, edAxis2_Acc->Text.ToIntDef(50), edAxis2_Vel->Text.ToIntDef(5), 0);    //StartSpeed , Acc , Speed , Jerk

    double fdist = StrToFloatDef(edAxis2_Dist->Text,10);

    pPaix->AbsMove(Axis2, fdist);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::mmMsgDblClick(TObject *Sender)
{
    mmMsg->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btSaveClick(TObject *Sender)
{
    if(cbUseOVSave    -> Checked) { pPaix -> m_bUseOVSave    = true ; }
    else { pPaix -> m_bUseOVSave = false ; }

    if(cbUsedCoverSEQ -> Checked) { m_bUsedCoverSEQ = true ; }
    else { m_bUsedCoverSEQ = false ; }

    SaveJobFile(m_sCrntJobName);
    SaveCooling();
    LoadSaveTemp(false);
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
    if(FrmMain -> edPx -> Text == ""){
        ShowMessage("POINT SETTING X, Y값을 설정하여 주세요!");  return; }
    SeqStart();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btLiveClick(TObject *Sender)
{
    if(!m_BarCam->m_bLive){
        Msg("Cam Live Start");
        m_BarCam -> SetStartTime() ;
        m_BarCam -> OpenAvr();
        m_BarCam->m_bLive = true ;
        m_BarCam->TriggerSW(); //확실하게 되는지 모르겟음.
        m_BarCam -> Grab() ;
        tmSaveBmpAvi -> Enabled = true ;
    }
    else {
        Msg("Cam Live Stop");
        m_BarCam->m_bLive = false ;
        m_BarCam -> CloseAvr();

        m_BarCam->TriggerHW(); //확실하게 되는지 모르겟음.
        tmSaveBmpAvi -> Enabled = false ;
    }
    return ;

}
void __fastcall TFrmMain::tmLiveCamTimer(TObject *Sender)
{
    m_BarCam -> Grab() ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btExitClick(TObject *Sender)
{
   // if(MsgYesNo("CONFIRM","Are You Sure ?") == mrNo ) return ;

    if(pSeq -> IsRun()) {Msg("SEQUENCE 동작중 입니다."); return ; }

    FrmMain->Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btCoolingClick(TObject *Sender)
{
    SeqCooling();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// 쓰레드에 의해 Data가 수신되면 SendMessage에 의해 호출된다. 즉 수신버퍼에
// TMessage의 WParam의 길이만큼 Data가 수신된 후 호출된다
void TFrmMain::Heat1Ch(DWORD _cbInQue)
{
    int iLen;
    BYTE RcvBuff[300];
    AnsiString sRcvMsg = "" ;
    memset(&RcvBuff, 0 , sizeof(RcvBuff));

    iLen = RS232C_H1ch -> ReadData(_cbInQue, RcvBuff);

    sRcvMsg = (char*)RcvBuff ;

//    if(m_bRecive) {
    for(int i=0; i<iLen; i++)
    {
        sMsg[m_iMsgCnt] = sRcvMsg.sprintf("%02x", RcvBuff[i]);
//        Memo1 -> Lines->Add(sMsg[m_iMsgCnt]);
        m_iMsgCnt++;
        if(m_iMsgCnt>=7) {
            m_iData = HexToInt(sMsg[3]) + HexToInt(sMsg[4]);
            m_bComplite = true ;
//            m_iMsgCnt1  = 0 ;
            return ;
        }
    }
}
//---------------------------------------------------------------------------
//void __fastcall TFrmMain::Heat2Ch(TMessage &Message)
//{
//    BYTE RcvBuff[300];
//    static AnsiString sRcvMsg = "" ;
//
//    memset(&RcvBuff, 0 , sizeof(RcvBuff));
//    sRcvMsg += (char*)RcvBuff ;
//
//}
//void __fastcall TFrmMain::Heat3Ch(TMessage &Message)
//{
//    BYTE RcvBuff[300];
//    static AnsiString sRcvMsg = "" ;
//
//    memset(&RcvBuff, 0 , sizeof(RcvBuff));
//    sRcvMsg += (char*)RcvBuff ;
//}

void __fastcall TFrmMain::brSetHeatClick(TObject *Sender)
{
    SetHeat(1, edInpHeat1->Text.ToIntDef(0));
    //SetHeat(2, edInpHeat2->Text.ToIntDef(0));
    //SetHeat(3, edInpHeat3->Text.ToIntDef(0));
}
//---------------------------------------------------------------------------
bool __fastcall TFrmMain::CheckHeat(int _iId )
{
    if( _iId != 1 && _iId != 2 && _iId != 3 ) return false ;
    String sDataHi,sDataLo;
    int    iDataHi,iDataLo;

    String sHeat1 = edSetHeat1 ->Text.ToIntDef(0) ;
    sHeat1.sprintf("%04d",sHeat1.ToIntDef(0));

    int    iTemp  = sHeat1.ToIntDef(0);
    String sTemp  = IntToHex(iTemp,4);

    sDataHi = sTemp.SubString(1,2);
    sDataLo = sTemp.SubString(3,4);

    iDataHi = HexToInt(sDataHi);
    iDataLo = HexToInt(sDataLo);

//    m_StQuery.id            = _iId ;
//    m_StQuery.func          = 3    ;
//    m_StQuery.start_addr_hi = 0    ;
//    m_StQuery.start_addr_lo = 54   ;
//    m_StQuery.size_hi       = 0    ;
//    m_StQuery.size_lo       = 2    ;
//    m_StQuery.CRC16         = CRC16_0((unsigned char*)&m_StQuery, sizeof(TStQuery)-2);

    m_StQuery.id            = _iId ;
    m_StQuery.func          = 4    ;
    m_StQuery.start_addr_hi = 0x03 ; // End Text
    m_StQuery.start_addr_lo = 0xE8 ; //28
    m_StQuery.size_hi       = 0x00 ;
    m_StQuery.size_lo       = 0x01 ;
    m_StQuery.CRC16         = CRC16_0((unsigned char*)&m_StQuery, sizeof(TStQuery)-2);

//    juhyeon
    RS232C_H1ch -> SendData(sizeof(TStQuery),(BYTE*)&m_StQuery);

    return true ;
}
//---------------------------------------------------------------------------
int __fastcall TFrmMain::HexToInt(String &str)
{
   String temp, result;
   int index = 1;
   int imsi1 = 0;
   int imsi2 = 0;
   for(int Length=str.Length(); Length>0; Length--,index++)
   {
      temp = str.SubString(index,1);
      if(temp == "a" || temp == "A" ) temp =10;
      else if(temp == "b" || temp == "B" ) temp =11;
      else if(temp == "c" || temp == "C" ) temp =12;
      else if(temp == "d" || temp == "D" ) temp =13;
      else if(temp == "e" || temp == "E" ) temp =14;
      else if(temp == "f" || temp == "F" ) temp =15;
      else temp = temp.ToIntDef(0);

      if(Length != 1)
      {
          imsi1 = temp*(pow(16,(Length-1)));
      }
      else
      {
          imsi1 = temp*1;
      }
      imsi2 += imsi1;
  }
//  result  = IntToStr(imsi2);
  return imsi2;
}
//---------------------------------------------------------------------------
bool __fastcall TFrmMain::SetHeat     (int _iId, int _iData )
{
    if(_iId == 1 ) {
        bSetHeat1   = true   ;
        iDataHeat1  = GetCalTemp(_iId,_iData) ;
        edInpHeat1 -> Text = _iData     ;
        edSetHeat1 -> Text = iDataHeat1 ;
//        Step.iCycle = 10     ;
    }
    else if(_iId == 2 ) {
        bSetHeat2   = true   ;
        iDataHeat2  = GetCalTemp(_iId,_iData) ;
        edInpHeat2 -> Text = _iData     ;
        edSetHeat2 -> Text = iDataHeat2 ;
//        Step.iCycle = 10     ;
    }
    else if(_iId == 3 ) {
        bSetHeat3   = true   ;
        iDataHeat3  = GetCalTemp(_iId,_iData) ;
        edInpHeat3 -> Text = _iData     ;
        edSetHeat3 -> Text = iDataHeat3 ;
//        Step.iCycle = 10     ;
    }
    else return false ;

    return true ;
}
//---------------------------------------------------------------------------
bool __fastcall TFrmMain::SetHeatTimer(int _iId, int _iData )
{
    if(_iId != 1 && _iId != 2 && _iId != 3 ) return false;
    m_bRecive = false ;

    String sDataHi,sDataLo;
    int    iDataHi,iDataLo;

//    String sHeat1 = edSetHeat1 ->Text.ToIntDef(0) ;
//    sHeat1.sprintf("%04d",sHeat1.ToIntDef(0));

//    int    iTemp  = sHeat1.ToIntDef(0);
    int    iTemp  = _iData;
    String sTemp  = IntToHex(iTemp,4);

    sDataHi = sTemp.SubString(1,2);
    sDataLo = sTemp.SubString(3,4);

    iDataHi = HexToInt(sDataHi);
    iDataLo = HexToInt(sDataLo);

    m_StResponse.id            = 0x00 + _iId   ;
    m_StResponse.func          = 0x06      ;
    m_StResponse.start_addr_hi = 0x00      ;
    m_StResponse.start_addr_lo = 0x00      ;
    m_StResponse.size_hi       = 0x00 + iDataHi;
    m_StResponse.size_lo       = 0x00 + iDataLo;
    m_StResponse.CRC16         = CRC16_0((unsigned char*)&m_StResponse, sizeof(TStResponse)-2);

//    juhyeon
    RS232C_H1ch -> SendData(sizeof(TStResponse),(BYTE*)&m_StResponse);

    return true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::tmHeatTimer(TObject *Sender)
{
//    tmHeat->Enabled = false ;
    if (m_CycleTimer.OnDelay(Step.iCycle == PreStep.iCycle , 2000 )) {
        Msg("Heat Com Error");
        m_iMsgCnt  = 0     ;
//        m_bRecive   = false ;
//        bSetHeat1   = false;
//        bSetHeat2   = false;
//        bSetHeat3   = false;
        Step.iCycle = 10    ;
        tmHeat->Enabled = true ;
        return;
    }

    PreStep.iCycle = Step.iCycle ;

    switch (Step.iCycle)
    {
        default : Step.iCycle = 0;
                  break ;

        case  10: if(bSetHeat1) {
                      bSetHeat1   = false;
//                      m_bRecive   = false;
                      m_bComplite = false;
                      m_iMsgCnt  = 0 ;
                      SetHeatTimer(1,iDataHeat1);
                      Step.iCycle++;
                      break ;
                  }
                  else {
                      Step.iCycle = 14;
                      break ;
                  }

        case  11: if(!m_bComplite) return ;
                  if(bSetHeat2) {
                      bSetHeat2   = false;
//                      m_bRecive   = false;
                      m_bComplite = false;
                      m_iMsgCnt  = 0 ;
                      SetHeatTimer(2,iDataHeat2);
                      Step.iCycle++;
                      break ;
                  }
                  else {
                      Step.iCycle = 14;
                      break ;
                  }

        case  12: if(!m_bComplite) return ;
                  if(bSetHeat3) {
                      bSetHeat3   = false;
//                      m_bRecive   = false;
                      m_bComplite = false;
                      m_iMsgCnt  = 0 ;
                      SetHeatTimer(3,iDataHeat3);
                      Step.iCycle++;
                      break ;
                  }
                  else {
                      Step.iCycle = 14;
                      break ;
                  }

        case  13: if(!m_bComplite) return ;
                  Step.iCycle++;
                  break ;

        case  14: //m_bRecive = true ;
                  m_bComplite = false ;
                  m_iMsgCnt   = 0 ;
                  CheckHeat(1);
                  Step.iCycle++;
                  break ;

        case  15: if(!m_bComplite) return ;
//                  m_bRecive   = false ;
                  //m_bComplite = false ;
                  m_bComplite = true;
                  m_iMsgCnt  = 0 ;
                  edTemp1 -> Caption = m_iData ;
                  //CheckHeat(2);
                  Step.iCycle ++;
                  break ;

        case  16: if(!m_bComplite) return ;
//                  m_bRecive   = false ;
                  //m_bComplite = false ;
                  m_bComplite = true;
                  m_iMsgCnt  = 0 ;
                  //edTemp2 -> Caption = m_iData ;
                  //CheckHeat(3);
                  Step.iCycle ++;
                  break ;

        case  17: if(!m_bComplite) return ;
//                  m_bRecive   = false ;
                  //m_bComplite = false ;
                  m_bComplite = true;
                  m_iMsgCnt  = 0 ;
                  //edTemp3 -> Caption = m_iData ;
                  Step.iCycle = 10;
                  break ;
    }

//    tmHeat->Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormShow(TObject *Sender)
{
    tmUpdate -> Enabled = true;
    tmHeat   -> Enabled = true;
    Step.iCycle = 10 ;

    Left = 0 ;
    Top  = 0 ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btAllHomeClick(TObject *Sender)
{
    SeqHome();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btChkErrCoolingClick(TObject *Sender)
{
    bool bRet ;
    lbCompileC -> Caption = "" ;
    bRet = pSeq -> CheckCode(mmCooling -> Lines ) ;

    lbCompileC -> Caption = pSeq -> GetErrName() ;
    Msg(pSeq -> GetErrName());

    pSeq -> DisplayFunc(StringGrid1) ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btMotor1SvClick(TObject *Sender)
{
//    pPaix->SetServoOnOff(nAxis,0);
    pPaix->SetServoOnOff(Axis1 ,1);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btHeatCalClick(TObject *Sender)
{
    double dHeat1Ofs[HEATER_DATA_CNT];
    double dHeat2Ofs[HEATER_DATA_CNT];
    double dHeat3Ofs[HEATER_DATA_CNT];

    //y=ax+b
    double dHeat1a , dHeat1b ;
    double dHeat2a , dHeat2b ;
    double dHeat3a , dHeat3b ;

    for( int i = 0 ; i < HEATER_DATA_CNT ; i++) {
        dHeat1Ofs[i] = StrToFloatDef(sgTemp -> Cells[1][i+1].ToDouble() , 0) ;
        //dHeat2Ofs[i] = StrToFloatDef(sgTemp -> Cells[2][i+1].ToDouble() , 0) ;
        //dHeat3Ofs[i] = StrToFloatDef(sgTemp -> Cells[3][i+1].ToDouble() , 0) ;
    }

    double y[] = {
        30 ,
        40 ,
        50 ,
        60 ,
        70 ,
        80 ,
        90 ,
        100
    };

    double *px, *py;
    int i;

    double D, ipr ;

    //1번 히터.
    px = dHeat1Ofs; py = y;
    D = get_Dsc(px, HEATER_DATA_CNT);
    ipr = get_IP(px, py, HEATER_DATA_CNT);
    dHeat1a = get_Slope(px, py, HEATER_DATA_CNT, ipr, D);
    dHeat1b = get_Interc(px, py, HEATER_DATA_CNT, ipr, D);

    //px = dHeat2Ofs; py = y;
    //D = get_Dsc(px, HEATER_DATA_CNT);
    //ipr = get_IP(px, py, HEATER_DATA_CNT);
    //dHeat2a = get_Slope(px, py, HEATER_DATA_CNT, ipr, D);
    //dHeat2b = get_Interc(px, py, HEATER_DATA_CNT, ipr, D);

    //px = dHeat3Ofs; py = y;
    //D = get_Dsc(px, HEATER_DATA_CNT);
    //ipr = get_IP(px, py, HEATER_DATA_CNT);
    //dHeat3a = get_Slope(px, py, HEATER_DATA_CNT, ipr, D);
    //dHeat3b = get_Interc(px, py, HEATER_DATA_CNT, ipr, D);


    edHeat1a->Text = dHeat1a ;
    edHeat1b->Text = dHeat1b ;

    edHeat2a->Text = dHeat2a ;
    edHeat2b->Text = dHeat2b ;

    edHeat3a->Text = dHeat3a ;
    edHeat3b->Text = dHeat3b ;

}
//---------------------------------------------------------------------------

double TFrmMain::GetCalTemp(int _iHeaterNo , double _dx)
{
    double a ;
    double b ;
    double ret ;

    if(_iHeaterNo == 1) {
         a = StrToFloatDef(edHeat1a->Text , 1  );
         b = StrToFloatDef(edHeat1b->Text , 0.0);
    }

    else if(_iHeaterNo == 2) {
         a = StrToFloatDef(edHeat2a->Text , 1  );
         b = StrToFloatDef(edHeat2b->Text , 0.0);
    }

    else if(_iHeaterNo ==3) {
         a = StrToFloatDef(edHeat3a->Text , 1  );
         b = StrToFloatDef(edHeat3b->Text , 0.0);
    }

    else return _dx ;

    ret = _dx * a + b ;

    return ret ;
}
void __fastcall TFrmMain::BitBtn1Click(TObject *Sender)
{
    pPaix->SetServoOnOff(Axis1 ,0);
//    pPaix->SetServoOnOff(Axis1 ,1);
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::Button2Click(TObject *Sender)
{
    pSeq -> StartSeq() ;

    m_bStop = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button5Click(TObject *Sender)
{
    m_BarCam -> TriggerSW();
    m_BarCam -> Grab();
    m_BarCam -> TriggerHW();
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::Button6Click(TObject *Sender)
{
    m_BarCam -> SaveImg();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button8Click(TObject *Sender)
{
   m_BarCam -> MakeAvi();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button9Click(TObject *Sender)
{
    if(!OpenPictureDialog1 -> Execute())return ;


    Graphics::TBitmap * pBmpAvi = new Graphics::TBitmap();

    pBmpAvi->LoadFromFile(OpenPictureDialog1 -> FileName);

    unsigned char* pixel_buffer ;
    for(int y = 0 ; y < IMAGE_HEIGHT ; y++){
        pixel_buffer=(unsigned char *)pBmpAvi ->ScanLine[y];
        memcpy(m_BarCam->m_pImgBuff + y * IMAGE_WIDTH * PIXEL_BIT/8 , pixel_buffer , IMAGE_WIDTH * PIXEL_BIT/8);
    }


    delete pBmpAvi ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button10Click(TObject *Sender)
{
    m_BarCam -> OpenAvr();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button13Click(TObject *Sender)
{
    m_BarCam -> CloseAvr();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button15Click(TObject *Sender)
{
    //m_BarCam -> SaveImg();
    //CRunThread thread(&m_BarCam -> SaveImg);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::tmSaveBmpAviTimer(TObject *Sender)
{
    // tmSaveBmpAvi -> Enabled = false;
    static double dPreTime = GetTime() ;
    int iFrameTime = GetTime() - dPreTime ;
    Label32 -> Caption = iFrameTime ;
    dPreTime = GetTime() ;
    m_BarCam -> SaveImg();
    //tmSaveBmpAvi -> Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btSEQ1CycleClick(TObject *Sender)
{
    m_bGoCheck = true ; // juhyeon

//    mmTemp -> Lines -> Clear();
    mmSeq1Cycle -> Lines -> Clear();
    mmTemp -> Lines = mmSeq -> Lines ;

    AnsiString sTemp, sTemp1;
    int iRPM  = edRTRPM1  -> Text.ToInt();
    int iTime = edRTTime1 -> Text.ToInt();

    //카메라 영상 획득 모드 를 종료.
    mmSeq1Cycle -> Lines -> Add("CAMERA(0);");

    //회전 모터의 홈을 잡는다.
    mmSeq1Cycle -> Lines -> Add("MOVEHOME(0);");

    //모터 호밍 종료를 확인한다.
    mmSeq1Cycle -> Lines -> Add("CHECKFINISH(0);");

    //회전 모터의 홈을 잡는다.
    mmSeq1Cycle -> Lines -> Add("MOVEHOME(1);");

    //모터 호밍 종료를 확인한다.
    mmSeq1Cycle -> Lines -> Add("CHECKFINISH(1);");

    mmSeq1Cycle -> Lines -> Add("CAMERA(1);");


    sTemp.printf("OR_SETSPEED0(10, %d,%d,%d,%d,%d,%d,%d,%d,%d,%d);" ,
                  edRTRPM1  -> Text.ToInt(),
                  edRTRPM2  -> Text.ToInt(),
                  edRTRPM3  -> Text.ToInt(),
                  edRTRPM4  -> Text.ToInt(),
                  edRTRPM5  -> Text.ToInt(),
                  edRTRPM6  -> Text.ToInt(),
                  edRTRPM7  -> Text.ToInt(),
                  edRTRPM8  -> Text.ToInt(),
                  edRTRPM9  -> Text.ToInt(),
                  edRTRPM10 -> Text.ToInt());

    mmSeq1Cycle -> Lines -> Add(sTemp);

    sTemp.printf("OR_SETTIME0(10, %d,%d,%d,%d,%d,%d,%d,%d,%d,%d);" ,
                  edRTTime1  -> Text.ToInt(),
                  edRTTime2  -> Text.ToInt(),
                  edRTTime3  -> Text.ToInt(),
                  edRTTime4  -> Text.ToInt(),
                  edRTTime5  -> Text.ToInt(),
                  edRTTime6  -> Text.ToInt(),
                  edRTTime7  -> Text.ToInt(),
                  edRTTime8  -> Text.ToInt(),
                  edRTTime9  -> Text.ToInt(),
                  edRTTime10 -> Text.ToInt());

    mmSeq1Cycle -> Lines -> Add(sTemp);

    mmSeq1Cycle -> Lines -> Add("OR_SETSTART0(100);");

    mmSeq1Cycle -> Lines -> Add("CHECKFINISH(0);");

    // 시퀀스 동작 후, 실린더 안 닫히게 하기 위해서.
//    mmSeq1Cycle -> Lines -> Add("MOVEACTR(0, 1);");
//    mmSeq1Cycle -> Lines -> Add("CHECKFINISH(0);");
//    mmSeq1Cycle -> Lines -> Add("MOVEACTR(1, 1);");
//    mmSeq1Cycle -> Lines -> Add("CHECKFINISH(1);");

    // juhyeon. 모터 x축과 cover 실린더 동작 체크일 때만 움직이게 만듬.
    if(m_bUsedCoverSEQ){
        mmSeq1Cycle -> Lines -> Add("MOVEABS(1, 50, 10, 189);");
        mmSeq1Cycle -> Lines -> Add("CHECKFINISH(1);");

        //실린더 움직인다.
        mmSeq1Cycle -> Lines -> Add("MOVEACTR(0, 0);");
        mmSeq1Cycle -> Lines -> Add("CHECKFINISH(1);");
        mmSeq1Cycle -> Lines -> Add("MOVEACTR(1, 0);");
        mmSeq1Cycle -> Lines -> Add("CHECKFINISH(1);");
    }

    for(int i = 0; i < edAngleCnt -> Text.ToInt(); i++){
        sTemp.printf("MOVEANG(0, %d, %d, %d);",
                     edAngDeleyTime -> Text.ToInt(),
                     edFirstAngle   -> Text.ToInt(),
                     edAngle        -> Text.ToInt());

        mmSeq1Cycle -> Lines -> Add(sTemp);
        mmSeq1Cycle -> Lines -> Add("CHECKFINISH(0);");
    }
    //모터 종료를 확인한다.

    //회전 모터의 홈을 잡는다.
    mmSeq1Cycle -> Lines -> Add("MOVEHOME(0);");
    mmSeq1Cycle -> Lines -> Add("CHECKFINISH(0);");
    mmSeq1Cycle -> Lines -> Add("MOVEHOME(1);");
    mmSeq1Cycle -> Lines -> Add("CHECKFINISH(1);");

    mmSeq1CycleEx -> Lines -> AddStrings(mmSeq -> Lines);

    mmSeq -> Lines -> Clear();
    mmSeq -> Lines -> AddStrings(mmSeq1Cycle -> Lines);

}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::Panel_PulseXDblClick(TObject *Sender)
{
    pPaix -> SetCmd(Axis1 , 0);
    pPaix -> SetEnc(Axis1 , 0);
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::Panel_PulseYDblClick(TObject *Sender)
{
    pPaix -> SetCmd(Axis2 , 0);
    pPaix -> SetEnc(Axis2 , 0);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::BitBtn2Click(TObject *Sender)
{
    pPaix->SetServoOnOff(Axis2 ,true);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::BitBtn3Click(TObject *Sender)
{
    pPaix->SetServoOnOff(Axis2 ,false);
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::tmSeqTimer(TObject *Sender)
{
    tmSeq -> Enabled = false;

    bool bStart   = (m_bPreBtnStart   != !NmcData.nmcBit.nInput0);
    bool bStop    = (m_bPreBtnStop    != !NmcData.nmcBit.nInput1);
    bool bHome    = (m_bPreBtnHome    != !NmcData.nmcBit.nInput2);
    bool bCooling = (m_bPreBtnCooling != !NmcData.nmcBit.nInput3);

    if(!pSeq->IsRun() &&
      (bStart || bStop || bHome || bCooling)){
        m_bBtnStart   = !NmcData.nmcBit.nInput0;
        m_bBtnStop    = !NmcData.nmcBit.nInput1;
        m_bBtnHome    = !NmcData.nmcBit.nInput2;
        m_bBtnCooling = !NmcData.nmcBit.nInput3;
    }

    if(m_bBtnStart  ){
        SeqStart();
        m_bBtnStart  = false;
    }
    if(m_bBtnStop   ){
        SeqStop();
        m_bBtnStop   = false;
    }
    if(m_bBtnHome   ){
        SeqHome();
        m_bBtnHome   = false;
    }
    if(m_bBtnCooling){
        SeqCooling();
        m_bBtnCooling = false;
    }

    m_bPreBtnStart   = !NmcData.nmcBit.nInput0;
    m_bPreBtnStop    = !NmcData.nmcBit.nInput1;
    m_bPreBtnHome    = !NmcData.nmcBit.nInput2;
    m_bPreBtnCooling = !NmcData.nmcBit.nInput3;

    tmSeq -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::SeqStart  ()
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
    if(m_BarCam->m_bLive || tmLiveCam -> Enabled) {
        Msg("LIVE 상태 입니다( LIVE 를 해제해 주세요 )");
        return ;
    }

    //m_sStartTime = MyDateTime.DateString() + FormatDateTime("(hh-nn-ss)",Now()) ;
//    m_BarCam->TriggerHW(); //확실하게 되는지 모르겟음.
    lbSeqStartTime -> Caption = "" ;
    lbSeqEndTime   -> Caption = "" ;
    pSeq -> StartSeq() ;

    m_BarCam -> SetStartTime() ;

    m_bStop = false;

}
void __fastcall TFrmMain::SeqStop()
{
    if( pPaix == NULL ) return;

    if(pSeq -> IsRun()){
        pSeq -> ReqStop(); return ;
    }

    pPaix->SetSlowStop(0);
    pPaix->SetSlowStop(1);

    pPaix->SetAlarmOnOff(Axis2, true);

    pPaix->SetAlarmOnOff(Axis2, false);

    pPaix->SetAlarmOnOff(Axis1, true);

    pPaix->SetAlarmOnOff(Axis1, false);

    //JH
    pPaix->ActrStat[0].bMoveDone  = false ;
    pPaix->ActrStat[1].bMoveDone  = false ;

    pPaix->Stat[0].bMoveAngle  = false ;
    pPaix->Stat[1].bMoveAngle  = false ;


    //pPaix->SetOutputBit(yETC_CoolingFan,1);

    Msg("Running Stoped");

    m_bStop = true;
}
void __fastcall TFrmMain::SeqHome()
{
    if( pPaix == NULL )
    {
        return;
    }
    Msg("ALL Home Start");
    m_bStop = false;
    //JH
    //pPaix->SetOutputBit(yETC_CoolingFan,1);
    pPaix->MoveHome(0);
    pPaix->MoveHome(1);

}
void __fastcall TFrmMain::SeqCooling()
{
    pPaix -> MoveActr(aiCoverUpDn  , true );
    pPaix -> MoveActr(aiHeaterDnUp , true );
}


void __fastcall TFrmMain::btSeqReturnClick(TObject *Sender)
{
    mmSeq -> Lines -> AddStrings(mmSeq1CycleEx -> Lines);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button16Click(TObject *Sender)
{
    pPaix -> RotateReset(Axis1);
    pPaix -> MoveShake(Axis1, edShakeTime -> Text.ToInt(), edShakeRage -> Text.ToInt() / MOTOR_CONVERSION, edShakeAcc -> Text.ToInt(), edShakeSpeed -> Text.ToInt());

}


//---------------------------------------------------------------------------


