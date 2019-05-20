//---------------------------------------------------------------------------

#ifndef FormVisnParaH
#define FormVisnParaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmVisnPara : public TForm
{
__published:	// IDE-managed Components
        TPageControl *pcVisnPara;
        TTabSheet *tsWfr;
        TLabel *Label3;
        TEdit *edWfrWidth;
        TLabel *Label5;
        TEdit *edWfrHeight;
        TLabel *Label7;
        TEdit *edWfrGap;
        TButton *btSave;
        TButton *btClose;
        TGroupBox *GroupBox1;
        TCheckBox *cbWfrFlipX;
        TCheckBox *cbWfrFlipY;
        TCheckBox *cbWfrFlipT;
        TLabel *Label21;
        TLabel *Label22;
        TLabel *Label23;
        TLabel *Label24;
        TLabel *Label26;
        TTabSheet *tsWfo;
        TLabel *Label16;
        TLabel *Label19;
        TEdit *edWfoSinc;
        TGroupBox *GroupBox3;
        TCheckBox *cbWfoFlipX;
        TCheckBox *cbWfoFlipY;
        TLabel *Label40;
        TLabel *angle;
        TEdit *edWfrAngle;
        TLabel *Label41;
        TTabSheet *tsCal;
        TTabSheet *tsWfp;
        TLabel *Label1;
        TComboBox *cbWfrImageSave;
        TLabel *Label2;
        TComboBox *cbWfoImageSave;
        TLabel *Label4;
        TComboBox *cbWfpImageSave;
        TLabel *Label6;
        TComboBox *cbCalImageSave;
        TEdit *edWfpPxCnt;
        TLabel *Label8;
        TGroupBox *GroupBox2;
        TCheckBox *cbWfpFlipX;
        TCheckBox *cbWfpFlipY;
    TLabel *Label38;
    TLabel *Label20;
    TEdit *edWfrFCPxCnt;
    TEdit *edWfrCCPxCnt;
    TEdit *edWfrStdPxCnt;
    TLabel *Label18;
        void __fastcall btCloseClick(TObject *Sender);
        void __fastcall btSaveClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmVisnPara(TComponent* Owner);

        void ApplyPara(bool _bToTable);
        void FormShow (int  _iPage   );
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmVisnPara *FrmVisnPara;
//---------------------------------------------------------------------------
#endif
