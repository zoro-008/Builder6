//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormTrainChk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmTrainChk *FrmTrainChk=NULL;
//---------------------------------------------------------------------------
__fastcall TFrmTrainChk::TFrmTrainChk(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------9------------------------------
void __fastcall TFrmTrainChk::FormCreate(TObject *Sender)
{
    OriginalProc = pnDraw->WindowProc ;
    pnDraw->WindowProc = PanelProc    ;

    m_pTrainImg  = NULL ;
    m_pTrainArea = NULL ;

}
//---------------------------------------------------------------------------
void TFrmTrainChk::SetTrainImg (IImage * _pImg )
{
    m_pTrainImg = _pImg ;

}

//---------------------------------------------------------------------------
void TFrmTrainChk::SetTrainArea(IArea  * _pArea)
{

    m_pTrainArea = _pArea ;

}


void __fastcall TFrmTrainChk::PanelProc(Messages::TMessage &msg)
{
    OriginalProc(msg);
    if (msg.Msg == WM_PAINT ) {
        OnPaint();
    }
}

void TFrmTrainChk::OnPaint()
{

    if(m_pTrainImg  == NULL) return ;
    if(m_pTrainArea == NULL) return ;

    RECT R ;
    R.left   = 0 ;
    R.top    = 0 ;
    R.right  = pnDraw -> Width  ;
    R.bottom = pnDraw -> Height ;


    // Create buffer with memory DC and bitmap, then clear it with background.
    HDC     hDc ;
    HDC     hMemDc ;
    HBITMAP hMemBm , hOldMemBm ;


    HWND Temphwnd =  pnDraw->Handle ;

    hDc       = GetDC(pnDraw->Handle); //imDraw -> Canvas -> Handle ;  //GetDC(pnDraw->Handle);
    hMemDc    = CreateCompatibleDC(hDc);
    hMemBm    = CreateCompatibleBitmap(hDc, R.right, R.bottom);
    hOldMemBm = SelectObject(hMemDc, hMemBm);

    m_pTrainImg ->ShowImgGrey  (hMemDc, R.left , R.top , R.right , R.bottom);
    m_pTrainArea->ShowArea     (hMemDc, R.left , R.top , m_pTrainImg ->GetScaleX() , m_pTrainImg ->GetScaleY() ) ;

    //GDI ±×¸².
    //IGdi * Gdi = GDI_New(hMemDc);
    //Gdi -> Pixel(Pnt1.x , Pnt1.y, clRed);
    //Gdi -> Pixel(Pnt2.x , Pnt2.y, clBlue);
    //GDI_Del(Gdi) ;

    //  Copy buffer bitmap into window dc.
    BitBlt(hDc, 0, 0, R.right, R.bottom, hMemDc, 0, 0, SRCCOPY);



    // Delete object
    SelectObject(hMemDc,hOldMemBm);
    DeleteObject(hMemBm);
    DeleteDC(hMemDc);
    ReleaseDC(pnDraw->Handle,hDc);

}
void __fastcall TFrmTrainChk::FormDestroy(TObject *Sender)
{
    pnDraw->WindowProc = OriginalProc ;        
}
//---------------------------------------------------------------------------


void __fastcall TFrmTrainChk::cbEraserClick(TObject *Sender)
{
    AnsiString sExpand = edExpand -> Text;

    int iWid = m_pTrainImg -> GetWidth() , iHei = m_pTrainImg -> GetHeight();

    int iSmall  =  iWid < iHei ? iWid : iHei;

    bool bEraserCheck = cbEraser -> Checked;

    if (bEraserCheck){
       if(sExpand        == ""    ){ShowMessage("VALUE IS NULL."             ); cbEraser->Checked = false; return;}
       if(sExpand.ToInt() < 1     ){ShowMessage("VALUE IS LESS THAN 1."      ); cbEraser->Checked = false; return;}
       if(sExpand.ToInt() > iSmall){ShowMessage("VALUE IS MORE THAN IMG LINE"); cbEraser->Checked = false; return;}
    }
    
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrainChk::pnDrawMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(cbEraser -> Checked == true && Shift.Contains(ssLeft)){
        int iX = 0 , iY = 0;

        iX = m_pTrainImg -> GetImgX(X);
        iY = m_pTrainImg -> GetImgY(Y);

        AnsiString sExpand = edExpand -> Text.ToInt();

        if(sExpand == 1) { m_pTrainArea -> SetPixel(iX , iY , 0); OnPaint(); }
        else {
            for(int y = iY-(sExpand-1); y < iY+(sExpand-1)+1; y++){
                for(int x = iX-(sExpand-1); x < iX+(sExpand-1)+1; x++){
                    m_pTrainArea -> SetPixel(x , y , 0);
                }
            }
        OnPaint();
        }
    }    
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrainChk::pnDrawMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    if(cbEraser -> Checked == true && Shift.Contains(ssLeft)){
        int iX = 0 , iY = 0;

        iX = m_pTrainImg -> GetImgX(X);
        iY = m_pTrainImg -> GetImgY(Y);

        AnsiString sExpand = edExpand -> Text.ToInt();

        if(sExpand == 1) { m_pTrainArea -> SetPixel(iX , iY , 0); OnPaint(); }
        else {
            for(int y = iY-(sExpand-1); y < iY+(sExpand-1)+1; y++){
                for(int x = iX-(sExpand-1); x < iX+(sExpand-1)+1; x++){
                    m_pTrainArea -> SetPixel(x , y , 0);
                }
            }
        OnPaint();
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrainChk::Image2Click(TObject *Sender)
{
    if(edExpand -> Text == ""){ edExpand -> Text = 1; }
    else                      { edExpand -> Text = edExpand -> Text.ToInt() + 1;}
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrainChk::Image1Click(TObject *Sender)
{
    if(cbEraser -> Checked == true){
        cbEraser -> Checked = false;
    }
    else{
        cbEraser -> Checked = true;
    }
}
//---------------------------------------------------------------------------

