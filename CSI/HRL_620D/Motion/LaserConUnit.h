//---------------------------------------------------------------------------

#ifndef LaserConUnitH
#define LaserConUnitH

#include <ExtCtrls.hpp>

#define MAX_LASER_ERR 400

#include "Timer.h"
#include "uRs232c.h"

enum EN_LASER_ERRCODE {
    leNoErr         = 0 ,
    leGetSettingErr     ,//1. 레이져 설정 상태 가져오기 에러.
    leSetSettingErr     ,//2. 레이져 설정 에러.
    leReadyErr          ,//3. 준비 상태 체크 에러.
    leWorkErr           ,//4. 레이저 인쇄 작업중 에러.
    leCheckSumErr       ,//5. 레이져 체크 섬 데이터가 틀림 에러.
    MAX_LASER_ERRCODE
};

// 레이져의 블럭은 잡파일이라 생각하면됌.
enum EN_LASER_MSG_ID {
    lmNone = 0 ,
    lmGA   = 1 , //block Change.
    lmRE   = 2 , //Ready
    lmTX   = 3 , //조사.
    MAX_LASER_MSG_ID

};

class CLaserConUnit
{
    public:
        __fastcall  CLaserConUnit(void);
        __fastcall ~CLaserConUnit(void);
        void Init      ();
        void Close     ();
        void Reset     ();

    private:
        void   SetErrName (); 
        AnsiString m_sErrName[MAX_LASER_ERR];

        CDelayTimer      m_tmCycle   ; //타임아웃용.

        EN_LASER_ERRCODE m_iLastErrCode ;

        EN_LASER_MSG_ID  m_iLaserMsgId ; //메세지 보내고 있는 것.

        AnsiString       m_sRcvMsg ; //LF CR까지 다 들어온 풀메세지.

        int              m_iGABlockNo ; //GA보낼때 쓸 아이디




        char        HexToDec (String     _sData);
        bool        ErrCheck (String _ErrCheck );
        bool        CheckSumCheck(String _sMsg);

        bool        bErrCode;
        bool        SendErrCode();

        bool        CycleTX  ();
        bool        CycleGA  ();
        //bool        CycleRE  ();
    public:
        bool         CycleRE  ();
        int          m_iLaserStep , m_iPreLaserStep ;

        AnsiString  GetChksum(AnsiString _sData);

        int         GetErrCode();
        AnsiString  GetErrName(int _iErrNo);
        TRS232C *   Rs232_L;

        //rs232 callback
        static void ReceiveMsg(DWORD _cbInQue);

        bool        SendMsg(AnsiString _sMsg);
        bool        m_bWrkLaser;

        //블럭 체인지 끝났는지 확인 하는것.
        //스테이지 작업 종료시에 블럭 체인지명령만 보내고
        //응답을 기다리지 않는다.. 블럭 파일에 따라 5초도 걸려서 이렇게 함.
        EN_LASER_MSG_ID GetLaserMsgId();

        void        Update(); //메세지 보내는 싸이클 돌려줌.
        bool        SetGABlockId(int _iBlockNo);
        bool        SetSendMsgId(EN_LASER_MSG_ID _iMsgId); //메세지 보내는 것..
        bool        GetMsgId(); //현재 보내고 있는 메세지 아이디.

        bool        GetGAFinished(); //블럭체인지 확인.
        int         GetBlockNoID (); //블럭 넘버 가져오기.

};
//---------------------------------------------------------------------------
extern CLaserConUnit LCU;
//---------------------------------------------------------------------------
#endif
