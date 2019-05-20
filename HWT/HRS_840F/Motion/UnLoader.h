//---------------------------------------------------------------------------

#ifndef UnLoaderH
#define UnLoaderH
//---------------------------------------------------------------------------
#include "PartInterface.h"

#include "Timer.h"
#include "DataMan.h"


//---------------------------------------------------------------------------
class CUnLoader : public CPartInterface
{
    public: //�ʱ�ȭ ���.
        //Constructor
        CUnLoader ();
        ~CUnLoader ();

        void Init(bool _bFront );
        void Close();


    public:

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle       =  0 ,
            scSupply     =  1 ,
            scIn         =  2 ,
            scWork       =  3 ,


            MAX_SEQ_CYCLE
        };

        int        m_iCycleSttTime             ;
        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //������ ����Ŭ �ð�. ms
        void   InitCycleName(){
            m_sCycleName[scIdle  ]   = "Idle    " ;
            m_sCycleName[scSupply]   = "Supply  " ;
            m_sCycleName[scIn    ]   = "In      " ;
            m_sCycleName[scWork  ]   = "Work    " ;
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

        SStat  Stat ;
        SStep  Step , PreStep ;

        double m_dLastIdxPos ;

        String m_sCheckSafeMsg ;

        void   ResetTimer();

        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

        //�Ȱ��� ���� 2���� �ε����� ������ �ִ´�.
        struct SAdd{
            EN_PART_ID     iPartId               ;
            EN_ARAY_ID     riULD                 ;
            EN_ARAY_ID     riPCK                 ;

            EN_ACTR_ID     aiULD_FoldUpDn        ;           
            EN_ACTR_ID     aiULD_PickUpFwBw      ;           
            EN_ACTR_ID     aiULD_RotateFwBw      ;           
            EN_ACTR_ID     aiULD_PackOpenFwBw    ;
            EN_ACTR_ID     aiULD_PackSrtFwBw     ;

            EN_INPUT_ID    xULD_MaskDetect       ;
            EN_INPUT_ID    xULD_PackVccm         ;
            EN_INPUT_ID    xULD_PickVccm         ;

            EN_OUTPUT_ID   yULD_ConMotorCw       ;
            EN_OUTPUT_ID   yULD_PickUpVccm       ;
            EN_OUTPUT_ID   yULD_PickUpEjet       ;
            EN_OUTPUT_ID   yULD_PackOpenVccm     ;
            EN_OUTPUT_ID   yULD_PackOpenEjet     ;

            bool bRunSkip ;

        };
        SAdd Add ;

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

        bool         Autorun        (); //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
        //======================================================================
    public:
        bool   FindChip   (int &r , EN_ARAY_ID &id);

        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

        // Cycle.
        bool CycleIn     ();
        bool CycleWork   ();
        bool CycleSupply ();

        bool CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        void  SetStat(SStat _sStat) { Stat = _sStat ; }
        SStat GetStat(            ) { return Stat;    }

        //Motor.
        //bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // ���͸� �����϶� ���� �Լ�.
        void MoveIncMotr (EN_MOTR_ID _iMotr , double _iPstn); // ���͸� �����϶� ���� �Լ�.

        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow = false);
        
        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.

        //For Start Stop Checking.
        void SetLastCmd (); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool _bLoad);

        // Bin ����
        int m_iBin1Cnt ; // Bin Clear �ϱ� ���� �߰�
        int m_iBin2Cnt ;

        int  GetBin1Cnt  () { return m_iBin1Cnt ; } ;
        int  GetBin2Cnt  () { return m_iBin2Cnt ; } ;

        void ClearBinCnt() {
            m_iBin1Cnt = 0;
            m_iBin2Cnt = 0;
        } ;
        
        void ClearBin1Cnt() { m_iBin1Cnt = 0; } ; // Bin ���� �߰�
        void ClearBin2Cnt() { m_iBin2Cnt = 0; } ; // Bin ���� �߰�

};

//---------------------------------------------------------------------------
extern CUnLoader ULD_F;
extern CUnLoader ULD_R;
//---------------------------------------------------------------------------
#endif
