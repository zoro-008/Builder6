#if !defined(AFX_POINTROI_H__FCCD39AA_ADEB_4974_8DDC_1FE984F496BD__INCLUDED_)
#define AFX_POINTROI_H__FCCD39AA_ADEB_4974_8DDC_1FE984F496BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PointRoi.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPointRoi window

class CPointRoi : public CWnd
{
private:
    int m_nScaleFactor;

// Construction
public:
	CPointRoi();

public:
    POINT m_cpt;
	
	CRect m_r;
	CRect m_sr;
	int   m_ctlEnable;

public:

    void SetEnable(int t) { m_ctlEnable=t; } 
	void SetPointData(POINT pt);
	void DrawRoi(CPaintDC *pDC,int num);
	void UIMouseMove(CPoint p);
	void UIMouseLBDown(CPoint p);
	void UIMouseLBUp(CPoint p);
	void UIMouseRBDown();
	void DisplayRect(CDC *pDC,CRect r,COLORREF rgb);
    void DisplayLine(CDC *pDC,POINT spt,POINT ept,COLORREF rgb);
	inline void CPointRoi::SetScaleFactor(int nFactor) {m_nScaleFactor=nFactor;}

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPointRoi)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPointRoi();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPointRoi)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POINTROI_H__FCCD39AA_ADEB_4974_8DDC_1FE984F496BD__INCLUDED_)
