//---------------------------------------------------------------------------

#ifndef FormMasterH
#define FormMasterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ImgList.hpp>
#include "PaixMotion.h"
#include "UtilDefine.h"
//---------------------------------------------------------------------------
class TFrmMaster : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TGroupBox *GroupBox1;
        TPanel *Panel2;
        TPanel *Panel3;
        TPanel *Panel4;
        TPanel *Panel5;
        TPanel *Panel6;
        TPanel *Panel7;
        TPanel *Panel8;
        TPanel *Panel9;
        TPanel *Panel10;
        TPanel *Panel11;
        TPanel *Panel12;
        TPanel *Panel13;
        TPanel *Panel14;
        TComboBox *cbEmg;
        TComboBox *cbEncCount1;
        TComboBox *cbEncZ1;
        TComboBox *cbEncDir1;
        TComboBox *cbNear1;
        TComboBox *cbMLimit1;
        TComboBox *cbPLimit1;
        TComboBox *cbAlarm1;
        TComboBox *cbInpo1;
        TComboBox *cbPulse1;
        TComboBox *cbBusy1;
        TComboBox *cbSvReady1;
        TComboBox *cbStop1;
        TComboBox *cbEncCount2;
        TComboBox *cbEncZ2;
        TComboBox *cbEncDir2;
        TComboBox *cbNear2;
        TComboBox *cbMLimit2;
        TComboBox *cbPLimit2;
        TComboBox *cbAlarm2;
        TComboBox *cbInpo2;
        TComboBox *cbPulse2;
        TComboBox *cbBusy2;
        TComboBox *cbSvReady2;
        TComboBox *cbStop2;
        TGroupBox *GroupBox2;
        TPanel *Panel15;
        TPanel *Panel16;
        TPanel *Panel17;
        TPanel *Panel18;
        TPanel *Panel19;
        TPanel *Panel20;
        TPanel *Panel21;
        TPanel *Panel22;
        TPanel *Panel23;
        TPanel *Panel24;
        TPanel *pnEmg1;
        TPanel *pnBusy1;
        TPanel *pnNear1;
        TPanel *pnEncZ1;
        TPanel *pnHPLimit1;
        TPanel *pnHMLimit1;
        TPanel *pnInpo1;
        TPanel *pnAlarm1;
        TPanel *pnSReady1;
        TPanel *pnHomeDone1;
        TPanel *pnBusy2;
        TPanel *pnNear2;
        TPanel *pnEncZ2;
        TPanel *pnHPLimit2;
        TPanel *pnHMLimit2;
        TPanel *pnInpo2;
        TPanel *pnAlarm2;
        TPanel *pnSReady2;
        TPanel *pnHomeDone2;
        TPanel *pnSPLimit1;
        TPanel *pnSMLimit1;
        TPanel *pnSMLimit2;
        TPanel *pnSPLimit2;
        TGroupBox *GroupBox3;
        TPanel *Panel25;
        TPanel *Panel26;
        TPanel *Panel27;
        TPanel *Panel28;
        TPanel *Panel29;
        TPanel *Panel30;
        TPanel *Panel31;
        TPanel *Panel32;
        TPanel *Panel33;
        TEdit *edOut1;
        TButton *btIncMinus1;
        TButton *btAbsMinus1;
        TButton *btStop1;
        TButton *btJogCCw1;
        TButton *btIncPlus1;
        TButton *btAbsPlus1;
        TButton *btJogCw1;
        TButton *btIncMinus2;
        TButton *btAbsMinus2;
        TButton *btStop2;
        TButton *btJogCCw2;
        TButton *btIncPlus2;
        TButton *btAbsPlus2;
        TButton *btJogCw2;
        TPanel *Panel34;
        TEdit *edOut2;
        TPanel *Panel35;
        TGroupBox *GroupBox4;
        TPanel *Panel36;
        TPanel *Panel37;
        TEdit *edCmdValue1;
        TEdit *edEncValue1;
        TEdit *edCmdValue2;
        TEdit *edEncValue2;
        TGroupBox *GroupBox5;
        TPanel *Panel38;
        TPanel *Panel39;
        TPanel *Panel40;
        TPanel *Panel41;
        TEdit *edStart1;
        TEdit *edAcc1;
        TEdit *edStart2;
        TEdit *edAcc2;
        TPanel *Panel42;
        TPanel *Panel43;
        TEdit *edDec1;
        TEdit *edMax1;
        TEdit *edDec2;
        TEdit *edMax2;
        TButton *btSpdSet1;
        TButton *btScvSet1;
        TPanel *Panel48;
        TEdit *edRatio1;
        TEdit *edRatio2;
        TButton *btSetRatio1;
        TPanel *Panel49;
        TImageList *ImageList1;
        TBitBtn *btCurrent1;
        TBitBtn *btCurrent2;
        TBitBtn *btServo1;
        TBitBtn *btServo2;
        TBitBtn *btAlReset1;
        TBitBtn *btAlReset2;
        TBitBtn *btDCC1;
        TBitBtn *btDCC2;
        TPanel *Panel46;
        TPanel *Panel47;
        TCheckBox *cbStop;
        TTimer *Timer1;
        TButton *btSpdSet2;
        TPanel *Panel44;
        TPanel *Panel50;
        TButton *btScvSet2;
        TButton *btSetRatio2;
        TGroupBox *GroupBox6;
        TPanel *Panel45;
        TEdit *edSpinMLimt;
        TEdit *edTiltMLimt;
        TEdit *edSpinPLimt;
        TEdit *edTiltPLimt;
        TPanel *Panel52;
        TPanel *Panel53;
        TCheckBox *cbSpinLimit;
        TPanel *Panel51;
        TCheckBox *cbTiltLimit;
        TGroupBox *GroupBox7;
        TPanel *pnIn11;
        TPanel *pnIn12;
        TPanel *pnIn13;
        TPanel *pnIn4;
        TPanel *pnIn5;
        TPanel *pnIn6;
        TPanel *pnIn3;
        TPanel *pnIn2;
        TPanel *pnIn1;
        TPanel *pnIn15;
        TPanel *pnIn9;
        TPanel *pnIn14;
        TPanel *pnIn0;
        TPanel *pnIn7;
        TPanel *pnIn8;
        TPanel *pnIn10;
        TPanel *pnOut11;
        TPanel *pnOut12;
        TPanel *pnOut13;
        TPanel *pnOut4;
        TPanel *pnOut5;
        TPanel *pnOut6;
        TPanel *pnOut3;
        TPanel *pnOut2;
        TPanel *pnOut1;
        TPanel *pnOut15;
        TPanel *pnOut9;
        TPanel *pnOut14;
        TPanel *pnOut0;
        TPanel *pnOut7;
        TPanel *pnOut8;
        TPanel *pnOut10;
        TLabel *Label1;
        TLabel *Label2;
        TTimer *Timer2;
        TEdit *edTrgValue2;
        TButton *btSave;
        TPanel *Panel54;
        void __fastcall btIncMinus1Click(TObject *Sender);
        void __fastcall btIncPlus1Click(TObject *Sender);
        void __fastcall btAbsMinus1Click(TObject *Sender);
        void __fastcall btAbsPlus1Click(TObject *Sender);
        void __fastcall btStop1Click(TObject *Sender);
        void __fastcall btJogCCw1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btJogCw1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btJogCCw1MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btCurrent1Click(TObject *Sender);
        void __fastcall SetOutStatus(void);
        void __fastcall GetNmcMotionOut();
        void __fastcall btServo1Click(TObject *Sender);
        void __fastcall btAlReset1Click(TObject *Sender);
        void __fastcall btDCC1Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall btSpdSet1Click(TObject *Sender);
        void __fastcall btSetRatio1Click(TObject *Sender);
        void __fastcall edCmdValue1DblClick(TObject *Sender);
        void __fastcall edEncValue1DblClick(TObject *Sender);
        void __fastcall pnOut0Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Timer2Timer(TObject *Sender);
        void __fastcall btSaveClick(TObject *Sender);
        void __fastcall btScvSet1Click(TObject *Sender);

private:	// User declarations
       Graphics::TBitmap *m_pOnimg  ;
       Graphics::TBitmap *m_pOffimg  ;

private:

    CDelayTimer m_tmTemp   ;



public:		// User declarations
        __fastcall TFrmMaster(TComponent* Owner);
        void __fastcall SetMLimit( int nAxis, bool bOn);
        void __fastcall SetPLimit( int nAxis, bool bOn);
        void __fastcall SetZ( int nAxis, bool bOn);
        void __fastcall SetNear( int nAxis, bool bOn);
        void __fastcall SetInpo( int nAxis, bool bOn);
        void __fastcall SetAlarm( int nAxis, bool bOn);
        void __fastcall SetError( long nError);
        void __fastcall SetBusy( int nAxis, bool bOn);
        void __fastcall SetHome( int nAxis, bool bOn);
        void __fastcall SetReady( int nAxis, bool bOn);

        void __fastcall SetSwPlus(int nId, bool onoff);
        void __fastcall SetSwMinus(int nId, bool onoff);
        void __fastcall SetSpeed(int nAxis);
        void __fastcall SetScvSpeed(int nAxis);

        void __fastcall DisplayInputX ();
        void __fastcall DisplayOutputX();

        //void __fastcall SaveCmnOption();
        void __fastcall UpdateCmnOption(bool _toTable);
        //short m_lInputXIo[128];
        //short m_lOutputXIo[128];

        //NMCAXESMOTIONOUT NmcMotion;

        void __fastcall GetSensorRead(TMessage msg) ;

        BEGIN_MESSAGE_MAP
            MESSAGE_HANDLER(PAIX_SENSORREAD, TMessage, GetSensorRead)
        END_MESSAGE_MAP (TForm)


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
#endif
