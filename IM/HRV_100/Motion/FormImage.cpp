//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormImage.h"
#include "Vision.h"
#include "CamUnit.h"
#include "OptionMan.h"
#include "Image.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmImage *FrmImage[MAX_STAGE_CNT]={NULL,NULL,NULL,NULL,NULL};
//---------------------------------------------------------------------------
__fastcall TFrmImage::TFrmImage(TComponent* Owner)
        : TForm(Owner)
{
}

void TFrmImage::SetStgId(int _iStgId)
{
    m_iStgId = _iStgId ;

    m_iActiveImg = iiSTG1Rslt + _iStgId;

    //iiSTGCrnt ;
    //iiSTG1End ;
    //iiSTG1Stt ;
    //iiSTG1Rslt ;



}

//---------------------------------------------------------------------------
void __fastcall TFrmImage::FormCreate(TObject *Sender)
{
    //ClientWidth  = 320 ;
    //ClientHeight = 240 ;

    Left = 0 ;
    Top  = 0 ;

    m_iStgId = 0 ;
    m_iActiveImg = iiSTGCrnt ;

    OriginalProc = pnDraw->WindowProc ;
    pnDraw->WindowProc = PanelProc    ;

    m_pMaxParent =  NULL ;

#ifdef COWELL  //20150915 코웰향은 그래프 안보이게.
    pnTopToolbar -> Visible = false ;
#endif




}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::PanelProc(Messages::TMessage &msg)
{
    OriginalProc(msg);
    if (msg.Msg == WM_PAINT ) {
        OnPaint();
    }
}


void TFrmImage::OnPaint()
{
    //Label1 -> Caption = String(pnDraw -> Width) + "," + String(pnDraw -> Height)  ;

    int iImgOfsX = 0 ;
    int iImgOfsY = 0 ;

    int iImgW = g_pImg[m_iActiveImg]->GetWidth () ;
    int iImgH = g_pImg[m_iActiveImg]->GetHeight() ;
    int iPanW = pnDraw -> Width  ;
    int iPanH = pnDraw -> Height ;

    //맥스화면 용.
    if(iPanW > 1280) iPanW = 1280 ; //
    if(iPanH > 960 ) iPanH = 960  ; //

    if(iImgW == 0 || iImgH == 0) return ;

    const int iFullRangePxY = g_pImg[m_iActiveImg]->GetHeight() ; //픽셀로본 풀레인지.
    const int iFullRangeMtY = OM.MstOptn.dVisnYMaxMin;            //분으로 본 풀레인지.
    const float iMtToPxRatio = iFullRangePxY / iFullRangeMtY ;  //이비율에 분을 곱하면 레인지로 나옴.


    //화면에 보여지는 이미지를 확대 하게 수정... 2014.11.29 곽채훈 대리 요청.
    int iRangeMtY     = OM.CmnOptn.dVisnYMaxDpMin ; //화면에 보여주는 분.
    int iDispPxRangeY = iRangeMtY * iMtToPxRatio ;
    int iDispPxRangeX = iDispPxRangeY * iImgW/iImgH ;
    int iDispPxOfsY   = (iImgH - iDispPxRangeY) / 2.0 ;
    int iDispPxOfsX   = (iImgW - iDispPxRangeX) / 2.0 ;

    double dMaxDispScale ;
    if(OM.MstOptn.dVisnYMaxMin) dMaxDispScale = OM.CmnOptn.dVisnYMaxDpMin / OM.MstOptn.dVisnYMaxMin ;
    else                        dMaxDispScale = 1.0 ;
    double dDispScaleX = iPanW / ((double)iImgW * dMaxDispScale) ;
    double dDispScaleY = iPanH / ((double)iImgH * dMaxDispScale) ;

    // Create buffer with memory DC and bitmap, then clear it with background.
    HDC          hDc ;
    HDC          hMemDc ;
    HBITMAP      hMemBm , hOldMemBm ;
    EN_DISP_OPTN iDpOptn ;
    RECT         Rect ;
    hDc          = GetDC(pnDraw->Handle); //imDraw -> Canvas -> Handle ;  //GetDC(pnDraw->Handle);
    GetClipBox   (hDc , &Rect) ; //DC의 갱신해야할 Rect를 받아온다.
    hMemDc       = CreateCompatibleDC(hDc);
    //hMemBm       = CreateCompatibleBitmap(hDc, iPanW , iPanH);///////////////////////////////////
    hMemBm       = CreateCompatibleBitmap(hDc, iPanW , iPanH);///////////////////////////////////
    hOldMemBm    = SelectObject(hMemDc, hMemBm); //Old멤비트맵을 저장해 두는것은 현재 선택되어 있는 오프젝트는 삭제가 안된다. 그래서 삭제할때 월래 놈으로 돌려 놓고 삭제.

    const bool bCntrLine = btCenter -> Down ;
    const int  iThresh   = 100  ;

    int iRet ;
    if(iDispPxRangeX !=0 && iDispPxRangeY != 0) {
#ifdef COWELL //코웰것은 비젼 디스플레이 하지 않는다. 남우석 차장 요청.
#else
        iRet = VSN.Paint(hMemDc,doGray, m_iActiveImg ,
                         0 , 0 , iPanW , iPanH ,
                         iDispPxOfsX , iDispPxOfsY , iDispPxRangeX , iDispPxRangeY , //0 , 0 , iImgW , iImgH ,
                         dDispScaleX ,dDispScaleY,iThresh , bCntrLine , g_VisnRslt[m_iStgId] , g_VisnRsltPnt[m_iStgId] , m_iActiveImg == iiSTG1Rslt + m_iStgId) ;
#endif
    }

    //Label2 -> Caption = iRet ;
    //GDI 그림.
    //IGdi * Gdi = GDI_New(hMemDc);
    //Gdi -> Pixel(Pnt1.x , Pnt1.y, clRed);
    //Gdi -> Pixel(Pnt2.x , Pnt2.y, clBlue);
    //GDI_Del(Gdi) ;

    iRet = BitBlt(hDc, 0 , 0 , iPanW, iPanH, hMemDc, 0, 0, SRCCOPY);



    // Delete object
    SelectObject(hMemDc,hOldMemBm);
    DeleteObject(hMemBm);
    DeleteDC(hMemDc);
    ReleaseDC(pnDraw->Handle,hDc);


}
void __fastcall TFrmImage::FormShow(TObject *Sender)
{
    int i = 0 ;
    i++;

}
//---------------------------------------------------------------------------






void __fastcall TFrmImage::btGrabClick(TObject *Sender)
{
    m_iActiveImg = iiSTGCrnt ;

    if(!VSN.GetLive()) {
        VSN.SetPaintCallback(OnPaint);
        VSN.Grab(m_iStgId);


    }
    else {
        VSN.Live(false,m_iStgId);
        VSN.SetPaintCallback(OnPaint);
        VSN.Grab(m_iStgId);

    }


}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btLiveClick(TObject *Sender)
{
   // pnDraw -> WindowProc = OriginalProc ;
    m_iActiveImg = iiSTGCrnt ;
    VSN.SetPaintCallback(OnPaint);
    VSN.Live(true,m_iStgId);

}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btLoadClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Load Button Click");

    VSN.Live(false,m_iStgId);
    if(!OpenPictureDialog1 -> Execute())return ;
    g_pImg[m_iActiveImg]->LoadBmp(OpenPictureDialog1 -> FileName.c_str());

    OnPaint();
}
//---------------------------------------------------------------------------


void __fastcall TFrmImage::btCenterClick(TObject *Sender)
{
   // bool bDown = btCenter   -> Down ;
   // btNoCenter -> Down = bDown ;

    //Trace("Operator", "Image Form_Center Line Button Click");
    OnPaint();
}
//---------------------------------------------------------------------------


void __fastcall TFrmImage::btSaveClick(TObject *Sender)
{
    //VSN.Live(false);
    if(!SavePictureDialog1 -> Execute()) return ;
    g_pImg[m_iActiveImg]->SaveBmp(SavePictureDialog1 -> FileName.c_str()) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::pnDrawClick(TObject *Sender)
{
    OnPaint();

}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::btMaxClick(TObject *Sender)
{
    if(this -> Parent != m_pMaxParent) {
        m_pOriParent = this -> Parent ;
        this -> Parent = m_pMaxParent ;
    }
    else {
        this -> Parent = m_pOriParent ;
    }

}
//---------------------------------------------------------------------------

void __fastcall TFrmImage::Timer1Timer(TObject *Sender)
{
    btMax -> Enabled = !VSN.GetLive();
}
//---------------------------------------------------------------------------

void TFrmImage::SetMaxParent(TWinControl * _pMaxParent)
{
    m_pMaxParent = _pMaxParent ;

}
void __fastcall TFrmImage::Label2Click(TObject *Sender)
{
//    TLiveRslt tRslt ;
//    VSN.LiveInspection(iiSTGCrnt , &tRslt);
    //VSN.m_tLiveRslt = tRslt ;
}
//---------------------------------------------------------------------------

