//---------------------------------------------------------------------------
#ifndef SortH
#define SortH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
//---------------------------------------------------------------------------
class CSort
{
    public:
        //Constructor
         CSort ();
        ~CSort ();

        void Init      ();
        void Close     ();
        void Reset     (); //���V ��ư �������� Ÿ�� �Լ�.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bSortMove   ;//Sort���� �����̱� ����...JS
            bool bSortDetect ;
            bool bWorkEnd    ;
            bool bReqStop    ;
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle       =  0 ,
            scSort       = 10 ,
            scDetect     = 20 ,
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
        AnsiString  m_sPartName ;
        //Timer.
        CDelayTimer m_tmMain    ;
        CDelayTimer m_tmCycle   ;
        CDelayTimer m_tmHome    ;
        CDelayTimer m_tmToStop  ;
        CDelayTimer m_tmToStart ;
        CDelayTimer m_tmTemp    ;

        SStat Stat ;
        SStep Step , PreStep ;

        int m_iHomeTO  ;
        int m_iStartTO ;
        int m_iStopTO  ;
        int m_iCycleTO ;

        bool   FindChip   (EN_ARAY_ID _riWhre , int &r , int &c     );
//        double GetMotrPos (EN_MOTR_ID _iMotr  , EN_PSTN_ID _iPstnId );

        int  m_iCrntBin    ; //���� ��        0 = WaitPos 1 = 1����.....
        int  m_iSetBin     ; //������ �� �ѹ�. 1,2,3,4,5    Work���� ������.
        bool m_bSetSort    ; //������ �� �ѹ�. 1,2,3,4,5    Work���� ������.
        bool m_bSetSortSub ; //������ �� �ѹ�. 1,2,3,4,5    Work���� ������.
        bool m_bUseDetect  ;

        int m_iBin1Cnt ;
        int m_iBin2Cnt ;
        int m_iBin3Cnt ;
        int m_iBin4Cnt ;
        int m_iBin5Cnt ;


    public:    /* Direct Accessable Var.  */
        double GetMotrPos (EN_MOTR_ID _iMotr  , EN_PSTN_ID _iPstnId );    
        bool   bFirstPkg ;
        bool   bSucDetect;
        bool   bNeedDetect;
        bool   bSortEnd   ;

        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );


        bool GetSortEnd () {return bSortEnd ;}



        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome = 0 ;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        SStat        GetStat        (){return Stat          ;}

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
        bool Autorun   (void); //

        //One Cycle.
        bool CycleSort  ();
        bool CycleDetect();

        //For Start Stop Checking.
//        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        double GetLastCmd(EN_MOTR_ID _iMotr);
        bool   CheckMoved(void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool   CheckStop (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool IsLoad);

        void SetBin     (int _iBinNo , bool _bWriteData = false);// , bool _bUseDetect = false );
        void SetSort    (bool _bRet );
        int  GetBin     (           );
        int  GetCntBin  (           );
        int  GetBinCnt  (int _iBinNo);
        void AddBinCnt  (int _iBinNo , int _iCnt);
        void ClearBinCnt(int _iBinNo);

};

//---------------------------------------------------------------------------
extern CSort SRT;
//---------------------------------------------------------------------------
#endif
