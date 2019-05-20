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
        bool CycleAlgnLaser        (); //얼라인 보고 레이져 조사.
        bool CycleMarkVisn         (); //마크비젼 한싸이클.
        bool CycleRailMoveNextPart (); //레이저 스트립 비젼 스트립으로.....
        bool CycleRailOutAll       (); //레일에 있는 스트립 모두 리젝 배출.
        bool CycleCntrMrk          (); //레이져-비젼 캘리브레이션 할때 레이져 센터 마킹하는 사이클
        bool CycleVisnReject       (); //비전 존에 있는 자재 리젝으로 제거.


        bool m_TempWorkRepeat ; // 테스트용으로 CycleAlgnLaser를 계속 돌림.

        AnsiString sLsrCalDev;
        AnsiString sBeforeDev;
} ;

//---------------------------------------------------------------------------
extern CManualMan MM;

//---------------------------------------------------------------------------
#endif
