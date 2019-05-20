//---------------------------------------------------------------------------

#ifndef aCircleH
#define aCircleH
//---------------------------------------------------------------------------
// 2011.12.10
// SUN Kye Won
//---------------------------------------------------------------------------
#include "Image.h"
#include "SVInspStruct.h"
#include "SVLinkedList.h"
#include "aEdge.h"

//Ransac
//==============================================================================
class CCircle {
    private :
        CEdge    Edge   ;
        CLinkedList<TFPoint> m_lEdgePx ;
    public  :

        //Functions
        CCircle();
        ~CCircle();

        bool Inspect  (CImage * _pImg , TCircle   _tCircle , CCL_Para _tPara , CCL_Rslt * _pRslt  );
        void PaintRslt(HDC      _hHdc , CCL_Rslt *_pRslt   , CCL_Disp _tDisp , float _fScaleX=1.0 , float _fScaleY=1.0);

};

//bool Inspect(CImage * _pImg, TPoint  _tSttPnt , TPoint  _tEndPnt , EDG_Para _tPara , EDG_Rslt * _pRslt);
//bool Inspect(CImage * _pImg, TFPoint _tSttPnt , TFPoint _tEndPnt , EDG_Para _tPara , EDG_Rslt * _pRslt); //float포인트로 시작 하는게 아니고 단지 int형으로 바꿔서 한다.
//void Paint  (HDC _hHdc , EDG_Rslt _pRslt , EDG_Disp _tDisp , float _fScaleX=1.0 , float _fScaleY=1.0);


#endif
