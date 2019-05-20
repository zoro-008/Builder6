//---------------------------------------------------------------------------

#ifndef FormProbeH
#define FormProbeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
//---------------------------------------------------------------------------
class TFrmProbe : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TImage *Image2;
    TLabel *lbDate;
    TLabel *lbStat;
    TLabel *Label6;
    TLabel *lbDevice;
    TLabel *lbName;
    TPanel *Panel3;
    TTimer *tmUpdate;
        TPageControl *pcData;
    TTabSheet *TabSheet1;
    TLabel *Label1;
    TLabel *Label3;
    TLabel *Label4;
    TStaticText *lbColPitch;
    TStaticText *lbVfAvr;
    TStringGrid *sgProbeCur;
    TStaticText *StaticText2;
    TStaticText *lbVfMin;
    TStaticText *StaticText4;
    TStaticText *lbVfMax;
    TStaticText *StaticText6;
    TStaticText *lbVrAvr;
    TStaticText *StaticText8;
    TStaticText *lbVrMin;
    TStaticText *StaticText10;
    TStaticText *lbVrMax;
    TEdit *edPb1ChData;
    TEdit *edPb2ChData;
    TOpenDialog *OpenDialog1;
        TPanel *Panel4;
        TPanel *Panel5;
        TPanel *Panel6;
        TPanel *Panel7;
        TPanel *Panel8;
        TTabSheet *TabSheet2;
        TPanel *pnGraph;
        TChart *Chart1;
        TFastLineSeries *Series1;
        TFastLineSeries *Series2;
        TCheckBox *CheckBox1;
        TFastLineSeries *Series3;
        TFastLineSeries *Series4;
        TFastLineSeries *Series5;
        TFastLineSeries *Series6;
        TFastLineSeries *Series7;
        TFastLineSeries *Series8;
        TFastLineSeries *Series9;
        TFastLineSeries *Series10;
        TFastLineSeries *Series11;
        TFastLineSeries *Series12;
        TFastLineSeries *Series13;
        TFastLineSeries *Series14;
        TFastLineSeries *Series15;
        TFastLineSeries *Series16;
        TFastLineSeries *Series17;
        TFastLineSeries *Series18;
        TFastLineSeries *Series19;
        TFastLineSeries *Series20;
        TFastLineSeries *Series21;
        TFastLineSeries *Series22;
        TFastLineSeries *Series23;
        TFastLineSeries *Series24;
        TFastLineSeries *Series25;
        TFastLineSeries *Series26;
        TFastLineSeries *Series27;
        TFastLineSeries *Series28;
        TFastLineSeries *Series29;
        TFastLineSeries *Series30;
        TFastLineSeries *Series31;
        TFastLineSeries *Series32;
        TCheckBox *CheckBox2;
        TCheckBox *CheckBox3;
        TCheckBox *CheckBox4;
        TCheckBox *CheckBox5;
        TCheckBox *CheckBox6;
        TCheckBox *CheckBox7;
        TCheckBox *CheckBox8;
        TCheckBox *CheckBox9;
        TCheckBox *CheckBox10;
        TCheckBox *CheckBox11;
        TCheckBox *CheckBox12;
        TCheckBox *CheckBox13;
        TCheckBox *CheckBox14;
        TCheckBox *CheckBox15;
        TCheckBox *CheckBox16;
        TCheckBox *CheckBox17;
        TCheckBox *CheckBox18;
        TCheckBox *CheckBox19;
        TCheckBox *CheckBox20;
        TCheckBox *CheckBox21;
        TCheckBox *CheckBox22;
        TCheckBox *CheckBox23;
        TCheckBox *CheckBox24;
        TCheckBox *CheckBox25;
        TCheckBox *CheckBox26;
        TCheckBox *CheckBox27;
        TCheckBox *CheckBox28;
        TCheckBox *CheckBox29;
        TCheckBox *CheckBox30;
        TCheckBox *CheckBox31;
        TCheckBox *CheckBox32;
    TSaveDialog *SaveDialog1;
        TPanel *pnInspSkip;
    TTabSheet *TabSheet3;
    TStringGrid *sgProbePast;
    TBitBtn *btPastSaveAs;
    TLabel *Label2;
    TStaticText *StaticText1;
    TStaticText *lbVfAvr2;
    TStaticText *StaticText5;
    TStaticText *lbVfMin2;
    TStaticText *StaticText9;
    TStaticText *lbVfMax2;
    TStaticText *StaticText12;
    TStaticText *lbVrAvr2;
    TStaticText *StaticText14;
    TStaticText *lbVrMin2;
    TStaticText *StaticText16;
    TStaticText *lbVrMax2;
    TPanel *Panel9;
    TPanel *Panel10;
    TPanel *Panel11;
    TPanel *Panel12;
    TPanel *Panel13;
    TLabel *Label5;
    TEdit *edPb1ChData2;
    TLabel *Label7;
    TEdit *edPb2ChData2;
        TPanel *Panel14;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall sgProbeCurDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
        void __fastcall sgProbePastDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall btPastSaveAsClick(TObject *Sender);
        void __fastcall sgProbeCurClick(TObject *Sender);
        void __fastcall sgProbePastClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

public:		// User declarations
    //Vars.
    struct SStat {
        double VfMin;
        double VfMax;
        double VfAvr;
        double VfSum;
        int    VfCnt;
        double VrMin;
        double VrMax;
        double VrAvr;
        double VrSum;
        int    VrCnt;
    };

private:	// User declarations
    SStat Stat ;

public:		// User declarations
    __fastcall TFrmProbe(TComponent* Owner);
    void __fastcall Init(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmProbe *FrmProbe;
//---------------------------------------------------------------------------
#endif
