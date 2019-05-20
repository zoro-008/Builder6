//---------------------------------------------------------------------------

#ifndef ConveyorLoaderH
#define ConveyorLoaderH
//---------------------------------------------------------------------------
#include "IoUnit.h"
#include "Actuator.h"
#include "Motor.h"
#include "OnDelay.h"
#include "DataMan.h"
#include "Sequence.h"

/***************************************************************************/
/* Constants                                                               */
/***************************************************************************/


/***************************************************************************/
/* Structures & Variables                                                  */
/***************************************************************************/


/***************************************************************************/
/* TClass                                                                  */
/***************************************************************************/
//---------------------------------------------------------------------------
class TConveyorLoader
{
    private:   /* Member Var.             */
        //Timer.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        TOnDelayTimer m_MainCyleTimer; //Main Cycle Timer.
        TOnDelayTimer m_OneCyleTimer ; //One  Cycle Timer.
        TOnDelayTimer m_HomeTimer    ; //Home       Timer.
        TOnDelayTimer m_ToStopTimer  ; //ToStop     Timer.
        TOnDelayTimer m_ToStartTimer ; //ToStart    Timer.
        TOnDelayTimer m_TempTimer    ; //ToStart    Timer.

        //Vars.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        bool   m_bWorkEnd      ; //Work End Flag.
        bool   m_bToStart      ; //To... Flag.
        bool   m_bToStop       ;
        bool   m_bReqStop      ;

        int    m_iSeqStep      ; //Step.
        int    m_iCycleStep    ;
        int    m_iPreCycleStep ;
        int    m_iHomeStep     ;

        AnsiString sPreMsg ;
        AnsiString sMsg    ;

        //Common Local
        bool r1 , r2 ,r3 , r4 , r5;
        AnsiString Temp ;

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				double    __fastcall GetMotrPos  (EN_MOTR_ID Motr , EN_MOTR_COMD_ID Cmd , int PstnId = -1);                      //������ �ٺ�. ��Ű? ���� �������� �޾ƿ��� �Լ�.
				bool      __fastcall CheckDstb   (EN_MOTR_ID Motr , EN_MOTR_COMD_ID Cmd , int PstnId = -1 , double dPstn = 0.0); //�����̴� ����. ���Ϳ� �Ǹ����� �����̴µ� �־ ������ Ȯ��.
				SORT_INFO __fastcall GetSlotInfo (EN_ARAY_ID Aray , EN_FIND_MODE FindMode);                                      //�����̴� �۾��� ������ Ȯ���Ѵ�.

		public:    /* Direct Accessable Var.  */
				//�ٸ� ��Ʈ�� Ŀ�´� ���̼��� ���� ����ü.
		struct TRqst //�ٸ� ��Ʈ�� ��û.
		{
		}Rqst ;
        struct TAply //�ٸ� ��Ʈ�� ��û�� ���� ���.
        {
			bool bRBZStep ;
		}Aply ;
		struct TStat
		{
			bool bChckSplyEmpty ; //���ö������� �����̾� ��Ʈ �������� ī��Ʈ������ Ȯ���� �����̴�.
		}Stat;
        
        SORT_INFO SORT;

        __property bool   _bWorkEnd      = {read = m_bWorkEnd      , write = m_bWorkEnd     };
        __property bool   _bToStart      = {read = m_bToStart                               };
        __property bool   _bToStop       = {read = m_bToStop                                };
        __property bool   _bReqStop      = {read = m_bReqStop      , write = m_bReqStop     };

        __property int    _iSeqStep      = {read = m_iSeqStep                               };
        __property int    _iCycleStep    = {read = m_iCycleStep    , write = m_iCycleStep   };
        __property int    _iPreCycleStep = {read = m_iPreCycleStep , write = m_iPreCycleStep};
        __property int    _iHomeStep     = {read = m_iHomeStep     , write = m_iHomeStep    };

        //Constructor
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        __fastcall TConveyorLoader (void);
        __fastcall ~TConveyorLoader (void);

        //Init.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		void __fastcall Init  (void);  //���α׷� �ε� �Ҷ� �ѹ��� Ÿ�� �Լ�.
		void __fastcall Reset (void);  //���V ��ư �������� Ÿ�� �Լ�.

		//Motor.
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		bool __fastcall MoveHome           (void                         ); //��Ʈ Ȩ�� ������ ���� �Լ�.
		bool __fastcall MoveToLastWorkPosn (void                         ); //�Ⱦ�.
		bool __fastcall MoveMotr           (EN_MOTR_ID Motr , EN_MOTR_COMD_ID Cmd , int PstnId = -1 , double dPstn = 0.0); // ���͸� �����϶� ���� �Լ�.

		//Actuator.
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		bool __fastcall MoveActr (EN_ACTR_ID Actr , bool On); //�Ǹ����� �����϶� ���� �Լ�.

		//One Cycle.
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		bool __fastcall CycleMove (void); //�����̾� ��Ʈ�� �����Ͽ� ī��Ʈ�� �ű�� �Լ�.
		bool __fastcall CycleSply (void); //Ŭ������ ���ö������� �ִ� ī��Ʈ�� ��� ���� �Լ�.
		bool __fastcall CycleStep (void); //Z�� - ī��Ʈ�� ��ũ���������� �̵��ϴ� �Լ�.
		bool __fastcall CyclePush (void); //�δ� Ǫ�ŷ� ��Ʈ���� Ǫ�� �ϴ� �Լ�.
		bool __fastcall CycleStck (void); //Ŭ������ �ִ� ī��Ʈ�� ��Ź���� ���� ���� ���� �Լ�.

        //Running Functions.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		bool __fastcall AutoRun   (void);  //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
		bool __fastcall ToStopCon (void);  //��ž�� �ϱ� ���� ������ ���� �Լ�.
		bool __fastcall ToStartCon(void);  //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
		bool __fastcall ToStart   (void);  //��ŸƮ�� �ϱ� ���� �Լ�.
		bool __fastcall ToStop    (void);  //��ž�� �ϱ� ���� �Լ�.
		bool __fastcall IsWorkEnd (void);  //�۾����� Ȯ�� �ϴ� �Լ�.(LOT END����)

        //Loading Para.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        void __fastcall Load(bool IsLoad , FILE *fp);
};

//---------------------------------------------------------------------------
extern TConveyorLoader VL;

//---------------------------------------------------------------------------
#endif
