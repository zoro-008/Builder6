// HVSystemView.cpp : implementation of the CHVSystemView class
//

#include "stdafx.h"
#include "HVSystem.h"
#include "HVSystemDoc.h"
#include "HVSystemView.h"
#include "mainfrm.h"
#include "..\\include\\KThresh.h"
#include "..\\include\\KRBLob.h"
#include "..\\include\\KFitting.h"
#include "..\\include\\KProcess.h"

#include <math.h>
#include <memory.h>

#include "HVSystemDefine.h"
#include "External.h"

//#include "slide.h"
#pragma comment(lib,"KThresh.lib")
#pragma comment(lib,"KRBlob.lib")
#pragma comment(lib,"KProcess.lib")
#pragma comment(lib,"KFitting.lib")
#pragma comment(lib,"KMorph.lib")
////////////////////////////////////////////////
///////20110104 lwb 1394 IMI 카메라 사용을 위해.
//#pragma comment(lib,"mil.lib")
//#pragma comment(lib,"milmet2.lib")
//#pragma comment(lib,"milvga.lib")

//#define SCALE 5.0/455.0
#define INSPECTION_TIMERID 7001
#define SCAN_TIME       1000
#define EXIT_TIMER      1001
#define CHECK_LICENSE_TIMER 1002
#define MOTION_TIMER_MESSAGE	1003
#define MOTION_TIMER_MODE		1004

 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

long g_nLiveCount=0;
long g_nFrameCount=0,g_nFailCount1=0,g_nFailCount2=0,g_nGoodCount=0,g_nTotalCount=0;
long  g_nAutoThread=1;
long  g_nLiveGrabThread=1;
//long  g_nSingleTestThread=1;
double g_StartTime=0.0,g_EndTime=0.0,g_TotalTime=0.0;
double g_chucking_start_time=0.0,g_chucking_end_time=0.0;
double g_inout_start_time=0.0,g_inout_end_time=0.0;
double g_uv_start_time=0.0, g_uv_end_time=0.0;
///////////////////
int    g_uv_button_flag=0;
double g_uv_button_time_start=0.0;
double g_uv_button_time_end=0.0;
/**********************************************************
Thread function or timer function variables
***********************************************************/
int g_start_flag=0;
int g_fixed_flag=0;
int g_vaccum_flag=0;
int g_cure_flag =0;
int g_uvend_flag=0;
int g_inout_flag=0;
int g_chucking_flag=0;
int g_after_UV_Updown_Up_flag=0;

int g_test_flag =0;
int g_opt_flag=0;
double g_stime_out=0.0;
double g_etime_out=0.0;

/***********************************************************/

char *gDirname=".\\image\\";
int g_FileNum=11;
char *gFname[21]= 
{"1.bmp","2.bmp","3.bmp","5.bmp","6.bmp","7.bmp","8.bmp",
"9.bmp","10.bmp","11.bmp","1-1.bmp","k_1.bmp","kk.bmp","kk.bmp","kk.bmp","kk.bmp","k5.bmp","k1.bmp","k2.bmp","k10.bmp","k11.bmp"};
/////////////////////////////////////////////////////////////////////////////
// CHVSystemView

IMPLEMENT_DYNCREATE(CHVSystemView, CFormView)

BEGIN_MESSAGE_MAP(CHVSystemView, CFormView)
	//{{AFX_MSG_MAP(CHVSystemView)
	ON_WM_TIMER()
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
	ON_MESSAGE(WM_COMM_READ,OnCommMessage)
	//}}AFX_MSG_MAP
	// Standard printing commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHVSystemView construction/destruction
UINT ThreadLiveGrab(LPVOID nParam);
UINT ThreadTestGrab(LPVOID nParam);

CHVSystemView::CHVSystemView()
	: CFormView(CHVSystemView::IDD)
{
	//{{AFX_DATA_INIT(CHVSystemView)7
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here
//	int i;
	m_nInitialUpdate=0;
	m_pTestThread=NULL;
	m_pLiveThread=NULL;
	m_pSingleTestThread=NULL;
	m_nAutoTestFlag=0;
	m_nTestFlag=0;
	m_nStartFlag=0;
	m_nSaveFlag=0;
	m_nCureFlag=0;
	m_nUVEndFlag=0;
	m_nRisingEdge=0;

	m_nAutoManualFlag=1;
	gLockKeyFlag = FALSE;


	
	//	for(i=0; i<3; i++)
	//		m_nZoomOut[i]=NULL;
	
#ifdef __SAMSUNG_TSST_VERSION
	m_bCD=FALSE;
#endif

 	m_iComportOpenFlag=0;
	m_strBCode="";
	m_iBarcodeReadingFlag=0; 
	m_iSaveMode=0;

//	m_bMotionMode = FALSE;
}

CHVSystemView::~CHVSystemView()
{

}

void CHVSystemView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHVSystemView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CHVSystemView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// CHVSystemView printing

BOOL CHVSystemView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CHVSystemView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CHVSystemView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CHVSystemView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CHVSystemView diagnostics

#ifdef _DEBUG
void CHVSystemView::AssertValid() const
{
	CFormView::AssertValid();
}

void CHVSystemView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CHVSystemDoc* CHVSystemView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHVSystemDoc)));
	return (CHVSystemDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHVSystemView message handlers

/*********************************************
 Prototype: void ExitProc()
 Function : 1) 쓰레드 종료
			2) 모든 윈도우 파괴.
			3) MIL 라이브러리 종료 
			4) 프로그램 종료 
 Parameter: none
 date     : 2002.4
 Author   : 김양주 
 Remark   :  none
**********************************************/
void CHVSystemView::ExitProc()
{
//	int i;

	g_bEndding = true ;

	g_nLiveGrabThread = 0;
	g_nSingleTestThread = 0;

	KillTimer(MOTION_TIMER_MESSAGE);
	KillTimer(MOTION_TIMER_MODE);

	if(m_iComportOpenFlag==1)
	{
		m_Comm.Close(); 
	}	

	if(m_pTestThread!=NULL)
	{
		g_nAutoThread=0;
		::Sleep(1000);
		m_pTestThread=NULL;
	}
	if(m_pLiveThread!=NULL)
	{
		g_nLiveGrabThread=0;
		::Sleep(1000);
		m_pLiveThread=NULL;
	}

	if(m_pSingleTestThread!=NULL)
	{
		g_nSingleTestThread=0;
		::Sleep(1000);
		m_pSingleTestThread=NULL;
	}

	

	/////////////////////////////////////////
	///20110104 lwb 1394 IMI 사용을 위해서
	if(m_pwImi!=NULL)
	{
		m_pwImi->EndCam();
		delete m_pwImi;
	}
	//m_scMBDOne.EndMil();
	//m_scMBDOne.FreeApp();
	/////////////////////////////////////////
	
	m_wBottomBar.DestroyWindow();
	m_wTitleBar.DestroyWindow();

//  for(i=0; i<3; i++)
//	{
//		if(m_nZoomOut[i]!=NULL) delete[] m_nZoomOut[i];
//	}
	m_wImgDisp1.DestroyWindow();
	m_wImgDisp2.DestroyWindow();
	m_wImgDisp3.DestroyWindow();
	m_wHProj.DestroyWindow();
	m_wVProj.DestroyWindow();
//	m_wAngle.DestroyWindow();
//20101216 lwb Debug모드와 Release 모드가 같게 하기 위해 주석 처리
//#ifndef _DEBUG
//	m_w3DModelWindow.DestroyWindow();
//#endif
	m_wDisplayComm.DestroyWindow();
//	m_wCntWindow.DestroyWindow();
	m_wTabWindow.DestroyWindow();
	m_wCtrlWindow.DestroyWindow();
	m_wIoWindow.DestroyWindow();
	m_wMoveArea.DestroyWindow();
	m_wMotionBase.DestroyWindow();

	


	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	pMainFrame->DestroyWindow();
	CloseApp();

}

/*********************************************************
1) Make window form. (Create child windows)
2) Initialize Matrox board
3) Initialize parameters
4) Initialize calibration parameters.
**********************************************************/
void CHVSystemView::OnInitialUpdate()
{

    SetWindowText("HVSystem");
	
	CRect MainFrameRect(0,0,WINDOW_XSIZE+5,WINDOW_YSIZE-1); // Formview window 영역 
	CRect TitleBarRect(0,0,WINDOW_XSIZE-1,39);               // Title window 영역 
    CRect BottomBarRect(0,WINDOW_YSIZE-40,WINDOW_XSIZE-1,WINDOW_YSIZE-1); // bottom window 영역 
	CRect DisplayCommRect(1024,775,1279,933); // RS-232 RX/TX event disply 영역  (640,40,1023,280)
    CRect MotionControlRect(0,39,383,471);    // motion control window
	//20101217 lwb Motion Base Tab Add
	CRect MotionBaseTab(0,39,639,519);

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CFormView::OnInitialUpdate();
	pMainFrame->MoveWindow(&MainFrameRect);   // 메인 프레임 크기 조절

	//Lock Key검사
	CString strTemp;
	FILE *stream;
	DWORD LockKey[3];
	int  numread/*, numwritten*/;
	DWORD SerialNum;
	TCHAR szDriveName[256];
	TCHAR szFileSystem[256];

	GetVolumeInformation(_T("d:\\"), szDriveName, 256, &SerialNum, NULL, NULL, szFileSystem, 256);	

/*
	if( (stream = _tfopen(_T("d:\\LockSys.lockkey"), _T("rb") )) != NULL )
	{
		// Attempt to read in 25 characters 
		numread = fread( LockKey, sizeof( DWORD ), 3, stream );
		fclose( stream );
		if(numread == 0) {
			AfxMessageBox(_T("Lock Key Error!"));
			gLockKeyFlag = TRUE;
			//return;// false;
		}
	}
	else {
		AfxMessageBox(_T("Lock Key Error!"));
		gLockKeyFlag = TRUE;
		//return ;//false;
	}

	if(LockKey[0] != 5678) {
		AfxMessageBox(_T("Lock Key Error!"));
		gLockKeyFlag = TRUE;
		//return ;//false;

	}
	if(LockKey[1] != SerialNum) {
		AfxMessageBox(_T("Lock Key Error!"));
		gLockKeyFlag = TRUE;
		//return ;//false;
	}
	if(LockKey[2] != 3942) {
		AfxMessageBox(_T("Lock Key Error!"));
		gLockKeyFlag = TRUE;
		//return;// false;
	}
*/

	//LockKey
	/////////////////////////////////////////////////////////////////////////////

	m_wTitleBar.Create(IDD_TITLE_BAR,this);   // 타이틀 바 생성 및 크기조절 
	m_wTitleBar.MoveWindow(&TitleBarRect);

	m_wBottomBar.Create(IDD_BOTTOM_BAR,this); // bottom bar 생성 및 크기 조절 
	m_wBottomBar.MoveWindow(&BottomBarRect);

	m_wDisplayComm.SetWndRect(DisplayCommRect);
	m_wDisplayComm.Create(IDD_DISPLAY_COMM,this); // communication & system message window
	m_wDisplayComm.MoveWindow(&DisplayCommRect);  // construction

	m_wMoveArea.Create(IDD_DIALOG_MOVE,this);
	m_wMoveArea.MoveWindow(&DisplayCommRect);
	m_wMoveArea.ShowWindow(SW_HIDE);

//	m_wCntWindow.Create(IDD_CNTWIN,this);
//	m_wCntWindow.MoveWindow(640,412,384,108);	
	
	//---

	CRect ControlWindowRect(639,775,1023,933);   // Control Window,[ 383 X 158 ]
	m_wCtrlWindow.SetWindowRect(ControlWindowRect);
    m_wCtrlWindow.Create(IDD_CONTROL_PANEL,this);
	m_wCtrlWindow.MoveWindow(&ControlWindowRect);

	if(gLockKeyFlag == TRUE)
	{
		return;
	}

	m_wIoWindow.Create(IDD_IO_CONTROL,this);
	m_wIoWindow.MoveWindow(640,933,640,50);

	m_wOkNgWindow.Create(IDD_DIALOG_OKNG,this);
	m_wOkNgWindow.MoveWindow(0,471,383,48);	

	//20110104 lwb 1394 IMI 사용을 위해서
	m_pwImi=new CImiTech;
	m_pwImi->InitCam();

	//GrabProc();

	m_nWidth= m_pwImi->GetImgWidth(0);
	m_nHeight=m_pwImi->GetImgHeight(0);
/***************************************************
 Initialize Matrox Board and set....attach display...
****************************************************/
	//m_scMBDOne.AllocApp(); // MIL Application init -> 하나만  실행 가능 
///////////////////////////////////////////////////////////////////
// MIL 7.1 standard
///////////////////////////////////////////////////////////////////

    //m_scMBDOne.InitMil(M_SYSTEM_METEOR_II,"rs170rgb_1.dcf",M_DEV0);
//////////////////////////////////////////////////////////////////////

   	MakeDisplayWindow();

/*************************************************
 Matrox  B/D Initialize Ending...
 *************************************************/
    m_wDisplayComm.AddListData("Initialize View Window Success");
	m_nInitialUpdate=1; // window  생성 확인 ...
/////////////////////////
//	MappTimer(M_TIMER_RESET,NULL);
	OpenApp();


    m_wTabWindow.SetAppType(0);
	m_wTabWindow.SetWndRect(MotionControlRect);   
	m_wTabWindow.Create(IDD_DIALOG_TABWIN,this);
	m_wTabWindow.MoveWindow(&MotionControlRect);

	//20101217 lwb Motion 구동부 추가.
	m_wMotionBase.SetWndRect(MotionBaseTab);
	m_wMotionBase.Create(IDD_DIALOG_TAB_M_BASE, this);
	m_wMotionBase.MoveWindow(&MotionBaseTab);

	//SetTimer(SCAN_TIME,150,NULL);
	SetTimer(MOTION_TIMER_MODE, 1000, NULL);
	SetTimer(MOTION_TIMER_MESSAGE, 1000, NULL);
	
	m_wTabWindow.ShowWindow(SW_HIDE);
	m_wOkNgWindow.ShowWindow(SW_HIDE);
	m_wMotionBase.ShowWindow(SW_HIDE);
//	gMotionResultData.bModeChange = TRUE;
	//CWind
	CWnd *  Handle;	
	Handle = FindWindow(NULL,WIN_NAME);
	if(Handle == NULL)
	{
		::ShellExecute(NULL, "open", "D:\\BC6Works\\TCm3_Motion\\bin\\TCosmos3_Motion", NULL, NULL, SW_SHOWNORMAL);	
	}
	
}
/*********************************************************************
 Prototype : void MakeDisplayWindow()
 Function  : 카메라 영상 4개를 표시할 윈도우를 설정한다.
 Parameter : none
 Return    : none
 Author    : Kim Yang Ju
 Date      : 2003.1.6
 Remark    : none
**********************************************************************/
void CHVSystemView::MakeDisplayWindow()
{
	CRect cImgWndRect1;
	CRect cImgWndRect2; // 디스플레이 윈도우 영역 
	CRect cImgWndRect3; // 디스플레이 윈도우 영역 
	CRect c3DWndRect;
	CRect cHProjRect;
	CRect cVProjRect;
	CRect cAngleRect;
//	int i;
	

	cImgWndRect1.SetRect(640,39,1279,519);
	cImgWndRect2.SetRect(0,520,639,983); //영상 표시 영역 
	cImgWndRect3.SetRect(0,39,639,519);
	c3DWndRect.SetRect(0,519,639,983);
	cVProjRect.SetRect(383,39,639,519);
	cHProjRect.SetRect(640,519,1279,775);
	cAngleRect.SetRect(0,279,319,519);
	

	//m_nWidth =m_scMBDOne.GetImgWidth();
	//m_nHeight=m_scMBDOne.GetImgHeight();

	m_wImgDisp1.SetBufSize(m_nWidth,m_nHeight);
	m_wImgDisp1.Create(IDD_IMG_DISPLAY,this);  // Display window 생성 및 위치 조절 
	m_wImgDisp1.MoveWindow(&cImgWndRect1);
	m_wImgDisp1.SetOrgRect(cImgWndRect1);

	m_wImgDisp2.SetBufSize(m_nWidth,m_nHeight);
	m_wImgDisp2.Create(IDD_IMG2_DISPLAY,this);  // Display window 생성 및 위치 조절 
	m_wImgDisp2.MoveWindow(&cImgWndRect2);
	m_wImgDisp2.SetOrgRect(cImgWndRect2);

	m_wImgDisp3.SetBufSize(m_nWidth,m_nHeight);
	m_wImgDisp3.Create(IDD_IMG3_DISPLAY,this);  // Display window 생성 및 위치 조절 
	m_wImgDisp3.MoveWindow(&cImgWndRect3);
	m_wImgDisp3.SetOrgRect(cImgWndRect3);
 
//20101216 lwb Debug모드와 Release 모드가 같게 하기 위해 주석 처리
//#ifndef _DEBUG
//	m_w3DModelWindow.SetBufSize(m_nWidth,m_nHeight);
//	m_w3DModelWindow.Create(IDD_DIALOG_3DMODEL,this);
//	m_w3DModelWindow.MoveWindow(&c3DWndRect);
//	m_w3DModelWindow.ShowWindow(SW_HIDE);
//	m_w3DModelWindow.ChangeModelWindowView(SW_HIDE);
//#endif
	m_wHProj.Create(IDD_DIALOG_HPROJ,this);
	m_wHProj.MoveWindow(&cHProjRect);

	m_wVProj.Create(IDD_DIALOG_VPROJ,this);
	m_wVProj.MoveWindow(&cVProjRect);

	m_wVProj.ShowWindow(SW_HIDE);
//	m_wAngle.SetWndPos(cAngleRect);
//	m_wAngle.Create(IDD_DIALOG_ANGLE,this);
//	m_wAngle.MoveWindow(&cAngleRect);
}

/************************************************************************
 Prototype: void GrabProc()
 Function : 이미지를 버퍼에 읽어 들인다.  한프레임의 영상을 획득한다. 
 Parameter: none
 Return   : none
 Author   : Kim Yang Ju
 Date     : 2002.10.21
 Remark   : none
***************************************************************************/
void CHVSystemView::GrabProc()
{

	//20110104 lwb 1394 IMI 사용을 위해서
	//MappTimer(M_TIMER_READ,&g_StartTime);

//////////////////////////////////////////////////
//   if(m_wCtrlWindow.GetLiveButton())
//	{
//
//		EndLiveGrab();
//		m_wDisplayComm.AddListData("continuous image grab finished!");
//		m_wCtrlWindow.SetLiveButton(0);
//  }
   //////////////////////////////////////////////////
/*
#ifdef FILE_LOAD_PROCESS
	char fname[256];
	char nStr[256];
	sprintf(fname,"%s%s",gDirname,gFname[g_nFrameCount%g_FileNum]);
//	if((g_nFrameCount%g_FileNum)==0) m_scMBDOne.Grab(GRAB_FILEJPG,fname);
//	else
	m_pwImi->GetImageFromFile(fname,unsigned char *p1,int iW,int iH)



		nStr=mdlg.GetPathName();
		sprintf(pFileFirst,"%s.bmp",nStr.GetBuffer(nStr.GetLength()));
		//20110104 lwb 1394 사용을 위해
		m_pwImi->GetImageFromFile(pFileFirst,m_pwImi->m_pCImg[0],m_pwImi->m_pCImg[1],m_pwImi->m_pCImg[2]);

		sprintf(pFileFirst,"%s_0.bmp",nStr.GetBuffer(nStr.GetLength()));
		m_pwImi->GetImageFromFile(pFileFirst,m_pwImi->m_pCImg[0],m_pwImi->m_nWidth[0],m_pwImi->m_nHeight[0]);

        sprintf(pFileFirst,"%s_1.bmp",nStr.GetBuffer(nStr.GetLength()));
		m_pwImi->GetImageFromFile(pFileFirst,m_pwImi->m_pCImg[1],m_pwImi->m_nWidth[1],m_pwImi->m_nHeight[1]);

		sprintf(pFileFirst,"%s_2.bmp",nStr.GetBuffer(nStr.GetLength()));
		m_pwImi->GetImageFromFile(pFileFirst,m_pwImi->m_pCImg[2],m_pwImi->m_nWidth[2],m_pwImi->m_nHeight[2]);

		//m_scMBDOne.MilSaveBmp(pFileFirst);
		::SetCurrentDirectory(&buf[0]);



	//m_scMBDOne.Grab(GRAB_FILE,fname);



	m_wDisplayComm.AddListData(fname);
	strcpy(m_pCurFname,fname);

#else
  */
	m_pwImi->Grab();   
//	m_scMBDOne.Grab(GRAB_SINGLE,NULL);
//	m_wDisplayComm.AddListData("Single image grab finished!"); 
//#endif







   
//   m_wImgDisp3.SetBuffer(m_pwImi->m_pCImg[2]);
//   m_wImgDisp2.SetBuffer(m_pwImi->m_pCImg[0]);   
//   m_wImgDisp1.SetBuffer(m_pwImi->m_pCImg[1]);
   
   g_nFrameCount++;
}

void CHVSystemView::SetBufferAll()
{
   m_wImgDisp3.SetBuffer(m_pwImi->m_pCImg[2]);
   m_wImgDisp2.SetBuffer(m_pwImi->m_pCImg[0]);   
   m_wImgDisp1.SetBuffer(m_pwImi->m_pCImg[1]);
}


void CHVSystemView::GrabProcUnchecked()
{

		m_pwImi->Grab();
		m_wImgDisp3.SetBuffer(m_pwImi->m_pCImg[2]);
		m_wImgDisp2.SetBuffer(m_pwImi->m_pCImg[0]);		
		m_wImgDisp1.SetBuffer(m_pwImi->m_pCImg[1]);

//	MappTimer(M_TIMER_READ,&g_StartTime);
//
//
//	//////////////////////////////////////////////////
//	m_scMBDOne.BufGet(0);
//	m_scMBDOne.BufGet(1);
//	m_scMBDOne.BufGet(2);
//
//	m_wImgDisp1.SetBuffer(m_scMBDOne.m_pCImg[1]);
//    m_wImgDisp2.SetBuffer(m_scMBDOne.m_pCImg[0]);
//	m_wImgDisp3.SetBuffer(m_scMBDOne.m_pCImg[2]);

	//g_nFrameCount++;
}
/**************************************************
prototype : int CheckHWLock()
function  : HASP 하드웨어 lock key의 유무를 체크한다.
parameter : none
return    : 1 -> hw key 있음. 0 -> hw key 없음 
date      : 2003.6.
author    : 김양주 
remark    : none
***************************************************/
int CHVSystemView::CheckHWLock()
{
	return 1;
}

void CHVSystemView::SettingParam(SETTING_DATA d) 
{
	memcpy(&m_cSettingData,&d,sizeof(SETTING_DATA)); 	
	
	m_wImgDisp3.SettingParam();
	m_wImgDisp2.SettingParam();
	m_wImgDisp1.SettingParam();
//	m_wAngle.SetDir(m_cSettingData.nRDir,m_cSettingData.nTDir);
//	m_wAngle.Invalidate(FALSE);
	m_wHProj.SetParam();
	m_wHProj.Invalidate(FALSE);
	m_wVProj.SetParam();
	m_wVProj.Invalidate(FALSE);
}

void CHVSystemView::SettingParam(SETTING_DATA_ADD d) 
{
	memcpy(&m_cSettingDataAdd,&d,sizeof(SETTING_DATA_ADD)); 
}


void CHVSystemView::SettingParam(SETTING_DATAEX d) 
{
	memcpy(&m_cSettingDataEx,&d,sizeof(SETTING_DATAEX)); 
	m_wImgDisp3.SettingParam();
	m_wImgDisp2.SettingParam();
	m_wImgDisp1.SettingParam();

	m_wHProj.SetParam();
	m_wHProj.Invalidate(FALSE);
	m_wVProj.SetParam();
	m_wVProj.Invalidate(FALSE);
}

void CHVSystemView::SettingParam(SETTING_DATAEX2 d) 
{
	memcpy(&m_cSettingDataEx2,&d,sizeof(SETTING_DATAEX2)); 
	if(d.iBPort==0) m_iBarcodeReadingFlag=1;
	
	if(d.iSaveMode<0 || d.iSaveMode>2) m_cSettingDataEx2.iSaveMode=0;

	ReInitComport(d.iBPort);
}
/**********************************************************************************************
 Above : General routine.
 Below : Special routine.
***********************************************************************************************/
void CHVSystemView::OpenApp()
{
	//20110104 lwb 1394 사용을 위해
//	int nWidth,nHeight;
//   
//	nWidth=m_scMBDOne.GetImgWidth();
//	nHeight=m_scMBDOne.GetImgHeight();
}

void CHVSystemView::CloseApp()
{
	return;
}

int CHVSystemView::StartAnalyzer(int pflag , EN_TEST_MODE iTestMode)
{
	int nFlag=1;
	nFlag=m_wImgDisp3.BeamDistributionAnalysis(pflag,iTestMode);
	nFlag=m_wImgDisp2.ProcessThreeBeam(iTestMode);
	nFlag=m_wImgDisp1.BeamDistributionAnalysis(pflag);
	
	
	return nFlag;
}



/************************************************************************
 Prototype: void TestProc()
 Function : 어플리케이션을 실행한다.
            Test button을 눌렀을 경우 실행 되는  저 레벨 함수 
 Parameter: none
 Return   : none
 Author   : Kim Yang Ju
 Date     : 
 Remark   :
***************************************************************************/
int CHVSystemView::TestProc(int pflag,EN_TEST_MODE iTestMode)
{
	int nFlag=0;
//////////////////////////////////////////////
	nFlag=StartAnalyzer(pflag,iTestMode);

//20101216 lwb Debug모드와 Release 모드가 같게 하기 위해 주석 처리
// #ifdef _DEBUG
// 	SaveData();
// 	SaveServerData();
// #endif
///////////////////
	return nFlag;
}

void CHVSystemView::ClearData()
{
	g_nFrameCount=0;
	g_nGoodCount=0;
	g_nFailCount1=0;
	g_nFailCount2=0;
	g_nTotalCount=0;
	g_TotalTime=0;
	CHVSystemApp *pApp=(CHVSystemApp*)AfxGetApp();
	pApp->m_nTestCount=0;

	ResetOkNgWindow();
}


void CHVSystemView::StartLiveThread()
{
	if(m_pLiveThread!=NULL)
	{
		g_nLiveGrabThread=0;
		::Sleep(1000);
		m_pLiveThread=NULL;
		g_nLiveGrabThread=1;
		m_pLiveThread=AfxBeginThread(ThreadLiveGrab,this);
	}
	else
	{
		g_nLiveGrabThread=1;
		m_pLiveThread=AfxBeginThread(ThreadLiveGrab,this);
	}
}

void CHVSystemView::EndLiveThread()
{
	g_nLiveGrabThread=0;
	::Sleep(1000);
	m_pLiveThread=NULL;

}
void CHVSystemView::StartTestThread()
{
	if(m_pSingleTestThread!=NULL)
	{
		g_nSingleTestThread=0;
		::Sleep(100);
		m_pSingleTestThread=NULL;
		g_nSingleTestThread=1;

		m_pSingleTestThread=AfxBeginThread(ThreadTestGrab,this);
	}
	else
	{
		g_nSingleTestThread=1;
		m_pSingleTestThread=AfxBeginThread(ThreadTestGrab,this);
	}
}

void CHVSystemView::EndTestThread()
{
	g_nSingleTestThread=0;
	::Sleep(100);
	m_pSingleTestThread=NULL;
}

void CHVSystemView::OnTimer(UINT nIDEvent) 
{
//	BOOL nInput[16];
//	char buf[1024];
//	int	 nAuto, nTest;	
//	static BOOL bModeSelectedAutoOld = !gMotionResultData.bModeSelectAuto;

	switch(nIDEvent)
	{		
	case MOTION_TIMER_MESSAGE:
/*		//모션에서 온 메시지를 넣기 위해 
		
		if(gMotionResultData.bInspection == VISION_INSPECTION)break;

		if(gMotionResultData.bMessageRequest == TRUE)
		{				
			gMotionResultData.bMessageRequest = FALSE;
			
			sprintf(buf, "%s",gMotionResultData.csMessage);
			m_wDisplayComm.AddListData(buf);			
		}	
*/		
		break;		
		
	case MOTION_TIMER_MODE:
/*		//Motion Mode 버튼을 제어 하기 위해.
		
		//	gMotionResultData.bModeSelectAuto = TRUE; //Auto 버튼 눌린 상태.
		//	gMotionResultData.bModeSelectAuto = FALSE;	// Test 버튼 눌린 상태.
		if(gMotionResultData.bInspection == VISION_INSPECTION)break;

		//if(gMotionResultData.bModeChange ==	TRUE)
		if(bModeSelectedAutoOld != gMotionResultData.bModeSelectAuto)
		{
			nAuto = m_wCtrlWindow.m_wCtrlButton[AUTO_BUTTON].GetCheck();
			nTest = m_wCtrlWindow.m_wCtrlButton[4].GetCheck();
			//gMotionResultData.bModeChange =	FALSE;
			
			if(gMotionResultData.bModeSelectAuto == TRUE)
			{			
				if(!nAuto)
				{
					if(nTest)
					{	
						m_wCtrlWindow.TestMode(FALSE);
						//Sleep(500);
					}
					m_wCtrlWindow.AutoMode(TRUE);
					m_wDisplayComm.AddListData("Auto Mode");
					//Sleep(500);
				}
			}			
			else if(gMotionResultData.bModeSelectAuto == FALSE)
			{				
				if(!nTest)
				{
					if(nAuto)
					{	
						m_wCtrlWindow.AutoMode(FALSE);
						//Sleep(500);
					}
					m_wCtrlWindow.TestMode(TRUE);
					m_wDisplayComm.AddListData("Test Mode");
					//Sleep(500);
				}
			}			
		}		

		bModeSelectedAutoOld = gMotionResultData.bModeSelectAuto;	
	*/
		break;
	}
	
	
	CFormView::OnTimer(nIDEvent);
}


/************************************************************************
Prototype : void RunSaveImg()
Function  : 1) Current directory를 저장 
2) CFileDialog window construct
3) File save(JPG, BMP) & retrieve directory
 Parameters: None
 Author    : Yang Ju Kim
 Date      : 2002.
 Remark    : None
*************************************************************************/
void CHVSystemView::RunSaveImg()
{
	CString nStr;
	CFileDialog mdlg(FALSE,NULL,"*.*",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,NULL,this);
	int nFrameCount=1;
	char buf[256];
	char pFileFirst[256];

	::GetCurrentDirectory(256,&buf[0]);
	if(mdlg.DoModal()==IDOK)
	{
		nStr=mdlg.GetPathName();
		sprintf(pFileFirst,"%s.bmp",nStr.GetBuffer(nStr.GetLength()));
		//20110104 lwb 1394 사용을 위해
		m_pwImi->SaveImageToFile(pFileFirst,m_pwImi->m_pCImg[0],m_pwImi->m_pCImg[1],m_pwImi->m_pCImg[2]);

		sprintf(pFileFirst,"%s_0.bmp",nStr.GetBuffer(nStr.GetLength()));
		m_pwImi->SaveImageToFile(pFileFirst,m_pwImi->m_pCImg[0],m_pwImi->m_nWidth[0],m_pwImi->m_nHeight[0]);

        sprintf(pFileFirst,"%s_1.bmp",nStr.GetBuffer(nStr.GetLength()));
		m_pwImi->SaveImageToFile(pFileFirst,m_pwImi->m_pCImg[1],m_pwImi->m_nWidth[1],m_pwImi->m_nHeight[1]);

		sprintf(pFileFirst,"%s_2.bmp",nStr.GetBuffer(nStr.GetLength()));
		m_pwImi->SaveImageToFile(pFileFirst,m_pwImi->m_pCImg[2],m_pwImi->m_nWidth[2],m_pwImi->m_nHeight[2]);

		//m_scMBDOne.MilSaveBmp(pFileFirst);
		::SetCurrentDirectory(&buf[0]);
	}
}


void CHVSystemView::RunLoadImg()
{
	CString nStr;
	CFileDialog mdlg(TRUE,NULL,"*.*",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,NULL,this);
	int nFrameCount=1;
	char buf[256];
	char pFileFirst[256];

	::GetCurrentDirectory(256,&buf[0]);
	if(mdlg.DoModal()==IDOK)
	{
		nStr=mdlg.GetPathName();
		sprintf(pFileFirst,"%s",nStr.GetBuffer(nStr.GetLength()));
		//20110104 lwb 1394 사용을 위해
		m_pwImi->GetImageFromFile(pFileFirst,m_pwImi->m_pCImg[0],m_pwImi->m_pCImg[1],m_pwImi->m_pCImg[2]);

		//sprintf(pFileFirst,"%s_0.bmp",nStr.GetBuffer(nStr.GetLength()));
		//m_pwImi->SaveImageToFile(pFileFirst,m_pwImi->m_pCImg[0],m_pwImi->m_nWidth[0],m_pwImi->m_nHeight[0]);

        //sprintf(pFileFirst,"%s_1.bmp",nStr.GetBuffer(nStr.GetLength()));
		//m_pwImi->SaveImageToFile(pFileFirst,m_pwImi->m_pCImg[1],m_pwImi->m_nWidth[1],m_pwImi->m_nHeight[1]);

		//sprintf(pFileFirst,"%s_2.bmp",nStr.GetBuffer(nStr.GetLength()));
		//m_pwImi->SaveImageToFile(pFileFirst,m_pwImi->m_pCImg[2],m_pwImi->m_nWidth[2],m_pwImi->m_nHeight[2]);

		//m_scMBDOne.MilSaveBmp(pFileFirst);
		::SetCurrentDirectory(&buf[0]);
	}

    m_wImgDisp3.SetBuffer(m_pwImi->m_pCImg[2]);
    m_wImgDisp2.SetBuffer(m_pwImi->m_pCImg[0]);   
    m_wImgDisp1.SetBuffer(m_pwImi->m_pCImg[1]);

	m_wImgDisp3.Invalidate(FALSE);
	m_wImgDisp2.Invalidate(FALSE);					
	m_wImgDisp1.Invalidate(FALSE);


	TestProc(0,tmNormal );
}

void CHVSystemView::SaveData()
{
	int i,j;
	int count=0; 
    char buf[256]="\0";
	char *title[27]=//sun
	{
		"Time",
		"Number",
		"Angle",
		"FA R",
		"FA T",
		"3Beam Tilt",
		"Beam Size",
		"GT shift",
		"Area",
		"Peak Ratio",
		"Parallelism",
		"Absolute Center X",
		"Absolute Center Y",
		"Beam Center X",
		"Beam Center Y",
		"Peak Center X",
		"Peak Center Y",
		"Distance",
		"Beam Shift R(x)",
		"Beam Shift T(y)",
		"HRim Loc X",
		"HRim Loc Y",
		"VRim Loc X",
		"VRim Loc Y",
		"HRim Value",
		"VRim Value",
		"Error"
	};

	sprintf(buf,"%d",g_nTotalCount);
	m_wTabWindow.m_pwData->AddListData(buf,1);
	for(i=0; i<4; i++)
		for(j=0; j<m_wTabWindow.m_pwCurrent->m_nSubItemNum[i]; j++)
		{
           m_wTabWindow.m_pwData->AddListData(m_wTabWindow.m_pwCurrent->m_cBuf[i][j],count+2);
		   count++;
		}
////////////////////////////////
// Save file

    FILE *fp;
	char fname[256];
	int findflag=0;

	sprintf(fname,".\\data\\%04d년%02d월%02d일.csv",
					m_wTabWindow.m_pwData->m_nSysTime.wYear,
					m_wTabWindow.m_pwData->m_nSysTime.wMonth,
					m_wTabWindow.m_pwData->m_nSysTime.wDay);

	fp=fopen(fname,"r+t");
	if(fp!=NULL) {	findflag=1;		fclose(fp);	}
	else    	 {	findflag=0;		            }

    fp=fopen(fname,"a+t");
	if(fp==NULL) return;
	if(findflag==0)
	{
		fprintf(fp,"Serial," );
		fprintf(fp,"Table,");
		for(i=0; i<27; i++) {
			fprintf(fp,"%s",title[i]);
			if(i<26)
				fprintf(fp,",");
			else
				fprintf(fp,"\n");
		}
	}

	fprintf(fp,"%s",m_strBCode.GetBuffer(m_strBCode.GetLength()));
	fprintf(fp, g_bLeftRight ? ",Right":",Left");
	fprintf(fp,",%s",m_wTabWindow.m_pwData->m_strCurrentTime);
	fprintf(fp,",%s",buf);

	for(i=0; i<4; i++)
		for(j=0; j<m_wTabWindow.m_pwCurrent->m_nSubItemNum[i]; j++)
		{
			fprintf(fp,",%s",m_wTabWindow.m_pwCurrent->m_cBuf[i][j]);
		}

	if(m_wImgDisp1.m_nOkNgStatus==0 && m_wImgDisp2.m_nOkNgStatus==0)
	{
			fprintf(fp,",0");
	}
	else
		fprintf(fp,",1");

	fprintf(fp,"\n");
	fclose(fp);

	return;
}


void CHVSystemView::SaveSimpleData(int nTotalCount)
{
	int i,j;
	int count=0; 
    char buf[256]="\0";
	char *title[27]=
	{
		"Time",
		"Number",
		"Angle",
		"FA R",
		"FA T",
		"3Beam Tilt",
		"Beam Size",
		"GT Shift",
		"Area",
		"Peak Ratio",
		"Parallelism",
		"Absolute Center X",
		"Absolute Center Y",
		"Beam Center X",
		"Beam Center Y",
		"Peak Center X",
		"Peak Center Y",
		"Distance",
		"Beam Shift R(x)",
		"Beam Shift T(y)",
		"HRim Loc X",
		"HRim Loc Y",
		"VRim Loc X",
		"VRim Loc Y",
		"HRim Value",
		"VRim Value",
		"Error",
		
	};

	sprintf(buf,"%d",nTotalCount);
	m_wTabWindow.m_pwData->AddListData(buf,1);
	for(i=0; i<4; i++)
		for(j=0; j<m_wTabWindow.m_pwCurrent->m_nSubItemNum[i]; j++)
		{
           m_wTabWindow.m_pwData->AddListData(m_wTabWindow.m_pwCurrent->m_cBuf[i][j],count+2);
		   count++;
		}
////////////////////////////////
// Save file

    FILE *fp;
	char fname[256];
	int findflag=0;

	sprintf(fname,".\\data\\test%04d년%02d월%02d일.csv",
					m_wTabWindow.m_pwData->m_nSysTime.wYear,
					m_wTabWindow.m_pwData->m_nSysTime.wMonth,
					m_wTabWindow.m_pwData->m_nSysTime.wDay);

	fp=fopen(fname,"r+t");
	if(fp!=NULL) {	findflag=1;		fclose(fp);	}
	else    	 {	findflag=0;		            }

    fp=fopen(fname,"a+t");
	if(fp==NULL) return;
	if(findflag==0)
	{
		fprintf(fp,"Serial,");
		fprintf(fp,"Table,");
		for(i=0; i<27; i++) {
			fprintf(fp,"%s",title[i]);
			if(i<26)
				fprintf(fp,",");
			else
				fprintf(fp,"\n");
		}
	}

	fprintf(fp,"%s",m_strBCode.GetBuffer(m_strBCode.GetLength()));
	fprintf(fp, g_bLeftRight ? ",Right" : ",Left");
	fprintf(fp,",%s",m_wTabWindow.m_pwData->m_strCurrentTime);
	fprintf(fp,",%s",buf);

	for(i=0; i<4; i++)
		for(j=0; j<m_wTabWindow.m_pwCurrent->m_nSubItemNum[i]; j++)
		{
			fprintf(fp,",%s",m_wTabWindow.m_pwCurrent->m_cBuf[i][j]);
		}

	if(m_wImgDisp1.m_nOkNgStatus==0 && m_wImgDisp2.m_nOkNgStatus==0)
	{
			fprintf(fp,",0");
	}
	else
		fprintf(fp,",1");

	

	fprintf(fp,"\n");
	fclose(fp);

	return;
}

void CHVSystemView::SaveServerData()
{
//20101216 lwb 참조 되지 않은 변수는 주석처리
//	int i,j;
	int count=0; 
    char buf[256]="\0";
 
    FILE *fp;
	char fname[256];
	int index=0;

	::CreateDirectory(".\\Server",NULL);
	sprintf(fname,".\\Server\\data.ini");

    fp=fopen(fname,"w+t");
	if(fp==NULL) return;

	fprintf(fp,"[DVD]\n");
	fprintf(fp,"Angle=%.3f\n",m_wImgDisp2.m_nAngle);
 	fprintf(fp,"FA_R=%.3f\n",m_wImgDisp2.m_cAngle.x);
 	fprintf(fp,"FA_T=%.3f\n",m_wImgDisp2.m_cAngle.y);
 	fprintf(fp,"3Beam_Tilt=%.3f\n",m_wImgDisp2.m_nTilt);
	fprintf(fp,"Beam_Size=%.3f\n",m_wImgDisp1.m_nBeamDiameter);
	fprintf(fp,"GT_Shift=%.3f\n",m_wImgDisp3.m_dGTShift);
	fprintf(fp,"Area=%.3f\n",m_wImgDisp1.m_nBeamArea);
	fprintf(fp,"Peak_Ratio=%.3f\n",m_wImgDisp1.m_nPAreaRatio);
	fprintf(fp,"Parallelism=%.3f\n",m_wImgDisp1.m_nParallelism);
	fprintf(fp,"Absolute_Center_X=%.3f\n",m_wImgDisp1.m_cDistanceCoordinate[(int)m_wImgDisp1.m_nCy][(int)m_wImgDisp1.m_nCx].x);
	fprintf(fp,"Absolute_Center_Y=%.3f\n",m_wImgDisp1.m_cDistanceCoordinate[(int)m_wImgDisp1.m_nCy][(int)m_wImgDisp1.m_nCx].y);
	fprintf(fp,"Beam_Center_X=%.3f\n",m_wImgDisp1.m_nBeamPos.x);
	fprintf(fp,"Beam_Center_Y=%.3f\n",m_wImgDisp1.m_nBeamPos.y);
	fprintf(fp,"Peak_Center_X=%.3f\n",m_wImgDisp1.m_nPBeamPos.x);
	fprintf(fp,"Peak_Center_Y=%.3f\n",m_wImgDisp1.m_nPBeamPos.y);
	fprintf(fp,"Beam_Shift=%.3f\n",m_wImgDisp1.m_dDistance);
	fprintf(fp,"Beam_Shift_R=%.3f\n",m_wImgDisp1.m_cIPDist.x);
	fprintf(fp,"Beam_Shift_T=%.3f\n",m_wImgDisp1.m_cIPDist.y);


 	fprintf(fp,"[COMMON]\n");


	if(m_wImgDisp1.m_nOkNgStatus==0 && m_wImgDisp2.m_nOkNgStatus==0)
	{
			fprintf(fp,"Judge=OK\n");
	}
	else
			fprintf(fp,"Judge=NG\n");

	if(m_strBCode.GetLength() == 0 ) 
	{

		fprintf(fp,"Serial=%d-%d-%d %02d:%02d:%02d\n",m_wTabWindow.m_pwData->m_nSysTime.wYear,
		m_wTabWindow.m_pwData->m_nSysTime.wMonth,
		m_wTabWindow.m_pwData->m_nSysTime.wDay,
		m_wTabWindow.m_pwData->m_nSysTime.wHour,
		m_wTabWindow.m_pwData->m_nSysTime.wMinute,
		m_wTabWindow.m_pwData->m_nSysTime.wSecond);
	}
	else
	{
	    fprintf(fp,"Serial=%s\n",m_strBCode.GetBuffer(m_strBCode.GetLength()));
	}
	fprintf(fp,"EvalTime=%d-%d-%d %02d:%02d:%02d\n",m_wTabWindow.m_pwData->m_nSysTime.wYear,
		m_wTabWindow.m_pwData->m_nSysTime.wMonth,
		m_wTabWindow.m_pwData->m_nSysTime.wDay,
		m_wTabWindow.m_pwData->m_nSysTime.wHour,
		m_wTabWindow.m_pwData->m_nSysTime.wMinute,
		m_wTabWindow.m_pwData->m_nSysTime.wSecond);



	fclose(fp);

	return;
}


void CHVSystemView::SetServerData()
{
	gMotionResultData.dAngle             = m_wImgDisp2.m_nAngle                                                                 ;
	gMotionResultData.dFA_R              = m_wImgDisp2.m_cAngle.x                                                               ;
	gMotionResultData.dFA_T              = m_wImgDisp2.m_cAngle.y                                                               ;
	gMotionResultData.dThree_Beam_Tilt   = m_wImgDisp2.m_nTilt                                                                  ;
	gMotionResultData.dBeam_Size         = m_wImgDisp1.m_nBeamDiameter                                                          ;
	gMotionResultData.dGT_Shift          = m_wImgDisp3.m_dGTShift                                                               ;
	gMotionResultData.dArea              = m_wImgDisp1.m_nBeamArea                                                              ;
	gMotionResultData.dPeak_Ratio        = m_wImgDisp1.m_nPAreaRatio                                                            ;
	gMotionResultData.dParallelism       = m_wImgDisp1.m_nParallelism                                                           ;
	gMotionResultData.dAbsolute_Center_X = m_wImgDisp1.m_cDistanceCoordinate[(int)m_wImgDisp1.m_nCy][(int)m_wImgDisp1.m_nCx].x  ;
	gMotionResultData.dAbsolute_Center_Y = m_wImgDisp1.m_cDistanceCoordinate[(int)m_wImgDisp1.m_nCy][(int)m_wImgDisp1.m_nCx].y  ;
	gMotionResultData.dBeam_Center_X     = m_wImgDisp1.m_nBeamPos.x                                                             ;
	gMotionResultData.dBeam_Center_Y     = m_wImgDisp1.m_nBeamPos.y                                                             ;
	gMotionResultData.dPeak_Center_X     = m_wImgDisp1.m_nPBeamPos.x                                                            ;
	gMotionResultData.dPeak_Center_Y     = m_wImgDisp1.m_nPBeamPos.y                                                            ;
	gMotionResultData.dBeam_Shift        = m_wImgDisp1.m_dDistance                                                              ;
	gMotionResultData.dBeam_Shift_R      = m_wImgDisp1.m_cIPDist.x                                                              ;
	gMotionResultData.dBeam_Shift_T      = m_wImgDisp1.m_cIPDist.y                                                              ;

	//gMotionResultData.sEvalTime.Format("%d-%d-%d %02d:%02d:%02d",m_wTabWindow.m_pwData->m_nSysTime.wYear,
	//	                                                     m_wTabWindow.m_pwData->m_nSysTime.wMonth,
	//	                                                     m_wTabWindow.m_pwData->m_nSysTime.wDay,
//		                                                     m_wTabWindow.m_pwData->m_nSysTime.wHour,
//		                                                     m_wTabWindow.m_pwData->m_nSysTime.wMinute,
//		                                                     m_wTabWindow.m_pwData->m_nSysTime.wSecond);
}

void CHVSystemView::Inspection(EN_TEST_MODE iTestMode)
{
	//검사 도중에 다시 검사가 들어올일은 없지만 확인차.
	//char buf[256];
	static bool bInsp = FALSE ;
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	if(bInsp) {m_wDisplayComm.AddListData("Insp from Eqip while Insp"); return ;}
	bInsp = true ;

	//모션쪽 결과값 클리어.
	memset(&gMotionResultData , 0 , sizeof(MOTION_RESULT_DATA));
	gMotionResultData.nAngleOkNg   = VISION_NG;
	gMotionResultData.nInlineROkNg = VISION_NG;	
	gMotionResultData.nGauVirOkNg  = VISION_NG;
	gMotionResultData.nGauHorOkNg  = VISION_NG;
	gMotionResultData.nBeamOkNg    = VISION_NG;   

	//사진 찍고.
    GrabProc();

	//이미지 담아 오기. 
	SetBufferAll();

	//검사
	TestProc(0,iTestMode );
	
	m_wImgDisp3.Invalidate(FALSE);
	m_wImgDisp2.Invalidate(FALSE);					
	m_wImgDisp1.Invalidate(FALSE);
	
	//서버에 보내는 데이터 세팅.
	SetServerData();

	bInsp = false ;
}

void CHVSystemView::ResetOkNgWindow()
{
	m_wOkNgWindow.m_pButton[0].SetCheck(1-m_cSettingData.nASt);
	m_wOkNgWindow.m_pButton[1].SetCheck(1-m_cSettingData.nLSt);
	m_wOkNgWindow.m_pButton[2].SetCheck(1-m_cSettingData.nPSt);
	m_wOkNgWindow.m_pButton[3].SetCheck(1-m_cSettingData.nHRSt);
	m_wOkNgWindow.m_pButton[4].SetCheck(1-m_cSettingData.nDSt);
	m_wOkNgWindow.m_pButton[5].SetCheck(1-m_cSettingData.nTSt);
	m_wOkNgWindow.m_pButton[6].SetCheck(1-m_cSettingData.nHSt);
	m_wOkNgWindow.m_pButton[3].SetCheck(1-m_cSettingData.nVRSt);

	m_wImgDisp3.ResetOkNgFlag();
	m_wImgDisp1.ResetOkNgFlag();
	m_wImgDisp2.ResetOkNgFlag();
	

	return;
}

UINT ThreadLiveGrab(LPVOID nParam)
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *pView=(CHVSystemView*)nParam;
	pMainFrame->OnSendData(MOTION_TEST_ON);
	while(1)
	{
		pView->GrabProcUnchecked();
		::Sleep(33);
		if(g_nLiveGrabThread==0) 
		{
			pMainFrame->OnSendData(MOTION_TEST_OFF);
			break;
		}

	}
	return 1;
}

UINT ThreadTestGrab(LPVOID nParam) //TEST 모드 일때 사용 하는 쓰레드.
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *pView=(CHVSystemView*)nParam;	
	
	while(1)
	{	
		pView->Inspection();		
		::Sleep(0);
		if(g_nSingleTestThread==0)
			break;
	}
	return 1;

}
