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


        //이미지 클레스
        IImage * Img ;

        //Calibration
        CCal * Cal ;

        //결과값 디스플레이.
        bool       m_bRlstOk  ;
        AnsiString m_sRsltMsg ;
        void SetRsltMsg(bool _bRslt , AnsiString _sMsg){m_bRlstOk = _bRslt ; m_sRsltMsg = _sMsg;}

        TFPoint    m_tRsltPnt ;
        void SetRsltPnt(TFPoint _tPnt) {m_tRsltPnt = _tPnt ; }


        //Image Processing 만들기.
        CProcPkg ProcPkg ;

        //Form의 함수 호출 콜백.
        PaintCallbackFunc   PaintCallback ;
        void SetPaintCallback(PaintCallbackFunc _pCalback);

        //라이브용 타이머.
        TTimer *tmLive;
        void __fastcall tmLiveTimer(TObject *Sender);

        //티칭인스펙션용 타이머.


        //카메라에 등록해주는 콜백.
        void CamCallback (int _iCamNo);

        //파라 폼 관련.
        void MakeVisnParaFrm(TWinControl * _wcBase);
        void MoveVisnParaFrm(TWinControl * _wcBase);
        void SetArrange (TControl * _tcControl , int _iCol , int _iRow);

        //View 관련
        void SetViewMode(EN_VIEW_MODE _iMode);
        int  GetViewMode();

        //오프셑까지 적용된 센터.
        int  GetCenterX();
        int  GetCenterY();


        //파일 관련.
        void LoadJobFile (bool _bLoad );
        void LoadCamPara (bool _bLoad );
        void LoadCalPara (bool _bLoad );

        //비젼 관련.
        bool m_bGrabbing ; //아직 사진 찍고 있는지.
        void SetLight  ();
        void Grab      ();
        void Live      (bool _bOn);
        bool GetGrabEnd(         );
        bool Inspect   ();
        bool Train     ();

        //트레커 관련.
        bool MouseTrackerDn (TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        bool MouseTrackerMv (                     TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void MouseTrackerUp (TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);

        //이미지 뿌리기.
        void PaintImage     (HDC _hHdc , float _fScaleX , float _fScaleY);

        void PaintTracker   (HDC _hHdc , float _fScaleX , float _fScaleY);

        //결과값출력.
        void PaintRslt (HDC  _hHdc , float _fScaleX , float _fScaleY);

        //센터 라인
        void PaintCntLine (HDC _hHdc , int _iWidth , int _iHeight);

        //야매 리턴.
        int m_iYameRet ;
};

extern CVision * VISN[MAX_VISN_ID];

#endif
