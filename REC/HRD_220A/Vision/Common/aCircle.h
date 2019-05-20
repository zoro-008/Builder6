//---------------------------------------------------------------------------

#ifndef aCircleH
#define aCircleH
//---------------------------------------------------------------------------
// 2011.12.10
// SUN Kye Won
//---------------------------------------------------------------------------
#include "UnitImage.h"
#include "BaseClass.h"
#include "aEdge.h"



//Ransac
//==============================================================================
class CCircle {
    public  :
        typedef void (__closure * FGetCalPos)(double  , double  , double *  , double * ); //Calibration Pos callback
        struct TPara {
            CEdge::TPara EdgPara     ;
            int          iGap        ; //원의 구성원으로 허용 하는 범위.
            int          iSearchGap  ; //원에 구성 에지를 찾는 범위. 원의 테두리에서 +- iSearchGap
            int          iSampleCnt  ; //몇개의 엦지를 추출 하여 원을 찾을 것인지. 360/iSampleCnt 각도 마다 검사.
            int          iStartAng   ; //엦지 검색 시작 각도.
            int          iEndAng     ; //엦지 검색 끝 각도.
            bool         bLtOnDk     ; //
        } ;

        struct TRslt {
            TRslt(){
                Clear();
            }
            void Clear(){
                dCntrX      = 0.0 ;
                dCntrY      = 0.0 ;
                dRadius     = 0.0 ;
                dScore      = 0.0 ;
                bDarkCircle = false ;
                iThreshold  = 0   ;

                lEdges.DeleteAll();

                dInspTime = 0.0 ;
                sErrMsg   = ""  ; //이놈ㄸ문에 memset못씀.
            }
            double  dCntrX   ;
            double  dCntrY   ;
            double  dRadius  ;
            double  dScore   ;
            bool    bDarkCircle ;
            int     iThreshold  ;

            CLinkedList<TDPoint> lEdges ;

            double  dInspTime  ; //검사시간.
            String  sErrMsg    ;
        } ;

        //Functions
        CCircle();
        ~CCircle();

        static bool Inspect  (CImage * _pImg , TCircle   _tCircle , TPara _tPara , TRslt * _pRslt , FGetCalPos _fpCalPos=NULL);

};

//bool Inspect(CImage * _pImg, TPoint  _tSttPnt , TPoint  _tEndPnt , EDG_Para _tPara , EDG_Rslt * _pRslt);
//bool Inspect(CImage * _pImg, TFPoint _tSttPnt , TFPoint _tEndPnt , EDG_Para _tPara , EDG_Rslt * _pRslt); //double포인트로 시작 하는게 아니고 단지 int형으로 바꿔서 한다.
//void Paint  (HDC _hHdc , EDG_Rslt _pRslt , EDG_Disp _tDisp , float _fScaleX=1.0 , float _fScaleY=1.0);


#endif
