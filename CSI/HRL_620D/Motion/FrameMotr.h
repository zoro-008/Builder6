//---------------------------------------------------------------------------


#ifndef FrameMotrH
#define FrameMotrH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
#include "UtilDefine.h"
#include "SMDllDefine.h"

//---------------------------------------------------------------------------
/*
enum EN_MOTR_DISP_TYPE {
    mdtCCwCw_X = 0 , // Left CCw , Right Cw
    mdtCwCCw_X     , //
    mdtCCwCw_Y     , //
    mdtCwCCw_Y     , //
    mdtCCwCw_Z     , //
    mdtCwCCw_Z     , //
    mdtCCwCw_L     , // Cw Time direction
    mdtCwCCw_L     ,

    MAX_MOTR_DISP_TYPE
};
*/
enum EN_UNIT_TYPE{
    utJog   = 0 , //
    utMove      , //
    utPitch     ,

    MAX_UNIT_TYPE
};

class TFraMotr : public TFrame
{
__published:	// IDE-managed Components
        TTimer *tmUpdate;
        TLabel *Label81;
        TLabel *Label82;
        TLabel *lbCmdPos;
        TLabel *lbEncPos;
        TLabel *LbStat2;
        TLabel *LbStat3;
        TLabel *LbStat1;
        TLabel *LbStat4;
        TLabel *LbStat5;
        TLabel *LbStat6;
        TLabel *LbStat7;

        //void __fastcall btBwMove(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y) ;
        //void __fastcall btFwMove(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y) ;

        void __fastcall tmUpdateTimer(TObject *Sender);
private:	// User declarations
        //Grid & Event.
        Graphics::TBitmap *m_pLtimg  ;
        Graphics::TBitmap *m_pRtimg  ;
        Graphics::TBitmap *m_pUpimg  ;
        Graphics::TBitmap *m_pDnimg  ;
        Graphics::TBitmap *m_pClimg  ; //Clock
        Graphics::TBitmap *m_pAcimg  ; //AntiClock Clock

        int    m_iId       ;
        int    m_iType     ;

        double dUnit       ;
        int    iUnitType   ;

        double dPitch      ;

        bool   CheckSafe(int _iId);

public:		// User declarations
//        __fastcall TFrameMotr(TComponent* Owner);
        __fastcall TFraMotr(void);

        bool SetIdType (EN_MOTR_ID _iId , EN_MTOR_DIRECTION _iType );
        void SetUnit   (EN_UNIT_TYPE _iUnitType , double _dUnit);
        void SetPitch  (double _dUnit);
};
//---------------------------------------------------------------------------
extern PACKAGE TFraMotr *FraMotr;
//---------------------------------------------------------------------------
#endif
