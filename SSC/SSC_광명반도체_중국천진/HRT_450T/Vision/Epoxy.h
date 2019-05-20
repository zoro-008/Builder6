//---------------------------------------------------------------------------

#ifndef EpoxyH
#define EpoxyH
//---------------------------------------------------------------------------
#include "SAInterface.h"
#include "VisionBase.h"
#include "Tracker.h"

//웨이퍼 칩 검사 비젼.
class CEpoxy: public CVisionBase
{

    public :
        CEpoxy();
        ~CEpoxy();

        bool Init(TVisnStaticPara _tPara);
        virtual void Close();

    public :
        //가상 함수... 상속시에 구현 해야함.
        void Inspection      (         );
        void Train           (         );
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
        void ApplyDevOptn    (                                         );//DevOptn중에 바뀐 놈들의 크기와 갯수를 바꾼다.


    protected :

        AnsiString m_sRsltMsg   ;

        void SaveImage();

    public :
        //자제 검사 기준 찾기.
        struct TOrig {
            TOutBandRectTracker *  pTrckInsp ; //기준점 Ocv
            IArea               *  pTrArea   ;
            IArea               *  pRsltArea ;
            IImage              *  pTrImage  ;
            OCV_Rslt               tRslt     ; //검사 결과.
        }Orig ;

        //실제 검사.
        struct TInsp {
            TRectTracker *  pTrckInsp1  ; //트레인 트렉커1
            TRectTracker *  pTrckInsp2  ; //트레인 트렉커2
            TSD_Rslt     *  pRslt       ; //검사 결과 배열.  (가로 세로 갯수 곱하기.
        }Insp ;



        //상속 하지 않은 그냥 함수.
        TRetResult  Rslt ;



};
#endif


