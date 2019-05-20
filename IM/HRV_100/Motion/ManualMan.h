//---------------------------------------------------------------------------
#ifndef ManualManH
#define ManualManH

#include "UtilDefine.h"
#include "Timer.h"

//---------------------------------------------------------------------------
enum EN_SORT_POS_ID {
    spNone  = 0 , //���� �����ǿ��� ��,�÷��̽�.
    spLDR       ,
    spZIG       ,
    spSTG       ,
    spRJ1       ,
    spRJ2       ,
    spRJ3       ,
    spRJ4       ,
    spULD       ,
    MAX_SORT_POS_ID
};






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
} ;

//---------------------------------------------------------------------------
extern CManualMan MM;

//---------------------------------------------------------------------------
#endif
