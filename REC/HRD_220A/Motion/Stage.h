//---------------------------------------------------------------------------

#ifndef StageH
#define StageH
//---------------------------------------------------------------------------
#include "PartInterface.h"

#include "Timer.h"
#include "DataMan.h"
#include "GridData.h"



//---------------------------------------------------------------------------
class CStage : public CPartInterface
{
    public: //�ʱ�ȭ ���.
        //Constructor
        CStage ();
        ~CStage ();

        void Init();
        void Close();


    public:

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle       =  0 ,
            scUp         =  1 ,
            scDown       =  2 ,
            MAX_SEQ_CYCLE
        };

        int        m_iCycleSttTime             ;
        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //������ ����Ŭ �ð�. ms
        void   InitCycleName(){
            m_sCycleName[scIdle]="Idle" ;
            m_sCycleName[scUp  ]="Up"   ;
            m_sCycleName[scDown]="Down" ;
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

        CDelayTimer m_tmFrstDelay;

        //TickTime.
        CCycleTimer m_tmTick    ;

        SStat  Stat ;
        SStep  Step , PreStep ;

        double m_dLastIdxPos ;

        double m_dTickTime   ;

        String m_sCheckSafeMsg ;

        double GetLastCmd(EN_MOTR_ID _iMotr);

        void   ResetTimer();

        bool   FindChip   (int &c , EN_ARAY_ID &id);


        double m_dPkPkgThickness ;
        double m_dStageHeight    ;
        double m_dFinishThickness;
        double m_dEpoxyThickness ;

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

        bool         Autorun        (); //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
        //======================================================================
    public:
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        
        double GetPkgThickness();  //Ű���� ������� ������ pkg �β�.
        void   SetPkgThickness(double _dThickness);
        double GetFinishThickness();
        void   SetFinishThickness(double _dThickness);
        double GetEpoxyThickness();
        void   SetEpoxyThickness(double _dThickness);


        double GetTickTime    () ; //Cycle Tick Time ��������
        void   SetTickTime    (double _dTickTime) ; //Cycle Tict Time ����..
        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

        //One Cycle.
        bool CycleUp      ();
        bool CycleDown    ();

        bool CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        void  SetStat(SStat _sStat) { Stat = _sStat ; }
        SStat GetStat(            ) { return Stat;    }

        //Motor.
        //bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // ���͸� �����϶� ���� �Լ�.
        void MoveIncMotr (EN_MOTR_ID _iMotr , double _iPstn); // ���͸� �����϶� ���� �Լ�.

        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow = false);

        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.

        //For Start Stop Checking.
        void SetLastCmd (); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool _bLoad);

        bool m_bVacErrDisp; //���� �� ����
        bool m_bNeedLotEnd; //���ص� ���� ����.


        //SPC Data��.
        CGridData WorkData            ;
        struct TStageSpc {
            double    dWorkSttTime      ;
            int       iWorkFrntBoatSlot ;
            String    sWorkFrntBoatId   ;
            String    sWorkFrntChipId   ;
            double    dPushWeight       ;
            double    dCrntStgTemp      ;
            int       iDefectCnt        ;
            TStageSpc(){
                Clear();
            }
            ~TStageSpc(){
            }
            void Clear(){
                dWorkSttTime      = 0 ;
                iWorkFrntBoatSlot = 0 ;
                sWorkFrntBoatId   = "";
                sWorkFrntChipId   = "";
                dPushWeight       = 0 ;
                dCrntStgTemp      = 0 ;
                iDefectCnt        = 0 ;
            }
        }Spc;

        void WriteSpc();



};

//---------------------------------------------------------------------------
extern CStage STG;
//---------------------------------------------------------------------------
#endif
