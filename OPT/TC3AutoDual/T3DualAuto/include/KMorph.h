/************************************************************************
 Hanra precision engineering CO.,LTD.

 Author   : KIM YANGJU
 Date     : 2002.3.21
 Version  : 1.0 for KVision libray
 Function : Binary & gray morphology
			iteratarative open & close
 <History : yy/mm/dd>---------------------------------------------------<>
 2002/4/1 : KmorphSetOpt(),KmorphGetCrossSection(),KmorphGetTopHatImage(),
			KmorphGetGradientImage() , appended by Yangju Kim
 2003/2/17: KmorphSet() appended by Yangju Kim
 *************************************************************************/
#ifndef __KMORPH_H_
#define __KMORPH_H_

#ifndef KIL_ID_DEF_
#define KIL_ID_DEF_
typedef long KIL_ID;
#endif

#ifndef KTYPE32
	#ifdef COMPILING_KMORPHDLL
		#define KTYPE32 __declspec(dllexport)
	#else
		#define KTYPE32 __declspec(dllimport)
	#endif
#endif

//////////////////////////////////////////////
// Setting parameter....
#define MORPH_PARAM_IMAGE_DIM   0 // width ,  height, NULL
#define MORPH_PARAM_KERNEL_DIM  1 // kwidth, kheight, NULL
#define MORPH_PARAM_KERNEL      2 // kwidth, kheight, *knlptr
#define MORPH_PARAM_KERNELTYPE  3 // kernel type.
#define MORPH_PARAM_COLOR       4 // object color, background color
#define MORPH_PARAM_MODE        5 // mode, gray or binary 
#define MORPH_PARAM_COPYBUFER   6 // flag... 0 or 1
#define MORPH_PARAM_ROI         7 // RECT
#define MORPH_PARAM_MAKEKERNEL  8 // width, height

///////////////////////////////////////////////
//KmorphSetOpt() nType definition :set operator
#define MORPH_PARAM_UNION            0
#define MORPH_PARAM_INTERSECTION     1
#define MORPH_PARAM_COMPLEMENTATION  2
#define MORPH_PARAM_DIFFERENCE       3
///////////////////////////////////////////////
//KmorphGetGradientImage() nType definition..
#define MORPH_PARAM_GRAD_DMF   0  // dilated image - original image
#define MORPH_PARAM_GRAD_FME   1  // original image - eroded image
#define MORPH_PARAM_GRAD_DME   2  // dilate image - eroded image
///////////////////////////////////////////////

typedef struct {
	int nWidth;    // image  width
	int nHeight;   // image  height
	int nKWidth;   // kernel width
	int nKHeight;  // kernel height
	int nKType;    // kernel type
	int nObjColor; // object color
	int nBkColor;  // background color
	int nMode;     // 0 : binary, 1: grey
	int nCopyBuffer; // 0: disable, 1: enable
	RECT r;        // region of interest
}MORPH_PARAM;

#ifdef _cplusplus
extern "C"
{
#endif

KTYPE32 int KmorphSetParam(int nParamType,void *pUsrPtr1,void *pUsrPtr2,void *pUsrPtr3);
KTYPE32 int KmorphGetParam(int nParamType,void *pUsrPtr1,void *pUsrPtr2,void *pUsrPtr3);
KTYPE32 int KmorphDilate(unsigned char *pSrc,unsigned char *pDst);
KTYPE32 int KmorphErode(unsigned char *pSrc,unsigned char *pDst);
KTYPE32 int KmorphOpenOpt(unsigned char *pSrc,unsigned char *pDst,int nItr);
KTYPE32 int KmorphCloseOpt(unsigned char *pSrc, unsigned char *pDst, int nItr);

KTYPE32 int KmorphSetOpt(int nType,unsigned char *pImg1, unsigned char *pImg2, unsigned char *pImg3);
KTYPE32 int KmorphGetCrossSection(unsigned char *pSrc, unsigned char *pDst,int nThresh);
KTYPE32 int KmorphGetTopHatImage(unsigned char *pSrc, unsigned char *pDst);
KTYPE32 int KmorphGetGradientImage(int nType,unsigned char *pSrc, unsigned char *pDst);
/*******************************************************
 Prototype : int KmorphSet(MORPH_PARAM cParam)
 Function  : set parameters related to mophology image operation
 Parameters: cParam -> morphology parameters 
 Return    : don't care
 Author    : Yang Ju Kim
 Date      : 2003.1.17
 Remark    : none
********************************************************/
KTYPE32 int KmorphSet(MORPH_PARAM cParam);
#ifdef _cplusplus
}
#endif

#endif
