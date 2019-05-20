//---------------------------------------------------------------------------
#ifndef ProbTesterH
#define ProbTesterH
//---------------------------------------------------------------------------
#include "Timer.h"
//---------------------------------------------------------------------------
enum EN_PROBE_RSLT {
    prFail     = 0 ,
    prGood     = 1 ,
//    prBwdGood  = 2 ,
    prTimeOut  = 2 ,
    prError    = 3  //시작시에 레디가 안되어 있거나 결과값이 이상하게 나온경우.
};

enum EN_PROBE_CMD {
    pcNone  = 0 ,
    pcProbe = 1 ,
    pcReset = 2

};


class CProbeTester
{
    public:
        //Constructor
         CProbeTester ();
        ~CProbeTester ();

    protected:
        AnsiString  m_sPartName ;
        //Timer.
        CDelayTimer m_tmCycle   ;
        CDelayTimer m_tmTemp    ;

        bool m_bProbeEnd ; //검사 끝났는지 확인.

        EN_PROBE_RSLT  m_iProbeRslt   ; //결과값이 OK인지.
        EN_PROBE_CMD   m_iProbeCmd    ; //프로브 명령.

        //자제 정방 , 역방 검사.
//        bool m_bFwdRslt ;
//        bool m_bBwdRslt ;
        bool m_bRslt ;

        int  m_iStep ;

    public:    /* Direct Accessable Var.  */
        void Update     (); //뺑뺑이.

        //One Cycle.
        bool CycleProbe (); //검사싸이클.
        bool CycleReset (); //리쎝 싸이클   현재 안쓰나 나중에 혹시 문제 될까봐 껍데기만 만들어놓음.

        void StartCycle (EN_PROBE_CMD _iCmd);
        bool GetCycleEnd(); //싸이클 끝났는지 확인.
        EN_PROBE_RSLT GetProbeRslt();
};

//---------------------------------------------------------------------------
extern CProbeTester PTR;
//---------------------------------------------------------------------------
#endif
