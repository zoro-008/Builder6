//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Chip.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


//---------------------------------------------------------------------------
__fastcall CChip::CChip(void)
{
    Init();
}

//---------------------------------------------------------------------------
__fastcall CChip::~CChip(void)
{
//    Trace( "", "~CChip") ;
}

//---------------------------------------------------------------------------
void __fastcall CChip::Init(void)
{
    m_iStat = csNone;
}


//---------------------------------------------------------------------------
void __fastcall CChip::Load(bool IsLoad , FILE *fp)
{
    if (IsLoad) {
        fread (&m_iStat    , sizeof(m_iStat    ) , 1 , fp);
    }

    else {
        fwrite(&m_iStat    , sizeof(m_iStat    ) , 1 , fp);
    }

}

