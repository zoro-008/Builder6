//---------------------------------------------------------------------------

#ifndef PartUnitH
#define PartUnitH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"

//---------------------------------------------------------------------------
class CPart
{
    public:
        //Constructor
        CPart (void);
        ~CPart (void);

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
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

	double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstn );
	bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstn );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd  );

    public:    /* Direct Accessable Var.  */
        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int  GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;}
        int  GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        int  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int  GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;}
        int  GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}

	void Reset   (); //리셑 버튼 눌렀을때 타는 함수.
        bool MoveHome(); //파트 홈을 잡을때 쓰는 함수.

	//Motor.
	bool MoveMotr (EN_MOTR_ID Motr , EN_PSTN_ID Cmd); // 모터를 움직일때 쓰는 함수.

	//Actuator.
	bool MoveActr (EN_ACTR_ID Actr , bool On); //실린더를 움직일때 쓰는 함수.

	//One Cycle.
	bool CycleMove (void); //컨베이어 벨트를 구동하여 카세트를 옮기는 함수.

        //Running Functions.
	bool Update    (void); //오토런닝시에 계속 타는 함수.
	bool ToStopCon (void); //스탑을 하기 위한 조건을 보는 함수.
	bool ToStartCon(void); //스타트를 하기 위한 조건을 보는 함수.
	bool ToStart   (void); //스타트를 하기 위한 함수.
	bool ToStop    (void); //스탑을 하기 위한 함수.

        //Loading Para.
        void Load(bool IsLoad , FILE *fp);
};

//---------------------------------------------------------------------------
//extern CPart VL;

//---------------------------------------------------------------------------
#endif
