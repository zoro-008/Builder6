//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormProbe.h"

#include "utilcls.h"
#include "OptionMan.h"
#include "ProbeComUnit.h"
#include "Clipbrd.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmProbe *FrmProbe;
//---------------------------------------------------------------------------
__fastcall TFrmProbe::TFrmProbe(TComponent* Owner)
    : TForm(Owner)
{
    sgProbeCur  -> DoubleBuffered = true;
    sgProbePast -> DoubleBuffered = true;

    memset(&Stat    , 0 , sizeof(SStat ));
}
//---------------------------------------------------------------------------
void __fastcall TFrmProbe::FormShow(TObject *Sender)
{
    Top    = 0 ;
    Left   = 1281 ;
    Width  = 1280 ;
    Height = 1024 ;

    //Left   = 0    ;

    Init();

    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmProbe::Init(void)
{
    if(OM.DevInfo.iColCnt == 0 || OM.DevInfo.iRowCnt == 0 ) return ;

//    sgProbeCur  -> Refresh();
//    sgProbePast -> Refresh();

    sgProbeCur  -> ColCount = OM.DevInfo.iColCnt + 1;
    sgProbeCur  -> RowCount = OM.DevInfo.iRowCnt + 1;
    sgProbePast -> ColCount = OM.DevInfo.iColCnt + 1;
    sgProbePast -> RowCount = OM.DevInfo.iRowCnt + 1;

    for (int iCnt =0 ; iCnt <= OM.DevInfo.iRowCnt + 1 ; iCnt ++ ) sgProbeCur  -> Cells[0   ][iCnt] = iCnt;
    for (int iCnt =0 ; iCnt <= OM.DevInfo.iColCnt + 1 ; iCnt ++ ) sgProbeCur  -> Cells[iCnt][0   ] = iCnt;
    for (int iCnt =0 ; iCnt <= OM.DevInfo.iRowCnt + 1 ; iCnt ++ ) sgProbePast -> Cells[0   ][iCnt] = iCnt;
    for (int iCnt =0 ; iCnt <= OM.DevInfo.iColCnt + 1 ; iCnt ++ ) sgProbePast -> Cells[iCnt][0   ] = iCnt;


}

#include "DataMan.h"
#include "Sequence.h"
void __fastcall TFrmProbe::tmUpdateTimer(TObject *Sender)
{
    AnsiString sAvr;

    int iCol = DM.ARAY[riWRK].FindLastCol(csUnkwn);
    static iColPast = 1 ;

    pnInspSkip -> Visible = OM.CmnOptn.bIgnrInsp ? SEQ._bFlick : false ;


    if(iColPast != iCol && SEQ._bRun) {
       sgProbeCur  -> Col = sgProbeCur  -> ColCount > iCol + 2 ? (iCol + 2 ) : (sgProbeCur  -> ColCount-1) ;
       sgProbePast -> Col = sgProbePast -> ColCount > iCol + 2 ? (iCol + 2 ) : (sgProbePast -> ColCount-1) ;
    }

    iColPast = iCol ;
    


    double dVf = 0.000 , dVr = 0.000 ;
    memset(&Stat    , 0 , sizeof(SStat ));

//    Stat.VfMin = dCChipVf[1][1];
//    Stat.VrMin = dCChipVr[1][1];
    Stat.VfMin = 999;
    Stat.VrMin = 999;
    for (int iR =0 ; iR < OM.DevInfo.iRowCnt +1 ; iR++ ) {
        for (int iC =0 ; iC < OM.DevInfo.iColCnt +1 ; iC++ ) {
            for( int k = 0 ; k < OM.DevOptn.iDgChipCnt ; k++ ) {
                dVf = dCChipVf[iR][iC][k] ;
                dVr = dCChipVr[iR][iC][k] ;
    /*            str.printf("%0.3f|%0.3f",dVf,dVr);
                if(((OM.DevOptn.d1ChLow   <= dVf <= OM.DevOptn.d1ChHigh  ) ||
                  ( (OM.DevOptn.dZenerLow <= dVr <= OM.DevOptn.dZenerHigh) && OM.DevOptn.bUseZener )) ) {
                    if(dVf) {
                        sgProbeCur->Canvas ->Pen   -> Color = clBlack ;
                        sgProbeCur->Canvas ->Brush -> Color = clWhite ;
                        sgProbeCur->Canvas ->FillRect(sgProbeCur->CellRect(iC,iR)) ;
                        sgProbeCur->Canvas ->TextOutA(sgProbeCur->CellRect(iC,iR).left+2,sgProbeCur->CellRect(iC,iR).top+2,str);
                    }
                }
                else {
                    if(dVf) {
    //                    str.printf("%0.3f|%0.3f",dVf,dVr);
                        sgProbeCur->Canvas ->Pen   -> Color = clBlack ;
                        sgProbeCur->Canvas ->Brush -> Color = clRed   ;
                        sgProbeCur->Canvas ->FillRect(sgProbeCur->CellRect(iC,iR)) ;
                        sgProbeCur->Canvas ->TextOutA(sgProbeCur->CellRect(iC,iR).left+2,sgProbeCur->CellRect(iC,iR).top+2,str);
                    }
                }
    */

    //            if(OM.DevOptn.bUseZener) str.printf("%0.3f %0.3f",dVf,dVr);
    //            else                     str.printf("%0.3f",dVf);
                //if(!dVf) str = "";
    //            sgProbeCur -> Cells[iC+1][iR+1] = str;

                if (dVf > 0 && Stat.VfMin > dVf) Stat.VfMin = dVf ;
                if (           Stat.VfMax < dVf) Stat.VfMax = dVf ;
                if (dVr > 0 && Stat.VrMin > dVr) Stat.VrMin = dVr ;
                if (           Stat.VrMax < dVr) Stat.VrMax = dVr ;

                if ( dVf > 0 ) { Stat.VfCnt += 1 ; Stat.VfSum += dVf ; }
                if ( dVr > 0 ) { Stat.VrCnt += 1 ; Stat.VrSum += dVr ; }
            }
        }
    }
    if(Stat.VfMin == 999) lbVfMin -> Caption = 0 ;
    else                  lbVfMin -> Caption = Stat.VfMin ;
    if(Stat.VrMin == 999) lbVrMin -> Caption = 0 ;
    else                  lbVrMin -> Caption = Stat.VrMin ;
    lbVfMax -> Caption = Stat.VfMax ;
    lbVrMax -> Caption = Stat.VrMax ;

    if( Stat.VfCnt ) lbVfAvr -> Caption = sAvr.sprintf("%0.2f", Stat.VfSum / Stat.VfCnt ) ;
    else lbVfAvr -> Caption = 0.000;
    if( Stat.VrCnt ) lbVrAvr -> Caption = sAvr.sprintf("%0.2f", Stat.VrSum / Stat.VrCnt ) ;
    else lbVrAvr -> Caption = 0.000;

//    for (int iR =1 ; iR <= OM.DevInfo.iRowCnt ; iR++ ) {
//        for (int iC =1 ; iC <= OM.DevInfo.iColCnt ; iC++ ) {
//            if(OM.DevOptn.bUseZener) str.printf("%0.3f %0.3f",dPChipVf[iR-1][iC-1],dPChipVr[iR-1][iC-1]);
//            else                     str.printf("%0.3f",dPChipVf[iR-1][iC-1]);
//            if(!dPChipVf[iR-1][iC-1]) str = "";
//            sgProbePast -> Cells[iC][iR] = str;
/*            dVf = dPChipVf[iR-1][iC-1] ;
            dVr = dPChipVr[iR-1][iC-1] ;
            str.printf("%0.3f %0.3f",dVf,dVr);

            if(((OM.DevOptn.d1ChLow   <= dVf <= OM.DevOptn.d1ChHigh  ) ||
              ( (OM.DevOptn.dZenerLow <= dVr <= OM.DevOptn.dZenerHigh) && OM.DevOptn.bUseZener )) ) {
                if(dVf) {
                    sgProbePast->Canvas ->Pen   -> Color = clBlack ;
                    sgProbePast->Canvas ->Brush -> Color = clWhite ;
                    sgProbePast->Canvas ->FillRect(sgProbePast->CellRect(iC,iR)) ;
                    sgProbePast->Canvas ->TextOutA(sgProbePast->CellRect(iC,iR).left+2,sgProbePast->CellRect(iC,iR).top+2,str);
                }
            }
            else {
                if(dVf) {
                    sgProbePast->Canvas ->Pen   -> Color = clBlack ;
                    sgProbePast->Canvas ->Brush -> Color = clRed   ;
                    sgProbePast->Canvas ->FillRect(sgProbePast->CellRect(iC,iR)) ;
//                    str.printf("%0.3f %0.3f",dVf,dVr);
                    sgProbePast->Canvas ->TextOutA(sgProbePast->CellRect(iC,iR).left+2,sgProbePast->CellRect(iC,iR).top+2,str);
                }
            }
*/
//        }
//    }

    sgProbeCur  -> Invalidate ();
    sgProbePast -> Invalidate ();



    int Age = FileAge(Application -> ExeName);
    AnsiString Date = FileDateToDateTime(Age).FormatString("''yyyy'³â 'm'¿ù 'd'ÀÏ 'AM/PM' 'h'½Ã 'n'ºÐ'");
    lbDate -> Caption = Date ;

    lbDevice -> Caption = OM.m_sCrntDev ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmProbe::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmProbe::sgProbeCurDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
    AnsiString sItemVf[3] , sItemVr[3] ;
    if(ARow == 0 || ACol == 0) return ;

    int iDftRowHeight = 18 ;
    int iRoom = 0 ;

    TRect dRect[6] ;

    for(int i =0 ; i<6 ; i++ )    dRect[i] = Rect ;

    sItemVf[0].printf("%0.2f",dCChipVf[ARow-1][ACol-1][0]);
    sItemVr[0].printf("%0.2f",dCChipVr[ARow-1][ACol-1][0]);
    sItemVf[1].printf("%0.2f",dCChipVf[ARow-1][ACol-1][1]);
    sItemVr[1].printf("%0.2f",dCChipVr[ARow-1][ACol-1][1]);
    sItemVf[2].printf("%0.2f",dCChipVf[ARow-1][ACol-1][2]);
    sItemVr[2].printf("%0.2f",dCChipVr[ARow-1][ACol-1][2]);

         if(OM.DevOptn.iDgChipCnt == 1 &&  OM.DevOptn.bUseZener ) iRoom = 2 ;//2Ä­
    else if(OM.DevOptn.iDgChipCnt == 1 && !OM.DevOptn.bUseZener ) iRoom = 1 ;//2Ä­
    else if(OM.DevOptn.iDgChipCnt == 2 &&  OM.DevOptn.bUseZener ) iRoom = 4 ;//4Ä­
    else if(OM.DevOptn.iDgChipCnt == 2 && !OM.DevOptn.bUseZener ) iRoom = 2 ;//2Ä­
    else if(OM.DevOptn.iDgChipCnt == 3 &&  OM.DevOptn.bUseZener ) iRoom = 6 ;//6Ä­
    else if(OM.DevOptn.iDgChipCnt == 3 && !OM.DevOptn.bUseZener ) iRoom = 3 ;//3Ä­


    switch(iRoom)
    {
        default :
        break ;
        case 1 : if( sgProbeCur->RowHeights[ARow] != iDftRowHeight    ) {
                     sgProbeCur->RowHeights[ARow]  = iDftRowHeight     ;
//                     sgProbeCur->DefaultColWidth   = 25 ;
                     return ;
                 }
//                 dRect[0].Bottom = dRect[0].Top + iDftRowHeight ;
//                 dRect[1].Top    = dRect[0].Bottom                 ;
        break ;

        case 2 : if( sgProbeCur->RowHeights[ARow] != iDftRowHeight * 2) {
                     sgProbeCur->RowHeights[ARow]  = iDftRowHeight * 2 ;
//                     sgProbeCur->DefaultColWidth   = 25 ;
                     return ;
                 }
                 dRect[0].Bottom = dRect[0].Top + iDftRowHeight ;
                 dRect[1].Top    = dRect[0].Bottom                 ;
        break ;
        case 3 : if( sgProbeCur->RowHeights[ARow] != iDftRowHeight * 3 ) {
                     sgProbeCur->RowHeights[ARow]  = iDftRowHeight * 3 ;
//                     sgProbeCur->DefaultColWidth   = 25 ;
                     return;
                 }

                 dRect[0].Bottom = dRect[0].Top + iDftRowHeight    ;
                 dRect[1].Top    = dRect[0].Bottom               ;
                 dRect[1].Bottom = dRect[1].Top + iDftRowHeight    ;
                 dRect[2].Top    = dRect[1].Bottom               ;
        break ;
        case 4 : if( sgProbeCur->RowHeights[ARow] != iDftRowHeight * 4 ) {
                     sgProbeCur->RowHeights[ARow]  = iDftRowHeight * 4 ;
//                     sgProbeCur->DefaultColWidth   = 25 ;
                     return;
                 }

                 dRect[0].Bottom = dRect[0].Top + iDftRowHeight  ;
                 dRect[1].Top    = dRect[0].Bottom               ;
                 dRect[1].Bottom = dRect[1].Top + iDftRowHeight  ;
                 dRect[2].Top    = dRect[1].Bottom               ;
                 dRect[2].Bottom = dRect[2].Top + iDftRowHeight  ;
                 dRect[3].Top    = dRect[2].Bottom               ;
                 //Rect4.Bottom = Rect4.Top + iRectHeight * 4;
        break ;
        case 6 : if( sgProbeCur->RowHeights[ARow] != iDftRowHeight * 6 ){
                     sgProbeCur->RowHeights[ARow]  = iDftRowHeight * 6 ;
//                     sgProbeCur->DefaultColWidth   = 25 ;
                     return ;
                 }

                 dRect[0].Bottom = dRect[0].Top + iDftRowHeight    ;
                 dRect[1].Top    = dRect[0].Bottom                 ;
                 dRect[1].Bottom = dRect[1].Top + iDftRowHeight * 2;
                 dRect[2].Top    = dRect[1].Bottom                 ;
                 dRect[2].Bottom = dRect[2].Top + iDftRowHeight * 3;
                 dRect[3].Top    = dRect[2].Bottom                 ;
                 dRect[3].Bottom = dRect[3].Top + iDftRowHeight * 4;
                 dRect[4].Top    = dRect[3].Bottom                 ;
                 dRect[4].Bottom = dRect[4].Top + iDftRowHeight * 5;
                 dRect[5].Top    = dRect[4].Bottom                 ;
        break ;

    }

    int iCount = -1 ;
    for(int i=0 ; i<OM.DevOptn.iDgChipCnt ; i++ )
    {
        iCount ++ ;
        if( iCChipRslt[ARow-1][ACol-1][i] == prReady) {
            sgProbeCur->Canvas->Brush->Color = clYellow ;
            sgProbeCur->Canvas->FillRect(Rect)          ;
        }
        else if( iCChipRslt[ARow-1][ACol-1][i] == prOk) {
            sgProbeCur->Canvas->Brush->Color = clLime   ;
            sgProbeCur->Canvas->FillRect(dRect[iCount])          ;
            sgProbeCur->Canvas->TextOut(dRect[iCount].Left + 1, dRect[iCount].Top , sItemVf[i]);
            if(OM.DevOptn.bUseZener) {
                sgProbeCur->Canvas->TextOut(dRect[iCount+1].Left + 1, dRect[iCount+1].Top , sItemVr[i]);
                iCount++;
            }
        }
        else if(iCChipRslt[ARow-1][ACol-1][i] == prVFVRNg) {
            sgProbeCur->Canvas->Brush->Color = clRed ;
            sgProbeCur->Canvas->FillRect(dRect[iCount])       ;
            sgProbeCur->Canvas->TextOut(dRect[iCount].Left + 1, dRect[iCount].Top , sItemVf[i]);
            if(OM.DevOptn.bUseZener) {
                sgProbeCur->Canvas->TextOut(dRect[iCount+1].Left + 1, dRect[iCount+1].Top , sItemVr[i]);
                iCount++;
            }
        }

        else if( iCChipRslt[ARow-1][ACol-1][i] == prVFNg) {
            sgProbeCur->Canvas->Brush->Color = clFuchsia  ;
            sgProbeCur->Canvas->FillRect(dRect[iCount])          ;
            sgProbeCur->Canvas->TextOut(dRect[iCount].Left + 1, dRect[iCount].Top , sItemVf[i]);
            //sgProbeCur->Canvas->Brush->Color = clLime ;
            //sgProbeCur->Canvas->FillRect(DnRect)      ;
            if(OM.DevOptn.bUseZener) {
                sgProbeCur->Canvas->TextOut(dRect[iCount+1].Left + 1, dRect[iCount+1].Top , sItemVr[i]);
                iCount++;
            }
        }

        else if( iCChipRslt[ARow-1][ACol-1][i] == prVRNg) {
            sgProbeCur->Canvas->Brush->Color = clOlive ;
            sgProbeCur->Canvas->FillRect(dRect[iCount])          ;
            sgProbeCur->Canvas->TextOut(dRect[iCount].Left + 1, dRect[iCount].Top , sItemVf[i]);
            //sgProbeCur->Canvas->Brush->Color = clRed  ;
            //sgProbeCur->Canvas->FillRect(DnRect)      ;
            if(OM.DevOptn.bUseZener) {
                sgProbeCur->Canvas->TextOut(dRect[iCount+1].Left + 1, dRect[iCount+1].Top , sItemVr[i]);
                iCount++;
            }
        }
    }
}

void __fastcall TFrmProbe::sgProbePastDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
    AnsiString sItemVf[3] , sItemVr[3] ;
    if(ARow == 0 || ACol == 0) return ;

    int iDftRowHeight = 18 ;
    int iRoom = 0 ;

    TRect dRect[6] ;

    for(int i =0 ; i<6 ; i++ )    dRect[i] = Rect ;

    sItemVf[0].printf("%0.2f",dPChipVf[ARow-1][ACol-1][0]);
    sItemVr[0].printf("%0.2f",dPChipVr[ARow-1][ACol-1][0]);
    sItemVf[1].printf("%0.2f",dPChipVf[ARow-1][ACol-1][1]);
    sItemVr[1].printf("%0.2f",dPChipVr[ARow-1][ACol-1][1]);
    sItemVf[2].printf("%0.2f",dPChipVf[ARow-1][ACol-1][2]);
    sItemVr[2].printf("%0.2f",dPChipVr[ARow-1][ACol-1][2]);

         if(OM.DevOptn.iDgChipCnt == 1 &&  OM.DevOptn.bUseZener ) iRoom = 2 ;//2Ä­
    else if(OM.DevOptn.iDgChipCnt == 1 && !OM.DevOptn.bUseZener ) iRoom = 1 ;//2Ä­
    else if(OM.DevOptn.iDgChipCnt == 2 &&  OM.DevOptn.bUseZener ) iRoom = 4 ;//4Ä­
    else if(OM.DevOptn.iDgChipCnt == 2 && !OM.DevOptn.bUseZener ) iRoom = 2 ;//2Ä­
    else if(OM.DevOptn.iDgChipCnt == 3 &&  OM.DevOptn.bUseZener ) iRoom = 6 ;//6Ä­
    else if(OM.DevOptn.iDgChipCnt == 3 && !OM.DevOptn.bUseZener ) iRoom = 3 ;//3Ä­


    switch(iRoom)
    {
        default :
        break ;
        case 1 : if( sgProbePast->RowHeights[ARow] != iDftRowHeight    ) {
                     sgProbePast->RowHeights[ARow]  = iDftRowHeight     ;
//                     sgProbeCur->DefaultColWidth   = 25 ;
                     return ;
                 }
//                 dRect[0].Bottom = dRect[0].Top + iDftRowHeight ;
//                 dRect[1].Top    = dRect[0].Bottom                 ;
        break ;
        case 2 : if( sgProbePast->RowHeights[ARow] != iDftRowHeight * 2) {
                     sgProbePast->RowHeights[ARow]  = iDftRowHeight * 2 ;
//                     sgProbePast->DefaultColWidth   = 25 ;
                     return ;
                 }
                 dRect[0].Bottom = dRect[0].Top + iDftRowHeight ;
                 dRect[1].Top    = dRect[0].Bottom                 ;
        break ;
        case 3 : if( sgProbePast->RowHeights[ARow] != iDftRowHeight * 3 ) {
                     sgProbePast->RowHeights[ARow]  = iDftRowHeight * 3 ;
//                     sgProbePast->DefaultColWidth   = 25 ;
                     return;
                 }

                 dRect[0].Bottom = dRect[0].Top + iDftRowHeight    ;
                 dRect[1].Top    = dRect[0].Bottom               ;
                 dRect[1].Bottom = dRect[1].Top + iDftRowHeight    ;
                 dRect[2].Top    = dRect[1].Bottom               ;
        break ;
        case 4 : if( sgProbePast->RowHeights[ARow] != iDftRowHeight * 4 ) {
                     sgProbePast->RowHeights[ARow]  = iDftRowHeight * 4 ;
//                     sgProbePast->DefaultColWidth   = 25 ;
                     return;
                 }

                 dRect[0].Bottom = dRect[0].Top + iDftRowHeight  ;
                 dRect[1].Top    = dRect[0].Bottom               ;
                 dRect[1].Bottom = dRect[1].Top + iDftRowHeight  ;
                 dRect[2].Top    = dRect[1].Bottom               ;
                 dRect[2].Bottom = dRect[2].Top + iDftRowHeight  ;
                 dRect[3].Top    = dRect[2].Bottom               ;
                 //Rect4.Bottom = Rect4.Top + iRectHeight * 4;
        break ;
        case 6 : if( sgProbePast->RowHeights[ARow] != iDftRowHeight * 6 ){
                     sgProbePast->RowHeights[ARow]  = iDftRowHeight * 6 ;
//                     sgProbePast->DefaultColWidth   = 25 ;
                     return ;
                 }

                 dRect[0].Bottom = dRect[0].Top + iDftRowHeight    ;
                 dRect[1].Top    = dRect[0].Bottom                 ;
                 dRect[1].Bottom = dRect[1].Top + iDftRowHeight * 2;
                 dRect[2].Top    = dRect[1].Bottom                 ;
                 dRect[2].Bottom = dRect[2].Top + iDftRowHeight * 3;
                 dRect[3].Top    = dRect[2].Bottom                 ;
                 dRect[3].Bottom = dRect[3].Top + iDftRowHeight * 4;
                 dRect[4].Top    = dRect[3].Bottom                 ;
                 dRect[4].Bottom = dRect[4].Top + iDftRowHeight * 5;
                 dRect[5].Top    = dRect[4].Bottom                 ;
        break ;

    }

    int iCount = -1 ;
    for(int i=0 ; i<OM.DevOptn.iDgChipCnt ; i++ )
    {
        iCount ++ ;
        if( iPChipRslt[ARow-1][ACol-1][i] == prReady) {
            sgProbePast->Canvas->Brush->Color = clYellow ;
            sgProbePast->Canvas->FillRect(Rect)          ;
        }
        else if( iPChipRslt[ARow-1][ACol-1][i] == prOk) {
            sgProbePast->Canvas->Brush->Color = clLime   ;
            sgProbePast->Canvas->FillRect(dRect[iCount])          ;
            sgProbePast->Canvas->TextOut(dRect[iCount].Left + 1, dRect[iCount].Top , sItemVf[i]);
            if(OM.DevOptn.bUseZener) {
                sgProbePast->Canvas->TextOut(dRect[iCount+1].Left + 1, dRect[iCount+1].Top , sItemVr[i]);
                iCount++;
            }
        }
        else if(iPChipRslt[ARow-1][ACol-1][i] == prVFVRNg) {
            sgProbePast->Canvas->Brush->Color = clRed ;
            sgProbePast->Canvas->FillRect(dRect[iCount])       ;
            sgProbePast->Canvas->TextOut(dRect[iCount].Left + 1, dRect[iCount].Top , sItemVf[i]);
            if(OM.DevOptn.bUseZener) {
                sgProbePast->Canvas->TextOut(dRect[iCount+1].Left + 1, dRect[iCount+1].Top , sItemVr[i]);
                iCount++;
            }
        }

        else if( iPChipRslt[ARow-1][ACol-1][i] == prVFNg) {
            sgProbePast->Canvas->Brush->Color = clFuchsia  ;
            sgProbePast->Canvas->FillRect(dRect[iCount])          ;
            sgProbePast->Canvas->TextOut(dRect[iCount].Left + 1, dRect[iCount].Top , sItemVf[i]);
            //sgProbePast->Canvas->Brush->Color = clLime ;
            //sgProbePast->Canvas->FillRect(DnRect)      ;
            if(OM.DevOptn.bUseZener) {
                sgProbePast->Canvas->TextOut(dRect[iCount+1].Left + 1, dRect[iCount+1].Top , sItemVr[i]);
                iCount++;
            }
        }

        else if( iPChipRslt[ARow-1][ACol-1][i] == prVRNg) {
            sgProbePast->Canvas->Brush->Color = clOlive ;
            sgProbePast->Canvas->FillRect(dRect[iCount])          ;
            sgProbePast->Canvas->TextOut(dRect[iCount].Left + 1, dRect[iCount].Top , sItemVf[i]);
            //sgProbePast->Canvas->Brush->Color = clRed  ;
            //sgProbePast->Canvas->FillRect(DnRect)      ;
            if(OM.DevOptn.bUseZener) {
                sgProbePast->Canvas->TextOut(dRect[iCount+1].Left + 1, dRect[iCount+1].Top , sItemVr[i]);
                iCount++;
            }
        }
    }
/*    AnsiString sItemVf[3] , sItemVr[3] ;
    if(ARow == 0 || ACol == 0) return ;

    int iDftColWidth = 18 ;
//    bool bChip1 , bChip2 , bChip3 ;
    int iRoom = 0 ;

    TRect dRect[6] ;

    for(int i =0 ; i<6 ; i++ )    dRect[i] = Rect ;

    sItemVf[0].printf("%0.2f",dPChipVf[ARow-1][ACol-1][0]);
    sItemVr[0].printf("%0.2f",dPChipVr[ARow-1][ACol-1][0]);
    sItemVf[1].printf("%0.2f",dPChipVf[ARow-1][ACol-1][1]);
    sItemVr[1].printf("%0.2f",dPChipVr[ARow-1][ACol-1][1]);
    sItemVf[2].printf("%0.2f",dPChipVf[ARow-1][ACol-1][2]);
    sItemVr[2].printf("%0.2f",dPChipVr[ARow-1][ACol-1][2]);

         if(OM.DevOptn.iDgChipCnt == 1 &&  OM.DevOptn.bUseZener ) iRoom = 2 ;//2Ä­
    else if(OM.DevOptn.iDgChipCnt == 1 && !OM.DevOptn.bUseZener ) iRoom = 2 ;//2Ä­
    else if(OM.DevOptn.iDgChipCnt == 2 &&  OM.DevOptn.bUseZener ) iRoom = 4 ;//4Ä­
    else if(OM.DevOptn.iDgChipCnt == 2 && !OM.DevOptn.bUseZener ) iRoom = 2 ;//2Ä­
    else if(OM.DevOptn.iDgChipCnt == 3 &&  OM.DevOptn.bUseZener ) iRoom = 6 ;//6Ä­
    else if(OM.DevOptn.iDgChipCnt == 3 && !OM.DevOptn.bUseZener ) iRoom = 3 ;//3Ä­

    int iRectHeight = 0 ;

    switch(iRoom)
    {
        default :
        break ;
        case 2 : sgProbeCur->DefaultColWidth = iDftColWidth * 2 ;
                 dRect[0].Bottom = dRect[0].Top + Rect.Height() /2 ;
                 dRect[1].Top    = dRect[0].Bottom                 ;
        break ;
        case 3 : sgProbeCur->DefaultColWidth = iDftColWidth * 3 ;

                 iRectHeight = Rect.Height() /3 ;

                 dRect[0].Bottom = dRect[0].Top + iRectHeight    ;
                 dRect[1].Top    = dRect[0].Bottom               ;
                 dRect[1].Bottom = dRect[1].Top + iRectHeight * 2;
                 dRect[2].Top    = dRect[1].Bottom               ;
        break ;
        case 4 : sgProbeCur->DefaultColWidth = iDftColWidth * 4 ;
                 iRectHeight = Rect.Height() /4 ;

                 dRect[0].Bottom = dRect[0].Top + iRectHeight    ;
                 dRect[1].Top    = dRect[0].Bottom               ;
                 dRect[1].Bottom = dRect[1].Top + iRectHeight * 2;
                 dRect[2].Top    = dRect[1].Bottom               ;
                 dRect[2].Bottom = dRect[2].Top + iRectHeight * 3;
                 dRect[3].Top    = dRect[2].Bottom               ;
                 //Rect4.Bottom = Rect4.Top + iRectHeight * 4;
        break ;
        case 6 : sgProbeCur->DefaultColWidth = iDftColWidth * 6 ;

                 iRectHeight = Rect.Height() /6 ;

                 dRect[0].Bottom = dRect[0].Top + iRectHeight    ;
                 dRect[1].Top    = dRect[0].Bottom               ;
                 dRect[1].Bottom = dRect[1].Top + iRectHeight * 2;
                 dRect[2].Top    = dRect[1].Bottom               ;
                 dRect[2].Bottom = dRect[2].Top + iRectHeight * 3;
                 dRect[3].Top    = dRect[2].Bottom               ;
                 dRect[3].Bottom = dRect[3].Top + iRectHeight * 4;
                 dRect[4].Top    = dRect[3].Bottom               ;
                 dRect[4].Bottom = dRect[4].Top + iRectHeight * 5;
                 dRect[5].Top    = dRect[4].Bottom               ;
        break ;

    }

    int iCount = 0 ;
    for(int i=0 ; i<OM.DevOptn.iDgChipCnt ; i++ )
    {
        if( iCChipRslt[ARow-1][ACol-1][iCount] == prReady) {
            sgProbeCur->Canvas->Brush->Color = clYellow ;
            sgProbeCur->Canvas->FillRect(Rect)          ;
        }
        else if( iCChipRslt[ARow-1][ACol-1][iCount] == prOk) {
            sgProbeCur->Canvas->Brush->Color = clLime   ;
            sgProbeCur->Canvas->FillRect(Rect)          ;
            sgProbeCur->Canvas->TextOut(dRect[iCount].Left + 1, dRect[iCount].Top , sItemVf[i]);
            if(OM.DevOptn.bUseZener) {
                sgProbeCur->Canvas->TextOut(dRect[iCount+1].Left + 1, dRect[iCount+1].Top , sItemVr[i]);
                iCount++;
            }
        }
        else if(iCChipRslt[ARow-1][ACol-1][iCount] == prVFVRNg) {
            sgProbeCur->Canvas->Brush->Color = clRed ;
            sgProbeCur->Canvas->FillRect(Rect)       ;
            sgProbeCur->Canvas->TextOut(dRect[iCount].Left + 1, dRect[iCount].Top , sItemVf[i]);
            if(OM.DevOptn.bUseZener) {
                sgProbeCur->Canvas->TextOut(dRect[iCount+1].Left + 1, dRect[iCount+1].Top , sItemVr[i]);
                iCount++;
            }
        }

        else if( iCChipRslt[ARow-1][ACol-1][i] == prVFNg) {
            sgProbeCur->Canvas->Brush->Color = clFuchsia  ;
            sgProbeCur->Canvas->FillRect(Rect)          ;
            sgProbeCur->Canvas->TextOut(dRect[iCount].Left + 1, dRect[iCount].Top , sItemVf[i]);
            //sgProbeCur->Canvas->Brush->Color = clLime ;
            //sgProbeCur->Canvas->FillRect(DnRect)      ;
            if(OM.DevOptn.bUseZener) {
                sgProbeCur->Canvas->TextOut(dRect[iCount+1].Left + 1, dRect[iCount+1].Top , sItemVr[i]);
                iCount++;
            }
        }

        else if( iCChipRslt[ARow-1][ACol-1][i] == prVRNg) {
            sgProbeCur->Canvas->Brush->Color = clOlive ;
            sgProbeCur->Canvas->FillRect(Rect)          ;
            sgProbeCur->Canvas->TextOut(dRect[iCount].Left + 1, dRect[iCount].Top , sItemVf[i]);
            //sgProbeCur->Canvas->Brush->Color = clRed  ;
            //sgProbeCur->Canvas->FillRect(DnRect)      ;
            if(OM.DevOptn.bUseZener) {
                sgProbeCur->Canvas->TextOut(dRect[iCount+1].Left + 1, dRect[iCount+1].Top , sItemVr[i]);
                iCount++;
            }
        }
    }
*/
}
//---------------------------------------------------------------------------
void __fastcall TFrmProbe::btPastSaveAsClick(TObject *Sender)
{
    if( !SaveDialog1->Execute() ) return ;
    AnsiString sPath;
    sPath = SaveDialog1 -> FileName ;
    PCM.WritePastProbeDataCsv(sPath);

}
//---------------------------------------------------------------------------
void __fastcall TFrmProbe::sgProbeCurClick(TObject *Sender)
{
//    sgProbePast -> Col =  sgProbeCur -> Col ; ½ºÅ©·Ñ ¹Ù¸¦ ÄÁÆ®·Ñ ÇÏ´Â °Í Ã£¾Æ ºÁ¾ß ÇÒµí.
//    sgProbePast -> Row =  sgProbeCur -> Row ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmProbe::sgProbePastClick(TObject *Sender)
{
//    sgProbeCur -> Col = sgProbePast -> Col ;
//    sgProbeCur -> Row = sgProbePast -> Row ;
}
//---------------------------------------------------------------------------

