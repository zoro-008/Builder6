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

        CDelayTimer     m_tmCycle        ; //�޴��� ����Ŭ Ÿ�Ӿƿ���.
        CDelayTimer     m_tmDelay        ; //�޴��� ����Ŭ
        CDelayTimer     m_tmLDspsDelay   ; // ���� DspsDelay�� �ֱ� ���� ��
        CDelayTimer     m_tmRDspsDelay   ; // ���� 


        bool            m_bManSetting ; //��������.

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
        bool CycleDispWait      (); //����� ���� ������ ����.
        bool CycleDispWork      (); //����� ���� ������ ����.
        bool CycleStopToDispShot();
        //bool CycleAllHome();  ��Ʈ���� ���������� ��� �ʿ� ����.
} ;

//---------------------------------------------------------------------------
extern CManualMan MM;

//---------------------------------------------------------------------------
#endif
