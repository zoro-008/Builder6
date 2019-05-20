//---------------------------------------------------------------------------

#ifndef Rs232LoadClH
#define Rs232LoadClH
//---------------------------------------------------------------------------
#include "uRs232c.h"
#include "Timer.h"


class CRs232LoadCl
{
    public:
        //Constructor
        CRs232LoadCl (void);
        ~CRs232LoadCl (void);

        void Init();
        void Close();

        enum EN_LOADCELL_CMD{
            lcNone     = 0,
            lcSetCheck = 1,
            lcSetHold  = 2,
            lcAutoZero = 3,

            MAX_LOADCELL_CMD
        };

    private:

    protected:
        TRS232C *m_pRS232C;
        double m_dLoadCellData ; // Measurement

        void RcvLoadClData  (DWORD _cbInQue);

        String m_sReadMsg;
        String m_sRcvMsg;
        int  m_iSeqCycle;
        int  m_iPreSeqCycle;

        EN_LOADCELL_CMD m_iCmdNo;

    public:
        bool GetMsgEnd();
        String GetMsg();

        bool WeightCheck();
        bool SetHold    (bool _bOnOff = true);
        bool AutoZero   ();

        double GetLoadCellData();

        bool Update(void);
};

#endif
