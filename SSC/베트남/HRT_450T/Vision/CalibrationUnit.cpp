//---------------------------------------------------------------------------


#pragma hdrstop

#include "CalibrationUnit.h"
#include "UserIni.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

struct TParaUi {
    TPanel * pnBase ;

    TLabel * lbDotSize  ; TEdit * edDotSize  ;
    TLabel * lbDotPitch ; TEdit * edDotPitch ;


    TLabel * lbPitchX     ; TEdit     * edPitchX     ;
    TLabel * lbPitchY     ; TEdit     * edPitchY     ;
    TLabel * lbStartXOfs  ; TEdit     * edStartXOfs  ;
    TLabel * lbStartYOfs  ; TEdit     * edStartYOfs  ;
    TLabel * lbThreshold  ; TEdit     * edThreshold  ;
    TLabel * lbFindLtOnDk ; TCheckBox * cbFindLtOnDk ;
    TLabel * lbIgnrInside ; TCheckBox * cbIgnrInside ;
    TLabel * lbIgnrSide   ; TCheckBox * cbIgnrSide   ;

    TLabel * lbMinArea        ; TEdit * edMinArea        ;  TEdit * edMaxArea        ;
    TLabel * lbMinWidth       ; TEdit * edMinWidth       ;  TEdit * edMaxWidth       ;
    TLabel * lbMinHeight      ; TEdit * edMinHeight      ;  TEdit * edMaxHeight      ;
    TLabel * lbMinBoundary    ; TEdit * edMinBoundary    ;  TEdit * edMaxBoundary    ;
    TLabel * lbMinCompactness ; TEdit * edMinCompactness ;  TEdit * edMaxCompactness ;
    TLabel * lbMinAngle       ; TEdit * edMinAngle       ;  TEdit * edMaxAngle       ;
    TLabel * lbMinAspectRatio ; TEdit * edMinAspectRatio ;  TEdit * edMaxAspectRatio ;

    TLabel * lbResX           ; TEdit * edResX           ;
    TLabel * lbResY           ; TEdit * edResY           ;
};
TParaUi ParaUi ;

CCal * Cal[MAX_CAL_ID] ;
void InitCal ()
{
    for(int i = 0 ; i < MAX_CAL_ID ; i++) {
        Cal[i] = new CCal() ;
    }

    ParaUi.pnBase = NULL ;
}
void CloseCal()
{
    for(int i = 0 ; i < MAX_CAL_ID ; i++) {
        delete Cal [i] ;
    }
}

/******************************************************************************/
/* Calibration                                                                */
/******************************************************************************/
void SetArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    const int iControlWidth   = 120 ;
    const int iControlHeight  = 20  ;
    const int iStartRowOffset = 5   ;
    const int iStartColOffset = 5   ;

    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;
}

void MakeUI(TParaUi  * _pParaUi , TWinControl * _tcControl)
{
    int iR = 0 ;
    _pParaUi->pnBase = new TPanel(_tcControl  ); _pParaUi->pnBase -> Parent = _tcControl ;

    _pParaUi->lbDotSize    = new TLabel(_pParaUi->pnBase); _pParaUi->edDotSize    = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbDotPitch   = new TLabel(_pParaUi->pnBase); _pParaUi->edDotPitch   = new TEdit    (_pParaUi->pnBase);

    _pParaUi->lbPitchX     = new TLabel(_pParaUi->pnBase); _pParaUi->edPitchX     = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbPitchY     = new TLabel(_pParaUi->pnBase); _pParaUi->edPitchY     = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbStartXOfs  = new TLabel(_pParaUi->pnBase); _pParaUi->edStartXOfs  = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbStartYOfs  = new TLabel(_pParaUi->pnBase); _pParaUi->edStartYOfs  = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbThreshold  = new TLabel(_pParaUi->pnBase); _pParaUi->edThreshold  = new TEdit    (_pParaUi->pnBase);
    _pParaUi->lbFindLtOnDk = new TLabel(_pParaUi->pnBase); _pParaUi->cbFindLtOnDk = new TCheckBox(_pParaUi->pnBase);
    _pParaUi->lbIgnrInside = new TLabel(_pParaUi->pnBase); _pParaUi->cbIgnrInside = new TCheckBox(_pParaUi->pnBase);
    _pParaUi->lbIgnrSide   = new TLabel(_pParaUi->pnBase); _pParaUi->cbIgnrSide   = new TCheckBox(_pParaUi->pnBase);

    _pParaUi->lbMinArea        = new TLabel(_pParaUi->pnBase); _pParaUi->edMinArea        = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxArea        = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbMinWidth       = new TLabel(_pParaUi->pnBase); _pParaUi->edMinWidth       = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxWidth       = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbMinHeight      = new TLabel(_pParaUi->pnBase); _pParaUi->edMinHeight      = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxHeight      = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbMinBoundary    = new TLabel(_pParaUi->pnBase); _pParaUi->edMinBoundary    = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxBoundary    = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbMinCompactness = new TLabel(_pParaUi->pnBase); _pParaUi->edMinCompactness = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxCompactness = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbMinAngle       = new TLabel(_pParaUi->pnBase); _pParaUi->edMinAngle       = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxAngle       = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbMinAspectRatio = new TLabel(_pParaUi->pnBase); _pParaUi->edMinAspectRatio = new TEdit(_pParaUi->pnBase); _pParaUi->edMaxAspectRatio = new TEdit(_pParaUi->pnBase);

    SetArrange(_pParaUi->lbDotSize    , 0 , iR  );  SetArrange(_pParaUi->edDotSize    , 1 , iR++ );
    SetArrange(_pParaUi->lbDotPitch   , 0 , iR  );  SetArrange(_pParaUi->edDotPitch   , 1 , iR++ );

    iR++;
    SetArrange(_pParaUi->lbPitchX     , 0 , iR  );  SetArrange(_pParaUi->edPitchX     , 1 , iR++ );
    SetArrange(_pParaUi->lbPitchY     , 0 , iR  );  SetArrange(_pParaUi->edPitchY     , 1 , iR++ );
    SetArrange(_pParaUi->lbStartXOfs  , 0 , iR  );  SetArrange(_pParaUi->edStartXOfs  , 1 , iR++ );
    SetArrange(_pParaUi->lbStartYOfs  , 0 , iR  );  SetArrange(_pParaUi->edStartYOfs  , 1 , iR++ );
    SetArrange(_pParaUi->lbThreshold  , 0 , iR  );  SetArrange(_pParaUi->edThreshold  , 1 , iR++ );
    SetArrange(_pParaUi->lbFindLtOnDk , 0 , iR  );  SetArrange(_pParaUi->cbFindLtOnDk , 1 , iR++ );
    SetArrange(_pParaUi->lbIgnrInside , 0 , iR  );  SetArrange(_pParaUi->cbIgnrInside , 1 , iR++ );
    SetArrange(_pParaUi->lbIgnrSide   , 0 , iR  );  SetArrange(_pParaUi->cbIgnrSide   , 1 , iR++ );

    iR++;
    SetArrange(_pParaUi->lbMinArea        , 0 , iR  );  SetArrange(_pParaUi->edMinArea        , 1 , iR ); SetArrange(_pParaUi->edMaxArea        , 2 , iR++ );
    SetArrange(_pParaUi->lbMinWidth       , 0 , iR  );  SetArrange(_pParaUi->edMinWidth       , 1 , iR ); SetArrange(_pParaUi->edMaxWidth       , 2 , iR++ );
    SetArrange(_pParaUi->lbMinHeight      , 0 , iR  );  SetArrange(_pParaUi->edMinHeight      , 1 , iR ); SetArrange(_pParaUi->edMaxHeight      , 2 , iR++ );
    SetArrange(_pParaUi->lbMinBoundary    , 0 , iR  );  SetArrange(_pParaUi->edMinBoundary    , 1 , iR ); SetArrange(_pParaUi->edMaxBoundary    , 2 , iR++ );
    SetArrange(_pParaUi->lbMinCompactness , 0 , iR  );  SetArrange(_pParaUi->edMinCompactness , 1 , iR ); SetArrange(_pParaUi->edMaxCompactness , 2 , iR++ );
    SetArrange(_pParaUi->lbMinAngle       , 0 , iR  );  SetArrange(_pParaUi->edMinAngle       , 1 , iR ); SetArrange(_pParaUi->edMaxAngle       , 2 , iR++ );
    SetArrange(_pParaUi->lbMinAspectRatio , 0 , iR  );  SetArrange(_pParaUi->edMinAspectRatio , 1 , iR ); SetArrange(_pParaUi->edMaxAspectRatio , 2 , iR++ );


    _pParaUi->lbDotSize    -> Parent = _pParaUi->pnBase ; _pParaUi->edDotSize    -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbDotPitch   -> Parent = _pParaUi->pnBase ; _pParaUi->edDotPitch   -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbPitchX     -> Parent = _pParaUi->pnBase ; _pParaUi->edPitchX     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbPitchY     -> Parent = _pParaUi->pnBase ; _pParaUi->edPitchY     -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbStartXOfs  -> Parent = _pParaUi->pnBase ; _pParaUi->edStartXOfs  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbStartYOfs  -> Parent = _pParaUi->pnBase ; _pParaUi->edStartYOfs  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbThreshold  -> Parent = _pParaUi->pnBase ; _pParaUi->edThreshold  -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbFindLtOnDk -> Parent = _pParaUi->pnBase ; _pParaUi->cbFindLtOnDk -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbIgnrInside -> Parent = _pParaUi->pnBase ; _pParaUi->cbIgnrInside -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbIgnrSide   -> Parent = _pParaUi->pnBase ; _pParaUi->cbIgnrSide   -> Parent = _pParaUi->pnBase ;

    _pParaUi->lbMinArea        -> Parent = _pParaUi->pnBase ; _pParaUi->edMinArea        -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxArea        -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbMinWidth       -> Parent = _pParaUi->pnBase ; _pParaUi->edMinWidth       -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxWidth       -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbMinHeight      -> Parent = _pParaUi->pnBase ; _pParaUi->edMinHeight      -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxHeight      -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbMinBoundary    -> Parent = _pParaUi->pnBase ; _pParaUi->edMinBoundary    -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxBoundary    -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbMinCompactness -> Parent = _pParaUi->pnBase ; _pParaUi->edMinCompactness -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxCompactness -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbMinAngle       -> Parent = _pParaUi->pnBase ; _pParaUi->edMinAngle       -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxAngle       -> Parent = _pParaUi->pnBase ;
    _pParaUi->lbMinAspectRatio -> Parent = _pParaUi->pnBase ; _pParaUi->edMinAspectRatio -> Parent = _pParaUi->pnBase ; _pParaUi->edMaxAspectRatio -> Parent = _pParaUi->pnBase ;


    _pParaUi->lbDotSize        -> Caption = "도트지름(mm) ";//"DotSize    ";
    _pParaUi->lbDotPitch       -> Caption = "도트피치(mm) ";//"DotPitch   ";

    _pParaUi->lbPitchX         -> Caption = "X축 검색간격 "          ;//"PitchX     ";
    _pParaUi->lbPitchY         -> Caption = "Y축 검색간격 "          ;//"PitchY     ";
    _pParaUi->lbStartXOfs      -> Caption = "X축 검색시작옵셑 "      ;//"StartXOfs  ";
    _pParaUi->lbStartYOfs      -> Caption = "Y축 검색시작옵셑 "      ;//"StartYOfs  ";
    _pParaUi->lbThreshold      -> Caption = "검색 밝기값 "           ;//"Threshold  ";
    _pParaUi->lbFindLtOnDk     -> Caption = "밝은물체찾기 "          ;//"FindLtOnDk ";
    _pParaUi->lbIgnrInside     -> Caption = "블랍내부무시 "          ;//"IgnrInside ";
    _pParaUi->lbIgnrSide       -> Caption = "ROI벽면에 걸친물체무시 ";//"IgnrSide   ";

    _pParaUi->lbMinArea        -> Caption = "영역개수 "          ;//"Area       ";
    _pParaUi->lbMinWidth       -> Caption = "넓이 "              ;//"Width      ";
    _pParaUi->lbMinHeight      -> Caption = "높이 "              ;//"Height     ";
    _pParaUi->lbMinBoundary    -> Caption = "테두리길이 "        ;//"Boundary   ";
    _pParaUi->lbMinCompactness -> Caption = "Compactness "       ;//"Compactness";
    _pParaUi->lbMinAngle       -> Caption = "Angle       "       ;//"Angle      ";
    _pParaUi->lbMinAspectRatio -> Caption = "AspectRatio "       ;//"AspectRatio";


    _pParaUi->lbResX = new TLabel(_pParaUi->pnBase); _pParaUi->edResX = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbResY = new TLabel(_pParaUi->pnBase); _pParaUi->edResY = new TEdit(_pParaUi->pnBase);
    _pParaUi->lbResX-> Parent = _pParaUi->pnBase ; _pParaUi->edResX-> Parent = _pParaUi->pnBase ;
    _pParaUi->lbResY-> Parent = _pParaUi->pnBase ; _pParaUi->edResY-> Parent = _pParaUi->pnBase ;
    iR++; 
    SetArrange(_pParaUi->lbResX , 0 , iR); SetArrange(_pParaUi->edResX , 1 , iR++ ); _pParaUi-> edResX -> Color = clSilver ;
    SetArrange(_pParaUi->lbResY , 0 , iR); SetArrange(_pParaUi->edResY , 1 , iR++ ); _pParaUi-> edResY -> Color = clSilver ;
    _pParaUi->lbResX -> Caption = "X픽셀 레졸루션 ";//"Px Res X";
    _pParaUi->lbResY -> Caption = "Y픽셀 레졸루션 ";//"Px Res Y";


}

//==============================================================================
CCal::CCal()
{

}

CCal::~CCal()
{

}

void CCal::Init(EN_CAL_ID _iCalId , bool _bCommonUse)
{
    Tracker = new TRectTracker() ;
    Tracker -> Caption = "CAL";

    m_iCalId = _iCalId ;
    m_bCommonUse = _bCommonUse ;

    Disp.BlbDisp.clBoundary = clYellow ;
    Disp.BlbDisp.clRect     = clLime   ;
    Disp.BlbDisp.clCross    = clLime   ;
    Disp.BlbDisp.clText     = clLime   ;
    Disp.clText             = clRed    ;
}

void CCal::Close()
{
    delete Tracker ;
}

void CCal::Load(bool _bLoad )
{
    TUserINI UserINI ;


    AnsiString sFilePath ;
    if(m_bCommonUse) {
        sFilePath = GetExeDir("Util\\Vision\\Calibration");
    }
    else {
        sFilePath = g_sJobFilePath + "Calibration"
    }

    AnsiString sTemp = "Calibration"+AnsiString(m_iCalId) ;
    AnsiString sFilePath = _sFilePath + ".ini" ;
    AnsiString sTrakPath = _sFilePath + "Tracker.ini" ;

    Tracker -> OnLoadSave(_bLoad , sTrakPath , sTemp + "_Tracker");

    if(_bLoad) {
        UserINI.Load(sFilePath, sTemp , "fDotSize   ", Para.fDotSize   );
        UserINI.Load(sFilePath, sTemp , "fDotPitch  ", Para.fDotPitch  );

        UserINI.Load(sFilePath, sTemp , "BlbPara.iPitchX    ", Para.BlbPara.iPitchX    );
        UserINI.Load(sFilePath, sTemp , "BlbPara.iPitchY    ", Para.BlbPara.iPitchY    );
        UserINI.Load(sFilePath, sTemp , "BlbPara.iStartXOfs ", Para.BlbPara.iStartXOfs );
        UserINI.Load(sFilePath, sTemp , "BlbPara.iStartYOfs ", Para.BlbPara.iStartYOfs );
        UserINI.Load(sFilePath, sTemp , "BlbPara.iThreshold ", Para.BlbPara.iThreshold );
        UserINI.Load(sFilePath, sTemp , "BlbPara.bFindLtOnDk", Para.BlbPara.bFindLtOnDk);
        UserINI.Load(sFilePath, sTemp , "BlbPara.bIgnrInside", Para.BlbPara.bIgnrInside);
        UserINI.Load(sFilePath, sTemp , "BlbPara.bIgnrSide  ", Para.BlbPara.bIgnrSide  );

        UserINI.Load(sFilePath, sTemp , "BlbPara.iMinArea       ", Para.BlbPara.iMinArea       );UserINI.Load(sFilePath, sTemp , "BlbPara.iMaxArea       ", Para.BlbPara.iMaxArea       );
        UserINI.Load(sFilePath, sTemp , "BlbPara.iMinWidth      ", Para.BlbPara.iMinWidth      );UserINI.Load(sFilePath, sTemp , "BlbPara.iMaxWidth      ", Para.BlbPara.iMaxWidth      );
        UserINI.Load(sFilePath, sTemp , "BlbPara.iMinHeight     ", Para.BlbPara.iMinHeight     );UserINI.Load(sFilePath, sTemp , "BlbPara.iMaxHeight     ", Para.BlbPara.iMaxHeight     );
        UserINI.Load(sFilePath, sTemp , "BlbPara.iMinBoundary   ", Para.BlbPara.iMinBoundary   );UserINI.Load(sFilePath, sTemp , "BlbPara.iMaxBoundary   ", Para.BlbPara.iMaxBoundary   );
        UserINI.Load(sFilePath, sTemp , "BlbPara.fMinCompactness", Para.BlbPara.fMinCompactness);UserINI.Load(sFilePath, sTemp , "BlbPara.fMaxCompactness", Para.BlbPara.fMaxCompactness);
        UserINI.Load(sFilePath, sTemp , "BlbPara.fMinAngle      ", Para.BlbPara.fMinAngle      );UserINI.Load(sFilePath, sTemp , "BlbPara.fMaxAngle      ", Para.BlbPara.fMaxAngle      );
        UserINI.Load(sFilePath, sTemp , "BlbPara.fMinAspectRatio", Para.BlbPara.fMinAspectRatio);UserINI.Load(sFilePath, sTemp , "BlbPara.fMaxAspectRatio", Para.BlbPara.fMaxAspectRatio);

        UserINI.Load(sFilePath, sTemp , "Rslt.fXPxRes ", Rslt.fXPxRes );
        UserINI.Load(sFilePath, sTemp , "Rslt.fYPxRes ", Rslt.fYPxRes );
    }
    else {
        UserINI.Save(sFilePath, sTemp , "fDotSize           ", Para.fDotSize           );
        UserINI.Save(sFilePath, sTemp , "fDotPitch          ", Para.fDotPitch          );

        UserINI.Save(sFilePath, sTemp , "BlbPara.iPitchX    ", Para.BlbPara.iPitchX    );
        UserINI.Save(sFilePath, sTemp , "BlbPara.iPitchY    ", Para.BlbPara.iPitchY    );
        UserINI.Save(sFilePath, sTemp , "BlbPara.iStartXOfs ", Para.BlbPara.iStartXOfs );
        UserINI.Save(sFilePath, sTemp , "BlbPara.iStartYOfs ", Para.BlbPara.iStartYOfs );
        UserINI.Save(sFilePath, sTemp , "BlbPara.iThreshold ", Para.BlbPara.iThreshold );
        UserINI.Save(sFilePath, sTemp , "BlbPara.bFindLtOnDk", Para.BlbPara.bFindLtOnDk);
        UserINI.Save(sFilePath, sTemp , "BlbPara.bIgnrInside", Para.BlbPara.bIgnrInside);
        UserINI.Save(sFilePath, sTemp , "BlbPara.bIgnrSide  ", Para.BlbPara.bIgnrSide  );

        UserINI.Save(sFilePath, sTemp , "BlbPara.iMinArea       ", Para.BlbPara.iMinArea       );UserINI.Save(sFilePath, sTemp , "BlbPara.iMaxArea       ", Para.BlbPara.iMaxArea       );
        UserINI.Save(sFilePath, sTemp , "BlbPara.iMinWidth      ", Para.BlbPara.iMinWidth      );UserINI.Save(sFilePath, sTemp , "BlbPara.iMaxWidth      ", Para.BlbPara.iMaxWidth      );
        UserINI.Save(sFilePath, sTemp , "BlbPara.iMinHeight     ", Para.BlbPara.iMinHeight     );UserINI.Save(sFilePath, sTemp , "BlbPara.iMaxHeight     ", Para.BlbPara.iMaxHeight     );
        UserINI.Save(sFilePath, sTemp , "BlbPara.iMinBoundary   ", Para.BlbPara.iMinBoundary   );UserINI.Save(sFilePath, sTemp , "BlbPara.iMaxBoundary   ", Para.BlbPara.iMaxBoundary   );
        UserINI.Save(sFilePath, sTemp , "BlbPara.fMinCompactness", Para.BlbPara.fMinCompactness);UserINI.Save(sFilePath, sTemp , "BlbPara.fMaxCompactness", Para.BlbPara.fMaxCompactness);
        UserINI.Save(sFilePath, sTemp , "BlbPara.fMinAngle      ", Para.BlbPara.fMinAngle      );UserINI.Save(sFilePath, sTemp , "BlbPara.fMaxAngle      ", Para.BlbPara.fMaxAngle      );
        UserINI.Save(sFilePath, sTemp , "BlbPara.fMinAspectRatio", Para.BlbPara.fMinAspectRatio);UserINI.Save(sFilePath, sTemp , "BlbPara.fMaxAspectRatio", Para.BlbPara.fMaxAspectRatio);

        UserINI.Save(sFilePath, sTemp , "Rslt.fXPxRes ", Rslt.fXPxRes );
        UserINI.Save(sFilePath, sTemp , "Rslt.fYPxRes ", Rslt.fYPxRes );
    }
}

void CCal::ApplyPara(bool _bTable)
{
    if(_bTable) {
        ParaUi.edDotSize    -> Text    = Para.fDotSize   ;
        ParaUi.edDotPitch   -> Text    = Para.fDotPitch  ;

        ParaUi.edPitchX     -> Text    = Para.BlbPara.iPitchX    ;
        ParaUi.edPitchY     -> Text    = Para.BlbPara.iPitchY    ;
        ParaUi.edStartXOfs  -> Text    = Para.BlbPara.iStartXOfs ;
        ParaUi.edStartYOfs  -> Text    = Para.BlbPara.iStartYOfs ;
        ParaUi.edThreshold  -> Text    = Para.BlbPara.iThreshold ;
        ParaUi.cbFindLtOnDk -> Checked = Para.BlbPara.bFindLtOnDk;
        ParaUi.cbIgnrInside -> Checked = Para.BlbPara.bIgnrInside;
        ParaUi.cbIgnrSide   -> Checked = Para.BlbPara.bIgnrSide  ;

        ParaUi.edMinArea        -> Text = Para.BlbPara.iMinArea        ; ParaUi.edMaxArea          -> Text = Para.BlbPara.iMaxArea        ;
        ParaUi.edMinWidth       -> Text = Para.BlbPara.iMinWidth       ; ParaUi.edMaxWidth         -> Text = Para.BlbPara.iMaxWidth       ;
        ParaUi.edMinHeight      -> Text = Para.BlbPara.iMinHeight      ; ParaUi.edMaxHeight        -> Text = Para.BlbPara.iMaxHeight      ;
        ParaUi.edMinBoundary    -> Text = Para.BlbPara.iMinBoundary    ; ParaUi.edMaxBoundary      -> Text = Para.BlbPara.iMaxBoundary    ;
        ParaUi.edMinCompactness -> Text = Para.BlbPara.fMinCompactness ; ParaUi.edMaxCompactness   -> Text = Para.BlbPara.fMaxCompactness ;
        ParaUi.edMinAngle       -> Text = Para.BlbPara.fMinAngle       ; ParaUi.edMaxAngle         -> Text = Para.BlbPara.fMaxAngle       ;
        ParaUi.edMinAspectRatio -> Text = Para.BlbPara.fMinAspectRatio ; ParaUi.edMaxAspectRatio   -> Text = Para.BlbPara.fMaxAspectRatio ;

        ParaUi.edResX -> Text = Rslt.fXPxRes ;
        ParaUi.edResY -> Text = Rslt.fYPxRes ;  //켈리브레이션 저장해서 적용 하게 하고 인스펙션 할때 물어 보고 하기.
    }
    else {
        Para.fDotSize            = StrToFloatDef(ParaUi.edDotSize  -> Text , 1.0 ) ;
        Para.fDotPitch           = StrToFloatDef(ParaUi.edDotPitch -> Text , 0.25) ;

        Para.BlbPara.iPitchX     = StrToIntDef (ParaUi.edPitchX     -> Text , 1  ) ;
        Para.BlbPara.iPitchY     = StrToIntDef (ParaUi.edPitchY     -> Text , 1  ) ;
        Para.BlbPara.iStartXOfs  = StrToIntDef (ParaUi.edStartXOfs  -> Text , 0  ) ;
        Para.BlbPara.iStartYOfs  = StrToIntDef (ParaUi.edStartYOfs  -> Text , 0  ) ;
        Para.BlbPara.iThreshold  = StrToIntDef (ParaUi.edThreshold  -> Text , 100) ;
        Para.BlbPara.bFindLtOnDk =              ParaUi.cbFindLtOnDk -> Checked     ;
        Para.BlbPara.bIgnrInside =              ParaUi.cbIgnrInside -> Checked     ;
        Para.BlbPara.bIgnrSide   =              ParaUi.cbIgnrSide   -> Checked     ;

        Para.BlbPara.iMinArea        = StrToIntDef  (ParaUi.edMinArea        -> Text , 0 ) ;
        Para.BlbPara.iMinWidth       = StrToIntDef  (ParaUi.edMinWidth       -> Text , 0 ) ;
        Para.BlbPara.iMinHeight      = StrToIntDef  (ParaUi.edMinHeight      -> Text , 0 ) ;
        Para.BlbPara.iMinBoundary    = StrToIntDef  (ParaUi.edMinBoundary    -> Text , 0 ) ;
        Para.BlbPara.fMinCompactness = StrToFloatDef(ParaUi.edMinCompactness -> Text , 0 ) ;
        Para.BlbPara.fMinAngle       = StrToFloatDef(ParaUi.edMinAngle       -> Text , 0 ) ;
        Para.BlbPara.fMinAspectRatio = StrToFloatDef(ParaUi.edMinAspectRatio -> Text , 0 ) ;

        Para.BlbPara.iMaxArea        = StrToIntDef  (ParaUi.edMaxArea        -> Text , 0 ) ;
        Para.BlbPara.iMaxWidth       = StrToIntDef  (ParaUi.edMaxWidth       -> Text , 0 ) ;
        Para.BlbPara.iMaxHeight      = StrToIntDef  (ParaUi.edMaxHeight      -> Text , 0 ) ;
        Para.BlbPara.iMaxBoundary    = StrToIntDef  (ParaUi.edMaxBoundary    -> Text , 0 ) ;
        Para.BlbPara.fMaxCompactness = StrToFloatDef(ParaUi.edMaxCompactness -> Text , 0 ) ;
        Para.BlbPara.fMaxAngle       = StrToFloatDef(ParaUi.edMaxAngle       -> Text , 0 ) ;
        Para.BlbPara.fMaxAspectRatio = StrToFloatDef(ParaUi.edMaxAspectRatio -> Text , 0 ) ;

        Rslt.fXPxRes = StrToFloatDef(ParaUi.edResX -> Text , 0 );
        Rslt.fYPxRes = StrToFloatDef(ParaUi.edResY -> Text , 0 );  //켈리브레이션 저장해서 적용 하게 하고 인스펙션 할때 물어 보고 하기.

        ApplyPara(true);
    }
}

void CCal::SetTrackerPaintCallback(TOnChanged ChangeFunc)
{
    Tracker -> SetFunc(ChangeFunc) ;
}

bool CCal::OnMouseDw(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker -> Focused = false ;

    if(Tracker -> TrackerDw(Button, Shift , X,Y, _fScaleX , _fScaleY)) {return true ;}

    return false ;
}

bool CCal::OnMouseMv(TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    if(Tracker -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY)) return true ;

    return false ;
}

void CCal::OnMouseUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    Tracker -> TrackerUp(Button, Shift );
}

void CCal::PaintTracker(HDC _hHdc , float _fScaleX , float _fScaleY)
{
    Tracker -> Paint(_hHdc , _fScaleX , _fScaleY);
}

void CCal::ShowPara(TWinControl* _wcBase)
{
    if(ParaUi.pnBase == NULL)MakeUI(&ParaUi,_wcBase) ;
    ParaUi.pnBase -> Parent = _wcBase ;
    ParaUi.pnBase -> Align = alClient ;
    ParaUi.pnBase -> Left = 0 ;
    ParaUi.pnBase -> Top  = 0 ;

    //ParaUi.pnBase -> Width  = 231 ;
    //ParaUi.pnBase -> Height = 714 ;

    ApplyPara(true)
}



bool CCal::Inspection(IImage * _pImg , CAL_Rslt * _pRslt)
{
    bool bRet ;

    bRet = CAL_Inspect(_pImg , Tracker -> GetTRect() , Para , _pRslt);

    //memcpy(&Rslt , _pRslt , sizeof(CAL_Rslt));

    if(Rslt.BlbRslt.pBlobs) {
        for(int i = 0 ; i < Rslt.BlbRslt.iBlobCnt ; i++) {
            Rslt.BlbRslt.pBlobs[i].OnClear();
        }
        delete [] Rslt.BlbRslt.pBlobs ;
        Rslt.BlbRslt.pBlobs   = NULL  ;
        Rslt.BlbRslt.iBlobCnt = 0 ;
    }
    Rslt.BlbRslt.iBlobCnt  = _pRslt->BlbRslt.iBlobCnt  ;
    Rslt.BlbRslt.fInspTime = _pRslt->BlbRslt.fInspTime ;
    Rslt.BlbRslt.pBlobs    = new SBlob[Rslt.BlbRslt.iBlobCnt];
    for(int i = 0 ; i <Rslt.BlbRslt.iBlobCnt ; i++) {
        Rslt.BlbRslt.pBlobs[i] = _pRslt->BlbRslt.pBlobs[i] ;
    }

    Rslt.fXPxRes   = _pRslt->fXPxRes   ;
    Rslt.fYPxRes   = _pRslt->fYPxRes   ;
    Rslt.fInspTime = _pRslt->fInspTime ;

    ParaUi.edResX -> Text = Rslt.fXPxRes ;
    ParaUi.edResY -> Text = Rslt.fYPxRes ;


    return bRet ;
}

void CCal::PaintRslt(IImage * _pImg , HDC _hHdc)
{
    CAL_PaintRslt(_pImg , _hHdc , &Rslt , Disp);
}

float CCal::GetXRes()
{
    return Rslt.fXPxRes ;
}

float CCal::GetYRes()
{
    return Rslt.fYPxRes ;


}

