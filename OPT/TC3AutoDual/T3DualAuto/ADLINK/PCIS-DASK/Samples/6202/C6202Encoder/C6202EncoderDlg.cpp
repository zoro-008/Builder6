// C6202EncoderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "C6202Encoder.h"
#include "C6202EncoderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//void EPT_CallBack();

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
// CC6202EncoderDlg dialog

CC6202EncoderDlg::CC6202EncoderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CC6202EncoderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CC6202EncoderDlg)
	m_data = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	g_tmp_value = 0;
}

void CC6202EncoderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CC6202EncoderDlg)
	DDX_Text(pDX, IDC_EDIT1, m_data);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CC6202EncoderDlg, CDialog)
	//{{AFX_MSG_MAP(CC6202EncoderDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_BN_CLICKED(IDC_BTN_READ, OnBtnRead)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CC6202EncoderDlg message handlers

BOOL CC6202EncoderDlg::OnInitDialog()
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

void CC6202EncoderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CC6202EncoderDlg::OnPaint() 
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
HCURSOR CC6202EncoderDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CC6202EncoderDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	g_dev = Register_Card(PCI_6202, 0);
	if(g_dev < 0)
	{
		MessageBox("Register_Card Error!");
		return 0;
	}
	GPTC_EventCallBack(g_dev, 1, P6202_EVT_TYPE_EPT0, (U32)(EPT_CallBack));
	return 0;
}

void CC6202EncoderDlg::OnClose() 
{
	I16 err;

	if(g_dev >= 0)
	{
		GPTC_EventCallBack(g_dev, 0, P6202_EVT_TYPE_EPT0, NULL);
		err = Release_Card(g_dev);
	}
	
	CDialog::OnClose();
}

void CC6202EncoderDlg::OnBtnStart() 
{
	// TODO: Add your control notification handler code here
	I16 err;

	err = GPTC_Clear (g_dev, P6202_ENCODER0);
	err = GPTC_EventSetup(g_dev, P6202_ENCODER0, P6202_EVT_MOD_EPT,P6202_EPT_PULWIDTH_200us |
		P6202_EPT_TRGOUT_AFI | P6202_EPT_TRGOUT_CALLBACK, 100, 0);
	err = GPTC_Setup(g_dev, P6202_ENCODER0, x4_AB_Phase_Encoder, 0, 0, 0, 0);
	err = GPTC_Read (g_dev, P6202_ENCODER0, (U32*)&m_data);
	UpdateData(FALSE);
}

void CC6202EncoderDlg::OnBtnRead() 
{
	// TODO: Add your control notification handler code here
	I16 err;

	err = GPTC_Read (g_dev, P6202_ENCODER0, (U32*)&m_data);
	UpdateData(FALSE);
}

void CC6202EncoderDlg::OnBtnStop() 
{
	// TODO: Add your control notification handler code here
	I16 err;

	err = GPTC_Read (g_dev, P6202_ENCODER0, (U32*)&m_data);
	err = GPTC_Clear (g_dev, P6202_ENCODER0);
	UpdateData(FALSE);
}

void CC6202EncoderDlg::EPT_CallBack()
{
	I16 err;
	g_tmp_value = g_tmp_value ^ 0xff;
	err = DO_WritePort(g_dev, P6202_TTL0, g_tmp_value);
	return;
}
