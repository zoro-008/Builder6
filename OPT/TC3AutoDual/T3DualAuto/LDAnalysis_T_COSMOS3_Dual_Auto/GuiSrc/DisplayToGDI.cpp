#include "stdafx.h"
#include "DisplayToGDI.h"


void CDisplayToGDI::DRect(CDC *pDC,RECT r,COLORREF rgb,int nMode)
{
	CPen pen,*oldPen;
	LOGBRUSH nLogBrush;
    CBrush brush,*oldbrush;
   
	nLogBrush.lbColor=rgb;
	nLogBrush.lbHatch=HS_VERTICAL;
	nLogBrush.lbStyle=BS_NULL;

	pen.CreatePen(PS_SOLID,1,rgb);
	oldPen=pDC->SelectObject(&pen);

	brush.CreateBrushIndirect(&nLogBrush);
	oldbrush=pDC->SelectObject(&brush);


	r.bottom+=1;
	r.right+=1;

	if(nMode!=1)
	{
		r.left  /=nMode;
		r.top   /=nMode;
		r.right /=nMode;
		r.bottom/=nMode;
	}

	pDC->Rectangle(&r);

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();

	pDC->SelectObject(oldbrush);
	brush.DeleteObject();

}

void CDisplayToGDI::DRectFill(CDC *pDC,RECT r,COLORREF rgb,COLORREF frgb,int nMode)
{
	CPen pen,*oldPen;
	LOGBRUSH nLogBrush;
    CBrush brush,*oldbrush;
   
	nLogBrush.lbColor=frgb;
	nLogBrush.lbHatch=HS_VERTICAL;
	nLogBrush.lbStyle=BS_SOLID;

	pen.CreatePen(PS_SOLID,1,rgb);
	oldPen=pDC->SelectObject(&pen);

	brush.CreateBrushIndirect(&nLogBrush);
	oldbrush=pDC->SelectObject(&brush);


	r.bottom+=1;
	r.right+=1;

	if(nMode!=1)
	{
		r.left  /=nMode;
		r.top   /=nMode;
		r.right /=nMode;
		r.bottom/=nMode;
	}

	pDC->Rectangle(&r);

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();

	pDC->SelectObject(oldbrush);
	brush.DeleteObject();

}

void CDisplayToGDI::DDotRect(CDC *pDC,RECT r,COLORREF rgb,int nMode)
{
	CPen pen,*oldPen;
	LOGBRUSH nLogBrush;
    CBrush brush,*oldbrush;
   
	nLogBrush.lbColor=rgb;
	nLogBrush.lbHatch=HS_VERTICAL;
	nLogBrush.lbStyle=BS_NULL;

	pen.CreatePen(PS_DOT,1,rgb);
	oldPen=pDC->SelectObject(&pen);

	brush.CreateBrushIndirect(&nLogBrush);
	oldbrush=pDC->SelectObject(&brush);


	r.bottom+=1;
	r.right+=1;

	if(nMode!=1)
	{
		r.left  /=nMode;
		r.top   /=nMode;
		r.right /=nMode;
		r.bottom/=nMode;
	}

	pDC->Rectangle(&r);

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();

	pDC->SelectObject(oldbrush);
	brush.DeleteObject();

}


void CDisplayToGDI::DPoint(CDC *pDC,usrPOINT *p,int nCnt,COLORREF rgb,int nMode)
{
	int i;
	CPen pen,*oldPen;
	
	pen.CreatePen(PS_SOLID,1,rgb);
	oldPen=pDC->SelectObject(&pen);

	POINT ipt;
	for(i=0; i<nCnt; i++)
	{
		ipt.x=(int)(p[i].x/(double)nMode+0.5);
		ipt.y=(int)(p[i].y/(double)nMode+0.5);
		pDC->SetPixel(ipt,rgb);
	}

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();
}

void CDisplayToGDI::DPoint(CDC *pDC,POINT *p,int nCnt,COLORREF rgb,int nMode)
{
	int i;
	CPen pen,*oldPen;
	
	pen.CreatePen(PS_SOLID,1,rgb);
	oldPen=pDC->SelectObject(&pen);

	POINT ipt;
	for(i=0; i<nCnt; i++)
	{
		ipt.x=(int)((double)p[i].x/(double)nMode);
		ipt.y=(int)((double)p[i].y/(double)nMode);
		pDC->SetPixel(ipt,rgb);
	}

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();
}
void CDisplayToGDI::DPoint(CDC *pDC,POINT p,COLORREF rgb,int nMode)
{
	CPen pen,*oldPen;
	
	pen.CreatePen(PS_SOLID,1,rgb);
	oldPen=pDC->SelectObject(&pen);

	POINT ipt;
	ipt.x=(int)((double)p.x/(double)nMode+0.5);
	ipt.y=(int)((double)p.y/(double)nMode+0.5);
	pDC->SetPixel(ipt,rgb);

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();
}

void CDisplayToGDI::DCrossPointWithRect(CDC *pDC,POINT p,int offset,COLORREF rgb)
{
	CPen pen,*oldPen;
	pen.CreatePen(PS_SOLID,1,rgb);
	oldPen=pDC->SelectObject(&pen);
	RECT r;

	pDC->MoveTo(p.x-offset,p.y);
	pDC->LineTo(p.x+offset,p.y);
	pDC->MoveTo(p.x,p.y-offset);
	pDC->LineTo(p.x,p.y+offset);

	r.left=p.x-offset;
	r.right=p.x+offset;
	r.top=p.y-offset;
	r.bottom=p.y+offset;
	DRect(pDC,r,RGB(255,0,0));

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();
}

void CDisplayToGDI::DPie(CDC *pDC,RECT r,POINT p1,POINT p2,COLORREF rgb)
{
	CPen pen,*oldPen;
	pen.CreatePen(PS_SOLID,1,rgb);
	oldPen=pDC->SelectObject(&pen);

	LOGBRUSH nLogBrush;
    CBrush brush,*oldbrush;
	nLogBrush.lbColor=rgb;
	nLogBrush.lbHatch=HS_VERTICAL;
	nLogBrush.lbStyle=BS_NULL;
	brush.CreateBrushIndirect(&nLogBrush);
	oldbrush=pDC->SelectObject(&brush);
	
	pDC->Pie(&r,p1,p2);

	
	pDC->SelectObject(oldPen);   
    pen.DeleteObject();

	pDC->SelectObject(oldbrush);
	brush.DeleteObject();

}



void CDisplayToGDI::DCrossPointWithEllipse(CDC *pDC,POINT p,int offset,COLORREF rgbp,COLORREF rgbe,int nMode)
{
	CPen pen,*oldPen;
	pen.CreatePen(PS_SOLID,1,rgbp);
	oldPen=pDC->SelectObject(&pen);
	RECT r;
    POINT pt;
    
	if(nMode!=1)
	{
		pt.x=p.x/nMode;
		pt.y=p.y/nMode;
		offset/=nMode;

		memcpy(&p,&pt,sizeof(POINT));
	}

	if(offset==0)
	{
		offset=10;
		pDC->MoveTo(0,p.y);
		pDC->LineTo(639,p.y);
		pDC->MoveTo(p.x,0);
		pDC->LineTo(p.x,479);
	}
	else
	{
		pDC->MoveTo(p.x-offset,p.y);
		pDC->LineTo(p.x+offset,p.y);
		pDC->MoveTo(p.x,p.y-offset);
		pDC->LineTo(p.x,p.y+offset);
	}
	r.left=p.x-offset;
	r.right=p.x+offset;
	r.top=p.y-offset;
	r.bottom=p.y+offset;
	DEllipse(pDC,r,rgbe);

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();
}
void CDisplayToGDI::DCrossPointWithEllipse(CDC *pDC,POINT p,int offset,int Thickness,COLORREF rgbp,COLORREF rgbe,int nMode)
{
	CPen pen,*oldPen;
	pen.CreatePen(PS_SOLID,Thickness,rgbp);
	oldPen=pDC->SelectObject(&pen);
	RECT r;
    POINT pt;
    
	if(nMode!=1)
	{
		pt.x=p.x/nMode;
		pt.y=p.y/nMode;
		offset/=nMode;

		memcpy(&p,&pt,sizeof(POINT));
	}

	if(offset==0)
	{
		offset=10;
		pDC->MoveTo(0,p.y);
		pDC->LineTo(639,p.y);
		pDC->MoveTo(p.x,0);
		pDC->LineTo(p.x,479);
	}
	else
	{
		pDC->MoveTo(p.x-offset,p.y);
		pDC->LineTo(p.x+offset,p.y);
		pDC->MoveTo(p.x,p.y-offset);
		pDC->LineTo(p.x,p.y+offset);
	}
	r.left=p.x-offset;
	r.right=p.x+offset;
	r.top=p.y-offset;
	r.bottom=p.y+offset;
	DEllipse(pDC,r,rgbe);

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();
}
void CDisplayToGDI::DCrossPointWithFillEllipse(CDC *pDC,POINT p,int offset,COLORREF rgbp,COLORREF rgbe,int nMode)
{
	CPen pen,*oldPen;
	pen.CreatePen(PS_SOLID,1,rgbp);
	oldPen=pDC->SelectObject(&pen);
	RECT r;
    POINT pt;
    
	if(nMode!=1)
	{
		pt.x=p.x/nMode;
		pt.y=p.y/nMode;
		offset/=nMode;

		memcpy(&p,&pt,sizeof(POINT));
	}


	if(offset==0)
	{
		offset=10;

		r.left=p.x-offset;
		r.right=p.x+offset;
		r.top=p.y-offset;
		r.bottom=p.y+offset;
		DSolidEllipse(pDC,r,rgbe);

		pDC->MoveTo(0,p.y);
		pDC->LineTo(639,p.y);
		pDC->MoveTo(p.x,0);
		pDC->LineTo(p.x,479);
	}
	else
	{
		r.left=p.x-offset;
		r.right=p.x+offset;
		r.top=p.y-offset;
		r.bottom=p.y+offset;
		DSolidEllipse(pDC,r,rgbe);

		pDC->MoveTo(p.x-offset,p.y);
		pDC->LineTo(p.x+offset,p.y);
		pDC->MoveTo(p.x,p.y-offset);
		pDC->LineTo(p.x,p.y+offset);
	}

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();
}

void CDisplayToGDI::DLine(CDC *MemDC,usrPOINT p1,usrPOINT p2,COLORREF rgb,int nMode)
{

	CPen pen,*oldPen;
	pen.CreatePen(PS_SOLID,1,rgb);
    POINT pt1,pt2;
	oldPen=MemDC->SelectObject(&pen);

	pt1.x=(int)(p1.x/(double)nMode+0.5);
	pt1.y=(int)(p1.y/(double)nMode+0.5);
	pt2.x=(int)(p2.x/(double)nMode+0.5);
	pt2.y=(int)(p2.y/(double)nMode+0.5);

	MemDC->MoveTo(pt1);
	MemDC->LineTo(pt2);
	MemDC->SelectObject(oldPen);   
    pen.DeleteObject();
}

void CDisplayToGDI::DLineW(CDC *MemDC,usrPOINT p1,usrPOINT p2,COLORREF rgb,int weight,int nMode)
{

	CPen pen,*oldPen;
	pen.CreatePen(PS_SOLID,weight,rgb);
    POINT pt1,pt2;
	oldPen=MemDC->SelectObject(&pen);

	pt1.x=(int)(p1.x/(double)nMode+0.5);
	pt1.y=(int)(p1.y/(double)nMode+0.5);
	pt2.x=(int)(p2.x/(double)nMode+0.5);
	pt2.y=(int)(p2.y/(double)nMode+0.5);

	MemDC->MoveTo(pt1);
	MemDC->LineTo(pt2);
	MemDC->SelectObject(oldPen);   
    pen.DeleteObject();
}

void CDisplayToGDI::DLineW(CDC *MemDC,POINT p1,POINT p2,COLORREF rgb,int weight,int nMode)
{

	CPen pen,*oldPen;
	pen.CreatePen(PS_SOLID,weight,rgb);
 
	oldPen=MemDC->SelectObject(&pen);

	p1.x=(int)(p1.x/(double)nMode);
	p1.y=(int)(p1.y/(double)nMode);
	p2.x=(int)(p2.x/(double)nMode);
	p2.y=(int)(p2.y/(double)nMode);

	MemDC->MoveTo(p1);
	MemDC->LineTo(p2);
	MemDC->SelectObject(oldPen);   
    pen.DeleteObject();
}


void CDisplayToGDI::DLine(CDC *MemDC,POINT p1,POINT p2,COLORREF rgb,int nMode)
{
	POINT pt1,pt2;
	CPen pen,*oldPen;
	pen.CreatePen(PS_SOLID,1,rgb);
    oldPen=MemDC->SelectObject(&pen);

	pt1.x=(int)(p1.x/nMode);
	pt1.y=(int)(p1.y/nMode);
	pt2.x=(int)(p2.x/nMode);
	pt2.y=(int)(p2.y/nMode);

	MemDC->MoveTo(pt1);
	MemDC->LineTo(pt2);
	MemDC->SelectObject(oldPen);   
    pen.DeleteObject();
}

void CDisplayToGDI::DPolygon(CDC *MemDC,POINT *p,int num,COLORREF rgb,int nMode)
{
	POINT tmp,start;
	CPen pen,*oldPen;
	pen.CreatePen(PS_SOLID,1,rgb);
    oldPen=MemDC->SelectObject(&pen);
	int i;

	start.x=(int)(p[0].x/nMode);
	start.y=(int)(p[0].y/nMode);

	MemDC->MoveTo(start);
	for(i=1; i<num; i++)
	{
		tmp.x=(int)(p[i].x/nMode);
		tmp.y=(int)(p[i].y/nMode);
		MemDC->LineTo(tmp);
	}

	MemDC->LineTo(start);

	MemDC->SelectObject(oldPen);   
    pen.DeleteObject();
}



void CDisplayToGDI::DDotLine(CDC *MemDC,usrPOINT p1,usrPOINT p2,COLORREF rgb,int nMode)
{
	CPen pen,*oldPen;
	pen.CreatePen(PS_DOT,1,rgb);
	oldPen=MemDC->SelectObject(&pen);
	MemDC->SetBkMode(TRANSPARENT);
	POINT pt1,pt2;

	pt1.x=(int)(p1.x+0.5);
	pt1.y=(int)(p1.y+0.5);
	pt2.x=(int)(p2.x+0.5);
	pt2.y=(int)(p2.y+0.5);

	if(nMode!=1)
	{
		pt1.x/=nMode;
		pt1.y/=nMode;
		pt2.x/=nMode;
		pt2.y/=nMode;
	}

	MemDC->MoveTo(pt1);
	MemDC->LineTo(pt2);

	MemDC->SelectObject(oldPen);  
    pen.DeleteObject();

}

void CDisplayToGDI::DDotLine(CDC *MemDC,POINT p1,POINT p2,COLORREF rgb,int nMode)
{
	CPen pen,*oldPen;
	pen.CreatePen(PS_DOT,1,rgb);
	oldPen=MemDC->SelectObject(&pen);
	MemDC->SetBkMode(TRANSPARENT);


	if(nMode!=1)
	{
		p1.x/=nMode;
		p1.y/=nMode;
		p2.x/=nMode;
		p2.y/=nMode;
	}

	MemDC->MoveTo(p1);
	MemDC->LineTo(p2);

	MemDC->SelectObject(oldPen);  
    pen.DeleteObject();

}

void CDisplayToGDI::DTextOut(CDC *pDC,int nx,int ny,char *pText,COLORREF rgb,int nMode)
{
	CPen pen,*oldPen;

	pen.CreatePen(PS_SOLID,1,rgb);
	oldPen=pDC->SelectObject(&pen);

    COLORREF nOldRgb;
	nOldRgb=pDC->SetTextColor(rgb);
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(nx/nMode,ny/nMode,pText);
	pDC->SetTextColor(nOldRgb);

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();

}


void CDisplayToGDI::DTextOutCenter(CDC *pDC,int nx,int ny,char *pText,COLORREF rgb)
{
	CPen pen,*oldPen;
	pen.CreatePen(PS_SOLID,1,rgb);
	oldPen=pDC->SelectObject(&pen);

    COLORREF nOldRgb;
	nOldRgb=pDC->SetTextColor(rgb);
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(nx-strlen(pText)*4,ny-4,pText);
	pDC->SetTextColor(nOldRgb);

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();

}

void CDisplayToGDI::DTextOutWithFont(CDC *pDC,int nx,int ny,int w,int h,char *pText,COLORREF rgb)
{

	CPen pen,*oldPen;
	CFont *oldFont;
	CFont	*tFont=new CFont;
	
	tFont->CreateFont(h,w, 0, 0, FW_ULTRABOLD, FALSE, FALSE,0,0,0,0,0,0, "Arial");

	pen.CreatePen(PS_SOLID,1,rgb);
	oldPen=pDC->SelectObject(&pen);

    COLORREF nOldRgb;
	nOldRgb=pDC->SetTextColor(rgb);
	pDC->SetBkMode(TRANSPARENT);
	oldFont=pDC->SelectObject(tFont);
	pDC->TextOut(nx-strlen(pText)*4,ny-4,pText);
	pDC->SetTextColor(nOldRgb);

	pDC->SelectObject(oldFont);
	pDC->SelectObject(oldPen);   
    pen.DeleteObject();

	delete tFont;

}


void CDisplayToGDI::DTextOutWithFontII(CDC *pDC,int nx,int ny,int w,int h,char *pText,COLORREF rgb)
{

	CPen pen,*oldPen;
	CFont *oldFont;
	CFont	*tFont=new CFont;
	
	tFont->CreateFont(h,w, 0, 0, FW_ULTRABOLD, FALSE, FALSE,0,0,0,0,0,0, "Arial");

	pen.CreatePen(PS_SOLID,1,rgb);
	oldPen=pDC->SelectObject(&pen);

    COLORREF nOldRgb;
	nOldRgb=pDC->SetTextColor(rgb);
	pDC->SetBkMode(TRANSPARENT);
	oldFont=pDC->SelectObject(tFont);
	pDC->TextOut(nx,ny-4,pText);
	pDC->SetTextColor(nOldRgb);

	pDC->SelectObject(oldFont);
	pDC->SelectObject(oldPen);   
    pen.DeleteObject();

	delete tFont;

}

void CDisplayToGDI::DCrossPoint(CDC *pDC,POINT p,int offset,COLORREF rgb,int nMode)
{
	CPen pen,*oldPen;
	POINT tpt;
	pen.CreatePen(PS_SOLID,1,rgb);
	oldPen=pDC->SelectObject(&pen);
	if(nMode!=1)
	{
		tpt.x=p.x/nMode;
		tpt.y=p.y/nMode;
		offset/=nMode;
		memcpy(&p,&tpt,sizeof(POINT));
	}
	
	pDC->MoveTo(p.x-offset,p.y);
	pDC->LineTo(p.x+offset,p.y);
	pDC->MoveTo(p.x,p.y-offset);
	pDC->LineTo(p.x,p.y+offset);

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();
}
void CDisplayToGDI::DCrossPointWithEllipse(CDC *pDC,usrPOINT p,int offset,COLORREF rgb,int nMode)
{
	CPen pen,*oldPen;
	LOGBRUSH nLogBrush;
    CBrush brush,*oldbrush;
   
	nLogBrush.lbColor=rgb;
	nLogBrush.lbHatch=HS_VERTICAL;
	nLogBrush.lbStyle=BS_NULL;

	pen.CreatePen(PS_SOLID,1,rgb);
	oldPen=pDC->SelectObject(&pen);

	brush.CreateBrushIndirect(&nLogBrush);
	oldbrush=pDC->SelectObject(&brush);
	
	POINT tpt;
	RECT r;
	tpt.x=(int)(p.x+0.5);
	tpt.y=(int)(p.y+0.5);
	

	if(nMode!=1)
	{
		r.left  =(tpt.x-offset)/nMode;
		r.right =(tpt.x+offset)/nMode;
		r.top   =(tpt.y-offset)/nMode;
		r.bottom=(tpt.y+offset)/nMode;
	}
	else
	{

		r.left  =(tpt.x-offset);
		r.right =(tpt.x+offset);
		r.top   =(tpt.y-offset);
		r.bottom=(tpt.y+offset);

	}
    r.bottom+=1;
	r.right+=1;



	DCrossPoint(pDC,tpt,offset,rgb,nMode);

	pDC->Ellipse(&r);

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();

	pDC->SelectObject(oldbrush);
	brush.DeleteObject();
}

void CDisplayToGDI::DCrossPoint(CDC *pDC,usrPOINT p,int offset,COLORREF rgb,int nMode)
{
	CPen pen,*oldPen;
	POINT tpt;
	POINT l,t,r,b;
	pen.CreatePen(PS_SOLID,1,rgb);
	oldPen=pDC->SelectObject(&pen);

	tpt.x=(int)(p.x+0.5);
	tpt.y=(int)(p.y+0.5);
	if(nMode!=1)
	{
		l.x=(tpt.x-offset)/nMode;
		l.y=tpt.y/nMode;

		r.x=(tpt.x+offset)/nMode;
		r.y=tpt.y/nMode;

		t.x=tpt.x/nMode;
		t.y=(tpt.y-offset)/nMode;

		b.x=tpt.x/nMode;
		b.y=(tpt.y+offset)/nMode;

	}
	else
	{

		l.x=(tpt.x-offset);
		l.y=tpt.y;

		r.x=(tpt.x+offset);
		r.y=tpt.y;

		t.x=tpt.x;
		t.y=(tpt.y-offset);

		b.x=tpt.x;
		b.y=(tpt.y+offset);

	}
	pDC->MoveTo(l.x,l.y);
	pDC->LineTo(r.x,r.y);
	pDC->MoveTo(t.x,t.y);
	pDC->LineTo(b.x,b.y);

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();
}

void CDisplayToGDI::DCrossPoint(CDC *pDC,POINT p,int offset, int weight,COLORREF rgb,int nMode)
{
	CPen pen,*oldPen;
	POINT tpt;
	pen.CreatePen(PS_SOLID,weight,rgb);
	oldPen=pDC->SelectObject(&pen);
	if(nMode!=1)
	{
		tpt.x=p.x/nMode;
		tpt.y=p.y/nMode;
		offset/=nMode;
		memcpy(&p,&tpt,sizeof(POINT));
	}
	
	pDC->MoveTo(p.x-offset,p.y);
	pDC->LineTo(p.x+offset,p.y);
	pDC->MoveTo(p.x,p.y-offset);
	pDC->LineTo(p.x,p.y+offset);

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();
}

void CDisplayToGDI::DCrossPoint(CDC *pDC,usrPOINT p,int offset,int weight,COLORREF rgb,int nMode)
{
	CPen pen,*oldPen;
	POINT tpt;
	pen.CreatePen(PS_SOLID,weight,rgb);
	oldPen=pDC->SelectObject(&pen);
	if(nMode!=1)
	{
		tpt.x=(int)(p.x+0.5)/nMode;
		tpt.y=(int)(p.y+0.5)/nMode;
		offset/=nMode;
	}
	else
	{
		tpt.x=(int)(p.x+0.5);
		tpt.y=(int)(p.y+0.5);
	}
	pDC->MoveTo(tpt.x-offset,tpt.y);
	pDC->LineTo(tpt.x+offset,tpt.y);
	pDC->MoveTo(tpt.x,tpt.y-offset);
	pDC->LineTo(tpt.x,tpt.y+offset);

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();
}

void CDisplayToGDI::DEllipse(CDC *pDC,RECT r,COLORREF nColor,int nMode)
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

	if(nMode!=1)
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

void CDisplayToGDI::DEllipseWithSolidEllipse(CDC *pDC,RECT r,RECT r2,COLORREF nColor,COLORREF nColor2,int nMode)
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

	if(nMode!=1)
	{
		r.left/=nMode;
		r.top/=nMode;
		r.right/=nMode;
		r.bottom/=nMode;
	}
	DSolidEllipse(pDC,r2,nColor2,nMode);
	pDC->Ellipse(&r);

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();

	pDC->SelectObject(oldbrush);
	brush.DeleteObject();
}

void CDisplayToGDI::DEllipseWithEllipse(CDC *pDC,RECT r,RECT r2,COLORREF nColor,COLORREF nColor2,int nMode)
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

	if(nMode!=1)
	{
		r.left/=nMode;
		r.top/=nMode;
		r.right/=nMode;
		r.bottom/=nMode;
	}
	DEllipse(pDC,r2,nColor2,nMode);
	pDC->Ellipse(&r);

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();

	pDC->SelectObject(oldbrush);
	brush.DeleteObject();
}

void CDisplayToGDI::DEllipseWithCross(CDC *pDC,RECT r,COLORREF nColor,int nMode)
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

	POINT p1;
	int offset=(r.right-r.left)/2;
	p1.x=(r.left  +r.right)/2;
	p1.y=(r.bottom+r.top  )/2;

	if(nMode!=1)
	{
		r.left/=nMode;
		r.top/=nMode;
		r.right/=nMode;
		r.bottom/=nMode;
	}


	DCrossPoint(pDC,p1,offset,nColor,nMode);

	pDC->Ellipse(&r);

	pDC->SelectObject(oldPen);   
    pen.DeleteObject();

	pDC->SelectObject(oldbrush);
	brush.DeleteObject();
}

void CDisplayToGDI::DSolidEllipse(CDC *pDC,RECT r,COLORREF nColor,int nMode)
{
	CPen pen,*oldPen;
	LOGBRUSH nLogBrush;
    CBrush brush,*oldbrush;
   
	nLogBrush.lbColor=nColor;
	nLogBrush.lbHatch=HS_VERTICAL;
	nLogBrush.lbStyle=BS_SOLID;

	pen.CreatePen(PS_SOLID,1,nColor);
	oldPen=pDC->SelectObject(&pen);

	brush.CreateBrushIndirect(&nLogBrush);
	oldbrush=pDC->SelectObject(&brush);


    r.bottom+=1;
	r.right+=1;

	if(nMode!=1)
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
void CDisplayToGDI::DPolygonFill(CDC *MemDC,POINT *p,int num,COLORREF rgb)
{
	CPen pen,*oldPen;
	LOGBRUSH nLogBrush;
    CBrush brush,*oldbrush;
   
	nLogBrush.lbColor=rgb;
	nLogBrush.lbHatch=HS_VERTICAL;
	nLogBrush.lbStyle=BS_SOLID;

	pen.CreatePen(PS_SOLID,1,rgb);
	oldPen=MemDC->SelectObject(&pen);

	brush.CreateBrushIndirect(&nLogBrush);
	oldbrush=MemDC->SelectObject(&brush);	
	

	MemDC->Polygon(p,num);



	MemDC->SelectObject(oldPen);   
    pen.DeleteObject();

	MemDC->SelectObject(oldbrush);
	brush.DeleteObject();
}

