//---------------------------------------------------------------------------
#pragma hdrstop

#include "ActCtrMan.h"
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"

//--------------------------------------
#include "Loader.h"
#include "PreBuff.h"
#include "WorkZone.h"
#include "PostBuff.h"
//--------------------------------------
#pragma package(smart_init)

#define ACTCTR_FONT_SIZE      12
#define ACTCTR_FONT_CHARSET   ARABIC_CHARSET
#define ACTCTR_TABFONT_SIZE   10
#define ACTCTR_COLCNT         10
#define ACTCTR_ROWCNT         1

CActCtrMan ACM;
//---------------------------------------------------------------------------
CActCtrMan::CActCtrMan()
{
//    if(MAX_MOTR != MT_GetMotorCnt()) {ShowMessageT("Motor Count is wrong");}

    memset(&ValTab    , 0 , sizeof(SValTab)*MAX_ACTTAB );
    memset(&ValACT    , 0 , sizeof(SValACT)*MAX_ACTCTR );

    //ACTuator RadioGroup Name 미얀하지만 순서대로 넣어주세요.
    ValACT[tmACT0 ].sName = "MGZ\rCLAMP"                  ;
    ValACT[tmACT1 ].sName = "Loader Pusher\rPusher"       ;
    ValACT[tmACT2 ].sName = "AC MOTOR\rIN BELT"           ;
    ValACT[tmACT3 ].sName = "AC MOTOR\rOUT BELT"          ;

    ValACT[tmACT4 ].sName = "Pre Buffer\rBELT"            ;
    ValACT[tmACT5 ].sName = "Pre Buffer\r1st CLAMP"       ;
    ValACT[tmACT6 ].sName = "Pre Buffer\rID_CHECK"        ;
    ValACT[tmACT7 ].sName = "Work Zone\r2st CLAMP"        ;
    ValACT[tmACT8 ].sName = "Post Buffer\r3st CLAMP"      ;
    ValACT[tmACT9 ].sName = "Post Buffer\rBELT"           ;

    ValACT[tmACT10].sName = "Sorting Tool\rYTop CLAMP"    ;
    ValACT[tmACT11].sName = "Sorting Tool\rYBottom CLAMP" ;

    ValACT[tmACT12].sName = "AC MOTOR\rIN BELT"           ;
    ValACT[tmACT13].sName = "AC MOTOR\rOUT BELT"          ;
    ValACT[tmACT14].sName = "UnLoader \rPusher"           ;
    ValACT[tmACT15].sName = "UnLoader \rCLAMP"            ;

    //ACTuator Index                               //ACTuator Row,Col
    ValACT[tmACT0 ].iACTId = (int)aiLD1_Cmp       ; ValACT[tmACT0 ].iRow = 0 ; ValACT[tmACT0 ].iCol = 0 ;
    ValACT[tmACT1 ].iACTId = (int)aiLD1_Psh       ; ValACT[tmACT1 ].iRow = 0 ; ValACT[tmACT1 ].iCol = 1 ;
    ValACT[tmACT2 ].iACTId = (int)yLD1_MgzInAC    ; ValACT[tmACT2 ].iRow = 0 ; ValACT[tmACT2 ].iCol = 1 ;
    ValACT[tmACT3 ].iACTId = (int)yLD1_MgzOutAC   ; ValACT[tmACT3 ].iRow = 0 ; ValACT[tmACT3 ].iCol = 2 ;

    ValACT[tmACT4 ].iACTId = (int)yPRB_FeedingAC  ; ValACT[tmACT4 ].iRow = 0 ; ValACT[tmACT4 ].iCol = 0 ;
    ValACT[tmACT5 ].iACTId = (int)aiPRB_Cmp       ; ValACT[tmACT5 ].iRow = 0 ; ValACT[tmACT5 ].iCol = 1 ;
    ValACT[tmACT6 ].iACTId = (int)aiPRB_IdChk     ; ValACT[tmACT6 ].iRow = 0 ; ValACT[tmACT6 ].iCol = 2 ;
    ValACT[tmACT7 ].iACTId = (int)aiWRK_Cmp       ; ValACT[tmACT7 ].iRow = 0 ; ValACT[tmACT7 ].iCol = 3 ;
    ValACT[tmACT8 ].iACTId = (int)aiPSB_Cmp       ; ValACT[tmACT8 ].iRow = 0 ; ValACT[tmACT8 ].iCol = 4 ;
    ValACT[tmACT9 ].iACTId = (int)yPSB_FeedingAC  ; ValACT[tmACT9 ].iRow = 0 ; ValACT[tmACT9 ].iCol = 5 ;

    ValACT[tmACT10].iACTId = (int)aiSTL_Top       ; ValACT[tmACT10].iRow = 0 ; ValACT[tmACT10].iCol = 0 ;
    ValACT[tmACT11].iACTId = (int)aiSTL_Btm       ; ValACT[tmACT11].iRow = 0 ; ValACT[tmACT11].iCol = 1 ;

    ValACT[tmACT12].iACTId = (int)yULD_MgzInAC    ; ValACT[tmACT12].iRow = 0 ; ValACT[tmACT12].iCol = 0 ;
    ValACT[tmACT13].iACTId = (int)yULD_MgzOutAC   ; ValACT[tmACT13].iRow = 0 ; ValACT[tmACT13].iCol = 1 ;
    ValACT[tmACT14].iACTId = (int)aiPSB_Psh       ; ValACT[tmACT14].iRow = 0 ; ValACT[tmACT14].iCol = 2 ;
    ValACT[tmACT15].iACTId = (int)aiULD_Cmp       ; ValACT[tmACT15].iRow = 0 ; ValACT[tmACT15].iCol = 3 ;

    //ACTinder Jog Disp Type
    ValACT[tmACT0 ].iType = cdtUpDown  ;
    ValACT[tmACT1 ].iType = cdtForBack ;
    ValACT[tmACT2 ].iType = cdtOnOff   ;
    ValACT[tmACT3 ].iType = cdtOnOff   ;

    ValACT[tmACT4 ].iType = cdtOnOff   ;
    ValACT[tmACT5 ].iType = cdtUpDown  ;
    ValACT[tmACT6 ].iType = cdtUpDown  ;
    ValACT[tmACT7 ].iType = cdtUpDown  ;
    ValACT[tmACT8 ].iType = cdtUpDown  ;
    ValACT[tmACT9 ].iType = cdtOnOff   ;

    ValACT[tmACT10].iType = cdtUpDown  ;
    ValACT[tmACT11].iType = cdtUpDown  ;

    ValACT[tmACT12].iType = cdtOnOff   ;
    ValACT[tmACT13].iType = cdtOnOff   ;
    ValACT[tmACT14].iType = cdtForBack ;
    ValACT[tmACT15].iType = cdtUpDown  ;

    //ACT Tab Count
    ValTab[ctiLDR  ].iACTCnt = 4 ;
    ValTab[ctiRAIL ].iACTCnt = 6 ;
    ValTab[ctiSORT ].iACTCnt = 2 ;
    ValTab[ctiULD  ].iACTCnt = 4 ;
//    ValTab[ctiPROBE].iACTCnt = 2 ;

    ValTab[ctiLDR  ].sName = "LDR"  ;
    ValTab[ctiRAIL ].sName = "RAIL" ;
    ValTab[ctiSORT ].sName = "SORT" ;
    ValTab[ctiULD  ].sName = "ULD"  ;
//    ValTab[ctiPROBE].sName = "PROBE";

}

CActCtrMan::~CActCtrMan()
{
    delete m_pFwimage ; m_pFwimage = NULL ;
    delete m_pBwimage ; m_pBwimage = NULL ;
    delete m_pUpimage ; m_pUpimage = NULL ;
    delete m_pDnimage ; m_pDnimage = NULL ;

    //Create
//    if(m_pJogRadioGroup) delete m_pJogRadioGroup ;
//    if(m_pPageControl  ) delete m_pPageControl   ;
//    if(m_pEdit         ) delete m_pEdit          ;
//
//    for(int i=0; i<MAX_TAB ; i++){
//        if(m_pTabSheet[i] ) delete m_pTabSheet[i] ;
//    }
//
//    for(int i=0; i<MAX_MOTR; i++){
//        if(m_pMtRadioGroup[i] )delete m_pMtRadioGroup[i];
//        if(m_pLbCmdCaption[i] )delete m_pLbCmdCaption[i];
//        if(m_pLbEncCaption[i] )delete m_pLbEncCaption[i];
//        if(m_pLbCmdPos    [i] )delete m_pLbCmdPos    [i];
//        if(m_pLbEncPos    [i] )delete m_pLbEncPos    [i];
//        if(m_pBwButton    [i] )delete m_pBwButton    [i];
//        if(m_pFwButton    [i] )delete m_pFwButton    [i];
//    }
//
//    for(int i=0; i<MAX_MOTR; i++){
//        for(int j=0; j<MAX_LABEL; j++){
//            if(m_pLbStat[i][j] ) delete m_pLbStat[i][j];
//        }
//    }
}

void CActCtrMan::SetWindow(TPanel * _pnBase , TImageList *_ImageList )
{
//    if(_iAxisNo >= MAX_MOTR) {ShowMessageT("Motor Count is wrong"); return ; }

//    const int iEditW = 52 ;
//    const int iEditH = 27 ;

    m_pFwimage           = new Graphics::TBitmap()   ; 
    m_pBwimage           = new Graphics::TBitmap()   ; 
    m_pUpimage           = new Graphics::TBitmap()   ; 
    m_pDnimage           = new Graphics::TBitmap()   ;

    _ImageList -> GetBitmap(0,m_pBwimage);
    _ImageList -> GetBitmap(1,m_pFwimage);
    _ImageList -> GetBitmap(2,m_pUpimage);
    _ImageList -> GetBitmap(3,m_pDnimage);

    //Create
    m_pPageControl       = new TPageControl(_pnBase) ;

    for(int i=0; i<MAX_ACTTAB ; i++){
        m_pTabSheet[i] = new TTabSheet   (_pnBase) ;
    }

    for(int i=0; i<MAX_ACTCTR; i++){
        m_pPanel       [i]     = new TPanel      (_pnBase) ;
        m_pStaticText  [i]     = new TStaticText (_pnBase) ;
        m_pBwButton    [i]     = new TBitBtn     (_pnBase) ;
        m_pFwButton    [i]     = new TBitBtn     (_pnBase) ;
    }

    //Parent Setting
    m_pPageControl       -> Font -> Size    = ACTCTR_TABFONT_SIZE ;
    m_pPageControl       -> Font -> Charset = ACTCTR_FONT_CHARSET ;
    m_pPageControl       -> Parent = _pnBase ;
    m_pPageControl       -> Align  = alClient ;

    int iCount = 0 ;

    for(int i=0; i< MAX_ACTTAB; i++){
        for(int j=0; j< ValTab[i].iACTCnt; j++){
            m_pPanel[iCount] ->Parent = m_pTabSheet[i] ;
//            m_pStaticText[iCount] ->Parent = m_pTabSheet[i] ;
//            m_pBwButton  [iCount] ->Parent = m_pTabSheet[i] ;
//            m_pFwButton  [iCount] ->Parent = m_pTabSheet[i] ;
            iCount++;
        }
    }

    for(int i=0; i< MAX_ACTCTR; i++){
        m_pStaticText[i] ->Parent = m_pPanel[i];
        m_pBwButton  [i] ->Parent = m_pPanel[i];
        m_pFwButton  [i] ->Parent = m_pPanel[i];
    }

    //TabSheet PageControl Setting
    for(int i=0; i<MAX_ACTTAB; i++){
        m_pTabSheet[i] -> PageControl = m_pPageControl ;
    }

    //Caption Setting
    for(int i=0; i<MAX_ACTTAB; i++){
        m_pTabSheet[i] -> Caption = ValTab[i].sName;
    }

    for(int i=0; i< MAX_ACTCTR; i++){
        m_pStaticText[i] -> Font -> Charset = ACTCTR_FONT_CHARSET ;
        m_pStaticText[i] -> Font -> Size    = ACTCTR_FONT_SIZE    ;
        m_pStaticText[i] -> Caption = ValACT[i].sName ;
        m_pStaticText[i] -> BevelKind = bkFlat   ;
        m_pStaticText[i] -> AutoSize  = false    ;
        m_pStaticText[i] -> Alignment = taCenter ;
    }

    //Position Setting
    int iWidth  =0 ,
        iHeight =0 ,
        iLeft   =0 ,
        iLeft1  =0 ,
        iTop    =0 ,
        iTop1   =0 ;

    iWidth  = m_pTabSheet[0] -> Width  / ACTCTR_COLCNT ;
    iHeight = m_pTabSheet[0] -> Height / ACTCTR_ROWCNT ;

    for(int i=0; i< MAX_ACTCTR; i++){
        m_pPanel[i]->Width  = iWidth ;
        m_pPanel[i]->Height = iHeight;
        m_pPanel[i]->Left   = ValACT[i].iCol * iWidth ;
        m_pPanel[i]->Top    = ValACT[i].iRow * iHeight;
    }

    for(int i=0; i< MAX_ACTCTR; i++){
        m_pStaticText[i] ->Width  = iWidth  - 4   ;
        m_pStaticText[i] ->Height = iHeight / 3.5 ;
        m_pStaticText[i] ->Left   = 2 ;
        m_pStaticText[i] ->Top    = 2 ;

        m_pBwButton  [i] ->Width  = iWidth  - 4   ;
        m_pBwButton  [i] ->Height = iHeight / 3.1 ;
        m_pBwButton  [i] ->Left   = 2 ;
        m_pBwButton  [i] ->Top    = m_pStaticText[i] ->Height + 4 ;

        m_pFwButton  [i] ->Width  = iWidth  - 4   ;
        m_pFwButton  [i] ->Height = iHeight / 3.1 ;
        m_pFwButton  [i] ->Left   = 2 ;
        m_pFwButton  [i] ->Top    = m_pStaticText[i] ->Height + 4 + m_pBwButton[i] ->Height + 2 ;
    }

//    iCount = 0 ;
//    for(int i=0; i< MAX_ACTTAB; i++){
        for(int j=0; j< MAX_ACTCTR; j++){
            switch(ValACT[j].iType){
                case cdtBackFor: m_pBwButton[j] -> Caption = "Bw";
                                 m_pFwButton[j] -> Caption = "Fw";
                                 m_pBwButton[j] -> OnClick = btBwClick;
                                 m_pFwButton[j] -> OnClick = btFwClick;

                                 m_pBwButton[j] -> Glyph = m_pBwimage ;
                                 m_pFwButton[j] -> Glyph = m_pFwimage ;

                break ;
                case cdtForBack: m_pBwButton[j] -> Caption = "Fw";
                                 m_pFwButton[j] -> Caption = "Bw";
                                 m_pBwButton[j] -> OnClick = btFwClick;
                                 m_pFwButton[j] -> OnClick = btBwClick;

                                 m_pBwButton[j] -> Glyph = m_pFwimage ;
                                 m_pFwButton[j] -> Glyph = m_pBwimage ;

                break ;
                case cdtDownUp : m_pBwButton[j] -> Caption = "Dn";
                                 m_pFwButton[j] -> Caption = "Up";
                                 m_pBwButton[j] -> OnClick = btBwClick;
                                 m_pFwButton[j] -> OnClick = btFwClick;

                                 m_pBwButton[j] -> Glyph = m_pDnimage ;
                                 m_pFwButton[j] -> Glyph = m_pUpimage ;

                break ;
                case cdtUpDown : m_pBwButton[j] -> Caption = "Up";
                                 m_pFwButton[j] -> Caption = "Dn";
                                 m_pBwButton[j] -> OnClick = btFwClick;
                                 m_pFwButton[j] -> OnClick = btBwClick;

                                 m_pBwButton[j] -> Glyph = m_pUpimage ;
                                 m_pFwButton[j] -> Glyph = m_pDnimage ;
                break ;
                case cdtOnOff  : m_pBwButton[j] -> Caption = "On";
                                 m_pFwButton[j] -> Caption = "Off";
                                 m_pBwButton[j] -> OnClick = btOnClick;
                                 m_pFwButton[j] -> OnClick = btOffClick;

//                                 m_pBwButton[j] -> Glyph = m_pUpimage ;
//                                 m_pFwButton[j] -> Glyph = m_pDnimage ;


                break ;
                default        : m_pBwButton[j] -> Caption = "Bw";
                                 m_pFwButton[j] -> Caption = "Fw";
                                 m_pBwButton[j] -> OnClick = btBwClick;
                                 m_pFwButton[j] -> OnClick = btFwClick;

                                 m_pBwButton[j] -> Glyph = m_pBwimage ;
                                 m_pFwButton[j] -> Glyph = m_pFwimage ;

                break ;
            }
//            iCount++;
//        }
    }

    for(int i=0; i<MAX_MOTR; i++){
        m_pBwButton[i] -> Font -> Charset = ACTCTR_FONT_CHARSET ;
        m_pBwButton[i] -> Font -> Size    = ACTCTR_FONT_SIZE ;
//        m_pBwButton[i] -> Font -> Style   = m_pBwButton[i] -> Font -> Style << fsBold;
        m_pBwButton[i] -> Tag             = ValACT[i].iACTId  ;

        m_pFwButton[i] -> Font -> Charset = ACTCTR_FONT_CHARSET ;
        m_pFwButton[i] -> Font -> Size  = ACTCTR_FONT_SIZE ;
//        m_pFwButton[i] -> Font -> Style = m_pFwButton[i] -> Font -> Style << fsBold;
        m_pFwButton[i] -> Tag     = ValACT[i].iACTId  ;
    }

    tmUpdate = new TTimer(Application);
    tmUpdate -> Interval = 100  ;
    tmUpdate -> OnTimer  = tmUpdateTimer ;
    tmUpdate -> Enabled  = true ;


}
void __fastcall CActCtrMan::btOnClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;

    IO_SetY(iSel , true);
}
void __fastcall CActCtrMan::btOffClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;

    IO_SetY(iSel , false);
}
void __fastcall CActCtrMan::btFwClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;

    bool bRet ;

    if(iSel == aiLD1_Cmp      )  if(IO_GetX(xLD1_MgzDetect1) || IO_GetX(xLD1_MgzDetect2)){
                                     if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Open the Clamp ?") != mrYes ) return ;
                                 }
                                 bRet = LD1.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;

    if(iSel == aiPRB_Cmp      ||
       iSel == aiPRB_IdChk    )  bRet = PRB.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;


    if(iSel == aiWRK_Cmp      )  bRet = WRK.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;

    if(iSel == aiPSB_Cmp      )  bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;

//    if(iSel == aiULD_Cmp      )  bRet = ULD.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;

    if(!bRet) return ;

    AT_MoveCyl(iSel , ccFwd) ;
}
void __fastcall CActCtrMan::btBwClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;

    bool bRet ;

    if(iSel == aiLD1_Cmp      )  bRet = LD1.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;

    if(iSel == aiPRB_Cmp      ||
       iSel == aiPRB_IdChk    )  bRet = PRB.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;


    if(iSel == aiWRK_Cmp      )  bRet = WRK.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;

    if(iSel == aiPSB_Cmp      )  bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;

//    if(iSel == aiULD_Cmp      )  bRet = ULD.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;

    if(!bRet) return ;

    AT_MoveCyl(iSel , ccBwd) ;
}

void CActCtrMan::SetTabIndex(int iIndex)
{
    if(iIndex > MAX_ACTTAB-1 || iIndex < 0 ) return ;
    m_pPageControl -> TabIndex = iIndex ;
}
void __fastcall CActCtrMan::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    for(int i=0; i<MAX_ACTCTR; i++){
        if(ValACT[i].iType == cdtOnOff ) {
            m_pBwButton[i] -> Font->Color   = IO_GetY(ValACT[i].iACTId) ? clLime : clBlack ;
//            m_pFwButton[i] ->
        }
        else {
            m_pBwButton[i] ->Font->Color = AT_Complete(ValACT[i].iACTId ,ccBwd) ? clLime : clBlack ;
            m_pFwButton[i] ->Font->Color = AT_Complete(ValACT[i].iACTId ,ccFwd) ? clLime : clBlack ;
        }
    }

    tmUpdate -> Enabled = true ;
}
