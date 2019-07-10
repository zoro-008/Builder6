//---------------------------------------------------------------------------
#pragma hdrstop
#include "PkgMatch.h"
#include "BaseMaths.h"

#include "UserINI.h"
//#include "Global.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


CMatch_V01::CMatch_V01()
{
                           
}

CMatch_V01::~CMatch_V01()
{
}

void CMatch_V01::OnTrackerChanged()
{
     //화면에서 다시 그리므로 여기서 메세지 날릴 필요 없다.
}

int CMatch_V01::GetAutoThreshold(CImage * _pImg)
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


    return iAvrGray ;
    /*
    for(i = 0 ; i <= iAvrGray ; i++){
        dGrayPrcnt1 = iGrayCnt[i]           /(double)iAllPxCnt ;
        dGrayPrcnt2 = iGrayCnt[iAvrGray+1+i]/(double)iAllPxCnt ; //이부분에서 인덱스 오버런된다.... 살펴 보기..
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
    */
}




//순수 가상 함수.
//==============================================================================
bool CMatch_V01::Init()
{
    m_pTrainArea = new CArea() ;
    m_pTrainImg  = new CImage() ;


    //이 블럭 은 나중에 트렉커 많아지면 같이 증가해야 함..
    TOutBandRectTracker * Tracker = new TOutBandRectTracker();
    Tracker -> SetFunc(OnTrackerChanged);
    Tracker -> HeightMax = 200 ;
    Tracker -> WidthMax  = 200 ;
    Tracker -> BandMax   = 100  ;
    m_lTrckInsp.PushBack(Tracker);
    m_lLPara   .PushBack(new TLocalPara ());

    m_sCrntJobFilePath = "" ;

}
bool CMatch_V01::Close()
{
    delete m_pTrainArea ; m_pTrainArea = NULL ;
    delete m_pTrainImg  ; m_pTrainImg  = NULL ;

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



}


        //검사 및 러닝 관련.
bool CMatch_V01::GetTrainAvailable()
{
    return true ;
}
bool CMatch_V01::Train(CImage * _pImg)
{
    m_sErrMsg = "";
    int    iTrThreshold  = MPara.iTrThreshold .GetInt() ;
    int    iTrInspOft    = MPara.iTrInspOft   .GetInt() ;
    int    iTrNoInspOft  = MPara.iTrNoInspOft .GetInt() ;
    int    iTrInspFreq   = MPara.iTrInspFreq  .GetInt() ;
    String aRefOfsXAdd   = MPara.aRefOfsXAdd  .GetAddress() ;
    String aRefOfsYAdd   = MPara.aRefOfsYAdd  .GetAddress() ;
    String aOutOfsXAdd   = MPara.aOutOfsXAdd  .GetAddress() ;
    String aOutOfsYAdd   = MPara.aOutOfsYAdd  .GetAddress() ;

    bool   bSkip        = CPara.bSkip       .GetBool  () ;
    bool   bFindNg      = CPara.bFindNg     .GetBool  () ;
    double dSinc        = CPara.dSinc       .GetDouble() ;



    //if(bSkip   ) return true ;

    if(iTrInspOft <= iTrNoInspOft) {
        m_sErrMsg = "ERROR : iTrInspOft <= iTrNoInspOft";
        return false ;
    }


    Rslt.Clear();

    TRect Rect ;
    TOutBandRectTracker * Tracker = m_lTrckInsp.GetCrntData(true) ;
    Rect.left   = Tracker -> Left   ;
    Rect.top    = Tracker -> Top    ;
    Rect.right  = Tracker -> GetRight()  ;
    Rect.bottom = Tracker -> GetBottom() ;




    _pImg -> SetRect(&Rect); //검사 네모가 이미지 밖으로 나갔을때 이미지를 벋어나지 않게 사이즈 줄여 준다.

    int iLeft   = Rect.Left    ;
    int iTop    = Rect.Top     ;
    int iRight  = Rect.Right   ;
    int iWidth  = Rect.Width ();
    int iHeight = Rect.Height();


    m_pTrainArea  -> SetSize(iWidth,iHeight) ; //검사 좌표

    _pImg -> Trim(Rect , m_pTrainImg); //렉트 사이즈 만큼 트레인 이미지에 잘라서 넣는다.

    bool bEdge  = false ;
    bool bPrePx = _pImg -> GetPixel(iLeft,iTop) ; //픽셀이 쓰레숄드 값보다 높으면 1 낮으면 0
    bool bPx    = _pImg -> GetPixel(iLeft,iTop) ;


    if(iTrThreshold==0)iTrThreshold = GetAutoThreshold(m_pTrainImg); //쓰레숄드 세팅이 0이면 오토쓰레숄드를 뽑아 온다.

    TCircle Circle ;

    //Set Temp Insp Area.
    Circle.rad = iTrInspOft ; //검사 영역을 마스킹 하기 위해
    for(int y = 0 ; y < iHeight ; y++){
        bPrePx = _pImg -> GetPixel(iLeft,iTop+y) > iTrThreshold;
        for(int x = 0 ; x < iWidth ; x++){
            bPx = _pImg -> GetPixel(iLeft+x,iTop+y) > iTrThreshold ;
            bEdge = bPx != bPrePx ;
            if(bEdge) {
                Circle.cx = x ;
                Circle.cy = y ;
                m_pTrainArea -> AddCircle(Circle , otTemp);
            }
            bPrePx = bPx ;
        }
    }
    for(int x = 0 ; x < iWidth ; x++){
        bPrePx = _pImg -> GetPixel(iLeft+x,iTop) > iTrThreshold ;
        for(int y = 0 ; y < iHeight ; y++){
            bPx = _pImg -> GetPixel(iLeft+x,iTop+y) > iTrThreshold ;
            bEdge = bPx != bPrePx ;
            if(bEdge) {
                Circle.cx = x ;
                Circle.cy = y ;
                m_pTrainArea -> AddCircle(Circle , otTemp);
            }
            bPrePx = bPx ;
        }
    }

    //Set Temp Insp Area.
    Circle.rad = iTrNoInspOft ;
    for(int y = 0 ; y < iHeight ; y++){
        bPx = _pImg -> GetPixel(iLeft,iTop+y) > iTrThreshold ;
        for(int x = 0 ; x < iWidth ; x++){
            bPx = _pImg -> GetPixel(iLeft+x,iTop+y) > iTrThreshold ;
            bEdge = bPx != bPrePx ;
            if(bEdge) {
                Circle.cx = x ;
                Circle.cy = y ;
                m_pTrainArea -> AddCircle(Circle , otUnknown);
            }
            bPrePx = bPx ;
        }
    }
    for(int x = 0 ; x < iWidth ; x++){
        bPx = _pImg -> GetPixel(iLeft+x,iTop) > iTrThreshold ;
        for(int y = 0 ; y < iHeight ; y++){
            bPx = _pImg -> GetPixel(iLeft+x,iTop+y) > iTrThreshold ;
            bEdge = bPx != bPrePx ;
            if(bEdge) {
                Circle.cx = x ;
                Circle.cy = y ;
                m_pTrainArea -> AddCircle(Circle , otUnknown);
            }
            bPrePx = bPx ;
        }
    }

    //Separate Light,Dark Area.
    for(int y = 0 ; y < iHeight ; y++){
        for(int x = 0 ; x < iWidth ; x++){
            if(m_pTrainArea -> GetPixel(x,y) != otTemp) continue ;
            if(_pImg -> GetPixel(iLeft+x,iTop+y) > iTrThreshold ){m_pTrainArea -> SetPixel(x,y,otLtInsp ) ; Rslt.iLtPxCnt++;}
            else                                                 {m_pTrainArea -> SetPixel(x,y,otDkInsp ) ; Rslt.iDkPxCnt++;}
        }
    }

    Orig.dCntPosX = Rect.left + Rect.Width ()/2.0 ;
    Orig.dCntPosY = Rect.top  + Rect.Height()/2.0 ;

    //트레인 끝나고 디스플레이 하기 위해.. .세팅.
    Rslt.bInspEnd = true ;
    Rslt.bRsltOk  = "OK" ;
    Rslt.dCntPosX = Orig.dCntPosX ;
    Rslt.dCntPosY = Orig.dCntPosY ;
    Rslt.dSinc    = 100  ;
    Rslt.tRect    = Rect ;

    CValue * Val ;
    Val = m_pValueList -> GetValueByName(MPara.aOutOfsXAdd.GetAddress());
    if(Val)Val -> SetDouble(Rslt.dCntPosX-Orig.dCntPosX);
    Val = m_pValueList -> GetValueByName(MPara.aOutOfsYAdd.GetAddress());
    if(Val)Val -> SetDouble(Rslt.dCntPosY-Orig.dCntPosY);


                     

    return true ;
}

void CMatch_V01::RsltClear() //검사 결과값을 검사전에 클리어 한번 하고 한다.
{
    Rslt.Clear();
}

bool CMatch_V01::GetRslt()
{
    return Rslt.bRsltOk ;
}



bool CMatch_V01::Run(CImage * _pImg)
{
    m_sErrMsg = "";
    Rslt.Clear();  //결과값 클리어.

    int    iTrThreshold  = MPara.iTrThreshold .GetInt() ;
    int    iTrInspOft    = MPara.iTrInspOft   .GetInt() ;
    int    iTrNoInspOft  = MPara.iTrNoInspOft .GetInt() ;
    int    iTrInspFreq   = MPara.iTrInspFreq  .GetInt() ;
    String aRefOfsXAdd   = MPara.aRefOfsXAdd  .GetAddress() ;
    String aRefOfsYAdd   = MPara.aRefOfsYAdd  .GetAddress() ;
    String aOutOfsXAdd   = MPara.aOutOfsXAdd  .GetAddress() ;
    String aOutOfsYAdd   = MPara.aOutOfsYAdd  .GetAddress() ;

    bool   bSkip        = CPara.bSkip       .GetBool  () ;
    bool   bFindNg      = CPara.bFindNg     .GetBool  () ;
    double dSinc        = CPara.dSinc       .GetDouble() ;

    CValue * Val ;
    if(bSkip   ) {
        //찾았을때 NG 옵션.
        Rslt.bRsltOk = true ;

        Val = m_pValueList -> GetValueByName(MPara.aOutOfsXAdd.GetAddress());
        if(Val)Val -> SetDouble(0.0);
        Val = m_pValueList -> GetValueByName(MPara.aOutOfsYAdd.GetAddress());
        if(Val)Val -> SetDouble(0.0);
        Rslt.bInspEnd = true ;

        return true ;

    }

    //벨류 테이블에서 기준값 오프셑 가져옴.
    double dInspOfsX =0, dInspOfsY=0;

    Val = m_pValueList -> GetValueByName(MPara.aRefOfsXAdd.GetAddress());
    if(Val)dInspOfsX = Val -> GetDouble();
    Val = m_pValueList -> GetValueByName(MPara.aRefOfsYAdd.GetAddress());
    if(Val)dInspOfsY = Val -> GetDouble();


    TRect tInspRect ;
    TOutBandRectTracker * Tracker = m_lTrckInsp.GetCrntData(true) ;
    tInspRect.left   = Tracker -> GetOutRect().left   + dInspOfsX;
    tInspRect.top    = Tracker -> GetOutRect().top    + dInspOfsY;
    tInspRect.right  = Tracker -> GetOutRect().right  + dInspOfsX;
    tInspRect.bottom = Tracker -> GetOutRect().bottom + dInspOfsY;

    _pImg -> SetRect(&tInspRect) ;





    int iLeft   = tInspRect.Left    ;
    int iTop    = tInspRect.Top     ;
    int iRight  = tInspRect.Right   ;
    int iWidth  = tInspRect.Width ();
    int iHeight = tInspRect.Height();


    //CValue * vdOriginOffsetX = Global.GetCmmVal()
    //CValue * vdOriginOffsetY = Global.GetCmmVal()





    if(iHeight < m_pTrainArea -> GetHeight() ) {
        m_sErrMsg = "ERROR : iHeight <= m_pTrainArea -> GetHeight()";
        Rslt.bInspEnd = true ;
        return false ;
    }

    if(iWidth < m_pTrainArea -> GetWidth() ) {
        m_sErrMsg = "ERROR : iWidth <= m_pTrainArea -> GetWidth()";
        Rslt.bInspEnd = true ;
        return false ;
    }

    CLinkedList<TPxPoint> llInspPnt  ;  //속도향상을 위해 검사 할 위치를 리스트에 넣어서 검사한다.

    int iXRange = iWidth  - m_pTrainArea -> GetWidth () ;
    int iYRange = iHeight - m_pTrainArea -> GetHeight() ;

    int iMinErrCnt = MAX_INT_VALUE ;
    int iMinErrX   = 0 ;
    int iMinErrY   = 0 ;
    int iErrCntLmt = 0 ;
    int iInspPxSum = 0 ;
    int iInspPxAvr = 0 ;
    int iInspPxCnt = 0 ;
    unsigned char cPx = 0 ;

    //미리 검사할 포인트를 링크드 리스트에 넣는다.
    //나중에 
    //const int iInspFreq = 10 ; //검사 빈도수.
    if(iTrInspFreq < 1)  iTrInspFreq = 1 ;
    for(int y = 0 ; y < m_pTrainArea -> GetHeight() ; y++) {
        for(int x = 0 ; x < m_pTrainArea -> GetWidth() ; x++) {
            if(x%iTrInspFreq == 0 || y%iTrInspFreq == 0) {
                if(m_pTrainArea -> GetPixel(x , y) == otDkInsp || m_pTrainArea -> GetPixel(x , y) == otLtInsp ){
                    cPx =  m_pTrainImg -> GetPixel(x , y) ;
                    llInspPnt.PushBack(TPxPoint(x,y,cPx));
                    iInspPxSum+= cPx;
                }
            }

        }
    }

    iInspPxCnt = llInspPnt.GetDataCnt() ;

    if(iInspPxCnt == 0 ) {
        m_sErrMsg = "ERROR : No Insp Point";
        Rslt.bInspEnd = true ;
        return false ;
    }
    iInspPxAvr = iInspPxSum / (float)iInspPxCnt ;

    //iErrCntLmt = iInspPxCnt - (iInspPxCnt * fSinc / 100) ;

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

                Rslt.iDkPxCnt     = iDkErrCnt ;
                Rslt.iLtPxCnt     = iLtErrCnt ;
                Rslt.dCntPosX     = iLeft + rx + m_pTrainArea -> GetWidth () / 2 ;
                Rslt.dCntPosY     = iTop  + ry + m_pTrainArea -> GetHeight() / 2 ;                         //화이트 나 블랙 바탕에서 무조건 50프로 이상 나오는 것 때문에 줄임. 50프로 미만은 고만 고만 해서 무시함.
                Rslt.dSinc        = ((iInspPxCnt*256 - iMinErrCnt) / (float)(iInspPxCnt*256)) * 100.0 ;
                Rslt.tRect.left   = iLeft + rx ;
                Rslt.tRect.top    = iTop  + ry ;
                Rslt.tRect.right  = iLeft + rx + m_pTrainArea -> GetWidth () ;
                Rslt.tRect.bottom = iTop  + ry + m_pTrainArea -> GetHeight() ;
            }
        }

        
        

    }

    //찾았을때 NG 옵션.
    Rslt.bRsltOk = bFindNg ? Rslt.dSinc < dSinc : Rslt.dSinc >= dSinc ;

    Val = m_pValueList -> GetValueByName(MPara.aOutOfsXAdd.GetAddress());
    if(Val)Val -> SetDouble(Rslt.dCntPosX-Orig.dCntPosX);
    Val = m_pValueList -> GetValueByName(MPara.aOutOfsYAdd.GetAddress());
    if(Val)Val -> SetDouble(Rslt.dCntPosY-Orig.dCntPosY);
    Rslt.bInspEnd = true ;

    delete [] tInspPxPnt ;

    return true ;

}

String CMatch_V01::GetErrMsg()
{
    return m_sErrMsg ;
}


        //화면 출력.
void CMatch_V01::PaintRslt(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    if(!Rslt.bInspEnd) return ;
    if(CPara.bSkip.GetBool()) return ;

    //나중에 Gdi에도 오프셑 적용 하게 해서... 스케일 오프셑 관련 다 걷어내자.......
    CGdi *Gdi = new CGdi(_hDc);
    //Gdi -> SetScale(_dScaleX , _dScaleY);

    String sTemp   ;
    String sTemp11 ;
    String sTemp22 ;
    String sTemp33 ;
    int    iCtrX  ;
    int    iCtrY  ;

    Gdi -> m_tText.Color = Rslt.bRsltOk ? clLime : clRed ;

    sTemp11 = Rslt.dSinc    ;
    sTemp22 = Rslt.dCntPosX ;
    sTemp33 = Rslt.dCntPosY ;

    String sTemp1 = sTemp11.sprintf("%.2f",Rslt.dSinc   );
    String sTemp2 = sTemp22.sprintf("%.2f",Rslt.dCntPosX);
    String sTemp3 = sTemp33.sprintf("%.2f",Rslt.dCntPosY);

    sTemp = String("X : ") + sTemp2 + String(" Y : ") + sTemp3 + String(" Sinc : ") + sTemp1 ;

    //Scale...
    //TDRect Rect  ;
    TRect Rect ;
    Rect.left    = (Rslt.tRect.left - _dOffsetX) * _dScaleX;
    Rect.top     = (Rslt.tRect.top  - _dOffsetY) * _dScaleY;
    Rect.right   = (Rslt.tRect.left - _dOffsetX+ Rslt.tRect.Width() ) * _dScaleX  ;
    Rect.bottom  = (Rslt.tRect.top  - _dOffsetY+ Rslt.tRect.Height()) * _dScaleY  ;

    double dRsltX = (Rslt.dCntPosX - _dOffsetX)* _dScaleX;
    double dRsltY = (Rslt.dCntPosY - _dOffsetY)* _dScaleY;

    //hMemDc, 0 , 0 , iPanW , iPanH , m_dDispImgX , m_dDispImgY , dw, dh);}
    //(hMemDc, DstX , DstY , m_pTrainImg ->GetScaleX() , m_pTrainImg ->GetScaleY() ) ;
    //int iAreaOfsX =

    //if(m_pTrainArea)m_pTrainArea -> ShowArea(_hDc , Rslt.tRect.left , Rslt.tRect.top , _dScaleX , _dScaleY);

    /*
    for(int y = 0 ; y < 10 ; y++){
        for(int x = 0 ; x< 10 ; x++){
            Rect(true, x, y , int x+1, int y2, int cs)


        }
    }
    */




    if(m_pTrainArea){
        if(CPara.bFindNg.GetBool()){
            if(!Rslt.bRsltOk) {
                m_pTrainArea -> ShowArea(_hDc , Rslt.tRect.left - _dOffsetX , Rslt.tRect.top  - _dOffsetY , _dScaleX , _dScaleY);
                Gdi -> m_tPen.Color = Rslt.bRsltOk ? clLime : clRed ;
                Gdi -> Rect(false , Rect);

            }
        }
        else {
            m_pTrainArea -> ShowArea(_hDc , Rslt.tRect.left - _dOffsetX , Rslt.tRect.top  - _dOffsetY , _dScaleX , _dScaleY);
            Gdi -> m_tPen.Color = Rslt.bRsltOk ? clLime : clRed ;
            Gdi -> Rect(false , Rect);
        }
    }




    Gdi -> Text(Rect.left + 5, Rect.top - 15 ,sTemp.c_str()) ;


    Gdi -> m_tPen.Color = Rslt.bRsltOk ? clLime : clRed ;
    Gdi -> LineCross(dRsltX , dRsltY , 10) ;




    delete Gdi;
    

}




void CMatch_V01::PaintTracker(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    CGdi *Gdi = new CGdi(_hDc);
    Gdi -> SetScale(_dScaleX , _dScaleY);

    TOutBandRectTracker * Tracker ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Tracker -> Paint(_hDc ,  _dScaleX ,  _dScaleY ,  _dOffsetX ,  _dOffsetY);

    }

    delete Gdi;

}



        //이미지 화면 클릭 이벤트
TTracker * CMatch_V01::MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    TOutBandRectTracker * Tracker ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        if(Tracker -> TrackerDw(Button,Shift ,x, y , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY))
            return Tracker ;
    }

    return NULL ;
}


TTracker * CMatch_V01::MouseMv(TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    TOutBandRectTracker * Tracker ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        if(Tracker -> TrackerMv(Shift ,x, y , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY))
            return Tracker ;
    }
    return NULL ;
}

void CMatch_V01::MouseUp(TMouseButton Button,TShiftState Shift )
{
    TOutBandRectTracker * Tracker ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Tracker -> TrackerUp(Button, Shift);
    }
}

        //파라미터 디스플레이
void CMatch_V01::UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        //_vePara -> DisplayOptions >> doColumnTitles ;

        _vePara -> Values[MPara.iTrThreshold.GetName()] = MPara.iTrThreshold.GetInt();
        _vePara -> Values[MPara.iTrInspOft  .GetName()] = MPara.iTrInspOft  .GetInt();
        _vePara -> Values[MPara.iTrNoInspOft.GetName()] = MPara.iTrNoInspOft.GetInt();
        _vePara -> Values[MPara.iTrInspFreq .GetName()] = MPara.iTrInspFreq .GetInt();

        _vePara -> Values[MPara.aRefOfsXAdd .GetName()] = MPara.aRefOfsXAdd .GetAddress();
        _vePara -> Values[MPara.aRefOfsYAdd .GetName()] = MPara.aRefOfsYAdd .GetAddress();
        _vePara -> Values[MPara.aOutOfsXAdd .GetName()] = MPara.aOutOfsXAdd .GetAddress();
        _vePara -> Values[MPara.aOutOfsYAdd .GetName()] = MPara.aOutOfsYAdd .GetAddress();
    }
    else {
        MPara.iTrThreshold.SetIntFromStr(_vePara -> Values[MPara.iTrThreshold.GetName()]) ;
        MPara.iTrInspOft  .SetIntFromStr(_vePara -> Values[MPara.iTrInspOft  .GetName()]) ;
        MPara.iTrNoInspOft.SetIntFromStr(_vePara -> Values[MPara.iTrNoInspOft.GetName()]) ;
        MPara.iTrInspFreq .SetIntFromStr(_vePara -> Values[MPara.iTrInspFreq .GetName()]) ;

        MPara.aRefOfsXAdd .SetAddress( _vePara -> Values[MPara.aRefOfsXAdd .GetName()]);
        MPara.aRefOfsYAdd .SetAddress( _vePara -> Values[MPara.aRefOfsYAdd .GetName()]);
        MPara.aOutOfsXAdd .SetAddress( _vePara -> Values[MPara.aOutOfsXAdd .GetName()]);
        MPara.aOutOfsYAdd .SetAddress( _vePara -> Values[MPara.aOutOfsYAdd .GetName()]);


        UpdateMasterPara(true , _vePara );
    }
}

void CMatch_V01::UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        //_vePara -> DisplayOptions >> doColumnTitles ;
        _vePara -> Values[CPara.bSkip   .GetName()] = (short)CPara.bSkip  .GetBool  ();
        _vePara -> Values[CPara.bFindNg .GetName()] = (short)CPara.bFindNg.GetBool  ();
        _vePara -> Values[CPara.dSinc   .GetName()] =        CPara.dSinc  .GetDouble();
    }
    else {
        CPara.bSkip   .SetBoolFromStr   (_vePara -> Values[CPara.bSkip  .GetName()]) ;
        CPara.bFindNg .SetBoolFromStr   (_vePara -> Values[CPara.bFindNg.GetName()]) ;
        CPara.dSinc   .SetDoubleFromStr (_vePara -> Values[CPara.dSinc  .GetName()]) ;

        UpdateCommonPara(true , _vePara );
    }

}
void CMatch_V01::UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara)
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
void CMatch_V01::LoadSave(bool _bLoad , String _sFolderPath , String _sVisnName)
{
    String sIniPath        = _sFolderPath +"\\Vision\\" + _sVisnName + "_" + m_sName + "_" + GetPkgName() + ".ini" ;
    String sTrainAreaPath  = _sFolderPath +"\\Vision\\" + _sVisnName + "_" + m_sName + "_TrainArea.bmp"  ;
    String sTrainImagePath = _sFolderPath +"\\Vision\\" + _sVisnName + "_" + m_sName + "_TrainImage.bmp" ;

    TUserINI Ini ;

    /*
    Tracker -> SetFunc(OnTrackerChanged);
    Tracker -> HeightMax = 100 ;
    Tracker -> WidthMax  = 100 ;
    m_lTrckInsp.PushBack(Tracker);
    m_lLPara   .PushBack(new TLocalPara ());
    */


    int iTrackerCnt ;
    TOutBandRectTracker* Tracker ;
    if(_bLoad) {
        m_pTrainArea -> LoadBmp(sTrainAreaPath .c_str());
        m_pTrainImg  -> LoadBmp(sTrainImagePath.c_str());

        //이파트는 무조건 1개만 되기 때문에 필요 없다.
        //m_lTrckInsp.DeleteAll();

        //Tracker ;
        Tracker = m_lTrckInsp.GetData(0);
        Tracker -> OnLoadSave(_bLoad , sIniPath , "Tracker0");

        Ini.Load(sIniPath, "MasterPara" , MPara.iTrThreshold.GetName(), MPara.iTrThreshold.GetStrPnt());
        Ini.Load(sIniPath, "MasterPara" , MPara.iTrInspOft  .GetName(), MPara.iTrInspOft  .GetStrPnt());
        Ini.Load(sIniPath, "MasterPara" , MPara.iTrNoInspOft.GetName(), MPara.iTrNoInspOft.GetStrPnt());
        Ini.Load(sIniPath, "MasterPara" , MPara.iTrInspFreq .GetName(), MPara.iTrInspFreq .GetStrPnt());

        Ini.Load(sIniPath, "MasterPara" , MPara.aRefOfsXAdd .GetName(), MPara.aRefOfsXAdd .GetStrPnt());
        Ini.Load(sIniPath, "MasterPara" , MPara.aRefOfsYAdd .GetName(), MPara.aRefOfsYAdd .GetStrPnt());
        Ini.Load(sIniPath, "MasterPara" , MPara.aOutOfsXAdd .GetName(), MPara.aOutOfsXAdd .GetStrPnt());
        Ini.Load(sIniPath, "MasterPara" , MPara.aOutOfsYAdd .GetName(), MPara.aOutOfsYAdd .GetStrPnt());

        Ini.Load(sIniPath, "CommonPara" , CPara.bSkip       .GetName(), CPara.bSkip       .GetStrPnt());
        Ini.Load(sIniPath, "CommonPara" , CPara.bFindNg     .GetName(), CPara.bFindNg     .GetStrPnt());
        Ini.Load(sIniPath, "CommonPara" , CPara.dSinc       .GetName(), CPara.dSinc       .GetStrPnt());

        Ini.Load(sIniPath, "TrainRslt"  , "dCntPosX"                  , Orig .dCntPosX                );
        Ini.Load(sIniPath, "TrainRslt"  , "dCntPosY"                  , Orig .dCntPosY                );

    }
    else {
        iTrackerCnt = m_lTrckInsp.GetDataCnt() ;

        if(m_sCrntJobFilePath != "")Ini.ClearFile(sIniPath       ) ;
        if(m_sCrntJobFilePath != "")Ini.ClearFile(sTrainAreaPath ) ;
        if(m_sCrntJobFilePath != "")Ini.ClearFile(sTrainImagePath) ;

        m_pTrainArea -> SaveBmp(sTrainAreaPath .c_str());
        m_pTrainImg  -> SaveBmp(sTrainImagePath.c_str());

        //Tracker ;
        Tracker = m_lTrckInsp.GetData(0);
        Tracker -> OnLoadSave(_bLoad , sIniPath , "Tracker0");

        Ini.Save(sIniPath, "MasterPara" , MPara.iTrThreshold.GetName(), MPara.iTrThreshold.GetStrPnt());
        Ini.Save(sIniPath, "MasterPara" , MPara.iTrInspOft  .GetName(), MPara.iTrInspOft  .GetStrPnt());
        Ini.Save(sIniPath, "MasterPara" , MPara.iTrNoInspOft.GetName(), MPara.iTrNoInspOft.GetStrPnt());
        Ini.Save(sIniPath, "MasterPara" , MPara.iTrInspFreq .GetName(), MPara.iTrInspFreq .GetStrPnt());

        Ini.Save(sIniPath, "MasterPara" , MPara.aRefOfsXAdd .GetName(), MPara.aRefOfsXAdd .GetStrPnt());
        Ini.Save(sIniPath, "MasterPara" , MPara.aRefOfsYAdd .GetName(), MPara.aRefOfsYAdd .GetStrPnt());
        Ini.Save(sIniPath, "MasterPara" , MPara.aOutOfsXAdd .GetName(), MPara.aOutOfsXAdd .GetStrPnt());
        Ini.Save(sIniPath, "MasterPara" , MPara.aOutOfsYAdd .GetName(), MPara.aOutOfsYAdd .GetStrPnt());

        Ini.Save(sIniPath, "CommonPara" , CPara.bSkip       .GetName(), CPara.bSkip       .GetStrPnt());
        Ini.Save(sIniPath, "CommonPara" , CPara.bFindNg     .GetName(), CPara.bFindNg     .GetStrPnt());
        Ini.Save(sIniPath, "CommonPara" , CPara.dSinc       .GetName(), CPara.dSinc       .GetStrPnt());

        Ini.Save(sIniPath, "TrainRslt"  , "dCntPosX"                  , Orig .dCntPosX                );
        Ini.Save(sIniPath, "TrainRslt"  , "dCntPosY"                  , Orig .dCntPosY                );
    }

    m_sCrntJobFilePath = _sFolderPath ;



}




