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
#include "SMDllDefine.h"
//---------------------------------------------------------------------------
class TFrmSPC : public TForm
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
    TLabel *Label18;
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
    TPanel *Panel15;
    TLabel *Label24;
    TLabel *Label25;
    TLabel *Label26;
    TLabel *Label27;
    TLabel *Label28;
    TLabel *Label29;
    TLabel *Label30;
    TLabel *Label31;
    TLabel *Label32;
    TLabel *Label36;
    TLabel *Label37;
    TLabel *Label40;
    TLabel *Label41;
    TLabel *Label42;
    TLabel *Label43;
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
    TLabel *Label17;
    TPanel *Panel16;
    TImage *Image8;
    TLabel *Label16;
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
    TPanel *Panel17;
    TPanel *Panel18;
    TImage *Image9;
    TLabel *Label19;
    TStringGrid *sgBuffer;
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
        void __fastcall FormShow(TObject *Sender);
        void __fastcall Label2Click(TObject *Sender);
        void __fastcall sgDateClick(TObject *Sender);
    void __fastcall sgLotClick(TObject *Sender);
    void __fastcall btErrDataViewClick(TObject *Sender);
    void __fastcall btErrCntViewClick(TObject *Sender);
    void __fastcall btMtbaViewClick(TObject *Sender);
    void __fastcall btMtbaSaveClick(TObject *Sender);
    void __fastcall btErrCntSaveClick(TObject *Sender);
    void __fastcall btErrDataSaveClick(TObject *Sender);
    void __fastcall btLotInfoViewClick(TObject *Sender);
    void __fastcall btLotInfoSaveClick(TObject *Sender);
    void __fastcall sgLotDayNameClick(TObject *Sender);
    void __fastcall btLotDayViewClick(TObject *Sender);
    void __fastcall btLotDaySaveClick(TObject *Sender);
        void __fastcall sgLotInfoClick(TObject *Sender);
    void __fastcall sgBufferClick(TObject *Sender);
    void __fastcall sgSlotNoClick(TObject *Sender);
private:	// User declarations
    bool __fastcall LanguageSel(EN_LAN_SEL lvType);
public:		// User declarations
    __fastcall TFrmSPC(TComponent* Owner);

    void __fastcall UpdateComOptn(bool _bToTable);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmSPC *FrmSPC;
//---------------------------------------------------------------------------
#endif
