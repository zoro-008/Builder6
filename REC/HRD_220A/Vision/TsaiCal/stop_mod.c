#include "cal_main.h"
#include "stop_mod.h"
#include <math.h>

int stop_criterion()
{
	double error,xerr,yerr;
	double xd,yd,xu,yu,xc,yc,zc,rd,ftol;
	int i;

	ftol=1.0e-3;
	error=0.;
	for(i=0;i<cd->point_count;i++){
		xd=cp->dpx*(cd->Xf[i]-cp->Cx);
		yd=cp->dpy*(cd->Yf[i]-cp->Cy);
		rd=xd*xd+yd*yd;
		xu=xd*(1.+cc->kappa1*rd);
		yu=yd*(1.+cc->kappa1*rd);
		xc=cc->r1*cd->xw[i]+cc->r2*cd->yw[i]+cc->r3*cd->zw[i]+cc->Tx;
		yc=cc->r4*cd->xw[i]+cc->r5*cd->yw[i]+cc->r6*cd->zw[i]+cc->Ty;
		zc=cc->r7*cd->xw[i]+cc->r8*cd->yw[i]+cc->r9*cd->zw[i]+cc->Tz;
		xerr=xu-cc->f*xc/zc;
		yerr=yu-cc->f*yc/zc;
		error=error+xerr*xerr+yerr*yerr;
	}
	if(error < ftol) return 1;

	return 0;
}

void xeqx1(double a[],double b[],int n)
{
	int i;
	for(i=0;i<n;i++) a[i]=b[i];
}