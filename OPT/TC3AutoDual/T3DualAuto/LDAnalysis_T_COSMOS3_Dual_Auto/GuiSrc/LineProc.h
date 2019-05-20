#ifndef __LINEPROC_H
#define __LINEPROC_H
// LineProc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLineProc window
typedef struct {
    double sx;
	double sy;
	double ex;
	double ey;
	double slope;
	double cx;
	double cy;
	double dx;
	double dy;
	double c_value; //constant org.
	int t;
}LINE_STRUCT;

typedef struct {
	double x;
	double y;
}usrPOINT;

typedef struct{
	usrPOINT sp;
	usrPOINT ep;
}LINE_DATA_POS;

class CLineProc : public CObject
{
// Construction
public:
	CLineProc();
	
// Attributes
public:
	double GetYPointValue(double x,LINE_STRUCT *l);
	double GetXPointValue(double y,LINE_STRUCT *l);

	void   SetLine(POINT sp,POINT ep,LINE_STRUCT *l);
	void   SetLine(double slope,double constant,RECT r,LINE_STRUCT *l);
	void   SetLine(double slope,usrPOINT cp,RECT r,LINE_STRUCT *l);
	void   SetLine(int spx,int spy, int epx,int epy,LINE_STRUCT *l);
	void   SetLine(usrPOINT sp, usrPOINT ep,LINE_STRUCT *l);

	void   MoveLine(int dxx, int dyy,LINE_STRUCT *l);
	void   CopyLine(LINE_STRUCT *sl,LINE_STRUCT *tl);
    int    GetCrossPoint(LINE_STRUCT *l1, LINE_STRUCT *l2,usrPOINT *p);
	int    GetCrossPoint(LINE_STRUCT *l1, LINE_STRUCT *l2,POINT *p);
	void   GetOthoLine(LINE_STRUCT *l,LINE_STRUCT *ol);
	void   GetOthoLine(LINE_STRUCT *l,LINE_STRUCT *ol,usrPOINT p);
	void   GetOthoLine(LINE_STRUCT *l,LINE_STRUCT *ol, POINT p);

	void   GetOthoLine(LINE_STRUCT *l,LINE_STRUCT *ol,usrPOINT p,RECT r, int dir);
	int    GetLineToOthoCrossPoint(LINE_STRUCT *l1,LINE_STRUCT *l2,usrPOINT *p);
	void   ReCompute(LINE_STRUCT *l);

	int    GetCircleCenter(POINT npt1,POINT npt2,POINT npt3,usrPOINT *p);

	double GetPointToLineDistance(LINE_STRUCT *l,POINT p);
	double GetPointToLineDistance(LINE_STRUCT *l,POINT p, int tag);
	double GetPointToLineDistance(LINE_STRUCT *l,POINT p,POINT *ptt);
	double GetPointToLineDistance(LINE_STRUCT *l,usrPOINT p);
	double GetPointToLineDistance(LINE_STRUCT *l,usrPOINT p, int tag);
	double GetPointToLineDistance(LINE_STRUCT *l,usrPOINT p, usrPOINT *pt,int tag);

	double GetLineValue(LINE_STRUCT l,POINT p);
	double GetLineValue(LINE_STRUCT l,int xx,int yy,double *c);

public:
	virtual ~CLineProc();


};

/////////////////////////////////////////////////////////////////////////////
#endif
