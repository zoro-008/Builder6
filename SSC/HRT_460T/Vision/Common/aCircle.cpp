//---------------------------------------------------------------------------
#pragma hdrstop

//#include <math.h>
//#include <vector>
//#include <stdio.h>
#include "aCircle.h"

#include "BaseMaths.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)

//Ransac.
//==============================================================================
CCircle::CCircle()
{
}

CCircle::~CCircle()
{
}

bool CCircle::Inspect(CImage * _pImg , TCircle  _tCircle , TPara _tPara , TRslt * _pRslt , FGetCalPos _fpCalPos)
{
/*

CLinkedList<TFPoint> m_lEdgePx ;
*/

    DWORD dwTime =GetTime();
    _pRslt -> Clear();

    if(_pImg == NULL        ) {_pRslt->sErrMsg = "Image Buffer is NULL." ; return false ; }
    if(_tCircle.rad      <=0) {_pRslt->sErrMsg = "Radius is under 0"     ; return false ; }
    if(_tPara.iGap       <=0) _tPara.iGap       = 1 ;
    if(_tPara.iSearchGap <=0) _tPara.iSearchGap = 1 ;
    if(_tPara.iSampleCnt < 8) _tPara.iSampleCnt = 8 ;
    if(!_tPara.iStartAng && !_tPara.iEndAng) {_tPara.iStartAng = 0 ; _tPara.iEndAng = 360 ;}

    int iCx     = _tCircle.cx  ;
    int iCy     = _tCircle.cy  ;
    int iInRad  = _tCircle.rad - _tPara.iSearchGap ;
    int iOutRad = _tCircle.rad + _tPara.iSearchGap ;

    double fAngUnit = (_tPara.iEndAng - _tPara.iStartAng)/(double)_tPara.iSampleCnt ;

    double fAngle = 0.0 ;
    TPoint  * pSttPnt = new TPoint [_tPara.iSampleCnt];
    TPoint  * pEndPnt = new TPoint [_tPara.iSampleCnt];
    int  iSumSttPnt = 0 ;
    int  iSumEndPnt = 0 ;

    _pRslt->bDarkCircle = true;

    double dTempX ;
    double dTempY ;

    for( int i = 0 ; i < _tPara.iSampleCnt ; i++) {
        fAngle = _tPara.iStartAng + fAngUnit * i ;
        CMath::GetPntFromPntByAngLen(iCx,iCy,fAngle,iInRad ,&dTempX , &dTempY);
        pSttPnt[i].x =dTempX ;
        pSttPnt[i].y =dTempY ;
        CMath::GetPntFromPntByAngLen(iCx,iCy,fAngle,iOutRad,&dTempX , &dTempY);
        pEndPnt[i].x =dTempX ;
        pEndPnt[i].y =dTempY ;

        iSumSttPnt += _pImg -> GetPixel(pSttPnt[i].x , pSttPnt[i].y);
        iSumEndPnt += _pImg -> GetPixel(pEndPnt[i].x , pEndPnt[i].y);
    }

    _pRslt->bDarkCircle = iSumSttPnt < iSumEndPnt ;
    _pRslt->iThreshold  =(iSumSttPnt + iSumEndPnt)/ (_tPara.iSampleCnt * 2) ;

    CEdge        Edge   ;
    CEdge::TRslt EgRslt ;
    TDPoint      RsltPnt;
    TDPoint      BfCalPnt ;
    for( int i = 0 ; i < _tPara.iSampleCnt ; i++) {
        if(!Edge.Inspect(_pImg , pSttPnt[i] , pEndPnt[i] , _tPara.EdgPara , &EgRslt)) {
            _pRslt->sErrMsg = EgRslt.sErrMsg ; 
            delete [] pSttPnt ;
            delete [] pEndPnt ;

            return false ;
        }

        if(_pRslt->bDarkCircle) {RsltPnt.x = EgRslt.DkToLtPnt.x ; RsltPnt.y = EgRslt.DkToLtPnt.y;}
        else                    {RsltPnt.x = EgRslt.LtToDkPnt.x ; RsltPnt.y = EgRslt.LtToDkPnt.y;}


        if(_fpCalPos) {
            BfCalPnt.x = RsltPnt.x ;
            BfCalPnt.y = RsltPnt.y ;
            _fpCalPos(BfCalPnt.x , BfCalPnt.y , &RsltPnt.x , &RsltPnt.y);
        }
        _pRslt->lEdges.PushBack(RsltPnt) ; //¾¥¼Å³Ö°í.

    }

    TDPoint * pEdges = new TDPoint[_pRslt->lEdges.GetDataCnt()];
    for(int i = 0 ; i < _pRslt->lEdges.GetDataCnt() ; i++) {
        pEdges[i] = _pRslt->lEdges.GetCrntData(!i);
    }

    bool bRet = CMath::GetCircleRansac(pEdges , _pRslt->lEdges.GetDataCnt() , _tPara.iGap , &_pRslt->dCntrX , &_pRslt->dCntrY  , &_pRslt->dRadius , &_pRslt->dScore) ;




    delete [] pEdges ;
    delete [] pSttPnt ;
    delete [] pEndPnt ;


    if(!bRet) {_pRslt->sErrMsg = "Racsac Failed." ; return false ;}




    

    _pRslt->dInspTime = GetTime() - dwTime ;
    return bRet ;
}


