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
#include "DataMan.h"
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

bool __fastcall CSPC::WriteErrData(int iErrNo , bool bRelease)
{
    AnsiString sPath , sTemp , sErrData , sFPath;
    int hFile;
    TDateTime  CurrDateTime = Now();;

    DelPastLog(ERR_FOLDER);

         if( iErrNo == (int)eiETC_LotEnd       ) return false;
    else if( iErrNo == (int)eiETC_Door         ) return false;
    else if( iErrNo == (int)eiLDR_YCmpHomeEnd  ) return false;
    else if( iErrNo == (int)eiLDR_ZCmpHomeEnd  ) return false;
    else if( iErrNo == (int)eiPSB_TSrtHomeEnd  ) return false;
    else if( iErrNo == (int)eiWK1_YVsnHomeEnd  ) return false;
    else if( iErrNo == (int)eiWK1_ZVsnHomeEnd  ) return false;
    else if( iErrNo == (int)eiWK2_YVsnHomeEnd  ) return false;
    else if( iErrNo == (int)eiWRK_XVsnHomeEnd  ) return false;
    else if( iErrNo == (int)eiULD_YCmpHomeEnd  ) return false;
    else if( iErrNo == (int)eiULD_ZCmpHomeEnd  ) return false;
    else if( iErrNo == (int)eiPSB_XTrmHomeEnd  ) return false;
    else if( iErrNo == (int)eiPSB_YTrmHomeEnd  ) return false;
    else if( iErrNo == (int)eiPSB_XCmpHomeEnd  ) return false;
    else if( iErrNo == (int)eiPSB_XInsHomeEnd  ) return false;
    else if( iErrNo == (int)eiWK1_VsnFailCnt   ) return false;
    else if( iErrNo == (int)eiWK2_VsnFailCnt   ) return false;
    else if( iErrNo == (int)eiHED_Visn1NotReady) return false;
    else if( iErrNo == (int)eiHED_Visn2NotReady) return false;
    else if( iErrNo == (int)eiLDR_MgzFull      ) return false;
    else if( iErrNo == (int)eiULD_MgzFull      ) return false;
    else if( iErrNo == (int)eiHED_Visn1Read    ) return false;
    else if( iErrNo == (int)eiHED_Visn2Read    ) return false;
    else if( iErrNo == (int)eiULD_NeedMgz      ) return false;
    else if( iErrNo == (int)eiETC_LotOverMgz   ) return false;
    else if( iErrNo == (int)eiWK1_VsnSFailCnt  ) return false;
    else if( iErrNo == (int)eiWK2_VsnSFailCnt  ) return false;

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

void __fastcall CSPC::DispErrCnt(AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrCnt)
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
        _sgErrCnt -> Cells[2][iErrCnt+1] = EM_GetErrName(m_pErrNoCnt -> Cells[0][i].ToIntDef(0));
        _sgErrCnt -> Cells[3][iErrCnt+1] = m_pErrNoCnt -> Cells[1][i] ;

        iErrCnt ++;
    }

    _sgErrCnt -> RowCount = iErrCnt + 1 ;

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

                if(m_pErrCnt -> Cells[2][j-1].ToIntDef(1)) {
                    _S1 -> AddXY(i , iTime / m_pErrCnt -> Cells[2][j-1].ToIntDef(1) , i , clGreen);
                }
                else {
                    _S1 -> AddXY(i , 0 , i , clGreen);
                }
                bErrCnt = true ;
                break;
            }
        }
        if(!bErrCnt) _S1 -> AddXY(i , 0 , i , clGreen);
    }
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

void __fastcall CSPC::SaveOptionLog (AnsiString _sJobFile , AnsiString _sLotNo ,AnsiString _sSlotNo , AnsiString _sMsg)
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

    DelPastLog(SPC_FOLDER);

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

    DelPastLog(SPC_FOLDER);

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
void __fastcall CSPC::SaveTrimLog (EN_ARAY_ID riId , bool bBefore , int iErr)
{
    AnsiString sPath , sTemp , sData , sFPath;
    int hFile;
    int hwnd, flen;

    int icsFail  ,icsRslt1  , icsRslt2 , icsRslt3 , icsRslt4 , icsRslt5 , icsRslt6 , icsRslt7 , icsRslt8 , icsRslt9 ,icsRslt10 = 0;
    int icsRslt11,icsRslt12 , icsRslt13 , icsRslt14 , icsEmpty , icsAllFail = 0;

    TDateTime  CurrDateTime = Now();

    //Set Path.
//    sTemp = "TrimLog" ;
    sPath = "D:\\TrimLog" ;//".csv";

    //Set File Path.
    sFPath = sPath + "\\" + CurrDateTime.FormatString("yyyy-mm-dd") + ".csv";

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    DelPastLog(SPC_FOLDER);

    sData = "";


    hFile = FileOpen(sFPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sFPath.c_str());
        if (hFile == -1) { Trace("Err",(sFPath + "is Can't made").c_str());  return ; }
        sData = "DATE,TIME,JOBFILE,LOTNO,SLOTNO,TRIM(B/A),FAILCOUNT,SENSORERR,TRIM,VEMPTY,HEATFAIL,CHIP,DUST,LFLOW,BROKEN,CRACK,BURR,ZENER,WIRE,ORIENT,PMEASURE,PDIST,MATCH,FLOW\r\n";
    }

    AnsiString sLotNo , sSlotNo , sJobFile;
    int iSlotNo , iSensorErr;
    sJobFile   = OM.m_sCrntDev ;
    sLotNo     = DM.ARAY[riId].GetLotNo() ;
    iSlotNo    = DM.ARAY[riId].GetID   ().ToIntDef(0) % 100;
    iSensorErr = iErr;
    icsEmpty   = DM.ARAY[riId].GetCntStat(csEmpty );
    icsRslt1   = DM.ARAY[riId].GetCntStat(csRslt1 );
    icsFail    = DM.ARAY[riId].GetCntStat(csFail  );
    icsRslt2   = DM.ARAY[riId].GetCntStat(csRslt2 );
    icsRslt3   = DM.ARAY[riId].GetCntStat(csRslt3 );
    icsRslt4   = DM.ARAY[riId].GetCntStat(csRslt4 );
    icsRslt5   = DM.ARAY[riId].GetCntStat(csRslt5 );
    icsRslt6   = DM.ARAY[riId].GetCntStat(csRslt6 );
    icsRslt7   = DM.ARAY[riId].GetCntStat(csRslt7 );
    icsRslt8   = DM.ARAY[riId].GetCntStat(csRslt8 );
    icsRslt9   = DM.ARAY[riId].GetCntStat(csRslt9 );
    icsRslt10  = DM.ARAY[riId].GetCntStat(csRslt10);
    icsRslt11  = DM.ARAY[riId].GetCntStat(csRslt11);
    icsRslt12  = DM.ARAY[riId].GetCntStat(csRslt12);
    icsRslt13  = DM.ARAY[riId].GetCntStat(csRslt13);
    icsRslt14  = DM.ARAY[riId].GetCntStat(csRslt14);

    icsAllFail = icsFail + icsRslt1 + icsRslt2 + icsRslt3 + icsRslt4 + icsRslt5 + icsRslt6 + icsRslt7 + icsRslt8 +
                 icsRslt9 + icsRslt10 + icsRslt11 + icsRslt12 + icsRslt13 + icsRslt14 + icsEmpty;

    //Set Data.
    sData += CurrDateTime.FormatString("yyyy-mm-dd")+ ",";
    sData += CurrDateTime.FormatString("hh:nn:ss")+ ",";
    sData += sJobFile   + ","    ;
    sData += sLotNo     + ","    ;
    sData += (AnsiString)iSlotNo    + ","    ;
    if(bBefore) sData   +="B,"   ;
    else        sData   +="A,"   ;
    sData += (AnsiString)icsAllFail + ","    ;
    sData += (AnsiString)iErr       + ","    ;
    sData += (AnsiString)icsEmpty   + ","    ;
    sData += (AnsiString)icsRslt1   + ","    ;
    sData += (AnsiString)icsFail    + ","    ;
    sData += (AnsiString)icsRslt2   + ","    ;
    sData += (AnsiString)icsRslt3   + ","    ;
    sData += (AnsiString)icsRslt4   + ","    ;
    sData += (AnsiString)icsRslt5   + ","    ;
    sData += (AnsiString)icsRslt6   + ","    ;
    sData += (AnsiString)icsRslt7   + ","    ;
    sData += (AnsiString)icsRslt8   + ","    ;
    sData += (AnsiString)icsRslt9   + ","    ;
    sData += (AnsiString)icsRslt10  + ","    ;
    sData += (AnsiString)icsRslt11  + ","    ;
    sData += (AnsiString)icsRslt12  + ","    ;
    sData += (AnsiString)icsRslt13  + ","    ;
    sData += (AnsiString)icsRslt14  + "\r\n" ;

    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);

    return ;

}


void __fastcall CSPC::DelPastLog(AnsiString _sFolderPath)
{
    UserFile.ClearDirDate(LOG_FOLDER , Now() - 30 * 13); //일년짜리 그래프때문에 12개월로 함.
}

