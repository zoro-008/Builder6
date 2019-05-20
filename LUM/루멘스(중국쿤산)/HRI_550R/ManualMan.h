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
        CArray          TempArray ;
    public:
        __fastcall  CManualMan(void);
        __fastcall ~CManualMan(void);

        //Test Stop Flg.
        bool m_bStopTestMrk ;

        //Init.
        void __fastcall Init (void);
        void __fastcall Reset(void){Init();}

        EN_MANUAL_CYCLE __fastcall GetManNo(                    );

        //Manual Processing.
        bool __fastcall SetManCycle(EN_MANUAL_CYCLE _iNo);
        void __fastcall Update();

        //ManCycle.
        //bool CycleAllHome();  ��Ʈ���� ���������� ��� �ʿ� ����.
        bool CycleStipRemove    ();
        bool CyclePreStipRemove ();
        bool CycleWrkStipRemove ();
        bool CyclePstStipRemove ();
        bool CycleMan1ndLift    ();
        bool CycleMan2ndLift    ();
        bool CycleMan3ndLift    ();
        bool CycleManMrkLift    ();
        bool CycleManAllLift    ();
        bool CycleManMrkUpDn    ();
        bool CycleManRemoveIndex();
        bool CycleManMrkChange  ();

        bool CycleManBindInsp   ();
        bool CycleManMrkBindInsp();

        bool CycleManPsbInitPos ();
        bool CycleManWorkSttPos ();
        bool CycleManTestMrk    ();
        bool CycleManUD1        ();

        bool CycleManLotEnd     ();
        bool CycleManFlprNomalPs();
        bool CycleManFlprInversePs();
};

//---------------------------------------------------------------------------
extern CManualMan MM;

//---------------------------------------------------------------------------
#endif
