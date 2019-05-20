//---------------------------------------------------------------------------

#ifndef MarkVisnH
#define MarkVisnH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"

//---------------------------------------------------------------------------
class CMarkVisn
{
    public:
        //Constructor
        CMarkVisn (void);
        ~CMarkVisn (void);

        void Init();
        void Close();
    
	void Reset     (); //리셑 버튼 눌렀을때 타는 함수.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd     ;
            bool bReqStop     ;
        };

        enum EN_SEQ_CYCLE {
            scIdle  =  0 , //놀기.
            scVisn  =  1 , //찝기전에 확인 비젼.
            scTrain =  2 , //랏마다 첫자제 트레인.

            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        void InitCycleName(){
            m_sCycleName[scIdle ]="Idle " ;
            m_sCycleName[scVisn ]="Visn " ;
            m_sCycleName[scTrain]="Train" ;
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

        bool   GetLtToRt  ();
        //bool   FindChip   (EN_ARAY_ID _iAray , int &r , int &c   );
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

        int    m_iLastInspRow ;
        String m_sTrainedLot ;

    public:    /* Direct Accessable Var.  */
        bool   FindChip   (EN_ARAY_ID _iAray , int &r , int &c   );
        bool CheckSafe (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool CheckSafe (EN_ACTR_ID _iActr , bool       _bFwd    );

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

        //마지막 검사열
        __property int    _iLastInspRow  = { read = m_iLastInspRow   };
        __property String _sTrainedLot   = { read = m_sTrainedLot    };

        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow = false); // 모터를 움직일때 쓰는 함수.
        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //실린더를 움직일때 쓰는 함수.

        //HomeCycle
        bool CycleHome(); //파트 홈을 잡을때 쓰는 함수.

        //Running Functions.
        bool ToStopCon (void); //스탑을 하기 위한 조건을 보는 함수.
        bool ToStartCon(void); //스타트를 하기 위한 조건을 보는 함수.
        bool ToStart   (void); //스타트를 하기 위한 함수.
        bool ToStop    (void); //스탑을 하기 위한 함수.
        bool Autorun   (void); //오토런닝시에 계속 타는 함수.

        //Ajin Trg 관련.
        void SetTrgPos();
        void ResetTrgPos();

        //Visn Rslt Data
        bool GetVisnRslt(EN_ARAY_ID _iAray , int _iRow);

        //Vision Cycle .
        bool CycleVisn ();
        bool CycleTrain();

        //For Start Stop Checking.
        void SetLastCmd (void); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (void); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (void); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool _bLoad );

        bool VisnJobChange();
        bool SaveVisnDevice(AnsiString _sDevice);


        //int m_mmm;
};

extern bool g_bMarkChangeFinish ;

//---------------------------------------------------------------------------
extern CMarkVisn VSN;
//---------------------------------------------------------------------------
#endif
