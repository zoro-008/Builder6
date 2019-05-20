//---------------------------------------------------------------------------

#ifndef VisionBaseH
#define VisionBaseH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <ValEdit.hpp>

//#include <ExtCtrls.hpp>
//#include <ComCtrls.hpp>


#include "SVInterface.h"
#include "Common.h"
struct TCamLightPara { //프로그램 켠상태에서 바뀔수 있고 파일세이브 하는 파라. Dynamic Para
    //camera Para
    int         iCntOffsetX  ;
    int         iCntOffsetY  ;

    //Cam Expos
    int         iCamExposure ;

    //Light Para
    int         iUseLightCh1 ;
    int         iLtBright1   ;
    int         iUseLightCh2 ;
    int         iLtBright2   ;
    int         iUseLightCh3 ;
    int         iLtBright3   ;
};

struct TVisnStaticPara { //처음에 코딩으로 세팅 하고 안바뀌는 파라. Static Para
    EN_VISN_ID  iVisnId      ; //비젼 아이디.
    AnsiString  sVisnName    ; //비젼 네임.
    EN_CAM_ID   iUseCamId    ; //사용 카메라 아이디.
    EN_LIGHT_ID iUseLightId  ; //사용 라이트 아이디.
};



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

class CVisionBase //조명 광량조절,카메라찍고,켈리브레이션
{

    public :
        CVisionBase();
        ~CVisionBase();

        virtual bool Init(TVisnStaticPara _tPara);
        virtual void Close();

        struct TTimeInfo {
            int iGrabStart ; //그랩 시작한 시간.
            int iGrab      ; //총 그랩에 소요된 시간.
            int iImgCpy    ; //카메라 이미지 내 이미지로 복사하는 시간.
            int iInsp      ; //검사 시간.
        };
    protected :
        TTimeInfo TimeInfo ;
        struct TStat {
            bool           bGrabbing ; //아직 사진 찍고 있는지.
            bool           bDispRslt ; //결과값을 디스플레이 할것 인지.
        }Stat ;

        //라이브용 타이머.
        TTimer *m_tmLive;

        //카메라 그랩시에 밝기 관련 라이트,캠익스포져타임 파라.
        TCamLightPara   m_tCamLightPara ;

        //비젼에서 쓰이는 초기화때 세팅 하고 안바뀌는 정적 파라.
        TVisnStaticPara m_tVisnStaticPara ;

        //이미지 클라스.
        IImage * m_pImg ;

        //검사안에서 Inspection의 종류가 복수일때 사용. ex) OCV , TSD ,
        EN_PAGE_ID m_iActivPage ;

        void PaintCntLine(HDC _hDc , int _iWidth , int _iHeight);
    public :
        //Form의 Paint 함수 호출 콜백.
        PaintCallbackFunc m_pPaintCallback ;

        IImage * GetImg() {return m_pImg;}
        TTimeInfo GetTimeInfo() {return TimeInfo ;}



        //라이브용 타이머.
        void __fastcall tmLiveTimer(TObject *Sender);

        //오프셑까지 적용된 카메라 센터.
        int  GetCenterX();
        int  GetCenterY();

        //비젼 관련.
        void SetLight  (); //Rs232로 비젼 컨트롤러에 세팅보내기... 그랩시에 하게 되면 좀 느려서 시퀜스에서 미리 보내 놔야 한다.
        void Grab      ();
        void Live      (bool _bOn);
        bool GetGrabEnd(         );

        //그랩시에 카메라에 등록해주는 콜백. 카메라에 이미지 들어오면 호출됨.
        void CamCallback (int _iCamNo);

        //이미지 관련.
        void LoadImg    (bool _bLoad);   //카메라 이미지 로드세이브

        //검사안에서 Inspection의 종류가 복수일때 사용. ex) OCV , TSD ,
        void        SetActivPage  (EN_PAGE_ID _iActivPage);
        EN_PAGE_ID  GetActivPage  (                      );

        //카메라 관련 파라미터.
        //void          ShowCamLightPara();
        TCamLightPara   GetCamLightPara ();
        void            SetCamLightPara (TCamLightPara _tCamLightPara);

        TVisnStaticPara GetVisnStaticPara();

        void            LoadCamLightPara(bool _bLoad);   //m_tCamLightPara 로드세이브

        void            InspectionThread();



        //상속시에 구현하고 먼져 호출한번 해줘야함.
        virtual void Paint         (HDC _hDc , EN_DISP_OPTN _iDpOptn ,
                                               int _iDstX, int _iDstY, int _iDstW, int _iDstH ,
                                               int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH ,
                                               float _fScaleX , float _fScaleY ,int _iThreshold , bool _bCntrLine);

        //Form의 Paint 함수 호출 콜백.
        virtual void SetPaintCallback(PaintCallbackFunc _pCalback);
        //가상 함수... 상속시에 구현 해야함.
        virtual void Inspection    (         ){ShowMessage("This Vision dosen't have Inspection Function!"      );return ;}

        virtual bool OnMouseDw     (TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY , float _fOffsetX, float _fOffsetY){ShowMessage("This Vision dosen't have OnMouseDw Function!");return false ;}
        virtual bool OnMouseMv     (                     TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY , float _fOffsetX, float _fOffsetY){ShowMessage("This Vision dosen't have OnMouseMv Function!");return false ;}
        virtual void OnMouseUp     (TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY , float _fOffsetX, float _fOffsetY){ShowMessage("This Vision dosen't have OnMouseUp Function!");              }

        virtual void ShowAlgoPara  (               TValueListEditor * _vePara){ShowMessage("This Vision dosen't have ShowAlgoPara   Function!"); }
        virtual void UpdateAlgoPara(bool _bTable , TValueListEditor * _vePara){ShowMessage("This Vision dosen't have UpdateAlgoPara Function!"); }
        virtual void Load          (bool _bLoad                              ){ShowMessage("This Vision dosen't have Load           Function!"); }
        virtual void ApplyDevOptn  (                                         ){ShowMessage("This Vision dosen't have ApplyDevOptn   Function!"); }
        virtual void Train         (                                         ){ShowMessage("This Vision dosen't have Train          Function!"); }
        virtual void TrackerInit   (                                         ){ShowMessage("This Vision dosen't have Tracker Init   Function!"); }
};

extern CVisionBase * VISN[MAX_VISN_ID] ;





/*
extern CVisionBase * VISN[MAX_VISN_ID];
CVisionBase * VISN[MAX_VISN_ID] ;
                //파일 관련.
        void LoadJobFile (bool _bLoad );

        void LoadCalPara (bool _bLoad );



        TFPoint    m_tRsltPnt ;
        void SetRsltPnt(TFPoint _tPnt) {m_tRsltPnt = _tPnt ; }

        //결과값 디스플레이.
        bool       m_bRlstOk  ;
        AnsiString m_sRsltMsg ;
        void SetRsltMsg(bool _bRslt , AnsiString _sMsg){m_bRlstOk = _bRslt ; m_sRsltMsg = _sMsg;}



        //파라 폼 관련.
        void MakeVisnParaFrm(TWinControl * _wcBase);
        void MoveVisnParaFrm(TWinControl * _wcBase);
        void SetArrange (TControl * _tcControl , int _iCol , int _iRow);






//파라 폼 관련.
void CVisionBase::MakeVisnParaFrm(TWinControl * _wcBase)
{
    ParaUi.pcParaBase = new TPageControl(_wcBase);
    ParaUi.pcParaBase -> Parent = _wcBase ;  //ParaUi.pcParaBase -> Visible = false ;
    ParaUi.pcParaBase -> TabPosition = tpRight ;
    ParaUi.pcParaBase -> Align = alClient ;

    ProcPkg.ShowParaForm(ParaUi.pcParaBase) ;

    ParaUi.pcParaBase -> OnChange = PageControlChange ;

}

void CVisionBase::SetArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    const int iControlWidth   = 70  ;
    const int iControlHeight  = 20  ;
    const int iStartRowOffset = 5   ;
    const int iStartColOffset = 5   ;

    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;

}

        //View 관련
        void SetViewMode(EN_VIEW_MODE _iMode);
        int  GetViewMode();
void CVisionBase::SetViewMode(EN_VIEW_MODE _iMode)
{
    //if(m_iViewMode == _iMode) return ;
    m_iViewMode = _iMode ;

    for(int i = 0 ; i < ProcPkg.m_iProcCnt ; i++) {
        ProcPkg.SetTrackerVisible(i , false);
    }
    Cal -> SetTrackerVisible(false);


    if(m_iViewMode ==vmNone){
    }

    else if(m_iViewMode ==vmCal ){
        Cal -> SetTrackerVisible(true);
    }

    else if(m_iViewMode == vmAll ){
    }
    else if(m_iViewMode == vmRslt){
        ProcPkg.SetTrackerVisible(ParaUi.pcParaBase -> ActivePageIndex , true);
    }


}

int CVisionBase::GetViewMode()
{
    return m_iViewMode ;

}

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




bool Inspect   ();
bool Train     ();
bool CVisionBase::Inspect()
{
    int iSel = ParaUi.pcParaBase -> ActivePageIndex ;
    //iSel = iSel -1 ; //카메라 폼은 출력 안하기 때문에.
    if(iSel < 0) return true ;
    bool bRet ;

    EDG_Rslt EdgRslt ;
    LIN_Rslt LinRslt ;
    LCP_Rslt LcpRslt ;
    RCT_Rslt RctRslt ;
    TSD_Rslt TsdRslt ;
    BLB_Rslt BlbRslt ;
    CAL_Rslt CalRslt ;
    CCL_Rslt CclRslt ;
    OCV_Rslt OcvRslt ;
    WRT_Rslt WrtRslt ;

    switch(ProcPkg.GetProcType(iSel)) {
        default    : bRet = false ;                                 break ;
        case ptEdg : bRet = ProcPkg.Process(iSel , m_pImg , &EdgRslt); break ;
        case ptLin : bRet = ProcPkg.Process(iSel , m_pImg , &LinRslt); break ;
        case ptLcp : bRet = ProcPkg.Process(iSel , m_pImg , &LcpRslt); break ;
        case ptRct : bRet = ProcPkg.Process(iSel , m_pImg , &RctRslt); break ;
        case ptTsd : bRet = ProcPkg.Process(iSel , m_pImg , &TsdRslt); break ;
        case ptBlb : bRet = ProcPkg.Process(iSel , m_pImg , &BlbRslt); break ;
        case ptCal : bRet = ProcPkg.Process(iSel , m_pImg , &CalRslt); break ;
        case ptCcl : bRet = ProcPkg.Process(iSel , m_pImg , &CclRslt); break ;
        case ptOcv : bRet = ProcPkg.Process(iSel , m_pImg , &OcvRslt); break ;
        case ptWrt : bRet = ProcPkg.Process(iSel , m_pImg , &WrtRslt); break ;
    }

    return bRet ;
}

bool CVisionBase::Train()
{
    int iSel = ParaUi.pcParaBase -> ActivePageIndex ;
    if(iSel < 0) return true ;
    bool bRet ;

    switch(ProcPkg.GetProcType(iSel)) {
        default    : ShowMessage("티칭이 필요없습니다.!"); bRet = false ; break ;
        case ptOcv : bRet = ProcPkg.Train(iSel , m_pImg );                   break ;
    }

    return bRet ;
}



*/

#endif
