//---------------------------------------------------------------------------
#ifndef ManualManH
#define ManualManH

#include "UtilDefine.h"
#include "Timer.h"

//---------------------------------------------------------------------------
class CManualMan
{
    private:
        //Object.
        int             m_iManStep , m_iPreManStep ;
        EN_MANUAL_CYCLE m_iManNo   ;

        CDelayTimer     m_tmCycle   ; //Ÿ�Ӿƿ���.
        CDelayTimer     m_tmDelay   ; //Ÿ�Ӿƿ���.

        bool            m_bManSetting ; //��������.

    public:
        __fastcall  CManualMan(void);
        __fastcall ~CManualMan(void);

        //Init.
        void Init (void);
        void Close(void);
        void __fastcall Reset(void){Init();}

        EN_MANUAL_CYCLE __fastcall GetManNo(                    );

        //Manual Processing.
        bool __fastcall SetManCycle(EN_MANUAL_CYCLE _iNo);
        void __fastcall Update();

        //ManCycle.
        //bool CycleAllHome();  ��Ʈ���� ���������� ��� �ʿ� ����.
        //bool CycleStipRemove   ();
        //bool CyclePreStipRemove();
        //bool CycleWrkStipRemove();
        //bool CyclePstStipRemove();
        bool CycleToolChange   ();
        bool CycleMasterInsp   ();
        
        double dMTPos;
        int  m_iSTLPnchCnt;
        int  m_iSTLSnsrCnt;

        //�˻� ���.
        bool _bInspMaster;
};

//---------------------------------------------------------------------------
extern CManualMan MM;

//---------------------------------------------------------------------------
#endif