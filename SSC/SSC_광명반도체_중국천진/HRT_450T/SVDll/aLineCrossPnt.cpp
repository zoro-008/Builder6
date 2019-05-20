
//---------------------------------------------------------------------------
#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------
#include "aLineCrossPnt.h"
#include "SVMaths.h"
#include "GdiUnit.h"
#include "Common.h"

CLineCrossPnt::CLineCrossPnt()
{
}

CLineCrossPnt::~CLineCrossPnt()
{

}

bool CLineCrossPnt::Inspect(CImage * _pImg  , TRect _tRect1 , TRect _tRect2 , LCP_Para _tPara , LCP_Rslt * _pRslt)
{
    DWORD dwTime =GetTickTimeDouble();
    g_sErrMsg = "";
    memset(_pRslt , 0 , sizeof(*_pRslt));

    //Para Check & Err Check
    if(_pImg == NULL             ) {g_sErrMsg = "Image Buffer is NULL."    ; return false ; }
    if(_pImg -> SetRect(&_tRect1)) {g_sErrMsg = "Rect1 Overed Image Size." ; return false ; }
    if(_pImg -> SetRect(&_tRect2)) {g_sErrMsg = "Rect2 Overed Image Size." ; return false ; }

    if(!Line1.Inspect(_pImg  , _tRect1 , _tPara.LinPara1 , &_pRslt -> LinRslt1)) return false ;
    if(!Line2.Inspect(_pImg  , _tRect2 , _tPara.LinPara2 , &_pRslt -> LinRslt2)) return false ;

    if(!MATH_GetLineCrossPnt(_pRslt->LinRslt1.StartPnt , _pRslt->LinRslt1.EndPnt , _pRslt->LinRslt2.StartPnt , _pRslt->LinRslt2.EndPnt , _pRslt->CrossPnt)){g_sErrMsg = "Lines are parallel"; return false ; }

    _pRslt->fInspTime = MATH_RoundOff(GetTickTimeDouble()- dwTime , 2) ;

    return true;

}

void CLineCrossPnt::PaintRslt(HDC _hHdc , LCP_Rslt * _pRslt , LCP_Disp _tDisp , float _fScaleX, float _fScaleY)
{
    CGdi *Gdi = new CGdi(_hHdc);
    AnsiString sTemp ;
    Gdi -> SetScale(_fScaleX , _fScaleY);

    Line1.PaintRslt(_hHdc , &_pRslt->LinRslt1 , _tDisp.LinDisp1 , _fScaleX, _fScaleY) ;
    Line2.PaintRslt(_hHdc , &_pRslt->LinRslt2 , _tDisp.LinDisp2 , _fScaleX, _fScaleY) ;

    if(_tDisp.clPoint != clNone) {
        Gdi->m_tPen.Color = _tDisp.clPoint ;
        Gdi->LineX((int)_pRslt->CrossPnt.x ,(int)_pRslt->CrossPnt.y , 5) ;
    }
    if(_tDisp.clText != clNone) {
        Gdi->m_tText.Color = _tDisp.clText ;
        sTemp = sTemp.sprintf("X:%.3f Y:%.3f",_pRslt->CrossPnt.x , _pRslt->CrossPnt.y) ;
        Gdi->Text((int)_pRslt->CrossPnt.x ,(int)_pRslt->CrossPnt.y , sTemp.c_str()) ;
    }


    delete Gdi;
}

