//---------------------------------------------------------------------------


#pragma hdrstop

#include "Axl_DIO.h"

#ifdef AJIN_AXL

//System Header
#include <vcl.h> //아씨발 빼면 안돼네...

//Ajin Lib AXL
#include "AXL.h"
#include "AXD.h"

//User Header
#include "Timer.h"
#include "SLogUnit.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


CAxlDIO::CAxlDIO()
{
    static bool bFirst = true ;

    if(!bFirst) return ;

    m_iMaxIn  = 0 ;
    m_iMaxOut = 0 ;

    DWORD dwRet ;
    long  lVal  ;

    if(!AxlIsOpened()){				// 통합 라이브러리가 사용 가능하지 (초기화가 되었는지) 확인
        if(AxlOpen(7) != AXT_RT_SUCCESS){			// 통합 라이브러리 초기화
            ShowMessageT("AJIN AXL Lib Loading Error");
            return ;
        }

        DWORD dwStatus;
        dwRet = AxdInfoIsDIOModule(&dwStatus);
        if(dwRet != AXT_RT_SUCCESS){
            ShowMessageT("AJIN AXL IO Module Loading Error");
            return ;
        }
    }

    //InPut OutPut Count Set
    AxdInfoGetModuleCount(&lVal);
    m_iMaxModuleNo = lVal ;
    m_pModuleInfo  = new CModuleInfo[m_iMaxModuleNo] ;

    //Get Max I Module , O Module Count.
    int iInputCnt , iOutputCnt ;
    for(int i = 0 ; i < m_iMaxModuleNo ; i++) {
        AxdInfoGetInputCount (i,&lVal ); iInputCnt  = lVal ;
        AxdInfoGetOutputCount(i,&lVal ); iOutputCnt = lVal ;
        if (iInputCnt == 16 && iOutputCnt == 16) {m_iMaxIn += 16; m_iMaxOut += 16; m_pModuleInfo[i].iModuleInByteCnt = 2 ; m_pModuleInfo[i].iModuleOutByteCnt = 2 ;}
        if (iInputCnt == 32 && iOutputCnt == 0 ) {m_iMaxIn += 32; m_iMaxOut += 0 ; m_pModuleInfo[i].iModuleInByteCnt = 4 ; m_pModuleInfo[i].iModuleOutByteCnt = 0 ;}
        if (iInputCnt == 0  && iOutputCnt == 32) {m_iMaxIn += 0 ; m_iMaxOut += 32; m_pModuleInfo[i].iModuleInByteCnt = 0 ; m_pModuleInfo[i].iModuleOutByteCnt = 4 ;}
    }

    bFirst = false ;
}


CAxlDIO::~CAxlDIO()
{
    delete [] m_pModuleInfo ;

    AxlClose();
}

bool CAxlDIO::GetInModuleNo (int _iNo , int &_iModuleNo , int &_iOffset)
{
    int iOffset = _iNo ;

    for( int i = 0 ; i < m_iMaxModuleNo ; i++) {
         if((iOffset - m_pModuleInfo[i].iModuleInByteCnt * BYTE_BIT_COUNT < 0)){
             _iModuleNo = i       ;
             _iOffset   = iOffset ;
             return true ;
         }
         iOffset -= m_pModuleInfo[i].iModuleInByteCnt * BYTE_BIT_COUNT ;
    }

    return false ;
}

bool CAxlDIO::GetOutModuleNo(int _iNo , int &_iModuleNo , int &_iOffset)
{
    int iOffset = _iNo ;

    for( int i = 0 ; i < m_iMaxModuleNo ; i++) {
         if((iOffset - m_pModuleInfo[i].iModuleOutByteCnt * BYTE_BIT_COUNT < 0)){
             _iModuleNo = i       ;
             _iOffset   = iOffset ;
             return true ;
         }
         iOffset -= m_pModuleInfo[i].iModuleOutByteCnt * BYTE_BIT_COUNT ;
    }

    return false ;
}

bool CAxlDIO::GetInModuleNoDp(int _iNo , int &_iModuleNo , int &_iOffset)
{
    int iOffset = _iNo ;
    int iDpNo   = 0 ;

    for( int i = 0 ; i < m_iMaxModuleNo ; i++) {
         if((iOffset - m_pModuleInfo[i].iModuleInByteCnt * BYTE_BIT_COUNT < 0)){
             _iModuleNo = iDpNo   ;
             _iOffset   = iOffset ;
             return true ;
         }
         iOffset -= m_pModuleInfo[i].iModuleInByteCnt * BYTE_BIT_COUNT ;
         if(m_pModuleInfo[i].iModuleInByteCnt)iDpNo++ ;
    }

    return false ;
}

bool CAxlDIO::GetOutModuleNoDp(int _iNo , int &_iModuleNo , int &_iOffset)
{
    int iOffset = _iNo ;
    int iDpNo   = 0 ;

    for( int i = 0 ; i < m_iMaxModuleNo ; i++) {
         if((iOffset - m_pModuleInfo[i].iModuleOutByteCnt * BYTE_BIT_COUNT < 0)){
             _iModuleNo = iDpNo   ;
             _iOffset   = iOffset ;
             return true ;
         }
         iOffset -= m_pModuleInfo[i].iModuleOutByteCnt * BYTE_BIT_COUNT ;
         if(m_pModuleInfo[i].iModuleOutByteCnt)iDpNo++;
    }

    return false ;
}

void CAxlDIO::SetOut(int _iNo , bool _bOn)
{
    //Check Error.
    if (_iNo >= m_iMaxOut || _iNo < 0) {return ;}

    //Get Addr.
    int iBitAddr ;
    int iModule  ;

    GetOutModuleNo(_iNo , iModule , iBitAddr);

    //OutPut
    AxdoWriteOutportBit(iModule, iBitAddr, _bOn);
}

bool CAxlDIO::GetIn (int _iNo)
{
    //Check Error.
    if (_iNo >= m_iMaxIn || _iNo < 0) return false;

    //Get Addr.
    int iBitAddr ;
    int iModule  ;
    unsigned long iOn ;

    GetInModuleNo(_iNo , iModule , iBitAddr);

    //Get Input
    AxdiReadInportBit(iModule, iBitAddr , &iOn);
    return iOn ;
}

bool CAxlDIO::GetOut (int _iNo)
{
    //Check Error.
    if (_iNo >= m_iMaxOut || _iNo < 0) return false;

    //Get Addr.
    int iBitAddr ;
    int iModule  ;
    unsigned long iOn ;

    GetOutModuleNo(_iNo , iModule , iBitAddr);

    //Get Input
    AxdoReadOutportBit(iModule, iBitAddr , &iOn);
    return iOn ;
}

#endif //AJIN_AXL



























