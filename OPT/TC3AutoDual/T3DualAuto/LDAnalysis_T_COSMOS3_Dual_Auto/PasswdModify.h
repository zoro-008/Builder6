#if !defined(AFX_PASSWDMODIFY_H__78CEC6CC_61DB_45FA_B9B0_4E57897A2B42__INCLUDED_)
#define AFX_PASSWDMODIFY_H__78CEC6CC_61DB_45FA_B9B0_4E57897A2B42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PasswdModify.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPasswdModify dialog
#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\StaticSkin.h"
#include "..\\guisrc\\xSkinButton.h"
class CPasswdModify : public CDialog
{
// Construction
public:
	CPasswdModify(CWnd* pParent = NULL);   // standard constructor
	CRect m_wndRect;
	CBmpRgn *pBmpRgn;
	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;
	CEdit *pPasswdEdit[2];
	CxSkinButton *pButton[2];

	void MakeControl();
// Dialog Data
	//{{AFX_DATA(CPasswdModify)
	enum { IDD = IDD_DIALOG_PASSMODIFY };
	CString	m_PasswdConfirm;
	CString	m_PasswdNew;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPasswdModify)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPasswdModify)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSWDMODIFY_H__78CEC6CC_61DB_45FA_B9B0_4E57897A2B42__INCLUDED_)
