// C6202GPTCDlg.h : header file
//

#if !defined(AFX_C6202GPTCDLG_H__ABB4F3EC_DEC9_4A39_8122_090E64978E01__INCLUDED_)
#define AFX_C6202GPTCDLG_H__ABB4F3EC_DEC9_4A39_8122_090E64978E01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\..\..\include\dask.h"

/////////////////////////////////////////////////////////////////////////////
// CC6202GPTCDlg dialog

class CC6202GPTCDlg : public CDialog
{
// Construction
public:
	CC6202GPTCDlg(CWnd* pParent = NULL);	// standard constructor

	I16 m_dev;

// Dialog Data
	//{{AFX_DATA(CC6202GPTCDlg)
	enum { IDD = IDD_C6202GPTC_DIALOG };
	int		m_mode;
	int		m_tc_num;
	DWORD	m_value;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC6202GPTCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CC6202GPTCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnBtnSetup();
	afx_msg void OnBtnStart();
	afx_msg void OnBtnStop();
	afx_msg void OnBtnSetIntGate();
	afx_msg void OnBtnResetIntGate();
	afx_msg void OnBtnRead();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_C6202GPTCDLG_H__ABB4F3EC_DEC9_4A39_8122_090E64978E01__INCLUDED_)
