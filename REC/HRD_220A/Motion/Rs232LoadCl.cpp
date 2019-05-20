//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Rs232LoadCl.h"
#include "UtilDefine.h"
#include "SLogUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//CRs232LoadCl Rs232LoadCl ;

CRs232LoadCl::CRs232LoadCl(void)
{
    Init();
}
CRs232LoadCl::~CRs232LoadCl(void)
{
    Close();
}

void CRs232LoadCl::Close()
{
    m_pRS232C -> Close() ;
    delete m_pRS232C;
}


void CRs232LoadCl::Init()
{
    m_iCmdNo = lcNone;
    m_pRS232C = new TRS232C;

    //ShowMessage("RS-232C Load Cell Open Failed!");
    if(!m_pRS232C->Open(rsLoadCell)) ShowMessage("RS-232C Load Cell Open Failed!");
     else                             m_pRS232C->CallBackReg(RcvLoadClData);
}

bool CRs232LoadCl::GetMsgEnd()
{
    return m_iCmdNo == lcNone ;

}

bool CRs232LoadCl::WeightCheck()
{
    String sSendMsg, sMsg ;

    //sSendMsg = "0R" ; //Chanel
    //m_pRS232C -> SendData(sSendMsg.Length() , sSendMsg.c_str());
    sSendMsg = "0" ; //Chanel
    m_pRS232C -> SendData(sSendMsg.Length() , sSendMsg.c_str());
    sSendMsg = "R" ;
    m_pRS232C -> SendData(sSendMsg.Length() , sSendMsg.c_str());

    m_iCmdNo = lcSetCheck ;

    return true ;
}
bool CRs232LoadCl::SetHold(bool _bOnOff)
{
    m_iCmdNo = lcSetHold ;
    String sSendMsg ;

    //sSendMsg = "0" ; //Chanel
    //m_pRS232C -> SendData(sSendMsg.Length() , sSendMsg.c_str());
    sSendMsg = _bOnOff ? "0H" : "0L" ;
    m_pRS232C -> SendData(sSendMsg.Length() , sSendMsg.c_str());

    return true ;
}

void CRs232LoadCl::RcvLoadClData (DWORD _cbInQue)
{
    int iLen;
    BYTE RcvBuff[300];
    AnsiString sRcvMsg = "" ;
    AnsiString sRcvMsgTest = "";
    memset(&RcvBuff, 0 , sizeof(RcvBuff));

    iLen = m_pRS232C->ReadData(_cbInQue, RcvBuff);

    sRcvMsg = (char*)RcvBuff ;
    m_sReadMsg+=sRcvMsg ;
    Trace("LoadCell",sRcvMsg.c_str());

    if(m_sReadMsg.Length() > 1024) {
        Trace("LoadCellData OverFlow", String(m_sReadMsg).c_str());
        m_sReadMsg = "" ;
    }

    if(!m_sReadMsg.Pos((char)0x02)) return ; //STX
    if(!m_sReadMsg.Pos((char)0x03)) return ; //ETX

    m_sRcvMsg = m_sReadMsg ;

    //©���� STX�� ETX���� �������.
    if(m_sReadMsg.Pos((char)0x03) < m_sReadMsg.Pos((char)0x02)) {
        m_sReadMsg.Delete(1,m_sReadMsg.Pos((char)0x02)-1);
        return ; //ETX
    }

    //����� ���� ���ÿ� ��� ������ ���ͼ� ����...
    m_pRS232C->sData = m_sReadMsg ;
    m_sReadMsg       = m_sReadMsg.Delete(1, 2);                //ó�� STX�� Indicator ID����
    m_sReadMsg       = m_sReadMsg.Delete(8, m_sReadMsg.Length());//������ ���� ������ ��, �ٸ� ������ ����.
    m_sReadMsg       = m_sReadMsg.Delete(2,1) ; //- 0.003 �̷� ������ �����̲��־ ��.

        //���⿡ ������ Ȯ�� �ؼ� �ִ´�.
    if(StrToFloatDef(m_sReadMsg , 0) == StrToFloatDef(m_sReadMsg , 1)) {
        m_dLoadCellData = StrToFloatDef(m_sReadMsg , m_dLoadCellData);
    }
    else {
        Trace("LoadCellData Err", String(m_sReadMsg).c_str());
    }
    m_sReadMsg = "";

    m_iCmdNo = lcNone ;
}

double CRs232LoadCl::GetLoadCellData()
{
    return m_dLoadCellData ;
}

String CRs232LoadCl::GetMsg()
{
    return m_sRcvMsg ;

}


bool CRs232LoadCl::Update(void)
{
/*  ��Ƽ��� ��� ��� �Ͽ� ��� ���� �����Ͽ� Rcv�ȴ�.

    AnsiString sSendMsg = "";
    AnsiString sPrcsMsg = "";

    m_iPreSeqCycle = m_iSeqCycle;

    switch (m_iSeqCycle) {
        default: if(m_iSeqCycle) {
                      ShowMessage("Rs232LoadCell Error"); //2015 JS
                 }
                 m_iSeqCycle = 0 ;
                 return true ;

        case 10: sSendMsg = m_iLoadClCh;
                 m_pRS232C -> SendData(sSendMsg.Length() , sSendMsg.c_str());
                 m_iSeqCycle++;
                 return false;

        case 11: sSendMsg = sLoadClMsg;
                 m_pRS232C -> SendData(sSendMsg.Length() , sSendMsg.c_str());
                 m_iSeqCycle++;
                 return false;

        case 12: m_iSeqCycle = 0;
                 m_bLoadClCmd = false;
                 return true;
    }
*/
}

