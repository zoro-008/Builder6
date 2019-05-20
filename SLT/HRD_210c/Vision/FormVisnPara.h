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
        TTabSheet *tsOri;
        TTabSheet *tsWfr;
        TTabSheet *tsRlb;
        TTabSheet *tsRla;
        TLabel *Label1;
        TEdit *edOriSinc;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *edWfrWidth;
        TLabel *Label5;
        TEdit *edWfrHeight;
        TLabel *Label7;
        TEdit *edWfrGap;
        TLabel *Label9;
        TEdit *edRlaWidth;
        TLabel *Label11;
        TEdit *edRlaHeight;
        TLabel *Label13;
        TEdit *edRlaGap;
        TLabel *Label15;
        TEdit *edRlbPxCnt;
        TButton *btSave;
        TButton *btClose;
        TLabel *Label6;
        TLabel *Label8;
        TEdit *edRlaAngle;
        TEdit *edRlaAngleGap;
        TLabel *Label10;
        TLabel *Label12;
        TLabel *Label14;
        TEdit *edRlaOffsetX;
        TEdit *edRlaOffsetY;
        TEdit *edRlaOffsetGap;
        TLabel *Label4;
        TLabel *Label17;
        TEdit *edRlbOffsetX;
        TEdit *edRlbOffsetY;
        TGroupBox *GroupBox1;
        TCheckBox *cbWfrFlipX;
        TCheckBox *cbWfrFlipY;
        TCheckBox *cbWfrFlipT;
        TGroupBox *GroupBox2;
        TCheckBox *cbRlbFlipX;
        TCheckBox *cbRlbFlipY;
        TCheckBox *cbRlbFlipT;
        TLabel *Label18;
        TEdit *edWfrStdPxCnt;
        TLabel *Label20;
        TLabel *Label21;
        TEdit *edWfrFCPxCnt;
        TLabel *Label22;
        TLabel *Label23;
        TLabel *Label24;
        TLabel *Label26;
        TLabel *Label27;
        TLabel *Label28;
        TLabel *Label29;
        TLabel *Label30;
        TLabel *Label31;
        TLabel *Label32;
        TLabel *Label33;
        TLabel *Label34;
        TLabel *Label35;
        TLabel *Label36;
        TLabel *Label37;
        TTabSheet *tsWfo;
        TLabel *Label16;
        TLabel *Label19;
        TEdit *edWfoSinc;
        TGroupBox *GroupBox3;
        TCheckBox *cbWfoFlipX;
        TCheckBox *cbWfoFlipY;
        TLabel *Label38;
        TEdit *edWfrCCPxCnt;
        TLabel *Label25;
        TLabel *Label39;
        TEdit *edRlbPkgPxAvr;
        TLabel *Label40;
        TLabel *angle;
        TEdit *edWfrAngle;
        TLabel *Label41;
        TComboBox *cbOriImageSave;
        TLabel *Label42;
        TComboBox *cbWfrImageSave;
        TLabel *Label43;
        TComboBox *cbRlbImageSave;
        TLabel *Label44;
        TComboBox *cbRlaImageSave;
        TLabel *Label45;
        TComboBox *cbWfoImageSave;
        TLabel *Label46;
        TCheckBox *cbRlbNotUseAng;
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
