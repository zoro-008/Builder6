//---------------------------------------------------------------------------
#ifndef VisnComUnitH
#define VisnComUnitH

#include "UtilDefine.h"
#include "Timer.h"
#include "DataMan.h"
enum EN_VISN_RESET_SEL
{
    vsVisn1 = 0,
    vsVisn2    ,
    vsVisn3    ,
    vsVisn4    ,
    vrBoth     ,
    MAX_VISN_RESET_SEL

};

class CVisnComUnit
{
    private:
        //struct TStep {
        //   int iReset       ;
        //   int iLotStart    ;
        //   int iModelChange ;
        //} Step , PreStep;

        int m_iVs1ErrCnt[MAX_CHIP_STAT] ; int m_iVs1TtlErrCnt ;
        int m_iVs2ErrCnt[MAX_CHIP_STAT] ; int m_iVs2TtlErrCnt ;
        int m_iVs3ErrCnt[MAX_CHIP_STAT] ; int m_iVs3TtlErrCnt ;
        int m_iVs4ErrCnt[MAX_CHIP_STAT] ; int m_iVs4TtlErrCnt ;

        CDelayTimer m_tmReset       ;
        CDelayTimer m_tmLotStart    ;
        CDelayTimer m_tmModelChange ;

        void __fastcall DelPastLog();
        bool __fastcall WriteLogVisnErr(AnsiString _sLog);

        void ThreadReset1      ();
        void ThreadReset2      ();
        void ThreadReset3      ();
        void ThreadReset4      ();

        void ThreadLotStart   ();
        void ThreadModelChange();
    public:
        CVisnComUnit(void);
        ~CVisnComUnit(void);
        void __fastcall Init(void);

        GetLastErrCnt   (int _iVisnNo , EN_CHIP_STAT _csStat);
        GetLastTtlErrCnt(int _iVisnNo                       );

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
