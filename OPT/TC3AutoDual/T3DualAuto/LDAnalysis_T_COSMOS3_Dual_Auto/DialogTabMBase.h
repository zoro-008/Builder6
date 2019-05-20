#if !defined(AFX_DIALOGTABMBASE_H__DAA70E96_03E7_46D8_A3DF_B3EAB802DB5B__INCLUDED_)
#define AFX_DIALOGTABMBASE_H__DAA70E96_03E7_46D8_A3DF_B3EAB802DB5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogTabMBase.h : header file
//
#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\xskinbutton.h"
#include "..\\guisrc\\StaticSkin.h"
#include "MotionDefine.h"
#include "DialogTabMIO.h"
#include "DialogTabMMotion.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogTabMBase dialog

class CDialogTabMBase : public CDialog
{
// Construction
public:
	CDialogTabMBase(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogTabMBase)
	enum { IDD = IDD_DIALOG_TAB_M_BASE };
	
	//}}AFX_DATA
	//

	//20101217 Variable
public:
	
	int				m_nTabBtnClickedOld;
	CRect			m_wndRect;
	CDialogTabMIO	m_TabMIO;
	
	CxSkinButton	*m_pbtn[MAX_M_TAP_BTN_NUM];
	CxSkinButton	m_btnIO;
	CxSkinButton	m_btnMotion;

	CBmpRgn *pBmpRgn;
	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;

	CDialogTabMMotion *m_pwMotion;
	CDialogTabMIO	  *m_pwIO;


	//20101217 Function
public:
	void SelectWindow(int nTabNum);
	void InitTabDialog();
	void InitDlgSkin();
	void InitControls();

	//inline Function
	inline void CDialogTabMBase::SetWndRect(CRect r)   { m_wndRect=r;          }
	
		


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTabMBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogTabMBase)
	virtual BOOL OnInitDialog();
	afx_msg void OnTabBtnMotion();
	afx_msg void OnTabBtnIo();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTABMBASE_H__DAA70E96_03E7_46D8_A3DF_B3EAB802DB5B__INCLUDED_)
