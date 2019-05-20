/*********************************
 (주)한라정밀 엔지니어링 
 
 만든이  : 김양주 
 날짜    : 2001/6/27,2002/2/27,2002/2/28
 Version : v1.01
 기능    : 이진화 영상처리에 관련한 알고리즘 구현. 
		   Minimum error Auto thresholding ( debugging required!!!! ),
		   오츠의 Auto- thresholding
   		   SHIFT 이미지 이진화 
*********************************/
#ifndef __KTHRESH_H_
#define __KTHRESH_H_
typedef long KIL_ID;

#ifndef KTYPE32
#ifdef COMPILING_KTHRESHDLL
#define KTYPE32 __declspec(dllexport)
#else
#define KTYPE32 __declspec(dllimport)
#endif
#endif
//nMethod...
#define MINERROR_THRESHOLD  0 // minimum error thresholding
#define OTSU_THRESHOLD      1 // maximize between-group variance
#define ABSOLUTE_THRESHOLD  2 // single level binarize
#define SHIFT_THRESHOLD     3 // binarize using shift operation
#define ITERATIVE_THRESHOLD 4 // iterative selection method

 
//parameter list
#define THRESH_PARAM_THRESH 0 // Single thresholding value (0 - 255) : int
#define THRESH_PARAM_BWSET  1 // object inverse flag set (0 --> NORMAL or 1->Inverse) : int
#define THRESH_PARAM_DIM    2 // image dimension set(data1 ->Width, data2 ->Height, long, long
#define THRESH_PARAM_FREQ   3 // frequency (frequency : 256) int
#define THRESH_PARAM_PROB   4 // probability (gray level probability, 256) double
#define THRESH_PARAM_THRESH_CF 5 // thresh +alpha ( double (0 - 1.0)) double
#define THRESH_PARAM_SHIFTBIT 6 // shift bit(0 - 8) int

#ifdef __cplusplus
extern "C"
{
#endif
	KTYPE32 int SetThreshParam(int nParamType,void *pUsrData1,void *pUsrData2);
	KTYPE32 int GetThreshParam(int nParamType,void *pUsrData1,void *pUsrData2);
	KTYPE32 int GlobalBinarize(BYTE *pSrc, BYTE *pTgt,long nWidth,long nHeight,int nMethod);
	KTYPE32 int ROIBinarize(BYTE *pSrc,BYTE *pTgt,long nWidth,long nHeight,long nSx,long nSy,long nEx,long nEy,int nMethod);
#ifdef __cplusplus
}
#endif
#endif

