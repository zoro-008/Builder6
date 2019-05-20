//---------------------------------------------------------------------------
#pragma hdrstop

//---------------------------------------------------------------------------
#include "GlobalMan.h"
#include "UserIni.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define GLOBAL_FOLDER  ExtractFilePath(Application -> ExeName) + "\\Util\\Vision\\"
#define JOBFILE_FOLDER ExtractFilePath(Application -> ExeName) + "\\JobFile\\" + GM.Stat.sCrntJobFile + "\\Vision\\"
//----------------------------------------------------------------------------
CGlobalMan GM;

//---------------------------------------------------------------------------
CGlobalMan::CGlobalMan(void)
{
    Init();
}

//---------------------------------------------------------------------------
CGlobalMan::~CGlobalMan(void)
{
    Close();
}
//---------------------------------------------------------------------------
void CGlobalMan::Close()
{
    Save();
    DeleteCriticalSection(&csTsai);
}
//---------------------------------------------------------------------------
void CGlobalMan::Init(void)
{
    if(!FileExists(GLOBAL_FOLDER+String("Global.INI"))) Save();
    else                                                Load();

    InitializeCriticalSection(&csTsai);

    //if(!FileExists(ExtractFilePath(Application -> ExeName) + "\\Util\\Vision\\"+String("Global.INI"))) Save();
}

void CGlobalMan::Load()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= GLOBAL_FOLDER + "Global.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "Optn"  , "bUseDeviceCal" , Optn.bUseDeviceCal );

    UserINI.Load(sPath.c_str()  , "Stat"  , "sCrntJobFile " , Stat.sCrntJobFile  );
}
void CGlobalMan::Save()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= GLOBAL_FOLDER + "Global.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save Device.
    UserINI.Save(sPath.c_str()  , "Optn"  , "bUseDeviceCal" , Optn.bUseDeviceCal );

    UserINI.Save(sPath.c_str()  , "Stat"  , "sCrntJobFile " , Stat.sCrntJobFile  );
}

String CGlobalMan::GetGlobalFolder ()
{
    return GLOBAL_FOLDER ;
}

String CGlobalMan::GetJobFileFolder()
{
    return JOBFILE_FOLDER ;
}

void CGlobalMan::SetCrntJobFile(String _sJobFileName)
{
    Stat.sCrntJobFile = _sJobFileName ;

}

String CGlobalMan::GetCrntJobFile()
{
    return Stat.sCrntJobFile ;
}
