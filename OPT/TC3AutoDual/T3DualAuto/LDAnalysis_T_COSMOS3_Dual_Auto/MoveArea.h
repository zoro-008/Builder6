#if !defined(AFX_MOVEAREA_H__39B97519_01F6_41DC_B83B_71D52ED4E6AC__INCLUDED_)
#define AFX_MOVEAREA_H__39B97519_01F6_41DC_B83B_71D52ED4E6AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MoveArea.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMoveArea dialog
#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\StaticSkin.h"
#include "..\\guisrc\\xSkinButton.h"
#include "xSkinButtonEx.h"

class CMoveArea : public CDialog
{
// Construction
private:
	CRect m_wndRect;
	CBmpRgn *pBmpRgn;
	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;
public:
	CxSkinButtonEx *m_pwButton[5];
	CxSkinButton *m_wSysMenu;

public:
	CMoveArea(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMoveArea)
	enum { IDD = IDD_DIALOG_MOVE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMoveArea)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMoveArea)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnButtonup();
	afx_msg void OnButtondown();
	afx_msg void OnButtonright();
	afx_msg void OnButtonleft();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtoncenter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOVEAREA_H__39B97519_01F6_41DC_B83B_71D52ED4E6AC__INCLUDED_)
