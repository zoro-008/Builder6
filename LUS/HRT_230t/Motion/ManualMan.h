//---------------------------------------------------------------------------
#ifndef ManualManH
#define ManualManH

#include "UtilDefine.h"
#include "Timer.h"
#include "Array.h"

//---------------------------------------------------------------------------
class CManualMan
{
    private:
        //Object.
        int             m_iManStep , m_iPreManStep ;
        EN_MANUAL_CYCLE m_iManNo   ;

        bool            m_bManSetting ; //�̰� ������ �޴��� �����߿� �޼��� â�� ���� �ٸ� ������ update����
                                        //���� Ŭ�����. ����.

        CDelayTimer     m_tmCycle   ; //Ÿ�Ӿƿ���.
        CDelayTimer     m_tmTemp    ;
    public:
        __fastcall  CManualMan(void);
        __fastcall ~CManualMan(void);

        //Init.
        void __fastcall Init (void);
        void __fastcall Reset(void){Init();}

        EN_MANUAL_CYCLE __fastcall GetManNo( );

        //Manual Processing.
        bool __fastcall SetManCycle(EN_MANUAL_CYCLE _iNo);
        void __fastcall Update();

        //ManCycle.
        //bool CycleAllHome();  ��Ʈ���� ���������� ��� �ʿ� ����.
        bool CycleOne();
        bool CycleMoveBin3();
        bool CycleMoveBin4();
        bool CycleMoveBin5();
};

//---------------------------------------------------------------------------
extern CManualMan MM;

//---------------------------------------------------------------------------
#endif
