//---------------------------------------------------------------------------
#ifndef LoaderH
#define LoaderH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
//---------------------------------------------------------------------------
class CLoader
{
    public:
        //Constructor
         CLoader ();
        ~CLoader ();

        void Init      ();
        void Close     ();
        void Reset     (); //���V ��ư �������� Ÿ�� �Լ�.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle       = 0 ,
            scSupply     = 1 ,
            scPick       = 2 ,
            scWork       = 3 ,
            scPush       = 4 ,
            scPushBack   = 5 ,
            scLdrYHome   = 6 ,
            scPlace      = 7 ,
            scPitchUp    = 8 , //�޴���.
            scPitchDn    = 9 ,
            MAX_SEQ_CYCLE
        };
        AnsiString m_sCycleName [MAX_SEQ_CYCLE] ;
        void InitCycleName(){
            m_sCycleName[scIdle    ] = "Idle    ";
            m_sCycleName[scSupply  ] = "Supply  ";
            m_sCycleName[scPick    ] = "Pick    ";
            m_sCycleName[scWork    ] = "Work    ";
            m_sCycleName[scPush    ] = "Push    ";
            m_sCycleName[scPushBack] = "PushBack";
            m_sCycleName[scLdrYHome] = "LdrYHome";
            m_sCycleName[scPlace   ] = "Place   ";
            m_sCycleName[scPlace   ] = "PitchUp ";
            m_sCycleName[scPlace   ] = "PitchDn ";
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
        CDelayTimer m_tmTemp    ;

        SStat Stat ;
        SStep Step , PreStep ;

        int    m_iMgzCnt ;

        bool   m_bRqstLdrYHome ;

        int m_iHomeTO  ;
        int m_iStartTO ;
        int m_iStopTO  ;
        int m_iCycleTO ;

        bool   FindChip   (EN_ARAY_ID _riWhre , int &r , int &c   );
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

    public:    /* Direct Accessable Var.  */
        double GetWorkPos (EN_MOTR_ID _iMotr);    

        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

  
        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome = 0 ;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        SStat        GetStat        (){return Stat          ;}
        AnsiString   GetCycleName   (){return m_sCycleName[Step.iSeq];}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

        //Property.
        __property int   _iMgzCnt      = { read = m_iMgzCnt    , write= m_iMgzCnt };
        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlowSpeed = false); // ���͸� �����϶� ���� �Լ�.
        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.

        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

        //Running Functions.
        bool ToStopCon (void); //��ž�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStartCon(void); //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStart   (void); //��ŸƮ�� �ϱ� ���� �Լ�.
        bool ToStop    (void); //��ž�� �ϱ� ���� �Լ�.
        bool Autorun   (void); //

        //One Cycle.
        bool CycleSupply  ();
        bool CyclePick    ();
        bool CycleWork    ();
        bool CyclePush    ();
        bool CyclePushBack();
        bool CycleLdrYHome();
        bool CyclePlace   ();

        //Manual
        bool CyclePitchUp ();
        bool CyclePitchDn ();

        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool IsLoad);

};

//---------------------------------------------------------------------------
extern CLoader LDR;
//---------------------------------------------------------------------------
#endif
