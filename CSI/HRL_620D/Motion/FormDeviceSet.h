//---------------------------------------------------------------------------

#ifndef FormDeviceSetH
#define FormDeviceSetH
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
#include "FrameMotr.h"
//---------------------------------------------------------------------------
class TFrmDeviceSet : public TForm
{
__published:	// IDE-managed Components
    TTimer *tmUpdate;
    TPageControl *pcDeviceSet;
    TTabSheet *TabSheet3;
    TPanel *Panel8;
    TPanel *Panel13;
    TPanel *Panel14;
    TPanel *pnBaseT1;
    TPanel *Panel18;
    TPanel *Panel7;
    TPanel *Panel99;
    TPanel *pnBrd1U;
    TEdit *edLsrDev_Brd1U_1st;
    TEdit *edLsrDev_Brd1U_2nd;
    TEdit *edLsrDev_Brd1U_3rd;
    TEdit *edLsrDev_Brd1U_4th;
    TEdit *edLsrDev_Brd1U_5th;
    TCheckBox *cbBrd1U_5th;
    TCheckBox *cbBrd1U_4th;
    TCheckBox *cbBrd1U_3rd;
    TCheckBox *cbBrd1U_2nd;
    TCheckBox *cbBrd1U_1st;
    TPanel *Panel6;
    TPanel *Panel22;
    TGroupBox *GroupBox7;
    TPanel *pnVBrd1;
    TGroupBox *GroupBox6;
    TPanel *pnUBrd1;
    TGroupBox *GroupBox5;
    TPanel *pnTBase1;
    TTabSheet *TabSheet4;
    TPanel *Panel2;
    TPanel *Panel3;
    TGroupBox *GroupBox9;
    TPanel *pnVBrd2;
    TGroupBox *GroupBox8;
    TPanel *pnUBrd2;
    TGroupBox *GroupBox10;
    TPanel *pnTBase2;
    TPanel *Panel12;
    TPanel *Panel26;
    TPanel *Panel4;
    TPanel *Panel16;
    TPanel *pnBaseT2;
    TPanel *Panel17;
    TPanel *Panel21;
    TEdit *edLsrDev_Brd2U_1st;
    TEdit *edLsrDev_Brd2U_2nd;
    TEdit *edLsrDev_Brd2U_3rd;
    TEdit *edLsrDev_Brd2U_4th;
    TEdit *edLsrDev_Brd2U_5th;
    TCheckBox *cbBrd2U_5th;
    TCheckBox *cbBrd2U_4th;
    TCheckBox *cbBrd2U_3rd;
    TCheckBox *cbBrd2U_2nd;
    TCheckBox *cbBrd2U_1st;
    TBitBtn *btSaveDevice;
    TPanel *Panel24;
    TPanel *Panel9;
    TPanel *Panel23;
    TPanel *pnBrd1V;
    TRadioGroup *rgJogUnit1;
    TEdit *edJogUnit1;
    TRadioGroup *rgJogUnit2;
    TEdit *edJogUnit2;
    TGroupBox *GroupBox15;
    TGroupBox *GroupBox16;
    TBitBtn *btStop1;
    TBitBtn *btUCw1;
    TBitBtn *btUCcw1;
    TBitBtn *btVCcw1;
    TBitBtn *btVCw1;
    TCheckBox *cbBrd1U_6th;
    TCheckBox *cbBrd2U_6th;
    TEdit *edLsrDev_Brd1U_6th;
    TEdit *edLsrDev_Brd2U_6th;
    TBitBtn *btVCcw2;
    TBitBtn *btUCcw2;
    TBitBtn *btUCw2;
        TTabSheet *pcImage;
        TGroupBox *GroupBox18;
        TImage *Image2;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TGroupBox *GroupBox11;
        TBitBtn *btBrd2VccL;
        TBitBtn *btBrd2VccR;
        TGroupBox *GroupBox1;
        TBitBtn *btDstCollct2;
        TGroupBox *GroupBox12;
        TBitBtn *btTCw2;
        TGroupBox *GroupBox2;
        TBitBtn *btTCcw2;
        TBitBtn *btStop2;
        TBitBtn *btVCw2;
        TEdit *edLsrDev_Brd2U_7th;
        TEdit *edLsrDev_Brd2U_8th;
        TEdit *edLsrDev_Brd2U_9th;
        TEdit *edLsrDev_Brd2U_10th;
        TCheckBox *cbBrd2U_10th;
        TCheckBox *cbBrd2U_9th;
        TCheckBox *cbBrd2U_8th;
        TCheckBox *cbBrd2U_7th;
        TGroupBox *GroupBox4;
        TBitBtn *btTCcw1;
        TGroupBox *GroupBox14;
        TBitBtn *btTCw1;
        TGroupBox *GroupBox13;
        TBitBtn *btDstCollct1;
        TGroupBox *GroupBox3;
        TBitBtn *btBrd1VccL;
        TBitBtn *btBrd1VccR;
        TEdit *edLsrDev_Brd1U_7th;
        TEdit *edLsrDev_Brd1U_8th;
        TEdit *edLsrDev_Brd1U_9th;
        TEdit *edLsrDev_Brd1U_10th;
        TCheckBox *cbBrd1U_10th;
        TCheckBox *cbBrd1U_9th;
        TCheckBox *cbBrd1U_8th;
        TCheckBox *cbBrd1U_7th;
        TPanel *pnBrd2U;
        TPanel *Panel1;
        TPanel *Panel5;
        TPanel *pnBrd2V;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btSaveDeviceClick(TObject *Sender);
    void __fastcall btBrd1VccLClick(TObject *Sender);
    void __fastcall rgJogUnit1Click(TObject *Sender);
    void __fastcall btStop1Click(TObject *Sender);
    void __fastcall btCcwMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btCcwMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall btCwMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btCwMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall pcDeviceSetChange(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall tmUpdateTimer(TObject *Sender);



private:	// User declarations
    TRadioGroup  * rgUnitUser ;
    TEdit        * edUnitUser ;

    double m_dUnit    ;
    double m_dPitch   ;
    int    m_iUnitType;

    void SetUnitInit();
    void SetUnit(EN_UNIT_TYPE _iUnitType , double _dUnit);
public:		// User declarations
     TFraMotr *FraMotor   [MAX_MOTR];

    __fastcall TFrmDeviceSet(TComponent* Owner);

    void __fastcall MtWindowSet();
    void __fastcall UpdateDevOptn(bool bTo);
    void __fastcall EditEnable();
    int  CheckBoxcheck();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
#endif
