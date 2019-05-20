//---------------------------------------------------------------------------

#ifndef Rs232LaserH
#define Rs232LaserH
//---------------------------------------------------------------------------
#include "uRs232c.h"
#include "Timer.h"
#include "LaserEO.h"



class CRs232Laser
{
    public:
        //Constructor
        CRs232Laser (void);
        ~CRs232Laser (void);

        void Init();
        void Close();


    private:

    protected:
        CDelayTimer m_tmDipsr ;

        AnsiString m_sAskMsg ;
        AnsiString m_sReadMsg;
        AnsiString m_sSendMsg;
        AnsiString m_sErrMsg ;

        int m_iSeqCycle   ;
        int m_iPreSeqCycle;

        TRS232C * m_pRS232C;

        void RcvLaserData   (DWORD _cbInQue);



    public:
        //void SendMsg(double _dX, double _dY, double _dx, double _dy, double _dT);
        void SendMsg(EN_EO_CMD _Cmd , AnsiString _sMsg);
        AnsiString ReadMsg();
        void ClearReadMsg();

        bool Update(void);
};
extern CRs232Laser Rs232Laser ;
#endif
