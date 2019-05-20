//---------------------------------------------------------------------------
#pragma hdrstop
#include "PkgMatch.h"
#include "BaseMaths.h"

#include "UserINI.h"
#include "aAutoThreshold.h"
#include "GlobalMan.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

REGIST_STATICLLINK_CPP(CMatch_V01)
CMatch_V01::CMatch_V01()
{
                           
}

CMatch_V01::~CMatch_V01()
{
}

void CMatch_V01::OnTrackerChanged()
{
     //ȭ�鿡�� �ٽ� �׸��Ƿ� ���⼭ �޼��� ���� �ʿ� ����.
}

//int CMatch_V01::GetAutoThreshold(CImage * _pImg)
//{
//    int iWidth   = _pImg -> GetWidth() ;
//    int iHeight  = _pImg -> GetHeight();
//
//    int iPixel = 0 , iCount = 0 , iTemp = 0;
//
//    int iAvrGray = 0 ;
//    int iSumGray = 0 ;
//
//    int iAllPxCnt = iWidth * iHeight ;
//
//    if(iAllPxCnt <= 0 ) return 0 ;
//
//    //double * dGrayCnt = new double[];
//    int iGrayCnt[256];
//
//    memset(iGrayCnt , 0 , sizeof(int) * 256);
//
//    for(int x = 0 ; x < iWidth ; x++){
//        for(int y = 0 ; y < iHeight ; y++){
//            iTemp = _pImg -> GetPixel(x,y);
//            iGrayCnt[iTemp]++ ;
//            iSumGray += iTemp ;
//        }
//    }
//
//    iAvrGray = iSumGray / iAllPxCnt ;
//
//    int i = 0 , iAutoThresh = 0;
//    double dBig = 0 , dResult = 0 ;
//    double dGrayPrcnt1 = 0 ;
//    double dGrayPrcnt2 = 0 ;
//    double dGrayW1 = 0 , dGrayU1 = 0 , dGrayA1 = 0 ,
//           dGrayW2 = 0 , dGrayU2 = 0 , dGrayA2 = 0 ;
//
//
//    return iAvrGray ;
//    /*
//    for(i = 0 ; i <= iAvrGray ; i++){
//        dGrayPrcnt1 = iGrayCnt[i]           /(double)iAllPxCnt ;
//        dGrayPrcnt2 = iGrayCnt[iAvrGray+1+i]/(double)iAllPxCnt ; //�̺κп��� �ε��� �������ȴ�.... ���� ����..
//        dGrayW1     = dGrayW1 +  dGrayPrcnt1                ;
//        dGrayW2     = dGrayW2 +  dGrayPrcnt2                ;
//        dGrayA1     = dGrayA1 + (dGrayPrcnt1 *  i     )     ;
//        dGrayA2     = dGrayA2 + (dGrayPrcnt2 * (i+1+i))     ;
//        dGrayU1     = dGrayU1 + (dGrayA1 / dGrayW1) * (dGrayA1 / dGrayW1) ;
//        dGrayU2     = dGrayU2 + (dGrayA2 / dGrayW2) * (dGrayA2 / dGrayW2) ;
//
//        dResult = (((1-dGrayPrcnt1)*dGrayW1*dGrayU1) + (dGrayW2 * dGrayU2)) ;
//
//        if(dBig < dResult) { dBig = dResult ; iAutoThresh = i ; }
//
//    }
//
//    //delete [] iGrayCnt;
//
//    return iAutoThresh ;
//    */
//}
//



//���� ���� �Լ�.
//==============================================================================
bool CMatch_V01::Init()
{

    m_pTrainArea = new CArea() ;
    m_pTrainImg  = new CImage() ;

    Prop.bUseTrain = true  ;
    Prop.bCamera   = false ;

    //�� ���� �� ���߿� Ʈ��Ŀ �������� ���� �����ؾ� ��..
    CTracker * Tracker = new CTracker();
    Tracker -> RoiEnabled = true ;
    Tracker -> RoiWidth   = 30   ;
    Tracker -> SetOnPaint(OnTrackerChanged);
    m_lTrckInsp.PushBack(Tracker);
    m_lLPara   .PushBack(new TLocalPara ());

    return true ;

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

    return true ;

}


        //�˻� �� ���� ����.
bool CMatch_V01::Train(CImage * _pImg)
{
    m_sErrMsg = "";




    //if(bSkip   ) return true ;

    if(MPara.iTrInspOft <= MPara.iTrNoInspOft) {
        m_sErrMsg = "ERROR : iTrInspOft <= iTrNoInspOft";
        return false ;
    }


    Rslt.Clear();

    TRect Rect ;
    CTracker * Tracker = m_lTrckInsp.GetCrntData(true) ;
    Rect = Tracker -> GetRect();

    _pImg -> SetRect(&Rect); //�˻� �׸� �̹��� ������ �������� �̹����� ����� �ʰ� ������ �ٿ� �ش�.
    int iLeft   = Rect.Left    ;
    int iTop    = Rect.Top     ;
    int iRight  = Rect.Right   ;
    int iWidth  = Rect.Width ();
    int iHeight = Rect.Height();


    m_pTrainArea  -> SetSize(iWidth,iHeight) ; //�˻� ��ǥ

    _pImg -> Trim(Rect , m_pTrainImg); //��Ʈ ������ ��ŭ Ʈ���� �̹����� �߶� �ִ´�.

    bool bEdge  = false ;
    bool bPrePx = _pImg -> GetPixel(iLeft,iTop) ; //�ȼ��� ������� ������ ������ 1 ������ 0
    bool bPx    = _pImg -> GetPixel(iLeft,iTop) ;

    int iTrThreshold = MPara.iTrThreshold ;
    if(iTrThreshold==0){
        CAutoThreshold::TPara ATPara;
        CAutoThreshold::TRslt ATRslt;
        ATPara.bSimple = true ;
        if(CAutoThreshold::Inspection(_pImg , Rect , ATPara , &ATRslt)){ //���侲����尪�� �������� �˰�����
            iTrThreshold = ATRslt.iThreshold ; //������� ������ 0�̸� ���侲����带 �̾� �´�.
        }
        else {
            m_sErrMsg = ATRslt.sErrMsg;
            return false ;
        }
    }

    TCircle Circle ;

    //Set Temp Insp Area.
    Circle.rad = MPara.iTrInspOft ; //�˻� ������ ����ŷ �ϱ� ����
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
    Circle.rad = MPara.iTrNoInspOft ;
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

    //Ʈ���� ������ ���÷��� �ϱ� ����.. .����.
    Rslt.bInspEnd = true ;
    Rslt.bRsltOk  = true ;
    Rslt.dCntPosX = Orig.dCntPosX ;
    Rslt.dCntPosY = Orig.dCntPosY ;
    Rslt.dSinc    = 100  ;
    Rslt.tRect    = Rect ;

    CValue2 * Val ;
    Val = m_pValueList -> GetValueByName(MPara.sOutOfsXAdd);
    if(Val)Val -> Set(Rslt.dCntPosX-Orig.dCntPosX);
    Val = m_pValueList -> GetValueByName(MPara.sOutOfsYAdd);
    if(Val)Val -> Set(Rslt.dCntPosY-Orig.dCntPosY);




    return true ;
}

void CMatch_V01::RsltClear() //�˻� ������� �˻����� Ŭ���� �ѹ� �ϰ� �Ѵ�.
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
    Rslt.Clear();  //����� Ŭ����.

    CValue2 * Val ;
    if(m_bSkip) {
        //ã������ NG �ɼ�.
        Rslt.bRsltOk = true ;

        Val = m_pValueList -> GetValueByName(MPara.sOutOfsXAdd);
        if(Val)Val -> Set(0.0);
        Val = m_pValueList -> GetValueByName(MPara.sOutOfsYAdd);
        if(Val)Val -> Set(0.0);
        Rslt.bInspEnd = true ;

        return true ;
    }


    //���� ���̺����� ���ذ� �����V ������.
    double dInspOfsX =0, dInspOfsY=0;

    Val = m_pValueList -> GetValueByName(MPara.sRefOfsXAdd);
    if(Val)dInspOfsX = Val -> GetDouble();
    Val = m_pValueList -> GetValueByName(MPara.sRefOfsYAdd);
    if(Val)dInspOfsY = Val -> GetDouble();


    TRect tInspRect ;
    CTracker * Tracker = m_lTrckInsp.GetCrntData(true) ;
    TDRect tTrackerRect = Tracker -> GetDRect() ;
    tInspRect.left   = tTrackerRect.Left   - Tracker -> RoiWidth + dInspOfsX ;
    tInspRect.top    = tTrackerRect.top    - Tracker -> RoiWidth + dInspOfsY ;
    tInspRect.right  = tTrackerRect.right  + Tracker -> RoiWidth + dInspOfsX ;
    tInspRect.bottom = tTrackerRect.bottom + Tracker -> RoiWidth + dInspOfsY ;

    _pImg -> SetRect(&tInspRect) ;

    int iLeft   = tInspRect.Left    ;
    int iTop    = tInspRect.Top     ;
    int iRight  = tInspRect.Right   ;
    int iWidth  = tInspRect.Width ();
    int iHeight = tInspRect.Height();


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

    CLinkedList<TPxPoint> llInspPnt  ;  //�ӵ������ ���� �˻� �� ��ġ�� ����Ʈ�� �־ �˻��Ѵ�.

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

    //�̸� �˻��� ����Ʈ�� ��ũ�� ����Ʈ�� �ִ´�.
    //���߿� 
    //const int iInspFreq = 10 ; //�˻� �󵵼�.
    int  iTrInspFreq = MPara.iTrInspFreq;
    if(MPara.iTrInspFreq < 1)  iTrInspFreq = 1 ;
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

    //��ũ�� ����Ʈ���� �迭�� �ű��.
    TPxPoint * tInspPxPnt = new TPxPoint[iInspPxCnt] ;

    for(int i = 0 ; i < iInspPxCnt ; i++) {
        tInspPxPnt[i] = llInspPnt.GetCrntData(!i) ;
    }


    //�غ� �˻� �غ���..
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

            //�˻����� ��� ���ϱ�.
            iImgInspAvr = 0 ;
            iImgInspSum = 0 ;
            iImgAvrGap  = 0 ;
            for(int i = 0 ; i < iInspPxCnt ; i++) {
                Pnt = tInspPxPnt[i] ;
                iImgInspSum += _pImg -> GetPixel(iLeft + rx + Pnt.x , iTop + ry + Pnt.y) ;
            }
            iImgInspAvr = iImgInspSum / (float)iInspPxCnt ;

            iImgAvrGap = iImgInspAvr - iInspPxAvr ;

            //���Ͽ� �˻������� Ʈ���� ������ ���� ����.
            for(int i = 0 ; i < iInspPxCnt ; i++) {
                Pnt = tInspPxPnt[i] ;
                iTemp = _pImg -> GetPixel(iLeft + rx + Pnt.x , iTop + ry + Pnt.y) - iImgAvrGap - Pnt.px ;
                if(iTemp > 0) iLtErrCnt+=iTemp;
                else          iDkErrCnt-=iTemp;

                iInspErrCnt += abs(iTemp) ;
            }

            //���� �ȼ��� �� �ϰ� �������� ����.
            if(iInspErrCnt < iMinErrCnt) {
                iMinErrCnt = iInspErrCnt ;
                iMinErrX   = rx ;
                iMinErrY   = ry ;

                Rslt.iDkPxCnt     = iDkErrCnt ;
                Rslt.iLtPxCnt     = iLtErrCnt ;
                Rslt.dCntPosX     = iLeft + rx + m_pTrainArea -> GetWidth () / 2 ;
                Rslt.dCntPosY     = iTop  + ry + m_pTrainArea -> GetHeight() / 2 ;                         //ȭ��Ʈ �� ���� �������� ������ 50���� �̻� ������ �� ������ ����. 50���� �̸��� ���� ���� �ؼ� ������.
                Rslt.dSinc        = ((iInspPxCnt*256 - iMinErrCnt) / (float)(iInspPxCnt*256)) * 100.0 ;
                Rslt.tRect.left   = iLeft + rx ;
                Rslt.tRect.top    = iTop  + ry ;
                Rslt.tRect.right  = iLeft + rx + m_pTrainArea -> GetWidth () ;
                Rslt.tRect.bottom = iTop  + ry + m_pTrainArea -> GetHeight() ;
            }
        }

        
        

    }

    //ã������ NG �ɼ�.
    Rslt.bRsltOk = CPara.bFindNg ? Rslt.dSinc < CPara.dSinc : Rslt.dSinc >= CPara.dSinc ;

    Val = m_pValueList -> GetValueByName(MPara.sOutOfsXAdd);
    if(Val)Val -> Set(Rslt.dCntPosX-Orig.dCntPosX);
    Val = m_pValueList -> GetValueByName(MPara.sOutOfsYAdd);
    if(Val)Val -> Set(Rslt.dCntPosY-Orig.dCntPosY);
    Val = m_pValueList -> GetValueByName(MPara.sOutRsltAdd);
    if(Val)Val -> Set(Rslt.bRsltOk ? String("OK"):String("NG"));

    Rslt.bInspEnd = true ;

    delete [] tInspPxPnt ;

    return true ;

}

String CMatch_V01::GetErrMsg()
{
    return m_sErrMsg ;
}


        //ȭ�� ���.
void CMatch_V01::PaintRslt(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    if(!Rslt.bInspEnd) return ;
    if(m_bSkip) return ;

    //���߿� Gdi���� �����V ���� �ϰ� �ؼ�... ������ �����V ���� �� �Ⱦ��.......
    CGdi *Gdi = new CGdi(_hDc,_dScaleX , _dScaleY , _dOffsetX , _dOffsetY);
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
    Rect.left    = Rslt.tRect.left   ;                    //(Rslt.tRect.left - _dOffsetX) * _dScaleX;
    Rect.top     = Rslt.tRect.top    ;                    //(Rslt.tRect.top  - _dOffsetY) * _dScaleY;
    Rect.right   = Rslt.tRect.right  ; //+ Rslt.tRect.Width() ;//(Rslt.tRect.left - _dOffsetX+ Rslt.tRect.Width() ) * _dScaleX  ;
    Rect.bottom  = Rslt.tRect.bottom ; //top + Rslt.tRect.Height();//(Rslt.tRect.top  - _dOffsetY+ Rslt.tRect.Height()) * _dScaleY  ;

    double dRsltX = Rslt.dCntPosX;//(Rslt.dCntPosX - _dOffsetX)* _dScaleX;
    double dRsltY = Rslt.dCntPosY;//(Rslt.dCntPosY - _dOffsetY)* _dScaleY;

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
        if(CPara.bFindNg){
            if(!Rslt.bRsltOk) {
                m_pTrainArea -> ShowArea(_hDc ,_dScaleX , _dScaleY , _dOffsetX , _dOffsetY , Rect.left  , Rect.top );
                Gdi -> m_tPen.Color = Rslt.bRsltOk ? clLime : clRed ;
                Gdi -> Rect(false , Rect);

            }
        }
        else {
            m_pTrainArea -> ShowArea(_hDc ,_dScaleX , _dScaleY , _dOffsetX , _dOffsetY , Rect.left  , Rect.top );
            Gdi -> m_tPen.Color = Rslt.bRsltOk ? clLime : clRed ;
            Gdi -> Rect(false , Rect);
        }
    }

    Gdi -> Text(Rect.left + 5, Rect.top - 15 ,sTemp.c_str()) ;
    Gdi -> m_tPen.Color = Rslt.bRsltOk ? clLime : clRed ;
    Gdi -> LineCross(dRsltX , dRsltY , 10) ;

    if(!Rslt.bRsltOk){
        Gdi -> m_tText.Color = clRed ;
        Gdi -> m_tFont  .Height = 30       ;
        Gdi -> Text(5, 5 ,m_sErrMsg.c_str()) ;
    }

    delete Gdi;
}




void CMatch_V01::PaintTracker(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
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

        //�̹��� ȭ�� Ŭ�� �̺�Ʈ
bool CMatch_V01::MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
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


bool CMatch_V01::MouseMv(TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
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

void CMatch_V01::MouseUp(TMouseButton Button,TShiftState Shift )
{
    CTracker * Tracker ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Tracker -> TrackerUp(Button, Shift);
    }
}

        //�Ķ���� ���÷���
void CMatch_V01::UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara)
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

void CMatch_V01::UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara)
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

        //�ε� ���̺�.
void CMatch_V01::LoadSave(bool _bLoad , String _sVisnName)
{
    String sIniPath        = GM.GetJobFileFolder() + _sVisnName + "_" + m_sName + "_" + GetStaticPkgLink()->GetPkgName() + ".ini" ;
    String sTrainAreaPath  = GM.GetJobFileFolder() + _sVisnName + "_" + m_sName + "_TrainArea.bmp"  ;
    String sTrainImagePath = GM.GetJobFileFolder() + _sVisnName + "_" + m_sName + "_TrainImage.bmp" ;

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
        m_pTrainArea -> LoadBmp(sTrainAreaPath .c_str());
        m_pTrainImg  -> LoadBmp(sTrainImagePath.c_str());

        //����Ʈ�� ������ 1���� �Ǳ� ������ �ʿ� ����.
        //m_lTrckInsp.DeleteAll();

        if(FileExists(sIniPath)){
            //Tracker ;
            Tracker = m_lTrckInsp.GetData(0);
            Tracker -> LoadSave(_bLoad , sIniPath , "Tracker0");

            LoadSaveList(_bLoad , sIniPath , "MasterPara" , &MPara.List);
            LoadSaveList(_bLoad , sIniPath , "CommonPara" , &CPara.List);

            Ini.Load(sIniPath, "TrainRslt"  , "dCntPosX" , Orig .dCntPosX );
            Ini.Load(sIniPath, "TrainRslt"  , "dCntPosY" , Orig .dCntPosY );
        }
        else {
            MPara.SetDefault();
            CPara.SetDefault();
        }

    }
    else {
        iTrackerCnt = m_lTrckInsp.GetDataCnt() ;

        //if(m_sCrntJobFilePath != "")Ini.ClearFile(sIniPath       ) ;
        //if(m_sCrntJobFilePath != "")Ini.ClearFile(sTrainAreaPath ) ;
        //if(m_sCrntJobFilePath != "")Ini.ClearFile(sTrainImagePath) ;

        m_pTrainArea -> SaveBmp(sTrainAreaPath .c_str());
        m_pTrainImg  -> SaveBmp(sTrainImagePath.c_str());

        //Tracker ;
        Tracker = m_lTrckInsp.GetData(0);
        Tracker -> LoadSave(_bLoad , sIniPath , "Tracker0");

        LoadSaveList(_bLoad , sIniPath , "MasterPara" , &MPara.List);
        LoadSaveList(_bLoad , sIniPath , "CommonPara" , &CPara.List);

        Ini.Save(sIniPath, "TrainRslt"  , "dCntPosX" , Orig .dCntPosX );
        Ini.Save(sIniPath, "TrainRslt"  , "dCntPosY" , Orig .dCntPosY );
    }

}



