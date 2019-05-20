//---------------------------------------------------------------------------
#pragma hdrstop

#include <stdio.h>

#include "SpcUnit.h"
#include "SLogUnit.h"
#include "UserIni.h"
#include "UtilDefine.h"
#include "UserFile.h"
#include "OptionMan.h"
#include "Timer.h"
#include "LotUnit.h"
//---------------------------------------------------------------------------
#include "FormSPC.h"
//---------------------------------------------------------------------------
#include "Rs232Man.h"
#pragma package(smart_init)

CSpc SPC;

__fastcall CSpc::CSpc(void)
{
    if(!UserFile.FileExist(SPC_FOLDER)) CreateDir(SPC_FOLDER);
    if(!UserFile.FileExist(ERR_FOLDER)) CreateDir(ERR_FOLDER);



//    ZeroMemory(iMapRslt , sizeof(int   ) * MAX_ARRAY_ROW * MAX_ARRAY_COL);
//    ZeroMemory(dMapVf   , sizeof(double) * MAX_ARRAY_ROW * MAX_ARRAY_COL);
//    ZeroMemory(dMapVr   , sizeof(double) * MAX_ARRAY_ROW * MAX_ARRAY_COL);
    iChipCnt   = 0     ;
    bUseZenner = false ;
}

__fastcall CSpc::~CSpc(void)
{
}

bool __fastcall CSpc::WriteErrData(int iErrNo , bool bRelease)
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

    //최근에 뜬 에러일시 팅겨냄...
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
            if(pfbuf){
                delete [] pfbuf;
                pfbuf = NULL;
            }
            return false;
        }
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

    if(pfbuf){
        delete [] pfbuf;
        pfbuf = NULL;
    }
    return true;
}
void __fastcall CSpc::SaveLastErr(int iErrNo)
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
int __fastcall CSpc::GetLastErr()
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

void __fastcall CSpc::DispErrData(AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrHistory)
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
        for(int i=iRowCnt -1 ; i > -1 ; i--){
            if(m_pErrHistory->Cells[1][i].ToIntDef(0) < sUDataTime.ToIntDef(0) && m_pErrHistory->Cells[1][i].ToIntDef(0) >= sFDataTime.ToIntDef(0)) {
                iLCol = i ;
                break;
            }
        }
    }

    if(iFCol == -1 || iLCol == -1 ){
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

        if(pfbuf){
           delete [] pfbuf;
           pfbuf = NULL ;
        }
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

void __fastcall CSpc::DispErrCnt(AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrCnt)
{
    AnsiString sPath ;
    AnsiString sFPath , sErrData = "";
    int iRowCnt , iColCnt ;
    int iR , iFCol = -1 , iLCol = -1 ;
    int hwnd, flen;
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
        for(int i=iRowCnt -1 ; i > -1 ; i--){
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

        if(pfbuf){
            delete [] pfbuf;
            pfbuf = NULL ;
        }
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

void __fastcall CSpc::DispMtba(AnsiString sDataTime , TBarSeries *_S1)
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

    UserFile.GridSearchDir(sPath , m_pErrCnt , 1 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬

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

        iErrCnt = 0;
        for (int i =0 ; i< 300 ; i++ ){
            if(sErrData.Pos("\r\n")) {
                sErrData.Delete(1,sErrData.Pos("\r\n") + 1);
                iErrCnt++;
            }
        }
        m_pErrCnt -> Cells[2][i] = iErrCnt;

        if(pfbuf){
            delete [] pfbuf;
            pfbuf = NULL ;
        }
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
                if(pfbuf){
                    delete [] pfbuf;
                    pfbuf = NULL ;
                }
                break;
            }
        }
        if(!bErrCnt) _S1 -> AddXY(i , 0 , i , clGreen);
    }

    delete m_pErrCnt   ;
    delete m_pErrNoCnt ;
    return ;
}

void __fastcall CSpc::SaveMtba(AnsiString sDataTime , TBarSeries *_S1)
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

void __fastcall CSpc::SaveErrCnt  (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrCnt)
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

void __fastcall CSpc::SaveErrData (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrCnt)
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

bool __fastcall CSpc::WriteArayData(EN_ARAY_ID riId ) //패스를 빼야 되나 ...매번 고쳐야 되자나...
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  , sArayName ;
    AnsiString sLotNo , sMgzNo , sSlotNo;
    AnsiString sTemp  ;
    AnsiString sTemp2 ;
    AnsiString sRslt  ;
    TDateTime  CurrDateTime;
    //int        iMgzNo , iSlotNo ;

    DelPastLog(LOG_FOLDER);

    if( DM.ARAY[riId].GetLotNo() == "" ) sLotNo = "NoLot";
    else                                 sLotNo = DM.ARAY[riId].GetLotNo();

//TODO ::
    sPath = LOG_FOLDER + CurrDateTime.CurrentDate().FormatString("yyyymmdd") + "\\" + sLotNo + "\\" ;

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    sPath = sPath + "\\" + sSlotNo + ".ini" ;

    for(int r = 0 ; r < OM.DevInfo.iWfRowCnt ; r++) {
        sRslt = "" ;
        for(int c = 0 ; c < OM.DevInfo.iWfColCnt ; c++) {
            sTemp2 = (int)DM.ARAY[riId].GetStat(r,c) ;
            sTemp2 = sTemp2.sprintf("%02d",(int)DM.ARAY[riId].GetStat(r,c)) + "_";
            sRslt += sTemp2 ;
        }
        sTemp.printf("R%02d", r);
        UserINI.Save(sPath.c_str()  , "Data"  , sTemp , sRslt );
    }

    UserINI.Save(sPath.c_str()  , "ETC"  , "ID"    , DM.ARAY[riId].GetID().ToIntDef(999999) );
    UserINI.Save(sPath.c_str()  , "ETC"  , "LotNo" , DM.ARAY[riId].GetLotNo()               );
    UserINI.Save(sPath.c_str()  , "ETC"  , "Row"   , OM.DevInfo.iWfRowCnt                   );
    UserINI.Save(sPath.c_str()  , "ETC"  , "Col"   , OM.DevInfo.iWfColCnt                   );

    return true ;
}

bool __fastcall CSpc::ReadArayData (AnsiString sPath )//, EN_ARAY_ID riId) //매거진 1개 일때만 랏드당 매거진 2개 이상 고려 안함.
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sTemp  ;
    AnsiString sVal   ;
    AnsiString sRslt  ;

    AnsiString sId    ;
    AnsiString sLotNo ;

    int iRow , iCol   ;

    AnsiString sTemp2= "" ;

    if(!FileExists(sPath)) return false ;

    UserINI.Load(sPath.c_str()  , "ETC"  , "ID"    , &sId                 );
    UserINI.Load(sPath.c_str()  , "ETC"  , "LotNo" , &sLotNo              );
    UserINI.Load(sPath.c_str()  , "ETC"  , "Row"   , &iRow                );
    UserINI.Load(sPath.c_str()  , "ETC"  , "Col"   , &iCol                );

    if(!iRow || !iCol) {
        SpcArray1.SetStat(csNone);
        return false;
    }

    SpcArray1.SetMaxColRow(iCol,iRow );

    for(int r = 0 ; r < iRow ; r++) {
        sTemp.printf("R%02d", r);
        UserINI.Load(sPath.c_str()  , "Data"  , sTemp , sRslt );
        for(int c = 0 ; c < iCol ; c++) {
            //sVal = "0x"+sRslt.SubString(c+1,1) ;
            sVal = sRslt.SubString(1,2) ;
            sRslt.Delete(1,3) ;
            SpcArray1.SetStat(r,c,(EN_CHIP_STAT)StrToInt(sVal)  );
            sTemp2 = sVal+"," ;
        }
    }

    SpcArray1.SetID   (sId   );
    SpcArray1.SetLotNo(sLotNo);

    Trace("SPC Array",sTemp2.c_str()) ;

    return true ;
}



void __fastcall CSpc::DelPastLog(AnsiString _sFolderPath)
{
    UserFile.ClearDirDate(LOG_FOLDER , Now() - 30 * 13); //일년짜리 그래프때문에 12개월로 함.
}


void __fastcall CSpc::DispLotDate(TStringGrid * _sgDate , TStringGrid * _sgLot , TStringGrid * _sgLotInfo)
{

    AnsiString sDay  = _sgDate -> Cells[1][ _sgDate -> Row] ;
    AnsiString sPath = LOG_FOLDER + sDay + "\\" + _sgLot -> Cells[1][ _sgLot -> Row];

    //StringGrid Clear
    for(int i =0 ; i < _sgLotInfo->ColCount ; i++ ) {
        for(int j =1 ; j < _sgLotInfo->RowCount ; j++ ) {
            _sgLotInfo -> Cells[i][j] = "" ;
        }
    }

    _sgLotInfo -> ColCount         = 33  ;
    _sgLotInfo -> DefaultColWidth  = 80  ;
    _sgLotInfo -> DefaultRowHeight = 25  ;
    _sgLotInfo -> ColWidths[0]     = 30  ;
    _sgLotInfo -> ColWidths[1]     = 90  ;
    _sgLotInfo -> ColWidths[2]     = 210 ;

    AnsiString  strName, str = "";
    AnsiString  sRowStr = "";
    AnsiString  sItmStr = "";

    int hwnd, flen;
    int iRowCnt = 0 ;
    int iColCnt = 0 ;
    char    *pfbuf;
    pfbuf = NULL;

    hwnd = FileOpen(sPath.c_str(), fmOpenRead) ;
    if(hwnd == NULL) return;

    flen = FileSeek(hwnd,0,2);
    FileSeek(hwnd,0,0);
    pfbuf = new char[flen+1];
    memset(pfbuf , 0 , sizeof(char)*(flen+1));
    FileRead(hwnd, pfbuf, flen);
    FileClose(hwnd);

    str = pfbuf ;

    while (str.Pos("\r\n")) {
        str.Delete(str.Pos("\r\n") , 2 ) ;
        if(iRowCnt > 100 ){
            if(pfbuf){
                delete [] pfbuf ;
                pfbuf = NULL ;
            }
            return ;
        }
        iRowCnt++ ;
    }

    _sgLotInfo -> RowCount = iRowCnt ;

    str = pfbuf ;
    iRowCnt = 0 ;

    if(pfbuf){
        delete [] pfbuf ;
        pfbuf = NULL ;
    }

    while (str.Pos("\r\n")) {
        sRowStr = str.SubString(1 , str.Pos("\r\n")) ;
        str.Delete(1,str.Pos("\r\n")+1) ;
        iColCnt = 0 ;
        _sgLotInfo -> Cells[iColCnt][iRowCnt] = iRowCnt  ;
        while (sRowStr.Pos(",")){
            iColCnt++ ;
            sItmStr = sRowStr.SubString(1,sRowStr.Pos(",")-1) ;
            sRowStr.Delete(1,sRowStr.Pos(",")) ;
            _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;
        }

        iColCnt++ ;
        sItmStr = sRowStr.SubString(1,sRowStr.Pos("\r\n" - 1 )) ;
        sRowStr.Delete(1,sRowStr.Pos("\r\n" - 1 )) ;
        _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;

        iRowCnt++;
    }
    _sgLotInfo -> RowCount = iRowCnt ;
}

void __fastcall CSpc::DispLotDate(AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgLotInfo)
{
    AnsiString sPath ;
    AnsiString sFPath , sFile ,sData = "";
    AnsiString sTemp;
    int iRowCnt , iColCnt ;
    int iR , iFCol = -1 , iLCol = -1 ;
    int hwnd, flen;
    char *pfbuf;
    pfbuf = NULL ;

    if(sFDataTime.ToIntDef(-1) > sUDataTime.ToIntDef(-2)) return ;

    //StringGrid Clear
    for(int i =0 ; i < _sgLotInfo->ColCount ; i++ ) {
        for(int j =1 ; j < _sgLotInfo->RowCount ; j++ ) {
            _sgLotInfo -> Cells[i][j] = "" ;
        }
    }

    _sgLotInfo -> ColCount =  33 ;
    _sgLotInfo -> DefaultColWidth  = 80 ;
    _sgLotInfo -> DefaultRowHeight = 25 ;
    _sgLotInfo -> ColWidths[0] = 30  ;
    _sgLotInfo -> ColWidths[1] = 90  ;
    _sgLotInfo -> ColWidths[2] = 210 ;

    m_pLotFolder = new TStringGrid(Application);
    m_pLotFile   = new TStringGrid(Application);

    sPath = LOG_FOLDER ;
    UserFile.GridSearchDir(sPath , m_pLotFolder , 0 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
    iRowCnt = m_pLotFolder->RowCount ;

    for(int i=0 ; i<iRowCnt ; i++){
        if( m_pLotFolder->Cells[1][i] == sFDataTime ) iFCol = i ;
        if( m_pLotFolder->Cells[1][i] == sUDataTime ) iLCol = i ;
    }

    if(iFCol == -1    ) {
        for(int i=0 ; i<iRowCnt ; i++){
            if(m_pLotFolder->Cells[1][i].ToIntDef(0) > sFDataTime.ToIntDef(0) && m_pLotFolder->Cells[1][i].ToIntDef(0) <= sUDataTime.ToIntDef(0)) {
                iFCol = i ;
                break;
            }
        }
    }

    if(iLCol == -1    ) {
        for(int i=iRowCnt -1 ; i > -1 ; i--){
            if(m_pLotFolder->Cells[1][i].ToIntDef(0) < sUDataTime.ToIntDef(0) && m_pLotFolder->Cells[1][i].ToIntDef(0) >= sFDataTime.ToIntDef(0)) {
                iLCol = i ;
                break;
            }
        }
    }

    if(iFCol == -1 || iLCol == -1 ) {
        delete m_pLotFolder ;
        delete m_pLotFile   ;
        return ;
    }

    AnsiString sTemp1,sTemp2;
    for(int i=iFCol ; i<=iLCol ; i++){
        sFPath= sPath + m_pLotFolder->Cells[1][i] + "\\";
        UserFile.GridSearchFile(sFPath , m_pLotFile , 1 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
        for(int j=0 ; j<m_pLotFile->RowCount ; j++) {
//            sTemp1 = m_pLotFile  ->Cells[1][j]          ;
            sFile += sFPath + m_pLotFile  ->Cells[1][j] + ",";
        }

    }

//    for(int i=iFCol ; i<=iLCol ; i++){


    AnsiString sFileSub ;
    while(sFile.Pos(","))
    {
        sFileSub = sFile.SubString(1,sFile.Pos(",")-1);
        sFile.Delete(1, sFile.Pos(",")  );
        sFPath= sFileSub;

        hwnd = FileOpen(sFPath.c_str(), fmOpenRead) ;
        if(hwnd == NULL) {
            delete m_pLotFolder ;
            delete m_pLotFile   ;
            return ;
        }

        flen = FileSeek(hwnd,0,2);
        FileSeek(hwnd,0,0);

        pfbuf = new char[flen+1];

        memset(pfbuf , 0 , sizeof(char)*(flen+1));
        FileRead(hwnd, pfbuf, flen);
        FileClose(hwnd);

        sTemp = pfbuf ;
//        sTemp.Delete(1,sTemp.Pos("\r\n")+1);
        sData += sTemp;

        if(pfbuf){
            delete [] pfbuf ;
            pfbuf = NULL ;
        }
    }


//    }

//    int iRowCnt = 0, iColCnt =0 ;
    AnsiString sRowStr, sItmStr ;

    iRowCnt = 0;
    while(sData.Pos("\r\n")){
        sRowStr = sData.SubString(1, sData.Pos("\r\n")) ;
        sData.Delete(1,sData.Pos("\r\n")+1) ;
        iColCnt = 0 ;
        _sgLotInfo->Cells[iColCnt][iRowCnt] = iRowCnt ;
        while (sRowStr.Pos(",")){
            iColCnt++ ;
            sItmStr = sRowStr.SubString(1,sRowStr.Pos(",")-1) ;
            sRowStr.Delete(1,sRowStr.Pos(",")) ;
            _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;
        }
        iColCnt++ ;
        sItmStr = sRowStr.SubString(1,sRowStr.Pos("\r\n" - 1 )) ;
        sRowStr.Delete(1,sRowStr.Pos("\r\n" - 1 )) ;
        _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;

        iRowCnt ++ ;
    }
    _sgLotInfo -> RowCount = iRowCnt ;

    delete m_pLotFolder ;
    delete m_pLotFile   ;
    return ;

}

void __fastcall CSpc::SaveLotData (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgLotInfo)
{
    AnsiString sPath , sTemp , sData , sFPath;
    int hFile;
    int hwnd, flen;

    if(sFDataTime.ToIntDef(-1) > sUDataTime.ToIntDef(-2)) return ;

    TDateTime  CurrDateTime = Now();

    //Set Path.
    sTemp = "LotInfo" ;
    sPath = SPC_FOLDER + sTemp;//".csv";

    //Set File Path.
    sFPath = sPath + "\\" + sFDataTime + "-" + sUDataTime + ".csv";

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    //Set Data.
    sData = sFDataTime + "," + sUDataTime + "," + CurrDateTime.CurrentDateTime().FormatString("yyyymmdd(hh:nn)") + "\r\n";

    for(int i=0; i<_sgLotInfo->RowCount; i++){
        for(int j=0; j<_sgLotInfo->ColCount; j++){
            sData += _sgLotInfo -> Cells[j][i] + "," ;
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
void __fastcall CSpc::DispLotDayDate (TStringGrid * _sgLotName , TStringGrid * _sgLotInfo)
{
    AnsiString sPath  = LOG_FOLDER ;
    AnsiString sFPath = LOG_FOLDER + _sgLotName -> Cells[1][ _sgLotName -> Row] ;

    for(int i =0 ; i < _sgLotInfo->ColCount ; i++ ) {
        for(int j =1 ; j < _sgLotInfo->RowCount ; j++ ) {
            _sgLotInfo -> Cells[i][j] = "" ;
        }
    }

    _sgLotInfo -> ColCount = 29 ;
    _sgLotInfo -> DefaultColWidth  = 80 ;
    _sgLotInfo -> DefaultRowHeight = 25 ;
    _sgLotInfo -> ColWidths[0] = 30  ;
    _sgLotInfo -> ColWidths[1] = 90  ;
    _sgLotInfo -> ColWidths[2] = 90  ;

    AnsiString  strName, str = "";
    AnsiString  sRowStr = "";
    AnsiString  sItmStr = "";

    int hwnd, flen;
    int iRowCnt = 0 ;
    int iColCnt = 0 ;
    char    *pfbuf;
    pfbuf = NULL ;

    hwnd = FileOpen(sFPath.c_str(), fmOpenRead) ;
    if(hwnd == NULL) return;

    flen = FileSeek(hwnd,0,2);
    FileSeek(hwnd,0,0);
    pfbuf = new char[flen+1];
    memset(pfbuf , 0 , sizeof(char)*(flen+1));
    FileRead(hwnd, pfbuf, flen);
    FileClose(hwnd);

    str = pfbuf ;

    if(pfbuf){
        delete [] pfbuf ;
        pfbuf = NULL ;
    }

    iRowCnt = 0 ;

    while (str.Pos("\r\n")) {
        sRowStr = str.SubString(1 , str.Pos("\r\n")) ;
        str.Delete(1,str.Pos("\r\n")+1) ;
        iColCnt = 0 ;
        _sgLotInfo -> Cells[iColCnt][iRowCnt] = iRowCnt  ;
        while (sRowStr.Pos(",")){
            iColCnt++ ;
            sItmStr = sRowStr.SubString(1,sRowStr.Pos(",")-1) ;
            sRowStr.Delete(1,sRowStr.Pos(",")) ;
            _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;
        }

        iColCnt++ ;
        sItmStr = sRowStr.SubString(1,sRowStr.Pos("\r\n" - 1 )) ;
        sRowStr.Delete(1,sRowStr.Pos("\r\n" - 1 )) ;
        _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;

        iRowCnt++;
    }
    _sgLotInfo -> RowCount = iRowCnt ;    
}
void __fastcall CSpc::DispLotDayDate (AnsiString sFDataTime    , AnsiString sUDataTime , TStringGrid * _sgLotInfo)
{
    AnsiString sPath;
    AnsiString sFPath , sFile ,sData = "";
    AnsiString sTemp;
    int iRowCnt , iColCnt ;
    int iR , iFCol = -1 , iLCol = -1 ;
    int hwnd, flen;
    char *pfbuf;
    pfbuf = NULL ;

    if(sFDataTime.ToIntDef(-1) > sUDataTime.ToIntDef(-2)) return ;

    for(int i =0 ; i < _sgLotInfo->ColCount ; i++ ) {
        for(int j =1 ; j < _sgLotInfo->RowCount ; j++ ) {
            _sgLotInfo -> Cells[i][j] = "" ;
        }
    }

    _sgLotInfo -> ColCount = 33 ;

    _sgLotInfo -> DefaultColWidth  = 80 ;
    _sgLotInfo -> DefaultRowHeight = 25 ;
    _sgLotInfo -> ColWidths[0] = 30  ;
    _sgLotInfo -> ColWidths[1] = 90  ;
    _sgLotInfo -> ColWidths[2] = 90  ;

    m_pLotFile   = new TStringGrid(Application);

    sPath = LOG_FOLDER ;

    UserFile.GridSearchFile(sPath , m_pLotFile , 0 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬

    iRowCnt = m_pLotFile->RowCount ;
//    sFDataTime += ".csv" ;
//    sUDataTime += ".csv" ;

    for(int i=0 ; i<iRowCnt ; i++){
        if( m_pLotFile->Cells[1][i].SubString(0,8) == sFDataTime ) iFCol = i ;
        if( m_pLotFile->Cells[1][i].SubString(0,8) == sUDataTime ) iLCol = i ;
    }

    if(iFCol == -1    ) {
        for(int i=0 ; i<iRowCnt ; i++){
            sTemp = m_pLotFile->Cells[1][i].SubString(0,8);
            if(sTemp.ToIntDef(0) > sFDataTime.ToIntDef(0) && sTemp.ToIntDef(0) <= sUDataTime.ToIntDef(0)) {
                iFCol = i ;
                break;
            }
        }
    }

    if(iLCol == -1    ) {
        for(int i=iRowCnt -1 ; i > -1 ; i--){
            sTemp = m_pLotFile->Cells[1][i].SubString(0,8);
            if(sTemp.ToIntDef(0) < sUDataTime.ToIntDef(0) && sTemp.ToIntDef(0) >= sFDataTime.ToIntDef(0)) {
                iLCol = i ;
                break;
            }
        }
    }

    if(iFCol == -1 || iLCol == -1  ){
        delete m_pLotFile ;
        return ;
    }

    AnsiString sTemp1,sTemp2;
    for(int i=iFCol ; i<=iLCol ; i++){
        sFPath= sPath + m_pLotFile->Cells[1][i] ;
        sFile += sFPath + ",";
    }

//    for(int i=iFCol ; i<=iLCol ; i++){

    AnsiString sFileSub ;
    while(sFile.Pos(","))
    {
        sFileSub = sFile.SubString(1,sFile.Pos(",")-1);
        sFile.Delete(1, sFile.Pos(",")  );
        sFPath= sFileSub;

        hwnd = FileOpen(sFPath.c_str(), fmOpenRead) ;
        if(hwnd == NULL) continue ;

        flen = FileSeek(hwnd,0,2);
        FileSeek(hwnd,0,0);

        pfbuf = new char[flen+1];
        memset(pfbuf , 0 , sizeof(char)*(flen+1));
        FileRead(hwnd, pfbuf, flen);
        FileClose(hwnd);

        sTemp = pfbuf ;
//        sTemp.Delete(1,sTemp.Pos("\r\n")+1);
        sData += sTemp;
        if(pfbuf){
            delete [] pfbuf ;
            pfbuf = NULL ;
        }
    }
//    int iRowCnt = 0, iColCnt =0 ;
    AnsiString sRowStr, sItmStr ;

    iRowCnt = 0;
    while(sData.Pos("\r\n")){
        sRowStr = sData.SubString(1, sData.Pos("\r\n")) ;
        sData.Delete(1,sData.Pos("\r\n")+1) ;
        iColCnt = 0 ;
        _sgLotInfo->Cells[iColCnt][iRowCnt] = iRowCnt ;
        while (sRowStr.Pos(",")){
            iColCnt++ ;
            sItmStr = sRowStr.SubString(1,sRowStr.Pos(",")-1) ;
            sRowStr.Delete(1,sRowStr.Pos(",")) ;
            _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;
        }
        iColCnt++ ;
        sItmStr = sRowStr.SubString(1,sRowStr.Pos("\r\n" - 1 )) ;
        sRowStr.Delete(1,sRowStr.Pos("\r\n" - 1 )) ;
        _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;

        iRowCnt ++ ;
    }
    _sgLotInfo -> RowCount = iRowCnt ;

    delete m_pLotFile ;
    return ;
}
void __fastcall CSpc::SaveLotDayData (AnsiString sFDataTime    , AnsiString sUDataTime , TStringGrid * _sgLotInfo)
{
    AnsiString sPath , sTemp , sData , sFPath;
    int hFile;
    int hwnd, flen;

    if(sFDataTime.ToIntDef(-1) > sUDataTime.ToIntDef(-2)) return ;

    TDateTime  CurrDateTime = Now();

    //Set Path.
    sTemp = "LotDayInfo" ;
    sPath = SPC_FOLDER + sTemp;//".csv";

    //Set File Path.
    sFPath = sPath + "\\" + sFDataTime + "-" + sUDataTime + ".csv";

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    //Set Data.
    sData = sFDataTime + "," + sUDataTime + "," + CurrDateTime.CurrentDateTime().FormatString("yyyymmdd(hh:nn)") + "\r\n";

    for(int i=0; i<_sgLotInfo->RowCount; i++){
        for(int j=0; j<_sgLotInfo->ColCount; j++){
            sData += _sgLotInfo -> Cells[j][i] + "," ;
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

bool __fastcall CSpc::WriteArayData(CArray * riAray) //패스를 빼야 되나 ...매번 고쳐야 되자나...
{
    //Local Var.
    TUserINI   UserINI        ;
    AnsiString sPath          ;
    AnsiString sLotNo         ;
    AnsiString sRslt          ;
    AnsiString sTemp  ,sTemp2 ;
    TDateTime  CurrDateTime   ;

    DelPastLotLog();

    if( riAray->GetLotNo() == "" ) sLotNo = "NoLot";
    else                           sLotNo = riAray->GetLotNo();

    sPath = LOG_FOLDER + CurrDateTime.CurrentDate().FormatString("yyyymmdd");

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    sPath = sPath + "\\" + sLotNo + ".ini";

    for(int r = 0 ; r < OM.DevInfo.iWfRowCnt ; r++) {
        sRslt = "" ;
        for(int c = 0 ; c < OM.DevInfo.iWfColCnt ; c++) {
            sTemp2 = (int)riAray->GetStat(r,c) ;
            sTemp2 = sTemp2.sprintf("%02d",(int)riAray->GetStat(r,c)) + "_";
            sRslt += sTemp2 ;
        }
        sTemp.printf("R%02d", r);
        UserINI.Save(sPath.c_str()  , "Data"  , sTemp , sRslt );
    }

    UserINI.Save(sPath.c_str()  , "ETC"  , "ID"    , riAray->GetID().ToIntDef(999999) );
    UserINI.Save(sPath.c_str()  , "ETC"  , "LotNo" , riAray->GetLotNo()               );
    UserINI.Save(sPath.c_str()  , "ETC"  , "Row"   , OM.DevInfo.iWfRowCnt             );
    UserINI.Save(sPath.c_str()  , "ETC"  , "Col"   , OM.DevInfo.iWfColCnt             );

    return true ;
}

bool __fastcall CSpc::ReadArayData (AnsiString sPath , CArray * riAray) //매거진 1개 일때만 랏드당 매거진 2개 이상 고려 안함.
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sTemp  ;
    AnsiString sVal   ;
    AnsiString sRslt  ;

    AnsiString sId    ;
    AnsiString sLotNo ;

    int iRow , iCol   ;

    AnsiString sTemp2= "" ;

    if(!FileExists(sPath)) return false ;

    UserINI.Load(sPath.c_str()  , "ETC"  , "ID"    , &sId    );
    UserINI.Load(sPath.c_str()  , "ETC"  , "LotNo" , &sLotNo );
    UserINI.Load(sPath.c_str()  , "ETC"  , "Row"   , &iRow   );
    UserINI.Load(sPath.c_str()  , "ETC"  , "Col"   , &iCol   );

    riAray -> SetMaxColRow(iCol,iRow );

    for(int r = 0 ; r < iRow ; r++) {
        sTemp.printf("R%02d", r);
        UserINI.Load(sPath.c_str()  , "Data"  , sTemp , sRslt );
        for(int c = 0 ; c < iCol ; c++) {
            //sVal = "0x"+sRslt.SubString(c+1,1) ;
            sVal = sRslt.SubString(1,2) ;
            sRslt.Delete(1,3) ;
            riAray->SetStat(r,c,(EN_CHIP_STAT)StrToInt(sVal)  );
            sTemp2 = sVal+"," ;
        }
    }

    riAray->SetID   (sId   );
    riAray->SetLotNo(sLotNo);

    Trace("Rcv Array",sTemp2.c_str()) ;

    return true ;
}

void __fastcall CSpc::UpdateDate(TStringGrid * _sgDate)
{
    UserFile.GridSearchDir(SPC_FOLDER , _sgDate , 1 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
}

void __fastcall CSpc::UpdateLotName(TStringGrid * _sgDate , TStringGrid * _sgLot)
{
    AnsiString sDay ,sPath ;
    sDay = _sgDate->Cells[1][_sgDate -> Row] ;
    sPath = LOG_FOLDER + sDay + "\\" ;
    UserFile.GridSearchFile(sPath , _sgLot , 1 , true);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
}

void __fastcall CSpc::DelPastLotLog()
{
    UserFile.ClearDirDate(LOG_FOLDER , Now() - 400);
}
//Lot Info Map StringGrid

bool __fastcall CSpc::WriteLotMap(EN_ARAY_ID riId )
{

}

bool __fastcall CSpc::ReadLotMap (AnsiString sPath , TStringGrid *_sgLotMap)
{

}


