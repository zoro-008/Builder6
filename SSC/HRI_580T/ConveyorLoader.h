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
				double    __fastcall GetMotrPos  (EN_MOTR_ID Motr , EN_MOTR_COMD_ID Cmd , int PstnId = -1);                      //병권이 바보. 오키? 모터 포지션을 받아오는 함수.
				bool      __fastcall CheckDstb   (EN_MOTR_ID Motr , EN_MOTR_COMD_ID Cmd , int PstnId = -1 , double dPstn = 0.0); //병권이는 색마. 모터와 실린더가 움직이는데 있어서 간섭을 확인.
				SORT_INFO __fastcall GetSlotInfo (EN_ARAY_ID Aray , EN_FIND_MODE FindMode);                                      //병권이는 작업할 슬랏을 확인한다.

		public:    /* Direct Accessable Var.  */
				//다른 파트와 커뮤니 케이션을 위한 구조체.
		struct TRqst //다른 파트에 요청.
		{
		}Rqst ;
        struct TAply //다른 파트의 요청에 대한 대답.
        {
			bool bRBZStep ;
		}Aply ;
		struct TStat
		{
			bool bChckSplyEmpty ; //서플라이존이 컨베이어 벨트 돌려보고 카세트없음을 확인한 상태이다.
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
		void __fastcall Init  (void);  //프로그램 로딩 할때 한번만 타는 함수.
		void __fastcall Reset (void);  //리셑 버튼 눌렀을때 타는 함수.

		//Motor.
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		bool __fastcall MoveHome           (void                         ); //파트 홈을 잡을때 쓰는 함수.
		bool __fastcall MoveToLastWorkPosn (void                         ); //안씀.
		bool __fastcall MoveMotr           (EN_MOTR_ID Motr , EN_MOTR_COMD_ID Cmd , int PstnId = -1 , double dPstn = 0.0); // 모터를 움직일때 쓰는 함수.

		//Actuator.
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		bool __fastcall MoveActr (EN_ACTR_ID Actr , bool On); //실린더를 움직일때 쓰는 함수.

		//One Cycle.
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		bool __fastcall CycleMove (void); //컨베이어 벨트를 구동하여 카세트를 옮기는 함수.
		bool __fastcall CycleSply (void); //클렘프로 서플라이존에 있는 카세트를 찝어 오는 함수.
		bool __fastcall CycleStep (void); //Z축 - 카세트의 워크포지션으로 이동하는 함수.
		bool __fastcall CyclePush (void); //로더 푸셔로 스트립을 푸쉬 하는 함수.
		bool __fastcall CycleStck (void); //클램프에 있는 카세트를 스탁존에 내려 놓고 오는 함수.

        //Running Functions.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		bool __fastcall AutoRun   (void);  //오토런닝시에 계속 타는 함수.
		bool __fastcall ToStopCon (void);  //스탑을 하기 위한 조건을 보는 함수.
		bool __fastcall ToStartCon(void);  //스타트를 하기 위한 조건을 보는 함수.
		bool __fastcall ToStart   (void);  //스타트를 하기 위한 함수.
		bool __fastcall ToStop    (void);  //스탑을 하기 위한 함수.
		bool __fastcall IsWorkEnd (void);  //작업끝을 확인 하는 함수.(LOT END조건)

        //Loading Para.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        void __fastcall Load(bool IsLoad , FILE *fp);
};

//---------------------------------------------------------------------------
extern TConveyorLoader VL;

//---------------------------------------------------------------------------
#endif
