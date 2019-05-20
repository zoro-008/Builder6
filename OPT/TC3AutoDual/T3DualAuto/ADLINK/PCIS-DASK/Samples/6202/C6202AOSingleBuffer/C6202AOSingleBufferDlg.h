// C6202AOSingleBufferDlg.h : header file
//

#if !defined(AFX_C6202AOSINGLEBUFFERDLG_H__2C022A7A_DE70_468D_ADA8_A5DD90727C2A__INCLUDED_)
#define AFX_C6202AOSINGLEBUFFERDLG_H__2C022A7A_DE70_468D_ADA8_A5DD90727C2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\..\..\include\dask.h"

#define DATA_CNT 1024

/////////////////////////////////////////////////////////////////////////////
// CC6202AOSingleBufferDlg dialog
class CC6202AOSingleBufferDlg : public CDialog
{
// Construction
public:
	CC6202AOSingleBufferDlg(CWnd* pParent = NULL);	// standard constructor

	I16 m_dev;
	U32 m_buffer[DATA_CNT];

// Dialog Data
	//{{AFX_DATA(CC6202AOSingleBufferDlg)
	enum { IDD = IDD_C6202AOSINGLEBUFFER_DIALOG };
	int		m_trg_src;
	DWORD	m_delay_1;
	DWORD	m_delay_2;
	int		m_iteration;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC6202AOSingleBufferDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CC6202AOSingleBufferDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnBtnStart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_C6202AOSINGLEBUFFERDLG_H__2C022A7A_DE70_468D_ADA8_A5DD90727C2A__INCLUDED_)
