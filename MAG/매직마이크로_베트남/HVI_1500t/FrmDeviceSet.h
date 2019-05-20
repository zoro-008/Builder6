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
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TFormDeviceSet : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *btSaveDevice;
        TGroupBox *GroupBox4;
        TStaticText *lbRowPitch;
        TEdit *edRowPitch;
        TStaticText *lbColPitch;
        TEdit *edColPitch;
        TStaticText *lbRowGrCnt;
        TEdit *edRowGroupCnt;
        TStaticText *lbColCnt;
        TEdit *edColCnt;
        TStaticText *lbRowGroupGap;
        TEdit *edRowGroupGap;
        TStaticText *lbColGroupGap;
        TEdit *edColGroupGap;
        TStaticText *lbRowCnt;
        TEdit *edRowCnt;
        TStaticText *lbColGrCnt;
        TEdit *edColGroupCnt;
        TGroupBox *x;
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
        TStaticText *lbRowSubGrCnt;
    TEdit *edRowSubGroupCount;
        TStaticText *lbRowSubGroupGap;
    TEdit *edRowSubGroupGap;
        TStaticText *StaticText3;
        TEdit *edColSubGroupCount;
        TEdit *edColSubGroupGap;
        TStaticText *StaticText4;
        TImage *imArray;
        TImage *imArray2;
        TImage *Image1;
        void __fastcall btSaveDeviceClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
    
private:	// User declarations
    void __fastcall UpdateDevInfo(bool bTable);
public:		// User declarations
    __fastcall TFormDeviceSet(TComponent* Owner);
    void __fastcall PaintStrpInfo();
    void __fastcall PaintCassetteInfo();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDeviceSet *FormDeviceSet;
//---------------------------------------------------------------------------
#endif

