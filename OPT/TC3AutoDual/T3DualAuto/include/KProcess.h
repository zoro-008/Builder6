/***************************************************************
 file name   : KProcess.h
 author      : Kim Yang Ju
 description : gerneral purpose image processing library.
               experimental processing routines included.
			   donut processing, circle fitting, line fitting,
			   edge detection, projection, thinning, chain-code,
			   binarization, filter, zoom in, zoom out.
 history     : 2003.7.3 , KIP_ZoomOutImage(), added by KYJ.
 ***************************************************************/
#ifndef KPROCESS_H_
#define KPROCESS_H_

#ifndef KIL_IDI_DEF_
#define KIL_IDI_DEF_
typedef int KIL_IDI;
#endif
#include "..\\guisrc\\lineproc.h"


#ifndef KTYPE32
	#ifdef COMPILING_KPROCESSDLL
		#define KTYPE32 __declspec(dllexport)
	#else
		#define KTYPE32 __declspec(dllimport)
	#endif
#endif


#ifdef _cplusplus
extern "C"
{
#endif
	/*************************************************************
	 Prototype : void KIP_ProcessInnerCircle(unsigned char *pSrc,unsigned char *pTgt,
						  RECT r,int nImgWidth, double nCx,double nCy,
						  double r1, int thresh,int flag)
	 Function  : r1보다 작은 원 여역을 이진화한다. 
				 Donut region.
	 Parameter : *pSrc -> Source image buffer (input)
				 *pTgt -> Target image buffer (output)
				 r     -> Region of interest (input)
				 nImgWidth -> image width (input)
				 nCx   -> Center point X (input)
				 nCy   -> Center point Y (input)
				 r1    -> 외원의 반지름 (input)
				 thresh-> Threshold value for bainarization(input)
				 flag  -> Selection flag 
					  (input, 0: Sobel edge operator, 1: binarize black, 2: binarize white)
	 Return     : none
	 Date       : 2002.12.9
	 Author     : 김 양주 
	 Remark     : made for rod guide
	 [2002.12.9] modified for DLL
	****************************************************************/
	KTYPE32 void KIP_ProcessInnerCircle(unsigned char *pSrc,unsigned char *pTgt,
						  RECT r,int nImgWidth, double nCx,double nCy,
						  double r1, int thresh,int flag);

	/*************************************************************
	 Prototype : void KIP_ProcessDonut(unsigned char *pSrc,unsigned char *pTgt,
						  RECT r,int nImgWidth, double nCx,double nCy,
						  double r1,double r2, int thresh,int flag)
	 Function  : r1보다 작고 r2 보다 큰 영역을 처리한다.
	             
				 Donut region.
	 Parameter : *pSrc -> Source image buffer (input)
				 *pTgt -> Target image buffer (output)
				 r     -> Region of interest (input)
				 nImgWidth -> image width (input)
				 nCx   -> Center point X (input)
				 nCy   -> Center point Y (input)
				 r1    -> 외원의 반지름 (input)
				 r2    -> 내원의 반지름의 크기 (input)
				 thresh-> Threshold value for bainarization(input)
				 flag  -> Selection flag 
					  (input, 0: Sobel edge operator, 1: binarize black, 2: binarize white)
	 Return     : none
	 Date       : 2002.
	 Author     : 김 양주 
	 Remark     : made for bearing & gear inspection
	 [2002.11.30] modified for DLL
	****************************************************************/
	KTYPE32 void KIP_ProcessDonut(unsigned char *pSrc,unsigned char *pTgt,
						  RECT r,int nImgWidth, double nCx,double nCy,
						  double r1,double r2, int thresh,int flag);

	/*************************************************************
	 Prototype :void KIP_Sobel(unsigned char *pSrc,
							   unsigned char *pTgt,
							   RECT r, int nWidth,int nThresh)
	 Function  : sobel edge detection
	 Parameter : pSrc    -> source image buffer (input)
				 pTgt    -> target image buffer (output)
				 r       -> region of interest
				 nWidth  -> image width
				 nThresh -> thresholding value
	 Return    : none
	 Author    : 김양주 
	 Date      : 2002.
	 Remark    : none
	**************************************************************/
	KTYPE32 void KIP_Sobel(unsigned char *pSrc,unsigned char *pTgt,
		                   RECT r, int nWidth,int nThresh);
	/************************************************************
	 Prototype  : void KIP_HorizEdge(unsigned char *pSrc,
								 unsigned char *pTgt,
								 RECT r,int nWidth,int nThresh)
	 Function   : Edge detection using horizontal gradient difference.
				  Make binarized image
	 Parameters : pSrc    -> Source buffer (input)
				  pTgt    -> Target buffer (output) : memset required!
				  r       -> region of interest
				  nWidth  -> Input/Output image width
				  nThresh -> Thresholding value.
	 Return     : none
	 Author     : 김양주 
	 Date       : 2002.12.23
	 Remark     ; 
	*************************************************************/
	KTYPE32 void KIP_HorizEdge(unsigned char *pSrc,unsigned char *pTgt,
							 RECT r,int nWidth,int nThresh);
	/************************************************************
	 Prototype  : void KIP_VertEdge(unsigned char *pSrc,
								 unsigned char *pTgt,
								 RECT r,int nWidth,int nThresh)
	 Function   : Edge detection using vetical gradient difference.
				  Make binarized image
	 Parameters : pSrc    -> Source buffer (input)
				  pTgt    -> Target buffer (output) : memset required!
				  r       -> region of interest
				  nWidth  -> Input/Output image width
				  nThresh -> Thresholding value.
	 Return     : none
	 Author     : 김양주 
	 Date       : 2002.12.23
	 Remark      
	*************************************************************/
	KTYPE32 void KIP_VertEdge(unsigned char *pSrc,unsigned char *pTgt,
							 RECT r,int nWidth,int nThresh);
	/************************************************************
	 Prototype  : void KIP_VertProjection(unsigned char *pSrc,
								 int *pProjV,RECT r,int nWidth)
	 Function   : Vertical binarized image projection.
	 Parameters : pSrc    -> Source buffer (input)
				  pProjV  -> Projection value buffer (output) : memset required!
				  r       -> region of interest
				  nWidth  -> Input image width
	 Return     : none
	 Author     : 김양주 
	 Date       : 2002.12.23
	 Remark      
	*************************************************************/
	KTYPE32 void KIP_VertProjection(unsigned char *pSrc,int *pProjV,RECT r,int nWidth);
	/************************************************************
	 Prototype  : void KIP_HorizProjection(unsigned char *pSrc,
									  int *pProjV,RECT r,int nWidth)
	 Function   : Horizontal binarized image projection.
	 Parameters : pSrc    -> Source buffer (input)
				  pProjV  -> Projection value buffer (output) 
							 memset required!
				  r       -> region of interest
				  nWidth  -> Input image width
	 Return     : none
	 Author     : 김양주 
	 Date       : 2002.12.23
	 Remark     
	*************************************************************/
	KTYPE32 void KIP_HorizProjection(unsigned char *pSrc,int *pProjV,RECT r,int nWidth);
	/****************************************************************************
	 Prototype : int KIP_GetHorizEdgePoints(unsigned char *pSrc,RECT r,
										double *ptx,double *pty)
	 Function  : 수직 방향의 Moment edge를 수평 방향 크기 만큼 구한다. 
	             STEP EDGE일경우 사용가능. 그밖에는 오류 발생.
	 Parameters: pSrc  -> Source image buffer (input)
				 r     -> ROI                 (input)
				 nWidth-> image width         (input)
				 ptx   -> edge point x        (output)
				 pty   -> edge point y        (output)
	 Return    :
	 Author    : 김양주 
	 Date      : 2002.12.24
	 Remark    : 
	*****************************************************************************/
	KTYPE32 int  KIP_GetHorizEdgePoints(unsigned char *pSrc,RECT r,int nWidth,double *ptx,double *pty);
	/****************************************************************************
	 Prototype : int KIP_GetHorizMAxGradientEdgePoints(unsigned char *pSrc,RECT r,POINT *pts)
	 Function  : 수직 방향의 edge를 수평 방향 크기 만큼 구한다. 
	 Parameters: pSrc  -> Source image buffer (input)
				 r     -> ROI                 (input)
				 nWidth-> image width         (input)
				 pts   -> edge points         (output)
						  Must be memory allocated! 
	 Return    : 
	 Author    : 김양주 
	 Date      : 2002.12.24
	 Remark    : 
	*****************************************************************************/
	KTYPE32 int KIP_GetHorizMaxGradientEdgePoints(unsigned char *pSrc,RECT r,int nWidth,POINT *pts);

	/********************************************************************
	 Prototype : void KIP_BinarizeAll(unsigned char *pSrc,unsigned char *pOut,
							int nWidth,int nHeight, int flag, int thresh)
	 Function  : absolutely binarize all image.
	 Parameters: pSrc    -> Source image buffer (input)
				 pOut    -> Target image buffer (output)
				 nWidth  -> image width  (input)
				 nHeight -> image height (input)
				 flag    -> inverse tag 1 -> white object, 0 -> dark object
				 thresh  -> thresholding value
	 Return    : none
	 Author    : 김 양주 
	 Date      : 2002.12.27
	 Reamrk    : none
	*********************************************************************/
	KTYPE32 void KIP_BinarizeAll(unsigned char *pSrc,unsigned char *pOut,
		                         int nWidth,int nHeight,int flag, int thresh);

	/****************************************************************************
	 Prototype : void KIP_GetHorizGradientEdge(unsigned char *pSrc,unsigned char *pOut,
												 int nWidth,int nHeight,int nThresh)
	 Function  : 수직 방향의 edge를 구한다. 
	 Parameters: pSrc    -> Source image buffer(input)
				 pOut    -> Target image buffer(output)
				 nWidth  -> image width        (input)
				 nHeight -> image height       (input)
				 nThresh -> Thresholding value (input)
	 Return    : none
	 Author    : 김양주 
	 Date      : 2002.12.27
	 Remark    : 
	*****************************************************************************/
	KTYPE32 void KIP_GetHorizGradientEdge(unsigned char *pSrc,unsigned char *pOut,
		                                  int nWidth,int nHeight,int nThresh);

	/****************************************************************************
	 Prototype : int KIP_GetGradientEdgePoints(unsigned char *pSrc,RECT r,
									 int nWidth,POINT *pts,int nThresh,int nDir)
	 Function  : 수직 방향의 edge를 수평 방향 크기 만큼 구한다. 
				 Gradient가 threshold 값보다 클경우 다음으로 ...
				 한라인의 edge 점들을 얻는다.
	 Parameters: pSrc    -> Source image buffer (input)
				 r       -> ROI                 (input)
				 nWidth  -> image width         (input)
				 pts     -> edge points         (output)
							Must be memory allocated! 
				 nThresh -> Edge thresholding value
				 nDir    -> edge direction
							0: top  -> bottom, 1: bottom -> top
							2: left -> right,  3: right  -> left
						
	 Return    : Number of point
	 Author    : 김 양주 
	 Date      : 2003.1.2
	 Remark    : 
	*****************************************************************************/
	KTYPE32 int KIP_GetGradientEdgePoints(unsigned char *pSrc,RECT r,int nWidth,
											 POINT *pts,int nThresh,int nDir);
	/****************************************************************************
	 Prototype : int KIP_GetSobelEdgePoints(unsigned char *pSrc,RECT r,
									 int nWidth,POINT *pts,int nThresh,int nDir)
	 Function  : 수직 방향의 edge를 수평 방향 크기 만큼 구한다. 
				 Gradient가 threshold 값보다 클경우 다음으로 ...
				 한라인의 edge 점들을 얻는다.
	 Parameters: pSrc    -> Source image buffer (input)
				 r       -> ROI                 (input)
				 nWidth  -> image width         (input)
				 pts     -> edge points         (output)
							Must be memory allocated! 
				 nThresh -> Edge thresholding value
				 nDir    -> edge direction
							0: top  -> bottom, 1: bottom -> top
							2: left -> right,  3: right  -> left
						
	 Return    : Number of point
	 Author    : 김 양주 
	 Date      : 2003.1.2
	 Remark    : 
	*****************************************************************************/
	KTYPE32 int KIP_GetSobelEdgePoints(unsigned char *pSrc,RECT r,int nWidth,
											 POINT *pts,int nThresh,int nDir);
	/****************************************************************************
	 Prototype : int KIP_GetBinaryEdgePoints(unsigned char *pSrc,RECT r,
									 int nWidth,POINT *pts,int nThresh,int nDir)
	 Function  : 이진화된 이미지에서 또는 gray 이미지에서의 에지 추출 .  
				 하나의 라인 에지점들을 얻는다.
	 Parameters: pSrc  -> Source image buffer (input)
				 r     -> ROI                 (input)
				 nWidth-> image width         (input)
				 pts   -> edge points         (output)
						  Must be memory allocated! 
				 nThresh-> Edge thresholding value
				 nDir  -> edge direction
						  0: top  -> bottom, 1: bottom -> top
						  2: left -> right,  3: right  -> left
							
	 Return    : Number of point
	 Author    : 김 양주 
	 Date      : 2003.1.2
	 Remark    : 
	*****************************************************************************/
	KTYPE32 int KIP_GetBinaryEdgePoints(unsigned char *pSrc,RECT r,int nWidth,
										POINT *pts,int nThresh,int nDir);
	/****************************************************************************
	 Prototype : int KIP_GetCenterLineGradientEdgePoints(unsigned char *pSrc,POINT spt,
					 POINT ept,int nWidth,int offset,int nThresh,POINT *pts,int nDir)
	 Function  : Center line을 중심으로 위 아래 offset 만큼의 영역 내에서 
				 에지 점들을 찾는다. 
	 Parameters: pSrc   -> Source image buffer (input)
				 spt    -> start point         (input)
				 ept    -> end point           (input)
				 nWidth -> image width         (input)
				 offset -> +/- pixel number 
				 pts    -> edge points         (output)
						   Must be memory allocated! 
				 nThresh-> Edge thresholding value
				 nDir   -> edge direction
						  0: top  -> bottom, 1: bottom -> top
						  2: left -> right,  3: right  -> left
	 Return    : Number of point
	 Author    : 김 양주 
	 Date      : 2003.1.3
	 Remark    : none
	*****************************************************************************/
	KTYPE32 int KIP_GetCenterLineGradientEdgePoints(unsigned char *pSrc,POINT spt,POINT ept,
							int nWidth,int offset,int nThresh,POINT *pts,int nDir);
	/****************************************************************************
	 Prototype : int KIP_GetCenterLineGradientEdgePointsWithWeight(unsigned char *pSrc,POINT spt,
					 POINT ept,int nWidth,int offset,int nThresh,POINT *pts,int nDir)
	 Function  : - Center line을 중심으로 위 아래 offset 만큼의 영역 내에서 
				   에지 점들을 찾는다. 
				 - Gradient differencing을 사용한다.
				 - 3pixel을 이용하여 weight를 정하고 이를 이용하여 정확한 에지점을 구한다. 

	 Parameters: pSrc   -> Source image buffer (input)
				 spt    -> start point         (input)
				 ept    -> end point           (input)
				 nWidth -> image width         (input)
				 offset -> +/- pixel number 
				 pts    -> edge points         (output)
						   Must be memory allocated! 
				 nThresh-> Edge thresholding value
				 nDir   -> edge direction
						  0: top  -> bottom, 1: bottom -> top
						  2: left -> right,  3: right  -> left
	 Return    : Number of point
	 Author    : 김 양주 
	 Date      : 2003.2.26
	 Remark    : none
	*****************************************************************************/
	KTYPE32 int KIP_GetCenterLineGradientEdgePointsWithWeight(unsigned char *pSrc,POINT spt,POINT ept,
							int nWidth,int offset,int nThresh,POINT *pts,int nDir);
	/****************************************************************************
	 Prototype : int KIP_GetCenterLineGradientEdgePointsWithWeightD(unsigned char *pSrc,POINT spt,
					 POINT ept,int nWidth,int offset,int nThresh,usrPOINT *pts,int nDir)
	 Function  : - Center line을 중심으로 위 아래 offset 만큼의 영역 내에서 
				   에지 점들을 찾는다. 
				 - Gradient differencing을 사용한다.
				 - 3pixel을 이용하여 weight를 정하고 이를 이용하여 정확한 에지점을 구한다. 

	 Parameters: pSrc   -> Source image buffer (input)
				 spt    -> start point         (input)
				 ept    -> end point           (input)
				 nWidth -> image width         (input)
				 offset -> +/- pixel number 
				 pts    -> edge points         (output) (double type points)
						   Must be memory allocated! 
				 nThresh-> Edge thresholding value
				 nDir   -> edge direction
						  0: top  -> bottom, 1: bottom -> top
						  2: left -> right,  3: right  -> left
	 Return    : Number of point
	 Author    : 김 양주 
	 Date      : 2003.2.27
	 Remark    : none
	*****************************************************************************/
	KTYPE32 int KIP_GetCenterLineGradientEdgePointsWithWeightD(unsigned char *pSrc,POINT spt,POINT ept,
							int nWidth,int offset,int nThresh,usrPOINT *pts,int nDir);

	/****************************************************************************
	 Prototype : int KIP_GetCenterLineBinaryEdgePoints(unsigned char *pSrc,POINT spt,
					 POINT ept,int nWidth,int offset,int nThresh,POINT *pts,int nDir)
	 Function  : Center line을 중심으로 위 아래 offset 만큼의 영역 내에서 
				 에지 점들을 찾는다. 
				 - 이진화 방법을 이용한다.
	 Parameters: pSrc   -> Source image buffer (input)
				 spt    -> start point         (input)
				 ept    -> end point           (input)
				 nWidth -> image width         (input)
				 offset -> +/- pixel number 
				 pts    -> edge points         (output)
						   Must be memory allocated! 
				 nThresh-> Edge thresholding value
				 nDir   -> edge direction
						  0: top  -> bottom, 1: bottom -> top
						  2: left -> right,  3: right  -> left
	 Return    : Number of point
	 Author    : 김 양주 
	 Date      : 2003.1.7
	 Remark    : none
	*****************************************************************************/
	KTYPE32 int KIP_GetCenterLineBinaryEdgePoints(unsigned char *pSrc,POINT spt,POINT ept,
							int nWidth,int offset,int nThresh,POINT *pts,int nDir);
	/*******************************************************************
	 Prototype : int KIP_Robert(unsigned char *pSrc, unsigned char *pTgt,
							 RECT r,int nImgWidth,int nThresh)
	 Function  : Robert edge operation
	 Parameter : pSrc       -> Source image buffer [in]
				 pTgt       -> Target image buffer [out] , alloction required!
							   Must be memory cleared.
				 r          -> ROI
				 nImgWidth  -> Image width
				 nThresh    -> Thresholding value
	 Return    : Number of pixel
	 Author    : Kim Yang Ju
	 Date      : 2003.1.13
	 Reamrk    : none
	 *******************************************************************/
	KTYPE32 int KIP_Robert(unsigned char *pSrc,unsigned char *pTgt,RECT r,
		                   int nImgWidth,int nThresh);
	/**********************************************************************
	 Prototype : void KIP_MakeImageFromPoints(POINT *pts,unsigned char *pTgt,
										int nNum,int nImgWidth,int nValue)
	 Function  : 주어진 점들을 이용하여 이미지를 만든다.
	 Parameter : pts       -> point data        [in]
				 pTgt      -> target image data [out]
				 nNum      -> number of point   [in]
				 nImgWidth -> image width       [in]
				 nValue    -> image value 8bit  [in]
	 Return    : none
	 Author    : Kim Yang ju
	 Date      : 2003.1.13
	 Remark    : none
	 **********************************************************************/
	KTYPE32 void KIP_MakeImageFromPoints(POINT *pts,unsigned char *pTgt,
		                                int nNum,int nImgWidth,int nValue);
	/***********************************************************************
	 Prototype : void KIP_ThinningWithDeutschAlgorithm(unsigned char *pSrc,
					  unsigned char *pTgt,int nOnceOnly)
	 Function  : Deutsch Algorithm을 이용하여 thinning을 한다. 
	 Parameters: pSrc -> input image buffer [in]
				 pTgt -> output image buffer[out]
				 nOnceOnly -> 1 : 1 iteration, 0: iteration all
	 Return    : none
	 Author    : Kim Yang Ju
	 Date      : 2003.1.13
	 Remark    : none
	 ***********************************************************************/
	KTYPE32 void KIP_ThinningWithDeutschAlgorithm(unsigned char *pSrc,
					  unsigned char *pTgt,int nWidth,int nHeight,int nOnceOnly);
	/***********************************************************************
	 Prototype : void KIP_ThinningWithDeutschAlgorithmInRect(unsigned char *pSrc,
					  unsigned char *pTgt,RECT r,int nOnceOnly)
	 Function  : Deutsch Algorithm을 이용하여 thinning을 한다. 
	 Parameters: pSrc -> input image buffer [in]
				 pTgt -> output image buffer[out]
				 r    -> ROI
				 nOnceOnly -> 1 : 1 iteration, 0: iteration all
	 Return    : none
	 Author    : Kim Yang Ju
	 Date      : 2003.1.13
	 Remark    : none
	 ***********************************************************************/
	KTYPE32 void KIP_ThinningWithDeutschAlgorithmInRect(unsigned char *pSrc,
					  unsigned char *pTgt,int nWidth,int nHeight,RECT r,int nOnceOnly);
	/*************************************************************************
	 Prototype : void ChainCode(unsigned char *pSrc,int nImgWidth,int nImgHeight,RECT r,
					  int *nCodeTable,POINT *pts,int *nNum,int nMaxChain)
	 Function  : 8 방향 chain code를 구한다. (edge following algorithm)
	 Parameters: pSrc       -> image buffer [in]
				 nImgWidth  -> image width  [in]
				 nImgHeight -> image height [in]
				 r          -> Rectangle ROI[in]
				 nCodeTable -> 0 -7 까지의 chain code를 저장한다. [out]
				 pts        -> boundary point buffer [out]
				 nx         -> start x point [in]
				 ny         -> start y point [in]
				 nNum       -> number of code, point [out]
				 nMaxChain  -> estimated maximum chain code number [in]
							   (imagewidth + image height)*2 or higher
	 Return    : none
	 Author    : Kim Yang Ju
	 Date      : 2003.1.14
	 Remark    : none
	 *************************************************************************/
	KTYPE32 void KIP_ChainCode(unsigned char *pSrc,int nImgWidth,int nImgHeight,RECT r,int *nCodeTable,
						  POINT *pts,int *nNum,int nMaxChain,int nV=255);
	/********************************************************************
	 Prototype : double KIP_GetPerimeter(int *nCodeTable,int nNum)
	 Function  : Chain code table 을 이용하여 둘레의 길이를 구한다. 
	 Parameter : nCodeTable -> Chain code table [in]
				 nNum       -> number of data   [in]
	 Return    : double, 둘레의 길이 
	 Author    : Kim Yang Ju
	 Date      : 2003.1.14
	 Remark    : none
	 *********************************************************************/
	KTYPE32 double KIP_GetPerimeter(int *nCodeTable, int nNum);
	/********************************************************************
	 Prototype : double KIP_GetArea(int *nCodeTable, int nNum)
	 Function  : Compute the area given the chain code.		
	 Parameter : nCodeTable -> Chain code table [in]
				 nNum       -> number of data   [in]
	 Return    : double, 면적 
	 Author    : Kim Yang Ju
	 Date      : 2003.1.14
	 Remark    : none
	 *********************************************************************/
	KTYPE32 double KIP_GetArea(int *nCodeTable, int nNum);
	/********************************************************************
	 Prototype : double KIP_GetCompactness(int *nCodeTable,int nNum)
	 Function  : Chain code table 을 이용하여 compactness, roundness를 구한다. 
	 Parameter : nCodeTable -> Chain code table [in]
				 nNum       -> number of data   [in]
	 Return    : double, 둘레의 길이 
	 Author    : Kim Yang Ju
	 Date      : 2003.1.14
	 Remark    : none
	 *********************************************************************/
	KTYPE32 double KIP_GetCompactness(int *nCodeTable,int nNum);
	/********************************************************************
	 Prototype : int KIP_GetImageBlock(unsigned char *pSrc,int swidth,int sHeight,
									   unsigned char *pTgt,int nX, int nY,
									   int tWidth,int tHeight)
	 Function  : 원본 이미지에서  block image를 얻는다.
	 Parameter : pSrc    -> Source image buffer
				 sWidth  -> source image width
				 sHeight -> source image height
				 pTgt    -> Target image buffer
				 nX      -> start x coordinate
				 nY      -> end x coordinate
				 tWidth  -> target image width
				 tHeight -> target image height
	 return    : 0 -> fail, 1 -> success
	 Author    : Kim Yang Ju
	 Date      : 2003.3.31
	 Remark    : none
	*********************************************************************/
    KTYPE32	int KIP_GetImageBlock(unsigned char *pSrc,int sWidth,int sHeight,
		                          unsigned char *pTgt,int nX,int nY,int tWidth,int tHeight);
	/*******************************************************************************
	 Prototype : void KIP_ZoomImageBilinear(unsigned char *pSrc,int sWidth,int sHeight,
											unsigned char *pTgt,int xZoom,int yZoom)
	 Function  : Bilinear 방법으로 이미지를 확대 한다. 
	 Parameters: pSrc    -> source image buffer
				 sWidth  -> source image width
				 sHeight -> source image height
				 pTgt    -> target image buffer (sWidth*xZoom, sHeight*yZoom)
				 xZoom   -> horizontal zooming factor (1,2,3,4,...) 
				 yZoom   -> vertical zooming factor   (1,2,3,4,...)
	 return    : none
	 author    : Kim Yang Ju
	 Date      : 2003.4.1
	 remark    : none
	 *******************************************************************************/
	KTYPE32 void KIP_ZoomImageBilinear(unsigned char *pSrc,int sWidth,int sHeight,
		                               unsigned char *pTgt,int xZoom,int yZoom);
	/*******************************************************************************
	 Prototype : void KIP_ZoomOutImage(unsigned char *pSrc,int sWidth,int sHeight,
											unsigned char *pTgt,int xZoomOut,int yZoomOut)
	 Function  : 입력이미지를 x, y의 zoom out 배율에 따라서 축소한다. 
	 Parameters: pSrc    -> source image buffer
				 sWidth  -> source image width
				 sHeight -> source image height
				 pTgt    -> target image buffer (sWidth*xZoom, sHeight*yZoom)
				 xZoomOut-> horizontal zoom out factor (1,2,4,...) 
				 yZoomOut-> vertical zoom out factor   (1,2,4,...)
	 return    : none
	 author    : Kim Yang Ju
	 Date      : 2003.7.3
	 remark    : none
	 *******************************************************************************/
	 KTYPE32 void KIP_ZoomOutImage(unsigned char *pSrc,int sWidth,int sHeight,
								   unsigned char *pTgt,int xZoomOut,int yZoomOut);
	/**************************************************************************************
	 Prototype : void KIP_Sort(int *pArr, int d, int h,BOOL bAscending)
	 Function  : data를 크기에  따라서 정렬을 한다. 
	 Parameters: pArr       -> data buffers
				 d          -> Start index
				 h          -> End index
				 bAscending -> 0 : descending, 1: ascending
	 return    : none
	 Date      : 2003.4.1
	 Author    : Martin Ziacek, Martin.Ziacek@swh.sk, http://www.swh.sk
				 Modified by Kim Yang Ju.
	 Remark    : none
	 **************************************************************************************/
	KTYPE32 void  KIP_Sort(int *pArr, int d, int h, BOOL bAscending);
	/***********************************************************************************
	 Prototype : void KIP_MedianFilter(unsigned char *pSrc,int nWidth,int nHeight,unsigned char *pTgt)
	 Function  : 메디안 filtering을 한다. 정열은  Recursive quick sort를 사용한다.
	 Parameters: pSrc    -> source image buffer
				 nWidth  -> image width
				 nHeight -> image height
				 pTgt    -> target image buffer
	 return    : none
	 date      : 2003.4.1
	 author    : Kim Yang Ju
	 remark    : none
	 ***********************************************************************************/
	KTYPE32 void KIP_MedianFilter(unsigned char *pSrc,int nWidth,int nHeight,unsigned char *pTgt);
	/*****************************************************************************
	 Prototype : int KIP_UnionImage(unsigned char *pSrc1, unsigned char *pSrc2,
									int nWidth,int nHeight,unsigned char *pDst)
	 Function  : 이미지 두개를 합친다. 단, 사이즈는 동일해야 한다. 
	 Parameters: pSrc1   -> first source image buffer
				 pSrc2   -> second source image buffer
				 nWidth  -> image width
				 nHeight -> image height
				 pDst    -> destination buffer
	 return    : 0 -> fail, 1->success
	 author    : Kim Yang Ju
	 date      : 2003.4.2
	 remark    : none
	 *****************************************************************************/
	KTYPE32 int KIP_UnionImage(unsigned char *pSrc1, unsigned char *pSrc2,
		                       int nWidth,int nHeight,unsigned char *pDst);
	/*****************************************************************************
	Prototype : int KIP_UnionImageSequence(unsigned char *pSrc,int nWidth,
										int nHeight,unsigned char *pDst)
	Function  : 이미지들을 합친다. 
        
	Parameters: pSrc    -> source image  buffer 
			 nWidth  -> image width
			 nHeight -> image height
			 pDst    -> destination buffer
	return    : 0 -> fail, 1->success
	author    : Kim Yang Ju
	date      : 2003.4.2
	remark    : none
	*****************************************************************************/
	KTYPE32 int KIP_UnionImageSequence(unsigned char *pSrc,int nWidth,
		                               int nHeight,unsigned char *pDst);
	/******************************************************************
	prototype : void KIP_FillImageBoundary(unsigned char *pImg,int nWidth,
										   int nHeight,int objColor,int setColor)
	function  : 이미지의 외곽부분의 objColor인 픽셀을 setColor로 채운다.
				(NDC aligner를 위하여 제작하였다.)
				-검사 대상 이외의 다른 물체가 가리울때 나타나는 영역을 제거.
				ex) KIP_FillImageBoundary(pTgtTwo,nXs,nYs,255,0);
	parameter : pImg     -> image source buffer[in/out]
				nWidth   -> image width
				nHeight  -> image height
				objColor -> 변경전 이미지상의 화소값 
				setColor -> 변경후 이미지상의 화소값
	return    : none
	date      : 2003.4
	author    : Kim Yang Ju
	remark    : none
	******************************************************************/
	KTYPE32 void KIP_FillImageBoundary(unsigned char *pImg,int nWidth,
		                               int nHeight,int objColor,int setColor);
	/*******************************************************************
	 prototype : void KIP_Rotate(unsigned char *pSrc,unsigned char *pOut,
								 int nWidth,int nHeight,double nAngle)
	 function  : 이미지를 각도 만큼 회전을 한다. 데이터 손실은 Bilinear
				 방식으로 보간을 한다. 
	 parameter : pSrc    -> source image buffer [in]
				 pOut    -> target image buffer [out]
				 nWidth  -> source image width [in]
				 nHeight -> source image height [in]
				 nAngle  -> 각도 (degree : radian이 아님 )
	 return    : none
	 date      : 2003.4
	 author    : Kim Yang Ju
	 remark    : none
	********************************************************************/
	KTYPE32 void KIP_Rotate(unsigned char *pSrc,unsigned char *pOut, int nWidth,int nHeight,double nAngle);
	/******************************************************************
	 prototype : void KIP_Smoothing(unsigned char *pSrc,unsigned char *pOut,
								   int nWidth,int nHeight,int size)
	 function  : 이미지를 부드럽게(smoothing)을 한다.
	 parameters: pSrc-> source image buffer
				 pOut-> output image buffer
				 nWidth -> image width
				 nheight -> image height
				 size -> kernel size
	 return    : none
	 date      : 2003.9
	 author    : Kim Yang Ju
	 remark    : none
	*******************************************************************/
	KTYPE32 void KIP_Smoothing(unsigned char *pSrc,unsigned char *pOut,int nWidth,int nHeight,int size);
	KTYPE32 void KIP_SmoothingFilter(unsigned char *pSrc,int nWidth,int nHeight,unsigned char *pTgt);


	KTYPE32 void KIP_DisplayStraightLineOnImage(unsigned char *pSrc,int nWidth,
		                                        POINT sp, POINT ep,int nColor);
	KTYPE32 void KIP_DisplayDotLineOnImage(unsigned char *pSrc,int nWidth,
		                                   POINT sp, POINT ep,int nColor);
	KTYPE32 int  KIP_GetImageSum(unsigned char *pSrc,int nWidth,RECT r,int nColor);
////////////////////////////////////////////////////////////////////////////////////
	KTYPE32 void KIP_Smoothing5x5(unsigned char *pSrc,unsigned char *pOut,int nWidth,int nHeight);
#ifdef _cplusplus
}
#endif
#endif