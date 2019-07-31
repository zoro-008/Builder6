//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormTrain.h"
#include "UserFile.h"
#include "CamUnit.h"
#include "PkgCamera.h"

#include "SLogUnit.h"



#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmTrain *FrmTrain;
//----------------------------------------------------------------- ----------
__fastcall TFrmTrain::TFrmTrain(TComponent* Owner)
    : TForm(Owner)
{

}
//---------------------------------------------------------------------------
bool TFrmTrain::Init()
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
    m_pActiveImg = NULL ;

    FrmMag       = NULL ;
    FrmPixel     = NULL ;
    FrmHisto     = NULL ;
    FrmValueList = NULL ;

    btGray  -> Down = true ;
    pnDraw -> Align = alClient ;

    RectTracker = new TRectTracker  (); RectTracker -> Caption = "Rect" ;
    LineTracker = new TLineTracker  (); LineTracker -> Caption = "Line" ;
    RectTracker -> LineColor = clLime ; RectTracker -> PointColor = clLime ; RectTracker -> Visible = false ;
    LineTracker -> LineColor = clLime ; LineTracker -> PointColor = clLime ; LineTracker -> Visible = false ;
    RectTracker-> SetFunc(OnPaintMsg); //혹시나 쓰레드에서 건드릴까봐 메세지 처리.
    LineTracker-> SetFunc(OnPaintMsg);


    m_dDispImgX   = 0 ;
    m_dDispImgY   = 0 ;

    return true ;
}

bool TFrmTrain::Close()
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

double TFrmTrain::GetScaleX()
{
    return m_dDispScaleX ;
}

double TFrmTrain::GetScaleY()
{
    return m_dDispScaleY ;
}

//---------------------------------------------------------------------------
void __fastcall TFrmTrain::PanelProc(Messages::TMessage &msg)
{
    OriginalProc(msg);
    if (msg.Msg == WM_PAINT ) {
        OnPaint();
    }
}

void __fastcall TFrmTrain::tmUpdateTimer(TObject *Sender)
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
    btUp         -> Visible = m_iLevel >= 2 ;
    btDel        -> Visible = m_iLevel >= 2 ;
    btDn         -> Visible = m_iLevel >= 2 ;
    btInsert     -> Visible = m_iLevel >= 2 ;

    btValueList  -> Enabled = m_iLevel >= 2 ;

    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::btGrayClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Gray Button Click");
    pnThreshold -> Visible = btBinary -> Down ;
    OnPaint();


}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::sbThreshChange(TObject *Sender)
{
    pnThreash -> Caption = sbThresh -> Position ;
    OnPaint();
}

//---------------------------------------------------------------------------
void __fastcall TFrmTrain::pnDrawMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(!m_pActiveImg || m_pActiveImg -> GetSize() == 0) return ;
    if(Button == mbLeft) {

        double dx = m_dDispImgX + X / m_dDispScaleX ;
        double dy = m_dDispImgY + Y / m_dDispScaleY ;

        Stat.iClickX = X ;
        Stat.iClickY = Y ;

        RectTracker     -> Focused = false ;
        LineTracker     -> Focused = false ;


        if(RectTracker  -> TrackerDw(Button, Shift , X,Y, GetScaleX() , GetScaleY(), m_dDispImgX , m_dDispImgY)) {Stat.bTrckrClick = true ; OnPaint();return ;}
        if(LineTracker  -> TrackerDw(Button, Shift , X,Y, GetScaleX() , GetScaleY(), m_dDispImgX , m_dDispImgY)) {Stat.bTrckrClick = true ; OnPaint();return ;}

        if(m_pVision    -> MouseDw(Button, Shift , X,Y, GetScaleX() , GetScaleY(), m_dDispImgX , m_dDispImgY)) {Stat.bTrckrClick = true ; OnPaint();return ;}


        //if(VISN[m_iVisnId]->OnMouseDw(Button, Shift , X,Y, GetScaleX() , GetScaleY(), m_dDispImgX , m_dDispImgY)) {Stat.bTrckrClick = true ; return ;}
        //if(!btStratch -> Down) {
            Stat.bImgClick = true ;
        //}

    }


}

void __fastcall TFrmTrain::pnDrawMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    if(!m_pActiveImg || m_pActiveImg -> GetSize() == 0) return ;
    //if(pnDraw -> Width < RectTracker -> Width){
    //    return;
    //}
    //if((Stat.iClickX-X) / GetScaleX() > VISN[m_iVisnId]->GetImg()->GetWidth () - pnDraw -> Width  / GetScaleX()){
    //    return;
    //}

    if(Stat.bTrckrClick) {
        RectTracker  -> TrackerMv(Shift , X,Y, GetScaleX() , GetScaleY(), m_dDispImgX , m_dDispImgY) ;
        LineTracker  -> TrackerMv(Shift , X,Y, GetScaleX() , GetScaleY(), m_dDispImgX , m_dDispImgY) ;

        m_pVision    -> MouseMv  (Shift , X,Y, GetScaleX() , GetScaleY(), m_dDispImgX , m_dDispImgY);
        OnPaint();
    }
    else if(Stat.bImgClick){
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
        OnPaint();
    }




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

void __fastcall TFrmTrain::pnDrawMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(!m_pActiveImg || m_pActiveImg -> GetSize() == 0) return ;
    if(Button == mbLeft) {
        RectTracker  -> TrackerUp(Button, Shift);
        LineTracker  -> TrackerUp(Button, Shift);
        m_pVision->MouseUp(Button, Shift);
    }

    Stat.bTrckrClick = false ;
    Stat.bImgClick   = false ;

}
//---------------------------------------------------------------------------


void __fastcall TFrmTrain::btRealRatioClick(TObject *Sender)
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

void __fastcall TFrmTrain::btLoadClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Load Button Click");
    if(!OpenPictureDialog1 -> Execute())return ;
    m_pActiveImg->LoadBmp(OpenPictureDialog1 -> FileName.c_str()) ;


    if(m_pActiveImg->GetImgBit() == 8) btGray  -> Down = true ;
    else                                            btColor -> Down = true ;
    pnThreshold -> Visible = btBinary -> Down ;

    btStratch -> Down = true ;


    OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::btSaveClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Save Button Click");
    if(!SavePictureDialog1 -> Execute()) return ;
    m_pActiveImg->SaveBmp(SavePictureDialog1 -> FileName.c_str()) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::btZoomOutClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Zoom Out Button Click");
    m_dDispScaleX = m_dDispScaleX * 8/10.0 ;
    m_dDispScaleY = m_dDispScaleY * 8/10.0 ;
    OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::btZoomInClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Zoom In Button Click");
    m_dDispScaleX = m_dDispScaleX * 10/8.0 ;
    m_dDispScaleY = m_dDispScaleY * 10/8.0 ;
    OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::btRealSizeClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Real Size Button Click");
    m_dDispScaleX = 1.0;
    m_dDispScaleY = 1.0;
    OnPaint();
}
//---------------------------------------------------------------------------
void TFrmTrain::OnPaintMsg()
{
    PostMessage(pnDraw->Handle , WM_PAINT , 0 , 0 );
}
void TFrmTrain::OnPaint()
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
     m_pVision->Paint(hMemDc,iDpOptn,
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


    m_pVision -> PaintRslt(hMemDc , GetScaleX() , GetScaleY() , m_dDispImgX , m_dDispImgY);


    if(btCenter -> Down) {
        CGdi * Gdi = new CGdi(hMemDc);
        int iCntrX = (iImgW /2.0 - m_dDispImgX) * GetScaleX() ;
        int iCntrY = (iImgH /2.0 - m_dDispImgY) * GetScaleY() ;
        Gdi -> m_tPen.Style = psDot  ;
        Gdi -> m_tPen.Color = clBlue ;
        Gdi -> LinePlus(iCntrX , iCntrY , 100);


        delete Gdi ;
    }


    m_pVision -> PaintTracker(hMemDc , GetScaleX() , GetScaleY() , m_dDispImgX , m_dDispImgY);


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
void __fastcall TFrmTrain::FormShow(TObject *Sender)
{
    this -> Left   = 0 ;
    this -> Top    = 0 ;
    this -> Width  = 1280 ;
    this -> Height = 1024 ;

    cbPkgKind -> Items -> Clear() ;
    for(int i = 0 ; i < CVision::MAX_PKG_KIND ; i++) {
        cbPkgKind -> Items -> Add(m_pVision -> GetPkgList(i));
        cbPkgKind -> ItemIndex =  i ;
    }

    sgPkgList -> Row = 1 ;

    m_pVision -> UpdateList(true , sgPkgList );



    tmUpdate -> Enabled = true ;


    m_pOriMsgWnd =  m_pVision -> GetSendMsgWnd();
    m_pVision -> SetSendMsgWnd(pnDraw->Handle);
    OnPaint();

    UpdateTable();
}
//---------------------------------------------------------------------------









void __fastcall TFrmTrain::pcParaChange(TObject *Sender)
{

    OnPaint();
}
//---------------------------------------------------------------------------



void __fastcall TFrmTrain::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int a  ;

    a++;
}
//---------------------------------------------------------------------------











void __fastcall TFrmTrain::btMagClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Mag Button Click");
    if(FrmMag) {
        delete FrmMag ;
    }
    FrmMag = new TFrmMag (this);
    FrmMag -> Show() ;        
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::btPixelClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Pixel Button Click");
    if(FrmPixel) {
        delete FrmPixel ;
    }
    FrmPixel = new TFrmPixel (this , m_pActiveImg  , LineTracker);
    FrmPixel -> Show() ;        
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::btHistoClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Histogram Button Click");
    if(FrmHisto) {
        delete FrmHisto ;
    }                                                         
    FrmHisto = new TFrmHisto (this , m_pActiveImg , RectTracker);
    FrmHisto -> Show();
}
//---------------------------------------------------------------------------








void __fastcall TFrmTrain::btCenterClick(TObject *Sender)
{
    OnPaint();
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
void TFrmTrain::SetMagView(double _dLeft , double _dTop , double _dWidth , double _dHeight)
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


void TFrmTrain::SetViewMode(bool _bStratch)
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


void __fastcall TFrmTrain::FormClose(TObject *Sender, TCloseAction &Action)
{
     if(FrmValueList)FrmValueList -> Close();
     tmLive   -> Enabled = false ;
     tmUpdate -> Enabled = false ;
     m_pVision -> SetSendMsgWnd(m_pOriMsgWnd);
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::btInsertClick(TObject *Sender)
{
    pnAdd -> Visible = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::btInsertCancelClick(TObject *Sender)
{
    pnAdd -> Visible = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::btInsertListClick(TObject *Sender)
{
    bool bCamExist = false ;
    for(int i= 0 ; i < m_pVision -> GetList() -> GetDataCnt() ; i++){
        m_pVision -> GetList() -> GetCrntData(!i);
        if(m_pVision -> GetList() -> GetCrntData(!i) -> GetComment().Pos("Camera")){
            bCamExist = true ;
        }
    }
    if(bCamExist && cbPkgKind -> Text.Pos("Camera")) {
        ShowMessage("Camera is already exist!");
        return ;

    }

    m_pVision -> Insert(sgPkgList -> Row -1 ,  edPkgName->Text , m_pVision -> GetPkgList(cbPkgKind -> ItemIndex));
    m_pVision -> SetActivePkg(sgPkgList -> Row -1);
    pnAdd -> Visible = false ;

    m_pVision -> LoadSave(false);

    UpdateTable() ;


}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::btUpClick(TObject *Sender)
{
    if(m_pVision -> MoveUp(sgPkgList -> Row -1)){
        sgPkgList -> Row -= 1 ;
    }
    m_pVision -> LoadSave(false);
    UpdateTable() ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::btDnClick(TObject *Sender)
{
    CPkgBase * Pkg = m_pVision ->GetCrntPkg() ;
    if(Pkg -> GetComment().Pos("Camera")) {
        ShowMessage("Can't not Move Camera PKG!");
        return ;
    }
    if(m_pVision -> MoveDown(sgPkgList -> Row -1)) {
        sgPkgList -> Row += 1;
    }
    m_pVision -> LoadSave(false);
    UpdateTable() ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmTrain::btDelClick(TObject *Sender)
{


    CPkgBase * Pkg = m_pVision ->GetCrntPkg() ;
    if(Pkg -> GetComment().Pos("Camera")) {
        ShowMessage("Can't not delete Camera PKG!");
        return ;
    }

    if (MessageBox(this->Handle, "PKG를 삭제 하시겠습니까?", "Confirmation", MB_YESNO)!=IDYES) return;

    m_pVision -> Delete(sgPkgList -> Row -1);
    if(sgPkgList -> Row <= sgPkgList -> RowCount)sgPkgList -> Row-- ;
    m_pVision -> LoadSave(false);
    UpdateTable() ;
}
//---------------------------------------------------------------------------
void TFrmTrain::UpdateTable()
{

    int iPkgSel = sgPkgList -> Row - 1 ;

    if(m_pVision ->GetPkgCnt() < 0) return ;

    AnsiString sPath = "D:\\BC6Work\\sTemp\\";

    if(m_pVision == NULL) return ;

    m_pVision -> UpdateList(true ,sgPkgList ) ;
    m_pVision -> SetActivePkg(iPkgSel);
    CPkgBase * Pkg = m_pVision -> GetCrntPkg();

    if(Pkg == NULL) return ;

    if(Pkg -> GetComment().Pos("Camera")) { //카메라 일 경우.
        m_pActiveImg = m_pVision -> GetCrntImg();

        //위에판넬
        btTrain -> Visible = Pkg -> GetTrainAvailable() ;
        btRun   -> Visible = false ;

        pnCamera -> Visible = true ;

        //btLoad -> Visible = true ;
        //btSave -> Visible = true ;
        //btLive -> Visible = true ;



    }
    else {
        tmLive -> Enabled = false ;
        btLive -> Down    = false ;
        m_pActiveImg = m_pVision -> GetTrainImg();

        btTrain -> Visible = Pkg -> GetTrainAvailable() ;

        btRun   -> Visible = true ;

        pnCamera -> Visible = false ;

        //btCapture -> Visible = false ;
        //btLoad -> Visible = false ;
        //btSave -> Visible = false ;
        //btLive -> Visible = false ;

    }

    btPre  -> Visible = sgPkgList -> Row > 1 ;
    btNext -> Visible = sgPkgList -> Row < sgPkgList -> RowCount - 1 ;




    if(btMasterPara -> Down) {
        m_pVision -> UpdateMasterPara(true , vlPara);
    }
    else if(btLocalPara -> Down) {
        m_pVision -> UpdateLocalPara(true , vlPara);
    }
    else if(btCommonPara -> Down) {
        m_pVision -> UpdateCommonPara(true , vlPara);
    }

    OnPaint();

}
                          //
void __fastcall TFrmTrain::sgPkgListClick(TObject *Sender)
{
    if(btMasterPara -> Down) {
        m_pVision -> UpdateMasterPara(false , vlPara);
    }
    else if(btLocalPara -> Down) {
        m_pVision -> UpdateLocalPara (false , vlPara);
    }
    else if(btCommonPara -> Down) {
        m_pVision -> UpdateCommonPara(false , vlPara);
    }
    btCommonPara -> Down = true ;
    UpdateTable() ;

}
//---------------------------------------------------------------------------


void __fastcall TFrmTrain::btLocalParaClick(TObject *Sender)
{

    UpdateTable();
}
//---------------------------------------------------------------------------









void __fastcall TFrmTrain::tmLiveTimer(TObject *Sender)
{
    tmLive -> Enabled = false ;
    //if(!Pkg -> GetComment().Pos("Camera")) {
    //    return ;
    //}
    if(m_pVision ->GetGrabEnd()) m_pVision -> Grab();
    tmLive -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::btLiveClick(TObject *Sender)
{
    tmLive -> Enabled = btLive -> Down ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::btTrainClick(TObject *Sender)
{
    CPkgBase * Pkg = m_pVision -> GetCrntPkg();

    Pkg -> Train(m_pActiveImg);
    OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::btRunClick(TObject *Sender)
{
    m_pVision -> TrainInsp();
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::btPreClick(TObject *Sender)
{
    if(btMasterPara -> Down) {
        m_pVision -> UpdateMasterPara(false , vlPara);
    }
    else if(btLocalPara -> Down) {
        m_pVision -> UpdateLocalPara (false , vlPara);
    }
    else if(btCommonPara -> Down) {
        m_pVision -> UpdateCommonPara(false , vlPara);
    }
    sgPkgList -> Row -- ;
    UpdateTable();        
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::btNextClick(TObject *Sender)
{
    if(btMasterPara -> Down) {
        m_pVision -> UpdateMasterPara(false , vlPara);
    }
    else if(btLocalPara -> Down) {
        m_pVision -> UpdateLocalPara (false , vlPara);
    }
    else if(btCommonPara -> Down) {
        m_pVision -> UpdateCommonPara(false , vlPara);
    }

    sgPkgList -> Row ++ ;
    UpdateTable();
}
//---------------------------------------------------------------------------






void __fastcall TFrmTrain::vlParaKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
//    if(btMasterPara -> Down) {
//        m_pVision -> UpdateMasterPara(false , vlPara);
//    }
//    else if(btLocalPara -> Down) {
//        m_pVision -> UpdateLocalPara (false , vlPara);
//    }
//    else if(btCommonPara -> Down) {
//        m_pVision -> UpdateCommonPara(false , vlPara);
//    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::btSaveParaClick(TObject *Sender)
{
    if(btMasterPara -> Down) {
        m_pVision -> UpdateMasterPara(false , vlPara);
    }
    else if(btLocalPara -> Down) {
        m_pVision -> UpdateLocalPara (false , vlPara);
    }
    else if(btCommonPara -> Down) {
        m_pVision -> UpdateCommonPara(false , vlPara);
    }

    m_pVision -> LoadSave(false);
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrain::btValueListClick(TObject *Sender)
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


void __fastcall TFrmTrain::btCaptureClick(TObject *Sender)
{
    CPkgBase * Pkg = m_pVision -> GetCrntPkg();

    ((CCamera_V01 *)Pkg) -> CaptureTrainImg();
}
//---------------------------------------------------------------------------

void TFrmTrain::SetLevel(int _iLevel)//0:oper 1,2: engineer
{
    m_iLevel = _iLevel ;
}
void __fastcall TFrmTrain::BitBtn1Click(TObject *Sender)
{
    TForm::Close();
    //Close();
}
//---------------------------------------------------------------------------

