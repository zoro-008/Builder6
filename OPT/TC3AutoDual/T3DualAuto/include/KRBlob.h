/***************************************************
 ������   : ����� 
 DATE     : 2002�� 11��26�� 
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
 - KRB_RunOne -> KRB_RunAround �� ���� 
 - KRB_RunOnce() �߰� : �ϳ��� blob object�� ã��.
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
	 Function  : id�� �Ҵ� �޴´�. ���������δ� 
				 blob �޸𸮸� �Ҵ��ϰ� �ǰ� �� blob 
				 �޸𸮿��� blob �����͵��� �����Ѵ�.
	 Parameter : pBlobID -> Blob id to allocate (input)
	 Return    : 1 -> memory allocation success
				 0 -> memory allocation fail
	 Date      : 2002.
	 Author    : �����
	 Remark    : none
	******************************************************/
	KTYPE32 int KRB_Alloc(KIL_IDI *pBlobID);
	/************************************
	 Prototype : int KRB_Free(KIL_IDI nId)
	 Function  : �Ҵ�� blob �޸𸮸� �����. 
	 Parameter : nId -> Blob id
	 Return    : 1 -> memory free success
				 0 -> memory free fail
	 Date      : 2002.11.26
	 Author    : ����� 
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
	 Author    : ����� 
	 Remark    : �̹��� �Ҵ� �� �� routine�� ���� 
				 Run�� �Ҷ� �Է� 
	*********************************************/
	KTYPE32 int KRB_Set(KIL_IDI nId,RBLOB_SET_STRUCT *pSetObj);
	/*******************************************************
	 Prototype : int KRB_Run(KIL_IDI nId,unsigned char *pImg,int nMode)
	 Function  : ���� Blob analysis routine�� �����Ѵ�. 
	 Parameter : nId  -> blob object identifier (input)
				 pImg -> image buffer (input)
				 nMode -> 0 (binarized image), 1(grey for white), 2(grey for black)
	 Return    : Number of blob
	 Date      : 2002.11.26
	 Author    : ����� 
	 Remark    : none
	********************************************************/
	KTYPE32 int KRB_Run(KIL_IDI nId,unsigned char *pImg,int nMode);
	/*******************************************************
	 Prototype : int KRB_RunOnce(KIL_IDI nId,POINT pts,unsigned char *pImg,int nMode)
	 Function  : Blob analysis routine�� �ѹ��� �����Ѵ�. 
	 Parameter : nId  -> blob object identifier (input)
				 pts  -> seed point
				 pImg -> image buffer (input)
				 nMode -> 0 (binarized image), 1(grey for white), 2(grey for black)
	 Return    : Number of blob
	 Date      : 2002.12.2
	 Author    : ����� 
	 Remark    : none
	********************************************************/
	KTYPE32 int KRB_RunAround(KIL_IDI nId,POINT pts,unsigned char *pImg,int nMode);
	/*******************************************************
	 Prototype : int KRB_RunOnce(KIL_IDI nId,unsigned char *pImg,int nMode)
	 Function  : Blob analysis routine�� �ѹ��� �����Ѵ�. 
				 ���� ũ���� blob�� ã���� ���Ḧ �Ѵ�. 
	 Parameter : nId  -> blob object identifier (input)
				 pts  -> seed point
				 pImg -> image buffer (input)
				 nMode -> 0 (binarized image), 1(grey for white), 2(grey for black)
	 Return    : Number of blob
	 Date      : 2002.12.2
	 Author    : ����� 
	 Remark    : none
	********************************************************/
	KTYPE32 int KRB_RunOnce(KIL_IDI nId,unsigned char *pImg,int nMode);
	/***************************************************
	 Prototype : int KRB_Get(KIL_IDI nBlobId, KIL_ID nIdx,
							 RBLOB_RESULT_STRUCT *pResult)
	 Function  : ���� Blob data�� ��´�.
	 Parameter : nBlobId -> �Ҵ�� Blob identifier
				 nIdx    -> ���� blob identifier
				 pResult -> ���� blob data structure
	 Return    : 1 -> success
				 0 -> fail
	 Date      : 2002.11.27
	 Author    : ����� 
	 Reamrk    : none
	****************************************************/
	KTYPE32 int KRB_Get(KIL_IDI nBlobId,KIL_IDI nIdx, RBLOB_RESULT_STRUCT *pResult);
	/*****************************************************
	 Prototpe : int KRB_GetPoint(KIL_IDI nBlobId,KIL_IDI nIdx,
								 POINT *pt)
	 Function : Blob ������ ��� �� �����͸� ��´�.
	 Parameter: nBlobId -> Blob identifier (input)
				nIdx    -> ���� blob identifier (input)
				pt      -> Point buffer(output, pre-allocated)
	 Return   : 1 -> success
				0 -> fail
	 Date     : 2002.11.27
	 Author   : ����� 
	 Remark   : none
	******************************************************/
	KTYPE32 int KRB_GetPoint(KIL_IDI nBlobId,KIL_IDI nIdx,POINT *pt);
	/**************************************************************************************
	 Prototype : void KRB_SortByCx(RBLOB_RESULT_STRUCT *pArr, int d, int h,BOOL bAscending)
	 Function  : Blob ����� �߽� ��ǥ x�����Ͽ� ������ �Ѵ�. 
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
	 Function  : Blob ����� �߽� ��ǥ y�����Ͽ� ������ �Ѵ�. 
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
	 Function  : Blob ����� ������ ũ�⿡ ���� ������ �Ѵ�. 
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