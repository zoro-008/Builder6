//---------------------------------------------------------------------------
#ifndef RailDieH
#define RailDieH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
//---------------------------------------------------------------------------
#include "VisionMan.h"
//---------------------------------------------------------------------------
class CRailDie
{
    public:
        //Constructor
         CRailDie ();
        ~CRailDie ();

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
            scWorkStt  = 10 ,
            scReGrip   = 20 ,
            scWork     = 30 ,
            scAtVisn   = 40 ,
            scBfVisn   = 50 ,
            scWorkEnd  = 60 ,

            MAX_SEQ_CYCLE
        };

        struct SStep {
            int          iHome    ;
            int          iToStart ;
            EN_SEQ_CYCLE iSeq     ;
            int          iCycle   ;
            int          iToStop  ;
        };
        
        CVisionMan::TRetResult RsltAt ;
        CVisionMan::TRetResult RsltBf ;

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

        //CVisionMan::TRetResult RsltAt ;
        //CVisionMan::TRetResult RsltBf ;

        double m_dLastIdxPos ;
        bool   m_bInsYReady  ;

        int m_iHomeTO  ;
        int m_iStartTO ;
        int m_iStopTO  ;
        int m_iCycleTO ;


//        bool   FindChipY  (EN_ARAY_ID _riWhre , int &r , int &c     );
        double GetMotrPos (EN_MOTR_ID _iMotr  , EN_PSTN_ID _iPstnId );


    public:    /* Direct Accessable Var.  */
        bool   m_bEpxFailIgnr ;

        bool   FindChip   (EN_ARAY_ID _riWhre , int &r , int &c     );
        double GetWorkPos (EN_MOTR_ID _iMotr); //Work Position WRE_XIdx

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
        __property bool   _bInsYReady      = { read = m_bInsYReady    , write= m_bInsYReady };
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
        bool CycleWorkStt(); //�۾�������ġ��
        bool CycleReGrip (); //���׸�. ���⼱ �Ⱦ��� ���� �ؼ� �Ⱦ�.
        bool CycleWork   (); //�۾� ��ġ�� �̵�.
        bool CycleAtVisn (); //������ �˻�
        bool CycleBfVisn (); //���� �˻�
        bool CycleWorkEnd(); //���� ����.

        //For Start Stop Checking.
//        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        double GetLastCmd (EN_MOTR_ID _iMotr);
        bool   CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool   CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool IsLoad);

        double GetXOffset  (){return RsltBf.fRsltX ;}
        double GetYOffset  (){return RsltBf.fRsltY ;}
        double GetTOffset  (){return RsltBf.fRsltT ;}

};

//---------------------------------------------------------------------------
extern CRailDie RAD;
//---------------------------------------------------------------------------
#endif
