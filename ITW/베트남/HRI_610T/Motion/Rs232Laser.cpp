//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Rs232Laser.h"
#include "UtilDefine.h"
#include "SLogUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
CRs232Laser Rs232Laser ;

void CRs232Laser::RcvLaserData  (DWORD _cbInQue)
{
    int iLen;
    BYTE RcvBuff[300];
    AnsiString sRcvMsg = "" ;
    AnsiString sRcvMsgTest = "";
    memset(&RcvBuff, 0 , sizeof(RcvBuff));

    iLen = m_pRS232C->ReadData(_cbInQue, RcvBuff);

    sRcvMsg = (char*)RcvBuff ;

    m_sReadMsg += sRcvMsg;

    if(!m_sReadMsg.Pos("@"))return ;
}

CRs232Laser::CRs232Laser(void)
{
    Init();
}
CRs232Laser::~CRs232Laser(void)
{
    Close();
}

void CRs232Laser::Close()
{
    m_pRS232C -> Close();
    delete m_pRS232C;
}


void CRs232Laser::Init()
{
    m_pRS232C = new TRS232C;

    if(!m_pRS232C->Open(rsLaser)) ShowMessage("RS-232C Laser Open Failed!");
    else                          m_pRS232C->CallBackReg(RcvLaserData);
}

//bool CRs232Laser::SendMsg(AnsiString _sMsg)
void CRs232Laser::SendMsg(double _dX, double _dY, double _dx, double _dy, double _dT)
{
    AnsiString sSendMsg = "";
    AnsiString sTemp = "";


    sSendMsg  = "#";
    sSendMsg += sTemp.sprintf("%.3f", _dX);
    sSendMsg += ";";
    sSendMsg += sTemp.sprintf("%.3f", _dY);
    sSendMsg += ";";
    sSendMsg += sTemp.sprintf("%.3f", _dx);
    sSendMsg += ";";
    sSendMsg += sTemp.sprintf("%.3f", _dy);
    sSendMsg += ";";
    sSendMsg += sTemp.sprintf("%.5f", _dT);
    sSendMsg += "@";

    m_pRS232C -> SendData(sSendMsg.Length(), sSendMsg.c_str());
    //return sSendMsg;
}

AnsiString CRs232Laser::ReadMsg()
{
    return m_sReadMsg;
}
void CRs232Laser::ClearReadMsg()
{
    m_sReadMsg = "";
}


bool CRs232Laser::Update(void)
{

}

