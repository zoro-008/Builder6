//---------------------------------------------------------------------------
#ifndef ManualManH
#define ManualManH

#include "UtilDefine.h"
#include "Timer.h"

//---------------------------------------------------------------------------
enum EN_SORT_POS_ID {
    spNone  = 0 , //현재 포지션에서 픽,플레이스.
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

        CDelayTimer     m_tmCycle   ; //메뉴얼 싸이클 타임아웃용.
        CDelayTimer     m_tmDelay   ; //메뉴얼 싸이클


        bool            m_bManSetting ; //쎄팅중임.

        bool            m_bUldNeedPlace ; // 레일 자제 배출 메뉴얼 시..
        bool            m_bUldNeedPick  ; // 레일 자제 배출 메뉴얼 시..
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
        //bool CycleAllHome();  파트간에 간섭조건이 없어서 필요 없다.
} ;

//---------------------------------------------------------------------------
extern CManualMan MM;

//---------------------------------------------------------------------------
#endif
