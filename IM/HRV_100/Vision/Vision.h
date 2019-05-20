//---------------------------------------------------------------------------

#ifndef VisionH
#define VisionH
//---------------------------------------------------------------------------
#include "RsltData.h"

#include "VisnDefine.h"



enum EN_DISP_OPTN {
    doGray   = 0  ,
    doColor       ,
    doFalse       ,
    doBinary      ,
    doRed         ,
    doGreen       ,
    doBlue        ,
    MAX_DISP_OPTN
};


//����Ʈ �ݹ�...
typedef void (__closure * PaintCallbackFunc)();

struct TLiveRslt{
    TPointData Rslt     ;
    TRect      RsltRect ;
};

//������ Ĩ �˻� ����.
class CVision//: public CVisionBase
{

    public :
        CVision();
        ~CVision();

        bool Init();
        void Close();

        void Grab      (int _iActiveStg);
        void Live      (bool _bOn, int _iActiveStg );
        bool GetGrabEnd(         );
        bool GetLive   (         );

        //void Inspection(EN_IMG_ID _iSttImage   ,
        //                EN_IMG_ID _iDriveImage ,
        //                EN_IMG_ID _iFullImage  ,
        //                EN_IMG_ID _iMaxImage   , TVisnRslt * _pVisnRslt);

        void LiveInspection(EN_IMG_ID _iInspImage , TLiveRslt * _tLiveRslt );

        void PaintCntLine(HDC _hDc , int _iWidth , int _iHeight , float _fOffsetX , float _fOffsetY , float _fScaleX , float _fScaleY);
        int  Paint(HDC _hDc , EN_DISP_OPTN _iDpOptn , EN_IMG_ID _iImage ,
                   int _iDstX, int _iDstY, int _iDstW, int _iDstH ,
                   int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH ,
                   float _fScaleX , float _fScaleY ,int _iThreshold , bool _bCntrLine , TVisnRslt & _pVisnRslt , TVisnRsltPnt & _pVisnRsltPnt , bool _bViewRslt);

        //��ȭ�鿡�� �ݹ� ���� ����Ʈ �Լ�.
        void SetPaintCallback(PaintCallbackFunc _pCalback);

        bool ImgCopy(EN_IMG_ID _iSrc , EN_IMG_ID _iDst);
        bool ImgRectMerge(EN_IMG_ID _iSrc , EN_IMG_ID _iDst , TRect _tRect , int _iOfsX = 0 , int _iOfsY = 0 );

        TLiveRslt GetLiveRslt();


    protected :
        bool m_bGrabbing;
        bool m_bLiveMode;

        int  m_iActiveStg ;

        TLiveRslt m_tLiveRslt ;



        PaintCallbackFunc m_pPaintCallback ;

        //�׷��ÿ� ī�޶� ������ִ� �ݹ�. ī�޶� �̹��� ������ ȣ���.
        void CamCallback (int _iCamNo);

        CRITICAL_SECTION m_hImageSection ;

       // bool ImgCopy(EN_IMG_ID _iSrc , EN_IMG_ID _iDst);

    public :
        int m_iDpRslt ;
};

extern CVision VSN;


#endif


