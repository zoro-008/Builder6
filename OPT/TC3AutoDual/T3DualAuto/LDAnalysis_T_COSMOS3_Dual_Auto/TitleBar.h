#if !defined(AFX_TITLEBAR_H__DE16877D_D43E_495D_9802_099990499A67__INCLUDED_)
#define AFX_TITLEBAR_H__DE16877D_D43E_495D_9802_099990499A67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TitleBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTitleBar dialog
#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\xskinbutton.h"
#include "..\\guisrc\\StaticSkin.h"


class CTitleBar : public CDialog
{
// Construction
private:
	CBmpRgn *pBmpRgn;
	HDC m_dcBkGrnd;
	long m_nWindowWidth;
	long m_nWindowHeight;
	CxSkinButton *m_pwButton[4];//20101217 lwb 

	CFont *m_pFont;
public:
	CTitleBar(CWnd* pParent = NULL);   // standard constructor
	void MakeGui();
	void SelectWindow(int idx);

// Dialog Data
	//{{AFX_DATA(CTitleBar)
	enum { IDD = IDD_TITLE_BAR };
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTitleBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTitleBar)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTitleButton1();
	afx_msg void OnTitleButton2();
	afx_msg void OnTitleButton3();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TITLEBAR_H__DE16877D_D43E_495D_9802_099990499A67__INCLUDED_)
