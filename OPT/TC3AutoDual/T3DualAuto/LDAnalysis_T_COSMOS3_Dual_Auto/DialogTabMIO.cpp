// DialogTabMIO.cpp : implementation file
//

#include "stdafx.h"
#include "hvsystem.h"
#include "DialogTabMIO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogTabMIO dialog


CDialogTabMIO::CDialogTabMIO(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTabMIO::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogTabMIO)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pLabelSkinTitle = NULL;
	m_nCurModuleIndex = IO_MODULE_FIRST;
	int nI;
	
	for(nI = 0; nI < MAX_IO_INDEX_NUM; nI++)
	{
		m_pLabelIOInput[nI] = NULL;
		m_pLabelIOOutput[nI] = NULL;
		m_pcTitleInput[nI] = NULL;
	}
}


void CDialogTabMIO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTabMIO)	
	DDX_Control(pDX, IDC_COMBO1, m_cbChangeIOModule);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogTabMIO, CDialog)
	//{{AFX_MSG_MAP(CDialogTabMIO)
	ON_WM_DESTROY()
	ON_WM_PAINT()
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
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogTabMIO message handlers

BOOL CDialogTabMIO::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//UI
	InitLoadIOName();
	InitDlgSkin();
	InitTitleName();
	InitIOButton();		
	InitIOModuleCombo();
	SetTimer(IO_MODULE_INPUT, 100, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogTabMIO::InitLoadIOName()
{
	CString csIndexInput[MAX_IO_MODULE_NUM], csIndexOutput[MAX_IO_MODULE_NUM], csInputNum[MAX_IO_INDEX_NUM], csOutputNum[MAX_IO_INDEX_NUM];
	int nI, nJ;
	
	m_SysData.GetPara("IO", "IOIndexName.ini", "IO_INDEX", "IO_INDEX_NUM",PARA_TYPE_INT,NULL, &m_IODataSetting.nMaxIOModuleNum,NULL);

	for(nI = 0; nI < m_IODataSetting.nMaxIOModuleNum; nI++)
	{
		csIndexInput[nI].Format("INPUT_INDEX%d",nI);
		csIndexOutput[nI].Format("OUTPUT_INDEX%d",nI);
		for(nJ = 0; nJ < MAX_IO_INDEX_NUM; nJ++)
		{
			csInputNum[nJ].Format("INPUT_%d",nJ);
			csOutputNum[nJ].Format("OUTPUT_%d",nJ);
			m_SysData.GetStringPara("IO", "IOIndexName.ini", csIndexInput[nI], csInputNum[nJ], &m_IODataSetting.csIONameInput[nI][nJ]);
			m_SysData.GetStringPara("IO", "IOIndexName.ini", csIndexOutput[nI], csOutputNum[nJ], &m_IODataSetting.csIONameOutput[nI][nJ]);
		}		
	}
}

void CDialogTabMIO::InitTitleName()
{	
	char *cTitle = {"IO Module"};
	m_pLabelSkinTitle = new CStaticSkin;	
	m_pLabelSkinTitle->SetBmpFile(".\\guiimage\\Motion_IO_Window_IO_TITLE.bmp");
	m_pLabelSkinTitle->SubclassDlgItem(IDC_LABEL_IO,this);	
	m_pLabelSkinTitle->OutTextXY(130,6,cTitle);
}

void CDialogTabMIO::SetIOName(int nCurModuleIndex, BOOL bInit)
{

	//IO Module Input;
	int nI;
	m_nCurModuleIndex = nCurModuleIndex;
	if(!bInit)
	{
		for(nI = 0; nI < MAX_IO_INDEX_NUM; nI++)
		{				
			m_pcTitleInput[nI] = LPSTR(LPCTSTR(m_IODataSetting.csIONameInput[nCurModuleIndex][nI]));
			m_pLabelIOInput[nI]->OutTextXY(6,2,m_pcTitleInput[nI]);	
			m_pcTitleOutput[nI] = LPSTR(LPCTSTR(m_IODataSetting.csIONameOutput[nCurModuleIndex][nI]));
			m_pLabelIOOutput[nI]->OutTextXY(6,2,m_pcTitleOutput[nI]);		
		}
		return;
	}

	UINT nSInputLabelIDs[MAX_IO_INDEX_NUM]=
	{
		IDC_LABEL_INPUT0,IDC_LABEL_INPUT1,IDC_LABEL_INPUT2,IDC_LABEL_INPUT3,
		IDC_LABEL_INPUT4,IDC_LABEL_INPUT5,IDC_LABEL_INPUT6,IDC_LABEL_INPUT7,
		IDC_LABEL_INPUT8,IDC_LABEL_INPUT9,IDC_LABEL_INPUT10,IDC_LABEL_INPUT11,
		IDC_LABEL_INPUT12,IDC_LABEL_INPUT13,IDC_LABEL_INPUT14,IDC_LABEL_INPUT15
	};

	for(nI = 0; nI < MAX_IO_INDEX_NUM; nI++)
	{			
		m_pcTitleInput[nI] = LPSTR(LPCTSTR(m_IODataSetting.csIONameInput[nCurModuleIndex][nI]));
		m_pLabelIOInput[nI] = new CStaticSkin;	
		m_pLabelIOInput[nI]->SetBmpFile(".\\guiimage\\Motion_IO_Window_INPUTNAME.bmp");
		m_pLabelIOInput[nI]->SubclassDlgItem(nSInputLabelIDs[nI],this);	
		m_pLabelIOInput[nI]->OutTextXY(6,2,m_pcTitleInput[nI]);		
	}

	//IO Module Output	
	UINT nSOutputLabelIDs[MAX_IO_INDEX_NUM]=
	{
		IDC_LABEL_OUTPUT0,IDC_LABEL_OUTPUT1,IDC_LABEL_OUTPUT2,IDC_LABEL_OUTPUT3,
		IDC_LABEL_OUTPUT4,IDC_LABEL_OUTPUT5,IDC_LABEL_OUTPUT6,IDC_LABEL_OUTPUT7,
		IDC_LABEL_OUTPUT8,IDC_LABEL_OUTPUT9,IDC_LABEL_OUTPUT10,IDC_LABEL_OUTPUT11,
		IDC_LABEL_OUTPUT12,IDC_LABEL_OUTPUT13,IDC_LABEL_OUTPUT14,IDC_LABEL_OUTPUT15
	};

	for(nI = 0; nI < MAX_IO_INDEX_NUM; nI++)
	{			
		m_pcTitleOutput[nI] = LPSTR(LPCTSTR(m_IODataSetting.csIONameOutput[nCurModuleIndex][nI]));
		m_pLabelIOOutput[nI] = new CStaticSkin;	
		m_pLabelIOOutput[nI]->SetBmpFile(".\\guiimage\\Motion_IO_Window_INPUTNAME.bmp");
		m_pLabelIOOutput[nI]->SubclassDlgItem(nSOutputLabelIDs[nI],this);	
		m_pLabelIOOutput[nI]->OutTextXY(6,2,m_pcTitleOutput[nI]);		
	}

}	

void CDialogTabMIO::InitIOModuleCombo()
{
	int nI;
	int nJ;
	CString csText;
	
	for(nI = 0; nI < m_IODataSetting.nMaxIOModuleNum; nI++)
	{
		for(nJ = 0; nJ < MAX_IO_INDEX_NUM; nJ++)
		{
			m_IODataSetting.bIOStatusInput[nI][nJ] = FALSE;
			m_IODataSetting.bIOStatusOutput[nI][nJ] = FALSE;
		}
	}

	for(nI = 0; nI < m_IODataSetting.nMaxIOModuleNum; nI++)
	{
		csText.Format("IO Module Number %d", nI);
		m_cbChangeIOModule.AddString(csText);		
	}
	m_cbChangeIOModule.SetCurSel(IO_MODULE_FIRST);
	SetIOName(IO_MODULE_FIRST, TRUE);

	for(nI = 0; nI < MAX_IO_INDEX_NUM; nI++)
	{
		SetIOStatus(IO_MODULE_INPUT, nI);
		SetIOStatus(IO_MODULE_OUTINIT, nI);
	}
}

void CDialogTabMIO::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here	 
	int nI;
	SetIOName(m_cbChangeIOModule.GetCurSel(), FALSE);
	for(nI = 0; nI < MAX_IO_INDEX_NUM; nI++)
	{
		SetIOStatus(IO_MODULE_INPUT, nI);
		SetIOStatus(IO_MODULE_OUTINIT, nI);
	}	
}

void CDialogTabMIO::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
	if(m_pLabelSkinTitle!=NULL) delete m_pLabelSkinTitle;

	int nI;
	for(nI = 0; nI < MAX_IO_INDEX_NUM; nI++)
	{		
		if(m_pLabelIOInput[nI] != NULL) delete m_pLabelIOInput[nI];
		if(m_pLabelIOOutput[nI] != NULL) delete m_pLabelIOOutput[nI];	
	}
		
	delete pBmpRgn;
}

void CDialogTabMIO::InitDlgSkin()
{
	pBmpRgn = new CBmpRgn(this);
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\Motion_IO_Window_IO.bmp", 0x00ff00ff);
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
	m_nWindowHeight= pBmpRgn->GetBitmapHeight();
}

void CDialogTabMIO::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDialogTabMIO::InitIOButton()
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
	char buf[256];
	for(i=0;i<16; i++)
	{
		m_wOut[i].SubclassDlgItem(nOutIds[i], this);		
		m_wOut[i].SetSkin(IDB_IOOFF,IDB_IOON,IDB_IOOFF,0,0,IDB_IOMASK,0,0,0);		
		m_wOut[i].SetTextColor(RGB(0,255,0));
		sprintf(buf,"%d",i);
		m_wOut[i].SetWindowText(buf);
	}

	UINT nInIds[16]=
	{
		IDC_IO_IN0 ,IDC_IO_IN1 ,IDC_IO_IN2 ,IDC_IO_IN3,
		IDC_IO_IN4 ,IDC_IO_IN5 ,IDC_IO_IN6 ,IDC_IO_IN7,
		IDC_IO_IN8 ,IDC_IO_IN9 ,IDC_IO_IN10,IDC_IO_IN11,
		IDC_IO_IN12,IDC_IO_IN13,IDC_IO_IN14,IDC_IO_IN15
	};

	for(i=0;i<16; i++)
	{
		m_wIn[i].SubclassDlgItem(nInIds[i], this);		
		m_wIn[i].SetSkin(IDB_IOOFF,IDB_IOON,IDB_IOOFF,0,0,IDB_IOMASK,0,0,0);	
		m_wIn[i].SetTextColor(RGB(255,255,0));
		sprintf(buf,"%d",i);
		m_wIn[i].SetWindowText(buf);
		m_wIn[i].SetMouseDownStatus(1);
	}
}

void CDialogTabMIO::SetIOStatus(int nInOut, int nNum)
{	
	if(nInOut == IO_MODULE_INPUT)
	{
		//Input Status : 
		if(m_bIOStatusInputOld[m_nCurModuleIndex][nNum] != m_IODataSetting.bIOStatusInput[m_nCurModuleIndex][nNum])
		{
			m_bIOStatusInputOld[m_nCurModuleIndex][nNum] = m_IODataSetting.bIOStatusInput[m_nCurModuleIndex][nNum];
			m_wIn[nNum].SetCheck(m_IODataSetting.bIOStatusInput[m_nCurModuleIndex][nNum]);	
		}
	}
	else
	{
		if(nInOut == IO_MODULE_OUTINIT)
		{
			//Output Status
			if(m_IODataSetting.bIOStatusOutput[m_nCurModuleIndex][nNum])
			{
				m_IODataSetting.bIOStatusOutput[m_nCurModuleIndex][nNum] = TRUE;			
			}
			else
			{
				m_IODataSetting.bIOStatusOutput[m_nCurModuleIndex][nNum] = FALSE;			
			}	
		}
		else if(nInOut == IO_MODULE_OUTPUT)
		{
			if(m_IODataSetting.bIOStatusOutput[m_nCurModuleIndex][nNum])
			{
				m_IODataSetting.bIOStatusOutput[m_nCurModuleIndex][nNum] = FALSE;			
			}
			else
			{
				m_IODataSetting.bIOStatusOutput[m_nCurModuleIndex][nNum] = TRUE;			
			}	
		}
			
		if(m_bIOStatusOutputOld[m_nCurModuleIndex][nNum] != m_IODataSetting.bIOStatusOutput[m_nCurModuleIndex][nNum])
		{
			m_bIOStatusOutputOld[m_nCurModuleIndex][nNum] = m_IODataSetting.bIOStatusOutput[m_nCurModuleIndex][nNum];
			m_wOut[nNum].SetCheck(m_IODataSetting.bIOStatusOutput[m_nCurModuleIndex][nNum]);
		}		
	}		
}

void CDialogTabMIO::OnIoOut0() 
{
	// TODO: Add your control notification handler code here
	SetIOStatus(IO_MODULE_OUTPUT, 0);
}

void CDialogTabMIO::OnIoOut1() 
{
	// TODO: Add your control notification handler code here
	SetIOStatus(IO_MODULE_OUTPUT, 1);
}

void CDialogTabMIO::OnIoOut2() 
{
	// TODO: Add your control notification handler code here
	SetIOStatus(IO_MODULE_OUTPUT, 2);
}

void CDialogTabMIO::OnIoOut3() 
{
	// TODO: Add your control notification handler code here
	SetIOStatus(IO_MODULE_OUTPUT, 3);
}

void CDialogTabMIO::OnIoOut4() 
{
	// TODO: Add your control notification handler code here
	SetIOStatus(IO_MODULE_OUTPUT, 4);
}

void CDialogTabMIO::OnIoOut5() 
{
	// TODO: Add your control notification handler code here
	SetIOStatus(IO_MODULE_OUTPUT, 5);
}

void CDialogTabMIO::OnIoOut6() 
{
	// TODO: Add your control notification handler code here
	SetIOStatus(IO_MODULE_OUTPUT, 6);
}

void CDialogTabMIO::OnIoOut7() 
{
	// TODO: Add your control notification handler code here
	SetIOStatus(IO_MODULE_OUTPUT, 7);
}

void CDialogTabMIO::OnIoOut8() 
{
	// TODO: Add your control notification handler code here
	SetIOStatus(IO_MODULE_OUTPUT, 8);
}

void CDialogTabMIO::OnIoOut9() 
{
	// TODO: Add your control notification handler code here
	SetIOStatus(IO_MODULE_OUTPUT, 9);
}

void CDialogTabMIO::OnIoOut10() 
{
	// TODO: Add your control notification handler code here
	SetIOStatus(IO_MODULE_OUTPUT, 10);
}

void CDialogTabMIO::OnIoOut11() 
{
	// TODO: Add your control notification handler code here
	SetIOStatus(IO_MODULE_OUTPUT, 11);
}

void CDialogTabMIO::OnIoOut12() 
{
	// TODO: Add your control notification handler code here
	SetIOStatus(IO_MODULE_OUTPUT, 12);
}

void CDialogTabMIO::OnIoOut13() 
{
	// TODO: Add your control notification handler code here
	SetIOStatus(IO_MODULE_OUTPUT, 13);
}

void CDialogTabMIO::OnIoOut14() 
{
	// TODO: Add your control notification handler code here
	SetIOStatus(IO_MODULE_OUTPUT, 14);
}

void CDialogTabMIO::OnIoOut15() 
{
	// TODO: Add your control notification handler code here
	SetIOStatus(IO_MODULE_OUTPUT, 15);
}

void CDialogTabMIO::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	int nI;
	if(nIDEvent == IO_MODULE_INPUT)
	{
		for(nI = 0; nI < MAX_IO_INDEX_NUM; nI++)
		{
			SetIOStatus(IO_MODULE_INPUT, nI);
		}
	}
	CDialog::OnTimer(nIDEvent);
}
