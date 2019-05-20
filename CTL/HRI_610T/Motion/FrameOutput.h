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
