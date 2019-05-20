//---------------------------------------------------------------------------

#ifndef PlaceH
#define PlaceH
//---------------------------------------------------------------------------
#include "SAInterface.h"
#include "VisionBase.h"
#include "Tracker.h"

//������ Ĩ �˻� ����.
class CPlace: public CVisionBase
{

    public :
        CPlace();
        ~CPlace();

        bool Init(TVisnStaticPara _tPara);
        virtual void Close();

    public :
        //���� �Լ�... ��ӽÿ� ���� �ؾ���.
        void Inspection      (         );
        void Paint           (HDC _hDc , EN_DISP_OPTN _iDpOptn ,
                                         int _iDstX, int _iDstY, int _iDstW, int _iDstH ,
                                         int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH ,
                                         float _fScaleX , float _fScaleY ,int _iThreshold , bool _bCntrLine);
        void SetPaintCallback(PaintCallbackFunc _pCalback);

        bool OnMouseDw       (TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY);
        bool OnMouseMv       (                     TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY);
        void OnMouseUp       (TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY);

        void ShowAlgoPara    (               TValueListEditor * _vePara = NULL);
        void UpdateAlgoPara  (bool _bTable , TValueListEditor * _vePara);
        void Load            (bool _bLoad                              );
        void ApplyDevOptn    (                                         );

    protected :
        EN_PAGE_ID m_iActivPage ;
        AnsiString m_sRsltMsg   ;

        void SaveImage();

    public :
        //���� �˻� ���� ã��.
        struct TPlcInsp {
            TRectTracker *  pTrckInsp1  ; //Ʈ���� Ʈ��Ŀ1
            TRectTracker *  pTrckInsp2  ; //Ʈ���� Ʈ��Ŀ2
            TSD_Rslt     *  pInspRslt   ; //�˻� ��� �迭...... tOcvIpRslt[Row][Col]
        }PlcInsp ;

        //��� ���� ���� �׳� �Լ�.
        void        SetActivPage (EN_PAGE_ID _iActivPage);
        EN_PAGE_ID  GetActivPage (                      );

        TRetResult  Rslt ;
};
#endif


