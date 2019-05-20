//---------------------------------------------------------------------------

#ifndef PickerH
#define PickerH
//---------------------------------------------------------------------------
#include "PartInterface.h"
#include "Rs232LoadCl.h"

#include "Timer.h"
#include "DataMan.h"
//---------------------------------------------------------------------------
class CPicker : public CPartInterface
{
    public: //초기화 등등.
        //Constructor
        CPicker ();
        ~CPicker ();

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
            scPick            ,
            scAlign           ,
            scAlignPick       ,
            scVisn            ,
            scAttach          ,
            scPush            ,
            scBoatPlace       ,
            scVoid            , //회피..

            scManStdCal       ,
            scManVsDefectPos  ,
            scManVsAlignPos   ,
            scManAlgnPlce     ,
            scManAlignerCal   ,

            MAX_SEQ_CYCLE
        };

        int        m_iCycleSttTime             ;
        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //마지막 사이클 시간. ms
        void   InitCycleName(){
            m_sCycleName[scIdle          ]="Idle"       ;
            m_sCycleName[scPick          ]="Pick"       ;
            m_sCycleName[scAlign         ]="Align"      ;
            m_sCycleName[scAlignPick     ]="AlignPick"  ;
            m_sCycleName[scVisn          ]="Visn"       ;
            m_sCycleName[scAttach        ]="Attach"     ;
            m_sCycleName[scPush          ]="Push"       ;
            m_sCycleName[scBoatPlace     ]="BoatPlace"  ;
            m_sCycleName[scVoid          ]="Void"       ;

            m_sCycleName[scManStdCal     ]="ManStdCal"  ;
            m_sCycleName[scManVsDefectPos]="ManVsDefectPos";
            m_sCycleName[scManVsAlignPos ]="ManVsAlignPos ";
            m_sCycleName[scManAlgnPlce   ]="ManAlgnPlce ";
            m_sCycleName[scManAlignerCal ]="ManAlignerCal";  

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

        bool   m_bVisn  ;
        double m_dVisnX , m_dVisnY , m_dVisnT ;
        int    m_iVisnDefCnt ;

        double GetLastCmd(EN_MOTR_ID _iMotr);

        void   ResetTimer();

        bool   FindChip   (int &c , EN_ARAY_ID &id);

        bool   m_bOneSkipDefect;

        
        double m_dEmptyToolHeight; //자제 없을때 측정.
        double m_dPkPkgThickness ; //키엔스 측정기로 측정한 pkg 두께.

    public: //인터페이스 상속.==================================================
	void   Reset (); //리셑 버튼 눌렀을때 타는 함수.

        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        
        double GetPkgThickness   () ; //키엔스 측정기로 측정한 pkg 두께.
        double GetEmptyToolHeight() ;
        double GetVisnDataX();
        double GetVisnDataY();
        int    GetVisnDefectCnt();

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
    
        //HomeCycle
        bool CycleHome(); //파트 홈을 잡을때 쓰는 함수.

        //One Cycle.
        bool CyclePick      ();
        bool CycleAlign     ();
        bool CycleAlignPick ();
        bool CycleVisn      ();
        bool CycleAttach    ();
        bool CyclePush      ();
        bool CycleBoatPlace ();
        bool CycleVoid      ();

        bool CycleManStdCal ();
        bool CycleManVsDefectPos();
        bool CycleManVsAlignPos ();
        bool CycleManAlgnPlce() ;
        bool CycleManAlignerCal() ;

        bool ToolVoid       ();


        //메뉴얼. 나중에 만들자.
        //bool CycleManAlignPlace(); //픽커에 있는것을 얼라이너에 내려 놓는다.
        //bool CycleManPickVisnPlace(); //얼라인에서 찝어서 비젼검사하고 다시 내려 놓는다.
        //bool CycleManPickerCheck  (); //아무것도 들고 있지 않은 툴의 터치센서 높이 측청.



        bool GetVisnRslt(double * _dVisnX , double * _dVisnY , double * _dVisnT);

        //Rs232Height Sensor.
        CRs232LoadCl  Rs232_LoadCl;



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

        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //실린더를 움직일때 쓰는 함수.

        //For Start Stop Checking.
        void SetLastCmd (); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool _bLoad);

        bool m_bRqstPMSave ;

        bool m_bVacErrDisp; //에러 폼 띄우기
        bool m_bVsnErrDisp; //에러 폼 띄우기

        bool m_bNeedLotEnd ;
};

//---------------------------------------------------------------------------
extern CPicker PCK;
//---------------------------------------------------------------------------
#endif
