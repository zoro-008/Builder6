//---------------------------------------------------------------------------
#ifndef VisnComUnitH
#define VisnComUnitH

#include "UtilDefine.h"
#include "Timer.h"
#include "DataMan.h"
enum EN_VISN
{
    vsVisn1_1 = 0,
    vsVisn1_2    ,
    vsVisn2_1    ,
    vsVisn2_2    ,
    vsVisn3_1    ,
    vsVisn3_2    ,
    MAX_VISN

};

class CVisnComUnit
{
    private:
        //struct TStep {
        //   int iReset       ;
        //   int iLotStart    ;
        //   int iModelChange ;
        //} Step , PreStep;

        //int m_iVs1ErrCnt[MAX_CHIP_STAT] ; int m_iVs1TtlErrCnt ;
        //int m_iVs2ErrCnt[MAX_CHIP_STAT] ; int m_iVs2TtlErrCnt ;
        //int m_iVs3ErrCnt[MAX_CHIP_STAT] ; int m_iVs3TtlErrCnt ;
        //int m_iVs4ErrCnt[MAX_CHIP_STAT] ; int m_iVs4TtlErrCnt ;

        CDelayTimer m_tmReset       ;
        CDelayTimer m_tmLotStart    ;
        CDelayTimer m_tmModelChange ;

        void __fastcall DelPastLog();
        bool __fastcall WriteLogVisnErr(AnsiString _sLog);

        void ThreadReset      (int _iVisn);
        void ThreadLotStart   (int _iVisn);
        void ThreadModelChange(int _iVisn);
        //void ThreadReset2      ();
        //void ThreadReset3      ();
        //void ThreadReset4      ();
        //


    public:
        CVisnComUnit(void);
        ~CVisnComUnit(void);
        void __fastcall Init(void);

        //GetLastErrCnt   (int _iVisnNo , EN_CHIP_STAT _csStat);
        //GetLastTtlErrCnt(int _iVisnNo                       );

        //ClearErrCnt     ();

        //Network Drive Connect
        //bool __fastcall NetDrvConnect(AnsiString _sDrv , AnsiString _sIp, AnsiString _sPath);

        //Vision Communication.
        EN_CHIP_STAT GetHighLevel(EN_CHIP_STAT _iOri , EN_CHIP_STAT _iNew);

        //실제 WK존 데이터 받아오기용.
        bool __fastcall ReadResult  (EN_VISN iVisnNo , EN_ARAY_ID riId , int _iColOffset = 0);

        //dp용 버퍼 데이터 받아오기용.
        int  __fastcall SimpleReadResult(EN_VISN iVisnNo, CArray * Aray = NULL , int _iColOffset = 0);

        bool SendReset      (EN_VISN _iSel );
        bool SendLotStart   (EN_VISN _iSel );
        bool SendModelChange(EN_VISN _iSel );

        bool __fastcall WriteVisnModel();
        bool __fastcall WriteVisnLot  ();
        bool __fastcall WriteLogMap(EN_ARAY_ID  riId );

        bool __fastcall WriteArayVisnFormat(CArray * riAray);

};
extern CVisnComUnit VC;


#endif
