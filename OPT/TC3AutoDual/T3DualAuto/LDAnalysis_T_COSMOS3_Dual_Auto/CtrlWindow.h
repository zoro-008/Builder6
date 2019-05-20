#if !defined(AFX_CTRLWINDOW_H__539BA1A2_6F74_424A_95D2_7200F616370F__INCLUDED_)
#define AFX_CTRLWINDOW_H__539BA1A2_6F74_424A_95D2_7200F616370F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CtrlWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCtrlWindow dialog
#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\xSkinButton.h"

enum {
	GRAB_BUTTON    =  0 ,
	LIVE_BUTTON    =  1,
	SAVE_BUTTON    =  2,
	AUTO_BUTTON    =  3,
	TEST_BUTTON    =  4,
	RESET_BUTTON   =  5,
    RSAVE_BUTTON   =  6,
	EXIT_BUTTON    =  7,
	MAX_BUTTON_CNT  
};

enum EN_MODE{
    NONE_MODE = 0 ,
	LIVE_MODE = 1 ,
	AUTO_MODE = 2 ,
	TEST_MODE = 3 ,
	MAX_MODE
};


class CCtrlWindow : public CDialog
{
// Construction
private:
    CRect m_wndRect;
	CRect m_ExitRect;
	CRect m_GrabRect;
	CRect m_LiveRect;
	CRect m_InspectRect;
	CRect m_SetupRect;

	int m_nBtnYoffset;
	int m_nLiveButton;
	
	CxSkinButton *m_wSysMenu;
	CxSkinButton *m_wSysLoad;

    
    EN_MODE m_iCrntMode ;



public:
	CString m_strBarcode;
	CStaticSkin *m_pwStBarcode;
	CEdit   *m_pwEdBarcode;

	void MakeGui();
	void CheckBarcode();

	EN_MODE GetCrntMode();

	void SetCrntMode(EN_MODE iMode);




public:
	CRect m_cBtnRect[8];
	CxSkinButton m_wCtrlButton[8];

	CCtrlWindow(CWnd* pParent = NULL);   // standard constructor

	CBmpRgn *pBmpRgn;
	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;
	void EnableButtons(int idx,BOOL bValue);
	void CheckButtons(int  idx,BOOL bValue);
	void SetCursorToWindow();

	inline void CCtrlWindow::SetLiveButton(int nLive) { m_nLiveButton=nLive; }
	inline int  CCtrlWindow::GetLiveButton()  { return m_nLiveButton; }
	inline void CCtrlWindow::SetWindowRect(CRect r) { m_wndRect = r; }

// Dialog Data
	//{{AFX_DATA(CCtrlWindow)
	enum { IDD = IDD_CONTROL_PANEL };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCtrlWindow)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CCtrlWindow)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnButtonExit();
	afx_msg void OnButtonGrab();
	afx_msg void OnButtonLive();
	afx_msg void OnButtonTest();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonAuto();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonRsave();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnButtonCameraControl();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonSysmenuControl2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTRLWINDOW_H__539BA1A2_6F74_424A_95D2_7200F616370F__INCLUDED_)
