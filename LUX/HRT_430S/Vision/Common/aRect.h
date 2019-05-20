//---------------------------------------------------------------------------
#ifndef aRectH
#define aRectH
//---------------------------------------------------------------------------
#include "aLine.h"

//
//

//---------------------------------------------------------------------------
// 2009.01.02
// SUN Kye Won
//---------------------------------------------------------------------------
class CRect {
    public :
        struct TPara {
            TPara(){
                bLtOnDk     = false ;
                iSampleGap  = 1     ;
                iLineGap    = 1     ;
                iEndOffset  = 0     ;
                iCntrOffset = 0     ;

            }

            bool     bLtOnDk    ; //검은바탕에 밝은 네모 찾기.

            //Line
            int      iSampleGap ;
            int      iLineGap   ;
            int      iEndOffset ;
            int      iCntrOffset;

            //Edge
            CEdge::TPara EdgPara ;

/*
//Edge
iAverage
iCompare
iChkPxLow
iChkPxHgh
iOfsGap
iOfsPxLow
iOfsPxHgh
bUseSubPx

//Line
iScanDirect
bLtToDk
iSampleGap
iLineGap
iEndOffset
iCntrOffset
*/
        } ;
        struct TRslt {
            TRslt(){
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){
                LinRsltL.Clear();
                LinRsltT.Clear();
                LinRsltR.Clear();
                LinRsltB.Clear();

                CrossPntLT.Clear();
                CrossPntRT.Clear();
                CrossPntLB.Clear();
                CrossPntRB.Clear();
                CenterPnt .Clear();

                dLengthL  = 0.0 ;
                dLengthT  = 0.0 ;
                dLengthR  = 0.0 ;
                dLengthB  = 0.0 ;

                dAngleL   = 0.0 ;
                dAngleT   = 0.0 ;
                dAngleR   = 0.0 ;
                dAngleB   = 0.0 ;
                dAngle    = 0.0 ;

                iTtlChkPxCnt = 0 ;
                iChkPxOutCnt = 0 ;

                lEdgeList.DeleteAll() ;

                dInspTime    = 0.0 ;
                sErrMsg      = "";
            }

            CLine::TRslt LinRsltL  ;
            CLine::TRslt LinRsltT  ;
            CLine::TRslt LinRsltR  ;
            CLine::TRslt LinRsltB  ;
            TDPoint  CrossPntLT   ; //교차점.
            TDPoint  CrossPntRT   ;
            TDPoint  CrossPntLB   ;
            TDPoint  CrossPntRB   ;
            TDPoint  CenterPnt    ;

            double   dLengthL     ;
            double   dLengthT     ;
            double   dLengthR     ;
            double   dLengthB     ;

            double   dAngleL      ;
            double   dAngleT      ;
            double   dAngleR      ;
            double   dAngleB      ;
            double   dAngle       ;

            int      iTtlChkPxCnt ;
            int      iChkPxOutCnt ;

            CLinkedList<TDPoint> lEdgeList ;

            double   dInspTime    ; //검사시간.
            String   sErrMsg      ;
        } ;

    private :
    protected :
    public :
        //Functions
        CRect();
        ~CRect();

    public  :
        static bool Inspect  (CImage * _pImg  , TRect _tRectL , TRect _tRectT , TRect _tRectR , TRect _tRectB , TPara _tPara , TRslt * _pRslt);
};


#endif