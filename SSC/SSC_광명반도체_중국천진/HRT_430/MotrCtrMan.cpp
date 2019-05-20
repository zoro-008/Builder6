//---------------------------------------------------------------------------
#pragma hdrstop

#include "MotrCtrMan.h"
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"

//--------------------------------------
#pragma package(smart_init)

#define MOTRCTR_FONT_SIZE      12
#define MOTRCTR_FONT_CHARSET   ARABIC_CHARSET
#define MOTRCTR_TABFONT_SIZE   10

CMotrCtrMan MCM;
//---------------------------------------------------------------------------
CMotrCtrMan::CMotrCtrMan()
{
//    if(MAX_MOTR != MT_GetMotorCnt()) {ShowMessageT("Motor Count is wrong");}

    memset(&ValTab    , 0 , sizeof(SValTab)*MAX_MOTRTAB );
    memset(&ValMtr    , 0 , sizeof(SValMtr)*MAX_MOTRCTR );

    // 이미지 리스트 0 레프트 1 라이트 2 업 3 다운. 순서대로

    //Motor RadioGroup Name
    ValMtr[tmMOTOR0].sName = "Loader Z"          ;
    ValMtr[tmMOTOR1].sName = "Loader Y"          ;

    ValMtr[tmMOTOR2].sName = "1st Insert X Axis" ;
    ValMtr[tmMOTOR3].sName = "2st Insert X Axis" ;
    ValMtr[tmMOTOR4].sName = "3st Insert X Axis" ;

    ValMtr[tmMOTOR5].sName = "Probe Top"         ;
    ValMtr[tmMOTOR6].sName = "Probe Bottom"      ;

    //Motor Index
    ValMtr[tmMOTOR0].iMtId = (int)miLD1_ZCmp     ;
    ValMtr[tmMOTOR1].iMtId = (int)miLD1_YCmp     ;
    ValMtr[tmMOTOR2].iMtId = (int)miPRB_XCmp     ;
    ValMtr[tmMOTOR3].iMtId = (int)miWRK_XCmp     ;
    ValMtr[tmMOTOR4].iMtId = (int)miPSB_XCmp     ;
    ValMtr[tmMOTOR5].iMtId = (int)miWRK_ZTop     ;
    ValMtr[tmMOTOR6].iMtId = (int)miWRK_ZBtm     ;

    //Motor Jog Disp Type
    ValMtr[tmMOTOR0].iType = mdtUpDown    ;
    ValMtr[tmMOTOR1].iType = mdtBackFor_X ;
    ValMtr[tmMOTOR2].iType = mdtBackFor_X ;
    ValMtr[tmMOTOR3].iType = mdtBackFor_X ;
    ValMtr[tmMOTOR4].iType = mdtBackFor_X ;
    ValMtr[tmMOTOR5].iType = mdtUpDown    ;
    ValMtr[tmMOTOR6].iType = mdtUpDown    ;

    //Motor Tab Count
    ValTab[mtiLDR  ].iMtCnt = 2 ;
    ValTab[mtiRAIL ].iMtCnt = 3 ;
    ValTab[mtiPROBE].iMtCnt = 2 ;

    //Motor Tab Name
    ValTab[mtiLDR  ].sName = "LDR"  ;
    ValTab[mtiRAIL ].sName = "RAIL" ;
    ValTab[mtiPROBE].sName = "PROBE";

}

CMotrCtrMan::~CMotrCtrMan()
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

void CMotrCtrMan::SetWindow(TPanel * _pnBase , TImageList *_ImageList )
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
    m_pJogRadioGroup     = new TRadioGroup (_pnBase) ;
    m_pPageControl       = new TPageControl(_pnBase) ;
    m_pEdit              = new TEdit       (_pnBase) ;

    for(int i=0; i<MAX_MOTRTAB ; i++){
        m_pTabSheet[i] = new TTabSheet   (_pnBase) ;
    }

    for(int i=0; i<MAX_MOTR; i++){
        m_pMtRadioGroup[i]     = new TRadioGroup (_pnBase) ;
        m_pLbCmdCaption[i]     = new TLabel      (_pnBase) ;
        m_pLbEncCaption[i]     = new TLabel      (_pnBase) ;
        m_pLbCmdPos    [i]     = new TLabel      (_pnBase) ;
        m_pLbEncPos    [i]     = new TLabel      (_pnBase) ;
        m_pBwButton    [i]     = new TBitBtn     (_pnBase) ;
        m_pFwButton    [i]     = new TBitBtn     (_pnBase) ;
    }

    for(int i=0; i<MAX_MOTR; i++){
        for(int j=0; j<MAX_LABEL; j++){
            m_pLbStat[i][j] = new TLabel (_pnBase) ;
        }
    }

    //Parent Setting
    m_pJogRadioGroup     -> Parent = _pnBase ;
    m_pPageControl       -> Parent = _pnBase ;
    m_pEdit              -> Parent = _pnBase ;

    m_pPageControl       -> Font -> Size = MOTRCTR_TABFONT_SIZE ;

    int iCount = 0 ;

    for(int i=0; i< MAX_MOTRTAB; i++){
        for(int j=0; j< ValTab[i].iMtCnt; j++){
            m_pMtRadioGroup[iCount] ->Parent = m_pTabSheet[i] ;
            iCount++;
        }
    }

    for(int i=0; i<MAX_MOTR; i++){
        m_pLbCmdCaption[i]     ->Parent = m_pMtRadioGroup[i] ;
        m_pLbEncCaption[i]     ->Parent = m_pMtRadioGroup[i] ;
        m_pLbCmdPos    [i]     ->Parent = m_pMtRadioGroup[i] ;
        m_pLbEncPos    [i]     ->Parent = m_pMtRadioGroup[i] ;
        m_pBwButton    [i]     ->Parent = m_pMtRadioGroup[i] ;
        m_pFwButton    [i]     ->Parent = m_pMtRadioGroup[i] ;
    }

    for(int i=0; i<MAX_MOTR; i++){
        for(int j=0; j<MAX_LABEL; j++){
            m_pLbStat[i][j] ->Parent = m_pMtRadioGroup[i] ;
        }
    }

    //TabSheet PageControl Setting
    for(int i=0; i<MAX_MOTRTAB; i++){
        m_pTabSheet[i] -> PageControl = m_pPageControl ;
    }

    //TabSheet TabMotorRadioGroup Caption Setting
    for(int i=0; i<MAX_MOTRTAB; i++){
        m_pTabSheet[i] -> Caption = ValTab[i].sName;
    }

    for(int i=0; i< MAX_MOTR; i++){
        m_pMtRadioGroup[i] -> Caption = ValMtr[i].sName ;
    }

    //JogRadioGroup Height Cal
    int iWidth  =0 ,                               
        iHeight =0 ,                               
        iLeft   =0 ,
        iLeft1  =0 ,
        iTop    =0 ,
        iTop1   =0 ;
    iHeight = _pnBase -> Height / 7.36 ;
    m_pJogRadioGroup  -> Height = iHeight;
    m_pJogRadioGroup  -> Align  = alTop ;

    //JogRadioGroup Width , Height Cal
    m_pJogRadioGroup -> Caption = "JOG UNIT" ;
    m_pJogRadioGroup -> Columns = 2 ;
    m_pJogRadioGroup -> Font -> Charset = MOTRCTR_FONT_CHARSET ;
    m_pJogRadioGroup -> Font -> Size    = MOTRCTR_FONT_SIZE    ;

    m_pJogRadioGroup -> Items->Add("JOG");
    m_pJogRadioGroup -> Items->Add("1mm");
    m_pJogRadioGroup -> Items->Add("0.5mm");
    m_pJogRadioGroup -> Items->Add("0.1mm");
    m_pJogRadioGroup -> Items->Add("0.05mm");
    m_pJogRadioGroup -> Items->Add("             mm");

    m_pJogRadioGroup -> ItemIndex = 0 ;

    iWidth  = _pnBase ->Width  / 5     ;
    iHeight = _pnBase ->Height / 30.3  ;
    iLeft   = _pnBase ->Width  / 1.71  ;  //이놈의 쉐끼 판넬 width 넓히면 제대로 안맞음...아나...
    iTop    = _pnBase ->Height / 10.35 ;

    m_pEdit       ->AutoSize = false  ;
    m_pEdit->Font ->Charset  = MOTRCTR_FONT_CHARSET ;
    m_pEdit->Font ->Size     = MOTRCTR_FONT_SIZE    ;
    m_pEdit       ->Text     = "0.01"  ;
    m_pEdit       ->Width    = iWidth  ;
    m_pEdit       ->Height   = iHeight ;
    m_pEdit       ->Left     = iLeft   ;
    m_pEdit       ->Top      = iTop    ;

    iHeight = _pnBase -> Height / 1.17 ;
    m_pPageControl->Height = iHeight ;
    m_pPageControl->Align  = alBottom ;

    iWidth  = m_pTabSheet[0]-> Width - 10 ;
    iHeight = m_pTabSheet[0]-> Height / 4 ;
    for(int i=0; i< MAX_MOTR; i++){
        m_pMtRadioGroup[i] -> Width  = iWidth  ;
        m_pMtRadioGroup[i] -> Height = iHeight ;
        m_pMtRadioGroup[i] -> Align  = alTop   ;
    }

    //Label Stat Postion Setting
    iWidth  = (m_pMtRadioGroup[0]->Width - 30) / 7 ;
    iHeight =  m_pMtRadioGroup[0]->Height      / 8 ;
    for(int i=0; i<MAX_MOTR; i++){
        for(int j=0; j<MAX_LABEL; j++){
            switch(j){
                case 0 : m_pLbStat[i][j] -> Font -> Size = MOTRCTR_FONT_SIZE ;
                         m_pLbStat[i][j] -> Alignment = taCenter ;
                         m_pLbStat[i][j] -> Caption = "-L";
                         m_pLbStat[i][j] ->Color = clSilver ;
                         break;
                case 1 : m_pLbStat[i][j] -> Font -> Size = MOTRCTR_FONT_SIZE ;
                         m_pLbStat[i][j] -> Alignment = taCenter ;
                         m_pLbStat[i][j] ->Caption = "HM";
                         m_pLbStat[i][j] ->Color = clSilver ;
                         break;
                case 2 : m_pLbStat[i][j] -> Font -> Size = MOTRCTR_FONT_SIZE ;
                         m_pLbStat[i][j] -> Alignment = taCenter ;
                         m_pLbStat[i][j] ->Caption = "+L";
                         m_pLbStat[i][j] ->Color = clSilver ;
                         break;
                case 3 : m_pLbStat[i][j] -> Font -> Size = MOTRCTR_FONT_SIZE ;
                         m_pLbStat[i][j] -> Alignment = taCenter ;
                         m_pLbStat[i][j] ->Caption = "AR";
                         m_pLbStat[i][j] ->Color = clSilver ;
                         break;
                case 4 : m_pLbStat[i][j] -> Font -> Size = MOTRCTR_FONT_SIZE ;
                         m_pLbStat[i][j] -> Alignment = taCenter ;
                         m_pLbStat[i][j] ->Caption = "SV";
                         m_pLbStat[i][j] ->Color = clSilver ;
                         break;
                case 5 : m_pLbStat[i][j] -> Font -> Size = MOTRCTR_FONT_SIZE ;
                         m_pLbStat[i][j] -> Alignment = taCenter ;
                         m_pLbStat[i][j] ->Caption = "ST";
                         m_pLbStat[i][j] ->Color = clSilver ;
                         break;
                case 6 : m_pLbStat[i][j] -> Font -> Size = MOTRCTR_FONT_SIZE ;
                         m_pLbStat[i][j] -> Alignment = taCenter ;
                         m_pLbStat[i][j] ->Caption = "HD";
                         m_pLbStat[i][j] ->Color = clSilver ;
                         break;
            }
            m_pLbStat[i][j] -> Tag   = i ;
            m_pLbStat[i][j] ->Width  = iWidth   ;
            m_pLbStat[i][j] ->Height = iHeight  ;
            m_pLbStat[i][j] ->Left   = 10 + (iWidth + 2)*j ;
            m_pLbStat[i][j] ->Top    = 20 ;
        }
    }

    iWidth  = m_pMtRadioGroup[0]->Width  / 3   ;
    iHeight = m_pMtRadioGroup[0]->Height / 8.2 ;
    iTop    = m_pMtRadioGroup[0]->Height / 3   ;
    iTop1   = m_pMtRadioGroup[0]->Height / 2   ;
    for(int i=0; i<MAX_MOTR; i++){
        m_pLbCmdCaption[i] -> Font -> Size = MOTRCTR_FONT_SIZE ;
        m_pLbCmdCaption[i] -> Caption = "CMD POS" ;
        m_pLbCmdCaption[i] -> Width  = iWidth  ;
        m_pLbCmdCaption[i] -> Height = iHeight ;
        m_pLbCmdCaption[i] -> Left   = 5       ;
        m_pLbCmdCaption[i] -> Top    = iTop    ;

        m_pLbEncCaption[i] -> Font -> Size = MOTRCTR_FONT_SIZE ;
        m_pLbEncCaption[i] -> Caption = "ENC POS" ;
        m_pLbEncCaption[i] -> Width  = iWidth  ;
        m_pLbEncCaption[i] -> Height = iHeight ;
        m_pLbEncCaption[i] -> Left   = 5       ;
        m_pLbEncCaption[i] -> Top    = iTop1   ;
    }

    iLeft   = m_pMtRadioGroup[0]->Width  / 2.5 ;
    iWidth  = m_pMtRadioGroup[0]->Width  / 1.7 ;

    for(int i=0; i<MAX_MOTR; i++){
        m_pLbCmdPos[i] -> Font -> Size = MOTRCTR_FONT_SIZE ;
        m_pLbCmdPos[i] -> AutoSize  = false    ;
        m_pLbCmdPos[i] -> Alignment = taCenter ;
        m_pLbCmdPos[i] -> Caption = "0.0";
        m_pLbCmdPos[i] -> Color   = clSilver ;
        m_pLbCmdPos[i] -> Left = iLeft ;
        m_pLbCmdPos[i] -> Top  = iTop  ;
        m_pLbCmdPos[i] -> Width  = iWidth ;
        m_pLbCmdPos[i] -> Height = iHeight;
        m_pLbCmdPos[i] -> Tag    = i;

        m_pLbEncPos[i] -> Font -> Size = MOTRCTR_FONT_SIZE ;
        m_pLbEncPos[i] -> AutoSize  = false    ;
        m_pLbEncPos[i] -> Alignment = taCenter ;
        m_pLbEncPos[i] -> Caption = "0.0";
        m_pLbEncPos[i] -> Color   = clSilver ;
        m_pLbEncPos[i] -> Left = iLeft ;
        m_pLbEncPos[i] -> Top  = iTop1 ;
        m_pLbEncPos[i] -> Width  = iWidth ;
        m_pLbEncPos[i] -> Height = iHeight;
        m_pLbEncPos[i] -> Tag    = i;

    }

    iWidth  = (m_pMtRadioGroup[0]->Width  / 2 )- 5 ;
    iHeight =  m_pMtRadioGroup[0]->Height / 3.6 ;
    iLeft   = 5                                ;
    iLeft1  = (m_pMtRadioGroup[0]->Width  / 2 ) + 5  ;
    iTop    =  m_pMtRadioGroup[0]->Height / 1.5      ;
//    iTop1   =  m_pMtRadioGroup[0]->Height / 2   ;

//    iCount = 0 ;

//    for(int i=0; i< MAX_MOTRTAB; i++){
        for(int j=0; j< MAX_MOTRCTR; j++){
            switch(ValMtr[j].iType){
                case mdtBackFor_X : m_pBwButton[j] -> Caption = "Bw";
                                    m_pFwButton[j] -> Caption = "Fw";
                                    m_pBwButton[j] -> OnMouseDown = btMtBw;
                                    m_pFwButton[j] -> OnMouseDown = btMtFw;
                                    m_pBwButton[j] -> OnMouseUp   = btMtUp;
                                    m_pFwButton[j] -> OnMouseUp   = btMtUp;

                                    m_pBwButton[j] -> Glyph = m_pBwimage ;
                                    m_pFwButton[j] -> Glyph = m_pFwimage ;

                break ;
                case mdtForBack_X : m_pBwButton[j] -> Caption = "Fw";
                                    m_pFwButton[j] -> Caption = "Bw";
                                    m_pBwButton[j] -> OnMouseDown = btMtFw;
                                    m_pFwButton[j] -> OnMouseDown = btMtBw;
                                    m_pBwButton[j] -> OnMouseUp   = btMtUp;
                                    m_pFwButton[j] -> OnMouseUp   = btMtUp;

                                    m_pBwButton[j] -> Glyph = m_pFwimage ;
                                    m_pFwButton[j] -> Glyph = m_pBwimage ;

                break ;
                case mdtDownUp :    m_pBwButton[j] -> Caption = "Dn";
                                    m_pFwButton[j] -> Caption = "Up";
                                    m_pBwButton[j] -> OnMouseDown = btMtBw;
                                    m_pFwButton[j] -> OnMouseDown = btMtFw;
                                    m_pBwButton[j] -> OnMouseUp   = btMtUp;
                                    m_pFwButton[j] -> OnMouseUp   = btMtUp;

                                    m_pBwButton[j] -> Glyph = m_pDnimage ;
                                    m_pFwButton[j] -> Glyph = m_pUpimage ;

                break ;
                case mdtUpDown :    m_pBwButton[j] -> Caption = "Up";
                                    m_pFwButton[j] -> Caption = "Dn";
                                    m_pBwButton[j] -> OnMouseDown = btMtFw;
                                    m_pFwButton[j] -> OnMouseDown = btMtBw;
                                    m_pBwButton[j] -> OnMouseUp   = btMtUp;
                                    m_pFwButton[j] -> OnMouseUp   = btMtUp;

                                    m_pBwButton[j] -> Glyph = m_pUpimage ;
                                    m_pFwButton[j] -> Glyph = m_pDnimage ;

                break ;
                case mdtBackFor_Y : m_pBwButton[j] -> Caption = "Bw";
                                    m_pFwButton[j] -> Caption = "Fw";
                                    m_pBwButton[j] -> OnMouseDown = btMtBw;
                                    m_pFwButton[j] -> OnMouseDown = btMtFw;
                                    m_pBwButton[j] -> OnMouseUp   = btMtUp;
                                    m_pFwButton[j] -> OnMouseUp   = btMtUp;

                                    m_pBwButton[j] -> Glyph = m_pUpimage ;
                                    m_pFwButton[j] -> Glyph = m_pDnimage ;

                break ;
                case mdtForBack_Y : m_pBwButton[j] -> Caption = "Fw";
                                    m_pFwButton[j] -> Caption = "Bw";
                                    m_pBwButton[j] -> OnMouseDown = btMtFw;
                                    m_pFwButton[j] -> OnMouseDown = btMtBw;
                                    m_pBwButton[j] -> OnMouseUp   = btMtUp;
                                    m_pFwButton[j] -> OnMouseUp   = btMtUp;

                                    m_pBwButton[j] -> Glyph = m_pUpimage ;
                                    m_pFwButton[j] -> Glyph = m_pDnimage ;

                break ;
                default        :    m_pBwButton[j] -> Caption = "Bw";
                                    m_pFwButton[j] -> Caption = "Fw";
                                    m_pBwButton[j] -> OnMouseDown = btMtBw;
                                    m_pFwButton[j] -> OnMouseDown = btMtFw;
                                    m_pBwButton[j] -> OnMouseUp   = btMtUp;
                                    m_pFwButton[j] -> OnMouseUp   = btMtUp;

                                    m_pBwButton[j] -> Glyph = m_pBwimage ;
                                    m_pFwButton[j] -> Glyph = m_pFwimage ;

                break ;
            }
//            iCount++;
        }
//    }

    for(int i=0; i<MAX_MOTR; i++){
        m_pBwButton[i] -> Font -> Size = MOTRCTR_FONT_SIZE ;
        m_pBwButton[i] -> Left    = iLeft  ;
        m_pBwButton[i] -> Top     = iTop   ;
        m_pBwButton[i] -> Width   = iWidth ;
        m_pBwButton[i] -> Height  = iHeight;
        m_pBwButton[i] -> Tag     = i      ;

        m_pFwButton[i] -> Font -> Size = MOTRCTR_FONT_SIZE ;
        m_pFwButton[i] -> Left    = iLeft1 ;
        m_pFwButton[i] -> Top     = iTop   ;
        m_pFwButton[i] -> Width   = iWidth ;
        m_pFwButton[i] -> Height  = iHeight;
        m_pFwButton[i] -> Tag     = i      ;        
    }

    tmUpdate = new TTimer(Application);
    tmUpdate -> Interval = 100  ;
    tmUpdate -> OnTimer  = tmUpdateTimer ;
    tmUpdate -> Enabled  = true ;


}

void __fastcall CMotrCtrMan::btMtBw(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int iSel  = ((TBitBtn *)Sender) -> Tag ;

    iSel = ValMtr[iSel].iMtId ;

    bool   bJog  = false ;
    double dUnit = 0.0 ;

    switch(m_pJogRadioGroup -> ItemIndex) {
        case 0 : dUnit = 0.0                           ;
                 bJog  = true                          ; break ;
        case 1 : dUnit = 1                             ; break ;
        case 2 : dUnit = 0.5                           ; break ;
        case 3 : dUnit = 0.1                           ; break ;
        case 4 : dUnit = 0.05                          ; break ;
        case 5 : dUnit = m_pEdit -> Text.ToDouble()    ; break ;
    }

    if(bJog) {
        MT_JogN(iSel) ;
    }
    else {
        MT_GoIncMan(iSel , -dUnit) ;
    }
}
void __fastcall CMotrCtrMan::btMtFw(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int iSel  = ((TBitBtn *)Sender) -> Tag ;

    iSel = ValMtr[iSel].iMtId;

    bool   bJog  = false ;
    double dUnit = 0.0 ;

    switch(m_pJogRadioGroup -> ItemIndex) {
        case 0 : dUnit = 0.0                           ;
                 bJog  = true                          ; break ;
        case 1 : dUnit = 1                             ; break ;
        case 2 : dUnit = 0.5                           ; break ;
        case 3 : dUnit = 0.1                           ; break ;
        case 4 : dUnit = 0.05                          ; break ;
        case 5 : dUnit = m_pEdit -> Text.ToDouble()    ; break ;
    }

    if(bJog) {
        MT_JogN(iSel) ;
    }
    else {
        MT_GoIncMan(iSel , -dUnit) ;
    }
}
void __fastcall CMotrCtrMan::btMtUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int iSel  = ((TBitBtn *)Sender) -> Tag ;

    iSel = ValMtr[iSel].iMtId ;

    if(m_pJogRadioGroup -> ItemIndex == 0) MT_Stop(iSel) ;
}
void CMotrCtrMan::SetTabIndex(int iIndex)
{
    if(iIndex > MAX_MOTRTAB-1 || iIndex < 0 ) return ;
    m_pPageControl -> TabIndex = iIndex ;
}
void __fastcall CMotrCtrMan::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    for(int i=0; i<MAX_MOTR; i++){

        m_pLbStat[i][0] -> Color  = MT_GetNLimSnsr (ValMtr[i].iMtId) ? clLime : clSilver ;
        m_pLbStat[i][1] -> Color  = MT_GetHomeEnd  (ValMtr[i].iMtId) ? clLime : clSilver ;
        m_pLbStat[i][2] -> Color  = MT_GetPLimSnsr (ValMtr[i].iMtId) ? clLime : clSilver ;
        m_pLbStat[i][3] -> Color  = MT_GetAlarm    (ValMtr[i].iMtId) ? clLime : clSilver ;
        m_pLbStat[i][4] -> Color  = MT_GetServo    (ValMtr[i].iMtId) ? clLime : clSilver ;
        m_pLbStat[i][5] -> Color  = MT_GetStopInpos(ValMtr[i].iMtId) ? clLime : clSilver ;
        m_pLbStat[i][6] -> Color  = MT_GetHomeEnd  (ValMtr[i].iMtId) ? clLime : clSilver ;

        m_pLbCmdPos[i] -> Caption = MT_GetCmdPos   (ValMtr[i].iMtId) ;
        m_pLbEncPos[i] -> Caption = MT_GetEncPos   (ValMtr[i].iMtId) ;
    }

    tmUpdate -> Enabled = true ;
}
