//---------------------------------------------------------------------------

#ifndef Ajin_AIOH
#define Ajin_AIOH
//SUN Kye Won 2010.12.27
//---------------------------------------------------------------------------


class CAjinAIO
{
    public:
        CAjinAIO();
        virtual ~CAjinAIO();

    protected:
        struct CModuleInfo {
            int iModuleInChCnt  ; //모듈별 인   채널 카운트
            int iModuleOutChCnt ; //모듈별 아웃 채널 카운트
        };
        CModuleInfo * m_pModuleInfo ;
        int m_iMaxModuleNo ;

        int m_iMaxIn  ;  //인 채널.
        int m_iMaxOut ;  //아웃 채널.

        bool GetInModuleNo (int _iNo , int &_iModuleNo , int &_iOffset);
        bool GetOutModuleNo(int _iNo , int &_iModuleNo , int &_iOffset);

    public:
        //전장쪽과 매칭 하기 위한 디스프레이.
        //bool GetInModuleNoDp (int _iNo , int &_iModuleNo , int &_iOffset); //Display용 모듈 순서
        //bool GetOutModuleNoDp(int _iNo , int &_iModuleNo , int &_iOffset);

        void   SetOut (int _iNo , double  _dVal) ;
        double GetOut (int _iNo                ) ;
        double GetIn  (int _iNo                ) ;
};

extern CAjinAIO AI;

#endif
