/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Usage  :                                                                */
/* Date   : 2004.11.24                                                     */
/* Modify : 2004.11.24                                                     */
/* Programmed by Primer(YongSik, Lee).                                     */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

//---------------------------------------------------------------------------
#ifndef LogUnitH
#define LogUnitH

//---------------------------------------------------------------------------
#include <stdio.h>

#define LOG_FOLDER "d:\\DataLog\\"

struct TLogData {
    AnsiString Serial     ;
    AnsiString Result     ;
    AnsiString Table      ;
    AnsiString StartTime  ;
    AnsiString AngleX     ;
    AnsiString AngleY     ;
    AnsiString Shift      ;
    AnsiString APC        ;
    AnsiString ReResult   ;
    AnsiString ReAngleX   ;
    AnsiString ReAngleY   ;
    AnsiString ReShift    ;
    AnsiString ReAPC      ;
    AnsiString EndTime    ;
    AnsiString WorkTime   ;

    AnsiString WaitTime   ;
    AnsiString InspTime   ;
    AnsiString BondTime   ;
    AnsiString UVTime     ;

    AnsiString TotalYield ;

};

struct TServerLogData {
    AnsiString Angle             ;
    AnsiString FA_R              ;
    AnsiString FA_T              ;
    AnsiString Three_Beam_Tilt   ;
    AnsiString Beam_Size         ;
    AnsiString GT_Shift          ;
    AnsiString Area              ;
    AnsiString Peak_Ratio        ;
    AnsiString Parallelism       ;
    AnsiString Absolute_Center_X ;
    AnsiString Absolute_Center_Y ;
    AnsiString Beam_Center_X     ;
    AnsiString Beam_Center_Y     ;
    AnsiString Peak_Center_X     ;
    AnsiString Peak_Center_Y     ;
    AnsiString Beam_Shift        ;
    AnsiString Beam_Shift_R      ;
    AnsiString Beam_Shift_T      ;
    AnsiString LD_Motor_Y        ;
    AnsiString LD_MOtor_Z        ;
    AnsiString GTX_Motor_X       ;

    AnsiString Judge             ;
    AnsiString Serial            ;
    AnsiString First_NG          ;
    AnsiString AdjEqNum          ;
    AnsiString EvalTime          ;
};

//---------------------------------------------------------------------------
class CLogUnit
{
    private:
        AnsiString m_sLogFilePath ;

    public:
        __fastcall  CLogUnit(void);
        __fastcall ~CLogUnit(void);

        TLogData        Data ;
        TServerLogData  ServerLogData ;

        void __fastcall Init(void);
        void __fastcall ClearData(){memset(&Data , 0 , sizeof(TLogData));}

        void __fastcall KillPastLog(void);
        void __fastcall ChangeOper(AnsiString sPath);

        void __fastcall ClearServerData(){memset(&Data , 0 , sizeof(TServerLogData));}
        void __fastcall WriteLog       ();
        void __fastcall WriteServerLog   (bool bLeftTable);
        void __fastcall WriteServerLogCsv(bool bLeftTable);


        void __fastcall Save();
        void __fastcall Load();
};

//---------------------------------------------------------------------------
#endif

