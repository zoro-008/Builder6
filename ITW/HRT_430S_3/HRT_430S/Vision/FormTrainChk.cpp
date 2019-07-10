//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormTrainChk.h"
#include "UserIni.h"

#include "UnitGdi.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#pragma resource "Cursor.res"
TFrmTrainChk *FrmTrainChk=NULL ;


//---------------------------------------------------------------------------
__fastcall TFrmTrainChk::TFrmTrainChk(TComponent* Owner)
        : TForm(Owner)
{




}

//---------------------------------------------------------------------------
class TCanvasPanel : public TPanel
{
public:
    __property Canvas;
};
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
void __fastcall TFrmTrainChk::FormCreate(TObject *Sender)
{
    OriginalProc = pnDraw->WindowProc ;
    pnDraw->WindowProc = PanelProc    ;

    m_pTrainImg  = NULL ;
    m_pTrainArea = NULL ;

}
//---------------------------------------------------------------------------
void TFrmTrainChk::SetTrainImg (CImage * _pImg )
{
    m_pTrainImg = _pImg ;

}

//---------------------------------------------------------------------------
void TFrmTrainChk::SetTrainArea(CArea   * _pArea)
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
    int DstX = 0 ;
    int DstY = 0 ;
    int DstW = pnDraw -> Width  ;
    int DstH = pnDraw -> Height ;

    int SrcX = 0 ;
    int SrcY = 0 ;
    int SrcW = m_pTrainImg -> GetWidth() ;
    int SrcH = m_pTrainImg -> GetHeight() ;

    // Create buffer with memory DC and bitmap, then clear it with background.
    HDC     hDc ;
    HDC     hMemDc ;
    HBITMAP hMemBm , hOldMemBm ;


    HWND Temphwnd =  pnDraw->Handle ;

    hDc       = GetDC(pnDraw->Handle); //imDraw -> Canvas -> Handle ;  //GetDC(pnDraw->Handle);
    hMemDc    = CreateCompatibleDC(hDc);
    hMemBm    = CreateCompatibleBitmap(hDc, DstW, DstH);
    hOldMemBm = SelectObject(hMemDc, hMemBm);

    m_pTrainImg ->ShowImgGrey  (hMemDc, DstX , DstY , DstW , DstH , SrcX , SrcY , SrcW , SrcH);
    m_pTrainArea->ShowArea     (hMemDc, DstX , DstY , m_pTrainImg ->GetScaleX() , m_pTrainImg ->GetScaleY() ) ;

    //GDI 그림....지우개 렉트 그리기

    double dOneWid , dOneHei , dPnWidth , dPnHeight , dImWidth , dImHeight;

    double dErsrRange , dCrossLine;

    int iTemp ;

    dPnWidth  = pnDraw -> Width;
    dPnHeight = pnDraw -> Height;




    dImWidth  = m_pTrainImg -> GetWidth ();
    dImHeight = m_pTrainImg -> GetHeight();

    if(dImWidth  <= 0)return ;
    if(dImHeight <= 0)return ;

    dOneWid   = dPnWidth  / dImWidth ;
    dOneHei   = dPnHeight / dImHeight;

    if(rgErsrVal -> ItemIndex == 0) { iTemp = 1; }
    if(rgErsrVal -> ItemIndex == 1) { iTemp = 2; }
    if(rgErsrVal -> ItemIndex == 2) { iTemp = 3; }
    if(rgErsrVal -> ItemIndex == 3) { iTemp = 4; }
    if(rgErsrVal -> ItemIndex == 4) { iTemp = 5; }

    dErsrRange = ((double)iTemp+(double)iTemp-1)/2;

    CGdi * Gdi = new CGdi(hMemDc);


    Gdi -> m_tPen.Color = clWhite;
    Gdi -> m_tPen.Width = 2;

         if(rgErsrVal -> ItemIndex == 0){ dCrossLine = 0;                    }
    else if(dOneWid <= dOneHei)         { dCrossLine = dOneWid * dErsrRange; }
    else                                { dCrossLine = dOneHei * dErsrRange; }

    if(btEraser -> Down) {
        Gdi -> Rect(false , m_iPointX - (dOneWid * dErsrRange) , m_iPointY - (dOneHei * dErsrRange) ,
                            m_iPointX + (dOneWid * dErsrRange) , m_iPointY + (dOneHei * dErsrRange) , dCrossLine);
    }

    delete Gdi ;

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



void __fastcall TFrmTrainChk::pnDrawMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(m_pTrainImg -> GetWidth () <=0) return ;
    if(m_pTrainImg -> GetHeight() <=0) return ;

    int iX = 0 , iY = 0;

    iX = m_pTrainImg -> GetImgX(X);
    iY = m_pTrainImg -> GetImgY(Y);

    int iTempX , iTempY;

    int iBtDownTag;
    if(btCursor  -> Down == true) { iBtDownTag = 0; }
    //if(btSpoite  -> Down == true) { iBtDownTag = 1; }
    if(btPencleP -> Down == true) { iBtDownTag = 2; }
    if(btPencleY -> Down == true) { iBtDownTag = 3; }
    if(btEraser  -> Down == true) { iBtDownTag = 4; }

    if(iBtDownTag == 0) {
        pnDraw -> Cursor = crDefault;

    }
    if(iBtDownTag == 1) {
        TCursor crPen = TCursor(5);
        Screen->Cursors[crPen] = LoadCursor(HInstance, "SPOITE");
        pnDraw->Cursor = crPen;

        int iValue = m_pTrainImg -> GetPixel(iX,iY);

        //SerchEdge(X,Y,iValue);

    }
    if(iBtDownTag == 2) {
        TCursor crPen = TCursor(5);
        Screen->Cursors[crPen] = LoadCursor(HInstance, "PEN");
        pnDraw->Cursor = crPen;

        if(Shift.Contains(ssLeft)){
            m_pTrainArea -> SetPixel(iX , iY , 3);
            OnPaint();
        }
    }
    if(iBtDownTag == 3) {
        TCursor crPen = TCursor(5);
        Screen->Cursors[crPen] = LoadCursor(HInstance, "PEN");
        pnDraw->Cursor = crPen;

        if(Shift.Contains(ssLeft)){
            m_pTrainArea -> SetPixel(iX , iY , 2);
            OnPaint();
        }
    }
    if(iBtDownTag == 4) {
        if(rgErsrVal -> ItemIndex == 0) { iTempX = 1; iTempY = 1; }
        if(rgErsrVal -> ItemIndex == 1) { iTempX = 2; iTempY = 2; }
        if(rgErsrVal -> ItemIndex == 2) { iTempX = 3; iTempY = 3; }
        if(rgErsrVal -> ItemIndex == 3) { iTempX = 4; iTempY = 4; }
        if(rgErsrVal -> ItemIndex == 4) { iTempX = 5; iTempY = 5; }

        m_iPointX = X , m_iPointY = Y ;

        pnDraw -> Cursor = crNone;
        //TCanvasPanel *pCanvasPanel = (TCanvasPanel *)pnDraw;
        //pCanvasPanel->Canvas->Brush -> Color = clWhite;
        //pCanvasPanel->Canvas->Rectangle(X - (dOneWid * dErsrRange) , Y - (dOneHei * dErsrRange) ,
        //                                X + (dOneWid * dErsrRange) , Y + (dOneHei * dErsrRange));

        if(Shift.Contains(ssLeft)){
            for(int y = iY-(iTempY-1); y < (iY+iTempY); y++){
                for(int x = iX-(iTempX-1); x < (iX+iTempX); x++){
                        m_pTrainArea -> SetPixel(x , y , 0);

                }
            }
        }
        OnPaint();
    }

}
//---------------------------------------------------------------------------

void __fastcall TFrmTrainChk::pnDrawMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    if(m_pTrainImg -> GetWidth () <=0) return ;
    if(m_pTrainImg -> GetHeight() <=0) return ;

    int iX = 0 , iY = 0;

    iX = m_pTrainImg -> GetImgX(X);
    iY = m_pTrainImg -> GetImgY(Y);

    int iTempX , iTempY;

    int iBtDownTag;
    if(btCursor  -> Down == true) { iBtDownTag = 0; }
    //if(btSpoite  -> Down == true) { iBtDownTag = 1; }
    if(btPencleP -> Down == true) { iBtDownTag = 2; }
    if(btPencleY -> Down == true) { iBtDownTag = 3; }
    if(btEraser  -> Down == true) { iBtDownTag = 4; }

    int PxValue  = m_pTrainImg->GetPixel(iX , iY);

    AnsiString Gray ;
    int DeGray ;

    Gray   = "0x" + IntToHex(PxValue,2) + IntToHex(PxValue,2) + IntToHex(PxValue,2);
    DeGray = StrToIntDef(Gray, 0);

    if (PxValue <128) pnValue -> Font -> Color = clWhite ;
    else              pnValue -> Font -> Color = clBlack ;

    pnValue -> Color = DeGray ;
    pnValue -> Caption = PxValue;



    if(iBtDownTag == 0) {
        pnDraw -> Cursor = crDefault;

    }
    if(iBtDownTag == 1) {
        TCursor crPen = TCursor(5);
        Screen->Cursors[crPen] = LoadCursor(HInstance, "SPOITE");
        pnDraw->Cursor = crPen;

    }
    if(iBtDownTag == 2) {
        TCursor crPen = TCursor(5);
        Screen->Cursors[crPen] = LoadCursor(HInstance, "PEN");
        pnDraw->Cursor = crPen;

        if(Shift.Contains(ssLeft)){

            m_pTrainArea -> SetPixel(iX , iY , 3);
            OnPaint();
        }
    }
    if(iBtDownTag == 3) {
        TCursor crPen = TCursor(5);
        Screen->Cursors[crPen] = LoadCursor(HInstance, "PEN");
        pnDraw->Cursor = crPen;

        if(Shift.Contains(ssLeft)){
            m_pTrainArea -> SetPixel(iX , iY , 2);
            OnPaint();
        }
    }
    if(iBtDownTag == 4) {
        if(rgErsrVal -> ItemIndex == 0) { iTempX = 1; iTempY = 1; }
        if(rgErsrVal -> ItemIndex == 1) { iTempX = 2; iTempY = 2; }
        if(rgErsrVal -> ItemIndex == 2) { iTempX = 3; iTempY = 3; }
        if(rgErsrVal -> ItemIndex == 3) { iTempX = 4; iTempY = 4; }
        if(rgErsrVal -> ItemIndex == 4) { iTempX = 5; iTempY = 5; }

        m_iPointX = X , m_iPointY = Y ;

        pnDraw -> Cursor = crNone;
        //TCanvasPanel *pCanvasPanel = (TCanvasPanel *)pnDraw;
        //pCanvasPanel->Canvas->Brush -> Color = clWhite;
        //pCanvasPanel->Canvas->Rectangle(X - (dOneWid * dErsrRange) , Y - (dOneHei * dErsrRange) ,
        //                                X + (dOneWid * dErsrRange) , Y + (dOneHei * dErsrRange));

        if(Shift.Contains(ssLeft)){
            for(int y = iY-(iTempY-1); y < (iY+iTempY); y++){
                for(int x = iX-(iTempX-1); x < (iX+iTempX); x++){
                        m_pTrainArea -> SetPixel(x , y , 0);
                }
            }
        }
        OnPaint();
    }
}
//---------------------------------------------------------------------------


void __fastcall TFrmTrainChk::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false;

    if(btEraser -> Down == true){ rgErsrVal -> Visible = true ;}
    else                        { rgErsrVal -> Visible = false;}

    OnPaint();

    tmUpdate -> Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrainChk::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate -> Enabled = false;

    Free();
}
//---------------------------------------------------------------------------

void __fastcall TFrmTrainChk::FormShow(TObject *Sender)
{
    tmUpdate -> Enabled = true;
}
//---------------------------------------------------------------------------

bool TFrmTrainChk::SerchEdge(int _iX , int _iY , int _iValue)
{
/*
    int iImWidth  = m_pTrainImg -> GetWidth ();
    int iImHeight = m_pTrainImg -> GetHeight();
    int iPnWidth  = pnDraw -> Width  - 2 ;
    int iPnHeight = pnDraw -> Height - 2 ;

    int iUpEdCnt = 0 , iDnEdCnt = 0;
    int iSumUpEd = 0 , iSumDnEd = 0;

    int iLTVal    = 0, iTopVal    = 0, iRTVal    = 0, iRghVal    = 0, 
        iPreLTVal = 0, iPreTopVal = 0, iPreRTVal = 0, iPreRghVal = 0,
        iRBVal    = 0, iBttVal    = 0, iLBVal    = 0, iLftVal    = 0, 
        iPreRBVal = 0, iPreBttVal = 0, iPreLBVal = 0, iPreLftVal = 0;

    bool bUpEdSerch = false , bDnEdSerch = false ;

    int iValue  = _iValue;
    int iAvrEd  ;

    // 왼쪽 위 다운 엣지 업 엣지 찾기...


    for(int i = _iX ; i > 1 ; i--){
        for(int j = _iY ; j > 1 ; j--){
            if(m_pTrainImg -> GetImgX(i) > 1 || m_pTrainImg -> GetImgY(j) > 1){
                iLTVal    = m_pTrainImg->GetPixel(m_pTrainImg->GetImgX(i  ) , m_pTrainImg->GetImgY(j  ));
                iPreLTVal = m_pTrainImg->GetPixel(m_pTrainImg->GetImgX(i-1) , m_pTrainImg->GetImgY(j-1));
                if(!bDnEdSerch && iLTVal - iPreLTVal > 19){
                    iSumDnEd += iPreLTVal;
                    iDnEdCnt ++;
                    bDnEdSerch = true;
                }
                if(!bUpEdSerch && iPreLTVal - iLTVal > 19){
                    iSumUpEd += iPreLTVal;
                    iUpEdCnt ++;
                    bUpEdSerch = true;
                }
                if(bUpEdSerch && bDnEdSerch ){
                    break;
                }
            }
        }
    }

    // 위쪽 다운 엣지 업 엣지 찾기...
    bUpEdSerch = false;
    bDnEdSerch = false;

    for(int j = _iY ; j > 1 ; j--){
        if(m_pTrainImg -> GetImgY(j) > 1){
            iTopVal    = m_pTrainImg->GetPixel(m_pTrainImg->GetImgX(_iX) , m_pTrainImg->GetImgY(j  ));
            iPreTopVal = m_pTrainImg->GetPixel(m_pTrainImg->GetImgX(_iX) , m_pTrainImg->GetImgY(j-1));
            if(!bDnEdSerch && iTopVal - iPreTopVal > 19){
                iSumDnEd += iPreTopVal;
                iDnEdCnt ++;
                bDnEdSerch = true;
            }
            if(!bUpEdSerch && iPreTopVal - iTopVal > 19){
                iSumUpEd += iPreTopVal;
                iUpEdCnt ++;
                bUpEdSerch = true;
            }
            if(bUpEdSerch && bDnEdSerch){
                break;
            }
        }
    }

    // 오른쪽 위 다운 엣지 업 엣지 찾기...
    bUpEdSerch = false;
    bDnEdSerch = false;

    for(int i = _iX ; i > iPnWidth ; i++){
        for(int j = _iY ; j > 1 ; j--){
            if(m_pTrainImg -> GetImgX(i) < iImWidth - 1 || m_pTrainImg -> GetImgY(j) > 1){
                iRTVal    = m_pTrainImg->GetPixel(m_pTrainImg->GetImgX(i  ) , m_pTrainImg->GetImgY(j  ));
                iPreRTVal = m_pTrainImg->GetPixel(m_pTrainImg->GetImgX(i+1) , m_pTrainImg->GetImgY(j-1));
                if(!bDnEdSerch && iRTVal - iPreRTVal > 19){
                    iSumDnEd += iPreRTVal;
                    iDnEdCnt ++;
                    bDnEdSerch = true;
                }
                if(!bUpEdSerch && iPreRTVal - iRTVal > 19){
                    iSumUpEd += iPreRTVal;
                    iUpEdCnt ++;
                    bUpEdSerch = true;
                }
                if(bUpEdSerch && bDnEdSerch){
                    break;
                }
            }
        }
    }

    // 오른쪽 다운 엣지 업 엣지 찾기...
    bUpEdSerch = false;
    bDnEdSerch = false;

    for(int i = _iX ; i > iPnWidth ; i++){
        if(m_pTrainImg -> GetImgX(i) < iImWidth - 1){
            iRghVal    = m_pTrainImg->GetPixel(m_pTrainImg->GetImgX(i  ) , m_pTrainImg->GetImgY(_iY));
            iPreRghVal = m_pTrainImg->GetPixel(m_pTrainImg->GetImgX(i+1) , m_pTrainImg->GetImgY(_iY));
            if(!bDnEdSerch && iRghVal - iPreRghVal > 19){
                iSumDnEd += iPreRghVal;
                iDnEdCnt ++;
                bDnEdSerch = true;
            }
            if(!bUpEdSerch && iPreRghVal - iRghVal > 19){
                iSumUpEd += iPreRghVal;
                iUpEdCnt ++;
                bUpEdSerch = true;
            }
            if(bUpEdSerch && bDnEdSerch){
                break;
            }
        }
    }

    // 오른쪽 아래 다운 엣지 업 엣지 찾기...
    bUpEdSerch = false;
    bDnEdSerch = false;

    for(int i = _iX ; i < iPnWidth ; i++){
        for(int j = _iY ; j < iPnHeight ; j++){
            if(m_pTrainImg -> GetImgX(i) < iImWidth - 1 || m_pTrainImg -> GetImgY(j) < iImHeight - 1){
                iRBVal    = m_pTrainImg->GetPixel(m_pTrainImg->GetImgX(i  ) , m_pTrainImg->GetImgY(j  ));
                iPreRBVal = m_pTrainImg->GetPixel(m_pTrainImg->GetImgX(i+1) , m_pTrainImg->GetImgY(j+1));
                if(!bDnEdSerch && iRBVal - iPreRBVal > 19){
                    iSumDnEd += iPreRBVal;
                    iDnEdCnt ++;
                    bDnEdSerch = true;
                }
                if(!bUpEdSerch && iPreRBVal - iRBVal > 19){
                    iSumUpEd += iPreRBVal;
                    iUpEdCnt ++;
                    bUpEdSerch = true;
                }
                if(bUpEdSerch && bDnEdSerch){
                    break;
                }
            }
        }
    }

    // 아래쪽 다운 엣지 업 엣지 찾기...
    bUpEdSerch = false;
    bDnEdSerch = false;

    for(int j = _iY ; j < iPnHeight ; j++){
        if(m_pTrainImg -> GetImgY(j) < iImHeight - 1){
            iBttVal    = m_pTrainImg->GetPixel(m_pTrainImg->GetImgX(_iX) , m_pTrainImg->GetImgY(j  ));
            iPreBttVal = m_pTrainImg->GetPixel(m_pTrainImg->GetImgX(_iX) , m_pTrainImg->GetImgY(j+1));
            if(!bDnEdSerch && iBttVal - iPreBttVal > 19){
                iSumDnEd += iPreBttVal;
                iDnEdCnt ++;
                bDnEdSerch = true;
            }
            if(!bUpEdSerch && iPreBttVal - iBttVal > 19){
                iSumUpEd += iPreBttVal;
                iUpEdCnt ++;
                bUpEdSerch = true;
            }
            if(bUpEdSerch && bDnEdSerch){
                break;
            }
        }
    }

    // 왼쪽 아래 다운 엣지 업 엣지 찾기...
    bUpEdSerch = false;
    bDnEdSerch = false;

    for(int i = _iX ; i > 1 ; i--){
        for(int j = _iY ; j < iPnHeight ; j++){
            if(m_pTrainImg -> GetImgX(i) > 1 || m_pTrainImg -> GetImgY(j) < iImHeight - 1){
                iLBVal    = m_pTrainImg->GetPixel(m_pTrainImg->GetImgX(i  ) , m_pTrainImg->GetImgY(j  ));
                iPreLBVal = m_pTrainImg->GetPixel(m_pTrainImg->GetImgX(i-1) , m_pTrainImg->GetImgY(j+1));
                if(!bDnEdSerch && iLBVal - iPreLBVal > 19){
                    iSumDnEd += iPreLBVal;
                    iDnEdCnt ++;
                    bDnEdSerch = true;
                }
                if(!bUpEdSerch && iPreLBVal - iLBVal > 19){
                    iSumUpEd += iPreLBVal;
                    iUpEdCnt ++;
                    bUpEdSerch = true;
                }
                if(bUpEdSerch && bDnEdSerch){
                    break;
                }
            }
        }
    }

    // 왼쪽 다운 엣지 업 엣지 찾기...
    bUpEdSerch = false;
    bDnEdSerch = false;

    for(int i = _iX ; i > 1 ; i--){
        if(m_pTrainImg -> GetImgX(i) > 1){
            iLftVal    = m_pTrainImg->GetPixel(m_pTrainImg->GetImgX(i  ) , m_pTrainImg->GetImgY(_iY));
            iPreLftVal = m_pTrainImg->GetPixel(m_pTrainImg->GetImgX(i-1) , m_pTrainImg->GetImgY(_iY));
            if(!bDnEdSerch && iLftVal - iPreLftVal > 19){
                iSumDnEd += iPreLftVal;
                iDnEdCnt ++;
                bDnEdSerch = true;
            }
            if(!bUpEdSerch && iPreLftVal - iLftVal > 19){
                iSumUpEd += iPreLftVal;
                iUpEdCnt ++;
                bUpEdSerch = true;
            }
            if(bUpEdSerch && bDnEdSerch){
                break;
            }
        }
    }

         if(iDnEdCnt > iUpEdCnt) { iAvrEd = iSumDnEd / iDnEdCnt; }
    else if(iDnEdCnt < iUpEdCnt) { iAvrEd = iSumUpEd / iUpEdCnt; }
    else if(iDnEdCnt == iUpEdCnt){ if(iValue < 128){ iAvrEd = iSumUpEd / iUpEdCnt; }
                                   else            { iAvrEd = iSumDnEd / iDnEdCnt; } }

    Panel2 -> Caption = iAvrEd;

    TUserINI UserINI ;

    AnsiString sTemp = "Orig_Ocv" ;

    //AnsiString sFilePath = g_sJobFilePath + "Epoxy.INI" ;

    UserINI.Save(sFilePath, sTemp , "iThreshold", iAvrEd);//_tPara.iTrThreshold);

    VISN[m_iVisnId]->Train();

    return true;

*/

}

//---------------------------------------------------------------------------


