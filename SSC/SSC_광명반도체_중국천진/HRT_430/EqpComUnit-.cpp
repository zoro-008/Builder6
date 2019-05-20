//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "EqpComUnit.h"
#include "UserFile.h"
//#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "UserIni.h"
#include "LotUnit.h"
#include "SLogUnit.h"
#include "Sequence.h"
#include "PstnMan.h"

#pragma package(smart_init)

CEqpComUnit EC;

CEqpComUnit::CEqpComUnit(void)
{
    memset(&Step    , 0 , sizeof(TStep));
    memset(&PreStep , 0 , sizeof(TStep));
}

CEqpComUnit::~CEqpComUnit(void)
{

}

//사용시 아이피 맞지 않으면 오래 걸림.
bool __fastcall CEqpComUnit::NetDrvConnect(AnsiString sDrv , AnsiString sIp, AnsiString sPath)
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
    //if(Result!=NO_ERROR){
    //    //Result = WNetCancelConnection2(slpLocalName.c_str(),CONNECT_UPDATE_PROFILE,FALSE);       //Sever Network Drive
    //    if(Result != NO_ERROR) return false ;//FM_MsgOk("ERROR","Net Connect Failed! Check the Connection.");
    //    Result = WNetAddConnection2(&NetRes, "mds", "mds", CONNECT_INTERACTIVE); //Connect Network Drive
    //    if(Result != NO_ERROR) return false ;//FM_MsgOk("ERROR","Net Connect Failed! Check the Connection.");
    //}
    
    return true ;
}

//사용시 아이피 맞지 않으면 오래 걸림.
bool __fastcall CEqpComUnit::NetCancelConnection(AnsiString sDrv)
{
    AnsiString slpLocalName ;
    DWORD Result;

    slpLocalName = sDrv + ":"; // "Z:";

    Result = WNetCancelConnection2(slpLocalName.c_str(),CONNECT_UPDATE_PROFILE,FALSE);

    return (Result == NO_ERROR) ;
}

bool __fastcall CEqpComUnit::WritePostEqip(EN_ARAY_ID riId)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;
    AnsiString sTemp2 ;
    AnsiString sRslt  ;

    //Set Dir.
    sPath = OM.MstOptn.sPstEquipPath + "\\Data.ini" ;

    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        sRslt = "" ;
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            //sRslt += IntToHex(((int)DM.ARAY[riId].GetStat(r,c)) , 2)  ;
            sTemp2 = (int)DM.ARAY[riId].GetStat(r,c) ;
            sTemp2 = sTemp2.sprintf("%02d",(int)DM.ARAY[riId].GetStat(r,c)) + "_";
            sRslt += sTemp2 ;
        }
        sTemp.printf("R%02d", r);
        UserINI.Save(sPath.c_str()  , "WorkInfo"  , sTemp , sRslt );
    }

    return true ;
}

bool __fastcall CEqpComUnit::ReadPreEqip(EN_ARAY_ID riId)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;
    AnsiString sVal   ;
    AnsiString sRslt  ;

    //Set Dir.
//    sPath = OM.MstOptn.sPreEquipDrv + ":\\" + OM.MstOptn.sPreEquipPath + "\\Data.ini";

    if(!FileExists(sPath)) return false ;

    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        sTemp.printf("R%02d", r);
        UserINI.Load(sPath.c_str()  , "WorkInfo"  , sTemp , sRslt );
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            //sVal = "0x"+sRslt.SubString(c+1,1) ;
            sVal = sRslt.SubString(c*3+1,2) ; //sRslt.Delete(c+1,2) ;
            DM.ARAY[riId].SetStat(r,c,(EN_CHIP_STAT)StrToInt(sVal)  );
        }
    }

    return true ;
}

bool __fastcall CEqpComUnit::ReadLotData (void)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
//    sPath = OM.MstOptn.sPreEquipDrv + ":\\"  + OM.MstOptn.sPreEquipPath + "\\LotData.ini" ;

    if(!FileExists(sPath)) return false ;

    //Load Device
    for(int i = 0 ; i < MAX_LOT_CNT ; i++) {
        UserINI.Load(sPath.c_str()  , "WorkInfo"  , "sLotNo"+AnsiString(i) , LT.WorkInfo.sLotNo[i] );
    }
    UserINI.Load(sPath.c_str()  , "WorkInfo"  , "sJobFile " , LT.WorkInfo.sJobFile );
    UserINI.Load(sPath.c_str()  , "WorkInfo"  , "sOperator" , LT.WorkInfo.sOperator);

    return true ;
}

bool __fastcall CEqpComUnit::WriteLotData(void)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = OM.MstOptn.sPstEquipPath + "\\LotData.ini";

    UserINI.ClearFile(sPath) ;

    //Load Device.
    for(int i = 0 ; i < MAX_LOT_CNT ; i++) {
        UserINI.Save(sPath.c_str()  , "WorkInfo"  , "sLotNo"+AnsiString(i) , LT.WorkInfo.sLotNo[i] );
    }
    UserINI.Save(sPath.c_str()  , "WorkInfo"  , "sJobFile " , LT.WorkInfo.sJobFile );
    UserINI.Save(sPath.c_str()  , "WorkInfo"  , "sOperator" , LT.WorkInfo.sOperator);

    return true ;
}

bool __fastcall CEqpComUnit::WriteChange(void)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath ,sCrntDev ;

    //Set Dir.
    sPath = OM.MstOptn.sPstEquipPath + "\\DeviceChange.ini";

    UserINI.ClearFile(sPath) ;

    sCrntDev = OM.GetCrntDev();
    //Load Device.
    UserINI.Save(sPath.c_str()  , "DeviceName"  , "sCrntDev" , sCrntDev );

    return true ;
}

bool __fastcall CEqpComUnit::ReadChange(void)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath,sCrntDev;

    //Set Dir.
//    sPath = OM.MstOptn.sPreEquipDrv + ":\\" + OM.MstOptn.sPreEquipPath + "\\DeviceChange.ini";

    if(!FileExists(sPath)) return false ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "DeviceName"  , "sCrntDev" , sCrntDev );

    OM.SetCrntDev(sCrntDev);
    //PM.
}

typedef void (__closure *Func)();

bool CEqpComUnit::SendStart(bool _bUseThread)
{
    Step.iResetY = 10 ; PreStep.iResetY = 0 ;
    if(_bUseThread){
        CRunThread<Func> * func = new CRunThread<Func>(&ThreadStartOutput) ;
    }
    else {
        ThreadStartOutput();
    }
    return true ;

}


bool CEqpComUnit::SendReset(bool _bUseThread)
{
    Step.iResetY = 10 ; PreStep.iResetY = 0 ;
    if(_bUseThread){
        CRunThread<Func> * func = new CRunThread<Func>(&ThreadResetOutput) ;
    }
    else {
        ThreadResetOutput();
    }
    return true ;

}

bool CEqpComUnit::SendLotStart(bool _bUseThread)
{
    Step.iLotStartY = 10 ; PreStep.iLotStartY = 0 ;
    if(_bUseThread){
        CRunThread<Func> * func = new CRunThread<Func>(&ThreadLotStartOutput) ;
    }
    else {
        ThreadLotStartOutput();
    }
    return true ;
}

bool CEqpComUnit::SendModelChange(bool _bUseThread)
{
    Step.iModelChangeY = 10 ; PreStep.iModelChangeY = 0 ;
    if(_bUseThread){
        CRunThread<Func> * func = new CRunThread<Func>(&ThreadModelChangeOutput) ;
    }
    else {
        ThreadModelChangeOutput();
    }
    return true ;
}

void CEqpComUnit::ThreadStartOutput()
{
    CDelayTimer Timer ;
    while(1) {
        Application -> ProcessMessages() ;
        Sleep(10);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (m_tmReset.OnDelay(Step.iStartY == PreStep.iStartY && !OM.MstOptn.bDebugMode , 2000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiETC_PostEqpCommTO);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iStartY=%02d" , __FUNC__ , Step.iStartY );
            Trace("CEqpComUnit",sTemp.c_str());
            IO_SetY(yETC_PostEqpStart , false) ;
            Step.iStartY = 0 ;
            return ;
        }

        if(Step.iStartY != PreStep.iStartY) {
            sTemp = sTemp.sprintf("%s Step.iStartY=%02d" , __FUNC__ , Step.iStartY );
            Trace("CEqpComUnit",sTemp.c_str());
        }

        PreStep.iStartY = Step.iStartY ;

//        static int iSendCnt = 0  ;

        //Cycle.
        switch (Step.iStartY) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iReset=%02d" , __FUNC__ , Step.iStartY );
                      Trace("CEqpComUnit", sTemp.c_str());
                      IO_SetY(yETC_PostEqpStart , false) ;
                      Step.iStartY = 0 ;
                      return ;

            case  10: if(!IO_GetX(xETC_PostEqpReady)) {
                          EM_SetErr(eiETC_PostEqpNotReady);
                          EM_SetDisp(true) ;
                          return ;
                      }
                      IO_SetY(yETC_PostEqpStart , false) ;
                      Step.iStartY++;
                      continue ;

            case  11: IO_SetY(yETC_PostEqpStart , true) ;
                      Step.iStartY++;
                      continue ;

            case  12: if(IO_GetX(xETC_PostEqpStart)) continue ;
                      Step.iStartY++;
                      continue ;

            case  13: IO_SetY(yETC_PostEqpStart , false) ;
                      Step.iStartY = 0;
                      return ;
        }
    }

}

void CEqpComUnit::ThreadStartInput()
{
    CDelayTimer Timer ;
    while(1) {
        Application -> ProcessMessages() ;
        Sleep(10);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (m_tmReset.OnDelay(Step.iStartX == PreStep.iStartX && !OM.MstOptn.bDebugMode , 2000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiETC_PostEqpCommTO);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iStartX=%02d" , __FUNC__ , Step.iStartX );
            Trace("CEqpComUnit",sTemp.c_str());
            IO_SetY(yETC_PostEqpStart , false) ;
            Step.iStartX = 0 ;
            return ;
        }

        if(Step.iStartX != PreStep.iStartX) {
            sTemp = sTemp.sprintf("%s Step.iStartX=%02d" , __FUNC__ , Step.iStartX );
            Trace("CEqpComUnit",sTemp.c_str());
        }

        PreStep.iStartX = Step.iStartX ;

//        static int iSendCnt = 0  ;

        //Cycle.
        switch (Step.iStartX) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iReset=%02d" , __FUNC__ , Step.iStartX );
                      Trace("CEqpComUnit", sTemp.c_str());
                      IO_SetY(yETC_PostEqpStart , false) ;
                      Step.iStartX = 0 ;
                      return ;

            case  10: IO_SetY(yETC_PostEqpStart , false) ;
                      Step.iStartX++;
                      continue ;

            case  11: IO_SetY(yETC_PostEqpStart , true) ;
                      Step.iStartX++;
                      continue ;

            case  12: if(!IO_GetX(xETC_PostEqpStart)) continue ;
                      Step.iStartX++;
                      continue ;

            case  13: IO_SetY(yETC_PostEqpStart , false) ;
                      SEQ._bBtnStart = true ;
                      Step.iStartX = 0;
                      return ;
        }
    }

}

void CEqpComUnit::ThreadStopOutput()
{
    CDelayTimer Timer ;
    while(1) {
        Application -> ProcessMessages() ;
        Sleep(10);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (m_tmReset.OnDelay(Step.iStopY == PreStep.iStopY && !OM.MstOptn.bDebugMode , 2000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiETC_PostEqpCommTO);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iReset=%02d" , __FUNC__ , Step.iStopY );
            Trace("CEqpComUnit",sTemp.c_str());
            IO_SetY(yETC_PostEqpReset , false) ;
            Step.iStopY = 0 ;
            return ;
        }

        if(Step.iStopY != PreStep.iStopY) {
            sTemp = sTemp.sprintf("%s Step.iStopY=%02d" , __FUNC__ , Step.iStopY );
            Trace("CEqpComUnit",sTemp.c_str());
        }

        PreStep.iStopY = Step.iStopY ;

//        static int iSendCnt = 0  ;

        //Cycle.
        switch (Step.iStopY) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iReset=%02d" , __FUNC__ , Step.iStopY );
                      Trace("CEqpComUnit", sTemp.c_str());
                      IO_SetY(yETC_PostEqpStop , false) ;
                      Step.iStopY = 0 ;
                      return ;

            case  10: IO_SetY(yETC_PostEqpStop , false) ;
                      Step.iStopY++;
                      continue ;

            case  11: IO_SetY(yETC_PostEqpStop , true ) ;
                      Step.iStopY++;
                      continue ;

            case  12: if(IO_GetX(xETC_PostEqpStop)) continue ;
                      IO_SetY(yETC_PostEqpStop , false) ;
                      Step.iStopY = 0;
                      return ;
        }
    }
}

void CEqpComUnit::ThreadStopInput()
{
    CDelayTimer Timer ;
    while(1) {
        Application -> ProcessMessages() ;
        Sleep(10);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (m_tmReset.OnDelay(Step.iStopX == PreStep.iStopX && !OM.MstOptn.bDebugMode , 2000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiETC_PostEqpCommTO);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iReset=%02d" , __FUNC__ , Step.iStopX );
            Trace("CEqpComUnit",sTemp.c_str());
            IO_SetY(yETC_PostEqpReset , false) ;
            Step.iStopX = 0 ;
            return ;
        }

        if(Step.iStopX != PreStep.iStopX) {
            sTemp = sTemp.sprintf("%s Step.iStopX=%02d" , __FUNC__ , Step.iStopX );
            Trace("CEqpComUnit",sTemp.c_str());
        }

        PreStep.iStopX = Step.iStopX ;

//        static int iSendCnt = 0  ;

        //Cycle.
        switch (Step.iStopX) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iReset=%02d" , __FUNC__ , Step.iStopX );
                      Trace("CEqpComUnit", sTemp.c_str());
                      IO_SetY(yETC_PostEqpStop , false) ;
                      Step.iStopX = 0 ;
                      return ;

            case  10: IO_SetY(yETC_PostEqpStop , false) ;
                      Step.iStopX++;
                      continue ;

            case  11: IO_SetY(yETC_PostEqpStop , true ) ;
                      Step.iStopX++;
                      continue ;

            case  12: if(!IO_GetX(xETC_PostEqpStop)) continue ;
                      IO_SetY(yETC_PostEqpStop , false) ;
                      SEQ._bBtnStop = true ;
                      Step.iStopX = 0;
                      return ;
        }
    }
}

void CEqpComUnit::ThreadResetOutput()
{
    CDelayTimer Timer ;
    while(1) {
        Application -> ProcessMessages() ;
        Sleep(10);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (m_tmReset.OnDelay(Step.iResetY == PreStep.iResetY && !OM.MstOptn.bDebugMode , 2000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiETC_PostEqpCommTO);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iResetY=%02d" , __FUNC__ , Step.iResetY );
            Trace("CEqpComUnit",sTemp.c_str());
            IO_SetY(yETC_PostEqpReset , false) ;
            Step.iResetY = 0 ;
            return ;
        }

        if(Step.iResetY != PreStep.iResetY) {
            sTemp = sTemp.sprintf("%s Step.iResetY=%02d" , __FUNC__ , Step.iResetY );
            Trace("CEqpComUnit",sTemp.c_str());
        }

        PreStep.iResetY = Step.iResetY ;

//        static int iSendCnt = 0  ;

        //Cycle.
        switch (Step.iResetY) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iResetY=%02d" , __FUNC__ , Step.iResetY );
                      Trace("CEqpComUnit", sTemp.c_str());
                      IO_SetY(yETC_PostEqpReset , false) ;
                      Step.iResetY = 0 ;
                      return ;

            case  10: IO_SetY(yETC_PostEqpReset , false) ;
                      Step.iResetY++;
                      continue ;

            case  11: IO_SetY(yETC_PostEqpReset , true ) ;
                      Step.iResetY++;
                      continue ;

            case  12: if(IO_GetX(xETC_PostEqpReset)) continue ;
                      Step.iResetY++;
                      continue ;

            case  13: IO_SetY(yETC_PostEqpReset , false) ;
                      Step.iResetY = 0;
                      return ;
        }
    }
}

void CEqpComUnit::ThreadResetInput()
{
    CDelayTimer Timer ;
    while(1) {
        Application -> ProcessMessages() ;
        Sleep(10);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (m_tmReset.OnDelay(Step.iResetX == PreStep.iResetX && !OM.MstOptn.bDebugMode , 2000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiETC_PostEqpCommTO);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iResetX=%02d" , __FUNC__ , Step.iResetX );
            Trace("CEqpComUnit",sTemp.c_str());
            IO_SetY(yETC_PostEqpReset , false) ;
            Step.iResetX = 0 ;
            return ;
        }

        if(Step.iResetX != PreStep.iResetX) {
            sTemp = sTemp.sprintf("%s Step.iResetX=%02d" , __FUNC__ , Step.iResetX );
            Trace("CEqpComUnit",sTemp.c_str());
        }

        PreStep.iResetX = Step.iResetX ;

//        static int iSendCnt = 0  ;

        //Cycle.
        switch (Step.iResetX) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iResetX=%02d" , __FUNC__ , Step.iResetX );
                      Trace("CEqpComUnit", sTemp.c_str());
                      IO_SetY(yETC_PostEqpReset , false) ;
                      Step.iResetX = 0 ;
                      return ;

            case  10: IO_SetY(yETC_PostEqpReset , false) ;
                      Step.iResetX++;
                      continue ;

            case  11: IO_SetY(yETC_PostEqpReset , true ) ;
                      Step.iResetX++;
                      continue ;

            case  12: if(!IO_GetX(xETC_PostEqpReset)) continue ;
                      Step.iResetX++;
                      continue ;

            case  13: IO_SetY(yETC_PostEqpReset , false) ;
                      SEQ._bBtnReset = true ;
                      Step.iResetX = 0;
                      return ;
        }
    }
}

void CEqpComUnit::ThreadLotStartOutput()
{
    while(1) {
        Application -> ProcessMessages() ;
        Sleep(10);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (m_tmLotStart.OnDelay(Step.iLotStartY == PreStep.iLotStartY && !OM.MstOptn.bDebugMode , 2000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiETC_PostEqpLotStr);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iLotStartY=%02d" , __FUNC__ , Step.iLotStartY );
            Trace("CEqpComUnit",sTemp.c_str());
            Step.iLotStartY = 0 ;
            return ;
        }

        if(Step.iLotStartY != PreStep.iLotStartY) {
            sTemp = sTemp.sprintf("%s Step.iLotStartY=%02d" , __FUNC__ , Step.iLotStartY );
            Trace("CEqpComUnit",sTemp.c_str());
        }

        PreStep.iLotStartY = Step.iLotStartY ;

        //Cycle.
        switch (Step.iLotStartY) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iLotStartY=%02d" , __FUNC__ , Step.iLotStartY );
                      Trace("CEqpComUnit", sTemp.c_str());
                      Step.iLotStartY = 0 ;
                      return ;

            case  10: if(!IO_GetX(xETC_PostEqpReady)) {
                          EM_SetErr(eiETC_PostEqpNotReady);
                          EM_SetDisp(true) ;
                          return ;
                      }
                      IO_SetY(yETC_PostEqpLotStart , false) ;
                      WriteLotData();
                      Step.iLotStartY++;
                      continue ;

            case  11: IO_SetY(yETC_PostEqpLotStart , true) ;
                      Step.iLotStartY++;
                      continue ;

            case  12: if(!IO_GetX(xETC_PostEqpReady)) continue ;
                      Step.iLotStartY++;
                      continue ;

            case  13: if(IO_GetX(xETC_PostEqpReady)) continue ;
                      Step.iLotStartY++;
                      continue ;

            case  14: IO_SetY(yETC_PostEqpLotStart ,false) ;
                      Step.iLotStartY = 0 ;
                      return ;
        }
    }
}

void CEqpComUnit::ThreadLotStartInput()
{
    while(1) {
        Application -> ProcessMessages() ;
        Sleep(10);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (m_tmLotStart.OnDelay(Step.iLotStartX == PreStep.iLotStartX && !OM.MstOptn.bDebugMode , 2000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiETC_PostEqpLotStr);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iLotStartX=%02d" , __FUNC__ , Step.iLotStartX );
            Trace("CEqpComUnit",sTemp.c_str());
            Step.iLotStartX = 0 ;
            return ;
        }

        if(Step.iLotStartX != PreStep.iLotStartX) {
            sTemp = sTemp.sprintf("%s Step.iLotStartX=%02d" , __FUNC__ , Step.iLotStartX );
            Trace("CEqpComUnit",sTemp.c_str());
        }

        PreStep.iLotStartX = Step.iLotStartX ;

        //Cycle.
        switch (Step.iLotStartX) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iLotStartX=%02d" , __FUNC__ , Step.iLotStartX );
                      Trace("CEqpComUnit", sTemp.c_str());
                      Step.iLotStartX = 0 ;
                      return ;

            case  10: IO_SetY(yETC_PostEqpReady , false) ;
                      ReadLotData();
///                      LT.LotOpen(LT.WorkInfo.sLotNo , LT.WorkInfo.sJobFile , LT.WorkInfo.sOperator , LT.WorkInfo);

                      OM.LoadJobFile (LT.WorkInfo.sJobFile);
                      DM.ARAY[riLD1].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);
                      DM.ARAY[riPR1].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
                      DM.ARAY[riPR2].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
                      DM.ARAY[riWRP].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
                      DM.ARAY[riWRK].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
                      DM.ARAY[riPS1].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
                      DM.ARAY[riPS2].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);
                      DM.ClearMap    (                    );
                      OM.SaveLastInfo(                    );
                      PM.Load        (LT.WorkInfo.sJobFile);

                      Step.iLotStartX++;
                      continue ;

            case  11: IO_SetY(yETC_PostEqpReady , true) ;
                      Step.iLotStartX = 0 ;
                      return ;
        }
    }
}

/*void CEqpComUnit::ThreadLotEnd()  몰라 몰랑......ㅇ룸.ㅇ룽ㅁ.ㄴ르ㅜㅁㄴㅇㄹ,.
{
    while(1) {
        Application -> ProcessMessages() ;
        Sleep(10);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (m_tmLotStart.OnDelay(Step.iLotEnd == PreStep.iLotEnd && !OM.MstOptn.bDebugMode , 2000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiETC_PostEqpLotEnd);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iLotEnd=%02d" , __FUNC__ , Step.iLotEnd );
            Trace("CEqpComUnit",sTemp.c_str());
            Step.iLotEnd = 0 ;
            return ;
        }

        if(Step.iLotEnd != PreStep.iLotEnd) {
            sTemp = sTemp.sprintf("%s Step.iLotEnd=%02d" , __FUNC__ , Step.iLotEnd );
            Trace("CEqpComUnit",sTemp.c_str());
        }

        PreStep.iLotEnd = Step.iLotEnd ;

        //Cycle.
        switch (Step.iLotEnd) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iLotEnd=%02d" , __FUNC__ , Step.iLotEnd );
                      Trace("CEqpComUnit", sTemp.c_str());
                      Step.iLotEnd = 0 ;
                      return ;

            case  10: IO_SetY(yETC_PostEqpLotEnd , true) ;
                      Step.iLotEnd++;
                      continue ;

            case  11: //if(IO_GetX(xETC_PostEqpLotEnd)) continue ;
                      IO_SetY(yETC_PostEqpLotEnd , false) ;
                      LT.LotEnd();
                      Step.iLotEnd = 0 ;
                      return ;
        }
    }
}
*/
void CEqpComUnit::ThreadModelChangeOutput()
{
    while(1) {
        Application -> ProcessMessages() ;
        Sleep(10);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (m_tmModelChange.OnDelay(Step.iModelChangeY == PreStep.iModelChangeY && !OM.MstOptn.bDebugMode , 2000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiETC_PostEqpModelChg);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iModelChangeY=%02d" , __FUNC__ , Step.iModelChangeY );
            Trace("CEqpComUnit",sTemp.c_str());
            Step.iModelChangeY = 0 ;
            return ;
        }

        if(Step.iModelChangeY != PreStep.iModelChangeY) {
            sTemp = sTemp.sprintf("%s Step.iModelChangeY=%02d" , __FUNC__ , Step.iModelChangeY );
            Trace("CEqpComUnit",sTemp.c_str());
        }

        PreStep.iModelChangeY = Step.iModelChangeY ;

        //Cycle.
        switch (Step.iModelChangeY) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iModelChangeY=%02d" , __FUNC__ , Step.iModelChangeY );
                      Trace("CEqpComUnit", sTemp.c_str());
                      Step.iModelChangeY = 0 ;
                      return  ;

            case  10: if(!IO_GetX(xETC_PostEqpReady)) {
                          EM_SetErr(eiETC_PostEqpNotReady);
                          EM_SetDisp(true) ;
                          return ;
                      }
                      WriteChange();

            case  11: IO_SetY(yETC_PostEqpJobChange , true) ;
                      Step.iModelChangeY++;
                      continue ;

            case  12: if(!IO_GetX(xETC_PostEqpReady)) continue ;
                      Step.iModelChangeY++;
                      continue ;

            case  13: if(IO_GetX(xETC_PostEqpReady)) continue ;
                      IO_SetY(yETC_PostEqpJobChange ,false) ;
                      Step.iModelChangeY = 0 ;
                      return ;
        }
    }
}

void CEqpComUnit::ThreadModelChangeInput()
{
    while(1) {
        Application -> ProcessMessages() ;
        Sleep(10);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (m_tmModelChange.OnDelay(Step.iModelChangeX == PreStep.iModelChangeX && !OM.MstOptn.bDebugMode , 2000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiETC_PostEqpModelChg);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iModelChangeX=%02d" , __FUNC__ , Step.iModelChangeX );
            Trace("CEqpComUnit",sTemp.c_str());
            Step.iModelChangeX = 0 ;
            return ;
        }

        if(Step.iModelChangeX != PreStep.iModelChangeX) {
            sTemp = sTemp.sprintf("%s Step.iModelChangeX=%02d" , __FUNC__ , Step.iModelChangeX );
            Trace("CEqpComUnit",sTemp.c_str());
        }

        PreStep.iModelChangeX = Step.iModelChangeX ;

        //Cycle.
        switch (Step.iModelChangeX) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iModelChangeX=%02d" , __FUNC__ , Step.iModelChangeX );
                      Trace("CEqpComUnit", sTemp.c_str());
                      Step.iModelChangeX = 0 ;
                      return  ;

            case  10: IO_SetY(yETC_PostEqpReady , false) ;
                      ReadChange();
                      Step.iModelChangeX++;
                      continue ;

            case  11: IO_SetY(yETC_PostEqpReady , true) ;
                      Step.iModelChangeX = 0 ;
                      return ;
        }
    }
}

void CEqpComUnit::ThreadDataOutput()
{
    while(1) {
        Application -> ProcessMessages() ;
        Sleep(10);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (m_tmModelChange.OnDelay(Step.iDataY == PreStep.iDataY && !OM.MstOptn.bDebugMode , 2000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiETC_PostEqpData);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iDataY=%02d" , __FUNC__ , Step.iDataY );
            Trace("CEqpComUnit",sTemp.c_str());
            Step.iDataY = 0 ;
            return ;
        }

        if(Step.iDataY != PreStep.iDataY) {
            sTemp = sTemp.sprintf("%s Step.iDataY=%02d" , __FUNC__ , Step.iDataY );
            Trace("CEqpComUnit",sTemp.c_str());
        }

        PreStep.iDataY = Step.iDataY ;

        //Cycle.
        switch (Step.iDataY) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iDataY=%02d" , __FUNC__ , Step.iDataY );
                      Trace("CEqpComUnit", sTemp.c_str());
                      Step.iDataY = 0 ;
                      return  ;

            case  10: if(!IO_GetX(xETC_PostEqpReady)) {
                          EM_SetErr(eiETC_PostEqpNotReady);
                          EM_SetDisp(true) ;
                          return ;
                      }
                      WritePostEqip(riPS2);
                      IO_SetY(yETC_PostEqpData , true) ;
                      Step.iDataY++;
                      continue ;

            case  11: if(!IO_GetX(xETC_PostEqpReady)) continue ;
                      Step.iDataY++;
                      continue ;

            case  12: if(IO_GetX(xETC_PostEqpReady)) continue ;
                      IO_SetY(yETC_PostEqpData , false) ;
                      Step.iDataY = 0 ;
                      return ;


        }
    }
}

void CEqpComUnit::ThreadDataInput()
{
    while(1) {
        Application -> ProcessMessages() ;
        Sleep(10);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (m_tmModelChange.OnDelay(Step.iDataX == PreStep.iDataY && !OM.MstOptn.bDebugMode , 2000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiETC_PostEqpData);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iDataY=%02d" , __FUNC__ , Step.iDataY );
            Trace("CEqpComUnit",sTemp.c_str());
            Step.iDataY = 0 ;
            return ;
        }

        if(Step.iDataY != PreStep.iDataY) {
            sTemp = sTemp.sprintf("%s Step.iDataY=%02d" , __FUNC__ , Step.iDataY );
            Trace("CEqpComUnit",sTemp.c_str());
        }

        PreStep.iDataY = Step.iDataY ;

        //Cycle.
        switch (Step.iDataY) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iDataY=%02d" , __FUNC__ , Step.iDataY );
                      Trace("CEqpComUnit", sTemp.c_str());
                      Step.iDataY = 0 ;
                      return  ;

            case  10: IO_SetY(yETC_PostEqpReady , false) ;
//                      ReadPreEqip(riPRB);
                      Step.iDataY++;
                      continue ;

            case  11: IO_SetY(yETC_PostEqpReady , true) ;
                      Step.iDataY++;
                      continue ;

            case  12: if(IO_GetX(xETC_PostEqpReady)) continue ;
                      Step.iDataY = 0 ;
                      return ;


        }
    }
}

void __fastcall CEqpComUnit::DelPastLog()
{
//    UserFile.ClearDirDate(VISION_FOLDER , Now() - 30);
}

