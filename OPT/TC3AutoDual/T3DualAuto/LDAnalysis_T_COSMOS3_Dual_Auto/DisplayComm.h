#if !defined(AFX_DISPLAYCOMM_H__F114B4D9_B27D_454C_AE28_8BB676D83A30__INCLUDED_)
#define AFX_DISPLAYCOMM_H__F114B4D9_B27D_454C_AE28_8BB676D83A30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DisplayComm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDisplayComm dialog
#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\StaticSkin.h"
#include "..\\guisrc\\xSkinButton.h"


class CDisplayComm : public CDialog
{
// Construction
private:
	CRect m_wndRect;
	CBmpRgn *pBmpRgn;
	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;
	long m_nMsgIndex;

public:
	int m_nGcapFlag;
	CDisplayComm(CWnd* pParent = NULL);   // standard constructor
	CxSkinButton *m_wSysMenu;

	void InitListCtrlCols();
	void AddListData(char *pText);

	void DeleteAllList();

	inline void CDisplayComm::SetWndRect(CRect r)  { m_wndRect=r;  }
	inline void CDisplayComm::SetWndRect(int nL,int nT,int nR,int nB){ m_wndRect.SetRect(nL,nT,nR,nB); }
	inline void CDisplayComm::GetWndRect(CRect *r) { *r=m_wndRect; }

// Dialog Data
	//{{AFX_DATA(CDisplayComm)
	enum { IDD = IDD_DISPLAY_COMM };
	CListCtrl	m_lcListCommEvent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDisplayComm)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDisplayComm)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPLAYCOMM_H__F114B4D9_B27D_454C_AE28_8BB676D83A30__INCLUDED_)
