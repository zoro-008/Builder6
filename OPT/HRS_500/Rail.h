//---------------------------------------------------------------------------

#ifndef RailH
#define RailH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "LotUnit.h"
//---------------------------------------------------------------------------
//Array Insp Use Aray Step.
enum EN_ARAY_STEP {
    asInspNeed = 0 ,
    asInspDone = 1
};

class CRail // 레일 시작 단서부터 워크 존까지만 워크존 이후는 언로더로 들어감.
{
    public:
        //Constructor
        CRail (void);
        ~CRail (void);

        void Init();
        void Close();

        void Reset     (); //리셑 버튼 눌렀을때 타는 함수.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd ; //파트가 할일이 없을경우 true ;
            bool bReqStop ; //에러가 나거나 혹은 스탑버튼을 눌렀을때 파트가 서기를 요청하는 프레그.
        };

        enum EN_SEQ_CYCLE {
            scIdle    =  0  ,
            scIn      = 10  , //
            scWorkStt = 20  , //
            scWork    = 30  , //
            scOut     = 40  , //
//            scIn      = 10  , //로더밑에서 끌어오는 동작
//            scWork    = 20  , //픽엔 플레이스 할때 밑에서 포지션 맞춰주기 및 작업완료 트레이 밀어주기.
//            scOut     = 30  , //맨마지막 트레이 밀어주기.
            MAX_SEQ_CYCLE
        };

        struct SStep {
            int          iHome    ; //홈싸이클에서 쓰는 스텝.
            int          iToStart ; //투스타트 싸이클에서 씀.
            EN_SEQ_CYCLE iSeq     ; //장비의 할일 결정.
            int          iCycle   ; //장비의 할일에서 스텝.
            int          iToStop  ; //투스탑 싸이클에서 씀.
        };

    protected:
        AnsiString  m_sPartName ;
        //Timer.
        CDelayTimer m_tmMain    ; //타임아웃용.
        CDelayTimer m_tmCycle   ; //타임아웃용.
        CDelayTimer m_tmHome    ; //타임아웃용.
        CDelayTimer m_tmToStop  ; //타임아웃용.
        CDelayTimer m_tmToStart ; //타임아웃용.
        CDelayTimer m_tmTemp    ; //싸이클 함수에서 막대려다 쓰는애.

        SStat Stat ;
        SStep Step , PreStep ;

        AnsiString m_sWorkedLotNo ;
        AnsiString m_sWorkedId    ;

        double m_dLastIndexPos ;

        bool   FindChip   (int &r , int &c );
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //모터가 가야할 포지션을 리턴함. Double형.

        bool   GetLastStrip();



    public:    /* Direct Accessable Var.  */

        unsigned long  m_lStrpCycleTime ; //한장 작업 시간.... 화면 디피용.
        unsigned long  m_lPreCycleTime  ;

        bool CheckIdxPos(); //인덱스 위치 작업해야하는 위치가 맞는지 비교.

        bool CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //모터가 움직일때 충돌조건 감지.
        bool CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    ); //실린더가 움직일때 충돌조건 감지.

        bool GetWorkEnd (){return Stat.bWorkEnd ;}
        bool GetReqStop (){return Stat.bReqStop ;}

        int           GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){Step.iHome  = 10;}
        int           GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int           GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){Step.iCycle = 10;}
        int           GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        SStat         GetStat        (){return Stat          ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

	    //Motor.
	    bool MoveMotr (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // 모터를 움직일때 쓰는 함수.

	    //Actuator.
	    bool MoveActr (EN_ACTR_ID _iActr , bool _bFwd); //실린더를 움직일때 쓰는 함수.

        //HomeCycle
        bool CycleHome(); //파트 홈을 잡을때 쓰는 함수.

        //Running Functions.
	bool ToStopCon   (void); //스탑을 하기 위한 조건을 보는 함수.
	bool ToStartCon  (void); //스타트를 하기 위한 조건을 보는 함수.
	bool ToStart     (void); //스타트를 하기 위한 함수.
	bool ToStop      (void); //스탑을 하기 위한 함수.
        bool Autorun     (void); //오토런닝시에 계속 타는 함수.

	//One Cycle.
        bool CycleIn     (void); //
        bool CycleWorkStt(void); //
        bool CycleWork   (void); //
        bool CycleOut    (void); //

        //For Start Stop Checking.
        void SetLastCmd (void); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (void); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (void); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool IsLoad);
};

//---------------------------------------------------------------------------
extern CRail RAL;

//---------------------------------------------------------------------------
#endif
