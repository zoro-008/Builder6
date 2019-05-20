#if !defined(AFX_DATADISPLAY_H__837BD0E4_64BC_47F6_871B_77C7BB62BB58__INCLUDED_)
#define AFX_DATADISPLAY_H__837BD0E4_64BC_47F6_871B_77C7BB62BB58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataDisplay.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataDisplay dialog
#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\StaticSkin.h"


#define MAX_DISPLAY_NUM 1000

class CDataDisplay : public CDialog
{
private:
	CRect m_wndRect;
// Construction
	CBmpRgn *pBmpRgn;
	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;
	int  m_nAppType;
	int m_nAppColumnNum;
	COLORREF m_nListColor;
// Construction
public:
	int m_nListNumber;
	CListCtrl m_lcData;
	char m_strCurrentTime[256];
	SYSTEMTIME m_nSysTime;

	CDataDisplay(CWnd* pParent = NULL);   // standard constructor
    void InitListCtrlCols();
	void AddListData(char *pText,int nSubItem);
	void DeleteAllList();

	inline void CDataDisplay::SetAppType(int nType)       { m_nAppType=nType;     }
	inline void CDataDisplay::SetAppColumnNum(int nNum)   { m_nAppColumnNum=nNum; }
	inline void CDataDisplay::SetWindowRect(CRect nRect)  { m_wndRect=nRect;      }
	inline void CDataDisplay::GetWindowRect(CRect *pRect) { *pRect=m_wndRect;     }

// Dialog Data
	//{{AFX_DATA(CDataDisplay)
	enum { IDD = IDD_DIALOG_TABDATA };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataDisplay)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataDisplay)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg void OnCustomdrawList(NMHDR*,LRESULT*);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATADISPLAY_H__837BD0E4_64BC_47F6_871B_77C7BB62BB58__INCLUDED_)
