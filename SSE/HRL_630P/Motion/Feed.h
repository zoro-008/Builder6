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

	void Reset     (); //리셑 버튼 눌렀을때 타는 함수.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd     ;
            bool bReqStop     ;
        };

        enum EN_SEQ_CYCLE {
            scIdle     =  0 , //놀기.
            scHome     =  1 , // 트랜스퍼 이동 사이클 중에 홈잡아주깅
            scWait     =  2 , //자제 안들어올때 혹은 자제들어와있는데도 아웃이 안나갔을때 쉐끼쉐끼.
            scWork     =  3 , //롤러로 스트립 밀어 넣는것.
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

            //모터 흔들기용.
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

        CDelayTimer m_tmFeedTimeOut      ; //작업 시작 하고 나서 중간에 Feeding in TimeOut
        CDelayTimer m_tmFeedStartTimeOut ; //작업 시작 할때 Feeding in Timeout
        CDelayTimer m_tmRqstLotEndTimer  ; //랏엔드 요청후 마지막 칩에서 부터 이시간 후에 WorkEnd됌

        bool        m_bFirstChipIn ; //랏오픈이 되고 첫칩이 들어오면 true , 아직 첫칩 안들어왔으면 false .

        CDelayTimer m_tmInSolLRFlickTimer  ; //sol 플릭 타이머....
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
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // 모터를 움직일때 쓰는 함수.
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

        //One Cycle.
        bool CycleWait   ();
        bool CycleWork   ();


        //For Start Stop Checking.
        void SetLastCmd (void); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (void); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (void); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        bool TwstMT     (bool _bVal);
        //Loading Para.
        void Load(bool _bLoad );

        //첫칩 수행 여부 함수
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

        //모터가 병신이라 1바퀴당 36의 배수 분해능 설정이 없다.
        //그래서 360도로 1바퀴 설정 하고 15도씩 계속 한방향으로 돌리면
        //누적 오차 발생되어서 않됌.
        //
        //해결책.
        //Dll에서 PulsePerRev = 100000
        //        UnitPerRev  = 100000 으로 놓고 즉 포지션 제어 단위를 펄스로 맞추고
        //        어플리케이션 단에서 환산해서 펄스를 뿌리고 1바퀴 단위로 클리어 시켜준다.

        //펄스분해능 0.0036도
        //이동횟수  포켓위치  누적각도  누적펄스                            버림오차
        //1         1          15       4166.6666666666666666666666666667   약0.0024도
        //2         2          30       8333.3333333333333333333333333333   약0.0012도
        //          .
        //          .
        //          .
        //24        24         360      100000                              0

        // miFED_TIdx 어플리케이션에서 모터 포지션 이동시에 무조건 밑에 함수를 태워서 환산시켜 MT_GoInc쓰지말고
        // 0 포지션에서 절대값으로 보내야 한다.
        // 그리고 24번
        double Get100000From360(double _dAbsAngle); //0.0기준으로 Abs포지션을 입력 해야함.
        //역으로 환산한 각도 리턴.
        double GetTCmdPos();
        DWORD m_dWorkTotalTm;
        DWORD m_dTempTm;

};

//---------------------------------------------------------------------------
extern CFeed FED;
//---------------------------------------------------------------------------
#endif
