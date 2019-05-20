//---------------------------------------------------------------------------


#pragma hdrstop

#include "Axt_DIO.h"

#include "Define.h"
#ifdef AJIN_AXT
    //Ajin Lib
    #include "AxtLIB.h"
    #include "AxtDIO.h"
#endif //AJIN_AXT

#ifdef AJIN_AXL
    //System Header
    #include <vcl.h> //아씨발 빼면 안돼네...순서 꼬인듯
    //Ajin Lib
    #include "AXL.h"
    #include "AXD.h"
#endif //AJIN_AXL

//User Header
#include "Timer.h"
#include "SLogUnit.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


CAjinDIO::CAjinDIO()
{
    static bool bFirst = true ;

    if(!bFirst) return ;

    m_iMaxIn  = 0 ;
    m_iMaxOut = 0 ;

#ifdef AJIN_AXT
    if(!AxtIsInitialized()){				// 통합 라이브러리가 사용 가능하지 (초기화가 되었는지) 확인
        if(!AxtInitialize(this, NULL)){			// 통합 라이브러리 초기화
            ShowMessageT("AJIN AXT Lib Loading Error");
            return ;
        }
    }

    if(!AxtIsInitializedBus(BUSTYPE_PCI)){		// PCI BUS가 초기화 되었는지 확인
        if(!AxtOpenDeviceAuto(BUSTYPE_PCI)){		// 새로운 베이스보드를 자동으로 통합라이브러리에 추가
            ShowMessageT("AJIN Axt Baseboard Loading Error");
            return ;
        }
    }

    if(!DIOIsInitialized()) {                           // IO모듈을 사용가능 한지 확인.
        if(!InitializeDIO()) {                          // IO모듈을 초기화(열려있는 모든베이스보드에서 FS모듈을 검색하여 초기화)
            ShowMessageT("AJIN Axt IO Module Loading Error");
            return  ;
        }
    }

    //InPut OutPut Count Set
    m_iMaxModuleNo = DIOget_module_count();
    m_pModuleInfo  = new CModuleInfo[m_iMaxModuleNo] ;


    //Get Max I Module , O Module Count.
    UINT16 iModuleType ;
    for (int i=0 ; i < m_iMaxModuleNo ; i++) {
        iModuleType = DIOget_module_id(i);
        if (iModuleType==0x99) {m_iMaxIn += 16; m_iMaxOut += 16; m_pModuleInfo[i].iModuleInByteCnt = 2 ; m_pModuleInfo[i].iModuleOutByteCnt = 2 ;}
        if (iModuleType==0x97) {m_iMaxIn += 32; m_iMaxOut += 0 ; m_pModuleInfo[i].iModuleInByteCnt = 4 ; m_pModuleInfo[i].iModuleOutByteCnt = 0 ;}
        if (iModuleType==0x98) {m_iMaxIn += 0 ; m_iMaxOut += 32; m_pModuleInfo[i].iModuleInByteCnt = 0 ; m_pModuleInfo[i].iModuleOutByteCnt = 4 ;}
    }
#endif //AJIN_AXT

#ifdef AJIN_AXL
    if(!AxlIsOpened()){				// 통합 라이브러리가 사용 가능하지 (초기화가 되었는지) 확인
        if(AxlOpen(7) != AXT_RT_SUCCESS){			// 통합 라이브러리 초기화
            ShowMessageT("AJIN AXL Lib Loading Error");
            return ;
        }

        DWORD dwStatus;
        DWORD dwRet = AxdInfoIsDIOModule(&dwStatus);
        if(dwRet != AXT_RT_SUCCESS){
            ShowMessageT("AJIN AXL IO Module Loading Error");
            return ;
        }
    }

    //InPut OutPut Count Set
    long lVal ;
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
#endif //AJIN_AXL

    bFirst = false ;
}


CAjinDIO::~CAjinDIO()
{
    delete [] m_pModuleInfo ;

#ifdef AJIN_AXT
    AxtCloseDeviceAll();
    AxtClose();
#endif
#ifdef AJIN_AXL
    AxlClose();
#endif

}

bool CAjinDIO::GetInModuleNo (int _iNo , int &_iModuleNo , int &_iOffset)
{
    int iOffset = _iNo ;

    for( int i = 0 ; i < m_iMaxModuleNo ; i++) {
         if(/*m_pModuleInfo[i].iModuleInByteCnt &&*/ (iOffset - m_pModuleInfo[i].iModuleInByteCnt * BYTE_BIT_COUNT < 0)){
             _iModuleNo = i       ;
             _iOffset   = iOffset ;
             return true ;
         }
         iOffset -= m_pModuleInfo[i].iModuleInByteCnt * BYTE_BIT_COUNT ;
    }

    return false ;
}

bool CAjinDIO::GetOutModuleNo(int _iNo , int &_iModuleNo , int &_iOffset)
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

bool CAjinDIO::GetInModuleNoDp(int _iNo , int &_iModuleNo , int &_iOffset)
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

bool CAjinDIO::GetOutModuleNoDp(int _iNo , int &_iModuleNo , int &_iOffset)
{
    int iOffset = _iNo ;
    int iDpNo   = 0 ;

    for( int i = 0 ; i < m_iMaxModuleNo ; i++) {
         if(/*m_pModuleInfo[i].iModuleOutByteCnt &&*/ (iOffset - m_pModuleInfo[i].iModuleOutByteCnt * BYTE_BIT_COUNT < 0)){
             _iModuleNo = iDpNo   ;
             _iOffset   = iOffset ;
             return true ;
         }
         iOffset -= m_pModuleInfo[i].iModuleOutByteCnt * BYTE_BIT_COUNT ;
         if(m_pModuleInfo[i].iModuleOutByteCnt)iDpNo++;
    }

    return false ;
}

void CAjinDIO::SetOut(int _iNo , bool _bOn)
{
    //Check Error.
    if (_iNo >= m_iMaxOut || _iNo < 0) {return ;}

    //Get Addr.
    int iBitAddr ;
    int iModule  ;

    GetOutModuleNo(_iNo , iModule , iBitAddr);

    //OutPut
#ifdef AJIN_AXT
    DIOwrite_outport_bit(iModule, iBitAddr, _bOn);
#endif

#ifdef AJIN_AXL
    AxdoWriteOutportBit(iModule, iBitAddr, _bOn);
#endif
}

bool CAjinDIO::GetIn (int _iNo)
{
    //Check Error.
    if (_iNo >= m_iMaxIn || _iNo < 0) return false;

    //Get Addr.
    int iBitAddr ;
    int iModule  ;

    GetInModuleNo(_iNo , iModule , iBitAddr);

#ifdef AJIN_AXT
    //Get Input
    return DIOread_inport_bit(iModule, iBitAddr);
#endif

#ifdef AJIN_AXL
    DWORD dOn ;
    AxdiReadInportBit(iModule, iBitAddr , &dOn);
    return dOn ;
#endif

}

bool CAjinDIO::GetOut (int _iNo)
{
    //Check Error.
    if (_iNo >= m_iMaxOut || _iNo < 0) return false;

    //Get Addr.
    int iBitAddr ;
    int iModule  ;

    GetOutModuleNo(_iNo , iModule , iBitAddr);

#ifdef AJIN_AXT
    //Get Input
    return DIOread_outport_bit(iModule, iBitAddr);
#endif

#ifdef AJIN_AXL
    DWORD dOn ;
    AxdoReadOutportBit(iModule, iBitAddr , &dOn);
    return dOn ;
#endif
}




























