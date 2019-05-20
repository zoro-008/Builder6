//---------------------------------------------------------------------------

#ifndef VisionH
#define VisionH
//---------------------------------------------------------------------------
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "SAInterface.h"
#include "VisionBase.h"

class CWfrChip: public CVisionBase //조명 광량조절,카메라찍고,켈리브레이션
{

    public :
        CVisionBase();
        ~CVisionBase();

        bool Init(TVisnStaticPara _tPara);
        virtual void close();

    protected :
        //Form의 Paint 함수 호출 콜백.
        PaintCallbackFunc m_pPaintCallback ;

        bool m_bGrabbing ; //아직 사진 찍고 있는지.

        //라이브용 타이머.
        TTimer *m_tmLive;

        //카메라 그랩시에 밝기 관련 라이트,캠익스포져타임 파라.
        TCamLightPara   m_tCamLightPara ;

        //비젼에서 쓰이는 초기화때 세팅 하고 안바뀌는 정적 파라.
        TVisnStaticPara m_tVisnStaticPara ;

        //이미지 클라스.
        IImage * m_pImg ;

    public :
        //캠파라,라이트파라,   밝기세팅 해놓는 것.
        void SetCamLightPara(TCamLightPara _tCamLightPara);

        //Form의 Paint 함수 호출 콜백.
        void SetPaintCallback(PaintCallbackFunc _pCalback);

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

        //가상 함수... 상속시에 구현 해야함.
        bool Inspection       (){ShowMessage("This Vision dosen't have Inspection Function!"      );return false ;}
        bool Paint            (){ShowMessage("This Vision dosen't have Paint Function!"           );return false ;}

        bool OnMouseDw        (TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0){ShowMessage("This Vision dosen't have OnMouseDown Function!"     );return false ;}
        bool OnMouseMv        (){ShowMessage("This Vision dosen't have OnMouseMove Function!"     );return false ;}
        bool OnMouseUp        (){ShowMessage("This Vision dosen't have OnMouseUp Function!"       );return false ;}

        bool ShowAlgoPara     (){ShowMessage("This Vision dosen't have ShowAlgoPara Function!"    );return false ;}
        bool ShowCalPara      (){ShowMessage("This Vision dosen't have ShowCalPara Function!"     );return false ;}
        bool ShowCamLightPara (){ShowMessage("This Vision dosen't have ShowCamLightPara Function!");return false ;}
        bool ShowVisnPara     (){ShowMessage("This Vision dosen't have ShowVisnPara Function!"    );return false ;}


};





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
