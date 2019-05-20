//---------------------------------------------------------------------------

#ifndef LoaderH
#define LoaderH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"

//---------------------------------------------------------------------------
class CLoader
{
    public:
        //Constructor
        CLoader (void);
        ~CLoader (void);
	void Reset     (); //리셑 버튼 눌렀을때 타는 함수.
        void ResetTimer();
        void Close(void);

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
            bool bChckSplyEmpty ; //서플라이존이 컨베이어 벨트 돌려보고 카세트없음을 확인한 상태이다.
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle       =  0 ,
            scSupply     =  1 ,
            scPick       =  2 ,
            scWork       =  3 ,
            scPlace      =  4 ,

            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        void   InitCycleName(){
            m_sCycleName[scIdle  ]="Idle  ";
            m_sCycleName[scSupply]="Supply";
            m_sCycleName[scPick  ]="Pick  ";
            m_sCycleName[scWork  ]="Work  ";
            m_sCycleName[scPlace ]="Place ";
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
        CDelayTimer m_tmTemp    ;

        SStat Stat ;
        SStep Step , PreStep ;

        int    m_iMgzCnrtNo;
        int    m_iMgzCnt ;
        bool   FindChip   (int &r , int &c   );
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

    public:    /* Direct Accessable Var.  */
        bool   m_bIsHome;

        SetMgzCnt(int _iCnt){m_iMgzCnt = _iCnt ;};
        int    GetMgzCnt() {return m_iMgzCnt;};

        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

  
        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome = 0 ;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        SStat        GetStat        (){return Stat          ;}
        AnsiString   GetCycleName   (){return m_sCycleName[Step.iSeq];}

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

        //Running Functions.
        bool ToStopCon (void); //스탑을 하기 위한 조건을 보는 함수.
        bool ToStartCon(void); //스타트를 하기 위한 조건을 보는 함수.
        bool ToStart   (void); //스타트를 하기 위한 함수.
        bool ToStop    (void); //스탑을 하기 위한 함수.
        bool Autorun   (void); //오토런닝시에 계속 타는 함수.

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
extern CLoader LDR;
//---------------------------------------------------------------------------
#endif
