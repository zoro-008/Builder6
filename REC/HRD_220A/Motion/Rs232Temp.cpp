//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Rs232Temp.h"
#include <stdio.h>
#include "UtilDefine.h" 
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#define  MAX_TEMP 200

#pragma package(smart_init)

CTempConUnit  Rs232_TempCh ;

void CTempConUnit::RcvTempCon  (DWORD _cbInQue)
{
    int iLen;
    BYTE RcvBuff[300];
    AnsiString sRcvMsg = "" ;
    memset(&RcvBuff, 0 , sizeof(RcvBuff));

    iLen = m_pRS232C->ReadData(_cbInQue, RcvBuff);

    sRcvMsg = (char*)RcvBuff ;
//    SetRcvMsg(sRcvMsg);
    //static AnsiString sRcvMsg = "";

    static bool bDispErr = true ;

    sRcvMsgTemp += sRcvMsg ;

    if(!sRcvMsgTemp.Pos("\r\n")) return ;

    //ShowMessage(sRcvMsg);
    //Trace("m_pRS232C_Msg",sRcvMsgTemp.c_str());

    m_sRcvMsg = sRcvMsgTemp ;
    sRcvMsg = "" ;
    sRcvMsgTemp = "";

    AnsiString sCheckStr ;
    AnsiString sCheckSum ;
    AnsiString sSum      ;

    sCheckStr = m_sRcvMsg.SubString(1 , m_sRcvMsg.Pos("\r\n")-3);
    sCheckSum = m_sRcvMsg.SubString(m_sRcvMsg.Pos("\r\n")-2 , 2);

    //Trace("m_pRS232C_Msg","1");
    //데이터 맞게 들어왔는지 확인.
    sSum = GetChksum(sCheckStr);


    //Trace("m_pRS232C_Msg","2");
    if(bDispErr && sSum != sCheckSum) { //체크썸 확인 해보자.
        ShowMessage("TempCon CheckSum is Wrong!!!!");
        bDispErr = false ;
    }

    if(!m_sRcvMsg.Pos("OK")) {
        if(bDispErr) {
            ShowMessage("TempCon NG Received");
            bDispErr = false ;
        }
        return ;
    }


    if(m_iLastRqst == trNone) {

    }
    else if(m_iLastRqst == trRqstCrntTemp ) {
        AnsiString sTempData ;
        AnsiString sOneData  ;

        //Trace("m_pRS232C_Msg","5");
        sTempData = sCheckStr ;


        sTempData.Delete(1,sTempData.Pos("OK")+2) ;
        //Trace("sTempData",sTempData.c_str());
        for(int i = 0 ; i < MAX_TEMP_CH ; i++) {
            sOneData = sTempData.SubString(1 + (5*i),4) ;
            //Trace("sOneData",sOneData.c_str());
            m_iCrntTemp[i] = StrToIntDef("0x"+sOneData,m_iCrntTemp[i]) ;
        }
        //Trace("m_pRS232C_Msg","7");
    }
    else if(m_iLastRqst == trSetTemp ) {
    }
    else if(m_iLastRqst == trSetTempAll ) {
    }
    else {
    }  

    m_iLastRqst = trNone ;
    
}



__fastcall CTempConUnit::CTempConUnit(void)
{
    Init();
}

__fastcall CTempConUnit::~CTempConUnit(void)
{


}

void CTempConUnit::Init()
{
    m_iLastRqst = trNone ;
    m_sRcvMsg = "";

    memset(m_iSetTemp , 0 ,sizeof(int) * MAX_TEMP_CH) ;
    memset(m_iCrntTemp, 0 ,sizeof(int) * MAX_TEMP_CH) ;

    //LoadTemperature();
    m_pRS232C = new TRS232C ;
    if(!m_pRS232C->Open(rsTemperature)) ShowMessage("RS-232C TempCon Open Failed!");
    else                                m_pRS232C->CallBackReg(RcvTempCon);

}
void CTempConUnit::Close()
{
}
bool CTempConUnit::RqstCrntTemp()
{
    AnsiString sSendMsg ;
    AnsiString sTemp    ;

    sSendMsg  = "01" ; //컨트롤러 아이디 한개만 쓰기에 무조건 1
    sSendMsg += "DRR"; //D레지스터에 여러개 한번에 요청 명령.
    sSendMsg += ","  ;
    sSendMsg += sTemp.sprintf("%02d",MAX_TEMP_CH); //데이터 갯수.. 8개
    for(int i = 1 ; i <= MAX_TEMP_CH ; i++) {
        sSendMsg += ","  ;
        sSendMsg += sTemp.sprintf("%04d",i); //현제 온도 메모리 영역 0001~0008
    }
    bool bRet ;
    bRet = SendMsg(sSendMsg);

    m_iLastRqst = trRqstCrntTemp ;

    return bRet ;
}

bool CTempConUnit::SetTemp(int _iCh , int _iTemp)
{
    if(_iCh < 1 || _iCh >9) return false ;
    if(_iTemp < 0  )_iTemp = 0   ; if(_iTemp > MAX_TEMP)_iTemp = MAX_TEMP;

    AnsiString sSendMsg ;

    sSendMsg  = "01" ; //컨트롤러 아이디 한개만 쓰기에 무조건 1
    sSendMsg += "DWR"; //D레지스터에 여러개 한번에 쓰는 명령.
    sSendMsg += ","  ;
    sSendMsg += "01" ; //데이터 갯수.. 1개
    sSendMsg += ","  ;
    if(_iCh < 5) sSendMsg += AnsiString(1001 + (_iCh - 1) * 8 ) ; //온도 세팅 하는 메모리 1001번 부터 시작. 1번채널 1번존  존의 개념을 모르겠음.
    else         sSendMsg += AnsiString(1101 + (_iCh - 5) * 8 ) ; //온도 세팅 하는 메모리 1001번 부터 시작. 1번채널 1번존  존의 개념을 모르겠음.
    sSendMsg += ","  ;
    sSendMsg += IntToHex(_iTemp , 4)  ; //의 반대는 StrToInt

    m_iSetTemp[_iCh - 1] = _iTemp ;
    bool bRet ;
    bRet = SendMsg(sSendMsg);

    m_iLastRqst = trSetTemp ;

    return bRet ;
}

bool CTempConUnit::SetTempAll (int _iTemp1 ,           //한큐 온도 세팅.
                               int _iTemp2 ,
                               int _iTemp3 )
{
     //if(_iTemp  < 0  )_iTemp = 0   ; if(_iTemp > 250)_iTemp = 250 ;
     //if(_iTemp1 < 0  )_iTemp1 = 0   ; if(_iTemp1 > OM.MstOptn.iMaxTemp)_iTemp1 = OM.MstOptn.iMaxTemp ;
     //if(_iTemp2 < 0  )_iTemp2 = 0   ; if(_iTemp2 > OM.MstOptn.iMaxTemp)_iTemp2 = OM.MstOptn.iMaxTemp ;
     //if(_iTemp3 < 0  )_iTemp3 = 0   ; if(_iTemp3 > OM.MstOptn.iMaxTemp)_iTemp3 = OM.MstOptn.iMaxTemp ;

    AnsiString sSendMsg ;
    AnsiString sTemp    ;
    //for(int i = 0 ; i < MAX_TEMP_CH ; i++){
    //    m_iSetTemp[i] = _iTemp ;
    //}
    m_iSetTemp[0] = _iTemp1 ;
    m_iSetTemp[1] = _iTemp2 ;
    m_iSetTemp[2] = _iTemp3 ;

    sSendMsg  = "01" ; //컨트롤러 아이디 한개만 쓰기에 무조건 1
    sSendMsg += "DWR"; //D레지스터에 여러개 한번에 쓰는 명령.
    sSendMsg += ","  ;
    sSendMsg += sTemp.sprintf("%02d",MAX_TEMP_CH); //데이터 갯수.. 8개
    for(int i = 1 ; i <= MAX_TEMP_CH ; i++) {
        sSendMsg += ","  ;
        if(i <  5) sSendMsg += AnsiString(1001 + (i - 1) * 8 ) ; //온도 세팅 하는 메모리 1001번 부터 시작. 1번채널 1번존  존의 개념을 모르겠음.
        else       sSendMsg += AnsiString(1101 + (i - 5) * 8 ) ; //온도 세팅 하는 메모리 1001번 부터 시작. 1번채널 1번존  존의 개념을 모르겠음.
        sSendMsg += ","  ;
        sSendMsg += IntToHex(m_iSetTemp[i-1] , 4)  ; //의 반대는 StrToInt
    }

    bool bRet ;
    bRet = SendMsg(sSendMsg);
    m_iLastRqst = trSetTempAll ;
    return bRet ;
}

int CTempConUnit::GetCrntTemp(int _iCh)
{
    //RqstCrntTemp();
    if(_iCh < 1 || _iCh >9) return 0 ;
    return m_iCrntTemp[_iCh - 1] ;

}

int CTempConUnit::GetSetTemp (int _iCh)
{
    //RqstCrntTemp();
    if(_iCh < 1 || _iCh >9) return 0 ;
    return m_iSetTemp[_iCh - 1] ;
}

AnsiString CTempConUnit::GetChksum(AnsiString sData) //원래는 STX는 짤라서 보내 줘야 하는데 귀찮음.
{
    int iDataLength = sData.Length() ;
    int iChksum=0;
    int iHex1,iHex2;
    char sHex[3]={0,};
    AnsiString sReturn ;
    char cData ;

    //Trace("m_pRS232C_Msg","3");
    for(int i=2 ; i<= iDataLength ; i++) {//STX는 포함 안하고 안시스트링은 첫문자가 1부터 시작이다 그래서 2부터.
        cData = sData[i] ;
    	iChksum = iChksum + cData;
    }
    iChksum = iChksum % 256;
    iHex1=iChksum/16;
    iHex2=iChksum%16;
    sprintf(sHex,"%X%X",iHex1,iHex2);
    sReturn = sHex ;
    //Trace("m_pRS232C_Msg","4");
    return sReturn ;
}

bool CTempConUnit::SendMsg(AnsiString _sMsg)
{
    AnsiString sSendMsg ;

    sSendMsg  = (char)0x02;
    sSendMsg += _sMsg.Trim() ;
    sSendMsg += GetChksum(sSendMsg);
    sSendMsg += (char)0x0D;
    sSendMsg += (char)0x0A;

    return m_pRS232C -> SendData(sSendMsg.Length() , sSendMsg.c_str());
}

void CTempConUnit::SetRcvMsg(AnsiString _sMsg)
{
    static AnsiString sRcvMsg = "";

    static bool bDispErr = true ;

    sRcvMsg += _sMsg ;

    if(!sRcvMsg.Pos("\r\n")) return ;

    //ShowMessage(sRcvMsg);

    Trace("m_pRS232C_Msg",sRcvMsg.c_str());

    m_sRcvMsg = sRcvMsg ;
    sRcvMsg = "" ;

    AnsiString sCheckStr ;
    AnsiString sCheckSum ;
    AnsiString sSum      ;

    sCheckStr = m_sRcvMsg.SubString(1 , m_sRcvMsg.Pos("\r\n")-3);
    sCheckSum = m_sRcvMsg.SubString(m_sRcvMsg.Pos("\r\n")-2 , 2);

    //Trace("m_pRS232C_Msg","1");
    //데이터 맞게 들어왔는지 확인.
    sSum = GetChksum(sCheckStr);


    //Trace("m_pRS232C_Msg","2");
    if(bDispErr && sSum != sCheckSum) { //체크썸 확인 해보자.
        ShowMessage("TempCon CheckSum is Wrong!!!!");
        bDispErr = false ;
    }

    if(!m_sRcvMsg.Pos("OK")) {
        if(bDispErr) {
            ShowMessage("TempCon NG Received");
            bDispErr = false ;
        }
        return ;
    }


    if(m_iLastRqst == trNone) {

    }
    else if(m_iLastRqst == trRqstCrntTemp ) {
        AnsiString sTempData ;
        AnsiString sOneData  ;

        //Trace("m_pRS232C_Msg","5");
        sTempData = sCheckStr ;


        sTempData.Delete(1,sTempData.Pos("OK")+2) ;

        for(int i = 0 ; i < MAX_TEMP_CH ; i++) {
            sOneData = sTempData.SubString(1 + (5*i),4) ;
            m_iCrntTemp[i] = StrToInt("0x"+sOneData) ;
        }
    }
    else if(m_iLastRqst == trSetTemp ) {
    }
    else if(m_iLastRqst == trSetTempAll ) {
    }
    else {
    }
}
//bool CTempConUnit::LoadTemperature()
//{
//    m_iSetTemp[0] = OM.CmnOptn.iTemperature1;
//    m_iSetTemp[1] = OM.CmnOptn.iTemperature2;
//    m_iSetTemp[2] = OM.CmnOptn.iTemperature3;
//    m_iSetTemp[3] = OM.CmnOptn.iTemperature4;
//    m_iSetTemp[4] = OM.CmnOptn.iTemperature5;
//    m_iSetTemp[5] = OM.CmnOptn.iTemperature6;
//    m_iSetTemp[6] = OM.CmnOptn.iTemperature7;
//    m_iSetTemp[7] = OM.CmnOptn.iTemperature8;
//}
