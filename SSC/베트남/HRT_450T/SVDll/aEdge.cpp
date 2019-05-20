
//---------------------------------------------------------------------------
#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------
#include "aEdge.h"
#include "SVMaths.h"
#include "GdiUnit.h"
#include "Common.h"

CEdge::CEdge()
{
}

CEdge::~CEdge()
{

}

//서브픽셀 오프셑 구하는 공식 루트 연산 추가.
//float GetSubOffsetS(float ValMinPos , float ValMidPos , float ValMaxPos) //x,y 값중 작은 포지션의 픽셀벨류 , x,y 값중 중간 포지션의 픽셀벨류 , x,y 값중 큰 포지션의 픽셀벨류
//{
//    //Sqrt.
//    float a = ValMidPos - ValMinPos ;
//    float b = ValMaxPos - ValMidPos ;
//
//    if(b+a == 0) return 0.0 ;
//
//    float c = (b-a)/(b+a) ;
//    bool d = c<0 ;
//    if(d) c = c * -1 ;
//    float e = sqrt(c);
//    float f = e/2.0 ;
//    if(d) f = f * -1 ;
//
//    return f ;
//
//
//}

//서브 픽셀 오프셑 구하는 공식. 실패.
float GetSubOffsetFail(float _fFstPosVal , float _fMidPosVal , float _fLstPosVal , float _fPxLength) //x,y 값중 작은 포지션의 픽셀벨류 , x,y 값중 중간 포지션의 픽셀벨류 , x,y 값중 큰 포지션의 픽셀벨류
{
    bool  bDkToLt ;
    bool  bGap    ;
    float fMidPosVal ;
    float fRatio ;

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
float GetSubOffset(float _fFstPosDif , float _fMidPosDif , float _fLstPosDif , float _fPxLength)
{
    //나중에 타당성 검토.
    if(2*(_fLstPosDif+_fFstPosDif-2*_fMidPosDif) == 0){
        return 0.0 ;
    }
    
    //3Point Parabola Fit.
    float fOffset = (_fFstPosDif - _fLstPosDif)/(2*(_fLstPosDif+_fFstPosDif-2*_fMidPosDif)) ;

    return fOffset * _fPxLength ;
}




bool CEdge::Inspect(CImage * _pImg  , TPoint  _tSttPnt , TPoint  _tEndPnt , EDG_Para _tPara , EDG_Rslt * _pRslt)
{
    DWORD dwTime =GetTickTimeDouble();
    g_sErrMsg = "";
    memset(_pRslt , 0 , sizeof(*_pRslt));

    //Para Check & Err Check
    if(_pImg == NULL                                                     ) {g_sErrMsg = "Image Buffer is NULL."; return false ;}
    if(_pImg -> SetLine(&_tSttPnt , &_tEndPnt , _tPara.iAverage)         ) {g_sErrMsg = "Image Range Over"     ; return false ;}
    if(_pImg -> SetLine(&_tSttPnt , &_tEndPnt , _tPara.iCompare)         ) {g_sErrMsg = "Image Range Over"     ; return false ;}
    if(MATH_GetLineLenth(_tSttPnt.x,_tSttPnt.y,_tEndPnt.x,_tEndPnt.y) < 5) {g_sErrMsg = "Range is too short"   ; return false ;}

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

    //Raw Data Getting.
    float  SubPos ;
    enum EN_LINE_ANGLE {la0,la90,la180,la270,la45_135,la135_225,la225_315,la315_45} laLineAngle;
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
        case la225_315 : for(int i = 0 ; i < iRange ; i++){pRaw[ i] = _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_tSttPnt.y+i),_tSttPnt.y+i);}break ;
        case la45_135  : for(int i = 0 ; i > iRange ; i--){pRaw[-i] = _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_tSttPnt.y+i),_tSttPnt.y+i);}break ;

        case la0       : for(int i = 0 ; i < iRange ; i++){pRaw[ i] = _pImg -> GetPixel(_tSttPnt.x+i,_tSttPnt.y) ;} break ;
        case la180     : for(int i = 0 ; i > iRange ; i--){pRaw[-i] = _pImg -> GetPixel(_tSttPnt.x+i,_tSttPnt.y) ;} break ;
        case la315_45  : for(int i = 0 ; i < iRange ; i++){pRaw[ i] = _pImg -> GetSubPixel(_tSttPnt.x+i,MATH_GetLineY(_tSttPnt,_tEndPnt,_tSttPnt.x+i));}break ;
        case la135_225 : for(int i = 0 ; i > iRange ; i--){pRaw[-i] = _pImg -> GetSubPixel(_tSttPnt.x+i,MATH_GetLineY(_tSttPnt,_tEndPnt,_tSttPnt.x+i));}break ;
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
    int   iDkToLtPos = 0 ;
    int   iLtToDkPos = 0 ;
    int   iLtToDkVal = 0 ;
    int   iDkToLtVal = 0 ;

    /*
    switch(laLineAngle) {
        case la270     : for(int i = 0 ; i < iRange ; i++){pRaw[ i] = _pImg -> GetPixel(_tSttPnt.x,_tSttPnt.y+i) ;} break ;
        case la90      : for(int i = 0 ; i > iRange ; i--){pRaw[-i] = _pImg -> GetPixel(_tSttPnt.x,_tSttPnt.y+i) ; } break ;
        case la225_315 : for(int i = 0 ; i < iRange ; i++){pRaw[ i] = _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_tSttPnt.y+i),_tSttPnt.y+i);}break ;
        case la45_135  : for(int i = 0 ; i > iRange ; i--){pRaw[-i] = _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_tSttPnt.y+i),_tSttPnt.y+i);}break ;

        case la0       : for(int i = 0 ; i < iRange ; i++){pRaw[ i] = _pImg -> GetPixel(_tSttPnt.x+i,_tSttPnt.y) ;} break ;
        case la180     : for(int i = 0 ; i > iRange ; i--){pRaw[-i] = _pImg -> GetPixel(_tSttPnt.x+i,_tSttPnt.y) ;} break ;
        case la315_45  : for(int i = 0 ; i < iRange ; i++){pRaw[ i] = _pImg -> GetSubPixel(_tSttPnt.x+i,MATH_GetLineY(_tSttPnt,_tEndPnt,_tSttPnt.x+i));}break ;
        case la135_225 : for(int i = 0 ; i > iRange ; i--){pRaw[-i] = _pImg -> GetSubPixel(_tSttPnt.x+i,MATH_GetLineY(_tSttPnt,_tEndPnt,_tSttPnt.x+i));}break ;
    }
    */
    unsigned char OffsetPx ;
    //상하 씨리즈
    if(laLineAngle == la270     ) { //아래로 수직인 직선.
        for(int i = 0 ; i < abs(iRange) ; i++) {
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
        for(int i = 0 ; i < abs(iRange) ; i++) {
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
    else if(laLineAngle == la225_315 ){ //밑으로 향하는 직선.
        for(int i = 0 ; i < abs(iRange) ; i++) {
            //현재 픽셀의 밝기 채크
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //오프셑 준 픽셀의 밝기 체크.
            OffsetPx = _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_tSttPnt.y+i+_tPara.iOfsGap),_tSttPnt.y+i+_tPara.iOfsGap);
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }
    else if(laLineAngle == la45_135  ){ //위로 향하는 직선.
        for(int i = 0 ; i < abs(iRange) ; i++) {
            //현재 픽셀의 밝기 채크
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //오프셑 준 픽셀의 밝기 체크.
            OffsetPx = _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_tSttPnt.y-i-_tPara.iOfsGap),_tSttPnt.y-i-_tPara.iOfsGap);
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }

    //좌우 씨리즈
    else if(laLineAngle == la0       ){  //오른쪽으로 수평
        for(int i = 0 ; i < abs(iRange) ; i++) {
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
        for(int i = 0 ; i < abs(iRange) ; i++) {
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
    else if(laLineAngle == la315_45  ){ //오른쪽으로 퍼진 직선.
        for(int i = 0 ; i < abs(iRange) ; i++) {
            //현재 픽셀의 밝기 채크
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //오프셑 준 픽셀의 밝기 체크.                      _pImg -> GetSubPixel(_tSttPnt.x+i,MATH_GetLineY(_tSttPnt,_tEndPnt,_tSttPnt.x+i));}break ;
            OffsetPx = _pImg -> GetSubPixel(_tSttPnt.x+i+_tPara.iOfsGap , MATH_GetLineY(_tSttPnt,_tEndPnt,_tSttPnt.x+i+_tPara.iOfsGap));
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }
    else if(laLineAngle == la135_225 ){ //왼쪽으로 향한 직선.
        for(int i = 0 ; i < abs(iRange) ; i++) {
            //현재 픽셀의 밝기 채크
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //오프셑 준 픽셀의 밝기 체크.
            OffsetPx = _pImg -> GetSubPixel(_tSttPnt.x-i-_tPara.iOfsGap , MATH_GetLineY(_tSttPnt,_tEndPnt,_tSttPnt.x-i-_tPara.iOfsGap));
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }

    //뻑~~~~ 위에로 대체.
    //for(int i = 0 ; i < abs(iRange) ; i++) {
    //    if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;  //현재 픽셀의 밝기 채크
    //    //if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < pRaw[i+_tPara.iOfsGap] && pRaw[i+_tPara.iOfsGap] < _tPara.iOfsPxHgh)) continue ; //오프셑 준 픽셀의 밝기 체크. 뻑!!!!!!
    //    if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < pRaw[i+_tPara.iOfsGap] && pRaw[i+_tPara.iOfsGap] < _tPara.iOfsPxHgh)) continue ; //오프셑 준 픽셀의 밝기 체크.
    //
    //    iTemp1 = pDif[i] ;
    //    if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
    //    if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
    //}

    //int cTemp ;
    //for( int i= 0 ; i < abs(iRange) ; i++) {
    //     cTemp = pRaw[i] ;
    //     cTemp = pAvr[i] ;
    //     cTemp = pDif[i] ;
    //}

    float fPxLength ;
    TFPoint TempPnt1 , TempPnt2 ;
    TempPnt1.x = _tSttPnt.x ;
    TempPnt1.y = _tSttPnt.y ;
    if(bThin) {
        TempPnt2.y = _tSttPnt.y +1 ;
        TempPnt2.x = MATH_GetLineX(_tSttPnt,_tEndPnt,TempPnt2.y);
        fPxLength=MATH_GetLineLenth(TempPnt1.x,TempPnt1.y,TempPnt2.x,TempPnt2.y);
    }
    else {
        TempPnt2.x = _tSttPnt.x +1 ;
        TempPnt2.y = MATH_GetLineY(_tSttPnt,_tEndPnt,TempPnt2.x);
        fPxLength=MATH_GetLineLenth(TempPnt1.x,TempPnt1.y,TempPnt2.x,TempPnt2.y);
    }

    _pRslt->StartPnt = _tSttPnt ;
    _pRslt->EndPnt   = _tEndPnt ;
    switch(laLineAngle) {
        case la270     : _pRslt->iLtToDkGap   = pDif[iLtToDkPos] ;
                         _pRslt->LtToDkPnt.x  = _tSttPnt.x ;
                         _pRslt->LtToDkPnt.y  = _tSttPnt.y + iLtToDkPos ;
                         _pRslt->LtToDkPntS.x = _tSttPnt.x ;
                         _pRslt->LtToDkPntS.y = _tSttPnt.y + iLtToDkPos + GetSubOffset(pDif[iLtToDkPos-1],pDif[iLtToDkPos],pDif[iLtToDkPos+1],fPxLength) ;
                         _pRslt->iLtToDkPx    = pRaw[iLtToDkPos] ;

                         _pRslt->iDkToLtGap   = pDif[iDkToLtPos] ;
                         _pRslt->DkToLtPnt.x  = _tSttPnt.x ;
                         _pRslt->DkToLtPnt.y  = _tSttPnt.y + iDkToLtPos ;
                         _pRslt->DkToLtPntS.x = _tSttPnt.x ;
                         _pRslt->DkToLtPntS.y = _tSttPnt.y + iDkToLtPos + GetSubOffset(pDif[iDkToLtPos-1],pDif[iDkToLtPos],pDif[iDkToLtPos+1],fPxLength) ;
                         _pRslt->iDkToLtPx    = pRaw[iDkToLtPos] ;
                         break ;
        case la90      : _pRslt->iLtToDkGap   = pDif[iLtToDkPos] ;
                         _pRslt->LtToDkPnt.x  = _tSttPnt.x ;
                         _pRslt->LtToDkPnt.y  = _tSttPnt.y - iLtToDkPos ;
                         _pRslt->LtToDkPntS.x = _tSttPnt.x ;
                         _pRslt->LtToDkPntS.y = _tSttPnt.y - iLtToDkPos - GetSubOffset(pDif[iLtToDkPos-1],pDif[iLtToDkPos],pDif[iLtToDkPos+1],fPxLength) ;
                         _pRslt->iLtToDkPx    = pRaw[iLtToDkPos] ;

                         _pRslt->iDkToLtGap   = pDif[iDkToLtPos] ;
                         _pRslt->DkToLtPnt.x  = _tSttPnt.x ;
                         _pRslt->DkToLtPnt.y  = _tSttPnt.y - iDkToLtPos ;
                         _pRslt->DkToLtPntS.x = _tSttPnt.x ;
                         _pRslt->DkToLtPntS.y = _tSttPnt.y - iDkToLtPos - GetSubOffset(pDif[iDkToLtPos-1],pDif[iDkToLtPos],pDif[iDkToLtPos+1],fPxLength) ;
                         _pRslt->iDkToLtPx    = pRaw[iDkToLtPos] ;
                         break ;
        case la225_315 : _pRslt->iLtToDkGap   = pDif[iLtToDkPos] ;
                         _pRslt->LtToDkPnt.y  = _tSttPnt.y + iLtToDkPos ;
                         _pRslt->LtToDkPnt.x  = MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.y);
                         _pRslt->LtToDkPntS.y = _tSttPnt.y + iLtToDkPos + GetSubOffset(pDif[iLtToDkPos-1],pDif[iLtToDkPos],pDif[iLtToDkPos+1],fPxLength) ;
                         _pRslt->LtToDkPntS.x = MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->LtToDkPntS.y);
                         _pRslt->iLtToDkPx    = pRaw[iLtToDkPos] ;

                         _pRslt->iDkToLtGap   = pDif[iDkToLtPos] ;
                         _pRslt->DkToLtPnt.y  = _tSttPnt.y + iDkToLtPos ;
                         _pRslt->DkToLtPnt.x  = MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.y);
                         _pRslt->DkToLtPntS.y = _tSttPnt.y + iDkToLtPos + GetSubOffset(pDif[iDkToLtPos-1],pDif[iDkToLtPos],pDif[iDkToLtPos+1],fPxLength) ;
                         _pRslt->DkToLtPntS.x = MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->DkToLtPntS.y);
                         _pRslt->iDkToLtPx    = pRaw[iDkToLtPos] ;
                         break ;
        case la45_135  : _pRslt->iLtToDkGap   = pDif[iLtToDkPos] ;
                         _pRslt->LtToDkPnt.y  = _tSttPnt.y - iLtToDkPos ;
                         _pRslt->LtToDkPnt.x  = MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.y);
                         _pRslt->LtToDkPntS.y = _tSttPnt.y - iLtToDkPos - GetSubOffset(pDif[iLtToDkPos-1],pDif[iLtToDkPos],pDif[iLtToDkPos+1],fPxLength) ;
                         _pRslt->LtToDkPntS.x = MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->LtToDkPntS.y);
                         _pRslt->iLtToDkPx    = pRaw[iLtToDkPos] ;

                         _pRslt->iDkToLtGap   = pDif[iDkToLtPos] ;
                         _pRslt->DkToLtPnt.y  = _tSttPnt.y - iDkToLtPos ;
                         _pRslt->DkToLtPnt.x  = MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.y);
                         _pRslt->DkToLtPntS.y = _tSttPnt.y - iDkToLtPos - GetSubOffset(pDif[iDkToLtPos-1],pDif[iDkToLtPos],pDif[iDkToLtPos+1],fPxLength) ;
                         _pRslt->DkToLtPntS.x = MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->DkToLtPntS.y);
                         _pRslt->iDkToLtPx    = pRaw[iDkToLtPos] ;
                         break ;

        case la0       : _pRslt->iLtToDkGap   = pDif[iLtToDkPos] ;
                         _pRslt->LtToDkPnt.x  = _tSttPnt.x + iLtToDkPos;
                         _pRslt->LtToDkPnt.y  = _tSttPnt.y ;
                         _pRslt->LtToDkPntS.x = _tSttPnt.x + iLtToDkPos + GetSubOffset(pDif[iLtToDkPos-1],pDif[iLtToDkPos],pDif[iLtToDkPos+1],1) ;
                         _pRslt->LtToDkPntS.y = _tSttPnt.y ;
                         _pRslt->iLtToDkPx    = pRaw[iLtToDkPos] ;

                         _pRslt->iDkToLtGap   = pDif[iDkToLtPos] ;
                         _pRslt->DkToLtPnt.x  = _tSttPnt.x + iDkToLtPos;
                         _pRslt->DkToLtPnt.y  = _tSttPnt.y ;
                         _pRslt->DkToLtPntS.x = _tSttPnt.x + iDkToLtPos + GetSubOffset(pDif[iDkToLtPos-1],pDif[iDkToLtPos],pDif[iDkToLtPos+1],1) ;
                         _pRslt->DkToLtPntS.y = _tSttPnt.y ;
                         _pRslt->iDkToLtPx    = pRaw[iDkToLtPos] ;
                         break ;


        case la180     : _pRslt->iLtToDkGap   = pDif[iLtToDkPos] ;
                         _pRslt->LtToDkPnt.x  = _tSttPnt.x - iLtToDkPos;
                         _pRslt->LtToDkPnt.y  = _tSttPnt.y ;
                         _pRslt->LtToDkPntS.x = _tSttPnt.x - iLtToDkPos - GetSubOffset(pDif[iLtToDkPos-1],pDif[iLtToDkPos],pDif[iLtToDkPos+1],1) ;
                         _pRslt->LtToDkPntS.y = _tSttPnt.y ;
                         _pRslt->iLtToDkPx    = pRaw[iLtToDkPos] ;

                         _pRslt->iDkToLtGap   = pDif[iDkToLtPos] ;
                         _pRslt->DkToLtPnt.x  = _tSttPnt.x - iDkToLtPos;
                         _pRslt->DkToLtPnt.y  = _tSttPnt.y ;
                         _pRslt->DkToLtPntS.x = _tSttPnt.x - iDkToLtPos - GetSubOffset(pDif[iDkToLtPos-1],pDif[iDkToLtPos],pDif[iDkToLtPos+1],1) ;
                         _pRslt->DkToLtPntS.y = _tSttPnt.y ;
                         _pRslt->iDkToLtPx    = pRaw[iDkToLtPos] ;
                         break ;
        case la315_45  : _pRslt->iLtToDkGap   = pDif[iLtToDkPos] ;
                         _pRslt->LtToDkPnt.x  = _tSttPnt.x + iLtToDkPos ;
                         _pRslt->LtToDkPnt.y  = MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.x);
                         _pRslt->LtToDkPntS.x = _tSttPnt.x + iLtToDkPos + GetSubOffset(pDif[iLtToDkPos-1],pDif[iLtToDkPos],pDif[iLtToDkPos+1],fPxLength) ;
                         _pRslt->LtToDkPntS.y = MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->LtToDkPntS.x);
                         _pRslt->iLtToDkPx    = pRaw[iLtToDkPos] ;

                         _pRslt->iDkToLtGap   = pDif[iDkToLtPos] ;
                         _pRslt->DkToLtPnt.x  = _tSttPnt.x + iDkToLtPos ;
                         _pRslt->DkToLtPnt.y  = MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.x);
                         _pRslt->DkToLtPntS.x = _tSttPnt.x + iDkToLtPos + GetSubOffset(pDif[iDkToLtPos-1],pDif[iDkToLtPos],pDif[iDkToLtPos+1],fPxLength) ;
                         _pRslt->DkToLtPntS.y = MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->DkToLtPntS.x);
                         _pRslt->iDkToLtPx    = pRaw[iDkToLtPos] ;
                         break ;
        case la135_225 : _pRslt->iLtToDkGap   = pDif[iLtToDkPos] ;
                         _pRslt->LtToDkPnt.x  = _tSttPnt.x - iLtToDkPos ;
                         _pRslt->LtToDkPnt.y  = MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.x);
                         _pRslt->LtToDkPntS.x = _tSttPnt.x - iLtToDkPos - GetSubOffset(pDif[iLtToDkPos-1],pDif[iLtToDkPos],pDif[iLtToDkPos+1],fPxLength) ;
                         _pRslt->LtToDkPntS.y = MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->LtToDkPntS.x);
                         _pRslt->iLtToDkPx    = pRaw[iLtToDkPos] ;

                         _pRslt->iDkToLtGap   = pDif[iDkToLtPos] ;
                         _pRslt->DkToLtPnt.x  = _tSttPnt.x - iDkToLtPos ;
                         _pRslt->DkToLtPnt.y  = MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.x);
                         _pRslt->DkToLtPntS.x = _tSttPnt.x - iDkToLtPos - GetSubOffset(pDif[iDkToLtPos-1],pDif[iDkToLtPos],pDif[iDkToLtPos+1],fPxLength) ;
                         _pRslt->DkToLtPntS.y = MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->DkToLtPntS.x);
                         _pRslt->iDkToLtPx    = pRaw[iDkToLtPos] ;
                         break ;
    }

    _pRslt->fInspTime = MATH_RoundOff(GetTickTimeDouble()- dwTime , 2) ;

    delete [] pRaw ;
    delete [] pAvr ;
    delete [] pDif ;
    return true;

}

/*
bool CEdge::Inspect2(CImage * _pImg  , TPoint  _tSttPnt , TPoint  _tEndPnt , EDG_Para _tPara , EDG_Rslt * _pRslt)
{
    DWORD dwTime =GetTickTimeDouble();
    g_sErrMsg = "";
    memset(_pRslt , 0 , sizeof(*_pRslt));

    //Para Check & Err Check
    if(_pImg == NULL        ) {g_sErrMsg = "Image Buffer is NULL." ; return false ; }
    _pImg -> SetLine(&_tSttPnt , &_tEndPnt) ;
    if(_tPara.iAverage < 1) {_tPara.iAverage = 1 ;}
    if(_tPara.iCompare < 1) {_tPara.iCompare = 1 ;}


    bool IsThin = (abs(_tSttPnt.x - _tEndPnt.x) - abs(_tSttPnt.y - _tEndPnt.y)) < 0 ;


    float fPxLength ;

    //Local.
    int iRange  ; //요놈이 -값이면 반대방향연산.
    int iStartX ;
    int iStartY ;
    int * pAvr  ;
    int iDataCnt ;
    int iSum    ;
    BYTE Px , PxOfs;

    TFPoint TempPnt1 , TempPnt2 ;

    TempPnt1.x = _tSttPnt.x ;
    TempPnt1.y = _tSttPnt.y ;
    if(IsThin) {
        TempPnt2.y = _tSttPnt.y +1 ;
        TempPnt2.x = MATH_GetLineX(_tSttPnt,_tEndPnt,TempPnt2.y);
        fPxLength=MATH_GetLineLenth(TempPnt1.x,TempPnt1.y,TempPnt2.x,TempPnt2.y);

    }
    else {
        TempPnt2.x = _tSttPnt.x +1 ;
        TempPnt2.y = MATH_GetLineY(_tSttPnt,_tEndPnt,TempPnt2.x);
        fPxLength=MATH_GetLineLenth(TempPnt1.x,TempPnt1.y,TempPnt2.x,TempPnt2.y);
    }

    // Vertical Line. 평균값 연산.
    if(_tSttPnt.x == _tEndPnt.x) {
        iRange = _tEndPnt.y - _tSttPnt.y ;                                            //범위가 평균내는 파라미터 보다 적으면 리턴.
        if(abs(iRange) <= _tPara.iAverage*2+1){g_sErrMsg="abs(iRange) <= _tPara.iAverage*2+1"; return false; }
        pAvr = new int[abs(iRange)];                                                  //레인지 범위 만큼 평균값넣을 메모리 동적 생성.

        iStartY = iRange > 0 ? _tSttPnt.y : _tEndPnt.y ;                              //작은값이 스타트가 되게 하기 위한 처리.
        iStartX = _tSttPnt.x ;

        for(register int y = 0 ; y < abs(iRange) ; y++ ) {                            //동적생성된 평균값배열에 실제 평균값을 계산해서 넣음.
           iDataCnt = 0 ;
           iSum     = 0 ;
           for(register int v = (-_tPara.iAverage) ; v <= _tPara.iAverage ; v++) {
              if((y + v) <  0         ) continue ;
              if((y + v) >= abs(iRange)) break    ;
              iSum += _pImg -> GetPixel(iStartX,(y+iStartY+v)) ;
              iDataCnt++;
           }
           pAvr[y] = iDataCnt ? iSum / iDataCnt : 0 ;
        }


        _pRslt->iDkToLtGap = (iRange > 0) ? -255 :  255 ;                           //가장 최고의 어둠에서 밝음으로 가는값 을 구하기 위해
        _pRslt->iLtToDkGap = (iRange > 0) ?  255 : -255 ;                           //처음에 가장 약한 값을 미리 넣어둠.

        for(register int y = 0 ; y < abs(iRange) ; y++ ) {     //실제 엦지값 추출.
           if((y - _tPara.iCompare) <  0          ) continue ;
           if((y + _tPara.iCompare) >= abs(iRange) ) break   ;

           Px = _pImg -> GetPixel(iStartX , iStartY + y) ;
           if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < Px && Px < _tPara.iChkPxHgh)) continue ; //현재 픽셀 밝기 옵션 적용.

           PxOfs = _pImg -> GetPixel(iStartX ,(iRange < 0) ? iStartY + y - _tPara.iOfsGap : iStartY + y + _tPara.iOfsGap) ;
           if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < PxOfs && PxOfs < _tPara.iOfsPxHgh)) continue ;
                                                                                                              //오프셑 픽셀 밝기 옵션 적용.
           if(iRange < 0) {  //Vertical - direction
              if( _pRslt->iLtToDkGap <  pAvr[y + _tPara.iCompare] - pAvr[y - _tPara.iCompare] ) {
                  _pRslt->iLtToDkGap  = pAvr[y + _tPara.iCompare] - pAvr[y - _tPara.iCompare] ;
                  _pRslt->LtToDkPnt.x = iStartX     ;
                  _pRslt->LtToDkPnt.y = iStartY + y ;
                  _pRslt->iLtToDkPx   = Px ;
              }
              if( _pRslt->iDkToLtGap >  pAvr[y + _tPara.iCompare] - pAvr[y - _tPara.iCompare] ) {
                  _pRslt->iDkToLtGap  = pAvr[y + _tPara.iCompare] - pAvr[y - _tPara.iCompare] ;
                  _pRslt->DkToLtPnt.x = iStartX     ;
                  _pRslt->DkToLtPnt.y = iStartY + y ;
                  _pRslt->iDkToLtPx   = Px ;
              }
           }

           else {           //Vertical + direction
              if( _pRslt->iLtToDkGap >  pAvr[y + _tPara.iCompare] - pAvr[y - _tPara.iCompare] ) {
                  _pRslt->iLtToDkGap  = pAvr[y + _tPara.iCompare] - pAvr[y - _tPara.iCompare] ;
                  _pRslt->LtToDkPnt.x = iStartX     ;
                  _pRslt->LtToDkPnt.y = iStartY + y ;
                  _pRslt->iLtToDkPx   = Px ;
              }
              if( _pRslt->iDkToLtGap <  pAvr[y + _tPara.iCompare] - pAvr[y - _tPara.iCompare] ) {
                  _pRslt->iDkToLtGap  = pAvr[y + _tPara.iCompare] - pAvr[y - _tPara.iCompare]  ;
                  _pRslt->DkToLtPnt.x = iStartX     ;
                  _pRslt->DkToLtPnt.y = iStartY + y ;
                  _pRslt->iDkToLtPx   = Px ;
              }
           }
        }


        //서브 픽셀.
        _pRslt->DkToLtPntS.x = _pRslt->DkToLtPnt.x ;
        _pRslt->LtToDkPntS.x = _pRslt->LtToDkPnt.x ;
        //_pRslt->DkToLtPntS.y = _pRslt->DkToLtPnt.y +GetSubOffset(pAvr[(int)(_pRslt->DkToLtPnt.y - iStartY - 1)] ,pAvr[(int)(_pRslt->DkToLtPnt.y - iStartY)] ,pAvr[(int)(_pRslt->DkToLtPnt.y - iStartY + 1)],fPxLength);
        //_pRslt->LtToDkPntS.y = _pRslt->LtToDkPnt.y +GetSubOffset(pAvr[(int)(_pRslt->LtToDkPnt.y - iStartY - 1)] ,pAvr[(int)(_pRslt->LtToDkPnt.y - iStartY)] ,pAvr[(int)(_pRslt->LtToDkPnt.y - iStartY + 1)],fPxLength);
        _pRslt->DkToLtPntS.y = _pRslt->DkToLtPnt.y +GetSubOffset(_pImg -> GetPixel(_pRslt->DkToLtPnt.x,_pRslt->DkToLtPnt.y-1) ,_pImg -> GetPixel(_pRslt->DkToLtPnt.x,_pRslt->DkToLtPnt.y) , _pImg -> GetPixel(_pRslt->DkToLtPnt.x,_pRslt->DkToLtPnt.y+1),fPxLength);
        _pRslt->LtToDkPntS.y = _pRslt->LtToDkPnt.y +GetSubOffset(_pImg -> GetPixel(_pRslt->LtToDkPnt.x,_pRslt->LtToDkPnt.y-1) ,_pImg -> GetPixel(_pRslt->LtToDkPnt.x,_pRslt->LtToDkPnt.y) , _pImg -> GetPixel(_pRslt->LtToDkPnt.x,_pRslt->LtToDkPnt.y+1),fPxLength);


        //ROW값으로 서브 픽셀.
        //_pRslt->DkToLtPntS.x = _pRslt->DkToLtPnt.x ;
        //_pRslt->LtToDkPntS.x = _pRslt->LtToDkPnt.x ;
        //_pRslt->DkToLtPntS.y = _pRslt->DkToLtPnt.y +GetSubOffset(_pImg -> GetPixel(_pRslt->DkToLtPnt.x,_pRslt->DkToLtPnt.y-1) ,_pImg -> GetPixel(_pRslt->DkToLtPnt.x,_pRslt->DkToLtPnt.y) , _pImg -> GetPixel(_pRslt->DkToLtPnt.x,_pRslt->DkToLtPnt.y+1));
        //_pRslt->LtToDkPntS.y = _pRslt->LtToDkPnt.y +GetSubOffset(_pImg -> GetPixel(_pRslt->LtToDkPnt.x,_pRslt->LtToDkPnt.y-1) ,_pImg -> GetPixel(_pRslt->LtToDkPnt.x,_pRslt->LtToDkPnt.y) , _pImg -> GetPixel(_pRslt->LtToDkPnt.x,_pRslt->LtToDkPnt.y+1));

        //ROW값으로 서브 픽셀.
        //_pRslt->DkToLtPntSS.x = _pRslt->DkToLtPnt.x ;
        //_pRslt->LtToDkPntSS.x = _pRslt->LtToDkPnt.x ;
        //_pRslt->DkToLtPntSS.y = _pRslt->DkToLtPnt.y +GetSubOffsetS(_pImg -> GetPixel(_pRslt->DkToLtPnt.x,_pRslt->DkToLtPnt.y-1) ,_pImg -> GetPixel(_pRslt->DkToLtPnt.x,_pRslt->DkToLtPnt.y) , _pImg -> GetPixel(_pRslt->DkToLtPnt.x,_pRslt->DkToLtPnt.y+1));
        //_pRslt->LtToDkPntSS.y = _pRslt->LtToDkPnt.y +GetSubOffsetS(_pImg -> GetPixel(_pRslt->LtToDkPnt.x,_pRslt->LtToDkPnt.y-1) ,_pImg -> GetPixel(_pRslt->LtToDkPnt.x,_pRslt->LtToDkPnt.y) , _pImg -> GetPixel(_pRslt->LtToDkPnt.x,_pRslt->LtToDkPnt.y+1));

        delete[] pAvr ;
    }

    // Horizental Line.
    else if(_tSttPnt.y == _tEndPnt.y) {
        iRange = _tEndPnt.x - _tSttPnt.x ;
        if(abs(iRange) <= _tPara.iAverage*2+1){g_sErrMsg = "abs(iRange) <= _tPara.iAverage*2+1"; return false; }
        pAvr = new int[abs(iRange)];
        iStartX = iRange > 0 ? _tSttPnt.x : _tEndPnt.x ;
        iStartY = _tSttPnt.y ;
        for(register int x = 0 ; x < abs(iRange) ; x++ ) {
           iDataCnt = 0 ;
           iSum     = 0 ;
           for(register int v = (-_tPara.iAverage) ; v <= _tPara.iAverage ; v++) {
              if((x + v) <  0          ) continue ;
              if((x + v) >= abs(iRange) ) break    ;
              iSum += _pImg -> GetPixel((x+iStartX+v), iStartY) ;
              iDataCnt++;
           }
           pAvr[x] = iDataCnt ? iSum / iDataCnt : 0 ;
        }
        _pRslt->iDkToLtGap = (iRange > 0) ? -255 :  255 ;
        _pRslt->iLtToDkGap = (iRange > 0) ?  255 : -255 ;
        for(register int x = 0 ; x < abs(iRange) ; x++ ) {
           if((x - _tPara.iCompare) <  0          ) continue ;
           if((x + _tPara.iCompare) >= abs(iRange) ) break    ;

           Px = _pImg -> GetPixel(iStartX + x ,iStartY) ;
           if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < Px && Px < _tPara.iChkPxHgh)) continue ;

           PxOfs = _pImg -> GetPixel((iRange < 0) ? iStartX + x - _tPara.iOfsGap : iStartX + x + _tPara.iOfsGap,iStartY) ;
           if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < PxOfs && PxOfs < _tPara.iOfsPxHgh)) continue ;

           if(iRange < 0) {  //horizental - direction
              if( _pRslt->iLtToDkGap < pAvr[x + _tPara.iCompare] - pAvr[x - _tPara.iCompare]  ) {
                  _pRslt->iLtToDkGap  = pAvr[x + _tPara.iCompare] - pAvr[x - _tPara.iCompare]  ;
                  _pRslt->LtToDkPnt.x = iStartX + x ;
                  _pRslt->LtToDkPnt.y = iStartY     ;
                  _pRslt->iLtToDkPx   = _pImg -> GetPixel(_pRslt->LtToDkPnt.x , _pRslt->LtToDkPnt.y) ;
              }

              if( _pRslt->iDkToLtGap > pAvr[x + _tPara.iCompare] - pAvr[x - _tPara.iCompare]  ) {
                  _pRslt->iDkToLtGap  = pAvr[x + _tPara.iCompare] - pAvr[x - _tPara.iCompare]  ;
                  _pRslt->DkToLtPnt.x = iStartX + x ;
                  _pRslt->DkToLtPnt.y = iStartY     ;
                  _pRslt->iDkToLtPx   = _pImg -> GetPixel(_pRslt->DkToLtPnt.x , _pRslt->DkToLtPnt.y) ;
              }
           }

           else {           //horizental + direction
              if( _pRslt->iLtToDkGap > pAvr[x + _tPara.iCompare] - pAvr[x - _tPara.iCompare]  ) {
                  _pRslt->iLtToDkGap  = pAvr[x + _tPara.iCompare] - pAvr[x - _tPara.iCompare]  ;
                  _pRslt->LtToDkPnt.x = iStartX + x ;
                  _pRslt->LtToDkPnt.y = iStartY     ;
                  _pRslt->iLtToDkPx   = _pImg -> GetPixel(_pRslt->LtToDkPnt.x , _pRslt->LtToDkPnt.y) ;
              }
              if( _pRslt->iDkToLtGap < pAvr[x + _tPara.iCompare] - pAvr[x - _tPara.iCompare]  ) {
                  _pRslt->iDkToLtGap  = pAvr[x + _tPara.iCompare] - pAvr[x - _tPara.iCompare]  ;
                  _pRslt->DkToLtPnt.x = iStartX + x ;
                  _pRslt->DkToLtPnt.y = iStartY     ;
                  _pRslt->iDkToLtPx   = _pImg -> GetPixel(_pRslt->DkToLtPnt.x , _pRslt->DkToLtPnt.y) ;
              }
           }
        }

        //서브 픽셀.
        //평균값 서브 픽셀.
        _pRslt->DkToLtPntS.y = _pRslt->DkToLtPnt.y ;
        _pRslt->LtToDkPntS.y = _pRslt->LtToDkPnt.y ;
        //_pRslt->DkToLtPntS.x = _pRslt->DkToLtPnt.x+GetSubOffset(pAvr[(int)(_pRslt->DkToLtPnt.x - iStartX - 1)] ,pAvr[(int)(_pRslt->DkToLtPnt.x - iStartX)] ,pAvr[(int)(_pRslt->DkToLtPnt.x - iStartX + 1)],fPxLength);
        //_pRslt->LtToDkPntS.x = _pRslt->LtToDkPnt.x+GetSubOffset(pAvr[(int)(_pRslt->LtToDkPnt.x - iStartX - 1)] ,pAvr[(int)(_pRslt->LtToDkPnt.x - iStartX)] ,pAvr[(int)(_pRslt->LtToDkPnt.x - iStartX + 1)],fPxLength);
        _pRslt->DkToLtPntS.x = _pRslt->DkToLtPnt.x+GetSubOffset(pAvr[(int)(_pRslt->DkToLtPnt.x - iStartX - 1)] ,pAvr[(int)(_pRslt->DkToLtPnt.x - iStartX)] ,pAvr[(int)(_pRslt->DkToLtPnt.x - iStartX + 1)],fPxLength);
        _pRslt->LtToDkPntS.x = _pRslt->LtToDkPnt.x+GetSubOffset(pAvr[(int)(_pRslt->LtToDkPnt.x - iStartX - 1)] ,pAvr[(int)(_pRslt->LtToDkPnt.x - iStartX)] ,pAvr[(int)(_pRslt->LtToDkPnt.x - iStartX + 1)],fPxLength);

        //Row값 서브 픽셀.
        //_pRslt->DkToLtPntS.y = _pRslt->DkToLtPnt.y ;
        //_pRslt->LtToDkPntS.y = _pRslt->LtToDkPnt.y ;
        //_pRslt->DkToLtPntS.x = _pRslt->DkToLtPnt.x+GetSubOffset(_pImg -> GetPixel(_pRslt->DkToLtPnt.x - 1,_pRslt->DkToLtPnt.y) ,_pImg -> GetPixel(_pRslt->DkToLtPnt.x,_pRslt->DkToLtPnt.y) ,_pImg -> GetPixel(_pRslt->DkToLtPnt.x + 1,_pRslt->DkToLtPnt.y));
        //_pRslt->LtToDkPntS.x = _pRslt->LtToDkPnt.x+GetSubOffset(_pImg -> GetPixel(_pRslt->LtToDkPnt.x - 1,_pRslt->LtToDkPnt.y) ,_pImg -> GetPixel(_pRslt->LtToDkPnt.x,_pRslt->LtToDkPnt.y) ,_pImg -> GetPixel(_pRslt->LtToDkPnt.x + 1,_pRslt->LtToDkPnt.y));

        //루트 연산 서브 픽셀.
        //_pRslt->DkToLtPntSS.y = _pRslt->DkToLtPnt.y ;
        //_pRslt->LtToDkPntSS.y = _pRslt->LtToDkPnt.y ;
        //_pRslt->DkToLtPntSS.x = _pRslt->DkToLtPnt.x+GetSubOffsetS(_pImg -> GetPixel(_pRslt->DkToLtPnt.x - 1,_pRslt->DkToLtPnt.y) ,_pImg -> GetPixel(_pRslt->DkToLtPnt.x,_pRslt->DkToLtPnt.y) ,_pImg -> GetPixel(_pRslt->DkToLtPnt.x + 1,_pRslt->DkToLtPnt.y));
        //_pRslt->LtToDkPntSS.x = _pRslt->LtToDkPnt.x+GetSubOffsetS(_pImg -> GetPixel(_pRslt->LtToDkPnt.x - 1,_pRslt->LtToDkPnt.y) ,_pImg -> GetPixel(_pRslt->LtToDkPnt.x,_pRslt->LtToDkPnt.y) ,_pImg -> GetPixel(_pRslt->LtToDkPnt.x + 1,_pRslt->LtToDkPnt.y));

        delete[] pAvr ;
    }


    else if(IsThin) { //Thin Diagonal Line.
        iRange = _tEndPnt.y - _tSttPnt.y ;                                           //계산복잡해 지고 속도 느려져서 1의 간격으로 안뽑고 한다.
        if(abs(iRange) <= _tPara.iAverage*2+1) {g_sErrMsg="abs(iRange) <= _tPara.iAverage*2+1"; return false; } ;
        pAvr = new int[abs(iRange)];                                                 //레인지 범위 만큼 평균값넣을 메모리 동적 생성.

        iStartY = iRange > 0 ? _tSttPnt.y : _tEndPnt.y ;                             //작은값이 스타트가 되게 하기 위한 처리.
        iStartX = _tSttPnt.x ;

        float SubX    ; //직선공식으로 계산된 Y값에 해당하는 X값.
        float SubXOfs ; //SubX에서 직선 공식으로 계산된 Offset값을 만큼 이동한 X값.

        for(register int y = 0 ; y < abs(iRange) ; y++ ) {                          //동적생성된 평균값배열에 실제 평균값을 계산해서 넣음.
           iDataCnt = 0 ;
           iSum     = 0 ;
           for(register int v = (-_tPara.iAverage) ; v <= _tPara.iAverage ; v++) {  //한픽셀에 대한 주변 평균값을 구한다.
              if((y + v) <  0          ) continue ;                                 //평균값이 스타트 값이나
              if((y + v) >= abs(iRange) ) break    ;                                //엔드값을 넘어가면 안구한다.

              SubX=MATH_GetLineX(_tSttPnt,_tEndPnt,(y+iStartY+v));
              iSum += _pImg -> GetSubPixel(SubX,(y+iStartY+v)) ;
              iDataCnt++;
           }
           pAvr[y] = iDataCnt ? iSum / iDataCnt : 0 ;
        }


        _pRslt->iDkToLtGap = (iRange > 0) ?  255 : -255 ; //가장 최고의 어둠에서 밝음으로 가는값 을 구하기 위해
        _pRslt->iLtToDkGap = (iRange > 0) ? -255 :  255 ; //처음에 가장 약한 값을 미리 넣어둠.


        float YPos ; //오프셋 먹은 위치.
        float XPos ;
        for(register int y = 0 ; y < abs(iRange) ; y++ ) {     //실제 엦지값 추출.
           if((y - _tPara.iCompare) <  0          ) continue ;
           if((y + _tPara.iCompare) >= abs(iRange)) break    ;

           SubX=MATH_GetLineX(_tSttPnt,_tEndPnt,(y+iStartY));  //현재 와이값에 대한 X값을 구해 놓는다.
           Px = _pImg -> GetSubPixel(SubX , y+iStartY) ;       //해당 좌표의 픽셀 밝기값.
           if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < Px && Px < _tPara.iChkPxHgh)) continue ; //찾은곳 밝기가 스펙아웃이면 컨티뉴.

           MATH_GetPntFromPntByAngLen(SubX , y+iStartY                                                   , //입력 시작점.
                                      MATH_GetLineAngle(_tSttPnt.x, _tSttPnt.y, _tEndPnt.x , _tEndPnt.y) , //각도.
                                      _tPara.iOfsGap                                                     , //입력 길이.
                                      &XPos , &YPos                                                      );//출력 포인트.
           //fgggggg
           //SubXOfs = MATH_GetLineX(_tSttPnt,_tEndPnt,(iRange < 0) ? (y+iStartY- _tPara.iOfsGap) : (y+iStartY+ _tPara.iOfsGap));
           //PxOfs = _pImg -> GetSubPixel(SubXOfs,(iRange < 0) ? iStartY + y - _tPara.iOfsGap : iStartY + y + _tPara.iOfsGap) ;
           PxOfs   = _pImg -> GetSubPixel(XPos,YPos) ;

           if(_tPara.iOfsGap && (_tPara.iOfsPxLow > PxOfs || PxOfs > _tPara.iOfsPxHgh)) continue ;






           if(iRange < 0) {  //Vertical - direction
              if( _pRslt->iLtToDkGap > pAvr[y - _tPara.iCompare] - pAvr[y + _tPara.iCompare] ) {
                  _pRslt->iLtToDkGap   = pAvr[y - _tPara.iCompare] - pAvr[y + _tPara.iCompare] ;
                  _pRslt->LtToDkPnt.x = SubX       ;
                  _pRslt->LtToDkPnt.y = iStartY + y ;
                  _pRslt->iLtToDkPx   = Px ;

                  m_pOfsLtToDk.x = XPos ;
                  m_pOfsLtToDk.y = YPos ;

              }
              if( _pRslt->iDkToLtGap < pAvr[y - _tPara.iCompare] - pAvr[y + _tPara.iCompare] ) {
                  _pRslt->iDkToLtGap  = pAvr[y - _tPara.iCompare] - pAvr[y + _tPara.iCompare] ;
                  _pRslt->DkToLtPnt.x = SubX       ;
                  _pRslt->DkToLtPnt.y = iStartY + y ;
                  _pRslt->iDkToLtPx   = Px ;

                  m_pOfsDkToLt.x = XPos ;
                  m_pOfsDkToLt.y = YPos ;
              }
           }

           else {           //Vertical + direction
              if( _pRslt->iLtToDkGap < pAvr[y - _tPara.iCompare] - pAvr[y + _tPara.iCompare] ) {
                  _pRslt->iLtToDkGap  = pAvr[y - _tPara.iCompare] - pAvr[y + _tPara.iCompare] ;
                  _pRslt->LtToDkPnt.x = SubX       ;
                  _pRslt->LtToDkPnt.y = iStartY + y ;
                  _pRslt->iLtToDkPx   = Px ;

                  m_pOfsLtToDk.x = XPos ;
                  m_pOfsLtToDk.y = YPos ;
              }
              if( _pRslt->iDkToLtGap > pAvr[y - _tPara.iCompare] - pAvr[y + _tPara.iCompare] ) {
                  _pRslt->iDkToLtGap  = pAvr[y - _tPara.iCompare] - pAvr[y + _tPara.iCompare] ;
                  _pRslt->DkToLtPnt.x = SubX       ;
                  _pRslt->DkToLtPnt.y = iStartY + y ;
                  _pRslt->iDkToLtPx   = Px ;

                  m_pOfsDkToLt.x = XPos ;
                  m_pOfsDkToLt.y = YPos ;
              }
           }
        }


        //서브 픽셀.                                //밑에 맨트 나중에 생각 해 보니 필요 없음.
        //Row값 서브 픽셀.                          //GetSubOffset으로 나온값에 대각선 길이 개념을 각도 별로 환산하여 곱해줘야 한다. 나중에 작업. ex) 45도 일때 루트2
        _pRslt->DkToLtPntS.y = _pRslt->DkToLtPnt.y +GetSubOffset(_pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.y-1),_pRslt->DkToLtPnt.y-1) ,
                                                                 _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.y  ),_pRslt->DkToLtPnt.y  ) ,
                                                                 _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.y+1),_pRslt->DkToLtPnt.y+1),fPxLength);
        _pRslt->DkToLtPntS.x = MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->DkToLtPntS.y  ) ;

        _pRslt->LtToDkPntS.y = _pRslt->LtToDkPnt.y +GetSubOffset(_pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.y-1),_pRslt->LtToDkPnt.y-1) ,
                                                                 _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.y  ),_pRslt->LtToDkPnt.y  ) ,
                                                                 _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.y+1),_pRslt->LtToDkPnt.y+1),fPxLength);
        _pRslt->LtToDkPntS.x = MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->LtToDkPntS.y  ) ;


        //루트 연산 서브 픽셀.
        //_pRslt->DkToLtPntSS.y = _pRslt->DkToLtPnt.y +GetSubOffsetS(_pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.y-1),_pRslt->DkToLtPnt.y-1) ,
        //                                                           _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.y  ),_pRslt->DkToLtPnt.y  ) ,
        //                                                           _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.y+1),_pRslt->DkToLtPnt.y+1));
        //_pRslt->DkToLtPntSS.x = MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->DkToLtPntSS.y  ) ;
        //
        //_pRslt->LtToDkPntSS.y = _pRslt->LtToDkPnt.y +GetSubOffsetS(_pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.y-1),_pRslt->LtToDkPnt.y-1) ,
        //                                                           _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.y  ),_pRslt->LtToDkPnt.y  ) ,
        //                                                           _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.y+1),_pRslt->LtToDkPnt.y+1));
        //_pRslt->LtToDkPntSS.x = MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->LtToDkPntSS.y  ) ;
        delete[] pAvr ;
    }

    else { //Fat Diagonal Line.
        iRange = _tEndPnt.x - _tSttPnt.x ;
        if(abs(iRange) <= _tPara.iAverage*2+1) {g_sErrMsg ="abs(iRange) <= _tPara.iAverage*2+1"; return false; } ;
        pAvr = new int[abs(iRange)];
        iStartX = iRange > 0 ? _tSttPnt.x : _tEndPnt.x ;
        iStartY = _tSttPnt.y ;

        float SubY    ; //직선공식으로 계산된 X값에 해당하는 Y값.
        float SubYOfs ;

        for(register int x = 0 ; x < abs(iRange) ; x++ ) {
           iDataCnt = 0 ;
           iSum     = 0 ;
           for(register int v = (-_tPara.iAverage) ; v <= _tPara.iAverage ; v++) {
              if((x + v) <  0          ) continue ;
              if((x + v) >= abs(iRange)) break    ;
              SubY = MATH_GetLineY(_tSttPnt,_tEndPnt,(x+iStartX+v));
              iSum += _pImg -> GetSubPixel((x+iStartX+v), SubY) ;
              iDataCnt++;
           }
           pAvr[x] = iDataCnt ? iSum / iDataCnt : 0 ;
        }
        _pRslt->iDkToLtGap = (iRange > 0) ? -255 :  255 ;
        _pRslt->iLtToDkGap = (iRange > 0) ?  255 : -255 ;

        float YPos ; //오프셋 먹은 위치.
        float XPos ;
        for(register int x = 0 ; x < abs(iRange) ; x++ ) {
           if((x - _tPara.iCompare) <  0          ) continue ;
           if((x + _tPara.iCompare) >= abs(iRange) ) break   ;

           SubY=MATH_GetLineY(_tSttPnt,_tEndPnt,(x+iStartX));
           Px = _pImg -> GetSubPixel(iStartX + x , SubY) ;
           if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < Px && Px < _tPara.iChkPxHgh)) continue ;


           if(_tPara.iOfsGap) {
               MATH_GetPntFromPntByAngLen(x+iStartX , SubY , // , y+iStartY                                                   , //입력 시작점.
                                          MATH_GetLineAngle(_tSttPnt.x, _tSttPnt.y, _tEndPnt.x , _tEndPnt.y) , //각도.
                                          _tPara.iOfsGap                                                     , //입력 길이.
                                          &XPos , &YPos                                                      );//출력 포인트.
               PxOfs   = _pImg -> GetSubPixel(XPos,YPos) ;
               if((_tPara.iOfsPxLow > PxOfs || PxOfs > _tPara.iOfsPxHgh)) continue ;

               // SubYOfs = MATH_GetLineY(_tSttPnt,_tEndPnt,(iRange < 0) ? (x+iStartX- _tPara.iOfsGap) : (x+iStartX+ _tPara.iOfsGap));
               // PxOfs = _pImg -> GetSubPixel( (iRange < 0) ? iStartX + x - _tPara.iOfsGap : iStartX + x + _tPara.iOfsGap , SubYOfs ) ;
               // if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < PxOfs && PxOfs < _tPara.iOfsPxHgh)) continue ;
           }



           if(iRange < 0) {  //horizental - direction
              if( _pRslt->iLtToDkGap < pAvr[x + _tPara.iCompare] - pAvr[x - _tPara.iCompare]  ) {
                  _pRslt->iLtToDkGap  = pAvr[x + _tPara.iCompare] - pAvr[x - _tPara.iCompare]  ;
                  _pRslt->LtToDkPnt.x = iStartX + x ;
                  _pRslt->LtToDkPnt.y = SubY       ;
                  _pRslt->iLtToDkPx   = Px ;
                  m_pOfsLtToDk.x = XPos ;
                  m_pOfsLtToDk.y = YPos ;
              }

              if( _pRslt->iDkToLtGap > pAvr[x + _tPara.iCompare] - pAvr[x - _tPara.iCompare]  ) {
                  _pRslt->iDkToLtGap  = pAvr[x + _tPara.iCompare] - pAvr[x - _tPara.iCompare]  ;
                  _pRslt->DkToLtPnt.x = iStartX + x ;
                  _pRslt->DkToLtPnt.y = SubY       ;
                  _pRslt->iDkToLtPx   = Px ;
                  m_pOfsDkToLt.x = XPos ;
                  m_pOfsDkToLt.y = YPos ;
              }
           }

           else {           //horizental + direction
              if( _pRslt->iLtToDkGap > pAvr[x + _tPara.iCompare] - pAvr[x - _tPara.iCompare]  ) {
                  _pRslt->iLtToDkGap  = pAvr[x + _tPara.iCompare] - pAvr[x - _tPara.iCompare]  ;
                  _pRslt->LtToDkPnt.x = iStartX + x ;
                  _pRslt->LtToDkPnt.y = SubY       ;
                  _pRslt->iLtToDkPx   = Px ;
                  m_pOfsLtToDk.x = XPos ;
                  m_pOfsLtToDk.y = YPos ;
              }
              if( _pRslt->iDkToLtGap < pAvr[x + _tPara.iCompare] - pAvr[x - _tPara.iCompare]  ) {
                  _pRslt->iDkToLtGap  = pAvr[x + _tPara.iCompare] - pAvr[x - _tPara.iCompare]  ;
                  _pRslt->DkToLtPnt.x = iStartX + x ;
                  _pRslt->DkToLtPnt.y = SubY       ;
                  _pRslt->iDkToLtPx   = Px ;
                  m_pOfsDkToLt.x = XPos ;
                  m_pOfsDkToLt.y = YPos ;
              }
           }
        }
        delete[] pAvr ;

        //Row값 서브 픽셀.
        _pRslt->DkToLtPntS.x = _pRslt->DkToLtPnt.x +GetSubOffset(_pImg -> GetSubPixel(_pRslt->DkToLtPnt.x-1 ,MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.x-1)) ,
                                                                 _pImg -> GetSubPixel(_pRslt->DkToLtPnt.x   ,MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.x  )) ,
                                                                 _pImg -> GetSubPixel(_pRslt->DkToLtPnt.x+1 ,MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.x+1)),fPxLength);
        _pRslt->DkToLtPntS.y = MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.x  ) ;

        _pRslt->LtToDkPntS.x = _pRslt->LtToDkPntS.x +GetSubOffset(_pImg -> GetSubPixel(_pRslt->LtToDkPnt.x-1 ,MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.x-1)) ,
                                                                  _pImg -> GetSubPixel(_pRslt->LtToDkPnt.x   ,MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.x  )) ,
                                                                  _pImg -> GetSubPixel(_pRslt->LtToDkPnt.x+1 ,MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.x+1)),fPxLength);
        _pRslt->LtToDkPntS.y = MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.x  ) ;


        //루트 연산 서브 픽셀.
        //_pRslt->DkToLtPntSS.x = _pRslt->DkToLtPnt.x +GetSubOffsetS(_pImg -> GetSubPixel(_pRslt->DkToLtPnt.x-1 ,MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.x-1)) ,
        //                                                           _pImg -> GetSubPixel(_pRslt->DkToLtPnt.x   ,MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.x  )) ,
        //                                                           _pImg -> GetSubPixel(_pRslt->DkToLtPnt.x+1 ,MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.x+1)));
        //_pRslt->DkToLtPntSS.y = MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.x  ) ;
        //
        //_pRslt->LtToDkPntSS.x = _pRslt->LtToDkPntS.x +GetSubOffsetS(_pImg -> GetSubPixel(_pRslt->LtToDkPnt.x-1 ,MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.x-1)) ,
        //                                                            _pImg -> GetSubPixel(_pRslt->LtToDkPnt.x   ,MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.x  )) ,
        //                                                            _pImg -> GetSubPixel(_pRslt->LtToDkPnt.x+1 ,MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.x+1)));
        //_pRslt->LtToDkPntSS.y = MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.x  ) ;
        //










    }

    _pRslt->StartPnt = _tSttPnt ;
    _pRslt->EndPnt   = _tEndPnt ;

    _pRslt->fInspTime = MATH_RoundOff(GetTickTimeDouble()- dwTime , 2) ;

    return true;

} */


//나중에 TFPoint 가 기본형이고 TPoint는 오버라이딩 되야함.
bool CEdge::Inspect(CImage * _pImg , TFPoint _tSttPnt , TFPoint _tEndPnt , EDG_Para _tPara , EDG_Rslt * _pRslt)
{


    TPoint tSttPnt , tEndPnt ;

    tSttPnt.x = _tSttPnt.x ;
    tSttPnt.y = _tSttPnt.y ;
    tEndPnt.x = _tEndPnt.x ;
    tEndPnt.y = _tEndPnt.y ;

    return Inspect(_pImg , tSttPnt , tEndPnt  , _tPara , _pRslt);

}


void CEdge::PaintRslt(HDC _hHdc , EDG_Rslt * _pRslt , EDG_Disp _tDisp , float _fScaleX , float _fScaleY)
{
    CGdi *Gdi = new CGdi(_hHdc);
    Gdi -> SetScale(_fScaleX , _fScaleY);

    int sx ,sy ,ex ,ey ;

    if(_tDisp.clDkToLtPnt != clNone) {
        Gdi->m_tPen  .Color = _tDisp.clDkToLtPnt ;
        Gdi->m_tBrush.Color = _tDisp.clDkToLtPnt ;

        Gdi->LineX(_pRslt->DkToLtPnt.x , _pRslt->DkToLtPnt.y , 5);
        Gdi->Circle(false,m_pOfsDkToLt,2,0);
    }

    if(_tDisp.clLtToDkPnt != clNone) {
        Gdi->m_tPen  .Color = _tDisp.clLtToDkPnt ;
        Gdi->m_tBrush.Color = _tDisp.clLtToDkPnt ;

        Gdi->LineX(_pRslt->LtToDkPnt.x , _pRslt->LtToDkPnt.y , 5);
        Gdi->Line(sx , sy , ex , ey ) ;

        Gdi->Circle(false,m_pOfsLtToDk,2,0);
    }

    Gdi -> m_tText.Color = clLime ;
    AnsiString sTemp , sTemp1 ,sTemp2;

    //sTemp = sTemp.sprintf("%.2f",_pRslt->fSinc);
    //sTemp = AnsiString("X : ") + _pRslt->iPosX + AnsiString(" Y : ") + _pRslt->iPosY + AnsiString(" Sinc : ") + sTemp ;

    //LtToDkPnt
    if(_tDisp.clText != clNone) {
        Gdi -> m_tText.Color = _tDisp.clText ;

        sTemp1 = sTemp1.sprintf("X:%.3f ",_pRslt->LtToDkPnt.x) ;
        sTemp2 = sTemp2.sprintf("Y:%.3f" ,_pRslt->LtToDkPnt.y) ;
        sTemp  = sTemp1 + sTemp2 ;
        Gdi -> Text(_pRslt->LtToDkPnt.x + 5, _pRslt->LtToDkPnt.y + 5,sTemp.c_str()) ;

        sTemp1 = sTemp1.sprintf("SX:%.3f ",_pRslt->LtToDkPntS.x) ;
        sTemp2 = sTemp2.sprintf("SY:%.3f" ,_pRslt->LtToDkPntS.y) ;
        sTemp  = sTemp1 + sTemp2 ;
        Gdi -> Text(_pRslt->LtToDkPnt.x + 5, _pRslt->LtToDkPnt.y + 15,sTemp.c_str()) ;


        //sTemp = "SSX:" + AnsiString(_pRslt->LtToDkPntSS.x) + " SSY:" + AnsiString(_pRslt->LtToDkPntSS.y) ;
        //Gdi -> Text(_pRslt->LtToDkPnt.x + 5, _pRslt->LtToDkPnt.y + 25,sTemp.c_str()) ;

        //DkToLtPnt
        sTemp1 = sTemp1.sprintf("X:%.3f ",_pRslt->DkToLtPnt.x) ;
        sTemp2 = sTemp2.sprintf("Y:%.3f" ,_pRslt->DkToLtPnt.y) ;
        sTemp  = sTemp1 + sTemp2 ;
        Gdi -> Text(_pRslt->DkToLtPnt.x + 5, _pRslt->DkToLtPnt.y + 5,sTemp.c_str()) ;

        sTemp1 = sTemp1.sprintf("X:%.3f ",_pRslt->DkToLtPntS.x) ;
        sTemp2 = sTemp2.sprintf("Y:%.3f" ,_pRslt->DkToLtPntS.y) ;
        sTemp  = sTemp1 + sTemp2 ;
        Gdi -> Text(_pRslt->DkToLtPnt.x + 15, _pRslt->DkToLtPnt.y + 15,sTemp.c_str()) ;

        //sTemp = "SX:" + AnsiString(_pRslt->DkToLtPntS.x) + " SY:" + AnsiString(_pRslt->DkToLtPntS.y) ;
        //Gdi -> Text(_pRslt->DkToLtPnt.x + 5, _pRslt->DkToLtPnt.y + 15,sTemp.c_str()) ;

        //sTemp = "SSX:" + AnsiString(_pRslt->DkToLtPntSS.x) + " SSY:" + AnsiString(_pRslt->DkToLtPntSS.y) ;
        //Gdi -> Text(_pRslt->DkToLtPnt.x + 5, _pRslt->DkToLtPnt.y + 25,sTemp.c_str()) ;



        sTemp = sTemp.sprintf("Length:%.3f ",MATH_GetLineLenth(_pRslt->DkToLtPntS.x , _pRslt->DkToLtPntS.y , _pRslt->LtToDkPntS.x , _pRslt->LtToDkPntS.y)) ;
        Gdi -> Text(_pRslt->DkToLtPnt.x + 5, _pRslt->DkToLtPnt.y + 25,sTemp.c_str()) ;

        //sTemp = MATH_GetLineLenth(_pRslt->DkToLtPntS.x , _pRslt->DkToLtPntS.y , _pRslt->LtToDkPntS.x , _pRslt->LtToDkPntS.y) ;
        //Gdi -> Text(_pRslt->DkToLtPnt.x + 5, _pRslt->DkToLtPnt.y + 50 ,sTemp.c_str()) ;

        //sTemp = MATH_GetLineLenth(_pRslt->DkToLtPntSS.x , _pRslt->DkToLtPntSS.y , _pRslt->LtToDkPntSS.x , _pRslt->LtToDkPntSS.y) ;
        //Gdi -> Text(_pRslt->DkToLtPnt.x + 5, _pRslt->DkToLtPnt.y + 60 ,sTemp.c_str()) ;
    }


    delete Gdi;
}

