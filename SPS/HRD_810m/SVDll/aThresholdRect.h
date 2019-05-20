//---------------------------------------------------------------------------
#ifndef aThresholdRectH
#define aThresholdRectH
//---------------------------------------------------------------------------
#include "SVInspStruct.h"
#include "Image.h"

//
//

//---------------------------------------------------------------------------
// 2009.01.02
// SUN Kye Won
//---------------------------------------------------------------------------
class CThresholdRect {
    private :
    protected :
    public :
        //Functions
        CThresholdRect();
        ~CThresholdRect();

    public  :
        bool Inspect  (CImage * _pImg  , TRect _tRect , TSD_Para _tPara , TSD_Rslt * _pRslt);
        void PaintRslt(HDC _hHdc , TSD_Rslt * _pRslt , TSD_Disp _tDisp , float _fScaleX=1.0 , float _fScaleY=1.0);
};
//---------------------------------------------------------------------------
#endif