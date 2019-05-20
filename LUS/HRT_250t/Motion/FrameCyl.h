//---------------------------------------------------------------------------


#ifndef FrameCylH
#define FrameCylH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
#include "UtilDefine.h"
#include "SMDllDefine.h"
#include "FrameMotr.h"
#include <ExtCtrls.hpp>
/*
enum EN_CYL_DISP_TYPE {
    cdtBwFw_X = 0 , // Ex Bw 는 좌측 Fw 는 우측으로 이동.
    cdtFwBw_X     , //
    cdtBwFw_Y     , // 좌표계 따라서...
    cdtFwBw_Y     , // Bw 좌측 밑 반시계 다운
    cdtBwFw_Z     , // Fw 우측 위 시계 업
    cdtFwBw_Z     , //
    cdtBwFw_L     , //
    cdtFwBw_L     , //

//    adLR = 0 , //정면에서   봤을때 Left 가 - Right가 +
//    adRL     , //정면에서   봤을때 Right가 - Left 가 +
//    adBF     , //정면에서   봤을때 Bwd  가 - Fwd  가 +
//    adFB     , //정면에서   봤을때 Fwd  가 - Bwd  가 +
//    adUD     , //정면에서   봤을때 Up   가 - Down 가 +
//    adDU     , //정면에서   봤을때 Down 가 - Up   가 +
//    adCA     , //회전축에서 봤을때 Clock가 - AntiC가 +
//    adAC       //회전축에서 봤을때 AntiC가 - Clock가 +
//
    MAX_CYL_DISP_TYPE
};
*/
class TFraCyl : public TFrame
{
__published:	// IDE-managed Components
        TBitBtn *bt1_Up;
        TBitBtn *bt2_Dn;
        TBitBtn *bt1_Left;
        TBitBtn *bt2_Right;
        TBitBtn *bt1_RLeft;
        TBitBtn *bt2_RRight;
        TBitBtn *bt1_Forward;
        TBitBtn *bt2_Backward;
        TTimer *tmUpdate;
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall btClick (TObject *Sender) ;
private:	// User declarations
        int m_iId   ;
        int m_iType ;
        bool BitBtnVisible(bool _bBool);
        bool CheckSafe(int _iId , bool _bFwd);

public:		// User declarations
//        __fastcall TFrameCyl(TComponent* Owner);
        __fastcall  TFraCyl(void);
        bool SetIdType(EN_ACTR_ID _iId , EN_ACTR_DIRECTION _iType );
};
//---------------------------------------------------------------------------
extern PACKAGE TFraCyl *FraCyl;
//---------------------------------------------------------------------------
#endif
