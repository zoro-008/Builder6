//---------------------------------------------------------------------------
#pragma hdrstop
#include "Vision.h"
#include "Image.h"
#include "CamUnit.h"
#include "UserFile.h"
#include "GdiUnit.h"
#include "aBlob.h"
#include "OptionMan.h"
#include "SLogUnit.h"
#include <math.h>
//#include "ParaUnit.h"
//#include "VOptnMan.h"
//#include "UserFile.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
CVision VSN;

TVisnRslt VisnRslt[MAX_STAGE_CNT];

CVision::CVision()
{
    Init();

}

CVision::~CVision()
{
    Close();

}

bool CVision::Init()
{

    for(int i = 0 ; i < MAX_IMG_ID ; i++) {
        g_pImg[i] = new CImage() ;
        g_pImg[i] -> LoadBmp(GetExeDir("Util\\Images\\640_480_8.bmp").c_str());
        //g_pImg[i] -> LoadBmp(GetExeDir("Util\\Images\\1.bmp").c_str());
        //"D:\\BC6Works\\IM\\HRV_100\\bin\\Util\\Images\\black.bmp"
        g_pImg[i] -> ClrImage();
    }
    g_pCam[ciCam1] = new CImi() ;
    g_pCam[ciCam1]-> SetGrabFunc(CamCallback);


    m_bLiveMode = false ;
    m_bGrabbing = false ;

    InitializeCriticalSection(&m_hImageSection);
}

void CVision::Close()
{
    Live(false,0);

    DeleteCriticalSection(&m_hImageSection);

    g_pCam[ciCam1] -> SetGrabFunc(NULL);

    //while(m_bGrabbing){
    //    Sleep(1);
    //}

    for(int i = 0 ; i < MAX_IMG_ID ; i++) {
        delete g_pImg[i] ;
        g_pImg[i] = NULL ;
    }

    delete g_pCam[ciCam1] ;
    g_pCam[ciCam1] = NULL ;
}


void CVision::Grab(int _iActiveStg )
{
    m_bLiveMode = false ;
    if(m_bGrabbing) return ;

    m_iActiveStg = _iActiveStg ;



    //TimeInfo.iGrabStart = GetTickCount() ;

    //Camera Setting.
    g_pCam[ciCam1] -> SetGrabFunc(CamCallback) ; //ī�޶� �ݹ� ���.

    //Grab
    g_pCam[ciCam1] -> Grab() ;
    m_bGrabbing = true ;

}

void CVision::Live(bool _bOn , int _iActiveStg )
{
    if(m_bLiveMode == _bOn) return ;
    m_bLiveMode = _bOn ;



    if(_bOn){
        m_iActiveStg = _iActiveStg ;

        //Camera Setting.
        g_pCam[ciCam1] -> SetGrabFunc(CamCallback) ; //ī�޶� �ݹ� ���.

        //Grab
        g_pCam[ciCam1] -> Grab() ;
        m_bGrabbing = true ;
    }
}

bool CVision::GetGrabEnd()
{
    return  !m_bGrabbing ;
}

bool CVision::GetLive()
{
    return m_bLiveMode ;

}

/*
void CVision::Inspection(EN_IMG_ID _iSttImage   ,
                         EN_IMG_ID _iDriveImage ,
                         EN_IMG_ID _iFullImage  ,
                         EN_IMG_ID _iMaxImage   , TVisnRslt * _pVisnRslt)
{
    //����� �ϴ� Ŭ����.
    memset(_pVisnRslt , 0 , sizeof(TVisnRslt));

    //�Ķ����.
    BLB_Para Para ;
    Para.iPitchX     =1 ; //��� �˻� X ��ġ
    Para.iPitchY     =3 ; //��� �˻� Y ��ġ
    Para.iStartXOfs  =0 ; //ó�� ��ŸƮ �ϴ� ��ġ �����V.
    Para.iStartYOfs  =0 ; //ó�� ��ŸƮ �ϴ� ��ġ �����V.
    Para.iThreshold  =30 ; //�˻� Ʈ�����. 0=���佺�����
    Para.bFindLtOnDk =true ; //����������ü���� ����� ������ü����.
    Para.bIgnrInside =true ; //��� ���� ��� ����.
    Para.bIgnrSide   =false; //�簢�� ������ ��迡 �ִ� ��� ����.
    //Filtering.
    Para.iMinArea       =0 ; Para.iMaxArea        =0; //������ �θƽ���
    Para.iMinWidth      =0 ; Para.iMaxWidth       =0; //����
    Para.iMinHeight     =0 ; Para.iMaxHeight      =0; //����
    Para.iMinBoundary   =0 ; Para.iMaxBoundary    =0; //�׵θ�
    Para.fMinCompactness=0 ; Para.fMaxCompactness =0; //������/�׵θ�
    Para.fMinAngle      =0 ; Para.fMaxAngle       =0; //���� �������� �ʾ���.
    Para.fMinAspectRatio=0 ; Para.fMaxAspectRatio =0; //

    //�˻� ����... ���δ�..
    TRect InspRect ;
    InspRect.left   = 0 ;
    InspRect.top    = 0 ;
    InspRect.right  = g_pImg[_iSttImage]->GetWidth ()-1;
    InspRect.bottom = g_pImg[_iSttImage]->GetHeight()-1;




    CBlob Blob ; //�˻� Ŭ����
    BLB_Rslt Rslt ; //�˻� ���.

    //===============
    //ù��° �̹��� �˻�.
    Blob.Inspect(g_pImg[_iSttImage] , InspRect ,Para ,  &Rslt);   //bool Inspect  (CImage *_pImg , TRect     _tRect , BLB_Para _tPara , BLB_Rslt *_pRslt        );

    //���� ū��.
    SBlob BiggestBlob ;
    int iBlobSize = 0 ;
    int iBigBlobSize = 0 ;
    for(int i = 0 ; i < Rslt.iBlobCnt ; i ++) {
        iBlobSize = Rslt.pBlobs[i].iBoundCnt ;
        iBigBlobSize = BiggestBlob.iBoundCnt ;
        if(iBlobSize > iBigBlobSize) {
            iBigBlobSize = iBlobSize  ;
            BiggestBlob  = Rslt.pBlobs[i] ;
        }
    }

    //����� ����.
    _pVisnRslt -> dSttCntX        = BiggestBlob.iCenterX ;
    _pVisnRslt -> dSttCntY        = BiggestBlob.iCenterY ;
    _pVisnRslt -> SttRect.left    = BiggestBlob.left     ;
    _pVisnRslt -> SttRect.top     = BiggestBlob.top      ;
    _pVisnRslt -> SttRect.right   = BiggestBlob.right    ;
    _pVisnRslt -> SttRect.Bottom  = BiggestBlob.bottom   ;




    //===============
    //Drive �̹��� �˻�.
    Blob.Inspect(g_pImg[_iDriveImage] , InspRect ,Para ,  &Rslt);   //bool Inspect  (CImage *_pImg , TRect     _tRect , BLB_Para _tPara , BLB_Rslt *_pRslt        );

    //���� ū��.
    iBlobSize = 0 ;
    iBigBlobSize = 0 ;
    BiggestBlob.OnClear();
    for(int i = 0 ; i < Rslt.iBlobCnt ; i ++) {
        iBlobSize = Rslt.pBlobs[i].iBoundCnt ;
        iBigBlobSize = BiggestBlob.iBoundCnt ;
        if(iBlobSize > iBigBlobSize) {
            iBigBlobSize = iBlobSize  ;
            BiggestBlob  = Rslt.pBlobs[i] ;
        }
    }
    //����� ����.
    _pVisnRslt -> dMaxCntX        = BiggestBlob.iCenterX ;
    _pVisnRslt -> dMaxCntY        = BiggestBlob.iCenterY ;
    _pVisnRslt -> MaxRect.left    = BiggestBlob.left  ;
    _pVisnRslt -> MaxRect.top     = BiggestBlob.top   ;
    _pVisnRslt -> MaxRect.right   = BiggestBlob.right ;
    _pVisnRslt -> MaxRect.bottom  = BiggestBlob.bottom;


    //==================
    //��� ���� �����.
    //unsigned char cPxStt = 0 ;
    //unsigned char cPxEnd = 0 ;
    ////g_pImg[_iRsltImage] = g_pImg[_iSttImage] ;  // SetSize(g_pImg[_iSttImage]->GetWidth () , g_pImg[_iSttImage]->GetHeight() , g_pImg[_iSttImage]->)() ;
    //g_pImg[_iRsltImage]->CopyImage(g_pImg[_iSttImage]);
    //for(int y = _pVisnRslt -> MaxRect.top ; y < _pVisnRslt -> MaxRect.Bottom ; y++){
    //    for(int x = _pVisnRslt -> MaxRect.left ; x < _pVisnRslt -> MaxRect.right ; x++){
    //        cPxStt = g_pImg[_iRsltImage]->GetPixel(x,y);
    //        cPxEnd = g_pImg[_iEndImage ]->GetPixel(x,y);
    //
    //        if(cPxStt < cPxEnd) g_pImg[_iRsltImage]->SetPixel(x,y,cPxEnd);
    //
    //    }
    //}
} */

void CVision::LiveInspection(EN_IMG_ID _iInspImage ,  TLiveRslt * _tLiveRslt )
{
    //����� �ϴ� Ŭ����.
    _tLiveRslt->Rslt.Clear() ;
    _tLiveRslt->RsltRect.left   = 0 ;
    _tLiveRslt->RsltRect.top    = 0 ;
    _tLiveRslt->RsltRect.right  = 0 ;
    _tLiveRslt->RsltRect.bottom = 0 ;

    //�Ķ����.
    BLB_Para Para ;
    Para.iPitchX     =1 ; //��� �˻� X ��ġ
    Para.iPitchY     =1 ; //��� �˻� Y ��ġ
    Para.iStartXOfs  =0 ; //ó�� ��ŸƮ �ϴ� ��ġ �����V.
    Para.iStartYOfs  =0 ; //ó�� ��ŸƮ �ϴ� ��ġ �����V.
    Para.iThreshold  =30 ; //�˻� Ʈ�����. 0=���佺�����
    Para.bFindLtOnDk =true ; //����������ü���� ����� ������ü����.         f
    Para.bIgnrInside =true ; //��� ���� ��� ����.
    Para.bIgnrSide   =false; //�簢�� ������ ��迡 �ִ� ��� ����.
    //Filtering.
    Para.iMinArea       =0 ; Para.iMaxArea        =0; //������ �θƽ���
    Para.iMinWidth      =0 ; Para.iMaxWidth       =0; //����
    Para.iMinHeight     =0 ; Para.iMaxHeight      =0; //����
    Para.iMinBoundary   =0 ; Para.iMaxBoundary    =0; //�׵θ�
    Para.fMinCompactness=0 ; Para.fMaxCompactness =0; //������/�׵θ�
    Para.fMinAngle      =0 ; Para.fMaxAngle       =0; //���� �������� �ʾ���.
    Para.fMinAspectRatio=0 ; Para.fMaxAspectRatio =0; //

    //�˻� ����... ���δ�..
    TRect InspRect ;
    InspRect.left   = 0 ;
    InspRect.top    = 0 ;
    InspRect.right  = g_pImg[_iInspImage]->GetWidth ()-1;
    InspRect.bottom = g_pImg[_iInspImage]->GetHeight()-1;




    CBlob Blob ; //�˻� Ŭ����
    BLB_Rslt Rslt ; //�˻� ���.

    //===============
    //ù��° �̹��� �˻�.
    Trace("Blob Start","------");
    Blob.Inspect(g_pImg[_iInspImage] , InspRect ,Para ,  &Rslt);   //bool Inspect  (CImage *_pImg , TRect     _tRect , BLB_Para _tPara , BLB_Rslt *_pRslt        );
    Trace("Blob End","------");

    //���� ū��.
    SBlob BiggestBlob ;
    BiggestBlob.OnClear();
    int iBlobSize = 0 ;
    int iBigBlobSize = 0 ;
    for(int i = 0 ; i < Rslt.iBlobCnt ; i ++) {
        iBlobSize = Rslt.pBlobs[i].iBoundCnt ;
        iBigBlobSize = BiggestBlob.iBoundCnt ;
        if(iBlobSize > iBigBlobSize) {
            iBigBlobSize = iBlobSize  ;
            BiggestBlob  = Rslt.pBlobs[i] ;
        }
    }


    //�˻� �Ϸ��� ���� ū�� ����� �� ����鼭 ���������� �����ͿəV�� �����Ͽ� ������ �ű�.
    //=======================================================================================
        TRect TrimRect ;
        TrimRect.left   = BiggestBlob.left  -1;
        TrimRect.top    = BiggestBlob.top   -1;
        TrimRect.right  = BiggestBlob.right +1;
        TrimRect.bottom = BiggestBlob.bottom+1;


        //UI���� �д����� ���õȰ��� PX������ �ٲٰ�.
        double dMstOfsXPx =  OM.MstDevOptn.dMstOfsX[m_iActiveStg] * g_pImg[iiSTGCrnt]->GetHeight() / OM.MstOptn.dVisnYMaxMin ;
        double dMstOfsYPx =  OM.MstDevOptn.dMstOfsY[m_iActiveStg] * g_pImg[iiSTGCrnt]->GetHeight() / OM.MstOptn.dVisnYMaxMin ;
        //dMstOfsYPx *= -1 ;     //Y���� ���� ���� ���� +�������� �ٲ۴�. 20150615 ǰ���� ��û����.



        //���� 1�ȼ� Ȯ�� ���Ѽ� �ڿ������� �Ѵ�.
        if(TrimRect.left   < 0                               )TrimRect.left   = 0;
        if(TrimRect.top    < 0                               )TrimRect.top    = 0;
        if(TrimRect.right  >=g_pImg[_iInspImage]->GetWidth ())TrimRect.right  = g_pImg[_iInspImage]->GetWidth () - 1 ;
        if(TrimRect.bottom >=g_pImg[_iInspImage]->GetHeight())TrimRect.bottom = g_pImg[_iInspImage]->GetHeight() - 1 ;

        char * TempImg = new char [TrimRect.Width() * TrimRect.Height()] ;
        for(int y = 0 ; y <  TrimRect.Height() ; y++){
            for(int x = 0 ; x < TrimRect.Width() ; x++){
                TempImg[y*TrimRect.Width()+x] = g_pImg[_iInspImage]->GetPixel(TrimRect.Left+ x,TrimRect.Top+y);
            }
        }
        g_pImg[_iInspImage]->ClrImage();
        for(int y = 0 ; y <  TrimRect.Height() ; y++){
            for(int x = 0 ; x < TrimRect.Width() ; x++){
                if(TrimRect.Left+x +dMstOfsXPx < 0                               ) continue ;
                if(TrimRect.Top +y +dMstOfsYPx < 0                               ) continue ;
                if(TrimRect.Left+x +dMstOfsXPx >=g_pImg[_iInspImage]->GetWidth ()) continue ;
                if(TrimRect.Top +y +dMstOfsYPx >=g_pImg[_iInspImage]->GetHeight()) continue ;
                g_pImg[_iInspImage]->SetPixel(TrimRect.Left+x +dMstOfsXPx,
                                              TrimRect.Top +y +dMstOfsYPx,
                                              TempImg[y*TrimRect.Width()+x]);
            }
        }
        delete [] TempImg ;
    //==========================================================================================


    //����� ����.
    _tLiveRslt -> Rslt.x          = BiggestBlob.iCenterX +dMstOfsXPx;
    _tLiveRslt -> Rslt.y          = BiggestBlob.iCenterY +dMstOfsYPx;
    _tLiveRslt -> RsltRect.left   = BiggestBlob.left     +dMstOfsXPx;
    _tLiveRslt -> RsltRect.top    = BiggestBlob.top      +dMstOfsYPx;
    _tLiveRslt -> RsltRect.right  = BiggestBlob.right    +dMstOfsXPx;
    _tLiveRslt -> RsltRect.bottom = BiggestBlob.bottom   +dMstOfsYPx;

    g_pImg[_iInspImage]->SetRect(&_tLiveRslt -> RsltRect);
    g_pImg[_iInspImage]->SetX   (&_tLiveRslt -> Rslt.x  );
    g_pImg[_iInspImage]->SetY   (&_tLiveRslt -> Rslt.y  );




    /*
    if(_tLiveRslt -> Rslt.x < 0                               )_tLiveRslt -> Rslt.x = 0 ;
    if(_tLiveRslt -> Rslt.y < 0                               )_tLiveRslt -> Rslt.y = 0 ;
    if(_tLiveRslt -> Rslt.x >=g_pImg[_iInspImage]->GetWidth ())_tLiveRslt -> Rslt.x =g_pImg[_iInspImage]->GetWidth () -1;
    if(_tLiveRslt -> Rslt.y >=g_pImg[_iInspImage]->GetHeight())_tLiveRslt -> Rslt.y =g_pImg[_iInspImage]->GetHeight() -1;

    if(_tLiveRslt -> RsltRect.left   < 0                               )_tLiveRslt -> RsltRect.left   = 0 ;
    if(_tLiveRslt -> RsltRect.top    < 0                               )_tLiveRslt -> RsltRect.top    = 0 ;
    if(_tLiveRslt -> RsltRect.right  >=g_pImg[_iInspImage]->GetWidth ())_tLiveRslt -> RsltRect.right  =g_pImg[_iInspImage]->GetWidth () -1;
    if(_tLiveRslt -> RsltRect.bottom >=g_pImg[_iInspImage]->GetHeight())_tLiveRslt -> RsltRect.bottom =g_pImg[_iInspImage]->GetHeight() -1;
    */








    //if(BiggestBlob.iCenterX == 0 && BiggestBlob.iCenterY == 0) g_pImg[_iInspImage]->SaveBmp("D:\\sunsun.bmp") ;
}


void CVision::PaintCntLine(HDC _hDc , int _iWidth , int _iHeight , float _fOffsetX , float _fOffsetY , float _fScaleX , float _fScaleY)
{

    CGdi *Gdi = new CGdi(_hDc);

    //Gdi -> SetOffset(_fOffsetX , _fOffsetY);
    //Gdi -> SetScale (_fScaleX , _fScaleY);



    int iCntrX = _iWidth /2  ;
    int iCntrY = _iHeight/2  ;
    Gdi -> m_tPen.Style = psDot  ;
    Gdi -> m_tPen.Color = clSilver ;
    Gdi -> LinePlus(iCntrX , iCntrY , _iWidth);

    int iGap = _iHeight / 6 ; //40 ;
    int iCnt = _iWidth / (20*2);

    //double dPxRes = OM.MstOptn.dVisnYMaxMin / _iHeight ;
    double dPxRes = OM.CmnOptn.dVisnYMaxDpMin / _iHeight ;
    double dGapMin ;
    String sGapMin ;

    for(int i = 1; i < iCnt ; i++ ) {
        Gdi -> Circle(false , iCntrX  , iCntrY  , i*iGap , 0);
        dGapMin = dPxRes * i*iGap ;
        sGapMin = sGapMin.sprintf("%.1f",dGapMin);
        Gdi -> m_tText.Color = clLime ;
        Gdi -> m_tFont.Height = 10 ;
        Gdi -> Text(iCntrX +i*iGap ,iCntrY+5 , sGapMin.c_str()  );

    }

    delete Gdi;
}

double GetScaled(double _dVal , double _dOffset , double _dScale)
{
    //double dPxRes = OM.CmnOptn.dVisnYMaxDpMin / _iHeight ;

    double dRet = (_dVal + _dOffset) * _dScale ;

    return dRet ;

}
int CVision::Paint(HDC _hDc , EN_DISP_OPTN _iDpOptn , EN_IMG_ID _iImage ,
                   int _iDstX, int _iDstY, int _iDstW, int _iDstH ,
                   int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH ,
                   float _fScaleX , float _fScaleY ,int _iThreshold , bool _bCntrLine , TVisnRslt & _pVisnRslt , TVisnRsltPnt & _pVisnRsltPnt, bool _bViewRslt)
{
   // CVisionBase::Paint(_hDc , _iDpOptn ,  _iDstX,  _iDstY,  _iDstW,  _iDstH , _iSrcX, _iSrcY, _iSrcW, _iSrcH , _fScaleX , _fScaleY ,_iThreshold , _bCntrLine);


    int iRet ;

         if(doGray   == _iDpOptn) {iRet = g_pImg[_iImage]->ShowImgGrey  (_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH);             }
    else if(doColor  == _iDpOptn) {iRet = g_pImg[_iImage]->ShowImgColor (_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH);             }
    else if(doFalse  == _iDpOptn) {iRet = g_pImg[_iImage]->ShowImgFalse (_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH);             }
    else if(doBinary == _iDpOptn) {iRet = g_pImg[_iImage]->ShowImgBinary(_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH ,_iThreshold);}
    else if(doRed    == _iDpOptn) {iRet = g_pImg[_iImage]->ShowImgR     (_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH);             }
    else if(doGreen  == _iDpOptn) {iRet = g_pImg[_iImage]->ShowImgG     (_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH);             }
    else if(doBlue   == _iDpOptn) {iRet = g_pImg[_iImage]->ShowImgB     (_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH);             }

    if(_bCntrLine) PaintCntLine(_hDc , _iDstW , _iDstH,_iSrcX , _iSrcY,_fScaleX , _fScaleY);

    CGdi * Gdi = new CGdi(_hDc);
    //Gdi -> SetOffset(_iSrcX , _iSrcY);
    //Gdi -> SetScale(_fScaleX , _fScaleY);
    Gdi -> m_tPen.Style = psSolid ;
    Gdi -> m_tPen.Width = 1  ;

   // double dPxRes = OM.CmnOptn.dVisnYMaxDpMin / _iHeight ;


    //if(m_tLiveRslt.x || m_tLiveRslt.y){
    //
    //}

    double dOfs   = -_iSrcX  ;
    double dScale = _fScaleX ;




    String sGap ;
    String sXY  ;

    //���̺� ��.
    double dDispRsltX ;
    double dDispRsltY ;

    //�������.
    double dDispRsltSttX ; double dDispRsltSttY ;
    double dDispRsltEndX ; double dDispRsltEndY ;




    if(!_bViewRslt) { //���� ���̺� ������� ���
        dDispRsltX = (m_tLiveRslt.Rslt.x - _iSrcX) * _fScaleX ;
        dDispRsltY = (m_tLiveRslt.Rslt.y - _iSrcY) * _fScaleY ;

        Gdi -> m_tPen.Color = clYellow ;
        Gdi -> LinePlus(dDispRsltX  , dDispRsltY , 10);

        //ȭ�鿡 ���÷��̴� �ȼ������� �ƴ� �д����� ȯ��.
        double dTemp1 = (m_tLiveRslt.Rslt.x - g_pImg[_iImage]->GetWidth ()/2) * OM.MstOptn.dVisnYMaxMin / g_pImg[_iImage]->GetHeight();
        double dTemp2 = (m_tLiveRslt.Rslt.y - g_pImg[_iImage]->GetHeight()/2) * OM.MstOptn.dVisnYMaxMin / g_pImg[_iImage]->GetHeight();
        //dTemp2 *= -1 ; //ƿƮ�����ʹ� Y���� ���� ���� +�� �ȴ�.20150615 ǰ���� ��û����.
        double dTemp3 = sqrt(dTemp1 * dTemp1 + dTemp2 * dTemp2)  ;

        sGap = sGap.sprintf("%.1f",dTemp3);
        sXY  = sXY .sprintf("X:%.1f,Y:%.1f",dTemp1,dTemp2);
        Gdi -> m_tText.Color = clYellow;
        Gdi -> m_tFont.Height = 15 ;
        Gdi -> Text(dDispRsltX + 3 , dDispRsltY + 3 , sGap.c_str()  );
        Gdi -> Text(dDispRsltX + 3 , dDispRsltY + 5 + Gdi -> m_tFont.Height, sXY.c_str()  );
    }

    else { //���� ����� ���.
        //���� ��� �׸���.
        //double dLastNon0X , dLastNon0Y ; //���� �ֱٿ� 0,0�� �ƴѰ�.
    
        //2014.09.25
        //�ȳ��ϼ���. ���ͱ� �����
        //���̿� ����2�� ����� ����Դϴ�
        //���� �ܺ� ���� �� ����� ���� Tilt�� ���� ���� ������ ����� ��� ���� �׸��� ���� ���� ��ܿ� ǥ�õǰ� �Ǿ� �� ������ �ʷϼ����� ǥ���ϰ� �Ǿ� �ֽ��ϴ�.
        //�� �κ��� �ܺ� ���� �� ����� �־� ���� ���� ������ ����� ��� �� ����Ʈ�� ���� ����� �ƴ� ������ ������ ��ü �Ͽ����� �մϴ�
    

        int a ;
        if(_pVisnRsltPnt.iDataCnt >= 1) {
            for(int i = 1 ; i < _pVisnRsltPnt.iDataCnt ; i++) {
                Gdi -> m_tPen.Color = clGreen ;
                if(_pVisnRsltPnt.tData[i-1].x != 0 && _pVisnRsltPnt.tData[i-1].y != 0 &&
                   _pVisnRsltPnt.tData[i  ].x != 0 && _pVisnRsltPnt.tData[i  ].y != 0 ){
                    dDispRsltSttX = (_pVisnRsltPnt.tData[i-1].x - _iSrcX) * _fScaleX ;
                    dDispRsltSttY = (_pVisnRsltPnt.tData[i-1].y - _iSrcY) * _fScaleY ;
    
                    dDispRsltEndX = (_pVisnRsltPnt.tData[i  ].x - _iSrcX) * _fScaleX ;
                    dDispRsltEndY = (_pVisnRsltPnt.tData[i  ].y - _iSrcY) * _fScaleY ;

    
    
                    Gdi -> Line(dDispRsltSttX,dDispRsltSttY ,dDispRsltEndX ,dDispRsltEndY) ;
                }
                else {
                    a++ ;
                }
            }
        }

        double dDispSttCntX ;
        double dDispSttCntY ;

        double dDispDrvCntX ;
        double dDispDrvCntY ;


        dDispSttCntX = (_pVisnRslt.dSttCntX - _iSrcX) * _fScaleX ;
        dDispSttCntY = (_pVisnRslt.dSttCntY - _iSrcY) * _fScaleY ;

        dDispDrvCntX = (_pVisnRslt.dDriveCntX - _iSrcX) * _fScaleX ;
        dDispDrvCntY = (_pVisnRslt.dDriveCntY - _iSrcY) * _fScaleY ;







        /*if(OM.DevOptn.bUseBfCttOfs) {
            dDispSttCntX += (_pVisnRslt.dBfCttX - _pVisnRslt.dSttCntX) * _fScaleX;
            dDispSttCntY += (_pVisnRslt.dBfCttY - _pVisnRslt.dSttCntY) * _fScaleY;

            dDispDrvCntX += (_pVisnRslt.dBfCttX - _pVisnRslt.dSttCntX) * _fScaleX;
            dDispDrvCntY += (_pVisnRslt.dBfCttY - _pVisnRslt.dSttCntY) * _fScaleY;
        }*/


        //�ƽ� ƿƮ �ּ� Ǯ���� ������ ��� �������.
        //if(OM.DevOptn.bUseTiltMax) {
        //    Gdi -> m_tPen.Color = clFuchsia ;
        //    Gdi -> LineAllow(_pVisnRslt.dSttCntX,_pVisnRslt.dSttCntY , _pVisnRslt.dMaxCntX,_pVisnRslt.dMaxCntY );
        //    sGap = "MAX";//sGap.sprintf("Max:%.1f",dTemp3);
        //    Gdi -> m_tText.Color = clFuchsia ;
        //    Gdi -> m_tFont.Height = 20 ;
        //    Gdi -> Text(_pVisnRslt.dMaxCntX + 3 , _pVisnRslt.dMaxCntY  + 3 , sGap.c_str());
        //}
        //
        ////Ǯ ƿƮ �ּ� Ǯ���� ������ ��� �������.
        //if(OM.DevOptn.bUseTiltFull) {
        //    Gdi -> m_tPen.Color = clYellow ;
        //    Gdi -> LineAllow(_pVisnRslt.dSttCntX,_pVisnRslt.dSttCntY , _pVisnRslt.dFullTiltX,_pVisnRslt.dFullTiltY );
        //    sGap = "FULL";//sGap.sprintf("Full:%.1f",dTemp3);
        //    Gdi -> m_tText.Color = clYellow ;
        //    Gdi -> m_tFont.Height = 20 ;
        //    Gdi -> Text(_pVisnRslt.dFullTiltX + 3 , _pVisnRslt.dFullTiltY  + 3 , sGap.c_str());
        //}
        //
    
        //����̺� ƿƮ.
        //if(OM.DevOptn.bUseTiltDrive) {
            Gdi -> m_tPen.Color = clRed ;//clLime ;
            if(_pVisnRslt.dDriveCntX != 0.0 || _pVisnRslt.dDriveCntX != 0.0) {
                Gdi -> LineAllow(dDispSttCntX,dDispSttCntY , dDispDrvCntX,dDispDrvCntY );
            }
            //
            Gdi -> m_tText.Color = clLime ;
            Gdi -> m_tFont.Height = 12 ;
            //Gdi -> Text(_pVisnRslt.dDriveCntX + 3 , _pVisnRslt.dDriveCntY  + 3 , sGap.c_str());


            //20160121 ���켮 ���� ��û. �ο� ������ Ȯ�� ���� �ϰ� ����.
            sGap.sprintf("InitX:%.1f Y:%.1f DriveX:%.1f Y:%.1f",_pVisnRslt.dSttCntX  * OM.MstOptn.dVisnYMaxMin / g_pImg[_iImage]->GetHeight(),
                                                                _pVisnRslt.dSttCntY  * OM.MstOptn.dVisnYMaxMin / g_pImg[_iImage]->GetHeight(),
                                                                _pVisnRslt.dDriveCntX* OM.MstOptn.dVisnYMaxMin / g_pImg[_iImage]->GetHeight(),
                                                                _pVisnRslt.dDriveCntY* OM.MstOptn.dVisnYMaxMin / g_pImg[_iImage]->GetHeight());
            Gdi -> Text(3 , 3 , sGap.c_str());
        //}
    }

    delete Gdi ;

    return iRet ;


//   m_pImg->ShowImgGrey  (_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH);

/*
    if(g_bSettingMode)PlcInsp.pTrckInsp1 -> Paint(_hDc,_fScaleX , _fScaleY);
    if(g_bSettingMode)PlcInsp.pTrckInsp2 -> Paint(_hDc,_fScaleX , _fScaleY);

    if(Stat.bDispRslt){

        IGdi *Gdi = GDI_New(_hDc);

        //�ʿ� ����.
        //Gdi -> SetScale(_fScaleX , _fScaleY);

        Gdi -> m_tPen.Style = psSolid ;
        Gdi -> m_tPen.Color = clYellow ;
        Gdi -> m_tPen.Width = 1  ;

        Gdi -> m_tText.Color = Rslt.iRet == veOk ? clLime : clRed ;
        Gdi -> m_tFont.Height = 40 ;
        Gdi -> Text(5 , 5 , m_sRsltMsg.c_str());

        GDI_Del(Gdi);
    }
*/
}

//��ȭ�鿡�� �ݹ� ���� ����Ʈ �Լ�.
void CVision::SetPaintCallback(PaintCallbackFunc _pCalback)
{
    m_pPaintCallback = _pCalback ;
}



//�׷��ÿ� ī�޶� ������ִ� �ݹ�. ī�޶� �̹��� ������ ȣ���.
void CVision::CamCallback (int _iCamNo)
{
    unsigned char * pImg ;
    int iImgWidth  ;
    int iImgHeight ;
    int iImgBit    ;

    //TimeInfo.iGrab = GetTickCount() - TimeInfo.iGrabStart ;


    int iImgCpyStartTime = GetTickCount() ;
    g_pCam[ciCam1   ] -> GetImg(pImg , iImgWidth , iImgHeight , iImgBit);

    EnterCriticalSection(&m_hImageSection); //ī�޶� �׷�������� �� ������� �ٸ��Ŷ� ���Ⱑ �ʸ¾� ����˰� �ö�ö� �־ ����.
        g_pImg[iiSTGCrnt] -> SetImage(pImg , iImgWidth , iImgHeight , iImgBit);
        //TimeInfo.iImgCpy =  GetTickCount() - iImgCpyStartTime ;

        TLiveRslt tRslt ;
        LiveInspection(iiSTGCrnt , &tRslt);
        m_tLiveRslt = tRslt ;
    LeaveCriticalSection(&m_hImageSection);


    //Stat.bDispRslt = g_bSettingMode ; //�˻�ÿ��� ����� ���� �ȵǰ�. m_bDispRslt �ν���ǽÿ� On��Ŵ.

    if(m_pPaintCallback)m_pPaintCallback();

    m_bGrabbing = false ;

    if(m_bLiveMode){
        //Camera Setting.
        g_pCam[ciCam1] -> SetGrabFunc(CamCallback) ; //ī�޶� �ݹ� ���.

        //Grab
        g_pCam[ciCam1] -> Grab() ;
        m_bGrabbing = true ;
    }

}

bool CVision::ImgCopy(EN_IMG_ID _iSrc , EN_IMG_ID _iDst)
{
    //*g_pImg[_iDst] = *g_pImg[_iSrc];

    EnterCriticalSection(&m_hImageSection);
    g_pImg[_iDst]->CopyImage(g_pImg[_iSrc]);
    LeaveCriticalSection(&m_hImageSection);
}

bool CVision::ImgRectMerge(EN_IMG_ID _iSrc , EN_IMG_ID _iDst , TRect _tRect , int _iOfsX , int _iOfsY )
{
    unsigned char cPxSrc = 0 ;
    unsigned char cPxDst = 0 ;
    //g_pImg[_iRsltImage] = g_pImg[_iSttImage] ;  // SetSize(g_pImg[_iSttImage]->GetWidth () , g_pImg[_iSttImage]->GetHeight() , g_pImg[_iSttImage]->)() ;
    //g_pImg[_iRsltImage]->CopyImage(g_pImg[_iSttImage]);





    EnterCriticalSection(&m_hImageSection);
        //�߸޷� �ϴ� ���Ƴ��µ� Ȯ�� �ؾ���.
        if(_tRect.left   < 0 || _tRect.left   >= g_pImg[_iDst]->GetWidth () ||
           _tRect.top    < 0 || _tRect.top    >= g_pImg[_iDst]->GetHeight() ||
           _tRect.right  < 0 || _tRect.right  >= g_pImg[_iDst]->GetWidth () ||
           _tRect.bottom < 0 || _tRect.bottom >= g_pImg[_iDst]->GetHeight() ){
           LeaveCriticalSection(&m_hImageSection);
           Trace("_tRect.left  " , String(_tRect.left  ).c_str());
           Trace("_tRect.top   " , String(_tRect.top   ).c_str());
           Trace("_tRect.right " , String(_tRect.right ).c_str());
           Trace("_tRect.bottom" , String(_tRect.bottom).c_str());
           return false ;
        }

        int iOfsX , iOfsY ;
        for(int y = _tRect.top ; y < _tRect.Bottom ; y++){
            for(int x = _tRect.left ; x < _tRect.right ; x++){
                iOfsX = _iOfsX + x ;
                iOfsY = _iOfsY + y ;
                if(x     < 0 || x     >= g_pImg[_iSrc]->GetWidth ()) continue ;
                if(y     < 0 || y     >= g_pImg[_iSrc]->GetWidth ()) continue ;
                if(iOfsX < 0 || iOfsX >= g_pImg[_iDst]->GetWidth ()) continue ;
                if(iOfsY < 0 || iOfsY >= g_pImg[_iDst]->GetWidth ()) continue ;

                cPxSrc = g_pImg[_iSrc] ->GetPixel(x,y);
                cPxDst = g_pImg[_iDst] ->GetPixel(iOfsX,iOfsY);

                if(cPxDst < cPxSrc) g_pImg[_iDst]->SetPixel(iOfsX,iOfsY,cPxSrc );

            }
        }


        /*
        for(int y = _tRect.top ; y < _tRect.Bottom ; y++){
            for(int x = _tRect.left ; x < _tRect.right ; x++){
                cPxSrc = g_pImg[_iSrc] ->GetPixel(x,y);
                cPxDst = g_pImg[_iDst] ->GetPixel(x,y);

                if(cPxDst < cPxSrc) g_pImg[_iDst]->SetPixel(x,y,cPxSrc );

            }
        }*/
    LeaveCriticalSection(&m_hImageSection);
}


/*
void CVision::SaveImage()
{
    //�Űܴ�Ƴ��´�.   ������ ������ ��縮 �����.
    AnsiString sSaveFolderPath , sSavePath;

    sSaveFolderPath = AnsiString("d:\\Image\\"+m_tVisnStaticPara.sVisnName+"\\");

    if(!UserFile.DirExists(sSaveFolderPath)) UserFile.CreateDir(sSaveFolderPath) ;

    sSavePath = sSaveFolderPath + Now().FormatString("yyyymmdd_hhnnss_zzz");
    m_pImg -> SaveBmp(sSavePath.c_str());
}
*/

TLiveRslt  CVision::GetLiveRslt(/*double _dXOfs , double _dYOfs*/)
{
    /*
    TLiveRslt RetRslt ;

    RetRslt.Rslt.x = m_tLiveRslt.Rslt.x + _dXOfs ;
    RetRslt.Rslt.y = m_tLiveRslt.Rslt.y + _dYOfs ;

    RetRslt.RsltRect.left   = m_tLiveRslt.RsltRect.left    + _dXOfs ;
    RetRslt.RsltRect.top    = m_tLiveRslt.RsltRect.top     + _dYOfs ;
    RetRslt.RsltRect.right  = m_tLiveRslt.RsltRect.right   + _dXOfs ;
    RetRslt.RsltRect.bottom = m_tLiveRslt.RsltRect.bottom  + _dYOfs ;



    return RetRslt ;
    */
    return m_tLiveRslt ;
}






