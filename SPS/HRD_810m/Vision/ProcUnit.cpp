//---------------------------------------------------------------------------


#pragma hdrstop

#include "ProcUnit.h"
#include "SVInterface.h"
#include "UserINI.h"
#include "SVMaths.h"
#include "SLogUnit.h"
#include "FormTrainChk.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

TRect CProc::GetOfsRect(TRect _tRect)
{
    TRect tRetRect ;

    tRetRect.left   = _tRect.left   + m_fInspOfsX ;
    tRetRect.top    = _tRect.top    + m_fInspOfsY ;
    tRetRect.right  = _tRect.right  + m_fInspOfsX ;
    tRetRect.bottom = _tRect.bottom + m_fInspOfsY ;

    return tRetRect ;
}

TPoint CProc::GetOfsPoint(TPoint _tPnt )
{
    TPoint tRetPnt ;

    tRetPnt.x = _tPnt.x + m_fInspOfsX ;
    tRetPnt.y = _tPnt.y + m_fInspOfsY ;

    return tRetPnt ;
}

TCircle CProc::GetOfsCircle(TCircle _tCcl )
{
    TCircle tRetCcl ;

    tRetCcl.cx = _tCcl.cx + m_fInspOfsX ;
    tRetCcl.cy = _tCcl.cy + m_fInspOfsY ;

    return tRetCcl ;
}

void CProc::PaintOfsRect(HDC _hHdc , float _fScaleX , float _fScaleY , TRect   _tRect)
{
    IGdi *Gdi = GDI_New(_hHdc);

    Gdi -> SetScale(_fScaleX , _fScaleY);

    Gdi -> m_tPen.Style = psSolid ;
    Gdi -> m_tPen.Color = clYellow ;
    Gdi -> m_tPen.Width = 1  ;
    Gdi -> Rect(false, _tRect, 0);

    char str[500];
    Gdi -> m_tText.Color = clFuchsia ;
    sprintf(str, "Insp Offset X, Y : %.3f, %.3f" , m_fInspOfsX , m_fInspOfsY );
    Gdi -> Text(_tRect.left+5 , _tRect.top-10 , str);

    GDI_Del(Gdi);

}

void CProc::PaintOfsLine(HDC _hHdc , float _fScaleX , float _fScaleY , TPoint  _tSttPnt , TPoint _tEndPnt)
{
    IGdi *Gdi = GDI_New(_hHdc);

    Gdi -> SetScale(_fScaleX , _fScaleY);

    Gdi -> m_tPen.Style = psSolid ;
    Gdi -> m_tPen.Color = clYellow ;
    Gdi -> m_tPen.Width = 1  ;
    Gdi -> LineAllow(_tSttPnt.x, _tSttPnt.y , _tEndPnt.x , _tEndPnt.y );

    char str[500];
    Gdi -> m_tText.Color = clFuchsia ;
    sprintf(str, "Insp Offset X, Y : %.1f, %.1f" , m_fInspOfsX , m_fInspOfsY );
    Gdi -> Text(_tSttPnt.x-10, _tSttPnt.y+5 , str);

    GDI_Del(Gdi);
}

void CProc::PaintOfsCircle (HDC _hHdc , float _fScaleX , float _fScaleY , TCircle _tCircle)
{
    IGdi *Gdi = GDI_New(_hHdc);

    Gdi -> SetScale(_fScaleX , _fScaleY);

    Gdi -> m_tPen.Style = psSolid ;
    Gdi -> m_tPen.Color = clYellow ;
    Gdi -> m_tPen.Width = 1  ;
    Gdi -> Circle(false , _tCircle.cx, _tCircle.cy , _tCircle.rad , 0);

    char str[500];
    Gdi -> m_tText.Color = clFuchsia ;
    sprintf(str, "Insp Offset X, Y : %.1f, %.1f" , m_fInspOfsX , m_fInspOfsY );
    Gdi -> Text(_tCircle.cx , _tCircle.cx , str);

    GDI_Del(Gdi);

}


/******************************************************************************/
/* Edge                                                                       */
/******************************************************************************/
CEdg::CEdg()
{
    m_iProcType = ptEdg ;

    Tracker = new TLineTracker() ;
    Tracker -> Caption = "EDG";

    Disp.clDkToLtPnt = clRed    ;
    Disp.clLtToDkPnt = clBlue   ;
    Disp.clText      = clLime   ;

    ParaUi.pnBase = NULL ;

    //MakeUI(&ParaUi);
}

CEdg::~CEdg()
{
    delete Tracker ;
}

void CEdg::MakeUI(TParaUi  * _pParaUi , TWinControl * _tcControl)
{
    int iR = 0 ;

    _pParaUi->pnBase = new TPanel(_tcControl); _pParaUi->pnBase -> Parent = _tcControl ;

    _pParaUi->lbAverage   = new TLabel(_pParaUi->pnBase); _pParaUi->edAverage  = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbCompare   = new TLabel(_pParaUi->pnBase); _pParaUi->edCompare  = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbChkPxLow  = new TLabel(_pParaUi->pnBase); _pParaUi->edChkPxLow = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbChkPxHgh  = new TLabel(_pParaUi->pnBase); _pParaUi->edChkPxHgh = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbOfsGap    = new TLabel(_pParaUi->pnBase); _pParaUi->edOfsGap   = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbOfsPxLow  = new TLabel(_pParaUi->pnBase); _pParaUi->edOfsPxLow = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbOfsPxHgh  = new TLabel(_pParaUi->pnBase); _pParaUi->edOfsPxHgh = new TEdit (_pParaUi->pnBase);

    SetArrange(_pParaUi->lbAverage  , 0 , iR ); SetArrange(_pParaUi->edAverage   , 1 , iR++ );
    SetArrange(_pParaUi->lbCompare  , 0 , iR ); SetArrange(_pParaUi->edCompare   , 1 , iR++ );
    SetArrange(_pParaUi->lbChkPxLow , 0 , iR ); SetArrange(_pParaUi->edChkPxLow  , 1 , iR++ );
    SetArrange(_pParaUi->lbChkPxHgh , 0 , iR ); SetArrange(_pParaUi->edChkPxHgh  , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsGap   , 0 , iR ); SetArrange(_pParaUi->edOfsGap    , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsPxLow , 0 , iR ); SetArrange(_pParaUi->edOfsPxLow  , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsPxHgh , 0 , iR ); SetArrange(_pParaUi->edOfsPxHgh  , 1 , iR++ );

    _pParaUi->lbAverage  -> Parent = _pParaUi->pnBase ; _pParaUi->edAverage  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbCompare  -> Parent = _pParaUi->pnBase ; _pParaUi->edCompare  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbChkPxLow -> Parent = _pParaUi->pnBase ; _pParaUi->edChkPxLow -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbChkPxHgh -> Parent = _pParaUi->pnBase ; _pParaUi->edChkPxHgh -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsGap   -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsGap   -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsPxLow -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsPxLow -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsPxHgh -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsPxHgh -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbAverage  -> Caption = "평균범위         ";//"Average " ;
    _pParaUi->lbCompare  -> Caption = "비교범위         ";//"Compare " ;
    _pParaUi->lbChkPxLow -> Caption = "확정픽셀Low      ";//"ChkPxLow" ;
    _pParaUi->lbChkPxHgh -> Caption = "확정픽셀High     ";//"ChkPxHgh" ;
    _pParaUi->lbOfsGap   -> Caption = "확정옵셑거리     ";//"OfsGap  " ;
    _pParaUi->lbOfsPxLow -> Caption = "확정옵셑거리Low  ";//"OfsPxLow" ;
    _pParaUi->lbOfsPxHgh -> Caption = "확정옵셑거리High ";//"OfsPxHgh" ;
}

void CEdg::SetArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    const int iControlWidth   = 120;
    const int iControlHeight  = 20  ;
    const int iStartRowOffset = 5   ;
    const int iStartColOffset = 5   ;

    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;
}

void CEdg::SetPaintCallback(TOnChanged ChangeFunc)
{
    Tracker -> SetFunc(ChangeFunc) ;
}

void CEdg::SetTrackerVisible(bool _bVisible)
{
    Tracker -> Visible = _bVisible ;
}

void CEdg::SetTrackerEnabled(bool _bEnabled)
{
    Tracker -> Enabled = _bEnabled ;
}

bool CEdg::MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker -> Focused = false ;

    if(Tracker -> TrackerDw(Button, Shift , X,Y, _fScaleX , _fScaleY)) { m_fInspOfsX = 0.0 ; m_fInspOfsY = 0.0 ; return true ;}

    return false ;
}

bool CEdg::MouseTrackerMv(TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    if(Tracker -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY)) return true ;

    return false ;
}

void CEdg::MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker -> TrackerUp(Button, Shift );
}

void CEdg::PaintTracker(HDC _hHdc , float _fScaleX , float _fScaleY)
{
    if(Tracker -> Visible)PaintOfsLine(_hHdc , _fScaleX , _fScaleY , GetOfsPoint(Tracker -> GetStartPnt()) , GetOfsPoint(Tracker -> GetEndPnt()));

    Tracker -> Paint(_hHdc , _fScaleX , _fScaleY);
}

void CEdg::ShowParaForm(TWinControl* _wcBase)
{
    if(ParaUi.pnBase == NULL)MakeUI(&ParaUi,_wcBase) ;
    ParaUi.pnBase -> Parent = _wcBase ;
    ParaUi.pnBase -> Align = alClient ;
}

void CEdg::LoadPara(bool _bLoad , AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId+"_Edge" ;

    AnsiString sFilePath = _sFilePath + ".ini" ;
    AnsiString sTrakPath = _sFilePath + "Tracker.ini" ;

    Tracker -> OnLoadSave(_bLoad , sTrakPath , sTemp + "_Tracker");

    if(_bLoad) {
        UserINI.Load(sFilePath, sTemp , "iAverage ", Para.iAverage );
        UserINI.Load(sFilePath, sTemp , "iCompare ", Para.iCompare );
        UserINI.Load(sFilePath, sTemp , "iChkPxLow", Para.iChkPxLow);
        UserINI.Load(sFilePath, sTemp , "iChkPxHgh", Para.iChkPxHgh);
        UserINI.Load(sFilePath, sTemp , "iOfsGap  ", Para.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "iOfsPxLow", Para.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "iOfsPxHgh", Para.iOfsPxHgh);
    }
    else {
        UserINI.Save(sFilePath, sTemp , "iAverage ", Para.iAverage );
        UserINI.Save(sFilePath, sTemp , "iCompare ", Para.iCompare );
        UserINI.Save(sFilePath, sTemp , "iChkPxLow", Para.iChkPxLow);
        UserINI.Save(sFilePath, sTemp , "iChkPxHgh", Para.iChkPxHgh);
        UserINI.Save(sFilePath, sTemp , "iOfsGap  ", Para.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "iOfsPxLow", Para.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "iOfsPxHgh", Para.iOfsPxHgh);
    }
}

void CEdg::ApplyPara(bool _bTable)
{
    if(_bTable) {
        ParaUi.edAverage  -> Text = Para.iAverage  ;
        ParaUi.edCompare  -> Text = Para.iCompare  ;
        ParaUi.edChkPxLow -> Text = Para.iChkPxLow ;
        ParaUi.edChkPxHgh -> Text = Para.iChkPxHgh ;
        ParaUi.edOfsGap   -> Text = Para.iOfsGap   ;
        ParaUi.edOfsPxLow -> Text = Para.iOfsPxLow ;
        ParaUi.edOfsPxHgh -> Text = Para.iOfsPxHgh ;
    }

    else {
        int iTemp ;

        iTemp = StrToIntDef(ParaUi.edAverage  -> Text , 1); if (Para.iAverage  != iTemp) { Trace("Operator" , "Both Edg_Average values are different."); }
        iTemp = StrToIntDef(ParaUi.edCompare  -> Text , 1); if (Para.iCompare  != iTemp) { Trace("Operator" , "Both Edg_Compare values are different."); }
        iTemp = StrToIntDef(ParaUi.edChkPxLow -> Text , 0); if (Para.iChkPxLow != iTemp) { Trace("Operator" , "Both Edg_ChkPxLow values are different."); }
        iTemp = StrToIntDef(ParaUi.edChkPxHgh -> Text , 0); if (Para.iChkPxHgh != iTemp) { Trace("Operator" , "Both Edg_ChkPxHgh values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsGap   -> Text , 0); if (Para.iOfsGap   != iTemp) { Trace("Operator" , "Both Edg_OfsGap values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsPxLow -> Text , 0); if (Para.iOfsPxLow != iTemp) { Trace("Operator" , "Both Edg_OfsPxLow values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsPxHgh -> Text , 0); if (Para.iOfsPxHgh != iTemp) { Trace("Operator" , "Both Edg_OfsPxHgh values are different."); }



        Para.iAverage  = StrToIntDef(ParaUi.edAverage  -> Text , 1);
        Para.iCompare  = StrToIntDef(ParaUi.edCompare  -> Text , 1);
        Para.iChkPxLow = StrToIntDef(ParaUi.edChkPxLow -> Text , 0);
        Para.iChkPxHgh = StrToIntDef(ParaUi.edChkPxHgh -> Text , 0);
        Para.iOfsGap   = StrToIntDef(ParaUi.edOfsGap   -> Text , 0);
        Para.iOfsPxLow = StrToIntDef(ParaUi.edOfsPxLow -> Text , 0);
        Para.iOfsPxHgh = StrToIntDef(ParaUi.edOfsPxHgh -> Text , 0);
        ApplyPara(true);
    }
}

bool CEdg::Process(IImage * _pImg , EDG_Rslt * _pRslt)
{
    bool bRet ;

    bRet = EDG_Inspect(_pImg , GetOfsPoint(Tracker -> GetStartPnt()) , GetOfsPoint(Tracker -> GetEndPnt()) , Para , _pRslt);
    memcpy(&Rslt , _pRslt , sizeof(EDG_Rslt));

    return bRet ;
}

void CEdg::PaintRslt(IImage * _pImg , HDC _hHdc)
{
    EDG_PaintRslt(_pImg , _hHdc , &Rslt , Disp);
}


/******************************************************************************/
/* Line                                                                       */
/******************************************************************************/
CLin::CLin()
{
    m_iProcType = ptLin ;

    Tracker = new TRectTracker() ;

    Tracker -> Caption = "LIN";

    //MakeUI(&ParaUi);
    ParaUi.pnBase = NULL ;

    Disp.clLine = clLime   ;
    Disp.clEdge = clLime   ;
    Disp.clText = clLime   ;

}

CLin::~CLin()
{
    delete Tracker ;
}

void CLin::MakeUI(TParaUi  * _pParaUi , TWinControl * _tcControl)
{
    int iR = 0 ;
    _pParaUi->pnBase = new TPanel(_tcControl  ); _pParaUi->pnBase -> Parent = _tcControl ;

    _pParaUi->lbScanDirect = new TLabel(_pParaUi->pnBase); _pParaUi->cbScanDirect = new TComboBox(_pParaUi->pnBase);
    _pParaUi->cbLtToDk     = new TCheckBox(_pParaUi->pnBase);
    _pParaUi->lbSampleGap  = new TLabel(_pParaUi->pnBase); _pParaUi->edSampleGap  = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbLineGap    = new TLabel(_pParaUi->pnBase); _pParaUi->edLineGap    = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbEndOffset  = new TLabel(_pParaUi->pnBase); _pParaUi->edEndOffset  = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbCntrOffset = new TLabel(_pParaUi->pnBase); _pParaUi->edCntrOffset = new TEdit    (_pParaUi->pnBase);

    SetArrange(_pParaUi->lbScanDirect , 0 , iR );  SetArrange(_pParaUi->cbScanDirect , 1 , iR++ );
    SetArrange(_pParaUi->cbLtToDk     , 0 , iR++ );
    SetArrange(_pParaUi->lbSampleGap  , 0 , iR );  SetArrange(_pParaUi->edSampleGap  , 1 , iR++ );
    SetArrange(_pParaUi->lbLineGap    , 0 , iR );  SetArrange(_pParaUi->edLineGap    , 1 , iR++ );
    SetArrange(_pParaUi->lbEndOffset  , 0 , iR );  SetArrange(_pParaUi->edEndOffset  , 1 , iR++ );
    SetArrange(_pParaUi->lbCntrOffset , 0 , iR );  SetArrange(_pParaUi->edCntrOffset , 1 , iR++ );

    _pParaUi->lbScanDirect -> Parent = _pParaUi->pnBase ; _pParaUi->cbScanDirect -> Parent = _pParaUi->pnBase ;
    _pParaUi->cbLtToDk     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbSampleGap  -> Parent = _pParaUi->pnBase ; _pParaUi->edSampleGap  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbLineGap    -> Parent = _pParaUi->pnBase ; _pParaUi->edLineGap    -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEndOffset  -> Parent = _pParaUi->pnBase ; _pParaUi->edEndOffset  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbCntrOffset -> Parent = _pParaUi->pnBase ; _pParaUi->edCntrOffset -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbScanDirect -> Caption = "검색방향           ";//"ScanDirect" ;
    _pParaUi->cbLtToDk     -> Caption = "밝은곳에서어두운곳 ";//"LtToDk    " ;
    _pParaUi->lbSampleGap  -> Caption = "샘플링 간격        ";//"SampleGap " ;
    _pParaUi->lbLineGap    -> Caption = "라인인정범위       ";//"LineGap   " ;
    _pParaUi->lbEndOffset  -> Caption = "검색 시작끝 오프셑 ";//"EndOffset " ;
    _pParaUi->lbCntrOffset -> Caption = "검색 중간 오프셑   ";//"CntrOffset" ;

    _pParaUi->cbScanDirect -> AddItem("Down" ,NULL);
    _pParaUi->cbScanDirect -> AddItem("Up"   ,NULL);
    _pParaUi->cbScanDirect -> AddItem("Right",NULL);
    _pParaUi->cbScanDirect -> AddItem("Left" ,NULL);
    _pParaUi->cbScanDirect -> ItemIndex = 0 ;

    //Edge
    _pParaUi->lbEdgAverage   = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgAverage  = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgCompare   = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgCompare  = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgChkPxLow  = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgChkPxLow = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgChkPxHgh  = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgChkPxHgh = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgOfsGap    = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgOfsGap   = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgOfsPxLow  = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgOfsPxLow = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgOfsPxHgh  = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgOfsPxHgh = new TEdit (_pParaUi->pnBase);

    iR++;
    SetArrange(_pParaUi->lbEdgAverage  , 0 , iR); SetArrange(_pParaUi->edEdgAverage   , 1 , iR++);
    SetArrange(_pParaUi->lbEdgCompare  , 0 , iR); SetArrange(_pParaUi->edEdgCompare   , 1 , iR++);
    SetArrange(_pParaUi->lbEdgChkPxLow , 0 , iR); SetArrange(_pParaUi->edEdgChkPxLow  , 1 , iR++);
    SetArrange(_pParaUi->lbEdgChkPxHgh , 0 , iR); SetArrange(_pParaUi->edEdgChkPxHgh  , 1 , iR++);
    SetArrange(_pParaUi->lbEdgOfsGap   , 0 , iR); SetArrange(_pParaUi->edEdgOfsGap    , 1 , iR++);
    SetArrange(_pParaUi->lbEdgOfsPxLow , 0 , iR); SetArrange(_pParaUi->edEdgOfsPxLow  , 1 , iR++);
    SetArrange(_pParaUi->lbEdgOfsPxHgh , 0 , iR); SetArrange(_pParaUi->edEdgOfsPxHgh  , 1 , iR++);

    _pParaUi->lbEdgAverage  -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgAverage  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgCompare  -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgCompare  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgChkPxLow -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgChkPxLow -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgChkPxHgh -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgChkPxHgh -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgOfsGap   -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgOfsGap   -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgOfsPxLow -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgOfsPxLow -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgOfsPxHgh -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgOfsPxHgh -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbEdgAverage  -> Caption = "평균범위        ";//"Average " ;"Average " ;
    _pParaUi->lbEdgCompare  -> Caption = "비교범위        ";//"Compare " ;"Compare " ;
    _pParaUi->lbEdgChkPxLow -> Caption = "확정픽셀Low     ";//"ChkPxLow" ;"ChkPxLow" ;
    _pParaUi->lbEdgChkPxHgh -> Caption = "확정픽셀High    ";//"ChkPxHgh" ;"ChkPxHgh" ;
    _pParaUi->lbEdgOfsGap   -> Caption = "확정옵셑거리    ";//"OfsGap  " ;"OfsGap  " ;
    _pParaUi->lbEdgOfsPxLow -> Caption = "확정옵셑거리Low ";//"OfsPxLow" ;"OfsPxLow" ;
    _pParaUi->lbEdgOfsPxHgh -> Caption = "확정옵셑거리High";//"OfsPxHgh" ;"OfsPxHgh" ;
}

void CLin::SetArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    const int iControlWidth   = 120 ;
    const int iControlHeight  = 20  ;
    const int iStartRowOffset = 5   ;
    const int iStartColOffset = 5   ;

    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;
}

void CLin::SetPaintCallback(TOnChanged ChangeFunc)
{
    Tracker -> SetFunc(ChangeFunc) ;
}

void CLin::SetTrackerVisible(bool _bVisible)
{
    Tracker -> Visible = _bVisible ;
}

void CLin::SetTrackerEnabled(bool _bEnabled)
{
    Tracker -> Enabled = _bEnabled ;
}

bool CLin::MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker -> Focused = false ;

    if(Tracker -> TrackerDw(Button, Shift , X,Y, _fScaleX , _fScaleY)) { m_fInspOfsX = 0.0 ; m_fInspOfsY = 0.0 ; return true ;}

    return false ;
}

bool CLin::MouseTrackerMv(TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    if(Tracker -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY)) return true ;

    return false ;
}

void CLin::MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker -> TrackerUp(Button, Shift );
}

void CLin::PaintTracker(HDC _hHdc , float _fScaleX , float _fScaleY)
{
    if(Tracker -> Visible)PaintOfsRect(_hHdc , _fScaleX , _fScaleY , GetOfsRect(Tracker -> GetTRect()));
    Tracker -> Paint(_hHdc , _fScaleX , _fScaleY);
}

void CLin::ShowParaForm(TWinControl* _wcBase)
{
    if(ParaUi.pnBase == NULL)MakeUI(&ParaUi,_wcBase) ;
    ParaUi.pnBase -> Parent = _wcBase ;
    ParaUi.pnBase -> Align = alClient ;
}

void CLin::LoadPara (bool _bLoad , AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId+"_Line" ;

    AnsiString sFilePath = _sFilePath + ".ini" ;
    AnsiString sTrakPath = _sFilePath + "Tracker.ini" ;

    Tracker -> OnLoadSave(_bLoad , sTrakPath , sTemp + "_Tracker");

    int iTemp ;

    if(_bLoad) {
        UserINI.Load(sFilePath, sTemp , "iScanDirect", iTemp           );Para.iScanDirect=iTemp ;
        UserINI.Load(sFilePath, sTemp , "bLtToDk    ", Para.bLtToDk    );
        UserINI.Load(sFilePath, sTemp , "iSampleGap ", Para.iSampleGap );
        UserINI.Load(sFilePath, sTemp , "iLineGap   ", Para.iLineGap   );
        UserINI.Load(sFilePath, sTemp , "iEndOffset ", Para.iEndOffset );
        UserINI.Load(sFilePath, sTemp , "iCntrOffset", Para.iCntrOffset);

        UserINI.Load(sFilePath, sTemp , "iEdgAverage   ", Para.EdgPara.iAverage );
        UserINI.Load(sFilePath, sTemp , "iEdgCompare   ", Para.EdgPara.iCompare );
        UserINI.Load(sFilePath, sTemp , "iEdgChkPxLow  ", Para.EdgPara.iChkPxLow);
        UserINI.Load(sFilePath, sTemp , "iEdgChkPxHgh  ", Para.EdgPara.iChkPxHgh);
        UserINI.Load(sFilePath, sTemp , "iEdgOfsGap    ", Para.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "iEdgOfsPxLow  ", Para.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "iEdgOfsPxHgh  ", Para.EdgPara.iOfsPxHgh);
    }
    else {
        UserINI.Save(sFilePath, sTemp , "iScanDirect", Para.iScanDirect);
        UserINI.Save(sFilePath, sTemp , "bLtToDk    ", Para.bLtToDk    );
        UserINI.Save(sFilePath, sTemp , "iSampleGap ", Para.iSampleGap );
        UserINI.Save(sFilePath, sTemp , "iLineGap   ", Para.iLineGap   );
        UserINI.Save(sFilePath, sTemp , "iEndOffset ", Para.iEndOffset );
        UserINI.Save(sFilePath, sTemp , "iCntrOffset", Para.iCntrOffset);

        UserINI.Save(sFilePath, sTemp , "iEdgAverage ", Para.EdgPara.iAverage );
        UserINI.Save(sFilePath, sTemp , "iEdgCompare ", Para.EdgPara.iCompare );
        UserINI.Save(sFilePath, sTemp , "iEdgChkPxLow", Para.EdgPara.iChkPxLow);
        UserINI.Save(sFilePath, sTemp , "iEdgChkPxHgh", Para.EdgPara.iChkPxHgh);
        UserINI.Save(sFilePath, sTemp , "iEdgOfsGap  ", Para.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "iEdgOfsPxLow", Para.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "iEdgOfsPxHgh", Para.EdgPara.iOfsPxHgh);
    }
}

void CLin::ApplyPara(bool _bTable)
{

    if(_bTable) {
        ParaUi.cbScanDirect  -> ItemIndex = Para.iScanDirect;
        ParaUi.cbLtToDk      -> Checked   = Para.bLtToDk    ;
        ParaUi.edSampleGap   -> Text      = Para.iSampleGap ;
        ParaUi.edLineGap     -> Text      = Para.iLineGap   ;
        ParaUi.edEndOffset   -> Text      = Para.iEndOffset ;
        ParaUi.edCntrOffset  -> Text      = Para.iCntrOffset;

        //Edge.
        ParaUi.edEdgAverage  -> Text = Para.EdgPara.iAverage  ;
        ParaUi.edEdgCompare  -> Text = Para.EdgPara.iCompare  ;
        ParaUi.edEdgChkPxLow -> Text = Para.EdgPara.iChkPxLow ;
        ParaUi.edEdgChkPxHgh -> Text = Para.EdgPara.iChkPxHgh ;
        ParaUi.edEdgOfsGap   -> Text = Para.EdgPara.iOfsGap   ;
        ParaUi.edEdgOfsPxLow -> Text = Para.EdgPara.iOfsPxLow ;
        ParaUi.edEdgOfsPxHgh -> Text = Para.EdgPara.iOfsPxHgh ;

    }
    else {
        int  iTemp ;
        bool bTemp ;

        iTemp = ParaUi.cbScanDirect  -> ItemIndex             ; if (Para.iScanDirect !=  iTemp) { Trace("Operator" , "Both Lin_ScanDirect values are different."); }
        bTemp = ParaUi.cbLtToDk      -> Checked               ; if (Para.bLtToDk     !=  bTemp) { Trace("Operator" , "Both Lin_LtToDk values are different."); }
        iTemp = StrToIntDef(ParaUi.edSampleGap   -> Text , 5 ); if (Para.iSampleGap  !=  iTemp) { Trace("Operator" , "Both Lin_SampleGap values are different."); }
        iTemp = StrToIntDef(ParaUi.edLineGap     -> Text , 1 ); if (Para.iLineGap    !=  iTemp) { Trace("Operator" , "Both Lin_LineGap values are different."); }
        iTemp = StrToIntDef(ParaUi.edEndOffset   -> Text , 0 ); if (Para.iEndOffset  !=  iTemp) { Trace("Operator" , "Both Lin_EndOffset values are different."); }
        iTemp = StrToIntDef(ParaUi.edCntrOffset  -> Text , 0 ); if (Para.iCntrOffset !=  iTemp) { Trace("Operator" , "Both Lin_CntrOffset values are different."); }

            //Edge.
        iTemp = StrToIntDef(ParaUi.edEdgAverage  -> Text , 1); if (Para.EdgPara.iAverage  != iTemp) { Trace("Operator" , "Both Lin_Average values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgCompare  -> Text , 1); if (Para.EdgPara.iCompare  != iTemp) { Trace("Operator" , "Both Lin_Compare values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgChkPxLow -> Text , 0); if (Para.EdgPara.iChkPxLow != iTemp) { Trace("Operator" , "Both Lin_ChkPxLow values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgChkPxHgh -> Text , 0); if (Para.EdgPara.iChkPxHgh != iTemp) { Trace("Operator" , "Both Lin_ChkPxHgh values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgOfsGap   -> Text , 0); if (Para.EdgPara.iOfsGap   != iTemp) { Trace("Operator" , "Both Lin_OfsGap values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgOfsPxLow -> Text , 0); if (Para.EdgPara.iOfsPxLow != iTemp) { Trace("Operator" , "Both Lin_OfsPxLow values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgOfsPxHgh -> Text , 0); if (Para.EdgPara.iOfsPxHgh != iTemp) { Trace("Operator" , "Both Lin_OfsPxHgh values are different."); }






        Para.iScanDirect = ParaUi.cbScanDirect  -> ItemIndex       ;
        Para.bLtToDk     = ParaUi.cbLtToDk      -> Checked         ;
        Para.iSampleGap  = StrToIntDef(ParaUi.edSampleGap   -> Text , 5 ) ;
        Para.iLineGap    = StrToIntDef(ParaUi.edLineGap     -> Text , 1 ) ;
        Para.iEndOffset  = StrToIntDef(ParaUi.edEndOffset   -> Text , 0 ) ;
        Para.iCntrOffset = StrToIntDef(ParaUi.edCntrOffset  -> Text , 0 ) ;

        //Edge.
        Para.EdgPara.iAverage  = StrToIntDef(ParaUi.edEdgAverage  -> Text , 1);
        Para.EdgPara.iCompare  = StrToIntDef(ParaUi.edEdgCompare  -> Text , 1);
        Para.EdgPara.iChkPxLow = StrToIntDef(ParaUi.edEdgChkPxLow -> Text , 0);
        Para.EdgPara.iChkPxHgh = StrToIntDef(ParaUi.edEdgChkPxHgh -> Text , 0);
        Para.EdgPara.iOfsGap   = StrToIntDef(ParaUi.edEdgOfsGap   -> Text , 0);
        Para.EdgPara.iOfsPxLow = StrToIntDef(ParaUi.edEdgOfsPxLow -> Text , 0);
        Para.EdgPara.iOfsPxHgh = StrToIntDef(ParaUi.edEdgOfsPxHgh -> Text , 0);
        ApplyPara(true);
    }

}

bool CLin::Process(IImage * _pImg , LIN_Rslt * _pRslt)
{
    bool bRet ;
    bRet = LIN_Inspect(_pImg , GetOfsRect(Tracker -> GetTRect()) , Para , _pRslt);
    memcpy(&Rslt , _pRslt , sizeof(LIN_Rslt));

    return bRet ;
}

void CLin::PaintRslt(IImage * _pImg , HDC _hHdc)
{
    LIN_PaintRslt(_pImg , _hHdc , &Rslt , Disp);
}


/******************************************************************************/
/* Line Crossed Point                                                         */
/******************************************************************************/
CLcp::CLcp()
{
    m_iProcType = ptLcp ;

    Tracker1 = new TRectTracker() ; Tracker1 -> Caption = "LCP1";
    Tracker2 = new TRectTracker() ; Tracker2 -> Caption = "LCP2";

    //MakeUI(&ParaUi);
    ParaUi.pnBase = NULL ;

    Disp.LinDisp1.clLine = clYellow ;
    Disp.LinDisp1.clEdge = clLime   ;
    Disp.LinDisp1.clText = clYellow ;

    Disp.LinDisp2.clLine = clYellow ;
    Disp.LinDisp2.clEdge = clLime   ;
    Disp.LinDisp2.clText = clYellow ;

    Disp.clPoint = clLime ;
    Disp.clText  = clLime ;
}

CLcp::~CLcp()
{
    delete Tracker1 ;
    delete Tracker2 ;
}

void CLcp::MakeUI(TParaUi  * _pParaUi , TWinControl * _tcControl)
{
    int iR = 0 ;
    _pParaUi->pnBase = new TPanel(_tcControl  ); _pParaUi->pnBase -> Parent = _tcControl ;

    //Line 1 ---------------------------------
    _pParaUi->lbScanDirect1 = new TLabel   (_pParaUi->pnBase); _pParaUi->cbScanDirect1 = new TComboBox(_pParaUi->pnBase);
    _pParaUi->cbLtToDk1     = new TCheckBox(_pParaUi->pnBase);
    _pParaUi->lbSampleGap1  = new TLabel   (_pParaUi->pnBase); _pParaUi->edSampleGap1  = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbLineGap1    = new TLabel   (_pParaUi->pnBase); _pParaUi->edLineGap1    = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbEndOffset1  = new TLabel   (_pParaUi->pnBase); _pParaUi->edEndOffset1  = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbCntrOffset1 = new TLabel   (_pParaUi->pnBase); _pParaUi->edCntrOffset1 = new TEdit    (_pParaUi->pnBase);

    SetArrange(_pParaUi->lbScanDirect1 , 0 , iR );  SetArrange(_pParaUi->cbScanDirect1 , 1 , iR++ );
    SetArrange(_pParaUi->cbLtToDk1     , 0 , iR++ );
    SetArrange(_pParaUi->lbSampleGap1  , 0 , iR );  SetArrange(_pParaUi->edSampleGap1  , 1 , iR++ );
    SetArrange(_pParaUi->lbLineGap1    , 0 , iR );  SetArrange(_pParaUi->edLineGap1    , 1 , iR++ );
    SetArrange(_pParaUi->lbEndOffset1  , 0 , iR );  SetArrange(_pParaUi->edEndOffset1  , 1 , iR++ );
    SetArrange(_pParaUi->lbCntrOffset1 , 0 , iR );  SetArrange(_pParaUi->edCntrOffset1 , 1 , iR++ );

    _pParaUi->lbScanDirect1 -> Parent = _pParaUi->pnBase ; _pParaUi->cbScanDirect1 -> Parent = _pParaUi->pnBase ;
    _pParaUi->cbLtToDk1     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbSampleGap1  -> Parent = _pParaUi->pnBase ; _pParaUi->edSampleGap1  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbLineGap1    -> Parent = _pParaUi->pnBase ; _pParaUi->edLineGap1    -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEndOffset1  -> Parent = _pParaUi->pnBase ; _pParaUi->edEndOffset1  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbCntrOffset1 -> Parent = _pParaUi->pnBase ; _pParaUi->edCntrOffset1 -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbScanDirect1 -> Caption = "1검색방향           ";//"ScanDirect1" ;
    _pParaUi->cbLtToDk1     -> Caption = "1밝은곳에서어두운곳 ";//"LtToDk1    " ;
    _pParaUi->lbSampleGap1  -> Caption = "1샘플링 간격        ";//"SampleGap1 " ;
    _pParaUi->lbLineGap1    -> Caption = "1라인인정범위       ";//"LineGap1   " ;
    _pParaUi->lbEndOffset1  -> Caption = "1검색 시작끝 오프셑 ";//"EndOffset1 " ;
    _pParaUi->lbCntrOffset1 -> Caption = "1검색 중간 오프셑   ";//"CntrOffset1" ;

    //페어런츠가 없으면 애드가 안되는듯.
    //ShowParaForm으로 이동.
    /*
    _pParaUi->cbScanDirect -> AddItem("Down" ,NULL);
    _pParaUi->cbScanDirect -> AddItem("Up"   ,NULL);
    _pParaUi->cbScanDirect -> AddItem("Right",NULL);
    _pParaUi->cbScanDirect -> AddItem("Left" ,NULL);
    _pParaUi->cbScanDirect -> ItemIndex = 0 ;
    */

    //ParaUi.cbScanDirect1 -> Items->Clear();
    //ParaUi.cbScanDirect1 -> AddItem("Down" ,NULL);
    //ParaUi.cbScanDirect1 -> AddItem("Up"   ,NULL);
    ParaUi.cbScanDirect1 -> AddItem("Right",NULL);
    ParaUi.cbScanDirect1 -> AddItem("Left" ,NULL);
    ParaUi.cbScanDirect1 -> ItemIndex = Para.LinPara1.iScanDirect - 2;


    //Edge
    _pParaUi->lbEdgAverage1   = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgAverage1  = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgCompare1   = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgCompare1  = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgChkPxLow1  = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgChkPxLow1 = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgChkPxHgh1  = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgChkPxHgh1 = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgOfsGap1    = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgOfsGap1   = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgOfsPxLow1  = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgOfsPxLow1 = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgOfsPxHgh1  = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgOfsPxHgh1 = new TEdit (_pParaUi->pnBase);

    iR++;
    SetArrange(_pParaUi->lbEdgAverage1  , 0 , iR); SetArrange(_pParaUi->edEdgAverage1   , 1 , iR++);
    SetArrange(_pParaUi->lbEdgCompare1  , 0 , iR); SetArrange(_pParaUi->edEdgCompare1   , 1 , iR++);
    SetArrange(_pParaUi->lbEdgChkPxLow1 , 0 , iR); SetArrange(_pParaUi->edEdgChkPxLow1  , 1 , iR++);
    SetArrange(_pParaUi->lbEdgChkPxHgh1 , 0 , iR); SetArrange(_pParaUi->edEdgChkPxHgh1  , 1 , iR++);
    SetArrange(_pParaUi->lbEdgOfsGap1   , 0 , iR); SetArrange(_pParaUi->edEdgOfsGap1    , 1 , iR++);
    SetArrange(_pParaUi->lbEdgOfsPxLow1 , 0 , iR); SetArrange(_pParaUi->edEdgOfsPxLow1  , 1 , iR++);
    SetArrange(_pParaUi->lbEdgOfsPxHgh1 , 0 , iR); SetArrange(_pParaUi->edEdgOfsPxHgh1  , 1 , iR++);

    _pParaUi->lbEdgAverage1  -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgAverage1  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgCompare1  -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgCompare1  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgChkPxLow1 -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgChkPxLow1 -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgChkPxHgh1 -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgChkPxHgh1 -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgOfsGap1   -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgOfsGap1   -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgOfsPxLow1 -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgOfsPxLow1 -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgOfsPxHgh1 -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgOfsPxHgh1 -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbEdgAverage1  -> Caption = "1평균범위        ";//"Average " ;
    _pParaUi->lbEdgCompare1  -> Caption = "1비교범위        ";//"Compare " ;
    _pParaUi->lbEdgChkPxLow1 -> Caption = "1확정픽셀Low     ";//"ChkPxLow" ;
    _pParaUi->lbEdgChkPxHgh1 -> Caption = "1확정픽셀High    ";//"ChkPxHgh" ;
    _pParaUi->lbEdgOfsGap1   -> Caption = "1확정옵셑거리    ";//"OfsGap  " ;
    _pParaUi->lbEdgOfsPxLow1 -> Caption = "1확정옵셑거리Low ";//"OfsPxLow" ;
    _pParaUi->lbEdgOfsPxHgh1 -> Caption = "1확정옵셑거리High";//"OfsPxHgh" ;


    //Line 2 ---------------------------------
    iR++;
    iR++;
    _pParaUi->lbScanDirect2 = new TLabel   (_pParaUi->pnBase); _pParaUi->cbScanDirect2 = new TComboBox(_pParaUi->pnBase);
    _pParaUi->cbLtToDk2     = new TCheckBox(_pParaUi->pnBase);
    _pParaUi->lbSampleGap2  = new TLabel   (_pParaUi->pnBase); _pParaUi->edSampleGap2  = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbLineGap2    = new TLabel   (_pParaUi->pnBase); _pParaUi->edLineGap2    = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbEndOffset2  = new TLabel   (_pParaUi->pnBase); _pParaUi->edEndOffset2  = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbCntrOffset2 = new TLabel   (_pParaUi->pnBase); _pParaUi->edCntrOffset2 = new TEdit    (_pParaUi->pnBase);

    SetArrange(_pParaUi->lbScanDirect2 , 0 , iR );  SetArrange(_pParaUi->cbScanDirect2 , 1 , iR++ );
    SetArrange(_pParaUi->cbLtToDk2     , 0 , iR++ );
    SetArrange(_pParaUi->lbSampleGap2  , 0 , iR );  SetArrange(_pParaUi->edSampleGap2  , 1 , iR++ );
    SetArrange(_pParaUi->lbLineGap2    , 0 , iR );  SetArrange(_pParaUi->edLineGap2    , 1 , iR++ );
    SetArrange(_pParaUi->lbEndOffset2  , 0 , iR );  SetArrange(_pParaUi->edEndOffset2  , 1 , iR++ );
    SetArrange(_pParaUi->lbCntrOffset2 , 0 , iR );  SetArrange(_pParaUi->edCntrOffset2 , 1 , iR++ );

    _pParaUi->lbScanDirect2 -> Parent = _pParaUi->pnBase ; _pParaUi->cbScanDirect2 -> Parent = _pParaUi->pnBase ;
    _pParaUi->cbLtToDk2     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbSampleGap2  -> Parent = _pParaUi->pnBase ; _pParaUi->edSampleGap2  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbLineGap2    -> Parent = _pParaUi->pnBase ; _pParaUi->edLineGap2    -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEndOffset2  -> Parent = _pParaUi->pnBase ; _pParaUi->edEndOffset2  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbCntrOffset2 -> Parent = _pParaUi->pnBase ; _pParaUi->edCntrOffset2 -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbScanDirect2 -> Caption = "2검색방향          ";//"ScanDirect2" ;
    _pParaUi->cbLtToDk2     -> Caption = "2밝은곳에서어두운곳";//"LtToDk2    " ;
    _pParaUi->lbSampleGap2  -> Caption = "2샘플링 간격       ";//"SampleGap2 " ;
    _pParaUi->lbLineGap2    -> Caption = "2라인인정범위      ";//"LineGap2   " ;
    _pParaUi->lbEndOffset2  -> Caption = "2검색 시작끝 오프셑";//"EndOffset2 " ;
    _pParaUi->lbCntrOffset2 -> Caption = "2검색 중간 오프셑  ";//"CntrOffset2" ;

    //ParaUi.cbScanDirect2 -> Items->Clear();
    ParaUi.cbScanDirect2 -> AddItem("Down" ,NULL);
    ParaUi.cbScanDirect2 -> AddItem("Up"   ,NULL);
    //ParaUi.cbScanDirect2 -> AddItem("Right",NULL);
    //ParaUi.cbScanDirect2 -> AddItem("Left" ,NULL);
    ParaUi.cbScanDirect2 -> ItemIndex = Para.LinPara2.iScanDirect ;


    //Edge
    _pParaUi->lbEdgAverage2   = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgAverage2  = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgCompare2   = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgCompare2  = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgChkPxLow2  = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgChkPxLow2 = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgChkPxHgh2  = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgChkPxHgh2 = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgOfsGap2    = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgOfsGap2   = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgOfsPxLow2  = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgOfsPxLow2 = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgOfsPxHgh2  = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgOfsPxHgh2 = new TEdit (_pParaUi->pnBase);

    iR++;
    SetArrange(_pParaUi->lbEdgAverage2  , 0 , iR); SetArrange(_pParaUi->edEdgAverage2   , 1 , iR++);
    SetArrange(_pParaUi->lbEdgCompare2  , 0 , iR); SetArrange(_pParaUi->edEdgCompare2   , 1 , iR++);
    SetArrange(_pParaUi->lbEdgChkPxLow2 , 0 , iR); SetArrange(_pParaUi->edEdgChkPxLow2  , 1 , iR++);
    SetArrange(_pParaUi->lbEdgChkPxHgh2 , 0 , iR); SetArrange(_pParaUi->edEdgChkPxHgh2  , 1 , iR++);
    SetArrange(_pParaUi->lbEdgOfsGap2   , 0 , iR); SetArrange(_pParaUi->edEdgOfsGap2    , 1 , iR++);
    SetArrange(_pParaUi->lbEdgOfsPxLow2 , 0 , iR); SetArrange(_pParaUi->edEdgOfsPxLow2  , 1 , iR++);
    SetArrange(_pParaUi->lbEdgOfsPxHgh2 , 0 , iR); SetArrange(_pParaUi->edEdgOfsPxHgh2  , 1 , iR++);

    _pParaUi->lbEdgAverage2  -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgAverage2  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgCompare2  -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgCompare2  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgChkPxLow2 -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgChkPxLow2 -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgChkPxHgh2 -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgChkPxHgh2 -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgOfsGap2   -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgOfsGap2   -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgOfsPxLow2 -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgOfsPxLow2 -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgOfsPxHgh2 -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgOfsPxHgh2 -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbEdgAverage2  -> Caption = "2평균범위        ";//"Average " ;
    _pParaUi->lbEdgCompare2  -> Caption = "2비교범위        ";//"Compare " ;
    _pParaUi->lbEdgChkPxLow2 -> Caption = "2확정픽셀Low     ";//"ChkPxLow" ;
    _pParaUi->lbEdgChkPxHgh2 -> Caption = "2확정픽셀High    ";//"ChkPxHgh" ;
    _pParaUi->lbEdgOfsGap2   -> Caption = "2확정옵셑거리    ";//"OfsGap  " ;
    _pParaUi->lbEdgOfsPxLow2 -> Caption = "2확정옵셑거리Low ";//"OfsPxLow" ;
    _pParaUi->lbEdgOfsPxHgh2 -> Caption = "2확정옵셑거리High";//"OfsPxHgh" ;
}

void CLcp::SetArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    const int iControlWidth   = 120 ;
    const int iControlHeight  = 20  ;
    const int iStartRowOffset = 5   ;
    const int iStartColOffset = 5   ;

    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;
}

void CLcp::SetPaintCallback(TOnChanged ChangeFunc)
{
    Tracker1 -> SetFunc(ChangeFunc) ;
    Tracker2 -> SetFunc(ChangeFunc) ;
}

void CLcp::SetTrackerVisible(bool _bVisible)
{
    Tracker1 -> Visible = _bVisible ;
    Tracker2 -> Visible = _bVisible ;
}



void CLcp::SetTrackerEnabled(bool _bEnabled)
{
    Tracker1 -> Enabled = _bEnabled ;
    Tracker2 -> Enabled = _bEnabled ;
}

bool CLcp::MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker1 -> Focused = false ;
    Tracker2 -> Focused = false ;

    if(Tracker1 -> TrackerDw(Button, Shift , X,Y, _fScaleX , _fScaleY)) { m_fInspOfsX = 0.0 ; m_fInspOfsY = 0.0 ; return true ;}
    if(Tracker2 -> TrackerDw(Button, Shift , X,Y, _fScaleX , _fScaleY)) { m_fInspOfsX = 0.0 ; m_fInspOfsY = 0.0 ; return true ;}

    return false ;
}

bool CLcp::MouseTrackerMv(TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    if(Tracker1 -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY)) return true ;
    if(Tracker2 -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY)) return true ;

    return false ;
}

void CLcp::MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker1 -> TrackerUp(Button, Shift );
    Tracker2 -> TrackerUp(Button, Shift );
}

void CLcp::PaintTracker(HDC _hHdc , float _fScaleX , float _fScaleY)
{
    if(Tracker1 -> Visible)PaintOfsRect(_hHdc , _fScaleX , _fScaleY , GetOfsRect(Tracker1 -> GetTRect()));
    if(Tracker2 -> Visible)PaintOfsRect(_hHdc , _fScaleX , _fScaleY , GetOfsRect(Tracker2 -> GetTRect()));

    Tracker1 -> Paint(_hHdc , _fScaleX , _fScaleY);
    Tracker2 -> Paint(_hHdc , _fScaleX , _fScaleY);
}

void CLcp::ShowParaForm(TWinControl* _wcBase)
{
    if(ParaUi.pnBase == NULL)MakeUI(&ParaUi,_wcBase) ;

    ParaUi.pnBase -> Parent = _wcBase ;
    ParaUi.pnBase -> Align = alClient ;
}

void CLcp::LoadPara(bool _bLoad , AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId+"_LineCrossPnt" ;

    AnsiString sFilePath = _sFilePath + ".ini" ;
    AnsiString sTrakPath = _sFilePath + "Tracker.ini" ;

    Tracker1 -> OnLoadSave(_bLoad , sTrakPath , sTemp + "_Tracker1");
    Tracker2 -> OnLoadSave(_bLoad , sTrakPath , sTemp + "_Tracker2");

    int iTemp ;

    if(_bLoad) {
        //Line1
        UserINI.Load(sFilePath, sTemp , "iScanDirect1", iTemp                    );Para.LinPara1.iScanDirect=iTemp ;
        UserINI.Load(sFilePath, sTemp , "bLtToDk1    ", Para.LinPara1.bLtToDk    );
        UserINI.Load(sFilePath, sTemp , "iSampleGap1 ", Para.LinPara1.iSampleGap );
        UserINI.Load(sFilePath, sTemp , "iLineGap1   ", Para.LinPara1.iLineGap   );
        UserINI.Load(sFilePath, sTemp , "iEndOffset1 ", Para.LinPara1.iEndOffset );
        UserINI.Load(sFilePath, sTemp , "iCntrOffset1", Para.LinPara1.iCntrOffset);

        //Edge.
        UserINI.Load(sFilePath, sTemp , "iEdgAverage1 ", Para.LinPara1.EdgPara.iAverage );
        UserINI.Load(sFilePath, sTemp , "iEdgCompare1 ", Para.LinPara1.EdgPara.iCompare );
        UserINI.Load(sFilePath, sTemp , "iEdgChkPxLow1", Para.LinPara1.EdgPara.iChkPxLow);
        UserINI.Load(sFilePath, sTemp , "iEdgChkPxHgh1", Para.LinPara1.EdgPara.iChkPxHgh);
        UserINI.Load(sFilePath, sTemp , "iEdgOfsGap1  ", Para.LinPara1.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "iEdgOfsPxLow1", Para.LinPara1.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "iEdgOfsPxHgh1", Para.LinPara1.EdgPara.iOfsPxHgh);

        //Line2
        UserINI.Load(sFilePath, sTemp , "iScanDirect2", iTemp                    );Para.LinPara2.iScanDirect=iTemp ;
        UserINI.Load(sFilePath, sTemp , "bLtToDk2    ", Para.LinPara2.bLtToDk    );
        UserINI.Load(sFilePath, sTemp , "iSampleGap2 ", Para.LinPara2.iSampleGap );
        UserINI.Load(sFilePath, sTemp , "iLineGap2   ", Para.LinPara2.iLineGap   );
        UserINI.Load(sFilePath, sTemp , "iEndOffset2 ", Para.LinPara2.iEndOffset );
        UserINI.Load(sFilePath, sTemp , "iCntrOffset2", Para.LinPara2.iCntrOffset);

        //Edge.
        UserINI.Load(sFilePath, sTemp , "iEdgAverage2 ", Para.LinPara2.EdgPara.iAverage );
        UserINI.Load(sFilePath, sTemp , "iEdgCompare2 ", Para.LinPara2.EdgPara.iCompare );
        UserINI.Load(sFilePath, sTemp , "iEdgChkPxLow2", Para.LinPara2.EdgPara.iChkPxLow);
        UserINI.Load(sFilePath, sTemp , "iEdgChkPxHgh2", Para.LinPara2.EdgPara.iChkPxHgh);
        UserINI.Load(sFilePath, sTemp , "iEdgOfsGap2  ", Para.LinPara2.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "iEdgOfsPxLow2", Para.LinPara2.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "iEdgOfsPxHgh2", Para.LinPara2.EdgPara.iOfsPxHgh);

        UserINI.Load(sFilePath, sTemp , "m_fRefValX", m_fRefValX);
        UserINI.Load(sFilePath, sTemp , "m_fRefValY", m_fRefValY);
        UserINI.Load(sFilePath, sTemp , "m_fRefValT", m_fRefValT);
    }
    else {
        //Line1
        UserINI.Save(sFilePath, sTemp , "iScanDirect1", Para.LinPara1.iScanDirect);
        UserINI.Save(sFilePath, sTemp , "bLtToDk1    ", Para.LinPara1.bLtToDk    );
        UserINI.Save(sFilePath, sTemp , "iSampleGap1 ", Para.LinPara1.iSampleGap );
        UserINI.Save(sFilePath, sTemp , "iLineGap1   ", Para.LinPara1.iLineGap   );
        UserINI.Save(sFilePath, sTemp , "iEndOffset1 ", Para.LinPara1.iEndOffset );
        UserINI.Save(sFilePath, sTemp , "iCntrOffset1", Para.LinPara1.iCntrOffset);

        //Edge.
        UserINI.Save(sFilePath, sTemp , "iEdgAverage1 ", Para.LinPara1.EdgPara.iAverage );
        UserINI.Save(sFilePath, sTemp , "iEdgCompare1 ", Para.LinPara1.EdgPara.iCompare );
        UserINI.Save(sFilePath, sTemp , "iEdgChkPxLow1", Para.LinPara1.EdgPara.iChkPxLow);
        UserINI.Save(sFilePath, sTemp , "iEdgChkPxHgh1", Para.LinPara1.EdgPara.iChkPxHgh);
        UserINI.Save(sFilePath, sTemp , "iEdgOfsGap1  ", Para.LinPara1.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "iEdgOfsPxLow1", Para.LinPara1.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "iEdgOfsPxHgh1", Para.LinPara1.EdgPara.iOfsPxHgh);

        UserINI.Save(sFilePath, sTemp , "iEdgOfsPxLow1", Para.LinPara1.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "iEdgOfsPxHgh1", Para.LinPara1.EdgPara.iOfsPxHgh);

        //Line2
        UserINI.Save(sFilePath, sTemp , "iScanDirect2", Para.LinPara2.iScanDirect);
        UserINI.Save(sFilePath, sTemp , "bLtToDk2    ", Para.LinPara2.bLtToDk    );
        UserINI.Save(sFilePath, sTemp , "iSampleGap2 ", Para.LinPara2.iSampleGap );
        UserINI.Save(sFilePath, sTemp , "iLineGap2   ", Para.LinPara2.iLineGap   );
        UserINI.Save(sFilePath, sTemp , "iEndOffset2 ", Para.LinPara2.iEndOffset );
        UserINI.Save(sFilePath, sTemp , "iCntrOffset2", Para.LinPara2.iCntrOffset);

        //Edge.
        UserINI.Save(sFilePath, sTemp , "iEdgAverage2 ", Para.LinPara2.EdgPara.iAverage );
        UserINI.Save(sFilePath, sTemp , "iEdgCompare2 ", Para.LinPara2.EdgPara.iCompare );
        UserINI.Save(sFilePath, sTemp , "iEdgChkPxLow2", Para.LinPara2.EdgPara.iChkPxLow);
        UserINI.Save(sFilePath, sTemp , "iEdgChkPxHgh2", Para.LinPara2.EdgPara.iChkPxHgh);
        UserINI.Save(sFilePath, sTemp , "iEdgOfsGap2  ", Para.LinPara2.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "iEdgOfsPxLow2", Para.LinPara2.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "iEdgOfsPxHgh2", Para.LinPara2.EdgPara.iOfsPxHgh);

        //여기에 레퍼런스 저장 하는 것..... 세이브 한번 해야지만 결과값을 뽑을수 있는 파라가 저장 되고 1번 검사를 해야 레퍼런스를 세팅 할수 있다.
        m_fRefValX = Rslt.CrossPnt.x ;
        m_fRefValY = Rslt.CrossPnt.y ;
        m_fRefValT = Rslt.LinRslt1.fAngle ;
        UserINI.Save(sFilePath, sTemp , "m_fRefValX", m_fRefValX);
        UserINI.Save(sFilePath, sTemp , "m_fRefValY", m_fRefValY);
        UserINI.Save(sFilePath, sTemp , "m_fRefValT", m_fRefValT);
    }

}


void CLcp::ApplyPara(bool _bTable)
{
    if(_bTable) {
        //Line1
        ParaUi.cbLtToDk1      -> Checked   = Para.LinPara1.bLtToDk           ;
        ParaUi.edSampleGap1   -> Text      = Para.LinPara1.iSampleGap        ;
        ParaUi.edLineGap1     -> Text      = Para.LinPara1.iLineGap          ;
        ParaUi.edEndOffset1   -> Text      = Para.LinPara1.iEndOffset        ;
        ParaUi.edCntrOffset1  -> Text      = Para.LinPara1.iCntrOffset       ;

        //Edge.
        ParaUi.edEdgAverage1  -> Text      = Para.LinPara1.EdgPara.iAverage  ;
        ParaUi.edEdgCompare1  -> Text      = Para.LinPara1.EdgPara.iCompare  ;
        ParaUi.edEdgChkPxLow1 -> Text      = Para.LinPara1.EdgPara.iChkPxLow ;
        ParaUi.edEdgChkPxHgh1 -> Text      = Para.LinPara1.EdgPara.iChkPxHgh ;
        ParaUi.edEdgOfsGap1   -> Text      = Para.LinPara1.EdgPara.iOfsGap   ;
        ParaUi.edEdgOfsPxLow1 -> Text      = Para.LinPara1.EdgPara.iOfsPxLow ;
        ParaUi.edEdgOfsPxHgh1 -> Text      = Para.LinPara1.EdgPara.iOfsPxHgh ;

        //Line2
        //ParaUi.cbScanDirect  -> ItemIndex = Para.LinPara2.iScanDirect       ;
        ParaUi.cbLtToDk2      -> Checked   = Para.LinPara2.bLtToDk           ;
        ParaUi.edSampleGap2   -> Text      = Para.LinPara2.iSampleGap        ;
        ParaUi.edLineGap2     -> Text      = Para.LinPara2.iLineGap          ;
        ParaUi.edEndOffset2   -> Text      = Para.LinPara2.iEndOffset        ;
        ParaUi.edCntrOffset2  -> Text      = Para.LinPara2.iCntrOffset       ;

        //Edge.
        ParaUi.edEdgAverage2  -> Text      = Para.LinPara2.EdgPara.iAverage  ;
        ParaUi.edEdgCompare2  -> Text      = Para.LinPara2.EdgPara.iCompare  ;
        ParaUi.edEdgChkPxLow2 -> Text      = Para.LinPara2.EdgPara.iChkPxLow ;
        ParaUi.edEdgChkPxHgh2 -> Text      = Para.LinPara2.EdgPara.iChkPxHgh ;
        ParaUi.edEdgOfsGap2   -> Text      = Para.LinPara2.EdgPara.iOfsGap   ;
        ParaUi.edEdgOfsPxLow2 -> Text      = Para.LinPara2.EdgPara.iOfsPxLow ;
        ParaUi.edEdgOfsPxHgh2 -> Text      = Para.LinPara2.EdgPara.iOfsPxHgh ;
    }
    else {
        int  iTemp ;
        bool bTemp ;
        iTemp = ParaUi.cbScanDirect1  -> ItemIndex + 2        ; if (Para.LinPara1.iScanDirect != iTemp)       { Trace("Operator" , "Both Lcp_ScanDirect1 values are different."); }
        bTemp = ParaUi.cbLtToDk1      -> Checked              ; if (Para.LinPara1.bLtToDk     != bTemp)       { Trace("Operator" , "Both Lcp_LtToDk1 values are different."); }
        iTemp = StrToIntDef(ParaUi.edSampleGap1   -> Text , 5); if (Para.LinPara1.iSampleGap  != iTemp)       { Trace("Operator" , "Both Lcp_SampleGap1 values are different."); }
        iTemp = StrToIntDef(ParaUi.edLineGap1     -> Text , 1); if (Para.LinPara1.iLineGap    != iTemp)       { Trace("Operator" , "Both Lcp_LineGap1 values are different."); }
        iTemp = StrToIntDef(ParaUi.edEndOffset1   -> Text , 0); if (Para.LinPara1.iEndOffset  != iTemp)       { Trace("Operator" , "Both Lcp_EndOffset1 values are different."); }
        iTemp = StrToIntDef(ParaUi.edCntrOffset1  -> Text , 0); if (Para.LinPara1.iCntrOffset != iTemp)       { Trace("Operator" , "Both Lcp_CntrOffset1 values are different."); }

        iTemp = StrToIntDef(ParaUi.edEdgAverage1  -> Text , 1); if (Para.LinPara1.EdgPara.iAverage  != iTemp) { Trace("Operator" , "Both Lcp_EdgAverage1 values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgCompare1  -> Text , 1); if (Para.LinPara1.EdgPara.iCompare  != iTemp) { Trace("Operator" , "Both Lcp_EdgCompare1 values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgChkPxLow1 -> Text , 0); if (Para.LinPara1.EdgPara.iChkPxLow != iTemp) { Trace("Operator" , "Both Lcp_EdgChkPxLow1 values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgChkPxHgh1 -> Text , 0); if (Para.LinPara1.EdgPara.iChkPxHgh != iTemp) { Trace("Operator" , "Both Lcp_EdgChkPxHgh1 values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgOfsGap1   -> Text , 0); if (Para.LinPara1.EdgPara.iOfsGap   != iTemp) { Trace("Operator" , "Both Lcp_EdgOfsGap1 values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgOfsPxLow1 -> Text , 0); if (Para.LinPara1.EdgPara.iOfsPxLow != iTemp) { Trace("Operator" , "Both Lcp_EdgOfsPxLow1 values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgOfsPxHgh1 -> Text , 0); if (Para.LinPara1.EdgPara.iOfsPxHgh != iTemp) { Trace("Operator" , "Both Lcp_EdgOfsPxHgh1 values are different."); }

        iTemp = ParaUi.cbScanDirect2  -> ItemIndex            ; if (Para.LinPara2.iScanDirect != iTemp)       { Trace("Operator" , "Both Lcp_ScanDirect2 values are different."); }
        bTemp = ParaUi.cbLtToDk2      -> Checked              ; if (Para.LinPara2.bLtToDk     != bTemp)       { Trace("Operator" , "Both Lcp_LtToDk2 values are different."); }
        iTemp = StrToIntDef(ParaUi.edSampleGap2   -> Text , 5); if (Para.LinPara2.iSampleGap  != iTemp)       { Trace("Operator" , "Both Lcp_SampleGap2 values are different."); }
        iTemp = StrToIntDef(ParaUi.edLineGap2     -> Text , 1); if (Para.LinPara2.iLineGap    != iTemp)       { Trace("Operator" , "Both Lcp_LineGap2 values are different."); }
        iTemp = StrToIntDef(ParaUi.edEndOffset2   -> Text , 0); if (Para.LinPara2.iEndOffset  != iTemp)       { Trace("Operator" , "Both Lcp_EndOffset2 values are different."); }
        iTemp = StrToIntDef(ParaUi.edCntrOffset2  -> Text , 0); if (Para.LinPara2.iCntrOffset != iTemp)       { Trace("Operator" , "Both Lcp_CntrOffset2 values are different."); }


        iTemp = StrToIntDef(ParaUi.edEdgAverage2  -> Text , 1); if (Para.LinPara1.EdgPara.iAverage  != iTemp) { Trace("Operator" , "Both Lcp_EdgAverage2 values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgCompare2  -> Text , 1); if (Para.LinPara1.EdgPara.iCompare  != iTemp) { Trace("Operator" , "Both Lcp_EdgCompare2 values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgChkPxLow2 -> Text , 0); if (Para.LinPara1.EdgPara.iChkPxLow != iTemp) { Trace("Operator" , "Both Lcp_EdgChkPxLow2 values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgChkPxHgh2 -> Text , 0); if (Para.LinPara1.EdgPara.iChkPxHgh != iTemp) { Trace("Operator" , "Both Lcp_EdgChkPxHgh2 values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgOfsGap2   -> Text , 0); if (Para.LinPara1.EdgPara.iOfsGap   != iTemp) { Trace("Operator" , "Both Lcp_EdgOfsGap2 values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgOfsPxLow2 -> Text , 0); if (Para.LinPara1.EdgPara.iOfsPxLow != iTemp) { Trace("Operator" , "Both Lcp_EdgOfsPxLow2 values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgOfsPxHgh2 -> Text , 0); if (Para.LinPara1.EdgPara.iOfsPxHgh != iTemp) { Trace("Operator" , "Both Lcp_EdgOfsPxHgh2 values are different."); }




        //Line1
        Para.LinPara1.iScanDirect = ParaUi.cbScanDirect1  -> ItemIndex + 2   ;
        Para.LinPara1.bLtToDk     = ParaUi.cbLtToDk1      -> Checked         ;
        Para.LinPara1.iSampleGap  = StrToIntDef(ParaUi.edSampleGap1   -> Text , 5 ) ;
        Para.LinPara1.iLineGap    = StrToIntDef(ParaUi.edLineGap1     -> Text , 1 ) ;
        Para.LinPara1.iEndOffset  = StrToIntDef(ParaUi.edEndOffset1   -> Text , 0 ) ;
        Para.LinPara1.iCntrOffset = StrToIntDef(ParaUi.edCntrOffset1  -> Text , 0 ) ;

        //Edge.
        Para.LinPara1.EdgPara.iAverage  = StrToIntDef(ParaUi.edEdgAverage1  -> Text , 1);
        Para.LinPara1.EdgPara.iCompare  = StrToIntDef(ParaUi.edEdgCompare1  -> Text , 1);
        Para.LinPara1.EdgPara.iChkPxLow = StrToIntDef(ParaUi.edEdgChkPxLow1 -> Text , 0);
        Para.LinPara1.EdgPara.iChkPxHgh = StrToIntDef(ParaUi.edEdgChkPxHgh1 -> Text , 0);
        Para.LinPara1.EdgPara.iOfsGap   = StrToIntDef(ParaUi.edEdgOfsGap1   -> Text , 0);
        Para.LinPara1.EdgPara.iOfsPxLow = StrToIntDef(ParaUi.edEdgOfsPxLow1 -> Text , 0);
        Para.LinPara1.EdgPara.iOfsPxHgh = StrToIntDef(ParaUi.edEdgOfsPxHgh1 -> Text , 0);

        //Line2
        Para.LinPara2.iScanDirect = ParaUi.cbScanDirect2  -> ItemIndex       ;
        Para.LinPara2.bLtToDk     = ParaUi.cbLtToDk2      -> Checked         ;
        Para.LinPara2.iSampleGap  = StrToIntDef(ParaUi.edSampleGap2   -> Text , 5 ) ;
        Para.LinPara2.iLineGap    = StrToIntDef(ParaUi.edLineGap2     -> Text , 1 ) ;
        Para.LinPara2.iEndOffset  = StrToIntDef(ParaUi.edEndOffset2   -> Text , 0 ) ;
        Para.LinPara2.iCntrOffset = StrToIntDef(ParaUi.edCntrOffset2  -> Text , 0 ) ;

        //Edge.
        Para.LinPara2.EdgPara.iAverage  = StrToIntDef(ParaUi.edEdgAverage2  -> Text , 1);
        Para.LinPara2.EdgPara.iCompare  = StrToIntDef(ParaUi.edEdgCompare2  -> Text , 1);
        Para.LinPara2.EdgPara.iChkPxLow = StrToIntDef(ParaUi.edEdgChkPxLow2 -> Text , 0);
        Para.LinPara2.EdgPara.iChkPxHgh = StrToIntDef(ParaUi.edEdgChkPxHgh2 -> Text , 0);
        Para.LinPara2.EdgPara.iOfsGap   = StrToIntDef(ParaUi.edEdgOfsGap2   -> Text , 0);
        Para.LinPara2.EdgPara.iOfsPxLow = StrToIntDef(ParaUi.edEdgOfsPxLow2 -> Text , 0);
        Para.LinPara2.EdgPara.iOfsPxHgh = StrToIntDef(ParaUi.edEdgOfsPxHgh2 -> Text , 0);

        ApplyPara(true);
    }

}

bool CLcp::Process(IImage * _pImg , LCP_Rslt * _pRslt)
{
    bool bRet ;
    bRet = LCP_Inspect(_pImg , GetOfsRect(Tracker1 -> GetTRect()) , GetOfsRect(Tracker2 -> GetTRect()) , Para , _pRslt);
    memcpy(&Rslt , _pRslt , sizeof(LCP_Rslt));

    return bRet ;
}

void CLcp::PaintRslt(IImage * _pImg , HDC _hHdc)
{
    LCP_PaintRslt(_pImg , _hHdc , &Rslt , Disp);
}

void CLcp::GetRefValOfs(float &_fX , float &_fY , float &_fT )
{
    _fX = Rslt.CrossPnt.x      - m_fRefValX  ;
    _fY = Rslt.CrossPnt.y      - m_fRefValY  ;
    _fT = Rslt.LinRslt1.fAngle - m_fRefValT  ; //앵글 조심. 0도 짜리 는 360 대로 떨어질수 있다.
}









/******************************************************************************/
/* Rect                                                                       */
/******************************************************************************/
CRct::CRct()
{
    m_iProcType = ptRct ;

    Tracker = new TRectTracker() ; Tracker -> Caption = "RCT";
    Tracker -> BandVisible = true ;
    Tracker -> Band = 130 ;

    //MakeUI(&ParaUi);
    ParaUi.pnBase = NULL ;

    //Disp.LinDisp.clLine = clLime ;
    Disp.LinDisp.clEdge = clRed ;
    //Disp.LinDisp.clText = clLime ;
    Disp.clPoint = clGreen;
    Disp.clText  = clLime ;
    Disp.clRect  = clLime ;

}

CRct::~CRct()
{
    delete Tracker ;
}

void CRct::MakeUI(TParaUi  * _pParaUi , TWinControl * _tcControl)
{
    int iR = 0 ;
    _pParaUi->pnBase = new TPanel(_tcControl  ); _pParaUi->pnBase -> Parent = _tcControl ;

    //Line 1 ---------------------------------
    _pParaUi->cbLtOnDk     = new TCheckBox(_pParaUi->pnBase);
    _pParaUi->lbSampleGap  = new TLabel   (_pParaUi->pnBase);  _pParaUi->edSampleGap   = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbLineGap    = new TLabel   (_pParaUi->pnBase);  _pParaUi->edLineGap     = new TEdit(_pParaUi->pnBase);

    _pParaUi->lbChkOfs     = new TLabel   (_pParaUi->pnBase);  _pParaUi->edChkOfs      = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbLowThsd    = new TLabel   (_pParaUi->pnBase);  _pParaUi->edLowThsd     = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbHighThsd   = new TLabel   (_pParaUi->pnBase);  _pParaUi->edHighThsd    = new TEdit(_pParaUi->pnBase);

    _pParaUi->lbAverage    = new TLabel   (_pParaUi->pnBase);  _pParaUi->edAverage     = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbCompare    = new TLabel   (_pParaUi->pnBase);  _pParaUi->edCompare     = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbChkPxLow   = new TLabel   (_pParaUi->pnBase);  _pParaUi->edChkPxLow    = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbChkPxHgh   = new TLabel   (_pParaUi->pnBase);  _pParaUi->edChkPxHgh    = new TEdit(_pParaUi->pnBase);

    _pParaUi->lbEndOffsetL = new TLabel   (_pParaUi->pnBase);  _pParaUi->edEndOffsetL  = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbCntrOffsetL= new TLabel   (_pParaUi->pnBase);  _pParaUi->edCntrOffsetL = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsGapL    = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsGapL     = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsPxLowL  = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsPxLowL   = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsPxHghL  = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsPxHghL   = new TEdit(_pParaUi->pnBase);

    _pParaUi->lbEndOffsetT = new TLabel   (_pParaUi->pnBase);  _pParaUi->edEndOffsetT  = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbCntrOffsetT= new TLabel   (_pParaUi->pnBase);  _pParaUi->edCntrOffsetT = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsGapT    = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsGapT     = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsPxLowT  = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsPxLowT   = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsPxHghT  = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsPxHghT   = new TEdit(_pParaUi->pnBase);

    _pParaUi->lbEndOffsetR = new TLabel   (_pParaUi->pnBase);  _pParaUi->edEndOffsetR  = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbCntrOffsetR= new TLabel   (_pParaUi->pnBase);  _pParaUi->edCntrOffsetR = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsGapR    = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsGapR     = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsPxLowR  = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsPxLowR   = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsPxHghR  = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsPxHghR   = new TEdit(_pParaUi->pnBase);

    _pParaUi->lbEndOffsetB = new TLabel   (_pParaUi->pnBase);  _pParaUi->edEndOffsetB  = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbCntrOffsetB= new TLabel   (_pParaUi->pnBase);  _pParaUi->edCntrOffsetB = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsGapB    = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsGapB     = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsPxLowB  = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsPxLowB   = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsPxHghB  = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsPxHghB   = new TEdit(_pParaUi->pnBase);

    SetArrange(_pParaUi->cbLtOnDk      , 0 , iR++);
    SetArrange(_pParaUi->lbSampleGap   , 0 , iR  );  SetArrange(_pParaUi->edSampleGap   , 1 , iR++ );
    SetArrange(_pParaUi->lbLineGap     , 0 , iR  );  SetArrange(_pParaUi->edLineGap     , 1 , iR++ );

    SetArrange(_pParaUi->lbChkOfs      , 0 , iR  );  SetArrange(_pParaUi->edChkOfs      , 1 , iR++ );
    SetArrange(_pParaUi->lbLowThsd     , 0 , iR  );  SetArrange(_pParaUi->edLowThsd     , 1 , iR++ );
    SetArrange(_pParaUi->lbHighThsd    , 0 , iR  );  SetArrange(_pParaUi->edHighThsd    , 1 , iR++ );

    SetArrange(_pParaUi->lbAverage     , 0 , iR  );  SetArrange(_pParaUi->edAverage     , 1 , iR++ );
    SetArrange(_pParaUi->lbCompare     , 0 , iR  );  SetArrange(_pParaUi->edCompare     , 1 , iR++ );
    SetArrange(_pParaUi->lbChkPxLow    , 0 , iR  );  SetArrange(_pParaUi->edChkPxLow    , 1 , iR++ );
    SetArrange(_pParaUi->lbChkPxHgh    , 0 , iR  );  SetArrange(_pParaUi->edChkPxHgh    , 1 , iR++ );

    SetArrange(_pParaUi->lbEndOffsetL  , 0 , iR  );  SetArrange(_pParaUi->edEndOffsetL  , 1 , iR++ );
    SetArrange(_pParaUi->lbCntrOffsetL , 0 , iR  );  SetArrange(_pParaUi->edCntrOffsetL , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsGapL     , 0 , iR  );  SetArrange(_pParaUi->edOfsGapL     , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsPxLowL   , 0 , iR  );  SetArrange(_pParaUi->edOfsPxLowL   , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsPxHghL   , 0 , iR  );  SetArrange(_pParaUi->edOfsPxHghL   , 1 , iR++ );

    SetArrange(_pParaUi->lbEndOffsetT  , 0 , iR  );  SetArrange(_pParaUi->edEndOffsetT  , 1 , iR++ );
    SetArrange(_pParaUi->lbCntrOffsetT , 0 , iR  );  SetArrange(_pParaUi->edCntrOffsetT , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsGapT     , 0 , iR  );  SetArrange(_pParaUi->edOfsGapT     , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsPxLowT   , 0 , iR  );  SetArrange(_pParaUi->edOfsPxLowT   , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsPxHghT   , 0 , iR  );  SetArrange(_pParaUi->edOfsPxHghT   , 1 , iR++ );

    SetArrange(_pParaUi->lbEndOffsetR  , 0 , iR  );  SetArrange(_pParaUi->edEndOffsetR  , 1 , iR++ );
    SetArrange(_pParaUi->lbCntrOffsetR , 0 , iR  );  SetArrange(_pParaUi->edCntrOffsetR , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsGapR     , 0 , iR  );  SetArrange(_pParaUi->edOfsGapR     , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsPxLowR   , 0 , iR  );  SetArrange(_pParaUi->edOfsPxLowR   , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsPxHghR   , 0 , iR  );  SetArrange(_pParaUi->edOfsPxHghR   , 1 , iR++ );

    SetArrange(_pParaUi->lbEndOffsetB  , 0 , iR  );  SetArrange(_pParaUi->edEndOffsetB  , 1 , iR++ );
    SetArrange(_pParaUi->lbCntrOffsetB , 0 , iR  );  SetArrange(_pParaUi->edCntrOffsetB , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsGapB     , 0 , iR  );  SetArrange(_pParaUi->edOfsGapB     , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsPxLowB   , 0 , iR  );  SetArrange(_pParaUi->edOfsPxLowB   , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsPxHghB   , 0 , iR  );  SetArrange(_pParaUi->edOfsPxHghB   , 1 , iR++ );

    _pParaUi->cbLtOnDk      -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbSampleGap   -> Parent = _pParaUi->pnBase ; _pParaUi->edSampleGap   -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbLineGap     -> Parent = _pParaUi->pnBase ; _pParaUi->edLineGap     -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbChkOfs      -> Parent = _pParaUi->pnBase ; _pParaUi->edChkOfs      -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbLowThsd     -> Parent = _pParaUi->pnBase ; _pParaUi->edLowThsd     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbHighThsd    -> Parent = _pParaUi->pnBase ; _pParaUi->edHighThsd    -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbAverage     -> Parent = _pParaUi->pnBase ; _pParaUi->edAverage     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbCompare     -> Parent = _pParaUi->pnBase ; _pParaUi->edCompare     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbChkPxLow    -> Parent = _pParaUi->pnBase ; _pParaUi->edChkPxLow    -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbChkPxHgh    -> Parent = _pParaUi->pnBase ; _pParaUi->edChkPxHgh    -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbEndOffsetL  -> Parent = _pParaUi->pnBase ; _pParaUi->edEndOffsetL  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbCntrOffsetL -> Parent = _pParaUi->pnBase ; _pParaUi->edCntrOffsetL -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsGapL     -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsGapL     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsPxLowL   -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsPxLowL   -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsPxHghL   -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsPxHghL   -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbEndOffsetT  -> Parent = _pParaUi->pnBase ; _pParaUi->edEndOffsetT  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbCntrOffsetT -> Parent = _pParaUi->pnBase ; _pParaUi->edCntrOffsetT -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsGapT     -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsGapT     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsPxLowT   -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsPxLowT   -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsPxHghT   -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsPxHghT   -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbEndOffsetR  -> Parent = _pParaUi->pnBase ; _pParaUi->edEndOffsetR  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbCntrOffsetR -> Parent = _pParaUi->pnBase ; _pParaUi->edCntrOffsetR -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsGapR     -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsGapR     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsPxLowR   -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsPxLowR   -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsPxHghR   -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsPxHghR   -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbEndOffsetB  -> Parent = _pParaUi->pnBase ; _pParaUi->edEndOffsetB  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbCntrOffsetB -> Parent = _pParaUi->pnBase ; _pParaUi->edCntrOffsetB -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsGapB     -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsGapB     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsPxLowB   -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsPxLowB   -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsPxHghB   -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsPxHghB   -> Parent = _pParaUi->pnBase ;

    _pParaUi->cbLtOnDk      -> Caption = "밝은물체찾기 ";//"bLtOnDk     " ;
    _pParaUi->lbSampleGap   -> Caption = "검색간격     ";//"iSampleGap  " ;
    _pParaUi->lbLineGap     -> Caption = "라인인정범위 ";//"iLineGap    " ;

    _pParaUi->lbChkOfs      -> Caption = "크랙확인위치 ";//"iChkOfs     " ;
    _pParaUi->lbLowThsd     -> Caption = "크랙Low      ";//"iLowThsd    " ;
    _pParaUi->lbHighThsd    -> Caption = "크랙High     ";//"iHighThsd   " ;


    _pParaUi->lbAverage     -> Caption = "평균범위        ";//"iAverage    " ;
    _pParaUi->lbCompare     -> Caption = "비교범위        ";//"iCompare    " ;
    _pParaUi->lbChkPxLow    -> Caption = "확정픽셀Low     ";//"iChkPxLow   " ;
    _pParaUi->lbChkPxHgh    -> Caption = "확정픽셀High    ";//"iChkPxHgh   " ;

    _pParaUi->lbEndOffsetL  -> Caption = "L검색 시작끝 오프셑 ";//"iEndOffsetL " ;
    _pParaUi->lbCntrOffsetL -> Caption = "L검색 중간 오프셑   ";//"iCntrOffsetL" ;
    _pParaUi->lbOfsGapL     -> Caption = "L확정옵셑거리       ";//"iOfsGapL    " ;
    _pParaUi->lbOfsPxLowL   -> Caption = "L확정옵셑거리Low    ";//"iOfsPxLowL  " ;
    _pParaUi->lbOfsPxHghL   -> Caption = "L확정옵셑거리High   ";//"iOfsPxHghL  " ;

    _pParaUi->lbEndOffsetT  -> Caption = "T검색 시작끝 오프셑 ";//"iEndOffsetT " ;
    _pParaUi->lbCntrOffsetT -> Caption = "T검색 중간 오프셑   ";//"iCntrOffsetT" ;
    _pParaUi->lbOfsGapT     -> Caption = "T확정옵셑거리       ";//"iOfsGapT    " ;
    _pParaUi->lbOfsPxLowT   -> Caption = "T확정옵셑거리Low    ";//"iOfsPxLowT  " ;
    _pParaUi->lbOfsPxHghT   -> Caption = "T확정옵셑거리High   ";//"iOfsPxHghT  " ;

    _pParaUi->lbEndOffsetR  -> Caption = "R검색 시작끝 오프셑 ";//"iEndOffsetR " ;
    _pParaUi->lbCntrOffsetR -> Caption = "R검색 중간 오프셑   ";//"iCntrOffsetR" ;
    _pParaUi->lbOfsGapR     -> Caption = "R확정옵셑거리       ";//"iOfsGapR    " ;
    _pParaUi->lbOfsPxLowR   -> Caption = "R확정옵셑거리Low    ";//"iOfsPxLowR  " ;
    _pParaUi->lbOfsPxHghR   -> Caption = "R확정옵셑거리High   ";//"iOfsPxHghR  " ;

    _pParaUi->lbEndOffsetB  -> Caption = "B검색 시작끝 오프셑 ";//"iEndOffsetB " ;
    _pParaUi->lbCntrOffsetB -> Caption = "B검색 중간 오프셑   ";//"iCntrOffsetB" ;
    _pParaUi->lbOfsGapB     -> Caption = "B확정옵셑거리       ";//"iOfsGapB    " ;
    _pParaUi->lbOfsPxLowB   -> Caption = "B확정옵셑거리Low    ";//"iOfsPxLowB  " ;
    _pParaUi->lbOfsPxHghB   -> Caption = "B확정옵셑거리High   ";//"iOfsPxHghB  " ;
}

void CRct::SetArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    const int iControlWidth   = 120 ;
    const int iControlHeight  = 20  ;
    const int iStartRowOffset = 1   ;
    const int iStartColOffset = 1   ;

    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;
}

void CRct::SetPaintCallback(TOnChanged ChangeFunc)
{
    Tracker -> SetFunc(ChangeFunc) ;
}

void CRct::SetTrackerVisible(bool _bVisible)
{
    Tracker -> Visible = _bVisible ;
}



void CRct::SetTrackerEnabled(bool _bEnabled)
{
    Tracker -> Enabled = _bEnabled ;
}

bool CRct::MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker -> Focused = false ;

    if(Tracker -> TrackerDw(Button, Shift , X,Y, _fScaleX , _fScaleY)) { m_fInspOfsX = 0.0 ; m_fInspOfsY = 0.0 ; return true ;}

    return false ;
}

bool CRct::MouseTrackerMv(TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    if(Tracker -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY)) return true ;

    return false ;
}

void CRct::MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker -> TrackerUp(Button, Shift );
}

void CRct::PaintTracker(HDC _hHdc , float _fScaleX , float _fScaleY)
{
    if(Tracker -> Visible)PaintOfsRect(_hHdc , _fScaleX , _fScaleY , GetOfsRect(Tracker -> GetTRect()));
    Tracker -> Paint(_hHdc , _fScaleX , _fScaleY);
}

void CRct::ShowParaForm(TWinControl* _wcBase)
{
    if(ParaUi.pnBase == NULL)MakeUI(&ParaUi,_wcBase) ;
    ParaUi.pnBase -> Parent = _wcBase ;
    ParaUi.pnBase -> Align = alClient ;
}

void CRct::LoadPara(bool _bLoad , AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId+"_Rect" ;
    AnsiString sFilePath = _sFilePath + ".ini" ;
    AnsiString sTrakPath = _sFilePath + "Tracker.ini" ;

    Tracker -> OnLoadSave(_bLoad , sTrakPath , sTemp + "_Tracker");

    int iTemp ;
    if(_bLoad) {
        UserINI.Load(sFilePath, sTemp , "bLtOnDk                   ", Para.bLtOnDk                   );

        UserINI.Load(sFilePath, sTemp , "iChkOfs                   ", Para.iChkOfs                   );
        UserINI.Load(sFilePath, sTemp , "iLowThsd                  ", Para.iLowThsd                  );
        UserINI.Load(sFilePath, sTemp , "iHighThsd                 ", Para.iHighThsd                 );

        UserINI.Load(sFilePath, sTemp , "LinParaL.iSampleGap       ", Para.LinParaL.iSampleGap       );
        UserINI.Load(sFilePath, sTemp , "LinParaL.iLineGap         ", Para.LinParaL.iLineGap         );

        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iAverage ", Para.LinParaL.EdgPara.iAverage );
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iCompare ", Para.LinParaL.EdgPara.iCompare );
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iChkPxLow", Para.LinParaL.EdgPara.iChkPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iChkPxHgh", Para.LinParaL.EdgPara.iChkPxHgh);


        UserINI.Load(sFilePath, sTemp , "LinParaL.iEndOffset       ", Para.LinParaL.iEndOffset       );
        UserINI.Load(sFilePath, sTemp , "LinParaL.iCntrOffset      ", Para.LinParaL.iCntrOffset      );
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iOfsGap  ", Para.LinParaL.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iOfsPxLow", Para.LinParaL.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iOfsPxHgh", Para.LinParaL.EdgPara.iOfsPxHgh);

        UserINI.Load(sFilePath, sTemp , "LinParaT.iEndOffset       ", Para.LinParaT.iEndOffset       );
        UserINI.Load(sFilePath, sTemp , "LinParaT.iCntrOffset      ", Para.LinParaT.iCntrOffset      );
        UserINI.Load(sFilePath, sTemp , "LinParaT.EdgPara.iOfsGap  ", Para.LinParaT.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "LinParaT.EdgPara.iOfsPxLow", Para.LinParaT.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaT.EdgPara.iOfsPxHgh", Para.LinParaT.EdgPara.iOfsPxHgh);

        UserINI.Load(sFilePath, sTemp , "LinParaR.iEndOffset       ", Para.LinParaR.iEndOffset       );
        UserINI.Load(sFilePath, sTemp , "LinParaR.iCntrOffset      ", Para.LinParaR.iCntrOffset      );
        UserINI.Load(sFilePath, sTemp , "LinParaR.EdgPara.iOfsGap  ", Para.LinParaR.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "LinParaR.EdgPara.iOfsPxLow", Para.LinParaR.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaR.EdgPara.iOfsPxHgh", Para.LinParaR.EdgPara.iOfsPxHgh);

        UserINI.Load(sFilePath, sTemp , "LinParaB.iEndOffset       ", Para.LinParaB.iEndOffset       );
        UserINI.Load(sFilePath, sTemp , "LinParaB.iCntrOffset      ", Para.LinParaB.iCntrOffset      );
        UserINI.Load(sFilePath, sTemp , "LinParaB.EdgPara.iOfsGap  ", Para.LinParaB.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "LinParaB.EdgPara.iOfsPxLow", Para.LinParaB.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaB.EdgPara.iOfsPxHgh", Para.LinParaB.EdgPara.iOfsPxHgh);

        UserINI.Load(sFilePath, sTemp , "m_fRefValX", m_fRefValX);
        UserINI.Load(sFilePath, sTemp , "m_fRefValY", m_fRefValY);
        UserINI.Load(sFilePath, sTemp , "m_fRefValT", m_fRefValT);
    }
    else {
        UserINI.Save(sFilePath, sTemp , "bLtOnDk                   ", Para.bLtOnDk                   );

        UserINI.Save(sFilePath, sTemp , "iChkOfs                   ", Para.iChkOfs                   );
        UserINI.Save(sFilePath, sTemp , "iLowThsd                  ", Para.iLowThsd                  );
        UserINI.Save(sFilePath, sTemp , "iHighThsd                 ", Para.iHighThsd                 );

        UserINI.Save(sFilePath, sTemp , "LinParaL.iSampleGap       ", Para.LinParaL.iSampleGap       );
        UserINI.Save(sFilePath, sTemp , "LinParaL.iLineGap         ", Para.LinParaL.iLineGap         );

        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iAverage ", Para.LinParaL.EdgPara.iAverage );
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iCompare ", Para.LinParaL.EdgPara.iCompare );
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iChkPxLow", Para.LinParaL.EdgPara.iChkPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iChkPxHgh", Para.LinParaL.EdgPara.iChkPxHgh);

        UserINI.Save(sFilePath, sTemp , "LinParaL.iEndOffset       ", Para.LinParaL.iEndOffset       );
        UserINI.Save(sFilePath, sTemp , "LinParaL.iCntrOffset      ", Para.LinParaL.iCntrOffset      );
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iOfsGap  ", Para.LinParaL.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iOfsPxLow", Para.LinParaL.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iOfsPxHgh", Para.LinParaL.EdgPara.iOfsPxHgh);

        UserINI.Save(sFilePath, sTemp , "LinParaT.iEndOffset       ", Para.LinParaT.iEndOffset       );
        UserINI.Save(sFilePath, sTemp , "LinParaT.iCntrOffset      ", Para.LinParaT.iCntrOffset      );
        UserINI.Save(sFilePath, sTemp , "LinParaT.EdgPara.iOfsGap  ", Para.LinParaT.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "LinParaT.EdgPara.iOfsPxLow", Para.LinParaT.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaT.EdgPara.iOfsPxHgh", Para.LinParaT.EdgPara.iOfsPxHgh);

        UserINI.Save(sFilePath, sTemp , "LinParaR.iEndOffset       ", Para.LinParaR.iEndOffset       );
        UserINI.Save(sFilePath, sTemp , "LinParaR.iCntrOffset      ", Para.LinParaR.iCntrOffset      );
        UserINI.Save(sFilePath, sTemp , "LinParaR.EdgPara.iOfsGap  ", Para.LinParaR.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "LinParaR.EdgPara.iOfsPxLow", Para.LinParaR.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaR.EdgPara.iOfsPxHgh", Para.LinParaR.EdgPara.iOfsPxHgh);

        UserINI.Save(sFilePath, sTemp , "LinParaB.iEndOffset       ", Para.LinParaB.iEndOffset       );
        UserINI.Save(sFilePath, sTemp , "LinParaB.iCntrOffset      ", Para.LinParaB.iCntrOffset      );
        UserINI.Save(sFilePath, sTemp , "LinParaB.EdgPara.iOfsGap  ", Para.LinParaB.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "LinParaB.EdgPara.iOfsPxLow", Para.LinParaB.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaB.EdgPara.iOfsPxHgh", Para.LinParaB.EdgPara.iOfsPxHgh);

        //여기에 레퍼런스 저장 하는 것..... 세이브 한번 해야지만 결과값을 뽑을수 있는 파라가 저장 되고 1번 검사를 해야 레퍼런스를 세팅 할수 있다.
        m_fRefValX = Rslt.CenterPnt.x ;
        m_fRefValY = Rslt.CenterPnt.y ;
        m_fRefValT = Rslt.fAngle      ;
        UserINI.Save(sFilePath, sTemp , "m_fRefValX", m_fRefValX);
        UserINI.Save(sFilePath, sTemp , "m_fRefValY", m_fRefValY);
        UserINI.Save(sFilePath, sTemp , "m_fRefValT", m_fRefValT);
    }
}

void CRct::ApplyPara(bool _bTable)
{
    if(_bTable) {
         ParaUi.cbLtOnDk      -> Checked = Para.bLtOnDk                 ;
         ParaUi.edChkOfs      -> Text    = Para.iChkOfs                 ;
         ParaUi.edLowThsd     -> Text    = Para.iLowThsd                ;
         ParaUi.edHighThsd    -> Text    = Para.iHighThsd               ;

         ParaUi.edSampleGap   -> Text = Para.LinParaL.iSampleGap        ;
         ParaUi.edLineGap     -> Text = Para.LinParaL.iLineGap          ;

         ParaUi.edAverage     -> Text = Para.LinParaL.EdgPara.iAverage  ;
         ParaUi.edCompare     -> Text = Para.LinParaL.EdgPara.iCompare  ;
         ParaUi.edChkPxLow    -> Text = Para.LinParaL.EdgPara.iChkPxLow ;
         ParaUi.edChkPxHgh    -> Text = Para.LinParaL.EdgPara.iChkPxHgh ;

         ParaUi.edEndOffsetL  -> Text = Para.LinParaL.iEndOffset        ;
         ParaUi.edCntrOffsetL -> Text = Para.LinParaL.iCntrOffset       ;
         ParaUi.edOfsGapL     -> Text = Para.LinParaL.EdgPara.iOfsGap   ;
         ParaUi.edOfsPxLowL   -> Text = Para.LinParaL.EdgPara.iOfsPxLow ;
         ParaUi.edOfsPxHghL   -> Text = Para.LinParaL.EdgPara.iOfsPxHgh ;

         ParaUi.edEndOffsetT  -> Text = Para.LinParaT.iEndOffset        ;
         ParaUi.edCntrOffsetT -> Text = Para.LinParaT.iCntrOffset       ;
         ParaUi.edOfsGapT     -> Text = Para.LinParaT.EdgPara.iOfsGap   ;
         ParaUi.edOfsPxLowT   -> Text = Para.LinParaT.EdgPara.iOfsPxLow ;
         ParaUi.edOfsPxHghT   -> Text = Para.LinParaT.EdgPara.iOfsPxHgh ;

         ParaUi.edEndOffsetR  -> Text = Para.LinParaR.iEndOffset        ;
         ParaUi.edCntrOffsetR -> Text = Para.LinParaR.iCntrOffset       ;
         ParaUi.edOfsGapR     -> Text = Para.LinParaR.EdgPara.iOfsGap   ;
         ParaUi.edOfsPxLowR   -> Text = Para.LinParaR.EdgPara.iOfsPxLow ;
         ParaUi.edOfsPxHghR   -> Text = Para.LinParaR.EdgPara.iOfsPxHgh ;

         ParaUi.edEndOffsetB  -> Text = Para.LinParaB.iEndOffset        ;
         ParaUi.edCntrOffsetB -> Text = Para.LinParaB.iCntrOffset       ;
         ParaUi.edOfsGapB     -> Text = Para.LinParaB.EdgPara.iOfsGap   ;
         ParaUi.edOfsPxLowB   -> Text = Para.LinParaB.EdgPara.iOfsPxLow ;
         ParaUi.edOfsPxHghB   -> Text = Para.LinParaB.EdgPara.iOfsPxHgh ;
    }
    else {
        int  iTemp ;
        bool bTemp ;
        bTemp = ParaUi.cbLtOnDk      -> Checked              ; if (Para.bLtOnDk                    != bTemp) { Trace("Operator" , "Both Rct_LtOnDk values are different."); }

        iTemp = StrToIntDef(ParaUi.edChkOfs      -> Text , 0); if (Para.iChkOfs                    != iTemp) { Trace("Operator" , "Both Rct_ChkOfs values are different."); }
        iTemp = StrToIntDef(ParaUi.edLowThsd     -> Text , 0); if (Para.iLowThsd                   != iTemp) { Trace("Operator" , "Both Rct_LowThsd values are different."); }
        iTemp = StrToIntDef(ParaUi.edHighThsd    -> Text , 0); if (Para.iHighThsd                  != iTemp) { Trace("Operator" , "Both Rct_HighThsd values are different."); }


        iTemp = StrToIntDef(ParaUi.edSampleGap   -> Text , 1); if (Para.LinParaL.iSampleGap        != iTemp) { Trace("Operator" , "Both Rct_SampleGap values are different."); }
        iTemp = StrToIntDef(ParaUi.edLineGap     -> Text , 1); if (Para.LinParaL.iLineGap          != iTemp) { Trace("Operator" , "Both Rct_LineGap values are different."); }


        iTemp = StrToIntDef(ParaUi.edAverage     -> Text , 1); if (Para.LinParaL.EdgPara.iAverage  != iTemp) { Trace("Operator" , "Both Rct_Average values are different."); }
        iTemp = StrToIntDef(ParaUi.edCompare     -> Text , 1); if (Para.LinParaL.EdgPara.iCompare  != iTemp) { Trace("Operator" , "Both Rct_Compare values are different."); }
        iTemp = StrToIntDef(ParaUi.edChkPxLow    -> Text , 0); if (Para.LinParaL.EdgPara.iChkPxLow != iTemp) { Trace("Operator" , "Both Rct_ChkPxLow values are different."); }
        iTemp = StrToIntDef(ParaUi.edChkPxHgh    -> Text , 0); if (Para.LinParaL.EdgPara.iChkPxHgh != iTemp) { Trace("Operator" , "Both Rct_ChkPxHgh values are different."); }

        iTemp = StrToIntDef(ParaUi.edEndOffsetL  -> Text , 0); if (Para.LinParaL.iEndOffset        != iTemp) { Trace("Operator" , "Both Rct_EndOffsetL values are different."); }
        iTemp = StrToIntDef(ParaUi.edCntrOffsetL -> Text , 0); if (Para.LinParaL.iCntrOffset       != iTemp) { Trace("Operator" , "Both Rct_CntrOffsetL values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsGapL     -> Text , 0); if (Para.LinParaL.EdgPara.iOfsGap   != iTemp) { Trace("Operator" , "Both Rct_OfsGapL values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsPxLowL   -> Text , 0); if (Para.LinParaL.EdgPara.iOfsPxLow != iTemp) { Trace("Operator" , "Both Rct_OfsPxLowL values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsPxHghL   -> Text , 0); if (Para.LinParaL.EdgPara.iOfsPxHgh != iTemp) { Trace("Operator" , "Both Rct_OfsPxHghL values are different."); }

        iTemp = StrToIntDef(ParaUi.edEndOffsetT  -> Text , 0); if (Para.LinParaT.iEndOffset        != iTemp) { Trace("Operator" , "Both Rct_EndOffsetT values are different."); }
        iTemp = StrToIntDef(ParaUi.edCntrOffsetT -> Text , 0); if (Para.LinParaT.iCntrOffset       != iTemp) { Trace("Operator" , "Both Rct_CntrOffsetT values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsGapT     -> Text , 0); if (Para.LinParaT.EdgPara.iOfsGap   != iTemp) { Trace("Operator" , "Both Rct_OfsGapT values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsPxLowT   -> Text , 0); if (Para.LinParaT.EdgPara.iOfsPxLow != iTemp) { Trace("Operator" , "Both Rct_OfsPxLowT values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsPxHghT   -> Text , 0); if (Para.LinParaT.EdgPara.iOfsPxHgh != iTemp) { Trace("Operator" , "Both Rct_OfsPxHghT values are different."); }

        iTemp = StrToIntDef(ParaUi.edEndOffsetR  -> Text , 0); if (Para.LinParaR.iEndOffset        != iTemp) { Trace("Operator" , "Both Rct_EndOffsetR values are different."); }
        iTemp = StrToIntDef(ParaUi.edCntrOffsetR -> Text , 0); if (Para.LinParaR.iCntrOffset       != iTemp) { Trace("Operator" , "Both Rct_CntrOffsetR values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsGapR     -> Text , 0); if (Para.LinParaR.EdgPara.iOfsGap   != iTemp) { Trace("Operator" , "Both Rct_OfsGapR values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsPxLowR   -> Text , 0); if (Para.LinParaR.EdgPara.iOfsPxLow != iTemp) { Trace("Operator" , "Both Rct_OfsPxLowR values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsPxHghR   -> Text , 0); if (Para.LinParaR.EdgPara.iOfsPxHgh != iTemp) { Trace("Operator" , "Both Rct_OfsPxHghR values are different."); }

        iTemp = StrToIntDef(ParaUi.edEndOffsetB  -> Text , 0); if (Para.LinParaB.iEndOffset        != iTemp) { Trace("Operator" , "Both Rct_EndOffsetB values are different."); }
        iTemp = StrToIntDef(ParaUi.edCntrOffsetB -> Text , 0); if (Para.LinParaB.iCntrOffset       != iTemp) { Trace("Operator" , "Both Rct_CntrOffsetB values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsGapB     -> Text , 0); if (Para.LinParaB.EdgPara.iOfsGap   != iTemp) { Trace("Operator" , "Both Rct_OfsGapB values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsPxLowB   -> Text , 0); if (Para.LinParaB.EdgPara.iOfsPxLow != iTemp) { Trace("Operator" , "Both Rct_OfsPxLowB values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsPxHghB   -> Text , 0); if (Para.LinParaB.EdgPara.iOfsPxHgh != iTemp) { Trace("Operator" , "Both Rct_OfsPxHghB values are different."); }


         Para.bLtOnDk                    = ParaUi.cbLtOnDk      -> Checked         ;

         Para.iChkOfs                    = StrToIntDef(ParaUi.edChkOfs      -> Text , 0)  ;
         Para.iLowThsd                   = StrToIntDef(ParaUi.edLowThsd     -> Text , 0)  ;
         Para.iHighThsd                  = StrToIntDef(ParaUi.edHighThsd    -> Text , 0)  ;


         Para.LinParaL.iSampleGap        = StrToIntDef(ParaUi.edSampleGap   -> Text , 1 ) ;
         Para.LinParaL.iLineGap          = StrToIntDef(ParaUi.edLineGap     -> Text , 1 ) ;

         Para.LinParaL.EdgPara.iAverage  = StrToIntDef(ParaUi.edAverage     -> Text , 1 ) ;
         Para.LinParaL.EdgPara.iCompare  = StrToIntDef(ParaUi.edCompare     -> Text , 1 ) ;
         Para.LinParaL.EdgPara.iChkPxLow = StrToIntDef(ParaUi.edChkPxLow    -> Text , 0 ) ;
         Para.LinParaL.EdgPara.iChkPxHgh = StrToIntDef(ParaUi.edChkPxHgh    -> Text , 0 ) ;

         Para.LinParaL.iEndOffset        = StrToIntDef(ParaUi.edEndOffsetL  -> Text , 0 ) ;
         Para.LinParaL.iCntrOffset       = StrToIntDef(ParaUi.edCntrOffsetL -> Text , 0 ) ;
         Para.LinParaL.EdgPara.iOfsGap   = StrToIntDef(ParaUi.edOfsGapL     -> Text , 0 ) ;
         Para.LinParaL.EdgPara.iOfsPxLow = StrToIntDef(ParaUi.edOfsPxLowL   -> Text , 0 ) ;
         Para.LinParaL.EdgPara.iOfsPxHgh = StrToIntDef(ParaUi.edOfsPxHghL   -> Text , 0 ) ;

         Para.LinParaT.iEndOffset        = StrToIntDef(ParaUi.edEndOffsetT  -> Text , 0 ) ;
         Para.LinParaT.iCntrOffset       = StrToIntDef(ParaUi.edCntrOffsetT -> Text , 0 ) ;
         Para.LinParaT.EdgPara.iOfsGap   = StrToIntDef(ParaUi.edOfsGapT     -> Text , 0 ) ;
         Para.LinParaT.EdgPara.iOfsPxLow = StrToIntDef(ParaUi.edOfsPxLowT   -> Text , 0 ) ;
         Para.LinParaT.EdgPara.iOfsPxHgh = StrToIntDef(ParaUi.edOfsPxHghT   -> Text , 0 ) ;

         Para.LinParaR.iEndOffset        = StrToIntDef(ParaUi.edEndOffsetR  -> Text , 0 ) ;
         Para.LinParaR.iCntrOffset       = StrToIntDef(ParaUi.edCntrOffsetR -> Text , 0 ) ;
         Para.LinParaR.EdgPara.iOfsGap   = StrToIntDef(ParaUi.edOfsGapR     -> Text , 0 ) ;
         Para.LinParaR.EdgPara.iOfsPxLow = StrToIntDef(ParaUi.edOfsPxLowR   -> Text , 0 ) ;
         Para.LinParaR.EdgPara.iOfsPxHgh = StrToIntDef(ParaUi.edOfsPxHghR   -> Text , 0 ) ;

         Para.LinParaB.iEndOffset        = StrToIntDef(ParaUi.edEndOffsetB  -> Text , 0 ) ;
         Para.LinParaB.iCntrOffset       = StrToIntDef(ParaUi.edCntrOffsetB -> Text , 0 ) ;
         Para.LinParaB.EdgPara.iOfsGap   = StrToIntDef(ParaUi.edOfsGapB     -> Text , 0 ) ;
         Para.LinParaB.EdgPara.iOfsPxLow = StrToIntDef(ParaUi.edOfsPxLowB   -> Text , 0 ) ;
         Para.LinParaB.EdgPara.iOfsPxHgh = StrToIntDef(ParaUi.edOfsPxHghB   -> Text , 0 ) ;

         ApplyPara(true);

    }
}

bool CRct::Process(IImage * _pImg , RCT_Rslt * _pRslt)
{
    bool bRet ;

    //Para.LinParaL.bLtToDk           = !Para.bLtOnDk                   ;
    //Para.LinParaL.iScanDirect       = lsLtToRt                        ;
    Para.LinParaL.iSampleGap        = Para.LinParaL.iSampleGap        ;
    Para.LinParaL.iLineGap          = Para.LinParaL.iLineGap          ;
    Para.LinParaL.EdgPara.iAverage  = Para.LinParaL.EdgPara.iAverage  ;
    Para.LinParaL.EdgPara.iCompare  = Para.LinParaL.EdgPara.iCompare  ;
    Para.LinParaL.EdgPara.iChkPxLow = Para.LinParaL.EdgPara.iChkPxLow ;
    Para.LinParaL.EdgPara.iChkPxHgh = Para.LinParaL.EdgPara.iChkPxHgh ;

    //Para.LinParaT.bLtToDk           = !Para.bLtOnDk                   ;
    //Para.LinParaT.iScanDirect       = lsUpToDn                        ;
    Para.LinParaT.iSampleGap        = Para.LinParaL.iSampleGap        ;
    Para.LinParaT.iLineGap          = Para.LinParaL.iLineGap          ;
    Para.LinParaT.EdgPara.iAverage  = Para.LinParaL.EdgPara.iAverage  ;
    Para.LinParaT.EdgPara.iCompare  = Para.LinParaL.EdgPara.iCompare  ;
    Para.LinParaT.EdgPara.iChkPxLow = Para.LinParaL.EdgPara.iChkPxLow ;
    Para.LinParaT.EdgPara.iChkPxHgh = Para.LinParaL.EdgPara.iChkPxHgh ;

    //Para.LinParaR.bLtToDk           = !Para.bLtOnDk                   ;
    //Para.LinParaR.iScanDirect       = lsRtToLt                        ;
    Para.LinParaR.iSampleGap        = Para.LinParaL.iSampleGap        ;
    Para.LinParaR.iLineGap          = Para.LinParaL.iLineGap          ;
    Para.LinParaR.EdgPara.iAverage  = Para.LinParaL.EdgPara.iAverage  ;
    Para.LinParaR.EdgPara.iCompare  = Para.LinParaL.EdgPara.iCompare  ;
    Para.LinParaR.EdgPara.iChkPxLow = Para.LinParaL.EdgPara.iChkPxLow ;
    Para.LinParaR.EdgPara.iChkPxHgh = Para.LinParaL.EdgPara.iChkPxHgh ;

    //Para.LinParaB.bLtToDk           = !Para.bLtOnDk                   ;
    //Para.LinParaB.iScanDirect       = lsDnToUp                        ;
    Para.LinParaB.iSampleGap        = Para.LinParaL.iSampleGap        ;
    Para.LinParaB.iLineGap          = Para.LinParaL.iLineGap          ;
    Para.LinParaB.EdgPara.iAverage  = Para.LinParaL.EdgPara.iAverage  ;
    Para.LinParaB.EdgPara.iCompare  = Para.LinParaL.EdgPara.iCompare  ;
    Para.LinParaB.EdgPara.iChkPxLow = Para.LinParaL.EdgPara.iChkPxLow ;
    Para.LinParaB.EdgPara.iChkPxHgh = Para.LinParaL.EdgPara.iChkPxHgh ;                        


    bRet = RCT_Inspect(_pImg , GetOfsRect(Tracker -> GetLTRect()) , GetOfsRect(Tracker -> GetTTRect()) , GetOfsRect(Tracker -> GetRTRect()), GetOfsRect(Tracker -> GetBTRect()) , Para , _pRslt);
    memcpy(&Rslt , _pRslt , sizeof(RCT_Rslt));

    return bRet ;
}

void CRct::PaintRslt(IImage * _pImg , HDC _hHdc)
{
    RCT_PaintRslt(_pImg , _hHdc , &Rslt , Disp);
}

void CRct::GetRefValOfs(float &_fX , float &_fY , float &_fT )
{
    _fX = Rslt.CenterPnt.x - m_fRefValX ;
    _fY = Rslt.CenterPnt.y - m_fRefValY ;
    _fT = Rslt.fAngle      - m_fRefValT ; //앵글 조심. 0도 짜리 는 360 대로 떨어질수 있다.
}

/******************************************************************************/
/* Wafer Rect                                                                 */
/******************************************************************************/
CWrt::CWrt()
{
    m_iProcType = ptWrt ;

    Tracker = new TRectTracker() ; Tracker -> Caption = "WRT";
    Tracker -> BandVisible = true ;
    //Tracker -> Band = 130 ;

    //MakeUI(&ParaUi);
    ParaUi.pnBase = NULL ;

    //Disp.LinDisp.clLine = clLime ;
    Disp.LinDisp.clEdge = clRed ;
    //Disp.LinDisp.clText = clLime ;
    Disp.clPoint = clGreen;
    Disp.clText  = clLime ;
    Disp.clRect  = clLime ;

}

CWrt::~CWrt()
{
    delete Tracker ;
}

void CWrt::MakeUI(TParaUi  * _pParaUi , TWinControl * _tcControl)
{
    int iR = 0 ;
    _pParaUi->pnBase = new TPanel(_tcControl  ); _pParaUi->pnBase -> Parent = _tcControl ;

    //Line 1 ---------------------------------
    _pParaUi->cbLtOnDk     = new TCheckBox(_pParaUi->pnBase);
    _pParaUi->lbSampleGap  = new TLabel   (_pParaUi->pnBase);  _pParaUi->edSampleGap   = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbLineGap    = new TLabel   (_pParaUi->pnBase);  _pParaUi->edLineGap     = new TEdit(_pParaUi->pnBase);

    _pParaUi->lbChkOfs     = new TLabel   (_pParaUi->pnBase);  _pParaUi->edChkOfs      = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbLowThsd    = new TLabel   (_pParaUi->pnBase);  _pParaUi->edLowThsd     = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbHighThsd   = new TLabel   (_pParaUi->pnBase);  _pParaUi->edHighThsd    = new TEdit(_pParaUi->pnBase);

    _pParaUi->lbAverage    = new TLabel   (_pParaUi->pnBase);  _pParaUi->edAverage     = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbCompare    = new TLabel   (_pParaUi->pnBase);  _pParaUi->edCompare     = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbChkPxLow   = new TLabel   (_pParaUi->pnBase);  _pParaUi->edChkPxLow    = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbChkPxHgh   = new TLabel   (_pParaUi->pnBase);  _pParaUi->edChkPxHgh    = new TEdit(_pParaUi->pnBase);

    _pParaUi->lbEndOffsetL = new TLabel   (_pParaUi->pnBase);  _pParaUi->edEndOffsetL  = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbCntrOffsetL= new TLabel   (_pParaUi->pnBase);  _pParaUi->edCntrOffsetL = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsGapL    = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsGapL     = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsPxLowL  = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsPxLowL   = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsPxHghL  = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsPxHghL   = new TEdit(_pParaUi->pnBase);

    _pParaUi->lbEndOffsetT = new TLabel   (_pParaUi->pnBase);  _pParaUi->edEndOffsetT  = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbCntrOffsetT= new TLabel   (_pParaUi->pnBase);  _pParaUi->edCntrOffsetT = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsGapT    = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsGapT     = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsPxLowT  = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsPxLowT   = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsPxHghT  = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsPxHghT   = new TEdit(_pParaUi->pnBase);

    _pParaUi->lbEndOffsetR = new TLabel   (_pParaUi->pnBase);  _pParaUi->edEndOffsetR  = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbCntrOffsetR= new TLabel   (_pParaUi->pnBase);  _pParaUi->edCntrOffsetR = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsGapR    = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsGapR     = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsPxLowR  = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsPxLowR   = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsPxHghR  = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsPxHghR   = new TEdit(_pParaUi->pnBase);

    _pParaUi->lbEndOffsetB = new TLabel   (_pParaUi->pnBase);  _pParaUi->edEndOffsetB  = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbCntrOffsetB= new TLabel   (_pParaUi->pnBase);  _pParaUi->edCntrOffsetB = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsGapB    = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsGapB     = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsPxLowB  = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsPxLowB   = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbOfsPxHghB  = new TLabel   (_pParaUi->pnBase);  _pParaUi->edOfsPxHghB   = new TEdit(_pParaUi->pnBase);

    SetArrange(_pParaUi->cbLtOnDk      , 0 , iR++);
    SetArrange(_pParaUi->lbSampleGap   , 0 , iR  );  SetArrange(_pParaUi->edSampleGap   , 1 , iR++ );
    SetArrange(_pParaUi->lbLineGap     , 0 , iR  );  SetArrange(_pParaUi->edLineGap     , 1 , iR++ );

    SetArrange(_pParaUi->lbChkOfs      , 0 , iR  );  SetArrange(_pParaUi->edChkOfs      , 1 , iR++ );
    SetArrange(_pParaUi->lbLowThsd     , 0 , iR  );  SetArrange(_pParaUi->edLowThsd     , 1 , iR++ );
    SetArrange(_pParaUi->lbHighThsd    , 0 , iR  );  SetArrange(_pParaUi->edHighThsd    , 1 , iR++ );

    SetArrange(_pParaUi->lbAverage     , 0 , iR  );  SetArrange(_pParaUi->edAverage     , 1 , iR++ );
    SetArrange(_pParaUi->lbCompare     , 0 , iR  );  SetArrange(_pParaUi->edCompare     , 1 , iR++ );
    SetArrange(_pParaUi->lbChkPxLow    , 0 , iR  );  SetArrange(_pParaUi->edChkPxLow    , 1 , iR++ );
    SetArrange(_pParaUi->lbChkPxHgh    , 0 , iR  );  SetArrange(_pParaUi->edChkPxHgh    , 1 , iR++ );

    SetArrange(_pParaUi->lbEndOffsetL  , 0 , iR  );  SetArrange(_pParaUi->edEndOffsetL  , 1 , iR++ );
    SetArrange(_pParaUi->lbCntrOffsetL , 0 , iR  );  SetArrange(_pParaUi->edCntrOffsetL , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsGapL     , 0 , iR  );  SetArrange(_pParaUi->edOfsGapL     , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsPxLowL   , 0 , iR  );  SetArrange(_pParaUi->edOfsPxLowL   , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsPxHghL   , 0 , iR  );  SetArrange(_pParaUi->edOfsPxHghL   , 1 , iR++ );

    SetArrange(_pParaUi->lbEndOffsetT  , 0 , iR  );  SetArrange(_pParaUi->edEndOffsetT  , 1 , iR++ );
    SetArrange(_pParaUi->lbCntrOffsetT , 0 , iR  );  SetArrange(_pParaUi->edCntrOffsetT , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsGapT     , 0 , iR  );  SetArrange(_pParaUi->edOfsGapT     , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsPxLowT   , 0 , iR  );  SetArrange(_pParaUi->edOfsPxLowT   , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsPxHghT   , 0 , iR  );  SetArrange(_pParaUi->edOfsPxHghT   , 1 , iR++ );

    SetArrange(_pParaUi->lbEndOffsetR  , 0 , iR  );  SetArrange(_pParaUi->edEndOffsetR  , 1 , iR++ );
    SetArrange(_pParaUi->lbCntrOffsetR , 0 , iR  );  SetArrange(_pParaUi->edCntrOffsetR , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsGapR     , 0 , iR  );  SetArrange(_pParaUi->edOfsGapR     , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsPxLowR   , 0 , iR  );  SetArrange(_pParaUi->edOfsPxLowR   , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsPxHghR   , 0 , iR  );  SetArrange(_pParaUi->edOfsPxHghR   , 1 , iR++ );

    SetArrange(_pParaUi->lbEndOffsetB  , 0 , iR  );  SetArrange(_pParaUi->edEndOffsetB  , 1 , iR++ );
    SetArrange(_pParaUi->lbCntrOffsetB , 0 , iR  );  SetArrange(_pParaUi->edCntrOffsetB , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsGapB     , 0 , iR  );  SetArrange(_pParaUi->edOfsGapB     , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsPxLowB   , 0 , iR  );  SetArrange(_pParaUi->edOfsPxLowB   , 1 , iR++ );
    SetArrange(_pParaUi->lbOfsPxHghB   , 0 , iR  );  SetArrange(_pParaUi->edOfsPxHghB   , 1 , iR++ );

    _pParaUi->cbLtOnDk      -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbSampleGap   -> Parent = _pParaUi->pnBase ; _pParaUi->edSampleGap   -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbLineGap     -> Parent = _pParaUi->pnBase ; _pParaUi->edLineGap     -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbChkOfs      -> Parent = _pParaUi->pnBase ; _pParaUi->edChkOfs      -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbLowThsd     -> Parent = _pParaUi->pnBase ; _pParaUi->edLowThsd     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbHighThsd    -> Parent = _pParaUi->pnBase ; _pParaUi->edHighThsd    -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbAverage     -> Parent = _pParaUi->pnBase ; _pParaUi->edAverage     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbCompare     -> Parent = _pParaUi->pnBase ; _pParaUi->edCompare     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbChkPxLow    -> Parent = _pParaUi->pnBase ; _pParaUi->edChkPxLow    -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbChkPxHgh    -> Parent = _pParaUi->pnBase ; _pParaUi->edChkPxHgh    -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbEndOffsetL  -> Parent = _pParaUi->pnBase ; _pParaUi->edEndOffsetL  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbCntrOffsetL -> Parent = _pParaUi->pnBase ; _pParaUi->edCntrOffsetL -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsGapL     -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsGapL     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsPxLowL   -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsPxLowL   -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsPxHghL   -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsPxHghL   -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbEndOffsetT  -> Parent = _pParaUi->pnBase ; _pParaUi->edEndOffsetT  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbCntrOffsetT -> Parent = _pParaUi->pnBase ; _pParaUi->edCntrOffsetT -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsGapT     -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsGapT     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsPxLowT   -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsPxLowT   -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsPxHghT   -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsPxHghT   -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbEndOffsetR  -> Parent = _pParaUi->pnBase ; _pParaUi->edEndOffsetR  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbCntrOffsetR -> Parent = _pParaUi->pnBase ; _pParaUi->edCntrOffsetR -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsGapR     -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsGapR     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsPxLowR   -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsPxLowR   -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsPxHghR   -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsPxHghR   -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbEndOffsetB  -> Parent = _pParaUi->pnBase ; _pParaUi->edEndOffsetB  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbCntrOffsetB -> Parent = _pParaUi->pnBase ; _pParaUi->edCntrOffsetB -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsGapB     -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsGapB     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsPxLowB   -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsPxLowB   -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbOfsPxHghB   -> Parent = _pParaUi->pnBase ; _pParaUi->edOfsPxHghB   -> Parent = _pParaUi->pnBase ;

    _pParaUi->cbLtOnDk      -> Caption = "밝은물체찾기 ";//"bLtOnDk     " ;
    _pParaUi->lbSampleGap   -> Caption = "검색간격     ";//"iSampleGap  " ;
    _pParaUi->lbLineGap     -> Caption = "라인인정범위 ";//"iLineGap    " ;

    _pParaUi->lbChkOfs      -> Caption = "크랙확인위치 ";//"iChkOfs     " ;
    _pParaUi->lbLowThsd     -> Caption = "자제밝기Low  ";//"iLowThsd    " ;
    _pParaUi->lbHighThsd    -> Caption = "자제밝기High ";//"iHighThsd   " ;


    _pParaUi->lbAverage     -> Caption = "평균범위        ";//"iAverage    " ;
    _pParaUi->lbCompare     -> Caption = "비교범위        ";//"iCompare    " ;
    _pParaUi->lbChkPxLow    -> Caption = "확정픽셀Low     ";//"iChkPxLow   " ;
    _pParaUi->lbChkPxHgh    -> Caption = "확정픽셀High    ";//"iChkPxHgh   " ;

    _pParaUi->lbEndOffsetL  -> Caption = "L검색 시작끝 오프셑 ";//"iEndOffsetL " ;
    _pParaUi->lbCntrOffsetL -> Caption = "L검색 중간 오프셑   ";//"iCntrOffsetL" ;
    _pParaUi->lbOfsGapL     -> Caption = "L확정옵셑거리       ";//"iOfsGapL    " ;
    _pParaUi->lbOfsPxLowL   -> Caption = "L확정옵셑거리Low    ";//"iOfsPxLowL  " ;
    _pParaUi->lbOfsPxHghL   -> Caption = "L확정옵셑거리High   ";//"iOfsPxHghL  " ;

    _pParaUi->lbEndOffsetT  -> Caption = "T검색 시작끝 오프셑 ";//"iEndOffsetT " ;
    _pParaUi->lbCntrOffsetT -> Caption = "T검색 중간 오프셑   ";//"iCntrOffsetT" ;
    _pParaUi->lbOfsGapT     -> Caption = "T확정옵셑거리       ";//"iOfsGapT    " ;
    _pParaUi->lbOfsPxLowT   -> Caption = "T확정옵셑거리Low    ";//"iOfsPxLowT  " ;
    _pParaUi->lbOfsPxHghT   -> Caption = "T확정옵셑거리High   ";//"iOfsPxHghT  " ;

    _pParaUi->lbEndOffsetR  -> Caption = "R검색 시작끝 오프셑 ";//"iEndOffsetR " ;
    _pParaUi->lbCntrOffsetR -> Caption = "R검색 중간 오프셑   ";//"iCntrOffsetR" ;
    _pParaUi->lbOfsGapR     -> Caption = "R확정옵셑거리       ";//"iOfsGapR    " ;
    _pParaUi->lbOfsPxLowR   -> Caption = "R확정옵셑거리Low    ";//"iOfsPxLowR  " ;
    _pParaUi->lbOfsPxHghR   -> Caption = "R확정옵셑거리High   ";//"iOfsPxHghR  " ;

    _pParaUi->lbEndOffsetB  -> Caption = "B검색 시작끝 오프셑 ";//"iEndOffsetB " ;
    _pParaUi->lbCntrOffsetB -> Caption = "B검색 중간 오프셑   ";//"iCntrOffsetB" ;
    _pParaUi->lbOfsGapB     -> Caption = "B확정옵셑거리       ";//"iOfsGapB    " ;
    _pParaUi->lbOfsPxLowB   -> Caption = "B확정옵셑거리Low    ";//"iOfsPxLowB  " ;
    _pParaUi->lbOfsPxHghB   -> Caption = "B확정옵셑거리High   ";//"iOfsPxHghB  " ;
}

void CWrt::SetArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    const int iControlWidth   = 120 ;
    const int iControlHeight  = 20  ;
    const int iStartRowOffset = 1   ;
    const int iStartColOffset = 1   ;

    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;
}

void CWrt::SetPaintCallback(TOnChanged ChangeFunc)
{
    Tracker -> SetFunc(ChangeFunc) ;
}

void CWrt::SetTrackerVisible(bool _bVisible)
{
    Tracker -> Visible = _bVisible ;
}



void CWrt::SetTrackerEnabled(bool _bEnabled)
{
    Tracker -> Enabled = _bEnabled ;
}

bool CWrt::MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker -> Focused = false ;

    if(Tracker -> TrackerDw(Button, Shift , X,Y, _fScaleX , _fScaleY)) { m_fInspOfsX = 0.0 ; m_fInspOfsY = 0.0 ; return true ;}

    return false ;
}

bool CWrt::MouseTrackerMv(TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    if(Tracker -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY)) return true ;

    return false ;
}

void CWrt::MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker -> TrackerUp(Button, Shift );
}

void CWrt::PaintTracker(HDC _hHdc , float _fScaleX , float _fScaleY)
{
    if(Tracker -> Visible)PaintOfsRect(_hHdc , _fScaleX , _fScaleY , GetOfsRect(Tracker -> GetTRect()));
    Tracker -> Paint(_hHdc , _fScaleX , _fScaleY);
}

void CWrt::ShowParaForm(TWinControl* _wcBase)
{
    if(ParaUi.pnBase == NULL)MakeUI(&ParaUi,_wcBase) ;
    ParaUi.pnBase -> Parent = _wcBase ;
    ParaUi.pnBase -> Align = alClient ;
}

void CWrt::LoadPara(bool _bLoad , AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId+"_WaferRect" ;
    AnsiString sFilePath = _sFilePath + ".ini" ;
    AnsiString sTrakPath = _sFilePath + "Tracker.ini" ;

    Tracker -> OnLoadSave(_bLoad , sTrakPath , sTemp + "_Tracker");

    int iTemp ;
    if(_bLoad) {
        UserINI.Load(sFilePath, sTemp , "bLtOnDk                   ", Para.bLtOnDk                   );

        UserINI.Load(sFilePath, sTemp , "iChkOfs                   ", Para.iChkOfs                   );
        UserINI.Load(sFilePath, sTemp , "iLowThsd                  ", Para.iLowThsd                  );
        UserINI.Load(sFilePath, sTemp , "iHighThsd                 ", Para.iHighThsd                 );

        UserINI.Load(sFilePath, sTemp , "LinParaL.iSampleGap       ", Para.LinParaL.iSampleGap       );
        UserINI.Load(sFilePath, sTemp , "LinParaL.iLineGap         ", Para.LinParaL.iLineGap         );

        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iAverage ", Para.LinParaL.EdgPara.iAverage );
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iCompare ", Para.LinParaL.EdgPara.iCompare );
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iChkPxLow", Para.LinParaL.EdgPara.iChkPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iChkPxHgh", Para.LinParaL.EdgPara.iChkPxHgh);


        UserINI.Load(sFilePath, sTemp , "LinParaL.iEndOffset       ", Para.LinParaL.iEndOffset       );
        UserINI.Load(sFilePath, sTemp , "LinParaL.iCntrOffset      ", Para.LinParaL.iCntrOffset      );
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iOfsGap  ", Para.LinParaL.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iOfsPxLow", Para.LinParaL.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iOfsPxHgh", Para.LinParaL.EdgPara.iOfsPxHgh);

        UserINI.Load(sFilePath, sTemp , "LinParaT.iEndOffset       ", Para.LinParaT.iEndOffset       );
        UserINI.Load(sFilePath, sTemp , "LinParaT.iCntrOffset      ", Para.LinParaT.iCntrOffset      );
        UserINI.Load(sFilePath, sTemp , "LinParaT.EdgPara.iOfsGap  ", Para.LinParaT.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "LinParaT.EdgPara.iOfsPxLow", Para.LinParaT.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaT.EdgPara.iOfsPxHgh", Para.LinParaT.EdgPara.iOfsPxHgh);

        UserINI.Load(sFilePath, sTemp , "LinParaR.iEndOffset       ", Para.LinParaR.iEndOffset       );
        UserINI.Load(sFilePath, sTemp , "LinParaR.iCntrOffset      ", Para.LinParaR.iCntrOffset      );
        UserINI.Load(sFilePath, sTemp , "LinParaR.EdgPara.iOfsGap  ", Para.LinParaR.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "LinParaR.EdgPara.iOfsPxLow", Para.LinParaR.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaR.EdgPara.iOfsPxHgh", Para.LinParaR.EdgPara.iOfsPxHgh);

        UserINI.Load(sFilePath, sTemp , "LinParaB.iEndOffset       ", Para.LinParaB.iEndOffset       );
        UserINI.Load(sFilePath, sTemp , "LinParaB.iCntrOffset      ", Para.LinParaB.iCntrOffset      );
        UserINI.Load(sFilePath, sTemp , "LinParaB.EdgPara.iOfsGap  ", Para.LinParaB.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "LinParaB.EdgPara.iOfsPxLow", Para.LinParaB.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaB.EdgPara.iOfsPxHgh", Para.LinParaB.EdgPara.iOfsPxHgh);

        UserINI.Load(sFilePath, sTemp , "m_fRefValX", m_fRefValX);
        UserINI.Load(sFilePath, sTemp , "m_fRefValY", m_fRefValY);
        UserINI.Load(sFilePath, sTemp , "m_fRefValT", m_fRefValT);
    }
    else {
        //sTemp = _sId+"_WaferRect" ;
        //sFilePath = _sFilePath + ".ini" ;


        UserINI.Save(sFilePath, sTemp , "bLtOnDk                   ", Para.bLtOnDk                   );

        UserINI.Save(sFilePath, sTemp , "iChkOfs                   ", Para.iChkOfs                   );
        UserINI.Save(sFilePath, sTemp , "iLowThsd                  ", Para.iLowThsd                  );
        UserINI.Save(sFilePath, sTemp , "iHighThsd                 ", Para.iHighThsd                 );

        UserINI.Save(sFilePath, sTemp , "LinParaL.iSampleGap       ", Para.LinParaL.iSampleGap       );
        UserINI.Save(sFilePath, sTemp , "LinParaL.iLineGap         ", Para.LinParaL.iLineGap         );

        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iAverage ", Para.LinParaL.EdgPara.iAverage );
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iCompare ", Para.LinParaL.EdgPara.iCompare );
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iChkPxLow", Para.LinParaL.EdgPara.iChkPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iChkPxHgh", Para.LinParaL.EdgPara.iChkPxHgh);

        UserINI.Save(sFilePath, sTemp , "LinParaL.iEndOffset       ", Para.LinParaL.iEndOffset       );
        UserINI.Save(sFilePath, sTemp , "LinParaL.iCntrOffset      ", Para.LinParaL.iCntrOffset      );
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iOfsGap  ", Para.LinParaL.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iOfsPxLow", Para.LinParaL.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iOfsPxHgh", Para.LinParaL.EdgPara.iOfsPxHgh);

        UserINI.Save(sFilePath, sTemp , "LinParaT.iEndOffset       ", Para.LinParaT.iEndOffset       );
        UserINI.Save(sFilePath, sTemp , "LinParaT.iCntrOffset      ", Para.LinParaT.iCntrOffset      );
        UserINI.Save(sFilePath, sTemp , "LinParaT.EdgPara.iOfsGap  ", Para.LinParaT.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "LinParaT.EdgPara.iOfsPxLow", Para.LinParaT.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaT.EdgPara.iOfsPxHgh", Para.LinParaT.EdgPara.iOfsPxHgh);

        UserINI.Save(sFilePath, sTemp , "LinParaR.iEndOffset       ", Para.LinParaR.iEndOffset       );
        UserINI.Save(sFilePath, sTemp , "LinParaR.iCntrOffset      ", Para.LinParaR.iCntrOffset      );
        UserINI.Save(sFilePath, sTemp , "LinParaR.EdgPara.iOfsGap  ", Para.LinParaR.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "LinParaR.EdgPara.iOfsPxLow", Para.LinParaR.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaR.EdgPara.iOfsPxHgh", Para.LinParaR.EdgPara.iOfsPxHgh);

        UserINI.Save(sFilePath, sTemp , "LinParaB.iEndOffset       ", Para.LinParaB.iEndOffset       );
        UserINI.Save(sFilePath, sTemp , "LinParaB.iCntrOffset      ", Para.LinParaB.iCntrOffset      );
        UserINI.Save(sFilePath, sTemp , "LinParaB.EdgPara.iOfsGap  ", Para.LinParaB.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "LinParaB.EdgPara.iOfsPxLow", Para.LinParaB.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaB.EdgPara.iOfsPxHgh", Para.LinParaB.EdgPara.iOfsPxHgh);

        //여기에 레퍼런스 저장 하는 것..... 세이브 한번 해야지만 결과값을 뽑을수 있는 파라가 저장 되고 1번 검사를 해야 레퍼런스를 세팅 할수 있다.
        m_fRefValX = Rslt.CenterPnt.x ;
        m_fRefValY = Rslt.CenterPnt.y ;
        m_fRefValT = Rslt.fAngle      ;
        UserINI.Save(sFilePath, sTemp , "m_fRefValX", m_fRefValX);
        UserINI.Save(sFilePath, sTemp , "m_fRefValY", m_fRefValY);
        UserINI.Save(sFilePath, sTemp , "m_fRefValT", m_fRefValT);
    }
}

void CWrt::ApplyPara(bool _bTable)
{
    if(_bTable) {
         ParaUi.cbLtOnDk      -> Checked = Para.bLtOnDk                 ;
         ParaUi.edChkOfs      -> Text    = Para.iChkOfs                 ;
         ParaUi.edLowThsd     -> Text    = Para.iLowThsd                ;
         ParaUi.edHighThsd    -> Text    = Para.iHighThsd               ;

         ParaUi.edSampleGap   -> Text = Para.LinParaL.iSampleGap        ;
         ParaUi.edLineGap     -> Text = Para.LinParaL.iLineGap          ;

         ParaUi.edAverage     -> Text = Para.LinParaL.EdgPara.iAverage  ;
         ParaUi.edCompare     -> Text = Para.LinParaL.EdgPara.iCompare  ;
         ParaUi.edChkPxLow    -> Text = Para.LinParaL.EdgPara.iChkPxLow ;
         ParaUi.edChkPxHgh    -> Text = Para.LinParaL.EdgPara.iChkPxHgh ;

         ParaUi.edEndOffsetL  -> Text = Para.LinParaL.iEndOffset        ;
         ParaUi.edCntrOffsetL -> Text = Para.LinParaL.iCntrOffset       ;
         ParaUi.edOfsGapL     -> Text = Para.LinParaL.EdgPara.iOfsGap   ;
         ParaUi.edOfsPxLowL   -> Text = Para.LinParaL.EdgPara.iOfsPxLow ;
         ParaUi.edOfsPxHghL   -> Text = Para.LinParaL.EdgPara.iOfsPxHgh ;

         ParaUi.edEndOffsetT  -> Text = Para.LinParaT.iEndOffset        ;
         ParaUi.edCntrOffsetT -> Text = Para.LinParaT.iCntrOffset       ;
         ParaUi.edOfsGapT     -> Text = Para.LinParaT.EdgPara.iOfsGap   ;
         ParaUi.edOfsPxLowT   -> Text = Para.LinParaT.EdgPara.iOfsPxLow ;
         ParaUi.edOfsPxHghT   -> Text = Para.LinParaT.EdgPara.iOfsPxHgh ;

         ParaUi.edEndOffsetR  -> Text = Para.LinParaR.iEndOffset        ;
         ParaUi.edCntrOffsetR -> Text = Para.LinParaR.iCntrOffset       ;
         ParaUi.edOfsGapR     -> Text = Para.LinParaR.EdgPara.iOfsGap   ;
         ParaUi.edOfsPxLowR   -> Text = Para.LinParaR.EdgPara.iOfsPxLow ;
         ParaUi.edOfsPxHghR   -> Text = Para.LinParaR.EdgPara.iOfsPxHgh ;

         ParaUi.edEndOffsetB  -> Text = Para.LinParaB.iEndOffset        ;
         ParaUi.edCntrOffsetB -> Text = Para.LinParaB.iCntrOffset       ;
         ParaUi.edOfsGapB     -> Text = Para.LinParaB.EdgPara.iOfsGap   ;
         ParaUi.edOfsPxLowB   -> Text = Para.LinParaB.EdgPara.iOfsPxLow ;
         ParaUi.edOfsPxHghB   -> Text = Para.LinParaB.EdgPara.iOfsPxHgh ;
    }
    else {
        int  iTemp ;
        bool bTemp ;
        bTemp = ParaUi.cbLtOnDk      -> Checked              ; if (Para.bLtOnDk                    != bTemp) { Trace("Operator" , "Both WRT_LtOnDk values are different."); }

        iTemp = StrToIntDef(ParaUi.edChkOfs      -> Text , 0); if (Para.iChkOfs                    != iTemp) { Trace("Operator" , "Both WRT_ChkOfs values are different."); }
        iTemp = StrToIntDef(ParaUi.edLowThsd     -> Text , 0); if (Para.iLowThsd                   != iTemp) { Trace("Operator" , "Both WRT_LowThsd values are different."); }
        iTemp = StrToIntDef(ParaUi.edHighThsd    -> Text , 0); if (Para.iHighThsd                  != iTemp) { Trace("Operator" , "Both WRT_HighThsd values are different."); }


        iTemp = StrToIntDef(ParaUi.edSampleGap   -> Text , 1); if (Para.LinParaL.iSampleGap        != iTemp) { Trace("Operator" , "Both WRT_SampleGap values are different."); }
        iTemp = StrToIntDef(ParaUi.edLineGap     -> Text , 1); if (Para.LinParaL.iLineGap          != iTemp) { Trace("Operator" , "Both WRT_LineGap values are different."); }


        iTemp = StrToIntDef(ParaUi.edAverage     -> Text , 1); if (Para.LinParaL.EdgPara.iAverage  != iTemp) { Trace("Operator" , "Both WRT_Average values are different."); }
        iTemp = StrToIntDef(ParaUi.edCompare     -> Text , 1); if (Para.LinParaL.EdgPara.iCompare  != iTemp) { Trace("Operator" , "Both WRT_Compare values are different."); }
        iTemp = StrToIntDef(ParaUi.edChkPxLow    -> Text , 0); if (Para.LinParaL.EdgPara.iChkPxLow != iTemp) { Trace("Operator" , "Both WRT_ChkPxLow values are different."); }
        iTemp = StrToIntDef(ParaUi.edChkPxHgh    -> Text , 0); if (Para.LinParaL.EdgPara.iChkPxHgh != iTemp) { Trace("Operator" , "Both WRT_ChkPxHgh values are different."); }

        iTemp = StrToIntDef(ParaUi.edEndOffsetL  -> Text , 0); if (Para.LinParaL.iEndOffset        != iTemp) { Trace("Operator" , "Both WRT_EndOffsetL values are different."); }
        iTemp = StrToIntDef(ParaUi.edCntrOffsetL -> Text , 0); if (Para.LinParaL.iCntrOffset       != iTemp) { Trace("Operator" , "Both WRT_CntrOffsetL values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsGapL     -> Text , 0); if (Para.LinParaL.EdgPara.iOfsGap   != iTemp) { Trace("Operator" , "Both WRT_OfsGapL values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsPxLowL   -> Text , 0); if (Para.LinParaL.EdgPara.iOfsPxLow != iTemp) { Trace("Operator" , "Both WRT_OfsPxLowL values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsPxHghL   -> Text , 0); if (Para.LinParaL.EdgPara.iOfsPxHgh != iTemp) { Trace("Operator" , "Both WRT_OfsPxHghL values are different."); }

        iTemp = StrToIntDef(ParaUi.edEndOffsetT  -> Text , 0); if (Para.LinParaT.iEndOffset        != iTemp) { Trace("Operator" , "Both WRT_EndOffsetT values are different."); }
        iTemp = StrToIntDef(ParaUi.edCntrOffsetT -> Text , 0); if (Para.LinParaT.iCntrOffset       != iTemp) { Trace("Operator" , "Both WRT_CntrOffsetT values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsGapT     -> Text , 0); if (Para.LinParaT.EdgPara.iOfsGap   != iTemp) { Trace("Operator" , "Both WRT_OfsGapT values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsPxLowT   -> Text , 0); if (Para.LinParaT.EdgPara.iOfsPxLow != iTemp) { Trace("Operator" , "Both WRT_OfsPxLowT values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsPxHghT   -> Text , 0); if (Para.LinParaT.EdgPara.iOfsPxHgh != iTemp) { Trace("Operator" , "Both WRT_OfsPxHghT values are different."); }

        iTemp = StrToIntDef(ParaUi.edEndOffsetR  -> Text , 0); if (Para.LinParaR.iEndOffset        != iTemp) { Trace("Operator" , "Both WRT_EndOffsetR values are different."); }
        iTemp = StrToIntDef(ParaUi.edCntrOffsetR -> Text , 0); if (Para.LinParaR.iCntrOffset       != iTemp) { Trace("Operator" , "Both WRT_CntrOffsetR values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsGapR     -> Text , 0); if (Para.LinParaR.EdgPara.iOfsGap   != iTemp) { Trace("Operator" , "Both WRT_OfsGapR values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsPxLowR   -> Text , 0); if (Para.LinParaR.EdgPara.iOfsPxLow != iTemp) { Trace("Operator" , "Both WRT_OfsPxLowR values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsPxHghR   -> Text , 0); if (Para.LinParaR.EdgPara.iOfsPxHgh != iTemp) { Trace("Operator" , "Both WRT_OfsPxHghR values are different."); }

        iTemp = StrToIntDef(ParaUi.edEndOffsetB  -> Text , 0); if (Para.LinParaB.iEndOffset        != iTemp) { Trace("Operator" , "Both WRT_EndOffsetB values are different."); }
        iTemp = StrToIntDef(ParaUi.edCntrOffsetB -> Text , 0); if (Para.LinParaB.iCntrOffset       != iTemp) { Trace("Operator" , "Both WRT_CntrOffsetB values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsGapB     -> Text , 0); if (Para.LinParaB.EdgPara.iOfsGap   != iTemp) { Trace("Operator" , "Both WRT_OfsGapB values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsPxLowB   -> Text , 0); if (Para.LinParaB.EdgPara.iOfsPxLow != iTemp) { Trace("Operator" , "Both WRT_OfsPxLowB values are different."); }
        iTemp = StrToIntDef(ParaUi.edOfsPxHghB   -> Text , 0); if (Para.LinParaB.EdgPara.iOfsPxHgh != iTemp) { Trace("Operator" , "Both WRT_OfsPxHghB values are different."); }


         Para.bLtOnDk                    = ParaUi.cbLtOnDk      -> Checked         ;

         Para.iChkOfs                    = StrToIntDef(ParaUi.edChkOfs      -> Text , 0)  ;
         Para.iLowThsd                   = StrToIntDef(ParaUi.edLowThsd     -> Text , 0)  ;
         Para.iHighThsd                  = StrToIntDef(ParaUi.edHighThsd    -> Text , 0)  ;


         Para.LinParaL.iSampleGap        = StrToIntDef(ParaUi.edSampleGap   -> Text , 1 ) ;
         Para.LinParaL.iLineGap          = StrToIntDef(ParaUi.edLineGap     -> Text , 1 ) ;

         Para.LinParaL.EdgPara.iAverage  = StrToIntDef(ParaUi.edAverage     -> Text , 1 ) ;
         Para.LinParaL.EdgPara.iCompare  = StrToIntDef(ParaUi.edCompare     -> Text , 1 ) ;
         Para.LinParaL.EdgPara.iChkPxLow = StrToIntDef(ParaUi.edChkPxLow    -> Text , 0 ) ;
         Para.LinParaL.EdgPara.iChkPxHgh = StrToIntDef(ParaUi.edChkPxHgh    -> Text , 0 ) ;

         Para.LinParaL.iEndOffset        = StrToIntDef(ParaUi.edEndOffsetL  -> Text , 0 ) ;
         Para.LinParaL.iCntrOffset       = StrToIntDef(ParaUi.edCntrOffsetL -> Text , 0 ) ;
         Para.LinParaL.EdgPara.iOfsGap   = StrToIntDef(ParaUi.edOfsGapL     -> Text , 0 ) ;
         Para.LinParaL.EdgPara.iOfsPxLow = StrToIntDef(ParaUi.edOfsPxLowL   -> Text , 0 ) ;
         Para.LinParaL.EdgPara.iOfsPxHgh = StrToIntDef(ParaUi.edOfsPxHghL   -> Text , 0 ) ;

         Para.LinParaT.iEndOffset        = StrToIntDef(ParaUi.edEndOffsetT  -> Text , 0 ) ;
         Para.LinParaT.iCntrOffset       = StrToIntDef(ParaUi.edCntrOffsetT -> Text , 0 ) ;
         Para.LinParaT.EdgPara.iOfsGap   = StrToIntDef(ParaUi.edOfsGapT     -> Text , 0 ) ;
         Para.LinParaT.EdgPara.iOfsPxLow = StrToIntDef(ParaUi.edOfsPxLowT   -> Text , 0 ) ;
         Para.LinParaT.EdgPara.iOfsPxHgh = StrToIntDef(ParaUi.edOfsPxHghT   -> Text , 0 ) ;

         Para.LinParaR.iEndOffset        = StrToIntDef(ParaUi.edEndOffsetR  -> Text , 0 ) ;
         Para.LinParaR.iCntrOffset       = StrToIntDef(ParaUi.edCntrOffsetR -> Text , 0 ) ;
         Para.LinParaR.EdgPara.iOfsGap   = StrToIntDef(ParaUi.edOfsGapR     -> Text , 0 ) ;
         Para.LinParaR.EdgPara.iOfsPxLow = StrToIntDef(ParaUi.edOfsPxLowR   -> Text , 0 ) ;
         Para.LinParaR.EdgPara.iOfsPxHgh = StrToIntDef(ParaUi.edOfsPxHghR   -> Text , 0 ) ;

         Para.LinParaB.iEndOffset        = StrToIntDef(ParaUi.edEndOffsetB  -> Text , 0 ) ;
         Para.LinParaB.iCntrOffset       = StrToIntDef(ParaUi.edCntrOffsetB -> Text , 0 ) ;
         Para.LinParaB.EdgPara.iOfsGap   = StrToIntDef(ParaUi.edOfsGapB     -> Text , 0 ) ;
         Para.LinParaB.EdgPara.iOfsPxLow = StrToIntDef(ParaUi.edOfsPxLowB   -> Text , 0 ) ;
         Para.LinParaB.EdgPara.iOfsPxHgh = StrToIntDef(ParaUi.edOfsPxHghB   -> Text , 0 ) ;

         ApplyPara(true);

    }
}

bool CWrt::Process(IImage * _pImg , WRT_Rslt * _pRslt)
{
    bool bRet ;

    //Para.LinParaL.bLtToDk           = !Para.bLtOnDk                   ;
    //Para.LinParaL.iScanDirect       = lsLtToRt                        ;
    Para.LinParaL.iSampleGap        = Para.LinParaL.iSampleGap        ;
    Para.LinParaL.iLineGap          = Para.LinParaL.iLineGap          ;
    Para.LinParaL.EdgPara.iAverage  = Para.LinParaL.EdgPara.iAverage  ;
    Para.LinParaL.EdgPara.iCompare  = Para.LinParaL.EdgPara.iCompare  ;
    Para.LinParaL.EdgPara.iChkPxLow = Para.LinParaL.EdgPara.iChkPxLow ;
    Para.LinParaL.EdgPara.iChkPxHgh = Para.LinParaL.EdgPara.iChkPxHgh ;

    //Para.LinParaT.bLtToDk           = !Para.bLtOnDk                   ;
    //Para.LinParaT.iScanDirect       = lsUpToDn                        ;
    Para.LinParaT.iSampleGap        = Para.LinParaL.iSampleGap        ;
    Para.LinParaT.iLineGap          = Para.LinParaL.iLineGap          ;
    Para.LinParaT.EdgPara.iAverage  = Para.LinParaL.EdgPara.iAverage  ;
    Para.LinParaT.EdgPara.iCompare  = Para.LinParaL.EdgPara.iCompare  ;
    Para.LinParaT.EdgPara.iChkPxLow = Para.LinParaL.EdgPara.iChkPxLow ;
    Para.LinParaT.EdgPara.iChkPxHgh = Para.LinParaL.EdgPara.iChkPxHgh ;

    //Para.LinParaR.bLtToDk           = !Para.bLtOnDk                   ;
    //Para.LinParaR.iScanDirect       = lsRtToLt                        ;
    Para.LinParaR.iSampleGap        = Para.LinParaL.iSampleGap        ;
    Para.LinParaR.iLineGap          = Para.LinParaL.iLineGap          ;
    Para.LinParaR.EdgPara.iAverage  = Para.LinParaL.EdgPara.iAverage  ;
    Para.LinParaR.EdgPara.iCompare  = Para.LinParaL.EdgPara.iCompare  ;
    Para.LinParaR.EdgPara.iChkPxLow = Para.LinParaL.EdgPara.iChkPxLow ;
    Para.LinParaR.EdgPara.iChkPxHgh = Para.LinParaL.EdgPara.iChkPxHgh ;

    //Para.LinParaB.bLtToDk           = !Para.bLtOnDk                   ;
    //Para.LinParaB.iScanDirect       = lsDnToUp                        ;
    Para.LinParaB.iSampleGap        = Para.LinParaL.iSampleGap        ;
    Para.LinParaB.iLineGap          = Para.LinParaL.iLineGap          ;
    Para.LinParaB.EdgPara.iAverage  = Para.LinParaL.EdgPara.iAverage  ;
    Para.LinParaB.EdgPara.iCompare  = Para.LinParaL.EdgPara.iCompare  ;
    Para.LinParaB.EdgPara.iChkPxLow = Para.LinParaL.EdgPara.iChkPxLow ;
    Para.LinParaB.EdgPara.iChkPxHgh = Para.LinParaL.EdgPara.iChkPxHgh ;                        


    bRet = WRT_Inspect(_pImg , GetOfsRect(Tracker -> GetLTRect()) , GetOfsRect(Tracker -> GetTTRect()) , GetOfsRect(Tracker -> GetRTRect()), GetOfsRect(Tracker -> GetBTRect()) , Para , _pRslt);
    memcpy(&Rslt , _pRslt , sizeof(WRT_Rslt));

    return bRet ;
}

void CWrt::PaintRslt(IImage * _pImg , HDC _hHdc)
{
    WRT_PaintRslt(_pImg , _hHdc , &Rslt , Disp);
}

void CWrt::GetRefValOfs(float &_fX , float &_fY , float &_fT )
{
    _fX = Rslt.CenterPnt.x - m_fRefValX ;
    _fY = Rslt.CenterPnt.y - m_fRefValY ;
    _fT = Rslt.fAngle      - m_fRefValT ; //앵글 조심. 0도 짜리 는 360 대로 떨어질수 있다.
}




/******************************************************************************/
/* Threshold                                                                  */
/******************************************************************************/
CTsd::CTsd(int _iTrackerCnt)
{
    if(_iTrackerCnt <= 0) _iTrackerCnt = 1 ;

    m_iProcType = ptTsd ;

    m_iTrackerCnt = _iTrackerCnt ;

    Tracker = new TRectTracker * [_iTrackerCnt] ;

    Rslt = new TSD_Rslt[_iTrackerCnt] ;

    for(int i = 0 ; i < m_iTrackerCnt ; i++) {
        Tracker[i] = new TRectTracker() ; Tracker[i] -> Caption = "TSD"+AnsiString(i);

    }

    //MakeUI(&ParaUi);
    ParaUi.pnBase = NULL ;

    Disp.clRect = clLime ;
    Disp.clText = clLime   ;
}

CTsd::~CTsd()
{
    for(int i = 0 ; i < m_iTrackerCnt ; i++) {
        delete Tracker[i] ; Tracker[i] = NULL ;
    }

    for(int i = 0 ; i < m_iTrackerCnt ; i++) {

    }

    delete [] Tracker ;
    delete [] Rslt    ;

    Tracker = NULL ;
}

void CTsd::MakeUI(TParaUi  * _pParaUi , TWinControl * _tcControl)
{
    int iR = 0 ;
    _pParaUi->pnBase = new TPanel(_tcControl  ); _pParaUi->pnBase -> Parent = _tcControl ;

    //Line 1 ---------------------------------
    _pParaUi->lbThresholdLow  = new TLabel (_pParaUi->pnBase);  _pParaUi->edThresholdLow   = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbThresholdHigh = new TLabel (_pParaUi->pnBase);  _pParaUi->edThresholdHigh  = new TEdit(_pParaUi->pnBase);

    SetArrange(_pParaUi->lbThresholdLow  , 0 , iR);  SetArrange(_pParaUi->edThresholdLow  , 1 , iR++ );
    SetArrange(_pParaUi->lbThresholdHigh , 0 , iR);  SetArrange(_pParaUi->edThresholdHigh , 1 , iR++ );

    _pParaUi->lbThresholdLow  -> Parent = _pParaUi->pnBase ; _pParaUi->edThresholdLow  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbThresholdHigh -> Parent = _pParaUi->pnBase ; _pParaUi->edThresholdHigh -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbThresholdLow  -> Caption = "밝기값Low" ;//"iThreshold Low"  ;
    _pParaUi->lbThresholdHigh -> Caption = "밝기값High";//"iThreshold High" ;
}

void CTsd::SetArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    const int iControlWidth   = 120 ;
    const int iControlHeight  = 20  ;
    const int iStartRowOffset = 5   ;
    const int iStartColOffset = 5   ;

    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;
}

void CTsd::SetPaintCallback(TOnChanged ChangeFunc)
{
    for(int i = 0 ; i < m_iTrackerCnt ; i++) {
        Tracker[i] -> SetFunc(ChangeFunc) ;
    }

}

void CTsd::SetTrackerVisible(bool _bVisible)
{
    for(int i = 0 ; i < m_iTrackerCnt ; i++) {
        Tracker[i] -> Visible = _bVisible ;
    }
}



void CTsd::SetTrackerEnabled(bool _bEnabled)
{
    for(int i = 0 ; i < m_iTrackerCnt ; i++) {
        Tracker[i] -> Enabled = _bEnabled ;
    }
}

bool CTsd::MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    for(int i = 0 ; i < m_iTrackerCnt ; i++) {
        Tracker[i] -> Focused = false ;
    }

    for(int i = 0 ; i < m_iTrackerCnt ; i++) {
        if(Tracker[i] -> TrackerDw(Button, Shift , X,Y, _fScaleX , _fScaleY)) { m_fInspOfsX = 0.0 ; m_fInspOfsY = 0.0 ; return true ;}
    }

    return false ;
}

bool CTsd::MouseTrackerMv(TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    for(int i = 0 ; i < m_iTrackerCnt ; i++) {
        if(Tracker[i] -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY)) return true ;
    }


    return false ;
}

void CTsd::MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    for(int i = 0 ; i < m_iTrackerCnt ; i++) {
        Tracker[i] -> TrackerUp(Button, Shift );
    }
}

void CTsd::PaintTracker(HDC _hHdc , float _fScaleX , float _fScaleY)
{
    for(int i = 0 ; i < m_iTrackerCnt ; i++) {
        if(Tracker[i] -> Visible)PaintOfsRect(_hHdc , _fScaleX , _fScaleY , GetOfsRect(Tracker[i] -> GetTRect()));
        Tracker[i] -> Paint(_hHdc , _fScaleX , _fScaleY);
    }
}

void CTsd::ShowParaForm(TWinControl* _wcBase)
{
    if(ParaUi.pnBase == NULL)MakeUI(&ParaUi,_wcBase) ;
    ParaUi.pnBase -> Parent = _wcBase ;
    ParaUi.pnBase -> Align = alClient ;
}

void CTsd::LoadPara(bool _bLoad , AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId+"_Threshold" ;
    AnsiString sFilePath = _sFilePath + ".ini" ;
    AnsiString sTrakPath = _sFilePath + "Tracker.ini" ;

    for(int i = 0 ; i < m_iTrackerCnt ; i++) {
        Tracker[i] -> OnLoadSave(_bLoad , sTrakPath , sTemp + "_Tracker" + AnsiString(i));
    }

    int iTemp ;

    if(_bLoad) {
        UserINI.Load(sFilePath, sTemp , "iThresholdLow ", Para.iThresholdLow );
        UserINI.Load(sFilePath, sTemp , "iThresholdHigh", Para.iThresholdHigh);
    }
    else {
        UserINI.Save(sFilePath, sTemp , "iThresholdLow ", Para.iThresholdLow );
        UserINI.Save(sFilePath, sTemp , "iThresholdHigh", Para.iThresholdHigh);
    }


}

void CTsd::ApplyPara(bool _bTable)
{
    if(_bTable) {
        ParaUi.edThresholdLow   -> Text = Para.iThresholdLow  ;
        ParaUi.edThresholdHigh  -> Text = Para.iThresholdHigh ;

    }
    else {
        int iTemp;
        iTemp = StrToIntDef(ParaUi.edThresholdLow  -> Text , 1 ); if (Para.iThresholdLow  != iTemp) { Trace("Operator" , "Both Tsd_ThresholdLow values are different."); }
        iTemp = StrToIntDef(ParaUi.edThresholdHigh -> Text , 1 ); if (Para.iThresholdHigh != iTemp) { Trace("Operator" , "Both Tsd_ThresholdHigh values are different."); }

        Para.iThresholdLow  = StrToIntDef(ParaUi.edThresholdLow  -> Text , 1 );
        Para.iThresholdHigh = StrToIntDef(ParaUi.edThresholdHigh -> Text , 1 );

        ApplyPara(true);

    }
}

bool CTsd::Process(IImage * _pImg , TSD_Rslt * _pRslt)
{
    bool bRet ;

    for(int i = 0 ; i < m_iTrackerCnt ; i++) {
        memset (&Rslt[i] , 0 , sizeof(TSD_Rslt)) ;
    }

    TSD_Rslt TempRslt ;
    memset(&TempRslt , 0 , sizeof(TSD_Rslt));

    int  iSum      = 0 ;
    int  iPxCntSum = 0 ;
    int  iPxCnt    = 0 ;


    for(int i = 0 ; i < m_iTrackerCnt ; i++) {
        bRet = TSD_Inspect(_pImg , GetOfsRect(Tracker[i] -> GetTRect()) , Para , _pRslt);
        memcpy(&Rslt[i] , _pRslt , sizeof(TSD_Rslt));

        TempRslt.iPxCnt    += Rslt[i].iPxCnt ;
        TempRslt.fInspTime += Rslt[i].fInspTime ;
        iPxCnt  = Tracker[i] -> Width * Tracker[i] -> Height ;
        iSum   += iPxCnt * Rslt[i].fAverage ;
        iPxCnt += iPxCnt ;

        if(!bRet) return bRet ;
    }

    TempRslt.fAverage = iSum / iPxCnt ;

    _pRslt -> iPxCnt    = TempRslt.iPxCnt    ;
    _pRslt -> fAverage  = TempRslt.fAverage  ;
    _pRslt -> fInspTime = TempRslt.fInspTime ;

    return bRet ;
}

void CTsd::PaintRslt(IImage * _pImg , HDC _hHdc)
{
    for(int i = 0 ; i < m_iTrackerCnt ; i++) {
        TSD_PaintRslt(_pImg , _hHdc , &Rslt[i] , Disp);
    }
}

void CTsd::GetRslt(int _iTrackerId , TSD_Rslt * _pRslt)
{
    memcpy(_pRslt , &Rslt[_iTrackerId] , sizeof(TSD_Rslt));
}


/******************************************************************************/
/* Blob                                                                       */
/******************************************************************************/
CBlb::CBlb()
{
    m_iProcType = ptBlb ;

    Tracker = new TRectTracker() ; Tracker -> Caption = "BLB";

    //MakeUI(&ParaUi);
    ParaUi.pnBase = NULL ;

    Rslt.pBlobs   = NULL ;
    Rslt.iBlobCnt = 0 ;

    Disp.clBoundary = clYellow ;
    Disp.clRect     = clLime   ;
    Disp.clCross    = clLime   ;
    Disp.clText     = clLime   ;
}

CBlb::~CBlb()
{
    delete Tracker ;
}

void CBlb::MakeUI(TParaUi  * _pParaUi , TWinControl * _tcControl)
{
    int iR = 0 ;
    _pParaUi->pnBase = new TPanel(_tcControl  ); _pParaUi->pnBase -> Parent = _tcControl ;

    _pParaUi->lbPitchX     = new TLabel(_pParaUi->pnBase); _pParaUi->edPitchX     = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbPitchY     = new TLabel(_pParaUi->pnBase); _pParaUi->edPitchY     = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbStartXOfs  = new TLabel(_pParaUi->pnBase); _pParaUi->edStartXOfs  = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbStartYOfs  = new TLabel(_pParaUi->pnBase); _pParaUi->edStartYOfs  = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbThreshold  = new TLabel(_pParaUi->pnBase); _pParaUi->edThreshold  = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbFindLtOnDk = new TLabel(_pParaUi->pnBase); _pParaUi->cbFindLtOnDk = new TCheckBox(_pParaUi->pnBase);
    _pParaUi->lbIgnrInside = new TLabel(_pParaUi->pnBase); _pParaUi->cbIgnrInside = new TCheckBox(_pParaUi->pnBase);
    _pParaUi->lbIgnrSide   = new TLabel(_pParaUi->pnBase); _pParaUi->cbIgnrSide   = new TCheckBox(_pParaUi->pnBase);

    _pParaUi->lbMinArea        = new TLabel(_pParaUi->pnBase); _pParaUi->edMinArea        = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxArea        = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbMinWidth       = new TLabel(_pParaUi->pnBase); _pParaUi->edMinWidth       = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxWidth       = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbMinHeight      = new TLabel(_pParaUi->pnBase); _pParaUi->edMinHeight      = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxHeight      = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbMinBoundary    = new TLabel(_pParaUi->pnBase); _pParaUi->edMinBoundary    = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxBoundary    = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbMinCompactness = new TLabel(_pParaUi->pnBase); _pParaUi->edMinCompactness = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxCompactness = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbMinAngle       = new TLabel(_pParaUi->pnBase); _pParaUi->edMinAngle       = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxAngle       = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbMinAspectRatio = new TLabel(_pParaUi->pnBase); _pParaUi->edMinAspectRatio = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxAspectRatio = new TEdit(_pParaUi->pnBase);

    SetArrange(_pParaUi->lbPitchX     , 0 , iR  );  SetArrange(_pParaUi->edPitchX     , 1 , iR++ );
    SetArrange(_pParaUi->lbPitchY     , 0 , iR  );  SetArrange(_pParaUi->edPitchY     , 1 , iR++ );
    SetArrange(_pParaUi->lbStartXOfs  , 0 , iR  );  SetArrange(_pParaUi->edStartXOfs  , 1 , iR++ );
    SetArrange(_pParaUi->lbStartYOfs  , 0 , iR  );  SetArrange(_pParaUi->edStartYOfs  , 1 , iR++ );
    SetArrange(_pParaUi->lbThreshold  , 0 , iR  );  SetArrange(_pParaUi->edThreshold  , 1 , iR++ );
    SetArrange(_pParaUi->lbFindLtOnDk , 0 , iR  );  SetArrange(_pParaUi->cbFindLtOnDk , 1 , iR++ );
    SetArrange(_pParaUi->lbIgnrInside , 0 , iR  );  SetArrange(_pParaUi->cbIgnrInside , 1 , iR++ );
    SetArrange(_pParaUi->lbIgnrSide   , 0 , iR  );  SetArrange(_pParaUi->cbIgnrSide   , 1 , iR++ );

    iR++;
    SetArrange(_pParaUi->lbMinArea        , 0 , iR  );  SetArrange(_pParaUi->edMinArea        , 1 , iR ); SetArrange(_pParaUi->edMaxArea        , 2 , iR++ );
    SetArrange(_pParaUi->lbMinWidth       , 0 , iR  );  SetArrange(_pParaUi->edMinWidth       , 1 , iR ); SetArrange(_pParaUi->edMaxWidth       , 2 , iR++ );
    SetArrange(_pParaUi->lbMinHeight      , 0 , iR  );  SetArrange(_pParaUi->edMinHeight      , 1 , iR ); SetArrange(_pParaUi->edMaxHeight      , 2 , iR++ );
    SetArrange(_pParaUi->lbMinBoundary    , 0 , iR  );  SetArrange(_pParaUi->edMinBoundary    , 1 , iR ); SetArrange(_pParaUi->edMaxBoundary    , 2 , iR++ );
    SetArrange(_pParaUi->lbMinCompactness , 0 , iR  );  SetArrange(_pParaUi->edMinCompactness , 1 , iR ); SetArrange(_pParaUi->edMaxCompactness , 2 , iR++ );
    SetArrange(_pParaUi->lbMinAngle       , 0 , iR  );  SetArrange(_pParaUi->edMinAngle       , 1 , iR ); SetArrange(_pParaUi->edMaxAngle       , 2 , iR++ );
    SetArrange(_pParaUi->lbMinAspectRatio , 0 , iR  );  SetArrange(_pParaUi->edMinAspectRatio , 1 , iR ); SetArrange(_pParaUi->edMaxAspectRatio , 2 , iR++ );


    _pParaUi->lbPitchX     -> Parent = _pParaUi->pnBase ; _pParaUi->edPitchX     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbPitchY     -> Parent = _pParaUi->pnBase ; _pParaUi->edPitchY     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbStartXOfs  -> Parent = _pParaUi->pnBase ; _pParaUi->edStartXOfs  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbStartYOfs  -> Parent = _pParaUi->pnBase ; _pParaUi->edStartYOfs  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbThreshold  -> Parent = _pParaUi->pnBase ; _pParaUi->edThreshold  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbFindLtOnDk -> Parent = _pParaUi->pnBase ; _pParaUi->cbFindLtOnDk -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbIgnrInside -> Parent = _pParaUi->pnBase ; _pParaUi->cbIgnrInside -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbIgnrSide   -> Parent = _pParaUi->pnBase ; _pParaUi->cbIgnrSide   -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbMinArea        -> Parent = _pParaUi->pnBase ; _pParaUi->edMinArea        -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxArea        -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbMinWidth       -> Parent = _pParaUi->pnBase ; _pParaUi->edMinWidth       -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxWidth       -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbMinHeight      -> Parent = _pParaUi->pnBase ; _pParaUi->edMinHeight      -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxHeight      -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbMinBoundary    -> Parent = _pParaUi->pnBase ; _pParaUi->edMinBoundary    -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxBoundary    -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbMinCompactness -> Parent = _pParaUi->pnBase ; _pParaUi->edMinCompactness -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxCompactness -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbMinAngle       -> Parent = _pParaUi->pnBase ; _pParaUi->edMinAngle       -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxAngle       -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbMinAspectRatio -> Parent = _pParaUi->pnBase ; _pParaUi->edMinAspectRatio -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxAspectRatio -> Parent = _pParaUi->pnBase ;



    _pParaUi->lbPitchX         -> Caption = "X축 검색간격           " ;//"PitchX        ";
    _pParaUi->lbPitchY         -> Caption = "Y축 검색간격           " ;//"PitchY        ";
    _pParaUi->lbStartXOfs      -> Caption = "X축 검색시작옵셑       " ;//"StartXOfs     ";
    _pParaUi->lbStartYOfs      -> Caption = "Y축 검색시작옵셑       " ;//"StartYOfs     ";
    _pParaUi->lbThreshold      -> Caption = "검색 밝기값            " ;//"Threshold     ";
    _pParaUi->lbFindLtOnDk     -> Caption = "밝은물체찾기           " ;//"FindLtOnDk    ";
    _pParaUi->lbIgnrInside     -> Caption = "블랍내부무시           " ;//"IgnrInside    ";
    _pParaUi->lbIgnrSide       -> Caption = "ROI벽면에 걸친물체무시 " ;//"IgnrSide      ";

    _pParaUi->lbMinArea        -> Caption = "영역개수    "  ;//"Area       ";
    _pParaUi->lbMinWidth       -> Caption = "넓이        "  ;//"Width      ";
    _pParaUi->lbMinHeight      -> Caption = "높이        "  ;//"Height     ";
    _pParaUi->lbMinBoundary    -> Caption = "테두리길이  "  ;//"Boundary   ";
    _pParaUi->lbMinCompactness -> Caption = "Compactness "  ;
    _pParaUi->lbMinAngle       -> Caption = "Angle       "  ;
    _pParaUi->lbMinAspectRatio -> Caption = "AspectRatio "  ;

}

void CBlb::SetArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    const int iControlWidth   = 120 ;
    const int iControlHeight  = 20  ;
    const int iStartRowOffset = 5   ;
    const int iStartColOffset = 5   ;

    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;
}

void CBlb::SetPaintCallback(TOnChanged ChangeFunc)
{
    Tracker -> SetFunc(ChangeFunc) ;
}

void CBlb::SetTrackerVisible(bool _bVisible)
{
    Tracker -> Visible = _bVisible ;
}



void CBlb::SetTrackerEnabled(bool _bEnabled)
{
    Tracker -> Enabled = _bEnabled ;
}

bool CBlb::MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker -> Focused = false ;

    if(Tracker -> TrackerDw(Button, Shift , X,Y, _fScaleX , _fScaleY)) { m_fInspOfsX = 0.0 ; m_fInspOfsY = 0.0 ; return true ;}

    return false ;
}

bool CBlb::MouseTrackerMv(TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    if(Tracker -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY)) return true ;

    return false ;
}

void CBlb::MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker -> TrackerUp(Button, Shift );
}

void CBlb::PaintTracker(HDC _hHdc , float _fScaleX , float _fScaleY)
{
    if(Tracker -> Visible)PaintOfsRect(_hHdc , _fScaleX , _fScaleY , GetOfsRect(Tracker -> GetTRect()));
    Tracker -> Paint(_hHdc , _fScaleX , _fScaleY);
}

void CBlb::ShowParaForm(TWinControl* _wcBase)
{
    if(ParaUi.pnBase == NULL)MakeUI(&ParaUi,_wcBase) ;
    ParaUi.pnBase -> Parent = _wcBase ;
    ParaUi.pnBase -> Align = alClient ;
}

void CBlb::LoadPara (bool _bLoad , AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId+"_Blob" ;
    AnsiString sFilePath = _sFilePath + ".ini" ;
    AnsiString sTrakPath = _sFilePath + "Tracker.ini" ;

    Tracker -> OnLoadSave(_bLoad , sTrakPath , sTemp + "_Tracker");

    if(_bLoad) {
        UserINI.Load(sFilePath, sTemp , "Para.iPitchX    ", Para.iPitchX    );
        UserINI.Load(sFilePath, sTemp , "Para.iPitchY    ", Para.iPitchY    );
        UserINI.Load(sFilePath, sTemp , "Para.iStartXOfs ", Para.iStartXOfs );
        UserINI.Load(sFilePath, sTemp , "Para.iStartYOfs ", Para.iStartYOfs );
        UserINI.Load(sFilePath, sTemp , "Para.iThreshold ", Para.iThreshold );
        UserINI.Load(sFilePath, sTemp , "Para.bFindLtOnDk", Para.bFindLtOnDk);
        UserINI.Load(sFilePath, sTemp , "Para.bIgnrInside", Para.bIgnrInside);
        UserINI.Load(sFilePath, sTemp , "Para.bIgnrSide  ", Para.bIgnrSide  );

        UserINI.Load(sFilePath, sTemp , "Fltr.iMinArea       ", Fltr.iMinArea       );UserINI.Load(sFilePath, sTemp , "Fltr.iMaxArea       ", Fltr.iMaxArea       );
        UserINI.Load(sFilePath, sTemp , "Fltr.iMinWidth      ", Fltr.iMinWidth      );UserINI.Load(sFilePath, sTemp , "Fltr.iMaxWidth      ", Fltr.iMaxWidth      );
        UserINI.Load(sFilePath, sTemp , "Fltr.iMinHeight     ", Fltr.iMinHeight     );UserINI.Load(sFilePath, sTemp , "Fltr.iMaxHeight     ", Fltr.iMaxHeight     );
        UserINI.Load(sFilePath, sTemp , "Fltr.iMinBoundary   ", Fltr.iMinBoundary   );UserINI.Load(sFilePath, sTemp , "Fltr.iMaxBoundary   ", Fltr.iMaxBoundary   );
        UserINI.Load(sFilePath, sTemp , "Fltr.fMinCompactness", Fltr.fMinCompactness);UserINI.Load(sFilePath, sTemp , "Fltr.fMaxCompactness", Fltr.fMaxCompactness);
        UserINI.Load(sFilePath, sTemp , "Fltr.fMinAngle      ", Fltr.fMinAngle      );UserINI.Load(sFilePath, sTemp , "Fltr.fMaxAngle      ", Fltr.fMaxAngle      );
        UserINI.Load(sFilePath, sTemp , "Fltr.fMinAspectRatio", Fltr.fMinAspectRatio);UserINI.Load(sFilePath, sTemp , "Fltr.fMaxAspectRatio", Fltr.fMaxAspectRatio);
    }
    else {
        UserINI.Save(sFilePath, sTemp , "Para.iPitchX    ", Para.iPitchX    );
        UserINI.Save(sFilePath, sTemp , "Para.iPitchY    ", Para.iPitchY    );
        UserINI.Save(sFilePath, sTemp , "Para.iStartXOfs ", Para.iStartXOfs );
        UserINI.Save(sFilePath, sTemp , "Para.iStartYOfs ", Para.iStartYOfs );
        UserINI.Save(sFilePath, sTemp , "Para.iThreshold ", Para.iThreshold );
        UserINI.Save(sFilePath, sTemp , "Para.bFindLtOnDk", Para.bFindLtOnDk);
        UserINI.Save(sFilePath, sTemp , "Para.bIgnrInside", Para.bIgnrInside);
        UserINI.Save(sFilePath, sTemp , "Para.bIgnrSide  ", Para.bIgnrSide  );

        UserINI.Save(sFilePath, sTemp , "Fltr.iMinArea       ", Fltr.iMinArea       );UserINI.Save(sFilePath, sTemp , "Fltr.iMaxArea       ", Fltr.iMaxArea       );
        UserINI.Save(sFilePath, sTemp , "Fltr.iMinWidth      ", Fltr.iMinWidth      );UserINI.Save(sFilePath, sTemp , "Fltr.iMaxWidth      ", Fltr.iMaxWidth      );
        UserINI.Save(sFilePath, sTemp , "Fltr.iMinHeight     ", Fltr.iMinHeight     );UserINI.Save(sFilePath, sTemp , "Fltr.iMaxHeight     ", Fltr.iMaxHeight     );
        UserINI.Save(sFilePath, sTemp , "Fltr.iMinBoundary   ", Fltr.iMinBoundary   );UserINI.Save(sFilePath, sTemp , "Fltr.iMaxBoundary   ", Fltr.iMaxBoundary   );
        UserINI.Save(sFilePath, sTemp , "Fltr.fMinCompactness", Fltr.fMinCompactness);UserINI.Save(sFilePath, sTemp , "Fltr.fMaxCompactness", Fltr.fMaxCompactness);
        UserINI.Save(sFilePath, sTemp , "Fltr.fMinAngle      ", Fltr.fMinAngle      );UserINI.Save(sFilePath, sTemp , "Fltr.fMaxAngle      ", Fltr.fMaxAngle      );
        UserINI.Save(sFilePath, sTemp , "Fltr.fMinAspectRatio", Fltr.fMinAspectRatio);UserINI.Save(sFilePath, sTemp , "Fltr.fMaxAspectRatio", Fltr.fMaxAspectRatio);
    }
}

void CBlb::ApplyPara(bool _bTable)
{
    if(_bTable) {
        ParaUi.edPitchX     -> Text    = Para.iPitchX    ;
        ParaUi.edPitchY     -> Text    = Para.iPitchY    ;
        ParaUi.edStartXOfs  -> Text    = Para.iStartXOfs ;
        ParaUi.edStartYOfs  -> Text    = Para.iStartYOfs ;
        ParaUi.edThreshold  -> Text    = Para.iThreshold ;
        ParaUi.cbFindLtOnDk -> Checked = Para.bFindLtOnDk;
        ParaUi.cbIgnrInside -> Checked = Para.bIgnrInside;
        ParaUi.cbIgnrSide   -> Checked = Para.bIgnrSide  ;

        ParaUi.edMinArea        -> Text = Fltr.iMinArea        ; ParaUi.edMaxArea          -> Text = Fltr.iMaxArea        ;
        ParaUi.edMinWidth       -> Text = Fltr.iMinWidth       ; ParaUi.edMaxWidth         -> Text = Fltr.iMaxWidth       ;
        ParaUi.edMinHeight      -> Text = Fltr.iMinHeight      ; ParaUi.edMaxHeight        -> Text = Fltr.iMaxHeight      ;
        ParaUi.edMinBoundary    -> Text = Fltr.iMinBoundary    ; ParaUi.edMaxBoundary      -> Text = Fltr.iMaxBoundary    ;
        ParaUi.edMinCompactness -> Text = Fltr.fMinCompactness ; ParaUi.edMaxCompactness   -> Text = Fltr.fMaxCompactness ;
        ParaUi.edMinAngle       -> Text = Fltr.fMinAngle       ; ParaUi.edMaxAngle         -> Text = Fltr.fMaxAngle       ;
        ParaUi.edMinAspectRatio -> Text = Fltr.fMinAspectRatio ; ParaUi.edMaxAspectRatio   -> Text = Fltr.fMaxAspectRatio ;
    }
    else {
        int   iTemp ;
        bool  bTemp ;
        float fTemp ;

        iTemp = StrToIntDef (ParaUi.edPitchX          -> Text , 1  );     if (Para.iPitchX         != iTemp) { Trace("Operator" , "Both Blb_PitchX values are different."); }
        iTemp = StrToIntDef (ParaUi.edPitchY          -> Text , 1  );     if (Para.iPitchY         != iTemp) { Trace("Operator" , "Both Blb_PitchY values are different."); }
        iTemp = StrToIntDef (ParaUi.edStartXOfs       -> Text , 0  );     if (Para.iStartXOfs      != iTemp) { Trace("Operator" , "Both Blb_StartXOfs values are different."); }
        iTemp = StrToIntDef (ParaUi.edStartYOfs       -> Text , 0  );     if (Para.iStartYOfs      != iTemp) { Trace("Operator" , "Both Blb_StartYOfs values are different."); }
        iTemp = StrToIntDef (ParaUi.edThreshold       -> Text , 100);     if (Para.iThreshold      != iTemp) { Trace("Operator" , "Both Blb_Threshold values are different."); }
        bTemp =              ParaUi.cbFindLtOnDk      -> Checked    ;     if (Para.bFindLtOnDk     != bTemp) { Trace("Operator" , "Both Blb_FindLtOnDk values are different."); }
        bTemp =              ParaUi.cbIgnrInside      -> Checked    ;     if (Para.bIgnrInside     != bTemp) { Trace("Operator" , "Both Blb_IgnrInside values are different."); }
        bTemp =              ParaUi.cbIgnrSide        -> Checked    ;     if (Para.bIgnrSide       != bTemp) { Trace("Operator" , "Both Blb_IgnrSide values are different."); }

        iTemp = StrToIntDef  (ParaUi.edMinArea        -> Text , 0 ); if (Fltr.iMinArea        != iTemp) { Trace("Operator" , "Both Blb_MinArea values are different."); }
        iTemp = StrToIntDef  (ParaUi.edMinWidth       -> Text , 0 ); if (Fltr.iMinWidth       != iTemp) { Trace("Operator" , "Both Blb_MinWidth values are different."); }
        iTemp = StrToIntDef  (ParaUi.edMinHeight      -> Text , 0 ); if (Fltr.iMinHeight      != iTemp) { Trace("Operator" , "Both Blb_MinHeight values are different."); }
        iTemp = StrToIntDef  (ParaUi.edMinBoundary    -> Text , 0 ); if (Fltr.iMinBoundary    != iTemp) { Trace("Operator" , "Both Blb_MinBoundary values are different."); }
        fTemp = StrToFloatDef(ParaUi.edMinCompactness -> Text , 0 ); if (Fltr.fMinCompactness != fTemp) { Trace("Operator" , "Both Blb_MinCompactness values are different."); }
        fTemp = StrToFloatDef(ParaUi.edMinAngle       -> Text , 0 ); if (Fltr.fMinAngle       != fTemp) { Trace("Operator" , "Both Blb_MinAngle values are different."); }
        fTemp = StrToFloatDef(ParaUi.edMinAspectRatio -> Text , 0 ); if (Fltr.fMinAspectRatio != fTemp) { Trace("Operator" , "Both Blb_MinAspectRatio values are different."); }

        iTemp = StrToIntDef  (ParaUi.edMaxArea        -> Text , 0 ); if (Fltr.iMaxArea        != iTemp) { Trace("Operator" , "Both Blb_MaxArea values are different."); }
        iTemp = StrToIntDef  (ParaUi.edMaxWidth       -> Text , 0 ); if (Fltr.iMaxWidth       != iTemp) { Trace("Operator" , "Both Blb_MaxWidth values are different."); }
        iTemp = StrToIntDef  (ParaUi.edMaxHeight      -> Text , 0 ); if (Fltr.iMaxHeight      != iTemp) { Trace("Operator" , "Both Blb_MaxHeight values are different."); }
        iTemp = StrToIntDef  (ParaUi.edMaxBoundary    -> Text , 0 ); if (Fltr.iMaxBoundary    != iTemp) { Trace("Operator" , "Both Blb_MaxBoundary values are different."); }
        fTemp = StrToFloatDef(ParaUi.edMaxCompactness -> Text , 0 ); if (Fltr.fMaxCompactness != fTemp) { Trace("Operator" , "Both Blb_MaxCompactness values are different."); }
        fTemp = StrToFloatDef(ParaUi.edMaxAngle       -> Text , 0 ); if (Fltr.fMaxAngle       != fTemp) { Trace("Operator" , "Both Blb_MaxAngle values are different."); }
        fTemp = StrToFloatDef(ParaUi.edMaxAspectRatio -> Text , 0 ); if (Fltr.fMaxAspectRatio != fTemp) { Trace("Operator" , "Both Blb_MaxAspectRatio values are different."); }

        Para.iPitchX     = StrToIntDef (ParaUi.edPitchX     -> Text , 1  ) ;
        Para.iPitchY     = StrToIntDef (ParaUi.edPitchY     -> Text , 1  ) ;
        Para.iStartXOfs  = StrToIntDef (ParaUi.edStartXOfs  -> Text , 0  ) ;
        Para.iStartYOfs  = StrToIntDef (ParaUi.edStartYOfs  -> Text , 0  ) ;
        Para.iThreshold  = StrToIntDef (ParaUi.edThreshold  -> Text , 100) ;
        Para.bFindLtOnDk =              ParaUi.cbFindLtOnDk -> Checked     ;
        Para.bIgnrInside =              ParaUi.cbIgnrInside -> Checked     ;
        Para.bIgnrSide   =              ParaUi.cbIgnrSide   -> Checked     ;

        Fltr.iMinArea        = StrToIntDef  (ParaUi.edMinArea        -> Text , 0 ) ;
        Fltr.iMinWidth       = StrToIntDef  (ParaUi.edMinWidth       -> Text , 0 ) ;
        Fltr.iMinHeight      = StrToIntDef  (ParaUi.edMinHeight      -> Text , 0 ) ;
        Fltr.iMinBoundary    = StrToIntDef  (ParaUi.edMinBoundary    -> Text , 0 ) ;
        Fltr.fMinCompactness = StrToFloatDef(ParaUi.edMinCompactness -> Text , 0 ) ;
        Fltr.fMinAngle       = StrToFloatDef(ParaUi.edMinAngle       -> Text , 0 ) ;
        Fltr.fMinAspectRatio = StrToFloatDef(ParaUi.edMinAspectRatio -> Text , 0 ) ;


        Fltr.iMaxArea        = StrToIntDef  (ParaUi.edMaxArea        -> Text , 0 ) ;
        Fltr.iMaxWidth       = StrToIntDef  (ParaUi.edMaxWidth       -> Text , 0 ) ;
        Fltr.iMaxHeight      = StrToIntDef  (ParaUi.edMaxHeight      -> Text , 0 ) ;
        Fltr.iMaxBoundary    = StrToIntDef  (ParaUi.edMaxBoundary    -> Text , 0 ) ;
        Fltr.fMaxCompactness = StrToFloatDef(ParaUi.edMaxCompactness -> Text , 0 ) ;
        Fltr.fMaxAngle       = StrToFloatDef(ParaUi.edMaxAngle       -> Text , 0 ) ;
        Fltr.fMaxAspectRatio = StrToFloatDef(ParaUi.edMaxAspectRatio -> Text , 0 ) ;
        ApplyPara(true);
    }
}

bool CBlb::Process(IImage * _pImg , BLB_Rslt * _pRslt)
{
    bool bRet ;

    bRet = BLB_Inspect(_pImg , GetOfsRect(Tracker -> GetTRect()) , Para , Fltr , _pRslt);

    //memcpy(&Rslt , _pRslt , sizeof(BLB_Rslt));

    if(Rslt.pBlobs) {
        for(int i = 0 ; i < Rslt.iBlobCnt ; i++) {
            Rslt.pBlobs[i].OnClear();
        }
        delete [] Rslt.pBlobs ;
        Rslt.pBlobs   = NULL  ;
        Rslt.iBlobCnt = 0 ;
    }



    Rslt.iBlobCnt  = _pRslt->iBlobCnt  ;
    Rslt.fInspTime = _pRslt->fInspTime ;
    Rslt.pBlobs    = new SBlob[Rslt.iBlobCnt];
    for(int i = 0 ; i <Rslt.iBlobCnt ; i++) {
        Rslt.pBlobs[i] = _pRslt->pBlobs[i] ;
    }



    return bRet ;
}

void CBlb::PaintRslt(IImage * _pImg , HDC _hHdc)
{
    BLB_PaintRslt(_pImg , _hHdc , &Rslt , Disp);
}




/******************************************************************************/
/* Calibration                                                                */
/******************************************************************************/
CCal::CCal()
{
    m_iProcType = ptCal ;

    Tracker = new TRectTracker() ; Tracker -> Caption = "CAL";

    //MakeUI(&ParaUi);
    ParaUi.pnBase = NULL ;

    Disp.BlbDisp.clBoundary = clYellow ;
    Disp.BlbDisp.clRect     = clLime   ;
    Disp.BlbDisp.clCross    = clLime   ;
    Disp.BlbDisp.clText     = clLime   ;
    Disp.clText             = clRed    ;
}

CCal::~CCal()
{
    delete Tracker ;
}

void CCal::MakeUI(TParaUi  * _pParaUi , TWinControl * _tcControl)
{
    int iR = 0 ;
    _pParaUi->pnBase = new TPanel(_tcControl  ); _pParaUi->pnBase -> Parent = _tcControl ;

    _pParaUi->lbDotSize    = new TLabel(_pParaUi->pnBase); _pParaUi->edDotSize    = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbDotPitch   = new TLabel(_pParaUi->pnBase); _pParaUi->edDotPitch   = new TEdit    (_pParaUi->pnBase);

    _pParaUi->lbPitchX     = new TLabel(_pParaUi->pnBase); _pParaUi->edPitchX     = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbPitchY     = new TLabel(_pParaUi->pnBase); _pParaUi->edPitchY     = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbStartXOfs  = new TLabel(_pParaUi->pnBase); _pParaUi->edStartXOfs  = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbStartYOfs  = new TLabel(_pParaUi->pnBase); _pParaUi->edStartYOfs  = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbThreshold  = new TLabel(_pParaUi->pnBase); _pParaUi->edThreshold  = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbFindLtOnDk = new TLabel(_pParaUi->pnBase); _pParaUi->cbFindLtOnDk = new TCheckBox(_pParaUi->pnBase);
    _pParaUi->lbIgnrInside = new TLabel(_pParaUi->pnBase); _pParaUi->cbIgnrInside = new TCheckBox(_pParaUi->pnBase);
    _pParaUi->lbIgnrSide   = new TLabel(_pParaUi->pnBase); _pParaUi->cbIgnrSide   = new TCheckBox(_pParaUi->pnBase);

    _pParaUi->lbMinArea        = new TLabel(_pParaUi->pnBase); _pParaUi->edMinArea        = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxArea        = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbMinWidth       = new TLabel(_pParaUi->pnBase); _pParaUi->edMinWidth       = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxWidth       = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbMinHeight      = new TLabel(_pParaUi->pnBase); _pParaUi->edMinHeight      = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxHeight      = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbMinBoundary    = new TLabel(_pParaUi->pnBase); _pParaUi->edMinBoundary    = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxBoundary    = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbMinCompactness = new TLabel(_pParaUi->pnBase); _pParaUi->edMinCompactness = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxCompactness = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbMinAngle       = new TLabel(_pParaUi->pnBase); _pParaUi->edMinAngle       = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxAngle       = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbMinAspectRatio = new TLabel(_pParaUi->pnBase); _pParaUi->edMinAspectRatio = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxAspectRatio = new TEdit(_pParaUi->pnBase);

    SetArrange(_pParaUi->lbDotSize    , 0 , iR  );  SetArrange(_pParaUi->edDotSize    , 1 , iR++ );
    SetArrange(_pParaUi->lbDotPitch   , 0 , iR  );  SetArrange(_pParaUi->edDotPitch   , 1 , iR++ );

    iR++;
    SetArrange(_pParaUi->lbPitchX     , 0 , iR  );  SetArrange(_pParaUi->edPitchX     , 1 , iR++ );
    SetArrange(_pParaUi->lbPitchY     , 0 , iR  );  SetArrange(_pParaUi->edPitchY     , 1 , iR++ );
    SetArrange(_pParaUi->lbStartXOfs  , 0 , iR  );  SetArrange(_pParaUi->edStartXOfs  , 1 , iR++ );
    SetArrange(_pParaUi->lbStartYOfs  , 0 , iR  );  SetArrange(_pParaUi->edStartYOfs  , 1 , iR++ );
    SetArrange(_pParaUi->lbThreshold  , 0 , iR  );  SetArrange(_pParaUi->edThreshold  , 1 , iR++ );
    SetArrange(_pParaUi->lbFindLtOnDk , 0 , iR  );  SetArrange(_pParaUi->cbFindLtOnDk , 1 , iR++ );
    SetArrange(_pParaUi->lbIgnrInside , 0 , iR  );  SetArrange(_pParaUi->cbIgnrInside , 1 , iR++ );
    SetArrange(_pParaUi->lbIgnrSide   , 0 , iR  );  SetArrange(_pParaUi->cbIgnrSide   , 1 , iR++ );

    iR++;
    SetArrange(_pParaUi->lbMinArea        , 0 , iR  );  SetArrange(_pParaUi->edMinArea        , 1 , iR ); SetArrange(_pParaUi->edMaxArea        , 2 , iR++ );
    SetArrange(_pParaUi->lbMinWidth       , 0 , iR  );  SetArrange(_pParaUi->edMinWidth       , 1 , iR ); SetArrange(_pParaUi->edMaxWidth       , 2 , iR++ );
    SetArrange(_pParaUi->lbMinHeight      , 0 , iR  );  SetArrange(_pParaUi->edMinHeight      , 1 , iR ); SetArrange(_pParaUi->edMaxHeight      , 2 , iR++ );
    SetArrange(_pParaUi->lbMinBoundary    , 0 , iR  );  SetArrange(_pParaUi->edMinBoundary    , 1 , iR ); SetArrange(_pParaUi->edMaxBoundary    , 2 , iR++ );
    SetArrange(_pParaUi->lbMinCompactness , 0 , iR  );  SetArrange(_pParaUi->edMinCompactness , 1 , iR ); SetArrange(_pParaUi->edMaxCompactness , 2 , iR++ );
    SetArrange(_pParaUi->lbMinAngle       , 0 , iR  );  SetArrange(_pParaUi->edMinAngle       , 1 , iR ); SetArrange(_pParaUi->edMaxAngle       , 2 , iR++ );
    SetArrange(_pParaUi->lbMinAspectRatio , 0 , iR  );  SetArrange(_pParaUi->edMinAspectRatio , 1 , iR ); SetArrange(_pParaUi->edMaxAspectRatio , 2 , iR++ );


    _pParaUi->lbDotSize    -> Parent = _pParaUi->pnBase ; _pParaUi->edDotSize    -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbDotPitch   -> Parent = _pParaUi->pnBase ; _pParaUi->edDotPitch   -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbPitchX     -> Parent = _pParaUi->pnBase ; _pParaUi->edPitchX     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbPitchY     -> Parent = _pParaUi->pnBase ; _pParaUi->edPitchY     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbStartXOfs  -> Parent = _pParaUi->pnBase ; _pParaUi->edStartXOfs  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbStartYOfs  -> Parent = _pParaUi->pnBase ; _pParaUi->edStartYOfs  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbThreshold  -> Parent = _pParaUi->pnBase ; _pParaUi->edThreshold  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbFindLtOnDk -> Parent = _pParaUi->pnBase ; _pParaUi->cbFindLtOnDk -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbIgnrInside -> Parent = _pParaUi->pnBase ; _pParaUi->cbIgnrInside -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbIgnrSide   -> Parent = _pParaUi->pnBase ; _pParaUi->cbIgnrSide   -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbMinArea        -> Parent = _pParaUi->pnBase ; _pParaUi->edMinArea        -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxArea        -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbMinWidth       -> Parent = _pParaUi->pnBase ; _pParaUi->edMinWidth       -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxWidth       -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbMinHeight      -> Parent = _pParaUi->pnBase ; _pParaUi->edMinHeight      -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxHeight      -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbMinBoundary    -> Parent = _pParaUi->pnBase ; _pParaUi->edMinBoundary    -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxBoundary    -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbMinCompactness -> Parent = _pParaUi->pnBase ; _pParaUi->edMinCompactness -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxCompactness -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbMinAngle       -> Parent = _pParaUi->pnBase ; _pParaUi->edMinAngle       -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxAngle       -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbMinAspectRatio -> Parent = _pParaUi->pnBase ; _pParaUi->edMinAspectRatio -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxAspectRatio -> Parent = _pParaUi->pnBase ;


    _pParaUi->lbDotSize        -> Caption = "도트지름(mm) ";//"DotSize    ";
    _pParaUi->lbDotPitch       -> Caption = "도트피치(mm) ";//"DotPitch   ";

    _pParaUi->lbPitchX         -> Caption = "X축 검색간격 "          ;//"PitchX     ";
    _pParaUi->lbPitchY         -> Caption = "Y축 검색간격 "          ;//"PitchY     ";
    _pParaUi->lbStartXOfs      -> Caption = "X축 검색시작옵셑 "      ;//"StartXOfs  ";
    _pParaUi->lbStartYOfs      -> Caption = "Y축 검색시작옵셑 "      ;//"StartYOfs  ";
    _pParaUi->lbThreshold      -> Caption = "검색 밝기값 "           ;//"Threshold  ";
    _pParaUi->lbFindLtOnDk     -> Caption = "밝은물체찾기 "          ;//"FindLtOnDk ";
    _pParaUi->lbIgnrInside     -> Caption = "블랍내부무시 "          ;//"IgnrInside ";
    _pParaUi->lbIgnrSide       -> Caption = "ROI벽면에 걸친물체무시 ";//"IgnrSide   ";

    _pParaUi->lbMinArea        -> Caption = "영역개수 "          ;//"Area       ";
    _pParaUi->lbMinWidth       -> Caption = "넓이 "              ;//"Width      ";
    _pParaUi->lbMinHeight      -> Caption = "높이 "              ;//"Height     ";
    _pParaUi->lbMinBoundary    -> Caption = "테두리길이 "        ;//"Boundary   ";
    _pParaUi->lbMinCompactness -> Caption = "Compactness "       ;//"Compactness";
    _pParaUi->lbMinAngle       -> Caption = "Angle       "       ;//"Angle      ";
    _pParaUi->lbMinAspectRatio -> Caption = "AspectRatio "       ;//"AspectRatio";


    _pParaUi->lbResX = new TLabel(_pParaUi->pnBase); _pParaUi->edResX = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbResY = new TLabel(_pParaUi->pnBase); _pParaUi->edResY = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbResX-> Parent = _pParaUi->pnBase ; _pParaUi->edResX-> Parent = _pParaUi->pnBase ;
    _pParaUi->lbResY-> Parent = _pParaUi->pnBase ; _pParaUi->edResY-> Parent = _pParaUi->pnBase ;
    iR++; 
    SetArrange(_pParaUi->lbResX , 0 , iR); SetArrange(_pParaUi->edResX , 1 , iR++ ); _pParaUi-> edResX -> Color = clSilver ;
    SetArrange(_pParaUi->lbResY , 0 , iR); SetArrange(_pParaUi->edResY , 1 , iR++ ); _pParaUi-> edResY -> Color = clSilver ;
    _pParaUi->lbResX -> Caption = "X픽셀 레졸루션 ";//"Px Res X";
    _pParaUi->lbResY -> Caption = "Y픽셀 레졸루션 ";//"Px Res Y";


}

void CCal::SetArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    const int iControlWidth   = 120 ;
    const int iControlHeight  = 20  ;
    const int iStartRowOffset = 5   ;
    const int iStartColOffset = 5   ;

    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;
}

void CCal::SetPaintCallback(TOnChanged ChangeFunc)
{
    Tracker -> SetFunc(ChangeFunc) ;
}

void CCal::SetTrackerVisible(bool _bVisible)
{
    Tracker -> Visible = _bVisible ;
}



void CCal::SetTrackerEnabled(bool _bEnabled)
{
    Tracker -> Enabled = _bEnabled ;
}

bool CCal::MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker -> Focused = false ;

    if(Tracker -> TrackerDw(Button, Shift , X,Y, _fScaleX , _fScaleY)) { m_fInspOfsX = 0.0 ; m_fInspOfsY = 0.0 ; return true ;}

    return false ;
}

bool CCal::MouseTrackerMv(TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    if(Tracker -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY)) return true ;

    return false ;
}

void CCal::MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker -> TrackerUp(Button, Shift );
}

void CCal::PaintTracker(HDC _hHdc , float _fScaleX , float _fScaleY)
{
    if(Tracker -> Visible)PaintOfsRect(_hHdc , _fScaleX , _fScaleY , GetOfsRect(Tracker -> GetTRect()));
    Tracker -> Paint(_hHdc , _fScaleX , _fScaleY);
}

void CCal::ShowParaForm(TWinControl* _wcBase)
{
    if(ParaUi.pnBase == NULL)MakeUI(&ParaUi,_wcBase) ;
    ParaUi.pnBase -> Parent = _wcBase ;
    ParaUi.pnBase -> Align = alClient ;
    ParaUi.pnBase -> Left = 0 ;
    ParaUi.pnBase -> Top  = 0 ;

    ParaUi.pnBase -> Width  = 231 ;
    ParaUi.pnBase -> Height = 714 ;



}

void CCal::LoadPara(bool _bLoad , AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId+"_Calibration" ;
    AnsiString sFilePath = _sFilePath + ".ini" ;
    AnsiString sTrakPath = _sFilePath + "Tracker.ini" ;

    Tracker -> OnLoadSave(_bLoad , sTrakPath , sTemp + "_Tracker");

    if(_bLoad) {

        UserINI.Load(sFilePath, sTemp , "fDotSize   ", Para.fDotSize   );
        UserINI.Load(sFilePath, sTemp , "fDotPitch  ", Para.fDotPitch  );

        UserINI.Load(sFilePath, sTemp , "BlbPara.iPitchX    ", Para.BlbPara.iPitchX    );
        UserINI.Load(sFilePath, sTemp , "BlbPara.iPitchY    ", Para.BlbPara.iPitchY    );
        UserINI.Load(sFilePath, sTemp , "BlbPara.iStartXOfs ", Para.BlbPara.iStartXOfs );
        UserINI.Load(sFilePath, sTemp , "BlbPara.iStartYOfs ", Para.BlbPara.iStartYOfs );
        UserINI.Load(sFilePath, sTemp , "BlbPara.iThreshold ", Para.BlbPara.iThreshold );
        UserINI.Load(sFilePath, sTemp , "BlbPara.bFindLtOnDk", Para.BlbPara.bFindLtOnDk);
        UserINI.Load(sFilePath, sTemp , "BlbPara.bIgnrInside", Para.BlbPara.bIgnrInside);
        UserINI.Load(sFilePath, sTemp , "BlbPara.bIgnrSide  ", Para.BlbPara.bIgnrSide  );

        UserINI.Load(sFilePath, sTemp , "BlbFltr.iMinArea       ", Para.BlbFltr.iMinArea       );UserINI.Load(sFilePath, sTemp , "BlbFltr.iMaxArea       ", Para.BlbFltr.iMaxArea       );
        UserINI.Load(sFilePath, sTemp , "BlbFltr.iMinWidth      ", Para.BlbFltr.iMinWidth      );UserINI.Load(sFilePath, sTemp , "BlbFltr.iMaxWidth      ", Para.BlbFltr.iMaxWidth      );
        UserINI.Load(sFilePath, sTemp , "BlbFltr.iMinHeight     ", Para.BlbFltr.iMinHeight     );UserINI.Load(sFilePath, sTemp , "BlbFltr.iMaxHeight     ", Para.BlbFltr.iMaxHeight     );
        UserINI.Load(sFilePath, sTemp , "BlbFltr.iMinBoundary   ", Para.BlbFltr.iMinBoundary   );UserINI.Load(sFilePath, sTemp , "BlbFltr.iMaxBoundary   ", Para.BlbFltr.iMaxBoundary   );
        UserINI.Load(sFilePath, sTemp , "BlbFltr.fMinCompactness", Para.BlbFltr.fMinCompactness);UserINI.Load(sFilePath, sTemp , "BlbFltr.fMaxCompactness", Para.BlbFltr.fMaxCompactness);
        UserINI.Load(sFilePath, sTemp , "BlbFltr.fMinAngle      ", Para.BlbFltr.fMinAngle      );UserINI.Load(sFilePath, sTemp , "BlbFltr.fMaxAngle      ", Para.BlbFltr.fMaxAngle      );
        UserINI.Load(sFilePath, sTemp , "BlbFltr.fMinAspectRatio", Para.BlbFltr.fMinAspectRatio);UserINI.Load(sFilePath, sTemp , "BlbFltr.fMaxAspectRatio", Para.BlbFltr.fMaxAspectRatio);

        UserINI.Load(sFilePath, sTemp , "Rslt.fXPxRes ", Rslt.fXPxRes );
        UserINI.Load(sFilePath, sTemp , "Rslt.fYPxRes ", Rslt.fYPxRes );
    }
    else {
        UserINI.Save(sFilePath, sTemp , "fDotSize           ", Para.fDotSize           );
        UserINI.Save(sFilePath, sTemp , "fDotPitch          ", Para.fDotPitch          );

        UserINI.Save(sFilePath, sTemp , "BlbPara.iPitchX    ", Para.BlbPara.iPitchX    );
        UserINI.Save(sFilePath, sTemp , "BlbPara.iPitchY    ", Para.BlbPara.iPitchY    );
        UserINI.Save(sFilePath, sTemp , "BlbPara.iStartXOfs ", Para.BlbPara.iStartXOfs );
        UserINI.Save(sFilePath, sTemp , "BlbPara.iStartYOfs ", Para.BlbPara.iStartYOfs );
        UserINI.Save(sFilePath, sTemp , "BlbPara.iThreshold ", Para.BlbPara.iThreshold );
        UserINI.Save(sFilePath, sTemp , "BlbPara.bFindLtOnDk", Para.BlbPara.bFindLtOnDk);
        UserINI.Save(sFilePath, sTemp , "BlbPara.bIgnrInside", Para.BlbPara.bIgnrInside);
        UserINI.Save(sFilePath, sTemp , "BlbPara.bIgnrSide  ", Para.BlbPara.bIgnrSide  );

        UserINI.Save(sFilePath, sTemp , "BlbFltr.iMinArea       ", Para.BlbFltr.iMinArea       );UserINI.Save(sFilePath, sTemp , "BlbFltr.iMaxArea       ", Para.BlbFltr.iMaxArea       );
        UserINI.Save(sFilePath, sTemp , "BlbFltr.iMinWidth      ", Para.BlbFltr.iMinWidth      );UserINI.Save(sFilePath, sTemp , "BlbFltr.iMaxWidth      ", Para.BlbFltr.iMaxWidth      );
        UserINI.Save(sFilePath, sTemp , "BlbFltr.iMinHeight     ", Para.BlbFltr.iMinHeight     );UserINI.Save(sFilePath, sTemp , "BlbFltr.iMaxHeight     ", Para.BlbFltr.iMaxHeight     );
        UserINI.Save(sFilePath, sTemp , "BlbFltr.iMinBoundary   ", Para.BlbFltr.iMinBoundary   );UserINI.Save(sFilePath, sTemp , "BlbFltr.iMaxBoundary   ", Para.BlbFltr.iMaxBoundary   );
        UserINI.Save(sFilePath, sTemp , "BlbFltr.fMinCompactness", Para.BlbFltr.fMinCompactness);UserINI.Save(sFilePath, sTemp , "BlbFltr.fMaxCompactness", Para.BlbFltr.fMaxCompactness);
        UserINI.Save(sFilePath, sTemp , "BlbFltr.fMinAngle      ", Para.BlbFltr.fMinAngle      );UserINI.Save(sFilePath, sTemp , "BlbFltr.fMaxAngle      ", Para.BlbFltr.fMaxAngle      );
        UserINI.Save(sFilePath, sTemp , "BlbFltr.fMinAspectRatio", Para.BlbFltr.fMinAspectRatio);UserINI.Save(sFilePath, sTemp , "BlbFltr.fMaxAspectRatio", Para.BlbFltr.fMaxAspectRatio);

        UserINI.Save(sFilePath, sTemp , "Rslt.fXPxRes ", Rslt.fXPxRes );
        UserINI.Save(sFilePath, sTemp , "Rslt.fYPxRes ", Rslt.fYPxRes );
    }


}

void CCal::ApplyPara(bool _bTable)
{
    if(_bTable) {
        ParaUi.edDotSize    -> Text    = Para.fDotSize   ;
        ParaUi.edDotPitch   -> Text    = Para.fDotPitch  ;

        ParaUi.edPitchX     -> Text    = Para.BlbPara.iPitchX    ;
        ParaUi.edPitchY     -> Text    = Para.BlbPara.iPitchY    ;
        ParaUi.edStartXOfs  -> Text    = Para.BlbPara.iStartXOfs ;
        ParaUi.edStartYOfs  -> Text    = Para.BlbPara.iStartYOfs ;
        ParaUi.edThreshold  -> Text    = Para.BlbPara.iThreshold ;
        ParaUi.cbFindLtOnDk -> Checked = Para.BlbPara.bFindLtOnDk;
        ParaUi.cbIgnrInside -> Checked = Para.BlbPara.bIgnrInside;
        ParaUi.cbIgnrSide   -> Checked = Para.BlbPara.bIgnrSide  ;

        ParaUi.edMinArea        -> Text = Para.BlbFltr.iMinArea        ; ParaUi.edMaxArea          -> Text = Para.BlbFltr.iMaxArea        ;
        ParaUi.edMinWidth       -> Text = Para.BlbFltr.iMinWidth       ; ParaUi.edMaxWidth         -> Text = Para.BlbFltr.iMaxWidth       ;
        ParaUi.edMinHeight      -> Text = Para.BlbFltr.iMinHeight      ; ParaUi.edMaxHeight        -> Text = Para.BlbFltr.iMaxHeight      ;
        ParaUi.edMinBoundary    -> Text = Para.BlbFltr.iMinBoundary    ; ParaUi.edMaxBoundary      -> Text = Para.BlbFltr.iMaxBoundary    ;
        ParaUi.edMinCompactness -> Text = Para.BlbFltr.fMinCompactness ; ParaUi.edMaxCompactness   -> Text = Para.BlbFltr.fMaxCompactness ;
        ParaUi.edMinAngle       -> Text = Para.BlbFltr.fMinAngle       ; ParaUi.edMaxAngle         -> Text = Para.BlbFltr.fMaxAngle       ;
        ParaUi.edMinAspectRatio -> Text = Para.BlbFltr.fMinAspectRatio ; ParaUi.edMaxAspectRatio   -> Text = Para.BlbFltr.fMaxAspectRatio ;

        ParaUi.edResX -> Text = Rslt.fXPxRes ;
        ParaUi.edResY -> Text = Rslt.fYPxRes ;  //켈리브레이션 저장해서 적용 하게 하고 인스펙션 할때 물어 보고 하기.
    }
    else {
        int iTemp ;
        bool bTemp ;
        float fTemp ;

        fTemp = StrToFloatDef(ParaUi.edDotSize        -> Text , 1.0 ); if (Para.fDotSize                != fTemp) { Trace("Operator" , "Both Cal_DotSize values are different."); }
        fTemp = StrToFloatDef(ParaUi.edDotPitch       -> Text , 0.25); if (Para.fDotPitch               != fTemp) { Trace("Operator" , "Both Cal_DotPitch values are different."); }

        iTemp = StrToIntDef  (ParaUi.edPitchX         -> Text , 1   ); if (Para.BlbPara.iPitchX         != iTemp) { Trace("Operator" , "Both Cal_PitchX values are different."); }
        iTemp = StrToIntDef  (ParaUi.edPitchY         -> Text , 1   ); if (Para.BlbPara.iPitchY         != iTemp) { Trace("Operator" , "Both Cal_PitchY values are different."); }
        iTemp = StrToIntDef  (ParaUi.edStartXOfs      -> Text , 0   ); if (Para.BlbPara.iStartXOfs      != iTemp) { Trace("Operator" , "Both Cal_StartXOfs values are different."); }
        iTemp = StrToIntDef  (ParaUi.edStartYOfs      -> Text , 0   ); if (Para.BlbPara.iStartYOfs      != iTemp) { Trace("Operator" , "Both Cal_StartYOfs values are different."); }
        iTemp = StrToIntDef  (ParaUi.edThreshold      -> Text , 100 ); if (Para.BlbPara.iThreshold      != iTemp) { Trace("Operator" , "Both Cal_Threshold values are different."); }
        fTemp =               ParaUi.cbFindLtOnDk     -> Checked     ; if (Para.BlbPara.bFindLtOnDk     != fTemp) { Trace("Operator" , "Both Cal_FindLtOnDk values are different."); }
        fTemp =               ParaUi.cbIgnrInside     -> Checked     ; if (Para.BlbPara.bIgnrInside     != fTemp) { Trace("Operator" , "Both Cal_IgnrInside values are different."); }
        fTemp =               ParaUi.cbIgnrSide       -> Checked     ; if (Para.BlbPara.bIgnrSide       != fTemp) { Trace("Operator" , "Both Cal_IgnrSide values are different."); }

        iTemp = StrToIntDef  (ParaUi.edMinArea        -> Text , 0   ); if (Para.BlbFltr.iMinArea        != iTemp) { Trace("Operator" , "Both Cal_MinArea values are different."); }
        iTemp = StrToIntDef  (ParaUi.edMinWidth       -> Text , 0   ); if (Para.BlbFltr.iMinWidth       != iTemp) { Trace("Operator" , "Both Cal_MinWidth values are different."); }
        iTemp = StrToIntDef  (ParaUi.edMinHeight      -> Text , 0   ); if (Para.BlbFltr.iMinHeight      != iTemp) { Trace("Operator" , "Both Cal_MinHeight values are different."); }
        iTemp = StrToIntDef  (ParaUi.edMinBoundary    -> Text , 0   ); if (Para.BlbFltr.iMinBoundary    != iTemp) { Trace("Operator" , "Both Cal_MinBoundary values are different."); }
        fTemp = StrToFloatDef(ParaUi.edMinCompactness -> Text , 0   ); if (Para.BlbFltr.fMinCompactness != fTemp) { Trace("Operator" , "Both Cal_MinCompactness values are different."); }
        fTemp = StrToFloatDef(ParaUi.edMinAngle       -> Text , 0   ); if (Para.BlbFltr.fMinAngle       != fTemp) { Trace("Operator" , "Both Cal_MinAngle values are different."); }
        fTemp = StrToFloatDef(ParaUi.edMinAspectRatio -> Text , 0   ); if (Para.BlbFltr.fMinAspectRatio != fTemp) { Trace("Operator" , "Both Cal_MinAspectRatio values are different."); }


        iTemp = StrToIntDef  (ParaUi.edMaxArea        -> Text , 0   ); if (Para.BlbFltr.iMaxArea        != iTemp) { Trace("Operator" , "Both Cal_MaxArea values are different."); }
        iTemp = StrToIntDef  (ParaUi.edMaxWidth       -> Text , 0   ); if (Para.BlbFltr.iMaxWidth       != iTemp) { Trace("Operator" , "Both Cal_MaxWidth values are different."); }
        iTemp = StrToIntDef  (ParaUi.edMaxHeight      -> Text , 0   ); if (Para.BlbFltr.iMaxHeight      != iTemp) { Trace("Operator" , "Both Cal_MaxHeight values are different."); }
        iTemp = StrToIntDef  (ParaUi.edMaxBoundary    -> Text , 0   ); if (Para.BlbFltr.iMaxBoundary    != iTemp) { Trace("Operator" , "Both Cal_MaxBoundary values are different."); }
        fTemp = StrToFloatDef(ParaUi.edMaxCompactness -> Text , 0   ); if (Para.BlbFltr.fMaxCompactness != fTemp) { Trace("Operator" , "Both Cal_MaxCompactnessvalues are different."); }
        fTemp = StrToFloatDef(ParaUi.edMaxAngle       -> Text , 0   ); if (Para.BlbFltr.fMaxAngle       != fTemp) { Trace("Operator" , "Both Cal_MaxAngle values are different."); }
        fTemp = StrToFloatDef(ParaUi.edMaxAspectRatio -> Text , 0   ); if (Para.BlbFltr.fMaxAspectRatio != fTemp) { Trace("Operator" , "Both Cal_MaxAspectRatio values are different."); }

        fTemp = StrToFloatDef(ParaUi.edResX           -> Text , 0   ); if (Rslt.fXPxRes                 != fTemp) { Trace("Operator" , "Both Cal_ResX values are different."); }
        fTemp = StrToFloatDef(ParaUi.edResY           -> Text , 0   ); if (Rslt.fYPxRes                 != fTemp) { Trace("Operator" , "Both Cal_ResY values are different."); }

        Para.fDotSize            = StrToFloatDef(ParaUi.edDotSize  -> Text , 1.0 ) ;
        Para.fDotPitch           = StrToFloatDef(ParaUi.edDotPitch -> Text , 0.25) ;

        Para.BlbPara.iPitchX     = StrToIntDef (ParaUi.edPitchX     -> Text , 1  ) ;
        Para.BlbPara.iPitchY     = StrToIntDef (ParaUi.edPitchY     -> Text , 1  ) ;
        Para.BlbPara.iStartXOfs  = StrToIntDef (ParaUi.edStartXOfs  -> Text , 0  ) ;
        Para.BlbPara.iStartYOfs  = StrToIntDef (ParaUi.edStartYOfs  -> Text , 0  ) ;
        Para.BlbPara.iThreshold  = StrToIntDef (ParaUi.edThreshold  -> Text , 100) ;
        Para.BlbPara.bFindLtOnDk =              ParaUi.cbFindLtOnDk -> Checked     ;
        Para.BlbPara.bIgnrInside =              ParaUi.cbIgnrInside -> Checked     ;
        Para.BlbPara.bIgnrSide   =              ParaUi.cbIgnrSide   -> Checked     ;

        Para.BlbFltr.iMinArea        = StrToIntDef  (ParaUi.edMinArea        -> Text , 0 ) ;
        Para.BlbFltr.iMinWidth       = StrToIntDef  (ParaUi.edMinWidth       -> Text , 0 ) ;
        Para.BlbFltr.iMinHeight      = StrToIntDef  (ParaUi.edMinHeight      -> Text , 0 ) ;
        Para.BlbFltr.iMinBoundary    = StrToIntDef  (ParaUi.edMinBoundary    -> Text , 0 ) ;
        Para.BlbFltr.fMinCompactness = StrToFloatDef(ParaUi.edMinCompactness -> Text , 0 ) ;
        Para.BlbFltr.fMinAngle       = StrToFloatDef(ParaUi.edMinAngle       -> Text , 0 ) ;
        Para.BlbFltr.fMinAspectRatio = StrToFloatDef(ParaUi.edMinAspectRatio -> Text , 0 ) ;

        Para.BlbFltr.iMaxArea        = StrToIntDef  (ParaUi.edMaxArea        -> Text , 0 ) ;
        Para.BlbFltr.iMaxWidth       = StrToIntDef  (ParaUi.edMaxWidth       -> Text , 0 ) ;
        Para.BlbFltr.iMaxHeight      = StrToIntDef  (ParaUi.edMaxHeight      -> Text , 0 ) ;
        Para.BlbFltr.iMaxBoundary    = StrToIntDef  (ParaUi.edMaxBoundary    -> Text , 0 ) ;
        Para.BlbFltr.fMaxCompactness = StrToFloatDef(ParaUi.edMaxCompactness -> Text , 0 ) ;
        Para.BlbFltr.fMaxAngle       = StrToFloatDef(ParaUi.edMaxAngle       -> Text , 0 ) ;
        Para.BlbFltr.fMaxAspectRatio = StrToFloatDef(ParaUi.edMaxAspectRatio -> Text , 0 ) ;

        Rslt.fXPxRes = StrToFloatDef(ParaUi.edResX -> Text , 0 );
        Rslt.fYPxRes = StrToFloatDef(ParaUi.edResY -> Text , 0 );  //켈리브레이션 저장해서 적용 하게 하고 인스펙션 할때 물어 보고 하기.

        ApplyPara(true);
    }
}

bool CCal::Process(IImage * _pImg , CAL_Rslt * _pRslt)
{
    bool bRet ;

    bRet = CAL_Inspect(_pImg , Tracker -> GetTRect() , Para , _pRslt);

    //memcpy(&Rslt , _pRslt , sizeof(CAL_Rslt));

    if(Rslt.BlbRslt.pBlobs) {
        for(int i = 0 ; i < Rslt.BlbRslt.iBlobCnt ; i++) {
            Rslt.BlbRslt.pBlobs[i].OnClear();
        }
        delete [] Rslt.BlbRslt.pBlobs ;
        Rslt.BlbRslt.pBlobs   = NULL  ;
        Rslt.BlbRslt.iBlobCnt = 0 ;
    }
    Rslt.BlbRslt.iBlobCnt  = _pRslt->BlbRslt.iBlobCnt  ;
    Rslt.BlbRslt.fInspTime = _pRslt->BlbRslt.fInspTime ;
    Rslt.BlbRslt.pBlobs    = new SBlob[Rslt.BlbRslt.iBlobCnt];
    for(int i = 0 ; i <Rslt.BlbRslt.iBlobCnt ; i++) {
        Rslt.BlbRslt.pBlobs[i] = _pRslt->BlbRslt.pBlobs[i] ;
    }

    Rslt.fXPxRes   = _pRslt->fXPxRes   ;
    Rslt.fYPxRes   = _pRslt->fYPxRes   ;
    Rslt.fInspTime = _pRslt->fInspTime ;

    ParaUi.edResX -> Text = Rslt.fXPxRes ;
    ParaUi.edResY -> Text = Rslt.fYPxRes ;


    return bRet ;
}

void CCal::PaintRslt(IImage * _pImg , HDC _hHdc)
{
    CAL_PaintRslt(_pImg , _hHdc , &Rslt , Disp);
}





/******************************************************************************/
/* Circle                                                                     */
/******************************************************************************/
CCcl::CCcl()
{
    m_iProcType = ptCcl ;

    Tracker = new TCircleTracker() ;
    Tracker -> Caption = "CCL";
    Tracker -> Band    = 30 ;

    Disp.clEdge   = clLime ;
    Disp.clCircle = clLime ;
    Disp.clText   = clLime ;

    //MakeUI(&ParaUi);
    ParaUi.pnBase = NULL ;
}

CCcl::~CCcl()
{
    delete Tracker ;
}

void CCcl::MakeUI(TParaUi  * _pParaUi , TWinControl * _tcControl)
{
    int iR = 0 ;

    _pParaUi->pnBase = new TPanel(_tcControl); _pParaUi->pnBase -> Parent = _tcControl ;



    _pParaUi->lbGap          = new TLabel(_pParaUi->pnBase); _pParaUi->edGap         = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbSampleCnt    = new TLabel(_pParaUi->pnBase); _pParaUi->edSampleCnt   = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbStartAng     = new TLabel(_pParaUi->pnBase); _pParaUi->edStartAng    = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEndAng       = new TLabel(_pParaUi->pnBase); _pParaUi->edEndAng      = new TEdit (_pParaUi->pnBase);

    _pParaUi->lbEdgAverage   = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgAverage  = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgCompare   = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgCompare  = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgChkPxLow  = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgChkPxLow = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgChkPxHgh  = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgChkPxHgh = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgOfsGap    = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgOfsGap   = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgOfsPxLow  = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgOfsPxLow = new TEdit (_pParaUi->pnBase);
    _pParaUi->lbEdgOfsPxHgh  = new TLabel(_pParaUi->pnBase); _pParaUi->edEdgOfsPxHgh = new TEdit (_pParaUi->pnBase);

    SetArrange(_pParaUi->lbGap         , 0 , iR ); SetArrange(_pParaUi->edGap          , 1 , iR++ );
    SetArrange(_pParaUi->lbSampleCnt   , 0 , iR ); SetArrange(_pParaUi->edSampleCnt    , 1 , iR++ );
    SetArrange(_pParaUi->lbStartAng    , 0 , iR ); SetArrange(_pParaUi->edStartAng     , 1 , iR++ );
    SetArrange(_pParaUi->lbEndAng      , 0 , iR ); SetArrange(_pParaUi->edEndAng       , 1 , iR++ );

    iR++;
    SetArrange(_pParaUi->lbEdgAverage  , 0 , iR ); SetArrange(_pParaUi->edEdgAverage   , 1 , iR++ );
    SetArrange(_pParaUi->lbEdgCompare  , 0 , iR ); SetArrange(_pParaUi->edEdgCompare   , 1 , iR++ );
    SetArrange(_pParaUi->lbEdgChkPxLow , 0 , iR ); SetArrange(_pParaUi->edEdgChkPxLow  , 1 , iR++ );
    SetArrange(_pParaUi->lbEdgChkPxHgh , 0 , iR ); SetArrange(_pParaUi->edEdgChkPxHgh  , 1 , iR++ );
    SetArrange(_pParaUi->lbEdgOfsGap   , 0 , iR ); SetArrange(_pParaUi->edEdgOfsGap    , 1 , iR++ );
    SetArrange(_pParaUi->lbEdgOfsPxLow , 0 , iR ); SetArrange(_pParaUi->edEdgOfsPxLow  , 1 , iR++ );
    SetArrange(_pParaUi->lbEdgOfsPxHgh , 0 , iR ); SetArrange(_pParaUi->edEdgOfsPxHgh  , 1 , iR++ );


    _pParaUi->lbGap         -> Parent = _pParaUi->pnBase ; _pParaUi->edGap         -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbSampleCnt   -> Parent = _pParaUi->pnBase ; _pParaUi->edSampleCnt   -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbStartAng    -> Parent = _pParaUi->pnBase ; _pParaUi->edStartAng    -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEndAng      -> Parent = _pParaUi->pnBase ; _pParaUi->edEndAng      -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbEdgAverage  -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgAverage  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgCompare  -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgCompare  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgChkPxLow -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgChkPxLow -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgChkPxHgh -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgChkPxHgh -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgOfsGap   -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgOfsGap   -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgOfsPxLow -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgOfsPxLow -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbEdgOfsPxHgh -> Parent = _pParaUi->pnBase ; _pParaUi->edEdgOfsPxHgh -> Parent = _pParaUi->pnBase ;


    _pParaUi->lbGap         -> Caption = "검색 간격     "  ;//"Gap      " ;
    _pParaUi->lbSampleCnt   -> Caption = "검색 개수     "  ;//"SampleCnt" ;
    _pParaUi->lbStartAng    -> Caption = "검색시작 각도 "  ;//"StartAng " ;
    _pParaUi->lbEndAng      -> Caption = "검색끝 각도   "  ;//"EndAng   " ;

    _pParaUi->lbEdgAverage  -> Caption = "에지 평균범위         ";//"Average " ;
    _pParaUi->lbEdgCompare  -> Caption = "에지 비교범위         ";//"Compare " ;
    _pParaUi->lbEdgChkPxLow -> Caption = "에지 확정픽셀Low      ";//"ChkPxLow" ;
    _pParaUi->lbEdgChkPxHgh -> Caption = "에지 확정픽셀High     ";//"ChkPxHgh" ;
    _pParaUi->lbEdgOfsGap   -> Caption = "에지 확정옵셑거리     ";//"OfsGap  " ;
    _pParaUi->lbEdgOfsPxLow -> Caption = "에지 확정옵셑거리Low  ";//"OfsPxLow" ;
    _pParaUi->lbEdgOfsPxHgh -> Caption = "에지 확정옵셑거리High ";//"OfsPxHgh" ;
}

void CCcl::SetArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    const int iControlWidth   = 120 ;
    const int iControlHeight  = 20  ;
    const int iStartRowOffset = 5   ;
    const int iStartColOffset = 5   ;

    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;
}

void CCcl::SetPaintCallback(TOnChanged ChangeFunc)
{
    Tracker -> SetFunc(ChangeFunc) ;
}

void CCcl::SetTrackerVisible(bool _bVisible)
{
    Tracker -> Visible = _bVisible ;
}

void CCcl::SetTrackerEnabled(bool _bEnabled)
{
    Tracker -> Enabled = _bEnabled ;
}

bool CCcl::MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker -> Focused = false ;

    if(Tracker -> TrackerDw(Button, Shift , X,Y, _fScaleX , _fScaleY)) { m_fInspOfsX = 0.0 ; m_fInspOfsY = 0.0 ; return true ;}
    return false ;
}

bool CCcl::MouseTrackerMv(TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    if(Tracker -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY)) return true ;

    return false ;
}

void CCcl::MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker -> TrackerUp(Button, Shift );
}

void CCcl::PaintTracker(HDC _hHdc , float _fScaleX , float _fScaleY)
{
    if(Tracker -> Visible)PaintOfsCircle(_hHdc , _fScaleX , _fScaleY , GetOfsCircle(Tracker -> GetTCircle()));
    Tracker -> Paint(_hHdc , _fScaleX , _fScaleY);
}

void CCcl::ShowParaForm(TWinControl* _wcBase)
{
    if(ParaUi.pnBase == NULL)MakeUI(&ParaUi,_wcBase) ;
    ParaUi.pnBase -> Parent = _wcBase ;
    ParaUi.pnBase -> Align = alClient ;
}

void CCcl::LoadPara(bool _bLoad , AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId+"_Circle" ;
    AnsiString sFilePath = _sFilePath + ".ini" ;
    AnsiString sTrakPath = _sFilePath + "Tracker.ini" ;

    Tracker -> OnLoadSave(_bLoad , sTrakPath , sTemp + "_Tracker");

    if(_bLoad) {
        UserINI.Load(sFilePath, sTemp , "iGap              ", Para.iGap      );
        UserINI.Load(sFilePath, sTemp , "iSampleCnt        ", Para.iSampleCnt);
        UserINI.Load(sFilePath, sTemp , "iStartAng         ", Para.iStartAng );
        UserINI.Load(sFilePath, sTemp , "iEndAng           ", Para.iEndAng   );

        UserINI.Load(sFilePath, sTemp , "EdgPara.iAverage  ", Para.EdgPara.iAverage );
        UserINI.Load(sFilePath, sTemp , "EdgPara.iCompare  ", Para.EdgPara.iCompare );
        UserINI.Load(sFilePath, sTemp , "EdgPara.iChkPxLow ", Para.EdgPara.iChkPxLow);
        UserINI.Load(sFilePath, sTemp , "EdgPara.iChkPxHgh ", Para.EdgPara.iChkPxHgh);
        UserINI.Load(sFilePath, sTemp , "EdgPara.iOfsGap   ", Para.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "EdgPara.iOfsPxLow ", Para.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "EdgPara.iOfsPxHgh ", Para.EdgPara.iOfsPxHgh);

        //여기에 레퍼런스 저장 하는 것..... 세이브 한번 해야지만 결과값을 뽑을수 있는 파라가 저장 되고 1번 검사를 해야 레퍼런스를 세팅 할수 있다.
        UserINI.Load(sFilePath, sTemp , "m_fRefValX", m_fRefValX);
        UserINI.Load(sFilePath, sTemp , "m_fRefValY", m_fRefValY);
        UserINI.Load(sFilePath, sTemp , "m_fRefValT", m_fRefValT);
    }
    else {
        UserINI.Save(sFilePath, sTemp , "iGap      ", Para.iGap       );
        UserINI.Save(sFilePath, sTemp , "iSampleCnt", Para.iSampleCnt );
        UserINI.Save(sFilePath, sTemp , "iStartAng ", Para.iStartAng  );
        UserINI.Save(sFilePath, sTemp , "iEndAng   ", Para.iEndAng    );

        UserINI.Save(sFilePath, sTemp , "EdgPara.iAverage ", Para.EdgPara.iAverage );
        UserINI.Save(sFilePath, sTemp , "EdgPara.iCompare ", Para.EdgPara.iCompare );
        UserINI.Save(sFilePath, sTemp , "EdgPara.iChkPxLow", Para.EdgPara.iChkPxLow);
        UserINI.Save(sFilePath, sTemp , "EdgPara.iChkPxHgh", Para.EdgPara.iChkPxHgh);
        UserINI.Save(sFilePath, sTemp , "EdgPara.iOfsGap  ", Para.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "EdgPara.iOfsPxLow", Para.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "EdgPara.iOfsPxHgh", Para.EdgPara.iOfsPxHgh);

        m_fRefValX = Rslt.fCntrX ;
        m_fRefValY = Rslt.fCntrY ;
        m_fRefValT = 0.0 ;
        UserINI.Save(sFilePath, sTemp , "m_fRefValX", m_fRefValX);
        UserINI.Save(sFilePath, sTemp , "m_fRefValY", m_fRefValY);
        UserINI.Save(sFilePath, sTemp , "m_fRefValT", m_fRefValT);
    }
}

void CCcl::ApplyPara(bool _bTable)
{
    if(_bTable) {

        ParaUi.edGap          -> Text = Para.iGap              ;
        ParaUi.edSampleCnt    -> Text = Para.iSampleCnt        ;
        ParaUi.edStartAng     -> Text = Para.iStartAng         ;
        ParaUi.edEndAng       -> Text = Para.iEndAng           ;

        ParaUi.edEdgAverage   -> Text = Para.EdgPara.iAverage  ;
        ParaUi.edEdgCompare   -> Text = Para.EdgPara.iCompare  ;
        ParaUi.edEdgChkPxLow  -> Text = Para.EdgPara.iChkPxLow ;
        ParaUi.edEdgChkPxHgh  -> Text = Para.EdgPara.iChkPxHgh ;
        ParaUi.edEdgOfsGap    -> Text = Para.EdgPara.iOfsGap   ;
        ParaUi.edEdgOfsPxLow  -> Text = Para.EdgPara.iOfsPxLow ;
        ParaUi.edEdgOfsPxHgh  -> Text = Para.EdgPara.iOfsPxHgh ;
    }
    else {
        int iTemp ;

        iTemp = StrToIntDef(ParaUi.edGap          -> Text , 1); if (Para.iGap              != iTemp) { Trace("Operator" , "Both Ccl_Gap         values are different."); }
        iTemp = StrToIntDef(ParaUi.edSampleCnt    -> Text , 0); if (Para.iSampleCnt        != iTemp) { Trace("Operator" , "Both Ccl_SampleCnt   values are different."); }
        iTemp = StrToIntDef(ParaUi.edStartAng     -> Text , 1); if (Para.iStartAng         != iTemp) { Trace("Operator" , "Both Ccl_StartAng    values are different."); }
        iTemp = StrToIntDef(ParaUi.edEndAng       -> Text , 0); if (Para.iEndAng           != iTemp) { Trace("Operator" , "Both Ccl_EndAng      values are different."); }

        iTemp = StrToIntDef(ParaUi.edEdgAverage   -> Text , 1); if (Para.EdgPara.iAverage  != iTemp) { Trace("Operator" , "Both Ccl_EdgAverage  values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgCompare   -> Text , 1); if (Para.EdgPara.iCompare  != iTemp) { Trace("Operator" , "Both Ccl_EdgCompare  values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgChkPxLow  -> Text , 0); if (Para.EdgPara.iChkPxLow != iTemp) { Trace("Operator" , "Both Ccl_EdgChkPxLow values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgChkPxHgh  -> Text , 0); if (Para.EdgPara.iChkPxHgh != iTemp) { Trace("Operator" , "Both Ccl_EdgChkPxHgh values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgOfsGap    -> Text , 0); if (Para.EdgPara.iOfsGap   != iTemp) { Trace("Operator" , "Both Ccl_EdgOfsGap   values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgOfsPxLow  -> Text , 0); if (Para.EdgPara.iOfsPxLow != iTemp) { Trace("Operator" , "Both Ccl_EdgOfsPxLow values are different."); }
        iTemp = StrToIntDef(ParaUi.edEdgOfsPxHgh  -> Text , 0); if (Para.EdgPara.iOfsPxHgh != iTemp) { Trace("Operator" , "Both Ccl_EdgOfsPxHgh values are different."); }


        Para.iGap              = StrToIntDef(ParaUi.edGap          -> Text , 1);
        Para.iSampleCnt        = StrToIntDef(ParaUi.edSampleCnt    -> Text , 0);
        Para.iStartAng         = StrToIntDef(ParaUi.edStartAng     -> Text , 1);
        Para.iEndAng           = StrToIntDef(ParaUi.edEndAng       -> Text , 0);

        Para.EdgPara.iAverage  = StrToIntDef(ParaUi.edEdgAverage   -> Text , 1);
        Para.EdgPara.iCompare  = StrToIntDef(ParaUi.edEdgCompare   -> Text , 1);
        Para.EdgPara.iChkPxLow = StrToIntDef(ParaUi.edEdgChkPxLow  -> Text , 0);
        Para.EdgPara.iChkPxHgh = StrToIntDef(ParaUi.edEdgChkPxHgh  -> Text , 0);
        Para.EdgPara.iOfsGap   = StrToIntDef(ParaUi.edEdgOfsGap    -> Text , 0);
        Para.EdgPara.iOfsPxLow = StrToIntDef(ParaUi.edEdgOfsPxLow  -> Text , 0);
        Para.EdgPara.iOfsPxHgh = StrToIntDef(ParaUi.edEdgOfsPxHgh  -> Text , 0);
        ApplyPara(true);
    }
}

bool CCcl::Process(IImage * _pImg ,CCL_Rslt * _pRslt)
{
    bool bRet ;

    Para.iSearchGap = Tracker -> Band ;
    bRet = CCL_Inspect(_pImg , GetOfsCircle(Tracker -> GetTCircle()) , Para , _pRslt);
    memcpy(&Rslt , _pRslt , sizeof(CCL_Rslt));

    return bRet ;
}

void CCcl::PaintRslt(IImage * _pImg , HDC _hHdc)
{
    CCL_PaintRslt(_pImg , _hHdc , &Rslt , Disp);
}

void CCcl::GetRefValOfs(float &_fX , float &_fY , float &_fT )
{
    _fX = Rslt.fCntrX - m_fRefValX ;
    _fY = Rslt.fCntrY - m_fRefValY ;
    _fT = 0                        ;
}


/******************************************************************************/
/* Ocv                                                                        */
/******************************************************************************/
COcv::COcv()
{
    m_iProcType = ptOcv ;

    Tracker = new TOutBandRectTracker() ;

    m_pTrArea   = ARA_New();
    m_pRsltArea = ARA_New();

    m_pTrImage  = IMG_New();

    Tracker -> Caption = "OCV_Train";

    //MakeUI(&ParaUi);
    ParaUi.pnBase = NULL ;

    Disp.clInspPx   = clBlue    ;
    Disp.clNGPx     = clRed     ;
    Disp.clText     = clLime    ;
    Disp.clCenter   = clLime    ;
    Disp.clRect     = clLime    ;
}

COcv::~COcv()
{
    delete Tracker ;

    ARA_Del(m_pTrArea  );
    ARA_Del(m_pRsltArea);

    IMG_Del(m_pTrImage);
}

void COcv::MakeUI(TParaUi  * _pParaUi , TWinControl * _tcControl)
{
    int iR = 0 ;
    _pParaUi->pnBase = new TPanel(_tcControl  ); _pParaUi->pnBase -> Parent = _tcControl ;


    _pParaUi->lbInspOft   = new TLabel(_pParaUi->pnBase); _pParaUi->edInspOft   = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbNoInspOft = new TLabel(_pParaUi->pnBase); _pParaUi->edNoInspOft = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbThreshold = new TLabel(_pParaUi->pnBase); _pParaUi->edThreshold = new TEdit    (_pParaUi->pnBase);

    _pParaUi->lbInspFreq  = new TLabel(_pParaUi->pnBase); _pParaUi->edInspFreq  = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbSinc      = new TLabel(_pParaUi->pnBase); _pParaUi->edSinc      = new TEdit    (_pParaUi->pnBase); _pParaUi->lbSinc -> Visible =false ;_pParaUi->edSinc -> Visible = false ;

    SetArrange(_pParaUi->lbInspOft   , 0 , iR );  SetArrange(_pParaUi->edInspOft   , 1 , iR++ );
    SetArrange(_pParaUi->lbNoInspOft , 0 , iR );  SetArrange(_pParaUi->edNoInspOft , 1 , iR++ );
    SetArrange(_pParaUi->lbThreshold , 0 , iR );  SetArrange(_pParaUi->edThreshold , 1 , iR++ );

    SetArrange(_pParaUi->lbInspFreq  , 0 , iR );  SetArrange(_pParaUi->edInspFreq  , 1 , iR++ );
    SetArrange(_pParaUi->lbSinc      , 0 , iR );  SetArrange(_pParaUi->edSinc      , 1 , iR++ );

    _pParaUi->lbInspOft   -> Parent = _pParaUi->pnBase ; _pParaUi->edInspOft   -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbNoInspOft -> Parent = _pParaUi->pnBase ; _pParaUi->edNoInspOft -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbThreshold -> Parent = _pParaUi->pnBase ; _pParaUi->edThreshold -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbInspFreq  -> Parent = _pParaUi->pnBase ; _pParaUi->edInspFreq  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbSinc      -> Parent = _pParaUi->pnBase ; _pParaUi->edSinc      -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbInspOft   -> Caption = "검사영역 옵셑   ";//"InspOft  " ;
    _pParaUi->lbNoInspOft -> Caption = "비검사영역 옵셑 ";//"NoInspOft" ;
    _pParaUi->lbThreshold -> Caption = "밝기            ";//"Threshold" ;

    _pParaUi->lbInspFreq  -> Caption = "검사 빈도       ";//"InspFreq " ;
    _pParaUi->lbSinc      -> Caption = "검사 매치율     ";//"Sinc     " ;
}

void COcv::SetArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    const int iControlWidth   = 120 ;
    const int iControlHeight  = 20  ;
    const int iStartRowOffset = 5   ;
    const int iStartColOffset = 5   ;

    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;
}

void COcv::SetPaintCallback(TOnChanged ChangeFunc)
{
    Tracker -> SetFunc(ChangeFunc) ;
}

void COcv::SetTrackerVisible(bool _bVisible)
{
    Tracker -> Visible = _bVisible ;
}

void COcv::SetTrackerEnabled(bool _bEnabled)
{
    Tracker -> Enabled = _bEnabled ;
}

bool COcv::MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker -> Focused = false ;

    if(Tracker -> TrackerDw(Button, Shift , X,Y, _fScaleX , _fScaleY)) { m_fInspOfsX = 0.0 ; m_fInspOfsY = 0.0 ; return true ;}

    return false ;
}

bool COcv::MouseTrackerMv(TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    if(Tracker -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY)) return true ;

    return false ;
}

void COcv::MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker -> TrackerUp(Button, Shift );
}

void COcv::PaintTracker(HDC _hHdc , float _fScaleX , float _fScaleY)
{
    if(Tracker -> Visible)PaintOfsRect(_hHdc , _fScaleX , _fScaleY , GetOfsRect(Tracker -> GetTRect()));
    Tracker -> Paint(_hHdc , _fScaleX , _fScaleY);

}

void COcv::ShowParaForm(TWinControl* _wcBase)
{
    if(ParaUi.pnBase == NULL)MakeUI(&ParaUi , _wcBase) ;
    ParaUi.pnBase -> Parent = _wcBase ;
    ParaUi.pnBase -> Align = alClient ;
}

void COcv::LoadPara(bool _bLoad , AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId+"_Ocv" ;

    AnsiString sTrArea = _sFilePath + "_" + _sId+"_OcvTrain.bmp"     ;
    AnsiString sTrImg  = _sFilePath + "_" + _sId+"_OcvTrainImg.bmp"  ;


    AnsiString sFilePath = _sFilePath + ".ini" ;
    AnsiString sTrakPath = _sFilePath + "Tracker.ini" ;

    Tracker -> OnLoadSave(_bLoad , sTrakPath , sTemp + "_Tracker");


    int iTemp ;
    if(_bLoad) {
        m_pTrArea ->LoadBmp(sTrArea.c_str());
        m_pTrImage->LoadBmp(sTrImg .c_str());
        UserINI.Load(sFilePath, sTemp , "iInspOft  ", TrPr.iInspOft  );
        UserINI.Load(sFilePath, sTemp , "iNoInspOft", TrPr.iNoInspOft);
        UserINI.Load(sFilePath, sTemp , "iThreshold", TrPr.iThreshold);
        UserINI.Load(sFilePath, sTemp , "iInspFreq ", TrPr.iInspFreq );

        UserINI.Load(sFilePath, sTemp , "fSinc     ", Para.fSinc     );

        UserINI.Load(sFilePath, sTemp , "m_fRefValX", m_fRefValX);
        UserINI.Load(sFilePath, sTemp , "m_fRefValY", m_fRefValY);
        UserINI.Load(sFilePath, sTemp , "m_fRefValT", m_fRefValT);
    }
    else {
        m_pTrArea ->SaveBmp(sTrArea.c_str());
        m_pTrImage->SaveBmp(sTrImg .c_str());
        UserINI.Save(sFilePath, sTemp , "iInspOft  ", TrPr.iInspOft  );
        UserINI.Save(sFilePath, sTemp , "iNoInspOft", TrPr.iNoInspOft);
        UserINI.Save(sFilePath, sTemp , "iThreshold", TrPr.iThreshold);
        UserINI.Save(sFilePath, sTemp , "iInspFreq ", TrPr.iInspFreq );

        UserINI.Save(sFilePath, sTemp , "fSinc     ", Para.fSinc     );


        //여기에 레퍼런스 저장 하는 것..... 세이브 한번 해야지만 결과값을 뽑을수 있는 파라가 저장 되고 1번 검사를 해야 레퍼런스를 세팅 할수 있다.
        m_fRefValX = Rslt.iPosX ;
        m_fRefValY = Rslt.iPosY ;
        m_fRefValT = 0.0 ;
        UserINI.Save(sFilePath, sTemp , "m_fRefValX", m_fRefValX);
        UserINI.Save(sFilePath, sTemp , "m_fRefValY", m_fRefValY);
        UserINI.Save(sFilePath, sTemp , "m_fRefValT", m_fRefValT);
    }
}


void COcv::ApplyPara(bool _bTable)
{
    if(_bTable) {
        ParaUi.edInspOft    -> Text = TrPr.iInspOft   ;
        ParaUi.edNoInspOft  -> Text = TrPr.iNoInspOft ;
        ParaUi.edThreshold  -> Text = TrPr.iThreshold ;
        ParaUi.edInspFreq   -> Text = TrPr.iInspFreq  ;

        ParaUi.edSinc       -> Text = Para.fSinc      ;
    }
    else {
        int   iTemp ;
        float fTemp ;

        iTemp = StrToIntDef(ParaUi.edInspOft    -> Text , 2  ); if (TrPr.iInspOft   != iTemp) { Trace("Operator" , "Both Ocv_InspOft values are different."); }
        iTemp = StrToIntDef(ParaUi.edNoInspOft  -> Text , 1  ); if (TrPr.iNoInspOft != iTemp) { Trace("Operator" , "Both Ocv_NoInspOft values are different."); }
        iTemp = StrToIntDef(ParaUi.edThreshold  -> Text , 100); if (TrPr.iThreshold != iTemp) { Trace("Operator" , "Both Ocv_Threshold values are different."); }
        iTemp = StrToIntDef(ParaUi.edInspFreq   -> Text , 10 ); if (TrPr.iInspFreq  != iTemp) { Trace("Operator" , "Both Ocv_InspFreq values are different."); }

        fTemp = StrToIntDef(ParaUi.edSinc       -> Text , 0  ); if (Para.fSinc      != fTemp) { Trace("Operator" , "Both Ocv_Sinc values are different."); }


        TrPr.iInspOft   = StrToIntDef(ParaUi.edInspOft    -> Text , 2  ) ; ParaUi.edInspOft    -> Text = TrPr.iInspOft  ;
        TrPr.iNoInspOft = StrToIntDef(ParaUi.edNoInspOft  -> Text , 1  ) ; ParaUi.edNoInspOft  -> Text = TrPr.iNoInspOft;
        TrPr.iThreshold = StrToIntDef(ParaUi.edThreshold  -> Text , 100) ; ParaUi.edThreshold  -> Text = TrPr.iThreshold;
        TrPr.iInspFreq  = StrToIntDef(ParaUi.edInspFreq   -> Text , 10 ) ; ParaUi.edInspFreq   -> Text = TrPr.iInspFreq ;

        Para.fSinc      = StrToIntDef(ParaUi.edSinc       -> Text , 0  ) ; ParaUi.edSinc       -> Text = Para.fSinc     ;
        ApplyPara(true);
    }
}

bool COcv::Train(IImage * _pImg)
{
    bool bRet ;

    bRet = OCV_Train(_pImg , Tracker -> GetTRect() , TrPr , m_pTrArea , m_pTrImage);

    if(FrmTrainChk != NULL) {
        delete FrmTrainChk ;
    }
    FrmTrainChk = new TFrmTrainChk (Application);
    FrmTrainChk -> SetTrainArea (m_pTrArea );
    FrmTrainChk -> SetTrainImg  (m_pTrImage);
    FrmTrainChk -> Show() ;

    return bRet ;
}

bool COcv::Process(IImage * _pImg , OCV_Rslt * _pRslt)
{
    bool bRet ;
    Para.iThreshold = TrPr.iThreshold ;

    bRet = OCV_Inspect(_pImg , m_pTrArea , m_pTrImage , GetOfsRect(Tracker -> GetOutRect()) , Para , m_pRsltArea , _pRslt);
    memcpy(&Rslt , _pRslt , sizeof(OCV_Rslt));

    return bRet ;
}

void COcv::PaintRslt(IImage * _pImg , HDC _hHdc)
{
    OCV_PaintRslt(_pImg , _hHdc , m_pRsltArea ,  &Rslt , Disp);
}

void COcv::GetRefValOfs(float &_fX , float &_fY , float &_fT )
{
    _fX = Rslt.iPosX - m_fRefValX ;
    _fY = Rslt.iPosY - m_fRefValY ;
    _fT = 0                       ;
}





 //------------------------------------------------------------------------------------------
CProcPkg::CProcPkg()
{
    m_sPkgName = "Process" ;
    m_iProcCnt = 0  ;
    m_pProc    = NULL ;
}
CProcPkg::~CProcPkg()
{
    //DelProcList();
}

//AnsiString m_sPkgName ;
//int        m_iProcCnt ;
//CProc *    m_pProc    ;

void CProcPkg::DelProcList()
{
    static int iCnt ;

    Trace("ProcList",("DelProcList"+AnsiString(iCnt)).c_str());
    if(m_pProc) {
        for(int i = 0 ; i < m_iProcCnt ; i++) {
                 if(m_pProc[i]->GetProcType() == ptEdg)delete (CEdg*)m_pProc[i];
            else if(m_pProc[i]->GetProcType() == ptLin)delete (CLin*)m_pProc[i];
            else if(m_pProc[i]->GetProcType() == ptLcp)delete (CLcp*)m_pProc[i];
            else if(m_pProc[i]->GetProcType() == ptRct)delete (CRct*)m_pProc[i];
            else if(m_pProc[i]->GetProcType() == ptTsd)delete (CTsd*)m_pProc[i];
            else if(m_pProc[i]->GetProcType() == ptBlb)delete (CBlb*)m_pProc[i];
            else if(m_pProc[i]->GetProcType() == ptCal)delete (CCal*)m_pProc[i];
            else if(m_pProc[i]->GetProcType() == ptCcl)delete (CCcl*)m_pProc[i];
            else if(m_pProc[i]->GetProcType() == ptOcv)delete (COcv*)m_pProc[i];
            else if(m_pProc[i]->GetProcType() == ptWrt)delete (CWrt*)m_pProc[i];
            //delete m_pProc[i] ;
            m_pProc[i]=NULL;
        }
        delete [] m_pProc ;
        m_pProc = NULL;
    }
    iCnt++;

}

void  CProcPkg::SetProcList(int _iProcCnt , EN_PROC_TYPE * _pProcType , int * _pIntPara)
{
    if(m_pProc) {
        for(int i = 0 ; i < m_iProcCnt ; i++) {
            delete m_pProc[i] ;
            m_pProc[i]=NULL;
        }
        delete [] m_pProc ;
    }

    m_iProcCnt = _iProcCnt ;

    m_pProc = new CProc * [_iProcCnt];

    for(int i= 0 ; i < _iProcCnt ; i++) {
             if(_pProcType[i] == ptEdg) m_pProc[i] = new CEdg() ;
        else if(_pProcType[i] == ptLin) m_pProc[i] = new CLin() ;
        else if(_pProcType[i] == ptLcp) m_pProc[i] = new CLcp() ;
        else if(_pProcType[i] == ptRct) m_pProc[i] = new CRct() ;
        else if(_pProcType[i] == ptTsd) m_pProc[i] = new CTsd(_pIntPara[i]) ;
        else if(_pProcType[i] == ptBlb) m_pProc[i] = new CBlb() ;
        else if(_pProcType[i] == ptCal) m_pProc[i] = new CCal() ;
        else if(_pProcType[i] == ptCcl) m_pProc[i] = new CCcl() ;
        else if(_pProcType[i] == ptOcv) m_pProc[i] = new COcv() ;
        else if(_pProcType[i] == ptWrt) m_pProc[i] = new CWrt() ;
        else                            ShowMessage("Unkown Processing Type!");
    }
}

void CProcPkg::SetPaintCallback(TOnChanged ChangeFunc)
{
    for(int i = 0 ; i < m_iProcCnt ; i++) {
        m_pProc[i] -> SetPaintCallback(ChangeFunc);
    }
}

void CProcPkg::SetTrackerVisible(int _iProcId , bool _bVisible)
{
    if(_iProcId >= m_iProcCnt) return  ;
    //for(int i = 0 ; i < m_iProcCnt ; i++) {
        m_pProc[_iProcId] -> SetTrackerVisible(_bVisible) ;
    //}
}

void CProcPkg::SetTrackerEnabled(int _iProcId , bool _bEnabled)
{
    if(_iProcId >= m_iProcCnt) return  ;
    //for(int i = 0 ; i < m_iProcCnt ; i++) {
        m_pProc[_iProcId] -> SetTrackerVisible(_bEnabled) ;
    //}
}

bool CProcPkg::MouseTrackerDn(int _iProcId , TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    if(_iProcId >= m_iProcCnt) return false ;
    //for(int i = 0 ; i < m_iProcCnt ; i++) {
        if(m_pProc[_iProcId] -> MouseTrackerDn(Button, Shift, X, Y , _fScaleX , _fScaleY)) return true ;
    //}
    return false ;
}

bool CProcPkg::MouseTrackerMv(int _iProcId , TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    if(_iProcId >= m_iProcCnt) return false ;
    //for(int i = 0 ; i < m_iProcCnt ; i++) {
        m_pProc[_iProcId] -> MouseTrackerMv(Shift, X, Y , _fScaleX , _fScaleY) ;
    //}
    return false ;
}
void CProcPkg::MouseTrackerUp(int _iProcId , TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    if(_iProcId >= m_iProcCnt) return  ;
    //for(int i = 0 ; i < m_iProcCnt ; i++) {
        m_pProc[_iProcId] -> MouseTrackerUp(Button,Shift, X, Y , _fScaleX , _fScaleY) ;
    //}
}
void CProcPkg::PaintTracker(int _iProcId ,HDC _hHdc , float _fScaleX , float _fScaleY)
{
    if(_iProcId >= m_iProcCnt) return ;
    //for(int i = 0 ; i < m_iProcCnt ; i++) {
        m_pProc[_iProcId] -> PaintTracker( _hHdc , _fScaleX , _fScaleY) ;
    //}
}

void CProcPkg::ShowParaForm(TPageControl * _pcBase)
{
    TTabSheet * tsInsp ;

    EN_PROC_TYPE iProcType ;

    for(int i = 0 ; i < m_iProcCnt ; i++) {
        iProcType = m_pProc[i] -> GetProcType() ;


        tsInsp  = new TTabSheet   (_pcBase );
        tsInsp->PageControl = _pcBase  ; // 이부분이 반드시 필요
        m_pProc[i] -> ShowParaForm(tsInsp);

             if(iProcType == ptEdg) tsInsp -> Caption = "Edge"        ;
        else if(iProcType == ptLin) tsInsp -> Caption = "Line"        ;
        else if(iProcType == ptLcp) tsInsp -> Caption = "LCP"         ;
        else if(iProcType == ptRct) tsInsp -> Caption = "Rect"        ;
        else if(iProcType == ptTsd) tsInsp -> Caption = "Threshold"   ;
        else if(iProcType == ptBlb) tsInsp -> Caption = "Blob"        ;
        else if(iProcType == ptCal) tsInsp -> Caption = "Calibration" ;
        else if(iProcType == ptCcl) tsInsp -> Caption = "Circle"      ;
        else if(iProcType == ptOcv) tsInsp -> Caption = "Ocv"         ;
        else if(iProcType == ptWrt) tsInsp -> Caption = "WaferRect"   ;
        else                        tsInsp -> Caption = ""            ;
    }
}


void CProcPkg::LoadPara(bool _bLoad , AnsiString _sFilePath)
{
    for(int i = 0 ; i < m_iProcCnt ; i++) {
        m_pProc[i]->LoadPara(_bLoad , _sFilePath , AnsiString(i));
    }


}

void CProcPkg::ApplyPara(bool _bTable)
{
    for(int i = 0 ; i < m_iProcCnt ; i++) {
        m_pProc[i]->ApplyPara(_bTable);
    }
}

EN_PROC_TYPE CProcPkg::GetProcType (int _iProcId )
{
    if(_iProcId >= m_iProcCnt) return ptInp ;

    return m_pProc[_iProcId]->GetProcType() ;
}

bool CProcPkg::Process (int _iProcId , IImage * _pImg , void * _pRslt)
{
    if(_iProcId >= m_iProcCnt) return false ;

    EN_PROC_TYPE iProcType = m_pProc[_iProcId] -> GetProcType() ;

    bool  bRet = false ;

         if(iProcType == ptEdg) bRet = ((CEdg*)m_pProc[_iProcId])->Process(_pImg , (EDG_Rslt *)_pRslt);
    else if(iProcType == ptLin) bRet = ((CLin*)m_pProc[_iProcId])->Process(_pImg , (LIN_Rslt *)_pRslt);
    else if(iProcType == ptLcp) bRet = ((CLcp*)m_pProc[_iProcId])->Process(_pImg , (LCP_Rslt *)_pRslt);
    else if(iProcType == ptRct) bRet = ((CRct*)m_pProc[_iProcId])->Process(_pImg , (RCT_Rslt *)_pRslt);
    else if(iProcType == ptTsd) bRet = ((CTsd*)m_pProc[_iProcId])->Process(_pImg , (TSD_Rslt *)_pRslt);
    else if(iProcType == ptBlb) bRet = ((CBlb*)m_pProc[_iProcId])->Process(_pImg , (BLB_Rslt *)_pRslt);
    else if(iProcType == ptCal) bRet = ((CCal*)m_pProc[_iProcId])->Process(_pImg , (CAL_Rslt *)_pRslt);
    else if(iProcType == ptCcl) bRet = ((CCcl*)m_pProc[_iProcId])->Process(_pImg , (CCL_Rslt *)_pRslt);
    else if(iProcType == ptOcv) bRet = ((COcv*)m_pProc[_iProcId])->Process(_pImg , (OCV_Rslt *)_pRslt);
    else if(iProcType == ptWrt) bRet = ((CWrt*)m_pProc[_iProcId])->Process(_pImg , (WRT_Rslt *)_pRslt);
    else                       {bRet = false ;  ShowMessage("Unkown Proccesing Type!");}

    return bRet ;

}

bool CProcPkg::Train (int _iProcId , IImage * _pImg )
{
    if(_iProcId >= m_iProcCnt) return false ;

    EN_PROC_TYPE iProcType = m_pProc[_iProcId] -> GetProcType() ;

    bool  bRet = false ;

    if(iProcType == ptOcv) bRet = ((COcv*)m_pProc[_iProcId])->Train(_pImg);
    else                  {bRet = false ;  ShowMessage("Unkown Proccesing Type!");}

    return bRet ;

}



void CProcPkg::PaintRslt (int _iProcId , IImage * _pImg , HDC  _hHdc )
{
    if(_iProcId >= m_iProcCnt) return  ;

    m_pProc[_iProcId] -> PaintRslt(_pImg , _hHdc);
}

void CProcPkg::PaintRsltAll (IImage * _pImg , HDC  _hHdc )
{

    for(int i = 0 ; i < m_iProcCnt ; i++) {
       m_pProc[i] -> PaintRslt(_pImg , _hHdc);
    }
}

void CProcPkg::SetInspOfs(int _iProcId , float  _fX , float  _fY)
{
    m_pProc[_iProcId] -> SetInspOfs(_fX , _fY);
}

void CProcPkg::GetRefValOfs(int _iProcId , float &_fX , float &_fY , float &_fT )
{
    m_pProc[_iProcId] -> GetRefValOfs(_fX , _fY , _fT);
}

void CProcPkg::SetRefVal(int _iProcId , float  _fX , float  _fY , float  _fT )
{
    m_pProc[_iProcId] -> SetRefVal(_fX,_fY,_fT);
}

void CProcPkg::ClrRefVal(int _iProcId )
{
    m_pProc[_iProcId] -> ClrRefVal();
}

void CProcPkg::GetRefVal(int _iProcId , float &_fX , float &_fY , float &_fT )
{
    m_pProc[_iProcId] -> GetRefVal(_fX,_fY,_fT);
}
