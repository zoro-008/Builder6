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
    cdtBwFw_X = 0 , // Ex Bw �� ���� Fw �� �������� �̵�.
    cdtFwBw_X     , //
    cdtBwFw_Y     , // ��ǥ�� ����...
    cdtFwBw_Y     , // Bw ���� �� �ݽð� �ٿ�
    cdtBwFw_Z     , // Fw ���� �� �ð� ��
    cdtFwBw_Z     , //
    cdtBwFw_L     , //
    cdtFwBw_L     , //

//    adLR = 0 , //���鿡��   ������ Left �� - Right�� +
//    adRL     , //���鿡��   ������ Right�� - Left �� +
//    adBF     , //���鿡��   ������ Bwd  �� - Fwd  �� +
//    adFB     , //���鿡��   ������ Fwd  �� - Bwd  �� +
//    adUD     , //���鿡��   ������ Up   �� - Down �� +
//    adDU     , //���鿡��   ������ Down �� - Up   �� +
//    adCA     , //ȸ���࿡�� ������ Clock�� - AntiC�� +
//    adAC       //ȸ���࿡�� ������ AntiC�� - Clock�� +
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
