// CurrentData.cpp : implementation file
//

#include "stdafx.h"
#include "hvsystem.h"
#include "CurrentData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCurrentData dialog


CCurrentData::CCurrentData(CWnd* pParent /*=NULL*/)
	: CDialog(CCurrentData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCurrentData)
	//}}AFX_DATA_INIT
	int i,j;

	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
		{
			memset(m_cBuf[i][j],0x00,256);
		}
}


void CCurrentData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCurrentData)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCurrentData, CDialog)
	//{{AFX_MSG_MAP(CCurrentData)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurrentData message handlers

BOOL CCurrentData::PreTranslateMessage(MSG* pMsg) 
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

void CCurrentData::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	delete pBmpRgn;	
}

BOOL CCurrentData::OnInitDialog() 
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
	// TODO: Add extra initialization here
	m_cListGeneral1.SubclassDlgItem(IDC_LIST_GNERALDATA1,this);
	m_cListGeneral1.MoveWindow(2,2,m_wndRect.Width()-8,64);
	
	DWORD dwStyle = m_cListGeneral1.GetExtendedStyle();

	dwStyle = LVS_EX_FLATSB|LVS_EX_GRIDLINES ;
	m_cListGeneral1.SetExtendedStyle(dwStyle);

	m_cListGeneral2.SubclassDlgItem(IDC_LIST_GNERALDATA2,this);
	m_cListGeneral2.MoveWindow(2,76,m_wndRect.Width()-8,64);
	m_cListGeneral2.SetExtendedStyle(dwStyle);

	m_cListPeak.SubclassDlgItem(IDC_LIST_PEAKDATA,this);
	m_cListPeak.MoveWindow(2,152,m_wndRect.Width()-8,64);
	m_cListPeak.SetExtendedStyle(dwStyle);

	m_cListRim.SubclassDlgItem(IDC_LIST_RIMDATA,this);
	m_cListRim.MoveWindow(2,228,m_wndRect.Width()-8,64);
	m_cListRim.SetExtendedStyle(dwStyle);
	
	InitListCtrlCols();
	InitDataField();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCurrentData::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);
	

	
	// Do not call CDialog::OnPaint() for painting messages
}

void CCurrentData::InitListCtrlCols()
{
	// Insert some columns
	CRect rect;
	
	m_cListGeneral1.GetClientRect(&rect);
	int nColInterval = rect.Width()/12;
 
	m_cListGeneral1.InsertColumn(0, _T("Field Angle"), LVCFMT_CENTER, nColInterval*6);
	m_cListGeneral1.InsertColumn(1, _T("Tilt(DEG.)"), LVCFMT_CENTER, nColInterval*3);
	m_cListGeneral1.InsertColumn(2, _T("Beam Size"), LVCFMT_CENTER, nColInterval*4);
	m_cListGeneral1.InsertColumn(3, _T("GT shift"), LVCFMT_CENTER, nColInterval*4);
	m_nSubItemNum[0]=4;

	m_cListGeneral2.InsertColumn(0, _T("Area(mm)"), LVCFMT_CENTER, nColInterval*4);
	m_cListGeneral2.InsertColumn(1, _T("P-Ratio(%)"), LVCFMT_CENTER, nColInterval*4);
	m_cListGeneral2.InsertColumn(2, _T("Para.(%)"), LVCFMT_CENTER, nColInterval*5);
	m_nSubItemNum[1]=3;

	m_cListPeak.GetClientRect(&rect);
	nColInterval = rect.Width()/12;

	m_cListPeak.InsertColumn(0, _T("A-Center"), LVCFMT_CENTER, nColInterval*4);
	m_cListPeak.InsertColumn(1, _T("I-Center"), LVCFMT_CENTER, nColInterval*4);
	m_cListPeak.InsertColumn(2, _T("P-Center"), LVCFMT_CENTER, nColInterval*4);
	m_cListPeak.InsertColumn(3, _T("Beam Shift"), LVCFMT_CENTER, nColInterval*6);
	m_nSubItemNum[2]=4;
	
	m_cListRim.GetClientRect(&rect);
	nColInterval = rect.Width()/12;

	m_cListRim.InsertColumn(0, _T("HRIM Loc"), LVCFMT_CENTER, nColInterval*3);
	m_cListRim.InsertColumn(1, _T("VRIM Loc"), LVCFMT_CENTER, nColInterval*3);
	m_cListRim.InsertColumn(2, _T("HRIM Val(%)"), LVCFMT_CENTER, nColInterval*4);
	m_cListRim.InsertColumn(3, _T("VRIM Val(%)"), LVCFMT_CENTER, nColInterval*4);
	m_nSubItemNum[3]=4;
	return;

}

void CCurrentData::InitDataField()
{
	LV_ITEM lvitem;
    int i;
	
	for(i=0; i<m_nSubItemNum[0]; i++)
	{
		lvitem.mask=LVIF_TEXT;
		lvitem.iItem=0;
		lvitem.iSubItem=i;
		lvitem.pszText="-";
		if(i==0)		m_cListGeneral1.InsertItem(&lvitem);
		else			m_cListGeneral1.SetItem(&lvitem);
	}
	for(i=0; i<m_nSubItemNum[1]; i++)
	{
		lvitem.mask=LVIF_TEXT;
		lvitem.iItem=0;
		lvitem.iSubItem=i;
		lvitem.pszText="-";
		if(i==0)		m_cListGeneral2.InsertItem(&lvitem);
		else			m_cListGeneral2.SetItem(&lvitem);
	}

	for(i=0; i<m_nSubItemNum[2]; i++)
	{
		lvitem.mask=LVIF_TEXT;
		lvitem.iItem=0;
		lvitem.iSubItem=i;
		lvitem.pszText="-";
		if(i==0)		m_cListPeak.InsertItem(&lvitem);
		else			m_cListPeak.SetItem(&lvitem);
	}

	for(i=0; i<m_nSubItemNum[3]; i++)
	{
		lvitem.mask=LVIF_TEXT;
		lvitem.iItem=0;
		lvitem.iSubItem=i;
		lvitem.pszText="-";
		if(i==0)		m_cListRim.InsertItem(&lvitem);
		else			m_cListRim.SetItem(&lvitem);
	}


}

void CCurrentData::SetListData(int listnum,int nSubItem,char *pText)
{
	LV_ITEM lvitem;

	lvitem.mask=LVIF_TEXT;
	lvitem.iItem=0;
	lvitem.iSubItem=nSubItem;
	lvitem.pszText=pText;

	strcpy(m_cBuf[listnum][nSubItem],pText);

	switch(listnum)
	{
	case 0:
		m_cListGeneral1.SetItem(&lvitem);
		break;
	case 1:
		m_cListGeneral2.SetItem(&lvitem);
		break;
	case 2:
		m_cListPeak.SetItem(&lvitem);
		break;
	case 3:
		m_cListRim.SetItem(&lvitem);
		break;
	default:
		break;
	}

    return;
}
