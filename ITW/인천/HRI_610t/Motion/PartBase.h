//---------------------------------------------------------------------------

#ifndef PartBaseH
#define PartBaseH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"

//---------------------------------------------------------------------------
class CPartInterface
{
    public:
        //Constructor
        CPartInterface (void); //���� ������ ���� �ʱ� ������ �ʱ�ȭ����Ʈ ��� ����.
        virtual ~CPartInterface (void);

        //���߾��� �׳ɽᵵ�ǰ� �߰� ��� �����ÿ� ������ �ؼ� ���� CPartBase�� �Լ� �����ϰ� �ڽ��� �߰� �Ұ� �Է�.
        virtual void Init();
        virtual void Close();

	virtual void Reset     (); //���V ��ư �������� Ÿ�� �Լ�.
        virtual void ResetTimer();

        virtual bool   FindChip  (EN_ARAY_ID _riAray, EN_CHIP_STAT _csStat , int &r , int &c )const=0;
        virtual double GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )const;
        virtual bool   CheckSafe (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )const;
        virtual bool   CheckSafe (EN_ACTR_ID _iActr , bool       _bFwd    )const;

        //Motor.
        virtual bool MoveMotr (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)const=0; // ���͸� �����϶� ���� �Լ�.
        //Actuator.
        virtual bool MoveActr (EN_ACTR_ID _iActr , bool _bFwd)const=0; //�Ǹ����� �����϶� ���� �Լ�.

        //HomeCycle
        virtual bool CycleHome()=0; //��Ʈ Ȩ�� ������ ���� �Լ�.

        //Running Functions.
        bool ToStopCon (void); //��ž�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStartCon(void); //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStart   (void); //��ŸƮ�� �ϱ� ���� �Լ�.
        bool ToStop    (void); //��ž�� �ϱ� ���� �Լ�.
        bool Autorun   (void); //���䷱�׽ÿ� ��� Ÿ�� �Լ�.

        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool _bLoad);












        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
        };

        struct SStep {
            int          iHome    ;
            int          iToStart ;
            int          iSeq     ;
            String       sSeqName ;
            int          iCycle   ;
            int          iToStop  ;
        };

    protected:
        String      m_sPartName ;
        //Timer.
        CDelayTimer m_tmMain    ;
        CDelayTimer m_tmCycle   ;
        CDelayTimer m_tmHome    ;
        CDelayTimer m_tmToStop  ;
        CDelayTimer m_tmToStart ;
        CDelayTimer m_tmDelay   ;

        SStat Stat ;
        SStep Step , PreStep ;




    public:    /* Direct Accessable Var.  */



        bool GetWorkEnd()const{return Stat.bWorkEnd ;}
        bool GetReqStop()const{return Stat.bReqStop ;}

        int    GetHomeStep    ()const{return Step.iHome    ;} int GetPreHomeStep   ()const{return PreStep.iHome    ;} void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome = 0 ;}
        int    GetToStartStep ()const{return Step.iToStart ;} int GetPreToStartStep()const{return PreStep.iToStart ;}
        int    GetSeqStep     ()const{return Step.iSeq     ;} int GetPreSeqStep    ()const{return PreStep.iSeq     ;}
        int    GetSeqName     ()const{return Step.sSeqName ;} int GetPreSeqName    ()const{return PreStep.sSeqName ;}
        int    GetCycleStep   ()const{return Step.iCycle   ;} int GetPreCycleStep  ()const{return PreStep.iCycle   ;} void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int    GetToStopStep  ()const{return Step.iToStop  ;} int GetPreToStopStep ()const{return PreStep.iToStop  ;}
        SStat  GetStat        ()const{return Stat          ;}
        String GetPartName    ()const{return m_sPartName   ;}

};

//---------------------------------------------------------------------------
#endif
