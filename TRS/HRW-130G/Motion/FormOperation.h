//---------------------------------------------------------------------------

#ifndef FormOperationH
#define FormOperationH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmOperation : public TForm
{
__published:	// IDE-managed Components
        TTimer *tmUpdate;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TPanel *btStart;
        TPanel *btStop;
        TPanel *btEmg;
        TPanel *btReset;
        TPanel *Panel6;
        TPanel *Panel7;
        TPanel *Panel8;
        TPanel *Panel9;
        TPanel *Panel10;
        TPanel *Panel11;
        TPanel *Panel12;
        TPanel *pnCleanSignal;
        TPanel *Panel13;
        TPanel *pnAirSignal;
        TPanel *Panel14;
        TPanel *pnDrySignal;
        TPanel *Panel15;
        TPanel *Panel16;
        TPanel *pnCleanRPM;
        TPanel *Panel17;
        TPanel *pnAirRPM;
        TPanel *Panel18;
        TPanel *pnDryRPM;
        TPanel *Panel19;
        TPanel *Panel20;
        TPanel *pnCleanTime;
        TPanel *Panel21;
        TPanel *pnAirTime;
        TPanel *Panel22;
        TPanel *pnDryTime;
        TPanel *Panel39;
        TPanel *pnVaccumOn;
        TPanel *pnWtrDrain;
        TPanel *pnWtrSupply;
        TPanel *pnAir2Sol;
        TPanel *pnAir1Sol;
        TPanel *pnTiltMotor;
        TPanel *pnSpinMotor;
        TPanel *Panel2;
        TPanel *Panel3;
        TPanel *pnTiltCmdPos;
        TPanel *Panel4;
        TPanel *Panel5;
        TPanel *pnSpinSpeed;
        TLabel *lbModelName;
        TTimer *tmAxisStatus;
        TLabel *Label3;
        TPanel *pnDoorUp;
        TLabel *Label12;
        TPanel *pnDoorDn;
        TLabel *Label13;
        TPanel *pnDeviceNo;
        TLabel *lbRemote;
        TLabel *lbHoming;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall btStartMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btStartMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall tmAxisStatusTimer(TObject *Sender);
        void __fastcall btResetClick(TObject *Sender);
        void __fastcall btStartClick(TObject *Sender);
        void __fastcall lbModelNameDblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmOperation(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
#endif
