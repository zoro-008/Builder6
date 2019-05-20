//---------------------------------------------------------------------------
#ifndef SequenceH
#define SequenceH

//---------------------------------------------------------------------------
#include "Timer.h"
#include "WorkTable.h"
#define MAX_PART 2
#define MAX_TIME 10

class TSequence
{
    private:   /* Member Var. */
        //Timer.
        CDelayTimer m_FlickOnTimer    ;
        CDelayTimer m_FlickOffTimer   ;
        CDelayTimer m_StopBtTimer     ;

        //Vars.
        bool         m_bFlick   ;
        EN_SEQ_STAT  m_iSeqStat ;




    public:    /* Direct Accessable Var.  */

        TWorkTable   LT ;
        TWorkTable   RT ;

        //Var.
        DWORD dwUD_Strt[MAX_TIME]; //Scan Time
        DWORD dwUP_Scan[MAX_TIME];

    public:    /* Direct Accessable Func. */
        __fastcall  TSequence(void);
        __fastcall ~TSequence(void);

        //Init.
        void __fastcall Init (void);
        void __fastcall Reset(void);

        void __fastcall SetOptn(void);

        //Functions.
        bool  __fastcall InspectMainAir   (void );
        bool  __fastcall InspectEmergency (void );

            //Real Time Update.
        void  __fastcall Update          (void);
        void  __fastcall UpdateSeqState  (void);

        EN_SEQ_STAT GetStat(){return m_iSeqStat;}

        void __fastcall ReqHome() ;
};

//---------------------------------------------------------------------------
extern TSequence SEQ;

//---------------------------------------------------------------------------
#endif


