//---------------------------------------------------------------------------
#ifndef aAutoThresholdH
#define aAutoThresholdH
//---------------------------------------------------------------------------
#include "UnitImage.h"

//
//

//---------------------------------------------------------------------------
// 2009.01.02
// SUN Kye Won
//---------------------------------------------------------------------------
class CAutoThreshold {
    public :
        //Functions
        CAutoThreshold();
        ~CAutoThreshold();

        struct TPara {
            bool bSimple ; //단순히 평균값만을 이용해서 Threshold값을 확인.
        } ;

        struct TRslt {
            TRslt(){
                Clear();
            }
            ~TRslt(){
            }
            void Clear(){
                iThreshold = 0   ;
                dInspTime  = 0.0 ;
                sErrMsg    = ""  ;
            }

            int     iThreshold ;
            double  dInspTime  ;
            String  sErrMsg    ;
        } ;

    public  :
        static bool Inspection(CImage * _pImg , TRect _tRect , TPara _tPara , TRslt * _pRslt); //오토쓰레숄드값을 가져오는 알고리즘
};
//---------------------------------------------------------------------------
#endif