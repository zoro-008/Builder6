//---------------------------------------------------------------------------

#ifndef FormValveH
#define FormValveH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmValve : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnMain;
        TLabel *Label8;
        TLabel *Label9;
        TPanel *btDoor;
        TLabel *Label1;
        TTimer *tmIOStatus;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *lbModelName;
        TPanel *btVacm;
        TPanel *btAir1;
        TPanel *btAir2;
        TPanel *btWtSply;
        TPanel *btWtDrain;
        void __fastcall btDoorClick(TObject *Sender);
        void __fastcall tmIOStatusTimer(TObject *Sender);
        void __fastcall btDoorMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btDoorMouseUp(TObject *Sender,
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
