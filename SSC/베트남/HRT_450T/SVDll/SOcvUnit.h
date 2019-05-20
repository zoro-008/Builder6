//---------------------------------------------------------------------------

#ifndef SOcvUnitH
#define SOcvUnitH

//#include "STraceUnit.h"
#include "SvImage.h"
#include "SArea.h"
#include "SLinkedList.h"
//---------------------------------------------------------------------------
#define MAX_OCR_CHAR     100

#define NO_INSP_AREA     0
#define DARK_INSP_AREA   1
#define LIGHT_INSP_AREA  2





class SOcv {
    private :
        SImage * m_pTrainImg  ;
        SArea  * m_pInspArea  ;

    public  :
        struct TRslt {
           int        iDkErrPx  ;
           int        iLtErrPx  ;
           int        iStartX   ;
           int        iStartY   ;


           float      fInspTime ;

           SLinkedList<TPoint> DkFailPx ;
           SLinkedList<TPoint> LtFailPx ;
        } Rslt;

        struct TPara {
           int iPxOffset   ;
           int iThreshold  ;
        } Para;

        //Functions
        SOcv();
        ~SOcv();

        int  OnTrain     (SImage * _pImg , TRect _Rect);
        int  OnInspection(SImage * _pImg , int _iSx , int _iSy);
        void OnPaintRslt (HDC _hHdc);
        void OnPaintTrain(HDC _hHdc);
        bool OnLoad      (AnsiString _sFilePath);   //Load Train Para.
        bool OnSave      (AnsiString _sFilePath);
};
#endif
