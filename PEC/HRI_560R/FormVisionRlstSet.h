//---------------------------------------------------------------------------

#ifndef FormVisionRlstSetH
#define FormVisionRlstSetH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormVisionRlstSet : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label80;
    TEdit *edDis_Name1;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TEdit *edCode_Name1;
    TEdit *edPriorty1;
    TPanel *pnColor1;
    TColorDialog *ColorDialog;
    TEdit *edCode_Name2;
    TEdit *edPriorty2;
    TPanel *pnColor2;
    TEdit *edDis_Name3;
    TEdit *edCode_Name3;
    TEdit *edPriorty3;
    TPanel *pnColor3;
    TEdit *edDis_Name4;
    TEdit *edCode_Name4;
    TEdit *edPriorty4;
    TPanel *pnColor4;
    TEdit *edDis_Name5;
    TEdit *edCode_Name5;
    TEdit *edPriorty5;
    TPanel *pnColor5;
    TEdit *edDis_Name6;
    TEdit *edCode_Name6;
    TEdit *edPriorty6;
    TPanel *pnColor6;
    TEdit *edDis_Name7;
    TEdit *edCode_Name7;
    TEdit *edPriorty7;
    TPanel *pnColor7;
    TEdit *edDis_Name8;
    TEdit *edCode_Name8;
    TEdit *edPriorty8;
    TPanel *pnColor8;
    TEdit *edDis_Name9;
    TEdit *edCode_Name9;
    TEdit *edPriorty9;
    TPanel *pnColor9;
    TEdit *edDis_Name10;
    TEdit *edCode_Name10;
    TEdit *edPriorty10;
    TPanel *pnColor10;
    TEdit *edDis_Name11;
    TEdit *edCode_Name11;
    TEdit *edPriorty11;
    TPanel *pnColor11;
    TEdit *edDis_Name12;
    TEdit *edCode_Name12;
    TEdit *edPriorty12;
    TPanel *pnColor12;
    TEdit *edDis_Name13;
    TEdit *edCode_Name13;
    TEdit *edPriorty13;
    TPanel *pnColor13;
    TEdit *edDis_Name14;
    TEdit *edCode_Name14;
    TEdit *edPriorty14;
    TPanel *pnColor14;
        TLabel *Label45;
        TLabel *Label46;
        TLabel *Label47;
        TLabel *Label48;
        TLabel *Label50;
        TLabel *Label51;
        TLabel *Label52;
        TLabel *Label53;
        TLabel *Label54;
        TLabel *Label55;
        TLabel *Label56;
        TLabel *Label57;
        TLabel *Label58;
        TLabel *Label59;
        TBitBtn *btYes;
        TBitBtn *btNo;
        TEdit *edDis_Name2;
        void __fastcall btNoClick(TObject *Sender);
        void __fastcall btYesClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateDevOptn(bool bTable);
public:		// User declarations
    __fastcall TFormVisionRlstSet(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormVisionRlstSet *FormVisionRlstSet;
//---------------------------------------------------------------------------
#endif

