//---------------------------------------------------------------------------


#ifndef FrameCylOneBtH
#define FrameCylOneBtH
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



class TFraCylOneBt : public TFrame
{
__published:	// IDE-managed Components
        TBitBtn *btAction;
        TTimer *tmUpdate;
        TImageList *imDirection;
        TStaticText *stTitle;
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall btActionClick(TObject *Sender);
        void __fastcall FrameResize(TObject *Sender);
private:	// User declarations
        EN_ACTR_ID         m_iActrId ;
        EN_ACTR_DIRECTION  m_iType   ;
        CheckSafeCallback  m_pCheckSafeCallback ;
        //bool               m_bPreDone ;
        bool               m_bPreCmd ;

        Graphics::TBitmap * m_pBtImgFw ;
        Graphics::TBitmap * m_pBtImgBw ;

        AnsiString         m_sCaptionFw ;
        AnsiString         m_sCaptionBw ;

        //bool               m_bSetSize ; //������ ��������� ���ָ� �Ǽ� ��...


public:		// User declarations
        __fastcall TFraCylOneBt(TComponent* Owner);
        //__fastcall  TFraCylOneBt(void);

        void SetSize();
        void SetConfig(EN_ACTR_ID        _iActrId           ,
                       AnsiString        _sTitle            ,
                       EN_ACTR_DIRECTION _iActrType         ,
                       TWinControl *     _wcParent          ,
                       CheckSafeCallback _pCheckSafeCallback);
};
//---------------------------------------------------------------------------
extern PACKAGE TFraCylOneBt *FraCylOneBt;
//---------------------------------------------------------------------------
#endif
