//---------------------------------------------------------------------------
#pragma hdrstop

#include "aCalibration.h"
#include "GdiUnit.h"
#include "SVMaths.h"
#include "Common.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

CCalibration::CCalibration()
{
}

CCalibration::~CCalibration()
{
}

bool CCalibration::Inspect(CImage *_pImg , TRect _tRect , CAL_Para _tPara , CAL_Rslt * _pRslt)
{
    DWORD dwTime =GetTickTimeDouble();
    g_sErrMsg = "";

    //Para Check & Err Check
    if(_pImg == NULL            ) {g_sErrMsg = "Image Buffer is NULL."    ; return false ; }
    if(_pImg -> SetRect(&_tRect)) {g_sErrMsg = "RectL Overed Image Size." ; return false ; }

    if(_tPara.fDotSize  <= 0) {g_sErrMsg = "DotSize is 0" ; return false ; }
    if(_tPara.fDotPitch <= 0) {g_sErrMsg = "DotPitch is 0" ; return false ; }

    //memset(_pRslt , 0 , sizeof(OCV_Rslt)); 블럽때문에 뻑남.
    _pRslt-> fXPxRes = 0.0 ;
    _pRslt-> fYPxRes = 0.0 ;


    _tPara.BlbPara.bIgnrSide = true;

    if(!Blob.Inspect(_pImg , _tRect , _tPara.BlbPara , _tPara.BlbFltr , &_pRslt->BlbRslt)) return false ;

    if( _pRslt->BlbRslt.iBlobCnt < 4) { g_sErrMsg = "Needed more Points than 4" ; return false ; }

    int iRowCnt = 0 ;
    int iColCnt = 0 ;
    int iRowBandTop ;
    int iRowBandBtm ;
    int iColBandLft ;
    int iColBandRgt ;

    SBlob Blob1 = _pRslt->BlbRslt.pBlobs[0] ;

    iRowBandTop = Blob1.top    ;
    iRowBandBtm = Blob1.bottom ;
    iColBandLft = Blob1.left   ;
    iColBandRgt = Blob1.right  ;
    int iFirstRowY  = Blob1.iCenterY ;
    int iLastRowY ;

    double dSumX = 0 ;
    int    iCntX = 0 ;
    double dSumY = 0 ;
    double dSumXAngle = 0 ;
    double dSumYAngle = 0 ;
    int    iCntY = 0 ;
    float  fAngle  ;
    float  fAngleRad ;
    TPoint FstBlobCntr ;
    TPoint LstBlobCntr ;

    //Para
    for (register int i = 0 ; i < _pRslt->BlbRslt.iBlobCnt ; i++ ) {
        Blob1 = _pRslt->BlbRslt.pBlobs[i] ;
        if(iRowBandTop < Blob1.iCenterY && iRowBandBtm > Blob1.iCenterY) iColCnt++ ;
        if(iColBandLft < Blob1.iCenterX && iColBandRgt > Blob1.iCenterX) iRowCnt++ ;
        iLastRowY = Blob1.iCenterY ;
    }

    if( _pRslt->BlbRslt.iBlobCnt != iColCnt * iRowCnt){g_sErrMsg = "There is Some Noise or Angle is Wrong!"  ; return false ; }
    if( iRowCnt < 2)                                  {g_sErrMsg = "Needed more Row than2" ; return false ; }
    if( iColCnt < 2)                                  {g_sErrMsg = "Needed more Col than2" ; return false ; }

    _tPara.BlbPara.iStartXOfs = 0 ;
    _tPara.BlbPara.iStartYOfs = iFirstRowY - _tRect.top ;
    _tPara.BlbPara.iPitchX    = 1 ;
    _tPara.BlbPara.iPitchY    = (iLastRowY - iFirstRowY)/ (iRowCnt -1) ;

    if(!Blob.Inspect(_pImg , _tRect , _tPara.BlbPara , _tPara.BlbFltr , &_pRslt->BlbRslt)) return false ;

    //켈 패드의 틀어진 각도 계산.
    FstBlobCntr.x = _pRslt->BlbRslt.pBlobs[0].iCenterX ;
    FstBlobCntr.y = _pRslt->BlbRslt.pBlobs[0].iCenterY ;
    LstBlobCntr.x = _pRslt->BlbRslt.pBlobs[iColCnt-1].iCenterX ;
    LstBlobCntr.y = _pRslt->BlbRslt.pBlobs[iColCnt-1].iCenterY ;

    fAngle = MATH_GetLineAngle(FstBlobCntr.x , FstBlobCntr.y , LstBlobCntr.x , LstBlobCntr.y);

    fAngleRad =(float)(fAngle*MATH_PI/180.0);//angle->radian으로 변환




    //검증 필요.
    for (register int y = 0 ; y < iRowCnt - 1; y++ ) {
        for (register int x = 0 ; x < iColCnt - 1; x++ ) {
            dSumX += _pRslt->BlbRslt.pBlobs[ iColCnt *  y + x  + 1 ].iCenterX - _pRslt->BlbRslt.pBlobs[ iColCnt * y + x ].iCenterX ; iCntX++ ;
            dSumY += _pRslt->BlbRslt.pBlobs[ iColCnt * (y + 1) + x ].iCenterY - _pRslt->BlbRslt.pBlobs[ iColCnt * y + x ].iCenterY ; iCntY++ ;
        }
    }

    dSumXAngle = dSumX * cos(fAngleRad);
    dSumYAngle = dSumY * cos(fAngleRad);

    _pRslt->  fXPxRes = _tPara.fDotPitch / (dSumXAngle / (float)iCntX) ;
    _pRslt->  fYPxRes = _tPara.fDotPitch / (dSumYAngle / (float)iCntY) ;

    _pRslt->fInspTime = MATH_RoundOff(GetTickTimeDouble()- dwTime , 2) ;
    return true ;
}

void CCalibration::PaintRslt(HDC _hHdc , CAL_Rslt *_pRslt , CAL_Disp _tDisp , float _fScaleX , float _fScaleY )
{//여기부터.
    CGdi *Gdi = new CGdi(_hHdc);
    Gdi -> SetScale(_fScaleX , _fScaleY);

    Blob.PaintRslt(_hHdc , &_pRslt->BlbRslt , _tDisp.BlbDisp , _fScaleX , _fScaleY );


//    SBlob  blob  ;
    TPoint point ;
    AnsiString sTemp ;



    if(_tDisp.clText != clNone) {
        Gdi->m_tPen .Color = _tDisp.clText ;
        Gdi->m_tText.Color = _tDisp.clText ;
        sTemp.sprintf("xRes=%.5f yRes=%.5f",_pRslt->fXPxRes , _pRslt->fYPxRes);
        if(_pRslt->BlbRslt.iBlobCnt)Gdi -> Text( _pRslt->BlbRslt.pBlobs[0].left , _pRslt->BlbRslt.pBlobs[0].top - 15 , sTemp.c_str() ) ;
    }



    delete Gdi;
}


