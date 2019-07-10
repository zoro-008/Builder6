//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "CommUnit.h"
#include "UserFile.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "UserIni.h"
#include "LotUnit.h"

#pragma package(smart_init)

CCommUnit COM;

CCommUnit::CCommUnit(void)
{
}

CCommUnit::~CCommUnit(void)
{
}

void __fastcall CCommUnit::NetDrvConnect(AnsiString sDrv , AnsiString sIp, AnsiString sPath)
{
    AnsiString sTemp, slpLocalName, slpRemoteName ;
    NETRESOURCE NetRes;
    NetRes.dwScope = RESOURCE_GLOBALNET;
    NetRes.dwType  = RESOURCETYPE_ANY;
    NetRes.dwDisplayType = RESOURCEDISPLAYTYPE_GENERIC;
    NetRes.dwUsage = RESOURCEUSAGE_CONNECTABLE;
    slpLocalName = sDrv + ":"; // "Z:";
    NetRes.lpLocalName  = slpLocalName.c_str();
    slpRemoteName = "\\\\" + sIp + "\\" + sPath; //" \\\\192.168.0.76\\com";
    NetRes.lpRemoteName = slpRemoteName.c_str();
    NetRes.lpProvider    = NULL;

    DWORD Result;
    Result = WNetAddConnection2(&NetRes, "mds", "mds", CONNECT_INTERACTIVE);
    if(Result!=NO_ERROR){
        Result = WNetCancelConnection2(slpLocalName.c_str(),CONNECT_UPDATE_PROFILE,FALSE);       //Sever Network Drive
        if(Result != NO_ERROR) EM_SetErr(eiETC_NetSeverFail );
        Result = WNetAddConnection2(&NetRes, "mds", "mds", CONNECT_INTERACTIVE); //Connect Network Drive
        if(Result != NO_ERROR) EM_SetErr(eiETC_NetConectFail);
    }
}

bool __fastcall CCommUnit::WritePostEqip(EN_ARAY_ID riId)
{
    //Local Var.
    int        hFile       ;
    AnsiString sPath,sName ;
    AnsiString sNewName    ;
    AnsiString sTemp       ;
    int        sState      ;

    sPath = "D:\\COM"  ; //430 Master Option 으로 빼면 귀찮을듯 하니 일단 ㄱ .
    sName = "Data.Log" ;
    sTemp = sPath + "\\" + sName;
    if (!DirectoryExists(sPath)) CreateDir(sPath);

    if(FileExists(sTemp)){
        sNewName = ChangeFileExt(sTemp,".BAK");
        DeleteFile(sNewName);
        RenameFile(sTemp , sNewName);
    }
    sTemp = sPath + "\\" + sName;
    hFile = FileOpen(sTemp.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sTemp.c_str());
        if (hFile == -1) return false;
    }
    //Save.
    for (int iRCount = 0; iRCount < OM.DevInfo.iRowCnt; iRCount ++ ){
        sTemp.printf("R%2d =", iRCount);
        FileSeek (hFile , 0             , SEEK_END      );
        FileWrite(hFile , sTemp.c_str() , sTemp.Length());
        for (int iCCount = 0; iCCount < OM.DevInfo.iColCnt; iCCount ++ ){
            sState = DM.ARAY[riId].GetStat(iRCount , iCCount);
//            sTemp = sState + ";";
            sTemp.printf("%2d;",sState);
            FileSeek (hFile , 0             , SEEK_END      );
            FileWrite(hFile , sTemp.c_str() , sTemp.Length());
        }
        sTemp = "\r\n";
        FileSeek (hFile , 0             , SEEK_END      );
        FileWrite(hFile , sTemp.c_str() , sTemp.Length());
    }

    //Close File.
    FileClose(hFile);
    return true;
}
void __fastcall CCommUnit::ReadPreEqip(EN_ARAY_ID riId)
{
    //Local Var.
    int hFile       ;
    int iFileHandle ;
    int iFileLength ;
    char *pszBuffer ;
    AnsiString sPath, Temp ,sName;

    sName = "Data.Log" ;
//    sPath = OM.MstOptn.sPreEquipDrv + ":\\" + sName;

    iFileHandle = FileOpen(sPath.c_str(), fmOpenRead);
    iFileLength = FileSeek(iFileHandle,0,2);
    FileSeek(iFileHandle,0,0);
    pszBuffer = new char[iFileLength+1];
    //메모리 동적 할당 나중에 해제하기...!
    FileRead(iFileHandle, pszBuffer, iFileLength);
    FileClose(iFileHandle);

    AnsiString AllField = pszBuffer;
    int divpos;

    for (int iRCount = 0; iRCount < OM.DevInfo.iRowCnt; iRCount ++ ) {
        divpos = AllField.AnsiPos("=");
        Temp = AllField.SubString(1,divpos-1);
        Temp = Temp.Trim();
        AllField.Delete(1,divpos);

        for ( int iCCount = 0; iCCount < OM.DevInfo.iColCnt; iCCount ++ ) {
            divpos = AllField.AnsiPos(";");
            Temp = AllField.SubString(1,divpos-1);
            Temp = Temp.Trim();
            AllField.Delete(1,divpos);
            DM.ARAY[riId].SetStat(iRCount , iCCount , (EN_CHIP_STAT)Temp.ToIntDef(-1)) ;
        }
    }

    delete [] pszBuffer;
}
void __fastcall CCommUnit::ReadLotData (void)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = OM.MstOptn.sPreEquipPath + "\\LotData.INI" ;

    //Load Device
    UserINI.Load(sPath.c_str()  , "SLotInfo"  , "sLotNo   " , LT.LotInfo.sLotNo   );
    UserINI.Load(sPath.c_str()  , "SLotInfo"  , "sJobFile " , LT.LotInfo.sJobFile );
    UserINI.Load(sPath.c_str()  , "SLotInfo"  , "sOperator" , LT.LotInfo.sOperator);
}

void __fastcall CCommUnit::WriteLotData(void)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = OM.MstOptn.sPreEquipPath + "\\LotData.INI" ;

    UserINI.ClearFile(sPath) ;

    //Load Device.
    UserINI.Save(sPath.c_str()  , "SLotInfo"  , "sLotNo   " , LT.LotInfo.sLotNo   );
    UserINI.Save(sPath.c_str()  , "SLotInfo"  , "sJobFile " , LT.LotInfo.sJobFile );
    UserINI.Save(sPath.c_str()  , "SLotInfo"  , "sOperator" , LT.LotInfo.sOperator);
}

void __fastcall CCommUnit::WriteChange(void)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath ,sCrntDev ;

    //Set Dir.
    sPath = "D:\\COM\\DeviceChange.INI";
    sPath = OM.MstOptn.sPreEquipPath + "\\LotData.INI" ;

    UserINI.ClearFile(sPath) ;

    sCrntDev = OM.GetCrntDev();
    //Load Device.
    UserINI.Save(sPath.c_str()  , "DeviceName"  , "sCrntDev" , sCrntDev );

}

void __fastcall CCommUnit::ReadChange(void)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath,sCrntDev;

    //Set Dir.
//    sPath = OM.MstOptn.sPreEquipDrv + ":\\DeviceChange.INI";

    //Load Device.
    UserINI.Load(sPath.c_str()  , "DeviceName"  , "sCrntDev" , sCrntDev );

    OM.SetCrntDev(sCrntDev);
}

