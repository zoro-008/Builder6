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
#include "Stage.h"
#include "LotUnit.h"

#include "SMInterfaceUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)



const  EN_ARAY_ID DATA_ARAY = riSTG ;

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
    int iSttDate = _tSttData ; //�Ҽ��� �������鼭 ���� ����.
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
    int iSttDate = _tSttData ; //�Ҽ��� �������鼭 ���� ����.
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

    if(_bNumberTime) { //���� �켱.
        for(int i = 0; i < _iDataCnt; i++) { //�������� ������ �������� ����!!
           for(int j = 0; j < _iDataCnt -i - 1; j++) {
              if(_tData[j].iErrNo > _tData[j + 1].iErrNo) {
                 _tTempData = _tData[j];
                 _tData[j] = _tData [j + 1];
                 _tData[j + 1] = _tTempData;
              }
           }
        }
    }
    else { //�ð� �켱.
        for(int i = 0; i < _iDataCnt; i++) { //�������� ������ �������� ����!!
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
    //�ֱٿ� �� �����Ͻ� �ðܳ�...
    if(m_iLastErr == _iErrNo && m_sLastErrMsg == _sErrMsg) return false;

    if(!DirectoryExists(ERR_FOLDER)) UserFile.CreateDir(ERR_FOLDER);

    //������ �ִ��͵� �����.
    UserFile.ClearDirDate(ERR_FOLDER , Now() - 30 * 12); //90��.

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

bool CErrData::ClearLastErr()
{
    m_iLastErr = -1 ;
    m_sLastErrMsg = "" ;

}


void CErrData::DispErrData (TDateTime _tSttData , TDateTime _tEndData , TStringGrid * _sgErrDate , bool _bNumberTime) //true�� �����ѹ� , false�� �ð�.
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

    //������ �ִ��͵� �����.
    UserFile.ClearDirDate(LOT_FOLDER , Now() - 30 * 12); //90��.


    //�۾� �ް��� ī��Ʈ ����.
    int iMgzCnt = 0;
    UserINI.Load(sPath.c_str() , "ETC" , "MgzCnt"  , iMgzCnt);
    if(_iMgzNo+1 >= iMgzCnt) UserINI.Save(sPath.c_str() , "ETC" , "MgzCnt" , _iMgzNo+1);

    //�ƽ� ���� ī��Ʈ ����.
    int iSlotCnt = 0;
    UserINI.Load(sPath.c_str() , "ETC" , "SlotCnt" , iSlotCnt);
    if(_iSlotNo+1 >= iSlotCnt) UserINI.Save(sPath.c_str() , "ETC" , "SlotCnt" , _iSlotNo+1);

    UserINI.Save(sPath.c_str() , "ETC" , "ColCnt"      , _rData.GetMaxCol());
    UserINI.Save(sPath.c_str() , "ETC" , "RowCnt"      , _rData.GetMaxRow());


    //�� ���� ����.
    UserINI.Save(sPath.c_str() , "Data" , "sLotId    " , _tData.sLotId    );
    UserINI.Save(sPath.c_str() , "Data" , "iWorkCnt  " , _tData.iWorkCnt  );

    UserINI.Save(sPath.c_str() , "Data" , "dSttTime  " , _tData.dSttTime  );
    UserINI.Save(sPath.c_str() , "Data" , "dEndTime  " , _tData.dEndTime  );
    UserINI.Save(sPath.c_str() , "Data" , "dWorkTime " , _tData.dWorkTime );
    UserINI.Save(sPath.c_str() , "Data" , "dErrTime  " , _tData.dErrTime  );
    UserINI.Save(sPath.c_str() , "Data" , "dStopTime " , _tData.dStopTime );
    UserINI.Save(sPath.c_str() , "Data" , "dTotalTime" , _tData.dTotalTime);
    UserINI.Save(sPath.c_str() , "Data" , "iUPEH     " , _tData.iUPEH     );
    UserINI.Save(sPath.c_str() , "Data" , "sJobFile  " , _tData.sJobFile  );


    //�� ����.
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

        UserINI.Load(sPath.c_str() , "Data" , "dSttTime  " , m_tData.dSttTime  );
        UserINI.Load(sPath.c_str() , "Data" , "dEndTime  " , m_tData.dEndTime  );
        UserINI.Load(sPath.c_str() , "Data" , "dWorkTime " , m_tData.dWorkTime );
        UserINI.Load(sPath.c_str() , "Data" , "dErrTime  " , m_tData.dErrTime  );
        UserINI.Load(sPath.c_str() , "Data" , "dStopTime " , m_tData.dStopTime );
        UserINI.Load(sPath.c_str() , "Data" , "dTotalTime" , m_tData.dTotalTime);
        UserINI.Load(sPath.c_str() , "Data" , "iUPEH     " , m_tData.iUPEH     );
        UserINI.Load(sPath.c_str() , "Data" , "sJobFile  " , m_tData.sJobFile  );
    }
    else {
        UserINI.Save(sPath.c_str() , "Data" , "sLotId    " , m_tData.sLotId    );
        UserINI.Save(sPath.c_str() , "Data" , "iWorkCnt  " , m_tData.iWorkCnt  );
        UserINI.Save(sPath.c_str() , "Data" , "dSttTime  " , m_tData.dSttTime  );
        UserINI.Save(sPath.c_str() , "Data" , "dEndTime  " , m_tData.dEndTime  );
        UserINI.Save(sPath.c_str() , "Data" , "dWorkTime " , m_tData.dWorkTime );
        UserINI.Save(sPath.c_str() , "Data" , "dErrTime  " , m_tData.dErrTime  );
        UserINI.Save(sPath.c_str() , "Data" , "dStopTime " , m_tData.dStopTime );
        UserINI.Save(sPath.c_str() , "Data" , "dTotalTime" , m_tData.dTotalTime);
        UserINI.Save(sPath.c_str() , "Data" , "iUPEH     " , m_tData.iUPEH     );
        UserINI.Save(sPath.c_str() , "Data" , "sJobFile  " , m_tData.sJobFile  );
    }

}

void CLotData::ClearData()
{
    m_tData.Clear();
    //memset(&m_tData , 0 , sizeof(TData)); �̹������ �ϸ� String ���� �޸� ���� ����.

}

void CLotData::DispWorkList(String _sFilePath , TStringGrid * _sgWorkList)
{
    DispData.LoadFromCsv(WRK_FOLDER + _sFilePath);
    _sgWorkList -> ColCount = DispData.GetMaxCol() ;
    _sgWorkList -> RowCount = DispData.GetMaxRow() ;
    _sgWorkList -> ColWidths[0] = 30 ;
    for(int c = 0 ; c < _sgWorkList -> ColCount ; c++){
        for(int r = 0 ; r < _sgWorkList -> RowCount ; r++){
            _sgWorkList -> Cells[c][r] = DispData.GetCell(c,r);
        }
    }

    //ColWidth
    int iTextCnt ;
    const int iTextPxRatio = 12 ;
    for(int c = 0 ; c < _sgWorkList -> ColCount ; c++){
        iTextCnt = 0 ;
        for(int r = 0 ; r < _sgWorkList -> RowCount ; r++){
            if(iTextCnt < _sgWorkList -> Cells[c][r].Length()) iTextCnt = _sgWorkList -> Cells[c][r].Length() ;
        }
        _sgWorkList -> ColWidths[c] =  iTextCnt * iTextPxRatio ;
    }
}

void CLotData::DispWorkDateList(TStringGrid * _sgDateList)
{
    //StringGrid�� Update�Ѵ�.
    UserFile.GridSearchFile(WRK_FOLDER , _sgDateList , 1 , true);     // ���丮 �о�ͼ� ��¥�� ���ĺ� ������ ����
}

void CLotData::SaveWorkListCSV(String _sFilePath)
{
    //20150823 ���̾� FOS������ �߰�...
    //���� �ϳ��ϳ��� ����Ʈ�� ���� ��� �Ѵ�.
    DispData.SaveToCsv(_sFilePath);
}

void CLotData::SaveWorkListXLS(String _sFilePath)
{
    //20150823 ���̾� FOS������ �߰�...
    //���� �ϳ��ϳ��� ����Ʈ�� ���� ��� �Ѵ�.
    DispData.SaveToXls(_sFilePath);
}

void CLotData::DispDateList(TStringGrid * _sgDateList)
{
    //StringGrid�� Update�Ѵ�.
    UserFile.GridSearchDir(LOT_FOLDER , _sgDateList , 1 , true);     // ���丮 �о�ͼ� ��¥�� ���ĺ� ������ ����
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

    //LotCountȮ��
    if (FindFirst(sSerchFile , faAnyFile , sr) == 0) {
        do {
            if ((sr.Attr & faDirectory) != sr.Attr) {
                iLotCnt++;
            }
        } while (FindNext(sr) == 0);
    }
    FindClose(sr);

    //sr.

    _sgLotList -> RowCount  = iLotCnt ? iLotCnt + 1 : 2;
    _sgLotList -> ColCount  = 9 ;
    _sgLotList -> FixedCols = 1 ;
    _sgLotList -> FixedRows = 1 ;

    _sgLotList -> Cells[0][0] = "No"       ;             _sgLotList -> ColWidths[0] = _sgLotList -> Cells[0][0].Length() * 15 ;
    _sgLotList -> Cells[1][0] = "LotId"    ;             _sgLotList -> ColWidths[1] = _sgLotList -> Cells[1][0].Length() * 25 ;
    _sgLotList -> Cells[2][0] = "WorkCOunt";             _sgLotList -> ColWidths[2] = _sgLotList -> Cells[2][0].Length() * 25 ;

    _sgLotList -> Cells[3][0] = "UPEH"     ;             _sgLotList -> ColWidths[3] = _sgLotList -> Cells[3][0].Length() * 15 ;
    _sgLotList -> Cells[4][0] = "Start"    ;             _sgLotList -> ColWidths[4] = _sgLotList -> Cells[4][0].Length() * 15 ;
    _sgLotList -> Cells[5][0] = "End  "    ;             _sgLotList -> ColWidths[5] = _sgLotList -> Cells[5][0].Length() * 15 ;
    _sgLotList -> Cells[6][0] = "WorkTime" ;             _sgLotList -> ColWidths[6] = _sgLotList -> Cells[6][0].Length() * 15 ;
    _sgLotList -> Cells[7][0] = "TotalTime";             _sgLotList -> ColWidths[7] = _sgLotList -> Cells[7][0].Length() * 15 ;
    _sgLotList -> Cells[8][0] = "JobFile"  ;             _sgLotList -> ColWidths[8] = _sgLotList -> Cells[8][0].Length() * 15 ;


    //LotData
    iLotCnt = 0 ;
    if (FindFirst(sSerchFile , faAnyFile , sr) == 0) {
        do {
            if ((sr.Attr & faDirectory) != sr.Attr) {
                sIniPath = sPath + "\\" + sr.Name  ;

                //�� ���� ��������.
                                                                                           _sgLotList -> Cells[0][iLotCnt+1] = iLotCnt+1       ;
                UserINI.Load(sIniPath.c_str() , "Data" , "sLotId    " , tData.sLotId    ); _sgLotList -> Cells[1][iLotCnt+1] = tData.sLotId    ;
                UserINI.Load(sIniPath.c_str() , "Data" , "iWorkCnt  " , tData.iWorkCnt  ); _sgLotList -> Cells[2][iLotCnt+1] = tData.iWorkCnt  ;

                UserINI.Load(sIniPath.c_str() , "Data" , "iUPEH     " , tData.iUPEH     ); _sgLotList -> Cells[3][iLotCnt+1] = tData.iUPEH     ;
                UserINI.Load(sIniPath.c_str() , "Data" , "dSttTime  " , tData.dSttTime  ); tTime.Val = tData.dSttTime  ; _sgLotList -> Cells[4][iLotCnt+1] = tTime.FormatString("hh:nn:ss") ;
                UserINI.Load(sIniPath.c_str() , "Data" , "dEndTime  " , tData.dEndTime  ); tTime.Val = tData.dEndTime  ; _sgLotList -> Cells[5][iLotCnt+1] = tTime.FormatString("hh:nn:ss") ;
                UserINI.Load(sIniPath.c_str() , "Data" , "dWorkTime " , tData.dWorkTime ); tTime.Val = tData.dWorkTime ; _sgLotList -> Cells[6][iLotCnt+1] = tTime.FormatString("hh:nn:ss") ;
                UserINI.Load(sIniPath.c_str() , "Data" , "dTotalTime" , tData.dTotalTime); tTime.Val = tData.dTotalTime; _sgLotList -> Cells[7][iLotCnt+1] = tTime.FormatString("hh:nn:ss") ;
                UserINI.Load(sIniPath.c_str() , "Data" , "sJobFile  " , tData.sJobFile  ); _sgLotList -> Cells[8][iLotCnt+1] = tData.sJobFile   ;
                UserINI.Load(sIniPath.c_str() , "Data" , "dErrTime  " , tData.dErrTime  ); //���÷��� ����.
                UserINI.Load(sIniPath.c_str() , "Data" , "dStopTime " , tData.dStopTime ); //���÷��� ����.
                iLotCnt++;
            }
        } while (FindNext(sr) == 0);
    }
    FindClose(sr);

    AnsiString sTemp;
    TDateTime  tCmprTime1 ;
    TDateTime  tCmprTime2 ;

    for (int i = 1 ; i < _sgLotList -> RowCount - 1 ; i++){
        if (_sgLotList -> Cells[9][i] == "") continue;

        for(int j = i + 1 ; j < _sgLotList -> RowCount ; j++){
            if (_sgLotList -> Cells[9][j] == "") continue;
                tCmprTime1 = _sgLotList -> Cells[9][i] ;
                tCmprTime2 = _sgLotList -> Cells[9][j] ;
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

    /*
    for (int i=1 ; i<_sgLotList->RowCount-1 ; i++){
        if (_sgLotList->Cells[5][i]=="") continue;

        for(int j=i+1 ; j<_sgLotList->RowCount ; j++){
            if (_sgLotList->Cells[5][j]=="") continue;
                tCmprTime1 = _sgLotList->Cells[5][i] ;
                tCmprTime2 = _sgLotList->Cells[5][j] ;
                tCmprTime1.FormatString("fffff.ffffffffff") ;
                tCmprTime2.FormatString("fffff.ffffffffff") ;
            if (tCmprTime1 > tCmprTime2){
               _sgLotList->Rows[_sgLotList->RowCount] = _sgLotList->Rows[i];
               _sgLotList->Rows[i] = _sgLotList->Rows[j];
               _sgLotList->Rows[j] = _sgLotList->Rows[_sgLotList->RowCount];
            }
        }
    }
    */
    //Sorting.
/*
    AnsiString tempName;
    int        tempTime;
    for (register int i = 0 ; i < Count ; i++) {
        for (register int j = i ; j < Count ; j++) {
            if (IDate[i] < IDate[j]) {
                tempName = Item [i]; Item [i] = Item [j]; Item[ j] = tempName;
                tempTime = IDate[i]; IDate[i] = IDate[j]; IDate[j] = tempTime;
            }
        }
    }

    //Redisplay.
    Grid -> RowCount = Count;
    if (Grid -> ColCount < 3) DispDate = false;
    for (register int i = 0 ; i < Count ; i++) {
        Grid -> Cells[0][i] = i + 1;
        Grid -> Cells[1][i] = Item[i];
        if (DispDate) Grid -> Cells[2][i] = FileDateToDateTime(IDate[i]);
        }
    Grid->Row = 0;
*/
}

void CLotData::DispMgzAndSlotList(AnsiString _sDate , AnsiString _sLotId , TStringGrid * _sgMgz , TStringGrid * _sgSlot )
{

    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    int iMgzCnt ;
    int iSlotCnt ;


    sPath = LOT_FOLDER + _sDate + "\\" + _sLotId + ".ini" ;

    //�۾� �ް��� ī��Ʈ ����.
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

    //�۾� �ް��� ī��Ʈ ����.
    UserINI.Load(sPath.c_str() , "ETC" , "ColCnt" , iColCnt);
    UserINI.Load(sPath.c_str() , "ETC" , "RowCnt" , iRowCnt);

    _sgMap -> RowCount = iRowCnt +1;
    _sgMap -> ColCount = iColCnt +1;

    _sgMap -> FixedCols = 1 ;
    _sgMap -> FixedRows = 1 ;


    //�� ����.
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
void CLotData::DispLotInfo(TStringGrid * _sgLotInfo) //���۷��̼� â��.
{
    if(!_sgLotInfo )return ;

    TDateTime tDateTime ;

    _sgLotInfo -> ColCount = 2 ;
    _sgLotInfo -> RowCount = 9 ;


    //double dYield = m_tData.iWorkCnt ? (m_tData.iWorkCnt - m_tData.iFailCnt)*100 / (double)m_tData.iWorkCnt : 0.0;
    //String sYield = sYield.sprintf("%.2f%%",dYield);

    _sgLotInfo -> Cells[0][0] = "SttTime  "  ; tDateTime.Val = m_tData.dSttTime   ;  _sgLotInfo -> Cells[1][0] = tDateTime.FormatString("hh:nn:ss");
    _sgLotInfo -> Cells[0][1] = "EndTime  "  ; tDateTime.Val = m_tData.dEndTime   ;  _sgLotInfo -> Cells[1][1] = tDateTime.FormatString("hh:nn:ss");
    _sgLotInfo -> Cells[0][2] = "WorkTime "  ; tDateTime.Val = m_tData.dWorkTime  ;  _sgLotInfo -> Cells[1][2] = tDateTime.FormatString("hh:nn:ss");
    _sgLotInfo -> Cells[0][3] = "TotalTime"  ; tDateTime.Val = m_tData.dTotalTime ;  _sgLotInfo -> Cells[1][3] = tDateTime.FormatString("hh:nn:ss");
    _sgLotInfo -> Cells[0][4] = "CycleTime"  ;                                       _sgLotInfo -> Cells[1][4] = String((int)m_tData.dTickTime) + "sec";
    _sgLotInfo -> Cells[0][5] = "WorkCnt  "  ;                                       _sgLotInfo -> Cells[1][5] = m_tData.iWorkCnt  ;
    _sgLotInfo -> Cells[0][6] = "UPEH     "  ;                                       _sgLotInfo -> Cells[1][6] = m_tData.iUPEH     ;

}



void CLotData::Update(EN_SEQ_STAT Stat)
{
    //Ÿ�� ���� ����.
    TDateTime  tDateTime = Now();
    static double dPreTime    = tDateTime.Val ;
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
    m_tData.dTickTime = STG.GetTickTime() / 1000;
    dPreTime = dCrntTime ;

    //������ ü���� �Ҷ� ��� ���� �Ҵ� �ٽ� �ϱ⶧���� ������� �Ѵ�.
    //������ ���� ���� ������ ��� ������ �׳� �̷��� ���д�.
    if(Stat == ssStop ) return ;
    //if(Stat != ssRunning && Stat != ssRunWarn) return ;
    ///////////////////////�ſ� �߿�.............

    AnsiString sLotId ;
    sLotId = LOT.GetLotNo() ; //LDR���� ��ĵ�غ��� ������ �̸� �������� �Ѵ�.
    if(sLotId != m_tData.sLotId && sLotId != "") { //���! ���ο� ��.
        m_tData.Clear();
        //memset(&m_tData , 0 , sizeof(TData)); �޸� ����.

        m_tData.sLotId    = LOT.GetLotNo() ;
        m_tData.dSttTime  = dCrntTime ;
    }

    static int iPreWorkCnt = DM.ARAY[riIDX_R].GetCntStat(csWork);
    int        iWorkCnt    = DM.ARAY[riIDX_R].GetCntStat(csWork);

    if(iWorkCnt != iPreWorkCnt && iWorkCnt > 0){
        m_tData.iWorkCnt += 1;
        iPreWorkCnt = iWorkCnt;
    }


    //int iAddCntGood  = DM.ARAY[riSTG].GetCntStat(csGood )-iPreGood  ;

    const int iSlotNo = 0 ;
    const int iMgzNo  = 0 ;

    m_tData.dEndTime  = dCrntTime ;
    m_tData.iUPEH     = m_tData.dTotalTime == 0 ? 0.0 : m_tData.iWorkCnt  / (m_tData.dTotalTime * 24) ;
    m_tData.sJobFile  = OM.GetCrntDev() ;

    //TickTimeǥ��..JS
    //AnsiString sTemp  = SRT.m_dTickTime / 1000;
    //sTemp.printf("%.2f", SRT.m_dTickTime / 1000);
    //m_tData.dTickTime = sTemp.ToDouble();







    //���ο� ��.
/*

    //Ĩ ī��Ʈ ����.
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
    if(bExist && !bPreExist) { //���� �۾� ������ ����Ʈ���ۿ� ����.
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

    //TickTimeǥ��..JS
    //AnsiString sTemp  = SRT.m_dTickTime / 1000;
    //sTemp.printf("%.2f", SRT.m_dTickTime / 1000);
    //m_tData.dTickTime = sTemp.ToDouble();
*/
}

void CLotData::WriteLotInfo()
{
    //Ĩ ī��Ʈ ����.
    //int iSlotNo ;
    //int iMgzNo  ;
    //double dCrntTime = Now().Val ;
    //m_tData.iWorkCnt  += DM.ARAY[DATA_ARAY].GetMaxCol() * DM.ARAY[DATA_ARAY].GetMaxRow() - DM.ARAY[DATA_ARAY].GetCntStat(csEmpty); //Empty ���ش�...���°��̹Ƿ�.JS
    //m_tData.dEndTime  = dCrntTime ;
    //m_tData.iUPEH     = m_tData.dTotalTime == 0 ? 0.0 : m_tData.iWorkCnt  / (m_tData.dTotalTime * 24) ;
    //m_tData.sJobFile  = OM.GetCrntDev() ;
    //
    //iSlotNo = DM.ARAY[DATA_ARAY].GetID().ToIntDef(0)%100 ;
    //iMgzNo  = DM.ARAY[DATA_ARAY].GetID().ToIntDef(0)/100 ;
    //SaveArrayData(m_tData , iMgzNo , iSlotNo , DM.ARAY[DATA_ARAY]);
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











//���� ���� �ϸ��.
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

    double dCrntTime  = tDateTime.Val ;
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



    //Day Log �� �����ʹ� ���� �Ҷ� �����µ� ��¥�� ���� ������ Day�� Lot�ϰ� ��� ���� �����̶� �׳� �ٷιٷ� Day�� ���� �Ѵ�.
    //Lot�� ������ ���� �ϰ� ������ ��������Ʈ�� �ִ� ���� ���� �����ؼ� ����.
    bool bDateChanged = ((int)dCrntTime) != ((int)dPreTime) ; //�Ҽ��� ���ϴ� �ð� ������.
    if(bDateChanged) { //���� �ٲ�.
        ClearData();
    }
    dPreTime = dCrntTime ;


    static AnsiString sPreLotNo = _sCrntLotNo ;
    if(sPreLotNo != _sCrntLotNo /*&& Stat !=ssStop*/) { //�� ���� �ٲ����!!!!
        m_tData.iLotCnt++;
    }
    sPreLotNo = _sCrntLotNo ;

    static int iPreWorkCnt = DM.ARAY[riIDX_R].GetCntStat(csWork);
    int        iWorkCnt    = DM.ARAY[riIDX_R].GetCntStat(csWork);

    if(iWorkCnt != iPreWorkCnt && iWorkCnt > 0){
        m_tData.iWorkCnt += 1;
        iPreWorkCnt = iWorkCnt;
    }


    m_tData.iUPEH     = m_tData.dTotalTime == 0 ? 0.0 : m_tData.iWorkCnt / (m_tData.dTotalTime * 24) ;
    m_tData.iUPH      = m_tData.dWorkTime  == 0 ? 0.0 : m_tData.iWorkCnt / (m_tData.dWorkTime  * 24) ;



    //������ ü���� �Ҷ� ��� ���� �Ҵ� �ٽ� �ϱ⶧���� ������� �Ѵ�.
    //������ ���� ���� ������ ��� ������ �׳� �̷��� ���д�.
    if(Stat == ssStop) return ;
    ///////////////////////�ſ� �߿�.............

}

void CDayData::WriteDayInfo()
{
    TDateTime  tDateTime = Now();
    int iFailCnt = DM.ARAY[DATA_ARAY].GetCntStat  (csRslt1) +
                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt2) +
                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt3) +
                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt4) +
                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt5) +
                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt6) +
                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt7) +
                   DM.ARAY[DATA_ARAY].GetCntStat  (csRslt8) ;
    m_tData.iWorkCnt += DM.ARAY[DATA_ARAY].GetMaxCol() * DM.ARAY[DATA_ARAY].GetMaxRow();
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
        UserINI.Load(sPath.c_str() , "Data" , "iUPEH     " , _tData.iUPEH      );
        UserINI.Load(sPath.c_str() , "Data" , "iUPH      " , _tData.iUPH       );
        UserINI.Load(sPath.c_str() , "Data" , "iWorkCnt  " , _tData.iWorkCnt   );
        UserINI.Load(sPath.c_str() , "Data" , "iLotCnt   " , _tData.iLotCnt    );
    }
    else {
        UserINI.Save(sPath.c_str() , "Data" , "dWorkTime " , _tData.dWorkTime  );
        UserINI.Save(sPath.c_str() , "Data" , "dStopTime " , _tData.dStopTime  );
        UserINI.Save(sPath.c_str() , "Data" , "dErrTime  " , _tData.dErrTime   );
        UserINI.Save(sPath.c_str() , "Data" , "dTotalTime" , _tData.dTotalTime );
        UserINI.Save(sPath.c_str() , "Data" , "iUPEH     " , _tData.iUPEH      );
        UserINI.Save(sPath.c_str() , "Data" , "iUPH      " , _tData.iUPH       );
        UserINI.Save(sPath.c_str() , "Data" , "iWorkCnt  " , _tData.iWorkCnt   );
        UserINI.Save(sPath.c_str() , "Data" , "iLotCnt   " , _tData.iLotCnt    );
    }                                                        


}

bool CDayData::SaveDayData(TData & _tData , TDateTime _tDateTime) //Ȥ�� ���߿� Lot�ϰ� ����ȭ �ұ�� TDateTime ���� ����.
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    sPath = LOT_FOLDER + _tDateTime.FormatString("yyyymmdd") + ".ini" ;

    //������ �ִ��͵� �����.
    UserFile.ClearDirDate(LOT_FOLDER , Now() - 30 * 12); //90��.

    UserINI.Save(sPath.c_str() , "Data" , "dWorkTime " , _tData.dWorkTime  );
    UserINI.Save(sPath.c_str() , "Data" , "dStopTime " , _tData.dStopTime  );
    UserINI.Save(sPath.c_str() , "Data" , "dErrTime  " , _tData.dErrTime   );
    UserINI.Save(sPath.c_str() , "Data" , "dTotalTime" , _tData.dTotalTime );
    UserINI.Save(sPath.c_str() , "Data" , "iUPEH     " , _tData.iUPEH      );
    UserINI.Save(sPath.c_str() , "Data" , "iUPH      " , _tData.iUPH       );
    UserINI.Save(sPath.c_str() , "Data" , "iWorkCnt  " , _tData.iWorkCnt   );
    UserINI.Save(sPath.c_str() , "Data" , "iLotCnt   " , _tData.iLotCnt    );

}

void CDayData::ClearData()
{
    memset(&m_tData , 0 , sizeof(TData));

}

void CDayData::DispDayList (TStringGrid * _sgDateList)
{
    //SPC�� �� �߰� �ϰ� ���� �ؾ��ϴµ�.
    //�ϴ� �Ⱦ�.
}

void CDayData::DispDayInfo (TStringGrid * _sgDayInfo) //���۷��̼� â��.
{
    if(!_sgDayInfo) return ;

    TDateTime tDateTime ;

    _sgDayInfo -> ColCount = 2 ;
    _sgDayInfo -> RowCount = 9 ;


    _sgDayInfo -> Cells[0][0] = "StopTime "; tDateTime.Val = m_tData.dStopTime  ; _sgDayInfo -> Cells[1][0] = tDateTime.FormatString("hh:nn:ss");
    _sgDayInfo -> Cells[0][1] = "ErrTime  "; tDateTime.Val = m_tData.dErrTime   ; _sgDayInfo -> Cells[1][1] = tDateTime.FormatString("hh:nn:ss");
    _sgDayInfo -> Cells[0][2] = "WorkTime "; tDateTime.Val = m_tData.dWorkTime  ; _sgDayInfo -> Cells[1][2] = tDateTime.FormatString("hh:nn:ss");
    _sgDayInfo -> Cells[0][3] = "TotalTime"; tDateTime.Val = m_tData.dTotalTime ; _sgDayInfo -> Cells[1][3] = tDateTime.FormatString("hh:nn:ss");
    _sgDayInfo -> Cells[0][4] = "LotCnt   ";                                      _sgDayInfo -> Cells[1][4] = m_tData.iLotCnt  ;
    _sgDayInfo -> Cells[0][5] = "WorkCnt  ";                                      _sgDayInfo -> Cells[1][5] = m_tData.iWorkCnt ;
    _sgDayInfo -> Cells[0][6] = "UPEH     ";                                      _sgDayInfo -> Cells[1][6] = m_tData.iUPEH    ;


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

