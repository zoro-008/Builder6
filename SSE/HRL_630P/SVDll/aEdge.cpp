
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

//�����ȼ� �����V ���ϴ� ���� ��Ʈ ���� �߰�.
//float GetSubOffsetS(float ValMinPos , float ValMidPos , float ValMaxPos) //x,y ���� ���� �������� �ȼ����� , x,y ���� �߰� �������� �ȼ����� , x,y ���� ū �������� �ȼ�����
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

//���� �ȼ� �����V ���ϴ� ����. ����.
float GetSubOffsetFail(float _fFstPosVal , float _fMidPosVal , float _fLstPosVal , float _fPxLength) //x,y ���� ���� �������� �ȼ����� , x,y ���� �߰� �������� �ȼ����� , x,y ���� ū �������� �ȼ�����
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

//���� �ȼ� �����V ���ϴ� ����.
//�y������ -1 , 0 , 1 �������� diff �� �� �ȼ��� �ȼ����� �Ÿ�.
float GetSubOffset(float _fFstPosDif , float _fMidPosDif , float _fLstPosDif , float _fPxLength)
{
    //���߿� Ÿ�缺 ����.
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


    BYTE * pRaw ; //�ο� ������.
    BYTE * pAvr ; //��� ������.
    int  * pDif ; //���� ������(1���̺�)

    bool bThin  = abs(_tSttPnt.x - _tEndPnt.x) < abs(_tSttPnt.y - _tEndPnt.y);
    int  iRange = bThin ? _tEndPnt.y - _tSttPnt.y : _tEndPnt.x - _tSttPnt.x ; //����� -���̸� �ݴ���⿬��.

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
    const bool bCenterType = true ; //CenterType�� ���� �ȼ����� �¿� ��, �ٸ� Ÿ���� ��ä �ȼ����� ������ �� �ȼ� ���� ��.
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
    //���� ������
    if(laLineAngle == la270     ) { //�Ʒ��� ������ ����.
        for(int i = 0 ; i < abs(iRange) ; i++) {
            //���� �ȼ��� ��� äũ
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //�����V �� �ȼ��� ��� üũ.
            OffsetPx = _pImg -> GetPixel(_tSttPnt.x,_tSttPnt.y+i+_tPara.iOfsGap) ;
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }
    else if(laLineAngle == la90      ) { //���� ������ ����.
        for(int i = 0 ; i < abs(iRange) ; i++) {
            //���� �ȼ��� ��� äũ
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //�����V �� �ȼ��� ��� üũ.
            OffsetPx = _pImg -> GetPixel(_tSttPnt.x,_tSttPnt.y-i-_tPara.iOfsGap) ;
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }
    else if(laLineAngle == la225_315 ){ //������ ���ϴ� ����.
        for(int i = 0 ; i < abs(iRange) ; i++) {
            //���� �ȼ��� ��� äũ
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //�����V �� �ȼ��� ��� üũ.
            OffsetPx = _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_tSttPnt.y+i+_tPara.iOfsGap),_tSttPnt.y+i+_tPara.iOfsGap);
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }
    else if(laLineAngle == la45_135  ){ //���� ���ϴ� ����.
        for(int i = 0 ; i < abs(iRange) ; i++) {
            //���� �ȼ��� ��� äũ
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //�����V �� �ȼ��� ��� üũ.
            OffsetPx = _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_tSttPnt.y-i-_tPara.iOfsGap),_tSttPnt.y-i-_tPara.iOfsGap);
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }

    //�¿� ������
    else if(laLineAngle == la0       ){  //���������� ����
        for(int i = 0 ; i < abs(iRange) ; i++) {
            //���� �ȼ��� ��� äũ
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //�����V �� �ȼ��� ��� üũ.
            OffsetPx = _pImg -> GetPixel(_tSttPnt.x+i+_tPara.iOfsGap,_tSttPnt.y) ;
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }
    else if(laLineAngle == la180     ){ //�������� ����.
        for(int i = 0 ; i < abs(iRange) ; i++) {
            //���� �ȼ��� ��� äũ
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //�����V �� �ȼ��� ��� üũ.
            OffsetPx = _pImg -> GetPixel(_tSttPnt.x-i-_tPara.iOfsGap,_tSttPnt.y) ;
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }
    else if(laLineAngle == la315_45  ){ //���������� ���� ����.
        for(int i = 0 ; i < abs(iRange) ; i++) {
            //���� �ȼ��� ��� äũ
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //�����V �� �ȼ��� ��� üũ.                      _pImg -> GetSubPixel(_tSttPnt.x+i,MATH_GetLineY(_tSttPnt,_tEndPnt,_tSttPnt.x+i));}break ;
            OffsetPx = _pImg -> GetSubPixel(_tSttPnt.x+i+_tPara.iOfsGap , MATH_GetLineY(_tSttPnt,_tEndPnt,_tSttPnt.x+i+_tPara.iOfsGap));
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }
    else if(laLineAngle == la135_225 ){ //�������� ���� ����.
        for(int i = 0 ; i < abs(iRange) ; i++) {
            //���� �ȼ��� ��� äũ
            if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;
            //�����V �� �ȼ��� ��� üũ.
            OffsetPx = _pImg -> GetSubPixel(_tSttPnt.x-i-_tPara.iOfsGap , MATH_GetLineY(_tSttPnt,_tEndPnt,_tSttPnt.x-i-_tPara.iOfsGap));
            if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < OffsetPx && OffsetPx < _tPara.iOfsPxHgh)) continue ;

            iTemp1 = pDif[i] ;
            if(pDif[i] > iDkToLtVal) {iDkToLtVal = pDif[i] ; iDkToLtPos = i;}
            if(pDif[i] < iLtToDkVal) {iLtToDkVal = pDif[i] ; iLtToDkPos = i;}
        }
    }

    //��~~~~ ������ ��ü.
    //for(int i = 0 ; i < abs(iRange) ; i++) {
    //    if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < pRaw[i] && pRaw[i] < _tPara.iChkPxHgh)) continue ;  //���� �ȼ��� ��� äũ
    //    //if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < pRaw[i+_tPara.iOfsGap] && pRaw[i+_tPara.iOfsGap] < _tPara.iOfsPxHgh)) continue ; //�����V �� �ȼ��� ��� üũ. ��!!!!!!
    //    if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < pRaw[i+_tPara.iOfsGap] && pRaw[i+_tPara.iOfsGap] < _tPara.iOfsPxHgh)) continue ; //�����V �� �ȼ��� ��� üũ.
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
    int iRange  ; //����� -���̸� �ݴ���⿬��.
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

    // Vertical Line. ��հ� ����.
    if(_tSttPnt.x == _tEndPnt.x) {
        iRange = _tEndPnt.y - _tSttPnt.y ;                                            //������ ��ճ��� �Ķ���� ���� ������ ����.
        if(abs(iRange) <= _tPara.iAverage*2+1){g_sErrMsg="abs(iRange) <= _tPara.iAverage*2+1"; return false; }
        pAvr = new int[abs(iRange)];                                                  //������ ���� ��ŭ ��հ����� �޸� ���� ����.

        iStartY = iRange > 0 ? _tSttPnt.y : _tEndPnt.y ;                              //�������� ��ŸƮ�� �ǰ� �ϱ� ���� ó��.
        iStartX = _tSttPnt.x ;

        for(register int y = 0 ; y < abs(iRange) ; y++ ) {                            //���������� ��հ��迭�� ���� ��հ��� ����ؼ� ����.
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


        _pRslt->iDkToLtGap = (iRange > 0) ? -255 :  255 ;                           //���� �ְ��� ��ҿ��� �������� ���°� �� ���ϱ� ����
        _pRslt->iLtToDkGap = (iRange > 0) ?  255 : -255 ;                           //ó���� ���� ���� ���� �̸� �־��.

        for(register int y = 0 ; y < abs(iRange) ; y++ ) {     //���� �y���� ����.
           if((y - _tPara.iCompare) <  0          ) continue ;
           if((y + _tPara.iCompare) >= abs(iRange) ) break   ;

           Px = _pImg -> GetPixel(iStartX , iStartY + y) ;
           if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < Px && Px < _tPara.iChkPxHgh)) continue ; //���� �ȼ� ��� �ɼ� ����.

           PxOfs = _pImg -> GetPixel(iStartX ,(iRange < 0) ? iStartY + y - _tPara.iOfsGap : iStartY + y + _tPara.iOfsGap) ;
           if(_tPara.iOfsGap && !(_tPara.iOfsPxLow < PxOfs && PxOfs < _tPara.iOfsPxHgh)) continue ;
                                                                                                              //�����V �ȼ� ��� �ɼ� ����.
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


        //���� �ȼ�.
        _pRslt->DkToLtPntS.x = _pRslt->DkToLtPnt.x ;
        _pRslt->LtToDkPntS.x = _pRslt->LtToDkPnt.x ;
        //_pRslt->DkToLtPntS.y = _pRslt->DkToLtPnt.y +GetSubOffset(pAvr[(int)(_pRslt->DkToLtPnt.y - iStartY - 1)] ,pAvr[(int)(_pRslt->DkToLtPnt.y - iStartY)] ,pAvr[(int)(_pRslt->DkToLtPnt.y - iStartY + 1)],fPxLength);
        //_pRslt->LtToDkPntS.y = _pRslt->LtToDkPnt.y +GetSubOffset(pAvr[(int)(_pRslt->LtToDkPnt.y - iStartY - 1)] ,pAvr[(int)(_pRslt->LtToDkPnt.y - iStartY)] ,pAvr[(int)(_pRslt->LtToDkPnt.y - iStartY + 1)],fPxLength);
        _pRslt->DkToLtPntS.y = _pRslt->DkToLtPnt.y +GetSubOffset(_pImg -> GetPixel(_pRslt->DkToLtPnt.x,_pRslt->DkToLtPnt.y-1) ,_pImg -> GetPixel(_pRslt->DkToLtPnt.x,_pRslt->DkToLtPnt.y) , _pImg -> GetPixel(_pRslt->DkToLtPnt.x,_pRslt->DkToLtPnt.y+1),fPxLength);
        _pRslt->LtToDkPntS.y = _pRslt->LtToDkPnt.y +GetSubOffset(_pImg -> GetPixel(_pRslt->LtToDkPnt.x,_pRslt->LtToDkPnt.y-1) ,_pImg -> GetPixel(_pRslt->LtToDkPnt.x,_pRslt->LtToDkPnt.y) , _pImg -> GetPixel(_pRslt->LtToDkPnt.x,_pRslt->LtToDkPnt.y+1),fPxLength);


        //ROW������ ���� �ȼ�.
        //_pRslt->DkToLtPntS.x = _pRslt->DkToLtPnt.x ;
        //_pRslt->LtToDkPntS.x = _pRslt->LtToDkPnt.x ;
        //_pRslt->DkToLtPntS.y = _pRslt->DkToLtPnt.y +GetSubOffset(_pImg -> GetPixel(_pRslt->DkToLtPnt.x,_pRslt->DkToLtPnt.y-1) ,_pImg -> GetPixel(_pRslt->DkToLtPnt.x,_pRslt->DkToLtPnt.y) , _pImg -> GetPixel(_pRslt->DkToLtPnt.x,_pRslt->DkToLtPnt.y+1));
        //_pRslt->LtToDkPntS.y = _pRslt->LtToDkPnt.y +GetSubOffset(_pImg -> GetPixel(_pRslt->LtToDkPnt.x,_pRslt->LtToDkPnt.y-1) ,_pImg -> GetPixel(_pRslt->LtToDkPnt.x,_pRslt->LtToDkPnt.y) , _pImg -> GetPixel(_pRslt->LtToDkPnt.x,_pRslt->LtToDkPnt.y+1));

        //ROW������ ���� �ȼ�.
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

        //���� �ȼ�.
        //��հ� ���� �ȼ�.
        _pRslt->DkToLtPntS.y = _pRslt->DkToLtPnt.y ;
        _pRslt->LtToDkPntS.y = _pRslt->LtToDkPnt.y ;
        //_pRslt->DkToLtPntS.x = _pRslt->DkToLtPnt.x+GetSubOffset(pAvr[(int)(_pRslt->DkToLtPnt.x - iStartX - 1)] ,pAvr[(int)(_pRslt->DkToLtPnt.x - iStartX)] ,pAvr[(int)(_pRslt->DkToLtPnt.x - iStartX + 1)],fPxLength);
        //_pRslt->LtToDkPntS.x = _pRslt->LtToDkPnt.x+GetSubOffset(pAvr[(int)(_pRslt->LtToDkPnt.x - iStartX - 1)] ,pAvr[(int)(_pRslt->LtToDkPnt.x - iStartX)] ,pAvr[(int)(_pRslt->LtToDkPnt.x - iStartX + 1)],fPxLength);
        _pRslt->DkToLtPntS.x = _pRslt->DkToLtPnt.x+GetSubOffset(pAvr[(int)(_pRslt->DkToLtPnt.x - iStartX - 1)] ,pAvr[(int)(_pRslt->DkToLtPnt.x - iStartX)] ,pAvr[(int)(_pRslt->DkToLtPnt.x - iStartX + 1)],fPxLength);
        _pRslt->LtToDkPntS.x = _pRslt->LtToDkPnt.x+GetSubOffset(pAvr[(int)(_pRslt->LtToDkPnt.x - iStartX - 1)] ,pAvr[(int)(_pRslt->LtToDkPnt.x - iStartX)] ,pAvr[(int)(_pRslt->LtToDkPnt.x - iStartX + 1)],fPxLength);

        //Row�� ���� �ȼ�.
        //_pRslt->DkToLtPntS.y = _pRslt->DkToLtPnt.y ;
        //_pRslt->LtToDkPntS.y = _pRslt->LtToDkPnt.y ;
        //_pRslt->DkToLtPntS.x = _pRslt->DkToLtPnt.x+GetSubOffset(_pImg -> GetPixel(_pRslt->DkToLtPnt.x - 1,_pRslt->DkToLtPnt.y) ,_pImg -> GetPixel(_pRslt->DkToLtPnt.x,_pRslt->DkToLtPnt.y) ,_pImg -> GetPixel(_pRslt->DkToLtPnt.x + 1,_pRslt->DkToLtPnt.y));
        //_pRslt->LtToDkPntS.x = _pRslt->LtToDkPnt.x+GetSubOffset(_pImg -> GetPixel(_pRslt->LtToDkPnt.x - 1,_pRslt->LtToDkPnt.y) ,_pImg -> GetPixel(_pRslt->LtToDkPnt.x,_pRslt->LtToDkPnt.y) ,_pImg -> GetPixel(_pRslt->LtToDkPnt.x + 1,_pRslt->LtToDkPnt.y));

        //��Ʈ ���� ���� �ȼ�.
        //_pRslt->DkToLtPntSS.y = _pRslt->DkToLtPnt.y ;
        //_pRslt->LtToDkPntSS.y = _pRslt->LtToDkPnt.y ;
        //_pRslt->DkToLtPntSS.x = _pRslt->DkToLtPnt.x+GetSubOffsetS(_pImg -> GetPixel(_pRslt->DkToLtPnt.x - 1,_pRslt->DkToLtPnt.y) ,_pImg -> GetPixel(_pRslt->DkToLtPnt.x,_pRslt->DkToLtPnt.y) ,_pImg -> GetPixel(_pRslt->DkToLtPnt.x + 1,_pRslt->DkToLtPnt.y));
        //_pRslt->LtToDkPntSS.x = _pRslt->LtToDkPnt.x+GetSubOffsetS(_pImg -> GetPixel(_pRslt->LtToDkPnt.x - 1,_pRslt->LtToDkPnt.y) ,_pImg -> GetPixel(_pRslt->LtToDkPnt.x,_pRslt->LtToDkPnt.y) ,_pImg -> GetPixel(_pRslt->LtToDkPnt.x + 1,_pRslt->LtToDkPnt.y));

        delete[] pAvr ;
    }


    else if(IsThin) { //Thin Diagonal Line.
        iRange = _tEndPnt.y - _tSttPnt.y ;                                           //��꺹���� ���� �ӵ� �������� 1�� �������� �Ȼ̰� �Ѵ�.
        if(abs(iRange) <= _tPara.iAverage*2+1) {g_sErrMsg="abs(iRange) <= _tPara.iAverage*2+1"; return false; } ;
        pAvr = new int[abs(iRange)];                                                 //������ ���� ��ŭ ��հ����� �޸� ���� ����.

        iStartY = iRange > 0 ? _tSttPnt.y : _tEndPnt.y ;                             //�������� ��ŸƮ�� �ǰ� �ϱ� ���� ó��.
        iStartX = _tSttPnt.x ;

        float SubX    ; //������������ ���� Y���� �ش��ϴ� X��.
        float SubXOfs ; //SubX���� ���� �������� ���� Offset���� ��ŭ �̵��� X��.

        for(register int y = 0 ; y < abs(iRange) ; y++ ) {                          //���������� ��հ��迭�� ���� ��հ��� ����ؼ� ����.
           iDataCnt = 0 ;
           iSum     = 0 ;
           for(register int v = (-_tPara.iAverage) ; v <= _tPara.iAverage ; v++) {  //���ȼ��� ���� �ֺ� ��հ��� ���Ѵ�.
              if((y + v) <  0          ) continue ;                                 //��հ��� ��ŸƮ ���̳�
              if((y + v) >= abs(iRange) ) break    ;                                //���尪�� �Ѿ�� �ȱ��Ѵ�.

              SubX=MATH_GetLineX(_tSttPnt,_tEndPnt,(y+iStartY+v));
              iSum += _pImg -> GetSubPixel(SubX,(y+iStartY+v)) ;
              iDataCnt++;
           }
           pAvr[y] = iDataCnt ? iSum / iDataCnt : 0 ;
        }


        _pRslt->iDkToLtGap = (iRange > 0) ?  255 : -255 ; //���� �ְ��� ��ҿ��� �������� ���°� �� ���ϱ� ����
        _pRslt->iLtToDkGap = (iRange > 0) ? -255 :  255 ; //ó���� ���� ���� ���� �̸� �־��.


        float YPos ; //������ ���� ��ġ.
        float XPos ;
        for(register int y = 0 ; y < abs(iRange) ; y++ ) {     //���� �y���� ����.
           if((y - _tPara.iCompare) <  0          ) continue ;
           if((y + _tPara.iCompare) >= abs(iRange)) break    ;

           SubX=MATH_GetLineX(_tSttPnt,_tEndPnt,(y+iStartY));  //���� ���̰��� ���� X���� ���� ���´�.
           Px = _pImg -> GetSubPixel(SubX , y+iStartY) ;       //�ش� ��ǥ�� �ȼ� ��Ⱚ.
           if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < Px && Px < _tPara.iChkPxHgh)) continue ; //ã���� ��Ⱑ ����ƿ��̸� ��Ƽ��.

           MATH_GetPntFromPntByAngLen(SubX , y+iStartY                                                   , //�Է� ������.
                                      MATH_GetLineAngle(_tSttPnt.x, _tSttPnt.y, _tEndPnt.x , _tEndPnt.y) , //����.
                                      _tPara.iOfsGap                                                     , //�Է� ����.
                                      &XPos , &YPos                                                      );//��� ����Ʈ.
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


        //���� �ȼ�.                                //�ؿ� ��Ʈ ���߿� ���� �� ���� �ʿ� ����.
        //Row�� ���� �ȼ�.                          //GetSubOffset���� ���°��� �밢�� ���� ������ ���� ���� ȯ���Ͽ� ������� �Ѵ�. ���߿� �۾�. ex) 45�� �϶� ��Ʈ2
        _pRslt->DkToLtPntS.y = _pRslt->DkToLtPnt.y +GetSubOffset(_pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.y-1),_pRslt->DkToLtPnt.y-1) ,
                                                                 _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.y  ),_pRslt->DkToLtPnt.y  ) ,
                                                                 _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.y+1),_pRslt->DkToLtPnt.y+1),fPxLength);
        _pRslt->DkToLtPntS.x = MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->DkToLtPntS.y  ) ;

        _pRslt->LtToDkPntS.y = _pRslt->LtToDkPnt.y +GetSubOffset(_pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.y-1),_pRslt->LtToDkPnt.y-1) ,
                                                                 _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.y  ),_pRslt->LtToDkPnt.y  ) ,
                                                                 _pImg -> GetSubPixel(MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.y+1),_pRslt->LtToDkPnt.y+1),fPxLength);
        _pRslt->LtToDkPntS.x = MATH_GetLineX(_tSttPnt,_tEndPnt,_pRslt->LtToDkPntS.y  ) ;


        //��Ʈ ���� ���� �ȼ�.
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

        float SubY    ; //������������ ���� X���� �ش��ϴ� Y��.
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

        float YPos ; //������ ���� ��ġ.
        float XPos ;
        for(register int x = 0 ; x < abs(iRange) ; x++ ) {
           if((x - _tPara.iCompare) <  0          ) continue ;
           if((x + _tPara.iCompare) >= abs(iRange) ) break   ;

           SubY=MATH_GetLineY(_tSttPnt,_tEndPnt,(x+iStartX));
           Px = _pImg -> GetSubPixel(iStartX + x , SubY) ;
           if((_tPara.iChkPxLow || _tPara.iChkPxHgh) && !(_tPara.iChkPxLow < Px && Px < _tPara.iChkPxHgh)) continue ;


           if(_tPara.iOfsGap) {
               MATH_GetPntFromPntByAngLen(x+iStartX , SubY , // , y+iStartY                                                   , //�Է� ������.
                                          MATH_GetLineAngle(_tSttPnt.x, _tSttPnt.y, _tEndPnt.x , _tEndPnt.y) , //����.
                                          _tPara.iOfsGap                                                     , //�Է� ����.
                                          &XPos , &YPos                                                      );//��� ����Ʈ.
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

        //Row�� ���� �ȼ�.
        _pRslt->DkToLtPntS.x = _pRslt->DkToLtPnt.x +GetSubOffset(_pImg -> GetSubPixel(_pRslt->DkToLtPnt.x-1 ,MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.x-1)) ,
                                                                 _pImg -> GetSubPixel(_pRslt->DkToLtPnt.x   ,MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.x  )) ,
                                                                 _pImg -> GetSubPixel(_pRslt->DkToLtPnt.x+1 ,MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.x+1)),fPxLength);
        _pRslt->DkToLtPntS.y = MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->DkToLtPnt.x  ) ;

        _pRslt->LtToDkPntS.x = _pRslt->LtToDkPntS.x +GetSubOffset(_pImg -> GetSubPixel(_pRslt->LtToDkPnt.x-1 ,MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.x-1)) ,
                                                                  _pImg -> GetSubPixel(_pRslt->LtToDkPnt.x   ,MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.x  )) ,
                                                                  _pImg -> GetSubPixel(_pRslt->LtToDkPnt.x+1 ,MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.x+1)),fPxLength);
        _pRslt->LtToDkPntS.y = MATH_GetLineY(_tSttPnt,_tEndPnt,_pRslt->LtToDkPnt.x  ) ;


        //��Ʈ ���� ���� �ȼ�.
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


//���߿� TFPoint �� �⺻���̰� TPoint�� �������̵� �Ǿ���.
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

