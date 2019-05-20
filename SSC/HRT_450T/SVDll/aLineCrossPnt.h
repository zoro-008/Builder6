//---------------------------------------------------------------------------
#ifndef aLineCrossPntH
#define aLineCrossPntH
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
class CLineCrossPnt {
    private :
    protected :
        CLine Line1 , Line2 ;
        LIN_Rslt LinRslt1 ;
        LIN_Rslt LinRslt2 ;

    public :
        //Functions
        CLineCrossPnt();
        ~CLineCrossPnt();

    public  :
        bool Inspect  (CImage * _pImg  , TRect _tRect1 , TRect _tRect2 , LCP_Para _tPara , LCP_Rslt * _pRslt);
        void PaintRslt(HDC _hHdc , LCP_Rslt * _pRslt , LCP_Disp _tDisp , float _fScaleX=1.0 , float _fScaleY=1.0);
};
//---------------------------------------------------------------------------
#endif