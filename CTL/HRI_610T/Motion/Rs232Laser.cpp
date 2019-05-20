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

    if(!m_sReadMsg.Pos((char)0x03))return ;
    //if(!m_sReadMsg.Pos("@"))return ;
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
void CRs232Laser::SendMsg(EN_EO_CMD _Cmd , AnsiString _sMsg)
{
    AnsiString sSendMsg = "";
    AnsiString sTemp , sLength;
    AnsiString sCmd;

    if(_Cmd == eoDAT) sCmd = "DAT";
    if(_Cmd == eoVAD) sCmd = "VAD";
    if(_Cmd == eoCHM) sCmd = "CHM";
    if(_Cmd == eoRDY) sCmd = "RDY";

    sTemp = sTemp.sprintf("%3d", _sMsg.Length());

    if(sTemp.SubString(0,1) == " " || sTemp.SubString(0,1) == "") sLength  = "0";
    else                                                          sLength  = sTemp.SubString(0,1);
    if(sTemp.SubString(1,1) == " " || sTemp.SubString(1,1) == "") sLength += "0";
    else                                                          sLength += sTemp.SubString(1,1);
    if(sTemp.SubString(2,1) == " " || sTemp.SubString(2,1) == "") sLength += "0";
    else                                                          sLength += sTemp.SubString(2,1);
    if(sTemp.SubString(3,1) == " " || sTemp.SubString(3,1) == "") sLength += "0";
    else                                                          sLength += sTemp.SubString(3,1);


    sSendMsg  = (char)0x02;
    sSendMsg += sCmd;
    sSendMsg += sLength;
    sSendMsg += _sMsg;
    sSendMsg += (char)0x03;

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

