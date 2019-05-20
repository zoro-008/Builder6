//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Rs232Keyence.h"
#include "UtilDefine.h"
#include "SLogUnit.h"

#include <stdio.h>
//---------------------------------------------------------------------------

#pragma package(smart_init)
CRs232Keyence Rs232Keyence ;

CRs232Keyence::CRs232Keyence(void)
{
    Init();
}
//---------------------------------------------------------------------------
CRs232Keyence::~CRs232Keyence(void)
{
    Close();
}
//---------------------------------------------------------------------------
void CRs232Keyence::Init()
{
    m_pRS232C = new TRS232C;
    m_sReadMsg = "" ;

    if(!m_pRS232C -> Open(rsHeightSensor)) ShowMessage("Height Sensor Rs232 Port Open Fail!");
    else                                   m_pRS232C -> CallBackReg(ReceiveHgtSnrMsg);
}
//---------------------------------------------------------------------------
void CRs232Keyence::Close()
{
    m_pRS232C -> Close() ;

    delete  m_pRS232C;
}

bool CRs232Keyence::GetMsgEnd()
{
    return m_iCmdNo == hcNone ;

}
//---------------------------------------------------------------------------
void CRs232Keyence::SetZero(EN_KEC_CH _Ch, bool _bOnOff)
{
    // ex) �۽� ���� : V0, ���� ���� : V0 ���� ��&���� ���� ����

    // Auto-Zero ON        Auto-Zero OFF       RESET
    // V0 : OUT1 + OUT2    W0 : OUT1 + OUT2    VR,0 : OUT1 + OUT2
    // V1 : OUT1           W1 : OUT1           VR,1 : OUT1
    // V2 : OUT2           W2 : OUT2           VR,2 : OUT2

    m_iCmdNo = hcZero ;

    String sMsg = "";

    int iCH = _Ch;

    if(_bOnOff) sMsg = "V" + (String)iCH ; // Zero On
    else        sMsg = "W" + (String)iCH ; // Zero Off

    sMsg += (char)0x0D ;

    m_pRS232C -> sData = "" ;
    m_pRS232C -> SendData(sMsg.Length() , sMsg.c_str());
}

//---------------------------------------------------------------------------
void CRs232Keyence::CheckHeight(EN_KEC_CH _Ch)
{
    m_iCmdNo = hcHeight ;

    String sMsg = "" ;

    int iCh = _Ch;

    sMsg  = "M" + (String)iCh; //"N"���� �ϸ� ����ؼ� �����°� ���� �ѵ�... �̻���...
    sMsg += (char)0x0D ;

    m_pRS232C -> sData = "" ;
    m_pRS232C -> SendData(sMsg.Length() , sMsg.c_str());
}
//---------------------------------------------------------------------------
void CRs232Keyence::ReceiveHgtSnrMsg(DWORD _cbInQue)
{
    if(_cbInQue == 0) return ;


    BYTE RcvBuff[256];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));

    m_pRS232C -> ReadData(_cbInQue, RcvBuff); // �ݹ����� ���� ���� ũ�� ���
    m_sReadMsg += (char*)RcvBuff ; // ĳ������ �����ͷ� ���� ����ȯ

    //Trace("Keyence RcvBuff", m_sReadMsg.c_str());
    if(m_sReadMsg == "") return ; // ReadData���� �޾ƿ� _cbInQue���� ������ return
    if(!m_sReadMsg.Pos((char)0x0D)) return ; // CR = 0x0D (���� ����� �Ϸ�� ��, Ȯ���ϴ� �޼���?)

    m_pRS232C -> sData = m_sReadMsg ;

    m_sRcvMsg   = m_sReadMsg ;
    String sMsg = m_sReadMsg ;
    //���⼭ �����Ͱ� Ȯ�� �ϰ� ä�� ����.
    //M0,-12.6567,-10.2030\r
    //M1,-12.6567\r
    //M2,-10.2030\r
    if(m_iCmdNo == hcHeight){
        String sChanel = sMsg.SubString(2,1);//0
        String sData    = sMsg.Delete(1,3); //-12.6567,-10.2030\r
        String sHeight1 ;
        String sHeight2 ;
        if(sChanel == "0") { //��ä�� ���.
            sHeight1 = sData.SubString(1,sData.Pos(",")-1);
            sData.Delete(1,sData.Pos(","));
            sHeight2 = sData.SubString(1,sData.Pos("\r")-1);

            if(StrToFloatDef(sHeight1 , -1) == StrToFloatDef(sHeight1 , 0)){
                m_dHeightData1 = StrToFloatDef(sHeight1 , LASER_MAX) ;
            }
            else {
                m_dHeightData1 = LASER_MAX ;
                Trace("Keynce1 Err", String(sHeight1).c_str());
            }

            if(StrToFloatDef(sHeight2 , -1) == StrToFloatDef(sHeight2 , 0)){
                m_dHeightData2 = StrToFloatDef(sHeight2 , LASER_MAX) ;
            }
            else {
                m_dHeightData2 = LASER_MAX ;
                Trace("Keynce2 Err", String(sHeight2).c_str());
            }
        }
        else if(sChanel == "1"){ //1ä�θ�
            sHeight1 = sData.SubString(1,sData.Pos("\r")-1);
            //if(sHeight1.Pos("FFFFFFF")){
            //
            //}
            if(StrToFloatDef(sHeight1 , -1) == StrToFloatDef(sHeight1 , 0)){
                m_dHeightData1 = StrToFloatDef(sHeight1 , LASER_MAX) ;
            }
            else {
                m_dHeightData1 = LASER_MAX ;
                Trace("Keynce1 Err", String(sHeight1).c_str());
            }
        }
        else if(sChanel == "2"){ //2ä�θ�.
            sHeight2 = sData.SubString(1,sData.Pos("\r")-1);
            //if(sHeight2.Pos("FFFFFFF")){
            //    Trace("Keynce2 Err", String(sHeight2).c_str());
            //}
            //m_dHeightData2 = StrToFloatDef(sHeight2 , LASER_MAX) ;


            if(StrToFloatDef(sHeight2 , -1) == StrToFloatDef(sHeight2 , 0)){
                m_dHeightData2 = StrToFloatDef(sHeight2 , LASER_MAX) ;
            }
            else {
                m_dHeightData2 = LASER_MAX ;
                Trace("Keynce2 Err", String(sHeight2).c_str());
            }
        }
     }
    m_iCmdNo = hcNone ;
    m_sReadMsg = "" ;
}

//---------------------------------------------------------------------------
String CRs232Keyence::GetMsg()
{
    return m_sRcvMsg ;
}


double CRs232Keyence::GetHeightData(EN_KEC_CH _iCh)
{
    if(_iCh == kcChA) return m_dHeightData1 ;

    return  m_dHeightData2 ;
}



void CRs232Keyence::Update()
{
/*
    AnsiString sMsg = "" ;

    switch (m_iSeqCycle) {
        default : if(m_iSeqCycle) { ShowMessage("������ �����Դϴ�."); }
                  m_iSeqCycle = 0 ;
                  return ;

        case 10 : if(m_iCmdNo != hcNone) return ;
                  CheckHeight(kcBoth);
                  m_iSeqCycle++ ;
                  return ;

        case 11 : if(m_pRS232C -> sData == "") return ;
                  m_iSeqCycle=10 ;
                  return ;
    }
*/
}
