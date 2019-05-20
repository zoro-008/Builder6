//---------------------------------------------------------------------------
#pragma hdrstop
#include "LightUnit.h"
#include "UserINI.h"
//#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

CLight ** CLight::m_apLight =NULL;
int CLight::m_MaxLight = 0 ;
int CLight::m_iMaxDkmSerial = 0 ;


int CLight::InitLight()
{
    TUserINI UserINI;
    String   sPath = EXE_FOLDER + "\\Util\\Vision\\LightPara.ini" ;

    int iLightCnt = 0 ;//총카메라 갯수.

    //IMI CAM Count
    //Setting
    UserINI.Load(sPath.c_str() , "Common" , "m_iMaxDkmSerial" , m_iMaxDkmSerial );
    if(m_iMaxDkmSerial){
        CDkmSerial::InitDkmSerial();
        if(m_iMaxDkmSerial != CDkmSerial::m_iMaxDkmSerial) ShowMessage("Initialed DkmSerial Light Count is Wrong!");
        iLightCnt += m_iMaxDkmSerial ;
    }


    //etc...
    //위의 imi 참고 해서 만들면됌.
    //int      iEurLightCnt ;
    //UserINI.Load(sPath.c_str() , "Common" , "EurLightCnt" , iEurLightCnt );
    //CEur::InitEur();
    //iLightCnt += CEur::m_iMaxEur ;



    //Make Lightera Pointer Memory.
    m_apLight = new CLight *[iLightCnt];


    int iMakeLightCnt = 0 ;
    //Make DkmSerial
    for(int i = 0 ; i < m_iMaxDkmSerial  ; i++) {
        m_apLight[i] = new CDkmSerial();
        iMakeLightCnt++;
    }

    //etc...
    //for(int i = 0 ; i < iEurLightCnt ; i++) {
    //    m_apLight[i+iMakeLightCnt] = new CEur();
    //    iMakeLightCnt++
    //}

    return iLightCnt ;
}

bool CLight::CloseLight()
{
    int iDeleteLightCnt = 0 ;
    //IMI CAM Count
    //Setting
    if(m_iMaxDkmSerial) {

        for(int i = 0 ; i < m_iMaxDkmSerial ; i++) {
            delete m_apLight[i] ;
            m_apLight[i] = NULL ;
        }
        CDkmSerial::CloseDkmSerial();
        iDeleteLightCnt += m_iMaxDkmSerial ;
    }

    delete [] m_apLight ;
    m_apLight = NULL ;

    return true ;
}


//ComboBoxList        ,
void  SetList(TValueListEditor * _vePara , int _iRow , String _sList){
    TStringList * slList = new TStringList ;
    slList->CommaText = _sList ;

    _vePara ->ItemProps[_iRow]-> EditStyle = esPickList ;
    _vePara ->ItemProps[_iRow]-> PickList  = slList     ;

    delete slList ;
}

void UpdateListPara(bool _bToTable , TValueListEditor * _vePara , CLinkedList<CValue2*> * _pList){
    CValue2* Value ;
    if(_bToTable) {
        //_vePara -> Strings -> Clear() ;
        //_vePara -> DisplayOptions >> doColumnTitles ;
        //int iRowCount  = _vePara -> RowCount ;
        int iRowCount = _vePara -> Strings -> Count ;
        for(int i = 0 ; i < _pList->GetDataCnt() ; i++){
            Value = _pList->GetCrntData(!i);
                 if(Value->GetValType()=="bool"  )_vePara -> Values[Value->GetName()] = Value->GetBoolStr();
            else if(Value->GetValType()=="int"   ){
                if(Value -> GetSelList() == "")   _vePara -> Values[Value->GetName()] = Value->GetInt    ();//int이고 SelList있을때 확인해야함.. 숫자로 되는지 아니면 Text로 되는지.
                else                              _vePara -> Values[Value->GetName()] = Value->GetIntString();//int이고 SelList있을때 확인해야함.. 숫자로 되는지 아니면 Text로 되는지.

            }
            else if(Value->GetValType()=="double")_vePara -> Values[Value->GetName()] = Value->GetDouble ();
            else if(Value->GetValType()=="String")_vePara -> Values[Value->GetName()] = Value->GetString ();
            if(Value->GetSelList()!="")SetList(_vePara , iRowCount + i , Value->GetSelList());
        }
    }
    else {
        for(int i = 0 ; i < _pList->GetDataCnt() ; i++){
            Value = _pList->GetCrntData(!i);
                 if(Value->GetValType()=="bool"  ) Value->SetBoolFromStr  (_vePara -> Values[Value->GetName()]) ;
            else if(Value->GetValType()=="int"   ) Value->SetIntFromStr   (_vePara -> Values[Value->GetName()]) ;
            else if(Value->GetValType()=="double") Value->SetDoubleFromStr(_vePara -> Values[Value->GetName()]) ;
            else if(Value->GetValType()=="String") Value->Set             (_vePara -> Values[Value->GetName()]) ;
        }
    }
}

void LoadSaveList(bool _bLoad , String _sIniPath , String _sSection , CLinkedList<CValue2*> * _pList){
    TUserINI Ini ;
    if(_bLoad) {
        CValue2* Value ;
        for(int i = 0 ; i < _pList->GetDataCnt() ; i++){
            Value = _pList->GetCrntData(!i);
                 if(Value->GetValType()=="bool"  )Ini.Load(_sIniPath, _sSection , Value->GetBinName(), (bool  *)Value->GetValAdd());
            else if(Value->GetValType()=="int"   )Ini.Load(_sIniPath, _sSection , Value->GetBinName(), (int   *)Value->GetValAdd());
            else if(Value->GetValType()=="double")Ini.Load(_sIniPath, _sSection , Value->GetBinName(), (double*)Value->GetValAdd());
            else if(Value->GetValType()=="String")Ini.Load(_sIniPath, _sSection , Value->GetBinName(), (String*)Value->GetValAdd());
        }
    }
    else {
        CValue2* Value ;
        for(int i = 0 ; i < _pList->GetDataCnt() ; i++){
            Value = _pList->GetCrntData(!i);
                 if(Value->GetValType()=="bool"  )Ini.Save(_sIniPath, _sSection , Value->GetBinName(), (bool  *)Value->GetValAdd());
            else if(Value->GetValType()=="int"   )Ini.Save(_sIniPath, _sSection , Value->GetBinName(), (int   *)Value->GetValAdd());
            else if(Value->GetValType()=="double")Ini.Save(_sIniPath, _sSection , Value->GetBinName(), (double*)Value->GetValAdd());
            else if(Value->GetValType()=="String")Ini.Save(_sIniPath, _sSection , Value->GetBinName(), (String*)Value->GetValAdd());
        }
    }
}













/*
void InitLight (ELightType * _iLightType)
{
    for(int i = 0 ; i < MAX_LIGHT_ID ; i++) {
             if(_iLightType[i] == ltDkm) { Light[i] = new CDkm() ; }
        else                             { ShowMessage("Unknown Light Type!");}
    }
}
void CloseLight()
{
    for(int i = 0 ; i < MAX_LIGHT_ID ; i++) {
        Light[i]->Close();
        delete Light [i] ;
    }
}
*/

bool CDkmSerial::InitDkmSerial()
{
    //할게 없음.
    return true ;
}
bool CDkmSerial::CloseDkmSerial()
{
    return true ;
}


CDkmSerial::CDkmSerial(void)
{
    Init();

}

CDkmSerial::~CDkmSerial(void)
{
    Close();
}

ELightType CDkmSerial::GetType()
{
    return  ltDkmSerial ;
}

bool CDkmSerial::Init()
{
    //LoadPara(_sPath);

    LoadPara(true , EXE_FOLDER + "\\Util\\Vision\\LightPara.ini");
    //LoadPara(false , EXE_FOLDER + "\\Util\\Vision\\LightPara.ini");

    Rs232c = new TRS232C ;
    TRS232C::TPara SetPara ;
    SetPara.PortNo   = Para.iPortNo   ;
    SetPara.BaudRate = Para.iBaudRate ;
    SetPara.ByteSize = Para.iByteSize ;
    SetPara.StopBits = Para.iStopBits ;
    SetPara.Parity   = Para.iParity   ;


    if(!Rs232c -> Open(SetPara)) {
        ShowMessage("Light Controler Rs232 Port Open Fail") ;
        return false ;
    }

    return true ;

    //if(!Rs232c -> Open(3)) ShowMessage("Light Controler Rs232 Port Open Fail") ;
    //else                   {}//Rs232c -> CallBackReg(procComH);



}

bool CDkmSerial::Close()
{
    Rs232c -> Close() ;
    if(Rs232c){
        delete Rs232c ;
        Rs232c = NULL ;
    }

    return true ;
}

bool CDkmSerial::ApplyUserPara(TLightUserPara * _pLightUserPara)
{
    String sChanel , sVal ;
    String sTemp="";

    TUserPara * UserPara = (TUserPara *)_pLightUserPara ;


    if(SetUserPara.iCh1 != UserPara->iCh1) {
        sChanel = sChanel.sprintf("%02d" , 01            ) ;  //1~8
        sVal    = sVal   .sprintf("%03d" , UserPara->iCh1) ;  //0~255
        sTemp  += "[" + sChanel + sVal ;// + (_iVal == 0 ? "0" : "1");
        SetUserPara.iCh1 = UserPara->iCh1 ;
    }
    if(SetUserPara.iCh2 != UserPara->iCh2) {
        sChanel = sChanel.sprintf("%02d" , 02            ) ;  //1~8
        sVal    = sVal   .sprintf("%03d" , UserPara->iCh2) ;  //0~255
        sTemp  += "[" + sChanel + sVal ;// + (_iVal == 0 ? "0" : "1");
        SetUserPara.iCh2 = UserPara->iCh2 ;
    }
    if(SetUserPara.iCh3 != UserPara->iCh3) {
        sChanel = sChanel.sprintf("%02d" , 03            ) ;  //1~8
        sVal    = sVal   .sprintf("%03d" , UserPara->iCh3) ;  //0~255
        sTemp  += "[" + sChanel + sVal ;// + (_iVal == 0 ? "0" : "1");
        SetUserPara.iCh3 = UserPara->iCh3 ;
    }

    if(sTemp == "") return true ;


//    double dSttTime , dEndTime;
//    dSttTime = GetTime() ;
    Rs232c->SendData(sTemp.Length(),sTemp.c_str());
//    dEndTime = GetTime() ;

//    dSttTime = dEndTime - dSttTime ;

    return true ;
}

void CDkmSerial::LoadPara(bool _bLoad , AnsiString _sPath)
{
    //Local Var.
    TUserINI   UserINI;
    String sPath  ;
    String sTemp  ;

    //Set Dir.
    sTemp = String(m_iLightNo)+"_DkmSerial" ;
    sPath = _sPath;

    //if(FileExists(sPath)

    if(_bLoad) {
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "Para.iPortNo  " , Para.iPortNo   );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "Para.iBaudRate" , Para.iBaudRate );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "Para.iByteSize" , Para.iByteSize );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "Para.iStopBits" , Para.iStopBits );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "Para.iParity  " , Para.iParity   );
    }
    else {
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "Para.iPortNo  " , Para.iPortNo   );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "Para.iBaudRate" , Para.iBaudRate );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "Para.iByteSize" , Para.iByteSize );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "Para.iStopBits" , Para.iStopBits );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "Para.iParity  " , Para.iParity   );
    }
}

void CDkmSerial::UpdateUserPara (bool _bToTable , TValueListEditor * _vePara,TLightUserPara * _pLightUserPara)
{
    TUserPara * pUserPara = (TUserPara *)_pLightUserPara ;
    if(_bToTable) {
        UpdateListPara(_bToTable , _vePara , &pUserPara->List);
    }
    else {
        UpdateListPara(_bToTable , _vePara , &pUserPara->List);
    }

}

void CDkmSerial::LoadSaveUserPara(bool _bLoad  , String _sFilePath,TLightUserPara * _pLightUserPara )
{
/*
    TUserINI Ini ;
    TUserPara * pUserPara = (TUserPara *)_pLightUserPara ;
    if(_bLoad) {
        Ini.Load(_sFilePath, "DkmSerialUserPara" , "iCh1", pUserPara->iCh1);
        Ini.Load(_sFilePath, "DkmSerialUserPara" , "iCh2", pUserPara->iCh2);
        Ini.Load(_sFilePath, "DkmSerialUserPara" , "iCh3", pUserPara->iCh3);
    }
    else {
        Ini.Save(_sFilePath, "DkmSerialUserPara" , "iCh1", pUserPara->iCh1);
        Ini.Save(_sFilePath, "DkmSerialUserPara" , "iCh2", pUserPara->iCh2);
        Ini.Save(_sFilePath, "DkmSerialUserPara" , "iCh3", pUserPara->iCh3);
    }
*/
    TUserPara * pUserPara = (TUserPara *)_pLightUserPara ;
    if(_bLoad) {
        if(FileExists(_sFilePath)){
            LoadSaveList(_bLoad , _sFilePath , "DkmSerialUserPara" , &pUserPara->List);
        }
        else {
            pUserPara->SetDefault();
        }
    }
    else {
        LoadSaveList(_bLoad , _sFilePath , "DkmSerialUserPara" , &pUserPara->List);
    }



}


