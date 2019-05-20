
//---------------------------------------------------------------------------
#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------
#include "aThreshold.h"
#include "SVMaths.h"
#include "GdiUnit.h"
#include "Common.h"

CThreshold::CThreshold()
{
}

CThreshold::~CThreshold()
{

}

bool CThreshold::Inspect(CImage * _pImg  , TRect _tRect , TSD_Para _tPara , TSD_Rslt * _pRslt )
{
    DWORD dwTime =GetTickTimeDouble();
    g_sErrMsg = "";
    memset(_pRslt , 0 , sizeof(*_pRslt));

    //Para Check & Err Check
    if(_pImg == NULL            ) {g_sErrMsg = "Image Buffer is NULL."    ; return false ; }
    if(_pImg -> SetRect(&_tRect)) {g_sErrMsg = "Rect  Overed Image Size." ; return false ; }

    //에리어 넣을려다 좀 이상할듯 해서 않넣음.
    //if(_pRsltAra == NULL        ) {g_sErrMsg = "Area Buffer is NULL."     ; return false ; }
    ////에리아 사이즈 않맞으면 맞춘다.
    //if(_pRsltAra -> GetWidth()  != _tRect.Width()  ||
    //   _pRsltAra -> GetHeight() != _tRect.Height() ){
    //    _pRsltAra -> SetSize(_tRect.Width(),_tRect.Height());
    //}


    int iSum = 0 ;
    int cPx ;

    int iSumCntrX = 0;
    int iSumCntrY = 0;

    float iCntrX ;
    float iCntrY ;

    for(int x = _tRect.left ; x < _tRect.right ; x++) {
        for(int y = _tRect.top ; y < _tRect.bottom ; y++) {
            cPx = _pImg -> GetPixel(x,y);
            if(cPx <= _tPara.iThresholdHigh && cPx > _tPara.iThresholdLow) {
                iSumCntrX += x ;
                iSumCntrY += y ;

                _pRslt -> iPxCnt ++ ;
            }
            iSum += cPx ;
        }
    }

    _pRslt -> fCntrX = _pRslt -> iPxCnt ? iSumCntrX / _pRslt -> iPxCnt : _tRect.Left + _tRect.Width ()/2.0 ;
    _pRslt -> fCntrY = _pRslt -> iPxCnt ? iSumCntrY / _pRslt -> iPxCnt : _tRect.Top  + _tRect.Height()/2.0 ;

    _pRslt -> tRect = _tRect ;

    _pRslt -> fAverage = (_tRect.Width() * _tRect.Height()) ? iSum / (float)(_tRect.Width() * _tRect.Height()) : 0 ;


    _pRslt->fInspTime = MATH_RoundOff(GetTickTimeDouble()- dwTime , 2) ;

    return true;

}

void CThreshold::PaintRslt(HDC _hHdc , TSD_Rslt * _pRslt , TSD_Disp _tDisp , float _fScaleX , float _fScaleY)
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

