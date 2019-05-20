// C6202DIODlg.h : header file
//

#if !defined(AFX_C6202DIODLG_H__B502D023_5FD7_423E_8688_910E29AAE265__INCLUDED_)
#define AFX_C6202DIODLG_H__B502D023_5FD7_423E_8688_910E29AAE265__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\..\..\include\dask.h"

/////////////////////////////////////////////////////////////////////////////
// CC6202DIODlg dialog

class CC6202DIODlg : public CDialog
{
// Construction
public:
	CC6202DIODlg(CWnd* pParent = NULL);	// standard constructor

	I16 m_dev;

// Dialog Data
	//{{AFX_DATA(CC6202DIODlg)
	enum { IDD = IDD_C6202DIO_DIALOG };
	DWORD	m_di_0;
	DWORD	m_di_1;
	DWORD	m_do_0;
	DWORD	m_do_1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC6202DIODlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CC6202DIODlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnButtonDi0();
	afx_msg void OnButtonDi1();
	afx_msg void OnButtonDo0();
	afx_msg void OnButtonDo1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_C6202DIODLG_H__B502D023_5FD7_423E_8688_910E29AAE265__INCLUDED_)
