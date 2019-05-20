//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormImage.h"
#include "UserFile.h"
//#include "VisionMan.h"

#include "FormAlgoPara.h"
#include "FormVisnPara.h"

#include "VOptnMan.h"
#include "SLogUnit.h"



#define USE_PAINT_MSG

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"





//---------------------------------------------------------------------------
__fastcall TFrmImage::TFrmImage(TComponent* Owner)
    : TForm(Owner)
{


}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::FormCreate(TObject *Sender)
{
    static int iFrmNo = -1 ;

    iFrmNo++;



    m_iDpCnt = 0 ;

    m_iOriImgWidth  = pnDraw -> Width  ;
    m_iOriImgHeight = pnDraw -> Height ;

#ifdef USE_PAINT_MSG
    OriginalProc = pnDraw->WindowProc ;
    pnDraw->WindowProc = PanelProc    ;
#endif


    memset(&Stat , 0 , sizeof(TStat));

    FrmMag   = NULL ;
    FrmPixel = NULL ;
    FrmHisto = NULL ;

    FrmCalPara      = NULL ; //new TFrmCalPara(this , iFrmNo);
    FrmCamLightPara = NULL ; //new TFrmCamPara(this , iFrmNo);

    //SetFormEnable(false);

    m_iPrePage = 0 ;

    btGray  -> Down = true ;

    pnDraw -> Align = alClient ;

    RectTracker = new TRectTracker  (); RectTracker -> Caption = "Rect" ;
    LineTracker = new TLineTracker  (); LineTracker -> Caption = "Line" ;

    RectTracker -> LineColor = clLime ; RectTracker -> PointColor = clLime ; RectTracker -> Visible = false ;
    LineTracker -> LineColor = clLime ; LineTracker -> PointColor = clLime ; LineTracker -> Visible = false ;


    RectTracker-> SetFunc(OnPaintMsg);
    LineTracker-> SetFunc(OnPaintMsg);


    VISN[iFrmNo]->SetViewMode(vmNone);

    VISN[iFrmNo]->SetPaintCallback(PaintCallback);


    //VISN[iFrmNo]->PaintCallback();

    bool b24Bit   = VISN[m_iVisnId]->Img->GetImgBit() == 24 ;

    if(!b24Bit) {
        btColor -> Visible = b24Bit ;
        btRed   -> Visible = b24Bit ;
        btGreen -> Visible = b24Bit ;
        btBlue  -> Visible = b24Bit ;
    }

}

void TFrmImage::PaintCallback()
{
    if(VISN[m_iVisnId]->Img->GetImgBit() == 8 && btColor -> Down){
        btGray -> Down = true ;
    }


    if( m_iOriImgWidth  != VISN[m_iVisnId]->Img->GetWidth ()|| m_iOriImgHeight != VISN[m_iVisnId]->Img->GetHeight()) {
        m_iOriImgWidth    = VISN[m_iVisnId]->Img->GetWidth () ;
        m_iOriImgHeight   = VISN[m_iVisnId]->Img->GetHeight() ;
    }


    OnPaintMsg();

}

void TFrmImage::SetInspCallbackFunc (InspCallbackFunc _InspCallback)
{
    InspCallback = _InspCallback ;
}

void __fastcall TFrmImage::FormDestroy(TObject *Sender)
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

    Trace("formDestroy",("FormImage"+AnsiString(m_iVisnId)).c_str());


}


//---------------------------------------------------------------------------
void __fastcall TFrmImage::PanelProc(Messages::TMessage &msg)
{
    OriginalProc(msg);
    if (msg.Msg == WM_PAINT ) {
        Trace("g_csPaint", ("PaintStart Visn" + AnsiString(m_iVisnId)).c_str());
        OnPaint();
        Trace("g_csPaint",  ("PaintEnd Visn" + AnsiString(m_iVisnId)).c_str());
    }
}

void __fastcall TFrmImage::tmUpdateTimer(TObject *Sender)
{
    if(g_iShowingVisnId != m_iVisnId) return ;
    tmUpdate -> Enabled = false ;

    bool b24Bit   = VISN[m_iVisnId]->Img->GetImgBit() == 24 ;
    bool bStratch = btStratch -> Down ;

    btColor -> Visible = b24Bit ;
    btRed   -> Visible = b24Bit ;
    btGreen -> Visible = b24Bit ;
    btBlue  -> Visible = b24Bit ;

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

    if(VISN[m_iVisnId]->ParaUi.pcParaBase) {

        if(m_iPrePage != VISN[m_iVisnId]->ParaUi.pcParaBase -> ActivePageIndex ) {
            m_iPrePage = VISN[m_iVisnId]->ParaUi.pcParaBase -> ActivePageIndex ;

        }

        VISN[m_iVisnId]->ParaUi.pcParaBase -> ActivePageIndex != 0 ;

        if(VISN[m_iVisnId]->ParaUi.pcParaBase -> ActivePageIndex == 0) { //카메라 폼.

        }
        else if(VISN[m_iVisnId]->ParaUi.pcParaBase -> ActivePageIndex > 0) {

        }
    }
    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btGrayClick(TObject *Sender)
{
    Trace("Operator", "Image Form_Gray Button Click");
    pnThreshold -> Visible = btBinary -> Down ;
    OnPaintMsg();


}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::sbThreshChange(TObject *Sender)
{
    pnThreash -> Caption = sbThresh -> Position ;
    OnPaintMsg();
}

//---------------------------------------------------------------------------
void __fastcall TFrmImage::pnDrawMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Button == mbLeft) {
        Stat.iClickX = X ;
        Stat.iClickY = Y ;

        RectTracker     -> Focused = false ;
        LineTracker     -> Focused = false ;

        if(RectTracker  -> TrackerDw(Button, Shift , X,Y, VISN[m_iVisnId]->Img->GetScaleX() , VISN[m_iVisnId]->Img->GetScaleY())) {Stat.bTrckrClick = true ; return ;}
        if(LineTracker  -> TrackerDw(Button, Shift , X,Y, VISN[m_iVisnId]->Img->GetScaleX() , VISN[m_iVisnId]->Img->GetScaleY())) {Stat.bTrckrClick = true ; return ;}

        if(VISN[m_iVisnId]->MouseTrackerDn(Button, Shift , X,Y, VISN[m_iVisnId]->Img->GetScaleX() , VISN[m_iVisnId]->Img->GetScaleY())) {Stat.bTrckrClick = true ; return ;}

        Stat.bImgClick = true ;
    }


}

//---------------------------------------------------------------------------
void __fastcall TFrmImage::pnDrawMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    if(Stat.bTrckrClick) {
        RectTracker  -> TrackerMv(Shift , X,Y, VISN[m_iVisnId]->Img->GetScaleX() , VISN[m_iVisnId]->Img->GetScaleY()) ;
        LineTracker  -> TrackerMv(Shift , X,Y, VISN[m_iVisnId]->Img->GetScaleX() , VISN[m_iVisnId]->Img->GetScaleY()) ;
        VISN[m_iVisnId]->MouseTrackerMv(Shift , X,Y, VISN[m_iVisnId]->Img->GetScaleX() , VISN[m_iVisnId]->Img->GetScaleY()) ;
    }
    else if(Stat.bImgClick){
        sbDraw->HorzScrollBar->Position += (Stat.iClickX-X);
        sbDraw->VertScrollBar->Position += (Stat.iClickY-Y);
    }




    AnsiString sTemp  ;
    AnsiString sTemp2 ;


    //픽셀 디스플레이.
    if(!VISN[m_iVisnId]->Img->GetImg()) return ;
    if(VISN[m_iVisnId]->Img->GetScaleX() && VISN[m_iVisnId]->Img->GetScaleY()) {
        if( X/VISN[m_iVisnId]->Img->GetScaleX() < VISN[m_iVisnId]->Img->GetWidth() && Y/VISN[m_iVisnId]->Img->GetScaleY() < VISN[m_iVisnId]->Img->GetHeight() &&
            X/VISN[m_iVisnId]->Img->GetScaleX() > 0               && Y/VISN[m_iVisnId]->Img->GetScaleY() > 0                ){
                sTemp = "<"+(AnsiString)VISN[m_iVisnId]->Img->GetWidth() + "." + (AnsiString)VISN[m_iVisnId]->Img->GetHeight() + "." + (AnsiString)VISN[m_iVisnId]->Img->GetImgBit()+"> " +
                        "("+(AnsiString)VISN[m_iVisnId]->Img->GetImgX(X) + "." + (AnsiString)VISN[m_iVisnId]->Img->GetImgY(Y) + "=" ;

                if(VISN[m_iVisnId]->Img->GetImgBit() == 8 ) sTemp2 = (AnsiString)VISN[m_iVisnId]->Img->GetPixel(VISN[m_iVisnId]->Img->GetImgX(X),VISN[m_iVisnId]->Img->GetImgY(Y))+")";
                if(VISN[m_iVisnId]->Img->GetImgBit() == 24) sTemp2 = "R:"  + (AnsiString)VISN[m_iVisnId]->Img->GetPixelR(VISN[m_iVisnId]->Img->GetImgX(X),VISN[m_iVisnId]->Img->GetImgY(Y))+
                                                    " G:" + (AnsiString)VISN[m_iVisnId]->Img->GetPixelG(VISN[m_iVisnId]->Img->GetImgX(X),VISN[m_iVisnId]->Img->GetImgY(Y))+
                                                    " B:" + (AnsiString)VISN[m_iVisnId]->Img->GetPixelB(VISN[m_iVisnId]->Img->GetImgX(X),VISN[m_iVisnId]->Img->GetImgY(Y))+")";
                pnPixelInfo -> Caption = sTemp + sTemp2 ;

            int PxValue  = VISN[m_iVisnId]->Img->GetPixel (VISN[m_iVisnId]->Img->GetImgX(X),VISN[m_iVisnId]->Img->GetImgY(Y)) ;
            AnsiString Gray ;
            int DeGray ;

            if(VISN[m_iVisnId]->Img->GetImgBit() == 8 ) {
                Gray   = "0x" + IntToHex(PxValue,2) + IntToHex(PxValue,2) + IntToHex(PxValue,2);
                DeGray = StrToIntDef(Gray, 0);
                pnColor -> Color = DeGray ;
                if (PxValue <128) pnColor -> Font -> Color = clWhite ;
                else              pnColor -> Font -> Color = clBlack ;

                pnColor -> Caption = PxValue ;
            }
            if(VISN[m_iVisnId]->Img->GetImgBit() == 24) {
                int PxValueR = VISN[m_iVisnId]->Img->GetPixelR(VISN[m_iVisnId]->Img->GetImgX(X),VISN[m_iVisnId]->Img->GetImgY(Y)) ;
                int PxValueG = VISN[m_iVisnId]->Img->GetPixelG(VISN[m_iVisnId]->Img->GetImgX(X),VISN[m_iVisnId]->Img->GetImgY(Y)) ;
                int PxValueB = VISN[m_iVisnId]->Img->GetPixelB(VISN[m_iVisnId]->Img->GetImgX(X),VISN[m_iVisnId]->Img->GetImgY(Y)) ;

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
        VISN[m_iVisnId]->MouseTrackerUp(Button, Shift, X, Y , VISN[m_iVisnId]->Img->GetScaleX() , VISN[m_iVisnId]->Img->GetScaleY());
    }

    Stat.bTrckrClick = false ;
    Stat.bImgClick   = false ;

}
//---------------------------------------------------------------------------


void __fastcall TFrmImage::btRealRatioClick(TObject *Sender)
{
    Trace("Operator", "Image Form_RealRatio Button Click");
    if(btRealRatio -> Down) {
        pnDraw -> Align = alNone ;
        pnDraw -> Width  = VISN[m_iVisnId]->Img->GetWidth () ;
        pnDraw -> Height = VISN[m_iVisnId]->Img->GetHeight() ;
    }
    else {
        pnDraw -> Align = alClient ;
    }
    OnPaintMsg();
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btLoadClick(TObject *Sender)
{
    Trace("Operator", "Image Form_Load Button Click");
    if(!OpenPictureDialog1 -> Execute())return ;
    VISN[m_iVisnId]->Img->LoadBmp(OpenPictureDialog1 -> FileName.c_str()) ;




    if( m_iOriImgWidth  != VISN[m_iVisnId]->Img->GetWidth ()||
        m_iOriImgWidth != VISN[m_iVisnId]->Img->GetHeight()  ) { //이상하게 이괄호 안에 있는것을 그대로 하지 않으면 화면이 안보이는 경우가 있다.
        m_iOriImgWidth    = VISN[m_iVisnId]->Img->GetWidth () ;
        m_iOriImgHeight   = VISN[m_iVisnId]->Img->GetHeight() ;
    }
    if(VISN[m_iVisnId]->Img->GetImgBit() == 8) btGray  -> Down = true ;
    else                                      btColor -> Down = true ;
    pnThreshold -> Visible = btBinary -> Down ;



    btRealRatioClick(Sender);

    OnPaintMsg();
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btSaveClick(TObject *Sender)
{
    Trace("Operator", "Image Form_Save Button Click");
    if(!SavePictureDialog1 -> Execute()) return ;
    VISN[m_iVisnId]->Img->SaveBmp(SavePictureDialog1 -> FileName.c_str()) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btZoomOutClick(TObject *Sender)
{
    Trace("Operator", "Image Form_Zoom Out Button Click");
    pnDraw -> Width  = pnDraw -> Width  * 8/10.0 ;
    pnDraw -> Height = pnDraw -> Height * 8/10.0 ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btZoomInClick(TObject *Sender)
{
    Trace("Operator", "Image Form_Zoom In Button Click");
    pnDraw -> Width  = pnDraw -> Width  * 10/8.0 ;
    pnDraw -> Height = pnDraw -> Height * 10/8.0 ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btRealSizeClick(TObject *Sender)
{
    Trace("Operator", "Image Form_Real Size Button Click");
    pnDraw -> Width  = VISN[m_iVisnId]->Img->GetWidth ();
    pnDraw -> Height = VISN[m_iVisnId]->Img->GetHeight();
}
//---------------------------------------------------------------------------

void TFrmImage::OnPaintMsg()
{
    if(g_iShowingVisnId != m_iVisnId) return ;
    //if(!this -> Showing) return ;


    PostMessage(pnDraw->Handle , WM_PAINT , 0 , 0 );
    //SendMessage(pnDraw->Handle , WM_PAINT , 0 , 0 );//이것 또한 멈춤.
    //OnPaint();//시퀜스 쓰레드에서 검사 끝나고 호출 될때에 메인쓰레드랑 충돌날 소지 있음.

}

void TFrmImage::OnPaint()
{




    RECT R ;
    R.left   = 0 ;
    R.top    = 0 ;
    R.right  = pnDraw -> Width  ;
    R.bottom = pnDraw -> Height ;


    m_iDpCnt++;
    if(m_iDpCnt > 99) m_iDpCnt = 0 ;
    lbDisp -> Caption = m_iDpCnt ;

    // Create buffer with memory DC and bitmap, then clear it with background.
    HDC     hDc ;
    HDC     hMemDc ;
    HBITMAP hMemBm , hOldMemBm ;


    HWND Temphwnd =  pnDraw->Handle ;

    hDc       = GetDC(pnDraw->Handle); //imDraw -> Canvas -> Handle ;  //GetDC(pnDraw->Handle);
    hMemDc    = CreateCompatibleDC(hDc);
    hMemBm    = CreateCompatibleBitmap(hDc, R.right, R.bottom);
    hOldMemBm = SelectObject(hMemDc, hMemBm);


         if(btGray   -> Down) {VISN[m_iVisnId]->m_iYameRet = VISN[m_iVisnId]->Img->ShowImgGrey  (hMemDc, R.left , R.top , R.right , R.bottom);                      }
    else if(btColor  -> Down) {VISN[m_iVisnId]->m_iYameRet = VISN[m_iVisnId]->Img->ShowImgColor (hMemDc, R.left , R.top , R.right , R.bottom);                      }
    else if(btFalse  -> Down) {VISN[m_iVisnId]->m_iYameRet = VISN[m_iVisnId]->Img->ShowImgFalse (hMemDc, R.left , R.top , R.right , R.bottom);                      }
    else if(btBinary -> Down) {VISN[m_iVisnId]->m_iYameRet = VISN[m_iVisnId]->Img->ShowImgBinary(hMemDc, R.left , R.top , R.right , R.bottom ,sbThresh -> Position);}
    else if(btRed    -> Down) {VISN[m_iVisnId]->m_iYameRet = VISN[m_iVisnId]->Img->ShowImgR     (hMemDc, R.left , R.top , R.right , R.bottom);                      }
    else if(btGreen  -> Down) {VISN[m_iVisnId]->m_iYameRet = VISN[m_iVisnId]->Img->ShowImgG     (hMemDc, R.left , R.top , R.right , R.bottom);                      }
    else if(btBlue   -> Down) {VISN[m_iVisnId]->m_iYameRet = VISN[m_iVisnId]->Img->ShowImgB     (hMemDc, R.left , R.top , R.right , R.bottom);                      }


    //GDI 그림.
    //IGdi * Gdi = GDI_New(hMemDc);
    //Gdi -> Pixel(Pnt1.x , Pnt1.y, clRed);
    //Gdi -> Pixel(Pnt2.x , Pnt2.y, clBlue);
    //GDI_Del(Gdi) ;



    //if(pnParaMain -> Visible)
      VISN[m_iVisnId]->PaintRslt(hMemDc , VISN[m_iVisnId]->Img->GetScaleX() , VISN[m_iVisnId]->Img->GetScaleY());
      VISN[m_iVisnId]->PaintTracker(hMemDc , VISN[m_iVisnId]->Img->GetScaleX() , VISN[m_iVisnId]->Img->GetScaleY());

      RectTracker -> Paint(hMemDc, VISN[m_iVisnId]->Img->GetScaleX() , VISN[m_iVisnId]->Img->GetScaleY() );
      LineTracker -> Paint(hMemDc, VISN[m_iVisnId]->Img->GetScaleX() , VISN[m_iVisnId]->Img->GetScaleY() );

    //Center Cross.
    if(miCntrLine -> Checked ) {
        VISN[m_iVisnId]->PaintCntLine(hMemDc , pnDraw -> Width , pnDraw -> Height );
    }


    //  Copy buffer bitmap into window dc.
    BitBlt(hDc, 0, 0, R.right, R.bottom, hMemDc, 0, 0, SRCCOPY);



    // Delete object
    SelectObject(hMemDc,hOldMemBm);
    DeleteObject(hMemBm);
    DeleteDC(hMemDc);
    ReleaseDC(pnDraw->Handle,hDc);
//



}
//---------------------------------------------------------------------------
void __fastcall TFrmImage::FormShow(TObject *Sender)
{
     OnPaintMsg();
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btGrabClick(TObject *Sender)
{
    Trace("Operator", "Image Form_Grab Button Click");
    if(VISN[m_iVisnId]->GetLive())VISN[m_iVisnId]->Live(false);
    else                          VISN[m_iVisnId]->Grab();

    VISN[m_iVisnId]->SetViewMode(vmNone) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btInspClick(TObject *Sender)
{
    Trace("Operator", "Image Form_Inspection Button Click");
    unsigned long lInspTime = GetTickCount() ;

    int iSel = 0 ; //pcPara -> ActivePageIndex ;

    SBlob RsltBlob ;
    TCircle SetCircle ;

    float fMaxScore = 0.0 ;
    float fMaxX     = 0.0 ;
    float fMaxY     = 0.0 ;

    TPoint SttPnt , EndPnt ;
    bool bRslt ;

    if(InspCallback) InspCallback (m_iVisnId , false) ;


    lbTime -> Caption = AnsiString(GetTickCount() -  lInspTime) + "ms";


}
//---------------------------------------------------------------------------



void __fastcall TFrmImage::btLiveClick(TObject *Sender)
{
    Trace("Operator", "Image Form_Live Button Click");
    VISN[m_iVisnId]->Live(true);
    VISN[m_iVisnId]->SetViewMode(vmNone) ;
}
//---------------------------------------------------------------------------




void __fastcall TFrmImage::pcParaChange(TObject *Sender)
{

    OnPaintMsg();
}
//---------------------------------------------------------------------------




bool TFrmImage::LoadData(AnsiString _sFilePath)
{
}


bool TFrmImage::SaveData(AnsiString _sFilePath)
{


}

void TFrmImage::SetVisnId(EN_VISN_ID _iId)
{
    m_iVisnId = _iId ;

}

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
void __fastcall TFrmImage::CameraCalibration1Click(TObject *Sender)
{
    Trace("Operator", "Image Form_Camera Calibration Button Click");
   

    if(FrmCalPara == NULL) {
        FrmCalPara = new TFrmCalPara (this);
    }

    FrmCalPara -> Show(m_iVisnId) ;







}
//---------------------------------------------------------------------------



void __fastcall TFrmImage::miCntrLineClick(TObject *Sender)
{
    Trace("Operator", "Image Form_Center Line Button Click");
    miCntrLine -> Checked = !miCntrLine -> Checked ;
    btCenter   -> Down = miCntrLine -> Checked ;
    btNoCenter -> Down =!miCntrLine -> Checked ;

    OnPaintMsg();

}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::VisionParameter1Click(TObject *Sender)
{
    Trace("Operator", "Image Form_Vision Parameter Button Click");

    if(!g_bSettingMode){
        FrmVisnPara -> Parent = NULL ;
        FrmVisnPara -> BorderStyle = bsToolWindow ;
    }
    FrmVisnPara -> FormShow(m_iVisnId) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::AlghorithmSetting1Click(TObject *Sender)
{
    Trace("Operator", "Image Form_Alghorithm Setting Button Click");

    if(!g_bSettingMode){
        FrmAlgoPara -> BorderStyle = bsSizeable ;
        FrmAlgoPara -> Parent = NULL ;
    }
    FrmAlgoPara -> FormShow(m_iVisnId) ;


}
//---------------------------------------------------------------------------
void TFrmImage::SetFormEnable(bool _bTrue)
{
    if(_bTrue) {

         //트레커 안보이게.
         if(VISN[m_iVisnId]->ParaUi.pcParaBase)
             VISN[m_iVisnId]->ParaUi.pcParaBase -> ActivePageIndex = 0 ;


    }
    else {
         VISN[m_iVisnId]->Live(false);

         //트레커 안보이게
         if(VISN[m_iVisnId]->ParaUi.pcParaBase)
             VISN[m_iVisnId]->ParaUi.pcParaBase -> ActivePageIndex = 0 ;
    }
    btRealRatioClick(btRealRatio);
}

void TFrmImage::SetLevel (int _iLevel)
{

    if(_iLevel ==0) {//오퍼레벨
        lbMenu -> Enabled = false ;
        SetFormEnable( false);
    }

    else {
        lbMenu -> Enabled = true ;
    }



}


void __fastcall TFrmImage::CameraLights1Click(TObject *Sender)
{
    Trace("Operator", "Image Form_Camera Lights Button Click");
    if(FrmCamLightPara==NULL) {
        FrmCamLightPara = new TFrmCamLightPara (this);
        //delete FrmCamLightPara ;
    }

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
    Trace("Operator", "Image Form_Mag Button Click");
    if(FrmMag) {
        delete FrmMag ;
    }
    FrmMag = new TFrmMag (this);
    FrmMag -> Show() ;        
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btPixelClick(TObject *Sender)
{
    Trace("Operator", "Image Form_Pixel Button Click");
    if(FrmPixel) {
        delete FrmPixel ;
    }
    FrmPixel = new TFrmPixel (this , VISN[m_iVisnId]->Img  , LineTracker);
    FrmPixel -> Show() ;        
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btHistoClick(TObject *Sender)
{
    Trace("Operator", "Image Form_Histogram Button Click");
    if(FrmHisto) {
        delete FrmHisto ;
    }
    FrmHisto = new TFrmHisto (this , VISN[m_iVisnId]->Img , RectTracker);
    FrmHisto -> Show() ;        
}
//---------------------------------------------------------------------------








void __fastcall TFrmImage::btCenterClick(TObject *Sender)
{
    miCntrLine -> Checked = !miCntrLine -> Checked ;

    btCenter   -> Down = miCntrLine -> Checked ;
    btNoCenter -> Down =!miCntrLine -> Checked ;

    Trace("Operator", "Image Form_Center Line Button Click");
    OnPaintMsg();
}
//---------------------------------------------------------------------------


