//---------------------------------------------------------------------------
#ifndef aRectH
#define aRectH
//---------------------------------------------------------------------------
#include "SVInspStruct.h"
#include "Image.h"
#include "aLine.h"

//
//

//---------------------------------------------------------------------------
// 2009.01.02
// SUN Kye Won
//---------------------------------------------------------------------------
class CRect {
    private :
    protected :
        CLine LineL , LineT ,LineR , LineB ;
        LIN_Rslt LinRsltL ;
        LIN_Rslt LinRsltT ;
        LIN_Rslt LinRsltR ;
        LIN_Rslt LinRsltB ;


    public :
        //Functions
        CRect();
        ~CRect();

    public  :
        bool Inspect  (CImage * _pImg  , TRect _tRectL , TRect _tRectT , TRect _tRectR , TRect _tRectB , RCT_Para _tPara , RCT_Rslt * _pRslt);
        void PaintRslt(HDC _hHdc , RCT_Rslt * _pRslt , RCT_Disp _tDisp , float _fScaleX=1.0 , float _fScaleY=1.0);
};


class CWfrRect {
    private :
    protected :
        CLine LineL , LineT ,LineR , LineB ;
        LIN_Rslt LinRsltL ;
        LIN_Rslt LinRsltT ;
        LIN_Rslt LinRsltR ;
        LIN_Rslt LinRsltB ;


    public :
        //Functions
        CWfrRect();
        ~CWfrRect();

    public  :
        bool Inspect  (CImage * _pImg  , TRect _tRectL , TRect _tRectT , TRect _tRectR , TRect _tRectB , WRT_Para _tPara , WRT_Rslt * _pRslt);
        void PaintRslt(HDC _hHdc , WRT_Rslt * _pRslt , WRT_Disp _tDisp , float _fScaleX=1.0 , float _fScaleY=1.0);
};
//---------------------------------------------------------------------------
#endif