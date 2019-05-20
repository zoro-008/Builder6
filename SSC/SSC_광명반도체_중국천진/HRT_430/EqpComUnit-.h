//---------------------------------------------------------------------------
#ifndef EqpComUnitH
#define EqpComUnitH

#include "UtilDefine.h"
#include "Timer.h"

class CEqpComUnit
{
    private:
        struct TStep {
           int iStartX        ;
           int iStartY        ;
           int iStopX         ;
           int iStopY         ;
           int iResetX        ;
           int iResetY        ;
           int iLotStartX     ;
           int iLotStartY     ;
           int iLotEnd        ;
           int iModelChangeX  ;
           int iModelChangeY  ;
           int iDataX          ;
           int iDataY          ;
        } Step , PreStep;

        CDelayTimer m_tmReset       ;
        CDelayTimer m_tmLotStart    ;
        CDelayTimer m_tmModelChange ;

        void __fastcall DelPastLog();
        bool __fastcall WriteLogVisnErr(AnsiString _sLog);
        bool __fastcall WriteLogVisnMap(EN_ARAY_ID  riId);

        void ThreadStartOutput        ();
        void ThreadStartInput         ();
        void ThreadStopOutput         ();
        void ThreadStopInput          ();
        void ThreadResetOutput        ();
        void ThreadResetInput         ();
        void ThreadLotStartOutput     ();
        void ThreadLotStartInput      ();
        void ThreadLotEnd             ();
        void ThreadModelChangeOutput  ();
        void ThreadModelChangeInput   ();
        void ThreadDataOutput         ();
        void ThreadDataInput          ();

    public:
        CEqpComUnit(void);
        ~CEqpComUnit(void);
        void __fastcall Init(void);
        //Network Drive Connect
        bool __fastcall NetDrvConnect(AnsiString sDrv , AnsiString sIp, AnsiString sPath);
        bool __fastcall NetCancelConnection(AnsiString sDrv);

        //Eqip Communication.
        bool __fastcall ReadPreEqip  (EN_ARAY_ID riId);
        bool __fastcall WritePostEqip(EN_ARAY_ID riId);

        //Lot Data Communication.
        bool __fastcall ReadLotData (void);
        bool __fastcall WriteLotData(void); //랏 오픈시에

        //Device Change Data Communication.
        bool __fastcall WriteChange (void); //디바이스 체인지 시에.
        bool __fastcall ReadChange  (void);

        bool SendStart      (bool _bUseThread = false);
        bool SendReset      (bool _bUseThread = false);
        bool SendLotStart   (bool _bUseThread = false);
        bool SendModelChange(bool _bUseThread = false);

        bool __fastcall WriteVisnModel();
        bool __fastcall WriteVisnLot  ();



};

extern CEqpComUnit EC;

template <typename T>
class CRunThread
{
    class TRunThread : public TThread
    {
        private:
            T Func;

        public:
            __fastcall TRunThread(T func) : TThread(false)
            {
                FreeOnTerminate = true;
                Func = func;
            }
            void __fastcall Execute()
            {
                Func();
            }
    };

    public:
        CRunThread(T func)
        {
            TRunThread*  thread = new TRunThread(func);
        }
};

/*  ** 사용법**
typedef void (__closure *Func)();
void __fastcall TForm1::Button3Click(TObject *Sender)
{
    Button3->Enabled = false ;
    CRunThread<Func> * func = new CRunThread<Func>(&TestFunc) ;
}

void TForm1::TestFunc()
{ 
    // 엄청 시간이 걸리는 루틴.
    int  k; 
    for(int c = 0; c < 299999999; c++)
    {
        k = c; 
        k++; 
        c = k; 
        c--; 
    } 
    Caption = k;
    Button3->Enabled = true;
}

*/

#endif
