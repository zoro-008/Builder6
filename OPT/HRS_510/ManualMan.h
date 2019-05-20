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
        CArray          TempArray1 ;
        CArray          TempArray2 ;
        CArray          TempArray3 ;
    public:
        __fastcall  CManualMan(void);
        __fastcall ~CManualMan(void);

        //Test Stop Flg.
        bool m_bStopTestTrm ;

        //Init.
        void __fastcall Init (void);
        void __fastcall Reset(void){Init();}

        EN_MANUAL_CYCLE __fastcall GetManNo(                    );

        //Manual Processing.
        bool __fastcall SetManCycle(EN_MANUAL_CYCLE _iNo);
        void __fastcall Update();

        //ManCycle.
        //bool CycleAllHome();  ��Ʈ���� ���������� ��� �ʿ� ����.
        bool CycleManInsAlignFlp();
        bool CycleManLift       (int iNo); // 0,1,2,3,--- 0:All
//        bool CycleStipRemove    ();
//        bool CyclePreStipRemove ();
//        bool CycleWrkStipRemove ();
//        bool CyclePstStipRemove ();
//        bool CycleMan1ndLift    ();
//        bool CycleMan2ndLift    ();
//        bool CycleMan3ndLift    ();
//        bool CycleManTrmLift    ();
//        bool CycleManAllLift    ();
//        bool CycleManTrmUpDn    ();
//        bool CycleManRemoveIndex();
//        bool CycleManTrmChange  ();

        bool CycleManBindInsp   ();
//        bool CycleManTrmBindInsp();

//        bool CycleManPsbInitPos ();
//        bool CycleManWorkSttPos ();
//        bool CycleManTestTrm    ();
//        bool CycleManUD1        ();

        bool CycleManLotEnd     ();
//        bool CycleManFlprNomalPs();
//        bool CycleManFlprInversePs();
};

//---------------------------------------------------------------------------
extern CManualMan MM;

//---------------------------------------------------------------------------
#endif
