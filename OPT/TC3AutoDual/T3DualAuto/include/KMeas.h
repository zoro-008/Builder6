#ifndef __KMEAS_H__
#define __KMEAS_H__
typedef long KIL_ID;

#ifndef KTYPE32
#ifdef COMPILING_KMEASDLL
#define KTYPE32 __declspec(dllexport)
#else
#define KTYPE32 __declspec(dllimport)
#endif
#endif

// interpolation
#define IML_NEAREST_NEIGHBOR	        0 // nearest neighbor
#define IML_BILINEAR		        	1 // bilinear
#define IML_TRILINEAR		        	2 // trilinear
#define IML_BICUBIC			        	3 // bicubic

//polarity
#define IML_POSITIVE		        	0 //positive
#define IML_NEGATIVE		        	1 //negative
#define IML_BOTH			        	2 //any

//direction
#define	IML_HORIZ						0 // horizontal direction
#define	IML_VERT						1 // vetical direction
#define	IML_DIAG						2 // diagonal
#define	IML_LEFT_TO_RIGHT				3 // left -> right
#define IML_TOP_TO_BOTTOM				4 //top -> bottom
#define IML_RIGHT_TO_LEFT				5 //right -> left
#define	IML_BOTTOM_TO_TOP				6 //bottom ->top

//setting parameters
#define IML_SUBPIXEL					1 // subpixeling parameter
#define IML_INTERPOLATION				2 // interpoaltion parameter
#define IML_OFFSET						3 // x,y offset parameter
#define IML_BOX_START_POINT				4 // box parameter
#define IML_BOX_END_POINT				5 // box parameter
#define IML_POINT_POS					6 // point position parameter
#define IML_POLARITY					7 // polarity parameter
#define IML_MARKER_TYPE					8 // marker type paramter
#define IML_MARKER_DIR					9 // marker direction parameter
#define IML_EDGE_THRESHOLD_FUNCTION    10 // define edge threshold function

//marker type definition
#define IML_POINT						0 // point marker
#define IML_LINE						1 // line or edge marker
#define IML_STRIPE						2 // stripe marker

#define IML_DEFAULT						0 // default

//subpixel
#define IML_NONE						0 //no subpixel 
#define IML_CUBIC						1 //cubic interpolation.
#define	IML_LAGRANGE					2 //Lagrange interpolation
#define IML_SPLINE						3 //Spline interpolation
#define IML_MOMENT						4 //subpixel using moment (skewness)
										
//Inquire parameter
#define IML_STRIPE_WIDTH			    0 // stripe width....
#define IML_LINE_FUNCTION				1 // left or top line
#define IML_LINE_FUNCTION_START_POINT	2 // line function parameter
#define IML_LINE_FUNCTION_END_POINT		3 // line function parameter

#define IML_ORIGIN_CENTER_POS           4 // origin center position
#define IML_CONVERTED_CENTER_POS        5 // converted center position


#ifdef __cplusplus
extern "C"
{
#endif
	KTYPE32 int  KmeasAlloc(KIL_ID *nMarkerPtr,int nMarkerType,int nWidth,int nHeight);
	KTYPE32 int  KmeasFree(KIL_ID nId);
	KTYPE32 int  KmeasFindMarker(KIL_ID nMarkerId,unsigned char *pSrcImg,long nList);
	KTYPE32 int  KmeasInquire(KIL_ID nMarkerId,int nKind,void *pValue1,void *pValue2);
	KTYPE32 void KmeasSetParam(KIL_ID nId,int nKind, void *pValue1,void *pValue2);
	KTYPE32 void KmeasGetParam(KIL_ID nId,int nKind, void *pValue1,void *pValue2);
#ifdef __cplusplus
}
#endif

#endif
