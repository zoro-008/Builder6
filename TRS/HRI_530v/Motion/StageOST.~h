//---------------------------------------------------------------------------

#ifndef StageOSTH
#define StageOSTH
//---------------------------------------------------------------------------
#include "PartInterface.h"

#include "Timer.h"
#include "DataMan.h"
#include "uRs232c.h"

#define MAX_OST_PIN_CNT 48
//원래 테라셈은 41핀까지만 있음. 16비트로 나누다 보니 48임.

//---------------------------------------------------------------------------
class CStageOST : public CPartInterface
{
    public: //초기화 등등.
        //Constructor
        CStageOST ();
        ~CStageOST ();

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
            scTest       =  2 ,
            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //마지막 사이클 시간. ms
        void   InitCycleName(){
            m_sCycleName[scIdle]="Idle";
            m_sCycleName[scWork]="Work";
            m_sCycleName[scTest]="Test";
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
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );



    public: //인터페이스 상속.==================================================
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

        //HomeCycle
        bool CycleHome(); //파트 홈을 잡을때 쓰는 함수.

        //One Cycle.
        bool CycleWork ();
        bool CycleTest ();


        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );


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
        double m_dLastIdxPos ;

        String m_sRcvMsg ;

        TRS232C * m_pRs232_OST ;
        static void ReceiveMsg(DWORD _cbInQue);

        //_iPockNo = 0,1,2,3
        bool GetOSTFail(int _iPockNo , String _sRcvMsg , bool &_bRslt);//Open Short Test
        bool GetIDDFail(int _iPockNo , String _sRcvMsg , bool &_bRslt);//구동 전류 테스트.
        bool GetIDSFail(int _iPockNo , String _sRcvMsg , bool &_bRslt);//대기 전류 테스트.
        bool GetI2CFail(int _iPockNo , String _sRcvMsg , bool &_bRslt);//I2C 페일 확인.



        struct TOSTRslt {
            bool   bRsltOSTFail[MAX_OST_PIN_CNT];

            double dRsltAVDD_IDD ;
            double dRsltCVDD_IDD ;
            double dRsltHVDD_IDD ;
            double dRsltDVDD_IDD ;

            double dRsltAVDD_IDS ;
            double dRsltCVDD_IDS ;
            double dRsltHVDD_IDS ;
            double dRsltDVDD_IDS ;
        };
        TOSTRslt OSTRslt[MAX_TOOL_SOCK_COL] ;

        void InitOstRslt(){
            memset(OSTRslt , 0 , sizeof(TOSTRslt)*4);
            for(int j = 0 ; j < MAX_TOOL_SOCK_COL ; j++) {
                for(int i = 0 ; i < MAX_OST_PIN_CNT ; i ++) {
                    OSTRslt[j].bRsltOSTFail[i] = true ; //Default fail setting.
                }
            }
        }

        bool WriteIDD_IDStoCSV(AnsiString _sFileName);

        int m_iOstNGCntPock1 ;
        int m_iOstNGCntPock2 ;
        int m_iOstNGCntPock3 ;
        int m_iOstNGCntPock4 ;

        int m_iCrtNGCntPock1 ;
        int m_iCrtNGCntPock2 ;
        int m_iCrtNGCntPock3 ;
        int m_iCrtNGCntPock4 ;

        int GetCrntOSTStgNo();




};

//---------------------------------------------------------------------------
extern CStageOST OST;
//---------------------------------------------------------------------------
#endif
