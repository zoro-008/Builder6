//---------------------------------------------------------------------------

#include <stdio.h>

#pragma hdrstop
#include "Tracker.h"

#include "UserINI.h"
#include "SVMaths.h"








#pragma package(smart_init)

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

    FOffsetX       = 0 ;
    FOffsetY       = 0 ;

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

void __fastcall TTracker::SetOffsetX(int value)
{
    if(FOffsetX == value) return ;
    //if(value < 1) value = 1 ;
    FOffsetX = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TTracker::SetOffsetY(int value)
{
    if(FOffsetY == value) return ;
    //if(value < 1) value = 1 ;
    FOffsetY = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void TTracker::OnLoadSave(bool _bLoad,AnsiString _sFilePath,AnsiString _sCaption)
{
    TUserINI UserINI ;

    //Load
    if(_bLoad) {

        UserINI.Load(_sFilePath, _sCaption , "m_iRefX    ", m_iRefX    );
        UserINI.Load(_sFilePath, _sCaption , "m_iRefY    ", m_iRefY    );
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
        UserINI.Save(_sFilePath, _sCaption , "m_iRefX    ", m_iRefX    );
        UserINI.Save(_sFilePath, _sCaption , "m_iRefY    ", m_iRefY    );
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

    //TTracker();
    //TTracker::
    //TTracker::TTracker();



    memset(&Stat , 0 , sizeof(TStat));
}

TRectTracker::~TRectTracker()
{
}

void __fastcall TRectTracker::SetLeft(int value)
{
    if(value < 0) return ;
    FLeft = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TRectTracker::SetTop(int value)
{
    if(value < 0) return ;
    FTop = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TRectTracker::SetHeight(int value)
{
    if(value < 1) return ;
    FHeight = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TRectTracker::SetWidth(int value)
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


void __fastcall TRectTracker::SetBand(int value)
{
    if(value < 0) return ;
    if(FBandMax != 0 && value > FBandMax) value = FBandMax ;
    FBand = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TRectTracker::SetBandMax(int value)
{
    if(value < 0) return ;
    FBandMax = value ;
    if(FBandMax != 0 && value > FBandMax) FBand = FBandMax ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}




void __fastcall TRectTracker::SetBandEnable(bool value)
{
    if(FVisible == value) return ;

    FBandEnable = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TRectTracker::SetLeftOffset(int value)
{
    if(value < 0) return ;
    FLeftOffset = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TRectTracker::SetTopOffset(int value)
{
    if(value < 0) return ;
    FTopOffset = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TRectTracker::SetRightOffset(int value)
{
    if(value < 0) return ;
    FRightOffset = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TRectTracker::SetBottomOffset(int value)
{
    if(value < 0) return ;
    FBottomOffset = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}







int __fastcall TRectTracker::GetRight()
{
    return FLeft + FWidth ;
}

int __fastcall TRectTracker::GetBottom()
{
    return FTop + FHeight ;
}

int __fastcall TRectTracker::GetCenterX()
{
    return FLeft + (FWidth/2) ;

}

int __fastcall TRectTracker::GetCenterY()
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

TPoint __fastcall TRectTracker::GetLTPnt()
{
    TPoint Pnt ;
    Pnt.x = FLeft ;
    Pnt.y = FTop  ;

    return Pnt ;
}

TPoint __fastcall TRectTracker::GetRTPnt()
{
    TPoint Pnt ;
    Pnt.x = FLeft + FWidth  ;
    Pnt.y = FTop  ;

    return Pnt ;
}

TPoint __fastcall TRectTracker::GetLBPnt()
{
    TPoint Pnt ;
    Pnt.x = FLeft   ;
    Pnt.y = FTop + FHeight ;

    return Pnt ;

}

TPoint __fastcall TRectTracker::GetRBPnt()
{
    TPoint Pnt ;
    Pnt.x = FLeft + FWidth  ;
    Pnt.y = FTop + FHeight ;

    return Pnt ;

}



void __fastcall TRectTracker::Paint(HDC _Dc , float _fScaleX , float _fScaleY )
{
    if(!FVisible) return ;

    IGdi *Gdi = GDI_New(_Dc);

    Gdi -> SetScale(_fScaleX , _fScaleY);

    RECT rect ;
    rect.left   = FLeft   ;
    rect.top    = FTop    ;
    rect.right  = FLeft + FWidth  ;
    rect.bottom = FTop  + FHeight ;

    //rect.left   *= _fScaleX ;
    //rect.top    *= _fScaleY ;
    //rect.right  *= _fScaleX ;
    //rect.bottom *= _fScaleY ;

    if(FBandVisible && FFocused) {
        Gdi -> m_tPen.Style = psDot ;
        Gdi -> m_tPen.Color = clGray ;
        Gdi -> m_tPen.Width = 1  ;

        Gdi -> Rect(false, GetLTRect(), 0);
        Gdi -> Rect(false, GetTTRect(), 0);
        Gdi -> Rect(false, GetRTRect(), 0);
        Gdi -> Rect(false, GetBTRect(), 0);

        Gdi -> m_tPen.Style = psSolid ;

        Gdi -> m_tPen  .Color = PointColor;
        Gdi -> m_tBrush.Color = PointColor;

        int iBtmRectCntX = GetBTRect().Left + GetBTRect().Width()/2 ;
        int iBtmRectTopY = GetBTRect().Top ;

        if(FBandEnable) Gdi -> Rect(true, iBtmRectCntX-PointSize * 3, iBtmRectTopY-PointSize, iBtmRectCntX-PointSize, iBtmRectTopY+PointSize);
    }



    Gdi -> m_tPen.Color = LineColor ;//FEnabled ? LineColor : clGray ;
    Gdi -> m_tPen.Width = LineWidth ;
    Gdi -> Rect(false, rect, 10);

    Gdi -> m_tPen  .Color = PointColor;
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
        sprintf(str, "Start X, Y : %d, %d" , x1 , y1   );
        Gdi -> Text(x1+10, y2+5 , str);
        sprintf(str, "End X, Y : %d, %d"   , x2 , y2   );
        Gdi -> Text(x1+10, y2+15, str);
        sprintf(str, "Center X, Y : %d, %d", rcx, rcy  );
        Gdi -> Text(x1+10, y2+25, str);
        sprintf(str, "Width : %d"          , FWidth    );
        Gdi -> Text(x1+10, y2+35, str);
        sprintf(str, "Height : %d"         , FHeight   );
        Gdi -> Text(x1+10, y2+45, str);
    }

    Gdi -> m_tFont.Height = 15 ;
    sprintf(str, Caption.c_str() , x1, y1  );
    Gdi -> Text(x1+10, y1-20, str);

    GDI_Del(Gdi);
}

bool __fastcall TRectTracker::TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY )
{
    bool ret = false ;

    if (!FVisible || !FEnabled) return ret ;

    //float dx = x * _fScaleX ;
    //float dy = y * _fScaleY ;

    //Rect
    float dLeft   = FLeft   * _fScaleX  ; //XScale ;
    float dTop    = FTop    * _fScaleY  ; //YScale ;
    float dWidth  = FWidth  * _fScaleX  ;
    float dHeight = FHeight * _fScaleY  ;

    float dRight  =(dLeft + dWidth ) ;
    float dBottom =(dTop  + dHeight) ;


    //Band
    int iBtmRectCntX = GetBTRect().Left + GetBTRect().Width()/2 ;
    int iBtmRectTopY = GetBTRect().Top ;
    int iBCntX  = (iBtmRectCntX-PointSize * 2) * _fScaleX ;
    int iBCntY  =  iBtmRectTopY                * _fScaleY ;

    int cx = (dLeft + dWidth *10/2/10) ;
    int cy = (dTop  + dHeight*10/2/10) ;

    Stat.mx = x ;
    Stat.my = y ;


         if (x > dLeft  -PointSize && x < dLeft  +PointSize && y > dTop   -PointSize && y < dTop   +PointSize ) {ret = Stat.IsLTM  = true; FFocused = true ;}
    else if (x > cx     -PointSize && x < cx     +PointSize && y > dTop   -PointSize && y < dTop   +PointSize ) {ret = Stat.IsCTM  = true; FFocused = true ;}
    else if (x > dRight -PointSize && x < dRight +PointSize && y > dTop   -PointSize && y < dTop   +PointSize ) {ret = Stat.IsRTM  = true; FFocused = true ;}
    else if (x > dRight -PointSize && x < dRight +PointSize && y > cy     -PointSize && y < cy     +PointSize ) {ret = Stat.IsRCM  = true; FFocused = true ;}
    else if (x > dRight -PointSize && x < dRight +PointSize && y > dBottom-PointSize && y < dBottom+PointSize ) {ret = Stat.IsRBM  = true; FFocused = true ;}
    else if (x > cx     -PointSize && x < cx     +PointSize && y > dBottom-PointSize && y < dBottom+PointSize ) {ret = Stat.IsCBM  = true; FFocused = true ;}
    else if (x > dLeft  -PointSize && x < dLeft  +PointSize && y > dBottom-PointSize && y < dBottom+PointSize ) {ret = Stat.IsLBM  = true; FFocused = true ;}
    else if (x > dLeft  -PointSize && x < dLeft  +PointSize && y > cy     -PointSize && y < cy     +PointSize ) {ret = Stat.IsLCM  = true; FFocused = true ;}
    else if (x > iBCntX -PointSize && x < iBCntX +PointSize && y > iBCntY -PointSize && y < iBCntY +PointSize && FBandVisible && FBandEnable) {ret = Stat.IsBND  = true; FFocused = true ;}

    //여기부터.
    else if (x > dLeft  +PointSize && x < dRight -PointSize && y > dTop   +PointSize && y < dBottom-PointSize ) {ret = Stat.IsMove = true; FFocused = true ;}




    else    return ret ;

    if(FOnChanged) FOnChanged() ;

    return ret ;
}

bool __fastcall TRectTracker::TrackerMv(TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY )
{
    if (!FVisible || !FEnabled ) return false;
    if (!Stat.IsLTM  && !Stat.IsCTM  && !Stat.IsRTM  &&
        !Stat.IsRCM  && !Stat.IsMove && !Stat.IsRBM  &&
        !Stat.IsCBM  && !Stat.IsLBM  && !Stat.IsLCM  && !Stat.IsBND) return false;

    m_bMoved = true ;

    //Scaled Position.
    int iLeft   = (FLeft   * _fScaleX) + 0.5 ;
    int iTop    = (FTop    * _fScaleY) + 0.5 ;
    int iWidth  = (FWidth  * _fScaleX) + 0.5 ;
    int iHeight = (FHeight * _fScaleY) + 0.5 ;

    int iRight  = (iLeft + iWidth ) ;
    int iBottom = (iTop  + iHeight) ;


    //Band
    int iBtmRectCntX = GetBTRect().Left + GetBTRect().Width()/2 ;
    int iBtmRectTopY = GetBTRect().Top ;
    int iOriBCntX    = iBtmRectCntX-PointSize * 2 * _fScaleX +0.5;
    int iOriBCntY    = iBtmRectTopY               * _fScaleY +0.5;
    int iBCntY       = 0 ;


         if (Stat.IsLTM ) {iLeft  = x         ; iTop = y         ;                                          }
    else if (Stat.IsCTM ) {                     iTop = y         ;                                          }
    else if (Stat.IsRTM ) {                     iTop = y         ; iRight = x         ;                     }
    else if (Stat.IsRCM ) {                                        iRight = x         ;                     }
    else if (Stat.IsRBM ) {                                        iRight = x         ; iBottom = y        ;}
    else if (Stat.IsCBM ) {                                                             iBottom = y        ;}
    else if (Stat.IsLBM ) {iLeft  = x         ;                                         iBottom = y        ;}
    else if (Stat.IsLCM ) {iLeft  = x         ;                                                             }
    else if (Stat.IsBND ) {                     iBCntY = y       ;                                          }
    else if (Stat.IsMove) {iLeft += x-Stat.mx ; iTop+= y-Stat.my ; iRight+= x-Stat.mx ; iBottom+=y-Stat.my ;}

    Stat.mx = x;
    Stat.my = y;

    iWidth  = iRight  - iLeft ; //* _fScaleX  ; * _fScaleX  ;
    iHeight = iBottom - iTop  ; //* _fScaleY  ; * _fScaleY  ;

    FLeft   = iLeft   / (_fScaleX ? _fScaleX : 1) + 0.5 ;
    FTop    = iTop    / (_fScaleY ? _fScaleY : 1) + 0.5 ;
    FWidth  = iWidth  / (_fScaleX ? _fScaleX : 1) + 0.5 ;
    FHeight = iHeight / (_fScaleY ? _fScaleY : 1) + 0.5 ;

    if(Stat.IsBND)FBand   += iOriBCntY - iBCntY ;
    if(FBand > FHeight / 2) FBand = FHeight / 2 ;
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

    //TTracker::TTracker();


    memset(&Stat , 0 , sizeof(TStat));
}

TOutBandRectTracker::~TOutBandRectTracker()
{
}

void __fastcall TOutBandRectTracker::SetLeft(int value)
{
    if(value < 0) return ;
    FLeft = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TOutBandRectTracker::SetTop(int value)
{
    if(value < 0) return ;
    FTop = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TOutBandRectTracker::SetHeight(int value)
{
    if(value < 1) value = 1 ; //return ;
    if(FHeightMax != 0 && value > FHeightMax) value = FHeightMax ;
    FHeight = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TOutBandRectTracker::SetWidth(int value)
{
    if(value < 1) value = 1 ; //return ;
    if(FWidthMax != 0 && value > FWidthMax) value = FWidthMax ;
    FWidth = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TOutBandRectTracker::SetWidthMax(int value)
{
    if(value < 0) return ;
    FWidthMax = value ;
    if(FWidth > FWidthMax && FWidthMax != 0) FWidth = FWidthMax ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TOutBandRectTracker::SetHeightMax(int value)
{
    if(value < 0) return ;
    FHeightMax = value ;
    if(FHeight > FHeightMax && FHeightMax != 0) FHeight = FHeightMax ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}



void __fastcall TOutBandRectTracker::SetBand(int value)
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

void __fastcall TOutBandRectTracker::SetBandMax(int value)
{
    if(value < 0) return ;
    FBandMax = value ;
    if(FBand > FBandMax && FBandMax != 0) FBand = FBandMax ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}







int __fastcall TOutBandRectTracker::GetRight()
{
    return FLeft + FWidth ;
}

int __fastcall TOutBandRectTracker::GetBottom()
{
    return FTop + FHeight ;
}

int __fastcall TOutBandRectTracker::GetCenterX()
{
    return FLeft + (FWidth/2) ;

}

int __fastcall TOutBandRectTracker::GetCenterY()
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

TRect __fastcall TOutBandRectTracker::GetOutRect()
{
    TRect Rect ;
    Rect.left   = FLeft       - FBand ;
    Rect.right  = GetRight()  + FBand ;
    Rect.top    = FTop        - FBand ;
    Rect.bottom = GetBottom() + FBand ;

    return Rect ;

}

TPoint __fastcall TOutBandRectTracker::GetLTPnt()
{
    TPoint Pnt ;
    Pnt.x = FLeft ;
    Pnt.y = FTop  ;

    return Pnt ;
}

TPoint __fastcall TOutBandRectTracker::GetRTPnt()
{
    TPoint Pnt ;
    Pnt.x = FLeft + FWidth  ;
    Pnt.y = FTop  ;

    return Pnt ;
}

TPoint __fastcall TOutBandRectTracker::GetLBPnt()
{
    TPoint Pnt ;
    Pnt.x = FLeft   ;
    Pnt.y = FTop + FHeight ;

    return Pnt ;

}

TPoint __fastcall TOutBandRectTracker::GetRBPnt()
{
    TPoint Pnt ;
    Pnt.x = FLeft + FWidth  ;
    Pnt.y = FTop + FHeight ;

    return Pnt ;

}



void __fastcall TOutBandRectTracker::Paint(HDC _Dc , float _fScaleX , float _fScaleY )
{
    if(!FVisible) return ;

    IGdi *Gdi = GDI_New(_Dc);

    Gdi -> SetScale(_fScaleX , _fScaleY);

    RECT rect ;
    rect.left   = FLeft   ;
    rect.top    = FTop    ;
    rect.right  = FLeft + FWidth  ;
    rect.bottom = FTop  + FHeight ;

    //rect.left   *= _fScaleX ;
    //rect.top    *= _fScaleY ;
    //rect.right  *= _fScaleX ;
    //rect.bottom *= _fScaleY ;

    //Band
    Gdi -> m_tPen.Style = psSolid  ;
    Gdi -> m_tPen.Color = clSilver ;
    Gdi -> m_tPen.Width = 1  ;
    Gdi -> Rect(false, GetOutRect(), 0);
    Gdi -> m_tPen.Style = psSolid ;

    if(FFocused && FBandEnable) {
        Gdi -> m_tPen  .Color = PointColor;
        Gdi -> m_tBrush.Color = PointColor;

        int iBtmRectCntX = GetOutRect().Left + GetOutRect().Width()/2 ;
        int iBtmRectBtmY = GetOutRect().Bottom ;

        Gdi -> Rect(true, iBtmRectCntX-PointSize * 3, iBtmRectBtmY-PointSize, iBtmRectCntX-PointSize, iBtmRectBtmY+PointSize);
    }



    Gdi -> m_tPen.Color = LineColor ;//FEnabled ? LineColor : clGray ;
    Gdi -> m_tPen.Width = LineWidth ;
    Gdi -> Rect(false, rect, 10);

    Gdi -> m_tPen  .Color = PointColor;
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
        sprintf(str, "Start X, Y : %d, %d" , x1 , y1   );
        Gdi -> Text(x1+10, y2+5 , str);
        sprintf(str, "End X, Y : %d, %d"   , x2 , y2   );
        Gdi -> Text(x1+10, y2+15, str);
        sprintf(str, "Center X, Y : %d, %d", rcx, rcy  );
        Gdi -> Text(x1+10, y2+25, str);
        sprintf(str, "Width : %d"          , FWidth    );
        Gdi -> Text(x1+10, y2+35, str);
        sprintf(str, "Height : %d"         , FHeight   );
        Gdi -> Text(x1+10, y2+45, str);
    }

    Gdi -> m_tFont.Height = 15 ;
    sprintf(str, Caption.c_str() , x1, y1  );
    Gdi -> Text(x1+10, y1-20, str);

    GDI_Del(Gdi);
}

bool __fastcall TOutBandRectTracker::TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY )
{
    bool ret = false ;

    if (!FVisible || !FEnabled) return ret ;

    //float dx = x * _fScaleX ;
    //float dy = y * _fScaleY ;

    //Rect
    float dLeft   = FLeft   * _fScaleX  ; //XScale ;
    float dTop    = FTop    * _fScaleY  ; //YScale ;
    float dWidth  = FWidth  * _fScaleX  ;
    float dHeight = FHeight * _fScaleY  ;

    float dRight  =(dLeft + dWidth ) ;
    float dBottom =(dTop  + dHeight) ;

    //Band
    int iBandRectCntX = GetOutRect().Left + GetOutRect().Width()/2 ;
    int iBandRectTopY = GetOutRect().Bottom ;
    int iBCntX  = (iBandRectCntX-PointSize * 2) * _fScaleX ;
    int iBCntY  =  iBandRectTopY                * _fScaleY ;

    int cx = (dLeft + dWidth *10/2/10) ;
    int cy = (dTop  + dHeight*10/2/10) ;

    Stat.mx = x ;
    Stat.my = y ;

         if (x > dLeft  -PointSize && x < dLeft  +PointSize && y > dTop   -PointSize && y < dTop   +PointSize ) {ret = Stat.IsLTM  = true; FFocused = true ;}
    else if (x > cx     -PointSize && x < cx     +PointSize && y > dTop   -PointSize && y < dTop   +PointSize ) {ret = Stat.IsCTM  = true; FFocused = true ;}
    else if (x > dRight -PointSize && x < dRight +PointSize && y > dTop   -PointSize && y < dTop   +PointSize ) {ret = Stat.IsRTM  = true; FFocused = true ;}
    else if (x > dRight -PointSize && x < dRight +PointSize && y > cy     -PointSize && y < cy     +PointSize ) {ret = Stat.IsRCM  = true; FFocused = true ;}
    else if (x > dRight -PointSize && x < dRight +PointSize && y > dBottom-PointSize && y < dBottom+PointSize ) {ret = Stat.IsRBM  = true; FFocused = true ;}
    else if (x > cx     -PointSize && x < cx     +PointSize && y > dBottom-PointSize && y < dBottom+PointSize ) {ret = Stat.IsCBM  = true; FFocused = true ;}
    else if (x > dLeft  -PointSize && x < dLeft  +PointSize && y > dBottom-PointSize && y < dBottom+PointSize ) {ret = Stat.IsLBM  = true; FFocused = true ;}
    else if (x > dLeft  -PointSize && x < dLeft  +PointSize && y > cy     -PointSize && y < cy     +PointSize ) {ret = Stat.IsLCM  = true; FFocused = true ;}
    else if (x > iBCntX -PointSize && x < iBCntX +PointSize && y > iBCntY -PointSize && y < iBCntY +PointSize && FBandEnable) {ret = Stat.IsBND  = true; FFocused = true ;}

    //여기부터.
    else if (x > dLeft  +PointSize && x < dRight -PointSize && y > dTop   +PointSize && y < dBottom-PointSize ) {ret = Stat.IsMove = true; FFocused = true ;}
    else    return ret ;

    if(FOnChanged) FOnChanged() ;

    return ret ;
}

bool __fastcall TOutBandRectTracker::TrackerMv(TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY )
{
    if (!FVisible || !FEnabled ) return false;
    if (!Stat.IsLTM  && !Stat.IsCTM  && !Stat.IsRTM  &&
        !Stat.IsRCM  && !Stat.IsMove && !Stat.IsRBM  &&
        !Stat.IsCBM  && !Stat.IsLBM  && !Stat.IsLCM  && !Stat.IsBND) return false;

    m_bMoved = true ;

    //Scaled Position.
    int iLeft   = (FLeft   * _fScaleX) + 0.5 ;
    int iTop    = (FTop    * _fScaleY) + 0.5 ;
    int iWidth  = (FWidth  * _fScaleX) + 0.5 ;
    int iHeight = (FHeight * _fScaleY) + 0.5 ;

    int iRight  = (iLeft + iWidth ) ;
    int iBottom = (iTop  + iHeight) ;


    //Band
    int iBandRectCntX = GetOutRect().Left + GetOutRect().Width()/2 ;
    int iBandRectTopY = GetOutRect().Bottom ;
    int iOriBCntX    = iBandRectCntX-PointSize * 2 * _fScaleX +0.5;
    int iOriBCntY    = iBandRectTopY               * _fScaleY +0.5;
    int iBCntY       = 0 ;


         if (Stat.IsLTM ) {iLeft  = x         ; iTop = y         ;                                          }
    else if (Stat.IsCTM ) {                     iTop = y         ;                                          }
    else if (Stat.IsRTM ) {                     iTop = y         ; iRight = x         ;                     }
    else if (Stat.IsRCM ) {                                        iRight = x         ;                     }
    else if (Stat.IsRBM ) {                                        iRight = x         ; iBottom = y        ;}
    else if (Stat.IsCBM ) {                                                             iBottom = y        ;}
    else if (Stat.IsLBM ) {iLeft  = x         ;                                         iBottom = y        ;}
    else if (Stat.IsLCM ) {iLeft  = x         ;                                                             }
    else if (Stat.IsBND ) {                     iBCntY = y       ;                                          }
    else if (Stat.IsMove) {iLeft += x-Stat.mx ; iTop+= y-Stat.my ; iRight+= x-Stat.mx ; iBottom+=y-Stat.my ;}

    Stat.mx = x;
    Stat.my = y;

    iWidth  = iRight  - iLeft ; //* _fScaleX  ; * _fScaleX  ;
    iHeight = iBottom - iTop  ; //* _fScaleY  ; * _fScaleY  ;

    FLeft   = iLeft   / (_fScaleX ? _fScaleX : 1) + 0.5 ;
    FTop    = iTop    / (_fScaleY ? _fScaleY : 1) + 0.5 ;
    FWidth  = iWidth  / (_fScaleX ? _fScaleX : 1) + 0.5 ;
    FHeight = iHeight / (_fScaleY ? _fScaleY : 1) + 0.5 ;

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
    FStartX = 1   ;
    FStartY = 1   ;
    FEndX   = 100 ;
    FEndY   = 100 ;
    
    //TTracker::TTracker();

    memset(&Stat , 0 , sizeof(TStat));
}

TLineTracker::~TLineTracker()
{
}

void __fastcall TLineTracker::SetStartX(int value)
{
    if(value < 0) return ;
    FStartX = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TLineTracker::SetStartY(int value)
{
    if(value < 0) return ;
    FStartY = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TLineTracker::SetEndX(int value)
{
    if(value < 1) return ;
    FEndX = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TLineTracker::SetEndY(int value)
{
    if(value < 1) return ;
    FEndY = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TLineTracker::Paint(HDC _Dc , float _fScaleX , float _fScaleY )
{
    if(!FVisible) return ;

    IGdi *Gdi = GDI_New(_Dc);

    Gdi -> SetScale(_fScaleX , _fScaleY);

    int iCx = (FStartX + FEndX)/ 2.0 ;
    int iCy = (FStartY + FEndY)/ 2.0 ;
    //int iScCx = iCx * _fScaleX + 0.5 ;
    //int iScCy = iCy * _fScaleY + 0.5 ;

    //int iScStartX = FStartX * _fScaleX + 0.5 ;
    //int iScStartY = FStartY * _fScaleY + 0.5 ;
    //int iScEndX   = FEndX   * _fScaleX + 0.5 ;
    //int iScEndY   = FEndY   * _fScaleY + 0.5 ;

    int iTextPntX = FStartY < FEndY ? FEndX : FStartX;//FStartY < FEndY ? iScEndX : iScStartX ;
    int iTextPntY = FStartY < FEndY ? FEndY : FStartY;//FStartY < FEndY ? iScEndY : iScStartY ;
    int iCaptPntX = FStartY < FEndY ? FStartX : FEndX;//FStartY < FEndY ? iScStartX : iScEndX ;
    int iCaptPntY = FStartY < FEndY ? FStartY : FEndY;//FStartY < FEndY ? iScStartY : iScEndY ;

    if(FFocused) {
        Gdi -> m_tPen  .Color = PointColor;
        Gdi -> m_tBrush.Color = PointColor;
        Gdi -> Circle(true , FEndX   , FEndY   , FPointSize , 0);
    }
    Gdi -> m_tPen.Color   = LineColor ; //FEnabled ? LineColor  : clGray ;
    Gdi -> m_tBrush.Color = PointColor; //FEnabled ? PointColor : clGray ;
    Gdi -> m_tPen.Width   = LineWidth ;
    Gdi -> LineAllow(FStartX , FStartY , FEndX , FEndY);
    if(FFocused) {
        Gdi -> m_tPen  .Color = PointColor;
        Gdi -> m_tBrush.Color = PointColor;
        Gdi -> Circle(true , FStartX , FStartY , FPointSize , 0);
        Gdi -> Rect  (true , iCx-FPointSize , iCy - FPointSize , iCx + FPointSize , iCy + FPointSize);
    }

    char str[500];
    Gdi -> m_tText.Color = clFuchsia ;
    sprintf(str, "Start X, Y : %d, %d" , FStartX , FStartY);
    Gdi -> Text(iTextPntX+10, iTextPntY+5 , str);
    sprintf(str, "End X, Y : %d, %d"   , FEndX   , FEndY  );
    Gdi -> Text(iTextPntX+10, iTextPntY+15, str);
    sprintf(str, "Angle : %0.2f"          , MATH_GetLineAngle(FStartX , FStartY , FEndX , FEndY));
    Gdi -> Text(iTextPntX+10, iTextPntY+25, str);
    sprintf(str, "Length : %0.2f"         , MATH_GetLineLenth(FStartX , FStartY , FEndX , FEndY));
    Gdi -> Text(iTextPntX+10, iTextPntY+35, str);

    Gdi -> m_tFont.Height = 15 ;
    sprintf(str, Caption.c_str() , iCaptPntX, iCaptPntY  );
    Gdi -> Text(iCaptPntX+10, iCaptPntY-20, str);

    GDI_Del(Gdi);



}

bool __fastcall TLineTracker::TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY )
{
    bool ret = false ;

    if (!FVisible || !FEnabled) return ret ;

    //float dx = x * _fScaleX ;
    //float dy = y * _fScaleY ;

    int iStartX = FStartX * _fScaleX + 0.5 ; //XScale ;
    int iStartY = FStartY * _fScaleY + 0.5 ; //YScale ;
    int iEndX   = FEndX   * _fScaleX + 0.5 ;
    int iEndY   = FEndY   * _fScaleY + 0.5 ;

    int iCx = (FStartX + FEndX) * _fScaleX / 2.0 + 0.5 ;
    int iCy = (FStartY + FEndY) * _fScaleY / 2.0 + 0.5 ;

    Stat.mx = x ;
    Stat.my = y ;


         if (x > iStartX -PointSize && x < iStartX +PointSize && y > iStartY -PointSize && y < iStartY +PointSize ) {ret = Stat.IsStt  = true; FFocused = true ;}
    else if (x > iEndX   -PointSize && x < iEndX   +PointSize && y > iEndY   -PointSize && y < iEndY   +PointSize ) {ret = Stat.IsEnd  = true; FFocused = true ;}
    else if (x > iCx     -PointSize && x < iCx     +PointSize && y > iCy     -PointSize && y < iCy     +PointSize ) {ret = Stat.IsMove = true; FFocused = true ;}
    else    return ret ;

    if(FOnChanged) FOnChanged() ;

    return ret ;
}

bool __fastcall TLineTracker::TrackerMv(TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY )
{
    if (!FVisible || !FEnabled ) return false;
    if (!Stat.IsStt  && !Stat.IsEnd  && !Stat.IsMove && !Stat.IsMove) return false;

    m_bMoved = true ;

    //Scaled Position.
    int iStartX = FStartX * _fScaleX + 0.5 ; //XScale ;
    int iStartY = FStartY * _fScaleY + 0.5 ; //YScale ;
    int iEndX   = FEndX   * _fScaleX + 0.5 ;
    int iEndY   = FEndY   * _fScaleY + 0.5 ;

         if (Stat.IsStt ) {iStartX  = x         ; iStartY = y         ;                                       }
    else if (Stat.IsEnd ) {iEndX    = x         ; iEndY   = y         ;                                       }
    else if (Stat.IsMove) {iStartX += x-Stat.mx ; iStartY+= y-Stat.my ; iEndX+= x-Stat.mx ; iEndY+=y-Stat.my ;}

    Stat.mx = x;
    Stat.my = y;

    FStartX = iStartX / (_fScaleX ? _fScaleX : 1) + 0.5 ;
    FStartY = iStartY / (_fScaleY ? _fScaleY : 1) + 0.5 ;
    FEndX   = iEndX   / (_fScaleX ? _fScaleX : 1) + 0.5 ;
    FEndY   = iEndY   / (_fScaleY ? _fScaleY : 1) + 0.5 ;

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
    FX      = 100 ;
    FY      = 100 ;
    FRadius = 100 ;

    //TTracker::TTracker();

    memset(&Stat , 0 , sizeof(TStat));
}

TCircleTracker::~TCircleTracker()
{

}

void __fastcall TCircleTracker::SetX(int value)
{
    if(value < 0) return ;
    FX = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TCircleTracker::SetY(int value)
{
    if(value < 0) return ;
    FY = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TCircleTracker::SetRadius(int value)
{
    if(value < 0) return ;
    FRadius = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

void __fastcall TCircleTracker::SetBand(int value)
{
    if(value < 0) return ;
    FBand = value ;
    if(FOnChanged&&FVisible) FOnChanged() ;
}

int __fastcall TCircleTracker::GetLeft()
{
    return FX-FRadius ;

}

int __fastcall TCircleTracker::GetTop()
{
    return FY-FRadius ;
}

int __fastcall TCircleTracker::GetRight()
{
    return FX+FRadius ;
}

int __fastcall TCircleTracker::GetBottom()
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



void __fastcall TCircleTracker::Paint(HDC _Dc , float _fScaleX , float _fScaleY)
{
    if(!FVisible) return ;

    IGdi *Gdi = GDI_New(_Dc);

    int iCx = FX * _fScaleX + 0.5 ;
    int iCy = FY * _fScaleY + 0.5 ;

    int iLeft   = (FX - FRadius) * _fScaleX + 0.5 ;
    int iTop    = (FY - FRadius) * _fScaleY + 0.5 ;
    int iRight  = (FX + FRadius) * _fScaleX + 0.5 ;
    int iBottom = (FY + FRadius) * _fScaleY + 0.5 ;

    int iOutLeft   = (FX - FRadius - FBand) * _fScaleX + 0.5 ;
    int iOutTop    = (FY - FRadius - FBand) * _fScaleY + 0.5 ;
    int iOutRight  = (FX + FRadius + FBand) * _fScaleX + 0.5 ;
    int iOutBottom = (FY + FRadius + FBand) * _fScaleY + 0.5 ;

    int iInLeft    = (FX - FRadius + FBand) * _fScaleX + 0.5 ;
    int iInTop     = (FY - FRadius + FBand) * _fScaleY + 0.5 ;
    int iInRight   = (FX + FRadius - FBand) * _fScaleX + 0.5 ;
    int iInBottom  = (FY + FRadius - FBand) * _fScaleY + 0.5 ;


    int iTextPntX = iCx     ;
    int iTextPntY = iBottom ;

    int iCaptPntX = iCx     ;
    int iCaptPntY = iTop    ;

    Gdi -> m_tPen.Color   = LineColor ;//FEnabled ? LineColor  : clGray ;
    Gdi -> m_tBrush.Color = PointColor;//FEnabled ? PointColor : clGray ;
    Gdi -> m_tPen.Width   = LineWidth ;
    Gdi -> Circle(false , iLeft, iTop, iRight, iBottom , 10);

    if(FBand) {
        Gdi -> m_tPen.Color   = clGray ;
        Gdi -> m_tPen.Style   = psDot  ;
        Gdi -> Circle(false , iOutLeft , iOutTop, iOutRight, iOutBottom , 0);
        Gdi -> Circle(false , iInLeft  , iInTop , iInRight , iInBottom  , 0);
        Gdi -> m_tPen.Style   = psSolid ;
    }

    if(FFocused) {
        Gdi -> m_tPen  .Color = PointColor;
        Gdi -> m_tBrush.Color = PointColor;
        Gdi -> Rect  (true , iLeft -FPointSize , iCy      - FPointSize , iLeft  + FPointSize , iCy      + FPointSize);
        Gdi -> Rect  (true , iCx   -FPointSize , iTop     - FPointSize , iCx    + FPointSize , iTop     + FPointSize);
        Gdi -> Rect  (true , iRight-FPointSize , iCy      - FPointSize , iRight + FPointSize , iCy      + FPointSize);
        Gdi -> Rect  (true , iCx   -FPointSize , iBottom  - FPointSize , iCx    + FPointSize , iBottom  + FPointSize);
    }

    char str[500];
    Gdi -> m_tText.Color = clFuchsia ;
    sprintf(str, "Center X, Y : %d, %d" , FX , FY);
    Gdi -> Text(iTextPntX+10, iTextPntY+5 , str);
    sprintf(str, "Radius : %d"   , FRadius   );
    Gdi -> Text(iTextPntX+10, iTextPntY+15, str);

    Gdi -> m_tFont.Height = 15 ;
    sprintf(str, Caption.c_str() , iCaptPntX, iCaptPntY  );
    Gdi -> Text(iCaptPntX+10, iCaptPntY-20, str);

    GDI_Del(Gdi);
}

bool __fastcall TCircleTracker::TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY)
{
    bool ret = false ;

    if (!FVisible || !FEnabled) return ret ;

    //float dx = x * _fScaleX ;
    //float dy = y * _fScaleY ;
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

    //반올림...
    int iCx     = fCx     + 0.5 ;
    int iCy     = fCy     + 0.5 ;
    int iLeft   = fLeft   + 0.5 ;
    int iTop    = fTop    + 0.5 ;
    int iRight  = fRight  + 0.5 ;
    int iBottom = fBottom + 0.5 ;

    Stat.mx = x ;
    Stat.my = y ;

         if (x > iCx   -(FPointSize+1) && x < iCx   +(FPointSize+1) && y > iTop   -(FPointSize+1) && y < iTop   +(FPointSize+1) ) {ret = Stat.IsTP   = true; FFocused = true ;}
    else if (x > iRight-(FPointSize+1) && x < iRight+(FPointSize+1) && y > iCy    -(FPointSize+1) && y < iCy    +(FPointSize+1) ) {ret = Stat.IsRT   = true; FFocused = true ;}
    else if (x > iCx   -(FPointSize+1) && x < iCx   +(FPointSize+1) && y > iBottom-(FPointSize+1) && y < iBottom+(FPointSize+1) ) {ret = Stat.IsBT   = true; FFocused = true ;}
    else if (x > iLeft -(FPointSize+1) && x < iLeft +(FPointSize+1) && y > iCy    -(FPointSize+1) && y < iCy    +(FPointSize+1) ) {ret = Stat.IsLT   = true; FFocused = true ;}
  //else if (x > iLeft +(FPointSize+1) && x < iRight-(FPointSize+1) && y > iTop   +(FPointSize+1) && y < iBottom-(FPointSize+1) ) {ret = Stat.IsMove = true; FFocused = true ;}
    else if (MATH_GetCircleInPoint(FX , FY , FRadius , fClickX , fClickY)                                       ) {ret = Stat.IsMove = true; FFocused = true ;}
    else    return ret ;

    if(FOnChanged) FOnChanged() ;

    return ret ;

}

bool __fastcall TCircleTracker::TrackerMv(TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY)
{
    if (!FVisible || !FEnabled ) return false;
    if (               !Stat.IsTP                 &&
        !Stat.IsLT  && !Stat.IsMove && !Stat.IsRT &&
                       !Stat.IsBT                 ) return false;

    m_bMoved = true ;


    float fCx = FX * _fScaleX ;
    float fCy = FY * _fScaleY ;

    float fLeft = GetLeft() * _fScaleX ;
    float fTop  = GetTop () * _fScaleY ;
    float fXRad = FRadius   * _fScaleX ;
    float fYRad = FRadius   * _fScaleY ;

    //반올림...
    int iCx   = fCx   + 0.5 ;
    int iCy   = fCy   + 0.5 ;
    int iXRad = fXRad + 0.5 ;
    int iYRad = fYRad + 0.5 ;

         if (Stat.IsTP  ) { iYRad -= y-Stat.my ; FRadius = iYRad / (_fScaleY ? _fScaleY : 1) + 0.5 ; }
    else if (Stat.IsLT  ) { iXRad -= x-Stat.mx ; FRadius = iXRad / (_fScaleX ? _fScaleX : 1) + 0.5 ; }
    else if (Stat.IsRT  ) { iXRad += x-Stat.mx ; FRadius = iXRad / (_fScaleX ? _fScaleX : 1) + 0.5 ; }
    else if (Stat.IsBT  ) { iYRad += y-Stat.my ; FRadius = iYRad / (_fScaleY ? _fScaleY : 1) + 0.5 ; }
    else if (Stat.IsMove) { iCx   += x-Stat.mx ; iCy+= y-Stat.my ;
                            FX = iCx / (_fScaleX ? _fScaleX : 1) + 0.5 ;
                            FY = iCy / (_fScaleY ? _fScaleY : 1) + 0.5 ;
                          }

    Stat.mx = x;
    Stat.my = y;


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

    //TTracker::TTracker();

    memset(&Stat , 0 , sizeof(TStat));
    Stat.iClickNo = -1 ;
    FPoints    = new TPoint[FPointCount];

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
    TPoint * pTempPnt = new TPoint[FPointCount] ;
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

    FPoints    = new TPoint[FPointCount] ;

    memcpy (FPoints , pTempPnt , sizeof(TPoint) * FPointCount) ;

    delete [] pTempPnt ;

    return true ;
}

bool TPolyTracker::DeletePoint(int _iNo)
{
    if(FPointCount < 4) return false ;

    FPointCount--;
    TPoint * pTempPnt = new TPoint[FPointCount] ;
    int iDeleted = 0 ;

    for(int i = 0 ; i < FPointCount ; i++){
        if(i==_iNo) {
            iDeleted = 1 ;
        }
        pTempPnt[i].x = FPoints[i+iDeleted].x ;
        pTempPnt[i].y = FPoints[i+iDeleted].y ;
    }

    if(FPoints   ) delete [] FPoints    ;

    FPoints    = new TPoint[FPointCount] ;

    memcpy (FPoints , pTempPnt , sizeof(TPoint) * FPointCount) ;

    delete [] pTempPnt ;

    return true ;

}

int TPolyTracker::GetPointX(int _iNo)
{
    if(FPointCount <= _iNo) return 0 ;
    return FPoints[_iNo].x ;
}

int TPolyTracker::GetPointY(int _iNo)
{
    if(FPointCount <= _iNo) return 0 ;
    return FPoints[_iNo].y ;
}

int TPolyTracker::GetPointCount()
{
    return FPointCount ;

}

//event link
void __fastcall TPolyTracker::Paint(HDC _Dc , float _fScaleX , float _fScaleY)
{
    if(!FVisible) return ;

    IGdi *Gdi = GDI_New(_Dc);

    RECT rect ;
    rect.left   = (FLeft           ) * _fScaleX + 0.5;
    rect.top    = (FTop            ) * _fScaleY + 0.5;
    rect.right  = (FLeft + FWidth  ) * _fScaleX + 0.5;
    rect.bottom = (FTop  + FHeight ) * _fScaleY + 0.5;

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
        iX1 = FPoints[i  ].x * _fScaleX + 0.5 ;
        iX2 = FPoints[i+1].x * _fScaleX + 0.5 ;

        iY1 = FPoints[i  ].y * _fScaleY + 0.5 ;
        iY2 = FPoints[i+1].y * _fScaleY + 0.5 ;

        Gdi -> Line(iX1 , iY1 , iX2 , iY2);
    }


    iX1 = FPoints[0            ].x * _fScaleX + 0.5 ;
    iX2 = FPoints[FPointCount-1].x * _fScaleX + 0.5 ;

    iY1 = FPoints[0            ].y * _fScaleY + 0.5 ;
    iY2 = FPoints[FPointCount-1].y * _fScaleY + 0.5 ;

    Gdi -> Line(iX1 , iY1 , iX2 , iY2);

    if(FFocused) {
        Gdi -> m_tPen  .Color = PointColor;
        Gdi -> m_tBrush.Color = PointColor;

        for(int i = 0 ; i < FPointCount ; i++) {
            iX1 = FPoints[i  ].x * _fScaleX + 0.5 ;
            iY1 = FPoints[i  ].y * _fScaleY + 0.5 ;
            Gdi -> Circle(true, iX1 , iY1 , FPointSize , 0 );
        }

        Gdi -> m_tText.Color = clFuchsia ;
        sprintf(str, "Point Count : %d"    , FPointCount);
        Gdi -> Text(x1+10, y2+ 5, str);
        sprintf(str, "Center X, Y : %d, %d", rcx, rcy   );
        Gdi -> Text(x1+10, y2+15, str);
        sprintf(str, "Width : %d"          , FWidth     );
        Gdi -> Text(x1+10, y2+25, str);
        sprintf(str, "Height : %d"         , FHeight    );
        Gdi -> Text(x1+10, y2+35, str);
    }

    Gdi -> m_tFont.Height = 15 ;
    sprintf(str, Caption.c_str() , x1, y1  );
    Gdi -> Text(x1+10, y1-20, str);

    GDI_Del(Gdi);
}

bool __fastcall TPolyTracker::TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY)
{
    bool ret = false ;

    if (!FVisible || !FEnabled) return ret ;

    //float dx = x * _fScaleX ;
    //float dy = y * _fScaleY ;
    Stat.mx = x ;
    Stat.my = y ;


    float fLeft   = FLeft   * _fScaleX  ; //XScale ;
    float fTop    = FTop    * _fScaleY  ; //YScale ;
    float fWidth  = FWidth  * _fScaleX  ;
    float fHeight = FHeight * _fScaleY  ;

    float fRight  =(fLeft + fWidth ) ;
    float fBottom =(fTop  + fHeight) ;


    //int cx = (dLeft + dWidth *10/2/10) ;
    //int cy = (dTop  + dHeight*10/2/10) ;

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

//         if (x > dLeft -PointSize && x < dLeft +PointSize && y > dTop   -PointSize && y < dTop   +PointSize ) {ret = Stat.IsLTM  = true; FFocused = true ;}
//    else if (x > cx    -PointSize && x < cx    +PointSize && y > dTop   -PointSize && y < dTop   +PointSize ) {ret = Stat.IsCTM  = true; FFocused = true ;}
//    else if (x > dRight-PointSize && x < dRight+PointSize && y > dTop   -PointSize && y < dTop   +PointSize ) {ret = Stat.IsRTM  = true; FFocused = true ;}
//    else if (x > dRight-PointSize && x < dRight+PointSize && y > cy     -PointSize && y < cy     +PointSize ) {ret = Stat.IsRCM  = true; FFocused = true ;}
//    else if (x > dRight-PointSize && x < dRight+PointSize && y > dBottom-PointSize && y < dBottom+PointSize ) {ret = Stat.IsRBM  = true; FFocused = true ;}
//    else if (x > cx    -PointSize && x < cx    +PointSize && y > dBottom-PointSize && y < dBottom+PointSize ) {ret = Stat.IsCBM  = true; FFocused = true ;}
//    else if (x > dLeft -PointSize && x < dLeft +PointSize && y > dBottom-PointSize && y < dBottom+PointSize ) {ret = Stat.IsLBM  = true; FFocused = true ;}
//    else if (x > dLeft -PointSize && x < dLeft +PointSize && y > cy     -PointSize && y < cy     +PointSize ) {ret = Stat.IsLCM  = true; FFocused = true ;}
    if (Stat.iClickNo < 0 && x > fLeft +PointSize && x < fRight-PointSize && y > fTop   +PointSize && y < fBottom-PointSize ) {ret = Stat.IsMove = true; FFocused = true ;}
    else    return ret ;

    if(FOnChanged) FOnChanged() ;

    return ret ;
}

bool __fastcall TPolyTracker::TrackerMv(TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY)
{
    if (!FVisible || !FEnabled ) return false;
    if (!Stat.IsLTM  && !Stat.IsCTM  && !Stat.IsRTM  &&
        !Stat.IsRCM  && !Stat.IsMove && !Stat.IsRBM  &&
        !Stat.IsCBM  && !Stat.IsLBM  && !Stat.IsLCM  && Stat.iClickNo < 0) return false;

    m_bMoved = true ;

    //Scaled Position.
    int iLeft   = (FLeft   * _fScaleX) + 0.5 ;
    int iTop    = (FTop    * _fScaleY) + 0.5 ;
    int iWidth  = (FWidth  * _fScaleX) + 0.5 ;
    int iHeight = (FHeight * _fScaleY) + 0.5 ;

    int iRight  = (iLeft + iWidth ) ;
    int iBottom = (iTop  + iHeight) ;

    int iX ;//= FPoints[Stat.iClickNo].x ;
    int iY ;//= FPoints[Stat.iClickNo].y ;

    int iMinX = ~(1<<(sizeof(int)*8-1));
    int iMaxX = 0 ;
    int iMinY = ~(1<<(sizeof(int)*8-1));
    int iMaxY = 0 ;

    if (Stat.iClickNo >=0 ) {
        iX = FPoints[Stat.iClickNo].x * _fScaleX + 0.5 ;
        iY = FPoints[Stat.iClickNo].y * _fScaleY + 0.5 ;

        iX += x-Stat.mx ;
        iY += y-Stat.my ;

        FPoints[Stat.iClickNo].x = iX / (_fScaleX ? _fScaleX : 1) + 0.5 ;
        FPoints[Stat.iClickNo].y = iY / (_fScaleY ? _fScaleY : 1) + 0.5 ;


    }
    else if (Stat.IsMove       ) {
        iLeft += x-Stat.mx ; iTop+= y-Stat.my ; iRight+= x-Stat.mx ; iBottom+=y-Stat.my ;
        for(int i = 0 ; i < FPointCount ; i++) {
            iX = FPoints[i].x * _fScaleX + 0.5 ;
            iY = FPoints[i].y * _fScaleY + 0.5 ;

            iX += x-Stat.mx ;
            iY += y-Stat.my ;

            FPoints[i].x = iX / (_fScaleX ? _fScaleX : 1) + 0.5 ;
            FPoints[i].y = iY / (_fScaleY ? _fScaleY : 1) + 0.5 ;

        }
    }

    for(int i = 0 ; i < FPointCount ; i++) {
        if(iMinX > FPoints[i].x) iMinX = FPoints[i].x ;
        if(iMaxX < FPoints[i].x) iMaxX = FPoints[i].x ;
        if(iMinY > FPoints[i].y) iMinY = FPoints[i].y ;
        if(iMaxY < FPoints[i].y) iMaxY = FPoints[i].y ;
    }

    Stat.mx = x;
    Stat.my = y;

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

    //TTracker::TTracker();

    memset(&Stat , 0 , sizeof(TStat));
    Stat.iClickNo = -1 ;
    FPoints    = new TPoint[FPointCount];

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
    TPoint * pTempPnt = new TPoint[FPointCount] ;
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

    FPoints    = new TPoint[FPointCount] ;

    memcpy (FPoints , pTempPnt , sizeof(TPoint) * FPointCount) ;

    delete [] pTempPnt ;

    return true ;
}

bool TPolyLineTracker::DeletePoint(int _iNo)
{
    if(FPointCount < 4) return false ;

    FPointCount--;
    TPoint * pTempPnt = new TPoint[FPointCount] ;
    int iDeleted = 0 ;

    for(int i = 0 ; i < FPointCount ; i++){
        if(i==_iNo) {
            iDeleted = 1 ;
        }
        pTempPnt[i].x = FPoints[i+iDeleted].x ;
        pTempPnt[i].y = FPoints[i+iDeleted].y ;
    }

    if(FPoints   ) delete [] FPoints    ;

    FPoints    = new TPoint[FPointCount] ;

    memcpy (FPoints , pTempPnt , sizeof(TPoint) * FPointCount) ;

    delete [] pTempPnt ;

    return true ;

}

int TPolyLineTracker::GetPointX(int _iNo)
{
    if(FPointCount <= _iNo) return 0 ;
    return FPoints[_iNo].x ;
}

int TPolyLineTracker::GetPointY(int _iNo)
{
    if(FPointCount <= _iNo) return 0 ;
    return FPoints[_iNo].y ;
}

int TPolyLineTracker::GetPointCount()
{
    return FPointCount ;

}

//event link
void __fastcall TPolyLineTracker::Paint(HDC _Dc , float _fScaleX , float _fScaleY)
{
    if(!FVisible) return ;

    IGdi *Gdi = GDI_New(_Dc);

    RECT rect ;
    rect.left   = (FLeft           ) * _fScaleX + 0.5;
    rect.top    = (FTop            ) * _fScaleY + 0.5;
    rect.right  = (FLeft + FWidth  ) * _fScaleX + 0.5;
    rect.bottom = (FTop  + FHeight ) * _fScaleY + 0.5;

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
        iX1 = FPoints[i  ].x * _fScaleX + 0.5 ;
        iX2 = FPoints[i+1].x * _fScaleX + 0.5 ;

        iY1 = FPoints[i  ].y * _fScaleY + 0.5 ;
        iY2 = FPoints[i+1].y * _fScaleY + 0.5 ;

        Gdi -> Line(iX1 , iY1 , iX2 , iY2);
    }


    //iX1 = FPoints[0            ].x * _fScaleX + 0.5 ;
    //iX2 = FPoints[FPointCount-1].x * _fScaleX + 0.5 ;
    //
    //iY1 = FPoints[0            ].y * _fScaleY + 0.5 ;
    //iY2 = FPoints[FPointCount-1].y * _fScaleY + 0.5 ;
    //
    //Gdi -> Line(iX1 , iY1 , iX2 , iY2);

    if(FFocused) {
        Gdi -> m_tPen  .Color = PointColor;
        Gdi -> m_tBrush.Color = PointColor;

        for(int i = 0 ; i < FPointCount ; i++) {
            iX1 = FPoints[i  ].x * _fScaleX + 0.5 ;
            iY1 = FPoints[i  ].y * _fScaleY + 0.5 ;
            Gdi -> Circle(true, iX1 , iY1 , FPointSize , 0 );
        }

        Gdi -> m_tText.Color = clFuchsia ;
        sprintf(str, "Point Count : %d"    , FPointCount);
        Gdi -> Text(x1+10, y2+ 5, str);
        sprintf(str, "Center X, Y : %d, %d", rcx, rcy   );
        Gdi -> Text(x1+10, y2+15, str);
        sprintf(str, "Width : %d"          , FWidth     );
        Gdi -> Text(x1+10, y2+25, str);
        sprintf(str, "Height : %d"         , FHeight    );
        Gdi -> Text(x1+10, y2+35, str);
    }

    Gdi -> m_tFont.Height = 15 ;
    sprintf(str, Caption.c_str() , x1, y1  );
    Gdi -> Text(x1+10, y1-20, str);

    GDI_Del(Gdi);
}

bool __fastcall TPolyLineTracker::TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY)
{
    bool ret = false ;

    if (!FVisible || !FEnabled) return ret ;

    //float dx = x * _fScaleX ;
    //float dy = y * _fScaleY ;
    Stat.mx = x ;
    Stat.my = y ;


    float fLeft   = FLeft   * _fScaleX  ; //XScale ;
    float fTop    = FTop    * _fScaleY  ; //YScale ;
    float fWidth  = FWidth  * _fScaleX  ;
    float fHeight = FHeight * _fScaleY  ;

    float fRight  =(fLeft + fWidth ) ;
    float fBottom =(fTop  + fHeight) ;


    //int cx = (dLeft + dWidth *10/2/10) ;
    //int cy = (dTop  + dHeight*10/2/10) ;

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

//         if (x > dLeft -PointSize && x < dLeft +PointSize && y > dTop   -PointSize && y < dTop   +PointSize ) {ret = Stat.IsLTM  = true; FFocused = true ;}
//    else if (x > cx    -PointSize && x < cx    +PointSize && y > dTop   -PointSize && y < dTop   +PointSize ) {ret = Stat.IsCTM  = true; FFocused = true ;}
//    else if (x > dRight-PointSize && x < dRight+PointSize && y > dTop   -PointSize && y < dTop   +PointSize ) {ret = Stat.IsRTM  = true; FFocused = true ;}
//    else if (x > dRight-PointSize && x < dRight+PointSize && y > cy     -PointSize && y < cy     +PointSize ) {ret = Stat.IsRCM  = true; FFocused = true ;}
//    else if (x > dRight-PointSize && x < dRight+PointSize && y > dBottom-PointSize && y < dBottom+PointSize ) {ret = Stat.IsRBM  = true; FFocused = true ;}
//    else if (x > cx    -PointSize && x < cx    +PointSize && y > dBottom-PointSize && y < dBottom+PointSize ) {ret = Stat.IsCBM  = true; FFocused = true ;}
//    else if (x > dLeft -PointSize && x < dLeft +PointSize && y > dBottom-PointSize && y < dBottom+PointSize ) {ret = Stat.IsLBM  = true; FFocused = true ;}
//    else if (x > dLeft -PointSize && x < dLeft +PointSize && y > cy     -PointSize && y < cy     +PointSize ) {ret = Stat.IsLCM  = true; FFocused = true ;}
    if (Stat.iClickNo < 0 && x > fLeft +PointSize && x < fRight-PointSize && y > fTop   +PointSize && y < fBottom-PointSize ) {ret = Stat.IsMove = true; FFocused = true ;}
    else    return ret ;

    if(FOnChanged) FOnChanged() ;

    return ret ;
}

bool __fastcall TPolyLineTracker::TrackerMv(TShiftState Shift ,int x, int y , float _fScaleX , float _fScaleY)
{
    if (!FVisible || !FEnabled ) return false;
    if (!Stat.IsLTM  && !Stat.IsCTM  && !Stat.IsRTM  &&
        !Stat.IsRCM  && !Stat.IsMove && !Stat.IsRBM  &&
        !Stat.IsCBM  && !Stat.IsLBM  && !Stat.IsLCM  && Stat.iClickNo < 0) return false;


    m_bMoved = true ;

    //Scaled Position.
    int iLeft   = (FLeft   * _fScaleX) + 0.5 ;
    int iTop    = (FTop    * _fScaleY) + 0.5 ;
    int iWidth  = (FWidth  * _fScaleX) + 0.5 ;
    int iHeight = (FHeight * _fScaleY) + 0.5 ;

    int iRight  = (iLeft + iWidth ) ;
    int iBottom = (iTop  + iHeight) ;

    int iX ;//= FPoints[Stat.iClickNo].x ;
    int iY ;//= FPoints[Stat.iClickNo].y ;

    int iMinX = ~(1<<(sizeof(int)*8-1));
    int iMaxX = 0 ;
    int iMinY = ~(1<<(sizeof(int)*8-1));
    int iMaxY = 0 ;

    if (Stat.iClickNo >=0 ) {
        iX = FPoints[Stat.iClickNo].x * _fScaleX + 0.5 ;
        iY = FPoints[Stat.iClickNo].y * _fScaleY + 0.5 ;

        iX += x-Stat.mx ;
        iY += y-Stat.my ;

        FPoints[Stat.iClickNo].x = iX / (_fScaleX ? _fScaleX : 1) + 0.5 ;
        FPoints[Stat.iClickNo].y = iY / (_fScaleY ? _fScaleY : 1) + 0.5 ;


    }
    else if (Stat.IsMove       ) {
        iLeft += x-Stat.mx ; iTop+= y-Stat.my ; iRight+= x-Stat.mx ; iBottom+=y-Stat.my ;
        for(int i = 0 ; i < FPointCount ; i++) {
            iX = FPoints[i].x * _fScaleX + 0.5 ;
            iY = FPoints[i].y * _fScaleY + 0.5 ;

            iX += x-Stat.mx ;
            iY += y-Stat.my ;

            FPoints[i].x = iX / (_fScaleX ? _fScaleX : 1) + 0.5 ;
            FPoints[i].y = iY / (_fScaleY ? _fScaleY : 1) + 0.5 ;

        }
    }

    for(int i = 0 ; i < FPointCount ; i++) {
        if(iMinX > FPoints[i].x) iMinX = FPoints[i].x ;
        if(iMaxX < FPoints[i].x) iMaxX = FPoints[i].x ;
        if(iMinY > FPoints[i].y) iMinY = FPoints[i].y ;
        if(iMaxY < FPoints[i].y) iMaxY = FPoints[i].y ;
    }

    Stat.mx = x;
    Stat.my = y;

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
