//---------------------------------------------------------------------------

#ifndef FrmVisionRlstSetH
#define FrmVisionRlstSetH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormVisionRlstSet : public TForm
{
__published:	// IDE-managed Components
    TColorDialog *cdSelColor;
    TPanel *Panel1;
    TBitBtn *btYes;
    TBitBtn *btNo;
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TTabSheet *TabSheet2;
        TLabel *Label80;
        TLabel *Label1;
        TLabel *Label3;
        TLabel *Label4;
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
        TLabel *Label2;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TEdit *ediRsltLevel1;
        TPanel *pnRsltColor1;
        TEdit *edsRsltName2;
        TEdit *ediRsltLevel2;
        TPanel *pnRsltColor2;
        TEdit *edsRsltName3;
        TEdit *ediRsltLevel3;
        TPanel *pnRsltColor3;
        TEdit *edsRsltName4;
        TEdit *ediRsltLevel4;
        TPanel *pnRsltColor4;
        TEdit *edsRsltName5;
        TEdit *ediRsltLevel5;
        TPanel *pnRsltColor5;
        TEdit *edsRsltName6;
        TEdit *ediRsltLevel6;
        TPanel *pnRsltColor6;
        TEdit *edsRsltName7;
        TEdit *ediRsltLevel7;
        TPanel *pnRsltColor7;
        TEdit *edsRsltName8;
        TEdit *ediRsltLevel8;
        TPanel *pnRsltColor8;
        TEdit *edsRsltName9;
        TEdit *ediRsltLevel9;
        TPanel *pnRsltColor9;
        TEdit *edsRsltNameA;
        TEdit *ediRsltLevelA;
        TPanel *pnRsltColorA;
        TEdit *edsRsltNameB;
        TEdit *ediRsltLevelB;
        TPanel *pnRsltColorB;
        TEdit *edsRsltNameC;
        TEdit *ediRsltLevelC;
        TPanel *pnRsltColorC;
        TEdit *edsRsltNameD;
        TEdit *ediRsltLevelD;
        TPanel *pnRsltColorD;
        TEdit *edsRsltName1;
        TEdit *edsRsltNameE;
        TEdit *ediRsltLevelE;
        TPanel *pnRsltColorE;
        TEdit *edsRsltNameF;
        TEdit *ediRsltLevelF;
        TPanel *pnRsltColorF;
        TEdit *edsRsltNameG;
        TEdit *ediRsltLevelG;
        TPanel *pnRsltColorG;
        TEdit *edsRsltNameH;
        TEdit *ediRsltLevelH;
        TPanel *pnRsltColorH;
        TEdit *edsRsltNameI;
        TEdit *ediRsltLevelI;
        TPanel *pnRsltColorI;
        TEdit *edsRsltNameJ;
        TEdit *ediRsltLevelJ;
        TPanel *pnRsltColorJ;
        TEdit *edsRsltNameK;
        TEdit *ediRsltLevelK;
        TPanel *pnRsltColorK;
        TEdit *edsRsltNameL;
        TEdit *ediRsltLevelL;
        TPanel *pnRsltColorL;
        TEdit *edSort1;
        TEdit *edSort2;
        TEdit *edSort3;
        TEdit *edSort4;
        TEdit *edSort5;
        TEdit *edSort6;
        TEdit *edSort7;
        TEdit *edSort8;
        TEdit *edSort9;
        TEdit *edSortA;
        TEdit *edSortB;
        TEdit *edSortC;
        TEdit *edSortD;
        TEdit *edSortE;
        TEdit *edSortF;
        TEdit *edSortG;
        TEdit *edSortH;
        TEdit *edSortI;
        TEdit *edSortJ;
        TEdit *edSortK;
        TEdit *edSortL;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *Label20;
        TLabel *Label21;
        TPanel *pnRsltColor22;
        TEdit *Edit2;
        TLabel *Label45;
        TEdit *edsRsltName0;
        TEdit *ediRsltLevel0;
        TPanel *pnRsltColor0;
        TEdit *edSort0;
        TLabel *Label19;
        TPanel *pnRsltColor24;
        TEdit *Edit1;
        TLabel *Label22;
        TPanel *pnRsltColor25;
        TEdit *Edit3;
        TLabel *Label23;
        TPanel *pnRsltColor26;
        TEdit *Edit4;
        TLabel *Label13;
        TPanel *pnRsltColor27;
        TEdit *Edit5;
        void __fastcall btNoClick(TObject *Sender);
        void __fastcall btOkClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
    void __fastcall pnRsltColor0Click(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateCmmOptn(bool bTable);
public:		// User declarations
    __fastcall TFormVisionRlstSet(TComponent* Owner);
    bool  m_bChanged ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormVisionRlstSet *FormVisionRlstSet;
//---------------------------------------------------------------------------
#endif

