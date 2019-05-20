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
    public: //�ʱ�ȭ ���.
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
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //������ ����Ŭ �ð�. ms
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

        String GetSendVcmConMsgSimplex(int _iCrnt); //�ܹ���˻��� ���.
        String GetSendVcmConMsgDuplex (int _iCrnt); //�����˻��� ���.

    public: //�������̽� ���.==================================================
        int    m_iInspTime ; //�˻� �ð�.

        double GetHeight(String _sRcvMsg);
        String GetSendVcmConMsg(int _iCrnt);


        bool GetAsciiFromStringNo(String _sStringNo , int _iDigit , String & _sAsciiNo);

	void Reset (); //���V ��ư �������� Ÿ�� �Լ�.

        //Running Functions.
        bool ToStopCon (); //��ž�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStartCon(); //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStart   (); //��ŸƮ�� �ϱ� ���� �Լ�.
        bool ToStop    (); //��ž�� �ϱ� ���� �Լ�.

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;}void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome  = 0 ;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        int          GetSeqStep     (){return (int)Step.iSeq;} int GetPreSeqStep    (){return (int)PreStep.iSeq;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;}void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}

        AnsiString   GetCycleName   (int _iSeq){return m_sCycleName[_iSeq];}
        int          GetCycleTime   (int _iSeq){return m_iCycleTime[_iSeq];}
        AnsiString   GetPartName    (){return m_sPartName   ;}

        int          GetCycleMaxCnt (){return MAX_SEQ_CYCLE ;}

        bool Autorun   (); //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
        //======================================================================
    public:
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

        //One Cycle.
        bool CycleWork ();


        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID    _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool          _bFwd    );
        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_VALUE _iPstnVal); //Position Form Callback.


        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // ���͸� �����϶� ���� �Լ�.
        void MoveIncMotr (EN_MOTR_ID _iMotr , double _iPstn); // ���͸� �����϶� ���� �Լ�.
        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.



        //For Start Stop Checking.
        void SetLastCmd (); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool _bLoad);

        //���߿� ���ͷ� ������. SaveCrntCmdPos()
        double m_dLastIdxPos ; //ARAY

        TRS232C * m_pVcmCon ; //��� VCM ��Ʈ�ѷ�.
        TRS232C * m_pHgtSnr ; //Ű���� ����������.

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
            rtCurrentRepeat     , //1��PKG ���������ϸ鼭 ����.
            rtPinRepeat         , //1��PKG �ɸ� ������ �ϸ鼭 ����.
            rtChuckPinRepeat    , //1��PKG Ȧ�� �� ���� ���� ����.
            rtFullRepeat        , //5��PKG ��ü ����.
            MAX_REPEAT_TYPE
        };

        EN_REPEAT_TYPE m_eRepeatType ;
        int  m_iRepeatCnt ;



};

//---------------------------------------------------------------------------
extern CStage STG;
//---------------------------------------------------------------------------
#endif
