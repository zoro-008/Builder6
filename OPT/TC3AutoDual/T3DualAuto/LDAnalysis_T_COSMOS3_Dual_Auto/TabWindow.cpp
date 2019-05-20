// TabWindow.cpp : implementation file
//

#include "stdafx.h"
#include "HVSystem.h"
#include "MainFrm.h"
#include "HVSystemDoc.h"
#include "HVSystemView.h"
#include "passwdInput.h"
#include "TabWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TabWindow dialog


TabWindow::TabWindow(CWnd* pParent /*=NULL*/)
	: CDialog(TabWindow::IDD, pParent),	m_nSetupStatus(0)
{
	//{{AFX_DATA_INIT(TabWindow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_wSysMenu =NULL;
	m_pwData   =NULL;
	m_pwSetup  =NULL;
	m_pwCurrent=NULL;
	m_pwEtc=NULL;
	m_nSaveFileFlag=0;
	m_nManualOptFlag=0;
    m_iSelectedWindow=0;
	int i;
	for(i=0; i<BTN_NUM; i++)
		m_pTabButton[i]=NULL;
}


void TabWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TabWindow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TabWindow, CDialog)
	//{{AFX_MSG_MAP(TabWindow)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_TABSYSMENU, OnButtonTabsysmenu)
	ON_BN_CLICKED(IDC_BUTTON_MOTIONTAB1, OnButtonCD)
	ON_BN_CLICKED(IDC_BUTTON_MOTIONTAB2, OnButtonDL)
	ON_BN_CLICKED(IDC_BUTTON_MOTIONTAB3, OnButtonSP)
	ON_BN_CLICKED(IDC_BUTTON_MOTIONTAB4, OnButton3DModel)
	ON_BN_CLICKED(IDC_BUTTON_ABCENTER, OnButtonAbcenter)
	ON_BN_CLICKED(IDC_BUTTON_MANUALOPT, OnButtonManualopt)
	ON_BN_CLICKED(IDC_BUTTON_AMOPT, OnButtonAmopt)
	ON_BN_CLICKED(IDC_BUTTON_RTOFFSET, OnButtonETC)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TabWindow message handlers

BOOL TabWindow::PreTranslateMessage(MSG* pMsg) 
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

void TabWindow::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(m_wSysMenu!=NULL)
		delete m_wSysMenu;

	if(m_pwData!=NULL)
	{
		m_pwData->DestroyWindow();
		delete m_pwData;
	}
	
	if(m_pwSetup!=NULL)
	{
		m_pwSetup->DestroyWindow();
		delete m_pwSetup;
	}
	
	if(m_pwCurrent!=NULL)
	{
		m_pwCurrent->DestroyWindow();
		delete m_pwCurrent;
	}

	if(m_pwEtc!=NULL)
	{
		m_pwEtc->DestroyWindow();
		delete m_pwEtc;
	}

	int i;
	for(i=0; i<BTN_NUM; i++)
	{
		if(m_pTabButton[i]!=NULL)
			delete m_pTabButton[i];
	}
	delete pBmpRgn;
	
}

BOOL TabWindow::OnInitDialog() 
{
	CDialog::OnInitDialog();

	pBmpRgn = new CBmpRgn(this);
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\tabwindow.bmp", 0x00ff00ff);
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
/**************************************************************************/
	m_wSysMenu=new CxSkinButton;
	m_wSysMenu->SubclassDlgItem(IDC_BUTTON_TABSYSMENU,this);
	m_wSysMenu->SetSkin(IDB_TITLE_SYSMENU,IDB_TITLE_SYSMENUCHECK,IDB_TITLE_SYSMENU,0,0,IDB_TITLE_SYSMENUMASK,0,0,0);
	m_wSysMenu->MoveWindow(2,2,20,20);
/**************************************************************************/
	
/**************************************************************************/    
	CRect r(m_wndRect.left+40,m_wndRect.top+26,m_wndRect.left+380,m_wndRect.top+426);
    m_pwData=new CDataDisplay;
	m_pwData->SetWindowRect(r);
	m_pwData->SetAppType(m_nAppType);
	
	m_pwData->Create(IDD_DIALOG_TABDATA,this);
	m_pwData->MoveWindow(&r);
/*************************************************************************/
	m_pwSetup=new CSetupWin;
	r.left=m_wndRect.left+40;
	r.top=m_wndRect.top+26;
	r.right=m_wndRect.left+380;
	r.bottom=m_wndRect.top+426;

	MakeButton();

	m_pwSetup->SetWndRect(r);
	m_pwSetup->Create(IDD_DIALOG_TABSETUP,this);
	m_pwSetup->MoveWindow(&r);

	m_pwCurrent=new CCurrentData;
	m_pwCurrent->SetWindowRect(r);
	m_pwCurrent->Create(IDD_DIALOG_TABCDATA,this);
	m_pwCurrent->MoveWindow(&r);

	m_pwEtc=new CEtcWindow;
	m_pwEtc->SetWindowRect(r);
	m_pwEtc->Create(IDD_DIALOG_TABETC,this);
	m_pwEtc->MoveWindow(&r);
    

		
	SetTimer(1011,200,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void TabWindow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void TabWindow::MakeButton()
{
	UINT nIDs[BTN_NUM]=
	{
		IDC_BUTTON_MOTIONTAB1,
		IDC_BUTTON_MOTIONTAB2,
		IDC_BUTTON_MOTIONTAB3,
		IDC_BUTTON_MOTIONTAB4,
		IDC_BUTTON_ABCENTER,
		IDC_BUTTON_MANUALOPT,
		IDC_BUTTON_AMOPT,
		IDC_BUTTON_RTOFFSET
	};
	UINT nIDBMPs[BTN_NUM][9]=
	{
		{IDB_CDATATABN ,IDB_CDATATABD ,IDB_CDATATABN ,0,0,IDB_MOTIONTABM,0,0,0},
		{IDB_DATATABN  ,IDB_DATATABD  ,IDB_DATATABN  ,0,0,IDB_MOTIONTABM,0,0,0},
		{IDB_SETUPTABN ,IDB_SETUPTABD ,IDB_SETUPTABN ,0,0,IDB_MOTIONTABM,0,0,0},
		{IDB_3DMODELN  ,IDB_3DMODELD  ,IDB_3DMODELN  ,0,0,IDB_MOTIONTABM,0,0,0},
		{IDB_ABCENTERN ,IDB_ABCENTERD ,IDB_ABCENTERN ,0,0,IDB_MOTIONTABM,0,0,0},
		{IDB_MASN      ,IDB_MASD      ,IDB_MASN      ,0,0,IDB_MOTIONTABM,0,0,0},
		{IDB_AOP       ,IDB_MOP       ,IDB_AOP       ,0,0,IDB_MOTIONTABM,0,0,0},
		{IDB_ETCN      ,IDB_ETCD      ,IDB_ETCN      ,0,0,IDB_MOTIONTABM,0,0,0}
	};  
	CRect nRect;

	int i;
	int offset=0;
	for(i=0; i<BTN_NUM; i++)
	{
		if(i==4) offset=1;
	    nRect.SetRect(2,26+40*(i+offset),122,26+40*(i+offset+1));
		m_pTabButton[i]=new CxSkinButton;
		m_pTabButton[i]->SubclassDlgItem(nIDs[i], this);
		m_pTabButton[i]->SetToolTipText("");
		m_pTabButton[i]->SetSkin(nIDBMPs[i][0],nIDBMPs[i][1],nIDBMPs[i][2],
			                     nIDBMPs[i][3],nIDBMPs[i][4],nIDBMPs[i][5],
							     nIDBMPs[i][6],nIDBMPs[i][7],nIDBMPs[i][8]);
		m_pTabButton[i]->MoveWindow(&nRect);
	}

	m_pTabButton[6]->ShowWindow(SW_SHOW);
	m_pTabButton[7]->ShowWindow(SW_SHOW);
	return;
}
void TabWindow::OnButtonTabsysmenu() 
{
	if(m_wSysMenu->GetCheck())	
	{
		m_nSaveFileFlag=1;
	}
	else 
	{
		m_nSaveFileFlag=0;
	}
}

void TabWindow::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==1011)
	{
		m_pwCurrent->ShowWindow(SW_HIDE);
		m_pwData->ShowWindow(SW_HIDE);
		m_pwSetup->ShowWindow(SW_HIDE);
		m_pwEtc->ShowWindow(SW_HIDE);
		KillTimer(1011);
	}
	
	CDialog::OnTimer(nIDEvent);
}
void TabWindow::SelectWindow(int nWin)
{
	m_nSetupStatus=0;
	switch(nWin)
	{
	case 0: 
		m_pTabButton[0]->SetCheck(1);
		m_pTabButton[1]->SetCheck(0);
		m_pTabButton[2]->SetCheck(0);
		m_pTabButton[7]->SetCheck(0);

		m_pwCurrent->ShowWindow(SW_SHOW);
		m_pwData->ShowWindow(SW_HIDE);
		m_pwSetup->ShowWindow(SW_HIDE);
		m_pwEtc->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pTabButton[0]->SetCheck(0);
		m_pTabButton[1]->SetCheck(1);
		m_pTabButton[2]->SetCheck(0);
		m_pTabButton[7]->SetCheck(0);

		m_pwCurrent->ShowWindow(SW_HIDE);
		m_pwData->ShowWindow(SW_SHOW);
		m_pwSetup->ShowWindow(SW_HIDE);
		m_pwEtc->ShowWindow(SW_HIDE);
		break;
    case 2:
		m_pTabButton[0]->SetCheck(0);
		m_pTabButton[1]->SetCheck(0);
		m_pTabButton[2]->SetCheck(1);
		m_pTabButton[7]->SetCheck(0);

		m_pwCurrent->ShowWindow(SW_HIDE);
		m_pwData->ShowWindow(SW_HIDE);
		m_pwSetup->ShowWindow(SW_SHOW);
		m_pwEtc->ShowWindow(SW_HIDE);
		m_nSetupStatus=1;
		break;
    case 3:
		m_pTabButton[0]->SetCheck(0);
		m_pTabButton[1]->SetCheck(0);
		m_pTabButton[2]->SetCheck(0);
		m_pTabButton[7]->SetCheck(1);

		m_pwCurrent->ShowWindow(SW_HIDE);
		m_pwData->ShowWindow(SW_HIDE);
		m_pwSetup->ShowWindow(SW_HIDE);
		m_pwEtc->ShowWindow(SW_SHOW);
		break;

	}
	m_iSelectedWindow=nWin;
	return;
}

void TabWindow::OnButtonCD() 
{
	SelectWindow(0);	
}

void TabWindow::OnButtonDL() 
{
	SelectWindow(1);
}

void TabWindow::OnButtonSP() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	

	char buf[256];
	CString str;

	m_nActivePasswd=1;
	CPasswdInput *pPasswd=new CPasswdInput;
    if(pPasswd->DoModal()==IDOK)
	{
		FILE *fp;
		fp=fopen(PASSWORD_FILE,"r+t");
		if(fp!=NULL)
		{
			fscanf(fp,"%s",buf);
			fclose(fp);

			str.Format("%s",buf);
			if(str==pPasswd->m_Passwd)
			{
/////////////////////////////////////////////////////////////
				m_pView->m_wImgDisp3.ShowWindow(SW_HIDE);
				m_pView->m_wImgDisp1.ShowWindow(SW_SHOW);
//////////////////////////////////////////////////////////////

				SelectWindow(2);
				delete pPasswd;
				return;
			}
		}
	}
/////////////////////////////////////////////////////////////
	m_pView->m_wImgDisp3.ShowWindow(SW_HIDE);
	m_pView->m_wImgDisp1.ShowWindow(SW_SHOW);
//////////////////////////////////////////////////////////////

	SelectWindow(0);
	delete pPasswd;
	m_nActivePasswd=0;
}



void TabWindow::OnButton3DModel() 
{

//20101216 lwb Debug모드와 Release 모드가 같게 하기 위해 주석 처리
//#ifndef _DEBUG
//	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
//	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	
//	if(m_pTabButton[3]->GetCheck())
//	{
//		//m_pView->m_w3DModelWindow.SetBuffer(m_pView->m_scMBDOne.m_pCImg[1]);
//		m_pView->m_w3DModelWindow.SetBuffer(m_pView->m_pwImi->m_pCImg[1]);
//		m_pView->m_w3DModelWindow.ShowWindow(SW_SHOW);
//		m_pView->m_w3DModelWindow.ChangeModelWindowView(SW_SHOW);
//	}
//	else
//	{
//		m_pView->m_w3DModelWindow.ShowWindow(SW_HIDE);
//		m_pView->m_w3DModelWindow.ChangeModelWindowView(SW_HIDE);
//	}
//#endif
}

void TabWindow::OnButtonAbcenter() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	

	if(m_pTabButton[4]->GetCheck())
	{
		if(m_pView->MessageBox("Absolute center changed!!!",NULL,MB_OKCANCEL)==IDOK)
		{
			m_pwSetup->SetABSCenter();
			m_pwSetup->m_nACenterFlag=1;
		}
		else
			m_pTabButton[4]->SetCheck(0);
	}
	else
	{
			m_pwSetup->m_nACenterFlag=0;
	}
}

void TabWindow::OnButtonManualopt() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	RECT r;
	double sizeb=(double)m_pView->m_wImgDisp1.m_cSettingData.nBSize;
	double scalef;

#ifndef USE_CAMERA_1
	scalef=((double)8.91/(double)640.0)+(double)m_pView->m_wImgDisp1.m_cSettingData.nBPixel/10000.0;
#else
	scalef=((double)4.8/(double)640.0)+(double)m_pView->m_wImgDisp1.m_cSettingData.nBPixel/10000.0;
#endif
    m_pView->m_wImgDisp1.ResetFindFlag();
	m_pView->m_wImgDisp1.Invalidate(FALSE);

	sizeb/=2.0;
	if(m_pTabButton[5]->GetCheck())
	{
		r.left=(int)m_pView->m_wImgDisp1.m_nCx  - (int)(sizeb/scalef+0.5);
		r.right=(int)m_pView->m_wImgDisp1.m_nCx + (int)(sizeb/scalef+0.5);
		r.top=(int)m_pView->m_wImgDisp1.m_nCy  - (int)(sizeb/scalef+0.5);
		r.bottom=(int)m_pView->m_wImgDisp1.m_nCy + (int)(sizeb/scalef+0.5);


		m_pView->m_wImgDisp1.m_cROI.SetRectData(r);
		m_pView->m_wImgDisp1.m_nRoiSet=1;
		m_nManualOptFlag=1;
		m_pView->m_wImgDisp1.Invalidate(FALSE);
		m_pView->m_wMoveArea.ShowWindow(SW_SHOW);
	}
	else
	{
		m_pView->m_wImgDisp1.m_nRoiSet=0;
		m_nManualOptFlag=1;
		m_pView->m_wImgDisp1.Invalidate(FALSE);
		m_pView->m_wMoveArea.ShowWindow(SW_HIDE);

	}
	return;
}

void TabWindow::OnButtonAmopt() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	
	if(m_pTabButton[6]->GetCheck())
		m_pView->m_nAutoManualFlag=0;
	else
		m_pView->m_nAutoManualFlag=1;
}

void TabWindow::OnButtonETC() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();


/*

	char buf[256];
	CString str;

	m_nActivePasswd=1;
	CPasswdInput *pPasswd=new CPasswdInput;
    if(pPasswd->DoModal()==IDOK)
	{
		FILE *fp;
		fp=fopen(PASSWORD_FILE,"r+t");
		if(fp!=NULL)
		{
			fscanf(fp,"%s",buf);
			fclose(fp);

			str.Format("%s",buf);
			if(str==pPasswd->m_Passwd)
			{
/////////////////////////////////////////////////////////////
				m_pView->m_wImgDisp3.ShowWindow(SW_HIDE);
				m_pView->m_wImgDisp1.ShowWindow(SW_SHOW);
//////////////////////////////////////////////////////////////

				SelectWindow(2);
				delete pPasswd;
				return;
			}
		}
	}
/////////////////////////////////////////////////////////////
	m_pView->m_wImgDisp3.ShowWindow(SW_HIDE);
	m_pView->m_wImgDisp1.ShowWindow(SW_SHOW);
//////////////////////////////////////////////////////////////

	SelectWindow(0);
	delete pPasswd;
	m_nActivePasswd=0;	
	
*/




	if(m_pTabButton[BTN_ETC]->GetCheck())
	{


    	char buf[256];
	    CString str;

		m_nActivePasswd=1;
		CPasswdInput *pPasswd=new CPasswdInput;
		if(pPasswd->DoModal()==IDOK)
		{
			FILE *fp;
			fp=fopen(PASSWORD_FILE,"r+t");
			if(fp!=NULL)
			{
				fscanf(fp,"%s",buf);
				fclose(fp);

				str.Format("%s",buf);
				if(str==pPasswd->m_Passwd)
				{
/////////////////////////////////////////////////////////////
					m_pView->m_wImgDisp3.MoveWindow(m_pView->m_wImgDisp1.GetOrgRect());
				    m_pView->m_wImgDisp3.ShowWindow(SW_SHOW);
					m_pView->m_wImgDisp1.ShowWindow(SW_HIDE);
//////////////////////////////////////////////////////////////

					SelectWindow(3);
					delete pPasswd;
					return;
				}
			}
		}
		else 
		{
			m_pTabButton[BTN_ETC]->SetCheck(false);
			delete pPasswd;
	        m_nActivePasswd=0;
			return ;
        } 
	}
	else
	{
///////////////////////////////////////////////////////
// change window 1 Laser beam
		m_pView->m_wImgDisp3.MoveWindow(m_pView->m_wImgDisp3.GetOrgRect());
        m_pView->m_wImgDisp3.ShowWindow(SW_HIDE);
        m_pView->m_wImgDisp1.ShowWindow(SW_SHOW);
		SelectWindow(0);
///////////////////////////////////////////////////////
	}


/////////////////////////////////////////////////////////////
	m_pView->m_wImgDisp3.ShowWindow(SW_HIDE);
	m_pView->m_wImgDisp1.ShowWindow(SW_SHOW);
//////////////////////////////////////////////////////////////

	SelectWindow(0);










}

void TabWindow::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	if(bShow)
	{
		m_pwCurrent->ShowWindow(SW_SHOW);
		m_pwSetup->ShowWindow(SW_SHOW);
		m_pwEtc->ShowWindow(SW_SHOW);
		m_pwData->ShowWindow(SW_SHOW);
		SelectWindow(m_iSelectedWindow);

	}
	else
	{
		m_pwCurrent->ShowWindow(SW_HIDE);
		m_pwData->ShowWindow(SW_HIDE);
		m_pwSetup->ShowWindow(SW_HIDE);
		m_pwEtc->ShowWindow(SW_HIDE);
	}
	// TODO: Add your message handler code here
	
}
