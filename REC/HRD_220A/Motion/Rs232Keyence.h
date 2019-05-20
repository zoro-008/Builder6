//---------------------------------------------------------------------------
#ifndef Rs232KeyenceH
#define Rs232KeyenceH
//---------------------------------------------------------------------------
#include "uRs232c.h"
//#include "Timer.h"


#define KEYENCE_ERR 999
// Keyence 
enum EN_KEC_CH {
    kcBoth = 0 ,
    kcChA  = 1 ,
    kcChB  = 2 ,
};

class CRs232Keyence // 키엔스 높이 측정기
{
    public:
         CRs232Keyence (void);
        ~CRs232Keyence (void);
        void Init();
        void Close();

        enum EN_HEIGHT_CMD{
            hcNone   = 0,
            hcZero   = 1,
            hcHeight = 2,
            MAX_HEIGHT_CMD
        };

    private:

    protected:
        TRS232C * m_pRS232C;
        double m_dHeightData1 ; // Measurement
        double m_dHeightData2 ; // Measurement

        int m_iSeqCycle    ;
        int m_iPreSeqCycle ;

        String m_sReadMsg;
        String m_sRcvMsg ;

        EN_HEIGHT_CMD m_iCmdNo;

        void ReceiveHgtSnrMsg(DWORD _cbInQue);

    public:
        bool GetMsgEnd();
        String GetMsg();

        void SetZero  (EN_KEC_CH _Ch, bool _bOnOff = true);
        void CheckHeight(EN_KEC_CH _Ch);

        double GetHeightData(EN_KEC_CH _iCh);

        void Update();
};

extern CRs232Keyence Rs232Keyence ;
//---------------------------------------------------------------------------
#endif


