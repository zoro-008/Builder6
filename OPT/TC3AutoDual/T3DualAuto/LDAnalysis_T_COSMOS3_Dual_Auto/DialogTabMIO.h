#if !defined(AFX_DIALOGTABMIO_H__E7EF7678_83A4_441F_A67E_94E92CC0F599__INCLUDED_)
#define AFX_DIALOGTABMIO_H__E7EF7678_83A4_441F_A67E_94E92CC0F599__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogTabMIO.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogTabMIO dialog
#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\xSkinButton.h"
#include "..\\guisrc\\StaticSkin.h"
#include "MotionDefine.h"
#include "SystemData.h"

typedef struct 
{
	int nMaxIOModuleNum;
	CString csIONameInput[MAX_IO_MODULE_NUM][MAX_IO_INDEX_NUM];
	CString csIONameOutput[MAX_IO_MODULE_NUM][MAX_IO_INDEX_NUM];

	BOOL bIOStatusInput[MAX_IO_MODULE_NUM][MAX_IO_INDEX_NUM];
	BOOL bIOStatusOutput[MAX_IO_MODULE_NUM][MAX_IO_INDEX_NUM];

}IO_DATA;


class CDialogTabMIO : public CDialog
{
// Construction
public:
	CDialogTabMIO(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogTabMIO)
	enum { IDD = IDD_DIALOG_TAB_M_IO };
	CComboBox	m_cbChangeIOModule;
	//}}AFX_DATA
public:	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTabMIO)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	inline void CDialogTabMIO::SetWindowRect(CRect r) { m_wndRect = r; }
	void InitDlgSkin();

public:
	void SetIOStatus(int nInOut, int nNum);
	void InitIOModuleCombo();
	void InitLoadIOName();
	IO_DATA m_IODataSetting;
	CSystemData m_SysData;
	char *m_pcTitleInput[MAX_IO_INDEX_NUM];
	char *m_pcTitleOutput[MAX_IO_INDEX_NUM];	
	CStaticSkin	*m_pLabelIOInput[MAX_IO_INDEX_NUM];
	CStaticSkin	*m_pLabelIOOutput[MAX_IO_INDEX_NUM];
	
	void SetIOName(int nCurModuleIndex, BOOL bInit = FALSE);
	void InitIOButton();
	void InitTitleName();

	int m_nCurModuleIndex;

	BOOL m_bIOStatusInputOld[MAX_IO_MODULE_NUM][MAX_IO_INDEX_NUM];
	BOOL m_bIOStatusOutputOld[MAX_IO_MODULE_NUM][MAX_IO_INDEX_NUM];


	CRect			m_wndRect;
	CStaticSkin		*m_pLabelSkinTitle;
	
	CxSkinButton m_wOut[MAX_IO_INDEX_NUM];
	CxSkinButton m_wIn[MAX_IO_INDEX_NUM];

	CBmpRgn *pBmpRgn;
	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogTabMIO)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnIoOut0();
	afx_msg void OnIoOut1();
	afx_msg void OnIoOut2();
	afx_msg void OnIoOut3();
	afx_msg void OnIoOut4();
	afx_msg void OnIoOut5();
	afx_msg void OnIoOut6();
	afx_msg void OnIoOut7();
	afx_msg void OnIoOut8();
	afx_msg void OnIoOut9();
	afx_msg void OnIoOut10();
	afx_msg void OnIoOut11();
	afx_msg void OnIoOut12();
	afx_msg void OnIoOut13();
	afx_msg void OnIoOut14();
	afx_msg void OnIoOut15();	
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTABMIO_H__E7EF7678_83A4_441F_A67E_94E92CC0F599__INCLUDED_)
