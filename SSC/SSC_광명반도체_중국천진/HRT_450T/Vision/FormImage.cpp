//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormImage.h"
#include "Form3d.h"
#include "FormAlgoPara.h"
#include "UserFile.h"

#include "VOptnMan.h"
#include "SLogUnit.h"



#define USE_PAINT_MSG

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmImage *FrmImage[MAX_VISN_ID];
//----------------------------------------------------------------- ----------
__fastcall TFrmImage::TFrmImage(TComponent* Owner)
    : TForm(Owner)
{
    static int iCnt = 0 ;
    m_iCnt = iCnt ;

    iCnt++;


}
//---------------------------------------------------------------------------
void TFrmImage::Init(EN_VISN_ID _iId)
{
    m_iVisnId = _iId ;

    m_iDpCnt = 0 ;

    int m_iPreImgW = 0 ;
    int m_iPreImgH = 0 ;
    int m_iPrePanW = 0 ;
    int m_iPrePanH = 0 ;




#ifdef USE_PAINT_MSG
    OriginalProc = pnDraw->WindowProc ;
    pnDraw->WindowProc = PanelProc    ;
#endif

    //pnDraw -> DoubleBuffered = true ;


    memset(&Stat , 0 , sizeof(TStat));

    FrmMag   = NULL ;
    FrmPixel = NULL ;
    FrmHisto = NULL ;

    FrmCamLightPara = NULL ; //new TFrmCamPara(this , iFrmNo);

    //SetFormEnable(false);

    btGray  -> Down = true ;

    pnDraw -> Align = alClient ;

    RectTracker = new TRectTracker  (); RectTracker -> Caption = "Rect" ;
    LineTracker = new TLineTracker  (); LineTracker -> Caption = "Line" ;

    RectTracker -> LineColor = clLime ; RectTracker -> PointColor = clLime ; RectTracker -> Visible = false ;
    LineTracker -> LineColor = clLime ; LineTracker -> PointColor = clLime ; LineTracker -> Visible = false ;


    RectTracker-> SetFunc(OnPaint);
    LineTracker-> SetFunc(OnPaint);

    //OnPaint();
    VISN[_iId]->SetPaintCallback(PaintCallback);


    bool b24Bit   = VISN[m_iVisnId]->GetImg()->GetImgBit() == 24 ;

    if(!b24Bit) {
        btColor -> Visible = b24Bit ;
        btRed   -> Visible = b24Bit ;
        btGreen -> Visible = b24Bit ;
        btBlue  -> Visible = b24Bit ;
    }

    m_fDispImgX   = 0 ;
    m_fDispImgY   = 0 ;

}

void TFrmImage::Close()
{
    static bool _bFirst = true ;

    if(_bFirst) {
        _bFirst = false ;
    }
    VISN[m_iVisnId]->Live(false);


#ifdef USE_PAINT_MSG
    pnDraw->WindowProc = OriginalProc ;
#endif
    delete RectTracker     ;
    delete LineTracker     ;

    if(FrmMag  ) {delete FrmMag   ;}
    if(FrmHisto) {delete FrmHisto ;}
    if(FrmPixel) {delete FrmPixel ;}

    //Trace("formDestroy",("FormImage"+AnsiString(m_iVisnId)).c_str());


}

void TFrmImage::PaintCallback()
{


    OnPaint();
}
float TFrmImage::GetScaleX()
{
    return m_fDispScaleX ;
}

float TFrmImage::GetScaleY()
{
    return m_fDispScaleY ;
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
    if(g_iActivVisnId != m_iVisnId) return ;

    tmUpdate -> Enabled = false ;

    CVisionBase::TTimeInfo Info = VISN[m_iVisnId]->GetTimeInfo();

    int a = Info.iInsp   ;
    int b = Info.iImgCpy ;
    int c = Info.iGrab   ;

    pnInspTime   -> Caption = AnsiString(Info.iInsp  ) + "ms(I)";
    pnImgCpyTime -> Caption = AnsiString(Info.iImgCpy) + "ms(C)";
    pnGrabTime   -> Caption = AnsiString(Info.iGrab  ) + "ms(G)";

    bool b24Bit   = VISN[m_iVisnId]->GetImg()->GetImgBit() == 24 ;
    bool bStratch = btStratch -> Down ;

    btColor -> Visible = b24Bit ;
    //btRed   -> Visible = b24Bit ;
    //btGreen -> Visible = b24Bit ;
    //btBlue  -> Visible = b24Bit ;

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

    lbMenu -> Visible = g_iCrntLevel == olMaster ;

    //lbX -> Caption = sbDraw -> HorzScrollBar -> Position ;
    //lbY -> Caption = sbDraw -> VertScrollBar -> Position ;


    //lbWidth  -> Caption = sbDraw -> ClientWidth  ;
    //lbHeight -> Caption = sbDraw -> ClientHeight ;


    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btGrayClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Gray Button Click");
    pnThreshold -> Visible = btBinary -> Down ;
    OnPaint();


}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::sbThreshChange(TObject *Sender)
{
    pnThreash -> Caption = sbThresh -> Position ;
    OnPaint();
}

//---------------------------------------------------------------------------
void __fastcall TFrmImage::pnDrawMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Button == mbLeft) {

        double dx = m_fDispImgX + X / m_fDispScaleX ;
        double dy = m_fDispImgY + Y / m_fDispScaleY ;

        double cx = VISN[m_iVisnId]->GetImg()->GetWidth () ;//* GetScaleX() ;
        double cy = VISN[m_iVisnId]->GetImg()->GetHeight() ;//* GetScaleY() ;

        if(dx >= cx) return ;
        if(dy >= cy) return ;


        Stat.iClickX = X ;
        Stat.iClickY = Y ;

        RectTracker     -> Focused = false ;
        LineTracker     -> Focused = false ;


        if(RectTracker  -> TrackerDw(Button, Shift , X,Y, GetScaleX() , GetScaleY(), m_fDispImgX , m_fDispImgY)) {Stat.bTrckrClick = true ; return ;}
        if(LineTracker  -> TrackerDw(Button, Shift , X,Y, GetScaleX() , GetScaleY(), m_fDispImgX , m_fDispImgY)) {Stat.bTrckrClick = true ; return ;}

        if(VISN[m_iVisnId]->OnMouseDw(Button, Shift , X,Y, GetScaleX() , GetScaleY(), m_fDispImgX , m_fDispImgY)) {Stat.bTrckrClick = true ; return ;}
        if(!btStratch -> Down) {
            Stat.bImgClick = true ;
        }

    }
}

void __fastcall TFrmImage::pnDrawMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    //static int iPreClickX = Stat.iClickX ;
    //static int iPreClickY = Stat.iClickY ;
    //
    //if(iPreClickX = Stat.iClickX || iPreClickY = Stat.iClickY) return

    if(Stat.bTrckrClick) {
        RectTracker  -> TrackerMv(Shift , X,Y, GetScaleX() , GetScaleY(), m_fDispImgX , m_fDispImgY) ;
        LineTracker  -> TrackerMv(Shift , X,Y, GetScaleX() , GetScaleY(), m_fDispImgX , m_fDispImgY) ;
        VISN[m_iVisnId]->OnMouseMv(Shift , X,Y, GetScaleX() , GetScaleY(), m_fDispImgX , m_fDispImgY) ;
    }
    else if(Stat.bImgClick){
        //존나 헛갈림....판넬 넓이 높이 고려하여 이미지 스타트 X,Y 리밋 확인.
        //왼쪽과 아래쪽 리밋.
        double dMaxX = VISN[m_iVisnId]->GetImg()->GetWidth () - pnDraw -> Width  / GetScaleX() ;
        double dMaxY = VISN[m_iVisnId]->GetImg()->GetHeight() - pnDraw -> Height / GetScaleY() ;

        //판넬이 더큰경우 ex)640,480
        if(dMaxX <0) dMaxX = 0 ;
        if(dMaxY <0) dMaxY = 0 ;


        const double dMoveX = (Stat.iClickX-X) / GetScaleX() ;
        const double dMoveY = (Stat.iClickY-Y) / GetScaleY() ;

        if(0<=m_fDispImgX + dMoveX && m_fDispImgX + dMoveX <= dMaxX){
            m_fDispImgX += dMoveX ;
        }
        else if(0>m_fDispImgX + dMoveX) {
            m_fDispImgX = 0 ;
        }
        else if(m_fDispImgX + dMoveX > dMaxX) {
            m_fDispImgX = dMaxX ;
        }

        if(0<=m_fDispImgY + dMoveY && m_fDispImgY + dMoveY <= dMaxY){
            m_fDispImgY += dMoveY ;
        }
        else if(0>m_fDispImgY + dMoveY) {
            m_fDispImgY = 0 ;
        }
        else if(m_fDispImgY + dMoveY > dMaxY) {
            m_fDispImgY = dMaxY ;
        }
        Stat.iClickX = X ;
        Stat.iClickY = Y ;
        OnPaint();
    }




    AnsiString sTemp  ;
    AnsiString sTemp2 ;


    //픽셀 디스플레이.
    if(!VISN[m_iVisnId]->GetImg()) return ;
    if(GetScaleX() && GetScaleY()) {
        int iScaledX = m_fDispImgX + X/ GetScaleX() ;
        int iScaledY = m_fDispImgY + Y/ GetScaleY() ;
        if( 0 < iScaledX && iScaledX < VISN[m_iVisnId]->GetImg()->GetWidth ()  &&
            0 < iScaledY && iScaledY < VISN[m_iVisnId]->GetImg()->GetHeight()  ){

                


                sTemp = "<"+(AnsiString)VISN[m_iVisnId]->GetImg()->GetWidth() + "." + (AnsiString)VISN[m_iVisnId]->GetImg()->GetHeight() + "." + (AnsiString)VISN[m_iVisnId]->GetImg()->GetImgBit()+"> " +
                        "("+(AnsiString)iScaledX + "." + (AnsiString)iScaledY + "=" ;

                if(VISN[m_iVisnId]->GetImg()->GetImgBit() == 8 ) sTemp2 = (AnsiString)VISN[m_iVisnId]->GetImg()->GetPixel(iScaledX,iScaledY)+")";
                if(VISN[m_iVisnId]->GetImg()->GetImgBit() == 24) sTemp2 = "R:" + (AnsiString)VISN[m_iVisnId]->GetImg()->GetPixelR(iScaledX,iScaledY)+
                                                                         " G:" + (AnsiString)VISN[m_iVisnId]->GetImg()->GetPixelG(iScaledX,iScaledY)+
                                                                         " B:" + (AnsiString)VISN[m_iVisnId]->GetImg()->GetPixelB(iScaledX,iScaledY)+")";
                pnPixelInfo -> Caption = sTemp + sTemp2 ;

                int PxValue  = VISN[m_iVisnId]->GetImg()->GetPixel (iScaledX,iScaledY) ;
                AnsiString Gray ;
                int DeGray ;

            if(VISN[m_iVisnId]->GetImg()->GetImgBit() == 8 ) {
                Gray   = "0x" + IntToHex(PxValue,2) + IntToHex(PxValue,2) + IntToHex(PxValue,2);
                DeGray = StrToIntDef(Gray, 0);
                pnColor -> Color = DeGray ;
                if (PxValue <128) pnColor -> Font -> Color = clWhite ;
                else              pnColor -> Font -> Color = clBlack ;

                pnColor -> Caption = PxValue ;
            }
            if(VISN[m_iVisnId]->GetImg()->GetImgBit() == 24) {
                int PxValueR = VISN[m_iVisnId]->GetImg()->GetPixelR(iScaledX,iScaledY) ;
                int PxValueG = VISN[m_iVisnId]->GetImg()->GetPixelG(iScaledX,iScaledY) ;
                int PxValueB = VISN[m_iVisnId]->GetImg()->GetPixelB(iScaledX,iScaledY) ;

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

void __fastcall TFrmImage::pnDrawMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Button == mbLeft) {
        RectTracker  -> TrackerUp(Button, Shift);
        LineTracker  -> TrackerUp(Button, Shift);
        VISN[m_iVisnId]->OnMouseUp(Button, Shift, X, Y , GetScaleX() , GetScaleY(),m_fDispImgX , m_fDispImgY);
    }

    Stat.bTrckrClick = false ;
    Stat.bImgClick   = false ;

}
//---------------------------------------------------------------------------


void __fastcall TFrmImage::btRealRatioClick(TObject *Sender)
{
    if(btRealRatio -> Down) {
        m_fDispScaleX = 1.0 ;
        m_fDispScaleY = 1.0 ;
    }
    else {
        int iImgW = VISN[m_iVisnId]->GetImg()->GetWidth ();
        int iImgH = VISN[m_iVisnId]->GetImg()->GetHeight();

        m_fDispImgX = 0 ;
        m_fDispImgY = 0 ;

        m_fDispScaleX = pnDraw->Width  / (double)iImgW ;
        m_fDispScaleY = pnDraw->Height / (double)iImgH ;
    }
    OnPaint();
}

/*
void TFrmImage::UpdateScale()
{
    //Trace("Operator", "Image Form_RealRatio Button Click");
    if(btRealRatio -> Down) {
        m_fDispScaleX = 1.0 ;
        m_fDispScaleY = 1.0 ;
    }
    else {
        int iImgW = VISN[m_iVisnId]->GetImg()->GetWidth ();
        int iImgH = VISN[m_iVisnId]->GetImg()->GetHeight();

        m_fDispImgX = 0 ;
        m_fDispImgY = 0 ;



        if(iImgW == 0.0 ||
           iImgH == 0.0 ) {
            m_fDispScaleX = 1.0 ;
            m_fDispScaleY = 1.0 ;
        }
        else {
            m_fDispScaleX = pnDraw->Width  / (double)iImgW ;
            m_fDispScaleY = pnDraw->Height / (double)iImgH ;

        }
    }
    //OnPaint();

}
*/

//---------------------------------------------------------------------------

void __fastcall TFrmImage::btLoadClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Load Button Click");
    if(!OpenPictureDialog1 -> Execute())return ;
    VISN[m_iVisnId]->GetImg()->LoadBmp(OpenPictureDialog1 -> FileName.c_str()) ;


    if(VISN[m_iVisnId]->GetImg()->GetImgBit() == 8) btGray  -> Down = true ;
    else                                            btColor -> Down = true ;
    pnThreshold -> Visible = btBinary -> Down ;

    btStratch -> Down = true ;


    OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btSaveClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Save Button Click");
    if(!SavePictureDialog1 -> Execute()) return ;
    VISN[m_iVisnId]->GetImg()->SaveBmp(SavePictureDialog1 -> FileName.c_str()) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btZoomOutClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Zoom Out Button Click");
    m_fDispScaleX = m_fDispScaleX * 8/10.0 ;
    m_fDispScaleY = m_fDispScaleY * 8/10.0 ;
    OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btZoomInClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Zoom In Button Click");
    m_fDispScaleX = m_fDispScaleX * 10/8.0 ;
    m_fDispScaleY = m_fDispScaleY * 10/8.0 ;
    OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btRealSizeClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Real Size Button Click");
    m_fDispScaleX = 1.0;
    m_fDispScaleY = 1.0;
    OnPaint();
}
//---------------------------------------------------------------------------

void TFrmImage::OnPaint()
{
    //요거 주석 빼면 타이머 야매에서 리턴 된다.
    //if(g_iActivVisnId != m_iVisnId) return ;
    m_iDpCnt++;
    if(m_iDpCnt > 99) m_iDpCnt = 0 ;
    pnDisp -> Caption = m_iDpCnt ;

    
    
    



    int iImgW = VISN[m_iVisnId]->GetImg()->GetWidth () ;
    int iImgH = VISN[m_iVisnId]->GetImg()->GetHeight() ;
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


             m_fDispImgX = 0 ;
             m_fDispImgY = 0 ;

             m_fDispScaleX = iPanW / (double)iImgW ;
             m_fDispScaleY = iPanH / (double)iImgH ;
         }
    }


    int sx = m_fDispImgX ;
    int sy = m_fDispImgY ;

    int dw = GetScaleX() ? iPanW / GetScaleX() : iPanW;
    int dh = GetScaleY() ? iPanH / GetScaleY() : iPanH;


    // Create buffer with memory DC and bitmap, then clear it with background.
    HDC     hDc ;
    HDC     hMemDc ;
    HBITMAP hMemBm , hOldMemBm ;
    EN_DISP_OPTN iDpOptn ;
    RECT    Rect ;
    hDc       = GetDC(pnDraw->Handle); //imDraw -> Canvas -> Handle ;  //GetDC(pnDraw->Handle);
    GetClipBox(hDc , &Rect) ; //DC의 갱신해야할 Rect를 받아온다.
    hMemDc    = CreateCompatibleDC(hDc);
    //hMemBm    = CreateCompatibleBitmap(hDc, iPanW , iPanH);///////////////////////////////////
    hMemBm    = CreateCompatibleBitmap(hDc, iPanW , iPanH);///////////////////////////////////
    hOldMemBm = SelectObject(hMemDc, hMemBm); //Old멤비트맵을 저장해 두는것은 현재 선택되어 있는 오프젝트는 삭제가 안된다. 그래서 삭제할때 월래 놈으로 돌려 놓고 삭제.

         if(btGray   -> Down) iDpOptn = doGray   ;
    else if(btColor  -> Down) iDpOptn = doColor  ;
    else if(btFalse  -> Down) iDpOptn = doFalse  ;
    else if(btBinary -> Down) iDpOptn = doBinary ;
    else if(btRed    -> Down) iDpOptn = doRed    ;
    else if(btGreen  -> Down) iDpOptn = doGreen  ;
    else if(btBlue   -> Down) iDpOptn = doBlue   ;

     VISN[m_iVisnId]->Paint(hMemDc,iDpOptn,
                           0 , 0 , iPanW , iPanH ,
                           m_fDispImgX , m_fDispImgY , dw, dh,
                           GetScaleX() ,GetScaleY() ,sbThresh -> Position , miCntrLine -> Checked) ;







    //GDI 그림.
    //IGdi * Gdi = GDI_New(hMemDc);
    //Gdi -> Pixel(Pnt1.x , Pnt1.y, clRed);
    //Gdi -> Pixel(Pnt2.x , Pnt2.y, clBlue);
    //GDI_Del(Gdi) ;

    RectTracker -> Paint(hMemDc, GetScaleX() , GetScaleY() , m_fDispImgX , m_fDispImgY);
    LineTracker -> Paint(hMemDc, GetScaleX() , GetScaleY() , m_fDispImgX , m_fDispImgY);

    int iRet ;
    iRet = BitBlt(hDc, 0 , 0 , iPanW, iPanH, hMemDc, 0, 0, SRCCOPY);



    // Delete object
    SelectObject(hMemDc,hOldMemBm);
    DeleteObject(hMemBm);
    DeleteDC(hMemDc);
    ReleaseDC(pnDraw->Handle,hDc);


}
//---------------------------------------------------------------------------
void TFrmImage::OnPaintT()
{
    CRunThread<PaintCallbackFunc> * func = new CRunThread<PaintCallbackFunc>(&OnPaintT) ;
}

void __fastcall TFrmImage::FormShow(TObject *Sender)
{
     OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btGrabClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Grab Button Click");
    VISN[m_iVisnId]->Grab();
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btInspClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Inspection Button Click");
    int iSel = 0 ; //pcPara -> ActivePageIndex ;

    SBlob RsltBlob ;
    TCircle SetCircle ;

    float fMaxScore = 0.0 ;
    float fMaxX     = 0.0 ;
    float fMaxY     = 0.0 ;

    TPoint SttPnt , EndPnt ;
    bool bRslt ;

    VISN[m_iVisnId]->Inspection();
}
//---------------------------------------------------------------------------



void __fastcall TFrmImage::btLiveClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Live Button Click");
    VISN[m_iVisnId]->Live(true);
}
//---------------------------------------------------------------------------




void __fastcall TFrmImage::pcParaChange(TObject *Sender)
{

    OnPaint();
}
//---------------------------------------------------------------------------



EN_VISN_ID TFrmImage::GetVisnId(void)
{
    return m_iVisnId ;

}

void __fastcall TFrmImage::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int a  ;

    a++;
}
//---------------------------------------------------------------------------


void __fastcall TFrmImage::lbMenuMouseEnter(TObject *Sender)
{
    lbMenu -> Color = clBlue ;
    lbMenu -> Font -> Color = clWhite ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::lbMenuMouseLeave(TObject *Sender)
{
    lbMenu -> Color = clBtnFace ;
    lbMenu -> Font -> Color = clBlack ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::lbMenuMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    tagPOINT      tpPosition ;
    GetCursorPos(&tpPosition);

    puMenu -> Popup(tpPosition.x - X - lbMenu -> Left + pnDraw -> Left , tpPosition.y + Y + lbMenu -> Top + pnDraw -> Top) ;

}
//---------------------------------------------------------------------------



void __fastcall TFrmImage::miCntrLineClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Center Line Button Click");
    miCntrLine -> Checked = !miCntrLine -> Checked ;
    btCenter   -> Down = miCntrLine -> Checked ;
    btNoCenter -> Down =!miCntrLine -> Checked ;

    OnPaint();

}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::AlgorithmParameter1Click(TObject *Sender)
{
    ////Trace("Operator", "Image Form_Vision Parameter Button Click");

    FrmAlgoPara -> FormShow(m_iVisnId) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::CameraLights1Click(TObject *Sender)
{
    ////Trace("Operator", "Image Form_Camera Lights Button Click");

    if(FrmCamLightPara!=NULL) {
        delete FrmCamLightPara ;
    }

    FrmCamLightPara = new TFrmCamLightPara (this);

    FrmCamLightPara -> Show(m_iVisnId) ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmImage::FormHide(TObject *Sender)
{
    int i ;
    i++;
}
//---------------------------------------------------------------------------
void __fastcall TFrmImage::btMagClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Mag Button Click");
    if(FrmMag) {
        delete FrmMag ;
    }
    FrmMag = new TFrmMag (this);
    FrmMag -> Show() ;        
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btPixelClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Pixel Button Click");
    if(FrmPixel) {
        delete FrmPixel ;
    }
    FrmPixel = new TFrmPixel (this , VISN[m_iVisnId]->GetImg()  , LineTracker);
    FrmPixel -> Show() ;
    LineTracker -> StartX = 10 ; LineTracker -> EndX = 100 ;
    LineTracker -> StartY = 10 ; LineTracker -> EndY = 100 ;



}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btHistoClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Histogram Button Click");
    if(FrmHisto) {
        delete FrmHisto ;
    }
    FrmHisto = new TFrmHisto (this , VISN[m_iVisnId]->GetImg() , RectTracker);
    FrmHisto -> Show();
    RectTracker -> Left = 10 ; RectTracker -> Width  = 90 ;
    RectTracker -> Top  = 10 ; RectTracker -> Height = 90 ;

}
//---------------------------------------------------------------------------








void __fastcall TFrmImage::btCenterClick(TObject *Sender)
{
    miCntrLine -> Checked = !miCntrLine -> Checked ;

    btCenter   -> Down = miCntrLine -> Checked ;
    btNoCenter -> Down =!miCntrLine -> Checked ;

    //Trace("Operator", "Image Form_Center Line Button Click");
    OnPaint();
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
void TFrmImage::SetMagView(float _fLeft , float _fTop , float _fWidth , float _fHeight)
{
    //if(_fLeft < 0) _fLeft = 0 ;
    //if(_fTop  < 0) _fTop  = 0 ;

    //if(_fLeft+_fWidth  >= VISN[m_iVisnId]->GetImg()->GetWidth ()) return ;
    //if(_fTop +_fHeight >= VISN[m_iVisnId]->GetImg()->GetHeight()) return ;

    float fApplyScale ; //화면에 적용될 스케일.
    float fImgOfsX ;    //화면에 적용될 X오프셑
    float fImgOfsY ;    //화면에 적용될 Y오프셑

    //입력된 사각형의 넓이대 높이 비.
    float fInputRatio = _fHeight / _fWidth ; //1이상이면 높이가 크고 1이하면 넓이가 크다.

    //화면에 뿌려줄 판넬의 넓이대 높이 비.
    float fPanelRatio = pnDraw -> Height / (float)pnDraw -> Width ; //1이상이면 높이가 크고 1이하면 넓이가 크다.

    if(fInputRatio > fPanelRatio) {
        fApplyScale = pnDraw -> Height / _fHeight ;
        fImgOfsX    = _fLeft - (pnDraw -> Width / fApplyScale - _fWidth)/2.0 ;
        fImgOfsY    = _fTop ;
    }
    else {
        fApplyScale = pnDraw -> Width / _fWidth ;
        fImgOfsY    = _fTop  - (pnDraw -> Height/ fApplyScale - _fHeight)/2.0 ;
        fImgOfsX    = _fLeft ;
    }

    if(fImgOfsX < 0) fImgOfsX = 0 ;
    if(fImgOfsY < 0) fImgOfsY = 0 ;

    if(fImgOfsX >= VISN[m_iVisnId]->GetImg()->GetWidth () - pnDraw -> Width  / fApplyScale) fImgOfsX = VISN[m_iVisnId]->GetImg()->GetWidth () - pnDraw -> Width  / fApplyScale - 1 ;
    if(fImgOfsY >= VISN[m_iVisnId]->GetImg()->GetHeight() - pnDraw -> Height / fApplyScale) fImgOfsY = VISN[m_iVisnId]->GetImg()->GetHeight() - pnDraw -> Height / fApplyScale - 1 ;


    SetViewMode(false);

    m_fDispImgX   = fImgOfsX ;
    m_fDispImgY   = fImgOfsY ;

    m_fDispScaleX = fApplyScale ;
    m_fDispScaleY = fApplyScale ;

    OnPaint();
}


void TFrmImage::SetViewMode(bool _bStratch)
{
    if(_bStratch) btStratch   -> Down = true ;
    else          btRealRatio -> Down = true ;

    if(btRealRatio -> Down) {
        m_fDispScaleX = 1.0 ;
        m_fDispScaleY = 1.0 ;
    }
    else {
        int iImgW = VISN[m_iVisnId]->GetImg()->GetWidth ();
        int iImgH = VISN[m_iVisnId]->GetImg()->GetHeight();

        m_fDispImgX = 0 ;
        m_fDispImgY = 0 ;

        m_fDispScaleX = pnDraw->Width  / (double)iImgW ;
        m_fDispScaleY = pnDraw->Height / (double)iImgH ;
    }
    OnPaint();
}
void __fastcall TFrmImage::SpeedButton1Click(TObject *Sender)
{
    //Trace("Operator", "Image Form_Histogram Button Click");
    if(Frm3d) {
        delete Frm3d ;
    }                    //TComponent* Owner , IImage * _pImage , TRectTracker * _pTracker , double * _pRefOstX , double * _pRefOstY);

    double dTemp1 , dTemp2 ;
    Frm3d = new TFrm3d( this , VISN[m_iVisnId]->GetImg() , RectTracker , &dTemp1 , &dTemp2 );
    Frm3d -> Show();
    RectTracker -> Left = 10 ; RectTracker -> Width  = 90 ;
    RectTracker -> Top  = 10 ; RectTracker -> Height = 90 ;
}
//---------------------------------------------------------------------------

