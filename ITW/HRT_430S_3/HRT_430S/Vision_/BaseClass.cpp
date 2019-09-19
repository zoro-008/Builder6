//---------------------------------------------------------------------------


#pragma hdrstop

#include "BaseClass.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)








/*
// XObject PDynamicClass
CPkgStatic *_pEndPkgStatic = NULL;
//CPkgStatic _FirstDynamicClass("__FIRST__", NULL);

CPkgStatic * GetStaticClass(char *pPkgName)
{
    CPkgStatic * pPkgStatic = _pEndPkgStatic;
    while(pPkgStatic->m_pPrev != NULL)
    {
        if(strcmp(pPkgStatic->m_pName, pPkgName) == 0){
            return pPkgStatic;
        }

        pPkgStatic = pPkgStatic->m_pPrev;
    }
    return 0;
}

CPkgStatic::CPkgStatic(char *pPkgName, FCreatePkg fCreatePkg)
{
    strcpy(m_pName, pPkgName);
    m_fCreatePkg = fCreatePkg;
    m_pPrev = _pEndPkgStatic;
    _pEndPkgStatic = this;
}

CBase *CPkgStatic::CreatePkg()
{
    if(m_fCreatePkg) {
        return m_fCreatePkg();
    }

    return NULL;
}

bool CPkgStatic::MatchName(char *pPkgName)
{
  return strcmp(m_pName, pPkgName) == 0;
}
*/
