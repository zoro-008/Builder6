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
    int  iAverage  ; //row 데이터를 smoothing한 데이터의 범위 평균값
    int  iCompare  ; //avr을 통해 dif 데이터를 뽑아 낼때 비교 하는 범위값.

    int  iChkPxLow ; //Low와 High 사이에 있는 엣지만 찾는다.
    int  iChkPxHgh ; //둘다 0 이면 사용 안함.
    int  iOfsGap   ; //Gap만큼 이동하여 OfsPxLow High사이에 있는 엣지만 찾는다.
    int  iOfsPxLow ; //Gap이 0이면 사용 안한다.
    int  iOfsPxHgh ;
} ;
struct EDG_Rslt {
    EDG_Rslt(){
        memset(this , 0 , sizeof(EDG_Rslt));
    }
    TFPoint DkToLtPnt  ; //밝아지는 엣지.
    TFPoint LtToDkPnt  ; //어두워지는 엦지.

    TFPoint DkToLtPntS ; //밝아지는 엣지.    서브픽셀.
    TFPoint LtToDkPntS ; //어두워지는 엦지.    서브픽셀.

    TFPoint DkToLtPntSS ; //밝아지는 엣지.    서브픽셀.  루트 연산.
    TFPoint LtToDkPntSS ; //어두워지는 엦지.    서브픽셀.  루트 연산.

    int     iDkToLtGap ; //밝아지는 엣지 차.
    int     iLtToDkGap ; //어두워지는 엦지 차.
    int     iDkToLtPx  ; //밝아지는 엣지 픽셀값.
    int     iLtToDkPx  ; //어두워지는 엣지 픽셀값.
    TPoint  StartPnt   ; //검사시작 포인트.
    TPoint  EndPnt     ; //검사끝나는 포인트.

    float   fInspTime  ; //검사시간.
} ;
struct EDG_Disp {
    EDG_Disp() {
        clDkToLtPnt = clNone ;
        clLtToDkPnt = clNone ;
        clText      = clNone ;
    }
    COLORREF clDkToLtPnt  ; //밝아지는 엣지.
    COLORREF clLtToDkPnt  ; //어두워지는 엦지.
    COLORREF clText       ; //
} ;

//Line..........................................................................
enum EN_LINE_SCAN_DIRECT {
    lsUpToDn = 0 ,   //위에서 아래로
    lsDnToUp = 1 ,   //아래에서 위로
    lsLtToRt = 2 ,   //왼쪽에서 오른쪽으로
    lsRtToLt = 3 ,   //오른쪽에서 왼쪽으로
};

struct LIN_Para {
    LIN_Para(){
        memset(this , 0 , sizeof(LIN_Para));
        iSampleGap=1;
        iLineGap=1;
    }

    EN_LINE_SCAN_DIRECT iScanDirect  ; //렉트안의 라인을 스캔하는 방향
    bool                bLtToDk      ; //밝은색에서 검은색으로 가는 라인 검색.
    int                 iSampleGap   ; //엣지 검출 간격
    int                 iLineGap     ; //선을 구성원으로 인정하는 간격.
    int                 iEndOffset   ; //검색하는 양끝단 범위 오프셑
    int                 iCntrOffset  ; //검색하는 가운데 범위 오프셑


    EDG_Para EdgPara ;

} ;
struct LIN_Rslt {
    LIN_Rslt(){
        memset(this , 0 , sizeof(LIN_Rslt));
    }
    float   fA         ; //직선의 기울기
    float   fB         ; //직선의 y절편.
    float   fAngle     ; //직선의 각도. 0~180
    TFPoint StartPnt   ; //검사시작 포인트.
    TFPoint EndPnt     ; //검사끝나는 포인트.

    float   fInspTime  ; //검사시간.
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

    //TFPoint  OriPnt   ; //처음 세이브 할때 기준값 저장.
    //float    OriAngle ; //처음 세이브 할때 기준값 저장. Line1의 앵글 저장.

} ;
struct LCP_Rslt {
    LCP_Rslt(){
        memset(this , 0 , sizeof(LCP_Rslt));
    }

    LIN_Rslt LinRslt1  ;
    LIN_Rslt LinRslt2  ;
    TFPoint  CrossPnt  ; //교차점.

    //float    fOffsetX  ; //세이브한 파라 기준값에서 차.
    //float    fOffsetY  ; //세이브한 파라 기준값에서 차.
    //float    fOffsetAng; //세이브한 파라 기준값에서 차.

    float    fInspTime ; //검사시간.
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

    bool     bLtOnDk  ; //검은바탕에 밝은 네모 찾기.
    int      iChkOfs  ; //사각형의 모양 잘찾았는지 확인.
    int      iLowThsd ; //사각형 모양 잘 찾았는지 확인 하는 Thsd
    int      iHighThsd; //사각형 모양 잘 찾았는지 확인 하는 Thsd

    LIN_Para LinParaL ;
    LIN_Para LinParaT ;
    LIN_Para LinParaR ;
    LIN_Para LinParaB ;

    //TFPoint  OriPnt   ; //처음 세이브 할때 기준값 저장.
    //float    OriAngle ; //처음 세이브 할때 기준값 저장. 앵글 저장.
} ;
struct RCT_Rslt {
    RCT_Rslt(){
        memset(this , 0 , sizeof(RCT_Rslt));
    }

    LIN_Rslt LinRsltL  ;
    LIN_Rslt LinRsltT  ;
    LIN_Rslt LinRsltR  ;
    LIN_Rslt LinRsltB  ;
    TFPoint  CrossPntLT ; //교차점.
    TFPoint  CrossPntRT ;
    TFPoint  CrossPntLB ;
    TFPoint  CrossPntRB ;
    TFPoint  CenterPnt  ;
    float    fAngle     ;

    int      iTtlChkPxCnt ;
    int      iChkPxOutCnt ;

    TFPoint  OffsetPntLT ; //교차점에서 픽셀 확인 하는 오프셑 포인트
    TFPoint  OffsetPntRT ;
    TFPoint  OffsetPntLB ;
    TFPoint  OffsetPntRB ;

    //float    fOffsetX  ; //세이브한 파라 기준값에서 차.
    //float    fOffsetY  ; //세이브한 파라 기준값에서 차.
    //float    fOffsetAng; //세이브한 파라 기준값에서 차.

    float    fInspTime ; //검사시간.
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

    bool     bLtOnDk  ; //검은바탕에 밝은 네모 찾기.
    int      iChkOfs  ; //사각형의 모양 잘찾았는지 확인.
    int      iLowThsd ; //사각형 모양 잘 찾았는지 확인 하는 Thsd
    int      iHighThsd; //사각형 모양 잘 찾았는지 확인 하는 Thsd

    LIN_Para LinParaL ;
    LIN_Para LinParaT ;
    LIN_Para LinParaR ;
    LIN_Para LinParaB ;

    //TFPoint  OriPnt   ; //처음 세이브 할때 기준값 저장.
    //float    OriAngle ; //처음 세이브 할때 기준값 저장. 앵글 저장.
} ;

struct WRT_Rslt {
    WRT_Rslt(){
        memset(this , 0 , sizeof(WRT_Rslt));
    }

    LIN_Rslt LinRsltL  ;
    LIN_Rslt LinRsltT  ;
    LIN_Rslt LinRsltR  ;
    LIN_Rslt LinRsltB  ;
    TFPoint  CrossPntLT ; //교차점.
    TFPoint  CrossPntRT ;
    TFPoint  CrossPntLB ;
    TFPoint  CrossPntRB ;
    TFPoint  CenterPnt  ;
    float    fAngle     ;

    int      iTtlChkPxCnt ;
    int      iChkPxOutCnt ;

    TFPoint  OffsetPntLT ; //교차점에서 픽셀 확인 하는 오프셑 포인트
    TFPoint  OffsetPntRT ;
    TFPoint  OffsetPntLB ;
    TFPoint  OffsetPntRB ;

    TRect    LTRect ;
    TRect    RTRect ;
    TRect    RBRect ;
    TRect    LBRect ;




    //float    fOffsetX  ; //세이브한 파라 기준값에서 차.
    //float    fOffsetY  ; //세이브한 파라 기준값에서 차.
    //float    fOffsetAng; //세이브한 파라 기준값에서 차.

    float    fInspTime ; //검사시간.
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

    float fInspTime ; //검사시간.
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

    float fInspTime ; //검사시간.
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
    int  iPitchX     ; //블랍 검색 X 피치
    int  iPitchY     ; //블랍 검색 Y 피치
    int  iStartXOfs  ; //처음 스타트 하는 위치 오프셑.
    int  iStartYOfs  ; //처음 스타트 하는 위치 오프셑.
    int  iThreshold  ; //검색 트레숄드.
    bool bFindLtOnDk ; //검은바탕흰물체인지 흰바탕 검은물체인지.
    bool bIgnrInside ; //블랍 내부 블랍 무시.
    bool bIgnrSide   ; //사각형 테투리 경계에 있는 놈들 무시.

    //Filtering.
    int   iMinArea        ; int   iMaxArea        ; //에리아 민맥스값
    int   iMinWidth       ; int   iMaxWidth       ; //넓이
    int   iMinHeight      ; int   iMaxHeight      ; //놉이
    int   iMinBoundary    ; int   iMaxBoundary    ; //테두리
    float fMinCompactness ; float fMaxCompactness ; //에리아/테두리
    float fMinAngle       ; float fMaxAngle       ; //각도 구현되지 않았음.
    float fMinAspectRatio ; float fMaxAspectRatio ; //
} ;

struct BLB_Rslt {
    BLB_Rslt(){
        iBlobCnt = 0 ;
        pBlobs   = NULL ;
    }
    ~BLB_Rslt(){ //이런젠장.....
        iBlobCnt = 0 ;
        if(pBlobs) {
            delete [] pBlobs ;
            pBlobs = NULL ;
        }
    }


    int     iBlobCnt  ; //나중에 시간 많이 걸리면 CLinkedList<SBlob> 으로 구현해보기 안될수도 있다.
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
    ~CAL_Rslt(){ //이런젠장.....
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
    int      iGap        ; //원의 구성원으로 허용 하는 범위.
    int      iSearchGap  ; //원에 구성 에지를 찾는 범위. 원의 테두리에서 +- iSearchGap
    int      iSampleCnt  ; //몇개의 엦지를 추출 하여 원을 찾을 것인지. 360/iSampleCnt 각도 마다 검사.
    int      iStartAng   ; //엦지 검색 시작 각도.
    int      iEndAng     ; //엦지 검색 끝 각도.
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
    int   iTrThreshold ; //검사 임계값
    int   iTrInspOft   ;
    int   iTrNoInspOft ;
    int   iInspFreq    ; //검사빈도수 높으면 높을수록 빨라짐.
    float fSinc        ; //싱크 인데 일단 안씀.
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

