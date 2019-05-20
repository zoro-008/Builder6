//---------------------------------------------------------------------------
#ifndef Rs232UVCuringH
#define Rs232UVCuringH
//---------------------------------------------------------------------------
#include "uRs232c.h"
//#include "Timer.h"

// UV Curing
enum EN_UV_CH {
    uvCh1 = 1,
    uvCh2 = 2,
    uvCh3 = 3,
    uvCh4 = 4,
    uvCh5 = 5,
};


class CRs232UV // 키엔스 높이 측정기
{
    public:
         CRs232UV (void);
        ~CRs232UV (void);
        void Init();
        void Close();

        enum EN_UV_CMD{
            ucNone     = 0,
            ucUVCuring = 1,
            ucUVOnOff  = 2,
            ucUVData   = 3,
            MAX_HEIGHT_CMD
        };

    private:

    protected:
        TRS232C * m_pRS232C;

        AnsiString m_sErrMsg  ;

        bool m_bErrFlg; // Error Flag

        String m_sReadMsg ; // 컨트롤러에서 받아온 메세지 담는 변수
        String m_sRcvMsg ; // 마스터 창에서 실시간으로 메세지 확인하기 위한 변수
        String m_sCuringData ; // 광량 셋팅 값 저장한 변수
        String m_sCuringOnOffData ; // 온/오프 송신한 문자열로써 조건 비교하기 위해 선언
        String m_sSendCuringData ; // 메세지 상태를 Get 하기 위해 송신 문자열 담는 변수
        String m_sData ; // GetData() 함수의 멤버 변수

        EN_UV_CMD m_iCmdNo ;

        void ReceiveUVMsg(DWORD _cbInQue);


        void LoadSave(bool _bLoad) ;
        //TopUV Time Check용.
        bool m_bTopUvOnOff;
        bool m_bBtmUvOnOff;

        double m_dTopUvUseTime_ms ;
        double m_dBtmUvUseTime_ms ;





    public:
        void   SetData(String _sMsg);

        String GetErrMsg();
        String GetMsg();
        String GetData();

        void SetCuring(EN_UV_CH _Ch, int _dLight);
        void SetCuringOnOff(EN_UV_CH _Ch, bool _bOnOff = true);
        void SendCuringData(EN_UV_CH _Ch);
        bool MsgReset(); // 모든 메세지 리셋
        bool GetErrFlg();
        bool GetMsgEnd();

        void Update();

        double GetTopUvUseTime_ms(); void ClearTopUvUseTime();
        double GetBtmUvUseTime_ms(); void ClearBtmUvUseTime();

};

extern CRs232UV Rs232UV ;
//---------------------------------------------------------------------------
#endif


