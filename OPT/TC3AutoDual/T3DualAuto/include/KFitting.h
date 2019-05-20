/***************************************************************
 
 ***************************************************************/
#ifndef KFITTING_H_
#define KFITTING_H_

#ifndef KIL_IDI_DEF_
#define KIL_IDI_DEF_
typedef int KIL_IDI;
#endif

#include "..\\GuiSrc\\lineproc.h"
#ifndef KTYPE32
	#ifdef COMPILING_KFITTINGDLL
		#define KTYPE32 __declspec(dllexport)
	#else
		#define KTYPE32 __declspec(dllimport)
	#endif
#endif

typedef struct {
	double m_nCx;         // fitting data center x
	double m_nCy;         // fitting data center y
	double m_nRadius;     // Fitting data radius
	RECT   m_r;           // Fitting data region  
	POINT  m_Cpt;         // fitting data POINT
}CIRCLE_DATA_GET_STRUCT;

typedef struct {
	int    m_nDataOffset; // data 분할 분모 
	double m_nCtFilter;   // Center point들에 대한 filtering value [0,0.5]
	double m_nLower;      // Radius data에 대한 하한 [0,1]
	double m_nUpper;      // Radius data에 대한 상한 [0,1]
	double m_nSigma;      // DataFiltering factor [0,3]
}CIRCLE_PARAM_STRUCT;

typedef struct{
	POINT sp; // start line point
	POINT ep; // end line point
	double length; // line length
}DIAMETER_DATA_STRUCT;


#ifdef _cplusplus
extern "C"
{
#endif
	/*********************************************************
	 Prototype : int KFT_CircleFitting(RECT r,unsigned char *pImg,
									   int nImgWidth);
	 Function  : 주어진 영역에서 circle fitting을 한다.
	 Parameter : r         -> Circle이 있는 영역 (input)
				 pImg      -> 이진화된 전체 이미지 버퍼 (input)
				 nImfWidth -> image X size
	 Return    : 1 -> success
				 0 -> fail
	 Date      : 2002.11.28
	 Author    : 김양주 
	 Reamrk    :  재 호출 시에 파라미터들은 이전 파라미터들로 변경
				  되어 있어서 상황에 맞게 셋팅이 필요하다.
				  default 값을 쓸경우 setting을 하지 않아도 된다.
	 *********************************************************/
	KTYPE32 int  KFT_CircleFitting(RECT r,unsigned char *pImg,int nImgWidth);
	/*********************************************************
	 Prototype : void KFT_CircleParmeterSet(CIRCLE_PARAM_STRUCT *pData)
	 Function  : Data setting to circle fitting
	 Parameter : pData -> circle data structure
	 Return    : none
	 Date      : 2002.11.28
	 Author    : 김양주 
	 Reamrk    : 
	 *********************************************************/
	KTYPE32 void KFT_CircleParmeterSet(CIRCLE_PARAM_STRUCT *pData);
	/*********************************************************
	 Prototype : void KFT_CircleParmeterGet(CIRCLE_PARAM_STRUCT *pData)
	 Function  : get old fitting parameter 
	 Parameter : pData -> Setting circle data structure(input)
	 Return    : none
	 Date      : 2002.11.28
	 Author    : 김양주 
	 Reamrk    : 
	 *********************************************************/
	KTYPE32 void KFT_CircleParmeterGet(CIRCLE_PARAM_STRUCT *pData);
	/*********************************************************
	 Prototype : void KFT_CircleDataGet(CIRCLE_DATA_GET_STRUCT *pData)
	 Function  : Data setting to circle fitting
	 Parameter : pData -> Getting circle data structure (output)
	 Return    : none
	 Date      : 2002.11.28
	 Author    : 김양주 
	 Reamrk    : 
	 *********************************************************/
	KTYPE32 void KFT_CircleDataGet(CIRCLE_DATA_GET_STRUCT *pData);
	/**********************************************************
	 Prototype : void KFT_GetCircleMinMaxAxisWithFilter(RECR r,
	                  unsigned char *pImg,
					  int nImgWidth,double nCx,double nCy,
					  DIAMETER_DATA_STRUCT *nMinAxis,
					  DIAMETER_DATA_STRUCT *nMaxAxis)
	 Function  : Circle의 장축과 단축을 구한다.
	 Parameter : r        -> Circle bounded rectangle (input)  
				 pImg     -> binarized image (input)
				 nImgWidth-> image width
				 nCx      -> Center Point x(input)
				 nCy      -> Center Point y(input)
				 nMinAxis -> Minimum axis DIAMETER data structure (output)
				 nMaxAxis -> Maximum axis DIAMETER data structure (output)
	 return    : none
	 Date      : 2002.11.29
	 Author    : 김양주 
	 Remark    : none
	 **********************************************************/
	KTYPE32 void KFT_GetCircleMinMaxAxisWithFilter(RECT r,
		                         unsigned char *pImg,
								 int nImgWidth,double nCx, double nCy,
								 DIAMETER_DATA_STRUCT *nMinAxis,
								 DIAMETER_DATA_STRUCT *nMaxAxis);
	/**********************************************************
	 Prototype : void KFT_GetCircleMinMaxAxis(RECR r,
					  unsigned char *pImg,
					  int nImgWidth,double nCx,double nCy,
					  DIAMETER_DATA_STRUCT *nMinAxis,
					  DIAMETER_DATA_STRUCT *nMaxAxis)
	 Function  : Circle의 장축과 단축을 구한다.
	 Parameter : r        -> Circle bounded rectangle (input)  
				 pImg     -> binarized image (input)
				 nImgWidth-> image width
				 nCx      -> Center Point x(input)
				 nCy      -> Center Point y(input)
				 nMinAxis -> Minimum axis DIAMETER data structure (output)
				 nMaxAxis -> Maximum axis DIAMETER data structure (output)
	 return    : none
	 Date      : 2002.11.29
	 Author    : 김양주 
	 Remark    : none
	 **********************************************************/
	KTYPE32 void KFT_GetCircleMinMaxAxis(RECT r,unsigned char *pImg,
		                         int nImgWidth,double nCx,
								 double nCy,DIAMETER_DATA_STRUCT *nMinAxis,
								 DIAMETER_DATA_STRUCT *nMaxAxis);
	/************************************************************
	 Prototype : void KFT_SetDiameterFilterSigma(double nSigma)
	 Function  : Set diameter paramter sigma range for filtering  
				 noise data(upper/lower central limit) 
	 Parameter : nSigma ->[0,3]
	 return    : none
	 Date      : 2002.11.29
	 Author    : 김양주 
	 Remark    : none
	 ************************************************************/
	KTYPE32 void KFT_SetDiameterFilterSigma(double nSigma);
	/************************************************************
	 Prototype : void KFT_GetDiameterFilterSigma(double *nSigma)
	 Function  : Get diameter paramter sigma range for filtering  
				 noise data(upper/lower central limit) 
	 Parameter : *nSigma ->[0,3] (output)
	 return    : none
	 Date      : 2002.11.29
	 Author    : 김양주 
	 Remark    : none
	 ************************************************************/
	KTYPE32 void KFT_GetDiameterFilterSigma(double *nSigma);
	/*************************************************************
	 Prototype : int KFT_GetStartEndPoint(POINT sp,POINT ep,
						  unsigned char *pImg,int nImgWidth,
						  POINT *spt,POINT *ept)
	 Function  : 두점간의 직선을 구하고 위에 있는 점들을 처리하여 
				 처음 위치와 끝 위치를 구한다.
	 Parameter : sp        -> input start point data
				 ep        -> input end point data
				 pImg      -> image buffer(input)
				 nImgWidth -> image width (input)
				 spt       -> output start object point data (output)
				 ept       -> output end object point data object(output)
	 Return    : 1 -> success
				 0 -> fail
	 Date      : 2002.11.30
	 Author    : 김양주 
	 Remark    : None
	 *************************************************************/
	KTYPE32 int KFT_GetStartEndPoint(POINT sp,POINT ep,unsigned char *pImg,
							 int nImgWidth,POINT *spt,POINT *ept);
	/************************************************************************
	 Prototype: int KFT_CircleFittingPoint(POINT *pts,int nNum,double *ncx,
	                                       double *ncy,double *radius)
	 Function : Circle fitting을 한다. 
	 Parameter: pts    -> points for circle fitting
				nNum   -> number of point 
				ncx    -> center point x (output)
				ncy    -> center point y (output)
				radius -> radius (output)
	 Return   : 1 -> Success
				0 -> Fail
	 Author   : Kim Yang Ju
	 Date     : 2002.12.4
	 Remark   : 
	***************************************************************************/
	KTYPE32 int KFT_CircleFittingPoint(POINT *pts,int nNum,double *ncx,double *ncy,
		                               double *radius);

	/************************************************************************
	 Prototype : int KFT_ComputeInnerRadius(RECT r,usrPOINT cpt,unsigned char *pImg,
								   int nImgWidth,DIAMETER_DATA_STRUCT *nTotalData)
	 function  : 모든 반지름의 길이를 측정하고 이를 버퍼에 저장한다. 
	 parameter : r    -> 검사 영역 
				 cpt  -> 중심죄표 
				 pImg -> 이미지 죄표
				nImgWidth -> image width
				nTotalData -> 모든 data를 저장하는 버퍼로 미리 할당이 되어 있어야 하며 그
							 크기는 (width+height)*2이다. (output)

	 return    : number of radius
	 Author    : 김양주 
	 date      : 2002.11.16
	 remark    : none
	 [2002.12.9] make DLL 
	************************************************************************/
	KTYPE32 int KFT_ComputeInnerRadius(RECT r,double nCx,double nCy,
		                               unsigned char *pImg,int nImgWidth,
									   DIAMETER_DATA_STRUCT *nTotalData);

	/************************************************************************
	 Prototype : int KFT_ComputeDonutWidth(RECT r,usrPOINT cpt,unsigned char *pImg,
									   int nImgWidth,DIAMETER_DATA_STRUCT *nTotalData)
	 function  : Donut  두께를 중심점을 지나는 직선을 이용하여 구한다. 
	 parameter : r    -> 검사 영역 
				 cpt  -> 중심죄표 
				 pImg -> 이미지 (input)
				 nImgwidth  -> 이미지 넓이 
				 nTotalData -> 전체 데이터 저장 버퍼 
	 return    : Number of data
	 Author    : 김양주 
	 date      : 2002.11.16
	 remark    : none
	 [2002.12.9] make DLL
	************************************************************************/
	KTYPE32 int KFT_ComputeDonutWidth(RECT r,double nCx,double nCy,unsigned char *pImg,
		                              int nImgWidth,DIAMETER_DATA_STRUCT *nTotalData);

	/*********************************************************
	 Prototype : int KFT_CircleFittingByLS(RECT r,unsigned char *pImg,
								int nImgWidth,double *nCx, double *nCy,
								double *nRadius,int nSizeFilter)
	 Function  : 주어진 영역에서 circle fitting을 한다.
				 Least sqaure circle fitting..
	 Parameter : r         -> Circle이 있는 영역 (input)
				 pImg      -> 이진화된 전체 이미지 버퍼 (input)
				 nImfWidth -> image X size (input)
				 nCx       -> 중심 좌표 X (output)
				 nCy       -> 중심 좌표 Y (output)
				 nRadus    -> 반지름 (output)
				 nSizeFilter -> 크기 기준값 
							   0 -> 크기 기준값 없이 있는 그대로 출력한다. 

	 Return    : 1 -> success
				 0 -> fail
	 Date      : 2002.11.28
	 Author    : 김양주 
	 Reamrk    :  재 호출 시에 파라미터들은 이전 파라미터들로 변경
				  되어 있어서 상황에 맞게 셋팅이 필요하다.
				  default 값을 쓸경우 setting을 하지 않아도 된다.
	 *********************************************************/
	KTYPE32 int KFT_CircleFittingByLS(RECT r,unsigned char *pImg,int nImgWidth,
							  double *nCx,double *nCy,double *nRadius,int nSizeFilter);
	KTYPE32 int KFT_CirleFittingByLs_Edge(RECT r,unsigned char *pImg,int nImgWidth,
		            double *nCx,double *nCy,double *nRadius,int thresh,int nSizeFilter);
	/***************************************************************************************
	 Prototype : void KFT_ComputeCircleByLS(POINT *pts,int nNum,double *nCx,
									double *nCy,double *nRadius)
	 Function  : 원 형태의 점들을 최소 자승법을 이용하여 circle fitting을 한다.
	 Parameters: pts     -> 점 데이터 (input)
				 nNum    -> 점들의 수 (input)
				 nCx     -> 중심점 X  (output)
				 nCy     -> 중심점 Y  (output)
				 nRadius -> 반지름 (output)
	 Return    : none
	 Author    : 김양주 
	 Date      : 2002.
	 Remark    : 
	  [참고]
	  "AN EFFICIENT METHOD OFESTIMATING EDGE LOCATION WITH SUBPIXEL ACCURACY IN NOISY IMAGES"
	****************************************************************************************/
	KTYPE32 void KFT_ComputeCircleByLS(POINT *pts,int nNum,double *nCx,double *nCy,double *nRadius);
	/************************************************************************
	 Prototype : void KFT_LineFittingbyLS(POINT *pt,int nNum,double *nA,double *nB)
	 Function  : 최소자승법을 이용하여 Line fitting을 한다.
				 y=nAx+nB;
	 Parameters: pt   -> Point buffer
				 nNum -> Number of point data
				 nA   -> slope
				 nB   -> constant
	 Return    : none
	 Author    : 김양주 
	 Date      : 2002.12.20
	 Remark    :
	  [참고]
	  "LEAST SQUARE LINE FITTING"
	  Charles R.O'Neill,1999
	************************************************************************/
	KTYPE32 void KFT_LineFittingByLS(POINT *pt,int nNum,double *nA,double *nB);

	/************************************************************************
	 Prototype : void KFT_LineFittingbyLSD(usrPOINT *pt,int nNum,double *nA,double *nB)
	 Function  : 최소자승법을 이용하여 Line fitting을 한다.
				 y=nAx+nB;
	 Parameters: pt   -> Point buffer (double type)
				 nNum -> Number of point data
				 nA   -> slope
				 nB   -> constant
	 Return    : none
	 Author    : 김양주 
	 Date      : 2002.2.27
	 Remark    :
	  [참고]
	  "LEAST SQUARE LINE FITTING"
	  Charles R.O'Neill,1999
	************************************************************************/
	KTYPE32 void KFT_LineFittingByLSD(usrPOINT *pt,int nNum,double *nA,double *nB);

	/************************************************************************
	 Prototype : void KFT_LineFittingByMEDD(usrPOINT *pt,int nNum,double *nA,double *nB)
	 Function  : data를 median filtering하여 Line fitting을 한다.
				 y=nAx+nB;
	 Parameters: pt   -> Point buffer
				 nNum -> Number of point data
				 nA   -> slope
				 nB   -> constant
	 Return    : none
	 Author    : 김양주 
	 Date      : 2002.12.20
	 Remark    :
	************************************************************************/
	KTYPE32 void KFT_LineFittingByMEDD(usrPOINT *pt,int nNum,double *nA,double *nB);

	/************************************************************************
	 Prototype : void KFT_LineFittingByMED(POINT *pt,int nNum,double *nA,double *nB)
	 Function  : data를 median filtering하여 Line fitting을 한다.
				 y=nAx+nB;
	 Parameters: pt   -> Point buffer
				 nNum -> Number of point data
				 nA   -> slope
				 nB   -> constant
	 Return    : none
	 Author    : 김양주 
	 Date      : 2002.12.20
	 Remark    :
	************************************************************************/
	KTYPE32 void KFT_LineFittingByMED(POINT *pt,int nNum,double *nA,double *nB);

	KTYPE32 void KFT_MatrixInverse(int nSx,int nSy,double *nData);
	KTYPE32 void KFT_DataSort(double *pValue,int nNum, int order);
#ifdef _cplusplus
}
#endif
#endif