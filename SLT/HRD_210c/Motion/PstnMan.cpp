
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PstnMan.h"
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff.h"
#include "RailEpx.h"
#include "Epoxy.h"
#include "RailDie.h"
#include "Head.h"
#include "Stage.h"
#include "PostBuff.h"
#include "UnLoader.h"
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

void CPstnMan::Init()
{
    if(MAX_MOTR != MT_GetMotorCnt()) {ShowMessageT("Motor Count is wrong");}

    memset(&GridInfo , 0 , sizeof(SGridSelInfo)             );
    memset( Value    , 0 , sizeof(SValue)*MAX_MOTR*MAX_PSTN );

    Value[miLDR_YCmp][pvLDR_YCmpWait      ].sName = "Wait          " ;

    Value[miLDR_YCmp][pvLDR_YCmpWorkStt   ].sName = "WorkStart     " ;

                                                                     
    Value[miLDR_ZCmp][pvLDR_ZCmpWait      ].sName = "Wait          " ;
    Value[miLDR_ZCmp][pvLDR_ZCmpIn        ].sName = "Pick          " ;
    Value[miLDR_ZCmp][pvLDR_ZCmpWorkStt   ].sName = "WorkStart     " ;
    Value[miLDR_ZCmp][pvLDR_ZCmpOut       ].sName = "Place         " ;

                                                                     
    Value[miPRB_XIdx][pvPRB_XIdxWait      ].sName = "Wait          " ;
    Value[miPRB_XIdx][pvPRB_XIdxWorkStt   ].sName = "WorkStart     " ;


    Value[miPRB_XIdx][pvPRB_XIdxOut       ].sName = "Out           " ;

                                                                     
    Value[miPRB_YIns][pvPRB_YInsWait      ].sName = "Wait          " ;
    Value[miPRB_YIns][pvPRB_YInsWorkStart ].sName = "WorkStart     " ;

                                                                     
    Value[miWRE_XIdx][pvWRE_XIdxWait      ].sName = "Wait          " ;
    Value[miWRE_XIdx][pvWRE_XIdxWorkStt   ].sName = "WorkStart     " ;
//    Value[miWRE_XIdx][pvWRE_XIdxPull      ].sName = "Pull        " ;
//    Value[miWRE_XIdx][pvWRE_XIdxBack      ].sName = "            " ;
    Value[miWRE_XIdx][pvWRE_XIdxOut       ].sName = "Out           " ;

                                                                     
    Value[miWRE_XEpx][pvWRE_XEpxWait      ].sName = "Wait          " ;
    Value[miWRE_XEpx][pvWRE_XEpxWorkStt   ].sName = "WorkStart     " ;
    Value[miWRE_XEpx][pvWRE_XEpxOri       ].sName = "Origin        " ;
    Value[miWRE_XEpx][pvWRE_XEpxClean     ].sName = "Clean         " ;

                                                                     
    Value[miWRE_YEpx][pvWRE_YEpxWait      ].sName = "Wait          " ;
    Value[miWRE_YEpx][pvWRE_YEpxWorkStt   ].sName = "WorkStart     " ;
    Value[miWRE_YEpx][pvWRE_YEpxOri       ].sName = "Origin        " ;
    Value[miWRE_YEpx][pvWRE_YEpxClean     ].sName = "Clean         " ;

                                                                     
    Value[miWRE_ZEpx][pvWRE_ZEpxWait      ].sName = "Wait          " ;
//    Value[miWRE_ZEpx][pvWRE_ZEpxOriStt    ].sName = "WorkStart     " ;
    Value[miWRE_ZEpx][pvWRE_ZEpxWork      ].sName = "Work          " ; //Value[miWRE_ZEpx][pvWRE_ZEpxWork   ].iType = vtFixValue ;
    Value[miWRE_ZEpx][pvWRE_ZEpxWorkAt    ].sName = "WorkAfter     " ;
    Value[miWRE_ZEpx][pvWRE_ZEpxCleanWait ].sName = "CleanMoveAble " ; Value[miWRE_ZEpx][pvWRE_ZEpxCleanWait ].iType = vtFixValue ;
    Value[miWRE_ZEpx][pvWRE_ZEpxClean     ].sName = "Clean         " ;


    Value[miWRD_XIdx][pvWRD_XIdxWait      ].sName = "Wait          " ;
    Value[miWRD_XIdx][pvWRD_XIdxWorkStt   ].sName = "WorkStart     " ;
//    Value[miWRD_XIdx][pvWRD_XIdxPull      ].sName = "            " ;
//    Value[miWRD_XIdx][pvWRD_XIdxBack      ].sName = "            " ;
    Value[miWRD_XIdx][pvWRD_XIdxOut       ].sName = "Out           " ;

                                                                     
    Value[miWRD_YIns][pvWRD_YInsWait      ].sName = "Wait          " ;
    Value[miWRD_YIns][pvWRD_YInsWorkStt   ].sName = "WorkStart     " ;



    Value[miWRD_XHed][pvWRD_XHedWait      ].sName = "Wait          " ;
    Value[miWRD_XHed][pvWRD_XHedWafer     ].sName = "WAFER         " ;
    Value[miWRD_XHed][pvWRD_XHedStrip     ].sName = "RAIL          " ;


    Value[miWRD_YHed][pvWRD_YHedWait      ].sName = "Wait          " ;
    Value[miWRD_YHed][pvWRD_YHedWafer     ].sName = "WAFER         " ;
    Value[miWRD_YHed][pvWRD_YHedWaferWait ].sName = "WAFER(Vision) " ; Value[miWRD_YHed][pvWRD_YHedWaferWait ].iType = vtFixValue ;
    Value[miWRD_YHed][pvWRD_YHedWaferZMove].sName = "WAFER(Z Move) " ; Value[miWRD_YHed][pvWRD_YHedWaferZMove].iType = vtFixValue ;
    Value[miWRD_YHed][pvWRD_YHedStripStt  ].sName = "RAIL Start    " ;
    Value[miWRD_YHed][pvWRD_YHedStripWait ].sName = "RAIL(Vision)  " ; Value[miWRD_YHed][pvWRD_YHedStripWait ].iType = vtFixValue ;
    Value[miWRD_YHed][pvWRD_YHedStripZMove].sName = "RAIL(Z Move)  " ; Value[miWRD_YHed][pvWRD_YHedStripZMove].iType = vtFixValue ;



    Value[miWRD_ZHed][pvWRD_ZHedWait      ].sName = "Wait           " ;
    Value[miWRD_ZHed][pvWRD_ZHedMove      ].sName = "HEDZMoveAble   " ;
    Value[miWRD_ZHed][pvWRD_ZHedWfWorkWait].sName = "WAFER WorkWait " ;
    Value[miWRD_ZHed][pvWRD_ZHedWfWork    ].sName = "WAFER Work     " ;
    Value[miWRD_ZHed][pvWRD_ZHedStWorkWait].sName = "RAIL WorkWait  " ;
    Value[miWRD_ZHed][pvWRD_ZHedStWork    ].sName = "RAIL Work      " ;
    Value[miWRD_ZHed][pvWRD_ZHedWaferCal  ].sName = "WAFER CAL      " ;
    Value[miWRD_ZHed][pvWRD_ZHedStripCal  ].sName = "BOAT CAL       " ;


    Value[miWRD_THed][pvWRD_THedWait      ].sName = "Wait          " ;
//    Value[miWRD_THed][pvWRD_THedWorkStt   ].sName = "WorkStart     " ;
    Value[miWRD_THed][pvWRD_THedWafer     ].sName = "WAFER         " ;
    Value[miWRD_THed][pvWRD_THedStrip     ].sName = "RAIL          " ;

                                                                     
                                                                     
    Value[miWRD_XStg][pvWRD_XStgWait      ].sName = "Wait          " ;
    Value[miWRD_XStg][pvWRD_XStgCassette  ].sName = "Cassette      " ;
    Value[miWRD_XStg][pvWRD_XStgAlign     ].sName = "AlignStart    " ;
    Value[miWRD_XStg][pvWRD_XStgFindOri   ].sName = "FindOrigin    " ; //Value[miWRD_XStg][poWRD_XStgWorkStt   ].iType = vtOffset ;
    Value[miWRD_XStg][poWRD_XStgFirstChip ].sName = "FirstOffset   " ; Value[miWRD_XStg][poWRD_XStgFirstChip   ].iType = vtOffset ;



    Value[miWRD_YStg][pvWRD_YStgWait      ].sName = "Wait          " ;
    Value[miWRD_YStg][pvWRD_YStgCassette  ].sName = "Cassette      " ;
    Value[miWRD_YStg][pvWRD_YStgAlign     ].sName = "AlignStart    " ;
    Value[miWRD_YStg][pvWRD_YStgFindOri   ].sName = "FindOrigin    " ; //Value[miWRD_YStg][poWRD_YStgWorkStt   ].iType = vtOffset ;
    Value[miWRD_YStg][poWRD_YStgFirstChip ].sName = "FirstOffset   " ; Value[miWRD_YStg][poWRD_YStgFirstChip   ].iType = vtOffset ;


    Value[miWRD_TStg][pvWRD_TStgWait      ].sName = "Wait          " ;
    Value[miWRD_TStg][pvWRD_TStgCassette  ].sName = "Cassette      " ;
    //Value[miWRD_TStg][pvWRD_TStgWorkStt   ].sName = "WorkStart     " ;
    Value[miWRD_TStg][pvWRD_TStgAlign     ].sName = "AlignStart    " ;



    Value[miWRD_ZEjt][pvWRD_ZEjtWait      ].sName = "Wait          " ;
    Value[miWRD_ZEjt][pvWRD_ZEjtWorkBefore].sName = "WorkBefore    " ;
    Value[miWRD_ZEjt][pvWRD_ZEjtWork      ].sName = "Work          " ;



    Value[miWRD_ZExd][pvWRD_ZExdWait      ].sName = "Wait          " ;
    Value[miWRD_ZExd][pvWRD_ZExdGet       ].sName = "Get           " ;
    Value[miWRD_ZExd][pvWRD_ZExdExpend    ].sName = "Expend        " ;


    Value[miWRD_ZCst][pvWRD_ZCstWait      ].sName = "Wait          " ;
    Value[miWRD_ZCst][pvWRD_ZCstWorkStt   ].sName = "WorkStart     " ;
    Value[miWRD_ZCst][poWRD_ZCstPlaceOfs  ].sName = "PlaceOfs      " ; Value[miWRD_ZCst][poWRD_ZCstPlaceOfs   ].iType = vtOffset ;


    Value[miWRD_XTrs][pvWRD_XTrsWait      ].sName = "Wait          " ;
    Value[miWRD_XTrs][pvWRD_XTrsPick      ].sName = "Pick          " ;
    Value[miWRD_XTrs][pvWRD_XTrsPickWait  ].sName = "PickWait      " ;
    Value[miWRD_XTrs][pvWRD_XTrsPlace     ].sName = "Place         " ;


    Value[miPSB_XIdx][pvPSB_XIdxWait      ].sName = "Wait          " ;
    Value[miPSB_XIdx][pvPSB_XIdxClamp     ].sName = "Clamp         " ;
    Value[miPSB_XIdx][pvPSB_XIdxPull      ].sName = "Pull          " ;
    Value[miPSB_XIdx][pvPSB_XIdxBack      ].sName = "Back          " ;
    Value[miPSB_XIdx][pvPSB_XIdxOut1      ].sName = "Out1          " ;
    Value[miPSB_XIdx][pvPSB_XIdxOut2      ].sName = "Out2          " ;

    Value[miULD_YCmp][pvULD_YCmpWait      ].sName = "Wait          " ;
    Value[miULD_YCmp][pvULD_YCmpWorkStt   ].sName = "WorkStart     " ;



    Value[miULD_ZCmp][pvULD_ZCmpWait      ].sName = "Wait          " ;
    Value[miULD_ZCmp][pvULD_ZCmpIn        ].sName = "Pick          " ;
    Value[miULD_ZCmp][pvULD_ZCmpWorkStt   ].sName = "WorkStart     " ;
    Value[miULD_ZCmp][pvULD_ZCmpOut       ].sName = "Place         " ;



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
         Trace(MT_GetName(GridInfo.iGrid),sTemp.c_str());
         if(GridInfo.iGrid == miLDR_YCmp) LDR.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miLDR_ZCmp) LDR.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miPRB_XIdx) PRB.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miPRB_YIns) PRB.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRE_XIdx) RAE.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRE_XEpx) EPX.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRE_YEpx) EPX.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRE_ZEpx) EPX.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRD_XIdx) RAD.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRD_YIns) RAD.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRD_XHed) HED.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRD_YHed) HED.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRD_ZHed) HED.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRD_THed) HED.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRD_XStg) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRD_YStg) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRD_TStg) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRD_ZEjt) HED.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRD_ZExd) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRD_ZCst) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miWRD_XTrs) STG.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miPSB_XIdx) PSB.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miULD_YCmp) ULD.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miULD_ZCmp) ULD.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);

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



