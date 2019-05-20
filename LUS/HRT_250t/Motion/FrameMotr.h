//---------------------------------------------------------------------------


#ifndef FrameMotrH
#define FrameMotrH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
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
        TImageList *ImgLstBt;
        TLabel *Label81;
        TLabel *Label82;
        TLabel *LbCmdPos;
        TLabel *LbEncPos;
        TLabel *LbStat2;
        TLabel *LbStat3;
        TLabel *LbStat1;
        TLabel *LbStat4;
        TLabel *LbStat5;
        TLabel *LbStat6;
        TLabel *LbStat7;
        TBitBtn *btLeft;
        TBitBtn *btRigh;

        void __fastcall btBwMove(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y) ;
        void __fastcall btFwMove(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y) ;
        void __fastcall btMtStop(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y) ;

        void __fastcall tmUpdateTimer(TObject *Sender);
private:	// User declarations
        //Grid & Event.
        Graphics::TBitmap *m_pFwimg ;
        Graphics::TBitmap *m_pBwimg ;
        Graphics::TBitmap *m_pUpimg ;
        Graphics::TBitmap *m_pDnimg ;
        Graphics::TBitmap *m_pLlimg ; // Lotation Left
        Graphics::TBitmap *m_pLrimg ; // Lotation Right

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
