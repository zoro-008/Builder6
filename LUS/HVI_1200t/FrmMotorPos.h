//---------------------------------------------------------------------------

#ifndef FrmMotorPosH
#define FrmMotorPosH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
#include "UtilDefine.h"
//---------------------------------------------------------------------------
//Motr Window Cylinder Window Frame Unit Refernce
//---------------------------------------------------------------------------
#include "FlmMotr.h"
#include "FlmCyl.h"
#include "UserFile.h"
//---------------------------------------------------------------------------
//#define MAX_MTPOSCYL 38
class TFormMotorPos : public TForm
{
__published:	// IDE-managed Components
    TPageControl *PageControl;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet3;
        TTabSheet *TabSheet5;
    TGroupBox *GroupBox5;
    TGroupBox *GroupBox1;
    TGroupBox *GroupBox2;
        TBitBtn *btSaveDevice;
        TPanel *pnPs_WRK_XVsn;
        TTabSheet *TabSheet2;
        TGroupBox *GroupBox6;
        TGroupBox *GroupBox11;
        TTimer *tmUpdateTimer;
        TTimer *Timer1;
        TPanel *pnMt_WRK_XVsn;
        TGroupBox *GroupBox14;
        TPanel *pnWK1_SttnUpDn;
        TGroupBox *GroupBox15;
        TPanel *pnWK1_StprUpDn;
        TGroupBox *GroupBox16;
        TPanel *pnWK2_SttnUpDn;
        TGroupBox *GroupBox17;
        TPanel *pnWK1_AlgnFwBw;
        TGroupBox *GroupBox19;
        TPanel *pnWK2_AlgnFwBw;
        TGroupBox *GroupBox20;
        TPanel *pnWK2_StprUpDn;
        TGroupBox *GroupBox21;
        TPanel *pnWK3_SttnUpDn;
        TGroupBox *GroupBox22;
        TPanel *pnWK3_StprUpDn;
        TGroupBox *GroupBox24;
        TPanel *pnWK3_AlgnFwBw;
        TPanel *pnPs_WRK_YVsn;
        TPanel *pnMt_WRK_YVsn;
        TPanel *pnPs_WR1_XCvr;
        TPanel *pnMt_WR1_XCvr;
        TPanel *pnPs_WR2_XCvr;
        TPanel *pnMt_WR2_XCvr;
        TPanel *pnPs_WR3_XCvr;
        TPanel *pnMt_WR3_XCvr;
        TImage *Image3;
        TGroupBox *GroupBox32;
        TBitBtn *btPSB_AllLiftUp1;
        TBitBtn *btPSB_AllLiftDn1;
        TBitBtn *btWRK_feedingOn1;
        TGroupBox *GroupBox18;
        TPanel *pnWK1_SttnUpDn2;
        TGroupBox *GroupBox23;
        TPanel *pnWK1_StprUpDn2;
        TGroupBox *GroupBox25;
        TPanel *pnWK2_SttnUpDn2;
        TGroupBox *GroupBox26;
        TPanel *pnWK1_AlgnFwBw2;
        TGroupBox *GroupBox27;
        TPanel *pnWK2_AlgnFwBw2;
        TGroupBox *GroupBox28;
        TPanel *pnWK2_StprUpDn2;
        TGroupBox *GroupBox29;
        TPanel *pnWK3_SttnUpDn2;
        TGroupBox *GroupBox30;
        TPanel *pnWK3_StprUpDn2;
        TGroupBox *GroupBox31;
        TPanel *pnWK3_AlgnFwBw2;
        TGroupBox *GroupBox33;
        TBitBtn *btPSB_AllLiftUp2;
        TBitBtn *btPSB_AllLiftDn2;
        TBitBtn *btWRK_feedingOn2;
        TGroupBox *GroupBox34;
        TImage *Image4;
        TPanel *pnPs_PSB_XTrm;
        TPanel *pnMt_PSB_XTrm;
        TGroupBox *GroupBox35;
        TPanel *pnPs_PSB_YTrm;
        TPanel *pnMt_PSB_YTrm;
        TGroupBox *GroupBox3;
        TPanel *pnPSB_IndxUpDn2;
        TGroupBox *GroupBox4;
        TPanel *pnPSB_FlprOpCl2;
        TGroupBox *GroupBox44;
        TPanel *pnPSB_SortFwBw;
        TGroupBox *GroupBox45;
        TPanel *pnPSB_AlgnFwBw2;
        TGroupBox *GroupBox46;
        TPanel *pnPSB_SrtStprFwBw;
        TGroupBox *GroupBox48;
        TBitBtn *btTrmLeftUpPs;
        TBitBtn *btTrmLeftDnPs;
        TBitBtn *btTrmRightUpPs;
        TBitBtn *btTrmRightDnPs;
        TBitBtn *btPSB_TrmYn;
    TBitBtn *btPSB_TrmXn;
        TBitBtn *btPSB_TrmYp;
    TBitBtn *btPSB_TrmXp;
        TTabSheet *TabSheet6;
        TGroupBox *GroupBox49;
        TPanel *pnPs_PSB_XIns;
        TPanel *pnMt_PSB_XIns;
        TGroupBox *GroupBox50;
        TPanel *pnPs_PSB_TFlp;
        TPanel *pnMt_PSB_TFlp;
        TGroupBox *GroupBox51;
        TPanel *pnPSB_IndxUpDn;
        TGroupBox *GroupBox52;
        TPanel *pnPSB_FlprOpCl;
        TGroupBox *GroupBox53;
        TPanel *pnPSB_PshrDnUp;
        TGroupBox *GroupBox54;
        TPanel *pnPSB_AlgnFwBw;
        TGroupBox *GroupBox55;
        TPanel *pnPSB_PshrFwBw;
        TGroupBox *GroupBox57;
        TPanel *pnPSB_PnchDnUp;
        TPanel *pnSpare1;
        TPanel *pnSpare2;
        TImage *Image2;
    TTabSheet *TabSheet7;
    TGroupBox *GroupBox9;
        TPanel *pnPs_ULD_ZCmp;
        TPanel *pnMt_ULD_ZCmp;
    TGroupBox *GroupBox13;
        TPanel *pnPs_ULD_YCmp;
        TPanel *pnMt_ULD_YCmp;
    TGroupBox *GroupBox39;
        TPanel *pnULD_ClmpUpDn;
    TGroupBox *GroupBox7;
        TPanel *pnLDR_ClmpUpDn;
    TGroupBox *GroupBox36;
        TPanel *pnPs_LDR_ZCmp;
        TPanel *pnMt_LDR_ZCmp;
    TGroupBox *GroupBox38;
        TPanel *pnPs_LDR_YCmp;
        TPanel *pnMt_LDR_YCmp;
    TGroupBox *GroupBox42;
        TPanel *pnLDR_PshrFwBw;
    TGroupBox *GroupBox56;
        TBitBtn *btLDR_InAcMotor;
        TBitBtn *btLDR_OutAcMotor;
        TGroupBox *GroupBox8;
        TPanel *pnPRB_StprUpDn;
        TGroupBox *GroupBox10;
        TPanel *pnPSB_GripUpDn;
        TGroupBox *GroupBox12;
        TPanel *pnPSB_GripUpDn2;
        TImage *Image8;
        TImage *Image1;
        TBitBtn *btPRB_feedingOn;
        TGroupBox *GroupBox37;
        TBitBtn *btULD_InAcMotor;
        TBitBtn *btULD_OutAcMotor;
        TComboBox *cbManualAction;
        void __fastcall btSaveDeviceClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall tmUpdateTimerTimer(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall PageControlChange(TObject *Sender);
        void __fastcall btPSB_AllLiftUp1Click(TObject *Sender);
        void __fastcall btWRK_feedingOn2Click(TObject *Sender);
        void __fastcall btPSB_TrmXnClick(TObject *Sender);
        void __fastcall btPSB_TrmYnClick(TObject *Sender);
        void __fastcall btPSB_TrmYpClick(TObject *Sender);
        void __fastcall btPSB_TrmXpClick(TObject *Sender);
        void __fastcall btTrmLeftUpPsClick(TObject *Sender);
        bool __fastcall MotorCheck(EN_MOTR_ID _iMotor) ;
private:	// User declarations
//    void __fastcall UpdateDevOptn(bool bTable);
//    EN_MOTR_ID m_iMotor ;
//    bool bRet1;


public:		// User declarations
    double GetMotrPosTable(EN_MOTR_ID _iMotr , int _iRow , int _iCol);
    TFrameMotr *FrmMotor   [MAX_MOTR];
    TFrameCyl  *FrmCylinder[MAX_ACTR];

    __fastcall TFormMotorPos(TComponent* Owner);
    void __fastcall JobFileBackUp();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMotorPos *FormMotorPos;
//---------------------------------------------------------------------------
#endif

