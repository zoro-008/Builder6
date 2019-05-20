//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "UserIni.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//AnsiString __fastcall GetExeDirPath(AnsiString FileName)
//{
//    return ExtractFilePath(Application -> ExeName) + FileName;
//}

//---------------------------------------------------------------------------
void __fastcall TUserINI::MakeDir(AnsiString Path)
{
    AnsiString sFileName ;
    AnsiString sDrectoty ;
    AnsiString sTotalDir ;
    AnsiString sRst      ;
    AnsiString sIniFName ;
    int        iDPos     ;       

    sFileName = Path;
    while(1) {
        iDPos = sFileName.Pos("\\");
        if (iDPos == 0 ) break;

        sDrectoty  = sFileName.SubString(1,iDPos);
        sTotalDir += sDrectoty;

        if(!DirectoryExists(sTotalDir)) CreateDir(sTotalDir);
        sRst = sFileName.Delete(1,iDPos);
    }

}

void __fastcall TUserINI::ClearFile(AnsiString Path)
{
    DeleteFile(Path);
}

void __fastcall TUserINI::Load(char *Path , char *Section , char *Name , double &Data)
{
    if (!Path)                return;
    if(!FileExists(Path)) MakeDir(Path);
    TIniFile *pINI = new TIniFile(Path);
    if (!pINI)                return;

    Data = pINI -> ReadFloat(Section , Name , false);
    delete pINI;

#ifdef _INIT_INI_
    Save(Path , Section , Name , Data);
#endif

}
void __fastcall TUserINI::Load(char *Path , char *Section , char *Name , double *Data)
{
    if (!Path)                return;
    if(!FileExists(Path)) MakeDir(Path);
    TIniFile *pINI = new TIniFile(Path);
    if (!pINI)                return;
    if (!Data)                return;

    *Data = pINI -> ReadFloat(Section , Name , false);
    delete pINI;
#ifdef _INIT_INI_
    Save(Path , Section , Name , Data);
#endif
}
void __fastcall TUserINI::Save(char *Path , char *Section , char *Name , double  Data)
{
    if (!Path)                return;
    if(!FileExists(Path)) MakeDir(Path);
    TIniFile *pINI = new TIniFile(Path);
    if (!pINI) {              return; }

    pINI -> WriteFloat(Section , Name , Data);
    delete pINI;

}
void __fastcall TUserINI::Save(char *Path , char *Section , char *Name , double *Data)
{
    if (!Path)                return;
    if(!FileExists(Path)) MakeDir(Path);
    TIniFile *pINI = new TIniFile(Path);
    if (!pINI) {              return; }

    pINI -> WriteFloat(Section , Name , *Data);
    delete pINI;
}
void __fastcall TUserINI::Load(char *Path , char *Section , char *Name , bool &Data)
{
    if (!Path)                return;
    if(!FileExists(Path)) MakeDir(Path);
    TIniFile *pINI = new TIniFile(Path);
    if (!pINI)                return;

    Data = pINI -> ReadBool(Section , Name , false);
    delete pINI;
#ifdef _INIT_INI_
    Save(Path , Section , Name , Data);
#endif
}
void __fastcall TUserINI::Load(char *Path , char *Section , char *Name , bool *Data)
{
    if (!Path)                return;
    if(!FileExists(Path)) MakeDir(Path);
    TIniFile *pINI = new TIniFile(Path);
    if (!pINI)                return;
    if (!Data)                return;

    *Data = pINI -> ReadBool(Section , Name , false);
    delete pINI;
#ifdef _INIT_INI_
    Save(Path , Section , Name , Data);
#endif
}
void __fastcall TUserINI::Save(char *Path , char *Section , char *Name , bool  Data)
{
    if (!Path)                return;
    if(!FileExists(Path)) MakeDir(Path);
    TIniFile *pINI = new TIniFile(Path);
    if (!pINI)                return;

    pINI -> WriteBool(Section , Name , Data);
    delete pINI;
}
void __fastcall TUserINI::Save(char *Path , char *Section , char *Name , bool *Data)
{
    if (!Path)                return;
    if(!FileExists(Path)) MakeDir(Path);
    TIniFile *pINI = new TIniFile(Path);
    if (!pINI)                return;

    pINI -> WriteBool(Section , Name , Data);
    delete pINI;
}
void __fastcall TUserINI::Load(char *Path , char *Section , char *Name , int &Data)
{
    if (!Path)                return;
    if(!FileExists(Path)) MakeDir(Path);
    TIniFile *pINI = new TIniFile(Path);
    if (!pINI)                return;

    Data = pINI -> ReadInteger(Section , Name , false);
    delete pINI;
#ifdef _INIT_INI_
    Save(Path , Section , Name , Data);
#endif
}
void __fastcall TUserINI::Load(char *Path , char *Section , char *Name , int *Data)
{
    if (!Path)                return;
    if(!FileExists(Path)) MakeDir(Path);
    TIniFile *pINI = new TIniFile(Path);
    if (!pINI)                return;
    if (!Data)                return;

    *Data = pINI -> ReadInteger(Section , Name , false);
    delete pINI;
#ifdef _INIT_INI_
    Save(Path , Section , Name , Data);
#endif
}
void __fastcall TUserINI::Save(char *Path , char *Section , char *Name , int  Data)
{
    if (!Path)                return;
    if(!FileExists(Path)) MakeDir(Path);
    TIniFile *pINI = new TIniFile(Path);
    if (!pINI)                return;

    pINI -> WriteInteger(Section , Name , Data);
    delete pINI;
}
void __fastcall TUserINI::Save(char *Path , char *Section , char *Name , int *Data)
{
    if (!Path)                return;
    if(!FileExists(Path)) MakeDir(Path);
    TIniFile *pINI = new TIniFile(Path);
    if (!pINI)                return;

    pINI -> WriteInteger(Section , Name , *Data);
    delete pINI;
}
void __fastcall TUserINI::Load(char *Path , char *Section , char *Name , AnsiString &Data)
{
    if (!Path)                return;
    if(!FileExists(Path)) MakeDir(Path);
    TIniFile *pINI = new TIniFile(Path);
    if (!pINI)                return;

    Data = pINI -> ReadString(Section , Name , "");
    delete pINI;
#ifdef _INIT_INI_
    Save(Path , Section , Name , Data);
#endif
}
void __fastcall TUserINI::Load(char *Path , char *Section , char *Name , AnsiString *Data)
{
    if (!Path)                return;
    if(!FileExists(Path)) MakeDir(Path);
    TIniFile *pINI = new TIniFile(Path);
    if (!pINI)                return;
    if (!Data)                return;

    *Data = pINI -> ReadString(Section , Name , "");
    delete pINI;
#ifdef _INIT_INI_
    Save(Path , Section , Name , Data);
#endif
}
void __fastcall TUserINI::Save(char *Path , char *Section , char *Name , AnsiString  Data)
{
    if (!Path)                return;
    if(!FileExists(Path)) MakeDir(Path);
    TIniFile *pINI = new TIniFile(Path);
    if (!pINI)                return;

    pINI -> WriteString(Section , Name , Data);
    delete pINI;
}
void __fastcall TUserINI::Save(char *Path , char *Section , char *Name , AnsiString *Data)
{
    if (!Path)                return;
    if(!FileExists(Path)) MakeDir(Path);
    TIniFile *pINI = new TIniFile(Path);
    if (!pINI)                return;

    pINI -> WriteString(Section , Name , *Data);
    delete pINI;
}
void __fastcall TUserINI::Load(AnsiString Path , char *Section , char *Name , double &Data)
{
    Load(Path.c_str() , Section, Name, Data);
}
void __fastcall TUserINI::Load(AnsiString Path , char *Section , char *Name , double *Data)
{
    Load(Path.c_str() , Section, Name, Data);
}
void __fastcall TUserINI::Save(AnsiString Path , char *Section , char *Name , double  Data)
{
    Save(Path.c_str() , Section, Name, Data);
}
void __fastcall TUserINI::Save(AnsiString Path , char *Section , char *Name , double *Data)
{
    Save(Path.c_str() , Section, Name, Data);
}
void __fastcall TUserINI::Load(AnsiString Path , char *Section , char *Name , bool &Data)
{
    Load(Path.c_str() , Section, Name, Data);
}
void __fastcall TUserINI::Load(AnsiString Path , char *Section , char *Name , bool *Data)
{
    Load(Path.c_str() , Section, Name, Data);
}
void __fastcall TUserINI::Save(AnsiString Path , char *Section , char *Name , bool  Data)
{
    Save(Path.c_str() , Section, Name, Data);
}
void __fastcall TUserINI::Save(AnsiString Path , char *Section , char *Name , bool *Data)
{
    Save(Path.c_str() , Section, Name, Data);
}
void __fastcall TUserINI::Load(AnsiString Path , char *Section , char *Name , int &Data)
{
    Load(Path.c_str() , Section, Name, Data);
}
void __fastcall TUserINI::Load(AnsiString Path , char *Section , char *Name , int *Data)
{
    Load(Path.c_str() , Section, Name, Data);
}
void __fastcall TUserINI::Save(AnsiString Path , char *Section , char *Name , int  Data)
{
    Save(Path.c_str() , Section, Name, Data);
}
void __fastcall TUserINI::Save(AnsiString Path , char *Section , char *Name , int *Data)
{
    Save(Path.c_str() , Section, Name, Data);
}
void __fastcall TUserINI::Load(AnsiString Path , char *Section , char *Name , AnsiString &Data)
{
    Load(Path.c_str() , Section, Name, Data);
}
void __fastcall TUserINI::Load(AnsiString Path , char *Section , char *Name , AnsiString *Data)
{
    Load(Path.c_str() , Section, Name, Data);
}
void __fastcall TUserINI::Save(AnsiString Path , char *Section , char *Name , AnsiString  Data)
{
    Save(Path.c_str() , Section, Name, Data);
}
void __fastcall TUserINI::Save(AnsiString Path , char *Section , char *Name , AnsiString *Data)
{
    Save(Path.c_str() , Section, Name, Data);
}
void __fastcall TUserINI::Load(AnsiString Path , AnsiString Section , char *Name , double &Data)
{
    Load(Path.c_str() , Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Load(AnsiString Path , AnsiString Section , char *Name , double *Data)
{
    Load(Path.c_str() , Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Save(AnsiString Path , AnsiString Section , char *Name , double  Data)
{
    Save(Path.c_str() , Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Save(AnsiString Path , AnsiString Section , char *Name , double *Data)
{
    Save(Path.c_str() , Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Load(AnsiString Path , AnsiString Section , char *Name , bool &Data)
{
    Load(Path.c_str() , Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Load(AnsiString Path , AnsiString Section , char *Name , bool *Data)
{
    Load(Path.c_str() , Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Save(AnsiString Path , AnsiString Section , char *Name , bool  Data)
{
    Save(Path.c_str() , Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Save(AnsiString Path , AnsiString Section , char *Name , bool *Data)
{
    Save(Path.c_str() , Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Load(AnsiString Path , AnsiString Section , char *Name , int &Data)
{
    Load(Path.c_str() , Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Load(AnsiString Path , AnsiString Section , char *Name , int *Data)
{
    Load(Path.c_str() , Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Save(AnsiString Path , AnsiString Section , char *Name , int  Data)
{
    Save(Path.c_str() , Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Save(AnsiString Path , AnsiString Section , char *Name , int *Data)
{
    Save(Path.c_str() , Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Load(AnsiString Path , AnsiString Section , char *Name , AnsiString &Data)
{
    Load(Path.c_str() , Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Load(AnsiString Path , AnsiString Section , char *Name , AnsiString *Data)
{
    Load(Path.c_str() , Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Save(AnsiString Path , AnsiString Section , char *Name , AnsiString  Data)
{
    Save(Path.c_str() , Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Save(AnsiString Path , AnsiString Section , char *Name , AnsiString *Data)
{
    Save(Path.c_str() , Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Load(AnsiString Path , AnsiString Section , AnsiString Name , double &Data)
{
    Load(Path.c_str() , Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Load(AnsiString Path , AnsiString Section , AnsiString Name , double *Data)
{
    Load(Path.c_str() , Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Save(AnsiString Path , AnsiString Section , AnsiString Name , double  Data)
{
    Save(Path.c_str() , Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Save(AnsiString Path , AnsiString Section , AnsiString Name , double *Data)
{
    Save(Path.c_str() , Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Load(AnsiString Path , AnsiString Section , AnsiString Name , bool &Data)
{
    Load(Path.c_str() , Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Load(AnsiString Path , AnsiString Section , AnsiString Name , bool *Data)
{
    Load(Path.c_str() , Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Save(AnsiString Path , AnsiString Section , AnsiString Name , bool  Data)
{
    Save(Path.c_str() , Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Save(AnsiString Path , AnsiString Section , AnsiString Name , bool *Data)
{
    Save(Path.c_str() , Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Load(AnsiString Path , AnsiString Section , AnsiString Name , int &Data)
{
    Load(Path.c_str() , Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Load(AnsiString Path , AnsiString Section , AnsiString Name , int *Data)
{
    Load(Path.c_str() , Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Save(AnsiString Path , AnsiString Section , AnsiString Name , int  Data)
{
    Save(Path.c_str() , Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Save(AnsiString Path , AnsiString Section , AnsiString Name , int *Data)
{
    Save(Path.c_str() , Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Load(AnsiString Path , AnsiString Section , AnsiString Name , AnsiString &Data)
{
    Load(Path.c_str() , Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Load(AnsiString Path , AnsiString Section , AnsiString Name , AnsiString *Data)
{
    Load(Path.c_str() , Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Save(AnsiString Path , AnsiString Section , AnsiString Name , AnsiString  Data)
{
    Save(Path.c_str() , Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Save(AnsiString Path , AnsiString Section , AnsiString Name , AnsiString *Data)
{
    Save(Path.c_str() , Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Load(char *Path , AnsiString Section , AnsiString Name , double &Data)
{
    Load(Path, Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Load(char *Path , AnsiString Section , AnsiString Name , double *Data)
{
    Load(Path, Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Save(char *Path , AnsiString Section , AnsiString Name , double  Data)
{
    Save(Path, Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Save(char *Path , AnsiString Section , AnsiString Name , double *Data)
{
    Save(Path, Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Load(char *Path , AnsiString Section , AnsiString Name , bool &Data)
{
    Load(Path, Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Load(char *Path , AnsiString Section , AnsiString Name , bool *Data)
{
    Load(Path, Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Save(char *Path , AnsiString Section , AnsiString Name , bool  Data)
{
    Save(Path, Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Save(char *Path , AnsiString Section , AnsiString Name , bool *Data)
{
    Save(Path, Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Load(char *Path , AnsiString Section , AnsiString Name , int &Data)
{
    Load(Path, Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Load(char *Path , AnsiString Section , AnsiString Name , int *Data)
{
    Load(Path, Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Save(char *Path , AnsiString Section , AnsiString Name , int  Data)
{
    Save(Path, Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Save(char *Path , AnsiString Section , AnsiString Name , int *Data)
{
    Save(Path, Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Load(char *Path , AnsiString Section , AnsiString Name , AnsiString &Data)
{
    Load(Path, Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Load(char *Path , AnsiString Section , AnsiString Name , AnsiString *Data)
{
    Load(Path, Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Save(char *Path , AnsiString Section , AnsiString Name , AnsiString  Data)
{
    Save(Path, Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Save(char *Path , AnsiString Section , AnsiString Name , AnsiString *Data)
{
    Save(Path, Section.c_str() , Name.c_str() , Data);
}
void __fastcall TUserINI::Load(char *Path , char *Section , AnsiString Name , double &Data)
{
    Load(Path, Section, Name.c_str() , Data);
}
void __fastcall TUserINI::Load(char *Path , char *Section , AnsiString Name , double *Data)
{
    Load(Path, Section, Name.c_str() , Data);
}
void __fastcall TUserINI::Save(char *Path , char *Section , AnsiString Name , double  Data)
{
    Save(Path, Section, Name.c_str() , Data);
}
void __fastcall TUserINI::Save(char *Path , char *Section , AnsiString Name , double *Data)
{
    Save(Path, Section, Name.c_str() , Data);
}
void __fastcall TUserINI::Load(char *Path , char *Section , AnsiString Name , bool &Data)
{
    Load(Path, Section, Name.c_str() , Data);
}
void __fastcall TUserINI::Load(char *Path , char *Section , AnsiString Name , bool *Data)
{
    Load(Path, Section, Name.c_str() , Data);
}
void __fastcall TUserINI::Save(char *Path , char *Section , AnsiString Name , bool  Data)
{
    Save(Path, Section, Name.c_str() , Data);
}
void __fastcall TUserINI::Save(char *Path , char *Section , AnsiString Name , bool *Data)
{
    Save(Path, Section, Name.c_str() , Data);
}
void __fastcall TUserINI::Load(char *Path , char *Section , AnsiString Name , int &Data)
{
    Load(Path, Section, Name.c_str() , Data);
}
void __fastcall TUserINI::Load(char *Path , char *Section , AnsiString Name , int *Data)
{
    Load(Path, Section, Name.c_str() , Data);
}
void __fastcall TUserINI::Save(char *Path , char *Section , AnsiString Name , int  Data)
{
    Save(Path, Section, Name.c_str() , Data);
}
void __fastcall TUserINI::Save(char *Path , char *Section , AnsiString Name , int *Data)
{
    Save(Path, Section, Name.c_str() , Data);
}
void __fastcall TUserINI::Load(char *Path , char *Section , AnsiString Name , AnsiString &Data)
{
    Load(Path, Section, Name.c_str() , Data);
}
void __fastcall TUserINI::Load(char *Path , char *Section , AnsiString Name , AnsiString *Data)
{
    Load(Path, Section, Name.c_str() , Data);
}
void __fastcall TUserINI::Save(char *Path , char *Section , AnsiString Name , AnsiString  Data)
{
    Save(Path, Section, Name.c_str() , Data);
}
void __fastcall TUserINI::Save(char *Path , char *Section , AnsiString Name , AnsiString *Data)
{
    Save(Path, Section, Name.c_str() , Data);
}
void __fastcall TUserINI::Load(char *Path , AnsiString Section , char *Name , double &Data)
{
    Load(Path, Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Load(char *Path , AnsiString Section , char *Name , double *Data)
{
    Load(Path, Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Save(char *Path , AnsiString Section , char *Name , double  Data)
{
    Save(Path, Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Save(char *Path , AnsiString Section , char *Name , double *Data)
{
    Save(Path, Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Load(char *Path , AnsiString Section , char *Name , bool &Data)
{
    Load(Path, Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Load(char *Path , AnsiString Section , char *Name , bool *Data)
{
    Load(Path, Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Save(char *Path , AnsiString Section , char *Name , bool  Data)
{
    Save(Path, Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Save(char *Path , AnsiString Section , char *Name , bool *Data)
{
    Save(Path, Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Load(char *Path , AnsiString Section , char *Name , int &Data)
{
    Load(Path, Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Load(char *Path , AnsiString Section , char *Name , int *Data)
{
    Load(Path, Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Save(char *Path , AnsiString Section , char *Name , int  Data)
{
    Save(Path, Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Save(char *Path , AnsiString Section , char *Name , int *Data)
{
    Save(Path, Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Load(char *Path , AnsiString Section , char *Name , AnsiString &Data)
{
    Load(Path, Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Load(char *Path , AnsiString Section , char *Name , AnsiString *Data)
{
    Load(Path, Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Save(char *Path , AnsiString Section , char *Name , AnsiString  Data)
{
    Save(Path, Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Save(char *Path , AnsiString Section , char *Name , AnsiString *Data)
{
    Save(Path, Section.c_str() , Name, Data);
}
void __fastcall TUserINI::Load(AnsiString Path , char *Section , AnsiString Name , double &Data)
{
    Load(Path.c_str() , Section, Name.c_str(), Data);
}
void __fastcall TUserINI::Load(AnsiString Path , char *Section , AnsiString Name , double *Data)
{
    Load(Path.c_str() , Section, Name.c_str(), Data);
}
void __fastcall TUserINI::Save(AnsiString Path , char *Section , AnsiString Name , double  Data)
{
    Save(Path.c_str() , Section, Name.c_str(), Data);
}
void __fastcall TUserINI::Save(AnsiString Path , char *Section , AnsiString Name , double *Data)
{
    Save(Path.c_str() , Section, Name.c_str(), Data);
}
void __fastcall TUserINI::Load(AnsiString Path , char *Section , AnsiString Name , bool &Data)
{
    Load(Path.c_str() , Section, Name.c_str(), Data);
}
void __fastcall TUserINI::Load(AnsiString Path , char *Section , AnsiString Name , bool *Data)
{
    Load(Path.c_str() , Section, Name.c_str(), Data);
}
void __fastcall TUserINI::Save(AnsiString Path , char *Section , AnsiString Name , bool  Data)
{
    Save(Path.c_str() , Section, Name.c_str(), Data);
}
void __fastcall TUserINI::Save(AnsiString Path , char *Section , AnsiString Name , bool *Data)
{
    Save(Path.c_str() , Section, Name.c_str(), Data);
}
void __fastcall TUserINI::Load(AnsiString Path , char *Section , AnsiString Name , int &Data)
{
    Load(Path.c_str() , Section, Name.c_str(), Data);
}
void __fastcall TUserINI::Load(AnsiString Path , char *Section , AnsiString Name , int *Data)
{
    Load(Path.c_str() , Section, Name.c_str(), Data);
}
void __fastcall TUserINI::Save(AnsiString Path , char *Section , AnsiString Name , int  Data)
{
    Save(Path.c_str() , Section, Name.c_str(), Data);
}
void __fastcall TUserINI::Save(AnsiString Path , char *Section , AnsiString Name , int *Data)
{
    Save(Path.c_str() , Section, Name.c_str(), Data);
}
void __fastcall TUserINI::Load(AnsiString Path , char *Section , AnsiString Name , AnsiString &Data)
{
    Load(Path.c_str() , Section, Name.c_str(), Data);
}
void __fastcall TUserINI::Load(AnsiString Path , char *Section , AnsiString Name , AnsiString *Data)
{
    Load(Path.c_str() , Section, Name.c_str(), Data);
}
void __fastcall TUserINI::Save(AnsiString Path , char *Section , AnsiString Name , AnsiString  Data)
{
    Save(Path.c_str() , Section, Name.c_str(), Data);
}
void __fastcall TUserINI::Save(AnsiString Path , char *Section , AnsiString Name , AnsiString *Data)
{
    Save(Path.c_str() , Section, Name.c_str(), Data);
}








//---------------------------------------------------------------------------
//void __fastcall TUserINI::Load(TIniFile *Ini , char *Section, char *Name, AnsiString *Data)
//{
//   if (!Ini ) return;
//   if (!Data) return;
//   *Data = Ini -> ReadString(Section , Name , "");
//}
////---------------------------------------------------------------------------
//void __fastcall TUserINI::Save(TIniFile *Ini, char *Section, char *Name, AnsiString Data)
//{
//   if (!Ini) return;
//   Ini -> WriteString(Section , Name , Data);
//}

//---------------------------------------------------------------------------
//void __fastcall TUserINI:: Load(AnsiString Path, AnsiString Section, AnsiString Name, AnsiString *Data)
//{
//    Load(Path.c_str() , Section.c_str() , Name.c_str() , Data);
//}
//
////---------------------------------------------------------------------------
//void __fastcall TUserINI:: Save(AnsiString Path, AnsiString Section, AnsiString Name, AnsiString  Data)
//{
//    Save(Path.c_str() , Section.c_str() , Name.c_str() , Data);
//}

