// IoWindow.cpp : implementation file
//

#include "stdafx.h"
#include "HVSystem.h"
#include "IoWindow.h"
#include "DialogMessage.h"
#include "MainFrm.h"
#include "HVSystemDefine.h"

#ifdef _IO_ENABLE

//#include "Dask.h"
//#pragma comment(lib,"PCI-Dask.lib")
short g_nDev,g_nCardNum=0;
unsigned short g_DiNum;
unsigned short  g_DoNum;

#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CIoWindow dialog
short g_Bit[16]= 
{
	(short)0x0001,(short)0x0002,(short)0x0004,(short)0x0008,
		(short)0x0010,(short)0x0020,(short)0x0040,(short)0x0080,
		(short)0x0100,(short)0x0200,(short)0x0400,(short)0x0800,
		(short)0x1000,(short)0x2000,(short)0x4000,(short)0x8000
};

CIoWindow::CIoWindow(CWnd* pParent /*=NULL*/)
: CDialog(CIoWindow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIoWindow)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nDeviceSuccess=FALSE;
	m_nCheckFlag=FALSE;
	m_nDevNum=2;
	m_nOutputValue=(short)0xffff;
	m_wLinkButton=NULL;
}


void CIoWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIoWindow)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIoWindow, CDialog)
//{{AFX_MSG_MAP(CIoWindow)
ON_WM_PAINT()
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_IO_OUT0, OnIoOut0)
ON_BN_CLICKED(IDC_IO_OUT1, OnIoOut1)
ON_BN_CLICKED(IDC_IO_OUT2, OnIoOut2)
ON_BN_CLICKED(IDC_IO_OUT3, OnIoOut3)
ON_BN_CLICKED(IDC_IO_OUT4, OnIoOut4)
ON_BN_CLICKED(IDC_IO_OUT5, OnIoOut5)
ON_BN_CLICKED(IDC_IO_OUT6, OnIoOut6)
ON_BN_CLICKED(IDC_IO_OUT7, OnIoOut7)
ON_BN_CLICKED(IDC_IO_OUT8, OnIoOut8)
ON_BN_CLICKED(IDC_IO_OUT9, OnIoOut9)
ON_BN_CLICKED(IDC_IO_OUT10, OnIoOut10)
ON_BN_CLICKED(IDC_IO_OUT11, OnIoOut11)
ON_BN_CLICKED(IDC_IO_OUT12, OnIoOut12)
ON_BN_CLICKED(IDC_IO_OUT13, OnIoOut13)
ON_BN_CLICKED(IDC_IO_OUT14, OnIoOut14)
ON_BN_CLICKED(IDC_IO_OUT15, OnIoOut15)
ON_BN_CLICKED(IDC_BUTTON_LINK, OnButtonLink)
ON_BN_CLICKED(IDC_BTN_UI_SHOW, OnBtnUiShow)
ON_BN_CLICKED(IDC_BTN_ALL_HOME, OnBtnAllHome)
ON_BN_CLICKED(IDC_BTN_RESET, OnBtnReset)
ON_BN_CLICKED(IDC_BTN_APC_LEFT, OnBtnApcLeft)
ON_BN_CLICKED(IDC_BTN_APC_RIGHT, OnBtnApcRight)
	ON_BN_CLICKED(IDC_BTN_APC_UP, OnBtnApcUp)
	ON_BN_CLICKED(IDC_BTN_APC_DOWN, OnBtnApcDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIoWindow message handlers

BOOL CIoWindow::PreTranslateMessage(MSG* pMsg) 
{
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

void CIoWindow::OnDestroy() 
{
	CDialog::OnDestroy();
	delete pBmpRgn;	
	
	if(m_wLinkButton!=NULL) delete m_wLinkButton;
	
	ResetAll();
	
}

BOOL CIoWindow::OnInitDialog() 
{
	CDialog::OnInitDialog();
	pBmpRgn=new CBmpRgn();
	pBmpRgn->SetParentWindow(this);
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\IOWindowWooju.bmp", 0x00ff00ff);
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
	
    OpenNetwork();
	//////////////////////
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CIoWindow::MakeGui()
{
	UINT nOutIds[16]=
	{
		IDC_IO_OUT0 ,IDC_IO_OUT1 ,IDC_IO_OUT2 ,IDC_IO_OUT3,
			IDC_IO_OUT4 ,IDC_IO_OUT5 ,IDC_IO_OUT6 ,IDC_IO_OUT7,
			IDC_IO_OUT8 ,IDC_IO_OUT9 ,IDC_IO_OUT10,IDC_IO_OUT11,
			IDC_IO_OUT12,IDC_IO_OUT13,IDC_IO_OUT14,IDC_IO_OUT15
	};
	CString nStr;
	int i;
	char *OStr[16]=
	{
		"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
	};
	
	char buf[256];
	for(i=0;i<16; i++)
	{
		m_wOut[i].SubclassDlgItem(nOutIds[i], this);
		
		nStr.Format("%s",OStr[i]);
		m_wOut[i].SetToolTipText(nStr);
		m_wOut[i].SetSkin(IDB_IOOFF,IDB_IOON,IDB_IOOFF,0,0,IDB_IOMASK,0,0,0);
		m_wOut[i].MoveWindow(78+i*20,3,20,20);
		m_wOut[i].SetTextColor(RGB(0,255,0));
		sprintf(buf,"%d",i);
		m_wOut[i].SetWindowText(buf);
		m_wOut[i].ShowWindow(SW_HIDE);
	}
	
	UINT nInIds[16]=
	{
		IDC_IO_IN0 ,IDC_IO_IN1 ,IDC_IO_IN2 ,IDC_IO_IN3,
			IDC_IO_IN4 ,IDC_IO_IN5 ,IDC_IO_IN6 ,IDC_IO_IN7,
			IDC_IO_IN8 ,IDC_IO_IN9 ,IDC_IO_IN10,IDC_IO_IN11,
			IDC_IO_IN12,IDC_IO_IN13,IDC_IO_IN14,IDC_IO_IN15
	};
	
	char *InStr[16]=
	{
		"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
	};
	
	for(i=0;i<16; i++)
	{
		m_wIn[i].SubclassDlgItem(nInIds[i], this);
		
		nStr.Format("%s",InStr[i]);
		m_wIn[i].SetToolTipText(nStr);
		m_wIn[i].SetSkin(IDB_IOOFF,IDB_IOON,IDB_IOOFF,0,0,IDB_IOMASK,0,0,0);
		m_wIn[i].MoveWindow(78+i*20,28,20,20);
		m_wIn[i].SetTextColor(RGB(255,255,0));
		sprintf(buf,"%d",i);
		m_wIn[i].SetWindowText(buf);
		m_wIn[i].SetMouseDownStatus(1);
		m_wIn[i].ShowWindow(SW_HIDE);
	}
	m_wLinkButton=new CxSkinButton;
	m_wLinkButton->SubclassDlgItem(IDC_BUTTON_LINK,this);
	m_wLinkButton->SetSkin(IDB_LINKN,IDB_LINKD,0,0,0,IDB_LINKM,0,0,0);
	m_wLinkButton->MoveWindow(410,3,90,44);
	m_wLinkButton->ShowWindow(SW_HIDE);
	
	//Motion Program Control
	
	UINT nIDs[8]=
	{
		IDC_BTN_UI_SHOW,
			IDC_BTN_ALL_HOME,
			IDC_BTN_RESET
	};
	
	UINT nBTNIDs[9]=
	{  //normal,down,over,disable,focus,mask.draw mode,border,margin.
		IDB_IO_BTN ,IDB_IO_BTN_CLICK,IDB_IO_BTN  ,IDB_IO_BTN,0,IDB_IO_BTN_MASK,0,0,0
	};	
	
	CString str[3]={"Motion/IO","All Home","Reset"};
	char *pBtnToolTip[3]=
	{
		"Motion/IO Control",
			"All Motor Origin Move",
			"All Motor/IO Reset"
	};	
	
	for(i=0; i<3; i++)
	{
	/*
	if(i<4)
	m_cBtnRect[i].SetRect(25+i*85,m_nBtnYoffset,105+i*85,m_nBtnYoffset+40);
	else
	m_cBtnRect[i].SetRect(25+(i-4)*85,m_nBtnYoffset+42,105+(i-4)*85,m_nBtnYoffset+84);
		*/
		
		m_btnMotion[i].SubclassDlgItem(nIDs[i],this);
		m_btnMotion[i].SetToolTipText(pBtnToolTip[i]);
		m_btnMotion[i].SetSkin(nBTNIDs[0],nBTNIDs[1],nBTNIDs[2],
			nBTNIDs[3],nBTNIDs[4],nBTNIDs[5],
			nBTNIDs[6],nBTNIDs[7],nBTNIDs[8]);
		//m_btnMotion[i].MoveWindow(&m_cBtnRect[i]);
		m_btnMotion[i].SetWindowText(str[i]);
	}
	
	UINT nIDAPC[2]=
	{
		IDC_BTN_APC_LEFT,
			IDC_BTN_APC_RIGHT
	};
	
	UINT nIDBIDAPC[9]=
	{  //normal,down,over,disable,focus,mask.draw mode,border,margin.
		IDB_IO_APC_BTN ,IDB_IO_APC_BTN_CLICKED,IDB_IO_APC_BTN  ,IDB_IO_APC_BTN,0,IDB_IO_APC_BTN_MASK,0,0,0
	};	
	
	CString strAPCSelect[2]={"LEFT","RIGHT"};
	char *pBtnAPCToolTip[2]= {	"APC LEFT",	"APC RIGHT"};	
	
	for(i=0; i<2; i++)
	{
	/*
	if(i<4)
	m_cBtnRect[i].SetRect(25+i*85,m_nBtnYoffset,105+i*85,m_nBtnYoffset+40);
	else
	m_cBtnRect[i].SetRect(25+(i-4)*85,m_nBtnYoffset+42,105+(i-4)*85,m_nBtnYoffset+84);
		*/
		
		m_btnAPC[i].SubclassDlgItem(nIDAPC[i],this);
		m_btnAPC[i].SetToolTipText(pBtnAPCToolTip[i]);
		m_btnAPC[i].SetSkin(nIDBIDAPC[0],nIDBIDAPC[1],nIDBIDAPC[2],
			nIDBIDAPC[3],nIDBIDAPC[4],nIDBIDAPC[5],
			nIDBIDAPC[6],nIDBIDAPC[7],nIDBIDAPC[8]);
		//m_btnMotion[i].MoveWindow(&m_cBtnRect[i]);
		m_btnAPC[i].SetWindowText(strAPCSelect[i]);
	}

	UINT nIDAPC_POWER[2]=
	{
		IDC_BTN_APC_UP,
			IDC_BTN_APC_DOWN
	};
	
	UINT nIDBIDAPC_POWER[9]=
	{  //normal,down,over,disable,focus,mask.draw mode,border,margin.
		IDB_IO_APC_POWER_BTN ,IDB_IO_APC_BTN_CLICKED,IDB_IO_APC_POWER_BTN  ,IDB_IO_APC_POWER_BTN,0,IDB_IO_APC_BTN_MASK,0,0,0
	};	
	
	CString strAPCSelect_POWER[2]={"APC +","APC -"};
	char *pBtnAPCToolTip_POWER[2]= {	"APC Power Quantity Up",	"APC Power Quantity Down"};	
	
	for(i=0; i<2; i++)
	{
	/*
	if(i<4)
	m_cBtnRect[i].SetRect(25+i*85,m_nBtnYoffset,105+i*85,m_nBtnYoffset+40);
	else
	m_cBtnRect[i].SetRect(25+(i-4)*85,m_nBtnYoffset+42,105+(i-4)*85,m_nBtnYoffset+84);
		*/
		
		m_btnAPC_POWER[i].SubclassDlgItem(nIDAPC_POWER[i],this);
		m_btnAPC_POWER[i].SetToolTipText(pBtnAPCToolTip_POWER[i]);
		m_btnAPC_POWER[i].SetSkin(nIDBIDAPC_POWER[0],nIDBIDAPC_POWER[1],nIDBIDAPC_POWER[2],
			nIDBIDAPC_POWER[3],nIDBIDAPC_POWER[4],nIDBIDAPC_POWER[5],
			nIDBIDAPC_POWER[6],nIDBIDAPC_POWER[7],nIDBIDAPC_POWER[8]);
		//m_btnMotion[i].MoveWindow(&m_cBtnRect[i]);
		m_btnAPC_POWER[i].SetWindowText(strAPCSelect_POWER[i]);
	}
}

void CIoWindow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);
}

void CIoWindow::OpenNetwork()
{	
	
}

void CIoWindow::CloseNetwork()
{
	
}
/*******************************************
실행 출력 
********************************************/
void CIoWindow::ResetAll()
{
	
}

void CIoWindow::OutAll()
{
	
}

void CIoWindow::CheckInput()
{
	
}

void CIoWindow::GetInputStatus(BOOL *pOut)
{
	memcpy(pOut,m_nInputStatus,sizeof(BOOL)*16);
}

/***********************************************
쓰레드 루틴을 위해 사용한다. 
************************************************/
void CIoWindow::GetIo(BOOL *pOut)
{
	CheckInput();
	GetInputStatus(pOut);
}

void CIoWindow::OutPort(int port_num,int on_off)
{
	
}


void CIoWindow::OnButtonLink() 
{
	if(m_wLinkButton->GetCheck()) 
	{
		OpenNetwork(); 
		ResetAll();		
	}
	else                          
	{ 
		ResetAll();
		CloseNetwork(); 
	}
}

/////////////////////////////////////////////////////////////////
// 출력 테스트 루틴 
/////////////////////////////////////////////////////////////////
void CIoWindow::OnIoOut0()  { if(m_wOut[0].GetCheck())   OutPort(0,1);   else 	 OutPort(0,0);  }
void CIoWindow::OnIoOut1()  { if(m_wOut[1].GetCheck())   OutPort(1,1);   else	 OutPort(1,0);  }
void CIoWindow::OnIoOut2()  { if(m_wOut[2].GetCheck())   OutPort(2,1);   else	 OutPort(2,0);  }
void CIoWindow::OnIoOut3()  { if(m_wOut[3].GetCheck())   OutPort(3,1);   else	 OutPort(3,0);  }
void CIoWindow::OnIoOut4()  { if(m_wOut[4].GetCheck())   OutPort(4,1);   else	 OutPort(4,0);  }
void CIoWindow::OnIoOut5()  { if(m_wOut[5].GetCheck())   OutPort(5,1);   else	 OutPort(5,0);  }
void CIoWindow::OnIoOut6()  { if(m_wOut[6].GetCheck())   OutPort(6,1);   else	 OutPort(6,0);  }
void CIoWindow::OnIoOut7()  { if(m_wOut[7].GetCheck())   OutPort(7,1);   else	 OutPort(7,0);  }
void CIoWindow::OnIoOut8()  { if(m_wOut[8].GetCheck())   OutPort(8,1);   else	 OutPort(8,0);  }
void CIoWindow::OnIoOut9()  { if(m_wOut[9].GetCheck())   OutPort(9,1);   else	 OutPort(9,0);  }
void CIoWindow::OnIoOut10() { if(m_wOut[10].GetCheck())  OutPort(10,1);  else	 OutPort(10,0); }
void CIoWindow::OnIoOut11() { if(m_wOut[11].GetCheck())  OutPort(11,1);  else	 OutPort(11,0); }
void CIoWindow::OnIoOut12() { if(m_wOut[12].GetCheck())  OutPort(12,1);  else	 OutPort(12,0); }
void CIoWindow::OnIoOut13() { if(m_wOut[13].GetCheck())  OutPort(13,1);  else	 OutPort(13,0); }
void CIoWindow::OnIoOut14() { if(m_wOut[14].GetCheck())  OutPort(14,1);  else	 OutPort(14,0); }
void CIoWindow::OnIoOut15() { if(m_wOut[15].GetCheck())  OutPort(15,1);  else	 OutPort(15,0); }




void CIoWindow::ButtonOuput(int nPort,int status)
{
	if(status) { m_wOut[nPort].SetCheck(1);	OutPort(nPort,1);	}
	else 	   { m_wOut[nPort].SetCheck(0);	OutPort(nPort,0);	}
	return;
}


void CIoWindow::UVSol(BOOL status)
{
	if(status) { m_wOut[COutputItem::UV_SOL].SetCheck(1);	OutPort(COutputItem::UV_SOL,1);	}
	else 	   { m_wOut[COutputItem::UV_SOL].SetCheck(0);	OutPort(COutputItem::UV_SOL,0);	}
	return ;
}



////////////////////////////////////////////////////////
// switch output

void CIoWindow::UVStart()
{
	m_wOut[COutputItem::UV_START].SetCheck(1);
	OutPort(COutputItem::UV_START,1);
	::Sleep(100);
	m_wOut[COutputItem::UV_START].SetCheck(0);
	OutPort(COutputItem::UV_START,0);
	
	::Sleep(100);
}

//////////////////////////////////////////////////////////
// UV
//////////////////////////////////////////////////////////



void CIoWindow::OnBtnUiShow() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	m_pFrame->OnSendData(MOTION_UI_SHOW);	
}

void CIoWindow::OnBtnAllHome() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	m_pFrame->OnSendData(MOTION_ALL_HOME);
}

void CIoWindow::OnBtnReset() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	m_pFrame->OnSendData(MOTION_RESET);
}

void CIoWindow::OnBtnApcLeft() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	m_pFrame->OnSendData(MOTION_APC_LEFT);
}

void CIoWindow::OnBtnApcRight() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	m_pFrame->OnSendData(MOTION_APC_RIGHT);
}

void CIoWindow::OnBtnApcUp() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	m_pFrame->OnSendData(MOTION_APC_POWER_UP);
}

void CIoWindow::OnBtnApcDown() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	m_pFrame->OnSendData(MOTION_APC_POWER_DOWN);
}
