// CounterWindow.cpp : implementation file
//

#include "stdafx.h"
#include "hvsystem.h"
#include "CounterWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCounterWindow dialog

#define DISPLAY_TIME 5001

CCounterWindow::CCounterWindow(CWnd* pParent /*=NULL*/)
	: CDialog(CCounterWindow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCounterWindow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pBmpRgn=NULL;

	int i;

	for(i=0; i<8; i++)
	{
		m_wpTitle[i]=NULL;
		m_wpData[i]=NULL;
	}
}


void CCounterWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCounterWindow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCounterWindow, CDialog)
	//{{AFX_MSG_MAP(CCounterWindow)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCounterWindow message handlers

BOOL CCounterWindow::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN) 
	{
		switch(pMsg->wParam) {
			case VK_RETURN:
			case VK_ESCAPE: return true;

			default: 
				 break; 
		}
	}		
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CCounterWindow::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	pBmpRgn=new CBmpRgn(this);
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\CntWin_256.bmp", 0x00ff00ff);
	if(rgnH)
	{
		SetWindowRgn(rgnH, TRUE);
		DeleteObject(rgnH);
	}	
	CDC *pDC = GetDC();
	m_dcBkGrnd = CreateCompatibleDC(pDC->m_hDC);
	ReleaseDC(pDC);
	// select background image
	HBITMAP hBmp = pBmpRgn->GetBitmap();
	SelectObject(m_dcBkGrnd, hBmp);
	m_nWindowWidth = pBmpRgn->GetBitmapWidth();
	m_nWindowHeight = pBmpRgn->GetBitmapHeight();
	
    int i,j;
   
	unsigned int nIDTitle[8]={
		IDC_CNT_NAME1,IDC_CNT_NAME2,IDC_CNT_NAME3,IDC_CNT_NAME4,
		IDC_CNT_NAME5,IDC_CNT_NAME6,IDC_CNT_NAME7,IDC_CNT_NAME8
	};
	unsigned int nIDData[8]={
		IDC_CNT_DATA1,IDC_CNT_DATA2,IDC_CNT_DATA3,IDC_CNT_DATA4,
		IDC_CNT_DATA5,IDC_CNT_DATA6,IDC_CNT_DATA7,IDC_CNT_DATA8
	};

    char *pTitle[8]={"검사수","양품수","불량수","불량율(%)"," 시간","처리시간","검사시간","속도(f/s)"};
	char *pData[8]={
		"0","0","0","0.000",
		"00:00:00","0","0","0"
	};
	for(j=0; j<2; j++)
		for(i=0; i<4; i++)
		{
			m_wpTitle[j*4+i]=new CStaticSkin;
			m_wpData[j*4+i]=new CStaticSkin;
			m_wpTitle[j*4+i]->SetBmpFile(".\\guiimage\\title2_256.bmp");
			m_wpTitle[j*4+i]->SubclassDlgItem(nIDTitle[j*4+i],this);
			m_wpTitle[j*4+i]->MoveWindow(95*i,52*j,95,25);
			m_wpTitle[j*4+i]->OutTextXY(8,3,pTitle[j*4+i]);
        

			m_wpData[j*4+i]->SetBmpFile(".\\guiimage\\Disp_256.bmp");
			m_wpData[j*4+i]->SubclassDlgItem(nIDData[j*4+i],this);
			m_wpData[j*4+i]->MoveWindow(95*i,25+52*j,95,25);
			m_wpData[j*4+i]->OutTextXY(8,5,pData[j*4+i]);
		}

	SetTimer(DISPLAY_TIME,1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCounterWindow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);

}

void CCounterWindow::OnDestroy() 
{
	CDialog::OnDestroy();

	KillTimer(DISPLAY_TIME);
	int i;	
	delete pBmpRgn;
    for(i=0; i<8; i++)
	{
		delete m_wpTitle[i];
		delete m_wpData[i];
	}
}
//	SYSTEMTIME nSysTime;
//	::GetLocalTime(&nSysTime);

void CCounterWindow::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	SYSTEMTIME nSysTime;
	char buf[256];

	if(nIDEvent==DISPLAY_TIME)
	{
		::GetLocalTime(&nSysTime);
		sprintf(buf,"%02d:%02d:%02d", nSysTime.wHour,nSysTime.wMinute,nSysTime.wSecond);

		m_wpData[CNT_TIME]->OutTextXY(8,5,buf);
	}
	CDialog::OnTimer(nIDEvent);
}
