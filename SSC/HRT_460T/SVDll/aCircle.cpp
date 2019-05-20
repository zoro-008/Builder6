//---------------------------------------------------------------------------
#pragma hdrstop

#include <math.h>
#include <vector>
#include <stdio.h>
#include "aCircle.h"
#include "GdiUnit.h"
#include "SVMaths.h"

#include "Common.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)

//Ransac.
//==============================================================================
CCircle::CCircle()
{
}

CCircle::~CCircle()
{
    m_lEdgePx.DeleteAll() ;
}

bool CCircle::Inspect(CImage * _pImg , TCircle  _tCircle , CCL_Para _tPara , CCL_Rslt * _pRslt)
{
    //검사 시작 시간.
    DWORD dwTime =GetTickTimeDouble();
    m_lEdgePx.DeleteAll() ;
    memset(_pRslt , 0 , sizeof(CCL_Rslt));
    g_sErrMsg = "";
    if(_pImg == NULL        ) {g_sErrMsg = "Image Buffer is NULL." ; return false ; }
    if(_tCircle.rad      <=0) {g_sErrMsg = "Radius is under 0"     ; return false ; }
    if(_tPara.iGap       <=0) _tPara.iGap       = 1 ;
    if(_tPara.iSearchGap <=0) _tPara.iSearchGap = 1 ;
    if(_tPara.iSampleCnt < 8) _tPara.iSampleCnt = 8 ;
    if(!_tPara.iStartAng && !_tPara.iEndAng) {_tPara.iStartAng = 0 ; _tPara.iEndAng = 360 ;}

    int iCx     = _tCircle.cx  ;
    int iCy     = _tCircle.cy  ;
    int iInRad  = _tCircle.rad - _tPara.iSearchGap ;
    int iOutRad = _tCircle.rad + _tPara.iSearchGap ;

    float fAngUnit = (_tPara.iEndAng - _tPara.iStartAng)/(float)_tPara.iSampleCnt ;

    float fAngle = 0.0 ;
    TFPoint * pEdge   = new TFPoint[_tPara.iSampleCnt];
    TFPoint * pSttPnt = new TFPoint[_tPara.iSampleCnt];
    TFPoint * pEndPnt = new TFPoint[_tPara.iSampleCnt];
    int  iSumSttPnt = 0 ;
    int  iSumEndPnt = 0 ;

    _pRslt->bDarkCircle = true;

    for( int i = 0 ; i < _tPara.iSampleCnt ; i++) {
        fAngle = _tPara.iStartAng + fAngUnit * i ;
        MATH_GetPntFromPntByAngLen(iCx,iCy,fAngle,iInRad ,&pSttPnt[i].x , &pSttPnt[i].y);
        MATH_GetPntFromPntByAngLen(iCx,iCy,fAngle,iOutRad,&pEndPnt[i].x , &pEndPnt[i].y);

        iSumSttPnt += _pImg -> GetPixel(pSttPnt[i].x , pSttPnt[i].y);
        iSumEndPnt += _pImg -> GetPixel(pEndPnt[i].x , pEndPnt[i].y);
    }

    _pRslt->bDarkCircle = iSumSttPnt < iSumEndPnt ;
    _pRslt->iThreshold  =(iSumSttPnt + iSumEndPnt)/ (_tPara.iSampleCnt * 2) ;


    EDG_Rslt EgRslt ;

    for( int i = 0 ; i < _tPara.iSampleCnt ; i++) {
        if(!Edge.Inspect(_pImg , pSttPnt[i] , pEndPnt[i] , _tPara.EdgPara , &EgRslt)) return false ;
        pEdge[i] = _pRslt->bDarkCircle ? EgRslt.DkToLtPntS : EgRslt.LtToDkPntS ;
        m_lEdgePx.PushFrnt(pEdge[i]) ;
    }

    bool bRet = MATH_GetCircleRansac(pEdge , _tPara.iSampleCnt , _tPara.iGap , &_pRslt->fCntrX , &_pRslt->fCntrY  , &_pRslt->fRadius , &_pRslt->fScore) ;

    if(!bRet) {g_sErrMsg = "Racsac Failed." ; return false ;}

    delete [] pEdge   ;
    delete [] pSttPnt ;
    delete [] pEndPnt ;

    _pRslt->fTime = GetTickTimeDouble() - dwTime ;
    return bRet ;
}


void CCircle::PaintRslt(HDC _hHdc , CCL_Rslt *_pRslt , CCL_Disp _tDisp , float _fScaleX , float _fScaleY)
{
    CGdi *Gdi = new CGdi(_hHdc);
    Gdi -> SetScale(_fScaleX , _fScaleY);

    float fTemp ;

    if(_tDisp.clCircle != clNone){
        Gdi->m_tPen.Color = _tDisp.clCircle ;
        fTemp = MATH_RoundOff(_pRslt->fCntrX-_pRslt->fRadius) ;
        Gdi->Circle(false , MATH_RoundOff(_pRslt->fCntrX-_pRslt->fRadius) , MATH_RoundOff(_pRslt->fCntrY-_pRslt->fRadius) ,
                            MATH_RoundOff(_pRslt->fCntrX+_pRslt->fRadius) , MATH_RoundOff(_pRslt->fCntrY+_pRslt->fRadius) , 10 );
    }

    if(_tDisp.clEdge != clNone) {
        Gdi->m_tPen.Color = _tDisp.clEdge ;
        TFPoint Pnt1;
        for(int i = 0 ; i < m_lEdgePx.GetDataCnt() ; i++) {
            Pnt1 = m_lEdgePx.GetCrntData(!i);
            Gdi->Pixel(MATH_RoundOff(Pnt1.x),MATH_RoundOff(Pnt1.y),_tDisp.clEdge) ;
        }
    }

    if(_tDisp.clText != clNone) {
        int iTextX , iTextY ;
        iTextX = _pRslt->fCntrX + 5 ;
        iTextY = _pRslt->fCntrY + 5 ;
        Gdi -> m_tText.Color = _tDisp.clText ;
        Gdi -> Text(MATH_RoundOff(iTextX) , MATH_RoundOff(iTextY) , ("Score : " + AnsiString(_pRslt->fScore)).c_str());
        Gdi -> Text(iTextX , iTextY + 8  , (AnsiString("Rad : ") + AnsiString(MATH_RoundOff(_pRslt->fRadius, 2))).c_str());
        Gdi -> Text(iTextX , iTextY + 16 , (AnsiString("X : "  ) + AnsiString(MATH_RoundOff(_pRslt->fCntrX , 2))).c_str());
        Gdi -> Text(iTextX , iTextY + 24 , (AnsiString("Y : "  ) + AnsiString(MATH_RoundOff(_pRslt->fCntrY , 2))).c_str());
    }

    delete Gdi;
}


