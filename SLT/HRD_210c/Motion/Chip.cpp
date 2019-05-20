//---------------------------------------------------------------------------
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Chip.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//CRITICAL_SECTION g_csChip ;

//---------------------------------------------------------------------------
__fastcall CChip::CChip(void)
{
    static bool bFirst = true ;

    if(bFirst) {
        //InitializeCriticalSection(&g_csChip);
        bFirst = false ;
    }


    Init();
}

//---------------------------------------------------------------------------
__fastcall CChip::~CChip(void)
{
    static bool bFirst = true ;

    if(bFirst) {
        //DeleteCriticalSection(&g_csChip);
        bFirst = false ;
    }

}

//---------------------------------------------------------------------------
void __fastcall CChip::Init(void)
{

    m_iStat = csNone;
}


//---------------------------------------------------------------------------
void __fastcall CChip::Load(bool IsLoad , FILE *fp)
{



//try {
//EnterCriticalSection(&g_csChip) ;
    if (IsLoad) {
        fread (&m_iStat    , sizeof(m_iStat    ) , 1 , fp);
    }

    else {
        fwrite(&m_iStat    , sizeof(m_iStat    ) , 1 , fp);
    }
//}
//__finally{
//LeaveCriticalSection(&g_csChip);
//}

}



//Check Chip Status & Result.
bool __fastcall CChip::CheckStat(EN_CHIP_STAT _iStat)
{
//try {
//EnterCriticalSection(&g_csChip) ;
    return (m_iStat == _iStat);
//}
//__finally{
//LeaveCriticalSection(&g_csChip);
//}
}

bool __fastcall CChip::CheckExist(void )
{
//try {
//EnterCriticalSection(&g_csChip) ;
    return (m_iStat != csNone && m_iStat != csMask && m_iStat != csEmpty);
//}
//__finally{
//LeaveCriticalSection(&g_csChip);
//}
}

bool __fastcall CChip::CheckRst(void)
{
//try {
//EnterCriticalSection(&g_csChip) ;
    return (m_iStat != csNone && m_iStat != csUnkwn && m_iStat != csWait && m_iStat != csEror && m_iStat != csMask  && m_iStat != csWork && m_iStat != csGood );
//}
//__finally{
//LeaveCriticalSection(&g_csChip);
//}
}

//Set
void __fastcall CChip::SetStat(EN_CHIP_STAT _iStat)
{
//try {
//EnterCriticalSection(&g_csChip) ;
    m_iStat = _iStat ;
//}
//__finally{
//LeaveCriticalSection(&g_csChip);
//}
}

//Get
EN_CHIP_STAT __fastcall CChip::GetStat()
{
//try {
//EnterCriticalSection(&g_csChip) ;
    return m_iStat ;
//}
//__finally{
//LeaveCriticalSection(&g_csChip);
//}
}


