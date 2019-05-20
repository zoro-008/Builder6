//---------------------------------------------------------------------------

#ifndef UnLoader430tH
#define UnLoader430tH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
#include "PartInterface.h"

//---------------------------------------------------------------------------
class CUnLoader : public CPartInterface
{
    public:
        //Constructor
        CUnLoader (void);
        ~CUnLoader (void);

        void Init   ();
        void Close  ();

    public:

        //Vars.
        struct SStat {
            bool bWorkEnd     ;
            bool bReqStop     ;
            bool bReadyMgz    ;
            bool bChckSplyEmpty ; //서플라이존이 컨베이어 벨트 돌려보고 카세트없음을 확인한 상태이다.
        };

        enum EN_SEQ_CYCLE {
            scIdle   =  0 ,
            scSupply =  1 ,
            scPick   =  2 ,
            scWork   =  3 ,
            scPlace  =  4 ,
            MAX_SEQ_CYCLE
        };

        int        m_iCycleSttTime             ;
        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //마지막 사이클 시간. ms
        void   InitCycleName(){
            m_sCycleName[scIdle  ]="Idle  ";
            m_sCycleName[scSupply]="Supply";
            m_sCycleName[scPick  ]="Pick  ";
            m_sCycleName[scWork  ]="Work  ";
            m_sCycleName[scPlace ]="Place ";
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

        CDelayTimer m_tmHeat    ; //프리인덱스에서 열받고 있는 시간. 이시간이 일정 시간 이상 되어야 작업을 시작한다.

        SStat Stat ;
        SStep Step , PreStep ;

        double m_dLastIdxPos ;

        String m_sCheckSafeMsg ;

        double GetLastCmd(EN_MOTR_ID _iMotr);

        void   ResetTimer();

        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

        AnsiString m_sWorkedLotNo ;

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
        int    m_iMgzCnrtNo;
        int    m_iMgzCnt ;
        bool   FindChip   (int &r , int &c   );

    public:    /* Direct Accessable Var.  */
        bool   m_bIsHome;

        SetMgzCnt(int _iCnt){m_iMgzCnt = _iCnt ;};
        int    GetMgzCnt() {return m_iMgzCnt;};

        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );


        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

        //Property.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        __property int   _iMgzCnrtNo   = { read = m_iMgzCnrtNo                    };
        __property int   _iMgzCnt      = { read = m_iMgzCnt    , write= m_iMgzCnt };
        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // 모터를 움직일때 쓰는 함수.
        void MoveIncMotr (EN_MOTR_ID _iMotr , double _iPstn); // 모터를 움직일때 쓰는 함수.
        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //실린더를 움직일때 쓰는 함수.

        //HomeCycle
        bool CycleHome(); //파트 홈을 잡을때 쓰는 함수.

        //One Cycle.
        bool CycleSupply  ();
        bool CyclePick    ();
        bool CycleWork    ();
        bool CyclePlace   ();

        //For Start Stop Checking.
        void SetLastCmd (void); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (void); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (void); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool IsLoad);

};

//---------------------------------------------------------------------------
extern CUnLoader ULD;
//---------------------------------------------------------------------------
#endif
