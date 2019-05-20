// BottomBar.cpp : implementation file
//

#include "stdafx.h"
#include "HVSystem.h"
#include "BottomBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBottomBar dialog


CBottomBar::CBottomBar(CWnd* pParent /*=NULL*/)
	: CDialog(CBottomBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBottomBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pBmpRgn=NULL;

}


void CBottomBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBottomBar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBottomBar, CDialog)
	//{{AFX_MSG_MAP(CBottomBar)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBottomBar message handlers

BOOL CBottomBar::PreTranslateMessage(MSG* pMsg) 
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
 


BOOL CBottomBar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	pBmpRgn=new CBmpRgn(this);
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\Bottombar1280.bmp", 0x00ff00ff);
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

	m_wHanraLogo=new CxSkinButton;
	m_wHanraLogo->SubclassDlgItem(IDC_BUTTON_HANRALOGO,this);
	m_wHanraLogo->SetSkin(IDB_HANRALOGO,IDB_HANRALOGO,IDB_HANRALOGO,IDB_HANRALOGO,0,IDB_HANRALOGOMASK,0,0,0);
	m_wHanraLogo->MoveWindow(390,4,32,32);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBottomBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);
}


void CBottomBar::OnDestroy() 
{
	CDialog::OnDestroy();

	delete m_wHanraLogo;
	if(pBmpRgn) delete pBmpRgn;
	pBmpRgn=NULL;
	m_dcBkGrnd=NULL;


	
}

