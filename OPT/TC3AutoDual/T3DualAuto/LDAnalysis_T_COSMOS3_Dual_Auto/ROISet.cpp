// ROISet.cpp : implementation file
//

#include "stdafx.h"
#include "ROISet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CROISet

CROISet::CROISet()
{
	m_nScaleFactor=1;
    InitData();
	m_r.SetRect(10,100,210,300);
    MakeSubRect(m_r);

}

CROISet::CROISet(int srw, int srh, CRect r)
{

	InitData();
	m_srw=srw;
	m_srh=srh;

	m_r=r; // 멤버 변수에 매개변수를 복사한다.
    // Control Point 영역을 만든다.
    MakeSubRect(m_r);

}

void CROISet::SetRectData(RECT r)
{
	InitData();
	m_r.SetRect(r.left,r.top,r.right,r.bottom);
	MakeSubRect(m_r);
}
void CROISet::InitData()
{
	m_srw=3;
	m_srh=3;
    m_ctlEnable=0;
	m_sel=CTL_RECT_NUM;
	for(int i=0; i<CTL_RECT_NUM; i++)
		m_srtag[i]=0;
}


CROISet::~CROISet()
{
}


BEGIN_MESSAGE_MAP(CROISet, CWnd)
	//{{AFX_MSG_MAP(CROISet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CROISet message handlers

void CROISet::MakeSubRect(CRect r)
{
	int rw,rh;
    
	rw=r.Width();
	rh=r.Height();
    
	m_sr[0].SetRect(r.left-m_srw     ,r.top-m_srh     ,r.left+m_srw     ,r.top+m_srh);//0
	m_sr[1].SetRect(r.left+rw/2-m_srw,r.top-m_srh     ,r.left+rw/2+m_srw,r.top+m_srh);
	m_sr[2].SetRect(r.right-m_srw    ,r.top-m_srh     ,r.right+m_srw    ,r.top+m_srh); //2
	m_sr[3].SetRect(r.right-m_srw    ,r.top+rh/2-m_srh,r.right+m_srw    ,r.top+rh/2+m_srh);
	m_sr[4].SetRect(r.right-m_srw    ,r.bottom-m_srh  ,r.right+m_srw    ,r.bottom+m_srh); //4
	m_sr[5].SetRect(r.left+rw/2-m_srw,r.bottom-m_srh  ,r.left+rw/2+m_srw,r.bottom+m_srh);
	m_sr[6].SetRect(r.left-m_srw     ,r.bottom-m_srh  ,r.left+m_srw     ,r.bottom+m_srh);//6
	m_sr[7].SetRect(r.left-m_srw     ,r.top+rh/2-m_srh,r.left+m_srw     ,r.top+rh/2+m_srh);
	m_sr[8].SetRect(r.left+rw/2-m_srw*2-2,r.top+rh/2-m_srh*2-2,
					r.left+rw/2+m_srw*2+2,r.top+rh/2+m_srh*2+2);

//	m_sr[8].SetRect(r.left+m_srw+2,r.top+m_srh+2,r.right-m_srw-2,r.bottom-m_srh-2);
}
void CROISet::DisplayRect(CDC *pDC,CRect r,COLORREF rgb)
{
	CPen pen,*oldPen;
	pen.CreatePen(PS_SOLID,1,rgb);
	oldPen=pDC->SelectObject(&pen);

		
    pDC->MoveTo(r.left/m_nScaleFactor,r.top/m_nScaleFactor);
	pDC->LineTo(r.right/m_nScaleFactor,r.top/m_nScaleFactor);
	pDC->LineTo(r.right/m_nScaleFactor,r.bottom/m_nScaleFactor);
	pDC->LineTo(r.left/m_nScaleFactor,r.bottom/m_nScaleFactor);
	pDC->LineTo(r.left/m_nScaleFactor,r.top/m_nScaleFactor);

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();
}

void CROISet::DisplayEllipse(CDC *pDC,RECT r,COLORREF nColor,int nMode)
{
	CPen pen,*oldPen;
	LOGBRUSH nLogBrush;
    CBrush brush,*oldbrush;
   
	nLogBrush.lbColor=nColor;
	nLogBrush.lbHatch=HS_VERTICAL;
	nLogBrush.lbStyle=BS_NULL;

	pen.CreatePen(PS_SOLID,1,nColor);
	oldPen=pDC->SelectObject(&pen);

	brush.CreateBrushIndirect(&nLogBrush);
	oldbrush=pDC->SelectObject(&brush);


    r.bottom+=1;
	r.right+=1;

	if(nMode==2)
	{
		r.left/=nMode;
		r.top/=nMode;
		r.right/=nMode;
		r.bottom/=nMode;
	}

	pDC->Ellipse(&r);

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();

	pDC->SelectObject(oldbrush);
	brush.DeleteObject();
}


void CROISet::DrawRoi(CDC *pDC)
{
	int i;
	MakeSubRect(m_r);
	DisplayRect(pDC,m_r,RGB(255,0,0));
	for(i=0; i<CTL_RECT_NUM; i++)  
	{
		if(i<CTL_RECT_NUM-1)
			DisplayRect(pDC,m_sr[i],RGB(0,255,0));
		else {
			if(m_ctlEnable==0)   DisplayRect(pDC,m_sr[i],RGB(0,0,64));
			else                 DisplayRect(pDC,m_sr[i],RGB(0,0,255));
		}
	}
}

void CROISet::DrawRoiEx(CDC *pDC)
{
	int i;
	MakeSubRect(m_r);
	DisplayRect(pDC,m_r,RGB(255,255,0));
	DisplayEllipse(pDC,m_r,RGB(0,255,0),m_nScaleFactor);

	if(m_ctlEnable==0)   DisplayRect(pDC,m_sr[CTL_RECT_NUM-1],RGB(0,64,128));
	else
	{
		DisplayRect(pDC,m_sr[CTL_RECT_NUM-1],RGB(0,0,255));
		for(i=0; i<CTL_RECT_NUM-1; i++)  
			DisplayRect(pDC,m_sr[i],RGB(0,255,0));
	}
}

void CROISet::DrawRoi(CDC *pDC,int num)
{

	int i;
    char buf[16];
	COLORREF oldColor;
	
	MakeSubRect(m_r);
	DisplayRect(pDC,m_r,RGB(255,0,0));

    sprintf(buf,"%1d",num);
	pDC->SetBkMode(TRANSPARENT);
	oldColor=pDC->SetTextColor(RGB(255,255,0));
	pDC->TextOut((m_r.left+m_r.Width()/2-4)/m_nScaleFactor,
		         (m_r.top+m_r.Height()/2-8)/m_nScaleFactor,buf);

	if(m_ctlEnable==0)   DisplayRect(pDC,m_sr[CTL_RECT_NUM-1],RGB(0,64,128));
	else
	{
		DisplayRect(pDC,m_sr[CTL_RECT_NUM-1],RGB(0,0,255));
		for(i=0; i<CTL_RECT_NUM-1; i++)  
			DisplayRect(pDC,m_sr[i],RGB(0,255,0));
	}
	pDC->SetTextColor(oldColor);
}

void CROISet::DrawRoi(CPaintDC *pDC,int num)
{
	int i;
    char buf[16];
	COLORREF oldColor;

	DisplayRect(pDC,m_r,RGB(255,0,0));
	
    sprintf(buf,"%1d",num);
	pDC->SetBkMode(TRANSPARENT);
	oldColor=pDC->SetTextColor(RGB(255,255,0));
	pDC->TextOut((m_r.left+m_r.Width()/2-4)/m_nScaleFactor,
		         (m_r.top+m_r.Height()/2-8)/m_nScaleFactor,buf);

	if(m_ctlEnable==0)   DisplayRect(pDC,m_sr[CTL_RECT_NUM-1],RGB(0,64,128));
	else 
	{
		DisplayRect(pDC,m_sr[CTL_RECT_NUM-1],RGB(0,0,255));
		for(i=0; i<CTL_RECT_NUM-1; i++)  
			DisplayRect(pDC,m_sr[i],RGB(0,255,0));
	}
	pDC->SetTextColor(oldColor);
}

void CROISet::DrawRoi(CPaintDC *pDC,char *buf)
{
	int i;
	COLORREF oldColor;

	DisplayRect(pDC,m_r,RGB(255,0,0));
	
	pDC->SetBkMode(TRANSPARENT);
	oldColor=pDC->SetTextColor(RGB(200,200,0));
	pDC->TextOut((m_r.left+m_r.Width()/2-8)/m_nScaleFactor,
		         (m_r.top+m_r.Height()/2+10)/m_nScaleFactor,buf);
	if(m_ctlEnable==0)  
		DisplayRect(pDC,m_sr[CTL_RECT_NUM-1],RGB(0,64,128));
	else     
	{
		DisplayRect(pDC,m_sr[CTL_RECT_NUM-1],RGB(0,0,255));

		for(i=0; i<CTL_RECT_NUM-1; i++)  
			DisplayRect(pDC,m_sr[i],RGB(0,255,0));
	}
	pDC->SetTextColor(oldColor);
}


void CROISet::UIMouseLBDown(CPoint p)
{
  int i;

  if(m_ctlEnable==1) 
  {
	  for(i=0; i<CTL_RECT_NUM-1; i++)
	  {
		  if(m_srtag[i]==0)
		  {
			  if(m_sr[i].PtInRect(p)!=0)
			  {
				m_srtag[i]=1;
				m_sel=i;
				switch(m_sel)
				{
				case 0:
					SetCursor(LoadCursor(NULL,IDC_SIZENWSE));
					break;
				case 1:
					SetCursor(LoadCursor(NULL,IDC_SIZENS));					
					break;
				case 2:
					SetCursor(LoadCursor(NULL,IDC_SIZENESW));
					break;
				case 3:
					SetCursor(LoadCursor(NULL,IDC_SIZEWE));					
					break;
				case 4:
					SetCursor(LoadCursor(NULL,IDC_SIZENWSE));
					break;
				case 5:
					SetCursor(LoadCursor(NULL,IDC_SIZENS));
					break;
				case 6:
					SetCursor(LoadCursor(NULL,IDC_SIZENESW));
					break;
				case 7:
					SetCursor(LoadCursor(NULL,IDC_SIZEWE));
					break;
				}
			  break;
			  }
		  }
	  }
  }

  if(m_srtag[8]==0)
  {
	  if(m_sr[8].PtInRect(p)!=0)
	  {
		m_srtag[8]=1;
		m_sel=8;
		SetCursor(LoadCursor(NULL,IDC_SIZEALL));
		SetEnable(1);
	  }
  }
}

void CROISet::UIMouseLBUp(CPoint p)
{
  int i;


  for(i=0; i<CTL_RECT_NUM; i++)
  {
		m_srtag[i]=0;
		m_sel=CTL_RECT_NUM;
  }
  SetCursor(LoadCursor(NULL,IDC_ARROW));

}

void CROISet::UIMouseRBDown()
{
  int i;

  if(m_ctlEnable==1)
  {
	  for(i=0; i<CTL_RECT_NUM; i++)
	  {
			m_srtag[i]=0;
			m_sel=CTL_RECT_NUM;
	  }
	  SetCursor(LoadCursor(NULL,IDC_ARROW));

	  m_ctlEnable=0;
  }
}

void CROISet::MoveOffset(int x,int y)
{

	m_r.OffsetRect(x,y);
    MakeSubRect(m_r);
}

void CROISet::UIMouseMove(CPoint p)
{
	CPoint cp;


    if(m_ctlEnable==1)
	{
		switch(m_sel)
		{
		case 0: //
			SetCursor(LoadCursor(NULL,IDC_SIZENWSE));
			m_r.left+=(p.x-m_r.left);
			m_r.top+=(p.y-m_r.top);
			break;
		case 1:
			SetCursor(LoadCursor(NULL,IDC_SIZENS));
			m_r.top+=(p.y-m_r.top);		
			break;
		case 2: //
			SetCursor(LoadCursor(NULL,IDC_SIZENESW));
			m_r.right+=(p.x-m_r.right);
			m_r.top+=(p.y-m_r.top);
			break;
		case 3:
			SetCursor(LoadCursor(NULL,IDC_SIZEWE));
			m_r.right+=(p.x-m_r.right);
			break;
		case 4:
			SetCursor(LoadCursor(NULL,IDC_SIZENWSE));
			m_r.right+=(p.x-m_r.right);
			m_r.bottom+=(p.y-m_r.bottom);

			break;
		case 5:
			SetCursor(LoadCursor(NULL,IDC_SIZENS));
			m_r.bottom+=(p.y-m_r.bottom);
			break;
		case 6:
			SetCursor(LoadCursor(NULL,IDC_SIZENESW));
			m_r.left+=(p.x-m_r.left);
			m_r.bottom+=(p.y-m_r.bottom);
			break;
		case 7: 
			SetCursor(LoadCursor(NULL,IDC_SIZEWE));
			m_r.left+=(p.x-m_r.left);
			break;
		case 8:
			SetCursor(LoadCursor(NULL,IDC_SIZEALL));
			cp.x=(m_r.right + m_r.left)/2;
			cp.y=(m_r.bottom + m_r.top)/2;
			m_r.OffsetRect(p.x-cp.x, p.y-cp.y);
			break;
		}
		if(m_sel <CTL_RECT_NUM)  MakeSubRect(m_r);
	}
}


void CROISet::UIMouseMove2(CPoint p)
{
	CPoint cp;
	int delta1,delta2;

    if(m_ctlEnable==1)
	{
		switch(m_sel)
		{
		case 0: //
			SetCursor(LoadCursor(NULL,IDC_SIZENWSE));
			delta1=(p.x-m_r.left);
			delta2=(p.y-m_r.top);
			if(delta1 > delta2) 
			{

				m_r.left+=delta1;
				m_r.top+=delta1;
			}
			else
			{
				m_r.left+=delta2;
				m_r.top+=delta2;
			}
			break;
		case 1:
			SetCursor(LoadCursor(NULL,IDC_SIZENS));
			m_r.top+=(p.y-m_r.top);		
			break;
		case 2: //
			SetCursor(LoadCursor(NULL,IDC_SIZENESW));
			delta1=(p.x-m_r.right);
			m_r.right+=delta1;
			m_r.top-=delta1;
			break;
		case 3:
			SetCursor(LoadCursor(NULL,IDC_SIZEWE));
			m_r.right+=(p.x-m_r.right);
			break;
		case 4:
			SetCursor(LoadCursor(NULL,IDC_SIZENWSE));
			delta1=(p.x-m_r.right);
			delta2=(p.y-m_r.bottom);
			if(delta1 > delta2)
			{
				m_r.right+=delta1;
				m_r.bottom+=delta1;
			}
			else
			{
				m_r.right+=delta2;
				m_r.bottom+=delta2;
			}
			break;
		case 5:
			SetCursor(LoadCursor(NULL,IDC_SIZENS));
			m_r.bottom+=(p.y-m_r.bottom);
			break;
		case 6:
			SetCursor(LoadCursor(NULL,IDC_SIZENESW));
			delta1=(p.x-m_r.left);
			m_r.left+=delta1;
			m_r.bottom-=delta1;
			break;
		case 7: 
			SetCursor(LoadCursor(NULL,IDC_SIZEWE));
			m_r.left+=(p.x-m_r.left);
			break;
		case 8:
			SetCursor(LoadCursor(NULL,IDC_SIZEALL));
			cp.x=(m_r.right + m_r.left)/2;
			cp.y=(m_r.bottom + m_r.top)/2;
			m_r.OffsetRect(p.x-cp.x, p.y-cp.y);
			break;
		}
		if(m_sel <CTL_RECT_NUM)  MakeSubRect(m_r);
	}
}
