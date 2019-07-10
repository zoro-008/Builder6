//---------------------------------------------------------------------------

#include <stdio.h>

#pragma hdrstop
#include "Tracker.h"

#include "UserINI.h"
#include "BaseMaths.h"
#include "UnitGdi.h"








#pragma package(smart_init)

/*
1. Overlay 영상 위에 그리실때는 Wincontrol 형태로 작업하는게 부적합합니다. 
    영상의 Render를 수행할 함수 안에서 Overlay Surface 위에 직접 그려주는 형태로 구현하면 되겠습니다만, 
    일반적인 메시징을 타시기 힘드실테고, Draw쪽과 Mouse Event등 이것 저것을 직접 다시 구현해야 될겁니다. 
    만약 카메라로 부터 얻어온 영상을 Window 위에 직접 그리시는 경우엔 상관이없겠지요. 

2. 위의 문제와 상관없이, 획득한 영상을 직접 Window Control위에 그리신다면, 
    배경 스크린 윈도우 위에 테두리나 내용을 반투명 처리하여 그려야 할 필요가 있을터이므로 
    TGraphicControl 정도로 무난합니다. 

3. 이 경우 트래커의 그립 부분과 그 외의 부분의 마우스 클릭을 판단하는 효과적인 방법은 
    원의 경우, 원의 중심으로 부터 마우스 커서가 반지름 보다 가까운지 먼지 판단하는 것이고, 
    (피타고라스의 정리) 
    혹은, 원이든 다각형이든 모두 Polygon 으로 만들어 그린 다음 폴리곤의 내부가 클릭되었는지 
    판단하는 방법이 되겠지요. 

bool __fastcall isPointInPolygon(TPoint *Points, TPoint P, int N){ 
    TPoint P1, P2; 
    bool Result = false; 

    P1 = Points[0]; 
    P2 = Points[N - 1]; 

    for(int i = 1; i <= N; i++){ 
        Result ^= (((P.y >= P1.y) ^ (P.y >= P2.y)) && (P.x - P1.x < MulDiv(P2.x - P1.x, P.y - P1.y, P2.y - P1.y))); 
        P2 = P1; 
        P1 = Points[i % N]; 
    } 
    return Result; 
} 

    이 함수를 쓰시면, TPoint의 배열(Points)과, 그 갯수(N)로 넘긴 폴리곤의 내부에 마우스 좌표(P)가 
    있는지 판단할 수 있습니다. 

4. 위와 같이 구현하였는데, 트래커를 그리는 과정에서 깜박인다. (물론 깜박이겠죠) 
    그럴 경우엔 트래커를 올려 놓은 배경스크린 컨트롤의 DoubleBuffered = true; 로 설정하시면 OK.
*/

//트렉커 클래스.
//==============================================================================
TTracker::TTracker()
{
    m_iTrackerType = ttNone;
    FOnChanged     = NULL  ;

    FVisible       = true  ;
    FEnabled       = true  ;
    FFocused       = false ;

    FLineColor     = clBlue ;
    FPointColor    = clLime ;
    FLineWidth     = 1      ;
    FPointSize     = 5      ;
    FCaption       = "Tracker" ;


    m_bMoved = false ;

}
//TTracker(TOnChanged _OnChanged);
TTracker::~TTracker()
{

}

void __fastcall TTracker::SetVisible(bool value)
{
    if(FVisible == value) return ;
    FVisible = value ;
    if(FOnChanged) FOnChanged() ;
}


void __fastcall TTracker::SetEnabled(bool value)
{
    if(FEnabled == value) return ;
    FEnabled = value ;
    if(FOnChanged) FOnChanged() ;
}

void __fastcall TTracker::SetFocused(bool value)
{
    if(FFocused == value) return ;
    FFocused = value ;
    if(FOnChanged) FOnChanged() ;
}

void __fastcall TTracker::SetLineColor(TColor value)
{
    if(FLineColor == value) return ;
    FLineColor = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TTracker::SetPointColor(TColor value)
{
    if(FPointColor == value) return ;
    FPointColor = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TTracker::SetLineWidth(int value)
{
    if(FLineWidth == value) return ;
    if(value < 1) value = 1 ;
    FLineWidth = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TTracker::SetPointSize(int value)
{
    if(FPointSize == value) return ;
    if(value < 1) value = 1 ;
    FPointSize = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TTracker::SetCaption(AnsiString value)
{
    if(FCaption == value) return ;
    FCaption = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void TTracker::OnLoadSave(bool _bLoad,AnsiString _sFilePath,AnsiString _sCaption)
{
    TUserINI UserINI ;

    //Load
    if(_bLoad) {

        UserINI.Load(_sFilePath, _sCaption , "m_iTrackerType", m_iTrackerType);
        //UserINI.Load(_sFilePath, FCaption , "FVisible      ", FVisible      );
        //UserINI.Load(_sFilePath, _sCaption , "FEnabled      ", FEnabled      );
        //UserINI.Load(_sFilePath, _sCaption , "FFocused      ", FFocused      );
        //UserINI.Load(_sFilePath, _sCaption , "FLineColor    ", FLineColor    );
        //UserINI.Load(_sFilePath, _sCaption , "FPointColor   ", FPointColor   );
        //UserINI.Load(_sFilePath, _sCaption , "FLineWidth    ", FLineWidth    );
        //UserINI.Load(_sFilePath, _sCaption , "FPointSize    ", FPointSize    );
        //UserINI.Load(_sFilePath, _sCaption , "FCaption      ", FCaption      );
    }
    else {
        UserINI.Save(_sFilePath, _sCaption , "m_iTrackerType", m_iTrackerType);
        //UserINI.Save(_sFilePath, _sCaption , "FVisible      ", FVisible      );
        //UserINI.Save(_sFilePath, _sCaption , "FEnabled      ", FEnabled      );
        //UserINI.Save(_sFilePath, _sCaption , "FFocused      ", FFocused      );
        //UserINI.Save(_sFilePath, _sCaption , "FLineColor    ", FLineColor    );
        //UserINI.Save(_sFilePath, _sCaption , "FPointColor   ", FPointColor   );
        //UserINI.Save(_sFilePath, _sCaption , "FLineWidth    ", FLineWidth    );
        //UserINI.Save(_sFilePath, _sCaption , "FPointSize    ", FPointSize    );
        //UserINI.Save(_sFilePath, _sCaption , "FCaption      ", FCaption      );
    }

    m_bMoved = false ;
}







//Rect Tracker.
//==============================================================================
TRectTracker::TRectTracker()
{
    m_iTrackerType = ttRect;
    FLeft        = 1   ;
    FWidth       = 100 ;
    FTop         = 1   ;
    FHeight      = 100 ;
    FBandVisible = false ;
    FBandEnable  = true;
    FBand        = 0   ;
    FLeftOffset  = 0   ;
    FTopOffset   = 0   ;
    FRightOffset = 0   ;
    FBottomOffset= 0   ;
    FSizeEnable  = true ;
    FCaption     = "Rect Tracker" ;

    //TTracker();
    //TTracker::
    //TTracker::TTracker();



    memset(&Stat , 0 , sizeof(TStat));
}

TRectTracker::~TRectTracker()
{
}

void __fastcall TRectTracker::SetLeft(float value)
{
    if(value < 0) return ;
    FLeft = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TRectTracker::SetTop(float value)
{
    if(value < 0) return ;
    FTop = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TRectTracker::SetHeight(float value)
{
    if(value < 1) return ;
    FHeight = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TRectTracker::SetWidth(float value)
{
    if(value < 1) return ;
    FWidth = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TRectTracker::SetBandVisible(bool value)
{
    if(FBandVisible == value) return ;
    FBandVisible = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}


void __fastcall TRectTracker::SetBand(float value)
{
    if(value < 0) return ;
    if(FBandMax != 0 && value > FBandMax) value = FBandMax ;
    FBand = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TRectTracker::SetBandMax(float value)
{
    if(value < 0) return ;
    FBandMax = value ;
    if(FBandMax != 0 && value > FBandMax) FBand = FBandMax ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}




void __fastcall TRectTracker::SetBandEnable(bool value)
{
    if(FBandEnable == value) return ;

    FBandEnable = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TRectTracker::SetLeftOffset(float value)
{
    if(value < 0) return ;
    FLeftOffset = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TRectTracker::SetTopOffset(float value)
{
    if(value < 0) return ;
    FTopOffset = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TRectTracker::SetRightOffset(float value)
{
    if(value < 0) return ;
    FRightOffset = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TRectTracker::SetBottomOffset(float value)
{
    if(value < 0) return ;
    FBottomOffset = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TRectTracker::SetSizeEnable(bool value)
{
    if(FSizeEnable == value) return ;

    FSizeEnable = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;

}







float __fastcall TRectTracker::GetRight()
{
    return FLeft + FWidth ;
}

float __fastcall TRectTracker::GetBottom()
{
    return FTop + FHeight ;
}

float __fastcall TRectTracker::GetCenterX()
{
    return FLeft + (FWidth/2) ;

}

float __fastcall TRectTracker::GetCenterY()
{
    return FTop + (FHeight/2) ;
}

void __fastcall TRectTracker::SetArea(int _iLeft , int _iTop , int _iWidth , int _iHeight)
{
    //FLeft =
    //작업 하자.ㅣ...

}

void __fastcall TRectTracker::UpdateRect()
{
    if(FLeft < 0) FLeft = 0 ;
    if(FTop  < 0) FTop  = 0 ;


    if(FWidth  < 1 ) {FWidth  = 1;}
    if(FHeight < 1 ) {FHeight = 1;}
}

TFRect __fastcall TRectTracker::GetLTFRect()
{
    TFRect Rect ;
    Rect.left   = FLeft       - FBand       ;
    Rect.right  = FLeft       + FBand       ;
    Rect.top    = FTop        + FLeftOffset ;
    Rect.bottom = GetBottom() - FLeftOffset ;

    return Rect ;

}

TFRect __fastcall TRectTracker::GetTTFRect()
{
    TFRect Rect ;
    Rect.left   = FLeft      + FTopOffset ;
    Rect.right  = GetRight() - FTopOffset ;
    Rect.top    = FTop       - FBand      ;
    Rect.bottom = FTop       + FBand      ;

    return Rect ;
}

TFRect __fastcall TRectTracker::GetRTFRect()
{
    TFRect Rect ;
    Rect.left   = GetRight()  - FBand         ;
    Rect.right  = GetRight()  + FBand         ;
    Rect.top    = FTop        + FTopOffset    ;
    Rect.bottom = GetBottom() - FBottomOffset ;

    return Rect ;

}

TFRect __fastcall TRectTracker::GetBTFRect()
{
    TFRect Rect ;
    Rect.left   = FLeft      + FBottomOffset ;
    Rect.right  = GetRight() - FBottomOffset ;
    Rect.top    = GetBottom()- FBand         ;
    Rect.bottom = GetBottom()+ FBand         ;

    return Rect ;
}

TRect __fastcall TRectTracker::GetLTRect()
{
    TRect Rect ;
    Rect.left   = FLeft       - FBand       ;
    Rect.right  = FLeft       + FBand       ;
    Rect.top    = FTop        + FLeftOffset ;
    Rect.bottom = GetBottom() - FLeftOffset ;

    return Rect ;

}

TRect __fastcall TRectTracker::GetTTRect()
{
    TRect Rect ;
    Rect.left   = FLeft      + FTopOffset ;
    Rect.right  = GetRight() - FTopOffset ;
    Rect.top    = FTop       - FBand      ;
    Rect.bottom = FTop       + FBand      ;

    return Rect ;
}

TRect __fastcall TRectTracker::GetRTRect()
{
    TRect Rect ;
    Rect.left   = GetRight()  - FBand         ;
    Rect.right  = GetRight()  + FBand         ;
    Rect.top    = FTop        + FTopOffset    ;
    Rect.bottom = GetBottom() - FBottomOffset ;

    return Rect ;

}

TRect __fastcall TRectTracker::GetBTRect()
{
    TRect Rect ;
    Rect.left   = FLeft      + FBottomOffset ;
    Rect.right  = GetRight() - FBottomOffset ;
    Rect.top    = GetBottom()- FBand         ;
    Rect.bottom = GetBottom()+ FBand         ;

    return Rect ;
}


TFPoint __fastcall TRectTracker::GetLTPnt()
{
    TFPoint Pnt ;
    Pnt.x = FLeft ;
    Pnt.y = FTop  ;

    return Pnt ;
}

TFPoint __fastcall TRectTracker::GetRTPnt()
{
    TFPoint Pnt ;
    Pnt.x = FLeft + FWidth  ;
    Pnt.y = FTop  ;

    return Pnt ;
}

TFPoint __fastcall TRectTracker::GetLBPnt()
{
    TFPoint Pnt ;
    Pnt.x = FLeft   ;
    Pnt.y = FTop + FHeight ;

    return Pnt ;

}

TFPoint __fastcall TRectTracker::GetRBPnt()
{
    TFPoint Pnt ;
    Pnt.x = FLeft + FWidth  ;
    Pnt.y = FTop + FHeight ;

    return Pnt ;

}

void __fastcall TRectTracker::Paint(HDC _Dc , float _fScaleX , float _fScaleY , float _dOffsetX , float _dOffsetY)
{
    if(!FVisible) return ;

    CGdi * Gdi = new CGdi(_Dc);

    //문제가 있어서 바꿈. 함수에서 그냥 스케일 계산한다.
    //Gdi -> SetScale(_fScaleX , _fScaleY);

    //화면 위치에 따른 오프셑 빼고 그림그릴 사각형.
    TFRect Rect  ;
    Rect.left    = (FLeft - _dOffsetX) * _fScaleX;
    Rect.top     = (FTop  - _dOffsetY) * _fScaleY;
    Rect.right   = (FLeft - _dOffsetX+ FWidth ) * _fScaleX  ;
    Rect.bottom  = (FTop  - _dOffsetY+ FHeight) * _fScaleY  ;

    TFRect RectL = GetLTFRect();
    TFRect RectT = GetTTFRect();
    TFRect RectR = GetRTFRect();
    TFRect RectB = GetBTFRect();

    RectL.left   = (RectL.left   - _dOffsetX )* _fScaleX ;
    RectL.top    = (RectL.top    - _dOffsetY )* _fScaleY ;
    RectL.right  = (RectL.right  - _dOffsetX )* _fScaleX ;
    RectL.bottom = (RectL.bottom - _dOffsetY )* _fScaleY ;

    RectT.left   = (RectT.left   - _dOffsetX )* _fScaleX ;
    RectT.top    = (RectT.top    - _dOffsetY )* _fScaleY ;
    RectT.right  = (RectT.right  - _dOffsetX )* _fScaleX ;
    RectT.bottom = (RectT.bottom - _dOffsetY )* _fScaleY ;

    RectR.left   = (RectR.left   - _dOffsetX )* _fScaleX ;
    RectR.top    = (RectR.top    - _dOffsetY )* _fScaleY ;
    RectR.right  = (RectR.right  - _dOffsetX )* _fScaleX ;
    RectR.bottom = (RectR.bottom - _dOffsetY )* _fScaleY ;

    RectB.left   = (RectB.left   - _dOffsetX )* _fScaleX ;
    RectB.top    = (RectB.top    - _dOffsetY )* _fScaleY ;
    RectB.right  = (RectB.right  - _dOffsetX )* _fScaleX ;
    RectB.bottom = (RectB.bottom - _dOffsetY )* _fScaleY ;


    if(FBandVisible && FFocused) {
        Gdi -> m_tPen.Style = psDot ;
        Gdi -> m_tPen.Color = clGray ;
        Gdi -> m_tPen.Width = 1  ;

        Gdi -> Rect(false, RectL.GetRECT() , 0);
        Gdi -> Rect(false, RectT.GetRECT() , 0);
        Gdi -> Rect(false, RectR.GetRECT() , 0);
        Gdi -> Rect(false, RectB.GetRECT() , 0);

        Gdi -> m_tPen.Style = psSolid ;

        Gdi -> m_tPen  .Color = PointColor;
        Gdi -> m_tBrush.Color = PointColor;

        float fBtmRectCntX = RectB.left + (RectB.right - RectB.left) /2.0 ;
        float fBtmRectTopY = RectB.top ;

        if(FBandEnable) Gdi -> Rect(true, fBtmRectCntX-PointSize * 3, fBtmRectTopY-PointSize, fBtmRectCntX-PointSize, fBtmRectTopY+PointSize);
    }



    Gdi -> m_tPen.Color = LineColor ;//FEnabled ? LineColor : clGray ;
    Gdi -> m_tPen.Width = LineWidth ;
    Gdi -> Rect(false, Rect.GetRECT() , 10);

    Gdi -> m_tPen  .Color = PointColor;
    Gdi -> m_tBrush.Color = PointColor;

    float x1 = Rect.left  ; //* (_fScaleX ? _fScaleX : 1.0) ;
    float y1 = Rect.top   ; //* (_fScaleY ? _fScaleY : 1.0) ;
    float x2 = Rect.right ; //* (_fScaleX ? _fScaleX : 1.0) ;
    float y2 = Rect.bottom; //* (_fScaleY ? _fScaleY : 1.0) ;
    float cx = x1+(x2-x1)*10/2/10;
    float cy = y1+(y2-y1)*10/2/10;

    char str[500];
    float rcx = x1 + (x2-x1)/2.0;
    float rcy = y1 + (y2-y1)/2.0;

    if(FFocused) {
        if(FSizeEnable) {
            Gdi -> Rect(true, x1-PointSize, y1-PointSize, x1+PointSize, y1+PointSize);
            Gdi -> Rect(true, cx-PointSize, y1-PointSize, cx+PointSize, y1+PointSize);
            Gdi -> Rect(true, x2-PointSize, y1-PointSize, x2+PointSize, y1+PointSize);
            Gdi -> Rect(true, x2-PointSize, cy-PointSize, x2+PointSize, cy+PointSize);
            Gdi -> Rect(true, x2-PointSize, y2-PointSize, x2+PointSize, y2+PointSize);
            Gdi -> Rect(true, cx-PointSize, y2-PointSize, cx+PointSize, y2+PointSize);
            Gdi -> Rect(true, x1-PointSize, y2-PointSize, x1+PointSize, y2+PointSize);
            Gdi -> Rect(true, x1-PointSize, cy-PointSize, x1+PointSize, cy+PointSize);
        }

        Gdi -> m_tText.Color = clFuchsia ;
        sprintf(str, "Start X, Y : %.1f, %.1f" , x1 , y1   );
        Gdi -> Text(x1+10, y2+5 , str);
        sprintf(str, "End X, Y : %.1f, %.1f"   , x2 , y2   );
        Gdi -> Text(x1+10, y2+15, str);
        sprintf(str, "Center X, Y : %.1f, %.1f", rcx, rcy  );
        Gdi -> Text(x1+10, y2+25, str);
        sprintf(str, "Width : %.1f"          , FWidth    );
        Gdi -> Text(x1+10, y2+35, str);
        sprintf(str, "Height : %.1f"         , FHeight   );
        Gdi -> Text(x1+10, y2+45, str);
    }

    Gdi -> m_tFont.Height = 15 ;
    sprintf(str, Caption.c_str() , x1, y1  );
    Gdi -> Text(x1+10, y1-20, str);

    delete Gdi;
}

bool __fastcall TRectTracker::TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY  , float _fOffsetX, float _fOffsetY)
{
    bool ret = false ;

    if (!FVisible || !FEnabled) return ret ;

    float fx = _fOffsetX + x / _fScaleX ;
    float fy = _fOffsetY + y / _fScaleY ;

    float fLeft   = FLeft     ;
    float fTop    = FTop      ;
    float fRight  =(FLeft + FWidth ) ;
    float fBottom =(FTop  + FHeight) ;
    float fWidth  = FWidth    ;
    float fHeight = FHeight   ;

    //Band
    TFRect subrect = GetBTFRect() ;
    float fBtmRectCntX = subrect.left + (subrect.right - subrect.left) /2 ;
    float fBtmRectTopY = subrect.top ;
    float fBCntX  = (fBtmRectCntX-PointSize * 2) * _fScaleX ;
    float fBCntY  =  fBtmRectTopY                * _fScaleY ;

    float cx = (fLeft + fWidth *10/2/10) ;
    float cy = (fTop  + fHeight*10/2/10) ;

    Stat.mx = fx ;
    Stat.my = fy ;


         if (FSizeEnable && fx > fLeft  -PointSize && fx < fLeft  +PointSize && fy > fTop   -PointSize && fy < fTop   +PointSize ) {ret = Stat.IsLTM  = true; FFocused = true ;}
    else if (FSizeEnable && fx > cx     -PointSize && fx < cx     +PointSize && fy > fTop   -PointSize && fy < fTop   +PointSize ) {ret = Stat.IsCTM  = true; FFocused = true ;}
    else if (FSizeEnable && fx > fRight -PointSize && fx < fRight +PointSize && fy > fTop   -PointSize && fy < fTop   +PointSize ) {ret = Stat.IsRTM  = true; FFocused = true ;}
    else if (FSizeEnable && fx > fRight -PointSize && fx < fRight +PointSize && fy > cy     -PointSize && fy < cy     +PointSize ) {ret = Stat.IsRCM  = true; FFocused = true ;}
    else if (FSizeEnable && fx > fRight -PointSize && fx < fRight +PointSize && fy > fBottom-PointSize && fy < fBottom+PointSize ) {ret = Stat.IsRBM  = true; FFocused = true ;}
    else if (FSizeEnable && fx > cx     -PointSize && fx < cx     +PointSize && fy > fBottom-PointSize && fy < fBottom+PointSize ) {ret = Stat.IsCBM  = true; FFocused = true ;}
    else if (FSizeEnable && fx > fLeft  -PointSize && fx < fLeft  +PointSize && fy > fBottom-PointSize && fy < fBottom+PointSize ) {ret = Stat.IsLBM  = true; FFocused = true ;}
    else if (FSizeEnable && fx > fLeft  -PointSize && fx < fLeft  +PointSize && fy > cy     -PointSize && fy < cy     +PointSize ) {ret = Stat.IsLCM  = true; FFocused = true ;}
    else if (FSizeEnable && fx > fBCntX -PointSize && fx < fBCntX +PointSize && fy > fBCntY -PointSize && fy < fBCntY +PointSize && FBandVisible && FBandEnable) {ret = Stat.IsBND  = true; FFocused = true ;}
    else if (               fx > fLeft             && fx < fRight            && fy > fTop              && fy < fBottom           ) {ret = Stat.IsMove = true; FFocused = true ;}
    else     return ret ;

    if(FOnChanged) FOnChanged() ;

    return ret ;
}

bool __fastcall TRectTracker::TrackerMv(TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY , float _fOffsetX, float _fOffsetY)
{
    if (!FVisible || !FEnabled ) return false;
    if (!Stat.IsLTM  && !Stat.IsCTM  && !Stat.IsRTM  &&
        !Stat.IsRCM  && !Stat.IsMove && !Stat.IsRBM  &&
        !Stat.IsCBM  && !Stat.IsLBM  && !Stat.IsLCM  && !Stat.IsBND) return false;

    m_bMoved = true ;

    float fx = _fOffsetX + x / _fScaleX ;
    float fy = _fOffsetY + y / _fScaleY ;

    //Scaled Position.
    float fLeft   = FLeft  ;
    float fTop    = FTop   ;
    float fWidth  = FWidth ;
    float fHeight = FHeight;

    float fRight  = fLeft  + fWidth  ;
    float fBottom = fTop   + fHeight ;


    //Band
    float fBtmRectCntX = GetBTRect().Left + GetBTRect().Width()/2 ;
    float fBtmRectTopY = GetBTRect().Top ;
    float fOriBCntX    = fBtmRectCntX-PointSize * 2 * _fScaleX ;
    float fOriBCntY    = fBtmRectTopY               * _fScaleY ;
    float fBCntY       = 0 ;


         if (Stat.IsLTM ) {fLeft  = fx         ; fTop = fy         ;                                            }
    else if (Stat.IsCTM ) {                      fTop = fy         ;                                            }
    else if (Stat.IsRTM ) {                      fTop = fy         ; fRight = fx         ;                      }
    else if (Stat.IsRCM ) {                                          fRight = fx         ;                      }
    else if (Stat.IsRBM ) {                                          fRight = fx         ; fBottom = fy        ;}
    else if (Stat.IsCBM ) {                                                                fBottom = fy        ;}
    else if (Stat.IsLBM ) {fLeft  = fx         ;                                           fBottom = fy        ;}
    else if (Stat.IsLCM ) {fLeft  = fx         ;                                                                }
    else if (Stat.IsBND ) {                      fBCntY = fy       ;                                            }
    else if (Stat.IsMove) {fLeft += fx-Stat.mx ; fTop+= fy-Stat.my ; fRight+= fx-Stat.mx ; fBottom+=fy-Stat.my ;}

    Stat.mx = fx;
    Stat.my = fy;

    fWidth  = fRight  - fLeft ;
    fHeight = fBottom - fTop  ;

    FLeft   = fLeft   ;
    FTop    = fTop    ;
    FWidth  = fWidth  ;
    FHeight = fHeight ;

    if(Stat.IsBND)FBand   += fOriBCntY - fBCntY ;
    if(FBand > FHeight / 2) FBand = FHeight / 2.0 ;
    if(FBand < 0          ) FBand = 0 ;
    if(FBandMax != 0 && FBand > FBandMax) FBand = FBandMax ;

    UpdateRect();

    if(FOnChanged) FOnChanged() ;

    return true ;
}


void __fastcall TRectTracker::TrackerUp(TMouseButton Button,TShiftState Shift)
{
    if (!FVisible|| !FEnabled) return ;
    if (!Stat.IsLTM  && !Stat.IsCTM  && !Stat.IsRTM  &&
        !Stat.IsRCM  && !Stat.IsMove && !Stat.IsRBM  &&
        !Stat.IsCBM  && !Stat.IsLBM  && !Stat.IsLCM  && !Stat.IsBND) return ;

    Stat.IsLTM = Stat.IsCTM = Stat.IsRTM =
    Stat.IsRCM = Stat.IsMove= Stat.IsRBM =
    Stat.IsCBM = Stat.IsLBM = Stat.IsLCM = Stat.IsBND = false ;

    if(FOnChanged) FOnChanged() ;

}


void TRectTracker::OnLoadSave(bool _bLoad,AnsiString _sFilePath , AnsiString _sCaption)
{
    TTracker::OnLoadSave(_bLoad , _sFilePath , _sCaption);

    TUserINI UserINI ;
    //Load
    if(_bLoad) {
        UserINI.Load(_sFilePath, _sCaption , "FLeft      ", FLeft      );
        UserINI.Load(_sFilePath, _sCaption , "FWidth     ", FWidth     );
        UserINI.Load(_sFilePath, _sCaption , "FTop       ", FTop       );
        UserINI.Load(_sFilePath, _sCaption , "FHeight    ", FHeight    );

        if(FBandEnable){
            UserINI.Load(_sFilePath, _sCaption , "FBand      ", FBand      );
            if(FBand == 0) FBand = 30 ;
        }
    }
    else {
        UserINI.Save(_sFilePath, _sCaption , "FLeft      ", FLeft      );
        UserINI.Save(_sFilePath, _sCaption , "FWidth     ", FWidth     );
        UserINI.Save(_sFilePath, _sCaption , "FTop       ", FTop       );
        UserINI.Save(_sFilePath, _sCaption , "FHeight    ", FHeight    );

        UserINI.Save(_sFilePath, _sCaption , "FBand      ", FBand      );
    }

    if(FLeft == 0 && FWidth == 0 && FTop == 0 && FHeight == 0) {
        FLeft        = 1   ;
        FWidth       = 100 ;
        FTop         = 1   ;
        FHeight      = 100 ;
    }
}

//OutBandRectTracker
//Rect Tracker.
//==============================================================================
TOutBandRectTracker::TOutBandRectTracker()
{
    m_iTrackerType = ttOutBandRect;
    FLeft        = 1   ;
    FWidth       = 100 ;                       
    FTop         = 1   ;
    FHeight      = 100 ;
    FHeightMax   = 0   ;
    FWidthMax    = 0   ;
    FBand        = 0   ;
    FBandEnable  = true ;
    FBandMax     = 0    ;
    FCaption     = "OutBand Tracker" ;

    //TTracker::TTracker();


    memset(&Stat , 0 , sizeof(TStat));
}

TOutBandRectTracker::~TOutBandRectTracker()
{
}

void __fastcall TOutBandRectTracker::SetLeft(float value)
{
    if(value < 0) return ;
    FLeft = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TOutBandRectTracker::SetTop(float value)
{
    if(value < 0) return ;
    FTop = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TOutBandRectTracker::SetHeight(float value)
{
    if(value < 1) value = 1 ; //return ;
    if(FHeightMax != 0 && value > FHeightMax) value = FHeightMax ;
    FHeight = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TOutBandRectTracker::SetWidth(float value)
{
    if(value < 1) value = 1 ; //return ;
    if(FWidthMax != 0 && value > FWidthMax) value = FWidthMax ;
    FWidth = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TOutBandRectTracker::SetWidthMax(float value)
{
    if(value < 0) return ;
    FWidthMax = value ;
    if(FWidth > FWidthMax && FWidthMax != 0) FWidth = FWidthMax ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TOutBandRectTracker::SetHeightMax(float value)
{
    if(value < 0) return ;
    FHeightMax = value ;
    if(FHeight > FHeightMax && FHeightMax != 0) FHeight = FHeightMax ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}



void __fastcall TOutBandRectTracker::SetBand(float value)
{
    if(value < 0       ) value = 0 ;
    if(FBandMax != 0 && value > FBandMax) value = FBandMax ;

    FBand = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TOutBandRectTracker::SetBandEnable(bool value)
{
    if(FVisible == value) return ;

    FBandEnable = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TOutBandRectTracker::SetBandMax(float value)
{
    if(value < 0) return ;
    FBandMax = value ;
    if(FBand > FBandMax && FBandMax != 0) FBand = FBandMax ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}







float __fastcall TOutBandRectTracker::GetRight()
{
    return FLeft + FWidth ;
}

float __fastcall TOutBandRectTracker::GetBottom()
{
    return FTop + FHeight ;
}

float __fastcall TOutBandRectTracker::GetCenterX()
{
    return FLeft + (FWidth/2) ;

}

float __fastcall TOutBandRectTracker::GetCenterY()
{
    return FTop + (FHeight/2) ;
}

void __fastcall TOutBandRectTracker::SetArea(int _iLeft , int _iTop , int _iWidth , int _iHeight)
{
    //FLeft =
    //작업 하자.ㅣ...

}

void __fastcall TOutBandRectTracker::UpdateRect()
{
    if(FLeft < 0) FLeft = 0 ;
    if(FTop  < 0) FTop  = 0 ;


    if(FWidth  < 1 ) {FWidth  = 1;}
    if(FHeight < 1 ) {FHeight = 1;}
}

TFRect __fastcall TOutBandRectTracker::GetOutFRect()
{
    TFRect Rect ;
    Rect.left   = FLeft       - FBand ;
    Rect.right  = GetRight()  + FBand ;
    Rect.top    = FTop        - FBand ;
    Rect.bottom = GetBottom() + FBand ;

    return Rect ;

}

TRect __fastcall TOutBandRectTracker::GetOutRect()
{
    TRect Rect ;
    Rect.left   = FLeft       - FBand ;
    Rect.right  = GetRight()  + FBand ;
    Rect.top    = FTop        - FBand ;
    Rect.bottom = GetBottom() + FBand ;

    return Rect ;

}

TFPoint __fastcall TOutBandRectTracker::GetLTPnt()
{
    TFPoint Pnt ;
    Pnt.x = FLeft ;
    Pnt.y = FTop  ;

    return Pnt ;
}

TFPoint __fastcall TOutBandRectTracker::GetRTPnt()
{
    TFPoint Pnt ;
    Pnt.x = FLeft + FWidth  ;
    Pnt.y = FTop  ;

    return Pnt ;
}

TFPoint __fastcall TOutBandRectTracker::GetLBPnt()
{
    TFPoint Pnt ;
    Pnt.x = FLeft   ;
    Pnt.y = FTop + FHeight ;

    return Pnt ;

}

TFPoint __fastcall TOutBandRectTracker::GetRBPnt()
{
    TFPoint Pnt ;
    Pnt.x = FLeft + FWidth  ;
    Pnt.y = FTop + FHeight ;

    return Pnt ;

}


void __fastcall TOutBandRectTracker::Paint(HDC _Dc , float _fScaleX , float _fScaleY , float _fOffsetX , float _fOffsetY)
{
    if(!FVisible) return ;

    CGdi *Gdi = new CGdi(_Dc);

    TFRect rect ;
    //RECT subrect = GetOutRect();
    rect.left   = (FLeft -_fOffsetX          ) * _fScaleX ;
    rect.top    = (FTop  -_fOffsetY          ) * _fScaleY ;
    rect.right  = (FLeft -_fOffsetX + FWidth ) * _fScaleX ;
    rect.bottom = (FTop  -_fOffsetY + FHeight) * _fScaleY ;

    //Band
    Gdi -> m_tPen.Style = psSolid  ;
    Gdi -> m_tPen.Color = clSilver ;
    Gdi -> m_tPen.Width = 1  ;
    Gdi -> Rect(false, rect.left-(FBand*_fScaleX) , rect.top-(FBand*_fScaleY) , rect.right+(FBand*_fScaleX)  , rect.bottom+(FBand*_fScaleY));
    Gdi -> m_tPen.Style = psSolid ;



    if(FFocused && FBandEnable) {
        Gdi -> m_tPen  .Color = PointColor;
        Gdi -> m_tBrush.Color = PointColor;

        //int iBtmRectCntX = GetOutRect().Left + GetOutRect().Width()/2 ;
        //int iBtmRectBtmY = GetOutRect().Bottom ;
        int iBtmRectCntX = (rect.left + rect.right)/2;
        int iBtmRectBtmY = rect.bottom+(FBand*_fScaleY);
        Gdi -> Rect(true, iBtmRectCntX-3*PointSize , iBtmRectBtmY-PointSize, iBtmRectCntX-PointSize  , iBtmRectBtmY+PointSize);
    }

        /*
    int iBandRectCntX = GetOutRect().Left + GetOutRect().Width()/2 ;
    int iBandRectTopY = GetOutRect().Bottom   ;
    int iBCntX  = (iBandRectCntX-PointSize * 2) ;
    int iBCntY  =  iBandRectTopY                ;
    */



    Gdi -> m_tPen.Color = LineColor ;//FEnabled ? LineColor : clGray ;
    Gdi -> m_tPen.Width = LineWidth ;
    Gdi -> Rect(false, rect.GetRECT() , 10);

    Gdi -> m_tPen  .Color = PointColor;
    Gdi -> m_tBrush.Color = PointColor;

    float x1 = rect.left  ; //* (_fScaleX ? _fScaleX : 1.0) ;
    float y1 = rect.top   ; //* (_fScaleY ? _fScaleY : 1.0) ;
    float x2 = rect.right ; //* (_fScaleX ? _fScaleX : 1.0) ;
    float y2 = rect.bottom; //* (_fScaleY ? _fScaleY : 1.0) ;
    float cx = x1+(x2-x1)*10/2/10;
    float cy = y1+(y2-y1)*10/2/10;

    char str[500];
    float rcx = x1 + (x2-x1)/2;
    float rcy = y1 + (y2-y1)/2;

    if(FFocused) {
        Gdi -> Rect(true, x1-PointSize, y1-PointSize, x1+PointSize, y1+PointSize);
        Gdi -> Rect(true, cx-PointSize, y1-PointSize, cx+PointSize, y1+PointSize);
        Gdi -> Rect(true, x2-PointSize, y1-PointSize, x2+PointSize, y1+PointSize);
        Gdi -> Rect(true, x2-PointSize, cy-PointSize, x2+PointSize, cy+PointSize);
        Gdi -> Rect(true, x2-PointSize, y2-PointSize, x2+PointSize, y2+PointSize);
        Gdi -> Rect(true, cx-PointSize, y2-PointSize, cx+PointSize, y2+PointSize);
        Gdi -> Rect(true, x1-PointSize, y2-PointSize, x1+PointSize, y2+PointSize);
        Gdi -> Rect(true, x1-PointSize, cy-PointSize, x1+PointSize, cy+PointSize);

        Gdi -> m_tText.Color = clFuchsia ;
        sprintf(str, "Start X, Y : %.1f, .1f" , x1 , y1   );
        Gdi -> Text(x1+10, y2+5 , str);
        sprintf(str, "End X, Y : %.1f, %.1f"   , x2 , y2   );
        Gdi -> Text(x1+10, y2+15, str);
        sprintf(str, "Center X, Y : %.1f, %.1f", rcx, rcy  );
        Gdi -> Text(x1+10, y2+25, str);
        sprintf(str, "Width : %.1f"          , FWidth    );
        Gdi -> Text(x1+10, y2+35, str);
        sprintf(str, "Height : %.1f"         , FHeight   );
        Gdi -> Text(x1+10, y2+45, str);
    }

    Gdi -> m_tFont.Height = 15 ;
    sprintf(str, Caption.c_str() , x1, y1  );
    Gdi -> Text(x1+10, y1-20, str);

    delete Gdi ;
}


bool __fastcall TOutBandRectTracker::TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY , float _fOffsetX, float _fOffsetY)
{
    bool ret = false ;

    if (!FVisible || !FEnabled) return ret ;

    float fx = _fOffsetX + x / _fScaleX ;
    float fy = _fOffsetY + y / _fScaleY ;


    //Rect
    float fLeft   = FLeft    ;
    float fTop    = FTop     ;
    float fWidth  = FWidth   ;
    float fHeight = FHeight  ;

    float fRight  =(fLeft + fWidth ) ;
    float fBottom =(fTop  + fHeight) ;

    //Band
    int iBandRectCntX = GetOutRect().Left + GetOutRect().Width()/2 ;
    int iBandRectTopY = GetOutRect().Bottom   ;
    int iBCntX  = (iBandRectCntX-PointSize * 2) ;
    int iBCntY  =  iBandRectTopY                ;

    int cx = (fLeft + fWidth *10/2/10) ;
    int cy = (fTop  + fHeight*10/2/10) ;

    Stat.mx = fx ;
    Stat.my = fy ;

         if (fx > fLeft  -PointSize && fx < fLeft  +PointSize && fy > fTop   -PointSize && fy < fTop   +PointSize ) {ret = Stat.IsLTM  = true; FFocused = true ;}
    else if (fx > cx     -PointSize && fx < cx     +PointSize && fy > fTop   -PointSize && fy < fTop   +PointSize ) {ret = Stat.IsCTM  = true; FFocused = true ;}
    else if (fx > fRight -PointSize && fx < fRight +PointSize && fy > fTop   -PointSize && fy < fTop   +PointSize ) {ret = Stat.IsRTM  = true; FFocused = true ;}
    else if (fx > fRight -PointSize && fx < fRight +PointSize && fy > cy     -PointSize && fy < cy     +PointSize ) {ret = Stat.IsRCM  = true; FFocused = true ;}
    else if (fx > fRight -PointSize && fx < fRight +PointSize && fy > fBottom-PointSize && fy < fBottom+PointSize ) {ret = Stat.IsRBM  = true; FFocused = true ;}
    else if (fx > cx     -PointSize && fx < cx     +PointSize && fy > fBottom-PointSize && fy < fBottom+PointSize ) {ret = Stat.IsCBM  = true; FFocused = true ;}
    else if (fx > fLeft  -PointSize && fx < fLeft  +PointSize && fy > fBottom-PointSize && fy < fBottom+PointSize ) {ret = Stat.IsLBM  = true; FFocused = true ;}
    else if (fx > fLeft  -PointSize && fx < fLeft  +PointSize && fy > cy     -PointSize && fy < cy     +PointSize ) {ret = Stat.IsLCM  = true; FFocused = true ;}
    else if (fx > iBCntX -PointSize && fx < iBCntX +PointSize && fy > iBCntY -PointSize && fy < iBCntY +PointSize && FBandEnable) {ret = Stat.IsBND  = true; FFocused = true ;}
    else if (fx > fLeft  +PointSize && fx < fRight -PointSize && fy > fTop   +PointSize && fy < fBottom-PointSize ) {ret = Stat.IsMove = true; FFocused = true ;}
    else    return ret ;

    if(FOnChanged) FOnChanged() ;

    return ret ;
}

bool __fastcall TOutBandRectTracker::TrackerMv(TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY , float _fOffsetX, float _fOffsetY)
{
    if (!FVisible || !FEnabled ) return false;
    if (!Stat.IsLTM  && !Stat.IsCTM  && !Stat.IsRTM  &&
        !Stat.IsRCM  && !Stat.IsMove && !Stat.IsRBM  &&
        !Stat.IsCBM  && !Stat.IsLBM  && !Stat.IsLCM  && !Stat.IsBND) return false;

    m_bMoved = true ;

    float fx = _fOffsetX + x / _fScaleX ;
    float fy = _fOffsetY + y / _fScaleY ;

    //Scaled Position.
    float iLeft   = FLeft   ;
    float iTop    = FTop    ;
    float iWidth  = FWidth  ;
    float iHeight = FHeight ;

    float iRight  = iLeft + iWidth  ;
    float iBottom = iTop  + iHeight ;


    //Band
    float iBandRectCntX = GetOutRect().Left + GetOutRect().Width()/2 ;
    float iBandRectTopY = GetOutRect().Bottom ;
    float iOriBCntX     = iBandRectCntX-PointSize * 2 ;
    float iOriBCntY     = iBandRectTopY               ;
    float iBCntY        = 0 ;


         if (Stat.IsLTM ) {iLeft  = fx         ; iTop = fy         ;                                            }
    else if (Stat.IsCTM ) {                      iTop = fy         ;                                            }
    else if (Stat.IsRTM ) {                      iTop = fy         ; iRight = fx         ;                      }
    else if (Stat.IsRCM ) {                                          iRight = fx         ;                      }
    else if (Stat.IsRBM ) {                                          iRight = fx         ; iBottom = fy        ;}
    else if (Stat.IsCBM ) {                                                                iBottom = fy        ;}
    else if (Stat.IsLBM ) {iLeft  = fx         ;                                           iBottom = fy        ;}
    else if (Stat.IsLCM ) {iLeft  = fx         ;                                                                }
    else if (Stat.IsBND ) {                      iBCntY = fy       ;                                            }
    else if (Stat.IsMove) {iLeft += fx-Stat.mx ; iTop+= fy-Stat.my ; iRight+= fx-Stat.mx ; iBottom+=fy-Stat.my ;}

    Stat.mx = fx;
    Stat.my = fy;

    iWidth  = iRight  - iLeft ; //* _fScaleX  ; * _fScaleX  ;
    iHeight = iBottom - iTop  ; //* _fScaleY  ; * _fScaleY  ;

    FLeft   = iLeft   ;
    FTop    = iTop    ;
    FWidth  = iWidth  ;
    FHeight = iHeight ;

    if(FHeightMax != 0 && FHeight > FHeightMax) FHeight = FHeightMax ;
    if(FWidthMax  != 0 && FWidth  > FWidthMax ) FWidth  = FWidthMax  ;

    if(Stat.IsBND)FBand -= iOriBCntY - iBCntY ;
    if(FBandMax != 0 &&FBand > FBandMax ) FBand = FBandMax ;
    if(FBand < 0        ) FBand = 0 ;



    UpdateRect();

    if(FOnChanged) FOnChanged() ;

    return true ;
}


void __fastcall TOutBandRectTracker::TrackerUp(TMouseButton Button,TShiftState Shift)
{
    if (!FVisible|| !FEnabled) return ;
    if (!Stat.IsLTM  && !Stat.IsCTM  && !Stat.IsRTM  &&
        !Stat.IsRCM  && !Stat.IsMove && !Stat.IsRBM  &&
        !Stat.IsCBM  && !Stat.IsLBM  && !Stat.IsLCM  && !Stat.IsBND) return ;

    Stat.IsLTM = Stat.IsCTM = Stat.IsRTM =
    Stat.IsRCM = Stat.IsMove= Stat.IsRBM =
    Stat.IsCBM = Stat.IsLBM = Stat.IsLCM = Stat.IsBND = false ;

    if(FOnChanged) FOnChanged() ;

}


void TOutBandRectTracker::OnLoadSave(bool _bLoad,AnsiString _sFilePath , AnsiString _sCaption)
{
    TTracker::OnLoadSave(_bLoad , _sFilePath , _sCaption);

    TUserINI UserINI ;
    //Load
    if(_bLoad) {
        UserINI.Load(_sFilePath, _sCaption , "FLeft      ", FLeft      );
        UserINI.Load(_sFilePath, _sCaption , "FWidth     ", FWidth     );
        UserINI.Load(_sFilePath, _sCaption , "FTop       ", FTop       );
        UserINI.Load(_sFilePath, _sCaption , "FHeight    ", FHeight    );

        if(FBandEnable){
            UserINI.Load(_sFilePath, _sCaption , "FBand      ", FBand      );
            if(FBand == 0) FBand = 30 ;
        }
    }
    else {
        UserINI.Save(_sFilePath, _sCaption , "FLeft      ", FLeft      );
        UserINI.Save(_sFilePath, _sCaption , "FWidth     ", FWidth     );
        UserINI.Save(_sFilePath, _sCaption , "FTop       ", FTop       );
        UserINI.Save(_sFilePath, _sCaption , "FHeight    ", FHeight    );

        UserINI.Save(_sFilePath, _sCaption , "FBand      ", FBand      );
    }

    if(FLeft == 0 && FWidth == 0 && FTop == 0 && FHeight == 0) {
        FLeft        = 1   ;
        FWidth       = 100 ;
        FTop         = 1   ;
        FHeight      = 100 ;
    }
}



//Line Tracker.
//==============================================================================
TLineTracker::TLineTracker()
{
    m_iTrackerType = ttLine;
    FStartX  = 1   ;
    FStartY  = 1   ;
    FEndX    = 100 ;
    FEndY    = 100 ;
    FCaption = "Line Tracker" ;

    //TTracker::TTracker();

    memset(&Stat , 0 , sizeof(TStat));
}

TLineTracker::~TLineTracker()
{
}

void __fastcall TLineTracker::SetStartX(float value)
{
    if(value < 0) return ;
    FStartX = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TLineTracker::SetStartY(float value)
{
    if(value < 0) return ;
    FStartY = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TLineTracker::SetEndX(float value)
{
    if(value < 1) return ;
    FEndX = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TLineTracker::SetEndY(float value)
{
    if(value < 1) return ;
    FEndY = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TLineTracker::Paint(HDC _Dc , float _fScaleX , float _fScaleY , float _fOffsetX , float _fOffsetY)
{
    if(!FVisible) return ;

    CGdi * Gdi = new CGdi(_Dc);


    //Display Position.
    float fStartX   = (FStartX - _fOffsetX) * _fScaleX ;
    float fStartY   = (FStartY - _fOffsetY) * _fScaleY ;
    float fEndX     = (  FEndX - _fOffsetX) * _fScaleX ;
    float fEndY     = (  FEndY - _fOffsetY) * _fScaleY ;
    float fCx       = (fStartX + fEndX)/ 2.0 ;
    float fCy       = (fStartY + fEndY)/ 2.0 ;
    float fTextPntX = fStartY < fEndY ? fEndX   : fStartX ;  //FStartY < FEndY ? iScEndX : iScStartX ;
    float fTextPntY = fStartY < fEndY ? fEndY   : fStartY ;  //FStartY < FEndY ? iScEndY : iScStartY ;
    float fCaptPntX = fStartY < fEndY ? fStartX : fEndX   ;  //FStartY < FEndY ? iScStartX : iScEndX ;
    float fCaptPntY = fStartY < fEndY ? fStartY : fEndY   ;  //FStartY < FEndY ? iScStartY : iScEndY ;

    if(FFocused) {
        Gdi -> m_tPen  .Color = PointColor;
        Gdi -> m_tBrush.Color = PointColor;
        Gdi -> Circle(true , fEndX , fEndY , FPointSize , 0);
    }
    Gdi -> m_tPen.Color   = LineColor ; //FEnabled ? LineColor  : clGray ;
    Gdi -> m_tBrush.Color = PointColor; //FEnabled ? PointColor : clGray ;
    Gdi -> m_tPen.Width   = LineWidth ;
    Gdi -> LineAllow(fStartX  , fStartY  , fEndX  , fEndY );
    if(FFocused) {
        Gdi -> m_tPen  .Color = PointColor;
        Gdi -> m_tBrush.Color = PointColor;
        Gdi -> Circle(true , fStartX , fStartY , FPointSize , 0);
    }
    //중간부분.
    Gdi -> Rect  (true , fCx - FPointSize , fCy - FPointSize , fCx + FPointSize , fCy + FPointSize);

    char str[500];
    Gdi -> m_tText.Color = clFuchsia ;
    sprintf(str, "Start X, Y : %.1f, %.1f" , FStartX , FStartY);
    Gdi -> Text(fTextPntX+10, fTextPntY+5 , str);
    sprintf(str, "End X, Y : %.1f, %.1f"   , FEndX   , FEndY  );
    Gdi -> Text(fTextPntX+10, fTextPntY+15, str);
    sprintf(str, "Angle : %0.2f"          , MATH_GetLineAngle(FStartX , FStartY , FEndX , FEndY));
    Gdi -> Text(fTextPntX+10, fTextPntY+25, str);
    sprintf(str, "Length : %0.2f"         , MATH_GetLineLenth(FStartX , FStartY , FEndX , FEndY));
    Gdi -> Text(fTextPntX+10, fTextPntY+35, str);

    Gdi -> m_tFont.Height = 15 ;
    sprintf(str, Caption.c_str() , fCaptPntX, fCaptPntY  );
    Gdi -> Text(fCaptPntX+10, fCaptPntY-20, str);

    delete Gdi ;



}

bool __fastcall TLineTracker::TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY , float _fOffsetX, float _fOffsetY)
{
    bool ret = false ;

    if (!FVisible || !FEnabled) return ret ;

    //float fx = x * _fScaleX ;
    //float fy = y * _fScaleY ;

    float fx = _fOffsetX + x / _fScaleX ;
    float fy = _fOffsetY + y / _fScaleY ;

    float fStartX = FStartX ; //XScale ;
    float fStartY = FStartY ; //YScale ;
    float fEndX   = FEndX   ;
    float fEndY   = FEndY   ;

    float fCx = (FStartX + FEndX) / 2.0 ;
    float fCy = (FStartY + FEndY) / 2.0 ;

    Stat.mx = fx ;
    Stat.my = fy ;


         if (fx > fStartX -PointSize && fx < fStartX +PointSize && fy > fStartY -PointSize && fy < fStartY +PointSize ) {ret = Stat.IsStt  = true; FFocused = true ;}
    else if (fx > fEndX   -PointSize && fx < fEndX   +PointSize && fy > fEndY   -PointSize && fy < fEndY   +PointSize ) {ret = Stat.IsEnd  = true; FFocused = true ;}
    else if (fx > fCx     -PointSize && fx < fCx     +PointSize && fy > fCy     -PointSize && fy < fCy     +PointSize ) {ret = Stat.IsMove = true; FFocused = true ;}
    else    return ret ;

    if(FOnChanged) FOnChanged() ;

    return ret ;
}

bool __fastcall TLineTracker::TrackerMv(TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY , float _fOffsetX, float _fOffsetY)
{
    if (!FVisible || !FEnabled ) return false;
    if (!Stat.IsStt  && !Stat.IsEnd  && !Stat.IsMove && !Stat.IsMove) return false;

    m_bMoved = true ;

    float fx = _fOffsetX + x / _fScaleX ;
    float fy = _fOffsetY + y / _fScaleY ;

    //Scaled Position.
    float fStartX = FStartX ; //XScale ;
    float fStartY = FStartY ; //YScale ;
    float fEndX   = FEndX   ;
    float fEndY   = FEndY   ;

         if (Stat.IsStt ) {fStartX  = fx         ; fStartY = fy         ;                                         }
    else if (Stat.IsEnd ) {fEndX    = fx         ; fEndY   = fy         ;                                         }
    else if (Stat.IsMove) {fStartX += fx-Stat.mx ; fStartY+= fy-Stat.my ; fEndX+= fx-Stat.mx ; fEndY+=fy-Stat.my ;}

    Stat.mx = fx;
    Stat.my = fy;

    FStartX = fStartX ;// (_fScaleX ? _fScaleX : 1) ;
    FStartY = fStartY ;// (_fScaleY ? _fScaleY : 1) ;
    FEndX   = fEndX   ;// (_fScaleX ? _fScaleX : 1) ;
    FEndY   = fEndY   ;// (_fScaleY ? _fScaleY : 1) ;

    if(FOnChanged) FOnChanged() ;

    return true ;
}


void __fastcall TLineTracker::TrackerUp(TMouseButton Button,TShiftState Shift )
{
    if (!FVisible || !FEnabled ) return ;
    if (!Stat.IsStt  && !Stat.IsEnd  && !Stat.IsMove && !Stat.IsMove) return ;

    Stat.IsStt = Stat.IsEnd = Stat.IsMove = Stat.IsMove = false;

    if(FOnChanged) FOnChanged() ;

}


void TLineTracker::OnLoadSave(bool _bLoad,AnsiString _sFilePath , AnsiString _sCaption)
{
    TTracker::OnLoadSave(_bLoad , _sFilePath , _sCaption);

    TUserINI UserINI ;
    //Load
    if(_bLoad) {
        UserINI.Load(_sFilePath, _sCaption , "FStartX", FStartX    );
        UserINI.Load(_sFilePath, _sCaption , "FStartY", FStartY    );
        UserINI.Load(_sFilePath, _sCaption , "FEndX  ", FEndX      );
        UserINI.Load(_sFilePath, _sCaption , "FEndY  ", FEndY      );
    }
    else {
        UserINI.Save(_sFilePath, _sCaption , "FStartX", FStartX    );
        UserINI.Save(_sFilePath, _sCaption , "FStartY", FStartY    );
        UserINI.Save(_sFilePath, _sCaption , "FEndX  ", FEndX      );
        UserINI.Save(_sFilePath, _sCaption , "FEndY  ", FEndY      );
    }

    if(FStartX == 0 && FStartY == 0 && FEndX == 0 && FEndY == 0) {
        FStartX = 1   ;
        FStartY = 1   ;
        FEndX   = 100 ;
        FEndY   = 100 ;
    }





}


//Circle Tracker.
//==============================================================================
TCircleTracker::TCircleTracker()
{
    m_iTrackerType = ttCircle;
    FX             = 100 ;
    FY             = 100 ;
    FRadius        = 100 ;
    FCaption       = "Circle Tracker" ;

    //TTracker::TTracker();

    memset(&Stat , 0 , sizeof(TStat));
}

TCircleTracker::~TCircleTracker()
{

}

void __fastcall TCircleTracker::SetX(float value)
{
    if(value < 0) return ;
    FX = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TCircleTracker::SetY(float value)
{
    if(value < 0) return ;
    FY = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TCircleTracker::SetRadius(float value)
{
    if(value < 0) return ;
    FRadius = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TCircleTracker::SetBand(float value)
{
    if(value < 0) return ;
    FBand = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

float __fastcall TCircleTracker::GetLeft()
{
    return FX-FRadius ;

}

float __fastcall TCircleTracker::GetTop()
{
    return FY-FRadius ;
}

float __fastcall TCircleTracker::GetRight()
{
    return FX+FRadius ;
}

float __fastcall TCircleTracker::GetBottom()
{
    return FY+FRadius ;
}

TCircle __fastcall TCircleTracker::GetTCircle()
{
    TCircle tRet ;

    tRet.cx  = FX ;
    tRet.cy  = FY ;
    tRet.rad = FRadius ;

    return tRet ;
}

TFCircle __fastcall TCircleTracker::GetTFCircle()
{
    TFCircle tRet ;

    tRet.cx  = FX ;
    tRet.cy  = FY ;
    tRet.rad = FRadius ;

    return tRet ;
}



void __fastcall TCircleTracker::Paint(HDC _Dc , float _fScaleX , float _fScaleY , float _fOffsetX , float _fOffsetY)
{
    if(!FVisible) return ;

    CGdi * Gdi = new CGdi(_Dc);

    int fCx = FX * _fScaleX ;
    int fCy = FY * _fScaleY ;

    float fLeft   = (FX - FRadius - _fOffsetX) * _fScaleX ;
    float fTop    = (FY - FRadius - _fOffsetY) * _fScaleY ;
    float fRight  = (FX + FRadius - _fOffsetX) * _fScaleX ;
    float fBottom = (FY + FRadius - _fOffsetY) * _fScaleY ;

    float fOutLeft   = (FX - FRadius - FBand - _fOffsetX) * _fScaleX ;
    float fOutTop    = (FY - FRadius - FBand - _fOffsetY) * _fScaleY ;
    float fOutRight  = (FX + FRadius + FBand - _fOffsetX) * _fScaleX ;
    float fOutBottom = (FY + FRadius + FBand - _fOffsetY) * _fScaleY ;

    float fInLeft    = (FX - FRadius + FBand - _fOffsetX) * _fScaleX ;
    float fInTop     = (FY - FRadius + FBand - _fOffsetY) * _fScaleY ;
    float fInRight   = (FX + FRadius - FBand - _fOffsetX) * _fScaleX ;
    float fInBottom  = (FY + FRadius - FBand - _fOffsetY) * _fScaleY ;


    float fTextPntX = fCx     ;
    float fTextPntY = fBottom ;

    float fCaptPntX = fCx     ;
    float fCaptPntY = fTop    ;

    Gdi -> m_tPen.Color   = LineColor ;//FEnabled ? LineColor  : clGray ;
    Gdi -> m_tBrush.Color = PointColor;//FEnabled ? PointColor : clGray ;
    Gdi -> m_tPen.Width   = LineWidth ;
    Gdi -> Circle(false , fLeft, fTop, fRight, fBottom , 10);

    if(FBand) {
        Gdi -> m_tPen.Color   = clGray ;
        Gdi -> m_tPen.Style   = psDot  ;
        Gdi -> Circle(false , fOutLeft , fOutTop, fOutRight, fOutBottom , 0);
        Gdi -> Circle(false , fInLeft  , fInTop , fInRight , fInBottom  , 0);
        Gdi -> m_tPen.Style   = psSolid ;
    }

    if(FFocused) {
        Gdi -> m_tPen  .Color = PointColor;
        Gdi -> m_tBrush.Color = PointColor;
        Gdi -> Rect  (true , fLeft -FPointSize , fCy      - FPointSize , fLeft  + FPointSize , fCy      + FPointSize);
        Gdi -> Rect  (true , fCx   -FPointSize , fTop     - FPointSize , fCx    + FPointSize , fTop     + FPointSize);
        Gdi -> Rect  (true , fRight-FPointSize , fCy      - FPointSize , fRight + FPointSize , fCy      + FPointSize);
        Gdi -> Rect  (true , fCx   -FPointSize , fBottom  - FPointSize , fCx    + FPointSize , fBottom  + FPointSize);
    }

    char str[500];
    Gdi -> m_tText.Color = clFuchsia ;
    sprintf(str, "Center X, Y : %.1f, %.1f" , FX , FY);
    Gdi -> Text(fTextPntX+10, fTextPntY+5 , str);
    sprintf(str, "Radius : %.1f"   , FRadius   );
    Gdi -> Text(fTextPntX+10, fTextPntY+15, str);

    Gdi -> m_tFont.Height = 15 ;
    sprintf(str, Caption.c_str() , fCaptPntX, fCaptPntY  );
    Gdi -> Text(fCaptPntX+10, fCaptPntY-20, str);

    delete Gdi ;
}

bool __fastcall TCircleTracker::TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    bool ret = false ;

    if (!FVisible || !FEnabled) return ret ;

    //float fx = x * _fScaleX ;
    //float fy = y * _fScaleY ;

    float fx = _fOffsetX + x / _fScaleX ;
    float fy = _fOffsetY + y / _fScaleY ;

    float fCx     = FX * _fScaleX ;
    float fCy     = FY * _fScaleY ;

    float fClickX = _fScaleX ? x / _fScaleX : x ;
    float fClickY = _fScaleY ? y / _fScaleY : y ;

    float fLeft   = GetLeft()   * _fScaleX ;
    float fTop    = GetTop ()   * _fScaleY ;
    float fWidth  = FRadius * 2 * _fScaleX ;
    float fHeight = FRadius * 2 * _fScaleY ;
    float fRight  =(fLeft + fWidth ) ;
    float fBottom =(fTop  + fHeight) ;

    Stat.mx = fx ;
    Stat.my = fy ;

         if (x > fCx   -(FPointSize+1) && x < fCx   +(FPointSize+1) && y > fTop   -(FPointSize+1) && y < fTop   +(FPointSize+1) ) {ret = Stat.IsTP   = true; FFocused = true ;}
    else if (x > fRight-(FPointSize+1) && x < fRight+(FPointSize+1) && y > fCy    -(FPointSize+1) && y < fCy    +(FPointSize+1) ) {ret = Stat.IsRT   = true; FFocused = true ;}
    else if (x > fCx   -(FPointSize+1) && x < fCx   +(FPointSize+1) && y > fBottom-(FPointSize+1) && y < fBottom+(FPointSize+1) ) {ret = Stat.IsBT   = true; FFocused = true ;}
    else if (x > fLeft -(FPointSize+1) && x < fLeft +(FPointSize+1) && y > fCy    -(FPointSize+1) && y < fCy    +(FPointSize+1) ) {ret = Stat.IsLT   = true; FFocused = true ;}
  //else if (x > fLeft +(FPointSize+1) && x < fRight-(FPointSize+1) && y > fTop   +(FPointSize+1) && y < fBottom-(FPointSize+1) ) {ret = Stat.IsMove = true; FFocused = true ;}
    else if (MATH_GetCircleInPoint(FX , FY , FRadius , fClickX , fClickY)                                       ) {ret = Stat.IsMove = true; FFocused = true ;}
    else    return ret ;

    if(FOnChanged) FOnChanged() ;

    return ret ;

}

bool __fastcall TCircleTracker::TrackerMv(TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    if (!FVisible || !FEnabled ) return false;
    if (               !Stat.IsTP                 &&
        !Stat.IsLT  && !Stat.IsMove && !Stat.IsRT &&
                       !Stat.IsBT                 ) return false;

    m_bMoved = true ;

    float fx = _fOffsetX + x / _fScaleX ;
    float fy = _fOffsetY + y / _fScaleY ;

    float fCx = FX * _fScaleX ;
    float fCy = FY * _fScaleY ;

    float fLeft = GetLeft() * _fScaleX ;
    float fTop  = GetTop () * _fScaleY ;
    float fXRad = FRadius   * _fScaleX ;
    float fYRad = FRadius   * _fScaleY ;

         if (Stat.IsTP  ) { fYRad -= y-Stat.my ; FRadius = fYRad / (_fScaleY ? _fScaleY : 1) ; }
    else if (Stat.IsLT  ) { fXRad -= x-Stat.mx ; FRadius = fXRad / (_fScaleX ? _fScaleX : 1) ; }
    else if (Stat.IsRT  ) { fXRad += x-Stat.mx ; FRadius = fXRad / (_fScaleX ? _fScaleX : 1) ; }
    else if (Stat.IsBT  ) { fYRad += y-Stat.my ; FRadius = fYRad / (_fScaleY ? _fScaleY : 1) ; }
    else if (Stat.IsMove) { fCx   += x-Stat.mx ; fCy+= y-Stat.my ;
                            FX = fCx / (_fScaleX ? _fScaleX : 1) ;
                            FY = fCy / (_fScaleY ? _fScaleY : 1) ;
                          }

    Stat.mx = fx;
    Stat.my = fy;


    if(FOnChanged) FOnChanged() ;

    return true ;
}

void __fastcall TCircleTracker::TrackerUp(TMouseButton Button,TShiftState Shift)
{
    if (!FVisible || !FEnabled ) return ;
    if (               !Stat.IsTP                 &&
        !Stat.IsLT  && !Stat.IsMove && !Stat.IsRT &&
                       !Stat.IsBT                 ) return ;

    Stat.IsTP = Stat.IsLT = Stat.IsMove = Stat.IsRT = Stat.IsBT = false ;

    if(FOnChanged) FOnChanged() ;


}


void TCircleTracker::OnLoadSave(bool _bLoad,AnsiString _sFilePath , AnsiString _sCaption)
{
    TTracker::OnLoadSave(_bLoad , _sFilePath , _sCaption);

    TUserINI UserINI ;
    //Load
    if(_bLoad) {
        UserINI.Load(_sFilePath, _sCaption , "FX     ", FX     );
        UserINI.Load(_sFilePath, _sCaption , "FY     ", FY     );
        UserINI.Load(_sFilePath, _sCaption , "FRadius", FRadius);
    }
    else {
        UserINI.Save(_sFilePath, _sCaption , "FX     ", FX     );
        UserINI.Save(_sFilePath, _sCaption , "FY     ", FY     );
        UserINI.Save(_sFilePath, _sCaption , "FRadius", FRadius);
    }

    if(FX == 0 && FY == 0 && FRadius == 0) {
        FX      = 100 ;
        FY      = 100 ;
        FRadius = 100 ;
    }


}




//Poly Tracker.
//==============================================================================
TPolyTracker::TPolyTracker()
{
    m_iTrackerType = ttPoly ;
    FPointCount = 4 ;
    FCaption    = "Poly Tracker" ;

    //TTracker::TTracker();

    memset(&Stat , 0 , sizeof(TStat));
    Stat.iClickNo = -1 ;
    FPoints    = new TFPoint[FPointCount];

    FLeft   = 10 ;
    FTop    = 10 ;
    FWidth  = 100;
    FHeight = 100;

    FPoints[0].x =  10 ; FPoints[0].y =  10 ;
    FPoints[1].x = 110 ; FPoints[1].y =  10 ;
    FPoints[2].x = 110 ; FPoints[2].y = 110 ;
    FPoints[3].x =  10 ; FPoints[3].y = 110 ;


}

TPolyTracker::~TPolyTracker()
{
    if(FPoints   ){delete [] FPoints    ; FPoints    = NULL; }
}
bool TPolyTracker::InsertPoint(int _iNo)
{
    FPointCount++;
    TFPoint * pTempPnt = new TFPoint[FPointCount] ;
    int iInserted = 0 ;
    for(int i = 0 ; i < FPointCount ; i++){
        if(i==_iNo) {
            pTempPnt[i].x = FPoints[i].x - 10 ;
            pTempPnt[i].y = FPoints[i].y - 10 ;
            iInserted = 1 ;
            continue ;
        }
        pTempPnt[i].x = FPoints[i-iInserted].x ;
        pTempPnt[i].y = FPoints[i-iInserted].y ;
    }

    if(FPoints   ) delete [] FPoints    ;

    FPoints    = new TFPoint[FPointCount] ;

    memcpy (FPoints , pTempPnt , sizeof(TFPoint) * FPointCount) ;

    delete [] pTempPnt ;

    return true ;
}

bool TPolyTracker::DeletePoint(int _iNo)
{
    if(FPointCount < 4) return false ;

    FPointCount--;
    TFPoint * pTempPnt = new TFPoint[FPointCount] ;
    int iDeleted = 0 ;

    for(int i = 0 ; i < FPointCount ; i++){
        if(i==_iNo) {
            iDeleted = 1 ;
        }
        pTempPnt[i].x = FPoints[i+iDeleted].x ;
        pTempPnt[i].y = FPoints[i+iDeleted].y ;
    }

    if(FPoints   ) delete [] FPoints    ;

    FPoints    = new TFPoint[FPointCount] ;

    memcpy (FPoints , pTempPnt , sizeof(TFPoint) * FPointCount) ;

    delete [] pTempPnt ;

    return true ;

}
float TPolyTracker::GetPointX(int _iNo)
{
    if(FPointCount <= _iNo) return 0 ;
    return FPoints[_iNo].x ;
}

float TPolyTracker::GetPointY(int _iNo)
{
    if(FPointCount <= _iNo) return 0 ;
    return FPoints[_iNo].y ;
}

float TPolyTracker::GetPointCount()
{
    return FPointCount ;

}

//event link
void __fastcall TPolyTracker::Paint(HDC _Dc , float _fScaleX , float _fScaleY , float _fOffsetX , float _fOffsetY)
{
    if(!FVisible) return ;

    CGdi * Gdi = new CGdi(_Dc);

    TFRect rect ;
    rect.left   = (FLeft           - _fOffsetX) * _fScaleX ;
    rect.top    = (FTop            - _fOffsetY) * _fScaleY ;
    rect.right  = (FLeft + FWidth  - _fOffsetX) * _fScaleX ;
    rect.bottom = (FTop  + FHeight - _fOffsetY) * _fScaleY ;

    Gdi -> m_tPen.Color = clGray ;
    Gdi -> m_tPen.Width = LineWidth ;
    Gdi -> m_tPen.Style = PS_DOT ;
    Gdi -> Rect(false, rect.GetRECT(), 10);
    Gdi -> m_tPen.Style = PS_SOLID ;


    Gdi -> m_tPen  .Color = LineColor ;
    Gdi -> m_tBrush.Color = PointColor;

    float x1 = rect.left  ; //* (_fScaleX ? _fScaleX : 1.0) ;
    float y1 = rect.top   ; //* (_fScaleY ? _fScaleY : 1.0) ;
    float x2 = rect.right ; //* (_fScaleX ? _fScaleX : 1.0) ;
    float y2 = rect.bottom; //* (_fScaleY ? _fScaleY : 1.0) ;
    float cx = x1+(x2-x1)*10/2/10;
    float cy = y1+(y2-y1)*10/2/10;

    char str[500];
    float rcx = x1 + (x2-x1)/2;
    float rcy = y1 + (y2-y1)/2;

    float fX1 ,fY1;
    float fX2 ,fY2;

    for(int i = 0 ; i < FPointCount - 1 ; i++) {
        fX1 = FPoints[i  ].x * _fScaleX ;
        fX2 = FPoints[i+1].x * _fScaleX ;

        fY1 = FPoints[i  ].y * _fScaleY ;
        fY2 = FPoints[i+1].y * _fScaleY ;

        Gdi -> Line(fX1 , fY1 , fX2 , fY2);
    }


    fX1 = FPoints[0            ].x * _fScaleX ;
    fX2 = FPoints[FPointCount-1].x * _fScaleX ;

    fY1 = FPoints[0            ].y * _fScaleY ;
    fY2 = FPoints[FPointCount-1].y * _fScaleY ;

    Gdi -> Line(fX1 , fY1 , fX2 , fY2);

    if(FFocused) {
        Gdi -> m_tPen  .Color = PointColor;
        Gdi -> m_tBrush.Color = PointColor;

        for(int i = 0 ; i < FPointCount ; i++) {
            fX1 = FPoints[i].x * _fScaleX ;
            fY1 = FPoints[i].y * _fScaleY ;
            Gdi -> Circle(true, fX1 , fY1 , FPointSize , 0 );
        }

        Gdi -> m_tText.Color = clFuchsia ;
        sprintf(str, "Point Count : %.1f"    , FPointCount);
        Gdi -> Text(x1+10, y2+ 5, str);
        sprintf(str, "Center X, Y : %.1f, %.1f", rcx, rcy   );
        Gdi -> Text(x1+10, y2+15, str);
        sprintf(str, "Width : %.1f"          , FWidth     );
        Gdi -> Text(x1+10, y2+25, str);
        sprintf(str, "Height : %.1f"         , FHeight    );
        Gdi -> Text(x1+10, y2+35, str);
    }

    Gdi -> m_tFont.Height = 15 ;
    sprintf(str, Caption.c_str() , x1, y1  );
    Gdi -> Text(x1+10, y1-20, str);

    delete Gdi ;
}

bool __fastcall TPolyTracker::TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    bool ret = false ;

    if (!FVisible || !FEnabled) return ret ;

    float fx = _fOffsetX + x / _fScaleX ;
    float fy = _fOffsetY + y / _fScaleY ;

    //float fx = x * _fScaleX ;
    //float fy = y * _fScaleY ;
    Stat.mx = fx ;
    Stat.my = fy ;


    float fLeft   = FLeft   * _fScaleX  ; //XScale ;
    float fTop    = FTop    * _fScaleY  ; //YScale ;
    float fWidth  = FWidth  * _fScaleX  ;
    float fHeight = FHeight * _fScaleY  ;

    float fRight  =(fLeft + fWidth ) ;
    float fBottom =(fTop  + fHeight) ;


    //int cx = (fLeft + fWidth *10/2/10) ;
    //int cy = (fTop  + fHeight*10/2/10) ;

    float fX ; //FX설리짱~~~~
    float fY ;

    for(int i = 0 ; i < FPointCount ; i++) {
        fX = FPoints[i].x * _fScaleX ;
        fY = FPoints[i].y * _fScaleY ;

        if (x > fX -PointSize && x < fX +PointSize && y > fY - PointSize && y < fY + PointSize ) {
            if(Shift.Contains(ssCtrl)) {
                InsertPoint(i);
                if(FOnChanged) FOnChanged() ;
                return true ;
            }
            else if(Shift.Contains(ssAlt)) {
                DeletePoint(i);
                if(FOnChanged) FOnChanged() ;
                return true ;
            }
            else {
                ret = true ;
                Stat.iClickNo = i;
                FFocused = true ;
                break ;
            }
        }
    }

//         if (x > fLeft -PointSize && x < fLeft +PointSize && y > fTop   -PointSize && y < fTop   +PointSize ) {ret = Stat.IsLTM  = true; FFocused = true ;}
//    else if (x > cx    -PointSize && x < cx    +PointSize && y > fTop   -PointSize && y < fTop   +PointSize ) {ret = Stat.IsCTM  = true; FFocused = true ;}
//    else if (x > fRight-PointSize && x < fRight+PointSize && y > fTop   -PointSize && y < fTop   +PointSize ) {ret = Stat.IsRTM  = true; FFocused = true ;}
//    else if (x > fRight-PointSize && x < fRight+PointSize && y > cy     -PointSize && y < cy     +PointSize ) {ret = Stat.IsRCM  = true; FFocused = true ;}
//    else if (x > fRight-PointSize && x < fRight+PointSize && y > fBottom-PointSize && y < fBottom+PointSize ) {ret = Stat.IsRBM  = true; FFocused = true ;}
//    else if (x > cx    -PointSize && x < cx    +PointSize && y > fBottom-PointSize && y < fBottom+PointSize ) {ret = Stat.IsCBM  = true; FFocused = true ;}
//    else if (x > fLeft -PointSize && x < fLeft +PointSize && y > fBottom-PointSize && y < fBottom+PointSize ) {ret = Stat.IsLBM  = true; FFocused = true ;}
//    else if (x > fLeft -PointSize && x < fLeft +PointSize && y > cy     -PointSize && y < cy     +PointSize ) {ret = Stat.IsLCM  = true; FFocused = true ;}
    if (Stat.iClickNo < 0 && x > fLeft +PointSize && x < fRight-PointSize && y > fTop   +PointSize && y < fBottom-PointSize ) {ret = Stat.IsMove = true; FFocused = true ;}
    else    return ret ;

    if(FOnChanged) FOnChanged() ;

    return ret ;
}

bool __fastcall TPolyTracker::TrackerMv(TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    if (!FVisible || !FEnabled ) return false;
    if (!Stat.IsLTM  && !Stat.IsCTM  && !Stat.IsRTM  &&
        !Stat.IsRCM  && !Stat.IsMove && !Stat.IsRBM  &&
        !Stat.IsCBM  && !Stat.IsLBM  && !Stat.IsLCM  && Stat.iClickNo < 0) return false;

    m_bMoved = true ;

    float fx = _fOffsetX + x / _fScaleX ;
    float fy = _fOffsetY + y / _fScaleY ;

    //Scaled Position.
    int iLeft   = (FLeft   * _fScaleX) ;
    int iTop    = (FTop    * _fScaleY) ;
    int iWidth  = (FWidth  * _fScaleX) ;
    int iHeight = (FHeight * _fScaleY) ;

    int iRight  = (iLeft + iWidth ) ;
    int iBottom = (iTop  + iHeight) ;

    int iX ;//= FPoints[Stat.iClickNo].x ;
    int iY ;//= FPoints[Stat.iClickNo].y ;

    int iMinX = ~(1<<(sizeof(int)*8-1));
    int iMaxX = 0 ;
    int iMinY = ~(1<<(sizeof(int)*8-1));
    int iMaxY = 0 ;

    if (Stat.iClickNo >=0 ) {
        iX = FPoints[Stat.iClickNo].x * _fScaleX ;
        iY = FPoints[Stat.iClickNo].y * _fScaleY ;

        iX += x-Stat.mx ;
        iY += y-Stat.my ;

        FPoints[Stat.iClickNo].x = iX / (_fScaleX ? _fScaleX : 1) ;
        FPoints[Stat.iClickNo].y = iY / (_fScaleY ? _fScaleY : 1) ;


    }
    else if (Stat.IsMove       ) {
        iLeft += x-Stat.mx ; iTop+= y-Stat.my ; iRight+= x-Stat.mx ; iBottom+=y-Stat.my ;
        for(int i = 0 ; i < FPointCount ; i++) {
            iX = FPoints[i].x * _fScaleX ;
            iY = FPoints[i].y * _fScaleY ;

            iX += x-Stat.mx ;
            iY += y-Stat.my ;

            FPoints[i].x = iX / (_fScaleX ? _fScaleX : 1) ;
            FPoints[i].y = iY / (_fScaleY ? _fScaleY : 1) ;

        }
    }

    for(int i = 0 ; i < FPointCount ; i++) {
        if(iMinX > FPoints[i].x) iMinX = FPoints[i].x ;
        if(iMaxX < FPoints[i].x) iMaxX = FPoints[i].x ;
        if(iMinY > FPoints[i].y) iMinY = FPoints[i].y ;
        if(iMaxY < FPoints[i].y) iMaxY = FPoints[i].y ;
    }

    Stat.mx = fx;
    Stat.my = fy;

    iLeft   = iMinX ;
    iTop    = iMinY ;
    iRight  = iMaxX ;
    iBottom = iMaxY ;

    iWidth  = iRight  - iLeft ; //* _fScaleX  ; * _fScaleX  ;
    iHeight = iBottom - iTop  ; //* _fScaleY  ; * _fScaleY  ;

    FLeft   = iLeft   ;
    FTop    = iTop    ;
    FWidth  = iWidth  ;
    FHeight = iHeight ;




    if(FOnChanged) FOnChanged() ;

    return true ;
}

void __fastcall TPolyTracker::TrackerUp(TMouseButton Button,TShiftState Shift)
{
    if (!FVisible || !FEnabled ) return ;
    if (!Stat.IsLTM  && !Stat.IsCTM  && !Stat.IsRTM  &&
        !Stat.IsRCM  && !Stat.IsMove && !Stat.IsRBM  &&
        !Stat.IsCBM  && !Stat.IsLBM  && !Stat.IsLCM  && Stat.iClickNo < 0) return ;

    memset(&Stat , 0 , sizeof(TStat));
    Stat.iClickNo = -1 ;

    if(FOnChanged) FOnChanged() ;
}

//Load Save.
void TPolyTracker::OnLoadSave(bool _bLoad,AnsiString _sFilePath , AnsiString _sCaption)
{

}





//Poly Line Tracker.
//==============================================================================
TPolyLineTracker::TPolyLineTracker()
{
    m_iTrackerType = ttPolyLine ;
    FPointCount = 4 ;
    FCaption    = "PolyLine Tracker" ;

    //TTracker::TTracker();

    memset(&Stat , 0 , sizeof(TStat));
    Stat.iClickNo = -1 ;
    FPoints    = new TFPoint[FPointCount];

    FLeft   = 10 ;
    FTop    = 10 ;
    FWidth  = 100;
    FHeight = 100;

    FPoints[0].x =  10 ; FPoints[0].y =  10 ;
    FPoints[1].x = 110 ; FPoints[1].y =  10 ;
    FPoints[2].x = 110 ; FPoints[2].y = 110 ;
    FPoints[3].x =  10 ; FPoints[3].y = 110 ;


}

TPolyLineTracker::~TPolyLineTracker()
{
    if(FPoints   ){delete [] FPoints    ; FPoints    = NULL; }
}
bool TPolyLineTracker::InsertPoint(int _iNo)
{
    FPointCount++;
    TFPoint * pTempPnt = new TFPoint[FPointCount] ;
    int iInserted = 0 ;
    for(int i = 0 ; i < FPointCount ; i++){
        if(i==_iNo) {
            pTempPnt[i].x = FPoints[i].x - 10 ;
            pTempPnt[i].y = FPoints[i].y - 10 ;
            iInserted = 1 ;
            continue ;
        }
        pTempPnt[i].x = FPoints[i-iInserted].x ;
        pTempPnt[i].y = FPoints[i-iInserted].y ;
    }

    if(FPoints   ) delete [] FPoints    ;

    FPoints    = new TFPoint[FPointCount] ;

    memcpy (FPoints , pTempPnt , sizeof(TFPoint) * FPointCount) ;

    delete [] pTempPnt ;

    return true ;
}

bool TPolyLineTracker::DeletePoint(int _iNo)
{
    if(FPointCount < 4) return false ;

    FPointCount--;
    TFPoint * pTempPnt = new TFPoint[FPointCount] ;
    int iDeleted = 0 ;

    for(int i = 0 ; i < FPointCount ; i++){
        if(i==_iNo) {
            iDeleted = 1 ;
        }
        pTempPnt[i].x = FPoints[i+iDeleted].x ;
        pTempPnt[i].y = FPoints[i+iDeleted].y ;
    }

    if(FPoints   ) delete [] FPoints    ;

    FPoints    = new TFPoint[FPointCount] ;

    memcpy (FPoints , pTempPnt , sizeof(TFPoint) * FPointCount) ;

    delete [] pTempPnt ;

    return true ;

}

float TPolyLineTracker::GetPointX(int _iNo)
{
    if(FPointCount <= _iNo) return 0 ;
    return FPoints[_iNo].x ;
}

float TPolyLineTracker::GetPointY(int _iNo)
{
    if(FPointCount <= _iNo) return 0 ;
    return FPoints[_iNo].y ;
}

float TPolyLineTracker::GetPointCount()
{
    return FPointCount ;

}

//event link
void __fastcall TPolyLineTracker::Paint(HDC _Dc , float _fScaleX , float _fScaleY , float _fOffsetX , float _fOffsetY)
{
    if(!FVisible) return ;

    CGdi * Gdi = new CGdi(_Dc);

    RECT rect ;
    rect.left   = (FLeft           ) * _fScaleX;
    rect.top    = (FTop            ) * _fScaleY;
    rect.right  = (FLeft + FWidth  ) * _fScaleX;
    rect.bottom = (FTop  + FHeight ) * _fScaleY;

    Gdi -> m_tPen.Color = clGray ;
    Gdi -> m_tPen.Width = LineWidth ;
    Gdi -> m_tPen.Style = PS_DOT ;
    Gdi -> Rect(false, rect, 10);
    Gdi -> m_tPen.Style = PS_SOLID ;


    Gdi -> m_tPen  .Color = LineColor ;
    Gdi -> m_tBrush.Color = PointColor;

    int x1 = rect.left  ; //* (_fScaleX ? _fScaleX : 1.0) ;
    int y1 = rect.top   ; //* (_fScaleY ? _fScaleY : 1.0) ;
    int x2 = rect.right ; //* (_fScaleX ? _fScaleX : 1.0) ;
    int y2 = rect.bottom; //* (_fScaleY ? _fScaleY : 1.0) ;
    int cx = x1+(x2-x1)*10/2/10;
    int cy = y1+(y2-y1)*10/2/10;

    char str[500];
    int rcx = x1 + (x2-x1)/2;
    int rcy = y1 + (y2-y1)/2;

    int iX1 ,iY1;
    int iX2 ,iY2;

    for(int i = 0 ; i < FPointCount - 1 ; i++) {
        iX1 = FPoints[i  ].x * _fScaleX ;
        iX2 = FPoints[i+1].x * _fScaleX ;

        iY1 = FPoints[i  ].y * _fScaleY ;
        iY2 = FPoints[i+1].y * _fScaleY ;

        Gdi -> Line(iX1 , iY1 , iX2 , iY2);
    }

    if(FFocused) {
        Gdi -> m_tPen  .Color = PointColor;
        Gdi -> m_tBrush.Color = PointColor;

        for(int i = 0 ; i < FPointCount ; i++) {
            iX1 = FPoints[i  ].x * _fScaleX ;
            iY1 = FPoints[i  ].y * _fScaleY ;
            Gdi -> Circle(true, iX1 , iY1 , FPointSize , 0 );
        }

        Gdi -> m_tText.Color = clFuchsia ;
        sprintf(str, "Point Count : %.1f"    , FPointCount);
        Gdi -> Text(x1+10, y2+ 5, str);
        sprintf(str, "Center X, Y : %.1f, %.1f", rcx, rcy   );
        Gdi -> Text(x1+10, y2+15, str);
        sprintf(str, "Width : %.1f"          , FWidth     );
        Gdi -> Text(x1+10, y2+25, str);
        sprintf(str, "Height : %.1f"         , FHeight    );
        Gdi -> Text(x1+10, y2+35, str);
    }

    Gdi -> m_tFont.Height = 15 ;
    sprintf(str, Caption.c_str() , x1, y1  );
    Gdi -> Text(x1+10, y1-20, str);

    delete Gdi ;
}

bool __fastcall TPolyLineTracker::TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    bool ret = false ;

    if (!FVisible || !FEnabled) return ret ;

    float fx = _fOffsetX + x / _fScaleX ;
    float fy = _fOffsetY + y / _fScaleY ;

    //float fx = x * _fScaleX ;
    //float fy = y * _fScaleY ;
    Stat.mx = fx ;
    Stat.my = fy ;


    float fLeft   = FLeft   * _fScaleX  ; //XScale ;
    float fTop    = FTop    * _fScaleY  ; //YScale ;
    float fWidth  = FWidth  * _fScaleX  ;
    float fHeight = FHeight * _fScaleY  ;

    float fRight  =(fLeft + fWidth ) ;
    float fBottom =(fTop  + fHeight) ;


    //int cx = (fLeft + fWidth *10/2/10) ;
    //int cy = (fTop  + fHeight*10/2/10) ;

    float fX ; //FX설리짱~~~~
    float fY ;

    for(int i = 0 ; i < FPointCount ; i++) {
        fX = FPoints[i].x * _fScaleX ;
        fY = FPoints[i].y * _fScaleY ;

        if (x > fX -PointSize && x < fX +PointSize && y > fY - PointSize && y < fY + PointSize ) {
            if(Shift.Contains(ssCtrl)) {
                InsertPoint(i);
                if(FOnChanged) FOnChanged() ;
                return true ;
            }
            else if(Shift.Contains(ssAlt)) {
                DeletePoint(i);
                if(FOnChanged) FOnChanged() ;
                return true ;
            }
            else {
                ret = true ;
                Stat.iClickNo = i;
                FFocused = true ;
                break ;
            }
        }
    }

//         if (x > fLeft -PointSize && x < fLeft +PointSize && y > fTop   -PointSize && y < fTop   +PointSize ) {ret = Stat.IsLTM  = true; FFocused = true ;}
//    else if (x > cx    -PointSize && x < cx    +PointSize && y > fTop   -PointSize && y < fTop   +PointSize ) {ret = Stat.IsCTM  = true; FFocused = true ;}
//    else if (x > fRight-PointSize && x < fRight+PointSize && y > fTop   -PointSize && y < fTop   +PointSize ) {ret = Stat.IsRTM  = true; FFocused = true ;}
//    else if (x > fRight-PointSize && x < fRight+PointSize && y > cy     -PointSize && y < cy     +PointSize ) {ret = Stat.IsRCM  = true; FFocused = true ;}
//    else if (x > fRight-PointSize && x < fRight+PointSize && y > fBottom-PointSize && y < fBottom+PointSize ) {ret = Stat.IsRBM  = true; FFocused = true ;}
//    else if (x > cx    -PointSize && x < cx    +PointSize && y > fBottom-PointSize && y < fBottom+PointSize ) {ret = Stat.IsCBM  = true; FFocused = true ;}
//    else if (x > fLeft -PointSize && x < fLeft +PointSize && y > fBottom-PointSize && y < fBottom+PointSize ) {ret = Stat.IsLBM  = true; FFocused = true ;}
//    else if (x > fLeft -PointSize && x < fLeft +PointSize && y > cy     -PointSize && y < cy     +PointSize ) {ret = Stat.IsLCM  = true; FFocused = true ;}
    if (Stat.iClickNo < 0 && x > fLeft +PointSize && x < fRight-PointSize && y > fTop   +PointSize && y < fBottom-PointSize ) {ret = Stat.IsMove = true; FFocused = true ;}
    else    return ret ;

    if(FOnChanged) FOnChanged() ;

    return ret ;
}

bool __fastcall TPolyLineTracker::TrackerMv(TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    if (!FVisible || !FEnabled ) return false;
    if (!Stat.IsLTM  && !Stat.IsCTM  && !Stat.IsRTM  &&
        !Stat.IsRCM  && !Stat.IsMove && !Stat.IsRBM  &&
        !Stat.IsCBM  && !Stat.IsLBM  && !Stat.IsLCM  && Stat.iClickNo < 0) return false;


    m_bMoved = true ;

    float fx = _fOffsetX + x / _fScaleX ;
    float fy = _fOffsetY + y / _fScaleY ;

    //Scaled Position.
    int iLeft   = (FLeft  ) ;
    int iTop    = (FTop   ) ;
    int iWidth  = (FWidth ) ;
    int iHeight = (FHeight) ;

    int iRight  = (iLeft + iWidth ) ;
    int iBottom = (iTop  + iHeight) ;

    int iX ;//= FPoints[Stat.iClickNo].x ;
    int iY ;//= FPoints[Stat.iClickNo].y ;

    int iMinX = ~(1<<(sizeof(int)*8-1));
    int iMaxX = 0 ;
    int iMinY = ~(1<<(sizeof(int)*8-1));
    int iMaxY = 0 ;

    if (Stat.iClickNo >=0 ) {
        iX = FPoints[Stat.iClickNo].x * _fScaleX ;
        iY = FPoints[Stat.iClickNo].y * _fScaleY ;

        iX += x-Stat.mx ;
        iY += y-Stat.my ;

        FPoints[Stat.iClickNo].x = iX / (_fScaleX ? _fScaleX : 1) ;
        FPoints[Stat.iClickNo].y = iY / (_fScaleY ? _fScaleY : 1) ;


    }
    else if (Stat.IsMove       ) {
        iLeft += x-Stat.mx ; iTop+= y-Stat.my ; iRight+= x-Stat.mx ; iBottom+=y-Stat.my ;
        for(int i = 0 ; i < FPointCount ; i++) {
            iX = FPoints[i].x * _fScaleX ;
            iY = FPoints[i].y * _fScaleY ;

            iX += x-Stat.mx ;
            iY += y-Stat.my ;

            FPoints[i].x = iX / (_fScaleX ? _fScaleX : 1) ;
            FPoints[i].y = iY / (_fScaleY ? _fScaleY : 1) ;

        }
    }

    for(int i = 0 ; i < FPointCount ; i++) {
        if(iMinX > FPoints[i].x) iMinX = FPoints[i].x ;
        if(iMaxX < FPoints[i].x) iMaxX = FPoints[i].x ;
        if(iMinY > FPoints[i].y) iMinY = FPoints[i].y ;
        if(iMaxY < FPoints[i].y) iMaxY = FPoints[i].y ;
    }

    Stat.mx = fx;
    Stat.my = fy;

    iLeft   = iMinX ;
    iTop    = iMinY ;
    iRight  = iMaxX ;
    iBottom = iMaxY ;

    iWidth  = iRight  - iLeft ; //* _fScaleX  ; * _fScaleX  ;
    iHeight = iBottom - iTop  ; //* _fScaleY  ; * _fScaleY  ;

    FLeft   = iLeft   ;
    FTop    = iTop    ;
    FWidth  = iWidth  ;
    FHeight = iHeight ;




    if(FOnChanged) FOnChanged() ;

    return true ;
}

void __fastcall TPolyLineTracker::TrackerUp(TMouseButton Button,TShiftState Shift)
{
    if (!FVisible || !FEnabled ) return ;
    if (!Stat.IsLTM  && !Stat.IsCTM  && !Stat.IsRTM  &&
        !Stat.IsRCM  && !Stat.IsMove && !Stat.IsRBM  &&
        !Stat.IsCBM  && !Stat.IsLBM  && !Stat.IsLCM  && Stat.iClickNo < 0) return ;

    memset(&Stat , 0 , sizeof(TStat));
    Stat.iClickNo = -1 ;

    if(FOnChanged) FOnChanged() ;
}


//Load Save.
void TPolyLineTracker::OnLoadSave(bool _bLoad,AnsiString _sFilePath , AnsiString _sCaption)
{


}
