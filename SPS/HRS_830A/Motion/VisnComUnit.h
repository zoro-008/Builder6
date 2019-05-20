//---------------------------------------------------------------------------
#ifndef VisnComUnitH
#define VisnComUnitH

#include "UtilDefine.h"
#include "Timer.h"
#include "DataMan.h"
enum EN_VISN_RESET_SEL
{
    vrOri = 0,
    vrLsr    ,
    vrInp    ,
    vrBoth   ,
    MAX_VISN_RESET_SEL

};

class CVisnComUnit
{
    private:
        int m_iVsErrCnt[MAX_CHIP_STAT] ; int m_iVsTtlErrCnt ;

        CDelayTimer m_tmReset       ;
        CDelayTimer m_tmLotStart    ;
        CDelayTimer m_tmModelChange ;

        void __fastcall DelPastLog();
        bool __fastcall WriteLogVisnErr(AnsiString _sLog);

        void ThreadReset      ();
        //void ThreadReset2      ();
        //void ThreadReset3      ();

        void ThreadLotStart   ();
        void ThreadModelChange();

        EN_VISN_RESET_SEL iVisnResetSel;
    public:
        CVisnComUnit(void);
        ~CVisnComUnit(void);
        void Init();
        void Close();
        //void __fastcall Init(void);

        GetLastErrCnt   (EN_CHIP_STAT _csStat);
        GetLastTtlErrCnt(                    );

        ClearErrCnt     ();

        //Network Drive Connect
        bool __fastcall NetDrvConnect(AnsiString _sDrv , AnsiString _sIp, AnsiString _sPath);

        //Vision Communication.
        EN_CHIP_STAT GetHighLevel(EN_CHIP_STAT _iOri , EN_CHIP_STAT _iNew);
        bool __fastcall ReadResult  (int iVisnNo , EN_ARAY_ID riId);
        int  __fastcall SimpleReadResult(int iVisnNo, CArray * Aray = NULL);

        bool SendReset      (EN_VISN_RESET_SEL _iSel = vrBoth , bool _bUseThread = false);
        bool SendLotStart   (bool _bUseThread = false);
        bool SendModelChange(bool _bUseThread = false);

        bool __fastcall WriteVisnModel();
        bool __fastcall WriteVisnLot  ();
        bool __fastcall WriteLogMap(EN_ARAY_ID  riId );

};
extern CVisnComUnit VC;


#endif
