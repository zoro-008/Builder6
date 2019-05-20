// MoveArea.cpp : implementation file
//

#include "stdafx.h"
#include "HVSystem.h"
#include "Mainfrm.h"
#include "HVSystemDoc.h"
#include "HVSystemView.h"
#include "MoveArea.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define IDT_MOVING WM_USER+4001

/////////////////////////////////////////////////////////////////////////////
// CMoveArea dialog


CMoveArea::CMoveArea(CWnd* pParent /*=NULL*/)
	: CDialog(CMoveArea::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMoveArea)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	int i;
	m_wSysMenu=NULL;
	for(i=0; i<5; i++)
		m_pwButton[i]=NULL;
}


void CMoveArea::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMoveArea)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMoveArea, CDialog)
	//{{AFX_MSG_MAP(CMoveArea)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTONUP, OnButtonup)
	ON_BN_CLICKED(IDC_BUTTONDOWN, OnButtondown)
	ON_BN_CLICKED(IDC_BUTTONRIGHT, OnButtonright)
	ON_BN_CLICKED(IDC_BUTTONLEFT, OnButtonleft)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTONCENTER, OnButtoncenter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMoveArea message handlers

BOOL CMoveArea::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN) {
		switch(pMsg->wParam) {
			case VK_RETURN:
			case VK_ESCAPE: return true;
			default: 
				 break; 
		}
	}	
	if(pMsg->message==WM_LBUTTONDBLCLK) return true;

	return CDialog::PreTranslateMessage(pMsg);
}

void CMoveArea::OnDestroy() 
{
	CDialog::OnDestroy();
	KillTimer(IDT_MOVING);
	int i;
	for(i=0; i<5; i++)
		if(m_pwButton[i]!=NULL) delete m_pwButton[i];
	if(m_wSysMenu!=NULL) delete m_wSysMenu;

	delete pBmpRgn;
}

BOOL CMoveArea::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int i;
/************************************/
	pBmpRgn = new CBmpRgn(this);
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\MoveWindow1280.bmp", 0x00ff00ff);
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

	m_wSysMenu=new CxSkinButton;
	m_wSysMenu->SubclassDlgItem(IDC_BUTTON_SYSMENU_MOVE,this);
	m_wSysMenu->SetSkin(IDB_TITLE_SYSMENU,IDB_TITLE_SYSMENU,IDB_TITLE_SYSMENU,0,0,IDB_TITLE_SYSMENUMASK,0,0,0);
	m_wSysMenu->MoveWindow(2,2,20,20);

/*************************************/	
	UINT nIds[5]=
	{
		IDC_BUTTONUP,
		IDC_BUTTONCENTER,
		IDC_BUTTONDOWN,
		IDC_BUTTONRIGHT,
		IDC_BUTTONLEFT
	};
	UINT nBIds[5][9]=
	{
		{IDB_UPARROWN,IDB_UPARROWD,IDB_UPARROWU,0,0,IDB_MASKARROW,0,0,0},
		{IDB_STOPARROWN,IDB_STOPARROWD,IDB_STOPARROWU,0,0,IDB_MASKARROW,0,0,0},
		{IDB_DOWNARROWN,IDB_DOWNARROWD,IDB_DOWNARROWU,0,0,IDB_MASKARROW,0,0,0},
		{IDB_RIGHTARROWN,IDB_RIGHTARROWD,IDB_RIGHTARROWU,0,0,IDB_MASKARROW,0,0,0},
		{IDB_LEFTARROWN,IDB_LEFTARROWD,IDB_LEFTARROWU,0,0,IDB_MASKARROW,0,0,0},
	};
	


	for(i=0; i<5; i++)
	{
		m_pwButton[i]=new CxSkinButtonEx;
		m_pwButton[i]->SubclassDlgItem(nIds[i],this);
		m_pwButton[i]->SetSkin(nBIds[i][0],nBIds[i][1],nBIds[i][2],nBIds[i][3],
			                nBIds[i][4],nBIds[i][5],nBIds[i][6],nBIds[i][7],nBIds[i][8]);
		if(i<3)  m_pwButton[i]->MoveWindow(90,26+60*i,70,60);
		if(i==3) m_pwButton[i]->MoveWindow(160,26+60,70,60);
		if(i==4) m_pwButton[i]->MoveWindow(20,26+60,70,60);
	}
	SetTimer(IDT_MOVING,50,NULL);

	return TRUE;
}

void CMoveArea::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);

}

void CMoveArea::OnButtonup() 
{

}

void CMoveArea::OnButtondown() 
{
	// TODO: Add your control notification handler code here
	
}

void CMoveArea::OnButtonright() 
{
	// TODO: Add your control notification handler code here
	
}

void CMoveArea::OnButtonleft() 
{
	// TODO: Add your control notification handler code here
	
}

void CMoveArea::OnTimer(UINT nIDEvent) 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
 
	if(nIDEvent==IDT_MOVING)
	{
		if(m_pwButton[0]->m_button_down)
		{
			m_pView->m_wImgDisp1.ResetFindFlag();
			m_pView->m_wImgDisp1.MoveDelta(0,-1);

		}
		if(m_pwButton[2]->m_button_down)
		{
			m_pView->m_wImgDisp1.ResetFindFlag();
			m_pView->m_wImgDisp1.MoveDelta(0,1);
		}
		if(m_pwButton[3]->m_button_down)
		{
			m_pView->m_wImgDisp1.ResetFindFlag();
			m_pView->m_wImgDisp1.MoveDelta(1,0);
		}
		if(m_pwButton[4]->m_button_down)
		{
			m_pView->m_wImgDisp1.ResetFindFlag();
			m_pView->m_wImgDisp1.MoveDelta(-1,0);
		}



	}
	
	CDialog::OnTimer(nIDEvent);
}

void CMoveArea::OnButtoncenter() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	RECT r;
	double sizeb=(double)m_pView->m_wImgDisp1.m_cSettingData.nBSize;
	double scalef;
	scalef=((double)8.91/(double)640.0)+(double)m_pView->m_wImgDisp1.m_cSettingData.nBPixel/10000.0;

    m_pView->m_wImgDisp1.ResetFindFlag();
	m_pView->m_wImgDisp1.Invalidate(FALSE);

	sizeb/=2.0;
	r.left=(int)m_pView->m_wImgDisp1.m_nCx  - (int)(sizeb/scalef+0.5);
	r.right=(int)m_pView->m_wImgDisp1.m_nCx + (int)(sizeb/scalef+0.5);
	r.top=(int)m_pView->m_wImgDisp1.m_nCy  - (int)(sizeb/scalef+0.5);
	r.bottom=(int)m_pView->m_wImgDisp1.m_nCy + (int)(sizeb/scalef+0.5);


	m_pView->m_wImgDisp1.m_cROI.SetRectData(r);
	m_pView->m_wImgDisp1.m_nRoiSet=1;
	m_pView->m_wImgDisp1.Invalidate(FALSE);
}
