// DialogMessage.cpp : implementation file
//

#include "stdafx.h"
#include "HVsystem.h"
#include "Mainfrm.h"
#include "HVsystemDoc.h"
#include "HVsystemView.h"
#include "DialogMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define WAIT_SENSOR_TIMER WM_USER+3003
/////////////////////////////////////////////////////////////////////////////
// CDialogMessage dialog


CDialogMessage::CDialogMessage(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMessage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogMessage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pBmpRgn=NULL;
	int i;
	for(i=0; i<10; i++)
		m_nStr[i][0]='\0';
	m_nMCount=0;
	m_wStatic=NULL;
	for(i=0; i<2; i++)
		m_wButton[i]=NULL;

	m_nOperationMode=0;
	m_nCharWidth=18;
	m_nCharHeight=10;
}


void CDialogMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMessage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogMessage, CDialog)
	//{{AFX_MSG_MAP(CDialogMessage)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMessage message handlers

BOOL CDialogMessage::PreTranslateMessage(MSG* pMsg) 
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
	return CDialog::PreTranslateMessage(pMsg);
}

void CDialogMessage::OnDestroy() 
{
	CDialog::OnDestroy();
    if(pBmpRgn!=NULL)       delete pBmpRgn;	
	if(m_wStatic!=NULL)     delete m_wStatic;
	
	int i;
	for(i=0; i<2; i++)
		if(m_wButton[i]!=NULL) delete m_wButton[i];

	// TODO: Add your message handler code here
	
}

BOOL CDialogMessage::OnInitDialog() 
{
	CDialog::OnInitDialog();
////////////////////////////////////////////////////
	pBmpRgn=new CBmpRgn();
	pBmpRgn->SetParentWindow(this);
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\message.bmp", 0x00ff00ff);
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

	MoveWindow(300,200,280,300); // WINDOW define
    //////////////////////////////////////////////////////////////////////////////
	m_wStatic=new CStaticSkin;

	m_wStatic->SetBmpFile(".\\guiimage\\static280x230.bmp");
	m_wStatic->SubclassDlgItem(IDC_STATIC_MESSAGE,this);
	m_wStatic->MoveWindow(10,10,260,230);
	m_wStatic->SetFontSize(m_nCharHeight,m_nCharWidth,"Arial");
	DrawMessage();
	///////////////////////////////////////////////////////////
	UINT nIDs[2]=
	{
		IDOK,
		IDCANCEL
	};
	
	UINT nBTNIDs[2][9]=
	{  //normal,down,over,disable,focus,mask.draw mode,border,margin.
		{IDB_OKN100x50    ,IDB_OKD100x50    ,IDB_OKU100x50    ,0   ,0  ,IDB_OCMASK100x50,0,0,0},
		{IDB_CANCELN100x50,IDB_CANCELD100x50,IDB_CANCELU100x50,0   ,0  ,IDB_OCMASK100x50,0,0,0}
	}; 

	char *pBtnToolTip[2]={"",""};

	int i;

	for(i=0; i<2; i++)
	{
		m_wButton[i]=new CxSkinButton;

		m_wButton[i]->SubclassDlgItem(nIDs[i],this);
		m_wButton[i]->SetToolTipText(pBtnToolTip[i]);
		m_wButton[i]->SetSkin(nBTNIDs[i][0],nBTNIDs[i][1],nBTNIDs[i][2],
								 nBTNIDs[i][3],nBTNIDs[i][4],nBTNIDs[i][5],
								 nBTNIDs[i][6],nBTNIDs[i][7],nBTNIDs[i][8]);
		m_wButton[i]->MoveWindow(10+160*i,245,100,50);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogMessage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);
	
	// Do not call CDialog::OnPaint() for painting messages
}


void CDialogMessage::SetMessage(char *m,int idx)
{
	m_nMCount=idx+1;
	strcpy(m_nStr[idx],m);
}

void CDialogMessage::DrawMessage()
{
	int i;
	for(i=0; i<m_nMCount; i++)
		m_wStatic->OutTextContXY(10,20+20*i,m_nStr[i],i);
	m_wStatic->Invalidate(FALSE);
}

