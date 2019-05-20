//---------------------------------------------------------------------------

#ifndef TrimH
#define TrimH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"

//---------------------------------------------------------------------------
class CTrim
{
    public:
        //Constructor
        CTrim (void);
        ~CTrim (void);
	void Reset     (); //���V ��ư �������� Ÿ�� �Լ�.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
        };

        enum EN_SEQ_CYCLE {
            scIdle       =  0 ,
            scTrim       = 10 ,

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

        bool   FindChip   (int &r , int &c   );
	double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
	bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

    public:    /* Direct Accessable Var.  */
        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int  GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){Step.iHome  = 10;}
        int  GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        int  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int  GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){Step.iCycle = 10;}
        int  GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}

	//Motor.
	bool MoveMotr (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // ���͸� �����϶� ���� �Լ�.

	//Actuator.
	bool MoveActr (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.

        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

	//One Cycle.
	bool CycleTrim (void); //�˻��ϴ� ����Ŭ.

        //Running Functions.
	bool Autorun     (void); //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
	bool ToStopCon   (void); //��ž�� �ϱ� ���� ������ ���� �Լ�.
	bool ToStartCon  (void); //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
	bool ToStart     (void); //��ŸƮ�� �ϱ� ���� �Լ�.
	bool ToStop      (void); //��ž�� �ϱ� ���� �Լ�.

        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool IsLoad , FILE *fp);

};

//---------------------------------------------------------------------------
extern CTrim TR;

//---------------------------------------------------------------------------
#endif
