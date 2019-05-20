//---------------------------------------------------------------------------

#ifndef WorkH
#define WorkH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
#include "LotUnit.h"
//---------------------------------------------------------------------------
#define  MAX_BIN  5
enum EN_PRI_STEP {
    asNotReady   =  0 ,
    asNeedTrim        ,
    MAX_PRI_STEP
};

class CWork
{
    public:
        //Constructor
        CWork (void);
        ~CWork (void);
	void Reset     (); //���V ��ư �������� Ÿ�� �Լ�.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd ; //��Ʈ�� ������ ������� true ;
            bool bReqStop ; //������ ���ų� Ȥ�� ��ž��ư�� �������� ��Ʈ�� ���⸦ ��û�ϴ� ������.
        };

        enum EN_SEQ_CYCLE {
            scIdle   =  0  ,
            scWork   = 10  ,
            MAX_SEQ_CYCLE
        };

        struct SStep {
            int          iHome    ; //Ȩ����Ŭ���� ���� ����.
            int          iToStart ; //����ŸƮ ����Ŭ���� ��.
            EN_SEQ_CYCLE iSeq     ; //����� ���� ����.
            int          iCycle   ; //����� ���Ͽ��� ����.
            int          iToStop  ; //����ž ����Ŭ���� ��.

        };

    protected:
        AnsiString  m_sPartName ;
        //Timer.
        CDelayTimer m_tmMain    ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmCycle   ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmHome    ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmToStop  ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmToStart ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmTemp    ; //����Ŭ �Լ����� ������� ���¾�.

        SStat Stat ;              //
        SStep Step , PreStep ;

        bool   FindChip   (int &r , int &c );
	    double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //���Ͱ� ������ �������� ������. Double��.

    public:    /* Direct Accessable Var.  */

        void ClearBin    (int _iBinNo );
        bool CheckBin    (int _iBinNo );
        bool CheckBinFull(int _iBinNo );


        unsigned long  m_lStrpCycleTime ; //���� �۾� �ð�.... ȭ�� ���ǿ�.
        unsigned long  m_lPreCycleTime  ;

        //���� �۾� Ĩ ����.
        int    m_iBinCnt  ;
        int    m_iBin1Cnt ;
        int    m_iBin2Cnt ;
        int    m_iBin3Cnt ;
        int    m_iBin4Cnt ;
        int    m_iBin5Cnt ;

        int    m_iBinNo         ;
        String m_sPreLotNo      ;
        int    m_iTotalChip     ; //�̻��� ����� UI �� ���ߴ� ���� ����. dd
        int    m_iTotalFailChip ;
        int    m_iTotalFrameCnt ;
        int    m_iTotalToolCnt  ;
        bool   m_bLotEnded      ;
        bool   m_bLotEndCheck   ;//���� ������ �ޱ� ����JS

        CLot::SLotInfo EndedLot ;

        void Init ();
        void Close();



        bool   GetLastStrip();
        bool   LotInfoAdd  ();

        bool CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //���Ͱ� �����϶� �浹���� ����.
        bool CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    ); //�Ǹ����� �����϶� �浹���� ����.

        bool GetWorkEnd (){return Stat.bWorkEnd ;}
        bool GetReqStop (){return Stat.bReqStop ;}

        int           GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep    (){return PreStep.iHome       ;} void InitHomeStep (){Step.iHome  = 10;}
        int           GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep (){return PreStep.iToStart    ;}
        EN_SEQ_CYCLE  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep     (){return PreStep.iSeq        ;}
        int           GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep   (){return PreStep.iCycle      ;} void InitCycleStep(){Step.iCycle    = 10;}

        int           GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep  (){return PreStep.iToStop     ;}
        SStat         GetStat        (){return Stat          ;}


        void SetStat   (SStat _sStat) { Stat = _sStat ; }

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
        bool CycleWork   (void); //Ʈ�� �ѹ� ���.

        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool IsLoad);

};

//---------------------------------------------------------------------------
extern CWork WRK;

//---------------------------------------------------------------------------
#endif
