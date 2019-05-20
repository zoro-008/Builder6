#ifndef __KMOMENT_H_
#define __KMOMENT_H_

#ifndef KIL_ID_DEF_
#define KIL_ID_DEF_
typedef long KIL_ID;
#endif

#ifndef KTYPE32
	#ifdef COMPILING_KMOMENTDLL
		#define KTYPE32 __declspec(dllexport)
	#else
		#define KTYPE32 __declspec(dllimport)
	#endif
#endif

// Parameter list..
#define KMOM_ROI        0 // RECT
#define KMOM_IMGPOINTER 1 // unsigned char *
#define KMOM_IMGDIM     2 // long width, long height
#define KMOM_OBJCOLOR   3 // int  0 : White,  1 : Black
#define KMOM_TYPE       4 // int  0 : binary, 1 : gray

// Feature list..
#define KMOM_CENTROID     0 // double X,double Y
#define KMOM_ECCENTRICITY 1 // double
#define KMOM_THETA        2 // double
#define KMOM_AXIS         3 // double, double
#define KMOM_ELLIPSE      4 // double(장축), double (단축)
#define KMOM_INVARIANT    5 // double a[7]...


typedef struct {
	double nCx;
	double nCy;
	double nTheta;
	double nEccentricity;
	double nLamdaMax;
	double nLamdaMin;
	double nLongAxis;
	double nShortAxis;
	double nHu[7];
}MOMENT_RESULT;

typedef struct{
	RECT r;
	unsigned char *pImg;
	long w;
	long h;
	int objcolor;
	int ntype; // 0: binary, 1: grey
}MOMENT_PARAM;

#ifdef _cplusplus
extern "C"
{
#endif
KTYPE32 void   KmomSetParamStructure(MOMENT_PARAM cparam);
KTYPE32 void   KmomSetParam(int nParamType,void *pUsrPtr1,void *pUsrPtr2=NULL);
KTYPE32 void   KmomGetParam(int nParamType,void *pUsrPtr1,void *pUsrPtr2=NULL);
KTYPE32 void   KmomCompute();
KTYPE32 int    KmomComputeWithEllipse();
KTYPE32 void   KmomGetFeature(int nParamType, void *pUsrPtr1,void *pUsrPtr2=NULL);
/**********************
 Setting required !!! 
 For higher order computation...
 **********************/
KTYPE32 double KmomBinMoment(int nXoder, int nYorder);
KTYPE32 double KmomCenteralMoment(int nXoder, int nYorder);
/***********************************************************************
 Prototype : int KmomComputeFeature(unsigned char *pSrc,int nWidth,RECT r,
                 int nObjColor,int nFlag)
 Function  : Compute data related to moment 
 Prototype : pSrc      -> Source image buffer
             nWidth    -> image width
			 r         -> ROI, region of interest
			 nObjColor -> Object color , 0 : white, 1 : black
			 nFalg     -> operation mode, 
			              0 : simple operation 
			              1 : HU's moment invariant feature
						  2 : Best fit ellipse
						  3 : Hu & best fit ellipse
 Return    : Don't care
 Author    : Kim Yang Ju
 Date      : 2003.1.8
 Remark    : none
 ***********************************************************************/
KTYPE32 void KmomComputeFeature(unsigned char *pSrc,int nWidth,RECT r,int nObjColor,int nFlag);
/***********************************************************************
 Prototype : void KmomComputeFeatureWithPoint(POINT *pts,int nNum,int nFlag)
 Function  : Compute data related to moment 
 Prototype : pts       -> Source point buffer
			 nFalg     -> operation mode, 
			              0 : simple operation 
			              1 : HU's moment invariant feature
						  2 : Best fit ellipse
						  3 : Hu & best fit ellipse
 Return    : Don't care
 Author    : Kim Yang Ju
 Date      : 2003.1.8
 Remark    : none
 ***********************************************************************/
KTYPE32 void KmomComputeFeatureWithPoint(POINT *pts,int nNum,int nFlag);
/***********************************************************************
 Prototype : void KmomGetFeatureResult(MOMENT_RESULT *pResult)
 Function  : Moment를 계산한 결과 모두를 얻는다.
             반드시 미리 계산이 모두 완료되어야 한다. 
 Parameters: pResult -> result variables [out]
 Return    : none
 Author    : Kim Yang Ju
 Date      : 2003.1.8
 Remark    : none
 ***********************************************************************/
KTYPE32 void KmomGetFeatureResult(MOMENT_RESULT *pResult);
#ifdef _cplusplus
}
#endif

#endif
