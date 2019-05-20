//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "VisnComUnit.h"
#include "UserFile.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"

#include "UserIni.h"
#include "LotUnit.h"
#include "SLogUnit.h"
#include "Timer.h"
#include "LotUnit.h"

#pragma package(smart_init)

CVisnComUnit VC;


typedef void (__closure *Func )(int _iVisn);
template <typename T>
class CRunThread
{
    class TRunThread : public TThread
    {
        private:
            T Func;
            int m_iPara ;

        public:
            __fastcall TRunThread(T func ,int _iPara) : TThread(false)
            {
                m_iPara = _iPara ;
                FreeOnTerminate = true;
                Func = func;
            }
            void __fastcall Execute()
            {
                Func(m_iPara);
            }
    };

    public:
        CRunThread(T func , int _iPara = 0)
        {
            TRunThread*  thread = new TRunThread(func , _iPara);
        }
};

/*  ** 사용법**
typedef void (__closure *Func)(int);
void __fastcall TForm1::Button3Click(TObject *Sender)
{
    Button3->Enabled = false ;
    CRunThread<Func> * func = new CRunThread<Func>(&TestFunc,0) ;
}

void TForm1::TestFunc(int _iPara)
{ 
    // 엄청 시간이 걸리는 루틴.
    int  k; 
    for(int c = 0; c < _iPara ; c++)
    {
        k = c; 
        k++;
        c = k; 
        c--; 
    } 
    Caption = k;
    Button3->Enabled = true;
}

*/

//#define VISION_FOLDER "D:\\VisionLog\\"
#define VISION_FOLDER "D:\\LotData\\"

#define LOT_LOG_FOLDER "D:\\LotLog\\"

CVisnComUnit::CVisnComUnit(void)
{
    //if (!DirectoryExists(OM.MstOptn.sPreEquipPath)) ForceDirectories(OM.MstOptn.sPreEquipPath) ;
    if (OM.MstOptn.sVisnPath == "") OM.MstOptn.sVisnPath = "C:\\Data" ;
    if (!DirectoryExists(OM.MstOptn.sVisnPath    )) ForceDirectories(OM.MstOptn.sVisnPath    ) ;


}

CVisnComUnit::~CVisnComUnit(void)
{
//    Trace("","");
}

//bool __fastcall CVisnComUnit::NetDrvConnect(AnsiString sDrv , AnsiString sIp, AnsiString sPath)
//{
//    AnsiString sTemp, slpLocalName, slpRemoteName ;
//    NETRESOURCE NetRes;
//    NetRes.dwScope = RESOURCE_GLOBALNET;
//    NetRes.dwType  = RESOURCETYPE_ANY;
//    NetRes.dwDisplayType = RESOURCEDISPLAYTYPE_GENERIC;
//    NetRes.dwUsage = RESOURCEUSAGE_CONNECTABLE;
//    slpLocalName = sDrv + ":"; // "Z:";
//    NetRes.lpLocalName  = slpLocalName.c_str();
//    slpRemoteName = "\\\\" + sIp + "\\" + sPath; //" \\\\192.168.0.76\\com";
//    NetRes.lpRemoteName = slpRemoteName.c_str();
//    NetRes.lpProvider    = NULL;
//
//    DWORD Result;
//    Result = WNetAddConnection2(&NetRes, "mds", "mds", CONNECT_INTERACTIVE);
//    if(Result!=NO_ERROR){
//        Result = WNetCancelConnection2(slpLocalName.c_str(),CONNECT_UPDATE_PROFILE,FALSE);       //Sever Network Drive
//        if(Result != NO_ERROR) return false ; //FM_MsgOk("ERROR","Net Connect Failed! Check the Connection.");
//        Result = WNetAddConnection2(&NetRes, "mds", "mds", CONNECT_INTERACTIVE); //Connect Network Drive
//        if(Result != NO_ERROR) return false ; //FM_MsgOk("ERROR","Net Connect Failed! Check the Connection.");
//    }
//
//    return true ;
//}
 
EN_CHIP_STAT CVisnComUnit::GetHighLevel (EN_CHIP_STAT _iOri , EN_CHIP_STAT _iNew)
{
    int iOriLevel ;
    int iNewLevel ;

    switch(_iOri) {
        default       : iOriLevel = 9999                   ; break ;
        case csRslt0  : iOriLevel = OM.CmnOptn.iRsltLevel0 ; break ;
        case csRslt1  : iOriLevel = OM.CmnOptn.iRsltLevel1 ; break ;
        case csRslt2  : iOriLevel = OM.CmnOptn.iRsltLevel2 ; break ;
        case csRslt3  : iOriLevel = OM.CmnOptn.iRsltLevel3 ; break ;
        case csRslt4  : iOriLevel = OM.CmnOptn.iRsltLevel4 ; break ;
        case csRslt5  : iOriLevel = OM.CmnOptn.iRsltLevel5 ; break ;
        case csRslt6  : iOriLevel = OM.CmnOptn.iRsltLevel6 ; break ;
        case csRslt7  : iOriLevel = OM.CmnOptn.iRsltLevel7 ; break ;
        case csRslt8  : iOriLevel = OM.CmnOptn.iRsltLevel8 ; break ;
        case csRslt9  : iOriLevel = OM.CmnOptn.iRsltLevel9 ; break ;
        case csRsltA  : iOriLevel = OM.CmnOptn.iRsltLevelA ; break ;
        case csRsltB  : iOriLevel = OM.CmnOptn.iRsltLevelB ; break ;
        case csRsltC  : iOriLevel = OM.CmnOptn.iRsltLevelC ; break ;
        case csRsltD  : iOriLevel = OM.CmnOptn.iRsltLevelD ; break ;
        case csRsltE  : iOriLevel = OM.CmnOptn.iRsltLevelE ; break ;
        case csRsltF  : iOriLevel = OM.CmnOptn.iRsltLevelF ; break ;
        case csRsltG  : iOriLevel = OM.CmnOptn.iRsltLevelG ; break ;
        case csRsltH  : iOriLevel = OM.CmnOptn.iRsltLevelH ; break ;
        case csRsltI  : iOriLevel = OM.CmnOptn.iRsltLevelI ; break ;
        case csRsltJ  : iOriLevel = OM.CmnOptn.iRsltLevelJ ; break ;
        case csRsltK  : iOriLevel = OM.CmnOptn.iRsltLevelK ; break ;
        case csRsltL  : iOriLevel = OM.CmnOptn.iRsltLevelL ; break ;
        case csUnkwn  : iOriLevel = 9999                   ; break ;
    }

    switch(_iNew) {
        default       : iNewLevel = 9999                   ; break ;
        case csRslt0  : iNewLevel = OM.CmnOptn.iRsltLevel0 ; break ;
        case csRslt1  : iNewLevel = OM.CmnOptn.iRsltLevel1 ; break ;
        case csRslt2  : iNewLevel = OM.CmnOptn.iRsltLevel2 ; break ;
        case csRslt3  : iNewLevel = OM.CmnOptn.iRsltLevel3 ; break ;
        case csRslt4  : iNewLevel = OM.CmnOptn.iRsltLevel4 ; break ;
        case csRslt5  : iNewLevel = OM.CmnOptn.iRsltLevel5 ; break ;
        case csRslt6  : iNewLevel = OM.CmnOptn.iRsltLevel6 ; break ;
        case csRslt7  : iNewLevel = OM.CmnOptn.iRsltLevel7 ; break ;
        case csRslt8  : iNewLevel = OM.CmnOptn.iRsltLevel8 ; break ;
        case csRslt9  : iNewLevel = OM.CmnOptn.iRsltLevel9 ; break ;
        case csRsltA  : iNewLevel = OM.CmnOptn.iRsltLevelA ; break ;
        case csRsltB  : iNewLevel = OM.CmnOptn.iRsltLevelB ; break ;
        case csRsltC  : iNewLevel = OM.CmnOptn.iRsltLevelC ; break ;
        case csRsltD  : iNewLevel = OM.CmnOptn.iRsltLevelD ; break ;
        case csRsltE  : iNewLevel = OM.CmnOptn.iRsltLevelE ; break ;
        case csRsltF  : iNewLevel = OM.CmnOptn.iRsltLevelF ; break ;
        case csRsltG  : iNewLevel = OM.CmnOptn.iRsltLevelG ; break ;
        case csRsltH  : iNewLevel = OM.CmnOptn.iRsltLevelH ; break ;
        case csRsltI  : iNewLevel = OM.CmnOptn.iRsltLevelI ; break ;
        case csRsltJ  : iNewLevel = OM.CmnOptn.iRsltLevelJ ; break ;
        case csRsltK  : iNewLevel = OM.CmnOptn.iRsltLevelK ; break ;
        case csRsltL  : iNewLevel = OM.CmnOptn.iRsltLevelL ; break ;
        case csUnkwn  : iNewLevel = 9999                   ; break ;
    }

    if( iOriLevel > iNewLevel) return _iNew ;
    else                       return _iOri ;

}

/*
bool __fastcall CVisnComUnit::ReadResult(EN_VISN iVisnNo, EN_ARAY_ID riId , int _iColOffset)
{
    //Local Var.
    int hFile       ;
    int iFileHandle ;
    int iFileLength ;
    int divpos      ;

    char *pszBuffer ;
    AnsiString AllField ;
    AnsiString sPath, Temp ,sName;

    int iFailCnt    ;
    int iFailRow    ;
    int iFailCol    ;
    int iFailItm    ;

    EN_CHIP_STAT iRslt ;

    sName = sName.sprintf("\\Vision%d.dat" ,iVisnNo+1 );  //나중에 확인.
    sPath = OM.MstOptn.sVisnPath + sName;

    if(!FileExists(sPath)) { Trace("Err",(sPath + " no file").c_str());  return false ;}

    iFileHandle = FileOpen(sPath.c_str(), fmOpenRead);
    iFileLength = FileSeek(iFileHandle,0,2);
    FileSeek(iFileHandle,0,0);
    pszBuffer = new char[iFileLength+1];
    memset(pszBuffer , 0 , sizeof(char)*(iFileLength+1));

    //메모리 동적 할당 나중에 해제하기...!
    FileRead (iFileHandle, pszBuffer, iFileLength);
    FileClose(iFileHandle);

    AllField = pszBuffer;

    Trace(sName.c_str() , AllField.c_str());

    iFailCnt = AllField.SubString(1,3).ToIntDef(-1) ;
    if(iFailCnt == -1) { Trace("Err","Fail Cnt is not a No"); return false ; }

    DM.ARAY[riId].ChangeStat(csUnkwn , csWork );

    WriteLogVisnErr(AllField);

    AllField.Delete(1,3);

    for (int i = 0; i < iFailCnt; i ++ ) {
        //Col.
        iFailCol = AllField.SubString(1,2).ToIntDef(-1);
        if(iFailCol <= 0                ) { Trace("Err","iFailCol < 0"      ); return false ; }
        if(iFailCol > OM.DevInfo.iColCnt) { Trace("Err","iFailCol > iColCnt"); return false ; }
        AllField.Delete(1,2);

        //Row.
        iFailRow = AllField.SubString(1,2).ToIntDef(-1);
        if(iFailRow <= 0                ) { Trace("Err","iFailRow < 0"      ); return false ; }
        if(iFailRow > OM.DevInfo.iRowCnt) { Trace("Err","iFailRow > iRowCnt"); return false ; }
        AllField.Delete(1,2);


        iFailItm = StrToInt("0x"+AllField.SubString(1,1));
        if(iFailItm <  0             ) { Trace("Err","iFailItm < 0"            ); return false ; }
        if(iFailItm >= MAX_CHIP_STAT ) { Trace("Err","iFailItm > MAX_CHIP_STAT"); return false ; }




        AllField.Delete(1,1);

        //chip Level 확인 해서 높은놈으로 세팅
        iRslt = GetHighLevel(DM.ARAY[riId].GetStat(iFailRow-1 ,_iColOffset+iFailCol-1),(EN_CHIP_STAT)iFailItm);
        DM.ARAY[riId].SetStat(iFailRow-1 ,_iColOffset+iFailCol-1 , iRslt) ;
    }

    WriteLogMap(riId);
    delete [] pszBuffer;

    if(!DeleteFile(sPath)) { Trace("Err",(sPath + "is Can't Deleted").c_str());  return false ; }

    return true ;
}

int __fastcall CVisnComUnit::SimpleReadResult(EN_VISN iVisnNo, CArray * Aray , int _iColOffset)
{
    //Local Var.
    int hFile       ;
    int iFileHandle ;
    int iFileLength ;
    int divpos      ;

    char *pszBuffer ;
    AnsiString AllField ;
    AnsiString sPath, Temp ,sName;

    int iFailCnt    ;
    int iFailRow    ;
    int iFailCol    ;
    int iFailItm    ;

    EN_CHIP_STAT iRslt ;

    sName = sName.sprintf("\\Vision%d.dat" ,iVisnNo+1 );
    sPath = OM.MstOptn.sVisnPath + sName;

    if(!FileExists(sPath)) { Trace("Err",(sPath + " no file").c_str());  return -1 ;}

    iFileHandle = FileOpen(sPath.c_str(), fmOpenRead);
    iFileLength = FileSeek(iFileHandle,0,2);
    FileSeek(iFileHandle,0,0);
    pszBuffer = new char[iFileLength+1];
    memset(pszBuffer , 0 , sizeof(char)*(iFileLength+1));

    //메모리 동적 할당 나중에 해제하기...!
    FileRead (iFileHandle, pszBuffer, iFileLength);
    FileClose(iFileHandle);

    AllField = pszBuffer;

    Trace(sName.c_str() , AllField.c_str());

    iFailCnt = AllField.SubString(1,3).ToIntDef(-1) ;
    if(iFailCnt == -1) { Trace("Err","Fail Cnt is not a No"); return -1 ; }

    Aray -> SetStat(csWork );

//    WriteLogVisnErr(AllField);

    AllField.Delete(1,3);

    for (int i = 0; i < iFailCnt; i ++ ) {
        //Col.
        iFailCol = AllField.SubString(1,2).ToIntDef(-1);
        if(iFailCol <= 0                ) { Trace("Err","iFailCol < 0"      ); return false ; }
        if(iFailCol > OM.DevInfo.iColCnt) { Trace("Err","iFailCol > iColCnt"); return false ; }
        AllField.Delete(1,2);

        //Row.
        iFailRow = AllField.SubString(1,2).ToIntDef(-1);
        if(iFailRow <= 0                ) { Trace("Err","iFailRow < 0"      ); return false ; }
        if(iFailRow > OM.DevInfo.iRowCnt) { Trace("Err","iFailRow > iRowCnt"); return false ; }
        AllField.Delete(1,2);


        iFailItm = StrToInt("0x"+AllField.SubString(1,1));
        if(iFailItm <  0             ) { Trace("Err","iFailItm < 0"            ); return false ; }
        if(iFailItm >= MAX_CHIP_STAT ) { Trace("Err","iFailItm > MAX_CHIP_STAT"); return false ; }

        AllField.Delete(1,1);

        if(Aray != NULL)Aray->SetStat(iFailRow-1 ,_iColOffset + iFailCol-1 , (EN_CHIP_STAT)iFailItm) ;
    }


    delete [] pszBuffer;

    return iFailCnt ;
}*/
bool __fastcall CVisnComUnit::ReadResult(EN_VISN iVisnNo, EN_ARAY_ID riId , int _iColOffset)
{
    //Local Var.
    int hFile       ;
    int iFileHandle ;
    int iFileLength ;
    int divpos      ;

    char *pszBuffer ;
    AnsiString AllField ;
    AnsiString sPath, Temp ,sName;

    int iFailCnt    ;
    int iFailRow    ;
    int iFailCol    ;
    EN_CHIP_STAT iFailItm    ;
    AnsiString sFailItm    ;

    EN_CHIP_STAT iRslt ;

    switch (iVisnNo) {
        default        : return false ;
        case vsVisn1_1 : sName = "\\Vision1_1.dat";  break ;  //수정.
        case vsVisn1_2 : sName = "\\Vision1_2.dat";  break ;
        case vsVisn2_1 : sName = "\\Vision2_1.dat";  break ;
        case vsVisn2_2 : sName = "\\Vision2_2.dat";  break ;
        case vsVisn3_1 : sName = "\\Vision3_1.dat";  break ;
        case vsVisn3_2 : sName = "\\Vision3_2.dat";  break ;
    }

    sPath = OM.MstOptn.sVisnPath + sName;

    if(!FileExists(sPath)) { Trace("Err",(sPath + " no file").c_str());  return false ;}

    iFileHandle = FileOpen(sPath.c_str(), fmOpenRead);
    iFileLength = FileSeek(iFileHandle,0,2);
    FileSeek(iFileHandle,0,0);
    pszBuffer = new char[iFileLength+1];
    memset(pszBuffer , 0 , sizeof(char)*(iFileLength+1));

    //메모리 동적 할당 나중에 해제하기...!
    FileRead (iFileHandle, pszBuffer, iFileLength);
    FileClose(iFileHandle);

    AllField = pszBuffer;

    Trace(sName.c_str() , AllField.c_str());


    iFailCnt = AllField.SubString(1,3).ToIntDef(-1) ;
    if(iFailCnt == -1) { Trace("Err","Fail Cnt is not a No"); return false ; }

    DM.ARAY[riId].ChangeStat(csUnkwn , csWork );

    WriteLogVisnErr(AllField);

    AllField.Delete(1,3);

    for (int i = 0; i < iFailCnt; i ++ ) {
        //Col.
        iFailCol = AllField.SubString(1,2).ToIntDef(-1);
        if(iFailCol <= 0                ) { Trace("Err","iFailCol < 0"      ); return false ; }
        if(iFailCol > OM.DevInfo.iColCnt) { Trace("Err","iFailCol > iColCnt"); return false ; }
        AllField.Delete(1,2);

        //Row.
        iFailRow = AllField.SubString(1,2).ToIntDef(-1);
        if(iFailRow <= 0                ) { Trace("Err","iFailRow < 0"      ); return false ; }
        if(iFailRow > OM.DevInfo.iRowCnt) { Trace("Err","iFailRow > iRowCnt"); return false ; }
        AllField.Delete(1,2);


        sFailItm = AllField.SubString(1,1);
             if(sFailItm == "0") iFailItm = csRslt0 ;
        else if(sFailItm == "1") iFailItm = csRslt1 ;
        else if(sFailItm == "2") iFailItm = csRslt2 ;
        else if(sFailItm == "3") iFailItm = csRslt3 ;
        else if(sFailItm == "4") iFailItm = csRslt4 ;
        else if(sFailItm == "5") iFailItm = csRslt5 ;
        else if(sFailItm == "6") iFailItm = csRslt6 ;
        else if(sFailItm == "7") iFailItm = csRslt7 ;
        else if(sFailItm == "8") iFailItm = csRslt8 ;
        else if(sFailItm == "9") iFailItm = csRslt9 ;
        else if(sFailItm == "A") iFailItm = csRsltA ;
        else if(sFailItm == "B") iFailItm = csRsltB ;
        else if(sFailItm == "C") iFailItm = csRsltC ;
        else if(sFailItm == "D") iFailItm = csRsltD ;
        else if(sFailItm == "E") iFailItm = csRsltE ;
        else if(sFailItm == "F") iFailItm = csRsltF ;
        else if(sFailItm == "G") iFailItm = csRsltG ;
        else if(sFailItm == "H") iFailItm = csRsltH ;
        else if(sFailItm == "I") iFailItm = csRsltI ;
        else if(sFailItm == "J") iFailItm = csRsltJ ;
        else if(sFailItm == "K") iFailItm = csRsltK ;
        else if(sFailItm == "L") iFailItm = csRsltL ;
        else                    {iFailItm = csRsltL ; Trace("Err","iFailItm 이상"); return false ; }
        AllField.Delete(1,1);

        //chip Level 확인 해서 높은놈으로 세팅
        //550인데...
        //iRslt = GetHighLevel(DM.ARAY[riId].GetStat(iFailRow-1 ,_iColOffset+iFailCol-1),(EN_CHIP_STAT)iFailItm);
        //DM.ARAY[riId].SetStat(iFailRow-1 ,_iColOffset+iFailCol-1 , iRslt) ;

        //580 , 1200
        iRslt = GetHighLevel(DM.ARAY[riId].GetStat(OM.DevInfo.iRowCnt - iFailRow ,_iColOffset+iFailCol-1),(EN_CHIP_STAT)iFailItm);
        DM.ARAY[riId].SetStat(OM.DevInfo.iRowCnt - iFailRow ,_iColOffset+iFailCol-1 , iRslt) ;

    }

    WriteLogMap(riId);
    delete [] pszBuffer;

    if(!DeleteFile(sPath)) { Trace("Err",(sPath + "is Can't Deleted").c_str());  return false ; }

    return true ;
}

int __fastcall CVisnComUnit::SimpleReadResult(EN_VISN iVisnNo, CArray * Aray , int _iColOffset)
{
    //Local Var.
    int hFile       ;
    int iFileHandle ;
    int iFileLength ;
    int divpos      ;

    char *pszBuffer ;
    AnsiString AllField ;
    AnsiString sPath, Temp ,sName;

    int iFailCnt    ;
    int iFailRow    ;
    int iFailCol    ;
    EN_CHIP_STAT iFailItm    ;
    AnsiString sFailItm    ;

    EN_CHIP_STAT iRslt ;

    switch (iVisnNo) {
        default        : return false ;
        case vsVisn1_1 : sName = "\\Vision1_1.dat";  break ;  //수정.
        case vsVisn1_2 : sName = "\\Vision1_2.dat";  break ;
        case vsVisn2_1 : sName = "\\Vision2_1.dat";  break ;
        case vsVisn2_2 : sName = "\\Vision2_2.dat";  break ;
        case vsVisn3_1 : sName = "\\Vision3_1.dat";  break ;
        case vsVisn3_2 : sName = "\\Vision3_2.dat";  break ;
    }
    //sName = sName.sprintf("\\Vision%d.dat" ,iVisnNo+1 );
    sPath = OM.MstOptn.sVisnPath + sName;

    if(!FileExists(sPath)) { Trace("Err",(sPath + " no file").c_str());  return -1 ;}

    iFileHandle = FileOpen(sPath.c_str(), fmOpenRead);
    iFileLength = FileSeek(iFileHandle,0,2);
    FileSeek(iFileHandle,0,0);
    pszBuffer = new char[iFileLength+1];
    memset(pszBuffer , 0 , sizeof(char)*(iFileLength+1));

    //메모리 동적 할당 나중에 해제하기...!
    FileRead (iFileHandle, pszBuffer, iFileLength);
    FileClose(iFileHandle);

    AllField = pszBuffer;

    Trace(sName.c_str() , AllField.c_str());

    iFailCnt = AllField.SubString(1,3).ToIntDef(-1) ;
    if(iFailCnt == -1) { Trace("Err","Fail Cnt is not a No"); return -1 ; }


    Aray -> ChangeStat(csUnkwn , csWork );

//    WriteLogVisnErr(AllField);

    AllField.Delete(1,3);

    for (int i = 0; i < iFailCnt; i ++ ) {
        //Col.
        iFailCol = AllField.SubString(1,2).ToIntDef(-1);
        if(iFailCol <= 0                ) { Trace("Err","iFailCol < 0"      ); return false ; }
        if(iFailCol > OM.DevInfo.iColCnt) { Trace("Err","iFailCol > iColCnt"); return false ; }
        AllField.Delete(1,2);

        //Row.
        iFailRow = AllField.SubString(1,2).ToIntDef(-1);
        if(iFailRow <= 0                ) { Trace("Err","iFailRow < 0"      ); return false ; }
        if(iFailRow > OM.DevInfo.iRowCnt) { Trace("Err","iFailRow > iRowCnt"); return false ; }
        AllField.Delete(1,2);


        sFailItm = AllField.SubString(1,1);
             if(sFailItm == "0") iFailItm = csRslt0 ;
        else if(sFailItm == "1") iFailItm = csRslt1 ;
        else if(sFailItm == "2") iFailItm = csRslt2 ;
        else if(sFailItm == "3") iFailItm = csRslt3 ;
        else if(sFailItm == "4") iFailItm = csRslt4 ;
        else if(sFailItm == "5") iFailItm = csRslt5 ;
        else if(sFailItm == "6") iFailItm = csRslt6 ;
        else if(sFailItm == "7") iFailItm = csRslt7 ;
        else if(sFailItm == "8") iFailItm = csRslt8 ;
        else if(sFailItm == "9") iFailItm = csRslt9 ;
        else if(sFailItm == "A") iFailItm = csRsltA ;
        else if(sFailItm == "B") iFailItm = csRsltB ;
        else if(sFailItm == "C") iFailItm = csRsltC ;
        else if(sFailItm == "D") iFailItm = csRsltD ;
        else if(sFailItm == "E") iFailItm = csRsltE ;
        else if(sFailItm == "F") iFailItm = csRsltF ;
        else if(sFailItm == "G") iFailItm = csRsltG ;
        else if(sFailItm == "H") iFailItm = csRsltH ;
        else if(sFailItm == "I") iFailItm = csRsltI ;
        else if(sFailItm == "J") iFailItm = csRsltJ ;
        else if(sFailItm == "K") iFailItm = csRsltK ;
        else if(sFailItm == "L") iFailItm = csRsltL ;
        else                    {iFailItm = csRsltL ; Trace("Err","iFailItm 이상"); return false ; }
        AllField.Delete(1,1);

        //550
        //if(Aray != NULL)Aray->SetStat(iFailRow-1 ,_iColOffset + iFailCol-1 , (EN_CHIP_STAT)iFailItm) ;

        if(Aray != NULL)Aray->SetStat(OM.DevInfo.iRowCnt - iFailRow ,_iColOffset + iFailCol-1 , (EN_CHIP_STAT)iFailItm) ;


    }


    delete [] pszBuffer;

    return iFailCnt ;
}

//int CVisnComUnit::GetLastErrCnt(int _iVisnNo , EN_CHIP_STAT _csStat)
//{
//    switch(_iVisnNo){
//        default : return -1;
//        case 1  : return m_iVs1ErrCnt[_csStat] ;
//        case 2  : return m_iVs2ErrCnt[_csStat] ;
//        case 3  : return m_iVs3ErrCnt[_csStat] ;
//        case 4  : return m_iVs4ErrCnt[_csStat] ;
//    }
////    return (_iVisnNo == 1 ? m_iVs1ErrCnt[_csStat] : m_iVs2ErrCnt[_csStat]);
//}

//int CVisnComUnit::GetLastTtlErrCnt(int _iVisnNo )
//{
//    switch(_iVisnNo){
//        default : return -1;
//        case 1  : return m_iVs1TtlErrCnt ;
//        case 2  : return m_iVs2TtlErrCnt ;
//        case 3  : return m_iVs3TtlErrCnt ;
//        case 4  : return m_iVs4TtlErrCnt ;
//    }
////    return (_iVisnNo == 1 ? m_iVs1TtlErrCnt : m_iVs2TtlErrCnt );
//}

//int CVisnComUnit::ClearErrCnt()
//{
//    memset(&m_iVs1ErrCnt , 0 ,sizeof(int) * MAX_CHIP_STAT);
//    memset(&m_iVs2ErrCnt , 0 ,sizeof(int) * MAX_CHIP_STAT);
//    memset(&m_iVs3ErrCnt , 0 ,sizeof(int) * MAX_CHIP_STAT);
//    memset(&m_iVs4ErrCnt , 0 ,sizeof(int) * MAX_CHIP_STAT);
//
//    m_iVs1TtlErrCnt = 0 ;
//    m_iVs2TtlErrCnt = 0 ;
//    m_iVs3TtlErrCnt = 0 ;
//    m_iVs4TtlErrCnt = 0 ;
//
//}

//EN_VISN_RESET_SEL iVisnResetSel = vrBoth ;

//typedef void (__closure *Func2)(int _iVisn);
bool CVisnComUnit::SendReset(EN_VISN _iSel)
{
    CRunThread<Func> * func  = new CRunThread<Func>(&ThreadReset , _iSel) ;

    return true ;

}
bool CVisnComUnit::SendLotStart(EN_VISN _iSel)
{
    CRunThread<Func> * func = new CRunThread<Func>(&ThreadLotStart , _iSel) ;

    return true ;
}

bool CVisnComUnit::SendModelChange(EN_VISN _iSel)
{
    CRunThread<Func> * func = new CRunThread<Func>(&ThreadModelChange , _iSel    ) ;
    return true ;
}


void CVisnComUnit::ThreadReset(int _iVisn)
{
    CDelayTimer Timer ;

    EN_OUTPUT_ID yVisnReset ;
    EN_INPUT_ID  xVisnReady ;
    EN_INPUT_ID  xVisnBusy  ;
    EN_INPUT_ID  xVisnEnd   ;

    if(_iVisn == vsVisn1_1) { yVisnReset = yVisn1_1Reset ; xVisnReady = xVisn1_1Ready ; xVisnBusy = xVisn1_1Busy ; xVisnEnd = xVisn1_1End ; }
    if(_iVisn == vsVisn1_2) { yVisnReset = yVisn1_2Reset ; xVisnReady = xVisn1_2Ready ; xVisnBusy = xVisn1_2Busy ; xVisnEnd = xVisn1_2End ; }
    if(_iVisn == vsVisn2_1) { yVisnReset = yVisn2_1Reset ; xVisnReady = xVisn2_1Ready ; xVisnBusy = xVisn2_1Busy ; xVisnEnd = xVisn2_1End ; }
    if(_iVisn == vsVisn2_2) { yVisnReset = yVisn2_2Reset ; xVisnReady = xVisn2_2Ready ; xVisnBusy = xVisn2_2Busy ; xVisnEnd = xVisn2_2End ; }
    if(_iVisn == vsVisn3_1) { yVisnReset = yVisn3_1Reset ; xVisnReady = xVisn3_1Ready ; xVisnBusy = xVisn3_1Busy ; xVisnEnd = xVisn3_1End ; }
    if(_iVisn == vsVisn3_2) { yVisnReset = yVisn3_2Reset ; xVisnReady = xVisn3_2Ready ; xVisnBusy = xVisn3_2Busy ; xVisnEnd = xVisn3_2End ; }

    int iStep    = 10 ;
    int iPreStep = 0  ;
    int iSendCnt = 0  ;


    while(1) {
        Application -> ProcessMessages() ;
        Sleep(1);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (m_tmReset.OnDelay(iStep == iPreStep && !OM.MstOptn.bDebugMode , 10000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiHED_VisnCommTO);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS Vision %d: iStep=%02d" , __FUNC__ , _iVisn , iStep );
            Trace(("VisnCom"+AnsiString(_iVisn)).c_str(),sTemp.c_str());
            IO_SetY(yVisnReset , false) ;
            iStep = 0 ;
            return ;
        }

        if(iStep != iPreStep) {
            sTemp = sTemp.sprintf("%s iStep=%02d" , __FUNC__ , iStep );
            Trace(("VisnCom"+AnsiString(_iVisn)).c_str(),sTemp.c_str());
        }

        iPreStep = iStep ;

        //Cycle.
        switch (iStep) {

            default : if(iStep) {
                          sTemp = sTemp.sprintf("Cycle Default Clear %s iStep=%02d" , __FUNC__ , iStep );
                          Trace(("VisnCom"+AnsiString(_iVisn)).c_str(), sTemp.c_str());
                      }
                      IO_SetY(yVisnReset , false) ;
                      iStep = 0 ;
                      return ;

            case  10: iSendCnt = 0 ;
                      IO_SetY(yVisnReset , false) ;
                      iStep++;
                      continue ;

            case  11: //iSendCnt ++ ;
                      //처음부터 레디가 꺼져 있으면 리셋을 날려서 레디를 먼저 킨다.
                      if(!IO_GetX(xVisnReady,true)) {
                          IO_SetY(yVisnReset , true) ;
                          iStep ++ ;
                          continue ;
                      }
                      iStep = 14;
                      continue ;

                      //여기부터.
            case  12: if(!IO_GetX(xVisnReady,true)) continue ;
                      IO_SetY(yVisnReset , false) ;
                      Timer.Clear() ;
                      iStep++;
                      continue ;

            case  13: if(!Timer.OnDelay(true , 500)) continue ;
                      iStep++;
                      continue ;

            case  14: iSendCnt ++ ;
                      IO_SetY(yVisnReset , true) ;
                      iStep++;
                      continue ;

            case  15: if(IO_GetX(xVisnReady,true)) continue ;
                      iStep++;
                      continue ;

            case  16: IO_SetY(yVisnReset ,false) ;
                      iStep++;
                      continue ;

            case  17: if(!IO_GetX(xVisnReady,true)) continue ;
                      Timer.Clear() ;
                      iStep++;
                      continue ;

            case  18: if(!Timer.OnDelay(true , 500)) continue ;
                      if(iSendCnt == 1 ) {
                          if(!IO_GetX(xVisnBusy,true)){  //비젼이 이상하게 2번 보내 줘야 검사위치가 클리어됌.
                              iStep = 14 ;
                              continue ;
                          }
                      }

                      if(IO_GetX(xVisnBusy,true)) {
                          if(iSendCnt > 2) {  //2번 해서도 비젼쪽 비지가 않풀리면 에러.
                              EM_SetDisp(true) ;
                              EM_SetErr(eiHED_VisnResetCnt) ;
                              return ;
                          }
                          iSendCnt++;
                          iStep = 14 ;
                          continue ;
                      }
                      Trace(("VisnCom"+AnsiString(_iVisn)).c_str(),"SendReset Complete!");

                      iStep = 0 ;
                      return ;
        }
    }
}

void CVisnComUnit::ThreadLotStart(int _iVisn)
{
    CDelayTimer Timer ;

    EN_OUTPUT_ID yVisnLotStart;
    EN_INPUT_ID  xVisnReady ;
    EN_INPUT_ID  xVisnBusy  ;
    EN_INPUT_ID  xVisnEnd   ;
    EN_ERR_ID    eiVISNNotReady ;

    if(_iVisn == vsVisn1_1) { yVisnLotStart = yVisn1_1LotStart ; xVisnReady = xVisn1_1Ready ; xVisnBusy = xVisn1_1Busy ; xVisnEnd = xVisn1_1End ; eiVISNNotReady = eiHED_Visn1_1NotReady ; }
    if(_iVisn == vsVisn1_2) { yVisnLotStart = yVisn1_2LotStart ; xVisnReady = xVisn1_2Ready ; xVisnBusy = xVisn1_2Busy ; xVisnEnd = xVisn1_2End ; eiVISNNotReady = eiHED_Visn1_2NotReady ; }
    if(_iVisn == vsVisn2_1) { yVisnLotStart = yVisn2_1LotStart ; xVisnReady = xVisn2_1Ready ; xVisnBusy = xVisn2_1Busy ; xVisnEnd = xVisn2_1End ; eiVISNNotReady = eiHED_Visn2_1NotReady ; }
    if(_iVisn == vsVisn2_2) { yVisnLotStart = yVisn2_2LotStart ; xVisnReady = xVisn2_2Ready ; xVisnBusy = xVisn2_2Busy ; xVisnEnd = xVisn2_2End ; eiVISNNotReady = eiHED_Visn2_2NotReady ; }
    if(_iVisn == vsVisn3_1) { yVisnLotStart = yVisn3_1LotStart ; xVisnReady = xVisn3_1Ready ; xVisnBusy = xVisn3_1Busy ; xVisnEnd = xVisn3_1End ; eiVISNNotReady = eiHED_Visn3_1NotReady ; }
    if(_iVisn == vsVisn3_2) { yVisnLotStart = yVisn3_2LotStart ; xVisnReady = xVisn3_2Ready ; xVisnBusy = xVisn3_2Busy ; xVisnEnd = xVisn3_2End ; eiVISNNotReady = eiHED_Visn3_2NotReady ; }

    int iStep    = 10 ;
    int iPreStep = 0  ;
    int iSendCnt = 0  ;

    while(1) {
        Application -> ProcessMessages() ;
        Sleep(1);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (m_tmLotStart.OnDelay(iStep == iPreStep && !OM.MstOptn.bDebugMode , 2000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiHED_VisnCommTO);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : iStep=%02d" , __FUNC__ , iStep );
            Trace(("VisnCom"+AnsiString(_iVisn)).c_str(),sTemp.c_str());
            iStep = 0 ;
            IO_SetY(yVisnLotStart , false) ;
            return ;
        }

        if(iStep != iPreStep) {
            sTemp = sTemp.sprintf("%s iStep=%02d" , __FUNC__ , iStep );
            Trace(("VisnCom"+AnsiString(_iVisn)).c_str(),sTemp.c_str());
        }

        iPreStep = iStep ;

        //Cycle.
        switch (iStep) {
            default : sTemp = sTemp.sprintf("Cycle Default Clear %s iStep=%02d" , __FUNC__ , iStep );
                      Trace(("VisnCom"+AnsiString(_iVisn)).c_str(), sTemp.c_str());
                      iStep = 0 ;
                      return ;

            case  10: if(!IO_GetX(xVisnReady,true)) {
                          EM_SetErr(eiVISNNotReady);
                          EM_SetDisp(true) ;
                          return ;
                      }

                      WriteVisnLot();
                      IO_SetY(yVisnLotStart , true) ;
                      iStep++;
                      continue ;

            case  11: if(IO_GetX(xVisnReady,true)) continue ;
                      iStep++;
                      continue ;

            case  12: IO_SetY(yVisnLotStart  ,false) ;
                      iStep++;
                      continue ;

            case  13: if(!IO_GetX(xVisnReady,true)) continue ;
                      Timer.Clear();
                      iStep++;
                      continue ;

            case  14: if(IO_GetX(xVisnBusy,true)) continue ;
                      Trace(("VisnCom"+AnsiString(_iVisn)).c_str(),"SendLotStart Complete!");
                      iStep = 0 ;
                      return ;
        }
    }
}

void CVisnComUnit::ThreadModelChange(int _iVisn)
{
    CDelayTimer Timer ;

    EN_OUTPUT_ID yVisnChange;
    EN_INPUT_ID  xVisnReady ;
    EN_INPUT_ID  xVisnBusy  ;
    EN_INPUT_ID  xVisnEnd   ;
    EN_ERR_ID    eiVISNNotReady ;

    if(_iVisn == vsVisn1_1) { yVisnChange = yVisn1_1JobChange ; xVisnReady = xVisn1_1Ready ; xVisnBusy = xVisn1_1Busy ; xVisnEnd = xVisn1_1End ; eiVISNNotReady = eiHED_Visn1_1NotReady ; }
    if(_iVisn == vsVisn1_2) { yVisnChange = yVisn1_2JobChange ; xVisnReady = xVisn1_2Ready ; xVisnBusy = xVisn1_2Busy ; xVisnEnd = xVisn1_2End ; eiVISNNotReady = eiHED_Visn1_2NotReady ; }
    if(_iVisn == vsVisn2_1) { yVisnChange = yVisn2_1JobChange ; xVisnReady = xVisn2_1Ready ; xVisnBusy = xVisn2_1Busy ; xVisnEnd = xVisn2_1End ; eiVISNNotReady = eiHED_Visn2_1NotReady ; }
    if(_iVisn == vsVisn2_2) { yVisnChange = yVisn2_2JobChange ; xVisnReady = xVisn2_2Ready ; xVisnBusy = xVisn2_2Busy ; xVisnEnd = xVisn2_2End ; eiVISNNotReady = eiHED_Visn2_2NotReady ; }
    if(_iVisn == vsVisn3_1) { yVisnChange = yVisn3_1JobChange ; xVisnReady = xVisn3_1Ready ; xVisnBusy = xVisn3_1Busy ; xVisnEnd = xVisn3_1End ; eiVISNNotReady = eiHED_Visn3_1NotReady ; }
    if(_iVisn == vsVisn3_2) { yVisnChange = yVisn3_2JobChange ; xVisnReady = xVisn3_2Ready ; xVisnBusy = xVisn3_2Busy ; xVisnEnd = xVisn3_2End ; eiVISNNotReady = eiHED_Visn3_2NotReady ; }

    int iStep    = 10 ;
    int iPreStep = 0  ;
    int iSendCnt = 0  ;
    while(1) {
        Application -> ProcessMessages() ;
        Sleep(1);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (m_tmModelChange.OnDelay(iStep == iPreStep && !OM.MstOptn.bDebugMode ,iStep == 14 ? 20000 : 5000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiHED_VisnCommTO);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : iStep=%02d" , __FUNC__ , iStep );
            Trace(("VisnCom"+AnsiString(_iVisn)).c_str(),sTemp.c_str());
            IO_SetY(yVisnChange , false) ;
            iStep = 0 ;
            return ;
        }

        if(iStep != iPreStep) {
            sTemp = sTemp.sprintf("%s iStep=%02d" , __FUNC__ , iStep );
            Trace(("VisnCom"+AnsiString(_iVisn)).c_str(),sTemp.c_str());
        }

        iPreStep = iStep ;

        //Cycle.
        switch (iStep) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s iStep=%02d" , __FUNC__ , iStep );
                      Trace(("VisnCom"+AnsiString(_iVisn)).c_str(), sTemp.c_str());
                      iStep = 0 ;
                      return  ;

            case  10: WriteVisnModel();
                      iStep++;
                      continue ;

            case  11: if(!IO_GetX(xVisnReady,true)) {
                          EM_SetErr(eiVISNNotReady);
                          EM_SetDisp(true) ;
                          return ;
                      }
                      IO_SetY(yVisnChange , true) ;
                      Timer.Clear();
                      iStep++;
                      continue ;

            case  12: if(IO_GetX(xVisnReady,true)) continue ;
                      iStep++;
                      continue ;

            case  13: IO_SetY(yVisnChange ,false) ;
                      iStep++;
                      continue ;

            case  14: if(!IO_GetX(xVisnReady,true)) continue ;
                      Timer.Clear();
                      iStep++;
                      continue ;

            //스텝바꿀때 타임아웃 주의
            case  15: if(IO_GetX(xVisnBusy,true)) continue ;
                      iStep = 0 ;
                      Trace(("VisnCom"+AnsiString(_iVisn)).c_str(),"SendJobChange Complete!");
                      return ;
        }
    }
}

bool __fastcall CVisnComUnit::WriteVisnLot()
{
    //Local Var.
    int        hFile  ;
    AnsiString sPath  ;
    AnsiString sData  ;
    int        sState ;

    //Set Path.
    sPath = OM.MstOptn.sVisnPath ;
    sData = LT.GetCrntLot().Trim()+";" ;
    //

    if (!DirectoryExists(sPath)) CreateDir(sPath);
    sPath = OM.MstOptn.sVisnPath + "\\" + "LotData.dat";
    if (FileExists(sPath)) DeleteFile(sPath) ;


    hFile = FileOpen(sPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sPath.c_str());
        if (hFile == -1) { Trace("Err",(sPath + "is Can't made").c_str());  return false ; }
    }

    //AnsiString sDate = Now().CurrentDateTime().FormatString("AM/PM h'시 'n'분' s'초 \r\n'");
    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);
    return true;
}

bool __fastcall CVisnComUnit::WriteVisnModel()
{
    //Local Var.
    int        hFile       ;
    AnsiString sPath       ;
    AnsiString sData       ;
    int        sState      ;

    //Set Path.

    sPath = OM.MstOptn.sVisnPath ;
    sData = OM.DevInfo.sVisnIndexId + ";" + OM.m_sCrntDev + ";";

    if (!DirectoryExists(sPath)) CreateDir(sPath);

    for(int i = 1 ; i< MAX_VISN+1 ; i++) {
        sPath = OM.MstOptn.sVisnPath + "\\" + "Change" + AnsiString(i) + ".dat";
        if (FileExists(sPath)) DeleteFile(sPath) ;
        hFile = FileCreate(sPath.c_str());
        if (hFile == -1) { Trace("Err",(sPath + "is Can't made").c_str());  return false ; }

        //AnsiString sDate = Now().CurrentDateTime().FormatString("AM/PM h'시 'n'분' s'초 \r\n'");
        FileSeek (hFile , 0             , SEEK_END      );
        FileWrite(hFile , sData.c_str() , sData.Length());

        //Close File.
        FileClose(hFile);
    }
    return true;
}


void __fastcall CVisnComUnit::DelPastLog()
{
    UserFile.ClearDirDate(VISION_FOLDER , Now() - 30);
}

bool __fastcall CVisnComUnit::WriteLogVisnErr(AnsiString _sLog) //비젼 결과값들을 로그로 관리 한다.
{
    //Local Var.
    int        hFile       ;
    AnsiString sPath       ;
    AnsiString sTemp,sTime ;
    AnsiString sData       ;
    int        sState      ;
    TDateTime  tLotStartTime;

    DelPastLog();

    //Set Path.
    if(LT.m_bLotOpen)  tLotStartTime.Val = LT.LotInfo.dStartTime;
    else               tLotStartTime     = Now()                ;
    sData = tLotStartTime.FormatString("yyyymmdd");
    sTime = tLotStartTime.FormatString("hhnnss");

    sPath = VISION_FOLDER + sData + "\\" ;

    if (!DirectoryExists(sPath)) CreateDir(sPath);

    if(LT.m_bLotOpen)sPath = VISION_FOLDER + LT.GetCrntLot() + "_" + sTime + ".dat";
    else             sPath = VISION_FOLDER + AnsiString("_") + sTime + ".dat";

//    if (!DirectoryExists(sPath)) CreateDir(sPath);

    hFile = FileOpen(sPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sPath.c_str());
        if (hFile == -1) { Trace("Err",(sPath + "is Can't made").c_str());  return false ; }
    }

    _sLog= _sLog + "\r\n" ;
    AnsiString sDate = Now().CurrentDateTime().FormatString("hh::nn::ss ");
    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , _sLog.c_str() , _sLog.Length());

    //Close File.
    FileClose(hFile);
    return true;
}

bool __fastcall CVisnComUnit::WriteLogMap(EN_ARAY_ID riId )//그냥 보고 싶어서 하나 만듬.
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath ,sFName ;
    AnsiString sData  ;
    AnsiString sTemp ,sTemp2 ;
    AnsiString sRslt  ;
    AnsiString sTime  ;
    TDateTime  CurrDate;
    TDateTime  CurrDateTime;
    int        iMgzCnt;
    int        hFile;
    DelPastLog();

    //Set Dir.
    sTemp        = CurrDate.CurrentDate().FormatString("yyyymmdd");
    sTime        = CurrDateTime.CurrentDate().FormatString("yyyymmdd_hh_nn_ss");
    sPath        = LOT_LOG_FOLDER + sTemp ;

    if(DM.ARAY[riId].GetLotNo() != "") sPath += "\\" + DM.ARAY[riId].GetLotNo();//   LT.GetCrntLot() ;
    else                               sPath += "\\NoLot"                      ;

    bool bRet = false ;

    if(!DirectoryExists(sPath)) bRet = UserFile.CreateDir(sPath);

    iMgzCnt = DM.ARAY[riId].GetID().ToIntDef(9999) / 100;

    //int iArayId = EN_ARAY_ID(riId);

    if(riId == riWR1) sFName = "Vision1"   ;
    if(riId == riWR2) sFName = "Vision2"   ;
    if(riId == riWR3) sFName = "Vision3"   ;
    if(riId == riPSB) sFName = "PostBuffer";

    if(DM.ARAY[riId].GetLotNo() != "") sPath += "\\" + sFName + ".csv" ;
    else                               sPath += "\\" + sTime  + ".csv" ;

    //File Open.
    hFile = FileOpen(sPath.c_str() , fmOpenWrite);

    if (hFile == -1) {
        hFile = FileCreate(sPath.c_str());
        if (hFile == -1) { Trace("Err",(sPath + "is Can't made").c_str());  return false ; }
    }

    sData  = "=================================================================================\r\n";
    sData += "Time,LotNo,MgzNo,SlotNo\r\n"        ;
    sData += Now().FormatString("hh:nn:ss") + "," ;
    sData += DM.ARAY[riId].GetLotNo()       + "," ;
    sData += AnsiString(DM.ARAY[riId].GetID().ToIntDef(9999)/100) + ","   ;
    sData += AnsiString(DM.ARAY[riId].GetID().ToIntDef(99  )%100) + "\r\n";
    int iStat;
    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
//        sRslt = "" ;
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            //sRslt += IntToHex(((int)DM.ARAY[riId].GetStat(r,c)) , 2)  ;
            iStat = (int)DM.ARAY[riId].GetStat(r,c);

                 if((EN_CHIP_STAT)csFail    == iStat ) sData +=  "HEAT    ," ;
            else if((EN_CHIP_STAT)csEmpty   == iStat ) sData +=  "EMPTY   ," ;
            else if((EN_CHIP_STAT)csNone    == iStat ) sData +=  "NONE    ," ;
            else if((EN_CHIP_STAT)csUnkwn   == iStat ) sData +=  "UNKNWN  ," ;
            else if((EN_CHIP_STAT)csWork    == iStat ) sData +=  "        ," ;
            else if((EN_CHIP_STAT)csRslt0   == iStat ) sData +=  "0       ," ;
            else if((EN_CHIP_STAT)csRslt1   == iStat ) sData +=  "1       ," ;
            else if((EN_CHIP_STAT)csRslt2   == iStat ) sData +=  "2       ," ;
            else if((EN_CHIP_STAT)csRslt3   == iStat ) sData +=  "3       ," ;
            else if((EN_CHIP_STAT)csRslt4   == iStat ) sData +=  "4       ," ;
            else if((EN_CHIP_STAT)csRslt5   == iStat ) sData +=  "5       ," ;
            else if((EN_CHIP_STAT)csRslt6   == iStat ) sData +=  "6       ," ;
            else if((EN_CHIP_STAT)csRslt7   == iStat ) sData +=  "7       ," ;
            else if((EN_CHIP_STAT)csRslt8   == iStat ) sData +=  "8       ," ;
            else if((EN_CHIP_STAT)csRslt9   == iStat ) sData +=  "9       ," ;
            else if((EN_CHIP_STAT)csRsltA   == iStat ) sData +=  "A       ," ;
            else if((EN_CHIP_STAT)csRsltB   == iStat ) sData +=  "B       ," ;
            else if((EN_CHIP_STAT)csRsltC   == iStat ) sData +=  "C       ," ;
            else if((EN_CHIP_STAT)csRsltD   == iStat ) sData +=  "D       ," ;
            else if((EN_CHIP_STAT)csRsltE   == iStat ) sData +=  "E       ," ;
            else if((EN_CHIP_STAT)csRsltF   == iStat ) sData +=  "F       ," ;
            else if((EN_CHIP_STAT)csRsltG   == iStat ) sData +=  "G       ," ;
            else if((EN_CHIP_STAT)csRsltH   == iStat ) sData +=  "H       ," ;
            else if((EN_CHIP_STAT)csRsltI   == iStat ) sData +=  "I       ," ;
            else if((EN_CHIP_STAT)csRsltK   == iStat ) sData +=  "K       ," ;
            else if((EN_CHIP_STAT)csRsltL   == iStat ) sData +=  "L       ," ;


            




//            else if((EN_CHIP_STAT)csToBuf   == iStat ) sData +=  "ToBuf "     ;
//            else if((EN_CHIP_STAT)csFromBuf == iStat ) sData +=  "FromBuf "   ;

//            sData += (AnsiString)iStat + ",";
//            sTemp2 = sTemp2.sprintf("%02d",(int)DM.ARAY[riId].GetStat(r,c)) + "_";
//            sRslt += sTemp2 ;
        }
//        sTemp.printf("R%02d", r);
//        UserINI.Save(sPath.c_str()  , sTime  , sTemp , sRslt );
        sData += "\r\n";
    }

    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);

    return true ;
}



#define VISN_DATA_FOLDER "d:\\LotData\\"
bool __fastcall CVisnComUnit::WriteArayVisnFormat(CArray * riAray)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sLotNo , sTime ;
    AnsiString sAllField ;
    AnsiString sFailCnt ;
    int        iFailCnt ;

    AnsiString sRow , sCol , sItem ;

    //delete past data.
    UserFile.ClearDirDate(VISN_DATA_FOLDER , Now() - 30);

    if( riAray->GetLotNo() == "" ) sLotNo = "NoLot";
    else                           sLotNo = riAray->GetLotNo();

    sPath  = VISN_DATA_FOLDER ;
    sPath += Now().CurrentDateTime().FormatString("yyyymmdd") + "\\" ;

    if(!DirectoryExists(sPath))
        UserFile.CreateDir(sPath.c_str());

    sPath = sPath + sLotNo + Now().CurrentDateTime().FormatString("_hhnnss") + ".dat" ;

    iFailCnt = 0 ;
    sAllField = "" ;

    //int iTemp = riAray -> GetMaxRow() ;


    for(int r = 0 ; r < riAray -> GetMaxRow() ; r++) {
        for(int c = 0 ; c < riAray -> GetMaxCol() ; c++) {
            if(csWork != riAray -> GetStat(r,c)) {
                iFailCnt++;

                sCol .sprintf("%02d" , c+1);
                sRow .sprintf("%02d" , r+1);

                     if(riAray -> GetStat(r,c) == csRslt0 ) sItem = "0";
                else if(riAray -> GetStat(r,c) == csRslt1 ) sItem = "1";
                else if(riAray -> GetStat(r,c) == csRslt2 ) sItem = "2";
                else if(riAray -> GetStat(r,c) == csRslt3 ) sItem = "3";
                else if(riAray -> GetStat(r,c) == csRslt4 ) sItem = "4";
                else if(riAray -> GetStat(r,c) == csRslt5 ) sItem = "5";
                else if(riAray -> GetStat(r,c) == csRslt6 ) sItem = "6";
                else if(riAray -> GetStat(r,c) == csRslt7 ) sItem = "7";
                else if(riAray -> GetStat(r,c) == csRslt8 ) sItem = "8";
                else if(riAray -> GetStat(r,c) == csRslt9 ) sItem = "9";
                else if(riAray -> GetStat(r,c) == csRsltA ) sItem = "A";
                else if(riAray -> GetStat(r,c) == csRsltB ) sItem = "B";
                else if(riAray -> GetStat(r,c) == csRsltC ) sItem = "C";
                else if(riAray -> GetStat(r,c) == csRsltD ) sItem = "D";
                else if(riAray -> GetStat(r,c) == csRsltE ) sItem = "E";
                else if(riAray -> GetStat(r,c) == csRsltF ) sItem = "F";
                else if(riAray -> GetStat(r,c) == csRsltG ) sItem = "G";
                else if(riAray -> GetStat(r,c) == csRsltH ) sItem = "H";
                else if(riAray -> GetStat(r,c) == csRsltI ) sItem = "I";
                else if(riAray -> GetStat(r,c) == csRsltJ ) sItem = "J";
                else if(riAray -> GetStat(r,c) == csRsltK ) sItem = "K";
                else if(riAray -> GetStat(r,c) == csRsltL ) sItem = "L";
                sAllField += sCol + sRow + sItem ; //+ " ";

            }
        }
    }

    sFailCnt.sprintf("%03d",iFailCnt);

    sAllField = sFailCnt + sAllField ;

    int hFile;
    hFile = FileOpen(sPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sPath.c_str());
        if (hFile == -1) { Trace("Err",(sPath + " is Can't made").c_str());  return false; }
    }

    FileSeek (hFile , 0                 , SEEK_END          );
    FileWrite(hFile , sAllField.c_str() , sAllField.Length());

    //Close File.
    FileClose(hFile);

    return true;
}
