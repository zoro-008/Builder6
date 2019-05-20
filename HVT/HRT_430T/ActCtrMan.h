//---------------------------------------------------------------------------
#ifndef ACTCtrManH
#define ACTCtrManH
//---------------------------------------------------------------------------
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>

#include "UtilDefine.h"
//#include "MotrCtrMan.h"
#include "TEdit.h"

//#define MAX_TAB_MOTR 4
#define MAX_LABEL    7

enum EN_ACTTAB_ID {
    ctiLDR    =0  ,    //Max 4 Motor
    ctiRAIL       ,    //
    ctiSORT       ,    //
    ctiULD        ,    //
//    ctiPROBE      ,    //

    MAX_ACTTAB
};

enum EN_ACTCTR_ID {
    tmACT0  =0  ,    //
    tmACT1      ,    //
    tmACT2      ,    //
    tmACT3      ,    //
    tmACT4      ,    //
    tmACT5      ,    //
    tmACT6      ,    //
    tmACT7      ,    //
    tmACT8      ,    //
    tmACT9      ,    //
    tmACT10     ,    //
    tmACT11     ,    //
    tmACT12     ,    //
    tmACT13     ,    //
    tmACT14     ,    //
    tmACT15     ,    //

    MAX_ACTCTR
};

//enum EN_ACMOTRCTR_ID {
//    tmACMOTR0  =0  ,    //
//    tmACMOTR1      ,    //
//    tmACMOTR2      ,    //
//    tmACMOTR3      ,    //
//    tmACMOTR4      ,    //
//
//    MAX_ACMOTRCTR
//};


enum EN_ACTDISP_TYPE {
    cdtBackFor = 0 ,    //  BackWard , ForWard
    cdtForBack     ,    //  ForWard  , BackWard
    cdtDownUp      ,    //  Down     , Up
    cdtUpDown      ,    //  Up       , Down
    cdtOnOff       ,    //  Up       , Down

    MAX_ACTDISP_TYPE
};

class CActCtrMan
{
    public :
        CActCtrMan();
        ~CActCtrMan();

    protected :
        struct SValTab {
            AnsiString    sName   ;
            int           iACTCnt ;
        };
        struct SValACT {
            AnsiString      sName   ;
            EN_ACTDISP_TYPE iType   ;
            int             iACTId  ;
            int             iRow    ;
            int             iCol    ;
        };

        TTimer * tmUpdate ;
        void __fastcall tmUpdateTimer(TObject *Sender);

        SValTab  ValTab[MAX_ACTTAB] ;
        SValACT  ValACT[MAX_ACTCTR] ;
        AnsiString m_sTabName[MAX_ACTTAB]  ;

        int        m_iACTId  [MAX_ACTCTR]  ;
        int        m_iMtList               ;

        //Grid & Event.
        Graphics::TBitmap *m_pFwimage ;
        Graphics::TBitmap *m_pBwimage ;
        Graphics::TBitmap *m_pUpimage ;
        Graphics::TBitmap *m_pDnimage ;

        TStaticText  *m_pStaticText[MAX_ACTCTR];
        TBitBtn      *m_pBwButton  [MAX_ACTCTR];
        TBitBtn      *m_pFwButton  [MAX_ACTCTR];

        TTabSheet    *m_pTabSheet  [MAX_ACTTAB];
        TPageControl *m_pPageControl;

        TPanel       *m_pPanel     [MAX_ACTCTR];


/*
        TRadioGroup  *m_pJogRadioGroup;

        TRadioGroup  *m_pMtRadioGroup[MAX_MOTR];
        TLabel       *m_pLbStat[MAX_MOTR][MAX_LABEL];

        TLabel       *m_pLbCmdCaption[MAX_MOTR];
        TLabel       *m_pLbEncCaption[MAX_MOTR];

        TLabel       *m_pLbCmdPos[MAX_MOTR];
        TLabel       *m_pLbEncPos[MAX_MOTR];
        TBitBtn      *m_pBwButton[MAX_MOTR];
        TBitBtn      *m_pFwButton[MAX_MOTR];

        TEdit        *m_pEdit;

*/

        //Edit & Event.
//        void __fastcall btMtBw(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
//        void __fastcall btMtFw(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
//        void __fastcall btMtUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
          void __fastcall btFwClick(TObject *Sender);
          void __fastcall btBwClick(TObject *Sender);

          void __fastcall btOnClick (TObject *Sender);
          void __fastcall btOffClick(TObject *Sender);
    public :
        void SetWindow(TPanel * _pnBase , TImageList *_ImageList );
        void SetTabIndex(int iIndex);

};
extern CActCtrMan ACM;
#endif
