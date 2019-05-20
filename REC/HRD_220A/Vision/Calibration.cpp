//---------------------------------------------------------------------------
#pragma hdrstop

#include "Calibration.h"
#include "UnitGdi.h"
#include "BaseMaths.h"
#include "aAutoThreshold.h"
#include "aBlob.h"
#include "GlobalMan.h"

//#include "SLogUnit.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)

const double DOMAIN_OFFSET = 1000.0 ;

//일단 등록안한다.
//REGIST_STATICLLINK_CPP(CCalibration)


CCalibration::CCalibration()
{

}

CCalibration::~CCalibration()
{
}

void CCalibration::OnTrackerChanged()
{
     //화면에서 다시 그리므로 여기서 메세지 날릴 필요 없다.
}

//순수 가상 함수.
//==============================================================================
bool CCalibration::Init()
{
    Prop.bUseTrain = false ;
    Prop.bCamera   = false ;

    //이 블럭 은 나중에 트렉커 많아지면 같이 증가해야 함..
    CTracker * Tracker = new CTracker();
    Tracker -> SetOnPaint(OnTrackerChanged);
    m_lTrckInsp.PushBack(Tracker);
    m_lLPara   .PushBack(new TLocalPara ());

    m_pCalImg = new CImage();



    return true ;

}
bool CCalibration::Close()
{
    int iTrackerCnt = m_lTrckInsp.GetDataCnt();
    for(int i = 0 ; i < iTrackerCnt ; i++) {
        delete m_lTrckInsp.GetCrntData(!i);
    }
    m_lTrckInsp.DeleteAll();

    int iLParaCnt = m_lLPara.GetDataCnt();
    for(int i = 0 ; i < iLParaCnt ; i++) {
        delete m_lLPara.GetCrntData(!i);
    }
    m_lLPara.DeleteAll();

    if(m_pCalImg){delete m_pCalImg ; m_pCalImg = NULL ;}

    Rslt.Clear();


    return true ;

}

CImage * CCalibration::GetCalImg()
{
    return m_pCalImg ;

}


        //검사 및 러닝 관련.
bool CCalibration::Train(CImage * _pImg)
{
    m_sErrMsg = "";

    return true ;
}

void CCalibration::RsltClear() //검사 결과값을 검사전에 클리어 한번 하고 한다.
{
    Rslt.Clear();
}

bool CCalibration::GetRslt()
{
    return Rslt.bRsltOk ;
}

double CCalibration::GetPxResolSimp()
{
    return Rslt.dPxResolSimp ;
}
void CCalibration::SetPxResolSimp(double _dResol)
{
    Rslt.dPxResolSimp = _dResol ;
}
double CCalibration::GetPxResolLenz()
{
    return Rslt.dPxResolLenz ;
}
void CCalibration::SetPxResolLenz(double _dResol)
{
    Rslt.dPxResolLenz = _dResol ;
}
double CCalibration::GetPxResol()
{
    return MPara.bUseSimpleCal ? Rslt.dPxResolSimp : Rslt.dPxResolLenz;
}

bool CCalibration::FindBlob(CImage * _pImg)
{
    m_sErrMsg = "" ;

    Rslt.Clear();

    if(_pImg == NULL) _pImg = m_pCalImg ;
    if(_pImg == NULL) {
        m_sErrMsg = "Image is Empty" ;
        return false ;
    }
    
    TRect Rect ;
    CTracker * Tracker = m_lTrckInsp.GetCrntData(true) ;
    Rect = Tracker -> GetRect() ;
    _pImg -> SetRect(&Rect); //검사 네모가 이미지 밖으로 나갔을때 이미지를 벋어나지 않게 사이즈 줄여 준다.


    //Threshold 추출...
    int iThreshold = CPara.iThreshold ;
    if(iThreshold==0){
        CAutoThreshold::TPara ATPara;
        CAutoThreshold::TRslt ATRslt;
        ATPara.bSimple = true ;
        if(CAutoThreshold::Inspection(_pImg , Rect , ATPara , &ATRslt)){ //오토쓰레숄드값을 가져오는 알고리즘
            iThreshold = ATRslt.iThreshold ; //쓰레숄드 세팅이 0이면 오토쓰레숄드를 뽑아 온다.
        }
        else {
            m_sErrMsg = ATRslt.sErrMsg;
            return false ;
        }
    }

    //처음불럽 추출.
    CBlob::TPara BLPara ;
    CBlob::TFltr BLFltr ;

    BLPara.dPitchX     = CPara.dPitchX      ; //블랍 검색 X 피치
    BLPara.dPitchY     = CPara.dPitchY      ; //블랍 검색 Y 피치
    BLPara.iThreshold  = CPara.iThreshold   ; //검색 트레숄드.
    BLPara.bFindLtOnDk = CPara.bFindLtOnDk  ; //검은바탕흰물체인지 흰바탕 검은물체인지.
    BLPara.bIgnrInside = CPara.bIgnrInside  ; //블랍 내부 블랍 무시.
    BLPara.bIgnrSide   = CPara.bIgnrSide    ; //사각형 테투리 경계에 있는 놈들 무시.
    BLPara.iStartXOfs  = 0                  ; //검사 시작 오프셑
    BLPara.iStartYOfs  = 0                  ; //검사 시작 오프셑.

    BLFltr.iMinArea    = CPara.iMinArea     ;
    BLFltr.iMaxArea    = CPara.iMaxArea     ; //에리아 민맥스값
    BLFltr.iMinWidth   = CPara.iMinWidth    ;
    BLFltr.iMaxWidth   = CPara.iMaxWidth    ; //넓이
    BLFltr.iMinHeight  = CPara.iMinHeight   ;
    BLFltr.iMaxHeight  = CPara.iMaxHeight   ; //놉이
    BLFltr.dMinBoundary= CPara.dMinBoundary ;
    BLFltr.dMaxBoundary= CPara.dMaxBoundary ; //테두리
    BLFltr.dMinCompact = CPara.dMinCompact  ;
    BLFltr.dMaxCompact = CPara.dMaxCompact  ; //에리아/테두리

    if(!CBlob::Inspect(_pImg , Rect , BLPara , BLFltr , &Rslt.BlobRslt)){
        m_sErrMsg = Rslt.BlobRslt.sErrMsg ;
        return false ;
    }

    if(Rslt.BlobRslt.iBlobCnt < 6){
        Rslt.bBlobEnd = true ;
        m_sErrMsg = "Needed More Point Than 6" ;
        return false ;
    }

    CBlob::TBlob * FrstBlob = Rslt.BlobRslt.plBlob.GetData(0);
    CBlob::TBlob * LastBlob = Rslt.BlobRslt.plBlob.GetData(0);

    int iColCnt = 0 ;
    int iRowCnt = 0 ;
    int iRowBandTop = FrstBlob->top    ;
    int iRowBandBtm = FrstBlob->bottom ;
    int iColBandLft = FrstBlob->left   ;
    int iColBandRgt = FrstBlob->right  ;
    int iFirstRowY  = FrstBlob->iCenterY ;
    int iLastRowY ;

    for (int i = 0 ; i < Rslt.BlobRslt.iBlobCnt ; i++ ) {
        LastBlob = Rslt.BlobRslt.plBlob.GetCrntData(!i) ;
        if(iRowBandTop < LastBlob->dCentroidY && iRowBandBtm > LastBlob->dCentroidY) iColCnt++ ;
        if(iColBandLft < LastBlob->dCentroidX && iColBandRgt > LastBlob->dCentroidX) iRowCnt++ ;
        iLastRowY = LastBlob->iCenterY ;
    }

    if( iRowCnt < 2)                                {m_sErrMsg = "Needed more Row than 2" ; Rslt.bBlobEnd = true ;return false ; }
    if( iColCnt < 2)                                {m_sErrMsg = "Needed more Col than 2" ; Rslt.bBlobEnd = true ;return false ; }
    if( Rslt.BlobRslt.iBlobCnt != iColCnt * iRowCnt){m_sErrMsg = "There is Some Noise or Angle is Wrong!"  ; Rslt.bBlobEnd = true ;return false ; }


    //이번엔 블랍 순서대로 소팅을 위해서 수행한다.
    BLPara.iStartXOfs = 0 ;
    BLPara.iStartYOfs = iFirstRowY - Rect.top ;
    BLPara.dPitchX    = 1.0 ;
    BLPara.dPitchY    = (iLastRowY - iFirstRowY)/ (double)(iRowCnt -1) ;

    if(!CBlob::Inspect(_pImg , Rect , BLPara , BLFltr , &Rslt.BlobRslt)){
        m_sErrMsg = Rslt.BlobRslt.sErrMsg ;
        return false ;
    }

    FrstBlob = Rslt.BlobRslt.plBlob.GetData(0);
    LastBlob = Rslt.BlobRslt.plBlob.GetData(0);
    iColCnt = 0 ;
    iRowCnt = 0 ;
    iRowBandTop = FrstBlob->top    ;
    iRowBandBtm = FrstBlob->bottom ;
    iColBandLft = FrstBlob->left   ;
    iColBandRgt = FrstBlob->right  ;
    iFirstRowY  = FrstBlob->iCenterY ;
    iLastRowY ;
    for (int i = 0 ; i < Rslt.BlobRslt.iBlobCnt ; i++ ) {
        LastBlob = Rslt.BlobRslt.plBlob.GetCrntData(!i) ;
        if(iRowBandTop < LastBlob->dCentroidY && iRowBandBtm > LastBlob->dCentroidY) iColCnt++ ;
        if(iColBandLft < LastBlob->dCentroidX && iColBandRgt > LastBlob->dCentroidX) iRowCnt++ ;
        iLastRowY = LastBlob->iCenterY ;
    }
    if( iRowCnt < 2)                                {m_sErrMsg = "2nd Needed more Row than 2" ; Rslt.bBlobEnd = true ;return false ; }
    if( iColCnt < 2)                                {m_sErrMsg = "2nd Needed more Col than 2" ; Rslt.bBlobEnd = true ;return false ; }
    if( Rslt.BlobRslt.iBlobCnt != iColCnt * iRowCnt){m_sErrMsg = "2nd There is Some Noise or Angle is Wrong!"  ; Rslt.bBlobEnd = true ;return false ; }

    
    
    



    double dLTCenterX = Rslt.BlobRslt.plBlob.GetData(        0)->dCentroidX ;
    double dLTCenterY = Rslt.BlobRslt.plBlob.GetData(        0)->dCentroidY ;
    double dRTCenterX = Rslt.BlobRslt.plBlob.GetData(iColCnt-1)->dCentroidX ;
    double dRTCenterY = Rslt.BlobRslt.plBlob.GetData(iColCnt-1)->dCentroidY ;
    double dLBCenterX = Rslt.BlobRslt.plBlob.GetData(iColCnt * (iRowCnt-1))->dCentroidX ;
    double dLBCenterY = Rslt.BlobRslt.plBlob.GetData(iColCnt * (iRowCnt-1))->dCentroidY ;
    double dRBCenterX = Rslt.BlobRslt.plBlob.GetData(iColCnt * iRowCnt - 1)->dCentroidX ;
    double dRBCenterY = Rslt.BlobRslt.plBlob.GetData(iColCnt * iRowCnt - 1)->dCentroidY ;
    double dPxLength ;
    double dMmLength ;

    Rslt.dCalPadAngle =(CMath::GetLineAngle(dRTCenterX , dRTCenterY , dLTCenterX , dLTCenterY)+
                        CMath::GetLineAngle(dRBCenterX , dRBCenterY , dLBCenterX , dLBCenterY))/2.0 ;

    Rslt.dCalPadAngle -= 180.0;

    //각도 오차는 계산 안함. 캘 할때 각도 봐가면서 맞춰야 함.
    //일단 캘패드 픽셀 거리.
    dPxLength = CMath::GetLineLenth(dLTCenterX , dLTCenterY , dRBCenterX , dRBCenterY);
    dMmLength = CMath::GetLineLenth(0          , 0          , iColCnt * CPara.dDotPitch,iRowCnt * CPara.dDotPitch);

    if(dPxLength)Rslt.dPxResolSimp = dMmLength / dPxLength;

    Rslt.iRowCnt = iRowCnt ;
    Rslt.iColCnt = iColCnt ;

    Rslt.bBlobEnd = true ;

    return true ;


}


bool CCalibration::Run(CImage * _pImg)
{
    if(m_bSkip) {
        Rslt.bInspEnd = true ;
        Rslt.bRsltOk = true ;
        return true ;
    }

    m_sErrMsg = "";

    if(_pImg == NULL) _pImg = m_pCalImg ;
    if(_pImg == NULL) {
        m_sErrMsg = "Image is Empty" ;
        return false ;
    }
    if(_pImg->GetWidth() == 0 || _pImg->GetHeight() == 0) {
        m_sErrMsg = "Check Image's Width and Height" ;
        return false ;
    }


    Rslt.Clear();  //결과값 클리어.

    TRect Rect ;
    CTracker * Tracker = m_lTrckInsp.GetCrntData(true) ;
    Rect = Tracker -> GetRect() ;

    _pImg -> SetRect(&Rect); //검사 네모가 이미지 밖으로 나갔을때 이미지를 벋어나지 않게 사이즈 줄여 준다.


    if(!FindBlob(_pImg)) return false ;

    //Tsai Cal-------------------------------------------------------------.
    double dTempX ;
    double dTempY ;

    cp = &CP;
    cd = &CD;
    cc = &CC;

    CP.Ncx = _pImg -> GetWidth()        ;    /* [sel]     Number of sensor elements in camera's x direction         X축 센서소자 개수             */
    CP.Nfx = _pImg -> GetWidth()        ;    /* [pix]     Number of pixels in frame grabber's x direction           프레임그래버X방향 화소수      */
    CP.dx  = CPara.dCellSize            ;    /* [mm/sel]  X dimension of camera's sensor element (in mm)            카메라센서 소자하나의 X사이즈 */
    CP.dy  = CPara.dCellSize            ;    /* [mm/sel]  Y dimension of camera's sensor element (in mm)            카메라센서 소자하나의 Y사이즈 */
    CP.dpx = CP.dx * CP.Ncx / CP.Nfx    ;    /* [mm/pix]  [mm/pix]  Effective X dimension of pixel in frame grabber   */
    CP.dpy = CP.dy                      ;    /* [mm/pix]  [mm/pix]  Effective Y dimension of pixel in frame grabber   */
    CP.Cx  = _pImg -> GetWidth() /2.0   ;    /* [pix]     Z axis intercept of camera coordinate system              센서평면의 Z축 절편*/
    CP.Cy  = _pImg -> GetHeight()/2.0   ;    /* [pix]     Z axis intercept of camera coordinate system              */
    CP.sx  = 1.0                        ;

    int iBlobIdx ;

    memset(&CD , 0 , sizeof(calibration_data));

    CBlob::TBlob * pBlob ;

    for (int r = 0 ; r < Rslt.iRowCnt ; r++ ) {
        for (int c = 0 ; c < Rslt.iColCnt ; c++ ) {
            //월드좌표 즉 캘패드 치수를 이용한 mm 좌표.
            iBlobIdx = Rslt.iColCnt*r +c ;
            CD.xw[iBlobIdx] = c * CPara.dDotPitch + DOMAIN_OFFSET ;
            CD.yw[iBlobIdx] = r * CPara.dDotPitch + DOMAIN_OFFSET ;
            CD.zw[iBlobIdx] = 0.0 ;

            //이미지상의 픽셀 좌표.
            pBlob = Rslt.BlobRslt.plBlob.GetCrntData(!iBlobIdx) ;
            CD.Xf[iBlobIdx] = pBlob->dCentroidX +1;
            CD.Yf[iBlobIdx] = pBlob->dCentroidY +1;

        }
    }
    CD.point_count = Rslt.iRowCnt * Rslt.iColCnt;
    coplanar_calibration_with_full_optimization();

    


    image_coord_to_world_coord(0.0,
                               0.0,
                               0.0,
                               &Rslt.dWorldSttX ,
                               &Rslt.dWorldSttY );

    image_coord_to_world_coord(_pImg ->GetWidth()-1  ,
                               _pImg ->GetHeight()-1 ,
                               0.0,
                               &Rslt.dWorldEndX ,
                               &Rslt.dWorldEndY );

    //분자 얘외처리 하기.
    Rslt.dMmResolLenz = (_pImg -> GetWidth()-1) / (Rslt.dWorldEndX - Rslt.dWorldSttX);
    Rslt.dPxResolLenz = (Rslt.dWorldEndX - Rslt.dWorldSttX) / (_pImg -> GetWidth()-1);

    //이미지가 원래. dWorldSttX,Y 기준으로 정열된것을 카메라 센터로 바꿔주는것.=
    double dWorldCntXpx ;
    double dWorldCntYpx ;
    image_coord_to_world_coord(_pImg ->GetWidth()/2.0, _pImg ->GetHeight()/2.0, 0.0, &dWorldCntXpx,&dWorldCntYpx);
    dWorldCntXpx -= Rslt.dWorldSttX;
    dWorldCntYpx -= Rslt.dWorldSttY;
    dWorldCntXpx /= Rslt.dPxResolLenz ;
    dWorldCntYpx /= Rslt.dPxResolLenz ;
    Rslt.dImgToWldCntOfsX = dWorldCntXpx - _pImg ->GetWidth ()/2.0 ;
    Rslt.dImgToWldCntOfsY = dWorldCntYpx - _pImg ->GetHeight()/2.0 ;


    //=========================================================================

    Rslt.apCalPnt = new TDPoint[Rslt.iRowCnt * Rslt.iColCnt];
    int iDataIdx = 0 ;
    for (register int r = 0 ; r < Rslt.iRowCnt ; r++ ) {
        for (register int c = 0 ; c < Rslt.iColCnt ; c++ ) {
            iDataIdx = Rslt.iColCnt*r +c ;
            image_coord_to_world_coord(CD.Xf[iDataIdx],CD.Yf[iDataIdx],0.0,&Rslt.apCalPnt[iDataIdx].x,&Rslt.apCalPnt[iDataIdx].y);
            Rslt.apCalPnt[iDataIdx].x -= Rslt.dWorldSttX;
            Rslt.apCalPnt[iDataIdx].y -= Rslt.dWorldSttY;

            Rslt.apCalPnt[iDataIdx].x /= Rslt.dPxResolLenz ;
            Rslt.apCalPnt[iDataIdx].y /= Rslt.dPxResolLenz ;

            Rslt.apCalPnt[iDataIdx].x -= Rslt.dImgToWldCntOfsX ;
            Rslt.apCalPnt[iDataIdx].y -= Rslt.dImgToWldCntOfsY ;
        }
    }

    //마지막 켈리브레이션 이미지 복사.
    //GetCalImgFromImg(m_pCalImg , m_pCalImg);



    Rslt.bRsltOk = true ;

    Rslt.bInspEnd = true ;

    return true ;


}

bool CCalibration::GetCalImgFromImg(CImage *_pSrcImg , CImage * _pDstImg)
{
    if(_pSrcImg->GetWidth()  != _pDstImg->GetWidth()||
       _pSrcImg->GetHeight() != _pDstImg->GetHeight()){
        _pDstImg -> SetSize(_pSrcImg->GetWidth() , _pSrcImg->GetHeight(),8) ;

    }

    CImage * TempImg = new CImage();
    TempImg->SetSize(_pSrcImg->GetWidth() , _pSrcImg->GetHeight(),8);

    double dXImgPlane = 0.0 ;
    double dYImgPlane = 0.0 ;

    double dXWrdPlane = 0.0 ;
    double dYWrdPlane = 0.0 ;
    double dZWrdPlane = 0.0 ;

    float fXImgPlane , fYImgPlane ;
    BYTE  bPx ;

    //현제 소스에서 그냥 Dst로 변환 하는데...
    //반대로 해야함 Dst를 for문 돌리고 역산하여 Src에서 가져와야함.
    dZWrdPlane = 0.0 ;


    //이거 나중에 검사에 들어가게 되면 쓰레드 때문에 크리티컬 섹션 써야함.==========================
    cp = &CP;
    cd = &CD;
    cc = &CC;

    for(int r = 0 ; r < _pDstImg->GetHeight()-1 ; r++) {
        for(int c = 0 ; c < _pDstImg->GetWidth()-1 ; c++) {
            dXWrdPlane = Rslt.dWorldSttX + (c + Rslt.dImgToWldCntOfsX) * Rslt.dPxResolLenz ;
            dYWrdPlane = Rslt.dWorldSttY + (r + Rslt.dImgToWldCntOfsY) * Rslt.dPxResolLenz ;
            world_coord_to_image_coord(dXWrdPlane, dYWrdPlane, dZWrdPlane , &dXImgPlane, &dYImgPlane);

            fXImgPlane = dXImgPlane ;
            fYImgPlane = dYImgPlane ;

            if(_pSrcImg -> CheckRangeOver( fXImgPlane,fYImgPlane )) {
                continue ;
            }

            bPx = _pSrcImg->GetSubPixel(fXImgPlane , fYImgPlane) ;
            TempImg -> SetPixel(c,r,bPx);
        }
    }


    *_pDstImg = *TempImg ; //  =  ;


    delete TempImg ;

    return true ;
}

void CCalibration::GetCalPos(double _dImgX , double _dImgY , double * _dWrdX , double * _dWrdY)
{
    if(MPara.bUseSimpleCal) {
        *_dWrdX = _dImgX ; //* Rslt.dPxResolLenz ;
        *_dWrdY = _dImgY ; //* Rslt.dPxResolLenz ;
        return ;
    }
    //이거 쓰레드 때문에 크리티컬 섹션 써야함. 검사중에 서로 호출할 가능성이 있음===================
    //이곳 말고는 검사중에 호출되는 부분 없어서 다른곳은 크리티컬 섹션 무시.
EnterCriticalSection(&GM.csTsai);
    cp = &CP;
    cd = &CD;
    cc = &CC;
    image_coord_to_world_coord(_dImgX, _dImgY, 0.0, _dWrdX, _dWrdY);
LeaveCriticalSection(&GM.csTsai);
    //===================================================================
    *_dWrdX -= Rslt.dWorldSttX;
    *_dWrdY -= Rslt.dWorldSttY;

    if(Rslt.dPxResolLenz != 0) {
        *_dWrdX /= Rslt.dPxResolLenz ;
        *_dWrdY /= Rslt.dPxResolLenz ;
    }
    else {
        *_dWrdX = _dImgX ;
        *_dWrdY = _dImgY ;
        static bool bFirst = true ;
        if(bFirst) {
            MessageBox(0,"dPxResolLenz is 0","Error",0);
            bFirst = false ;
            return ;
        }
    }

    *_dWrdX -= Rslt.dImgToWldCntOfsX ;
    *_dWrdY -= Rslt.dImgToWldCntOfsY ;
}





String CCalibration::GetErrMsg()
{
    return m_sErrMsg ;
}


        //화면 출력.
void CCalibration::PaintRslt(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    if(!Rslt.bInspEnd && !Rslt.bBlobEnd) return ;
    if(m_bSkip) return ;

    CGdi *Gdi = new CGdi(_hDc,_dScaleX , _dScaleY , _dOffsetX , _dOffsetY);

    Gdi -> m_tText  .Color = clLime  ;
    



    TPoint point ;
    AnsiString sTemp ;

    CBlob::TBlob * pBlob1 ;
    CBlob::TBlob * pBlob2 ;

    TDPoint dPnt1 ;
    TDPoint dPnt2 ;

    //바둑판.파란색.

            //원영상.
    if(Rslt.bBlobEnd) {
        for(int i= 0 ; i < Rslt.BlobRslt.plBlob.GetDataCnt() ; i++) {
            Gdi -> m_tPen.Color  = clBlue;
            Gdi -> m_tText.Color = clBlue;
            pBlob1 = Rslt.BlobRslt.plBlob.GetData(i);
            Gdi -> LinePlus(pBlob1->dCentroidX,pBlob1->dCentroidY,10) ;
            sTemp.sprintf("%03d",i);
            Gdi -> Text( pBlob1->dCentroidX + 3 ,pBlob1->dCentroidY + 3 , sTemp.c_str() ) ;
        }
        sTemp.sprintf("%.3f",Rslt.dCalPadAngle);
        Gdi -> Text( pBlob1->dCentroidX + 3 ,pBlob1->dCentroidY + 18 , sTemp.c_str() ) ;
    }





    if(Rslt.bInspEnd) {
        for(int r = 0 ; r < Rslt.iRowCnt ; r++) { //가로줄.
            //캘영상.
            Gdi -> m_tPen.Style = psDot ;
            Gdi -> m_tPen.Color = clBlue;
            pBlob1 = Rslt.BlobRslt.plBlob.GetData(r * Rslt.iColCnt);
            pBlob2 = Rslt.BlobRslt.plBlob.GetData((r+1) * Rslt.iColCnt - 1 );
            Gdi -> Line(pBlob1->dCentroidX , pBlob1->dCentroidY , pBlob2->dCentroidX , pBlob2->dCentroidY );

            Gdi -> m_tPen.Style = psSolid ;
            Gdi -> m_tPen.Color = clLime;
            dPnt1 = Rslt.apCalPnt[r * Rslt.iColCnt] ;
            dPnt2 = Rslt.apCalPnt[(r+1) * Rslt.iColCnt - 1 ];
            Gdi -> Line(dPnt1.x , dPnt1.y , dPnt2.x , dPnt2.y);
        }
    }                                 //iColCnt * r + c

    if(Rslt.bInspEnd) {
        for(int c = 0 ; c < Rslt.iColCnt ; c++) { //세로줄.
            Gdi -> m_tPen.Style = psDot ;
            Gdi -> m_tPen.Color = clBlue;
            pBlob1 = Rslt.BlobRslt.plBlob.GetData(c);
            pBlob2 = Rslt.BlobRslt.plBlob.GetData(c + Rslt.iColCnt * (Rslt.iRowCnt - 1));
            Gdi -> Line(pBlob1->dCentroidX , pBlob1->dCentroidY , pBlob2->dCentroidX , pBlob2->dCentroidY );

            //캘영상.
            Gdi -> m_tPen.Style = psSolid ;
            Gdi -> m_tPen.Color = clLime;
            dPnt1 = Rslt.apCalPnt[c] ;
            dPnt2 = Rslt.apCalPnt[c + Rslt.iColCnt * (Rslt.iRowCnt - 1)];
            Gdi -> Line(dPnt1.x , dPnt1.y , dPnt2.x , dPnt2.y);
        }
    }

    if(Rslt.bInspEnd) {
        Gdi -> m_tPen.Style = psSolid ;
        for(int i = 0 ; i < Rslt.BlobRslt.iBlobCnt ; i++) {

            Gdi -> m_tPen.Color  = clLime;
            Gdi -> m_tText.Color = clLime;
            dPnt1 = Rslt.apCalPnt[i] ;
            Gdi -> LinePlus(dPnt1.x , dPnt1.y,10) ;
        }
    }


    delete Gdi;
}




void CCalibration::PaintTracker(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    CGdi *Gdi = new CGdi(_hDc);
    Gdi -> SetScale(_dScaleX , _dScaleY);

    CTracker * Tracker ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Tracker -> Paint(_hDc ,  _dScaleX ,  _dScaleY ,  _dOffsetX ,  _dOffsetY);

    }

    delete Gdi;

}



        //이미지 화면 클릭 이벤트
bool CCalibration::MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    CTracker * Tracker ;
    bool bRet = false ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        if(Tracker -> TrackerDw(Button,Shift ,x, y , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY))bRet = true;
    }
    return bRet;
}

bool CCalibration::MouseMv(TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    CTracker * Tracker ;
    bool bRet = false ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        if(Tracker -> TrackerMv(Shift ,x, y , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY)) bRet = true ;
    }
    return bRet ;
}

void CCalibration::MouseUp(TMouseButton Button,TShiftState Shift )
{
    CTracker * Tracker ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Tracker -> TrackerUp(Button, Shift);
    }
}

        //파라미터 디스플레이
void CCalibration::UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        UpdateListPara(_bToTable , _vePara , &MPara.List);
    }
    else {
        UpdateListPara(_bToTable , _vePara , &MPara.List);
        UpdateMasterPara(true , _vePara );
    }
}

void CCalibration::UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        UpdateListPara(_bToTable , _vePara , &CPara.List);
    }
    else {
        UpdateListPara(_bToTable , _vePara , &CPara.List);

        UpdateCommonPara(true , _vePara );
    }

}
void CCalibration::UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        //_vePara -> DisplayOptions >> doColumnTitles ;

    }
    else {

        UpdateLocalPara(true , _vePara );
    }

}

        //로드 세이브.
void CCalibration::LoadSave(bool _bLoad , String _sVisnName)
{
    String sIniPath ;
    String sCalImagePath ;//= _sFolderPath + "\\Vision\\" + _sVisnName + "_" + m_sName + "_TrainImage.bmp" ;
    if(GM.Optn.bUseDeviceCal) {  //이거 처음 프로그램 켤때 확인 해야 하는데.....
        //sIniPath = _sFolderPath +"\\Vision\\" + _sVisnName + "_" + m_sName + "_" + GetStaticPkgLink()->GetPkgName() + ".ini" ;
        sIniPath      = GM.GetJobFileFolder() + _sVisnName + "_CCalibration.ini" ;
        sCalImagePath = GM.GetJobFileFolder() + _sVisnName + "_CCalibration.bmp" ;
    }
    else {
        sIniPath      = GM.GetGlobalFolder() + _sVisnName + "_CCalibration.ini" ;
        sCalImagePath = GM.GetGlobalFolder() + _sVisnName + "_CCalibration.bmp" ;
    }
    //LoadPara(true , );
    TUserINI Ini ;

    /*
    Tracker -> SetFunc(OnTrackerChanged);
    Tracker -> HeightMax = 100 ;
    Tracker -> WidthMax  = 100 ;
    m_lTrckInsp.PushBack(Tracker);
    m_lLPara   .PushBack(new TLocalPara ());
    */


    int iTrackerCnt ;
    CTracker* Tracker ;
    if(_bLoad) {
        //이파트는 무조건 1개만 되기 때문에 필요 없다.
        //m_lTrckInsp.DeleteAll();
        m_pCalImg   -> LoadBmp(sCalImagePath .c_str());

        if(FileExists(sIniPath)){
            //Tracker ;
            Tracker = m_lTrckInsp.GetData(0);
            Tracker -> LoadSave(_bLoad , sIniPath , "Tracker0");

            LoadSaveList(_bLoad   , sIniPath , "MasterPara  " , &MPara.List       );
            LoadSaveList(_bLoad   , sIniPath , "CommonPara  " , &CPara.List       );

            Ini.Load    (sIniPath , "Result" , "dPxResolSimp" , Rslt.dPxResolSimp );
            Ini.Load    (sIniPath , "Result" , "dPxResolLenz" , Rslt.dPxResolLenz );
            Ini.Load    (sIniPath , "Result" , "dMmResolLenz" , Rslt.dMmResolLenz );
             Ini.Load    (sIniPath , "Result" , "dWorldSttX  " , Rslt.dWorldSttX   );
            Ini.Load    (sIniPath , "Result" , "dWorldSttY  " , Rslt.dWorldSttY   );
            Ini.Load    (sIniPath , "Result" , "dWorldEndX  " , Rslt.dWorldEndX   );
            Ini.Load    (sIniPath , "Result" , "dWorldEndY  " , Rslt.dWorldEndY   );

            Ini.Load    (sIniPath , "Result" , "dImgToWldCntOfsX" , Rslt.dImgToWldCntOfsX);
            Ini.Load    (sIniPath , "Result" , "dImgToWldCntOfsY" , Rslt.dImgToWldCntOfsY);


            //나중에 캘포인트를 저장하고 그걸로 다시 캘하는 방식으로 변경....
            Ini.Load    (sIniPath , "TsaiCC" , "f         " , CC.f          ); // [mm]
            Ini.Load    (sIniPath , "TsaiCC" , "kappa1    " , CC.kappa1     ); // [1/mm^2]
            Ini.Load    (sIniPath , "TsaiCC" , "p1        " , CC.p1         ); // [1/mm]
            Ini.Load    (sIniPath , "TsaiCC" , "p2        " , CC.p2         ); // [1/mm]
            Ini.Load    (sIniPath , "TsaiCC" , "Tx        " , CC.Tx         ); // [mm]
            Ini.Load    (sIniPath , "TsaiCC" , "Ty        " , CC.Ty         ); // [mm]
            Ini.Load    (sIniPath , "TsaiCC" , "Tz        " , CC.Tz         ); // [mm]
            Ini.Load    (sIniPath , "TsaiCC" , "Rx        " , CC.Rx         ); // [rad]
            Ini.Load    (sIniPath , "TsaiCC" , "Ry        " , CC.Ry         ); // [rad]
            Ini.Load    (sIniPath , "TsaiCC" , "Rz        " , CC.Rz         ); // [rad]
            Ini.Load    (sIniPath , "TsaiCC" , "r1        " , CC.r1         ); // []
            Ini.Load    (sIniPath , "TsaiCC" , "r2        " , CC.r2         ); // []
            Ini.Load    (sIniPath , "TsaiCC" , "r3        " , CC.r3         ); // []
            Ini.Load    (sIniPath , "TsaiCC" , "r4        " , CC.r4         ); // []
            Ini.Load    (sIniPath , "TsaiCC" , "r5        " , CC.r5         ); // []
            Ini.Load    (sIniPath , "TsaiCC" , "r6        " , CC.r6         ); // []
            Ini.Load    (sIniPath , "TsaiCC" , "r7        " , CC.r7         ); // []
            Ini.Load    (sIniPath , "TsaiCC" , "r8        " , CC.r8         ); // []
            Ini.Load    (sIniPath , "TsaiCC" , "r9        " , CC.r9         ); // []

            Ini.Load    (sIniPath , "TsaiCP" , "Ncx       " , CP.Ncx        ); // [sel]     Number of sensor elements in camera's x direction
            Ini.Load    (sIniPath , "TsaiCP" , "Nfx       " , CP.Nfx        ); // [pix]     Number of pixels in frame grabber's x direction
            Ini.Load    (sIniPath , "TsaiCP" , "dx        " , CP.dx         ); // [mm/sel]  X dimension of camera's sensor element (in mm)
            Ini.Load    (sIniPath , "TsaiCP" , "dy        " , CP.dy         ); // [mm/sel]  Y dimension of camera's sensor element (in mm)
            Ini.Load    (sIniPath , "TsaiCP" , "dpx       " , CP.dpx        ); // [mm/pix]  Effective X dimension of pixel in frame grabber
            Ini.Load    (sIniPath , "TsaiCP" , "dpy       " , CP.dpy        ); // [mm/pix]  Effective Y dimension of pixel in frame grabber
            Ini.Load    (sIniPath , "TsaiCP" , "Cx        " , CP.Cx         ); // [pix]     Z axis intercept of camera coordinate system
            Ini.Load    (sIniPath , "TsaiCP" , "Cy        " , CP.Cy         ); // [pix]     Z axis intercept of camera coordinate system
            Ini.Load    (sIniPath , "TsaiCP" , "sx        " , CP.sx         ); // []        Scale factor to compensate for any error in dpx
        }
        else {


            MPara.SetDefault();
            CPara.SetDefault();
        }

    }
    else {
        m_pCalImg   -> SaveBmp(sCalImagePath .c_str());

        //if(m_sCrntJobFilePath != "")Ini.ClearFile(sIniPath       ) ;

        //Tracker ;
        Tracker = m_lTrckInsp.GetData(0);
        Tracker -> LoadSave(_bLoad , sIniPath , "Tracker0");

        LoadSaveList(_bLoad   , sIniPath , "MasterPara" , &MPara.List   );
        LoadSaveList(_bLoad   , sIniPath , "CommonPara" , &CPara.List   );

        Ini.Save    (sIniPath , "Result" , "dPxResolSimp" , Rslt.dPxResolSimp );
        Ini.Save    (sIniPath , "Result" , "dPxResolLenz" , Rslt.dPxResolLenz );
        Ini.Save    (sIniPath , "Result" , "dMmResolLenz" , Rslt.dMmResolLenz );
        Ini.Save    (sIniPath , "Result" , "dWorldSttX  " , Rslt.dWorldSttX   );
        Ini.Save    (sIniPath , "Result" , "dWorldSttY  " , Rslt.dWorldSttY   );
        Ini.Save    (sIniPath , "Result" , "dWorldEndX  " , Rslt.dWorldEndX   );
        Ini.Save    (sIniPath , "Result" , "dWorldEndY  " , Rslt.dWorldEndY   );

        Ini.Save    (sIniPath , "Result" , "dImgToWldCntOfsX" , Rslt.dImgToWldCntOfsX);
        Ini.Save    (sIniPath , "Result" , "dImgToWldCntOfsY" , Rslt.dImgToWldCntOfsY);

        Ini.Save    (sIniPath , "TsaiCC" , "f         " , CC.f          ); // [mm]
        Ini.Save    (sIniPath , "TsaiCC" , "kappa1    " , CC.kappa1     ); // [1/mm^2]
        Ini.Save    (sIniPath , "TsaiCC" , "p1        " , CC.p1         ); // [1/mm]
        Ini.Save    (sIniPath , "TsaiCC" , "p2        " , CC.p2         ); // [1/mm]
        Ini.Save    (sIniPath , "TsaiCC" , "Tx        " , CC.Tx         ); // [mm]
        Ini.Save    (sIniPath , "TsaiCC" , "Ty        " , CC.Ty         ); // [mm]
        Ini.Save    (sIniPath , "TsaiCC" , "Tz        " , CC.Tz         ); // [mm]
        Ini.Save    (sIniPath , "TsaiCC" , "Rx        " , CC.Rx         ); // [rad]
        Ini.Save    (sIniPath , "TsaiCC" , "Ry        " , CC.Ry         ); // [rad]
        Ini.Save    (sIniPath , "TsaiCC" , "Rz        " , CC.Rz         ); // [rad]
        Ini.Save    (sIniPath , "TsaiCC" , "r1        " , CC.r1         ); // []
        Ini.Save    (sIniPath , "TsaiCC" , "r2        " , CC.r2         ); // []
        Ini.Save    (sIniPath , "TsaiCC" , "r3        " , CC.r3         ); // []
        Ini.Save    (sIniPath , "TsaiCC" , "r4        " , CC.r4         ); // []
        Ini.Save    (sIniPath , "TsaiCC" , "r5        " , CC.r5         ); // []
        Ini.Save    (sIniPath , "TsaiCC" , "r6        " , CC.r6         ); // []
        Ini.Save    (sIniPath , "TsaiCC" , "r7        " , CC.r7         ); // []
        Ini.Save    (sIniPath , "TsaiCC" , "r8        " , CC.r8         ); // []
        Ini.Save    (sIniPath , "TsaiCC" , "r9        " , CC.r9         ); // []

        Ini.Save    (sIniPath , "TsaiCP" , "Ncx       " , CP.Ncx        ); // [sel]     Number of sensor elements in camera's x direction
        Ini.Save    (sIniPath , "TsaiCP" , "Nfx       " , CP.Nfx        ); // [pix]     Number of pixels in frame grabber's x direction
        Ini.Save    (sIniPath , "TsaiCP" , "dx        " , CP.dx         ); // [mm/sel]  X dimension of camera's sensor element (in mm)
        Ini.Save    (sIniPath , "TsaiCP" , "dy        " , CP.dy         ); // [mm/sel]  Y dimension of camera's sensor element (in mm)
        Ini.Save    (sIniPath , "TsaiCP" , "dpx       " , CP.dpx        ); // [mm/pix]  Effective X dimension of pixel in frame grabber
        Ini.Save    (sIniPath , "TsaiCP" , "dpy       " , CP.dpy        ); // [mm/pix]  Effective Y dimension of pixel in frame grabber
        Ini.Save    (sIniPath , "TsaiCP" , "Cx        " , CP.Cx         ); // [pix]     Z axis intercept of camera coordinate system
        Ini.Save    (sIniPath , "TsaiCP" , "Cy        " , CP.Cy         ); // [pix]     Z axis intercept of camera coordinate system
        Ini.Save    (sIniPath , "TsaiCP" , "sx        " , CP.sx         ); // []        Scale factor to compensate for any error in dpx
    }

}


