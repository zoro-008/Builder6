// PointRoi.cpp : implementation file
//

#include "stdafx.h"
#include "hvsystem.h"
#include "PointRoi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPointRoi

CPointRoi::CPointRoi()
{
	m_nScaleFactor=2;
	m_ctlEnable=0;
	m_cpt.x=100;
	m_cpt.y=100;
	m_sr.SetRect(m_cpt.x-50,m_cpt.y-50,m_cpt.x,m_cpt.y);
	m_r.SetRect(m_cpt.x-50,m_cpt.y-50,m_cpt.x+50,m_cpt.y+50);
}

CPointRoi::~CPointRoi()
{
}


BEGIN_MESSAGE_MAP(CPointRoi, CWnd)
	//{{AFX_MSG_MAP(CPointRoi)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPointRoi message handlers

void CPointRoi::SetPointData(POINT pt)
{
	m_cpt.x=pt.x;
	m_cpt.y=pt.y;
	m_sr.SetRect(m_cpt.x-50,m_cpt.y-50,m_cpt.x,m_cpt.y);
	m_r.SetRect(m_cpt.x-50,m_cpt.y-50,m_cpt.x+50,m_cpt.y+50);
}

void CPointRoi::DisplayRect(CDC *pDC,CRect r,COLORREF rgb)
{
	CPen pen,*oldPen;
	pen.CreatePen(PS_SOLID,1,rgb);
	oldPen=pDC->SelectObject(&pen);
	POINT tc_pt,bc_pt,lc_pt,rc_pt;

    pDC->MoveTo(r.left/m_nScaleFactor,r.top/m_nScaleFactor);
	pDC->LineTo(r.right/m_nScaleFactor,r.top/m_nScaleFactor);
	pDC->LineTo(r.right/m_nScaleFactor,r.bottom/m_nScaleFactor);
	pDC->LineTo(r.left/m_nScaleFactor,r.bottom/m_nScaleFactor);
	pDC->LineTo(r.left/m_nScaleFactor,r.top/m_nScaleFactor);

	bc_pt.x=tc_pt.x=(r.left+r.right)/2;
	tc_pt.y=r.top;
	bc_pt.y=r.bottom;
	lc_pt.x=r.left;
	rc_pt.y=lc_pt.y=(r.top+r.bottom)/2;
	rc_pt.x=r.right;

	DisplayLine(pDC,tc_pt,bc_pt,rgb);
	DisplayLine(pDC,lc_pt,rc_pt,rgb);
	

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();
}

void CPointRoi::DisplayLine(CDC *pDC,POINT spt,POINT ept,COLORREF rgb)
{
	CPen pen,*oldPen;
	pen.CreatePen(PS_SOLID,1,rgb);
	oldPen=pDC->SelectObject(&pen);

		
	spt.x/=m_nScaleFactor;
	spt.y/=m_nScaleFactor;
	ept.x/=m_nScaleFactor;
	ept.y/=m_nScaleFactor;

    pDC->MoveTo(spt);
	pDC->LineTo(ept);

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();
	return;
}

void CPointRoi::DrawRoi(CPaintDC *pDC,int num)
{

	if(m_ctlEnable==0) {
		DisplayRect(pDC,m_r,RGB(0,64,128));
		DisplayRect(pDC,m_sr,RGB(0,64,128));
	}
	else 
	{
		DisplayRect(pDC,m_r,RGB(0,255,0));
		DisplayRect(pDC,m_sr,RGB(255,0,0));
	}
	return;
}


void CPointRoi::UIMouseLBDown(CPoint p)
{
	if(m_ctlEnable==0)
	{
		if(m_sr.PtInRect(p)!=0)
		{
			SetCursor(LoadCursor(NULL,IDC_SIZEALL));
			m_ctlEnable=1;
		}
	}
	return;
}

void CPointRoi::UIMouseLBUp(CPoint p)
{
  SetCursor(LoadCursor(NULL,IDC_ARROW));
  return;
}

void CPointRoi::UIMouseRBDown()
{
	if(m_ctlEnable==1)
	{
		SetCursor(LoadCursor(NULL,IDC_ARROW));
		m_ctlEnable=0;
	}
	return;
}


void CPointRoi::UIMouseMove(CPoint p)
{
	CPoint cp;
	int  ddx,ddy;

    if(m_ctlEnable==1)
	{
		if(m_sr.PtInRect(p)!=0)
		{
			SetCursor(LoadCursor(NULL,IDC_SIZEALL));

			cp.x=(m_sr.left+m_sr.right)/2;
			cp.y=(m_sr.top+m_sr.bottom)/2;

			ddx=(cp.x-p.x);
			ddy=(cp.y-p.y);

			m_cpt.x-=ddx;
			m_cpt.y-=ddy;

			m_r.SetRect(m_cpt.x-50,m_cpt.y-50,m_cpt.x+50,m_cpt.y+50);
			m_sr.SetRect(m_cpt.x-50,m_cpt.y-50,m_cpt.x,m_cpt.y);
		}
	}
	return;
}

