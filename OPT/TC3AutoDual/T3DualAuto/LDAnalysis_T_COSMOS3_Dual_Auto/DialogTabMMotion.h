#if !defined(AFX_DIALOGTABMMOTION_H__8478FFCE_88E6_4DF9_A692_95D924CCE82A__INCLUDED_)
#define AFX_DIALOGTABMMOTION_H__8478FFCE_88E6_4DF9_A692_95D924CCE82A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogTabMMotion.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogTabMMotion dialog

class CDialogTabMMotion : public CDialog
{
// Construction
public:
	CDialogTabMMotion(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogTabMMotion)
	enum { IDD = IDD_DIALOG_TAB_M_MOTION };
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTabMMotion)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	inline void CDialogTabMMotion::SetWindowRect(CRect r) { m_wndRect = r; }

public:
	CRect			m_wndRect;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogTabMMotion)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTABMMOTION_H__8478FFCE_88E6_4DF9_A692_95D924CCE82A__INCLUDED_)
