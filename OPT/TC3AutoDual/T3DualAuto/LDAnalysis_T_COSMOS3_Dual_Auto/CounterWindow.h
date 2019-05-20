#if !defined(AFX_COUNTERWINDOW_H__EB1986C4_CD08_4AF6_9937_9AACF3F5DFDA__INCLUDED_)
#define AFX_COUNTERWINDOW_H__EB1986C4_CD08_4AF6_9937_9AACF3F5DFDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CounterWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCounterWindow dialog
#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\xSkinButton.h"
#include "..\\guisrc\\StaticSkin.h"

#define CNT_TOTAL_COUNT 0
#define CNT_GODO_COUNT  1
#define CNT_FAIL_COUNT  2
#define CNT_FRATE       3
#define CNT_TIME        4
#define CNT_PROC_TIME   5
#define CNT_TEST_TIME   6
#define CNT_FS_COUNT    7

class CCounterWindow : public CDialog
{
// Construction
public:
	CCounterWindow(CWnd* pParent = NULL);   // standard constructor
	CBmpRgn *pBmpRgn;

	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;
	CStaticSkin *m_wpTitle[8];
	CStaticSkin *m_wpData[8];
// Dialog Data
	//{{AFX_DATA(CCounterWindow)
	enum { IDD = IDD_CNTWIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCounterWindow)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCounterWindow)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COUNTERWINDOW_H__EB1986C4_CD08_4AF6_9937_9AACF3F5DFDA__INCLUDED_)
