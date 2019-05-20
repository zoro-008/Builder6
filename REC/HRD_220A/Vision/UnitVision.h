//---------------------------------------------------------------------------

#ifndef UnitVisionH
#define UnitVisionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
#include "PkgCamera.h"
#include "UnitTracker.h"

class CVision;
class CThreadInspect : public TThread
{            
    private:
        CVision * m_pVision ;
    protected:
        void __fastcall Execute();
    public:
        __fastcall CThreadInspect(CVision * _pVision, bool CreateSuspended) : TThread(CreateSuspended)
        {
            m_pVision = _pVision;
        }
};
//---------------------------------------------------------------------------


class CVision : public CBase
{
    friend class CThreadInspect;

    public : //초기화 함수.
        CVision(){ }
        virtual ~CVision(){}

        bool Init();
        bool Close();

    public : //struct
        /*
        enum EPkgKind {
            pkCamera_V01    = 0 ,
            pkMatch_V01     = 1 ,
            pkThreshold_V01 = 2 ,

            MAX_PKG_KIND
        };
        */

        struct TTime { //ms
            int iGrab ;
            int iInsp ;
            TTime(){
                iGrab = 0 ;
                iInsp = 0 ;
            }
        };

        struct TStat {
            bool bGrabbing ;
            bool bInspectionning ;
            TStat(){
                bGrabbing       = false ;
                bInspectionning = false ;
            }

        };

        struct TRqst {
            bool bReady;
            bool bGrab ;
            bool bInsp ;
            bool bGrabInsp ;

            bool bTrainInsp  ;
            int  iTrainInspNo; //특정검사만 하고 플때...

            TRqst(){
                bReady    =false ;
                bGrab     =false ;
                bInsp     =false ;
                bGrabInsp =false ;

                bTrainInsp=false ;
            }
        };

    protected : //Member
        TTime Time ;
        TStat Stat ;
        TRqst Rqst ;

        CPkgBase    * m_pActivePkg  ; //트레인시에 선택한.PKG
        void        * m_hSendMsgWnd ;
        //CPkgBase    * m_pCamPkg     ;
        CCamera_V01 * m_pCamPkg     ; //아.. 시발 이게 문제네...

    protected : //메세지 엑티브 창으로 보냄.
        void   SendPaintMsg(){
            if(m_hSendMsgWnd)PostMessage(m_hSendMsgWnd , WM_PAINT , 0 , 0 );
        }
//==============================================================================
    protected :
        //String m_asPkgNameTable[MAX_PKG_KIND];
        CLinkedList<CPkgBase *> m_lPkg ;
//        String                  m_sCrntJobFilePath ;
        CValueList *            m_pValueList ; //검사공용 결과값 리스트.
        CRITICAL_SECTION        m_csPkgList  ; //UI쓰레드랑 CThreadInspect 에서 동시 접근 하면 안됀다.


    public :
        //List 핸들링 관련.
        bool Add         (            String _sName    , String _sPkgName );
        bool Insert      (int _iIdx , String _sName    , String _sPkgName );
        bool MoveUp      (int _iIdx                                       );
        bool MoveDown    (int _iIdx                                       );
        bool Delete      (int _iIdx                                       );
        bool DeleteAll   (                                                );

        //
        CValueList              * GetValueList(        ){return m_pValueList;}
        CLinkedList<CPkgBase *> * GetList     (        ){return &m_lPkg;}

        void UpdateList(bool _bToTable , TStringGrid * _veGrid);
        void LoadSave  (bool _bLoad , String _sJobFileName);
        void LoadSave  (bool _bLoad );

        void SetGetPxResol(FGetPxResol _fpGetPxResol);
        void SetGetCalPos (FGetCalPos  _fpGetCalPos );

//==============================================================================


    protected : //쓰레드에서 플레그 확인 해서 동작 하는 함수..  쓰레드 관련.
        CThreadInspect * m_pThreadInspect ;
        void CheckReady();
        void CheckGrab();
        void CheckInsp();
        void CheckGrabInsp();
        void CheckTrainInsp(); //트레인창에서 쓴다.

    public : //m_pActivePkg이용 함수.
        void SetActivePkg(int _iNo){
            if(_iNo < 0 || _iNo >= m_lPkg.GetDataCnt()) {
                m_pActivePkg = NULL ;    //이렇게 예외처리 안ㅎ놓으면 PKG 하나도 없는 잡파일에서 2버째 로딩시 뻑남.
            }
            else m_pActivePkg = m_lPkg.GetData(_iNo) ;
        }
        CPkgBase    * GetCrntPkg(){return m_pActivePkg ;}
        CCamera_V01 * GetCamPkg (){return m_pCamPkg    ;}

        //화면 출력.
        void PaintRslt        (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        void PaintTracker     (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);

        //이미지 화면 클릭 이벤트
        bool MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        bool MouseMv(                    TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        void MouseUp(TMouseButton Button,TShiftState Shift );

        //파라미터 디스플레이
        void   UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara);
        void   UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara);
        void   UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara);


    public : //다른쓰레드에서는 이걸로 쓰자..  쓰레드 사용 함수들....
        void   SetSendMsgWnd(void * _hWnd);
        void * GetSendMsgWnd();
        void   PaintRsltAll    (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);


        CImage * GetCrntImg ();
        CImage * GetTrainImg();

        void Ready();
        void Grab();
        void Insp();
        void GrabInsp();

        void TrainInsp();  //트레인 창에서 쓰는 검사..

        bool GetGrabEnd();
        bool GetInspEnd();

        bool GetRslt();

        String GetRsltMsg();

        //이것은 크리티컬 섹션 있는 내부 함수에서 사용 하면 안됀다. 데드락 걸림.
        int  GetPkgCnt();

        TTime GetTimeInf(){return Time ;}

        int    m_iVisnID ; //트레이스 남기려고 임시로 만듬.
        String m_sPkgID  ; //이미지 저장용 아이디 세팅 이미지 저장시에 어떤 자제인지 구분이 안간다고 해서.

        //마지막 검사한 비젼 아이디.
        static int m_iLastInspId ;
};

#endif

