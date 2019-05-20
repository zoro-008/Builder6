#if !defined(AFX_PASSWDINPUT_H__10887DB0_0BB3_4B34_B920_14C160EF5FC5__INCLUDED_)
#define AFX_PASSWDINPUT_H__10887DB0_0BB3_4B34_B920_14C160EF5FC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PasswdInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPasswdInput dialog
#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\StaticSkin.h"
#include "..\\guisrc\\xSkinButton.h"

class CPasswdInput : public CDialog
{
// Construction
public:
	CPasswdInput(CWnd* pParent = NULL);   // standard constructor
	CRect m_wndRect;
	CBmpRgn *pBmpRgn;
	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;
// Dialog Data
	//{{AFX_DATA(CPasswdInput)
	enum { IDD = IDD_DIALOG_PASSINPUT };
	CString	m_Passwd;
	//}}AFX_DATA

    CEdit *pPasswdEdit;
	CxSkinButton *pButton[2];
	void MakeControl();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPasswdInput)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPasswdInput)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSWDINPUT_H__10887DB0_0BB3_4B34_B920_14C160EF5FC5__INCLUDED_)
