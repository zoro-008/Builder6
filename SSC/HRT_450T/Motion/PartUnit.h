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

	void Reset   (); //���V ��ư �������� Ÿ�� �Լ�.
        bool MoveHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

	//Motor.
	bool MoveMotr (EN_MOTR_ID Motr , EN_PSTN_ID Cmd); // ���͸� �����϶� ���� �Լ�.

	//Actuator.
	bool MoveActr (EN_ACTR_ID Actr , bool On); //�Ǹ����� �����϶� ���� �Լ�.

	//One Cycle.
	bool CycleMove (void); //�����̾� ��Ʈ�� �����Ͽ� ī��Ʈ�� �ű�� �Լ�.

        //Running Functions.
	bool Update    (void); //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
	bool ToStopCon (void); //��ž�� �ϱ� ���� ������ ���� �Լ�.
	bool ToStartCon(void); //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
	bool ToStart   (void); //��ŸƮ�� �ϱ� ���� �Լ�.
	bool ToStop    (void); //��ž�� �ϱ� ���� �Լ�.

        //Loading Para.
        void Load(bool IsLoad , FILE *fp);
};

//---------------------------------------------------------------------------
//extern CPart VL;

//---------------------------------------------------------------------------
#endif
