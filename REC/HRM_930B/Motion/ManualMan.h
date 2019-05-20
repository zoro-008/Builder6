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
        bool            m_bStdCalPick ; //Picker StdCal한 후 Pick 동작 하기 위해.


        double  m_dVisnRsltGapX ;
        double  m_dVisnRsltGapY ;
        double  m_dVisnRsltGapT ;

    public:
        CManualMan(void);
        ~CManualMan(void);

        //Init.
        void Init (void);
        void Close(void);
        void Reset(void){Init();}

        EN_MANUAL_CYCLE  GetManNo  ();
        int              GetManStep();

        //Manual Processing.
        bool SetManCycle(EN_MANUAL_CYCLE _iNo);
        void Update();

        //ManCycle.
        bool CycleVisnCal (); //픽커가 얼라인에서 집어서 비젼검사하고 스테이지에 내려 놓고 At비젼으로 검사해봐서 오프셑이 얼마나 발생하는지 확인.
        bool CycleAlgnPlce(); //얼라인에 놓는 메뉴얼.
        bool CycleCnvr    ();
        bool CycleNdlCheck(); //니들체크 하기전에 클린 해야하기 때문에.

        double m_dCadMoveX ;
        double m_dCadMoveY ;

        EN_TOOL_ID m_eToolId ;

        void SetCadMove(EN_TOOL_ID _tiTool, double _dX, double _dY);
        bool CycleCadMove();

        bool CycleManualEndHeight();

        bool CycleLDR_IdxGet ();    //리어 로더에서 로테이터로 자재 이송
        bool CycleLDR_IdxPlce();   //로테이터에서 리어 로더로 자재 플레이스

        bool CycleLDR_FPreGet();
        bool CyclePstRailULDPlce();

        bool CycleSTGChange();


        bool GetVisnGap(double * _dVisnX , double * _dVisnY , double * _dVisnT);
} ;

//---------------------------------------------------------------------------
extern CManualMan MM;

//---------------------------------------------------------------------------
#endif
