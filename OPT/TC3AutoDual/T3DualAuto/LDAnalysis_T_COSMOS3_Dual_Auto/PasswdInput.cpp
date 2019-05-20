// PasswdInput.cpp : implementation file
//

#include "stdafx.h"
#include "hvsystem.h"
#include "PasswdInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPasswdInput dialog


CPasswdInput::CPasswdInput(CWnd* pParent /*=NULL*/)
	: CDialog(CPasswdInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPasswdInput)
	m_Passwd = _T("");
	//}}AFX_DATA_INIT
	pPasswdEdit=NULL;
	int i;

	for(i=0; i<2; i++)
		pButton[i]=NULL;
}


void CPasswdInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasswdInput)
	DDX_Text(pDX, IDC_EDIT_PASSINPUT, m_Passwd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPasswdInput, CDialog)
	//{{AFX_MSG_MAP(CPasswdInput)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasswdInput message handlers

BOOL CPasswdInput::PreTranslateMessage(MSG* pMsg) 
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

void CPasswdInput::OnDestroy() 
{
	CDialog::OnDestroy();
    if(pPasswdEdit!=NULL) delete pPasswdEdit;
	int i;
	for(i=0; i<2; i++)
		if(pButton[i]!=NULL) delete pButton[i];
	delete pBmpRgn;	

}

BOOL CPasswdInput::OnInitDialog() 
{
	CDialog::OnInitDialog();
/************************************/
	pBmpRgn = new CBmpRgn(this);
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\PasswdWindow_256.bmp", 0x00ff00ff);
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
	MoveWindow(600,400,m_nWindowWidth,m_nWindowHeight);
	MakeControl();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPasswdInput::MakeControl()
{
	pPasswdEdit=new CEdit;
	pPasswdEdit->SubclassDlgItem(IDC_EDIT_PASSINPUT,this);
	pPasswdEdit->MoveWindow(90,30,100,24);

	int i;
///////////////////////////
	UINT nBTNIDs[2]={IDOK,IDCANCEL};
	UINT nIDBMPs[2][9]=
	{
		{IDB_OKN,IDB_OKD,IDB_OKD,0,0,IDB_OKCANCELMASK,0,0,0},
		{IDB_CANCELN,IDB_CANCELD,IDB_CANCELD,0,0,IDB_OKCANCELMASK,0,0,0}
	};

	for(i=0; i<2; i++)
	{
		pButton[i]=new CxSkinButton;
		pButton[i]->SubclassDlgItem(nBTNIDs[i], this);
		pButton[i]->SetToolTipText("");
		pButton[i]->SetSkin(nIDBMPs[i][0],nIDBMPs[i][1],nIDBMPs[i][2],
			                     nIDBMPs[i][3],nIDBMPs[i][4],nIDBMPs[i][5],
							     nIDBMPs[i][6],nIDBMPs[i][7],nIDBMPs[i][8]);
		pButton[i]->MoveWindow(10+95*i,62,80,30);
	}

//////////////////////////


}

void CPasswdInput::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);
	
	
	// Do not call CDialog::OnPaint() for painting messages
}
