// LineProc.cpp : implementation file
//

#include "stdafx.h"
#include "LineProc.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineProc

CLineProc::CLineProc()
{

}


void CLineProc::SetLine(POINT sp, POINT ep,LINE_STRUCT *l)
{
  l->sx=(double)sp.x;
  l->sy=(double)sp.y;
  l->ex=(double)ep.x;
  l->ey=(double)ep.y;

  l->cx=(l->sx+l->ex)/2.0;
  l->cy=(l->sy+l->ey)/2.0;
  l->dx=l->ex-l->sx;
  l->dy=l->ey-l->sy;

  if((l->dx)==0.0) { l->sx+=0.00001; ReCompute(l); }
  if(l->dy == 0.0) { l->sy+=0.00001; ReCompute(l); }

  l->slope=l->dy/l->dx; 
  l->c_value= -(l->slope)*(l->cx)+l->cy;
}

void CLineProc::SetLine(usrPOINT sp, usrPOINT ep,LINE_STRUCT *l)
{
  l->sx=sp.x;
  l->sy=sp.y;
  l->ex=ep.x;
  l->ey=ep.y;

  l->cx=(l->sx+l->ex)/2.0;
  l->cy=(l->sy+l->ey)/2.0;
  l->dx=l->ex-l->sx;
  l->dy=l->ey-l->sy;

  if((l->dx)==0.0) { l->sx+=0.00001; ReCompute(l); }
  if(l->dy == 0.0) { l->sy+=0.00001; ReCompute(l); }

  l->slope=l->dy/l->dx; 
  l->c_value= -(l->slope)*(l->cx)+l->cy;
}

void CLineProc::SetLine(int spx,int spy, int epx,int epy,LINE_STRUCT *l)
{
  l->sx=(double)spx;
  l->sy=(double)spy;
  l->ex=(double)epx;
  l->ey=(double)epy;

  l->cx=(l->sx+l->ex)/2.0;
  l->cy=(l->sy+l->ey)/2.0;
  l->dx=l->ex-l->sx;
  l->dy=l->ey-l->sy;

  if((l->dx)==0.0) { l->sx+=0.00001; ReCompute(l); }
  if(l->dy == 0.0) { l->sy+=0.00001; ReCompute(l); }

  l->slope=l->dy/l->dx; 
  l->c_value= -(l->slope)*(l->cx)+l->cy;
}

void CLineProc::SetLine(double slope,usrPOINT cp,RECT r,LINE_STRUCT *l)
{

  l->cx=cp.x;
  l->cy=cp.y;
  l->slope=slope;
  l->c_value= -(l->slope)*(l->cx)+l->cy;

  if(fabs(slope)<1.0)   
  {
	l->sy=GetYPointValue((double)r.left,l);
	l->sx=(double)(r.left);
	l->ey=GetYPointValue((double)r.right,l);
	l->ex=(double)(r.right);
  }
  else
  {
  	l->sx=GetXPointValue((double)r.top,l);
	l->sy=(double)(r.top);
	l->ex=GetXPointValue((double)r.bottom,l);
	l->ey=(double)(r.bottom);
  }
  l->dx=l->ex-l->sx;
  l->dy=l->ey-l->sy;
}

void CLineProc::SetLine(double slope,double constant,RECT r,LINE_STRUCT *l)
{

  l->slope=slope;
  l->c_value=constant;

  if(fabs(slope)<=1.0)   
  {
	l->sy=GetYPointValue((double)r.left,l);
	l->sx=(double)(r.left);
	l->ey=GetYPointValue((double)r.right,l);
	l->ex=(double)(r.right);
  }
  else
  {
  	l->sx=GetXPointValue((double)r.top,l);
	l->sy=(double)(r.top);
	l->ex=GetXPointValue((double)r.bottom,l);
	l->ey=(double)(r.bottom);
  }
  l->cx=(l->sx+l->ex)/2.0;
  l->cy=(l->sy+l->ey)/2.0;

  l->dx=l->ex-l->sx;
  l->dy=l->ey-l->sy;
}

void CLineProc::MoveLine(int dxx, int dyy,LINE_STRUCT *l)
{
  l->sx+=((double)dxx);
  l->sy+=((double)dyy);
  l->ex+=((double)dxx);
  l->ey+=((double)dyy);


  l->cx=(l->sx+l->ex)/2.0;
  l->cy=(l->sy+l->ey)/2.0;
  
  l->c_value= -(l->slope)*(l->cx)+l->cy;

   
  l->dx=l->ex-l->sx;
  l->dy=l->ey-l->sy;
}

void CLineProc::ReCompute(LINE_STRUCT *l)
{
  l->cx=(l->sx+l->ex)/2.0;
  l->cy=(l->sy+l->ey)/2.0;
  l->dx=l->ex-l->sx;
  l->dy=l->ey-l->sy;
}

void CLineProc::GetOthoLine(LINE_STRUCT *l,LINE_STRUCT *ol)
{ 
	CopyLine(l,ol);

	if(l->slope == 0) l->slope = 0.00001;
	ol->slope=-(1.0/(l->slope));

	ol->sy=ol->cy-1;

	ol->c_value=ol->cy-(ol->slope)*(ol->cx);

	ol->sx=GetXPointValue(ol->sy,ol);
	ol->ey=ol->cy+1;
	ol->ex=GetXPointValue(ol->ey,ol);
	ol->dx=ol->ex-ol->sx;
	ol->dy=ol->ey-ol->sy;
}

void CLineProc::GetOthoLine(LINE_STRUCT *l,LINE_STRUCT *ol, usrPOINT p)
{ 
	CopyLine(l,ol);

	if(l->slope == 0) l->slope = 0.00001;
	ol->slope=-(1.0/(l->slope));

	ol->sy=p.y-1;

	ol->c_value=(double)p.y-(ol->slope)*((double)p.x);

	ol->sx=GetXPointValue(ol->sy,ol);
	ol->ey=p.y+1;
	ol->ex=GetXPointValue(ol->ey,ol);
	ol->dx=ol->ex-ol->sx;
	ol->dy=ol->ey-ol->sy;
}

void CLineProc::GetOthoLine(LINE_STRUCT *l,LINE_STRUCT *ol, POINT p)
{ 
	CopyLine(l,ol);

	if(l->slope == 0) l->slope = 0.00001;
	ol->slope=-(1.0/(l->slope));

	ol->sy=(double)p.y-1;

	ol->c_value=(double)p.y-(ol->slope)*((double)p.x);

	ol->sx=GetXPointValue(ol->sy,ol);
	ol->ey=(double)p.y+1;
	ol->ex=GetXPointValue(ol->ey,ol);
	ol->dx=ol->ex-ol->sx;
	ol->dy=ol->ey-ol->sy;
}

void CLineProc::GetOthoLine(LINE_STRUCT *l,LINE_STRUCT *ol,usrPOINT p,RECT r, int dir)
{ 
	CopyLine(l,ol);

	if(l->slope == 0) l->slope = 0.00000001;
	ol->slope=-(1.0/(l->slope));
	
	ol->c_value=(double)p.y-(ol->slope)*((double)p.x);

    if(dir == 0) // x direction
	{
	   ol->sx=r.left;
   	   ol->sy=GetYPointValue(ol->sx,ol);
	   ol->ex=r.right;
	   ol->ey=GetYPointValue(ol->ex,ol);
    }
	else 
	{
		ol->sy=r.top;
		ol->sx=GetXPointValue(ol->sy,ol);
		ol->ey=r.bottom;
		ol->ex=GetXPointValue(ol->ey,ol);
	}
	ol->dx=ol->ex-ol->sx;
	ol->dy=ol->ey-ol->sy;
}


double CLineProc::GetYPointValue(double x,LINE_STRUCT *l)
{  
	double value_y=(l->slope*x+l->c_value);
	if(value_y <0)
		return 10;
   	return value_y;
}

double CLineProc::GetXPointValue(double y,LINE_STRUCT *l)
{
	double value_x;
   if(l->slope == 0) l->slope = 0.00001;
   value_x=((y-(l->c_value))/(l->slope));

   if(value_x <0) value_x=10;
   return  value_x;
}


void CLineProc::CopyLine(LINE_STRUCT *sl,LINE_STRUCT *tl)
{
  tl->sx=sl->sx;
  tl->sy=sl->sy;
  tl->ex=sl->ex;
  tl->ey=sl->ey;

  tl->cx=sl->cx;
  tl->cy=sl->cy;
  tl->dx=sl->dx;
  tl->dy=sl->dy;
  tl->t=sl->t;
  tl->slope=sl->slope;
  tl->c_value=sl->c_value;
}

int CLineProc::GetLineToOthoCrossPoint(LINE_STRUCT *l1,LINE_STRUCT *l2,usrPOINT *p)
{
    LINE_STRUCT l;

    GetOthoLine(l2,&l);
    return GetCrossPoint(l1,&l,p);
}

double CLineProc::GetPointToLineDistance(LINE_STRUCT *l,usrPOINT p)
{
    LINE_STRUCT ol;
	usrPOINT pt;
    double dx, dy;

    GetOthoLine(l,&ol,p);
	GetCrossPoint(l,&ol,&pt);
    
	dx=(p.x - pt.x);
	dy=(p.y - pt.y);

	return sqrt(dx*dx+dy*dy);
}



double CLineProc::GetPointToLineDistance(LINE_STRUCT *l,POINT p)
{
    LINE_STRUCT ol;
	usrPOINT pt;
    double dx, dy;
	usrPOINT input;

	input.x=(double)p.x;
	input.y=(double)p.y;

    GetOthoLine(l,&ol,input);
	GetCrossPoint(l,&ol,&pt);
    
	dx=(input.x - pt.x);
	dy=(input.y - pt.y);

	return sqrt(dx*dx+dy*dy);
}

double CLineProc::GetPointToLineDistance(LINE_STRUCT *l,POINT p, int tag)
{
    LINE_STRUCT ol;
	usrPOINT pt;
    double dx, dy;
    double distance=0.0;
	usrPOINT input;

	input.x=(double)p.x;
	input.y=(double)p.y;

    GetOthoLine(l,&ol,input);
	GetCrossPoint(l,&ol,&pt);
    
	dx=(input.x - pt.x);
	dy=(input.y - pt.y);

	switch(tag){
	case 0: // left
		if(dx <0)
		{
		   distance = 0.0;
		}
		else
           distance=sqrt(dx*dx+dy*dy);
		break;
	case 1: // top
		if(dy <0)
		{
		   distance = 0.0;
		}
		else
           distance=sqrt(dx*dx+dy*dy);
		break;

	case 2: //right
		if(dx >0)
		{
		   distance = 0.0;
		}
		else
           distance=sqrt(dx*dx+dy*dy);
		break;

	case 3: //bottom
		if(dy >0)
		{
		   distance = 0.0;
		}
		else
           distance=sqrt(dx*dx+dy*dy);
		break;

	}

	return distance;
}

double CLineProc::GetPointToLineDistance(LINE_STRUCT *l,POINT p,POINT *ptt)
{
    LINE_STRUCT ol;
	usrPOINT pt;
    double dx, dy;
	usrPOINT input;

	input.x=(double)p.x;
	input.y=(double)p.y;

    GetOthoLine(l,&ol,input);
	GetCrossPoint(l,&ol,&pt);
    
	ptt->x=(int)(pt.x+0.5);
	ptt->y=(int)(pt.y+0.5);
	dx=(input.x - pt.x);
	dy=(input.y - pt.y);

	return sqrt(dx*dx+dy*dy);
}



double CLineProc::GetPointToLineDistance(LINE_STRUCT *l,usrPOINT p, int tag)
{
    LINE_STRUCT ol;
	usrPOINT pt;
    double dx, dy;
    double distance=0.0;

    GetOthoLine(l,&ol,p);
	GetCrossPoint(l,&ol,&pt);
    
	dx=(p.x - pt.x);
	dy=(p.y - pt.y);

	switch(tag){
	case 0: // left
		if(dx <0)
		{
		   distance = 0.0;
		}
		else
           distance=sqrt(dx*dx+dy*dy);
		break;
	case 1: // top
		if(dy <0)
		{
		   distance = 0.0;
		}
		else
           distance=sqrt(dx*dx+dy*dy);
		break;

	case 2: //right
		if(dx >0)
		{
		   distance = 0.0;
		}
		else
           distance=sqrt(dx*dx+dy*dy);
		break;

	case 3: //bottom
		if(dy >0)
		{
		   distance = 0.0;
		}
		else
           distance=sqrt(dx*dx+dy*dy);
		break;

	}

	return distance;
}

double CLineProc::GetPointToLineDistance(LINE_STRUCT *l,usrPOINT p, usrPOINT *pt,int tag)
{
    LINE_STRUCT ol;
    double dx, dy;
    double distance=0.0;

    GetOthoLine(l,&ol,p);
	GetCrossPoint(l,&ol,pt);
    
	dx=(p.x - pt->x);
	dy=(p.y - pt->y);

	switch(tag){
	case 0: // left
		if(dx <0)
		{
		   distance = 0.0;
		}
		else
           distance=sqrt(dx*dx+dy*dy);
		break;
	case 1: // top
		if(dy <0)
		{
		   distance = 0.0;
		}
		else
           distance=sqrt(dx*dx+dy*dy);
		break;

	case 2: //right
		if(dx >=0)
		{
		   distance = 0.0;
		}
		else
           distance=sqrt(dx*dx+dy*dy);
		break;

	case 3: //bottom
		if(dy >0)
		{
		   distance = 0.0;
		}
		else
           distance=sqrt(dx*dx+dy*dy);
		break;

	}

	return distance;
}


int CLineProc::GetCrossPoint(LINE_STRUCT *l1, LINE_STRUCT *l2,usrPOINT *p)
{
    double x,y;
 
	
	if(l1->slope == l2->slope) return 0;


	x=(l1->c_value - l2->c_value)/(l2->slope - l1->slope);
	y=l2->slope*x+l2->c_value;

	p->x=x;
	p->y=y;

    return 1;
}

int CLineProc::GetCrossPoint(LINE_STRUCT *l1, LINE_STRUCT *l2,POINT *p)
{
    double x,y;
 
	
	if(l1->slope == l2->slope) return 0;


	x=(l1->c_value - l2->c_value)/(l2->slope - l1->slope);
	y=l2->slope*x+l2->c_value;

	p->x=(int)(x+0.5);
	p->y=(int)(y+0.5);

    return 1;
}


int CLineProc::GetCircleCenter(POINT npt1,POINT npt2,POINT npt3,usrPOINT *p)
{
	int rvalue=0;
	usrPOINT fcp,scp;
	LINE_STRUCT l1,l2,fo,so;
    

    SetLine(npt1,npt2,&l1);
// line center point
	fcp.x=l1.cx;
	fcp.y=l1.cy;
	GetOthoLine(&l1,&fo,fcp);

	SetLine(npt2,npt3,&l2);
// line center point 
	scp.x=l2.cx;
	scp.y=l2.cy;
	GetOthoLine(&l2,&so,scp);
	rvalue=GetCrossPoint(&fo,&so,p);

	return rvalue;
}
//for use boudary definition.

double CLineProc::GetLineValue(LINE_STRUCT l,POINT p)
{
	double v=0;
	
	v=(double)p.y-l.slope*(double)p.x-l.c_value;
	return v;
}
double CLineProc::GetLineValue(LINE_STRUCT l,int xx,int yy,double *c)
{
	double v=0;
	
	v=(double)yy-l.slope*(double)xx;
	*c=l.c_value;
	return v;
}

CLineProc::~CLineProc()
{
}


