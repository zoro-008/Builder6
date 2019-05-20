//---------------------------------------------------------------------------

#ifndef SortingToolH
#define SortingToolH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"

#include <Grids.hpp>
#include <Series.hpp>

#define  MAXBIN  10
//---------------------------------------------------------------------------
class CSortingTool
{
    public:
        //Constructor
        CSortingTool (void);
        ~CSortingTool (void);

        void Reset     (); //���V ��ư �������� Ÿ�� �Լ�.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd  ;
            bool bReqStop  ;
            bool bRepeat   ; //�޴��� �����߿� ����Ʈ ���ۿ�.
        };

        enum EN_SEQ_CYCLE {
            scIdle          =  0 ,
            scPunch         =  1 ,
            scDevChk        =  3 , //���� Ȯ��.
            scTrmChk        =  4 ,
            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        void   InitCycleName(){
            m_sCycleName[scIdle  ]="Idle  ";
            m_sCycleName[scPunch ]="Punch ";
            m_sCycleName[scDevChk]="DevChk";
            m_sCycleName[scTrmChk]="TrmChk";
        }

        struct SStep {
            int          iHome    ; //Ȩ����Ŭ���� ���� ����.
            int          iToStart ; //����ŸƮ ����Ŭ���� ��.
            EN_SEQ_CYCLE iSeq     ; //����� ���� ����.
            int          iCycle   ; //����� ���Ͽ��� ����.
            int          iToStop  ; //����ž ����Ŭ���� ��.
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

        SStat Stat              ;
        SStep Step , PreStep    ;

        bool       m_bLtDir       ;
        int        m_iPreCol      ;
        bool       m_bDir         ;
        double     m_dLastIndPos  ;
        AnsiString m_sLastID      ;

        double     m_dFailChipPos ;

        AnsiString  m_sCrentID;

        //bool   FindChip   (int &r , int &c , EN_CHIP_STAT Stat  );
        void   GetCrntStep(void              );

        bool GetReadyPos(EN_CHIP_STAT Mode);
        //void   PickDir    (int Cnt           );
        //double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

    public:    /* Direct Accessable Var.  */
        AnsiString m_sLastStrip ;
        int TopRowCnt;
        int BtmRowCnt;

        __property AnsiString   _sLastID   = {read = m_sLastID};
        __property int          _iBinCnt   = {read = m_iBinCnt , write = m_iBinCnt};
        double RejectMove (void              );

        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int           GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){Step.iHome  = 10 ; PreStep.iHome = 0 ;}
        int           GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int           GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int           GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        AnsiString    GetCycleName   (){return m_sCycleName[Step.iSeq];}
        SStat         GetStat        (){return Stat          ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

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
        bool CyclePunch    ();
        bool CycleDevCheck ();

        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void   Load(bool IsLoad );
        bool   FindChip   (int &r , int &c , EN_CHIP_STAT Stat  );
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

        int    m_iLastWorkCol   ;
        int    m_iLastWorkRow   ;
        int    m_iLastDecCol    ;
        int    m_iLastTrmDecCol ;
        int    m_iLastDecRow    ;
        int    m_iLastTrmDecRow ;

        int        m_iTrimCnt   ;
        AnsiString m_sLastLotNo ;
        int        m_iBinCnt    ;
        int        m_iBin0Cnt   ;
        int        m_iBin1Cnt   ;
        int        m_iBin2Cnt   ;
        int        m_iBin3Cnt   ;
        int        m_iBin4Cnt   ;
        int        m_iBin5Cnt   ;
        int        m_iBin6Cnt   ;
        int        m_iBin7Cnt   ;
        int        m_iBin8Cnt   ;
        int        m_iBin9Cnt   ;







};

//---------------------------------------------------------------------------
extern CSortingTool STL;

#endif

