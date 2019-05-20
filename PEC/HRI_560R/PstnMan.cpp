//---------------------------------------------------------------------------
#pragma hdrstop

#include "PstnMan.h"
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "TEdit.h"

#include "Loader.h"
#include "Rail.h"
#include "Head.h"
#include "PstBuff.h"
#include "UnLoader.h"
#include "UnLoaderMotor.h"

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
    if(MAX_MOTR != MT_GetMotorCnt()) {ShowMessageT("Motor Count is wrong");}

    memset(&GridInfo , 0 , sizeof(SGridSelInfo)             );
    memset( Value    , 0 , sizeof(SValue)*MAX_MOTR*MAX_PSTN );

    Value[miLDR_XPsh][pvLDR_XPshWait       ].sName = "Wait        " ;
    Value[miLDR_XPsh][pvLDR_XPshWorkStt    ].sName = "WorkStt     " ;

    Value[miLDR_YFed][pvLDR_YFedWait       ].sName = "Wait        " ;
    Value[miLDR_YFed][pvLDR_YFedWorkStt    ].sName = "WorkStt     " ;
    Value[miLDR_YFed][pvLDR_YFedPlace      ].sName = "Place       " ;

    Value[miLDR_XPck][pvLDR_XPckWait       ].sName = "Wait        " ;
    Value[miLDR_XPck][pvLDR_XPckWorkStt    ].sName = "WorkStt     " ;
    Value[miLDR_XPck][pvLDR_XPckInsert     ].sName = "Insert      " ;

    Value[miLDR_ZPck][pvLDR_ZPckWait       ].sName = "Wait        " ;
    Value[miLDR_ZPck][pvLDR_ZPckWork       ].sName = "Work        " ;
    Value[miLDR_ZPck][pvLDR_ZPckMove       ].sName = "Move        " ;
    Value[miLDR_ZPck][pvLDR_ZPckPlace      ].sName = "Place       " ;

    Value[miWK1_YFlp][pvWK1_YFlpNormal     ].sName = "Normal      " ;
    Value[miWK1_YFlp][pvWK1_YFlpInverse    ].sName = "Inverse     " ;

    Value[miWK1_XIns][pvWK1_XInsWait       ].sName = "Wait        " ;
    Value[miWK1_XIns][pvWK1_XInsStpr       ].sName = "Stopper     " ;
    Value[miWK1_XIns][pvWK1_XInsOut        ].sName = "Out         " ;

    Value[miWRK_XVsn][pvWRK_XVsnWait       ].sName = "Wait        " ;
    Value[miWRK_XVsn][pvWRK_XVsnWorkStart  ].sName = "WorkStart   " ;
    Value[miWRK_XVsn][pvWRK_XVsnWorkEnd    ].sName = "WorkEnd     " ;

    Value[miWRK_YVsn][pvWRK_YVsnWait       ].sName = "Wait        " ;
    Value[miWRK_YVsn][pvWRK_YVsnWorkStart  ].sName = "WorkStart   " ;

    Value[miPSB_XMrk][pvPSB_XMrkWait       ].sName = "Wait        " ;
    Value[miPSB_XMrk][pvPSB_XMrkVisnStart  ].sName = "VisnStart   " ;
    Value[miPSB_XMrk][pvPSB_XMrkWorkStart  ].sName = "WorkStart   " ;

    Value[miPSB_YMrk][pvPSB_YMrkWait       ].sName = "Wait        " ;
    Value[miPSB_YMrk][pvPSB_YMrkVisnStart  ].sName = "VisnStart   " ;
    Value[miPSB_YMrk][pvPSB_YMrkWorkStart  ].sName = "WorkStart   " ;

    Value[miULD_ZPck][pvULD_ZPckWait       ].sName = "Wait        " ;
    Value[miULD_ZPck][pvULD_ZPckMove       ].sName = "Move        " ;
    Value[miULD_ZPck][pvULD_ZPckPick       ].sName = "Pick        " ;
    Value[miULD_ZPck][pvULD_ZPckPlace      ].sName = "Place       " ;
    Value[miULD_ZPck][pvULD_ZPckPaper      ].sName = "Paper       " ;

    Value[miULD_YPck][pvULD_YPckWait       ].sName = "Wait        " ;
    Value[miULD_YPck][pvULD_YPckPick       ].sName = "Pick        " ;
    Value[miULD_YPck][pvULD_YPckPlaceStt   ].sName = "PlaceStt    " ;
    Value[miULD_YPck][pvULD_YPckPaper      ].sName = "Paper       " ;

    Value[miULD_ZTrI][pvULD_ZTrIWait       ].sName = "Wait        " ;
    Value[miULD_ZTrI][pvULD_ZTrIWorkStt    ].sName = "WorkStt     " ;
    Value[miULD_ZTrI][pvULD_ZTrIWorkEnd    ].sName = "WorkEnd     " ;

    Value[miULD_ZTrO][pvULD_ZTrOWait       ].sName = "Wait        " ;
    Value[miULD_ZTrO][pvULD_ZTrOWorkStt    ].sName = "WorkStt     " ;
    Value[miULD_ZTrO][pvULD_ZTrOWorkEnd    ].sName = "WorkEnd     " ;

    Value[miULD_XStp][pvULD_XStpWait       ].sName = "Wait        " ;
    Value[miULD_XStp][pvULD_XStpBwd        ].sName = "Bwd         " ;
    Value[miULD_XStp][pvULD_XStpFwd        ].sName = "Fwd         " ;

    Value[miULD_ZPpr][pvULD_ZPprWait       ].sName = "Wait        " ;
    Value[miULD_ZPpr][pvULD_ZPprWorkStt    ].sName = "WorkStt     " ;
    Value[miULD_ZPpr][pvULD_ZPprWorkEnd    ].sName = "WorkEnd     " ;


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
                         if(m_pPstnList[m] -> Cells[1][i].ToDouble() > MT_GetMaxPos(m)){ FM_MsgOk("Warnning" , "Motor Max Position is larger than Move Position.");}
                    else if(m_pPstnList[m] -> Cells[1][i].ToDouble() < MT_GetMinPos(m)){ FM_MsgOk("Warnning" , "Motor Min Position is small than Move Position." );}
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


         if(Value[iSel][ARow].iType == vtPosition) {
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

         if(Value[iSel][ARow].iType == vtPosition) {
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
    if(1 == GridInfo.iCol ) {
        m_pPstnEdit[GridInfo.iGrid] -> Left    = GridInfo.iColX ;
        m_pPstnEdit[GridInfo.iGrid] -> Top     = GridInfo.iRowY ;
        m_pPstnEdit[GridInfo.iGrid] -> Visible = true ;
        m_pPstnEdit[GridInfo.iGrid] -> SetFocus()     ;
    }

    if(2 == GridInfo.iCol ) {
         AnsiString sTemp ;
         sTemp = " going to goto " + Value[GridInfo.iGrid][GridInfo.iRow].sName + " Pos" ;
         Trace(MT_GetName(GridInfo.iGrid).c_str(),sTemp.c_str());
         if(GridInfo.iGrid == miLDR_XPsh) LDR.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miLDR_YFed) LDR.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miLDR_XPck) LDR.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miLDR_ZPck) LDR.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWK1_YFlp) RAL.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWK1_XIns) RAL.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRK_XVsn) HED.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRK_YVsn) HED.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miPSB_XMrk) PSB.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miPSB_YMrk) PSB.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miULD_ZPck) ULD.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miULD_YPck) ULD.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miULD_ZTrI) ULM.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miULD_ZTrO) ULM.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miULD_XStp) ULD.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miULD_ZPpr) ULM.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);






    }

    if(3 == GridInfo.iCol ) {
        Trace(MT_GetName(GridInfo.iGrid).c_str() , (m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow]+"->"+MT_GetCmdPos(GridInfo.iGrid)).c_str());
        m_pPstnList[GridInfo.iGrid] -> Cells[1][GridInfo.iRow] = MT_GetCmdPos(GridInfo.iGrid) ;
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

    const int iPosCelHeight = 30 ;
    const int iPosCelWidth  = 120 ;

    m_pPstnList[_iAxisNo]                    = new TStringGrid(_pnBase);
    m_pPstnList[_iAxisNo]-> Tag              = _iAxisNo;
    m_pPstnList[_iAxisNo]-> Parent           = _pnBase ;
    m_pPstnList[_iAxisNo]-> Tag              = _iAxisNo  ;
    m_pPstnList[_iAxisNo]-> ColCount         = 4  ;
    m_pPstnList[_iAxisNo]-> DefaultRowHeight = iPosCelHeight ;
    m_pPstnList[_iAxisNo]-> DefaultColWidth  = 65 ;
    m_pPstnList[_iAxisNo]-> ColWidths[0]     = 110 ;
    m_pPstnList[_iAxisNo]-> ColWidths[1]     = iPosCelWidth ;
    m_pPstnList[_iAxisNo]-> ColWidths[2]     = 60 ;
    m_pPstnList[_iAxisNo]-> ColWidths[3]     = 70 ;
    m_pPstnList[_iAxisNo]-> FixedRows        = 0  ;
    m_pPstnList[_iAxisNo]-> FixedCols        = 1  ;
    m_pPstnList[_iAxisNo]-> Options          >> goEditing     ;
    m_pPstnList[_iAxisNo]-> Options          << goRowSelect   ;
    m_pPstnList[_iAxisNo]-> Options          >> goRangeSelect ;
    m_pPstnList[_iAxisNo]-> ScrollBars       = ssVertical     ;
    m_pPstnList[_iAxisNo]-> Align            = alClient       ;
    m_pPstnList[_iAxisNo]-> Font -> Charset  = ARABIC_CHARSET ;
    m_pPstnList[_iAxisNo]-> Font -> Size     = 14             ;
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
//    m_pPstnList[_iAxisNo]-> Font -> Size = 8                 ;
    m_pPstnEdit[_iAxisNo]-> OnKeyDown    = pPstnEditKeyDown  ;
    m_pPstnEdit[_iAxisNo]-> OnExit       = pPstnEditExit     ;
    m_pPstnEdit[_iAxisNo]-> Visible      = false ;
}

//---------------------------------------------------------------------------
void __fastcall CPstnMan::pPstnEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   if(Key == VK_RETURN){
        if(m_pPstnEdit[GridInfo.iGrid] -> Text == "") m_pPstnEdit[GridInfo.iGrid] -> Text = 0.0 ;   
        Trace(MT_GetName(GridInfo.iGrid).c_str() , (m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow]+"->"+m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble()).c_str());
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
    Trace(MT_GetName(GridInfo.iGrid).c_str() , (m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow]+"->"+m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble()).c_str());
    m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow] = m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble() ;
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


