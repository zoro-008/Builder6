//---------------------------------------------------------------------------

#ifndef LearLaserH
#define LearLaserH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "PartInterface.h"

#include "Timer.h"
#include "DataMan.h"
//---------------------------------------------------------------------------
class CLearLaser : public CPartInterface
{
    public:
        //Constructor
        CLearLaser (void);
        ~CLearLaser (void);

        void Init();
        void Close();

    public:
        //Vars.
        struct SStat {
            bool bWorkEnd     ;
            bool bReqStop     ;
        };

        enum EN_SEQ_CYCLE {
            scIdle       =  0 , //���.
            scWork       =  1 , //
            scPlace      =  2 , //
            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ;
        void InitCycleName(){
            m_sCycleName[scIdle ]="Idle "  ;
            m_sCycleName[scWork ]="Work "  ;
            m_sCycleName[scPlace]="Place"  ;
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

        SStat Stat ;
        SStep Step , PreStep ;

        SStat GetStat(){return Stat;}

        void ResetTimer();

        bool   FindChip   (int &r , int &c   );

        String m_sLaserWorkLotNo ;

    public: //�������̽� ���.==================================================
	void Reset (); //���V ��ư �������� Ÿ�� �Լ�.

        //Running Functions.
        bool ToStopCon (); //��ž�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStartCon(); //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStart   (); //��ŸƮ�� �ϱ� ���� �Լ�.
        bool ToStop    (); //��ž�� �ϱ� ���� �Լ�.

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;}void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome  = 0 ;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        int          GetSeqStep     (){return (int)Step.iSeq;} int GetPreSeqStep    (){return (int)PreStep.iSeq;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;}void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}

        AnsiString   GetCycleName   (int _iSeq){return m_sCycleName[_iSeq];}
        int          GetCycleTime   (int _iSeq){return m_iCycleTime[_iSeq];}
        AnsiString   GetPartName    (){return m_sPartName   ;}

        int          GetCycleMaxCnt (){return MAX_SEQ_CYCLE ;}

        bool Autorun   (); //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
        //======================================================================

    public:    /* Direct Accessable Var.  */
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

        //One Cycle.
        bool CycleWork  ();
        bool CyclePlace ();

        bool CheckSafe (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool CheckSafe (EN_ACTR_ID _iActr , bool       _bFwd    );

        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

        //�������� �������� LotNo ;
        __property String _sLaserWorkLotNo  = { read = m_sLaserWorkLotNo   };

        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // ���͸� �����϶� ���� �Լ�.
        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.

        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool _bLoad );

        bool       m_bTestMark ;
        bool       m_bAutoRun  ;
};
//---------------------------------------------------------------------------
extern CLearLaser LLS;
//---------------------------------------------------------------------------
#endif
