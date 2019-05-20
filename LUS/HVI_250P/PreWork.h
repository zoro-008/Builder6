//---------------------------------------------------------------------------

#ifndef WorkZoneH
#define WorkZoneH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
#include "LotUnit.h"
//---------------------------------------------------------------------------
#define  MAXBIN  5
enum EN_WRK_STEP {
    asWRK_ToWrkStt    =  0  ,
    asWRK_1IdxWork          ,
    asWRK_To2Idx            ,
    asWRK_2IdxWorkStt       ,
    asWRK_2IdxWork          ,
    asWRK_2IdxReWork        ,
    asWRK_2IdxPull          ,
    asWRK_2IdxOut           ,
    asWRK_None              ,

    MAX_WRK_STEP
};

class CPreWork
{
    public:
        //Constructor
        CPreWork (void);
        ~CPreWork (void);
	void Reset     (); //���V ��ư �������� Ÿ�� �Լ�.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd ; //��Ʈ�� ������ ������� true ;
            bool bReqStop ; //������ ���ų� Ȥ�� ��ž��ư�� �������� ��Ʈ�� ���⸦ ��û�ϴ� ������.
        };

        enum EN_SEQ_CYCLE {
            scIdle        =  0  ,
            scWrkStt      = 10  ,
            sc1IdxWork    = 20  ,
            scTo2Idx      = 30  ,
            sc2IdxWorkStt = 40  ,
            sc2IdxWork    = 50  ,
            sc2IdxReWork  = 60  ,
            sc2IdxOut     = 70  ,
            MAX_SEQ_CYCLE
        };

        struct SStep {
            int          iHome    ; //Ȩ����Ŭ���� ���� ����.
            int          iToStart ; //����ŸƮ ����Ŭ���� ��.
            EN_SEQ_CYCLE iSeq     ; //����� ���� ����.
            int          iCycle   ; //����� ���Ͽ��� ����.
            int          iToStop  ; //����ž ����Ŭ���� ��.

//            EN_SEQ_CYCLE iSubSeq  ; //Sub Autorun �ϳ��� ������� ����غ�.dd
//            int          iSubCycle; //Sub
        };

        struct SBinInfo {
            int  iBinCnt ;
            bool bBinFull;
        };

    protected:
        AnsiString  m_sPartName ;
        //Timer.
        CDelayTimer m_tmMain    ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmCycle   ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmHome    ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmToStop  ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmToStart ; //Ÿ�Ӿƿ���.
        CDelayTimer m_tmTemp    ; //����Ŭ �Լ����� ������� ���¾�.

        CDelayTimer m_tmSubCycle; //SubŸ�Ӿƿ���.         

        SStat Stat ;              //
        SStep Step , PreStep ;
//        SStep SubStep , SubPreStep ;

        double     m_dLast1IdxPos  ;
        double     m_dLast2IdxPos  ;
        double     m_dLastXSrtPos  ;
        //        bool m_bWaitLotChange ;

        bool   FindChip   (int &r , int &c );
	double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //���Ͱ� ������ �������� ������. Double��.

    public:    /* Direct Accessable Var.  */

        SBinInfo Bin1 , Bin2 ,Bin3 , Bin4 , Bin5 ;

        bool           m_bLotEnded  ; //ȭ�鿡 ������ �� �����.
        int            iGoodCnt ;

        unsigned long  m_lStrpCycleTime ; //���� �۾� �ð�.... ȭ�� ���ǿ�.
        unsigned long  m_lPreCycleTime  ;

        int m_iBinCnt ;
        String m_sLastStrip ;
        String m_sPreLotNo  ;
        double m_dFailChipPos ;

//        int iBin1Cnt , iBin2Cnt , iBin3Cnt , iBin4Cnt , iBin5Cnt ;

        CLot::SLotInfo EndedLot ;

        void Init ();
        void Close();
        
        double RejectMove  (void);
        bool   BinFullCheck(    );
        bool   BinInfoAdd  (    );

        bool   GetLastStrip();
        bool   LotInfoAdd  ();

        bool CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //���Ͱ� �����϶� �浹���� ����.
        bool CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    ); //�Ǹ����� �����϶� �浹���� ����.

        bool GetWorkEnd (){return Stat.bWorkEnd ;}
        bool GetReqStop (){return Stat.bReqStop ;}

        int           GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep    (){return PreStep.iHome       ;} void InitHomeStep (){Step.iHome  = 10;}
        int           GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep (){return PreStep.iToStart    ;}
        EN_SEQ_CYCLE  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep     (){return PreStep.iSeq        ;}
        int           GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep   (){return PreStep.iCycle      ;} void InitCycleStep   (){Step.iCycle    = 10;}

        int           GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep  (){return PreStep.iToStop     ;}
        SStat         GetStat        (){return Stat          ;}

        EN_SEQ_CYCLE  GetSubSeqStep  (){return Step.iSubSeq  ;} int GetPreSubSeqStep  (){return PreStep.iSubSeq     ;}
        int           GetSubCycleStep(){return Step.iSubCycle;} int GetPreSubCycleStep(){return PreStep.iSubCycle   ;} void InitSubCycleStep(){Step.iSubCycle = 10;}

        void SetStat   (SStat _sStat) { Stat = _sStat ; }

	//Motor.
	bool MoveMotr (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // ���͸� �����϶� ���� �Լ�.

	//Actuator.
	bool MoveActr (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.

        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

        //Running Functions.
	bool ToStopCon   (void); //��ž�� �ϱ� ���� ������ ���� �Լ�.
	bool ToStartCon  (void); //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
	bool ToStart     (void); //��ŸƮ�� �ϱ� ���� �Լ�.
	bool ToStop      (void); //��ž�� �ϱ� ���� �Լ�.
        bool Autorun     (void); //���䷱�׽ÿ� ��� Ÿ�� �Լ�.

	//One Cycle.
        bool CycleToWrkStt   (void); //
        bool Cycle1IdxWork   (void); //
        bool CycleTo2Idx     (void); //
        bool Cycle2IdxWorkStt(void); //
        bool Cycle2IdxWork   (void); //
        bool Cycle2IdxReWork (void); //
        bool Cycle2IdxOut    (void); //
                
        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool IsLoad);

};

//---------------------------------------------------------------------------
extern CPreWork WRP;

//---------------------------------------------------------------------------
#endif
