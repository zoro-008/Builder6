#if !defined(AFX_ANGLEWINDOW_H__9ABA3E81_36C2_4485_813A_A2B7F7CE08D5__INCLUDED_)
#define AFX_ANGLEWINDOW_H__9ABA3E81_36C2_4485_813A_A2B7F7CE08D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AngleWindow.h : header file
//
#include "..\\guisrc\\LineProc.h"


/////////////////////////////////////////////////////////////////////////////
// CAngleWindow dialog
class CAngleWindow : public CDialog
{
// Construction
private:
	BITMAPINFOHEADER m_BitmapInfoHeader;
	int m_nWidth,m_nHeight;
	double m_nCx,m_nCy;
	int m_nDummy;
	int m_nMouseDown;
	CPoint m_cPrevPoint;
	int m_nPrevFlag;
	CRect m_cWndPos;
	CRect m_cWndMPos;
	int m_nDrawTag;
	int m_nRDir,m_nTDir;
	POINT m_cCenter;
	double m_nRMinute,m_nTMinute;
public:
	LINE_DATA_POS m_cSHp[41];
	LINE_DATA_POS m_cSHm[41];
	LINE_DATA_POS m_cSVp[41];
	LINE_DATA_POS m_cSVm[41];
	
	LINE_DATA_POS m_cLHp[4];
	LINE_DATA_POS m_cLHm[4];
	LINE_DATA_POS m_cLVp[4];
	LINE_DATA_POS m_cLVm[4];

	LINE_DATA_POS m_cLHpG[4];
	LINE_DATA_POS m_cLHmG[4];
	LINE_DATA_POS m_cLVpG[4];
	LINE_DATA_POS m_cLVmG[4];

	usrPOINT m_cMinuteCoordinate[240][320];
	usrPOINT m_cHsp,m_cHep,m_cVsp,m_cVep;
	double m_nScaleFactor;

	CAngleWindow(CWnd* pParent = NULL);   // standard constructor
    unsigned char *m_pBitmapBuffer;
	void BitmapInit();
	void DrawGrid(CPaintDC *dc);
	void DisplayGrid(CDC *memDC);
	void SettingCoordinate();
	void MakeCoordTable(int rDir,int tDir);
	void MakeGridCoord();
	void DisplayAngle(CDC *pDC,double xpos,double ypos);
	void MinuteToPixel(double rValue,double tValue,POINT *coord);
	void SetMinute(double rv,double tv);

	inline void CAngleWindow::SetWndPos(CRect r)          { m_cWndPos=r; m_cWndMPos=r;}
	inline void CAngleWindow::SetDrawTag(int nTag)        { m_nDrawTag=nTag ; }
	inline void CAngleWindow::SetDir(int nRDir,int nTDir) { m_nRDir=nRDir; m_nTDir=nTDir; }

// Dialog Data
	//{{AFX_DATA(CAngleWindow)
	enum { IDD = IDD_DIALOG_ANGLE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAngleWindow)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAngleWindow)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANGLEWINDOW_H__9ABA3E81_36C2_4485_813A_A2B7F7CE08D5__INCLUDED_)
