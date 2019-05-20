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
    m_sErrName[  0] = "에러없음                           ";
    m_sErrName[  1] = "레이저 반사파 에러                 ";
    m_sErrName[  2] = "레이저 고온 에러                   ";
    m_sErrName[  3] = "레이저 과전압 에러                 ";
    m_sErrName[  4] = "헤드 통신 에러                     ";
    m_sErrName[  5] = "스캐너 에러                        ";
    m_sErrName[  6] = "셔터 에러                          ";
    m_sErrName[  7] = "헤드 미접속 에러                   ";
    m_sErrName[  8] = "헤드 형식 에러                     ";
    m_sErrName[  9] = "컨트롤러 FPGA 버전 에러            ";
    m_sErrName[ 10] = "헤드 FPGA 버전 에러                ";
    m_sErrName[ 11] = "인쇄 블록 없음 에러                ";
    m_sErrName[ 12] = "내장 메모리 카드 인식 불능 에러    ";
    m_sErrName[ 13] = "헤드 데이터 에러                   ";
    m_sErrName[ 14] = "전개 메모리 풀 에러                ";
    m_sErrName[ 15] = "인쇄 메모리 풀 에러                ";
    m_sErrName[ 16] = "미설정 에러                        ";
    m_sErrName[ 17] = "최적화 미실행 에러                 ";
    m_sErrName[ 18] = "폰트 파일 없음 에러                ";
    m_sErrName[ 19] = "인코더 인쇄 속도 오버 에러         ";
    m_sErrName[ 20] = "인쇄 트리거 에러                   ";
    m_sErrName[ 21] = "전개 메모리 풀 에러 2              ";
    m_sErrName[ 22] = "센서 아웃 에러                     ";
    m_sErrName[ 23] = "영역 오버 에러                     ";
    m_sErrName[ 24] = "이동 인쇄 영역 오버 에러           ";
    m_sErrName[ 26] = "로고 파일 에러                     ";
    m_sErrName[ 27] = "외국 문자 파일 에러                ";
    m_sErrName[ 28] = "인코드 불능 에러                   ";
    m_sErrName[ 29] = "설정 전환 불능 에러                ";
    m_sErrName[ 30] = "스캐너 에러 2                      ";
    m_sErrName[ 31] = "기한 설정 에러                     ";
    m_sErrName[ 32] = "재기동 에러                        ";
    m_sErrName[ 33] = "로고 . 외국 문자 크기 확대 에러    ";
    m_sErrName[ 34] = "교점 제거 에러                     ";
    m_sErrName[ 35] = "인코드 불능 에러                   ";
    m_sErrName[ 36] = "고속 문자 변경 전개 에러           ";
    m_sErrName[ 38] = "가공기 타원 에러                   ";
    m_sErrName[ 39] = "로고 외국 문자 버퍼 풀 에러        ";
    m_sErrName[ 41] = "링크 블록 에러                     ";
    m_sErrName[ 42] = "3D 배치 부정 에러                  ";
    m_sErrName[ 43] = "인쇄 누락 검출 에러                ";
    m_sErrName[ 44] = "오발광 검출 에러                   ";
    m_sErrName[ 45] = "Z 영역 오버 에러                   ";
    m_sErrName[ 46] = "바코드 미 등록 에러                ";
    m_sErrName[ 47] = "웜업 설정 에러                     ";
    m_sErrName[ 48] = "3D 블록 크기 에러                  ";
    m_sErrName[ 49] = "Z-MAP 파일 에러                    ";
    m_sErrName[ 50] = "폰트 없음 에러                     ";
    m_sErrName[ 51] = "인쇄 데이터 생성 에러              ";
    m_sErrName[ 52] = "시스템 에러 2                      ";
    m_sErrName[ 53] = "시스템 에러 3                      ";
    m_sErrName[ 54] = "시스템 에러 4                      ";
    m_sErrName[ 55] = "시스템 에러 5                      ";
    m_sErrName[ 56] = "시스템 에러 6                      ";
    m_sErrName[ 57] = "시스템 에러 7                      ";
    m_sErrName[ 58] = "시스템 에러 8                      ";
    m_sErrName[ 59] = "시스템 에러 9                      ";
    m_sErrName[ 60] = "시스템 에러 10                     ";
    m_sErrName[ 61] = "시스템 에러 11                     ";
    m_sErrName[ 62] = "시스템 에러 12                     ";
    m_sErrName[ 63] = "시스템 에러 13                     ";
    m_sErrName[ 64] = "시스템 에러 14                     ";
    m_sErrName[ 65] = "시스템 에러 15                     ";
    m_sErrName[ 66] = "시스템 에러 16                     ";
    m_sErrName[ 67] = "시스템 에러 17                     ";
    m_sErrName[ 68] = "시스템 에러 18                     ";
    m_sErrName[ 69] = "시스템 에러 19                     ";
    m_sErrName[ 70] = "시스템 에러 20                     ";
    m_sErrName[ 91] = "내부 시계 미설정 에러              ";
    m_sErrName[101] = "LD 고온 에러                       ";
    m_sErrName[102] = "LD 저온 에러                       ";
    m_sErrName[103] = "발진기 / 헤드 고온 에러            ";
    m_sErrName[104] = "발진기 / 헤드 저온 에러            ";
    m_sErrName[105] = "Q 스위치 정지 에러                 ";
    m_sErrName[107] = "Q 스위치 제어 에러                 ";
    m_sErrName[108] = "Q 스위치 동작 체크 에러            ";
    m_sErrName[111] = "레이저 파워 오토 캘리브레이션 에러 ";
    m_sErrName[121] = "발진기 고온 에러 2                 ";
    m_sErrName[122] = "발진기 저온 에러 2                 ";
    m_sErrName[123] = "온도 제어 미접속 에러              ";
    m_sErrName[131] = "시스템 에러 21                     ";
    m_sErrName[132] = "시스템 에러 22                     ";
    m_sErrName[133] = "시스템 에러 23                     ";
    m_sErrName[134] = "시스템 에러 24                     ";
    m_sErrName[135] = "시스템 에러 25                     ";
    m_sErrName[136] = "시스템 에러 26                     ";
    m_sErrName[137] = "시스템 에러 27                     ";
    m_sErrName[138] = "시스템 에러 28                     ";
    m_sErrName[139] = "시스템 에러 29                     ";
    m_sErrName[140] = "시스템 에러 30                     ";
    m_sErrName[141] = "시스템 에러 31                     ";
    m_sErrName[142] = "시스템 에러 32                     ";
    m_sErrName[143] = "시스템 에러 33                     ";
    m_sErrName[144] = "시스템 에러 34                     ";
    m_sErrName[205] = "헤드 제어 케이블 미접속 에러       ";
    m_sErrName[301] = "메모리 체크 에러 1                 ";
    m_sErrName[302] = "메모리 체크 에러 2                 ";
    m_sErrName[303] = "메모리 체크 에러 3                 ";
    m_sErrName[304] = "메모리 체크 에러 4                 ";
    m_sErrName[305] = "메모리 체크 에러 5                 ";
    m_sErrName[306] = "메모리 체크 에러 6                 ";
    m_sErrName[307] = "메모리 체크 에러 7                 ";
    m_sErrName[308] = "메모리 체크 에러 8                 ";
    m_sErrName[309] = "메모리 체크 에러 9                 ";
    m_sErrName[310] = "메모리 체크 에러 10                ";
    m_sErrName[311] = "메모리 체크 에러 11                ";
    m_sErrName[312] = "메모리 체크 에러 12                ";
    m_sErrName[313] = "메모리 체크 에러 13                ";
    m_sErrName[314] = "메모리 체크 에러 14                ";
    m_sErrName[315] = "메모리 체크 에러 15                ";
    m_sErrName[316] = "메모리 체크 에러 16                ";
    m_sErrName[317] = "메모리 체크 에러 17                ";
    m_sErrName[318] = "메모리 체크 에러 18                ";
    m_sErrName[319] = "메모리 체크 에러 19                ";
    m_sErrName[320] = "메모리 체크 에러 20                ";
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

    //상시 data 전송.
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


        //에러코드 받아오자.
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
                      m_iLaserStep = 100; // GA명령은 응답에 에러 코드가 오는 경우가 있다.
                      return false;
                  }
                  //Rs232_L -> sData = "";
                  //sSendMsg = "RE";
                  //SendMsg(sSendMsg);
                  m_iLaserStep=0;
                  return true;


        //에러코드 받아오자.
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
                          m_iLaserStep = 100; // GA명령은 응답에 에러 코드가 오는 경우가 있다.
                          return false;
                      }
                      else {
                          m_iLaserStep = 11 ;
                          return false ;
                      }
                  }
                  m_iLaserStep=0;
                  return true;


        //에러코드 받아오자.
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
    char *cChekSum;                   //CheckSum 데이터를 Char형으로 변환하기 위해서.
    int iLength = _sData.Length();    //길이를 위하여.
    int iXORSum;                      //sXOR1, sXOR2 데이터를 합하기 위해서.
    String sChekQue[20];              //cChekSum의 값을 이진수 화.
    String sXOR1 , sXOR2;             //이진수화 한 값을 XOR 하기 위한 변수.
    String sPreData = "00000000";     //XOR한 값의 빽업용.
    String sChekSum , sResult;        //sChekSum -> XOR한 마지막 값 2진수로 되어 있음. sResult -> sCheckSum을 16진수로 나타낸 결과 값.
    String sChekData;                 //cChekSum값을 16진수로 표현한 값.

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
int CLaserConUnit::GetErrCode()/*이거 쓸려면 고쳐야 함.*/
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
        sSendMsg = "EX"; // 콤마 뺀 상태 기존에 있었음..
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

