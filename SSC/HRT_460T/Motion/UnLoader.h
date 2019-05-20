//---------------------------------------------------------------------------

#ifndef UnLoader430tH
#define UnLoader430tH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
#include "PartInterface.h"

//---------------------------------------------------------------------------
class CUnLoader : public CPartInterface
{
    public:
        //Constructor
        CUnLoader (void);
        ~CUnLoader (void);

        void Init   ();
        void Close  ();

    public:

        //Vars.
        struct SStat {
            bool bWorkEnd     ;
            bool bReqStop     ;
            bool bReadyMgz    ;
            bool bChckSplyEmpty ; //���ö������� �����̾� ��Ʈ �������� ī��Ʈ������ Ȯ���� �����̴�.
        };

        enum EN_SEQ_CYCLE {
            scIdle   =  0 ,
            scSupply =  1 ,
            scPick   =  2 ,
            scWork   =  3 ,
            scPlace  =  4 ,
            MAX_SEQ_CYCLE
        };

        int        m_iCycleSttTime             ;
        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //������ ����Ŭ �ð�. ms
        void   InitCycleName(){
            m_sCycleName[scIdle  ]="Idle  ";
            m_sCycleName[scSupply]="Supply";
            m_sCycleName[scPick  ]="Pick  ";
            m_sCycleName[scWork  ]="Work  ";
            m_sCycleName[scPlace ]="Place ";
        }
        void   InitCycleTime(){
            memset(&m_iCycleTime  , 0 , sizeof(int )*MAX_SEQ_CYCLE);
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

        CDelayTimer m_tmHeat    ; //�����ε������� ���ް� �ִ� �ð�. �̽ð��� ���� �ð� �̻� �Ǿ�� �۾��� �����Ѵ�.

        SStat Stat ;
        SStep Step , PreStep ;

        double m_dLastIdxPos ;

        String m_sCheckSafeMsg ;

        double GetLastCmd(EN_MOTR_ID _iMotr);

        void   ResetTimer();

        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

        AnsiString m_sWorkedLotNo ;

    public: //�������̽� ���.==================================================
        void Reset (); //���V ��ư �������� Ÿ�� �Լ�.

        //Running Functions.
        void Update    ();
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

        bool         Autorun        (); //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
        //======================================================================
    public:
        int    m_iMgzCnrtNo;
        int    m_iMgzCnt ;
        bool   FindChip   (int &r , int &c   );

    public:    /* Direct Accessable Var.  */
        bool   m_bIsHome;

        SetMgzCnt(int _iCnt){m_iMgzCnt = _iCnt ;};
        int    GetMgzCnt() {return m_iMgzCnt;};

        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );


        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

        //Property.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        __property int   _iMgzCnrtNo   = { read = m_iMgzCnrtNo                    };
        __property int   _iMgzCnt      = { read = m_iMgzCnt    , write= m_iMgzCnt };
        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // ���͸� �����϶� ���� �Լ�.
        void MoveIncMotr (EN_MOTR_ID _iMotr , double _iPstn); // ���͸� �����϶� ���� �Լ�.
        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.

        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

        //One Cycle.
        bool CycleSupply  ();
        bool CyclePick    ();
        bool CycleWork    ();
        bool CyclePlace   ();

        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool IsLoad);

};

//---------------------------------------------------------------------------
extern CUnLoader ULD;
//---------------------------------------------------------------------------
#endif
