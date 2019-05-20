//---------------------------------------------------------------------------

#ifndef EpoxyH
#define EpoxyH
//---------------------------------------------------------------------------
#include "SAInterface.h"
#include "VisionBase.h"
#include "Tracker.h"

//������ Ĩ �˻� ����.
class CEpoxy: public CVisionBase
{

    public :
        CEpoxy();
        ~CEpoxy();

        bool Init(TVisnStaticPara _tPara);
        virtual void Close();

    public :
        //���� �Լ�... ��ӽÿ� ���� �ؾ���.
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
        void ApplyDevOptn    (                                         );//DevOptn�߿� �ٲ� ����� ũ��� ������ �ٲ۴�.


    protected :

        AnsiString m_sRsltMsg   ;

        void SaveImage();

    public :
        //���� �˻� ���� ã��.
        struct TOrig {
            TOutBandRectTracker *  pTrckInsp ; //������ Ocv
            IArea               *  pTrArea   ;
            IArea               *  pRsltArea ;
            IImage              *  pTrImage  ;
            OCV_Rslt               tRslt     ; //�˻� ���.
        }Orig ;

        //���� �˻�.
        struct TInsp {
            TRectTracker *  pTrckInsp1  ; //Ʈ���� Ʈ��Ŀ1
            TRectTracker *  pTrckInsp2  ; //Ʈ���� Ʈ��Ŀ2
            TSD_Rslt     *  pRslt       ; //�˻� ��� �迭.  (���� ���� ���� ���ϱ�.
        }Insp ;



        //��� ���� ���� �׳� �Լ�.
        TRetResult  Rslt ;



};
#endif

