//---------------------------------------------------------------------------

#ifndef PickerH
#define PickerH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
#include "uRs232c.h"
#include "OptionMan.h"

//---------------------------------------------------------------------------
#define MAX_VALUE_GRAPH_CNT 40 //5��ũ�� �� ���� �Ѵ�.
#define MAX_BIN_GRAPH_CNT   10 //bin 0,1,2,3,4,5


class CPicker
{
    public:
        //Constructor
        CPicker (void);
        ~CPicker (void);

        void Init();
        void Close();
        void Reset     (); //���V ��ư �������� Ÿ�� �Լ�.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd    ;
            bool bReqStop    ;
        };

        enum EN_SEQ_CYCLE {
            scIdle       =  0 ,
            scRezero     = 10 ,
            scPick       = 20 ,
            scPlace      = 30 ,
            scCheck      = 40 ,
            scRezeroCheck= 50 ,
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
        AnsiString  m_sPartName  ;
        //Timer.
        CDelayTimer m_tmMain    ;
        CDelayTimer m_tmCycle   ;
        CDelayTimer m_tmHome    ;
        CDelayTimer m_tmToStop  ;
        CDelayTimer m_tmToStart ;
        CDelayTimer m_tmTemp    ;
        CDelayTimer m_tmTemp2   ;

        SStat Stat ;
        SStep Step , PreStep ;

        bool FindChip(EN_CHIP_STAT _iStat , int &r , int &c ) ;


	double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );



    public:    /* Direct Accessable Var.  */
        double m_dWorkCycleTime                 ;
        double m_dZeroPoint     [MAX_PICKER_CNT]; //�����νÿ� ������ ������ ���� ��� �ߴٰ� �ٲ�� ���� �Ѵ�.
        double m_dZeroPointCheck[MAX_PICKER_CNT]; //�����νÿ� ������ ������ ���� ��� �ߴٰ� �ٲ�� ���� �Ѵ�.
        //int    m_iPickCnt                       ; //�� ī��Ʈ �ö� ���鼭 ������ ����
        int    m_iFindRow                       ;
        int    m_iFindCol                       ;


        EN_GRADE_STAT GetBin     (double _dVal);
        int           GetGraphBin(double _dVal);


        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //���Ͱ� �����϶� �浹���� ����.
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    ); //�Ǹ����� �����϶� �浹���� ����.

        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} int InitHomeStep (){ return Step.iHome  = 10 ;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} int InitCycleStep(){ return Step.iCycle = 10 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        SStat        GetStat        (){return Stat          ;}

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
        bool CyclePick       (void); //���� ���� ����Ŭ.
        bool CyclePlace      (void); //���� �������� �˻��ϴ� ����Ŭ.
        bool CycleRezero     (void); //��� ���� ������ ������ �ϴ� ����Ŭ. �޴���.
        bool CycleRezeroCheck(void); //
        bool CycleCheck      (void); //���� ���� ��� �� �˻��ϰ� �ٽ� ������ ����.
        bool CycleStation    (void); //������ �����̼ǿ� ���� ���̸� �����ϴ� �޴��� ����. �ѹ� ����ٰ� �����鼭 ����.

        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool IsLoad );

        static void procCom1(DWORD _cbInQue);


        void ClearGraphData(){memset(m_iValGraphCnt , 0 , sizeof(int)*MAX_VALUE_GRAPH_CNT);
                              memset(m_iBinGraphCnt , 0 , sizeof(int)*MAX_BIN_GRAPH_CNT  );}
        int m_iValGraphCnt[MAX_VALUE_GRAPH_CNT] ; //ȭ�鿡 ������ ��Ÿ���ַ��� ������.
        int m_iBinGraphCnt[MAX_BIN_GRAPH_CNT  ] ; //ȭ�鿡 �ѷ��ַ��� ���� �� ī��Ʈ ������ .
        AnsiString m_sPreDate ;


        TRS232C * Rs232_H ;
        double m_dPrbData[MAX_PICKER_CNT] ;
        static void procComH(DWORD _cbInQue);





};

//---------------------------------------------------------------------------
extern CPicker PCK;

//---------------------------------------------------------------------------
#endif
