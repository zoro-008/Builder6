//---------------------------------------------------------------------------

#ifndef DispensorH
#define DispensorH
//---------------------------------------------------------------------------
#include "PartInterface.h"

#include "Timer.h"
#include "DataMan.h"
#include "Rs232Dispr.h"
//#include "Rs232Keyence.h"


//---------------------------------------------------------------------------
class CDispensor : public CPartInterface
{
    public: //초기화 등등.
        //Constructor
        CDispensor ();
        ~CDispensor ();

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
            scVisnBf       ,
            scDisp         ,
            scVisnAt       ,
            scClean        ,

            scManStdCal    ,
            scManSlnChange ,
            MAX_SEQ_CYCLE
        };

        int        m_iCycleSttTime             ;
        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //마지막 사이클 시간. ms
        void InitCycleName(){
            m_sCycleName[scIdle        ]="Idle"         ;
            m_sCycleName[scVisnBf      ]="VisnBf"       ;
            m_sCycleName[scDisp        ]="Disp"         ;
            m_sCycleName[scVisnAt      ]="VisnAt"       ;
            m_sCycleName[scClean       ]="Clean"        ;

            m_sCycleName[scManStdCal   ]="ManStdCal"    ;
            m_sCycleName[scManSlnChange]="ManSlnChange" ;
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
            EN_SEQ_CYCLE iLastSeq ;
        };

    protected:
        AnsiString m_sPartName  ;

        CCycleTimer m_tmDispr   ;
        //Timer.
        CDelayTimer m_tmMain    ;
        CDelayTimer m_tmCycle   ;
        CDelayTimer m_tmHome    ;
        CDelayTimer m_tmToStop  ;
        CDelayTimer m_tmToStart ;
        CDelayTimer m_tmDelay   ;

        SStat  Stat ;
        SStep  Step , PreStep ;

        double m_dLastIdxPos ;

        String m_sCheckSafeMsg ;

        int    m_iReptCnt    ;
        double m_dDisprTime  ; 

        bool   m_bBfVisn  ;
        double m_dBfVisnX , m_dBfVisnY , m_dBfVisnT ;
        double m_dPkgCntX , m_dPkgCntY , m_dPkgCntT ;


        bool   m_bAtVisn  ;
        double m_dAtVisnX , m_dAtVisnY , m_dAtVisnT ;

        double GetLastCmd(EN_MOTR_ID _iMotr);

        void   ResetTimer();

        bool   FindChip   (int &c , EN_ARAY_ID &id);
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

        int    m_iDisprBoatCnt;

        bool   m_bOneSkipVisnAt;



    public: //인터페이스 상속.==================================================
	void Reset (); //리셑 버튼 눌렀을때 타는 함수.

        int  GetReptCnt();
        //Running Functions.
        void Update    ();
        bool ToStopCon (); //스탑을 하기 위한 조건을 보는 함수.
        bool ToStartCon(); //스타트를 하기 위한 조건을 보는 함수.
        bool ToStart   (); //스타트를 하기 위한 함수.
        bool ToStop    (); //스탑을 하기 위한 함수.

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;}void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome  = 0 ;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        int          GetSeqStep     (){return (int)Step.iSeq;} int GetPreSeqStep    (){return (int)PreStep.iSeq;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;}void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        SStep        GetStep        (){return Step          ;}

        AnsiString   GetCycleName   (int _iSeq){return m_sCycleName[_iSeq];}
        int          GetCycleTime   (int _iSeq){return m_iCycleTime[_iSeq];}
        AnsiString   GetPartName    (){return m_sPartName   ;}

        int          GetCycleMaxCnt (){return MAX_SEQ_CYCLE ;}

        bool         Autorun        (); //오토런닝시에 계속 타는 함수.
        //======================================================================
    public:
        double GetDisprTime();
        void   SetDisprTime(double _dDisprTime);

        void   AddDisprBoatCnt();

        //HomeCycle
        bool CycleHome(); //파트 홈을 잡을때 쓰는 함수.

        //One Cycle.
        bool CycleVisnBf();
        bool CycleDisp  ();
        bool CycleVisnAt();
        bool CycleClean ();

        //Manual Cycle.
        bool CycleManStdCal();
        bool CycleManNdlCheck();
        bool CycleManVisnPos ();
        bool CycleManSlnChange();

        bool ToolVoid(); //디스펜서 일할때 피해주기.

        //bool CycleManClean ();
        
        CRs232Dispr   Rs232_Dispr;
        //CRs232Keyence Rs232_Hght ;


        bool GetVisnRslt(double * _dVisnX , double * _dVisnY , double * _dVisnT , bool _bAt = false);

        //메뉴얼. 나중에 만들자.
        //bool CycleManAlignPlace(); //픽커에 있는것을 얼라이너에 내려 놓는다.
        //bool CycleManPickVisnPlace(); //얼라인에서 찝어서 비젼검사하고 다시 내려 놓는다.
        //bool CycleManPickerCheck(); //아무것도 들고 있지 않은 툴의 터치센서 높이 측청.


        bool CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        void  SetStat(SStat _sStat) { Stat = _sStat ; }
        SStat GetStat(            ) { return Stat;    }

        //Motor.
        //bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // 모터를 움직일때 쓰는 함수.
        void MoveIncMotr (EN_MOTR_ID _iMotr , double _iPstn); // 모터를 움직일때 쓰는 함수.

        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow = false);

        //시바 동기구동 하려면 볼스크류 피치 같아야 쓸듯 ;;
        bool MoveMotrSync(EN_MOTR_ID _iMotr1 , EN_MOTR_ID _iMotr2 , double dMotr1Pos , double dMotr2Pos , double _dSpeed , double _dAcc , double _dDcc);

        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //실린더를 움직일때 쓰는 함수.

        //For Start Stop Checking.
        void SetLastCmd (); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool _bLoad);

        //그냥 DM.보고 판단 하면 될듯.
        //bool   m_bNeedWorkAfterDisp ; //디스펜싱 끝나고 에러나 스탑 버튼 누른 상태에도 작업을 진행 해야 하는경우.
                                      //픽커에서 이걸 보고 오토런에서 RqstStop인상황에도 오토런을 돌려줘야함.
                                      //별도로 UpdateButton에서 투스탑 타임아웃 늘려놨음.

        bool   m_bRqstPMSave ; //스텐다드 캘등... 캘작업후 포지션 저장이 필요한경우. true시키고
                               //폼 타이머에서 확인 하고 UI에서 저장 한다.
        bool   m_bVsnBfErrDisp; //에러 폼 띄우기
        bool   m_bVsnAtErrDisp; //에러 폼 띄우기

        int    m_iCrntDispNode;

        bool   m_bNiddleChecked;//랏오픈하면 한번씩 니들 체크를 수행해야 한다.

};

class CSetInterpolation {
    public :
        int iMotorAixs[2] ; // 보간 운동 모터 축 설정
        double dCenPos[2] ; // 원호 보간 운동 하는 CircleCenterMove 함수를 호출할 때 사용
        double dEndPos[2] ; // 직선 및 원호 보간 운동 하는 함수들을 호출할 때 사용

        int iCoord ; // 좌표계 설정
        int iAngle ; // 자동 가감속 모드를 설정시 사용, 0 ~ 360도
        int iAxisCnt ; // 보간 운동할 모터 개수 설정

        DWORD dwAbsRelMode ; // 보간 운동할 때, 절대 = 0, 상대 모드 = 1 을 설정
        DWORD dwProfileset ; // 가감속 모드를 설정, 일반 = 1, 노드 가감속 모드 = 2, 자동 가감속 모드 = 3
        DWORD dwCWDir ; // 원호 보간 운동할 때, 원호를 그리는 방향 설정, 반시계 방향 = 0, 시계 방향 = 1

        double dVel ; // 보간 운동할 때, 기본 모터 속도를 설정
        double dAcc ; // 보간 운동할 때, 모터 가속도를 설정
        double dDec ; // 보간 운동할 때, 모터 감속도를 설정
};


//---------------------------------------------------------------------------
extern CDispensor DSP;
//---------------------------------------------------------------------------
#endif
