/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Usage  :                                                                */
/* Date   : 2004.05.26                                                     */
/* Modify : 2004.05.26                                                     */
/* Programmed by Primer(YongSik, Lee).                                     */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

//---------------------------------------------------------------------------
#ifndef UserFileH
#define UserFileH

//---------------------------------------------------------------------------
#include <Controls.hpp>
#include <StdCtrls.hpp>
//
#include <Grids.hpp>

//---------------------------------------------------------------------------
#define MAX_FIND 100

//---------------------------------------------------------------------------
AnsiString __fastcall NormalDir(AnsiString Name);
//AnsiString __fastcall GetExeDir(AnsiString FileName);

//---------------------------------------------------------------------------
class TUserFile {
    private:

    protected:

    public:
         AnsiString m_FindItem[100];

    public:
         //File Attributes.
         int       __fastcall GetFileSize    (AnsiString FileName           );
         TDateTime __fastcall GetFileDateTime(AnsiString FileName           );
         bool      __fastcall HasAttr        (AnsiString FileName , int Attr);

         //Directory Processing Functions.
         bool       __fastcall DirExists      (AnsiString Name);
         bool       __fastcall BrowseDirectory(AnsiString &AFolderName);
         bool       __fastcall CreateDir      (AnsiString Dir );
         bool       __fastcall CopyDir        (AnsiString FileName , AnsiString DestName);
         bool       __fastcall RenameDir      (AnsiString FileName , AnsiString DestName);
         bool       __fastcall MoveDir        (AnsiString FileName , AnsiString DestName);
         bool       __fastcall ClearDir       (AnsiString Path , bool Delete);
         bool       __fastcall ClearDirDate   (AnsiString Path , TDateTime ClsDate);
         int        __fastcall SearchDir      (AnsiString Path);
         int        __fastcall GridSearchDir  (AnsiString Path , TStringGrid *Grid , int DispType , bool DispDate = false , int Mode = 0);
         AnsiString __fastcall GetWindowsDir  (void);
         AnsiString __fastcall GetSystemDir   (void);

         //File Processing Functions.
         bool       __fastcall FileCopy      (AnsiString FileName , AnsiString DestName);
         bool       __fastcall FileRename    (AnsiString FileName , AnsiString DestName);
         bool       __fastcall FileDelete    (AnsiString FileName);
         bool       __fastcall DeleteFiles   (AnsiString FileMask);
         bool       __fastcall FileExist     (AnsiString FileName);
         AnsiString __fastcall ExFileName    (AnsiString Name    );
         AnsiString __fastcall ExPathName    (AnsiString Path    );
         int        __fastcall SearchFile    (AnsiString Mask    );
         int        __fastcall GridSearchFile(AnsiString Mask , TStringGrid *Grid , int DispType , bool DispDate = false);
    	 int        __fastcall SearchFileDate(AnsiString Mask , int MaxDate);

         //CSV Function
         bool       __fastcall SaveGridToCsv (TStringGrid *Grid , AnsiString DestName);
};

//---------------------------------------------------------------------------

extern TUserFile UserFile ;
#endif
