 //---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormCadPosRtt.h"
#include "CadPosition.h"
#include "OptionMan.h"
#include "ManualMan.h"
#include "SMInterfaceUnit.h"
#include "LeftTool.h"
#include "RightTool.h"

#include "HexaPot.h"
//#include "FormJob.h"
//#include "UserFile.h"

//#include "SLogUnit.h"


//#define MAX_DSPCMD_POS 50
//#define MAX_HGHTCMD_POS 50
#define MAX_ROW 3
#define MAX_COL 2

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFrmCadPosRtt *FrmCadPosRtt;
//----------------------------------------------------------------- ----------
__fastcall TFrmCadPosRtt::TFrmCadPosRtt(TComponent* Owner)
    : TForm(Owner)
{
    Init();
}
//---------------------------------------------------------------------------
void TFrmCadPosRtt::Init()
{
    //this -> Width  = 1600 ;
    //this -> Height = 1000 ;



    OriginalProc = pnDraw->WindowProc ;
    pnDraw->WindowProc = PanelProc    ;

    pnDraw -> Align = alNone ;
    pnDraw -> Width  = 800   ;
    pnDraw -> Height = 800   ;

    //CAD.SetRowCol(1,2);


    //LineTracker = new CTracker  (); LineTracker -> Caption = "Line" ;
    //LineTracker -> TrackerType = CTracker::ttLine ;
    //LineTracker -> LineColor   = clLime ;
    //LineTracker -> PointColor  = clLime ;
    //LineTracker -> Visible     = false  ;
    //LineTracker-> SetOnPaint(OnPaint) ;

    m_dDispImgX   = 0 ;
    m_dDispImgY   = 0 ;

    m_dDispScaleX = 2.0 ;
    m_dDispScaleY = 2.0 ;


    //Pattern StringGrid .
    sgDspCmdPos -> ColCount         = 8   ;
    sgDspCmdPos -> DefaultRowHeight = 22  ;
    sgDspCmdPos -> DefaultColWidth  = 80  ;
    sgDspCmdPos -> ColWidths[0]     = 30 ;
    sgDspCmdPos -> ColWidths[1]     = 50 ;
    sgDspCmdPos -> ColWidths[2]     = 50 ;
    sgDspCmdPos -> ColWidths[3]     = 50 ;
    sgDspCmdPos -> ColWidths[4]     = 50 ;
    sgDspCmdPos -> ColWidths[5]     = 80 ;
    sgDspCmdPos -> ColWidths[6]     = 80 ;
    sgDspCmdPos -> ColWidths[7]     = 80 ;//sgDspCmdPos -> DefaultColWidth ;
    sgDspCmdPos -> FixedRows        = 1 ;
    sgDspCmdPos -> FixedCols        = 1 ;
    sgDspCmdPos -> Options          >> goRangeSelect ;
    sgDspCmdPos -> ScrollBars       = ssVertical     ;
    //sgPatternPos -> Align            = alClient       ;
    sgDspCmdPos -> Font -> Charset  = ARABIC_CHARSET ;
    sgDspCmdPos -> Font -> Size     = 11             ;
    sgDspCmdPos -> RowCount         = MAX_DSP_CMD + 1 ;
    sgDspCmdPos -> Visible          = true ;
    sgDspCmdPos -> Cells[1][0]      = "X Pos    " ;
    sgDspCmdPos -> Cells[2][0]      = "Y Pos    " ;
    sgDspCmdPos -> Cells[3][0]      = "Z Pos    " ;
    sgDspCmdPos -> Cells[4][0]      = "Delay    " ;
    sgDspCmdPos -> Cells[5][0]      = "Speed    " ;
    sgDspCmdPos -> Cells[6][0]      = "Dispensor" ;
    sgDspCmdPos -> Cells[7][0]      = "UV Speed " ;

    //Pattern StringGrid .
    sgHghtCmdPos -> ColCount         = 4   ;
    sgHghtCmdPos -> DefaultRowHeight = 22  ;
    sgHghtCmdPos -> DefaultColWidth  = 80  ;
    sgHghtCmdPos -> ColWidths[0]     = 80  ;//sgHghtCmdPos -> DefaultColWidth ;
    sgHghtCmdPos -> ColWidths[1]     = 80  ;//sgHghtCmdPos -> DefaultColWidth ;
    sgHghtCmdPos -> ColWidths[2]     = 80  ;//sgHghtCmdPos -> DefaultColWidth ;
    sgHghtCmdPos -> ColWidths[3]     = 80  ;//sgHghtCmdPos -> DefaultColWidth ;
    sgHghtCmdPos -> FixedRows        = 1 ;
    sgHghtCmdPos -> FixedCols        = 1 ;
    sgHghtCmdPos -> Options          >> goRangeSelect ;
    sgHghtCmdPos -> ScrollBars       = ssVertical     ;
    //sgPatternPos -> Align            = alClient       ;
    sgHghtCmdPos -> Font -> Charset  = ARABIC_CHARSET ;
    sgHghtCmdPos -> Font -> Size     = 11             ;
    sgHghtCmdPos -> RowCount         = MAX_DSP_CMD + 1 ;
    sgHghtCmdPos -> Visible          = true ;
    sgHghtCmdPos -> Cells[1][0]      = "X Pos    " ;
    sgHghtCmdPos -> Cells[2][0]      = "Y Pos    " ;
    sgHghtCmdPos -> Cells[3][0]      = "Z Pos    " ;

    for(int i = 1 ; i < MAX_DSP_CMD + 1 ; i++) {
        AnsiString sNum = i;
        sgDspCmdPos -> Cells[0][i] = sNum ;
    }

    for(int i = 1 ; i < MAX_HGHT_CMD + 1 ; i++) {
        AnsiString sNum = i;
        sgHghtCmdPos -> Cells[0][i] = sNum ;
    }

    //String sPath = EXE_FOLDER + "Util\\SAVE.INI" ;
    //CAD.LoadSave(true, sPath);
}

void TFrmCadPosRtt::Close()
{
    static bool _bFirst = true ;

    if(_bFirst) {
        _bFirst = false ;
    }

    pnDraw->WindowProc = OriginalProc ;
    //delete LineTracker     ;
}


//---------------------------------------------------------------------------
void __fastcall TFrmCadPosRtt::PanelProc(Messages::TMessage &msg)
{
    OriginalProc(msg);
    if (msg.Msg == WM_PAINT ) {
        OnPaint();
    }
}

void __fastcall TFrmCadPosRtt::tmUpdateTimer(TObject *Sender)
{

    //Label37 -> Caption = pnDraw -> Width ;
    //Label38 -> Caption = pnDraw -> Height ;

    double dX, dY;


    if(cbToolID -> ItemIndex == tlLVisn ||
       cbToolID -> ItemIndex == tlLHght ){
        dX = MT_GetCmdPos(miLTL_XGenRr) - CAD_Rtt.CT->GetCntrPosX() + g_tMidPosOfs.dX;
        dY = MT_GetCmdPos(miLTL_YVisn ) - CAD_Rtt.CT->GetCntrPosY() + g_tMidPosOfs.dY;
    }
    else if(cbToolID -> ItemIndex == tlLUV   ||
            cbToolID -> ItemIndex == tlLDspR ||
            cbToolID -> ItemIndex == tlLDspF ){
        dX = MT_GetCmdPos(miLTL_XGenRr) - CAD_Rtt.CT->GetCntrPosX()  + g_tMidPosOfs.dX;
        dY = CAD_Rtt.CT->GetCntrPosY()  - MT_GetCmdPos(miLTL_YDispr) - g_tMidPosOfs.dY;
    }
    else if(cbToolID -> ItemIndex == tlRTrns){
        dX = CAD_Rtt.CT->GetCntrPosX()  - MT_GetCmdPos(miRTL_XGenRr) - g_tMidPosOfs.dX;
        dY = MT_GetCmdPos(miRTL_YTrspr) - CAD_Rtt.CT->GetCntrPosY()  + g_tMidPosOfs.dY;
    }
    else if(cbToolID -> ItemIndex == tlRVisn ||
            cbToolID -> ItemIndex == tlRHght){
        dX = CAD_Rtt.CT->GetCntrPosX();
        dY = CAD_Rtt.CT->GetCntrPosY();

        dX = MT_GetCmdPos(miRTL_XGenRr);
        dY = MT_GetCmdPos(miRTL_YVisn );

        dX = CAD_Rtt.CT->GetCntrPosX() - MT_GetCmdPos(miRTL_XGenRr) - g_tMidPosOfs.dX;
        dY = CAD_Rtt.CT->GetCntrPosY() - MT_GetCmdPos(miRTL_YVisn ) - g_tMidPosOfs.dY;
    }
    else if(cbToolID -> ItemIndex == tlStage){
        dX = MT_GetCmdPos(miSTG_XStage) - CAD_Rtt.CT->GetCntrPosX() +g_tMidPosOfs.dX;
        dY = MT_GetCmdPos(miSTG_YStage) - CAD_Rtt.CT->GetCntrPosY() +g_tMidPosOfs.dY;
    }

    lbPos -> Caption = String("X:")+ dX + " Y:" + dY ;

}
//---------------------------------------------------------------------------


void __fastcall TFrmCadPosRtt::pnDrawMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
/*
    Trace("Lock", (m_pVision->GetName() + "pnDrawMouseDown Stt").c_str());
    if(m_pVision->GetCrntImg()==NULL)return ;
    if(Button == mbLeft) {

        double dx = m_dDispImgX + X / m_dDispScaleX ;
        double dy = m_dDispImgY + Y / m_dDispScaleY ;

        Trace("Lock", (m_pVision->GetName() + "pnDrawMouseDown 1").c_str());
        double ddd = m_pVision->GetCrntImg()->GetWidth ();

        double dda = m_pVision->GetCrntImg()->GetHeight();

        if(dx >= ddd) return ;
        if(dy >= dda) return ;

        Stat.iClickX = X ;
        Stat.iClickY = Y ;
        Trace("Lock", (m_pVision->GetName() + "pnDrawMouseDown 2").c_str());
        if(RectTracker  -> TrackerDw(Button, Shift , X,Y, m_dDispScaleX , m_dDispScaleY, m_dDispImgX , m_dDispImgY)) {Stat.bTrckrClick = true ; return ;}
        if(LineTracker  -> TrackerDw(Button, Shift , X,Y, m_dDispScaleX , m_dDispScaleY, m_dDispImgX , m_dDispImgY)) {Stat.bTrckrClick = true ; return ;}


        Stat.bImgClick = true ;

    }
*/
    if(Button == mbLeft) {
        if(Shift.Contains(ssCtrl)){
            int iOriClickX    = m_dDispImgX + X / m_dDispScaleX ;
            int iOriClickY    = m_dDispImgY + Y / m_dDispScaleY ;

            m_dDispScaleX = m_dDispScaleX * 10/8.0 ;
            m_dDispScaleY = m_dDispScaleY * 10/8.0 ;


            int iClickX    = m_dDispImgX + X / m_dDispScaleX ;
            int iClickY    = m_dDispImgY + Y / m_dDispScaleY ;

            m_dDispImgX += (iOriClickX-iClickX) ;
            m_dDispImgY += (iOriClickY-iClickY) ;

            OnPaint();
            return ;
        }
        if(Shift.Contains(ssShift)){
            int iOriClickX    = m_dDispImgX + X / m_dDispScaleX ;
            int iOriClickY    = m_dDispImgY + Y / m_dDispScaleY ;

            m_dDispScaleX = m_dDispScaleX * 8.0/10.0 ;
            m_dDispScaleY = m_dDispScaleY * 8.0/10.0 ;


            int iClickX    = m_dDispImgX + X / m_dDispScaleX ;
            int iClickY    = m_dDispImgY + Y / m_dDispScaleY ;

            m_dDispImgX += (iOriClickX-iClickX) ;
            m_dDispImgY += (iOriClickY-iClickY) ;

            OnPaint();
            return ;
        }



        double dx = m_dDispImgX + X / m_dDispScaleX ;
        double dy = m_dDispImgY + Y / m_dDispScaleY ;

        CCadObject * Obj = CAD_Rtt.OnMouseDown(dx,dy,m_dDispScaleX , m_dDispScaleY, m_dDispImgX , m_dDispImgY) ;
        if(Obj){
            cbObjectList -> Text = Obj -> GetName() ;
            if((Obj -> GetType()) != pcObjType -> ActivePageIndex){
                pcObjType -> ActivePageIndex = Obj -> GetType();
            }
            UpdateCadPosition(true);
        }
    }

    Stat.iClickX = X ;
    Stat.iClickY = Y ;
    Stat.bImgClick = true ;

    OnPaint();

}

void __fastcall TFrmCadPosRtt::pnDrawMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
//    bNeedRepaint |= LineTracker  -> TrackerMv(Shift , X,Y, m_dDispScaleX , m_dDispScaleY, m_dDispImgX , m_dDispImgY) ;
    double dx = m_dDispImgX + X / m_dDispScaleX ;            //-34.5
    double dy = m_dDispImgY + Y / m_dDispScaleY ;            //-125.13


    Label35 -> Caption = String(X) + "," + String(Y) ;
    Label36 -> Caption = String(dx) + "," + String(dy) ;



    CAD_Rtt.OnMouseMove(dx,dy,m_dDispScaleX , m_dDispScaleY, m_dDispImgX , m_dDispImgY) ;


    if(Stat.bImgClick){
        //존나 헛갈림....판넬 넓이 높이 고려하여 이미지 스타트 X,Y 리밋 확인.
        //왼쪽과 아래쪽 리밋.

        double dMaxX = CAD_Rtt.GetCoodWidth()  - pnDraw -> Width  / m_dDispScaleX ;
        double dMaxY = CAD_Rtt.GetCoodHeight() - pnDraw -> Height / m_dDispScaleY ;
        //판넬이 더큰경우 ex)640,480
        if(dMaxX <0) dMaxX = 0 ;
        if(dMaxY <0) dMaxY = 0 ;


        const double dMoveX = (Stat.iClickX-X) / m_dDispScaleX ;
        const double dMoveY = (Stat.iClickY-Y) / m_dDispScaleY ;


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
    }


    //if(Stat.bImgClick){
    //    //sbDraw -> HorzScrollBar -> Position += (m_iClickX-X);
    //    //sbDraw -> VertScrollBar -> Position += (m_iClickY-Y);
    //    Stat.iClickX = X ;
    //    Stat.iClickY = Y ;
    //}

    OnPaint();
                                                     
}

//---------------------------------------------------------------------------

void __fastcall TFrmCadPosRtt::pnDrawMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    //if(Button == mbLeft) {
        //LineTracker  -> TrackerUp(Button, Shift);
        Stat.bImgClick = false ;
    //}



}
//---------------------------------------------------------------------------





void __fastcall TFrmCadPosRtt::btZoomOutClick(TObject *Sender)
{
    //Trace("Operator", "Image Form_Zoom Out Button Click");
    m_dDispScaleX = m_dDispScaleX * 8/10.0 ;
    m_dDispScaleY = m_dDispScaleY * 8/10.0 ;

    int iOriWidth  = pnDraw -> Width  ;
    int iOriHeight = pnDraw -> Height ;


    int iGapWidth  = pnDraw -> Width  - iOriWidth  ;
    int iGapHeight = pnDraw -> Height - iOriHeight ;

    //sbDraw -> VertScrollBar -> Position += iGapWidth /2.0 ;
    //sbDraw -> HorzScrollBar -> Position += iGapHeight/2.0 ;


    OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmCadPosRtt::btZoomInClick(TObject *Sender)
{
    m_dDispScaleX = m_dDispScaleX * 10/8.0 ;
    m_dDispScaleY = m_dDispScaleY * 10/8.0 ;

    int iOriWidth  = pnDraw -> Width  ;
    int iOriHeight = pnDraw -> Height ;


    int iGapWidth  = pnDraw -> Width  - iOriWidth  ;
    int iGapHeight = pnDraw -> Height - iOriHeight ;

    //sbDraw -> VertScrollBar -> Position += iGapWidth /2.0 ;
    //sbDraw -> HorzScrollBar -> Position += iGapHeight/2.0 ;




    OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmCadPosRtt::btRealSizeClick(TObject *Sender)
{
    m_dDispScaleX = 2.0;
    m_dDispScaleY = 2.0;

    m_dDispImgX = 0.0 ;
    m_dDispImgY = 0.0 ;


    OnPaint();
}
//---------------------------------------------------------------------------

void TFrmCadPosRtt::OnPaint()
{

    int iPanW = pnDraw -> Width  ;
    int iPanH = pnDraw -> Height ;

    // Create buffer with memory DC and bitmap, then clear it with background.

    HDC     hDc ;
    HDC     hMemDc ;
    HBITMAP hMemBm , hOldMemBm ;

    RECT    Rect ;
    hDc       = GetDC(pnDraw->Handle); //imDraw -> Canvas -> Handle ;  //GetDC(pnDraw->Handle);
    GetClipBox(hDc , &Rect) ; //DC의 갱신해야할 Rect를 받아온다.
    hMemDc    = CreateCompatibleDC(hDc);
    hMemBm    = CreateCompatibleBitmap(hDc, iPanW , iPanH);///////////////////////////////////
    hOldMemBm = SelectObject(hMemDc, hMemBm); //Old멤비트맵을 저장해 두는것은 현재 선택되어 있는 오프젝트는 삭제가 안된다. 그래서 삭제할때 월래 놈으로 돌려 놓고 삭제.

    //===========================================
    CAD_Rtt.Paint(hMemDc ,m_dDispScaleX , m_dDispScaleY, m_dDispImgX , m_dDispImgY);


    //if(btCenter -> Down) {
    //    CGdi * Gdi = new CGdi(hMemDc,pImg->m_dDispScaleX,pImg->m_dDispScaleY,pImg->GetOffsetX(),pImg->GetOffsetY());
    //    Gdi -> m_tPen.Style = psDot  ;
    //    Gdi -> m_tPen.Color = clBlue ;
    //    Gdi -> LinePlus(iImgW /2.0 , iImgH /2.0 , 100);
    //    delete Gdi ;
    //}

    //LineTracker -> Paint(hMemDc, m_dDispScaleX , m_dDispScaleY , m_dDispImgX , m_dDispImgY);

    int iRet ;
    iRet = BitBlt(hDc, 0 , 0 , iPanW, iPanH, hMemDc, 0, 0, SRCCOPY);


    // Delete object
    SelectObject(hMemDc,hOldMemBm);
    DeleteObject(hMemBm);
    DeleteDC(hMemDc);
    ReleaseDC(pnDraw->Handle,hDc);
}
//---------------------------------------------------------------------------
void __fastcall TFrmCadPosRtt::FormShow(TObject *Sender)
{
    tmUpdate -> Enabled = true ;


    TStringList * ObjectList = new TStringList();

    ObjectList -> CommaText = CAD_Rtt.GetNameList() ;
    //ObjectList -> Sorted = true;


    cbObjectList -> Items = ObjectList ;
    cbObjectList -> ItemIndex = 0 ;

    cbObjectListChange(Sender);

    delete ObjectList ;
}
//---------------------------------------------------------------------------



void __fastcall TFrmCadPosRtt::btCenterClick(TObject *Sender)
{
    OnPaint();
}



//---------------------------------------------------------------------------


void __fastcall TFrmCadPosRtt::cbObjectListChange(TObject *Sender)
{
    CCadObject * Obj = CAD_Rtt.FindObjectByName(cbObjectList -> Text) ;
    CAD_Rtt.SetSelectedObject(Obj);

    /*

    ctObject = 0 ,
    ctPoint      ,
    ctCntPoint   ,
    ctDspCmd     ,
    ctLine       ,
    ctHeight     ,
    ctRect       ,
    ctHghtCmd    ,
    ctRhombus    ,
    */

    if((Obj -> GetType()) != pcObjType -> ActivePageIndex){
        pcObjType -> ActivePageIndex = Obj -> GetType();
    }

    UpdateCadPosition(true);

    OnPaint();
}
//---------------------------------------------------------------------------


void __fastcall TFrmCadPosRtt::btSaveClick(TObject *Sender)
{
    //CCadObject * Obj = CAD_Rtt.FindObjectByName(cbObjectList -> Text) ;
    //
    //CAD_Rtt.SetSelectedObject(Obj);
    //
    //if((Obj -> GetType() - 1) != pcObjType -> ActivePageIndex){
    //    pcObjType -> ActivePageIndex = Obj -> GetType() - 1;
    //
    //}

    UpdateCadPosition(false);

    CCadObject * Obj = CAD_Rtt.FindObjectByName(cbObjectList -> Text) ;

    String sPath = EXE_FOLDER + "JobFile\\" + OM.GetCrntDev() + "\\CadPos.RPP";

    Obj -> LoadSave(false ,sPath);

    OnPaint();
}
//---------------------------------------------------------------------------


void TFrmCadPosRtt::UpdateCadPosition(bool bToTable)
{
    CCadObject * Obj = CAD_Rtt.FindObjectByName(cbObjectList -> Text) ;

    //CAD_Rtt.SetGainX(1);
    //CAD_Rtt.SetGainY(1);
    //CAD_Rtt.SetGainZ(1);
    //
    //CAD_Rtt.SetOffsetX(0.0);
    //CAD_Rtt.SetOffsetY(0.0);
    //CAD_Rtt.SetOffsetZ(0.0);

    CCadObject    *ObjObject     ;
    CCadPoint     *ObjPoint      ;
    CCadCntPoint  *ObjCntPoint   ;
    CCadVisnPoint *ObjVisnPoint  ;
    CCadDspCmd    *ObjDspCmd     ;
    CCadLine      *ObjLine       ;
    CCadHeight    *ObjHeight     ;
    CCadRect      *ObjRect       ;
    CCadHghtCmd   *ObjHghtCmd    ;
    CCadRhombus   *ObjRhombus    ;

    int iTempNodeCnt ;

    if(!bToTable){
        Obj -> SetColor(cdColor -> Color);
        Obj -> SetVisible(cbVisible -> Checked);

        //pnColor -> Color   = Obj -> GetColor() ;
        //pnColor -> Caption = Obj -> GetColor() ;

        /*
        ctObject = 
        ctPoint    
        ctCntPoint 
        ctVisnPoint
        ctDspCmd   
        ctLine
        ctHeight   
        ctRect     
        ctHghtCmd  
        ctRhombus  
        */

        switch(Obj -> GetType()){
            default          : break ;

            case ctObject    : break ;

            case ctPoint     : ObjPoint = (CCadPoint *) Obj ;
                               ObjPoint -> SetCntrPosX(StrToFloatDef(edPntCntrPosX -> Text , 1.0));
                               ObjPoint -> SetCntrPosY(StrToFloatDef(edPntCntrPosY -> Text , 1.0));
                               break ;

            case ctCntPoint  : ObjCntPoint = (CCadCntPoint *) Obj ;
                               ObjCntPoint -> SetCntrPosX(StrToFloatDef(edCntPntCntrPosX -> Text , 1.0));
                               ObjCntPoint -> SetCntrPosY(StrToFloatDef(edCntPntCntrPosY -> Text , 1.0));
                               break ;

            case ctVisnPoint : ObjVisnPoint = (CCadVisnPoint *) Obj ;
                               ObjVisnPoint -> SetCntrPosX(StrToFloatDef(edVisnPntCntrPosX -> Text , 1.0));
                               ObjVisnPoint -> SetCntrPosY(StrToFloatDef(edVisnPntCntrPosY -> Text , 1.0));
                               break ;

            case ctDspCmd    : ObjDspCmd = (CCadDspCmd *) Obj ;

                               iTempNodeCnt = StrToIntDef(edDspCmdNodeCnt -> Text , 10);
                               if(iTempNodeCnt < 1          ) iTempNodeCnt = 1  ;
                               if(iTempNodeCnt > MAX_DSP_CMD) iTempNodeCnt = MAX_DSP_CMD;

                               ObjDspCmd -> SetNodeCnt(iTempNodeCnt);
                               for(int r = 0; r < ObjDspCmd -> GetNodeCnt() ; r++){
                                   ObjDspCmd -> SetPosX   (r , StrToFloatDef(sgDspCmdPos -> Cells[1][r+1], 1));
                                   ObjDspCmd -> SetPosY   (r , StrToFloatDef(sgDspCmdPos -> Cells[2][r+1], 1));
                                   ObjDspCmd -> SetPosZ   (r , StrToFloatDef(sgDspCmdPos -> Cells[3][r+1], 1));
                                   ObjDspCmd -> SetDelay  (r , StrToFloatDef(sgDspCmdPos -> Cells[4][r+1], 1));
                                   ObjDspCmd -> SetSpeed  (r , StrToFloatDef(sgDspCmdPos -> Cells[5][r+1], 1));
                                   ObjDspCmd -> SetDispOn (r , sgDspCmdPos -> Cells[6][r+1] != "0");
                                   ObjDspCmd -> SetUVSpeed(r , StrToFloatDef(sgDspCmdPos -> Cells[7][r+1], 1));
                               }
                               sgDspCmdPos -> Refresh();
                               break;

            case ctLine      : ObjLine =  (CCadLine *) Obj ;
                               ObjLine -> SetSttPosX(StrToFloatDef(edLineSttPosX -> Text , 1.0)) ;
                               ObjLine -> SetSttPosY(StrToFloatDef(edLineSttPosY -> Text , 1.0)) ;
                               ObjLine -> SetEndPosX(StrToFloatDef(edLineEndPosX -> Text , 1.0)) ;
                               ObjLine -> SetEndPosY(StrToFloatDef(edLineEndPosY -> Text , 1.0)) ;
                               break;

            case ctHeight    : ObjHeight = (CCadHeight *) Obj;
                               ObjHeight -> SetSttPosX(StrToFloatDef(edHghtSttPosX -> Text , 1.0)) ;
                               ObjHeight -> SetSttPosY(StrToFloatDef(edHghtSttPosY -> Text , 1.0)) ;
                               ObjHeight -> SetSubPosX(StrToFloatDef(edHghtSubPosX -> Text , 1.0)) ;
                               ObjHeight -> SetSubPosY(StrToFloatDef(edHghtSubPosY -> Text , 1.0)) ;
                               ObjHeight -> SetEndPosX(StrToFloatDef(edHghtEndPosX -> Text , 1.0)) ;
                               ObjHeight -> SetEndPosY(StrToFloatDef(edHghtEndPosY -> Text , 1.0)) ;
                               break;

            case ctRect      : ObjRect = (CCadRect *) Obj;
                               ObjRect -> SetLeft  (StrToFloatDef(edRectLeft   -> Text , 1.0)) ;
                               ObjRect -> SetTop   (StrToFloatDef(edRectTop    -> Text , 1.0)) ;
                               ObjRect -> SetWidth (StrToFloatDef(edRectWidth  -> Text , 1.0)) ;
                               ObjRect -> SetHeight(StrToFloatDef(edRectHeight -> Text , 1.0)) ;
                               break;

            case ctHghtCmd   : ObjHghtCmd = (CCadHghtCmd *) Obj ;

                               iTempNodeCnt = StrToIntDef(edHghtCmdNodeCnt -> Text , 10);
                               if(iTempNodeCnt < 1           ) iTempNodeCnt = 1  ;
                               if(iTempNodeCnt > MAX_HGHT_CMD) iTempNodeCnt = MAX_HGHT_CMD;

                               ObjHghtCmd -> SetNodeCnt(iTempNodeCnt) ;
                               for(int r = 0; r < ObjHghtCmd -> GetNodeCnt() ; r++){
                                   ObjHghtCmd -> SetPosX (r , StrToFloatDef(sgHghtCmdPos -> Cells[1][r+1], 1));
                                   ObjHghtCmd -> SetPosY (r , StrToFloatDef(sgHghtCmdPos -> Cells[2][r+1], 1));
                                   ObjHghtCmd -> SetPosZ (r , StrToFloatDef(sgHghtCmdPos -> Cells[3][r+1], 1));
                               }
                               sgHghtCmdPos -> Refresh();
                               break;
            case ctRhombus   : ObjRhombus = (CCadRhombus *) Obj;
                               ObjRhombus -> SetLeft  (StrToFloatDef(edRhombusLeft   -> Text , 1.0)) ;
                               ObjRhombus -> SetTop   (StrToFloatDef(edRhombusTop    -> Text , 1.0)) ;
                               ObjRhombus -> SetWidth (StrToFloatDef(edRhombusWidth  -> Text , 1.0)) ;
                               ObjRhombus -> SetHeight(StrToFloatDef(edRhombusHeight -> Text , 1.0)) ;
                               break;

         }
         UpdateCadPosition(true);

    }
    else{

        cbVisible -> Checked = Obj -> GetVisible();
        cdColor   -> Color   = Obj -> GetColor();

        pnColor -> Color   = Obj -> GetColor() ;
        pnColor -> Caption = Obj -> GetColor() ;


        switch(Obj -> GetType()){
            default          : break ;
            case ctObject    : break ;

            case ctPoint     : ObjPoint = (CCadPoint *) Obj ;
                               edPntCntrPosX -> Text = Obj -> GetCntrPosX(true) ;
                               edPntCntrPosY -> Text = Obj -> GetCntrPosY(true) ;
                               break ;

            case ctCntPoint  : ObjCntPoint = (CCadCntPoint *) Obj ;
                               edCntPntCntrPosX -> Text = ObjCntPoint -> GetCntrPosX(true);
                               edCntPntCntrPosY -> Text = ObjCntPoint -> GetCntrPosY(true);
                               break ;

            case ctVisnPoint : ObjVisnPoint = (CCadVisnPoint *) Obj ;
                               edVisnPntCntrPosX -> Text = ObjVisnPoint -> GetCntrPosX(true);
                               edVisnPntCntrPosY -> Text = ObjVisnPoint -> GetCntrPosY(true);
                               break ;

            case ctDspCmd    : ObjDspCmd = (CCadDspCmd *) Obj ;
                               if(ObjDspCmd -> GetNodeCnt() < 2) {
                                   sgDspCmdPos -> RowCount = 2;
                               }
                               else {
                                   sgDspCmdPos -> RowCount = ObjDspCmd -> GetNodeCnt() + 1;
                               }

                               edDspCmdNodeCnt -> Text = ObjDspCmd -> GetNodeCnt();
                               for(int r = 0; r < ObjDspCmd -> GetNodeCnt() ; r++){
                                   sgDspCmdPos -> Cells[1][r+1] = ObjDspCmd -> GetPosX  (r,true);
                                   sgDspCmdPos -> Cells[2][r+1] = ObjDspCmd -> GetPosY  (r,true);
                                   sgDspCmdPos -> Cells[3][r+1] = ObjDspCmd -> GetPosZ  (r,true);
                                   sgDspCmdPos -> Cells[4][r+1] = ObjDspCmd -> GetDelay (r);
                                   sgDspCmdPos -> Cells[5][r+1] = ObjDspCmd -> GetSpeed (r);
                                   sgDspCmdPos -> Cells[6][r+1] = ObjDspCmd -> GetDispOn(r)?"1":"0";
                                   sgDspCmdPos -> Cells[7][r+1] = ObjDspCmd -> GetUVSpeed(r);
                               }
                               break;

            case ctLine      : ObjLine =  (CCadLine *) Obj ;
                               edLineSttPosX -> Text = ObjLine -> GetSttPosX(true);
                               edLineSttPosY -> Text = ObjLine -> GetSttPosY(true);
                               edLineEndPosX -> Text = ObjLine -> GetEndPosX(true);
                               edLineEndPosY -> Text = ObjLine -> GetEndPosY(true);
                               break;

            case ctHeight    : ObjHeight = (CCadHeight *) Obj;
                               edHghtSttPosX -> Text = ObjHeight -> GetSttPosX(true);
                               edHghtSttPosY -> Text = ObjHeight -> GetSttPosY(true);
                               edHghtSubPosX -> Text = ObjHeight -> GetSubPosX(true);
                               edHghtSubPosY -> Text = ObjHeight -> GetSubPosY(true);
                               edHghtEndPosX -> Text = ObjHeight -> GetEndPosX(true);
                               edHghtEndPosY -> Text = ObjHeight -> GetEndPosY(true);
                               break;

            case ctRect      : ObjRect = (CCadRect *) Obj;
                               edRectLeft   -> Text = ObjRect -> GetLeft  (true);
                               edRectTop    -> Text = ObjRect -> GetTop   (true);
                               edRectWidth  -> Text = ObjRect -> GetWidth ();
                               edRectHeight -> Text = ObjRect -> GetHeight();
                               edRectRight  -> Text = ObjRect -> GetRight ();
                               edRectBottom -> Text = ObjRect -> GetBottom();
                               break;

            case ctHghtCmd   : ObjHghtCmd = (CCadHghtCmd *) Obj;
                               //edHghtCmdLeft    -> Text = ObjHghtCmd -> GetLeft   ();
                               //edHghtCmdTop     -> Text = ObjHghtCmd -> GetTop    ();
                               //edHghtCmdWidth   -> Text = ObjHghtCmd -> GetWidth  ();
                               //edHghtCmdHeight  -> Text = ObjHghtCmd -> GetHeight ();
                               //edHghtCmdRight   -> Text = ObjHghtCmd -> GetRight  ();
                               //edHghtCmdBottom  -> Text = ObjHghtCmd -> GetBottom ();
                               edHghtCmdNodeCnt -> Text = ObjHghtCmd -> GetNodeCnt();

                               sgHghtCmdPos -> RowCount = ObjHghtCmd -> GetNodeCnt() + 1;;
                               for(int r = 0; r < ObjHghtCmd -> GetNodeCnt() ; r++){
                                   sgHghtCmdPos -> Cells[1][r+1] = ObjHghtCmd -> GetPosX  (r,true);
                                   sgHghtCmdPos -> Cells[2][r+1] = ObjHghtCmd -> GetPosY  (r,true);
                                   sgHghtCmdPos -> Cells[3][r+1] = ObjHghtCmd -> GetPosZ  (r,true);
                               }

            case ctRhombus   : ObjRhombus = (CCadRhombus *) Obj;
                               edRhombusLeft   -> Text = ObjRhombus -> GetLeft  (true) ;
                               edRhombusTop    -> Text = ObjRhombus -> GetTop   (true) ;
                               edRhombusWidth  -> Text = ObjRhombus -> GetWidth () ;
                               edRhombusHeight -> Text = ObjRhombus -> GetHeight() ;
                               //dCntrPosX = edLineCntrPosX -> Text.ToDouble();
                               //dCntrPosY = edLineCntrPosX -> Text.ToDouble();
                               //if(dCntrPosX != ObjRhombus -> GetCntrPosX()) ObjRhombus -> SetCntrPosX(StrToFloatDef(edRhombusCntrPosX -> Text, 1.0));
                               //if(dCntrPosY != ObjRhombus -> GetCntrPosY()) ObjRhombus -> SetCntrPosY(StrToFloatDef(edRhombusCntrPosY -> Text, 1.0));

                               break;




         }
         //나중에 저장 로드 부분 수정하기!
         //String sPath = EXE_FOLDER + "Util\\SAVE.INI" ;
         //Obj -> LoadSaveAll(true, sPath);
    }

}
void __fastcall TFrmCadPosRtt::sgDspCmdPosDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
    AnsiString sItem ;
    if(ARow > 0){
        if(ACol == 6 ) { //OnOff
            sItem = sgDspCmdPos -> Cells[ACol][ARow];
            sgDspCmdPos -> Canvas -> Brush -> Color = sItem == "0" ? clGray : clLime ;
            sgDspCmdPos -> Canvas -> FillRect(Rect) ;
            sgDspCmdPos -> Canvas -> TextOut(Rect.Left + 3, Rect.Top + (Rect.Height() - sgDspCmdPos -> Canvas -> TextHeight(sItem)) / 2, sItem) ;
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmCadPosRtt::sgDspCmdPosSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
    sgDspCmdPos -> Options << goEditing ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmCadPosRtt::pcObjTypeChange(TObject *Sender)
{
    CCadObject * Obj = CAD_Rtt.FindObjectByName(cbObjectList -> Text) ;

    CAD_Rtt.SetSelectedObject(Obj);

    if((Obj -> GetType()) != pcObjType -> ActivePageIndex){
        pcObjType -> ActivePageIndex = Obj -> GetType();
    }
}
//---------------------------------------------------------------------------
void TFrmCadPosRtt::VisibleCheck()
{

}


void __fastcall TFrmCadPosRtt::sgHghtCmdPosSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
    sgHghtCmdPos -> Options << goEditing ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmCadPosRtt::btColorClick(TObject *Sender)
{
    cdColor -> Execute();


    CCadObject * Obj = CAD_Rtt.FindObjectByName(cbObjectList -> Text) ;
    Obj -> SetColor(cdColor -> Color);

    OnPaint();

}
//---------------------------------------------------------------------------



void __fastcall TFrmCadPosRtt::cbVisibleClick(TObject *Sender)
{
    CCadObject * Obj = CAD_Rtt.FindObjectByName(cbObjectList -> Text) ;
    Obj -> SetVisible(cbVisible -> Checked);
    OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmCadPosRtt::btMOVEClick(TObject *Sender)
{
    //EN_TOOL_ID _tiTool, double _dX, double _dY
    if(!CAD_Rtt.IsPntSelected()) {
        FM_MsgOk("ERROR","선택된 점이 없습니다.");
        return ;
    }

    double dX, dY;

    if(cbToolID -> ItemIndex == tlLVisn ||
       cbToolID -> ItemIndex == tlLHght ){
       LTL.SetCadOfsGain(cbToolID -> ItemIndex);
       dX =   g_tMidPosOfs.dX;
       dY =   g_tMidPosOfs.dY;
    }
    else if(cbToolID -> ItemIndex == tlLUV   ||
            cbToolID -> ItemIndex == tlLDspR ||
            cbToolID -> ItemIndex == tlLDspF ){
        LTL.SetCadOfsGain(cbToolID -> ItemIndex);
        dX =  g_tMidPosOfs.dX;
        dY = -g_tMidPosOfs.dY;
    }
    else if(cbToolID -> ItemIndex == tlRTrns){
        RTL.SetCadOfsGain(cbToolID -> ItemIndex);
        dX = -g_tMidPosOfs.dX;
        dY =  g_tMidPosOfs.dY;
    }
    else if(cbToolID -> ItemIndex == tlRVisn ||
            cbToolID -> ItemIndex == tlRHght ||
            cbToolID -> ItemIndex == tlRConfocal ){
        RTL.SetCadOfsGain(cbToolID -> ItemIndex);
        dX = -g_tMidPosOfs.dX;
        dY = -g_tMidPosOfs.dY;
    }
    else if(cbToolID -> ItemIndex == tlStage){
        RTL.SetCadOfsGain(cbToolID -> ItemIndex);
        dX =  g_tMidPosOfs.dX;
        dY =  g_tMidPosOfs.dY;
    }


    MM.SetCadMove(cbToolID -> ItemIndex, CAD_Rtt.GetSelectedPndX()+dX,CAD_Rtt.GetSelectedPndY()+dY);
    //MM.SetManCycle(mcCycleCadPos);

    //Stage 선택 후 이동 하는 메뉴얼 만들기.


}
//---------------------------------------------------------------------------

void __fastcall TFrmCadPosRtt::Button1Click(TObject *Sender)
{
    double dTempX = StrToFloatDef(edShiftX -> Text , 0.0);
    double dTempY = StrToFloatDef(edShiftY -> Text , 0.0);
    CAD_Rtt.ShiftAll(dTempX , dTempY);
    edShiftX -> Text = "0.0";
    edShiftY -> Text = "0.0";

    UpdateCadPosition(true);

    OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmCadPosRtt::btOpenAsClick(TObject *Sender)
{
    dgLoadFilePath -> Execute() ;

    CAD_Rtt.LoadSaveFile(true , dgLoadFilePath -> FileName);

    UpdateCadPosition(true);
    OnPaint();

}
//---------------------------------------------------------------------------

void __fastcall TFrmCadPosRtt::btSaveAsClick(TObject *Sender)
{
    dgSaveFilePath -> Execute() ;
    CAD_Rtt.LoadSaveFile(false , dgSaveFilePath -> FileName);

    

}
//---------------------------------------------------------------------------

void __fastcall TFrmCadPosRtt::btSaveAllClick(TObject *Sender)
{

    //UpdateCadPosition(false);

    //CCadObject * Obj; = CAD_Rtt.FindObjectByName(cbObjectList -> Text) ;

    String sPath = EXE_FOLDER + "JobFile\\" + OM.GetCrntDev() + "\\CadPos.RPP";

    CAD_Rtt.LoadSaveFile(false, sPath);
    //Obj -> LoadSave(false ,sPath);

    OnPaint();
}
//---------------------------------------------------------------------------



void __fastcall TFrmCadPosRtt::cbToolIDChange(TObject *Sender)
{
    double dX, dY;


    if(cbToolID -> ItemIndex == tlLVisn ||
       cbToolID -> ItemIndex == tlLHght ){
       LTL.SetCadOfsGain(cbToolID -> ItemIndex);
    }
    else if(cbToolID -> ItemIndex == tlLUV   ||
            cbToolID -> ItemIndex == tlLDspR ||
            cbToolID -> ItemIndex == tlLDspF ){
        LTL.SetCadOfsGain(cbToolID -> ItemIndex);
    }
    else if(cbToolID -> ItemIndex == tlRTrns){
        RTL.SetCadOfsGain(cbToolID -> ItemIndex);
    }
    else if(cbToolID -> ItemIndex == tlRVisn ||
            cbToolID -> ItemIndex == tlRHght){
        RTL.SetCadOfsGain(cbToolID -> ItemIndex);
    }
    else if(cbToolID -> ItemIndex == tlStage){
        RTL.SetCadOfsGain(cbToolID -> ItemIndex);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmCadPosRtt::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmCadPosRtt::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate -> Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmCadPosRtt::Button2Click(TObject *Sender)
{
    CCadDspCmd *ObjDspCmd = (CCadDspCmd *)CAD_Rtt.FindObjectByName(cbObjectList -> Text) ;
    ObjDspCmd -> SetNodeCnt(edDspCmdNodeCnt -> Text.ToIntDef(10)+1);
    UpdateCadPosition(true);

    for(int r = ObjDspCmd -> GetNodeCnt() - 1 ; r >= sgDspCmdPos -> Row ; r--){
        for(int c = 1 ; c < sgDspCmdPos -> ColCount ; c++) {
            sgDspCmdPos -> Cells[c][r+1] = sgDspCmdPos -> Cells[c][r] ;
        }
    }

    UpdateCadPosition(false);
}
//---------------------------------------------------------------------------

void __fastcall TFrmCadPosRtt::Button3Click(TObject *Sender)
{
    CCadDspCmd *ObjDspCmd = (CCadDspCmd *)CAD_Rtt.FindObjectByName(cbObjectList -> Text) ;
    for(int r = sgDspCmdPos -> Row ; r <= ObjDspCmd -> GetNodeCnt() - 1 ; r++){
        for(int c = 1 ; c < sgDspCmdPos -> ColCount ; c++) {
            sgDspCmdPos -> Cells[c][r] = sgDspCmdPos -> Cells[c][r+1] ;
        }
    }
    UpdateCadPosition(false);

    ObjDspCmd -> SetNodeCnt(edDspCmdNodeCnt -> Text.ToIntDef(10)-1);
    UpdateCadPosition(true);
    

}

//---------------------------------------------------------------------------
                                               //sgDspCmdPos -> Row
