// CtrlWindow.cpp : implementation file
//

#include "stdafx.h"
#include "HVSystem.h"
#include "Mainfrm.h"
#include "HVSystemDoc.h"
#include "HVSystemView.h"
#include "CtrlWindow.h"
//#include "Teachwindow.h"
#include "CameraControl.h"
#include "External.h"
#include "HVSystemDefine.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCtrlWindow dialog

#define BUTTON_DISPLAY_TIME 1001


CCtrlWindow::CCtrlWindow(CWnd* pParent /*=NULL*/)
	: CDialog(CCtrlWindow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCtrlWindow)
	//}}AFX_DATA_INIT
	m_nBtnYoffset=30;
	m_nLiveButton=0;
	m_wSysMenu=NULL;
	m_wSysLoad=NULL;

	m_strBarcode.Format("");

	m_pwStBarcode=NULL;
	m_pwEdBarcode=NULL;

    m_iCrntMode = NONE_MODE ;

}


void CCtrlWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCtrlWindow)
	DDX_Text(pDX, IDC_EDIT_BCODE, m_strBarcode);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCtrlWindow, CDialog)
	//{{AFX_MSG_MAP(CCtrlWindow)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_GRAB, OnButtonGrab)
	ON_BN_CLICKED(IDC_BUTTON_LIVE, OnButtonLive)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_AUTO, OnButtonAuto)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_RSAVE, OnButtonRsave)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_SYSMENU_CONTROL,OnButtonCameraControl)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SYSMENU_CONTROL2, OnButtonSysmenuControl2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCtrlWindow message handlers

BOOL CCtrlWindow::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN) {
		switch(pMsg->wParam) {
			case VK_RETURN:
				CheckBarcode();
			case VK_ESCAPE: return true;
			default: 
				 break; 
		}
	}	
	return CDialog::PreTranslateMessage(pMsg);
}



BOOL CCtrlWindow::OnInitDialog() 
{
	CDialog::OnInitDialog();
	pBmpRgn=new CBmpRgn();

	pBmpRgn->SetParentWindow(this);
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\controlwindow1024.bmp", 0x00ff00ff);
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

	UINT nIDs[MAX_BUTTON_CNT]=
	{
		IDC_BUTTON_GRAB,
		IDC_BUTTON_LIVE,
		IDC_BUTTON_SAVE,
		IDC_BUTTON_AUTO,
		IDC_BUTTON_TEST,
		IDC_BUTTON_RESET,
		IDC_BUTTON_RSAVE,
		IDC_BUTTON_EXIT
	};
	
	UINT nBTNIDs[9]=
	{  //normal,down,over,disable,focus,mask.draw mode,border,margin.
		IDB_B80X40N  ,IDB_B80X40D  ,IDB_B80X40U  ,IDB_B80X40IM,0,IDB_B80X40M,0,0,0
	};	
	
	CString str[MAX_BUTTON_CNT]={"GRAB","LIVE","SAVE","AUTO","TEST","RESET","RSAVE","EXIT"};
	
	if(gLockKeyFlag == TRUE)
	{
		str[0] = "If you";
		str[1] = "Use This";
		str[2] = "Program,";
		str[3] = "Register";
		str[4] = "Serial";
		str[5] = "Key";
		str[6] = "->";
		str[7] = "LockKey";	
	}
	char *pBtnToolTip[8]=
	{
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		""
	};

	int i;

	for(i=0; i<MAX_BUTTON_CNT; i++)
	{
		if(i<4)
			m_cBtnRect[i].SetRect(25+i*85,m_nBtnYoffset,105+i*85,m_nBtnYoffset+40);
		else
			m_cBtnRect[i].SetRect(25+(i-4)*85,m_nBtnYoffset+42,105+(i-4)*85,m_nBtnYoffset+84);

		m_wCtrlButton[i].SubclassDlgItem(nIDs[i],this);
		m_wCtrlButton[i].SetToolTipText(pBtnToolTip[i]);
		m_wCtrlButton[i].SetSkin(nBTNIDs[0],nBTNIDs[1],nBTNIDs[2],
								 nBTNIDs[3],nBTNIDs[4],nBTNIDs[5],
								 nBTNIDs[6],nBTNIDs[7],nBTNIDs[8]);
		m_wCtrlButton[i].MoveWindow(&m_cBtnRect[i]);
		m_wCtrlButton[i].SetWindowText(str[i]);
	}
/////////////////////////////////////////////////////////////////
	m_wSysMenu=new CxSkinButton;
	m_wSysMenu->SubclassDlgItem(IDC_BUTTON_SYSMENU_CONTROL,this);
	m_wSysMenu->SetSkin(IDB_TITLE_SYSMENU,IDB_TITLE_SYSMENU,IDB_TITLE_SYSMENU,0,0,IDB_TITLE_SYSMENUMASK,0,0,0);
	m_wSysMenu->MoveWindow(2,2,20,20);

	m_wSysLoad=new CxSkinButton;
	m_wSysLoad->SubclassDlgItem(IDC_BUTTON_SYSMENU_CONTROL2,this);
	m_wSysLoad->SetSkin(IDB_TITLE_SYSMENU,IDB_TITLE_SYSMENU,IDB_TITLE_SYSMENU,0,0,IDB_TITLE_SYSMENUMASK,0,0,0);
	m_wSysLoad->MoveWindow(22,2,20,20);

/////////////////////////////////////////////////////////////////
//   default setting .....
	MakeGui();

//////////////////////////////////////////////////////////////////


    SetTimer(BUTTON_DISPLAY_TIME,500,NULL);

	return TRUE;
}
void CCtrlWindow::MakeGui()
{
	m_pwStBarcode=new CStaticSkin;
	m_pwStBarcode->SetBmpFile(".\\guiimage\\st128x26_256.bmp");
	m_pwStBarcode->SubclassDlgItem(IDC_STATIC_BCODE,this);
	m_pwStBarcode->OutTextXY(4,6,"BARCODE");
	m_pwStBarcode->MoveWindow(2,m_nBtnYoffset+98,80,26);
		
	m_pwEdBarcode=new CEdit ;
	m_pwEdBarcode->SubclassDlgItem(IDC_EDIT_BCODE,this);
	m_pwEdBarcode->MoveWindow(82,m_nBtnYoffset+98,m_wndRect.Width()-88,26);
}

void CCtrlWindow::CheckBarcode()
{
	if(m_pwEdBarcode!=NULL)
	{
		UpdateData(TRUE);
		m_pwEdBarcode->GetWindowText(m_strBarcode);
		if(m_strBarcode.GetLength()>5) // 자리수 제한 값 
		{
			m_strBarcode=_T("");
			m_pwEdBarcode->SetFocus();
		}
		UpdateData(FALSE);
	}
}

/**********************************
 draw bmp image.
**********************************/
void CCtrlWindow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);
}
/*********************************
Control Window 종료 Operation.
*********************************/
void CCtrlWindow::OnDestroy() 
{
	CDialog::OnDestroy();
    KillTimer(BUTTON_DISPLAY_TIME);


	if(m_pwStBarcode!=NULL) delete m_pwStBarcode;
	if(m_pwEdBarcode!=NULL) delete m_pwEdBarcode;
	
	if(m_wSysMenu!=NULL)	delete m_wSysMenu;
	if(m_wSysLoad!=NULL)    delete m_wSysLoad;
    delete pBmpRgn;	
}


void CCtrlWindow::OnButtonExit() 
{  
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	
	CString strTemp;
	FILE *stream;
	DWORD list[1];
	int  /*numread,*/ numwritten;
	DWORD SerialNum;
	TCHAR szDriveName[256];
	TCHAR szFileSystem[256];
	
	m_pFrame->OnSendData(MOTION_UI_CLOSE);
	
	if(gLockKeyFlag == TRUE)
	{
		GetVolumeInformation(_T("d:\\"), szDriveName, 256, &SerialNum, NULL, NULL, szFileSystem, 256);		
		
		
		if( (stream = _tfopen(_T("d:\\LockSys.system") , _T("wb") )) != NULL )
		{
			list[0] = SerialNum;
			numwritten = fwrite( list, sizeof( DWORD ), 1, stream );
			fclose( stream );
			if(numwritten != 0) AfxMessageBox(_T("OK!"));
			else AfxMessageBox(_T("Fail!"));
		}
		else 
		{
			AfxMessageBox(_T("Fail!"));
		}
		Sleep(500);
		
		m_pView->ExitProc();
	}
	else
	{
		m_pView->ExitProc();
	}
}


void CCtrlWindow::OnButtonGrab() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
  	m_pView->GrabProc();

}


void CCtrlWindow::OnButtonLive() 
{

    if(m_iCrntMode == LIVE_MODE)
	{
        SetCrntMode(NONE_MODE);

	}
	else 
	{
        SetCrntMode(LIVE_MODE);
	}
	OnTimer(BUTTON_DISPLAY_TIME) ;
}

void CCtrlWindow::OnButtonTest() 
{	

    if(m_iCrntMode == TEST_MODE)
	{
        SetCrntMode(NONE_MODE);

	}
	else 
	{
        SetCrntMode(TEST_MODE);
	}
	OnTimer(BUTTON_DISPLAY_TIME) ;
	
}
/************************************************************************
 Prototype: void OnButtonAuto() 
 Function : Auto 버튼 처리 메시지 함수  
 Parameter: none
 Return   : None
 Author   : Kim Yang Ju
 Date     : 2002.10.2
 Remark   : [check]
***************************************************************************/
void CCtrlWindow::OnButtonAuto() 
{
	if(m_iCrntMode == AUTO_MODE)
	{
        SetCrntMode(NONE_MODE);

	}
	else 
	{
        SetCrntMode(AUTO_MODE);
	}
	OnTimer(BUTTON_DISPLAY_TIME) ;
}


void CCtrlWindow::OnButtonSave() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	m_pView->RunSaveImg();


}




void CCtrlWindow::OnButtonReset() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

    m_pView->m_wImgDisp1.ResetFindFlag();
	m_pView->m_wImgDisp1.Invalidate(FALSE);
	m_pView->m_wImgDisp2.ResetFindFlag();
	m_pView->m_wImgDisp2.Invalidate(FALSE);

	m_pView->m_wDisplayComm.DeleteAllList();
	m_pView->m_wTabWindow.m_pwData->DeleteAllList();
	m_pView->ClearData();
	
	
}





void CCtrlWindow::OnButtonRsave() 
{
	//이미지 검증 할때 임시로 쓰는 것.
//	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
//	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
//	m_pView->RunLoadImg();


	CString nStr;
	CFileDialog mdlg(FALSE,NULL,"*.*",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,NULL,this);
	char buf[256];
	char pFileFirst[256],pFileSecond[256];
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	::GetCurrentDirectory(256,&buf[0]);
	if(mdlg.DoModal()==IDOK)
	{
		nStr=mdlg.GetPathName();
		sprintf(pFileFirst,"%s_1.bmp",nStr.GetBuffer(nStr.GetLength()));
		sprintf(pFileSecond,"%s_2.bmp",nStr.GetBuffer(nStr.GetLength()));
		
		m_pView->m_wImgDisp1.RunSaveBmp(pFileFirst);
		m_pView->m_wImgDisp2.RunSaveBmp(pFileSecond);

		
		::SetCurrentDirectory(&buf[0]);
	}

}
void CCtrlWindow::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if(bShow)
	{
		if(m_pwEdBarcode!=NULL)
		{
			m_pwEdBarcode->SetFocus();
			m_pwEdBarcode->ShowCaret();
		}
	}	
}


void CCtrlWindow::SetCursorToWindow()
{
	if(m_pwEdBarcode!=NULL)
	{
		m_pwEdBarcode->SetFocus();
		m_pwEdBarcode->ShowCaret();
	}
}

void CCtrlWindow::OnButtonCameraControl()
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	CCameraControl wCam;
	wCam.SetCamBD(m_pView->m_pwImi);
	wCam.DoModal();
}

void CCtrlWindow::EnableButtons(int idx,BOOL bValue)
{
	int i;

	for(i=0; i<MAX_BUTTON_CNT; i++)
	{
		if(i!=idx)
		{
			m_wCtrlButton[i].EnableWindow(bValue);
		}
	}
}

void CCtrlWindow::CheckButtons(int  idx,BOOL bValue)
{
	int i;

	for(i=0; i<MAX_BUTTON_CNT; i++)
	{
		if(i!=idx)
		{
			//m_wCtrlButton[i].SetCheck(bValue);
		}
	}
}

EN_MODE CCtrlWindow::GetCrntMode()
{
    return m_iCrntMode ;
}

void CCtrlWindow::SetCrntMode(EN_MODE iMode)
{
	if(m_iCrntMode == iMode) return ;

	g_iTestMode = tmNormal ;

	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	if(m_iCrntMode == LIVE_MODE )
	{
	    m_pView -> EndLiveThread();	
		
	}
	else if(m_iCrntMode == TEST_MODE)
	{
		m_pView -> EndTestThread();	
		
	}
	else if(m_iCrntMode == AUTO_MODE)
	{
	}
	else if(m_iCrntMode == NONE_MODE)
	{		
	}
	else 
	{
	}

    m_iCrntMode = iMode ;

	if(m_iCrntMode == LIVE_MODE )
	{
	    m_pView -> StartLiveThread();	
		
	}
	else if(m_iCrntMode == TEST_MODE)
	{
		m_pView -> StartTestThread();	
		
	}
	else if(m_iCrntMode == AUTO_MODE)
	{
	}
	else if(m_iCrntMode == NONE_MODE)
	{		
	}
	else 
	{
	}

	OnTimer(BUTTON_DISPLAY_TIME) ;//화면이 늦어서. 한번 불러줌.
}

void CCtrlWindow::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
//	SYSTEMTIME nSysTime;
//	char buf[256];

	static EN_MODE iPreCrntMode ;

	if(nIDEvent==BUTTON_DISPLAY_TIME)
	{
		if(iPreCrntMode != m_iCrntMode)
		{
			if(m_iCrntMode == LIVE_MODE )
			{
				
				EnableButtons(LIVE_BUTTON,FALSE);
				//CheckButtons (LIVE_BUTTON,FALSE);
				m_wCtrlButton[LIVE_BUTTON].EnableWindow(TRUE);
				if(!m_wCtrlButton[LIVE_BUTTON].GetCheck())m_wCtrlButton[LIVE_BUTTON].SetCheck(TRUE);
				
				
					
			}
			
			else if(m_iCrntMode == TEST_MODE)
			{
				
				EnableButtons(TEST_BUTTON,FALSE);
				//CheckButtons (TEST_BUTTON,FALSE);
				m_wCtrlButton[TEST_BUTTON].EnableWindow(TRUE);
				if(!m_wCtrlButton[TEST_BUTTON].GetCheck())m_wCtrlButton[TEST_BUTTON].SetCheck(TRUE);
				
				
			}
			
			else if(m_iCrntMode == AUTO_MODE)
			{
				
				EnableButtons(AUTO_BUTTON,FALSE);
				//CheckButtons (AUTO_BUTTON,FALSE);
				m_wCtrlButton[AUTO_BUTTON].EnableWindow(TRUE);
				if(!m_wCtrlButton[AUTO_BUTTON].GetCheck())m_wCtrlButton[AUTO_BUTTON].SetCheck    (TRUE);
				
				
			}
			else if(m_iCrntMode == NONE_MODE)
			{
				
				EnableButtons(AUTO_BUTTON,TRUE );
				//CheckButtons (AUTO_BUTTON,FALSE);
				m_wCtrlButton[AUTO_BUTTON].EnableWindow(TRUE );
				if(m_wCtrlButton[AUTO_BUTTON].GetCheck())m_wCtrlButton[AUTO_BUTTON].SetCheck    (FALSE);
				
				
				
			}
			else 
			{
				
				EnableButtons(AUTO_BUTTON,TRUE );
				//CheckButtons (AUTO_BUTTON,FALSE);
				m_wCtrlButton[AUTO_BUTTON].EnableWindow(TRUE );
				if(m_wCtrlButton[AUTO_BUTTON].GetCheck())m_wCtrlButton[AUTO_BUTTON].SetCheck    (FALSE);
				
				
			}
		}

		iPreCrntMode = m_iCrntMode ;
	}
	CDialog::OnTimer(nIDEvent);
}

void CCtrlWindow::OnButtonSysmenuControl2() 
{
	//이미지 검증 할때 임시로 쓰는 것.
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	m_pView->RunLoadImg();
	
}
