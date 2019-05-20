//---------------------------------------------------------------------------
#ifndef ManProcH
#define ManProcH

//---------------------------------------------------------------------------
#include "Manual.h"
#include "OnDelay.h"
#include "Motor.h"
#include "Actuator.h"




/***************************************************************************/
/* Constants                                                               */
/***************************************************************************/
//...
//===========================================================================


/***************************************************************************/
/* Structures & Variables                                                  */
/***************************************************************************/
//...
//===========================================================================

enum EN_MANUAL_CYCLE
{
    mcNoneCycle     = -1   ,
    mcAllHome       =  0   ,
    mcDIS_XHome     =  1   ,
    mcDIS_YHome     =  2   ,
    mcHT1_XHome     =  3   ,
    mcHT3_XHome     =  4   ,
    mcHT1_YHome     =  5   ,
    mcHT3_YHome     =  6   ,
    mcHT1_ZHome     =  7   ,
    mcHT2_ZHome     =  8   ,
    mcHT3_ZHome     =  9   ,
    mcCLD_ZHome     =  10  , mcVLD_ZHome     =  10  ,
    mcCUD_ZHome     =  11  , mcVUD_ZHome     =  11  ,
    mcDH1_ZHome     =  12  ,
    mcDH2_ZHome     =  13  ,
    mcDH3_ZHome     =  14  ,
    mcVLD_YHome     =  15  ,
    mcVUD_YHome     =  16  ,

    mcCLHome        = 100  ,
    mcCLSuply       = 101  ,

    mcRLHome        = 200  ,
    mcRLToCst       = 201  ,
    mcRLToBuff2     = 202  ,
    mcRLMask        = 203  ,
    mcRLToWork      = 204  ,
    mcRLToBuff      = 205  ,
    mcRLHeat        = 206  ,
    mcRLWork        = 207  ,

    mcHDHome        = 300  ,
    mcHDAlign       = 301  , //얼라인 비젼검사.
    mcHDProbe       = 302  , //레이져 높이 측정
    mcHDWork        = 303  , //실제 토출 작업.
    mcHDVisn        = 304  , //비전런 동작인데 확인아직 안함.
    mcHDClean       = 305  , //클린 동작.
    mcHDCalZ        = 306  , //Z축 캘리브레이션.
    mcHDCalXY       = 307  , //XY축 캘리브레이션.
    mcHDPurge       = 308  , //퍼징.
    mcHDManWork     = 309  , //메뉴얼 더미 패드 위에서 토출.
    mcHDGoToClean   = 310  , //클린존으로  XY만 이동.
    mcHDToolChange  = 311  , //툴 정비 구역.
    mcHDGotoNdlWork = 312  , //첫번째 자제 토출 영역으로 XY만 이동.
    mcHDPurgeAll    = 313  , //HD퍼지 올..
    mcHDCalT        = 314  , //무게로 토출량 보정.
    mcHDManProbe    = 315  , //레이져 프로브 메뉴얼로 하는 동작.
    mcHDMoveWait    = 316  , //해드가 대기 위치로 가는 동작.



    mcCUHome        = 400  ,
    mcCUStock       = 401  ,
    mcCUStep        = 402  ,

    //여러싸이클 연동.
    mcMNAllCal      = 500  , //XYZ를 3개 해드 동시에 켈리브레이션 하는 동작.
    mcMNInToPb      = 501  , //로더에서 레일 프리버퍼로 들어오는 싸이클 로더,레일 연동 동작.
    mcMNWkToOt      = 502  , //레일 워크존에서 언로더로 나가는 싸이클 동작. 로더,레일 연동 동작.
    mcMNFeedIn      = 503  ,

    mcLotOpen       = 600  ,
    mcDummyDispn    = 601
};

/***************************************************************************/
/* TClass                                                                  */
/***************************************************************************/
//---------------------------------------------------------------------------
class TManProc
{
    private:   /* Member Var.             */
        //Object.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        TOnDelayTimer m_ChngMotrOnTimer ;
        TOnDelayTimer m_ChngMotrOffTimer;
        TOnDelayTimer m_ChngActrOnTimer ;
        TOnDelayTimer m_ChngActrOffTimer;
        TOnDelayTimer m_HomeTimer       ;
        TOnDelayTimer m_ManRunTimer     ;
        TOnDelayTimer m_CycleTimer      ;

        //Vars.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        int             m_iManStep    ;
        EN_MANUAL_CYCLE m_iManNo      ;
        EN_MANUAL_CYCLE m_iManCycleNo ;
        bool            m_bHoming     ; //For Homing.


        //Internal Vars.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        bool       m_bRptMotr      ; //For Repeat Functions.
        bool       m_bDirMotr      ;
        bool       m_bRptActr      ;
        bool       m_bDirActr      ;
        EN_MOTR_ID m_iRMotrID      ;
        EN_ACTR_ID m_iRActrID      ;
        int        m_iChngMotrDlay ;
        int        m_iChngActrDlay ;
        double     m_dP1           ;
        double     m_dV1           ;
        double     m_dA1           ;
        double     m_dP2           ;
        double     m_dV2           ;
        double     m_dA2           ;

        bool  r1,r2,r3,r4,r5 ;

    protected: /* Inheritable Func.       */
        void __fastcall SetAllHomeStep (void );

        //User Defined Functions.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        bool __fastcall RptMotr (void);
        bool __fastcall RptActr (void);

    public:    /* Direct Accessable Var.  */
        //Property.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        __property int              _iManStep    = { read = m_iManStep    };
        __property EN_MANUAL_CYCLE  _iManNo      = { read = m_iManNo   , write =  m_iManNo   };
        __property bool             _bHoming     = { read = m_bHoming     };


        //Buffers
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        FUNC_ARG  FuncArg; //Manual Processing Function Arguments.

    public:    /* Direct Accessable Func. */
        //생성자 & 소멸자. (Constructor & Destructor)
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        __fastcall  TManProc(void);
        __fastcall ~TManProc(void);

        //Init.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        void __fastcall Init (void);
        void __fastcall Reset(void) { Init(); }

        //Master Functions.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        void __fastcall SetRptMotr(EN_MOTR_ID Motr , bool Flag                                                            ) { m_iRMotrID      = Motr; m_bRptMotr = Flag; m_bDirMotr  = false;                                                                       }
        void __fastcall SetRMPara (int        Dlay , double P1 , double V1 , double A1 , double P2 , double V2 , double A2) { m_iChngMotrDlay = Dlay; m_dP1      = P1  ; m_dV1       = V1   ; m_dA1       = A1; m_dP2       = P2; m_dV2        = V2  ; m_dA2 = A2 ; }
        void __fastcall SetRptActr(EN_ACTR_ID Actr , bool Flag                                                            ) { m_iRActrID      = Actr; m_bRptActr = Flag; m_bDirActr  = false;                                                                       }
        void __fastcall SetRAPara (int        Dlay                                                                        ) { m_iChngActrDlay = Dlay;                                                                                                               }

        //Homing.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        bool __fastcall MoveAllHome(void );

        //Manual Processing.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        bool __fastcall SetManCycle(EN_MANUAL_CYCLE No );
        bool __fastcall ManCycleRun(void               );

        //ManCycle.
        bool __fastcall CycleAllCal(void); //해드의 XYZ축을 동시에 보정하는 싸이클.
        bool __fastcall CycleInToPb(void); //로더에서 프리버퍼까지 가는 싸이클 로더와 레일의 연동 작업.
        bool __fastcall CycleWkToOt(void); //워크존에서 언로더 까지 가는 싸이클 언로더와 레일의 연동 작업.
        bool __fastcall CycleFeedIn(void); //프리버퍼에서 워크존까지 들어와서 바인딩 되는것 까지.
};

//---------------------------------------------------------------------------
extern TManProc MAN;

//---------------------------------------------------------------------------
#endif
