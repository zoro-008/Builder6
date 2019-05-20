//---------------------------------------------------------------------------

#ifndef StageH
#define StageH
//---------------------------------------------------------------------------
#include "PartInterface.h"
#include "UtilDefine.h"
#include "uRs232c.h"

#include "Timer.h"


//---------------------------------------------------------------------------
class CStage : public CPartInterface
{
    public: //초기화 등등.
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
            scWork       =  1 ,
            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //마지막 사이클 시간. ms
        void   InitCycleName(){
            m_sCycleName[scIdle]="Idle";
            m_sCycleName[scWork]="Work";
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

        SStat Stat ;
        SStep Step , PreStep ;

        SStat GetStat(){return Stat;}

        void ResetTimer();

        bool   FindChip   (int &r , int &c   );

        String GetSendVcmConMsgSimplex(int _iCrnt); //단방향검사일 경우.
        String GetSendVcmConMsgDuplex (int _iCrnt); //양방향검사일 경우.

    public: //인터페이스 상속.==================================================
        int    m_iInspTime ; //검사 시간.

        double GetHeight(String _sRcvMsg);
        String GetSendVcmConMsg(int _iCrnt);


        bool GetAsciiFromStringNo(String _sStringNo , int _iDigit , String & _sAsciiNo);

	void Reset (); //리셑 버튼 눌렀을때 타는 함수.

        //Running Functions.
        bool ToStopCon (); //스탑을 하기 위한 조건을 보는 함수.
        bool ToStartCon(); //스타트를 하기 위한 조건을 보는 함수.
        bool ToStart   (); //스타트를 하기 위한 함수.
        bool ToStop    (); //스탑을 하기 위한 함수.

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;}void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome  = 0 ;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        int          GetSeqStep     (){return (int)Step.iSeq;} int GetPreSeqStep    (){return (int)PreStep.iSeq;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;}void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}

        AnsiString   GetCycleName   (int _iSeq){return m_sCycleName[_iSeq];}
        int          GetCycleTime   (int _iSeq){return m_iCycleTime[_iSeq];}
        AnsiString   GetPartName    (){return m_sPartName   ;}

        int          GetCycleMaxCnt (){return MAX_SEQ_CYCLE ;}

        bool Autorun   (); //오토런닝시에 계속 타는 함수.
        //======================================================================
    public:
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

        //HomeCycle
        bool CycleHome(); //파트 홈을 잡을때 쓰는 함수.

        //One Cycle.
        bool CycleWork ();


        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID    _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool          _bFwd    );
        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_VALUE _iPstnVal); //Position Form Callback.


        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // 모터를 움직일때 쓰는 함수.
        void MoveIncMotr (EN_MOTR_ID _iMotr , double _iPstn); // 모터를 움직일때 쓰는 함수.
        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //실린더를 움직일때 쓰는 함수.



        //For Start Stop Checking.
        void SetLastCmd (); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool _bLoad);

        //나중에 모터로 빼야함. SaveCrntCmdPos()
        double m_dLastIdxPos ; //ARAY

        TRS232C * m_pVcmCon ; //백상 VCM 컨트롤러.
        TRS232C * m_pHgtSnr ; //키엔스 높이측정기.

        //rs232 callback
        static void ReceiveVcmConMsg(DWORD _cbInQue);
        static void ReceiveHgtSnrMsg(DWORD _cbInQue);

        //RS232 VCM
        bool   SetVCMCrnt(int _iCrnt);
        bool   RqstVCMCrntVolt();
        double GetRegistance(double _dCrnt , double _dVolt);
        double GetCurrent(String _sMsg);
        double GetVolt   (String _sMsg);

        //void   SendVCMCrnt(int _iCrnt);


        int m_iActiveStage ;

        enum EN_REPEAT_TYPE {
            rtNone          = 0 ,
            rtCurrentRepeat     , //1개PKG 컨텍유지하면서 리핏.
            rtPinRepeat         , //1개PKG 핀만 리컨텍 하면서 리핏.
            rtChuckPinRepeat    , //1개PKG 홀더 와 핀을 같이 리핏.
            rtFullRepeat        , //5개PKG 전체 리핏.
            MAX_REPEAT_TYPE
        };

        EN_REPEAT_TYPE m_eRepeatType ;
        int  m_iRepeatCnt ;



};

//---------------------------------------------------------------------------
extern CStage STG;
//---------------------------------------------------------------------------
#endif
