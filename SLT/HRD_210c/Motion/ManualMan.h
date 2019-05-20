//---------------------------------------------------------------------------
#ifndef ManualManH
#define ManualManH

#include "UtilDefine.h"
#include "Timer.h"
#include "SAInterface.h"
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
//        CArray          TempArray1 ;
//        CArray          TempArray2 ;
//        CArray          TempArray3 ;

        TRetResult RsltWfr ;
    public:
        __fastcall  CManualMan(void);
        __fastcall ~CManualMan(void);

        //Test Stop Flg.
        bool m_bStop ; //TestTrm ;

//        int GetManualNo() { return (int)m_iManNo; }
        //Init.
        void __fastcall Init (void);
        void __fastcall Reset(void){Init();}

        EN_MANUAL_CYCLE __fastcall GetManNo(                    );

        //Manual Processing.
        bool __fastcall SetManCycle(EN_MANUAL_CYCLE _iNo);
        void __fastcall Update();

        //ManCycle.

        bool PickPlaceHome();
        bool PickPlaceTest();
        bool STG_CycleSttVsTest();
        bool ETC_CycleTest();
        bool STG_CycleCenterMove();

        int m_iClickMoveCol ;
        int m_iClickMoveRow ;
        void SetWaferClickMove(int _iCol , int _iRow);
        bool STG_CycleClickMove();
        bool ETC_ToolChange();

        bool CycleManLotEnd     ();

};

//---------------------------------------------------------------------------
extern CManualMan MM;

//---------------------------------------------------------------------------
#endif