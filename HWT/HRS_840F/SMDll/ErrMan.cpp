//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ErrMan.h"
#include "UserIni.h"
#include "SLogUnit.h"
#include "FormErr.h"

//#include "UserFile.h"

//#include "Graphics.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
CErrMan EM;

/***************************************************************************/
/* 생성자 & 소멸자. (Constructor & Destructor)                             */
/***************************************************************************/
//---------------------------------------------------------------------------
CErrMan::CErrMan(void)
{
    LoadDnmVar();

    Err = new TErr[m_iMaxErr] ;

    memset(Err , 0 , sizeof(TErr)*m_iMaxErr);

    Load   ();


    Clear();

    m_bShowForm     = false ;
}

//---------------------------------------------------------------------------
CErrMan::~CErrMan(void)
{
    delete [] Err ;
}

void CErrMan::Clear (void)
{
    Trace("","Error Clear");
    for(int i = 0 ; i < m_iMaxErr ; i++) Err[i].bOn = false ;

    m_iUpdated = false ;

    HideErr() ;
}

void CErrMan::SetDisp(bool _bOn)
{
    m_bShowForm = _bOn ;
}


void CErrMan::SetErr(int _iNo)
{
    if(_iNo < 0 || _iNo >= m_iMaxErr) {ShowMessageT("Err Range is Over"); return ; }

    AnsiString sErrNo = "ERROR_"+(AnsiString)_iNo ;

    if (Err[_iNo].bOn) return;

    Err[_iNo].bOn = true ;

    Err[_iNo].sErrMsg = "";

    if(m_bShowForm /* || (_iNo != m_iLastErr)*/) {
        ShowErr(_iNo) ;
        //FrmMain -> OnSendVisnMsg(mvMsg , ("ERR_"+Err[_iNo].sName).c_str()) ;
    }

    m_bShowForm = true ;

    m_iLastErr = _iNo ;

    Trace(sErrNo.c_str(),Err[_iNo].sName.c_str());
}

void CErrMan::SetErrMsg(int _iNo , AnsiString _sErrMsg)
{
    if(_iNo < 0 || _iNo >= m_iMaxErr) {ShowMessageT("Err Range is Over"); return ; }

    AnsiString sErrNo  = "ERROR_"+(AnsiString)_iNo ;

    if (Err[_iNo].bOn) return;

    Err[_iNo].bOn = true ;

    if(m_bShowForm /* || (_iNo != m_iLastErr)*/) {
        ShowErr(_iNo) ;
        //FrmMain -> OnSendVisnMsg(mvMsg , ("ERR_"+Err[_iNo].sName).c_str()) ;
    }

    m_bShowForm = true ;

    m_iLastErr = _iNo ;

    Err[_iNo].sErrMsg = _sErrMsg;

    Trace(sErrNo.c_str(),(Err[_iNo].sName +" Msg:"+ _sErrMsg).c_str());
}

AnsiString CErrMan::GetErrMsg (int _iNo)
{
    return Err[_iNo].sErrMsg ;

}

bool CErrMan::IsErr(void)
{
    for(int i = 0 ; i < m_iMaxErr ; i++) if(Err[i].bOn)return true ;

    return false ;
}

int CErrMan::GetLastErr() //에러 넘버 GetLastErrNo()
{
    return m_iLastErr ;
}

AnsiString CErrMan::GetErrName( int _iNo )
{
    return Err[_iNo].sName ;
}

AnsiString CErrMan::GetErrAction( int _iNo )
{
    return Err[_iNo].sAction ;
}

bool CErrMan::GetUsedErrPictr ()
{
    return m_bUsedErrPictr ;
}

AnsiString CErrMan::GetLastErrName()
{
    return Err[m_iLastErr].sName ;
}

void CErrMan::LoadDnmVar()
{
    //Local Var.
    TUserINI   UserINI  ;
    AnsiString sPath    ;
    int        iLangSel ;
    int        iUsedPictr;

    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    //Load Device.
    UserINI.Load(sPath , "CErrMan" , "m_iMaxErr " , m_iMaxErr        );
    UserINI.Load(sPath , "Member"  , "m_iLangSel" , iLangSel         ); m_iLangSel = (EN_LAN_SEL)iLangSel ;
    UserINI.Load(sPath , "Member"  , "m_bErrPict" , m_bUsedErrPictr  );
}

void CErrMan::SaveDnmVar()                                                                              
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;


    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    //Load Device.
    UserINI.Save(sPath , "CErrMan" , "m_iMaxErr " ,      m_iMaxErr );
    UserINI.Save(sPath , "Member"  , "m_iLangSel" , (int)m_iLangSel);
    UserINI.Save(sPath , "Member"  , "m_bErrPict" , m_bUsedErrPictr);
}

void CErrMan::Save()
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sPath   ;
    AnsiString sErrNo  ;
    AnsiString sSelLan ;

    //Set Dir.
    sPath  = EXE_FOLDER + "Util\\Err.INI"  ;
    //UserINI.ClearFile(sPath) ;

    switch(m_iLangSel) {
        default         : sSelLan = "E_" ; break ;
        case lsKorean   : sSelLan = "K_" ; break ;
        case lsEnglish  : sSelLan = "E_" ; break ;
        case lsChinese  : sSelLan = "C_" ; break ;
    }

    for(int i = 0 ; i < m_iMaxErr ; i++) {
        sErrNo.printf("ERR(%03d)" , i );      //999가 일단 끝.
        UserINI.Save(sPath,           "sEnum       ", sErrNo , Err[i].sEnum       );
        UserINI.Save(sPath, sSelLan + "sName       ", sErrNo , Err[i].sName       );
        UserINI.Save(sPath, sSelLan + "sAction     ", sErrNo , Err[i].sAction     );
        UserINI.Save(sPath,           "sImgPath    ", sErrNo , Err[i].sImgPath    );
        UserINI.Save(sPath,           "dRectLeft   ", sErrNo , Err[i].dRectLeft   );
        UserINI.Save(sPath,           "dRectTop    ", sErrNo , Err[i].dRectTop    );
        UserINI.Save(sPath,           "dRectWidth  ", sErrNo , Err[i].dRectWidth  );
        UserINI.Save(sPath,           "dRectHeight ", sErrNo , Err[i].dRectHeight );
    }
}

void CErrMan::Load()
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sPath   ;
    AnsiString sPath2  ;
    AnsiString sErrNo  ;
    AnsiString sSelLan ;

    //Set Dir.
    sPath  = EXE_FOLDER + "Util\\Err.INI" ;

    switch(m_iLangSel) {
        default         : sSelLan = "E_" ; break ;
        case lsKorean   : sSelLan = "K_" ; break ;
        case lsEnglish  : sSelLan = "E_" ; break ;
        case lsChinese  : sSelLan = "C_" ; break ;
    }

    for(int i = 0 ; i < m_iMaxErr ; i++) {
        sErrNo.printf("ERR(%03d)" , i );
        UserINI.Load(sPath,           "sEnum       ", sErrNo , Err[i].sEnum       );
        UserINI.Load(sPath, sSelLan + "sName       ", sErrNo , Err[i].sName       );
        UserINI.Load(sPath, sSelLan + "sAction     ", sErrNo , Err[i].sAction     );
        UserINI.Load(sPath,           "sImgPath    ", sErrNo , Err[i].sImgPath    );
        UserINI.Load(sPath,           "dRectLeft   ", sErrNo , Err[i].dRectLeft   );
        UserINI.Load(sPath,           "dRectTop    ", sErrNo , Err[i].dRectTop    );
        UserINI.Load(sPath,           "dRectWidth  ", sErrNo , Err[i].dRectWidth  );
        UserINI.Load(sPath,           "dRectHeight ", sErrNo , Err[i].dRectHeight );
    }
}

/*
void __fastcall CErrMan::SelLan(bool _bToTble , TComboBox * _cbLan) //랭귀지 선택.
{
    //Check Null.
    if (_cbLan == NULL ) return ;

    //Local Var.
    EN_LAN_SEL  iSelLan = (EN_LAN_SEL)_cbLan -> ItemIndex ;

    if (iSelLan == m_iLangSel) return ;

    //Display.
    if (_bToTble) {
        _cbLan -> ItemIndex = m_iLangSel ;
    }
    else {
        m_iLangSel =(EN_LAN_SEL)_cbLan -> ItemIndex ;
        SaveVar();
    }


}
*/

void __fastcall CErrMan::DisplayProperty(bool         _bToTble    , //스트링 그리드 외에 것 업데이트.
                                         TStringGrid *_sgTable    ,
                                         TEdit       *_edEnum     ,
                                         TEdit       *_edErrNo    ,
                                         TEdit       *_edName     ,
                                         TEdit       * _edImgPath ,
                                         TMemo       *_mmAction   ,
                                         TImage      *_imImage    ,
                                         TShape      *_spErrRect  )
{
    //Local Var.
    AnsiString Temp,Str;
    int        iIndex    ;
    int        iRect     ;
    int        Cnt,iCount;
    char       CChar     ;
    int        iPicWidth ;
    int        iPicHeight;

    iPicWidth  = _imImage -> Width  ;
    iPicHeight = _imImage -> Height ;

    //Check Null.
    if (_sgTable     == NULL ||
        _edEnum      == NULL ||
        _edErrNo     == NULL ||
        _edName      == NULL ||
         _edImgPath  == NULL ||
        _mmAction    == NULL ||
        _imImage     == NULL ||
        _spErrRect   == NULL
        ) return ;


    //Get Index.
    iIndex = _sgTable -> Row - 1;

    if(iIndex >= m_iMaxErr) return ;

    //Display.
    if (_bToTble) {
        Temp = Temp.sprintf("ERR%03d", iIndex );
        _edEnum    -> Text = Err[iIndex].sEnum;
        _edErrNo   -> Text = Temp;
        _edName    -> Text = Err[iIndex].sName;
        _edImgPath -> Text = Err[iIndex].sImgPath;
        _mmAction  -> Clear();

        //Display Solution
        Temp = Err[iIndex].sAction;
        Cnt = 0;
        CChar = '*';
        for(register int i = 1 ; i <= Temp.Length() ; i++) if (Temp[i] == CChar) iCount++;
        Cnt = iCount;

        for (int i=0; i <= Cnt ; i++) {
            AnsiString Line  = "";
            int n = Temp.Pos("*");
            Line  = Temp.SubString(1, n ? n-1 : n);
            Temp  = Temp.Delete(1, n);
            if (!n) { _mmAction -> Lines -> Add(Temp); break; }
            else    { _mmAction -> Lines -> Add(Line);        }
        }


        Temp = Err[iIndex].sImgPath ;
        if (_edImgPath -> Text == "" || !FileExists(_edImgPath -> Text)) {
            _imImage->Visible = false ;
        }
        else {
            _imImage->Visible = true ;
            _imImage -> Picture -> LoadFromFile(_edImgPath -> Text);
        }
        if (Err[iIndex].dRectWidth == 0.0|| Err[iIndex].dRectHeight == 0.0) {
            _spErrRect -> Left    = 1;
            _spErrRect -> Top     = 1;
            _spErrRect -> Width   = 60;
            _spErrRect -> Height  = 60;
        }
        else {
            _spErrRect -> Left    = (Err[iIndex].dRectLeft   * iPicWidth )/100.0 ;
            _spErrRect -> Top     = (Err[iIndex].dRectTop    * iPicHeight)/100.0 ;
            _spErrRect -> Width   = (Err[iIndex].dRectWidth  * iPicWidth )/100.0 ;
            _spErrRect -> Height  = (Err[iIndex].dRectHeight * iPicHeight)/100.0 ;
        }
    }
    else {
        Err[iIndex].sEnum    = _edEnum     -> Text;
        Err[iIndex].sName    = _edName     -> Text;
        Err[iIndex].sImgPath = _edImgPath  -> Text;

        Temp = "";
        for(register int i = 0 ; i < _mmAction -> Lines -> Count ; i ++) {
           Temp += _mmAction -> Lines -> Strings[i] ;
           if(_mmAction -> Lines -> Count - 1 != i) Temp += "*";
        }
        Err[iIndex].sAction = Temp ;
        Err[iIndex].dRectLeft   = (100 * _spErrRect->Left  ) / (double)iPicWidth  ;
        Err[iIndex].dRectTop    = (100 * _spErrRect->Top   ) / (double)iPicHeight ;
        Err[iIndex].dRectWidth  = (100 * _spErrRect->Width ) / (double)iPicWidth  ;
        Err[iIndex].dRectHeight = (100 * _spErrRect->Height) / (double)iPicHeight ;
    }
}

void __fastcall CErrMan::DisplayList(TStringGrid * _sgGrid) //스트링 그리드 업데이트.
{
    //Local Var.
    int        rCount;
    AnsiString Temp;

    //Check Error.
    if (_sgGrid == NULL) return;

    _sgGrid -> Options << goRowSelect ;
    _sgGrid -> DefaultRowHeight = 24 ;

    //Set StringGrid.
    rCount                 = m_iMaxErr;
    _sgGrid -> RowCount      = rCount + 1;  if(_sgGrid -> RowCount == 1) _sgGrid -> RowCount = 2 ;
    _sgGrid -> ColCount      = 3;
    _sgGrid -> ColWidths[0 ] = 34;
    _sgGrid -> ColWidths[1 ] = 200;
    _sgGrid -> ColWidths[2 ] = 250;

    //Set Col.
    _sgGrid -> Cells[0 ][0] = "No"      ;
    _sgGrid -> Cells[1 ][0] = "Name"    ;
    _sgGrid -> Cells[2 ][0] = "Action"  ;

    _sgGrid -> FixedCols = 1  ;
    _sgGrid -> FixedRows = 1  ;

    //Index No. Cell.
    //Display.
    for (int i = 0 ; i < rCount ; i++) {
        //Display.
        _sgGrid -> Cells[0 ][i + 1] = i;
        _sgGrid -> Cells[1 ][i + 1] = Err[i].sName;
        _sgGrid -> Cells[2 ][i + 1] = Err[i].sAction;
    }

    //Selected Row to 0.
    static bool bFirst = false ;
    if(bFirst) {_sgGrid -> TopRow = 1; _sgGrid -> Row = 1 ; }
    bFirst = false ;
}
TRect CErrMan::GetTRect(TStringGrid * _sgGrid)
{
    //Get Index.
    int        iIndex ;
    iIndex = _sgGrid -> Row - 1;

    TRect tRect;
    tRect.Left    = Err[iIndex].dRectLeft  ;
    tRect.Top     = Err[iIndex].dRectTop   ;
    tRect.Right   = Err[iIndex].dRectWidth ;
    tRect.Bottom  = Err[iIndex].dRectHeight;
    return tRect;
}

//---------------------------------------------------------------------------
void __fastcall CErrMan::DispErr(TListBox * ListBox)
{
    if(!ListBox) return ;

    //Local Var.
    AnsiString Str;
    static int iPreErrCnt  = 0 ;
    int        iCrntErrCnt ;

    iCrntErrCnt = GetErrCnt (    );

    if (iPreErrCnt != iCrntErrCnt ) {
        ListBox -> Clear();

        AnsiString Str;
        ListBox -> Items -> Clear();
        int iErrNo = GetLastErr();

        for (int i = 0 ; i < m_iMaxErr ; i++) {
            if (GetErr(i)) {
                Str.sprintf("[ERR%03d]" , i) ;
                Str += (Err[i].sName + " (Msg)" + Err[i].sErrMsg);
                ListBox -> Items -> Add(Str);
                if (iErrNo == i) ListBox -> ItemIndex = ListBox -> Count - 1 ;
            }
        }
    }
    iPreErrCnt = iCrntErrCnt ;
}

int   CErrMan::GetErrCnt (    )
{
    int  iCrntErrCnt ;
    for (int i = 0 ; i < m_iMaxErr ; i++) {
        if (Err[i].bOn) iCrntErrCnt++;
    }
    return iCrntErrCnt ;

}
int   CErrMan::GetMaxErr (    )
{
    return m_iMaxErr ;

}

bool  CErrMan::GetErr    (int _iNo)
{
    return Err[_iNo].bOn ;

}

void __fastcall CErrMan::DisplayErrForm (TLabel *_lbErrNo    ,
                                         TLabel *_lbName     ,
                                         TLabel *_lbEnum     ,
                                         TMemo  *_mmAction   ,
                                         TMemo  *_mmErrMsg   ,
                                         TImage *_imErr      ,
                                         TShape *_spErrRect  )
{
    //Local Var.
    AnsiString Temp,Str  ;
    int        iRect     ;
    int        Cnt,iCount;
    char       CChar     ;
    int        iErrNo    ;
    int        iPicWidth ;
    int        iPicHeight;

    iPicWidth  = _imErr -> Width  ;
    iPicHeight = _imErr -> Height ;


    iErrNo = GetLastErr() ;





    //Set Caption.
    Temp = Temp.sprintf("ERR%03d", iErrNo );
    _lbErrNo   -> Caption = Temp;
    _lbName    -> Caption = Err[iErrNo].sName;
    _lbEnum    -> Caption = Err[iErrNo].sEnum;
    _mmAction  -> Clear();
    _mmErrMsg  -> Clear();
    _mmErrMsg  -> Lines -> Add(Err[iErrNo].sErrMsg) ;

    //Display Solution
    Temp = Err[iErrNo].sAction;
    Cnt = 0;
    CChar = '*';
    for(register int i = 1 ; i <= Temp.Length() ; i++) if (Temp[i] == CChar) iCount++;
    Cnt = iCount;

    for (int i=0; i <= Cnt ; i++) {
        AnsiString Line  = "";
        int n = Temp.Pos("*");
        Line  = Temp.SubString(1, n ? n-1 : n);
        Temp  = Temp.Delete(1, n);
        if (!n) { _mmAction -> Lines -> Add(Temp); break; }
        else    { _mmAction -> Lines -> Add(Line);        }
    }


    Temp = Err[iErrNo].sImgPath ;
    if (Err[iErrNo].sImgPath == "" || !FileExists(Err[iErrNo].sImgPath)) {
        _imErr->Visible = false ;
    }
    else {
        _imErr -> Visible = true ;
        _imErr -> Picture -> LoadFromFile(Err[iErrNo].sImgPath);
    }
    if (Err[iErrNo].dRectWidth == 0|| Err[iErrNo].dRectHeight == 0) {
        _spErrRect -> Left   = 1;
        _spErrRect -> Top    = 1;
        _spErrRect -> Width  = 60;
        _spErrRect -> Height = 60;
    }
    else {
        _spErrRect -> Left   = (Err[iErrNo].dRectLeft   * iPicWidth )/100.0 ;
        _spErrRect -> Top    = (Err[iErrNo].dRectTop    * iPicHeight)/100.0 ;
        _spErrRect -> Width  = (Err[iErrNo].dRectWidth  * iPicWidth )/100.0 ;
        _spErrRect -> Height = (Err[iErrNo].dRectHeight * iPicHeight)/100.0 ;
    }
   // Err[iErrNo].sErrMsg = "";
}




