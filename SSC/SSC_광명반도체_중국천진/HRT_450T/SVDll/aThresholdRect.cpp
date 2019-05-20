
//---------------------------------------------------------------------------
#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------
#include "aThresholdRect.h"
#include "SVMaths.h"
#include "GdiUnit.h"
#include "Common.h"

CThresholdRect::CThresholdRect()
{
}

CThresholdRect::~CThresholdRect()
{

}

bool CThresholdRect::Inspect(CImage * _pImg  , TPoint _tCntPnt , TSD_Para _tPara , TSD_Rslt * _pRslt)
{
    DWORD dwTime =GetTickTimeDouble();
    g_sErrMsg = "";
    memset(_pRslt , 0 , sizeof(*_pRslt));

    //Para Check & Err Check
    if(_pImg == NULL            ) {g_sErrMsg = "Image Buffer is NULL."    ; return false ; }
    if(_pImg -> SetRect(&_tRect)) {g_sErrMsg = "RectL Overed Image Size." ; return false ; }

    int iSum = 0.0 ;
    BYTE cPx ;

    for(int x = _tRect.left ; x < _tRect.right ; x++) {
        for(int y = _tRect.top ; y < _tRect.bottom ; y++) {
            cPx = _pImg -> GetPixel(x,y);
            if(cPx < _tPara.iThresholdHigh && cPx > _tPara.iThresholdLow) _pRslt -> iPxCnt ++ ;
            iSum += cPx ;
        }
    }

    _pRslt -> tRect = _tRect ;

    _pRslt -> fAverage = iSum / (float)(_tRect.Width() * _tRect.Height()) ;


    _pRslt->fInspTime = MATH_RoundOff(GetTickTimeDouble()- dwTime , 2) ;

    return true;

}

void CThresholdRect::PaintRslt(HDC _hHdc , TSD_Rslt * _pRslt , TSD_Disp _tDisp , float _fScaleX , float _fScaleY)
{
    CGdi *Gdi = new CGdi(_hHdc);
    AnsiString sTemp ;
    Gdi -> SetScale(_fScaleX , _fScaleY);

    if(_tDisp.clRect != clNone) {
        Gdi->m_tPen.Color = _tDisp.clRect ;

        Gdi->Rect(false, _pRslt -> tRect);


    }
    if(_tDisp.clText != clNone) {
        Gdi->m_tText.Color = _tDisp.clText ;

        sTemp = "Px Cnt:" + AnsiString(_pRslt -> iPxCnt) ;
        Gdi->Text((int)_pRslt->tRect.left ,(int)_pRslt->tRect.bottom + 5  , sTemp.c_str()) ;

        sTemp = "Average:" + AnsiString(MATH_RoundOff(_pRslt -> fAverage , 2)) ;
        Gdi->Text((int)_pRslt->tRect.left ,(int)_pRslt->tRect.bottom + 15 , sTemp.c_str()) ;
    }
    if(_tDisp.clDark != clNone) { //이미지가 없네 ;;
        //for(int y = _pRslt -> tRect.top ; y < _pRslt -> tRect.bottom ; y++) {
        //    for(int x = _pRslt -> tRect.left ; x < _pRslt -> tRect.right ; x++) {
        //        if(   Gdi -> Pixel()
        //
        //    }
        //}
    }


    delete Gdi;
}

