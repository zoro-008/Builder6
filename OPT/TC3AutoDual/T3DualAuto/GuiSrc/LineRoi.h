#if !defined(AFX_LINEROI_H__C19650AB_295B_4D1F_8A55_6ED4A4AB6E58__INCLUDED_)
#define AFX_LINEROI_H__C19650AB_295B_4D1F_8A55_6ED4A4AB6E58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineRoi.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLineRoi window
#define CTL_LINE_NUM 3

class CLineRoi : public CWnd
{
private:
    int m_nScaleFactor;
public:
	CLineRoi();
    CLineRoi(int srw, int srh,POINT spt,POINT ept);
	

public:
    POINT m_spt,m_ept;
	
	CRect m_sr[CTL_LINE_NUM];
	int   m_srtag[CTL_LINE_NUM];
	int   m_sel;
	int   m_srw,m_srh;
	int   m_ctlEnable;

public:

    void SetEnable(int t) { m_ctlEnable=t; } 
	void SetPointData(POINT spt,POINT ept);
    void MakeSubRect(POINT spt,POINT ept);
	void DrawRoi(CDC *pDC);
	void DrawRoi(CDC *pDC,int num);
	void DrawRoi(CPaintDC *pDC,int num);
	void DrawRoi(CPaintDC *pDC,char *buf);
	void UIMouseMove(CPoint p);
	void UIMouseLBDown(CPoint p);
	void UIMouseLBUp(CPoint p);
	void UIMouseRBDown();
	void DisplayRect(CDC *pDC,CRect r,COLORREF rgb);
	void DisplayLine(CDC *pDC,POINT spt,POINT ept,COLORREF rgb);


	inline void CLineRoi::SetScaleFactor(int nFactor) {m_nScaleFactor=nFactor;}

	// Generated message map functions
private:
	void InitData();

public:
	virtual ~CLineRoi();
protected:
	//{{AFX_MSG(CLineRoi)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINEROI_H__C19650AB_295B_4D1F_8A55_6ED4A4AB6E58__INCLUDED_)
