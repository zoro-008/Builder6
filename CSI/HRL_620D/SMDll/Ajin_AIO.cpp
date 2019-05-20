//---------------------------------------------------------------------------


#pragma hdrstop

#include "Ajin_AIO.h"

#include "Define.h"
#ifdef AJIN_AXT
    #include "AxtAIO.h"
#endif //AJIN_AXT
#ifdef AJIN_AXL
    #include "AXA.h"
#endif //AJIN_AXL

//User Header
#include "Timer.h"
#include "SLogUnit.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
CAjinAIO AI;

CAjinAIO::CAjinAIO()
{
    static bool bFirst = true ;

    if(!bFirst) return ;
    bFirst = false ;

    m_iMaxIn  = 0 ;
    m_iMaxOut = 0 ;

    m_pModuleInfo = NULL ;

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

    if(!AIOIsInitialized()) {                           // IO모듈을 사용가능 한지 확인.
        if(!InitializeAIO()) {                          // IO모듈을 초기화(열려있는 모든베이스보드에서 AIO 을 검색하여 초기화)
            ShowMessageT("AJIN Axt AIO Module Loading Error");
            return  ;
        }
    }

    //InPut OutPut Count Set
    m_iMaxModuleNo = AIOget_module_count();
    if(m_iMaxModuleNo == 0) return ;
    m_pModuleInfo  = new CModuleInfo[m_iMaxModuleNo] ;


    //Get Max I Module , O Module Count.
    UINT16 iModuleType ;
    /*
    AXT_SIO_AI4R  A1h SIO-AI4R  , 입력 4채널, 12비트
    AXT_SIO_AO4R  A2h SIO-AO4R  , 출력 4채널, 12비트
    AXT_SIO_AI16H A3h SIO-AI16HB, 입력 16채널, 16비트
    AXT_SIO_AO8H  A4h SIO-AO8H  , 출력 8채널, 16비트

    AXT_SIO_AI4RB = 0xA1,// A1h(161) : AI 4Ch, 12 bit
    AXT_SIO_AO4RB = 0xA2,// A2h(162) : AO 4Ch, 12 bit
    AXT_SIO_AI16H = 0xA3,// A3h(163) : AI 4Ch, 16 bit
    AXT_SIO_AO8H  = 0xA4,// A4h(164) : AO 4Ch, 16 bit


    */
    for (int i=0 ; i < m_iMaxModuleNo ; i++) {
        iModuleType = AIOget_moduleid(i);
             if (iModuleType==AXT_SIO_AI16H) {m_iMaxIn += 16; m_iMaxOut += 2 ; m_pModuleInfo[i].iModuleInChCnt = 16; m_pModuleInfo[i].iModuleOutChCnt = 2 ;}
        else if (iModuleType==AXT_SIO_AI4RB) {m_iMaxIn += 4 ; m_iMaxOut += 0 ; m_pModuleInfo[i].iModuleInChCnt = 4 ; m_pModuleInfo[i].iModuleOutChCnt = 0 ;}
        else if (iModuleType==AXT_SIO_AO4RB) {m_iMaxIn += 0 ; m_iMaxOut += 4 ; m_pModuleInfo[i].iModuleInChCnt = 0 ; m_pModuleInfo[i].iModuleOutChCnt = 4 ;}
        else if (iModuleType==AXT_SIO_AO8H ) {m_iMaxIn += 0 ; m_iMaxOut += 8 ; m_pModuleInfo[i].iModuleInChCnt = 0 ; m_pModuleInfo[i].iModuleOutChCnt = 8 ;}
        else                                 {ShowMessageT("AJIN AXT AIO Unknown Channel Count Type!"); }
    }
    for(int i = 0 ; i < m_iMaxIn ; i++) {
        AIOset_range_adc(i,-10,10); //볼트 범위 설정.나중에 바꾸기...
    }
    for(int i = 0 ; i < m_iMaxOut ; i++) {
        AIOset_range_dac(i,-10,10); //나중에 바꾸기...
    }



#endif //AJIN_AXT

#ifdef AJIN_AXL
    if(!AxlIsOpened()){				// 통합 라이브러리가 사용 가능하지 (초기화가 되었는지) 확인
        if(AxlOpen(7) != AXT_RT_SUCCESS){			// 통합 라이브러리 초기화
            ShowMessageT("AJIN AXL Lib Loading Error");
            return ;
        }

        DWORD dwStatus;
        DWORD dwRet = AxaInfoIsAIOModule(&dwStatus); //아날로그 모줄 있는지 확인.
        if(dwRet != AXT_RT_SUCCESS){
            //ShowMessageT("AJIN AXL AIO Module Loading Error");
            return ;
        }
    }

    //InPut OutPut Count Set
    long lVal ;
    AxaInfoGetModuleCount(&lVal);

    m_iMaxModuleNo = lVal ;
    if(m_iMaxModuleNo == 0) return ;
    m_pModuleInfo  = new CModuleInfo[m_iMaxModuleNo] ;

    //Get Max I Module , O Module Count.
    int iInputCnt , iOutputCnt ;
    for(int i = 0 ; i < m_iMaxModuleNo ; i++) {
        AxaInfoGetInputCount (i,&lVal ); iInputCnt  = lVal ;
        AxaInfoGetOutputCount(i,&lVal ); iOutputCnt = lVal ;
             if (iInputCnt == 16 && iOutputCnt == 2 ) {m_iMaxIn += 16; m_iMaxOut += 2 ; m_pModuleInfo[i].iModuleInChCnt = 16; m_pModuleInfo[i].iModuleOutChCnt = 2 ;}
        else if (iInputCnt == 4  && iOutputCnt == 0 ) {m_iMaxIn += 4 ; m_iMaxOut += 0 ; m_pModuleInfo[i].iModuleInChCnt = 4 ; m_pModuleInfo[i].iModuleOutChCnt = 0 ;}
        else if (iInputCnt == 0  && iOutputCnt == 4 ) {m_iMaxIn += 0 ; m_iMaxOut += 4 ; m_pModuleInfo[i].iModuleInChCnt = 0 ; m_pModuleInfo[i].iModuleOutChCnt = 4 ;}
        else                                          {ShowMessageT("AJIN AXA Unknown Channel Count Type!"); }
    }

    AxaiInfoGetChannelCount(&lVal);
    if(lVal != m_iMaxIn){ShowMessageT("AJIN AXA Input Channel Count is Wrong!"); }

    AxaoInfoGetChannelCount(&lVal);
    if(lVal != m_iMaxOut){ShowMessageT("AJIN AXA Output Channel Count is Wrong!"); }

    for(int i = 0 ; i < m_iMaxIn ; i++) {
        AxaiSetRange(i,-10,10); //볼트 범위 설정.나중에 바꾸기...
    }
    for(int i = 0 ; i < m_iMaxOut ; i++) {
        AxaoSetRange(i,-10,10); //나중에 바꾸기...
    }

    /*

*/
#endif //AJIN_AXL


}


CAjinAIO::~CAjinAIO()
{
    if(m_pModuleInfo) delete [] m_pModuleInfo ;

#ifdef AJIN_AXT
    AxtCloseDeviceAll();
    AxtClose();
#endif
#ifdef AJIN_AXL
    AxlClose();
#endif

}

bool CAjinAIO::GetInModuleNo (int _iNo , int &_iModuleNo , int &_iOffset)
{
    int iOffset = _iNo ;

    for( int i = 0 ; i < m_iMaxModuleNo ; i++) {
         if(/*m_pModuleInfo[i].iModuleInChCnt &&*/ (iOffset - m_pModuleInfo[i].iModuleInChCnt < 0)){
             _iModuleNo = i       ;
             _iOffset   = iOffset ;
             return true ;
         }
         iOffset -= m_pModuleInfo[i].iModuleInChCnt ;
    }

    return false ;
}

bool CAjinAIO::GetOutModuleNo(int _iNo , int &_iModuleNo , int &_iOffset)
{
    int iOffset = _iNo ;

    for( int i = 0 ; i < m_iMaxModuleNo ; i++) {
         if((iOffset - m_pModuleInfo[i].iModuleOutChCnt < 0)){
             _iModuleNo = i       ;
             _iOffset   = iOffset ;
             return true ;
         }
         iOffset -= m_pModuleInfo[i].iModuleOutChCnt ;
    }

    return false ;
}

/* 아날로그에서는 필요 없을듯.
bool CAjinAIO::GetInModuleNoDp(int _iNo , int &_iModuleNo , int &_iOffset)
{
    int iOffset = _iNo ;
    int iDpNo   = 0 ;

    for( int i = 0 ; i < m_iMaxModuleNo ; i++) {
         if((iOffset - m_pModuleInfo[i].iModuleInChCnt < 0)){
             _iModuleNo = iDpNo   ;
             _iOffset   = iOffset ;
             return true ;
         }
         iOffset -= m_pModuleInfo[i].iModuleInChCnt ;
         if(m_pModuleInfo[i].iModuleInChCnt)iDpNo++ ;
    }

    return false ;
}

bool CAjinAIO::GetOutModuleNoDp(int _iNo , int &_iModuleNo , int &_iOffset)
{
    int iOffset = _iNo ;
    int iDpNo   = 0 ;

    for( int i = 0 ; i < m_iMaxModuleNo ; i++) {
         if((iOffset - m_pModuleInfo[i].iModuleOutChCnt < 0)){
             _iModuleNo = iDpNo   ;
             _iOffset   = iOffset ;
             return true ;
         }
         iOffset -= m_pModuleInfo[i].iModuleOutChCnt ;
         if(m_pModuleInfo[i].iModuleOutChCnt)iDpNo++;
    }

    return false ;
}
*/
void CAjinAIO::SetOut(int _iNo , double  _dVal)
{
    //Check Error.
    if (_iNo >= m_iMaxOut || _iNo < 0) {return ;}



    //OutPut
#ifdef AJIN_AXT
//    //Get Addr.
//    int iBitAddr ;
//    int iModule  ;
//
//    GetOutModuleNo(_iNo , iModule , iBitAddr);
//
//    AIOwrite_outport_bit(iModule, iBitAddr, _bOn);

    AIOwrite_dac(_iNo , _dVal);
#endif

#ifdef AJIN_AXL
    AxaoWriteVoltage(_iNo, _dVal);
#endif
}

double CAjinAIO::GetIn (int _iNo)
{
    //Check Error.
    if (_iNo >= m_iMaxIn || _iNo < 0) return false;



#ifdef AJIN_AXT
    ////Get Addr.
    //int iBitAddr ;
    //int iModule  ;
    //
    //GetInModuleNo(_iNo , iModule , iBitAddr);
    ////Get Input
    //return AIOread_inport_bit(iModule, iBitAddr);

    return AIOread_one_volt_adc( _iNo);
#endif

#ifdef AJIN_AXL
    double dVal ;
    AxaoReadVoltage(_iNo , &dVal );
    return dVal ;
#endif

}

double CAjinAIO::GetOut (int _iNo)
{
    //Check Error.
    if (_iNo >= m_iMaxOut || _iNo < 0) return false;



#ifdef AJIN_AXT
    //Get Addr.
    //int iBitAddr ;
    //int iModule  ;
    //
    //GetOutModuleNo(_iNo , iModule , iBitAddr);
    //Get Input
    //return AIOread_outport_bit(iModule, iBitAddr);
    return AIOread_dac(_iNo);
#endif

#ifdef AJIN_AXL
    double dVal ;
    AxaoReadVoltage(_iNo , &dVal);
    return dVal ;
#endif
}




























