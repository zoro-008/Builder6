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
	int    m_nDataOffset; // data ���� �и� 
	double m_nCtFilter;   // Center point�鿡 ���� filtering value [0,0.5]
	double m_nLower;      // Radius data�� ���� ���� [0,1]
	double m_nUpper;      // Radius data�� ���� ���� [0,1]
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
	 Function  : �־��� �������� circle fitting�� �Ѵ�.
	 Parameter : r         -> Circle�� �ִ� ���� (input)
				 pImg      -> ����ȭ�� ��ü �̹��� ���� (input)
				 nImfWidth -> image X size
	 Return    : 1 -> success
				 0 -> fail
	 Date      : 2002.11.28
	 Author    : ����� 
	 Reamrk    :  �� ȣ�� �ÿ� �Ķ���͵��� ���� �Ķ���͵�� ����
				  �Ǿ� �־ ��Ȳ�� �°� ������ �ʿ��ϴ�.
				  default ���� ����� setting�� ���� �ʾƵ� �ȴ�.
	 *********************************************************/
	KTYPE32 int  KFT_CircleFitting(RECT r,unsigned char *pImg,int nImgWidth);
	/*********************************************************
	 Prototype : void KFT_CircleParmeterSet(CIRCLE_PARAM_STRUCT *pData)
	 Function  : Data setting to circle fitting
	 Parameter : pData -> circle data structure
	 Return    : none
	 Date      : 2002.11.28
	 Author    : ����� 
	 Reamrk    : 
	 *********************************************************/
	KTYPE32 void KFT_CircleParmeterSet(CIRCLE_PARAM_STRUCT *pData);
	/*********************************************************
	 Prototype : void KFT_CircleParmeterGet(CIRCLE_PARAM_STRUCT *pData)
	 Function  : get old fitting parameter 
	 Parameter : pData -> Setting circle data structure(input)
	 Return    : none
	 Date      : 2002.11.28
	 Author    : ����� 
	 Reamrk    : 
	 *********************************************************/
	KTYPE32 void KFT_CircleParmeterGet(CIRCLE_PARAM_STRUCT *pData);
	/*********************************************************
	 Prototype : void KFT_CircleDataGet(CIRCLE_DATA_GET_STRUCT *pData)
	 Function  : Data setting to circle fitting
	 Parameter : pData -> Getting circle data structure (output)
	 Return    : none
	 Date      : 2002.11.28
	 Author    : ����� 
	 Reamrk    : 
	 *********************************************************/
	KTYPE32 void KFT_CircleDataGet(CIRCLE_DATA_GET_STRUCT *pData);
	/**********************************************************
	 Prototype : void KFT_GetCircleMinMaxAxisWithFilter(RECR r,
	                  unsigned char *pImg,
					  int nImgWidth,double nCx,double nCy,
					  DIAMETER_DATA_STRUCT *nMinAxis,
					  DIAMETER_DATA_STRUCT *nMaxAxis)
	 Function  : Circle�� ����� ������ ���Ѵ�.
	 Parameter : r        -> Circle bounded rectangle (input)  
				 pImg     -> binarized image (input)
				 nImgWidth-> image width
				 nCx      -> Center Point x(input)
				 nCy      -> Center Point y(input)
				 nMinAxis -> Minimum axis DIAMETER data structure (output)
				 nMaxAxis -> Maximum axis DIAMETER data structure (output)
	 return    : none
	 Date      : 2002.11.29
	 Author    : ����� 
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
	 Function  : Circle�� ����� ������ ���Ѵ�.
	 Parameter : r        -> Circle bounded rectangle (input)  
				 pImg     -> binarized image (input)
				 nImgWidth-> image width
				 nCx      -> Center Point x(input)
				 nCy      -> Center Point y(input)
				 nMinAxis -> Minimum axis DIAMETER data structure (output)
				 nMaxAxis -> Maximum axis DIAMETER data structure (output)
	 return    : none
	 Date      : 2002.11.29
	 Author    : ����� 
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
	 Author    : ����� 
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
	 Author    : ����� 
	 Remark    : none
	 ************************************************************/
	KTYPE32 void KFT_GetDiameterFilterSigma(double *nSigma);
	/*************************************************************
	 Prototype : int KFT_GetStartEndPoint(POINT sp,POINT ep,
						  unsigned char *pImg,int nImgWidth,
						  POINT *spt,POINT *ept)
	 Function  : �������� ������ ���ϰ� ���� �ִ� ������ ó���Ͽ� 
				 ó�� ��ġ�� �� ��ġ�� ���Ѵ�.
	 Parameter : sp        -> input start point data
				 ep        -> input end point data
				 pImg      -> image buffer(input)
				 nImgWidth -> image width (input)
				 spt       -> output start object point data (output)
				 ept       -> output end object point data object(output)
	 Return    : 1 -> success
				 0 -> fail
	 Date      : 2002.11.30
	 Author    : ����� 
	 Remark    : None
	 *************************************************************/
	KTYPE32 int KFT_GetStartEndPoint(POINT sp,POINT ep,unsigned char *pImg,
							 int nImgWidth,POINT *spt,POINT *ept);
	/************************************************************************
	 Prototype: int KFT_CircleFittingPoint(POINT *pts,int nNum,double *ncx,
	                                       double *ncy,double *radius)
	 Function : Circle fitting�� �Ѵ�. 
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
	 function  : ��� �������� ���̸� �����ϰ� �̸� ���ۿ� �����Ѵ�. 
	 parameter : r    -> �˻� ���� 
				 cpt  -> �߽���ǥ 
				 pImg -> �̹��� ��ǥ
				nImgWidth -> image width
				nTotalData -> ��� data�� �����ϴ� ���۷� �̸� �Ҵ��� �Ǿ� �־�� �ϸ� ��
							 ũ��� (width+height)*2�̴�. (output)

	 return    : number of radius
	 Author    : ����� 
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
	 function  : Donut  �β��� �߽����� ������ ������ �̿��Ͽ� ���Ѵ�. 
	 parameter : r    -> �˻� ���� 
				 cpt  -> �߽���ǥ 
				 pImg -> �̹��� (input)
				 nImgwidth  -> �̹��� ���� 
				 nTotalData -> ��ü ������ ���� ���� 
	 return    : Number of data
	 Author    : ����� 
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
	 Function  : �־��� �������� circle fitting�� �Ѵ�.
				 Least sqaure circle fitting..
	 Parameter : r         -> Circle�� �ִ� ���� (input)
				 pImg      -> ����ȭ�� ��ü �̹��� ���� (input)
				 nImfWidth -> image X size (input)
				 nCx       -> �߽� ��ǥ X (output)
				 nCy       -> �߽� ��ǥ Y (output)
				 nRadus    -> ������ (output)
				 nSizeFilter -> ũ�� ���ذ� 
							   0 -> ũ�� ���ذ� ���� �ִ� �״�� ����Ѵ�. 

	 Return    : 1 -> success
				 0 -> fail
	 Date      : 2002.11.28
	 Author    : ����� 
	 Reamrk    :  �� ȣ�� �ÿ� �Ķ���͵��� ���� �Ķ���͵�� ����
				  �Ǿ� �־ ��Ȳ�� �°� ������ �ʿ��ϴ�.
				  default ���� ����� setting�� ���� �ʾƵ� �ȴ�.
	 *********************************************************/
	KTYPE32 int KFT_CircleFittingByLS(RECT r,unsigned char *pImg,int nImgWidth,
							  double *nCx,double *nCy,double *nRadius,int nSizeFilter);
	KTYPE32 int KFT_CirleFittingByLs_Edge(RECT r,unsigned char *pImg,int nImgWidth,
		            double *nCx,double *nCy,double *nRadius,int thresh,int nSizeFilter);
	/***************************************************************************************
	 Prototype : void KFT_ComputeCircleByLS(POINT *pts,int nNum,double *nCx,
									double *nCy,double *nRadius)
	 Function  : �� ������ ������ �ּ� �ڽ¹��� �̿��Ͽ� circle fitting�� �Ѵ�.
	 Parameters: pts     -> �� ������ (input)
				 nNum    -> ������ �� (input)
				 nCx     -> �߽��� X  (output)
				 nCy     -> �߽��� Y  (output)
				 nRadius -> ������ (output)
	 Return    : none
	 Author    : ����� 
	 Date      : 2002.
	 Remark    : 
	  [����]
	  "AN EFFICIENT METHOD OFESTIMATING EDGE LOCATION WITH SUBPIXEL ACCURACY IN NOISY IMAGES"
	****************************************************************************************/
	KTYPE32 void KFT_ComputeCircleByLS(POINT *pts,int nNum,double *nCx,double *nCy,double *nRadius);
	/************************************************************************
	 Prototype : void KFT_LineFittingbyLS(POINT *pt,int nNum,double *nA,double *nB)
	 Function  : �ּ��ڽ¹��� �̿��Ͽ� Line fitting�� �Ѵ�.
				 y=nAx+nB;
	 Parameters: pt   -> Point buffer
				 nNum -> Number of point data
				 nA   -> slope
				 nB   -> constant
	 Return    : none
	 Author    : ����� 
	 Date      : 2002.12.20
	 Remark    :
	  [����]
	  "LEAST SQUARE LINE FITTING"
	  Charles R.O'Neill,1999
	************************************************************************/
	KTYPE32 void KFT_LineFittingByLS(POINT *pt,int nNum,double *nA,double *nB);

	/************************************************************************
	 Prototype : void KFT_LineFittingbyLSD(usrPOINT *pt,int nNum,double *nA,double *nB)
	 Function  : �ּ��ڽ¹��� �̿��Ͽ� Line fitting�� �Ѵ�.
				 y=nAx+nB;
	 Parameters: pt   -> Point buffer (double type)
				 nNum -> Number of point data
				 nA   -> slope
				 nB   -> constant
	 Return    : none
	 Author    : ����� 
	 Date      : 2002.2.27
	 Remark    :
	  [����]
	  "LEAST SQUARE LINE FITTING"
	  Charles R.O'Neill,1999
	************************************************************************/
	KTYPE32 void KFT_LineFittingByLSD(usrPOINT *pt,int nNum,double *nA,double *nB);

	/************************************************************************
	 Prototype : void KFT_LineFittingByMEDD(usrPOINT *pt,int nNum,double *nA,double *nB)
	 Function  : data�� median filtering�Ͽ� Line fitting�� �Ѵ�.
				 y=nAx+nB;
	 Parameters: pt   -> Point buffer
				 nNum -> Number of point data
				 nA   -> slope
				 nB   -> constant
	 Return    : none
	 Author    : ����� 
	 Date      : 2002.12.20
	 Remark    :
	************************************************************************/
	KTYPE32 void KFT_LineFittingByMEDD(usrPOINT *pt,int nNum,double *nA,double *nB);

	/************************************************************************
	 Prototype : void KFT_LineFittingByMED(POINT *pt,int nNum,double *nA,double *nB)
	 Function  : data�� median filtering�Ͽ� Line fitting�� �Ѵ�.
				 y=nAx+nB;
	 Parameters: pt   -> Point buffer
				 nNum -> Number of point data
				 nA   -> slope
				 nB   -> constant
	 Return    : none
	 Author    : ����� 
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