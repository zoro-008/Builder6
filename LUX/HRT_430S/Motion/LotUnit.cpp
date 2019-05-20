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
#include "SortingTool.h"
#include "Sequence.h"
#include "Loader.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

#define SPC_FOLDER "d:\\Spc\\"

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

AnsiString GetLotNo (void ) //참조만.
{
    return g_sLotNo[0] ;
}

AnsiString PopLotNo (void )
{
   AnsiString sLotNo = g_sLotNo[0] ;
   int i;
   for(i = 0 ; i < MAX_LOT_CNT - 1 ; i++) {
       g_sLotNo[i] = g_sLotNo[i+1] ;
   }
   g_sLotNo[i] = "";
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
}

void CLot::Init()
{
    //Init. Flag.
    m_bLotOpen  = false;

    //Init. Buffer.
    ClearLotInfo();
    ClearDayInfo();

    LoadLotInfo();
    LoadDayInfo();

    if(!FileExists(LOG_FOLDER)) CreateDir(LOG_FOLDER);
}

__fastcall CLot::~CLot(void)
{

}

void CLot::Close()
{
    SaveLotInfo();
    SaveDayInfo();
//    Trace("","");
}

void __fastcall CLot::ClearLotInfo(void)
{
    //memset(&LotInfo   , 0  , sizeof(SLotInfo));
    for(int i = 0 ; i < MAX_LOT_CNT ; i++) g_sLotNo[i] == "";
    LotInfo.sLotNo     = "" ;
    LotInfo.iCrntLotNo = 0  ;
    LotInfo.iLastLotNo = 0  ;
    LotInfo.sJobFile   = "" ;
    LotInfo.sOperator  = "" ;
    LotInfo.iLotMgzCnt = 0  ;

    LotInfo.dStartTime = 0.0;
    LotInfo.dEndTime   = 0.0;

    LotInfo.dRunTime   = 0.0;
    LotInfo.dIdleTime  = 0.0;
    LotInfo.dJamTime   = 0.0;
    LotInfo.dTotalTime = 0.0;

    LotInfo.iStrpUPEH  = 0  ;
    LotInfo.iChipUPEH  = 0  ;

    LotInfo.iStrpUPH   = 0  ;
    LotInfo.iChipUPH   = 0  ;

    LotInfo.iWorkStrp  = 0  ;
    LotInfo.iWorkChip  = 0  ;
    LotInfo.iFailChip  = 0  ;
    LotInfo.iBin0      = 0  ;
    LotInfo.iBin1      = 0  ;
    LotInfo.iBin2      = 0  ;
    LotInfo.iBin3      = 0  ;
    LotInfo.iBin4      = 0  ;
    LotInfo.iBin5      = 0  ;
    LotInfo.iBin6      = 0  ;
    LotInfo.iBin7      = 0  ;
    LotInfo.iBin8      = 0  ;
    LotInfo.iBin9      = 0  ;
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

    LotInfo.sLotNo     = _sLotNo    ;
    if(_sJobFile  == "" ) LotInfo.sJobFile  = sTempJobFile  ;
    else                  LotInfo.sJobFile  = _sJobFile     ;
    if(_sOperator == "" ) LotInfo.sOperator = sTempOperator ;
    else                  LotInfo.sOperator = _sOperator    ;

    LotInfo.iCrntLotNo = 0 ;

    LotInfo.dStartTime = Now();

    //for(int i = 0 ; i < MAX_ARAY ; i++) {
    //    DM.ARAY[i].SetStat(csEmpty) ;
    //}

    //Set Lot Open Flag.
    m_bLotOpen  = true ;
}

//Lot Processing.
/*void __fastcall CLot::LotOpen (AnsiString _sLotNo)
{

    LotInfo.sLotNo     = _sLotNo    ;

    //Set Lot Open Flag.
    m_bLotOpen  = true ;
}*/

AnsiString  __fastcall CLot::GetLotNo ()
{
    return LotInfo.sLotNo ;
}


void __fastcall CLot::LotEnd (void)
{
    //Check already opened Lot.
    if (!m_bLotOpen) return;

    //Set EndTime.
    LotInfo.dEndTime = Now();

    //Reset Lot Flag.
    m_bLotOpen = false;
    m_bLotEnd  = true ;



    //Check Retest Mode.
    STL._iBinCnt = 0 ; //빈 카운트 0번으로 셋팅함.
    LDR._iMgzCnt = 0 ;

    //SEQ._bInspMstr = false;
//    SEQ.Reset();

    Trace("SEQ","Lot Finished"   );
    FM_MsgOk("Confirm" , "LOT IS FINISHED"   );


    //m_bLotEnd  = false ;

}

void __fastcall CLot::DispLotInfo(TPanel * _pnLotNo    ,
                                  TPanel * _pnJobFile  ,
                                  TPanel * _pnOperator ,
                                  TPanel * _pnStartTime,
                                  TPanel * _pnEndTime  ,
                                  TPanel * _pnRunTime  ,
                                  TPanel * _pnIdleTime ,
                                  TPanel * _pnJamTime  ,
                                  TPanel * _pnTotalTime,
                                  TPanel * _pnStrpUPEH ,
                                  TPanel * _pnChipUPEH ,
                                  TPanel * _pnStrpUPH  ,
                                  TPanel * _pnChipUPH  ,
                                  TPanel * _pnWorkStrp ,
                                  TPanel * _pnWorkChip ,
                                  TPanel * _pnFailChip )
{
    TDateTime tTemp ;

    if(_pnLotNo   ) _pnLotNo     -> Caption = LotInfo.sLotNo     ;//g_sLotNo[0] ;
    if(_pnJobFile ) _pnJobFile   -> Caption = LotInfo.sJobFile   ;
    if(_pnOperator) _pnOperator  -> Caption = LotInfo.sOperator  ;
    tTemp.Val = LotInfo.dStartTime ; if(_pnStartTime) _pnStartTime -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = LotInfo.dEndTime   ; if(_pnEndTime  ) _pnEndTime   -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = LotInfo.dRunTime   ; if(_pnRunTime  ) _pnRunTime   -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = LotInfo.dIdleTime  ; if(_pnIdleTime ) _pnIdleTime  -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = LotInfo.dJamTime   ; if(_pnJamTime  ) _pnJamTime   -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = LotInfo.dTotalTime ; if(_pnTotalTime) _pnTotalTime -> Caption = tTemp.FormatString("hh:nn:ss") ;
    if(_pnStrpUPEH) _pnStrpUPEH -> Caption = LotInfo.iStrpUPEH  ;
    if(_pnChipUPEH) _pnChipUPEH -> Caption = LotInfo.iChipUPEH  ;
    if(_pnStrpUPH ) _pnStrpUPH  -> Caption = LotInfo.iStrpUPH   ;
    if(_pnChipUPH ) _pnChipUPH  -> Caption = LotInfo.iChipUPH   ;
    if(_pnWorkStrp) _pnWorkStrp -> Caption = LotInfo.iWorkStrp  ;
    if(_pnWorkChip) _pnWorkChip -> Caption = LotInfo.iWorkChip  ;
    if(_pnFailChip) _pnFailChip -> Caption = LotInfo.iFailChip  ;

}

void __fastcall CLot::DispDayInfo(TPanel * _pnRunTime   ,
                                  TPanel * _pnIdleTime  ,
                                  TPanel * _pnJamTime   ,
                                  TPanel * _pnTotalTime ,
                                  TPanel * _pnStrpUPEH  ,
                                  TPanel * _pnChipUPEH  ,
                                  TPanel * _pnStrpUPH   ,
                                  TPanel * _pnChipUPH   ,
                                  TPanel * _pnWorkStrp  ,
                                  TPanel * _pnWorkChip  ,
                                  TPanel * _pnFailChip  ,
                                  TPanel * _pnWorkLot   )
{
    TDateTime tTemp ;

    tTemp.Val = DayInfo.dRunTime   ; if(_pnRunTime  ) _pnRunTime   -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = DayInfo.dIdleTime  ; if(_pnIdleTime ) _pnIdleTime  -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = DayInfo.dJamTime   ; if(_pnJamTime  ) _pnJamTime   -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = DayInfo.dTotalTime ; if(_pnTotalTime) _pnTotalTime -> Caption = tTemp.FormatString("hh:nn:ss") ;
    if(_pnStrpUPEH) _pnStrpUPEH -> Caption = DayInfo.iStrpUPEH  ;
    if(_pnChipUPEH) _pnChipUPEH -> Caption = DayInfo.iChipUPEH  ;
    if(_pnStrpUPH ) _pnStrpUPH  -> Caption = DayInfo.iStrpUPH   ;
    if(_pnChipUPH ) _pnChipUPH  -> Caption = DayInfo.iChipUPH   ;
    if(_pnWorkStrp) _pnWorkStrp -> Caption = DayInfo.iWorkStrp  ;
    if(_pnWorkChip) _pnWorkChip -> Caption = DayInfo.iWorkChip  ;
    if(_pnFailChip) _pnFailChip -> Caption = DayInfo.iFailChip  ;
    if(_pnWorkLot ) _pnWorkLot  -> Caption = DayInfo.iLotCnt    ;
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
    Path = ExtractFilePath(Application->ExeName) + "SeqData\\" + "LotInfo.ini";

    //Current Lot Informations.
    UserINI.Load(Path.c_str() , "Member " , "m_bLotOpen " , m_bLotOpen         );
    UserINI.Load(Path.c_str() , "LotInfo" , "sLotNo     " , LotInfo.sLotNo     );
    UserINI.Load(Path.c_str() , "LotInfo" , "sJobFile   " , LotInfo.sJobFile   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iLotMgzCnt " , LotInfo.iLotMgzCnt );
    UserINI.Load(Path.c_str() , "LotInfo" , "sOperator  " , LotInfo.sOperator  );
    UserINI.Load(Path.c_str() , "LotInfo" , "dStartTime " , LotInfo.dStartTime );
    UserINI.Load(Path.c_str() , "LotInfo" , "dEndTime   " , LotInfo.dEndTime   );
    UserINI.Load(Path.c_str() , "LotInfo" , "dRunTime   " , LotInfo.dRunTime   );
    UserINI.Load(Path.c_str() , "LotInfo" , "dIdleTime  " , LotInfo.dIdleTime  );
    UserINI.Load(Path.c_str() , "LotInfo" , "dJamTime   " , LotInfo.dJamTime   );
    UserINI.Load(Path.c_str() , "LotInfo" , "dTotalTime " , LotInfo.dTotalTime );
    UserINI.Load(Path.c_str() , "LotInfo" , "iStrpUPEH  " , LotInfo.iStrpUPEH  );
    UserINI.Load(Path.c_str() , "LotInfo" , "iChipUPEH  " , LotInfo.iChipUPEH  );
    UserINI.Load(Path.c_str() , "LotInfo" , "iStrpUPH   " , LotInfo.iStrpUPH   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iChipUPH   " , LotInfo.iChipUPH   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iWorkStrp  " , LotInfo.iWorkStrp  );
    UserINI.Load(Path.c_str() , "LotInfo" , "iWorkChip  " , LotInfo.iWorkChip  );
//    UserINI.Load(Path.c_str() , "LotInfo" , "iGood      " , LotInfo.iGood      );
//    UserINI.Load(Path.c_str() , "LotInfo" , "iFail      " , LotInfo.iFail      );
    UserINI.Load(Path.c_str() , "LotInfo" , "iBin0      " , LotInfo.iBin0      );
    UserINI.Load(Path.c_str() , "LotInfo" , "iBin1      " , LotInfo.iBin1      );
    UserINI.Load(Path.c_str() , "LotInfo" , "iBin2      " , LotInfo.iBin2      );
    UserINI.Load(Path.c_str() , "LotInfo" , "iBin3      " , LotInfo.iBin3      );
    UserINI.Load(Path.c_str() , "LotInfo" , "iBin4      " , LotInfo.iBin4      );
    UserINI.Load(Path.c_str() , "LotInfo" , "iBin5      " , LotInfo.iBin5      );
    UserINI.Load(Path.c_str() , "LotInfo" , "iBin6      " , LotInfo.iBin6      );
    UserINI.Load(Path.c_str() , "LotInfo" , "iBin7      " , LotInfo.iBin7      );
    UserINI.Load(Path.c_str() , "LotInfo" , "iBin8      " , LotInfo.iBin8      );
    UserINI.Load(Path.c_str() , "LotInfo" , "iBin9      " , LotInfo.iBin9      );

    for(int i = 0 ; i < MAX_LOT_CNT ; i++)
        UserINI.Load(Path.c_str() , "LotNoQue" , "g_sLotNo"+AnsiString(i) , g_sLotNo[i]     );

}

void __fastcall CLot::SaveLotInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Path;
    AnsiString TempStr ;
    TDateTime  TempTime;

    //Make Dir.
    Path = ExtractFilePath(Application->ExeName) + "SeqData\\" + "LotInfo.ini";

    //Current Lot Informations.
    UserINI.Save(Path.c_str() , "Member " , "m_bLotOpen " , m_bLotOpen         );
    if(LotInfo.sLotNo   != "" )UserINI.Save(Path.c_str() , "LotInfo" , "sLotNo     " , LotInfo.sLotNo     );
    if(LotInfo.sJobFile != "" )UserINI.Save(Path.c_str() , "LotInfo" , "sJobFile   " , LotInfo.sJobFile   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iLotMgzCnt " , LotInfo.iLotMgzCnt );
    UserINI.Save(Path.c_str() , "LotInfo" , "sOperator  " , LotInfo.sOperator  );
    UserINI.Save(Path.c_str() , "LotInfo" , "dStartTime " , LotInfo.dStartTime );
    UserINI.Save(Path.c_str() , "LotInfo" , "dEndTime   " , LotInfo.dEndTime   );
    UserINI.Save(Path.c_str() , "LotInfo" , "dRunTime   " , LotInfo.dRunTime   );
    UserINI.Save(Path.c_str() , "LotInfo" , "dIdleTime  " , LotInfo.dIdleTime  );
    UserINI.Save(Path.c_str() , "LotInfo" , "dJamTime   " , LotInfo.dJamTime   );
    UserINI.Save(Path.c_str() , "LotInfo" , "dTotalTime " , LotInfo.dTotalTime );
    UserINI.Save(Path.c_str() , "LotInfo" , "iStrpUPEH  " , LotInfo.iStrpUPEH  );
    UserINI.Save(Path.c_str() , "LotInfo" , "iChipUPEH  " , LotInfo.iChipUPEH  );
    UserINI.Save(Path.c_str() , "LotInfo" , "iStrpUPH   " , LotInfo.iStrpUPH   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iChipUPH   " , LotInfo.iChipUPH   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iWorkStrp  " , LotInfo.iWorkStrp  );
    UserINI.Save(Path.c_str() , "LotInfo" , "iWorkChip  " , LotInfo.iWorkChip  );
//    UserINI.Save(Path.c_str() , "LotInfo" , "iGood      " , LotInfo.iGood      );
//    UserINI.Save(Path.c_str() , "LotInfo" , "iFail      " , LotInfo.iFail      );
    UserINI.Save(Path.c_str() , "LotInfo" , "iBin0      " , LotInfo.iBin0      );
    UserINI.Save(Path.c_str() , "LotInfo" , "iBin1      " , LotInfo.iBin1      );
    UserINI.Save(Path.c_str() , "LotInfo" , "iBin2      " , LotInfo.iBin2      );
    UserINI.Save(Path.c_str() , "LotInfo" , "iBin3      " , LotInfo.iBin3      );
    UserINI.Save(Path.c_str() , "LotInfo" , "iBin4      " , LotInfo.iBin4      );
    UserINI.Save(Path.c_str() , "LotInfo" , "iBin5      " , LotInfo.iBin5      );
    UserINI.Save(Path.c_str() , "LotInfo" , "iBin6      " , LotInfo.iBin6      );
    UserINI.Save(Path.c_str() , "LotInfo" , "iBin7      " , LotInfo.iBin7      );
    UserINI.Save(Path.c_str() , "LotInfo" , "iBin8      " , LotInfo.iBin8      );
    UserINI.Save(Path.c_str() , "LotInfo" , "iBin9      " , LotInfo.iBin9      );

    for(int i = 0 ; i < MAX_LOT_CNT ; i++)
        UserINI.Save(Path.c_str() , "LotNoQue" , "g_sLotNo"+AnsiString(i) , g_sLotNo[i]     );
}

void __fastcall CLot::LoadDayInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Path;
    AnsiString TempStr ;
    TDateTime  TempTime;

    //Make Dir.
    Path = ExtractFilePath(Application->ExeName) + "SeqData\\" + "DayInfo.ini";

    //Current Lot Informations.
    UserINI.Load(Path.c_str() , "DayInfo" , "dRunTime  " , DayInfo.dRunTime   );
    UserINI.Load(Path.c_str() , "DayInfo" , "dIdleTime " , DayInfo.dIdleTime  );
    UserINI.Load(Path.c_str() , "DayInfo" , "dJamTime  " , DayInfo.dJamTime   );
    UserINI.Load(Path.c_str() , "DayInfo" , "dTotalTime" , DayInfo.dTotalTime );
    UserINI.Load(Path.c_str() , "DayInfo" , "iStrpUPEH " , DayInfo.iStrpUPEH  );
    UserINI.Load(Path.c_str() , "DayInfo" , "iChipUPEH " , DayInfo.iChipUPEH  );
    UserINI.Load(Path.c_str() , "DayInfo" , "iStrpUPH  " , DayInfo.iStrpUPH   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iChipUPH  " , DayInfo.iChipUPH   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iWorkStrp " , DayInfo.iWorkStrp  );
    UserINI.Load(Path.c_str() , "DayInfo" , "iWorkChip " , DayInfo.iWorkChip  );
    UserINI.Load(Path.c_str() , "DayInfo" , "iFailChip " , DayInfo.iFailChip  );
    UserINI.Load(Path.c_str() , "DayInfo" , "iLotCnt   " , DayInfo.iLotCnt    );
//    UserINI.Load(Path.c_str() , "DayInfo" , "iGood     " , DayInfo.iGood      );
//    UserINI.Load(Path.c_str() , "DayInfo" , "iFail     " , DayInfo.iFail      );
}

void __fastcall CLot::SaveDayInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Path;
    AnsiString TempStr ;
    TDateTime  TempTime;

    //Make Dir.
    Path = ExtractFilePath(Application->ExeName) + "SeqData\\" + "DayInfo.ini";

    //Current Lot Informations.
    UserINI.Save(Path.c_str() , "DayInfo" , "dRunTime  " , DayInfo.dRunTime   );
    UserINI.Save(Path.c_str() , "DayInfo" , "dIdleTime " , DayInfo.dIdleTime  );
    UserINI.Save(Path.c_str() , "DayInfo" , "dJamTime  " , DayInfo.dJamTime   );
    UserINI.Save(Path.c_str() , "DayInfo" , "dTotalTime" , DayInfo.dTotalTime );
    UserINI.Save(Path.c_str() , "DayInfo" , "iStrpUPEH " , DayInfo.iStrpUPEH  );
    UserINI.Save(Path.c_str() , "DayInfo" , "iChipUPEH " , DayInfo.iChipUPEH  );
    UserINI.Save(Path.c_str() , "DayInfo" , "iStrpUPH  " , DayInfo.iStrpUPH   );
    UserINI.Save(Path.c_str() , "DayInfo" , "iChipUPH  " , DayInfo.iChipUPH   );
    UserINI.Save(Path.c_str() , "DayInfo" , "iWorkStrp " , DayInfo.iWorkStrp  );
    UserINI.Save(Path.c_str() , "DayInfo" , "iWorkChip " , DayInfo.iWorkChip  );
    UserINI.Save(Path.c_str() , "DayInfo" , "iFailChip " , DayInfo.iFailChip  );
    UserINI.Save(Path.c_str() , "DayInfo" , "iLotCnt   " , DayInfo.iLotCnt    );
//    UserINI.Save(Path.c_str() , "DayInfo" , "iGood     " , DayInfo.iGood      );
//    UserINI.Save(Path.c_str() , "DayInfo" , "iFail     " , DayInfo.iFail      );
}

void __fastcall CLot::WriteLotDayLog()
{
    //Local Var.
    int        hFile   ;
    AnsiString Path    ;
    AnsiString Temp    ;
    TDateTime  CurrDateTime;
    TDateTime  tTemp   ;
    bool       bNewDay ;

    DelPastLotLog();

    //Set Path.
    CurrDateTime = Now();
    Temp         = CurrDateTime.CurrentDate().FormatString("yyyymmdd");
    Path         = LOG_FOLDER ;//".csv";

    if(!UserFile.FileExist(Path)) CreateDir(Path);

    Path         = LOG_FOLDER + Temp + ".csv";

    bNewDay = !UserFile.FileExist(Path); //새로운 날인지 확인하고

    UserFile.DeleteFiles(Path); // 지운다.
    //File Open.
    hFile = FileOpen(Path.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(Path.c_str());
        if (hFile == -1) return;
        Temp  = "Day(Time),"   ;
        Temp += "RunTime,"     ;
        Temp += "IdleTime,"    ;
        Temp += "JamTime,"     ;
        Temp += "TotalTime,"   ;
        Temp += "StrpUPEH,"    ;
        Temp += "ChipUPEH,"    ;
        Temp += "StrpUPH,"     ;
        Temp += "ChipUPH,"     ;
        Temp += "WorkStrp,"    ;
        Temp += "WorkChip,"    ;
        Temp += "FailChip,"    ;
        Temp += "LotCnt,"      ;
        Temp += "\r\n"         ;

        FileSeek (hFile , 0            , SEEK_END     );
        FileWrite(hFile , Temp.c_str() , Temp.Length());

        if(bNewDay) { //새로운 날이면 전날 마지막 돌리던 물량 데이터 인계 받음.
            tTemp.Val = DayInfo.dRunTime   ; Trace("DayInfo.dRunTime"  ,tTemp.FormatString("hh:nn:ss").c_str());
            tTemp.Val = DayInfo.dIdleTime  ; Trace("DayInfo.dIdleTime" ,tTemp.FormatString("hh:nn:ss").c_str());
            tTemp.Val = DayInfo.dJamTime   ; Trace("DayInfo.dJamTime"  ,tTemp.FormatString("hh:nn:ss").c_str());
            tTemp.Val = DayInfo.dTotalTime ; Trace("DayInfo.dTotalTime",tTemp.FormatString("hh:nn:ss").c_str());
                                             Trace("DayInfo.iStrpUPEH" ,AnsiString(DayInfo.iStrpUPEH ).c_str());
                                             Trace("DayInfo.iChipUPEH" ,AnsiString(DayInfo.iChipUPEH ).c_str());
                                             Trace("DayInfo.iStrpUPH"  ,AnsiString(DayInfo.iStrpUPH  ).c_str());
                                             Trace("DayInfo.iChipUPH"  ,AnsiString(DayInfo.iChipUPH  ).c_str());
                                             Trace("DayInfo.iWorkStrp" ,AnsiString(DayInfo.iWorkStrp ).c_str());
                                             Trace("DayInfo.iWorkChip" ,AnsiString(DayInfo.iWorkChip ).c_str());
                                             Trace("DayInfo.iFailChip" ,AnsiString(DayInfo.iFailChip ).c_str());
                                             Trace("DayInfo.iLotCnt"   ,AnsiString(DayInfo.iLotCnt   ).c_str());

            DayInfo.dRunTime    = LotInfo.dRunTime     ;
            DayInfo.dIdleTime   = LotInfo.dIdleTime    ;
            DayInfo.dJamTime    = LotInfo.dJamTime     ;
            DayInfo.dTotalTime  = LotInfo.dTotalTime   ;
            DayInfo.iStrpUPEH   = LotInfo.iStrpUPEH    ;
            DayInfo.iChipUPEH   = LotInfo.iChipUPEH    ;
            DayInfo.iStrpUPH    = LotInfo.iStrpUPH     ;
            DayInfo.iChipUPH    = LotInfo.iChipUPH     ;
            DayInfo.iWorkStrp   = LotInfo.iWorkStrp    ;
            DayInfo.iWorkChip   = LotInfo.iWorkChip    ;
            DayInfo.iFailChip   = LotInfo.iFailChip    ;
            DayInfo.iLotCnt     = 1                    ;

                                             Trace("After Change"      ,"------------------------------------");
            tTemp.Val = DayInfo.dRunTime   ; Trace("DayInfo.dRunTime"  ,tTemp.FormatString("hh:nn:ss").c_str());
            tTemp.Val = DayInfo.dIdleTime  ; Trace("DayInfo.dIdleTime" ,tTemp.FormatString("hh:nn:ss").c_str());
            tTemp.Val = DayInfo.dJamTime   ; Trace("DayInfo.dJamTime"  ,tTemp.FormatString("hh:nn:ss").c_str());
            tTemp.Val = DayInfo.dTotalTime ; Trace("DayInfo.dTotalTime",tTemp.FormatString("hh:nn:ss").c_str());
                                             Trace("DayInfo.iStrpUPEH" ,AnsiString(DayInfo.iStrpUPEH ).c_str());
                                             Trace("DayInfo.iChipUPEH" ,AnsiString(DayInfo.iChipUPEH ).c_str());
                                             Trace("DayInfo.iStrpUPH"  ,AnsiString(DayInfo.iStrpUPH  ).c_str());
                                             Trace("DayInfo.iChipUPH"  ,AnsiString(DayInfo.iChipUPH  ).c_str());
                                             Trace("DayInfo.iWorkStrp" ,AnsiString(DayInfo.iWorkStrp ).c_str());
                                             Trace("DayInfo.iWorkChip" ,AnsiString(DayInfo.iWorkChip ).c_str());
                                             Trace("DayInfo.iFailChip" ,AnsiString(DayInfo.iFailChip ).c_str());
                                             Trace("DayInfo.iLotCnt"   ,AnsiString(DayInfo.iLotCnt   ).c_str());

            SEQ._bInspMstr = false;
        }
    }

    //Save.
    Temp  = CurrDateTime.FormatString("yy.mm.dd(hh:nn:ss)") + "," ;

    tTemp.Val = DayInfo.dRunTime   ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = DayInfo.dIdleTime  ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = DayInfo.dJamTime   ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = DayInfo.dTotalTime ; Temp += tTemp.FormatString("hh:nn:ss") + ",";

    Temp += (AnsiString)DayInfo.iStrpUPEH    + ",";
    Temp += (AnsiString)DayInfo.iChipUPEH    + ",";
    Temp += (AnsiString)DayInfo.iStrpUPH     + ",";
    Temp += (AnsiString)DayInfo.iChipUPH     + ",";
    Temp += (AnsiString)DayInfo.iWorkStrp    + ",";
    Temp += (AnsiString)DayInfo.iWorkChip    + ",";
    Temp += (AnsiString)DayInfo.iFailChip    + ",";
    Temp += (AnsiString)DayInfo.iLotCnt      + ",";
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


    if(!UserFile.FileExist(Path)) {

        CreateDir(Path);


        //ClearDayInfo() ;
        //
        //DayInfo.dRunTime    = LotInfo.dRunTime     ;
        //DayInfo.dIdleTime   = LotInfo.dIdleTime    ;
        //DayInfo.dJamTime    = LotInfo.dJamTime     ;
        //DayInfo.dTotalTime  = LotInfo.dTotalTime   ;
        //
        //DayInfo.iStrpUPEH   = LotInfo.iStrpUPEH    ;
        //DayInfo.iChipUPEH   = LotInfo.iChipUPEH    ;
        //DayInfo.iStrpUPH    = LotInfo.iStrpUPH     ;
        //DayInfo.iChipUPH    = LotInfo.iChipUPH     ;
        //
        //DayInfo.iWorkStrp   = LotInfo.iWorkStrp    ;
        //DayInfo.iWorkChip   = LotInfo.iWorkChip    ;
        //DayInfo.iFailChip   = LotInfo.iFailChip    ;
        //
        //DayInfo.iLotCnt     = 1                    ;
    }

    if(LotInfo.sLotNo != "") Path = LOG_FOLDER + Temp + "\\" + LotInfo.sLotNo + ".csv";
    else                     Path = LOG_FOLDER + Temp + "\\" + Now() + ".csv";

    //File Open.
    hFile = FileOpen(Path.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(Path.c_str());
        if (hFile == -1) return;
        Temp  = "Day(Time),";
        Temp += "LotNo,"    ;
        Temp += "JobFile,"  ;
        Temp += "Operator," ;
        Temp += "StartTime,";
        Temp += "EndTime,"  ;
        Temp += "RunTime,"  ;
        Temp += "IdleTime," ;
        Temp += "JamTime,"  ;
        Temp += "TotalTime,";
        Temp += "StrpUPEH," ;
        Temp += "ChipUPEH," ;
        Temp += "StrpUPH,"  ;
        Temp += "ChipUPH,"  ;
        Temp += "WorkStrp," ;
        Temp += "WorkChip," ;
        Temp += "FailChip," ;
        Temp += "Bin0";
        Temp += "Bin1";
        Temp += "Bin2";
        Temp += "Bin3";
        Temp += "Bin4";
        Temp += "Bin5";
        Temp += "Bin6";
        Temp += "Bin7";
        Temp += "Bin8";
        Temp += "Bin9";

//        Temp += "iGood"     ;
//        Temp += "iFail"     ;
        Temp += "\r\n"      ;

        FileSeek (hFile , 0            , SEEK_END     );
        FileWrite(hFile , Temp.c_str() , Temp.Length());
    }

    TDateTime tTemp ;

    //Save.
    Temp  = CurrDateTime.FormatString("yy.mm.dd(hh:nn:ss)") + "," ;
    Temp += (AnsiString)LotInfo.sLotNo       + ",";
    Temp += (AnsiString)LotInfo.sJobFile     + ",";
    Temp += (AnsiString)LotInfo.sOperator    + ",";

    tTemp.Val = LotInfo.dStartTime ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = LotInfo.dEndTime   ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = LotInfo.dRunTime   ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = LotInfo.dIdleTime  ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = LotInfo.dJamTime   ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = LotInfo.dTotalTime ; Temp += tTemp.FormatString("hh:nn:ss") + ",";

    Temp += (AnsiString)LotInfo.iStrpUPEH + "," ;
    Temp += (AnsiString)LotInfo.iChipUPEH + "," ;
    Temp += (AnsiString)LotInfo.iStrpUPH  + "," ;
    Temp += (AnsiString)LotInfo.iChipUPH  + "," ;
    Temp += (AnsiString)LotInfo.iWorkStrp + "," ;
    Temp += (AnsiString)LotInfo.iWorkChip + "," ;
    Temp += (AnsiString)LotInfo.iFailChip + "," ;

    Temp += (AnsiString)LotInfo.iBin0     + "," ;
    Temp += (AnsiString)LotInfo.iBin1     + "," ;
    Temp += (AnsiString)LotInfo.iBin2     + "," ;
    Temp += (AnsiString)LotInfo.iBin3     + "," ;
    Temp += (AnsiString)LotInfo.iBin4     + "," ;
    Temp += (AnsiString)LotInfo.iBin5     + "," ;
    Temp += (AnsiString)LotInfo.iBin6     + "," ;
    Temp += (AnsiString)LotInfo.iBin7     + "," ;
    Temp += (AnsiString)LotInfo.iBin8     + "," ;
    Temp += (AnsiString)LotInfo.iBin9     + "," ;

    Temp += (AnsiString)"\r\n"                  ;

    UpdateMESInfo();

    FileSeek (hFile , 0            , SEEK_END     );
    FileWrite(hFile , Temp.c_str() , Temp.Length());

    //Close File.
    FileClose(hFile);
}

void __fastcall CLot::DelPastLotLog()
{
    UserFile.ClearDirDate(LOG_FOLDER , Now() - 30);
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

    AnsiString sDate = tCrntTime.CurrentDate();

    if(tCrntTime.CurrentDate() != tPreTime.CurrentDate()) {
        //ClearDayInfo() ;
    }

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
    if(LotInfo.dRunTime)   {LotInfo.iChipUPH  = LotInfo.iWorkChip / (LotInfo.dRunTime * 24) ;}
    else                   {LotInfo.iChipUPH  = 0 ;                                        }
    if(LotInfo.dRunTime)   {LotInfo.iStrpUPH  = LotInfo.iWorkStrp / (LotInfo.dRunTime * 24) ;}
    else                   {LotInfo.iStrpUPH  = 0 ;                                        }

    if(LotInfo.dTotalTime) {LotInfo.iChipUPEH = LotInfo.iWorkChip / (LotInfo.dTotalTime * 24) ;}
    else                   {LotInfo.iChipUPEH = 0 ;                                          }
    if(LotInfo.dTotalTime) {LotInfo.iStrpUPEH = LotInfo.iWorkStrp / (LotInfo.dTotalTime * 24) ;}
    else                   {LotInfo.iStrpUPEH = 0 ;                                              }

    //Day Info
    if(DayInfo.dRunTime)   {DayInfo.iChipUPH  = DayInfo.iWorkChip / (DayInfo.dRunTime * 24) ;}
    else                   {DayInfo.iChipUPH  = 0 ;                                          }
    if(DayInfo.dRunTime)   {DayInfo.iStrpUPH  = DayInfo.iWorkStrp / (DayInfo.dRunTime * 24) ;}
    else                   {DayInfo.iStrpUPH  = 0 ;                                          }

    if(DayInfo.dTotalTime) {DayInfo.iChipUPEH = DayInfo.iWorkChip / (DayInfo.dTotalTime * 24) ;}
    else                   {DayInfo.iChipUPEH = 0 ;                                            }
    if(DayInfo.dTotalTime) {DayInfo.iStrpUPEH = DayInfo.iWorkStrp / (DayInfo.dTotalTime * 24) ;}
    else                   {DayInfo.iStrpUPEH = 0 ;                                            }

    tPreTime = tCrntTime ;
}


void __fastcall CLot::UpdateDate(TStringGrid * _sgDate)
{
    UserFile.GridSearchDir(LOG_FOLDER , _sgDate , 1 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
}

void __fastcall CLot::UpdateLotName(TStringGrid * _sgDate , TStringGrid * _sgLot)
{
    AnsiString sDay ,sPath ;
    sDay = _sgDate->Cells[1][_sgDate -> Row] ;
    sPath = LOG_FOLDER + sDay + "\\" ;
    UserFile.GridSearchFile(sPath , _sgLot , 1 , true);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
}

void __fastcall CLot::DispLotDate(TStringGrid * _sgDate , TStringGrid * _sgLot , TStringGrid * _sgLotInfo)
{
    AnsiString sDay  = _sgDate -> Cells[1][ _sgDate -> Row] ;
    AnsiString sPath = LOG_FOLDER + sDay + "\\" + _sgLot -> Cells[1][ _sgLot -> Row];

    //StringGrid Clear
    for(int i =0 ; i < _sgLotInfo->ColCount ; i++ ) {
        for(int j =1 ; j < _sgLotInfo->RowCount ; j++ ) {
            _sgLotInfo -> Cells[i][j] = "" ;
        }
    }

    _sgLotInfo -> ColCount = 18 ;
    _sgLotInfo -> RowCount = 100 ;//UserFile.SearchDir(sPath) + 1 ;

//    _sgLotInfo -> FixedCols = 1 ;
    //_sgLotInfo -> FixedRows = 1 ;

    _sgLotInfo -> DefaultColWidth  = 80 ;
    _sgLotInfo -> DefaultRowHeight = 25 ;
    _sgLotInfo -> ColWidths[0] = 30  ;
    _sgLotInfo -> ColWidths[1] = 145 ;
    _sgLotInfo -> ColWidths[2] = 210 ;
    _sgLotInfo -> ColWidths[3] = 150 ;

    AnsiString  strName, str = "";
    AnsiString  sRowStr = "";
    AnsiString  sItmStr = "";
    int hwnd, flen;
    int iRowCnt = 0 ;
    int iColCnt = 0 ;
    char    *pfbuf;

    hwnd = FileOpen(sPath.c_str(), fmOpenRead) ;
    if(hwnd == NULL) return;

    flen = FileSeek(hwnd,0,2);
    FileSeek(hwnd,0,0);
    pfbuf = new char[flen+1];
    memset(pfbuf , 0 , sizeof(char)*(flen+1));
    FileRead(hwnd, pfbuf, flen);
    FileClose(hwnd);

    str = pfbuf ;

    while (str.Pos("\r\n")) {
        str.Delete(str.Pos("\r\n") , 2 ) ;
        if(iRowCnt > 100 ) return ;
        iRowCnt++ ;
    }

    _sgLotInfo -> RowCount = iRowCnt ;

    str = pfbuf ;
    iRowCnt = 0 ;

    while (str.Pos("\r\n")) {
        sRowStr = str.SubString(1 , str.Pos("\r\n")) ;
        str.Delete(1,str.Pos("\r\n")+1) ;
        iColCnt = 0 ;
        _sgLotInfo -> Cells[iColCnt][iRowCnt] = iRowCnt  ;
        while (sRowStr.Pos(",")){
            iColCnt++ ;
            sItmStr = sRowStr.SubString(0,sRowStr.Pos(",")-1) ;
            sRowStr.Delete(1,sRowStr.Pos(",")) ;
            _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;
        }

        iColCnt++ ;
        sItmStr = sRowStr.SubString(1,sRowStr.Pos("\r\n" - 1 )) ;
        sRowStr.Delete(1,sRowStr.Pos("\r\n" - 1 )) ;
        _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;

//        _sgLotInfo -> Cells[iColCnt][iRowCnt] = sRowStr ;

        iRowCnt++;
    }
    _sgLotInfo -> FixedCols = 1 ;
}


void __fastcall CLot::DispLotDate(AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgLotInfo)
{
    AnsiString sPath ;
    AnsiString sFPath , sFile ,sData = "";
    AnsiString sTemp;
    int iRowCnt , iColCnt ;
    int iR , iFCol = -1 , iLCol = -1 ;
    int hwnd, flen;
    char *pfbuf;

    if(sFDataTime.ToIntDef(-1) > sUDataTime.ToIntDef(-2)) return ;

    for(int i =0 ; i < _sgLotInfo->ColCount ; i++ ) {
        for(int j =1 ; j < _sgLotInfo->RowCount ; j++ ) {
            _sgLotInfo -> Cells[i][j] = "" ;
        }
    }

    _sgLotInfo -> ColCount = 18 ;

    _sgLotInfo -> DefaultColWidth  = 80 ;
    _sgLotInfo -> DefaultRowHeight = 25 ;
    _sgLotInfo -> ColWidths[0] = 30  ;
    _sgLotInfo -> ColWidths[1] = 145 ;
    _sgLotInfo -> ColWidths[2] = 210 ;

    m_pLotFolder = new TStringGrid(Application);
    m_pLotFile   = new TStringGrid(Application);

    sPath = LOG_FOLDER ;
    UserFile.GridSearchDir(sPath , m_pLotFolder , 0 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
    iRowCnt = m_pLotFolder->RowCount ;

    for(int i=0 ; i<iRowCnt ; i++){
        if( m_pLotFolder->Cells[1][i] == sFDataTime ) iFCol = i ;
        if( m_pLotFolder->Cells[1][i] == sUDataTime ) iLCol = i ;
    }

    if(iFCol == -1    ) {
        for(int i=0 ; i<iRowCnt ; i++){
            if(m_pLotFolder->Cells[1][i].ToIntDef(0) > sFDataTime.ToIntDef(0) && m_pLotFolder->Cells[1][i].ToIntDef(0) <= sUDataTime.ToIntDef(0)) {
                iFCol = i ;
                break;
            }
        }
    }

    if(iLCol == -1    ) {
        for(int i=iRowCnt -1 ; i >= 1 ; i--){
            if(m_pLotFolder->Cells[1][i].ToIntDef(0) < sUDataTime.ToIntDef(0) && m_pLotFolder->Cells[1][i].ToIntDef(0) >= sFDataTime.ToIntDef(0)) {
                iLCol = i ;
                break;
            }
        }
    }

    if(iFCol == -1    ) return ;
    if(iLCol == -1    ) return ;


    iR = iLCol - iFCol ;
    AnsiString sTemp1,sTemp2;
    for(int i=iFCol ; i<=iLCol ; i++){
        sFPath= sPath + m_pLotFolder->Cells[1][i] + "\\";
        UserFile.GridSearchFile(sFPath , m_pLotFile , 1 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
        for(int j=0 ; j<m_pLotFile->RowCount ; j++) {
            sTemp1 = m_pLotFile  ->Cells[1][j]          ;
            sFile += sFPath + sTemp1 + ",";
        }

    }

//    for(int i=iFCol ; i<=iLCol ; i++){
    AnsiString sFileSub ;
    while(sFile.Pos(","))
    {
        sFileSub = sFile.SubString(1,sFile.Pos(",")-1);
        sFile.Delete(1, sFile.Pos(",")  );
        sFPath= sFileSub;

        hwnd = FileOpen(sFPath.c_str(), fmOpenRead) ;
        if(hwnd == NULL) return ;

        flen = FileSeek(hwnd,0,2);
        FileSeek(hwnd,0,0);
        pfbuf = new char[flen+1];
        memset(pfbuf , 0 , sizeof(char)*(flen+1));
        FileRead(hwnd, pfbuf, flen);
        FileClose(hwnd);

        sTemp = pfbuf ;
        sTemp.Delete(1,sTemp.Pos("\r\n")+1);
        sData += sTemp;
    }
//    }

//    int iRowCnt = 0, iColCnt =0 ;
    AnsiString sRowStr, sItmStr ;

    iRowCnt = 1;
    while(sData.Pos("\r\n")){
        sRowStr = sData.SubString(1, sData.Pos("\r\n")) ;
        sData.Delete(1,sData.Pos("\r\n")+1) ;
        iColCnt = 0 ;
        _sgLotInfo->Cells[iColCnt][iRowCnt] = iRowCnt ;
        while (sRowStr.Pos(",")){
            iColCnt++ ;
            sItmStr = sRowStr.SubString(1,sRowStr.Pos(",")-1) ;
            sRowStr.Delete(1,sRowStr.Pos(",")) ;
            _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;
        }
        iColCnt++ ;
        sItmStr = sRowStr.SubString(1,sRowStr.Pos("\r\n" - 1 )) ;
        sRowStr.Delete(1,sRowStr.Pos("\r\n" - 1 )) ;
        _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;

        iRowCnt ++ ;
    }
    _sgLotInfo -> RowCount = iRowCnt ;
    return ;

}

void __fastcall CLot::SaveLotData (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgLotInfo)
{
    AnsiString sPath , sTemp , sData , sFPath;
    int hFile;
    int hwnd, flen;

    if(sFDataTime.ToIntDef(-1) > sUDataTime.ToIntDef(-2)) return ;

    TDateTime  CurrDateTime = Now();;

    //Set Path.
    sTemp = "LotInfo" ;
    sPath = SPC_FOLDER + sTemp;//".csv";

    //Set File Path.
    sFPath = sPath + "\\" + sFDataTime + "-" + sUDataTime + ".csv";

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    //Set Data.
    sData = sFDataTime + "," + sUDataTime + "," + CurrDateTime.CurrentDateTime().FormatString("yyyymmdd(hh:nn)") + "\r\n";

    for(int i=0; i<_sgLotInfo->RowCount; i++){
        for(int j=0; j<_sgLotInfo->ColCount; j++){
            sData += _sgLotInfo -> Cells[j][i] + "," ;
        }
        sData += "\r\n" ;
    }

    hFile = FileOpen(sFPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sFPath.c_str());
        if (hFile == -1) { Trace("Err",(sFPath + "is Can't made").c_str());  return ; }
    }

    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);

    return ;
}
void __fastcall CLot::DispLotDayDate (TStringGrid * _sgLotName , TStringGrid * _sgLotInfo)
{
    AnsiString sPath  = LOG_FOLDER ;
    AnsiString sFPath = LOG_FOLDER + _sgLotName -> Cells[1][ _sgLotName -> Row] ;

    for(int i =0 ; i < _sgLotInfo->ColCount ; i++ ) {
        for(int j =1 ; j < _sgLotInfo->RowCount ; j++ ) {
            _sgLotInfo -> Cells[i][j] = "" ;
        }
    }

    _sgLotInfo -> ColCount = 13;//29 ;
    _sgLotInfo -> DefaultColWidth  = 80 ;
    _sgLotInfo -> DefaultRowHeight = 25 ;
    _sgLotInfo -> ColWidths[0] = 30  ;
    _sgLotInfo -> ColWidths[1] = 145 ;
    _sgLotInfo -> ColWidths[2] = 90  ;

    AnsiString  strName, str = "";
    AnsiString  sRowStr = "";
    AnsiString  sItmStr = "";

    int hwnd, flen;
    int iRowCnt = 0 ;
    int iColCnt = 0 ;
    char    *pfbuf;

    hwnd = FileOpen(sFPath.c_str(), fmOpenRead) ;
    if(hwnd == NULL) return;

    flen = FileSeek(hwnd,0,2);
    FileSeek(hwnd,0,0);
    pfbuf = new char[flen+1];
    memset(pfbuf , 0 , sizeof(char)*(flen+1));
    FileRead(hwnd, pfbuf, flen);
    FileClose(hwnd);

    str = pfbuf ;

    delete [] pfbuf;
    pfbuf = NULL;

    iRowCnt = 0 ;

    while (str.Pos("\r\n")) {
        sRowStr = str.SubString(1 , str.Pos("\r\n")) ;
        str.Delete(1,str.Pos("\r\n")+1) ;
        iColCnt = 0 ;
        _sgLotInfo -> Cells[iColCnt][iRowCnt] = iRowCnt  ;
        while (sRowStr.Pos(",")){
            iColCnt++ ;
            sItmStr = sRowStr.SubString(1,sRowStr.Pos(",")-1) ;
            sRowStr.Delete(1,sRowStr.Pos(",")) ;
            _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;
        }

        iColCnt++ ;
        sItmStr = sRowStr.SubString(1,sRowStr.Pos("\r\n" - 1 )) ;
        sRowStr.Delete(1,sRowStr.Pos("\r\n" - 1 )) ;
        _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;

        iRowCnt++;
    }
    _sgLotInfo -> RowCount = iRowCnt ;    
}
void __fastcall CLot::DispLotDayDate (AnsiString sFDataTime    , AnsiString sUDataTime , TStringGrid * _sgLotInfo)
{
    AnsiString sPath;
    AnsiString sFPath , sFile ,sData = "";
    AnsiString sTemp;
    int iRowCnt , iColCnt ;
    int iR , iFCol = -1 , iLCol = -1 ;
    int hwnd, flen;
    char *pfbuf;

    if(sFDataTime.ToIntDef(-1) > sUDataTime.ToIntDef(-2)) return ;

    for(int i =0 ; i < _sgLotInfo->ColCount ; i++ ) {
        for(int j =1 ; j < _sgLotInfo->RowCount ; j++ ) {
            _sgLotInfo -> Cells[i][j] = "" ;
        }
    }

    _sgLotInfo -> ColCount = 13;//33 ;

    _sgLotInfo -> DefaultColWidth  = 80 ;
    _sgLotInfo -> DefaultRowHeight = 25 ;
    _sgLotInfo -> ColWidths[0] = 30  ;
    _sgLotInfo -> ColWidths[1] = 145 ;
    _sgLotInfo -> ColWidths[2] = 90  ;

    m_pLotFile   = new TStringGrid(Application);

    sPath = LOG_FOLDER ;

    UserFile.GridSearchFile(sPath , m_pLotFile , 0 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬

    iRowCnt = m_pLotFile->RowCount ;

//    sFDataTime += ".csv" ;
//    sUDataTime += ".csv" ;

    for(int i=0 ; i<iRowCnt ; i++){
        if( m_pLotFile->Cells[1][i].SubString(0,8) == sFDataTime ) iFCol = i ;
        if( m_pLotFile->Cells[1][i].SubString(0,8) == sUDataTime ) iLCol = i ;
    }

    if(iFCol == -1    ) {
        for(int i=0 ; i<iRowCnt ; i++){
            sTemp = m_pLotFile->Cells[1][i].SubString(0,8);
            if(sTemp.ToIntDef(0) > sFDataTime.ToIntDef(0) && sTemp.ToIntDef(0) <= sUDataTime.ToIntDef(0)) {
                iFCol = i ;
                break;
            }
        }
    }

    if(iLCol == -1    ) {
        for(int i=iRowCnt -1 ; i >= 1 ; i--){
            sTemp = m_pLotFile->Cells[1][i].SubString(0,8);
            if(sTemp.ToIntDef(0) < sUDataTime.ToIntDef(0) && sTemp.ToIntDef(0) >= sFDataTime.ToIntDef(0)) {
                iLCol = i ;
                break;
            }
        }
    }

    if(iFCol == -1    ) return ;
    if(iLCol == -1    ) return ;

    AnsiString sTemp1,sTemp2;
    for(int i=iFCol ; i<=iLCol ; i++){
        sFPath= sPath + m_pLotFile->Cells[1][i] ;
        sFile += sFPath + ",";
    }

    if(iFCol == -1    ) {
        for(int i=0 ; i<iRowCnt ; i++){
            if(m_pLotFile->Cells[1][i].ToIntDef(0) > sFDataTime.ToIntDef(0) && m_pLotFile->Cells[1][i].ToIntDef(0) <= sUDataTime.ToIntDef(0)) {
                iFCol = i ;
                break;
            }
        }
    }

//    for(int i=iFCol ; i<=iLCol ; i++){
    AnsiString sFileSub ;
    while(sFile.Pos(","))
    {
        sFileSub = sFile.SubString(1,sFile.Pos(",")-1);
        sFile.Delete(1, sFile.Pos(",")  );
        sFPath= sFileSub;

        hwnd = FileOpen(sFPath.c_str(), fmOpenRead) ;
        if(hwnd == NULL) continue ;

        flen = FileSeek(hwnd,0,2);
        FileSeek(hwnd,0,0);
        pfbuf = new char[flen+1];
        memset(pfbuf , 0 , sizeof(char)*(flen+1));
        FileRead(hwnd, pfbuf, flen);
        FileClose(hwnd);

        sTemp = pfbuf ;
        sTemp.Delete(1,sTemp.Pos("\r\n")+1);
        sData += sTemp;
    }

    delete [] pfbuf;
    pfbuf = NULL;
//    }

//    int iRowCnt = 0, iColCnt =0 ;
    AnsiString sRowStr, sItmStr ;

    iRowCnt = 1;
    while(sData.Pos("\r\n")){
        sRowStr = sData.SubString(1, sData.Pos("\r\n")) ;
        sData.Delete(1,sData.Pos("\r\n")+1) ;
        iColCnt = 0 ;
        _sgLotInfo->Cells[iColCnt][iRowCnt] = iRowCnt ;
        while (sRowStr.Pos(",")){
            iColCnt++ ;
            sItmStr = sRowStr.SubString(1,sRowStr.Pos(",")-1) ;
            sRowStr.Delete(1,sRowStr.Pos(",")) ;
            _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;
        }
        iColCnt++ ;
        sItmStr = sRowStr.SubString(1,sRowStr.Pos("\r\n" - 1 )) ;
        sRowStr.Delete(1,sRowStr.Pos("\r\n" - 1 )) ;
        _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;

        iRowCnt ++ ;
    }
    _sgLotInfo -> RowCount = iRowCnt ;
    return ;
}
void __fastcall CLot::SaveLotDayData (AnsiString sFDataTime    , AnsiString sUDataTime , TStringGrid * _sgLotInfo)
{
    AnsiString sPath , sTemp , sData , sFPath;
    int hFile;
    int hwnd, flen;

    if(sFDataTime.ToIntDef(-1) > sUDataTime.ToIntDef(-2)) return ;

    TDateTime  CurrDateTime = Now();;

    //Set Path.
    sTemp = "LotDayInfo" ;
    sPath = SPC_FOLDER + sTemp;//".csv";

    //Set File Path.
    sFPath = sPath + "\\" + sFDataTime + "-" + sUDataTime + ".csv";

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    //Set Data.
    sData = sFDataTime + "," + sUDataTime + "," + CurrDateTime.CurrentDateTime().FormatString("yyyymmdd(hh:nn)") + "\r\n";

    for(int i=0; i<_sgLotInfo->RowCount; i++){
        for(int j=0; j<_sgLotInfo->ColCount; j++){
            sData += _sgLotInfo -> Cells[j][i] + "," ;
        }
        sData += "\r\n" ;
    }

    hFile = FileOpen(sFPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sFPath.c_str());
        if (hFile == -1) { Trace("Err",(sFPath + "is Can't made").c_str());  return ; }
    }

    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);

    return ;
}

bool __fastcall CLot::WriteArayData(EN_ARAY_ID riId ) //매거진 1개 일때만 랏드당 매거진 2개 이상 고려 안함.
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


    DelPastLotLog();

    //Set Path.
//         if(riId ==  riWRK ) sArayName = "Buffer";
//    else if(riId ==  riWK1 ) sArayName = "Vision1";
//    else if(riId ==  riWK2 ) sArayName = "Vision2";
//    else if(riId ==  riPSB ) sArayName = "Trim";

    if( DM.ARAY[riId].GetLotNo() == "" ) sLotNo = "NoLot";
    else                                 sLotNo = DM.ARAY[riId].GetLotNo();

    iSlotNo = DM.ARAY[riId].GetID().ToIntDef(9999)%100 ;
    iMgzNo  = DM.ARAY[riId].GetID().ToIntDef(9999)/100 ;

    sPath = LOG_FOLDER + CurrDateTime.CurrentDate().FormatString("yyyymmdd") + "\\" + sLotNo + "\\" + (String)iMgzNo ;

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    sPath   = sPath + "\\" + (String)iSlotNo + ".ini" ;

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

    UserINI.Save(sPath.c_str()  , "ETC"  , "ID"          , DM.ARAY[riId].GetID().ToIntDef(9999) );
    UserINI.Save(sPath.c_str()  , "ETC"  , "LotNo"       , DM.ARAY[riId].GetLotNo()             );
    UserINI.Save(sPath.c_str()  , "ETC"  , "Row"         , OM.DevInfo.iRowCnt                   );
    UserINI.Save(sPath.c_str()  , "ETC"  , "Col"         , OM.DevInfo.iColCnt                   );

    return true ;
}

bool __fastcall CLot::ReadArayData (AnsiString sPath , EN_ARAY_ID riId) //매거진 1개 일때만 랏드당 매거진 2개 이상 고려 안함.
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

    DM.ARAY[riSPC].SetMaxColRow(iCol,iRow );

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

bool __fastcall CLot::WriteBinInfo(AnsiString _sLotNo, int _iBinNo0,
                                                       int _iBinNo1,
                                                       int _iBinNo2,
                                                       int _iBinNo3,
                                                       int _iBinNo4,
                                                       int _iBinNo5,
                                                       int _iBinNo6,
                                                       int _iBinNo7,
                                                       int _iBinNo8,
                                                       int _iBinNo9)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  , sArayName ;
    AnsiString sLotNo , sMgzNo , sSlotNo;
    TDateTime  CurrDateTime;
    int        iMgzNo , iSlotNo ;
    double     dTemp  ;
    AnsiString sBinName ;

    //Set Path.
    sPath = LOG_FOLDER + CurrDateTime.CurrentDate().FormatString("yyyymmdd") + "\\" + _sLotNo + "\\" ;

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    sPath   = sPath + "BinLog.ini" ;

    UserINI.Save(sPath.c_str()  , "BinLog"  , "BinNo0" , _iBinNo0 );
    UserINI.Save(sPath.c_str()  , "BinLog"  , "BinNo1" , _iBinNo1 );
    UserINI.Save(sPath.c_str()  , "BinLog"  , "BinNo2" , _iBinNo2 );
    UserINI.Save(sPath.c_str()  , "BinLog"  , "BinNo3" , _iBinNo3 );
    UserINI.Save(sPath.c_str()  , "BinLog"  , "BinNo4" , _iBinNo4 );
    UserINI.Save(sPath.c_str()  , "BinLog"  , "BinNo5" , _iBinNo5 );
    UserINI.Save(sPath.c_str()  , "BinLog"  , "BinNo6" , _iBinNo6 );
    UserINI.Save(sPath.c_str()  , "BinLog"  , "BinNo7" , _iBinNo7 );
    UserINI.Save(sPath.c_str()  , "BinLog"  , "BinNo8" , _iBinNo8 );
    UserINI.Save(sPath.c_str()  , "BinLog"  , "BinNo9" , _iBinNo9 );

    MESInfo.iBin0 = _iBinNo0;
    MESInfo.iBin1 = _iBinNo1;
    MESInfo.iBin2 = _iBinNo2;
    MESInfo.iBin3 = _iBinNo3;
    MESInfo.iBin4 = _iBinNo4;
    MESInfo.iBin5 = _iBinNo5;
    MESInfo.iBin6 = _iBinNo6;
    MESInfo.iBin7 = _iBinNo7;
    MESInfo.iBin8 = _iBinNo8;
    MESInfo.iBin9 = _iBinNo9;

    return true ;
}

bool __fastcall CLot::ReadBinInfo(TStringGrid * _sgDate, TStringGrid * _sgLot , int *_iBinNo0,
                                                                                int *_iBinNo1,
                                                                                int *_iBinNo2,
                                                                                int *_iBinNo3,
                                                                                int *_iBinNo4,
                                                                                int *_iBinNo5,
                                                                                int *_iBinNo6,
                                                                                int *_iBinNo7,
                                                                                int *_iBinNo8,
                                                                                int *_iBinNo9)


{
    TUserINI   UserINI;

    AnsiString sDay  = _sgDate -> Cells[1][ _sgDate -> Row] ;
    AnsiString sPath = LOG_FOLDER + sDay + "\\" + _sgLot -> Cells[1][ _sgLot -> Row] ;
    sPath.Delete(sPath.Pos(".csv"),4);
    sPath = sPath + "\\" + "BinLog.ini";

    if(!FileExists(sPath)) return false;

    UserINI.Load(sPath.c_str()  , "BinLog"  , "BinNo0" , _iBinNo0 );
    UserINI.Load(sPath.c_str()  , "BinLog"  , "BinNo1" , _iBinNo1 );
    UserINI.Load(sPath.c_str()  , "BinLog"  , "BinNo2" , _iBinNo2 );
    UserINI.Load(sPath.c_str()  , "BinLog"  , "BinNo3" , _iBinNo3 );
    UserINI.Load(sPath.c_str()  , "BinLog"  , "BinNo4" , _iBinNo4 );
    UserINI.Load(sPath.c_str()  , "BinLog"  , "BinNo5" , _iBinNo5 );
    UserINI.Load(sPath.c_str()  , "BinLog"  , "BinNo6" , _iBinNo6 );
    UserINI.Load(sPath.c_str()  , "BinLog"  , "BinNo7" , _iBinNo7 );
    UserINI.Load(sPath.c_str()  , "BinLog"  , "BinNo8" , _iBinNo8 );
    UserINI.Load(sPath.c_str()  , "BinLog"  , "BinNo9" , _iBinNo9 );
}

void __fastcall CLot::UpdateMESInfo ()
{
    MESInfo.dStartTime = LotInfo.dStartTime;
    MESInfo.dEndTime   = LotInfo.dEndTime  ;
    MESInfo.sJobFile   = LotInfo.sJobFile  ;
    MESInfo.sLotNo     = LotInfo.sLotNo    ;
    MESInfo.sOperator  = LotInfo.sOperator ;
    MESInfo.iLotMgzCnt = LotInfo.iLotMgzCnt;
    MESInfo.iWorkStrp  = LotInfo.iWorkStrp ;
    MESInfo.iWorkChip  = LotInfo.iWorkChip ;
    MESInfo.iGoodChip  = LotInfo.iWorkChip - LotInfo.iFailChip ;
    MESInfo.iFailChip  = LotInfo.iFailChip ;
}

void __fastcall CLot::WriteMESInfo()
{
    //Local Var.
    int        hFile   ;
    AnsiString Path    ;                                                                                                                     
    AnsiString Temp    ;
    TDateTime  CurrDateTime;

    DelPastLotLog();

    //Set Path.
    CurrDateTime = Now();
    //Temp         = CurrDateTime.CurrentDate().FormatString("yyyymmdd");
    Path         = OM.CmnOptn.sMESPath + "\\" ;//+ Temp ;//".csv";



    if(!UserFile.FileExist(Path)) CreateDir(Path);

    TDateTime tTemp ;
    String    sLotNo ;
    tTemp.Val = LotInfo.dStartTime ;

    int iGoodChip= 0;

    if(LotInfo.sLotNo == "") sLotNo = "NoLot" ;
    else                     sLotNo = MESInfo.sLotNo ;

    Path = OM.CmnOptn.sMESPath + "\\" + Temp + "\\" + sLotNo + ".csv";
//    Path = LOG_FOLDER + Temp + ".csv";
    //File Open.
    hFile = FileOpen(Path.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(Path.c_str());
        if (hFile == -1) return;
        Temp  = "작업일자,";
        Temp += "Lot. 시작시간,";
        Temp += "Lot. 종료시간,"  ;
        Temp += "Program,"  ;
        Temp += "Lot. Card,"    ;
        Temp += "Operator," ;
        //Temp += "WorkStrp," ;  총L/F수량???
        Temp += "완료 L/F 수량," ;
        Temp += "측정 수량," ;
        Temp += "양품 수," ;
        Temp += "불량 수," ;
        Temp += "Bin1,"     ;
        Temp += "Bin2,"     ;
        Temp += "Bin3,"     ;
        Temp += "Bin4,"     ;
        Temp += "Bin5,"     ;
        Temp += "Bin6,"     ;
        Temp += "Bin7,"     ;
        Temp += "Bin8,"     ;
        Temp += "Bin9,"     ;
        Temp += "Bin10,"     ;
//        Temp += "Rslt4,"    ;
//        Temp += "Rslt5,"    ;
        Temp += "\r\n"      ;

        FileSeek (hFile , 0            , SEEK_END     );
        FileWrite(hFile , Temp.c_str() , Temp.Length());
    }

//    TDateTime tTemp ;


    //Save.
    Temp  = CurrDateTime.FormatString("yy.mm.dd") + "," ;
    tTemp.Val = MESInfo.dStartTime ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = MESInfo.dEndTime   ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    Temp += (AnsiString)MESInfo.sJobFile     + ",";
    Temp += (AnsiString)MESInfo.sLotNo       + ",";
    Temp += (AnsiString)MESInfo.sOperator    + ",";
    //Temp += (AnsiString)LotInfo.iWorkStrp    + ",";   총 L/F수량??
    Temp += (AnsiString)MESInfo.iWorkStrp    + ",";
    Temp += (AnsiString)MESInfo.iWorkChip    + ",";

    iGoodChip = LotInfo.iWorkChip - LotInfo.iFailChip;
    
    Temp += (AnsiString)MESInfo.iGoodChip    + ",";
    Temp += (AnsiString)MESInfo.iFailChip    + ",";

    Temp += (AnsiString)MESInfo.iBin0        + ",";
    Temp += (AnsiString)MESInfo.iBin1        + ",";
    Temp += (AnsiString)MESInfo.iBin2        + ",";
    Temp += (AnsiString)MESInfo.iBin3        + ",";
    Temp += (AnsiString)MESInfo.iBin4        + ",";
    Temp += (AnsiString)MESInfo.iBin5        + ",";
    Temp += (AnsiString)MESInfo.iBin6        + ",";
    Temp += (AnsiString)MESInfo.iBin7        + ",";
    Temp += (AnsiString)MESInfo.iBin8        + ",";
    Temp += (AnsiString)MESInfo.iBin9        + ",";


    Temp += (AnsiString)"\r\n"                    ;

    FileSeek (hFile , 0            , SEEK_END     );
    FileWrite(hFile , Temp.c_str() , Temp.Length());

    //Close File.
    FileClose(hFile);

    //bMESEnded 초기화하자.
    bMESEnded = false;
}

