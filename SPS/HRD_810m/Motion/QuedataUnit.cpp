//---------------------------------------------------------------------------
#pragma hdrstop
#include "QuedataUnit.h"
#include "UserINI.h"

#pragma package(smart_init)



__fastcall CQueData::CQueData(void)
{
    ClearData();

    Load(true);

}

__fastcall CQueData::~CQueData(void)
{
    Load(false);                                       

}

void CQueData::Load(bool _IsLoad)
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sPath   ;

    //Set Dir.
    sPath = EXE_FOLDER + "SeqData\\"+"MarkData.INI" ;

    if(_IsLoad){
        UserINI.Load(sPath, "m_iDataCnt" , "m_iDataCnt"   , m_iDataCnt);
        for(int i = 0 ; i < MAX_QUE_DATA_CNT ; i++) {
            UserINI.Load(sPath, (AnsiString)i+"_Que" , "iRow"   , Data[i].iRow );
            UserINI.Load(sPath, (AnsiString)i+"_Que" , "iCow"   , Data[i].iCol );
            UserINI.Load(sPath, (AnsiString)i+"_Que" , "bFail"  , Data[i].bFail);
        }
    }
    else {
        UserINI.Save(sPath, "m_iDataCnt" , "m_iDataCnt"   , m_iDataCnt);
        for(int i = 0 ; i < MAX_QUE_DATA_CNT ; i++) {
            UserINI.Save(sPath, (AnsiString)i+"_Que" , "iRow"   , Data[i].iRow );
            UserINI.Save(sPath, (AnsiString)i+"_Que" , "iCow"   , Data[i].iCol );
            UserINI.Save(sPath, (AnsiString)i+"_Que" , "bFail"  , Data[i].bFail);
        }
    }

}

void CQueData::ClearData(int _iClrCnt)
{
    if(_iClrCnt == MAX_QUE_DATA_CNT) { //싹지우기..
        memset(Data , 0 , sizeof(TQueData)*MAX_QUE_DATA_CNT );
        m_iDataCnt = 0 ;
        return ;
    }
    else { //갯수 만큼만 지우고 스택방식으로 데이터 쉬프트.
        for(int i = 0 ; i < _iClrCnt ; i++) {
            for(int j = 0 ; j < MAX_QUE_DATA_CNT - 1 ; j++) {
                Data[j].iRow  = Data[j+1].iRow  ;
                Data[j].iCol  = Data[j+1].iCol  ;
                Data[j].bFail = Data[j+1].bFail ;
            }
            //맨마지막 데이터는 0,0
            Data[MAX_QUE_DATA_CNT - 1].iRow  = 0     ;
            Data[MAX_QUE_DATA_CNT - 1].iCol  = 0     ;
            Data[MAX_QUE_DATA_CNT - 1].bFail = false ;

            m_iDataCnt--;
        }
    }


}

void CQueData::PushData(int _iRow  , int _iCol , bool _bFail)
{
    for(int i = MAX_QUE_DATA_CNT - 2 ; i >= 0 ; i--){
        Data[i + 1].iRow  = Data[i].iRow  ;
        Data[i + 1].iCol  = Data[i].iCol  ;
        Data[i + 1].bFail = Data[i].bFail ;

    }

    Data[0].iRow  = _iRow  ;
    Data[0].iCol  = _iCol  ;
    Data[0].bFail = _bFail ;

    m_iDataCnt++;

}

int  CQueData::GetDataR(int _iQueue)
{
    if(_iQueue <  0               ) return 0 ;
    if(_iQueue >= MAX_QUE_DATA_CNT) return 0 ;
    return Data[_iQueue].iRow ;

}

int  CQueData::GetDataC(int _iQueue)
{
    if(_iQueue <  0               ) return 0 ;
    if(_iQueue >= MAX_QUE_DATA_CNT) return 0 ;
    return Data[_iQueue].iCol ;
}
bool CQueData::GetDataF(int _iQueue)
{
    if(_iQueue < 0                ) return 0 ;
    if(_iQueue >= MAX_QUE_DATA_CNT) return 0 ;

    return Data[_iQueue].bFail ;
}

int  CQueData::GetDataCnt()
{
    return m_iDataCnt ;
}

















