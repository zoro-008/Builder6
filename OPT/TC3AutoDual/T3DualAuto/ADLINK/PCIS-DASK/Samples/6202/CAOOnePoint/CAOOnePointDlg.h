// CAOOnePointDlg.h : header file
//

#if !defined(AFX_CAOONEPOINTDLG_H__AF337BF6_3007_48DD_859D_A1036E368D0B__INCLUDED_)
#define AFX_CAOONEPOINTDLG_H__AF337BF6_3007_48DD_859D_A1036E368D0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\..\..\include\dask.h"

/////////////////////////////////////////////////////////////////////////////
// CCAOOnePointDlg dialog

class CCAOOnePointDlg : public CDialog
{
// Construction
public:
	CCAOOnePointDlg(CWnd* pParent = NULL);	// standard constructor

	I16 m_dev;

// Dialog Data
	//{{AFX_DATA(CCAOOnePointDlg)
	enum { IDD = IDD_CAOONEPOINT_DIALOG };
	int		m_ch;
	double	m_voltage;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCAOOnePointDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCAOOnePointDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnBtnOutput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAOONEPOINTDLG_H__AF337BF6_3007_48DD_859D_A1036E368D0B__INCLUDED_)
