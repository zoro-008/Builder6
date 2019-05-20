//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "UserFile.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

TUserFile UserFile ;

/***************************************************************************/
/* Global Functions.                                                       */
/***************************************************************************/
//---------------------------------------------------------------------------
AnsiString __fastcall NormalDir(AnsiString Name)
{
    //Local Var.
    AnsiString LastStr = Name.SubString(Name.Length() , 1);

    //Return.
    if (LastStr != "\\") Name += "\\";
    return Name;
}

//---------------------------------------------------------------------------
AnsiString __fastcall GetExeDir()
{
    return ExtractFilePath(Application->ExeName);
}
//---------------------------------------------------------------------------
AnsiString __fastcall GetExeDir(AnsiString FileName)
{
    return ExtractFilePath(Application->ExeName)+FileName;
}



/***************************************************************************/
/* File Attributes.                                                        */
/***************************************************************************/
//---------------------------------------------------------------------------
int  __fastcall TUserFile::GetFileSize(AnsiString FileName)
{
    //Local Var.
    int        Size;
    TSearchRec sr;

    //Find & GetSize.
    if (FindFirst(ExpandFileName(FileName) , faAnyFile , sr) == 0) Size = sr.Size;
    else                                                           Size = -1;
    FindClose(sr);

    //Return
    return Size;
}

//---------------------------------------------------------------------------
TDateTime __fastcall TUserFile::GetFileDateTime(AnsiString FileName)
{
   //Local Var.
   long int Age = FileAge(FileName);

   //Get Date.
   if (Age == -1) return 0.0;
   else           return FileDateToDateTime(Age);
}

//---------------------------------------------------------------------------
bool __fastcall TUserFile::HasAttr(AnsiString FileName, int Attr)
{
   //Local Var.
   int FileAttr = FileGetAttr(FileName);

   //File Attr.
   return (FileAttr >= 0) & ((FileAttr & Attr) == Attr);
}

/***************************************************************************/
/* Directory Processing Functions.                                         */
/***************************************************************************/
//---------------------------------------------------------------------------
bool __fastcall TUserFile::DirExists(AnsiString Name)
{
    return DirectoryExists(Name);
}

//---------------------------------------------------------------------------
bool __fastcall TUserFile::BrowseDirectory(AnsiString &AFolderName)
{
    return SelectDirectory(AFolderName , TSelectDirOpts() << sdAllowCreate << sdPerformCreate << sdPrompt , 0);
}

//---------------------------------------------------------------------------
bool __fastcall TUserFile::CreateDir(AnsiString Dir)
{
    if (!DirectoryExists(Dir)) {
        if (!ForceDirectories(Dir)) return false;
        else                        return true;
        }
    return false;
}

//---------------------------------------------------------------------------
bool __fastcall TUserFile::CopyDir(AnsiString FileName , AnsiString DestName)
{
    //Local Var.
    bool       Result   ;
    int        DosCode  ;
    AnsiString SerchFile;
    TSearchRec sr       ;

    //Check Exist source dir.
    if (!(Result = DirExists(FileName))) return 0;

    //Copy Dir.
    SerchFile = ExpandFileName(NormalDir(FileName) + "*.*");
    CreateDir(DestName);
    DosCode = FindFirst(SerchFile , faAnyFile , sr);
    while (DosCode == 0) {
        //if ((sr.Name == ".") || (sr.Name == "..")) {
        if ((sr.Name[1] != '.') && (sr.Attr != faVolumeID)) {
            if ((sr.Attr & faDirectory) ==  faDirectory) {
                CreateDir(NormalDir(DestName) + sr.Name);
                Result =  CopyDir(NormalDir(FileName) + sr.Name , NormalDir(DestName) + sr.Name) & Result;
                }
            else if ((sr.Attr & faVolumeID) !=  faVolumeID) {
                if ((sr.Attr & faReadOnly) ==  faReadOnly) FileSetAttr(NormalDir(FileName)  + sr.Name, faArchive);
                Result =  FileCopy(NormalDir(FileName) + sr.Name, NormalDir(DestName) + sr.Name ) & Result;
                }
            }
        DosCode = FindNext(sr);
        }
    FindClose(sr);

    //Return Code.
    return 1;
}

//---------------------------------------------------------------------------
bool __fastcall TUserFile::RenameDir(AnsiString FileName, AnsiString DestName)
{
    return RenameFile(FileName , DestName);
}

//---------------------------------------------------------------------------
bool __fastcall TUserFile::MoveDir(AnsiString FileName, AnsiString DestName)
{
    //Local Var.
    bool       Result   ;
    int        DosCode  ;
    AnsiString SerchFile;
    TSearchRec sr       ;

    //Check Exist source dir.
    if (!(Result = DirExists(FileName))) return 0;

    //Move Dir.
    SerchFile = ExpandFileName(NormalDir(FileName) + "*.*");
    CreateDir(DestName);
    DosCode = FindFirst(SerchFile , faAnyFile , sr);
    while(DosCode == 0) {
        //if ((sr.Name == ".") || (sr.Name == "..")) {
        if ((sr.Name[1] != '.') && (sr.Attr != faVolumeID)) {
            if ((sr.Attr & faDirectory) ==  faDirectory) {
                CreateDir(NormalDir(DestName) + sr.Name);
                Result =  MoveDir(NormalDir(FileName) + sr.Name , NormalDir(DestName) + sr.Name) & Result;
                }
            else if ((sr.Attr & faVolumeID) !=  faVolumeID) {
                if ((sr.Attr & faReadOnly) ==  faReadOnly) FileSetAttr(NormalDir(FileName)  + sr.Name, faArchive);
                Result =  FileCopy(NormalDir(FileName) + sr.Name, NormalDir(DestName) + sr.Name ) & Result;
                }
            }
        DosCode = FindNext(sr);
        }
    FindClose(sr);

    //Delete Source Dir.
    ClearDir(FileName , 1);

    //Return Code.
    return 1;
}

//---------------------------------------------------------------------------
bool __fastcall TUserFile::ClearDir(AnsiString Path, bool Delete)
{
    //Local Var.
    bool       Result   ;
    int        DosCode  ;
    int        FileNotFound = 18;
    AnsiString FileName ;
    TSearchRec sr       ;

    //Check Exist source dir.
    if (!(Result = DirExists(Path))) return 0;

    //Clear Dir.
    FileName = ExpandFileName(NormalDir(Path) + "*.*");
    DosCode  = FindFirst(FileName , faAnyFile , sr);
    while (DosCode == 0) {
        //if ((sr.Name == ".") || (sr.Name == "..")) {
        if ((sr.Name[1] != '.') && (sr.Attr != faVolumeID)) {
            if ((sr.Attr & faDirectory) == faDirectory) {
                if ((sr.Attr & faReadOnly) ==  faReadOnly) FileSetAttr(NormalDir(Path) + sr.Name , faArchive);
                Result = ClearDir(NormalDir(Path) + sr.Name , Delete) & Result;
                }
            else if ((sr.Attr & faVolumeID) != faVolumeID) {
                if ((sr.Attr & faReadOnly) ==  faReadOnly) FileSetAttr(NormalDir(Path) + sr.Name , faArchive);
                Result =  DeleteFile(NormalDir(Path) + sr.Name) & Result;
                }
            }
        DosCode = FindNext(sr);
        }
    FindClose(sr);

    //
    if (Delete && Result && (DosCode == FileNotFound) && !((Path.Length() == 2) && (Path[2] == ':'))) RemoveDir(Path);

    //Return Code.
    return Result;
}

//---------------------------------------------------------------------------
bool __fastcall TUserFile::ClearDirDate(AnsiString Path , TDateTime ClsDate)
{
    //Local Var.
    int        Count     = 0;
    AnsiString SerchFile = ExpandFileName(NormalDir(Path) + "*.*");
    TSearchRec sr;

    //Clear.
    if (FindFirst(SerchFile, faAnyFile, sr) == 0){
        do {
            if ((sr.Name == ".") || (sr.Name == "..")) continue;
            if (FileDateToDateTime(sr.Time) <= ClsDate) {
                if((sr.Attr & faDirectory) == sr.Attr) ClearDir  (NormalDir(Path) + sr.Name , 1);
                else                                   FileDelete(NormalDir(Path) + sr.Name    );
                }
            Count++;
            } while(FindNext(sr) == 0);
         }
     FindClose(sr);

     //Return.
     return Count;
}

//---------------------------------------------------------------------------
int  __fastcall TUserFile::SearchDir(AnsiString Path)
{
    //Local Var.
    int        Count     = 0;
    TSearchRec sr           ;
    AnsiString SerchFile    ;

    //Init. mem.
    for (int i = 0 ; i < MAX_FIND ; i++) m_FindItem[i] ="";

    //Searching.
    SerchFile = ExpandFileName(NormalDir(Path)+"*.*");
    if (FindFirst(SerchFile , faAnyFile, sr) == 0){
        do {
            if ((sr.Attr & faDirectory) == sr.Attr) {
                if((sr.Name == ".") || (sr.Name == "..")) continue;
                m_FindItem[Count] = sr.Name.c_str();
                Count++;
                }
            } while(FindNext(sr) == 0);
        }
    FindClose(sr);

    //Return.
    return Count;
}
//---------------------------------------------------------------------------
int  __fastcall TUserFile::GridSearchDir(AnsiString Path, TStringGrid *Grid, int DispType, bool DispDate , int /*Mode*/)
{
    //Local Var.
    int        Count = 0 ;
    TSearchRec sr        ;
    AnsiString SerchFile ;
    AnsiString Item [500];
    int        IDate[500];                                          

    //Init. Grid Data.
    for (int i = 0 ;i < Grid -> RowCount ; i++) Grid -> Rows[i] -> Clear();
    Grid -> RowCount = 2;



    //Search.
    SerchFile = ExpandFileName(NormalDir(Path) + "*.*");
    if (FindFirst(SerchFile, faAnyFile, sr) == 0) {
        do {
            if ((sr.Attr & faDirectory) == sr.Attr) {
                if ((sr.Name == ".") || (sr.Name == "..")) continue;
                    Item [Count] = sr.Name;
                    IDate[Count] = SearchFileDate(NormalDir(Path) + sr.Name  + "\\*.*" , sr.Time);
                    //IDate[Count] = FileAge(NormalDir(Path) + sr.Name);

                    Count ++;
                    }                                                 
            } while(FindNext(sr) == 0);
         }
     FindClose(sr);

     //Check NoFind.
     if (Count == 0) return 0;

     //Sorting.
     AnsiString tempName;
     int        tempTime;
     if (DispType == 1) {//날짜 내림차순
         for (int i = 0 ; i < Count ; i++) {
             for (int j = i ; j < Count ; j++) {
                 if (IDate[i] < IDate[j]) {
                     tempName = Item [i]; Item [i] = Item [j]; Item[ j] = tempName;
                     tempTime = IDate[i]; IDate[i] = IDate[j]; IDate[j] = tempTime;
                     }
                 }
             }
         }

     //Redisplay.
     Grid -> RowCount = Count;
     if (Grid -> ColCount < 3) DispDate = false;
     for (int i = 0 ; i < Count ; i++) {
         Grid -> Cells[0][i] = i + 1;
         Grid -> Cells[1][i] = Item[i];
         if (DispDate) Grid -> Cells[2][i] = FileDateToDateTime(IDate[i]);
         }
     Grid->Row = 0;

     //Return.
     return Count;
}

//---------------------------------------------------------------------------
AnsiString __fastcall TUserFile::GetWindowsDir(void)
{
    //Local Var.
    char Buffer[256];

     //Return.
    return AnsiString(GetWindowsDirectory(Buffer, sizeof(Buffer)));
}
//---------------------------------------------------------------------------
AnsiString __fastcall TUserFile::GetSystemDir(void)
{
    //Local Var.
    char Buffer[256];

    //Return.
    return AnsiString(GetSystemDirectory(Buffer, sizeof(Buffer)));
}


/***************************************************************************/
/* File Processing Functions.                                              */
/***************************************************************************/
//---------------------------------------------------------------------------
bool __fastcall TUserFile::FileCopy     (AnsiString FileName, AnsiString DestName)
{
    return CopyFile(FileName.c_str(), DestName.c_str(), false);
}

//---------------------------------------------------------------------------
bool __fastcall TUserFile::FileRename     (AnsiString FileName, AnsiString DestName)
{
    return RenameFile(FileName, DestName);
}

//---------------------------------------------------------------------------
bool __fastcall TUserFile::FileDelete     (AnsiString FileName)
{
    return DeleteFile(FileName);
}

//---------------------------------------------------------------------------
bool __fastcall TUserFile::DeleteFiles(AnsiString FileMask)
{
    //Local Var.
    TSearchRec sr;

    //Delete.
    if (FindFirst(ExpandFileName(FileMask) , faAnyFile , sr) == 0) {
        do {
            if ((sr.Name[1] != '.') & ((sr.Attr & faVolumeID)  != faVolumeID) & ((sr.Attr & faDirectory) != faDirectory))
                DeleteFile(ExtractFilePath(FileMask) + sr.Name);
            } while(FindNext(sr) == 0);
        }
    FindClose(sr);

    //Return.
    return 1;
}

//---------------------------------------------------------------------------
bool __fastcall TUserFile::FileExist(AnsiString FileName)
{
    //Local Var.
    bool       Exist;
    TSearchRec sr   ;
    AnsiString SerchFile = ExpandFileName(FileName);

    //Check Exist.
    if (FindFirst(SerchFile, faAnyFile, sr) == 0) Exist = true ;
    else                                          Exist = false;
    FindClose(sr);

    //Return.
    return Exist;
}

//---------------------------------------------------------------------------
AnsiString __fastcall TUserFile::ExFileName(AnsiString Name)
{
    return ExtractFileName(Name);
}

//---------------------------------------------------------------------------
AnsiString __fastcall TUserFile::ExPathName(AnsiString Name)
{
    return ExtractFilePath(Name);
}

//---------------------------------------------------------------------------
int  __fastcall TUserFile::SearchFile(AnsiString Mask)
{
    //Local Var.
    int        Count = 0 ;
    TSearchRec sr        ;
    AnsiString SearchFile;

    //Init. mem.
    for (int i = 0 ; i < MAX_FIND ; i++) m_FindItem[i] ="";

    //Search File.
    SearchFile = ExpandFileName(Mask);
    if (FindFirst(SearchFile, faAnyFile, sr) == 0){
        do {
            if ((sr.Attr & faDirectory) != sr.Attr) {
                m_FindItem[Count] = sr.Name.c_str();
                Count++;
                }
            } while(FindNext(sr) == 0);
        }
    FindClose(sr);

    //Return.
    return Count;
}

//---------------------------------------------------------------------------
int  __fastcall TUserFile::GridSearchFile(AnsiString Mask, TStringGrid *Grid, int DispType, bool DispDate)
{
    //Local Var.
    int        Count = 0 ;
    TSearchRec sr        ;
    AnsiString SerchFile ;
    AnsiString Item [500];
    int        IDate[500];

    //Init. Grid Data.
    for (int i = 0 ;i < Grid -> RowCount ; i++) Grid -> Rows[i] -> Clear();
    Grid -> RowCount = 2;

    //Search.
    SerchFile = ExpandFileName(Mask) + "\\*.*";
    if (FindFirst(SerchFile , faAnyFile , sr) == 0) {
        do {
            if ((sr.Attr & faDirectory) != sr.Attr) {
                Item [Count] = sr.Name;
                IDate[Count] = sr.Time;
                Count++;
                }
            } while (FindNext(sr) == 0);
         }
     FindClose(sr);

     //Check NoFind.
     if (Count == 0) return 0;

     //Sorting.
     AnsiString tempName;
     int        tempTime;
     if (DispType == 1) {//날짜 내림차순
         for (int i = 0 ; i < Count ; i++) {
             for (int j = i ; j < Count ; j++) {
                 if (IDate[i]<IDate[j]) {
                     tempName = Item [i]; Item [i] = Item [j]; Item[ j] = tempName;
                     tempTime = IDate[i]; IDate[i] = IDate[j]; IDate[j] = tempTime;
                     }
                 }
             }
         }

     //Redisplay.
     Grid -> RowCount = Count;
     if (Grid -> ColCount < 3) DispDate = false;
     for (int i=0; i<Count; i++) {
         Grid -> Cells[0][i] = i + 1;
         Grid -> Cells[1][i] = Item[i];
         if (DispDate) Grid -> Cells[2][i] = FileDateToDateTime(IDate[i]);
         }
     Grid -> Row = 0;

     //Return.
     return Count;
}

//---------------------------------------------------------------------------
int  __fastcall TUserFile::SearchFileDate    (AnsiString Mask, int MaxDate)
{
    //Local Var.
    int        Count = 0;
    TSearchRec sr;
    AnsiString SerchFile = ExpandFileName(Mask);

    //Search.
    if (FindFirst(SerchFile, faAnyFile, sr) == 0){
        do {
            if ((sr.Attr & faDirectory) != sr.Attr) {
                if (sr.Time > MaxDate) MaxDate = sr.Time;
                Count++;
                }
            } while(FindNext(sr) == 0);
        }
    FindClose(sr);

    //Return.
    return MaxDate;
}

//---------------------------------------------------------------------------
/*
//OperTypes   : FO_COPY, FO_DELETE, FO_MOVE, FO_RENAME
//OperOption  : FOF_ALLOWUNDO, FOF_CONFIRMMOUSE, FOF_FILESONLY, FOF_MULTIDESTFILES,
//             FOF_NOCONFIRMATION, FOF_NOCONFIRMMKDIR, FOF_RENAMEONCOLLISION,
//             FOF_SILENT, FOF_SIMPLEPROGRESS, FOF_NOERRORUI);
bool CFileUnit::FileOperator(AnsiString FileName, AnsiString DestName,
                             unsigned int OperTypes, short OperOption)
{
    SHFILEOPSTRUCT ShFileOp;
    ShFileOp.hwnd   = 0;
    ShFileOp.wFunc  = OperTypes;
    ShFileOp.pFrom  = ExpandFileName(FileName).c_str();
    ShFileOp.pTo    = ExpandFileName(DestName).c_str();
    ShFileOp.fFlags = FOF_SILENT;
    return SHFileOperation(&ShFileOp);

}
*/

