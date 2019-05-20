// OkNgWindow.cpp : implementation file
//

#include "stdafx.h"
#include "hvsystem.h"
#include "OkNgWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COkNgWindow dialog


COkNgWindow::COkNgWindow(CWnd* pParent /*=NULL*/)
	: CDialog(COkNgWindow::IDD, pParent)
{
	//{{AFX_DATA_INIT(COkNgWindow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COkNgWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COkNgWindow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COkNgWindow, CDialog)
	//{{AFX_MSG_MAP(COkNgWindow)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_PEAK, OnCheckPeak)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COkNgWindow message handlers

BOOL COkNgWindow::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN) {
		switch(pMsg->wParam) {
			case VK_RETURN:
			case VK_ESCAPE: return true;
			default: 
				 break; 
		}
	}		
	return CDialog::PreTranslateMessage(pMsg);
}

void COkNgWindow::OnDestroy() 
{
	CDialog::OnDestroy();
	delete[] m_pButton;
	delete pBmpRgn;		
	// TODO: Add your message handler code here
	
}

BOOL COkNgWindow::OnInitDialog() 
{
	CDialog::OnInitDialog();
	pBmpRgn=new CBmpRgn();
	pBmpRgn->SetParentWindow(this);
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\okngwindow.bmp", 0x00ff00ff);
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
	
	// TODO: Add extra initialization here
	int i;
	CString nStr;
	UINT nIdC[7]={IDC_CHECK_ANGLE,IDC_CHECK_LOCATION,IDC_CHECK_PARALLELISM,IDC_CHECK_RIM,IDC_CHECK_SIZE,IDC_CHECK_TILT,IDC_CHECK_PEAK};
	UINT nIdB[7][9]=
	{
		{IDB_ANGN,IDB_AOKN,0,0,0,IDB_OKNG_MASK,0,0,0},
		{IDB_LNGN,IDB_LOKN,0,0,0,IDB_OKNG_MASK,0,0,0},
		{IDB_PNG,IDB_POK,0,0,0,IDB_OKNG_MASK,0,0,0},
		{IDB_RNG,IDB_ROK,0,0,0,IDB_OKNG_MASK,0,0,0},
		{IDB_SNG,IDB_SOK,0,0,0,IDB_OKNG_MASK,0,0,0},
		{IDB_TNGN,IDB_TOKN,0,0,0,IDB_OKNG_MASK,0,0,0},
		{IDB_HNG,IDB_HOK,0,0,0,IDB_OKNG_MASK,0,0,0}
	};
	
	char *Str[7]=
	{
		"",
		"",
		"",
		"",
		"",
		"",
		""
	};

	m_pButton=new CxSkinButton [7];
	for(i=0;i<7; i++)
	{
		
		m_pButton[i].SubclassDlgItem(nIdC[i], this);

		nStr.Format("%s",Str[i]);
		m_pButton[i].SetToolTipText(nStr);
		m_pButton[i].SetSkin(nIdB[i][0],nIdB[i][1],nIdB[i][2],nIdB[i][3],nIdB[i][4],nIdB[i][5],nIdB[i][6],nIdB[i][7],nIdB[i][8]);
		m_pButton[i].MoveWindow(2+i*54,2,50,40);
		m_pButton[i].SetMouseDownStatus(1);

	}

//////////////////////	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COkNgWindow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);
	
	// Do not call CDialog::OnPaint() for painting messages
}


void COkNgWindow::OnCheckPeak() 
{
	// TODO: Add your control notification handler code here
	
}
