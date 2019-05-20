/************************************************************************
 Hanra precision engineering CO.,LTD.

 Author   : KIM YANGJU
 Date     : 2002.6.4
 Version  : 1.0 for KVision libray
 Function : Edge detection 
 <History : yy/mm/dd>---------------------------------------------------<>
 2002/6/4 : Starting project....  
 *************************************************************************/
#ifndef __KEDGE_H
#define __KEDGE_H

#ifndef KIL_ID_DEF_
#define KIL_ID_DEF_
typedef long KIL_ID;
#endif

#ifndef KTYPE32
	#ifdef COMPILING_KEDGEDLL
		#define KTYPE32 __declspec(dllexport)
	#else
		#define KTYPE32 __declspec(dllimport)
	#endif
#endif

#define EDGE_LAPLACIAN    0 // zero crossing 
#define EDGE_PREWITT      1 // prewitt operator
#define EDGE_SOBEL        2 // sobel operator
#define EDGE_ROBINSON     3 // robinson operator
#define EDGE_KIRSH        4 // kirsh operator
#define EDGE_MARRHILDRETH 5 // marr-hildreth edge detector
#define EDGE_CANNY        6 // canny edge detector

#ifdef _cplusplus
extern "C"
{
#endif
KTYPE32 void EdgeFind(unsigned char *pSrc,unsigned char *pDst,int nWidth,int nHeight,int nMethod,int nThresh,int nThresh2);
KTYPE32 void SetKernelSigma(double nSigma);
#ifdef _cplusplus
}
#endif

#endif

