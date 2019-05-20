//---------------------------------------------------------------------------
#pragma hdrstop

#include <stdio.h>

#include "SPCUnit.h"
#include "SLogUnit.h"
#include "UserIni.h"
#include "UtilDefine.h"
#include "UserFile.h"
#include "OptionMan.h"
#include "Timer.h"
#include "TempConUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define ERR_FOLDER "d:\\ErrLog\\"
#define LOG_FOLDER "d:\\LotLog\\"
#define SPC_FOLDER "d:\\Spc\\"

CSPC SPC;

__fastcall CSPC::CSPC(void)
{
}

__fastcall CSPC::~CSPC(void)
{
}

void CSPC::Init (void)
{
    ZeroMemory(iMapRslt , sizeof(int   ) * MAX_ARRAY_ROW * MAX_ARRAY_COL);
    ZeroMemory(dMapVf   , sizeof(double) * MAX_ARRAY_ROW * MAX_ARRAY_COL);
    ZeroMemory(dMapVr   , sizeof(double) * MAX_ARRAY_ROW * MAX_ARRAY_COL);
    iChipCnt   = 0     ;
    bUseZenner = false ;
}

void CSPC::Close(void)
{
}
bool __fastcall CSPC::WriteErrData(int iErrNo , bool bRelease)
{
    AnsiString sPath , sTemp , sErrData , sFPath;
    TDateTime  CurrDateTime = Now();;
    int hFile;
    sErrData = "";
    int hwnd, flen;
    char *pfbuf;
    pfbuf = NULL ;

    DelPastLog(ERR_FOLDER);

    if( iErrNo == (int)eiETC_LotEnd) return false;

    //�ֱٿ� �� �����Ͻ� �ðܳ�...
    int iLastErr = GetLastErr();
    if( iLastErr != -1 && iLastErr == iErrNo && !bRelease) return false;

    //Set Path.
    sPath        = ERR_FOLDER + CurrDateTime.CurrentDate().FormatString("yyyymmdd") ;//".csv";

    //Set File Path.
    sFPath = sPath + "\\ErrLog.csv";

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    DelPastLog(ERR_FOLDER);

    //Set Data.
    if(!bRelease) sErrData.sprintf("%03d,%s,%s\r\n", iErrNo , CurrDateTime.FormatString("(yy.mm.dd)hh:nn:ss") , "NONE");
    else {
//        AnsiString str = "";
        hwnd = FileOpen(sFPath.c_str(), fmOpenRead) ;
        if(hwnd == NULL) return false;

        flen = FileSeek(hwnd,0,2);
        FileSeek(hwnd,0,0);
        pfbuf = new char[flen+1];
        memset(pfbuf , 0 , sizeof(char)*(flen+1));
        FileRead(hwnd, pfbuf, flen);
        FileClose(hwnd);

        sErrData = pfbuf ;
        if(sErrData.SubString(sErrData.Length()-5 , 4) != "NONE" ) {
            //if(pfbuf){
            delete [] pfbuf;
            pfbuf = NULL;
            //}
            return false;
        }
        sErrData.Delete(sErrData.Length()-5 , 6 );

        sErrData = sErrData + CurrDateTime.FormatString("yy.mm.dd(hh:nn:ss)") + "\r\n";

        UserFile.ClearDir (sPath , true);
        UserFile.CreateDir(sPath.c_str());

    }
    //if(pfbuf){
        delete [] pfbuf;
        pfbuf = NULL ;
    //}

    hFile = FileOpen(sFPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sFPath.c_str());
        if (hFile == -1) { Trace("Err",(sFPath + "is Can't made").c_str());  return false ; }
    }

    FileSeek (hFile , 0                , SEEK_END         );
    FileWrite(hFile , sErrData.c_str() , sErrData.Length());

    //Close File.
    FileClose(hFile);

    //LastErr
    SaveLastErr(iErrNo);

    return true;
}
void __fastcall CSPC::SaveLastErr(int iErrNo)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath  = ERR_FOLDER    ;
    sPath += "LastErr.csv" ;

    UserINI.ClearFile(sPath) ;

    //Save.
    UserINI.Save(sPath.c_str() , "ErrNo" , "iLastErrNo " , iErrNo );

}
int __fastcall CSPC::GetLastErr()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    int iErrNo = -1;

    //Set Dir.
    sPath  = ERR_FOLDER    ;
    sPath += "LastErr.csv" ;

    //Load.
    UserINI.Load(sPath.c_str() , "ErrNo" , "iLastErrNo " , iErrNo );

    return iErrNo ;

}

void __fastcall CSPC::DispErrData(AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrHistory)
{
    AnsiString sPath ;
    AnsiString sFPath , sErrData = "";
    int iRowCnt , iColCnt ;
    int iR , iFCol = -1 , iLCol = -1 ;
    int hwnd, flen;
    char *pfbuf;
    pfbuf = NULL ;

    for(int i =0 ; i < _sgErrHistory->ColCount ; i++ ) {
        for(int j =1 ; j < _sgErrHistory->RowCount ; j++ ) {
            _sgErrHistory -> Cells[i][j] = "" ;
        }
    }

    m_pErrHistory = new TStringGrid(Application);

    sPath = ERR_FOLDER ;
    UserFile.GridSearchDir(sPath , m_pErrHistory , 0 , false);     // ���丮 �о�ͼ� ��¥�� ���ĺ� ������ ����

    iRowCnt = m_pErrHistory->RowCount ;

    AnsiString sSTemp;
    for(int i=0 ; i<iRowCnt ; i++){
        sSTemp = m_pErrHistory->Cells[1][i];
        if( m_pErrHistory->Cells[1][i] == sFDataTime ) iFCol = i ;
        if( m_pErrHistory->Cells[1][i] == sUDataTime ) iLCol = i ;
    }

    if(iFCol == -1    ) {
        for(int i=0 ; i<iRowCnt ; i++){
            if(m_pErrHistory->Cells[1][i].ToIntDef(0) > sFDataTime.ToIntDef(0) && m_pErrHistory->Cells[1][i].ToIntDef(0) <= sUDataTime.ToIntDef(0)) {
                iFCol = i ;
                break;
            }
        }
    }

    if(iLCol == -1    ) {
        for(int i=iRowCnt -1 ; i >= 1 ; i--){
            if(m_pErrHistory->Cells[1][i].ToIntDef(0) < sUDataTime.ToIntDef(0) && m_pErrHistory->Cells[1][i].ToIntDef(0) >= sFDataTime.ToIntDef(0)) {
                iLCol = i ;
                break;
            }
        }
    }

    if(iFCol == -1 || iLCol == -1 ) {
        delete m_pErrHistory;
        return ;
    }

    for(int i=iFCol ; i<=iLCol ; i++){
        sFPath= sPath + m_pErrHistory->Cells[1][i] + "\\ErrLog.csv";

        hwnd = FileOpen(sFPath.c_str(), fmOpenRead) ;
        if(hwnd == NULL) continue ;

        flen = FileSeek(hwnd,0,2);
        FileSeek(hwnd,0,0);

        pfbuf = new char[flen+1];

        memset(pfbuf , 0 , sizeof(char)*(flen+1));
        FileRead(hwnd, pfbuf, flen);
        FileClose(hwnd);

        sErrData = sErrData + pfbuf ;

        delete [] pfbuf;
        pfbuf = NULL;
    }

    AnsiString sTemp;

    int iCnt = 0;
    while(1) {
//        for(int i=0 ; i<300 ; i++){
            _sgErrHistory -> Cells[0][iCnt+1] = iCnt+1;

            _sgErrHistory -> Cells[1][iCnt+1] = sErrData.SubString( 1 , sErrData.Pos(",") - 1 );
            sErrData.Delete( 1 , sErrData.Pos(",") );

            _sgErrHistory -> Cells[2][iCnt+1] = EM_GetErrName(_sgErrHistory -> Cells[1][iCnt+1].ToIntDef(0));

            _sgErrHistory -> Cells[3][iCnt+1] = sErrData.SubString( 1 , sErrData.Pos(",") - 1  );
            sErrData.Delete( 1 , sErrData.Pos(",") );

            _sgErrHistory -> Cells[4][iCnt+1] = sErrData.SubString( 1 , sErrData.Pos("\r\n") - 1);
            sErrData.Delete( 1 , sErrData.Pos("\r\n") + 1);

            if(!sErrData.Pos("\r\n") || sErrData == "") {
                _sgErrHistory -> RowCount = iCnt + 2;
                break ;
            }
            iCnt++;

//        }
    }
    delete m_pErrHistory ;
    return ;

}

void __fastcall CSPC::DispErrCnt(AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrCnt)
{
    AnsiString sPath ;
    AnsiString sFPath , sErrData = "";
    int iRowCnt , iColCnt ;
    int iR , iFCol = -1 , iLCol = -1 ;
    int hwnd, flen = 100000;
    char *pfbuf;
    pfbuf = NULL ;

    for(int i =0 ; i < _sgErrCnt->ColCount ; i++ ) {
        for(int j =1 ; j < _sgErrCnt->RowCount ; j++ ) {
            _sgErrCnt -> Cells[i][j] = "" ;
        }
    }

    m_pErrCnt   = new TStringGrid(Application);
    m_pErrNoCnt = new TStringGrid(Application);

    sPath = ERR_FOLDER ;
    UserFile.GridSearchDir(sPath , m_pErrCnt , 0 , false);     // ���丮 �о�ͼ� ��¥�� ���ĺ� ������ ����

    iRowCnt = m_pErrCnt->RowCount ;

    AnsiString sSTemp;
    for(int i=0 ; i<iRowCnt ; i++){
        sSTemp = m_pErrCnt->Cells[1][i];
        if( m_pErrCnt->Cells[1][i] == sFDataTime ) iFCol = i ;
        if( m_pErrCnt->Cells[1][i] == sUDataTime ) iLCol = i ;
    }

    if(iFCol == -1    ) {
        for(int i=0 ; i<iRowCnt ; i++){
            if(m_pErrCnt->Cells[1][i].ToIntDef(0) > sFDataTime.ToIntDef(0) && m_pErrCnt->Cells[1][i].ToIntDef(0) <= sUDataTime.ToIntDef(0)) {
                iFCol = i ;
                break;
            }
        }
    }

    if(iLCol == -1    ) {
        for(int i=iRowCnt -1 ; i >= 1 ; i--){
            if(m_pErrCnt->Cells[1][i].ToIntDef(0) < sUDataTime.ToIntDef(0) && m_pErrCnt->Cells[1][i].ToIntDef(0) >= sFDataTime.ToIntDef(0)) {
                iLCol = i ;
                break;
            }
        }
    }

    if(iFCol == -1 || iLCol == -1 ) {
        delete m_pErrCnt   ;
        delete m_pErrNoCnt ;
        return ;
    }

    for(int i=iFCol ; i<=iLCol ; i++){
        sFPath= sPath + m_pErrCnt->Cells[1][i] + "\\ErrLog.csv";

        hwnd = FileOpen(sFPath.c_str(), fmOpenRead) ;
        if(hwnd == NULL) continue ;

        flen = FileSeek(hwnd,0,2);
        FileSeek(hwnd,0,0);

        pfbuf = new char[flen+1];

        memset(pfbuf , 0 , sizeof(char)*(flen+1));
        FileRead(hwnd, pfbuf, flen);
        FileClose(hwnd);

        sErrData = sErrData + pfbuf ;

        delete [] pfbuf;
        pfbuf = NULL;
    }

    AnsiString sTemp;

    int iCnt = 0;
    while(1) {
//        for(int i=0 ; i<300 ; i++){
            m_pErrNoCnt -> Cells[0][iCnt] = sErrData.SubString( 1 , sErrData.Pos(",") - 1 );
            sErrData.Delete( 1 , sErrData.Pos("\r\n") + 1) ;

            if(!sErrData.Pos("\r\n") || sErrData == "") {
                m_pErrNoCnt -> RowCount = iCnt + 2;
                break ;
            }
            iCnt++;
//        }
    }

    for(int i=0 ; i<m_pErrNoCnt -> RowCount ; i++){
        for(int j=0 ; j<m_pErrNoCnt -> RowCount ; j++){
            if( m_pErrNoCnt -> Cells[0][i] == "" ) continue ;
            if( m_pErrNoCnt -> Cells[0][i] == m_pErrNoCnt -> Cells[0][j] ) {
                m_pErrNoCnt -> Cells[1][i] = m_pErrNoCnt -> Cells[1][i].ToIntDef(0) + 1 ;
                if(i!=j) m_pErrNoCnt -> Cells[0][j] = "";
            }
        }
    }

    int iErrCnt = 0;
    for(int i=0 ; i<m_pErrNoCnt -> RowCount ; i++){
        if( m_pErrNoCnt -> Cells[0][i] == "" ) continue ;

        _sgErrCnt -> Cells[0][iErrCnt+1] = iErrCnt+1;
        _sgErrCnt -> Cells[1][iErrCnt+1] = m_pErrNoCnt -> Cells[0][i] ;
        _sgErrCnt -> Cells[2][iErrCnt+1] = EM_GetErrName(m_pErrNoCnt -> Cells[0][i].ToIntDef(0)/*_sgErrCnt -> Cells[1][i+1]*/);
        _sgErrCnt -> Cells[3][iErrCnt+1] = m_pErrNoCnt -> Cells[1][i] ;

        iErrCnt ++;
    }

    _sgErrCnt -> RowCount = iErrCnt + 1 ;

    delete m_pErrCnt   ;
    delete m_pErrNoCnt ;
    return;
}

void __fastcall CSPC::DispMtba(AnsiString sDataTime , TBarSeries *_S1)
{
    AnsiString sPath ;
    AnsiString sFPath , sErrData = "";
    AnsiString sTemp , sTemp1 , sTemp2;
    TDateTime  tTime ;

    int iRowCnt , iColCnt ;
    int iR , iFCol = -1 , iLCol = -1 ;
    int iTime ;
    int hwnd, flen;
    char *pfbuf;
    pfbuf = NULL;

    //Tchart �ʱ�ȭ.
    _S1 -> Clear();

    m_pErrCnt     = new TStringGrid(Application);
    m_pErrNoCnt   = new TStringGrid(Application);

    sPath = ERR_FOLDER ;

    UserFile.GridSearchDir(sPath , m_pErrCnt , 1 , false);     // ���丮 �о�ͼ� ��¥�� ���ĺ� ������ ����

    iRowCnt = m_pErrCnt->RowCount ;

    int iMtbaCnt = 0;
    for(int i=0 ; i<iRowCnt ; i++){
        if(m_pErrCnt->Cells[1][i].Pos(sDataTime)) {
            m_pErrCnt->Cells[1][iMtbaCnt] = m_pErrCnt->Cells[1][i] ;
            iMtbaCnt ++ ;
        }
        else m_pErrCnt->Cells[1][i] = "";
    }

    if(!m_pErrCnt->Cells[1][0].Pos(sDataTime)) {
        _S1 -> Clear();
        delete m_pErrCnt   ;
        delete m_pErrNoCnt ;
        return ;
    }

    m_pErrCnt   -> RowCount = iMtbaCnt ;
    m_pErrNoCnt -> RowCount = iMtbaCnt ;


    int iErrCnt = 0;
    for(int i=0 ; i < m_pErrCnt -> RowCount ; i++ ){
        sFPath= sPath + m_pErrCnt->Cells[1][i] + "\\ErrLog.csv";
        hwnd = FileOpen(sFPath.c_str(), fmOpenRead) ;
        if(hwnd == NULL) continue ;    //Hm.............
        flen = FileSeek(hwnd,0,2);
        FileSeek(hwnd,0,0);

        pfbuf = new char[flen+1];

        memset(pfbuf , 0 , sizeof(char)*(flen+1));
        FileRead(hwnd, pfbuf, flen);
        FileClose(hwnd);

        sErrData = pfbuf ;

        delete [] pfbuf;
        pfbuf = NULL;

        iErrCnt = 0;
        for (int i =0 ; i< 300 ; i++ ){
            if(sErrData.Pos("\r\n")) {
                sErrData.Delete(1,sErrData.Pos("\r\n") + 1);
                iErrCnt++;
            }
        }
        m_pErrCnt -> Cells[2][i] = iErrCnt;

    }

    bool bErrCnt;
    for(int i=1 ; i<32 ; i++){
        bErrCnt = false ;
        for(int j=1 ; j<32 ; j++) {
            sTemp = m_pErrCnt -> Cells[1][j-1].SubString(7 , 2);
            if(sTemp.ToIntDef(0) == i){
                sFPath = LOG_FOLDER + sDataTime + sTemp.sprintf("%02d",i) + ".csv";
                hwnd = FileOpen(sFPath.c_str(), fmOpenRead) ;
                if(hwnd == NULL) break ;    //Hm.............
                flen = FileSeek(hwnd,0,2);
                FileSeek(hwnd,0,0);

                pfbuf = new char[flen+1];

                memset(pfbuf , 0 , sizeof(char)*(flen+1));
                FileRead(hwnd, pfbuf, flen);
                FileClose(hwnd);
                sErrData = pfbuf ;
                sErrData.Delete(1,sErrData.Pos("\r\n")+1);
                sErrData.Delete(1,sErrData.Pos(","));
                sErrData = sErrData.SubString(1,sErrData.Pos(",")-1);
                if(sErrData == "") break ;
                tTime.Val = StrToTime(sErrData) ;

                iTime = tTime.FormatString("ss").ToIntDef(0) + tTime.FormatString("nn").ToIntDef(0)*60 + tTime.FormatString("hh").ToIntDef(0)*3600 ;

                _S1 -> AddXY(i , iTime / m_pErrCnt -> Cells[2][j-1].ToIntDef(1) , i , clGreen);
                bErrCnt = true ;

                delete [] pfbuf ;
                pfbuf = NULL;

                break;
            }
        }
        if(!bErrCnt) _S1 -> AddXY(i , 0 , i , clGreen);
    }
    delete m_pErrCnt   ;
    delete m_pErrNoCnt ;
    return ;
}

void __fastcall CSPC::SaveMtba(AnsiString sDataTime , TBarSeries *_S1)
{
//    str  = Series1 -> XValues -> Value[0] ;
//    str  = Series1 -> XValues -> Value[0] ;
    AnsiString sPath , sTemp , sData , sFPath;
    int hFile;
    int hwnd, flen;
//    char *pfbuf;

    TDateTime  CurrDateTime = Now();


    //Set Path.
    sTemp = "Mtba";
    sPath = SPC_FOLDER + sTemp ;

    //Set File Path.
    sFPath = sPath + "\\" + sDataTime + ".csv";

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    DelPastLog(SPC_FOLDER);

    //Set Data.
    sData = sDataTime + "," + CurrDateTime.CurrentDateTime().FormatString("yyyymmdd(hh:nn)") + "\r\n";
    for(int i=1; i<32; i++){
        sData += (AnsiString)i + "," ;
    }
    sData += "\r\n" ;

    sTemp = "";
    for(int i=0; i<_S1 -> XValues->Last(); i++){
        sTemp = _S1 -> YValues-> Value[i];
        sData += sTemp + ",";
    }
    sData += "\r\n" ;

    hFile = FileOpen(sFPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sFPath.c_str());
        if (hFile == -1) { Trace("Err",(sFPath + "is Can't made").c_str());  return ; }
    }

    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);

    return ;
}

void __fastcall CSPC::SaveErrCnt  (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrCnt)
{
    AnsiString sPath , sTemp , sData , sFPath;
    int hFile;
    int hwnd, flen;

    TDateTime  CurrDateTime = Now();

    //Set Path.
    sTemp = "ErrCount" ;
    sPath = SPC_FOLDER + sTemp;//".csv";

    //Set File Path.
    sFPath = sPath + "\\" + sFDataTime + "-" + sUDataTime + ".csv";

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    DelPastLog(SPC_FOLDER);

    //Set Data.
    sData = sFDataTime + "," + sUDataTime + "," + CurrDateTime.CurrentDateTime().FormatString("yyyymmdd(hh:nn)") + "\r\n";

    for(int i=0; i<_sgErrCnt->RowCount; i++){
        for(int j=0; j<_sgErrCnt->ColCount; j++){
            sData += _sgErrCnt -> Cells[j][i] + "," ;
        }
        sData += "\r\n" ;
    }

    hFile = FileOpen(sFPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sFPath.c_str());
        if (hFile == -1) { Trace("Err",(sFPath + "is Can't made").c_str());  return ; }
    }

    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);

    return ;
}

void __fastcall CSPC::SaveErrData (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrCnt)
{
    AnsiString sPath , sTemp , sData , sFPath;
    int hFile;
    int hwnd, flen;

    TDateTime  CurrDateTime = Now();

    //Set Path.
    sTemp = "ErrHistory" ;
    sPath = SPC_FOLDER + sTemp;//".csv";

    //Set File Path.
    sFPath = sPath + "\\" + sFDataTime + "-" + sUDataTime + ".csv";

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    DelPastLog(SPC_FOLDER);

    //Set Data.
    sData = sFDataTime + "," + sUDataTime + "," + CurrDateTime.CurrentDateTime().FormatString("yyyymmdd(hh:nn)") + "\r\n";

    for(int i=0; i<_sgErrCnt->RowCount; i++){
        for(int j=0; j<_sgErrCnt->ColCount; j++){
            sData += _sgErrCnt -> Cells[j][i] + "," ;
        }
        sData += "\r\n" ;
    }

    hFile = FileOpen(sFPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sFPath.c_str());
        if (hFile == -1) { Trace("Err",(sFPath + "is Can't made").c_str());  return ; }
    }

    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);

    return ;
}

void __fastcall CSPC::SaveOptionLog ( int iIndex , bool bInverse)
{
/*
    AnsiString sPath , sTemp , sData , sFPath;
    int hFile;
    int hwnd, flen;

    TDateTime  CurrDateTime = Now();

    //Set Path.
    sTemp = "OptionLog" ;
    sPath = SPC_FOLDER + sTemp;//".csv";

    //Set File Path.
    sFPath = sPath + "\\" + CurrDateTime.FormatString("yyyy-mm") + ".csv";

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    DelPastLog(SPC_FOLDER);

    sData = "";

    hFile = FileOpen(sFPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sFPath.c_str());
        if (hFile == -1) { Trace("Err",(sFPath + "is Can't made").c_str());  return ; }
        sData = "DATE,TIME,LOG \r\n";
    }

    //Set Data.
    sData += CurrDateTime.FormatString("yyyy-mm-dd")+ ",";
    sData += CurrDateTime.FormatString("hh:nn:ss")+ ",";

    if(bInverse) {
        switch(iIndex) {
            case 0 : if(OM.CmnOptn.bDryRun        ) sData += "Saved DryRun On"           ;
                     else                           sData += "Saved DryRun Off"          ;break ;
            case 1 : if(OM.CmnOptn.bIgnrDoor      ) sData += "Saved IgnrDoor On"         ;
                     else                           sData += "Saved IgnrDoor Off"        ;break ;
            case 2 : if(OM.CmnOptn.bIgnrIdCehck   ) sData += "Saved IgnrIdCehck On"      ;
                     else                           sData += "Saved IgnrIdCehck Off"     ;break ;
            case 3 : if(OM.CmnOptn.bIgnrInsp      ) sData += "Saved IgnrInsp On"         ;
                     else                           sData += "Saved IgnrInsp Off"        ;break ;
            case 4 : if(OM.CmnOptn.bIgnrHeatAlm   ) sData += "Saved IgnrHeatAlm On"      ;
                     else                           sData += "Saved IgnrHeatAlm Off"     ;break ;
            case 5 : if(OM.CmnOptn.bIgnrStrOverAlm) sData += "Saved IgnrStrOverAlm On"   ;
                     else                           sData += "Saved IgnrStrOverAlm Off"  ; break ;
            case 6 : if(OM.CmnOptn.bLoadingStop   ) sData += "Saved LoadingStop On"      ;
                     else                           sData += "Saved LoadingStop Off"     ; break ;
            case 7 : if(OM.CmnOptn.bCheckFailCnt  ) sData += "Saved CheckFailCnt On"     ;
                     else                           sData += "Saved CheckFailCnt Off"    ; break ;
            case 8 : if(OM.CmnOptn.bAutoConvesion ) sData += "Saved AutoConvesion On"    ;
                     else                           sData += "Saved AutoConvesion Off"   ; break ;
            case 9 : if(OM.CmnOptn.bAutoOperation ) sData += "Saved AutoOperation On"    ;
                     else                           sData += "Saved AutoOperation Off"   ; break ;
            case 10: if(OM.CmnOptn.bIgnrTrim      ) sData += "Saved IgnrTrim On"         ;
                     else                           sData += "Saved IgnrTrim Off"        ; break ;
            case 11: if(OM.CmnOptn.bIgnrVacuum    ) sData += "Saved IgnrVacuum On"       ;
                     else                           sData += "Saved IgnrVacuum Off"      ; break ;

            default : return ;
        }
    }
    else {
        switch(iIndex) {
            case 0 : if(OM.CmnOptn.bDryRun        ) sData += "Saved DryRun Off"          ;
                     else                           sData += "Saved DryRun On"           ; break ;
            case 1 : if(OM.CmnOptn.bIgnrDoor      ) sData += "Saved IgnrDoor Off"        ;
                     else                           sData += "Saved IgnrDoor On"         ; break ;
            case 2 : if(OM.CmnOptn.bIgnrIdCehck   ) sData += "Saved IgnrIdCehck Off"     ;
                     else                           sData += "Saved IgnrIdCehck On"      ; break ;
            case 3 : if(OM.CmnOptn.bIgnrInsp      ) sData += "Saved IgnrInsp Off"        ;
                     else                           sData += "Saved IgnrInsp On"         ; break ;
            case 4 : if(OM.CmnOptn.bIgnrHeatAlm   ) sData += "Saved IgnrHeatAlm Off"     ;
                     else                           sData += "Saved IgnrHeatAlm On"      ; break ;
            case 5 : if(OM.CmnOptn.bIgnrStrOverAlm) sData += "Saved IgnrStrOverAlm Off"  ;
                     else                           sData += "Saved IgnrStrOverAlm On"   ; break ;
            case 6 : if(OM.CmnOptn.bLoadingStop   ) sData += "Saved LoadingStop Off"     ;
                     else                           sData += "Saved LoadingStop On"      ; break ;
            case 7 : if(OM.CmnOptn.bCheckFailCnt  ) sData += "Saved CheckFailCnt Off"    ;
                     else                           sData += "Saved CheckFailCnt On"     ; break ;
            case 8 : if(OM.CmnOptn.bAutoConvesion ) sData += "Saved AutoConvesion Off"   ;
                     else                           sData += "Saved AutoConvesion On"    ; break ;
            case 9 : if(OM.CmnOptn.bAutoOperation ) sData += "Saved AutoOperation Off"   ;
                     else                           sData += "Saved AutoOperation On"    ; break ;
            case 10: if(OM.CmnOptn.bIgnrTrim      ) sData += "Saved IgnrTrim Off"        ;
                     else                           sData += "Saved IgnrTrim On"         ; break ;
            case 11: if(OM.CmnOptn.bIgnrVacuum    ) sData += "Saved IgnrVacuum Off"      ;
                     else                           sData += "Saved IgnrVacuum On"       ; break ;

            default : return ;
        }
    }

    sData += "\r\n";

    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);

    return ;
*/    

}
void __fastcall CSPC::SaveClearLog (AnsiString _sJobFile , AnsiString _sLotNo ,AnsiString _sSlotNo , AnsiString _sMsg)
{
    AnsiString sPath , sTemp , sData , sFPath;
    int hFile;
    int hwnd, flen;

    TDateTime  CurrDateTime = Now();

    //Set Path.
    sTemp = "ClearLog" ;
    sPath = SPC_FOLDER + sTemp;//".csv";

    //Set File Path.
    sFPath = sPath + "\\" + CurrDateTime.FormatString("yyyy-mm") + ".csv";

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    sData = "";

    hFile = FileOpen(sFPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sFPath.c_str());
        if (hFile == -1) { Trace("Err",(sFPath + "is Can't made").c_str());  return ; }
        sData = "DATE,TIME,JOBFILE,LOTNO,SLOTNO,LOG\r\n";
    }

    //AnsiString sLotNo ;
    //
    //     if(DM.ARAY[riVS2].GetLotNo() != "") sLotNo = DM.ARAY[riVS2].GetLotNo() ;
    //else if(DM.ARAY[riVS1].GetLotNo() != "") sLotNo = DM.ARAY[riVS2].GetLotNo() ;
    //else if(DM.ARAY[riVS2].GetLotNo() != "") sLotNo = DM.ARAY[riVS2].GetLotNo() ;


    //Set Data.
    sData += CurrDateTime.FormatString("yyyy-mm-dd")+ ",";
    sData += CurrDateTime.FormatString("hh:nn:ss")+ ",";
    sData += _sJobFile + ","    ;
    sData += _sLotNo   + ","    ;
    sData += _sSlotNo  + ","    ;
    sData += _sMsg     + "\r\n" ;

    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);

    return ;

}

void __fastcall CSPC::DelPastLog(AnsiString _sFolderPath)
{
    UserFile.ClearDirDate(LOG_FOLDER , Now() - 30 * 13); //�ϳ�¥�� �׷��������� 12������ ��.
}

//Lot Info Map StringGrid

bool __fastcall CSPC::WriteLotMap(EN_ARAY_ID riId )
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  , sArayName ;
    AnsiString sLotNo , sMgzNo , sSlotNo;
    AnsiString sTemp  ;
    AnsiString sTemp2 ;
    AnsiString sRslt  ;
    TDateTime  CurrDateTime;
    int        iMgzNo , iSlotNo ;
    double     dTemp  ;

    DelPastLog(LOG_FOLDER);

    //Set Path.

    if( DM.ARAY[riId].GetLotNo() == "" ) sLotNo = "NoLot";
    else                                 sLotNo = DM.ARAY[riId].GetLotNo();

    iSlotNo = DM.ARAY[riId].GetID().ToIntDef(9999)%100 ;
    iMgzNo  = DM.ARAY[riId].GetID().ToIntDef(9999)/100 ;

    sPath = LOG_FOLDER + CurrDateTime.CurrentDate().FormatString("yyyymmdd") + "\\" + sLotNo + "\\" + (String)iMgzNo ;

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    sPath   = sPath + "\\" + (String)iSlotNo + ".ini" ;

//
    for(int k =0 ; k < OM.DevOptn.iDgChipCnt ; k++ )
    {
        for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++)
        {
            sRslt = "" ;
            for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++)
            {
                sTemp2 = sTemp2.sprintf("%06.3f",dCChipVf[r][c][k]) + "_";
                sRslt += sTemp2 ;
            }
            sRslt.Delete(sRslt.Length(),1);
            sTemp.printf("VF%02d", r);
            sTemp2.printf("Chip%01d",k);

            UserINI.Save(sPath.c_str()  , sTemp2  , sTemp , sRslt );
        }

        for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++)
        {
            sRslt = "" ;
            for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++)
            {
                sTemp2 = sTemp2.sprintf("%06.3f",dCChipVr[r][c][k]) + "_";
                sRslt += sTemp2 ;
            }
            sRslt.Delete(sRslt.Length(),1);
            sTemp.printf("VR%02d", r);
            sTemp2.printf("Chip%01d",k);

            UserINI.Save(sPath.c_str()  , sTemp2  , sTemp , sRslt );
        }

        for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++)
        {
            sRslt = "" ;
            for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++)
            {
                     if(iCChipRslt[r][c][k] == prReady ) sTemp2.sprintf("%02d",(int)prReady ) ;
                else if(iCChipRslt[r][c][k] == prOk    ) sTemp2.sprintf("%02d",(int)prOk    ) ;
                else if(iCChipRslt[r][c][k] == prVFNg  ) sTemp2.sprintf("%02d",(int)prVFNg  ) ;
                else if(iCChipRslt[r][c][k] == prVRNg  ) sTemp2.sprintf("%02d",(int)prVRNg  ) ;
                else if(iCChipRslt[r][c][k] == prVFVRNg) sTemp2.sprintf("%02d",(int)prVFVRNg) ;
                sRslt += sTemp2 +"_";
            }
            sRslt.Delete(sRslt.Length(),1);
            sTemp.printf("VS%02d", r);
            sTemp2.printf("Chip%01d",k);

            UserINI.Save(sPath.c_str()  , sTemp2  , sTemp , sRslt );
        }
    }

    // juhyeon
    int iContactTemp;
    if(!OM.MstOptn.bUsedTempRS232) iContactTemp = TCU.GetCrntTemp(6);
    else                           iContactTemp = 0;

    UserINI.Save(sPath.c_str()  , "ETC"  , "ID"        , DM.ARAY[riId].GetID().ToIntDef(9999) );
    UserINI.Save(sPath.c_str()  , "ETC"  , "LotNo"     , DM.ARAY[riId].GetLotNo()             );
    UserINI.Save(sPath.c_str()  , "ETC"  , "Row"       , OM.DevInfo.iRowCnt                   );
    UserINI.Save(sPath.c_str()  , "ETC"  , "Col"       , OM.DevInfo.iColCnt                   );
    UserINI.Save(sPath.c_str()  , "ETC"  , "ChipCnt"   , OM.DevOptn.iDgChipCnt                );
    UserINI.Save(sPath.c_str()  , "ETC"  , "UseZenner" , OM.DevOptn.bUseZener                 );

    return true ;
/*
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  , sArayName ;
    AnsiString sLotNo , sMgzNo , sSlotNo;
    AnsiString sTemp  ;
    AnsiString sTemp2 ;
    AnsiString sRslt  ;
    TDateTime  CurrDateTime;
    int        iMgzNo , iSlotNo ;
    double     dTemp  ;

    DelPastLog(LOG_FOLDER);

    //Set Path.

    if( DM.ARAY[riId].GetLotNo() == "" ) sLotNo = "NoLot";
    else                                 sLotNo = DM.ARAY[riId].GetLotNo();

    iSlotNo = DM.ARAY[riId].GetID().ToIntDef(9999)%100 ;
    iMgzNo  = DM.ARAY[riId].GetID().ToIntDef(9999)/100 ;

    sPath = LOG_FOLDER + CurrDateTime.CurrentDate().FormatString("yyyymmdd") + "\\" + sLotNo + "\\" + (String)iMgzNo ;

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    sPath   = sPath + "\\" + (String)iSlotNo + ".ini" ;

//

    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        sRslt = "" ;
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            sTemp2 = sTemp2.sprintf("%06.3f",dCChipVf[r][c]) + "_";
//            if(sTemp2.Length() == 5) sTemp2 = "0" + sTemp2 ;
//                 if(sTemp2 < 1  ) sTemp2 = "0" + sTemp2 ;
//            if(sTemp2.ToDouble() < 10 ) sTemp2 = "0" + sTemp2 + "_";; ���� �Ǽ�.
            sRslt += sTemp2 ;
        }
        sTemp.printf("VF%02d", r);
//        dTemp = StrToFloatDef(sRslt,0);

        UserINI.Save(sPath.c_str()  , "Data"  , sTemp , sRslt );
    }

    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        sRslt = "" ;
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            sTemp2 = sTemp2.sprintf("%06.3f",dCChipVr[r][c]) + "_";
//            if(sTemp2.Length() == 5) sTemp2 = "0" + sTemp2 ;
//            if(sTemp2.ToDouble() < 10 ) sTemp2 = "0" + sTemp2 + "_";;
            sRslt += sTemp2 ;
        }
        sTemp.printf("VR%02d", r);
//        dTemp = StrToFloatDef(sRslt,0);
        UserINI.Save(sPath.c_str()  , "Data"  , sTemp , sRslt );
    }

    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        sRslt = "" ;
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            for(int k =0 ; k < OM.DevOptn.iDgChipCnt ; k++ ) {
                     if(iCChipRslt[r][c][k] == prReady ) sTemp2 = (int)prReady  ;
                else if(iCChipRslt[r][c][k] == prOk    ) sTemp2 = (int)prOk     ;
                else if(iCChipRslt[r][c][k] == prVFNg  ) sTemp2 = (int)prVFNg   ;
                else if(iCChipRslt[r][c][k] == prVRNg  ) sTemp2 = (int)prVRNg   ;
                else if(iCChipRslt[r][c][k] == prVFVRNg) sTemp2 = (int)prVFVRNg ;

//            sTemp2 = sTemp2.sprintf("%02d",sTemp2.ToIntDef(0)) + ",";
//            sRslt += sTemp2 ;
//            sTemp2 = sTemp2.sprintf("%2.3f",dCChipVf[r][c]) + "/";
//            sRslt += sTemp2 ;
//            sTemp2 = sTemp2.sprintf("%2.3f",dCChipVr[r][c]) + "_";
                sRslt += sTemp2 + "_";
            }
        }
        sTemp.printf("VS%02d", r);
        UserINI.Save(sPath.c_str()  , "Data"  , sTemp , sRslt );
    }

    UserINI.Save(sPath.c_str()  , "ETC"  , "ID"    , DM.ARAY[riId].GetID().ToIntDef(9999) );
    UserINI.Save(sPath.c_str()  , "ETC"  , "LotNo" , DM.ARAY[riId].GetLotNo()             );
    UserINI.Save(sPath.c_str()  , "ETC"  , "Row"   , OM.DevInfo.iRowCnt                   );
    UserINI.Save(sPath.c_str()  , "ETC"  , "Col"   , OM.DevInfo.iColCnt                   );

    return true ;
*/
}

bool __fastcall CSPC::ReadLotMap (AnsiString sPath , TStringGrid *_sgLotMap)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sTemp  ;
    AnsiString sVal   ;
    AnsiString sVF    ;
    AnsiString sVR    ;
    AnsiString sRslt  ;

    AnsiString sId    ;
    AnsiString sLotNo ;

    int iRow = 0 ;
    int iCol = 0 ;
    iChipCnt = 0 ;

    double dTemp ;

    TRect Rect ;

    AnsiString sTemp2= "" ;

    if(!FileExists(sPath)) return false ;

    UserINI.Load(sPath.c_str()  , "ETC"  , "ID"        , &sId                 );
    UserINI.Load(sPath.c_str()  , "ETC"  , "LotNo"     , &sLotNo              );
    UserINI.Load(sPath.c_str()  , "ETC"  , "Row"       , &iRow                );
    UserINI.Load(sPath.c_str()  , "ETC"  , "Col"       , &iCol                );
    UserINI.Load(sPath.c_str()  , "ETC"  , "ChipCnt"   , &iChipCnt            );
    UserINI.Load(sPath.c_str()  , "ETC"  , "UseZenner" , &bUseZenner          );

    if(!iRow || !iCol || !iChipCnt) return false ;

    _sgLotMap->RowCount = iRow + 1;
    _sgLotMap->ColCount = iCol + 1;

    for (int iCnt =0 ; iCnt <= iRow + 1 ; iCnt ++ ) _sgLotMap  -> Cells[0   ][iCnt] = iCnt;
    for (int iCnt =0 ; iCnt <= iCol + 1 ; iCnt ++ ) _sgLotMap  -> Cells[iCnt][0   ] = iCnt;

    for(int k =0 ; k < iChipCnt ; k++ )
    {
        for(int r = 0 ; r < iRow ; r++) {
            sTemp2.printf("Chip%01d",k);
            sTemp.printf("VF%02d", r);
            UserINI.Load(sPath.c_str()  , sTemp2  , sTemp , sRslt );
            for(int c = 0 ; c < iCol ; c++)
            {
                sVal = sRslt.SubString(1,6) ;
                sRslt.Delete(1,7) ;
                dTemp = StrToFloatDef(sVal,0);
                dMapVf[r][c][k] = dTemp;
            }
        }

        for(int r = 0 ; r < iRow ; r++)
        {
            sTemp2.printf("Chip%01d",k);
            sTemp.printf("VR%02d", r);
            UserINI.Load(sPath.c_str()  , sTemp2  , sTemp , sRslt );
            for(int c = 0 ; c < iCol ; c++)
            {
                sVal = sRslt.SubString(1,6) ;
                sRslt.Delete(1,7) ;
                dTemp = StrToFloatDef(sVal,0);
                dMapVr[r][c][k] = dTemp ;
            }
        }

        for(int r = 0 ; r < iRow ; r++)
        {
            sTemp2.printf("Chip%01d",k);
            sTemp.printf("VS%02d", r);
            UserINI.Load(sPath.c_str()  , sTemp2  , sTemp , sRslt );
            for(int c = 0 ; c < iCol ; c++)
            {
                sVal = sRslt.SubString(1,2) ;
                sRslt.Delete(1,3) ;

                iMapRslt[r][c][k] = sVal.ToIntDef(0);
            }
        }
    }


    return true ;
/*
    //Local Var.
    TUserINI   UserINI;
    AnsiString sTemp  ;
    AnsiString sVal   ;
    AnsiString sVF    ;
    AnsiString sVR    ;
    AnsiString sRslt  ;

    AnsiString sId    ;
    AnsiString sLotNo ;

    int iRow = 0 ;
    int iCol = 0 ;

    double dTemp ;

    TRect Rect ;

    AnsiString sTemp2= "" ;

    if(!FileExists(sPath)) return false ;

    UserINI.Load(sPath.c_str()  , "ETC"  , "ID"    , &sId                 );
    UserINI.Load(sPath.c_str()  , "ETC"  , "LotNo" , &sLotNo              );
    UserINI.Load(sPath.c_str()  , "ETC"  , "Row"   , &iRow                );
    UserINI.Load(sPath.c_str()  , "ETC"  , "Col"   , &iCol                );

    if(!iRow || !iCol) return false ;

    _sgLotMap->RowCount = iRow + 1;
    _sgLotMap->ColCount = iCol + 1;

    for (int iCnt =0 ; iCnt <= iRow + 1 ; iCnt ++ ) _sgLotMap  -> Cells[0   ][iCnt] = iCnt;
    for (int iCnt =0 ; iCnt <= iCol + 1 ; iCnt ++ ) _sgLotMap  -> Cells[iCnt][0   ] = iCnt;

    for(int r = 0 ; r < iRow ; r++) {
        sTemp.printf("VF%02d", r);
        UserINI.Load(sPath.c_str()  , "Data"  , sTemp , sRslt );
        for(int c = 0 ; c < iCol ; c++) {
            //sVal = "0x"+sRslt.SubString(c+1,1) ;
            sVal = sRslt.SubString(1,6) ;
            sRslt.Delete(1,7) ;
            dTemp = StrToFloatDef(sVal,0);
            dMapVf[r][c] = dTemp;
            sTemp2 = sVal+"," ;
        }
    }
    Trace("Rcv Vf Array",sTemp2.c_str()) ;
    sTemp2 = "" ;

    for(int r = 0 ; r < iRow ; r++) {
        sTemp.printf("VR%02d", r);
        UserINI.Load(sPath.c_str()  , "Data"  , sTemp , sRslt );
        for(int c = 0 ; c < iCol ; c++) {
            //sVal = "0x"+sRslt.SubString(c+1,1) ;
            sVal = sRslt.SubString(1,6) ;
            sRslt.Delete(1,7) ;
            dTemp = StrToFloatDef(sVal,0);
            dMapVr[r][c] = dTemp ;
            //_sgLotMap->Cells[r][c] = _sgLotMap->Cells[r][c] + "/" +sVal ;
            sTemp2 = sVal+"," ;
        }
    }
    Trace("Rcv Vr Array",sTemp2.c_str()) ;
    sTemp2 = "" ;

//    _sgLotMap->Font->Color = clBlack ;
    for(int r = 0 ; r < iRow ; r++) {
        sTemp.printf("VS%02d", r);
        UserINI.Load(sPath.c_str()  , "Data"  , sTemp , sRslt );
        for(int c = 0 ; c < iCol ; c++) {
            sVal = sRslt.SubString(1,1) ;
            sRslt.Delete(1,2) ;
            Rect = _sgLotMap->CellRect(c+1,r+1);

//                 if(sVal.ToIntDef(0) == (int)prReady ) _sgLotMap->Canvas->Brush->Color = clYellow  ;
//            else if(sVal.ToIntDef(0) == (int)prOk    ) _sgLotMap->Canvas->Brush->Color = clLime    ;
//            else if(sVal.ToIntDef(0) == (int)prVFNg  ) _sgLotMap->Canvas->Brush->Color = clFuchsia ;
//            else if(sVal.ToIntDef(0) == (int)prVRNg  ) _sgLotMap->Canvas->Brush->Color = clOlive   ;
//            else if(sVal.ToIntDef(0) == (int)prVFVRNg) _sgLotMap->Canvas->Brush->Color = clRed     ;

//            _sgLotMap->Canvas->FillRect(Rect);
            //VF VR
//            sVal = sRslt.SubString(1,2) ;
//            sRslt.Delete(1,3) ;

            iMapRslt[r][c] = sVal.ToIntDef(0);
            sTemp2 = sVal+"," ;
//            _sgLotMap->Font->Color = clBlack ;
//            _sgLotMap->Canvas->TextOut(Rect.Left + 1,Rect.Top,sVal);
//            _sgLotMap->Cells[c][r] = sVal ;

        }
    }

    Trace("Rcv Rslt Array",sTemp2.c_str()) ;

    return true ;
*/
}