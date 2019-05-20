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
            //무사시에서 받아오는 것들.
            dcSylFill     = 8, //현재 잔량  받아오기.
            dcDispData    = 9, //무사시에 세팅값 받아오기.


            MAX_DISP_CMD
        };

        struct TDispData {
            double dPrsPres   ;
            double dVacPres   ;
            bool   bSigmaMode ;
        };

        int m_iSylFill   ; //현재 잔량. 0~100



    private:

    protected:
        EN_RS232 m_iId ;

        CDelayTimer m_tmDipsr ;

        AnsiString m_sAskMsg ;
        AnsiString m_sReadMsg;
        AnsiString m_sSendMsg;
        AnsiString m_sErrMsg ;

        EN_DISP_CMD m_iCmdNo;

        TDispData DispData ; //무사시에 세팅되어 있는데이터를 받아온값.

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

        //통신 함수.
        bool SetDisprOnOff  (bool _bOnOff);
        bool SetSigmaModeOn (            ); //3번토출 하는 동작을 수행하면서 시그마 모드에 진입.
        bool SetSigmaModeOff(int _iCh    );
        bool SetMode        (bool _bManual); //true:Manual false:Timed
        bool SetLoadCh      (int _iCh    );
        bool SetPTV         (double _dPres , int _iTime, double _dVac);
        bool SetSyringeSize (int _iSize  );
        bool GetSylVolm     (int _iCh    ); //잔양체크
        bool GetDispData    (int _iCh    ); //토출세팅값 받아오기.

        bool Update(void);

        //void Load(bool _bLoad);
};

extern CRs232Dispr   Rs232_DisprFt;
extern CRs232Dispr   Rs232_DisprRr;
#endif
