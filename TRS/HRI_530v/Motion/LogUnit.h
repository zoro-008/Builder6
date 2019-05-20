#ifndef LogUnitH
#define LogUnitH

#include <stdio.h>
#include "UtilDefine.h"

class CLogUnit
{
    public:
        int m_iMarkCnt;
        AnsiString m_sPreString ;

    public:
        __fastcall  CLogUnit(void);
        __fastcall ~CLogUnit(void);

        void __fastcall Init(void);
        void __fastcall KillPastLog(void);

        bool __fastcall HanraVs(void);
        bool __fastcall DmLog(EN_ARAY_ID riId);

        void __fastcall Trace (AnsiString Msg);
};

//---------------------------------------------------------------------------
extern CLogUnit LOG;
//---------------------------------------------------------------------------
#endif
