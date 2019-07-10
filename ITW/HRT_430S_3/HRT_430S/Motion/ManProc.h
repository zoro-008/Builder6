//---------------------------------------------------------------------------
#ifndef ManProcH
#define ManProcH

//---------------------------------------------------------------------------
#include "UtilDefine.h"
#include "Timer.h"


/***************************************************************************/
/* Constants                                                               */
/***************************************************************************/
//...
//===========================================================================


/***************************************************************************/
/* Structures & Variables                                                  */
/***************************************************************************/
//...
//===========================================================================


/***************************************************************************/
/* TClass                                                                  */
/***************************************************************************/
//---------------------------------------------------------------------------
class TManProc
{
    private:   /* Member Var.             */
        //Object.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        CDelayTimer m_MotrOnTimer ;
        CDelayTimer m_MotrOffTimer;
        CDelayTimer m_ActrOnTimer ;
        CDelayTimer m_ActrOffTimer;
        CDelayTimer m_TempTimer   ;

        //Internal Vars.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        bool       m_bRptMotr               ; //For Repeat Functions.
        bool       m_bDirMotr               ;
        bool       m_bRptActr               ;
        bool       m_bDirActr               ;
        EN_MOTR_ID m_iRMotrID               ;
        EN_ACTR_ID m_iRActrID               ;

        int        m_iManNo                 ;
        int        m_iPrevManNo             ;
        bool       m_bLtProcOn              ;

    public:    /* Direct Accessable Var.  */
        //Property.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        __property int   _iManNo        = { read = m_iManNo    };
        __property bool  _bLtProcOn     = { read = m_bLtProcOn };

    public:    /* Direct Accessable Func. */
        //持失切 & 社瑚切. (Constructor & Destructor)
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        __fastcall  TManProc(void);
        __fastcall ~TManProc(void);

        //Init.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        void __fastcall Init (void);
        void __fastcall Reset(void) { Init(); }

        //Master Functions.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        void __fastcall SetRptMotr (EN_MOTR_ID Motr , bool Flag ) { m_iRMotrID      = Motr; m_bRptMotr = Flag; m_bDirMotr  = false;                                                                       }
        void __fastcall SetRptActr (EN_ACTR_ID Actr , bool Flag ) { m_iRActrID      = Actr; m_bRptActr = Flag; m_bDirActr  = false;                                                                       }

        //User Defined Functions.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        bool __fastcall RptMotr (void);
        bool __fastcall RptActr (void);

        //Manual Processing.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        void __fastcall ManProcOn  (int No , bool OnKey               );
        bool __fastcall ManCycleRun(void                              );
};

//---------------------------------------------------------------------------
extern TManProc MAN;

//---------------------------------------------------------------------------
#endif
