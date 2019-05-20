// C6202AODoubleBufferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "C6202AODoubleBuffer.h"
#include "C6202AODoubleBufferDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CC6202AODoubleBufferDlg dialog

CC6202AODoubleBufferDlg::CC6202AODoubleBufferDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CC6202AODoubleBufferDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CC6202AODoubleBufferDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CC6202AODoubleBufferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CC6202AODoubleBufferDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CC6202AODoubleBufferDlg, CDialog)
	//{{AFX_MSG_MAP(CC6202AODoubleBufferDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CC6202AODoubleBufferDlg message handlers

BOOL CC6202AODoubleBufferDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CC6202AODoubleBufferDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CC6202AODoubleBufferDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CC6202AODoubleBufferDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CC6202AODoubleBufferDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	int i;

	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	for(i = 0; i < DATA_CNT; i++)
	{
//		m_data_buffer[0][i] = i * (65536/DATA_CNT);
//		m_data_buffer[1][i] = 65535 - i * (65536/DATA_CNT);
//		m_data_buffer[2][i] = 32768 * (65536/DATA_CNT);
		if(i%2)
		{
			m_data_buffer[0][i] = i * (65536/DATA_CNT);
			m_data_buffer[1][i] = 65535 - i * (65536/DATA_CNT);
			m_data_buffer[2][i] = 32768 * (65536/DATA_CNT);
		}
		else
		{
			m_data_buffer[0][i] = 65535 - i * (65536/DATA_CNT);
			m_data_buffer[1][i] = i * (65536/DATA_CNT);
			m_data_buffer[2][i] = 32768 * (65536/DATA_CNT);
		}
	}
	m_dev = Register_Card(PCI_6202, 0);
	if(m_dev < 0)
	{
		MessageBox("Register_Card Error!");
	}
	
	return 0;
}

void CC6202AODoubleBufferDlg::OnClose() 
{
	I16 err;

	if(m_dev >= 0)
	{
		err = Release_Card(m_dev);
	}
	
	CDialog::OnClose();
}

void CC6202AODoubleBufferDlg::OnBtnStart() 
{
	I16 err;
	int i,j;
	U16 BufId_0;
	U16 BufId_1;
	BOOLEAN Stopped, HalfReady;
	U32 AccessCnt;
	U16 chans[4];

 	err = AO_6202_Config(m_dev, P6202_DA_WRSRC_Int,
		P6202_DA_TRGSRC_SOFT | P6202_DA_TRGMOD_POST ,
		1,//ReTrg cont
		0,//delay1
		0,//delay2
		true);
	err = AO_ContBufferReset(m_dev);
	err = AO_ContBufferSetup(m_dev, m_data_buffer[0], DATA_CNT, &BufId_0);
	err = AO_ContBufferSetup(m_dev, m_data_buffer[1], DATA_CNT, &BufId_1);
	err = AO_AsyncDblBufferMode(m_dev ,1 );
	chans[0]=0;
	chans[1]=1;
/*	err = AO_ContWriteChannel(m_dev,
		0,//channel
		BufId_0,
		DATA_CNT,
		0,//Iterations
		8000,//CHUI
		0,//definite
		ASYNCH_OP);*/
	err = AO_ContWriteMultiChannels(m_dev,
		2,
		chans,
		BufId_0,
		DATA_CNT,
		0,//Iterations
		800000,//CHUI
		0,//definite
		ASYNCH_OP);
	
}

void CC6202AODoubleBufferDlg::OnBtnStop() 
{
	I16 err;
	U32 AccessCnt;

	err = AO_AsyncClear(m_dev , &AccessCnt, 0); 
	
}
