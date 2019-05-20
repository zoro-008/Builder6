//---------------------------------------------------------------------------

#ifndef PostBuffH
#define PostBuffH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"

//---------------------------------------------------------------------------
class CPostBuff
{
    public:
        //Constructor
        CPostBuff (void);
        ~CPostBuff (void);
	void Reset     (); //���V ��ư �������� Ÿ�� �Լ�.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
            bool bInWorkEnd ;
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle       =  0 ,
            scDrag       = 10 ,
            scGood       = 20 ,
            scWork       = 30 ,
            scReGrip     = 40 ,
            scOut        = 50 ,
            scPreDrag    = 60 ,
            MAX_SEQ_CYCLE
        };

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
        CDelayTimer m_tmTemp    ;

        SStat Stat ;
        SStep Step , PreStep ;

        int    m_iReGripCnt ;
        double m_dLastIndPos  ;
        double m_dFailChipPos ;
        int    m_iCrntCol     ;

        //bool   FindChip   (int &r , int &c , EN_CHIP_STAT Stat  );
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , EN_CHIP_STAT Mode = csFail);

    public:    /* Direct Accessable Var.  */
        

        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        bool   GetRegribCnt (){return m_iReGripCnt;}

        bool   GetReadyPos(EN_CHIP_STAT Mode = csFail);

        int           GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){Step.iHome  = 10 ; PreStep.iHome = 0 ;}
        int           GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int           GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int           GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        SStat         GetStat        (){return Stat          ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }
        bool   FindChip   (int &r , int &c , EN_CHIP_STAT Stat  );
        //Property.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        __property int   _iReGripCnt   = { read = m_iReGripCnt                 };
        __property int   _iReGripCnt   = {                   write = m_iReGripCnt};
        __property int   _iCrntCol     = {read = m_iCrntCol                      };

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
        bool CyclePreDrag();
        bool CycleDrag   ();
        bool CycleGood   ();
        bool CycleWork   ();
        bool CycleReGrip ();
        bool CycleOut    ();

        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool IsLoad );

};

//---------------------------------------------------------------------------
extern CPostBuff PSB;
//---------------------------------------------------------------------------
#endif
