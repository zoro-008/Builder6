/****************************************************************************
* Copyright (c) 2000, LEE SANG HOON. All rights reserved.                   *
* Modify 2004/05/02 by Primer.                                              *
* Modify 2009/12/01 by JT Kim                                               *
* Modify 2010/12/15 by SUN Kye Won                                          *
****************************************************************************/

//---------------------------------------------------------------------------
#ifndef UserIniH
#define UserIniH

//---------------------------------------------------------------------------
#include <IniFiles.hpp>
#include <Forms.hpp>

//#define _INIT_INI_  //처음에 파일들 및 항목들 생성하고 싶으면 오픈하고 컴파일


//AnsiString __fastcall GetExeDirPath(AnsiString FileName);

#define EXE_FOLDER ExtractFilePath(Application -> ExeName)


//---------------------------------------------------------------------------
class TUserINI
{

    protected :

    public :
        __fastcall  TUserINI(void) { }
        __fastcall ~TUserINI(void) { }

        void __fastcall MakeDir  (AnsiString Path);
        void __fastcall ClearFile(AnsiString Path);

        void __fastcall Load(char      *Path , char      *Section , char      *Name , double     &Data);
        void __fastcall Load(char      *Path , char      *Section , char      *Name , double     *Data);
        void __fastcall Save(char      *Path , char      *Section , char      *Name , double      Data);
        void __fastcall Save(char      *Path , char      *Section , char      *Name , double     *Data);

        void __fastcall Load(char      *Path , char      *Section , char      *Name , bool       &Data);
        void __fastcall Load(char      *Path , char      *Section , char      *Name , bool       *Data);
        void __fastcall Save(char      *Path , char      *Section , char      *Name , bool        Data);
        void __fastcall Save(char      *Path , char      *Section , char      *Name , bool       *Data);

        void __fastcall Load(char      *Path , char      *Section , char      *Name , int        &Data);
        void __fastcall Load(char      *Path , char      *Section , char      *Name , int        *Data);
        void __fastcall Save(char      *Path , char      *Section , char      *Name , int         Data);
        void __fastcall Save(char      *Path , char      *Section , char      *Name , int        *Data);

        void __fastcall Load(char      *Path , char      *Section , char      *Name , AnsiString &Data);
        void __fastcall Load(char      *Path , char      *Section , char      *Name , AnsiString *Data);
        void __fastcall Save(char      *Path , char      *Section , char      *Name , AnsiString  Data);
        void __fastcall Save(char      *Path , char      *Section , char      *Name , AnsiString *Data);

        void __fastcall Load(AnsiString Path , char      *Section , char      *Name , double     &Data);
        void __fastcall Load(AnsiString Path , char      *Section , char      *Name , double     *Data);
        void __fastcall Save(AnsiString Path , char      *Section , char      *Name , double      Data);
        void __fastcall Save(AnsiString Path , char      *Section , char      *Name , double     *Data);

        void __fastcall Load(AnsiString Path , char      *Section , char      *Name , bool       &Data);
        void __fastcall Load(AnsiString Path , char      *Section , char      *Name , bool       *Data);
        void __fastcall Save(AnsiString Path , char      *Section , char      *Name , bool        Data);
        void __fastcall Save(AnsiString Path , char      *Section , char      *Name , bool       *Data);

        void __fastcall Load(AnsiString Path , char      *Section , char      *Name , int        &Data);
        void __fastcall Load(AnsiString Path , char      *Section , char      *Name , int        *Data);
        void __fastcall Save(AnsiString Path , char      *Section , char      *Name , int         Data);
        void __fastcall Save(AnsiString Path , char      *Section , char      *Name , int        *Data);

        void __fastcall Load(AnsiString Path , char      *Section , char      *Name , AnsiString &Data);
        void __fastcall Load(AnsiString Path , char      *Section , char      *Name , AnsiString *Data);
        void __fastcall Save(AnsiString Path , char      *Section , char      *Name , AnsiString  Data);
        void __fastcall Save(AnsiString Path , char      *Section , char      *Name , AnsiString *Data);

        void __fastcall Load(AnsiString Path , AnsiString Section , char      *Name , double     &Data);
        void __fastcall Load(AnsiString Path , AnsiString Section , char      *Name , double     *Data);
        void __fastcall Save(AnsiString Path , AnsiString Section , char      *Name , double      Data);
        void __fastcall Save(AnsiString Path , AnsiString Section , char      *Name , double     *Data);

        void __fastcall Load(AnsiString Path , AnsiString Section , char      *Name , bool       &Data);
        void __fastcall Load(AnsiString Path , AnsiString Section , char      *Name , bool       *Data);
        void __fastcall Save(AnsiString Path , AnsiString Section , char      *Name , bool        Data);
        void __fastcall Save(AnsiString Path , AnsiString Section , char      *Name , bool       *Data);

        void __fastcall Load(AnsiString Path , AnsiString Section , char      *Name , int        &Data);
        void __fastcall Load(AnsiString Path , AnsiString Section , char      *Name , int        *Data);
        void __fastcall Save(AnsiString Path , AnsiString Section , char      *Name , int         Data);
        void __fastcall Save(AnsiString Path , AnsiString Section , char      *Name , int        *Data);

        void __fastcall Load(AnsiString Path , AnsiString Section , char      *Name , AnsiString &Data);
        void __fastcall Load(AnsiString Path , AnsiString Section , char      *Name , AnsiString *Data);
        void __fastcall Save(AnsiString Path , AnsiString Section , char      *Name , AnsiString  Data);
        void __fastcall Save(AnsiString Path , AnsiString Section , char      *Name , AnsiString *Data);

        void __fastcall Load(AnsiString Path , AnsiString Section , AnsiString Name , double     &Data);
        void __fastcall Load(AnsiString Path , AnsiString Section , AnsiString Name , double     *Data);
        void __fastcall Save(AnsiString Path , AnsiString Section , AnsiString Name , double      Data);
        void __fastcall Save(AnsiString Path , AnsiString Section , AnsiString Name , double     *Data);

        void __fastcall Load(AnsiString Path , AnsiString Section , AnsiString Name , bool       &Data);
        void __fastcall Load(AnsiString Path , AnsiString Section , AnsiString Name , bool       *Data);
        void __fastcall Save(AnsiString Path , AnsiString Section , AnsiString Name , bool        Data);
        void __fastcall Save(AnsiString Path , AnsiString Section , AnsiString Name , bool       *Data);

        void __fastcall Load(AnsiString Path , AnsiString Section , AnsiString Name , int        &Data);
        void __fastcall Load(AnsiString Path , AnsiString Section , AnsiString Name , int        *Data);
        void __fastcall Save(AnsiString Path , AnsiString Section , AnsiString Name , int         Data);
        void __fastcall Save(AnsiString Path , AnsiString Section , AnsiString Name , int        *Data);

        void __fastcall Load(AnsiString Path , AnsiString Section , AnsiString Name , AnsiString &Data);
        void __fastcall Load(AnsiString Path , AnsiString Section , AnsiString Name , AnsiString *Data);
        void __fastcall Save(AnsiString Path , AnsiString Section , AnsiString Name , AnsiString  Data);
        void __fastcall Save(AnsiString Path , AnsiString Section , AnsiString Name , AnsiString *Data);

        void __fastcall Load(char     * Path , AnsiString Section , AnsiString Name , double     &Data);
        void __fastcall Load(char     * Path , AnsiString Section , AnsiString Name , double     *Data);
        void __fastcall Save(char     * Path , AnsiString Section , AnsiString Name , double      Data);
        void __fastcall Save(char     * Path , AnsiString Section , AnsiString Name , double     *Data);

        void __fastcall Load(char     * Path , AnsiString Section , AnsiString Name , bool       &Data);
        void __fastcall Load(char     * Path , AnsiString Section , AnsiString Name , bool       *Data);
        void __fastcall Save(char     * Path , AnsiString Section , AnsiString Name , bool        Data);
        void __fastcall Save(char     * Path , AnsiString Section , AnsiString Name , bool       *Data);

        void __fastcall Load(char     * Path , AnsiString Section , AnsiString Name , int        &Data);
        void __fastcall Load(char     * Path , AnsiString Section , AnsiString Name , int        *Data);
        void __fastcall Save(char     * Path , AnsiString Section , AnsiString Name , int         Data);
        void __fastcall Save(char     * Path , AnsiString Section , AnsiString Name , int        *Data);

        void __fastcall Load(char     * Path , AnsiString Section , AnsiString Name , AnsiString &Data);
        void __fastcall Load(char     * Path , AnsiString Section , AnsiString Name , AnsiString *Data);
        void __fastcall Save(char     * Path , AnsiString Section , AnsiString Name , AnsiString  Data);
        void __fastcall Save(char     * Path , AnsiString Section , AnsiString Name , AnsiString *Data);

        void __fastcall Load(char     * Path , char     * Section , AnsiString Name , double     &Data);
        void __fastcall Load(char     * Path , char     * Section , AnsiString Name , double     *Data);
        void __fastcall Save(char     * Path , char     * Section , AnsiString Name , double      Data);
        void __fastcall Save(char     * Path , char     * Section , AnsiString Name , double     *Data);

        void __fastcall Load(char     * Path , char     * Section , AnsiString Name , bool       &Data);
        void __fastcall Load(char     * Path , char     * Section , AnsiString Name , bool       *Data);
        void __fastcall Save(char     * Path , char     * Section , AnsiString Name , bool        Data);
        void __fastcall Save(char     * Path , char     * Section , AnsiString Name , bool       *Data);

        void __fastcall Load(char     * Path , char     * Section , AnsiString Name , int        &Data);
        void __fastcall Load(char     * Path , char     * Section , AnsiString Name , int        *Data);
        void __fastcall Save(char     * Path , char     * Section , AnsiString Name , int         Data);
        void __fastcall Save(char     * Path , char     * Section , AnsiString Name , int        *Data);

        void __fastcall Load(char     * Path , char     * Section , AnsiString Name , AnsiString &Data);
        void __fastcall Load(char     * Path , char     * Section , AnsiString Name , AnsiString *Data);
        void __fastcall Save(char     * Path , char     * Section , AnsiString Name , AnsiString  Data);
        void __fastcall Save(char     * Path , char     * Section , AnsiString Name , AnsiString *Data);

        void __fastcall Load(char     * Path , AnsiString Section , char     * Name , double     &Data);
        void __fastcall Load(char     * Path , AnsiString Section , char     * Name , double     *Data);
        void __fastcall Save(char     * Path , AnsiString Section , char     * Name , double      Data);
        void __fastcall Save(char     * Path , AnsiString Section , char     * Name , double     *Data);

        void __fastcall Load(char     * Path , AnsiString Section , char     * Name , bool       &Data);
        void __fastcall Load(char     * Path , AnsiString Section , char     * Name , bool       *Data);
        void __fastcall Save(char     * Path , AnsiString Section , char     * Name , bool        Data);
        void __fastcall Save(char     * Path , AnsiString Section , char     * Name , bool       *Data);

        void __fastcall Load(char     * Path , AnsiString Section , char     * Name , int        &Data);
        void __fastcall Load(char     * Path , AnsiString Section , char     * Name , int        *Data);
        void __fastcall Save(char     * Path , AnsiString Section , char     * Name , int         Data);
        void __fastcall Save(char     * Path , AnsiString Section , char     * Name , int        *Data);

        void __fastcall Load(char     * Path , AnsiString Section , char     * Name , AnsiString &Data);
        void __fastcall Load(char     * Path , AnsiString Section , char     * Name , AnsiString *Data);
        void __fastcall Save(char     * Path , AnsiString Section , char     * Name , AnsiString  Data);
        void __fastcall Save(char     * Path , AnsiString Section , char     * Name , AnsiString *Data);

        void __fastcall Load(AnsiString Path , char     * Section , AnsiString Name , double     &Data);
        void __fastcall Load(AnsiString Path , char     * Section , AnsiString Name , double     *Data);
        void __fastcall Save(AnsiString Path , char     * Section , AnsiString Name , double      Data);
        void __fastcall Save(AnsiString Path , char     * Section , AnsiString Name , double     *Data);

        void __fastcall Load(AnsiString Path , char     * Section , AnsiString Name , bool       &Data);
        void __fastcall Load(AnsiString Path , char     * Section , AnsiString Name , bool       *Data);
        void __fastcall Save(AnsiString Path , char     * Section , AnsiString Name , bool        Data);
        void __fastcall Save(AnsiString Path , char     * Section , AnsiString Name , bool       *Data);

        void __fastcall Load(AnsiString Path , char     * Section , AnsiString Name , int        &Data);
        void __fastcall Load(AnsiString Path , char     * Section , AnsiString Name , int        *Data);
        void __fastcall Save(AnsiString Path , char     * Section , AnsiString Name , int         Data);
        void __fastcall Save(AnsiString Path , char     * Section , AnsiString Name , int        *Data);
                                                                                
        void __fastcall Load(AnsiString Path , char     * Section , AnsiString Name , AnsiString &Data);
        void __fastcall Load(AnsiString Path , char     * Section , AnsiString Name , AnsiString *Data);
        void __fastcall Save(AnsiString Path , char     * Section , AnsiString Name , AnsiString  Data);
        void __fastcall Save(AnsiString Path , char     * Section , AnsiString Name , AnsiString *Data);

};

#endif
