//---------------------------------------------------------------------------

#ifndef IndexH
#define IndexH
//---------------------------------------------------------------------------
#include "PartInterface.h"
#include "UnLoader.h"

#include "Timer.h"
#include "DataMan.h"

//---------------------------------------------------------------------------
class CIndex : public CPartInterface
{
    public: //�ʱ�ȭ ���.
        //Constructor
        CIndex ();
        ~CIndex ();

        void Init();
        void Close();


    public:

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
            bool bDontMove;
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle       =  0 ,                                                 
            scGet        =  1 ,                                                 
            scRotator    =  2 ,
            MAX_SEQ_CYCLE
        };

        int        m_iCycleSttTime             ;
        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //������ ����Ŭ �ð�. ms
        void   InitCycleName(){
            m_sCycleName[scIdle   ]="Idle"    ;
            m_sCycleName[scGet    ]="Get"     ;
            m_sCycleName[scRotator]="Rotator" ;
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

        //Front Rail�� �ǹ� ����.
        CDelayTimer m_tmHeat    ; //�����ε������� ���ް� �ִ� �ð�. �̽ð��� ���� �ð� �̻� �Ǿ�� �۾��� �����Ѵ�.
    

        SStat  Stat ;
        SStep  Step , PreStep ;

        double m_dLastIdxPos ;

        String m_sCheckSafeMsg ;

        bool   m_bPassedDetect4 ; //Out�� Detect4�� ���� �ƾ�����.

        double GetLastCmd(EN_MOTR_ID _iMotr);

        void   ResetTimer();


        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

        //�Ȱ��� ���� 2���� �ε����� ������ �ִ´�.
//        struct SAdd{
//            EN_PART_ID     iPartId               ;
//            EN_ARAY_ID     riIDX                 ;
//            EN_MOTR_ID     miIDX_XIndx           ;
//            EN_PSTN_VALUE  pvIDX_XIndxWait       ;
//            EN_PSTN_VALUE  pvIDX_XIndxGet        ;
//            EN_PSTN_VALUE  pvIDX_XIndxWorkStart  ;
//            EN_PSTN_VALUE  pvIDX_XIndxOut        ;
//            EN_PSTN_ID     piIDX_XIndxWait       ;
//            EN_PSTN_ID     piIDX_XIndxGet        ;
//            EN_PSTN_ID     piIDX_XIndxWorkStart  ;
//            EN_PSTN_ID     piIDX_XIndxOut        ;
//            EN_PSTN_ID     piIDX_XIndxWork       ;
//            EN_INPUT_ID    xIDX_WrkDetect3       ;
//            EN_INPUT_ID    xIDX_Detect4          ;
//            EN_INPUT_ID    xIDX_IndxNotOvLd      ;
//            EN_ACTR_ID     aiIDX_IndxUpDn        ;
//            EN_CHIP_STAT   csAfterWork           ;
//
//            EN_ARAY_ID     riPSI                 ;
//
//            CUnLoader    * ULD                   ;
//            EN_ARAY_ID     riULDTop              ;
//            EN_ARAY_ID     riULDBtm              ;
//            EN_ACTR_ID     aiPushLtRt            ;
//            EN_ACTR_ID     aiPushUpDn            ;
//            EN_INPUT_ID    xULD_Detect5          ;
//
//            CPreIndex    * PRI                   ;
//            EN_ARAY_ID     riPRI                 ;
//
//            EN_MOTR_ID     miTool                ;
//            EN_PSTN_VALUE  pvToolWait            ;
//
//        };
//        SAdd Add ;

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
//        bool         FindChip   (int &c , EN_ARAY_ID &id);
        bool         IsWorkPos();

        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

        //One Cycle.
        bool CycleGet     ();
        bool CycleRotator ();

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


};

//---------------------------------------------------------------------------
extern CIndex IDX;
//---------------------------------------------------------------------------
#endif
