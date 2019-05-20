//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Rs232UVCuring.h"
#include "UtilDefine.h"
#include "SLogUnit.h"
#include "UserINI.h"
#include "Timer.h"

#include <stdio.h>
//---------------------------------------------------------------------------

#pragma package(smart_init)
CRs232UV Rs232UV ;

CRs232UV::CRs232UV(void)
{
    Init();
}
//---------------------------------------------------------------------------
CRs232UV::~CRs232UV(void)
{                                                  
    Close();
}
//---------------------------------------------------------------------------
void CRs232UV::Init()
{
    m_pRS232C = new TRS232C;
    m_sReadMsg = "" ;

    if(!m_pRS232C -> Open(rsUV)) ShowMessage("UV Curing Rs232 Port Open Fail!");
    else                         m_pRS232C -> CallBackReg(ReceiveUVMsg        );

    LoadSave(true);
}
//---------------------------------------------------------------------------
void CRs232UV::Close()
{
    m_pRS232C -> Close() ;

    delete  m_pRS232C;

    LoadSave(false);
}
//---------------------------------------------------------------------------
void CRs232UV::SetCuring(EN_UV_CH _Ch, int _iLight)
{
    // UART(RS232) 통신사양
    // * 보오레이트 : 9600bps
    // * DATA 크기 : 8Bit(아스키값)
    // * STOP Bit : 1Bit
    // * 패리티 : 없슴
    // * 모든 포맷은 아스키 포맷임(6 바이트)

    // LED 컨트롤러 채널 DATA 변경
    //  * C  : command
    //  * n1 : 1~9, A~G(10~16) 채널번호
    //  * n2 : 출력 DATA 상위 NIBBLE(0~F)
    //  * n3 : 출력 DATA 하위 NIBBLE(0~F)
    //  예) CH-1에 DATA값 8 입력 시 => C108
    //      CH-7에 DATA값 255 입력 시 => C7FF
    //  예) RETURN => R108, R7FF


    m_iCmdNo = ucUVCuring ;      // 10 보내면 16 수신

    String sMsg = "";
    String sMsgCmd = "C"; // sMsgCmd = 메세지 커맨드
    String sHexData = "";

    int iCH = _Ch;
    int iLIGHT = _iLight;

    if(iLIGHT > 255){
        iLIGHT = 255 ;
    }
               // 10 넣으면 "0A" 들어옴
    sHexData = IntToHex(iLIGHT, 2); // 10진수를 16진수로 변환해서 메세지 송신

    m_sCuringData = "";
    m_sCuringData = sMsg.sprintf("%s%d%02s", sMsgCmd, iCH, sHexData);

    sMsg += (char)0x0D; // /r
    sMsg += (char)0x0A; // /n

    m_pRS232C -> sData = "";
    m_pRS232C -> SendData(sMsg.Length() , sMsg.c_str());
}
//---------------------------------------------------------------------------
void CRs232UV::SetCuringOnOff(EN_UV_CH _Ch, bool _bOnOff)
{
    // 채널 ON/ OFF
    // * H  : Command.
    // * n1 : 1~9, A~G(10~16) 채널번호
    // * n2,n3 : "ON", "OF"
    // 예) CH-1 ON시 => H1ON
    //     CH-2 OFF시 => H2OF
    // 예) RETURN => R1ON, R2OF


    m_iCmdNo = ucUVOnOff ;

    String sMsg       = ""  ;
    String sMsgCmd    = "H" ;
    String sCuringOn  = "ON";  // On  : "ON" -> UV 경화기 On/Off 하기 위한 송신 메세지 코드
    String sCuringOff = "OF";  // Off : "OF"

    int iCH = _Ch;
    m_sCuringOnOffData = "";

    if(_bOnOff) m_sCuringOnOffData = sMsg.sprintf("%s%d%s", sMsgCmd, iCH, sCuringOn ); // UV 경화기 채널 On
    else        m_sCuringOnOffData = sMsg.sprintf("%s%d%s", sMsgCmd, iCH, sCuringOff); // UV 경화기 채널 Off

    sMsg += (char)0x0D ; // /r
    sMsg += (char)0x0A ; // /n

    if(iCH == 1 ||
       iCH == 2 ||
       iCH == 3 ||
       iCH == 4 ) m_bBtmUvOnOff = _bOnOff;
    if(iCH == 5 ) m_bTopUvOnOff = _bOnOff;

    m_pRS232C -> sData = "";
    m_pRS232C -> SendData(sMsg.Length() , sMsg.c_str());
}
//---------------------------------------------------------------------------
void CRs232UV::SendCuringData(EN_UV_CH _Ch)
{
    // LED 컨트롤러 상태 확인
    // * S : Command
    // * n1: 1~9, A~G(10~16) 채널번호
    // * n2,n3 : "00"
    // 예) CH-1  현재 상태 확인 시 => S100
    //     CH-10 현재 상태 확인 시 => SA00
    // 예) RETURN => R100, RA00

    m_iCmdNo = ucUVData ;

    int iCH = _Ch;

    String sMsg     = ""  ;
    String sMsgCmd  = "S" ;
    String sMsgTemp = "00";

    m_sSendCuringData = sMsg.sprintf("%s%d%s", sMsgCmd, _Ch, sMsgTemp);

    sMsg += (char)0x0D ; // /r
    sMsg += (char)0x0A ; // /n

    m_pRS232C -> sData = "";
    m_pRS232C -> SendData(sMsg.Length() , sMsg.c_str());
}
//---------------------------------------------------------------------------
void CRs232UV::ReceiveUVMsg(DWORD _cbInQue)
{
    if(_cbInQue == 0) return ;

    BYTE RcvBuff[256];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));

    m_pRS232C -> ReadData(_cbInQue, RcvBuff); // 콜백으로 받은 문자 크기 담기
    m_sReadMsg += (char*)RcvBuff ; // 캐릭터형 포인터로 강제 형변환

    Trace("UV Curing RcvBuff", m_sReadMsg.c_str());
    if(m_sReadMsg == "") return ; // ReadData에서 받아온 _cbInQue값이 없으면 return
    if(!m_sReadMsg.Pos((char)0x0D)) return ; // CR = 0x0D (현재 통신이 완료될 때, 확인하는 메세지?)

    m_pRS232C -> sData = m_sReadMsg ;
    m_sRcvMsg          = m_sReadMsg ; // 마스터창에서 현재 메세지 상태 확인하는 부분

    m_sErrMsg = "";
    m_bErrFlg = false;
    String sMsgCmd = "R"; // 컨트롤러에서 수신하는 메시지 첫 번째 문자를 비교하기 위해 사용


    if(m_iCmdNo == ucUVCuring){ // 1번 UV 채널과 광량 송/수신 비교하는 곳
        String sMsgRst1 ="";
        sMsgRst1 = m_sCuringData; // sMsgRst1 = 메세지 결과
        sMsgRst1.Delete(1, 1);
        sMsgRst1.sprintf("%s%s", sMsgCmd, sMsgRst1);

        if(sMsgRst1 != m_sRcvMsg){
            Trace("SetCuring", m_sReadMsg.c_str());
            m_sErrMsg = "SetCuring Error";
            m_bErrFlg = true;
        }
    }

    if(m_iCmdNo == ucUVOnOff){ // 2번 UV On/Off 송/수신 비교하는 곳
        String sMsgRst2 = "";
        sMsgRst2 = m_sCuringOnOffData;
        sMsgRst2.Delete(1, 1);
        sMsgRst2.sprintf("%s%s", sMsgCmd, sMsgRst2);

        if(sMsgRst2 != m_sRcvMsg){
            Trace("SetCuringOnOff", m_sReadMsg.c_str());
            m_sErrMsg = "SetCuringOnOff Error";
            m_bErrFlg = true;
        }
    }

    if(m_iCmdNo == ucUVData){ // 3번 UV 설정한 채널과 광량 데이터 받아 비교하는 곳
        String sMsgRst3 = "";
        String sMsgCuring = m_sSendCuringData;
        sMsgCuring.Delete(1, 1);
        sMsgRst3.sprintf("%s%s", sMsgCmd, sMsgCuring);
        //sMsgRst3 = m_sSendCuringData;  // S400
        //sMsgRst3.Delete(1, 1); // 400
        //sTemp11 = sMsgRst3.sprintf("%s%s", sMsgCmd, sMsgRst3);
        //sTemp11.sprintf("%s%s", sMsgCmd, sMsgRst3);
        sMsgRst3 += "\r\n";

        if(sMsgRst3 != m_sRcvMsg){
            Trace("SendCuringData", m_sReadMsg.c_str());
            m_sErrMsg = "SendCuringData Error";
            m_bErrFlg = true;
        }
        else if(sMsgRst3 == m_sRcvMsg){ // UV 광량 데이터 받아 리턴하는 곳
            Trace("SendCuringData Success", m_sReadMsg.c_str());
            m_sErrMsg = "SendCuringData Success";
            m_bErrFlg = false;
        }
    }

    SetData(m_sRcvMsg);

    String sAlramOn  = "RER1";
    String sAlramOff = "RER0";

    if(sAlramOn == m_sRcvMsg){
        Trace("sAlramOn", m_sReadMsg.c_str());
        m_sErrMsg = "Temperature 55ºOver Error";
        m_bErrFlg = true;
    }
    else if(sAlramOff == m_sRcvMsg){
        Trace("sAlramOff", m_sReadMsg.c_str());
        m_sErrMsg = "Temperature Alram Off";
        m_bErrFlg = false;
    }

    m_iCmdNo = ucNone ;
    m_sReadMsg = "";
}
//---------------------------------------------------------------------------
String CRs232UV::GetMsg()
{
    return m_sRcvMsg ;
}
//---------------------------------------------------------------------------
bool CRs232UV::GetMsgEnd()
{
    return m_iCmdNo == ucNone ;
}
//---------------------------------------------------------------------------

String CRs232UV::GetErrMsg()
{
    return m_sErrMsg ;
}
//---------------------------------------------------------------------------
bool CRs232UV::MsgReset()
{
    m_sErrMsg        = ""; // UV 쪽에서 받아온 에러 메시지 초기화
//    m_sReadMsg       = ""; // ReceiveUVMsg 함수에서 받은 모든 메시지 초기화

    return m_bErrFlg = false;
}
//---------------------------------------------------------------------------
bool CRs232UV::GetErrFlg()
{
    return m_bErrFlg ;
}
//---------------------------------------------------------------------------
String CRs232UV::GetData()
{
    return m_sData;
}
//---------------------------------------------------------------------------

void CRs232UV::SetData(String _sMsg)
{
    m_sData = _sMsg;
}
//---------------------------------------------------------------------------


void CRs232UV::Update()
{
    static double dPreTime = GetTime() ;
    double dCrntTime = GetTime();
    double dGapTime = dCrntTime - dPreTime ;
    dPreTime = dCrntTime ;
    if(m_bTopUvOnOff) m_dTopUvUseTime_ms += dGapTime ;
    if(m_bBtmUvOnOff) m_dBtmUvUseTime_ms += dGapTime ;




}

double CRs232UV::GetTopUvUseTime_ms()
{
    return m_dTopUvUseTime_ms ;
}

double CRs232UV::GetBtmUvUseTime_ms()
{
    return m_dBtmUvUseTime_ms ;

}

void CRs232UV::ClearTopUvUseTime()
{
    m_dTopUvUseTime_ms = 0.0 ;
}

void CRs232UV::ClearBtmUvUseTime()
{
    m_dBtmUvUseTime_ms = 0.0 ;
}

void CRs232UV::LoadSave(bool _bLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "Util\\Rs232Uv.INI"   ;
    //if(!FileExists(sPath))

    if(_bLoad){
        //Load Device.
        UserINI.Load(sPath.c_str() , "Member" , "m_dTopUvUseTime_ms" , m_dTopUvUseTime_ms);
        UserINI.Load(sPath.c_str() , "Member" , "m_dBtmUvUseTime_ms" , m_dBtmUvUseTime_ms);
    }

    else{
        //Save Device.
        UserINI.Save(sPath.c_str() , "Member" , "m_dTopUvUseTime_ms" , m_dTopUvUseTime_ms);
        UserINI.Save(sPath.c_str() , "Member" , "m_dBtmUvUseTime_ms" , m_dBtmUvUseTime_ms);
    }

}
