// TitleBar.cpp : implementation file
//

#include "stdafx.h"
#include "HVSystem.h"
#include "MainFrm.h"
#include "HVSystemDoc.h"
#include "HVSystemView.h"
#include "TitleBar.h"
#include "External.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTitleBar dialog


CTitleBar::CTitleBar(CWnd* pParent /*=NULL*/)
	: CDialog(CTitleBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTitleBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pBmpRgn=NULL;

	m_pwButton[0]=NULL;
	m_pwButton[1]=NULL;
	m_pwButton[2]=NULL;

	m_pFont=new CFont;
	m_pFont->CreateFont(20,10, 0, 0, FW_BOLD, FALSE, FALSE,0,0,0,0,0,0, "¸íÁ¶Ã¼");



}


void CTitleBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTitleBar)
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTitleBar, CDialog)
	//{{AFX_MSG_MAP(CTitleBar)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_TITLE_BUTTON1, OnTitleButton1)
	ON_BN_CLICKED(IDC_TITLE_BUTTON2, OnTitleButton2)
	ON_BN_CLICKED(IDC_TITLE_BUTTON3, OnTitleButton3)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTitleBar message handlers

BOOL CTitleBar::PreTranslateMessage(MSG* pMsg) 
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

BOOL CTitleBar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	pBmpRgn=new CBmpRgn();

	pBmpRgn->SetParentWindow(this);

#ifdef _ADLINK_IO_
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\Titlebar1280_PCI.bmp", 0x00ff00ff);
#else
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\Titlebar1280.bmp", 0x00ff00ff);
#endif
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
	
	
	MakeGui();
	SetTimer(1, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTitleBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);
}

void CTitleBar::OnDestroy() 
{

	CDialog::OnDestroy();
    int i;
	
	for(i=0; i<4; i++)
		if(m_pwButton[i]!=NULL) delete m_pwButton[i];

	if(m_pFont!=NULL)  delete m_pFont;
	if(pBmpRgn)	delete pBmpRgn;
	pBmpRgn=NULL;	
	KillTimer(1);
}

void CTitleBar::MakeGui()
{
	int i;

	UINT uBID[3]=
	{
		IDC_TITLE_BUTTON1,
		IDC_TITLE_BUTTON2,
		IDC_TITLE_BUTTON3
	};
	CString str[3]=
	{
		"SUB-BEAM",
		"SETTING",
		"MOTION"
	};
	
	for(i=0; i<3; i++)
	{
		m_pwButton[i]=new CxSkinButton;
		m_pwButton[i]->SubclassDlgItem(uBID[i],this);
		m_pwButton[i]->SetSkin(IDB_SUBN,IDB_SUBD,0,0,0,IDB_SUBM,0,0,0);
		m_pwButton[i]->MoveWindow(2+140*i,4,140,36);
		m_pwButton[i]->SetFont(m_pFont);
		m_pwButton[i]->SetWindowText(str[i]);

	}

	m_pwButton[0]->SetCheck(1);
	m_pwButton[1]->SetCheck(0);
	m_pwButton[2]->SetCheck(0);
	m_pwButton[2]->ShowWindow(SW_HIDE);

	m_pwButton[3]=new CxSkinButton;
	m_pwButton[3]->SubclassDlgItem(IDC_TITLE_BUTTON4,this);
	m_pwButton[3]->SetSkin(IDB_TITLE_LEFT,IDB_TITLE_RIGHT,0,0,0,IDB_TITLE_M,0,0,0);
	m_pwButton[3]->MoveWindow(1220,5,54,32);
	//m_pwButton[i]->SetFont(m_pFont);
	//m_pwButton[i]->SetWindowText(str[i]);

	return;
}

void CTitleBar::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		static BOOL bOld = !g_bLeftRight;
		if(bOld != g_bLeftRight)
		{
			bOld = g_bLeftRight;
			if(g_bLeftRight)//Right
			{						
				m_pwButton[3]->SetCheck(1);				
			}	
			else//Left
			{
				m_pwButton[3]->SetCheck(0);
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}


void CTitleBar::SelectWindow(int idx)
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	if(idx==0)
	{
		m_pwButton[0]->SetCheck(1);
		m_pwButton[1]->SetCheck(0);
		m_pwButton[2]->SetCheck(0);
		m_pView->m_wOkNgWindow.ShowWindow(SW_HIDE);
		m_pView->m_wTabWindow.ShowWindow(SW_HIDE);
		m_pView->m_wVProj.ShowWindow(SW_HIDE);

		m_pView->m_wImgDisp3.MoveWindow(m_pView->m_wImgDisp3.GetOrgRect());
		m_pView->m_wImgDisp3.ShowWindow(SW_SHOW);
		m_pView->m_wImgDisp1.ShowWindow(SW_SHOW);

		m_pView->m_wMotionBase.m_pwMotion->ShowWindow(SW_HIDE);
		m_pView->m_wMotionBase.m_pwIO->ShowWindow(SW_HIDE);
	}
	else if(idx==1)
	{
		m_pwButton[0]->SetCheck(0);
		m_pwButton[1]->SetCheck(1);
		m_pwButton[2]->SetCheck(0);

		m_pView->m_wOkNgWindow.ShowWindow(SW_SHOW);
		m_pView->m_wTabWindow.ShowWindow(SW_SHOW);
		m_pView->m_wVProj.ShowWindow(SW_SHOW);
		m_pView->m_wImgDisp3.ShowWindow(SW_HIDE);
		m_pView->m_wImgDisp1.ShowWindow(SW_SHOW);

		m_pView->m_wTabWindow.SelectWindow(0);

		m_pView->m_wMotionBase.m_pwMotion->ShowWindow(SW_HIDE);
		m_pView->m_wMotionBase.m_pwIO->ShowWindow(SW_HIDE);

	}
	else if(idx==2)
	{
		m_pwButton[0]->SetCheck(0);
		m_pwButton[1]->SetCheck(0);
		m_pwButton[2]->SetCheck(1);
		m_pView->m_wOkNgWindow.ShowWindow(SW_HIDE);
		m_pView->m_wTabWindow.ShowWindow(SW_HIDE);
		m_pView->m_wVProj.ShowWindow(SW_HIDE);
		
		m_pView->m_wImgDisp3.MoveWindow(m_pView->m_wImgDisp3.GetOrgRect());
		m_pView->m_wImgDisp3.ShowWindow(SW_HIDE);
		m_pView->m_wImgDisp1.ShowWindow(SW_SHOW);

		m_pView->m_wMotionBase.ShowWindow(SW_SHOW);
		m_pView->m_wMotionBase.SelectWindow(m_pView->m_wMotionBase.m_nTabBtnClickedOld);		
	}
	return;
}



void CTitleBar::OnTitleButton1(){	SelectWindow(0);}
void CTitleBar::OnTitleButton2(){	SelectWindow(1);}
void CTitleBar::OnTitleButton3(){	SelectWindow(2);}


