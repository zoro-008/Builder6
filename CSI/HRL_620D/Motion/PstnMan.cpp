
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PstnMan.h"
#include "UserINI.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "SMInterfaceUnit.h"

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Stage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define NAME_COL_WIDTH   80
#define POS_COL_WIDTH    90
#define GO_COL_WIDTH     60
#define INPUT_COL_WIDTH  60

CPstnMan PM;
//---------------------------------------------------------------------------
CPstnMan::CPstnMan()
{


}

CPstnMan::~CPstnMan()
{

}

void CPstnMan::Init()
{
    if(MAX_MOTR != MT_GetMotorCnt()) {ShowMessageT("Motor Count is wrong");}

    memset(&GridInfo , 0 , sizeof(SGridSelInfo)             );
    memset( Value    , 0 , sizeof(SValue)*MAX_MOTR*MAX_PSTN );

    Value[miWRK_VBrd1][pvWRK_VBrd1_Wait   ].sName = "Wait        " ;                             Value[miWRK_VBrd1][pvWRK_VBrd1_Wait ].iType = vtPosition ;                     
    Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk1   ].sName = "Position 1  " ; if(!OM.DevOptn.bUsePstU1 ){ Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk1 ].iType = vtHide ; }
    Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk2   ].sName = "Position 2  " ; if(!OM.DevOptn.bUsePstU2 ){ Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk2 ].iType = vtHide ; }
    Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk3   ].sName = "Position 3  " ; if(!OM.DevOptn.bUsePstU3 ){ Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk3 ].iType = vtHide ; }
    Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk4   ].sName = "Position 4  " ; if(!OM.DevOptn.bUsePstU4 ){ Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk4 ].iType = vtHide ; }
    Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk5   ].sName = "Position 5  " ; if(!OM.DevOptn.bUsePstU5 ){ Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk5 ].iType = vtHide ; }
    Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk6   ].sName = "Position 6  " ; if(!OM.DevOptn.bUsePstU6 ){ Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk6 ].iType = vtHide ; }
    Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk7   ].sName = "Position 7  " ; if(!OM.DevOptn.bUsePstU7 ){ Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk7 ].iType = vtHide ; }
    Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk8   ].sName = "Position 8  " ; if(!OM.DevOptn.bUsePstU8 ){ Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk8 ].iType = vtHide ; }
    Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk9   ].sName = "Position 9  " ; if(!OM.DevOptn.bUsePstU9 ){ Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk9 ].iType = vtHide ; }
    Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk10  ].sName = "Position 10 " ; if(!OM.DevOptn.bUsePstU10){ Value[miWRK_VBrd1][pvWRK_VBrd1_Wrk10].iType = vtHide ; }

    Value[miWRK_VBrd2][pvWRK_VBrd2_Wait   ].sName = "Wait        " ;
    Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk1   ].sName = "Position 1  " ; if(!OM.DevOptn.bUsePstU1 ){ Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk1 ].iType = vtHide ; }
    Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk2   ].sName = "Position 2  " ; if(!OM.DevOptn.bUsePstU2 ){ Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk2 ].iType = vtHide ; }
    Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk3   ].sName = "Position 3  " ; if(!OM.DevOptn.bUsePstU3 ){ Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk3 ].iType = vtHide ; }
    Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk4   ].sName = "Position 4  " ; if(!OM.DevOptn.bUsePstU4 ){ Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk4 ].iType = vtHide ; }
    Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk5   ].sName = "Position 5  " ; if(!OM.DevOptn.bUsePstU5 ){ Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk5 ].iType = vtHide ; }
    Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk6   ].sName = "Position 6  " ; if(!OM.DevOptn.bUsePstU6 ){ Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk6 ].iType = vtHide ; }
    Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk7   ].sName = "Position 7  " ; if(!OM.DevOptn.bUsePstU7 ){ Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk7 ].iType = vtHide ; }
    Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk8   ].sName = "Position 8  " ; if(!OM.DevOptn.bUsePstU8 ){ Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk8 ].iType = vtHide ; }
    Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk9   ].sName = "Position 9  " ; if(!OM.DevOptn.bUsePstU9 ){ Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk9 ].iType = vtHide ; }
    Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk10  ].sName = "Position 10 " ; if(!OM.DevOptn.bUsePstU10){ Value[miWRK_VBrd2][pvWRK_VBrd2_Wrk10].iType = vtHide ; }

    Value[miWRK_UBrd1][pvWRK_UBrd1_Wait   ].sName = "Wait        " ;
    Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk1   ].sName = "Position 1  " ; if(!OM.DevOptn.bUsePstU1 ){ Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk1 ].iType = vtHide ; }
    Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk2   ].sName = "Position 2  " ; if(!OM.DevOptn.bUsePstU2 ){ Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk2 ].iType = vtHide ; }
    Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk3   ].sName = "Position 3  " ; if(!OM.DevOptn.bUsePstU3 ){ Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk3 ].iType = vtHide ; }
    Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk4   ].sName = "Position 4  " ; if(!OM.DevOptn.bUsePstU4 ){ Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk4 ].iType = vtHide ; }
    Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk5   ].sName = "Position 5  " ; if(!OM.DevOptn.bUsePstU5 ){ Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk5 ].iType = vtHide ; }
    Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk6   ].sName = "Position 6  " ; if(!OM.DevOptn.bUsePstU6 ){ Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk6 ].iType = vtHide ; }
    Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk7   ].sName = "Position 7  " ; if(!OM.DevOptn.bUsePstU7 ){ Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk7 ].iType = vtHide ; }
    Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk8   ].sName = "Position 8  " ; if(!OM.DevOptn.bUsePstU8 ){ Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk8 ].iType = vtHide ; }
    Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk9   ].sName = "Position 9  " ; if(!OM.DevOptn.bUsePstU9 ){ Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk9 ].iType = vtHide ; }
    Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk10  ].sName = "Position 10 " ; if(!OM.DevOptn.bUsePstU10){ Value[miWRK_UBrd1][pvWRK_UBrd1_Wrk10].iType = vtHide ; }

    Value[miWRK_UBrd2][pvWRK_UBrd2_Wait   ].sName = "Wait        " ;
    Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk1   ].sName = "Position 1  " ; if(!OM.DevOptn.bUsePstU1 ){ Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk1 ].iType = vtHide ; }
    Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk2   ].sName = "Position 2  " ; if(!OM.DevOptn.bUsePstU2 ){ Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk2 ].iType = vtHide ; }
    Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk3   ].sName = "Position 3  " ; if(!OM.DevOptn.bUsePstU3 ){ Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk3 ].iType = vtHide ; }
    Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk4   ].sName = "Position 4  " ; if(!OM.DevOptn.bUsePstU4 ){ Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk4 ].iType = vtHide ; }
    Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk5   ].sName = "Position 5  " ; if(!OM.DevOptn.bUsePstU5 ){ Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk5 ].iType = vtHide ; }
    Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk6   ].sName = "Position 6  " ; if(!OM.DevOptn.bUsePstU6 ){ Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk6 ].iType = vtHide ; }
    Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk7   ].sName = "Position 7  " ; if(!OM.DevOptn.bUsePstU7 ){ Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk7 ].iType = vtHide ; }
    Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk8   ].sName = "Position 8  " ; if(!OM.DevOptn.bUsePstU8 ){ Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk8 ].iType = vtHide ; }
    Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk9   ].sName = "Position 9  " ; if(!OM.DevOptn.bUsePstU9 ){ Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk9 ].iType = vtHide ; }
    Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk10  ].sName = "Position 10 " ; if(!OM.DevOptn.bUsePstU10){ Value[miWRK_UBrd2][pvWRK_UBrd2_Wrk10].iType = vtHide ; }

    Value[miSTG_TBase][pvSTG_TBaseOrign   ].sName = "Original    " ;
    Value[miSTG_TBase][pvSTG_TBaseRvers   ].sName = "Reverse     " ;
    LoadLastInfo();


    Load(m_sCrntDev);

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

    if(_dValue > MT_GetMaxPos(_iAxisNo) || _dValue < MT_GetMinPos(_iAxisNo)) {
        ShowMessageT("Position Range is wrong(Check Position Limit Max Or Min)");
        return 0.0 ;
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
            UserINI.Save(sPath, (AnsiString)i+"_Pstn" , "iType " , Value[m][i].iType  );
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
        if( Value[iSel][ARow].iType != vtHide) {
            ACanvas->Brush -> Style = bsClear ;
            ACanvas->Pen   -> Color = clBlack ;

            TextRect = CellRect;
            TextRect.Left = TextRect.Left ;
            ACanvas->FillRect (CellRect)  ;
            ACanvas->Rectangle(CellRect)  ;
        }
        else {
            ACanvas->Brush -> Style = bsClear ;
            ACanvas->Brush -> Color = (TColor)RGB(220,220,220);
            ACanvas->FillRect (CellRect) ;
            TextRect = CellRect;
            TextRect.Right = TextRect.Right ;
        }
    }

    if( ACol == 2) {
         AnsiString CellText = "GO";

         unsigned char step = 30;
         unsigned char r = 0x80,g = 0x80,b = 0xC0;
         TColor clr_highlight = static_cast<TColor>( PALETTERGB(r+step,g+step,b+step) );
         TColor clr_shadow    = static_cast<TColor>( PALETTERGB(r-step,g-step,b-step) );

         TextRect = CellRect;
         TextRect.Left = TextRect.Left  + ((GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol ) ? 16 : 15) ;
         TextRect.Top  = TextRect.Top   + ((GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol ) ? 1  : 0 ) ;

         if(Value[iSel][ARow].iType == vtPosition || Value[iSel][ARow].iType == vtFixValue || Value[iSel][ARow].iType != vtHide) {
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

         if(Value[iSel][ARow].iType == vtPosition || Value[iSel][ARow].iType != vtHide) {
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
    if(1 == GridInfo.iCol && Value[(EN_MOTR_ID)GridInfo.iGrid][(EN_PSTN_ID)GridInfo.iRow].iType != vtFixValue
                          && Value[(EN_MOTR_ID)GridInfo.iGrid][(EN_PSTN_ID)GridInfo.iRow].iType != vtHide    ) {
        m_pPstnEdit[GridInfo.iGrid] -> Left    = GridInfo.iColX ;
        m_pPstnEdit[GridInfo.iGrid] -> Top     = GridInfo.iRowY + (int)GridInfo.iGrid;
        m_pPstnEdit[GridInfo.iGrid] -> Visible = true ;
        m_pPstnEdit[GridInfo.iGrid] -> SetFocus()     ;
    }

    if(2 == GridInfo.iCol ) {
         AnsiString sTemp ;
         sTemp = " going to goto " + Value[GridInfo.iGrid][GridInfo.iRow].sName + " Pos" ;
         Trace(MT_GetName(GridInfo.iGrid),sTemp.c_str());

         bool bOrigPstn    = (PM.GetValue(miSTG_TBase , pvSTG_TBaseOrign) - 1.0) <= MT_GetCmdPos(miSTG_TBase) &&
                             (MT_GetCmdPos(miSTG_TBase) <= PM.GetValue(miSTG_TBase , pvSTG_TBaseOrign) + 1.0) ;



         bool bRvrsPstn    = (PM.GetValue(miSTG_TBase , pvSTG_TBaseRvers )-1.0  <= MT_GetCmdPos(miSTG_TBase)) &&
                             (MT_GetCmdPos(miSTG_TBase) <= PM.GetValue(miSTG_TBase , pvSTG_TBaseRvers )+1.0 ) ;

         if(!IO_GetX(xSTG_TltDetect)){
             if(GridInfo.iGrid == miSTG_TBase) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
             if(GridInfo.iGrid == miSTG_TBase) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);

             if(GridInfo.iGrid == miWRK_UBrd1) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
             if(GridInfo.iGrid == miWRK_UBrd1) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
             if(GridInfo.iGrid == miWRK_UBrd1) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
             if(GridInfo.iGrid == miWRK_UBrd1) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
             if(GridInfo.iGrid == miWRK_UBrd1) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
             if(GridInfo.iGrid == miWRK_UBrd1) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
             if(GridInfo.iGrid == miWRK_UBrd1) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);

             if(GridInfo.iGrid == miWRK_VBrd1){
                 if( bRvrsPstn) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
                 if( bRvrsPstn) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
                 if( bRvrsPstn) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
                 if( bRvrsPstn) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
                 if( bRvrsPstn) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
                 if( bRvrsPstn) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
                 if( bRvrsPstn) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
                 if(!bRvrsPstn) FM_MsgOk("ERROR","V축이 움직일 수 있는 위치가 아닙니다.");
             }

             if(GridInfo.iGrid == miWRK_UBrd2) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
             if(GridInfo.iGrid == miWRK_UBrd2) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
             if(GridInfo.iGrid == miWRK_UBrd2) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
             if(GridInfo.iGrid == miWRK_UBrd2) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
             if(GridInfo.iGrid == miWRK_UBrd2) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
             if(GridInfo.iGrid == miWRK_UBrd2) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
             if(GridInfo.iGrid == miWRK_UBrd2) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);

             if(GridInfo.iGrid == miWRK_VBrd2){
                 if( bOrigPstn) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
                 if( bOrigPstn) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
                 if( bOrigPstn) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
                 if( bOrigPstn) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
                 if( bOrigPstn) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
                 if( bOrigPstn) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
                 if( bOrigPstn) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
                 if(!bOrigPstn) FM_MsgOk("ERROR","V축이 움직일 수 있는 위치가 아닙니다.");
             }
         }
         else { FM_MsgOk("WARNING" , "틸팅 센서가 감지 됩니다."); }
    }

    if(3 == GridInfo.iCol ) {
        Trace(MT_GetName(GridInfo.iGrid) , (m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow]+"->"+MT_GetCmdPos(GridInfo.iGrid)).c_str());
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

    const int iPosCelHeight = 32 ;
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
    m_pPstnList[_iAxisNo]-> Font -> Size     = 12             ;
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
        Trace(MT_GetName(GridInfo.iGrid) , (m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow]+"->"+m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble()).c_str());
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
    Trace(MT_GetName(GridInfo.iGrid), (m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow]+"->"+m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble()).c_str());
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



