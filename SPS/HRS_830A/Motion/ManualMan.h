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

        CDelayTimer     m_tmCycle        ; //메뉴얼 싸이클 타임아웃용.
        CDelayTimer     m_tmDelay        ; //메뉴얼 싸이클
        CDelayTimer     m_tmLDspsDelay   ; // 좌측 DspsDelay를 주기 위한 것
        CDelayTimer     m_tmRDspsDelay   ; // 우측 


        bool            m_bManSetting ; //쎄팅중임.

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
        bool CycleDispWait      (); //얼라인 보고 레이져 조사.
        bool CycleDispWork      (); //얼라인 보고 레이져 조사.
        bool CycleStopToDispShot();
        //bool CycleAllHome();  파트간에 간섭조건이 없어서 필요 없다.
} ;

//---------------------------------------------------------------------------
extern CManualMan MM;

//---------------------------------------------------------------------------
#endif
