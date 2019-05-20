//---------------------------------------------------------------------------
#ifndef FormOperationH
#define FormOperationH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
//#include "FormPassword.h"
#include <Graphics.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <jpeg.hpp>
#include "CSPIN.h"
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
#include "FrameMotr.h"
#include "Timer.h"
#include <ImgList.hpp>
class TFrmOperation : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel5;
    TPanel *Panel14;
    TPanel *Panel15;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TPanel *Panel6;
    TPanel *Panel8;
    TLabel *lbDInfo3;
    TLabel *Label1;
    TPanel *Panel9;
    TPanel *pnDayTotalTime;
        TPanel *pnTodayWork;
    TButton *btTotalTmReset;
    TButton *btTotalWkReset;
    TPanel *pnManual;
    TPanel *Panel20;
    TBitBtn *btStart;
    TBitBtn *btRotate;
    TBitBtn *btHome;
    TBitBtn *btReset;
    TTimer *tmShowImg;
    TPanel *Panel7;
        TImage *ImBaseT;
    TPanel *Panel18;
    TPanel *pnBoard2;
    TPanel *pnBoard1;
    TTimer *tmUpdate;
    TShape *spLaser;
        TShape *spBaseDetect;
        TShape *Shape1;
        TShape *Shape2;
        TLabel *Label4;
        TGroupBox *GroupBox1;
        TPanel *plPassWord;
        TPanel *Panel19;
        TLabel *Label17;
        TBitBtn *btPasswordClose;
        TBitBtn *btOper;
        TBitBtn *btMast;
        TBitBtn *btOperator;
        TListBox *lbErr;
        TPanel *pnTest;
        TButton *Button6;
        TEdit *Edit3;
        TEdit *Edit1;
        TButton *Button3;
        TEdit *Edit2;
        TButton *Button2;
        TPanel *pnWorkTime;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *lbBlockNo;
        TLabel *Label9;
        TLabel *Label3;
        TPanel *Panel10;
        TLabel *lbOption2;
        TLabel *Label2;
        TLabel *Label5;
        TPanel *Panel11;
        TPanel *pnDoor;
        TPanel *pnLaser;
        TPanel *pnVaccum;
        TButton *btMasterOptn;
        TPanel *pnMaster;
        TLabel *Label6;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TPanel *pnBrd2Skip;
        TPanel *pnUseRgh;
        TButton *btClose;
        TPanel *pnDryRun;
        TPanel *pnNotUsedVAxis;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall btStopClick(TObject *Sender);
        void __fastcall btResetClick(TObject *Sender);
    void __fastcall btMan1_1Click(TObject *Sender);
    void __fastcall btStartClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall btTotalTmResetClick(TObject *Sender);
    void __fastcall tmShowImgTimer(TObject *Sender);
    void __fastcall btRotateClick(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
        void __fastcall btHomeClick(TObject *Sender);
        void __fastcall btOperClick(TObject *Sender);
        void __fastcall btOperatorClick(TObject *Sender);
        void __fastcall pnDoorClick(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
    void __fastcall btMasterOptnClick(TObject *Sender);
    void __fastcall btCloseClick(TObject *Sender);
    void __fastcall pnBrd2SkipClick(TObject *Sender);

private:	// User declarations
    double dUnit    ;
    double dPitch   ;

    int m_iCount ;

    int    iUnitType;
    void LT_Display(); //Lot Display
    void OP_Dispaly(); //Option Display
    void ER_Display(); //Err List Display.

    void LV_Display(); 
    void UpdateAray();

    CDelayTimer m_tmTemp    ;





public:		// User declarations
    __fastcall TFrmOperation(TComponent* Owner);
    bool __fastcall CheckStripDisappear();
    bool __fastcall CheckStripUnknwon  ();
    AnsiString sOutName;
    int iTemp;

    int m_iLaserStep ;



};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
#endif
