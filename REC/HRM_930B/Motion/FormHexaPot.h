//---------------------------------------------------------------------------

#ifndef FormHexaPotH
#define FormHexaPotH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TFrmHexaPot : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnManual;
        TBitBtn *btMan1_14;
        TBitBtn *btMan2_14;
        TBitBtn *btMan3_14;
        TBitBtn *btMan4_14;
        TBitBtn *btMan5_14;
        TBitBtn *btMan6_14;
        TImageList *imgDirection;
        TPanel *Panel12;
        TPanel *Panel13;
        TPanel *Panel14;
        TPanel *Panel15;
        TBitBtn *btHexaUCw;
        TBitBtn *btHexaUCcw;
        TBitBtn *btHexaVCcw;
        TBitBtn *btHexaVCw;
        TPanel *Panel16;
        TPanel *Panel17;
        TBitBtn *btHexaXRight;
        TBitBtn *btHexaXLeft;
        TBitBtn *btHexaYFwd;
        TBitBtn *btHexaYBwd;
        TBitBtn *btHexaWCw;
        TBitBtn *btHexaWCcw;
        TBitBtn *btHexaZUp;
        TBitBtn *btHexaZDown;
        TPanel *Panel1;
        TPanel *Panel2;
        TRadioButton *rb1um;
        TRadioButton *rb5um;
        TRadioButton *rb10um;
        TEdit *edHexaPitch;
        TLabel *Label23;
        TLabel *Label24;
        TLabel *Label25;
        TLabel *Label26;
        TLabel *Label27;
        TLabel *Label28;
        TEdit *edEncX;
        TEdit *edEncY;
        TEdit *edEncZ;
        TEdit *edEncU;
        TEdit *edEncV;
        TEdit *edEncW;
        TTimer *tmEnc;
        TLabel *lbErr;
        TPanel *Panel3;
        TPanel *Panel4;
        TBitBtn *btLinearXLeft;
        TBitBtn *btLinearXRight;
        TBitBtn *btLinearYBwd;
        TBitBtn *btLinearYFwd;
        TLabel *Label1;
        TLabel *Label2;
        TEdit *edEncLinearX;
        TEdit *edEncLinearY;
        TButton *btMainMaster;
        TButton *btMainSlave;
        TButton *btSubMaster;
        TButton *btSubSlave;
        TButton *btVisnLive;
        void __fastcall rb1umClick(TObject *Sender);
        void __fastcall rb5umClick(TObject *Sender);
        void __fastcall rb10umClick(TObject *Sender);
        void __fastcall edHexaPitchExit(TObject *Sender);
        void __fastcall btHexaPClick(TObject *Sender);
        void __fastcall btHexaNClick(TObject *Sender);
        void __fastcall tmEncTimer(TObject *Sender);
        void __fastcall btLinearXRightClick(TObject *Sender);
        void __fastcall btLinearXLeftClick(TObject *Sender);
        void __fastcall btLinearYBwdClick(TObject *Sender);
        void __fastcall btLinearYFwdClick(TObject *Sender);
        void __fastcall btMainMasterClick(TObject *Sender);
        void __fastcall btVisnLiveClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmHexaPot(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmHexaPot *FrmHexaPot;
//---------------------------------------------------------------------------
#endif
