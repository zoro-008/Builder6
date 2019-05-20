//---------------------------------------------------------------------------
#ifndef FormOperationH
#define FormOperationH
//---------------------------------------------------------------------------
//#include "SortCmn.h"
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
class TFrmOperation : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel4;
    TPanel *Panel5;
    TPanel *Panel6;
    TPanel *Panel7;
    TBitBtn *btStart;
    TBitBtn *btStop;
    TBitBtn *btReset;
    TBitBtn *btOperator;
    TPanel *plPassWord;
    TPanel *Panel19;
    TLabel *Label17;
    TBitBtn *btPasswordClose;
    TBitBtn *btEngr;
    TBitBtn *btOper;
    TBitBtn *btMast;
    TPanel *Panel8;
    TPanel *Panel9;
    TPanel *Panel10;
    TPanel *Panel11;
    TPanel *Panel14;
    TPanel *Panel15;
    TListBox *lbErr;
    TPanel *Panel16;
    TPanel *Panel18;
    TBitBtn *btLotOpen;
    TBitBtn *btLotEnd;
    TLabel *Label1;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label31;
    TLabel *Label32;
    TPanel *pnDayRunTime;
    TPanel *pnDayIdleTime;
    TPanel *pnDayJamTime;
    TPanel *pnDayTotalTime;
    TPanel *pnDayChipUpeh;
    TPanel *pnDayWorkChip;
    TPanel *pnDayLotCount;
    TLabel *Label16;
    TPanel *pnDayFailChip;
    TLabel *Label2;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label33;
    TLabel *Label34;
    TPanel *pnLotNo;
    TPanel *pnLotWorkChip;
    TPanel *pnLotChipUpeh;
    TPanel *pnLotStartTime;
    TPanel *pnLotRunTime;
    TPanel *pnLotFailChip;
    TPanel *pnLotJobFile;
        TPanel *pnTotalTime;
        TLabel *Label11;
        TStringGrid *sgLotInfo;
        TStringGrid *sgDayInfo;
    TPanel *Panel2;
    TPanel *Panel3;
    TGroupBox *GroupBox1;
        TPanel *pnLST;
    TGroupBox *GroupBox5;
    TGroupBox *GroupBox6;
    TLabel *Label15;
    TLabel *Label18;
    TLabel *lbVSNLotNo;
    TLabel *lbPSBLotNo;
        TLabel *Label19;
    TEdit *Edit1;
    TEdit *Edit2;
        TLabel *Label24;
        TLabel *Label39;
        TLabel *lbMgnCntVSN;
        TLabel *Label41;
        TLabel *lbLaserDataVSN;
        TLabel *Label43;
        TLabel *lbMgnCntPSB;
        TLabel *Label45;
        TLabel *lbLaserDataPSB;
        TBitBtn *btAllHome;
        TGroupBox *GroupBox7;
        TGroupBox *GroupBox4;
        TPanel *pnUST;
        TGroupBox *GroupBox8;
        TPanel *pnULD;
        TGroupBox *GroupBox9;
        TGroupBox *GroupBox2;
        TPanel *pnLDR;
        TGroupBox *GroupBox3;
        TLabel *Label13;
        TLabel *Label29;
        TLabel *Label30;
        TLabel *Label35;
        TLabel *Label37;
        TLabel *Label38;
        TPanel *pnSGLeft;
        TGroupBox *GroupBox10;
        TLabel *Label40;
        TLabel *Label42;
        TLabel *Label44;
        TLabel *Label46;
        TLabel *Label47;
        TLabel *Label48;
        TPanel *pnSGRear;
        TGroupBox *GroupBox11;
        TLabel *Label49;
        TLabel *Label50;
        TLabel *Label51;
        TLabel *Label52;
        TLabel *Label53;
        TLabel *Label54;
        TPanel *pnSGRght;
        TGroupBox *GroupBox12;
        TLabel *Label55;
        TLabel *Label56;
        TLabel *Label57;
        TLabel *Label58;
        TLabel *Label59;
        TLabel *Label60;
        TPanel *pnSGFrnt;
        TPanel *pnSRT;
        TPanel *pnZIG;
        TPanel *pnLSP;
        TPanel *pnUSP;
        TImage *Image1;
        TPanel *Panel12;
        TPanel *Panel13;
        TPanel *pnDoor;
        TLabel *Label14;
        TPanel *pnIgnrDoor;
        TPanel *Panel25;
        TLabel *Label36;
        TPanel *pnSkipOST;
        TPanel *Panel23;
        TLabel *Label28;
        TPanel *pnLoadingStop;
        TPanel *Panel17;
        TLabel *Label21;
        TPanel *pnSkipVT;
        TPanel *Panel21;
        TLabel *Label22;
        TPanel *pnSkipVSN1;
        TPanel *Panel20;
        TLabel *Label12;
        TPanel *pnSkipVSN2;
        TTimer *tmUpdate;
        TPageControl *pcManual;
        TTabSheet *TabSheet1;
        TBitBtn *btMan1_1;
        TBitBtn *btMan1_2;
        TBitBtn *btMan1_7;
        TBitBtn *btMan1_3;
        TBitBtn *btMan1_4;
        TBitBtn *btMan1_5;
        TBitBtn *btMan1_6;
        TBitBtn *btMan1_8;
        TBitBtn *btMan1_9;
        TBitBtn *btMan1_10;
        TBitBtn *btMan1_11;
        TBitBtn *btMan1_12;
        TBitBtn *btMan1_13;
        TBitBtn *btMan1_14;
        TTabSheet *TabSheet2;
        TBitBtn *btMan2_1;
        TBitBtn *btMan2_2;
        TBitBtn *btMan2_7;
        TBitBtn *btMan2_3;
        TBitBtn *btMan2_4;
        TBitBtn *btMan2_5;
        TBitBtn *btMan2_6;
        TBitBtn *btMan2_8;
        TBitBtn *btMan2_9;
        TBitBtn *btMan2_10;
        TBitBtn *btMan2_11;
        TBitBtn *btMan2_12;
        TBitBtn *btMan2_13;
        TBitBtn *btMan2_14;
        TTabSheet *TabSheet3;
        TBitBtn *btMan3_1;
        TBitBtn *btMan3_2;
        TBitBtn *btMan3_7;
        TBitBtn *btMan3_3;
        TBitBtn *btMan3_4;
        TBitBtn *btMan3_5;
        TBitBtn *btMan3_6;
        TBitBtn *btMan3_8;
        TBitBtn *btMan3_9;
        TBitBtn *btMan3_10;
        TBitBtn *btMan3_11;
        TBitBtn *btMan3_12;
        TBitBtn *btMan3_13;
        TBitBtn *btMan3_14;
        TTabSheet *TabSheet4;
        TBitBtn *btMan4_1;
        TBitBtn *btMan4_2;
        TBitBtn *btMan4_7;
        TBitBtn *btMan4_3;
        TBitBtn *btMan4_4;
        TBitBtn *btMan4_5;
        TBitBtn *btMan4_6;
        TBitBtn *btMan4_8;
        TBitBtn *btMan4_9;
        TBitBtn *btMan4_10;
        TBitBtn *btMan4_11;
        TBitBtn *btMan4_12;
        TBitBtn *btMan4_13;
        TBitBtn *btMan4_14;
        TTabSheet *TabSheet11;
        TBitBtn *btMan5_1;
        TBitBtn *btMan5_2;
        TBitBtn *btMan5_7;
        TBitBtn *btMan5_3;
        TBitBtn *btMan5_4;
        TBitBtn *btMan5_5;
        TBitBtn *btMan5_6;
        TBitBtn *btMan5_8;
        TBitBtn *btMan5_9;
        TBitBtn *btMan5_10;
        TBitBtn *btMan5_11;
        TBitBtn *btMan5_12;
        TBitBtn *btMan5_13;
        TBitBtn *btMan5_14;
        TPanel *pnManRsltVT;
        TPanel *pnManRsltVS;
        TPanel *pnManRsltOS;
        TPanel *pnSTU;
        TLabel *Label20;
        TPanel *pnOSTRslt;
        TStringGrid *sgOSTPinName;
        TLabel *Label23;
        TCheckBox *cbViewOSTRslt;
        TButton *Button4;
        TPanel *pnRJ1;
        TPanel *pnRJ2;
        TPanel *pnRJ3;
        TPanel *pnRJ4;
        TButton *btClearRj2;
        TButton *btClearRj3;
        TButton *btClearRj4;
        TButton *btClearRj1;
        TLabel *lbRj1;
        TLabel *lbRj2;
        TLabel *lbRj3;
        TLabel *lbRj4;
        TPanel *pnRjName;
        TEdit *edRjName;
        TButton *btRjNameSet;
        TButton *Button6;
        TLabel *Label25;
        TLabel *Label26;
        TLabel *Label27;
        TLabel *Label61;
        TLabel *Label62;
        TLabel *Label63;
        TLabel *Label64;
        TLabel *Label65;
        TLabel *Label66;
        TLabel *Label67;
        TRadioGroup *rgWorkOffset;
        TCheckBox *cbRemote;
        TPanel *pnRemoteControl;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btOperatorClick(TObject *Sender);
    void __fastcall btPasswordCloseClick(TObject *Sender);
    void __fastcall btOperClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall btLotOpenClick(TObject *Sender);
        void __fastcall btStartClick(TObject *Sender);
        void __fastcall btStopClick(TObject *Sender);
        void __fastcall btResetClick(TObject *Sender);
        void __fastcall btLotEndClick(TObject *Sender);
    void __fastcall pnIgnoreTrimDblClick(TObject *Sender);
    void __fastcall btMan1_1Click(TObject *Sender);
    void __fastcall btPR1LotInputClick(TObject *Sender);
        void __fastcall btPR1InputClick(TObject *Sender);
    void __fastcall pnDayRunTimeDblClick(TObject *Sender);
    void __fastcall btPrbClearClick(TObject *Sender);
        void __fastcall btAllHomeClick(TObject *Sender);
        void __fastcall btClearRj1Click(TObject *Sender);
        void __fastcall sgOSTPinNameDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall lbRj1Click(TObject *Sender);
        void __fastcall btRjNameSetClick(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall rgWorkOffsetClick(TObject *Sender);
        void __fastcall Label19Click(TObject *Sender);
        void __fastcall Panel3Click(TObject *Sender);

private:	// User declarations

public:		// User declarations
    __fastcall TFrmOperation(TComponent* Owner);


    //void ChangeArayParent(EN_STG_ANGLE _iAngle) ;

    //bool m_bLotOpenMsgShow;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
#endif
