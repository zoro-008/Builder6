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
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TFrmTrainChk : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnDraw;
    TPanel *Panel1;
    TCheckBox *cbEraser;
    TImage *Image1;
    TEdit *edExpand;
    TImage *Image2;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
    void __fastcall cbEraserClick(TObject *Sender);
    void __fastcall pnDrawMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall pnDrawMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall Image2Click(TObject *Sender);
        void __fastcall Image1Click(TObject *Sender);
private:	// User declarations
    IImage * m_pTrainImg  ;
    IArea  * m_pTrainArea ;

    //Main Thread에서만 호출해야됌.
    void OnPaint();



public:		// User declarations
        __fastcall TFrmTrainChk(TComponent* Owner);

    void SetTrainImg (IImage * _pImg ) ;
    void SetTrainArea(IArea  * _pArea) ;

        //화면 그리는 것 ....
    TWndMethod OriginalProc;
    void __fastcall PanelProc(Messages::TMessage &msg);



};
//---------------------------------------------------------------------------
extern PACKAGE TFrmTrainChk *FrmTrainChk;
//---------------------------------------------------------------------------
#endif
