//---------------------------------------------------------------------------

#ifndef FormSPCH
#define FormSPCH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//#include "TEdit.h"
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <Mask.hpp>
#include <Chart.hpp>
#include <DbChart.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <Series.hpp>
#include <ValEdit.hpp>
//---------------------------------------------------------------------------
class TFrmSPC : public TForm
{
__published:	// IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TTabSheet *TabSheet4;
    TPanel *Panel7;
    TPanel *Panel8;
        TStringGrid *sgErrData;
    TLabel *Label5;
    TLabel *Label6;
    TButton *btErrDataView;
        TDateTimePicker *edErrSttTime;
        TDateTimePicker *edErrEndTime;
    TDateTimePicker *edMtbaDT;
    TButton *btMtbaView;
    TChart *ChatMtba;
    TBarSeries *S1_Mtba;
    TButton *btMtbaSave;
    TLabel *Label16;
    TPanel *Panel2;
    TPanel *Panel3;
        TStringGrid *sgLotDate;
    TPanel *Panel5;
    TPanel *Panel6;
    TStringGrid *sgLotInfo;
    TPanel *Panel19;
    TPanel *Panel20;
        TStringGrid *sgLotSlot;
    TPanel *Panel21;
    TPanel *Panel22;
    TPanel *pnLotMap;
        TStringGrid *sgLotMap;
        TTimer *tmUpdate;
        TRadioGroup *rgNoTime;
    TPanel *Panel15;
    TPanel *Panel16;
    TStringGrid *sgLotMgz;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall sgLotDateClick(TObject *Sender);
    void __fastcall btErrDataViewClick(TObject *Sender);
    void __fastcall btErrCntViewClick(TObject *Sender);
    void __fastcall sgLotSlotClick(TObject *Sender);
        void __fastcall sgLotMapDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall sgLotInfoClick(TObject *Sender);
        void __fastcall sgLotMgzClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmSPC(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmSPC *FrmSPC;
//---------------------------------------------------------------------------
#endif
