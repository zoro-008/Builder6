//---------------------------------------------------------------------------
#ifndef aLineH
#define aLineH
//---------------------------------------------------------------------------
#include "SVInspStruct.h"
#include "Image.h"
#include "aEdge.h"

//
//

//---------------------------------------------------------------------------
// 2009.01.02
// SUN Kye Won
//---------------------------------------------------------------------------
class CLine {
    private :
    protected :
        CEdge Edge ;
        CLinkedList<TFPoint> EdgList ;

    public :
        //Functions
        CLine();
        ~CLine();

    public  :
        bool Inspect  (CImage * _pImg  , TRect _tRect , LIN_Para _tPara , LIN_Rslt * _pRslt);
        void PaintRslt(HDC _hHdc , LIN_Rslt * _pRslt , LIN_Disp _tDisp , float _fScaleX=1.0 , float _fScaleY=1.0);
};
//---------------------------------------------------------------------------
#endif