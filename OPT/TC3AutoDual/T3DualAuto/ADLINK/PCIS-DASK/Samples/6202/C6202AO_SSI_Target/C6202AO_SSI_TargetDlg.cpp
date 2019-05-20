// C6202AOSingleBufferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "C6202AO_SSI_Target.h"
#include "C6202AO_SSI_TargetDlg.h"

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
// CC6202AOSingleBufferDlg dialog

CC6202AOSingleBufferDlg::CC6202AOSingleBufferDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CC6202AOSingleBufferDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CC6202AOSingleBufferDlg)
	m_trg_src = 0;
	m_delay_1 = 0;
	m_delay_2 = 0;
	m_iteration = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CC6202AOSingleBufferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CC6202AOSingleBufferDlg)
	DDX_CBIndex(pDX, IDC_COMBO_TRG_SRC, m_trg_src);
	DDX_Text(pDX, IDC_EDIT_DELAY_1, m_delay_1);
	DDX_Text(pDX, IDC_EDIT_DELAY_2, m_delay_2);
	DDX_CBIndex(pDX, IDC_COMBO_ITERATION, m_iteration);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CC6202AOSingleBufferDlg, CDialog)
	//{{AFX_MSG_MAP(CC6202AOSingleBufferDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CC6202AOSingleBufferDlg message handlers

BOOL CC6202AOSingleBufferDlg::OnInitDialog()
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

void CC6202AOSingleBufferDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CC6202AOSingleBufferDlg::OnPaint() 
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
HCURSOR CC6202AOSingleBufferDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CC6202AOSingleBufferDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	int i;

	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	for(i = 0; i < DATA_CNT; i++)
	{
		m_buffer[i] = i * (65536/DATA_CNT);
	}
	m_dev = Register_Card(PCI_6202, 1);
	if(m_dev < 0)
	{
		MessageBox("Register_Card Error!");
	}
	
	return 0;
}

void CC6202AOSingleBufferDlg::OnClose() 
{
	I16 err;

	if(m_dev >= 0)
	{
		err = Release_Card(m_dev);
	}
	
	CDialog::OnClose();
}

void CC6202AOSingleBufferDlg::OnBtnStart() 
{
	I16 err;
	U16 trg_src;
	U16 BufId_0;
	BOOLEAN  bStopFlag = FALSE;
	DWORD dwWriteCnt = 0;

	UpdateData();
	switch(m_trg_src)
	{
	case 0:
		trg_src = P6202_DA_TRGSRC_SSI;
		break;
	}
	err = AO_6202_Config(m_dev, P6202_DA_WRSRC_Int,
		trg_src | P6202_DA_TRGMOD_DELAY ,
		0,//ReTrg cont
		m_delay_1,//delay1
		m_delay_2,//delay2
		true);
	err = AO_ContBufferReset(m_dev);
	err = AO_ContBufferSetup(m_dev, m_buffer, DATA_CNT, &BufId_0);
	err = AO_AsyncDblBufferMode(m_dev , 0);
	err = AO_ContWriteChannel(m_dev,
			0,//ch
			BufId_0,
			DATA_CNT,
			m_iteration+1,//Iterations
			80,//CHUI
			1,//definite
			ASYNCH_OP);
	while(!bStopFlag)
	{
		AO_AsyncCheck(m_dev, &bStopFlag, &dwWriteCnt);
	}
	AO_AsyncClear(m_dev, &dwWriteCnt, 0);
	AfxMessageBox("ok");
}

