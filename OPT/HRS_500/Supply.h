//---------------------------------------------------------------------------

#ifndef SupplyH
#define SupplyH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "UtilDefine.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------


enum EN_LEFT_RIGHT {
    lrLeftBin  ,
    lrRightBin

};



class CSupply
{
    public:
        //Constructor
        CSupply (void);
        ~CSupply (void);

        void Init(EN_LEFT_RIGHT _iPartId );

        void Close();

        void Reset     (); //���V ��ư �������� Ÿ�� �Լ�.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd ; //��Ʈ�� ������ ������� true ;
            bool bReqStop ; //������ ���ų� Ȥ�� ��ž��ư�� �������� ��Ʈ�� ���⸦ ��û�ϴ� ������.
        };

        enum EN_SEQ_CYCLE {
            scIdle      =  0  ,
            scSupply    = 10  , //������ �ؾ� �Ҷ�.
            scSupplyAll = 20  , //��� ���� ���� �Ҷ�.
            MAX_SEQ_CYCLE
        };

        struct SStep {
            int          iHome    ; //Ȩ����Ŭ���� ���� ����.
            int          iToStart ; //����ŸƮ ����Ŭ���� ��.
            EN_SEQ_CYCLE iSeq     ; //����� ���� ����.
            int          iCycle   ; //����� ���Ͽ��� ����.
            int          iToStop  ; //����ž ����Ŭ���� ��.
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

        SStat Stat ;
        SStep Step , PreStep ;

//        int  m_iPkgCnt[MAX_BIN_CNT];
        int  m_iSetBin ;  // �����Ұ��� �̸� ���� ��. -1�̸� �ƹ��͵� �ƴϰ� 0~7���� ��ѹ�.
        bool m_bSetAll ;  // ��� ���� �ϰ� ������ ������.

        bool m_bOpenBin[MAX_BIN_CNT]  ;



        EN_ACTR_ID  aiSPL_Bn0 ; EN_INPUT_ID xSPL_Detect0 ;
        EN_ACTR_ID  aiSPL_Bn1 ; EN_INPUT_ID xSPL_Detect1 ;
        EN_ACTR_ID  aiSPL_Bn2 ; EN_INPUT_ID xSPL_Detect2 ;
        EN_ACTR_ID  aiSPL_Bn3 ; EN_INPUT_ID xSPL_Detect3 ;
        EN_ACTR_ID  aiSPL_Bn4 ; EN_INPUT_ID xSPL_Detect4 ;
        EN_ACTR_ID  aiSPL_Bn5 ; EN_INPUT_ID xSPL_Detect5 ;
        EN_ACTR_ID  aiSPL_Bn6 ; EN_INPUT_ID xSPL_Detect6 ;
        EN_ACTR_ID  aiSPL_Bn7 ; EN_INPUT_ID xSPL_Detect7 ;
        EN_ACTR_ID  aiSPL_Bn8 ; EN_INPUT_ID xSPL_Detect8 ;
        EN_ACTR_ID  aiSPL_Bn9 ; EN_INPUT_ID xSPL_Detect9 ;

        int   GetOpenBin(double _dVal);

        bool   FindChip   (int &r , int &c );
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //���Ͱ� ������ �������� ������. Double��.

        bool   GetLastStrip(); //���� ��Ʈ�� ��� �����Ͱ� ���� �Ҷ��� ����. �� ���������� üũ�ؾ���.

    public:    /* Direct Accessable Var.  */

        int  m_iPkgCnt[MAX_BIN_CNT];
        bool m_bSetSupplyAll ;

        double dIndicator ;
//        double dRIndicator ;

        void SetBin   (int _iBin , bool _bAll) { m_iSetBin = _iBin ; m_bSetAll = _bAll; } //����߸��� �������ִ� �Լ�.
        int  GetPkgCnt(int _iBin)              { return m_iPkgCnt[_iBin] ;              }

        bool CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //���Ͱ� �����϶� �浹���� ����.
        bool CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    ); //�Ǹ����� �����϶� �浹���� ����.

        bool GetWorkEnd (){return Stat.bWorkEnd ;}
        bool GetReqStop (){return Stat.bReqStop ;}

        int           GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){Step.iHome  = 10;}
        int           GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int           GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){Step.iCycle = 10;}
        int           GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        SStat         GetStat        (){return Stat          ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

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
        bool Update      (void); //������Ʈ �ÿ� Ÿ�� �����ÿ� ������� ���� �ϱ� ���� ����.

	    //One Cycle.
        bool CycleSupply    (void); //���۰� ������� ������ ���� ����.
        bool CycleSupplyAll (void); //���۰� ������� ������ ���� ����.

        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool IsLoad);

        static void procCom2(DWORD _cbInQue);
        static void procCom3(DWORD _cbInQue);
};

//---------------------------------------------------------------------------
extern CSupply SPL; //���ʺ�
extern CSupply SPR; //�����ʺ�

//---------------------------------------------------------------------------
#endif
