#ifndef __KBLOB_H_
#define __KBLOB_H_

#ifndef KIL_ID_DEF_
#define KIL_ID_DEF_
typedef long KIL_ID;
#endif

#ifndef KTYPE32
	#ifdef COMPILING_KBLOBDLL
		#define KTYPE32 __declspec(dllexport)
	#else
		#define KTYPE32 __declspec(dllimport)
	#endif
#endif
// setting parameter type definition
#define PARAM_MINBLOB          0 // minimum blob set
#define PARAM_DIMENSION        1 // image dimension set
#define PARAM_ROI_LEFTTOP      2 // roi left-top point set
#define PARAM_ROI_RIGHTBOTTOM  3 // roi right-bottom point set
#define PARAM_FGCOLOR          4 // fore ground color set
#define PARAM_BKCOLOR          5 // back ground color set
#define PARAM_IMAGE            6 // image data settting
                                 // not available get image data
// getting blob feature.....
#define FEATURE_RECT           0
#define FEATURE_CENTER_POINT   1
#define FEATURE_START_POINT    2
#define FEATURE_OBJ_SIZE       3


#ifdef __cplusplus
extern "C" 
{
#endif
	KTYPE32 int KblobAlloc(KIL_ID *pBlobID);
	KTYPE32 int KblobFree(KIL_ID nId);
	KTYPE32 int KblobSetParam(KIL_ID nId,int nParamType,void *pUsrPtr1,void *pUsrPtr2);
	KTYPE32 int KblobGetParam(KIL_ID nId,int nParamType,void *pUsrPtr1,void *pUsrPtr2);
	KTYPE32 int KblobRun(KIL_ID nId);
	KTYPE32 int KblobGetFeature(KIL_ID nId,KIL_ID nBlobId,int nFeatureType,void *pUsrPtr1,void *pUsrPtr2);

	/******************************************
	 Hidden library routines for 네스디스플레이 
	*******************************************/
	KTYPE32 void KblobDeleteExEllipse(KIL_ID nId,KIL_ID nBlobId);
#ifdef __cplusplus
}
#endif
#endif