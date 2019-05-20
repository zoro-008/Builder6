//---------------------------------------------------------------------------

#ifndef Rs232DisprH
#define Rs232DisprH
//---------------------------------------------------------------------------
#include "uRs232c.h"
#include "Timer.h"
#include "UtilDefine.h"


class CRs232Dispr
{
    public:
        //Constructor
        CRs232Dispr (void);
        ~CRs232Dispr (void);

        void Init(EN_RS232 _iId);
        void Close();

        enum EN_DISP_CMD{
            dcNone        = 0,
            dcDisprOnOff  = 1,
            dcSigmaModeOn = 2,
            dcSigmaModeOff= 3,
            dcMode        = 4,
            dcLoadCh      = 5,
            dcPTV         = 6,
            dcSyringeSize = 7,
            //����ÿ��� �޾ƿ��� �͵�.
            dcSylFill     = 8, //���� �ܷ�  �޾ƿ���.
            dcDispData    = 9, //����ÿ� ���ð� �޾ƿ���.


            MAX_DISP_CMD
        };

        struct TDispData {
            double dPrsPres   ;
            double dVacPres   ;
            bool   bSigmaMode ;
        };

        int m_iSylFill   ; //���� �ܷ�. 0~100



    private:

    protected:
        EN_RS232 m_iId ;

        CDelayTimer m_tmDipsr ;

        AnsiString m_sAskMsg ;
        AnsiString m_sReadMsg;
        AnsiString m_sSendMsg;
        AnsiString m_sErrMsg ;

        EN_DISP_CMD m_iCmdNo;

        TDispData DispData ; //����ÿ� ���õǾ� �ִµ����͸� �޾ƿ°�.

        int m_iSeqCycle   ;
        int m_iPreSeqCycle;

        TRS232C * m_pRS232C;
        bool m_bDisprOn;

        AnsiString GetChksum(AnsiString sData);
        void RcvDisprData   (DWORD _cbInQue);

        AnsiString GetSendMsg(AnsiString _sMsg);



    public:
        AnsiString GetErrMsg();
        bool       GetMsgEnd();

        TDispData  GetDispData();
        int        GetSylFill ();

        //��� �Լ�.
        bool SetDisprOnOff  (bool _bOnOff);
        bool SetSigmaModeOn (            ); //3������ �ϴ� ������ �����ϸ鼭 �ñ׸� ��忡 ����.
        bool SetSigmaModeOff(int _iCh    );
        bool SetMode        (bool _bManual); //true:Manual false:Timed
        bool SetLoadCh      (int _iCh    );
        bool SetPTV         (double _dPres , int _iTime, double _dVac);
        bool SetSyringeSize (int _iSize  );
        bool GetSylVolm     (int _iCh    ); //�ܾ�üũ
        bool GetDispData    (int _iCh    ); //���⼼�ð� �޾ƿ���.

        bool Update(void);

        //void Load(bool _bLoad);
};

extern CRs232Dispr   Rs232_DisprFt;
extern CRs232Dispr   Rs232_DisprRr;
#endif
