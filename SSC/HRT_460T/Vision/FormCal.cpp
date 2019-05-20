//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormCal.h"
#include "UserFile.h"

//#include "SLogUnit.h"



#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmCal *FrmCal;
//----------------------------------------------------------------- ----------
__fastcall TFrmCal::TFrmCal(TComponent* Owner)
    : TForm(Owner)
{

}
//---------------------------------------------------------------------------
bool TFrmCal::Init()
{
    m_iDpCnt = 0 ;

    int m_iPreImgW = 0 ;
    int m_iPreImgH = 0 ;
    int m_iPrePanW = 0 ;
    int m_iPrePanH = 0 ;

    OriginalProc = pnDraw->WindowProc ;
    pnDraw->WindowProc = PanelProc    ;

    memset(&Stat , 0 , sizeof(TStat));

    m_pVision    = NULL ;
    m_pPkgCam    = NULL ;
    m_pCal       = NULL ;
    m_pActiveImg = NULL ;

    FrmMag       = NULL ;
    FrmPixel     = NULL ;
    FrmHisto     = NULL ;
    FrmValueList = NULL ;

    btGray  -> Down = true ;
    pnDraw -> Align = alClient ;

    RectTracker = new CTracker  (); RectTracker -> Caption = "Rect" ;
    LineTracker = new CTracker  (); LineTracker -> Caption = "Line" ;
    RectTracker -> TrackerType = CTracker::ttRect ;
    LineTracker -> TrackerType = CTracker::ttLine ;
    RectTracker -> LineColor = clLime ; RectTracker -> PointColor = clLime ; RectTracker -> Visible = false ;
    LineTracker -> LineColor = clLime ; LineTracker -> PointColor = clLime ; LineTracker -> Visible = false ;
    RectTracker -> SetOnPaint(OnPaintMsg); //혹시나 쓰레드에서 건드릴까봐 메세지 처리.
    LineTracker -> SetOnPaint(OnPaintMsg);


    m_dDispImgX   = 0 ;
    m_dDispImgY   = 0 ;

    m_dDispScaleX = 1.0 ;
    m_dDispScaleY = 1.0 ;

    return true ;
}

bool TFrmCal::Close()
{
    pnDraw->WindowProc = OriginalProc ;
    if(RectTracker) {delete RectTracker ; RectTracker = NULL ;}
    if(LineTracker) {delete LineTracker ; LineTracker = NULL ;}

    if(FrmMag       ) {delete FrmMag        ;FrmMag       =NULL;}
    if(FrmHisto     ) {delete FrmHisto      ;FrmHisto     =NULL;}
    if(FrmPixel     ) {delete FrmPixel      ;FrmPixel     =NULL;}
    if(FrmValueList ) {delete FrmValueList  ;FrmValueList =NULL;} //FrmValueList -> Close()

    return true ;

}

double TFrmCal::GetScaleX()
{
    return m_dDispScaleX ;
}

double TFrmCal::GetScaleY()
{
    return m_dDispScaleY ;
}

//---------------------------------------------------------------------------
void __fastcall TFrmCal::PanelProc(Messages::TMessage &msg)
{
    OriginalProc(msg);
    if (msg.Msg == WM_PAINT ) {
        OnPaint();
    }
}

void __fastcall TFrmCal::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;


    //CVisionBase::TTimeInfo Info = VISN[m_iVisnId]->GetTimeInfo();
    //
    //int a = Info.iInsp   ;
    //int b = Info.iImgCpy ;
    //int c = Info.iGrab   ;
    //
    //pnInspTime   -> Caption = AnsiString(Info.iInsp  ) + "ms(I)";
    //pnImgCpyTime -> Caption = AnsiString(Info.iImgCpy) + "ms(C)";
    //pnGrabTime   -> Caption = AnsiString(Info.iGrab  ) + "ms(G)";

    bool bStratch = btStratch -> Down ;

    btZoomOut  -> Enabled = !bStratch ;
    btZoomIn   -> Enabled = !bStratch ;
    btRealSize -> Enabled = !bStratch ;

    bool FrmHistoShowing = FrmHisto != NULL && FrmHisto -> Showing ;
    bool FrmPixelShowing = FrmPixel != NULL && FrmPixel -> Showing ;

    if(RectTracker -> Visible != FrmHistoShowing) {
        RectTracker -> Visible = FrmHistoShowing ;
    }

    if(LineTracker -> Visible != FrmPixelShowing) {
        LineTracker -> Visible = FrmPixelShowing ;
    }

    //Level
    btValueList  -> Enabled = m_iLevel >= 2 ;

    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmCal::btGrayClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Gray Button Click");
    pnThreshold -> Visible = btBinary -> Down ;
    OnPaint();


}
//---------------------------------------------------------------------------

void __fastcall TFrmCal::sbThreshChange(TObject *Sender)
{
    pnThreash -> Caption = sbThresh -> Position ;
    OnPaint();
}

//---------------------------------------------------------------------------
void __fastcall TFrmCal::pnDrawMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    //if(m_pVision->GetCrntImg()==NULL)return ;
    if(!m_pActiveImg || m_pActiveImg -> GetSize() == 0) return ;
    if(Button == mbLeft) {

        double dx = m_dDispImgX + X / m_dDispScaleX ;
        double dy = m_dDispImgY + Y / m_dDispScaleY ;

        Stat.iClickX = X ;
        Stat.iClickY = Y ;

        if(RectTracker  -> TrackerDw(Button, Shift , X,Y, GetScaleX() , GetScaleY(), m_dDispImgX , m_dDispImgY)) {Stat.bTrckrClick = true ; OnPaint();return ;}
        if(LineTracker  -> TrackerDw(Button, Shift , X,Y, GetScaleX() , GetScaleY(), m_dDispImgX , m_dDispImgY)) {Stat.bTrckrClick = true ; OnPaint();return ;}

        if(m_pCal       -> MouseDw(Button, Shift , X,Y, GetScaleX() , GetScaleY(), m_dDispImgX , m_dDispImgY)) {Stat.bTrckrClick = true ; OnPaint();return ;}


        //if(VISN[m_iVisnId]->OnMouseDw(Button, Shift , X,Y, GetScaleX() , GetScaleY(), m_dDispImgX , m_dDispImgY)) {Stat.bTrckrClick = true ; return ;}
        //if(!btStratch -> Down) {
            Stat.bImgClick = true ;
        //}

    }


}

void __fastcall TFrmCal::pnDrawMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{

    if(!m_pActiveImg || m_pActiveImg -> GetSize() == 0) return ;
    //if(pnDraw -> Width < RectTracker -> Width){
    //    return;
    //}
    //if((Stat.iClickX-X) / GetScaleX() > VISN[m_iVisnId]->GetImg()->GetWidth () - pnDraw -> Width  / GetScaleX()){
    //    return;
    //}

    bool bNeedRepaint = false ;

    bNeedRepaint |= RectTracker  -> TrackerMv(Shift , X,Y, GetScaleX() , GetScaleY(), m_dDispImgX , m_dDispImgY) ;
    bNeedRepaint |= LineTracker  -> TrackerMv(Shift , X,Y, GetScaleX() , GetScaleY(), m_dDispImgX , m_dDispImgY) ;

    bNeedRepaint |= m_pCal    -> MouseMv  (Shift , X,Y, GetScaleX() , GetScaleY(), m_dDispImgX , m_dDispImgY);

    if(Stat.bImgClick){
        //존나 헛갈림....판넬 넓이 높이 고려하여 이미지 스타트 X,Y 리밋 확인.
        //왼쪽과 아래쪽 리밋.

        double dMaxX = m_pActiveImg->GetWidth () - pnDraw -> Width  / GetScaleX() ;
        double dMaxY = m_pActiveImg->GetHeight() - pnDraw -> Height / GetScaleY() ;

        //판넬이 더큰경우 ex)640,480
        if(dMaxX <0) dMaxX = 0 ;
        if(dMaxY <0) dMaxY = 0 ;


        const double dMoveX = (Stat.iClickX-X) / GetScaleX() ;
        const double dMoveY = (Stat.iClickY-Y) / GetScaleY() ;

        if(0<=m_dDispImgX + dMoveX && m_dDispImgX + dMoveX <= dMaxX){
            m_dDispImgX += dMoveX ;
        }
        else if(0>m_dDispImgX + dMoveX) {
            m_dDispImgX = 0 ;
        }
        else if(m_dDispImgX + dMoveX > dMaxX) {
            m_dDispImgX = dMaxX ;
        }

        if(0<=m_dDispImgY + dMoveY && m_dDispImgY + dMoveY <= dMaxY){
            m_dDispImgY += dMoveY ;
        }
        else if(0>m_dDispImgY + dMoveY) {
            m_dDispImgY = 0 ;
        }
        else if(m_dDispImgY + dMoveY > dMaxY) {
            m_dDispImgY = dMaxY ;
        }
        Stat.iClickX = X ;
        Stat.iClickY = Y ;
        bNeedRepaint = true ;
    }

    if(bNeedRepaint)OnPaint();




    AnsiString sTemp  ;
    AnsiString sTemp2 ;


    //픽셀 디스플레이.
    if(GetScaleX() && GetScaleY()) {
        int iScaledX = m_dDispImgX + X/ GetScaleX() ;
        int iScaledY = m_dDispImgY + Y/ GetScaleY() ;
        //if( 0 < iScaledX && iScaledX < VISN[m_iVisnId]->GetImg()->GetWidth () * GetScaleX() &&
        //    0 < iScaledY && iScaledY < VISN[m_iVisnId]->GetImg()->GetHeight() * GetScaleY() ){
        if( 0 < iScaledX && iScaledX <= m_pActiveImg->GetWidth () &&
            0 < iScaledY && iScaledY <= m_pActiveImg->GetHeight() ){



                sTemp = "<"+(AnsiString)m_pActiveImg->GetWidth() + "." + (AnsiString)m_pActiveImg->GetHeight() + "." + (AnsiString)m_pActiveImg->GetImgBit()+"> " +
                        "("+(AnsiString)iScaledX + "." + (AnsiString)iScaledY + "=" ;

                if(m_pActiveImg->GetImgBit() == 8 ) sTemp2 = (AnsiString)m_pActiveImg->GetPixel(iScaledX,iScaledY)+")";
                if(m_pActiveImg->GetImgBit() == 24) sTemp2 = "R:"  + (AnsiString)m_pActiveImg->GetPixelR(iScaledX,iScaledY)+
                                                             " G:" + (AnsiString)m_pActiveImg->GetPixelG(iScaledX,iScaledY)+
                                                             " B:" + (AnsiString)m_pActiveImg->GetPixelB(iScaledX,iScaledY)+")";
                pnPixelInfo -> Caption = sTemp + sTemp2 ;

                int PxValue  = m_pActiveImg->GetPixel (iScaledX,iScaledY) ;
                AnsiString Gray ;
                int DeGray ;

            if(m_pActiveImg->GetImgBit() == 8 ) {
                Gray   = "0x" + IntToHex(PxValue,2) + IntToHex(PxValue,2) + IntToHex(PxValue,2);
                DeGray = StrToIntDef(Gray, 0);
                pnColor -> Color = DeGray ;
                if (PxValue <128) pnColor -> Font -> Color = clWhite ;
                else              pnColor -> Font -> Color = clBlack ;

                pnColor -> Caption = PxValue ;
            }
            if(m_pActiveImg->GetImgBit() == 24) {
                int PxValueR = m_pActiveImg->GetPixelR(iScaledX,iScaledY) ;
                int PxValueG = m_pActiveImg->GetPixelG(iScaledX,iScaledY) ;
                int PxValueB = m_pActiveImg->GetPixelB(iScaledX,iScaledY) ;

                Gray   = "0x" + IntToHex(PxValueB,2) + IntToHex(PxValueG,2) + IntToHex(PxValueR,2);
                DeGray = StrToIntDef(Gray, 0);
                pnColor -> Color = DeGray ;

                if((PxValueB + PxValueG + PxValueR)/3 <128) pnColor -> Font -> Color = clWhite ;
                else                                        pnColor -> Font -> Color = clBlack ;

                pnColor -> Caption = (PxValueB + PxValueG + PxValueR)/3 ;
            }
        }
    }



}

//---------------------------------------------------------------------------

void __fastcall TFrmCal::pnDrawMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(!m_pActiveImg || m_pActiveImg -> GetSize() == 0) return ;
    if(Button == mbLeft) {
        RectTracker  -> TrackerUp(Button, Shift);
        LineTracker  -> TrackerUp(Button, Shift);
        m_pCal->MouseUp(Button, Shift);
    }

    Stat.bTrckrClick = false ;
    Stat.bImgClick   = false ;

}
//---------------------------------------------------------------------------


void __fastcall TFrmCal::btRealRatioClick(TObject *Sender)
{                                                                        
    if(btRealRatio -> Down) {
        m_dDispScaleX = 1.0 ;
        m_dDispScaleY = 1.0 ;
    }
    else {
        int iImgW = m_pActiveImg->GetWidth ();
        int iImgH = m_pActiveImg->GetHeight();

        m_dDispImgX = 0 ;
        m_dDispImgY = 0 ;

        m_dDispScaleX = pnDraw->Width  / (double)iImgW ;
        m_dDispScaleY = pnDraw->Height / (double)iImgH ;
    }
    OnPaint();
}

/*
void TFrmImage::UpdateScale()
{
    //Trace("Operator", "Image Form_RealRatio Button Click");
    if(btRealRatio -> Down) {
        m_dDispScaleX = 1.0 ;
        m_dDispScaleY = 1.0 ;
    }
    else {
        int iImgW = m_pActiveImg->GetWidth ();
        int iImgH = m_pActiveImg->GetHeight();

        m_dDispImgX = 0 ;
        m_dDispImgY = 0 ;



        if(iImgW == 0.0 ||
           iImgH == 0.0 ) {
            m_dDispScaleX = 1.0 ;
            m_dDispScaleY = 1.0 ;
        }
        else {
            m_dDispScaleX = pnDraw->Width  / (double)iImgW ;
            m_dDispScaleY = pnDraw->Height / (double)iImgH ;

        }
    }
    //OnPaint();

}
*/

//---------------------------------------------------------------------------



void __fastcall TFrmCal::btZoomOutClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Zoom Out Button Click");
    m_dDispScaleX = m_dDispScaleX * 8/10.0 ;
    m_dDispScaleY = m_dDispScaleY * 8/10.0 ;
    OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmCal::btZoomInClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Zoom In Button Click");
    m_dDispScaleX = m_dDispScaleX * 10/8.0 ;
    m_dDispScaleY = m_dDispScaleY * 10/8.0 ;
    OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmCal::btRealSizeClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Real Size Button Click");
    m_dDispScaleX = 1.0;
    m_dDispScaleY = 1.0;
    OnPaint();
}
//---------------------------------------------------------------------------
void TFrmCal::OnPaintMsg()
{
    PostMessage(pnDraw->Handle , WM_PAINT , 0 , 0 );
}
void TFrmCal::OnPaint()
{
    //요거 주석 빼면 타이머 야매에서 리턴 된다.
    //if(g_iActivVisnId != m_iVisnId) return ;
    m_iDpCnt++;
    if(m_iDpCnt > 99) m_iDpCnt = 0 ;
    pnDisp -> Caption = m_iDpCnt ;

    if(!m_pActiveImg) return ;

    int iImgW = m_pActiveImg->GetWidth () ;
    int iImgH = m_pActiveImg->GetHeight() ;
    int iPanW = pnDraw -> Width  ;
    int iPanH = pnDraw -> Height ;


    if(iImgW == 0 || iImgH == 0) return ;

    if(iImgW != m_iPreImgW || iImgH != m_iPreImgH || m_iPrePanW != iPanW || m_iPrePanH != iPanH) {
         if(btRealRatio -> Down) {
         }
         else {
             m_iPreImgW = iImgW ;
             m_iPreImgH = iImgH ;

             m_iPrePanW = iPanW ;
             m_iPrePanH = iPanH ;


             m_dDispImgX = 0 ;
             m_dDispImgY = 0 ;

             m_dDispScaleX = iPanW / (double)iImgW ;
             m_dDispScaleY = iPanH / (double)iImgH ;
         }
    }


    int sx = m_dDispImgX ;
    int sy = m_dDispImgY ;

    int dw = GetScaleX() ? iPanW / GetScaleX() : iPanW;
    int dh = GetScaleY() ? iPanH / GetScaleY() : iPanH;


    // Create buffer with memory DC and bitmap, then clear it with background.
    HDC     hDc ;
    HDC     hMemDc ;
    HBITMAP hMemBm , hOldMemBm ;

    RECT    Rect ;
    hDc       = GetDC(pnDraw->Handle); //imDraw -> Canvas -> Handle ;  //GetDC(pnDraw->Handle);
    GetClipBox(hDc , &Rect) ; //DC의 갱신해야할 Rect를 받아온다.
    hMemDc    = CreateCompatibleDC(hDc);
    //hMemBm    = CreateCompatibleBitmap(hDc, iPanW , iPanH);///////////////////////////////////
    hMemBm    = CreateCompatibleBitmap(hDc, iPanW , iPanH);///////////////////////////////////
    hOldMemBm = SelectObject(hMemDc, hMemBm); //Old멤비트맵을 저장해 두는것은 현재 선택되어 있는 오프젝트는 삭제가 안된다. 그래서 삭제할때 월래 놈으로 돌려 놓고 삭제.


    //===========================================


    /*
         VISN[m_iVisnId]->Paint(hMemDc,iDpOptn,
                           0 , 0 , iPanW , iPanH ,
                           m_fDispImgX , m_fDispImgY , dw, dh,
                           GetScaleX() ,GetScaleY() ,sbThresh -> Position , miCntrLine -> Checked) ;
    */










     /*
     m_pCal->Paint(hMemDc,iDpOptn,
                      0 , 0 , iPanW , iPanH ,
                      m_dDispImgX , m_dDispImgY , dw, dh,
                      GetScaleX() ,GetScaleY() ,sbThresh -> Position , miCntrLine -> Checked) ;

     */


    CImage * pImg = m_pActiveImg;
         if(btGray   -> Down ) {pImg->ShowImgGrey  (hMemDc, 0 , 0 , iPanW , iPanH , m_dDispImgX , m_dDispImgY , dw, dh);}
    else if(btBinary -> Down ) {pImg->ShowImgBinary(hMemDc, 0 , 0 , iPanW , iPanH , m_dDispImgX , m_dDispImgY , dw, dh ,sbThresh -> Position);}
    else if(btFalse  -> Down ) {pImg->ShowImgFalse (hMemDc, 0 , 0 , iPanW , iPanH , m_dDispImgX , m_dDispImgY , dw, dh);}
    else if(btColor  -> Down ) {pImg->ShowImgColor (hMemDc, 0 , 0 , iPanW , iPanH , m_dDispImgX , m_dDispImgY , dw, dh);}
    else if(btRed    -> Down ) {pImg->ShowImgR     (hMemDc, 0 , 0 , iPanW , iPanH , m_dDispImgX , m_dDispImgY , dw, dh);}
    else if(btGreen  -> Down ) {pImg->ShowImgG     (hMemDc, 0 , 0 , iPanW , iPanH , m_dDispImgX , m_dDispImgY , dw, dh);}
    else if(btBlue   -> Down ) {pImg->ShowImgB     (hMemDc, 0 , 0 , iPanW , iPanH , m_dDispImgX , m_dDispImgY , dw, dh);}


    m_pCal -> PaintRslt(hMemDc , pImg->GetScaleX() , pImg->GetScaleY() , pImg->GetOffsetX() , pImg->GetOffsetY());


    if(btCenter -> Down) {
        CGdi * Gdi = new CGdi(hMemDc);
        int iCntrX = (iImgW /2.0 - m_dDispImgX) * GetScaleX() ;
        int iCntrY = (iImgH /2.0 - m_dDispImgY) * GetScaleY() ;
        Gdi -> m_tPen.Style = psDot  ;
        Gdi -> m_tPen.Color = clBlue ;
        Gdi -> LinePlus(iCntrX , iCntrY , 100);


        delete Gdi ;
    }


    m_pCal -> PaintTracker(hMemDc , GetScaleX() , GetScaleY() , m_dDispImgX , m_dDispImgY);


    RectTracker -> Paint(hMemDc, GetScaleX() , GetScaleY() , m_dDispImgX , m_dDispImgY);
    LineTracker -> Paint(hMemDc, GetScaleX() , GetScaleY() , m_dDispImgX , m_dDispImgY);

    int iRet ;
    iRet = BitBlt(hDc, 0 , 0 , iPanW, iPanH, hMemDc, 0, 0, SRCCOPY);



    // Delete object
    SelectObject(hMemDc,hOldMemBm);
    DeleteObject(hMemBm);
    DeleteDC(hMemDc);
    ReleaseDC(pnDraw->Handle,hDc);


}
//---------------------------------------------------------------------------
void __fastcall TFrmCal::FormShow(TObject *Sender)
{
    this -> Left   = 0 ;
    this -> Top    = 0 ;
    this -> Width  = 1280 ;
    this -> Height = 1024 ;

    tmUpdate -> Enabled = true ;

    edPxResolSmpl->Text = m_pCal -> GetPxResolSimp();
    edPxResolLenz->Text = m_pCal -> GetPxResolLenz();
    //m_pOriMsgWnd =  m_pCal -> GetSendMsgWnd();
    //m_pCal -> SetSendMsgWnd(pnDraw->Handle);
    OnPaint();

    UpdateTable();
}
//---------------------------------------------------------------------------









void __fastcall TFrmCal::pcParaChange(TObject *Sender)
{

    OnPaint();
}
//---------------------------------------------------------------------------



void __fastcall TFrmCal::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int a  ;

    a++;
}
//---------------------------------------------------------------------------











void __fastcall TFrmCal::btMagClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Mag Button Click");
    if(FrmMag) {
        delete FrmMag ;
    }
    FrmMag = new TFrmMag (this);
    FrmMag -> Show() ;        
}
//---------------------------------------------------------------------------

void __fastcall TFrmCal::btPixelClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Pixel Button Click");
    if(FrmPixel) {
        delete FrmPixel ;
    }
    FrmPixel = new TFrmPixel (this , m_pActiveImg  , LineTracker);
    FrmPixel -> Show() ;        
}
//---------------------------------------------------------------------------

void __fastcall TFrmCal::btHistoClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Histogram Button Click");
    if(FrmHisto) {
        delete FrmHisto ;
    }                                                         
    FrmHisto = new TFrmHisto (this , m_pActiveImg , RectTracker);
    FrmHisto -> Show();
}
//---------------------------------------------------------------------------








void __fastcall TFrmCal::btCenterClick(TObject *Sender)
{
    OnPaint();
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
void TFrmCal::SetMagView(double _dLeft , double _dTop , double _dWidth , double _dHeight)
{
    //if(_dLeft < 0) _dLeft = 0 ;
    //if(_dTop  < 0) _dTop  = 0 ;

    //if(_dLeft+_dWidth  >= m_pActiveImg->GetWidth ()) return ;
    //if(_dTop +_dHeight >= m_pActiveImg->GetHeight()) return ;

    double fApplyScale ; //화면에 적용될 스케일.
    double fImgOfsX ;    //화면에 적용될 X오프셑
    double fImgOfsY ;    //화면에 적용될 Y오프셑

    //입력된 사각형의 넓이대 높이 비.
    double fInputRatio = _dHeight / _dWidth ; //1이상이면 높이가 크고 1이하면 넓이가 크다.

    //화면에 뿌려줄 판넬의 넓이대 높이 비.
    double fPanelRatio = pnDraw -> Height / (double)pnDraw -> Width ; //1이상이면 높이가 크고 1이하면 넓이가 크다.

    if(fInputRatio > fPanelRatio) {
        fApplyScale = pnDraw -> Height / _dHeight ;
        fImgOfsX    = _dLeft - (pnDraw -> Width / fApplyScale - _dWidth)/2.0 ;
        fImgOfsY    = _dTop ;
    }
    else {
        fApplyScale = pnDraw -> Width / _dWidth ;
        fImgOfsY    = _dTop  - (pnDraw -> Height/ fApplyScale - _dHeight)/2.0 ;
        fImgOfsX    = _dLeft ;
    }

    if(fImgOfsX < 0) fImgOfsX = 0 ;
    if(fImgOfsY < 0) fImgOfsY = 0 ;

    if(fImgOfsX >= m_pActiveImg->GetWidth () - pnDraw -> Width  / fApplyScale) fImgOfsX = m_pActiveImg->GetWidth () - pnDraw -> Width  / fApplyScale - 1 ;
    if(fImgOfsY >= m_pActiveImg->GetHeight() - pnDraw -> Height / fApplyScale) fImgOfsY = m_pActiveImg->GetHeight() - pnDraw -> Height / fApplyScale - 1 ;


    SetViewMode(false);

    m_dDispImgX   = fImgOfsX ;
    m_dDispImgY   = fImgOfsY ;

    m_dDispScaleX = fApplyScale ;
    m_dDispScaleY = fApplyScale ;

    OnPaint();
}


void TFrmCal::SetViewMode(bool _bStratch)
{
    if(_bStratch) btStratch   -> Down = true ;
    else          btRealRatio -> Down = true ;

    if(btRealRatio -> Down) {
        m_dDispScaleX = 1.0 ;
        m_dDispScaleY = 1.0 ;
    }
    else {
        int iImgW = m_pActiveImg->GetWidth ();
        int iImgH = m_pActiveImg->GetHeight();

        m_dDispImgX = 0 ;
        m_dDispImgY = 0 ;

        m_dDispScaleX = pnDraw->Width  / (double)iImgW ;
        m_dDispScaleY = pnDraw->Height / (double)iImgH ;
    }
    OnPaint();
}


void __fastcall TFrmCal::FormClose(TObject *Sender, TCloseAction &Action)
{
     if(FrmValueList)FrmValueList -> Close();
     tmUpdate -> Enabled = false ;
     //m_pCal -> SetSendMsgWnd(m_pOriMsgWnd);
}
//---------------------------------------------------------------------------







void TFrmCal::UpdateTable()
{
    if(btMasterPara -> Down) {
        m_pCal -> UpdateMasterPara(true , vlPara);
    }
    else if(btLocalPara -> Down) {
        m_pCal -> UpdateLocalPara(true , vlPara);
    }
    else if(btCommonPara -> Down) {
        m_pCal -> UpdateCommonPara(true , vlPara);
    }

    OnPaint();
}
                          //
void __fastcall TFrmCal::sgPkgListClick(TObject *Sender)
{
    if(btMasterPara -> Down) {
        m_pCal -> UpdateMasterPara(false , vlPara);
    }
    else if(btLocalPara -> Down) {
        m_pCal -> UpdateLocalPara (false , vlPara);
    }
    else if(btCommonPara -> Down) {
        m_pCal -> UpdateCommonPara(false , vlPara);
    }
    btCommonPara -> Down = true ;
    UpdateTable() ;

}
//---------------------------------------------------------------------------


void __fastcall TFrmCal::btLocalParaClick(TObject *Sender)
{

    UpdateTable();
}
//---------------------------------------------------------------------------





















void __fastcall TFrmCal::btSaveParaClick(TObject *Sender)
{
    if(btMasterPara -> Down) {
        m_pCal -> UpdateMasterPara(false , vlPara);
    }
    else if(btLocalPara -> Down) {
        m_pCal -> UpdateLocalPara (false , vlPara);
    }
    else if(btCommonPara -> Down) {
        m_pCal -> UpdateCommonPara(false , vlPara);
    }

    m_pCal -> SetPxResolSimp(StrToFloatDef(edPxResolSmpl->Text,m_pCal -> GetPxResolSimp()));
    edPxResolSmpl->Text = m_pCal -> GetPxResolSimp();

    m_pCal -> SetPxResolSimp(StrToFloatDef(edPxResolSmpl->Text,m_pCal -> GetPxResolSimp()));

    m_pCal -> LoadSave(false,m_pVision->GetName());

//m_pVision->SetPxResol(m_pCal -> GetResol());.
}
//---------------------------------------------------------------------------

void __fastcall TFrmCal::btValueListClick(TObject *Sender)
{
    if(FrmValueList) {
        delete FrmValueList ;
        FrmValueList = NULL ;
    }

    FrmValueList = new TFrmValueList(this);

    FrmValueList -> SetValueList(m_pVision -> GetValueList());
    FrmValueList -> Show();
}
//---------------------------------------------------------------------------



void TFrmCal::SetLevel(int _iLevel)//0:oper 1,2: engineer
{
    m_iLevel = _iLevel ;
}
void __fastcall TFrmCal::btCloseClick(TObject *Sender)
{
    TForm::Close();
    //Close();
}
//---------------------------------------------------------------------------








void __fastcall TFrmCal::btGetCamImgClick(TObject *Sender)
{

    *m_pActiveImg = *m_pPkgCam -> GetCrntImg();

    m_pCal -> RsltClear();

    OnPaint();
}
//---------------------------------------------------------------------------


void __fastcall TFrmCal::btSimpleCalClick(TObject *Sender)
{

    m_pCal -> FindBlob(NULL);

    if(m_pCal->GetErrMsg() != "") ShowMessage(m_pCal->GetErrMsg());

    edPxResolSmpl -> Text = m_pCal->GetPxResolSimp();

    OnPaint();
}
//---------------------------------------------------------------------------


void __fastcall TFrmCal::btGetCalImgClick(TObject *Sender)
{
    m_pCal->GetCalImgFromImg(m_pActiveImg , m_pActiveImg);

    OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmCal::btLenzCalClick(TObject *Sender)
{
    m_pCal -> Run(NULL);

    if(m_pCal->GetErrMsg() != "") ShowMessage(m_pCal->GetErrMsg());

    edPxResolLenz -> Text = m_pCal->GetPxResolLenz();

    OnPaint();
}
//---------------------------------------------------------------------------

