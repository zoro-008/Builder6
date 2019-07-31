//---------------------------------------------------------------------------

#ifndef RailH
#define RailH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
//---------------------------------------------------------------------------
class CRail
{
    public:
        //Constructor
        CRail (void);
        ~CRail (void);
	void Reset     (); //���V ��ư �������� Ÿ�� �Լ�.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd ; //��Ʈ�� ������ ������� true ;
            bool bReqStop ; //������ ���ų� Ȥ�� ��ž��ư�� �������� ��Ʈ�� ���⸦ ��û�ϴ� ������.
        };

        enum EN_SEQ_CYCLE {
            scIdle       =  0 ,
            scOut        = 10 ,
            scFlip       = 20 ,
            scInsp3Out   = 30 ,
            scInsp2Out   = 40 ,
            scInsp1Out   = 50 ,
            scPreBuffOut = 60 ,
            scIn         = 70 ,

            MAX_SEQ_CYCLE
        };

        struct SStep {
            int          iHome    ; //Ȩ����Ŭ���� ���� ����.
            int          iToStart ; //����ŸƮ ����Ŭ���� ��.
            EN_SEQ_CYCLE iSeq     ; //����� ���� ����.
            int          iCycle   ; //����� ���Ͽ��� ����.
            int          iToStop  ; //����ž ����Ŭ���� ��.
        };

    protected:
        //Timer.
        CDelayTimer m_tmMain    ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmCycle   ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmHome    ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmToStop  ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmToStart ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmTemp    ; //����Ŭ �Լ����� ������� ���¾�.

        SStat Stat ;              //
        SStep Step , PreStep ;

        bool   FindChip   (int &r , int &c );
	double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //���Ͱ� ������ �������� ������. Double��.
	bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //���Ͱ� �����϶� �浹���� ����.
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    ); //�Ǹ����� �����϶� �浹���� ����.

    public:    /* Direct Accessable Var.  */
        bool GetWorkEnd (){return Stat.bWorkEnd ;}
        bool GetReqStop (){return Stat.bReqStop ;}

        int           GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;}
        int           GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int           GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;}
        int           GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}

	//Motor.
	bool MoveMotr (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // ���͸� �����϶� ���� �Լ�.

	//Actuator.
	bool MoveActr (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.

        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

	//One Cycle.
	bool CycleOut        (void); //Rail���ܿ��� ��� ������ ���� �Ǵ� �Լ�.
        bool CycleFlip       (void); //Rail���ܿ��� Ʈ���� ���� �ø��� �ϴ� ����.
      //bool CycleReFlip     (void); //Rail�������� �ø��Ѵ�.
        bool CycleInsp3Out   (void); //Insp3Zone���� PostBuffer�� ���� �̼�.
        bool CycleInsp2Out   (void); //Insp3Zone���� PostBuffer�� ���� �̼�.
        bool CycleInsp1Out   (void); //Insp3Zone���� PostBuffer�� ���� �̼�.
        bool CyclePreBuffOut (void); //Insp3Zone���� PostBuffer�� ���� �̼�.
        bool CycleIn         (void); //��Ʈ���� PreBuff�� �̼�.

        //Running Functions.
	bool Update      (void); //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
	bool ToStopCon   (void); //��ž�� �ϱ� ���� ������ ���� �Լ�.
	bool ToStartCon  (void); //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
	bool CycleToStart(void); //��ŸƮ�� �ϱ� ���� �Լ�.
	bool CycleToStop (void); //��ž�� �ϱ� ���� �Լ�.

        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool IsLoad , FILE *fp);

};

//---------------------------------------------------------------------------
extern CRail RL;

//---------------------------------------------------------------------------
#endif
