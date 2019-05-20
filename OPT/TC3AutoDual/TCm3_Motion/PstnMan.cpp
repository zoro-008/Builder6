//---------------------------------------------------------------------------


#pragma hdrstop

#include "PstnMan.h"
#include "UserINI.h"
#include "Motors.h"
#include "SLogUnit.h"
#include "Sequence.h"

//--------------------------------------
#pragma package(smart_init)

#define NAME_COL_WIDTH   35
#define POS_COL_WIDTH    35
#define GO_COL_WIDTH     20
#define INPUT_COL_WIDTH  20

CPstnMan PM;
//---------------------------------------------------------------------------
CPstnMan::CPstnMan()
{
    if(MAX_MOTR != MT.GetMotorCnt()) {ShowMessageT("Motor Count is wrong");}

    memset(&GridInfo , 0 , sizeof(TGridSelInfo));
}

CPstnMan::~CPstnMan()
{
/* parent 할당시에 자동 삭제 됌.
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

double CPstnMan::GetPos(int _iAxisNo , int _iPstnNo)
{

    if( _iAxisNo < 0 || _iAxisNo >= MAX_MOTR          ) {ShowMessageT("Motor Axis Range is wrong"    ); return 0.0 ;}
    if( _iPstnNo < 0 || _iPstnNo >= PSTN_CNT[_iAxisNo]) {ShowMessageT("Position Index Range is wrong"); return 0.0 ;}

    return m_pPstn[_iAxisNo][_iPstnNo].dPstn ;
}

bool CPstnMan::CmprPos(int _iAxisNo , int _iPstnNo , double _dRange)
{
    if( _iAxisNo < 0 || _iAxisNo >= MAX_MOTR          ) {ShowMessageT("Motor Axis Range is wrong"    ); return 0.0 ;}
    if( _iPstnNo < 0 || _iPstnNo >= PSTN_CNT[_iAxisNo]) {ShowMessageT("Position Index Range is wrong"); return 0.0 ;}

    return MT.CmprPos(_iAxisNo , m_pPstn[_iAxisNo][_iPstnNo].dPstn , _dRange) ;

}

void CPstnMan::Save()
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sPath   ;

    //Save Device.
    for(int m = 0 ; m < MAX_MOTR ; m++) {
        //Set Dir.
        sPath = EXE_FOLDER + "JobFile\\None\\"+MT.GetName(m)+"_Pos.INI" ;
        UserINI.ClearFile(sPath) ;

        for(int i = 0 ; i < PSTN_CNT[m] ; i++) {
            UserINI.Save(sPath, (AnsiString)i+"_Pstn" , "sName" , m_pPstn[m][i].sName );
            UserINI.Save(sPath, (AnsiString)i+"_Pstn" , "dPstn" , m_pPstn[m][i].dPstn );
        }
    }
}

void CPstnMan::Load()
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sPath   ;

    //Load Device.
    for(int m = 0 ; m < MAX_MOTR ; m++) {
        //Set Dir.
        sPath = EXE_FOLDER + "JobFile\\None\\"+MT.GetName(m)+"_Pos.INI" ;
        for(int i = 0 ; i < PSTN_CNT[m] ; i++) {
            UserINI.Load(sPath, (AnsiString)i+"_Pstn" , "sName" , m_pPstn[m][i].sName );
            UserINI.Load(sPath, (AnsiString)i+"_Pstn" , "dPstn" , m_pPstn[m][i].dPstn );
        }
    }
}

void CPstnMan::UpdatePstn(bool _bToTable)
{
    if(_bToTable) {
        for(int m = 0 ; m < MAX_MOTR ; m++) {
            for(int i = 0 ; i < PSTN_CNT[m] ; i++) {
                m_pPstnList[m] -> Cells[0][i] = (m_pPstn[m][i].sName != "" ) ?             m_pPstn[m][i].sName : "NoName"+(AnsiString)i;
                m_pPstnList[m] -> Cells[1][i] = (m_pPstn[m][i].dPstn != 0.0) ? (AnsiString)m_pPstn[m][i].dPstn : (AnsiString)"0.0"     ;
            }
        }
    }

    else {
        for(int m = 0 ; m < MAX_MOTR ; m++) {
            for(int i = 0 ; i < PSTN_CNT[m] ; i++) {
                m_pPstn[m][i].sName = m_pPstnList[m] -> Cells[0][i]           ;
                m_pPstn[m][i].dPstn = m_pPstnList[m] -> Cells[1][i].ToDouble();
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

       ACanvas->Brush->Color = (TColor)RGB(128,128,192);

       unsigned char step = 30;
       unsigned char r = 0x80,g = 0x80,b = 0xC0;
       TColor clr_highlight = static_cast<TColor>( PALETTERGB(r+step,g+step,b+step) );
       TColor clr_shadow = static_cast<TColor>( PALETTERGB(r-step,g-step,b-step) );

       TextRect = CellRect;
       TextRect.Left = TextRect.Left  + ((GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol ) ? 16 : 15) ;
       TextRect.Top  = TextRect.Top   + ((GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol ) ? 1  : 0 ) ;

       ACanvas->FillRect( CellRect );
       if(GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol )
            Frame3D( ACanvas,CellRect,clr_shadow,clr_highlight,2 );
       else Frame3D( ACanvas,CellRect,clr_highlight,clr_shadow,2 );

       DrawText( ACanvas->Handle,CellText.c_str(),CellText.Length(),&TextRect,
                              DT_LEFT | DT_VCENTER | DT_SINGLELINE );
    }

    if( ACol == 3) {
       AnsiString CellText = "INPUT";

       ACanvas->Brush->Color = (TColor)RGB(192,192,128);

       unsigned char step = 30;
       unsigned char r = 0xC0,g = 0xC0,b = 0x80;
       TColor clr_highlight = static_cast<TColor>( PALETTERGB(r+step,g+step,b+step) );
       TColor clr_shadow = static_cast<TColor>( PALETTERGB(r-step,g-step,b-step) );

       TextRect = CellRect ;
       TextRect.Left = TextRect.Left  + ((GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol ) ? 6  : 5 ) ;
       TextRect.Top  = TextRect.Top   + ((GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol ) ? 1  : 0 ) ;

       ACanvas->FillRect( CellRect );

       if(GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol && iSel == GridInfo.iGrid)
            Frame3D( ACanvas,CellRect,clr_shadow,clr_highlight,2 );
       else Frame3D( ACanvas,CellRect,clr_highlight,clr_shadow,2 );

       DrawText( ACanvas->Handle,CellText.c_str(),CellText.Length(),&TextRect,
                              DT_LEFT | DT_VCENTER | DT_SINGLELINE );
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
    if(1 == GridInfo.iCol ) {
        m_pPstnEdit[GridInfo.iGrid] -> Left    = GridInfo.iColX ;
        m_pPstnEdit[GridInfo.iGrid] -> Top     = GridInfo.iRowY ;
        m_pPstnEdit[GridInfo.iGrid] -> Visible = true ;
        m_pPstnEdit[GridInfo.iGrid] -> SetFocus()     ;
    }

    if(2 == GridInfo.iCol ) {
         AnsiString sTemp ;
         sTemp = " going to goto " + m_pPstn[GridInfo.iGrid][GridInfo.iRow].sName + " Pos" ;
         Trace(MT.GetName(GridInfo.iGrid).c_str(),sTemp.c_str());
         if(GridInfo.iGrid == miLT_Z || GridInfo.iGrid == miLT_Y || GridInfo.iGrid == miLT_I) {/*if(mrOk==MessageBox(NULL , "Motor Move" , sTemp.c_str() , MB_OKCANCEL ))*/ SEQ.LT.MoveMotr(GridInfo.iGrid , GridInfo.iRow);}
         if(GridInfo.iGrid == miRT_Z || GridInfo.iGrid == miRT_Y || GridInfo.iGrid == miRT_I) {/*if(mrOk==MessageBox(NULL , "Motor Move" , sTemp.c_str() , MB_OKCANCEL ))*/ SEQ.RT.MoveMotr(GridInfo.iGrid , GridInfo.iRow);}
    }

    if(3 == GridInfo.iCol ) {
        Trace(MT.GetName(GridInfo.iGrid).c_str() , (m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow]+"->"+MT.GetCmdPos(GridInfo.iGrid)).c_str());
        m_pPstnList[GridInfo.iGrid] -> Cells[1][GridInfo.iRow] = MT.GetCmdPos(GridInfo.iGrid) ;
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

void CPstnMan::SetWindow(TPanel * _pnBase , int _iAxisNo)
{
    if(_iAxisNo >= MAX_MOTR) {ShowMessageT("Motor Count is wrong"); return ; }

    const int iPosCelHeight = 20 ;
    const int iPosCelWidth  = 80 ;

    m_pPstnList[_iAxisNo]                    = new TStringGrid(_pnBase);
    m_pPstnList[_iAxisNo]-> Tag              = _iAxisNo;
    m_pPstnList[_iAxisNo]-> Parent           = _pnBase ;
    m_pPstnList[_iAxisNo]-> Tag              = _iAxisNo  ;
    m_pPstnList[_iAxisNo]-> ColCount         = 4  ;
    m_pPstnList[_iAxisNo]-> DefaultRowHeight = iPosCelHeight ;
    m_pPstnList[_iAxisNo]-> DefaultColWidth  = 65 ;
    m_pPstnList[_iAxisNo]-> ColWidths[0]     = 80 ;
    m_pPstnList[_iAxisNo]-> ColWidths[1]     = iPosCelWidth ;
    m_pPstnList[_iAxisNo]-> ColWidths[2]     = 50 ;
    m_pPstnList[_iAxisNo]-> ColWidths[3]     = 50 ;
    m_pPstnList[_iAxisNo]-> FixedRows        = 0  ;
    m_pPstnList[_iAxisNo]-> FixedCols        = 1  ;
    m_pPstnList[_iAxisNo]-> Options          >> goEditing     ;
    m_pPstnList[_iAxisNo]-> Options          << goRowSelect   ;
    m_pPstnList[_iAxisNo]-> Options          >> goRangeSelect ;
    m_pPstnList[_iAxisNo]-> ScrollBars       = ssVertical     ;
    m_pPstnList[_iAxisNo]-> Align            = alClient       ;
    m_pPstnList[_iAxisNo]-> Font -> Size     = 8              ;
    m_pPstnList[_iAxisNo]-> OnMouseDown      = pPstnListMouseDown ;
    m_pPstnList[_iAxisNo]-> OnMouseUp        = pPstnListMouseUp   ;
    m_pPstnList[_iAxisNo]-> OnDrawCell       = pPstnListDrawCell  ;

    m_pPstnList[_iAxisNo]-> RowCount         = PSTN_CNT[_iAxisNo] ;
    m_pPstnList[_iAxisNo]-> Visible          = true ;

    m_pPstnEdit[_iAxisNo]                = new TEdit(_pnBase);
    m_pPstnEdit[_iAxisNo]-> Parent       = _pnBase           ;
    m_pPstnEdit[_iAxisNo]-> NumberOnly   = true              ;
    m_pPstnEdit[_iAxisNo]-> UseApoint    = true              ;
    m_pPstnEdit[_iAxisNo]-> UseMinus     = true              ;
    m_pPstnEdit[_iAxisNo]-> UseComma     = false             ;
    m_pPstnEdit[_iAxisNo]-> Alignment    = taRightJustify    ;
    m_pPstnEdit[_iAxisNo]-> Height       = iPosCelHeight     ;
    m_pPstnEdit[_iAxisNo]-> Width        = iPosCelWidth      ;
    m_pPstnList[_iAxisNo]-> Font -> Size = 8                 ;
    m_pPstnEdit[_iAxisNo]-> OnKeyDown    = pPstnEditKeyDown  ;
    m_pPstnEdit[_iAxisNo]-> OnExit       = pPstnEditExit     ;
    m_pPstnEdit[_iAxisNo]-> Visible      = false ;
}

//---------------------------------------------------------------------------
void __fastcall CPstnMan::pPstnEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   if(Key == VK_RETURN){
        Trace(MT.GetName(GridInfo.iGrid).c_str() , (m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow]+"->"+m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble()).c_str());
        m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow] = m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble() ;
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
    Trace(MT.GetName(GridInfo.iGrid).c_str() , (m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow]+"->"+m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble()).c_str());
    m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow] = m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble() ;
    m_pPstnEdit[GridInfo.iGrid] -> Visible = false ;
    m_pPstnList[GridInfo.iGrid] -> Repaint() ;
}



