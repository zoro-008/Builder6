// DisplayComm.cpp : implementation file
//

#include "stdafx.h"
#include "HVSystem.h"
#include "DisplayComm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDisplayComm dialog


CDisplayComm::CDisplayComm(CWnd* pParent /*=NULL*/)
	: CDialog(CDisplayComm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDisplayComm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nMsgIndex=0;
	m_wSysMenu=NULL;
}


void CDisplayComm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDisplayComm)
	DDX_Control(pDX, IDC_LIST_COMM_EVENT, m_lcListCommEvent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDisplayComm, CDialog)
	//{{AFX_MSG_MAP(CDisplayComm)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDisplayComm message handlers

BOOL CDisplayComm::PreTranslateMessage(MSG* pMsg) 
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
/*****************************************************************
 Prototype : BOOL OnInitDialog()                               
 Function  : Initialize all this window data                   
               1) 윈도우 초기화
			   2) 데이터 리스트 윈도우 초기화
			   3) G-Cap processing button 추가 [020625]
			   4) start time display timer
 Parameter : None
 Return    : None
 Date      : 2002.2
 Author    : Yang Ju Kim
 Remark    : None
******************************************************************/
BOOL CDisplayComm::OnInitDialog() 
{
	CDialog::OnInitDialog();
/************************************/
	pBmpRgn = new CBmpRgn(this);
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\CommMsgWindow1280.bmp", 0x00ff00ff);
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
/*************************************/	
	m_wSysMenu=new CxSkinButton;
	m_wSysMenu->SubclassDlgItem(IDC_BUTTON_SYSMENU_COMM,this);
	m_wSysMenu->SetSkin(IDB_TITLE_SYSMENU,IDB_TITLE_SYSMENU,IDB_TITLE_SYSMENU,0,0,IDB_TITLE_SYSMENUMASK,0,0,0);
	m_wSysMenu->MoveWindow(2,2,20,20);



	m_lcListCommEvent.MoveWindow(2,25,m_wndRect.Width()-6,m_wndRect.Height()-30);

	DWORD dwStyle = m_lcListCommEvent.GetExtendedStyle();
	dwStyle = LVS_EX_FULLROWSELECT|LVS_EX_FLATSB|LVS_EX_GRIDLINES;
	m_lcListCommEvent.SetExtendedStyle(dwStyle);

    InitListCtrlCols();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDisplayComm::OnDestroy() 
{
	CDialog::OnDestroy();

	if(m_wSysMenu!=NULL) delete m_wSysMenu;
	delete pBmpRgn;
}

void CDisplayComm::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);
}


void CDisplayComm::InitListCtrlCols()
{
	// Insert some columns
	CRect rect;
	m_lcListCommEvent.GetClientRect(&rect);
	int nColInterval = rect.Width()/20;

	m_lcListCommEvent.InsertColumn(0, _T("   Time"), LVCFMT_CENTER, nColInterval*6);
	m_lcListCommEvent.InsertColumn(1, _T("Message"), LVCFMT_LEFT, nColInterval*16);

}


void CDisplayComm::AddListData(char *pText)
{
	LV_ITEM lvitem;
	SYSTEMTIME nSysTime;
	char buf[256];
	if(m_nMsgIndex > 100){
		m_lcListCommEvent.DeleteAllItems();
		m_nMsgIndex=0;
	}

	::GetLocalTime(&nSysTime);
//    sprintf(buf,"%4d%02d%02d%02d%02d%02d%03d",nSysTime.wYear,nSysTime.wMonth,
//		    nSysTime.wDay,nSysTime.wHour,nSysTime.wMinute,nSysTime.wSecond,nSysTime.wMilliseconds);

    sprintf(buf,"%02d:%02d:%02d", nSysTime.wHour,nSysTime.wMinute,nSysTime.wSecond);

	lvitem.mask=LVIF_TEXT;
	lvitem.iItem=m_nMsgIndex;
	lvitem.iSubItem=0;
	lvitem.pszText=buf;
	m_lcListCommEvent.InsertItem(&lvitem);
	

	lvitem.iItem=m_nMsgIndex;
	lvitem.iSubItem=1;
	lvitem.pszText=pText;
	m_lcListCommEvent.SetItem(&lvitem);

//  scroll required!!!!
	CSize s(0,50);
	m_nMsgIndex++;
	if(m_nMsgIndex >3)	m_lcListCommEvent.Scroll(s);
    
}

void CDisplayComm::DeleteAllList()
{
	m_lcListCommEvent.DeleteAllItems();
	m_nMsgIndex=0;
}



