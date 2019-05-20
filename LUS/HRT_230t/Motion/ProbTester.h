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
    prError    = 3  //���۽ÿ� ���� �ȵǾ� �ְų� ������� �̻��ϰ� ���°��.
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

        bool m_bProbeEnd ; //�˻� �������� Ȯ��.

        EN_PROBE_RSLT  m_iProbeRslt   ; //������� OK����.
        EN_PROBE_CMD   m_iProbeCmd    ; //���κ� ���.

        //���� ���� , ���� �˻�.
//        bool m_bFwdRslt ;
//        bool m_bBwdRslt ;
        bool m_bRslt ;

        int  m_iStep ;

    public:    /* Direct Accessable Var.  */
        void Update     (); //������.

        //One Cycle.
        bool CycleProbe (); //�˻����Ŭ.
        bool CycleReset (); //���� ����Ŭ   ���� �Ⱦ��� ���߿� Ȥ�� ���� �ɱ�� �����⸸ ��������.

        void StartCycle (EN_PROBE_CMD _iCmd);
        bool GetCycleEnd(); //����Ŭ �������� Ȯ��.
        EN_PROBE_RSLT GetProbeRslt();
};

//---------------------------------------------------------------------------
extern CProbeTester PTR;
//---------------------------------------------------------------------------
#endif
