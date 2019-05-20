// DialogTabMBase.cpp : implementation file
//

#include "stdafx.h"
#include "hvsystem.h"
#include "DialogTabMBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogTabMBase dialog




CDialogTabMBase::CDialogTabMBase(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTabMBase::IDD, pParent)
{
	int i;
	for(i=0; i<MAX_M_TAP_BTN_NUM; i++)
		m_pbtn[i]=NULL;

	m_pwMotion = NULL;
	m_pwIO = NULL;

	m_nTabBtnClickedOld = 0;


}


void CDialogTabMBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTabMBase)
	DDX_Control(pDX, IDC_TAB_BTN_IO, m_btnIO);
	DDX_Control(pDX, IDC_TAB_BTN_MOTION, m_btnMotion);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogTabMBase, CDialog)
	//{{AFX_MSG_MAP(CDialogTabMBase)
	ON_BN_CLICKED(IDC_TAB_BTN_MOTION, OnTabBtnMotion)
	ON_BN_CLICKED(IDC_TAB_BTN_IO, OnTabBtnIo)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogTabMBase message handlers

BOOL CDialogTabMBase::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitDlgSkin();
	InitControls();
	InitTabDialog();	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogTabMBase::OnTabBtnMotion() 
{
	SelectWindow(TAB_BTN_MOTION);	
}

void CDialogTabMBase::OnTabBtnIo() 
{
	SelectWindow(TAB_BTN_IO);	
}

void CDialogTabMBase::InitControls()
{
	int nI;
	int nOffset=0;
	int nBtnYSize = 40;
	int nBaseYOffset = 40 - 25;
	UINT nIDBMPs[2][9]=
	{
		{IDB_M_MOTION_UNCLICKED ,IDB_M_MOTION_CLICKED ,IDB_M_MOTION_UNCLICKED ,0,0,IDB_MOTIONTABM,0,0,0},
		{IDB_M_IO_UNCLICKED  ,IDB_M_IO_CLICKED  ,IDB_M_IO_UNCLICKED  ,0,0,IDB_MOTIONTABM,0,0,0}		
	};
	CRect rectBtnFirst[MAX_M_TAP_BTN_NUM];

	for(nI = 0; nI < MAX_M_TAP_BTN_NUM; nI++)
	{
		rectBtnFirst[nI].left = m_wndRect.left;
		rectBtnFirst[nI].top = m_wndRect.top + nBtnYSize*nI - nBaseYOffset;
		rectBtnFirst[nI].right = m_wndRect.left + nBtnYSize;
		rectBtnFirst[nI].bottom = m_wndRect.top + nBtnYSize*(nI + 1) - nBaseYOffset;	
	}

 	m_pbtn[TAB_BTN_MOTION] = &m_btnMotion;
 	m_pbtn[TAB_BTN_IO] = &m_btnIO;

 	for(nI = 0; nI < MAX_M_TAP_BTN_NUM; nI++)
 	{				
 		m_pbtn[nI]->SetToolTipText("");
 		m_pbtn[nI]->SetSkin(nIDBMPs[nI][0],nIDBMPs[nI][1],nIDBMPs[nI][2],
 						nIDBMPs[nI][3],nIDBMPs[nI][4],nIDBMPs[nI][5],
 				     nIDBMPs[nI][6],nIDBMPs[nI][7],nIDBMPs[nI][8]); 		
 		m_pbtn[nI]->MoveWindow(&rectBtnFirst[nI]);	
	}
}

void CDialogTabMBase::InitDlgSkin()
{
	pBmpRgn = new CBmpRgn(this);
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\Motion_IO_Window_Base.bmp", 0x00ff00ff);
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
	m_nWindowHeight= pBmpRgn->GetBitmapHeight();
}

void CDialogTabMBase::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDialogTabMBase::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	int i;
	for(i=0; i < MAX_M_TAP_BTN_NUM ; i++)
	{
		if(m_pbtn[i]!=NULL)
		{
			m_pbtn[i] = NULL;
			delete m_pbtn[i];
		}
	}
	
	delete pBmpRgn;

	delete m_pwMotion;
	delete m_pwIO;	
}

void CDialogTabMBase::InitTabDialog()
{

	CRect r(m_wndRect.left + 40, m_wndRect.top + 23, m_wndRect.right, m_wndRect.bottom);

	m_pwMotion=new CDialogTabMMotion;
	m_pwMotion->SetWindowRect(r);
	m_pwMotion->Create(IDD_DIALOG_TAB_M_MOTION ,this);
	m_pwMotion->MoveWindow(&r);

	m_pwIO=new CDialogTabMIO;
	m_pwIO->SetWindowRect(r);
	m_pwIO->Create(IDD_DIALOG_TAB_M_IO,this);
	m_pwIO->MoveWindow(&r);

	m_pwMotion->ShowWindow(SW_HIDE);
	m_pwIO->ShowWindow(SW_HIDE);
}

void CDialogTabMBase::SelectWindow(int nTabNum)
{
	int nI;	
	m_nTabBtnClickedOld = nTabNum;
	for(nI = 0; nI < MAX_M_TAP_BTN_NUM; nI++)
	{		
		if(nI == nTabNum)
		{				
			m_pbtn[nI]->SetCheck(1);			
		}
		else
		{
			m_pbtn[nI]->SetCheck(0);			
		}
	}

	switch(nTabNum)
	{
	case TAB_BTN_MOTION:
		m_pwMotion->ShowWindow(SW_SHOW);
		m_pwIO->ShowWindow(SW_HIDE);
		break;
	case TAB_BTN_IO:
		m_pwMotion->ShowWindow(SW_HIDE);
		m_pwIO->ShowWindow(SW_SHOW);
		break;
	}
}
