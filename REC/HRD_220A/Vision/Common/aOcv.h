//---------------------------------------------------------------------------

#ifndef aOcvH
#define aOcvH

#include "SVInspStruct.h"
#include "SVLinkedList.h"
#include "Image.h"
#include "Area.h"
#include "GdiUnit.h"
//---------------------------------------------------------------------------




class COcv {
    private :
        //CImage * m_pTrainImg  ;
        //CArea  * m_pInspArea  ;




    public  :

        //Functions
        COcv();
        ~COcv();

        bool Train     (CImage * _pImg , TRect _Rect , OCV_TrPr _tTrPr ,
                        CArea * _pTrainArea , CImage * _pTrainImg);

        bool Inspect   (CImage * _pImg , CArea * _pTrainArea , CImage * _pTrainImg, TRect _tInspRect , OCV_Para _tPara ,
                        CArea * _pRsltArea , OCV_Rslt * _pRslt);
        void PaintRslt (HDC _hHdc , CArea * _pRsltArea , OCV_Rslt * _pRslt, OCV_Disp _tDisp, float _fScaleX , float _fScaleY);

};
#endif

/*
        bool       Inspect  (CImage * _pImg, TPoint  _tSttPnt , TPoint  _tEndPnt , EDG_Para _tPara , EDG_Rslt * _pRslt);
        bool       Inspect  (CImage * _pImg, TFPoint _tSttPnt , TFPoint _tEndPnt , EDG_Para _tPara , EDG_Rslt * _pRslt); //float포인트로 시작 하는게 아니고 단지 int형으로 바꿔서 한다.
        void       PaintRslt(HDC _hHdc , EDG_Rslt _pRslt , EDG_Disp _tDisp , float _fScaleX=1.0 , float _fScaleY=1.0);
*/
