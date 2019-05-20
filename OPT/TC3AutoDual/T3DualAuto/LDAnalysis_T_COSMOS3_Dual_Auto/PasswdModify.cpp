// PasswdModify.cpp : implementation file
//

#include "stdafx.h"
#include "HVSystem.h"
#include "Mainfrm.h"
#include "HVSystemDoc.h"
#include "HVSystemView.h"
#include "PasswdModify.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPasswdModify dialog


CPasswdModify::CPasswdModify(CWnd* pParent /*=NULL*/)
	: CDialog(CPasswdModify::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPasswdModify)
	m_PasswdConfirm = _T("");
	m_PasswdNew = _T("");
	//}}AFX_DATA_INIT
	int i;

	for(i=0; i<2; i++)
	{
		pButton[i]=NULL;
		pPasswdEdit[i]=NULL;
	}
}


void CPasswdModify::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasswdModify)
	DDX_Text(pDX, IDC_EDIT_PASSCONFIRM, m_PasswdConfirm);
	DDX_Text(pDX, IDC_EDIT_PASSNEW, m_PasswdNew);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPasswdModify, CDialog)
	//{{AFX_MSG_MAP(CPasswdModify)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasswdModify message handlers

BOOL CPasswdModify::PreTranslateMessage(MSG* pMsg) 
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

BOOL CPasswdModify::OnInitDialog() 
{
	CDialog::OnInitDialog();
/************************************/
	pBmpRgn = new CBmpRgn(this);
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\PasswdModify_256.bmp", 0x00ff00ff);
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

void CPasswdModify::MakeControl()
{
    int i;

	pPasswdEdit[0]=new CEdit;
	pPasswdEdit[0]->SubclassDlgItem(IDC_EDIT_PASSNEW,this);
	pPasswdEdit[0]->MoveWindow(70,29,120,24);

	pPasswdEdit[1]=new CEdit;
	pPasswdEdit[1]->SubclassDlgItem(IDC_EDIT_PASSCONFIRM,this);
	pPasswdEdit[1]->MoveWindow(70,58,120,24);

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
		pButton[i]->MoveWindow(10+95*i,90,80,30);
	}

//////////////////////////


}

void CPasswdModify::OnDestroy() 
{
	CDialog::OnDestroy();
	int i;
	
	for(i=0;i<2; i++)
	{
		if(pButton[i]!=NULL) delete pButton[i];
		if(pPasswdEdit[i]!=NULL) delete pPasswdEdit[i];
	}

    delete pBmpRgn;	
}

void CPasswdModify::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);	
	// Do not call CDialog::OnPaint() for painting messages
}

void CPasswdModify::OnOK() 
{
	// TODO: Add extra validation here
	CString bufnew,bufconfirm;
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	UpdateData(TRUE);    
	pPasswdEdit[0]->GetWindowText(bufnew);
	pPasswdEdit[1]->GetWindowText(bufconfirm);
//    m_nMinute=atoi());
    if(bufnew==bufconfirm)
	{
		FILE *fp;
		fp=fopen(PASSWORD_FILE,"w+t");
		fprintf(fp,"%s",bufnew.GetBuffer(bufnew.GetLength()));
		fclose(fp);
	}
	else
	{
		m_pView->m_wDisplayComm.AddListData("Password mismatch!");
		return;
	}
	UpdateData(FALSE);
	CDialog::OnOK();
}

