// C6202EncoderDlg.h : header file
//

#if !defined(AFX_C6202ENCODERDLG_H__01E13B6E_EA35_4628_A5D5_1A2263FAF06B__INCLUDED_)
#define AFX_C6202ENCODERDLG_H__01E13B6E_EA35_4628_A5D5_1A2263FAF06B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\..\..\include\dask.h"

/////////////////////////////////////////////////////////////////////////////
// CC6202EncoderDlg dialog
static I16 g_dev;
static int g_tmp_value;

class CC6202EncoderDlg : public CDialog
{
// Construction
public:
	CC6202EncoderDlg(CWnd* pParent = NULL);	// standard constructor
	static void EPT_CallBack();


// Dialog Data
	//{{AFX_DATA(CC6202EncoderDlg)
	enum { IDD = IDD_C6202ENCODER_DIALOG };
	long	m_data;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC6202EncoderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CC6202EncoderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnBtnStart();
	afx_msg void OnBtnRead();
	afx_msg void OnBtnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_C6202ENCODERDLG_H__01E13B6E_EA35_4628_A5D5_1A2263FAF06B__INCLUDED_)
