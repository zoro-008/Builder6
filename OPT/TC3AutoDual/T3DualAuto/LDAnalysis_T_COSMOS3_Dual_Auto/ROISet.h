#if !defined(AFX_ROISET_H__B48ADB1D_4E5F_11D5_87DE_00105A6D3728__INCLUDED_)
#define AFX_ROISET_H__B48ADB1D_4E5F_11D5_87DE_00105A6D3728__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ROISet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CROISet window
#define CTL_RECT_NUM   9

  
class CROISet : public CWnd
{
private:
    int m_nScaleFactor;
public:
	CROISet();
    CROISet(int srw, int srh,CRect r);

public:
    CRect m_r;
	CRect m_sr[CTL_RECT_NUM];
	int   m_srtag[CTL_RECT_NUM];
	int   m_sel;
	int   m_srw,m_srh;
	int   m_ctlEnable;

public:

    void SetEnable(int t) { m_ctlEnable=t; } 
	void SetRectData(RECT r);
    void MakeSubRect(CRect r);
	void MoveOffset(int x, int y);
	void DrawRoi(CDC *pDC);
	void DrawRoi(CDC *pDC,int num);
	void DrawRoi(CPaintDC *pDC,int num);
	void DrawRoi(CPaintDC *pDC,char *buf);
	void DrawRoiEx(CDC *pDC);
	void UIMouseMove(CPoint p);
	void UIMouseMove2(CPoint p);
	void UIMouseLBDown(CPoint p);
	void UIMouseLBUp(CPoint p);
	void UIMouseRBDown();
	void DisplayRect(CDC *pDC,CRect r,COLORREF rgb);
	void DisplayEllipse(CDC *pDC,RECT r,COLORREF nColor,int nMode);

	inline void CROISet::SetScaleFactor(int nFactor) {m_nScaleFactor=nFactor;}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CROISet)
	//}}AFX_VIRTUAL

// Implementation
private:
	void InitData();

public:
	virtual ~CROISet();


protected:
	//{{AFX_MSG(CROISet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROISET_H__B48ADB1D_4E5F_11D5_87DE_00105A6D3728__INCLUDED_)
