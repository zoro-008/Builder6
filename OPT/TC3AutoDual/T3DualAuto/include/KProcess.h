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
	 Function  : r1���� ���� �� ������ ����ȭ�Ѵ�. 
				 Donut region.
	 Parameter : *pSrc -> Source image buffer (input)
				 *pTgt -> Target image buffer (output)
				 r     -> Region of interest (input)
				 nImgWidth -> image width (input)
				 nCx   -> Center point X (input)
				 nCy   -> Center point Y (input)
				 r1    -> �ܿ��� ������ (input)
				 thresh-> Threshold value for bainarization(input)
				 flag  -> Selection flag 
					  (input, 0: Sobel edge operator, 1: binarize black, 2: binarize white)
	 Return     : none
	 Date       : 2002.12.9
	 Author     : �� ���� 
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
	 Function  : r1���� �۰� r2 ���� ū ������ ó���Ѵ�.
	             
				 Donut region.
	 Parameter : *pSrc -> Source image buffer (input)
				 *pTgt -> Target image buffer (output)
				 r     -> Region of interest (input)
				 nImgWidth -> image width (input)
				 nCx   -> Center point X (input)
				 nCy   -> Center point Y (input)
				 r1    -> �ܿ��� ������ (input)
				 r2    -> ������ �������� ũ�� (input)
				 thresh-> Threshold value for bainarization(input)
				 flag  -> Selection flag 
					  (input, 0: Sobel edge operator, 1: binarize black, 2: binarize white)
	 Return     : none
	 Date       : 2002.
	 Author     : �� ���� 
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
	 Author    : ����� 
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
	 Author     : ����� 
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
	 Author     : ����� 
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
	 Author     : ����� 
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
	 Author     : ����� 
	 Date       : 2002.12.23
	 Remark     
	*************************************************************/
	KTYPE32 void KIP_HorizProjection(unsigned char *pSrc,int *pProjV,RECT r,int nWidth);
	/****************************************************************************
	 Prototype : int KIP_GetHorizEdgePoints(unsigned char *pSrc,RECT r,
										double *ptx,double *pty)
	 Function  : ���� ������ Moment edge�� ���� ���� ũ�� ��ŭ ���Ѵ�. 
	             STEP EDGE�ϰ�� ��밡��. �׹ۿ��� ���� �߻�.
	 Parameters: pSrc  -> Source image buffer (input)
				 r     -> ROI                 (input)
				 nWidth-> image width         (input)
				 ptx   -> edge point x        (output)
				 pty   -> edge point y        (output)
	 Return    :
	 Author    : ����� 
	 Date      : 2002.12.24
	 Remark    : 
	*****************************************************************************/
	KTYPE32 int  KIP_GetHorizEdgePoints(unsigned char *pSrc,RECT r,int nWidth,double *ptx,double *pty);
	/****************************************************************************
	 Prototype : int KIP_GetHorizMAxGradientEdgePoints(unsigned char *pSrc,RECT r,POINT *pts)
	 Function  : ���� ������ edge�� ���� ���� ũ�� ��ŭ ���Ѵ�. 
	 Parameters: pSrc  -> Source image buffer (input)
				 r     -> ROI                 (input)
				 nWidth-> image width         (input)
				 pts   -> edge points         (output)
						  Must be memory allocated! 
	 Return    : 
	 Author    : ����� 
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
	 Author    : �� ���� 
	 Date      : 2002.12.27
	 Reamrk    : none
	*********************************************************************/
	KTYPE32 void KIP_BinarizeAll(unsigned char *pSrc,unsigned char *pOut,
		                         int nWidth,int nHeight,int flag, int thresh);

	/****************************************************************************
	 Prototype : void KIP_GetHorizGradientEdge(unsigned char *pSrc,unsigned char *pOut,
												 int nWidth,int nHeight,int nThresh)
	 Function  : ���� ������ edge�� ���Ѵ�. 
	 Parameters: pSrc    -> Source image buffer(input)
				 pOut    -> Target image buffer(output)
				 nWidth  -> image width        (input)
				 nHeight -> image height       (input)
				 nThresh -> Thresholding value (input)
	 Return    : none
	 Author    : ����� 
	 Date      : 2002.12.27
	 Remark    : 
	*****************************************************************************/
	KTYPE32 void KIP_GetHorizGradientEdge(unsigned char *pSrc,unsigned char *pOut,
		                                  int nWidth,int nHeight,int nThresh);

	/****************************************************************************
	 Prototype : int KIP_GetGradientEdgePoints(unsigned char *pSrc,RECT r,
									 int nWidth,POINT *pts,int nThresh,int nDir)
	 Function  : ���� ������ edge�� ���� ���� ũ�� ��ŭ ���Ѵ�. 
				 Gradient�� threshold ������ Ŭ��� �������� ...
				 �Ѷ����� edge ������ ��´�.
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
	 Author    : �� ���� 
	 Date      : 2003.1.2
	 Remark    : 
	*****************************************************************************/
	KTYPE32 int KIP_GetGradientEdgePoints(unsigned char *pSrc,RECT r,int nWidth,
											 POINT *pts,int nThresh,int nDir);
	/****************************************************************************
	 Prototype : int KIP_GetSobelEdgePoints(unsigned char *pSrc,RECT r,
									 int nWidth,POINT *pts,int nThresh,int nDir)
	 Function  : ���� ������ edge�� ���� ���� ũ�� ��ŭ ���Ѵ�. 
				 Gradient�� threshold ������ Ŭ��� �������� ...
				 �Ѷ����� edge ������ ��´�.
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
	 Author    : �� ���� 
	 Date      : 2003.1.2
	 Remark    : 
	*****************************************************************************/
	KTYPE32 int KIP_GetSobelEdgePoints(unsigned char *pSrc,RECT r,int nWidth,
											 POINT *pts,int nThresh,int nDir);
	/****************************************************************************
	 Prototype : int KIP_GetBinaryEdgePoints(unsigned char *pSrc,RECT r,
									 int nWidth,POINT *pts,int nThresh,int nDir)
	 Function  : ����ȭ�� �̹������� �Ǵ� gray �̹��������� ���� ���� .  
				 �ϳ��� ���� ���������� ��´�.
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
	 Author    : �� ���� 
	 Date      : 2003.1.2
	 Remark    : 
	*****************************************************************************/
	KTYPE32 int KIP_GetBinaryEdgePoints(unsigned char *pSrc,RECT r,int nWidth,
										POINT *pts,int nThresh,int nDir);
	/****************************************************************************
	 Prototype : int KIP_GetCenterLineGradientEdgePoints(unsigned char *pSrc,POINT spt,
					 POINT ept,int nWidth,int offset,int nThresh,POINT *pts,int nDir)
	 Function  : Center line�� �߽����� �� �Ʒ� offset ��ŭ�� ���� ������ 
				 ���� ������ ã�´�. 
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
	 Author    : �� ���� 
	 Date      : 2003.1.3
	 Remark    : none
	*****************************************************************************/
	KTYPE32 int KIP_GetCenterLineGradientEdgePoints(unsigned char *pSrc,POINT spt,POINT ept,
							int nWidth,int offset,int nThresh,POINT *pts,int nDir);
	/****************************************************************************
	 Prototype : int KIP_GetCenterLineGradientEdgePointsWithWeight(unsigned char *pSrc,POINT spt,
					 POINT ept,int nWidth,int offset,int nThresh,POINT *pts,int nDir)
	 Function  : - Center line�� �߽����� �� �Ʒ� offset ��ŭ�� ���� ������ 
				   ���� ������ ã�´�. 
				 - Gradient differencing�� ����Ѵ�.
				 - 3pixel�� �̿��Ͽ� weight�� ���ϰ� �̸� �̿��Ͽ� ��Ȯ�� �������� ���Ѵ�. 

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
	 Author    : �� ���� 
	 Date      : 2003.2.26
	 Remark    : none
	*****************************************************************************/
	KTYPE32 int KIP_GetCenterLineGradientEdgePointsWithWeight(unsigned char *pSrc,POINT spt,POINT ept,
							int nWidth,int offset,int nThresh,POINT *pts,int nDir);
	/****************************************************************************
	 Prototype : int KIP_GetCenterLineGradientEdgePointsWithWeightD(unsigned char *pSrc,POINT spt,
					 POINT ept,int nWidth,int offset,int nThresh,usrPOINT *pts,int nDir)
	 Function  : - Center line�� �߽����� �� �Ʒ� offset ��ŭ�� ���� ������ 
				   ���� ������ ã�´�. 
				 - Gradient differencing�� ����Ѵ�.
				 - 3pixel�� �̿��Ͽ� weight�� ���ϰ� �̸� �̿��Ͽ� ��Ȯ�� �������� ���Ѵ�. 

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
	 Author    : �� ���� 
	 Date      : 2003.2.27
	 Remark    : none
	*****************************************************************************/
	KTYPE32 int KIP_GetCenterLineGradientEdgePointsWithWeightD(unsigned char *pSrc,POINT spt,POINT ept,
							int nWidth,int offset,int nThresh,usrPOINT *pts,int nDir);

	/****************************************************************************
	 Prototype : int KIP_GetCenterLineBinaryEdgePoints(unsigned char *pSrc,POINT spt,
					 POINT ept,int nWidth,int offset,int nThresh,POINT *pts,int nDir)
	 Function  : Center line�� �߽����� �� �Ʒ� offset ��ŭ�� ���� ������ 
				 ���� ������ ã�´�. 
				 - ����ȭ ����� �̿��Ѵ�.
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
	 Author    : �� ���� 
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
	 Function  : �־��� ������ �̿��Ͽ� �̹����� �����.
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
	 Function  : Deutsch Algorithm�� �̿��Ͽ� thinning�� �Ѵ�. 
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
	 Function  : Deutsch Algorithm�� �̿��Ͽ� thinning�� �Ѵ�. 
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
	 Function  : 8 ���� chain code�� ���Ѵ�. (edge following algorithm)
	 Parameters: pSrc       -> image buffer [in]
				 nImgWidth  -> image width  [in]
				 nImgHeight -> image height [in]
				 r          -> Rectangle ROI[in]
				 nCodeTable -> 0 -7 ������ chain code�� �����Ѵ�. [out]
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
	 Function  : Chain code table �� �̿��Ͽ� �ѷ��� ���̸� ���Ѵ�. 
	 Parameter : nCodeTable -> Chain code table [in]
				 nNum       -> number of data   [in]
	 Return    : double, �ѷ��� ���� 
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
	 Return    : double, ���� 
	 Author    : Kim Yang Ju
	 Date      : 2003.1.14
	 Remark    : none
	 *********************************************************************/
	KTYPE32 double KIP_GetArea(int *nCodeTable, int nNum);
	/********************************************************************
	 Prototype : double KIP_GetCompactness(int *nCodeTable,int nNum)
	 Function  : Chain code table �� �̿��Ͽ� compactness, roundness�� ���Ѵ�. 
	 Parameter : nCodeTable -> Chain code table [in]
				 nNum       -> number of data   [in]
	 Return    : double, �ѷ��� ���� 
	 Author    : Kim Yang Ju
	 Date      : 2003.1.14
	 Remark    : none
	 *********************************************************************/
	KTYPE32 double KIP_GetCompactness(int *nCodeTable,int nNum);
	/********************************************************************
	 Prototype : int KIP_GetImageBlock(unsigned char *pSrc,int swidth,int sHeight,
									   unsigned char *pTgt,int nX, int nY,
									   int tWidth,int tHeight)
	 Function  : ���� �̹�������  block image�� ��´�.
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
	 Function  : Bilinear ������� �̹����� Ȯ�� �Ѵ�. 
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
	 Function  : �Է��̹����� x, y�� zoom out ������ ���� ����Ѵ�. 
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
	 Function  : data�� ũ�⿡  ���� ������ �Ѵ�. 
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
	 Function  : �޵�� filtering�� �Ѵ�. ������  Recursive quick sort�� ����Ѵ�.
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
	 Function  : �̹��� �ΰ��� ��ģ��. ��, ������� �����ؾ� �Ѵ�. 
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
	Function  : �̹������� ��ģ��. 
        
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
	function  : �̹����� �ܰ��κ��� objColor�� �ȼ��� setColor�� ä���.
				(NDC aligner�� ���Ͽ� �����Ͽ���.)
				-�˻� ��� �̿��� �ٸ� ��ü�� �����ﶧ ��Ÿ���� ������ ����.
				ex) KIP_FillImageBoundary(pTgtTwo,nXs,nYs,255,0);
	parameter : pImg     -> image source buffer[in/out]
				nWidth   -> image width
				nHeight  -> image height
				objColor -> ������ �̹������� ȭ�Ұ� 
				setColor -> ������ �̹������� ȭ�Ұ�
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
	 function  : �̹����� ���� ��ŭ ȸ���� �Ѵ�. ������ �ս��� Bilinear
				 ������� ������ �Ѵ�. 
	 parameter : pSrc    -> source image buffer [in]
				 pOut    -> target image buffer [out]
				 nWidth  -> source image width [in]
				 nHeight -> source image height [in]
				 nAngle  -> ���� (degree : radian�� �ƴ� )
	 return    : none
	 date      : 2003.4
	 author    : Kim Yang Ju
	 remark    : none
	********************************************************************/
	KTYPE32 void KIP_Rotate(unsigned char *pSrc,unsigned char *pOut, int nWidth,int nHeight,double nAngle);
	/******************************************************************
	 prototype : void KIP_Smoothing(unsigned char *pSrc,unsigned char *pOut,
								   int nWidth,int nHeight,int size)
	 function  : �̹����� �ε巴��(smoothing)�� �Ѵ�.
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