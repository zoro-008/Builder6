// HVSystemView.h : interface of the CHVSystemView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HVSYSTEMVIEW_H__547E8339_BAC4_4E5D_8EA4_E715803B53F2__INCLUDED_)
#define AFX_HVSYSTEMVIEW_H__547E8339_BAC4_4E5D_8EA4_E715803B53F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TitleBar.h"
#include "BottomBar.h"

////////////////////////////////////////////////
///////20110104 lwb 1394 IMI 카메라 사용을 위해.
#include "ImiTech.h"
//#include "MatroxBD.h"
////////////////////////////////////////////////
//ssss
#include "HVSystemDoc.h"


#include "ImgDisplay.h"
#include "ImgDisplay2.h"
#include "ImgDisplay3.h"
#include "HProjection.h"
#include "VProjection.h"
#include "Anglewindow.h"
#include "3dmodelwindow.h"
#include "DisplayComm.h"
#include "TabWindow.h"
#include "CtrlWindow.h"
#include "IoWindow.h"
#include "CounterWindow.h"
#include "OkNgWindow.h"
#include "DataSequence.h"
#include "MoveArea.h"
#include "CommThread.h"
//20101217 lwb Motion Dialog 추가.
#include "DialogTabMBase.h"
#include "External.h"

#include "..\\GuiSrc\\xSkinButton.h"
#include "..\\guisrc\\LineProc.h"

#include "..\\include\\KProcess.h"
#include "..\\include\\KThresh.h"
#include "..\\include\\KMorph.h"
#include "..\\include\\KMeas.h"
#include "..\\include\\KRBLob.h"
#include "..\\include\\KFitting.h"
#include "..\\include\\KProcess.h"
#include "..\\include\\KMoment.h"

class CHVSystemView : public CFormView
{
private:
	int m_nInitialUpdate;    // 초기화 종료 flag...

protected: // create from serialization only
	CHVSystemView();
	DECLARE_DYNCREATE(CHVSystemView)

public:
	//{{AFX_DATA(CHVSystemView)
	enum{ IDD = IDD_HVSYSTEM_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	SETTING_DATA     m_cSettingData;
	SETTING_DATA_ADD m_cSettingDataAdd;
	SETTING_DATAEX   m_cSettingDataEx;
	SETTING_DATAEX2 m_cSettingDataEx2;

	double m_nCx,m_nCy;
	int m_nWidth,m_nHeight;
	int m_nAutoTestFlag;
	int m_nFailFlag;
	int m_nAutoManualFlag;


//	BOOL m_bMotionMode;
////////////////////////////////////////////////
///////20110104 lwb 1394 IMI 카메라 사용을 위해.
	CImiTech	*m_pwImi;		
//	CMatroxBD m_scMBDOne;
////////////////////////////////////////////////
	CImgDisplay  m_wImgDisp1;
	CImgDisplay2 m_wImgDisp2;
	CImgDisplay3 m_wImgDisp3;
	CHProjection m_wHProj;
	CVProjection m_wVProj;
//	CAngleWindow m_wAngle;

	//20101217 lwb Motion 구동부 추가.
	CDialogTabMBase m_wMotionBase;

//20101216 lwb Debug모드와 Release 모드가 같게 하기 위해 주석 처리
//#ifndef _DEBUG
//	C3DModelWindow m_w3DModelWindow;
//#endif	
	CTitleBar m_wTitleBar;
	CBottomBar m_wBottomBar;
	CDisplayComm m_wDisplayComm;
	CCtrlWindow m_wCtrlWindow;
    CIoWindow   m_wIoWindow;
	COkNgWindow m_wOkNgWindow;
	TabWindow m_wTabWindow;
	CMoveArea m_wMoveArea;

	CWinThread *m_pTestThread;
	CWinThread *m_pLiveThread;	
	CWinThread *m_pSingleTestThread;


	char m_pCurFname[256];
public:
	CCommThread m_Comm;
	int m_iComportOpenFlag;

    CString m_strBCode;
	int     m_iBarcodeReadingFlag;
	int     m_iSaveMode;
 
	int  ByteToChar(BYTE *pIn,char *pOut,int num);
	void ReInitComport(int port);
	int  ReadBarcode();
 	inline int CHVSystemView::GetBarcodeStatus() { return m_iBarcodeReadingFlag; }
	inline int CHVSystemView::GetSaveMode()      { return m_iSaveMode;           }
public:
public:
	CHVSystemDoc* GetDocument();
	int m_nTestFlag;
	int m_nStartFlag;
	int m_nSaveFlag;
	int m_nCureFlag;
	int m_nUVEndFlag;
	int m_nRisingEdge;
#ifdef __SAMSUNG_TSST_VERSION
	BOOL m_bCD;
	inline BOOL CHVSystemView::GetCDStatus() { return m_bCD; }
#endif
	double m_fUVTime;
	double m_fLDTime;

	void MakeDisplayWindow();
	void ExitProc();

	void GrabProc();
	void SetBufferAll();
	void GrabProcUnchecked();
	int  TestProc(int pflag=1,EN_TEST_MODE iTestMode=tmNormal);
	void StartLiveThread();
	void EndLiveThread();
	void StartTestThread();
	void EndTestThread();
    void RunSaveImg();
	void RunLoadImg();
    void ClearData();
	void OpenApp();
	void CloseApp();
	int  StartAnalyzer(int pflag=1,EN_TEST_MODE iTestMode=tmNormal);
	int  CheckHWLock();
	void SaveData();
	void SaveSimpleData(int nTotalCount=0);
	void ResetOkNgWindow();
	void SettingParam(SETTING_DATA d);
	void SettingParam(SETTING_DATA_ADD d);
	void SettingParam(SETTING_DATAEX d) ;
	void SettingParam(SETTING_DATAEX2 d); 
	
	void Inspection(EN_TEST_MODE iTestMode = tmNormal);
	void SaveServerData();
	void SetServerData();
	
	
	inline int  CHVSystemView::GetUpdateFlag()              { return m_nInitialUpdate; }
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHVSystemView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

	afx_msg LRESULT OnCommMessage(WPARAM wParam,LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SucktionManual(BOOL nInput[16], int flag);
	virtual ~CHVSystemView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHVSystemView)
	afx_msg void OnTimer(UINT nIDEvent);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#ifndef _DEBUG  // debug version in HVSystemView.cpp
inline CHVSystemDoc* CHVSystemView::GetDocument()
   { return (CHVSystemDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HVSYSTEMVIEW_H__547E8339_BAC4_4E5D_8EA4_E715803B53F2__INCLUDED_)
