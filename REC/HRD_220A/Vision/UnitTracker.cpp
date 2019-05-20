//---------------------------------------------------------------------------

#include <stdio.h>

#pragma hdrstop
#include "UnitTracker.h"

#include "UserINI.h"
#include "BaseMaths.h"
#include "UnitGdi.h"

#pragma package(smart_init)
void CTracker::SetOnPaint(FOnPaint function)
{
  m_fpOnPaint = function ;
}

void CTracker::SetTag(int value)
{
    if(FTag == value) return ;
    FTag = value ;
    if(m_fpOnPaint) m_fpOnPaint() ;
}

void CTracker::SetVisible(bool value)
{
    if(FVisible == value) return ;
    FVisible = value ;
    if(m_fpOnPaint) m_fpOnPaint() ;
}

void CTracker::SetEnabled(bool value)
{
    if(FEnabled == value) return ;
    FEnabled = value ;
    m_bSelected = false ;
    if(m_fpOnPaint) m_fpOnPaint() ;
}

void CTracker::SetBandEnabled(bool value)
{
    if(FBandEnabled == value) return ;
    FBandEnabled = value ;
    if(m_fpOnPaint) m_fpOnPaint() ;
}
void CTracker::SetRoiEnabled(bool value)
{
    if(FRoiEnabled == value) return ;
    FRoiEnabled = value ;
    if(m_fpOnPaint) m_fpOnPaint() ;
}

void CTracker::SetTrackerType(ETrackerType value)
{
    if(FTrackerType == value) return ;

    TDRect Rect = GetDRect();

    TDPoint aRectPoint[4];
    aRectPoint[piRectLT].x = Rect.left   ;    aRectPoint[piRectLT].y = Rect.top    ;
    aRectPoint[piRectRT].x = Rect.right  ;    aRectPoint[piRectRT].y = Rect.top    ;
    aRectPoint[piRectRB].x = Rect.right  ;    aRectPoint[piRectRB].y = Rect.bottom ;
    aRectPoint[piRectLB].x = Rect.left   ;    aRectPoint[piRectLB].y = Rect.bottom ;




    if(value == ttRect){
        delete [] m_aPoints ;

        m_iPointCount = 4 ;
        m_aPoints = new TDPoint[m_iPointCount] ;

        for(int i = 0 ; i < m_iPointCount ; i++){
            m_aPoints[i] = aRectPoint[i] ;
        }
    }
    else if(value == ttLine){
        delete [] m_aPoints ;

        m_iPointCount = 2 ;
        m_aPoints = new TDPoint[m_iPointCount] ;
        m_aPoints[piLineStt]=aRectPoint[piRectLT];
        m_aPoints[piLineEnd]=aRectPoint[piRectRB];
    }
    else if(value == ttCircle){
        delete [] m_aPoints ;
        m_iPointCount = 1 ;
        m_aPoints = new TDPoint[m_iPointCount] ;
        m_aPoints[piCircleCnt].x=(Rect.left + Rect.right )/2.0 ;
        m_aPoints[piCircleCnt].y=(Rect.top  + Rect.bottom)/2.0 ;
        FCircleRadian = Rect.Width() / 2.0 ;

    }
    else if(value == ttPolygon){
        delete [] m_aPoints ;

        m_iPointCount = 4 ;
        m_aPoints = new TDPoint[m_iPointCount] ;

        for(int i = 0 ; i < m_iPointCount ; i++){
            m_aPoints[i] = aRectPoint[i] ;
        }
    }
    else if(value == ttPolyline){
        delete [] m_aPoints ;

        m_iPointCount = 4 ;
        m_aPoints = new TDPoint[m_iPointCount] ;

        for(int i = 0 ; i < m_iPointCount ; i++){
            m_aPoints[i] = aRectPoint[i] ;
        }
    }
    else{
        delete [] m_aPoints ;

        m_iPointCount = 4 ;
        m_aPoints = new TDPoint[m_iPointCount] ;

        for(int i = 0 ; i < m_iPointCount ; i++){
            m_aPoints[i] = aRectPoint[i] ;
        }
    }

    FTrackerType = value ;
    if(m_fpOnPaint) m_fpOnPaint() ;
}

void CTracker::SetCircleRadian(double value)
{
    if(FCircleRadian == value) return ;
    if(value < 1) value = 1 ;
    FCircleRadian = value ;
    if(m_fpOnPaint&&FVisible) m_fpOnPaint() ;

}


void CTracker::SetLineColor(TColor value)
{
    if(FLineColor == value) return ;
    FLineColor = value ;
    if(m_fpOnPaint&&FVisible) m_fpOnPaint() ;
}
void CTracker::SetRectPointColor(TColor value)
{
    if(FRectPointColor == value) return ;
    FRectPointColor = value ;
    if(m_fpOnPaint&&FVisible) m_fpOnPaint() ;
}
void CTracker::SetPointColor(TColor value)
{
    if(FPointColor == value) return ;
    FPointColor = value ;
    if(m_fpOnPaint&&FVisible) m_fpOnPaint() ;
}
void CTracker::SetBandWidth(double value)
{
    if(FBandWidth == value) return ;
    if(value < 1) value = 1 ;
    FBandWidth = value ;
    if(m_fpOnPaint&&FVisible) m_fpOnPaint() ;
}
void CTracker::SetRoiWidth(double value)
{
    if(FRoiWidth == value) return ;
    if(value < 1) value = 1 ;
    FRoiWidth = value ;
    if(m_fpOnPaint&&FVisible) m_fpOnPaint() ;
}

void CTracker::SetLineWidth(int value)
{
    if(FLineWidth == value) return ;
    if(value < 1) value = 1 ;
    FLineWidth = value ;
    if(m_fpOnPaint&&FVisible) m_fpOnPaint() ;
}

void CTracker::SetPointSize(int value)
{
    if(FPointSize == value) return ;
    if(value < 2) value = 2 ;
    FPointSize = value ;
    if(m_fpOnPaint&&FVisible) m_fpOnPaint() ;
}

void CTracker::SetRectPointSize(int value)
{
    if(FRectPointSize == value) return ;
    if(value < 2) value = 2 ;
    FRectPointSize = value ;
    if(m_fpOnPaint&&FVisible) m_fpOnPaint() ;
}

void CTracker::SetCaption(AnsiString value)
{
    if(FCaption == value) return ;
    FCaption = value ;
    if(m_fpOnPaint&&FVisible) m_fpOnPaint() ;
}


//트렉커 클래스.
//==============================================================================
TDPoint * CTracker::GetPoints ()
{
    return m_aPoints ;

}

TRect CTracker::GetRect()
{
    TDRect Rect = GetDRect() ;
    TRect RetRect(Rect.Left+0.5 , Rect.Top+0.5 , Rect.Right+0.5 , Rect.Bottom+0.5);

    return RetRect ;

}
void CTracker::SetRect (TRect  _tRect )
{
    TDRect Rect ;

    Rect.Left   = _tRect.Left   ;
    Rect.Right  = _tRect.Right  ;
    Rect.Top    = _tRect.Top    ;
    Rect.Bottom = _tRect.Bottom ;

    SetDRect(Rect);
}

TDRect CTracker::GetDRect()
{
    double dLeft   = FLT_MAX ;
    double dTop    = FLT_MAX ;
    double dRight  = 0 ;
    double dBottom = 0 ;




    for(int i = 0 ; i < m_iPointCount ; i++) {
        if(m_aPoints[i].x < dLeft  ) dLeft   = m_aPoints[i].x ;
        if(m_aPoints[i].x > dRight ) dRight  = m_aPoints[i].x ;
        if(m_aPoints[i].y < dTop   ) dTop    = m_aPoints[i].y ;
        if(m_aPoints[i].y > dBottom) dBottom = m_aPoints[i].y ;
    }

    TDRect tRetRect(dLeft , dTop , dRight , dBottom);

    if(FTrackerType == ttCircle) {
         tRetRect.left   = m_aPoints[piCircleCnt].x - FCircleRadian ;
         tRetRect.top    = m_aPoints[piCircleCnt].y - FCircleRadian ;
         tRetRect.right  = m_aPoints[piCircleCnt].x + FCircleRadian ;
         tRetRect.bottom = m_aPoints[piCircleCnt].y + FCircleRadian ;
    }

    return tRetRect ;

}
void CTracker::SetDRect(TDRect _tDRect)
{
    TDRect OriRect = GetDRect() ;

    double dScaleX = _tDRect.Width()  / OriRect.Width()  ;
    double dScaleY = _tDRect.Height() / OriRect.Height() ;

    double dOffsetX = _tDRect.Left - OriRect.Left ;
    double dOffsetY = _tDRect.Top  - OriRect.Top  ;

    for(int i = 0 ; i < m_iPointCount ; i++) {
        m_aPoints[i].x = m_aPoints[i].x + dOffsetX;
        m_aPoints[i].y = m_aPoints[i].y + dOffsetY;

        m_aPoints[i].x =   OriRect.Left + (m_aPoints[i].x - OriRect.Left) * dScaleX ;
        m_aPoints[i].y =   OriRect.Top  + (m_aPoints[i].y - OriRect.Top ) * dScaleY ;
    }
}

TDRect CTracker::GetRoiDRect()
{
    TDRect tRetRect = GetDRect() ;
    tRetRect.left   -= FRoiWidth ;
    tRetRect.top    -= FRoiWidth ;
    tRetRect.right  += FRoiWidth ;
    tRetRect.bottom += FRoiWidth ;

    return tRetRect ;
}

bool CTracker::GetMoved()
{
    return m_bMoved ;
}

bool CTracker::GetSelected()
{
    return m_bSelected ;
}

void CTracker::SetSelected (bool _bSelected)
{
    m_bSelected = _bSelected ;
}

bool CTracker::InsertPoint(int _iNo)
{
    if(_iNo < 0               ) _iNo = 0 ;
    if(_iNo >= m_iPointCount+1) _iNo = m_iPointCount+1 ;

    m_iPointCount++;
    TDPoint * aTempPnt = new TDPoint[m_iPointCount] ;
    int iInserted = 0 ;
    for(int i = 0 ; i < m_iPointCount ; i++){
        if(i==_iNo) {
            aTempPnt[i].x = m_aPoints[i].x + FPointSize ;
            aTempPnt[i].y = m_aPoints[i].y + FPointSize ;
            iInserted = 1 ;
            continue ;
        }
        aTempPnt[i].x = m_aPoints[i-iInserted].x ;
        aTempPnt[i].y = m_aPoints[i-iInserted].y ;
    }

    if(m_aPoints) delete [] m_aPoints    ;
    m_aPoints    = new TDPoint[m_iPointCount] ;

    for(int i = 0 ; i < m_iPointCount ; i++){
        m_aPoints[i] = aTempPnt[i] ;
    }

    delete [] aTempPnt ;

    return true ;
}

bool CTracker::DeletePoint(int _iNo)
{
    if(m_iPointCount < 1) return false ;
    if(_iNo < 0 || m_iPointCount <= _iNo) return false ;

    m_iPointCount--;
    TDPoint * aTempPnt = new TDPoint[m_iPointCount] ;
    int iDeleted = 0 ;

    for(int i = 0 ; i < m_iPointCount ; i++){
        if(i==_iNo) {
            iDeleted = 1 ;
        }
        aTempPnt[i].x = m_aPoints[i+iDeleted].x ;
        aTempPnt[i].y = m_aPoints[i+iDeleted].y ;
    }

    if(m_aPoints) delete [] m_aPoints    ;
    m_aPoints    = new TDPoint[m_iPointCount] ;

    for(int i = 0 ; i < m_iPointCount ; i++){
        m_aPoints[i] = aTempPnt[i] ;
    }

    delete [] aTempPnt ;

    return true ;

}
double CTracker::GetPointX(int _iNo)
{
    if(_iNo < 0 || m_iPointCount <= _iNo) return 0 ;
    return m_aPoints[_iNo].x ;
}

double CTracker::GetPointY(int _iNo)
{
    if(_iNo < 0 || m_iPointCount <= _iNo) return 0 ;
    return m_aPoints[_iNo].y ;
}

int CTracker::GetPointCount()
{
    return m_iPointCount ;

}

TDRect CTracker::GetBandDRect(EPointId _piPnt1 , EPointId _piPnt2 )
{
    TDRect tRetRect ;

    if(_piPnt1 < 0 || _piPnt1 >= m_iPointCount) return tRetRect ;
    if(_piPnt2 < 0 || _piPnt2 >= m_iPointCount) return tRetRect ;

    TDPoint Pnt1 = m_aPoints[_piPnt1] ;
    TDPoint Pnt2 = m_aPoints[_piPnt2] ;

    TDPoint tRsltPnt[4];

    GetLineBandPnts(Pnt1,Pnt2,FBandWidth,tRsltPnt); //_aPoints 4개 포인트 배열 넘겨야함.


    tRetRect.Left   = FLT_MAX ;
    tRetRect.Top    = FLT_MAX ;
    tRetRect.Right  = 0 ;
    tRetRect.Bottom = 0 ;
    for(int i = 0 ; i < 4 ; i++) {
        if(tRsltPnt[i].x < tRetRect.Left  ) tRetRect.Left   = tRsltPnt[i].x ;
        if(tRsltPnt[i].y < tRetRect.Top   ) tRetRect.Top    = tRsltPnt[i].y ;
        if(tRsltPnt[i].x > tRetRect.Right ) tRetRect.Right  = tRsltPnt[i].x ;
        if(tRsltPnt[i].y > tRetRect.Bottom) tRetRect.Bottom = tRsltPnt[i].y ;
    }

    return tRetRect ;

}

CTracker::TMoveStat CTracker::GetMoveStat()
{
    return MoveStat ;
}
//event link


bool CTracker::CheckRectIn(double _dPntX , double _dPntY , double _dX , double _dY , double _dPntHfSize) //사각포인트 중간크기를 넘김.
{
    return _dX > _dPntX -_dPntHfSize && _dX <= _dPntX +_dPntHfSize &&
           _dY > _dPntY -_dPntHfSize && _dY <= _dPntY +_dPntHfSize ;
}

bool CTracker::CheckCircleIn(double _dPntX , double _dPntY , double _dX , double _dY , double _dRad) //원의 반지름 넘김.
{
    return CMath::GetCircleInPoint(_dPntX , _dPntY , _dRad , _dX , _dY ) ;
}

void CTracker::GetLineBandPnts(TDPoint _tPnt1 , TDPoint _tPnt2 , double _dMargin , TDPoint * _a4Points) //_aPoints 4개 포인트 배열 넘겨야함.
{
    double dAngle = CMath::GetLineAngle(_tPnt1.x , _tPnt1.y , _tPnt2.x , _tPnt2.y);

    CMath::GetPntFromPntByAngLen( _tPnt1.x , _tPnt1.y ,                //입력 시작점.
                                 dAngle+90.0        ,                //입력 각도.
                                _dMargin            ,                //입력 길이.
                                &_a4Points[0].x , &_a4Points[0].y ); //출력 포인트.

    CMath::GetPntFromPntByAngLen( _tPnt2.x , _tPnt2.y ,                //입력 시작점.
                                 dAngle+90.0        ,                //입력 각도.
                                _dMargin            ,                //입력 길이.
                                &_a4Points[1].x , &_a4Points[1].y ); //출력 포인트.

    CMath::GetPntFromPntByAngLen( _tPnt2.x , _tPnt2.y ,                //입력 시작점.
                                 dAngle-90.0        ,                //입력 각도.
                                _dMargin            ,                //입력 길이.
                                &_a4Points[2].x , &_a4Points[2].y ); //출력 포인트.

    CMath::GetPntFromPntByAngLen( _tPnt1.x , _tPnt1.y ,                //입력 시작점.
                                 dAngle-90.0        ,                //입력 각도.
                                _dMargin            ,                //입력 길이.
                                &_a4Points[3].x , &_a4Points[3].y ); //출력 포인트.



}

bool CTracker::CheckLineIn(TDPoint * _aPoints , int _iPointCount , double _dX , double _dY , double _dMargin) //라인 클릭으로 판단할 마진 넘김.
{
    if(_iPointCount < 2) return false ;

    TDPoint P1 , P2 , Pnt;
    TDPoint aRectPnt[4] ;

    P1 = _aPoints[0] ;
    P2 = _aPoints[_iPointCount-1]  ;
    double dRectPntHfSize = FRectPointSize / 2.0 ;

    for(int i = 1 ; i <= _iPointCount ; i++) {
        GetLineBandPnts(P1,P2,dRectPntHfSize,aRectPnt);
        Pnt.x = _dX ;
        Pnt.y = _dY ;
        if(CMath::IsPntInPolygon(aRectPnt , 4 , Pnt))return true ;
        P2 = P1;
        P1 = _aPoints[i % _iPointCount];
    }

    return false ;

}



bool CTracker::TrackerDw(TMouseButton Button, TShiftState Shift , int x, int y , double _dScaleX, double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    if (!FVisible || !FEnabled ) return false ;

    //Rect.left    = (FLeft - _dOffsetX) * _fScaleX;
    //Rect.top     = (FTop  - _dOffsetY) * _fScaleY;
    //Rect.right   = (FLeft - _dOffsetX+ FWidth ) * _fScaleX  ;
    //Rect.bottom  = (FTop  - _dOffsetY+ FHeight) * _fScaleY  ;

    double dX = _dOffsetX + x / _dScaleX ;
    double dY = _dOffsetY + y / _dScaleY ;

    TDRect DRect = GetDRect() ;
    TRect  Rect  = GetRect () ;

    double dLeft   = DRect.left    ;
    double dTop    = DRect.top     ;
    double dRight  = DRect.right   ;
    double dBottom = DRect.bottom  ;
    double dWidth  = DRect.Width ();
    double dHeight = DRect.Height();

    double dPntHfSize     = FPointSize     / 2.0 ;
    double dRectPntHfSize = FRectPointSize / 2.0 ;

    double dCx = (dLeft + dRight )/2 ;
    double dCy = (dTop  + dBottom)/2 ;

    MoveStat.dX = dX ;
    MoveStat.dY = dY ;

    MoveStat.bRight = Button == mbRight ;


    if(FBandEnabled) { //밴드 클릭 이벤트
        if(FTrackerType == ttCircle){
            if (CheckRectIn  (dCx , dTop + FBandWidth , dX , dY , dRectPntHfSize)){
                MoveStat.eDnType=MoveStat.eLastDnType=dtBand ;
                m_bSelected = MoveStat.eDnType != dtNone ;
                if(m_fpOnPaint) m_fpOnPaint() ;
                return true ;
            }
        }
        else { //서클이 아니면 m_aPoints가 2개 이상이다.
            TDPoint aBandPnt[4] ;
            GetLineBandPnts(m_aPoints[0] , m_aPoints[1] , FBandWidth , aBandPnt);
            double dRectX , dRectY ;
            dRectX = (aBandPnt[2].x + aBandPnt[3].x) / 2.0 ;
            dRectY = (aBandPnt[2].y + aBandPnt[3].y) / 2.0 ;
            if (CheckRectIn (dRectX , dRectY , dX , dY , dRectPntHfSize)){
                MoveStat.eDnType=MoveStat.eLastDnType= dtBand ;
                m_bSelected = MoveStat.eDnType != dtNone ;
                if(m_fpOnPaint) m_fpOnPaint() ;
                return true ;
            }
        }
    }

    if(FRoiEnabled) { //ROI클릭 이벤트
        TDRect DRoiRect = GetRoiDRect();
        if (CheckRectIn  (dCx , dTop - FRoiWidth , dX , dY , dRectPntHfSize)){
            MoveStat.eDnType=MoveStat.eLastDnType= dtRoi ;
            m_bSelected = MoveStat.eDnType != dtNone ;
            if(m_fpOnPaint) m_fpOnPaint() ;
            return true ;
        }
    }

    if(FTrackerType == ttRect) {
        //Band
        //TFRect subrect = GetBTFRect() ;
        //double dBtmRectCntX = subrect.left + (subrect.right - subrect.left) /2 ;
        //double dBtmRectTopY = subrect.top ;
        //double fBCntX  = (fBtmRectCntX-PointSize * 2) * _fScaleX ;
        //double fBCntY  =  fBtmRectTopY                * _fScaleY ;

        //double dBCntX  = (dBtmRectCntX-PointSize * 2)  ;
        //double dBCntY  =  dBtmRectTopY                 ;

        //Point...
        //Point Click
             if (CheckCircleIn(dLeft  , dTop    , dX , dY , dPntHfSize)){MoveStat.eDnType=MoveStat.eLastDnType= dtLT ; }
        else if (CheckCircleIn(dRight , dTop    , dX , dY , dPntHfSize)){MoveStat.eDnType=MoveStat.eLastDnType= dtRT ; }
        else if (CheckCircleIn(dRight , dBottom , dX , dY , dPntHfSize)){MoveStat.eDnType=MoveStat.eLastDnType= dtRB ; }
        else if (CheckCircleIn(dLeft  , dBottom , dX , dY , dPntHfSize)){MoveStat.eDnType=MoveStat.eLastDnType= dtLB ; }

        //Rect Point Click
        else if (CheckRectIn  (dCx    , dTop    , dX , dY , dRectPntHfSize)){MoveStat.eDnType=MoveStat.eLastDnType= dtCT ; }
        else if (CheckRectIn  (dCx    , dBottom , dX , dY , dRectPntHfSize)){MoveStat.eDnType=MoveStat.eLastDnType= dtCB ; }
        else if (CheckRectIn  (dLeft  , dCy     , dX , dY , dRectPntHfSize)){MoveStat.eDnType=MoveStat.eLastDnType= dtLC ; }
        else if (CheckRectIn  (dRight , dCy     , dX , dY , dRectPntHfSize)){MoveStat.eDnType=MoveStat.eLastDnType= dtRC ; }

        else if (CheckLineIn  (m_aPoints , m_iPointCount , dX , dY , dRectPntHfSize)) MoveStat.eDnType =MoveStat.eLastDnType= dtMV   ;
        else                                                                          MoveStat.eDnType =MoveStat.eLastDnType= dtNone ;
    }
    else if(FTrackerType == ttLine){
             if (CheckCircleIn(m_aPoints[piLineStt].x , m_aPoints[piLineStt].y , dX , dY , dPntHfSize)){MoveStat.eDnType=MoveStat.eLastDnType= dtStt ;}
        else if (CheckCircleIn(m_aPoints[piLineEnd].x , m_aPoints[piLineEnd].y , dX , dY , dPntHfSize)){MoveStat.eDnType=MoveStat.eLastDnType= dtEnd ;}

        else if (CheckLineIn  (m_aPoints , m_iPointCount , dX , dY , dRectPntHfSize)) MoveStat.eDnType =MoveStat.eLastDnType= dtMV   ;
        else                                                                          MoveStat.eDnType =MoveStat.eLastDnType= dtNone ;
    }
    else if(FTrackerType == ttCircle){
        //Rect Point Click
             if (CheckRectIn  (dCx    , dTop    , dX , dY , dRectPntHfSize)){MoveStat.eDnType=MoveStat.eLastDnType= dtCT ; }
        else if (CheckRectIn  (dCx    , dBottom , dX , dY , dRectPntHfSize)){MoveStat.eDnType=MoveStat.eLastDnType= dtCB ; }
        else if (CheckRectIn  (dLeft  , dCy     , dX , dY , dRectPntHfSize)){MoveStat.eDnType=MoveStat.eLastDnType= dtLC ; }
        else if (CheckRectIn  (dRight , dCy     , dX , dY , dRectPntHfSize)){MoveStat.eDnType=MoveStat.eLastDnType= dtRC ; }

        else if (CheckCircleIn(m_aPoints[piCircleCnt].x , m_aPoints[piCircleCnt].y , dX , dY , dWidth/2.0+dRectPntHfSize)&&
                !CheckCircleIn(m_aPoints[piCircleCnt].x , m_aPoints[piCircleCnt].y , dX , dY , dWidth/2.0-dRectPntHfSize)){MoveStat.eDnType=MoveStat.eLastDnType= dtMV ;}
    }
    else if(FTrackerType == ttPolygon || FTrackerType == ttPolyline){
        //Rect Point Click
        MoveStat.eDnType=MoveStat.eLastDnType= dtNone ;
        int iPntCnt = m_iPointCount ;

        for(int i = 0 ; i < iPntCnt ; i++) {

            if (CheckCircleIn(m_aPoints[i].x  , m_aPoints[i].y , dX , dY , dPntHfSize)) {
                if(Shift.Contains(ssShift)&&Shift.Contains(ssCtrl)) {
                    DeletePoint(i);
                    break ;
                }
                else if(Shift.Contains(ssCtrl)) {
                    InsertPoint(i);
                    break ;
                }

            }
        }







        for(int i = 0 ; i < m_iPointCount ; i++){
            if(CheckCircleIn(m_aPoints[i].x  , m_aPoints[i].y , dX , dY , dPntHfSize)){MoveStat.eDnType=MoveStat.eLastDnType= (EDownType)i ; }
        }
        if(MoveStat.eDnType < (EDownType)0) {
            //나중에 하자..
            //     if (CheckRectIn  (dLeft  , dTop    , dX , dY , dRectPntHfSize)){MoveStat.eDnType = dtLT   ; }
            //else if (CheckRectIn  (dRight , dTop    , dX , dY , dRectPntHfSize)){MoveStat.eDnType = dtRT   ; }
            //else if (CheckRectIn  (dRight , dBottom , dX , dY , dRectPntHfSize)){MoveStat.eDnType = dtRB   ; }
            //else if (CheckRectIn  (dLeft  , dBottom , dX , dY , dRectPntHfSize)){MoveStat.eDnType = dtLB   ; }
            //else if (CheckRectIn  (dCx    , dTop    , dX , dY , dRectPntHfSize)){MoveStat.eDnType = dtCT   ; }
            //else if (CheckRectIn  (dCx    , dBottom , dX , dY , dRectPntHfSize)){MoveStat.eDnType = dtCB   ; }
            //else if (CheckRectIn  (dLeft  , dCy     , dX , dY , dRectPntHfSize)){MoveStat.eDnType = dtLC   ; }
            //else if (CheckRectIn  (dRight , dCy     , dX , dY , dRectPntHfSize)){MoveStat.eDnType = dtRC   ; }
            //else
            if (CheckLineIn  (m_aPoints , m_iPointCount , dX , dY , dPntHfSize)) MoveStat.eDnType =MoveStat.eLastDnType= dtMV   ;
            else                                                                 MoveStat.eDnType =MoveStat.eLastDnType= dtNone ;
        }
    }

    m_bSelected = MoveStat.eDnType != dtNone ;

    if(m_fpOnPaint) m_fpOnPaint() ;

    return m_bSelected ;
}



bool CTracker::TrackerMv(TShiftState Shift , int x, int y , double _dScaleX, double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    if (!FVisible || !FEnabled ) return false;
    if (MoveStat.bRight) return false ;

    double dX = _dOffsetX + x / _dScaleX ;
    double dY = _dOffsetY + y / _dScaleY ;

    TDRect DRect = GetDRect() ;
    TRect  Rect  = GetRect () ;

    double dLeft   = DRect.left    ;
    double dTop    = DRect.top     ;
    double dRight  = DRect.right   ;
    double dBottom = DRect.bottom  ;
    double dWidth  = DRect.Width ();
    double dHeight = DRect.Height();
    double dCntX   = (dLeft + dRight )/2.0;
    double dCntY   = (dTop  + dBottom)/2.0;

    double dPntHfSize     = FPointSize     / 2.0 ;
    double dRectPntHfSize = FRectPointSize / 2.0 ;


    m_bPreInRange = m_bInRange ;
    m_bInRange = false ;
    if (MoveStat.eDnType == dtNone){
        if(FTrackerType == ttCircle){
            m_bInRange  |= CheckCircleIn(m_aPoints[piCircleCnt].x , m_aPoints[piCircleCnt].y , dX , dY , dWidth/2.0+dRectPntHfSize)&&
                          !CheckCircleIn(m_aPoints[piCircleCnt].x , m_aPoints[piCircleCnt].y , dX , dY , dWidth/2.0-dRectPntHfSize);
            return m_bPreInRange != m_bInRange ;
        }
        else {
            m_bInRange |= CheckLineIn  (m_aPoints , m_iPointCount , dX , dY , dPntHfSize);
            return m_bPreInRange != m_bInRange ;
        }
        return false;
    }
    
    m_bMoved = true ;

    if(FBandEnabled && m_bSelected && MoveStat.eDnType== dtBand) {
        double dMoveX = dX - MoveStat.dX ;
        double dMoveY = dY - MoveStat.dY ;
        if(FTrackerType == ttCircle){
            if(FBandWidth + dMoveY > 1) {
                FBandWidth += dMoveY ;

                MoveStat.dX = dX ;
                MoveStat.dY = dY ;
                if(m_fpOnPaint) m_fpOnPaint() ;
                return true ;
            }
        }
        else {
            TDPoint aBandPnt[4] ;
            GetLineBandPnts(m_aPoints[0] , m_aPoints[1] , FBandWidth , aBandPnt);
            double dRectX , dRectY ;
            double dAngle = CMath::GetLineAngle(aBandPnt[2].x , aBandPnt[2].y , aBandPnt[3].x , aBandPnt[3].y);
            dRectX = (aBandPnt[2].x + aBandPnt[3].x) / 2.0 ;
            dRectY = (aBandPnt[2].y + aBandPnt[3].y) / 2.0 ;
            double dMoveX     = MoveStat.dX - dX ;
            double dMoveY     = MoveStat.dY - dY ;
            double dMove      = sqrt((dMoveX * dMoveX)+(dMoveY * dMoveY));
            double dMoveAngle = CMath::GetLineAngle(MoveStat.dX , MoveStat.dY , dX , dY);
            double dAngleGap  = (dAngle + 270 - dMoveAngle) ; //변의 수선의 각도 에서 마우스 포인트 이동한 각간의 차이나는 각도를 구함.
            dAngleGap = dAngleGap-((int)dAngleGap/360)*360 ; //변의 수선의 각도 에서 마우스 포인트 이동한 각간의 차이나는 각도를 구함.
            double dRotRad=(double)(dAngleGap*MATH_PI/180.0);//angle->radian으로 변환
            double dSinRad=(double)sin(dRotRad);
            double dCosRad=(double)cos(dRotRad);

            double dBandMove  = -dCosRad * dMove ;
            if(FBandWidth + dBandMove > 1) {
                FBandWidth += dBandMove ;
                MoveStat.dX = dX ;
                MoveStat.dY = dY ;
                if(m_fpOnPaint) m_fpOnPaint();
                return true ;
            }
        }
    }


    if(FRoiEnabled && MoveStat.eDnType== dtRoi) {
        TDRect DRoiRect = GetRoiDRect();
        double dMoveY = MoveStat.dY - dY ;

        if(FRoiWidth + dMoveY > 1) {
            FRoiWidth += dMoveY ;

            MoveStat.dX = dX ;
            MoveStat.dY = dY ;
        }
        if(m_fpOnPaint) m_fpOnPaint() ;
        return true ;
    }


    //Band
    //double fBtmRectCntX = GetBTRect().Left + GetBTRect().Width()/2 ;
    //double fBtmRectTopY = GetBTRect().Top ;
    //double fOriBCntX    = fBtmRectCntX-PointSize * 2  ;
    //double fOriBCntY    = fBtmRectTopY                ;
    //double fBCntY       = 0 ;
    const int iSM = 5 ; //최소크기마진.
    if(FTrackerType == ttRect){
        //렉트의 포인트 눌렸을때==================================================
        if(MoveStat.eDnType == dtLT) {
            if(dX >= DRect.right-iSM) dX = DRect.right-iSM  ; if(dY >= DRect.bottom-iSM) dY = DRect.bottom-iSM ;
            if(dX <  0              ) dX = 0                ; if(dY <  0               ) dY = 0                ;

            m_aPoints[piRectLT].x = dX ; m_aPoints[piRectLT].y = dY ;

            m_aPoints[piRectLB].x = dX ;
            m_aPoints[piRectRT].y = dY ;
        }
        else if(MoveStat.eDnType == dtRT) {
            if(dX <  DRect.left+iSM) dX = DRect.left+iSM  ; if(dY >= DRect.bottom-iSM) dY = DRect.bottom-iSM ;
                                                            if(dY <  0               ) dY = 0              ;

            m_aPoints[piRectRT].x = dX ; m_aPoints[piRectRT].y = dY ;

            m_aPoints[piRectRB].x = dX ;
            m_aPoints[piRectLT].y = dY ;
        }
        else if(MoveStat.eDnType == dtLB) {
            if(dX >= DRect.right-iSM) dX = DRect.right-iSM ;
            if(dX <  0              ) dX = 0               ;

            if(dY <  DRect.top+iSM  ) dY = DRect.top+iSM   ;

            m_aPoints[piRectLB].x = dX ; m_aPoints[piRectLB].y = dY ;

            m_aPoints[piRectLT].x = dX ;
            m_aPoints[piRectRB].y = dY ;
        }
        else if(MoveStat.eDnType == dtRB) {
            if(dX <  DRect.left+iSM) dX = DRect.left+iSM   ;
            if(dY <  DRect.top +iSM) dY = DRect.top +iSM   ;

            m_aPoints[piRectRB].x = dX ; m_aPoints[piRectRB].y = dY ;

            m_aPoints[piRectRT].x = dX ;
            m_aPoints[piRectLB].y = dY ;
        }

        //렉트 의 렉트포인트 눌렀을때===========================================
        else if(MoveStat.eDnType == dtCT) {
            if(dY >  DRect.bottom + iSM) dY = DRect.bottom + iSM;
            if(dY <  0                 ) dY = 0                 ;

            m_aPoints[piRectLT].y = dY ;
            m_aPoints[piRectRT].y = dY ;
        }
        else if(MoveStat.eDnType == dtCB) {
            if(dY <  DRect.top+iSM) dY = DRect.top + iSM ;

            m_aPoints[piRectLB].y = dY ;
            m_aPoints[piRectRB].y = dY ;
        }
        else if(MoveStat.eDnType == dtLC) {
            if(dX >  DRect.right - iSM) dX = DRect.right - iSM ;
            if(dX <  0                ) dX = 0                 ;

            m_aPoints[piRectLT].x = dX ;
            m_aPoints[piRectLB].x = dX ;
        }
        else if(MoveStat.eDnType == dtRC) {
            if(dX <   DRect.left + iSM ) dX = DRect.left + iSM  ;

            m_aPoints[piRectRT].x = dX ;
            m_aPoints[piRectRB].x = dX ;
        }
        else if(MoveStat.eDnType == dtMV) {
            double dMoveX = dX - MoveStat.dX ;
            double dMoveY = dY - MoveStat.dY ;

            m_aPoints[piRectLT].x += dMoveX ; m_aPoints[piRectLT].y += dMoveY ;
            m_aPoints[piRectRT].x += dMoveX ; m_aPoints[piRectRT].y += dMoveY ;
            m_aPoints[piRectRB].x += dMoveX ; m_aPoints[piRectRB].y += dMoveY ;
            m_aPoints[piRectLB].x += dMoveX ; m_aPoints[piRectLB].y += dMoveY ;

            MoveStat.dX = dX ;
            MoveStat.dY = dY ;
        }
    }
    else if(FTrackerType == ttLine){
        if(dX < 0 ) dX = 0 ;
        if(dY < 0 ) dY = 0 ;
        if(MoveStat.eDnType == dtStt) {
            m_aPoints[piLineStt].x = dX ;
            m_aPoints[piLineStt].y = dY ;
        }
        else if(MoveStat.eDnType == dtEnd) {
            m_aPoints[piLineEnd].x = dX ;
            m_aPoints[piLineEnd].y = dY ;
        }
        else if(MoveStat.eDnType == dtMV) {
            double dMoveX = dX - MoveStat.dX ;
            double dMoveY = dY - MoveStat.dY ;

            m_aPoints[piLineStt].x += dMoveX ; m_aPoints[piLineStt].y += dMoveY ;
            m_aPoints[piLineEnd].x += dMoveX ; m_aPoints[piLineEnd].y += dMoveY ;

            MoveStat.dX = dX ;
            MoveStat.dY = dY ;
        }
    }
    else if(FTrackerType == ttCircle){
        //렉트 의 렉트포인트 눌렀을때===========================================
        if(MoveStat.eDnType == dtCT) {
            if(dY >  DRect.bottom - iSM) dY = DRect.bottom - iSM;
            if(dY <  0                 ) dY = 0                 ;

            m_aPoints[piCircleCnt].y = (dY + dBottom)/2.0 ;
            FCircleRadian = (dBottom - dY) / 2.0 ;
        }
        else if(MoveStat.eDnType == dtCB) {
            if(dY <  DRect.top + iSM  ) dY = DRect.top + iSM   ;

            m_aPoints[piCircleCnt].y = (dY + dTop  )/2.0 ;
            FCircleRadian = (dY - dTop) / 2.0 ;
        }
        else if(MoveStat.eDnType == dtLC) {
            if(dX >= DRect.right - iSM) dX = DRect.right - iSM ;
            if(dX <  0                ) dX = 0                 ;

            m_aPoints[piCircleCnt].x = (dX + dRight)/2.0 ;
            FCircleRadian = (dRight - dX) / 2.0 ;
        }
        else if(MoveStat.eDnType == dtRC) {
            if(dX <  DRect.left + iSM ) dX = DRect.left + iSM  ;

            m_aPoints[piCircleCnt].x = (dX + dLeft)/2.0 ;
            FCircleRadian = (dX - dLeft ) / 2.0 ;
        }
        else if(MoveStat.eDnType == dtMV) {
            double dMoveX = dX - MoveStat.dX ;
            double dMoveY = dY - MoveStat.dY ;

            m_aPoints[piCircleCnt].x += dMoveX ; m_aPoints[piCircleCnt].y += dMoveY ;

            MoveStat.dX = dX ;
            MoveStat.dY = dY ;
        }
    }
    else if(FTrackerType == ttPolygon || FTrackerType == ttPolyline){
        //Rect Point Click
        //렉트의 포인트 눌렸을때==================================================
        if(MoveStat.eDnType >= 0) {
            m_aPoints[(EPointId)MoveStat.eDnType].x = dX ; //아... 형이 안맞게 되네... 짜증.. 나중에 고치자.
            m_aPoints[(EPointId)MoveStat.eDnType].y = dY ; //다운타입에 그냥 포인트 다운 만들고 EPointId를 TMoveStat에 추가해야함.
        }
        else if(MoveStat.eDnType == dtMV) {
            double dMoveX = dX - MoveStat.dX ;
            double dMoveY = dY - MoveStat.dY ;
            for(int i = 0 ; i < m_iPointCount ; i++){
                m_aPoints[i].x += dMoveX ;
                m_aPoints[i].y += dMoveY ;
            }
            MoveStat.dX = dX ;
            MoveStat.dY = dY ;

        }
    }

    if(m_fpOnPaint) m_fpOnPaint() ;

    return true ;
}

bool CTracker::TrackerUp(TMouseButton Button, TShiftState Shift  )
{
    bool bRet = (MoveStat.eDnType != dtNone && MoveStat.bRight);
    MoveStat.eDnType = dtNone ;
    return bRet ;
}

void CTracker::Paint(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    if(!FVisible) return ;

    CGdi * Gdi = new CGdi(_hDc , _dScaleX , _dScaleY , -_dOffsetX , -_dOffsetY); //오프셑 화면에서 보낼때 마이너스 값으로 들어오게 수정해야 한다.

    //화면 위치에 따른 오프셑 빼고 그림그릴 사각형.
    TDRect DRect    = GetDRect() ;
    TRect  Rect     = GetRect () ;


    double dPntHfSize     = FPointSize     / 2.0 ;
    double dRectPntHfSize = FRectPointSize / 2.0 ;


    double dLeft   = DRect.left  ; //* (_dScaleX ? _dScaleX : 1.0) ;
    double dTop    = DRect.top   ; //* (_dScaleY ? _dScaleY : 1.0) ;
    double dRight  = DRect.right ; //* (_dScaleX ? _dScaleX : 1.0) ;
    double dBottom = DRect.bottom; //* (_dScaleY ? _dScaleY : 1.0) ;
    double dCx      = dLeft + (dRight -dLeft)/2.0;
    double dCy      = dTop  + (dBottom-dTop )/2.0;

    /* //디버깅할때 포인트 그려 보는 것.
    Gdi -> m_tPen.Style = psDot ;
    Gdi -> m_tPen.Color = clRed  ;
    Gdi -> m_tPen.Width = 1  ;
    for(int i = 0 ; i < m_iPointCount ; i++) {
        Gdi -> Line(m_aPoints[i].x,m_aPoints[i].y,m_aPoints[(i+1)%m_iPointCount].x,m_aPoints[(i+1)%m_iPointCount].y);
    }
    */

    if(FBandEnabled){
        if(FEnabled) {
            Gdi -> m_tPen.Style = psDot ;
            Gdi -> m_tPen.Color = clGray ;//FEnabled ? LineColor  : clGray ;
            Gdi -> m_tPen.Width = LineWidth ;
            if(FTrackerType == ttCircle){
                Gdi -> Circle(false , dLeft - FBandWidth ,  dTop - FBandWidth, dRight + FBandWidth, dBottom + FBandWidth , 0);
                Gdi -> Circle(false , dLeft + FBandWidth ,  dTop + FBandWidth, dRight - FBandWidth, dBottom - FBandWidth , 0);
            }
            else {
                TDPoint aBandPnt[4] ;
                for(int i = 0 ; i < m_iPointCount ; i++) {
                    if(FTrackerType == ttPolyline && i >= m_iPointCount - 1) continue ;

                    GetLineBandPnts(m_aPoints[i] , m_aPoints[(i+1)%m_iPointCount] , FBandWidth , aBandPnt);

                    Gdi -> Line(aBandPnt[0].x , aBandPnt[0].y , aBandPnt[1].x , aBandPnt[1].y);
                    Gdi -> Line(aBandPnt[1].x , aBandPnt[1].y , aBandPnt[2].x , aBandPnt[2].y);
                    Gdi -> Line(aBandPnt[2].x , aBandPnt[2].y , aBandPnt[3].x , aBandPnt[3].y);
                    Gdi -> Line(aBandPnt[3].x , aBandPnt[3].y , aBandPnt[0].x , aBandPnt[0].y);
                }
            }
        }
    }

    TDRect DRoiRect = GetRoiDRect() ;
    if(FRoiEnabled){
        if(FEnabled) {
            Gdi -> m_tPen.Style = psDot    ;
            Gdi -> m_tPen.Color = clSilver ;
            Gdi -> m_tPen.Width = 1  ;
            Gdi -> Rect(false, DRoiRect.left , DRoiRect.top , DRoiRect.right , DRoiRect.bottom , 0 );

        }
    }



    //각 타입별 그림.
    if(FTrackerType == ttRect) {
        Gdi -> m_tPen.Style = psSolid ;
        Gdi -> m_tPen.Color = FLineColor ;
        Gdi -> m_tPen.Width = m_bInRange ? FLineWidth+1 : FLineWidth ;
        Gdi -> Rect(false , DRect.left , DRect.top , DRect.right , DRect.bottom) ;
        Gdi -> m_tPen.Width = FLineWidth ;

        if(m_bSelected) {
            Gdi -> m_tPen  .Color = FPointColor;
            Gdi -> m_tBrush.Color = FPointColor;

            Gdi -> Circle(true, dLeft , dTop   , dPntHfSize, 0);
            Gdi -> Circle(true, dRight, dTop   , dPntHfSize, 0);
            Gdi -> Circle(true, dRight, dBottom, dPntHfSize, 0);
            Gdi -> Circle(true, dLeft , dBottom, dPntHfSize, 0);

            Gdi -> m_tPen  .Color = FRectPointColor;
            Gdi -> m_tBrush.Color = FRectPointColor;
            Gdi -> Rect(true,    dCx-dRectPntHfSize, dTop    -dRectPntHfSize, dCx    +dRectPntHfSize, dTop    +dRectPntHfSize);
            Gdi -> Rect(true, dRight-dRectPntHfSize, dCy     -dRectPntHfSize, dRight +dRectPntHfSize, dCy     +dRectPntHfSize);
            Gdi -> Rect(true,    dCx-dRectPntHfSize, dBottom -dRectPntHfSize, dCx    +dRectPntHfSize, dBottom +dRectPntHfSize);
            Gdi -> Rect(true, dLeft -dRectPntHfSize, dCy     -dRectPntHfSize, dLeft  +dRectPntHfSize, dCy     +dRectPntHfSize);


            char str[64];
            Gdi -> m_tFont.Height = 15 ;
            Gdi -> m_tText.Color = clFuchsia ;
            sprintf(str, Caption.c_str());                                 Gdi -> Text(dLeft+10, dTop-20   , str);

            sprintf(str, "Start X, Y : %.1f, %.1f" , dLeft , dTop       ); Gdi -> Text(dLeft+10, dBottom+5 , str);
            sprintf(str, "End X, Y : %.1f, %.1f"   , dRight , dBottom   ); Gdi -> Text(dLeft+10, dBottom+15, str);
            sprintf(str, "Center X, Y : %.1f, %.1f", dCx, dCy           ); Gdi -> Text(dLeft+10, dBottom+25, str);
            sprintf(str, "Width : %.1f"            , DRect.Width ()     ); Gdi -> Text(dLeft+10, dBottom+35, str);
            sprintf(str, "Height : %.1f"           , DRect.Height()     ); Gdi -> Text(dLeft+10, dBottom+45, str);
        }
    }
    else if(FTrackerType == ttLine){


        //Display Position.
        double dStartX        = m_aPoints[piLineStt].x ;
        double dStartY        = m_aPoints[piLineStt].y ;
        double dEndX          = m_aPoints[piLineEnd].x ;
        double dEndY          = m_aPoints[piLineEnd].y ;
        double dTextPntX      = dStartY < dEndY ? dEndX   : dStartX ;  //FStartY < FEndY ? iScEndX : iScStartX ;
        double dTextPntY      = dStartY < dEndY ? dEndY   : dStartY ;  //FStartY < FEndY ? iScEndY : iScStartY ;
        double dCaptPntX      = dStartY < dEndY ? dStartX : dEndX   ;  //FStartY < FEndY ? iScStartX : iScEndX ;
        double dCaptPntY      = dStartY < dEndY ? dStartY : dEndY   ;  //FStartY < FEndY ? iScStartY : iScEndY ;

        if(m_bSelected) {
            Gdi -> m_tPen.Style   = psSolid ;
            Gdi -> m_tPen  .Color = PointColor;
            Gdi -> m_tBrush.Color = PointColor;
            Gdi -> Circle(true , dEndX , dEndY , dPntHfSize , 0);

            Gdi -> m_tPen.Color   = FLineColor ; //FEnabled ? LineColor  : clGray ;
            Gdi -> m_tBrush.Color = FLineColor; //FEnabled ? PointColor : clGray ;
            Gdi -> m_tPen.Width   = m_bInRange ? FLineWidth+1 : FLineWidth ;
            Gdi -> LineAllow(dStartX  , dStartY  , dEndX  , dEndY );
            Gdi -> m_tPen.Width = FLineWidth ;

            Gdi -> m_tPen  .Color = PointColor;
            Gdi -> m_tBrush.Color = PointColor;
            Gdi -> Circle(true , dStartX , dStartY , dPntHfSize , 0);

            char str[64];
            Gdi -> m_tFont.Height = 15 ;
            Gdi -> m_tText.Color = clFuchsia ;
            sprintf(str, Caption.c_str());                                                                 Gdi -> Text(dCaptPntX+10, dCaptPntY-20, str);

            sprintf(str, "Start X, Y : %.1f, %.1f" , dStartX , dStartY                                   );Gdi -> Text(dTextPntX+10, dTextPntY+5 , str);
            sprintf(str, "End X, Y : %.1f, %.1f"   , dEndX   , dEndY                                     );Gdi -> Text(dTextPntX+10, dTextPntY+15, str);
            sprintf(str, "Angle : %0.2f"           , CMath::GetLineAngle(dStartX , dStartY , dEndX , dEndY));Gdi -> Text(dTextPntX+10, dTextPntY+25, str);
            sprintf(str, "Length : %0.2f"          , CMath::GetLineLenth(dStartX , dStartY , dEndX , dEndY));Gdi -> Text(dTextPntX+10, dTextPntY+35, str);

        }
        else {
            Gdi -> m_tPen.Style   = psSolid ;
            Gdi -> m_tPen.Color   = FLineColor ; //FEnabled ? LineColor  : clGray ;
            Gdi -> m_tBrush.Color = FLineColor; //FEnabled ? PointColor : clGray ;
            Gdi -> m_tPen.Width   = FLineWidth ;
            Gdi -> LineAllow(dStartX , dStartY , dEndX , dEndY );
        }
    }
    else if(FTrackerType == ttCircle){
        //Display Position.

        int    iCx = dCx ;
        int    iCy = dCy ;
        /*
        double dOutLeft   = (dCx - FCircleRadian - FBand - _fOffsetX) * _fScaleX ;
        double dOutTop    = (dCy - FCircleRadian - FBand - _fOffsetY) * _fScaleY ;
        double dOutRight  = (dCx + FCircleRadian + FBand - _fOffsetX) * _fScaleX ;
        double dOutBottom = (dCy + FCircleRadian + FBand - _fOffsetY) * _fScaleY ;

        double dInLeft    = (dCx - FCircleRadian + FBand - _fOffsetX) * _fScaleX ;
        double dInTop     = (dCy - FCircleRadian + FBand - _fOffsetY) * _fScaleY ;
        double dInRight   = (dCx + FCircleRadian - FBand - _fOffsetX) * _fScaleX ;
        double dInBottom  = (dCy + FCircleRadian - FBand - _fOffsetY) * _fScaleY ;
        */

        Gdi -> m_tPen.Style = psSolid ;
        Gdi -> m_tPen.Color = LineColor ;//FEnabled ? LineColor  : clGray ;
        Gdi -> m_tPen.Width = m_bInRange ? FLineWidth+1 : FLineWidth ;
        Gdi -> Circle(false , iCx , iCy , FCircleRadian , m_bSelected ? 5 : 0);
        Gdi -> m_tPen.Width = FLineWidth ;



        if(m_bSelected) {
            Gdi -> m_tPen  .Color = PointColor;
            Gdi -> m_tBrush.Color = PointColor;
            Gdi -> Rect  (true , dLeft -dRectPntHfSize , dCy      - dRectPntHfSize , dLeft  + dRectPntHfSize , dCy      + dRectPntHfSize);
            Gdi -> Rect  (true , dCx   -dRectPntHfSize , dTop     - dRectPntHfSize , dCx    + dRectPntHfSize , dTop     + dRectPntHfSize);
            Gdi -> Rect  (true , dRight-dRectPntHfSize , dCy      - dRectPntHfSize , dRight + dRectPntHfSize , dCy      + dRectPntHfSize);
            Gdi -> Rect  (true , dCx   -dRectPntHfSize , dBottom  - dRectPntHfSize , dCx    + dRectPntHfSize , dBottom  + dRectPntHfSize);
            char str[64];
            Gdi -> m_tText.Color = clFuchsia ;
            Gdi -> m_tFont.Height = 15 ;
            sprintf(str, Caption.c_str()  );
            Gdi -> Text(dLeft+10, dTop-20, str);
            sprintf(str, "Center X, Y : %.2f, %.2f" , dCx , dCy    ); Gdi -> Text(dLeft+10, dBottom +5 , str);
            sprintf(str, "Radius : %.2f"            , FCircleRadian); Gdi -> Text(dLeft+10, dBottom +15, str);


        }
    }
    else if(FTrackerType == ttPolygon || FTrackerType == ttPolyline){
        if(m_bSelected) {
            Gdi -> m_tPen.Style = psDot ;
            Gdi -> m_tPen.Color = clGray ;
            Gdi -> m_tPen.Width = 1  ;
            Gdi -> Rect(false , DRect.left , DRect.top , DRect.right , DRect.bottom) ;

            Gdi -> m_tPen  .Color = FPointColor;
            Gdi -> m_tBrush.Color = FPointColor;

            //이건 나중에 하자...
            //Gdi -> Rect(true, dLeft-dRectPntHfSize, dTop -dRectPntHfSize, dLeft+dRectPntHfSize, dTop +dRectPntHfSize);
            //Gdi -> Rect(true, dCx-dRectPntHfSize, dTop -dRectPntHfSize, dCx+dRectPntHfSize, dTop +dRectPntHfSize);
            //Gdi -> Rect(true, dRight-dRectPntHfSize, dTop -dRectPntHfSize, dRight+dRectPntHfSize, dTop +dRectPntHfSize);
            //Gdi -> Rect(true, dRight-dRectPntHfSize, dCy-dRectPntHfSize, dRight+dRectPntHfSize, dCy+dRectPntHfSize);
            //Gdi -> Rect(true, dRight-dRectPntHfSize, dBottom-dRectPntHfSize, dRight+dRectPntHfSize, dBottom+dRectPntHfSize);
            //Gdi -> Rect(true, dCx-dRectPntHfSize, dBottom-dRectPntHfSize, dCx+dRectPntHfSize, dBottom+dRectPntHfSize);
            //Gdi -> Rect(true, dLeft-dRectPntHfSize, dBottom-dRectPntHfSize, dLeft+dRectPntHfSize, dBottom+dRectPntHfSize);
            //Gdi -> Rect(true, dLeft-dRectPntHfSize, dCy-dRectPntHfSize, dLeft+dRectPntHfSize, dCy+dRectPntHfSize);

            char str[64];
            Gdi -> m_tFont.Height = 15 ;
            Gdi -> m_tText.Color = clFuchsia ;
            Gdi -> m_tFont.Height = 15 ;
            sprintf(str, Caption.c_str());                               Gdi -> Text(dLeft+10, dTop-20   , str);
            sprintf(str, "Start X, Y : %.1f, %.1f" , dLeft , dTop     ); Gdi -> Text(dLeft+10, dBottom+5 , str);
            sprintf(str, "End X, Y : %.1f, %.1f"   , dRight , dBottom ); Gdi -> Text(dLeft+10, dBottom+15, str);
            sprintf(str, "Center X, Y : %.1f, %.1f", dCx, dCy         ); Gdi -> Text(dLeft+10, dBottom+25, str);
            sprintf(str, "Width : %.1f"            , DRect.Width ()   ); Gdi -> Text(dLeft+10, dBottom+35, str);
            sprintf(str, "Height : %.1f"           , DRect.Height()   ); Gdi -> Text(dLeft+10, dBottom+45, str);
        }

        Gdi -> m_tPen.Style = psSolid ;
        Gdi -> m_tPen.Color = LineColor ;//FEnabled ? LineColor  : clGray ;
        Gdi -> m_tPen.Width = m_bInRange ? FLineWidth+1 : FLineWidth ;
        for(int i = 0 ; i < m_iPointCount ; i++) {
            if(FTrackerType == ttPolyline && i >= m_iPointCount - 1) continue ;
            Gdi -> Line(m_aPoints[i].x , m_aPoints[i].y , m_aPoints[(i+1)%m_iPointCount].x , m_aPoints[(i+1)%m_iPointCount].y);
        }
        Gdi -> m_tPen.Width = FLineWidth ;

        Gdi -> m_tPen  .Color = FRectPointColor;
        Gdi -> m_tBrush.Color = FRectPointColor;
        for(int i = 0 ; i < m_iPointCount ; i++) {

            if(m_bSelected&& FEnabled)Gdi -> Circle(true , m_aPoints[i].x , m_aPoints[i].y , dPntHfSize ,0);
        }

    }

    //위쪽 렉트 와 겹쳐서 맨뒤에 그림.
    if(FRoiEnabled){
        if(m_bSelected) {
            Gdi -> m_tPen  .Color = clFuchsia;
            Gdi -> m_tBrush.Color = clYellow ;

            double dRoiRectCntX =(DRoiRect.left + DRoiRect.right)  /2.0 ;
            double dRoiRectTopY = DRoiRect.top ;

            Gdi -> Rect(true, dRoiRectCntX - dRectPntHfSize , dRoiRectTopY - dRectPntHfSize,
                              dRoiRectCntX + dRectPntHfSize , dRoiRectTopY + dRectPntHfSize);
        }
    }

    if(FBandEnabled){
        if(m_bSelected) {

            Gdi -> m_tPen  .Style = psSolid  ;
            Gdi -> m_tPen  .Color = clFuchsia;
            Gdi -> m_tBrush.Color = clPurple ;

            if(FTrackerType == ttCircle){
                Gdi -> Rect(true, dCx - dRectPntHfSize , dTop + FBandWidth - dRectPntHfSize,
                                  dCx + dRectPntHfSize , dTop + FBandWidth + dRectPntHfSize);
            }
            else {
                TDPoint aBandPnt[4] ;
                GetLineBandPnts(m_aPoints[0] , m_aPoints[1] , FBandWidth , aBandPnt);

                double dRectX , dRectY ;
                dRectX = (aBandPnt[2].x + aBandPnt[3].x) / 2.0 ;
                dRectY = (aBandPnt[2].y + aBandPnt[3].y) / 2.0 ;

                Gdi -> Rect(true, dRectX - dRectPntHfSize , dRectY - dRectPntHfSize,
                                  dRectX + dRectPntHfSize , dRectY + dRectPntHfSize);
            }
        }
    }

    delete Gdi;
}

//Load Save.
void CTracker::LoadSave(bool _bLoad,AnsiString _sFilePath,AnsiString _sCaption)
{
    TUserINI UserINI ;

    //타입 로딩 안되는 곳부터 하면됌....
    // 그거 하고 디펙트에서 카피 지우기 하면됌.



    //Load
    int iTemp ;

    if(_bLoad) {
        //UserINI.Load(_sFilePath, _sCaption , "FTag            ", FTag           );
        //UserINI.Load(_sFilePath, _sCaption , "FVisible        ", FVisible       );
        //UserINI.Load(_sFilePath, _sCaption , "FEnabled        ", FEnabled       );
        //UserINI.Load(_sFilePath, _sCaption , "FBandEnabled    ", FBandEnabled   );
        //UserINI.Load(_sFilePath, _sCaption , "FRoiEnabled     ", FRoiEnabled    );
           UserINI.Load(_sFilePath, _sCaption , "FTrackerType    ", iTemp            ); if(iTemp != ttNone) FTrackerType = iTemp; //처음 생성하고 로딩하면서 타입이 바뀌는 문제땜에.
        //UserINI.Load(_sFilePath, _sCaption , "FBandWidth      ", FBandWidth     );
        //UserINI.Load(_sFilePath, _sCaption , "FRoiWidth       ", FRoiWidth      );
        //UserINI.Load(_sFilePath, _sCaption , "FLineColor      ", FLineColor     );
        //UserINI.Load(_sFilePath, _sCaption , "FRectPointColor ", FRectPointColor);
        //UserINI.Load(_sFilePath, _sCaption , "FPointColor     ", FPointColor    );
        //UserINI.Load(_sFilePath, _sCaption , "FLineWidth      ", FLineWidth     );
        //UserINI.Load(_sFilePath, _sCaption , "FPointSize      ", FPointSize     );
        //UserINI.Load(_sFilePath, _sCaption , "FRectPointSize  ", FRectPointSize );
        //UserINI.Load(_sFilePath, _sCaption , "FCaption        ", FCaption       );

        UserINI.Load(_sFilePath, _sCaption , "FBandWidth      ", FBandWidth     ); if(FBandWidth    <= 1) FBandWidth    = 1 ;
        UserINI.Load(_sFilePath, _sCaption , "FCircleRadian   ", FCircleRadian  ); if(FCircleRadian <= 1) FCircleRadian = 1 ;
        UserINI.Load(_sFilePath, _sCaption , "FRoiWidth       ", FRoiWidth      ); if(FRoiWidth     <= 1) FRoiWidth     = 1 ;
        UserINI.Load(_sFilePath, _sCaption , "m_iPointCount   ", m_iPointCount  );
        if(m_iPointCount==0){
                 if(FTrackerType == ttCircle) {m_iPointCount = 1 ; }
            else if(FTrackerType == ttLine  ) {m_iPointCount = 2 ; }
            else                              {m_iPointCount = 4 ; }
        }

        delete [] m_aPoints ;
        m_aPoints = new TDPoint[m_iPointCount];
        bool bNew = true ;
        for(int i = 0 ; i < m_iPointCount ; i++) {
            UserINI.Load(_sFilePath, _sCaption , "m_aPoints["+String(i)+"].x", m_aPoints[i].x );
            UserINI.Load(_sFilePath, _sCaption , "m_aPoints["+String(i)+"].y", m_aPoints[i].y );
            if(m_aPoints[i].x && m_aPoints[i].y)bNew = false ;
        }

        if(bNew){
            if(FTrackerType == ttCircle && FCircleRadian == 0) {
                m_aPoints[piCircleCnt].x = 50 ;
                m_aPoints[piCircleCnt].y = 50 ;
                FCircleRadian = 50 ;
            }
            else if(FTrackerType == ttLine) {
                m_aPoints[piLineStt].x = 10 ;
                m_aPoints[piLineStt].y = 10 ;

                m_aPoints[piLineEnd].x = 90 ;
                m_aPoints[piLineEnd].y = 90 ;
            }
            else {
                m_aPoints[piRectLT].x = 10 ;  m_aPoints[piRectRT].x = 90 ;
                m_aPoints[piRectLT].y = 10 ;  m_aPoints[piRectRT].y = 10 ;

                m_aPoints[piRectLB].x = 10 ;  m_aPoints[piRectRB].x = 90 ;
                m_aPoints[piRectLB].y = 90 ;  m_aPoints[piRectRB].y = 90 ;
            }

        }



    }
    else {
        //UserINI.Save(_sFilePath, _sCaption , "FTag            ", FTag           );
        //UserINI.Save(_sFilePath, _sCaption , "FVisible        ", FVisible       );
        //UserINI.Save(_sFilePath, _sCaption , "FEnabled        ", FEnabled       );
        //UserINI.Save(_sFilePath, _sCaption , "FBandEnabled    ", FBandEnabled   );
        //UserINI.Save(_sFilePath, _sCaption , "FRoiEnabled     ", FRoiEnabled    );
        UserINI.Save(_sFilePath, _sCaption , "FTrackerType    ", FTrackerType   );
        //
        //
        //
        //UserINI.Save(_sFilePath, _sCaption , "FLineColor      ", FLineColor     );
        //UserINI.Save(_sFilePath, _sCaption , "FRectPointColor ", FRectPointColor);
        //UserINI.Save(_sFilePath, _sCaption , "FPointColor     ", FPointColor    );
        //UserINI.Save(_sFilePath, _sCaption , "FLineWidth      ", FLineWidth     );
        //UserINI.Save(_sFilePath, _sCaption , "FPointSize      ", FPointSize     );
        //UserINI.Save(_sFilePath, _sCaption , "FRectPointSize  ", FRectPointSize );
        //UserINI.Save(_sFilePath, _sCaption , "FCaption        ", FCaption       );

        UserINI.Save(_sFilePath, _sCaption , "FBandWidth      ", FBandWidth     );
        UserINI.Save(_sFilePath, _sCaption , "FRoiWidth       ", FRoiWidth      );
        UserINI.Save(_sFilePath, _sCaption , "FCircleRadian   ", FCircleRadian  );
        UserINI.Save(_sFilePath, _sCaption , "m_iPointCount ", m_iPointCount );

        for(int i = 0 ; i < m_iPointCount ; i++) {
            UserINI.Save(_sFilePath, _sCaption , "m_aPoints["+String(i)+"].x", m_aPoints[i].x );
            UserINI.Save(_sFilePath, _sCaption , "m_aPoints["+String(i)+"].y", m_aPoints[i].y );
        }
    }

    m_bMoved = false ;

}














