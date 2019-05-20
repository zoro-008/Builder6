//---------------------------------------------------------------------------

#ifndef FormValveH
#define FormValveH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "Timer.h"
//---------------------------------------------------------------------------
class TFrmValve : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnMain;
        TLabel *Label8;
        TLabel *Label9;
        TPanel *btDoorUp;
        TTimer *tmIOStatus;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *lbModelName;
        TPanel *btBNRAir;
        TPanel *btIONAir;
        TPanel *btWtBNR;
        TPanel *btWtPMP;
        TPanel *btWtDrain;
        TLabel *Label6;
        TPanel *btDoorDn;
        TPanel *btPMPAir;
        TLabel *Label1;
        TLabel *Label7;
        TPanel *btMskFwd;
        TPanel *btMskBwd;
        TLabel *Label10;
        TLabel *Label11;
        void __fastcall btDoorUpClick(TObject *Sender);
        void __fastcall tmIOStatusTimer(TObject *Sender);
        void __fastcall btDoorUpMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btDoorUpMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmValve(TComponent* Owner);
        bool Temp;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmValve *FrmValve;
//---------------------------------------------------------------------------
#endif
