//---------------------------------------------------------------------------

#ifndef FormOperationH
#define FormOperationH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "Timer.h"
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
        TPanel *pnIONSignal;
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
        TPanel *pnWtrDrain;
        TPanel *pnWtrPMP;
        TPanel *pnWtrBNR;
        TPanel *pnIONSol;
        TPanel *pnPMPSol;
        TPanel *pnBNRSol;
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
        TPanel *pnTiltMotor;
        TLabel *Label11;
        TPanel *pnWorkName;
        TButton *Button1;
        TTimer *Timer1;
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
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmOperation(TComponent* Owner);
        CDelayTimer m_tmTemp       ;


        short m_lInputXIo[128];
        short m_lOutputXIo[128];
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
#endif
