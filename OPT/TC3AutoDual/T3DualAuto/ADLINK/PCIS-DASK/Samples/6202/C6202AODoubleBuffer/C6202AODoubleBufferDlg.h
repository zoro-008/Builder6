// C6202AODoubleBufferDlg.h : header file
//

#if !defined(AFX_C6202AODOUBLEBUFFERDLG_H__BEA45924_71F5_4B16_94C0_DA782FD85D61__INCLUDED_)
#define AFX_C6202AODOUBLEBUFFERDLG_H__BEA45924_71F5_4B16_94C0_DA782FD85D61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\..\..\include\dask.h"

#define DATA_CNT 1024

/////////////////////////////////////////////////////////////////////////////
// CC6202AODoubleBufferDlg dialog

class CC6202AODoubleBufferDlg : public CDialog
{
// Construction
public:
	CC6202AODoubleBufferDlg(CWnd* pParent = NULL);	// standard constructor

	I16 m_dev;
	U32 m_data_buffer[3][DATA_CNT];
	U32 m_dev_buffer[2][DATA_CNT];

// Dialog Data
	//{{AFX_DATA(CC6202AODoubleBufferDlg)
	enum { IDD = IDD_C6202AODOUBLEBUFFER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC6202AODoubleBufferDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CC6202AODoubleBufferDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnBtnStart();
	afx_msg void OnBtnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_C6202AODOUBLEBUFFERDLG_H__BEA45924_71F5_4B16_94C0_DA782FD85D61__INCLUDED_)
