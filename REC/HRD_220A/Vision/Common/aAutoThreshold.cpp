
//---------------------------------------------------------------------------
#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------
#include "aAutoThreshold.h"
#include "BaseDefine.h"

CAutoThreshold::CAutoThreshold()
{
}

CAutoThreshold::~CAutoThreshold()
{

}


bool CAutoThreshold::Inspection(CImage * _pImg , TRect _tRect , TPara _tPara , TRslt * _pRslt) //그냥 단순하게 평균값으로 가져오는 알고리즘.
{
    DWORD dwTime =GetTime();
    _pRslt -> Clear();

    int iWidth   = _tRect.Width() ;
    int iHeight  = _tRect.Height();

    int iTemp = 0;

    int iAvrGray = 0 ;
    int iSumGray = 0 ;

    int iAllPxCnt = iWidth * iHeight ;

    if(iAllPxCnt <= 0 ) {
        _pRslt->sErrMsg = "Inspection Area is 0";

        return false ;
    }

    //double * dGrayCnt = new double[];
    int iGrayCnt[256];

    memset(iGrayCnt , 0 , sizeof(int) * 256);

    for(int x = _tRect.left ; x < _tRect.right ; x++){
        for(int y = _tRect.top ; y < _tRect.bottom ; y++){
            iTemp = _pImg -> GetPixel(x,y);
            iGrayCnt[iTemp]++ ;
            iSumGray += iTemp ;
        }
    }

    iAvrGray = iSumGray / iAllPxCnt ;

    if(_tPara.bSimple) {
        _pRslt->iThreshold = iAvrGray ;
        _pRslt->dInspTime  = GetTime() - dwTime ;
        return iAvrGray ;
    }

    int i = 0 , iAutoThresh = 0;
    double dBig = 0 , dResult = 0 ;
    double dGrayPrcnt1 = 0 ;
    double dGrayPrcnt2 = 0 ;
    double dGrayW1 = 0 , dGrayU1 = 0 , dGrayA1 = 0 ,
           dGrayW2 = 0 , dGrayU2 = 0 , dGrayA2 = 0 ;

    for(i = 0 ; i <= iAvrGray ; i++){

        if(iAvrGray+1+i < 256){
            dGrayPrcnt2 = iGrayCnt[iAvrGray+1+i]/(double)iAllPxCnt ;
            dGrayW2     = dGrayW2 +  dGrayPrcnt2                ;
            dGrayA2     = dGrayA2 + (dGrayPrcnt2 * (i+1+i))     ;
            dGrayU2     = dGrayU2 + (dGrayA2 / dGrayW2) * (dGrayA2 / dGrayW2) ;
        }
        dGrayPrcnt1 = iGrayCnt[i]/(double)iAllPxCnt ;
        dGrayW1     = dGrayW1 +  dGrayPrcnt1                ;
        dGrayA1     = dGrayA1 + (dGrayPrcnt1 *  i     )     ;
        dGrayU1     = dGrayU1 + (dGrayA1 / dGrayW1) * (dGrayA1 / dGrayW1) ;

        dResult = (((1-dGrayPrcnt1)*dGrayW1*dGrayU1) + (dGrayW2 * dGrayU2)) ;

        if(dBig < dResult) { dBig = dResult ; iAutoThresh = i ; }

    }

    _pRslt->iThreshold = iAutoThresh ;
    _pRslt->dInspTime  = GetTime() - dwTime ;

    return true ;

}

