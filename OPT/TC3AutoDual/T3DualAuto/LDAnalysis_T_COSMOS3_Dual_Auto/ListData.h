#if !defined(AFX_LISTDATA_H__5E15E447_5900_42EF_ABC8_D41ADDC505CF__INCLUDED_)
#define AFX_LISTDATA_H__5E15E447_5900_42EF_ABC8_D41ADDC505CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListData dialog

#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\StaticSkin.h"

class CListData : public CDialog
{
private:
	CRect m_wndRect;
// Construction
	CBmpRgn *pBmpRgn;
	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;
public:
	int m_nListNumber;

	CListData(CWnd* pParent = NULL);   // standard constructor
	CStaticSkin m_wTitle[5];
	CStaticSkin m_wData[5];

	void InitListCtrlCols();
	void AddListData(int nRow,int nCols,char *pText);
	void AddListDataII(int nCols, long nNum);
	void DeleteAllListData();
	void MakeDisplayDataWindow();
	void DisplayExtText(int nIdx,char *buf);
    inline void CListData::SetWindowRect(CRect nRect) { m_wndRect=nRect;}
	inline void CListData::GetWindowRect(CRect *pRect){ *pRect=m_wndRect;}
// Dialog Data
	//{{AFX_DATA(CListData)
	enum { IDD = IDD_LIST_DATA };
	CListCtrl	m_lcData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListData)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CListData)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTDATA_H__5E15E447_5900_42EF_ABC8_D41ADDC505CF__INCLUDED_)
