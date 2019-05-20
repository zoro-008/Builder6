//---------------------------------------------------------------------------
#pragma hdrstop
#include "aTempMatch.h"
#include "SVMaths.h"
#include "Common.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


CTempMatch::CTempMatch()
{
//    Rslt.iDkErrPx  = 0 ;
//    Rslt.iLtErrPx  = 0 ;
//    Rslt.fInspTime = 0.0 ;
//    Rslt.DkFailPx.DeleteAll()  ;
//    Rslt.LtFailPx.DeleteAll()  ;
//    Rslt.iStartX = 0 ;
//    Rslt.iStartY = 0 ;

}

CTempMatch::~CTempMatch()
{
//    delete m_pTrainImg  ;
//    delete m_pInspArea  ;
//
//    Rslt.DkFailPx.DeleteAll()  ;
//    Rslt.LtFailPx.DeleteAll()  ;
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
    int iGrayCnt[256];

    memset(iGrayCnt , 0 , sizeof(int) * 256);

    for(int x = 0 ; x < iWidth ; x++){
        for(int y = 0 ; y < iHeight ; y++){
            iTemp = _pImg -> GetPixel(x,y);
            iGrayCnt[iTemp]++ ;
            iSumGray += iTemp ;
        }
    }

    iAvrGray = iSumGray / iAllPxCnt ;

    int i = 0 , iAutoThresh = 0;
    double dBig = 0 , dResult = 0 ;
    double dGrayPrcnt1 = 0 ;
    double dGrayPrcnt2 = 0 ;
    double dGrayW1 = 0 , dGrayU1 = 0 , dGrayA1 = 0 ,
           dGrayW2 = 0 , dGrayU2 = 0 , dGrayA2 = 0 ;

    for(i = 0 ; i <= iAvrGray ; i++){
        dGrayPrcnt1 = iGrayCnt[i]/(double)iAllPxCnt ;
        dGrayPrcnt2 = iGrayCnt[iAvrGray+1+i]/(double)iAllPxCnt ;
        dGrayW1     = dGrayW1 +  dGrayPrcnt1                ;
        dGrayW2     = dGrayW2 +  dGrayPrcnt2                ;
        dGrayA1     = dGrayA1 + (dGrayPrcnt1 *  i     )     ;
        dGrayA2     = dGrayA2 + (dGrayPrcnt2 * (i+1+i))     ;
        dGrayU1     = dGrayU1 + (dGrayA1 / dGrayW1) * (dGrayA1 / dGrayW1) ;
        dGrayU2     = dGrayU2 + (dGrayA2 / dGrayW2) * (dGrayA2 / dGrayW2) ;

        dResult = (((1-dGrayPrcnt1)*dGrayW1*dGrayU1) + (dGrayW2 * dGrayU2)) ;

        if(dBig < dResult) { dBig = dResult ; iAutoThresh = i ; }

    }

    //delete [] iGrayCnt;

    return iAutoThresh ;
}



bool CTempMatch::Train(CImage * _pImg , TRect _Rect , OCV_Para _tPara  ,
                 CArea * _pTrainArea , CImage * _pTrainImg)
{
    if(_tPara.iTrInspOft <= _tPara.iTrNoInspOft) {
        g_sErrMsg = "_tPara.iTrInspOft <= _tPara.iTrNoInspOft";
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

    if(_tPara.iTrThreshold==0)_tPara.iTrThreshold = AutoThreshold(_pTrainImg);

    TCircle Circle ;

    //Set Temp Insp Area.
    Circle.rad = _tPara.iTrInspOft ;
    for(int y = 0 ; y < iHeight ; y++){
        bPrePx = _pImg -> GetPixel(iLeft,iTop+y) > _tPara.iTrThreshold;
        for(int x = 0 ; x < iWidth ; x++){
            bPx = _pImg -> GetPixel(iLeft+x,iTop+y) > _tPara.iTrThreshold ;
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
        bPrePx = _pImg -> GetPixel(iLeft+x,iTop) > _tPara.iTrThreshold ;
        for(int y = 0 ; y < iHeight ; y++){
            bPx = _pImg -> GetPixel(iLeft+x,iTop+y) > _tPara.iTrThreshold ;
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
    Circle.rad = _tPara.iTrNoInspOft ;
    for(int y = 0 ; y < iHeight ; y++){
        bPx = _pImg -> GetPixel(iLeft,iTop+y) > _tPara.iTrThreshold ;
        for(int x = 0 ; x < iWidth ; x++){
            bPx = _pImg -> GetPixel(iLeft+x,iTop+y) > _tPara.iTrThreshold ;
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
        bPx = _pImg -> GetPixel(iLeft+x,iTop) > _tPara.iTrThreshold ;
        for(int y = 0 ; y < iHeight ; y++){
            bPx = _pImg -> GetPixel(iLeft+x,iTop+y) > _tPara.iTrThreshold ;
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
            if(_pImg -> GetPixel(iLeft+x,iTop+y) > _tPara.iTrThreshold )_pTrainArea -> SetPixel(x,y,otLtInsp ) ;
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


//CImage * _pImg , CArea * _pTrainArea , CImage * _pTrainImg , TRect _tInspRect , OCV_Para _tPara ,
//                   CArea * _pRsltArea , OCV_Rslt * _pRslt)

bool CTempMatch::Inspect(CImage * _pImg , CArea * _pTrainArea , CImage * _pTrainImg , TRect _tInspRect , OCV_Para _tPara ,
                   CArea * _pRsltArea , OCV_Rslt * _pRslt)
//double NCC(image_ptr buf, const int *rows, const int *cols, image_ptr temp, const int *tRows, const int *tCols, int *retX, int *retY)
{
/*
    if(_pImg      == NULL) return false ;
    if(_pTrainImg == NULL) return false ;

    int iImgWidth  = _pImg -> GetWidth () ;
    int iImgHeight = _pImg -> GetHeight() ;
    int iTrnWidth  = _pTrainImg -> GetWidth() ;
    int iTrnHeight = _pTrainImg -> GetHeight() ;

    if(iImgWidth  <= 0 ) return false ;
    if(iImgHeight <= 0 ) return false ;
    if(iTrnWidth  <= 0 ) return false ;
    if(iTrnHeight <= 0 ) return false ;

    int iTrnSum       = 0 ;
    double dTrnAvr    = 0.0 ;
    int iDiffSqur     = 0 ;
    int iTemp         = 0 ;
    double dTemp      = 0.0 ;

    //트레인한 템플릿 영상 평균.
    for(int iTrnY = 0; iTrnY < iTrnHeight ; iTrnY++)
        for(int iTrnX = 0; iTrnX < iTrnWidth ; iTrnX++) {
            iTrnSum += _pTrainImg -> GetPixel(iTrnX , iTrnY);
        }
    }
    dTrnAvr = iTrnSum / (double)(iTrnHeight * iTrnWidth) ; //속도때문에 소수점 무시.

    //(Txy-Tavr)^2
    for(u = 0; u < *tRows; ++u){
        for(v = 0; v < *tCols; ++v){
            dTemp = _pT
            iDiffSqur += (((double)*(temp + u*(*tCols) + v)) - meanTemp)*(((double)*(temp + u*(*tCols) + v)) - meanTemp);
        }
    }


    int u, v;
    double meanBuf = 0.0, meanTemp = 0.0;
    double numer = 0.0;
    double denom = 0.0;
    double meanSub = 0.0;
    double NCC = 0.0;
    double maxNCC = 0.0;
    double numTempPixel = (*tRows)*(*tCols);


    //트레인한 템플릿 영상 평균.
    for(u = 0; u < *tRows; ++u)
        for(v = 0; v < *tCols; ++v)
            meanTemp += ((double)*(temp + u*(*tCols) + v))/numTempPixel;

    //(T-Tavr)^2
    for(u = 0; u < *tRows; ++u)
        for(v = 0; v < *tCols; ++v)
            meanSub += (((double)*(temp + u*(*tCols) + v)) - meanTemp)*(((double)*(temp + u*(*tCols) + v)) - meanTemp);

    for(i = 0; i < (*rows - *tRows); ++i){
        for(j = 0; j < (*cols - *tCols); ++j){

            // get mean of the image which is underneath the template
            meanBuf = 0.0;
            for(u = 0; u < *tRows; ++u)
                for(v = 0; v < *tCols; ++v)
                    //meanBuf += ((double)*(buf + (i+u)*(*cols) + (j+v)))/numTempPixel;
                    meanBuf += ((double)*(buf + (i+u)*(*cols) + (j+v)))/numTempPixel;


            // get correlation
            NCC = 0.0;
            numer = 0.0;
            denom = 0.0;
            for(u = 0; u < *tRows; ++u){
                for(v = 0; v < *tCols; ++v){
                    numer += (((double)*(buf + (i+u)*(*cols) + (j+v))) - meanBuf)*(((double)*(temp + u*(*tCols) + v)) - meanTemp);
                    denom += (((double)*(buf + (i+u)*(*cols) + (j+v))) - meanBuf)*(((double)*(buf + (i+u)*(*cols) + (j+v))) - meanBuf);
                }
            }
            NCC = numer/sqrt(denom*meanSub);
            if(maxNCC < NCC){
                maxNCC = NCC;
                *retX = j;
                *retY = i;
            }
        }
    }
    return maxNCC; */

}

/*
bool CTempMatch::Inspect(CImage * _pImg , CArea * _pTrainArea , CImage * _pTrainImg , TRect _tInspRect , OCV_Para _tPara ,
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
                _pRslt -> iCntPosX     = iLeft + rx + _pTrainArea -> GetWidth () / 2 ;
                _pRslt -> iCntPosY     = iTop  + ry + _pTrainArea -> GetHeight() / 2 ;                         //화이트 나 블랙 바탕에서 무조건 50프로 이상 나오는 것 때문에 줄임. 50프로 미만은 고만 고만 해서 무시함.
                _pRslt -> fSinc        = ((iInspPxCnt*256 - iMinErrCnt) / (float)(iInspPxCnt*256)) * 100.0 ;
                _pRslt -> tRect.left   = iLeft + rx ;
                _pRslt -> tRect.top    = iTop  + ry ;
                _pRslt -> tRect.right  = iLeft + rx + _pTrainArea -> GetWidth () ;
                _pRslt -> tRect.bottom = iTop  + ry + _pTrainArea -> GetHeight() ;



            }
        }
    }

    //iErrCntLmt = iInspPxCnt - (iInspPxCnt * _tPara.fSinc / 100) ;



    delete [] tInspPxPnt ;


    return true ;
}*/

void CTempMatch::PaintRslt(HDC _hHdc , CArea * _pRsltArea , OCV_Rslt * _pRslt, OCV_Disp _tDisp, float _fScaleX , float _fScaleY)
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
        sTemp = AnsiString("X : ") + _pRslt->iCntPosX + AnsiString(" Y : ") + _pRslt->iCntPosY + AnsiString(" Sinc : ") + sTemp ;
        Gdi -> Text(_pRslt->tRect.left + 5, _pRslt->tRect.top - 15 ,sTemp.c_str()) ;
    }

    if(_tDisp.clCenter != clNone) {
        Gdi -> m_tPen.Color = _tDisp.clCenter ;
        Gdi -> LineCross(_pRslt->iCntPosX , _pRslt->iCntPosY , 10) ;
    }

    if(_tDisp.clRect != clNone) {
        Gdi -> m_tPen.Color = _tDisp.clRect ;
        Gdi -> Rect(false , _pRslt->tRect);

    }

    delete Gdi;

}












