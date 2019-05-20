//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormImage.h"
#include "FormJob.h"
//#include "UserFile.h"

#include "SLogUnit.h"





//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFrmImage *FrmImage;
//----------------------------------------------------------------- ----------
__fastcall TFrmImage::TFrmImage(TComponent* Owner)
    : TForm(Owner)
{
    static int iCnt = 0 ;
    m_iCnt = iCnt ;

    iCnt++;


}
//---------------------------------------------------------------------------
void TFrmImage::Init()
{

    m_iDpCnt = 0 ;

    int m_iPreImgW = 0 ;
    int m_iPreImgH = 0 ;
    int m_iPrePanW = 0 ;
    int m_iPrePanH = 0 ;


    m_pVision    = NULL ;
    //m_pActiveImg = NULL ;

    OriginalProc = pnDraw->WindowProc ;
    pnDraw->WindowProc = PanelProc    ;

    //pnDraw -> DoubleBuffered = true ;


    memset(&Stat , 0 , sizeof(TStat));

    FrmMag   = NULL ;
    FrmPixel = NULL ;
    FrmHisto = NULL ;
    Frm3d    = NULL ;

    FrmTrain = NULL ;


    btGray  -> Down = true ;

    pnDraw -> Align = alClient ;

    RectTracker = new CTracker  (); RectTracker -> Caption = "Rect" ;
    LineTracker = new CTracker  (); LineTracker -> Caption = "Line" ;

    RectTracker -> TrackerType = CTracker::ttRect ;
    LineTracker -> TrackerType = CTracker::ttLine ;

    //Test용.
    //RectTracker -> BandEnabled = true ; RectTracker -> RoiEnabled = true ;




    RectTracker -> LineColor = clLime ; RectTracker -> PointColor = clLime ; RectTracker -> Visible = false ;
    LineTracker -> LineColor = clLime ; LineTracker -> PointColor = clLime ; LineTracker -> Visible = false ;


    RectTracker-> SetOnPaint(OnPaint);
    LineTracker-> SetOnPaint(OnPaint);



    btColor -> Visible = false ;
    btRed   -> Visible = false ;
    btGreen -> Visible = false ;
    btBlue  -> Visible = false ;

    m_dDispImgX   = 0 ;
    m_dDispImgY   = 0 ;

    m_dDispScaleX = 1.0 ;
    m_dDispScaleY = 1.0 ;


}

void TFrmImage::Close()
{
    static bool _bFirst = true ;

    if(_bFirst) {
        _bFirst = false ;
    }

    pnDraw->WindowProc = OriginalProc ;
    delete RectTracker     ;
    delete LineTracker     ;

    if(FrmMag  ) { delete FrmMag   ;}
    if(FrmHisto) { delete FrmHisto ;}
    if(FrmPixel) { delete FrmPixel ;}
    if(Frm3d   ) { delete Frm3d    ;}

    if(FrmTrain) {FrmTrain -> Close(); delete FrmTrain ;}

    //Trace("formDestroy",("FormImage"+AnsiString(m_iVisnId)).c_str());


}

double TFrmImage::GetScaleX()
{
    return m_dDispScaleX ;
}

double TFrmImage::GetScaleY()
{
    return m_dDispScaleY ;
}

//---------------------------------------------------------------------------
void __fastcall TFrmImage::PanelProc(Messages::TMessage &msg)
{
    OriginalProc(msg);
    if (msg.Msg == WM_PAINT ) {
        OnPaint();
    }
}

void __fastcall TFrmImage::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;
    Trace("<@SETBUFF>", (String(m_iCnt+20) + "," + m_pVision->GetName() + "Timer1").c_str());


    pnInspTime -> Caption = String("G:") + m_pVision -> GetTimeInf().iGrab + "ms";
    pnGrabTime -> Caption = String("I:") + m_pVision -> GetTimeInf().iInsp + "ms";

    Trace("<@SETBUFF>", (String(m_iCnt+20) + "," + m_pVision->GetName() + "Timer2").c_str());

    bool bStratch = btStratch -> Down ;

    //btRed   -> Visible = b24Bit ;
    //btGreen -> Visible = b24Bit ;
    //btBlue  -> Visible = b24Bit ;

    btZoomOut  -> Enabled = !bStratch ;
    btZoomIn   -> Enabled = !bStratch ;
    btRealSize -> Enabled = !bStratch ;

    Trace("<@SETBUFF>", (String(m_iCnt+20) + "," + m_pVision->GetName() + "Timer3").c_str());

    bool FrmHistoShowing = FrmHisto != NULL && FrmHisto -> Showing ;
    bool FrmPixelShowing = FrmPixel != NULL && FrmPixel -> Showing ;

    if(RectTracker -> Visible != FrmHistoShowing) {
        RectTracker -> Visible = FrmHistoShowing ;
    }
    Trace("<@SETBUFF>", (String(m_iCnt+20) + "," + m_pVision->GetName() + "Timer4").c_str());

    if(LineTracker -> Visible != FrmPixelShowing) {
        LineTracker -> Visible = FrmPixelShowing ;
    }

    Trace("<@SETBUFF>", (String(m_iCnt+20) + "," + m_pVision->GetName() + "Timer5").c_str());

    //lbMenu -> Visible = g_iCrntLevel == olMaster ;

    //lbX -> Caption = sbDraw -> HorzScrollBar -> Position ;
    //lbY -> Caption = sbDraw -> VertScrollBar -> Position ;

    Trace("<@SETBUFF>", (String(m_iCnt+20) + "," + m_pVision->GetName() + "Timer6").c_str());
    //lbWidth  -> Caption = sbDraw -> ClientWidth  ;
    //lbHeight -> Caption = sbDraw -> ClientHeight ;

    btTrain -> Enabled = !m_bAutorun && m_iLevel > 0 ;


    Trace("<@SETBUFF>", (String(m_iCnt+20) + "," + m_pVision->GetName() + "Timer7").c_str());
    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btGrayClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Gray Button Click");
    Trace("Lock", (m_pVision->GetName() + "btGrayClick Stt").c_str());
    pnThreshold -> Visible = btBinary -> Down ;
    Trace("Lock", (m_pVision->GetName() + "btGrayClick 1").c_str());
    OnPaint();
    Trace("Lock", (m_pVision->GetName() + "btGrayClick End").c_str());


}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::sbThreshChange(TObject *Sender)
{
    Trace("Lock", (m_pVision->GetName() + "sbThreshChange Stt").c_str());
    pnThreash -> Caption = sbThresh -> Position ;
    OnPaint();

    Trace("Lock", (m_pVision->GetName() + "sbThreshChange End").c_str());
}

//---------------------------------------------------------------------------
void __fastcall TFrmImage::pnDrawMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    Trace("Lock", (m_pVision->GetName() + "pnDrawMouseDown Stt").c_str());
    if(m_pVision->GetCrntImg()==NULL)return ;
    if(Button == mbLeft) {

        double dx = m_dDispImgX + X / m_dDispScaleX ;
        double dy = m_dDispImgY + Y / m_dDispScaleY ;

        Trace("Lock", (m_pVision->GetName() + "pnDrawMouseDown 1").c_str());
        double ddd = m_pVision->GetCrntImg()->GetWidth ();
        double dd_ = GetScaleX() ;

        double dda = m_pVision->GetCrntImg()->GetHeight();
        double dds = GetScaleY() ;

        //if(dx >= ddd * dd_) return ;
        //if(dy >= dda * dds) return ;

        if(dx >= ddd) return ;
        if(dy >= dda) return ;

        Stat.iClickX = X ;
        Stat.iClickY = Y ;
        Trace("Lock", (m_pVision->GetName() + "pnDrawMouseDown 2").c_str());
        if(RectTracker  -> TrackerDw(Button, Shift , X,Y, GetScaleX() , GetScaleY(), m_dDispImgX , m_dDispImgY)) {Stat.bTrckrClick = true ; return ;}
        if(LineTracker  -> TrackerDw(Button, Shift , X,Y, GetScaleX() , GetScaleY(), m_dDispImgX , m_dDispImgY)) {Stat.bTrckrClick = true ; return ;}


        Stat.bImgClick = true ;

    }
    Trace("Lock", (m_pVision->GetName() + "pnDrawMouseDown End").c_str());


}

void __fastcall TFrmImage::pnDrawMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    //if(pnDraw -> Width < RectTracker -> Width){
    //    return;
    //}
    //if((Stat.iClickX-X) / GetScaleX() > m_pVision->GetCrntImg()->GetWidth () - pnDraw -> Width  / GetScaleX()){
    //    return;
    //}
    Trace("Lock", (m_pVision->GetName() + "pnDrawMouseMove Stt").c_str());
    bool bNeedRepaint = false ;

    bNeedRepaint |= RectTracker  -> TrackerMv(Shift , X,Y, GetScaleX() , GetScaleY(), m_dDispImgX , m_dDispImgY) ;
    bNeedRepaint |= LineTracker  -> TrackerMv(Shift , X,Y, GetScaleX() , GetScaleY(), m_dDispImgX , m_dDispImgY) ;

    if(Stat.bImgClick){
        //존나 헛갈림....판넬 넓이 높이 고려하여 이미지 스타트 X,Y 리밋 확인.
        //왼쪽과 아래쪽 리밋.
        Trace("Lock", (m_pVision->GetName() + "pnDrawMouseMove 1").c_str());
        double dMaxX = m_pVision->GetCrntImg()->GetWidth () - pnDraw -> Width  / GetScaleX() ;
        double dMaxY = m_pVision->GetCrntImg()->GetHeight() - pnDraw -> Height / GetScaleY() ;
        Trace("Lock", (m_pVision->GetName() + "pnDrawMouseMove 2").c_str());
        //판넬이 더큰경우 ex)640,480
        if(dMaxX <0) dMaxX = 0 ;
        if(dMaxY <0) dMaxY = 0 ;


        const double dMoveX = (Stat.iClickX-X) / GetScaleX() ;
        const double dMoveY = (Stat.iClickY-Y) / GetScaleY() ;
        Trace("Lock", (m_pVision->GetName() + "pnDrawMouseMove 3").c_str());
        if(0<=m_dDispImgX + dMoveX && m_dDispImgX + dMoveX <= dMaxX){
            m_dDispImgX += dMoveX ;
        }
        else if(0>m_dDispImgX + dMoveX) {
            m_dDispImgX = 0 ;
        }
        else if(m_dDispImgX + dMoveX > dMaxX) {
            m_dDispImgX = dMaxX ;
        }
        Trace("Lock", (m_pVision->GetName() + "pnDrawMouseMove 4").c_str());
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

    Trace("Lock", (m_pVision->GetName() + "pnDrawMouseMove OnPaint Stt").c_str());
    if(bNeedRepaint) OnPaint();
    Trace("Lock", (m_pVision->GetName() + "pnDrawMouseMove OnPaint End").c_str());



    AnsiString sTemp  ;
    AnsiString sTemp2 ;


    //픽셀 디스플레이.
    if(!m_pVision->GetCrntImg()) return ;
    if(GetScaleX() && GetScaleY()) {
        int iScaledX = m_dDispImgX + X/ GetScaleX() ;
        int iScaledY = m_dDispImgY + Y/ GetScaleY() ;
        //if( 0 < iScaledX && iScaledX < m_pVision->GetCrntImg()->GetWidth () * GetScaleX() &&
        //    0 < iScaledY && iScaledY < m_pVision->GetCrntImg()->GetHeight() * GetScaleY() ){
        if( 0 < iScaledX && iScaledX < m_pVision->GetCrntImg()->GetWidth () &&
            0 < iScaledY && iScaledY < m_pVision->GetCrntImg()->GetHeight() ){


                Trace("Lock", (m_pVision->GetName() + "pnDrawMouseMove 5").c_str());
                sTemp = "<"+(AnsiString)m_pVision->GetCrntImg()->GetWidth() + "." + (AnsiString)m_pVision->GetCrntImg()->GetHeight() + "." + (AnsiString)m_pVision->GetCrntImg()->GetImgBit()+"> " +
                        "("+(AnsiString)iScaledX + "." + (AnsiString)iScaledY + "=" ;

                if(m_pVision->GetCrntImg()->GetImgBit() == 8 ) sTemp2 = (AnsiString)m_pVision->GetCrntImg()->GetPixel(iScaledX,iScaledY)+")";
                if(m_pVision->GetCrntImg()->GetImgBit() == 24) sTemp2 = "R:" + (AnsiString)m_pVision->GetCrntImg()->GetPixelR(iScaledX,iScaledY)+
                                                                         " G:" + (AnsiString)m_pVision->GetCrntImg()->GetPixelG(iScaledX,iScaledY)+
                                                                         " B:" + (AnsiString)m_pVision->GetCrntImg()->GetPixelB(iScaledX,iScaledY)+")";
                pnPixelInfo -> Caption = sTemp + sTemp2 ;
                Trace("Lock", (m_pVision->GetName() + "pnDrawMouseMove 6").c_str());
                int PxValue  = m_pVision->GetCrntImg()->GetPixel (iScaledX,iScaledY) ;
                AnsiString Gray ;
                int DeGray ;

            if(m_pVision->GetCrntImg()->GetImgBit() == 8 ) {
                Gray   = "0x" + IntToHex(PxValue,2) + IntToHex(PxValue,2) + IntToHex(PxValue,2);
                DeGray = StrToIntDef(Gray, 0);
                pnColor -> Color = DeGray ;
                Trace("Lock", (m_pVision->GetName() + "pnDrawMouseMove 7").c_str());
                if (PxValue <128) pnColor -> Font -> Color = clWhite ;
                else              pnColor -> Font -> Color = clBlack ;
                Trace("Lock", (m_pVision->GetName() + "pnDrawMouseMove 8").c_str());

                pnColor -> Caption = PxValue ;
            }
            if(m_pVision->GetCrntImg()->GetImgBit() == 24) {
                int PxValueR = m_pVision->GetCrntImg()->GetPixelR(iScaledX,iScaledY) ;
                int PxValueG = m_pVision->GetCrntImg()->GetPixelG(iScaledX,iScaledY) ;
                int PxValueB = m_pVision->GetCrntImg()->GetPixelB(iScaledX,iScaledY) ;

                Gray   = "0x" + IntToHex(PxValueB,2) + IntToHex(PxValueG,2) + IntToHex(PxValueR,2);
                DeGray = StrToIntDef(Gray, 0);
                pnColor -> Color = DeGray ;

                if((PxValueB + PxValueG + PxValueR)/3 <128) pnColor -> Font -> Color = clWhite ;
                else                                        pnColor -> Font -> Color = clBlack ;

                pnColor -> Caption = (PxValueB + PxValueG + PxValueR)/3 ;
            }
        }
    }

    Trace("Lock", (m_pVision->GetName() + "pnDrawMouseMove End").c_str());

}

//---------------------------------------------------------------------------

void __fastcall TFrmImage::pnDrawMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    Trace("Lock", (m_pVision->GetName() + "pnDrawMouseUp Stt").c_str());
    if(Button == mbLeft) {
        RectTracker  -> TrackerUp(Button, Shift);
        LineTracker  -> TrackerUp(Button, Shift);

    }

    Stat.bTrckrClick = false ;
    Stat.bImgClick   = false ;
    Trace("Lock", (m_pVision->GetName() + "pnDrawMouseUp End").c_str());

}
//---------------------------------------------------------------------------


void __fastcall TFrmImage::btRealRatioClick(TObject *Sender)
{
    Trace("Lock", (m_pVision->GetName() + "btRealRatioClick Stt").c_str());
    if(btRealRatio -> Down) {
        m_dDispScaleX = 1.0 ;
        m_dDispScaleY = 1.0 ;
    }
    else {
        int iImgW = m_pVision->GetCrntImg()->GetWidth ();
        int iImgH = m_pVision->GetCrntImg()->GetHeight();

        m_dDispImgX = 0 ;
        m_dDispImgY = 0 ;

        m_dDispScaleX = pnDraw->Width  / (double)iImgW ;
        m_dDispScaleY = pnDraw->Height / (double)iImgH ;
    }
    OnPaint();
    Trace("Lock", (m_pVision->GetName() + "btRealRatioClick End").c_str());
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
        int iImgW = m_pVision->GetCrntImg()->GetWidth ();
        int iImgH = m_pVision->GetCrntImg()->GetHeight();

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

void __fastcall TFrmImage::btLoadClick(TObject *Sender)
{
    Trace("Lock", (m_pVision->GetName() + "btLoadClick Stt").c_str());
    //Trace("Operator", "Image Form_Load Button Click");
    if(!OpenPictureDialog1 -> Execute())return ;
    if(!m_pVision->GetCrntImg()->LoadBmp(OpenPictureDialog1 -> FileName.c_str())){
        ShowMessage(OpenPictureDialog1 -> FileName  +" 이미지 파일을 열수 없습니다.");
    }


    if(m_pVision->GetCrntImg()->GetImgBit() == 8) btGray  -> Down = true ;
    else                                          btColor -> Down = true ;
    pnThreshold -> Visible = btBinary -> Down ;

    btStratch -> Down = true ;


    OnPaint();
    Trace("Lock", (m_pVision->GetName() + "btLoadClick End").c_str());

}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btSaveClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Save Button Click");
    Trace("Lock", (m_pVision->GetName() + "btSaveClick Stt").c_str());
    if(!SavePictureDialog1 -> Execute()) return ;
    m_pVision->GetCrntImg()->SaveBmp(SavePictureDialog1 -> FileName.c_str()) ;
    Trace("Lock", (m_pVision->GetName() + "btSaveClick End").c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btZoomOutClick(TObject *Sender)
{
    Trace("Lock", (m_pVision->GetName() + "btZoomOutClick Stt").c_str());
    //Trace("Operator", "Image Form_Zoom Out Button Click");
    m_dDispScaleX = m_dDispScaleX * 8/10.0 ;
    m_dDispScaleY = m_dDispScaleY * 8/10.0 ;
    OnPaint();
    Trace("Lock", (m_pVision->GetName() + "btZoomOutClick End").c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btZoomInClick(TObject *Sender)
{
    Trace("Lock", (m_pVision->GetName() + "btZoomInClick Stt").c_str());
    //Trace("Operator", "Image Form_Zoom In Button Click");
    m_dDispScaleX = m_dDispScaleX * 10/8.0 ;
    m_dDispScaleY = m_dDispScaleY * 10/8.0 ;
    OnPaint();
    Trace("Lock", (m_pVision->GetName() + "btZoomInClick End").c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btRealSizeClick(TObject *Sender)
{
    Trace("Lock", (m_pVision->GetName() + "btRealSizeClick Stt").c_str());
    //Trace("Operator", "Image Form_Real Size Button Click");
    m_dDispScaleX = 1.0;
    m_dDispScaleY = 1.0;
    OnPaint();
    Trace("Lock", (m_pVision->GetName() + "btRealSizeClick End").c_str());
}
//---------------------------------------------------------------------------

void TFrmImage::OnPaint()
{
    Trace("<@SETBUFF>", (String(m_iCnt+30) + "," + m_pVision->GetName() + "OnPaint1").c_str());
    Trace("Lock", (m_pVision->GetName() + "OnPaint Stt").c_str());
    m_iDpCnt++;
    if(m_iDpCnt > 99) m_iDpCnt = 0 ;
    pnDisp -> Caption = m_iDpCnt ;

    Trace("Lock", (m_pVision->GetName() + "OnPaint 1").c_str());
    if(!m_pVision ||!m_pVision->GetCrntImg()) return ;

    int iImgW = m_pVision->GetCrntImg()->GetWidth () ;
    int iImgH = m_pVision->GetCrntImg()->GetHeight() ;
    int iPanW = pnDraw -> Width  ;
    int iPanH = pnDraw -> Height ;

    Trace("Lock", (m_pVision->GetName() + "OnPaint 2").c_str());
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

    Trace("Lock", (m_pVision->GetName() + "OnPaint 3").c_str());
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

    Trace("Lock", (m_pVision->GetName() + "OnPaint 4").c_str());
    //===========================================
    CImage * pImg = m_pVision->GetCrntImg() ; //m_pVision -> GetCrntImg();

         if(btGray   -> Down ) {pImg->ShowImgGrey  (hMemDc, 0 , 0 , iPanW , iPanH , m_dDispImgX , m_dDispImgY , dw, dh);}
    else if(btBinary -> Down ) {pImg->ShowImgBinary(hMemDc, 0 , 0 , iPanW , iPanH , m_dDispImgX , m_dDispImgY , dw, dh ,sbThresh -> Position);}
    else if(btFalse  -> Down ) {pImg->ShowImgFalse (hMemDc, 0 , 0 , iPanW , iPanH , m_dDispImgX , m_dDispImgY , dw, dh);}
    else if(btColor  -> Down ) {pImg->ShowImgColor (hMemDc, 0 , 0 , iPanW , iPanH , m_dDispImgX , m_dDispImgY , dw, dh);}
    else if(btRed    -> Down ) {pImg->ShowImgR     (hMemDc, 0 , 0 , iPanW , iPanH , m_dDispImgX , m_dDispImgY , dw, dh);}
    else if(btGreen  -> Down ) {pImg->ShowImgG     (hMemDc, 0 , 0 , iPanW , iPanH , m_dDispImgX , m_dDispImgY , dw, dh);}
    else if(btBlue   -> Down ) {pImg->ShowImgB     (hMemDc, 0 , 0 , iPanW , iPanH , m_dDispImgX , m_dDispImgY , dw, dh);}
    Trace("Lock", (m_pVision->GetName() + "OnPaint 5").c_str());

    if(!btNoRslt->Down) m_pVision -> PaintRsltAll(hMemDc , pImg->GetScaleX() , pImg->GetScaleY() , pImg->GetOffsetX() , pImg->GetOffsetY());


    if(btCenter -> Down) {
        CGdi * Gdi = new CGdi(hMemDc,pImg->GetScaleX(),pImg->GetScaleY(),pImg->GetOffsetX(),pImg->GetOffsetY());
        Gdi -> m_tPen.Style = psDot  ;
        Gdi -> m_tPen.Color = clBlue ;
        Gdi -> LinePlus(iImgW /2.0 , iImgH /2.0 , 100);
        delete Gdi ;
    }
    Trace("Lock", (m_pVision->GetName() + "OnPaint 6").c_str());

    //m_pVision -> PaintTracker(hMemDc , GetScaleX() , GetScaleY() , m_dDispImgX , m_dDispImgY);
    RectTracker -> Paint(hMemDc, GetScaleX() , GetScaleY() , m_dDispImgX , m_dDispImgY);
    LineTracker -> Paint(hMemDc, GetScaleX() , GetScaleY() , m_dDispImgX , m_dDispImgY);

    int iRet ;
    iRet = BitBlt(hDc, 0 , 0 , iPanW, iPanH, hMemDc, 0, 0, SRCCOPY);


    Trace("Lock", (m_pVision->GetName() + "OnPaint 7").c_str());
    // Delete object
    SelectObject(hMemDc,hOldMemBm);
    DeleteObject(hMemBm);
    DeleteDC(hMemDc);
    ReleaseDC(pnDraw->Handle,hDc);
    Trace("Lock", (m_pVision->GetName() + "OnPaint End").c_str());
    Trace("<@SETBUFF>", (String(m_iCnt+30) + "," + m_pVision->GetName() + "OnPaintEnd").c_str());

}
//---------------------------------------------------------------------------
void __fastcall TFrmImage::FormShow(TObject *Sender)
{
    Trace("Lock", (m_pVision->GetName() + "FormShow Stt").c_str());
    tmUpdate -> Enabled = true ;
    //m_pVision->GetCrntImg() = m_pVision -> GetCrntImg() ;
    m_pVision -> SetSendMsgWnd(pnDraw->Handle);
    OnPaint();
    Trace("Lock", (m_pVision->GetName() + "FormShow End").c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btGrabClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Grab Button Click");
    //VISN[m_iVisnId]->Grab();     \\
    Trace("Lock", (m_pVision->GetName() + "btGrabClick Stt").c_str());
    tmLive -> Enabled = false ;
    btLive -> Down    = false ;
    btLiveInsp -> Down    = false ;

    m_pVision -> Grab();
    Trace("Lock", (m_pVision->GetName() + "btGrabClick End").c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btInspClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Inspection Button Click");
    Trace("Lock", (m_pVision->GetName() + "btInspClick Stt").c_str());

    int iSel = 0 ; //pcPara -> ActivePageIndex ;


    double fMaxScore = 0.0 ;
    double fMaxX     = 0.0 ;
    double fMaxY     = 0.0 ;

    TPoint SttPnt , EndPnt ;
    bool bRslt ;

    btNoRslt -> Down = false ;

    m_pVision -> Insp();

    Trace("Lock", (m_pVision->GetName() + "btInspClick End").c_str());

    //VISN[m_iVisnId]->Inspection();
}
//---------------------------------------------------------------------------



void __fastcall TFrmImage::btLiveClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Live Button Click");
    //VISN[m_iVisnId]->Live(true);
    Trace("Lock", (m_pVision->GetName() + "btLiveClick Stt").c_str());
    tmLive -> Enabled = btLive -> Down ;
    Trace("Lock", (m_pVision->GetName() + "btLiveClick End").c_str());
}
//---------------------------------------------------------------------------









void __fastcall TFrmImage::lbMenuMouseEnter(TObject *Sender)
{
    Trace("Lock", (m_pVision->GetName() + "lbMenuMouseEnter Stt").c_str());
    lbMenu -> Color = clBlue ;
    lbMenu -> Font -> Color = clWhite ;
    Trace("Lock", (m_pVision->GetName() + "lbMenuMouseEnter End").c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::lbMenuMouseLeave(TObject *Sender)
{
    Trace("Lock", (m_pVision->GetName() + "lbMenuMouseLeave Stt").c_str());
    lbMenu -> Color = clBtnFace ;
    lbMenu -> Font -> Color = clBlack ;
    Trace("Lock", (m_pVision->GetName() + "lbMenuMouseLeave End").c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::lbMenuMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    Trace("Lock", (m_pVision->GetName() + "lbMenuMouseUp Stt").c_str());
    tagPOINT      tpPosition ;
    GetCursorPos(&tpPosition);

    puMenu -> Popup(tpPosition.x - X - lbMenu -> Left + pnDraw -> Left , tpPosition.y + Y + lbMenu -> Top + pnDraw -> Top) ;
    Trace("Lock", (m_pVision->GetName() + "lbMenuMouseUp End").c_str());
}
//---------------------------------------------------------------------------



void __fastcall TFrmImage::miCntrLineClick(TObject *Sender)
{
    Trace("Lock", (m_pVision->GetName() + "miCntrLineClick Stt").c_str());
    //Trace("Operator", "Image Form_Center Line Button Click");
    miCntrLine -> Checked = !miCntrLine -> Checked ;
    btCenter   -> Down = miCntrLine -> Checked ;

    OnPaint();
    Trace("Lock", (m_pVision->GetName() + "miCntrLineClick End").c_str());

}
//---------------------------------------------------------------------------




void __fastcall TFrmImage::btMagClick(TObject *Sender)
{
    Trace("Lock", (m_pVision->GetName() + "btMagClick Stt").c_str());
    //Trace("Operator", "Image Form_Mag Button Click");
    if(FrmMag) {
        delete FrmMag ;
    }
    FrmMag = new TFrmMag (this);
    FrmMag -> Show() ;
    Trace("Lock", (m_pVision->GetName() + "btMagClick End").c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btPixelClick(TObject *Sender)
{
    Trace("Lock", (m_pVision->GetName() + "btPixelClick Stt").c_str());
    //Trace("Operator", "Image Form_Pixel Button Click");
    if(FrmPixel) {
        delete FrmPixel ;
    }
    FrmPixel = new TFrmPixel (this , m_pVision->GetCrntImg()  , LineTracker);
    FrmPixel -> Show() ;
    Trace("Lock", (m_pVision->GetName() + "btPixelClick End").c_str());     
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btHistoClick(TObject *Sender)
{
    Trace("Lock", (m_pVision->GetName() + "btHistoClick Stt").c_str());
    //Trace("Operator", "Image Form_Histogram Button Click");
    if(FrmHisto) {
        delete FrmHisto ;
    }
    FrmHisto = new TFrmHisto (this , m_pVision->GetCrntImg() , RectTracker);
    FrmHisto -> Show();
    Trace("Lock", (m_pVision->GetName() + "btHistoClick End").c_str());
}
//---------------------------------------------------------------------------








void __fastcall TFrmImage::btCenterClick(TObject *Sender)
{
    miCntrLine -> Checked = !miCntrLine -> Checked ;

    btCenter   -> Down = miCntrLine -> Checked ;

    //Trace("Operator", "Image Form_Center Line Button Click");
    OnPaint();
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
void TFrmImage::SetMagView(double _dLeft , double _dTop , double _dWidth , double _dHeight)
{
    //if(_dLeft < 0) _dLeft = 0 ;
    //if(_dTop  < 0) _dTop  = 0 ;

    //if(_dLeft+_dWidth  >= m_pVision->GetCrntImg()->GetWidth ()) return ;
    //if(_dTop +_dHeight >= m_pVision->GetCrntImg()->GetHeight()) return ;

    double dApplyScale ; //화면에 적용될 스케일.
    double dImgOfsX ;    //화면에 적용될 X오프셑
    double dImgOfsY ;    //화면에 적용될 Y오프셑

    //입력된 사각형의 넓이대 높이 비.
    double fInputRatio = _dHeight / _dWidth ; //1이상이면 높이가 크고 1이하면 넓이가 크다.

    //화면에 뿌려줄 판넬의 넓이대 높이 비.
    double fPanelRatio = pnDraw -> Height / (double)pnDraw -> Width ; //1이상이면 높이가 크고 1이하면 넓이가 크다.

    if(fInputRatio > fPanelRatio) {
        dApplyScale = pnDraw -> Height / _dHeight ;
        dImgOfsX    = _dLeft - (pnDraw -> Width / dApplyScale - _dWidth)/2.0 ;
        dImgOfsY    = _dTop ;
    }
    else {
        dApplyScale = pnDraw -> Width / _dWidth ;
        dImgOfsY    = _dTop  - (pnDraw -> Height/ dApplyScale - _dHeight)/2.0 ;
        dImgOfsX    = _dLeft ;
    }

    if(dImgOfsX < 0) dImgOfsX = 0 ;
    if(dImgOfsY < 0) dImgOfsY = 0 ;

    if(dImgOfsX >= m_pVision->GetCrntImg()->GetWidth () - pnDraw -> Width  / dApplyScale) dImgOfsX = m_pVision->GetCrntImg()->GetWidth () - pnDraw -> Width  / dApplyScale - 1 ;
    if(dImgOfsY >= m_pVision->GetCrntImg()->GetHeight() - pnDraw -> Height / dApplyScale) dImgOfsY = m_pVision->GetCrntImg()->GetHeight() - pnDraw -> Height / dApplyScale - 1 ;


    SetViewMode(false);

    m_dDispImgX   = dImgOfsX ;
    m_dDispImgY   = dImgOfsY ;

    m_dDispScaleX = dApplyScale ;
    m_dDispScaleY = dApplyScale ;

    OnPaint();
}


void TFrmImage::SetViewMode(bool _bStratch)
{
    if(_bStratch) btStratch   -> Down = true ;
    else          btRealRatio -> Down = true ;

    if(btRealRatio -> Down) {
        m_dDispScaleX = 1.0 ;
        m_dDispScaleY = 1.0 ;
    }
    else {
        int iImgW = m_pVision->GetCrntImg()->GetWidth ();
        int iImgH = m_pVision->GetCrntImg()->GetHeight();

        m_dDispImgX = 0 ;
        m_dDispImgY = 0 ;

        m_dDispScaleX = pnDraw->Width  / (double)iImgW ;
        m_dDispScaleY = pnDraw->Height / (double)iImgH ;
    }
    OnPaint();
}


void __fastcall TFrmImage::btTrainClick(TObject *Sender)
{
    tmLive     -> Enabled = false ;
    btLive     -> Down    = false ;
    btLiveInsp -> Down    = false ;

    if(FrmTrain) {

        FrmTrain -> Close();
        delete FrmTrain ;

    }

    FrmTrain = new TFrmTrain (this);
    FrmTrain -> Init(); 
    FrmTrain -> SetVision(m_pVision);
    FrmTrain -> SetLevel(m_iLevel);

    FrmTrain -> Show();
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::tmLiveTimer(TObject *Sender)
{
    tmLive -> Enabled = false ;
    //if(!Pkg -> GetComment().Pos("Camera")) {
    //    return ;
    //}
    if(btLive -> Down) {
        if(m_pVision ->GetGrabEnd()) m_pVision -> Grab();
    }
    else {
        if(m_pVision ->GetGrabEnd() && m_pVision ->GetInspEnd()) m_pVision -> GrabInsp();
    }
    tmLive -> Enabled = true ;
}
//---------------------------------------------------------------------------

void TFrmImage::SetLive(bool _bLive)
{
    //트레인중에는 작동 안함.
    if(FrmTrain && FrmTrain->Showing) return ;

    btLive -> Down    = _bLive ;
    tmLive -> Enabled = _bLive ;
}
void TFrmImage::SetLevel(int _iLevel)
{
    m_iLevel = _iLevel ;
    //btTrain -> Enabled = _iLevel > 0 ;
}
void TFrmImage::SetAutorun(bool _bRun)
{
    m_bAutorun = _bRun ;

    if(_bRun) {
        if(FrmTrain && FrmTrain->Showing) FrmTrain -> TForm::Close();
        btLive     -> Down    = false ;
        btLiveInsp -> Down    = false ;
        tmLive     -> Enabled = false ;
    }

    btLive     -> Enabled = !_bRun ;
    btLiveInsp -> Enabled = !_bRun ;
    btInsp     -> Enabled = !_bRun ;
    btGrab     -> Enabled = !_bRun ;

}





void __fastcall TFrmImage::btDeviceClick(TObject *Sender)
{
    tmLive     -> Enabled = false ;
    btLive     -> Down    = false ;
    btLiveInsp -> Down    = false ;

    if(FrmJob) FrmJob -> Show() ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btNoRsltClick(TObject *Sender)
{
    OnPaint();        
}
//---------------------------------------------------------------------------




void __fastcall TFrmImage::btLiveInspClick(TObject *Sender)
{
    tmLive -> Enabled = btLiveInsp -> Down ;
    btNoRslt -> Down = false ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmImage::Button1Click(TObject *Sender)
{
    if(Frm3d) {
        delete Frm3d ;
    }                  //(TComponent* Owner , CImage * _pImage , CTracker * _pTracker , double * _pRefOstX , double * _pRefOstY)

    Frm3d = new TFrm3d (this , m_pVision->GetCrntImg() , RectTracker );
    Frm3d -> Show();
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::Button2Click(TObject *Sender)
{
    m_pVision->GetCrntImg()->Rotate(RectTracker->GetRect(),1.0 ) ;
}
//---------------------------------------------------------------------------

