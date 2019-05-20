//---------------------------------------------------------------------------

#ifndef PstBuffH
#define PstBuffH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
#include "LotUnit.h"
//---------------------------------------------------------------------------
class CPstBuff
{
    public:
        //Constructor
        CPstBuff (void);
        ~CPstBuff (void);
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
            scIdle   =  0 ,
            scIn     =  1,
            scFlip   =  2,
            scWork   =  3,
            scOut    =  4,
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

        SStat Stat ;              //
        SStep Step , PreStep ;

        AnsiString m_sWorkedLotNo ;
        AnsiString m_sWorkedId    ;

        double GetMotrPos   (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //모터가 가야할 포지션을

        bool   FindChip     (EN_ARAY_ID _iAray , EN_CHIP_STAT _iStat ,  int &r , int &c );
        bool   GetChipFlipY (int r, int c , int &rr , int &rc);
        bool   FindChipMrk  ( int &r , int &c ); //

        int    m_iCrntBin   ; //1,2,3

        bool   FindReject(EN_PSTN_ID &piFindPosX , EN_PSTN_ID &piFindPosY , int &iRjctSrt , EN_CHIP_STAT &csFindChip );

    public:    /* Direct Accessable Var.  */
        int    m_iReviewR   ;
        int    m_iReviewC   ;



        bool   m_bLotEnded    ;
        bool   GetLastStrip() ;
        bool   LotInfoAdd  (CArray * Array);
        bool   bTrim ;

        bool CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //모터가 움직일때 충돌조건 감지.
        bool CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    ); //실린더가 움직일때 충돌조건 감지.

        bool GetWorkEnd (){return Stat.bWorkEnd ;}
        bool GetReqStop (){return Stat.bReqStop ;}

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome = 0 ;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        SStat        GetStat        (){return Stat          ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

	//Motor.
	bool MoveMotr (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // 모터를 움직일때 쓰는 함수.

	//Actuator.
	bool MoveActr (EN_ACTR_ID _iActr , bool _bFwd); //실린더를 움직일때 쓰는 함수.

        //HomeCycle
        bool CycleHome(); //파트 홈을 잡을때 쓰는 함수.

        //Running Functions.
	bool ToStopCon  (void); //스탑을 하기 위한 조건을 보는 함수.
	bool ToStartCon (void); //스타트를 하기 위한 조건을 보는 함수.
	bool ToStart    (void); //스타트를 하기 위한 함수.
	bool ToStop     (void); //스탑을 하기 위한 함수.
        bool Autorun    (void); //오토런닝시에 계속 타는 함수.

        //One Cycle.
        bool CycleIn           (void); //In    cycle DataMasking.
        bool CycleFlip         (void); //플립 싸이클.
        bool CycleWork         (void); //트림 싸이클.
        bool CycleOut          (void); //자제 아웃.

        bool MoveSort(int _iBinNo , bool _bInit); //Work Cycle에서 빈 움직여 놓는것.
        bool CheckSort(int _iBinNo);

        //For Start Stop Checking.
        void SetLastCmd (void); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (void); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (void); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool IsLoad );

        //Display용 Lot정보.
        CLot::SLotInfo EndedLot ;

};

//---------------------------------------------------------------------------
extern CPstBuff PSB;

//---------------------------------------------------------------------------
#endif
