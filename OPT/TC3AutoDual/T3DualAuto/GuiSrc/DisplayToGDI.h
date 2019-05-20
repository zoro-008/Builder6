#ifndef _DISPLAYTOGDI_H
#define _DISPLAYTOGDI_H

#include "LineProc.h"

class CDisplayToGDI{
public:
	CDisplayToGDI(){};
	~CDisplayToGDI(){};
// polygon 
	void DRect(CDC *pDC,RECT r,COLORREF rgb,int nMode=1);
	void DRectFill(CDC *pDC,RECT r,COLORREF rgb,COLORREF frgb,int nMode=1);
	void DDotRect(CDC *pDC,RECT r,COLORREF rgb,int nMode=1);
	void DEllipse(CDC *pDC,RECT r,COLORREF nColor,int nMode=1);
	void DEllipseWithCross(CDC *pDC,RECT r,COLORREF nColor,int nMode=1);
	void DSolidEllipse(CDC *pDC,RECT r,COLORREF nColor,int nMode=1);
	void DEllipseWithSolidEllipse(CDC *pDC,RECT r,RECT r2,COLORREF nColor,COLORREF nColor2,int nMode=1);
	void DEllipseWithEllipse(CDC *pDC,RECT r,RECT r2,COLORREF nColor,COLORREF nColor2,int nMode=1);
	void DPie(CDC *pDC,RECT r,POINT p1,POINT p2,COLORREF rgb);
// point
	void DPoint(CDC *pDC,usrPOINT *p,int nCnt,COLORREF rgb,int nMode=1);
	void DPoint(CDC *pDC,POINT *p,int nCnt,COLORREF rgb,int nMode=1);

	void DPoint(CDC *pDC,POINT p,COLORREF rgb,int nMode=1);
	void DCrossPoint(CDC *pDC,POINT p,int offset,COLORREF rgb,int nMode=1);
	void DCrossPoint(CDC *pDC,usrPOINT p,int offset,COLORREF rgb,int nMode=1);
	void DCrossPoint(CDC *pDC,usrPOINT p,int offset,int weight,COLORREF rgb,int nMode=1);
	void DCrossPoint(CDC *pDC,POINT p,int offset, int weight,COLORREF rgb,int nMode=1);
	void DCrossPointWithRect(CDC *pDC,POINT p,int offset,COLORREF rgb);
	void DCrossPointWithEllipse(CDC *pDC,POINT p,int offset,COLORREF rgbp,COLORREF rgbe,int nMode=1);
    void DCrossPointWithEllipse(CDC *pDC,POINT p,int offset,int Thickness,COLORREF rgbp,COLORREF rgbe,int nMode=1);
	void DCrossPointWithEllipse(CDC *pDC,usrPOINT p,int offset,COLORREF rgb,int nMode=1);
	void DCrossPointWithFillEllipse(CDC *pDC,POINT p,int offset,COLORREF rgbp,COLORREF rgbe,int nMode=1);

//line	
	void DLine(CDC *MemDC,usrPOINT p1,usrPOINT p2,COLORREF rgb,int nMode=1);
	void DLineW(CDC *MemDC,usrPOINT p1,usrPOINT p2,COLORREF rgb,int weight=1,int nMode=1);
	void DLineW(CDC *MemDC,POINT p1,POINT p2,COLORREF rgb,int weight=1,int nMode=1);

	void DLine(CDC *MemDC,POINT p1,POINT p2,COLORREF rgb,int nMode=1);
	void DDotLine(CDC *MemDC,usrPOINT p1,usrPOINT p2,COLORREF rgb,int nMode=1);
	void DDotLine(CDC *MemDC,POINT p1,POINT p2,COLORREF rgb,int nMode=1);
// text	
	void DTextOut(CDC *pDC,int nx,int ny,char *pText,COLORREF rgb,int nMode=1);
	void DTextOutCenter(CDC *pDC,int nx,int ny,char *pText,COLORREF rgb);
	void DTextOutWithFont(CDC *pDC,int nx,int ny,int w,int h,char *pText,COLORREF rgb);
	void DTextOutWithFontII(CDC *pDC,int nx,int ny,int w,int h,char *pText,COLORREF rgb);
//polygon
	void DPolygon(CDC *MemDC,POINT *p,int num,COLORREF rgb,int nMode=1);
	void DPolygonFill(CDC *MemDC,POINT *p,int num,COLORREF rgb);

};	

#endif