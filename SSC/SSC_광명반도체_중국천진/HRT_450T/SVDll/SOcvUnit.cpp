//---------------------------------------------------------------------------
#pragma hdrstop
#include "SOcvUnit.h"
#include "SGdiUnit.h"
#include "SMath.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//0=검사아직안함 , 1=다크 , 2=라이트  , 3=검사하지않는 영역.
enum EN_AREA_TYPE { atNone = 0 , atDark , atLight , atNoInsp };


SOcv::SOcv()
{
    m_pTrainImg  = new SImage ;
    m_pInspArea  = new SArea  ;

    Rslt.iDkErrPx  = 0 ;
    Rslt.iLtErrPx  = 0 ;
    Rslt.fInspTime = 0.0 ;
    Rslt.DkFailPx.DeleteAll()  ;
    Rslt.LtFailPx.DeleteAll()  ;
    Rslt.iStartX = 0 ;
    Rslt.iStartY = 0 ;

}

SOcv::~SOcv()
{
    delete m_pTrainImg  ;
    delete m_pInspArea  ;

    Rslt.DkFailPx.DeleteAll()  ;
    Rslt.LtFailPx.DeleteAll()  ;
}

int SOcv::OnTrain(SImage * _pImg , TRect _Rect)
{
    int iLeft   = _Rect.Left    ;
    int iTop    = _Rect.Top     ;
    int iRight  = _Rect.Right   ;
    int iBottom = _Rect.Bottom  ;
    int iWidth  = _Rect.Width ();
    int iHeight = _Rect.Height();
    int iImgBit = _pImg->GetImgBit() ;


    m_pTrainImg  -> SetSize(iWidth,iHeight,iImgBit) ;
    m_pInspArea  -> SetSize(iWidth,iHeight        ) ;

    //이미지 복사.
    for(int x = 0 ; x < iWidth ; x++){
        for(int y = 0 ; y < iHeight ; y++) {
            m_pTrainImg -> SetPixel(x,y,_pImg->GetPixel(iLeft+x,iTop+y)) ;
            if(iImgBit==24){
                m_pTrainImg -> SetPixelR(x,y,_pImg->GetPixelR(iLeft+x,iTop+y)) ;
                m_pTrainImg -> SetPixelG(x,y,_pImg->GetPixelG(iLeft+x,iTop+y)) ;
                m_pTrainImg -> SetPixelB(x,y,_pImg->GetPixelB(iLeft+x,iTop+y)) ;
            }
        }
    }


    int iSum = 0 ;
    for(int x = 0 ; x < iWidth ; x++){
        for(int y = 0 ; y < iHeight ; y++) {
            iSum += m_pTrainImg -> GetPixel(x,y) ;
        }
    }
    Para.iThreshold = iSum / (iWidth * iHeight) ;









    //검사영역 설정.  0=검사아직안함 , 1=다크 , 2=라이트  , 3=검사하지않는 영역.
    unsigned char cPpx  = m_pTrainImg -> GetPixel(0,0) ;
    unsigned char cCpx  = m_pTrainImg -> GetPixel(0,0) ;
    unsigned char cInsp = m_pInspArea -> GetPixel(0,0) ;

    for(int y = 0 ; y < iHeight ; y++) {
        cPpx = m_pTrainImg -> GetPixel(0,y) ;
        for(int x = 0 ; x < iWidth ; x++){
            cCpx  = m_pTrainImg -> GetPixel(x,y) ;
            cInsp = m_pInspArea -> GetPixel(x,y) ;
            if(cInsp != atNoInsp) {
                if(cCpx > Para.iThreshold ) m_pInspArea -> SetPixel(x,y,atLight) ;
                else                        m_pInspArea -> SetPixel(x,y,atDark ) ;
            }

            //엦지 발견.
            if((cCpx >  Para.iThreshold && cPpx <= Para.iThreshold) || (cCpx <= Para.iThreshold && cPpx >  Para.iThreshold)) {
                for(int i = x - Para.iPxOffset ; i <= x + Para.iPxOffset ;i++) {
                    for(int j = y - Para.iPxOffset ; j <= y + Para.iPxOffset ;j++) {
                        if(i<0 || j<0 || i>=m_pTrainImg -> GetWidth() || j>=m_pTrainImg -> GetHeight()) continue ;
                        if(Math.GetCircleInPoint(x , y , Para.iPxOffset , i , j)){
                            m_pInspArea -> SetPixel(i,j,atNoInsp) ;
                        }
                    }
                }
            }
            cPpx = cCpx ;
        }
    }


    for(int x = 0 ; x < iWidth ; x++){
        cPpx = m_pTrainImg -> GetPixel(x,0) ;
        for(int y = 0 ; y < iHeight ; y++) {
            cCpx  = m_pTrainImg -> GetPixel(x,y) ;
            cInsp = m_pInspArea -> GetPixel(x,y) ;
            if(cInsp != atNoInsp) {
                if(cCpx > Para.iThreshold ) m_pInspArea -> SetPixel(x,y,atLight) ;
                else                        m_pInspArea -> SetPixel(x,y,atDark ) ;
            }

            //엦지 발견.
            if((cCpx >  Para.iThreshold && cPpx <= Para.iThreshold) || (cCpx <= Para.iThreshold && cPpx >  Para.iThreshold)) {
                for(int i = x - Para.iPxOffset ; i <= x + Para.iPxOffset ;i++) {
                    for(int j = y - Para.iPxOffset ; j <= y + Para.iPxOffset ;j++) {
                        if(i<0 || j<0 || i>=m_pTrainImg -> GetWidth() || j>=m_pTrainImg -> GetHeight()) continue ;
                        if(Math.GetCircleInPoint(x , y , Para.iPxOffset , i , j)){
                            m_pInspArea -> SetPixel(i,j,atNoInsp) ;
                        }
                    }
                }
            }
            cPpx = cCpx ;
        }
    }
}

int SOcv::OnInspection(SImage * _pImg , int _iSx , int _iSy)
{
    Rslt.iDkErrPx  = 0 ;
    Rslt.iLtErrPx  = 0 ;
    Rslt.fInspTime = 0.0 ;
    Rslt.DkFailPx.DeleteAll()  ;
    Rslt.LtFailPx.DeleteAll()  ;
    Rslt.iStartX = _iSx ;
    Rslt.iStartY = _iSy ;

    double dTime = GetTickTime() ;

    int iWidth  = m_pInspArea -> GetWidth () ;
    int iHeight = m_pInspArea -> GetHeight() ;

    for(int x = 0 ; x < iWidth ; x++) {
        for(int y = 0 ; y < iHeight ; y++) {
            if(m_pInspArea -> GetPixel(x,y) == atNoInsp) continue ;
            if(m_pInspArea -> GetPixel(x,y) == atDark  ) {
                 if(_pImg -> GetPixel(_iSx+x,_iSy+y) > Para.iThreshold){
                     Rslt.iDkErrPx ++ ;
                     Rslt.DkFailPx.PushFrnt(TPoint(x,y));
                 }
            }

            else if(m_pInspArea -> GetPixel(x,y) == atLight ) {
                 if(_pImg -> GetPixel(_iSx+x,_iSy+y) <= Para.iThreshold){
                     Rslt.iLtErrPx ++ ;
                     Rslt.LtFailPx.PushFrnt(TPoint(x,y));
                 }
            }
            else continue ;
        }
    }

    dTime = GetTickTime() - dTime ;

    Rslt.fInspTime = Math.RoundOff(dTime,3);
}

void SOcv::OnPaintRslt (HDC _hHdc)
{
    if(Rslt.fInspTime == 0.0) return ;

    SGdi *Gdi = new SGdi(_hHdc);

    Gdi->CPen  .Color = clRed ;
    Gdi->CBrush.Color = clRed ;
    Gdi->CPen  .Width = 1     ;

    TPoint Pnt ;
    int iPxCnt = 0 ;
    for(int i = 0 ; i < Rslt.DkFailPx.GetDataCnt() ; i++){
        Pnt = Rslt.DkFailPx.GetCrntData(!i);
        Pnt.x += Rslt.iStartX ;
        Pnt.y += Rslt.iStartY ;
        if(!(iPxCnt++%2))Gdi -> Pixel(Pnt,clBlack) ;
    }

    for(int i = 0 ; i < Rslt.LtFailPx.GetDataCnt() ; i++){
        Pnt = Rslt.LtFailPx.GetCrntData(!i);
        Pnt.x += Rslt.iStartX ;
        Pnt.y += Rslt.iStartY ;
        if(!(iPxCnt++%2))Gdi -> Pixel(Pnt,clWhite) ;
    }
    delete Gdi ;
}

void SOcv::OnPaintTrain(HDC _hHdc)
{
    if(!m_pTrainImg -> GetHeight()) return ;

    if(m_pTrainImg -> GetImgBit() == 8) m_pTrainImg->ShowImgGrey  (_hHdc, 0 , 0 , m_pTrainImg -> GetWidth() , m_pTrainImg -> GetHeight());
    else                                m_pTrainImg->ShowImgColor (_hHdc, 0 , 0 , m_pTrainImg -> GetWidth() , m_pTrainImg -> GetHeight());

    SGdi * Gdi = new SGdi(_hHdc);

    int iPxCnt = 0 ;

    for(int x = 0 ; x < m_pInspArea -> GetWidth() ; x++) {
        for(int y = 0 ; y < m_pInspArea -> GetHeight() ; y++) {
            if(m_pInspArea -> GetPixel(x,y) == atNoInsp) {
                Gdi -> Pixel(x,y,clRed) ;

            }
            else if(m_pInspArea -> GetPixel(x,y) == atDark  ) {
                /*if(!(iPxCnt++%7))*/Gdi -> Pixel(x,y,clBlack) ;

            }

            else if(m_pInspArea -> GetPixel(x,y) == atLight ) {
                /*if(!(iPxCnt++%7))*/Gdi -> Pixel(x,y,clWhite) ;
            }
            else continue ;
        }
    }
    
    delete Gdi ;



}

bool SOcv::OnLoad(AnsiString _sFilePath)
{

}

bool SOcv::OnSave(AnsiString _sFilePath)
{


}










