//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "Heater.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "Timer.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "Sequence.h"
#define  IDC_485ID   1009
#define  ZERO_MEMORY(s)  :: ZeroMemory(&s, sizeof(s))
//---------------------------------------------------------------------------
#pragma package(smart_init)

CHeater HTR ;
struct TTkQuery {    //명령할때
    unsigned char  id;
    unsigned char  func;
    unsigned char  start_addr_hi;
    unsigned char  start_addr_lo;
    unsigned char  size_hi;
    unsigned char  size_lo;          //1바이트
    unsigned short CRC16;            //2바이트
};
struct TTkResponse { //응답올때
    unsigned char  id;
    unsigned char  func;
    unsigned char  start_addr_hi;
    unsigned char  start_addr_lo;
    unsigned char  size_hi;
    unsigned char  size_lo;
    unsigned short CRC16;
};

CHeater::CHeater(void)
{
    Init();
}

CHeater::~CHeater(void)
{
    Close();
}
void CHeater::Init(void)
{
    Rs232_1 = new TRS232C (); //오토닉스 9핀 일때 1pin:-  2pin:+
    Rs232_2 = new TRS232C ();

    if(!Rs232_1->Open(0)) ShowMessage("Heater 1 Rs232 Port Open Fail") ;
    else                  Rs232_1->CallBackReg(RxCallback1);

    if(!Rs232_2->Open(1)) ShowMessage("Heater 2 Rs232 Port Open Fail") ;
    else                  Rs232_2->CallBackReg(RxCallback2);

    //Rs232c.SetCallBack(RxCallback);
    IO_SetY(yETC_HeatOn1,true);
    IO_SetY(yETC_HeatOn2,true);
    HTR.SetTemp(1,OM.DevOptn.iTempSet1);
    HTR.SetTemp(2,OM.DevOptn.iTempSet2);
    bStopTemp = false;
}
void CHeater::Close(void)
{
    Rs232_1 -> Close();
    delete Rs232_1 ;
    Rs232_2 -> Close();
    delete Rs232_2 ;
}

void CHeater::RxCallback1(DWORD _dTxCnt)
{
    BYTE    RcvBuff[64] ;
    String  sRcvMsg = "" ;
    int     iLen;
    String  sTemp1 , sTemp2 ;

    memset(&RcvBuff, 0 , sizeof(RcvBuff));

    iLen = HTR.Rs232_1->ReadData(_dTxCnt, RcvBuff);

    for(int i=0; i<iLen; i++){
        HTR.m_sRcvQue1[HTR.m_iMsgCnt1] = sRcvMsg.sprintf("%02x", RcvBuff[i]);
        HTR.m_iMsgCnt1++;
        if(HTR.m_iMsgCnt1>=7) {
            sTemp1 = HTR.m_sRcvQue1[3] ;
            sTemp2 = HTR.m_sRcvQue1[4] ;
            HTR.m_sRcvMsg1 = HTR.HexToInt(sTemp1) + HTR.HexToInt(sTemp2);
            return ;
        }
    }
}
void CHeater::RxCallback2(DWORD _dTxCnt)
{
    BYTE    RcvBuff[64] ;
    String  sRcvMsg = "" ;
    int     iLen;
    String  sTemp1 , sTemp2 ;

    memset(&RcvBuff, 0 , sizeof(RcvBuff));

    iLen = HTR.Rs232_2->ReadData(_dTxCnt, RcvBuff);


    for(int i=0; i<iLen; i++){
        HTR.m_sRcvQue2[HTR.m_iMsgCnt2] = sRcvMsg.sprintf("%02x", RcvBuff[i]);
        HTR.m_iMsgCnt2++;
        if(HTR.m_iMsgCnt2>=7) {
            sTemp1 = HTR.m_sRcvQue2[3] ;
            sTemp2 = HTR.m_sRcvQue2[4] ;
            HTR.m_sRcvMsg2 = HTR.HexToInt(sTemp1) + HTR.HexToInt(sTemp2);
            return ;
        }
    }

}

//---------------------------------------------------------------------------
int CHeater::HexToInt(String str)
{
   String temp, result;
   int index = 1;
   int imsi1 = 0;
   int imsi2 = 0;
   for(int Length=str.Length(); Length>0; Length--,index++)
   {
      temp = str.SubString(index,1);
           if(temp == "a" || temp == "A" ) temp =10;
      else if(temp == "b" || temp == "B" ) temp =11;
      else if(temp == "c" || temp == "C" ) temp =12;
      else if(temp == "d" || temp == "D" ) temp =13;
      else if(temp == "e" || temp == "E" ) temp =14;
      else if(temp == "f" || temp == "F" ) temp =15;
      else temp = temp.ToIntDef(0);

      if(Length != 1)
      {
          imsi1 = temp*(pow(16,(Length-1)));
      }
      else
      {
          imsi1 = temp*1;
      }
      imsi2 += imsi1;
  }
  return imsi2;
}

//오토닉스에서 제공 하는 함수.
unsigned short CRC16_0(unsigned char *nData, int wLength)
{
    static const unsigned short wCRCTable[] = {
       0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
       0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
       0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
       0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
       0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
       0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
       0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
       0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,

       0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
       0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
       0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
       0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
       0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
       0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
       0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
       0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,

       0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
       0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
       0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
       0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
       0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
       0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
       0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
       0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,

       0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
       0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
       0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
       0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
       0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
       0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
       0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
       0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040
    };

    unsigned char nTemp;
    unsigned short wCRCWord = 0xFFFF;

    while (wLength--) {
    	nTemp = *nData++ ^ wCRCWord;
    	wCRCWord >>= 8;
    	wCRCWord  ^= wCRCTable[nTemp];
    }

    return wCRCWord;
}

void CHeater::RqstTemp1()
{

    TTkQuery Query ;

    Query.id            = 0x01 ; //실제 컨트롤러는 0번.
    Query.func          = 4    ;
    Query.start_addr_hi = 0x03 ;
    Query.start_addr_lo = 0xE8 ; //28
    Query.size_hi       = 0x00 ;
    Query.size_lo       = 0x01 ;
    Query.CRC16         = CRC16_0((unsigned char*)&Query, sizeof(TTkQuery)-2);

    //Rs232c.SendData(sizeof(TTkQuery),(BYTE*)&Query);
    Rs232_1->SendData(sizeof(TTkQuery),(BYTE*)&Query);

}




//void CHeater::RqstTemp1()
//{
//    int iPort = 1009;
//    int m_nwCnt;
//    int m_nRegNoS[1000];
//    int m_nWDataS[1000];
//
//    ZeroMemory(m_nWDataS,1000);
//    ZeroMemory(m_nRegNoS,1000);
//
//    m_nwCnt = 1;
//    m_nRegNoS[0] = 305;
//    m_nWDataS[0] = 1;
//
//    int iAddr1,iAddr2;
//    int iCNT1,iCNT2;
//    int iReg0,iReg1,iReg2,iReg3;
//
//    char TxAddr[10]={0x00,};
//    char TxDRR[10]={0x00,};
//    char TxCNT[10]={0x00,};
//    char TxRegNo[10]={0x00,};
//
//    AnsiString str;
//    int		Pos = 0;
//    int		iCR;
//    int		iLF;
//    int		iLenTx;
//    char	Tx_Buf[1000] = {0x00,};
//    char	TptrCR[1000]={0x00,};
//    char	TptrLF[1000]={0x00,};
//    int iCnt;
//
//    memset(&Tx_Buf,0, sizeof(Tx_Buf));
//
//    iAddr1=iPort/10;
//    iAddr2=iPort%10;
//
//    iCNT1=m_nwCnt/10;
//    iCNT2=m_nwCnt%10;
//
//    sprintf(TxAddr,"%d%d",iAddr1,iAddr2);
//    sprintf(TxDRR,"%c%c%c",'D','R','R');
//    sprintf(TxCNT,"%d%d",iCNT1,iCNT2);
//
//    Tx_Buf[0]=0x02;
//    Tx_Buf[1]=TxAddr[0];
//    Tx_Buf[2]=TxAddr[1];
//    Tx_Buf[3]=TxDRR[0];
//    Tx_Buf[4]=TxDRR[1];
//    Tx_Buf[5]=TxDRR[2];
//    Tx_Buf[6]=0x2C;
//    Tx_Buf[7]=TxCNT[0];
//    Tx_Buf[8]=TxCNT[1];
//
//    for(iCnt=1;iCnt<=m_nwCnt;iCnt++) {
//        iReg0=m_nRegNoS[iCnt-1]/1000;
//        iReg1=(m_nRegNoS[iCnt-1]%1000)/100;
//        iReg2=(m_nRegNoS[iCnt-1]%100)/10;
//        iReg3=(m_nRegNoS[iCnt-1]%10);
//        sprintf(TxRegNo,"%d%d%d%d",iReg0,iReg1,iReg2,iReg3);
//        Tx_Buf[(iCnt-1)*5+9]=0x2C;
//        Tx_Buf[(iCnt-1)*5+10]=TxRegNo[0];
//        Tx_Buf[(iCnt-1)*5+11]=TxRegNo[1];
//        Tx_Buf[(iCnt-1)*5+12]=TxRegNo[2];
//        Tx_Buf[(iCnt-1)*5+13]=TxRegNo[3];
//    }
//
//    //if(m_iChksum>0){
//        iCnt=strlen(Tx_Buf);
//        Add_Chksum(Tx_Buf,iCnt);
//    //}
//
//    iCR = 13;
//    sprintf(TptrCR, "%1c", iCR);
//    iLF = 10;
//    sprintf(TptrLF, "%1c", iLF);
//
//    strcat(Tx_Buf, TptrCR);
//    strcat(Tx_Buf, TptrLF);
//
//    iLenTx = strlen(Tx_Buf);
//
//    Rs232_1->SendData(sizeof((LPSTR)Tx_Buf),(BYTE*)&iLenTx);
///*
//    TTkQuery Query ;
//
//    Query.id            = 0x01 ; //실제 컨트롤러는 0번.
//    Query.func          = 4    ;
//    Query.start_addr_hi = 0x03 ;
//    Query.start_addr_lo = 0xE8 ; //28
//    Query.size_hi       = 0x00 ;
//    Query.size_lo       = 0x01 ;
//    Query.CRC16         = CRC16_0((unsigned char*)&Query, sizeof(TTkQuery)-2);
//
//    Rs232c.SendData(sizeof(TTkQuery),(BYTE*)&Query);
//*/
//}

void CHeater::RqstTemp2()
{
    TTkQuery Query ;

    Query.id            = 0x01 ; //rs485 포트2개 여서 각각 아이디 1이다.
    Query.func          = 4    ;
    Query.start_addr_hi = 0x03 ;
    Query.start_addr_lo = 0xE8 ; //28
    Query.size_hi       = 0x00 ;
    Query.size_lo       = 0x01 ;
    Query.CRC16         = CRC16_0((unsigned char*)&Query, sizeof(TTkQuery)-2);

    Rs232_2->SendData(sizeof(TTkQuery),(BYTE*)&Query);
}

void CHeater::SetTemp(int _iNo , int _iTemp)
{
    String sDataHi,sDataLo;
    int    iDataHi,iDataLo;

    String sTemp  = IntToHex(_iTemp,4);

    sDataHi = sTemp.SubString(1,2);
    sDataLo = sTemp.SubString(3,4);

    iDataHi = HexToInt(sDataHi);
    iDataLo = HexToInt(sDataLo);


    TTkQuery Query ;

    Query.id            = 0x01 ;//RS485 포트 2개 쓰기 때문에 히터아이디는 둘다 1 
    Query.func          = 0x06 ;
    Query.start_addr_hi = 0x00 ;
    Query.start_addr_lo = 0x00 ;
    Query.size_hi       = 0x00 + iDataHi;
    Query.size_lo       = 0x00 + iDataLo;
    Query.CRC16         = CRC16_0((unsigned char*)&Query, sizeof(TTkQuery)-2);

    //Rs232_1->SendData(sizeof(TTkQuery),(BYTE*)&Query);
    if(_iNo == 1) Rs232_1->SendData(sizeof(TTkQuery),(BYTE*)&Query);
    else          Rs232_2->SendData(sizeof(TTkQuery),(BYTE*)&Query);

}

float CHeater::GetTemp1()
{
    return m_fCrntTemp1 ;
}

float CHeater::GetTemp2()
{
    return m_fCrntTemp2 ;
}

void CHeater::Update()
{
    //500미리 마다 한번만 돔.
    static CDelayTimer tmStepDelay ;
    static CDelayTimer tmStopDelay ;
    if(!tmStepDelay.OnDelay(true , 300)) return ;
    tmStepDelay.Clear();
//    tmStopDelay.Clear();
    bool bRet = false ;
    float fTemp1 = 0.0 ;
    float fTemp2 = 0.0 ;

    static int iStep = 10 ;
    switch (iStep) {
        default : iStep = 10;
                  return ;

        case  10: RqstTemp1();
                  m_iMsgCnt1 = 0 ;
                  iStep++;
                  return ;

        case  11: if(m_iMsgCnt1 < 7) return ;

                  m_fCrntTemp1 = StrToFloatDef(m_sRcvMsg1 , 0) ;

                  RqstTemp2();
                  m_iMsgCnt2 = 0 ;
                  iStep++;
                  return ;

        case  12: if(m_iMsgCnt2 < 7) return ;
                  m_fCrntTemp2 = StrToFloatDef(m_sRcvMsg2 , 0) ;
                  iStep++;
                  return ;

        case  13:
                  if(tmStopDelay.OnDelay(SEQ._iSeqStat != ssRunning && SEQ._iSeqStat != ssRunWarn && IO_GetX(xRAL_PkgInChk), OM.DevOptn.iStopWaitSec * 1000)) {
                      SetTemp(1,OM.DevOptn.iStopTemp);
                      bStopTemp = true ;

                      bRet = false ;
                      fTemp1 = GetTemp1() ;
                      fTemp2 = GetTemp2() ;
                      if((OM.DevOptn.iStopTemp + OM.DevOptn.iTempGap) < (int)GetTemp1() ) bRet = true ;
                      if((OM.DevOptn.iStopTemp - OM.DevOptn.iTempGap) > (int)GetTemp1() ) bRet = true ;
                      if((OM.DevOptn.iStopTemp + OM.DevOptn.iTempGap) < (int)GetTemp2() ) bRet = true ;
                      if((OM.DevOptn.iStopTemp - OM.DevOptn.iTempGap) > (int)GetTemp2() ) bRet = true ;

                      if(!IO_GetY(yRAL_AirBlower) && bRet) IO_SetY(yRAL_AirBlower,true);
                      else if(IO_GetY(yRAL_AirBlower) && !bRet) IO_SetY(yRAL_AirBlower,false);
                  }
                  else {
                      SetTemp(1,OM.DevOptn.iTempSet1);
                      if( IO_GetY(yRAL_AirBlower)) IO_SetY(yRAL_AirBlower,false);

                      bStopTemp = false ;
                  }

                  iStep++;
                  return ;

        case  14: if(tmStopDelay.OnDelay(SEQ._iSeqStat != ssRunning && SEQ._iSeqStat != ssRunWarn && IO_GetX(xRAL_PkgInChk), OM.DevOptn.iStopWaitSec * 1000)) {
                      SetTemp(2,OM.DevOptn.iStopTemp);
                  }
                  else {
                      SetTemp(2,OM.DevOptn.iTempSet2);
                  }
                  iStep=10 ;
                  return ;
    }
}


void CHeater::Add_Chksum(char *pTxData,int iCnt)
{
	int i;
	int iChksum=0;
	int iHex1,iHex2;
	char sHex[3]={0,};

	for(i=1;i<=iCnt;i++)
	{
		iChksum = iChksum + pTxData[i];
	}
	iChksum = iChksum % 256;
	iHex1=iChksum/16;
	iHex2=iChksum%16;
	sprintf(sHex,"%X%X",iHex1,iHex2);
	pTxData[iCnt]=sHex[0];
	pTxData[iCnt+1]=sHex[1];
}
