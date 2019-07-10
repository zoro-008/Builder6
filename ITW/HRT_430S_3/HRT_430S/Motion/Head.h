//---------------------------------------------------------------------------

#ifndef HeadH
#define HeadH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"

//---------------------------------------------------------------------------
class CHead
{
    public:
        //Constructor
        CHead (void);
        ~CHead (void);
	void Reset     (); //리셑 버튼 눌렀을때 타는 함수.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
        };

        enum EN_SEQ_CYCLE {
            scIdle       =  0 ,
            scInsp       = 10 ,

            MAX_SEQ_CYCLE
        };

        struct SStep {
            int iHome    ;
            int iToStart ;
            int iSeq     ;
            int iCycle   ;
            int iToStop  ;
        };

    protected:
        //Timer.
        CDelayTimer m_tmMain    ;
        CDelayTimer m_tmCycle   ;
        CDelayTimer m_tmHome    ;
        CDelayTimer m_tmToStop  ;
        CDelayTimer m_tmToStart ;
        CDelayTimer m_tmTemp    ;

        SStat Stat ;
        SStep Step , PreStep ;

        bool   FindChip   (int &r , int &c );

	double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
	bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

    public:    /* Direct Accessable Var.  */
        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int  GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;}
        int  GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        int  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int  GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;}
        int  GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}

	//Motor.
	bool MoveMotr (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // 모터를 움직일때 쓰는 함수.

	//Actuator.
	bool MoveActr (EN_ACTR_ID _iActr , bool _bFwd); //실린더를 움직일때 쓰는 함수.

        //HomeCycle
        bool CycleHome(); //파트 홈을 잡을때 쓰는 함수.

	//One Cycle.
	bool CycleInsp (void); //검사하는 싸이클.

        //Running Functions.
	bool Update    (void); //오토런닝시에 계속 타는 함수.
	bool ToStopCon (void); //스탑을 하기 위한 조건을 보는 함수.
	bool ToStartCon(void); //스타트를 하기 위한 조건을 보는 함수.
	bool ToStart   (void); //스타트를 하기 위한 함수.
	bool ToStop    (void); //스탑을 하기 위한 함수.

        //For Start Stop Checking.
        void SetLastCmd (void); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (void); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (void); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool IsLoad , FILE *fp);

};

//---------------------------------------------------------------------------
extern CHead HD;

//---------------------------------------------------------------------------
#endif
