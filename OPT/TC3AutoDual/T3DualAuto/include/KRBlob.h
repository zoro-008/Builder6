/***************************************************
 만든이   : 김양주 
 DATE     : 2002년 11월26일 
 Version  : 2.0
 Function : image blob analysis
 [usage]
	KRB_Alloc() 
	KRB_Set()
	KRB_Run()
	KRB_Get()  
	KRB_Free()
 - Updated  Remark -
 [2002.12.11]
 - Bug fixed boundary area handling... [stack overflow error]
 [2002.12.13]
 - KRB_RunOne -> KRB_RunAround 로 변경 
 - KRB_RunOnce() 추가 : 하나의 blob object만 찾음.
 [2002.12.14]
 - Processing time optimize.
 [2003.3.29 ]
 -sorting routine appended!
 ****************************************************/
#ifndef __KRBLOB_H_
#define __KRBLOB_H_

#ifndef KIL_IDI_DEF_
#define KIL_IDI_DEF_
typedef int KIL_IDI;
#endif


#ifndef KTYPE32
	#ifdef COMPILING_KRBLOBDLL
		#define KTYPE32 __declspec(dllexport)
	#else
		#define KTYPE32 __declspec(dllimport)
	#endif
#endif
/*****************************
 Blob aprameter setting structure
******************************/
typedef struct{
	int nImgWidth;   // Image width
	int nImgHeight;  // image Height
	int nOffsetX;    // search offset x
	int nOffsetY;    // search offset y
	int nSizeFilter; // Object size to analysis
	RECT r;          // Region of interest
	int nThresh ;    // thresholding value
}RBLOB_SET_STRUCT;
/*****************************
 Blob analysis data structure
******************************/
typedef struct
{
	int    nIdx;
	int    nArea;     // Blob area
	int    nPointNum; // Number of point in blob area
	double nCx;       // Blob area center x
	double nCy;       // Blob area center y
	RECT   r;         // Blob area retangle
}RBLOB_RESULT_STRUCT;

#ifdef __cplusplus
extern "C" 
{
#endif
	/*****************************************************
	 Prototype : int KRB_Alloc(KIL_IDI *pBlobID)
	 Function  : id를 할당 받는다. 내부적으로는 
				 blob 메모리를 할당하게 되고 이 blob 
				 메모리에는 blob 데이터들이 존재한다.
	 Parameter : pBlobID -> Blob id to allocate (input)
	 Return    : 1 -> memory allocation success
				 0 -> memory allocation fail
	 Date      : 2002.
	 Author    : 김양주
	 Remark    : none
	******************************************************/
	KTYPE32 int KRB_Alloc(KIL_IDI *pBlobID);
	/************************************
	 Prototype : int KRB_Free(KIL_IDI nId)
	 Function  : 할당된 blob 메모리를 지운다. 
	 Parameter : nId -> Blob id
	 Return    : 1 -> memory free success
				 0 -> memory free fail
	 Date      : 2002.11.26
	 Author    : 김양주 
	 Remark    : none
	*************************************/
	KTYPE32 int KRB_Free(KIL_IDI pBlobID);
	/********************************************
	 Prototype : void KRB_Set(KIL_IDI nId,RBLOB_SET_STRUCT *pSetObj)
	 Function  : Set blob parameters
	 Parameter : nId     -> Blob identifier
				 pSetObj -> Blob setting parameter object
	 Return    : 1  -> setting parameter success
				 0  -> setting parameter fail
	 Date      : 2002.11.26
	 Author    : 김양주 
	 Remark    : 이미지 할당 및 셋 routine이 없음 
				 Run을 할때 입력 
	*********************************************/
	KTYPE32 int KRB_Set(KIL_IDI nId,RBLOB_SET_STRUCT *pSetObj);
	/*******************************************************
	 Prototype : int KRB_Run(KIL_IDI nId,unsigned char *pImg,int nMode)
	 Function  : 실제 Blob analysis routine을 수행한다. 
	 Parameter : nId  -> blob object identifier (input)
				 pImg -> image buffer (input)
				 nMode -> 0 (binarized image), 1(grey for white), 2(grey for black)
	 Return    : Number of blob
	 Date      : 2002.11.26
	 Author    : 김양주 
	 Remark    : none
	********************************************************/
	KTYPE32 int KRB_Run(KIL_IDI nId,unsigned char *pImg,int nMode);
	/*******************************************************
	 Prototype : int KRB_RunOnce(KIL_IDI nId,POINT pts,unsigned char *pImg,int nMode)
	 Function  : Blob analysis routine을 한번만 수행한다. 
	 Parameter : nId  -> blob object identifier (input)
				 pts  -> seed point
				 pImg -> image buffer (input)
				 nMode -> 0 (binarized image), 1(grey for white), 2(grey for black)
	 Return    : Number of blob
	 Date      : 2002.12.2
	 Author    : 김양주 
	 Remark    : none
	********************************************************/
	KTYPE32 int KRB_RunAround(KIL_IDI nId,POINT pts,unsigned char *pImg,int nMode);
	/*******************************************************
	 Prototype : int KRB_RunOnce(KIL_IDI nId,unsigned char *pImg,int nMode)
	 Function  : Blob analysis routine을 한번만 수행한다. 
				 일정 크기의 blob을 찾으면 종료를 한다. 
	 Parameter : nId  -> blob object identifier (input)
				 pts  -> seed point
				 pImg -> image buffer (input)
				 nMode -> 0 (binarized image), 1(grey for white), 2(grey for black)
	 Return    : Number of blob
	 Date      : 2002.12.2
	 Author    : 김양주 
	 Remark    : none
	********************************************************/
	KTYPE32 int KRB_RunOnce(KIL_IDI nId,unsigned char *pImg,int nMode);
	/***************************************************
	 Prototype : int KRB_Get(KIL_IDI nBlobId, KIL_ID nIdx,
							 RBLOB_RESULT_STRUCT *pResult)
	 Function  : 개별 Blob data를 얻는다.
	 Parameter : nBlobId -> 할당된 Blob identifier
				 nIdx    -> 개별 blob identifier
				 pResult -> 개별 blob data structure
	 Return    : 1 -> success
				 0 -> fail
	 Date      : 2002.11.27
	 Author    : 김양주 
	 Reamrk    : none
	****************************************************/
	KTYPE32 int KRB_Get(KIL_IDI nBlobId,KIL_IDI nIdx, RBLOB_RESULT_STRUCT *pResult);
	/*****************************************************
	 Prototpe : int KRB_GetPoint(KIL_IDI nBlobId,KIL_IDI nIdx,
								 POINT *pt)
	 Function : Blob 영역의 모든 점 데이터를 얻는다.
	 Parameter: nBlobId -> Blob identifier (input)
				nIdx    -> 개별 blob identifier (input)
				pt      -> Point buffer(output, pre-allocated)
	 Return   : 1 -> success
				0 -> fail
	 Date     : 2002.11.27
	 Author   : 김양주 
	 Remark   : none
	******************************************************/
	KTYPE32 int KRB_GetPoint(KIL_IDI nBlobId,KIL_IDI nIdx,POINT *pt);
	/**************************************************************************************
	 Prototype : void KRB_SortByCx(RBLOB_RESULT_STRUCT *pArr, int d, int h,BOOL bAscending)
	 Function  : Blob 결과를 중심 좌표 x에대하여 정렬을 한다. 
	 Parameters: pArr       -> Blob result buffers
				 d          -> Start index
				 h          -> End index
				 bAscending -> 0 : descending, 1: ascending
	 return    : none
	 Date      : 2003.3.29
	 Author    : Martin Ziacek, Martin.Ziacek@swh.sk, http://www.swh.sk
				 Modified by Kim Yang Ju.
	 Remark    : none
	 **************************************************************************************/
	 KTYPE32 void  KRB_SortByCx(RBLOB_RESULT_STRUCT *pArr, int d, int h, BOOL bAscending);
	/**************************************************************************************
	 Prototype : void KRB_SortByCy(RBLOB_RESULT_STRUCT *pArr, int d, int h,BOOL bAscending)
	 Function  : Blob 결과를 중심 좌표 y에대하여 정렬을 한다. 
	 Parameters: pArr       -> Blob result buffers
				 d          -> Start index
				 h          -> End index
				 bAscending -> 0 : descending, 1: ascending
	 return    : none
	 Date      : 2003.3.29
	 Author    : Martin Ziacek, Martin.Ziacek@swh.sk, http://www.swh.sk
				 Modified by Kim Yang Ju.
	 Remark    : none
	 **************************************************************************************/
	KTYPE32 void  KRB_SortByCy(RBLOB_RESULT_STRUCT *pArr, int d, int h, BOOL bAscending);
	/**************************************************************************************
	 Prototype : void KRB_SortByArea(RBLOB_RESULT_STRUCT *pArr, int d, int h,BOOL bAscending)
	 Function  : Blob 결과를 영역의 크기에 따라서 정렬을 한다. 
	 Parameters: pArr       -> Blob result buffers
				 d          -> Start index
				 h          -> End index
				 bAscending -> 0 : descending, 1: ascending
	 return    : none
	 Date      : 2003.3.29
	 Author    : Martin Ziacek, Martin.Ziacek@swh.sk, http://www.swh.sk
				 Modified by Kim Yang Ju.
	 Remark    : none
	 **************************************************************************************/
	KTYPE32 void  KRB_SortByArea(RBLOB_RESULT_STRUCT *pArr, int d, int h, BOOL bAscending);

	KTYPE32 int KRB_Reset(KIL_IDI nId);

#ifdef __cplusplus
}
#endif

#endif