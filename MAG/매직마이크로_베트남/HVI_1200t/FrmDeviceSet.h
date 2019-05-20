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
        TGroupBox *GroupBox29;
        TLabel *Label8;
        TLabel *Label9;
        TEdit *edInsColCnt;
        TStaticText *StaticText30;
        TEdit *edInsRowCnt;
        TStaticText *StaticText31;
        TGroupBox *GroupBox35;
        TLabel *Label10;
        TEdit *edVisnIndexId;
    TLabel *Label1;
    TStaticText *StaticText1;
    TEdit *edRowSubGroupCount;
    TStaticText *StaticText2;
    TEdit *edRowSubGroupGap;
    TGroupBox *GroupBox2;
    TLabel *Label15;
    TLabel *Label2;
    TLabel *Label18;
    TLabel *Label19;
    TCheckBox *cbUsePenMrkMvY;
    TEdit *edPenMrkMvY;
    TCheckBox *cbUsePenMrkMvX;
    TEdit *edPenMrkMvX;
        TStaticText *StaticText3;
        TEdit *edColSubGroupCount;
        TEdit *edColSubGroupGap;
        TStaticText *StaticText4;
        void __fastcall btSaveDeviceClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateDevInfo(bool bTable);
public:		// User declarations
    __fastcall TFormDeviceSet(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDeviceSet *FormDeviceSet;
//---------------------------------------------------------------------------
#endif

