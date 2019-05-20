//---------------------------------------------------------------------------

#ifndef FrmDeviceSetH
#define FrmDeviceSetH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TFormDeviceSet : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *btSaveDevice;
        TGroupBox *GroupBox4;
        TImage *Image2;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *Label16;
        TLabel *Label17;
        TStaticText *lbRowPitch;
        TEdit *edRowPitch;
        TStaticText *lbColPitch;
        TEdit *edColPitch;
        TStaticText *lbRowGroupCount;
        TEdit *edRowGroupCount;
        TStaticText *lbColCount;
        TEdit *edColCount;
        TStaticText *lbRowGroupGap;
        TEdit *edRowGroupGap;
        TStaticText *lbColGroupGap;
        TEdit *edColGroupGap;
        TStaticText *lbRowCount;
        TEdit *edRowCount;
        TStaticText *StaticText16;
        TEdit *edColGroupCount;
        TGroupBox *x;
        TImage *Image3;
        TStaticText *StaticText17;
        TEdit *edCsSlotCnt;
        TStaticText *StaticText18;
        TEdit *edCsSlotPitch;
        TGroupBox *GroupBox1;
        TLabel *Label1;
        TLabel *Label2;
        TEdit *edLotEndMgzCnt;
        TEdit *edTrimColCnt;
        TLabel *Label3;
        TLabel *Label4;
        TEdit *ed1IdxReWorkCnt;
    TLabel *Label5;
    TEdit *ed2IdxReWorkCnt;
    TLabel *Label6;
        void __fastcall btSaveDeviceClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateDevInfo(bool bTable);
    void __fastcall UpdateDevOptn(bool bTable);

public:		// User declarations
    __fastcall TFormDeviceSet(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDeviceSet *FormDeviceSet;
//---------------------------------------------------------------------------
#endif

