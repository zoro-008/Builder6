//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include <stdio.h>

#include "LotUnit.h"
#include "SLogUnit.h"
#include "UserIni.h"
#include "UserFile.h"
#include "OptionMan.h"
#include "Timer.h"
#include "DataMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

AnsiString g_sLotNo[MAX_LOT_CNT] ;
void PushLotNo (AnsiString _sLotNo)
{
   for(int i = 0 ; i < MAX_LOT_CNT ; i++) {
       if(g_sLotNo[i] == "") {
           g_sLotNo[i] = _sLotNo ;
           return;
       }
   }
}
//Add Push Top
//TODO :: for 문에서 뻑남 플레이스 시에.
void PushTopLotNo (AnsiString _sLotNo)
{
    for(int i = MAX_LOT_CNT-1  ; i > 0 ; i--) {
        g_sLotNo[i] = g_sLotNo[i-1] ;
    }
    g_sLotNo[0] = _sLotNo ;
    return ;
}

AnsiString GetLotNo (void ) //참조만.
{
    return g_sLotNo[0] ;
}

AnsiString PopLotNo (void )
{
   AnsiString sLotNo = g_sLotNo[0] ;
   for(int i = 0 ; i < MAX_LOT_CNT - 1 ; i++) {
       g_sLotNo[i] = g_sLotNo[i+1] ;
   }
   return sLotNo ;
}

void DeleteLotNo(AnsiString _sLotNo)
{
   for(int i = 0 ; i < MAX_LOT_CNT ; i++) {
       if(_sLotNo == g_sLotNo[i]){
           g_sLotNo[i] = "" ;
           for(int j = i ; j < MAX_LOT_CNT - 1; i++) {
               g_sLotNo[j] = g_sLotNo[j+1] ;
           }
       }
   }
}

void DeleteLotNo(int _iLotNo)
{
   for(int i = _iLotNo ; i < MAX_LOT_CNT - 1 ; i++) {
       g_sLotNo[i] = g_sLotNo[i+1] ;
   }
}

void DeleteLotNoOne()
{
    for(int i = MAX_LOT_CNT-1  ; i >= 0 ; i--) {
        if(g_sLotNo[i] != "") {
            g_sLotNo[i] = "" ;
            break ;
        }
    }
}

void DeleteLotNoAll (void )
{
   for(int i = 0 ; i < MAX_LOT_CNT ; i++) {
       g_sLotNo[i] = "";
   }
}

int GetLotCnt(void)
{
   int i  ;
   for(i = 0 ; i < MAX_LOT_CNT ; i++) {
       if(g_sLotNo[i] == "") break ;
   }

   return i ;
}

CLot LT;

__fastcall CLot::CLot(void)
{
    //Init. Flag.
    m_bLotOpen  = false;

    //Init. Buffer.
    ClearEqpInfo();
    ClearLotInfo();
    ClearDayInfo();


    LoadEqpInfo();
    LoadLotInfo();
    LoadDayInfo();

    if(!FileExists(LOG_FOLDER)) CreateDir(LOG_FOLDER);


}
__fastcall CLot::~CLot(void)
{
    SaveEqpInfo();
    SaveLotInfo();
    SaveDayInfo();
//    Trace("","");
}
void __fastcall CLot::ClearEqpInfo(void)
{
    memset(&EqpInfo , 0 , sizeof(SEqpInfo));
}

void __fastcall CLot::ClearLotInfo(void)
{
    memset(&LotInfo , 0 , sizeof(SLotInfo));

}
void __fastcall CLot::ClearDayInfo(void)
{
    memset(&DayInfo , 0 , sizeof(SDayInfo));
}

//Lot Processing.
void __fastcall CLot::LotOpen (AnsiString _sLotNo , AnsiString _sJobFile , AnsiString _sOperator)
{
    AnsiString sTempJobFile  = LotInfo.sJobFile  ;
    AnsiString sTempOperator = LotInfo.sOperator ;
    ClearLotInfo();

    LotInfo.sWaferName     = _sLotNo    ;
    if(_sJobFile == "") LotInfo.sJobFile   = sTempJobFile  ;
    else                LotInfo.sJobFile   = _sJobFile     ;
    if(_sOperator== "") LotInfo.sOperator  = sTempOperator ;
    else                LotInfo.sOperator  = _sOperator    ;

    LotInfo.iCrntLotNo = 0 ;

    LotInfo.dStartTime = Now();
    Trace("LotOpen",_sLotNo.c_str());

    //for(int i = 0 ; i < MAX_ARAY ; i++) {
    //    DM.ARAY[i].SetStat(csEmpty) ;
    //}

    //Set Lot Open Flag.
    m_bLotOpen  = true ;
}

AnsiString __fastcall CLot::GetCrntLot ()
{
    return LotInfo.sWaferName ;

}


void __fastcall CLot::LotEnd (void)
{
    //Check already opened Lot.
    if (!m_bLotOpen) return;

    //Set EndTime.
    LotInfo.dEndTime = Now();
    WriteLotLog   ();
    WriteLotDayLog();

    //Reset Lot Flag.
    m_bLotOpen = false;

//    LDR._iMgzCnt = 0 ; //안대서 추가 하긴 하는데 이게 여기가 맞는지 모르겟넹.
//    EPX.ClearSlotCnt( );
//    EPX.ClearMgzCnt ( );

    //WriteLotLog();

    //Check Retest Mode.
    Trace("SEQ","Lot Work Finished"   );
    FM_MsgOk("Confirm" , "랏이 종료 되었습니다."   );

    //여기서 하면 부져가 안울림 그래서 리셑에 역음.
    //m_bLotEnd = false ;

}

void __fastcall CLot::DispEqpInfo (TPanel * _pnTotalCnt ,
                                   TPanel * _pnFailCnt  ,
                                   TPanel * _pnGoodCnt  ,
                                   TPanel * _pnYield    )
{
    if(_pnTotalCnt) _pnTotalCnt -> Caption = EqpInfo.iTotalChip     ;
    if(_pnFailCnt ) _pnFailCnt  -> Caption = EqpInfo.iTotalFailChip ;
    if(_pnGoodCnt ) _pnGoodCnt  -> Caption = EqpInfo.iTotalChip - EqpInfo.iTotalFailChip ;

    float dTemp = (EqpInfo.iTotalChip - EqpInfo.iTotalFailChip)* 100/(float)EqpInfo.iTotalChip ;
    dTemp       = ((int)dTemp)/100.0 ;
    if(_pnYield   ) _pnYield    -> Caption = dTemp    ;
}

void __fastcall CLot::DispLotInfo(TPanel * _pnWaferName,
                                  TPanel * _pnJobFile  ,
                                  TPanel * _pnOperator ,
                                  TPanel * _pnStartTime,
                                  TPanel * _pnEndTime  ,
                                  TPanel * _pnRunTime  ,
                                  TPanel * _pnIdleTime ,
                                  TPanel * _pnJamTime  ,
                                  TPanel * _pnTotalTime,
                                  TPanel * _pnStrpUPEH ,
                                  //TPanel * _pnChipUPEH ,
                                  TPanel * _pnStrpUPH  ,
                                  //TPanel * _pnChipUPH  ,
                                  TPanel * _pnWorkStrp ,
                                  TPanel * _pnWorkChip ,
                                  TPanel * _pnFailChip )

{
    TDateTime tTemp ;

    if(_pnWaferName) _pnWaferName -> Caption = LotInfo.sWaferName ;
    if(_pnJobFile  ) _pnJobFile   -> Caption = LotInfo.sJobFile   ;
    if(_pnOperator ) _pnOperator  -> Caption = LotInfo.sOperator  ;
    tTemp.Val = LotInfo.dStartTime ; if(_pnStartTime) _pnStartTime -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = LotInfo.dEndTime   ; if(_pnEndTime  ) _pnEndTime   -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = LotInfo.dRunTime   ; if(_pnRunTime  ) _pnRunTime   -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = LotInfo.dIdleTime  ; if(_pnIdleTime ) _pnIdleTime  -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = LotInfo.dJamTime   ; if(_pnJamTime  ) _pnJamTime   -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = LotInfo.dTotalTime ; if(_pnTotalTime) _pnTotalTime -> Caption = tTemp.FormatString("hh:nn:ss") ;
    if(_pnWorkStrp) _pnWorkStrp -> Caption = LotInfo.iWrkWafer  ;
    if(_pnWorkChip) _pnWorkChip -> Caption = LotInfo.iMarkChip  ;
    if(_pnFailChip) _pnFailChip -> Caption = LotInfo.iFailChip  ;

}

void __fastcall CLot::DispDayInfo(TPanel * _pnRunTime   ,
                                  TPanel * _pnIdleTime  ,
                                  TPanel * _pnJamTime   ,
                                  TPanel * _pnTotalTime ,
                                  TPanel * _pnStrpUPEH  ,
                                  //TPanel * _pnChipUPEH  ,
                                  TPanel * _pnStrpUPH   ,
                                  //TPanel * _pnChipUPH   ,
                                  TPanel * _pnWorkStrp  ,
                                  TPanel * _pnWorkChip  ,
                                  TPanel * _pnFailChip  )
                                  //TPanel * _pnWorkLot   )
{
    TDateTime tTemp ;

    tTemp.Val = DayInfo.dRunTime   ; if(_pnRunTime  ) _pnRunTime   -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = DayInfo.dIdleTime  ; if(_pnIdleTime ) _pnIdleTime  -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = DayInfo.dJamTime   ; if(_pnJamTime  ) _pnJamTime   -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = DayInfo.dTotalTime ; if(_pnTotalTime) _pnTotalTime -> Caption = tTemp.FormatString("hh:nn:ss") ;
    if(_pnWorkStrp) _pnWorkStrp -> Caption = DayInfo.iWrkWafer  ;
    if(_pnWorkChip) _pnWorkChip -> Caption = DayInfo.iMarkChip  ;
    if(_pnFailChip) _pnFailChip -> Caption = DayInfo.iFailChip  ;
}

void __fastcall CLot::LoadEqpInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Path;
    AnsiString TempStr ;
    TDateTime  TempTime;

    //Make Dir.
    Path = EXE_FOLDER + "SeqData\\" + "EqpInfo.ini";

    //Current Lot Informations.
    UserINI.Load(Path.c_str() , "LotInfo" , "iTotalChip    " , EqpInfo.iTotalChip    );
    UserINI.Load(Path.c_str() , "LotInfo" , "iTotalFailChip" , EqpInfo.iTotalFailChip);
}

void __fastcall CLot::SaveEqpInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Path;
    AnsiString TempStr ;
    TDateTime  TempTime;

    //Make Dir.
    Path = EXE_FOLDER + "SeqData\\" + "EqpInfo.ini";

    //Current Lot Informations.
    UserINI.Save(Path.c_str() , "LotInfo" , "iTotalChip    " , EqpInfo.iTotalChip    );
    UserINI.Save(Path.c_str() , "LotInfo" , "iTotalFailChip" , EqpInfo.iTotalFailChip);

}

//File Processing.
void __fastcall CLot::LoadLotInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Path;
    AnsiString TempStr ;
    TDateTime  TempTime;

    //Make Dir.
    Path = EXE_FOLDER + "SeqData\\" + "LotInfo.ini";

    //Current Lot Informations.
    UserINI.Load(Path.c_str() , "Member " , "m_bLotOpen  " , m_bLotOpen          );
    UserINI.Load(Path.c_str() , "LotInfo" , "sWaferName  " , LotInfo.sWaferName  );
    UserINI.Load(Path.c_str() , "LotInfo" , "sJobFile    " , LotInfo.sJobFile    );
    UserINI.Load(Path.c_str() , "LotInfo" , "sOperator   " , LotInfo.sOperator   );
    UserINI.Load(Path.c_str() , "LotInfo" , "dStartTime  " , LotInfo.dStartTime  );
    UserINI.Load(Path.c_str() , "LotInfo" , "dEndTime    " , LotInfo.dEndTime    );
    UserINI.Load(Path.c_str() , "LotInfo" , "dRunTime    " , LotInfo.dRunTime    );
    UserINI.Load(Path.c_str() , "LotInfo" , "dIdleTime   " , LotInfo.dIdleTime   );
    UserINI.Load(Path.c_str() , "LotInfo" , "dJamTime    " , LotInfo.dJamTime    );
    UserINI.Load(Path.c_str() , "LotInfo" , "dTotalTime  " , LotInfo.dTotalTime  );
    //UserINI.Load(Path.c_str() , "LotInfo" , "iStrpUPEH   " , LotInfo.iStrpUPEH   );
    //UserINI.Load(Path.c_str() , "LotInfo" , "iStrpUPH    " , LotInfo.iStrpUPH    );
    UserINI.Load(Path.c_str() , "LotInfo" , "iWrkWafer   " , LotInfo.iWrkWafer   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iMarkChip   " , LotInfo.iMarkChip   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iFailChip   " , LotInfo.iFailChip   );  //SPC 목록
}

void __fastcall CLot::SaveLotInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Path;
    AnsiString TempStr ;
    TDateTime  TempTime;

    //Make Dir.
    Path = EXE_FOLDER + "SeqData\\" + "LotInfo.ini";

    if(LotInfo.sJobFile =="") LotInfo.sJobFile = OM.GetCrntDev() ;  /*sun*/

    //Current Lot Informations.
    UserINI.Save(Path.c_str() , "Member " , "m_bLotOpen  " , m_bLotOpen          );
    UserINI.Save(Path.c_str() , "LotInfo" , "sWaferName  " , LotInfo.sWaferName  );
    UserINI.Save(Path.c_str() , "LotInfo" , "sJobFile    " , LotInfo.sJobFile    );
    UserINI.Save(Path.c_str() , "LotInfo" , "sOperator   " , LotInfo.sOperator   );
    UserINI.Save(Path.c_str() , "LotInfo" , "dStartTime  " , LotInfo.dStartTime  );
    UserINI.Save(Path.c_str() , "LotInfo" , "dEndTime    " , LotInfo.dEndTime    );
    UserINI.Save(Path.c_str() , "LotInfo" , "dRunTime    " , LotInfo.dRunTime    );
    UserINI.Save(Path.c_str() , "LotInfo" , "dIdleTime   " , LotInfo.dIdleTime   );
    UserINI.Save(Path.c_str() , "LotInfo" , "dJamTime    " , LotInfo.dJamTime    );
    UserINI.Save(Path.c_str() , "LotInfo" , "dTotalTime  " , LotInfo.dTotalTime  );
    //UserINI.Save(Path.c_str() , "LotInfo" , "iStrpUPEH   " , LotInfo.iStrpUPEH   );
    //UserINI.Save(Path.c_str() , "LotInfo" , "iStrpUPH    " , LotInfo.iStrpUPH    );
    UserINI.Save(Path.c_str() , "LotInfo" , "iWrkWafer   " , LotInfo.iWrkWafer   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iMarkChip   " , LotInfo.iMarkChip   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iFailChip   " , LotInfo.iFailChip   );
}

void __fastcall CLot::LoadDayInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Path;
    AnsiString TempStr ;
    TDateTime  TempTime;

    //Make Dir.
    Path = EXE_FOLDER + "SeqData\\" + "DayInfo.ini";

    //Current Lot Informations.
    UserINI.Load(Path.c_str() , "DayInfo" , "dRunTime    " , DayInfo.dRunTime    );
    UserINI.Load(Path.c_str() , "DayInfo" , "dIdleTime   " , DayInfo.dIdleTime   );
    UserINI.Load(Path.c_str() , "DayInfo" , "dJamTime    " , DayInfo.dJamTime    );
    UserINI.Load(Path.c_str() , "DayInfo" , "dTotalTime  " , DayInfo.dTotalTime  );
    UserINI.Load(Path.c_str() , "DayInfo" , "iWrkWafer   " , DayInfo.iWrkWafer   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iMarkChip   " , DayInfo.iMarkChip   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iFailChip   " , DayInfo.iFailChip   );
}

void __fastcall CLot::SaveDayInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Path;
    AnsiString TempStr ;
    TDateTime  TempTime;

    //Make Dir.
    Path = EXE_FOLDER + "SeqData\\" + "DayInfo.ini";

    //Current Lot Informations.
    UserINI.Save(Path.c_str() , "DayInfo" , "dRunTime    " , DayInfo.dRunTime    );
    UserINI.Save(Path.c_str() , "DayInfo" , "dIdleTime   " , DayInfo.dIdleTime   );
    UserINI.Save(Path.c_str() , "DayInfo" , "dJamTime    " , DayInfo.dJamTime    );
    UserINI.Save(Path.c_str() , "DayInfo" , "dTotalTime  " , DayInfo.dTotalTime  );
    UserINI.Save(Path.c_str() , "DayInfo" , "iWrkWafer   " , DayInfo.iWrkWafer   );
    UserINI.Save(Path.c_str() , "DayInfo" , "iMarkChip   " , DayInfo.iMarkChip   );
    UserINI.Save(Path.c_str() , "DayInfo" , "iFailChip   " , DayInfo.iFailChip   );
}
void __fastcall CLot::WriteLotDayLog()
{
    //Local Var.
    int        hFile   ;
    AnsiString Path    ;
    AnsiString Temp    ;
    TDateTime  CurrDateTime;

    DelPastLotLog();

    //Set Path.
    CurrDateTime = Now();
    Temp         = CurrDateTime.CurrentDate().FormatString("yyyymmdd");
    Path         = LOG_FOLDER ;//".csv";

    if(!UserFile.FileExist(Path)) CreateDir(Path);

    Path         = LOG_FOLDER + Temp + ".csv";
    UserFile.DeleteFiles(Path);
    //File Open.
    hFile = FileOpen(Path.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(Path.c_str());
        if (hFile == -1) return;
        Temp  = "Day,"   ;
        Temp += "RunTime,"     ;
        Temp += "IdleTime,"    ;
        Temp += "JamTime,"     ;
        Temp += "TotalTime,"   ;
        Temp += "StrpUPEH,"    ;
        Temp += "StrpUPH,"     ;
        Temp += "WorkStrp,"    ;
        Temp += "WorkChip,"    ;
        Temp += "FailChip,"    ;
        Temp += "\r\n"         ;
        FileSeek (hFile , 0            , SEEK_END     );
        FileWrite(hFile , Temp.c_str() , Temp.Length());
    }

    TDateTime tTemp ;

    //Save.
    Temp  = CurrDateTime.FormatString("yy.mm.dd") + "," ;

    tTemp.Val = DayInfo.dRunTime   ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = DayInfo.dIdleTime  ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = DayInfo.dJamTime   ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = DayInfo.dTotalTime ; Temp += tTemp.FormatString("hh:nn:ss") + ",";

    Temp += (AnsiString)DayInfo.iWrkWafer    + ",";
    Temp += (AnsiString)DayInfo.iMarkChip    + ",";
    Temp += (AnsiString)DayInfo.iFailChip    + ",";
    Temp += (AnsiString)"\r\n"                    ;

    FileSeek (hFile , 0            , SEEK_END     );
    FileWrite(hFile , Temp.c_str() , Temp.Length());

    //Close File.
    FileClose(hFile);
}

void __fastcall CLot::WriteLotLog()
{
    //Local Var.
    int        hFile   ;
    AnsiString Path    ;
    AnsiString Temp    ;
    TDateTime  CurrDateTime;

    DelPastLotLog();

    //Set Path.
    CurrDateTime = Now();
    Temp         = CurrDateTime.CurrentDate().FormatString("yyyymmdd");
    Path         = LOG_FOLDER + Temp ;//".csv";



    if(!UserFile.FileExist(Path))  {
        CreateDir(Path);

        ClearDayInfo() ;

        DayInfo.dRunTime    = LotInfo.dRunTime     ;
        DayInfo.dIdleTime   = LotInfo.dIdleTime    ;
        DayInfo.dJamTime    = LotInfo.dJamTime     ;
        DayInfo.dTotalTime  = LotInfo.dTotalTime   ;

        //DayInfo.iStrpUPEH   = LotInfo.iStrpUPEH    ;
        //DayInfo.iStrpUPH    = LotInfo.iStrpUPH     ;

        DayInfo.iWrkWafer   = LotInfo.iWrkWafer    ;
        DayInfo.iMarkChip   = LotInfo.iMarkChip    ;
        DayInfo.iFailChip   = LotInfo.iFailChip    ;
    }

    TDateTime tTemp ;
    String    sLotNo ;
    tTemp.Val = LotInfo.dStartTime ;

    if(LotInfo.sWaferName == "") sLotNo = "NoLot" ;
    else                         sLotNo = LotInfo.sWaferName ;

    Path = LOG_FOLDER + Temp + "\\" + sLotNo + ".csv";
    //File Open.
    hFile = FileOpen(Path.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(Path.c_str());
        if (hFile == -1) return;
        Temp  = "Day,";
        Temp += "WaferName,";
        Temp += "JobFile,"  ;
        Temp += "Operator," ;
        Temp += "StartTime,";
        Temp += "EndTime,"  ;
        Temp += "RunTime,"  ;
        Temp += "IdleTime," ;
        Temp += "JamTime,"  ;
        Temp += "TotalTime,";
        Temp += "WorkWafer,";
        Temp += "WorkChip," ;
        Temp += "FailChip," ;
        Temp += "\r\n"      ;

        

        FileSeek (hFile , 0            , SEEK_END     );
        FileWrite(hFile , Temp.c_str() , Temp.Length());
    }

//    TDateTime tTemp ;

    //Save.
    Temp  = CurrDateTime.FormatString("yy.mm.dd") + "," ;
    Temp += (AnsiString)LotInfo.sWaferName       + ",";
    Temp += (AnsiString)LotInfo.sJobFile     + ",";
    Temp += (AnsiString)LotInfo.sOperator    + ",";

    tTemp.Val = LotInfo.dStartTime ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = LotInfo.dEndTime   ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = LotInfo.dRunTime   ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = LotInfo.dIdleTime  ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = LotInfo.dJamTime   ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = LotInfo.dTotalTime ; Temp += tTemp.FormatString("hh:nn:ss") + ",";

    //Temp += (AnsiString)LotInfo.iStrpUPEH    + ",";
    //Temp += (AnsiString)LotInfo.iStrpUPH     + ",";
    Temp += (AnsiString)LotInfo.iWrkWafer    + ",";
    Temp += (AnsiString)LotInfo.iMarkChip    + ",";
    Temp += (AnsiString)LotInfo.iFailChip    + ",";

    Temp += (AnsiString)"\r\n"                    ;

    FileSeek (hFile , 0            , SEEK_END     );
    FileWrite(hFile , Temp.c_str() , Temp.Length());

    //Close File.
    FileClose(hFile);
}

void __fastcall CLot::DelPastLotLog()
{
    UserFile.ClearDirDate(LOG_FOLDER , Now() - 400);
}

//Update Lot Efficiency.
void __fastcall CLot::Update(EN_SEQ_STAT Stat)
{
    static int iCnt = 0 ;

    iCnt++;
//    if(iCnt < 100) return ;
    iCnt = 0 ;

    static TDateTime tPreTime = Now();
    TDateTime tCrntTime = Now();
    double dGapTime = tCrntTime.Val - tPreTime.Val ;

    switch(Stat) {
        case ssInit     : DayInfo.dIdleTime += dGapTime ; LotInfo.dIdleTime += dGapTime ; break ;
        case ssWarning  : DayInfo.dIdleTime += dGapTime ; LotInfo.dIdleTime += dGapTime ; break ;
        case ssError    : DayInfo.dJamTime  += dGapTime ; LotInfo.dJamTime  += dGapTime ; break ;
        case ssRunning  : DayInfo.dRunTime  += dGapTime ; LotInfo.dRunTime  += dGapTime ; break ;
        case ssStop     : DayInfo.dIdleTime += dGapTime ; LotInfo.dIdleTime += dGapTime ; break ;
        case ssMaint    : DayInfo.dIdleTime += dGapTime ; LotInfo.dIdleTime += dGapTime ; break ;
        case ssRunWarn  : DayInfo.dRunTime  += dGapTime ; LotInfo.dRunTime  += dGapTime ; break ;
        case ssWorkEnd  : DayInfo.dIdleTime += dGapTime ; LotInfo.dIdleTime += dGapTime ; break ;
    }

    double dTotalTime = LotInfo.dIdleTime + LotInfo.dJamTime + LotInfo.dRunTime ;

    LotInfo.dTotalTime += dGapTime ;
    DayInfo.dTotalTime += dGapTime ;


    //Lot Info
    //if(LotInfo.dRunTime  ) {LotInfo.iStrpUPH  = LotInfo.iWorkStrp / (LotInfo.dRunTime * 24) ;}
    //else                   {LotInfo.iStrpUPH  = 0 ;                                          }

    //if(LotInfo.dTotalTime) {LotInfo.iStrpUPEH = LotInfo.iWorkStrp / (LotInfo.dTotalTime * 24) ;}
    //else                   {LotInfo.iStrpUPEH = 0 ;                                            }

    //Day Info
    //if(DayInfo.dRunTime)   {DayInfo.iStrpUPH  = DayInfo.iWorkStrp / (DayInfo.dRunTime * 24) ;}
    //else                   {DayInfo.iStrpUPH  = 0 ;                                          }

    //if(DayInfo.dTotalTime) {DayInfo.iStrpUPEH = DayInfo.iWorkStrp / (DayInfo.dTotalTime * 24) ;}
    //else                   {DayInfo.iStrpUPEH = 0 ;                                            }

    tPreTime = tCrntTime ;
}

bool __fastcall CLot::WriteArayData(EN_ARAY_ID riId )
{
    Trace("WriteArayData","1111111");
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  , sArayName ;
    AnsiString sLotNo ;
    AnsiString sTemp  , sTemp2 ;
    AnsiString sRslt  ;
    TDateTime  CurrDateTime;
    int        iMgzNo , iSlotNo ;
    int        iMaxCol , iMaxRow;

    DelPastLotLog();

    if( DM.ARAY[riId].GetLotNo() == "" ) sLotNo = "NoLot";
    else                                 sLotNo = DM.ARAY[riId].GetLotNo();

    TDateTime tTemp ;
    String    sDate ;
    tTemp.Val = LotInfo.dStartTime ;
    sDate = tTemp.FormatString("hh_nn_ss") ;

    //아오 ㅄ같네 진짜.
    sPath = LOG_FOLDER + CurrDateTime.CurrentDate().FormatString("yyyymmdd") + "\\" + sLotNo + "\\"+ sLotNo;

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    sPath = sPath + ".ini" ;

    iMaxCol = DM.ARAY[riId].GetMaxCol();
    iMaxRow = DM.ARAY[riId].GetMaxRow();


    for(int r = 0 ; r < iMaxRow ; r++) {
        sRslt = "" ;
        for(int c = 0 ; c < iMaxCol ; c++) {
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
    UserINI.Save(sPath.c_str()  , "ETC"  , "Row"   , iMaxRow                              );
    UserINI.Save(sPath.c_str()  , "ETC"  , "Col"   , iMaxCol                              );

    return true ;


}

bool __fastcall CLot::ReadArayData(AnsiString sPath , EN_ARAY_ID riId)
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

    if(!FileExists(sPath)) return false ;

    UserINI.Load(sPath.c_str()  , "ETC"  , "ID"    , &sId                 );
    UserINI.Load(sPath.c_str()  , "ETC"  , "LotNo" , &sLotNo              );
    UserINI.Load(sPath.c_str()  , "ETC"  , "Row"   , &iRow                );
    UserINI.Load(sPath.c_str()  , "ETC"  , "Col"   , &iCol                );

    DM.ARAY[riId].SetMaxColRow(iCol,iRow );

    for(int r = 0 ; r < iRow ; r++) {
        sTemp.printf("R%02d", r);
        UserINI.Load(sPath.c_str()  , "Data"  , sTemp , sRslt );
        for(int c = 0 ; c < iCol ; c++) {
            //sVal = "0x"+sRslt.SubString(c+1,1) ;
            sVal = sRslt.SubString(1,2) ;
            sRslt.Delete(1,3) ;
            DM.ARAY[riId].SetStat(r,c,(EN_CHIP_STAT)StrToInt(sVal)  );
            sTemp2 = sVal+"," ;
        }
    }

    DM.ARAY[riId].SetID   (sId   );
    DM.ARAY[riId].SetLotNo(sLotNo);

//    UserFile.DeleteFiles(sPath);

    Trace("Rcv Array",sTemp2.c_str()) ;

    return true ;
}
