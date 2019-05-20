//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "LaserConUnit.h"
#include <stdio.h>
//#include "OptionMan.h"
#include "SLogUnit.h"
#include "UtilDefine.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
CLaserConUnit LCU;

void CLaserConUnit::SetErrName ()
{
    m_sErrName[  0] = "��������                           ";
    m_sErrName[  1] = "������ �ݻ��� ����                 ";
    m_sErrName[  2] = "������ ��� ����                   ";
    m_sErrName[  3] = "������ ������ ����                 ";
    m_sErrName[  4] = "��� ��� ����                     ";
    m_sErrName[  5] = "��ĳ�� ����                        ";
    m_sErrName[  6] = "���� ����                          ";
    m_sErrName[  7] = "��� ������ ����                   ";
    m_sErrName[  8] = "��� ���� ����                     ";
    m_sErrName[  9] = "��Ʈ�ѷ� FPGA ���� ����            ";
    m_sErrName[ 10] = "��� FPGA ���� ����                ";
    m_sErrName[ 11] = "�μ� ��� ���� ����                ";
    m_sErrName[ 12] = "���� �޸� ī�� �ν� �Ҵ� ����    ";
    m_sErrName[ 13] = "��� ������ ����                   ";
    m_sErrName[ 14] = "���� �޸� Ǯ ����                ";
    m_sErrName[ 15] = "�μ� �޸� Ǯ ����                ";
    m_sErrName[ 16] = "�̼��� ����                        ";
    m_sErrName[ 17] = "����ȭ �̽��� ����                 ";
    m_sErrName[ 18] = "��Ʈ ���� ���� ����                ";
    m_sErrName[ 19] = "���ڴ� �μ� �ӵ� ���� ����         ";
    m_sErrName[ 20] = "�μ� Ʈ���� ����                   ";
    m_sErrName[ 21] = "���� �޸� Ǯ ���� 2              ";
    m_sErrName[ 22] = "���� �ƿ� ����                     ";
    m_sErrName[ 23] = "���� ���� ����                     ";
    m_sErrName[ 24] = "�̵� �μ� ���� ���� ����           ";
    m_sErrName[ 26] = "�ΰ� ���� ����                     ";
    m_sErrName[ 27] = "�ܱ� ���� ���� ����                ";
    m_sErrName[ 28] = "���ڵ� �Ҵ� ����                   ";
    m_sErrName[ 29] = "���� ��ȯ �Ҵ� ����                ";
    m_sErrName[ 30] = "��ĳ�� ���� 2                      ";
    m_sErrName[ 31] = "���� ���� ����                     ";
    m_sErrName[ 32] = "��⵿ ����                        ";
    m_sErrName[ 33] = "�ΰ� . �ܱ� ���� ũ�� Ȯ�� ����    ";
    m_sErrName[ 34] = "���� ���� ����                     ";
    m_sErrName[ 35] = "���ڵ� �Ҵ� ����                   ";
    m_sErrName[ 36] = "��� ���� ���� ���� ����           ";
    m_sErrName[ 38] = "������ Ÿ�� ����                   ";
    m_sErrName[ 39] = "�ΰ� �ܱ� ���� ���� Ǯ ����        ";
    m_sErrName[ 41] = "��ũ ��� ����                     ";
    m_sErrName[ 42] = "3D ��ġ ���� ����                  ";
    m_sErrName[ 43] = "�μ� ���� ���� ����                ";
    m_sErrName[ 44] = "���߱� ���� ����                   ";
    m_sErrName[ 45] = "Z ���� ���� ����                   ";
    m_sErrName[ 46] = "���ڵ� �� ��� ����                ";
    m_sErrName[ 47] = "���� ���� ����                     ";
    m_sErrName[ 48] = "3D ��� ũ�� ����                  ";
    m_sErrName[ 49] = "Z-MAP ���� ����                    ";
    m_sErrName[ 50] = "��Ʈ ���� ����                     ";
    m_sErrName[ 51] = "�μ� ������ ���� ����              ";
    m_sErrName[ 52] = "�ý��� ���� 2                      ";
    m_sErrName[ 53] = "�ý��� ���� 3                      ";
    m_sErrName[ 54] = "�ý��� ���� 4                      ";
    m_sErrName[ 55] = "�ý��� ���� 5                      ";
    m_sErrName[ 56] = "�ý��� ���� 6                      ";
    m_sErrName[ 57] = "�ý��� ���� 7                      ";
    m_sErrName[ 58] = "�ý��� ���� 8                      ";
    m_sErrName[ 59] = "�ý��� ���� 9                      ";
    m_sErrName[ 60] = "�ý��� ���� 10                     ";
    m_sErrName[ 61] = "�ý��� ���� 11                     ";
    m_sErrName[ 62] = "�ý��� ���� 12                     ";
    m_sErrName[ 63] = "�ý��� ���� 13                     ";
    m_sErrName[ 64] = "�ý��� ���� 14                     ";
    m_sErrName[ 65] = "�ý��� ���� 15                     ";
    m_sErrName[ 66] = "�ý��� ���� 16                     ";
    m_sErrName[ 67] = "�ý��� ���� 17                     ";
    m_sErrName[ 68] = "�ý��� ���� 18                     ";
    m_sErrName[ 69] = "�ý��� ���� 19                     ";
    m_sErrName[ 70] = "�ý��� ���� 20                     ";
    m_sErrName[ 91] = "���� �ð� �̼��� ����              ";
    m_sErrName[101] = "LD ��� ����                       ";
    m_sErrName[102] = "LD ���� ����                       ";
    m_sErrName[103] = "������ / ��� ��� ����            ";
    m_sErrName[104] = "������ / ��� ���� ����            ";
    m_sErrName[105] = "Q ����ġ ���� ����                 ";
    m_sErrName[107] = "Q ����ġ ���� ����                 ";
    m_sErrName[108] = "Q ����ġ ���� üũ ����            ";
    m_sErrName[111] = "������ �Ŀ� ���� Ķ���극�̼� ���� ";
    m_sErrName[121] = "������ ��� ���� 2                 ";
    m_sErrName[122] = "������ ���� ���� 2                 ";
    m_sErrName[123] = "�µ� ���� ������ ����              ";
    m_sErrName[131] = "�ý��� ���� 21                     ";
    m_sErrName[132] = "�ý��� ���� 22                     ";
    m_sErrName[133] = "�ý��� ���� 23                     ";
    m_sErrName[134] = "�ý��� ���� 24                     ";
    m_sErrName[135] = "�ý��� ���� 25                     ";
    m_sErrName[136] = "�ý��� ���� 26                     ";
    m_sErrName[137] = "�ý��� ���� 27                     ";
    m_sErrName[138] = "�ý��� ���� 28                     ";
    m_sErrName[139] = "�ý��� ���� 29                     ";
    m_sErrName[140] = "�ý��� ���� 30                     ";
    m_sErrName[141] = "�ý��� ���� 31                     ";
    m_sErrName[142] = "�ý��� ���� 32                     ";
    m_sErrName[143] = "�ý��� ���� 33                     ";
    m_sErrName[144] = "�ý��� ���� 34                     ";
    m_sErrName[205] = "��� ���� ���̺� ������ ����       ";
    m_sErrName[301] = "�޸� üũ ���� 1                 ";
    m_sErrName[302] = "�޸� üũ ���� 2                 ";
    m_sErrName[303] = "�޸� üũ ���� 3                 ";
    m_sErrName[304] = "�޸� üũ ���� 4                 ";
    m_sErrName[305] = "�޸� üũ ���� 5                 ";
    m_sErrName[306] = "�޸� üũ ���� 6                 ";
    m_sErrName[307] = "�޸� üũ ���� 7                 ";
    m_sErrName[308] = "�޸� üũ ���� 8                 ";
    m_sErrName[309] = "�޸� üũ ���� 9                 ";
    m_sErrName[310] = "�޸� üũ ���� 10                ";
    m_sErrName[311] = "�޸� üũ ���� 11                ";
    m_sErrName[312] = "�޸� üũ ���� 12                ";
    m_sErrName[313] = "�޸� üũ ���� 13                ";
    m_sErrName[314] = "�޸� üũ ���� 14                ";
    m_sErrName[315] = "�޸� üũ ���� 15                ";
    m_sErrName[316] = "�޸� üũ ���� 16                ";
    m_sErrName[317] = "�޸� üũ ���� 17                ";
    m_sErrName[318] = "�޸� üũ ���� 18                ";
    m_sErrName[319] = "�޸� üũ ���� 19                ";
    m_sErrName[320] = "�޸� üũ ���� 20                ";
}

String convertHex2Bin(char* hex);
int convertASCII2Number(char ascii);

String convertHex2Bin(char* hex)
{
    String result;
    for(int i = 0; hex[i]; i++)
    {
        if(hex[i] == '.'){result += ".";continue;}
        int num = convertASCII2Number(hex[i]);
        for(int bit = 3; bit >= 0; bit--)
        {
            result += num&(1<<bit)? "1" : "0";
        }
    }

    return result;
}
int convertASCII2Number(char ascii)
{
    if(isdigit(ascii)){ return ascii - '0'; }
    if(islower(ascii)){ return ascii - 'a' + 10; }
    return ascii - 'A' + 10;
}

__fastcall CLaserConUnit::CLaserConUnit(void)
{
    Init();
}

__fastcall CLaserConUnit::~CLaserConUnit(void)
{
    Close();
}
void CLaserConUnit::Init()
{
    m_iLastErrCode = leNoErr ;
    m_sRcvMsg = "";
    SetErrName();

    Rs232_L = new TRS232C ();

    if(!Rs232_L->Open(0)) ShowMessage("Laser Rs232 Port Open Fail") ;
    else                  Rs232_L->CallBackReg(ReceiveMsg);

    //m_dLaserWorkTime = 0.0 ;
}
void CLaserConUnit::ReceiveMsg(DWORD _cbInQue)
{
    //Ex : GA,0,CheckSum
    static AnsiString sRcvMsg = "" ;
    String sTemp ;
    double dTemp = 0.0 ;
    BYTE RcvBuff[300];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));
    LCU.Rs232_L -> ReadData(_cbInQue, RcvBuff);
    sRcvMsg += (char*)RcvBuff ;

    Trace("LaserCom",RcvBuff);

    //��� data ����.
    if(sRcvMsg.Pos("ST") || sRcvMsg.Pos("UT")) { sRcvMsg = "" ; return ; }

    if(!sRcvMsg.Pos("\r"  )) return ;
    //Trace("LaserCom",sRcvMsg.c_str());
    LCU.Rs232_L->sData = sRcvMsg;
    sRcvMsg  = "" ;
}

void CLaserConUnit::Close()
{
    Rs232_L -> Close();
    delete Rs232_L ;
}

bool CLaserConUnit::SetGABlockId(int _iBlockNo)
{
    m_iGABlockNo = _iBlockNo ;

}

bool CLaserConUnit::SetSendMsgId(EN_LASER_MSG_ID _iMsgId)
{
    if(_iMsgId <  0                ) return false ;
    if(_iMsgId >= MAX_LASER_MSG_ID ) return false ;
    m_iLaserMsgId = _iMsgId ;
    m_iLaserStep  = 10 ;
    return true ;

}

void CLaserConUnit::Update()
{
         if(m_iLaserMsgId == lmNone) return ;
    else if(m_iLaserMsgId == lmGA  ){if(CycleGA()){m_iLaserMsgId = lmNone; }}
    else if(m_iLaserMsgId == lmRE  ){if(CycleRE()){m_iLaserMsgId = lmNone; }}
    else if(m_iLaserMsgId == lmTX  ){if(CycleTX()){m_iLaserMsgId = lmNone; }}
    else                            {m_iLaserMsgId = lmNone;                }

}

EN_LASER_MSG_ID CLaserConUnit::GetLaserMsgId()
{
    return m_iLaserMsgId ;

}

bool CLaserConUnit::CycleTX()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    AnsiString sErrCode ;
    AnsiString sSendMsg, sReadMsg;
    int iErrCode ;
    int iChekBlockNo;

    static DWORD dStartTime ;

    if (m_tmCycle.OnDelay(m_iLaserStep && m_iLaserStep == m_iPreLaserStep , OM.MstOptn.iCycleTm )) {
        EM_SetErr(eiSTG_Laser);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : m_iLaserStep=%02d" , __FUNC__ , m_iLaserStep );
        Trace("Laser",sTemp.c_str());
        m_iLaserStep = 0;
        return true;
    }

    sTemp = sTemp.sprintf("%s m_iLaserStep=%02d" , __FUNC__ , m_iLaserStep );
    if(m_iLaserStep != m_iPreLaserStep) {
        Trace("Laser CycleTX",sTemp.c_str());
    }

    m_iPreLaserStep = m_iLaserStep;

    //Cycle.
    switch (m_iLaserStep) {

        default : if(m_iLaserStep) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s m_iLaserStep=%02d" , __FUNC__ , m_iLaserStep);
                      Trace("Laser CycleTX", sTemp.c_str());
                  }
                  m_iLaserStep = 0 ;
                  return true ;

        case  10: m_bWrkLaser = true;
                  Rs232_L -> sData = "";
                  sSendMsg = "TX";
                  SendMsg(sSendMsg);
                  m_iLaserStep++;
                  return false;

        case  11: if(Rs232_L -> sData == "") return false;
                  sReadMsg = Rs232_L -> sData;
                  if(CheckSumCheck(sReadMsg)) {
                      EM_SetErr(eiSTG_CheckSumFail);
                      m_bWrkLaser = false;
                      m_iLaserStep = 0;
                      return true;
                  }

                  if(ErrCheck(sReadMsg)) {
                      m_iLaserStep = 100;
                      return false;
                  }
                  Rs232_L -> sData = "";
                  m_bWrkLaser = false;
                  m_iLaserStep = 0;

                  return true;


        //�����ڵ� �޾ƿ���.
        case 100: if(Rs232_L -> sData == "") return false;
                  sReadMsg = Rs232_L -> sData;
                  if(CheckSumCheck(sReadMsg)) {
                      EM_SetErr(eiSTG_CheckSumFail);
                      m_bWrkLaser = false;
                      m_iLaserStep = 0;
                      return true;
                  }
                  iErrCode = sReadMsg.SubString(4,1).ToInt();
                  EM_SetErrMsg(eiSTG_LaserErrCode , sReadMsg.c_str());
                  m_bWrkLaser = false;
                  m_iLaserStep = 0 ;
                  return true ;
    }
}

bool CLaserConUnit::CycleGA()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    AnsiString sErrCode ;
    AnsiString sSendMsg, sReadMsg;
    int iErrCode ;
    int iChekBlockNo;
    if (m_tmCycle.OnDelay(m_iLaserStep && m_iLaserStep == m_iPreLaserStep , OM.MstOptn.iCycleTm )) {
        EM_SetErr(eiSTG_Laser);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : m_iLaserStep=%02d" , __FUNC__ , m_iLaserStep );
        Trace("Laser",sTemp.c_str());
        m_iLaserStep = 0;
        return true;
    }

    sTemp = sTemp.sprintf("%s m_iLaserStep=%02d" , __FUNC__ , m_iLaserStep );
    if(m_iLaserStep != m_iPreLaserStep) {
        Trace("Laser CycleGA",sTemp.c_str());
    }

    m_iPreLaserStep = m_iLaserStep;

    //Cycle.
    switch (m_iLaserStep) {

        default : if(m_iLaserStep) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s m_iLaserStep=%02d" , __FUNC__ , m_iLaserStep);
                      Trace("Laser CycleGA", sTemp.c_str());
                  }
                  m_iLaserStep = 0 ;
                  return true ;

        case  10: m_iLaserStep++;
                  return false;

        case  11:Rs232_L -> sData = "";
                  sSendMsg = "GA,";
                  sSendMsg += m_iGABlockNo;
                  SendMsg(sSendMsg);
                  m_iLaserStep++;
                  return false;

        case  12: if(Rs232_L -> sData == "") return false;
                  sReadMsg = Rs232_L -> sData;
                  if(CheckSumCheck(sReadMsg)) {
                      EM_SetErr(eiSTG_CheckSumFail);
                      m_iLaserStep = 0;
                      return true;
                  }

                  if(ErrCheck(sReadMsg)) {
                      m_iLaserStep = 100; // GA����� ���信 ���� �ڵ尡 ���� ��찡 �ִ�.
                      return false;
                  }
                  //Rs232_L -> sData = "";
                  //sSendMsg = "RE";
                  //SendMsg(sSendMsg);
                  m_iLaserStep=0;
                  return true;


        //�����ڵ� �޾ƿ���.
        case 100: if(Rs232_L -> sData == "") return false;
                  sReadMsg = Rs232_L -> sData;
                  iErrCode = sReadMsg.SubString(4,1).ToInt();
                  EM_SetErrMsg(eiSTG_LaserErrCode , sReadMsg.c_str());
                  m_iLaserStep = 0 ;
                  return true ;
    }
}

bool CLaserConUnit::CycleRE()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    AnsiString sErrCode ;
    AnsiString sSendMsg, sReadMsg;
    int iErrCode ;
    int iChekBlockNo;
    if (m_tmCycle.OnDelay(m_iLaserStep && m_iLaserStep == m_iPreLaserStep , OM.MstOptn.iCycleTm )) {
        EM_SetErr(eiSTG_Laser);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : m_iLaserStep=%02d" , __FUNC__ , m_iLaserStep );
        Trace("Laser",sTemp.c_str());
        m_iLaserStep = 0;
        return true;
    }

    sTemp = sTemp.sprintf("%s m_iLaserStep=%02d" , __FUNC__ , m_iLaserStep );
    if(m_iLaserStep != m_iPreLaserStep) {
        Trace("Laser CycleRE",sTemp.c_str());
    }

    m_iPreLaserStep = m_iLaserStep;

    static int iMsgSendCnt = 0 ;

    const int MAX_MSG_SEND_CNT = 10 ;

    //Cycle.
    switch (m_iLaserStep) {

        default : if(m_iLaserStep) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s m_iLaserStep=%02d" , __FUNC__ , m_iLaserStep);
                      Trace("Laser CycleRE", sTemp.c_str());
                  }
                  m_iLaserStep = 0 ;
                  return true ;

        case  10: iMsgSendCnt = 0 ;
                  m_iLaserStep++;

                  return false;

        case  11:Rs232_L -> sData = "";
                  sSendMsg = "RE";
                  SendMsg(sSendMsg);
                  iMsgSendCnt++;
                  m_iLaserStep++;
                  return false;

        case  12: if(Rs232_L -> sData == "") return false;
                  sReadMsg = Rs232_L -> sData;
                  if(CheckSumCheck(sReadMsg)) {
                      EM_SetErr(eiSTG_CheckSumFail);
                      m_iLaserStep = 0;
                      return true;
                  }

                  if(ErrCheck(sReadMsg)) {
                      if( iMsgSendCnt >= MAX_MSG_SEND_CNT) {
                          m_iLaserStep = 100; // GA����� ���信 ���� �ڵ尡 ���� ��찡 �ִ�.
                          return false;
                      }
                      else {
                          m_iLaserStep = 11 ;
                          return false ;
                      }
                  }
                  m_iLaserStep=0;
                  return true;


        //�����ڵ� �޾ƿ���.
        case 100: if(Rs232_L -> sData == "") return false;
                  sReadMsg = Rs232_L -> sData;
                  iErrCode = sReadMsg.SubString(4,1).ToInt();
                  EM_SetErrMsg(eiSTG_LaserErrCode , sReadMsg.c_str());
                  m_iLaserStep = 0 ;
                  return true ;
    }

}

AnsiString CLaserConUnit::GetChksum(AnsiString _sData)
{
    char *cChekSum;                   //CheckSum �����͸� Char������ ��ȯ�ϱ� ���ؼ�.
    int iLength = _sData.Length();    //���̸� ���Ͽ�.
    int iXORSum;                      //sXOR1, sXOR2 �����͸� ���ϱ� ���ؼ�.
    String sChekQue[20];              //cChekSum�� ���� ������ ȭ.
    String sXOR1 , sXOR2;             //������ȭ �� ���� XOR �ϱ� ���� ����.
    String sPreData = "00000000";     //XOR�� ���� ������.
    String sChekSum , sResult;        //sChekSum -> XOR�� ������ �� 2������ �Ǿ� ����. sResult -> sCheckSum�� 16������ ��Ÿ�� ��� ��.
    String sChekData;                 //cChekSum���� 16������ ǥ���� ��.

    cChekSum = new char[iLength];

    memset(cChekSum , 0 , iLength);

    StrCopy(cChekSum , _sData.c_str());

    for(int i = 0 ; i < iLength ; i++){
        sChekData.printf("%x" , cChekSum[i]);
        sChekQue[i] = sChekData;
        sChekQue[i].sprintf("%s", convertHex2Bin(sChekQue[i].c_str()));
    }

    for(int j = 0 ; j < iLength ; j++){
        sChekSum = "";
        for(int z = 1 ; z <= 8; z++){
            sXOR1 = sPreData   .SubString(z,1);
            sXOR2 = sChekQue[j].SubString(z,1);
            if(sXOR1 == NULL)  sXOR1 = "0";
            iXORSum = sXOR1.ToInt() + sXOR2.ToInt();
            if(iXORSum == 2) sChekSum += 0;
            else             sChekSum += iXORSum;
        }
        sPreData = sChekSum ;
    }

    sResult =  "";
    sResult =  HexToDec(sChekSum.SubString(1,4));
    sResult += HexToDec(sChekSum.SubString(5,4));

    delete [] cChekSum;

    return sResult;
}

bool CLaserConUnit::SendMsg(AnsiString _sMsg)
{
    AnsiString sSendMsg ;


    //sSendMsg  = (char)0x02;
    sSendMsg  = _sMsg.Trim() ;
    sSendMsg += "," ;
    sSendMsg += GetChksum(sSendMsg);
    //sSendMsg += COMM_CR ; //"\r";//   COMM_CR ; //(char)0x0D;
    //sSendMsg += (char)0x0A;

    char * str ;
    str = new char[256] ;
    memset(str , 0 , 256);

    int a ;
    int iMsgLeng =  sSendMsg.Length() ;
    for(a = 0 ; a < iMsgLeng ; a++) {
        str[a]=sSendMsg[a+1];
    }
    str[a]= COMM_CR ;
    iMsgLeng++;

    if(Rs232_L -> SendData(iMsgLeng , str)) { delete[] str; return true; }
    else                                      return false;



    //return Rs232_L -> SendData(iMsgLeng , str);// -> WriteData(sSendMsg.Length() , sSendMsg.c_str());
    //
    //delete [] str ;



}

char CLaserConUnit::HexToDec(String _sData)
{
    char cChr ;
    if(_sData == "0000") cChr = '0';
    else if(_sData == "0001") cChr = '1';
    else if(_sData == "0010") cChr = '2';
    else if(_sData == "0011") cChr = '3';
    else if(_sData == "0100") cChr = '4';
    else if(_sData == "0101") cChr = '5';
    else if(_sData == "0110") cChr = '6';
    else if(_sData == "0111") cChr = '7';
    else if(_sData == "1000") cChr = '8';
    else if(_sData == "1001") cChr = '9';
    else if(_sData == "1010") cChr = 'A';
    else if(_sData == "1011") cChr = 'B';
    else if(_sData == "1100") cChr = 'C';
    else if(_sData == "1101") cChr = 'D';
    else if(_sData == "1110") cChr = 'E';
    else if(_sData == "1111") cChr = 'F';

    return cChr;
}
bool CLaserConUnit::ErrCheck(String _ErrCheck)
{
    bool ErrRult;
    int iErrCheck = 0;
    iErrCheck = _ErrCheck.SubString(4,1).ToInt();
    if(iErrCheck != 0) ErrRult = true;
    else               ErrRult = false;

    return ErrRult;
}
bool CLaserConUnit::CheckSumCheck(String _sMsg)
{
    bool CheckSumRult;
    AnsiString sCheckStr, sCheckSum , sSum;
    AnsiString sTemp = _sMsg;


    sCheckStr = sTemp.SubString(1 , sTemp.Pos("\r")-3);
    sCheckSum = sTemp.SubString(sTemp.Pos("\r")-2 , 2);

    sSum = GetChksum(sCheckStr);

    if(sCheckStr == sSum) {m_iLastErrCode = leCheckSumErr; CheckSumRult = true; }
    else                                                   CheckSumRult = false;

    return CheckSumRult;
}
int CLaserConUnit::GetErrCode()/*�̰� ������ ���ľ� ��.*/
{
    AnsiString sReadMsg;
    int iErrCode;

    //if(SendErrCode()) return -1;

    sReadMsg = Rs232_L -> sData ;
    CheckSumCheck(sReadMsg);

    iErrCode = sReadMsg.SubString(4,1).ToInt();
    if(iErrCode != 0) iErrCode = sReadMsg.SubString(7,5).ToInt() + 1;
    else              iErrCode = 0;

    return iErrCode;
}
/*
bool CLaserConUnit::SendErrCode()
{
    static bool bErrCode = false;
    bool bPreErrCode ;
    AnsiString sSendMsg;

    bPreErrCode = true;
    if(bErrCode != bPreErrCode){
        bErrCode = bPreErrCode;
        sSendMsg = "EX"; // �޸� �� ���� ������ �־���..
        SendMsg(sSendMsg);
    }

    if(Rs232_L -> sData == "") return false ;
    else                       return true  ;
    bErrCode = true;
}
*/

AnsiString CLaserConUnit::GetErrName(int _iErrNo)
{
    AnsiString sErrName ;

    sErrName = m_sErrName[_iErrNo];

    return sErrName;
}

bool CLaserConUnit::GetGAFinished()
{
    return Rs232_L -> sData.Pos("GA,0");
}

int CLaserConUnit::GetBlockNoID()
{
    return m_iGABlockNo;
}

