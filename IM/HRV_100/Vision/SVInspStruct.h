//---------------------------------------------------------------------------

#ifndef SVInspStructH
#define SVInspStructH
#include <Graphics.hpp>

#include "SVDefine.h"
#include "SVLinkedList.h"

#include "Image.h"
#include "Area.h"




//---------------------------------------------------------------------------
//Edge..........................................................................
struct EDG_Para {
    EDG_Para(){
        memset(this , 0 , sizeof(EDG_Para));
        iAverage=1;
        iCompare=1;
    }
    int  iAverage  ; //row �����͸� smoothing�� �������� ���� ��հ�
    int  iCompare  ; //avr�� ���� dif �����͸� �̾� ���� �� �ϴ� ������.

    int  iChkPxLow ; //Low�� High ���̿� �ִ� ������ ã�´�.
    int  iChkPxHgh ; //�Ѵ� 0 �̸� ��� ����.
    int  iOfsGap   ; //Gap��ŭ �̵��Ͽ� OfsPxLow High���̿� �ִ� ������ ã�´�.
    int  iOfsPxLow ; //Gap�� 0�̸� ��� ���Ѵ�.
    int  iOfsPxHgh ;
} ;
struct EDG_Rslt {
    EDG_Rslt(){
        memset(this , 0 , sizeof(EDG_Rslt));
    }
    TFPoint DkToLtPnt  ; //������� ����.
    TFPoint LtToDkPnt  ; //��ο����� �y��.

    TFPoint DkToLtPntS ; //������� ����.    �����ȼ�.
    TFPoint LtToDkPntS ; //��ο����� �y��.    �����ȼ�.

    TFPoint DkToLtPntSS ; //������� ����.    �����ȼ�.  ��Ʈ ����.
    TFPoint LtToDkPntSS ; //��ο����� �y��.    �����ȼ�.  ��Ʈ ����.

    int     iDkToLtGap ; //������� ���� ��.
    int     iLtToDkGap ; //��ο����� �y�� ��.
    int     iDkToLtPx  ; //������� ���� �ȼ���.
    int     iLtToDkPx  ; //��ο����� ���� �ȼ���.
    TPoint  StartPnt   ; //�˻���� ����Ʈ.
    TPoint  EndPnt     ; //�˻糡���� ����Ʈ.

    float   fInspTime  ; //�˻�ð�.
} ;
struct EDG_Disp {
    EDG_Disp() {
        clDkToLtPnt = clNone ;
        clLtToDkPnt = clNone ;
        clText      = clNone ;
    }
    COLORREF clDkToLtPnt  ; //������� ����.
    COLORREF clLtToDkPnt  ; //��ο����� �y��.
    COLORREF clText       ; //
} ;

//Line..........................................................................
enum EN_LINE_SCAN_DIRECT {
    lsUpToDn = 0 ,   //������ �Ʒ���
    lsDnToUp = 1 ,   //�Ʒ����� ����
    lsLtToRt = 2 ,   //���ʿ��� ����������
    lsRtToLt = 3 ,   //�����ʿ��� ��������
};

struct LIN_Para {
    LIN_Para(){
        memset(this , 0 , sizeof(LIN_Para));
        iSampleGap=1;
        iLineGap=1;
    }

    EN_LINE_SCAN_DIRECT iScanDirect  ; //��Ʈ���� ������ ��ĵ�ϴ� ����
    bool                bLtToDk      ; //���������� ���������� ���� ���� �˻�.
    int                 iSampleGap   ; //���� ���� ����
    int                 iLineGap     ; //���� ���������� �����ϴ� ����.
    int                 iEndOffset   ; //�˻��ϴ� �糡�� ���� �����V
    int                 iCntrOffset  ; //�˻��ϴ� ��� ���� �����V


    EDG_Para EdgPara ;

} ;
struct LIN_Rslt {
    LIN_Rslt(){
        memset(this , 0 , sizeof(LIN_Rslt));
    }
    float   fA         ; //������ ����
    float   fB         ; //������ y����.
    float   fAngle     ; //������ ����. 0~180
    TFPoint StartPnt   ; //�˻���� ����Ʈ.
    TFPoint EndPnt     ; //�˻糡���� ����Ʈ.

    float   fInspTime  ; //�˻�ð�.
} ;
struct LIN_Disp {
    LIN_Disp() {
        clLine = clNone ;
        clText = clNone ;
        clEdge = clNone ;
    }
    COLORREF clLine  ;
    COLORREF clText  ;
    COLORREF clEdge  ;
} ;

//LineCrossPnt..................................................................
struct LCP_Para {
    LCP_Para(){
        memset(this , 0 , sizeof(LCP_Para));
    }

    LIN_Para LinPara1 ;
    LIN_Para LinPara2 ;

    //TFPoint  OriPnt   ; //ó�� ���̺� �Ҷ� ���ذ� ����.
    //float    OriAngle ; //ó�� ���̺� �Ҷ� ���ذ� ����. Line1�� �ޱ� ����.

} ;
struct LCP_Rslt {
    LCP_Rslt(){
        memset(this , 0 , sizeof(LCP_Rslt));
    }

    LIN_Rslt LinRslt1  ;
    LIN_Rslt LinRslt2  ;
    TFPoint  CrossPnt  ; //������.

    //float    fOffsetX  ; //���̺��� �Ķ� ���ذ����� ��.
    //float    fOffsetY  ; //���̺��� �Ķ� ���ذ����� ��.
    //float    fOffsetAng; //���̺��� �Ķ� ���ذ����� ��.

    float    fInspTime ; //�˻�ð�.
} ;
struct LCP_Disp {
    LCP_Disp() {
        clPoint = clNone ;
    }
    LIN_Disp LinDisp1 ;
    LIN_Disp LinDisp2 ;
    COLORREF clPoint  ;
    COLORREF clText   ;
} ;

//Rect..........................................................................
struct RCT_Para {
    RCT_Para(){
        memset(this , 0 , sizeof(RCT_Para));
    }

    bool     bLtOnDk  ; //���������� ���� �׸� ã��.
    int      iChkOfs  ; //�簢���� ��� ��ã�Ҵ��� Ȯ��.
    int      iLowThsd ; //�簢�� ��� �� ã�Ҵ��� Ȯ�� �ϴ� Thsd
    int      iHighThsd; //�簢�� ��� �� ã�Ҵ��� Ȯ�� �ϴ� Thsd

    LIN_Para LinParaL ;
    LIN_Para LinParaT ;
    LIN_Para LinParaR ;
    LIN_Para LinParaB ;

    //TFPoint  OriPnt   ; //ó�� ���̺� �Ҷ� ���ذ� ����.
    //float    OriAngle ; //ó�� ���̺� �Ҷ� ���ذ� ����. �ޱ� ����.
} ;
struct RCT_Rslt {
    RCT_Rslt(){
        memset(this , 0 , sizeof(RCT_Rslt));
    }

    LIN_Rslt LinRsltL  ;
    LIN_Rslt LinRsltT  ;
    LIN_Rslt LinRsltR  ;
    LIN_Rslt LinRsltB  ;
    TFPoint  CrossPntLT ; //������.
    TFPoint  CrossPntRT ;
    TFPoint  CrossPntLB ;
    TFPoint  CrossPntRB ;
    TFPoint  CenterPnt  ;
    float    fAngle     ;

    int      iTtlChkPxCnt ;
    int      iChkPxOutCnt ;

    TFPoint  OffsetPntLT ; //���������� �ȼ� Ȯ�� �ϴ� �����V ����Ʈ
    TFPoint  OffsetPntRT ;
    TFPoint  OffsetPntLB ;
    TFPoint  OffsetPntRB ;

    //float    fOffsetX  ; //���̺��� �Ķ� ���ذ����� ��.
    //float    fOffsetY  ; //���̺��� �Ķ� ���ذ����� ��.
    //float    fOffsetAng; //���̺��� �Ķ� ���ذ����� ��.

    float    fInspTime ; //�˻�ð�.
} ;
struct RCT_Disp {
    RCT_Disp() {
        clPoint = clNone ;
        clText  = clNone ;
        clRect  = clNone ;
    }
    LIN_Disp LinDisp  ;
    COLORREF clPoint  ;
    COLORREF clText   ;
    COLORREF clRect   ;
} ;

//Wfr Rect..........................................................................
struct WRT_Para {
    WRT_Para(){
        memset(this , 0 , sizeof(WRT_Para));
    }

    bool     bLtOnDk  ; //���������� ���� �׸� ã��.
    int      iChkOfs  ; //�簢���� ��� ��ã�Ҵ��� Ȯ��.
    int      iLowThsd ; //�簢�� ��� �� ã�Ҵ��� Ȯ�� �ϴ� Thsd
    int      iHighThsd; //�簢�� ��� �� ã�Ҵ��� Ȯ�� �ϴ� Thsd

    LIN_Para LinParaL ;
    LIN_Para LinParaT ;
    LIN_Para LinParaR ;
    LIN_Para LinParaB ;

    //TFPoint  OriPnt   ; //ó�� ���̺� �Ҷ� ���ذ� ����.
    //float    OriAngle ; //ó�� ���̺� �Ҷ� ���ذ� ����. �ޱ� ����.
} ;

struct WRT_Rslt {
    WRT_Rslt(){
        memset(this , 0 , sizeof(WRT_Rslt));
    }

    LIN_Rslt LinRsltL  ;
    LIN_Rslt LinRsltT  ;
    LIN_Rslt LinRsltR  ;
    LIN_Rslt LinRsltB  ;
    TFPoint  CrossPntLT ; //������.
    TFPoint  CrossPntRT ;
    TFPoint  CrossPntLB ;
    TFPoint  CrossPntRB ;
    TFPoint  CenterPnt  ;
    float    fAngle     ;

    int      iTtlChkPxCnt ;
    int      iChkPxOutCnt ;

    TFPoint  OffsetPntLT ; //���������� �ȼ� Ȯ�� �ϴ� �����V ����Ʈ
    TFPoint  OffsetPntRT ;
    TFPoint  OffsetPntLB ;
    TFPoint  OffsetPntRB ;

    TRect    LTRect ;
    TRect    RTRect ;
    TRect    RBRect ;
    TRect    LBRect ;




    //float    fOffsetX  ; //���̺��� �Ķ� ���ذ����� ��.
    //float    fOffsetY  ; //���̺��� �Ķ� ���ذ����� ��.
    //float    fOffsetAng; //���̺��� �Ķ� ���ذ����� ��.

    float    fInspTime ; //�˻�ð�.
} ;
struct WRT_Disp {
    WRT_Disp() {
        clPoint = clNone ;
        clText  = clNone ;
        clRect  = clNone ;
    }
    LIN_Disp LinDisp  ;
    COLORREF clPoint  ;
    COLORREF clText   ;
    COLORREF clRect   ;
} ;



//Threshold.....................................................................
struct TSD_Para {
    TSD_Para(){
        memset(this , 0 , sizeof(TSD_Para));
    }
    int iThresholdLow ;
    int iThresholdHigh;
} ;
struct TSD_Rslt {
    TSD_Rslt(){
        memset(this , 0 , sizeof(TSD_Rslt));
    }
    float fCntrX   ;
    float fCntrY   ;
    int   iPxCnt   ;
    float fAverage ;
    TRect tRect    ;

    float fInspTime ; //�˻�ð�.
} ;
struct TSD_Disp {
    TSD_Disp() {
        clRect = clNone ;
        clText = clNone ;
        clDark = clNone ;
        clLight= clNone ;
    }
    COLORREF clRect  ;
    COLORREF clText  ;
    COLORREF clDark  ;
    COLORREF clLight ;

} ;

//ThresholdRect.....................................................................
/*struct TSR_Para {
    TSR_Para(){
        memset(this , 0 , sizeof(TSR_Para));
    }
    int  iThresholdLow ;
    int  iThresholdHigh;
    int  iCntX         ;
    int  iCntY         ;
    int  iLeftOffset   ;
    int  iRightOffset  ;
    int  iTopOffset    ;
    int  iBottomOffset ;
    bool bCheckInside  ;

} ;
struct TSR_Rslt {
    TSR_Rslt(){
        memset(this , 0 , sizeof(TSR_Rslt));
    }
    int   iPxCnt   ;
    int   iInPxCnt ;
    float fAverage ;
    TRect tRect    ;

    float fInspTime ; //�˻�ð�.
} ;
struct TSR_Disp {
    TSR_Disp() {
        clRect = clNone ;
        clText = clNone ;
        clDark = clNone ;
        clLight= clNone ;
    }
    COLORREF clRect  ;
    COLORREF clText  ;
    COLORREF clDark  ;
    COLORREF clLight ;

} ;  */

//Blob..........................................................................
struct BLB_Para {
    int  iPitchX     ; //��� �˻� X ��ġ
    int  iPitchY     ; //��� �˻� Y ��ġ
    int  iStartXOfs  ; //ó�� ��ŸƮ �ϴ� ��ġ �����V.
    int  iStartYOfs  ; //ó�� ��ŸƮ �ϴ� ��ġ �����V.
    int  iThreshold  ; //�˻� Ʈ�����.
    bool bFindLtOnDk ; //����������ü���� ����� ������ü����.
    bool bIgnrInside ; //��� ���� ��� ����.
    bool bIgnrSide   ; //�簢�� ������ ��迡 �ִ� ��� ����.

    //Filtering.
    int   iMinArea        ; int   iMaxArea        ; //������ �θƽ���
    int   iMinWidth       ; int   iMaxWidth       ; //����
    int   iMinHeight      ; int   iMaxHeight      ; //����
    int   iMinBoundary    ; int   iMaxBoundary    ; //�׵θ�
    float fMinCompactness ; float fMaxCompactness ; //������/�׵θ�
    float fMinAngle       ; float fMaxAngle       ; //���� �������� �ʾ���.
    float fMinAspectRatio ; float fMaxAspectRatio ; //
} ;

struct BLB_Rslt {
    BLB_Rslt(){
        iBlobCnt = 0 ;
        pBlobs   = NULL ;
    }
    ~BLB_Rslt(){ //�̷�����.....
        iBlobCnt = 0 ;
        if(pBlobs) {
            delete [] pBlobs ;
            pBlobs = NULL ;
        }
    }


    int     iBlobCnt  ; //���߿� �ð� ���� �ɸ��� CLinkedList<SBlob> ���� �����غ��� �ȵɼ��� �ִ�.
    SBlob * pBlobs    ;

    float   fInspTime ;
} ;

struct BLB_Disp {
    BLB_Disp() {
        clRect     = clNone ;
        clCross    = clNone ;
        clBoundary = clNone ;
        clText     = clNone ;
    }
    COLORREF clRect     ;
    COLORREF clCross    ;
    COLORREF clBoundary ;
    COLORREF clText     ;
} ;

//Calibration...................................................................
struct CAL_Para {
    BLB_Para BlbPara ;

    float fDotSize  ;
    float fDotPitch ;

} ;

struct CAL_Rslt {
    CAL_Rslt(){
        fXPxRes = 0.0 ;
        fYPxRes = 0.0 ;
    }
    ~CAL_Rslt(){ //�̷�����.....
    }

    BLB_Rslt BlbRslt ;

    float   fXPxRes ;
    float   fYPxRes ;

    float   fInspTime ;
} ;

struct CAL_Disp {
    BLB_Disp BlbDisp ;
    COLORREF clText  ;
} ;

//Circle........................................................................
struct CCL_Para {
    EDG_Para EdgPara     ;
    int      iGap        ; //���� ���������� ��� �ϴ� ����.
    int      iSearchGap  ; //���� ���� ������ ã�� ����. ���� �׵θ����� +- iSearchGap
    int      iSampleCnt  ; //��� �y���� ���� �Ͽ� ���� ã�� ������. 360/iSampleCnt ���� ���� �˻�.
    int      iStartAng   ; //�y�� �˻� ���� ����.
    int      iEndAng     ; //�y�� �˻� �� ����.
} ;

struct CCL_Rslt {
    float  fCntrX   ;
    float  fCntrY   ;
    float  fRadius  ;
    float  fScore   ;
    bool   bDarkCircle ;
    int    iThreshold  ;

    float  fTime;
} ;

struct CCL_Disp {
    CCL_Disp() {
        clCircle   = clNone ;
        clEdge     = clNone ;
        clText     = clNone ;
    }
    COLORREF clCircle ;
    COLORREF clEdge   ;
    COLORREF clText   ;
} ;

//OCV...........................................................................
enum OCV_TRAIN_AREA {
    otUnknown = 0 ,
    otNoInsp  = 1 ,
    otDkInsp  = 2 ,
    otLtInsp  = 3 ,
    otTemp    = 4 ,
    otInsp    = 5 ,

    MAX_OCV_TRAIN_AREA
};

struct OCV_Para { //Insp Para
    int   iTrThreshold ; //�˻� �Ӱ谪
    int   iTrInspOft   ;
    int   iTrNoInspOft ;
    int   iInspFreq    ; //�˻�󵵼� ������ �������� ������.
    float fSinc        ; //��ũ �ε� �ϴ� �Ⱦ�.
} ;

enum OCV_RSLT_AREA {
    orNone    = 0 ,
    orInsp    = 1 ,
    MAX_OCV_RSLT_AREA
};

struct OCV_Rslt {
    int   iDkPxCnt ;
    int   iLtPxCnt ;
    float fSinc    ;
    int   iPosX    ;
    int   iPosY    ;
    TRect tRect    ;
} ;

struct OCV_Disp {
    OCV_Disp() {
        clInspPx   = clNone ;
        clText     = clNone ;
        clCenter   = clNone ;
        clRect     = clNone ;
    }
    COLORREF clInspPx   ;
    COLORREF clNGPx     ;
    COLORREF clText     ;
    COLORREF clCenter   ;
    COLORREF clRect     ;
} ;















#endif

