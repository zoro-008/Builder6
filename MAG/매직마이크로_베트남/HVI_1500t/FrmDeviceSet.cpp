//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmDeviceSet.h"
//---------------------------------------------------------------------------
#include "SlogUnit.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "UtilDefine.h"
#include "Head.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDeviceSet *FormDeviceSet;
//---------------------------------------------------------------------------
__fastcall TFormDeviceSet::TFormDeviceSet(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormDeviceSet::btSaveDeviceClick(TObject *Sender)
{                      
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    UpdateDevInfo(toBuff);
    OM.SaveDevInfo(OM.m_sCrntDev);

    OM.DMSetMaxColRow();

    PaintStrpInfo();
    PaintCassetteInfo();
}
//---------------------------------------------------------------------------
void __fastcall TFormDeviceSet::UpdateDevInfo(bool bTable)
{
    if(bTable) {
        edColPitch       -> Text = OM.DevInfo.dColPitch     ;
        edRowPitch       -> Text = OM.DevInfo.dRowPitch     ;

        edColGroupCnt    -> Text = OM.DevInfo.iColGrCnt     ;
        edRowGroupCnt    -> Text = OM.DevInfo.iRowGrCnt     ;

        edColGroupGap    -> Text = OM.DevInfo.dColGrGap     ;
        edRowGroupGap    -> Text = OM.DevInfo.dRowGrGap     ;

        edColCnt         -> Text = OM.DevInfo.iColCnt       ;
        edRowCnt         -> Text = OM.DevInfo.iRowCnt       ;

        edCsSlotCnt      -> Text = OM.DevInfo.iCsSlCnt      ;
        edCsSlotPitch    -> Text = OM.DevInfo.dCsSlPitch    ;

        edVisnIndexId    -> Text = OM.DevInfo.sVisnIndexId  ;

        edInsColCnt      -> Text = OM.DevInfo.iColInspCnt   ;
        edInsRowCnt      -> Text = OM.DevInfo.iRowInspCnt   ;

        edRowSubGroupCount -> Text = OM.DevInfo.iRowSubGroupCount   ;
        edRowSubGroupGap   -> Text = OM.DevInfo.dRowSubGroupGap     ;

        edColSubGroupCount -> Text = OM.DevInfo.iColSubGroupCount   ;
        edColSubGroupGap   -> Text = OM.DevInfo.dColSubGroupGap     ;

    }
    else {
        OM.DevInfo.dColPitch      = edColPitch       -> Text.ToDouble() ;
        OM.DevInfo.dRowPitch      = edRowPitch       -> Text.ToDouble() ;

        OM.DevInfo.iColGrCnt      = edColGroupCnt    -> Text.ToInt()    ;
        OM.DevInfo.iRowGrCnt      = edRowGroupCnt    -> Text.ToInt()    ;

        OM.DevInfo.dColGrGap      = edColGroupGap    -> Text.ToDouble() ;
        OM.DevInfo.dRowGrGap      = edRowGroupGap    -> Text.ToDouble() ;

        OM.DevInfo.iColCnt        = edColCnt         -> Text.ToInt()    ;
        OM.DevInfo.iRowCnt        = edRowCnt         -> Text.ToInt()    ;

        OM.DevInfo.iCsSlCnt       = edCsSlotCnt      -> Text.ToInt()    ;
        OM.DevInfo.dCsSlPitch     = edCsSlotPitch    -> Text.ToDouble() ;

        OM.DevInfo.sVisnIndexId   = edVisnIndexId    -> Text            ;

        OM.DevInfo.iColInspCnt    = edInsColCnt      -> Text.ToInt() ;
        OM.DevInfo.iRowInspCnt    = edInsRowCnt      -> Text.ToInt() ;

        OM.DevInfo.iRowSubGroupCount = edRowSubGroupCount -> Text.ToIntDef(0);
        OM.DevInfo.dRowSubGroupGap   = edRowSubGroupGap   -> Text.ToDouble( );

        OM.DevInfo.iColSubGroupCount = edColSubGroupCount -> Text.ToIntDef(0);
        OM.DevInfo.dColSubGroupGap   = edColSubGroupGap   -> Text.ToDouble( );

    }

}

void __fastcall TFormDeviceSet::FormShow(TObject *Sender)
{
    UpdateDevInfo (toTabl);

    PaintStrpInfo();
    PaintCassetteInfo();
}
//---------------------------------------------------------------------------

void __fastcall TFormDeviceSet::PaintStrpInfo()
{
    imArray -> Picture -> Bitmap =NULL; //이미지 초기화

    int uX1, uX2, uY1, uY2;
    int iColGrCnt , iRowGrCnt , iColCnt , iRowCnt , iColGrInCnt , iRowGrInCnt, iRowSubGrCnt, iRowSubGrInCnt ;

    iRowGrCnt      = OM.DevInfo.iRowGrCnt         ;
    iColGrCnt      = OM.DevInfo.iColGrCnt         ;
    iRowCnt        = OM.DevInfo.iRowCnt           ;
    iColCnt        = OM.DevInfo.iColCnt           ;
    iRowSubGrCnt   = OM.DevInfo.iRowSubGroupCount ;

    if(!iColGrCnt)    iColGrCnt    = 1 ;
    if(!iRowGrCnt)    iRowGrCnt    = 1 ;
    if(!iRowSubGrCnt) iRowSubGrCnt = 1 ;

    iColGrInCnt    = iColCnt / iColGrCnt; // 전체 카운터 / 그룹 카운터 = 한 그룹 카운터 내에 카운터 개수
    iRowGrInCnt    = iRowCnt / iRowGrCnt;
    iRowSubGrInCnt = (iRowCnt / iRowGrCnt) / iRowSubGrCnt ;

    int iGetWidth  = imArray -> Width  - (5 * (iColGrCnt-1)) ; // 사각형들이 이미지안에 다 들어오게 하기 위함.. 그냥 원래 사이즈로 하면 삐져나감 사각형들이..
  //int iGetHeight = imArray -> Height - (5 * (iRowGrCnt-1)) - (5 * (iRowCnt / iRowSubGrCnt-1));

    if(!iRowSubGrInCnt) iRowSubGrInCnt = 1 ;

    int iGetHeight = imArray -> Height - (5 * (iRowGrCnt-1)) - (5 * (iRowCnt / iRowSubGrInCnt-1));

    int iSetWidth = 0 , iSetHeight = 0 ; // 일단 0으로 초기화를 해줌, 이유는 아직...

    double uGw   = ((double)iGetWidth  / (double)(iColCnt)); // 이미지 안에 들어오는 영역
    double uGh   = ((double)iGetHeight / (double)(iRowCnt));
    int    iWOff = (int)((double)(iGetWidth  - uGw * (iColCnt)) / 2.0); // 의미 없는 0 값
    int    iHOff = (int)((double)(iGetHeight - uGh * (iRowCnt)) / 2.0);
    //int    iWOff = 0;
    //int    iHOff = 0;

    TColor sColor;

    int iGrCountR = 0 , iGrCountC = 0 ; // 0으로 초기화
    int iGrSubCountR = 0 ;

    for (int r = 0 ; r < iRowCnt ; r++) {
        for (int c = 0 ; c < iColCnt ; c++) {                             
            if(iRowSubGrCnt == 1) {
                sColor = 0x00BBFFFF ;

                iGrCountR = (r/iRowGrInCnt < iRowGrCnt) ? (5 * (r/iRowGrInCnt)) : 5 * (iRowGrCnt-1) ;
                iGrCountC = (c/iColGrInCnt < iColGrCnt) ? (5 * (c/iColGrInCnt)) : 5 * (iColGrCnt-1) ;

                uY1 = iHOff + r * uGh       + 1 + iGrCountR;
                uY2 = iHOff + r * uGh + uGh - 1 + iGrCountR;
                uX1 = iWOff + c * uGw       + 1 + iGrCountC;
                uX2 = iWOff + c * uGw + uGw - 1 + iGrCountC;

                imArray -> Canvas -> Brush -> Color = TColor(sColor );
                imArray -> Canvas -> Rectangle(uX1 +1 , uY1 +1 , uX2 -1 , uY2 -1);

                iSetWidth  += uY2;
                iSetHeight += uX2;
            }
            else {
                sColor = 0x00BBFFFF ;

                iGrCountR = (r/iRowGrInCnt < iRowGrCnt) ? (5 * (r/iRowGrInCnt)) : 5 * (iRowGrCnt-1) ;
                iGrCountC = (c/iColGrInCnt < iColGrCnt) ? (5 * (c/iColGrInCnt)) : 5 * (iColGrCnt-1) ;

                iGrSubCountR = (r/iRowSubGrInCnt) ? (5 * (r/iRowSubGrInCnt)) : 0 ;

                uY1 = iHOff + r * uGh       + 1 + iGrCountR + iGrSubCountR;
                uY2 = iHOff + r * uGh + uGh - 1 + iGrCountR + iGrSubCountR;
                uX1 = iWOff + c * uGw       + 1 + iGrCountC;
                uX2 = iWOff + c * uGw + uGw - 1 + iGrCountC;

                imArray -> Canvas -> Brush -> Color = TColor(sColor );
                imArray -> Canvas -> Rectangle(uX1 +1 , uY1 +1 , uX2 -1 , uY2 -1);

                iSetWidth  += uY2; // 아직 정확히 알 수 없는 애
                iSetHeight += uX2;
            }
        }
    }

    imArray -> Canvas -> TextFlags;

  //  lbColCnt   -> Caption = OM.DevInfo.iColCnt  ;
  //  lbRowCnt   -> Caption = OM.DevInfo.iRowCnt  ;
  //  lbColGrCnt -> Caption = OM.DevInfo.iColGrCnt;
  //  lbRowGrCnt -> Caption = OM.DevInfo.iRowGrCnt;
}

void __fastcall TFormDeviceSet::PaintCassetteInfo()
{
    imArray2 -> Picture -> Bitmap = NULL;

    int iRowCnt = 0, iColCnt = 1;
    int uX1, uX2, uY1, uY2;

    TColor sColor;

    //edCsSlotCnt -> Text = 10;
    iRowCnt = OM.DevInfo.iCsSlCnt;

    if(!iRowCnt) iRowCnt = 1; // iRowCnt 값이 0일 때, 10으로 초기화

    //int iGetWidth  = imArray2 -> Width  ;
    //int iGetHeight = imArray2 -> Height ;

    double uGw = (double)imArray2 -> Width  / (double)(iColCnt);
    double uGh = (double)imArray2 -> Height / (double)(iRowCnt);

    for(int r = 0; r < iRowCnt; r++)
    {
        int c = 0;

        sColor = cl3DLight ;

        uY1 = r * uGh       + 1 ;
        uY2 = r * uGh + uGh - 1 ;
        uX1 = c + uGw       ;
        uX2 = c - uGw + uGw ;

        imArray2 -> Canvas -> Brush -> Color = TColor(sColor );
        imArray2 -> Canvas -> Rectangle(uX1, uY1, uX2, uY2   );
     }

    imArray2 -> Canvas -> TextFlags;
}




