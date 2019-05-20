//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "IOs.h"
//---------------------------------------------------------------------------
//System Header

//Ajin Lib

//User Header
#include "Timer.h"
#include "UserINI.h"
#include "SLogUnit.h"



//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
CIOs IO;
                                  
CIOs::CIOs()
{
    LoadDnmVar();

    m_pIn  = new SBit[m_iMaxIn ] ; 
    m_pOut = new SBit[m_iMaxOut] ;

    m_pInOnDelay   = new CDelayTimer[m_iMaxIn ];
    m_pInOffDelay  = new CDelayTimer[m_iMaxIn ];
    m_pOutOnDelay  = new CDelayTimer[m_iMaxOut];
    m_pOutOffDelay = new CDelayTimer[m_iMaxOut];

    memset( m_pIn  , 0 , sizeof(SBit)* m_iMaxIn ) ;
    memset( m_pOut , 0 , sizeof(SBit)* m_iMaxOut) ;

    Load();

    for(int i = 0 ; i < m_iMaxOut ; i++) {
        m_pOut[i].bVtrVal = GetOut(m_pOut[i].iAdd) ;
    }

    m_bTestMode = false ;
}

CIOs::~CIOs()
{
    delete [] m_pIn  ; 
    delete [] m_pOut ;

    delete [] m_pInOnDelay   ;
    delete [] m_pInOffDelay  ;
    delete [] m_pOutOnDelay  ;
    delete [] m_pOutOffDelay ;
    Trace("","");
}

void CIOs::SetOut(int _iNo , bool _bVal)
{
    bool bVal = m_pOut[_iNo].bInv ? !_bVal : _bVal ;

    AjinDIO.SetOut(m_pOut[_iNo].iAdd , bVal) ;
}

bool CIOs::GetOut(int _iNo )
{
    bool bRet = m_pOut[_iNo].bInv ? !AjinDIO.GetOut(m_pOut[_iNo].iAdd) : AjinDIO.GetOut(m_pOut[_iNo].iAdd) ;
    return bRet ;
}

bool CIOs::GetIn(int _iNo )
{
    bool bRet = m_pIn[_iNo].bInv ? !AjinDIO.GetIn(m_pIn[_iNo].iAdd) : AjinDIO.GetIn(m_pIn[_iNo].iAdd) ;
    return bRet ;
}

void CIOs::Update()
{
    //Static�� �׻� ���� �ؾ� �Ѵ�. �迭�� ����Ǵ� Class�ȿ����� ��� ����.
    bool bPreYVal = false ;
    bool bGetYVal = false ;

    bool bPreXVal = false ;
    bool bGetXVal = false ;


    bool bTemp = false ;


    //Output.
    for (int i = 0 ; i < m_iMaxOut ; i++) {
        //Check Delay.
        if(m_pOut[i].bVtrVal) {
            m_pOutOffDelay [i].Clear();
            if(m_pOut[i].iOnDelay) {
                if(!m_pOutOnDelay [i].OnDelay (m_pOut[i].bVtrVal != m_pOut[i].bAtrVal , m_pOut[i].iOnDelay )){
                    continue ;
                }
            }
        }

        if(!m_pOut[i].bVtrVal) {
            m_pOutOnDelay [i].Clear();
            if(m_pOut[i].iOffDelay) {
                if(!m_pOutOffDelay[i].OnDelay (m_pOut[i].bVtrVal != m_pOut[i].bAtrVal , m_pOut[i].iOffDelay)){
                    continue ;
                }
            }
        }

        //Edge.
        if(m_pOut[i].bVtrVal != m_pOut[i].bAtrVal ) {
            m_pOut[i].bUpEdge = !m_pOut[i].bAtrVal ;
            m_pOut[i].bDnEdge =  m_pOut[i].bAtrVal ;

            Trace( "Output", (m_pOut[i].sEnum + "(" + m_pOut[i].sName + ")_" + String(m_pOut[i].bVtrVal ? "ON" : "OFF") ).c_str());


        }
        else {
            m_pOut[i].bUpEdge = false ;
            m_pOut[i].bDnEdge = false ;
        }

        //Set Output.
        SetOut(m_pOut[i].iAdd , m_pOut[i].bVtrVal);
        m_pOut[i].bAtrVal = GetOut(m_pOut[i].iAdd) ;
        //m_pOut[i].bAtrVal = GetOut(m_pOut[i].iAdd) ;
    }

    for (int i = 0 ; i < m_iMaxIn ; i++) {
        m_pIn[i].bAtrVal = GetIn(m_pIn[i].iAdd) ;

        //Check
        if(m_pIn[i].iOnDelay && !m_pIn[i].bVtrVal){
            if(!m_pInOnDelay [i].OnDelay (m_pIn[i].bAtrVal != m_pIn[i].bVtrVal , m_pIn[i].iOnDelay )){
                continue ;
            }
        }

        if(m_pIn[i].iOffDelay &&  m_pIn[i].bVtrVal ){
            if(!m_pInOffDelay[i].OnDelay (m_pIn[i].bAtrVal != m_pIn[i].bVtrVal , m_pIn[i].iOffDelay)){
                continue ;
            }
        }


        //Edge.
        if(m_pIn[i].bVtrVal != m_pIn[i].bAtrVal) {
            m_pIn[i].bUpEdge = !m_pIn[i].bVtrVal ;
            m_pIn[i].bDnEdge =  m_pIn[i].bVtrVal ;
            Trace( "Input", (m_pIn[i].sEnum + "(" + m_pIn[i].sName + ")_" + String(m_pIn[i].bAtrVal ? "ON" : "OFF") ).c_str());
        }
        else {
            m_pIn[i].bUpEdge = false ;
            m_pIn[i].bDnEdge = false ;
        }

        //Set Input.
        m_pIn[i].bVtrVal = m_pIn[i].bAtrVal ;
    }
}

void CIOs::SetY(int _iNo , bool _bVal , bool _bDirect) //���̷�Ʈ�� Ʈ���̸� �׽�Ʈ ��忡���� ����.
{
    if( _iNo < 0 || _iNo >= m_iMaxOut) {/*ShowMessageT("Range is wrong");*/return ; }

    m_pOut[_iNo].bVtrVal = _bVal ;

    if(_bDirect) {
        SetOut(m_pOut[_iNo].iAdd , _bVal);
        if(GetOut(m_pOut[_iNo].iAdd) != _bVal) {
            Trace( "Direct Output", (m_pOut[_iNo].sEnum + "(" + m_pOut[_iNo].sName + ")_" + String(_bVal ? "ON" : "OFF") ).c_str());
        }
    }


}

bool CIOs::GetY(int _iNo , bool _bDirect )
{
    if( _iNo < 0 || _iNo >= m_iMaxOut) {/*ShowMessageT("Range is wrong");*/ return false; }

    if(_bDirect) return GetOut(m_pOut[_iNo].iAdd);
    return m_pOut[_iNo].bAtrVal ;
}

bool CIOs::GetYDn(int _iNo )
{
    if( _iNo < 0 || _iNo >= m_iMaxOut) {/*ShowMessageT("Range is wrong");*/ return false; }

    return m_pOut[_iNo].bDnEdge ;
}

bool CIOs::GetYUp(int _iNo )
{                                                        
    if( _iNo < 0 || _iNo >= m_iMaxOut) {/*ShowMessageT("Range is wrong");*/ return false; }

    return m_pOut[_iNo].bUpEdge ;
}



bool CIOs::GetX(int _iNo , bool _bDirect)
{
    if( _iNo < 0 || _iNo >= m_iMaxIn) {/*ShowMessageT("Range is wrong");*/return false; }

    if(_bDirect) return GetIn(m_pIn[_iNo].iAdd);

    return m_pIn[_iNo].bVtrVal ;
}

bool CIOs::GetXDn(int _iNo )
{
    if( _iNo < 0 || _iNo >= m_iMaxIn) {/*ShowMessageT("Range is wrong");*/ return false; }

    return m_pIn[_iNo].bDnEdge ;
}

bool CIOs::GetXUp(int _iNo )
{
    if( _iNo < 0 || _iNo >= m_iMaxIn) {/*ShowMessageT("Range is wrong");*/ return false; }

    return m_pIn[_iNo].bUpEdge ;
}

bool CIOs::GetInModuleNo (int _iNo , int &_iModuleNo , int &_iOffset)
{
    if( _iNo < 0 || _iNo >= m_iMaxIn) {/*ShowMessageT("Range is wrong");*/ return false; }

    return AjinDIO.GetInModuleNoDp(m_pIn[_iNo].iAdd , _iModuleNo , _iOffset ) ;
}

bool CIOs::GetOutModuleNo(int _iNo , int &_iModuleNo , int &_iOffset)
{
    if( _iNo < 0 || _iNo >= m_iMaxOut) {/*ShowMessageT("Range is wrong");*/ return false; }

    return AjinDIO.GetOutModuleNoDp(m_pOut[_iNo].iAdd , _iModuleNo , _iOffset) ;
}

void CIOs::Load()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sPath2 ;
    AnsiString sIndex ;

    //Set Path.
    sPath  = EXE_FOLDER + "Util\\Input.INI";

    AnsiString sSelLan ;
    switch(m_iLangSel) {
        default         : sSelLan = "E_" ; break ;
        case lsKorean   : sSelLan = "K_" ; break ;
        case lsEnglish  : sSelLan = "E_" ; break ;
        case lsChinese  : sSelLan = "C_" ; break ;
    }

    for (int i = 0 ; i < m_iMaxIn  ; i++) {
        sIndex.printf("Input(%04d)" , i );
        UserINI.Load(sPath ,           "Address  " , sIndex , m_pIn[i].iAdd     );
        UserINI.Load(sPath ,           "Invert   " , sIndex , m_pIn[i].bInv     );
        UserINI.Load(sPath ,           "Enum     " , sIndex , m_pIn[i].sEnum    );
        UserINI.Load(sPath , sSelLan + "Name     " , sIndex , m_pIn[i].sName    );
        UserINI.Load(sPath , sSelLan + "Comment  " , sIndex , m_pIn[i].SComt    );
        UserINI.Load(sPath ,           "OnDelay  " , sIndex , m_pIn[i].iOnDelay );
        UserINI.Load(sPath ,           "OffDelay " , sIndex , m_pIn[i].iOffDelay);
        Application -> ProcessMessages();
    }

    //Set Path.
    sPath  = EXE_FOLDER + "Util\\Output.INI";

    for (int i = 0 ; i < m_iMaxOut  ; i++) {
        sIndex.printf("Output(%04d)" , i );
        UserINI.Load(sPath ,           "Address  " , sIndex , m_pOut[i].iAdd     );
        UserINI.Load(sPath ,           "Invert   " , sIndex , m_pOut[i].bInv     );
        UserINI.Load(sPath ,           "Enum     " , sIndex , m_pOut[i].sEnum    );
        UserINI.Load(sPath , sSelLan + "Name     " , sIndex , m_pOut[i].sName    );
        UserINI.Load(sPath , sSelLan + "Comment  " , sIndex , m_pOut[i].SComt    );
        UserINI.Load(sPath ,           "OnDelay  " , sIndex , m_pOut[i].iOnDelay );
        UserINI.Load(sPath ,           "OffDelay " , sIndex , m_pOut[i].iOffDelay);
        Application -> ProcessMessages();
    }
}
void CIOs::Save()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sPath2 ;
    AnsiString sIndex ;

    //Set Path.
    sPath  = EXE_FOLDER + "Util\\Input.INI" ;

    //UserINI.ClearFile(sPath) ;

    AnsiString sSelLan ;
    switch(m_iLangSel) {
        default         : sSelLan = "E_" ; break ;
        case lsKorean   : sSelLan = "K_" ; break ;
        case lsEnglish  : sSelLan = "E_" ; break ;
        case lsChinese  : sSelLan = "C_" ; break ;
    }

    for (int i = 0 ; i < m_iMaxIn  ; i++) {
        sIndex.printf("Input(%04d)" , i );
        UserINI.Save(sPath ,           "Address  ", sIndex  , m_pIn[i].iAdd     );
        UserINI.Save(sPath ,           "Invert   ", sIndex  , m_pIn[i].bInv     );
        UserINI.Save(sPath ,           "Enum     ", sIndex  , m_pIn[i].sEnum    );
        UserINI.Save(sPath , sSelLan + "Name     ", sIndex  , m_pIn[i].sName    );
        UserINI.Save(sPath , sSelLan + "Comment  ", sIndex  , m_pIn[i].SComt    );
        UserINI.Save(sPath ,           "OnDelay  ", sIndex  , m_pIn[i].iOnDelay );
        UserINI.Save(sPath ,           "OffDelay ", sIndex  , m_pIn[i].iOffDelay);
        Application -> ProcessMessages();
    }

    //Set Path.
    sPath = EXE_FOLDER + "Util\\Output.INI";

    for (int i = 0 ; i < m_iMaxOut  ; i++) {
        sIndex.printf("Output(%04d)" , i );
        UserINI.Save(sPath ,           "Address  " , sIndex ,  m_pOut[i].iAdd     );
        UserINI.Save(sPath ,           "Invert   " , sIndex ,  m_pOut[i].bInv     );
        UserINI.Save(sPath ,           "Enum     " , sIndex ,  m_pOut[i].sEnum    );
        UserINI.Save(sPath , sSelLan + "Name     " , sIndex ,  m_pOut[i].sName    );
        UserINI.Save(sPath , sSelLan + "Comment  " , sIndex ,  m_pOut[i].SComt    );
        UserINI.Save(sPath ,           "OnDelay  " , sIndex ,  m_pOut[i].iOnDelay );
        UserINI.Save(sPath ,           "OffDelay " , sIndex ,  m_pOut[i].iOffDelay);
        Application -> ProcessMessages();
    }
}

void CIOs::LoadDnmVar()
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
    UserINI.Load(sPath , "Member"  , "m_iLangSel" , iLangSel ); m_iLangSel = (EN_LAN_SEL)iLangSel ;
}

void CIOs::SaveDnmVar()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    //Load Device.
    UserINI.Save(sPath , "CIOs"   , "m_iMaxIn "  ,      m_iMaxIn  );
    UserINI.Save(sPath , "CIOs"   , "m_iMaxOut"  ,      m_iMaxOut );
    UserINI.Save(sPath , "Member" , "m_iLangSel" , (int)m_iLangSel);
}

void CIOs::DisplayList(bool _bOutput , TStringGrid *_sgTable , bool _bFixedWidth )
{
    //Local Var.
    AnsiString sAdd,sHexAdd ;
    AnsiString sTemp = ""   ;

    int iIOModuleNo ;
    int iIOOffset   ;

    //Check Error.
    if (_sgTable == NULL) return;

    //Set StringGrid.
    _sgTable -> DoubleBuffered = true  ;
    _sgTable -> Options << goRowSelect ;
    _sgTable -> DefaultRowHeight = 20 ;
    _sgTable -> RowCount  = (_bOutput ? m_iMaxOut : m_iMaxIn ) + 1 ;   if(_sgTable -> RowCount == 1) _sgTable -> RowCount = 2 ;
    _sgTable -> ColCount  = MAX_SG_COL  ;
    _sgTable -> FixedCols = 1  ;
    _sgTable -> FixedRows = 1  ;
    if(_bFixedWidth)_sgTable -> ColWidths[0] = 25 ;  _sgTable -> Cells[0][0] = "No"      ;
    if(_bFixedWidth)_sgTable -> ColWidths[1] = 50 ;  _sgTable -> Cells[1][0] = "Add"     ;
    if(_bFixedWidth)_sgTable -> ColWidths[2] = 50 ;  _sgTable -> Cells[2][0] = "HexAdd"  ;
    if(_bFixedWidth)_sgTable -> ColWidths[3] = 150;  _sgTable -> Cells[3][0] = "Name"    ;
    if(_bFixedWidth)_sgTable -> ColWidths[4] = 20 ;  _sgTable -> Cells[4][0] = "Invert"  ;
    if(_bFixedWidth)_sgTable -> ColWidths[5] = 170;  _sgTable -> Cells[5][0] = "Comment" ;
    if(_bFixedWidth)_sgTable -> ColWidths[6] = 55 ;  _sgTable -> Cells[6][0] = "Delay"   ;
    if(_bFixedWidth)_sgTable -> ColWidths[7] = 40 ;  _sgTable -> Cells[7][0] = "V Val"   ;
    if(_bFixedWidth)_sgTable -> ColWidths[8] = 40 ;  _sgTable -> Cells[8][0] = "A Val"   ;
    if(_bFixedWidth)_sgTable -> ColWidths[9] = 60 ;  _sgTable -> Cells[9][0] = "Address" ;
    //Display.
    if (_bOutput) {
        //Clear View Index.
        for (int i = 0 ; i < m_iMaxOut ; i++) {
            IO.GetOutModuleNo(i , iIOModuleNo , iIOOffset ) ;
            sTemp.sprintf("DO%d_%02d" ,iIOModuleNo+1 , iIOOffset );
            //Display To List.
//            if(_bDispHexAdd) sAdd = "Y" + sAdd.IntToHex(m_pOut[i].iAdd , 4);
//            else             sAdd = "Y" + sAdd.sprintf ("%04d",m_pOut[i].iAdd) ;
            sAdd    = "Y" + sAdd.sprintf ("%04d",m_pOut[i].iAdd) ;
            sHexAdd = "Y" + sHexAdd.IntToHex(m_pOut[i].iAdd , 4);


            _sgTable -> Cells[0][i + 1] = i                                                     ;
            _sgTable -> Cells[1][i + 1] = sAdd                                                  ;
            _sgTable -> Cells[2][i + 1] = sHexAdd                                               ;
            _sgTable -> Cells[3][i + 1] = m_pOut[i].sName                                       ;
            _sgTable -> Cells[4][i + 1] = m_pOut[i].bInv ? 1 : 0                                ;
            _sgTable -> Cells[5][i + 1] = m_pOut[i].SComt                                       ;
            _sgTable -> Cells[6][i + 1] = AnsiString(m_pOut[i].iOnDelay )+","+
                                          AnsiString(m_pOut[i].iOffDelay)                       ;
            _sgTable -> Cells[7][i + 1] = m_pOut[i].bVtrVal ? "ON" : "OFF"                      ;
            _sgTable -> Cells[8][i + 1] = m_pOut[i].bAtrVal ? "ON" : "OFF"                      ;
            _sgTable -> Cells[9][i + 1] = sTemp                                                 ;
        }
    }
    else {
        //Clear View Index.
        for (int i = 0 ; i < m_iMaxIn ; i++) {
            IO.GetInModuleNo(i , iIOModuleNo , iIOOffset ) ;
            sTemp.sprintf("DI%d_%02d" ,iIOModuleNo+1 , iIOOffset );

            //Display To List.
//            if(_bDispHexAdd) sAdd = "X" + sAdd.IntToHex(m_pIn[i].iAdd , 4);
//            else             sAdd = "X" + sAdd.sprintf ("%04d",m_pIn[i].iAdd) ;
            sAdd    = "X" + sAdd.sprintf ("%04d",m_pIn[i].iAdd) ;
            sHexAdd = "X" + sHexAdd.IntToHex(m_pIn[i].iAdd , 4);

            

            _sgTable -> Cells[0][i + 1] = i                                                      ;
            _sgTable -> Cells[1][i + 1] = sAdd                                                   ;
            _sgTable -> Cells[2][i + 1] = sHexAdd                                                ;
            _sgTable -> Cells[3][i + 1] = m_pIn[i].sName                                         ;
            _sgTable -> Cells[4][i + 1] = m_pIn[i].bInv ? 1 : 0                                  ;
            _sgTable -> Cells[5][i + 1] = m_pIn[i].SComt                                         ;
            _sgTable -> Cells[6][i + 1] = AnsiString(m_pIn[i].iOnDelay )+","+
                                          AnsiString(m_pIn[i].iOffDelay)                         ;
            _sgTable -> Cells[7][i + 1] = m_pIn[i].bVtrVal ? "ON" : "OFF"                        ;
            _sgTable -> Cells[8][i + 1] = m_pIn[i].bAtrVal ? "ON" : "OFF"                        ;
            _sgTable -> Cells[9][i + 1] = sTemp                                                  ;        }
    }

    //Selected Row to 0.
    static bool bFirst = false ;
    if(bFirst) {_sgTable -> TopRow = 1; _sgTable -> Row = 1 ; }
    bFirst = false ;
}

void CIOs::UpdateStatus(bool _bOutput , TStringGrid *_sgTable )
{
    //Local Var.

    //Check Error.
    if (_sgTable == NULL) return;

    //Set StringGrid.
//    _sgTable -> RowCount = (_bOutput ? m_iMaxOut : m_iMaxIn ) + 1 ;
//    _sgTable -> ColCount = MAX_SG_COL ;

    //Display.
    if (_bOutput) {
        //Clear View Index.
        for (int i = 0 ; i < m_iMaxOut ; i++) {
            _sgTable -> Cells[7][i + 1] = m_pOut[i].bVtrVal ? "ON" : "OFF";
            _sgTable -> Cells[8][i + 1] = m_pOut[i].bAtrVal ? "ON" : "OFF";
        }
    }
    else {
        //Clear View Index.
        for (int i = 0 ; i < m_iMaxIn ; i++) {
            _sgTable -> Cells[7][i + 1] = m_pIn[i].bVtrVal ? "ON" : "OFF";
            _sgTable -> Cells[8][i + 1] = m_pIn[i].bAtrVal ? "ON" : "OFF";
        }
    }

}

void CIOs::DisplayProperty(bool          _bTable      ,
                           int           _bOutput     ,
                           TEdit       * _edIndex     ,
                           TEdit       * _edEnum      ,
                           TEdit       * _edAdd       ,
                           TEdit       * _edIOHexAdd  ,
                           TEdit       * _edName      ,
                           TEdit       * _edComt      ,
                           TCheckBox   * _cbIOInverse ,
                           TEdit       * _edOnDelay   ,
                           TEdit       * _edOffDelay  ,
                           TStringGrid *_sgTable      ,
                           bool         _bDispHexAdd  )
{
    //Check Error.
    if (_sgTable == NULL || _edAdd        == NULL
                         || _edName       == NULL
                         || _edEnum       == NULL
                         || _edComt       == NULL
                         || _cbIOInverse  == NULL
                         || _edOnDelay    == NULL
                         || _edOffDelay   == NULL) return;

    //Local Var.
    int iSelNo = _sgTable -> Row - 1 ;
    int iAdd   ;
    AnsiString sAdd , sHexAdd;

    //Set
    _edIndex -> Enabled = false ;

    //Check Address.
    if (              iSelNo <  0          ) return;
    if (!_bOutput && (iSelNo >= m_iMaxIn  )) return;
    if ( _bOutput && (iSelNo >= m_iMaxOut )) return;

    //Get StringGrid Data.
    if (_bOutput) {
        if (_bTable) {
            //Display To List.
//            if(_bDispHexAdd) sAdd = "Y" + sAdd.IntToHex(m_pOut[iSelNo].iAdd , 4);
//            else             sAdd = "Y" + sAdd.sprintf ("%04d",m_pOut[iSelNo].iAdd) ;
            sAdd    = m_pOut[iSelNo].iAdd ; //"Y" +sAdd.sprintf ("%04d",m_pOut[iSelNo].iAdd) ;
            sHexAdd = "Y" + sHexAdd.IntToHex(m_pOut[iSelNo].iAdd , 4);

            _edIndex     -> Text    = iSelNo                      ;
            _edAdd       -> Text    = sAdd                        ;
            _edIOHexAdd  -> Text    = sHexAdd                     ;
            _edEnum      -> Text    = m_pOut[iSelNo].sEnum        ;
            _edName      -> Text    = m_pOut[iSelNo].sName        ;
            _edComt      -> Text    = m_pOut[iSelNo].SComt        ;
            _cbIOInverse -> Checked = m_pOut[iSelNo].bInv ? 1 : 0 ;
            _edOnDelay   -> Text    = m_pOut[iSelNo].iOnDelay     ;
            _edOffDelay  -> Text    = m_pOut[iSelNo].iOffDelay    ;
        }
        else {
            m_pOut[iSelNo].iAdd      =_edAdd       -> Text.ToIntDef(iSelNo) ;
            m_pOut[iSelNo].sEnum     =_edEnum      -> Text                  ;
            m_pOut[iSelNo].sName     =_edName      -> Text                  ;
            m_pOut[iSelNo].SComt     =_edComt      -> Text                  ;
            m_pOut[iSelNo].bInv      =_cbIOInverse -> Checked               ;
            m_pOut[iSelNo].iOnDelay  =_edOnDelay   -> Text.ToIntDef(0)      ;
            m_pOut[iSelNo].iOffDelay =_edOffDelay  -> Text.ToIntDef(0)      ;

            sHexAdd = "Y" + sHexAdd.IntToHex(m_pOut[iSelNo].iAdd , 4);
            _edIOHexAdd  -> Text    = sHexAdd ;
        }
    }
    else {
        if (_bTable) {
            //Display To List.
//            if(_bDispHexAdd) sAdd = "X" + sAdd.IntToHex(m_pIn[iSelNo].iAdd , 4);
//            else             sAdd = "X" + sAdd.sprintf ("%04d",m_pIn[iSelNo].iAdd) ;
            sAdd    = m_pIn[iSelNo].iAdd; //"X" + sAdd.sprintf ("%04d",m_pIn[iSelNo].iAdd) ;
            sHexAdd = "X" + sHexAdd.IntToHex(m_pIn[iSelNo].iAdd , 4);

            _edIndex     -> Text    = iSelNo                     ;
            _edAdd       -> Text    = sAdd                       ;
            _edIOHexAdd  -> Text    = sHexAdd                    ;
            _edEnum      -> Text    = m_pIn[iSelNo].sEnum        ;
            _edName      -> Text    = m_pIn[iSelNo].sName        ;
            _edComt      -> Text    = m_pIn[iSelNo].SComt        ;
            _cbIOInverse -> Checked = m_pIn[iSelNo].bInv ? 1 : 0 ;
            _edOnDelay   -> Text    = m_pIn[iSelNo].iOnDelay     ;
            _edOffDelay  -> Text    = m_pIn[iSelNo].iOffDelay    ;
        }
        else {
            m_pIn[iSelNo].iAdd      =_edAdd       -> Text.ToIntDef(iSelNo) ;
            m_pIn[iSelNo].sName     =_edEnum      -> Text                  ;
            m_pIn[iSelNo].sName     =_edName      -> Text                  ;
            m_pIn[iSelNo].SComt     =_edComt      -> Text                  ;
            m_pIn[iSelNo].bInv      =_cbIOInverse -> Checked               ;
            m_pIn[iSelNo].iOnDelay  =_edOnDelay   -> Text.ToIntDef(0)      ;
            m_pIn[iSelNo].iOffDelay =_edOffDelay  -> Text.ToIntDef(0)      ;

            sHexAdd = "X" + sHexAdd.IntToHex(m_pIn[iSelNo].iAdd , 4);
            _edIOHexAdd  -> Text = sHexAdd ;
        }
    }
}


