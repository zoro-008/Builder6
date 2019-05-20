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
    mcHDAlign       = 301  , //����� �����˻�.
    mcHDProbe       = 302  , //������ ���� ����
    mcHDWork        = 303  , //���� ���� �۾�.
    mcHDVisn        = 304  , //������ �����ε� Ȯ�ξ��� ����.
    mcHDClean       = 305  , //Ŭ�� ����.
    mcHDCalZ        = 306  , //Z�� Ķ���극�̼�.
    mcHDCalXY       = 307  , //XY�� Ķ���극�̼�.
    mcHDPurge       = 308  , //��¡.
    mcHDManWork     = 309  , //�޴��� ���� �е� ������ ����.
    mcHDGoToClean   = 310  , //Ŭ��������  XY�� �̵�.
    mcHDToolChange  = 311  , //�� ���� ����.
    mcHDGotoNdlWork = 312  , //ù��° ���� ���� �������� XY�� �̵�.
    mcHDPurgeAll    = 313  , //HD���� ��..
    mcHDCalT        = 314  , //���Է� ���ⷮ ����.
    mcHDManProbe    = 315  , //������ ���κ� �޴���� �ϴ� ����.
    mcHDMoveWait    = 316  , //�ص尡 ��� ��ġ�� ���� ����.



    mcCUHome        = 400  ,
    mcCUStock       = 401  ,
    mcCUStep        = 402  ,

    //��������Ŭ ����.
    mcMNAllCal      = 500  , //XYZ�� 3�� �ص� ���ÿ� �̸��극�̼� �ϴ� ����.
    mcMNInToPb      = 501  , //�δ����� ���� �������۷� ������ ����Ŭ �δ�,���� ���� ����.
    mcMNWkToOt      = 502  , //���� ��ũ������ ��δ��� ������ ����Ŭ ����. �δ�,���� ���� ����.
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
        //������ & �Ҹ���. (Constructor & Destructor)
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
        bool __fastcall CycleAllCal(void); //�ص��� XYZ���� ���ÿ� �����ϴ� ����Ŭ.
        bool __fastcall CycleInToPb(void); //�δ����� �������۱��� ���� ����Ŭ �δ��� ������ ���� �۾�.
        bool __fastcall CycleWkToOt(void); //��ũ������ ��δ� ���� ���� ����Ŭ ��δ��� ������ ���� �۾�.
        bool __fastcall CycleFeedIn(void); //�������ۿ��� ��ũ������ ���ͼ� ���ε� �Ǵ°� ����.
};

//---------------------------------------------------------------------------
extern TManProc MAN;

//---------------------------------------------------------------------------
#endif
