//---------------------------------------------------------------------------
#pragma hdrstop

//---------------------------------------------------------------------------

#include "Array.h"


#include "SLogUnit.h"
#include "UserINI.h"

//#include "ManualMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TColor clStat[MAX_CHIP_STAT];
CChip BUFF[MAX_ARRAY_ROW][MAX_ARRAY_COL]; //나중에 동적 생성으로 바꾼다.
/*
try {
EnterCriticalSection(&g_CSAray) ;
}
__finally{
    LeaveCriticalSection(&g_CSAray);
}

*/


bool __fastcall CArray::CallBackReg(CallbackAray _CallbackKey)
{
    CallAray = _CallbackKey ;
    return true ;
}
//---------------------------------------------------------------------------
__fastcall CArray::CArray(void)
{
    CallAray = NULL;
    
    //InitializeCriticalSection(&g_CSAray);
    static bool bFirst = true ;
    if(bFirst) {
        //Default Set.
        clStat[csRslt0  ] = clInactiveCaption ;//csInactiveCaption = 0  ,
        clStat[csRslt1  ] = clMaroon          ;//csMaroon          = 1  ,
        clStat[csRslt2  ] = clPurple          ;//csPurple          = 2  ,
        clStat[csRslt3  ] = clTeal            ;//csTeal            = 3  ,
        clStat[csRslt4  ] = clOlive           ;//csOlive           = 4  ,
        clStat[csRslt5  ] = clNavy            ;//csNavy            = 5  ,
        clStat[csRslt6  ] = clMoneyGreen      ;//csMoneyGreen      = 6  ,
        clStat[csRslt7  ] = clGreen           ;//csGreen           = 7  ,
        clStat[csRslt8  ] = clHighlight       ;//clHighlight       = 8  ,
        clStat[csRslt9  ] = clBlack           ;//csBlack           = 9  ,

        clStat[csNone   ] = clWhite           ;//csWhite           = 22 ,//스트립이나 카세트가 없는 상태.                                            >
        clStat[csMask   ] = clSkyBlue         ;//csSkyBlue         = 23 ,//PKG가 없지만 채크를 해놓은 상태.                                          >  요거 세개가 PKG 없는거.
        clStat[csEmpty  ] = clSilver          ;//csSilver          = 24 ,//카세트에 스트립이 없는 상태 혹은 더미패드에 토출자국이 없는경우. 프로브끝 >
        clStat[csUnkwn  ] = clAqua            ;//csAqua            = 25 ,//작업전 있는지 없는지 모르는 경우.
        clStat[csWork   ] = clBlue            ;//csBlue            = 26 ,// 작업 끝.
        clStat[csWait   ] = clYellow          ;//csYellow          = 27 ,
        clStat[csFail   ] = clFuchsia         ;//csYellow          = 28 ,
        clStat[csEror   ] = clRed             ;//csFuchsia         = 29 ,
        clStat[csGood   ] = clLime            ;//csRed             = 30 ,
        clStat[csToBuf  ] = clBlack           ;//clBlack           = 34 ,
        //clBlack           = 35 ,
        bFirst = false ;
    }
    m_sMenuName[csRslt0  ] = "RSLT_0"     ;
    m_sMenuName[csRslt1  ] = "RSLT_1"     ;
    m_sMenuName[csRslt2  ] = "RSLT_2"     ;
    m_sMenuName[csRslt3  ] = "RSLT_3"     ;
    m_sMenuName[csRslt4  ] = "RSLT_4"     ;
    m_sMenuName[csRslt5  ] = "RSLT_5"     ;
    m_sMenuName[csRslt6  ] = "RSLT_6"     ;
    m_sMenuName[csRslt7  ] = "RSLT_7"     ;
    m_sMenuName[csRslt8  ] = "RSLT_8"     ;
    m_sMenuName[csRslt9  ] = "RSLT_9"     ;
    m_sMenuName[csNone   ] = "NONE"      ;
    m_sMenuName[csMask   ] = "MASK"      ;
    m_sMenuName[csEmpty  ] = "EMPTY"     ;
    m_sMenuName[csUnkwn  ] = "UNKNOWN"   ;
    m_sMenuName[csWork   ] = "WORK"      ;
    m_sMenuName[csWait   ] = "WAIT"      ;
    m_sMenuName[csFail   ] = "FAIL"      ;
    m_sMenuName[csEror   ] = "ERROR"     ;
    m_sMenuName[csGood   ] = "GOOD"      ;
    m_sMenuName[csToBuf  ] = "ToBuf"     ;
    m_sMenuName[csFromBuf] = "FromBuf"   ;


    m_iMaxRow = MAX_ARRAY_ROW;
    m_iMaxCol = MAX_ARRAY_COL;

    pnParent = NULL ;
    imArray  = new TImage (Application -> MainForm);
    for( int i = 0 ; i < MAX_STATUS_POPUP_ID ; i++) {
        pmStatus[i] = new TPopupMenu(Application -> MainForm);
        pmStatus[i] -> OwnerDraw = true ;
    }

    //Graphics::TBitmap* Bitmap = new Graphics::TBitmap(Application);
    //
    //Bitmap -> Canvas->Pen  ->Color = clBlack ;
    //Bitmap -> Canvas->Pen  ->Style = psSolid ;
    //Bitmap -> Canvas->Pen  ->Width = 1;

    for( int i = 0 ; i < MAX_STATUS_POPUP_ID ; i++) {
        for( int j = 0 ; j < MAX_CHIP_STAT ; j++) {
            miStatus[i][j] = new TMenuItem(Application -> MainForm);
            miStatus[i][j] -> OnDrawItem = MenuDrawItem ;

            switch(i) {
                default     : miStatus[i][j]-> Caption = "ERR_" + m_sMenuName[j] ;                                                                      break ;
                case spOne  : miStatus[i][j]-> Caption = "ONE_" + m_sMenuName[j] ; miStatus[i][j]-> Tag = spOne ; miStatus[i][j]-> OnClick = OneClick ; break ;
                case spCol  : miStatus[i][j]-> Caption = "COL_" + m_sMenuName[j] ; miStatus[i][j]-> Tag = spCol ; miStatus[i][j]-> OnClick = ColClick ; break ;
                case spRow  : miStatus[i][j]-> Caption = "ROW_" + m_sMenuName[j] ; miStatus[i][j]-> Tag = spRow ; miStatus[i][j]-> OnClick = RowClick ; break ;
                case spAll  : miStatus[i][j]-> Caption = "ALL_" + m_sMenuName[j] ; miStatus[i][j]-> Tag = spAll ; miStatus[i][j]-> OnClick = AllClick ; break ;
                case spDrag : miStatus[i][j]-> Caption = "DRG_" + m_sMenuName[j] ; miStatus[i][j]-> Tag = spDrag; miStatus[i][j]-> OnClick = DragClick; break ;
            }

           // miStatus[i][j]-> Bitmap -> Canvas->Brush->Color = clStat[j];
           // miStatus[i][j]-> Bitmap -> Canvas->Rectangle(0, 0, 50, 50);
           //
           // miStatus[i][j] -> Setbit
           //

            miStatus[i][j]-> Tag  = j;
            pmStatus[i]   -> Items -> Insert(j  , miStatus[i][j]);
            miStatus[i][j]-> AutoHotkeys = false ;
        }

    }

    imArray -> OnMouseDown = ImageMouseDown ;
    imArray -> OnMouseMove = ImageMouseMove ;
    imArray -> OnMouseUp   = ImageMouseUp   ;

}
//---------------------------------------------------------------------------
__fastcall CArray::~CArray(void)
{
    Trace( "", "~CArray") ;    //14
   // DeleteCriticalSection(&g_CSAray);
/*    delete imArray ;
    for( int i = 0 ; i < MAX_STATUS_POPUP_ID ; i++) {
        delete pmStatus[i] ;
    }

    for( int i = 0 ; i < MAX_STATUS_POPUP_ID ; i++) {
        for( int j = 0 ; j < MAX_CHIP_STAT ; j++) {
            delete miStatus[i][j] ;
        }
    }
*/
}

void __fastcall CArray::PopupMenuEnable(bool bState)
{
    for(int i = 0 ; i < MAX_STATUS_POPUP_ID ; i++ ) {
        for(int j = 0 ; j < MAX_CHIP_STAT ; j++ ) {
            miStatus[i][j] -> Enabled = bState ;
        }
    }
}

void __fastcall CArray::ClearDispItem()
{
    for(int i = 0 ; i < MAX_STATUS_POPUP_ID ; i++ ) {
        for(int j = 0 ; j < MAX_CHIP_STAT ; j++ ) {
            miStatus[i][j] -> Visible = false ;
        }
    }
}

void __fastcall CArray::SetDispItem(EN_CHIP_STAT _iStat , AnsiString _sSubCaption )
{
    bool isAllVisible = true ;
    for(int i = 0 ; i < MAX_STATUS_POPUP_ID ; i++ ) {
        for(int j = 0 ; j < MAX_CHIP_STAT ; j++ ) {
            if(!miStatus[i][j] -> Visible) {
                isAllVisible = false ;
                break ;
            }
        }
        if(isAllVisible) break ;
    }

    if(isAllVisible) ClearDispItem();

    for(int i = 0 ; i < MAX_STATUS_POPUP_ID ; i++ ) {
        miStatus[i][_iStat] -> Visible = true ;
        if((EN_CHIP_STAT)i != spAll && (_iStat == csToBuf || _iStat == csFromBuf) ) miStatus[i][_iStat] -> Visible = false ;
        //miStatus[i][_iStat] = new TMenuItem(Application);
        m_sMenuName[_iStat] = _sSubCaption ;
        miStatus[i][_iStat]->AutoHotkeys = maManual ; //동작 안함....
        pmStatus[i]        ->AutoHotkeys = maManual ;

        switch(i) {                                       //글자수 바꾸지 말기.  MenuDrawItem에서 4글자 앞에 트림함.
            default     : miStatus[i][_iStat]-> Caption = "ERR_" + m_sMenuName[_iStat] ; break ;
            case spOne  : miStatus[i][_iStat]-> Caption = "ONE_" + m_sMenuName[_iStat] ; break ;
            case spCol  : miStatus[i][_iStat]-> Caption = "COL_" + m_sMenuName[_iStat] ; break ;
            case spRow  : miStatus[i][_iStat]-> Caption = "ROW_" + m_sMenuName[_iStat] ; break ;
            case spAll  : miStatus[i][_iStat]-> Caption = "ALL_" + m_sMenuName[_iStat] ; break ;
            case spDrag : miStatus[i][_iStat]-> Caption = "DRG_" + m_sMenuName[_iStat] ; break ;
        }
    }
}

void __fastcall CArray::SetDispColor(EN_CHIP_STAT _iStat , TColor _cColor )
{
    clStat[_iStat] = _cColor ;
}

//---------------------------------------------------------------------------
void __fastcall CArray::MenuDrawItem(TObject *Sender,TCanvas *ACanvas,const TRect &ARect, bool Selected)
{
//try {
//        EnterCriticalSection(&g_CSAray) ;

        ACanvas->Lock();

        if(Selected) {      //나중에 뻑보기.
            ACanvas->Brush->Color = (TColor)0x339900;
            ACanvas->Font ->Color = clWhite;
        }
        else {
            ACanvas->Brush->Color = (TColor)0x99FFCC;
            ACanvas->Font ->Color = clBlack;
        }

        AnsiString sCaption = ((TMenuItem *)Sender)->Caption ;
        AnsiString sTemp , sTemp1   ;

        //for(int i = 0 ; i < ((TMenuItem *)Sender)->Caption.

        ACanvas->TextRect(ARect, ARect.Left + 6, ARect.Top + 3, "   "+sCaption);


        //TColor     clStat     [MAX_CHIP_STAT]
        //AnsiString m_sMenuName[MAX_CHIP_STAT]
        sTemp  = sCaption.Delete(1,4) ;
        for(int i = 0 ; i < MAX_CHIP_STAT ; i++ ) { ///잠시만...  뻑나면 Sender NULL 확인 하기.
            sTemp1 = m_sMenuName[i] ;
            if(sTemp1 == sTemp && sTemp != "") {
                ACanvas -> Brush -> Color = clStat[i] ;
                break ;
            }
        }
        ACanvas->Rectangle(1,ARect.top + 1,15,ARect.top + 15) ;


        ACanvas->Unlock();
//}
//__finally{
//    LeaveCriticalSection(&g_CSAray);
//}


}

//---------------------------------------------------------------------------
void __fastcall CArray::OneClick(TObject *Sender)
{
    //Local Var.
    int Tag = ((TMenuItem *)Sender) -> Tag;

    AnsiString sTemp ;
    sTemp.sprintf("%02d,%s",Tag,"OneClicked");
    Trace( "Array(Chip Stats)", sTemp.c_str()) ;

    //Local Var.
    SetStat(ClickStat.iSelR2,ClickStat.iSelC2 ,(EN_CHIP_STAT)Tag);
    if(CheckAllStat(csNone)) m_iStep = 0 ;

    ClickStat.iSelX1 = ClickStat.iSelX2 = ClickStat.iSelY1 = ClickStat.iSelY2 = 0 ;

    UpdateAray();
}
//---------------------------------------------------------------------------
void __fastcall CArray::ColClick(TObject *Sender)
{
    //Local Var.
    int Tag = ((TMenuItem *)Sender) -> Tag;

    AnsiString sTemp ;
    sTemp.sprintf("%02d,%s",Tag,"ColClicked");
    Trace( "Array(Chip Stats)", sTemp.c_str()) ;

    for(int r = 0 ; r < GetMaxRow() ; r++)
        SetStat(r,ClickStat.iSelC2,(EN_CHIP_STAT)Tag);
    if(CheckAllStat(csNone)) m_iStep = 0 ;
    ClickStat.iSelX1 = ClickStat.iSelX2 = ClickStat.iSelY1 = ClickStat.iSelY2 = 0 ;

    UpdateAray();

}
//---------------------------------------------------------------------------
void __fastcall CArray::RowClick(TObject *Sender)
{
    //Local Var.
    int Tag = ((TMenuItem *)Sender) -> Tag;

    AnsiString sTemp ;
    sTemp.sprintf("%02d,%s",Tag,"RowClicked");
    Trace( "Array(Chip Stats)", sTemp.c_str()) ;

    //Get RC.
    for(int c = 0 ; c < GetMaxCol() ; c++) SetStat(ClickStat.iSelR2,c,(EN_CHIP_STAT)Tag);
    if(CheckAllStat(csNone)) m_iStep = 0 ;

    ClickStat.iSelX1 = ClickStat.iSelX2 = ClickStat.iSelY1 = ClickStat.iSelY2 = 0 ;

    UpdateAray();
}
//---------------------------------------------------------------------------
void __fastcall CArray::AllClick(TObject *Sender)
{
    //Local Var.
    int Tag = ((TMenuItem *)Sender) -> Tag;

    AnsiString sTemp ;
    sTemp.sprintf("%02d,%s",Tag,"AllClicked");
    Trace( "Array(Chip Stats)", sTemp.c_str()) ;

    if(Tag == csToBuf) {
        memcpy(BUFF , CHPS , sizeof(CChip) * MAX_ARRAY_COL * MAX_ARRAY_ROW );
        return ;
    }

    if(Tag == csFromBuf) {
        memcpy(CHPS , BUFF , sizeof(CChip) * MAX_ARRAY_COL * MAX_ARRAY_ROW );
        return ;
    }

    //Get RC.
    SetStat((EN_CHIP_STAT)Tag);
    if(CheckAllStat(csNone)) m_iStep = 0 ;
    ClickStat.iSelX1 = ClickStat.iSelX2 = ClickStat.iSelY1 = ClickStat.iSelY2 = 0 ;

    UpdateAray();
}
//---------------------------------------------------------------------------
void __fastcall CArray::DragClick(TObject *Sender)
{
    //Local Var.
    int Tag = ((TMenuItem *)Sender) -> Tag;
    int iRow1 , iCol1;
    int iRow2 , iCol2;

    int r1,r2,c1,c2;
    if( ClickStat.iSelR1 < ClickStat.iSelR2 ) { r1 = ClickStat.iSelR1 ; r2 = ClickStat.iSelR2; }
    else                                      { r1 = ClickStat.iSelR2 ; r2 = ClickStat.iSelR1; }
    if( ClickStat.iSelC1 < ClickStat.iSelC2 ) { c1 = ClickStat.iSelC1 ; c2 = ClickStat.iSelC2; }
    else                                      { c1 = ClickStat.iSelC2 ; c2 = ClickStat.iSelC1; }


    if(r1 < 0 ) r1 = 0;
    if(c1 < 0 ) c1 = 0;


    for(int c = c1 ; c <= c2 ; c++)
        for(int r = r1 ; r <= r2 ; r++)
            SetStat(r,c,(EN_CHIP_STAT)Tag);

    if(CheckAllStat(csNone)) m_iStep = 0 ;

    AnsiString sTemp ;
    sTemp.sprintf("%02d,%s",Tag,"DragClicked");
    Trace( "Array(Chip Stats)", sTemp.c_str());

    ClickStat.iSelX1 = ClickStat.iSelX2 = ClickStat.iSelY1 = ClickStat.iSelY2 = 0 ;

    UpdateAray();
}

//---------------------------------------------------------------------------
void __fastcall CArray::ImageMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    //To remove error.
    ClickStat.bMouseDown = true;

    imArray ->Canvas->MoveTo(X, Y);
    ClickStat.iSelX1 = X ;
    ClickStat.iSelY1 = Y ;
    ClickStat.iSelX2 = X ;
    ClickStat.iSelY2 = Y ;

    if(m_bSameCellSize) {
        ImageArayRCFix(ClickStat.iSelX1 , ClickStat.iSelY1 , ClickStat.iSelR1 , ClickStat.iSelC1 );
        ImageArayRCFix(ClickStat.iSelX2 , ClickStat.iSelY2 , ClickStat.iSelR2 , ClickStat.iSelC2 );
    }
    else {
        ImageArayRC(ClickStat.iSelX1 , ClickStat.iSelY1 , ClickStat.iSelR1 , ClickStat.iSelC1 );
        ImageArayRC(ClickStat.iSelX2 , ClickStat.iSelY2 , ClickStat.iSelR2 , ClickStat.iSelC2 );
    }
    UpdateAray() ;

}

//---------------------------------------------------------------------------
void __fastcall CArray::ImageMouseMove(TObject *Sender,TShiftState Shift, int X, int Y)
{
    if(!ClickStat.bMouseDown) return ;
    ClickStat.iSelX2 = X ;
    ClickStat.iSelY2 = Y ;

    if(m_bSameCellSize) {
        ImageArayRCFix(ClickStat.iSelX2 , ClickStat.iSelY2 , ClickStat.iSelR2 , ClickStat.iSelC2 );
    }
    else {
        ImageArayRC(ClickStat.iSelX2 , ClickStat.iSelY2 , ClickStat.iSelR2 , ClickStat.iSelC2 );
    }

    UpdateAray();
}

//---------------------------------------------------------------------------
void __fastcall CArray::ImageMouseUp(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(!ClickStat.bMouseDown) return ;
    ClickStat.bMouseDown = false ;

    POINT lpPoint ;

    GetCursorPos(&lpPoint);

    if(ClickStat.iSelR1 == ClickStat.iSelR2 && ClickStat.iSelC1 == ClickStat.iSelC2){
        if(Button == mbRight) {
            //m_iRtUpX = ClickStat.iSelR1 ;
            //m_iRtUpY = ClickStat.iSelC1 ;
            //m_iRtUpS = CHPS[m_iRtUpX][m_iRtUpY].GetStat();
            if(CallAray)CallAray(ClickStat.iSelC1 , ClickStat.iSelR1) ;

//            MM.SetManCycle(mcSTG_ClickMove);

//            miStatusSub[0] -> Caption = "Row_" + (String)iR +"   " ;
//            miStatusSub[1] -> Caption = "Col_" + (String)iC +"   " ;
//            miStatusSub[2] -> Caption = m_sMenuName[iStats]   +"   " ;

//            pmStatusSub ->Popup(lpPoint.x + 5 , lpPoint.y + 5) ;

        }
        else {

            if(Shift.Contains(ssCtrl)&&Shift.Contains(ssShift)) {
                pmStatus[spAll] -> Popup(lpPoint.x + 5 , lpPoint.y + 5) ;
            }

            else if(Shift.Contains(ssCtrl))                     {
                pmStatus[spCol] -> Popup(lpPoint.x + 5 , lpPoint.y + 5) ;
            }

            else if(Shift.Contains(ssShift))                    {
                pmStatus[spRow] -> Popup(lpPoint.x + 5 , lpPoint.y + 5) ;
            }

            else                                                {
                pmStatus[spOne] -> Popup(lpPoint.x + 5 , lpPoint.y + 5) ;
            }
        }
    }

    else{
        pmStatus[spDrag] -> Popup(lpPoint.x + 5 , lpPoint.y + 5) ;
    }

}

void __fastcall CArray::ImageArayRC(int X , int Y , int &R , int &C)
{
//    try {
//    EnterCriticalSection(&g_CSAray) ;

//        imArray->Canvas->Lock();
        const int iDire = 0 ; //나중에 쓸일있으면 만들자.
        //Local Var.
        TColor     sColor;
        AnsiString str = "";
        int        uRow , uCol;
        int        iLeft;

        //Set Disp Dir.
        if(iDire == 0 || iDire == 2) {
            uRow = GetMaxRow() ;
            uCol = GetMaxCol() ;
        }
        else {
            uRow = GetMaxCol() ;
            uCol = GetMaxRow() ;
        }
        //Get Row, Col.
        double ImageW = (double)imArray -> Width ;
        double ImageH = (double)imArray -> Height;
        double uGw    = ImageW / (double)uCol  ;
        double uGh    = ImageH / (double)uRow  ;
        int    Urow   = (int)((double)Y / uGh) ;
        int    Ucol   = (int)((double)X / uGw) ;
        int    rRow, rCol;

        //Set Row Col.
        if      (iDire == 0) { rRow = Urow;              rCol = Ucol;              } // 0
        else if (iDire == 1) { rRow = Ucol;              rCol = (uRow - 1) - Urow; } // 90
        else if (iDire == 2) { rRow = (uRow - 1) - Urow; rCol = (uCol - 1) - Ucol; } // 180
        else if (iDire == 3) { rRow = (uCol - 1) - Ucol; rCol = Urow;              } // 270
        else if (iDire == 4) { rRow = (uCol - 1) - Ucol; rCol = (uRow - 1) - Urow; } // 270 + vert. mirror.
        else if (iDire == 5) { rRow = Urow;              rCol = (uCol - 1) - Ucol; } // Horz. Mirrror.

        //return.
        R = rRow;
        C = rCol;
//        imArray->Canvas->Unlock();
//    }
//    __finally{
//        LeaveCriticalSection(&g_CSAray);
//    }

}

void __fastcall CArray::ImageArayRCFix(int X , int Y , int &R , int &C)
{
//    try {
//        EnterCriticalSection(&g_CSAray) ;
//        imArray->Canvas->Lock();

        const int iDire = 0 ; //나중에 쓸일있으면 만들자.

        //Local Var.
        AnsiString str = "";
        int        uRow , uCol;

        //Set Disp Dir.
        if(iDire == 0 || iDire == 2) {
            uRow = GetMaxRow() ;
            uCol = GetMaxCol() ;
        }
        else  {
            uRow = GetMaxCol() ;
            uCol = GetMaxRow() ;
        }


        //Get Row, Col.

        int    iWidth   = imArray ->Width  / uCol  ;
        int    iHeight  = imArray ->Height / uRow  ;
        int    iXOffset = (imArray ->Width  - iWidth  * uCol)/2 ;
        int    iYOffset = (imArray ->Height - iHeight * uRow)/2 ;
        double ImageW   = (double)iWidth  * uCol ;
        double ImageH   = (double)iHeight * uRow ;
        int    Urow     = (int)((double)(Y - iYOffset) / iHeight)  ;
        int    Ucol     = (int)((double)(X - iXOffset) / iWidth )  ;

        int    rRow, rCol;

        //Set Row Col.
        if      (iDire == 0) { rRow = Urow;              rCol = Ucol;              } // 0
        else if (iDire == 1) { rRow = Ucol;              rCol = (uRow - 1) - Urow; } // 90
        else if (iDire == 2) { rRow = (uRow - 1) - Urow; rCol = (uCol - 1) - Ucol; } // 180
        else if (iDire == 3) { rRow = (uCol - 1) - Ucol; rCol = Urow;              } // 270
        else if (iDire == 4) { rRow = (uCol - 1) - Ucol; rCol = (uRow - 1) - Urow; } // 270 + vert. mirror.
        else if (iDire == 5) { rRow = Urow;              rCol = (uCol - 1) - Ucol; } // Horz. Mirrror.

        //return.
        R = rRow;
        C = rCol;
//        imArray->Canvas->Unlock();
//    }
//    __finally{
//        LeaveCriticalSection(&g_CSAray);
//    }

}

void __fastcall CArray::UpdateAray()
{
    if(pnParent == NULL) return ;
    if(imArray  == NULL) return ;

    if(m_bSameCellSize) UpdateArayFix() ;
    else                UpdateArayNrm() ;

}

void __fastcall CArray::UpdateArayNrm()
{
//    try {
//    EnterCriticalSection(&g_CSAray) ;
        //To remove error.
        const int iDire = 0 ;

        //Local Var.
        TColor sColor;
        char   str[10] = { 0 , };
        int    uRow , uCol;
        int    iLeft;

        //Set Disp Dir.
        uRow = GetMaxRow();
        uCol = GetMaxCol();

        if(!uRow) uRow = 1 ;
        if(!uCol) uCol = 1 ;

        int uX1, uX2, uY1, uY2;
        double uGw   = (double)imArray -> Width  / (double)uCol;
        double uGh   = (double)imArray -> Height / (double)uRow;
        int    iWOff = (int)((double)(imArray -> Width  - uGw * uCol) / 2.0);
        int    iHOff = (int)((double)(imArray -> Height - uGh * uRow) / 2.0);
        int    sRow, sCol, rRow;

        //Sleep(1);
        imArray -> Canvas -> Lock();

        imArray -> Canvas -> Pen  -> Color = TColor(clBlack);
        imArray -> Canvas -> Pen  -> Style = psSolid        ;

        if(CheckAllStat(csNone)) {
            sColor = clStat[csNone] ;

            //Drawing.
            sprintf(str , "NONE");
            imArray -> Canvas -> Brush -> Color = TColor(sColor );
            //imArray -> Canvas -> Rectangle(uX1 - 1 , uY1 - 1 , uX2 + 1 , uY2 + 1);
            imArray -> Canvas -> Rectangle(0 , 0 , imArray -> Width , imArray -> Height);
            //View Chip Info.

            //자꾸 뻑남.... 확인 필요.
            //imArray -> Canvas -> Font -> Name = "Times New Roman";
            //imArray -> Canvas -> Font -> Size = 12;
            //imArray -> Canvas -> TextOut(imArray -> Width / 2 - 24 , imArray -> Height / 2 - 12 , str);
        }
        else {
            imArray -> Canvas -> Font -> Name  = "Small Fonts"  ;
            for (int r = 0 ; r < uRow ; r++) {
                for (int c = 0 ; c < uCol ; c++) {
                    switch (iDire) {
                        case 0: sRow = r;              sCol = c;              break; // 0
                        case 1: sRow = c;              sCol = (uRow - 1) - r; break; // 90
                        case 2: sRow = (uRow - 1) - r; sCol = (uCol - 1) - c; break; // 180
                        case 3: sRow = (uCol - 1) - c; sCol = r;              break; // 270
                        case 4: sRow = (uCol - 1) - c; sCol = (uRow - 1) - r; break; // 270 + Vert. Mirror.
                        case 5: sRow = r;              sCol = (uCol - 1) - c; break; // Horz. Mirrror.
                    }

                    uX1 = iWOff + c * uGw       + 2;
                    uX2 = iWOff + c * uGw + uGw - 2;
                    uY1 = iHOff + r * uGh       + 2;
                    uY2 = iHOff + r * uGh + uGh - 2;

                    sColor = clStat[GetStat(r,c)] ;

                    //Drawing.
                    sprintf(str , "%d" , sRow * uCol + sCol + 1);
                    imArray -> Canvas -> Brush -> Color = TColor(sColor );
                    //imArray -> Canvas -> Rectangle(uX1 - 1 , uY1 - 1 , uX2 + 1 , uY2 + 1);
                    imArray -> Canvas -> Rectangle(uX1 -2 , uY1 -2 , uX2 +2 , uY2 +2);
                    //View Chip Info.
                    imArray -> Canvas -> Font -> Size = 6;
                    //imArray -> Canvas -> TextOut(uX1 , uY1 , str);
                }
            }
        }

        if(ClickStat.iSelX1 != ClickStat.iSelX2 || ClickStat.iSelY1 != ClickStat.iSelY2) {
            imArray->Canvas->Brush->Style = bsClear;
            imArray->Canvas->Pen->Color = clBlack;
            imArray->Canvas->Pen->Style = psDot ;
            imArray->Canvas->Pen->Width = 1;
            imArray->Canvas->Rectangle(ClickStat.iSelX1, ClickStat.iSelY1, ClickStat.iSelX2, ClickStat.iSelY2);
        }

        imArray -> Canvas -> Unlock();
//    }
//    __finally{
//        LeaveCriticalSection(&g_CSAray);
//    }

}

void __fastcall CArray::UpdateArayFix()
{
//    try {
//        EnterCriticalSection(&g_CSAray) ;

        //To remove error.
        if (imArray == NULL) return;

        const int iDire = 0 ;

        //init
        int  uRow    = 0 ;
        int  uCol    = 0 ;
        int  iWidth  = 0 ;
        int  iHeight = 0 ;

        int  iXOffset = 0 ;
        int  iYOffset = 0 ;

        int  sRow, sCol  ;
        TColor sColor ;

        //Set Disp Dir.
        if(iDire == 0 || iDire == 2) {
            uRow = GetMaxRow() ;
            uCol = GetMaxCol() ;
        }
        else {
            uRow = GetMaxCol() ;
            uCol = GetMaxRow() ;
        }

        if(uRow < 1) uRow = 1 ;
        if(uCol < 1) uCol = 1 ;

    //    int  a = imArray -> Width ;
    //    a = imArray ->Width  / uCol ;
    //    a = imArray ->Height ;

        //iWidth  = imArray ->Width  / uCol - 1;
        //iHeight = imArray ->Height / uRow - 1;

        iWidth  = imArray ->Width  / uCol ;
        iHeight = imArray ->Height / uRow ;

        iXOffset = (imArray ->Width  - iWidth  * uCol)/2 ;
        iYOffset = (imArray ->Height - iHeight * uRow)/2 ;

        imArray -> Canvas -> Lock();

        TRect rect ;
        rect.left = 0 ;
        rect.top  = 0 ;
        rect.right  = imArray ->Width   ;
        rect.bottom = imArray ->Height  ;

        imArray -> Canvas -> FillRect(rect); //Clear

        for (int r = 0 ; r < uRow; r++) {
            for (int c = 0 ; c < uCol ; c++) {
                imArray -> Canvas -> Pen  -> Color = clBtnFace      ;
                imArray -> Canvas -> Pen  -> Style = psSolid        ;
                switch (iDire) {
                    case 0: sRow = r;              sCol = c;              break; // 0
                    case 1: sRow = c;              sCol = (uRow - 1) - r; break; // 90
                    case 2: sRow = (uRow - 1) - r; sCol = (uCol - 1) - c; break; // 180
                    case 3: sRow = (uCol - 1) - c; sCol = r;              break; // 270
                    case 4: sRow = (uCol - 1) - c; sCol = (uRow - 1) - r; break; // 270 + Vert. Mirror.
                    case 5: sRow = r;              sCol = (uCol - 1) - c; break; // Horz. Mirrror.
                }

                sColor = clStat[GetStat(r,c)] ;

                imArray->Canvas->Pen  ->Mode  = pmCopy ;
                imArray->Canvas->Pen  ->Style = psSolid;
                imArray->Canvas->Pen  ->Width = 1;
                imArray->Canvas->Brush->Color = sColor ;
                imArray->Canvas->Rectangle(iXOffset + (sCol * iWidth           ), iYOffset + (sRow * iHeight            ),
                                         iXOffset + (sCol * iWidth + iWidth +1 ), iYOffset + (sRow * iHeight + iHeight +1 ));
                if(r == uRow - 1 && !(c%10) && c){
                    imArray->Canvas->Pen  ->Color = clBlue  ;
                    imArray->Canvas->Brush->Color = clWhite ;
                    imArray->Canvas->MoveTo(c*iWidth + iXOffset ,    iYOffset               ) ;
                    imArray->Canvas->LineTo(c*iWidth + iXOffset , imArray ->Height - iYOffset ) ;
                    imArray->Canvas->Pen  ->Width = 1;
                    imArray -> Canvas -> Font -> Name  = "Small Fonts"  ;
                    imArray -> Canvas -> Font -> Size  = 7;
                    imArray->Canvas->TextOutA(c*iWidth - 1 + iXOffset ,                  iYOffset - 12 , (AnsiString)c ) ;
                    imArray->Canvas->TextOutA(c*iWidth - 1 + iXOffset , imArray ->Height - iYOffset + 1  , (AnsiString)c ) ;

                }

            }


            if(!(r%10)&& r) {
                imArray->Canvas->Pen  ->Color = clBlue ;
                imArray->Canvas->Brush->Color = clWhite ;
                imArray->Canvas->MoveTo(iXOffset                , r * iHeight + iYOffset) ;
                imArray->Canvas->LineTo(imArray ->Width - iXOffset, r * iHeight + iYOffset) ;
                imArray->Canvas->Pen  ->Width = 1;
                imArray -> Canvas -> Font -> Name  = "Small Fonts"  ;
                imArray -> Canvas -> Font -> Size  = 7;
                imArray -> Canvas ->TextOut(                  iXOffset - 12 , r * iHeight - 1 + iYOffset , (AnsiString)r) ;
                imArray -> Canvas ->TextOut(imArray ->Width - iXOffset + 1  , r * iHeight - 1 + iYOffset , (AnsiString)r) ;
            }
        }

        if(ClickStat.iSelX1 != ClickStat.iSelX2 || ClickStat.iSelY1 != ClickStat.iSelY2) {
            imArray->Canvas->Brush->Style = bsClear;
            imArray->Canvas->Pen->Color = clBlack;
            imArray->Canvas->Pen->Style = psDot ;
            imArray->Canvas->Pen->Width = 1;
            imArray->Canvas->Rectangle(ClickStat.iSelX1, ClickStat.iSelY1, ClickStat.iSelX2, ClickStat.iSelY2);
        }

        imArray -> Canvas -> Unlock();

//    }
//    __finally{
//        LeaveCriticalSection(&g_CSAray);
//    }


}

void __fastcall CArray::SetParent(TPanel * _pnBase)
{
    if(_pnBase == NULL) return ;
    pnParent = _pnBase ;
    imArray -> Parent = _pnBase  ;
    //imArray -> Align  = alClient ;

    imArray -> Picture -> Bitmap -> Width  = _pnBase -> Width  ;
    imArray -> Picture -> Bitmap -> Height = _pnBase -> Height ;

    imArray -> Width  = _pnBase -> Width  ;
    imArray -> Height = _pnBase -> Height ;

    _pnBase -> DoubleBuffered = true ;

    //UpdateAray();//sun


}

void __fastcall CArray::SetConfig(bool _bSameCellSize , AnsiString _sName)
{
    m_bSameCellSize = _bSameCellSize ;
    m_sName         = _sName         ;
    //UpdateAray();//sun
}

void __fastcall CArray::SetID(AnsiString _sID)
{
    m_sID = _sID ;
}

AnsiString __fastcall CArray::GetID  (void)
{
    return m_sID ;
}

void __fastcall CArray::SetLotNo(AnsiString _sLotNo)
{
    m_sLotNo = _sLotNo ;
}

AnsiString __fastcall CArray::GetLotNo  (void)
{
    return m_sLotNo ;
}

void __fastcall CArray::SetStep(int _iStep)
{
    m_iStep = _iStep ;
}

int __fastcall CArray::GetStep(void)
{
    return m_iStep ;
}

//---------------------------------------------------------------------------
void __fastcall CArray::ClearMap(void)
{
    SetStat(csNone);
    //UpdateAray();//sun

}

CChip chps[MAX_ARRAY_ROW][MAX_ARRAY_COL];
void __fastcall CArray::FlipX()
{
    //CChip chps[MAX_ARRAY_ROW][MAX_ARRAY_COL]; ;

    for (int i = 0 ; i < m_iMaxRow ; i++) {
        for (int j = 0 ; j < m_iMaxCol ; j++) {
            chps[i][j]=CHPS[i][m_iMaxCol - 1 - j] ;
        }
    }
    memcpy(CHPS , chps , sizeof(CChip) * MAX_ARRAY_ROW * MAX_ARRAY_COL);
    //UpdateAray();//sun
}

void __fastcall CArray::FlipY()
{
    //CChip chps[MAX_ARRAY_ROW][MAX_ARRAY_COL]; ;

    for (int i = 0 ; i < m_iMaxRow ; i++) {
        for (int j = 0 ; j < m_iMaxCol ; j++) {
            chps[i][j]=CHPS[m_iMaxRow - 1 - i][j] ;
        }
    }
    memcpy(CHPS , chps , sizeof(CChip) * MAX_ARRAY_ROW * MAX_ARRAY_COL);
    //UpdateAray();//sun
}

void __fastcall CArray::FlipXY()
{
    //CChip chps[MAX_ARRAY_ROW][MAX_ARRAY_COL]; ;

    for (int i = 0 ; i < m_iMaxRow ; i++) {
        for (int j = 0 ; j < m_iMaxCol ; j++) {
            chps[i][j]=CHPS[m_iMaxRow - 1 - i][m_iMaxCol - 1 - j] ;
        }
    }
    memcpy(CHPS , chps , sizeof(CChip) * MAX_ARRAY_ROW * MAX_ARRAY_COL);
    //UpdateAray();//sun
}

void __fastcall CArray::TurnCw90()
{
    //CChip chps[MAX_ARRAY_ROW][MAX_ARRAY_COL];
    for (int i = 0 ; i < m_iMaxCol ; i++) {
        for (int j = 0 ; j < m_iMaxRow ; j++) {
            chps[i][j]=CHPS[m_iMaxRow - 1 - j][i] ;
        }
    }

    SetMaxColRow(m_iMaxRow , m_iMaxCol);

    memcpy(CHPS , chps , sizeof(CChip) * MAX_ARRAY_ROW * MAX_ARRAY_COL);
}

void __fastcall CArray::TurnCw180()
{
    //CChip chps[MAX_ARRAY_ROW][MAX_ARRAY_COL];
    for (int i = 0 ; i < m_iMaxRow ; i++) {
        for (int j = 0 ; j < m_iMaxCol ; j++) {
            chps[i][j]=CHPS[m_iMaxRow - 1 - i][m_iMaxCol - 1 - j] ;
        }
    }

    memcpy(CHPS , chps , sizeof(CChip) * MAX_ARRAY_ROW * MAX_ARRAY_COL);
}

void __fastcall CArray::TurnCw270()
{

    //CChip chps[MAX_ARRAY_ROW][MAX_ARRAY_COL];
    for (int i = 0 ; i < m_iMaxCol ; i++) {
        for (int j = 0 ; j < m_iMaxRow ; j++) {
                           //r,c
            chps[i][j]=CHPS[j][m_iMaxCol - 1 - i] ;
        }
    }

    SetMaxColRow(m_iMaxRow , m_iMaxCol);

    memcpy(CHPS , chps , sizeof(CChip) * MAX_ARRAY_ROW * MAX_ARRAY_COL);


}

/***************************************************************************/
/* Direct Accessable Functions.                                            */
/***************************************************************************/
void __fastcall CArray::SetStat( int r , int c , EN_CHIP_STAT _iStat)
{

//try {
//EnterCriticalSection(&g_CSAray) ;
    if(m_iMaxCol <= c || m_iMaxCol < 0) return ;
    if(m_iMaxRow <= r || m_iMaxRow < 0) return ;
    CHPS[r][c].SetStat(_iStat);
    //UpdateAray();//sun
//}
//__finally{
//LeaveCriticalSection(&g_CSAray);
//}



}

void __fastcall CArray::SetStat(EN_CHIP_STAT _iStat)
{
//try {
//EnterCriticalSection(&g_CSAray) ;
    for(int r = 0 ; r < m_iMaxRow ; r++) {
        for(int c = 0 ; c < m_iMaxCol ; c++) {
            CHPS[r][c].SetStat(_iStat);
        }
    }
    if(_iStat == csNone){
        SetID("");
        SetLotNo("");
        SetStep(0);
    }
    //UpdateAray();//sun
//}
//__finally{
//LeaveCriticalSection(&g_CSAray);
//}

}


//---------------------------------------------------------------------------
bool __fastcall CArray::FindChip(EN_CHIP_STAT _iStat)
{
    for (int i = 0 ; i < m_iMaxRow ; i++) {
        for (int j = 0 ; j < m_iMaxCol ; j++) {
            if (FindChip(_iStat , i , j)) return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall CArray::FindChip(EN_CHIP_STAT _iStat , int r , int c)
{
    if ((r < 0) || (r >= m_iMaxRow)) return false;
    if ((c < 0) || (c >= m_iMaxCol)) return false;

    bool bRet = CheckStat(r,c,_iStat);

    return bRet ;
    /*
    switch (_iStat) {
        case csNone  : return  CheckStat(r,c,csRslt0);
        case csMask  : return  CheckStat(r,c,csRslt1);
        case csEmpty : return  CheckStat(r,c,csRslt2);
        case csUnkwn : return  CheckStat(r,c,csRslt3);
        case csRslt0 : return  CheckStat(r,c,csRslt4);
        case csRslt1 : return  CheckStat(r,c,csRslt5);
        case csRslt2 : return  CheckStat(r,c,csRslt6);
        case csRslt3 : return  CheckStat(r,c,csRslt7);
        case csRslt4 : return  CheckStat(r,c,csRslt8);
        case csRslt5 : return  CheckStat(r,c,csRslt9);
        case csRslt6 : return  CheckStat(r,c,csNone );
        case csRslt7 : return  CheckStat(r,c,csMask );
        case csRslt8 : return  CheckStat(r,c,csEmpty);
        case csRslt9 : return  CheckStat(r,c,csUnkwn);
        case csWork  : return  CheckStat(r,c,csWork );
        case csWait  : return  CheckStat(r,c,csWait );
        case csFail  : return  CheckStat(r,c,csFail );
        case csEror  : return  CheckStat(r,c,csEror );
        case csGood  : return  CheckStat(r,c,csGood );
    }*/

    //return false;
}

//---------------------------------------------------------------------------
int __fastcall CArray::FindFrstRow(EN_CHIP_STAT _iStat)
{
    //Local Var.
    int iRowNum;
    int iColNum;

    //Find First Row and Col.
    FindFrstRowCol(_iStat , iRowNum , iColNum);

    //return Find Row No.
    return iRowNum;
}

//---------------------------------------------------------------------------
int __fastcall CArray::FindFrstCol(EN_CHIP_STAT _iStat)
{
    //Local Var.
    int iRowNum;
    int iColNum;

    //Find First Row and Col.
    FindFrstColRow(_iStat , iRowNum , iColNum);

    //return Find Col No.
    return iColNum;
}
//---------------------------------------------------------------------------
int __fastcall CArray::FindLastRow(EN_CHIP_STAT _iStat)
{
    //Local Var.
    int iRowNum;
    int iColNum;

    //Find First Row and Col.
    FindLastRowCol(_iStat , iRowNum , iColNum);

    //return Find Row No.
    return iRowNum;
}

//---------------------------------------------------------------------------
int __fastcall CArray::FindLastCol(EN_CHIP_STAT _iStat)
{
    //Local Var.
    int iRowNum;
    int iColNum;

    //Find First Row and Col.
    FindLastColRow(_iStat , iRowNum , iColNum);

    //return Find Col No.
    return iColNum;
}

//---------------------------------------------------------------------------
bool __fastcall CArray::FindFrstRowCol(EN_CHIP_STAT _iStat , int &r , int &c)
{
    //Local Var.
    for (int i = 0 ; i < m_iMaxRow ; i++) {
        for (int j = 0 ; j < m_iMaxCol ; j++) {
            if (FindChip(_iStat , i , j)) {
                r = i;
                c = j;
                return true;
            }
        }
    }

    //No Find.
    r = -1;
    c = -1;
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall CArray::FindFrstColRow(EN_CHIP_STAT _iStat , int &r , int &c)
{
    //Local Var.
    for (int i = 0 ; i < m_iMaxCol ; i++) {
        for (int j = 0 ; j < m_iMaxRow ; j++) {
            if (FindChip(_iStat , j , i)) {
                r = j;
                c = i;
                return true;
            }
        }
    }

    //No Find.
    r = -1;
    c = -1;
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall CArray::FindLastRowCol(EN_CHIP_STAT _iStat , int &r , int &c)
{
    for (int i = m_iMaxRow - 1 ; i >= 0 ; i--) {
        for (int j = m_iMaxCol - 1 ; j >= 0 ; j--) {
            if (FindChip(_iStat , i , j)) {
                r = i;
                c = j;
                return true;
            }
        }
    }

    //No Find.
    r = -1;
    c = -1;
    return false;
}

//---------------------------------------------------------------------------
bool __fastcall CArray::FindFrstRowLastCol(EN_CHIP_STAT _iStat , int &r , int &c)
{
    //Local Var.
    for (int i = 0 ; i < m_iMaxRow ; i++) {
        for (int j = m_iMaxCol - 1 ; j >= 0 ; j--) {
            if (FindChip(_iStat , i , j)) {
                r = i;
                c = j;
                return true;
            }
        }
    }

    //No Find.
    r = -1;
    c = -1;
    return false;
}

//---------------------------------------------------------------------------
bool __fastcall CArray::FindLastRowFrstCol(EN_CHIP_STAT _iStat , int &r , int &c)
{
    for (int i = m_iMaxRow - 1 ; i >= 0 ; i--) {
        for (int j = 0 ; j < m_iMaxCol ; j++) {
            if (FindChip(_iStat , i , j)) {
                r = i;
                c = j;
                return true;
            }
        }
    }


    //No Find.
    r = -1;
    c = -1;
    return false;
}

//---------------------------------------------------------------------------
bool __fastcall CArray::FindLastColFrstRow(EN_CHIP_STAT _iStat , int &r , int &c)
{
    for (int i = m_iMaxCol - 1 ; i >= 0 ; i--) {
        for (int j = 0 ; j < m_iMaxRow ; j++) {
            if (FindChip(_iStat , j , i)) {
                c = i;
                r = j;
                return true;
            }
        }
    }


    //No Find.
    r = -1;
    c = -1;
    return false;
}

//---------------------------------------------------------------------------
bool __fastcall CArray::FindFrstColLastRow(EN_CHIP_STAT _iStat , int &r , int &c)
{
    //Local Var.
    for (int i = 0 ; i < m_iMaxCol ; i++) {
        for (int j = m_iMaxRow - 1 ; j >= 0 ; j--) {
            if (FindChip(_iStat , j , i)) {
                r = j;
                c = i;
                return true;
            }
        }
    }

    //No Find.
    r = -1;
    c = -1;
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall CArray::FindLastColRow(EN_CHIP_STAT _iStat , int &r , int &c)
{
    //Local Var.
    for (int i = m_iMaxCol-1 ; i >= 0 ; i--) {
        for (int j = m_iMaxRow-1 ; j >= 0 ; j--) {
            if (FindChip(_iStat , j , i)) {
                r = j;
                c = i;
                return true;
            }
        }
    }

    //No Find.
    r = -1;
    c = -1;
    return false;
}

//---------------------------------------------------------------------------
bool __fastcall CArray::FindFrstRowCol_Indx (EN_CHIP_STAT _iStat , int iStrCol , int iEndCol , int &R , int &C)
{
    if (iStrCol >= m_iMaxCol) {R = -1; C = -1; return false ;}
    if (iStrCol >= m_iMaxCol) {R = -1; C = -1; return false ;}
    if (iEndCol <  0        ) {R = -1; C = -1; return false ;}
    if (iEndCol <  0        ) {R = -1; C = -1; return false ;}

    for (int i = 0 ; i < m_iMaxRow ; i++) {
        for (int j = iStrCol ; j <= iEndCol ; j++) {
            if (FindChip(_iStat , i , j)) {
                R = i;
                C = j;
                return true;
            }
        }
    }
    //No Find.
    R = -1;
    C = -1;
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall CArray::FindFrstRowLastCol_Indx  (EN_CHIP_STAT _iStat , int iStrCol , int iEndCol , int &R , int &C)
{
    if (iStrCol >= m_iMaxCol) {R = -1; C = -1; return false ;}
    if (iStrCol >= m_iMaxCol) {R = -1; C = -1; return false ;}
    if (iEndCol <  0        ) {R = -1; C = -1; return false ;}
    if (iEndCol <  0        ) {R = -1; C = -1; return false ;}

    for (int i = 0 ; i < m_iMaxRow ; i++) {
        for (int j = iEndCol ; j >= iStrCol ; j--) {
            if (FindChip(_iStat , i , j)) {
                R = i;
                C = j;
                return true;
            }
        }
    }
    //No Find.
    R = -1;
    C = -1;
    return false;
}

//---------------------------------------------------------------------------
bool __fastcall CArray::FindFrstColRow_Indx (EN_CHIP_STAT _iStat , int iStrCol , int iEndCol , int &R , int &C)
{
    if (iStrCol >= m_iMaxCol) {R = -1; C = -1; return false ;}
    if (iStrCol >= m_iMaxCol) {R = -1; C = -1; return false ;}
    if (iEndCol <  0        ) {R = -1; C = -1; return false ;}
    if (iEndCol <  0        ) {R = -1; C = -1; return false ;}

    for (int c = iStrCol ; c <= iEndCol ; c++) {
        for (int r = 0 ; r < m_iMaxRow ; r++) {
            if (FindChip(_iStat , r , c)) {
                R = r;
                C = c;
                return true;
            }
        }
    }

    //No Find.
    R = -1;
    C = -1;
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall CArray::FindFrstColLastRow_Indx  (EN_CHIP_STAT _iStat , int iStrCol , int iEndCol , int &R , int &C)
{
    if (iStrCol >= m_iMaxCol) {R = -1; C = -1; return false ;}
    if (iStrCol >= m_iMaxCol) {R = -1; C = -1; return false ;}
    if (iEndCol <  0        ) {R = -1; C = -1; return false ;}
    if (iEndCol <  0        ) {R = -1; C = -1; return false ;}

    //Local Var.
    for (int c = iStrCol ; c <= iEndCol ; c++) {
        for (int r = m_iMaxRow - 1 ; r >= 0 ; r--) {
            if (FindChip(_iStat , r , c)) {
                R = r;
                C = c;
                return true;
            }
        }
    }

    //No Find.
    R = -1;
    C = -1;
    return false;
}


//---------------------------------------------------------------------------
void __fastcall CArray::Load(bool _bLoad , AnsiString _sBinPath , AnsiString _sIniPath , int _iNo)
{
    //Local Var.
    FILE       *fp;
    TUserINI   UserINI;

    //Make Dir.
    AnsiString  DevName ;
    AnsiString  sPath = _sBinPath ;

    //Load dd
    if(_bLoad)
    {
        fp = fopen(sPath.c_str() , "rb");
        if (fp == NULL) {
            fp = fopen(sPath.c_str() , "wb");
            return ;
        }
    }
    //Save dd
    else
    {
        fp = fopen(sPath.c_str() , "wb");
        if (fp == NULL) return;
    }

    //File Open.
//    fp = fopen(sPath.c_str() , "wb");
//    if (fp == NULL) return;

    //Read&Write.
    for (int r = 0 ; r < MAX_ARRAY_ROW ; r++) {
        for (int c = 0 ; c < MAX_ARRAY_COL ; c++) {
            CHPS[r][c].Load(_bLoad , fp);
        }
    }
    fclose(fp);

    sPath= _sIniPath ;

    //Load
    if(_bLoad) {
        UserINI.Load(sPath, _iNo , "m_iMaxRow" , m_iMaxRow );
        UserINI.Load(sPath, _iNo , "m_iMaxCol" , m_iMaxCol );
        UserINI.Load(sPath, _iNo , "m_sID"     , m_sID     );
        UserINI.Load(sPath, _iNo , "m_sLotNo"  , m_sLotNo  );
        UserINI.Load(sPath, _iNo , "m_iStep"   , m_iStep   );
    }
    else {
        UserINI.Save(sPath, _iNo , "m_iMaxRow" , m_iMaxRow );
        UserINI.Save(sPath, _iNo , "m_iMaxCol" , m_iMaxCol );
        UserINI.Save(sPath, _iNo , "m_sID"     , m_sID     );
        UserINI.Save(sPath, _iNo , "m_sLotNo"  , m_sLotNo  );
        UserINI.Save(sPath, _iNo , "m_iStep"   , m_iStep   );
    }

    if(!m_iMaxRow) m_iMaxRow = 1 ;
    if(!m_iMaxCol) m_iMaxCol = 1 ;

    //UpdateAray();//sun

}


//---------------------------------------------------------------------------
/*
void __fastcall CArray::Load(bool _bLoad , AnsiString _sBinPath , AnsiString _sIniPath , int _iNo)
{
    //Local Var.
    FILE       *fp;
    TUserINI   UserINI;

    //Make Dir.
    AnsiString  DevName ;
    AnsiString  sPath = _sBinPath ;

    //File Open.
    fp = fopen(sPath.c_str() , "wb");
    if (fp == NULL) return;

    //Read&Write.
    for (int r = 0 ; r < MAX_ARRAY_ROW ; r++) {
        for (int c = 0 ; c < MAX_ARRAY_COL ; c++) {
            CHPS[r][c].Load(_bLoad , fp);
        }
    }
    fclose(fp);

    sPath= _sIniPath ;

    //Load
    if(_bLoad) {
        UserINI.Load(sPath, _iNo , "m_iMaxRow" , m_iMaxRow );
        UserINI.Load(sPath, _iNo , "m_iMaxCol" , m_iMaxCol );
        UserINI.Load(sPath, _iNo , "m_sID"     , m_sID     );
        UserINI.Load(sPath, _iNo , "m_sLotNo"  , m_sLotNo  );
        UserINI.Load(sPath, _iNo , "m_iStep"   , m_iStep   );
    }
    else {
        UserINI.Save(sPath, _iNo , "m_iMaxRow" , m_iMaxRow );
        UserINI.Save(sPath, _iNo , "m_iMaxCol" , m_iMaxCol );
        UserINI.Save(sPath, _iNo , "m_sID"     , m_sID     );
        UserINI.Save(sPath, _iNo , "m_sLotNo"  , m_sLotNo  );
        UserINI.Save(sPath, _iNo , "m_iStep"   , m_iStep   );
    }

    if(!m_iMaxRow) m_iMaxRow = 1 ;
    if(!m_iMaxCol) m_iMaxCol = 1 ;

}
*/









//void __fastcall CArray::SetMaxColRow (int c,int r) { m_iMaxCol = c; m_iMaxRow = r; UpdateAray();} //sun
//void __fastcall CArray::SetMaxRow    (int n      ) { m_iMaxRow = n     ;UpdateAray();}//sun
//void __fastcall CArray::SetMaxCol    (int n      ) { m_iMaxCol = n     ;UpdateAray();}//sun
