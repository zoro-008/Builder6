#if !defined(AFX_CURRENTDATA_H__803AB8A4_302B_4724_99B9_5C66735275A6__INCLUDED_)
#define AFX_CURRENTDATA_H__803AB8A4_302B_4724_99B9_5C66735275A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CurrentData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCurrentData dialog
#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\StaticSkin.h"

class CCurrentData : public CDialog
{
// Construction
private:
	CRect m_wndRect;
// Construction
	CBmpRgn *pBmpRgn;
	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;
	int  m_nAppType;
	int m_nAppColumnNum;
	COLORREF m_nListColor;
	
public:
	CCurrentData(CWnd* pParent = NULL);   // standard constructor
	char m_cBuf[4][4][256];
	int m_nSubItemNum[4];
	CListCtrl m_cListGeneral1;
	CListCtrl m_cListGeneral2;
	CListCtrl m_cListPeak;
	CListCtrl m_cListRim;

	void InitListCtrlCols();
	void InitDataField();
	void SetListData(int listnum,int nSubItem,char *pText);
	inline void CCurrentData::SetWindowRect(CRect nRect)  { m_wndRect=nRect;      }
	inline void CCurrentData::GetWindowRect(CRect *pRect) { *pRect=m_wndRect;     }

// Dialog Data
	//{{AFX_DATA(CCurrentData)
	enum { IDD = IDD_DIALOG_TABCDATA };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurrentData)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCurrentData)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURRENTDATA_H__803AB8A4_302B_4724_99B9_5C66735275A6__INCLUDED_)
