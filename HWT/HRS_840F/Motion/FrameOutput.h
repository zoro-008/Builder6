//---------------------------------------------------------------------------


#ifndef FrameOutputH
#define FrameOutputH
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
#include <ImgList.hpp>
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
//bool CLoader::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
typedef bool (__closure*CheckSafeCallback)(EN_ACTR_ID _iActr , bool _bFwd);



class TFraOutput : public TFrame
{
__published:	// IDE-managed Components
        TBitBtn *btAction;
        TTimer *tmUpdate;
        TStaticText *stTitle;
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall btActionClick(TObject *Sender);
private:	// User declarations
        EN_OUTPUT_ID m_iYadd    ;
        bool         m_bPreDone ;
        bool         m_bSetSize ;

public:		// User declarations
        //__fastcall TFrameCyl(TComponent* Owner);
        __fastcall  TFraOutput(void);
        void SetSize();
        void SetConfig(EN_OUTPUT_ID   _iYadd    ,
                       AnsiString     _sTitle   ,
                       TWinControl *  _wcParent );
};
//---------------------------------------------------------------------------
extern PACKAGE TFraOutput *FraOutput;
//---------------------------------------------------------------------------
#endif
