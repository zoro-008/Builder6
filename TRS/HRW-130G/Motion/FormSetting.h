//---------------------------------------------------------------------------

#ifndef FormSettingH
#define FormSettingH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmSetting : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnMain;
        TPanel *btStart;
        TPanel *btStop;
        TPanel *btEmg;
        TPanel *Panel6;
        TPanel *Panel7;
        TPanel *Panel8;
        TPanel *Panel9;
        TPanel *Panel10;
        TPanel *Panel15;
        TPanel *Panel16;
        TPanel *pnCleanRpm;
        TPanel *Panel17;
        TPanel *pnAirRpm;
        TPanel *Panel18;
        TPanel *pnDryRpm;
        TPanel *Panel19;
        TPanel *Panel20;
        TPanel *pnCleanTime;
        TPanel *Panel21;
        TPanel *pnAirTime;
        TPanel *Panel22;
        TPanel *pnDryTime;
        TPanel *pnButton;
        TButton *bt1;
        TButton *bt2;
        TButton *bt3;
        TButton *bt4;
        TButton *bt5;
        TButton *bt6;
        TButton *bt7;
        TButton *bt8;
        TButton *bt9;
        TButton *btClear;
        TButton *bt0;
        TButton *btBack;
        TButton *btOK;
        TEdit *edCleanRpm;
        TEdit *edAirRpm;
        TEdit *edDryRpm;
        TEdit *edCleanTime;
        TEdit *edAirTime;
        TEdit *edDryTime;
        TLabel *Label8;
        TLabel *lbModelName;
        void __fastcall btStartMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btStartMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btOKClick(TObject *Sender);
        void __fastcall bt1Click(TObject *Sender);
        void __fastcall btClearClick(TObject *Sender);
        void __fastcall btBackClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btEmgClick(TObject *Sender);
        void __fastcall pnCleanRpmClick(TObject *Sender);
        void __fastcall btStartClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmSetting(TComponent* Owner);
        int iEditNo;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmSetting *FrmSetting;
//---------------------------------------------------------------------------
#endif
