#if !defined(AFX_IOWINDOW_H__C0E212C1_62BB_4AF6_806F_EC95382F2F1C__INCLUDED_)
#define AFX_IOWINDOW_H__C0E212C1_62BB_4AF6_806F_EC95382F2F1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IoWindow.h : header file

#define _IO_ENABLE
#define _ADLINK_IO_ 

#ifndef _ADLINK_IO_ 
#define _PAIX_IO_ 
#endif

/////////////////////////////////////////////////////////////////////////////
// CIoWindow dialog
#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\xSkinButton.h"

class CInputItem
{
public:
	CInputItem(){}
	~CInputItem(){}
	enum
	{
		//TCOSMOS3 IO MAP INPUT
		PCIKUP_SWITCH=0, //0 , 픽업 스위치 버튼 
		CHUCKING_SWITCH, //1 , 척킹 버튼 
		INOUT_SWITCH,//2     , 인아웃 버튼 
		UV_SWITCH,//3        , UV 버튼 
 		PICKUP_FSENSOR,//4   , 픽업 주축 전진 센서 
		UPDOWN_FSENSOR,//5 , 척킹 전진 센서 
		INOUT_FSENSOR,//6    , 인아웃 전진 센서 
		UV_RSENSOR,//7       , UV 후진 센서 
		UV_FSENSOR,//8       , UV 전진 센서 
		UV_OFF,//9           , UV OFF ( 셔터 ON/OFF)
		ADJUST_SENSOR,//10    , 조정기 Up/Down 센서 
		UPDOWN_SWITCH,//11   
		VACUUM_SWITCH//12
	};
};

class COutputItem
{
public:
	COutputItem(){}
	~COutputItem(){}
	enum
	{
		//TCOSMOS3 IO MAP OUTPUT
		PICKUP1_SOL=0, //0  , 픽업 주축 SOL
		CHUCKING2_SOL,//1 , 픽업 부축 SOL
		PICKUP2_SOL,//2   , 척킹 SOL
		UPDOWN_SOL,//3      , INOUT SOL
		SUCKTION_SOL,//4         , UV SOL
 		PICKUP_SW_LAMP,//5 , 픽업 스위치 램프 
		CHUCKING_SW_LAMP,//6, 척킹 스위치 램프 
		INOUT_SW_LAMP,//7  ,INOUT 스위치 램프 
		UV_SW_LAMP,//8     , UV 스위치 램프 
 		UV_START,//9
		CHUCKING1_SOL,//10
		UPDOWN_LAMP,//11
		VACUUM_LAMP,//12
		RESERVED13,//13
		UV_SOL,//14
		INOUT_SOL//15
	};
};
/////////////////////////////////////
// output definition

class CIoWindow : public CDialog
{
// Construction
private:
 
public:
	CIoWindow(CWnd* pParent = NULL);   // standard constructor
	CBmpRgn *pBmpRgn;
	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;
	CxSkinButton m_wOut[16];
	CxSkinButton m_wIn[16];
	CxSkinButton *m_wLinkButton;

	CxSkinButton m_btnMotion[3];
	CxSkinButton m_btnAPC[2];
	CxSkinButton m_btnAPC_POWER[2];

	short m_nOutputValue;
	short m_nInputValue;
	BOOL  m_nDeviceSuccess;
	short m_nDevNum;
	BOOL  m_nInputStatus[16];
	BOOL  m_nCheckFlag;
	
	void MakeGui();
	void StartIoCheck();
	void EndIoCheck();
	void CheckInput();
	void GetInputStatus(BOOL *pOut);
	void GetIo(BOOL *pOut);

    void OpenNetwork();
	void CloseNetwork();
	void OutPort(int port_num,int on_off);
////////////////////////////////////////////////
	void ResetAll();
	void OutAll();

	void ButtonOuput(int nPort,int status);
/////////////////////////////////////////////////
/////////////////////////////////////////////////

	void UVSol(BOOL status);

	void UVStart();
////////////////////////////////////////////////
	inline BOOL CIoWindow::GetCheckFlag() { return m_nCheckFlag; }
// Dialog Data
	//{{AFX_DATA(CIoWindow)
	enum { IDD = IDD_IO_CONTROL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIoWindow)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIoWindow)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnIoOut0();
	afx_msg void OnIoOut1();
	afx_msg void OnIoOut2();
	afx_msg void OnIoOut3();
	afx_msg void OnIoOut4();
	afx_msg void OnIoOut5();
	afx_msg void OnIoOut6();
	afx_msg void OnIoOut7();
	afx_msg void OnIoOut8();
	afx_msg void OnIoOut9();
	afx_msg void OnIoOut10();
	afx_msg void OnIoOut11();
	afx_msg void OnIoOut12();
	afx_msg void OnIoOut13();
	afx_msg void OnIoOut14();
	afx_msg void OnIoOut15();
	afx_msg void OnButtonLink();
	afx_msg void OnBtnUiShow();
	afx_msg void OnBtnAllHome();
	afx_msg void OnBtnReset();
	afx_msg void OnBtnApcLeft();
	afx_msg void OnBtnApcRight();
	afx_msg void OnBtnApcUp();
	afx_msg void OnBtnApcDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IOWINDOW_H__C0E212C1_62BB_4AF6_806F_EC95382F2F1C__INCLUDED_)
