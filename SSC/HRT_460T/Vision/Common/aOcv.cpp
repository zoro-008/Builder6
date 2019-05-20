//---------------------------------------------------------------------------
#pragma hdrstop
#include "aOcv.h"
#include "SVMaths.h"
#include "Common.h"
#include "SLogUnit.h"

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

    Trace("TrainOCV","BfTrim");
    _pImg -> Trim(_Rect , _pTrainImg);
    Trace("TrainOCV","AtTrim");

    bool bEdge  = false ;
    bool bPrePx = _pImg -> GetPixel(iLeft,iTop) ;
    bool bPx    = _pImg -> GetPixel(iLeft,iTop) ;

    Trace("TrainOCV","Bf AutoThreshold");
    if(_tTrPr.iThreshold==0)_tTrPr.iThreshold = AutoThreshold(_pTrainImg);
    Trace("TrainOCV","At AutoThreshold");

    TCircle Circle ;

    //Set Temp Insp Area.
    Circle.rad = _tTrPr.iInspOft ;
    for(int y = 0 ; y < iHeight ; y++){
        bPrePx = _pImg -> GetPixel(iLeft,iTop+y) > _tTrPr.iThreshold;
        for(int x = 0 ; x < iWidth ; x++){
            //Trace("TrainOCV",("1 x:" + String(x) + "y:"+String(y)).c_str()  );
            bPx = _pImg -> GetPixel(iLeft+x,iTop+y) > _tTrPr.iThreshold ;
            bEdge = bPx != bPrePx ;

            if(bEdge) {
                Circle.cx = x ;
                Circle.cy = y ;
                _pTrainArea -> AddCircle(Circle , otTemp); //일단 밝기 구분 하기전에 검사영역만 설정.
            }
            bPrePx = bPx ;
        }
    }
    for(int x = 0 ; x < iWidth ; x++){
        bPrePx = _pImg -> GetPixel(iLeft+x,iTop) > _tTrPr.iThreshold ;
        for(int y = 0 ; y < iHeight ; y++){
            //Trace("TrainOCV",("2 x:" + String(x) + "y:"+String(y)).c_str()  );
            bPx = _pImg -> GetPixel(iLeft+x,iTop+y) > _tTrPr.iThreshold ;
            bEdge = bPx != bPrePx ;

            if(bEdge) {
                Circle.cx = x ;
                Circle.cy = y ;
                _pTrainArea -> AddCircle(Circle , otTemp); //일단 밝기 구분 하기전에 검사영역만 설정.
            }
            bPrePx = bPx ;
        }
    }

    //Set Temp Insp Area.
    Circle.rad = _tTrPr.iNoInspOft ;
    for(int y = 0 ; y < iHeight ; y++){
        bPx = _pImg -> GetPixel(iLeft,iTop+y) > _tTrPr.iThreshold ;
        for(int x = 0 ; x < iWidth ; x++){
            //Trace("TrainOCV",("3 x:" + String(x) + "y:"+String(y)).c_str()  );
            bPx = _pImg -> GetPixel(iLeft+x,iTop+y) > _tTrPr.iThreshold ;
            bEdge = bPx != bPrePx ;
            if(bEdge) {
                Circle.cx = x ;
                Circle.cy = y ;
                _pTrainArea -> AddCircle(Circle , otUnknown); //검사 마진 때문에 가운데를 속아낸다.
            }
            bPrePx = bPx ;
        }
    }
    for(int x = 0 ; x < iWidth ; x++){
        bPx = _pImg -> GetPixel(iLeft+x,iTop) > _tTrPr.iThreshold ;
        for(int y = 0 ; y < iHeight ; y++){
            //Trace("TrainOCV",("4 x:" + String(x) + "y:"+String(y)).c_str()  );
            bPx = _pImg -> GetPixel(iLeft+x,iTop+y) > _tTrPr.iThreshold ;
            bEdge = bPx != bPrePx ;
            if(bEdge) {
                Circle.cx = x ;
                Circle.cy = y ;
                _pTrainArea -> AddCircle(Circle , otUnknown); //가운데를 속아낸다.
            }
            bPrePx = bPx ;
        }
    }

    //Separate Light,Dark Area.
    for(int y = 0 ; y < iHeight ; y++){
        for(int x = 0 ; x < iWidth ; x++){
            //Trace("TrainOCV",("5 x:" + String(x) + "y:"+String(y)).c_str()  );
            if(_pTrainArea -> GetPixel(x,y) != otTemp) continue ;
            if(_pImg -> GetPixel(iLeft+x,iTop+y) > _tTrPr.iThreshold )_pTrainArea -> SetPixel(x,y,otLtInsp ) ; //분류...
            else                                                      _pTrainArea -> SetPixel(x,y,otDkInsp ) ;
        }
    }





    /*
    enum OCV_TRAIN_AREA {
        otUnknown = 0 ,
        otNoInsp  = 1 ,
        otDkInsp  = 2 ,
        otLtInsp  = 3 ,
        otTemp    = 4 ,
        otInsp    = 5 ,

        MAX_OCV_TRAIN_AREA
    };
    */







    //여기부터는 검사영역을 줄이는 옵션에 따라 검사 포인트 추림.

    //Visited Pixel



                                  // 1    , LOOT_2 , 1    , LOOT_2 , 1     , LOOT_2, 1    , LOOT_2
    enum EN_NEI_ID                         { niR =0, niRT=1, niT=2 , niLT=3 , niL=4 , niLB=5, niB=6, niRB=7, MAX_NEI_ID };
    register const POINT nei[MAX_NEI_ID] = { {1, 0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1} };
    unsigned char * pVisitPx = new unsigned char[iHeight * iWidth] ;
    memset(pVisitPx , 0 , iHeight * iWidth) ;
    #define VPixel(x,y) (*(pVisitPx + x + iWidth * y))

    unsigned char * pIgnrPx = new unsigned char[iHeight * iWidth] ;
    memset(pIgnrPx , 0 , iHeight * iWidth) ;
    #define IPixel(x,y) (*(pIgnrPx + x + iWidth * y))

    int spx    ; // start X
    int spy    ; // start y
    int tx     ; // Tracker X
    int ty     ; // Tracker Y
    int iSkipCnt ; //검사를 빠르게 할목적으로 뜨문뜨문 검사하게 하는것.
    int iSkipPer ; //나머지값.
    int u,v ;

    int n      ; //검색시작 방향.
    const int iSkipCntOptn = _tTrPr.iInspFreq ;  //_tPara.iInspFreq를 TrPa로 옮기기가 기존 잡파일들이 있어서 애매함.
    bool findNei = false ;


    //뜨문뜨문 검사인데 검사영역이 (iInspOft - iNoInspOft == 2)2줄 까지만 적용되고 그이상은 의미가 거의 없다.
    if(iSkipCntOptn > 2) {
        for(int y = 0 ; y < iHeight ; y++){
            for(int x = 0 ; x < iWidth ; x++){
                Trace("TrainOCV",("6 x:" + String(x) + "y:"+String(y)).c_str()  );
                if(_pTrainArea -> GetPixel(x,y) != otLtInsp&&
                   _pTrainArea -> GetPixel(x,y) != otDkInsp) continue ;

                if (VPixel(x,y)) continue ; //이미 방문했던곳이면 무시.






                int k ;
                iSkipCnt = 0 ;
                n= niLB ;//진행픽셀의 왼쪽아래부터 시계반대방향으로 검색.
                spx = tx = x ;
                spy = ty = y ;

                do {
                    //데이터 지움.
                    if (!VPixel(tx,ty)) { //선이다 보니 한번 트레킹 한곳을 다시 돌아 온다. 돌아 올때는 이미 걸른곳이라 안걸르게함.
                        iSkipPer = iSkipCnt % iSkipCntOptn ;
                        if(iSkipPer) {
                            IPixel(tx,ty) = true ;
                            //_pTrainArea->SetPixel(tx,ty,otUnknown);//검사영역 해제.
                        }
                        iSkipCnt++;
                    }

                    //방문했다는 도장찍고
                    VPixel(tx,ty) = true;

                    //Find next pixel.
                    for (k=0; k<8; k++, n=((n+1) & 7)) {
                        u = tx + nei[n].x ;
                        v = ty + nei[n].y ;
                        if (u<0       || v< 0      ) { continue; }
                        if (u>=iWidth || v>=iHeight) { continue; }
                        //if (VPixel(tx,ty)          ) { continue; }
                        if (otLtInsp == _pTrainArea->GetPixel(u,v) ||
                            otDkInsp == _pTrainArea->GetPixel(u,v) ){
                            break;
                        }
                    }

                    tx = u ;
                    ty = v ;
                    n = (n+5) & 7;

                }
                while(!(spx==tx && spy==ty));

                /* 무한루프에 빠지는 문제.
                                do {
                    //Find next pixel.
                    for (k=0; k<8; k++, n=((n+1) & 7)) {
                        u = tx + nei[n].x ;
                        v = ty + nei[n].y ;
                        if (u>=iWidth || v>=iHeight) { continue; }
                        if (otLtInsp == _pTrainArea->GetPixel(u,v) ||
                            otDkInsp == _pTrainArea->GetPixel(u,v) ){
                            break;
                        }
                    }


                    if (!VPixel(tx,ty)) { //선이다 보니 한번 트레킹 한곳을 다시 돌아 온다. 돌아 올때는 이미 걸른곳이라 안걸르게함.
                        iSkipPer = iSkipCnt % iSkipCntOptn ;
                        if(iSkipPer) {
                            _pTrainArea->SetPixel(tx,ty,otUnknown);//검사영역 해제.
                        }

                        VPixel(tx,ty) = true;
                        iSkipCnt++;
                    }

                    tx = u ;
                    ty = v ;
                    n = (n+5) & 7;

                }
                while(!(spx==tx && spy==ty)&& k<8);
                */
            }
        }

        for(int y = 0 ; y < iHeight ; y++){
            for(int x = 0 ; x < iWidth ; x++){
                if(IPixel(x,y)) _pTrainArea->SetPixel(x,y,otUnknown);//검사영역 해제.
            }
        }
    }

    delete [] pVisitPx ;
    delete [] pIgnrPx  ;
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
    //if(_tPara.iInspFreq < 1)  _tPara.iInspFreq = 1 ;
    for(int y = 0 ; y < _pTrainArea -> GetHeight() ; y++) {
        for(int x = 0 ; x < _pTrainArea -> GetWidth() ; x++) {
            //if(x%_tPara.iInspFreq == 0 || y%_tPara.iInspFreq == 0) { 트레인 쪽으로 이동.
                if(_pTrainArea -> GetPixel(x , y) == otDkInsp || _pTrainArea -> GetPixel(x , y) == otLtInsp ){
                    cPx =  _pTrainImg -> GetPixel(x , y) ;
                    llInspPnt.PushBack(TPxPoint(x,y,cPx));
                    _pRsltArea -> SetPixel(x,y,orInsp);
                    iInspPxSum+= cPx;
                }
            //}

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
 */
