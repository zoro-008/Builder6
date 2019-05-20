#if !defined(AFX_OKNGWINDOW_H__DE30818E_672A_4F78_9907_A122E35E7C6A__INCLUDED_)
#define AFX_OKNGWINDOW_H__DE30818E_672A_4F78_9907_A122E35E7C6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OkNgWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COkNgWindow dialog
#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\xSkinButton.h"
class COkNgWindow : public CDialog
{
// Construction
public:
	COkNgWindow(CWnd* pParent = NULL);   // standard constructor

	CBmpRgn *pBmpRgn;
	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;
	CxSkinButton *m_pButton;
// Dialog Data
	//{{AFX_DATA(COkNgWindow)
	enum { IDD = IDD_DIALOG_OKNG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COkNgWindow)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COkNgWindow)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnCheckPeak();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OKNGWINDOW_H__DE30818E_672A_4F78_9907_A122E35E7C6A__INCLUDED_)
