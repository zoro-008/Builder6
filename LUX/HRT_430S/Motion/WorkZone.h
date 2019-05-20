//---------------------------------------------------------------------------
#ifndef WorkZoneH
#define WorkZoneH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
#include "LotUnit.h"
#include "uRs232C.h"
//---------------------------------------------------------------------------
#define MAX_PROB_COL 4
extern int iPinFailCnt[MAX_ARRAY_ROW][MAX_PROB_COL]  ;

enum EN_VISN_ERR_STAT { //���� �� �� �ϰ� �ִ� ����.
    vsNomal         = 0 , //ó�� �˻�.
    vsErrReTry      = 1 , //�ٽ� �˻�.
    vsErrSkip       = 2 , //���� ��ŵ.
    vsGood          = 3 ,
};


class CWorkZone
{
    public:
        //Constructor
        CWorkZone (void);
        ~CWorkZone (void);

        void Reset     (); //���V ��ư �������� Ÿ�� �Լ�.
        void ResetTimer();
        void Init (void);
        void Close(void);

        //Vars.
        struct SStat {
            bool bWorkEnd  ;
            bool bReqStop  ;
            bool bRepeat   ; //�޴��� �����߿� ����Ʈ ���ۿ�.
        };

        enum EN_SEQ_CYCLE {
            scIdle          =  0 ,
            scPreWork       = 10 ,
            scVisn          = 20 ,
            scWork          = 30 ,
            scWkReGrip      = 40 ,
            scPostWork      = 50 ,
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
        AnsiString m_sPartName  ;
        //Timer.
        CDelayTimer m_tmMain    ;
        CDelayTimer m_tmCycle   ;
        CDelayTimer m_tmHome    ;
        CDelayTimer m_tmToStop  ;
        CDelayTimer m_tmToStart ;
        CDelayTimer m_tmTemp    ;
        CDelayTimer m_tmCont1   ;
        CDelayTimer m_tmCont2   ;

        SStat Stat ;
        SStep Step , PreStep ;


        int    m_iReGripCnt  ;
        double m_dLastIndPos ;


        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

    public:    /* Direct Accessable Var.  */
        AnsiString m_sLastStrip ;

        //Manual���� ���.

        __property int _iReGripCnt = { read = m_iReGripCnt                           };
        bool   FindChip   (int &r , int &c );

        bool           m_bDispLotEnded ; //ȭ�鿡 ������ �� �����.
        int            m_iDataErrCnt   ; //������ ���� �̻󿡷�... �׽�Ʈ��..
        int            m_iDataErrCntRe ;
        CLot::SLotInfo EndedLot        ;

        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int           GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){Step.iHome  = 10 ; PreStep.iHome = 0 ;}
        int           GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int           GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int           GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
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
        bool CyclePreWork    ();
        bool CycleVisn       ();
        bool CycleWork       ();
        bool CycleWkReGrip   ();
        bool CyclePostWork   ();

        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool IsLoad );

        //CallBack ��� Rs232
        static void HeatProbe1Ch(DWORD _cbInQue);
        static void HeatProbe2Ch(DWORD _cbInQue);

        bool bPb1Rslt ;
        bool bPb2Rslt ;
        AnsiString sPb1Msg ;
        AnsiString sPb2Msg ;

        String sPb1Bin[2] ;
        String sPb2Bin[2] ;
        String sPb1Vf [2] ;
        String sPb2Vf [2] ;

        // juhyeon ������ ���� Vf ���� ���� ����
        String sTempPb1Vf[2] ;
        String sTempPb2Vf[2] ;

        // Zenner �� VF �� ������ �ޱ� ���� ��� ���� ����
        String sPb1ZnrVf[2] ;
        String sPb2ZnrVf[2] ;

        int  m_iVisnErr      ; // Vision Error ��ŵ��.
        bool m_bVisnErrDisp  ; // Vision Error �ÿ� ���÷���

};

//---------------------------------------------------------------------------
extern CWorkZone WRK;

extern TRS232C *RS232C_Pb1ch  ;
extern TRS232C *RS232C_Pb2ch  ;

//---------------------------------------------------------------------------
#endif
