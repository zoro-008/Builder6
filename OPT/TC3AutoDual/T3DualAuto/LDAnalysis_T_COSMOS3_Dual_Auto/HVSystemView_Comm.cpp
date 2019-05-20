#include "stdafx.h"
#include "HVSystem.h"
#include "HVSystemDoc.h"
#include "HVSystemView.h"
#include "mainfrm.h"

int CHVSystemView::ByteToChar(BYTE *pIn,char *pOut,int num)
{
	int i;
	int k=0;
	for(i=0; i<num; i++)
	{
        if(pIn[i]==0x02 || pIn[i]==0x03) continue;

		pOut[k]=(char)pIn[i];
		k++;
	}
	return k;
}

int CHVSystemView::ReadBarcode()
{
	if(m_cSettingDataEx2.iBPort==0) 
	{ 
		m_wCtrlWindow.m_strBarcode="NONE";
  	    m_strBCode=m_wCtrlWindow.m_strBarcode;
		m_wCtrlWindow.m_pwEdBarcode->SetWindowText(m_wCtrlWindow.m_strBarcode);
		m_iBarcodeReadingFlag=1;
		return 1; 
	}

	if(m_wCtrlWindow.m_strBarcode.GetLength() > 3) // 3 =>minimum string size
	{
		m_iBarcodeReadingFlag=1;
  	    m_strBCode=m_wCtrlWindow.m_strBarcode;

		m_wCtrlWindow.m_strBarcode="";
		m_wCtrlWindow.m_pwEdBarcode->SetWindowText(m_wCtrlWindow.m_strBarcode);

		return 1;
	}
	else m_iBarcodeReadingFlag=0;
 
	return 0;
}

LRESULT CHVSystemView::OnCommMessage(WPARAM wParam,LPARAM lParam)
{
	BYTE Receive[BUFF_SIZE];
 	char cTmp[BUFF_SIZE];
	int iSize=0;

    iSize=m_Comm.GetString(Receive);

	if(iSize <3) return 1;

	iSize=ByteToChar(Receive,cTmp,iSize);
	cTmp[iSize]='\0';
	


	m_wCtrlWindow.m_strBarcode.Format("%s",cTmp);
	m_strBCode=m_wCtrlWindow.m_strBarcode;
	m_wCtrlWindow.m_pwEdBarcode->SetWindowText(m_wCtrlWindow.m_strBarcode);
	return 1;
}

void CHVSystemView::ReInitComport(int port)
{
	CString strPort[8]=
	{
		"NONE",
		"KEY",
		"COM1",
		"COM2",
		"COM3",
		"COM4",
		"COM5",
		"COM6"
	};
	if(m_iComportOpenFlag==1) // 포트가 열려 있는지 확인을 한다. 
	{
		// 포트가 열려 있으면 타이머를 종료 시키고  포트를 닫는다.
		m_Comm.Close();
		m_iComportOpenFlag=0;
	}

    if(port<0 || port >7) port=0;
	if(port<=1) return;

	m_Comm.SetOwnerWindow(this->GetSafeHwnd());

	::Sleep(200);
	if(m_Comm.Open(strPort[port],9600,port-1)!=TRUE)
	{
		m_iComportOpenFlag=0;
		AfxMessageBox("PORT Init Fail");
	}
	else 
	{
		m_iComportOpenFlag=1;
	}

	return;
}