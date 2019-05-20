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
            bool bSimple ; //�ܼ��� ��հ����� �̿��ؼ� Threshold���� Ȯ��.
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
        static bool Inspection(CImage * _pImg , TRect _tRect , TPara _tPara , TRslt * _pRslt); //���侲����尪�� �������� �˰���
};
//---------------------------------------------------------------------------
#endif