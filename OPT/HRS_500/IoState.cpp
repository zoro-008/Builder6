//---------------------------------------------------------------------------
#pragma hdrstop
#include "IoState.h"
#include "SMInterfaceUnit.h"
#include "UserINI.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

CIoState IOS;
//---------------------------------------------------------------------------
CIoState::CIoState()
{
    LoadDnmVar(); //Dll 에서 받아와야 될듯.
//    m_pIn  = new SIo[m_iMaxIn  ] ;
//    m_pOut = new SIo[m_iMaxOut ] ;
}

CIoState::~CIoState()
{
//    delete [] m_pIn  ;
//    delete [] m_pOut ;
}

void CIoState::SetInPutWindow (TPanel * _pnBase , int _iFontSize , int _iRow )
{
    int iWidth , iHeight ;
    int iHeightInc ;
    int iFontSize  ;
    int iRow   , iCol    ;
    int iTemp = 0;

    iFontSize = _iFontSize   ;

    iHeightInc = 0 ;

    iRow = _iRow ;
    if(m_iMaxIn % iRow == 0 ) iCol = m_iMaxIn / iRow     ;
    else                      iCol = m_iMaxIn / iRow + 1 ;

    iWidth  = _pnBase->Width  / iRow ;
    iHeight = _pnBase->Height / iCol ;

    _pnBase -> Caption = "" ;
    _pnBase -> Height  = iHeight * iCol ;    
//    TPanel **m_pInPanel;
    m_pInPanel = new TPanel*[m_iMaxIn];

    for(int i = 0 ; i < m_iMaxIn ; i++ )
    {
        m_pInPanel [i] = new TPanel(_pnBase)       ;
        //Parent Setting
        m_pInPanel [i] -> Parent     = _pnBase ;
        m_pInPanel [i] -> Caption    = IO_GetXName(i) ;
        m_pInPanel [i] -> Font->Size = iFontSize      ;
        m_pInPanel [i] -> Tag        = i              ;
        m_pInPanel [i] -> Width      = iWidth  ;
        m_pInPanel [i] -> Height     = iHeight ;
        iTemp = i % iRow ;
        if(iTemp == 0 && i != 0)
        {
            iHeightInc += iHeight ;
            m_pInPanel [i] -> Left  = iTemp * m_pInPanel [i] -> Width ;
            m_pInPanel [i] -> Top   = iHeightInc ;

        }
        else
        {
            m_pInPanel [i] -> Left  = ((i % iRow))* m_pInPanel [i] -> Width ;
            m_pInPanel [i] -> Top   = iHeightInc ;            
        }

    }

    tmInUpdate = new TTimer(Application);
    tmInUpdate -> Interval = 300  ;
    tmInUpdate -> OnTimer  = tmInUpdateTimer ;
    tmInUpdate -> Enabled  = true ;
}

void CIoState::SetOutPutWindow(TPanel * _pnBase , int _iFontSize , int _iRow )
{
    int iWidth , iHeight ;
    int iHeightInc ;
    int iFontSize  ;
    int iRow   , iCol    ;
    int iTemp = 0;

    iFontSize = _iFontSize   ;

    iHeightInc = 0 ;

    iRow = _iRow ;
    if(m_iMaxIn % iRow == 0 ) iCol = m_iMaxOut / iRow     ;
    else                      iCol = m_iMaxOut / iRow + 1 ;

    iWidth  = _pnBase->Width  / iRow ;
    iHeight = _pnBase->Height / iCol ;

    _pnBase -> Caption = "" ;
    _pnBase -> Height  = iHeight * iCol ;
//    TPanel **m_pInPanel;
    m_pOutPanel = new TPanel*[m_iMaxOut];

    for(int i = 0 ; i < m_iMaxOut ; i++ )
    {
        m_pOutPanel [i] = new TPanel(_pnBase)       ;
        //Parent Setting
        m_pOutPanel [i] -> Parent     = _pnBase ;
        m_pOutPanel [i] -> Caption    = IO_GetYName(i) ;
        m_pOutPanel [i] -> Font->Size = iFontSize      ;
        m_pOutPanel [i] -> Tag        = i              ;
        m_pOutPanel [i] -> Width      = iWidth  ;
        m_pOutPanel [i] -> Height     = iHeight ;
        m_pOutPanel [i] -> OnDblClick = pnDbClick ;
        iTemp = i % iRow ;
        if(iTemp == 0 && i != 0)
        {
            iHeightInc += iHeight ;
            m_pOutPanel [i] -> Left  = iTemp * m_pOutPanel [i] -> Width ;
            m_pOutPanel [i] -> Top   = iHeightInc ;

        }
        else
        {
            m_pOutPanel [i] -> Left  = ((i % iRow))* m_pOutPanel [i] -> Width ;
            m_pOutPanel [i] -> Top   = iHeightInc ;
        }

    }
    
    tmOutUpdate = new TTimer(Application);
    tmOutUpdate -> Interval = 300  ;
    tmOutUpdate -> OnTimer  = tmOutUpdateTimer ;
    tmOutUpdate -> Enabled  = true ;
}
void __fastcall CIoState::pnDbClick(TObject *Sender)
{
    TPanel * Panel  = (TPanel *)Sender ;
    int      iIndex =  Panel -> Tag ;
    IO_SetY(iIndex,!IO_GetY(iIndex));
}

void CIoState::LoadDnmVar()
{
    //Local Var.
    TUserINI   UserINI  ;
    AnsiString sPath    ;

    int        iLangSel ;

    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    //Load Device.
    UserINI.Load(sPath , "CIOs"    , "m_iMaxIn "  , m_iMaxIn );
    UserINI.Load(sPath , "CIOs"    , "m_iMaxOut"  , m_iMaxOut);

}
void __fastcall CIoState::tmInUpdateTimer(TObject *Sender)
{
    tmInUpdate -> Enabled = false ;

    for(int i=0; i<m_iMaxIn; i++){
        m_pInPanel[i] -> Color  = IO_GetX(i) ? clRed : clSkyBlue ;
    }

    tmInUpdate -> Enabled = true ;
}
void __fastcall CIoState::tmOutUpdateTimer(TObject *Sender)
{
    tmOutUpdate -> Enabled = false ;

    for(int i=0; i<m_iMaxOut; i++){
        m_pOutPanel[i] -> Color  = IO_GetY(i) ? clRed : clSkyBlue ;
    }

    tmOutUpdate -> Enabled = true ;
}
