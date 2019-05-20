//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ErrMan.h"
#include "UserIni.h"
#include "SLogUnit.h"

#include "FormErr.h"
//#include "FormMain.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
CErrMan EM;

/***************************************************************************/
/* 持失切 & 社瑚切. (Constructor & Destructor)                             */
/***************************************************************************/
//---------------------------------------------------------------------------
CErrMan::CErrMan(void)
{
    LoadDnmVar();

    Err = new TErr[m_iMaxErr] ;

    memset(Err , 0 , sizeof(TErr)*m_iMaxErr);

    //Load();

    Clear();

    m_bShowForm = true ;
}

//---------------------------------------------------------------------------
CErrMan::~CErrMan(void)
{
    Save();
    delete [] Err ;
}

void CErrMan::Clear (void)
{
    Trace("","Error Clear");
    for(int i = 0 ; i < m_iMaxErr ; i++) Err[i].m_bOn = false ;

    HideErr() ;
}

void CErrMan::ClearPart(int _iPart)
{
    Trace("","Error Clear");
    for(int i = 0 ; i < m_iMaxErr ; i++) {if(Err[i].m_iPart == _iPart) Err[i].m_bOn = false ;}

    if(!IsErr()) HideErr() ;
}

void CErrMan::SetErr(int _iNo)
{
    if(_iNo < 0 || _iNo >= m_iMaxErr) {ShowMessageT("Err Range is Over"); return ; }

    AnsiString sErrNo = "ERROR_"+(AnsiString)_iNo ;

    if (Err[_iNo].m_bOn) return;

    Err[_iNo].m_bOn = true ;
    Err[_iNo].m_sSetTime = Now().FormatString("yyyy-mm-dd hh:nn:ss");
    Err[_iNo].m_iOcrCnt  += 1 ;



    if(!m_bShowForm /* || (_iNo != m_iLastErr)*/) {
        ShowErr(_iNo , Err[_iNo].m_sCmmt , Err[_iNo].m_sSolut , Err[_iNo].m_sSetTime) ;
        //FrmMain -> OnSendVisnMsg(mvMsg , ("ERR_"+Err[_iNo].m_sName).c_str()) ;
    }

    m_bShowForm = true ;

    m_iLastErr = _iNo ;

    Trace("SetErr",Err[_iNo].m_sName.c_str());
}

bool CErrMan::IsErr(void)
{
    for(int i = 0 ; i < m_iMaxErr ; i++) if(Err[i].m_bOn)return true ;

    return false ;
}

int CErrMan::GetLastErr()
{
    return m_iLastErr ;
}

AnsiString CErrMan::GetErrName( int _iNo )
{
    return Err[_iNo].m_sName ;
}

AnsiString CErrMan::GetLastErrName()
{
    return Err[m_iLastErr].m_sName ;
}

AnsiString CErrMan::GetErrCmnt(int _iNo)
{
    return Err[_iNo].m_sCmmt ;
}

bool CErrMan::IsPartErr( int _iPart )
{
    for(int i = 0 ; i < m_iMaxErr ; i++) {
        if(Err[i].m_bOn && Err[i].m_iPart == _iPart)
            return true ;
    }

    return false ;
}



int CErrMan::GetOcrCnt(int _iNo)
{
    return Err[_iNo].m_iOcrCnt ;

}

void CErrMan::SetOcrCnt(int _iNo , int _iCnt)
{

    Err[_iNo].m_iOcrCnt = _iCnt ;

}

void CErrMan::ClearOcrCnt(void)
{
    for(int i = 0 ; i < m_iMaxErr ; i++) {
        Err[i].m_iOcrCnt = 0 ;
    }
}

int CErrMan::GetMaxErrCnt(void)
{
    return m_iMaxErr ;
}


void CErrMan::LoadDnmVar()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    //Load Device.
    UserINI.Load(sPath , "CErrMan" , "m_iMaxErr" , m_iMaxErr);
}

void CErrMan::SaveDnmVar()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    //Load Device.
    UserINI.Save(sPath , "CErrMan" , "m_iMaxErr" , m_iMaxErr);
}

void CErrMan::Save()
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sPath   ;
    AnsiString sErrNo  ;

    //Set Dir.
    sPath  = EXE_FOLDER + "Util\\Err.INI"  ;



    //UserINI.ClearFile(sPath) ;

    for(int i = 0 ; i < m_iMaxErr ; i++) {
        sErrNo.printf("ERR(%03d)" , i );
        //UserINI.Save(sPath, "m_sName  ", sErrNo , Err[i].m_sName  );
        //UserINI.Save(sPath, "m_sCmmt  ", sErrNo , Err[i].m_sCmmt  );
        //UserINI.Save(sPath, "m_sSolut ", sErrNo , Err[i].m_sSolut );
        //UserINI.Save(sPath, "m_iPart  ", sErrNo , Err[i].m_iPart  );
        UserINI.Save(sPath, "m_iOcrCnt", sErrNo , Err[i].m_iOcrCnt);
    }
}

void CErrMan::Load()
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sPath   ;
    AnsiString sPath2  ;
    AnsiString sErrNo  ;

    //Set Dir.
    sPath  = EXE_FOLDER + "Util\\Err.INI" ;

    for(int i = 0 ; i < m_iMaxErr ; i++) {
        sErrNo.printf("ERR(%03d)" , i );
        UserINI.Load(sPath , "m_sName " , sErrNo , Err[i].m_sName  );
        UserINI.Load(sPath , "m_sCmmt " , sErrNo , Err[i].m_sCmmt  );
        UserINI.Load(sPath , "m_sSolut" , sErrNo , Err[i].m_sSolut );
        UserINI.Load(sPath , "m_iPart " , sErrNo , Err[i].m_iPart  );
        UserINI.Load(sPath , "m_iOcrCnt", sErrNo , Err[i].m_iOcrCnt);
    }
}














