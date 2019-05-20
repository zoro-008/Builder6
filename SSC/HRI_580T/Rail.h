//---------------------------------------------------------------------------

#ifndef RailH
#define RailH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
#include "LotUnit.h"
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
            scIdle    =  0 ,
            scIn      = 10 ,
            scMvAuto  = 20 ,
            scBind    = 30 ,
            scMvAll   = 40 ,
            scMvR2    = 50 ,
            scMvR1    = 60 ,
            scMv12    = 70 ,
            scMvR     = 80 ,
            scMv1     = 90 ,
            scMv2     = 100 ,
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
        AnsiString  m_sPartName ;
        //Timer.
        CDelayTimer m_tmMain    ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmCycle   ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmHome    ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmToStop  ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmToStart ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmTemp    ; //����Ŭ �Լ����� ������� ���¾�.

        SStat Stat ;              //
        SStep Step , PreStep ;

        bool m_bWaitLotChange ;

        AnsiString m_sPreLotNo ;

        bool   FindChip   (int &r , int &c );
	double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //���Ͱ� ������ �������� ������. Double��.

    public:    /* Direct Accessable Var.  */

        bool           m_bLotEnded ; //ȭ�鿡 ������ �� �����.
        CLot::SLotInfo EndedLot    ;

        bool CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //���Ͱ� �����϶� �浹���� ����.
        bool CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    ); //�Ǹ����� �����϶� �浹���� ����.

        bool GetWorkEnd (){return Stat.bWorkEnd ;}
        bool GetReqStop (){return Stat.bReqStop ;}

        int           GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){Step.iHome  = 10;}
        int           GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int           GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){Step.iCycle = 10;}
        int           GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        SStat         GetStat        (){return Stat          ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

	//Motor.
	bool MoveMotr (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // ���͸� �����϶� ���� �Լ�.

	//Actuator.
	bool MoveActr (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.

        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

        //Running Functions.
	bool ToStopCon   (void); //��ž�� �ϱ� ���� ������ ���� �Լ�.
	bool ToStartCon  (void); //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
	bool ToStart     (void); //��ŸƮ�� �ϱ� ���� �Լ�.
	bool ToStop      (void); //��ž�� �ϱ� ���� �Լ�.
        bool Autorun     (void); //���䷱�׽ÿ� ��� Ÿ�� �Լ�.

	//One Cycle.
        bool CycleIn     (void); //��Ʈ���� PreBuff�� �̼�.
        bool CycleMvAuto (void); //(pRb->wk1->wk2)->pSb �� ��� �Ͽ� �ڵ����� �̵��� �͸� �ؼ� �̵� �Ѵ�.
        bool CycleBind   (void); //

        //�� ������ �𹰵�.  CycleMvAuto�� ����.
/*
        bool CycleMvAll  (void); //(pRb->wk1->wk2)->pSb
        bool CycleMvR2   (void); //(pRb->wk1  wk2)->pSb
        bool CycleMvR1   (void); //(pRb->wk1->wk2)
        bool CycleMv12   (void); //(     wk1->wk2)->pSb
        bool CycleMvR    (void); //(pRb->wk1     )
        bool CycleMv1    (void); //(     wk1->wk2)
        bool CycleMv2    (void); //(          wk2)->pSb

        bool CycleWK1Up  (void);
        bool CycleWK2Up  (void);
*/
        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool IsLoad);

};

//---------------------------------------------------------------------------
extern CRail RAL;

//---------------------------------------------------------------------------
#endif
