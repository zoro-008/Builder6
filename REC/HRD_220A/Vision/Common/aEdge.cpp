
//---------------------------------------------------------------------------
#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------
#include "aEdge.h"
#include "BaseMaths.h"

CEdge::CEdge()
{
}

CEdge::~CEdge()
{

}

//서브픽셀 오프셑 구하는 공식 루트 연산 추가.
//double GetSubOffsetS(double ValMinPos , double ValMidPos , double  ValMaxPos) //x,y 값중 작은 포지션의 픽셀벨류 , x,y 값중 중간 포지션의 픽셀벨류 , x,y 값중 큰 포지션의 픽셀벨류
//{
//    //Sqrt.
//    double a = ValMidPos - ValMinPos ;
//    double b = ValMaxPos - ValMidPos ;
//
//    if(b+a == 0) return 0.0 ;
//
//    double c = (b-a)/(b+a) ;
//    bool d = c<0 ;
//    if(d) c = c * -1 ;
//    double e = sqrt(c);
//    double f = e/2.0 ;
//    if(d) f = f * -1 ;
//
//    return f ;
//
//
//}

//서브 픽셀 오프셑 구하는 공식. 실패.
//x,y 값중 작은 포지션의 픽셀벨류 , x,y 값중 중간 포지션의 픽셀벨류 , x,y 값중 큰 포지션의 픽셀벨류
double GetSubOffsetFail(double _fFstPosVal , double _fMidPosVal , double _fLstPosVal , double _fPxLength)
{
    bool  bDkToLt ;
    bool  bGap    ;
    double fMidPosVal ;
    double fRatio ;

    bDkToLt = _fFstPosVal < _fLstPosVal ;

    if(bDkToLt) {
        fMidPosVal = _fMidPosVal ;
        if(fMidPosVal < _fFstPosVal) fMidPosVal = _fFstPosVal ;
        if(fMidPosVal > _fLstPosVal) fMidPosVal = _fLstPosVal ;

        fRatio = ((fMidPosVal - _fFstPosVal) / (_fLstPosVal - _fFstPosVal)) - 0.5 ;
    }


    else {
        fMidPosVal = _fMidPosVal ;
        if(fMidPosVal > _fFstPosVal) fMidPosVal = _fFstPosVal ;
        if(fMidPosVal < _fLstPosVal) fMidPosVal = _fLstPosVal ;

        fRatio = ((fMidPosVal - _fLstPosVal) / (_fFstPosVal - _fLstPosVal)) - 0.5 ;
    }




    return fRatio * _fPxLength;

}

//서브 픽셀 오프셑 구하는 공식.
//엦지에서 -1 , 0 , 1 포지션의 diff 값 과 픽셀과 픽셀간의 거리.
double GetSubOffset(double _fFstPosDif , double _fMidPosDif , double _fLstPosDif , double _fPxLength)
{
    //나중에 타당성 검토.
    if(2*(_fLstPosDif+_fFstPosDif-2*_fMidPosDif) == 0){
        return 0.0 ;
    }
    
    //3Point Parabola Fit.
    double fOffset = (_fFstPosDif - _fLstPosDif)/(2*(_fLstPosDif+_fFstPosDif-2*_fMidPosDif)) ;

    return fOffset * _fPxLength ;
}




bool CEdge::Inspect(CImage * _pImg  , TPoint  _tSttPnt , TPoint  _tEndPnt , TPara _tPara , TRslt * _pRslt)
{
    DWORD dwTime =GetTime();

    _pRslt -> Clear();

    //Para Check & Err Check
    if(_pImg == NULL                                                     ) {_pRslt->sErrMsg = "Image Buffer is NULL."; return false ;}
    if(_pImg -> SetLine(&_tSttPnt , &_tEndPnt , _tPara.iAverage)         ) {_pRslt->sErrMsg = "Image Range Over"     ; return false ;}
    if(_pImg -> SetLine(&_tSttPnt , &_tEndPnt , _tPara.iCompare)         ) {_pRslt->sErrMsg = "Image Range Over"     ; return false ;}
    if(CMath::GetLineLenth(_tSttPnt.x,_tSttPnt.y,_tEndPnt.x,_tEndPnt.y) < 5) {_pRslt->sErrMsg = "Range is too short"   ; return false ;}

    if(_tPara.iAverage < 1) {_tPara.iAverage = 1 ;}
    if(_tPara.iCompare < 1) {_tPara.iCompare = 1 ;}


    BYTE * pRaw ; //로우 데이터.
    BYTE * pAvr ; //평균 데이터.
    int  * pDif ; //차이 데이터(1차미분)

    bool bThin  = abs(_tSttPnt.x - _tEndPnt.x) < abs(_tSttPnt.y - _tEndPnt.y);
    int  iRange = bThin ? _tEndPnt.y - _tSttPnt.y : _tEndPnt.x - _tSttPnt.x ; //요놈이 -값이면 반대방향연산.

    pRaw = new BYTE[abs(iRange)] ;
    pAvr = new BYTE[abs(iRange)] ;
    pDif = new int [abs(iRange)] ;
    BYTE byTemp ;

    //Raw Data Getting.
    double  SubPos ;
    enum ELineAngle {la0,la90,la180,la270,la45_135,la135_225,la225_315,la315_45} laLineAngle;
         if( bThin && _tEndPnt.x == _tSttPnt.x && iRange > 0) laLineAngle = la270     ;
    else if( bThin && _tEndPnt.x == _tSttPnt.x && iRange < 0) laLineAngle = la90      ;
    else if( bThin && _tEndPnt.x != _tSttPnt.x && iRange > 0) laLineAngle = la225_315 ;
    else if( bThin && _tEndPnt.x != _tSttPnt.x && iRange < 0) laLineAngle = la45_135  ;
    else if(!bThin && _tEndPnt.y == _tSttPnt.y && iRange > 0) laLineAngle = la0       ;
    else if(!bThin && _tEndPnt.y == _tSttPnt.y && iRange < 0) laLineAngle = la180     ;
    else if(!bThin && _tEndPnt.y != _tSttPnt.y && iRange > 0) laLineAngle = la315_45  ;
    else if(!bThin && _tEndPnt.y != _tSttPnt.y && iRange < 0) laLineAngle = la135_225 ;
    switch(laLineAngle) {
        case la270     : for(int i = 0 ; i < iRange ; i++){pRaw[ i] = _pImg -> GetPixel(_tSttPnt.x,_tSttPnt.y+i) ;} break ;
        case la90      : for(int i = 0 ; i > iRange ; i--){pRaw[-i] = _pImg -> GetPixel(_tSttPnt.x,_tSttPnt.y+i) ; } break ;
        case la225_315 : for(int i = 0 ; i < iRange ; i++){pRaw[ i] = _pImg -> GetSubPixel(CMath::GetLineX(_tSttPnt,_tEndPnt,_tSttPnt.y+i),_tSttPnt.y+i);}break ;
        case la45_135  : for(int i = 0 ; i > iRange ; i--){pRaw[-i] = _pImg -> GetSubPixel(CMath::GetLineX(_tSttPnt,_tEndPnt,_tSttPnt.y+i),_tSttPnt.y+i);}break ;

        case la0       :
            for(int i = 0 ; i < iRange ; i++){
                byTemp   = _pImg -> GetPixel(_tSttPnt.x+i,_tSttPnt.y) ;
                pRaw[ i] = byTemp ;
            } break ;
        case la180     : for(int i = 0 ; i > iRange ; i--){pRaw[-i] = _pImg -> GetPixel(_tSttPnt.x+i,_tSttPnt.y) ;} break ;
        case la315_45  : for(int i = 0 ; i < iRange ; i++){pRaw[ i] = _pImg -> GetSubPixel(_tSttPnt.x+i,CMath::GetLineY(_tSttPnt,_tEndPnt,_tSttPnt.x+i));}break ;
        case la135_225 : for(int i = 0 ; i > iRange ; i--){pRaw[-i] = _pImg -> GetSubPixel(_tSttPnt.x+i,CMath::GetLineY(_tSttPnt,_tEndPnt,_tSttPnt.x+i));}break ;
    }


    //Avr Data Getting.
    int iSum ;
    int iDataCnt ;
    for(int i = 0 ; i < abs(iRange) ; i++) {
        iSum     = 0.0 ;
        iDataCnt = 0   ;
       for(int j = (-_tPara.iAverage) ; j <= _tPara.iAverage ; j++) {
            if(i + j < 0           ) continue ;
            if(i + j >= abs(iRange)) continue ;
            iSum += pRaw[i+j];
            iDataCnt++;
        }
        pAvr[i] = iSum / iDataCnt ;
    }

    //Diffrence Data Getting.
    const bool bCenterType = true ; //CenterType은 현재 픽셀에서 좌우 비교, 다른 타입은 현채 픽셀에서 앞으로 몇 픽셀 가서 비교.
    memset(pDif , 0 , sizeof(int)*abs(iRange));
    int iTemp1 , iTemp2 , iTemp3 ;
    if(bCenterType) {
        for(int i = _tPara.iCompare ; i < abs(iRange) - _tPara.iCompare ; i++) {
            iTemp1 = pAvr[i + _tPara.iCompare] ;
            iTemp2 = pAvr[i - _tPara.iCompare] ;
            iTemp3 = iTemp1 - iTemp2 ;
            pDif[i] = iTemp3 ;
            //pDif[i] = pAvr[i + _tPara.iCompare] - pAvr[i - _tPara.iCompare] ;
        }
    }
    else {
        _tPara.iCompare = 2 ;
        for(int i = 0 ; i < abs(iRange) - _tPara.iCompare ; i++) {
            pDif[i] = pAvr[i + _tPara.iCompare] - pAvr[i] ;
        }
    }

    //Get Edge.
    int   iDkToLtPos = 1 ;   //밑에서 서브픽셀 찾을때 언더런 나서.. 수정.
    int   iLtToDkPos = 1 ;
    int   iLtToDkVal = 1 ;
    int   iDkToLtVal = 1 ;

    /*
    switch(laLineAngle) {
        case la270     : for(int i = 0 ; i < iRange ; i++){pRaw[ i] = _pImg -> GetPixel(_tSttPnt.x,_tSttPnt.y+i) ;} break ;
        case la90      : for(int i = 0 ; i > iRange ; i--){pRaw[-i] = _pImg -> GetPixel(_tSttPnt.x,_tSttPnt.y+i) ; } break ;
        case la225_315 : for(int i = 0 ; i < iRange ; i++){pRaw[ i] = _pImg -> GetSubPixel(CMath::GetLineX(_tSttPnt,_tEndPnt,_tSttPnt.y+i),_tSttPnt.y+i);}break ;
        case la45_135  : for(int i = 0 ; i > iRange ; i--){pRaw[-i] = _pImg -> GetSubPixel(CMath::GetLineX(_tSttPnt,_tEndPnt,_tSttPnt.y+i),_tSttPnt.y+i);}break ;

        case la0       : for(int i = 0 ; i < iRange ; i++){pRaw[ i] = _pImg -> GetPixel(_tSttPnt.x+i,_tSttPnt.y) ;} break ;
        case la180     : for(int i = 0 ; i > iRange ; i--){pRaw[-i] = _pImg -> GetPixel(_tSttPnt.x+i,_tSttPnt.y) ;} break ;
        case la315_45  : for(int i = 0 ; i < iRange ; i++){pRaw[ i] = _pImg -> GetSubPixel(_tSttPnt.x+i,CMath::GetLineY(_tSttPnt,_tEndPnt,_tSttPnt.x+i));}break ;
        case la135_225 : for(int i = 0 ; i > iRange ; i--){pRaw[-i] = _pImg -> GetSubPixel(_tSttPnt.x+i,CMath::GetLineY(_tSttPnt,_tEndPnt,_tSttPnt.x+i));}break ;
    }
    */
    unsigned char OffsetPx ;
    const int iRangeEnd = abs(iRange)-1 ; //밑에서 서브픽셀 찾을때 언더런 나서.. 수정.
    const int iRangeStt = 1             ;

    //상하 씨리즈
    if(laLineAngle == la270     ) { //아래로 수직인 직선.
        for(int i = iRangeStt ; i < iRangeEnd ; i++) {
            //현재 픽셀의 밝기 채크
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //오프셑 준 픽셀의 밝기 체크.
            OffsetPx = _pImg -> GetPixel(_tSttPnt.x,_tSttPnt.y+i+_tPara.iOfsGap) ;
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }
    else if(laLineAngle == la90      ) { //위로 수직인 직선.
        for(int i = iRangeStt ; i < iRangeEnd ; i++) {
            //현재 픽셀의 밝기 채크
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //오프셑 준 픽셀의 밝기 체크.
            OffsetPx = _pImg -> GetPixel(_tSttPnt.x,_tSttPnt.y-i-_tPara.iOfsGap) ;
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }
    else if(laLineAngle == la225_315 ){ //밑으로 향하는 선.
        for(int i = iRangeStt ; i < iRangeEnd ; i++) {
            //현재 픽셀의 밝기 채크
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //오프셑 준 픽셀의 밝기 체크.
            OffsetPx = _pImg -> GetSubPixel(CMath::GetLineX(_tSttPnt,_tEndPnt,_tSttPnt.y+i+_tPara.iOfsGap),_tSttPnt.y+i+_tPara.iOfsGap);
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }
    else if(laLineAngle == la45_135  ){ //위로 향하는 선.
        for(int i = iRangeStt ; i < iRangeEnd ; i++) {
            //현재 픽셀의 밝기 채크
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //오프셑 준 픽셀의 밝기 체크.
            OffsetPx = _pImg -> GetSubPixel(CMath::GetLineX(_tSttPnt,_tEndPnt,_tSttPnt.y-i-_tPara.iOfsGap),_tSttPnt.y-i-_tPara.iOfsGap);
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }

    //좌우 씨리즈
    else if(laLineAngle == la0       ){  //오른쪽으로 수평
        for(int i = iRangeStt ; i < iRangeEnd ; i++) {
            //현재 픽셀의 밝기 채크
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //오프셑 준 픽셀의 밝기 체크.
            OffsetPx = _pImg -> GetPixel(_tSttPnt.x+i+_tPara.iOfsGap,_tSttPnt.y) ;
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }
    else if(laLineAngle == la180     ){ //왼쪽으로 수평.
        for(int i = iRangeStt ; i < iRangeEnd ; i++) {
            //현재 픽셀의 밝기 채크
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //오프셑 준 픽셀의 밝기 체크.
            OffsetPx = _pImg -> GetPixel(_tSttPnt.x-i-_tPara.iOfsGap,_tSttPnt.y) ;
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }
    else if(laLineAngle == la315_45  ){ //오른쪽으로 퍼진 선.
        for(int i = iRangeStt ; i < iRangeEnd ; i++) {
            //현재 픽셀의 밝기 채크
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //오프셑 준 픽셀의 밝기 체크.                      _pImg -> GetSubPixel(_tSttPnt.x+i,CMath::GetLineY(_tSttPnt,_tEndPnt,_tSttPnt.x+i));}break ;
            OffsetPx = _pImg -> GetSubPixel(_tSttPnt.x+i+_tPara.iOfsGap , CMath::GetLineY(_tSttPnt,_tEndPnt,_tSttPnt.x+i+_tPara.iOfsGap));
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }
    else if(laLineAngle == la135_225 ){ //왼쪽으로 향한 선.
        for(int i = iRangeStt ; i < iRangeEnd ; i++) {
            //현재 픽셀의 밝기 채크
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //오프셑 준 픽셀의 밝기 체크.
            OffsetPx = _pImg -> GetSubPixel(_tSttPnt.x-i-_tPara.iOfsGap , CMath::GetLineY(_tSttPnt,_tEndPnt,_tSttPnt.x-i-_tPara.iOfsGap));
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }

    //뻑~~~~ 위에로 대체.
    //for(int i = iRangeStt ; i < iRangeEnd ; i++) {
    //    if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;  //현재 픽셀의 밝기 채크
    //    //if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < pRaw[i+_tPara.iOfsGap] && pRaw[i+_tPara.iOfsGap] < _tPara.iOfsPxHgh)) continue ; //오프셑 준 픽셀의 밝기 체크. 뻑!!!!!!
    //    if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < pRaw[i+_tPara.iOfsGap] && pRaw[i+_tPara.iOfsGap] < _tPara.iOfsPxHgh)) continue ; //오프셑 준 픽셀의 밝기 체크.
    //
    //    iTemp1 = pDif[i] ;
    //    if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
    //    if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
    //}

    //int cTemp ;
    //for( int i= iRangeStt ; i < iRangeEnd ; i++) {
    //     cTemp = pRaw[i] ;
    //     cTemp = pAvr[i] ;
    //     cTemp = pDif[i] ;
    //}

    double dPxLength ;
    TFPoint TempPnt1 , TempPnt2 ;
    TempPnt1.x = _tSttPnt.x ;
    TempPnt1.y = _tSttPnt.y ;
    if(bThin) {
        TempPnt2.y = _tSttPnt.y +1 ;
        TempPnt2.x = CMath::GetLineX(_tSttPnt,_tEndPnt,TempPnt2.y);
        dPxLength=CMath::GetLineLenth(TempPnt1.x,TempPnt1.y,TempPnt2.x,TempPnt2.y);
    }
    else {
        TempPnt2.x = _tSttPnt.x +1 ;
        TempPnt2.y = CMath::GetLineY(_tSttPnt,_tEndPnt,TempPnt2.x);
        dPxLength=CMath::GetLineLenth(TempPnt1.x,TempPnt1.y,TempPnt2.x,TempPnt2.y);
    }

    _pRslt->StartPnt = _tSttPnt ;
    _pRslt->EndPnt   = _tEndPnt ;
    switch(laLineAngle) {
        case la270     : _pRslt->iLtToDkGap   = pDif[iLtToDkPos] ;
                         _pRslt->LtToDkPnt.x  = _tSttPnt.x ;
                         _pRslt->LtToDkPnt.y  = _tSttPnt.y + iLtToDkPos ;
                         _pRslt->LtToDkPnt.y += GetSubOffset(pDif[iLtToDkPos-1],pDif[iLtToDkPos],pDif[iLtToDkPos+1],dPxLength) ;
                         _pRslt->iLtToDkPx    = pRaw[iLtToDkPos] ;

                         _pRslt->iDkToLtGap   = pDif[iDkToLtPos] ;
                         _pRslt->DkToLtPnt.x  = _tSttPnt.x ;
                         _pRslt->DkToLtPnt.y  = _tSttPnt.y + iDkToLtPos ;
                         _pRslt->DkToLtPnt.y += GetSubOffset(pDif[iDkToLtPos-1],pDif[iDkToLtPos],pDif[iDkToLtPos+1],dPxLength) ;

                         _pRslt->iDkToLtPx    = pRaw[iDkToLtPos] ;
                         break ;
        case la90      : _pRslt->iLtToDkGap   = pDif[iLtToDkPos] ;
                         _pRslt->LtToDkPnt.x  = _tSttPnt.x ;
                         _pRslt->LtToDkPnt.y  = _tSttPnt.y - iLtToDkPos ;
                         _pRslt->LtToDkPnt.y -= GetSubOffset(pDif[iLtToDkPos-1],pDif[iLtToDkPos],pDif[iLtToDkPos+1],dPxLength) ;

                         _pRslt->iLtToDkPx    = pRaw[iLtToDkPos] ;

                         _pRslt->iDkToLtGap   = pDif[iDkToLtPos] ;
                         _pRslt->DkToLtPnt.x  = _tSttPnt.x ;
                         _pRslt->DkToLtPnt.y  = _tSttPnt.y - iDkToLtPos ;
                         _pRslt->DkToLtPnt.y -= GetSubOffset(pDif[iDkToLtPos-1],pDif[iDkToLtPos],pDif[iDkToLtPos+1],dPxLength) ;

                         _pRslt->iDkToLtPx    = pRaw[iDkToLtPos] ;
                         break ;
        case la225_315 : _pRslt->iLtToDkGap   = pDif[iLtToDkPos] ;
                         _pRslt->LtToDkPnt.y  = _tSttPnt.y + iLtToDkPos ;
                         _pRslt->LtToDkPnt.x  = CMath::GetLineX(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.y);
                         _pRslt->LtToDkPnt.y +=GetSubOffset(pDif[iLtToDkPos-1],pDif[iLtToDkPos],pDif[iLtToDkPos+1],dPxLength) ;
                         _pRslt->iLtToDkPx    = pRaw[iLtToDkPos] ;

                         _pRslt->iDkToLtGap   = pDif[iDkToLtPos] ;
                         _pRslt->DkToLtPnt.y  = _tSttPnt.y + iDkToLtPos ;
                         _pRslt->DkToLtPnt.x  = CMath::GetLineX(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.y);
                         _pRslt->DkToLtPnt.y += GetSubOffset(pDif[iDkToLtPos-1],pDif[iDkToLtPos],pDif[iDkToLtPos+1],dPxLength) ;
                         _pRslt->iDkToLtPx    = pRaw[iDkToLtPos] ;
                         break ;
        case la45_135  : _pRslt->iLtToDkGap   = pDif[iLtToDkPos] ;
                         _pRslt->LtToDkPnt.y  = _tSttPnt.y - iLtToDkPos ;
                         _pRslt->LtToDkPnt.x  = CMath::GetLineX(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.y);
                         _pRslt->LtToDkPnt.y -= GetSubOffset(pDif[iLtToDkPos-1],pDif[iLtToDkPos],pDif[iLtToDkPos+1],dPxLength) ;
                         _pRslt->iLtToDkPx    = pRaw[iLtToDkPos] ;

                         _pRslt->iDkToLtGap   = pDif[iDkToLtPos] ;
                         _pRslt->DkToLtPnt.y  = _tSttPnt.y - iDkToLtPos ;
                         _pRslt->DkToLtPnt.x  = CMath::GetLineX(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.y);
                         _pRslt->DkToLtPnt.y -=GetSubOffset(pDif[iDkToLtPos-1],pDif[iDkToLtPos],pDif[iDkToLtPos+1],dPxLength) ;
                         _pRslt->iDkToLtPx    = pRaw[iDkToLtPos] ;
                         break ;

        case la0       : _pRslt->iLtToDkGap   = pDif[iLtToDkPos] ;
                         _pRslt->LtToDkPnt.x  = _tSttPnt.x + iLtToDkPos;
                         _pRslt->LtToDkPnt.y  = _tSttPnt.y ;
                         if(iLtToDkPos > 1 && iLtToDkPos < _pImg -> GetWidth()-1 )
                         _pRslt->LtToDkPnt.x += GetSubOffset(pDif[iLtToDkPos-1],pDif[iLtToDkPos],pDif[iLtToDkPos+1],1) ;
                         _pRslt->iLtToDkPx    = pRaw[iLtToDkPos] ;

                         _pRslt->iDkToLtGap   = pDif[iDkToLtPos] ;
                         _pRslt->DkToLtPnt.x  = _tSttPnt.x + iDkToLtPos;
                         _pRslt->DkToLtPnt.y  = _tSttPnt.y ;
                         _pRslt->DkToLtPnt.x += GetSubOffset(pDif[iDkToLtPos-1],pDif[iDkToLtPos],pDif[iDkToLtPos+1],1) ;
                         _pRslt->iDkToLtPx    = pRaw[iDkToLtPos] ;
                         break ;


        case la180     : _pRslt->iLtToDkGap   = pDif[iLtToDkPos] ;
                         _pRslt->LtToDkPnt.x  = _tSttPnt.x - iLtToDkPos;
                         _pRslt->LtToDkPnt.y  = _tSttPnt.y ;
                         _pRslt->LtToDkPnt.x -= GetSubOffset(pDif[iLtToDkPos-1],pDif[iLtToDkPos],pDif[iLtToDkPos+1],1) ;
                         _pRslt->iLtToDkPx    = pRaw[iLtToDkPos] ;

                         _pRslt->iDkToLtGap   = pDif[iDkToLtPos] ;
                         _pRslt->DkToLtPnt.x  = _tSttPnt.x - iDkToLtPos;
                         _pRslt->DkToLtPnt.y  = _tSttPnt.y ;
                         _pRslt->DkToLtPnt.x -= GetSubOffset(pDif[iDkToLtPos-1],pDif[iDkToLtPos],pDif[iDkToLtPos+1],1) ;
                         _pRslt->iDkToLtPx    = pRaw[iDkToLtPos] ;
                         break ;
        case la315_45  : _pRslt->iLtToDkGap   = pDif[iLtToDkPos] ;
                         _pRslt->LtToDkPnt.x  = _tSttPnt.x + iLtToDkPos ;
                         _pRslt->LtToDkPnt.y  = CMath::GetLineY(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.x);
                         _pRslt->LtToDkPnt.x += GetSubOffset(pDif[iLtToDkPos-1],pDif[iLtToDkPos],pDif[iLtToDkPos+1],dPxLength) ;
                         _pRslt->iLtToDkPx    = pRaw[iLtToDkPos] ;

                         _pRslt->iDkToLtGap   = pDif[iDkToLtPos] ;
                         _pRslt->DkToLtPnt.x  = _tSttPnt.x + iDkToLtPos ;
                         _pRslt->DkToLtPnt.y  = CMath::GetLineY(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.x);
                         _pRslt->DkToLtPnt.x += GetSubOffset(pDif[iDkToLtPos-1],pDif[iDkToLtPos],pDif[iDkToLtPos+1],dPxLength) ;
                         _pRslt->iDkToLtPx    = pRaw[iDkToLtPos] ;
                         break ;
        case la135_225 : _pRslt->iLtToDkGap   = pDif[iLtToDkPos] ;
                         _pRslt->LtToDkPnt.x  = _tSttPnt.x - iLtToDkPos ;
                         _pRslt->LtToDkPnt.y  = CMath::GetLineY(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.x);
                         _pRslt->LtToDkPnt.x -= GetSubOffset(pDif[iLtToDkPos-1],pDif[iLtToDkPos],pDif[iLtToDkPos+1],dPxLength) ;
                         _pRslt->iLtToDkPx    = pRaw[iLtToDkPos] ;

                         _pRslt->iDkToLtGap   = pDif[iDkToLtPos] ;
                         _pRslt->DkToLtPnt.x  = _tSttPnt.x - iDkToLtPos ;
                         _pRslt->DkToLtPnt.y  = CMath::GetLineY(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.x);
                         _pRslt->DkToLtPnt.x -=GetSubOffset(pDif[iDkToLtPos-1],pDif[iDkToLtPos],pDif[iDkToLtPos+1],dPxLength) ;
                         _pRslt->iDkToLtPx    = pRaw[iDkToLtPos] ;
                         break ;
    }

    _pRslt -> dInspTime = GetTime()- dwTime ;

    delete [] pRaw ;
    delete [] pAvr ;
    delete [] pDif ;
    return true;

}

