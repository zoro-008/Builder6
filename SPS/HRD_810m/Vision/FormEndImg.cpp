//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormEndImg.h"
#include "VOptnMan.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmEndImg *FrmEndImg = NULL;

//Seq쓰레드에서 써도 되는 함수들.




//---------------------------------------------------------------------------
__fastcall TFrmEndImg::TFrmEndImg(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TFrmEndImg::FormCreate(TObject *Sender)
{
    //EndCam = new CImi();
    //EndCam -> Init(ciEnd);

    Img = IMG_New();

    Img->SetSize(1600,1200,8);

    OriginalProc = pnDraw->WindowProc ;
    pnDraw->WindowProc = PanelProc    ;

    Height = 634 ;
    Width  = 808 ;

    m_bGrabbing = false ;
    m_bLive     = false ;
}

//---------------------------------------------------------------------------
void __fastcall TFrmEndImg::FormDestroy(TObject *Sender)
{
    pnDraw->WindowProc = OriginalProc ;

    IMG_Del(Img);

    //delete EndCam ;


}


//---------------------------------------------------------------------------
void __fastcall TFrmEndImg::PanelProc(Messages::TMessage &msg)
{
    OriginalProc(msg);
    if (msg.Msg == WM_PAINT ) {
        OnPaint();
    }
}

//---------------------------------------------------------------------------
void __fastcall TFrmEndImg::tmCloseTimer(TObject *Sender)
{
    tmClose -> Enabled = false ;

    Close();
}

//---------------------------------------------------------------------------

//카메라에 등록해주는 콜백.
void TFrmEndImg::CamCallback (int _iCamNo)
{
    unsigned char * pImg ;
    int iImgWidth  ;
    int iImgHeight ;
    int iImgBit    ;

    Cam[ciEnd] -> GetImg(pImg , iImgWidth , iImgHeight , iImgBit);

    Img->SetImage(pImg , iImgWidth , iImgHeight , iImgBit);

    //시퀜스에서 쓰면 크로스 쓰레드다. 그래서 메세지로 날림. PostMessage는 씹힐 확율 존재함.
    //시퀜스쓰레드가 아니고 커널 혹은 imi dll 쓰레드 이다.
    //OnPaint();

    //폼이 안떠있을때 그냥 메세지 날리면 프로그램 엄.
    if(Showing)PostMessage(pnDraw -> Handle , WM_PAINT , 0 , 0);





    m_bGrabbing = false ;
    if(m_bLive)Grab();
}

//---------------------------------------------------------------------------

//씨퀜스 쓰레드에서 써도 안전하다.
void TFrmEndImg::Grab()
{
    //Live Off.
    //tmLive -> Enabled = false ;
    if(m_bGrabbing) return ;
    //Lights Setting
    //SetLight();

    //Camera Setting.
    Cam[ciEnd] -> SetExposure(VOM.VDevOptn.iEndExposure); //일단 파라로.
    Cam[ciEnd] -> SetGrabFunc(CamCallback) ;
    //Sleep(50);

    //Grab
    bool bRet = Cam[ciEnd] -> Grab() ;
    if(bRet) m_bGrabbing = true ;
}

void TFrmEndImg::Live(bool _bOn)
{
    //tmLive -> Enabled = _bOn ;


    m_bLive = _bOn ;
    if(_bOn){
        if(!m_bGrabbing)Grab();
    }

}

bool TFrmEndImg::GetGrabEnd()
{
    return !m_bGrabbing ;
}

bool TFrmEndImg::SaveImg(String _sFilePath)
{
    return Img -> SaveBmp( _sFilePath.c_str());

}

void TFrmEndImg::OnPaint()
{



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


    Img->ShowImgGrey  (hMemDc, R.left , R.top , R.right , R.bottom);


    //GDI 그림.
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
void __fastcall TFrmEndImg::FormShow(TObject *Sender)
{
    Trace("TFrmEndImg","Show()");
    edExps -> Text = VOM.VDevOptn.iEndExposure ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmEndImg::Button1Click(TObject *Sender)
{
    VOM.VDevOptn.iEndExposure = StrToFloatDef(edExps -> Text , VOM.VDevOptn.iEndExposure) ;
    edExps -> Text = VOM.VDevOptn.iEndExposure ;

    VOM.LoadJobFile(false);
}
//---------------------------------------------------------------------------

void __fastcall TFrmEndImg::Button2Click(TObject *Sender)
{
    Grab();
}
//---------------------------------------------------------------------------

void __fastcall TFrmEndImg::btSaveAsClick(TObject *Sender)
{
    if(!SavePictureDialog1 -> Execute()) return ;
    Img->SaveBmp(SavePictureDialog1 -> FileName.c_str()) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmEndImg::Button3Click(TObject *Sender)
{
    Cam[ciEnd] -> SetTrgMode(tmSw); 
}
//---------------------------------------------------------------------------

void __fastcall TFrmEndImg::Button4Click(TObject *Sender)
{
    Cam[ciEnd] -> SetTrgMode(tmLive);
}
//---------------------------------------------------------------------------

void __fastcall TFrmEndImg::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Cam[ciEnd] -> SetTrgMode(tmSw);         
}
//---------------------------------------------------------------------------

