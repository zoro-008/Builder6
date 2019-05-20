//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormGraph.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFrmGraph *FrmGraph[MAX_STAGE_CNT]={NULL,NULL,NULL,NULL,NULL};
//---------------------------------------------------------------------------

__fastcall TFrmGraph::TFrmGraph(TComponent* Owner)
        : TForm(Owner)
{
    pnBase -> DoubleBuffered = true;
    m_pIncData    = NULL ;
    m_iIncDataCnt = 0 ;

    m_pDecData    = NULL ;
    m_iDecDataCnt = 0 ;


    Para.m_dLeftX    = 0   ;//-5  ;
    Para.m_dBottomY  = 0   ;//-10 ;
    Para.m_dRightX   = 85  ;
    Para.m_dTopY     = 300 ;
    Para.m_dStepX    = (int)(Para.m_dRightX - Para.m_dLeftX  ) / 8 ; //10  ;
    Para.m_dStepY    = (int)(Para.m_dTopY   - Para.m_dBottomY) / 15; //20  ;
    Para.m_dBigStepY = (int)(Para.m_dTopY   - Para.m_dBottomY) / 6 ; //50  ;//큰눈금

    Para.m_iLOfsX    = 24 ;
    Para.m_iROfsX    = 10 ;//15  ;
    Para.m_iTOfsY    = 5  ;//15  ;
    Para.m_iBOfsY    = 20 ;


    OriginalProc = pnBase -> WindowProc ;
    pnBase -> WindowProc = PanelProc ;

    m_pMaxParent =  NULL ;

    //SetIncDataString("-0.001,-0.002,-0.002,-0.003,-0.004,-0.004,-0.004,-0.005,-0.005,-0.005,-0.005,-0.005,-0.006,-0.006,-0.006,-0.006,-0.006,-0.006,-0.006,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.002,0.005,0.011,0.018,0.024,0.031,0.037,0.045,0.051,0.058,0.064,0.071,0.078,0.085,0.092,0.098,0.105,0.111,0.118,0.124,0.13,0.136,0.142,0.148,0.154,0.16,0.166,0.171,0.178,0.183,0.189,0.194,0.2,0.206,0.211,0.216,0.221,0.227,0.232,0.237,0.242,0.247,0.251,0.257,0.261,0.266,0.27,0.273,0.274,0.275,0.276,0.277,0.278,0.279");
    //SetDecDataString("-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.007,-0.002,0.005,0.011,0.018,0.025,0.032,0.038,0.045,0.052,0.059,0.065,0.072,0.078,0.085,0.092,0.099,0.106,0.112,0.118,0.124,0.131,0.137,0.144,0.149,0.156,0.161,0.167,0.172,0.179,0.184,0.19,0.195,0.201,0.207,0.212,0.217,0.222,0.227,0.232,0.237,0.242,0.247,0.252,0.256,0.261,0.266,0.27,0.273,0.274,0.276,0.277,0.278,0.278,0.279");

}

void TFrmGraph::SetStgId(int _iStgId)
{
    m_iStgId = _iStgId ;

}
//---------------------------------------------------------------------------
void __fastcall TFrmGraph::PanelProc(Messages::TMessage &msg)
{
    OriginalProc(msg);
    if (msg.Msg == WM_PAINT) {
        UpdateView();
    }
}
//---------------------------------------------------------------------------
void TFrmGraph::SetGraphPara(TGraphPara _tPara)
{
    Para = _tPara ;
}

double TFrmGraph::GetXOnGraph(TRect _tRect , double _dData)
{
    double dRet = 0.0 ;
    double dGap = Para.m_dRightX - Para.m_dLeftX ;

    if(dGap == 0.0) return dRet ;

    double dScale = _tRect.Width()/dGap  ;


    if(dScale > 0) { //정상적으로 적은게 왼쪽 많은게 오른쪽일 경우.
        dRet = _tRect.left +( _dData - Para.m_dLeftX) * dScale ;
    }
    else {
        dRet =  _tRect.right + ( _dData - Para.m_dRightX) * dScale;
    }

    return dRet ;
}

double TFrmGraph::GetYOnGraph(TRect _tRect , double _dData)
{
    double dRet = 0.0 ;
    double dGap = Para.m_dBottomY - Para.m_dTopY ;

    if(dGap == 0.0) return dRet ;

    double dScale = _tRect.Height()/dGap  ;


    if(dScale > 0) { //정상적으로 적은게 왼쪽 많은게 오른쪽일 경우.
        dRet =  _tRect.Top + ( _dData - Para.m_dTopY) * dScale ;
    }
    else {
        dRet = _tRect.Bottom + ( _dData - Para.m_dBottomY) * dScale  ;
    }

    return dRet ;

}

void __fastcall TFrmGraph::UpdateView()
{
    TRect    Rect ;
    Rect.left   = Para.m_iLOfsX ;
    Rect.top    = Para.m_iTOfsY ;
    Rect.right  = pnBase -> ClientWidth  - Para.m_iROfsX ;
    Rect.bottom = pnBase -> ClientHeight - Para.m_iBOfsY ;




    int iPanW = pnBase -> Width  ;
    int iPanH = pnBase -> Height ;
    // Create buffer with memory DC and bitmap, then clear it with background.
    HDC          hDc ;
    HDC          hMemDc ;
    HBITMAP      hMemBm , hOldMemBm ;
    hDc          = GetDC(pnBase->Handle); //imDraw -> Canvas -> Handle ;  //GetDC(pnBase->Handle);
    hMemDc       = CreateCompatibleDC(hDc);
    hMemBm       = CreateCompatibleBitmap(hDc, iPanW , iPanH);///////////////////////////////////
    hOldMemBm    = SelectObject(hMemDc, hMemBm); //Old멤비트맵을 저장해 두는것은 현재 선택되어 있는 오프젝트는 삭제가 안된다. 그래서 삭제할때 월래 놈으로 돌려 놓고 삭제.


    UpdateGraphBk(hMemDc,Rect);
    UpdateGraphDt(hMemDc,Rect);


    BitBlt(hDc, 0 , 0 , iPanW, iPanH, hMemDc, 0, 0, SRCCOPY);

    // Delete object
    SelectObject(hMemDc,hOldMemBm);
    DeleteObject(hMemBm);
    DeleteDC(hMemDc);
    ReleaseDC(pnBase->Handle,hDc);

}

void __fastcall TFrmGraph::UpdateGraphBk(HDC _hDc ,TRect _tRect) //그래프 외형 그리기
{

    TCanvas *PanCanvas = new Graphics::TCanvas;
    PanCanvas->Handle = _hDc;

    PanCanvas -> Brush -> Color = clGradientInactiveCaption ;

    PanCanvas -> Rectangle(0,0,pnBase->Width,pnBase->Height);


    PanCanvas -> Font -> Height = 180 ;
    PanCanvas -> Font -> Color  = clBlack ;
    PanCanvas -> TextOut(110,30,m_iStgId + 1);



    // Y축 눈금을 그리고, 숫자를 표시합니다.
    int iGraphi ;
    String sCrntStep ;
    //귀찮아서 밑에가 0이고 위에가 MAX세팅일때만 고려함.
    for(double i = Para.m_dBottomY ; i <= Para.m_dTopY ; i += Para.m_dStepY) {

       iGraphi = GetYOnGraph(_tRect , i);

       PanCanvas->Pen->Color = clGray;
       PanCanvas->Pen->Style = psDot ;
       PanCanvas->Pen->Width = 1;
       PanCanvas -> MoveTo(_tRect.left, iGraphi);
       PanCanvas -> LineTo(_tRect.right, iGraphi);
       sCrntStep = (int)i;//sCrntStep.sprintf("%.1f",i) ;

       PanCanvas -> Font -> Height = 8 ;
       PanCanvas->TextOut(_tRect.left - 24 , iGraphi - 6 , sCrntStep);
       //PanCanvas->TextOut(_tRect.left - 29 , iGraphi - 6 , sCrntStep);
    }

    for(double i = Para.m_dBottomY ; i <= Para.m_dTopY ; i += Para.m_dBigStepY) {

       iGraphi = GetYOnGraph(_tRect , i);

       PanCanvas->Pen->Color = clBlack;
       PanCanvas->Pen->Style = psDot  ;
       PanCanvas->Pen->Width = 1;
       PanCanvas -> MoveTo(_tRect.left, iGraphi);
       PanCanvas -> LineTo(_tRect.right, iGraphi);
    }

    for(double i = Para.m_dLeftX ; i <= Para.m_dRightX ; i += Para.m_dStepX) {

       iGraphi = GetXOnGraph(_tRect , i);

       PanCanvas->Pen->Color = clGray;
       PanCanvas->Pen->Style = psDot ;
       PanCanvas->Pen->Width = 1;
       PanCanvas -> MoveTo(iGraphi ,  _tRect.bottom);
       PanCanvas -> LineTo(iGraphi , _tRect.top    );
       sCrntStep = (int)i;//sCrntStep.sprintf("%.1f",i) ;

       PanCanvas -> Font -> Height = 8 ;
       TextOut(PanCanvas->Handle , iGraphi - 10 , _tRect.Bottom + 6 , sCrntStep.c_str() , sCrntStep.Length() );
    }


    //0.0 선..
    PanCanvas->Pen->Color = clBlack;
    PanCanvas->Pen->Style = psSolid ;
    iGraphi = GetXOnGraph(_tRect , 0.0);
    PanCanvas -> MoveTo(iGraphi , _tRect.top   );
    PanCanvas -> LineTo(iGraphi , _tRect.bottom);

    iGraphi = GetYOnGraph(_tRect , 0.0);
    PanCanvas -> MoveTo(_tRect.left  , iGraphi);
    PanCanvas -> LineTo(_tRect.right , iGraphi);





    // 외곽선을 그립니다.
    PanCanvas -> Pen -> Width = 1 ; //조금 굵게.
    PanCanvas->Pen->Style = psSolid ;
    PanCanvas->Pen->Color = clBlack;

    PanCanvas -> MoveTo(_tRect.left , _tRect.top   ) ;
    PanCanvas -> LineTo(_tRect.left , _tRect.bottom) ;
    PanCanvas -> LineTo(_tRect.right, _tRect.bottom) ;
    PanCanvas -> LineTo(_tRect.right, _tRect.top   ) ;
    PanCanvas -> LineTo(_tRect.left , _tRect.top   ) ;

    delete PanCanvas;


}

void __fastcall TFrmGraph::UpdateGraphDt(HDC _hDc ,TRect _tRect) //그래프 데이터 그리기.
{
    TCanvas *PanCanvas = new Graphics::TCanvas;
    PanCanvas->Handle = _hDc;
    PanCanvas -> Brush -> Color = clGradientInactiveCaption ;

    int iGraphX ;
    int iGraphY ;
    bool bFirst = true ;

    int iIncDataCnt = g_HeightRslt[m_iStgId].iIncDataCnt ;


    //증가 그래프....
    for(int i = 0 ; i < iIncDataCnt ; i++) {
        iGraphX = GetXOnGraph(_tRect , g_HeightRslt[m_iStgId].tIncData[i].x);
        iGraphY = GetYOnGraph(_tRect , g_HeightRslt[m_iStgId].tIncData[i].y);

        if(bFirst){
            bFirst = false ;
            PanCanvas -> MoveTo(iGraphX , iGraphY) ;
        }

        PanCanvas->Pen->Style = psSolid ;
        PanCanvas->Pen->Color = clBlue  ;
        PanCanvas -> LineTo(iGraphX , iGraphY) ;
        PanCanvas -> Ellipse(iGraphX-1,iGraphY-1,iGraphX+1,iGraphY+1);
    }

    //감소 그래프...
    int iDecDataCnt = g_HeightRslt[m_iStgId].iDecDataCnt ;
    bFirst = true ;
    for(int i = 0 ; i < iDecDataCnt ; i++) {
        iGraphX = GetXOnGraph(_tRect , g_HeightRslt[m_iStgId].tDecData[i].x);
        iGraphY = GetYOnGraph(_tRect , g_HeightRslt[m_iStgId].tDecData[i].y);

        if(bFirst){
            bFirst = false ;
            PanCanvas -> MoveTo(iGraphX , iGraphY) ;
        }

        PanCanvas->Pen->Style = psSolid ;
        PanCanvas->Pen->Color = clRed   ;
        PanCanvas -> LineTo(iGraphX , iGraphY) ;
        //PanCanvas -> Ellipse(iGraphX-2,iGraphY-2,iGraphX+2,iGraphY+2);
        PanCanvas -> Ellipse(iGraphX-1,iGraphY-1,iGraphX+1,iGraphY+1);

    }


    delete PanCanvas;
}


void __fastcall TFrmGraph::FormDestroy(TObject *Sender)
{
    pnBase -> WindowProc = OriginalProc ;

/*
    m_iIncDataCnt = 0 ;
    if(m_pIncData){
        delete [] m_pIncData ;
        m_pIncData = NULL ;
    }
    m_iDecDataCnt = 0 ;
    if(m_pDecData){
        delete [] m_pDecData ;
        m_pDecData = NULL ;
    }*/
}
//---------------------------------------------------------------------------



void __fastcall TFrmGraph::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    if(m_iIncDataCnt != g_HeightRslt[m_iStgId].iIncDataCnt ||
       m_iDecDataCnt != g_HeightRslt[m_iStgId].iDecDataCnt ){
        UpdateView();
        m_iIncDataCnt = g_HeightRslt[m_iStgId].iIncDataCnt ;
        m_iDecDataCnt = g_HeightRslt[m_iStgId].iDecDataCnt ;
    }

    tmUpdate -> Enabled = true ;            
}
//---------------------------------------------------------------------------

void __fastcall TFrmGraph::FormShow(TObject *Sender)
{
    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmGraph::Button1Click(TObject *Sender)
{
    if(this -> Parent != m_pMaxParent) {
        m_pOriParent = this -> Parent ;
        this -> Parent = m_pMaxParent ;
        Para.m_dStepX /= 2 ;
        Para.m_dStepY /= 4 ;



    }
    else {
        this -> Parent = m_pOriParent ;
        Para.m_dStepX *= 2 ;
        Para.m_dStepY *= 4 ;

    }

    if(OM.DevOptn.bUseDuplex) {
        FrmGraph[m_iStgId] -> Para.m_dLeftX    = OM.DevOptn.iTestCrntMin ;
        //FrmGraph[m_iStgId] -> Para.m_dBottomY  = -300                    ;

    }
    else {
        FrmGraph[m_iStgId] -> Para.m_dLeftX    = 0                       ;
        //FrmGraph[m_iStgId] -> Para.m_dBottomY  = 0                       ;
    }
    FrmGraph[m_iStgId] -> Para.m_dBottomY  = OM.DevOptn.iGraphMinY   ;
    FrmGraph[m_iStgId] -> Para.m_dRightX   = OM.DevOptn.iTestCrntMax ;
    FrmGraph[m_iStgId] -> Para.m_dTopY     = OM.DevOptn.iGraphMaxY   ;
    FrmGraph[m_iStgId] -> Para.m_dStepX    = (int)(FrmGraph[m_iStgId] -> Para.m_dRightX - FrmGraph[m_iStgId] -> Para.m_dLeftX  ) / 8 ;
    FrmGraph[m_iStgId] -> Para.m_dStepY    = (int)(FrmGraph[m_iStgId] -> Para.m_dTopY   - FrmGraph[m_iStgId] -> Para.m_dBottomY) / 15;
    FrmGraph[m_iStgId] -> Para.m_dBigStepY = (int)(FrmGraph[m_iStgId] -> Para.m_dTopY   - FrmGraph[m_iStgId] -> Para.m_dBottomY) / 6 ;

    FrmGraph[m_iStgId] -> UpdateView();
}

//---------------------------------------------------------------------------

void TFrmGraph::SetMaxParent(TWinControl * _pMaxParent)
{
    m_pMaxParent = _pMaxParent ;

}
