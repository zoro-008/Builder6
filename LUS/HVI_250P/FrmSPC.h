//---------------------------------------------------------------------------

#ifndef FrmSPCH
#define FrmSPCH
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
//---------------------------------------------------------------------------
#include "DataMan.h"
//---------------------------------------------------------------------------
class TFormSPC : public TForm
{
__published:	// IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TTabSheet *TabSheet3;
    TTabSheet *TabSheet4;
    TPanel *Panel7;
    TPanel *Panel8;
    TImage *Image4;
    TLabel *Label4;
    TStringGrid *sgErrHistory;
    TLabel *Label5;
    TLabel *Label6;
    TButton *btErrDataView;
    TDateTimePicker *edFDataTime;
    TDateTimePicker *edUDataTime;
    TPanel *Panel9;
    TPanel *Panel10;
    TImage *Image5;
    TLabel *Label7;
    TStringGrid *sgErrCnt;
    TLabel *Label8;
    TLabel *Label9;
    TButton *btErrCntView;
    TDateTimePicker *edF_ErrCnt;
    TDateTimePicker *edU_ErrCnt;
    TDateTimePicker *edMtbaDT;
    TButton *btMtbaView;
    TChart *ChatMtba;
    TBarSeries *S1_Mtba;
    TButton *btMtbaSave;
    TButton *btErrCntSave;
    TButton *btErrDataSave;
    TTabSheet *TabSheet5;
    TPanel *Panel11;
    TPanel *Panel12;
    TImage *Image6;
    TLabel *Label12;
    TStringGrid *sgLotDayName;
    TPanel *Panel13;
    TPanel *Panel14;
    TImage *Image7;
    TLabel *Label13;
    TStringGrid *sgLotDayInfo;
    TLabel *Label14;
    TLabel *Label15;
    TButton *btLotDayView;
    TDateTimePicker *edF_LotDayInfo;
    TDateTimePicker *edU_LotDayInfo;
    TButton *btLotDaySave;
    TLabel *Label16;
    TLabel *Label10;
    TLabel *Label11;
    TPanel *Panel2;
    TPanel *Panel3;
    TImage *Image1;
    TLabel *Label1;
    TStringGrid *sgDate;
    TPanel *Panel1;
    TPanel *Panel4;
    TImage *Image2;
    TLabel *Label2;
    TStringGrid *sgLot;
    TPanel *Panel5;
    TPanel *Panel6;
    TImage *Image3;
    TLabel *Label3;
    TStringGrid *sgLotInfo;
    TButton *btLotInfoView;
    TDateTimePicker *edF_LotInfo;
    TDateTimePicker *edU_LotInfo;
    TButton *btLotInfoSave;
    TPanel *Panel19;
    TPanel *Panel20;
    TImage *Image10;
    TLabel *Label20;
    TStringGrid *sgSlotNo;
    TPanel *Panel21;
    TPanel *Panel22;
    TImage *Image11;
    TLabel *Label21;
    TPanel *pnLotMap;
    TPanel *Panel15;
    TPanel *Panel16;
    TImage *Image8;
    TLabel *Label17;
    TStringGrid *sgMgzNo;
    TPanel *Panel17;
    TPanel *Panel18;
    TImage *Image9;
    TLabel *Label19;
        TLabel *lbFailName1;
        TLabel *lbFailName2;
        TLabel *lbFailName3;
        TLabel *lbFailName4;
        TLabel *lbFailName5;
        TLabel *lbFailName6;
        TLabel *lbFailName7;
        TLabel *lbFailName8;
        TLabel *lbFailName9;
        TLabel *lbFailName10;
        TLabel *lbFailName11;
        TLabel *lbFailName12;
        TLabel *lbFailName13;
        TLabel *lbFailName14;
        TLabel *lbFailName15;
        TLabel *Label44;
        TLabel *Label45;
        TLabel *Label46;
        TLabel *Label47;
        TLabel *Label48;
        TLabel *Label50;
        TLabel *Label51;
        TLabel *Label52;
        TLabel *Label53;
        TLabel *Label54;
        TLabel *Label55;
        TLabel *Label56;
        TLabel *Label57;
        TLabel *Label58;
        TLabel *Label59;
        TLabel *Label18;
        TEdit *edRslt1;
        TEdit *edRslt2;
        TEdit *edRslt3;
        TEdit *edRslt4;
        TEdit *edRslt5;
        TEdit *edRslt6;
        TEdit *edRslt7;
        TEdit *edRslt8;
        TEdit *edRslt9;
        TEdit *edRslt10;
        TEdit *edRslt11;
        TEdit *edRslt12;
        TEdit *edRslt13;
        TEdit *edRslt14;
        TEdit *edFail;
        TEdit *edTotal;
        TTimer *tmUpdate;
        TLabel *Label22;
        TLabel *Label23;
        TEdit *Edit1;
        TLabel *Label24;
        TLabel *Label25;
        TEdit *Edit2;
        TLabel *Label26;
        TLabel *Label27;
        TEdit *Edit3;
        TLabel *Label28;
        TLabel *Label29;
        TEdit *Edit4;
        TLabel *Label30;
        TLabel *Label31;
        TEdit *Edit5;
        TLabel *Label32;
        TLabel *Label33;
        TEdit *Edit6;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall sgDateClick(TObject *Sender);
    void __fastcall sgLotClick(TObject *Sender);
    void __fastcall btLotInfoViewClick(TObject *Sender);
    void __fastcall btLotInfoSaveClick(TObject *Sender);
    void __fastcall sgLotDayNameClick(TObject *Sender);
    void __fastcall sgSlotNoClick(TObject *Sender);
    void __fastcall sgMgzNoClick(TObject *Sender);
    void __fastcall sgLotInfoClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormSPC(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TFormSPC *FormSPC;
//---------------------------------------------------------------------------
#endif
