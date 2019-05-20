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
struct TCamLightPara { //���α׷� �һ��¿��� �ٲ�� �ְ� ���ϼ��̺� �ϴ� �Ķ�. Dynamic Para
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

struct TVisnStaticPara { //ó���� �ڵ����� ���� �ϰ� �ȹٲ�� �Ķ�. Static Para
    EN_VISN_ID  iVisnId      ; //���� ���̵�.
    AnsiString  sVisnName    ; //���� ����.
    EN_CAM_ID   iUseCamId    ; //��� ī�޶� ���̵�.
    EN_LIGHT_ID iUseLightId  ; //��� ����Ʈ ���̵�.
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

class CVisionBase //���� ��������,ī�޶����,�̸��극�̼�
{

    public :
        CVisionBase();
        ~CVisionBase();

        virtual bool Init(TVisnStaticPara _tPara);
        virtual void Close();

        struct TTimeInfo {
            int iGrabStart ; //�׷� ������ �ð�.
            int iGrab      ; //�� �׷��� �ҿ�� �ð�.
            int iImgCpy    ; //ī�޶� �̹��� �� �̹����� �����ϴ� �ð�.
            int iInsp      ; //�˻� �ð�.
        };
    protected :
        TTimeInfo TimeInfo ;
        struct TStat {
            bool           bGrabbing ; //���� ���� ��� �ִ���.
            bool           bDispRslt ; //������� ���÷��� �Ұ� ����.
        }Stat ;

        //���̺�� Ÿ�̸�.
        TTimer *m_tmLive;

        //ī�޶� �׷��ÿ� ��� ���� ����Ʈ,ķ�ͽ�����Ÿ�� �Ķ�.
        TCamLightPara   m_tCamLightPara ;

        //�������� ���̴� �ʱ�ȭ�� ���� �ϰ� �ȹٲ�� ���� �Ķ�.
        TVisnStaticPara m_tVisnStaticPara ;

        //�̹��� Ŭ��.
        IImage * m_pImg ;

        //�˻�ȿ��� Inspection�� ������ �����϶� ���. ex) OCV , TSD ,
        EN_PAGE_ID m_iActivPage ;

        void PaintCntLine(HDC _hDc , int _iWidth , int _iHeight);
    public :
        //Form�� Paint �Լ� ȣ�� �ݹ�.
        PaintCallbackFunc m_pPaintCallback ;

        IImage * GetImg() {return m_pImg;}
        TTimeInfo GetTimeInfo() {return TimeInfo ;}



        //���̺�� Ÿ�̸�.
        void __fastcall tmLiveTimer(TObject *Sender);

        //�����V���� ����� ī�޶� ����.
        int  GetCenterX();
        int  GetCenterY();

        //���� ����.
        void SetLight  (); //Rs232�� ���� ��Ʈ�ѷ��� ���ú�����... �׷��ÿ� �ϰ� �Ǹ� �� ������ �ó������� �̸� ���� ���� �Ѵ�.
        void Grab      ();
        void Live      (bool _bOn);
        bool GetGrabEnd(         );

        //�׷��ÿ� ī�޶� ������ִ� �ݹ�. ī�޶� �̹��� ������ ȣ���.
        void CamCallback (int _iCamNo);

        //�̹��� ����.
        void LoadImg    (bool _bLoad);   //ī�޶� �̹��� �ε弼�̺�

        //�˻�ȿ��� Inspection�� ������ �����϶� ���. ex) OCV , TSD ,
        void        SetActivPage  (EN_PAGE_ID _iActivPage);
        EN_PAGE_ID  GetActivPage  (                      );

        //ī�޶� ���� �Ķ����.
        //void          ShowCamLightPara();
        TCamLightPara   GetCamLightPara ();
        void            SetCamLightPara (TCamLightPara _tCamLightPara);

        TVisnStaticPara GetVisnStaticPara();

        void            LoadCamLightPara(bool _bLoad);   //m_tCamLightPara �ε弼�̺�

        void            InspectionThread();



        //��ӽÿ� �����ϰ� ���� ȣ���ѹ� �������.
        virtual void Paint         (HDC _hDc , EN_DISP_OPTN _iDpOptn ,
                                               int _iDstX, int _iDstY, int _iDstW, int _iDstH ,
                                               int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH ,
                                               float _fScaleX , float _fScaleY ,int _iThreshold , bool _bCntrLine);

        //Form�� Paint �Լ� ȣ�� �ݹ�.
        virtual void SetPaintCallback(PaintCallbackFunc _pCalback);
        //���� �Լ�... ��ӽÿ� ���� �ؾ���.
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
                //���� ����.
        void LoadJobFile (bool _bLoad );

        void LoadCalPara (bool _bLoad );



        TFPoint    m_tRsltPnt ;
        void SetRsltPnt(TFPoint _tPnt) {m_tRsltPnt = _tPnt ; }

        //����� ���÷���.
        bool       m_bRlstOk  ;
        AnsiString m_sRsltMsg ;
        void SetRsltMsg(bool _bRslt , AnsiString _sMsg){m_bRlstOk = _bRslt ; m_sRsltMsg = _sMsg;}



        //�Ķ� �� ����.
        void MakeVisnParaFrm(TWinControl * _wcBase);
        void MoveVisnParaFrm(TWinControl * _wcBase);
        void SetArrange (TControl * _tcControl , int _iCol , int _iRow);






//�Ķ� �� ����.
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

        //View ����
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




bool Inspect   ();
bool Train     ();
bool CVisionBase::Inspect()
{
    int iSel = ParaUi.pcParaBase -> ActivePageIndex ;
    //iSel = iSel -1 ; //ī�޶� ���� ��� ���ϱ� ������.
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
        default    : ShowMessage("ƼĪ�� �ʿ�����ϴ�.!"); bRet = false ; break ;
        case ptOcv : bRet = ProcPkg.Train(iSel , m_pImg );                   break ;
    }

    return bRet ;
}



*/

#endif
