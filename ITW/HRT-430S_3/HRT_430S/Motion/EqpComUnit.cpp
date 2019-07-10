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

template <typename T>
class CRunThread
{
    class TRunThread : public TThread
    {
        private:
            T Func;

        public:
            __fastcall TRunThread(T func) : TThread(false)
            {
                FreeOnTerminate = true;
                Func = func;
            }
            void __fastcall Execute()
            {
                Func();
            }
    };

    public:
        CRunThread(T func)
        {
            TRunThread*  thread = new TRunThread(func);
        }
};

typedef void (__closure *Func)();

EN_PRE_EQP_SEND iPreSendMsg ;
EN_PST_EQP_SEND iPstSendMsg ;

/*  ** 사용법**
typedef void (__closure *Func)();
void __fastcall TForm1::Button3Click(TObject *Sender)
{
    Button3->Enabled = false ;
    CRunThread<Func> * func = new CRunThread<Func>(&TestFunc) ;
}

void TForm1::TestFunc()
{
    // 엄청 시간이 걸리는 루틴.
    int  k;
    for(int c = 0; c < 299999999; c++)
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

CEqpComUnit EC;


void CEqpComUnit::ThreadPreEqpMsg()
{
/*
         if(iPreSendMsg == rsNone      ) {                                                                                      }
    else if(iPreSendMsg == rsStart     ) {IO_SetY(yETC_PreEqpStart  , true ); Sleep(1000); IO_SetY(yETC_PreEqpStart  , false ); }
    else if(iPreSendMsg == rsStop      ) {IO_SetY(yETC_PreEqpStop   , true ); Sleep(1000); IO_SetY(yETC_PreEqpStop   , false ); }
    else if(iPreSendMsg == rsReset     ) {IO_SetY(yETC_PreEqpReset  , true ); Sleep(1000); IO_SetY(yETC_PreEqpReset  , false ); }
  //else if(iPreSendMsg == rsReady     ) {IO_SetY(yETC_PreEqpReady  , true ); Sleep(1000); IO_SetY(yETC_PreEqpReady  , false ); } 요것은 함수가 따로 있음.
    else if(iPreSendMsg == rsSpare1    ) {IO_SetY(yETC_PreEqpSpare1 , true ); Sleep(1000); IO_SetY(yETC_PreEqpSpare1 , false ); }
    else if(iPreSendMsg == rsSpare2    ) {IO_SetY(yETC_PreEqpSpare2 , true ); Sleep(1000); IO_SetY(yETC_PreEqpSpare2 , false ); }
    else if(iPreSendMsg == rsSpare3    ) {IO_SetY(yETC_PreEqpSpare4 , true ); Sleep(1000); IO_SetY(yETC_PreEqpSpare4 , false ); }
    else if(iPreSendMsg == rsSpare4    ) {IO_SetY(yETC_PreEqpSpare4 , true ); Sleep(1000); IO_SetY(yETC_PreEqpSpare4 , false ); }
*/
}

void CEqpComUnit::ThreadPstEqpMsg()
{

         if(iPstSendMsg == ::psNone    ) {                                                                                            }
    else if(iPstSendMsg == psStart     ) {IO_SetY(yETC_PstEqpStart     , true ); Sleep(1000); IO_SetY(yETC_PstEqpStart     , false ); }
    else if(iPstSendMsg == psStop      ) {IO_SetY(yETC_PstEqpStop      , true ); Sleep(1000); IO_SetY(yETC_PstEqpStop      , false ); }
    else if(iPstSendMsg == psReset     ) {IO_SetY(yETC_PstEqpReset     , true ); Sleep(1000); IO_SetY(yETC_PstEqpReset     , false ); }
    else if(iPstSendMsg == psLotStart  ) {IO_SetY(yETC_PstEqpLotStart  , true ); Sleep(1000); IO_SetY(yETC_PstEqpLotStart  , false ); }
  //else if(iPstSendMsg == psLotEnd    ) {IO_SetY(yETC_PstEqpLotEnd    , true ); Sleep(1000); IO_SetY(yETC_PstEqpLotEnd    , false ); }
  //else if(iPstSendMsg == psData      ) {IO_SetY(yETC_PstEqpSending   , true ); Sleep(1000); IO_SetY(yETC_PstEqpData      , false ); }
    else if(iPstSendMsg == psJobChange ) {IO_SetY(yETC_PstEqpJobChange , true ); Sleep(1000); IO_SetY(yETC_PstEqpJobChange , false ); }
    else if(iPstSendMsg == psSpare1    ) {IO_SetY(yETC_PstEqpSpare1    , true ); Sleep(1000); IO_SetY(yETC_PstEqpSpare1    , false ); }

}


CEqpComUnit::CEqpComUnit(void)
{
    m_bPstEqpReady = false ;
}

CEqpComUnit::~CEqpComUnit(void)
{

}

bool CEqpComUnit::GetPstEqpReady()
{
    return m_bPstEqpReady ;

}
bool CEqpComUnit::GetPreEqpLotEnd()
{
    return m_bPreEqpLotEnd ;
}

void CEqpComUnit::SetPstEqpLotEnd(bool _bOn)
{
    IO_SetY(yETC_PstEqpLotEnd , _bOn );

}

void CEqpComUnit::SetPstEqpSending(bool _bOn)
{
    IO_SetY(yETC_PstEqpSending , _bOn );

    

}

void CEqpComUnit::SetPreEqpReady(bool _bOn)
{
    //IO_SetY(yETC_PreEqpReady , _bOn );
}

void CEqpComUnit::Update()
{
    SetPstEqpLotEnd(!LT.m_bLotOpen) ;

    bool bPreEqpStart     = false ; //IO_GetXUp(xETC_PreEqpStart    ) ;
    bool bPreEqpStop      = false ; //IO_GetXUp(xETC_PreEqpStop     ) ;
    bool bPreEqpReset     = false ; //IO_GetXUp(xETC_PreEqpReset    ) ;
    bool bPreEqpLotStart  = false ; //IO_GetXUp(xETC_PreEqpLotStart ) ;
    bool bPreEqpLotEnd    = false ; //IO_GetX  (xETC_PreEqpLotEnd   ) ;
    bool bPreEqpData      = false ; //IO_GetXUp(xETC_PreEqpData     ) ;
    bool bPreEqpJobChange = false ; //IO_GetXUp(xETC_PreEqpJobChange) ;
    bool bPreEqpSpare1    = false ; //IO_GetXUp(xETC_PreEqpSpare    ) ;

    if(bPreEqpStart     ) { TRACE("Rcv PreEqp","Start    "); SEQ._bBtnStart = true ;          }
    if(bPreEqpStop      ) { TRACE("Rcv PreEqp","Stop     "); SEQ._bBtnStop  = true ;          }
    if(bPreEqpReset     ) { TRACE("Rcv PreEqp","Reset    "); SEQ._bBtnReset = true ;          }
    if(bPreEqpLotStart  ) { TRACE("Rcv PreEqp","LotStart "); ReadLotData();                   }
    m_bPreEqpLotEnd = bPreEqpLotEnd ;
    if(bPreEqpData      ) { TRACE("Rcv PreEqp","Data     ");                                  }
    if(bPreEqpJobChange ) { TRACE("Rcv PreEqp","JobChange"); ReadChange();                    }
    if(bPreEqpSpare1    ) { TRACE("Rcv PreEqp","Spare1   ");                                  }

    bool bPstEqpStart  = IO_GetXUp(xETC_PstEqpStart ) && OM.CmnOptn.bAutoOperation;
    bool bPstEqpStop   = IO_GetXUp(xETC_PstEqpStop  ) && OM.CmnOptn.bAutoOperation;
    bool bPstEqpReset  = IO_GetXUp(xETC_PstEqpReset ) && OM.CmnOptn.bAutoOperation;
    bool bPstEqpReady  = IO_GetX  (xETC_PstEqpReady ) ;
    bool bPstEqpSpare1 = IO_GetXUp(xETC_PstEqpSpare1) ;
    bool bPstEqpSpare2 = IO_GetXUp(xETC_PstEqpSpare2) ;
    bool bPstEqpSpare3 = IO_GetXUp(xETC_PstEqpSpare3) ;
    bool bPstEqpSpare4 = IO_GetXUp(xETC_PstEqpSpare4) ;

    if(bPstEqpStart     ) { TRACE("Rcv PstEqp","Start "); SEQ._bBtnStart = true ;         }
    if(bPstEqpStop      ) { TRACE("Rcv PstEqp","Stop  "); SEQ._bBtnStop  = true ;         }
    if(bPstEqpReset     ) { TRACE("Rcv PstEqp","Reset "); SEQ._bBtnReset = true ;         }
                            m_bPstEqpReady = bPstEqpReady ; 
    if(bPstEqpSpare1    ) { TRACE("Rcv PstEqp","Spare1");                                 }
    if(bPstEqpSpare2    ) { TRACE("Rcv PstEqp","Spare2");                                 }
    if(bPstEqpSpare3    ) { TRACE("Rcv PstEqp","Spare3");                                 }
    if(bPstEqpSpare4    ) { TRACE("Rcv PstEqp","Spare4");                                 }
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



void CEqpComUnit::SendPreEqp(EN_PRE_EQP_SEND _iMsg)
{
    iPreSendMsg = _iMsg ;

         if(_iMsg == rsStart  ) {TRACE("Smd PreEqp","Start ");}
    else if(_iMsg == rsStop   ) {TRACE("Smd PreEqp","Stop  ");}
    else if(_iMsg == rsReset  ) {TRACE("Smd PreEqp","Reset ");}
    else if(_iMsg == rsReady  ) {TRACE("Smd PreEqp","Ready ");}
    else if(_iMsg == rsSpare1 ) {TRACE("Smd PreEqp","Spare1");}
    else if(_iMsg == rsSpare2 ) {TRACE("Smd PreEqp","Spare2");}
    else if(_iMsg == rsSpare3 ) {TRACE("Smd PreEqp","Spare3");}
    else if(_iMsg == rsSpare4 ) {TRACE("Smd PreEqp","Spare4");}
    else if(_iMsg == ::rsNone ) {TRACE("Smd PreEqp","None  ");}

    CRunThread<Func> * func = new CRunThread<Func>(&ThreadPreEqpMsg) ;

}

void CEqpComUnit::SendPstEqp(EN_PST_EQP_SEND _iMsg)
{
    iPstSendMsg = _iMsg ;


         if(_iMsg == psStart     ) {TRACE("Smd PstEqp","Start    ");                       }
    else if(_iMsg == psStop      ) {TRACE("Smd PstEqp","Stop     ");                       }
    else if(_iMsg == psReset     ) {TRACE("Smd PstEqp","Reset    ");                       }
    else if(_iMsg == psLotStart  ) {TRACE("Smd PstEqp","LotStart "); WriteLotData();       }
    else if(_iMsg == psLotEnd    ) {TRACE("Smd PstEqp","LotEnd   ");                       }
    else if(_iMsg == psSending   ) {TRACE("Smd PstEqp","Sending  "); WriteArayData(riPS2); }
    else if(_iMsg == psJobChange ) {TRACE("Smd PstEqp","JobChange"); WriteChange  (     ); }
    else if(_iMsg == psSpare1    ) {TRACE("Smd PstEqp","Spare1   ");                       }
    else if(_iMsg == ::psNone    ) {TRACE("Smd PstEqp","None     ");                       }


    CRunThread<Func> * func = new CRunThread<Func>(&ThreadPstEqpMsg) ;
}

bool __fastcall CEqpComUnit::WriteArayData(EN_ARAY_ID riId) //다음장비 네트워크 드라이브에 스트립 데이터를 쓴다.
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;
    AnsiString sTemp2 ;
    AnsiString sRslt  ;

    //Set Dir.
    sPath = "\\\\" + OM.MstOptn.sPstEquipPath + "\\Data.ini";

    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        sRslt = "" ;
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            //sRslt += IntToHex(((int)DM.ARAY[riId].GetStat(r,c)) , 2)  ;
            sTemp2 = (int)DM.ARAY[riId].GetStat(r,c) ;
            sTemp2 = sTemp2.sprintf("%02d",(int)DM.ARAY[riId].GetStat(r,c)) + "_";
            sRslt += sTemp2 ;
        }
        sTemp.printf("R%02d", r);
        UserINI.Save(sPath.c_str()  , "Data"  , sTemp , sRslt );
    }

    UserINI.Save(sPath.c_str()  , "ETC"  , "ID"    , DM.ARAY[riId].GetID().ToIntDef(9999) );
    UserINI.Save(sPath.c_str()  , "ETC"  , "LotNo" , DM.ARAY[riId].GetLotNo()             );


    return true ;
}

bool __fastcall CEqpComUnit::ReadArayData (EN_ARAY_ID riId) //전 장비가 써 놓은 스트립 데이터 읽기.
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;
    AnsiString sVal   ;
    AnsiString sRslt  ;
    int iId,iLotNo;

    //Set Dir.
    sPath = "\\\\" + OM.MstOptn.sPreEquipPath + "\\Data.ini" ;


    if(!FileExists(sPath)) return false ;

    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        sTemp.printf("R%02d", r);
        UserINI.Load(sPath.c_str()  , "Data"  , sTemp , sRslt );
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            //sVal = "0x"+sRslt.SubString(c+1,1) ;
            sVal = sRslt.SubString(c*3+1,2) ;
            sRslt.Delete(c+1,3) ;
            DM.ARAY[riId].SetStat(r,c,(EN_CHIP_STAT)StrToInt(sVal)  );
        }
    }

    UserINI.Load(sPath.c_str()  , "ETC"  , "ID"    , &iId   );
    UserINI.Load(sPath.c_str()  , "ETC"  , "LotNo" , &iLotNo);

    DM.ARAY[riId].SetID   (iId   );
    DM.ARAY[riId].SetLotNo(iLotNo);

    return true ;
}



bool __fastcall CEqpComUnit::ReadLotData (void) //LotOpen시에 씀.
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = "\\\\" + OM.MstOptn.sPreEquipPath + "\\LotData.ini" ;

    if(!FileExists(sPath)) return false ;

    //Load Device
    //for(int i = 0 ; i < MAX_LOT_CNT ; i++) {
    UserINI.Load(sPath.c_str()  , "LotInfo"  , "sLotNo   " , LT.LotInfo.sLotNo   );
    //}
    UserINI.Load(sPath.c_str()  , "LotInfo"  , "sJobFile " , LT.LotInfo.sJobFile );
    UserINI.Load(sPath.c_str()  , "LotInfo"  , "sOperator" , LT.LotInfo.sOperator);

    return true ;
}

bool __fastcall CEqpComUnit::WriteLotData(void)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = "\\\\" + OM.MstOptn.sPstEquipPath + "\\LotData.ini";

    UserINI.ClearFile(sPath) ;

    //Load Device.
    //for(int i = 0 ; i < MAX_LOT_CNT ; i++) {
    UserINI.Save(sPath.c_str()  , "LotInfo"  , "sLotNo   " , LT.LotInfo.sLotNo   );
    //}
    UserINI.Save(sPath.c_str()  , "LotInfo"  , "sJobFile " , LT.LotInfo.sJobFile );
    UserINI.Save(sPath.c_str()  , "LotInfo"  , "sOperator" , LT.LotInfo.sOperator);

    return true ;
}

bool __fastcall CEqpComUnit::WriteChange (void)  //Job Change시에 씀.
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath ,sCrntDev ;

    //Set Dir.
    sPath = "\\\\" + OM.MstOptn.sPstEquipPath + "\\DeviceChange.ini";

    UserINI.ClearFile(sPath) ;

    sCrntDev = OM.GetCrntDev();
    //Load Device.
    UserINI.Save(sPath.c_str()  , "DeviceName"  , "sCrntDev" , sCrntDev );

    return true ;
}

bool __fastcall CEqpComUnit::ReadChange  (void)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath,sCrntDev;

    //Set Dir.
    sPath = "\\\\" + OM.MstOptn.sPstEquipPath + "\\DeviceChange.ini";

    //Load Device.
    UserINI.Load(sPath.c_str()  , "DeviceName"  , "sCrntDev" , sCrntDev );

    //Set Dir.
    sPath = OM.MstOptn.sPreEquipPath + "\\DeviceChange.ini";

    //eiETC_PreEqpNoJob
    if(!FileExists(sPath)) return false ;

    OM.LoadJobFile (sCrntDev);
    DM.ARAY[riLD1].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);
    DM.ARAY[riULD].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);
    DM.ARAY[riPR1].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riPR2].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riWRP].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riWRK].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riPS1].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riPS2].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riTmp].SetMaxColRow(OM.DevOptn.iContColCnt , OM.DevInfo.iRowCnt);
    DM.ClearMap    (        );
    OM.SaveLastInfo(        );
    PM.Load        (sCrntDev);

    return true ;
}






















