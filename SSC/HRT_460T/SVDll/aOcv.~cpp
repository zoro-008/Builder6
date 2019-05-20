//---------------------------------------------------------------------------
#pragma hdrstop
#include "aOcv.h"
#include "SVMaths.h"
#include "Common.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


COcv::COcv()
{
//    Rslt.iDkErrPx  = 0 ;
//    Rslt.iLtErrPx  = 0 ;
//    Rslt.fInspTime = 0.0 ;
//    Rslt.DkFailPx.DeleteAll()  ;
//    Rslt.LtFailPx.DeleteAll()  ;
//    Rslt.iStartX = 0 ;
//    Rslt.iStartY = 0 ;

}

COcv::~COcv()
{
//    delete m_pTrainImg  ;
//    delete m_pInspArea  ;
//
//    Rslt.DkFailPx.DeleteAll()  ;
//    Rslt.LtFailPx.DeleteAll()  ;
}


bool COcv::Train(CImage * _pImg , TRect _Rect , OCV_TrPr _tTrPr ,
                 CArea * _pTrainArea , CImage * _pTrainImg)
{
    if(_tTrPr.iInspOft <= _tTrPr.iNoInspOft) {
        g_sErrMsg = "_tTrPr.iInspOft <= _tTrPr.iNoInspOft";
        return false ;
    }

    _pImg -> SetRect(&_Rect);

    int iLeft   = _Rect.Left    ;
    int iTop    = _Rect.Top     ;
    int iRight  = _Rect.Right   ;
    int iWidth  = _Rect.Width ();
    int iHeight = _Rect.Height();

    _pTrainArea  -> SetSize(iWidth,iHeight) ;

    _pImg -> Trim(_Rect , _pTrainImg);

    bool bEdge  = false ;
    bool bPrePx = _pImg -> GetPixel(iLeft,iTop) ;
    bool bPx    = _pImg -> GetPixel(iLeft,iTop) ;

    TCircle Circle ;


    //Set Temp Insp Area.
    Circle.rad = _tTrPr.iInspOft ;
    for(int y = 0 ; y < iHeight ; y++){
        bPrePx = _pImg -> GetPixel(iLeft,iTop+y) > _tTrPr.iThreshold;
        for(int x = 0 ; x < iWidth ; x++){
            bPx = _pImg -> GetPixel(iLeft+x,iTop+y) > _tTrPr.iThreshold ;
            bEdge = bPx != bPrePx ;
            if(bEdge) {
                Circle.cx = x ;
                Circle.cy = y ;
                _pTrainArea -> AddCircle(Circle , otTemp);
            }
            bPrePx = bPx ;
        }
    }
    for(int x = 0 ; x < iWidth ; x++){
        bPrePx = _pImg -> GetPixel(iLeft+x,iTop) > _tTrPr.iThreshold ;
        for(int y = 0 ; y < iHeight ; y++){
            bPx = _pImg -> GetPixel(iLeft+x,iTop+y) > _tTrPr.iThreshold ;
            bEdge = bPx != bPrePx ;
            if(bEdge) {
                Circle.cx = x ;
                Circle.cy = y ;
                _pTrainArea -> AddCircle(Circle , otTemp);
            }
            bPrePx = bPx ;
        }
    }

    //Set Temp Insp Area.
    Circle.rad = _tTrPr.iNoInspOft ;
    for(int y = 0 ; y < iHeight ; y++){
        bPx = _pImg -> GetPixel(iLeft,iTop+y) > _tTrPr.iThreshold ;
        for(int x = 0 ; x < iWidth ; x++){
            bPx = _pImg -> GetPixel(iLeft+x,iTop+y) > _tTrPr.iThreshold ;
            bEdge = bPx != bPrePx ;
            if(bEdge) {
                Circle.cx = x ;
                Circle.cy = y ;
                _pTrainArea -> AddCircle(Circle , otUnknown);
            }
            bPrePx = bPx ;
        }
    }
    for(int x = 0 ; x < iWidth ; x++){
        bPx = _pImg -> GetPixel(iLeft+x,iTop) > _tTrPr.iThreshold ;
        for(int y = 0 ; y < iHeight ; y++){
            bPx = _pImg -> GetPixel(iLeft+x,iTop+y) > _tTrPr.iThreshold ;
            bEdge = bPx != bPrePx ;
            if(bEdge) {
                Circle.cx = x ;
                Circle.cy = y ;
                _pTrainArea -> AddCircle(Circle , otUnknown);
            }
            bPrePx = bPx ;
        }
    }

    //Separate Light,Dark Area.
    for(int y = 0 ; y < iHeight ; y++){
        for(int x = 0 ; x < iWidth ; x++){
            if(_pTrainArea -> GetPixel(x,y) != otTemp) continue ;
            if(_pImg -> GetPixel(iLeft+x,iTop+y) > _tTrPr.iThreshold )_pTrainArea -> SetPixel(x,y,otLtInsp ) ;
            else                                                      _pTrainArea -> SetPixel(x,y,otDkInsp ) ;
        }
    }

    //_pTrainArea -> SaveBmp("D:\\sun.bmp");


    return true ;
}

struct TPxPoint
{
    int x,y;
    unsigned char px ;
    TPxPoint() {x=0;y=0;px=0;}
    TPxPoint(int _x, int _y , unsigned char _px) { x=_x; y=_y; px=_px;}
};

bool COcv::Inspect(CImage * _pImg , CArea * _pTrainArea , CImage * _pTrainImg , TRect _tInspRect , OCV_Para _tPara ,
                   CArea * _pRsltArea , OCV_Rslt * _pRslt)
{
    _pImg -> SetRect(&_tInspRect) ;

    int iLeft   = _tInspRect.Left    ;
    int iTop    = _tInspRect.Top     ;
    int iRight  = _tInspRect.Right   ;
    int iWidth  = _tInspRect.Width ();
    int iHeight = _tInspRect.Height();

    memset(_pRslt , 0 , sizeof(OCV_Rslt));

    if(iHeight < _pTrainArea -> GetHeight() ) {
        g_sErrMsg = "iHeight <= _pTrainArea -> GetHeight()";
        return false ;
    }

    if(iWidth < _pTrainArea -> GetWidth() ) {
        g_sErrMsg = "iWidth <= _pTrainArea -> GetWidth()";
        return false ;
    }

    CLinkedList<TPxPoint> llInspPnt  ;  //속도향상을 위해 검사 할 위치를 리스트에 넣어서 검사한다.

    _pRsltArea -> SetSize(_pTrainArea -> GetWidth() , _pTrainArea -> GetHeight());

    int iXRange = iWidth  - _pTrainArea -> GetWidth () ;
    int iYRange = iHeight - _pTrainArea -> GetHeight() ;

    int iMinErrCnt = MAX_INT_VALUE ;
    int iMinErrX   = 0 ;
    int iMinErrY   = 0 ;
    int iErrCntLmt = 0 ;
    int iInspPxSum = 0 ;
    int iInspPxAvr = 0 ;
    int iInspPxCnt = 0 ;
    unsigned char cPx = 0 ;

    //미리 검사할 포인트를 링크드 리스트에 넣는다.
    //const int iInspFreq = 10 ; //검사 빈도수.
    if(_tPara.iInspFreq < 1)  _tPara.iInspFreq = 1 ;
    for(int y = 0 ; y < _pTrainArea -> GetHeight() ; y++) {
        for(int x = 0 ; x < _pTrainArea -> GetWidth() ; x++) {
            if(x%_tPara.iInspFreq == 0 || y%_tPara.iInspFreq == 0) {
                if(_pTrainArea -> GetPixel(x , y) == otDkInsp || _pTrainArea -> GetPixel(x , y) == otLtInsp ){
                    cPx =  _pTrainImg -> GetPixel(x , y) ;
                    llInspPnt.PushBack(TPxPoint(x,y,cPx));
                    _pRsltArea -> SetPixel(x,y,orInsp);
                    iInspPxSum+= cPx;
                }
            }

        }
    }
    /*
    for(int y = 0 ; y < _pTrainArea -> GetHeight() ; 9y+=_tPara.iInspFreq) {
        for(int x = 0 ; x < _pTrainArea -> GetWidth() ; x+=_tPara.iInspFreq) {

            cPx =  _pTrainImg -> GetPixel(x , y) ;
            if(_pTrainArea -> GetPixel(x , y) == otDkInsp || _pTrainArea -> GetPixel(x , y) == otLtInsp){
                llInspPnt.PushBack(TPxPoint(x,y,cPx));
                _pRsltArea -> SetPixel(x,y,orInsp);

            }
            iInspPxSum+= cPx;
        }
    }*/

    iInspPxCnt = llInspPnt.GetDataCnt() ;

    if(iInspPxCnt == 0 ) { g_sErrMsg = "No Insp Point"; return false ;}
    iInspPxAvr = iInspPxSum / (float)iInspPxCnt ;

    //iErrCntLmt = iInspPxCnt - (iInspPxCnt * _tPara.fSinc / 100) ;

    //링크드 리스트에서 배열로 옮기기.
    TPxPoint * tInspPxPnt = new TPxPoint[iInspPxCnt] ;

    for(int i = 0 ; i < iInspPxCnt ; i++) {
        tInspPxPnt[i] = llInspPnt.GetCrntData(!i) ;
    }


    //준비끝 검사 해보자..
    TPxPoint Pnt ;
    int      iImgInspAvr ;
    int      iImgInspSum ;
    int      iImgAvrGap  ;
    int      iInspErrCnt ;
    int      iDkErrCnt   ;
    int      iLtErrCnt   ;
    int      iTemp       ;
    for(int ry = 0 ; ry <= iYRange ; ry++) {
        for(int rx = 0 ; rx <= iXRange ; rx++) {
            iInspErrCnt = 0 ;
            iLtErrCnt   = 0 ;
            iDkErrCnt   = 0 ;

            //검사지역 평균 구하기.
            iImgInspAvr = 0 ;
            iImgInspSum = 0 ;
            iImgAvrGap  = 0 ;
            for(int i = 0 ; i < iInspPxCnt ; i++) {
                Pnt = tInspPxPnt[i] ;
                iImgInspSum += _pImg -> GetPixel(iLeft + rx + Pnt.x , iTop + ry + Pnt.y) ;
            }
            iImgInspAvr = iImgInspSum / (float)iInspPxCnt ;

            iImgAvrGap = iImgInspAvr - iInspPxAvr ;

            //비교하여 검사지역과 트레인 영역의 갭을 구함.
            for(int i = 0 ; i < iInspPxCnt ; i++) {
                Pnt = tInspPxPnt[i] ;
                iTemp = _pImg -> GetPixel(iLeft + rx + Pnt.x , iTop + ry + Pnt.y) - iImgAvrGap - Pnt.px ;
                if(iTemp > 0) iLtErrCnt+=iTemp;
                else          iDkErrCnt-=iTemp;

                iInspErrCnt += abs(iTemp) ;
            }

            //에러 픽셀들 비교 하고 적은것을 세팅.
            if(iInspErrCnt < iMinErrCnt) {
                iMinErrCnt = iInspErrCnt ;
                iMinErrX   = rx ;
                iMinErrY   = ry ;

                _pRslt -> iDkPxCnt     = iDkErrCnt ;
                _pRslt -> iLtPxCnt     = iLtErrCnt ;
                _pRslt -> iPosX        = iLeft + rx + _pTrainArea -> GetWidth () / 2 ;
                _pRslt -> iPosY        = iTop  + ry + _pTrainArea -> GetHeight() / 2 ;                         //화이트 나 블랙 바탕에서 무조건 50프로 이상 나오는 것 때문에 줄임. 50프로 미만은 고만 고만 해서 무시함.
                _pRslt -> fSinc        = ((iInspPxCnt*256 - iMinErrCnt) / (float)(iInspPxCnt*256)) * 100.0 ;
                _pRslt -> tRect.left   = iLeft + rx ;
                _pRslt -> tRect.top    = iTop  + ry ;
                _pRslt -> tRect.right  = iLeft + rx + _pTrainArea -> GetWidth()  ;
                _pRslt -> tRect.bottom = iTop  + ry + _pTrainArea -> GetHeight() ;



            }
        }
    }

    //iErrCntLmt = iInspPxCnt - (iInspPxCnt * _tPara.fSinc / 100) ;

    /*
    for(int y = 0 ; y < _pTrainArea -> GetHeight() ; y++) {
        for(int x = 0 ; x < _pTrainArea -> GetWidth() ; x++) {
            if(_pTrainArea -> GetPixel(x , y) == otDkInsp){
                if(_pImg -> GetPixel(iLeft + iMinErrX +x , iTop + iMinErrY +y) >  _tPara.iThreshold ) _pRsltArea -> SetPixel(x,y,orDkFail) ;
            }
            else if(_pTrainArea -> GetPixel(x , y) == otLtInsp){
                if(_pImg -> GetPixel(iLeft + iMinErrX +x , iTop + iMinErrY +y) <= _tPara.iThreshold ) _pRsltArea -> SetPixel(x,y,orLtFail) ;
            }
        }
    }
    */

    delete [] tInspPxPnt ;


    return true ;
}

void COcv::PaintRslt(HDC _hHdc , CArea * _pRsltArea , OCV_Rslt * _pRslt, OCV_Disp _tDisp, float _fScaleX , float _fScaleY)
{

    CGdi *Gdi = new CGdi(_hHdc);
    Gdi -> SetScale(_fScaleX , _fScaleY);

    AnsiString sTemp ;
    int        iCtrX ;
    int        iCtrY ;

             // clDkFailPx ;
             // clLtFailPx ;
             // clText     ;



    //if(_tDisp.clDkFailPx) _pRsltArea  -> m_iPxColor[orDkFail ] = _tDisp.clDkFailPx ;
    //if(_tDisp.clLtFailPx) _pRsltArea  -> m_iPxColor[orLtFail ] = _tDisp.clLtFailPx ;


    if(_tDisp.clInspPx ) {
        _pRsltArea -> m_iPxColor[orNone   ] = clBlack ;
        _pRsltArea -> m_iPxColor[orInsp   ] = _tDisp.clInspPx ;
        _pRsltArea -> ShowArea(_hHdc , _pRslt->tRect.left , _pRslt->tRect.top ,_fScaleX , _fScaleY);
    }


    if(_tDisp.clText != clNone){
        Gdi -> m_tText.Color = _tDisp.clText ;
        sTemp = sTemp.sprintf("%.2f",_pRslt->fSinc);       
        sTemp = AnsiString("X : ") + _pRslt->iPosX + AnsiString(" Y : ") + _pRslt->iPosY + AnsiString(" Sinc : ") + sTemp ;
        Gdi -> Text(_pRslt->tRect.left + 5, _pRslt->tRect.top - 15 ,sTemp.c_str()) ;
    }

    if(_tDisp.clCenter != clNone) {
        Gdi -> m_tPen.Color = _tDisp.clCenter ;
        Gdi -> LineCross(_pRslt->iPosX , _pRslt->iPosY , 10) ;
    }

    if(_tDisp.clRect != clNone) {
        Gdi -> m_tPen.Color = _tDisp.clRect ;
        Gdi -> Rect(false , _pRslt->tRect);

    }

    delete Gdi;

}

/*
int COcv::Train(CImage * _pImg , TRect _Rect)
{
    int iLeft   = _Rect.Left    ;
    int iTop    = _Rect.Top     ;
    int iRight  = _Rect.Right   ;
    int iBottom = _Rect.Bottom  ;
    int iWidth  = _Rect.Width ();
    int iHeight = _Rect.Height();
    int iImgBit = _pImg->GetImgBit();

    m_pTrainImg  -> SetSize(iWidth,iHeight,iImgBit) ;
    m_pInspArea  -> SetSize(iWidth,iHeight        ) ;

    //이미지 복사.
    for(int x = 0 ; x < iWidth ; x++){
        for(int y = 0 ; y < iHeight ; y++) {
            m_pTrainImg -> SetPixel(x,y,_pImg->GetPixel(iLeft+x,iTop+y)) ;
            if(iImgBit==24){
                m_pTrainImg -> SetPixelR(x,y,_pImg->GetPixelR(iLeft+x,iTop+y)) ;
                m_pTrainImg -> SetPixelG(x,y,_pImg->GetPixelG(iLeft+x,iTop+y)) ;
                m_pTrainImg -> SetPixelB(x,y,_pImg->GetPixelB(iLeft+x,iTop+y)) ;
            }
        }
    }


    int iSum = 0 ;
    for(int x = 0 ; x < iWidth ; x++){
        for(int y = 0 ; y < iHeight ; y++) {
            iSum += m_pTrainImg -> GetPixel(x,y) ;
        }
    }
    if(!Para.iThreshold) Para.iThreshold = iSum / (iWidth * iHeight) ;









    //검사영역 설정.  0=검사아직안함 , 1=다크 , 2=라이트  , 3=검사하지않는 영역.
    unsigned char cPpx  = m_pTrainImg -> GetPixel(0,0) ;
    unsigned char cCpx  = m_pTrainImg -> GetPixel(0,0) ;
    unsigned char cInsp = m_pInspArea -> GetPixel(0,0) ;

    for(int y = 0 ; y < iHeight ; y++) {
        cPpx = m_pTrainImg -> GetPixel(0,y) ;
        for(int x = 0 ; x < iWidth ; x++){
            cCpx  = m_pTrainImg -> GetPixel(x,y) ;
            cInsp = m_pInspArea -> GetPixel(x,y) ;
            if(cInsp != atNoInsp) {
                if(cCpx > Para.iThreshold ) m_pInspArea -> SetPixel(x,y,atLight) ;
                else                        m_pInspArea -> SetPixel(x,y,atDark ) ;
            }

            //엦지 발견.
            if((cCpx >  Para.iThreshold && cPpx <= Para.iThreshold) || (cCpx <= Para.iThreshold && cPpx > Para.iThreshold)) {
                for(int i = x - Para.iPxOffset ; i <= x + Para.iPxOffset ;i++) {
                    for(int j = y - Para.iPxOffset ; j <= y + Para.iPxOffset ;j++) {
                        if(i<0 || j<0 || i>=m_pTrainImg -> GetWidth() || j>=m_pTrainImg -> GetHeight()) continue ;
                        if(Math.GetCircleInPoint(x , y , Para.iPxOffset , i , j)){
                            m_pInspArea -> SetPixel(i,j,atNoInsp) ;
                        }
                    }
                }
            }
            cPpx = cCpx ;
        }
    }


    for(int x = 0 ; x < iWidth ; x++){
        cPpx = m_pTrainImg -> GetPixel(x,0) ;
        for(int y = 0 ; y < iHeight ; y++) {
            cCpx  = m_pTrainImg -> GetPixel(x,y) ;
            cInsp = m_pInspArea -> GetPixel(x,y) ;
            if(cInsp != atNoInsp) {
                if(cCpx > Para.iThreshold ) m_pInspArea -> SetPixel(x,y,atLight) ;
                else                        m_pInspArea -> SetPixel(x,y,atDark ) ;
            }

            //엦지 발견.
            if((cCpx >  Para.iThreshold && cPpx <= Para.iThreshold) || (cCpx <= Para.iThreshold && cPpx >  Para.iThreshold)) {
                for(int i = x - Para.iPxOffset ; i <= x + Para.iPxOffset ;i++) {
                    for(int j = y - Para.iPxOffset ; j <= y + Para.iPxOffset ;j++) {
                        if(i<0 || j<0 || i>=m_pTrainImg -> GetWidth() || j>=m_pTrainImg -> GetHeight()) continue ;
                        if(Math.GetCircleInPoint(x , y , Para.iPxOffset , i , j)){
                            m_pInspArea -> SetPixel(i,j,atNoInsp) ;
                        }
                    }
                }
            }
            cPpx = cCpx ;
        }
    }
}

int COcv::Inspection(CImage * _pImg , int _iSx , int _iSy)
{
    Rslt.iDkErrPx  = 0 ;
    Rslt.iLtErrPx  = 0 ;
    Rslt.fInspTime = 0.0 ;
    Rslt.DkFailPx.DeleteAll()  ;
    Rslt.LtFailPx.DeleteAll()  ;
    Rslt.iStartX = _iSx ;
    Rslt.iStartY = _iSy ;

    double dTime = GetTickTimeDouble () ;

    int iWidth  = m_pInspArea -> GetWidth () ;
    int iHeight = m_pInspArea -> GetHeight() ;

    for(int x = 0 ; x < iWidth ; x++) {
        for(int y = 0 ; y < iHeight ; y++) {
            if(m_pInspArea -> GetPixel(x,y) == atNoInsp) continue ;
            if(m_pInspArea -> GetPixel(x,y) == atDark  ) {
                 if(_pImg -> GetPixel(_iSx+x,_iSy+y) > Para.iThreshold){
                     Rslt.iDkErrPx ++ ;
                     Rslt.DkFailPx.PushFrnt(TPoint(x,y));
                 }
            }

            else if(m_pInspArea -> GetPixel(x,y) == atLight ) {
                 if(_pImg -> GetPixel(_iSx+x,_iSy+y) <= Para.iThreshold){
                     Rslt.iLtErrPx ++ ;
                     Rslt.LtFailPx.PushFrnt(TPoint(x,y));
                 }
            }
            else continue ;
        }
    }

    dTime = GetTickTimeDouble() - dTime ;

    Rslt.fInspTime = Math.RoundOff(dTime,3);
}

void COcv::PaintRslt (HDC _hHdc)
{
    if(Rslt.fInspTime == 0.0) return ;

    SGdi *Gdi = new SGdi(_hHdc);

    Gdi->m_tPen  .Color = clRed ;
    Gdi->m_tBrush.Color = clRed ;
    Gdi->m_tPen  .Width = 1     ;

    TPoint Pnt ;
    int iPxCnt = 0 ;
    for(int i = 0 ; i < Rslt.DkFailPx.GetDataCnt() ; i++){
        Pnt = Rslt.DkFailPx.GetCrntData(!i);
        Pnt.x += Rslt.iStartX ;
        Pnt.y += Rslt.iStartY ;
        if(!(iPxCnt++%2))Gdi -> Pixel(Pnt,clBlack) ;
    }

    for(int i = 0 ; i < Rslt.LtFailPx.GetDataCnt() ; i++){
        Pnt = Rslt.LtFailPx.GetCrntData(!i);
        Pnt.x += Rslt.iStartX ;
        Pnt.y += Rslt.iStartY ;
        if(!(iPxCnt++%2))Gdi -> Pixel(Pnt,clWhite) ;
    }
    delete Gdi ;
}
*/







