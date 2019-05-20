//---------------------------------------------------------------------------


#ifndef FlmMotrH
#define FlmMotrH
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
//---------------------------------------------------------------------------
enum EN_DISP_TYPE {
    dtCCwCw_X = 0 , // Left CCw , Right Cw
    dtCwCCw_X     , //
    dtCCwCw_Y     , //
    dtCwCCw_Y     , //
    dtCCwCw_Z     , //
    dtCwCCw_Z     , //
    dtCCwCw_L     , // Cw Time direction
    dtCwCCw_L     ,

    MAX_DISP_TYPE
};

class TFrameMotr : public TFrame
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TLabel *Label2;
        TLabel *LbCmdPos;
        TLabel *LbEncPos;
        TLabel *Label5;
        TLabel *LbStat1;
        TLabel *LbStat2;
        TLabel *LbStat3;
        TLabel *LbStat4;
        TLabel *LbStat5;
        TLabel *LbStat6;
        TLabel *LbStat7;
        TUpDown *UpDown;
        TEdit *edMtIncPos;
        TBitBtn *btStop;
        TBitBtn *btBwdMove;
        TBitBtn *btFwdMove;
        TBitBtn *btBwdJog;
        TBitBtn *btFwdJog;
        TTimer *tmUpdate;
        TImageList *ImgLstBt;

        void __fastcall btBwMoveClick(TObject *Sender);
        void __fastcall btFwMoveClick(TObject *Sender);
        void __fastcall btMtStop     (TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y) ;

        void __fastcall btBwJogDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y) ;
        void __fastcall btFwJogDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y) ;
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall UpDownChangingEx(TObject *Sender,
          bool &AllowChange, short NewValue, TUpDownDirection Direction);
        void __fastcall btStopClick(TObject *Sender);
private:	// User declarations
        //Grid & Event.
        Graphics::TBitmap *m_pFwimg ;
        Graphics::TBitmap *m_pBwimg ;
        Graphics::TBitmap *m_pUpimg ;
        Graphics::TBitmap *m_pDnimg ;
        Graphics::TBitmap *m_pLlimg ; // Lotation Left
        Graphics::TBitmap *m_pLrimg ; // Lotation Right

        int  m_iId ;
        int  m_iType    ;
        bool CheckSafe(int _iId);

public:		// User declarations
//        __fastcall TFrameMotr(TComponent* Owner);
        __fastcall TFrameMotr(void);

        bool SetIdType (EN_MOTR_ID _iId , EN_DISP_TYPE _iType );
};
//---------------------------------------------------------------------------
extern PACKAGE TFrameMotr *FrameMotr;
//---------------------------------------------------------------------------
#endif
