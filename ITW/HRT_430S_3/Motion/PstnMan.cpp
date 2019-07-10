
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
#include "Loader.h"
#include "PreBuff.h"
#include "WorkZone.h"
#include "PostBuff.h"
#include "UnLoader430t.h"
#include "SortingTool.h"
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

void CPstnMan::Init(void)
{
    if(MAX_MOTR != MT_GetMotorCnt()) {ShowMessageT("Motor Count is wrong");}

    memset(&GridInfo , 0 , sizeof(SGridSelInfo)             );
    memset( Value    , 0 , sizeof(SValue)*MAX_MOTR*MAX_PSTN );

    Value[miPRB_XCmp][pvPRB_XWaitPs      ].sName = "Wait        " ;  
    Value[miPRB_XCmp][pvPRB_XGripPs      ].sName = "Grip        " ;  
    Value[miPRB_XCmp][pvPRB_XReGripPs    ].sName = "ReGrip      " ;  
    Value[miPRB_XCmp][pvPRB_XPullPs      ].sName = "Pull        " ;  
    Value[miPRB_XCmp][pvPRB_XBackPs      ].sName = "Back        " ;  
    Value[miPRB_XCmp][pvPRB_XIdCheck     ].sName = "ID Check    " ;  
    Value[miPRB_XCmp][pvPRB_XOut1Ps      ].sName = "Out1        " ;  
    Value[miPRB_XCmp][pvPRB_XOut2Ps      ].sName = "Out2        " ;  

    Value[miWRK_XCmp][pvWRK_XWaitPs      ].sName = "Wait        " ;
    Value[miWRK_XCmp][pvWRK_XGripPs      ].sName = "Grip        " ;
    Value[miWRK_XCmp][pvWRK_XWorkStartPs ].sName = "WorkStart   " ;
//    Value[miWRK_XCmp][pvWRK_XChangeGripPs].sName = "ChangeGrip" ;
//    Value[miWRK_XCmp][pvWRK_XReGripPs    ].sName = "ReGrip    " ;
    Value[miWRK_XCmp][pvWRK_XOutPs       ].sName = "Out       " ;

    Value[miPSB_XCmp][pvPSB_XWaitPs     ].sName = "Wait        " ;
    Value[miPSB_XCmp][pvPSB_XGripPs     ].sName = "Grip        " ;
    Value[miPSB_XCmp][pvPSB_XPullPs     ].sName = "Pull        " ;
    Value[miPSB_XCmp][pvPSB_XBackPs     ].sName = "Back        " ;
    Value[miPSB_XCmp][pvPSB_XWorkStartPs].sName = "WorkStartPs " ;
    Value[miPSB_XCmp][pvPSB_XOutPs      ].sName = "Out         " ;
//    Value[miPSB_XCmp][pvPSB_XReGripPs   ].sName = "ReGripOffset" ;
//    Value[miPSB_XCmp][pvPSB_XReGripPs   ].iType =  vtOffset      ;

    Value[miWRK_ZTop][pvWRK_ZBWaitPs     ].sName = "Wait        " ;
    Value[miWRK_ZTop][pvWRK_ZBWorkPs     ].sName = "Work        " ;
    Value[miWRK_ZTop][pvWRK_ZBChangePs   ].sName = "Change      " ;

    Value[miWRK_YBtm][pvWRK_YBWaitPs     ].sName = "Wait        " ;
    Value[miWRK_YBtm][pvWRK_YBWorkPs     ].sName = "Work        " ;
    Value[miWRK_YBtm][pvWRK_YBContectPs  ].sName = "Contect     " ;
    Value[miWRK_YBtm][pvWRK_YBChangePs   ].sName = "Change      " ;

    Value[miWRK_ZBtm][pvWRK_ZTWaitPs     ].sName = "Wait        " ;
    Value[miWRK_ZBtm][pvWRK_ZTWorkPs     ].sName = "Work        " ;
    Value[miWRK_ZBtm][pvWRK_ZTChangePs   ].sName = "Change      " ;

    Value[miLDR_YCmp][pvLDR_YWaitPs      ].sName = "Wait        " ;
    Value[miLDR_YCmp][pvLDR_YPickPs      ].sName = "Pick        " ;
    Value[miLDR_YCmp][pvLDR_YWorkPs      ].sName = "Work        " ;
    Value[miLDR_YCmp][pvLDR_YPlacePs     ].sName = "Place       " ;

    Value[miLDR_ZCmp][pvLDR_ZWaitPs      ].sName = "Wait        " ;
    Value[miLDR_ZCmp][pvLDR_ZPickFwdPs   ].sName = "PickFwd     " ;
    Value[miLDR_ZCmp][pvLDR_ZClampOnPs   ].sName = "ClampOn     " ;
    Value[miLDR_ZCmp][pvLDR_ZPickBwdPs   ].sName = "PickBwd     " ;
    Value[miLDR_ZCmp][pvLDR_ZWorkStartPs ].sName = "WorkStart   " ;
    Value[miLDR_ZCmp][pvLDR_ZPlaceFwdPs  ].sName = "PlaceFwd    " ;
    Value[miLDR_ZCmp][pvLDR_ZClampOffPs  ].sName = "ClampOff    " ;
    Value[miLDR_ZCmp][pvLDR_ZPlaceBwdPs  ].sName = "PlaceBwd    " ;

    Value[miSTL_YTop][pvSTL_YTWaitPs     ].sName = "Wait        " ;
    Value[miSTL_YTop][pvSTL_YTWorkStartPs].sName = "WorkStart   " ;
    Value[miSTL_YTop][pvSTL_YTPlacePs    ].sName = "Place       " ;
    Value[miSTL_YTop][pvSTL_YTDevCheckPs ].sName = "DevCheck    " ;
    Value[miSTL_YTop][pvSTL_YTChangePs   ].sName = "Change      " ;

    Value[miSTL_YBtm][pvSTL_YBWaitPs     ].sName = "Wait        " ;
    Value[miSTL_YBtm][pvSTL_YBWorkStartPs].sName = "WorkStart   " ;
    Value[miSTL_YBtm][pvSTL_YBChangePs   ].sName = "Change      " ;

    Value[miREJ_XBtm][pvREJ_XBWaitPs     ].sName = "Wait        " ;
    Value[miREJ_XBtm][pvREJ_XBWorkStartPs].sName = "WorkStart   " ;

    Value[miULD_YCmp][pvULD_YWaitPs      ].sName = "Wait        " ;
    Value[miULD_YCmp][pvULD_YWorkPs      ].sName = "Work        " ;
    Value[miULD_YCmp][pvULD_YPickPs      ].sName = "Pick        " ;
    Value[miULD_YCmp][pvULD_YPlacePs     ].sName = "Place       " ;

    Value[miULD_ZCmp][pvULD_ZWaitPs      ].sName = "Wait        " ;
    Value[miULD_ZCmp][pvULD_ZPickFwdPs   ].sName = "PickFwd     " ;
    Value[miULD_ZCmp][pvULD_ZClampOnPs   ].sName = "ClampOn     " ;
    Value[miULD_ZCmp][pvULD_ZPickBwdPs   ].sName = "PickBwd     " ;
    Value[miULD_ZCmp][pvULD_ZWorkStartPs ].sName = "WorkStart   " ;
    Value[miULD_ZCmp][pvULD_ZPlaceFwdPs  ].sName = "PlaceFwd    " ;
    Value[miULD_ZCmp][pvULD_ZClampOffPs  ].sName = "ClampOff    " ;
    Value[miULD_ZCmp][pvULD_ZPlaceBwdPs  ].sName = "PlaceBwd    " ;

    LoadLastInfo();

    Load(m_sCrntDev);
}

void CPstnMan::Close(void)
{

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

double CPstnMan::SetPos(int _iAxisNo , EN_PSTN_VALUE _iPstnNo , double _dValue )
{
    if( _iAxisNo < 0 || _iAxisNo >= MAX_MOTR          ) {ShowMessageT("Motor Axis Range is wrong"    ); return Value[_iAxisNo][_iPstnNo].dValue ;}
    if( _iPstnNo < 0 || _iPstnNo >= PSTN_CNT[_iAxisNo]) {
        ShowMessageT("Position Index Range is wrong");
        return Value[_iAxisNo][_iPstnNo].dValue ;
    }

    if(_dValue > MT_GetMaxPos(_iAxisNo) || _dValue < MT_GetMinPos(_iAxisNo)) {
        ShowMessageT("Position Range is wrong(Check Position Limit Max Or Min)");
        return Value[_iAxisNo][_iPstnNo].dValue ;
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
        //Set Dir.                                                               u
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
                Value[m][i].sName  = m_pPstnList[m] -> Cells[0][i]           ;
                Value[m][i].dValue = m_pPstnList[m] -> Cells[1][i].ToDouble();
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
         if(GridInfo.iGrid == miLDR_YCmp) LDR.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miLDR_ZCmp) LDR.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miPRB_XCmp) PRB.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miPSB_XCmp) PSB.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRK_XCmp) WRK.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRK_ZTop) WRK.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRK_ZBtm) WRK.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRK_YBtm) WRK.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miSTL_YTop) STL.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miREJ_XBtm) STL.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miSTL_YBtm) STL.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miULD_ZCmp) ULD.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miULD_YCmp) ULD.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);

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
    m_pPstnList[_iAxisNo]-> Font -> Charset  = ARABIC_CHARSET ;
    m_pPstnList[_iAxisNo]-> Font -> Size     = 11             ;
    m_pPstnList[_iAxisNo]-> OnMouseDown      = pPstnListMouseDown ;
    m_pPstnList[_iAxisNo]-> OnMouseUp        = pPstnListMouseUp   ;
    m_pPstnList[_iAxisNo]-> OnDrawCell       = pPstnListDrawCell  ;

    m_pPstnList[_iAxisNo]-> RowCount         = PSTN_CNT[_iAxisNo] ;
    m_pPstnList[_iAxisNo]-> Visible          = true ;

    m_pPstnEdit[_iAxisNo]                = new TEdit(_pnBase);
    m_pPstnEdit[_iAxisNo]-> Parent       = _pnBase           ;
    m_pPstnEdit[_iAxisNo]-> ImeMode      = imDisable         ;
    m_pPstnEdit[_iAxisNo]-> NumberOnly   = true              ;
    m_pPstnEdit[_iAxisNo]-> UseApoint    = true              ;
    m_pPstnEdit[_iAxisNo]-> UseMinus     = true              ;
    m_pPstnEdit[_iAxisNo]-> UseComma     = false             ;
    m_pPstnEdit[_iAxisNo]-> Alignment    = taRightJustify    ;
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



