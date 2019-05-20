//---------------------------------------------------------------------------
#ifndef HeadH
#define HeadH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
//---------------------------------------------------------------------------
//Vision
//---------------------------------------------------------------------------
#include "SAInterface.h"
//---------------------------------------------------------------------------
//static void __stdcall HedZStripWorkWait(long _lAxis, DWORD _dwFlag);
//static void __stdcall HedZWaferWorkWait(long _lAxis, DWORD _dwFlag);
//static void __stdcall HedZWait         (long _lAxis, DWORD _dwFlag);
#define VOICE_COIL_ID     0

enum EN_ARAY_STEP {
    hsNotWork = 0 ,
    hsGrabStt     ,
    hsEmpty       ,
    hsWorkEnd     ,
    hsWorkFail    ,
};
class CHead
{
    public:
        //Constructor
         CHead ();
        ~CHead ();

        void Init      ();
        void Close     ();
        void Reset     (); //���V ��ư �������� Ÿ�� �Լ�.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle     = 0 ,
            scPick     = 1 ,
            scPlace    = 2 ,

            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName [MAX_SEQ_CYCLE] ;
        void InitCycleName(){
            m_sCycleName[scIdle ]="Idle ";
            m_sCycleName[scPick ]="Pick ";
            m_sCycleName[scPlace]="Place";

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

        double m_dLastIdxPos ;

        int m_iHomeTO  ;
        int m_iStartTO ;
        int m_iStopTO  ;
        int m_iCycleTO ;



        bool   FindChip   (EN_ARAY_ID _riWhre , int &r , int &c , EN_CHIP_STAT _stats = csUnkwn );
        double GetWorkPos (EN_MOTR_ID _iMotr);
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

    private:
        static void __stdcall CallAxisZWaferPs (long _lAxis, DWORD _dwFlag);
        static void __stdcall CallAxisZWaferVc (long _lAxis, DWORD _dwFlag);
        static void __stdcall CallVisnAtGrib   (long _lAxis, DWORD _dwFlag);

        static void __stdcall CallAxisZStripPs (long _lAxis, DWORD _dwFlag);

        static void __stdcall CallAxisYWaferPs (long _lAxis, DWORD _dwFlag);


        static void __stdcall HedZStripWorkWait(long _lAxis, DWORD _dwFlag);
        static void __stdcall HedZWaferWorkWait(long _lAxis, DWORD _dwFlag);
        static void __stdcall HedZWaitPick     (long _lAxis, DWORD _dwFlag);
        static void __stdcall HedZWaitPlace    (long _lAxis, DWORD _dwFlag);

    public:    /* Direct Accessable Var.  */
        bool   m_bDispPickCal  ;
        bool   m_bDispPlaceCal ;
        double m_dPickCalPos   ;
        double m_dPlaceCalPos  ;



        double m_dVoiceCoilWafer   ;
        double m_dVoiceCoilWafer1  ;
        double m_dVoiceCoilStrip   ;
        double m_dVoiceCoilStrip1  ;

        double m_dPickCycleTime ;
        void   SetIntrptPos1();
        void   SetIntrptPos2();
        void   SetIntrptPos3();
        void   SetIntrptPos4();

        double GetVoiceCoilVolt(double _dGram);

        int    iStgCheck ;

        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

  
        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome = 0 ; m_iHomeSubStep = 10;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        SStat        GetStat        (){return Stat          ;}
        AnsiString   GetCycleName   (){return m_sCycleName[Step.iSeq];}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

        //Property.
//        __property int   _iMgzCnt      = { read = m_iMgzCnt    , write= m_iMgzCnt };
        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlowSpeed = false); // ���͸� �����϶� ���� �Լ�.
        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.

        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.
        bool CycleHomeSub(); //������ ����̸� �������.

        //Running Functions.
        bool ToStopCon (void); //��ž�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStartCon(void); //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStart   (void); //��ŸƮ�� �ϱ� ���� �Լ�.
        bool ToStop    (void); //��ž�� �ϱ� ���� �Լ�.
        bool Autorun   (void); //

        //One Cycle.
        bool CyclePick      (void); //�޴����϶� ������ T�� �ȵ�����.
        bool CyclePickCurve (void);
        bool CyclePlace     (void);
        bool CyclePlaceCurve(void);

        //Manual Cycle.
        bool CyclePickCal   (void); //���ϴ� ���� ����. �����ۿ� �޴���� Ĩ����� ���� ��ģä�� ���� �Ѵ�.
        bool CyclePlaceCal  (void); //�÷��̽� �ϴ� ���� ����. ���Ͽ� ��Ʈ�� ������ �ִ� ä�� ���� �Ѵ�.
        bool CycleWfrPlace  (void); //���Ѱ��� �����ۿ� �ٽ� ���� ���� �۾� �� ���� �Ҷ� ���δ�.
        bool CycleRalPlace  (void); //���Ѱ��� �÷��̽� ���Ϳ� ���°�. �� ���� �Ҷ� ���δ�.


        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool IsLoad);

        bool InspectToolSafe();


        int m_iHomeSubStep ;

};

//---------------------------------------------------------------------------
extern CHead HED;
//---------------------------------------------------------------------------
#endif
