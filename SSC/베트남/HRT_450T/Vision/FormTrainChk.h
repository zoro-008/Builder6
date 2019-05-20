//---------------------------------------------------------------------------

#ifndef FormTrainChkH
#define FormTrainChkH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "SVInterface.h"
#include "VisionBase.h"
#include <Graphics.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFrmTrainChk : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnDraw;
    TPanel *Panel1;
    TRadioGroup *rgErsrVal;
    TTimer *tmUpdate;
    TSpeedButton *btCursor;
    TSpeedButton *btSpoite;
    TSpeedButton *btPencleP;
    TSpeedButton *btPencleY;
    TSpeedButton *btEraser;
    TPanel *pnValue;
        TPanel *Panel2;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
    void __fastcall pnDrawMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall pnDrawMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    IImage * m_pTrainImg  ;
    IArea  * m_pTrainArea ;
    EN_VISN_ID m_iVisnId ;


public:		// User declarations
        __fastcall TFrmTrainChk(TComponent* Owner);

    int m_iPointX , m_iPointY ;

    void SetTrainImg (IImage * _pImg ) ;
    void SetTrainArea(IArea  * _pArea) ;
    bool SerchEdge   (int _iX , int _iY , int _iValue);

        //화면 그리는 것 ....
    TWndMethod OriginalProc;
    void __fastcall PanelProc(Messages::TMessage &msg);

    void OnPaint();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmTrainChk *FrmTrainChk;
//---------------------------------------------------------------------------
#endif
