//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "LogUnit.h"
#include "UserFile.h"
#include "UserIni.h"
#include "SLogUnit.h"
#include "OptionMan.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------


#define LEFT_SERVER_LOG_PATH "d:\\LD\\DATA1\\LD.TXT"
#define RGHT_SERVER_LOG_PATH "d:\\LD\\DATA2\\LD.TXT"

__fastcall CLogUnit::CLogUnit(void)
{
    Init();
    Load();
}

//---------------------------------------------------------------------------
__fastcall CLogUnit::~CLogUnit(void)
{
    Save();
}

void __fastcall CLogUnit::Init(void)
{

    //Make Dir.
    if (!DirectoryExists(LOG_FOLDER)) CreateDir(LOG_FOLDER);
}

void __fastcall CLogUnit::KillPastLog(void)
{
    UserFile.ClearDirDate(LOG_FOLDER , Now() - 30);
}

void __fastcall CLogUnit::WriteLog()
{
    //Local Var.
    int        hFile   ;
    AnsiString Path    ;
    AnsiString Temp    ;
    AnsiString Temp2   ;

    KillPastLog();

    //File Open.
    hFile = FileOpen(m_sLogFilePath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(m_sLogFilePath.c_str());
        if (hFile == -1) return;
        Temp  = "1_=Nomal 2_=AfterDispense 3_=GT_Again 4_=After GtUp\r\n" ;

        Temp += "LoggingTime,"  ;
        Temp += "Serial,"       ;
        Temp += "Table,"        ;
        Temp += "1st Result,"   ;
        Temp += "1st StartTime,";
        Temp += "1st AngleX,"   ;
        Temp += "1st AngleY,"   ;
        Temp += "1st Shift,"    ;
        Temp += "1st APC,"      ;
        Temp += "2nd Result,"   ;
        Temp += "2nd AngleX,"   ;
        Temp += "2nd AngleY,"   ;
        Temp += "2nd Shift,"    ;
        Temp += "2nd APC,"      ;
        Temp += "EndTime,"      ;

        Temp += "WorkTime,"      ;
        Temp += "WaitTime,"      ;
        Temp += "InspTime,"      ;
        Temp += "BondTime,"      ;
        Temp += "UVTime,"        ;
        Temp += "TotalYield,"    ;
        Temp += "Mode"           ;
        Temp += "\r\n"           ;

        FileSeek (hFile , 0            , SEEK_END     );
        FileWrite(hFile , Temp.c_str() , Temp.Length());
    }

    switch(OM.DevOptn.iModeSelect) {
        case 0 : Temp2 = "NORMAL"      ; break ;
        case 1 : Temp2 = "RE ADJUST"   ; break ;
        case 2 : Temp2 = "LONG RUN"    ; break ;
        case 3 : Temp2 = "ADJUST ONLY" ; break ;
        case 4 : Temp2 = "DATA CHECK"  ; break ;
        case 5 : Temp2 = "DISP FIRST"  ; break ;
    }







    //Save.
    Temp  = Now().FormatString("hh:mm:ss") + "," ;
    Temp += Data.Serial     + ",";
    Temp += Data.Table      + ",";
    Temp += Data.Result     + ",";
    Temp += Data.StartTime  + ",";
    Temp += Data.AngleX     + ",";
    Temp += Data.AngleY     + ",";
    Temp += Data.Shift      + ",";
    Temp += Data.APC        + ",";
    Temp += Data.ReResult   + ",";
    Temp += Data.ReAngleX   + ",";
    Temp += Data.ReAngleY   + ",";
    Temp += Data.ReShift    + ",";
    Temp += Data.ReAPC      + ",";
    Temp += Data.EndTime    + ",";
    Temp += Data.WorkTime   + ",";
    Temp += Data.WaitTime   + ",";
    Temp += Data.InspTime   + ",";
    Temp += Data.BondTime   + ",";
    Temp += Data.UVTime     + ",";
    Temp += Data.TotalYield + ",";
    Temp += Temp2           + "\r\n" ;

    FileSeek (hFile , 0            , SEEK_END     );
    FileWrite(hFile , Temp.c_str() , Temp.Length());

    //Close File.
    FileClose(hFile);

}

void __fastcall CLogUnit::ChangeOper(AnsiString sPath)
{
    m_sLogFilePath = sPath ;

    Save();
}

void __fastcall CLogUnit::WriteServerLog(bool bLeftTable)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    if(bLeftTable)sPath= LEFT_SERVER_LOG_PATH ;
    else          sPath= RGHT_SERVER_LOG_PATH ;

    AnsiString sPath2 = "d:\\LD_Backup\\DATA" + (AnsiString)(bLeftTable?"1":"2") + "\\LD_Sample.TXT" ;

    //Trace("Server Log " + AnsiString(bLeftTable?"1":"2") , (ServerLogData.Judge + "_" + ServerLogData.First_NG).c_str());

    //Save
    UserINI.Save(sPath, "DVD"    , "Angle            "  , ServerLogData.Angle             ); UserINI.Save(sPath2, "DVD"    , "Angle            "  , ServerLogData.Angle             );
    UserINI.Save(sPath, "DVD"    , "FA_R             "  , ServerLogData.FA_R              ); UserINI.Save(sPath2, "DVD"    , "FA_R             "  , ServerLogData.FA_R              );
    UserINI.Save(sPath, "DVD"    , "FA_T             "  , ServerLogData.FA_T              ); UserINI.Save(sPath2, "DVD"    , "FA_T             "  , ServerLogData.FA_T              );
    UserINI.Save(sPath, "DVD"    , "Three_Beam_Tilt  "  , ServerLogData.Three_Beam_Tilt   ); UserINI.Save(sPath2, "DVD"    , "Three_Beam_Tilt  "  , ServerLogData.Three_Beam_Tilt   );
    UserINI.Save(sPath, "DVD"    , "Beam_Size        "  , ServerLogData.Beam_Size         ); UserINI.Save(sPath2, "DVD"    , "Beam_Size        "  , ServerLogData.Beam_Size         );
    UserINI.Save(sPath, "DVD"    , "GT_Shift         "  , ServerLogData.GT_Shift          ); UserINI.Save(sPath2, "DVD"    , "GT_Shift         "  , ServerLogData.GT_Shift          );
    UserINI.Save(sPath, "DVD"    , "Area             "  , ServerLogData.Area              ); UserINI.Save(sPath2, "DVD"    , "Area             "  , ServerLogData.Area              );
    UserINI.Save(sPath, "DVD"    , "Peak_Ratio       "  , ServerLogData.Peak_Ratio        ); UserINI.Save(sPath2, "DVD"    , "Peak_Ratio       "  , ServerLogData.Peak_Ratio        );
    UserINI.Save(sPath, "DVD"    , "Parallelism      "  , ServerLogData.Parallelism       ); UserINI.Save(sPath2, "DVD"    , "Parallelism      "  , ServerLogData.Parallelism       );
    UserINI.Save(sPath, "DVD"    , "Absolute_Center_X"  , ServerLogData.Absolute_Center_X ); UserINI.Save(sPath2, "DVD"    , "Absolute_Center_X"  , ServerLogData.Absolute_Center_X );
    UserINI.Save(sPath, "DVD"    , "Absolute_Center_Y"  , ServerLogData.Absolute_Center_Y ); UserINI.Save(sPath2, "DVD"    , "Absolute_Center_Y"  , ServerLogData.Absolute_Center_Y );
    UserINI.Save(sPath, "DVD"    , "Beam_Center_X    "  , ServerLogData.Beam_Center_X     ); UserINI.Save(sPath2, "DVD"    , "Beam_Center_X    "  , ServerLogData.Beam_Center_X     );
    UserINI.Save(sPath, "DVD"    , "Beam_Center_Y    "  , ServerLogData.Beam_Center_Y     ); UserINI.Save(sPath2, "DVD"    , "Beam_Center_Y    "  , ServerLogData.Beam_Center_Y     );
    UserINI.Save(sPath, "DVD"    , "Peak_Center_X    "  , ServerLogData.Peak_Center_X     ); UserINI.Save(sPath2, "DVD"    , "Peak_Center_X    "  , ServerLogData.Peak_Center_X     );
    UserINI.Save(sPath, "DVD"    , "Peak_Center_Y    "  , ServerLogData.Peak_Center_Y     ); UserINI.Save(sPath2, "DVD"    , "Peak_Center_Y    "  , ServerLogData.Peak_Center_Y     );
    UserINI.Save(sPath, "DVD"    , "Beam_Shift       "  , ServerLogData.Beam_Shift        ); UserINI.Save(sPath2, "DVD"    , "Beam_Shift       "  , ServerLogData.Beam_Shift        );
    UserINI.Save(sPath, "DVD"    , "Beam_Shift_R     "  , ServerLogData.Beam_Shift_R      ); UserINI.Save(sPath2, "DVD"    , "Beam_Shift_R     "  , ServerLogData.Beam_Shift_R      );
    UserINI.Save(sPath, "DVD"    , "Beam_Shift_T     "  , ServerLogData.Beam_Shift_T      ); UserINI.Save(sPath2, "DVD"    , "Beam_Shift_T     "  , ServerLogData.Beam_Shift_T      );
    UserINI.Save(sPath, "DVD"    , "LD_Motor_Y       "  , ServerLogData.LD_Motor_Y        ); UserINI.Save(sPath2, "DVD"    , "LD_Motor_Y       "  , ServerLogData.LD_Motor_Y        );
    UserINI.Save(sPath, "DVD"    , "LD_MOtor_Z       "  , ServerLogData.LD_MOtor_Z        ); UserINI.Save(sPath2, "DVD"    , "LD_MOtor_Z       "  , ServerLogData.LD_MOtor_Z        );
    UserINI.Save(sPath, "DVD"    , "GTX_Motor_X      "  , ServerLogData.GTX_Motor_X       ); UserINI.Save(sPath2, "DVD"    , "GTX_Motor_X      "  , ServerLogData.GTX_Motor_X       );
    UserINI.Save(sPath, "COMMON" , "Judge            "  , ServerLogData.Judge             ); UserINI.Save(sPath2, "COMMON" , "Judge            "  , ServerLogData.Judge             );
    UserINI.Save(sPath, "COMMON" , "Serial           "  , ServerLogData.Serial            ); UserINI.Save(sPath2, "COMMON" , "Serial           "  , ServerLogData.Serial            );
    UserINI.Save(sPath, "COMMON" , "First_NG         "  , ServerLogData.First_NG          ); UserINI.Save(sPath2, "COMMON" , "First_NG         "  , ServerLogData.First_NG          );
    UserINI.Save(sPath, "COMMON" , "AdjEqNum         "  , ServerLogData.AdjEqNum          ); UserINI.Save(sPath2, "COMMON" , "AdjEqNum         "  , ServerLogData.AdjEqNum          );
    UserINI.Save(sPath, "COMMON" , "EvalTime         "  , ServerLogData.EvalTime          ); UserINI.Save(sPath2, "COMMON" , "EvalTime         "  , ServerLogData.EvalTime          );

    WriteServerLogCsv(bLeftTable);
}

void __fastcall CLogUnit::WriteServerLogCsv(bool bLeftTable)
{
    //Local Var.
    int        hFile   ;
    AnsiString Path = LOG_FOLDER + Now().FormatString("yyyy_mm_dd") + "_ServerLog.csv" ;
    AnsiString Temp    ;

    KillPastLog();

    //File Open.
    hFile = FileOpen(Path.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(Path.c_str());
        if (hFile == -1) return;
        Temp  = "Time,"             ;
        Temp += "Table,"            ;
        Temp += "Angle,"            ;
        Temp += "FA_R,"             ;
        Temp += "FA_T,"             ;
        Temp += "Three_Beam_Tilt,"  ;
        Temp += "Beam_Size,"        ;
        Temp += "GT_Shift,"         ;
        Temp += "Area,"             ;
        Temp += "Peak_Ratio,"       ;
        Temp += "Parallelism,"      ;
        Temp += "Absolute_Center_X,";
        Temp += "Absolute_Center_Y,";
        Temp += "Beam_Center_X,"    ;
        Temp += "Beam_Center_Y,"    ;
        Temp += "Peak_Center_X,"    ;
        Temp += "Peak_Center_Y,"    ;
        Temp += "Beam_Shift,"       ;
        Temp += "Beam_Shift_R,"     ;
        Temp += "Beam_Shift_T,"     ;
        Temp += "LD_Motor_Y,"       ;
        Temp += "LD_MOtor_Z,"       ;
        Temp += "GTX_Motor_X,"      ;
        Temp += "Judge,"            ;
        Temp += "Serial,"           ;
        Temp += "First_NG,"         ;
        Temp += "AdjEqNum,"         ;
        Temp += "EvalTime"          ;
        Temp += "\r\n"              ;

        FileSeek (hFile , 0            , SEEK_END     );
        FileWrite(hFile , Temp.c_str() , Temp.Length());
    }

    //Save.
    Temp  = Now().FormatString("hh:mm:ss") + ","  ;
    Temp += bLeftTable ? "Left," : "Right,"       ;
    Temp += ServerLogData.Angle              + ",";
    Temp += ServerLogData.FA_R               + ",";
    Temp += ServerLogData.FA_T               + ",";
    Temp += ServerLogData.Three_Beam_Tilt    + ",";
    Temp += ServerLogData.Beam_Size          + ",";
    Temp += ServerLogData.GT_Shift           + ",";
    Temp += ServerLogData.Area               + ",";
    Temp += ServerLogData.Peak_Ratio         + ",";
    Temp += ServerLogData.Parallelism        + ",";
    Temp += ServerLogData.Absolute_Center_X  + ",";
    Temp += ServerLogData.Absolute_Center_Y  + ",";
    Temp += ServerLogData.Beam_Center_X      + ",";
    Temp += ServerLogData.Beam_Center_Y      + ",";
    Temp += ServerLogData.Peak_Center_X      + ",";
    Temp += ServerLogData.Peak_Center_Y      + ",";
    Temp += ServerLogData.Beam_Shift         + ",";
    Temp += ServerLogData.Beam_Shift_R       + ",";
    Temp += ServerLogData.Beam_Shift_T       + ",";
    Temp += ServerLogData.LD_Motor_Y         + ",";
    Temp += ServerLogData.LD_MOtor_Z         + ",";
    Temp += ServerLogData.GTX_Motor_X        + ",";
    Temp += ServerLogData.Judge              + ",";
    Temp += ServerLogData.Serial             + ",";
    Temp += ServerLogData.First_NG           + ",";
    Temp += ServerLogData.AdjEqNum           + ",";
    Temp += ServerLogData.EvalTime                ;
    Temp += "\r\n"                                ;

    FileSeek (hFile , 0            , SEEK_END     );
    FileWrite(hFile , Temp.c_str() , Temp.Length());

    //Close File.
    FileClose(hFile);

}
                                                                                                               
void __fastcall CLogUnit::Load()                                                                               
{                                                                                                              
    //Local Var.                                                                                               
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    if (!DirectoryExists(ExtractFilePath(Application->ExeName) + "SeqData"))
        CreateDir(ExtractFilePath(Application->ExeName) + "SeqData");
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Save
    UserINI.Load(sPath, "CLogUnit"    , "m_sLogFilePath"  , m_sLogFilePath );
}

void __fastcall CLogUnit::Save()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    if (!DirectoryExists(ExtractFilePath(Application->ExeName) + "SeqData"))
        CreateDir(ExtractFilePath(Application->ExeName) + "SeqData");
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Save
    UserINI.Save(sPath, "CLogUnit"    , "m_sLogFilePath"  , m_sLogFilePath );
}

