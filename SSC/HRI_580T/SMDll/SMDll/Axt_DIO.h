//---------------------------------------------------------------------------

#ifndef Axt_DIOH
#define Axt_DIOH
//SUN Kye Won 2010.12.27
//---------------------------------------------------------------------------
#define BYTE_BIT_COUNT 8



class CAxtDIO
{
    public:
        CAxtDIO();
        virtual ~CAxtDIO();

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
        void SetOut (int _iNo , bool  _bOn) ;
        bool GetOut (int _iNo             ) ;   
        bool GetIn  (int _iNo             ) ;
};

#endif
