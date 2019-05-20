//---------------------------------------------------------------------------

#ifndef VisionH
#define VisionH
//---------------------------------------------------------------------------
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "CamUnit.h"
#include "LightUnit.h"
#include "SAInterface.h"
#include "ProcUnit.h"
#include "VisnDefine.h"

typedef void (__closure *Func)();
template <typename T>
class CRunThread
{
    class TRunThread : public TThread {
        private:
            T Func;
        public:
            __fastcall TRunThread(T func) : TThread(false) {
                FreeOnTerminate = true;
                Func = func;
            }
            void __fastcall Execute() {
                Func();
            }
    };

    public:
        CRunThread(T func) {
            TRunThread*  thread = new TRunThread(func);
        }
};


typedef void (__closure * PaintCallbackFunc)();
class CVision
{

    public :
        CVision();
        ~CVision();

    protected :

        EN_VIEW_MODE m_iViewMode ;

        AnsiString m_sLastJobPath ;

    public :
        void __fastcall PageControlChange(TObject *Sender);
        struct TParaUi {
            TPageControl *  pcParaBase ;
        }ParaUi ;


        void Calibration();

        struct TPara {
            EN_CAM_ID   iUseCamNo     ;
            int         iCamExposure  ;
            int         iCntOffsetX   ;
            int         iCntOffsetY   ;

            EN_LIGHT_ID iUseLightNo  ;
            int         iUseLightCh1 ;
            int         iLtBright1   ;
            int         iUseLightCh2 ;
            int         iLtBright2   ;
            int         iUseLightCh3 ;
            int         iLtBright3   ;

        }Para ;

        bool Init(AnsiString _sVisnName);
        void close();


        AnsiString m_sVisnName ;


        //�̹��� Ŭ����
        IImage * Img ;

        //Calibration
        CCal * Cal ;

        //����� ���÷���.
        bool       m_bRlstOk  ;
        AnsiString m_sRsltMsg ;
        void SetRsltMsg(bool _bRslt , AnsiString _sMsg){m_bRlstOk = _bRslt ; m_sRsltMsg = _sMsg;}

        TFPoint    m_tRsltPnt ;
        void SetRsltPnt(TFPoint _tPnt) {m_tRsltPnt = _tPnt ; }


        //Image Processing �����.
        CProcPkg ProcPkg ;

        //Form�� �Լ� ȣ�� �ݹ�.
        PaintCallbackFunc   PaintCallback ;
        void SetPaintCallback(PaintCallbackFunc _pCalback);

        //���̺�� Ÿ�̸�.
        TTimer *tmLive;
        void __fastcall tmLiveTimer(TObject *Sender);

        //ƼĪ�ν���ǿ� Ÿ�̸�.


        //ī�޶� ������ִ� �ݹ�.
        void CamCallback (int _iCamNo);

        //�Ķ� �� ����.
        void MakeVisnParaFrm(TWinControl * _wcBase);
        void MoveVisnParaFrm(TWinControl * _wcBase);
        void SetArrange (TControl * _tcControl , int _iCol , int _iRow);

        //View ����
        void SetViewMode(EN_VIEW_MODE _iMode);
        int  GetViewMode();

        //�����V���� ����� ����.
        int  GetCenterX();
        int  GetCenterY();


        //���� ����.
        void LoadJobFile (bool _bLoad );
        void LoadCamPara (bool _bLoad );
        void LoadCalPara (bool _bLoad );

        //���� ����.
        bool m_bGrabbing ; //���� ���� ��� �ִ���.
        void SetLight  ();
        void Grab      ();
        void Live      (bool _bOn);
        bool GetGrabEnd(         );
        bool Inspect   ();
        bool Train     ();

        //Ʈ��Ŀ ����.
        bool MouseTrackerDn (TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        bool MouseTrackerMv (                     TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void MouseTrackerUp (TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);

        //�̹��� �Ѹ���.
        void PaintImage     (HDC _hHdc , float _fScaleX , float _fScaleY);

        void PaintTracker   (HDC _hHdc , float _fScaleX , float _fScaleY);

        //��������.
        void PaintRslt (HDC  _hHdc , float _fScaleX , float _fScaleY);

        //���� ����
        void PaintCntLine (HDC _hHdc , int _iWidth , int _iHeight);

        //�߸� ����.
        int m_iYameRet ;
};

extern CVision * VISN[MAX_VISN_ID];

#endif
