//---------------------------------------------------------------------------

#ifndef FormTrainChkH
#define FormTrainChkH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include <Graphics.hpp>
#include <Buttons.hpp>

#include "UnitImage.h"
#include "UnitArea.h"
//---------------------------------------------------------------------------
class TFrmTrainChk : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnDraw;
    TPanel *Panel1;
    TRadioGroup *rgErsrVal;
    TTimer *tmUpdate;
    TSpeedButton *btCursor;
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
    CImage * m_pTrainImg  ;
    CArea  * m_pTrainArea ;


public:		// User declarations
        __fastcall TFrmTrainChk(TComponent* Owner);

    int m_iPointX , m_iPointY ;

    void SetTrainImg (CImage * _pImg ) ;
    void SetTrainArea(CArea  * _pArea) ;
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
