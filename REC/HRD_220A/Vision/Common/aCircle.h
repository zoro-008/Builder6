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
            int          iGap        ; //���� ���������� ��� �ϴ� ����.
            int          iSearchGap  ; //���� ���� ������ ã�� ����. ���� �׵θ����� +- iSearchGap
            int          iSampleCnt  ; //��� �y���� ���� �Ͽ� ���� ã�� ������. 360/iSampleCnt ���� ���� �˻�.
            int          iStartAng   ; //�y�� �˻� ���� ����.
            int          iEndAng     ; //�y�� �˻� �� ����.
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
                sErrMsg   = ""  ; //�̳𤨹��� memset����.
            }
            double  dCntrX   ;
            double  dCntrY   ;
            double  dRadius  ;
            double  dScore   ;
            bool    bDarkCircle ;
            int     iThreshold  ;

            CLinkedList<TDPoint> lEdges ;

            double  dInspTime  ; //�˻�ð�.
            String  sErrMsg    ;
        } ;

        //Functions
        CCircle();
        ~CCircle();

        static bool Inspect  (CImage * _pImg , TCircle   _tCircle , TPara _tPara , TRslt * _pRslt , FGetCalPos _fpCalPos=NULL);

};

//bool Inspect(CImage * _pImg, TPoint  _tSttPnt , TPoint  _tEndPnt , EDG_Para _tPara , EDG_Rslt * _pRslt);
//bool Inspect(CImage * _pImg, TFPoint _tSttPnt , TFPoint _tEndPnt , EDG_Para _tPara , EDG_Rslt * _pRslt); //double����Ʈ�� ���� �ϴ°� �ƴϰ� ���� int������ �ٲ㼭 �Ѵ�.
//void Paint  (HDC _hHdc , EDG_Rslt _pRslt , EDG_Disp _tDisp , float _fScaleX=1.0 , float _fScaleY=1.0);


#endif
