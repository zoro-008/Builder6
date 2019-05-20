//---------------------------------------------------------------------------

#ifndef IndexH
#define IndexH
//---------------------------------------------------------------------------
#include "PartInterface.h"
#include "UnLoader.h"

#include "Timer.h"
#include "DataMan.h"

//---------------------------------------------------------------------------
class CIndex : public CPartInterface
{
    public: //초기화 등등.
        //Constructor
        CIndex ();
        ~CIndex ();

        void Init();
        void Close();


    public:

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
            bool bDontMove;
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle       =  0 ,                                                 
            scGet        =  1 ,                                                 
            scRotator    =  2 ,
            MAX_SEQ_CYCLE
        };

        int        m_iCycleSttTime             ;
        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //마지막 사이클 시간. ms
        void   InitCycleName(){
            m_sCycleName[scIdle   ]="Idle"    ;
            m_sCycleName[scGet    ]="Get"     ;
            m_sCycleName[scRotator]="Rotator" ;
        }
        void   InitCycleTime(){
            memset(&m_iCycleTime  , 0 , sizeof(int )*MAX_SEQ_CYCLE);
        }

        struct SStep {
            int          iHome    ;
            int          iToStart ;
            EN_SEQ_CYCLE iSeq     ;
            int          iCycle   ;
            int          iToStop  ;
        };

    protected:
        AnsiString m_sPartName  ;
        //Timer.
        CDelayTimer m_tmMain    ;
        CDelayTimer m_tmCycle   ;
        CDelayTimer m_tmHome    ;
        CDelayTimer m_tmToStop  ;
        CDelayTimer m_tmToStart ;
        CDelayTimer m_tmDelay   ;

        //Front Rail은 의미 없다.
        CDelayTimer m_tmHeat    ; //프리인덱스에서 열받고 있는 시간. 이시간이 일정 시간 이상 되어야 작업을 시작한다.
    

        SStat  Stat ;
        SStep  Step , PreStep ;

        double m_dLastIdxPos ;

        String m_sCheckSafeMsg ;

        bool   m_bPassedDetect4 ; //Out때 Detect4이 감지 됐었는지.

        double GetLastCmd(EN_MOTR_ID _iMotr);

        void   ResetTimer();


        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

        //똑같은 놈이 2개라 인덱스를 가지고 있는다.
//        struct SAdd{
//            EN_PART_ID     iPartId               ;
//            EN_ARAY_ID     riIDX                 ;
//            EN_MOTR_ID     miIDX_XIndx           ;
//            EN_PSTN_VALUE  pvIDX_XIndxWait       ;
//            EN_PSTN_VALUE  pvIDX_XIndxGet        ;
//            EN_PSTN_VALUE  pvIDX_XIndxWorkStart  ;
//            EN_PSTN_VALUE  pvIDX_XIndxOut        ;
//            EN_PSTN_ID     piIDX_XIndxWait       ;
//            EN_PSTN_ID     piIDX_XIndxGet        ;
//            EN_PSTN_ID     piIDX_XIndxWorkStart  ;
//            EN_PSTN_ID     piIDX_XIndxOut        ;
//            EN_PSTN_ID     piIDX_XIndxWork       ;
//            EN_INPUT_ID    xIDX_WrkDetect3       ;
//            EN_INPUT_ID    xIDX_Detect4          ;
//            EN_INPUT_ID    xIDX_IndxNotOvLd      ;
//            EN_ACTR_ID     aiIDX_IndxUpDn        ;
//            EN_CHIP_STAT   csAfterWork           ;
//
//            EN_ARAY_ID     riPSI                 ;
//
//            CUnLoader    * ULD                   ;
//            EN_ARAY_ID     riULDTop              ;
//            EN_ARAY_ID     riULDBtm              ;
//            EN_ACTR_ID     aiPushLtRt            ;
//            EN_ACTR_ID     aiPushUpDn            ;
//            EN_INPUT_ID    xULD_Detect5          ;
//
//            CPreIndex    * PRI                   ;
//            EN_ARAY_ID     riPRI                 ;
//
//            EN_MOTR_ID     miTool                ;
//            EN_PSTN_VALUE  pvToolWait            ;
//
//        };
//        SAdd Add ;

    public: //인터페이스 상속.==================================================
	void Reset (); //리셑 버튼 눌렀을때 타는 함수.

        //Running Functions.
        void Update    ();
        bool ToStopCon (); //스탑을 하기 위한 조건을 보는 함수.
        bool ToStartCon(); //스타트를 하기 위한 조건을 보는 함수.
        bool ToStart   (); //스타트를 하기 위한 함수.
        bool ToStop    (); //스탑을 하기 위한 함수.

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;}void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome  = 0 ;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        int          GetSeqStep     (){return (int)Step.iSeq;} int GetPreSeqStep    (){return (int)PreStep.iSeq;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;}void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}

        AnsiString   GetCycleName   (int _iSeq){return m_sCycleName[_iSeq];}
        int          GetCycleTime   (int _iSeq){return m_iCycleTime[_iSeq];}
        AnsiString   GetPartName    (){return m_sPartName   ;}

        int          GetCycleMaxCnt (){return MAX_SEQ_CYCLE ;}

        bool         Autorun        (); //오토런닝시에 계속 타는 함수.
        //======================================================================
    public:
//        bool         FindChip   (int &c , EN_ARAY_ID &id);
        bool         IsWorkPos();

        //HomeCycle
        bool CycleHome(); //파트 홈을 잡을때 쓰는 함수.

        //One Cycle.
        bool CycleGet     ();
        bool CycleRotator ();

        bool CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        void  SetStat(SStat _sStat) { Stat = _sStat ; }
        SStat GetStat(            ) { return Stat;    }

        //Motor.
        //bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // 모터를 움직일때 쓰는 함수.
        void MoveIncMotr (EN_MOTR_ID _iMotr , double _iPstn); // 모터를 움직일때 쓰는 함수.

        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow = false);

        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //실린더를 움직일때 쓰는 함수.

        //For Start Stop Checking.
        void SetLastCmd (); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool _bLoad);


};

//---------------------------------------------------------------------------
extern CIndex IDX;
//---------------------------------------------------------------------------
#endif
