// ListData.cpp : implementation file
//

#include "stdafx.h"
#include "HVSystem.h"
#include "ListData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListData dialog


CListData::CListData(CWnd* pParent /*=NULL*/)
	: CDialog(CListData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListData)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_wndRect.SetRect(0,0,10,10);
	m_nListNumber=0;
}


void CListData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListData)
	DDX_Control(pDX, IDC_LIST_DATA_REPORT, m_lcData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListData, CDialog)
	//{{AFX_MSG_MAP(CListData)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListData message handlers
BOOL CListData::PreTranslateMessage(MSG* pMsg) 
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

BOOL CListData::OnInitDialog() 
{
	CDialog::OnInitDialog();
/************************************/
	pBmpRgn = new CBmpRgn(this);
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\DataListWindow640x235_256.bmp", 0x00ff00ff);
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

	
    //title bar가 있을 경우  width '-6', height '-26'
	m_lcData.MoveWindow(2,26,m_wndRect.Width()-6,m_wndRect.Height()-88);

	DWORD dwStyle = m_lcData.GetExtendedStyle();
	dwStyle = LVS_EX_FULLROWSELECT|LVS_EX_FLATSB|LVS_EX_GRIDLINES;
	m_lcData.SetExtendedStyle(dwStyle);
    InitListCtrlCols();


	MakeDisplayDataWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CListData::MakeDisplayDataWindow()
{
	UINT nIDs[2][5]=
	{ 
		{IDC_LIST_TITLE1,IDC_LIST_TITLE2,IDC_LIST_TITLE3,IDC_LIST_TITLE4,IDC_LIST_TITLE5},
		{IDC_LIST_MODEL ,IDC_LIST_GOOD  ,IDC_LIST_BAD   ,IDC_LIST_TOTAL ,IDC_LIST_RATIO}
	};
	char *pBmpFiles[2]={".\\guiimage\\model_256.bmp",".\\guiimage\\datas_256.bmp"};

#ifdef CHINA_VERSION
	char *pTitle[2][5]=
	{
		{"Model","Good","Fail","Total","Fail rate(%)"},
		{"DEFAULT","0","0","0","0%"}
	};
#else
	char *pTitle[2][5]=
	{
		{"모델명","양품수","불량수","합계","불량율"},
		{"DEFAULT","0","0","0","0%"}
	};
#endif

	int i;

	for(i=0;i<5; i++)
	{
		m_wTitle[i].SetBmpFile(pBmpFiles[0]);
		m_wTitle[i].SubclassDlgItem(nIDs[0][i],this);
		m_wTitle[i].MoveWindow(2+(128)*i,m_wndRect.Height()-60,118*(i+1),25);
		m_wTitle[i].SetFgColor(RGB(0,64,128));
		m_wTitle[i].OutText(pTitle[0][i]);

		m_wData[i].SetBmpFile(pBmpFiles[1]);
		m_wData[i].SubclassDlgItem(nIDs[1][i],this);
		m_wData[i].MoveWindow(2+128*i,m_wndRect.Height()-35,118*(i+1),25);
		m_wData[i].SetFgColor(RGB(0,0,0));
		m_wData[i].OutText(pTitle[1][i]);

	}

}

void CListData::DisplayExtText(int nIdx,char *buf)
{
	m_wData[nIdx].OutTextII(buf);
}

void CListData::InitListCtrlCols()
{
	// Insert some columns
	CRect rect;
	m_lcData.GetClientRect(&rect);
	int nColInterval = rect.Width()/20;

#ifdef CHINA_VERSION
	m_lcData.InsertColumn(0, _T("No."), LVCFMT_CENTER, nColInterval*2);
	m_lcData.InsertColumn(1, _T("None"),LVCFMT_CENTER, nColInterval*2); 
	m_lcData.InsertColumn(2, _T("Size"), LVCFMT_CENTER, nColInterval*2);
	m_lcData.InsertColumn(3, _T("Mark"), LVCFMT_CENTER, nColInterval*2);
	m_lcData.InsertColumn(4, _T("Scratch&Dust"), LVCFMT_CENTER, nColInterval*4);
	m_lcData.InsertColumn(5, _T("Particle"), LVCFMT_CENTER,(int)((double)nColInterval*2.5 +0.5));
	m_lcData.InsertColumn(6, _T("Package"), LVCFMT_CENTER, (int)((double)nColInterval*2.5 +0.5));
	m_lcData.InsertColumn(7, _T("size over"),LVCFMT_CENTER,nColInterval*3);
	AddListData(0,0,"Total");
#else
	m_lcData.InsertColumn(0, _T("번호"), LVCFMT_CENTER, nColInterval*3);
	m_lcData.InsertColumn(1, _T("없음"),LVCFMT_CENTER, nColInterval*2); 
	m_lcData.InsertColumn(2, _T("치수"), LVCFMT_CENTER, nColInterval*2);
	m_lcData.InsertColumn(3, _T("마킹"), LVCFMT_CENTER, nColInterval*2);
	m_lcData.InsertColumn(4, _T("긁힘&이물"), LVCFMT_CENTER, nColInterval*3);
	m_lcData.InsertColumn(5, _T("파티클"), LVCFMT_CENTER, nColInterval*3);
	m_lcData.InsertColumn(6, _T("포장"), LVCFMT_CENTER, nColInterval*2);
	m_lcData.InsertColumn(7, _T("크기오버"),LVCFMT_CENTER,nColInterval*3);
	AddListData(0,0,"합계");
#endif
	int i;
	for(i=1; i<=7; i++)	AddListData(0,i,"0");
}

void CListData::AddListDataII(int nCols,long nNum)
{
	LV_ITEM lvitem;
	if(m_nListNumber > 2400){
		m_lcData.DeleteAllItems();
		m_nListNumber=0;
	}

	lvitem.mask=LVIF_TEXT;
	lvitem.iItem=m_nListNumber;

//////////////////////////////////////////////////////


	int i;
	char buf[256];

	sprintf(buf,"%d",nNum);

	lvitem.iSubItem=0;
	lvitem.pszText=buf;

	m_lcData.InsertItem(&lvitem);

	for(i=1; i<8; i++)
	{
		lvitem.iSubItem=i;
		if(i==nCols)	lvitem.pszText="1";
		else  
		{
			lvitem.pszText="0";
		}
		m_lcData.SetItem(&lvitem);
	}
//////////////////////////////////////////////////////


//  scroll required!!!!
	CSize s(0,48);
	if(m_nListNumber >3)	m_lcData.Scroll(s);
	m_nListNumber++;
}


void CListData::AddListData(int nRow,int nCols,char *pText)
{
	LV_ITEM lvitem;

	lvitem.mask=LVIF_TEXT;
	lvitem.iItem=nRow;
	lvitem.iSubItem=nCols;
	lvitem.pszText=pText;
	if(nCols==0) m_lcData.InsertItem(&lvitem);
	else	     m_lcData.SetItem(&lvitem);

//  scroll required!!!!
	CSize s(0,24);
	if(nRow >3)	m_lcData.Scroll(s);
}



void CListData::DeleteAllListData()
{
	m_lcData.DeleteAllItems();
	m_nListNumber=0;

#ifdef CHINA_VERSION
	AddListData(0,0,"Total");
#else
	AddListData(0,0,"합계");
#endif

	int i;
	for(i=1; i<=7; i++)	AddListData(0,i,"0");
}


void CListData::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);
}

void CListData::OnDestroy() 
{
	CDialog::OnDestroy();
	
	delete pBmpRgn;
	
}
