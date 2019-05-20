//---------------------------------------------------------------------------

#ifndef PlaceH
#define PlaceH
//---------------------------------------------------------------------------
#include "SAInterface.h"
#include "VisionBase.h"
#include "Tracker.h"

//웨이퍼 칩 검사 비젼.
class CPlace: public CVisionBase
{

    public :
        CPlace();
        ~CPlace();

        bool Init(TVisnStaticPara _tPara);
        virtual void Close();

    public :
        //가상 함수... 상속시에 구현 해야함.
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
        //자제 검사 기준 찾기.
        struct TPlcInsp {
            TRectTracker *  pTrckInsp1  ; //트레인 트렉커1
            TRectTracker *  pTrckInsp2  ; //트레인 트렉커2
            TSD_Rslt     *  pInspRslt   ; //검사 결과 배열...... tOcvIpRslt[Row][Col]
        }PlcInsp ;

        //상속 하지 않은 그냥 함수.
        void        SetActivPage (EN_PAGE_ID _iActivPage);
        EN_PAGE_ID  GetActivPage (                      );

        TRetResult  Rslt ;
};
#endif


