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
    TLabel *Label18;
    TPanel *Panel18;
    TImage *Image9;
    TLabel *Label19;
    TEdit *edTotal;
        TStringGrid *sgLotMap;
        TTimer *tmUpdate;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TButton *Button1;
        TLabel *Label23;
        TEdit *edTemp6;
        TLabel *Label22;
        TEdit *edBin0;
        TLabel *Label24;
        TEdit *edBin1;
        TLabel *Label25;
        TEdit *edBin2;
        TLabel *Label26;
        TEdit *edBin3;
        TLabel *Label27;
        TEdit *edBin4;
        TLabel *Label28;
        TEdit *edBin5;
        TLabel *Label29;
        TEdit *edBin6;
        TLabel *Label30;
        TEdit *edBin7;
        TLabel *Label31;
        TEdit *edBin8;
        TLabel *Label32;
        TEdit *edBin9;
        void __fastcall FormShow(TObject *Sender);
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
    void __fastcall sgSlotNoClick(TObject *Sender);
    void __fastcall sgMgzNoClick(TObject *Sender);
    void __fastcall sgLotInfoClick(TObject *Sender);
        void __fastcall sgLotMapDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmSPC(TComponent* Owner);

    void __fastcall UpdateComOptn(bool _bToTable);

    int  m_iBin0Cnt;
    int  m_iBin1Cnt;
    int  m_iBin2Cnt;
    int  m_iBin3Cnt;
    int  m_iBin4Cnt;
    int  m_iBin5Cnt;
    int  m_iBin6Cnt;
    int  m_iBin7Cnt;
    int  m_iBin8Cnt;
    int  m_iBin9Cnt;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmSPC *FrmSPC;
//---------------------------------------------------------------------------
#endif
