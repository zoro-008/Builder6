//---------------------------------------------------------------------------
#ifndef EpoxyH
#define EpoxyH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
//---------------------------------------------------------------------------
//enum EN_ARAY_STEP {
//    asNotWorkStt  = 0 ,
//    asComWorkStt  = 1
//};
class CEpoxy
{
    public:
        //Constructor
         CEpoxy ();
        ~CEpoxy ();

        void Init      ();
        void Close     ();
        void Reset     (); //���V ��ư �������� Ÿ�� �Լ�.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle     =  0 ,
            scClean    = 10 ,
            scOri      = 20 ,
            scWork     = 30 ,
            scWorkOut  = 40 ,
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

        double m_dLastIdxPos ;

        bool   m_bToStarted  ; //����ŸƮ Ÿ�� ������ ¥����.

        int m_iHomeTO  ;
        int m_iStartTO ;
        int m_iStopTO  ;
        int m_iCycleTO ;

        int m_iEpxCnt ;
        int m_iEpxLotCnt ;
        int m_iSlotCnt   ;
        int m_iMgzCnt    ;

        bool   FindChip   (EN_ARAY_ID _riWhre , int &r , int &c   );
        double GetWorkPos (EN_MOTR_ID _iMotr);
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

    public:    /* Direct Accessable Var.  */

        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );


        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome = 0 ;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        SStat        GetStat        (){return Stat          ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

        //Property.
        __property int   _iEpxCnt      = { read = m_iEpxCnt    , write= m_iEpxCnt    };
//        __property int   _iEpxLotCnt   = { read = m_iEpxLotCnt , write= m_iEpxLotCnt };

        int  GetEpxLotCnt(         ) { return m_iEpxLotCnt;  }
        bool SetEpxLotCnt(int _iCnt) { m_iEpxLotCnt = _iCnt; }
        bool AddEpxLotCnt(         ) { m_iEpxLotCnt ++;      }
        bool DelEpxLotCnt(         ) { if(m_iEpxLotCnt>0) m_iEpxLotCnt --; }

        bool ClearSlotCnt(         ) { m_iSlotCnt = 0 ;      }
        bool ClearMgzCnt (         ) { m_iMgzCnt  = 0 ;      }

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
        bool CycleClean   (void);
        bool CycleOri     (void);
        bool CycleWork    (void);
        bool CycleWorkOut (void);

        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool IsLoad);

};

//---------------------------------------------------------------------------
extern CEpoxy EPX;
//---------------------------------------------------------------------------
#endif
