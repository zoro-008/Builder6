//---------------------------------------------------------------------------
#pragma hdrstop

#include "aBlob.h"
#include "aAutoThreshold.h"
#include "BaseMaths.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)

CBlob::CBlob()
{
}

CBlob::~CBlob()
{
}

bool CBlob::Inspect(unsigned char *_pPxl, int _iPxlWidth  , TRect _tRect , TPara _tPara , TFltr _tFltr , TRslt *_pRslt)
{
                                  // 1    , LOOT_2 , 1    , LOOT_2 , 1     , LOOT_2, 1    , LOOT_2
    register const POINT aNeibor[8] = { {1, 0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1} };

    unsigned char * acVisit = new unsigned char[_tRect.Width() * _tRect.Height()] ;
    memset(acVisit , 0 , _tRect.Width() * _tRect.Height()) ;

    //Visited Pixel
    #define VInfo(x,y) (*(acVisit + x + _tRect.Width() * y))
    //Pixel Info.
    #define PInfo(x,y) (*(_pPxl   + x + _iPxlWidth     * y))

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

    TBlob * BBlob = new TBlob() ; //Buffer Blob.

    int iY ;
    int iX ;
    unsigned char cCrntPx ;
    for (double y = _tRect.top + _tPara.iStartYOfs ; y < _tRect.bottom ; y += _tPara.dPitchY ) {
        iY = y ;
        if( 0 > _tRect.Height() - _tPara.dPitchY ) break ;
        cp = PInfo((_tRect.left),iY) > _tPara.iThreshold ; //PastPx
        for (double x = _tRect.left + _tPara.iStartXOfs; x < _tRect.right ; x += _tPara.dPitchX ) {
            if( _tPara.dPitchX > _tRect.Width() - 1) continue ;
            iX = x ;
            cCrntPx = PInfo(iX,iY) ;
            pp = cp ;
            cp = cCrntPx > _tPara.iThreshold ;

            IsDnEg =  pp && !cp && !_tPara.bFindLtOnDk && !VInfo((iX-_tRect.left ),(iY-_tRect.top)) ;
            IsUpEg = !pp &&  cp &&  _tPara.bFindLtOnDk && !VInfo((iX-_tRect.left ),(iY-_tRect.top)) ;
            bnp = cp ;

            //IsEdge?
            if(IsDnEg || IsUpEg) {
                IsSide = false ;
                BBlob -> Clear();
                for (int i = iX ; i > iX - _tPara.dPitchX ; i--) {
                    bcp = bnp ;
                    bnp = (PInfo((i-1),iY) > _tPara.iThreshold) || (i <= _tRect.left)  ;
                    if (bcp==bnp) continue ;
                    if (bcp!=_tPara.bFindLtOnDk ) continue ;

                    if (VInfo((i-_tRect.left ),(iY-_tRect.top))) continue ;

                    tx = spx = i ;
                    ty = spy = iY ;
                    BBlob->left = BBlob->right  = i ;
                    BBlob->top  = BBlob->bottom = iY ;

                    boundXsum = 0 ;
                    boundYsum = 0 ;

                    n = 5 ;
                    int k ;
                    do {
                        //Find next pixel.         여기 혹시 전에 8방향으로 돌려볼때 8개에서도 엣지 찾아야 되지않나???=>아니군 LB부터 검색이라 상과 없음.
                        for (k=0; k<8; k++, n=((n+1) & 7)) {
                            u = tx + aNeibor[n].x ;
                            v = ty + aNeibor[n].y ;
                            if (u<_tRect.left || u>=_tRect.right || v<_tRect.top || v>=_tRect.bottom) { IsSide = true ; continue; }
                            if (cp == (PInfo(u,v) > _tPara.iThreshold )) {
                                BBlob->dBoundary += (n%2) ? 1 : ROOT_2 ;
                                break;
                            }
                        }

                        VInfo((tx-_tRect.left),(ty-_tRect.top)) = true;

                        if(tx > BBlob->right ) BBlob->right  = tx ;
                        if(tx < BBlob->left  ) BBlob->left   = tx ;
                        if(ty < BBlob->top   ) BBlob->top    = ty ;
                        if(ty > BBlob->bottom) BBlob->bottom = ty ;

                        BBlob -> plBoundary.PushBack(TPoint(tx,ty)) ;

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
                        for (register int by = BBlob->top ; by < BBlob->bottom ; by ++) {
                            cp = PInfo((BBlob->left),by) > _tPara.iThreshold ; //PastPx
                            for (register int bx = BBlob->left ; bx < BBlob->right ; bx ++ ) {
                                if( 0 > BBlob->Width() - _tPara.iPitchX - 1) continue ;
                                pp = cp ;
                                cp = PInfo(bx,by) > _tPara.iThreshold ;

                                IsDnEg =  pp && !cp && !_tPara.bFindLtOnDk && !VInfo((bx-_tRect.left ),(by-_tRect.top)) ;
                                IsUpEg = !pp &&  cp &&  _tPara.bFindLtOnDk && !VInfo((bx-_tRect.left ),(by-_tRect.top)) ;
                                bnp = cp ;

                                //IsEdge?
                                if(IsDnEg || IsUpEg) {
                                    bnp = cp ;
                                    for (register int j = bx ; j > bx - _tPara.iPitchX ; j--) {
                                        bcp = bnp ;
                                        bnp = (PInfo((j-1),by) > _tPara.iThreshold)  ;
                                        if (bcp==bnp) continue ;
                                        if (bcp!=_tPara.bFindLtOnDk ) continue ;

                                        if (VInfo((j-_tRect.left ),(by-_tRect.top))) continue ;

                                        tx = spx = j  ;
                                        ty = spy = by ;

                                        n = 5 ;
                                        int k ;
                                        do {
                                            //Find next pixel.
                                            for (k=0; k<8; k++, n=((n+1) & 7)) {
                                                u = tx + aNeibor[n].iX ;
                                                v = ty + aNeibor[n].iY ;
                                                if (cp == (PInfo(u,v) > _tPara.iThreshold )) break;
                                            }

                                            VInfo((tx-_tRect.left),(ty-_tRect.top)) = true;
                                            BPnt.iX = tx ;
                                            BPnt.iY = ty ;

                                            //_pRslt.lBoundary.PushBack(BPnt) ;
                                            BBlob->lBoundary.PushBack(BPnt) ;

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

                    if( BBlob->dBoundary &&  BBlob->Width() && BBlob->Height()) {
                        BBlob->iWidth       = BBlob->Width ();
                        BBlob->iHeight      = BBlob->Height();
                        BBlob->iCenterX     = BBlob->Width ()/2 + BBlob->left ;
                        BBlob->iCenterY     = BBlob->Height()/2 + BBlob->top  ;

                        //무게중심 및 에리아 추출.
                        unsigned int iPxCntX = 0;
                        unsigned int iPxSumX = 0;
                        unsigned int iPxCntY = 0;
                        unsigned int iPxSumY = 0;
                        unsigned char cPxValue = 0 ;
                        if(_tPara.bFindLtOnDk) { //검은것 위에 하얀 물체 찾을때.
                            for (int f = BBlob->top ; f < BBlob->bottom ; f ++) {
                                for (int g = BBlob->left ; g < BBlob->right ; g ++) {
                                    cPxValue = PInfo(g,f) ;
                                    if(cPxValue > _tPara.iThreshold ) {
                                        BBlob->iArea++ ;
                                        //pPxSumY[f] += PInfo(g,f) ;
                                        iPxCntY += cPxValue ;
                                        iPxSumY += cPxValue * f ;

                                    }
                                }
                            }
                            for (int g = BBlob->left ; g < BBlob->right ; g ++) {
                                for (int f = BBlob->top ; f < BBlob->bottom ; f ++) {
                                    cPxValue = PInfo(g,f) ;
                                    if(cPxValue >  _tPara.iThreshold ) {
                                        //pPxSumX[g] += PInfo(g,f) ;
                                        iPxCntX += cPxValue ;
                                        iPxSumX += cPxValue * g ;

                                    }
                                }
                            }
                        }
                        else {  //하얀것 위에 검은 물체 찾을때.
                            for (int f = BBlob->top ; f < BBlob->bottom ; f ++) {
                                for (int g = BBlob->left ; g < BBlob->right ; g ++) {
                                    cPxValue = 255 - PInfo(g,f) ; //계산 편하게 반전시킴.
                                    if(cPxValue > 255 - _tPara.iThreshold ) {
                                        BBlob->iArea++ ;
                                        //pPxSumY[f] += PInfo(g,f) ;
                                        iPxCntY += cPxValue ;
                                        iPxSumY += cPxValue * f ;

                                    }
                                }
                            }
                            for (int g = BBlob->left ; g < BBlob->right ; g ++) {
                                for (int f = BBlob->top ; f < BBlob->bottom ; f ++) {
                                    cPxValue = 255 - PInfo(g,f) ;
                                    if(cPxValue > 255 - _tPara.iThreshold ) {
                                        //pPxSumX[g] += PInfo(g,f) ;
                                        iPxCntX += cPxValue ;
                                        iPxSumX += cPxValue * g ;

                                    }
                                }
                            }
                        }


                        BBlob->dCompact     = BBlob->iArea / BBlob->dBoundary ;

                        //무게중심 찾기.
                        if(iPxCntX)BBlob-> dCentroidX = iPxSumX / (double)iPxCntX ;
                        if(iPxCntY)BBlob-> dCentroidY = iPxSumY / (double)iPxCntY ;
                        //BBlob-> dCentroidX = BBlob-> iCenterX ;
                        //BBlob-> dCentroidY = BBlob-> iCenterY ;


                    }

                    if(_tFltr.iMinArea     && BBlob->iArea     < _tFltr.iMinArea    ) continue ; if(_tFltr.iMaxArea     && BBlob->iArea     > _tFltr.iMaxArea    ) continue ;
                    if(_tFltr.iMinWidth    && BBlob->iWidth    < _tFltr.iMinWidth   ) continue ; if(_tFltr.iMaxWidth    && BBlob->iWidth    > _tFltr.iMaxWidth   ) continue ;
                    if(_tFltr.iMinHeight   && BBlob->iHeight   < _tFltr.iMinHeight  ) continue ; if(_tFltr.iMaxHeight   && BBlob->iHeight   > _tFltr.iMaxHeight  ) continue ;
                    if(_tFltr.dMinBoundary && BBlob->dBoundary < _tFltr.dMinBoundary) continue ; if(_tFltr.dMaxBoundary && BBlob->dBoundary > _tFltr.dMaxBoundary) continue ;
                    if(_tFltr.dMinCompact  && BBlob->dCompact  < _tFltr.dMinCompact ) continue ; if(_tFltr.dMaxCompact  && BBlob->dCompact  > _tFltr.dMaxCompact ) continue ;

                    _pRslt -> plBlob.PushBack(BBlob) ; //큐에 넣어버리고
                    BBlob = new TBlob();      //새로 만듬.

                    if(_tPara.bIgnrInside) {
                        for(int bx = BBlob->Left - _tRect.left ; bx < BBlob->Right - _tRect.left ; bx++) {
                            for(int by = BBlob->Top - _tRect.top ; by < BBlob->Bottom - _tRect.top ; by++)
                                VInfo(bx,by) = true;
                        }
                    }
                }
            }
        }
    }
    delete BBlob ; BBlob = NULL;
    delete[] acVisit ;

    return true ;
}

bool CBlob::Inspect(CImage *_pImg , TRect _tRect , TPara _tPara , TFltr _tFltr , TRslt *_pRslt)
{
    DWORD dwTime =GetTime();
    _pRslt -> Clear();

    //Para Check & Err Check
    if(_pImg == NULL ) {_pRslt->sErrMsg = "Image Buffer is NULL."   ; return false ; }
    _pImg -> SetRect(&_tRect);

    if(_tPara.dPitchX < 1.0) _tPara.dPitchX    = 1.0 ;
    if(_tPara.dPitchY < 1.0) _tPara.dPitchY    = 1.0 ;


    if(_tPara.iThreshold ==0) { //if 0 AutoThresholding.
        CAutoThreshold::TPara TshPara ;
        CAutoThreshold::TRslt TshRslt ;

        TshPara.bSimple = false ;

        if(CAutoThreshold::Inspection(_pImg , _tRect , TshPara , &TshRslt)){
            _tPara.iThreshold = TshRslt.iThreshold ;
        }
        else {
            _pRslt->sErrMsg = TshRslt.sErrMsg ;
            return false ;
        }
    }

    Inspect(_pImg->GetImg(),_pImg->GetMem8Width()  , _tRect , _tPara , _tFltr , _pRslt);

    _pRslt -> iBlobCnt  = _pRslt -> plBlob.GetDataCnt() ;
    _pRslt -> dInspTime = GetTime()- dwTime ;

    return true ;
}

bool CBlob::Inspect(CArea *_pAra , TRect _tRect , TPara _tPara , TFltr _tFltr , TRslt *_pRslt)
{
    DWORD dwTime =GetTime();
    _pRslt -> Clear();

    //Para Check & Err Check
    if(_pAra == NULL ) {_pRslt->sErrMsg = "Image Buffer is NULL."   ; return false ; }
    _pAra -> SetRect(&_tRect);

    if(_tPara.dPitchX < 1.0) _tPara.dPitchX    = 1.0 ;
    if(_tPara.dPitchY < 1.0) _tPara.dPitchY    = 1.0 ;

    Inspect(_pAra->GetArea() ,_pAra->GetMemWidth(), _tRect , _tPara , _tFltr , _pRslt);

    _pRslt -> iBlobCnt  = _pRslt -> plBlob.GetDataCnt() ;
    _pRslt -> dInspTime = GetTime()- dwTime ;

    return true ;
}

