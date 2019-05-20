// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include <winuser.h>
#include "HVSystem.h"


#include "MainFrm.h"
#include "External.h"
#include "HVSystemDefine.h"
#include "HVSystemView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
//	gMotionResultData.bInspection = VISION_READY;
//	gMotionResultData.bReInsp = FALSE ;
	gMotionResultData.dAngleX = 0.0;
	gMotionResultData.dAngleY = 0.0;
	gMotionResultData.dInlineRShift = 0.0;
	gMotionResultData.nAngleOkNg = VISION_NG;
	gMotionResultData.nInlineROkNg = VISION_NG;
}

CMainFrame::~CMainFrame()
{

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowText("HVSystemMain");


	//OnSendData(1);

	return 0;
}

/*void CMainFrame::OnCopyData(WPARAM wParam , LPARAM lParam)
{
    COPYDATASTRUCT  *pCds;
    pCds = (PCOPYDATASTRUCT)lParam;

    if( pCds == NULL) return;

    AfxMessageBox( (char*)pCds->lpData) ;    

}*/

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.hMenu=NULL;
	cs.style = WS_POPUP|WS_VISIBLE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
struct
{
   HANDLE  hMutex;
   CWnd *  Handle;
   int     cmd;
   char    Data[1024];
} m;

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// TODO: Add your message handler code here and/or call default
	//CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	//CHVSystem *m_pApp = (CHVSystem *)AfxGetApp()->GetActiveView();
	
/*
	
	


CView       ------------ ( GetDocument ) -------------->     CDocument
            <---( GetFirstViewPosition / GetNextView ) ---

 
         전역 공통

AfxGetApp / AfxGetmainWnd     

CView      -------------( GetParentFrame) ------------->    CFrameWnd
           <------------( GetActiveView ) --------------

 

CFrameWnd ------------( GetActiveDocument ) --------->   CDocument
*/

    if( pCopyDataStruct == NULL) return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);

	CHVSystemView *m_pView = (CHVSystemView *)GetActiveView();	

	 

	//m_pView -> m_wDisplayComm.AddListData("OnCopyData");

/*

Caption = "TCosmos3MotionManager"
Vision -> Motion
(VC01)             //ALLHOME
(VC02)             //RESET
(VC03)             //CLOSE AP
(VC04)<좌표,좌표>  //검사결과.
(VC05)             //UI Show

Motion -> Vision
(MC01)             //검사.//이것만 확인 하고, 검사.
*/

    //AfxMessageBox( (char*)pCopyDataStruct->lpData) ; 
	//20110124 lwb 다음은 메시지를 받으면, Thread에서 쓸 수 있도록 검사하라는 스레드를 날려준다.
	CString csTemp, csCMD, csMessage;
	int nNum;
	//memcpy(&m_cBeamRect[i],&scResult[i].r,sizeof(RECT));
	csTemp = (char*)pCopyDataStruct->lpData;
	csCMD = csTemp.Left(6);	
	char buf[256];

	if(csCMD == "(MC01)") //정식 검사.
	{
        m_pView -> Inspection(tmNormal); //쓰레드 안타게 하기 위해서 삽입.
		g_iTestMode = tmNormal ;

		//모션에 데이터 보내기.
		
		
		m_pView -> m_wDisplayComm.AddListData(buf);
		OnSendData(MOTION_POSITION, 
			gMotionResultData.dAngleX, gMotionResultData.dAngleY, gMotionResultData.nAngleOkNg,
			gMotionResultData.dInlineRShift, gMotionResultData.nInlineROkNg, 
			gMotionResultData.dVerticalCMD, gMotionResultData.nGauVirOkNg, 
			gMotionResultData.dHosizontalCMD, gMotionResultData.nGauHorOkNg,
			gMotionResultData.dBeam_Shift_R , gMotionResultData.dBeam_Shift_T,gMotionResultData.nBeamOkNg ,	
			gMotionResultData.dAngle            ,
			gMotionResultData.dFA_R             ,
			gMotionResultData.dFA_T             ,
			gMotionResultData.dThree_Beam_Tilt  ,
			gMotionResultData.dBeam_Size        ,
			gMotionResultData.dGT_Shift         ,
			gMotionResultData.dArea             ,
			gMotionResultData.dPeak_Ratio       ,
			gMotionResultData.dParallelism      ,
			gMotionResultData.dAbsolute_Center_X,
			gMotionResultData.dAbsolute_Center_Y,
			gMotionResultData.dBeam_Center_X    ,
			gMotionResultData.dBeam_Center_Y    ,
			gMotionResultData.dPeak_Center_X    ,
			gMotionResultData.dPeak_Center_Y    ,
			gMotionResultData.dBeam_Shift       ,
			gMotionResultData.dBeam_Shift_R     ,
			gMotionResultData.dBeam_Shift_T		);//,	
		//		gMotionResultData.sEvalTime         );
		
		sprintf(buf,"B CMD : %6.2f-----------",gMotionResultData.dHosizontalCMD);
		m_pView -> m_wDisplayComm.AddListData(buf);
	

	}
	else if(csCMD == "(MC02)") //오토 모드 전환
	{
        m_pView -> m_wCtrlWindow.SetCrntMode(AUTO_MODE);

	}
	else if(csCMD == "(MC03)") //Test 모드 전환.
	{
		m_pView -> m_wCtrlWindow.SetCrntMode(TEST_MODE);		

	}		
	else if(csCMD == "(MC04)") //ERR 메세지.
	{		
		nNum = csTemp.Find(">");
		csMessage = csTemp.Mid(7,nNum - 7);
		//gMotionResultData.csMessage = csMessage;		
		//gMotionResultData.bMessageRequest = TRUE;
	}	
	else if(csCMD == "(MC05)") //Left Table
	{
		g_bLeftRight = FALSE;
	}
	else if(csCMD == "(MC06)") //Right Table
	{
		g_bLeftRight = TRUE;
	}
	else if(csCMD == "(MC07)") //리 테스트.
	{
//		gMotionResultData.bInspection = VISION_INSPECTION;
//        gMotionResultData.bReInsp = TRUE ;
		g_iTestMode = tmReTest ;
        m_pView -> Inspection(g_iTestMode); //쓰레드 안타게 하기 위해서 삽입.

		sprintf(buf,"B CMD : %6.2f",gMotionResultData.dHosizontalCMD);
		m_pView -> m_wDisplayComm.AddListData(buf);
		OnSendData(MOTION_POSITION, 
			gMotionResultData.dAngleX, gMotionResultData.dAngleY, gMotionResultData.nAngleOkNg,
			gMotionResultData.dInlineRShift, gMotionResultData.nInlineROkNg, 
			gMotionResultData.dVerticalCMD, gMotionResultData.nGauVirOkNg, 
			gMotionResultData.dHosizontalCMD, gMotionResultData.nGauHorOkNg,
			gMotionResultData.dBeam_Shift_R , gMotionResultData.dBeam_Shift_T,gMotionResultData.nBeamOkNg ,	
			gMotionResultData.dAngle            ,
			gMotionResultData.dFA_R             ,
			gMotionResultData.dFA_T             ,
			gMotionResultData.dThree_Beam_Tilt  ,
			gMotionResultData.dBeam_Size        ,
			gMotionResultData.dGT_Shift         ,
			gMotionResultData.dArea             ,
			gMotionResultData.dPeak_Ratio       ,
			gMotionResultData.dParallelism      ,
			gMotionResultData.dAbsolute_Center_X,
			gMotionResultData.dAbsolute_Center_Y,
			gMotionResultData.dBeam_Center_X    ,
			gMotionResultData.dBeam_Center_Y    ,
			gMotionResultData.dPeak_Center_X    ,
			gMotionResultData.dPeak_Center_Y    ,
			gMotionResultData.dBeam_Shift       ,
			gMotionResultData.dBeam_Shift_R     ,
			gMotionResultData.dBeam_Shift_T		);//,	
		//		gMotionResultData.sEvalTime         );
		
		sprintf(buf,"B CMD : %6.2f-----------",gMotionResultData.dHosizontalCMD);
		m_pView -> m_wDisplayComm.AddListData(buf);
	}
    else if(csCMD == "(MC08)") //3 테스트.
	{

		g_iTestMode = tm3rdTest ;
        m_pView -> Inspection(g_iTestMode); //쓰레드 안타게 하기 위해서 삽입.

		sprintf(buf,"B CMD : %6.2f",gMotionResultData.dHosizontalCMD);
		m_pView -> m_wDisplayComm.AddListData(buf);
		OnSendData(MOTION_POSITION, 
			gMotionResultData.dAngleX, gMotionResultData.dAngleY, gMotionResultData.nAngleOkNg,
			gMotionResultData.dInlineRShift, gMotionResultData.nInlineROkNg, 
			gMotionResultData.dVerticalCMD, gMotionResultData.nGauVirOkNg, 
			gMotionResultData.dHosizontalCMD, gMotionResultData.nGauHorOkNg,
			gMotionResultData.dBeam_Shift_R , gMotionResultData.dBeam_Shift_T,gMotionResultData.nBeamOkNg ,	
			gMotionResultData.dAngle            ,
			gMotionResultData.dFA_R             ,
			gMotionResultData.dFA_T             ,
			gMotionResultData.dThree_Beam_Tilt  ,
			gMotionResultData.dBeam_Size        ,
			gMotionResultData.dGT_Shift         ,
			gMotionResultData.dArea             ,
			gMotionResultData.dPeak_Ratio       ,
			gMotionResultData.dParallelism      ,
			gMotionResultData.dAbsolute_Center_X,
			gMotionResultData.dAbsolute_Center_Y,
			gMotionResultData.dBeam_Center_X    ,
			gMotionResultData.dBeam_Center_Y    ,
			gMotionResultData.dPeak_Center_X    ,
			gMotionResultData.dPeak_Center_Y    ,
			gMotionResultData.dBeam_Shift       ,
			gMotionResultData.dBeam_Shift_R     ,
			gMotionResultData.dBeam_Shift_T		);//,	
		//		gMotionResultData.sEvalTime         );
		
		sprintf(buf,"B CMD : %6.2f-----------",gMotionResultData.dHosizontalCMD);
		m_pView -> m_wDisplayComm.AddListData(buf);
	}


	
    //GetActiveView() ->Clear  ;
    
	
	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

void CMainFrame::OnSendData(int cmd,  double Para1 , double Para2, int Para3, double Para4, int Para5, double Para6, int Para7, double Para8, int Para9, /*Beam*/ double Para10 , double Para11 , int Para12 ,
							double  SPara1   , double SPara2   ,
							double  SPara3   , double SPara4   ,
							double  SPara5   , double SPara6   ,
							double  SPara7   , double SPara8   ,
							double  SPara9   , double SPara10  ,
							double  SPara11  , double SPara12  ,
							double  SPara13  , double SPara14  ,
							double  SPara15  , double SPara16  ,
							double  SPara17  , double SPara18  )//,
							//CString SPara19  )
/*
 void OnSendData(int cmd,  
		            double Para1  = 0.0 , double Para2 = 0.0 , int    Para3  = 0  , double Para4  = 0.0, int    Para5  = 0  , double Para6  = 0  , int    Para7  = 0  , double Para8  = 0  , int    Para9  = 0  , double Para10  = 0.0, double Para11 = 0.0 , int Para12 = 0, 
		            double SPara1 = 0.0 , double SPara2 = 0.0, double SPara3 = 0.0, double SPara4 = 0.0, double SPara5 = 0.0, double SPara6 = 0.0, double SPara7 = 0.0, double SPara8 = 0.0, double SPara9 = 0.0, double SPara10 = 0.0,
					double SPara11= 0.0 , double SPara12= 0.0, double SPara13= 0.0, double SPara14= 0.0, double SPara15= 0.0, double SPara16= 0.0, double SPara17= 0.0, double SPara18= 0.0, CString SPara19="" );

*/
{
	    char buf[256];
     	CHVSystemView *m_pView = (CHVSystemView *)GetActiveView();
        sprintf(buf,"PARA8 : %6.2f",Para8);
	    m_pView -> m_wDisplayComm.AddListData(buf);


/*
Vision -> Motion
(VC01)             //ALLHOME//Btn
(VC02)             //RESET//Btn
(VC03)             //CLOSE AP
(VC04)<좌표,좌표,OK/NG>  //검사결과.//cmd 여기서만 Para1,2,3 들어감. Para3이 OK = 1, NG = 2
(VC05)             //UI Show//Btn
(VC06)				//TEST ON
(VC07)				//TEST OFF
(VC08)				//MOTION_APC_LEFT
(VC09)				//MOTION_APC_RIGHT
(VC10)				//APC POWER QUANTITY UP
(VC11)				//APC POWER QUANTITY DOWN
 
Motion -> Vision
(MC01)             //검사.
*/


/*
CString Browser="123456798";

char string[100];

strcpy ( string, Browser);

	
*/
   CString sCmd  ;
   sCmd.Format("(VC%02d)" , cmd );

   CString sData ;
                                                                                                     //여기부터 서버꺼.
   if(cmd == MOTION_POSITION)sData.Format(
	   "<%.3f,%.3f,%d,%.3f,%d,%.3f,%d,%.3f,%d,%.3f,%.3f,%d,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%s>" ,
	   Para1,Para2,Para3,Para4,Para5,Para6,Para7,Para8,Para9,Para10,Para11,Para12,
	   SPara1 ,SPara2 ,SPara3 ,SPara4 ,SPara5 ,SPara6 ,SPara7 ,SPara8 ,SPara9 ,SPara10,
	   SPara11,SPara12,SPara13,SPara14,SPara15,SPara16,SPara17,SPara18,"");

   static int iLastLogTick = GetTickCount();

   if( m.hMutex == NULL ) {
      m.hMutex = CreateMutex(NULL, FALSE, MUTEX_VISION);
      if( m.hMutex == NULL) {
         m.hMutex = OpenMutex(MUTEX_ALL_ACCESS	, FALSE, MUTEX_VISION);
         if( m.hMutex == NULL) {
            return;
         }
      }
   }

   char szBuffer[1024];
   CString csTemp;
   csTemp = sCmd + sData;

   strcpy(szBuffer , (LPSTR)(LPCSTR)csTemp) ;

   int nBuf = strlen(szBuffer);

   DWORD  dwError = GetLastError();
   COPYDATASTRUCT cds;
   cds.cbData = nBuf + 1;
   cds.lpData = (void *) szBuffer;

   if (!m.Handle) {
   	//m.Handle = FindWindow(WIN_NAME , NULL);
        m.Handle = FindWindow(NULL,WIN_NAME);
   }
   else if( GetTickCount() - iLastLogTick > 1000) {
	//m.Handle = FindWindow(WIN_NAME , NULL);
        m.Handle = FindWindow(NULL,WIN_NAME);
   }

   if (m.Handle) ::SendMessage(m.Handle->GetSafeHwnd() ,   WM_COPYDATA, NULL, (LPARAM)&cds);

   SetLastError(dwError);
   ReleaseMutex(m.hMutex);
}



