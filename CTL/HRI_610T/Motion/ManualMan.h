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

        CDelayTimer     m_tmCycle   ; //�޴��� ����Ŭ Ÿ�Ӿƿ���.
        CDelayTimer     m_tmDelay   ; //�޴��� ����Ŭ


        bool            m_bManSetting ; //��������.

        bool            m_bUldNeedPlace ; // ���� ���� ���� �޴��� ��..
        bool            m_bUldNeedPick  ; // ���� ���� ���� �޴��� ��..

    public:
        __fastcall  CManualMan(void);
        __fastcall ~CManualMan(void);

        //Init.
        void __fastcall Init (void);
        void __fastcall Close(void);
        void __fastcall Reset(void){Init();}

        EN_MANUAL_CYCLE __fastcall GetManNo  ();
        int                        GetManStep();

        //Manual Processing.
        bool __fastcall SetManCycle(EN_MANUAL_CYCLE _iNo);
        void __fastcall Update();

        //ManCycle.
        //bool CycleAllHome();  ��Ʈ���� ���������� ��� �ʿ� ����.
        bool CycleAlgnLaser        (); //����� ���� ������ ����.
        bool CycleMarkVisn         (); //��ũ���� �ѽ���Ŭ.
        bool CycleRailMoveNextPart (); //������ ��Ʈ�� ���� ��Ʈ������.....
        bool CycleRailOutAll       (); //���Ͽ� �ִ� ��Ʈ�� ��� ���� ����.
        bool CycleCntrMrk          (); //������-���� Ķ���극�̼� �Ҷ� ������ ���� ��ŷ�ϴ� ����Ŭ
        bool CycleVisnReject       (); //���� ���� �ִ� ���� �������� ����.


        bool m_TempWorkRepeat ; // �׽�Ʈ������ CycleAlgnLaser�� ��� ����.

        AnsiString sLsrCalDev;
        AnsiString sBeforeDev;
} ;

//---------------------------------------------------------------------------
extern CManualMan MM;

//---------------------------------------------------------------------------
#endif
