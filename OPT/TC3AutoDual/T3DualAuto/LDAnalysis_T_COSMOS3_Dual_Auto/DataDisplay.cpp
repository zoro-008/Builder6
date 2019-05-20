// DataDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "hvsystem.h"
#include "DataDisplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataDisplay dialog


CDataDisplay::CDataDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CDataDisplay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataDisplay)
	//}}AFX_DATA_INIT
//	m_wndRect.SetRect(0,0,10,10);
	m_nListNumber=0;
	m_nListColor=RGB(0,0,0);
}


void CDataDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataDisplay)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataDisplay, CDialog)
	//{{AFX_MSG_MAP(CDataDisplay)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CUSTOMDRAW,IDC_LIST_DATADISPLAY,OnCustomdrawList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataDisplay message handlers

BOOL CDataDisplay::PreTranslateMessage(MSG* pMsg) 
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
	if(pMsg->message==WM_LBUTTONDBLCLK) return true;
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDataDisplay::OnDestroy() 
{
	CDialog::OnDestroy();

	delete pBmpRgn;
}

BOOL CDataDisplay::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
/************************************/
	pBmpRgn = new CBmpRgn(this);
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\SetupWindowU_256.bmp", 0x00ff00ff);
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
	m_lcData.SubclassDlgItem(IDC_LIST_DATADISPLAY,this);
	m_lcData.MoveWindow(2,2,m_wndRect.Width()-6,m_wndRect.Height()-6);

	DWORD dwStyle = m_lcData.GetExtendedStyle();
	dwStyle = LVS_EX_FULLROWSELECT|LVS_EX_FLATSB|LVS_EX_GRIDLINES;
	m_lcData.SetExtendedStyle(dwStyle);

	InitListCtrlCols();

/////////////////////////////////////

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDataDisplay::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);
}


void CDataDisplay::InitListCtrlCols()
{
	// Insert some columns
	CRect rect;
	
	m_lcData.GetClientRect(&rect);
	int nColInterval = rect.Width()/20;
    switch(m_nAppType)
	{
	case 0: // bearing
		m_lcData.InsertColumn(0, _T("Time"), LVCFMT_CENTER, nColInterval*4);
		m_lcData.InsertColumn(1, _T("Num."), LVCFMT_CENTER, nColInterval*4);
		m_lcData.InsertColumn(2, _T("Angle."), LVCFMT_CENTER, nColInterval*4);
		m_lcData.InsertColumn(3, _T("Tilt"), LVCFMT_CENTER, nColInterval*4);
		m_lcData.InsertColumn(4, _T("Dia."), LVCFMT_CENTER, nColInterval*4);
		m_lcData.InsertColumn(5, _T("GT_SHT"), LVCFMT_CENTER, nColInterval*4);
		m_lcData.InsertColumn(6, _T("Area"), LVCFMT_CENTER, nColInterval*4);
		m_lcData.InsertColumn(7, _T("P-Ratio"), LVCFMT_CENTER, nColInterval*4);
		m_lcData.InsertColumn(8, _T("Para."), LVCFMT_CENTER, nColInterval*4);
		m_lcData.InsertColumn(9, _T("A-C"), LVCFMT_CENTER, nColInterval*6);
		m_lcData.InsertColumn(10, _T("I-C"), LVCFMT_CENTER, nColInterval*6);
		m_lcData.InsertColumn(11, _T("P-C"), LVCFMT_CENTER, nColInterval*6);
		m_lcData.InsertColumn(12, _T("I-PD"), LVCFMT_CENTER, nColInterval*4);
		m_lcData.InsertColumn(13, _T("HRL"), LVCFMT_CENTER, nColInterval*6);
		m_lcData.InsertColumn(14, _T("VRL"), LVCFMT_CENTER, nColInterval*6);
		m_lcData.InsertColumn(15, _T("HRV"), LVCFMT_CENTER, nColInterval*4);
		m_lcData.InsertColumn(16, _T("VRV"), LVCFMT_CENTER, nColInterval*4);
		m_nAppColumnNum=16;
		break;

	default:
		m_lcData.InsertColumn(0, _T("   Time"), LVCFMT_CENTER, nColInterval*4);
		m_lcData.InsertColumn(1, _T("Message"), LVCFMT_CENTER, nColInterval*16);
		m_nAppColumnNum=1;
		break;
	}
	return;

}


void CDataDisplay::AddListData(char *pText,int nSubItem)
{
	LV_ITEM lvitem;
    int nCountNum;

	::GetLocalTime(&m_nSysTime);

	nCountNum=m_lcData.GetItemCount(); 
	lvitem.mask=LVIF_TEXT;
    if(nSubItem==1)
	{
		sprintf(m_strCurrentTime,"%02d:%02d:%02d", m_nSysTime.wHour,m_nSysTime.wMinute,m_nSysTime.wSecond);
		lvitem.iItem=nCountNum;
		lvitem.iSubItem=0;
		lvitem.pszText=m_strCurrentTime;
		m_lcData.InsertItem(&lvitem);
	
	}

	
	nCountNum=m_lcData.GetItemCount(); 
	lvitem.iItem=nCountNum-1;
	lvitem.iSubItem=nSubItem;
	lvitem.pszText=pText;
	m_lcData.SetItem(&lvitem);
    

	CSize s(0,50);
	
	if(m_lcData.GetItemCount() >6)	m_lcData.Scroll(s);

	nCountNum=m_lcData.GetItemCount(); 
	if((nCountNum>=MAX_DISPLAY_NUM) &&(m_nAppColumnNum==nSubItem))
	{
		m_lcData.DeleteAllItems();
		m_lcData.SetItemCount(0);
	}
    return;
}


void CDataDisplay::DeleteAllList()
{
	m_lcData.DeleteAllItems();
	m_lcData.SetItemCount(0);
	return;
}
//////////////////////////////////////////////////////////////////////
// by Michael Dunn
//////////////////////////////////////////////////////////////////////
void CDataDisplay::OnCustomdrawList ( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

    // Take the default processing unless we set this to something else below.
    *pResult = 0;

    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.

    if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
        {
        *pResult = CDRF_NOTIFYITEMDRAW;
        }
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
        {
        // This is the notification message for an item.  We'll request
        // notifications before each subitem's prepaint stage.

        *pResult = CDRF_NOTIFYSUBITEMDRAW;
        }
    else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
        {
        // This is the prepaint stage for a subitem. Here's where we set the
        // item's text and background colors. Our return value will tell 
        // Windows to draw the subitem itself, but it will use the new colors
        // we set here.
        // The text color will cycle through red, green, and light blue.
        // The background color will be light blue for column 0, red for
        // column 1, and black for column 2.
    
        COLORREF crText=RGB(0,0,0);
//		COLORREF crBkgnd;
        
		CString str;
		
        if ( 2 == pLVCD->iSubItem )
        {
			str=m_lcData.GetItemText(pLVCD->nmcd.dwItemSpec ,2);
//			if(strcmp(str.GetBuffer(str.GetLength()),"0")==1)
//				crText = RGB(255,0,0);
//			else
				crText = RGB(0,0,0);
        }
        if ( 3 == pLVCD->iSubItem )
        {
			str=m_lcData.GetItemText(pLVCD->nmcd.dwItemSpec ,3);
//			if(strcmp(str.GetBuffer(str.GetLength()),"0")==1)
//				crText = RGB(255,0,0);
//			else
				crText = RGB(0,0,0);
        }

        // Store the colors back in the NMLVCUSTOMDRAW struct.
        pLVCD->clrText = crText;
  //      pLVCD->clrTextBk = crBkgnd;

        // Tell Windows to paint the control itself.
        *pResult = CDRF_DODEFAULT;
        }
}
