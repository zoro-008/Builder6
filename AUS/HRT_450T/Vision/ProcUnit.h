//---------------------------------------------------------------------------

#ifndef ProcUnitH
#define ProcUnitH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <ComCtrls.hpp>

#include "SVInterface.h"
#include "Tracker.h"

enum EN_PROC_TYPE {
    ptInp =-1 ,
    ptEdg = 0 ,
    ptLin     ,
    ptLcp     ,
    ptRct     ,
    ptTsd     ,
    ptBlb     ,
    ptCal     ,
    ptCcl     ,
    ptOcv     ,
    ptWrt     ,
};

class CProc
{
    public :
        CProc(){
            m_iProcType = ptInp ;
            m_fRefValX  = 0.0 ;
            m_fRefValY  = 0.0 ;
            m_fRefValT  = 0.0 ;

            m_fInspOfsX = 0.0 ;
            m_fInspOfsY = 0.0 ;

        }
        ~CProc(){}

    protected :
        EN_PROC_TYPE m_iProcType ;

        float m_fInspOfsX ; //전에 검사의 파라저장시에 값과 결과값의 오프셑. 검사시에 검사영역에 더해서 영역설정.
        float m_fInspOfsY ; //전에 검사의 파라저장시에 값과 결과값의 오프셑. 검사시에 검사영역에 더해서 영역설정.
        //float m_fInspOfsT ; //전에 검사의 파라저장시에 값과 결과값의 오프셑. 검사시에 검사영역에 더해서 영역설정. 일단 Theta는 안씀.

        TRect   GetOfsRect  (TRect   _tRect);
        TPoint  GetOfsPoint (TPoint  _tPnt );
        TCircle GetOfsCircle(TCircle _tCcl );

        void PaintOfsRect   (HDC _hHdc , float _fScaleX , float _fScaleY , TRect   _tRect);
        void PaintOfsLine   (HDC _hHdc , float _fScaleX , float _fScaleY , TPoint  _tSttPnt , TPoint _tEndPnt);
        void PaintOfsCircle (HDC _hHdc , float _fScaleX , float _fScaleY , TCircle _tCircle);


        float m_fRefValX ; //현재 검사의 파라저장시에 결과값으로 기준이 되어
        float m_fRefValY ; //추후에 GetRefValOfs()을 통하여 이번검사와 파라저장시의 값의 차를 받아옴.
        float m_fRefValT ; //CLcp 와 CRct 와 CCcl 과 COcv만 활용 가능 하다.

    public :
        EN_PROC_TYPE GetProcType () {return m_iProcType ;}

        void SetInspOfs(float _fX , float _fY ) {m_fInspOfsX = _fX ; m_fInspOfsY = _fY ; /*m_fInspOfsT = _fT ;*/ };

        void SetRefVal(float  _fX , float  _fY , float  _fT ) {m_fRefValX = _fX ; m_fRefValY = _fY ; m_fRefValT = _fT ; }
        void ClrRefVal(                                     ) {SetRefVal(0,0,0);                                        }
        void GetRefVal(float &_fX , float &_fY , float &_fT ) {_fX = m_fRefValX ; _fY = m_fRefValY ; _fT = m_fRefValT ; }

        virtual void SetPaintCallback(TOnChanged ChangeFunc)                                                               {ShowMessage("Inspection dosen't have this Function!"); return      ;}
        virtual void SetTrackerVisible(bool _bVisible)                                                                     {ShowMessage("Inspection dosen't have this Function!"); return      ;}
        virtual void SetTrackerEnabled(bool _bEnabled)                                                                     {ShowMessage("Inspection dosen't have this Function!"); return      ;}
        virtual bool MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY){ShowMessage("Inspection dosen't have this Function!"); return false;}
        virtual bool MouseTrackerMv(                     TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY){ShowMessage("Inspection dosen't have this Function!"); return false;}
        virtual void MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY){ShowMessage("Inspection dosen't have this Function!"); return      ;}
        virtual void PaintTracker  (HDC _hHdc                                            , float _fScaleX , float _fScaleY){ShowMessage("Inspection dosen't have this Function!"); return      ;}

        virtual void ShowParaForm(TWinControl * _wcBase                                 ) {ShowMessage("Inspection dosen't have this Function!"); return ;}
        virtual void LoadPara    (bool _bLoad , AnsiString _sFilePath , AnsiString _sId ) {ShowMessage("Inspection dosen't have this Function!"); return ;}
        virtual void ApplyPara   (bool _bTable                                          ) {ShowMessage("Inspection dosen't have this Function!"); return ;}
        virtual void PaintRslt   (IImage * _pImg        , HDC _hHdc                     ) {ShowMessage("Inspection dosen't have this Function!"); return ;}
        virtual bool Train       (IImage * _pImg                                        ) {ShowMessage("Inspection dosen't have this Function!"); return false ;}


        virtual void GetRefValOfs(float &_fX , float &_fY , float &_fT    ) {ShowMessage("Inspection dosen't have this Function!"); return ;}
};

class CEdg : public CProc
{
    public :
        CEdg();
        ~CEdg();

        struct TParaUi {
            TPanel * pnBase ;
            TLabel * lbAverage   ; TEdit * edAverage   ;
            TLabel * lbCompare   ; TEdit * edCompare   ;
            TLabel * lbChkPxLow  ; TEdit * edChkPxLow  ;
            TLabel * lbChkPxHgh  ; TEdit * edChkPxHgh  ;
            TLabel * lbOfsGap    ; TEdit * edOfsGap    ;
            TLabel * lbOfsPxLow  ; TEdit * edOfsPxLow  ;
            TLabel * lbOfsPxHgh  ; TEdit * edOfsPxHgh  ;
        };
        void MakeUI     (TParaUi  * _pParaUi , TWinControl * _tcControl);
        void SetArrange (TControl * _tcControl , int _iCol , int _iRow);

    protected :
        TParaUi ParaUi ;
    public :

        EDG_Para Para ;
        EDG_Rslt Rslt ;
        EDG_Disp Disp ;

        TLineTracker * Tracker ;

        void SetPaintCallback(TOnChanged ChangeFunc);
        void SetTrackerVisible(bool _bVisible);
        void SetTrackerEnabled(bool _bEnabled);
        bool MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        bool MouseTrackerMv(                     TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void PaintTracker  (HDC _hHdc                                            , float _fScaleX , float _fScaleY);

        void ShowParaForm(TWinControl   * _wcBase   ) ;
        void LoadPara    (bool _bLoad , AnsiString _sFilePath , AnsiString _sId) ;
        void ApplyPara   (bool _bTable                                         ) ;

        bool Process     (IImage * _pImg , EDG_Rslt * _pRslt);
        void PaintRslt   (IImage * _pImg , HDC _hHdc);

};

class CLin : public CProc
{
    public :
        CLin();
        ~CLin();

        struct TParaUi {
            TPanel * pnBase ;
            TLabel * lbScanDirect  ; TComboBox * cbScanDirect ;
                                     TCheckBox * cbLtToDk     ;
            TLabel * lbSampleGap   ; TEdit     * edSampleGap  ;
            TLabel * lbLineGap     ; TEdit     * edLineGap    ;
            TLabel * lbEndOffset   ; TEdit     * edEndOffset  ;
            TLabel * lbCntrOffset  ; TEdit     * edCntrOffset ;

            //Edge.
            TLabel * lbEdgAverage  ; TEdit     * edEdgAverage  ;
            TLabel * lbEdgCompare  ; TEdit     * edEdgCompare  ;
            TLabel * lbEdgChkPxLow ; TEdit     * edEdgChkPxLow ;
            TLabel * lbEdgChkPxHgh ; TEdit     * edEdgChkPxHgh ;
            TLabel * lbEdgOfsGap   ; TEdit     * edEdgOfsGap   ;
            TLabel * lbEdgOfsPxLow ; TEdit     * edEdgOfsPxLow ;
            TLabel * lbEdgOfsPxHgh ; TEdit     * edEdgOfsPxHgh ;
        };

        void MakeUI     (TParaUi  * _pParaUi , TWinControl * _tcControl);
        void SetArrange (TControl * _tcControl , int _iCol , int _iRow);
    protected :
        TParaUi ParaUi  ;

    public :
        LIN_Para Para ;
        LIN_Rslt Rslt ;
        LIN_Disp Disp ;

        TRectTracker *Tracker ;
        void SetPaintCallback(TOnChanged ChangeFunc);
        void SetTrackerVisible(bool _bVisible);
        void SetTrackerEnabled(bool _bEnabled);
        bool MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        bool MouseTrackerMv(                     TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void PaintTracker  (HDC _hHdc                                            , float _fScaleX , float _fScaleY);

        void ShowParaForm(TWinControl * _wcBase ) ;
        void LoadPara    (bool _bLoad , AnsiString _sFilePath , AnsiString _sId) ;
        void ApplyPara   (bool _bTable                                         ) ;

        bool Process     (IImage * _pImg , LIN_Rslt * _pRslt);
        void PaintRslt   (IImage * _pImg , HDC _hHdc);

};

class CLcp : public CProc
{
    public :
        CLcp();
        ~CLcp();

        struct TParaUi {
            TPanel * pnBase ;
            //Line1
            TLabel * lbScanDirect1  ; TComboBox * cbScanDirect1 ;
                                      TCheckBox * cbLtToDk1     ;
            TLabel * lbSampleGap1   ; TEdit     * edSampleGap1  ;
            TLabel * lbLineGap1     ; TEdit     * edLineGap1    ;
            TLabel * lbEndOffset1   ; TEdit     * edEndOffset1  ;
            TLabel * lbCntrOffset1  ; TEdit     * edCntrOffset1 ;

            //Edge.
            TLabel * lbEdgAverage1  ; TEdit     * edEdgAverage1  ;
            TLabel * lbEdgCompare1  ; TEdit     * edEdgCompare1  ;
            TLabel * lbEdgChkPxLow1 ; TEdit     * edEdgChkPxLow1 ;
            TLabel * lbEdgChkPxHgh1 ; TEdit     * edEdgChkPxHgh1 ;
            TLabel * lbEdgOfsGap1   ; TEdit     * edEdgOfsGap1   ;
            TLabel * lbEdgOfsPxLow1 ; TEdit     * edEdgOfsPxLow1 ;
            TLabel * lbEdgOfsPxHgh1 ; TEdit     * edEdgOfsPxHgh1 ;

            //Line2
            TLabel * lbScanDirect2  ; TComboBox * cbScanDirect2 ;
                                      TCheckBox * cbLtToDk2     ;
            TLabel * lbSampleGap2   ; TEdit     * edSampleGap2  ;
            TLabel * lbLineGap2     ; TEdit     * edLineGap2    ;
            TLabel * lbEndOffset2   ; TEdit     * edEndOffset2  ;
            TLabel * lbCntrOffset2  ; TEdit     * edCntrOffset2 ;

            //Edge.
            TLabel * lbEdgAverage2  ; TEdit     * edEdgAverage2  ;
            TLabel * lbEdgCompare2  ; TEdit     * edEdgCompare2  ;
            TLabel * lbEdgChkPxLow2 ; TEdit     * edEdgChkPxLow2 ;
            TLabel * lbEdgChkPxHgh2 ; TEdit     * edEdgChkPxHgh2 ;
            TLabel * lbEdgOfsGap2   ; TEdit     * edEdgOfsGap2   ;
            TLabel * lbEdgOfsPxLow2 ; TEdit     * edEdgOfsPxLow2 ;
            TLabel * lbEdgOfsPxHgh2 ; TEdit     * edEdgOfsPxHgh2 ;
        };

        void MakeUI     (TParaUi  * _pParaUi , TWinControl * _tcControl);
        void SetArrange (TControl * _tcControl , int _iCol , int _iRow);
    protected :
        TParaUi ParaUi  ;

    public :
        LCP_Para Para ;
        LCP_Rslt Rslt ;
        LCP_Disp Disp ;

        TRectTracker *Tracker1 ;
        TRectTracker *Tracker2 ;
        void SetPaintCallback(TOnChanged ChangeFunc);
        void SetTrackerVisible(bool _bVisible);
        void SetTrackerEnabled(bool _bEnabled);
        bool MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        bool MouseTrackerMv(                     TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void PaintTracker  (HDC _hHdc                                            , float _fScaleX , float _fScaleY);

        void ShowParaForm(TWinControl * _wcBase ) ;
        void LoadPara    (bool _bLoad , AnsiString _sFilePath , AnsiString _sId) ;
        void ApplyPara   (bool _bTable                                         ) ;

        bool Process     (IImage * _pImg , LCP_Rslt * _pRslt);
        void PaintRslt   (IImage * _pImg , HDC _hHdc);

        void GetRefValOfs(float &_fX , float &_fY , float &_fT );
};



class CRct : public CProc
{
    public :
        CRct();
        ~CRct();

        struct TParaUi {
            TPanel * pnBase ;

            TCheckBox * cbLtOnDk     ;
            TLabel    * lbSampleGap  ; TEdit * edSampleGap  ;
            TLabel    * lbLineGap    ; TEdit * edLineGap    ;

            TLabel    * lbChkOfs     ; TEdit * edChkOfs     ;
            TLabel    * lbLowThsd    ; TEdit * edLowThsd    ;
            TLabel    * lbHighThsd   ; TEdit * edHighThsd   ;

            TLabel    * lbAverage    ; TEdit * edAverage    ;
            TLabel    * lbCompare    ; TEdit * edCompare    ;
            TLabel    * lbChkPxLow   ; TEdit * edChkPxLow   ;
            TLabel    * lbChkPxHgh   ; TEdit * edChkPxHgh   ;

            TLabel    * lbEndOffsetL ; TEdit * edEndOffsetL ;
            TLabel    * lbCntrOffsetL; TEdit * edCntrOffsetL;
            TLabel    * lbOfsGapL    ; TEdit * edOfsGapL    ;
            TLabel    * lbOfsPxLowL  ; TEdit * edOfsPxLowL  ;
            TLabel    * lbOfsPxHghL  ; TEdit * edOfsPxHghL  ;

            TLabel    * lbEndOffsetT ; TEdit * edEndOffsetT ;
            TLabel    * lbCntrOffsetT; TEdit * edCntrOffsetT;
            TLabel    * lbOfsGapT    ; TEdit * edOfsGapT    ;
            TLabel    * lbOfsPxLowT  ; TEdit * edOfsPxLowT  ;
            TLabel    * lbOfsPxHghT  ; TEdit * edOfsPxHghT  ;

            TLabel    * lbEndOffsetR ; TEdit * edEndOffsetR ;
            TLabel    * lbCntrOffsetR; TEdit * edCntrOffsetR;
            TLabel    * lbOfsGapR    ; TEdit * edOfsGapR    ;
            TLabel    * lbOfsPxLowR  ; TEdit * edOfsPxLowR  ;
            TLabel    * lbOfsPxHghR  ; TEdit * edOfsPxHghR  ;

            TLabel    * lbEndOffsetB ; TEdit * edEndOffsetB ;
            TLabel    * lbCntrOffsetB; TEdit * edCntrOffsetB;
            TLabel    * lbOfsGapB    ; TEdit * edOfsGapB    ;
            TLabel    * lbOfsPxLowB  ; TEdit * edOfsPxLowB  ;
            TLabel    * lbOfsPxHghB  ; TEdit * edOfsPxHghB  ;
        };

        void MakeUI     (TParaUi  * _pParaUi , TWinControl * _tcControl);
        void SetArrange (TControl * _tcControl , int _iCol , int _iRow);
    protected :
        TParaUi ParaUi  ;

    public :
        RCT_Para Para ;
        RCT_Rslt Rslt ;
        RCT_Disp Disp ;

        TRectTracker *Tracker ;
        void SetPaintCallback(TOnChanged ChangeFunc);
        void SetTrackerVisible(bool _bVisible);
        void SetTrackerEnabled(bool _bEnabled);
        bool MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        bool MouseTrackerMv(                     TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void PaintTracker  (HDC _hHdc                                            , float _fScaleX , float _fScaleY);

        void ShowParaForm(TWinControl * _wcBase ) ;
        void LoadPara    (bool _bLoad , AnsiString _sFilePath , AnsiString _sId) ;
        void ApplyPara   (bool _bTable                                         ) ;

        bool Process     (IImage * _pImg , RCT_Rslt * _pRslt);
        void PaintRslt   (IImage * _pImg , HDC _hHdc);

        void GetRefValOfs(float &_fX , float &_fY , float &_fT );

};

class CWrt : public CProc
{
    public :
        CWrt();
        ~CWrt();

        struct TParaUi {
            TPanel * pnBase ;

            TCheckBox * cbLtOnDk     ;
            TLabel    * lbSampleGap  ; TEdit * edSampleGap  ;
            TLabel    * lbLineGap    ; TEdit * edLineGap    ;

            TLabel    * lbChkOfs     ; TEdit * edChkOfs     ;
            TLabel    * lbLowThsd    ; TEdit * edLowThsd    ;
            TLabel    * lbHighThsd   ; TEdit * edHighThsd   ;

            TLabel    * lbAverage    ; TEdit * edAverage    ;
            TLabel    * lbCompare    ; TEdit * edCompare    ;
            TLabel    * lbChkPxLow   ; TEdit * edChkPxLow   ;
            TLabel    * lbChkPxHgh   ; TEdit * edChkPxHgh   ;

            TLabel    * lbEndOffsetL ; TEdit * edEndOffsetL ;
            TLabel    * lbCntrOffsetL; TEdit * edCntrOffsetL;
            TLabel    * lbOfsGapL    ; TEdit * edOfsGapL    ;
            TLabel    * lbOfsPxLowL  ; TEdit * edOfsPxLowL  ;
            TLabel    * lbOfsPxHghL  ; TEdit * edOfsPxHghL  ;

            TLabel    * lbEndOffsetT ; TEdit * edEndOffsetT ;
            TLabel    * lbCntrOffsetT; TEdit * edCntrOffsetT;
            TLabel    * lbOfsGapT    ; TEdit * edOfsGapT    ;
            TLabel    * lbOfsPxLowT  ; TEdit * edOfsPxLowT  ;
            TLabel    * lbOfsPxHghT  ; TEdit * edOfsPxHghT  ;

            TLabel    * lbEndOffsetR ; TEdit * edEndOffsetR ;
            TLabel    * lbCntrOffsetR; TEdit * edCntrOffsetR;
            TLabel    * lbOfsGapR    ; TEdit * edOfsGapR    ;
            TLabel    * lbOfsPxLowR  ; TEdit * edOfsPxLowR  ;
            TLabel    * lbOfsPxHghR  ; TEdit * edOfsPxHghR  ;

            TLabel    * lbEndOffsetB ; TEdit * edEndOffsetB ;
            TLabel    * lbCntrOffsetB; TEdit * edCntrOffsetB;
            TLabel    * lbOfsGapB    ; TEdit * edOfsGapB    ;
            TLabel    * lbOfsPxLowB  ; TEdit * edOfsPxLowB  ;
            TLabel    * lbOfsPxHghB  ; TEdit * edOfsPxHghB  ;
        };

        void MakeUI     (TParaUi  * _pParaUi , TWinControl * _tcControl);
        void SetArrange (TControl * _tcControl , int _iCol , int _iRow);
    protected :
        TParaUi ParaUi  ;

    public :
        WRT_Para Para ;
        WRT_Rslt Rslt ;
        WRT_Disp Disp ;

        TRectTracker *Tracker ;
        void SetPaintCallback(TOnChanged ChangeFunc);
        void SetTrackerVisible(bool _bVisible);
        void SetTrackerEnabled(bool _bEnabled);
        bool MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        bool MouseTrackerMv(                     TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void PaintTracker  (HDC _hHdc                                            , float _fScaleX , float _fScaleY);

        void ShowParaForm(TWinControl * _wcBase ) ;
        void LoadPara    (bool _bLoad , AnsiString _sFilePath , AnsiString _sId) ;
        void ApplyPara   (bool _bTable                                         ) ;

        bool Process     (IImage * _pImg , WRT_Rslt * _pRslt);
        void PaintRslt   (IImage * _pImg , HDC _hHdc);

        void GetRefValOfs(float &_fX , float &_fY , float &_fT );

};

class CTsd : public CProc
{
    public :
        CTsd(int _iTrackerCnt = 1);
        ~CTsd();

        struct TParaUi {
            TPanel * pnBase ;
            TLabel * lbThresholdLow  ; TEdit * edThresholdLow  ;
            TLabel * lbThresholdHigh ; TEdit * edThresholdHigh ;
        };

        void MakeUI     (TParaUi  * _pParaUi , TWinControl * _tcControl);
        void SetArrange (TControl * _tcControl , int _iCol , int _iRow);
    protected :
        TParaUi ParaUi  ;

        int m_iTrackerCnt ;

    public :
        TSD_Para   Para ;
        TSD_Rslt * Rslt ;
        TSD_Disp   Disp ;

        TRectTracker **Tracker ;
        int  GetTrackerCnt(){return m_iTrackerCnt ;}


        void SetPaintCallback (TOnChanged ChangeFunc);
        void SetTrackerVisible(bool _bVisible);
        void SetTrackerEnabled(bool _bEnabled);
        bool MouseTrackerDn   (TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        bool MouseTrackerMv   (                     TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void MouseTrackerUp   (TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void PaintTracker     (HDC _hHdc                                            , float _fScaleX , float _fScaleY);

        void ShowParaForm(TWinControl * _wcBase ) ;
        void LoadPara    (bool _bLoad , AnsiString _sFilePath , AnsiString _sId) ;
        void ApplyPara   (bool _bTable                                         ) ;

        bool Process     (IImage * _pImg , TSD_Rslt * _pRslt);
        void PaintRslt   (IImage * _pImg , HDC _hHdc);

        void GetRslt     (int _iTrackerId , TSD_Rslt * _pRslt);
};

class CBlb : public CProc
{
    public :
        CBlb();
        ~CBlb();

        struct TParaUi {
            TPanel * pnBase ;

            TLabel * lbPitchX     ; TEdit     * edPitchX     ;
            TLabel * lbPitchY     ; TEdit     * edPitchY     ;
            TLabel * lbStartXOfs  ; TEdit     * edStartXOfs  ;
            TLabel * lbStartYOfs  ; TEdit     * edStartYOfs  ;
            TLabel * lbThreshold  ; TEdit     * edThreshold  ;
            TLabel * lbFindLtOnDk ; TCheckBox * cbFindLtOnDk ;
            TLabel * lbIgnrInside ; TCheckBox * cbIgnrInside ;
            TLabel * lbIgnrSide   ; TCheckBox * cbIgnrSide   ;

            TLabel * lbMinArea        ; TEdit * edMinArea        ;  TEdit * edMaxArea        ;
            TLabel * lbMinWidth       ; TEdit * edMinWidth       ;  TEdit * edMaxWidth       ;
            TLabel * lbMinHeight      ; TEdit * edMinHeight      ;  TEdit * edMaxHeight      ;
            TLabel * lbMinBoundary    ; TEdit * edMinBoundary    ;  TEdit * edMaxBoundary    ;
            TLabel * lbMinCompactness ; TEdit * edMinCompactness ;  TEdit * edMaxCompactness ;
            TLabel * lbMinAngle       ; TEdit * edMinAngle       ;  TEdit * edMaxAngle       ;
            TLabel * lbMinAspectRatio ; TEdit * edMinAspectRatio ;  TEdit * edMaxAspectRatio ;
        };

        void MakeUI     (TParaUi  * _pParaUi , TWinControl * _tcControl);
        void SetArrange (TControl * _tcControl , int _iCol , int _iRow);
    protected :
        TParaUi ParaUi  ;

    public :
        BLB_Para Para ;
        BLB_Fltr Fltr ;
        BLB_Rslt Rslt ;
        BLB_Disp Disp ;

        TRectTracker *Tracker ;
        void SetPaintCallback(TOnChanged ChangeFunc);
        void SetTrackerVisible(bool _bVisible);
        void SetTrackerEnabled(bool _bEnabled);
        bool MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        bool MouseTrackerMv(                     TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void PaintTracker  (HDC _hHdc                                            , float _fScaleX , float _fScaleY);

        void ShowParaForm(TWinControl * _wcBase ) ;
        void LoadPara    (bool _bLoad , AnsiString _sFilePath , AnsiString _sId) ;
        void ApplyPara   (bool _bTable                                         ) ;

        bool Process     (IImage * _pImg , BLB_Rslt * _pRslt);
        void PaintRslt   (IImage * _pImg , HDC _hHdc);

};

class CCal : public CProc
{
    public :
        CCal();
        ~CCal();

        struct TParaUi {
            TPanel * pnBase ;

            TLabel * lbDotSize  ; TEdit * edDotSize  ;
            TLabel * lbDotPitch ; TEdit * edDotPitch ;


            TLabel * lbPitchX     ; TEdit     * edPitchX     ;
            TLabel * lbPitchY     ; TEdit     * edPitchY     ;
            TLabel * lbStartXOfs  ; TEdit     * edStartXOfs  ;
            TLabel * lbStartYOfs  ; TEdit     * edStartYOfs  ;
            TLabel * lbThreshold  ; TEdit     * edThreshold  ;
            TLabel * lbFindLtOnDk ; TCheckBox * cbFindLtOnDk ;
            TLabel * lbIgnrInside ; TCheckBox * cbIgnrInside ;
            TLabel * lbIgnrSide   ; TCheckBox * cbIgnrSide   ;

            TLabel * lbMinArea        ; TEdit * edMinArea        ;  TEdit * edMaxArea        ;
            TLabel * lbMinWidth       ; TEdit * edMinWidth       ;  TEdit * edMaxWidth       ;
            TLabel * lbMinHeight      ; TEdit * edMinHeight      ;  TEdit * edMaxHeight      ;
            TLabel * lbMinBoundary    ; TEdit * edMinBoundary    ;  TEdit * edMaxBoundary    ;
            TLabel * lbMinCompactness ; TEdit * edMinCompactness ;  TEdit * edMaxCompactness ;
            TLabel * lbMinAngle       ; TEdit * edMinAngle       ;  TEdit * edMaxAngle       ;
            TLabel * lbMinAspectRatio ; TEdit * edMinAspectRatio ;  TEdit * edMaxAspectRatio ;


            TLabel * lbResX           ; TEdit * edResX           ;  
            TLabel * lbResY           ; TEdit * edResY           ;  
        };

        void MakeUI     (TParaUi  * _pParaUi , TWinControl * _tcControl);
        void SetArrange (TControl * _tcControl , int _iCol , int _iRow);
    protected :
        TParaUi ParaUi  ;

    public :
        CAL_Para Para ;
        CAL_Rslt Rslt ;
        CAL_Disp Disp ;

        TRectTracker *Tracker ;
        void SetPaintCallback(TOnChanged ChangeFunc);
        void SetTrackerVisible(bool _bVisible);
        void SetTrackerEnabled(bool _bEnabled);
        bool MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        bool MouseTrackerMv(                     TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void PaintTracker  (HDC _hHdc                                            , float _fScaleX , float _fScaleY);

        void ShowParaForm(TWinControl * _wcBase ) ;
        void LoadPara    (bool _bLoad , AnsiString _sFilePath , AnsiString _sId) ;
        void ApplyPara   (bool _bTable                                         ) ;

        bool Process     (IImage * _pImg , CAL_Rslt * _pRslt);
        void PaintRslt   (IImage * _pImg , HDC _hHdc);

};



class CCcl : public CProc
{
    public :
        CCcl();
        ~CCcl();

        struct TParaUi {
            TPanel * pnBase ;

            TLabel * lbGap          ; TEdit * edGap          ;
            TLabel * lbSampleCnt    ; TEdit * edSampleCnt    ;
            TLabel * lbStartAng     ; TEdit * edStartAng     ;
            TLabel * lbEndAng       ; TEdit * edEndAng       ;

            TLabel * lbEdgAverage   ; TEdit * edEdgAverage   ;
            TLabel * lbEdgCompare   ; TEdit * edEdgCompare   ;
            TLabel * lbEdgChkPxLow  ; TEdit * edEdgChkPxLow  ;
            TLabel * lbEdgChkPxHgh  ; TEdit * edEdgChkPxHgh  ;
            TLabel * lbEdgOfsGap    ; TEdit * edEdgOfsGap    ;
            TLabel * lbEdgOfsPxLow  ; TEdit * edEdgOfsPxLow  ;
            TLabel * lbEdgOfsPxHgh  ; TEdit * edEdgOfsPxHgh  ;
        };
        void MakeUI     (TParaUi  * _pParaUi  , TWinControl * _tcControl);
        void SetArrange (TControl * _tcControl , int _iCol , int _iRow);

    protected :
        TParaUi ParaUi ;
    public :

        CCL_Para Para ;
        CCL_Rslt Rslt ;
        CCL_Disp Disp ;

        TCircleTracker * Tracker ;

        void SetPaintCallback(TOnChanged ChangeFunc);
        void SetTrackerVisible(bool _bVisible);
        void SetTrackerEnabled(bool _bEnabled);
        bool MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        bool MouseTrackerMv(                     TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void PaintTracker  (HDC _hHdc                                            , float _fScaleX , float _fScaleY);

        void ShowParaForm(TWinControl   * _wcBase   ) ;
        void LoadPara    (bool _bLoad , AnsiString _sFilePath , AnsiString _sId) ;
        void ApplyPara   (bool _bTable                                         ) ;

        bool Process     (IImage * _pImg , CCL_Rslt * _pRslt);
        void PaintRslt   (IImage * _pImg , HDC _hHdc);

        void GetRefValOfs(float &_fX , float &_fY , float &_fT );

};

class COcv : public CProc
{
    public :
        COcv();
        ~COcv();

/*                                   
struct OCV_TrPr { //Train Para
    int iInspOft   ;
    int iNoInspOft ;
    int iThreshold ;
} ;

struct OCV_Para { //Insp Para
    int   iThreshold ; //검사 임계값
    int   iInspFreq  ; //검사빈도수 높으면 높을수록 빨라짐.
    float fSinc      ; //싱크 인데 일단 안씀.
} ;

*/

        struct TParaUi {
            TPanel * pnBase ;
            TLabel * lbInspOft   ; TEdit * edInspOft   ;
            TLabel * lbNoInspOft ; TEdit * edNoInspOft ;
            TLabel * lbThreshold ; TEdit * edThreshold ;

            TLabel * lbInspFreq  ; TEdit * edInspFreq  ;
            TLabel * lbSinc      ; TEdit * edSinc      ;
        };

        void MakeUI     (TParaUi  * _pParaUi , TWinControl * _tcControl);
        void SetArrange (TControl * _tcControl , int _iCol , int _iRow);
    protected :
        TParaUi ParaUi  ;

    public :
        OCV_TrPr TrPr ;
        OCV_Para Para ;
        OCV_Rslt Rslt ;
        OCV_Disp Disp ;

        IArea *  m_pTrArea   ;
        IArea *  m_pRsltArea ;

        IImage * m_pTrImage  ;

        TRectTracker *TrackerT ;//트레인용.
        TRectTracker *TrackerI ;// 검사용. 

        void SetPaintCallback(TOnChanged ChangeFunc);
        void SetTrackerVisible(bool _bVisible);
        void SetTrackerEnabled(bool _bEnabled);
        bool MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        bool MouseTrackerMv(                     TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void PaintTracker  (HDC _hHdc                                            , float _fScaleX , float _fScaleY);

        void ShowParaForm(TWinControl * _wcBase ) ;
        void LoadPara    (bool _bLoad , AnsiString _sFilePath , AnsiString _sId) ;
        void ApplyPara   (bool _bTable                                         ) ;

        bool Train       (IImage * _pImg );
        bool Process     (IImage * _pImg , OCV_Rslt * _pRslt);
        void PaintRslt   (IImage * _pImg , HDC _hHdc);

        void GetRefValOfs(float &_fX , float &_fY , float &_fT );

};

//검사 묶음.
class CProcPkg {
    public :
        CProcPkg();
        ~CProcPkg();

    protected :
    public :
        AnsiString m_sPkgName ;
        int        m_iProcCnt ;
        CProc **   m_pProc    ;

        void SetProcList(int _iProcCnt , EN_PROC_TYPE * _pProcType , int * _pIntPara);
        void DelProcList();

        void SetPaintCallback(TOnChanged ChangeFunc);
        void SetTrackerVisible(int _iProcId , bool _bVisible);
        void SetTrackerEnabled(int _iProcId , bool _bEnabled);
        bool MouseTrackerDn(int _iProcId , TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        bool MouseTrackerMv(int _iProcId ,                      TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void MouseTrackerUp(int _iProcId , TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void PaintTracker  (int _iProcId , HDC _hHdc , float _fScaleX , float _fScaleY);

        void ShowParaForm(TPageControl * _pcBase) ;
        void LoadPara    (bool _bLoad , AnsiString _sFilePath) ;
        void ApplyPara   (bool _bTable                       ) ;

        EN_PROC_TYPE GetProcType (int _iProcId );
        bool         Process     (int _iProcId , IImage * _pImg , void * _pRslt);
        bool         Train       (int _iProcId , IImage * _pImg                );
        void         PaintRslt   (int _iProcId , IImage * _pImg , HDC    _hHdc );
        void         PaintRsltAll(               IImage * _pImg , HDC    _hHdc );

        void SetInspOfs  (int _iProcId , float  _fX , float  _fY );
        void GetRefValOfs(int _iProcId , float &_fX , float &_fY , float &_fT);

        void SetRefVal(int _iProcId , float  _fX , float  _fY , float  _fT );
        void ClrRefVal(int _iProcId                                        );
        void GetRefVal(int _iProcId , float &_fX , float &_fY , float &_fT );


};


#endif
