//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMag.h"
#include "SLogUnit.h"
//#include "FormMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"

TFrmMag *FrmMag = NULL;

#define MAGNIFYING_INTERVAL 0

//---------------------------------------------------------------------------

__fastcall TFrmMag::TFrmMag(TComponent* Owner) : TForm(Owner)
{
    Left = 100 ;
    Top  = 100 ;
}

//---------------------------------------------------------------------------

void __fastcall TFrmMag::FormCreate(TObject *Sender)
{

     // Bitmap 생성
     m_pBmp   = new Graphics::TBitmap();
     m_iScale = 5;

     m_iCrntX = 0;
     m_iCrntY = 0;

     DoubleBuffered = true;

     this->Left = 10;
     this->Top  = 10;

     SetWindowSize();

     tmUpdate -> Enabled = false ;

}

//---------------------------------------------------------------------------

void __fastcall TFrmMag::SetWindowSize()
{

     m_pBmp->Width  = imMagnifying->Width  / m_iScale;
     m_pBmp->Height = imMagnifying->Height / m_iScale;

}

//---------------------------------------------------------------------------

void __fastcall TFrmMag::FormDestroy(TObject *Sender)
{
     tmUpdate -> Enabled = false ;
     delete m_pBmp;
     
     static int i = 0 ;
     //Trace("",AnsiString(i).c_str());
     i++;

}

//---------------------------------------------------------------------------

void __fastcall TFrmMag::tmUpdateTimer(TObject *Sender)
{
     tmUpdate -> Enabled = false ;


     UpdateView();

     tmUpdate -> Enabled = true ;
}

void __fastcall TFrmMag::UpdateView()
{
     tagPOINT tpPosition;
     HWND     itDC;
     int      iSourceX, iSourceY, iWinX, iWinY ;
     int      iMagnifyX, iMagnifyY;

     GetCursorPos(&tpPosition);

     //if(m_iPreX == tpPosition.x && m_iPreY == tpPosition.y) return ;

     m_iPreX = tpPosition.x;
     m_iPreY = tpPosition.y;

     // 화면을 캡쳐한 후 Zoom 시킴
     iSourceX = m_iPreX - (imMagnifying->Width  / 2 / m_iScale);
     iSourceY = m_iPreY - (imMagnifying->Height / 2 / m_iScale);

     if(iSourceX < 0) iSourceX = 0;
     if(iSourceY < 0) iSourceY = 0;
     if(iSourceX + m_pBmp->Width  > Screen->Width)  iSourceX = Screen->Width  - m_pBmp->Width;
     if(iSourceY + m_pBmp->Height > Screen->Height) iSourceY = Screen->Height - m_pBmp->Height;


     iMagnifyX = ((m_iPreX - iSourceX) * m_iScale) + imMagnifying->Left;
     iMagnifyY = ((m_iPreY - iSourceY) * m_iScale) + imMagnifying->Top;

     itDC = GetDC(0);
     BitBlt(m_pBmp->Canvas->Handle, 0, 0,
            m_pBmp->Width, m_pBmp->Height, itDC,
            iSourceX, iSourceY,
            SRCCOPY);
     ReleaseDC(0, itDC);

     imMagnifying->Picture->Bitmap->Assign(m_pBmp);


//     imMagnifying->Picture->Bitmap->Canvas->Draw(
//       (iMagnifyX / m_iScale - 15) + m_iCrntX,
//       (iMagnifyY / m_iScale - 15) + m_iCrntY,
//       imCursor->Picture->Graphic
//     );

     //DrawIcon(imMagnifying->Picture->Bitmap->Canvas->Handle,
     //         (iMagnifyX / m_iScale - 15) + m_iCrntX,
     //         (iMagnifyY / m_iScale - 15) + m_iCrntY,
     //         LoadCursor(HInstance, IDC_ARROW));

}

//---------------------------------------------------------------------------

void __fastcall TFrmMag::FormResize(TObject *Sender)
{
    SetWindowSize();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMag::edScaleChange(TObject *Sender)
{
    m_iScale = edScale -> Value ;
    SetWindowSize();
    UpdateView();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMag::FormClose(TObject *Sender, TCloseAction &Action)
{
    //Action = caFree;
    tmUpdate -> Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMag::FormShow(TObject *Sender)
{
    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------

