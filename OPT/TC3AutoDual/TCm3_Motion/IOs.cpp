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

    m_pIn  = new CBit[m_iMaxIn ] ; m_pInDelay  = new CDelayTimer[m_iMaxIn ];
    m_pOut = new CBit[m_iMaxOut] ; m_pOutDelay = new CDelayTimer[m_iMaxOut];

    memset( m_pIn  , 0 , sizeof(CBit)* m_iMaxIn ) ;
    memset( m_pOut , 0 , sizeof(CBit)* m_iMaxOut) ;

    //Load();
}

CIOs::~CIOs()
{
    delete [] m_pIn  ; delete [] m_pInDelay  ;
    delete [] m_pOut ; delete [] m_pOutDelay ;
}

void CIOs::SetOut(int _iNo , bool _bVal)
{
    bool bVal = m_pOut[_iNo].bInv ? !_bVal : _bVal ;

    AxtDIO.SetOut(m_pOut[_iNo].iAdd , bVal) ;
}

bool CIOs::GetOut(int _iNo )
{
    bool bRet = m_pOut[_iNo].bInv ? !AxtDIO.GetOut(m_pOut[_iNo].iAdd) : AxtDIO.GetOut(m_pOut[_iNo].iAdd) ;
    return bRet ;
}

bool CIOs::GetIn(int _iNo )
{
    bool bRet = m_pIn[_iNo].bInv ? !AxtDIO.GetIn(m_pIn[_iNo].iAdd) : AxtDIO.GetIn(m_pIn[_iNo].iAdd) ;
    return bRet ;
}

void CIOs::Update()
{
    //Static은 항상 주의 해야 한다. 배열로 선언되는 Class안에서는 사용 금지.
    bool bPreYVal = false ;
    bool bGetYVal = false ;

    bool bPreXVal = false ;
    bool bGetXVal = false ;



    //Output.
    for (int i = 0 ; i < m_iMaxOut ; i++) {
        //Check Delay.
        bPreYVal = GetOut(m_pOut[i].iAdd) ;

        if(m_pOut[i].iDelay && !m_pOutDelay[i].OnDelay(m_pOut[i].bSetVal != bPreYVal , m_pOut[i].iDelay)) continue ;

        //Set Output.
        //SetOut(m_pOut[i].iAdd , m_pOut[i].bSetVal); 실린더 초기화 동작 때문에 이렇게 함.
        bGetYVal = GetOut(m_pOut[i].iAdd) ;
        m_pOut[i].bGetVal = bGetYVal ;

        //Edge.
        if(bPreYVal != bGetYVal) {
            m_pOut[i].bUpEdge = !bPreYVal ;
            m_pOut[i].bDnEdge =  bPreYVal ;
        }
        else {
            m_pOut[i].bUpEdge = false ;
            m_pOut[i].bDnEdge = false ;
        }
    }

    //Input
/*    for (int i = 0 ; i < m_iMaxIn ; i++) {
        //Check
        bPreVal = m_pIn[i].bSetVal ;
        bGetVal = GetIn(m_pIn[i].iAdd) ;
        if(m_pIn[i].iDelay && !m_pInDelay[i].OnDelay(bGetVal != bPreVal , m_pIn[i].iDelay)) continue ;

        //Set Input.
        m_pIn[i].bSetVal = bGetVal ;
        m_pIn[i].bGetVal = bGetVal ;

         //Edge.
        if(bPreVal != bGetVal) {
            m_pIn[i].bUpEdge = !bPreVal ;
            m_pIn[i].bDnEdge =  bPreVal ;
        }
        else {
            m_pIn[i].bUpEdge = false ;
            m_pIn[i].bDnEdge = false ;
        }
    } */
    for (int i = 0 ; i < m_iMaxIn ; i++) {

        bGetXVal = GetIn(m_pIn[i].iAdd) ;
        bPreXVal = m_pIn[i].bGetVal ;
        m_pIn[i].bUpEdge = false ;
        m_pIn[i].bDnEdge = false ;

        //Check
        if(m_pIn[i].iDelay && !m_pInDelay[i].OnDelay(bGetXVal != bPreXVal , m_pIn[i].iDelay)) continue ;

        //bGetXVal = bTempGetXVal ;

         //Edge.
        if(bPreXVal != bGetXVal) {
            m_pIn[i].bUpEdge = !bPreXVal ;
            m_pIn[i].bDnEdge =  bPreXVal ;
        }

        //Set Input.
        m_pIn[i].bSetVal = bGetXVal ;
        m_pIn[i].bGetVal = bGetXVal ;
    }
}

void CIOs::SetY(int _iNo , bool _bVal , bool _bDirect)
{
    if( _iNo < 0 || _iNo >= m_iMaxOut) {/*ShowMessageT("Range is wrong");*/return ; }

    //if(_bDirect) SetOut(m_pOut[_iNo].iAdd , _bVal);
    SetOut(m_pOut[_iNo].iAdd , _bVal); //실린더 초기화 동작 때문에 이렇게 바꿈.
    m_pOut[_iNo].bSetVal = _bVal ;
}

bool CIOs::GetY(int _iNo )
{
    if( _iNo < 0 || _iNo >= m_iMaxOut) {/*ShowMessageT("Range is wrong");*/ return false; }

    return m_pOut[_iNo].bGetVal ;
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

    if(_bDirect) m_pIn[_iNo].bSetVal = GetIn(m_pIn[_iNo].iAdd);

    return m_pIn[_iNo].bSetVal ;
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

void CIOs::Load()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sPath2 ;
    AnsiString sIndex ;

    //Set Path.
    sPath  = EXE_FOLDER + "Util\\Input.INI";

    for (register int i = 0 ; i < m_iMaxIn  ; i++) {
        sIndex.printf("Input(%04d)" , i );
        UserINI.Load(sPath , "Address" , sIndex , m_pIn[i].iAdd  );
        UserINI.Load(sPath , "Invert " , sIndex , m_pIn[i].bInv  );
        UserINI.Load(sPath , "Name   " , sIndex , m_pIn[i].sName );
        UserINI.Load(sPath , "Comment" , sIndex , m_pIn[i].SComt );
        UserINI.Load(sPath , "Delay  " , sIndex , m_pIn[i].iDelay);
        Application -> ProcessMessages();
    }

    //Set Path.
    sPath  = EXE_FOLDER + "Util\\Output.INI";

    for (register int i = 0 ; i < m_iMaxOut  ; i++) {
        sIndex.printf("Output(%04d)" , i );
        UserINI.Load(sPath , "Address" , sIndex , m_pOut[i].iAdd  );
        UserINI.Load(sPath , "Invert " , sIndex , m_pOut[i].bInv  );
        UserINI.Load(sPath , "Name   " , sIndex , m_pOut[i].sName );
        UserINI.Load(sPath , "Comment" , sIndex , m_pOut[i].SComt );
        UserINI.Load(sPath , "Delay  " , sIndex , m_pOut[i].iDelay);
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

    UserINI.ClearFile(sPath) ;

    for (register int i = 0 ; i < m_iMaxIn  ; i++) {
        sIndex.printf("Input(%04d)" , i );
        UserINI.Save(sPath , "Address", sIndex  , m_pIn[i].iAdd  );
        UserINI.Save(sPath , "Invert ", sIndex  , m_pIn[i].bInv  );
        UserINI.Save(sPath , "Name   ", sIndex  , m_pIn[i].sName );
        UserINI.Save(sPath , "Comment", sIndex  , m_pIn[i].SComt );
        UserINI.Save(sPath , "Delay  ", sIndex  , m_pIn[i].iDelay);
        Application -> ProcessMessages();
    }

    //Set Path.
    sPath = EXE_FOLDER + "Util\\Output.INI";

    for (register int i = 0 ; i < m_iMaxOut  ; i++) {
        sIndex.printf("Output(%04d)" , i );
        UserINI.Save(sPath , "Address" , sIndex ,  m_pOut[i].iAdd  );
        UserINI.Save(sPath , "Invert " , sIndex ,  m_pOut[i].bInv  );
        UserINI.Save(sPath , "Name   " , sIndex ,  m_pOut[i].sName );
        UserINI.Save(sPath , "Comment" , sIndex ,  m_pOut[i].SComt );
        UserINI.Save(sPath , "Delay  " , sIndex ,  m_pOut[i].iDelay);
        Application -> ProcessMessages();
    }
}

void CIOs::LoadDnmVar()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    //Load Device.
    UserINI.Load(sPath , "CIOs"  , "m_iMaxIn " , m_iMaxIn );
    UserINI.Load(sPath , "CIOs"  , "m_iMaxOut" , m_iMaxOut);
}

void CIOs::SaveDnmVar()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    //Load Device.
    UserINI.Save(sPath , "CIOs"  , "m_iMaxIn " , m_iMaxIn );
    UserINI.Save(sPath , "CIOs"  , "m_iMaxOut" , m_iMaxOut);
}

void CIOs::DisplayList(bool _bOutput , TStringGrid *_sgTable , bool _bFixedWidth , bool _bDispHexAdd)
{
    //Local Var.
    AnsiString sAdd ;

    //Check Error.
    if (_sgTable == NULL) return;

    //Set StringGrid.
    _sgTable -> DoubleBuffered = true  ;
    _sgTable -> Options << goRowSelect ;
    _sgTable -> DefaultRowHeight = 20 ;
    _sgTable -> RowCount  = (_bOutput ? m_iMaxOut : m_iMaxIn ) + 1 ;
    _sgTable -> ColCount  = MAX_SG_COL  ;
    _sgTable -> FixedCols = 1  ;
    _sgTable -> FixedRows = 1  ;
    if(_bFixedWidth)_sgTable -> ColWidths[0] = 20 ;  _sgTable -> Cells[0][0] = "No"      ;
    if(_bFixedWidth)_sgTable -> ColWidths[1] = 50 ;  _sgTable -> Cells[1][0] = "Add"     ;
    if(_bFixedWidth)_sgTable -> ColWidths[2] = 110;  _sgTable -> Cells[2][0] = "Name"    ;
    if(_bFixedWidth)_sgTable -> ColWidths[3] = 20 ;  _sgTable -> Cells[3][0] = "Invert"  ;
    if(_bFixedWidth)_sgTable -> ColWidths[4] = 250;  _sgTable -> Cells[4][0] = "Comment" ;
    if(_bFixedWidth)_sgTable -> ColWidths[5] = 50 ;  _sgTable -> Cells[5][0] = "Delay"   ;
    if(_bFixedWidth)_sgTable -> ColWidths[6] = 30 ;  _sgTable -> Cells[6][0] = "Val"     ;
    if(_bFixedWidth)_sgTable -> ColWidths[7] = 30 ;  _sgTable -> Cells[7][0] = "Up"      ;

    //Display.
    if (_bOutput) {
        //Clear View Index.
        for (register int i = 0 ; i < m_iMaxOut ; i++) {

            //Display To List.
            if(_bDispHexAdd) sAdd = "Y" + sAdd.IntToHex(m_pOut[i].iAdd , 4);
            else             sAdd = "Y" + sAdd.sprintf ("%04d",m_pOut[i].iAdd) ;

            _sgTable -> Cells[0][i + 1] = i                ;
            _sgTable -> Cells[1][i + 1] = sAdd             ;
            _sgTable -> Cells[2][i + 1] = m_pOut[i].sName  ;
            _sgTable -> Cells[3][i + 1] = m_pOut[i].bInv ? 1 : 0 ;
            _sgTable -> Cells[4][i + 1] = m_pOut[i].SComt  ;
            _sgTable -> Cells[5][i + 1] = m_pOut[i].iDelay ;
            _sgTable -> Cells[6][i + 1] = m_pOut[i].bGetVal ? "ON" : "OFF";
            _sgTable -> Cells[7][i + 1] = m_pOut[i].bUpEdge ? "ON" : "OFF";
        }
    }
    else {
        //Clear View Index.
        for (register int i = 0 ; i < m_iMaxIn ; i++) {

            //Display To List.
            if(_bDispHexAdd) sAdd = "X" + sAdd.IntToHex(m_pIn[i].iAdd , 4);
            else             sAdd = "X" + sAdd.sprintf ("%04d",m_pIn[i].iAdd) ;

            _sgTable -> Cells[0][i + 1] = i                ;
            _sgTable -> Cells[1][i + 1] = sAdd             ;
            _sgTable -> Cells[2][i + 1] = m_pIn[i].sName  ;
            _sgTable -> Cells[3][i + 1] = m_pIn[i].bInv ? 1 : 0 ;
            _sgTable -> Cells[4][i + 1] = m_pIn[i].SComt  ;
            _sgTable -> Cells[5][i + 1] = m_pIn[i].iDelay ;
            _sgTable -> Cells[6][i + 1] = m_pIn[i].bGetVal ? "ON" : "OFF";
            _sgTable -> Cells[7][i + 1] = m_pIn[i].bUpEdge ? "ON" : "OFF";
        }
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
    _sgTable -> RowCount = (_bOutput ? m_iMaxOut : m_iMaxIn ) + 1 ;
    _sgTable -> ColCount = MAX_SG_COL ;

    //Display.
    if (_bOutput) {
        //Clear View Index.
        for (register int i = 0 ; i < m_iMaxOut ; i++) {
            _sgTable -> Cells[6][i + 1] = m_pOut[i].bGetVal ? "ON" : "OFF";
            _sgTable -> Cells[7][i + 1] = m_pOut[i].bUpEdge ? "ON" : "OFF";
        }
    }
    else {
        //Clear View Index.
        for (register int i = 0 ; i < m_iMaxIn ; i++) {
            _sgTable -> Cells[6][i + 1] = m_pIn[i].bGetVal ? "ON" : "OFF";
            _sgTable -> Cells[7][i + 1] = m_pIn[i].bUpEdge ? "ON" : "OFF";
        }
    }

}

void CIOs::DisplayProperty(bool          _bTable    ,
                           int           _bOutput   ,
                           TEdit       * _edIndex   ,
                           TEdit       * _edAdd     ,
                           TEdit       * _edName    ,
                           TEdit       * _edComt    ,
                           TEdit       * _edInv     ,
                           TEdit       * _edDelay   ,
                           TStringGrid *_sgTable    ,
                           bool         _bDispHexAdd)
{
    //Check Error.
    if (_sgTable == NULL || _edAdd  == NULL
                         || _edName == NULL
                         || _edComt == NULL
                         || _edInv  == NULL
                         || _edDelay== NULL) return;

    //Local Var.
    int iSelNo = _sgTable -> Row - 1 ;
    int iAdd   ;
    AnsiString sAdd ;

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
            if(_bDispHexAdd) sAdd = "Y" + sAdd.IntToHex(m_pOut[iSelNo].iAdd , 4);
            else             sAdd = "Y" + sAdd.sprintf ("%04d",m_pOut[iSelNo].iAdd) ;

            _edIndex -> Text = iSelNo                ;
            _edAdd   -> Text = sAdd                  ;
            _edName  -> Text = m_pOut[iSelNo].sName  ;
            _edComt  -> Text = m_pOut[iSelNo].SComt  ;
            _edInv   -> Text = m_pOut[iSelNo].bInv ? 1 : 0 ;
            _edDelay -> Text = m_pOut[iSelNo].iDelay ;
        }
        else {
            if(_bDispHexAdd) iAdd = ("0x"+_edAdd -> Text.Delete(1 , 1)).ToIntDef(iSelNo);
            else             iAdd = (     _edAdd -> Text.Delete(1 , 1)).ToIntDef(iSelNo);
            m_pOut[iSelNo].iAdd   = iAdd  ;
            m_pOut[iSelNo].sName  =_edName  -> Text  ;
            m_pOut[iSelNo].SComt  =_edComt  -> Text  ;
            m_pOut[iSelNo].bInv   =_edInv   -> Text.ToIntDef(0)  ;
            m_pOut[iSelNo].iDelay =_edDelay -> Text.ToIntDef(0)  ;
        }
    }
    else {
        if (_bTable) {
            //Display To List.
            if(_bDispHexAdd) sAdd = "X" + sAdd.IntToHex(m_pIn[iSelNo].iAdd , 4);
            else             sAdd = "X" + sAdd.sprintf ("%04d",m_pIn[iSelNo].iAdd) ;

            _edIndex -> Text = iSelNo               ;
            _edAdd   -> Text = sAdd                 ;
            _edName  -> Text = m_pIn[iSelNo].sName  ;
            _edComt  -> Text = m_pIn[iSelNo].SComt  ;
            _edInv   -> Text = m_pIn[iSelNo].bInv ? 1 : 0 ;
            _edDelay -> Text = m_pIn[iSelNo].iDelay ;
        }
        else {
            if(_bDispHexAdd) iAdd = ("0x"+_edAdd -> Text.Delete(1 , 1)).ToIntDef(iSelNo);
            else             iAdd = (     _edAdd -> Text.Delete(1 , 1)).ToIntDef(iSelNo);
            m_pIn[iSelNo].iAdd   = iAdd  ;
            m_pIn[iSelNo].sName  =_edName  -> Text  ;
            m_pIn[iSelNo].SComt  =_edComt  -> Text  ;
            m_pIn[iSelNo].bInv   =_edInv   -> Text.ToIntDef(0)  ;
            m_pIn[iSelNo].iDelay =_edDelay -> Text.ToIntDef(0)  ;
        }
    }
}



