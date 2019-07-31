//---------------------------------------------------------------------------
#ifndef StageH
#define StageH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
//---------------------------------------------------------------------------
//Vision
//---------------------------------------------------------------------------
#include "uRs232c.h"
//---------------------------------------------------------------------------



class CStage
{
    public:
        //Constructor
         CStage ();
        ~CStage ();

        void Init      ();
        void Close     ();
        void Reset     (); //���V ��ư �������� Ÿ�� �Լ�.
        void ResetTimer();

        //Vars. ��� ���� ���� �ܺο��� �ʿ��� ������..... ���̺� ����.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;

        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE { //����Ʈ ���䷱���� ��� �Ǵ� ����Ŭ�� ����.
            scIdle      = 0 ,
            scSupply    = 1 , //������ ����.
            scExpend    = 2 , //�ͽ����.
            scPlace     = 3 , //������ ����.
            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        void   InitCycleName(){
            m_sCycleName[scIdle  ]="Idle  ";
            m_sCycleName[scSupply]="Supply";
            m_sCycleName[scExpend]="Expend";
            m_sCycleName[scPlace ]="Place ";
        }

        struct SCycleTime { //����Ŭ �ð� ���� ���÷��̿�.
            double dStart ; //����Ŭ ��ŸƮ.
            double dPitch ; //����Ŭ ���� ����.
            double dWork  ; //����Ŭ �ҿ�ð�.
        };

        struct SStep { //�⺻�� ����Ŭ�鿡�� ���ܵ�.
            int          iHome    ;  //��Ʈ Ȩ��� ����Ŭ.
            int          iToStart ;  //��ŸƮ ��ư �������ÿ� ���䷱ ���� ������ �غ� ����Ŭ...
            EN_SEQ_CYCLE iSeq     ;  //
            int          iCycle   ;
            int          iToStop  ;
        };

    protected:
        AnsiString m_sPartName  ; //���� ��Ʈ �̸� ex) "STAGE"
        //Timer.
        CDelayTimer m_tmMain    ;
        CDelayTimer m_tmCycle   ;
        CDelayTimer m_tmHome    ;
        CDelayTimer m_tmToStop  ;
        CDelayTimer m_tmToStart ;
        CDelayTimer m_tmTemp    ; //Cycle���� �ð� ������ �ʿ��Ҷ� ��.

        SStat Stat ;
        SStep Step , PreStep ;

        SCycleTime Time[MAX_SEQ_CYCLE] ;

        int m_iHomeTO  ;
        int m_iStartTO ;
        int m_iStopTO  ;
        int m_iCycleTO ;

        void SetStartTime(EN_SEQ_CYCLE _iSeqCycle);
        void SetEndTime  (EN_SEQ_CYCLE _iSeqCycle);

        bool   FindChip   (EN_ARAY_ID _riWhre , int &r , int &c , EN_CHIP_STAT _csStats);
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

        static void CallbackSTGRightClickAray(int _iCol , int _iRow , TShiftState Shift);

        void (__closure*CallbackAray)(int _iCol ,int _iRow , TShiftState Shift);

    public:    /* Direct Accessable Var.  */
        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

        AnsiString sMainWfName  ;
        AnsiString sMainWfName1 ;

        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome = 0 ;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        AnsiString   GetCycleName   (){return m_sCycleName[Step.iSeq];}
        SStat        GetStat        (){return Stat          ;}


        void SetStat(SStat _sStat) { Stat = _sStat ; }


        double GetTimePitch (EN_SEQ_CYCLE _iSeqCycle){return Time[_iSeqCycle].dPitch ; }
        double GetTimeWork  (EN_SEQ_CYCLE _iSeqCycle){return Time[_iSeqCycle].dWork  ; }

        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlowSpeed = false); // ���͸� �����϶� ���� �Լ�.
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
        bool BarcodeLoadingTest();
        bool CycleSupply  (void); //������ ����.
        bool CycleExpend  (void); //�ͽ����.
        bool CyclePlace   (void); //������ ����.

        //�޴��� ����Ŭ.
        bool CycleUnExpend(void); //������ Ǯ��.

        

        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool IsLoad);

        bool LotInfoAdd();
        static void OnReadBarcode(DWORD _cbInQue) ;

        TRS232C * Barcode ;
        void SendBarcodeMsg(AnsiString _sMsg);
        AnsiString GetWaferId (AnsiString _sBarcodeData); //������ ���̵�.
        AnsiString GetWaferIdx(AnsiString _sBarcodeData); //������ �ε��� (���Գѹ�)

        double GetStageAbsMmFromAbsAngle(double _dAbsAngle); //���ϴ� ���� �ְ� mm����
        double GetStageAbsAngleFromAbsMm(double _dAbsMm   ); //���ϴ� mm �ְ� ���� ����.

        String m_sWorkingWfrId ; //���� �۾� �ϰ� �ִ� �������� ���ڵ� �ѹ� ..���ڵ� ��� ����.


};

//---------------------------------------------------------------------------
extern CStage STG;
//---------------------------------------------------------------------------
#endif
