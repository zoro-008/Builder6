//---------------------------------------------------------------------------
#ifndef aEdgeH
#define aEdgeH
//---------------------------------------------------------------------------
#include "UnitImage.h"
#include "BaseDefine.h"

//
//

//---------------------------------------------------------------------------
// 2009.01.02
// SUN Kye Won
//---------------------------------------------------------------------------
class CEdge {
    public :
        struct TPara {
            TPara(){
                iAverage=1;
                iCompare=1;

                iChkPxLow = 0 ;
                iChkPxHgh = 0 ;
                iOfsGap   = 0 ;
                iOfsPxLow = 0 ;
                iOfsPxHgh = 0 ;

            }
            int  iAverage  ; //row 데이터를 smoothing한 데이터의 범위 평균값
            int  iCompare  ; //avr을 통해 dif 데이터를 뽑아 낼때 비교 하는 범위값.

            int  iChkPxLow ; //Low와 High 사이에 있는 엣지만 찾는다.
            int  iChkPxHgh ; //둘다 0 이면 사용 안함.
            int  iOfsGap   ; //Gap만큼 이동하여 OfsPxLow High사이에 있는 엣지만 찾는다.
            int  iOfsPxLow ; //Gap이 0이면 사용 안한다.
            int  iOfsPxHgh ;

        } ;
        struct TRslt {
            TRslt(){
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){
                DkToLtPnt.Clear();
                LtToDkPnt.Clear();

                iDkToLtGap = 0 ;
                iLtToDkGap = 0 ;
                iDkToLtPx  = 0 ;
                iLtToDkPx  = 0 ;
                StartPnt.x = 0 ;
                StartPnt.y = 0 ;
                EndPnt  .x = 0 ;
                EndPnt  .y = 0 ;

                dInspTime = 0.0 ;
                sErrMsg   = ""  ; //이놈ㄸ문에 memset못씀.
            }
            TDPoint DkToLtPnt  ; //밝아지는 엣지.    서브픽셀.
            TDPoint LtToDkPnt  ; //어두워지는 엦지.    서브픽셀.

            int     iDkToLtGap ; //밝아지는 엣지 차.
            int     iLtToDkGap ; //어두워지는 엦지 차.
            int     iDkToLtPx  ; //밝아지는 엣지 픽셀값.
            int     iLtToDkPx  ; //어두워지는 엣지 픽셀값.
            TPoint  StartPnt   ; //검사시작 포인트.
            TPoint  EndPnt     ; //검사끝나는 포인트.

            double  dInspTime  ; //검사시간.
            String  sErrMsg    ;
        } ;
        //Functions
        CEdge();
        ~CEdge();

        static bool Inspect  (CImage * _pImg, TPoint  _tSttPnt , TPoint  _tEndPnt , TPara _tPara , TRslt * _pRslt);
};
//---------------------------------------------------------------------------
#endif