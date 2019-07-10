//---------------------------------------------------------------------------
#pragma hdrstop

#include <stdio.h>

#include "ErrUnit.h"
#include "SLogUnit.h"
#include "UserIni.h"
#include "UtilDefine.h"
#include "UserFile.h"
#include "OptionMan.h"
#include "Timer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define ERR_FOLDER "d:\\ErrLog\\"
#define LOG_FOLDER "d:\\LotLog\\"
#define SPC_FOLDER "d:\\Spc\\"

CErr ER;

__fastcall CErr::CErr(void)
{
}

__fastcall CErr::~CErr(void)
{
}

bool __fastcall CErr::WriteErrData(int iErrNo , bool bRelease)
{
    AnsiString sPath , sTemp , sErrData , sFPath;
    int hFile;
    TDateTime  CurrDateTime = Now();;

    if( iErrNo == (int)eiETC_LotEnd) return false;

    //최근에 뜬 에러일시 팅겨냄...
    int iLastErr = GetLastErr();
    if( iLastErr != -1 && iLastErr == iErrNo ) return false;

    //Set Path.
    sPath        = ERR_FOLDER + CurrDateTime.CurrentDate().FormatString("yyyymmdd") ;//".csv";

    //Set File Path.
    sFPath = sPath + "\\ErrLog.csv";

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    //Set Data.
    if(!bRelease) sErrData.sprintf("%03d,%s,%s\r\n", iErrNo , CurrDateTime.FormatString("(yy.mm.dd)hh:nn:ss") , "NONE");
    else {
//        AnsiString str = "";
        sErrData = "";
        int hwnd, flen;
        char *pfbuf;

        hwnd = FileOpen(sFPath.c_str(), fmOpenRead) ;
        if(hwnd == NULL) return false;

        flen = FileSeek(hwnd,0,2);
        FileSeek(hwnd,0,0);
        pfbuf = new char[flen+1];
        memset(pfbuf , 0 , sizeof(char)*(flen+1));
        FileRead(hwnd, pfbuf, flen);
        FileClose(hwnd);

        sErrData = pfbuf ;
        if(sErrData.SubString(sErrData.Length()-5 , 4) != "NONE" ) return false;
        sErrData.Delete(sErrData.Length()-5 , 6 );

        sErrData = sErrData + CurrDateTime.FormatString("yy.mm.dd(hh:nn:ss)") + "\r\n";

        UserFile.ClearDir (sPath , true);
        UserFile.CreateDir(sPath.c_str());

    }

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
void __fastcall CErr::SaveLastErr(int iErrNo)
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
int __fastcall CErr::GetLastErr()
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

void __fastcall CErr::DispErrData(AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrHistory)
{
    AnsiString sPath ;
    AnsiString sFPath , sErrData = "";
    int iRowCnt , iColCnt ;
    int iR , iFCol = -1 , iLCol = -1 ;
    int hwnd, flen;
    char *pfbuf;

    for(int i =0 ; i < _sgErrHistory->ColCount ; i++ ) {
        for(int j =1 ; j < _sgErrHistory->RowCount ; j++ ) {
            _sgErrHistory -> Cells[i][j] = "" ;
        }
    }

    m_pErrHistory = new TStringGrid(Application);

    sPath = ERR_FOLDER ;
    UserFile.GridSearchDir(sPath , m_pErrHistory , 0 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬

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

    if(iFCol == -1    ) return ;
    if(iLCol == -1    ) return ;

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
    return ;

}

void __fastcall CErr::DispErrCnt(AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrCnt)
{
    AnsiString sPath ;
    AnsiString sFPath , sErrData = "";
    int iRowCnt , iColCnt ;
    int iR , iFCol = -1 , iLCol = -1 ;
    int hwnd, flen;
    char *pfbuf;

    for(int i =0 ; i < _sgErrCnt->ColCount ; i++ ) {
        for(int j =1 ; j < _sgErrCnt->RowCount ; j++ ) {
            _sgErrCnt -> Cells[i][j] = "" ;
        }
    }

    m_pErrCnt   = new TStringGrid(Application);
    m_pErrNoCnt = new TStringGrid(Application);
    
    sPath = ERR_FOLDER ;
    UserFile.GridSearchDir(sPath , m_pErrCnt , 0 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬

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

    if(iFCol == -1    ) return ;
    if(iLCol == -1    ) return ;

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

    return;
}

void __fastcall CErr::DispMtba(AnsiString sDataTime , TBarSeries *_S1)
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

    //Tchart 초기화.
    _S1 -> Clear();

    m_pErrCnt     = new TStringGrid(Application);
    m_pErrNoCnt   = new TStringGrid(Application);

    sPath = ERR_FOLDER ;

    UserFile.GridSearchDir(sPath , m_pErrCnt , 0 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬

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
                break;
            }
        }
        if(!bErrCnt) _S1 -> AddXY(i , 0 , i , clGreen);
    }
    return ;
}

void __fastcall CErr::SaveMtba(AnsiString sDataTime , TBarSeries *_S1)
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

void __fastcall CErr::SaveErrCnt  (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrCnt)
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

void __fastcall CErr::SaveErrData (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrCnt)
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

void __fastcall CErr::SaveOptionLog ( int iIndex , bool bInverse)
{
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

            default : return ;
        }
    }

    sData += "\r\n";

    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);

    return ;

}
void __fastcall CErr::SaveClearLog (AnsiString _sJobFile , AnsiString _sLotNo ,AnsiString _sSlotNo , AnsiString _sMsg)
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
