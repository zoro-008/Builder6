//---------------------------------------------------------------------------
#pragma hdrstop

#include "aBlob.h"
#include "GdiUnit.h"
#include "SVMaths.h"
#include "Common.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

CBlob::CBlob()
{
}

CBlob::~CBlob()
{
}

int AutoThreshold(CImage * _pImg)
{
    int iWidth   = _pImg -> GetWidth() ;
    int iHeight  = _pImg -> GetHeight();

    int iPixel = 0 , iCount = 0 , iTemp = 0;

    int iAvrGray = 0 ;
    int iSumGray = 0 ;

    int iAllPxCnt = iWidth * iHeight ;

    if(iAllPxCnt <= 0 ) return 0 ;

    //double * dGrayCnt = new double[];
    const int iMaxGrayCnt = 256 ;
    int iGrayCnt[iMaxGrayCnt];

    memset(iGrayCnt , 0 , sizeof(int) * iMaxGrayCnt);

    for(int x = 0 ; x < iWidth ; x++){
        for(int y = 0 ; y < iHeight ; y++){
            iTemp = _pImg -> GetPixel(x,y);
            iGrayCnt[iTemp]++ ;
            iSumGray += iTemp ;
        }
    }

    iAvrGray = iSumGray / iAllPxCnt ;

    int i = 0 , iAutoThresh = 0;
    double dBig = 0.0 , dResult = 0.0 ;
    double dGrayPrcnt1 = 0.0 ;
    double dGrayPrcnt2 = 0.0 ;
    double dGrayW1 = 0.0 , dGrayU1 = 0.0 , dGrayA1 = 0.0 ,
           dGrayW2 = 0.0 , dGrayU2 = 0.0 , dGrayA2 = 0.0 ;


    for(i = 0 ; i < iAvrGray ; i++){
        dGrayPrcnt1 = iGrayCnt[i]/(double)iAllPxCnt ;
    }
    for(i = iAvrGray ; i < iMaxGrayCnt ; i++) {
        dGrayPrcnt2 = iGrayCnt[i]/(double)iAllPxCnt ;

    }


    for(i = 0 ; i < iAvrGray ; i++){
        dGrayW1     = dGrayW1 +  dGrayPrcnt1                ;
        dGrayW2     = dGrayW2 +  dGrayPrcnt2                ;
        dGrayA1     = dGrayA1 + (dGrayPrcnt1 *  i     )     ;
        dGrayA2     = dGrayA2 + (dGrayPrcnt2 * (i+1+i))     ;
        dGrayU1     = dGrayU1 + (dGrayA1 / dGrayW1) * (dGrayA1 / dGrayW1) ;
        dGrayU2     = dGrayU2 + (dGrayA2 / dGrayW2) * (dGrayA2 / dGrayW2) ;

        dResult = ((1-dGrayPrcnt1)*dGrayW1*dGrayU1) + (dGrayW2 * dGrayU2) ;

        if(dBig < dResult) { dBig = dResult ; iAutoThresh = i ; }

    }

    //delete [] iGrayCnt;

    return iAutoThresh ;
}

bool CBlob::Inspect(CImage *_pImg , TRect _tRect , BLB_Para _tPara , BLB_Rslt * _pRslt)
{
    DWORD dwTime =GetTickTimeDouble();
    g_sErrMsg = "";

    //Para Check & Err Check
    if(_pImg == NULL            ) {g_sErrMsg = "Image Buffer is NULL."   ; return false ; }
    //_pImg->SetRect(&_tRect);
    if(_pImg -> SetRect(&_tRect)) {g_sErrMsg = "Rect Overed Image Size." ; return false ; }

    if(_tPara.iPitchX     < 1) _tPara.iPitchX    = 1 ;
    if(_tPara.iPitchY     < 1) _tPara.iPitchY    = 1 ;


    if(_pRslt->pBlobs) {
        for(int i = 0 ; i < _pRslt -> iBlobCnt ; i++) {
            _pRslt -> pBlobs[i].OnClear();
        }
        delete [] _pRslt -> pBlobs ;
        _pRslt -> pBlobs   = NULL  ;
        _pRslt -> iBlobCnt = 0 ;
    }
                                  // 1    , LOOT_2 , 1    , LOOT_2 , 1     , LOOT_2, 1    , LOOT_2
    register const POINT nei[8] = { {1, 0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1} };

    unsigned char * PixelInfo = new unsigned char[_tRect.Width() * _tRect.Height()] ;
    memset(PixelInfo , 0 , _tRect.Width() * _tRect.Height()) ;

    if(_tPara.iThreshold ==0) {
        CImage * pTempImg = new CImage();
        _pImg -> Trim(_tRect , pTempImg);

        _tPara.iThreshold = AutoThreshold(pTempImg);
        delete pTempImg ;
    }

    //Visited Pixel
    #define VPixel(x,y) (*(PixelInfo + x + _tRect.Width() * y))

    //Local
    bool cp ; //CrntPx
    bool pp ; //PastPx
    bool bcp ; //Back Step crnt pixel.
    bool bnp ; //Back step next pixel.
    bool IsDnEg = false ;
    bool IsUpEg = false ;
    bool IsSide = false ;

    register int spx    ; // boundary  start X
    register int spy    ; // boundary  startf y
    register int n      ;
    register int tx = 0 ; // Trace X
    register int ty = 0 ;
    register int u      ; // Trace neiborhood.
    register int v      ;
    register float boundXsum ; // Boundary Position x Sum .
    register float boundYsum ; // Boundary Position y Sum .

    CLinkedList<SBlob>      BBlobs       ; //Buffer Blobs .
    SBlob                   BBlob        ; //Buffer Blob.

    if(1 > _tPara.iPitchY) _tPara.iPitchY = 1 ;
    if(1 > _tPara.iPitchX) _tPara.iPitchX = 1 ;

    bool bTemp ;

    for (int y = _tRect.top + _tPara.iStartYOfs ; y < _tRect.bottom ; y += _tPara.iPitchY ) {
        if( 0 > _tRect.Height() - _tPara.iPitchY ) break ;
        cp = _pImg->GetPixel((_tRect.left),y) > _tPara.iThreshold ; //PastPx
        for (int x = _tRect.left + _tPara.iStartXOfs ; x < _tRect.right ; x += _tPara.iPitchX ) {
            if( 0 > _tRect.Width() - _tPara.iPitchX - 1) continue ;
            pp = cp ;
            cp = _pImg->GetPixel(x,y) > _tPara.iThreshold ;

            if(x-_tRect.left < 0 || x-_tRect.left >= _tRect.Width()) {
                bTemp = true ;
            }
            if(y-_tRect.top  < 0 || y-_tRect.top  >= _tRect.Height()) {
                bTemp = true ;
            }
            IsDnEg =  pp && !cp && !_tPara.bFindLtOnDk && !VPixel((x-_tRect.left ),(y-_tRect.top)) ;
            IsUpEg = !pp &&  cp &&  _tPara.bFindLtOnDk && !VPixel((x-_tRect.left ),(y-_tRect.top)) ;
            bnp = cp ;

            //IsEdge?
            if(IsDnEg || IsUpEg) {
                IsSide = false ;
                for (int i = x ; i > x - _tPara.iPitchX ; i--) {
                    bcp = bnp ;
                    bnp = (_pImg->GetPixel((i-1),y) > _tPara.iThreshold) || (i <= _tRect.left)  ;
                    if (bcp==bnp) continue ;
                    if (bcp!=_tPara.bFindLtOnDk ) continue ;

                    if (VPixel((i-_tRect.left ),(y-_tRect.top))) continue ;

                    tx = spx = i ;
                    ty = spy = y ;
                    BBlob.left = BBlob.right  = BBlob.iStartX = i ;
                    BBlob.top  = BBlob.bottom = BBlob.iStartY = y ;

                    boundXsum = 0 ;
                    boundYsum = 0 ;

                    n = 5 ;
                    int k ;
                    do {
                        //Find next pixel.
                        for (k=0; k<8; k++, n=((n+1) & 7)) {
                            u = tx + nei[n].x ;
                            v = ty + nei[n].y ;
                            if (u<_tRect.left || u>=_tRect.right || v<_tRect.top || v>=_tRect.bottom) { IsSide = true ; continue; }
                            if (cp == (_pImg->GetPixel(u,v) > _tPara.iThreshold )) {
                                BBlob.fBoundary += (n%2) ? 1 : ROOT_2 ;
                                BBlob.iBoundCnt ++ ;
                                break;
                            }
                        }

                        VPixel((tx-_tRect.left),(ty-_tRect.top)) = true;

                        if(tx > BBlob.right ) BBlob.right  = tx ;
                        if(tx < BBlob.left  ) BBlob.left   = tx ;
                        if(ty < BBlob.top   ) BBlob.top    = ty ;
                        if(ty > BBlob.bottom) BBlob.bottom = ty ;

                        boundXsum += tx ;
                        boundYsum += ty ;

                        tx = u ;
                        ty = v ;

                        n = (n+5) & 7;
                    }
                    while(!(spx==tx && spy==ty)&& k<8);

                    if(IsSide && _tPara.bIgnrSide) break ; //경계에 걸처 있는 놈들 무시.


 /////////////////////////////////////////////////////// 이거 쓰면 뻑남. 나중에 확인.
 /*
                    if(_tPara.bIgnrInside) {
                        for (register int by = BBlob.top ; by < BBlob.bottom ; by ++) {
                            cp = _pImg->GetPixel((BBlob.left),by) > _tPara.iThreshold ; //PastPx
                            for (register int bx = BBlob.left ; bx < BBlob.right ; bx ++ ) {
                                if( 0 > BBlob.Width() - _tPara.iPitchX - 1) continue ;
                                pp = cp ;
                                cp = _pImg->GetPixel(bx,by) > _tPara.iThreshold ;

                                IsDnEg =  pp && !cp && !_tPara.bFindLtOnDk && !VPixel((bx-_tRect.left ),(by-_tRect.top)) ;
                                IsUpEg = !pp &&  cp &&  _tPara.bFindLtOnDk && !VPixel((bx-_tRect.left ),(by-_tRect.top)) ;
                                bnp = cp ;

                                //IsEdge?
                                if(IsDnEg || IsUpEg) {
                                    bnp = cp ;
                                    for (register int j = bx ; j > bx - _tPara.iPitchX ; j--) {
                                        bcp = bnp ;
                                        bnp = (_pImg->GetPixel((j-1),by) > _tPara.iThreshold)  ;
                                        if (bcp==bnp) continue ;
                                        if (bcp!=_tPara.bFindLtOnDk ) continue ;

                                        if (VPixel((j-_tRect.left ),(by-_tRect.top))) continue ;

                                        tx = spx = j  ;
                                        ty = spy = by ;

                                        n = 5 ;
                                        int k ;
                                        do {
                                            //Find next pixel.
                                            for (k=0; k<8; k++, n=((n+1) & 7)) {
                                                u = tx + nei[n].x ;
                                                v = ty + nei[n].y ;
                                                if (cp == (_pImg->GetPixel(u,v) > _tPara.iThreshold )) break;
                                            }

                                            VPixel((tx-_tRect.left),(ty-_tRect.top)) = true;
                                            BPnt.x = tx ;
                                            BPnt.y = ty ;

                                            //_pRslt.lBoundary.PushBack(BPnt) ;
                                            BBlob.lBoundary.PushBack(BPnt) ;

                                            boundXsum += tx ;
                                            boundYsum += ty ;

                                            tx = u ;
                                            ty = v ;

                                            n = (n+5) & 7;
                                        }
                                        while(!(spx==tx && spy==ty)&& k<8);
                                    }
                                }
                            }
                        }
                    }


 ////////////////////////////////////////////
*/




                    //Make Blob Info.
                    //BBlob.pBoundary = new TPoint[BPoints.GetDataCnt()] ;
                    //for(int i = 0 ; i < BPoints.GetDataCnt() ; i++) {
                    //    BBlob.pBoundary[i] = BPoints.GetCrntData(!i);
                    //}


                    for (int f = BBlob.top ; f < BBlob.bottom ; f ++) {
                        for (int g = BBlob.left ; g < BBlob.right ; g ++) {
                            if(_pImg->GetPixel(g,f) > _tPara.iThreshold &&  _tPara.bFindLtOnDk) BBlob.iArea++ ;
                            if(_pImg->GetPixel(g,f) < _tPara.iThreshold && !_tPara.bFindLtOnDk) BBlob.iArea++ ;
                        }
                    }



                    if( BBlob.fBoundary &&  BBlob.Width() && BBlob.Height()) {
                        BBlob.iWidth       = BBlob.Width ();
                        BBlob.iHeight      = BBlob.Height();
                        BBlob.iCenterX     = BBlob.Width ()/2 + BBlob.left ;
                        BBlob.iCenterY     = BBlob.Height()/2 + BBlob.top  ;
                        BBlob.fCompactness = BBlob.iArea / BBlob.fBoundary ;
                        BBlob.fCentroidX   = boundXsum   / BBlob.iBoundCnt ;
                        BBlob.fCentroidY   = boundYsum   / BBlob.iBoundCnt ;
                        BBlob.fAngle       = 0.0 ;
                        BBlob.fAspectRatio = (BBlob.iWidth < BBlob.iHeight) ? (BBlob.iWidth / (double)BBlob.iHeight) : (BBlob.iHeight / (double)BBlob.iWidth) ;
                    }

                    if((!_tPara.iMinArea        || _tPara.iMinArea        < BBlob.iArea       )&& (!_tPara.iMaxArea        || _tPara.iMaxArea        > BBlob.iArea       )&&
                       (!_tPara.iMinWidth       || _tPara.iMinWidth       < BBlob.iWidth      )&& (!_tPara.iMaxWidth       || _tPara.iMaxWidth       > BBlob.iWidth      )&&
                       (!_tPara.iMinHeight      || _tPara.iMinHeight      < BBlob.iHeight     )&& (!_tPara.iMaxHeight      || _tPara.iMaxHeight      > BBlob.iHeight     )&&
                       (!_tPara.iMinBoundary    || _tPara.iMinBoundary    < BBlob.fBoundary   )&& (!_tPara.iMaxBoundary    || _tPara.iMaxBoundary    > BBlob.fBoundary   )&&
                       (!_tPara.fMinCompactness || _tPara.fMinCompactness < BBlob.fCompactness)&& (!_tPara.fMaxCompactness || _tPara.fMaxCompactness > BBlob.fCompactness)&&
                       (!_tPara.fMinAngle       || _tPara.fMinAngle       < BBlob.fAngle      )&& (!_tPara.fMaxAngle       || _tPara.fMaxAngle       > BBlob.fAngle      )&&
                       (!_tPara.fMinAspectRatio || _tPara.fMinAspectRatio < BBlob.fAspectRatio)&& (!_tPara.fMaxAspectRatio || _tPara.fMaxAspectRatio > BBlob.fAspectRatio)){

                       BBlobs.PushBack(BBlob) ;
                       BBlob .OnClear () ;



                       if(_tPara.bIgnrInside) {
                           for(register int bx = BBlob.Left - _tRect.left ; bx < BBlob.Right - _tRect.left ; bx++) {
                               for(register int by = BBlob.Top - _tRect.top ; by < BBlob.Bottom - _tRect.top ; by++)
                                   VPixel(bx,by) = true;
                           }
                       }
                    }
                }
            }
        }
    }

    //Make Blob Info.
    _pRslt-> iBlobCnt = BBlobs.GetDataCnt() ;
    _pRslt-> pBlobs = new SBlob[_pRslt-> iBlobCnt] ;
    for(int i = 0 ; i < _pRslt-> iBlobCnt ; i++) {
        SBlob Blob = BBlobs.GetCrntData(!i);
        _pRslt-> pBlobs[i] = Blob ;
    }
    BBlobs.DeleteAll();

    delete[] PixelInfo ;
    _pRslt->fInspTime = MATH_RoundOff(GetTickTimeDouble()- dwTime , 2) ;
    return true ;
}
                              //나중에 _Rslt확인 하기.... LinkedList잘 넘어오는지.
void CBlob::PaintRslt(HDC _hHdc , BLB_Rslt *_pRslt , BLB_Disp _tDisp , float _fScaleX , float _fScaleY )
{//여기부터.
    CGdi *Gdi = new CGdi(_hHdc);
    Gdi -> SetScale(_fScaleX , _fScaleY);

//    SBlob  blob  ;
    TPoint point ;
    AnsiString sTemp ;

    if(_tDisp.clRect != clNone) {
        for(int i = 0 ; i < _pRslt->iBlobCnt ; i++) {
            Gdi->m_tPen .Color = _tDisp.clRect;
            Gdi -> Rect(false,_pRslt->pBlobs[i].left - 1 , _pRslt->pBlobs[i].top - 1 , _pRslt->pBlobs[i].right + 1 , _pRslt->pBlobs[i].bottom + 1);
        }
    }

    if(_tDisp.clCross != clNone) {
        for(int i = 0 ; i < _pRslt->iBlobCnt ; i++) {
            Gdi->m_tPen .Color = _tDisp.clCross;
            Gdi -> LinePlus(_pRslt->pBlobs[i].iCenterX,_pRslt->pBlobs[i].iCenterY,_pRslt->pBlobs[i].iHeight/4) ;
        }
    }

    if(_tDisp.clText != clNone) {
        for(int i = 0 ; i < _pRslt->iBlobCnt ; i++) {
            Gdi->m_tPen .Color = _tDisp.clText ;
            Gdi->m_tText.Color = _tDisp.clText ;
            sTemp.sprintf("%03d",i);
            Gdi -> Text( _pRslt->pBlobs[i].right , _pRslt->pBlobs[i].bottom + 1 , sTemp.c_str() ) ;
        }
    }

    if(_tDisp.clBoundary != clNone) {
        for(int i = 0 ; i < _pRslt->iBlobCnt ; i++) {
            Gdi->m_tPen .Color = _tDisp.clBoundary ;

            //for(int j = 0 ; j < _pRslt->pBlobs[i].iBoundCnt ; j++) {
            //    Gdi -> Pixel(_pRslt->pBlobs[i].pBoundary[j].x, _pRslt->pBlobs[i].pBoundary[j].y,_tDisp.clBoundary);
            //}
        }
    }

    delete Gdi;
}


