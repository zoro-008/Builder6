//---------------------------------------------------------------------------
#pragma hdrstop

#include <stdio.h>

#include "SPCUnit.h"
#include "SLogUnit.h"
#include "UserIni.h"
#include "UtilDefine.h"
#include "UserFile.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "LotUnit.h"


#include "SMInterfaceUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define ERR_FOLDER "d:\\ErrLog\\"+OM.EqpOptn.sModelName+"\\"
#define LOT_FOLDER "d:\\LotLog\\"+OM.EqpOptn.sModelName+"\\"
#define DAY_FOLDER "d:\\DayLog\\"+OM.EqpOptn.sModelName+"\\"

//const  EN_ARAY_ID DATA_ARAY = riSRT ;

CSPC SPC;

CErrData::CErrData()
{
}
CErrData::~CErrData()
{
}

void CErrData::Init()
{
    LoadSaveLastErr(true);
}

void CErrData::Close()
{
    LoadSaveLastErr(false);
}

void CErrData::LoadSaveLastErr(bool _bLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    AnsiString sCaption ;

    //Set Dir.
    sPath    = "d:\\ErrLog\\"+OM.EqpOptn.sModelName+"\\" ;//ERR_FOLDER    ;
    sPath   += "LastErr.ini" ;
    sCaption = "LastInfo"    ;

    if(_bLoad) {
        UserINI.Load(sPath.c_str() , sCaption , "m_iLastErr"    , m_iLastErr   );
        UserINI.Load(sPath.c_str() , sCaption , "m_sLastErrMsg" , m_sLastErrMsg);
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath.c_str() , sCaption , "m_iLastErr"    , m_iLastErr   );
        UserINI.Save(sPath.c_str() , sCaption , "m_sLastErrMsg" , m_sLastErrMsg);
    }
}


bool CErrData::SaveErrIni (TData &_tData)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    AnsiString sCaption ;
    TDateTime  tErrDateTime ;

    tErrDateTime.Val = _tData.dSttTime ;

    sPath = ERR_FOLDER + tErrDateTime.CurrentDate().FormatString("yyyymmdd") + ".ini" ;

    int iCnt ;
    UserINI.Load(sPath.c_str() , "ETC" , "ErrCnt"  , iCnt);

    sCaption = iCnt ;

    UserINI.Save(sPath.c_str() , sCaption , "iErrNo  " , _tData.iErrNo  );
    UserINI.Save(sPath.c_str() , sCaption , "sErrName" , _tData.sErrName);
    UserINI.Save(sPath.c_str() , sCaption , "dSttTime" , _tData.dSttTime);
    UserINI.Save(sPath.c_str() , sCaption , "sErrMsg " , _tData.sErrMsg );
    UserINI.Save(sPath.c_str() , sCaption , "sLotId  " , _tData.sLotId  );

    iCnt++;
    UserINI.Save(sPath.c_str() , "ETC" , "ErrCnt"  , iCnt);
}

int CErrData::GetErrCnt  (TDateTime _tSttData , TDateTime _tEndData)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    int iCnt = 0 ;
    int iCntSum = 0 ;

    TDateTime SearchDate ;
    int iSttDate = _tSttData ; //소수점 없어지면서 날만 남음.
    int iEndDate = _tEndData ;

    for(int d = iSttDate ; d <= iEndDate ; d++) {
        SearchDate.Val = d ;
        sPath = ERR_FOLDER + SearchDate.FormatString("yyyymmdd") + ".ini" ;
        UserINI.Load(sPath.c_str() , "ETC" , "ErrCnt"  , iCnt);
        iCntSum += iCnt ;
    }
    return iCntSum ;
}

bool CErrData::LoadErrIni(TDateTime _tSttData , TDateTime _tEndData , TData * _tData)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    AnsiString sCaption ;
    int iErrCnt = 0 ;
    int iMaxErrDayCnt = 0 ;

    TDateTime SearchDate ;
    int iSttDate = _tSttData ; //소수점 없어지면서 날만 남음.
    int iEndDate = _tEndData ;

    for(int d = iSttDate ; d <= iEndDate ; d++) {
        SearchDate.Val = d ;
        sPath          = ERR_FOLDER + SearchDate.FormatString("yyyymmdd") + ".ini" ;
        iMaxErrDayCnt  = GetErrCnt  (SearchDate , SearchDate);
        for(int c = 0 ; c < iMaxErrDayCnt ; c++) {
            sCaption = c ;
            UserINI.Load(sPath.c_str() , sCaption , "iErrNo  " , _tData[iErrCnt].iErrNo  );
            UserINI.Load(sPath.c_str() , sCaption , "sErrName" , _tData[iErrCnt].sErrName);
            UserINI.Load(sPath.c_str() , sCaption , "dSttTime" , _tData[iErrCnt].dSttTime);
            UserINI.Load(sPath.c_str() , sCaption , "sErrMsg " , _tData[iErrCnt].sErrMsg );
            UserINI.Load(sPath.c_str() , sCaption , "sLotId  " , _tData[iErrCnt].sLotId  );
            iErrCnt++;
        }
    }
    return true ;
}

void CErrData::SortErrData(bool _bNumberTime , int _iDataCnt , TData * _tData)
{
    TData _tTempData ;

    if(_bNumberTime) { //숫자 우선.
        for(int i = 0; i < _iDataCnt; i++) { //버블버블 버블팝 버블버블 팝팝!!
           for(int j = 0; j < _iDataCnt -i - 1; j++) {
              if(_tData[j].iErrNo > _tData[j + 1].iErrNo) {
                 _tTempData = _tData[j];
                 _tData[j] = _tData [j + 1];
                 _tData[j + 1] = _tTempData;
              }
           }
        }
    }
    else { //시간 우선.
        for(int i = 0; i < _iDataCnt; i++) { //버블버블 버블팝 버블버블 팝팝!!
           for(int j = 0; j < _iDataCnt -i - 1; j++) {
              if(_tData[j].dSttTime > _tData[j + 1].dSttTime) {
                 _tTempData = _tData[j];
                 _tData[j] = _tData [j + 1];
                 _tData[j + 1] = _tTempData;
              }
           }
        }
    }
}

bool CErrData::SetErr(int _iErrNo , AnsiString _sErrName , AnsiString _sErrMsg , AnsiString _sLotId)
{
    //최근에 뜬 에러일시 팅겨냄...
    if(m_iLastErr == _iErrNo && m_sLastErrMsg == _sErrMsg) return false;

    if(!DirectoryExists(ERR_FOLDER)) UserFile.CreateDir(ERR_FOLDER);

    //기존에 있던것들 지우기.
    UserFile.ClearDirDate(ERR_FOLDER , Now() - 30 * 3); //90일.

    m_tErrData.iErrNo   = _iErrNo   ;
    m_tErrData.sErrName = _sErrName ;
    m_tErrData.dSttTime = Now().Val ;
    m_tErrData.sErrMsg  = _sErrMsg  ;
    m_tErrData.sLotId   = _sLotId   ;

    SaveErrIni(m_tErrData);

    m_iLastErr    = _iErrNo  ;
    m_sLastErrMsg = _sErrMsg ;

    return true;
}


void CErrData::DispErrData (TDateTime _tSttData , TDateTime _tEndData , TStringGrid * _sgErrDate , bool _bNumberTime) //true면 에러넘버 , false면 시간.
{
    if(!_sgErrDate) return ;

    int iErrCnt = GetErrCnt(_tSttData , _tEndData) ;

    TData * Datas = new TData[iErrCnt];

    LoadErrIni(_tSttData , _tEndData , Datas);

    SortErrData(_bNumberTime , iErrCnt , Datas) ;

    _sgErrDate -> RowCount  = iErrCnt + 1 ;
    if(iErrCnt == 0 ) return ;
    _sgErrDate -> ColCount  = 6 ;
    _sgErrDate -> FixedCols = 1 ;
    _sgErrDate -> FixedRows = 1 ;
    //_sgErrDate -> DefaultRowHeight = 25 ;
    //_sgErrDate -> DefaultColWidth  = 80 ;

    _sgErrDate -> Cells[0][0] = "No"        ;
    _sgErrDate -> Cells[1][0] = "Err No"    ;
    _sgErrDate -> Cells[2][0] = "Err Name"  ;
    _sgErrDate -> Cells[3][0] = "Time"      ;
    _sgErrDate -> Cells[4][0] = "Err Msg"   ;
    _sgErrDate -> Cells[5][0] = "Lot Id"    ;

    TDateTime Time ;

    for(int r = 0 ; r < iErrCnt ; r++) {
        _sgErrDate -> Cells[0][r+1] = r+1 ;
        _sgErrDate -> Cells[1][r+1] = Datas[r].iErrNo   ;
        _sgErrDate -> Cells[2][r+1] = Datas[r].sErrName ;
        Time.Val = Datas[r].dSttTime ;
        _sgErrDate -> Cells[3][r+1] = Time.FormatString("yyyy-mm-dd hh:nn:ss");
        _sgErrDate -> Cells[4][r+1] = Datas[r].sErrMsg  ;
        _sgErrDate -> Cells[5][r+1] = Datas[r].sLotId   ;
    }

    delete [] Datas ;
}

void CErrData::Update(AnsiString _sCrntLotNo)
{
    //Err Log
    static bool bPreErr = false ;
    bool isErr = EM_IsErr() ;
    if(isErr && !bPreErr) {
        SetErr(EM_GetLastErr() , EM_GetErrName(EM_GetLastErr()) , EM_GetErrMsg(EM_GetLastErr()) , _sCrntLotNo  );
    }
    bPreErr = isErr ;
}

CLotData::CLotData()
{

}

CLotData::~CLotData()
{
}

void CLotData::Init()
{
    LoadSaveLotIni(true);
}

void CLotData::Close()
{
    LoadSaveLotIni(false);
}

bool CLotData::SaveArrayData(TData & _tData , int _iMgzNo , int _iSlotNo , CArray & _rData)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    TDateTime  tDateTime ;

    tDateTime.Val = _tData.dSttTime ;

    sPath = LOT_FOLDER + tDateTime.FormatString("yyyymmdd\\") + _tData.sLotId + ".ini" ;

    //기존에 있던것들 지우기.
    UserFile.ClearDirDate(LOT_FOLDER , Now() - 30 * 3); //90일.


    //작업 메거진 카운트 갱신.
    int iMgzCnt = 0;
    UserINI.Load(sPath.c_str() , "ETC" , "MgzCnt"  , iMgzCnt);
    if(_iMgzNo+1 >= iMgzCnt) UserINI.Save(sPath.c_str() , "ETC" , "MgzCnt" , _iMgzNo+1);

    //맥스 슬랏 카운트 갱신.
    int iSlotCnt = 0;
    UserINI.Load(sPath.c_str() , "ETC" , "SlotCnt" , iSlotCnt);
    if(_iSlotNo+1 >= iSlotCnt) UserINI.Save(sPath.c_str() , "ETC" , "SlotCnt" , _iSlotNo+1);

    UserINI.Save(sPath.c_str() , "ETC" , "ColCnt"      , _rData.GetMaxCol());
    UserINI.Save(sPath.c_str() , "ETC" , "RowCnt"      , _rData.GetMaxRow());


    //랏 정보 저장.
    UserINI.Save(sPath.c_str() , "Data" , "sLotId    " , _tData.sLotId    );
    UserINI.Save(sPath.c_str() , "Data" , "iWorkCnt  " , _tData.iWorkCnt  );
//    UserINI.Save(sPath.c_str() , "Data" , "iFailCnt  " , _tData.iFailCnt  );
//    UserINI.Save(sPath.c_str() , "Data" , "iRslt1Cnt " , _tData.iRslt1Cnt );
//    UserINI.Save(sPath.c_str() , "Data" , "iRslt2Cnt " , _tData.iRslt2Cnt );
//    UserINI.Save(sPath.c_str() , "Data" , "iRslt3Cnt " , _tData.iRslt3Cnt );
//    UserINI.Save(sPath.c_str() , "Data" , "iRslt4Cnt " , _tData.iRslt4Cnt );
//    UserINI.Save(sPath.c_str() , "Data" , "iRslt5Cnt " , _tData.iRslt5Cnt );
//    UserINI.Save(sPath.c_str() , "Data" , "iRslt6Cnt " , _tData.iRslt6Cnt );
//    UserINI.Save(sPath.c_str() , "Data" , "iRslt7Cnt " , _tData.iRslt7Cnt );
//    UserINI.Save(sPath.c_str() , "Data" , "iRslt8Cnt " , _tData.iRslt8Cnt );
//    UserINI.Save(sPath.c_str() , "Data" , "iPck1VHCnt" , _tData.iPck1VHCnt);
//    UserINI.Save(sPath.c_str() , "Data" , "iPck2VHCnt" , _tData.iPck2VHCnt);
//    UserINI.Save(sPath.c_str() , "Data" , "iPck3VHCnt" , _tData.iPck3VHCnt);
//    UserINI.Save(sPath.c_str() , "Data" , "iPck4VHCnt" , _tData.iPck4VHCnt);
    UserINI.Save(sPath.c_str() , "Data" , "dSttTime  " , _tData.dSttTime  );
    UserINI.Save(sPath.c_str() , "Data" , "dEndTime  " , _tData.dEndTime  );
    UserINI.Save(sPath.c_str() , "Data" , "dWorkTime " , _tData.dWorkTime );
    UserINI.Save(sPath.c_str() , "Data" , "dErrTime  " , _tData.dErrTime  );
    UserINI.Save(sPath.c_str() , "Data" , "dStopTime " , _tData.dStopTime );
    UserINI.Save(sPath.c_str() , "Data" , "dTotalTime" , _tData.dTotalTime);
//    UserINI.Save(sPath.c_str() , "Data" , "iUPEH     " , _tData.iUPEH     );
    UserINI.Save(sPath.c_str() , "Data" , "sJobFile  " , _tData.sJobFile  );


    //맵 저장.
    AnsiString sRowData ;
    AnsiString sCaption ;
    AnsiString sIndex   ;
    AnsiString sTemp    ;
    sCaption = AnsiString(_iMgzNo) + AnsiString("_") + _iSlotNo ;
    for(int r = 0 ; r < _rData.GetMaxRow() ; r++) {
        sRowData = "" ;
        sIndex.sprintf("Row%02d", r ) ;
        for(int c = 0 ; c < _rData.GetMaxCol() ; c++) {
            //sTemp.sprintf("r=%02d,c=%02d", r,c);
            //Trace("Aray", sTemp.c_str());
            sTemp.sprintf("%02d",(int)_rData.GetStat(r,c)) ;
            if(c<_rData.GetMaxCol()-1) sTemp += "_" ;
            sRowData += sTemp ;
        }
        UserINI.Save(sPath.c_str() , sCaption , sIndex , sRowData);
    }
}

bool CLotData::LoadSaveLotIni(bool _bLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    sPath = LOT_FOLDER + "LotInfo.ini" ;

    if(_bLoad) {
        UserINI.Load(sPath.c_str() , "Data" , "sLotId    " , m_tData.sLotId    );
        UserINI.Load(sPath.c_str() , "Data" , "iWorkCnt  " , m_tData.iWorkCnt  );
//        UserINI.Load(sPath.c_str() , "Data" , "iFailCnt  " , m_tData.iFailCnt  );
//        UserINI.Load(sPath.c_str() , "Data" , "iRslt1Cnt " , m_tData.iRslt1Cnt );
//        UserINI.Load(sPath.c_str() , "Data" , "iRslt2Cnt " , m_tData.iRslt2Cnt );
//        UserINI.Load(sPath.c_str() , "Data" , "iRslt3Cnt " , m_tData.iRslt3Cnt );
//        UserINI.Load(sPath.c_str() , "Data" , "iRslt4Cnt " , m_tData.iRslt4Cnt );
//        UserINI.Load(sPath.c_str() , "Data" , "iRslt5Cnt " , m_tData.iRslt5Cnt );
//        UserINI.Load(sPath.c_str() , "Data" , "iRslt6Cnt " , m_tData.iRslt6Cnt );
//        UserINI.Load(sPath.c_str() , "Data" , "iRslt7Cnt " , m_tData.iRslt7Cnt );
//        UserINI.Load(sPath.c_str() , "Data" , "iRslt8Cnt " , m_tData.iRslt8Cnt );
//        UserINI.Load(sPath.c_str() , "Data" , "iPck1VHCnt" , m_tData.iPck1VHCnt);
//        UserINI.Load(sPath.c_str() , "Data" , "iPck2VHCnt" , m_tData.iPck2VHCnt);
//        UserINI.Load(sPath.c_str() , "Data" , "iPck3VHCnt" , m_tData.iPck3VHCnt);
//        UserINI.Load(sPath.c_str() , "Data" , "iPck4VHCnt" , m_tData.iPck4VHCnt);
        UserINI.Load(sPath.c_str() , "Data" , "dSttTime  " , m_tData.dSttTime  );
        UserINI.Load(sPath.c_str() , "Data" , "dEndTime  " , m_tData.dEndTime  );
        UserINI.Load(sPath.c_str() , "Data" , "dWorkTime " , m_tData.dWorkTime );
        UserINI.Load(sPath.c_str() , "Data" , "dErrTime  " , m_tData.dErrTime  );
        UserINI.Load(sPath.c_str() , "Data" , "dStopTime " , m_tData.dStopTime );
        UserINI.Load(sPath.c_str() , "Data" , "dTotalTime" , m_tData.dTotalTime);
//        UserINI.Load(sPath.c_str() , "Data" , "iUPEH     " , m_tData.iUPEH     );
        UserINI.Load(sPath.c_str() , "Data" , "sJobFile  " , m_tData.sJobFile  );
    }
    else {
        UserINI.Save(sPath.c_str() , "Data" , "sLotId    " , m_tData.sLotId    );
        UserINI.Save(sPath.c_str() , "Data" , "iWorkCnt  " , m_tData.iWorkCnt  );
//        UserINI.Save(sPath.c_str() , "Data" , "iFailCnt  " , m_tData.iFailCnt  );
//        UserINI.Save(sPath.c_str() , "Data" , "iRslt1Cnt " , m_tData.iRslt1Cnt );
//        UserINI.Save(sPath.c_str() , "Data" , "iRslt2Cnt " , m_tData.iRslt2Cnt );
//        UserINI.Save(sPath.c_str() , "Data" , "iRslt3Cnt " , m_tData.iRslt3Cnt );
//        UserINI.Save(sPath.c_str() , "Data" , "iRslt4Cnt " , m_tData.iRslt4Cnt );
//        UserINI.Save(sPath.c_str() , "Data" , "iRslt5Cnt " , m_tData.iRslt5Cnt );
//        UserINI.Save(sPath.c_str() , "Data" , "iRslt6Cnt " , m_tData.iRslt6Cnt );
//        UserINI.Save(sPath.c_str() , "Data" , "iRslt7Cnt " , m_tData.iRslt7Cnt );
//        UserINI.Save(sPath.c_str() , "Data" , "iRslt8Cnt " , m_tData.iRslt8Cnt );
//        UserINI.Save(sPath.c_str() , "Data" , "iPck1VHCnt" , m_tData.iPck1VHCnt);
//        UserINI.Save(sPath.c_str() , "Data" , "iPck2VHCnt" , m_tData.iPck2VHCnt);
//        UserINI.Save(sPath.c_str() , "Data" , "iPck3VHCnt" , m_tData.iPck3VHCnt);
//        UserINI.Save(sPath.c_str() , "Data" , "iPck4VHCnt" , m_tData.iPck4VHCnt);
        UserINI.Save(sPath.c_str() , "Data" , "dSttTime  " , m_tData.dSttTime  );
        UserINI.Save(sPath.c_str() , "Data" , "dEndTime  " , m_tData.dEndTime  );
        UserINI.Save(sPath.c_str() , "Data" , "dWorkTime " , m_tData.dWorkTime );
        UserINI.Save(sPath.c_str() , "Data" , "dErrTime  " , m_tData.dErrTime  );
        UserINI.Save(sPath.c_str() , "Data" , "dStopTime " , m_tData.dStopTime );
        UserINI.Save(sPath.c_str() , "Data" , "dTotalTime" , m_tData.dTotalTime);
//        UserINI.Save(sPath.c_str() , "Data" , "iUPEH     " , m_tData.iUPEH     );
        UserINI.Save(sPath.c_str() , "Data" , "sJobFile  " , m_tData.sJobFile  );
    }

}

void CLotData::ClearData()
{
    m_tData.Clear();
    //memset(&m_tData , 0 , sizeof(TData)); 이방식으로 하면 String 에서 메모리 누수 생김.

}


void CLotData::DispDateList(TStringGrid * _sgDateList)
{
    //StringGrid를 Update한다.
    UserFile.GridSearchDir(LOT_FOLDER , _sgDateList , 1 , true);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
}

void CLotData::DispLotList(AnsiString _sDate , TStringGrid * _sgLotList)
{
    //Local Var.
    int        iLotCnt = 0 ;
    TSearchRec sr          ;
    AnsiString sPath = LOT_FOLDER + _sDate ;
    AnsiString sIniPath ;
    AnsiString sSerchFile = sPath + "\\*.ini" ;
    TUserINI   UserINI;
    TData      tData ;
    TDateTime  tTime ;

    //LotCount확인
    if (FindFirst(sSerchFile , faAnyFile , sr) == 0) {
        do {
            if ((sr.Attr & faDirectory) != sr.Attr) {
                iLotCnt++;
            }
        } while (FindNext(sr) == 0);
    }
    FindClose(sr);

    //sr.

    _sgLotList -> RowCount = iLotCnt + 1;
    _sgLotList -> ColCount  = 22 ;
    _sgLotList -> FixedCols = 1 ;
    _sgLotList -> FixedRows = 1 ;

    //VT_Dft , VT_Dim ,VSN ,VT_Blk ,
    //                 OST ,VT_Vh
//    _sgLotList -> Cells[0 ][0] = "No"       ;             _sgLotList -> ColWidths[0 ] = _sgLotList -> Cells[0 ][0].Length() * 15 ;
//    _sgLotList -> Cells[1 ][0] = "LotId"    ;             _sgLotList -> ColWidths[1 ] = _sgLotList -> Cells[1 ][0].Length() * 25 ;
//    _sgLotList -> Cells[2 ][0] = "Work"     ;             _sgLotList -> ColWidths[2 ] = _sgLotList -> Cells[2 ][0].Length() * 15 ;
//    _sgLotList -> Cells[3 ][0] = "Fail"     ;             _sgLotList -> ColWidths[3 ] = _sgLotList -> Cells[3 ][0].Length() * 15 ;
  //_sgLotList -> Cells[4 ][0] = OM.GetBinName(csRslt1);  _sgLotList -> ColWidths[4 ] = _sgLotList -> Cells[4 ][0].Length() * 15 ;
  //_sgLotList -> Cells[5 ][0] = OM.GetBinName(csRslt2);  _sgLotList -> ColWidths[5 ] = _sgLotList -> Cells[5 ][0].Length() * 15 ;
  //_sgLotList -> Cells[6 ][0] = OM.GetBinName(csRslt3);  _sgLotList -> ColWidths[6 ] = _sgLotList -> Cells[6 ][0].Length() * 15 ;
  //_sgLotList -> Cells[7 ][0] = OM.GetBinName(csRslt4);  _sgLotList -> ColWidths[7 ] = _sgLotList -> Cells[7 ][0].Length() * 15 ;
  //_sgLotList -> Cells[8 ][0] = OM.GetBinName(csRslt5);  _sgLotList -> ColWidths[8 ] = _sgLotList -> Cells[8 ][0].Length() * 15 ;
  //_sgLotList -> Cells[9 ][0] = OM.GetBinName(csRslt6);  _sgLotList -> ColWidths[9 ] = _sgLotList -> Cells[9 ][0].Length() * 15 ;
  //_sgLotList -> Cells[10][0] = OM.GetBinName(csRslt7);  _sgLotList -> ColWidths[10] = _sgLotList -> Cells[10][0].Length() * 15 ;
  //_sgLotList -> Cells[11][0] = OM.GetBinName(csRslt8);  _sgLotList -> ColWidths[11] = _sgLotList -> Cells[11][0].Length() * 15 ;

    //조현호 과장 요청 사항... 검사 순서대로 보이게.
//    _sgLotList -> Cells[4 ][0] = OM.GetBinName(csRslt7);  _sgLotList -> ColWidths[4 ] = _sgLotList -> Cells[4 ][0].Length() * 15 ;
//    _sgLotList -> Cells[5 ][0] = OM.GetBinName(csRslt1);  _sgLotList -> ColWidths[5 ] = _sgLotList -> Cells[5 ][0].Length() * 15 ;
//    _sgLotList -> Cells[6 ][0] = OM.GetBinName(csRslt2);  _sgLotList -> ColWidths[6 ] = _sgLotList -> Cells[6 ][0].Length() * 15 ;
//    _sgLotList -> Cells[7 ][0] = OM.GetBinName(csRslt4);  _sgLotList -> ColWidths[7 ] = _sgLotList -> Cells[7 ][0].Length() * 15 ;
//    _sgLotList -> Cells[8 ][0] = OM.GetBinName(csRslt8);  _sgLotList -> ColWidths[8 ] = _sgLotList -> Cells[8 ][0].Length() * 15 ;
//    _sgLotList -> Cells[9 ][0] = OM.GetBinName(csRslt3);  _sgLotList -> ColWidths[9 ] = _sgLotList -> Cells[9 ][0].Length() * 15 ;
//    _sgLotList -> Cells[10][0] = OM.GetBinName(csRslt5);  _sgLotList -> ColWidths[10] = _sgLotList -> Cells[10][0].Length() * 15 ;
//    _sgLotList -> Cells[11][0] = OM.GetBinName(csRslt6);  _sgLotList -> ColWidths[11] = _sgLotList -> Cells[11][0].Length() * 15 ;
//
//    _sgLotList -> Cells[12][0] = "Pk1 VH"  ;              _sgLotList -> ColWidths[12] = _sgLotList -> Cells[12][0].Length() * 15 ;
//    _sgLotList -> Cells[13][0] = "Pk2 VH"  ;              _sgLotList -> ColWidths[13] = _sgLotList -> Cells[13][0].Length() * 15 ;
//    _sgLotList -> Cells[14][0] = "Pk3 VH"  ;              _sgLotList -> ColWidths[14] = _sgLotList -> Cells[14][0].Length() * 15 ;
//    _sgLotList -> Cells[15][0] = "Pk4 VH"  ;              _sgLotList -> ColWidths[15] = _sgLotList -> Cells[15][0].Length() * 15 ;
//
//    _sgLotList -> Cells[16][0] = "UPEH"     ;             _sgLotList -> ColWidths[16] = _sgLotList -> Cells[16][0].Length() * 15 ;
//    _sgLotList -> Cells[17][0] = "Start"    ;             _sgLotList -> ColWidths[17] = _sgLotList -> Cells[17][0].Length() * 15 ;
//    _sgLotList -> Cells[18][0] = "End  "    ;             _sgLotList -> ColWidths[18] = _sgLotList -> Cells[18][0].Length() * 15 ;
//    _sgLotList -> Cells[19][0] = "WorkTime" ;             _sgLotList -> ColWidths[19] = _sgLotList -> Cells[19][0].Length() * 15 ;
//    _sgLotList -> Cells[20][0] = "TotalTime";             _sgLotList -> ColWidths[20] = _sgLotList -> Cells[20][0].Length() * 15 ;
//    _sgLotList -> Cells[21][0] = "JobFile"  ;             _sgLotList -> ColWidths[21] = _sgLotList -> Cells[21][0].Length() * 15 ;

                                                                                                          
    //LotData                                                                                             
    iLotCnt = 0 ;
    if (FindFirst(sSerchFile , faAnyFile , sr) == 0) {
        do {
            if ((sr.Attr & faDirectory) != sr.Attr) {
                sIniPath = sPath + "\\" + sr.Name  ;

                //랏 정보 블러오기.
                                                                                           _sgLotList -> Cells[ 0][iLotCnt+1] = iLotCnt+1       ;
                UserINI.Load(sIniPath.c_str() , "Data" , "sLotId    " , tData.sLotId    ); _sgLotList -> Cells[ 1][iLotCnt+1] = tData.sLotId    ;
                UserINI.Load(sIniPath.c_str() , "Data" , "iWorkCnt  " , tData.iWorkCnt  ); _sgLotList -> Cells[ 2][iLotCnt+1] = tData.iWorkCnt  ;
//                UserINI.Load(sIniPath.c_str() , "Data" , "iFailCnt  " , tData.iFailCnt  ); _sgLotList -> Cells[ 3][iLotCnt+1] = tData.iFailCnt  ;
//                UserINI.Load(sIniPath.c_str() , "Data" , "iRslt7Cnt " , tData.iRslt7Cnt ); _sgLotList -> Cells[ 4][iLotCnt+1] = tData.iRslt7Cnt ;
//                UserINI.Load(sIniPath.c_str() , "Data" , "iRslt1Cnt " , tData.iRslt1Cnt ); _sgLotList -> Cells[ 5][iLotCnt+1] = tData.iRslt1Cnt ;
//                UserINI.Load(sIniPath.c_str() , "Data" , "iRslt2Cnt " , tData.iRslt2Cnt ); _sgLotList -> Cells[ 6][iLotCnt+1] = tData.iRslt2Cnt ;
//                UserINI.Load(sIniPath.c_str() , "Data" , "iRslt4Cnt " , tData.iRslt4Cnt ); _sgLotList -> Cells[ 7][iLotCnt+1] = tData.iRslt4Cnt ;
//                UserINI.Load(sIniPath.c_str() , "Data" , "iRslt8Cnt " , tData.iRslt8Cnt ); _sgLotList -> Cells[ 8][iLotCnt+1] = tData.iRslt8Cnt ;
//                UserINI.Load(sIniPath.c_str() , "Data" , "iRslt3Cnt " , tData.iRslt3Cnt ); _sgLotList -> Cells[ 9][iLotCnt+1] = tData.iRslt3Cnt ;
//                UserINI.Load(sIniPath.c_str() , "Data" , "iRslt5Cnt " , tData.iRslt5Cnt ); _sgLotList -> Cells[10][iLotCnt+1] = tData.iRslt5Cnt ;
//                UserINI.Load(sIniPath.c_str() , "Data" , "iRslt6Cnt " , tData.iRslt6Cnt ); _sgLotList -> Cells[11][iLotCnt+1] = tData.iRslt6Cnt ;
//                UserINI.Load(sIniPath.c_str() , "Data" , "iPck1VHCnt" , tData.iPck1VHCnt); _sgLotList -> Cells[12][iLotCnt+1] = tData.iPck1VHCnt ;
//                UserINI.Load(sIniPath.c_str() , "Data" , "iPck2VHCnt" , tData.iPck2VHCnt); _sgLotList -> Cells[13][iLotCnt+1] = tData.iPck2VHCnt ;
//                UserINI.Load(sIniPath.c_str() , "Data" , "iPck3VHCnt" , tData.iPck3VHCnt); _sgLotList -> Cells[14][iLotCnt+1] = tData.iPck3VHCnt ;
//                UserINI.Load(sIniPath.c_str() , "Data" , "iPck4VHCnt" , tData.iPck4VHCnt); _sgLotList -> Cells[15][iLotCnt+1] = tData.iPck4VHCnt ;
//                UserINI.Load(sIniPath.c_str() , "Data" , "iUPEH     " , tData.iUPEH     ); _sgLotList -> Cells[16][iLotCnt+1] = tData.iUPEH     ;
                UserINI.Load(sIniPath.c_str() , "Data" , "dSttTime  " , tData.dSttTime  ); tTime.Val = tData.dSttTime  ; _sgLotList -> Cells[17][iLotCnt+1] = tTime.FormatString("hh:nn:ss") ;
                UserINI.Load(sIniPath.c_str() , "Data" , "dEndTime  " , tData.dEndTime  ); tTime.Val = tData.dEndTime  ; _sgLotList -> Cells[18][iLotCnt+1] = tTime.FormatString("hh:nn:ss") ;
                UserINI.Load(sIniPath.c_str() , "Data" , "dWorkTime " , tData.dWorkTime ); tTime.Val = tData.dWorkTime ; _sgLotList -> Cells[19][iLotCnt+1] = tTime.FormatString("hh:nn:ss") ;
                UserINI.Load(sIniPath.c_str() , "Data" , "dTotalTime" , tData.dTotalTime); tTime.Val = tData.dTotalTime; _sgLotList -> Cells[20][iLotCnt+1] = tTime.FormatString("hh:nn:ss") ;
                UserINI.Load(sIniPath.c_str() , "Data" , "sJobFile  " , tData.sJobFile  ); _sgLotList -> Cells[21][iLotCnt+1] = tData.sJobFile   ;
                UserINI.Load(sIniPath.c_str() , "Data" , "dErrTime  " , tData.dErrTime  ); //디스플레이 안함.
                UserINI.Load(sIniPath.c_str() , "Data" , "dStopTime " , tData.dStopTime ); //디스플레이 안함.
                iLotCnt++;
            }
        } while (FindNext(sr) == 0);
    }
    FindClose(sr);

    AnsiString sTemp;
    TDateTime  tCmprTime1 ;
    TDateTime  tCmprTime2 ;

    for (int i = 1 ; i < _sgLotList -> RowCount - 1 ; i++){
        if (_sgLotList -> Cells[17][i] == "") continue;

        for(int j = i + 1 ; j < _sgLotList -> RowCount ; j++){
            if (_sgLotList -> Cells[17][j] == "") continue;
                tCmprTime1 = _sgLotList -> Cells[17][i] ;
                tCmprTime2 = _sgLotList -> Cells[17][j] ;
                tCmprTime1.FormatString("fffff.ffffffffff") ;
                tCmprTime2.FormatString("fffff.ffffffffff") ;
            if (tCmprTime1 > tCmprTime2){
                for(int x = 1 ; x < _sgLotList -> ColCount ; x++){
                    _sgLotList->Cells[x][_sgLotList->RowCount] = _sgLotList->Cells[x][i                   ];
                    _sgLotList->Cells[x][i                   ] = _sgLotList->Cells[x][j                   ];
                    _sgLotList->Cells[x][j                   ] = _sgLotList->Cells[x][_sgLotList->RowCount];
                }
            }
        }
    }

}

void CLotData::DispMgzAndSlotList(AnsiString _sDate , AnsiString _sLotId , TStringGrid * _sgMgz , TStringGrid * _sgSlot )
{

    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    int iMgzCnt ;
    int iSlotCnt ;

    sPath = LOT_FOLDER + _sDate + "\\" + _sLotId + ".ini" ;

    //작업 메거진 카운트 갱신.
    UserINI.Load(sPath.c_str() , "ETC" , "MgzCnt"  , iMgzCnt );
    UserINI.Load(sPath.c_str() , "ETC" , "SlotCnt" , iSlotCnt);

    _sgMgz  -> RowCount = iMgzCnt  ;
    _sgSlot -> RowCount = iSlotCnt ;

    for(int m = 0 ; m < iMgzCnt ; m++) {
        _sgMgz -> Cells[0][m] = m ;
        _sgMgz -> Cells[1][m] = m ;
    }

    for(int s = 0 ; s < iSlotCnt ; s++) {
        _sgSlot -> Cells[0][s] = s ;
        _sgSlot -> Cells[1][s] = s ;
    }

}

void CLotData::DispStripMap(AnsiString _sDate , AnsiString _sLotId , int _iMgz , int _iSlot , TStringGrid * _sgMap)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    int iColCnt ;
    int iRowCnt ;


    sPath = LOT_FOLDER + _sDate + "//" + _sLotId + ".ini" ;

    //작업 메거진 카운트 갱신.
    UserINI.Load(sPath.c_str() , "ETC" , "ColCnt" , iColCnt);
    UserINI.Load(sPath.c_str() , "ETC" , "RowCnt" , iRowCnt);

    _sgMap -> RowCount = iRowCnt +1;
    _sgMap -> ColCount = iColCnt +1;

    _sgMap -> FixedCols = 1 ;
    _sgMap -> FixedRows = 1 ;


    //맵 저장.
    AnsiString sRowData ;
    AnsiString sCaption ;
    AnsiString sIndex   ;
    AnsiString sData    ;
    sCaption = AnsiString(_iMgz) + AnsiString("_") + _iSlot ;
    for(int r = 0 ; r < iRowCnt ; r++) {
        _sgMap -> Cells[0][r+1] = r ;
        sIndex.sprintf("Row%02d", r ) ;
        UserINI.Load(sPath.c_str() , sCaption , sIndex , sRowData);
        for(int c = 0 ; c < iColCnt ; c++) {
            if(r==0)_sgMap -> Cells[c+1][0] = c ;
            sData = sRowData.SubString(1,2) ;
            sRowData.Delete(1,3) ;
            _sgMap -> Cells[c+1][r+1] = sData ;
        }

    }
}
void CLotData::DispLotInfo(TStringGrid * _sgLotInfo) //오퍼레이션 창용.
{
    if(!_sgLotInfo )return ;

    TDateTime tDateTime ;

    _sgLotInfo -> ColCount = 2 ;
    _sgLotInfo -> RowCount = 6 ;


    double dYield = m_tData.iWorkCnt ? (m_tData.iWorkCnt - m_tData.iFailCnt)*100 / (double)m_tData.iWorkCnt : 0.0;
    String sYield = sYield.sprintf("%.2f%%",dYield);


    //_sgLotInfo -> Cells[0][0 ] = "LotId    "  ;                                       _sgLotInfo -> Cells[1][0 ] = m_tData.sLotId    ;
    _sgLotInfo -> Cells[0][0] = "WorkCnt  "  ;                                       _sgLotInfo -> Cells[1][0] = m_tData.iWorkCnt  ;
//    _sgLotInfo -> Cells[0][1] = "FailCnt  "  ;                                       _sgLotInfo -> Cells[1][1] = m_tData.iFailCnt  ;
//    _sgLotInfo -> Cells[0][2] = "UPEH     "  ;                                       _sgLotInfo -> Cells[1][2] = m_tData.iUPEH     ;
    _sgLotInfo -> Cells[0][1] = "SttTime  "  ; tDateTime.Val = m_tData.dSttTime   ;  _sgLotInfo -> Cells[1][1] = tDateTime.FormatString("hh:nn:ss");
    _sgLotInfo -> Cells[0][2] = "EndTime  "  ; tDateTime.Val = m_tData.dEndTime   ;  _sgLotInfo -> Cells[1][2] = tDateTime.FormatString("hh:nn:ss");
    _sgLotInfo -> Cells[0][3] = "WorkTime "  ; tDateTime.Val = m_tData.dWorkTime  ;  _sgLotInfo -> Cells[1][3] = tDateTime.FormatString("hh:nn:ss");
    _sgLotInfo -> Cells[0][4] = "TotalTime"  ; tDateTime.Val = m_tData.dTotalTime ;  _sgLotInfo -> Cells[1][4] = tDateTime.FormatString("hh:nn:ss");
    _sgLotInfo -> Cells[0][5] = "CycleTime"  ;                                       _sgLotInfo -> Cells[1][5] = m_tData.dTickTime ;
//    _sgLotInfo -> Cells[0][8] = "Yield    "  ;                                       _sgLotInfo -> Cells[1][8] = sYield;

    //_sgLotInfo -> Cells[0][8 ] = "JobFile  "  ;                                       _sgLotInfo -> Cells[1][8 ] = m_tData.sJobFile  ;
    //_sgLotInfo -> Cells[0][8 ] = "ErrTime  "  ; tDateTime.Val = m_tData.dErrTime   ;  _sgLotInfo -> Cells[1][9 ] = tDateTime.FormatString("hh:nn:ss");
    //_sgLotInfo -> Cells[0][9 ] = "StopTime "  ; tDateTime.Val = m_tData.dStopTime  ;  _sgLotInfo -> Cells[1][10] = tDateTime.FormatString("hh:nn:ss");
}



void CLotData::Update(EN_SEQ_STAT Stat)
{

    //타임 인포 갱신.
    TDateTime  tDateTime = Now();
    static double dPreTime = tDateTime.Val ;
    double dCrntTime = Now().Val ;
    double dCycleTime = dCrntTime - dPreTime ;
    switch(Stat) {
        case ssInit     : m_tData.dStopTime += dCycleTime ; break ;
        case ssWarning  : m_tData.dStopTime += dCycleTime ; break ;
        case ssError    : m_tData.dErrTime  += dCycleTime ; break ;
        case ssRunning  : m_tData.dWorkTime += dCycleTime ; break ;
        case ssStop     : m_tData.dStopTime += dCycleTime ; break ;
        case ssMaint    : m_tData.dStopTime += dCycleTime ; break ;
        case ssRunWarn  : m_tData.dWorkTime += dCycleTime ; break ;
        case ssWorkEnd  : m_tData.dStopTime += dCycleTime ; break ;
    }
    m_tData.dTotalTime += dCycleTime ;
    dPreTime = dCrntTime ;

    //잡파일 체인지 할때 어레이 동적 할당 다시 하기때문에 돌려줘야 한다.
    //이장비는 툴만 보기 때문에 상관 없지만 그냥 이렇게 내둔다.
    if(Stat == ssStop ) return ;
    //if(Stat != ssRunning && Stat != ssRunWarn) return ;
    ///////////////////////매우 중요.............

    AnsiString sLotId ;
    sLotId = LOT.GetLotNo() ; //LDR까지 스캔해보고 있으면 미리 랏오픈을 한다.
    if(sLotId != m_tData.sLotId && sLotId != "") { //어맛! 새로운 랏.
        m_tData.Clear();
        //memset(&m_tData , 0 , sizeof(TData)); 메모리 누수.

        m_tData.sLotId    = LOT.GetLotNo() ;
        m_tData.dSttTime  = dCrntTime ;
    }

//    static int iPreRslt1 = DM.ARAY[riSRT].GetCntStat(csRslt1);
//    static int iPreRslt2 = DM.ARAY[riSRT].GetCntStat(csRslt2);
//    static int iPreRslt3 = DM.ARAY[riSRT].GetCntStat(csRslt3);
//    static int iPreRslt4 = DM.ARAY[riSRT].GetCntStat(csRslt4);
//    static int iPreRslt5 = DM.ARAY[riSRT].GetCntStat(csRslt5);
//    static int iPreRslt6 = DM.ARAY[riSRT].GetCntStat(csRslt6);
//    static int iPreRslt7 = DM.ARAY[riSRT].GetCntStat(csRslt7);
//    static int iPreRslt8 = DM.ARAY[riSRT].GetCntStat(csRslt8);
//    static int iPreGood  = DM.ARAY[riSRT].GetCntStat(csGood );

//    int iAddCntRslt1 = DM.ARAY[riSRT].GetCntStat(csRslt1)-iPreRslt1 ;
//    int iAddCntRslt2 = DM.ARAY[riSRT].GetCntStat(csRslt2)-iPreRslt2 ;
//    int iAddCntRslt3 = DM.ARAY[riSRT].GetCntStat(csRslt3)-iPreRslt3 ;
//    int iAddCntRslt4 = DM.ARAY[riSRT].GetCntStat(csRslt4)-iPreRslt4 ;
//    int iAddCntRslt5 = DM.ARAY[riSRT].GetCntStat(csRslt5)-iPreRslt5 ;
//    int iAddCntRslt6 = DM.ARAY[riSRT].GetCntStat(csRslt6)-iPreRslt6 ;
//    int iAddCntRslt7 = DM.ARAY[riSRT].GetCntStat(csRslt7)-iPreRslt7 ;
//    int iAddCntRslt8 = DM.ARAY[riSRT].GetCntStat(csRslt8)-iPreRslt8 ;
//    int iAddCntGood  = DM.ARAY[riSRT].GetCntStat(csGood )-iPreGood  ;
//
    const int iSlotNo = 0 ;
    const int iMgzNo  = 0 ;

    m_tData.dEndTime  = dCrntTime ;
    m_tData.iUPEH     = m_tData.dTotalTime == 0 ? 0.0 : m_tData.iWorkCnt  / (m_tData.dTotalTime * 24) ;
    m_tData.sJobFile  = OM.GetCrntDev() ;

//    bool bNeedSave = false ;
//    if(iAddCntRslt1 > 0) {
//        m_tData.iWorkCnt  += iAddCntRslt1;
//        m_tData.iRslt1Cnt += iAddCntRslt1;
//        m_tData.iFailCnt  += iAddCntRslt1;
//        bNeedSave = true ;
//
//    }
//    if(iAddCntRslt2 > 0) {
//        m_tData.iWorkCnt  += iAddCntRslt2;
//        m_tData.iRslt2Cnt += iAddCntRslt2;
//        m_tData.iFailCnt  += iAddCntRslt2;
//        bNeedSave = true ;
//    }
//    if(iAddCntRslt3 > 0) {
//        m_tData.iWorkCnt  += iAddCntRslt3;
//        m_tData.iRslt3Cnt += iAddCntRslt3;
//        m_tData.iFailCnt  += iAddCntRslt3;
//        bNeedSave = true ;
//    }
//    if(iAddCntRslt4 > 0) {
//        m_tData.iWorkCnt  += iAddCntRslt4;
//        m_tData.iRslt4Cnt += iAddCntRslt4;
//        m_tData.iFailCnt  += iAddCntRslt4;
//        bNeedSave = true ;
//    }
//    if(iAddCntRslt5 > 0) {
//        m_tData.iWorkCnt  += iAddCntRslt5;
//        m_tData.iRslt5Cnt += iAddCntRslt5;
//        m_tData.iFailCnt  += iAddCntRslt5;
//        bNeedSave = true ;
//    }
//    if(iAddCntRslt6 > 0) {
//        m_tData.iWorkCnt  += iAddCntRslt6;
//        m_tData.iRslt6Cnt += iAddCntRslt6;
//        m_tData.iFailCnt  += iAddCntRslt6;
//        bNeedSave = true ;
//    }
//    if(iAddCntRslt7 > 0) {
//        m_tData.iWorkCnt  += iAddCntRslt7;
//        m_tData.iRslt7Cnt += iAddCntRslt7;
//        m_tData.iFailCnt  += iAddCntRslt7;
//        bNeedSave = true ;
//    }
//    if(iAddCntRslt8 > 0) {
//        m_tData.iWorkCnt  += iAddCntRslt8;
//        m_tData.iRslt8Cnt += iAddCntRslt8;
//        m_tData.iFailCnt  += iAddCntRslt8;
//        bNeedSave = true ;
//    }
//    if(iAddCntGood > 0) {
//        m_tData.iWorkCnt  += iAddCntGood;
//        bNeedSave = true ;
//    }
//
//    iPreRslt1 = DM.ARAY[riSRT].GetCntStat(csRslt1);
//    iPreRslt2 = DM.ARAY[riSRT].GetCntStat(csRslt2);
//    iPreRslt3 = DM.ARAY[riSRT].GetCntStat(csRslt3);
//    iPreRslt4 = DM.ARAY[riSRT].GetCntStat(csRslt4);
//    iPreRslt5 = DM.ARAY[riSRT].GetCntStat(csRslt5);
//    iPreRslt6 = DM.ARAY[riSRT].GetCntStat(csRslt6);
//    iPreRslt7 = DM.ARAY[riSRT].GetCntStat(csRslt7);
//    iPreRslt8 = DM.ARAY[riSRT].GetCntStat(csRslt8);
//    iPreGood  = DM.ARAY[riSRT].GetCntStat(csGood );
//
//
//    //VH 포켓별 카운팅 추가 20150330이윤승 팀장 요청.
//    static bool bPrePck1VHExist = DM.ARAY[riSRT].GetStat(0,0) == OM.CmnOptn.iVTVHBin + 1; ;
//    static bool bPrePck2VHExist = DM.ARAY[riSRT].GetStat(0,1) == OM.CmnOptn.iVTVHBin + 1; ;
//    static bool bPrePck3VHExist = DM.ARAY[riSRT].GetStat(0,2) == OM.CmnOptn.iVTVHBin + 1; ;
//    static bool bPrePck4VHExist = DM.ARAY[riSRT].GetStat(0,3) == OM.CmnOptn.iVTVHBin + 1; ;
//
//    bool bPck1VHExist = DM.ARAY[riSRT].GetStat(0,0) == OM.CmnOptn.iVTVHBin + 1;
//    bool bPck2VHExist = DM.ARAY[riSRT].GetStat(0,1) == OM.CmnOptn.iVTVHBin + 1;
//    bool bPck3VHExist = DM.ARAY[riSRT].GetStat(0,2) == OM.CmnOptn.iVTVHBin + 1;
//    bool bPck4VHExist = DM.ARAY[riSRT].GetStat(0,3) == OM.CmnOptn.iVTVHBin + 1;
//
//    if(!bPrePck1VHExist && bPck1VHExist) {m_tData.iPck1VHCnt++; bNeedSave = true ;}
//    if(!bPrePck2VHExist && bPck2VHExist) {m_tData.iPck2VHCnt++; bNeedSave = true ;}
//    if(!bPrePck3VHExist && bPck3VHExist) {m_tData.iPck3VHCnt++; bNeedSave = true ;}
//    if(!bPrePck4VHExist && bPck4VHExist) {m_tData.iPck4VHCnt++; bNeedSave = true ;}
//
//    bPrePck1VHExist = bPck1VHExist ;
//    bPrePck2VHExist = bPck2VHExist ;
//    bPrePck3VHExist = bPck3VHExist ;
//    bPrePck4VHExist = bPck4VHExist ;
//
//    if(bNeedSave)SaveArrayData(m_tData , iMgzNo , iSlotNo , DM.ARAY[DATA_ARAY]);
//
//    //TickTime표시..JS
//    AnsiString sTemp  = SRT.m_dTickTime / 1000;
//    sTemp.printf("%.2f", SRT.m_dTickTime / 1000);
//    m_tData.dTickTime = sTemp.ToDouble();


    //새로운 랏.
/*

    //칩 카운트 갱신.
    bool bExist =!DM.ARAY[DATA_ARAY].CheckAllStat(csNone) ;
    int  iRsltCnt = DM.ARAY[DATA_ARAY].GetCntStat  (csRslt1) +
                    DM.ARAY[DATA_ARAY].GetCntStat  (csRslt2) +
                    DM.ARAY[DATA_ARAY].GetCntStat  (csRslt3) +
                    DM.ARAY[DATA_ARAY].GetCntStat  (csRslt4) +
                    DM.ARAY[DATA_ARAY].GetCntStat  (csRslt5) +
                    DM.ARAY[DATA_ARAY].GetCntStat  (csRslt6) ;

    static bool bPreExist = bExist ;
    int iFailCnt = DM.ARAY[DATA_ARAY].GetCntStat  (csRslt1) +
                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt2) +
                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt3) +
                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt4) +
                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt5) +
                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt6) ;
    if(bExist && !bPreExist) { //자제 작업 끝나서 포스트버퍼에 들어옴.
        m_tData.iWorkCnt  += DM.ARAY[DATA_ARAY].GetMaxCol() * DM.ARAY[DATA_ARAY].GetMaxRow();
        m_tData.iFailCnt  += iFailCnt ;
        m_tData.dEndTime  = dCrntTime ;
        m_tData.iUPEH     = m_tData.dTotalTime == 0 ? 0.0 : m_tData.iWorkCnt  / (m_tData.dTotalTime * 24) ;
        m_tData.sJobFile  = OM.GetCrntDev() ;

        iSlotNo = DM.ARAY[DATA_ARAY].GetID().ToIntDef(0)%100 ;
        iMgzNo  = DM.ARAY[DATA_ARAY].GetID().ToIntDef(0)/100 ;
        SaveArrayData(m_tData , iMgzNo , iSlotNo , DM.ARAY[DATA_ARAY]);
    }
    bPreExist = bExist ;

    //TickTime표시..JS
    //AnsiString sTemp  = SRT.m_dTickTime / 1000;
    //sTemp.printf("%.2f", SRT.m_dTickTime / 1000);
    //m_tData.dTickTime = sTemp.ToDouble();
*/
}

void CLotData::WriteLotInfo()
{
    /*
    //칩 카운트 갱신.
    int iSlotNo ;
    int iMgzNo  ;
    double dCrntTime = Now().Val ;
    int iFailCnt = DM.ARAY[DATA_ARAY].GetCntStat  (csRslt1) +
                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt2) +
                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt3) +
                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt4) +
                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt5) +
                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt6) +
                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt7) +
                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt8) ;
    m_tData.iWorkCnt  += DM.ARAY[DATA_ARAY].GetMaxCol() * DM.ARAY[DATA_ARAY].GetMaxRow() - DM.ARAY[DATA_ARAY].GetCntStat(csEmpty); //Empty 빼준다...없는것이므로.JS
    m_tData.iFailCnt  += iFailCnt ;
    m_tData.dEndTime  = dCrntTime ;
    m_tData.iUPEH     = m_tData.dTotalTime == 0 ? 0.0 : m_tData.iWorkCnt  / (m_tData.dTotalTime * 24) ;
    m_tData.sJobFile  = OM.GetCrntDev() ;

    iSlotNo = DM.ARAY[DATA_ARAY].GetID().ToIntDef(0)%100 ;
    iMgzNo  = DM.ARAY[DATA_ARAY].GetID().ToIntDef(0)/100 ;
    SaveArrayData(m_tData , iMgzNo , iSlotNo , DM.ARAY[DATA_ARAY]);
    */
}

AnsiString CLotData::GetCrntLotNo()
{

    //if(DM.ARAY[riSRT].GetCntStat(csRslt1)

//     return "";
    return LOT.GetLotNo();
/*
         if(!DM.ARAY[riSRT].CheckAllStat(csNone)) return DM.ARAY[riSRT].GetLotNo() ;
    else if(!DM.ARAY[riZIG].CheckAllStat(csNone)) return DM.ARAY[riZIG].GetLotNo() ;
    else if(!DM.ARAY[riSG1].CheckAllStat(csNone)) return DM.ARAY[riSG1].GetLotNo() ;
    else if(!DM.ARAY[riSG2].CheckAllStat(csNone)) return DM.ARAY[riSG2].GetLotNo() ;
    else if(!DM.ARAY[riSG3].CheckAllStat(csNone)) return DM.ARAY[riSG3].GetLotNo() ;
    else if(!DM.ARAY[riSG4].CheckAllStat(csNone)) return DM.ARAY[riSG4].GetLotNo() ;
    else if(!DM.ARAY[riPRL].CheckAllStat(csNone)) return DM.ARAY[riPRL].GetLotNo() ;
    else if(!DM.ARAY[riLDR].CheckAllStat(csNone)) return DM.ARAY[riLDR].GetLotNo() ;
    else if(!DM.ARAY[riPSL].CheckAllStat(csNone)) return DM.ARAY[riPSL].GetLotNo() ;
    else if(!DM.ARAY[riPRU].CheckAllStat(csNone)) return DM.ARAY[riPRU].GetLotNo() ;
    else if(!DM.ARAY[riULD].CheckAllStat(csNone)) return DM.ARAY[riULD].GetLotNo() ;
    else if(!DM.ARAY[riPSU].CheckAllStat(csNone)) return DM.ARAY[riPSU].GetLotNo() ;
    else if(!DM.ARAY[riRJ1].CheckAllStat(csNone)) return DM.ARAY[riRJ1].GetLotNo() ;
    else if(!DM.ARAY[riRJ2].CheckAllStat(csNone)) return DM.ARAY[riRJ2].GetLotNo() ;
    else if(!DM.ARAY[riRJ3].CheckAllStat(csNone)) return DM.ARAY[riRJ3].GetLotNo() ;
    else if(!DM.ARAY[riRJ4].CheckAllStat(csNone)) return DM.ARAY[riRJ4].GetLotNo() ;

    else                                          return "";
*/
}

void CLotData::AddWorkCnt()
{
    m_tData.iWorkCnt += 1;
    SaveData(m_tData);
}

bool CLotData::SaveData(TData & _tData)
{
    TUserINI UserINI ;
    AnsiString sPath ;

    TDateTime tDateTime ;

    tDateTime.Val = _tData.dSttTime ;

    sPath = LOT_FOLDER + tDateTime.FormatString("yyyymmd\\") + _tData.sLotId + ".ini" ;

    UserFile.ClearDirDate(LOT_FOLDER, Now() - 30 * 3); // 90 Day

    UserINI.Save(sPath.c_str(), "Data" , "sLotId    ", _tData.sLotId    );
    UserINI.Save(sPath.c_str(), "Data" , "iWorkCnt  ", _tData.iWorkCnt  );
    UserINI.Save(sPath.c_str(), "Data" , "dSttTime  ", _tData.dSttTime  );
    UserINI.Save(sPath.c_str(), "Data" , "dEndTime  ", _tData.dEndTime  );
    UserINI.Save(sPath.c_str(), "Data" , "dWorkTime ", _tData.dWorkTime );
    UserINI.Save(sPath.c_str(), "Data" , "dErrTime  ", _tData.dErrTime  );
    UserINI.Save(sPath.c_str(), "Data" , "dStopTime ", _tData.dStopTime );
    UserINI.Save(sPath.c_str(), "Data" , "dTotalTime", _tData.dTotalTime);
    UserINI.Save(sPath.c_str(), "Data" , "sJobFile  ", _tData.sJobFile  );
}




//여기 부터 하면됌.
CDayData::CDayData()
{

}

CDayData::~CDayData()
{

}

void CDayData::Init()
{
    LoadSaveDayIni(true , m_tData) ;
}

void CDayData::Close()
{
    LoadSaveDayIni(false , m_tData) ;
}

void CDayData::Update(AnsiString _sCrntLotNo, EN_SEQ_STAT Stat)
{

    AnsiString sLotId ;
    TDateTime  tDateTime = Now();

    static double dPreTime = tDateTime.Val ;

    double dCrntTime = tDateTime.Val ;
    double dCycleTime = dCrntTime - dPreTime ;

    //Time Info.
    switch(Stat) {
        case ssInit     : m_tData.dStopTime += dCycleTime ; break ;
        case ssWarning  : m_tData.dStopTime += dCycleTime ; break ;
        case ssError    : m_tData.dErrTime  += dCycleTime ; break ;
        case ssRunning  : m_tData.dWorkTime += dCycleTime ; break ;
        case ssStop     : m_tData.dStopTime += dCycleTime ; break ;
        case ssMaint    : m_tData.dStopTime += dCycleTime ; break ;
        case ssRunWarn  : m_tData.dWorkTime += dCycleTime ; break ;
        case ssWorkEnd  : m_tData.dStopTime += dCycleTime ; break ;
    }
    m_tData.dTotalTime += dCycleTime ;


    //Day Log 랏 데이터는 저장 할때 랏오픈된 날짜에 저장 하지만 Day는 Lot하고 상관 없는 개념이라 그냥 바로바로 Day에 저장 한다.
    //Lot과 데이터 연동 하고 싶으면 랏업데이트에 있는 저장 패턴 복사해서 쓴다.
    bool bDateChanged = ((int)dCrntTime) != ((int)dPreTime) ; //소수점 이하는 시간 데이터.
    if(bDateChanged) { //날자 바뀜.
        ClearData();
    }
    dPreTime = dCrntTime ;


    static AnsiString sPreLotNo = _sCrntLotNo ;
    if(sPreLotNo != _sCrntLotNo /*&& Stat !=ssStop*/) { //앗 랏이 바뀌었네!!!!
        m_tData.iLotCnt++;
    }
    sPreLotNo = _sCrntLotNo ;

    m_tData.iUPEH     = m_tData.dTotalTime == 0 ? 0.0 : m_tData.iWorkCnt / (m_tData.dTotalTime * 24) ;
    m_tData.iUPH      = m_tData.dWorkTime  == 0 ? 0.0 : m_tData.iWorkCnt / (m_tData.dWorkTime  * 24) ;



    //잡파일 체인지 할때 어레이 동적 할당 다시 하기때문에 돌려줘야 한다.
    //이장비는 툴만 보기 때문에 상관 없지만 그냥 이렇게 내둔다.
    if(Stat == ssStop) return ;
    ///////////////////////매우 중요.............


    /*
    //카운팅.
    static int iPreRslt1 = DM.ARAY[DATA_ARAY].GetCntStat(csRslt1);
    static int iPreRslt2 = DM.ARAY[DATA_ARAY].GetCntStat(csRslt2);
    static int iPreRslt3 = DM.ARAY[DATA_ARAY].GetCntStat(csRslt3);
    static int iPreRslt4 = DM.ARAY[DATA_ARAY].GetCntStat(csRslt4);
    static int iPreRslt5 = DM.ARAY[DATA_ARAY].GetCntStat(csRslt5);
    static int iPreRslt6 = DM.ARAY[DATA_ARAY].GetCntStat(csRslt6);
    static int iPreRslt7 = DM.ARAY[DATA_ARAY].GetCntStat(csRslt7);
    static int iPreRslt8 = DM.ARAY[DATA_ARAY].GetCntStat(csRslt8);
    static int iPreGood  = DM.ARAY[DATA_ARAY].GetCntStat(csGood );

    int iAddCntRslt1 = DM.ARAY[DATA_ARAY].GetCntStat(csRslt1)-iPreRslt1 ;
    int iAddCntRslt2 = DM.ARAY[DATA_ARAY].GetCntStat(csRslt2)-iPreRslt2 ;
    int iAddCntRslt3 = DM.ARAY[DATA_ARAY].GetCntStat(csRslt3)-iPreRslt3 ;
    int iAddCntRslt4 = DM.ARAY[DATA_ARAY].GetCntStat(csRslt4)-iPreRslt4 ;
    int iAddCntRslt5 = DM.ARAY[DATA_ARAY].GetCntStat(csRslt5)-iPreRslt5 ;
    int iAddCntRslt6 = DM.ARAY[DATA_ARAY].GetCntStat(csRslt6)-iPreRslt6 ;
    int iAddCntRslt7 = DM.ARAY[DATA_ARAY].GetCntStat(csRslt7)-iPreRslt7 ;
    int iAddCntRslt8 = DM.ARAY[DATA_ARAY].GetCntStat(csRslt8)-iPreRslt8 ;
    int iAddCntGood  = DM.ARAY[DATA_ARAY].GetCntStat(csGood )-iPreGood  ;

    bool bNeedSave = false ;
    if(iAddCntRslt1 > 0) {
        m_tData.iWorkCnt  += iAddCntRslt1;
        m_tData.iFailCnt  += iAddCntRslt1;
        bNeedSave = true ;
    }
    if(iAddCntRslt2 > 0) {
        m_tData.iWorkCnt  += iAddCntRslt2;
        m_tData.iFailCnt  += iAddCntRslt2;
        bNeedSave = true ;
    }
    if(iAddCntRslt3 > 0) {
        m_tData.iWorkCnt  += iAddCntRslt3;
        m_tData.iFailCnt  += iAddCntRslt3;
        bNeedSave = true ;
    }
    if(iAddCntRslt4 > 0) {
        m_tData.iWorkCnt  += iAddCntRslt4;
        m_tData.iFailCnt  += iAddCntRslt4;
        bNeedSave = true ;
    }
    if(iAddCntRslt5 > 0) {
        m_tData.iWorkCnt  += iAddCntRslt5;
        m_tData.iFailCnt  += iAddCntRslt5;
        bNeedSave = true ;
    }
    if(iAddCntRslt6 > 0) {
        m_tData.iWorkCnt  += iAddCntRslt6;
        m_tData.iFailCnt  += iAddCntRslt6;
        bNeedSave = true ;
    }
    if(iAddCntRslt7 > 0) {
        m_tData.iWorkCnt  += iAddCntRslt7;
        m_tData.iFailCnt  += iAddCntRslt7;
        bNeedSave = true ;
    }
    if(iAddCntRslt8 > 0) {
        m_tData.iWorkCnt  += iAddCntRslt8;
        m_tData.iFailCnt  += iAddCntRslt8;
        bNeedSave = true ;
    }
    if(iAddCntGood > 0) {
        m_tData.iWorkCnt  += iAddCntGood;
        bNeedSave = true ;
    }

    if(bNeedSave) SaveDayData(m_tData , tDateTime);

    iPreRslt1 = DM.ARAY[riSRT].GetCntStat(csRslt1);
    iPreRslt2 = DM.ARAY[riSRT].GetCntStat(csRslt2);
    iPreRslt3 = DM.ARAY[riSRT].GetCntStat(csRslt3);
    iPreRslt4 = DM.ARAY[riSRT].GetCntStat(csRslt4);
    iPreRslt5 = DM.ARAY[riSRT].GetCntStat(csRslt5);
    iPreRslt6 = DM.ARAY[riSRT].GetCntStat(csRslt6);
    iPreRslt7 = DM.ARAY[riSRT].GetCntStat(csRslt7);
    iPreRslt8 = DM.ARAY[riSRT].GetCntStat(csRslt8);
    iPreGood  = DM.ARAY[riSRT].GetCntStat(csGood );

    */




}

void CDayData::WriteDayInfo()
{

    TDateTime  tDateTime = Now();
//    int iFailCnt = DM.ARAY[DATA_ARAY].GetCntStat  (csRslt1) +
//                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt2) +
//                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt3) +
//                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt4) +
//                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt5) +
//                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt6) +
//                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt7) +
//                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt8) ;
//    m_tData.iWorkCnt += DM.ARAY[DATA_ARAY].GetMaxCol() * DM.ARAY[DATA_ARAY].GetMaxRow();
//    m_tData.iFailCnt += iFailCnt;
    m_tData.iUPEH     = m_tData.dTotalTime == 0 ? 0.0 : m_tData.iWorkCnt / (m_tData.dTotalTime * 24) ;
    m_tData.iUPH      = m_tData.dWorkTime  == 0 ? 0.0 : m_tData.iWorkCnt / (m_tData.dWorkTime  * 24) ;

    SaveDayData(m_tData , tDateTime);


}
bool CDayData::LoadSaveDayIni(bool _bLoad , TData & _tData)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath;

    //Make Dir.
    sPath = DAY_FOLDER + "DayInfo.ini";

    if(_bLoad) {
        UserINI.Load(sPath.c_str() , "Data" , "dWorkTime " , _tData.dWorkTime  );
        UserINI.Load(sPath.c_str() , "Data" , "dStopTime " , _tData.dStopTime  );
        UserINI.Load(sPath.c_str() , "Data" , "dErrTime  " , _tData.dErrTime   );
        UserINI.Load(sPath.c_str() , "Data" , "dTotalTime" , _tData.dTotalTime );
//        UserINI.Load(sPath.c_str() , "Data" , "iUPEH     " , _tData.iUPEH      );
//        UserINI.Load(sPath.c_str() , "Data" , "iUPH      " , _tData.iUPH       );
        UserINI.Load(sPath.c_str() , "Data" , "iWorkCnt  " , _tData.iWorkCnt   );
//        UserINI.Load(sPath.c_str() , "Data" , "iFailCnt  " , _tData.iFailCnt   );
        UserINI.Load(sPath.c_str() , "Data" , "iLotCnt   " , _tData.iLotCnt    );

    }
    else {
        UserINI.Save(sPath.c_str() , "Data" , "dWorkTime " , _tData.dWorkTime  );
        UserINI.Save(sPath.c_str() , "Data" , "dStopTime " , _tData.dStopTime  );
        UserINI.Save(sPath.c_str() , "Data" , "dErrTime  " , _tData.dErrTime   );
        UserINI.Save(sPath.c_str() , "Data" , "dTotalTime" , _tData.dTotalTime );
//        UserINI.Save(sPath.c_str() , "Data" , "iUPEH     " , _tData.iUPEH      );
//        UserINI.Save(sPath.c_str() , "Data" , "iUPH      " , _tData.iUPH       );
        UserINI.Save(sPath.c_str() , "Data" , "iWorkCnt  " , _tData.iWorkCnt   );
//        UserINI.Save(sPath.c_str() , "Data" , "iFailCnt  " , _tData.iFailCnt   );
        UserINI.Save(sPath.c_str() , "Data" , "iLotCnt   " , _tData.iLotCnt    );
    }


}

bool CDayData::SaveDayData(TData & _tData , TDateTime _tDateTime) //혹시 나중에 Lot하고 동기화 할까봐 TDateTime 인자 넣음.
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    sPath = LOT_FOLDER + _tDateTime.FormatString("yyyymmdd") + ".ini" ;

    //기존에 있던것들 지우기.
    UserFile.ClearDirDate(LOT_FOLDER , Now() - 30 * 3); //90일.

    UserINI.Save(sPath.c_str() , "Data" , "dWorkTime " , _tData.dWorkTime  );
    UserINI.Save(sPath.c_str() , "Data" , "dStopTime " , _tData.dStopTime  );
    UserINI.Save(sPath.c_str() , "Data" , "dErrTime  " , _tData.dErrTime   );
    UserINI.Save(sPath.c_str() , "Data" , "dTotalTime" , _tData.dTotalTime );
//    UserINI.Save(sPath.c_str() , "Data" , "iUPEH     " , _tData.iUPEH      );
//    UserINI.Save(sPath.c_str() , "Data" , "iUPH      " , _tData.iUPH       );
    UserINI.Save(sPath.c_str() , "Data" , "iWorkCnt  " , _tData.iWorkCnt   );
//    UserINI.Save(sPath.c_str() , "Data" , "iFailCnt  " , _tData.iFailCnt   );
    UserINI.Save(sPath.c_str() , "Data" , "iLotCnt   " , _tData.iLotCnt    );

}

void CDayData::ClearData()
{
    memset(&m_tData , 0 , sizeof(TData));

}

void CDayData::DispDayList (TStringGrid * _sgDateList)
{
    //SPC에 탭 추가 하고 구현 해야하는데.
    //일단 안씀.
}

void CDayData::DispDayInfo (TStringGrid * _sgDayInfo) //오퍼레이션 창용.
{
    if(!_sgDayInfo) return ;

    TDateTime tDateTime ;

    _sgDayInfo -> ColCount = 2 ;
    _sgDayInfo -> RowCount = 6 ;

    double dYield = m_tData.iWorkCnt ? (m_tData.iWorkCnt - m_tData.iFailCnt) * 100 / (double)m_tData.iWorkCnt : 0.0;
    String sYield = sYield.sprintf("%.2f%%",dYield);


    _sgDayInfo -> Cells[0][0] = "WorkTime "; tDateTime.Val = m_tData.dWorkTime  ; _sgDayInfo -> Cells[1][0] = tDateTime.FormatString("hh:nn:ss");
    _sgDayInfo -> Cells[0][1] = "StopTime "; tDateTime.Val = m_tData.dStopTime  ; _sgDayInfo -> Cells[1][1] = tDateTime.FormatString("hh:nn:ss");
    _sgDayInfo -> Cells[0][2] = "ErrTime  "; tDateTime.Val = m_tData.dErrTime   ; _sgDayInfo -> Cells[1][2] = tDateTime.FormatString("hh:nn:ss");
    _sgDayInfo -> Cells[0][3] = "TotalTime"; tDateTime.Val = m_tData.dTotalTime ; _sgDayInfo -> Cells[1][3] = tDateTime.FormatString("hh:nn:ss");
//    _sgDayInfo -> Cells[0][4] = "UPEH     ";                                      _sgDayInfo -> Cells[1][4] = m_tData.iUPEH    ;
    //_sgDayInfo -> Cells[0][4] = "UPH      ";                                      _sgDayInfo -> Cells[1][4] = m_tData.iUPH     ;
    _sgDayInfo -> Cells[0][4] = "WorkCnt  ";                                      _sgDayInfo -> Cells[1][4] = m_tData.iWorkCnt ;
//    _sgDayInfo -> Cells[0][6] = "FailCnt  ";                                      _sgDayInfo -> Cells[1][6] = m_tData.iFailCnt ;
    _sgDayInfo -> Cells[0][5] = "LotCnt   ";                                      _sgDayInfo -> Cells[1][5] = m_tData.iLotCnt  ;
//    _sgDayInfo -> Cells[0][8] = "Yield    ";                                      _sgDayInfo -> Cells[1][8] = sYield           ;
}

__fastcall CSPC::CSPC(void)
{

}

__fastcall CSPC::~CSPC(void)
{
}

void CSPC::Init()
{
    ERR.Init();
    LOT.Init();
    DAY.Init();
}

void CSPC::Close()
{
    ERR.Close();
    LOT.Close();
    DAY.Close();
}

void CSPC::Update(EN_SEQ_STAT Stat)
{
    ERR.Update(LOT.GetCrntLotNo()     );
    LOT.Update(                   Stat);
    DAY.Update(LOT.GetCrntLotNo(),Stat);
}



