//---------------------------------------------------------------------------

#ifndef SortH
#define SortH
//---------------------------------------------------------------------------
#include "PartInterface.h"

#include "Timer.h"
#include "DataMan.h"

enum EN_VAC_ERR_STAT { //���� �� �� �ϰ� �ִ� ����.
    vsNomal            = 0 , //�������� ���� �ٽ� ���ϰ� �������� �ٽ� ����.
    vsErrSkipPick      = 1 , //���ߴ��� �ٽ� ���ϰ� ������ �׳� ���� ������ �Ѿ -> vsNomal.
    vsErrSkipRow       = 2 , //���ߴ��� �ٽ� ���ϰ� �ش� ������������ ���� ��ŵ -> vsNomal.
    vsErrSkipTray      = 3 , //���ߴ��� �ٽ� ���ϰ� �ش� Ʈ���̿����� ���� ��ŵ -> vsNomal.
    vsErrSkipRowOut    = 4 , //���ߴ��� ��ŵ���� Ȯ�� �ϰ� Ʈ���� �ƿ�.
    vsErrSkipRowOutLE  = 5 , //���ߴ��� ��ŵ���� Ȯ���ϰ� Ʈ���� �ƿ��� �������û.

    //�̰��� Ŭ���ÿ� �ٷ� ó�� �Ǽ� �ʿ� ����.  vsTrayOut          = 6 , //���� Ʈ���� ��� Empty ��Ű��.
    //�̰��� Ŭ���ÿ� �ٷ� ó�� �Ǽ� �ʿ� ����.  vsTrayOutLE        = 7 , //Ʈ���� �ƿ� ��Ű�� ���ص� ��û.

};


//---------------------------------------------------------------------------
class CSort : public CPartInterface
{
    public: //�ʱ�ȭ ���.
        //Constructor
        CSort ();
        ~CSort ();

        void Init();
        void Close();


    public:

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle    =  0 ,
            scPickSTG =  1 ,
            scPlceRJT =  2 ,
            scPlceULD =  3 ,
            scPickZIG =  4 ,
            scPlceSTG =  5 ,
            scPickLDR =  6 ,
            scPlceZIG =  7 ,
            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //������ ����Ŭ �ð�. ms
        void   InitCycleName(){
            m_sCycleName[scIdle   ]="Idle   ";
            m_sCycleName[scPickSTG]="PickSTG";
            m_sCycleName[scPlceRJT]="PlceRJT";
            m_sCycleName[scPlceULD]="PlceULD";
            m_sCycleName[scPickZIG]="PickZIG";
            m_sCycleName[scPlceSTG]="PlceSTG";
            m_sCycleName[scPickLDR]="PickLDR";
            m_sCycleName[scPlceZIG]="PlceZIG";
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

        SStat Stat ;
        SStep Step , PreStep ;

        SStat GetStat(){return Stat;}

        void ResetTimer();

        bool   FindChip   (EN_ARAY_ID _iArray , EN_CHIP_STAT _iStat , int &r , int &c   );
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );



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

        bool Autorun   (); //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
        //======================================================================
    public:

        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

        //One Cycle.
        bool CyclePickSTG();
        bool CyclePlceRJT();
        bool CyclePlceULD();
        bool CyclePickZIG();
        bool CyclePlceSTG();
        bool CyclePickLDR();
        bool CyclePlceZIG();


        bool CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );


        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // ���͸� �����϶� ���� �Լ�.
        void MoveIncMotr (EN_MOTR_ID _iMotr , double _iPstn); // ���͸� �����϶� ���� �Լ�.
        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.



        //For Start Stop Checking.
        void SetLastCmd (); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool _bLoad);










        //���� ����=============================================================
        struct TSortInfo{
            bool         bFindOk     ; //�۾����ɿ���.
            bool         isPick      ; //�ȿ����� �÷��̽� ������.
            int          iCol        ; //iARAY�� �۾��� ������ ����
            int          iRow        ; //iARAY�� �۾��� ������ ��������.
            EN_CHIP_STAT iTargetStat ; //���� Ĩ ����.
            EN_ARAY_ID   iAray       ; //�۾��� ���
            EN_ARAY_ID   iTool       ; //��Ŀ�� ���.
            int          iToolShft   ; //���� ù��° �÷� ���� �۾��� �ȵɰ�� ����Ʈ �ȴ�.
            int          iDnCnt      ; //��ų� �������� Ĩ ����.
            bool         bDn[MAX_TOOL_SOCK_COL] ;
        };
        TSortInfo SortInfo ;
        //Ʈ���̿��� ��ų� Ʈ���̿� ������ ��� �ϴ� �Լ�.
        bool GetTraySortInfo  (bool _bPick , EN_ARAY_ID _iTool , EN_ARAY_ID _iAray , TSortInfo & _tInfo);
        bool ShiftTraySortInfo(TSortInfo & _tInfo , bool _bVacErr1 = false , bool _bVacErr2 = false ,bool _bVacErr3 = false ,bool _bVacErr4 = false );

        //���׳� ���Ͽ��� ��ų� ������ ��� �ϴ� �Լ�.
        bool GetSockSortInfo  (bool _bPick , EN_ARAY_ID _iTool , EN_ARAY_ID _iAray , TSortInfo & _tInfo);
        bool ShiftSockSortInfo(TSortInfo & _tInfo , bool _bVacErr1 = false , bool _bVacErr2 = false ,bool _bVacErr3 = false ,bool _bVacErr4 = false );

        //========================================================================

        //��Ũ����
        EN_VAC_ERR_STAT m_iVacErrStat ;
        bool            m_bVacErr[MAX_TOOL_SOCK_COL] ;

        bool CheckRetryStgPick();
        bool CheckRetryZigPick();


        bool m_bRetryZigPick ; //�� �̽� ���� ������ ��Ʈ���� ���ý�..
        bool m_bRetryStgPick ;

        bool m_bLdrPickErrDisp ; //�δ����� �� �̽� ������ ����â ���÷���
        bool m_bZigPickErrDisp ; //���׿��� �� �̽� ������ ����â ���÷���
        bool m_bStgPickErrDisp ; //������������ �� �̽� ������ ����â ���÷���

        double m_dTickTime;


        void Test();
};

//---------------------------------------------------------------------------
extern CSort SRT;
//---------------------------------------------------------------------------
#endif

