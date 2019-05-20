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

        bool            m_bManSetting ; //��������.
        CDelayTimer     m_tmCycle ;
        CDelayTimer     m_tmTemp  ;
        CArray          TempArray ;

    public:
        __fastcall  CManualMan(void);
        __fastcall ~CManualMan(void);

        bool m_bStopTestMrk;

        //Init.
        void __fastcall Init (void);
        void __fastcall Reset(void){Init();}

        EN_MANUAL_CYCLE __fastcall GetManNo(                    );

        //Manual Processing.
        bool __fastcall SetManCycle(EN_MANUAL_CYCLE _iNo);
        void __fastcall Update();

        //ManCycle.
        //bool CycleAllHome();  ��Ʈ���� ���������� ��� �ʿ� ����.


        bool CycleManBindInsp      ();
        bool CycleManFlprNomalPs   ();
        bool CycleManFlprInversePs ();
        bool CycleManPsbInitPos    ();
        bool CycleManWorkSttPos    ();
        bool CycleManVisnSttPos    ();
        bool CycleManMrkChange     ();
        bool CycleManTestMrk       ();
        bool CycleManLotEnd        ();
        bool CycleManTraySupply    ();
        bool CycleManTrayOpen      ();
        bool CycleManULDPickPs     ();
        bool CycleManULDFrstPlacePs();
        bool CycleManULDPaperPs    ();
        bool CycleManULDPick       ();
        bool CycleManULDPlace      ();
        bool CycleManULMPprSupply  ();
};

//---------------------------------------------------------------------------
extern CManualMan MM;

//---------------------------------------------------------------------------
#endif
