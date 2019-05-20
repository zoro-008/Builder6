
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PstnMan.h"
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"

#include "Sequence.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Head.h"
#include "Stage.h"
//---------------------------------------------------------------------------
#include "OptionMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//#define NAME_COL_WIDTH   30
//#define POS_COL_WIDTH    30
//#define GO_COL_WIDTH     15
//#define INPUT_COL_WIDTH  15
#define NAME_COL_WIDTH   120
#define POS_COL_WIDTH    102
#define GO_COL_WIDTH     50
#define INPUT_COL_WIDTH  54

CPstnMan PM;
//---------------------------------------------------------------------------
CPstnMan::CPstnMan()
{
    if(MAX_MOTR != MT_GetMotorCnt()) {ShowMessageT("Motor Count is wrong");}

    memset(&GridInfo , 0 , sizeof(SGridSelInfo)             );
    memset( Value    , 0 , sizeof(SValue)*MAX_MOTR*MAX_PSTN );

    Value[miSTG_ZCst][pvSTG_ZCstWait       ].sName = "대기 위치    " ;
    Value[miSTG_ZCst][pvSTG_ZCstPickStt    ].sName = "공급 위치    " ;
    Value[miSTG_ZCst][poSTG_ZCstPlaceOfs   ].sName = "배출 오프셑  " ;Value[miSTG_ZCst][poSTG_ZCstPlaceOfs  ].iType = vtOffset ;

    Value[miSTG_XTrs][pvSTG_XTrsWait       ].sName = "대기 위치    " ;
    Value[miSTG_XTrs][pvSTG_XTrsPick       ].sName = "공급 및 배출 " ;
    Value[miSTG_XTrs][pvSTG_XTrsPickWait   ].sName = "공급대기 위치" ;
    Value[miSTG_XTrs][pvSTG_XTrsBarcode    ].sName = "바코드 위치  " ;
    Value[miSTG_XTrs][pvSTG_XTrsPlace      ].sName = "스테이지 공급" ;

    Value[miHED_XHed][pvHED_XHedWait       ].sName = "대기 위치    " ;
    Value[miHED_XHed][pvHED_XHedVTAlgn     ].sName = "T얼라인 위치 " ;Value[miHED_XHed][pvHED_XHedVTAlgn    ].iType = vtFixValue ;
    Value[miHED_XHed][pvHED_XHedVOri       ].sName = "기준칩 위치  " ;Value[miHED_XHed][pvHED_XHedVOri      ].iType = vtFixValue ;
    Value[miHED_XHed][pvHED_XHedVCalPad    ].sName = "캘리브래이션 " ;Value[miHED_XHed][pvHED_XHedVCalPad   ].iType = vtFixValue ;
    Value[miHED_XHed][pvHED_XHedVBoardStt  ].sName = "화이트 보드  " ;Value[miHED_XHed][pvHED_XHedVBoardStt ].iType = vtFixValue ;
    Value[miHED_XHed][pvHED_XHedPCover     ].sName = "펜커버 위치  " ;
    Value[miHED_XHed][pvHED_XHedHWCntrPos  ].sName = "하드웨어 센터" ;
    Value[miHED_XHed][poHED_XHedVisnPenOfs ].sName = "펜 비전 옵셑 " ;Value[miHED_XHed][poHED_XHedVisnPenOfs].iType = vtOffset ;
    Value[miHED_XHed][poHED_XHedChipPenOfs ].sName = "펜 마킹 옵셑 " ;Value[miHED_XHed][poHED_XHedChipPenOfs].iType = vtOffset ;

    Value[miHED_YHed][pvHED_YHedWait       ].sName = "대기 위치    " ;
    Value[miHED_YHed][pvHED_YHedVTAlgn     ].sName = "T얼라인 위치 " ;Value[miHED_YHed][pvHED_YHedVTAlgn    ].iType = vtFixValue ;
    Value[miHED_YHed][pvHED_YHedVOri       ].sName = "기준칩 위치  " ;Value[miHED_YHed][pvHED_YHedVOri      ].iType = vtFixValue ;
    Value[miHED_YHed][pvHED_YHedVCalPad    ].sName = "캘리브래이션 " ;Value[miHED_YHed][pvHED_YHedVCalPad   ].iType = vtFixValue ;
    Value[miHED_YHed][pvHED_YHedVBoardStt  ].sName = "화이트 보드  " ;Value[miHED_YHed][pvHED_YHedVBoardStt ].iType = vtFixValue ;
    Value[miHED_YHed][pvHED_YHedPCover     ].sName = "펜커버 위치  " ;
    Value[miHED_YHed][pvHED_YHedHWCntrPos  ].sName = "하드웨어 센터" ;
    Value[miHED_YHed][poHED_YHedVisnPenOfs ].sName = "펜 비전 옵셑 " ;Value[miHED_YHed][poHED_YHedVisnPenOfs].iType = vtOffset ;
    Value[miHED_YHed][poHED_YHedChipPenOfs ].sName = "펜 마킹 옵셑 " ;Value[miHED_YHed][poHED_YHedChipPenOfs].iType = vtOffset ;

    Value[miSTG_TStg][pvSTG_TStgWait       ].sName = "공급 위치    " ;
    Value[miSTG_TStg][pvSTG_TStgTAlgn      ].sName = "T얼라인 위치 " ;Value[miSTG_TStg][pvSTG_TStgTAlgn     ].iType = vtFixValue ;

    Value[miHED_ZVsn][pvHED_ZVsnWait       ].sName = "대기 위치    " ;Value[miHED_ZVsn][pvHED_ZVsnWait      ].iType = vtFixValue ;
    Value[miHED_ZVsn][pvHED_ZVsnWork       ].sName = "비전 Z축 워킹" ;Value[miHED_ZVsn][pvHED_ZVsnWork      ].iType = vtFixValue ;

    Value[miHED_TZom][pvHED_TZomWait       ].sName = "대기 위치    " ;Value[miHED_TZom][pvHED_TZomWait      ].iType = vtFixValue ;
    Value[miHED_TZom][pvHED_TZomWork       ].sName = "비전 줌 워킹 " ;Value[miHED_TZom][pvHED_TZomWork      ].iType = vtFixValue ;

    LoadLastInfo();

    Load(m_sCrntDev);

}

CPstnMan::~CPstnMan()
{
//    SaveLastInfo();
/*  parent 할당시에 자동 삭제 됌.
    for( int i=0;i<MAX_MOTR;i++){
        if(m_pPstnList[i]) {
            delete m_pPstnList[i];
            m_pPstnList[i] = NULL ;
        }
        if(m_pPstnEdit[i]) {
            delete m_pPstnEdit[i] ;
            m_pPstnEdit[i] = NULL ;
        }
    }
*/
}

double CPstnMan::GetValue(int _iAxisNo , EN_PSTN_VALUE _iPstnNo)
{
    if( _iAxisNo < 0 || _iAxisNo >= MAX_MOTR          ) {ShowMessageT("Motor Axis Range is wrong"    ); return 0.0 ;}
    if( _iPstnNo < 0 || _iPstnNo >= PSTN_CNT[_iAxisNo]) {
        ShowMessageT("Position Index Range is wrong");
        return 0.0 ;
    }

    return Value[_iAxisNo][_iPstnNo].dValue ;
}

EN_VALUE_TYPE CPstnMan::GetType(int _iAxisNo , EN_PSTN_VALUE _iPstnNo)
{
    if( _iAxisNo < 0 || _iAxisNo >= MAX_MOTR          ) {ShowMessageT("Motor Axis Range is wrong"    ); return 0.0 ;}
    if( _iPstnNo < 0 || _iPstnNo >= PSTN_CNT[_iAxisNo]) {
        ShowMessageT("Position Index Range is wrong");
        return 0.0 ;
    }

    return Value[_iAxisNo][_iPstnNo].iType ;
}

bool CPstnMan::SetType  (int _iAxisNo , EN_PSTN_VALUE _iPstnNo , EN_VALUE_TYPE _iType)
{
    if( _iAxisNo < 0 || _iAxisNo >= MAX_MOTR          ) {ShowMessageT("Motor Axis Range is wrong"    ); return 0.0 ;}
    if( _iPstnNo < 0 || _iPstnNo >= PSTN_CNT[_iAxisNo]) {
        ShowMessageT("Position Index Range is wrong");
        return 0.0 ;
    }

    Value[_iAxisNo][_iPstnNo].iType = _iType ;

    return true ;

}

double CPstnMan::SetValue(int _iAxisNo , EN_PSTN_VALUE _iPstnNo , double _dValue )
{
    if( _iAxisNo < 0 || _iAxisNo >= MAX_MOTR          ) {ShowMessageT("Motor Axis Range is wrong"    ); return 0.0 ;}
    if( _iPstnNo < 0 || _iPstnNo >= PSTN_CNT[_iAxisNo]) {
        ShowMessageT("Position Index Range is wrong");
        return 0.0 ;
    }

    if(Value[_iAxisNo][_iPstnNo].iType == vtPosition) {
        if(_dValue > MT_GetMaxPos(_iAxisNo) || _dValue < MT_GetMinPos(_iAxisNo)) {
            ShowMessageT("Position Range is wrong(Check Position Limit Max Or Min)");
            return 0.0 ;
        }
    }


    Value[_iAxisNo][_iPstnNo].dValue = _dValue ;
    m_pPstnList[_iAxisNo]->Cells[1][_iPstnNo] = Value[_iAxisNo][_iPstnNo].dValue ;

    return Value[_iAxisNo][_iPstnNo].dValue ;


}

/*
bool CPstnMan::CmprPos(int _iAxisNo , EN_PSTN_VALUE _iPstnNo)
{
    if( _iAxisNo < 0 || _iAxisNo >= MAX_MOTR          ) {ShowMessageT("Motor Axis Range is wrong"    ); return 0.0 ;}
    if( _iPstnNo < 0 || _iPstnNo >= PSTN_CNT[_iAxisNo]) {ShowMessageT("Position Index Range is wrong"); return 0.0 ;}

    return MT_CmprPos(_iAxisNo , Value[_iAxisNo][_iPstnNo].dValue ) ;

}
*/
void CPstnMan::Save(AnsiString sJobName)
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sPath   ;

    //Save Device.
    for(int m = 0 ; m < MAX_MOTR ; m++) {
        //Set Dir.
        sPath = EXE_FOLDER + "JobFile\\"+sJobName+"\\"+MT_GetName(m)+"_Pos.INI" ;
        UserINI.ClearFile(sPath) ;

        for(int i = 0 ; i < PSTN_CNT[m] ; i++) {
            UserINI.Save(sPath, (AnsiString)i+"_Pstn" , "sName " , Value[m][i].sName  );
            UserINI.Save(sPath, (AnsiString)i+"_Pstn" , "dValue" , Value[m][i].dValue );
            UserINI.Save(sPath, (AnsiString)i+"_Pstn" , "iType " , Value[m][i].iType  );
        }
    }
}

void CPstnMan::Load(AnsiString sJobName)
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sPath   ;

    //Load Device.
    for(int m = 0 ; m < MAX_MOTR ; m++) {
        //Set Dir.
        sPath = EXE_FOLDER + "JobFile\\"+sJobName+"\\"+MT_GetName(m)+"_Pos.INI" ;
        for(int i = 0 ; i < PSTN_CNT[m] ; i++) {
            UserINI.Load(sPath, (AnsiString)i+"_Pstn" , "dValue" , Value[m][i].dValue );
        }
    }
}

void CPstnMan::UpdatePstn(bool _bToTable)
{
     if(_bToTable) {
        for(int m = 0 ; m < MAX_MOTR ; m++) {
            for(int i = 0 ; i < PSTN_CNT[m] ; i++) {
                m_pPstnList[m] -> Cells[0][i] = (Value[m][i].sName != ""  ) ?             Value[m][i].sName  : "NoName"+(AnsiString)i;
                m_pPstnList[m] -> Cells[1][i] = (Value[m][i].dValue != 0.0) ? (AnsiString)Value[m][i].dValue : (AnsiString)"0.0"     ;
            }
        }
    }

    else {
        for(int m = 0 ; m < MAX_MOTR ; m++) {
            for(int i = 0 ; i < PSTN_CNT[m] ; i++) {
                if(Value[m][i].iType != vtOffset){
                         if(m_pPstnList[m] -> Cells[1][i].ToDouble() > MT_GetMaxPos(m)){ FM_MsgOk("Warnning" , "움직이려는 위치가 이동 가능한 최대 위치를 벗어 납니다." );}
                    else if(m_pPstnList[m] -> Cells[1][i].ToDouble() < MT_GetMinPos(m)){ FM_MsgOk("Warnning" , "움직이려는 위치가 이동 가능한 최소 위치를 벗어 납니다." );}
                    else {
                        Value[m][i].sName  = m_pPstnList[m] -> Cells[0][i]           ;
                        Value[m][i].dValue = m_pPstnList[m] -> Cells[1][i].ToDouble();
                    }
                }
                else {
                    Value[m][i].sName  = m_pPstnList[m] -> Cells[0][i]           ;
                    Value[m][i].dValue = m_pPstnList[m] -> Cells[1][i].ToDouble();
                }
            }
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall CPstnMan::pPstnListDrawCell(TObject *Sender, int ACol,
      int ARow, const TRect &Rect, TGridDrawState State)
{
    TRect CellRect,TextRect;
    TStringGrid* StringGrid = static_cast<TStringGrid*>(Sender);
    int iSel = StringGrid -> Tag ;
    TCanvas* ACanvas = StringGrid->Canvas;

    CellRect = Rect ;

    if( ACol == 0) {
    }

    if( ACol == 1) {


        ACanvas->Brush -> Style = bsClear ;
        ACanvas->Pen   -> Color = clBlack ;

        TextRect = CellRect;
        TextRect.Left = TextRect.Left ;
        ACanvas->FillRect (CellRect) ;
        ACanvas->Rectangle(CellRect) ;

//        if(GridInfo.bDown ) {
//            if(ARow == GridInfo.iRow && ACol == GridInfo.iCol ) {
//                m_pPstnEdit[GridInfo.iGrid] -> Left    = GridInfo.iColX ;
//                m_pPstnEdit[GridInfo.iGrid] -> Top     = GridInfo.iRowY ;
//                m_pPstnEdit[GridInfo.iGrid] -> Visible = true ;
//                GridInfo.bEdit = true ;
//            }
//        }
    }

    if( ACol == 2) {
         AnsiString CellText = "GO";



         unsigned char step = 30;
         unsigned char r = 0x80,g = 0x80,b = 0xC0;
         TColor clr_highlight = static_cast<TColor>( PALETTERGB(r+step,g+step,b+step) );
         TColor clr_shadow = static_cast<TColor>( PALETTERGB(r-step,g-step,b-step) );

         TextRect = CellRect;
         TextRect.Left = TextRect.Left  + ((GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol ) ? 16 : 15) ;
         TextRect.Top  = TextRect.Top   + ((GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol ) ? 1  : 0 ) ;

         // 색 바꾸기 이거 참조....
         if(Value[iSel][ARow].iType == vtPosition || Value[iSel][ARow].iType == vtFixValue) {
             ACanvas->Brush->Color = (TColor)RGB(128,128,192);
             ACanvas->FillRect( CellRect );
             if(GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol && iSel == GridInfo.iGrid)
                  Frame3D( ACanvas,CellRect,clr_shadow,clr_highlight,2 );
             else Frame3D( ACanvas,CellRect,clr_highlight,clr_shadow,2 );

             DrawText( ACanvas->Handle,CellText.c_str(),CellText.Length(),&TextRect,
                                    DT_LEFT | DT_VCENTER | DT_SINGLELINE );
         }
         else {
             ACanvas->Brush->Color = (TColor)RGB(255,255,255);
             ACanvas->FillRect( CellRect );
         }
    }

    if( ACol == 3) {
         AnsiString CellText = "INPUT";

         unsigned char step = 30;
         unsigned char r = 0xC0,g = 0xC0,b = 0x80;
         TColor clr_highlight = static_cast<TColor>( PALETTERGB(r+step,g+step,b+step) );
         TColor clr_shadow = static_cast<TColor>( PALETTERGB(r-step,g-step,b-step) );

         TextRect = CellRect ;
         TextRect.Left = TextRect.Left  + ((GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol ) ? 6  : 5 ) ;
         TextRect.Top  = TextRect.Top   + ((GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol ) ? 1  : 0 ) ;

         if(Value[iSel][ARow].iType == vtPosition ) {
             ACanvas->Brush->Color = (TColor)RGB(192,192,128);
             ACanvas->FillRect( CellRect );
             if(GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol && iSel == GridInfo.iGrid)
                  Frame3D( ACanvas,CellRect,clr_shadow,clr_highlight,2 );
             else Frame3D( ACanvas,CellRect,clr_highlight,clr_shadow,2 );

             DrawText( ACanvas->Handle,CellText.c_str(),CellText.Length(),&TextRect,
                                    DT_LEFT | DT_VCENTER | DT_SINGLELINE );
         }
         else {
             ACanvas->Brush->Color = (TColor)RGB(255,255,255);
             ACanvas->FillRect( CellRect );
         }
     }
}

//---------------------------------------------------------------------------
void __fastcall CPstnMan::pPstnListMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int iSel      = ((TStringGrid *)Sender)-> Tag ;
    int iX        = X ;
    int iTempColX = 0 ;

    for( int i=0;i<MAX_MOTR;i++){
        if(m_pPstnEdit[i] -> Visible ) {m_pPstnEdit[i] -> SetFocus(); return ; }
    }

    GridInfo.iGrid = iSel ;
    GridInfo.bDown = true ;
    for(int i = 0 ; i < m_pPstnList[iSel] -> ColCount ; i ++) {
        if((iX -= m_pPstnList[iSel] -> ColWidths[i] )> 0) {
            iTempColX += m_pPstnList[iSel] -> ColWidths[i] ;
            continue ;
        }
        GridInfo.iCol = i;
        break ;
    }

    GridInfo.iRow  = Y/m_pPstnList[iSel]->DefaultRowHeight;

    if(GridInfo.iRow >= PSTN_CNT[iSel]) return ;
    GridInfo.iX    = X ;
    GridInfo.iY    = Y ;
    GridInfo.iColX = iTempColX ;
    GridInfo.iRowY = GridInfo.iRow * m_pPstnList[iSel]->DefaultRowHeight;

    m_pPstnEdit[GridInfo.iGrid] -> Text = m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow] ;
    if(1 == GridInfo.iCol && Value[(EN_MOTR_ID)GridInfo.iGrid][(EN_PSTN_ID)GridInfo.iRow].iType != vtFixValue) {
        m_pPstnEdit[GridInfo.iGrid] -> Left    = GridInfo.iColX ;
        m_pPstnEdit[GridInfo.iGrid] -> Top     = GridInfo.iRowY + (int)GridInfo.iGrid;
        m_pPstnEdit[GridInfo.iGrid] -> Visible = true ;
        m_pPstnEdit[GridInfo.iGrid] -> SetFocus()     ;
    }

    if(2 == GridInfo.iCol ) {
         AnsiString sTemp ;
         sTemp = " going to goto " + Value[GridInfo.iGrid][GridInfo.iRow].sName + " Pos" ;
         Trace(MT_GetName(GridInfo.iGrid).c_str(),sTemp.c_str());
         if(GridInfo.iGrid == miSTG_ZCst) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miSTG_XTrs) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miHED_XHed) HED.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miHED_YHed) HED.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miSTG_TStg) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miHED_ZVsn) HED.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miHED_TZom) HED.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
    }

    if(3 == GridInfo.iCol ) {
        Trace(MT_GetName(GridInfo.iGrid).c_str() , (m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow]+"->"+MT_GetCmdPos(GridInfo.iGrid)).c_str());
        if(Value[(EN_MOTR_ID)GridInfo.iGrid][(EN_PSTN_ID)GridInfo.iRow].iType == vtPosition) m_pPstnList[GridInfo.iGrid] -> Cells[1][GridInfo.iRow] = MT_GetCmdPos(GridInfo.iGrid) ;
    }

    m_pPstnList[GridInfo.iGrid] -> Repaint() ;
}
//---------------------------------------------------------------------------
void __fastcall CPstnMan::pPstnListMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    GridInfo.bDown = false ;
    m_pPstnList[GridInfo.iGrid] -> Repaint() ;
}
void CPstnMan::SetParent(TPanel * _pnBase , int _iAxisNo)
{
    m_pPstnList[_iAxisNo]-> Parent = _pnBase ;
    m_pPstnEdit[_iAxisNo]-> Parent = _pnBase ;
}
void CPstnMan::SetWindow(TPanel * _pnBase , int _iAxisNo)
{
    if(_iAxisNo >= MAX_MOTR) {ShowMessageT("Motor Count is wrong"); return ; }

    const int iPosCelHeight = 30 ;
//    const int iPosCelWidth  = 120 ;

    m_pPstnList[_iAxisNo]                    = new TStringGrid(_pnBase);
    m_pPstnList[_iAxisNo]-> Tag              = _iAxisNo;
    m_pPstnList[_iAxisNo]-> Parent           = _pnBase ;
    m_pPstnList[_iAxisNo]-> Tag              = _iAxisNo  ;
    m_pPstnList[_iAxisNo]-> ColCount         = 4  ;
    m_pPstnList[_iAxisNo]-> DefaultRowHeight = iPosCelHeight ;
    m_pPstnList[_iAxisNo]-> DefaultColWidth  = 65 ;
    m_pPstnList[_iAxisNo]-> ColWidths[0]     = NAME_COL_WIDTH  ;
    m_pPstnList[_iAxisNo]-> ColWidths[1]     = POS_COL_WIDTH   ;
    m_pPstnList[_iAxisNo]-> ColWidths[2]     = GO_COL_WIDTH    ;
    m_pPstnList[_iAxisNo]-> ColWidths[3]     = INPUT_COL_WIDTH ;
    m_pPstnList[_iAxisNo]-> FixedRows        = 0  ;
    m_pPstnList[_iAxisNo]-> FixedCols        = 1  ;
    m_pPstnList[_iAxisNo]-> Options          >> goEditing     ;
    m_pPstnList[_iAxisNo]-> Options          << goRowSelect   ;
    m_pPstnList[_iAxisNo]-> Options          >> goRangeSelect ;
    m_pPstnList[_iAxisNo]-> ScrollBars       = ssVertical     ;
    m_pPstnList[_iAxisNo]-> Align            = alClient       ;
    m_pPstnList[_iAxisNo]-> Font -> Charset  = HANGEUL_CHARSET;
    m_pPstnList[_iAxisNo]-> Font -> Size     = 11             ;
    m_pPstnList[_iAxisNo]-> Font -> Name     = "MS Sans Serif" ;

    m_pPstnList[_iAxisNo]-> OnMouseDown      = pPstnListMouseDown ;
    m_pPstnList[_iAxisNo]-> OnMouseUp        = pPstnListMouseUp   ;
    m_pPstnList[_iAxisNo]-> OnDrawCell       = pPstnListDrawCell  ;

    m_pPstnList[_iAxisNo]-> RowCount         = PSTN_CNT[_iAxisNo] ;
    m_pPstnList[_iAxisNo]-> Visible          = true ;

    m_pPstnEdit[_iAxisNo]                = new TEdit(_pnBase);
    m_pPstnEdit[_iAxisNo]-> Parent       = _pnBase           ;
    m_pPstnEdit[_iAxisNo]-> ImeMode      = imDisable         ;
/*
    m_pPstnEdit[_iAxisNo]-> NumberOnly   = true              ;
    m_pPstnEdit[_iAxisNo]-> UseApoint    = true              ;
    m_pPstnEdit[_iAxisNo]-> UseMinus     = true              ;
    m_pPstnEdit[_iAxisNo]-> UseComma     = false             ;
*/
    //m_pPstnEdit[_iAxisNo]-> Alignment    = taRightJustify    ;
    m_pPstnEdit[_iAxisNo]-> Height       = iPosCelHeight     ;
    m_pPstnEdit[_iAxisNo]-> Width        = GO_COL_WIDTH      ;




//    m_pPstnList[_iAxisNo]-> Font -> Size = 8                 ;

    m_pPstnEdit[_iAxisNo]-> OnKeyDown    = pPstnEditKeyDown  ;
    m_pPstnEdit[_iAxisNo]-> OnExit       = pPstnEditExit     ;
    m_pPstnEdit[_iAxisNo]-> Visible      = false ;



//    _pnBase -> Width  = NAME_COL_WIDTH + POS_COL_WIDTH + GO_COL_WIDTH + INPUT_COL_WIDTH + 11 ;

//    _pnBase -> Height = m_pPstnList[_iAxisNo]->Height ; // ((iPosCelHeight) * (int)(PSTN_CNT[_iAxisNo])) + (int)(PSTN_CNT[_iAxisNo]) - 1 ;

}

//---------------------------------------------------------------------------
void __fastcall CPstnMan::pPstnEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
/*
   if(Key == VK_RETURN){
        Trace(MT_GetName(GridInfo.iGrid).c_str() , (m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow]+"->"+m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble()).c_str());
        m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow] = m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble() ;
        m_pPstnEdit[GridInfo.iGrid] -> Visible = false ;
        m_pPstnList[GridInfo.iGrid] -> Repaint() ;
   }
   else if(Key == VK_ESCAPE) {
        m_pPstnEdit[GridInfo.iGrid] -> Visible = false ;
        m_pPstnList[GridInfo.iGrid] -> Repaint() ;
   }
*/
    if(Key == VK_RETURN){
        if(m_pPstnEdit[GridInfo.iGrid] -> Text == "") m_pPstnEdit[GridInfo.iGrid] -> Text = 0.0 ;
        //TEdit 교체형 인클루드 안하기 위해.
        //m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow] = m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble() ;
         Trace(MT_GetName(GridInfo.iGrid).c_str() , (m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow]+"->"+(double)StrToFloatDef( m_pPstnEdit[GridInfo.iGrid] -> Text , Value[GridInfo.iGrid][GridInfo.iRow].dValue)).c_str());
        m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow] = (double)StrToFloatDef( m_pPstnEdit[GridInfo.iGrid] -> Text , Value[GridInfo.iGrid][GridInfo.iRow].dValue) ;
        //Value[m][i].dValue = m_pPstnList[m] -> Cells[1][i].ToDouble();

        //Trace(MT_GetName(GridInfo.iGrid).c_str() , (m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow]+"->"+m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble()).c_str());
        m_pPstnEdit[GridInfo.iGrid] -> Visible = false ;
        m_pPstnList[GridInfo.iGrid] -> Repaint() ;
    }
    else if(Key == VK_ESCAPE) {
        m_pPstnEdit[GridInfo.iGrid] -> Visible = false ;
        m_pPstnList[GridInfo.iGrid] -> Repaint() ;
    }

}


void __fastcall CPstnMan::pPstnEditExit(TObject *Sender)
{
    Trace(MT_GetName(GridInfo.iGrid).c_str() , (m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow]+"->"+(double)StrToFloatDef( m_pPstnEdit[GridInfo.iGrid] -> Text , Value[GridInfo.iGrid][GridInfo.iRow].dValue)).c_str());
    //m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow] = m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble() ;
    m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow] = (double)StrToFloatDef( m_pPstnEdit[GridInfo.iGrid] -> Text , Value[GridInfo.iGrid][GridInfo.iRow].dValue) ;

    m_pPstnEdit[GridInfo.iGrid] -> Visible = false ;
    m_pPstnList[GridInfo.iGrid] -> Repaint() ;

}

void CPstnMan::LoadLastInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Load
    UserINI.Load(sPath, "LAST WORK INFO" , "Device"  , m_sCrntDev );

    if (m_sCrntDev == "") m_sCrntDev  = "NONE";
}

void CPstnMan::SaveLastInfo()
{
    //Local Var.
    TUserINI   UserINI;    
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Save
    UserINI.Save(sPath, "LAST WORK INFO" , "Device"  , m_sCrntDev );
}



