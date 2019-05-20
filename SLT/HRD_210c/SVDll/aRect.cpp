
//---------------------------------------------------------------------------
#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------
#include "aRect.h"
#include "SVMaths.h"
#include "GdiUnit.h"
#include "Common.h"

CRect::CRect()
{
}

CRect::~CRect()
{

}

bool CRect::Inspect(CImage * _pImg  , TRect _tRectL , TRect _tRectT , TRect _tRectR , TRect _tRectB , RCT_Para _tPara , RCT_Rslt * _pRslt)
{
    DWORD dwTime =GetTickTimeDouble();
    g_sErrMsg = "";
    memset(_pRslt , 0 , sizeof(*_pRslt));

    float fAngleSum = 0.0;
    float fTemp ;

    //Para Check & Err Check
    if(_pImg == NULL             ) {g_sErrMsg = "Image Buffer is NULL."    ; return false ; }
    if(_pImg -> SetRect(&_tRectL)) {g_sErrMsg = "RectL Overed Image Size." ; return false ; }
    if(_pImg -> SetRect(&_tRectT)) {g_sErrMsg = "RectT Overed Image Size." ; return false ; }
    if(_pImg -> SetRect(&_tRectR)) {g_sErrMsg = "RectR Overed Image Size." ; return false ; }
    if(_pImg -> SetRect(&_tRectB)) {g_sErrMsg = "RectB Overed Image Size." ; return false ; }

    _tPara.LinParaL.bLtToDk = !_tPara.bLtOnDk ;
    _tPara.LinParaT.bLtToDk = !_tPara.bLtOnDk ;
    _tPara.LinParaR.bLtToDk = !_tPara.bLtOnDk ;
    _tPara.LinParaB.bLtToDk = !_tPara.bLtOnDk ;

    _tPara.LinParaL.iScanDirect = lsLtToRt ;
    _tPara.LinParaT.iScanDirect = lsUpToDn ;
    _tPara.LinParaR.iScanDirect = lsRtToLt ;
    _tPara.LinParaB.iScanDirect = lsDnToUp ;


    if(!LineL.Inspect(_pImg  , _tRectL , _tPara.LinParaL , &_pRslt -> LinRsltL)) return false ;
    if(!LineT.Inspect(_pImg  , _tRectT , _tPara.LinParaT , &_pRslt -> LinRsltT)) return false ;
    if(!LineR.Inspect(_pImg  , _tRectR , _tPara.LinParaR , &_pRslt -> LinRsltR)) return false ;
    if(!LineB.Inspect(_pImg  , _tRectB , _tPara.LinParaB , &_pRslt -> LinRsltB)) return false ;


    if(!MATH_GetLineCrossPnt(_pRslt->LinRsltL.StartPnt , _pRslt->LinRsltL.EndPnt , _pRslt->LinRsltT.StartPnt , _pRslt->LinRsltT.EndPnt , _pRslt->CrossPntLT)){g_sErrMsg = "LT Lines are parallel"; return false ; }
    if(!MATH_GetLineCrossPnt(_pRslt->LinRsltT.StartPnt , _pRslt->LinRsltT.EndPnt , _pRslt->LinRsltR.StartPnt , _pRslt->LinRsltR.EndPnt , _pRslt->CrossPntRT)){g_sErrMsg = "RT Lines are parallel"; return false ; }
    if(!MATH_GetLineCrossPnt(_pRslt->LinRsltR.StartPnt , _pRslt->LinRsltR.EndPnt , _pRslt->LinRsltB.StartPnt , _pRslt->LinRsltB.EndPnt , _pRslt->CrossPntRB)){g_sErrMsg = "RB Lines are parallel"; return false ; }
    if(!MATH_GetLineCrossPnt(_pRslt->LinRsltB.StartPnt , _pRslt->LinRsltB.EndPnt , _pRslt->LinRsltL.StartPnt , _pRslt->LinRsltL.EndPnt , _pRslt->CrossPntLB)){g_sErrMsg = "LB Lines are parallel"; return false ; }

    _pRslt -> CenterPnt.x = (_pRslt->CrossPntLT.x + _pRslt->CrossPntRT.x + _pRslt->CrossPntRB.x + _pRslt->CrossPntLB.x) / 4.0 ;
    _pRslt -> CenterPnt.y = (_pRslt->CrossPntLT.y + _pRslt->CrossPntRT.y + _pRslt->CrossPntRB.y + _pRslt->CrossPntLB.y) / 4.0 ;



    //오프셑값에서 트레숄드 확인 하기.
    float a , b , x , y , px , sum;

    _pRslt -> OffsetPntLT.x = _pRslt->CrossPntLT.x + _tPara.iChkOfs ;
    _pRslt -> OffsetPntLT.y = _pRslt->CrossPntLT.y + _tPara.iChkOfs ;

    _pRslt -> OffsetPntRT.x = _pRslt->CrossPntRT.x - _tPara.iChkOfs ;
    _pRslt -> OffsetPntRT.y = _pRslt->CrossPntRT.y + _tPara.iChkOfs ;

    _pRslt -> OffsetPntLB.x = _pRslt->CrossPntLB.x + _tPara.iChkOfs ;
    _pRslt -> OffsetPntLB.y = _pRslt->CrossPntLB.y - _tPara.iChkOfs ;

    _pRslt -> OffsetPntRB.x = _pRslt->CrossPntRB.x - _tPara.iChkOfs ;
    _pRslt -> OffsetPntRB.y = _pRslt->CrossPntRB.y - _tPara.iChkOfs ;

    //왼쪽변.
    a = MATH_GetLineA(_pRslt->OffsetPntLT , _pRslt->OffsetPntLB );
    b = MATH_GetLineB(_pRslt->OffsetPntLT , _pRslt->OffsetPntLB );
    for(int y = _pRslt->OffsetPntLT.y ; y < _pRslt->OffsetPntLB.y ; y++){
        x  = MATH_GetLineX(y , a ,b ) ;
        px = _pImg -> GetSubPixel(x,y) ;

        _pRslt->iTtlChkPxCnt++;
        sum += px ;
        if( _tPara.iLowThsd > px || px > _tPara.iHighThsd) {
            _pRslt->iChkPxOutCnt++;
        }
    }

    //윗변.
    a = MATH_GetLineA(_pRslt->OffsetPntLT , _pRslt->OffsetPntRT );
    b = MATH_GetLineB(_pRslt->OffsetPntLT , _pRslt->OffsetPntRT );
    for(int x = _pRslt->OffsetPntLT.x ; x < _pRslt->OffsetPntRT.x ; x++){
        y  = MATH_GetLineY(a,x ,b) ;
        px = _pImg -> GetSubPixel(x,y) ;

        _pRslt->iTtlChkPxCnt++;
        sum += px ;
        if( _tPara.iLowThsd > px || px > _tPara.iHighThsd) {
            _pRslt->iChkPxOutCnt++;
        }
    }

    //오른쪽변.
    a = MATH_GetLineA(_pRslt->OffsetPntRT , _pRslt->OffsetPntRB );
    b = MATH_GetLineB(_pRslt->OffsetPntRT , _pRslt->OffsetPntRB );
    for(int y = _pRslt->OffsetPntRT.y ; y < _pRslt->OffsetPntRB.y ; y++){
        x  = MATH_GetLineX(y , a ,b );
        px = _pImg -> GetSubPixel(x,y) ;

        _pRslt->iTtlChkPxCnt++;
        sum += px ;
        if( _tPara.iLowThsd > px || px > _tPara.iHighThsd) {
            _pRslt->iChkPxOutCnt++;
        }
    }

    //아래변.
    a = MATH_GetLineA(_pRslt->OffsetPntLB , _pRslt->OffsetPntRB );
    b = MATH_GetLineB(_pRslt->OffsetPntLB , _pRslt->OffsetPntRB );
    for(int x = _pRslt->OffsetPntLB.x ; x < _pRslt->OffsetPntRB.x ; x++){
        y  = MATH_GetLineY(a,x ,b);
        px = _pImg -> GetSubPixel(x,y) ;

        _pRslt->iTtlChkPxCnt++;
        sum += px ;
        if( _tPara.iLowThsd > px || px > _tPara.iHighThsd) {
            _pRslt->iChkPxOutCnt++;
        }
    }

    fTemp = MATH_GetLineAngle(_pRslt->LinRsltL.StartPnt.x , _pRslt->LinRsltL.StartPnt.y , _pRslt->LinRsltL.EndPnt.x   , _pRslt->LinRsltL.EndPnt.y  ) - 270 ; fAngleSum += fTemp ;
    fTemp = MATH_GetLineAngle(_pRslt->LinRsltT.EndPnt.x   , _pRslt->LinRsltT.EndPnt.y   , _pRslt->LinRsltT.StartPnt.x , _pRslt->LinRsltT.StartPnt.y) - 180 ; fAngleSum += fTemp ;
    fTemp = MATH_GetLineAngle(_pRslt->LinRsltR.StartPnt.x , _pRslt->LinRsltR.StartPnt.y , _pRslt->LinRsltR.EndPnt.x   , _pRslt->LinRsltR.EndPnt.y  ) - 270 ; fAngleSum += fTemp ;
    fTemp = MATH_GetLineAngle(_pRslt->LinRsltB.EndPnt.x   , _pRslt->LinRsltB.EndPnt.y   , _pRslt->LinRsltB.StartPnt.x , _pRslt->LinRsltB.StartPnt.y) - 180 ; fAngleSum += fTemp ;

    _pRslt -> fAngle = fAngleSum / 4.0 ;

    _pRslt->fInspTime = MATH_RoundOff(GetTickTimeDouble()- dwTime , 2) ;

    return true;

}

void CRect::PaintRslt(HDC _hHdc , RCT_Rslt * _pRslt , RCT_Disp _tDisp , float _fScaleX , float _fScaleY)
{
    CGdi *Gdi = new CGdi(_hHdc);
    AnsiString sTemp ;
    Gdi -> SetScale(_fScaleX , _fScaleY);

    if(_tDisp.clRect != clNone) {
        Gdi->m_tPen.Color = _tDisp.clRect ;
        Gdi->m_tPen.Style = PS_SOLID ;
        Gdi->Line((int)_pRslt -> CrossPntLT.x , (int)_pRslt -> CrossPntLT.y ,(int)_pRslt -> CrossPntRT.x , (int)_pRslt -> CrossPntRT.y);
        Gdi->Line((int)_pRslt -> CrossPntRT.x , (int)_pRslt -> CrossPntRT.y ,(int)_pRslt -> CrossPntRB.x , (int)_pRslt -> CrossPntRB.y);
        Gdi->Line((int)_pRslt -> CrossPntRB.x , (int)_pRslt -> CrossPntRB.y ,(int)_pRslt -> CrossPntLB.x , (int)_pRslt -> CrossPntLB.y);
        Gdi->Line((int)_pRslt -> CrossPntLB.x , (int)_pRslt -> CrossPntLB.y ,(int)_pRslt -> CrossPntLT.x , (int)_pRslt -> CrossPntLT.y);

        Gdi->m_tPen.Style = PS_DOT ;
        Gdi->Line((int)_pRslt -> OffsetPntLT.x , (int)_pRslt -> OffsetPntLT.y ,(int)_pRslt -> OffsetPntRT.x , (int)_pRslt -> OffsetPntRT.y);
        Gdi->Line((int)_pRslt -> OffsetPntRT.x , (int)_pRslt -> OffsetPntRT.y ,(int)_pRslt -> OffsetPntRB.x , (int)_pRslt -> OffsetPntRB.y);
        Gdi->Line((int)_pRslt -> OffsetPntRB.x , (int)_pRslt -> OffsetPntRB.y ,(int)_pRslt -> OffsetPntLB.x , (int)_pRslt -> OffsetPntLB.y);
        Gdi->Line((int)_pRslt -> OffsetPntLB.x , (int)_pRslt -> OffsetPntLB.y ,(int)_pRslt -> OffsetPntLT.x , (int)_pRslt -> OffsetPntLT.y);

        Gdi->m_tPen.Style = PS_SOLID ;

    }

    LineL.PaintRslt(_hHdc , &_pRslt->LinRsltL , _tDisp.LinDisp , _fScaleX, _fScaleY) ;
    LineT.PaintRslt(_hHdc , &_pRslt->LinRsltT , _tDisp.LinDisp , _fScaleX, _fScaleY) ;
    LineR.PaintRslt(_hHdc , &_pRslt->LinRsltR , _tDisp.LinDisp , _fScaleX, _fScaleY) ;
    LineB.PaintRslt(_hHdc , &_pRslt->LinRsltB , _tDisp.LinDisp , _fScaleX, _fScaleY) ;





    if(_tDisp.clPoint != clNone) {
        Gdi->m_tPen.Color = _tDisp.clPoint ;
        Gdi->LineX((int)_pRslt->CenterPnt.x ,(int)_pRslt->CenterPnt.y , 5) ;

        Gdi->LineX((int)_pRslt->CrossPntLT.x ,(int)_pRslt->CrossPntLT.y , 5) ;
        Gdi->LineX((int)_pRslt->CrossPntRT.x ,(int)_pRslt->CrossPntRT.y , 5) ;
        Gdi->LineX((int)_pRslt->CrossPntRB.x ,(int)_pRslt->CrossPntRB.y , 5) ;
        Gdi->LineX((int)_pRslt->CrossPntLB.x ,(int)_pRslt->CrossPntLB.y , 5) ;


    }
    if(_tDisp.clText != clNone) {
        Gdi->m_tText.Color = _tDisp.clText ;
        sTemp = sTemp.sprintf("X:%.3f Y:%.3f",_pRslt->CenterPnt.x , _pRslt->CenterPnt.y) ;
        Gdi->Text((int)_pRslt->CenterPnt.x + 5 ,(int)_pRslt->CenterPnt.y + 5 , sTemp.c_str()) ;
        sTemp = sTemp.sprintf("Angle:%.3f",_pRslt->fAngle) ;
        Gdi->Text((int)_pRslt->CenterPnt.x + 5 ,(int)_pRslt->CenterPnt.y + 15 , sTemp.c_str()) ;
        sTemp = sTemp.sprintf("OutPxCnt:%d",_pRslt->iChkPxOutCnt) ;
        Gdi->Text((int)_pRslt->CenterPnt.x + 5 ,(int)_pRslt->CenterPnt.y + 25 , sTemp.c_str()) ;
    }


    delete Gdi;
}














//웨이퍼 칩을 위한 렉트
CWfrRect::CWfrRect()
{
}

CWfrRect::~CWfrRect()
{
}

bool CWfrRect::Inspect(CImage * _pImg  , TRect _tRectL , TRect _tRectT , TRect _tRectR , TRect _tRectB , WRT_Para _tPara , WRT_Rslt * _pRslt)
{
    DWORD dwTime =GetTickTimeDouble();
    g_sErrMsg = "";
    memset(_pRslt , 0 , sizeof(*_pRslt));

    float fAngleSum = 0.0;
    float fTemp ;

    //Para Check & Err Check
    if(_pImg == NULL             ) {g_sErrMsg = "Image Buffer is NULL."    ; return false ; }
    if(_pImg -> SetRect(&_tRectL)) {g_sErrMsg = "RectL Overed Image Size." ; return false ; }
    if(_pImg -> SetRect(&_tRectT)) {g_sErrMsg = "RectT Overed Image Size." ; return false ; }
    if(_pImg -> SetRect(&_tRectR)) {g_sErrMsg = "RectR Overed Image Size." ; return false ; }
    if(_pImg -> SetRect(&_tRectB)) {g_sErrMsg = "RectB Overed Image Size." ; return false ; }




    TRect RectLT ; RectLT.left = _tRectL.left ; RectLT.top = _tRectT.top ; RectLT.right = _tRectL.right ; RectLT.bottom = _tRectT.bottom ;
    TRect RectRT ; RectRT.left = _tRectR.left ; RectRT.top = _tRectT.top ; RectRT.right = _tRectR.right ; RectRT.bottom = _tRectT.bottom ;
    TRect RectRB ; RectRB.left = _tRectR.left ; RectRB.top = _tRectB.top ; RectRB.right = _tRectR.right ; RectRB.bottom = _tRectB.bottom ;
    TRect RectLB ; RectLB.left = _tRectL.left ; RectLB.top = _tRectB.top ; RectLB.right = _tRectL.right ; RectLB.bottom = _tRectB.bottom ;

    int iScanLenth = _tRectL.Width() ;
    int iCrntScanLength = 0 ;
    unsigned char cPx ; //커런트픽셀
    bool bCrtPxIn ; //현제 픽셀인.
    const int iMargin = 10 ;
    const int iYMargin = 1 ;
    int iYMarginCnt = 0 ;
    bool bYMarginXBreak = false ;

    int iTemp ;

    //LeftTop ROI찾기.
    //왼쪽에서 오른쪽으로 찔러 본다. 가장 왼쪽에 있는 놈 찾음.
    int iLTFindX = 0 ;
    int iLTFindY = 0 ;
    iYMarginCnt = 0 ;
    bYMarginXBreak = false ;
    for(int y = RectLT.top ; y < RectLT.bottom ; y++){
        iCrntScanLength = 0 ;
        for(int x = RectLT.left ; x < RectLT.right + iScanLenth ; x++){
            cPx = _pImg -> GetPixel(x,y);
            if(_tPara.iLowThsd <= cPx && cPx <= _tPara.iHighThsd) {
                iCrntScanLength++;
            }
            else {
                iCrntScanLength = 0 ;
            }
            if(iCrntScanLength >= iScanLenth) {
                for(int yy = y ; yy < y + iScanLenth ; yy++) {
                    cPx = _pImg -> GetPixel(x - iScanLenth,yy);
                    if(_tPara.iLowThsd > cPx || cPx > _tPara.iHighThsd) break ;
                    if(yy == y + iScanLenth - 1) {
                        if(iYMarginCnt >= iYMargin) { //너무 밀착하면 X쪽 오프셑 난다.
                            iLTFindX = x - iScanLenth;
                            iLTFindY = y ;
                            break ;
                        }
                        iYMarginCnt++;
                        bYMarginXBreak = true ;

                    }
                }

                iTemp = 0 ;

            }
            if(iLTFindX != 0  || iLTFindY != 0 || bYMarginXBreak) {
                bYMarginXBreak = false ;
                break ; //마진 있으면 X찾는것 중단 하고 Y 올림.
            }
        }
        if(iLTFindX != 0  || iLTFindY != 0) break ;
    }
    if(iLTFindX == 0  && iLTFindY == 0){ //디폴트 세팅.
        iLTFindX = RectLT.left ;
        iLTFindY = RectLT.top  ;
    }
    else {
        RectLT.left = iLTFindX -iMargin ;
        RectLT.top  = iLTFindY -iMargin ;
    }


    //LeftBottom ROI찾기.
    //왼쪽에서 오른쪽으로 찔러 본다. 가장 왼쪽에 있는 놈 찾음.
    int iLBFindX = 0 ;
    int iLBFindY = 0 ;
    iYMarginCnt = 0 ;
    bYMarginXBreak = false ;
    for(int y = RectLB.bottom - 1 ; y >= RectLB.top ; y--){
        iCrntScanLength = 0 ;
        for(int x = RectLB.left ; x < RectLB.right + iScanLenth ; x++){
            cPx = _pImg -> GetPixel(x,y);
            if(_tPara.iLowThsd <= cPx && cPx <= _tPara.iHighThsd) {
                iCrntScanLength++;
            }
            else {
                iCrntScanLength = 0 ;
            }
            if(iCrntScanLength >= iScanLenth) {
                for(int yy = y - 1 ; yy >= y - iScanLenth ; yy--) {
                    cPx = _pImg -> GetPixel(x - iScanLenth ,yy);
                    if(_tPara.iLowThsd > cPx || cPx > _tPara.iHighThsd) break ;
                    if(yy == y - iScanLenth) {
                        if(iYMarginCnt >= iYMargin) { //너무 밀착하면 X쪽 오프셑 난다.
                            iLBFindX = x - iScanLenth;
                            iLBFindY = y ;
                            break ;
                        }
                        iYMarginCnt++;
                        bYMarginXBreak = true ;
                    }
                }
            }
            if(iLBFindX != 0  || iLBFindY != 0 || bYMarginXBreak) {
                bYMarginXBreak = false ;
                break ;
            }
        }
        if(iLBFindX != 0  || iLBFindY != 0) break ;
    }
    if(iLBFindX == 0  && iLBFindY == 0){ //디폴트 세팅.
        iLBFindX = RectLB.left   ;
        iLBFindY = RectLB.bottom ;
    }
    else {
        RectLB.left   = iLBFindX - iMargin ;
        RectLB.bottom = iLBFindY + iMargin ;
    }

    //RightTop ROI찾기.
    //왼쪽에서 오른쪽으로 찔러 본다. 가장 왼쪽에 있는 놈 찾음.
    int iRTFindX = 0 ;
    int iRTFindY = 0 ;
    iYMarginCnt = 0 ;
    bYMarginXBreak = false ;
    for(int y = RectRT.top ; y < RectRT.bottom ; y++){
        iCrntScanLength = 0 ;
        for(int x = RectRT.right - 1 ; x >= RectRT.left - iScanLenth  ; x--){
            cPx = _pImg -> GetPixel(x,y);
            if(_tPara.iLowThsd <= cPx && cPx <= _tPara.iHighThsd) {
                iCrntScanLength++;
            }
            else {
                iCrntScanLength = 0 ;
            }
            if(iCrntScanLength >= iScanLenth) {
                for(int yy = y ; yy < y + iScanLenth ; yy++) {
                    cPx = _pImg -> GetPixel(x + iScanLenth ,yy);
                    if(_tPara.iLowThsd > cPx || cPx > _tPara.iHighThsd) break ;
                    if(yy == y + iScanLenth - 1) {
                        if(iYMarginCnt >= iYMargin) { //너무 밀착하면 X쪽 오프셑 난다.
                            iRTFindX = x + iScanLenth;
                            iRTFindY = y ;
                            break ;
                        }
                        iYMarginCnt++;
                        bYMarginXBreak = true ;
                    }
                }
            }
            if(iRTFindX != 0  || iRTFindY != 0 || bYMarginXBreak) {
                bYMarginXBreak = false ;
                break ;
            }
        }
        if(iRTFindX != 0  || iRTFindY != 0) break ;
    }
    if(iRTFindX == 0  && iRTFindY == 0){ //디폴트 세팅.
        iRTFindX = RectRT.right;
        iRTFindY = RectRT.top  ;
    }
    else { //디폴트 세팅.
        RectRT.right = iRTFindX + iMargin ;
        RectRT.top   = iRTFindY - iMargin ;
    }

    //RightBottom ROI찾기.
    //왼쪽에서 오른쪽으로 찔러 본다. 가장 왼쪽에 있는 놈 찾음.
    int iRBFindX = 0 ;
    int iRBFindY = 0 ;
    iYMarginCnt = 0 ;
    bYMarginXBreak = false ;
    for(int y = RectRB.bottom - 1 ; y >= RectRB.top  ; y--){
        iCrntScanLength = 0 ;
        for(int x = RectRB.right - 1 ; x >= RectRB.left - iScanLenth  ; x--){
            cPx = _pImg -> GetPixel(x,y);
            if(_tPara.iLowThsd <= cPx && cPx <= _tPara.iHighThsd) {
                iCrntScanLength++;
            }
            else {
                iCrntScanLength = 0 ;
            }
            if(iCrntScanLength >= iScanLenth) {
                for(int yy = y - 1 ;  y - iScanLenth <= yy ; yy--) {
                    cPx = _pImg -> GetPixel(x+ iScanLenth,yy);
                    if(_tPara.iLowThsd > cPx || cPx > _tPara.iHighThsd) break ;
                    if(yy == y - iScanLenth ) {
                        if(iYMarginCnt >= iYMargin) { //너무 밀착하면 X쪽 오프셑 난다.
                            iRBFindX = x + iScanLenth;
                            iRBFindY = y ;
                            break ;
                        }
                        iYMarginCnt++;
                        bYMarginXBreak = true ;
                    }
                }
            }
            if(iRBFindX != 0  || iRBFindY != 0 || bYMarginXBreak) {
                bYMarginXBreak = false ;
                break ;
            }
        }
        if(iRBFindX != 0  || iRBFindY != 0) break ;
    }
    if(iRBFindX == 0  && iRBFindY == 0){ //디폴트 세팅.
        iRBFindX = RectRB.right;
        iRBFindY = RectRB.top  ;
    }
    else { //디폴트 세팅.
        RectRB.right  = iRBFindX + iMargin ;
        RectRB.bottom = iRBFindY + iMargin ;
    }


    _pRslt -> LTRect = RectLT ;
    _pRslt -> RTRect = RectRT ;
    _pRslt -> RBRect = RectRB ;
    _pRslt -> LBRect = RectLB ;





    _tRectL.left   = RectLT.left   < RectLB.left   ? RectLT.left   : RectLB.left   ;
    _tRectL.top    = RectLT.top    < RectRT.Top    ? RectLT.top    : RectRT.Top    ;
    _tRectL.bottom = RectLB.bottom > RectRB.bottom ? RectLB.bottom : RectRB.bottom ;

    _tRectT.left   = RectLT.left   < RectLB.left   ? RectLT.left   : RectLB.left   ;
    _tRectT.right  = RectRT.right  > RectRB.right  ? RectRT.right  : RectRB.right  ;
    _tRectT.top    = RectLT.top    < RectRT.top    ? RectLT.top    : RectRT.Top    ;

    _tRectR.top    = RectLT.top    < RectRT.Top    ? RectLT.top    : RectRT.Top    ;
    _tRectR.right  = RectRT.right  > RectRB.right  ? RectRT.right  : RectRB.right  ;
    _tRectR.bottom = RectLB.bottom > RectRB.bottom ? RectLB.bottom : RectRB.bottom ;

    _tRectB.left   = RectLB.left   < RectLT.left   ? RectLB.left   : RectLT.left   ;
    _tRectB.right  = RectRT.right  > RectRB.right  ? RectRT.right  : RectRB.right  ;
    _tRectB.bottom = RectLB.bottom > RectRB.bottom ? RectLB.bottom : RectRB.bottom ;


    _tPara.LinParaL.bLtToDk = !_tPara.bLtOnDk ;
    _tPara.LinParaT.bLtToDk = !_tPara.bLtOnDk ;
    _tPara.LinParaR.bLtToDk = !_tPara.bLtOnDk ;
    _tPara.LinParaB.bLtToDk = !_tPara.bLtOnDk ;

    _tPara.LinParaL.iScanDirect = lsLtToRt ;
    _tPara.LinParaT.iScanDirect = lsUpToDn ;
    _tPara.LinParaR.iScanDirect = lsRtToLt ;
    _tPara.LinParaB.iScanDirect = lsDnToUp ;


    if(!LineL.Inspect(_pImg  , _tRectL , _tPara.LinParaL , &_pRslt -> LinRsltL)) return false ;
    if(!LineT.Inspect(_pImg  , _tRectT , _tPara.LinParaT , &_pRslt -> LinRsltT)) return false ;
    if(!LineR.Inspect(_pImg  , _tRectR , _tPara.LinParaR , &_pRslt -> LinRsltR)) return false ;
    if(!LineB.Inspect(_pImg  , _tRectB , _tPara.LinParaB , &_pRslt -> LinRsltB)) return false ;


    if(!MATH_GetLineCrossPnt(_pRslt->LinRsltL.StartPnt , _pRslt->LinRsltL.EndPnt , _pRslt->LinRsltT.StartPnt , _pRslt->LinRsltT.EndPnt , _pRslt->CrossPntLT)){g_sErrMsg = "LT Lines are parallel"; return false ; }
    if(!MATH_GetLineCrossPnt(_pRslt->LinRsltT.StartPnt , _pRslt->LinRsltT.EndPnt , _pRslt->LinRsltR.StartPnt , _pRslt->LinRsltR.EndPnt , _pRslt->CrossPntRT)){g_sErrMsg = "RT Lines are parallel"; return false ; }
    if(!MATH_GetLineCrossPnt(_pRslt->LinRsltR.StartPnt , _pRslt->LinRsltR.EndPnt , _pRslt->LinRsltB.StartPnt , _pRslt->LinRsltB.EndPnt , _pRslt->CrossPntRB)){g_sErrMsg = "RB Lines are parallel"; return false ; }
    if(!MATH_GetLineCrossPnt(_pRslt->LinRsltB.StartPnt , _pRslt->LinRsltB.EndPnt , _pRslt->LinRsltL.StartPnt , _pRslt->LinRsltL.EndPnt , _pRslt->CrossPntLB)){g_sErrMsg = "LB Lines are parallel"; return false ; }

    _pRslt -> CenterPnt.x = (_pRslt->CrossPntLT.x + _pRslt->CrossPntRT.x + _pRslt->CrossPntRB.x + _pRslt->CrossPntLB.x) / 4.0 ;
    _pRslt -> CenterPnt.y = (_pRslt->CrossPntLT.y + _pRslt->CrossPntRT.y + _pRslt->CrossPntRB.y + _pRslt->CrossPntLB.y) / 4.0 ;



    //오프셑값에서 트레숄드 확인 하기.
    float a , b , x , y , px , sum;

    _pRslt -> OffsetPntLT.x = _pRslt->CrossPntLT.x + _tPara.iChkOfs ;
    _pRslt -> OffsetPntLT.y = _pRslt->CrossPntLT.y + _tPara.iChkOfs ;

    _pRslt -> OffsetPntRT.x = _pRslt->CrossPntRT.x - _tPara.iChkOfs ;
    _pRslt -> OffsetPntRT.y = _pRslt->CrossPntRT.y + _tPara.iChkOfs ;

    _pRslt -> OffsetPntLB.x = _pRslt->CrossPntLB.x + _tPara.iChkOfs ;
    _pRslt -> OffsetPntLB.y = _pRslt->CrossPntLB.y - _tPara.iChkOfs ;

    _pRslt -> OffsetPntRB.x = _pRslt->CrossPntRB.x - _tPara.iChkOfs ;
    _pRslt -> OffsetPntRB.y = _pRslt->CrossPntRB.y - _tPara.iChkOfs ;

    //왼쪽변.
    a = MATH_GetLineA(_pRslt->OffsetPntLT , _pRslt->OffsetPntLB );
    b = MATH_GetLineB(_pRslt->OffsetPntLT , _pRslt->OffsetPntLB );
    for(int y = _pRslt->OffsetPntLT.y ; y < _pRslt->OffsetPntLB.y ; y++){
        x  = MATH_GetLineX(y , a ,b ) ;
        px = _pImg -> GetSubPixel(x,y) ;

        _pRslt->iTtlChkPxCnt++;
        sum += px ;
        if( _tPara.iLowThsd > px || px > _tPara.iHighThsd) {
            _pRslt->iChkPxOutCnt++;
        }
    }

    //윗변.
    a = MATH_GetLineA(_pRslt->OffsetPntLT , _pRslt->OffsetPntRT );
    b = MATH_GetLineB(_pRslt->OffsetPntLT , _pRslt->OffsetPntRT );
    for(int x = _pRslt->OffsetPntLT.x ; x < _pRslt->OffsetPntRT.x ; x++){
        y  = MATH_GetLineY(a,x ,b) ;
        px = _pImg -> GetSubPixel(x,y) ;

        _pRslt->iTtlChkPxCnt++;
        sum += px ;
        if( _tPara.iLowThsd > px || px > _tPara.iHighThsd) {
            _pRslt->iChkPxOutCnt++;
        }
    }

    //오른쪽변.
    a = MATH_GetLineA(_pRslt->OffsetPntRT , _pRslt->OffsetPntRB );
    b = MATH_GetLineB(_pRslt->OffsetPntRT , _pRslt->OffsetPntRB );
    for(int y = _pRslt->OffsetPntRT.y ; y < _pRslt->OffsetPntRB.y ; y++){
        x  = MATH_GetLineX(y , a ,b );
        px = _pImg -> GetSubPixel(x,y) ;

        _pRslt->iTtlChkPxCnt++;
        sum += px ;
        if( _tPara.iLowThsd > px || px > _tPara.iHighThsd) {
            _pRslt->iChkPxOutCnt++;
        }
    }

    //아래변.
    a = MATH_GetLineA(_pRslt->OffsetPntLB , _pRslt->OffsetPntRB );
    b = MATH_GetLineB(_pRslt->OffsetPntLB , _pRslt->OffsetPntRB );
    for(int x = _pRslt->OffsetPntLB.x ; x < _pRslt->OffsetPntRB.x ; x++){
        y  = MATH_GetLineY(a,x ,b);
        px = _pImg -> GetSubPixel(x,y) ;

        _pRslt->iTtlChkPxCnt++;
        sum += px ;
        if( _tPara.iLowThsd > px || px > _tPara.iHighThsd) {
            _pRslt->iChkPxOutCnt++;
        }
    }

    fTemp = MATH_GetLineAngle(_pRslt->LinRsltL.StartPnt.x , _pRslt->LinRsltL.StartPnt.y , _pRslt->LinRsltL.EndPnt.x   , _pRslt->LinRsltL.EndPnt.y  ) - 270 ; fAngleSum += fTemp ;
    fTemp = MATH_GetLineAngle(_pRslt->LinRsltT.EndPnt.x   , _pRslt->LinRsltT.EndPnt.y   , _pRslt->LinRsltT.StartPnt.x , _pRslt->LinRsltT.StartPnt.y) - 180 ; fAngleSum += fTemp ;
    fTemp = MATH_GetLineAngle(_pRslt->LinRsltR.StartPnt.x , _pRslt->LinRsltR.StartPnt.y , _pRslt->LinRsltR.EndPnt.x   , _pRslt->LinRsltR.EndPnt.y  ) - 270 ; fAngleSum += fTemp ;
    fTemp = MATH_GetLineAngle(_pRslt->LinRsltB.EndPnt.x   , _pRslt->LinRsltB.EndPnt.y   , _pRslt->LinRsltB.StartPnt.x , _pRslt->LinRsltB.StartPnt.y) - 180 ; fAngleSum += fTemp ;

    _pRslt -> fAngle = fAngleSum / 4.0 ;

    _pRslt->fInspTime = MATH_RoundOff(GetTickTimeDouble()- dwTime , 2) ;

    return true;

}

void CWfrRect::PaintRslt(HDC _hHdc , WRT_Rslt * _pRslt , WRT_Disp _tDisp , float _fScaleX , float _fScaleY)
{
    CGdi *Gdi = new CGdi(_hHdc);
    AnsiString sTemp ;
    Gdi -> SetScale(_fScaleX , _fScaleY);

    if(_tDisp.clRect != clNone) {
        Gdi->m_tPen.Color = _tDisp.clRect ;
        Gdi->m_tPen.Style = PS_SOLID ;
        Gdi->Line((int)_pRslt -> CrossPntLT.x , (int)_pRslt -> CrossPntLT.y ,(int)_pRslt -> CrossPntRT.x , (int)_pRslt -> CrossPntRT.y);
        Gdi->Line((int)_pRslt -> CrossPntRT.x , (int)_pRslt -> CrossPntRT.y ,(int)_pRslt -> CrossPntRB.x , (int)_pRslt -> CrossPntRB.y);
        Gdi->Line((int)_pRslt -> CrossPntRB.x , (int)_pRslt -> CrossPntRB.y ,(int)_pRslt -> CrossPntLB.x , (int)_pRslt -> CrossPntLB.y);
        Gdi->Line((int)_pRslt -> CrossPntLB.x , (int)_pRslt -> CrossPntLB.y ,(int)_pRslt -> CrossPntLT.x , (int)_pRslt -> CrossPntLT.y);

        Gdi->m_tPen.Style = PS_DOT ;
        Gdi->Line((int)_pRslt -> OffsetPntLT.x , (int)_pRslt -> OffsetPntLT.y ,(int)_pRslt -> OffsetPntRT.x , (int)_pRslt -> OffsetPntRT.y);
        Gdi->Line((int)_pRslt -> OffsetPntRT.x , (int)_pRslt -> OffsetPntRT.y ,(int)_pRslt -> OffsetPntRB.x , (int)_pRslt -> OffsetPntRB.y);
        Gdi->Line((int)_pRslt -> OffsetPntRB.x , (int)_pRslt -> OffsetPntRB.y ,(int)_pRslt -> OffsetPntLB.x , (int)_pRslt -> OffsetPntLB.y);
        Gdi->Line((int)_pRslt -> OffsetPntLB.x , (int)_pRslt -> OffsetPntLB.y ,(int)_pRslt -> OffsetPntLT.x , (int)_pRslt -> OffsetPntLT.y);

        Gdi->m_tPen.Style = PS_SOLID ;

    }

    LineL.PaintRslt(_hHdc , &_pRslt->LinRsltL , _tDisp.LinDisp , _fScaleX, _fScaleY) ;
    LineT.PaintRslt(_hHdc , &_pRslt->LinRsltT , _tDisp.LinDisp , _fScaleX, _fScaleY) ;
    LineR.PaintRslt(_hHdc , &_pRslt->LinRsltR , _tDisp.LinDisp , _fScaleX, _fScaleY) ;
    LineB.PaintRslt(_hHdc , &_pRslt->LinRsltB , _tDisp.LinDisp , _fScaleX, _fScaleY) ;

    Gdi -> Rect(false ,  _pRslt -> LTRect);
    Gdi -> Rect(false ,  _pRslt -> RTRect);
    Gdi -> Rect(false ,  _pRslt -> RBRect);
    Gdi -> Rect(false ,  _pRslt -> LBRect);





    if(_tDisp.clPoint != clNone) {
        Gdi->m_tPen.Color = _tDisp.clPoint ;
        Gdi->LineX((int)_pRslt->CenterPnt.x ,(int)_pRslt->CenterPnt.y , 5) ;

        Gdi->LineX((int)_pRslt->CrossPntLT.x ,(int)_pRslt->CrossPntLT.y , 5) ;
        Gdi->LineX((int)_pRslt->CrossPntRT.x ,(int)_pRslt->CrossPntRT.y , 5) ;
        Gdi->LineX((int)_pRslt->CrossPntRB.x ,(int)_pRslt->CrossPntRB.y , 5) ;
        Gdi->LineX((int)_pRslt->CrossPntLB.x ,(int)_pRslt->CrossPntLB.y , 5) ;


    }
    if(_tDisp.clText != clNone) {
        Gdi->m_tText.Color = _tDisp.clText ;
        sTemp = sTemp.sprintf("X:%.3f Y:%.3f",_pRslt->CenterPnt.x , _pRslt->CenterPnt.y) ;
        Gdi->Text((int)_pRslt->CenterPnt.x + 5 ,(int)_pRslt->CenterPnt.y + 5 , sTemp.c_str()) ;
        sTemp = sTemp.sprintf("Angle:%.3f",_pRslt->fAngle) ;
        Gdi->Text((int)_pRslt->CenterPnt.x + 5 ,(int)_pRslt->CenterPnt.y + 15 , sTemp.c_str()) ;
        sTemp = sTemp.sprintf("OutPxCnt:%d",_pRslt->iChkPxOutCnt) ;
        Gdi->Text((int)_pRslt->CenterPnt.x + 5 ,(int)_pRslt->CenterPnt.y + 25 , sTemp.c_str()) ;
    }


    delete Gdi;
}

