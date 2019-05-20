//---------------------------------------------------------------------------

#ifndef PartBaseH
#define PartBaseH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"

//---------------------------------------------------------------------------
class CPartInterface
{
    public:
        //Constructor
        CPartInterface (void); //많이 생성이 되지 않기 때문에 초기화리스트 사용 안함.
        virtual ~CPartInterface (void);

        //버추얼은 그냥써도되고 추가 기능 있을시엔 재정의 해서 먼저 CPartBase의 함수 수행하고 자신이 추가 할것 입력.
        virtual void Init();
        virtual void Close();

	virtual void Reset     (); //리셑 버튼 눌렀을때 타는 함수.
        virtual void ResetTimer();

        virtual bool   FindChip  (EN_ARAY_ID _riAray, EN_CHIP_STAT _csStat , int &r , int &c )const=0;
        virtual double GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )const;
        virtual bool   CheckSafe (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )const;
        virtual bool   CheckSafe (EN_ACTR_ID _iActr , bool       _bFwd    )const;

        //Motor.
        virtual bool MoveMotr (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)const=0; // 모터를 움직일때 쓰는 함수.
        //Actuator.
        virtual bool MoveActr (EN_ACTR_ID _iActr , bool _bFwd)const=0; //실린더를 움직일때 쓰는 함수.

        //HomeCycle
        virtual bool CycleHome()=0; //파트 홈을 잡을때 쓰는 함수.

        //Running Functions.
        bool ToStopCon (void); //스탑을 하기 위한 조건을 보는 함수.
        bool ToStartCon(void); //스타트를 하기 위한 조건을 보는 함수.
        bool ToStart   (void); //스타트를 하기 위한 함수.
        bool ToStop    (void); //스탑을 하기 위한 함수.
        bool Autorun   (void); //오토런닝시에 계속 타는 함수.

        //For Start Stop Checking.
        void SetLastCmd (void); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (void); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (void); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool _bLoad);












        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
        };

        struct SStep {
            int          iHome    ;
            int          iToStart ;
            int          iSeq     ;
            String       sSeqName ;
            int          iCycle   ;
            int          iToStop  ;
        };

    protected:
        String      m_sPartName ;
        //Timer.
        CDelayTimer m_tmMain    ;
        CDelayTimer m_tmCycle   ;
        CDelayTimer m_tmHome    ;
        CDelayTimer m_tmToStop  ;
        CDelayTimer m_tmToStart ;
        CDelayTimer m_tmDelay   ;

        SStat Stat ;
        SStep Step , PreStep ;




    public:    /* Direct Accessable Var.  */



        bool GetWorkEnd()const{return Stat.bWorkEnd ;}
        bool GetReqStop()const{return Stat.bReqStop ;}

        int    GetHomeStep    ()const{return Step.iHome    ;} int GetPreHomeStep   ()const{return PreStep.iHome    ;} void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome = 0 ;}
        int    GetToStartStep ()const{return Step.iToStart ;} int GetPreToStartStep()const{return PreStep.iToStart ;}
        int    GetSeqStep     ()const{return Step.iSeq     ;} int GetPreSeqStep    ()const{return PreStep.iSeq     ;}
        int    GetSeqName     ()const{return Step.sSeqName ;} int GetPreSeqName    ()const{return PreStep.sSeqName ;}
        int    GetCycleStep   ()const{return Step.iCycle   ;} int GetPreCycleStep  ()const{return PreStep.iCycle   ;} void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int    GetToStopStep  ()const{return Step.iToStop  ;} int GetPreToStopStep ()const{return PreStep.iToStop  ;}
        SStat  GetStat        ()const{return Stat          ;}
        String GetPartName    ()const{return m_sPartName   ;}

};

//---------------------------------------------------------------------------
#endif
