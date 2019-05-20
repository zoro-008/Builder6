//---------------------------------------------------------------------------

#ifndef MarkVisnH
#define MarkVisnH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"

//---------------------------------------------------------------------------
class CMarkVisn
{
    public:
        //Constructor
        CMarkVisn (void);
        ~CMarkVisn (void);

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
            scIdle  =  0 , //���.
            scVisn  =  1 , //������� Ȯ�� ����.
            scTrain =  2 , //������ ù���� Ʈ����.

            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        void InitCycleName(){
            m_sCycleName[scIdle ]="Idle " ;
            m_sCycleName[scVisn ]="Visn " ;
            m_sCycleName[scTrain]="Train" ;
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

        bool   GetLtToRt  ();
        //bool   FindChip   (EN_ARAY_ID _iAray , int &r , int &c   );
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

        int    m_iLastInspRow ;
        String m_sTrainedLot ;

    public:    /* Direct Accessable Var.  */
        bool   FindChip   (EN_ARAY_ID _iAray , int &r , int &c   );
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

        //������ �˻翭
        __property int    _iLastInspRow  = { read = m_iLastInspRow   };
        __property String _sTrainedLot   = { read = m_sTrainedLot    };

        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow = false); // ���͸� �����϶� ���� �Լ�.
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

        //Ajin Trg ����.
        void SetTrgPos();
        void ResetTrgPos();

        //Visn Rslt Data
        bool GetVisnRslt(EN_ARAY_ID _iAray , int _iRow);

        //Vision Cycle .
        bool CycleVisn ();
        bool CycleTrain();

        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool _bLoad );

        bool VisnJobChange();
        bool SaveVisnDevice(AnsiString _sDevice);


        //int m_mmm;
};

extern bool g_bMarkChangeFinish ;

//---------------------------------------------------------------------------
extern CMarkVisn VSN;
//---------------------------------------------------------------------------
#endif
