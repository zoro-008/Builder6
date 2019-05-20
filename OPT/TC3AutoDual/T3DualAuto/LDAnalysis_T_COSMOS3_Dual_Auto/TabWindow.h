#if !defined(AFX_TABWINDOW_H__261B886E_407C_4FB0_8385_72450094D98B__INCLUDED_)
#define AFX_TABWINDOW_H__261B886E_407C_4FB0_8385_72450094D98B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TabWindow dialog
#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\xskinbutton.h"
#include "..\\guisrc\\StaticSkin.h"

#include "DataDisplay.h"
#include "SetupWin.h"
#include "CurrentData.h"
#include "EtcWindow.h"

#define BTN_NUM 8
class TabWindow : public CDialog
{
// Construction
private:
	CRect m_wndRect;
	CRect m_CtrlRect;
	CBmpRgn *pBmpRgn;
	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;
	CxSkinButton *m_wSysMenu;
    int m_nAppType;
	int m_iSelectedWindow;
    enum
	{
		BTN_CD=0,
		BTN_DL,
		BTN_SP,
		BTN_3D,
		BTN_ACS,
		BTN_MAS,
		BTN_AOP,
		BTN_ETC
	};
public:
	CxSkinButton *m_pTabButton[BTN_NUM];
	CDataDisplay *m_pwData;
    CSetupWin    *m_pwSetup;
	CCurrentData *m_pwCurrent;
	CEtcWindow   *m_pwEtc;
	int m_nSaveFileFlag;
	int m_nManualOptFlag;
	int m_nActivePasswd;
	int m_nSetupStatus;

	TabWindow(CWnd* pParent = NULL);   // standard constructor

	void MakeButton();
	void SelectWindow(int nWin);

	inline void TabWindow::SetAppType(int nType) { m_nAppType=nType; }
	inline void TabWindow::SetWndRect(CRect r)   { m_wndRect=r;          }
	inline int  TabWindow::GetSaveFileFlag()     { return m_nSaveFileFlag; }
	inline int  TabWindow::GetSetupActive()      { return m_nSetupStatus;}

// Dialog Data
	//{{AFX_DATA(TabWindow)
	enum { IDD = IDD_DIALOG_TABWIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TabWindow)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TabWindow)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnButtonDL();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonSP();
	afx_msg void OnButtonTabsysmenu();
	afx_msg void OnButtonCD();
	afx_msg void OnButton3DModel();
	afx_msg void OnButtonAbcenter();
	afx_msg void OnButtonManualopt();
	afx_msg void OnButtonAmopt();
	afx_msg void OnButtonETC();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABWINDOW_H__261B886E_407C_4FB0_8385_72450094D98B__INCLUDED_)
