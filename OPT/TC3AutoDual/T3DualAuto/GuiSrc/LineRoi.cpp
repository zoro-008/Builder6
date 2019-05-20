// LineRoi.cpp : implementation file
//

#include "stdafx.h"
#include "LineRoi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineRoi

CLineRoi::CLineRoi()
{
	m_nScaleFactor=2;
    InitData();
	
	m_spt.x=100;
	m_spt.y=100;
	m_ept.x=200;
	m_ept.y=100;

    MakeSubRect(m_spt,m_ept);

}

CLineRoi::CLineRoi(int srw, int srh, POINT spt,POINT ept)
{

	InitData();
	m_srw=srw;
	m_srh=srh;

    memcpy(&m_spt,&spt,sizeof(POINT));
	memcpy(&m_ept,&ept,sizeof(POINT));

    // Control Point 영역을 만든다.
    MakeSubRect(m_spt,m_ept);

}

void CLineRoi::SetPointData(POINT spt,POINT ept)
{
    memcpy(&m_spt,&spt,sizeof(POINT));
	memcpy(&m_ept,&ept,sizeof(POINT));
	InitData();

	MakeSubRect(spt,ept);
}
void CLineRoi::InitData()
{
	m_srw=8;
	m_srh=8;
    m_ctlEnable=0;
	m_sel=CTL_LINE_NUM;
	for(int i=0; i<CTL_LINE_NUM; i++)
		m_srtag[i]=0;
}


CLineRoi::~CLineRoi()
{
}



BEGIN_MESSAGE_MAP(CLineRoi, CWnd)
	//{{AFX_MSG_MAP(CLineRoi)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLineRoi message handlers
void CLineRoi::MakeSubRect(POINT spt,POINT ept)
{
	m_sr[0].SetRect(spt.x-m_srw     ,spt.y-m_srh     ,spt.x+m_srw     ,spt.y+m_srh);
	m_sr[1].SetRect(ept.x-m_srw     ,ept.y-m_srh     ,ept.x+m_srw     ,ept.y+m_srh);

	m_sr[2].SetRect((spt.x+ept.x)/2-m_srw,
		            (spt.y+ept.y)/2-m_srh,
					(spt.x+ept.x)/2+m_srw,
					(spt.y+ept.y)/2+m_srh);

}

void CLineRoi::DisplayRect(CDC *pDC,CRect r,COLORREF rgb)
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

void CLineRoi::DisplayLine(CDC *pDC,POINT spt,POINT ept,COLORREF rgb)
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
}


void CLineRoi::DrawRoi(CDC *pDC)
{
	int i;
	MakeSubRect(m_spt,m_ept);
	DisplayLine(pDC,m_spt,m_ept,RGB(255,0,0));
	for(i=0; i<CTL_LINE_NUM; i++)  
	{
		if(i<CTL_LINE_NUM-1)
			DisplayRect(pDC,m_sr[i],RGB(0,255,0));
		else {
			if(m_ctlEnable==0)   DisplayRect(pDC,m_sr[i],RGB(255,0,0));
			else                 DisplayRect(pDC,m_sr[i],RGB(0,0,255));
		}
	}
}

void CLineRoi::DrawRoi(CDC *pDC,int num)
{

	int i;

	
	MakeSubRect(m_spt,m_ept);
	DisplayLine(pDC,m_spt,m_ept,RGB(255,0,0));

	if(m_ctlEnable==0)   DisplayRect(pDC,m_sr[CTL_LINE_NUM-1],RGB(255,0,0));
	else
	{
		DisplayRect(pDC,m_sr[CTL_LINE_NUM-1],RGB(0,0,255));
		for(i=0; i<CTL_LINE_NUM-1; i++)  
			DisplayRect(pDC,m_sr[i],RGB(0,255,0));
	}
}

void CLineRoi::DrawRoi(CPaintDC *pDC,int num)
{
	int i;


	DisplayLine(pDC,m_spt,m_ept,RGB(255,0,0));
	

	if(m_ctlEnable==0)   DisplayRect(pDC,m_sr[CTL_LINE_NUM-1],RGB(255,0,0));
	else 
	{
		DisplayRect(pDC,m_sr[CTL_LINE_NUM-1],RGB(0,0,255));
		for(i=0; i<CTL_LINE_NUM-1; i++)  
			DisplayRect(pDC,m_sr[i],RGB(0,255,0));
	}
}

void CLineRoi::DrawRoi(CPaintDC *pDC,char *buf)
{
	int i;


	DisplayLine(pDC,m_spt,m_ept,RGB(255,0,0));
	

	if(m_ctlEnable==0)  
		DisplayRect(pDC,m_sr[CTL_LINE_NUM-1],RGB(255,0,0));
	else     
	{
		DisplayRect(pDC,m_sr[CTL_LINE_NUM-1],RGB(0,0,255));

		for(i=0; i<CTL_LINE_NUM-1; i++)  
			DisplayRect(pDC,m_sr[i],RGB(0,255,0));
	}
}


void CLineRoi::UIMouseLBDown(CPoint p)
{
  int i;

  if(m_ctlEnable==1) 
  {
	  for(i=0; i<CTL_LINE_NUM-1; i++)
	  {
		  if(m_srtag[i]==0)
		  {
			  if(m_sr[i].PtInRect(p)!=0)
			  {
				m_srtag[i]=1;
				m_sel=i;
				SetEnable(1);
				switch(m_sel)
				{
				case 0:
					SetCursor(LoadCursor(NULL,IDC_SIZEALL));
					break;
				case 1:
					SetCursor(LoadCursor(NULL,IDC_SIZEALL));					
					break;
				}
		        break;
			  }
		  }
	  }
  }
  if(m_srtag[CTL_LINE_NUM-1]==0)
  {
	  if(m_sr[CTL_LINE_NUM-1].PtInRect(p)!=0)
	  {
		m_srtag[CTL_LINE_NUM-1]=1;
		m_sel=CTL_LINE_NUM-1;
		SetCursor(LoadCursor(NULL,IDC_SIZEALL));
		SetEnable(1);
	  }
  }

}

void CLineRoi::UIMouseLBUp(CPoint p)
{
  int i;


  for(i=0; i<CTL_LINE_NUM; i++)
  {
		m_srtag[i]=0;
		m_sel=CTL_LINE_NUM;
  }
  SetCursor(LoadCursor(NULL,IDC_ARROW));

}

void CLineRoi::UIMouseRBDown()
{
  int i;

  for(i=0; i<CTL_LINE_NUM; i++)
  {
		m_srtag[i]=0;
		m_sel=CTL_LINE_NUM;
  }
  SetCursor(LoadCursor(NULL,IDC_ARROW));

  m_ctlEnable=0;
}


void CLineRoi::UIMouseMove(CPoint p)
{
	CPoint cp;


	int ddx,ddy;

    if(m_ctlEnable==1)
	{
		switch(m_sel)
		{
		case 0:
			SetCursor(LoadCursor(NULL,IDC_SIZEALL));
			m_spt.x+=(p.x-m_spt.x);
			m_spt.y+=(p.y-m_spt.y);
			break;
		case 1:
			SetCursor(LoadCursor(NULL,IDC_SIZEALL));
			m_ept.x+=(p.x-m_ept.x);
			m_ept.y+=(p.y-m_ept.y);
			break;
		case 2:
			SetCursor(LoadCursor(NULL,IDC_SIZEALL));
			ddx=(p.x-(m_ept.x+m_spt.x)/2);
			ddy=(p.y-(m_spt.y+m_ept.y)/2);

			m_spt.x+=ddx;
			m_ept.x+=ddx;
			m_spt.y+=ddy;
			m_ept.y+=ddy;

			break;

		}
		if(m_sel <CTL_LINE_NUM)  MakeSubRect(m_spt,m_ept);
	}
}

