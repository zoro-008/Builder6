//---------------------------------------------------------------------------

#ifndef FeedH
#define FeedH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
//---------------------------------------------------------------------------
class CFeed
{
    public:
        //Constructor
        CFeed (void);
        ~CFeed (void);

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
            scIdle     =  0 , //���.
            scHome     =  1 , // Ʈ������ �̵� ����Ŭ �߿� Ȩ����ֱ�
            scWait     =  2 , //���� �ȵ��ö� Ȥ�� ���������ִµ��� �ƿ��� �ȳ������� ��������.
            scWork     =  3 , //�ѷ��� ��Ʈ�� �о� �ִ°�.
            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        void   InitCycleName(){
            m_sCycleName[scIdle ]="Idle" ;
            m_sCycleName[scHome ]="Home" ;
            m_sCycleName[scWait ]="Wait" ;
            m_sCycleName[scWork ]="Work" ;
        }

        struct SStep {
            int          iHome    ;
            int          iToStart ;
            EN_SEQ_CYCLE iSeq     ;
            int          iCycle   ;
            int          iToStop  ;

            //���� �����.
            int          iTemp    ;
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

        AnsiString m_sWorkedLotNo ;

        double     m_dLastIndPos  ;

        bool   FindChip   (int &r , int &c , EN_ARAY_ID riTRS  );
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

        CDelayTimer m_tmFeedTimeOut      ; //�۾� ���� �ϰ� ���� �߰��� Feeding in TimeOut
        CDelayTimer m_tmFeedStartTimeOut ; //�۾� ���� �Ҷ� Feeding in Timeout
        CDelayTimer m_tmRqstLotEndTimer  ; //������ ��û�� ������ Ĩ���� ���� �̽ð� �Ŀ� WorkEnd��

        bool        m_bFirstChipIn ; //�������� �ǰ� ùĨ�� ������ true , ���� ùĨ �ȵ������� false .

        CDelayTimer m_tmInSolLRFlickTimer  ; //sol �ø� Ÿ�̸�....
        CDelayTimer m_tmInSolRLFlickTimer  ;
        CDelayTimer m_tmOutSolRLFlickTimer ;
        CDelayTimer m_tmInSolRLWaitTimer   ;

    public:    /* Direct Accessable Var.  */
        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

        AnsiString GetWorkedLotNo(){return m_sWorkedLotNo ;}

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
        bool CycleWait   ();
        bool CycleWork   ();


        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        bool TwstMT     (bool _bVal);
        //Loading Para.
        void Load(bool _bLoad );

        //ùĨ ���� ���� �Լ�
        void SetLotStarted(bool _bVal) { m_bFirstChipIn = true ; }
        bool GetLotStarted(void      ) { return m_bFirstChipIn ; }

        DWORD dCntTickTime;
        double dTickTime;

        int m_iSolFlickCnt;


        bool m_bOutSnsrDetect;
        bool m_bNeedFeedHome ;

        int m_iPitchCnt;
        int m_iVibrationCnt ;

        bool m_bOutFlick;

        //���Ͱ� �����̶� 1������ 36�� ��� ���ش� ������ ����.
        //�׷��� 360���� 1���� ���� �ϰ� 15���� ��� �ѹ������� ������
        //���� ���� �߻��Ǿ �ʉ�.
        //
        //�ذ�å.
        //Dll���� PulsePerRev = 100000
        //        UnitPerRev  = 100000 ���� ���� �� ������ ���� ������ �޽��� ���߰�
        //        ���ø����̼� �ܿ��� ȯ���ؼ� �޽��� �Ѹ��� 1���� ������ Ŭ���� �����ش�.

        //�޽����ش� 0.0036��
        //�̵�Ƚ��  ������ġ  ��������  �����޽�                            ��������
        //1         1          15       4166.6666666666666666666666666667   ��0.0024��
        //2         2          30       8333.3333333333333333333333333333   ��0.0012��
        //          .
        //          .
        //          .
        //24        24         360      100000                              0

        // miFED_TIdx ���ø����̼ǿ��� ���� ������ �̵��ÿ� ������ �ؿ� �Լ��� �¿��� ȯ����� MT_GoInc��������
        // 0 �����ǿ��� ���밪���� ������ �Ѵ�.
        // �׸��� 24��
        double Get100000From360(double _dAbsAngle); //0.0�������� Abs�������� �Է� �ؾ���.
        //������ ȯ���� ���� ����.
        double GetTCmdPos();
        DWORD m_dWorkTotalTm;
        DWORD m_dTempTm;

};

//---------------------------------------------------------------------------
extern CFeed FED;
//---------------------------------------------------------------------------
#endif
