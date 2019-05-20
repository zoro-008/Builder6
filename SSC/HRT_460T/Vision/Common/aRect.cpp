
//---------------------------------------------------------------------------
#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------
#include "aRect.h"
#include "BaseMaths.h"

CRect::CRect()
{
}

CRect::~CRect()
{

}

bool CRect::Inspect(CImage * _pImg  , TRect _tRectL , TRect _tRectT , TRect _tRectR , TRect _tRectB , TPara _tPara , TRslt * _pRslt)
{
    DWORD dwTime =GetTime();
    _pRslt->Clear();

    double dAngleSum = 0.0;
    double dTemp ;

    //Para Check & Err Check
    if(_pImg == NULL             ) {_pRslt->sErrMsg = "Image Buffer is NULL."    ; return false ; }
    if(_pImg -> SetRect(&_tRectL)) {_pRslt->sErrMsg = "RectL Overed Image Size." ; return false ; }
    if(_pImg -> SetRect(&_tRectT)) {_pRslt->sErrMsg = "RectT Overed Image Size." ; return false ; }
    if(_pImg -> SetRect(&_tRectR)) {_pRslt->sErrMsg = "RectR Overed Image Size." ; return false ; }
    if(_pImg -> SetRect(&_tRectB)) {_pRslt->sErrMsg = "RectB Overed Image Size." ; return false ; }

    //CLine        Line ;
    CLine::TPara LinePara ;


    LinePara.bLtToDk     = !_tPara.bLtOnDk     ;
    LinePara.iSampleGap  =  _tPara.iSampleGap  ;
    LinePara.iLineGap    =  _tPara.iLineGap    ;
    LinePara.iEndOffset  =  _tPara.iEndOffset  ;
    LinePara.iCntrOffset =  _tPara.iCntrOffset ;
    LinePara.EdgPara     =  _tPara.EdgPara     ;

    //Left
    LinePara.iScanDirect =  CLine::lsLtToRt ;
    if(!CLine::Inspect(_pImg  , _tRectL , LinePara , &_pRslt -> LinRsltL)) {_pRslt->sErrMsg = _pRslt->LinRsltL.sErrMsg;  return false ;}
    for(int i = 0 ; i < _pRslt -> LinRsltL.EdgList.GetDataCnt() ;i++){
        _pRslt ->lEdgeList.PushBack(_pRslt -> LinRsltL.EdgList.GetCrntData(!i));
    }

    //Top
    LinePara.iScanDirect = CLine::lsUpToDn ;
    if(!CLine::Inspect(_pImg  , _tRectT , LinePara , &_pRslt -> LinRsltT)) {_pRslt->sErrMsg = _pRslt->LinRsltT.sErrMsg;  return false ;}
    for(int i = 0 ; i < _pRslt -> LinRsltT.EdgList.GetDataCnt() ;i++){
        _pRslt ->lEdgeList.PushBack(_pRslt -> LinRsltT.EdgList.GetCrntData(!i));
    }

    //Right
    LinePara.iScanDirect = CLine::lsRtToLt ;
    if(!CLine::Inspect(_pImg  , _tRectR , LinePara , &_pRslt -> LinRsltR)) {_pRslt->sErrMsg = _pRslt->LinRsltR.sErrMsg;  return false ;}
    for(int i = 0 ; i < _pRslt -> LinRsltR.EdgList.GetDataCnt() ;i++){
        _pRslt ->lEdgeList.PushBack(_pRslt -> LinRsltR.EdgList.GetCrntData(!i));
    }

    //Bottom
    LinePara.iScanDirect = CLine::lsDnToUp ;
    if(!CLine::Inspect(_pImg  , _tRectB , LinePara , &_pRslt -> LinRsltB)) {_pRslt->sErrMsg = _pRslt->LinRsltB.sErrMsg;  return false ;}
    for(int i = 0 ; i < _pRslt -> LinRsltB.EdgList.GetDataCnt() ;i++){
        _pRslt ->lEdgeList.PushBack(_pRslt -> LinRsltB.EdgList.GetCrntData(!i));
    }

    if(!CMath::GetLineCrossPnt(_pRslt->LinRsltL.StartPnt , _pRslt->LinRsltL.EndPnt , _pRslt->LinRsltT.StartPnt , _pRslt->LinRsltT.EndPnt , _pRslt->CrossPntLT)){_pRslt->sErrMsg = "LT Lines are parallel"; return false ; }
    if(!CMath::GetLineCrossPnt(_pRslt->LinRsltT.StartPnt , _pRslt->LinRsltT.EndPnt , _pRslt->LinRsltR.StartPnt , _pRslt->LinRsltR.EndPnt , _pRslt->CrossPntRT)){_pRslt->sErrMsg = "RT Lines are parallel"; return false ; }
    if(!CMath::GetLineCrossPnt(_pRslt->LinRsltR.StartPnt , _pRslt->LinRsltR.EndPnt , _pRslt->LinRsltB.StartPnt , _pRslt->LinRsltB.EndPnt , _pRslt->CrossPntRB)){_pRslt->sErrMsg = "RB Lines are parallel"; return false ; }
    if(!CMath::GetLineCrossPnt(_pRslt->LinRsltB.StartPnt , _pRslt->LinRsltB.EndPnt , _pRslt->LinRsltL.StartPnt , _pRslt->LinRsltL.EndPnt , _pRslt->CrossPntLB)){_pRslt->sErrMsg = "LB Lines are parallel"; return false ; }

    _pRslt -> CenterPnt.x = (_pRslt->CrossPntLT.x + _pRslt->CrossPntRT.x + _pRslt->CrossPntRB.x + _pRslt->CrossPntLB.x) / 4.0 ;
    _pRslt -> CenterPnt.y = (_pRslt->CrossPntLT.y + _pRslt->CrossPntRT.y + _pRslt->CrossPntRB.y + _pRslt->CrossPntLB.y) / 4.0 ;


    //기준이 0도 이다.
    _pRslt -> dAngleL = CMath::GetLineAngle(_pRslt->LinRsltL.StartPnt.x , _pRslt->LinRsltL.StartPnt.y , _pRslt->LinRsltL.EndPnt.x   , _pRslt->LinRsltL.EndPnt.y  ) - 270 ; dAngleSum += _pRslt -> dAngleL ;
    _pRslt -> dAngleT = CMath::GetLineAngle(_pRslt->LinRsltT.EndPnt.x   , _pRslt->LinRsltT.EndPnt.y   , _pRslt->LinRsltT.StartPnt.x , _pRslt->LinRsltT.StartPnt.y) - 180 ; dAngleSum += _pRslt -> dAngleT ;
    _pRslt -> dAngleR = CMath::GetLineAngle(_pRslt->LinRsltR.StartPnt.x , _pRslt->LinRsltR.StartPnt.y , _pRslt->LinRsltR.EndPnt.x   , _pRslt->LinRsltR.EndPnt.y  ) - 270 ; dAngleSum += _pRslt -> dAngleR ;
    _pRslt -> dAngleB = CMath::GetLineAngle(_pRslt->LinRsltB.EndPnt.x   , _pRslt->LinRsltB.EndPnt.y   , _pRslt->LinRsltB.StartPnt.x , _pRslt->LinRsltB.StartPnt.y) - 180 ; dAngleSum += _pRslt -> dAngleB ;

    _pRslt -> dAngle = dAngleSum / 4.0 ;

    _pRslt -> dLengthL = CMath::GetLineLenth(_pRslt->CrossPntLB.x , _pRslt->CrossPntLB.y , _pRslt->CrossPntLT.x , _pRslt->CrossPntLT.y); //왼쪽변.
    _pRslt -> dLengthT = CMath::GetLineLenth(_pRslt->CrossPntLT.x , _pRslt->CrossPntLT.y , _pRslt->CrossPntRT.x , _pRslt->CrossPntRT.y); //위쪽 변
    _pRslt -> dLengthR = CMath::GetLineLenth(_pRslt->CrossPntRT.x , _pRslt->CrossPntRT.y , _pRslt->CrossPntRB.x , _pRslt->CrossPntRB.y); //오른쪽변
    _pRslt -> dLengthB = CMath::GetLineLenth(_pRslt->CrossPntRB.x , _pRslt->CrossPntRB.y , _pRslt->CrossPntLB.x , _pRslt->CrossPntLB.y); //아래변

    _pRslt->dInspTime = CMath::RoundOff(GetTime()- dwTime , 2) ;

    return true;

}

/*
void CRect::PaintRslt(HDC _hHdc , TRslt * _pRslt , RCT_Disp _tDisp , float _fScaleX , float _fScaleY)
{
    CGdi *Gdi = new CGdi(_hHdc);
    AnsiString sTemp ;
    Gdi -> SetScale(_fScaleX , _fScaleY);

    if(_tDisp.clRect != clNone) {
        Gdi->m_tPen.Color = _tDisp.clRect ;
        Gdi->m_tPen.Style = PS_SOLID ;
        Gdi->Line((int)_pRslt -> CrossPntLT.x , (int)_pRslt -> CrossPntLT.y ,(int)_pRslt -> CrossPntRT.x , (int)_pRslt -> CrossPntRT.y);
        Gdi->Line((int)_pRslt -> CrossPntRT.x , (int)_pRslt -> CrossPntRT.y ,(int)_pRslt -> CrossPntRB.x , (int)_pRslt -> CrossPntRB.y);
        Gdi->Line((int)_pRslt -> CrossPntRB.x , (int)_pRslt -> CrossPntRB.y ,(int)_pRslt -> CrossPntLB.x , (int)_pRslt -> CrossPntLB.y);
        Gdi->Line((int)_pRslt -> CrossPntLB.x , (int)_pRslt -> CrossPntLB.y ,(int)_pRslt -> CrossPntLT.x , (int)_pRslt -> CrossPntLT.y);

        Gdi->m_tPen.Style = PS_DOT ;
        Gdi->Line((int)_pRslt -> OffsetPntLT.x , (int)_pRslt -> OffsetPntLT.y ,(int)_pRslt -> OffsetPntRT.x , (int)_pRslt -> OffsetPntRT.y);
        Gdi->Line((int)_pRslt -> OffsetPntRT.x , (int)_pRslt -> OffsetPntRT.y ,(int)_pRslt -> OffsetPntRB.x , (int)_pRslt -> OffsetPntRB.y);
        Gdi->Line((int)_pRslt -> OffsetPntRB.x , (int)_pRslt -> OffsetPntRB.y ,(int)_pRslt -> OffsetPntLB.x , (int)_pRslt -> OffsetPntLB.y);
        Gdi->Line((int)_pRslt -> OffsetPntLB.x , (int)_pRslt -> OffsetPntLB.y ,(int)_pRslt -> OffsetPntLT.x , (int)_pRslt -> OffsetPntLT.y);

        Gdi->m_tPen.Style = PS_SOLID ;

    }

    LineL.PaintRslt(_hHdc , &_pRslt->LinRsltL , _tDisp.LinDisp , _fScaleX, _fScaleY) ;
    LineT.PaintRslt(_hHdc , &_pRslt->LinRsltT , _tDisp.LinDisp , _fScaleX, _fScaleY) ;
    LineR.PaintRslt(_hHdc , &_pRslt->LinRsltR , _tDisp.LinDisp , _fScaleX, _fScaleY) ;
    LineB.PaintRslt(_hHdc , &_pRslt->LinRsltB , _tDisp.LinDisp , _fScaleX, _fScaleY) ;





    if(_tDisp.clPoint != clNone) {
        Gdi->m_tPen.Color = _tDisp.clPoint ;
        Gdi->LineX((int)_pRslt->CenterPnt.x ,(int)_pRslt->CenterPnt.y , 5) ;

        Gdi->LineX((int)_pRslt->CrossPntLT.x ,(int)_pRslt->CrossPntLT.y , 5) ;
        Gdi->LineX((int)_pRslt->CrossPntRT.x ,(int)_pRslt->CrossPntRT.y , 5) ;
        Gdi->LineX((int)_pRslt->CrossPntRB.x ,(int)_pRslt->CrossPntRB.y , 5) ;
        Gdi->LineX((int)_pRslt->CrossPntLB.x ,(int)_pRslt->CrossPntLB.y , 5) ;


    }
    if(_tDisp.clText != clNone) {
        Gdi->m_tText.Color = _tDisp.clText ;
        sTemp = sTemp.sprintf("X:%.3f Y:%.3f",_pRslt->CenterPnt.x , _pRslt->CenterPnt.y) ;
        Gdi->Text((int)_pRslt->CenterPnt.x + 5 ,(int)_pRslt->CenterPnt.y + 5 , sTemp.c_str()) ;
        sTemp = sTemp.sprintf("Angle:%.3f",_pRslt->fAngle) ;
        Gdi->Text((int)_pRslt->CenterPnt.x + 5 ,(int)_pRslt->CenterPnt.y + 15 , sTemp.c_str()) ;
        sTemp = sTemp.sprintf("OutPxCnt:%d",_pRslt->iChkPxOutCnt) ;
        Gdi->Text((int)_pRslt->CenterPnt.x + 5 ,(int)_pRslt->CenterPnt.y + 25 , sTemp.c_str()) ;
    }


    delete Gdi;
}
*/





