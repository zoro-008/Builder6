//---------------------------------------------------------------------------
#pragma hdrstop
#include "MapUnit.h"
#include "UserINI.h"
#include "UserFile.h"
#include "UtilDefine.h"
//---------------------------------------------------------------------------
#include "OptionMan.h"
#include "DataMan.h"
#include "stdio.h"
#pragma package(smart_init)

//AnsiString g_sMapList[MAX_MAP_CNT] ;
CMapUnit MAPU;
__fastcall CMapUnit::CMapUnit(void)
{
}

__fastcall CMapUnit::~CMapUnit(void)
{
//    delete m_pMemo ;
}
void CMapUnit::Init()
{
    if(!DirectoryExists(OM.CmnOptn.sSourcePath)) UserFile.CreateDir(OM.CmnOptn.sSourcePath);
    if(!DirectoryExists(OM.CmnOptn.sWorkedPath)) UserFile.CreateDir(OM.CmnOptn.sWorkedPath);
    sSource = OM.CmnOptn.sSourcePath ;
    sWorked = OM.CmnOptn.sWorkedPath ;
}

void CMapUnit::SetMemo (TMemo * _mmRowMap)
{
    m_mmRowMap = _mmRowMap ;
}

bool CMapUnit::ConvertCmn ( EN_ARAY_ID _riDst )
{
    //Local Var.
    int        Count = 0 ;
    TSearchRec sr        ;
    AnsiString SerchFile ;
    AnsiString Item [500];
    int        IDate[500];
    int iMapType  = OM.DevInfo.iMapType;

    int DispType = 0 ;

    //Init. Grid Data.
    //Search.
    //"모든파일|*.*|sinf형식|*.sinf|wmap_a형식|*.wmap_a|xml형식|*.xml" ;
    //SerchFile = NULL;
    //if(SerchFile == NULL) { SerchFile = ExpandFileName(OM.CmnOptn.sSourcePath) + "\\*.sinf"  ;}
    //if(SerchFile == NULL) { SerchFile = ExpandFileName(OM.CmnOptn.sSourcePath) + "\\*.wmap_a";}
    //if(SerchFile == NULL) { SerchFile = ExpandFileName(OM.CmnOptn.sSourcePath) + "\\*.xml"   ;}
    //if(SerchFile == NULL) { SerchFile = ExpandFileName(OM.CmnOptn.sSourcePath) + "\\*.*"     ;}

    SerchFile = ExpandFileName(OM.CmnOptn.sSourcePath) + "\\*.*" ;


    //if (FindFirst(OM.CmnOptn.sSourcePath+"\\*.*" , "*.*" , sr) == 0) {
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

    //Return.
    if(Count <= 0 ) return false ;

    //Convert
    TUserINI UserINI;
    String sPath   = OM.CmnOptn.sSourcePath + "\\" + Item[0] ;
    if(!FileExists(sPath)) return false ;

    String sTemp , sName;
    sName = ChangeFileExt(Item[0],"") ;

    String sSorc  = sSource + "\\" + Item[0] ;
    String sDest  = sWorked + "\\" + Now().FormatString("yyyymmdd") + "\\" + Item[0] ;
    sPath         = sWorked + "\\" + Now().FormatString("yyyymmdd") + "\\" ;

    String sSourceName ;
    if(!GetSourceName(sSourceName)) return false ;

    //if(UserFile.FileCopy(sSorc , SDest))
    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath);
    UserFile.FileCopy(sSorc , sDest);
    UserFile.DeleteFiles(sSorc);

    //     if(iMapType == 3) { if(!ConvertCoordi8in(sDest , _riDst)) return false; }
    //else if(iMapType == 2) { if(!ConvertCoordi2  (sDest , _riDst)) return false; }
    //else if(iMapType == 1) { if(!ConvertCoordi1  (sDest , _riDst)) return false; }
         if(iMapType == 0) { if(!ConvertCmn (sDest , _riDst)) return false; }
   // else if(iMapType == 1) { if(!ConvertNxt (sDest , _riDst)) return false; }
    DM.ARAY[_riDst].SetID(Item[0]) ;

/*
//추가 삭제소스.
//


//    sName  = ChangeFileExt(Item[0],"") ;
    sSorc  = sSource + "\\" + sSourceName ;
    sDest  = sWorked + "\\" + Now().FormatString("yyyymmdd") + "\\" + sSourceName ;
    sPath  = sWorked + "\\" + Now().FormatString("yyyymmdd") + "\\" ;

    //if(UserFile.FileCopy(sSorc , SDest))
    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath);
    UserFile.FileCopy(sSorc , sDest);
    UserFile.DeleteFiles(sSorc);
*/
    UserFile.ClearDirDate(sWorked , Now() - 90);
}

bool CMapUnit::OpenRawMap  (String _sSrcFilePath)
{
    if(m_mmRowMap == NULL    ) return false ;
    if(!FileExists(_sSrcFilePath)) return false ;

    //m_mmRowMap -> Text = "";
    m_mmRowMap -> Clear();
    m_mmRowMap -> Lines -> LoadFromFile(_sSrcFilePath);
}

bool CMapUnit::Copyfile(String _sSrcFilePath , String _sDstFolderPath)
{
    AnsiString Dir;
    Dir = "Hanra";

    if(!FileExists     (_sSrcFilePath  )) return false ;
    if(!DirectoryExists(_sDstFolderPath)) {
        if (!ForceDirectories(_sDstFolderPath)) return false;
        else                                    return true ;
    }

    CopyFile(_sSrcFilePath.c_str(), _sDstFolderPath.c_str(), false);
}

bool CMapUnit::ConvertCmn(TStringGrid * _sgSource , EN_ARAY_ID _riDst)
{
    String sSrcFilePath;
    sSrcFilePath = OM.CmnOptn.sSourcePath + "\\" + _sgSource->Cells[1][_sgSource->Row];
    sMapBackUpName = OM.CmnOptn.sSourcePath + "\\" + _sgSource->Cells[1][_sgSource->Row];
    if(!ConvertCmn(sSrcFilePath , _riDst)) return false ;

    return true;

}

bool CMapUnit::ConvertCmn(String _sSrcFilePath , EN_ARAY_ID _riDst)
{
    //Open 시에... 뻑... 확인 JS..
    DM.ARAY[_riDst].SetStat(csNone);
    AnsiString _sDstFolderPath = OM.CmnOptn.sWorkedPath;

    if(_sDstFolderPath == NULL) return false;   

    if(!OpenRawMap(_sSrcFilePath                         )) return false ;
    if(!Copyfile  (_sSrcFilePath , OM.CmnOptn.sWorkedPath)) return false ;

    const int    iFlatAngle    = OM.DevInfo.iFlatAngle   ;
    const int    iColCnt       = OM.DevInfo.iWColCnt     ;
    const int    iRowCnt       = OM.DevInfo.iWRowCnt     ;
    const int    iStrSttCol    = OM.DevInfo.iStrSttCol   ;
    const int    iStrSttRow    = OM.DevInfo.iStrSttRow   ;
    const double dDiameter     = OM.DevInfo.dWf_Diameter  ;
    const double dDevicePitchX = OM.DevInfo.dWf_PitchX ;
    const double dDevicePitchY = OM.DevInfo.dWf_PitchY ;

    String sGoodStr1     = OM.DevInfo.sGoodStr1    ;
    String sEmptStr1     = OM.DevInfo.sEmptStr1    ;


    //스페이스바가 안보여서 ~으로 치완해서 입력하기 때문에 여기서 다시 치완한다.
    sGoodStr1 = StringReplace(sGoodStr1 , "~" , " " , TReplaceFlags()<<rfReplaceAll);
    sEmptStr1 = StringReplace(sEmptStr1 , "~" , " " , TReplaceFlags()<<rfReplaceAll);

    DM.ARAY[_riDst].SetMaxColRow(iColCnt , iRowCnt);

    if(iRowCnt != DM.ARAY[_riDst].GetMaxRow() ||
       iColCnt != DM.ARAY[_riDst].GetMaxCol() ) return false ;
    if(sGoodStr1  == ""                       ) return false ;


    //위쪽 오프셑 건너띄기.
    AnsiString sTemp , sTemp1 ;
    for(int r=0; r< iRowCnt; r++){
        sTemp = m_mmRowMap->Lines->Strings[r+iStrSttRow] ;

        //앞쪽 오프셑 제거.
        sTemp.Delete(1,iStrSttCol);

        for(int c=0; c< iColCnt ; c++) {
            sTemp1 = sTemp.SubString(c * sGoodStr1.Length() + 1, sGoodStr1.Length()) ;
                 if(sTemp1 == sEmptStr1 ) DM.ARAY[_riDst].SetStat(r,c,csNone );
            else if(sTemp1 == ""        ) DM.ARAY[_riDst].SetStat(r,c,csNone );
            else if(sTemp1 == sGoodStr1 ) DM.ARAY[_riDst].SetStat(r,c,csUnkwn);
            else                          DM.ARAY[_riDst].SetStat(r,c,csFail );
        }
    }

    //군데군데 빵꾸처럼 있는 csNone csFail로 바꿈.
    bool bPreExist , bPstExist ;
    for(int r=1; r< iRowCnt-1; r++){
        for(int c=1; c< iColCnt-1 ; c++) {
            if(DM.ARAY[_riDst].GetStat(r,c) == csNone) {
                //가로 확인.
                bPreExist = false ;
                bPstExist = false ;
                for(int n=1; n < c-1; n++){
                    if(DM.ARAY[_riDst].GetStat(r,n) != csNone) {bPreExist = true ; break ;}
                }
                for(int n=c+1; n < iColCnt - 1; n++){
                    if(DM.ARAY[_riDst].GetStat(r,n) != csNone) {bPstExist = true ; break ;}
                }
                if(bPreExist && bPstExist) {
                    DM.ARAY[_riDst].SetStat(r, c, csFail);
                    continue ;
                }

                //세로 확인.
                bPreExist = false ;
                bPstExist = false ;
                for(int n=1; n < r-1; n++){
                    if(DM.ARAY[_riDst].GetStat(n,c) != csNone) {bPreExist = true ; break ;}
                }
                for(int n=r+1; n < iRowCnt - 1; n++){
                    if(DM.ARAY[_riDst].GetStat(n,c) != csNone) {bPstExist = true ; break ;}
                }
                if(bPreExist && bPstExist) {
                    DM.ARAY[_riDst].SetStat(r, c, csFail);
                    continue ;
                }
            }
        }
    }


         if(iFlatAngle == 0) {                            }/*0  */
    else if(iFlatAngle == 1) {DM.ARAY[_riDst].TurnCw90 ();}/*90 */
    else if(iFlatAngle == 2) {DM.ARAY[_riDst].TurnCw180();}/*180*/
    else if(iFlatAngle == 3) {DM.ARAY[_riDst].TurnCw270();}/*270*/

    DM.ARAY[_riDst].SetID(_sSrcFilePath) ;


    return true ;
}

bool CMapUnit::ConvertNxt ( EN_ARAY_ID _riDst )
{
    //Local Var.
    int        Count = 0 ;
    TSearchRec sr        ;
    AnsiString SerchFile ;
    AnsiString Item [500];
    int        IDate[500];
    int iMapType  = OM.DevInfo.iMapType;

    int DispType = 0 ;

         if(iMapType == 1) { SerchFile = ExpandFileName(OM.CmnOptn.sSourcePath) + "\\*.dat"   ;}
    else                   { SerchFile = ExpandFileName(OM.CmnOptn.sSourcePath) + "\\*.*"     ;}

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

    //Return.
    if(Count <= 0 ) return false ;

    //Convert
    TUserINI UserINI;
    String sPath   = OM.CmnOptn.sSourcePath + "\\" + Item[0] ;
    if(!FileExists(sPath)) return false ;

    String sTemp , sName;
    sName = ChangeFileExt(Item[0],"") ;



    String sSorc  = sSource + "\\" + Item[0] ;
    String sDest  = sWorked + "\\" + Now().FormatString("yyyymmdd") + "\\" + Item[0] ;
    sPath         = sWorked + "\\" + Now().FormatString("yyyymmdd") + "\\" ;

    String sSourceName ;
    if(!GetSourceName(sSourceName)) return false ;

    //if(UserFile.FileCopy(sSorc , SDest))
    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath);
    UserFile.FileCopy(sSorc , sDest);
    UserFile.DeleteFiles(sSorc);

    //     if(iMapType == 0) { if(!ConvertCmn (sDest , _riDst)) return false; }
    if(iMapType == 1) { if(!ConvertNxt (sDest , _riDst)) return false; }



    UserFile.ClearDirDate(sWorked , Now() - 90);
}

bool CMapUnit::ConvertNxt(TStringGrid * _sgSource , EN_ARAY_ID _riDst)
{
    String _sSrcFilePath;
    _sSrcFilePath = OM.CmnOptn.sSourcePath + "\\" + _sgSource->Cells[1][_sgSource->Row];

    sMapBackUpName = OM.CmnOptn.sSourcePath + "\\" + _sgSource->Cells[1][_sgSource->Row];

    if(!ConvertNxt(_sSrcFilePath , _riDst)) return false ;

    return true;

}

bool CMapUnit::ConvertNxt(String _sSrcFilePath , EN_ARAY_ID _riDst)
{
    AnsiString _sDstFolderPath = OM.CmnOptn.sWorkedPath;

    if(_sDstFolderPath == NULL) return false;

    if(!OpenRawMap(_sSrcFilePath                         )) return false ;
    if(!Copyfile  (_sSrcFilePath , OM.CmnOptn.sWorkedPath)) return false ;


    const int iFLATANGLE    = OM.DevInfo.iFlatAngle ;
    const int iMAPTYPE      = OM.DevInfo.iMapType   ;
    const int iDevicePitchX = OM.DevInfo.dWf_PitchX ;
    const int iDevicePitchY = OM.DevInfo.dWf_PitchY ;
    const int iRowCnt = OM.DevInfo.iWRowCnt  ;
    const int iColCnt = OM.DevInfo.iWColCnt  ;

    DM.ARAY[_riDst].SetStat(csNone);

    int iRowOfs ;
    int iColOfs ;


    AnsiString sEmpty = OM.DevInfo.sEmptStr1 ;
    AnsiString sGood1 = OM.DevInfo.sGoodStr1 ;

    if(iRowCnt <= 0 || iColCnt <= 0 ) return false ;
    if(sGood1  == ""                ) return false ;

    sGood1 = StringReplace(sGood1 , "~" , " " , TReplaceFlags()<<rfReplaceAll);
    sEmpty = StringReplace(sEmpty , "~" , " " , TReplaceFlags()<<rfReplaceAll);

    String sPath ;//, sSub ;

    String sData[MAX_WAFER_ROW] ;

    //아스키 NEXTCHIP
    FILE *fp ;
    int iLast = 0 ;
    BYTE bTemp ;
    int iData[MAX_WAFER_ROW*2][MAX_WAFER_COL*2] ;
    int iFour[4] ;


    sPath = _sSrcFilePath;

    for(int i=0; i<MAX_WAFER_ROW; i++) sData[i] = "" ;
    memset(&iData , 0  , sizeof(int)*MAX_WAFER_ROW*2*MAX_WAFER_COL*2 );



//---------------------------------------------------------------------------
//아스키
//---------------------------------------------------------------------------

        if(iMAPTYPE == 1){

           //File Open
           fp = fopen(sPath.c_str() , "rb");
           if (fp == NULL) {
//               fp = fopen(sPath.c_str() , "wb");
               fclose(fp);
               return false;
           }
           fseek(fp , 0, SEEK_END);
           iLast = ftell(fp);
           fseek(fp , 0, SEEK_SET);

           memset(&iFour , 0 , sizeof(int)*4);
           for(int j=0; j< iLast; j++) {
               fread (&bTemp , sizeof(bTemp) , 1 , fp);
               iFour[j%4] = bTemp ;
               if(j%4==3) {
                   if(iFour[0] < MAX_WAFER_ROW && iFour[1] < MAX_WAFER_COL) {
                       iData[iFour[0]][iFour[1]] = iFour[3] ;
                   }
               }
           }
           for(int j=0; j< MAX_WAFER_ROW; j++) {
               for(int k=0; k< MAX_WAFER_COL; k++) {
                   if(iData[j][k] != 0) {
                       iRowOfs = j ; //개가라 등장 우하하하.
                       j= MAX_WAFER_ROW ; k= MAX_WAFER_COL ;
                       break ;
                   }
               }
           }

           for(int j=0; j< MAX_WAFER_COL; j++) {
               for(int k=0; k< MAX_WAFER_ROW; k++) {
                   if(iData[k][j] != 0) {
                       iColOfs = j ;
                       k= MAX_WAFER_ROW ; j= MAX_WAFER_COL ;
                       break ;
                   }
               }
           }
 
           int iColMapCnt = iColCnt + iColOfs ;  //MAX_WAFER_COL로 뿌리면 많아서 줄임. JS
           int iRowMapCnt = iRowCnt + iRowOfs ;  //MAX_WAFER_ROW로 뿌리면 많아서 줄임.  JS
           for(int j=iRowOfs; j< iRowMapCnt; j++){
               for(int k=iColOfs; k< iColMapCnt; k++){
                        if(iData[j][k] == 0      ) DM.ARAY[_riDst].SetStat(j-iRowOfs,k-iColOfs,csNone );
                   else if(iData[j][k] == sGood1 ) DM.ARAY[_riDst].SetStat(j-iRowOfs,k-iColOfs,csUnkwn);
                   else                            DM.ARAY[_riDst].SetStat(j-iRowOfs,k-iColOfs,csFail );
               }
           }
           fclose(fp);
        }
        DM.ARAY[_riDst].SetMaxColRow(iColCnt, iRowCnt);

         if(iFLATANGLE == 0) {                            }
    else if(iFLATANGLE == 1) {DM.ARAY[_riDst].TurnCw90 ();}
    else if(iFLATANGLE == 2) {DM.ARAY[_riDst].TurnCw180();}
    else if(iFLATANGLE == 3) {DM.ARAY[_riDst].TurnCw270();}

    DM.ARAY[_riDst].SetID(_sSrcFilePath) ;

    return true ;
}

bool CMapUnit::MergeArray(EN_ARAY_ID _riMsk , EN_ARAY_ID _riMap , EN_ARAY_ID _riDst)
{
    int r1,c1 , r2,c2 ;
    int iOfsR , iOfsC ;

    DM.ARAY[_riDst] = DM.ARAY[_riMap] ;

    for(int c = 0 ; c < DM.ARAY[_riMap].GetMaxCol() ; c++) {
        for(int r = 0 ; r < DM.ARAY[_riMap].GetMaxRow() ; r++) {
                 if(DM.ARAY[_riMap].GetStat(r,c) == csUnkwn) DM.ARAY[_riDst].SetStat(r, c, csUnkwn) ;
            else if(DM.ARAY[_riMap].GetStat(r,c) == csFail ) DM.ARAY[_riDst].SetStat(r, c, csFail ) ;
        }
    }

    return true ;
}

bool CMapUnit::ChangeMaskNone (String _sSrcFilePath , int _iMapType , EN_ARAY_ID _riDst)
{

         if(_iMapType == 0) {if(!ConvertCmn (_sSrcFilePath , _riDst)) return false;}
    else if(_iMapType == 1) {if(!ConvertNxt (_sSrcFilePath , _riDst)) return false;}

    int iMaxRow = DM.ARAY[_riDst].GetMaxRow();
    int iMaxCol = DM.ARAY[_riDst].GetMaxCol();
    for(int i=0 ; i <= iMaxRow ; i++){
        for(int j =0 ; j <= iMaxCol ; j++) {
           if(DM.ARAY[_riDst].GetStat(i,j) == csNone) DM.ARAY[_riDst].SetStat(i,j,csNone);
           else                                       DM.ARAY[_riDst].SetStat(i,j,csEror);
        }
    }

    return true;
}


bool CMapUnit::CheckConvert()
{
    //Local Var.
    int        Count = 0 ;
    TSearchRec sr        ;
    AnsiString SerchFile ;
    AnsiString Item [500];
    int        IDate[500];

    AnsiString Mask = OM.CmnOptn.sSourcePath ;
    int DispType = 0 ;

    //Init. Grid Data.
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

    //Return.
    if(Count <= 0 ) return false ;

    //Convert
    TUserINI UserINI;
    String sPath = sSource + "\\"+ Item[0] ;
//    String sPath = sWorked + "\\"+ Item[0] ;
    if(!FileExists(sPath)) return false  ;

    return true ;
}



bool CMapUnit::GetSourceName(String &_sName )
{
    //Local Var.
    int        Count = 0 ;
    TSearchRec sr        ;
    AnsiString SerchFile ;
    AnsiString Item [500];
    int        IDate[500];

    AnsiString Mask = sSource ;
    int DispType = 0 ;

    //Init. Grid Data.
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

    //Return.
    if(Count <= 0 ) return false ;

    //Convert
    TUserINI UserINI;
    String sPath = sSource + "\\" + Item[0] ;
    if(!FileExists(sPath)) return false ;

    _sName = Item[0];
    return true ;

}

void CMapUnit::SaveHanraMap(EN_ARAY_ID _iAray , AnsiString _sFileName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  , sArayName ;
    AnsiString sLotNo , sMgzNo , sSlotNo;
    AnsiString sTemp  ;
    AnsiString sTemp2 ;
    AnsiString sRslt  ;
    TDateTime  CurrDateTime;
    int        iMgzNo , iSlotNo ;


    if( DM.ARAY[_iAray].GetLotNo() == "" ) sLotNo = "NoLot";
    else                                   sLotNo = DM.ARAY[_iAray].GetLotNo();

    iSlotNo = DM.ARAY[_iAray].GetID().ToIntDef(999999)%1000 ;
    iMgzNo  = DM.ARAY[_iAray].GetID().ToIntDef(999999)/1000 ;

    sPath = _sFileName; //sPath + "\\" + sSlotNo + ".ini" ;

    for(int r = 0 ; r < OM.DevInfo.iWRowCnt ; r++) {
        sRslt = "" ;
        for(int c = 0 ; c < OM.DevInfo.iWColCnt ; c++) {
            sTemp2 = (int)DM.ARAY[_iAray].GetStat(r,c) ;
            sTemp2 = sTemp2.sprintf("%02d",(int)DM.ARAY[_iAray].GetStat(r,c)) + "_";
            sRslt += sTemp2 ;
        }
        sTemp.printf("R%02d", r);
        UserINI.Save(sPath.c_str()  , "Data"  , sTemp , sRslt );
    }

    UserINI.Save(sPath.c_str()  , "ETC"  , "ID"    , DM.ARAY[_iAray].GetID()         );
    UserINI.Save(sPath.c_str()  , "ETC"  , "LotNo" , DM.ARAY[_iAray].GetLotNo()      );
    UserINI.Save(sPath.c_str()  , "ETC"  , "Row"   , OM.DevInfo.iWRowCnt             );
    UserINI.Save(sPath.c_str()  , "ETC"  , "Col"   , OM.DevInfo.iWColCnt             );

}

void CMapUnit::LoadHanraMap(EN_ARAY_ID _iAray , AnsiString _sFileName)
{

    //Local Var.
    TUserINI   UserINI;
    AnsiString sTemp  ;
    AnsiString sVal   ;
    AnsiString sRslt  ;

    AnsiString sId    ;
    AnsiString sLotNo ;

    int iRow , iCol   ;

    AnsiString sTemp2= "" ;

    String sPath = _sFileName;
    if(!FileExists(sPath)) return ;

    UserINI.Load(sPath.c_str()  , "ETC"  , "ID"    , &sId    );
    UserINI.Load(sPath.c_str()  , "ETC"  , "LotNo" , &sLotNo );
    UserINI.Load(sPath.c_str()  , "ETC"  , "Row"   , &iRow   );
    UserINI.Load(sPath.c_str()  , "ETC"  , "Col"   , &iCol   );

    DM.ARAY[_iAray].SetMaxColRow(iCol,iRow );

    for(int r = 0 ; r < iRow ; r++) {
        sTemp.printf("R%02d", r);
        UserINI.Load(sPath.c_str()  , "Data"  , sTemp , sRslt );
        for(int c = 0 ; c < iCol ; c++) {
            //sVal = "0x"+sRslt.SubString(c+1,1) ;
            sVal = sRslt.SubString(1,2) ;
            sRslt.Delete(1,3) ;
            DM.ARAY[_iAray].SetStat(r,c,(EN_CHIP_STAT)StrToInt(sVal)  );
            sTemp2 = sVal+"," ;
        }
    }

    DM.ARAY[_iAray].SetID   (sId   );
    DM.ARAY[_iAray].SetLotNo(sLotNo);

    return ;

}

