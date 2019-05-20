#if !defined(AFX_BOTTOMBAR_H__1F9DD1CC_CDAD_4DD0_BC49_33EAD605A51D__INCLUDED_)
#define AFX_BOTTOMBAR_H__1F9DD1CC_CDAD_4DD0_BC49_33EAD605A51D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BottomBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBottomBar dialog
#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\xSkinButton.h"

class CBottomBar : public CDialog
{
// Construction
public:
	CBottomBar(CWnd* pParent = NULL);   // standard constructor
	CBmpRgn *pBmpRgn;

	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;
	CxSkinButton *m_wHanraLogo;
// Dialog Data
	//{{AFX_DATA(CBottomBar)
	enum { IDD = IDD_BOTTOM_BAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBottomBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBottomBar)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOTTOMBAR_H__1F9DD1CC_CDAD_4DD0_BC49_33EAD605A51D__INCLUDED_)
