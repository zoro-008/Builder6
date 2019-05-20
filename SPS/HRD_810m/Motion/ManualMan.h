//---------------------------------------------------------------------------
#ifndef ManualManH
#define ManualManH

#include "UtilDefine.h"
#include "Timer.h"
#include "Array.h"
#include "VisionMan.h"
//---------------------------------------------------------------------------
class CManualMan
{
    private:
        //Object.
        int             m_iManStep , m_iPreManStep ;
        EN_MANUAL_CYCLE m_iManNo   ;

        bool            m_bManSetting ; //이거 없으면 메뉴얼 세팅중에 메세지 창등 떠서 다른 쓰레드 update에서
                                        //스텝 클리어됌. 주의.

        CDelayTimer     m_tmCycle   ; //타임아웃용.
        CDelayTimer     m_tmTemp    ;

        CVisionMan::TRetResult RsltWfr ;
    public:
        __fastcall  CManualMan(void);
        __fastcall ~CManualMan(void);

        //Test Stop Flg.
        bool m_bStop ; //TestTrm ;

        //Init.
        void __fastcall Init (void);
        void __fastcall Reset(void){Init();}

        EN_MANUAL_CYCLE __fastcall GetManNo();

        //Manual Processing.
        bool __fastcall SetManCycle(EN_MANUAL_CYCLE _iNo);
        void __fastcall Update();

        bool CycleHedVisnMove();
        


};

//---------------------------------------------------------------------------
extern CManualMan MM;

//---------------------------------------------------------------------------
#endif
