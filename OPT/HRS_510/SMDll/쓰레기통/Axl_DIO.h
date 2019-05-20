//---------------------------------------------------------------------------

#ifndef Axl_DIOH
#define Axl_DIOH
//SUN Kye Won 2012.04.27
//---------------------------------------------------------------------------
#include "Define.h"
#ifdef AJIN_AXL
class CAxlDIO
{
    public:
        CAxlDIO();
        virtual ~CAxlDIO();

    protected:
        struct CModuleInfo {
            int iModuleInByteCnt  ;
            int iModuleOutByteCnt ;
        };
        CModuleInfo * m_pModuleInfo ;
        int m_iMaxModuleNo ;

        int m_iMaxIn  ;
        int m_iMaxOut ;

        bool GetInModuleNo (int _iNo , int &_iModuleNo , int &_iOffset);
        bool GetOutModuleNo(int _iNo , int &_iModuleNo , int &_iOffset);

    public:
        bool GetInModuleNoDp (int _iNo , int &_iModuleNo , int &_iOffset); //Display용 모듈 순서
        bool GetOutModuleNoDp(int _iNo , int &_iModuleNo , int &_iOffset);

        void SetOut (int _iNo , bool  _bOn) ;
        bool GetOut (int _iNo             ) ;
        bool GetIn  (int _iNo             ) ;
};

#endif //AJIN_AXL
#endif
