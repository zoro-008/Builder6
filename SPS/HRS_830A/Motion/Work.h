//---------------------------------------------------------------------------

#ifndef WorkH
#define WorkH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
//---------------------------------------------------------------------------
class CWork
{
    public:
        //Constructor
        CWork (void);
        ~CWork (void);

        void Init();
        void Close();

	void Reset     (); //���V ��ư �������� Ÿ�� �Լ�.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd     ;
            bool bReqStop     ;
        };

        enum EN_SEQ_CYCLE {
            scIdle    =  0 , //���.
            scIn      =  1 , //����.
            scWork    =  2 , //���ϱ�.
            scOut     =  3 , //������.
            MAX_SEQ_CYCLE
        };

        enum EN_GETFINDCHIP {
            fcCol      =  0 ,
            fcRowOdd   =  1 ,
            fcRowEven  =  2 ,
        };

        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        void InitCycleName(){
            m_sCycleName[scIdle ]="Idle" ;
            m_sCycleName[scIn   ]="In  " ;
            m_sCycleName[scWork ]="Work" ;
            m_sCycleName[scOut  ]="Out " ;
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
        CDelayTimer m_tmLDisps  ;
        CDelayTimer m_tmRDisps  ;
        CDelayTimer m_tmTemp    ;

        SStat Stat ;
        SStep Step , PreStep ;

        bool   m_bNeedStrip  ;
        int    m_iDisprCnt   ;
        double m_dLastIndPos ;

    public:    /* Direct Accessable Var.  */
        bool   FindChip   (int &r , int &c , EN_GETFINDCHIP iFCNum);
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

        bool CheckSafe (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool CheckSafe (EN_ACTR_ID _iActr , bool       _bFwd    );

        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int           GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome = 0 ;}
        int           GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int           GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int           GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        AnsiString    GetCycleName   (){return m_sCycleName[Step.iSeq];}
        SStat         GetStat        (){return Stat          ;}
        AnsiString    GetPartName    (){return m_sPartName   ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

        //�������ۿ��� �ʿ�� �ϴ� �÷���...
        __property bool _bNeedStrip   = { read = m_bNeedStrip };

        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // ���͸� �����϶� ���� �Լ�.
        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.

        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

        //Running Functions.
        bool ToStopCon (void); //��ž�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStartCon(void); //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStart   (void); //��ŸƮ�� �ϱ� ���� �Լ�.
        bool ToStop    (void); //��ž�� �ϱ� ���� �Լ�.
        bool Autorun   (void); //���䷱�׽ÿ� ��� Ÿ�� �Լ�.

        //One Cycle.
        bool CycleIn   ();
        bool CycleWork ();
        bool CycleOut  ();
        bool CycleSingleWrk  ();

        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool _bLoad );

        int    m_iShotCnt;

        double m_dWorkCycleTime;

        __property int   _iDisprCnt      = { read = m_iDisprCnt    , write= m_iDisprCnt };


};

//---------------------------------------------------------------------------
extern CWork WRK;
//---------------------------------------------------------------------------
#endif
