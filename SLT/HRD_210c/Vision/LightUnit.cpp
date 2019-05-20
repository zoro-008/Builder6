//---------------------------------------------------------------------------
#pragma hdrstop
#include "LightUnit.h"
#include "UserINI.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

CLight * Light[MAX_LIGHT_ID] ;

CDkm::CDkm(void)
{
    Init();

}

CDkm::~CDkm(void)
{
    Close();
        //Close();
    static int i = 0 ;
    Trace("",AnsiString(i).c_str());
    i++;
}

EN_LIGHT_TYPE CDkm::GetLightType()
{
    return  ltDkm ;
}

bool CDkm::Init()
{
    //LoadPara(_sPath);

    Rs232c = new TRS232C ;
    //Rs232c -> Open(0) ;

    if(!Rs232c -> Open(0)) ShowMessage("Light Controler Rs232 Port Open Fail") ;
    else                   {}//Rs232c -> CallBackReg(procComH);



}

bool CDkm::Close()
{
    Rs232c -> Close() ;
    delete Rs232c ;
}

bool CDkm::SetBright(int _iCh , int _iVal)
{
    String sChanel , sVal , sTemp;

    if(_iCh <= 0 ) return false ;
    if(_iVal < 0   ) _iVal = 0   ;
    if(_iVal > 250 ) _iVal = 250 ;

    sChanel = sChanel.sprintf("%02d" , _iCh ) ;  //1~8
    sVal    = sVal   .sprintf("%03d" , _iVal) ;  //0~255

    sTemp = "[" + sChanel + sVal ;// + (_iVal == 0 ? "0" : "1");
    Rs232c->SendData(sTemp.Length(),sTemp.c_str());
}

void CDkm::LoadPara(bool _bLoad , AnsiString _sPath)
{
}


