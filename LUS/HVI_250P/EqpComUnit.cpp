//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "EqpComUnit.h"
#include "UserFile.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "UserIni.h"
#include "LotUnit.h"
#include "SLogUnit.h"
#include "Sequence.h"
#include "PstnMan.h"
#include "VisnComUnit.h"
#include "PreBuff.h"

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

         if(iPreSendMsg == ::rsNone    ) {                                                                                      }
    else if(iPreSendMsg == rsStart     ) {IO_SetY(yETC_PreEqpStart  , true ); Sleep(1000); IO_SetY(yETC_PreEqpStart  , false ); }
    else if(iPreSendMsg == rsStop      ) {IO_SetY(yETC_PreEqpStop   , true ); Sleep(1000); IO_SetY(yETC_PreEqpStop   , false ); }
    else if(iPreSendMsg == rsReset     ) {IO_SetY(yETC_PreEqpReset  , true ); Sleep(1000); IO_SetY(yETC_PreEqpReset  , false ); }
  //else if(iPreSendMsg == rsReady     ) {IO_SetY(yETC_PreEqpReady  , true ); Sleep(1000); IO_SetY(yETC_PreEqpReady  , false ); } 요것은 함수가 따로 있음.
//    else if(iPreSendMsg == rsSpare1    ) {IO_SetY(yETC_PreEqpSpare1 , true ); Sleep(1000); IO_SetY(yETC_PreEqpSpare1 , false ); }
//    else if(iPreSendMsg == rsSpare2    ) {IO_SetY(yETC_PreEqpSpare2 , true ); Sleep(1000); IO_SetY(yETC_PreEqpSpare2 , false ); }
//    else if(iPreSendMsg == rsSpare3    ) {IO_SetY(yETC_PreEqpSpare4 , true ); Sleep(1000); IO_SetY(yETC_PreEqpSpare4 , false ); }
//    else if(iPreSendMsg == rsSpare4    ) {IO_SetY(yETC_PreEqpSpare4 , true ); Sleep(1000); IO_SetY(yETC_PreEqpSpare4 , false ); }

}

void CEqpComUnit::ThreadPstEqpMsg()
{
/*
         if(iPstSendMsg == ::psNone    ) {                                                                                            }
    else if(iPstSendMsg == psStart     ) {IO_SetY(yETC_PstEqpStart     , true ); Sleep(1000); IO_SetY(yETC_PstEqpStart     , false ); }
    else if(iPstSendMsg == psStop      ) {IO_SetY(yETC_PstEqpStop      , true ); Sleep(1000); IO_SetY(yETC_PstEqpStop      , false ); }
    else if(iPstSendMsg == psReset     ) {IO_SetY(yETC_PstEqpReset     , true ); Sleep(1000); IO_SetY(yETC_PstEqpReset     , false ); }
    else if(iPstSendMsg == psLotStart  ) {IO_SetY(yETC_PstEqpLotStart  , true ); Sleep(1000); IO_SetY(yETC_PstEqpLotStart  , false ); }
  //else if(iPstSendMsg == psLotEnd    ) {IO_SetY(yETC_PstEqpLotEnd    , true ); Sleep(1000); IO_SetY(yETC_PstEqpLotEnd    , false ); }
    else if(iPstSendMsg == psData      ) {IO_SetY(yETC_PstEqpData      , true ); Sleep(1000); IO_SetY(yETC_PstEqpData      , false ); }
    else if(iPstSendMsg == psJobChange ) {IO_SetY(yETC_PstEqpJobChange , true ); Sleep(1000); IO_SetY(yETC_PstEqpJobChange , false ); }
    else if(iPstSendMsg == psSpare1    ) {IO_SetY(yETC_PstEqpSpare1    , true ); Sleep(1000); IO_SetY(yETC_PstEqpSpare1    , false ); }
*/
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
bool CEqpComUnit::GetPreEqpSending()
{
    return m_bPreEqpSending ;
}

void CEqpComUnit::SetPstEqpLotEnd(bool _bOn)
{
//    IO_SetY(yETC_PstEqpLotEnd , _bOn );

}

void CEqpComUnit::SetPreEqpReady(bool _bOn)
{
    IO_SetY(yETC_PreEqpReady , _bOn );

}

void CEqpComUnit::Update()
{
    SetPstEqpLotEnd(!LT.m_bLotOpen) ;

    bool b1 = DM.ARAY[riPRB].CheckAllStat(csNone) && DM.ARAY[riINB].CheckAllStat(csNone);
    bool b2 = !IO_GetX(xPRB_Strip ) ;
    bool b3 = !IO_GetX(xPRB_Strip2) ;
    bool b4 = !IO_GetX(xPRI_Strip3) ;
    bool b5 =  MT_CmprPos(miPRB_TFlp,PM.GetValue(miPRB_TFlp,pvPRB_TFlpNormal)) ;
    bool b6 =  AT_Complete(aiPRB_Idx,ccBwd) ;
    bool b7 = SEQ._iStep == SEQ.scRun ;
    bool b8 = PRB.GetSeqStep() == CPreBuff::scIdle ;
    bool b9 = !SEQ._bReqStop ;

    bool bSet = b1 && b2 && b3 && b4 && b5 && b6 && b7 && b8 && b9;
    SetPreEqpReady(bSet);

    static bool bPreSet = bSet ;

    if(bPreSet != bSet) {
        if(bSet) {Trace("PreEqp","Ready to Pre ON!! "); }
        else     {Trace("PreEqp","Ready to Pre OFF!!"); }
    }

    bPreSet = bSet ;

    bool bPreEqpStart     = IO_GetXUp(xETC_PreEqpStart    ) && !OM.MstOptn.bDebugMode;
    bool bPreEqpStop      = IO_GetXUp(xETC_PreEqpStop     ) && !OM.MstOptn.bDebugMode;
    bool bPreEqpReset     = IO_GetXUp(xETC_PreEqpReset    ) && !OM.MstOptn.bDebugMode;
    bool bPreEqpLotStart  = IO_GetXUp(xETC_PreEqpLotStart ) ;
    bool bPreEqpLotEnd    = IO_GetX  (xETC_PreEqpLotEnd   ) ;
    bool bPreEqpSending   = IO_GetX  (xETC_PreEqpSending  ) ;
    bool bPreEqpJobChange = IO_GetXUp(xETC_PreEqpJobChange) ;
//    bool bPreEqpSpare1    = IO_GetXUp(xETC_PreEqpSpare    ) ;

    if(bPreEqpStart     ) { Trace("Rcv PreEqp","Start    "); if(OM.CmnOptn.bAutoOperation)  SEQ._bBtnStart = true ;          }
    if(bPreEqpStop      ) { Trace("Rcv PreEqp","Stop     "); if(OM.CmnOptn.bAutoOperation)  SEQ._bBtnStop  = true ;          }
    if(bPreEqpReset     ) { Trace("Rcv PreEqp","Reset    "); if(OM.CmnOptn.bAutoOperation)  SEQ._bBtnReset = true ;          }
    if(bPreEqpLotStart  ) { Trace("Rcv PreEqp","LotStart ");                                ReadLotData();                   }
    if(bPreEqpLotEnd    ) m_bEqpLotEnd = true ; //Lot End 신호를 받았다는 것을 표시하기 위해 JS
    else                  m_bEqpLotEnd = false; //?? dd
    m_bPreEqpLotEnd = bPreEqpLotEnd  ;
    m_bPreEqpSending= bPreEqpSending ;

    //if(bPreEqpData      ) { Trace("Rcv PreEqp","ArayData ");                                                                 }
    if(bPreEqpJobChange ) { Trace("Rcv PreEqp","JobChange"); if(OM.CmnOptn.bAutoConvesion)  ReadChange();                    }
//    if(bPreEqpSpare1    ) { Trace("Rcv PreEqp","Spare1   ");                                                                 }


    bool bPstEqpStart  = false ;//&& !OM.MstOptn.bDebugMode; //= IO_GetXUp(xETC_PstEqpStart ) ;
    bool bPstEqpStop   = false ;//&& !OM.MstOptn.bDebugMode; //= IO_GetXUp(xETC_PstEqpStop  ) ;
    bool bPstEqpReset  = false ;//&& !OM.MstOptn.bDebugMode; //= IO_GetXUp(xETC_PstEqpReset ) ;
    bool bPstEqpReady  = false ; //= IO_GetX  (xETC_PstEqpReady ) ;
    bool bPstEqpSpare1 = false ; //= IO_GetXUp(xETC_PstEqpSpare1) ;
    bool bPstEqpSpare2 = false ; //= IO_GetXUp(xETC_PstEqpSpare2) ;
    bool bPstEqpSpare3 = false ; //= IO_GetXUp(xETC_PstEqpSpare3) ;
    bool bPstEqpSpare4 = false ; //= IO_GetXUp(xETC_PstEqpSpare4) ;

    if(bPstEqpStart  ) { Trace("Rcv PstEqp","Start    "); if(OM.CmnOptn.bAutoOperation)SEQ._bBtnStart = true ;         }
    if(bPstEqpStop   ) { Trace("Rcv PstEqp","Stop     "); if(OM.CmnOptn.bAutoOperation)SEQ._bBtnStop  = true ;         }
    if(bPstEqpReset  ) { Trace("Rcv PstEqp","Reset    "); if(OM.CmnOptn.bAutoOperation)SEQ._bBtnReset = true ;         }
                         m_bPstEqpReady = bPstEqpReady ;
    if(bPstEqpSpare1 ) { Trace("Rcv PstEqp","Spare1   ");                                                              }
    if(bPstEqpSpare2 ) { Trace("Rcv PstEqp","Spare2   ");                                                              }
    if(bPstEqpSpare3 ) { Trace("Rcv PstEqp","Spare3   ");                                                              }
    if(bPstEqpSpare4 ) { Trace("Rcv PstEqp","Spare4   ");                                                              }
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

         if(_iMsg == rsStart  ) {if(!OM.CmnOptn.bAutoOperation)return ;    }
    else if(_iMsg == rsStop   ) {                                          }//if(!OM.CmnOptn.bAutoOperation)return ;    }
    else if(_iMsg == rsReset  ) {if(!OM.CmnOptn.bAutoOperation)return ;    }

         if(_iMsg == rsStart  ) {if(OM.CmnOptn.bAutoOperation)Trace("Snd PreEqp","Start  ");}
    else if(_iMsg == rsStop   ) {                             Trace("Snd PreEqp","Stop   ");}//if(OM.CmnOptn.bAutoOperation)Trace("Snd PreEqp","Stop   ");}
    else if(_iMsg == rsReset  ) {if(OM.CmnOptn.bAutoOperation)Trace("Snd PreEqp","Reset  ");}
    else if(_iMsg == rsReady  ) {                             Trace("Snd PreEqp","Ready  ");}
    else if(_iMsg == rsSpare1 ) {                             Trace("Snd PreEqp","Spare1 ");}
    else if(_iMsg == rsSpare2 ) {                             Trace("Snd PreEqp","Spare2 ");}
    else if(_iMsg == rsSpare3 ) {                             Trace("Snd PreEqp","Spare3 ");}
    else if(_iMsg == rsSpare4 ) {                             Trace("Snd PreEqp","Spare4 ");}
    else if(_iMsg == ::rsNone ) {                             Trace("Snd PreEqp","None   ");}

    CRunThread<Func> * func = new CRunThread<Func>(&ThreadPreEqpMsg) ;
}

void CEqpComUnit::SendPstEqp(EN_PST_EQP_SEND _iMsg)
{
    iPstSendMsg = _iMsg ;

    //if(!OM.CmnOptn.bAutoOperation) return ;

         if(_iMsg == psStart     ) {if(!OM.CmnOptn.bAutoOperation)return ;    }
    else if(_iMsg == psStop      ) {if(!OM.CmnOptn.bAutoOperation)return ;    }
    else if(_iMsg == psReset     ) {if(!OM.CmnOptn.bAutoOperation)return ;    }


         if(_iMsg == psStart     ) {if(OM.CmnOptn.bAutoOperation)Trace("Snd PstEqp","Start    ");                       }
    else if(_iMsg == psStop      ) {if(OM.CmnOptn.bAutoOperation)Trace("Snd PstEqp","Stop     ");                       }
    else if(_iMsg == psReset     ) {if(OM.CmnOptn.bAutoOperation)Trace("Snd PstEqp","Reset    ");                       }
    else if(_iMsg == psLotStart  ) {                             Trace("Snd PstEqp","LotStart "); WriteLotData();       }
    else if(_iMsg == psLotEnd    ) {                             Trace("Snd PstEqp","LotEnd   ");                       }
    else if(_iMsg == psData      ) {                             Trace("Snd PstEqp","ArayData ");                       } //WriteArayData(riPSB); }
    else if(_iMsg == psJobChange ) {                             Trace("Snd PstEqp","JobChange"); WriteChange  (     ); }
    else if(_iMsg == psSpare1    ) {                             Trace("Snd PstEqp","Spare1   ");                       }
    else if(_iMsg == ::psNone    ) {                             Trace("Snd PstEqp","None     ");                       }


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
    sPath = OM.MstOptn.sPstEquipPath + "\\Data.ini";

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

    UserINI.Save(sPath.c_str()  , "ETC"  , "ID     " , DM.ARAY[riId].GetID().ToIntDef(9999) );
    UserINI.Save(sPath.c_str()  , "ETC"  , "LotNo  " , DM.ARAY[riId].GetLotNo()             );
    UserINI.Save(sPath.c_str()  , "ETC"  , "iRowCnt" , OM.DevInfo.iRowCnt                   );
    UserINI.Save(sPath.c_str()  , "ETC"  , "iColCnt" , OM.DevInfo.iColCnt                   );

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

    AnsiString sId    ;
    AnsiString sLotNo ;

    int iRow = 0 ;
    int iCol = 0 ;

    AnsiString sTemp2= "" ;

    //Set Dir.
    sPath = OM.MstOptn.sPreEquipPath + "\\Data.ini" ;

    if(!FileExists(sPath)) return false ;

    UserINI.Load(sPath.c_str()  , "ETC"  , "iRowCnt" , &iRow );
    UserINI.Load(sPath.c_str()  , "ETC"  , "iColCnt" , &iCol );

    if( iRow != OM.DevInfo.iRowCnt || iCol != OM.DevInfo.iColCnt ) return false ;

    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        sTemp.printf("R%02d", r);
        UserINI.Load(sPath.c_str()  , "Data"  , sTemp , sRslt );
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            //sVal = "0x"+sRslt.SubString(c+1,1) ;
            sVal = sRslt.SubString(1,2) ;
            sRslt.Delete(1,3) ;
            DM.ARAY[riId].SetStat(r,c,(EN_CHIP_STAT)StrToInt(sVal)  );
            sTemp2 = sVal+"," ;
        }
    }

    UserINI.Load(sPath.c_str()  , "ETC"  , "ID"    , &sId   );
    UserINI.Load(sPath.c_str()  , "ETC"  , "LotNo" , &sLotNo);

    DM.ARAY[riId].SetID   (sId   );
    DM.ARAY[riId].SetLotNo(sLotNo);

    UserFile.DeleteFiles(sPath);

    Trace("Rcv Array",sTemp2.c_str()) ;

    return true ;
}

bool __fastcall CEqpComUnit::ReadArayData (CArray * Aray)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;
    AnsiString sVal   ;
    AnsiString sRslt  ;

    AnsiString sId    ;
    AnsiString sLotNo ;

    int iRow = 0 ;
    int iCol = 0 ;

    AnsiString sTemp2= "" ;

    //Set Dir.
    sPath = OM.MstOptn.sPreEquipPath + "\\Data.ini" ;

    if(!FileExists(sPath)) return false ;

    UserINI.Load(sPath.c_str()  , "ETC"  , "iRowCnt" , &iRow );
    UserINI.Load(sPath.c_str()  , "ETC"  , "iColCnt" , &iCol );

    if( iRow != OM.DevInfo.iRowCnt || iCol != OM.DevInfo.iColCnt ) return false ;

    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        sTemp.printf("R%02d", r);
        UserINI.Load(sPath.c_str()  , "Data"  , sTemp , sRslt );
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            //sVal = "0x"+sRslt.SubString(c+1,1) ;
            sVal = sRslt.SubString(1,2) ;
            sRslt.Delete(1,3) ;
            Aray->SetStat(r,c,(EN_CHIP_STAT)StrToInt(sVal)  );
            sTemp2 = sVal+"," ;
        }
    }

    UserINI.Load(sPath.c_str()  , "ETC"  , "ID"    , &sId   );
    UserINI.Load(sPath.c_str()  , "ETC"  , "LotNo" , &sLotNo);

    Aray->SetID   (sId   );
    Aray->SetLotNo(sLotNo);

//    UserFile.DeleteFiles(sPath);

    Trace("Rcv Array",sTemp2.c_str()) ;

    return true ;
}



bool __fastcall CEqpComUnit::ReadLotData (void) //LotOpen시에 씀.
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = OM.MstOptn.sPreEquipPath + "\\LotData.ini" ;

    if(!FileExists(sPath)) return false ;

    //Load Device
    // for(int i = 0 ; i < MAX_LOT_CNT ; i++) {
    UserINI.Load(sPath.c_str()  , "LotInfo"  , "sLotNo   " , EndCheckLot.sLotNo);
    //}
    UserINI.Load(sPath.c_str()  , "LotInfo"  , "sJobFile " , EndCheckLot.sJobFile );
    UserINI.Load(sPath.c_str()  , "LotInfo"  , "sOperator" , EndCheckLot.sOperator);

    //Load Device
    // for(int i = 0 ; i < MAX_LOT_CNT ; i++) {
    //UserINI.Load(sPath.c_str()  , "LotInfo"  , "sLotNo   " , LT.LotInfo.sLotNo );
    //}
    //UserINI.Load(sPath.c_str()  , "LotInfo"  , "sJobFile " , LT.LotInfo.sJobFile );
    //UserINI.Load(sPath.c_str()  , "LotInfo"  , "sOperator" , LT.LotInfo.sOperator);

    return true ;
}

bool __fastcall CEqpComUnit::WriteLotData(void)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    AnsiString sLotNo   ;
    AnsiString sJobFile ;
    AnsiString sOperator ;



    //Set Dir.
    sPath = OM.MstOptn.sPstEquipPath + "\\LotData.ini";

    UserINI.ClearFile(sPath) ;

    //Load Device.
    UserINI.Save(sPath.c_str()  , "LotInfo"  , "sLotNo"    , sLotNo   );
    UserINI.Save(sPath.c_str()  , "LotInfo"  , "sJobFile " , sJobFile );
    UserINI.Save(sPath.c_str()  , "LotInfo"  , "sOperator" , sOperator);

    if(!LT.m_bLotOpen) LT.LotOpen(sLotNo , sJobFile , sOperator) ;

    return true ;
}

bool __fastcall CEqpComUnit::WriteChange (void)  //Job Change시에 씀.
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

bool __fastcall CEqpComUnit::ReadChange  (void)
{
    if(LT.m_bLotOpen) {
        FM_MsgOk("Error" , "Can't Change JobFile While LotOpen");
        return false;
    }

    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath,sCrntDev;

    //Set Dir.
    sPath = OM.MstOptn.sPreEquipPath + "\\DeviceChange.ini";

    //Load Device.
    UserINI.Load(sPath.c_str()  , "DeviceName"  , "sCrntDev" , sCrntDev );

    //eiETC_PreEqpNoJob
    if(!FileExists(sPath)) return false ;

    OM.LoadJobFile (sCrntDev);
    DM.ARAY[riINB].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt);
    DM.ARAY[riPRB].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt);
    DM.ARAY[riPRI].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt);
    DM.ARAY[riPSI].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt);

    DM.ClearMap    (      );
    OM.SaveLastInfo(      );
    PM.Load        (sCrntDev);

//    if(OM.CmnOptn.bAutoVsChange)VC.SendModelChange();

    return true ;
}






















