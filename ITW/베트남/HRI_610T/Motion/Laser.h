//---------------------------------------------------------------------------

#ifndef LaserH
#define LaserH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
//---------------------------------------------------------------------------

struct TAlignResult {
    double dMainX ;
    double dMainY ;
    double dTheta ;
    double dSubX  ;
    double dSubY  ;
};

class CLaser
{
    public:
        //Constructor
        CLaser (void);
        ~CLaser (void);

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
            scIdle       =  0 , //놀기.
            scVisn       =  1 , //메뉴얼 비젼만.
            scLaser      =  2 , //메뉴얼 레이져 조사만.
            scWork       =  3 , //비젼& 레이져.
            scDataChange =  4 , //잡파일 혹은
            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        void InitCycleName(){
            m_sCycleName[scIdle ]="Idle "  ;
            m_sCycleName[scVisn ]="Visn "  ; //얼라인 비젼.
            m_sCycleName[scLaser]="Laser"  ;
            m_sCycleName[scWork ]="Work "  ;
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
        CDelayTimer m_tmTemp    ;

        SStat Stat ;
        SStep Step , PreStep ;

        bool   FindChip   (int &r , int &c   );
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

        String m_sLaserWorkLotNo ;


    public:    /* Direct Accessable Var.  */
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

        //레이저에 보내놓은 LotNo ;
        __property String _sLaserWorkLotNo  = { read = m_sLaserWorkLotNo   };

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
        bool CycleVisn (); //메뉴얼만
        bool CycleLaser(); //메뉴얼만

        bool CycleWork (); //오토런에서는 이것만 쓴다.


        //For Start Stop Checking.
        void SetLastCmd (void); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (void); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (void); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool _bLoad );

        //Laser 관련 Data
        bool       CheckFormat(AnsiString _sFormat);
        AnsiString GetLaserDataFromLotNot(AnsiString _sFormat , AnsiString _sLotNo);

        bool       LaserJobChange(AnsiString _sDevName);
        //void       SaveLaserLotNo(AnsiString _sLotNo, bool _bDeviceChage = false);
        bool       SaveLaserLotNo(AnsiString _sLotNo);

        bool       SaveLaserDevice(AnsiString _sDevice);

        //비전 찍고 데이터 가져오자.
        bool       LoadAlignRslt(TAlignResult *_tAlign);
        
        bool       m_bChangeLotNo ;


        //메뉴얼 마킹 관련.
        bool       m_bManMark ;
        String     m_sManMark ;

        bool       m_bAutoRun  ;

        TAlignResult m_tAlign ;
        void       GetRotPntFromCntrPnt(double _dX     , double _dY     , double _dAng , //입력 x , 입력 y , 돌릴각도. , +가 반시계방향.
                                        double _dCntrX , double _dCntrY ,                //돌리는 기준점 x , y
                                        double *_pRetX , double *_pRetY );                //결과값 x,y
};

//---------------------------------------------------------------------------
extern CLaser LSR;
//---------------------------------------------------------------------------
#endif
