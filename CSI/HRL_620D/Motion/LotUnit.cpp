//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "LotUnit.h"
#include "SLogUnit.h"
#include "UserIni.h"
#include "UserFile.h"
//#include "OptionMan.h"
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
    ClearDayInfo();


    LoadDayInfo();
    LoadEqpInfo();

    if(!FileExists(LOG_FOLDER)) CreateDir(LOG_FOLDER);


}
__fastcall CLot::~CLot(void)
{
    //메인폼으로 이동.
    //Close();
}

void CLot::Close()
{
    SaveEqpInfo();
    SaveDayInfo();
}

void CLot::Init()
{

}

void __fastcall CLot::ClearEqpInfo(void)
{
    memset(&EqpInfo , 0 , sizeof(SEqpInfo));
}

void __fastcall CLot::ClearDayInfo(void)
{
    memset(&DayInfo , 0 , sizeof(SDayInfo));
}


//Lot Processing.
void __fastcall CLot::LotOpen (AnsiString _sLotNo , AnsiString _sJobFile , AnsiString _sOperator)
{




    m_bLotOpen  = true ;
}


void __fastcall CLot::LotEnd (void)
{
    //Check already opened Lot.
    if (!m_bLotOpen) return;

    //Set EndTime.
    WriteLotDayLog();

    //Reset Lot Flag.
    m_bLotOpen = false;

//    LDR._iMgzCnt = 0 ; //안대서 추가 하긴 하는데 이게 여기가 맞는지 모르겟넹.

    //WriteLotLog();

    //Check Retest Mode.
    Trace("SEQ","Lot Work Finished"   );
    FM_MsgOk("Confirm" , "랏이 종료 됐습니다."   );

    //랏엔드 부져 부져가
    m_bLotEnd = true ;

}

void __fastcall CLot::DispEqpInfo (TPanel * _pnTotalTime , TPanel * _pnTodayWork)
{
    TDateTime tTemp ;
    tTemp.Val = EqpInfo.dRunTime ;

    


    if(_pnTotalTime) _pnTotalTime -> Caption = tTemp.FormatString("hh:nn:ss") ;
    if(_pnTodayWork) _pnTodayWork -> Caption = EqpInfo.iTodayWork ;



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
    UserINI.Load(Path.c_str() , "EqpInfo" , "dRunTime  " , EqpInfo.dRunTime   );
    UserINI.Load(Path.c_str() , "EqpInfo" , "iTodayWork" , EqpInfo.iTodayWork );
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
    UserINI.Save(Path.c_str() , "EqpInfo" , "dRunTime  " , EqpInfo.dRunTime   );
    UserINI.Save(Path.c_str() , "EqpInfo" , "iTodayWork" , EqpInfo.iTodayWork );

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
    UserINI.Load(Path.c_str() , "DayInfo" , "dWorkedDay    " , DayInfo.sWorkedDay    );
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
    UserINI.Save(Path.c_str() , "DayInfo" , "dWorkedDay    " , DayInfo.sWorkedDay    );
}




//File Processing.

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
        if (hFile == -1){
            EqpInfo.dRunTime = 0.0;
            return;
        }
        Temp  = "Day,"   ;
        Temp += "TotalTime," ;
        Temp += "TotalWork," ;
        Temp += "TodayWork," ;
        Temp += "\r\n"       ;
        FileSeek (hFile , 0            , SEEK_END     );
        FileWrite(hFile , Temp.c_str() , Temp.Length());

    }

    TDateTime tTemp ;

    //Save.
    Temp  = CurrDateTime.FormatString("yy.mm.dd") + "," ;

    tTemp.Val = EqpInfo.dRunTime   ; Temp += tTemp.FormatString("hh:nn:ss") + ",";

    Temp += (AnsiString)EqpInfo.iTodayWork + ",";
    Temp += (AnsiString)"\r\n"                    ;

    FileSeek (hFile , 0            , SEEK_END     );
    FileWrite(hFile , Temp.c_str() , Temp.Length());

    //Close File.
    FileClose(hFile);
}

void __fastcall CLot::DelPastLotLog()
{
    UserFile.ClearDirDate(LOG_FOLDER , Now() - 90);
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
        case ssRunning  : EqpInfo.dRunTime += dGapTime ; break ;
    }

    

    tPreTime = tCrntTime ;


}


