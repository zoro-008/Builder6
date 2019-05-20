//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Rs232Dispr.h"
#include "UtilDefine.h"
#include "SLogUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//이것이 트루이면 명령을 내리고 리턴될때까지 기달린다.
const bool bSyncReturn = true ;

//밑에 있는 4개 배열 나중에 주석 처리 하던가 범용성 있게 변경.....
AnsiString CRs232Dispr::GetChksum(AnsiString sData) //원래는 STX는 짤라서 보내 줘야 하는데 귀찮음.
{
    int iDataLength = sData.Length() ;
    int iChksum=0;
    int iHex1,iHex2;
    char sHex[3]={0,};
    AnsiString sReturn ;
    char cData ;
    int iTemp = 0;

    for(int i=1 ; i<=iDataLength; i++) {//STX는 포함 안하고 안시스트링은 첫문자가 1부터 시작이다 그래서 2부터.
        cData = sData[i] ;
        iTemp = cData;
    	iChksum = iChksum + cData;
    }
    iChksum = iChksum % 256;
    iChksum = 256 - iChksum;
    iHex1=iChksum/16;
    iHex2=iChksum%16;
    sprintf(sHex,"%X%X",iHex1,iHex2);
    sReturn = sHex ;
    return sReturn ;
}


void CRs232Dispr::RcvDisprData  (DWORD _cbInQue)
{
    int iLen;
    BYTE RcvBuff[300];
    AnsiString sRcvMsg = "" ;
    AnsiString sRcvMsgTest = "";
    memset(&RcvBuff, 0 , sizeof(RcvBuff));

    iLen = m_pRS232C->ReadData(_cbInQue, RcvBuff);

    sRcvMsg = (char*)RcvBuff ;

    //Trace("Musashi sRcvMsg",sRcvMsg.c_str());

    //RSD.sReadMsg = sRcvMsg;
    if(sRcvMsg.Length()){
             if(RcvBuff[0] == (char)0x04) m_sAskMsg = sRcvMsg;
        else if(RcvBuff[0] == (char)0x05) m_sAskMsg = sRcvMsg;
        else if(RcvBuff[0] == (char)0x06) m_sAskMsg = sRcvMsg;
        else {
            m_sReadMsg += sRcvMsg;
            Trace("Musashi m_sReadMsg",m_sReadMsg.c_str());
        }
    }


}

CRs232Dispr::CRs232Dispr(void)
{
    Init();
}
CRs232Dispr::~CRs232Dispr(void)
{
    Close();
}

void CRs232Dispr::Close()
{
    m_pRS232C -> Close();
    delete m_pRS232C;
}


void CRs232Dispr::Init()
{
    m_iSeqCycle = 0;
    m_iPreSeqCycle = 0;
    m_iCmdNo = dcNone;
    m_sAskMsg  = "";
    m_sReadMsg = "";
    m_sErrMsg  = "";

    m_iSylFill = 100;

    memset(&DispData , 0 , sizeof(TDispData));

    m_pRS232C = new TRS232C;

    if(!m_pRS232C->Open(rsDispensor)) ShowMessage("RS-232C Dispensor Open Failed!");
    else                              m_pRS232C->CallBackReg(RcvDisprData);
}

//bool CRs232Dispr::SendMsg(AnsiString _sMsg)
AnsiString CRs232Dispr::GetSendMsg(AnsiString _sMsg)
{
    AnsiString sSendMsg = "";

    sSendMsg  = (char)0x02;
    sSendMsg += _sMsg;
    sSendMsg += GetChksum(_sMsg);
    sSendMsg += (char)0x03;

    return sSendMsg;
}

bool CRs232Dispr::SetDisprOnOff(bool _bOnOff)
{

    if(m_bDisprOn  == _bOnOff) return false ;

    AnsiString sSendMsg;
    m_bDisprOn  = _bOnOff;
    m_iCmdNo    = dcDisprOnOff ;

    sSendMsg    = "04DI  ";
    sSendMsg    = GetSendMsg(sSendMsg);
    m_sSendMsg  = sSendMsg ;
    m_iSeqCycle = 10;
    m_sAskMsg   = "";
    m_sReadMsg  = "";
    m_sErrMsg   = "";


    return true ;
}

bool CRs232Dispr::SetSigmaModeOn()
{
    AnsiString sSendMsg;
    m_iCmdNo = dcSigmaModeOn;

    sSendMsg = "04SM  ";
    sSendMsg = GetSendMsg(sSendMsg);

    m_sSendMsg = sSendMsg ;

    m_iSeqCycle = 10;
    m_sAskMsg   = "";
    m_sReadMsg  = "";
    m_sErrMsg   = "";

    return true ;
}

bool CRs232Dispr::SetSigmaModeOff(int _iCh)
{
    AnsiString sSendMsg;
    m_iCmdNo = dcSigmaModeOff;

    if(_iCh > 100) _iCh = 100;
    if(_iCh <   1) _iCh =   1;


    String sTemp ;
    sTemp = sTemp.sprintf("%03d",_iCh);
    sSendMsg = "05CL" + sTemp ;

    m_sSendMsg = GetSendMsg(sSendMsg); ;

    m_iSeqCycle = 10;
    m_sAskMsg   = "";
    m_sReadMsg  = "";
    m_sErrMsg   = "";

    return true ;
}

bool CRs232Dispr::SetMode(bool _bManual)
{
    AnsiString sSendMsg;
    AnsiString sMode ;
    m_iCmdNo = dcMode;

    if(_bManual) sMode= "1";
    else         sMode= "0";

    sSendMsg = "04TM0" + sMode;
    sSendMsg = GetSendMsg(sSendMsg);
    m_sSendMsg = sSendMsg ;
    m_iSeqCycle = 10;
    m_sAskMsg   = "";
    m_sReadMsg  = "";
    m_sErrMsg   = "";

    return true ;
}

bool CRs232Dispr::SetLoadCh (int _iCh)
{
    AnsiString sSendMsg;
    AnsiString sCh     ;

    m_iCmdNo = dcLoadCh;

    if(_iCh > 100) _iCh = 100;
    if(_iCh <   1) _iCh =   1;

    sCh.printf("%02d", _iCh);

    sSendMsg = "04LH" + sCh ; //(String)_iCh;
    sSendMsg = GetSendMsg(sSendMsg);
    m_sSendMsg = sSendMsg ;

    m_iSeqCycle = 10;
    m_sAskMsg   = "";
    m_sReadMsg  = "";
    m_sErrMsg   = "";

    return true ;
}

bool CRs232Dispr::SetPTV(double _dPres , int _iTime, double _dVac)
{
    AnsiString sSendMsg;
    AnsiString sPres, sTime, sVac;
    int iLength;

    m_iCmdNo = dcPTV;

    int iPres = _dPres * 10 ;    //무사시쪽 단위가 0.1 kPa
    int iVac  = _dVac  * 100 ;   //무사시쪽 단위가 0.01 kPa



    sPres =  iPres;
    sTime = _iTime;
    sVac  =  iVac ;

    iLength = sPres.Length();
         if(iLength == 1) sPres = "000" + sPres;
    else if(iLength == 2) sPres = "00"  + sPres;
    else if(iLength == 3) sPres = "0"   + sPres;

    iLength = sTime.Length();
         if(iLength == 1) sTime = "000" + sTime;
    else if(iLength == 2) sTime = "00"  + sTime;
    else if(iLength == 3) sTime = "0"   + sTime;

    iLength = sVac .Length();
         if(iLength == 1) sVac = "000" + sVac;
    else if(iLength == 2) sVac = "00"  + sVac;
    else if(iLength == 3) sVac = "0"   + sVac;

    sSendMsg = "14PR  P" + sPres + "T" + sTime + "V-" + sVac;
    sSendMsg = GetSendMsg(sSendMsg);
    m_sSendMsg = sSendMsg ;

    m_iSeqCycle = 10;
    m_sAskMsg   = "";
    m_sReadMsg  = "";
    m_sErrMsg   = "";

    return true ;
}
bool CRs232Dispr::SetSyringeSize(int _iSize)
{
    AnsiString sSendMsg;
    AnsiString sSize   ;

    m_iCmdNo = dcSyringeSize;

    //Syringe Size
    // 3CC : 0 ,
    // 5CC : 1 ,
    //10CC : 2 ,
    //20CC : 3 ,
    //30CC : 4 ,
    //50CC : 5 ,
    //70CC : 6

    if(_iSize > 6) _iSize = 6;
    if(_iSize < 1) _iSize = 1;

    sSendMsg = "07SY001S0" + (String)_iSize;
    //sSendMsg = "08DA02S" + (String)_iSize + "A1" ;
    sSendMsg = GetSendMsg(sSendMsg);
    m_sSendMsg = sSendMsg ;
    m_iSeqCycle = 10;
    m_sAskMsg   = "";
    m_sReadMsg  = "";
    m_sErrMsg   = "";

    return true ;
}

bool CRs232Dispr::GetSylVolm(int _iCh) //잔양체크
{
    AnsiString sSendMsg;

    m_iCmdNo = dcSylFill;

    if(_iCh > 100) _iCh = 100;
    if(_iCh <   1) _iCh =   1;

    AnsiString sCh     ;
    sCh.printf("%03d", _iCh);
    sSendMsg = "08UL" + sCh + "D05"; //(String)_iCh;
    sSendMsg = GetSendMsg(sSendMsg);
    m_sSendMsg = sSendMsg ;

    m_iSeqCycle = 10;
    m_sAskMsg   = "";
    m_sReadMsg  = "";
    m_sErrMsg   = "";

    return true ;
}
bool CRs232Dispr::GetDispData(int _iCh) //토출세팅값 받아오기.
{
    AnsiString sSendMsg;

    m_iCmdNo = dcDispData;

    if(_iCh > 100) _iCh = 100;
    if(_iCh <   1) _iCh =   1;

    AnsiString sCh     ;
    sCh.printf("%03d", _iCh);
    sSendMsg = "08UL" + sCh + "D01"; //(String)_iCh;
    sSendMsg = GetSendMsg(sSendMsg);
    m_sSendMsg = sSendMsg ;

    m_iSeqCycle = 10;
    m_sAskMsg   = "";
    m_sReadMsg  = "";
    m_sErrMsg   = "";

    return true ;
}

CRs232Dispr::TDispData CRs232Dispr::GetDispData()
{
    return DispData ;
}

int CRs232Dispr::GetSylFill ()
{
    return m_iSylFill ;

}

AnsiString CRs232Dispr::GetErrMsg()
{

    return m_sErrMsg;
}

bool CRs232Dispr::GetMsgEnd()
{
    if(m_iCmdNo == dcNone) return true ;
    else                   return false;

}

bool CRs232Dispr::Update(void)
{
    if(m_iCmdNo == dcNone) return true ;

    AnsiString sSendMsg = "";
    AnsiString sPrcsMsg = "";

    AnsiString sMsgOk , sMsgErr;

    sMsgOk  = (char)0x02;
    sMsgOk += "02A02D";
    sMsgOk += (char)0x03;

    sMsgErr = (char)0x02;
    sMsgErr+= "02A22B";
    sMsgErr+= (char)0x03;


    /*
    통신 제어 코드.
    STX      0x02 //통신 시작 신호
    ETX      0x03 //통신 종단 신호(통신 문장 마지막)
    EOT      0x04 //통신 종료(통신 완료 후 종료 알림)
    ENQ      0x05 //통신 준비 신호(통신 시작 전 상태 체크)
    ACK      0x06 //ENQ후 통신 준비 알림
    CAN(can) 0x18 //Cancle 신호.
    */
    if(m_iPreSeqCycle != m_iSeqCycle){
        Trace("Musashi Step",String(m_iSeqCycle).c_str());

    }

    m_iPreSeqCycle = m_iSeqCycle;

    switch (m_iSeqCycle) {
        default: if(m_iSeqCycle) {
                      ShowMessage("Sequence Default"); //2015 JS
                 }
                 m_iCmdNo = dcNone ;
                 m_iSeqCycle = 0 ;
                 return true;

        //Send ENQ
        case 10: sSendMsg = (char)0x05;      //통신 준비 상태 체크
                 Trace("Musashi Send","Send ENQ");
                 m_sReadMsg=m_sAskMsg="";
                 m_pRS232C -> SendData(sSendMsg.Length() , sSendMsg.c_str());

                 m_tmDipsr.Clear();
                 m_iSeqCycle++;
                 return false ;

        //Receive ACK
        case 11: if(m_tmDipsr.OnDelay(true, 500)){
                     if(m_sAskMsg != (char)0x06){ //대기 상태가 아닐때..
                         m_sErrMsg = "ACK Error" ;
                         m_iSeqCycle = 0;
                         return false;
                     }
                 }
                 if(m_sAskMsg != (char)0x06) return false;
                 m_iSeqCycle++;
                 return false;

        //Send COMMAND
        case 12:
                 Trace("Musashi Send","Send COMMAND");
                 m_sReadMsg=m_sAskMsg="";
                 m_pRS232C -> SendData(m_sSendMsg.Length() , m_sSendMsg.c_str());  //통신 커맨드 전송

                 m_tmDipsr.Clear();
                 m_iSeqCycle++;
                 return false;

        //Receive A0
        case 13: if(m_tmDipsr.OnDelay(true, 500)){
                     if(m_sReadMsg == sMsgErr){     //통신 정상 전송이 아닐때
                         m_iSeqCycle = 50;
                         return false;
                     }
                 }
                 if(m_sReadMsg != sMsgOk) return false;


                 //업로드계는 한번 더 주고 받아야함.
                 if(m_iCmdNo == dcSylFill ||
                    m_iCmdNo == dcDispData){
                     m_iSeqCycle=20;
                     return false ;
                 }
                 m_iSeqCycle=30; //다운로드계는 명령이 30번으로 가서 끝남.
                 return false;
         //Send ACK
        case 20: sSendMsg = (char)0x06;      //통신 준비 상태 체크
                 Trace("Musashi Send","Send ACK");
                 m_sReadMsg=m_sAskMsg="";
                 m_pRS232C -> SendData(sSendMsg.Length() , sSendMsg.c_str());

                 m_tmDipsr.Clear();
                 m_iSeqCycle++;
                 return false;

        //Receive Data
        case 21: if(m_tmDipsr.OnDelay(true, 500)){ //타임아웃.
                     m_iSeqCycle = 50;
                     return false;
                 }

                 if(m_sReadMsg.Pos((char)0x03)==0) return false;
                 //if(m_sReadMsg != sMsgOk) { //통신 정상 전송이 아닐때
                 //    m_iSeqCycle = 50 ;
                 //    return false ;
                 //}


                 if(m_iCmdNo == dcSylFill ) {
                     String sTemp ;
                     sTemp = m_sReadMsg.SubString(9,3);//stx08DA05R050C7etx
                     m_iSylFill = sTemp.ToIntDef(m_iSylFill) ;
                 }
                 if(m_iCmdNo == dcDispData) {
                     String sTemp ;

                     sTemp = m_sReadMsg.SubString( 9,4); //토출압력 4자리
                     DispData.dPrsPres = sTemp.ToIntDef(DispData.dPrsPres*10 ) /10.0 ;

                     sTemp = m_sReadMsg.SubString(20,4); //배큠압력 4자리
                     DispData.dVacPres = sTemp.ToIntDef(DispData.dVacPres*100) /100.0 ;

                     sTemp = m_sReadMsg.SubString(25,1); //모드 1자리
                     if(sTemp.ToIntDef(0) == 2 || sTemp.ToIntDef(0) == 3){
                         DispData.bSigmaMode = true ;
                     }
                     else {
                         DispData.bSigmaMode = false ;
                     }
                 }
                 m_iSeqCycle = 30;
                 return false;


        case 30: sSendMsg = (char)0x04;                                         //통신 종료 메시지 전송
                 Trace("Musashi Send","Send End");
                 m_sReadMsg=m_sAskMsg="";
                 m_pRS232C -> SendData(sSendMsg.Length() , sSendMsg.c_str());


                  m_iCmdNo = dcNone ;
                 m_iSeqCycle=0;
                 return true;

        //OnErr
        case 50: sSendMsg = (char)0x18;                                         //통신 커맨드 취소 신호 전송
                 m_sReadMsg=m_sAskMsg="";
                 m_pRS232C -> SendData(sSendMsg.Length() , sSendMsg.c_str());

                 sSendMsg = (char)0x04;                                         //통신 종료 메시지 전송
                 m_pRS232C -> SendData(sSendMsg.Length() , sSendMsg.c_str());
                 Trace("Musashi Send","Error");
                 m_sErrMsg="Communication Error";
                  m_iCmdNo = dcNone ;
                 m_iSeqCycle = 0;
                 return true;
    }
}

