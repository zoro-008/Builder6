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
            int iModuleInChCnt  ; //��⺰ ��   ä�� ī��Ʈ
            int iModuleOutChCnt ; //��⺰ �ƿ� ä�� ī��Ʈ
        };
        CModuleInfo * m_pModuleInfo ;
        int m_iMaxModuleNo ;

        int m_iMaxIn  ;  //�� ä��.
        int m_iMaxOut ;  //�ƿ� ä��.

        bool GetInModuleNo (int _iNo , int &_iModuleNo , int &_iOffset);
        bool GetOutModuleNo(int _iNo , int &_iModuleNo , int &_iOffset);

    public:
        //�����ʰ� ��Ī �ϱ� ���� ��������.
        //bool GetInModuleNoDp (int _iNo , int &_iModuleNo , int &_iOffset); //Display�� ��� ����
        //bool GetOutModuleNoDp(int _iNo , int &_iModuleNo , int &_iOffset);

        void   SetOut (int _iNo , double  _dVal) ;
        double GetOut (int _iNo                ) ;
        double GetIn  (int _iNo                ) ;
};

extern CAjinAIO AI;

#endif
