//---------------------------------------------------------------------------


#ifndef FlmCylH
#define FlmCylH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
#include "UtilDefine.h"
#include "FrameMotr.h"
#include <ExtCtrls.hpp>
class TFrameCyl : public TFrame
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
        bool CheckSafe(int _iId);
                        
public:		// User declarations
//        __fastcall TFrameCyl(TComponent* Owner);
        __fastcall TFrameCyl(void);
        bool SetIdType(EN_ACTR_ID _iId , EN_DISP_TYPE _iType );
};
//---------------------------------------------------------------------------
extern PACKAGE TFrameCyl *FrameCyl;
//---------------------------------------------------------------------------
#endif
