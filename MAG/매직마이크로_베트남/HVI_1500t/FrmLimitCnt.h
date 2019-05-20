//---------------------------------------------------------------------------

#ifndef FrmLimitCntH
#define FrmLimitCntH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormLimitCnt : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TBitBtn *btYes;
    TBitBtn *btNo;
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TLabel *lbRsltName0;
        TLabel *Label80;
        TLabel *lbRsltName1;
        TLabel *lbRsltName2;
        TLabel *lbRsltName3;
        TLabel *lbRsltName4;
        TLabel *lbRsltName5;
        TLabel *lbRsltName6;
        TLabel *lbRsltName7;
        TLabel *lbRsltName8;
        TLabel *lbRsltName9;
        TLabel *lbRsltNameA;
        TLabel *lbRsltNameB;
        TLabel *lbRsltNameC;
        TLabel *lbRsltNameD;
        TLabel *lbRsltNameE;
        TLabel *lbRsltNameF;
        TLabel *lbRsltNameG;
        TLabel *lbRsltNameH;
        TLabel *lbRsltNameI;
        TLabel *lbRsltNameJ;
        TLabel *lbRsltNameK;
        TLabel *lbRsltNameL;
        TEdit *edCheckFailCnt;
        TEdit *edCheckFailCnt1;
        TPanel *pnRsltColor1;
        TPanel *Panel22;
        TEdit *edCheckFailCnt2;
        TPanel *pnRsltColor2;
        TPanel *Panel24;
        TEdit *edCheckFailCnt3;
        TPanel *pnRsltColor3;
        TPanel *Panel26;
        TEdit *edCheckFailCnt4;
        TPanel *pnRsltColor4;
        TPanel *Panel28;
        TEdit *edCheckFailCnt5;
        TPanel *pnRsltColor5;
        TPanel *Panel30;
        TEdit *edCheckFailCnt6;
        TPanel *pnRsltColor6;
        TPanel *Panel32;
        TEdit *edCheckFailCnt7;
        TPanel *pnRsltColor7;
        TPanel *Panel34;
        TEdit *edCheckFailCnt8;
        TPanel *pnRsltColor8;
        TPanel *Panel36;
        TEdit *edCheckFailCnt9;
        TPanel *pnRsltColor9;
        TPanel *Panel38;
        TEdit *edCheckFailCnt0;
        TPanel *pnRsltColor0;
        TPanel *Panel40;
        TEdit *edCheckFailCntA;
        TPanel *pnRsltColorA;
        TPanel *Panel42;
        TEdit *edCheckFailCntB;
        TPanel *pnRsltColorB;
        TPanel *Panel44;
        TEdit *edCheckFailCntC;
        TPanel *pnRsltColorC;
        TPanel *Panel46;
        TEdit *edCheckFailCntD;
        TPanel *pnRsltColorD;
        TPanel *Panel48;
        TEdit *edCheckFailCntE;
        TPanel *pnRsltColorE;
        TPanel *Panel50;
        TEdit *edCheckFailCntF;
        TPanel *pnRsltColorF;
        TPanel *Panel52;
        TEdit *edCheckFailCntG;
        TPanel *pnRsltColorG;
        TPanel *Panel54;
        TEdit *edCheckFailCntH;
        TPanel *pnRsltColorH;
        TPanel *Panel56;
        TEdit *edCheckFailCntI;
        TPanel *pnRsltColorI;
        TPanel *Panel58;
        TEdit *edCheckFailCntJ;
        TPanel *pnRsltColorJ;
        TPanel *Panel60;
        TEdit *edCheckFailCntK;
        TPanel *pnRsltColorK;
        TPanel *Panel62;
        TEdit *edCheckFailCntL;
        TPanel *pnRsltColorL;
        TPanel *Panel64;
        void __fastcall btNoClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btYesClick(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateCmmOptn(bool bTable);
public:		// User declarations
    __fastcall TFormLimitCnt(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormLimitCnt *FormLimitCnt;
//---------------------------------------------------------------------------
#endif

