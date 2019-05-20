//---------------------------------------------------------------------------
#ifndef MotrCtrManH
#define MotrCtrManH
//---------------------------------------------------------------------------
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>

#include "UtilDefine.h"
#include "TEdit.h"

//#define MAX_TAB_MOTR 4
#define MAX_LABEL    7

enum EN_MOTRTAB_ID {
    mtiLDR    =0  ,    //Max 4 Motor
    mtiRAIL       ,    //
    mtiPROBE      ,    //

    MAX_MOTRTAB
};

enum EN_MOTRCTR_ID {
    tmMOTOR0  =0  ,    //Max 4 Motor
    tmMOTOR1      ,    //
    tmMOTOR2      ,    //
    tmMOTOR3      ,    //
    tmMOTOR4      ,    //
    tmMOTOR5      ,    //
    tmMOTOR6      ,    //

    MAX_MOTRCTR
};

enum EN_MOTRDISP_TYPE {
    mdtBackFor_X = 0 ,    //  BackWard_X , ForWard_X
    mdtForBack_X     ,    //  ForWard_X  , BackWard_X
    mdtDownUp        ,    //  Down       , Up
    mdtUpDown        ,    //  Up         , Down
    mdtBackFor_Y     ,    //  BackWard_Y , ForWard_Y
    mdtForBack_Y     ,    //  ForWard_Y  , BackWard_Y

    MAX_MOTRDISP_TYPE
};

class CMotrCtrMan
{
    public :
        CMotrCtrMan();
        ~CMotrCtrMan();

    protected :
        struct SValTab {
            AnsiString    sName  ;
            int           iMtCnt ;
        };
        struct SValMtr {
            AnsiString       sName  ;
            EN_MOTRDISP_TYPE iType  ;
            int              iMtId  ;
        };

        TTimer * tmUpdate ;
        void __fastcall tmUpdateTimer(TObject *Sender);

        SValTab  ValTab[MAX_MOTRTAB] ;
        SValMtr  ValMtr[MAX_MOTRCTR] ;
        AnsiString m_sTabName[MAX_MOTRTAB]  ;
        int        m_iMtId   [MAX_MOTR]     ;
        int        m_iMtList                ;

        //Grid & Event.
        Graphics::TBitmap *m_pFwimage ;
        Graphics::TBitmap *m_pBwimage ;
        Graphics::TBitmap *m_pUpimage ;
        Graphics::TBitmap *m_pDnimage ;

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
        TPageControl *m_pPageControl;
        TTabSheet    *m_pTabSheet[MAX_MOTRTAB];

        //Edit & Event.
        void __fastcall btMtBw(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btMtFw(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btMtUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

    public :
        void SetWindow(TPanel * _pnBase , TImageList *_ImageList );
        void SetTabIndex(int iIndex);

};
extern CMotrCtrMan MCM;
#endif
