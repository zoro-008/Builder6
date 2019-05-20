//---------------------------------------------------------------------------

#ifndef LaserConUnitH
#define LaserConUnitH

#include <ExtCtrls.hpp>

#define MAX_LASER_ERR 400

#include "Timer.h"
#include "uRs232c.h"

enum EN_LASER_ERRCODE {
    leNoErr         = 0 ,
    leGetSettingErr     ,//1. ������ ���� ���� �������� ����.
    leSetSettingErr     ,//2. ������ ���� ����.
    leReadyErr          ,//3. �غ� ���� üũ ����.
    leWorkErr           ,//4. ������ �μ� �۾��� ����.
    leCheckSumErr       ,//5. ������ üũ �� �����Ͱ� Ʋ�� ����.
    MAX_LASER_ERRCODE
};

// �������� ���� �������̶� �����ϸ��.
enum EN_LASER_MSG_ID {
    lmNone = 0 ,
    lmGA   = 1 , //block Change.
    lmRE   = 2 , //Ready
    lmTX   = 3 , //����.
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

        CDelayTimer      m_tmCycle   ; //Ÿ�Ӿƿ���.

        EN_LASER_ERRCODE m_iLastErrCode ;

        EN_LASER_MSG_ID  m_iLaserMsgId ; //�޼��� ������ �ִ� ��.

        AnsiString       m_sRcvMsg ; //LF CR���� �� ���� Ǯ�޼���.

        int              m_iGABlockNo ; //GA������ �� ���̵�




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

        //�� ü���� �������� Ȯ�� �ϴ°�.
        //�������� �۾� ����ÿ� �� ü������ɸ� ������
        //������ ��ٸ��� �ʴ´�.. �� ���Ͽ� ���� 5�ʵ� �ɷ��� �̷��� ��.
        EN_LASER_MSG_ID GetLaserMsgId();

        void        Update(); //�޼��� ������ ����Ŭ ������.
        bool        SetGABlockId(int _iBlockNo);
        bool        SetSendMsgId(EN_LASER_MSG_ID _iMsgId); //�޼��� ������ ��..
        bool        GetMsgId(); //���� ������ �ִ� �޼��� ���̵�.

        bool        GetGAFinished(); //��ü���� Ȯ��.
        int         GetBlockNoID (); //�� �ѹ� ��������.

};
//---------------------------------------------------------------------------
extern CLaserConUnit LCU;
//---------------------------------------------------------------------------
#endif
