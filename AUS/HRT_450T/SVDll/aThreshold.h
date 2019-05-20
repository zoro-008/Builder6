//---------------------------------------------------------------------------
#ifndef aThresholdH
#define aThresholdH
//---------------------------------------------------------------------------
#include "SVInspStruct.h"
#include "Image.h"
//#include "Area.h"

//
//

//---------------------------------------------------------------------------
// 2009.01.02
// SUN Kye Won
//---------------------------------------------------------------------------
class CThreshold {
    private :
    protected :
    public :
        //Functions
        CThreshold();
        ~CThreshold();

    public  :
        bool Inspect  (CImage * _pImg  , TRect _tRect , TSD_Para _tPara , TSD_Rslt * _pRslt );
        void PaintRslt(HDC _hHdc , TSD_Rslt * _pRslt , TSD_Disp _tDisp , float _fScaleX=1.0 , float _fScaleY=1.0);
};
//---------------------------------------------------------------------------
#endif