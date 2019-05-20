//---------------------------------------------------------------------------
#ifndef HeadH
#define HeadH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
#include "QuedataUnit.h"
//---------------------------------------------------------------------------
//Vision
//---------------------------------------------------------------------------
#include "VisionMan.h"
//---------------------------------------------------------------------------

class CHead
{
    public:
        //Constructor
         CHead ();
        ~CHead ();

        void Init      ();
        void Close     ();
        void Reset     (); //리셑 버튼 눌렀을때 타는 함수.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle      = 0 ,
            scCal       = 1 , //비젼 켈리브레이션.
            scPenCheck  = 2 , //펜 나오는지 화이트 보드 가서 찍어보고 체크.
            scTAlign    = 3 , //비전 사각형으로 웨이퍼 세타 얼라인 잡는것.
            scOrigin    = 4 , //비젼 매치로 기준 칩 찾기.
            scChipAlign = 5 , //비전 사각형으로 칩 얼라인 위치 보정.
            scMark      = 6 , //펜마킹 & 경우에 따라 찍고 비젼 확인 까지.
            scWaitPos   = 7 , //웨이퍼 빼거나 넣을때 충돌 하지 않기 위해 툴이 대기 위치로 가있는다.
            scMarkTrace = 8 , //펜마크 검사 페일시에 역추적..
            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName [MAX_SEQ_CYCLE] ;
        void InitCycleName(){                                            
            m_sCycleName[scIdle     ]="Idle     ";                       
            m_sCycleName[scCal      ]="Cal      ";                       
            m_sCycleName[scPenCheck ]="PenCheck ";                       
            m_sCycleName[scTAlign   ]="TAlign   ";                       
            m_sCycleName[scOrigin   ]="Origin   ";                       
            m_sCycleName[scChipAlign]="ChipAlign";
            m_sCycleName[scMark     ]="Mark     ";
            m_sCycleName[scWaitPos  ]="WaitPos  ";
            m_sCycleName[scMarkTrace]="MarkTrace";
        }

        struct SCycleTime {
            double dStart ; //싸이클 스타트.
            double dPitch ; //싸이클 시작 간격.
            double dWork  ; //싸이클 소요시간.
        };



        struct SStep {
            int          iHome    ;
            int          iToStart ;
            EN_SEQ_CYCLE iSeq     ;
            int          iCycle   ;
            int          iToStop  ;

            //Vision.
            int          iVisn    ;
        };

        struct SRqst { //작업요청.
            bool bNeedTAlign    ; //얼라인 작업 필요.                          
            bool bNeedOrigin    ; //오리진 작업 필요.                          
            bool bNeedCal       ; //켈리브레이션 필요.                         
            bool bNeedPenCheck  ; //화이트 보드에서 펜찍히는지 체크 작업 필요.
            bool bNeedMarkTrace ; //펜 마킹 안되어 최종으로 잘 찍힌 곳 역추적 찾아감.
        }Rqst;
        int m_iMarkTraceCnt ; //마킹 자국 역추적 하는 횟수.

        int m_iPenDnCnt ; //펜의 작업 횟수. //펜체크 하면 클리어 된다.
        int m_iTotalPenDnCnt ; //현재 펜의 총작업수... 펜교체용 에러 띄운다.


    protected:
        AnsiString m_sPartName  ;
        //Timer.
        CDelayTimer m_tmMain    ;
        CDelayTimer m_tmCycle   ;
        CDelayTimer m_tmHome    ;
        CDelayTimer m_tmToStop  ;
        CDelayTimer m_tmToStart ;
        CDelayTimer m_tmTemp    ;

        //Vision.
        CDelayTimer m_tmVisn     ;
        CDelayTimer m_tmVisnTemp ;

        SStat Stat ;
        SStep Step , PreStep ;

        SCycleTime Time[MAX_SEQ_CYCLE] ;

        CVisionMan::TRetResult VsRslt ;

        int m_iHomeTO  ;
        int m_iStartTO ;
        int m_iStopTO  ;
        int m_iCycleTO ;

        void SetStartTime(EN_SEQ_CYCLE _iSeqCycle);
        void SetEndTime  (EN_SEQ_CYCLE _iSeqCycle);

        bool GetAdjustPos(double  _CrtX , double  _CrtY , double  _CrtT , //엔코더 값들....//계원꺼.
                          double  _OfsX , double  _OfsY , double  _OfsT , //비젼 결과 오프셑값들.
                          double &_RltX , double &_RltY , double &_RltT , //결과값. <===이걸 가지고 때려주면 된다.
                          bool    _bUseTheta);                            //세타보정 사용 여부.


        bool   FindChip   (EN_ARAY_ID _riWhre , int &r , int &c , EN_CHIP_STAT _csStat);
        //double GetWorkPos (EN_MOTR_ID _iMotr);
    public:     double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );




    public:    /* Direct Accessable Var.  */
        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );


        bool GetWorkEnd (){return Stat.bWorkEnd ;}
        bool GetReqStop (){return Stat.bReqStop ;}

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome = 0 ; }
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        AnsiString   GetCycleName   (){return m_sCycleName[Step.iSeq];}
        SStat        GetStat        (){return Stat          ;}

        
        void SetStat(SStat _sStat) { Stat = _sStat ; }

        double GetTimePitch (EN_SEQ_CYCLE _iSeqCycle){return Time[_iSeqCycle].dPitch ; }
        double GetTimeWork  (EN_SEQ_CYCLE _iSeqCycle){return Time[_iSeqCycle].dWork  ; }


        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlowSpeed = false); // 모터를 움직일때 쓰는 함수.
        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //실린더를 움직일때 쓰는 함수.

        //HomeCycle
        bool CycleHome(); //파트 홈을 잡을때 쓰는 함수.

        //Running Functions.
        bool ToStopCon (void); //스탑을 하기 위한 조건을 보는 함수.
        bool ToStartCon(void); //스타트를 하기 위한 조건을 보는 함수.
        bool ToStart   (void); //스타트를 하기 위한 함수.
        bool ToStop    (void); //스탑을 하기 위한 함수.
        bool Autorun   (void); //

        //One Cycle.
        bool CycleCal       (void); //비젼 켈리브레이션.
        bool CyclePenCheck  (void); //펜 나오는지 화이트 보드 가서 찍어보고 체크.
        bool CycleTAlign    (void); //비전 사각형으로 웨이퍼 세타 얼라인 잡는것.
        bool CycleOrigin    (void); //비젼 매치로 기준 칩 찾기.
        bool CycleChipAlign (void); //비전 사각형으로 칩 얼라인 위치 보정.
        bool CycleMark      (void); //펜마킹 & 경우에 따라 찍고 비젼 확인 까지.
        bool CycleWaitPos   (void); //대기 위치로 가기.
        bool CyclePenChange (void); //펜 교체 위치로 가기.
        bool CycleMarkTrace (void); //마크 하고 펜이 안찍힌경우 찍힌데까지 찾아감.
        bool CyclePenCoverPs(void); //펜 커버 위치로 이동후 실린더 동작함.

        //Vision Cycle
        bool CycleVisn     (EN_VISN_ID _iId , bool _bInit = false);
        //viWfr        = 0 , //칩
        //viWfo            , //피니셜마크
        //viWfp            , //펜 검사.
        //viCal            , //캘리브레이션.

        //Manual Cycle.
        //....
        bool CycleClickMove();
        bool CycleCntrMove ();


        //Click CallBackMove

        void SetClickMove(int _iCol ,int _iRow , TShiftState Shift);
        int  m_iClickMoveCol;
        int  m_iClickMoveRow;



        //For Start Stop Checking.
        void SetLastCmd (void); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (void); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (void); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        double GetWaferPitchX  (double _dOffsetX);
        double GetWaferPitchY  (double _dOffsetY);

        //Loading Para.
        void Load(bool IsLoad);

        int  m_iLastInspChipR;
        int  m_iLastInspChipC;

        int  m_iLastMrkChipR ;
        int  m_iLastMrkChipC ;

        bool m_bNeedOriDataSet ; //오리엔테이션칩 메뉴얼 세팅 필요...

        CQueData MarkData ;

        bool m_bNeedWideWfrChipInsp ;//칩 검사를 광역으로 할때 이용.
        bool m_bNeedWideWfoChipInsp ; //

        double m_dRevisePitchX ;
        double m_dRevisePitchY ;
        bool   m_bFnshCycTAlgn ;


        /*비전 테스트용.
        double m_dTempPosX ;
        double m_dTempPosY ;
        */

};

//---------------------------------------------------------------------------
extern CHead HED;
//---------------------------------------------------------------------------
#endif
